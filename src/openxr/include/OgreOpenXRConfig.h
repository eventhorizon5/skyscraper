/*
    Skyscraper 2.1 - OpenXR Configuration
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
/**
* OpenXR expects these things to be set in order to retrieve platform-specific info.
* This is not the best place to define this, but it'll do for the time being.
*/
#define XR_USE_GRAPHICS_API_D3D11
#define XR_USE_PLATFORM_WIN32
#define ENABLE_GLOBAL_XR_DISPATCH_TABLE

#if defined(_WIN32)
#   ifdef OGREOPENXRDLL_EXPORTS
#       define _OgreOpenXRExport __declspec(dllexport)
#   else
#       if defined( __MINGW32__ )
#           define _OgreOpenXRExport
#       else
#           define _OgreOpenXRExport __declspec(dllimport)
#       endif
#   endif
#else
#   define _OgreOpenXRExport
#endif
