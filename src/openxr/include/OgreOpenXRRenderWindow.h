/*
    Skyscraper 2.1 - OpenXR Render Window
    Portions Copyright (C)2024 Ryan Thoryk
    https://www.skyscrapersim.net
    https://sourceforge.net/projects/skyscraper/
    Contact - ryan@skyscrapersim.net
*/

/*
    Original work produced by Glastonbridge Software Limited. This code is provided under the MIT license.
    https://github.com/glastonbridge/OgreOpenXRRenderWindow
*/

#pragma once

#include "Ogre.h"
#include "OgreRenderWindow.h"
#include "OgreRenderSystem.h"
#include "OgreOpenXRConfig.h"

_OgreOpenXRExport Ogre::RenderWindow* CreateOpenXRRenderWindow(Ogre::RenderSystem* rsys);
_OgreOpenXRExport void SetOpenXRParameters(int index, const Ogre::Vector3& position, const Ogre::Quaternion& orientation);
