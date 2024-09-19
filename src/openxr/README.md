# An OpenXR Render Window for OGRE 3D

## About

This has been tested working on Valve Index. This appears to be broken on
Windows Mixed Reality due to the framebuffers being allocated in a different
order.

This was made as part of a prototype development for a VR sculpting application
based on an existing OGRE 3D app. It is an exploration, not a production-ready
library.

I have implemented the plugin as a RenderWindow with some helper files that can
be run inside Ogre's Direct3D11 RenderSystem. Some of the tasks should really
belong to a RenderSystem, but extending a RenderSystem is a bigger job and
keeping the new logic in one place makes it easier to understand what the work
actually is.

As it stands, you can treat this as a starting point for implementing OpenXR
in your project. Glastonbridge does not provide community support for this
library.

Gotchas:

- assumes that Ogre and OpenXR are going to request the same Direct3D adapter
- doesn't report OpenXR lifecycle events back to Ogre
- requires a separate primary window
- it's a hack made to get results in a short timeframe

## Installation

You will need the OGRE classic (i.e. not `ogre-next`) SDK, possibly the source
code. You will also need the OpenXR loader, which I obtained from
https://github.com/KhronosGroup/OpenXR-SDK

Because it is dependent on the D3D11RenderSystem, it needs to link against the
lib for that RenderSystem (also the openxr_loader), which breaks the nice neat
plugin framework that Ogre provides. Instead, you can use the source to build
a DLL (see [[src/OgreOpenXRConfig.h]]) that links to it, so your project just
needs to link against the DLL. This may have been overthinking it, IDK.

## Usage

In your application's code, you create an OpenXR RenderWindow like so

```
#include "OgreOpenXRRenderWindow.h"

...

  xrRenderWindow = CreateOpenXRRenderWindow(root->getRenderSystem(), "VR Crafting");
  xrRenderWindow->create("VR Crafting", 0,0,true,nullptr);
```

Then you need to attach exactly two cameras to it (no surprises).
These cameras will then be entirely controlled by the RenderWindow.

```
  Camera* leftEye = scnMgr->createCamera("leftEye");
  Camera* rightEye = scnMgr->createCamera("rightEye");

  xrRenderWindow->addViewport(leftEye,1 ,0,0,1,1);
  xrRenderWindow->addViewport(rightEye,0,0  ,0,1,1);
```

## Credit and license

This project was written using the BasicXRApp sample provided by Microsoft as a
reference, https://github.com/microsoft/OpenXR-MixedReality but it is its own
work produced by Glastonbridge Software Limited. This code is provided under
the MIT license.
