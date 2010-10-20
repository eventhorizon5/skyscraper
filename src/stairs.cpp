/* $Id$ */

/*
	Scalable Building Simulator - Stairs Subsystem Class
	The Skyscraper Project - Version 1.8 Alpha
	Copyright (C)2004-2010 Ryan Thoryk
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
#include "sbs.h"
#include "stairs.h"
#include "camera.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

Stairs::Stairs(int number, float CenterX, float CenterZ, int _startfloor, int _endfloor)
{
	//set up SBS object
	object = new Object();
	object->SetValues(this, sbs->object, "Stairs", "", false);

	StairsNum = number;
	startfloor = _startfloor;
	endfloor = _endfloor;
	origin.x = CenterX;
	origin.z = CenterZ;
	origin.y = sbs->GetFloor(startfloor)->GetBase();
	cutstart = 0;
	cutend = 0;
	InsideStairwell = false;
	IsEnabled = true;
	lastfloor = 0;
	lastfloorset = false;
	lastcheckresult = false;
	checkfirstrun = true;

	csString buffer, buffer2, buffer3;

	buffer = number;
	object->SetName("Stairwell " + buffer);

	StairArray.SetSize(endfloor - startfloor + 1);
	EnableArray.SetSize(endfloor - startfloor + 1);
	ModelArray.SetSize(endfloor - startfloor + 1);

	for (int i = startfloor; i <= endfloor; i++)
	{
		//Create stairwell meshes
		buffer2 = number;
		buffer3 = i;
		buffer = "Stairwell " + buffer2 + ":" + buffer3;
		buffer.Trim();
		StairArray[i - startfloor] = new MeshObject(object, buffer);
		EnableArray[i - startfloor] = true;
	}
}

Stairs::~Stairs()
{
	//delete models
	for (int i = 0; i < ModelArray.GetSize(); i++)
	{
		for (int j = 0; j < ModelArray[i].GetSize(); j++)
		{
			if (ModelArray[i][j])
				delete ModelArray[i][j];
			ModelArray[i][j] = 0;
		}
	}

	//delete lights
	for (int i = 0; i < lights.GetSize(); i++)
	{
		for (int j = 0; j < lights[i].GetSize(); j++)
		{
			if (lights[i][j])
				delete lights[i][j];
			lights[i][j] = 0;
		}
	}

	//delete doors
	for (int i = 0; i < DoorArray.GetSize(); i++)
	{
		if (DoorArray[i].object)
		{
			DoorArray[i].object->object->parent_deleting = true;
			delete DoorArray[i].object;
		}
		DoorArray[i].object = 0;
	}
	DoorArray.DeleteAll();

	//delete mesh array objects
	for (int i = 0; i < StairArray.GetSize(); i++)
	{
		if (StairArray[i])
			delete StairArray[i];
		StairArray[i] = 0;
	}
	for (int i = 0; i < StairDoorArray.GetSize(); i++)
	{
		if (StairDoorArray[i])
			delete StairDoorArray[i];
		StairDoorArray[i] = 0;
	}
	StairArray.DeleteAll();
	StairDoorArray.DeleteAll();

	//unregister from parent
	if (sbs->FastDelete == false && object->parent_deleting == false)
		sbs->RemoveStairs(this);

	delete object;
}

WallObject* Stairs::AddStairs(int floor, const char *name, const char *texture, const char *direction, float CenterX, float CenterZ, float width, float risersize, float treadsize, int num_stairs, float voffset, float tw, float th)
{
	//num_stairs is subtracted by 1 since it includes the floor platform above, but not below
	//direction is where the stair base is - front, back, left, or right.

	//exit with an error if floor is invalid
	if (IsValidFloor(floor) == false)
	{
		ReportError("AddStairs: Floor " + csString(_itoa(floor, intbuffer, 10)) + " out of range");
		return 0;
	}

	//create wall object
	WallObject *wall = GetMeshObject(floor)->CreateWallObject(this->object, name);

	csString buffer, buffer2, buffer3;
	csString Direction = direction;
	Direction.Downcase();
	buffer3 = name;
	buffer3.Trim();

	sbs->ResetTextureMapping(true);
	if (Direction == "right" || Direction == "back")
		sbs->SetWallOrientation("right");
	if (Direction == "left" || Direction == "front")
		sbs->SetWallOrientation("left");

	for (int i = 1; i <= num_stairs; i++)
	{
		float pos = 0;
		buffer2 = i;
		float thickness = 0;
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
			AddWall(wall, floor, buffer, texture, thickness, pos + treadsize, -(width / 2) + CenterZ, pos + treadsize, (width / 2) + CenterZ, risersize, risersize, voffset + (risersize * (i - 1)), voffset + (risersize * (i - 1)), tw, th);
			buffer = buffer3 + " " + buffer2 + "-tread";
			if (i != num_stairs)
			{
				sbs->DrawWalls(false, true, false, false, false, false);
				AddFloor(wall, floor, buffer, texture, 0, pos, -(width / 2) + CenterZ, pos + treadsize, (width / 2) + CenterZ, voffset + (risersize * i), voffset + (risersize * i), tw, th);
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
			AddWall(wall, floor, buffer, texture, thickness, pos - treadsize, (width / 2) + CenterZ, pos - treadsize, -(width / 2) + CenterZ, risersize, risersize, voffset + (risersize * (i - 1)), voffset + (risersize * (i - 1)), tw, th);
			buffer = buffer3 + " " + buffer2 + "-tread";
			if (i != num_stairs)
			{
				sbs->DrawWalls(false, true, false, false, false, false);
				AddFloor(wall, floor, buffer, texture, 0, pos - treadsize, -(width / 2) + CenterZ, pos, (width / 2) + CenterZ, voffset + (risersize * i), voffset + (risersize * i), tw, th);
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
			AddWall(wall, floor, buffer, texture, thickness, (width / 2) + CenterX, pos + treadsize, -(width / 2) + CenterX, pos + treadsize, risersize, risersize, voffset + (risersize * (i - 1)), voffset + (risersize * (i - 1)), tw, th);
			buffer = buffer3 + " " + buffer2 + "-tread";
			if (i != num_stairs)
			{
				sbs->DrawWalls(false, true, false, false, false, false);
				AddFloor(wall, floor, buffer, texture, 0, -(width / 2) + CenterX, pos, (width / 2) + CenterX, pos + treadsize, voffset + (risersize * i), voffset + (risersize * i), tw, th);
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
			AddWall(wall, floor, buffer, texture, thickness, -(width / 2) + CenterX, pos - treadsize, (width / 2) + CenterX, pos - treadsize, risersize, risersize, voffset + (risersize * (i - 1)), voffset + (risersize * (i - 1)), tw, th);
			buffer = buffer3 + " " + buffer2 + "-tread";
			if (i != num_stairs)
			{
				sbs->DrawWalls(false, true, false, false, false, false);
				AddFloor(wall, floor, buffer, texture, 0, -(width / 2) + CenterX, pos - treadsize, (width / 2) + CenterX, pos, voffset + (risersize * i), voffset + (risersize * i), tw, th);
			}
		}
	}
	sbs->ResetWalls(true);
	sbs->ResetTextureMapping();

	return wall;
}

WallObject* Stairs::AddWall(int floor, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th)
{
	//exit with an error if floor is invalid
	if (IsValidFloor(floor) == false)
	{
		ReportError("AddWall: Floor " + csString(_itoa(floor, intbuffer, 10)) + " out of range");
		return 0;
	}

	WallObject *wall = GetMeshObject(floor)->CreateWallObject(this->object, name);
	AddWall(wall, floor, name, texture, thickness, x1, z1, x2, z2, height1, height2, voffset1, voffset2, tw, th);
	return wall;
}

int Stairs::AddWall(WallObject *wall, int floor, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th)
{
	return sbs->AddWallMain(wall, name, texture, thickness, origin.x + x1, origin.z + z1, origin.x + x2, origin.z + z2, height1, height2, sbs->GetFloor(floor)->GetBase() + voffset1, sbs->GetFloor(floor)->GetBase() + voffset2, tw, th, true);
}

WallObject* Stairs::AddFloor(int floor, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, float tw, float th)
{
	//exit with an error if floor is invalid
	if (IsValidFloor(floor) == false)
	{
		ReportError("AddFloor: Floor " + csString(_itoa(floor, intbuffer, 10)) + " out of range");
		return 0;
	}

	WallObject *wall = GetMeshObject(floor)->CreateWallObject(this->object, name);
	AddFloor(wall, floor, name, texture, thickness, x1, z1, x2, z2, voffset1, voffset2, tw, th);
	return wall;
}

int Stairs::AddFloor(WallObject *wall, int floor, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, float tw, float th)
{
	return sbs->AddFloorMain(wall, name, texture, thickness, origin.x + x1, origin.z + z1, origin.x + x2, origin.z + z2, sbs->GetFloor(floor)->GetBase() + voffset1, sbs->GetFloor(floor)->GetBase() + voffset2, tw, th, true);
}

void Stairs::Enabled(int floor, bool value)
{
	//turns stairwell on/off for a specific floor

	if (IsEnabledFloor(floor) != value && floor >= startfloor && floor <= endfloor)
	{
		GetMeshObject(floor)->Enable(value);
		EnableArray[floor - startfloor] = value;

		//models
		for (size_t i = 0; i < ModelArray[floor - startfloor].GetSize(); i++)
		{
			if (ModelArray[floor - startfloor][i])
				ModelArray[floor - startfloor][i]->Enable(value);
		}

		//enable/disable door
		EnableDoor(floor, value);
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

	//if last position is the same as new, return previous result
	if (position == lastposition && checkfirstrun == false)
		return lastcheckresult;

	bool hit = false;
	bool hittmp = false;
	float bottom = sbs->GetFloor(startfloor)->GetBase();
	float top = sbs->GetFloor(endfloor)->Altitude + sbs->GetFloor(endfloor)->FullHeight();

	//determine floor
	int floor;

	if (lastfloorset == true)
		floor = sbs->GetFloorNumber(position.y, lastfloor, true);
	else
		floor = sbs->GetFloorNumber(position.y);

	lastfloor = floor;
	lastfloorset = true;

	Floor *floorptr = sbs->GetFloor(floor);

	if (position.y > bottom && position.y < top)
	{
		//check both the current floor and floor below
		csVector3 endposition;
		endposition.Set(position.x, position.y - floorptr->FullHeight(), position.z);
		if (floor > startfloor)
			hit = GetMeshObject(floor - 1)->MeshWrapper->HitBeam(sbs->ToRemote(position), sbs->ToRemote(endposition)).hit;
		if (floor >= startfloor && floor <= endfloor)
		{
			hittmp = GetMeshObject(floor)->MeshWrapper->HitBeam(sbs->ToRemote(position), sbs->ToRemote(endposition)).hit;
			if (hittmp == true)
				hit = true;
		}
	}
	floorptr = 0;

	//cache values
	checkfirstrun = false;
	lastcheckresult = hit;
	lastposition = position;

	return hit;
}

Object* Stairs::AddDoor(int floor, const char *open_sound, const char *close_sound, bool open_state, const char *texture, float thickness, int direction, float speed, float CenterX, float CenterZ, float width, float height, float voffset, float tw, float th)
{
	//add a door

	//exit with an error if floor is invalid
	if (IsValidFloor(floor) == false)
	{
		ReportError("AddDoor: Floor " + csString(_itoa(floor, intbuffer, 10)) + " out of range");
		return 0;
	}

	Floor *floorptr = sbs->GetFloor(floor);
	float x1, z1, x2, z2;
	//set up coordinates
	if (direction < 5)
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

	//cut area
	if (direction < 5)
	{
		CutWall(1, floor, csVector3(x1 - 0.5, voffset, z1), csVector3(x2 + 0.5, voffset + height, z2), 1, "Stairs");
		floorptr->Cut(csVector3(origin.x + x1 - 0.5, floorptr->GetBase(true) + voffset, origin.z + z1), csVector3(origin.x + x2 + 0.5, floorptr->GetBase(true) + voffset + height, origin.z + z2), true, false, true, 2, "Stairs");
	}
	else
	{
		CutWall(1, floor, csVector3(x1, voffset, z1 - 0.5), csVector3(x2, voffset + height, z2 + 0.5), 1, "Stairs");
		floorptr->Cut(csVector3(origin.x + x1, floorptr->GetBase(true) + voffset, origin.z + z1 - 0.5), csVector3(origin.x + x2, floorptr->GetBase(true) + voffset + height, origin.z + z2 + 0.5), true, false, true, 2, "Stairs");
	}

	//create doorway walls
	WallObject *wall = GetMeshObject(floor)->CreateWallObject(object, "Connection Walls");
	sbs->AddDoorwayWalls(wall, "ConnectionWall", 0, 0);

	DoorArray.SetSize(DoorArray.GetSize() + 1);
	DoorArray[DoorArray.GetSize() - 1].floornumber = floor;
	csString stairsnum = _itoa(StairsNum, intbuffer, 10);
	csString num = _itoa(DoorArray.GetSize() - 1, intbuffer, 10);
	DoorArray[DoorArray.GetSize() - 1].object = new Door(this->object, "Stairwell " + stairsnum + ":Door " + num, open_sound, close_sound, open_state, texture, thickness, direction, speed, origin.x + CenterX, origin.z + CenterZ, width, height, floorptr->Altitude + floorptr->GetBase(true) + voffset, tw, th);
	floorptr = 0;
	return DoorArray[DoorArray.GetSize() - 1].object->object;
}

void Stairs::CutFloors(bool relative, const csVector2 &start, const csVector2 &end, float startvoffset, float endvoffset)
{
	//Cut through floor/ceiling polygons on all associated levels, within the voffsets

	sbs->Report("Cutting for stairwell " + csString(_itoa(StairsNum, intbuffer, 10)) + "...");

	float voffset1, voffset2;
	cutstart = start;
	cutend = end;

	for (int i = startfloor; i <= endfloor; i++)
	{
		Floor *floorptr = sbs->GetFloor(i);
		voffset1 = 0;
		voffset2 = sbs->GetFloor(i)->FullHeight() + 1;

		if (i == startfloor)
			voffset1 = startvoffset;
		else if (i == endfloor)
			voffset2 = endvoffset;

		if (relative == true)
			floorptr->Cut(csVector3(origin.x + start.x, voffset1, origin.z + start.y), csVector3(origin.x + end.x, voffset2, origin.z + end.y), false, true, false);
		else
			floorptr->Cut(csVector3(start.x, voffset1, start.y), csVector3(end.x, voffset2, end.y), false, true, false);
		floorptr = 0;
	}

	//cut external
	voffset1 = sbs->GetFloor(startfloor)->Altitude + startvoffset;
	voffset2 = sbs->GetFloor(endfloor)->Altitude + sbs->GetFloor(endfloor)->FullHeight() + endvoffset;
	for (int i = 0; i < sbs->External->Walls.GetSize(); i++)
	{
		if (relative == true)
			sbs->Cut(sbs->External->Walls[i], csVector3(origin.x + start.x, voffset1, origin.z + start.y), csVector3(origin.x + end.x, voffset2, origin.z + end.y), false, true, csVector3(0, 0, 0), csVector3(0, 0, 0));
		else
			sbs->Cut(sbs->External->Walls[i], csVector3(start.x, voffset1, start.y), csVector3(end.x, voffset2, end.y), false, true, csVector3(0, 0, 0), csVector3(0, 0, 0));
	}
}

bool Stairs::CutWall(bool relative, int floor, const csVector3 &start, const csVector3 &end, int checkwallnumber, const char *checkstring)
{
	//Cut through a wall segment
	//the Y values in start and end are both relative to the floor's altitude + interfloor

	//exit with an error if floor is invalid
	if (IsValidFloor(floor) == false)
	{
		if (sbs->Verbose)
			ReportError("CutWall: Floor " + csString(_itoa(floor, intbuffer, 10)) + " out of range");
		return false;
	}

	float base = sbs->GetFloor(floor)->GetBase();

	for (int i = 0; i < GetMeshObject(floor)->Walls.GetSize(); i++)
	{
		bool reset = true;
		if (i > 0)
			reset = false;

		if (relative == true)
			sbs->Cut(GetMeshObject(floor)->Walls[i], csVector3(origin.x + start.x, base + start.y, origin.z + start.z), csVector3(origin.x + end.x, base + end.y, origin.z + end.z), true, false, csVector3(0, 0, 0), origin, checkwallnumber, checkstring, reset);
		else
			sbs->Cut(GetMeshObject(floor)->Walls[i], csVector3(start.x, base + start.y, start.z), csVector3(end.x, base + end.y, end.z), true, false, csVector3(0, 0, 0), origin, checkwallnumber, checkstring, reset);
	}

	return true;
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
	if (floor - additionalfloors - 1 >= startfloor && floor - additionalfloors - 1 <= endfloor)
	{
		if (sbs->GetFloor(floor)->IsInGroup(floor - additionalfloors - 1) == false) //only disable if not in group
			Enabled(floor - additionalfloors - 1, false);
	}
	if (floor + additionalfloors + 1 >= startfloor && floor + additionalfloors + 1 <= endfloor)
	{
		if (sbs->GetFloor(floor)->IsInGroup(floor + additionalfloors + 1) == false) //only disable if not in group
			Enabled(floor + additionalfloors + 1, false);
	}

	//enable floors within range
	for (int i = floor - additionalfloors; i <= floor + additionalfloors; i++)
	{
		if (i >= startfloor && i <= endfloor)
			Enabled(i, true);
	}
}

void Stairs::EnableDoor(int floor, bool value)
{
	//turn on door(s) on the specified floor

	for (int i = 0; i < DoorArray.GetSize(); i++)
	{
		if (DoorArray[i].floornumber == floor && DoorArray[i].object)
			DoorArray[i].object->Enabled(value);
	}
}

void Stairs::OpenDoor(int number)
{
	//open door
	if (number < DoorArray.GetSize())
	{
		if (DoorArray[number].object)
			DoorArray[number].object->Open();
	}
	else
		Report("Invalid door " + csString(_itoa(number, intbuffer, 10)));
}

void Stairs::CloseDoor(int number)
{
	//close door
	if (number < DoorArray.GetSize())
	{
		if (DoorArray[number].object)
			DoorArray[number].object->Close();
	}
	else
		Report("Invalid door " + csString(_itoa(number, intbuffer, 10)));
}

bool Stairs::IsDoorOpen(int number)
{
	//check to see if door is open
	if (number < DoorArray.GetSize())
	{
		if (DoorArray[number].object)
			return DoorArray[number].object->IsOpen();
	}
	else
		Report("Invalid door " + csString(_itoa(number, intbuffer, 10)));
	return false;
}

bool Stairs::IsEnabledFloor(int floor)
{
	if (floor >= startfloor && floor <= endfloor)
		return EnableArray[floor - startfloor];
	else
		return false;
}

bool Stairs::IsValidFloor(int floor)
{
	//return true if the stairwell services the specified floor

	if (floor < startfloor || floor > endfloor)
		return false;

	if (!StairArray[floor - startfloor])
		return false;

	return true;
}

bool Stairs::IsDoorMoving(int number)
{
	//check to see if door is moving
	if (number < DoorArray.GetSize())
	{
		if (DoorArray[number].object)
			return DoorArray[number].object->IsMoving;
	}
	else
		Report("Invalid door " + csString(_itoa(number, intbuffer, 10)));
	return false;
}

void Stairs::Report(const char *message)
{
	//general reporting function
	sbs->Report("Stairwell " + csString(_itoa(StairsNum, intbuffer, 10)) + ": " + message);
}

bool Stairs::ReportError(const char *message)
{
	//general reporting function
	return sbs->ReportError("Stairwell " + csString(_itoa(StairsNum, intbuffer, 10)) + ": " + message);
}

void Stairs::RemoveDoor(Door *door)
{
	//remove a door from the array (this does not delete the object)
	for (int i = 0; i < DoorArray.GetSize(); i++)
	{
		if (DoorArray[i].object == door)
		{
			DoorArray.DeleteIndex(i);
			return;
		}
	}
}

Object* Stairs::AddLight(int floor, const char *name, int type, csVector3 position, csVector3 direction, float radius, float max_distance, float color_r, float color_g, float color_b, float spec_color_r, float spec_color_g, float spec_color_b, float directional_cutoff_radius, float spot_falloff_inner, float spot_falloff_outer)
{
	//add a global light

	//exit if floor is invalid
	if (!IsValidFloor(floor))
		return 0;

	Light* light = new Light(name, type, position + csVector3(origin.x, sbs->GetFloor(floor)->Altitude, origin.z), direction, radius, max_distance, color_r, color_g, color_b, spec_color_r, spec_color_g, spec_color_b, directional_cutoff_radius, spot_falloff_inner, spot_falloff_outer);
	lights[floor - startfloor].Push(light);
	return light->object;
}

MeshObject* Stairs::GetMeshObject(int floor)
{
	//returns the mesh object for the specified floor

	if (!IsValidFloor(floor))
		return 0;

	return StairArray[floor - startfloor];
}

Object* Stairs::AddModel(int floor, const char *name, const char *filename, csVector3 position, csVector3 rotation, float max_render_distance, float scale_multiplier)
{
	//add a model

	//exit if floor is invalid
	if (!IsValidFloor(floor))
		return 0;

	Model* model = new Model(name, filename, position + csVector3(origin.x, sbs->GetFloor(floor)->Altitude, origin.z), rotation, max_render_distance, scale_multiplier);
	if (model->load_error == true)
	{
		delete model;
		return 0;
	}
	ModelArray[floor - startfloor].Push(model);
	return model->object;
}
