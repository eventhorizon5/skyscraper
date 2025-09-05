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

static inline Ogre::Quaternion yawFromQuat(const Ogre::Quaternion& q)
{
    //ogre forward is -Z. Project forward to XZ, compute yaw about +Y.
    const Ogre::Vector3 fwd = q * Ogre::Vector3(0,0,-1);
    Ogre::Vector3 f = Ogre::Vector3(fwd.x, 0, fwd.z);
    if (!f.isZeroLength()) f.normalise();
    const Ogre::Radian yaw = Ogre::Math::ATan2(f.x, -f.z);
    return Ogre::Quaternion(yaw, Ogre::Vector3::UNIT_Y);
}

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

    Ogre::Quaternion mBodyYaw = Ogre::Quaternion::IDENTITY;
    Ogre::Vector3    mBodyPos = Ogre::Vector3::ZERO;
    Ogre::Quaternion mHeadYaw = Ogre::Quaternion::IDENTITY;
    XrTime mPrevDisplayTime = 0; // to compute dt

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

    //refresh OpenXR view info for this predicted display time
    mViewProjections->UpdateXrViewInfo(mXrViewState, mXrState.get(), mXrFrameState.predictedDisplayTime);

    const uint32_t viewCount = 2;

    //acquire swapchain images and build RTVs
    swapchainL->AcquireImages();
    swapchainR->AcquireImages();

    const CD3D11_RENDER_TARGET_VIEW_DESC rtvDescL(D3D11_RTV_DIMENSION_TEXTURE2DARRAY, swapchainL->ColorSwapchainPixelFormat);
    mRenderTargetViewL = nullptr;
    CHECK_HRCMD(mDevice->CreateRenderTargetView(swapchainL->getColorTexture(), &rtvDescL, mRenderTargetViewL.put()));

    const CD3D11_RENDER_TARGET_VIEW_DESC rtvDescR(D3D11_RTV_DIMENSION_TEXTURE2DARRAY, swapchainR->ColorSwapchainPixelFormat);
    mRenderTargetViewR = nullptr;
    CHECK_HRCMD(mDevice->CreateRenderTargetView(swapchainR->getColorTexture(), &rtvDescR, mRenderTargetViewR.put()));

    //default to left eye at frame start so Ogre has a valid target before the listener switches per camera.
    setActiveEye(0);  // sets mpBackBuffer to left’s color texture under the hood

    //compute views/projections for both eyes
    std::vector<xr::math::ViewProjection> vps(viewCount);
    mViewProjections->CalculateViewProjections(vps);
    const auto imageRect = swapchainL->getImageRect();

    //cache HMD yaw (use either eye’s orientation; they are the same)
    const Ogre::Quaternion hmdOriCenter(
        (Ogre::Real)vps[0].Pose.orientation.w,
        (Ogre::Real)vps[0].Pose.orientation.x,
        (Ogre::Real)vps[0].Pose.orientation.y,
        (Ogre::Real)vps[0].Pose.orientation.z);

    //convert to yaw-only so pitch/roll don’t affect movement
    mHeadYaw = yawFromQuat(hmdOriCenter);

    //compute head-center in app space from the two views (positions only)
    const Ogre::Vector3 eyePosL(
        (Ogre::Real)vps[0].Pose.position.x,
        (Ogre::Real)vps[0].Pose.position.y,
        (Ogre::Real)vps[0].Pose.position.z);

    const Ogre::Vector3 eyePosR(
        (Ogre::Real)vps[1].Pose.position.x,
        (Ogre::Real)vps[1].Pose.position.y,
        (Ogre::Real)vps[1].Pose.position.z);

    //center of head (between eyes) in app space
    const Ogre::Vector3 headCenterApp = (eyePosL + eyePosR) * 0.5f;

    //world transform parts
    const Ogre::Quaternion worldFromApp_R = mBodyYaw;     //rig yaw
    const Ogre::Vector3    worldFromApp_T = mBodyPos;     //accumulated locomotion (in world space)

    //wire the layer views to the correct swapchains & rects
    mViewProjections->ProjectionLayerViews[0].subImage.swapchain = swapchainL->getColorSwapchain();
    mViewProjections->ProjectionLayerViews[1].subImage.swapchain = swapchainR->getColorSwapchain();
    mViewProjections->DepthInfoViews[0].subImage.swapchain      = swapchainL->getDepthSwapchain();
    mViewProjections->DepthInfoViews[1].subImage.swapchain      = swapchainR->getDepthSwapchain();
    for (uint32_t i = 0; i < viewCount; ++i) {
        mViewProjections->ProjectionLayerViews[i].subImage.imageRect = imageRect;
        mViewProjections->DepthInfoViews[i].subImage.imageRect       = imageRect;
    }

    //D3D viewport & backbuffer
    auto* deviceContext = mDevice.GetImmediateContext();
    CD3D11_VIEWPORT vp(
        static_cast<float>(imageRect.offset.x),
        static_cast<float>(imageRect.offset.y),
        static_cast<float>(imageRect.extent.width),
        static_cast<float>(imageRect.extent.height));
    deviceContext->RSSetViewports(1, &vp);

    if (mNumberOfEyesAdded != 2) return;

    //helpers to convert & rotate for the submitted layer poses
    const auto toXr = [](const Ogre::Quaternion& q){
        return XrQuaternionf{ (float)q.x, (float)q.y, (float)q.z, (float)q.w };
    };
    const auto qmul = [](const XrQuaternionf& a, const XrQuaternionf& b){
        // a * b  (OpenXR order: x,y,z,w)
        return XrQuaternionf{
            a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y,
            a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x,
            a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w,
            a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z
        };
    };
    const auto vrot = [&](const Ogre::Quaternion& q, const XrVector3f& v)->XrVector3f{
        Ogre::Vector3 ov((Ogre::Real)v.x,(Ogre::Real)v.y,(Ogre::Real)v.z);
        Ogre::Vector3 ro = q * ov; // rotate by yaw
        return XrVector3f{ (float)ro.x,(float)ro.y,(float)ro.z };
    };

    //drive the Ogre cameras: apply same yaw to orientation and position
    for (uint32_t k = 0; k < viewCount; ++k) {
        const auto& vpk = vps[k];

        //raw HMD per-eye pose (already includes IPD)
        const Ogre::Quaternion hmdOri(
            (Ogre::Real)vpk.Pose.orientation.w,
            (Ogre::Real)vpk.Pose.orientation.x,
            (Ogre::Real)vpk.Pose.orientation.y,
            (Ogre::Real)vpk.Pose.orientation.z);
        const Ogre::Vector3 hmdPos(
            (Ogre::Real)vpk.Pose.position.x,
            (Ogre::Real)vpk.Pose.position.y,
            (Ogre::Real)vpk.Pose.position.z);

        //eye position in app space (like above for L/R):
        const Ogre::Vector3 eyePosApp(
            (Ogre::Real)vpk.Pose.position.x,
            (Ogre::Real)vpk.Pose.position.y,
            (Ogre::Real)vpk.Pose.position.z);

        //rotate *around the head center*, then translate the center
        const Ogre::Vector3 eyeOffsetApp = eyePosApp - headCenterApp;
        const Ogre::Vector3 finalPos =
            (worldFromApp_R * eyeOffsetApp) +        // rotate the eye offset
            (worldFromApp_R * headCenterApp) +       // rotate the center
            worldFromApp_T;                          // then add locomotion

        //const Ogre::Quaternion finalOri = mBodyYaw * hmdOri;
        const Ogre::Quaternion finalOri = worldFromApp_R * hmdOri;
        //const Ogre::Vector3    finalPos = (mBodyYaw * hmdPos) + mBodyPos;

        //build Ogre projection from OpenXR FOV
        xr::math::NearFar nf = { 0.1f, std::numeric_limits<float>::infinity() };
        DirectX::XMMATRIX projM = ComposeProjectionMatrix(vpk.Fov, nf);

        Ogre::Matrix4 ogreProj;
        for (size_t i = 0; i < 4; ++i) {
            for (size_t j = 0; j < 4; ++j) {
#if OGRE_CPU == OGRE_CPU_ARM
                ogreProj[i][j] = (Ogre::Real)projM.r[j].n128_f32[i];
#else
                ogreProj[i][j] = (Ogre::Real)projM.r[j].m128_f32[i];
#endif
            }
        }

        //set camera ONCE (don’t overwrite later)
        mEyeCameras[k]->setPosition(finalPos);
        mEyeCameras[k]->setOrientation(finalOri);
        mEyeCameras[k]->setCustomProjectionMatrix(true, ogreProj);
        mEyeCameras[k]->setAspectRatio(
            Ogre::Real(imageRect.extent.width) / Ogre::Real(imageRect.extent.height));
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
          //extract left controller pose
          XrPosef pose = leftLocation.pose;
          //convert XrPosef to your math type and use it
      }

      //poll right controller pose
      XrSpaceLocation rightLocation{XR_TYPE_SPACE_LOCATION};
      xrLocateSpace(mXrState->rightControllerSpace, mXrState->getAppSpace().Get(), mXrFrameState.predictedDisplayTime, &rightLocation);

      if ((rightLocation.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT) &&
        (rightLocation.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT))
      {
          //extract right controller pose
          XrPosef pose = rightLocation.pose;
          //convert XrPosef to your math type and use it
      }

      XrActionStateGetInfo getInfo{XR_TYPE_ACTION_STATE_GET_INFO};
      getInfo.action = mXrState->selectAction;

      XrActionStateBoolean state{XR_TYPE_ACTION_STATE_BOOLEAN};
      xrGetActionStateBoolean(mXrState->GetSession().Get(), &getInfo, &state);

      //left hand
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

        //trigger
        {
            auto& state = gControllerStates[handIndex];
            XrActionStateGetInfo getInfo{XR_TYPE_ACTION_STATE_GET_INFO};
            getInfo.action = mXrState->selectAction;
            getInfo.subactionPath = mXrState->leftHandPath;

            XrActionStateBoolean triggerState{XR_TYPE_ACTION_STATE_BOOLEAN};
            xrGetActionStateBoolean(mXrState->GetSession().Get(), &getInfo, &triggerState);

            state.triggerPressed = (triggerState.isActive && triggerState.currentState);
        }

        //joystick
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

      //right hand
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

        //trigger
        {
            XrActionStateGetInfo getInfo{XR_TYPE_ACTION_STATE_GET_INFO};
            getInfo.action = mXrState->selectAction;
            getInfo.subactionPath = mXrState->rightHandPath;

            XrActionStateBoolean triggerState{XR_TYPE_ACTION_STATE_BOOLEAN};
            xrGetActionStateBoolean(mXrState->GetSession().Get(), &getInfo, &triggerState);

            state.triggerPressed = (triggerState.isActive && triggerState.currentState);
        }

        //joystick
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

      //integrate smooth body yaw from right stick X
      double dt = 0.0;
      if (mPrevDisplayTime != 0)
          dt = double(mXrFrameState.predictedDisplayTime - mPrevDisplayTime) / 1e9;
      mPrevDisplayTime = mXrFrameState.predictedDisplayTime;

      const float deadzone = 0.15f;
      const float turnRateDegPerSec = 120.0f; // tune to taste
      float rx = gControllerStates[1].joystickX; // right-hand stick X

      if (std::fabs(rx) > deadzone && dt > 0.0) {
          float yawDeg = -rx * turnRateDegPerSec * float(dt);
          mBodyYaw = Ogre::Quaternion(Ogre::Degree(yawDeg), Ogre::Vector3::UNIT_Y) * mBodyYaw;
          mBodyYaw.normalise();
      }

      //left stick drives movement (x=strafe, y=forward)
      float lx = gControllerStates[0].joystickX;
      float ly = gControllerStates[0].joystickY;

      //deadzone & scaling
      const float dz = 0.15f;
      if (std::fabs(lx) < dz) lx = 0.f;
      if (std::fabs(ly) < dz) ly = 0.f;

      //local (camera) movement vector: X = right, Z = forward(-)
      Ogre::Vector3 localMove(lx, 0.f, -ly);

      Ogre::Quaternion heading = mBodyYaw * mHeadYaw;

      //rotate local vector into world/app space
      Ogre::Vector3 worldMove = heading * localMove;

      //speed (meters/second) and integration
      const float speed = 3.0f; // tune
      if (dt > 0.0) {
          mBodyPos += worldMove * (speed * (float)dt);
      }

      if (state.isActive && state.currentState)
      {
          //trigger/button is pressed
      }

      if (mXrSessionState == XR_SESSION_STATE_FOCUSED)
      {
          XrInteractionProfileState ps{ XR_TYPE_INTERACTION_PROFILE_STATE };
          XrResult pr = xrGetCurrentInteractionProfile(mXrState->GetSession().Get(), mXrState->leftHandPath, &ps);
          if (XR_SUCCEEDED(pr) && ps.interactionProfile != XR_NULL_PATH) {
              char s[XR_MAX_PATH_LENGTH] = {};
              uint32_t len = 0;
              xrPathToString(mXrState->GetInstanceHandle().Get(), ps.interactionProfile, XR_MAX_PATH_LENGTH, &len, s);
              //LogManager::getSingleton().logMessage("Left hand profile: " + Ogre::String(s));
          }
          else {
              //LogManager::getSingleton().logMessage("Left hand profile: (none) r=0x" + Ogre::StringConverter::toString(pr));
          }
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
