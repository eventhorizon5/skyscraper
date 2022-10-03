# Table of contents

* [Compiling for Debian Linux](#skyscraper-source-build-instructions-for-debian-linux)
* [Compiling for Apple Silicon](#skyscraper-source-build-for-apple-silicon)

# Skyscraper source build instructions for Debian Linux

#### Install required stuff:

Debian 11:
---
    sudo apt-get install git cmake gcc g++ wx3.0-headers libwxgtk3.0-gtk3-dev freeglut3-dev zlib1g-dev libfreeimage-dev libfreetype-dev libois-dev libzzip-dev libxaw7-dev libxrandr-dev x11proto-randr-dev nvidia-cg-toolkit libgtk2.0-dev libboost-dev cmake-curses-gui libgtk-3-dev

If you have a multi-core system, at the step that says to type "make", you can instead type "make -j2" to build using 2 cores, or "make -j4" for 4 cores.

Step 1 - build and install Bullet
----------

#### Get Bullet source (my custom-patched version):

    wget https://www.skyscrapersim.net/downloads/dev/other_apps/bullet.tar.bz2

#### Extract Bullet:

    tar xfvj bullet.tar.bz2

#### Build and install Bullet:

    cd bullet-svn
    cmake -DCMAKE_C_FLAGS=-fPIC -DCMAKE_CXX_FLAGS=-fPIC -DINSTALL_EXTRA_LIBS=ON -DBUILD_DEMOS=OFF -DUSE_DOUBLE_PRECISION=ON -DCMAKE_BUILD_TYPE=Release
    make
    sudo make install
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
    sudo cp -a lib/x86_64/* /usr/local/lib/
    cd


Step 3 - build and install OGRE
-----------

#### First get the source:
    
    wget https://www.skyscrapersim.net/downloads/dev/other_apps/ogre-13-skyscraper.tar.bz2
    tar xfvz ogre-13-skyscraper.tar.bz2
    cd ogre-13.4.3
    cmake -DOGRE_CONFIG_DOUBLE=ON -DOGRE_BUILD_SAMPLES=OFF -DCMAKE_BUILD_TYPE=Release -DOGRE_CONFIG_THREADS=0 -DOGRE_NODELESS_POSITIONING=ON OGRE_BUILD_COMPONENT_BULLET=OFF
    make
    sudo make install
    cd

(if you want to install the OGRE samples, for the cmake line above do "cmake -DINSTALL_SAMPLES=ON" instead)


Step 4 - build and install Caelum (sky system)
-----------
This one you can pull from Git, and I have custom-modified source code.
So pull the source from Git, and build:

    git clone https://github.com/OGRECave/ogre-caelum.git caelum
    cd caelum
    cmake -DCMAKE_BUILD_TYPE=Release
    make
    sudo make install
    cd


Step 5 - build wxWidgets
-----------

    wget https://github.com/wxWidgets/wxWidgets/releases/download/v3.2.1/wxWidgets-3.2.1.tar.bz2
    cd wxWidgets-3.2.1
    cmake . --prefix=/opt/wx
    make
    make install


Step 6 - build Skyscraper (this also builds the built-in OgreBullet libraries)
-----------
First grab the source via git, and build, pointing it to the wxWidgets you just built.

    git clone https://github.com/eventhorizon5/skyscraper.git skyscraper
    cd skyscraper
    wget https://www.skyscrapersim.net/downloads/dev/other_apps/plugins.cfg
    cmake . -DwxWidgets_CONFIG_EXECUTABLE=/opt/wx/bin/wx-config
    make

##### Finally, run Skyscraper:

    ./skyscraper

##### To update the app in the future, first update git, rebuild and run:

    git pull
    make
    ./skyscraper

##### If any files were added in git, you'll have to run CMake before the "make" command, or else it'll have linking errors:

    git pull
    cmake .
    make
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
    
    wget https://www.skyscrapersim.net/downloads/dev/other_apps/ogre-13-skyscraper.tar.bz2
    tar xfvz ogre-13-skyscraper.tar.bz2
    cd ogre-13.4.3
    cmake -DOGRE_CONFIG_DOUBLE=ON -DOGRE_BUILD_SAMPLES=OFF -DCMAKE_BUILD_TYPE=Release -DOGRE_CONFIG_THREADS=0 -DOGRE_NODELESS_POSITIONING=ON -DOGRE_BUILD_COMPONENT_BULLET=OFF -DCMAKE_OSX_ARCHITECTURES="arm64" -DCMAKE_INSTALL_PREFIX=/usr/local/ -DOGRE_BUILD_RENDERSYSTEM_METAL=ON
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
    cmake -DCMAKE_C_FLAGS=-fPIC -DCMAKE_CXX_FLAGS=-fPIC -DINSTALL_EXTRA_LIBS=ON -DBUILD_DEMOS=OFF -DUSE_DOUBLE_PRECISION=ON -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES="arm64"
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
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-I/usr/local/include -arch arm64 -std=c++11"
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