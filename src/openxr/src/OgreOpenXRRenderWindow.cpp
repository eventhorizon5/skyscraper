/*
    Skyscraper 2.1 - OpenXR Render Window
    Portions Copyright (C)2024 Ryan Thoryk
    MIT license - see LICENSE file
    https://www.skyscrapersim.net
    https://sourceforge.net/projects/skyscraper/
    Contact - ryan@skyscrapersim.net
*/

/*
    Original work produced by Glastonbridge Software Limited. This code is provided under the MIT license.
    https://github.com/glastonbridge/OgreOpenXRRenderWindow
*/

#include "OgreOpenXRRenderWindow.h"
#include "OgreRoot.h"

#include "RenderSystems/Direct3D11/OgreD3D11RenderWindow.h"
#include "RenderSystems/Direct3D11/OgreD3D11RenderSystem.h"
#include "RenderSystems/Direct3D11/OgreD3D11Device.h"

#include "OgreOpenXRState.h"
#include "OgreOpenXRViewProjection.h"
#include "OgreOpenXRSwapchain.h"

#include "OgreOpenXRConfig.h"
#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>
#include <openxr/openxr_platform_defines.h>

#include <memory>
#include <winrt/base.h>

std::vector<Ogre::Vector3> XRPosition;
std::vector<Ogre::Quaternion> XROrientation;

//controller states
static OpenXRControllerState gControllerStates[2] = {};

namespace Ogre {
  class OpenXRState;
  class OpenXRViewProjection;
  class OpenXRSwapchain;

  class _OgreOpenXRExport OpenXRRenderWindow : public D3D11RenderWindowBase {
  public:
    /**
    * The rendersystem must be a D3D11RenderSystem, because we use it to get
    * the D3D11 device, but we don't request it directly so that the D3D11RenderSystem
    * doesn't need to be linked to the consuming project.
    */
    OpenXRRenderWindow(RenderSystem* rsys);
    ~OpenXRRenderWindow();

    /**
    * Ignores everything, yolo
    */
    virtual void create(const String& name, unsigned int width, unsigned int height,
      bool fullScreen, const NameValuePairList* miscParams) override;

    bool requiresTextureFlipping() const { return false; }

    void _beginUpdate() override;
    void _endUpdate() override;
    
    ID3D11Texture2D* getSurface(uint index) const override;

    uint getNumberOfViews() const;
    ID3D11RenderTargetView* getRenderTargetView(uint index) const;
    Viewport* addViewport(Camera* cam, int zOrder = 0, float x = 0.0f, float y = 0.0f, float w = 0.0f, float h = 0.0f) override;

    void setActiveEye(size_t eye);

    // D3D11RenderWindowBase 
    void notifyDeviceLost(D3D11Device* device);
    void notifyDeviceRestored(D3D11Device* device);

  private:
    Ogre::Camera* mEyeCameras[2];
    size_t mNumberOfEyesAdded = 0;
    size_t mActiveEye = 0; // TODO: this window should be two camera-based objects plus a headset manager

    ComPtr<ID3D11Texture2D>         mpBackBuffer;
    winrt::com_ptr<ID3D11RenderTargetView> mRenderTargetViewL;
    winrt::com_ptr<ID3D11RenderTargetView> mRenderTargetViewR;

    std::unique_ptr<OpenXRState> mXrState;
    std::unique_ptr<OpenXRSwapchain> swapchainL;
    std::unique_ptr<OpenXRSwapchain> swapchainR;
    std::unique_ptr<OpenXRViewProjection> mViewProjections;

    XrViewState mXrViewState{ XR_TYPE_VIEW_STATE };

    XrSessionState mXrSessionState;

    XrFrameState mXrFrameState{ XR_TYPE_FRAME_STATE };
    XrCompositionLayerProjection mXrLayer{ XR_TYPE_COMPOSITION_LAYER_PROJECTION };
    std::vector<XrCompositionLayerBaseHeader*> mXrLayers;

