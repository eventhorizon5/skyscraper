/*
    Skyscraper 2.1 - OpenXR Instance
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
#include <d3d11.h>
#include <utility>
#include "XrUtility/XrHandle.h"

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
