/* $Id$ */

/*
	Scalable Building Simulator - Floor Class
	The Skyscraper Project - Version 1.1 Alpha
	Copyright (C)2005-2007 Ryan Thoryk
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

#include "globals.h"
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
	Level_state = scfQueryInterface<iThingFactoryState> (Level->GetMeshObject()->GetFactory());
	Level->SetZBufMode(CS_ZBUF_USE);

	//Create interfloor mesh
	buffer = Number;
	buffer.Insert(0, "Interfloor ");
	buffer.Trim();
	Interfloor = sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData());
	Interfloor_state = scfQueryInterface<iThingFactoryState> (Interfloor->GetMeshObject()->GetFactory());
	Interfloor->SetZBufMode(CS_ZBUF_USE);

	//set enabled flag
	IsEnabled = true;

	//init other variables
	Name = "";
	ID = "";
	FloorType = "";
	Description = "";
	IndicatorTexture = "";
	Altitude = 0;
	Height = 0;
	InterfloorHeight = 0;

	//init arrays
	floor_polys.DeleteAll();
	ifloor_polys.DeleteAll();
	wall_polys.DeleteAll();
	iwall_polys.DeleteAll();

}

Floor::~Floor()
{
	//Destructor
	CallButtonArray.DeleteAll();
}

void Floor::SetCameraFloor()
{
	//Moves camera to specified floor (sets altitude to the floor's altitude plus CameraAltitude)

	csVector3 camlocation = sbs->camera->GetPosition();
	sbs->camera->SetPosition(csVector3(camlocation.x, Altitude + sbs->camera->DefaultAltitude, camlocation.z));
}

int Floor::AddFloor(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, float tw, float th, bool isexternal)
{
	//Adds a floor with the specified dimensions and vertical offset
	float tw2;
	float th2;

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
		int index = sbs->AddFloorMain(Level_state, name, texture, thickness, x1, z1, x2, z2, Altitude + voffset1, Altitude + voffset2, tw2, th2);
		floor_polys.Push(index);
		return floor_polys.GetSize() - 1;
	}
	else
		return sbs->AddFloorMain(sbs->External_state, name, texture, thickness, x1, z1, x2, z2, Altitude + voffset1, Altitude + voffset2, tw2, th2);
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

int Floor::AddInterfloorFloor(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, float tw, float th)
{
	//Adds an interfloor floor with the specified dimensions and vertical offset
	float tw2;
	float th2;

	//Set horizontal scaling
	x1 = x1 * sbs->HorizScale;
	x2 = x2 * sbs->HorizScale;
	z1 = z1 * sbs->HorizScale;
	z2 = z2 * sbs->HorizScale;

	//Texture autosizing formulas
	tw2 = AutoSize(x1, x2, true, false, tw);
	th2 = AutoSize(z1, z2, false, false, th);

	int index = sbs->AddFloorMain(Interfloor_state, name, texture, thickness, x1, z1, x2, z2, Altitude + Height + voffset1, Altitude + Height + voffset2, tw2, th2);
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

int Floor::AddWall(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float voffset1, float voffset2, float tw, float th, bool isexternal)
{
	//Adds a wall with the specified dimensions
	float tw2 = tw;
	float th2;
	float tempw1;
	float tempw2;

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
		int index = sbs->AddWallMain(Level_state, name, texture, thickness, x1, z1, x2, z2, height_in1, height_in2, Altitude + voffset1, Altitude + voffset2, tw2, th2);
		wall_polys.Push(index);
		return wall_polys.GetSize() - 1;
	}
	else
		return sbs->AddWallMain(sbs->External_state, name, texture, thickness, x1, z1, x2, z2, height_in1, height_in2, Altitude + voffset1, Altitude + voffset2, tw2, th2);
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

int Floor::AddInterfloorWall(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float voffset1, float voffset2, float tw, float th)
{
	//Adds an interfloor wall with the specified dimensions
	float tw2 = tw;
	float th2;

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

	int index = sbs->AddWallMain(Interfloor_state, name, texture, thickness, x1, z1, x2, z2, height_in1, height_in2, Altitude + Height + voffset1, Altitude + Height + voffset2, tw2, th2);
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
	for (size_t i = 0; i < CallButtonArray.GetSize(); i++)
		CallButtonArray[i]->Enabled(value);
}

float Floor::FullHeight()
{
	//calculate full height of a floor
	return Height + InterfloorHeight;
}

void Floor::AddCallButtons(csArray<int> &elevators, const char *BackTexture, const char *UpButtonTexture, const char *DownButtonTexture, float CenterX, float CenterZ, float voffset, const char *direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th)
{
	//adds call buttons

	CallButtonArray.SetSize(CallButtonArray.GetSize() + 1);
	int Current = CallButtonArray.GetSize() - 1;
	CallButtonArray[Current] = new CallButton(elevators, Number, Current, BackTexture, UpButtonTexture, DownButtonTexture, CenterX, CenterZ, voffset, direction, BackWidth, BackHeight, ShowBack, tw, th);
}

void Floor::AddGroupFloor(int number)
{
	//adds a floor number to the group list.
	//Groups are used to enable multiple floors at the same time when
	//a user arrives at a floor

	if (Group.Find(number) == csArrayItemNotFound)
		Group.InsertSorted(number);
}

void Floor::RemoveGroupFloor(int number)
{
	//removes a floor number from the group list

	if (Group.Find(number) != csArrayItemNotFound)
		Group.Delete(number);
}

void Floor::EnableGroup(bool value)
{
	//enables or disables all floors in group list

	for (size_t i = 0; i < Group.GetSize(); i++)
		sbs->GetFloor(Group[i])->Enabled(value);
}
