//Scalable Building Simulator - Floor Class
//The Skyscraper Project - Version 1.1 Alpha
//Copyright ©2005 Ryan Thoryk
//http://www.tliquest.net/skyscraper
//http://sourceforge.net/projects/skyscraper
//Contact - ryan@tliquest.net

//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include <iostream>
#include <string>

//CrystalSpace Includes
#include "cssysdef.h"
#include "csutil/sysfunc.h"
#include "iutil/vfs.h"
#include "csutil/cscolor.h"
#include "cstool/csview.h"
#include "cstool/initapp.h"
#include "iutil/eventq.h"
#include "iutil/event.h"
#include "iutil/objreg.h"
#include "iutil/csinput.h"
#include "iutil/virtclk.h"
#include "iengine/sector.h"
#include "iengine/engine.h"
#include "iengine/camera.h"
#include "iengine/light.h"
#include "iengine/texture.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/material.h"
#include "imesh/thing.h"
#include "imesh/object.h"
#include "ivideo/graph3d.h"
#include "ivideo/graph2d.h"
#include "ivideo/texture.h"
#include "ivideo/material.h"
#include "ivideo/fontserv.h"
#include "ivideo/natwin.h"
#include "igraphic/imageio.h"
#include "imap/parser.h"
#include "ivaria/reporter.h"
#include "ivaria/stdrep.h"
#include "csutil/cmdhelp.h"
#include "csutil/event.h"

#include "floor.h"

Floor::Floor(int number)
{
//Set floor's object number
FloorNumber = number;

//size shaft arrays accordingly
//ReDim CrawlSpaceShaft(ElevatorShafts)
//ReDim Shafts(ElevatorShafts)

//Create and name new meshes

//Set Level = Scene.CreateMeshBuilder("Level " + Str$(Number))
//Set CrawlSpace = Scene.CreateMeshBuilder("CrawlSpace " + Str$(Number))
//Set ShaftsFloor = Scene.CreateMeshBuilder("ShaftsFloor " + Str$(Number))
//Dim i As Integer
//For i = 1 To ElevatorShafts
//    DoEvents
//    Set CrawlSpaceShaft(i) = Scene.CreateMeshBuilder("CrawlSpaceShaft" + LTrim(Str$(i)) + " " + Str$(Number))
//    Set Shafts(i) = Scene.CreateMeshBuilder("Shafts" + LTrim(Str$(i)) + " " + Str$(Number))
//Next i


}

Floor::~Floor()
{

}

void Floor::SetCameraFloor()
{

}

void Floor::AddFloor(csString texture, float x1, float z1, float x2, float z2, float voffset, float tw, float th)
{

}

void Floor::AddCrawlSpaceFloor(csString texture, float x1, float z1, float x2, float z2, float voffset, float tw, float th)
{

}

void Floor::AddWall(csString texture, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th)
{

}

void Floor::AddCrawlSpaceWall(csString texture, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th)
{

}

void Floor::CreateWallBox(csString texture, float WidthX, float LengthZ, float CenterX, float CenterZ, bool CSpace, float ResX, float ResY)
{

}

void Floor::EnableFloor()
{

}

void Floor::DisableFloor()
{

}
