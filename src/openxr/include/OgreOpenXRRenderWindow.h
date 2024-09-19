#pragma once

#include "OgreRenderWindow.h"
#include "OgreRenderSystem.h"
#include "OgreOpenXRConfig.h"

_OgreOpenXRExport Ogre::RenderWindow* CreateOpenXRRenderWindow(Ogre::RenderSystem* rsys, const char* appName);

