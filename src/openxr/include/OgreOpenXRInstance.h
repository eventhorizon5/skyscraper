#pragma once

#include "OgreOpenXRConfig.h"
#include <openxr/openxr.h>
#include <d3d11.h>
#include <utility>
#include "XrHandle.h"

#include <string>

namespace Ogre {
  class OpenXRInstance {
  public:
    OpenXRInstance();
    ~OpenXRInstance();

    void Initialize(const std::string& applicationName);
    const xr::InstanceHandle& getHandle();
    const bool initialized();

  private:
    xr::InstanceHandle m_instanceHandle;
  };
}