    bool sessionReady() {
        return (mXrSessionState == XR_SESSION_STATE_READY) ||
            (mXrSessionState == XR_SESSION_STATE_FOCUSED);
    }
    bool shouldRender() { return sessionReady() && mXrFrameState.shouldRender; }

    void ProcessOpenXREvents();
    void ProcessControllers();
    void _startXrFrame();
    void _endXrFrame();
  };
}
namespace {
  // TODO: This is a nasty-ass way to draw to the correct viewport, hacking it in now
  // but should really set up the two eyes as separate viewports
  class EyeListener : public Ogre::Camera::Listener {
  public:
    EyeListener(Ogre::OpenXRRenderWindow* renderWindow, size_t eyeIndex) :
      m_renderWindow(renderWindow),
      m_eyeIndex(eyeIndex) {

    }
    virtual void cameraPreRenderScene(Ogre::Camera* cam) override
    {
      m_renderWindow->setActiveEye(m_eyeIndex);
    }
  private:
    Ogre::OpenXRRenderWindow* m_renderWindow;
    size_t m_eyeIndex;
  };

}

namespace Ogre {
  OpenXRRenderWindow::OpenXRRenderWindow(RenderSystem* rsys) :
    D3D11RenderWindowBase(static_cast<D3D11RenderSystem*>(rsys)->_getDevice()),
    mXrState(new OpenXRState),
    mViewProjections(new OpenXRViewProjection),
    swapchainL(new OpenXRSwapchain),
    swapchainR(new OpenXRSwapchain)
  {
    mIsFullScreen = true;
    mIsExternal = true;
    mActive = false;
    mSizing = false;
    mHidden = false;
    XRPosition.resize(2);
    XROrientation.resize(2);
    XRPosition[0] = Ogre::Vector3::ZERO;
    XRPosition[1] = Ogre::Vector3::ZERO;
    XROrientation[0] = Ogre::Quaternion::ZERO;
    XROrientation[1] = Ogre::Quaternion::ZERO;
    LogManager::getSingleton().logMessage("\nOpenXR Interface loaded\nOriginal work produced by Glastonbridge Software Limited, MIT license");
    LogManager::getSingleton().logMessage("OpenXR: Initializing...");
  }

  OpenXRRenderWindow::~OpenXRRenderWindow() {

    D3D11RenderSystem* rsys = static_cast<D3D11RenderSystem*>(Root::getSingleton().getRenderSystem());
    rsys->fireDeviceEvent(&mDevice, "RenderWindowDestroyed", this);

    mActive = false;
    mClosed = true;
  }
  void OpenXRRenderWindow::create(const String& name, unsigned int width, unsigned int height,
    bool fullScreen, const NameValuePairList* miscParams)
  {
    D3D11RenderSystem* rsys = static_cast<D3D11RenderSystem*>(Root::getSingleton().getRenderSystem());
    mXrState->Initialize(name);
    mXrState->InitializeSystem();
    mXrState->initializeSession(mDevice);
    mXrState->InitializeControllers();

    mViewProjections->Initialize(mXrState.get());
    mWidth = mViewProjections->getWidth();
    mHeight = mViewProjections->getHeight();
    swapchainL->Initialize(mXrState.get(), mViewProjections.get());
    swapchainR->Initialize(mXrState.get(), mViewProjections.get());

    mActive = true;
  }


