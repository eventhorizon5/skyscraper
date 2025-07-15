# Table of contents

* [Compiling for Debian Linux](#skyscraper-source-build-instructions-for-debian-linux)
* [Compiling for macOS](#skyscraper-source-build-for-macos)
* [Compiling for Windows](#skyscraper-source-build-for-windows-using-visual-studio-2022)

# Skyscraper source build instructions for Debian Linux

#### Install required stuff:

Debian 12:
---
    sudo apt-get install git cmake gcc g++ wx3.2-headers libwxgtk3.2-dev freeglut3-dev zlib1g-dev libfreeimage-dev libfreetype-dev libois-dev libzzip-dev libxaw7-dev libxrandr-dev x11proto-randr-dev libboost-dev cmake-curses-gui libgtk-3-dev nvidia-cg-toolkit

If you have a multi-core system, at the step that says to type "make", you can instead type "make -j2" to build using 2 cores, or "make -j4" for 4 cores.

If building on a system with an ARM CPU (such as a Raspberry Pi), leave out the "nvidia-cg-toolkit" since that's not available on ARM systems.

Step 1 - build and install Bullet
----------

#### Get Bullet source:

    git clone https://github.com/eventhorizon5/bullet3.git bullet3

#### Build and install Bullet:

    cd bullet3
    cmake . -DCMAKE_C_FLAGS=-fPIC -DCMAKE_CXX_FLAGS=-fPIC -DINSTALL_EXTRA_LIBS=ON -DUSE_DOUBLE_PRECISION=ON -DCMAKE_BUILD_TYPE=Release
    make -j4
    sudo make install
    make clean
    cd


Step 2 - install the FMOD sound system
----------

#### Get the related Linux package from:
http://www.fmod.org/download/

###### (Get the FMOD Studio Programmer's API for Linux)

    tar xfvz fmodstudioapi20208linux.tar.gz
    cd fmodstudioapi20208linux/api/core
    sudo mkdir /usr/local/include/fmod
    sudo cp -a inc/* /usr/local/include/fmod/

For Intel/AMD systems, do this:

    sudo cp -a lib/x86_64/* /usr/local/lib/
    cd

For ARM systems, do this:

    sudo cp -a lib/arm64/* /usr/local/lib/
    cd

Step 3 - build and install OGRE
-----------

#### First get the source:
    
    wget https://www.skyscrapersim.net/downloads/dev/other_apps/ogre-14-skyscraper.tar.bz2
    tar xfvj ogre-14-skyscraper.tar.bz2
    cd ogre-14.1.2
    cmake . -DOGRE_CONFIG_DOUBLE=ON -DOGRE_BUILD_SAMPLES=OFF -DCMAKE_BUILD_TYPE=Release -DOGRE_NODELESS_POSITIONING=ON -DOGRE_BUILD_COMPONENT_BULLET=OFF
    make -j4
    sudo make install
    make clean
    cd

(if you want to install the OGRE samples, for the cmake line above do "-DOGRE_BUILD_SAMPLES=ON" instead)


Step 4 - build and install Caelum (sky system)
-----------
So pull the source from Git, and build:

    git clone https://github.com/OGRECave/ogre-caelum.git caelum
    cd caelum
    cmake . -DCMAKE_BUILD_TYPE=Release
    make -j4
    sudo make install
    make clean
    cd


Step 5 - build and install OgreProcedural
-----------
Pull the source from Git, and build:

    git clone https://github.com/eventhorizon5/ogre-procedural.git ogre-procedural
    cd ogre-procedural
    cmake . -DCMAKE_BUILD_TYPE=Release
    make -j4
    sudo make install
    make clean
    cd


Step 6 - (optionally) build wxWidgets
-----------

    wget https://github.com/wxWidgets/wxWidgets/releases/download/v3.2.4/wxWidgets-3.2.4.tar.bz2
    tar xfj wxWidgets-3.2.4.tar.bz2
    cd wxWidgets-3.2.4
    ./configure --prefix=/opt/wx
    make
    make install


Step 7 - build Skyscraper (this also builds the built-in OgreBullet libraries)
-----------
First grab the source via git, and build.

    git clone https://github.com/eventhorizon5/skyscraper.git skyscraper
    cd skyscraper
    wget https://www.skyscrapersim.net/downloads/dev/other_apps/plugins.cfg

On ARM systems, you'll have to comment out the CG Program Manager plugin in the plugins.cfg file, for Skyscraper to run.

If you built a custom wxWidgets, run CMake like this:

    cmake . -DwxWidgets_CONFIG_EXECUTABLE=/opt/wx/bin/wx-config

Otherwise, just run CMake with defaults:

    cmake .

And build it:

    make -j4

This will build OgreBullet (the collisions and dynamics libraries), the Skyscraper Script Processor, the SBS engine, and Skyscraper's frontend.

##### Finally, run Skyscraper:

    ./skyscraper

##### To update the app in the future, first update git, rebuild and run:

    git pull
    make -j4
    ./skyscraper

##### If any files were added in git, you'll have to run CMake before the "make" command, or else it'll have linking errors:

    git pull
    cmake .
    make -j4
    ./skyscraper

# Skyscraper source build for macOS

## If you want to build for Intel(>2020) Macs
-----------
Simply use `-DCMAKE_OSX_ARCHITECTURES="x86_64"` instead of `-DCMAKE_OSX_ARCHITECTURES="arm64"`. Or, use `-DCMAKE_OSX_ARCHITECTURES="arm64,x86_64"` to create a universal binary.

Intel Prerequisite - Install Cg.framework
-----------
    wget http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012.dmg
    open ./Cg-3.1_April2012.dmg
    tar xfvz /Volumes/Cg-3.1.0013/Cg-3.1.0013.app/Contents/Resources/Installer\ Items/NVIDIA_Cg.tgz
    sudo mv Library/Frameworks/Cg.framework /usr/local/lib/Cg.framework



Step 1 - build and install OGRE
-----------

#### First get the source:
    
    wget https://www.skyscrapersim.net/downloads/dev/other_apps/ogre-14-skyscraper.tar.bz2
    tar xfvz ogre-14-skyscraper.tar.bz2
    cd ogre-14.1.2
    cmake . -DOGRE_CONFIG_DOUBLE=ON -DOGRE_BUILD_SAMPLES=OFF -DCMAKE_BUILD_TYPE=Release -DOGRE_NODELESS_POSITIONING=ON -DOGRE_BUILD_COMPONENT_BULLET=OFF -DCMAKE_OSX_ARCHITECTURES="arm64" -DCMAKE_INSTALL_PREFIX=/usr/local/ -DOGRE_BUILD_RENDERSYSTEM_METAL=ON -DOGRE_RESOURCEMANAGER_STRICT=0
    make
    sudo make install
    cd

(if you want to install the OGRE samples, for the cmake line above do "cmake -DINSTALL_SAMPLES=ON" instead)

Step 2 - build and install Bullet
----------
#### Get Bullet source:

    git clone https://github.com/eventhorizon5/bullet3.git bullet

#### Build and install Bullet:

    cd bullet3
    cmake . -DCMAKE_C_FLAGS=-fPIC -DCMAKE_CXX_FLAGS=-fPIC -DINSTALL_EXTRA_LIBS=ON -DUSE_DOUBLE_PRECISION=ON -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES="arm64"
    make
    sudo make install
    cd

Step 3 - install the FMOD sound system
----------

#### Get the related macOS package from:
http://www.fmod.org/download/

###### (Get the FMOD Studio Programmer's API for macOS)

Open the .dmg file, then run:

    cd "/Volumes/FMOD Programmers API Mac/FMOD Programmers API/api/core"
    sudo mkdir /usr/local/include/fmod
    sudo cp -a inc/* /usr/local/include/fmod/
    sudo cp -a lib/* /usr/local/lib/
    cd

Step 4 - build and install OgreProcedural
-----------
Pull the source from Git, and build:

    git clone https://github.com/eventhorizon5/ogre-procedural.git ogre-procedural
    cd ogre-procedural
    cmake . -DCMAKE_BUILD_TYPE=Release
    make -j4
    sudo make install
    make clean
    cd

Step 5 - build and install Caelum (sky system, this only works on Intel Macs, so skip it for Apple Silicon Macs)
-----------
This one you can pull from Git, and I have custom-modified source code.
So pull the source from Git, and build:

    git clone https://github.com/OGRECave/ogre-caelum.git caelum
    cd caelum
    cmake . -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-I/usr/local/include -arch arm64 -std=c++11"
    make
    sudo make install
    cd
(you may want to use sudo vim to edit /lib/OGRE to /lib/macosx/release)

Step 6 - build wxWidgets
-----------

    wget https://github.com/wxWidgets/wxWidgets/releases/download/v3.2.1/wxWidgets-3.2.1.tar.bz2
    cd wxWidgets-3.2.1
    cmake . -DCMAKE_INSTALL_PREFIX=/opt/wx -DCMAKE_OSX_ARCHITECTURES="arm64"
    make
    sudo make install

Step 7 - build Skyscraper (this also builds the built-in OgreBullet libraries)
-----------
First grab the source via git, and build, pointing it to the wxWidgets you just built.

    git clone https://github.com/eventhorizon5/skyscraper.git skyscraper
    cd skyscraper
    wget https://www.skyscrapersim.net/downloads/dev/other_apps/plugins.cfg
    cmake . -DwxWidgets_CONFIG_EXECUTABLE=/opt/wx/bin/wx-config -DCMAKE_CXX_FLAGS="-arch arm64 -std=c++11 -I/usr/local/include"
    make

You may need to edit plugins.cfg to redirect to `Contents/Framework`, and comment out plugins like Cg if you are on M1.

##### Finally, run Skyscraper:

    open Skyscraper.app

##### To update the app in the future, first update git, rebuild and run:

    git pull
    make
    open Skyscraper.app

##### If any files were added in git, you'll have to run CMake before the "make" command, or else it'll have linking errors:

    git pull
    cmake .
    make
    open Skyscraper.app


# Skyscraper source build for Windows using Visual Studio 2022

Step 1 - download the dependencies
-----------
First, download the dependencies archive here:

https://www.skyscrapersim.net/downloads/dev/other_apps/skyscraper-deps-winbuild-release.7z
    
Step 2 - extract the dependencies
-----------
Use the 7zip tool from https://7-zip.org to extract the package, extract it into the root of your C: drive.

Step 3 - install the FMOD sound system
-----------
Go to https://fmod.com and sign up for an account.

Log in, click Download, and download and install the "FMOD Engine" for Windows.

Step 4 - download and install Git
-----------
Get a Git client, such as the one from https://gitforwindows.org/

In a terminal or Powershell, go to the folder where you'd like to store Skyscraper (I put it in C:\Skyscraper), then download the Skyscraper source repository:

    git clone https://github.com/eventhorizon5/skyscraper.git skyscraper
    cd skyscraper

Download the file at https://www.skyscrapersim.net/downloads/dev/other_apps/plugins-windows.cfg and place it into your new Skyscraper folder, rename it to plugins.cfg.

Step 5 - install and run cmake
-----------
Go to https://cmake.org/download/ to download cmake

Run CMake on Skyscraper's folder, using C:\Skyscraper for both the source and binary folders (click Configure).

It'll have errors, to fix them, do this:

    -OGRE_DIR needs to be set to c:/ogre-14.2.6/SDK/CMake
    -click Configure again
    -set the ogreprocedural include directory to c:/ogre-procedural/SDK/include
    -click Configure again
    -uncheck the wxWidgets_USE_REL_AND_DBG
    -click Configure again
    -BULLET_INCLUDE_DIR needs to be c:/bullet/include
    -click Configure again
    -check Advanced, and then you need to resolve the Bullet libraries (CMake can't find them), so for all the BULLET_LIBRARY statements, point them to the related libraries in c:/bullet/lib.  Ignore the DEBUG paths.
    -click Configure again
    -then configure Caelum by pointing Caelum_INCLUDE_DIR to c:/caelum-win64/main/include and Caelum_LIBRARY to c:/caelum-win64/lib/RelWithDebInfo/Caelum.lib
    -for OpenXR, set OPENXR_DIR to C:/openxr and OPENXR_loader_LIBRARY to C:/OpenXR/x64/lib/openxr_loader.lib
    -click Configure again
    -finally click Configure and then Generate

Step 6 - copy over the DLLs into Skyscraper's folder so it'll run
------------
Download the package https://download.skyscrapersim.net/dev/other_apps/skyscraper_dlls.zip and extract it into c:\skyscraper

Step 7 - build and run
------------
Run Visual Studio and open the Skyscraper.sln project in Skyscraper's folder, and it'll start up in Debug mode.  Keep it this way for debugging, or choose RelWithDebInfo for faster release builds.  You can also change the renderer settings on startup by deleting the ogre.cfg file and running again.

If Skyscraper crashes when starting, make sure you install the Visual C++ runtime here: https://aka.ms/vs/17/release/vc_redist.x64.exe
