Skyscraper 2.0 Alpha 8
Copyright (C)2003-2011 Ryan Thoryk
http://www.skyscrapersim.com
http://sourceforge.net/projects/skyscraper

Contents
1. Legal Notices
2. Introduction
3. Release Notes
4. Building and Installing
5. Usage
6. Contact


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. Legal Notices

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

This software utilizes the FMOD sound system
FMOD Sound System, copyright (C) Firelight Technologies Pty, Ltd., 1994-2010.

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

This release is the eighth development relase of the 2.0 series (which is a complete rewrite of
the original 1.0 version), and is part of an ongoing effort towards a 2.0 stable release.

This software requires both the Crystal Space graphics engine library (version 1.4) which
can be found at http://www.crystalspace3d.org, and the wxWidgets library (version 2.6.3 or
later) which can be found at http://www.wxwidgets.org.  The Windows packages comes bundled
with a custom-patched Crystal Space 1.4 (SVN snapshot) and wxWidgets 2.8.9 runtimes.  The custom patch for
Crystal Space is included and is called cs12-poly3d.patch, which fixes a skybox issue.

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

 1. Set these system environment variables (usually from System Properties in Windows):
    CRYSTAL=(path to CS)
    ex: CRYSTAL=C:\CS
    WXWIN=(path to wxWidgets)
    wx: WXWIN=C:\wxWidgets-2.8.9
 2. Open the solution file skyscraper/msvc/skyscraper.sln.
 3. Choose "Release" or "Debug" build mode.
 4. Build the project.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

5. Usage

To use Skyscraper, run the program, and a main menu screen will appear with a button for each
available building (and one button for loading user buildings).
Choose the building to load, and in a few moments you will be walking around in that building.
The only buildings that are completely simulated in this release are "Simple" and "Triton Center".
While in one of these buildings, you can click on objects such as elevator call buttons, etc
to perform actions.  The recommended building to try out is the Triton Center, since it is the most
complete official building.  The "Simple" building is a minimalistic demo to basically show how to make
your own buildings in the application's scripting language.  Other user-created buildings are included,
and can be loaded by choosing the "Other Building" button on the main screen.

------------------------------------
Keys currently used in the simulator:

W or Up Arrow - move forwards
S or Down Arrow - move backwards
A or Left Arrow - move left
D or Right Arrow - move right
PgUp - Look upwards
PgDown - Look downwards
Space - Jump
Home - Float upwards (or jump if collision detection is on)
End - Float downwards (or crouch if collision detection is on)
F2 - print FPS on console
F3 - Reset camera rotation and zoom to default
F4 - Enable/disable wireframe mode
F5 - Freelook (mouse look) mode
F6 - Full camera reset (respawn)
F10 - Fullscreen mode
F11 - Take screenshot
F12 - Open control panel
Shift - Run (hold down with other keys)
Control - Walk slowly (hold down with other keys)
Alt + arrow keys - Strafe
Alt + PgUp - Spin right
Alt + PgDown - Spin left
Plus - Zoom in (decrease FOV angle)
Minus - Zoom out (increase FOV angle)
Esc - exit to main menu
R - Toggle noclip mode (gravity and collisions off)
Ctrl-Alt-C - Crash program (throw exception) - used for testing handlers

--- Mouse actions ---
-elevator shaft doors can be manually opened and closed by holding down the Shift key and clicking on them
-hallway call buttons can be lit and unlit also by holding down Shift and clicking on them (this does not call an elevator, but just changes the light status)
-polygons (wall/floor sides) can be deleted by holding down Ctrl and clicking on the object

--- Debug options ---
Other functions are available via a Crystal Space plugin called BugPlug.
To type a debug command: type ctrl-d and then press the bugplug key (so for wireframe mode, you'd press ctrl-d, the screen will prompt you for a command, and then you'd type "e".)
To select an object/mesh: type ctrl-s and then click on an object

Show wireframe mode (the second command is needed due to a quirk with the wxGL plugin - also make sure you turn off the sky when doing this):
ctrl-d e
ctrl-d c

Disable wireframe mode:
ctrl-d e

change gamma:
ctrl-d g

show camera info:
ctrl-d shift-c

dump info on all CS engine objects:
ctrl-d d

dump all 3D info to an xml file (saves to world0.xml):
ctrl-d f10

change FOV:
ctrl-d f

change FOV angle:
ctrl-d shift-f

move meshes:
first select an object using ctrl-s
then,
ctrl-d shift-up = move up (+y)
ctrl-d shift-down = move down (-y)
ctrl-d shift-left = left (-x)
ctrl-d shift-right = right (+x)
ctrl-d shift-pgup = forward (+z)
ctrl-d shift-pgdn = backward (-z)

screenshots:
ctrl-d f11 = normal
ctrl-d shift-f11 = large

show/hide FPS display:
ctrl-d ctrl-alt-p

hide selected mesh:
ctrl-d h

unhide selected mesh:
ctrl-d shift-h

select mesh(es) by name:
ctrl-d ctrl-alt-m

list loaded CS plugins:
ctrl-d ctrl-alt-l


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

6. Contact

Bug-Reports, suggestions, and requests should be posted to
the Skyscraper forum at http://forum.skyscrapersim.com
I can be contacted directly at ryan at tliquest dot net

