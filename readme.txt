Skyscraper 2.0 Alpha 9
Copyright (C)2003-2013 Ryan Thoryk
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
FMOD Sound System, copyright (C) Firelight Technologies Pty, Ltd., 1994-2010.

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

This release is the ninth development relase of the 2.0 series (which is a complete rewrite of
the original 1.0 version), and is part of an ongoing effort towards a 2.0 stable release.

This software requires both the OGRE graphics engine library (version 1.7 or later) which
can be found at http://www.ogre3d.org, the Bullet physics engine with some custom patches,
available at http://www.skyscrapersim.com/downloads/dev/other_apps/, the FMOD sound system (www.fmod.org), and the wxWidgets library (version 2.6.3 or later) which can be found at http://www.wxwidgets.org.

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
available building (and one button for loading user buildings). Choose the building to load, and in a few moments you will be walking around in that building. The only buildings that are completely simulated in this release are "Simple" and "Triton Center". While in one of these buildings, you can click on objects such as elevator call buttons, etc to perform actions.  The recommended building to try out is the Triton Center, since it is the most complete official building.  The "Simple" building is a minimalistic demo to basically show how to make your own buildings in the application's scripting language. Other user-created buildings are included, and can be loaded by choosing the "Other Building" button on the main screen.

------------------------------------
Keys currently used in the simulator:

W or Up Arrow - move forwards
S or Down Arrow - move backwards
A or Left Arrow - move left
D or Right Arrow - move right
PgUp or P - Look upwards
PgDown or L - Look downwards
Space - Jump
Home or O - Float upwards (or jump if collision detection is on)
End or K - Float downwards (or crouch if collision detection is on)
F2 - print FPS on console
F3 - Reset camera rotation and zoom to default
F4 - Enable/disable wireframe mode
F5 - Freelook (mouse look) mode
F6 - Full camera reset (respawn)
F7 - Show colliders
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
Ctrl-R - Reload current building
Ctrl-Alt-C - Crash program (throw exception) - used for testing handlers

--- Mouse actions ---
-elevator shaft doors can be manually opened and closed by holding down the Shift key and clicking on them - note that this only works when the interlocks are released, which can be done in the elevator editor by clicking "Set" next to "Interlocks" on the related elevator
-polygons (wall/floor sides) can be deleted by holding down Ctrl and clicking on the object
-doors, controls/buttons, and call buttons can be locked/unlocked by holding down the Shift key and clicking on the side to lock, if you have the associated key

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

6. Detailed Release Notes (major changes since Alpha 7)

Skyscraper now generates it's own raw 3D geometry.  Alpha 7 relied on Crystal Space's "thingmesh" system, and during the Alpha 8 (Crystal Space version) development, I ported everything off and created Skyscraper's geometry processor which made the OGRE port possible.

Skyscraper now uses the OGRE graphics engine for all graphics rendering and other operations, Bullet for collision detection and physics, and FMOD for sound.  This has resulted in many more features and greater expandibility.  Previous releases used the Crystal Space game engine.

Skyscraper supports both DirectX and OpenGL (previous versions only used OpenGL).  To switch between them, simply delete the ogre.cfg file in Skyscraper's folder, run the program again, and choose from the OGRE rendering menu.

This version has a new performance profiler built in (the same one Bullet uses), and is mainly to help pinpoint performance issues.  To get to it, click the Profiler button on the control panel window.  It'll show different components and functions of the simulator, and their per-frame timing in milliseconds.

This version supports starting elevators in a certain fire mode (FireService1 and FireService1 parameters), and supports only playing elevator music while the elevator is moving (MusicOnMove parameter) - you can also globally set those in the INI file.

Small objects (such as directional indicators, controls, call buttons) now have a maximum distance value (default 100) that they'll be shown, to improve performance.

Skyscraper now supports loading standard OGRE mesh models.  A couple couches and keys are loaded in the Simple building as an example.  See this forum thread on how to create models using Google Sketchup:
https://forum.skyscrapersim.com/index.php?topic=5556.0

To optimize rendering speed, the main thing that will cause overall slowdowns is the type of texture filtering used.  The better the filtering, the better the visual quality but the slower the app.  Skyscraper defaults to anisotropic filtering at level 4.  Lower-quality filtering is available too in the forms of bilinear and trilinear filtering, so to improve rendering performance, either lower the anisotropic level in the INI file or switch to a different filtering method.  Also, the Vsync option normally locks the FPS at 60, but can cause abnormal slowdowns that can decrease the framerate below what it should (such as an FPS reading of 30 with vsync on, while one of 40 with it off).  These options can all be set in the INI file.

This version supports selective floor displays on indicators (meaning you can specify which floors to display on indicators) using the DisplayFloors command in the elevator section.

Caelum is used to create dynamic skies, including moving clouds, moving sun and moon, effects like fog and rain, stars at night, etc.  To use Caelum, choose (or create) a sky function from the data/caelum/sky.os script, and either add it to your building script using the DynamicSky command or in the INI file with the SkyName parameter.  See the script guide (designguide.html) for more information on the available options.  The Caelum sky-related time multiplier can be set in the Camera Control box, in order to quickly show day/night cycles.

