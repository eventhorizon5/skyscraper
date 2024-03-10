# Table of contents

* [Compiling for Debian Linux](#skyscraper-source-build-instructions-for-debian-linux)
* [Compiling for Apple Silicon](#skyscraper-source-build-for-apple-silicon)
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

#### Get Bullet source (my custom-patched version):

    wget https://www.skyscrapersim.net/downloads/dev/other_apps/bullet.tar.bz2

#### Extract Bullet:

    tar xfvj bullet.tar.bz2

#### Build and install Bullet:

    cd bullet-svn
    cmake . -DCMAKE_C_FLAGS=-fPIC -DCMAKE_CXX_FLAGS=-fPIC -DINSTALL_EXTRA_LIBS=ON -DBUILD_DEMOS=OFF -DUSE_DOUBLE_PRECISION=ON -DCMAKE_BUILD_TYPE=Release
    make -j4
    sudo make install
    make clean
    cd

If you want to play around with the Bullet demos, remove the "-DBUILD_DEMOS=OFF" portion when running cmake above and then build it.


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


Step 5 - (optionally) build wxWidgets
-----------

    wget https://github.com/wxWidgets/wxWidgets/releases/download/v3.2.4/wxWidgets-3.2.4.tar.bz2
    tar xfj wxWidgets-3.2.4.tar.bz2
    cd wxWidgets-3.2.4
    ./configure --prefix=/opt/wx
    make
    make install


Step 6 - build Skyscraper (this also builds the built-in OgreBullet libraries)
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

# Skyscraper source build for Apple Silicon

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
#### Get Bullet source (my custom-patched version):

    wget https://www.skyscrapersim.net/downloads/dev/other_apps/bullet.tar.bz2

#### Extract Bullet:

    tar xfvj bullet.tar.bz2

#### Build and install Bullet:

    cd bullet-svn
    cmake . -DCMAKE_C_FLAGS=-fPIC -DCMAKE_CXX_FLAGS=-fPIC -DINSTALL_EXTRA_LIBS=ON -DBUILD_DEMOS=OFF -DUSE_DOUBLE_PRECISION=ON -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES="arm64"
    make
    sudo make install
    cd

If you want to play around with the Bullet demos, remove the "-DBUILD_DEMOS=OFF" portion when running cmake above and then build it.

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

Step 4 - build and install Caelum (sky system)
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

Step 5 - build wxWidgets
-----------

    wget https://github.com/wxWidgets/wxWidgets/releases/download/v3.2.1/wxWidgets-3.2.1.tar.bz2
    cd wxWidgets-3.2.1
    cmake . -DCMAKE_INSTALL_PREFIX=/opt/wx -DCMAKE_OSX_ARCHITECTURES="arm64"
    make
    sudo make install

Step 6 - build Skyscraper (this also builds the built-in OgreBullet libraries)
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

Step 4 - set up environment variables
-----------
Skyscraper's build system requires you to set up environment variables that point to the individual dependencies.

Search for "environment variables" in the Windows search box, and click on "Edit the system environment variables".

Here's the variables that need to be set up:

BULLET_HOME_64 points to C:\bullet-svn

CAELUM_HOME_64 points to C:\caelum-win64

FMOD_HOME points to C:\Program Files (x86)\FMOD SoundSystem\FMOD Studio API Windows

OGRE_HOME_64 points to C:\ogre-14.1.2\SDK

WXWIN_64 points to C:\wxWidgets-3.2.4

Step 5 - download and install Git
-----------
Get a Git client, such as the one from https://gitforwindows.org/

In a terminal or Powershell, go to the folder where you'd like to store Skyscraper, then download the Skyscraper source repository:

    git clone https://github.com/eventhorizon5/skyscraper.git skyscraper
    cd skyscraper

Make sure you run the gitrev script before a build, otherwise the build will fail.  I did this by typing:

    cmd
    gitrev.cmd

Download the file at https://www.skyscrapersim.net/downloads/dev/other_apps/plugins-windows.cfg and place it into your new Skyscraper folder, rename it to plugins.cfg.

Once that's done go into Visual Studio, and load the project file (Skyscraper.sln) which is in the "msvc" folder.

Debug builds might have issues, so do a release build.

Step 6 - copy over the DLLs into Skyscraper's folder so it'll run
------------
Download the latest build of Skyscraper from https://www.skyscrapersim.net and copy all of the "dll" and "pdb" files from that package into the folder you created that contains the Skyscraper source code.  This will allow Skyscraper to run in release mode.

Step 7 - build and run
------------
Try having Visual Studio build from source, and then see if Skyscraper will run for you.  You can change the renderer settings on startup by deleting the ogre.cfg file and running again.
