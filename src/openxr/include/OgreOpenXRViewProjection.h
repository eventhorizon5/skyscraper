#pragma once

#include <openxr/openxr.h>
#include <utility>
#include "XrHandle.h"
#include "XrMath.h"
#include <vector>

namespace Ogre {
  class OpenXRState;
  /**
  * Here is where we keep all the stuff relating to the 3d projection of the views.
  */
  class OpenXRViewProjection {
  public:
    OpenXRViewProjection();

    void Initialize(OpenXRState* state);

    void CalculateViewProjections(std::vector<xr::math::ViewProjection>& viewProjections);

    size_t getNumberOfViews() const;

    uint32_t getWidth();
    uint32_t getHeight();
    uint32_t getRecommendedSampleCount();

    void UpdateXrViewInfo(XrViewState& ViewState, OpenXRState* state, XrTime displayTime);

    std::vector<XrCompositionLayerProjectionView> ProjectionLayerViews;
    std::vector<XrCompositionLayerDepthInfoKHR> DepthInfoViews;
  private:

    std::vector<XrView> Views;
    std::vector<XrViewConfigurationView> ConfigViews;
  };
}
#pragma once
