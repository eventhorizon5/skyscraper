/* $Id$ */

/*
	Scalable Building Simulator - Floor Class
	The Skyscraper Project - Version 1.1 Alpha
	Copyright (C)2005-2006 Ryan Thoryk
	http://www.tliquest.net/skyscraper
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@tliquest.net

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
*/

//#include <wx/wx.h>
#include <crystalspace.h>
#include "floor.h"
#include "sbs.h"
#include "camera.h"

extern SBS *sbs; //external pointer to the SBS engine

Floor::Floor(int number)
{
	csString buffer;

	//Set floor's object number
	Number = number;

	//Create primary level mesh
	buffer = Number;
	buffer.Insert(0, "Level ");
	buffer.Trim();
	Level = sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData());
	Level_object = Level->GetMeshObject ();
	Level_factory = Level_object->GetFactory();
	Level_state = scfQueryInterface<iThingFactoryState> (Level_factory);
	Level->SetZBufMode(CS_ZBUF_USE);

	//Create interfloor mesh
	buffer = Number;
	buffer.Insert(0, "Interfloor ");
	buffer.Trim();
	Interfloor = sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData());
	Interfloor_object = Level->GetMeshObject ();
	Interfloor_factory = Level_object->GetFactory();
	Interfloor_state = scfQueryInterface<iThingFactoryState> (Level_factory);
	Interfloor->SetZBufMode(CS_ZBUF_USE);

	//set enabled flag
	IsEnabled = true;

	//init other variables
	Name = "";
	ID = "";
	FloorType = "";
	Description = "";
	Altitude = 0;
	Height = 0;
	InterfloorHeight = 0;
	DoorHeight = 0;
	DoorWidth = 0;

	//init arrays
	floor_polys.DeleteAll();
	ifloor_polys.DeleteAll();
	wall_polys.DeleteAll();
	iwall_polys.DeleteAll();

}

Floor::~Floor()
{
	//Destructor

	for (int i = 0; i < CallButtonArray.GetSize(); i++)
	{
		delete CallButtonArray[i];
		CallButtonArray[i] = 0;
	}
	CallButtonArray.DeleteAll();

}

void Floor::SetCameraFloor()
{
	//Moves camera to specified floor (sets altitude to the floor's altitude plus CameraAltitude)
	
	csVector3 camlocation = sbs->camera->GetPosition();
	sbs->camera->SetPosition(csVector3(camlocation.x, Altitude + sbs->camera->DefaultAltitude, camlocation.z));
}

int Floor::AddFloor(const char *texture, double x1, double z1, double x2, double z2, double voffset1, double voffset2, double tw, double th, bool isexternal)
{
	//Adds a floor with the specified dimensions and vertical offset
	double tw2;
	double th2;

	//Set horizontal scaling
	x1 = x1 * sbs->HorizScale;
	x2 = x2 * sbs->HorizScale;
	z1 = z1 * sbs->HorizScale;
	z2 = z2 * sbs->HorizScale;

	//Call texture autosizing formulas
	tw2 = AutoSize(x1, x2, true, isexternal, tw);
	th2 = AutoSize(z1, z2, false, isexternal, th);
	
	if (isexternal == false)
	{
		int index;
		index = sbs->AddFloorMain(Level_state, texture, x1, z1, x2, z2, Altitude + voffset1, Altitude + voffset2, tw2, th2);
		floor_polys.Push(index);
		return floor_polys.GetSize() - 1;
	}
	else
		return sbs->AddFloorMain(sbs->External_state, texture, x1, z1, x2, z2, Altitude + voffset1, Altitude + voffset2, tw2, th2);
}

void Floor::DeleteFloor(int index)
{
	//delete floor polygon from level mesh
	if (floor_polys[index] > -1)
	{
		sbs->DeleteFloor(Level_state, floor_polys[index]);
		floor_polys[index] = -1;
	}
}

int Floor::AddInterfloorFloor(const char *texture, double x1, double z1, double x2, double z2, double voffset1, double voffset2, double tw, double th)
{
	//Adds an interfloor floor with the specified dimensions and vertical offset
	double tw2;
	double th2;

	//Set horizontal scaling
	x1 = x1 * sbs->HorizScale;
	x2 = x2 * sbs->HorizScale;
	z1 = z1 * sbs->HorizScale;
	z2 = z2 * sbs->HorizScale;

	//Texture autosizing formulas
	tw2 = AutoSize(x1, x2, true, false, tw);
	th2 = AutoSize(z1, z2, false, false, th);

	int index;
	index = sbs->AddFloorMain(Interfloor_state, texture, x1, z1, x2, z2, Altitude + Height + voffset1, Altitude + Height + voffset2, tw2, th2);
	ifloor_polys.Push(index);
	return ifloor_polys.GetSize() - 1;
}

