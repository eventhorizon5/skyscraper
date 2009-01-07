Skyscraper 2.0 Alpha 1
Copyright (C)2003-2009 Ryan Thoryk
http://www.skyscrapersim.com
http://sourceforge.net/projects/skyscraper

Contents
1. Legal Notice
2. Introduction
3. Release Notes
4. Building and Installing
5. Usage
6. Contact


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. Legal Notice

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

For more information, see the LICENSE file.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

2. Introduction

Skyscraper is a 3D virtual building simulator written in C++ using the Crystal Space
graphics engine.  The goal of the Skyscraper project is to create a fully-featured,
modular, multiplatform 3D realtime building simulation engine, including gaming support (single
and network multiplayer). Everything possible is planned to be simulated, including
elevators, stairs, interfloor areas, ductwork, and more, all in first-person 3D. The
simulator core is called the Scalable Building Simulator, or SBS. Buildings are stored
in scripted text files that are loaded and generated in realtime by the simulator. A
graphical building designer application is planned to assist in creating building data
files.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

3. Release Notes

This release is a complete rewrite of the original 1.0 version, and is part of an
ongoing effort towards a 2.0 stable release.

This software requires both the Crystal Space graphics engine library (1.2 or later) which
can be found at http://www.crystalspace3d.org, and the wxWidgets library (version 2.6.3 or
later) which can be found at http://www.wxwidgets.org.  The Windows packages comes bundled
with a custom-patched Crystal Space 1.2.1 and wxWidgets 2.8.9 runtimes.  The custom patch for
Crystal Space is included and is called cs12-poly3d.patch - it applies to Crystal Space 1.2.1.  It does
not work correctly with version 1.4.  The patch fixes a skybox related issue.

See the changelog for new features, fixes, and other changes in this release.

For third-party texture credits, see the file "license-info.txt" in the "data" folder.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

4. Building and installing

Skyscraper can be built on Unix, Msys/Mingw, and Cygwin using the Jam build
tool. Microsoft Visual C++ users can build the project using the project files
in the skyscraper/msvc directory.

4.1 Jam

 The build system is based upon Jam rather than 'make'.  If you do not yet have
 Jam installed, then check your distribution for a Jam package or download the
 source and compile/install Jam yourself.  You can find the source here:

    ftp://ftp.perforce.com/pub/jam/

 You will need at least version 2.4.

4.1.1 Building

 1. Go to the top-level directory in the project tree.
 2. Run "./configure", possibly with some options, if you wish.  You can see a
    list of available options by invoking configure with the "--help" option.
 3. Run "jam".

4.1.2 Installing

 1. Build the project.
 2. Ensure that you have write permissions to the install directories. On Unix
    (GNU/Linux, MacOS/X, Darwin, BSD) this usually means that you need to be
    the 'root' user.
 3. Run "jam install".

4.2 Microsoft Visual C++

 1. Open the solution file skyscraper/msvc/skyscraper.sln.
 2. Choose "Release" or "Debug" build mode.
 3. Build the project.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

5. Usage

Currently the application doesn't have a title/menu screen as 1.0 did, but will eventually.
To use Skyscraper, run the program, and a building selection box will appear.  Choose the
building script to load, and in a few moments you will be walking around in that building.
The only buildings that are completely simulated in this release are "Simple" and "Triton Center".
While in one of these buildings, you can click on objects such as elevator call buttons, etc
to perform actions.  The door code hasn't been finished yet, and so doors (except for elevator
doors) are currently non-existent.  Sound support will exist in the next release.

Keys currently used in the simulator:

Arrow keys - movement
PgUp - Look upwards
PgDown - Look downwards
Space - Jump
Home - Float upwards (or jump if collision detection is on)
End - Float downwards (or crouch if collision detection is on)
F2 - print FPS on console
F3 - Reset camera rotation to default
Shift - Run (hold down with other keys)
Control - Walk slowly (hold down with other keys)
Alt - Strafe (hold down with other keys)


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

6. Contact

Bug-Reports, suggestions, and requests should be send to
ryan at tliquest dot net


