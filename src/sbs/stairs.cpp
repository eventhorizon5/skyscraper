/* $Id$ */

/*
	Scalable Building Simulator - Stairwell Class
	The Skyscraper Project - Version 1.9 Alpha
	Copyright (C)2004-2015 Ryan Thoryk
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
	lastposition = 0;
	ShowFloors = false;
	ShowFullStairs = false;

	std::string buffer, buffer2, buffer3;

	buffer = ToString(number);
	object->SetName(std::string("Stairwell " + buffer).c_str());

	StairArray.resize(endfloor - startfloor + 1);
	EnableArray.resize(endfloor - startfloor + 1);
	ModelArray.resize(endfloor - startfloor + 1);
	ControlArray.resize(endfloor - startfloor + 1);
	//TriggerArray.resize(endfloor - startfloor + 1);

	for (int i = startfloor; i <= endfloor; i++)
	{
		//Create stairwell meshes
		buffer2 = ToString(number);
		buffer3 = ToString(i);
		buffer = "Stairwell " + buffer2 + ":" + buffer3;
		TrimString(buffer);
		StairArray[i - startfloor] = new MeshObject(object, buffer.c_str());
		EnableArray[i - startfloor] = true;
	}
}

Stairs::~Stairs()
{
	//delete controls
	for (int i = 0; i < (int)ControlArray.size(); i++)
	{
		for (int j = 0; j < (int)ControlArray[i].size(); j++)
		{
			if (ControlArray[i][j])
			{
				ControlArray[i][j]->object->parent_deleting = true;
				delete ControlArray[i][j];
			}
			ControlArray[i][j] = 0;
		}
	}

	//delete triggers
	/*for (int i = 0; i < (int)TriggerArray.size(); i++)
	{
		for (int j = 0; j < (int)TriggerArray[i].size(); j++)
		{
			if (TriggerArray[i][j])
			{
				TriggerArray[i][j]->object->parent_deleting = true;
				delete TriggerArray[i][j];
			}
			TriggerArray[i][j] = 0;
		}
	}*/

	//delete models
	for (int i = 0; i < (int)ModelArray.size(); i++)
	{
		for (int j = 0; j < (int)ModelArray[i].size(); j++)
		{
			if (ModelArray[i][j])
			{
				ModelArray[i][j]->object->parent_deleting = true;
				delete ModelArray[i][j];
			}
			ModelArray[i][j] = 0;
		}
	}

	//delete lights
	for (int i = 0; i < (int)lights.size(); i++)
	{
		for (int j = 0; j < (int)lights[i].size(); j++)
		{
			if (lights[i][j])
			{
				lights[i][j]->object->parent_deleting = true;
				delete lights[i][j];
			}
			lights[i][j] = 0;
		}
	}

	//delete doors
	for (int i = 0; i < (int)DoorArray.size(); i++)
	{
		if (DoorArray[i].object)
		{
			DoorArray[i].object->object->parent_deleting = true;
			delete DoorArray[i].object;
		}
		DoorArray[i].object = 0;
	}
	DoorArray.clear();

	//delete mesh array objects
	for (int i = 0; i < (int)StairArray.size(); i++)
	{
		if (StairArray[i])
			delete StairArray[i];
		StairArray[i] = 0;
	}
	StairArray.clear();

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
		ReportError("AddStairs: Floor " + ToString2(floor) + " out of range");
		return 0;
	}

	//create wall object
	WallObject *wall = GetMeshObject(floor)->CreateWallObject(object, name);

	std::string buffer, buffer2, buffer3;
	std::string Direction = direction;
	SetCase(Direction, false);
	buffer3 = name;
	TrimString(buffer3);

	sbs->ResetTextureMapping(true);
	if (Direction == "right" || Direction == "back")
		sbs->SetWallOrientation("right");
	if (Direction == "left" || Direction == "front")
		sbs->SetWallOrientation("left");

	for (int i = 1; i <= num_stairs; i++)
	{
		float pos = 0;
		buffer2 = ToString(i);
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
			AddWall(wall, floor, buffer.c_str(), texture, thickness, pos + treadsize, -(width / 2) + CenterZ, pos + treadsize, (width / 2) + CenterZ, risersize, risersize, voffset + (risersize * (i - 1)), voffset + (risersize * (i - 1)), tw, th);
			buffer = buffer3 + " " + buffer2 + "-tread";
			if (i != num_stairs)
			{
				sbs->DrawWalls(false, true, false, false, false, false);
				AddFloor(wall, floor, buffer.c_str(), texture, 0, pos, -(width / 2) + CenterZ, pos + treadsize, (width / 2) + CenterZ, voffset + (risersize * i), voffset + (risersize * i), false, false, tw, th);
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
			AddWall(wall, floor, buffer.c_str(), texture, thickness, pos - treadsize, (width / 2) + CenterZ, pos - treadsize, -(width / 2) + CenterZ, risersize, risersize, voffset + (risersize * (i - 1)), voffset + (risersize * (i - 1)), tw, th);
			buffer = buffer3 + " " + buffer2 + "-tread";
			if (i != num_stairs)
			{
				sbs->DrawWalls(false, true, false, false, false, false);
				AddFloor(wall, floor, buffer.c_str(), texture, 0, pos - treadsize, -(width / 2) + CenterZ, pos, (width / 2) + CenterZ, voffset + (risersize * i), voffset + (risersize * i), false, false, tw, th);
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
			AddWall(wall, floor, buffer.c_str(), texture, thickness, (width / 2) + CenterX, pos + treadsize, -(width / 2) + CenterX, pos + treadsize, risersize, risersize, voffset + (risersize * (i - 1)), voffset + (risersize * (i - 1)), tw, th);
			buffer = buffer3 + " " + buffer2 + "-tread";
			if (i != num_stairs)
			{
				sbs->DrawWalls(false, true, false, false, false, false);
				AddFloor(wall, floor, buffer.c_str(), texture, 0, -(width / 2) + CenterX, pos, (width / 2) + CenterX, pos + treadsize, voffset + (risersize * i), voffset + (risersize * i), false, false, tw, th);
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
			AddWall(wall, floor, buffer.c_str(), texture, thickness, -(width / 2) + CenterX, pos - treadsize, (width / 2) + CenterX, pos - treadsize, risersize, risersize, voffset + (risersize * (i - 1)), voffset + (risersize * (i - 1)), tw, th);
			buffer = buffer3 + " " + buffer2 + "-tread";
			if (i != num_stairs)
			{
				sbs->DrawWalls(false, true, false, false, false, false);
				AddFloor(wall, floor, buffer.c_str(), texture, 0, -(width / 2) + CenterX, pos - treadsize, (width / 2) + CenterX, pos, voffset + (risersize * i), voffset + (risersize * i), false, false, tw, th);
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
		ReportError("AddWall: Floor " + ToString2(floor) + " out of range");
		return 0;
	}

	WallObject *wall = GetMeshObject(floor)->CreateWallObject(object, name);
	AddWall(wall, floor, name, texture, thickness, x1, z1, x2, z2, height1, height2, voffset1, voffset2, tw, th);
	return wall;
}

bool Stairs::AddWall(WallObject *wall, int floor, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th)
{
	//exit with an error if floor is invalid
	if (IsValidFloor(floor) == false)
		return ReportError("AddWall: Floor " + ToString2(floor) + " out of range");

	return sbs->AddWallMain(wall, name, texture, thickness, origin.x + x1, origin.z + z1, origin.x + x2, origin.z + z2, height1, height2, sbs->GetFloor(floor)->GetBase() + voffset1, sbs->GetFloor(floor)->GetBase() + voffset2, tw, th, true);
}

WallObject* Stairs::AddFloor(int floor, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, bool reverse_axis, bool texture_direction, float tw, float th, bool legacy_behavior)
{
	//exit with an error if floor is invalid
	if (IsValidFloor(floor) == false)
	{
		ReportError("AddFloor: Floor " + ToString2(floor) + " out of range");
		return 0;
	}

	WallObject *wall = GetMeshObject(floor)->CreateWallObject(object, name);
	AddFloor(wall, floor, name, texture, thickness, x1, z1, x2, z2, voffset1, voffset2, reverse_axis, texture_direction, tw, th, legacy_behavior);
	return wall;
}

bool Stairs::AddFloor(WallObject *wall, int floor, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, bool reverse_axis, bool texture_direction, float tw, float th, bool legacy_behavior)
{
	//exit with an error if floor is invalid
	if (IsValidFloor(floor) == false)
		return ReportError("AddFloor: Floor " + ToString2(floor) + " out of range");

	return sbs->AddFloorMain(wall, name, texture, thickness, origin.x + x1, origin.z + z1, origin.x + x2, origin.z + z2, sbs->GetFloor(floor)->GetBase() + voffset1, sbs->GetFloor(floor)->GetBase() + voffset2, reverse_axis, texture_direction, tw, th, true, legacy_behavior);
}

void Stairs::Enabled(int floor, bool value)
{
	//turns stairwell on/off for a specific floor

	SBS_PROFILE("Stairs::Enabled");
	if (IsEnabledFloor(floor) != value && floor >= startfloor && floor <= endfloor)
	{
		GetMeshObject(floor)->Enable(value);
		EnableArray[floor - startfloor] = value;

		//controls
		for (size_t i = 0; i < ControlArray[floor - startfloor].size(); i++)
		{
			if (ControlArray[floor - startfloor][i])
				ControlArray[floor - startfloor][i]->Enabled(value);
		}

		//triggers
		/*for (size_t i = 0; i < TriggerArray[floor - startfloor].size(); i++)
		{
			if (TriggerArray[floor - startfloor][i])
				TriggerArray[floor - startfloor][i]->Enabled(value);
		}*/

		//models
		for (size_t i = 0; i < ModelArray[floor - startfloor].size(); i++)
		{
			if (ModelArray[floor - startfloor][i])
				ModelArray[floor - startfloor][i]->Enable(value);
		}

		//enable/disable door
		EnableDoor(floor, value);
	}
}

void Stairs::EnableWholeStairwell(bool value, bool force)
{
	//turn on/off entire stairwell

	if (force == true)
		IsEnabled = !value;

	if (IsEnabled == !value)
	{
		for (int i = startfloor; i <= endfloor; i++)
		{
			if (force == true)
				EnableArray[i - startfloor] = !value;
			Enabled(i, value);
		}
	}
	IsEnabled = value;
}

bool Stairs::IsInStairwell(const Ogre::Vector3 &position)
{
	//determine if user is in the stairwell

	//SBS_PROFILE("Stairs::IsInStairwell");

	//if last position is the same as new, return previous result
	if ((position.x >= (lastposition.x - 0.01)) &&
		(position.y >= (lastposition.y - 0.01)) &&
		(position.z >= (lastposition.z - 0.01)) &&
		(position.x <= (lastposition.x + 0.01)) &&
		(position.y <= (lastposition.y + 0.01)) &&
		(position.z <= (lastposition.z + 0.01)) &&
		checkfirstrun == false)
		return lastcheckresult;

	if (!sbs->GetFloor(startfloor) || !sbs->GetFloor(endfloor))
		return false;

	bool hit = false;
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
	if (!floorptr)
		return false;

	if (position.y > bottom && position.y < top)
	{
		//check for hit with current floor
		float distance = floorptr->FullHeight();
		hit = GetMeshObject(floor)->HitBeam(position, Ogre::Vector3::NEGATIVE_UNIT_Y, distance) >= 0;

		//if no hit, check hit against lower floor
		if (hit == false && sbs->GetFloor(floor - 1) && floor > startfloor)
		{
			distance = position.y - sbs->GetFloor(floor - 1)->Altitude;
			hit = GetMeshObject(floor - 1)->HitBeam(position, Ogre::Vector3::NEGATIVE_UNIT_Y, distance) >= 0;
		}

		//if no hit, check hit against starting floor
		if (hit == false && sbs->GetFloor(startfloor))
		{
			distance = position.y - sbs->GetFloor(startfloor)->Altitude;
			hit = GetMeshObject(startfloor)->HitBeam(position, Ogre::Vector3::NEGATIVE_UNIT_Y, distance) >= 0;
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
		ReportError("AddDoor: Floor " + ToString2(floor) + " out of range");
		return 0;
	}

	Floor *floorptr = sbs->GetFloor(floor);
	if (!floorptr)
		return 0;

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
	sbs->ResetDoorwayWalls();
	if (direction < 5)
	{
		Cut(1, floor, Ogre::Vector3(x1 - 0.5f, voffset, z1), Ogre::Vector3(x2 + 0.5f, voffset + height, z2), true, false, 1);
		floorptr->Cut(Ogre::Vector3(origin.x + x1 - 0.5f, floorptr->GetBase(true) + voffset, origin.z + z1), Ogre::Vector3(origin.x + x2 + 0.5f, floorptr->GetBase(true) + voffset + height, origin.z + z2), true, false, true, 2);
	}
	else
	{
		Cut(1, floor, Ogre::Vector3(x1, voffset, z1 - 0.5f), Ogre::Vector3(x2, voffset + height, z2 + 0.5f), true, false, 1);
		floorptr->Cut(Ogre::Vector3(origin.x + x1, floorptr->GetBase(true) + voffset, origin.z + z1 - 0.5f), Ogre::Vector3(origin.x + x2, floorptr->GetBase(true) + voffset + height, origin.z + z2 + 0.5f), true, false, true, 2);
	}

	//create doorway walls
	WallObject *wall = GetMeshObject(floor)->CreateWallObject(object, "Connection Walls");
	sbs->AddDoorwayWalls(wall, "ConnectionWall", 0, 0);

	DoorArray.resize(DoorArray.size() + 1);
	DoorArray[DoorArray.size() - 1].floornumber = floor;
	std::string stairsnum = ToString(StairsNum);
	std::string num = ToString((int)DoorArray.size() - 1);
	DoorArray[DoorArray.size() - 1].object = new Door(object, std::string("Stairwell " + stairsnum + ":Door " + num).c_str(), open_sound, close_sound, open_state, texture, thickness, direction, speed, origin.x + CenterX, origin.z + CenterZ, width, height, floorptr->Altitude + floorptr->GetBase(true) + voffset, tw, th);
	floorptr = 0;
	return DoorArray[DoorArray.size() - 1].object->object;
}

void Stairs::CutFloors(bool relative, const Ogre::Vector2 &start, const Ogre::Vector2 &end, float startvoffset, float endvoffset)
{
	//Cut through floor/ceiling polygons on all associated levels, within the voffsets

	Report("cutting...");

	float voffset1, voffset2;
	cutstart = start;
	cutend = end;

	if (!sbs->GetFloor(startfloor) || !sbs->GetFloor(endfloor))
		return;

	for (int i = startfloor; i <= endfloor; i++)
	{
		Floor *floorptr = sbs->GetFloor(i);
		if (!floorptr)
			continue;

		voffset1 = 0;
		voffset2 = sbs->GetFloor(i)->FullHeight() + 1;

		if (i == startfloor)
			voffset1 = startvoffset;
		else if (i == endfloor)
			voffset2 = endvoffset;

		if (relative == true)
			floorptr->Cut(Ogre::Vector3(origin.x + start.x, voffset1, origin.z + start.y), Ogre::Vector3(origin.x + end.x, voffset2, origin.z + end.y), false, true, false);
		else
			floorptr->Cut(Ogre::Vector3(start.x, voffset1, start.y), Ogre::Vector3(end.x, voffset2, end.y), false, true, false);
		floorptr = 0;
	}

	//cut external
	voffset1 = sbs->GetFloor(startfloor)->Altitude + startvoffset;
	voffset2 = sbs->GetFloor(endfloor)->Altitude + endvoffset;
	for (int i = 0; i < (int)sbs->External->Walls.size(); i++)
	{
		if (relative == true)
			sbs->Cut(sbs->External->Walls[i], Ogre::Vector3(origin.x + start.x, voffset1, origin.z + start.y), Ogre::Vector3(origin.x + end.x, voffset2, origin.z + end.y), false, true, Ogre::Vector3(0, 0, 0), Ogre::Vector3(0, 0, 0));
		else
			sbs->Cut(sbs->External->Walls[i], Ogre::Vector3(start.x, voffset1, start.y), Ogre::Vector3(end.x, voffset2, end.y), false, true, Ogre::Vector3(0, 0, 0), Ogre::Vector3(0, 0, 0));
	}
}

bool Stairs::Cut(bool relative, int floor, const Ogre::Vector3 &start, const Ogre::Vector3 &end, bool cutwalls, bool cutfloors, int checkwallnumber)
{
	//Cut through a wall segment
	//the Y values in start and end are both relative to the floor's altitude + interfloor

	//exit with an error if floor is invalid
	if (IsValidFloor(floor) == false)
	{
		if (sbs->Verbose)
			ReportError("Cut: Floor " + ToString2(floor) + " out of range");
		else
			sbs->LastError = "Cut: Floor " + ToString2(floor) + " out of range";
		return false;
	}

	if (!sbs->GetFloor(floor))
		return false;

	float base = sbs->GetFloor(floor)->GetBase();

	for (int i = 0; i < (int)GetMeshObject(floor)->Walls.size(); i++)
	{
		bool reset = true;
		if (i > 0)
			reset = false;

		if (relative == true)
			sbs->Cut(GetMeshObject(floor)->Walls[i], Ogre::Vector3(origin.x + start.x, base + start.y, origin.z + start.z), Ogre::Vector3(origin.x + end.x, base + end.y, origin.z + end.z), cutwalls, cutfloors, Ogre::Vector3(0, 0, 0), origin, checkwallnumber, reset);
		else
			sbs->Cut(GetMeshObject(floor)->Walls[i], Ogre::Vector3(start.x, base + start.y, start.z), Ogre::Vector3(end.x, base + end.y, end.z), cutwalls, cutfloors, Ogre::Vector3(0, 0, 0), origin, checkwallnumber, reset);
	}

	return true;
}

void Stairs::EnableRange(int floor, int range, bool value)
{
	//turn on a range of floors
	//if range is 3, show stairwell on current floor (floor), and 1 floor below and above (3 total floors)
	//if range is 1, show only the current floor (floor)

	if (!sbs->GetFloor(floor))
		return;

	SBS_PROFILE("Stairs::EnableRange");

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
	if (value == true)
	{
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
	}

	//enable floors within range
	for (int i = floor - additionalfloors; i <= floor + additionalfloors; i++)
	{
		if (i >= startfloor && i <= endfloor)
			Enabled(i, value);
	}
}

void Stairs::EnableDoor(int floor, bool value)
{
	//turn on door(s) on the specified floor

	for (int i = 0; i < (int)DoorArray.size(); i++)
	{
		if (DoorArray[i].object)
		{
			if (DoorArray[i].floornumber == floor && DoorArray[i].object)
				DoorArray[i].object->Enabled(value);
		}
	}
}

Door* Stairs::GetDoor(int number)
{
	//get door object
	if (number < (int)DoorArray.size())
	{
		if (DoorArray[number].object)
			return DoorArray[number].object;
	}

	return 0;
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

void Stairs::Report(std::string message)
{
	//general reporting function
	sbs->Report("Stairwell " + ToString2(StairsNum) + ": " + message);
}

bool Stairs::ReportError(std::string message)
{
	//general reporting function
	return sbs->ReportError("Stairwell " + ToString2(StairsNum) + ": " + message);
}

void Stairs::RemoveDoor(Door *door)
{
	//remove a door from the array (this does not delete the object)
	for (int i = 0; i < (int)DoorArray.size(); i++)
	{
		if (DoorArray[i].object == door)
		{
			DoorArray.erase(DoorArray.begin() + i);
			return;
		}
	}
}

void Stairs::RemoveLight(Light *light)
{
	//remove a light reference (does not delete the object itself)
	for (int i = 0; i < (int)lights.size(); i++)
	{
		for (int j = 0; j < (int)lights[i].size(); j++)
		{
			if (lights[i][j] == light)
			{
				lights[i].erase(lights[i].begin() + i);
				return;
			}
		}
	}
}

void Stairs::RemoveModel(Model *model)
{
	//remove a model reference (does not delete the object itself)
	for (int i = 0; i < (int)ModelArray.size(); i++)
	{
		for (int j = 0; j < (int)ModelArray[i].size(); j++)
		{
			if (ModelArray[i][j] == model)
			{
				ModelArray[i].erase(ModelArray[i].begin() + i);
				return;
			}
		}
	}
}

void Stairs::RemoveControl(Control *control)
{
	//remove a control reference (does not delete the object itself)
	for (int i = 0; i < (int)ControlArray.size(); i++)
	{
		for (int j = 0; j < (int)ControlArray[i].size(); j++)
		{
			if (ControlArray[i][j] == control)
			{
				ControlArray[i].erase(ControlArray[i].begin() + i);
				return;
			}
		}
	}
}

void Stairs::RemoveTrigger(Trigger *trigger)
{
	//remove a trigger reference (does not delete the object itself)
	/*for (int i = 0; i < (int)TriggerArray.size(); i++)
	{
		for (int j = 0; j < (int)TriggerArray[i].size(); j++)
		{
			if (TriggerArray[i][j] == trigger)
			{
				TriggerArray[i].erase(TriggerArray[i].begin() + i);
				return;
			}
		}
	}*/
}

Object* Stairs::AddLight(int floor, const char *name, int type, Ogre::Vector3 position, Ogre::Vector3 direction, float color_r, float color_g, float color_b, float spec_color_r, float spec_color_g, float spec_color_b, float spot_inner_angle, float spot_outer_angle, float spot_falloff, float att_range, float att_constant, float att_linear, float att_quadratic)
{
	//add a global light

	//exit if floor is invalid
	if (!IsValidFloor(floor))
		return 0;

	Light* light = new Light(object, name, type, position + Ogre::Vector3(origin.x, sbs->GetFloor(floor)->Altitude, origin.z), direction, color_r, color_g, color_b, spec_color_r, spec_color_g, spec_color_b, spot_inner_angle, spot_outer_angle, spot_falloff, att_range, att_constant, att_linear, att_quadratic);
	lights[floor - startfloor].push_back(light);
	return light->object;
}

MeshObject* Stairs::GetMeshObject(int floor)
{
	//returns the mesh object for the specified floor

	if (!IsValidFloor(floor))
		return 0;

	return StairArray[floor - startfloor];
}

Object* Stairs::AddModel(int floor, const char *name, const char *filename, bool center, Ogre::Vector3 position, Ogre::Vector3 rotation, float max_render_distance, float scale_multiplier, bool enable_physics, float restitution, float friction, float mass)
{
	//add a model

	//exit if floor is invalid
	if (!IsValidFloor(floor))
		return 0;

	Model* model = new Model(object, name, filename, center, position + Ogre::Vector3(origin.x, sbs->GetFloor(floor)->Altitude, origin.z), rotation, max_render_distance, scale_multiplier, enable_physics, restitution, friction, mass);
	if (model->load_error == true)
	{
		delete model;
		return 0;
	}
	ModelArray[floor - startfloor].push_back(model);
	return model->object;
}

Object* Stairs::AddControl(int floor, const char *name, const char *sound, const char *direction, float CenterX, float CenterZ, float width, float height, float voffset, std::vector<std::string> &action_names, std::vector<std::string> &textures)
{
	//add a control
	std::vector<Action*> actionnull; //not used
	Control* control = new Control(object, name, sound, action_names, actionnull, textures, direction, width, height, voffset, true);
	control->SetPosition(Ogre::Vector3(CenterX + origin.x, sbs->GetFloor(floor)->Altitude, CenterZ + origin.z));
	ControlArray[floor - startfloor].push_back(control);
	return control->object;
}

Object* Stairs::AddTrigger(int floor, const char *name, const char *sound_file, Ogre::Vector3 &area_min, Ogre::Vector3 &area_max, std::vector<std::string> &action_names)
{
	//triggers are disabled for now

	//add a trigger
	/*Trigger* trigger = new Trigger(object, name, sound_file, area_min, area_max, action_names);
	TriggerArray[floor - startfloor].push_back(trigger);
	trigger->SetPosition(Ogre::Vector3(origin.x, sbs->GetFloor(floor)->Altitude, origin.z));
	return trigger->object;*/
	return 0;
}

void Stairs::ReplaceTexture(const std::string &oldtexture, const std::string &newtexture)
{
	for (int i = startfloor; i <= endfloor; i++)
		GetMeshObject(i)->ReplaceTexture(oldtexture, newtexture);
}

void Stairs::Init()
{
	//startup initialization of stairs
	EnableWholeStairwell(false);
}

void Stairs::AddShowFloor(int floor)
{
	//adds a floor number to the ShowFloors array

	int index = -1;
	for (int i = 0; i < (int)ShowFloorsList.size(); i++)
	{
		if (ShowFloorsList[i] == floor)
			index = i;
	}
	if (index == -1)
	{
		ShowFloorsList.push_back(floor);
		std::sort(ShowFloorsList.begin(), ShowFloorsList.end());
	}
}

void Stairs::RemoveShowFloor(int floor)
{
	//removes a floor number from the ShowFloors array

	int index = -1;
	for (int i = 0; i < (int)ShowFloorsList.size(); i++)
	{
		if (ShowFloorsList[i] == floor)
			index = i;
	}
	if (index != -1)
	{
		for (int i = 0; i < (int)ShowFloorsList.size(); i++)
		{
			if (ShowFloorsList[i] == floor)
				ShowFloorsList.erase(ShowFloorsList.begin() + i);
		}
	}
}

bool Stairs::IsShowFloor(int floor)
{
	//return true if a floor is in the ShowFloors list

	for (int i = 0; i < (int)ShowFloorsList.size(); i++)
	{
		if (ShowFloorsList[i] == floor)
			return true;
	}
	return false;
}

void Stairs::Check(Ogre::Vector3 position, int current_floor, int previous_floor)
{
	//check to see if user (camera) is in the stairwell

	if (IsInStairwell(position) == true)
	{
		if (InsideStairwell == false)
		{
			InsideStairwell = true;
			sbs->InStairwell = true;

			//turn on entire stairwell if ShowFullStairs is true
			if (ShowFullStairs == true)
				EnableWholeStairwell(true);
		}

		//show specified stairwell range while in the stairwell
		if (ShowFullStairs == false)
			EnableRange(current_floor, sbs->StairsDisplayRange, true);

		//if user walked to a different floor, enable new floor and disable previous
		if (current_floor != previous_floor)
		{
			if (sbs->GetFloor(current_floor)->IsInGroup(previous_floor) == false)
			{
				//only disable other floor if it's not in a group list
				sbs->GetFloor(previous_floor)->Enabled(false);
				sbs->GetFloor(previous_floor)->EnableGroup(false);
			}
			sbs->GetFloor(current_floor)->Enabled(true);
			sbs->GetFloor(current_floor)->EnableGroup(true);

			//turn on related floors if ShowFloors is true
			if (ShowFloors == true)
			{
				for (int i = 0; i < (int)ShowFloorsList.size(); i++)
				{
					Floor *floor = sbs->GetFloor(ShowFloorsList[i]);
					if (floor->IsEnabled == false)
					{
						floor->Enabled(true);
						//floor->EnableGroup(true);
					}
				}
			}
		}
	}
	else if (InsideStairwell == true)
	{
		InsideStairwell = false;
		sbs->InStairwell = false;

		//turn off stairwell if ShowFullStairs is true
		if (ShowFullStairs == true)
			EnableWholeStairwell(false);

		//turn off related floors if outside stairwell
		if (ShowFloors == true)
		{
			for (int i = 0; i < (int)ShowFloorsList.size(); i++)
			{
				if (ShowFloorsList[i] != current_floor)
				{
					Floor *floor = sbs->GetFloor(ShowFloorsList[i]);
					if (floor->IsEnabled == true && floor->IsInGroup(current_floor) == false)
					{
						floor->Enabled(false);
						//floor->EnableGroup(false);
					}
				}
			}
		}
	}
	else if (InsideStairwell == false)
	{
		//show specified stairwell range if outside the stairwell
		EnableRange(current_floor, sbs->StairsOutsideDisplayRange, true);
	}
}
