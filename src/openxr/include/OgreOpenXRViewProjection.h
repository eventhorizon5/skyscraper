/*
    Skyscraper 2.1 - OpenXR View Projection
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

#include <openxr/openxr.h>
#include <utility>
#include "XrUtility/XrHandle.h"
#include "XrUtility/XrMath.h"
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
