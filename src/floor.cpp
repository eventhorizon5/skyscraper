/* $Id$ */

/*
	Scalable Building Simulator - Floor Class
	The Skyscraper Project - Version 1.1 Alpha
	Copyright (C)2005-2007 Ryan Thoryk
	http://www.skyscrapersim.com
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
	sbs->camera->SetPosition(csVector3(camlocation.x, Altitude + InterfloorHeight + sbs->camera->DefaultAltitude, camlocation.z));
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
		int index = sbs->AddFloorMain(Level_state, name, texture, thickness, x1, z1, x2, z2, Altitude + InterfloorHeight + voffset1, Altitude + InterfloorHeight + voffset2, tw2, th2);
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

	int index = sbs->AddFloorMain(Interfloor_state, name, texture, thickness, x1, z1, x2, z2, Altitude + voffset1, Altitude + voffset2, tw2, th2);
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
		int index = sbs->AddWallMain(Level_state, name, texture, thickness, x1, z1, x2, z2, height_in1, height_in2, Altitude + InterfloorHeight + voffset1, Altitude + InterfloorHeight + voffset2, tw2, th2);
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

	int index = sbs->AddWallMain(Interfloor_state, name, texture, thickness, x1, z1, x2, z2, height_in1, height_in2, Altitude + voffset1, Altitude + voffset2, tw2, th2);
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

void Floor::AddAutoFloor(const char *name, const char *texture, float voffset, float tw, float th)
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

float Floor::FullHeight()
{
	//calculate full height of a floor
	return InterfloorHeight + Height;
}

void Floor::AddCallButtons(csArray<int> &elevators, const char *BackTexture, const char *UpButtonTexture, const char *DownButtonTexture, float CenterX, float CenterZ, float voffset, const char *direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th)
{
	//adds call buttons

	CallButtonArray.SetSize(CallButtonArray.GetSize() + 1);
	int Current = CallButtonArray.GetSize() - 1;
	CallButtonArray[Current] = new CallButton(elevators, Number, Current, BackTexture, UpButtonTexture, DownButtonTexture, CenterX, CenterZ, voffset, direction, BackWidth, BackHeight, ShowBack, tw, th);
}

void Floor::CutFloor(csVector2 start, csVector2 end)
{
	//cuts a rectangular hole in the listed floor polygons (floor, ceiling, etc)

	Cut(csVector3(start.x, 0, start.y), csVector3(end.x, 0, end.y), false);
}

void Floor::CutWall(csVector3 start, csVector3 end)
{
	//cuts a rectangular hole in the listed floor polygons (floor, ceiling, etc)

	Cut(start, end, true);
}

void Floor::Cut(csVector3 start, csVector3 end, bool IsWall)
{
	//cuts a rectangular hole in the listed floor polygons (floor, ceiling, etc)

	csPoly3D temppoly, temppoly2, temppoly3, temppoly4, temppoly5;
	int addpolys;
	int tmpindex = -1;
	csArray<int> *polys;

	if (IsWall == true)
		polys = &floor_polys;
	else
		polys = &wall_polys;

	//step through each floor polygon (floor, ceiling, etc)
	for (size_t i = 0; i <= polys->GetSize() - 1; i++)
	{
		temppoly.MakeEmpty();
		temppoly2.MakeEmpty();
		temppoly3.MakeEmpty();
		temppoly4.MakeEmpty();
		temppoly5.MakeEmpty();
		addpolys = 0;

		//copy polygon vertices
		for (int j = 0; j <= Level_state->GetPolygonVertexCount(polys->Get(i)); j++)
			temppoly.AddVertex(Level_state->GetPolygonVertex(polys->Get(i), j));

		if (IsWall == true)
		{
			if (start.x - end.x > start.z - end.z)
			{
				//wall is facing forward/backward

				//get left side
				temppoly.SplitWithPlaneX(temppoly2, temppoly, start.x);

				//get right side
				temppoly2.SplitWithPlaneX(temppoly2, temppoly3, end.x);

				//get lower
				temppoly3.SplitWithPlaneY(temppoly4, temppoly3, start.y);

				//get upper
				temppoly4.SplitWithPlaneY(temppoly4, temppoly5, end.y);
			}
			else
			{
				//wall is facing left/right

				//get left side
				temppoly.SplitWithPlaneZ(temppoly2, temppoly, start.z);

				//get right side
				temppoly2.SplitWithPlaneZ(temppoly2, temppoly3, end.z);

				//get lower
				temppoly3.SplitWithPlaneY(temppoly4, temppoly3, start.y);

				//get upper
				temppoly4.SplitWithPlaneY(temppoly4, temppoly5, end.y);
			}
		}
		else
		{
			//get left side
			temppoly.SplitWithPlaneX(temppoly2, temppoly, start.x);

			//get right side
			temppoly2.SplitWithPlaneX(temppoly2, temppoly3, end.x);

			//get lower
			temppoly3.SplitWithPlaneZ(temppoly4, temppoly3, start.z);

			//get upper
			temppoly4.SplitWithPlaneZ(temppoly4, temppoly5, end.z);
		}

		//delete original polygon
		Level_state->RemovePolygon(polys->Get(i));
		polys->DeleteIndex(i);

		//create splitted polygons
		if (temppoly.GetVertexCount() > 0)
		{
			addpolys++;
			tmpindex = Level_state->AddQuad(temppoly[1], temppoly[2], temppoly[3], temppoly[4]);
			polys->Insert(i, tmpindex);
			i++;
		}
		if (temppoly2.GetVertexCount() > 0)
		{
			addpolys++;
			tmpindex = Level_state->AddQuad(temppoly2[1], temppoly2[2], temppoly2[3], temppoly2[4]);
			polys->Insert(i, tmpindex);
			i++;
		}
		if (temppoly3.GetVertexCount() > 0)
		{
			addpolys++;
			tmpindex = Level_state->AddQuad(temppoly3[1], temppoly3[2], temppoly3[3], temppoly3[4]);
			polys->Insert(i, tmpindex);
			i++;
		}
		if (temppoly4.GetVertexCount() > 0)
		{
			addpolys++;
			tmpindex = Level_state->AddQuad(temppoly4[1], temppoly4[2], temppoly4[3], temppoly4[4]);
			polys->Insert(i, tmpindex);
			i++;
		}
	}
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
