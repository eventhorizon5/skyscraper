
#include <d3d11.h>
#include "OgreOpenXRSwapchain.h"
#include "OgreOpenXRState.h"
#include "OgreOpenXRViewProjection.h"

SwapchainD3D11 CreateSwapchainD3D11(XrSession session,
  DXGI_FORMAT format,
  uint32_t width,
  uint32_t height,
  uint32_t arraySize,
  uint32_t sampleCount,
  XrSwapchainCreateFlags createFlags,
  XrSwapchainUsageFlags usageFlags) {
  SwapchainD3D11 swapchain;
  swapchain.Format = format;
  swapchain.Width = width;
  swapchain.Height = height;
  swapchain.ArraySize = arraySize;

  XrSwapchainCreateInfo swapchainCreateInfo{ XR_TYPE_SWAPCHAIN_CREATE_INFO };
  swapchainCreateInfo.arraySize = arraySize;
  swapchainCreateInfo.format = format;
  swapchainCreateInfo.width = width;
  swapchainCreateInfo.height = height;
  swapchainCreateInfo.mipCount = 1;
  swapchainCreateInfo.faceCount = 1;
  swapchainCreateInfo.sampleCount = sampleCount;
  swapchainCreateInfo.createFlags = createFlags;
  swapchainCreateInfo.usageFlags = usageFlags;

  CHECK_XRCMD(xrCreateSwapchain(session, &swapchainCreateInfo, swapchain.Handle.Put(xrDestroySwapchain)));

  uint32_t chainLength;
  CHECK_XRCMD(xrEnumerateSwapchainImages(swapchain.Handle.Get(), 0, &chainLength, nullptr));

  swapchain.Images.resize(chainLength, { XR_TYPE_SWAPCHAIN_IMAGE_D3D11_KHR });
  CHECK_XRCMD(xrEnumerateSwapchainImages(swapchain.Handle.Get(),
    (uint32_t)swapchain.Images.size(),
    &chainLength,
    reinterpret_cast<XrSwapchainImageBaseHeader*>(swapchain.Images.data())));

  return swapchain;
}

uint32_t AcquireAndWaitForSwapchainImage(XrSwapchain handle) {
  uint32_t swapchainImageIndex;
  XrSwapchainImageAcquireInfo acquireInfo{ XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO };
  CHECK_XRCMD(xrAcquireSwapchainImage(handle, &acquireInfo, &swapchainImageIndex));

  XrSwapchainImageWaitInfo waitInfo{ XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO };
  waitInfo.timeout = XR_INFINITE_DURATION;
  CHECK_XRCMD(xrWaitSwapchainImage(handle, &waitInfo));

  return swapchainImageIndex;
}

Ogre::OpenXRSwapchain::OpenXRSwapchain() : isAcquired(false)
{
}

void Ogre::OpenXRSwapchain::Initialize(OpenXRState* state, OpenXRViewProjection* viewProjection)
{
  XrSystemProperties systemProperties{ XR_TYPE_SYSTEM_PROPERTIES };

  state->GetSystemProperties(systemProperties);

  // Select color and depth swapchain pixel formats.
  const auto [colorSwapchainFormat, depthSwapchainFormat] = state->SelectSwapchainPixelFormats();
  ColorSwapchainPixelFormat = colorSwapchainFormat;
  DepthSwapchainPixelFormat = depthSwapchainFormat;

  // Query and cache view configuration views.
  uint32_t viewCount;
  CHECK_XRCMD(
    xrEnumerateViewConfigurationViews(state->GetInstanceHandle().Get(), state->GetSystemId(), state->primaryViewConfigType, 0, &viewCount, nullptr));
  CHECK(viewCount == 2);

  // Create swapchains with texture array for color and depth images.
  // The texture array has the size of viewCount, and they are rendered in a single pass using VPRT.
  const uint32_t textureArraySize = 1;
  ColorSwapchain =
    CreateSwapchainD3D11(state->GetSession().Get(),
      colorSwapchainFormat,
      viewProjection->getWidth(),
      viewProjection->getHeight(),
      textureArraySize,
      viewProjection->getRecommendedSampleCount(),
      0 /*createFlags*/,
      XR_SWAPCHAIN_USAGE_SAMPLED_BIT | XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT);

  DepthSwapchain =
    CreateSwapchainD3D11(state->GetSession().Get(),
      depthSwapchainFormat,
      viewProjection->getWidth(),
      viewProjection->getHeight(),
      textureArraySize,
      viewProjection->getRecommendedSampleCount(),
      0 /*createFlags*/,
      XR_SWAPCHAIN_USAGE_SAMPLED_BIT | XR_SWAPCHAIN_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

void Ogre::OpenXRSwapchain::AcquireImages()
{
  CHECK(ColorSwapchain.Width == DepthSwapchain.Width);
  CHECK(ColorSwapchain.Height == DepthSwapchain.Height);

  colorSwapchainImageIndex = AcquireAndWaitForSwapchainImage(ColorSwapchain.Handle.Get());
  depthSwapchainImageIndex = AcquireAndWaitForSwapchainImage(DepthSwapchain.Handle.Get());
  isAcquired=true;
}

void Ogre::OpenXRSwapchain::ReleaseImages()
{
  //isAcquired = false;
  if (ColorSwapchain.Handle.Get()) {
    XrSwapchainImageReleaseInfo releaseInfo{ XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO };
    CHECK_XRCMD(xrReleaseSwapchainImage(ColorSwapchain.Handle.Get(), &releaseInfo));
    CHECK_XRCMD(xrReleaseSwapchainImage(DepthSwapchain.Handle.Get(), &releaseInfo));
  }
}

XrRect2Di Ogre::OpenXRSwapchain::getImageRect()
{
  return { {0, 0}, {(int32_t)ColorSwapchain.Width, (int32_t)ColorSwapchain.Height} };
}

ID3D11Texture2D* Ogre::OpenXRSwapchain::getSurface(size_t index) const
{
  return ColorSwapchain.Images[index].texture;
}

XrSwapchain Ogre::OpenXRSwapchain::getColorSwapchain()
{
  return ColorSwapchain.Handle.Get();
}

XrSwapchain Ogre::OpenXRSwapchain::getDepthSwapchain()
{
  return DepthSwapchain.Handle.Get();
}

ID3D11Texture2D* Ogre::OpenXRSwapchain::getColorTexture()
{
  if (!isAcquired) return nullptr;
  return ColorSwapchain.Images[colorSwapchainImageIndex].texture;
}

ID3D11Texture2D* Ogre::OpenXRSwapchain::getDepthTexture()
{
  if (!isAcquired) return nullptr;
  return DepthSwapchain.Images[depthSwapchainImageIndex].texture;
}
