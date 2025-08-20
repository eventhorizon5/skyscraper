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

#include "Ogre.h"
#include "OgreOpenXRState.h"

#include "OgreOpenXRInstance.h"

#include <openxr/openxr_platform_defines.h>
#include <chrono>
#include <thread>

#include "XrUtility/XrMath.h"

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
        LogManager::getSingleton().logMessage("OpenXR: Headset detected");
        break;
      }
      else if (result == XR_ERROR_FORM_FACTOR_UNAVAILABLE) {
        LogManager::getSingleton().logMessage("OpenXR: No headset detected.  Trying again in one second...");
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

    InitializeControllers();
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

  void OpenXRState::InitializeControllers()
  {
    XrInstance instance = m_xrInstance->getHandle().Get();
  
    //create action set
    XrActionSetCreateInfo actionSetInfo{XR_TYPE_ACTION_SET_CREATE_INFO};
    strcpy(actionSetInfo.actionSetName, "gameplay");
    strcpy(actionSetInfo.localizedActionSetName, "Gameplay");
    actionSetInfo.priority = 0;
    xrCreateActionSet(instance, &actionSetInfo, &actionSet);

    {
      //create pose action for left controller
      XrPath leftHandPath;
      xrStringToPath(instance, "/user/hand/left", &leftHandPath);
      XrActionCreateInfo actionInfo{XR_TYPE_ACTION_CREATE_INFO};
      actionInfo.actionType = XR_ACTION_TYPE_POSE_INPUT;
      strcpy(actionInfo.actionName, "left_hand_pose");
      strcpy(actionInfo.localizedActionName, "Left Hand Pose");
      actionInfo.countSubactionPaths = 1;
      XrPath subactionPathsLeft[1] = { leftHandPath };
      actionInfo.subactionPaths = subactionPathsLeft;
      xrCreateAction(actionSet, &actionInfo, &poseActionLeft);

      //create space for left controller
      XrActionSpaceCreateInfo spaceInfo{XR_TYPE_ACTION_SPACE_CREATE_INFO};
      spaceInfo.action = poseActionLeft;
      spaceInfo.subactionPath = leftHandPath;
      spaceInfo.poseInActionSpace.orientation.w = 1.0f; // identity rotation
      xrCreateActionSpace(_sessionHandle.Get(), &spaceInfo, &leftControllerSpace);
    }

    {
      //create pose action for right controller
      XrPath rightHandPath;
      xrStringToPath(instance, "/user/hand/right", &rightHandPath);
      XrActionCreateInfo actionInfo{XR_TYPE_ACTION_CREATE_INFO};
      actionInfo.actionType = XR_ACTION_TYPE_POSE_INPUT;
      strcpy(actionInfo.actionName, "right_hand_pose");
      strcpy(actionInfo.localizedActionName, "Right Hand Pose");
      actionInfo.countSubactionPaths = 1;
      XrPath subactionPathsLeft[1] = { rightHandPath };
      actionInfo.subactionPaths = subactionPathsLeft;
      xrCreateAction(actionSet, &actionInfo, &poseActionRight);

      //create space for right controller
      {
        XrActionSpaceCreateInfo spaceInfo{XR_TYPE_ACTION_SPACE_CREATE_INFO};
        spaceInfo.action = poseActionRight;
        spaceInfo.subactionPath = rightHandPath;
        spaceInfo.poseInActionSpace.orientation.w = 1.0f;
        xrCreateActionSpace(_sessionHandle.Get(), &spaceInfo, &rightControllerSpace);
      }
    }

    //select/trigger button
    {
      XrPath leftHandPath, rightHandPath;
      xrStringToPath(instance, "/user/hand/left", &leftHandPath);
      xrStringToPath(instance, "/user/hand/right", &rightHandPath);
      XrPath subactionPathsBoth[2] = { leftHandPath, rightHandPath };
      XrActionCreateInfo selectActionInfo{XR_TYPE_ACTION_CREATE_INFO};
      selectActionInfo.actionType = XR_ACTION_TYPE_BOOLEAN_INPUT;
      strcpy(selectActionInfo.actionName, "select_action");
      strcpy(selectActionInfo.localizedActionName, "Select Action");
      selectActionInfo.countSubactionPaths = 2;
      selectActionInfo.subactionPaths = subactionPathsBoth;
      XrAction selectAction;
      xrCreateAction(actionSet, &selectActionInfo, &selectAction);
    }

    // Suggest bindings for Meta Quest (Oculus Touch)
    {
      XrPath interactionProfilePath;
      xrStringToPath(instance, "/interaction_profiles/oculus/touch_controller", &interactionProfilePath);

      XrPath leftHandPath, rightHandPath;
      xrStringToPath(instance, "/user/hand/left", &leftHandPath);
      xrStringToPath(instance, "/user/hand/right", &rightHandPath);

      XrPath gripPosePath, triggerClickPath;
      xrStringToPath(instance, "/input/grip/pose", &gripPosePath);
      xrStringToPath(instance, "/input/trigger/click", &triggerClickPath);

      std::vector<XrActionSuggestedBinding> bindings = {
          {poseActionLeft, gripPosePath},
          {poseActionRight, gripPosePath},
          {selectAction, triggerClickPath},
      };

      //attach the action set to the session
      XrInteractionProfileSuggestedBinding suggestedBindings{XR_TYPE_INTERACTION_PROFILE_SUGGESTED_BINDING};
      suggestedBindings.interactionProfile = interactionProfilePath;
      suggestedBindings.suggestedBindings = bindings.data();
      suggestedBindings.countSuggestedBindings = (uint32_t)bindings.size();

      xrSuggestInteractionProfileBindings(instance, &suggestedBindings);

      XrSessionActionSetsAttachInfo attachInfo{XR_TYPE_SESSION_ACTION_SETS_ATTACH_INFO};
      attachInfo.countActionSets = 1;
      attachInfo.actionSets = &actionSet;
      xrAttachSessionActionSets(_sessionHandle.Get(), &attachInfo);
    }
  }
}