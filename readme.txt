Skyscraper 2.0 Alpha 10 
Copyright (C)2003-2016 Ryan Thoryk
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

This software utilizes the OGRE rendering engine library (version 1.8 or later), the Bullet physics engine with some custom patches, the FMOD sound system, the wxWidgets library (version 2.8 or later), and the Caleum sky system addon for OGRE.

Skyscraper on Windows supports both DirectX and OpenGL (previous versions only used OpenGL).  To switch between them, simply delete the ogre.cfg file in Skyscraper's folder, run the program again, and choose from the OGRE rendering menu.

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

This release has a very large amount of bug fixes and stability fixes, which results in Alpha 9 being the most stable release of Skyscraper yet.  Many fixes were intended to make Skyscraper stable for dynamic creation and deletion of objects during runtime, in anticipation for a building designer.

Most objects can be deleted by pressing Ctrl and Alt together, and clicking on the object.  On a Mac, the keys are Command and Option.  Previously only the Ctrl key was used, but this caused interference with the Ctrl walking modifier feature.  Controls and Call Buttons can now be deleted this way.

The lock/unlock feature is now done by pressing Ctrl and Shift together, and clicking on the object.  Previously just the Shift key was used, but this change fixes interference with the shift running modifier.

A new building called Underground has been added, which has been used for scalability tests to enhance the simulation engine.  This building features an extremely large underground environment, with very fast elevators.

Skyscraper has supported loading standard OGRE mesh models since the Alpha 8 release.  A couple couches and keys are loaded in the Simple building as an example, and there is a couch on the roof of the Triton Center to demonstrate physics.  See this forum thread on how to create models using Google Sketchup:
http://forum.skyscrapersim.com/index.php?topic=5556.0

To optimize rendering speed, the main thing that will cause overall slowdowns is the type of texture filtering used.  The better the filtering, the better the visual quality but the slower the app.  Skyscraper defaults to anisotropic filtering at level 4.  Lower-quality filtering is available too in the forms of bilinear and trilinear filtering, so to improve rendering performance, either lower the anisotropic level in the INI file or switch to a different filtering method.  Also, the Vsync option normally locks the FPS at 60, but can cause abnormal slowdowns that can decrease the framerate below what it should (such as an FPS reading of 30 with vsync on, while one of 40 with it off).  These options can all be set in the INI file.

Caelum is used to create dynamic skies, including moving clouds, moving sun and moon, effects like fog and rain, stars at night, etc.  To use Caelum, choose (or create) a sky function from the data/caelum/sky.os script, and either add it to your building script using the DynamicSky command or in the INI file with the SkyName parameter.  See the script guide (designguide.html) for more information on the available options.  The Caelum sky-related time multiplier can be set in the Camera Control box, in order to quickly show day/night cycles.

To revert back to the old sky system, turn off Caelum in the skyscraper.ini file using the Skyscraper.Frontent.Caelum option.


Major changes since the Alpha 8 release:
------------------------------------

Ogre 1.8.1 is now used instead of 1.7.4, which Alpha 8 used.  Model mesh files need to be upgraded to the newer format to prevent warning messages on startup - they'll still work fine though.

A pool has been added to the Triton Center, on the Pool level (132).  This floor can be accessed by the Pool Express elevator in the Hotel (80) and Residential (100) skylobbies.

The Triton Center now has railings, and also displays interfloors along with shaft lights in the core utility shaft, accessible by a door on the subbasement (S) level.

For the Glass Tower, the atrium base is now the Atrium level (previously mezzanine) to match movie depictions of the Hyatt Regency and Bank of America buildings.  The neighboring fictional Peerless Building was also made, based on a spotting blueprint.

The StairsShowFloors and ShowFullStairs commands have been added, which allow stairwells to display multiple or all levels if in that stairwell.  This is used in the Triton Center's West Stairwell (serves floors 80-119) allowing you to look down the center of the whole stairwell.

More detailed information is shown in script error messages, and also in the Object Manager for each object.

Performance improvements were made for buildings with thousands of floors or very fast elevators.

Names can be used for variables instead of just variable numbers.  It's recommended to change any scripts that use numbered variables to the newer named format, to make script code easier to read.

When inspection mode is disabled in an elevator, the elevators now automatically re-level to the nearest floor.

