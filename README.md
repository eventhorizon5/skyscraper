# Skyscraper

<img src="https://www.skyscrapersim.net/guide/guide/skyscraper128.png"/>

Skyscraper, also known as SkyscraperSim, is a 3D, first-person virtual building, city and elevator simulator, written in C++

https://www.skyscrapersim.net

https://github.com/eventhorizon5/skyscraper

https://sourceforge.net/projects/skyscraper/

[Documentation](https://www.skyscrapersim.net/guide)

<img src="https://www.skyscrapersim.net/content/city_640.png"/>


## Legal Notices

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

## Introduction

Skyscraper is a 3D virtual building simulator written in C++ using the OGRE
(previously Crystal Space) graphics engine.  The goal of the Skyscraper project is to create a
fully-featured, modular, multiplatform 3D realtime building simulation engine, including
gaming support (single and network multiplayer).  The simulator core is called the Scalable Building Simulator,
or SBS. Buildings are stored in scripted text files that are loaded and generated in realtime by the simulator. A
graphical building designer is planned to assist in creating building data files.

## Screenshots

<img src="https://www.skyscrapersim.net/content/glass_elevators.jpg"/>

<img src="https://www.skyscrapersim.net/content/series1_fixturesetdemo.jpg"/>

<img src="https://www.skyscrapersim.net/content/glasstower3.jpg"/>

## Release Notes

This release is an alpha release of the 2.1 series.  Skyscraper 2.0 is a complete rewrite of the original 1.0 version.

This software utilizes the OGRE rendering engine library (version 14), the Bullet physics engine with some custom patches, OgreBullet, the FMOD sound system, the wxWidgets library (version 3.2 or later), OgreProcedural, and the Caleum sky system addon.

Skyscraper currently only supports OpenGL.  DirectX 11 support is in development.

See the changelog for a detailed list of new features, fixes, and other changes in this release.

For third-party texture credits, see the file "license-info.txt" in the "data" folder.

See the bottom of this document for detailed release notes.

## Building and installing

Skyscraper uses the Cmake build system

See the compiling.md file for information on how to build from source.

## Usage

To use Skyscraper, run the program, and a main menu screen will appear with a button for each
available building (and one button for loading user buildings).

Choose the building to load, and in a few moments you will be walking around in that building.

You can press F5 to switch between standard and freelook modes.

The only buildings that are completely simulated in this release are "Simple" and "Triton Center".

While in one of these buildings, you can click on objects such as elevator call buttons, etc to perform actions.

The recommended building to try out is the Triton Center, since it is the most complete official building, and also the Glass Tower, which has some of the more complex designs.

The "Simple" building is a minimalistic demo to basically show how to make your own buildings in the application's scripting language

Other user-created buildings are included, and can be loaded by choosing the "Other Building" button on the main screen.

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

= - Open editor

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

## Detailed Release Notes (changes from Alpha 11)

-destination dispatch was added, along with dispatch controllers and call stations

-lots of Mac fixes have been made, with a custom data directory now being available in the Application Support folder

-the Door System feature was introduced, to make possible things like sliding doors.  The door system consolidates the door backend code into shared code.

-Ogre has been upgraded to 14

-experimental Virtual Reality support using OpenXR has been introduced, this currently has been tested with Meta Quest 2 headsets, and can be turned on via the INI file

-the Sound Manager dialog has been added

-Call Buttons have been removed, and replaced with the more advanced Call Stations

-the script guide (now the Users Guide, designguide.html) has been revamped

-lots of code reorganizations have taken place, introducing new components such as VM (Virtual Manager), etc.

-Virtual Manager (VM) was introduced, which provides a Unix-like management environment for sim engines.

-development has moved to Git, and the GitRev system has been created to version-stamp the builds

-generic indicators were introduced

-experimental DirectX 11 support has been added, mainly used for VR mode and Windows/ARM builds

-the camera "RestrictRotation" feature is now enabled by default.

-the Texture Manager dialog has been introduced

-internally, the geometry tables have been moved from the Mesh object to the Polygon object

-For loops have been introduced

-new Simple City demo

-in-cab elevator keypad support has been added by MultiMonorail, you can test this in the "Simple - Keypad" building

-joystick support has been added

-the OgreProcedural library was added, adding support for geometry primitives

-the "Simple - Runloop" building and "Simple - Hydraulic" were introduced

-added support for script runloops

-cross-engine mouseclicks are now supported

-malfunctions were introduced

-building power management was added

-FreeBSD support has been added

-reverb objects are now supported

-the Moving Walkway Control dialog was added

-the City Simulation building was added

-a serviced floor manager has been added to the Elevator Editor

-regular call cancel support has been brought back

-virtual people can now be assigned a lobby level to start at

-the Virtual Manager Console (VMConsole) was introduced

-directional indicators can now blink

-the RenderOnStartup feature has been reintroduced

-maps, demonstrated in the "Simple - Map" building, have been added

-an alternate native frontend has been introduced, that doesn't rely on wxWidgets

-walls are now fully movable objects

-the original file selection dialog has been brought back, and can be enabled via the INI file

-sounds can now be disabled in the Sound Manager

-building loads can now be cancelled via the progress dialog

-the progress dialog now shows the elapsed and estimated building load times

-the Triton Center building now has a map screen in the lobby service area, and the new Skyscraper 2.1 intro music in the 135th floor Grand Ballroom

-lots of fixes


## New commands/parameters introduced:

AddSlidingDoor (create sliding doors)

AddStdDoor (create standard doors)

CreateCustomDoor

CustomDoorComponent (used with CreateCustomDoor)

FinishDoor (used with CreateCustomDoor)

MoveDoor

SetAutoClose

MusicUp (elevator car)

MusicDown (elevator car)

MusicAlwaysOn (elevator car)

MessageOnMove (elevator car)

For loops

MessageOnStart (elevator)

AddElevatorIDSigns (for destination dispatch)

EnablePhysics

Lobby

PrimCollider

ShowDirection (callstations)

## New actions introduced:

Door actions: Open, Close, AutoClose

Revolving Door actions: On, Off

Elevator actions: InsUpOn, InsUpOff, InsDownOn, InsDownOff

## Contact

Bug-Reports, suggestions, and requests should be posted to GitHub

I can be contacted directly at ryan@skyscrapersim.net
