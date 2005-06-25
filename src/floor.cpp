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

void Floor::AddFloor(wxString texture, float x1, float z1, float x2, float z2, float voffset, float tw, float th)
{

}

void Floor::AddCrawlSpaceFloor(wxString texture, float x1, float z1, float x2, float z2, float voffset, float tw, float th)
{

}

void Floor::AddWall(wxString texture, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th)
{

}

void Floor::AddCrawlSpaceWall(wxString texture, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th)
{

}

void Floor::CreateWallBox(wxString texture, float WidthX, float LengthZ, float CenterX, float CenterZ, bool CSpace, float ResX, float ResY)
{

}

void Floor::EnableFloor()
{

}

void Floor::DisableFloor()
{

}
