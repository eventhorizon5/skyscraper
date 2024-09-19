#include "OgreOpenXRViewProjection.h"

#include "OgreOpenXRState.h"
#include "XrError.h"

// holy shit, piiiiiissss
#undef near
#undef far

Ogre::OpenXRViewProjection::OpenXRViewProjection()
{
}

void Ogre::OpenXRViewProjection::Initialize(OpenXRState* state)
{
  uint32_t viewCount = 2;
  ConfigViews.resize(viewCount, { XR_TYPE_VIEW_CONFIGURATION_VIEW });
  CHECK_XRCMD(
    xrEnumerateViewConfigurationViews(
      state->GetInstanceHandle().Get(), state->GetSystemId(), state->primaryViewConfigType, viewCount, &viewCount, ConfigViews.data())
  );

  // Using texture array for better performance, so requiring left/right views have identical sizes.
  const XrViewConfigurationView& view = ConfigViews[0];
  CHECK(ConfigViews[0].recommendedImageRectWidth ==
    ConfigViews[1].recommendedImageRectWidth);
  CHECK(ConfigViews[0].recommendedImageRectHeight ==
    ConfigViews[1].recommendedImageRectHeight);
  CHECK(ConfigViews[0].recommendedSwapchainSampleCount ==
    ConfigViews[1].recommendedSwapchainSampleCount);

  Views.resize(viewCount, { XR_TYPE_VIEW });
  ProjectionLayerViews.resize(viewCount);
  DepthInfoViews.resize(viewCount);
}

/**
* ProjectionLayerViews are used by the endXRFrame layer data.
* 
* TODO: We do not presently seem to consume the depth info.
*/
void Ogre::OpenXRViewProjection::CalculateViewProjections(std::vector<xr::math::ViewProjection>& viewProjections)
{
  float far = 0.1f, near = 20.0f;
  uint32_t viewCount = 2;
  for (uint32_t i = 0; i < viewCount; ++i) {
    viewProjections[i] = { Views[i].pose, Views[i].fov, {near, far} };

    ProjectionLayerViews[i] = { XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW };
    ProjectionLayerViews[i].pose = Views[i].pose;
    ProjectionLayerViews[i].fov = Views[i].fov;
    ProjectionLayerViews[i].subImage.imageArrayIndex = 0;

    DepthInfoViews[i] = { XR_TYPE_COMPOSITION_LAYER_DEPTH_INFO_KHR };
    DepthInfoViews[i].minDepth = 0;
    DepthInfoViews[i].maxDepth = 1;
    DepthInfoViews[i].nearZ = near;
    DepthInfoViews[i].farZ = far;
    DepthInfoViews[i].subImage.imageArrayIndex = i;

    // Chain depth info struct to the corresponding projection layer view's next pointer
    //ProjectionLayerViews[i].next = &DepthInfoViews[i];
  }
}

size_t Ogre::OpenXRViewProjection::getNumberOfViews() const
{
  return Views.size();
}

uint32_t Ogre::OpenXRViewProjection::getHeight()
{
  return ConfigViews[0].recommendedImageRectHeight;
}

uint32_t Ogre::OpenXRViewProjection::getWidth()
{
  return ConfigViews[0].recommendedImageRectWidth;
}

uint32_t Ogre::OpenXRViewProjection::getRecommendedSampleCount()
{
  return ConfigViews[0].recommendedSwapchainSampleCount;
}

void Ogre::OpenXRViewProjection::UpdateXrViewInfo(XrViewState& ViewState, OpenXRState* state, XrTime displayTime)
{
    XrViewLocateInfo viewLocateInfo{ XR_TYPE_VIEW_LOCATE_INFO };
    viewLocateInfo.viewConfigurationType = OpenXRState::primaryViewConfigType;
    viewLocateInfo.displayTime = displayTime;
    viewLocateInfo.space = state->getAppSpace().Get();

    // The output view count of xrLocateViews is always same as xrEnumerateViewConfigurationViews.
    // Therefore, Views can be preallocated and avoid two call idiom here.
    uint32_t viewCapacityInput = (uint32_t)Views.size();
    uint32_t viewCountOutput;
    CHECK_XRCMD(
      xrLocateViews(state->GetSession().Get(),
        &viewLocateInfo,
        &ViewState,
        viewCapacityInput,
        &viewCountOutput,
        Views.data()));

    CHECK(viewCountOutput == viewCapacityInput);
}
