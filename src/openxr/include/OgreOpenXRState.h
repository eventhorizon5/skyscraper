#pragma once

#include <vector>
#include <string>

#include "OgreD3D11Prerequisites.h"

#include "OgreOpenXRConfig.h"

#include "OgreD3D11Device.h"

#include <openxr/openxr.h>
#include "XrHandle.h"
#include "XrError.h"
#include "XrDispatchTable.h"
#include <winrt/base.h>
#include <memory>
#include "OgreOpenXRInstance.h"

namespace Ogre {

  /**
  * A container for all the state that is required between Ogre and an OpenXR device, the instance, system, session etc.
  */
  class OpenXRState {
  public:
    OpenXRState();
    void Initialize(const std::string& applicationName);
    void InitializeSystem();
    const LUID GetAdapterLUID();
    xr::SessionHandle& GetSession();
    const xr::InstanceHandle& GetInstanceHandle();
    const uint64_t GetSystemId();
    void GetSystemProperties(XrSystemProperties& systemProperties);
    std::tuple<DXGI_FORMAT, DXGI_FORMAT> SelectSwapchainPixelFormats();
    void initializeSession(D3D11Device& device);
    constexpr static XrViewConfigurationType primaryViewConfigType{ XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO };
    XrEnvironmentBlendMode* GetEnvironmentBlendModes();
    xr::SpaceHandle& getAppSpace();

  private:
    std::unique_ptr<OpenXRInstance> m_xrInstance;
    uint64_t _systemId;
    xr::SessionHandle _sessionHandle;
    std::vector<XrEnvironmentBlendMode> environmentBlendModes;

    constexpr static XrFormFactor _formFactor{ XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY };

    xr::SpaceHandle _appSpace;
    XrReferenceSpaceType _appSpaceType{ XR_REFERENCE_SPACE_TYPE_LOCAL };
  };
}