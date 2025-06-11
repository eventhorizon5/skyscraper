# Skyscraper

<img src="https://www.skyscrapersim.net/guide/guide/skyscraper128.png"/>

Skyscraper, also known as SkyscraperSim, is a 3D, first-person virtual building and elevator simulator, written in C++

https://www.skyscrapersim.net

https://github.com/eventhorizon5/skyscraper

https://sourceforge.net/projects/skyscraper/

<img src="https://www.skyscrapersim.net/content/triton-sunset.jpg"/>

## Contents

### Legal Notices

### Introduction

### Screenshots

### Release Notes

### Building and Installing

### Usage

### Detailed Release Notes

### Contact

<br>

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
gaming support (single and network multiplayer). Everything possible is planned to be simulated,
including elevators, stairs, interfloor areas, ductwork, and more, all in first-person 3D.
The simulator core is called the Scalable Building Simulator, or SBS. Buildings are stored
in scripted text files that are loaded and generated in realtime by the simulator. A
graphical building designer application is planned to assist in creating building data
files.

## Screenshots

<img src="https://www.skyscrapersim.net/content/glass_elevators.jpg"/>

<img src="https://www.skyscrapersim.net/content/series1_fixturesetdemo.jpg"/>

<img src="https://www.skyscrapersim.net/content/glasstower3.jpg"/>

## Release Notes

This release is the eleventh development release of the 2.0 series (which is a complete rewrite of
the original 1.0 version), and is part of an ongoing effort towards a 2.0 stable release.

This software utilizes the OGRE rendering engine library (version 13), the Bullet physics engine with some custom patches, the FMOD sound system, the wxWidgets library (version 3.2 or later), and the Caleum sky system addon for OGRE.

Skyscraper currently only supports OpenGL.

See the changelog for a detailed list of new features, fixes, and other changes in this release.

For third-party texture credits, see the file "license-info.txt" in the "data" folder.

See the bottom of this document for detailed release notes.

## Building and installing

Skyscraper uses the Cmake build system, and also includes Visual C++ project files.

See the compiling.txt file for information on how to build from source.

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

## Detailed Release Notes

(in progress)

## Copyright and Contact

Copyright (C)2003-2025 Ryan Thoryk

Licensed under the GPL2 License

Bug-Reports, suggestions, and requests should be posted to GitHub

I can be contacted directly at ryan@skyscrapersim.net
