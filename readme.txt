Skyscraper 2.0 Alpha 11 
Copyright (C)2003-2023 Ryan Thoryk
https://www.skyscrapersim.net
https://github.com/eventhorizon5/skyscraper
https://sourceforge.net/projects/skyscraper/

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

Skyscraper is a 3D virtual building simulator written in C++ using the OGRE
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

This release is the eleventh development release of the 2.0 series (which is a complete rewrite of
the original 1.0 version), and is part of an ongoing effort towards a 2.0 stable release.

This software utilizes the OGRE rendering engine library (version 13), the Bullet physics engine with some custom patches, the FMOD sound system, the wxWidgets library (version 3.2 or later), and the Caleum sky system addon for OGRE.

Skyscraper currently only supports OpenGL.

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
F9 - Toggle statistics display
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
X - Crouch
; - (semicolon) - Load an additional building
1 to 0 - (number keys) - Switch active camera to engine instance number (ex. 2 for building engine 2)
E - Enter/Exit a vehicle
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

Most objects can be deleted by pressing Ctrl and Alt together, and clicking on the object.  On a Mac, the keys are Command and Option.

Skyscraper has supported loading standard OGRE mesh models since the Alpha 8 release.  A couple couches and keys are loaded in the Simple building as an example, and there is a couch on the roof of the Triton Center to demonstrate physics.  See this forum thread on how to create models:
https://skyscrapersimulatorforum.createaforum.com/tutorials/guide-to-importing-models-into-skyscraper/

To optimize rendering speed, the main thing that will cause overall slowdowns is the type of texture filtering used.  The better the filtering, the better the visual quality but the slower the app.  Skyscraper defaults to anisotropic filtering at level 4.  Lower-quality filtering is available too in the forms of bilinear and trilinear filtering, so to improve rendering performance, either lower the anisotropic level in the INI file or switch to a different filtering method.  Also, the Vsync option normally locks the FPS at 60, but can cause abnormal slowdowns that can decrease the framerate below what it should (such as an FPS reading of 30 with vsync on, while one of 40 with it off).  These options can all be set in the INI file.

Caelum is used to create dynamic skies, including moving clouds, moving sun and moon, effects like fog and rain, stars at night, etc.  To use Caelum, choose (or create) a sky function from the data/caelum/sky.os script, and either add it to your building script using the DynamicSky command or in the INI file with the SkyName parameter.  See the script guide (designguide.html) for more information on the available options.  The Caelum sky-related time multiplier can be set in the Camera Control box, in order to quickly show day/night cycles.

To revert back to the old sky system, turn off Caelum in the skyscraper.ini file using the Skyscraper.Frontent.Caelum option.


Major changes since the Alpha 9 release:
------------------------------------

Ogre 13 is now used instead of 1.9, which Alpha 10 used.  Model mesh files need to be upgraded to the newer format to prevent warning messages on startup - they'll still work fine though.

MacOS is now fully supported, with an application bundle.  The official build of Skyscraper runs on both Intel and ARM/M1 macs.

A statistics bar has been added to the simulator, which can be turned on and off by pressing F9.

Vehicles are now supported, demonstrated in a vehicle demo building.

Ropes and counterweights are now supported for elevators.

Skyscraper's source code is now hosted on GitHub:
https://github.com/eventhorizon5/skyscraper

Subversion/SVN information in the simulator has been removed.

Lighting has been added, as an experimental feature, and can be enabled in the INI file

CameraTexture objects are now supported

Shaft and Stairwell objects now use the new Levels design in the source code

Many bugfixes.


Modified keys:
-------
F9 shows/expands/hides the statistics bar
-------


New commands/parameters:
RunState
RopePosition
RopeTexture
CounterweightStartSound
CounterweightMoveSound
CounterweightStopSound
EarlyUpChimeSound
EarlyDownChimeSound
TimeScale
AccelJerk
DecelJerk
CreateCounterweight
AddRails

(Extended Commands/Functions section was added)

(Vehicles section has been added)


New elevator actions:
AccessDown
AccessUp
AccessOff

New call button actions:
PressUp
PressDown

New CameraTexture actions:
Enable
Disable

New Light actions:
On
Off

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

7. Contact

Bug-Reports, suggestions, and requests should be posted to GitHub
I can be contacted directly at ryan@thoryk.com