void Floor::DeleteInterfloorFloor(int index)
{
	//delete floor polygon from interfloor mesh
	if (ifloor_polys[index] > -1)
	{
		sbs->DeleteFloor(Interfloor_state, ifloor_polys[index]);
		ifloor_polys[index] = -1;
	}
}

int Floor::AddWall(const char *texture, double x1, double z1, double x2, double z2, double height_in1, double height_in2, double voffset1, double voffset2, double tw, double th, bool revX, bool revY, bool revZ, bool isexternal)
{
	//Adds a wall with the specified dimensions
	double tw2 = tw;
	double th2;
	double tempw1;
	double tempw2;
	
	//Set horizontal scaling
	x1 = x1 * sbs->HorizScale;
	x2 = x2 * sbs->HorizScale;
	z1 = z1 * sbs->HorizScale;
	z2 = z2 * sbs->HorizScale;

	//Call texture autosizing formulas
	if (z1 == z2)
		tw2 = AutoSize(x1, x2, true, isexternal, tw);
	if (x1 == x2)
		tw2 = AutoSize(z1, z2, true, isexternal, tw);
	if ((z1 != z2) && (x1 != x2))
	{
		//calculate diagonals
	    if (x1 > x2)
			tempw1 = x1 - x2;
		else
			tempw1 = x2 - x1;
		if (z1 > z2)
			tempw2 = z1 - z2;
		else
			tempw2 = z2 - z1;
	    tw2 = AutoSize(0, sqrt(pow(tempw1, 2) + pow(tempw2, 2)), true, isexternal, tw);
	}
	th2 = AutoSize(0, height_in1, false, isexternal, th);
	
	if (isexternal == false)
	{
		int index;
		index = sbs->AddWallMain(Level_state, texture, x1, z1, x2, z2, height_in1, height_in2, Altitude + voffset1, Altitude + voffset2, tw2, th2, revX, revY, revZ);
		wall_polys.Push(index);
		return wall_polys.GetSize() - 1;
	}
	else
		return sbs->AddWallMain(sbs->External_state, texture, x1, z1, x2, z2, height_in1, height_in2, Altitude + voffset1, Altitude + voffset2, tw2, th2, revX, revY, revZ);
}

void Floor::DeleteWall(int index)
{
	//delete wall polygon from level mesh
	if (wall_polys[index] > -1)
	{
		sbs->DeleteFloor(Level_state, wall_polys[index]);
		wall_polys[index] = -1;
	}
}

int Floor::AddInterfloorWall(const char *texture, double x1, double z1, double x2, double z2, double height_in1, double height_in2, double voffset1, double voffset2, double tw, double th)
{
	//Adds an interfloor wall with the specified dimensions
	double tw2 = tw;
	double th2;
	
	//Set horizontal scaling
	x1 = x1 * sbs->HorizScale;
	x2 = x2 * sbs->HorizScale;
	z1 = z1 * sbs->HorizScale;
	z2 = z2 * sbs->HorizScale;

	//Texture autosizing formulas
	if (z1 == z2)
		tw2 = AutoSize(x1, x2, true, false, tw);
	if (x1 == x2)
		tw2 = AutoSize(z1, z2, true, false, tw);
	th2 = AutoSize(0, height_in1, false, false, th);
	
	int index;
	index = sbs->AddWallMain(Interfloor_state, texture, x1, z1, x2, z2, height_in1, height_in2, Altitude + Height + voffset1, Altitude + Height + voffset2, tw2, th2, false, false, false);
	iwall_polys.Push(index);
	return iwall_polys.GetSize() - 1;
}

void Floor::DeleteInterfloorWall(int index)
{
	//delete wall polygon from interfloor mesh
	if (iwall_polys[index] > -1)
	{
		sbs->DeleteFloor(Interfloor_state, iwall_polys[index]);
		iwall_polys[index] = -1;
	}
}

