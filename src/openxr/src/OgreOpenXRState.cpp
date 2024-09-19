#include "OgreOpenXRState.h"

#include "OgreOpenXRInstance.h"

#include <openxr/openxr_platform_defines.h>
#include <chrono>
#include <thread>

#include "XrMath.h"

namespace Ogre {

  OpenXRState::OpenXRState():
    _systemId(XR_NULL_SYSTEM_ID),
    m_xrInstance(new OpenXRInstance)
  {}

  void OpenXRState::Initialize(const std::string& applicationName) {
    m_xrInstance->Initialize(applicationName);
  }

  void OpenXRState::InitializeSystem() {
    CHECK(m_xrInstance->initialized());
    CHECK(_systemId == XR_NULL_SYSTEM_ID);

    XrSystemGetInfo systemInfo{ XR_TYPE_SYSTEM_GET_INFO };
    systemInfo.formFactor = _formFactor;
    while (true) {
      XrResult result = xrGetSystem(m_xrInstance->getHandle().Get(), &systemInfo, &_systemId);
      if (SUCCEEDED(result)) {
        break;
      }
      else if (result == XR_ERROR_FORM_FACTOR_UNAVAILABLE) {
        DEBUG_PRINT("No headset detected.  Trying again in one second...");
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
      }
      else {
        CHECK_XRRESULT(result, "xrGetSystem");
      }

    };

    // Choose an environment blend mode.
    {
      // Query the list of supported environment blend modes for the current system.
      uint32_t count;
      CHECK_XRCMD(xrEnumerateEnvironmentBlendModes(m_xrInstance->getHandle().Get(), _systemId, primaryViewConfigType, 0, &count, nullptr));
      CHECK(count > 0); // A system must support at least one environment blend mode.

      environmentBlendModes.resize(count);
      CHECK_XRCMD(xrEnumerateEnvironmentBlendModes(
        m_xrInstance->getHandle().Get(), _systemId, primaryViewConfigType, count, &count, environmentBlendModes.data()));

      // This sample supports all modes, pick the system's preferred one.
      //m_environmentBlendMode = environmentBlendModes[0];
    }

    // Choosing a reasonable depth range can help improve hologram visual quality.
    // Use reversed-Z (near > far) for more uniform Z resolution.
    // m_nearFar = { 20.f, 0.1f };
  }

  const LUID OpenXRState::GetAdapterLUID() {
    XrGraphicsRequirementsD3D11KHR graphicsRequirements{ XR_TYPE_GRAPHICS_REQUIREMENTS_D3D11_KHR };
    CHECK_XRCMD(
    xrGetD3D11GraphicsRequirementsKHR(m_xrInstance->getHandle().Get(), _systemId, &graphicsRequirements));

    return graphicsRequirements.adapterLuid;
  }

  const xr::InstanceHandle& OpenXRState::GetInstanceHandle()
  {
    return m_xrInstance->getHandle();
  }

  const uint64_t OpenXRState::GetSystemId()
  {
    return _systemId;
  }

  void OpenXRState::GetSystemProperties(XrSystemProperties& systemProperties)
  {
    CHECK_XRCMD(xrGetSystemProperties(m_xrInstance->getHandle().Get(), _systemId, &systemProperties));
  }

  void OpenXRState::initializeSession(D3D11Device& device)
  {
    CHECK(m_xrInstance->initialized());
    GetAdapterLUID();
    XrGraphicsBindingD3D11KHR graphicsBinding{ XR_TYPE_GRAPHICS_BINDING_D3D11_KHR };
    graphicsBinding.device = device.get();

    XrSessionCreateInfo createInfo{ XR_TYPE_SESSION_CREATE_INFO };
    createInfo.next = &graphicsBinding;
    createInfo.systemId = _systemId;
    CHECK_XRCMD(xrCreateSession(m_xrInstance->getHandle().Get(), &createInfo, _sessionHandle.Put(xrDestroySession)));

    XrReferenceSpaceCreateInfo spaceCreateInfo{ XR_TYPE_REFERENCE_SPACE_CREATE_INFO };
    spaceCreateInfo.referenceSpaceType = _appSpaceType;
    spaceCreateInfo.poseInReferenceSpace = xr::math::Pose::Identity();
    CHECK_XRCMD(
      xrCreateReferenceSpace(GetSession().Get(), &spaceCreateInfo, _appSpace.Put(xrDestroySpace))
    );
  }


  std::tuple<DXGI_FORMAT, DXGI_FORMAT> OpenXRState::SelectSwapchainPixelFormats()
  {
    CHECK(_sessionHandle.Get() != XR_NULL_HANDLE);

    // Query the runtime's preferred swapchain formats.
    uint32_t swapchainFormatCount;
    CHECK_XRCMD(xrEnumerateSwapchainFormats(_sessionHandle.Get(), 0, &swapchainFormatCount, nullptr));

    std::vector<int64_t> swapchainFormats(swapchainFormatCount);
    CHECK_XRCMD(xrEnumerateSwapchainFormats(
      _sessionHandle.Get(), (uint32_t)swapchainFormats.size(), &swapchainFormatCount, swapchainFormats.data()));

    // Choose the first runtime-preferred format that this app supports.
    auto SelectPixelFormat = [](const std::vector<int64_t>& runtimePreferredFormats,
      const std::vector<DXGI_FORMAT>& applicationSupportedFormats) {
      auto found = std::find_first_of(std::begin(runtimePreferredFormats),
        std::end(runtimePreferredFormats),
        std::begin(applicationSupportedFormats),
        std::end(applicationSupportedFormats));
      if (found == std::end(runtimePreferredFormats)) {
        THROW("No runtime swapchain format is supported.");
      }
      return (DXGI_FORMAT)*found;
    };

    const static std::vector<DXGI_FORMAT> SupportedColorFormats = {
        DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_FORMAT_B8G8R8A8_UNORM,
        DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
        DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
    };

    const static std::vector<DXGI_FORMAT> SupportedDepthFormats = {
        DXGI_FORMAT_D32_FLOAT,
        DXGI_FORMAT_D16_UNORM,
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        DXGI_FORMAT_D32_FLOAT_S8X24_UINT,
    };

    DXGI_FORMAT colorSwapchainFormat = SelectPixelFormat(swapchainFormats, SupportedColorFormats);
    DXGI_FORMAT depthSwapchainFormat = SelectPixelFormat(swapchainFormats, SupportedDepthFormats);

    return { colorSwapchainFormat, depthSwapchainFormat };
  }

  xr::SessionHandle& OpenXRState::GetSession()
  {
    return _sessionHandle;
  }

  XrEnvironmentBlendMode* OpenXRState::GetEnvironmentBlendModes()
  {
    return environmentBlendModes.data();
  }

  xr::SpaceHandle& OpenXRState::getAppSpace()
  {
    return _appSpace;
  }

}