To revert back to the old sky system, turn off Caelum in the skyscraper.ini file using the Skyscraper.Frontent.Caelum option.

A basic code constructor has been added.  It's basically in-sim menus that let you build things during runtime by generating the related script commands.  You can access it by clicking "Object Manager" on the control panel and clicking "Create".

The Triton Center and Glass Tower now have new external windows using the new LoadAlphaBlendTexture command.

Skyscraper supports loading Ogre material definitions and textures directly by placing all of the files into the data/materials folder, and using the LoadMaterial command to map the material to a simulator texture name (more info in the script guide).

Texture animation has been added, and is done by many of the new commands - a new building has been created to show off the animations, and is called "Simple - Animated" - this uses all of the commands, and you can quickly learn how to use them this way.  This feature was very easy to add due to the capabilities of OGRE and how easy those were implemented in that graphics engine.

An Action system has been created (along with a viewer/player in the control panel), allowing you to create custom controls and triggers anywhere in the simulator and bind them to specific simulator actions.  Custom controls work just like elevator buttons/controls, and perform certain actions - see the animated simple building for an example.  Triggers are defined areas that perform actions just like controls, except that it does them whenever you enter/leave the trigger area.  See the animated simple building for an example/demo of this also.  Sounds can also be played/stopped from actions.

A bunch of new textures have been added, which are mostly there for window shading effects (see the Glass Tower and Triton Center buildings for examples).

A new AddFloor command syntax has been made which fixes a number of polygon generation problems with the old version

A "GotoFloor" option has been made in the camera controller, allowing you to instantly jump to any floor.

Due to changes made to the planar texture mapper, there's a chance that some walls in existing buildings will have their textures flipped - I haven't noticed anything during testing, but if you notice anything, it would be best just to fix those walls, since previous builds (including CS versions) worked in a more broken way.

ReverseAxis command is now deprecated and only used for the old AddFloor syntax

SetPlanarMapping command has been updated, with a "rotate" option

The default walking camera speed has been changed to half of the old speed, for a more realistic walking speed.  If you want the old behavior, just set the StepSpeed and StrafeSpeed parameters in skyscraper.ini to 140.

AddShaftDoor command has been updated with a voffset parameter - shaft doors can now be placed at any height, not just the floor's base.  The Glass Tower uses this for the executive elevator.

SetShaftDoors command is now deprecated, and the options have been moved into the AddShaftDoor command.

You can now fully configure the main menu from the INI file (and can even create as many buttons as you want).

Since single-sided colliders are not possible (in order to support some "locking doors" people have made, which isn't a very good idea anyway, since you'd still be walking through a wall), I added support for actual door locks and keys.  Models are loaded and assigned as keys, and all controls (including elevator buttons) can be locked and unlocked by shift-clicking on the control (but only if you have the related key).  Keys are assigned numbers, and you can see which keys you have by clicking the "List Keys" button in the control panel.  When you click on a key, the number is added to your inventory and the key model is deleted.

The elevator bumpers feature has been improved, and now works regardless if the user is moving or not.

Go toggle options have been added as actions, so you can manually operate the elevator while in inspection mode from any control (see the AddControl command in the script guide for more info).

The Simple building now has a key model in it, which unlocks both the elevator's 10th floor button and the 10th floor stairwell door.  Other controls can be locked and unlocked, and by default they're assigned a key ID of 0 (which means no key required).

A couch has been added to the Triton Center's roof, which can be pushed off due to Bullet physics support.


Modified keys:
-------
PgUp or P - Look upwards
PgDown or L - Look downwards
Home or O - Float upwards (or jump if collision detection is on)
End or K - Float downwards (or crouch if collision detection is on)
F7 - Show colliders
Alt + PgUp/P - Spin right
Alt + PgDown/L - Spin left
Plus or ] - Zoom in (decrease FOV angle)
Minus or [ - Zoom out (increase FOV angle)
V - Toggle noclip mode (gravity and collisions off)
Shift-click - lock or unlock objects
-------


New commands/parameters:
DisplayFloors
AddModel
AddShaftModel
AddStairsModel
DisplayFloors
FireService1
FireService2
MusicOn
MusicOnMove
AutoEnable
LoadMaterial
LoadAnimatedTexture
LoadAlphaBlendTexture
RotateTexture
RotateAnimTexture
ScrollTexture
ScrollAnimTexture
ScaleTexture
TransformTexture
AddActionControl
AddShaftActionControl
AddStairsActionControl
AddTrigger
AddAction
AddSound (updated)
AddFloor (updated)
MotorUpStartSound
MotorDownStartSound
MotorUpRunSound
MotorDownRunSound
MotorUpStopSound
MotorDownStopSound
CarUpStartSound
CarDownStartSound
CarUpRunSound
CarDownRunSound
CarUpStopSound
CarDownStopSound
(inspection mode actions: UpOn, UpOff, DownOn, DownOff, GoOn and GoOff)
SetKey
SetLock
InspectionSpeed
LimitQueue
AutoEnable
ReOpen

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

7. Contact

Bug-Reports, suggestions, and requests should be posted to
the Skyscraper forum at http://forum.skyscrapersim.com
I can be contacted directly at ryan at skyscrapersim.com

