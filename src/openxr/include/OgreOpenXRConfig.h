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
