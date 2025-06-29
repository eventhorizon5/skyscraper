/*
    Skyscraper 2.1 - OpenXR Swapchain
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

#include "OgreOpenXRConfig.h"
#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>
#include <utility>
#include "XrUtility/XrHandle.h"
#include <d3d11.h>
#include <vector>


struct SwapchainD3D11 {
  xr::SwapchainHandle Handle;
  DXGI_FORMAT Format{ DXGI_FORMAT_UNKNOWN };
  uint32_t Width{ 0 };
  uint32_t Height{ 0 };
  uint32_t ArraySize{ 0 };
  std::vector<XrSwapchainImageD3D11KHR> Images;
};

namespace Ogre {
  class OpenXRState;
  class OpenXRViewProjection;

  /**
  * Here is where we keep all the stuff relating to swapchain images, allocation etc.
  */
  class OpenXRSwapchain {
  public:
    OpenXRSwapchain();

    void Initialize(OpenXRState* state, OpenXRViewProjection* viewProjection);

    void AcquireImages();
    void ReleaseImages();
    bool hasImages() {return isAcquired;}

    XrRect2Di getImageRect();
    ID3D11Texture2D* getSurface(size_t index) const;

    XrSwapchain getColorSwapchain();
    XrSwapchain getDepthSwapchain();
    ID3D11Texture2D* getColorTexture();
    ID3D11Texture2D* getDepthTexture();
    DXGI_FORMAT ColorSwapchainPixelFormat;
    DXGI_FORMAT DepthSwapchainPixelFormat;
  private:
    SwapchainD3D11 ColorSwapchain;
    SwapchainD3D11 DepthSwapchain;
    uint32_t colorSwapchainImageIndex;
    uint32_t depthSwapchainImageIndex;
    bool isAcquired;
  };
}
