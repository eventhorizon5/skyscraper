/*
    Skyscraper 2.1 - OpenXR Render Window
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

#include "Ogre.h"
#include "OgreRenderWindow.h"
#include "OgreRenderSystem.h"
#include "OgreVector3.h"
#include "OgreQuaternion.h"
#include "OgreOpenXRConfig.h"

_OgreOpenXRExport Ogre::RenderWindow* CreateOpenXRRenderWindow(Ogre::RenderSystem* rsys);
_OgreOpenXRExport void SetOpenXRParameters(int index, const Ogre::Vector3& position, const Ogre::Quaternion& orientation);

struct OpenXRControllerState {
    bool poseValid;
    Ogre::Vector3 position;
    Ogre::Quaternion orientation;
    bool triggerPressed;
    float joystickX;
    float joystickY;
};

_OgreOpenXRExport bool GetControllerState(int handIndex, OpenXRControllerState* outState);
