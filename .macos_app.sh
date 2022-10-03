#!/bin/bash

cp -r ./tools/mac/skyscraper.app/ ./Skyscraper.app/
mkdir -p ./Skyscraper.app/Contents/Frameworks
mkdir -p ./Skyscraper.app/Contents/MacOS/Libs
cp ./libSBS.dylib ./Skyscraper.app/Contents/MacOS/Libs
cp ./libOgreBulletCol.dylib ./Skyscraper.app/Contents/MacOS/Libs
cp ./libOgreBulletDyn.dylib ./Skyscraper.app/Contents/MacOS/Libs
cp ./skyscraper ./Skyscraper.app/Contents/MacoS
cp `grep Caelum_LIBRARY CMakeCache.txt | cut -d "=" -f2` ./Skyscraper.app/Contents/MacOS/Libs
cp `grep FMOD_LIBRARY CMakeCache.txt | cut -d "=" -f2` ./Skyscraper.app/Contents/MacOS/Libs
ver=$(`grep wxWidgets_CONFIG_EXECUTABLE CMakeCache.txt | cut -d "=" -f2` --release)
pre=$(`grep wxWidgets_CONFIG_EXECUTABLE CMakeCache.txt | cut -d "=" -f2` --prefix)
cp -P $pre/lib/libwx_baseu-$ver*.dylib ./Skyscraper.app/Contents/MacOS/Libs
cp -P $pre/lib/libwx_osx_cocoau_core-$ver*.dylib ./Skyscraper.app/Contents/MacOS/Libs
cp -P $pre/lib/libwx_osx_cocoau_gl-$ver*.dylib ./Skyscraper.app/Contents/MacOS/Libs
cp -r "$1/Codec_FreeImage.framework" ./Skyscraper.app/Contents/Frameworks
cp -r "$1/Ogre.framework" ./Skyscraper.app/Contents/Frameworks
cp -r "$1/OgreBites.framework" ./Skyscraper.app/Contents/Frameworks
cp -r "$1/OgreMeshLodGenerator.framework" ./Skyscraper.app/Contents/Frameworks
cp -r "$1/OgreOverlay.framework" ./Skyscraper.app/Contents/Frameworks
cp -r "$1/OgrePaging.framework" ./Skyscraper.app/Contents/Frameworks
cp -r "$1/OgreProperty.framework" ./Skyscraper.app/Contents/Frameworks
cp -r "$1/OgreRTShaderSystem.framework" ./Skyscraper.app/Contents/Frameworks
cp -r "$1/OgreTerrain.framework" ./Skyscraper.app/Contents/Frameworks
cp -r "$1/OgreVolume.framework" ./Skyscraper.app/Contents/Frameworks
cp -r "$1/Plugin_OctreeSceneManager.framework" ./Skyscraper.app/Contents/Frameworks
cp -r "$1/Plugin_OctreeSceneManager.framework" ./Skyscraper.app/Contents/Frameworks
cp -r "/usr/local/lib/Cg.framework" ./Skyscraper.app/Contents/Frameworks
cp -r "$1/Plugin_CgProgramManager.framework" ./Skyscraper.app/Contents/Frameworks
cp -r "$1/RenderSystem_GL.framework" ./Skyscraper.app/Contents/Frameworks
cp -r "$1/RenderSystem_GL3Plus.framework" ./Skyscraper.app/Contents/Frameworks
cp -r "$1/RenderSystem_Metal.framework" ./Skyscraper.app/Contents/Frameworks
cp -r ./data ./Skyscraper.app/Contents/Resources
cp -r ./buildings ./Skyscraper.app/Contents/Resources
cp -r ./media ./Skyscraper.app/Contents/Resources
cp -r ./screenshots ./Skyscraper.app/Contents/Resources
cp -r ./tools ./Skyscraper.app/Contents/Resources
cp -r ./guide ./Skyscraper.app/Contents/Resources
cp ./changelog.txt ./Skyscraper.app/Contents/Resources
cp ./design.rtf ./Skyscraper.app/Contents/Resources
cp ./designguide.html ./Skyscraper.app/Contents/Resources
cp ./keyboard.ini ./Skyscraper.app/Contents/Resources
cp ./plugins.cfg ./Skyscraper.app/Contents/Resources
cp ./readme.txt ./Skyscraper.app/Contents/Resources
cp ./resources.cfg ./Skyscraper.app/Contents/Resources
cp ./skyscraper.ini ./Skyscraper.app/Contents/Resources
echo "Done"