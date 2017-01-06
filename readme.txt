Skyscraper 2.0 Alpha 10 
Copyright (C)2003-2017 Ryan Thoryk
http://www.skyscrapersim.com
http://sourceforge.net/projects/skyscraper

Contents
1. Legal Notices
2. Introduction
3. Release Notes
4. Building and Installing
5. Usage
6. Detailed Release Notes
7. Contact


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
FMOD Sound System, copyright (C) Firelight Technologies Pty, Ltd., 1994-2015.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

2. Introduction

Skyscraper is a 3D virtual building simulator written in C++ using the OGRE graphics engine
(previously Crystal Space) graphics engine.  The goal of the Skyscraper project is to create a
fully-featured, modular, multiplatform 3D realtime building simulation engine, including
gaming support (single and network multiplayer). Everything possible is planned to be simulated,
including elevators, stairs, interfloor areas, ductwork, and more, all in first-person 3D.
The simulator core is called the Scalable Building Simulator, or SBS. Buildings are stored
in scripted text files that are loaded and generated in realtime by the simulator. A
graphical building designer application is planned to assist in creating building data
files.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

3. Release Notes

This release is the tenth development release of the 2.0 series (which is a complete rewrite of
the original 1.0 version), and is part of an ongoing effort towards a 2.0 stable release.

This software utilizes the OGRE rendering engine library (version 1.8 or later), the Bullet physics engine with some custom patches, the FMOD sound system, the wxWidgets library (version 3.0 or later), and the Caleum sky system addon for OGRE.

Skyscraper on Windows supports both DirectX and OpenGL.  To switch between them, simply delete the ogre.cfg file in Skyscraper's folder, run the program again, and choose from the OGRE rendering menu.

See the changelog for a detailed list of new features, fixes, and other changes in this release.

For third-party texture credits, see the file "license-info.txt" in the "data" folder.

See the bottom of this document for detailed release notes.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

4. Building and installing

Skyscraper uses the Cmake build system, and also includes Visual C++ project files.

See the compiling.txt file for information on how to build from source.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

5. Usage

To use Skyscraper, run the program, and a main menu screen will appear with a button for each
available building (and one button for loading user buildings). Choose the building to load, and in a few moments you will be walking around in that building. You can press F5 to switch between standard and freelook modes. The only buildings that are completely simulated in this release are "Simple" and "Triton Center". While in one of these buildings, you can click on objects such as elevator call buttons, etc to perform actions.  The recommended building to try out is the Triton Center, since it is the most complete official building, and also the Glass Tower, which has some of the more complex designs.  The "Simple" building is a minimalistic demo to basically show how to make your own buildings in the application's scripting language. Other user-created buildings are included, and can be loaded by choosing the "Other Building" button on the main screen.

------------------------------------
Keys currently used in the simulator:

On a Mac, the Control key is the Command key (to the right of the Option key), and Alt is the Option key

