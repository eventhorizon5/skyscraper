#include "OgreOpenXRRenderWindow.h"
#include "OgreRoot.h"

#include "OgreD3D11RenderWindow.h"
#include "OgreD3D11RenderSystem.h"
#include "OgreD3D11Device.h"

#include "OgreOpenXRState.h"
#include "OgreOpenXRViewProjection.h"
#include "OgreOpenXRSwapchain.h"

#include "OgreOpenXRConfig.h"
#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>
#include <openxr/openxr_platform_defines.h>

#include <memory>
#include <winrt/base.h>

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
    OpenXRRenderWindow(RenderSystem* rsys, const char* appName);
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

    bool sessionReady() { return mXrSessionState == XR_SESSION_STATE_READY || XR_SESSION_STATE_FOCUSED; }
    bool shouldRender() { return sessionReady() && mXrFrameState.shouldRender; }

    void ProcessOpenXREvents();
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
  OpenXRRenderWindow::OpenXRRenderWindow(RenderSystem* rsys, const char* appName) :
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

    mXrState->Initialize(appName);
    mXrState->InitializeSystem();
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
    mXrState->initializeSession(mDevice);

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
          viewMatrix[i][j] = spaceToView.r[i].m128_f32[j];
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
          eyeProjectionMatrix[i][j] = projectionMatrix.r[j].m128_f32[i];
        }
      }
      mEyeCameras[k]->setCustomViewMatrix(true, viewMatrix);
      mEyeCameras[k]->setCustomProjectionMatrix(true, eyeProjectionMatrix);
      mEyeCameras[k]->setFarClipDistance(3000);
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
}

Ogre::RenderWindow* CreateOpenXRRenderWindow(Ogre::RenderSystem* rsys, const char* appName)
{
  Ogre::OpenXRRenderWindow* xrRenderWindow = new Ogre::OpenXRRenderWindow(rsys, appName);
  rsys->attachRenderTarget(*xrRenderWindow);
  return xrRenderWindow;
}