  void OpenXRRenderWindow::_beginUpdate()
  {
    RenderTarget::_beginUpdate();
    ProcessOpenXREvents();
    _startXrFrame();

    if (!shouldRender()) return;

    mViewProjections->UpdateXrViewInfo(mXrViewState, mXrState.get(), mXrFrameState.predictedDisplayTime);
    uint32_t viewCount = 2;
 
    swapchainL->AcquireImages();
    swapchainR->AcquireImages();

    const CD3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDescL(D3D11_RTV_DIMENSION_TEXTURE2DARRAY, swapchainL->ColorSwapchainPixelFormat);
    mRenderTargetViewL = nullptr;
    CHECK_HRCMD(
      mDevice->CreateRenderTargetView(swapchainL->getColorTexture(), &renderTargetViewDescL, mRenderTargetViewL.put()));

    const CD3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDescR(D3D11_RTV_DIMENSION_TEXTURE2DARRAY, swapchainR->ColorSwapchainPixelFormat);
    mRenderTargetViewR = nullptr;
    CHECK_HRCMD(
      mDevice->CreateRenderTargetView(swapchainR->getColorTexture(), &renderTargetViewDescR, mRenderTargetViewR.put()));

    std::vector<xr::math::ViewProjection> vps(viewCount);
    mViewProjections->CalculateViewProjections(vps);
    auto imageRect = swapchainL->getImageRect();

    mViewProjections->ProjectionLayerViews[0].subImage.swapchain = swapchainL->getColorSwapchain();
    mViewProjections->ProjectionLayerViews[1].subImage.swapchain = swapchainR->getColorSwapchain();

    mViewProjections->DepthInfoViews[0].subImage.swapchain = swapchainL->getDepthSwapchain();
    mViewProjections->DepthInfoViews[1].subImage.swapchain = swapchainR->getDepthSwapchain();

    for (uint32_t i = 0; i < viewCount; ++i) {
      mViewProjections->ProjectionLayerViews[i].subImage.imageRect = imageRect;
      mViewProjections->DepthInfoViews[i].subImage.imageRect = imageRect;
    }

    auto deviceContext = mDevice.GetImmediateContext();

    const uint32_t viewInstanceCount = (uint32_t)vps.size();

    CD3D11_VIEWPORT viewport(
      (float)imageRect.offset.x, (float)imageRect.offset.y, (float)imageRect.extent.width, (float)imageRect.extent.height);
    deviceContext->RSSetViewports(1, &viewport);

    mpBackBuffer = swapchainR->getColorTexture();

    const bool reversedZ = vps[0].NearFar.Near > vps[0].NearFar.Far;
    const float depthClearValue = reversedZ ? 0.f : 1.f;

    if (mNumberOfEyesAdded != 2) return;

    for (uint32_t k = 0; k < 2; ++k) {
      const DirectX::XMMATRIX spaceToView = xr::math::LoadXrPose(vps[k].Pose);
      Ogre::Quaternion orientation(vps[k].Pose.orientation.w, vps[k].Pose.orientation.x, vps[k].Pose.orientation.y, vps[k].Pose.orientation.z);
      Ogre::Vector3 position(vps[k].Pose.position.x, vps[k].Pose.position.y, vps[k].Pose.position.z);

      Ogre::Affine3 viewMatrix;
      for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
#if OGRE_CPU == OGRE_CPU_ARM
          viewMatrix[i][j] = spaceToView.r[i].n128_f32[j];
#else
          viewMatrix[i][j] = spaceToView.r[i].m128_f32[j];
#endif
        }
      }

      xr::math::NearFar nf = { 0.1, std::numeric_limits<float>::infinity() };

      DirectX::XMMATRIX projectionMatrix = ComposeProjectionMatrix(
        vps[k].Fov,
        nf);
      Ogre::Matrix4 eyeProjectionMatrix;