W or Up Arrow - move forwards
S or Down Arrow - move backwards
A or Left Arrow - move left
D or Right Arrow - move right
PgUp or P - Look upwards
PgDown or L - Look downwards
Space - Jump
Home or O - Float upwards (or jump if collision detection is on)
End or K - Float downwards (or crouch if collision detection is on)
F1 - show control reference
F2 - print render statistics on console
F3 - Reset camera rotation and zoom to default
F4 - Enable/disable wireframe mode
F5 - Freelook (mouse look) mode
F6 - Full camera reset (respawn)
F7 - Show colliders
F8 - Show mesh bounding boxes
F10 - Fullscreen mode
F11 - Take screenshot
F12 - Open control panel
Shift - Run (hold down with other keys)
Control - Walk slowly (hold down with other keys)
Alt + arrow keys - Strafe
Alt + PgUp/P - Spin right
Alt + PgDown/L - Spin left
Plus or ] - Zoom in (decrease FOV angle)
Minus or [ - Zoom out (increase FOV angle)
Esc - exit to main menu
V - Toggle noclip mode (gravity and collisions off)
B - Binoculars mode (hold down), and reset FOV to default
C - Pick up object (physics-enabled models)
; - (semicolon) - Load an additional building
1 to 0 - (number keys) - Switch active camera to engine instance number (ex. 2 for building engine 2)
Ctrl-R - Reload current building
Ctrl-Alt-C - Crash program (throw exception) - used for testing handlers

--- Mouse actions ---
-the selection order of controls (buttons and switches) can be reversed by right-clicking them
-elevator shaft doors can be manually opened and closed by holding down the Shift key and clicking on them - note that this only works when the interlocks are released, which can be done in the elevator editor by clicking "Set" next to "Interlocks" on the related elevator
-polygons (wall/floor sides) and other objects can be deleted by holding down both Ctrl and Alt, and clicking on the object
-doors, controls/buttons, and call buttons can be locked/unlocked by holding down both the Ctrl and Shift keys while clicking on the side to lock, if you have the associated key

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

6. Detailed Release Notes


Information for this release:
------------------------------------

This release has a very large amount of bug fixes and stability fixes, which results in Alpha 10 being the most stable release of Skyscraper yet.  Many fixes were intended to make Skyscraper stable for dynamic creation and deletion of objects during runtime, in anticipation for a building designer.

Most objects can be deleted by pressing Ctrl and Alt together, and clicking on the object.  On a Mac, the keys are Command and Option.

Skyscraper has supported loading standard OGRE mesh models since the Alpha 8 release.  A couple couches and keys are loaded in the Simple building as an example, and there is a couch on the roof of the Triton Center to demonstrate physics.  See this forum thread on how to create models using Google Sketchup:
http://forum.skyscrapersim.com/index.php?topic=5556.0

To optimize rendering speed, the main thing that will cause overall slowdowns is the type of texture filtering used.  The better the filtering, the better the visual quality but the slower the app.  Skyscraper defaults to anisotropic filtering at level 4.  Lower-quality filtering is available too in the forms of bilinear and trilinear filtering, so to improve rendering performance, either lower the anisotropic level in the INI file or switch to a different filtering method.  Also, the Vsync option normally locks the FPS at 60, but can cause abnormal slowdowns that can decrease the framerate below what it should (such as an FPS reading of 30 with vsync on, while one of 40 with it off).  These options can all be set in the INI file.

Caelum is used to create dynamic skies, including moving clouds, moving sun and moon, effects like fog and rain, stars at night, etc.  To use Caelum, choose (or create) a sky function from the data/caelum/sky.os script, and either add it to your building script using the DynamicSky command or in the INI file with the SkyName parameter.  See the script guide (designguide.html) for more information on the available options.  The Caelum sky-related time multiplier can be set in the Camera Control box, in order to quickly show day/night cycles.

To revert back to the old sky system, turn off Caelum in the skyscraper.ini file using the Skyscraper.Frontent.Caelum option.


Major changes since the Alpha 9 release:
------------------------------------

Ogre 1.9 is now used instead of 1.8.1, which Alpha 9 used.  Model mesh files need to be upgraded to the newer format to prevent warning messages on startup - they'll still work fine though.

Skyscraper now supports running multiple buildings simultaneously via the new Engine Contexts feature.  This makes the development of cities possible.

Sky parameters can now be controlled via the Sky Control dialog.

Virtual people are supported, to simulate elevator traffic.

A new sound system has been added, which is more efficient than the previous one.

A new Dynamic Meshes system has been added, which greatly improves the rendering efficiency of the simulator.

See the forum and changelog for more information on the new changes and features.

Command line options are now supported.

Double-decker elevators (really multi-deck elevators) are now supported.

Elevators now support hoistway access functions.

Working revolving doors are supported.


Modified keys:
-------
Number keys switch between engines, when multiple buildings are loaded.
-------


New commands/parameters:
ListVisibleMeshes
CreatewallObject
AddPolygon
AddExternalDoor
ChimeOnArrival
new Buildings and Car sections
Position
Bounds
ShowLoadedSounds
ShowPlayingSounds
Teleport
GotoFloor

New elevator actions:
PeakOff
StopDoors
SensorReset

New call button actions:
Off
Up
Down
FireOff
FireOn
FireBypass

New escalator/moving walkway actions:
Forward
Reverse
Stop

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

7. Contact

Bug-Reports, suggestions, and requests should be posted to
the Skyscraper forum at http://forum.skyscrapersim.com
I can be contacted directly at ryan at skyscrapersim.com

