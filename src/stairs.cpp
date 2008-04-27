/* $Id$ */

/*
	Scalable Building Simulator - Stairs Subsystem Class
	The Skyscraper Project - Version 1.1 Alpha
	Copyright (C)2005-2008 Ryan Thoryk
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
#include "stairs.h"
#include "sbs.h"
#include "camera.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

Stairs::Stairs(int number, float CenterX, float CenterZ, int _startfloor, int _endfloor)
{
	StairsNum = number;
	startfloor = _startfloor;
	endfloor = _endfloor;
	origin.x = CenterX;
	origin.z = CenterZ;
	origin.y = sbs->GetFloor(startfloor)->Altitude + sbs->GetFloor(startfloor)->InterfloorHeight;
	cutstart = 0;
	cutend = 0;
	InsideStairwell = false;
	IsEnabled = true;

	csString buffer, buffer2, buffer3;

	StairArray.SetSize(endfloor - startfloor + 1);
	StairArray_state.SetSize(endfloor - startfloor + 1);

	for (int i = startfloor; i <= endfloor; i++)
	{
		//Create stairwell meshes
		buffer2 = number;
		buffer3 = i;
		buffer = "Stairwell " + buffer2 + ":" + buffer3;
		buffer.Trim();
		csRef<iMeshWrapper> tmpmesh;
		csRef<iThingFactoryState> tmpstate;
		tmpmesh = sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData());
		StairArray[i - startfloor] = tmpmesh;
		tmpstate = scfQueryInterface<iThingFactoryState> (StairArray[i - startfloor]->GetMeshObject()->GetFactory());
		StairArray_state[i - startfloor] = tmpstate;
		StairArray[i - startfloor]->SetZBufMode(CS_ZBUF_USE);
		StairArray[i - startfloor]->GetMovable()->SetPosition(csVector3(origin.x, sbs->GetFloor(i)->Altitude + sbs->GetFloor(i)->InterfloorHeight, origin.z));
		StairArray[i - startfloor]->GetMovable()->UpdateMove();
	}
}

Stairs::~Stairs()
{

}

int Stairs::AddStairs(int floor, const char *name, const char *texture, const char *direction, float CenterX, float CenterZ, float width, float risersize, float treadsize, int num_stairs, float voffset, float tw, float th)
{
	//num_stairs is subtracted by 1 since it includes the floor platform above, but not below
	//direction is where the stair base is - front, back, left, or right.

	csString buffer, buffer2, buffer3;
	csString Direction = direction;
	Direction.Downcase();
	int index = -1;
	int tmpindex = 0;
	buffer3 = name;
	buffer3.Trim();

	sbs->ReverseExtents(false, false, false);
	if (Direction == "right" || Direction == "back")
		sbs->SetWallOrientation("right");
	if (Direction == "left" || Direction == "front")
		sbs->SetWallOrientation("left");

	for (int i = 1; i <= num_stairs; i++)
	{
		float pos;
		buffer2 = i;
		float thickness;
		if (i < num_stairs - 1)
			thickness = treadsize * 2;
		if (i == num_stairs - 1)
			thickness = treadsize;
		if (i == num_stairs)
			thickness = 0;

		if (Direction == "right")
		{
			pos = CenterX + ((treadsize * (num_stairs - 1)) / 2) - (treadsize * i);
			buffer = buffer3 + " " + buffer2 + "-riser";
			if (i != num_stairs)
				sbs->DrawWalls(true, true, true, true, false, true);
			else
				sbs->DrawWalls(true, true, false, false, false, false);
			tmpindex = AddWall(floor, buffer.GetData(), texture, thickness, pos + treadsize, -(width / 2) + CenterZ, pos + treadsize, (width / 2) + CenterZ, risersize, risersize, voffset + (risersize * (i - 1)), voffset + (risersize * (i - 1)), tw, th);
			buffer = buffer3 + " " + buffer2 + "-tread";
			if (i != num_stairs)
			{
				sbs->DrawWalls(true, false, false, false, false, false);
				AddFloor(floor, buffer.GetData(), texture, 0, pos, -(width / 2) + CenterZ, pos + treadsize, (width / 2) + CenterZ, voffset + (risersize * i), voffset + (risersize * i), tw, th);
			}
		}
		if (Direction == "left")
		{
			pos = CenterX - ((treadsize * (num_stairs - 1)) / 2) + (treadsize * i);
			buffer = buffer3 + " " + buffer2 + "-riser";
			if (i != num_stairs)
				sbs->DrawWalls(true, true, true, true, false, true);
			else
				sbs->DrawWalls(true, true, false, false, false, false);
			tmpindex = AddWall(floor, buffer.GetData(), texture, thickness, pos - treadsize, (width / 2) + CenterZ, pos - treadsize, -(width / 2) + CenterZ, risersize, risersize, voffset + (risersize * (i - 1)), voffset + (risersize * (i - 1)), tw, th);
			buffer = buffer3 + " " + buffer2 + "-tread";
			if (i != num_stairs)
			{
				sbs->DrawWalls(true, false, false, false, false, false);
				AddFloor(floor, buffer.GetData(), texture, 0, pos, (width / 2) + CenterZ, pos - treadsize, -(width / 2) + CenterZ, voffset + (risersize * i), voffset + (risersize * i), tw, th);
			}
		}
		if (Direction == "back")
		{
			pos = CenterZ + ((treadsize * (num_stairs - 1)) / 2) - (treadsize * i);
			buffer = buffer3 + " " + buffer2 + "-riser";
			if (i != num_stairs)
				sbs->DrawWalls(true, true, true, true, false, true);
			else
				sbs->DrawWalls(true, true, false, false, false, false);
			tmpindex = AddWall(floor, buffer.GetData(), texture, thickness, (width / 2) + CenterX, pos + treadsize, -(width / 2) + CenterX, pos + treadsize, risersize, risersize, voffset + (risersize * (i - 1)), voffset + (risersize * (i - 1)), tw, th);
			buffer = buffer3 + " " + buffer2 + "-tread";
			if (i != num_stairs)
			{
				sbs->DrawWalls(true, false, false, false, false, false);
				AddFloor(floor, buffer.GetData(), texture, 0, (width / 2) + CenterX, pos, -(width / 2) + CenterX, pos + treadsize, voffset + (risersize * i), voffset + (risersize * i), tw, th);
			}
		}
		if (Direction == "front")
		{
			pos = CenterZ - ((treadsize * (num_stairs - 1)) / 2) + (treadsize * i);
			buffer = buffer3 + " " + buffer2 + "-riser";
			if (i != num_stairs)
				sbs->DrawWalls(true, true, true, true, false, true);
			else
				sbs->DrawWalls(true, true, false, false, false, false);
			tmpindex = AddWall(floor, buffer.GetData(), texture, thickness, -(width / 2) + CenterX, pos - treadsize, (width / 2) + CenterX, pos - treadsize, risersize, risersize, voffset + (risersize * (i - 1)), voffset + (risersize * (i - 1)), tw, th);
			buffer = buffer3 + " " + buffer2 + "-tread";
			if (i != num_stairs)
			{
				sbs->DrawWalls(true, false, false, false, false, false);
				AddFloor(floor, buffer.GetData(), texture, 0, -(width / 2) + CenterX, pos, (width / 2), pos - treadsize, voffset + (risersize * i), voffset + (risersize * i), tw, th);
			}
		}

		if (index == -1)
			index = tmpindex;
	}
	sbs->ResetWalls(true);
	sbs->ResetExtents();

	return index;
}

int Stairs::AddWall(int floor, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th)
{
	return sbs->AddWallMain(StairArray_state[floor - startfloor], name, texture, thickness, x1, z1, x2, z2, height1, height2, voffset1, voffset2, tw, th);
}

int Stairs::AddFloor(int floor, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, float tw, float th)
{
	return sbs->AddFloorMain(StairArray_state[floor - startfloor], name, texture, thickness, x1, z1, x2, z2, voffset1, voffset2, tw, th);
}

void Stairs::Enabled(int floor, bool value)
{
	//turns stairwell on/off for a specific floor
	if (value == true)
	{
		StairArray[floor - startfloor]->GetFlags().Reset (CS_ENTITY_INVISIBLEMESH);
		StairArray[floor - startfloor]->GetFlags().Reset (CS_ENTITY_NOSHADOWS);
		StairArray[floor - startfloor]->GetFlags().Reset (CS_ENTITY_NOHITBEAM);
	}
	else
	{
		StairArray[floor - startfloor]->GetFlags().Set (CS_ENTITY_INVISIBLEMESH);
		StairArray[floor - startfloor]->GetFlags().Set (CS_ENTITY_NOSHADOWS);
		StairArray[floor - startfloor]->GetFlags().Set (CS_ENTITY_NOHITBEAM);
	}
}

void Stairs::EnableWholeStairwell(bool value)
{
	//turn on/off entire stairwell
	if ((value == false && IsEnabled == true) || (value == true && IsEnabled == false))
	{
		for (int i = startfloor; i <= endfloor; i++)
			Enabled(i, value);
	}
	IsEnabled = value;
}

bool Stairs::IsInStairwell(const csVector3 &position)
{
	//determine if user is in the stairwell

	float bottom = sbs->GetFloor(startfloor)->Altitude + sbs->GetFloor(startfloor)->InterfloorHeight;
	float top = sbs->GetFloor(endfloor)->Altitude + sbs->GetFloor(endfloor)->FullHeight();

	if (position.y > bottom && position.y < top)
	{
		csHitBeamResult result = StairArray[startfloor]->HitBeam(position, csVector3(position.x, position.y - (top - bottom), position.z));
		return result.hit;
	}
	return false;
}

int Stairs::AddDoor(int floor, const char *texture, float thickness, int direction, float CenterX, float CenterZ, float width, float height, float voffset, float tw, float th)
{
	//interface to the SBS AddDoor function
	
	return sbs->CreateDoor(StairArray_state[floor - startfloor], texture, thickness, direction, CenterX, CenterZ, width, height, voffset + sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight, tw, th);
}

void Stairs::CutFloors(csVector2 start, csVector2 end, float startvoffset, float endvoffset)
{
	//Cut through floor/ceiling polygons on all associated levels, within the voffsets
	
	sbs->Report("Cutting for stairwell " + csString(_itoa(StairsNum, intbuffer, 10)) + "...");
	
	float voffset1, voffset2;
	cutstart = start;
	cutend = end;

	for (int i = startfloor; i <= endfloor; i++)
	{
		voffset1 = 0;
		voffset2 = sbs->GetFloor(i)->FullHeight() + 1;

		if (i == startfloor)
			voffset1 = startvoffset;
		else if (i == endfloor)
			voffset2 = endvoffset;

		sbs->GetFloor(i)->Cut(csVector3(origin.x + start.x, voffset1, origin.z + start.y), csVector3(origin.x + end.x, voffset2, origin.z + end.y), false, true, false);
	}
}

void Stairs::CutWall(int floor, csVector3 start, csVector3 end)
{
	//Cut through a wall segment
	//the Y values in start and end are both relative to the floor's altitude + interfloor

	float base = sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight;
	sbs->Cut(StairArray_state[floor - startfloor], csVector3(start.x, base + start.y, start.z), csVector3(end.x, base + end.y, end.z), true, false);
}

void Stairs::EnableRange(int floor, int range)
{
	//turn on a range of floors
	//if range is 3, show stairwell on current floor (floor), and 1 floor below and above (3 total floors)
	//if range is 1, show only the current floor (floor)

	//range must be greater than 0
	if (range < 1)
		range = 1;

	//range must be an odd number; if it's even, then add 1
	if (IsEven(range) == true)
		range++;

	int additionalfloors;
	if (range > 1)
		additionalfloors = (range - 1) / 2;
	else
		additionalfloors = 0;

	//disable floors 1 floor outside of range
	if (floor - additionalfloors - 1 >= startfloor)
		Enabled(floor - additionalfloors - 1, false);
	if (floor + additionalfloors + 1 <= endfloor)
		Enabled(floor + additionalfloors + 1, false);

	//enable floors within range
	for (int i = floor - additionalfloors; i <= floor + additionalfloors; i++)
	{
		if (i >= startfloor && i <= endfloor)
			Enabled(i, true);
	}
}
