/*
    Skyscraper 2.1 - OpenXR State
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

#pragma once

#include <vector>
#include <string>

#include "RenderSystems/Direct3D11/OgreD3D11Prerequisites.h"

#include "OgreOpenXRConfig.h"

#include "RenderSystems/Direct3D11/OgreD3D11Device.h"

#include <openxr/openxr.h>
#include "XrUtility/XrHandle.h"
#include "XrUtility/XrError.h"
#include "XrUtility/XrDispatchTable.h"
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
    void InitializeControllers();

    //controller data
    XrActionSet actionSet;
    XrSpace leftControllerSpace;
    XrSpace rightControllerSpace;
    XrAction poseActionLeft;
    XrAction poseActionRight;
    XrAction selectAction;
    XrPath leftHandPath;
    XrPath rightHandPath;
    XrAction thumbstickVector;

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