A window has been added in the Object Manager to move objects such as models and walls.  To move an object (in this example I'll move a couch in the Simple building), first start Skyscraper and choose the Simple building.  Move into the building and look at both couches.  In the control panel, click "Object Manager", click the arrow next to "Floor 0" to expand it, scroll down, and click on one of the "Couch" entries.  Now click the Move button below, and you're now able to move and rotate it.

Elevator button texture sizing parameters have been fixed - this may break button positioning in existing buildings that have values other than 1 or 0 for the button size, but so far it seems to be limited (and is easily fixed if you just put in the correct value).

The simulator now fully supports manual elevators and platform lifts.

Elevators support constant-pressure (hold) open and close buttons.

Elevator fire modes have been reworked for full realism.

Elevators now use interlocks for all doors; to manually open and close doors, interlocks must be disabled first in the Elevator Editor, and then shift-click can be used to open/close them.  If interlocks are enabled, the elevator will not move unless all associated shaft doors are closed.

A new console window has been created, and all logging in this window is put in the skyscraper.log file.

The RenderOnStartup option has been added to the INI file, which if enabled, lets you view the building as it's being build.  This results in a very slow startup, but is interesting.

While statements have been added, and functions can be called from both If and While statements.

Nested functions are now supported, and use a basic stack system for switching between contexts.

Elevator door sensors have been added, which is enabled by default for all automatic doors.  Door sensors are automatically created triggers, sized in the elevator's internal door area with 1/4 of the character collider's width surrounding it for room.  Two new elevator commands were made which are used by it's action, and are "sensor" and "reset".  When a user passes into the sensor's trigger area, the "sensor" command is run which opens and holds the doors.  The "reset" command simply calls a function that has existed before, that resets door timers to disable a door hold.  The door sensors can be switched on and off in the elevator editor.

The selection for controls (button and switches) can be reversed by right-clicking on them.

Floor signs now take the shaft door vertical offset into account when being created.

Call buttons are now created with directions based on the combined range of all elevators, instead of just the first one specified.

A binoculars feature has been added, which can be toggled with the B key.  Pressing the B key will also reset the camera's field of view (FOV).

The interfloors adjacent to the current active floor are now shown, which follows the original design.

Elevators now support a separate emergency stop speed multiplier, along with car and motor sounds that are played during an emergency stop.

A "List Textures" button has been added to the control panel, which lists all loaded textures from the texture tiling information table, along with the related filename.

The internal sound code now supports sound queuing, which now makes the floor sounds and message sounds all play in their initiated order, one after the other, instead of simultaneously.

Call buttons now support sounds.

The elevator floor indicators and beep sounds have been revamped to update the floor number while they're passing the floor or beginning to level instead of when the elevator's base is located within that floors range, making them more realistic.

The elevator editor's floor selector now only shows the related serviced floors.


Modified keys:
-------
Ctrl-Alt-click - delete objects (previously Ctrl-click)
Ctrl-Shift-click - lock or unlock objects
Shift-click - toggle call button status
-------


New commands/parameters:
FloorInfo
ListTextures
StairsShowFloors
ShowFullStairs
ShaftShowFloors (new parameter)
ShaftShowInterfloors
DynamicSky (fixed for Caelum)
AutoDoors
OpenOnStart
Delete
RunAction
GotoFloor
OpenMessage
CloseMessage
While statements
FinishDoors (new filler walls parameter)
FinishShaftDoors (new filler walls parameter)
AddActionParent
RemoveActionParent
AddCustomFloor
EndPoint
AddFloor (parameter fix)
Interlocks
FloorHold
DoorSensor
CarEmergencyStopSound
MotorEmergencyStopSound
EmergencyStopSpeed
CreateCallButtons (new sound parameter)

New elevator actions:
OpenInt
CloseInt
OpenExt
CloseExt
OpenManual
CloseManual
OpenIntManual
CloseIntManual
Return
Up
Down
InterlocksOn
InterlocksOff
Sensor
Reset
SensorOn
SensorOff

New advanced math functions:
cos
sine
tan
acos
asin
atan
atan2
sqrt
abs
exp
log
log2
log10
mod
hypot
ceil
flr
rnd
round


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

7. Contact

Bug-Reports, suggestions, and requests should be posted to
the Skyscraper forum at http://forum.skyscrapersim.com
I can be contacted directly at ryan at skyscrapersim.com