      // Note the transpose
      for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
#if OGRE_CPU == OGRE_CPU_ARM
          eyeProjectionMatrix[i][j] = projectionMatrix.r[j].n128_f32[i];
#else
          eyeProjectionMatrix[i][j] = projectionMatrix.r[j].m128_f32[i];
#endif
        }
      }
      //mEyeCameras[k]->setCustomViewMatrix(true, viewMatrix);
      //mEyeCameras[k]->setPosition(XRPosition[k] + position);
      mEyeCameras[k]->setPosition(position);
      mEyeCameras[k]->setOrientation(orientation);
      mEyeCameras[k]->setCustomProjectionMatrix(true, eyeProjectionMatrix);
      //mEyeCameras[k]->setFarClipDistance(3000);
      mEyeCameras[k]->setAspectRatio(Ogre::Real(imageRect.extent.height / imageRect.extent.width));
    }
  }

  void OpenXRRenderWindow::_endUpdate()
  {
    RenderTarget::_endUpdate();
    
    if (!shouldRender()) return;

    swapchainL->ReleaseImages();
    swapchainR->ReleaseImages();

    _endXrFrame();
  }

  uint OpenXRRenderWindow::getNumberOfViews() const
  {
    return 1;
  }

  ID3D11Texture2D* OpenXRRenderWindow::getSurface(uint index) const
  {
    return mActiveEye == 0 ? swapchainL->getSurface(0) : swapchainR->getSurface(0);
  }

  ID3D11RenderTargetView* OpenXRRenderWindow::getRenderTargetView(uint index) const
  {
    switch (mActiveEye) {
    case 0: return mRenderTargetViewL.get();
    case 1: return mRenderTargetViewR.get();
    default: return nullptr;
    }
  }

  Viewport* OpenXRRenderWindow::addViewport(Camera* cam, int zOrder, float x, float y, float w, float h)
  {
    if (mNumberOfEyesAdded < 2) {
      mEyeCameras[mNumberOfEyesAdded] = cam;
      cam->addListener(new EyeListener(this, mNumberOfEyesAdded++)); // TODO: Memory leak baby
      return RenderWindow::addViewport(cam, zOrder, x, y, w, h);
    }

    OGRE_EXCEPT(Exception::ERR_INVALID_CALL,
      "You can't have more than two cameras in OpenXR renderer.",
      "OpenXRRenderWindow::addViewport");
  }

  void OpenXRRenderWindow::setActiveEye(size_t eye)
  {
    if (!sessionReady()) return;
    const auto& swapchain = eye == 0 ? swapchainL : swapchainR;
    mpBackBuffer = swapchain->getColorTexture();
    mActiveEye = eye;
  }

  void OpenXRRenderWindow::notifyDeviceLost(D3D11Device* device)
  {
  }
  void OpenXRRenderWindow::notifyDeviceRestored(D3D11Device* device)
  {
  }
  void OpenXRRenderWindow::ProcessOpenXREvents()
  {
    auto pollEvent = [&](XrEventDataBuffer& eventData) -> bool {
      eventData.type = XR_TYPE_EVENT_DATA_BUFFER;
      eventData.next = nullptr;
      return CHECK_XRCMD(
        xrPollEvent(mXrState->GetInstanceHandle().Get(), &eventData)
      ) == XR_SUCCESS;
    };

    XrEventDataBuffer eventData{};
    while (pollEvent(eventData)) {
      switch (eventData.type) {
      case XR_TYPE_EVENT_DATA_INSTANCE_LOSS_PENDING: {
        //*exitRenderLoop = true;
        //*requestRestart = false;
        return;
      }
      case XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED: {
        auto& xrSession = mXrState->GetSession();
        const auto stateEvent = *reinterpret_cast<const XrEventDataSessionStateChanged*>(&eventData);
        CHECK(xrSession.Get() != XR_NULL_HANDLE && xrSession.Get() == stateEvent.session);
        mXrSessionState = stateEvent.state;
        switch (mXrSessionState) {
        case XR_SESSION_STATE_READY: {
          CHECK(xrSession.Get() != XR_NULL_HANDLE);
          XrSessionBeginInfo sessionBeginInfo{ XR_TYPE_SESSION_BEGIN_INFO };
          sessionBeginInfo.primaryViewConfigurationType = OpenXRState::primaryViewConfigType;
          CHECK_XRCMD(xrBeginSession(xrSession.Get(), &sessionBeginInfo));
          break;
        }
        case XR_SESSION_STATE_STOPPING: {
          CHECK_XRCMD(xrEndSession(xrSession.Get()));
          break;
        }
        case XR_SESSION_STATE_EXITING: {
          // TODO:
          // Do not attempt to restart, because user closed this session.
          //*exitRenderLoop = true;
          //*requestRestart = false;
          break;
        }
        case XR_SESSION_STATE_LOSS_PENDING: {
          // TODO:
          // Session was lost, so start over and poll for new systemId.
          //*exitRenderLoop = true;
          //*requestRestart = true;
          break;
        }
        }
        break;
      }
      case XR_TYPE_EVENT_DATA_REFERENCE_SPACE_CHANGE_PENDING:
      case XR_TYPE_EVENT_DATA_INTERACTION_PROFILE_CHANGED:
      default: {
        DEBUG_PRINT("Ignoring event type %d", eventData.type);
        break;
      }
      }
    }
  }


  void OpenXRRenderWindow::_startXrFrame()
  {
    mXrLayers.clear();
    if (!sessionReady()) return;

    auto& session = mXrState->GetSession();

    CHECK(session.Get() != XR_NULL_HANDLE);

    XrFrameWaitInfo frameWaitInfo{ XR_TYPE_FRAME_WAIT_INFO };
    CHECK_XRCMD(xrWaitFrame(session.Get(), &frameWaitInfo, &mXrFrameState));

    XrFrameBeginInfo frameBeginInfo{ XR_TYPE_FRAME_BEGIN_INFO };
    CHECK_XRCMD(xrBeginFrame(session.Get(), &frameBeginInfo));

    if (!mXrFrameState.shouldRender) return;

    // The projection layer consists of projection layer views.

    // Inform the runtime that the app's submitted alpha channel has valid data for use during composition.
    // The primary display on HoloLens has an additive environment blend mode. It will ignore the alpha channel.
    // However, mixed reality capture uses the alpha channel if this bit is set to blend content with the environment.
    mXrLayer = { XR_TYPE_COMPOSITION_LAYER_PROJECTION };
    mXrLayer.layerFlags = XR_COMPOSITION_LAYER_BLEND_TEXTURE_SOURCE_ALPHA_BIT;
    mXrLayers.push_back(reinterpret_cast<XrCompositionLayerBaseHeader*>(&mXrLayer));

    ProcessControllers();

  }

  void OpenXRRenderWindow::_endXrFrame()
  {
    mXrLayer.space = mXrState->getAppSpace().Get();
    mXrLayer.viewCount = (uint32_t) mViewProjections->ProjectionLayerViews.size();
    mXrLayer.views = mViewProjections->ProjectionLayerViews.data();

    XrFrameEndInfo frameEndInfo{ XR_TYPE_FRAME_END_INFO };
    frameEndInfo.displayTime = mXrFrameState.predictedDisplayTime;
    frameEndInfo.environmentBlendMode = *mXrState->GetEnvironmentBlendModes();
    frameEndInfo.layerCount = mXrLayers.size();
    frameEndInfo.layers = mXrLayers.data();

    CHECK_XRCMD(xrEndFrame(mXrState->GetSession().Get(), &frameEndInfo));
  }

  void OpenXRRenderWindow::ProcessControllers()
  {
      //sync input actions
      XrActiveActionSet activeActionSet{};
      activeActionSet.actionSet = mXrState->actionSet;

      XrActionsSyncInfo syncInfo{XR_TYPE_ACTIONS_SYNC_INFO};
      syncInfo.countActiveActionSets = 1;
      syncInfo.activeActionSets = &activeActionSet;

      xrSyncActions(mXrState->GetSession().Get(), &syncInfo);

      //poll left controller pose
      XrSpaceLocation leftLocation{XR_TYPE_SPACE_LOCATION};
      xrLocateSpace(mXrState->leftControllerSpace, mXrState->getAppSpace().Get(), mXrFrameState.predictedDisplayTime, &leftLocation);

      if ((leftLocation.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT) &&
        (leftLocation.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT))
      {
          // Extract left controller pose
          XrPosef pose = leftLocation.pose;
          // Convert XrPosef to your math type and use it
      }

      //poll right controller pose
      XrSpaceLocation rightLocation{XR_TYPE_SPACE_LOCATION};
      xrLocateSpace(mXrState->rightControllerSpace, mXrState->getAppSpace().Get(), mXrFrameState.predictedDisplayTime, &rightLocation);

      if ((rightLocation.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT) &&
        (rightLocation.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT))
      {
          // Extract right controller pose
          XrPosef pose = rightLocation.pose;
          // Convert XrPosef to your math type and use it
      }

      XrActionStateGetInfo getInfo{XR_TYPE_ACTION_STATE_GET_INFO};
      getInfo.action = mXrState->selectAction;

      XrActionStateBoolean state{XR_TYPE_ACTION_STATE_BOOLEAN};
      xrGetActionStateBoolean(mXrState->GetSession().Get(), &getInfo, &state);

      // Left hand
      {
        int handIndex = 0;
        auto& state = gControllerStates[handIndex];
        state.poseValid = false;

        XrSpaceLocation loc{XR_TYPE_SPACE_LOCATION};
        xrLocateSpace(mXrState->leftControllerSpace, mXrState->getAppSpace().Get(), mXrFrameState.predictedDisplayTime, &loc);
        if ((loc.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT) &&
            (loc.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT))
        {
            state.poseValid = true;
            state.position = Ogre::Vector3(loc.pose.position.x, loc.pose.position.y, loc.pose.position.z);
            state.orientation = Ogre::Quaternion(loc.pose.orientation.w, loc.pose.orientation.x, loc.pose.orientation.y, loc.pose.orientation.z);
        }

        // Trigger
        {
            auto& state = gControllerStates[handIndex];
            XrActionStateGetInfo getInfo{XR_TYPE_ACTION_STATE_GET_INFO};
            getInfo.action = mXrState->selectAction;
            getInfo.subactionPath = mXrState->leftHandPath;

            XrActionStateBoolean triggerState{XR_TYPE_ACTION_STATE_BOOLEAN};
            xrGetActionStateBoolean(mXrState->GetSession().Get(), &getInfo, &triggerState);

            state.triggerPressed = (triggerState.isActive && triggerState.currentState);
        }

        // Joystick
        {
            auto& state = gControllerStates[handIndex];
            XrActionStateGetInfo getInfo{XR_TYPE_ACTION_STATE_GET_INFO};
            getInfo.action = mXrState->thumbstickVector; // a VECTOR2F action
            getInfo.subactionPath = mXrState->leftHandPath;

            XrActionStateVector2f axisState{XR_TYPE_ACTION_STATE_VECTOR2F};
            xrGetActionStateVector2f(mXrState->GetSession().Get(), &getInfo, &axisState);

            state.joystickX = axisState.currentState.x;
            state.joystickY = axisState.currentState.y;
        }

        XrActionStateGetInfo getInfo{ XR_TYPE_ACTION_STATE_GET_INFO };
        getInfo.action = mXrState->thumbstickVector;
        getInfo.subactionPath = mXrState->leftHandPath; // or rightHandPath for handIndex 1

        XrActionStateVector2f axisState{ XR_TYPE_ACTION_STATE_VECTOR2F };
        xrGetActionStateVector2f(mXrState->GetSession().Get(), &getInfo, &axisState);

        gControllerStates[0].joystickX = axisState.currentState.x;
        gControllerStates[0].joystickY = axisState.currentState.y;
      }

      // Right hand
      {
        int handIndex = 1;
        auto& state = gControllerStates[handIndex];
        state.poseValid = false;

        XrSpaceLocation loc{XR_TYPE_SPACE_LOCATION};
        xrLocateSpace(mXrState->rightControllerSpace, mXrState->getAppSpace().Get(), mXrFrameState.predictedDisplayTime, &loc);
        if ((loc.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT) &&
            (loc.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT))
        {
            state.poseValid = true;
            state.position = Ogre::Vector3(loc.pose.position.x, loc.pose.position.y, loc.pose.position.z);
            state.orientation = Ogre::Quaternion(loc.pose.orientation.w, loc.pose.orientation.x, loc.pose.orientation.y, loc.pose.orientation.z);
        }

        // Trigger
        {
            XrActionStateGetInfo getInfo{XR_TYPE_ACTION_STATE_GET_INFO};
            getInfo.action = mXrState->selectAction;
            getInfo.subactionPath = mXrState->rightHandPath;

            XrActionStateBoolean triggerState{XR_TYPE_ACTION_STATE_BOOLEAN};
            xrGetActionStateBoolean(mXrState->GetSession().Get(), &getInfo, &triggerState);

            state.triggerPressed = (triggerState.isActive && triggerState.currentState);
        }

        // Joystick
        {
            XrActionStateGetInfo getInfo{XR_TYPE_ACTION_STATE_GET_INFO};
            getInfo.action = mXrState->thumbstickVector; // a VECTOR2F action
            getInfo.subactionPath = mXrState->rightHandPath;

            XrActionStateVector2f axisState{XR_TYPE_ACTION_STATE_VECTOR2F};
            xrGetActionStateVector2f(mXrState->GetSession().Get(), &getInfo, &axisState);

            state.joystickX = axisState.currentState.x;
            state.joystickY = axisState.currentState.y;
        }

        XrActionStateGetInfo getInfo{ XR_TYPE_ACTION_STATE_GET_INFO };
        getInfo.action = mXrState->thumbstickVector;
        getInfo.subactionPath = mXrState->rightHandPath;

        XrActionStateVector2f axisState{ XR_TYPE_ACTION_STATE_VECTOR2F };
        xrGetActionStateVector2f(mXrState->GetSession().Get(), &getInfo, &axisState);

        gControllerStates[1].joystickX = axisState.currentState.x;
        gControllerStates[1].joystickY = axisState.currentState.y;

      }

      if (state.isActive && state.currentState)
      {
          // Trigger/button is pressed
      }

      if (mXrSessionState == XR_SESSION_STATE_FOCUSED)
      {
          XrInteractionProfileState profileState{ XR_TYPE_INTERACTION_PROFILE_STATE };
          char pathStr[XR_MAX_PATH_LENGTH];
          uint32_t length;

          profileState.type = XR_TYPE_INTERACTION_PROFILE_STATE;
          xrGetCurrentInteractionProfile(mXrState->GetSession().Get(), mXrState->leftHandPath, &profileState);
          xrPathToString(mXrState->GetInstanceHandle().Get(), profileState.interactionProfile, XR_MAX_PATH_LENGTH, &length, pathStr);

          LogManager::getSingleton().logMessage("Left hand profile: " + String(pathStr));
      }
  }
}

Ogre::RenderWindow* CreateOpenXRRenderWindow(Ogre::RenderSystem* rsys)
{
  Ogre::OpenXRRenderWindow* xrRenderWindow = new Ogre::OpenXRRenderWindow(rsys);
  rsys->attachRenderTarget(*xrRenderWindow);
  return xrRenderWindow;
}

void SetOpenXRParameters(int index, const Ogre::Vector3& position, const Ogre::Quaternion& orientation)
{
    XRPosition[index] = position;
    XROrientation[index] = orientation;
}

bool GetControllerState(int handIndex, OpenXRControllerState* outState)
{
    if (handIndex < 0 || handIndex > 1 || !outState)
        return false;

    *outState = gControllerStates[handIndex];
    return true;
}