void Floor::Enabled(bool value)
{
	//turns floor on/off
	if (value == true)
	{
		Level->GetFlags().Reset (CS_ENTITY_INVISIBLEMESH);
		Level->GetFlags().Reset (CS_ENTITY_NOSHADOWS);
		Level->GetFlags().Reset (CS_ENTITY_NOHITBEAM);

		Interfloor->GetFlags().Reset (CS_ENTITY_INVISIBLEMESH);
		Interfloor->GetFlags().Reset (CS_ENTITY_NOSHADOWS);
		Interfloor->GetFlags().Reset (CS_ENTITY_NOHITBEAM);

		IsEnabled = true;
	}
	else
	{
		Level->GetFlags().Set (CS_ENTITY_INVISIBLEMESH);
		Level->GetFlags().Set (CS_ENTITY_NOSHADOWS);
		Level->GetFlags().Set (CS_ENTITY_NOHITBEAM);

		Interfloor->GetFlags().Set (CS_ENTITY_INVISIBLEMESH);
		Interfloor->GetFlags().Set (CS_ENTITY_NOSHADOWS);
		Interfloor->GetFlags().Set (CS_ENTITY_NOHITBEAM);

		IsEnabled = false;
	}

	//call buttons
	for (int i = 0; i < CallButtonArray.GetSize(); i++)
		CallButtonArray[i]->Enabled(value);
}

void Floor::AddAutoFloor(const char *texture, double voffset, double tw, double th)
{
	/* Autogenerate a floor
	 
	   this function splits the "external" mesh's polygons at a certain altitude
	   (y plane), discards the lower ones, makes a list the vertices at the Y value,
	   does a clockwise sort using the Graham scan method, and creates a new floor
	   polygon from the sorted vertices.

	*/

	//int index;
	//index = sbs->AddCustomFloor(Level_state, texture, varray3, tw, th, false, false, false, false);
	//FloorList.Push(index);
	//return index;
}

double Floor::FullHeight()
{
	//calculate full height of a floor
	return Height + InterfloorHeight;
}

int Floor::AddCallButtons(bool up, bool down, int elevatornumber, bool direction, const char *backtexture, const char *buttontexture, double CenterX, double CenterZ, double width, double height, double tw, double th)
{
	//adds call buttons

	double x1, x2;
	double z1, z2;
	int firstidx;

	if (direction == false)
	{
		x1 = CenterX;
		x2 = CenterX;
		z1 = CenterZ - (width / 2);
		z2 = CenterZ + (width / 2);
	}
	else
	{
		x1 = CenterX - (width / 2);
		x2 = CenterX + (width / 2);
		z1 = CenterZ;
		z2 = CenterZ;
	}

	//firstidx = sbs->AddWallMain(CallButtonsUp_state, texture, x1, z1, x2, z2, height, height, voffset + GetPosition().y, voffset + GetPosition().y, tw, th);
	//sbs->AddWallMain(CallButtonsDown_state, texture, x1, z1, x2, z2, height, height, voffset + GetPosition().y, voffset + GetPosition().y, tw, th);
	return firstidx;
}

void Floor::CutFloor(double x1, double x2, double z1, double z2)
{
	//cuts a rectangular hole in the listed floor polygons (floor, ceiling, etc)
	
	csPoly3D temppoly, temppoly2, temppoly3, temppoly4, temppoly5;

	//step through each floor polygon
	for (int i = 0; i <= floor_polys.GetSize() - 1; i++)
	{
		temppoly.MakeEmpty();
		temppoly2.MakeEmpty();
		temppoly3.MakeEmpty();
		temppoly4.MakeEmpty();
		temppoly5.MakeEmpty();

		//copy polygon vertices
		for (int j = 0; j <= Level_state->GetPolygonVertexCount(floor_polys[i]); j++)
			temppoly.AddVertex(Level_state->GetPolygonVertex(floor_polys[i], j));
		
		//get left side
		temppoly.SplitWithPlaneX(temppoly2, temppoly, x1);

		//get right side
		temppoly2.SplitWithPlaneX(temppoly2, temppoly3, x2);

		//get lower
		temppoly3.SplitWithPlaneZ(temppoly4, temppoly3, z1);

		//get upper
		temppoly4.SplitWithPlaneZ(temppoly4, temppoly5, z2);

		//reconstruct 4 resulting polygons into single polygon

		//delete original floor polygon and create a new one
		
	}
}
