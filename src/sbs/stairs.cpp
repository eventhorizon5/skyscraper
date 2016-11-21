/* $Id$ */

/*
	Scalable Building Simulator - Stairwell Object
	The Skyscraper Project - Version 1.10 Alpha
	Copyright (C)2004-2016 Ryan Thoryk
	http://www.skyscrapersim.com
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@skyscrapersim.com

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
#include "floor.h"
#include "dynamicmesh.h"
#include "mesh.h"
#include "control.h"
#include "sound.h"
#include "door.h"
#include "model.h"
#include "texture.h"
#include "light.h"
#include "profiler.h"
#include "stairs.h"

namespace SBS {

Stairs::Stairs(Object *parent, int number, float CenterX, float CenterZ, int startfloor, int endfloor) : Object(parent)
{
	//creates a stairwell in the location specified by CenterX and CenterZ
	//and that spans the floor range specified by startfloor and endfloor

	//set up SBS object
	SetValues("Stairs", "", false);

	StairsNum = number;
	this->startfloor = startfloor;
	this->endfloor = endfloor;
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
	ShowFullStairs = 0;

	std::string name;
	name = "Stairwell " + ToString(number);
	SetName(name);
	SetPosition(CenterX, sbs->GetFloor(startfloor)->GetBase(), CenterZ);

	dynamic_mesh = new DynamicMesh(this, GetSceneNode(), name);

	StairArray.resize(endfloor - startfloor + 1);
	EnableArray.resize(endfloor - startfloor + 1);
	DoorArray.resize(endfloor - startfloor + 1);
	ModelArray.resize(endfloor - startfloor + 1);
	ControlArray.resize(endfloor - startfloor + 1);
	//TriggerArray.resize(endfloor - startfloor + 1);

	for (int i = startfloor; i <= endfloor; i++)
	{
		//Create stairwell meshes
		StairArray[i - startfloor] = new MeshObject(this, name + ":" + ToString(i), dynamic_mesh);
		StairArray[i - startfloor]->SetPositionY(sbs->GetFloor(i)->GetBase());
		EnableArray[i - startfloor] = true;
	}

	//create a dynamic mesh for doors
	DoorWrapper = new DynamicMesh(this, GetSceneNode(), GetName() + " Door Container", 0, true);

	EnableLoop(true);
}

Stairs::~Stairs()
{
	//delete controls
	for (size_t i = 0; i < ControlArray.size(); i++)
	{
		for (size_t j = 0; j < ControlArray[i].size(); j++)
		{
			if (ControlArray[i][j])
			{
				ControlArray[i][j]->parent_deleting = true;
				delete ControlArray[i][j];
			}
			ControlArray[i][j] = 0;
		}
	}

	//delete triggers
	/*for (size_t i = 0; i < TriggerArray.size(); i++)
	{
		for (size_t j = 0; j < TriggerArray[i].size(); j++)
		{
			if (TriggerArray[i][j])
			{
				TriggerArray[i][j]->parent_deleting = true;
				delete TriggerArray[i][j];
			}
			TriggerArray[i][j] = 0;
		}
	}*/

	//delete models
	for (size_t i = 0; i < ModelArray.size(); i++)
	{
		for (size_t j = 0; j < ModelArray[i].size(); j++)
		{
			if (ModelArray[i][j])
			{
				ModelArray[i][j]->parent_deleting = true;
				delete ModelArray[i][j];
			}
			ModelArray[i][j] = 0;
		}
	}

	//delete lights
	for (size_t i = 0; i < lights.size(); i++)
	{
		for (size_t j = 0; j < lights[i].size(); j++)
		{
			if (lights[i][j])
			{
				lights[i][j]->parent_deleting = true;
				delete lights[i][j];
			}
			lights[i][j] = 0;
		}
	}

	//delete doors
	for (size_t i = 0; i < DoorArray.size(); i++)
	{
		for (size_t j = 0; j < DoorArray[i].size(); j++)
		{
			if (DoorArray[i][j])
			{
				DoorArray[i][j]->parent_deleting = true;
				delete DoorArray[i][j];
			}
			DoorArray[i][j] = 0;
		}
	}

	if (DoorWrapper)
		delete DoorWrapper;
	DoorWrapper = 0;

	//delete mesh array objects
	for (size_t i = 0; i < StairArray.size(); i++)
	{
		if (StairArray[i])
		{
			StairArray[i]->parent_deleting = true;
			delete StairArray[i];
		}
		StairArray[i] = 0;
	}

	//delete dynamic mesh
	if (dynamic_mesh)
		delete dynamic_mesh;
	dynamic_mesh = 0;

	//unregister from parent
	if (sbs->FastDelete == false && parent_deleting == false)
		sbs->RemoveStairs(this);
}

Wall* Stairs::AddStairs(int floor, const std::string &name, const std::string &texture, const std::string &direction, float CenterX, float CenterZ, float width, float risersize, float treadsize, int num_stairs, float voffset, float tw, float th)
{
	//num_stairs is subtracted by 1 since it includes the floor platform above, but not below
	//direction is where the stair base is - front, back, left, or right.

	//exit with an error if floor is invalid
	if (IsValidFloor(floor) == false)
	{
		ReportError("AddStairs: Floor " + ToString(floor) + " out of range");
		return 0;
	}

	//create wall object
	Wall *wall = GetMeshObject(floor)->CreateWallObject(name);

	std::string Name = name;
	TrimString(Name);
	std::string Direction = direction;
	SetCase(Direction, false);

	sbs->GetTextureManager()->ResetTextureMapping(true);
	if (Direction == "right" || Direction == "back")
		sbs->SetWallOrientation("right");
	if (Direction == "left" || Direction == "front")
		sbs->SetWallOrientation("left");

	for (int i = 1; i <= num_stairs; i++)
	{
		float pos = 0;
		std::string base = Name + " " + ToString(i);
		std::string buffer;

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
			buffer = base + "-riser";
			if (i != num_stairs)
				sbs->DrawWalls(true, true, true, true, false, true);
			else
				sbs->DrawWalls(true, true, false, false, false, false);
			AddWall(wall, floor, buffer, texture, thickness, pos + treadsize, -(width / 2) + CenterZ, pos + treadsize, (width / 2) + CenterZ, risersize, risersize, voffset + (risersize * (i - 1)), voffset + (risersize * (i - 1)), tw, th);
			buffer = base + "-tread";
			if (i != num_stairs)
			{
				sbs->DrawWalls(false, true, false, false, false, false);
				AddFloor(wall, floor, buffer, texture, 0, pos, -(width / 2) + CenterZ, pos + treadsize, (width / 2) + CenterZ, voffset + (risersize * i), voffset + (risersize * i), false, false, tw, th);
			}
		}
		if (Direction == "left")
		{
			pos = CenterX - ((treadsize * (num_stairs - 1)) / 2) + (treadsize * i);
			buffer = base + "-riser";
			if (i != num_stairs)
				sbs->DrawWalls(true, true, true, true, false, true);
			else
				sbs->DrawWalls(true, true, false, false, false, false);
			AddWall(wall, floor, buffer, texture, thickness, pos - treadsize, (width / 2) + CenterZ, pos - treadsize, -(width / 2) + CenterZ, risersize, risersize, voffset + (risersize * (i - 1)), voffset + (risersize * (i - 1)), tw, th);
			buffer = base + "-tread";
			if (i != num_stairs)
			{
				sbs->DrawWalls(false, true, false, false, false, false);
				AddFloor(wall, floor, buffer, texture, 0, pos - treadsize, -(width / 2) + CenterZ, pos, (width / 2) + CenterZ, voffset + (risersize * i), voffset + (risersize * i), false, false, tw, th);
			}
		}
		if (Direction == "back")
		{
			pos = CenterZ + ((treadsize * (num_stairs - 1)) / 2) - (treadsize * i);
			buffer = base + "-riser";
			if (i != num_stairs)
				sbs->DrawWalls(true, true, true, true, false, true);
			else
				sbs->DrawWalls(true, true, false, false, false, false);
			AddWall(wall, floor, buffer, texture, thickness, (width / 2) + CenterX, pos + treadsize, -(width / 2) + CenterX, pos + treadsize, risersize, risersize, voffset + (risersize * (i - 1)), voffset + (risersize * (i - 1)), tw, th);
			buffer = base + "-tread";
			if (i != num_stairs)
			{
				sbs->DrawWalls(false, true, false, false, false, false);
				AddFloor(wall, floor, buffer, texture, 0, -(width / 2) + CenterX, pos, (width / 2) + CenterX, pos + treadsize, voffset + (risersize * i), voffset + (risersize * i), false, false, tw, th);
			}
		}
		if (Direction == "front")
		{
			pos = CenterZ - ((treadsize * (num_stairs - 1)) / 2) + (treadsize * i);
			buffer = base + "-riser";
			if (i != num_stairs)
				sbs->DrawWalls(true, true, true, true, false, true);
			else
				sbs->DrawWalls(true, true, false, false, false, false);
			AddWall(wall, floor, buffer, texture, thickness, -(width / 2) + CenterX, pos - treadsize, (width / 2) + CenterX, pos - treadsize, risersize, risersize, voffset + (risersize * (i - 1)), voffset + (risersize * (i - 1)), tw, th);
			buffer = base + "-tread";
			if (i != num_stairs)
			{
				sbs->DrawWalls(false, true, false, false, false, false);
				AddFloor(wall, floor, buffer, texture, 0, -(width / 2) + CenterX, pos - treadsize, (width / 2) + CenterX, pos, voffset + (risersize * i), voffset + (risersize * i), false, false, tw, th);
			}
		}
	}
	sbs->ResetWalls(true);
	sbs->GetTextureManager()->ResetTextureMapping();

	return wall;
}

Wall* Stairs::AddWall(int floor, const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th)
{
	//exit with an error if floor is invalid
	if (IsValidFloor(floor) == false)
	{
		ReportError("AddWall: Floor " + ToString(floor) + " out of range");
		return 0;
	}

	Wall *wall = GetMeshObject(floor)->CreateWallObject(name);
	AddWall(wall, floor, name, texture, thickness, x1, z1, x2, z2, height1, height2, voffset1, voffset2, tw, th);
	return wall;
}

bool Stairs::AddWall(Wall *wall, int floor, const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th)
{
	//exit with an error if floor is invalid
	if (IsValidFloor(floor) == false)
		return ReportError("AddWall: Floor " + ToString(floor) + " out of range");

	return sbs->AddWallMain(wall, name, texture, thickness, x1, z1, x2, z2, height1, height2, voffset1, voffset2, tw, th, true);
}

Wall* Stairs::AddFloor(int floor, const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, bool reverse_axis, bool texture_direction, float tw, float th, bool legacy_behavior)
{
	//exit with an error if floor is invalid
	if (IsValidFloor(floor) == false)
	{
		ReportError("AddFloor: Floor " + ToString(floor) + " out of range");
		return 0;
	}

	Wall *wall = GetMeshObject(floor)->CreateWallObject(name);
	AddFloor(wall, floor, name, texture, thickness, x1, z1, x2, z2, voffset1, voffset2, reverse_axis, texture_direction, tw, th, legacy_behavior);
	return wall;
}

bool Stairs::AddFloor(Wall *wall, int floor, const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, bool reverse_axis, bool texture_direction, float tw, float th, bool legacy_behavior)
{
	//exit with an error if floor is invalid
	if (IsValidFloor(floor) == false)
		return ReportError("AddFloor: Floor " + ToString(floor) + " out of range");

	return sbs->AddFloorMain(wall, name, texture, thickness, x1, z1, x2, z2, voffset1, voffset2, reverse_axis, texture_direction, tw, th, true, legacy_behavior);
}

void Stairs::Enabled(int floor, bool value)
{
	//turns stairwell on/off for a specific floor

	SBS_PROFILE("Stairs::Enabled");
	if (IsEnabledFloor(floor) != value && floor >= startfloor && floor <= endfloor)
	{
		GetMeshObject(floor)->Enable(value);
		EnableArray[floor - startfloor] = value;

		//doors
		for (size_t i = 0; i < DoorArray[floor - startfloor].size(); i++)
		{
			if (DoorArray[floor - startfloor][i])
				DoorArray[floor - startfloor][i]->Enabled(value);
		}

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
	}
}

void Stairs::EnableWholeStairwell(bool value, bool force)
{
	//turn on/off entire stairwell

	if (value == false && ShowFullStairs == 2)
		return;

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

	//enable/disable dynamic meshes
	dynamic_mesh->Enable(value);
	DoorWrapper->Enable(value);

	IsEnabled = value;
}

bool Stairs::IsInStairwell(const Ogre::Vector3 &position)
{
	//determine if user is in the stairwell

	//SBS_PROFILE("Stairs::IsInStairwell");

	//if last position is the same as new, return previous result
	if (position.positionEquals(lastposition) == true && checkfirstrun == false)
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
		if (IsValidFloor(floor))
			hit = GetMeshObject(floor)->HitBeam(position, Ogre::Vector3::NEGATIVE_UNIT_Y, distance) >= 0;

		//if no hit, check hit against lower floor
		if (hit == false && sbs->GetFloor(floor - 1) && floor > startfloor)
		{
			distance = position.y - sbs->GetFloor(floor - 1)->Altitude;
			if (IsValidFloor(floor - 1))
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

Door* Stairs::AddDoor(int floor, const std::string &open_sound, const std::string &close_sound, bool open_state, const std::string &texture, float thickness, int direction, float speed, float CenterX, float CenterZ, float width, float height, float voffset, float tw, float th)
{
	//add a door

	//exit with an error if floor is invalid
	if (IsValidFloor(floor) == false)
	{
		ReportError("AddDoor: Floor " + ToString(floor) + " out of range");
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
		floorptr->Cut(Ogre::Vector3(GetPosition().x + x1 - 0.5f, floorptr->GetBase(true) + voffset, GetPosition().z + z1), Ogre::Vector3(GetPosition().x + x2 + 0.5f, floorptr->GetBase(true) + voffset + height, GetPosition().z + z2), true, false, true, 2);
	}
	else
	{
		Cut(1, floor, Ogre::Vector3(x1, voffset, z1 - 0.5f), Ogre::Vector3(x2, voffset + height, z2 + 0.5f), true, false, 1);
		floorptr->Cut(Ogre::Vector3(GetPosition().x + x1, floorptr->GetBase(true) + voffset, GetPosition().z + z1 - 0.5f), Ogre::Vector3(GetPosition().x + x2, floorptr->GetBase(true) + voffset + height, GetPosition().z + z2 + 0.5f), true, false, true, 2);
	}

	//create doorway walls
	sbs->AddDoorwayWalls(GetMeshObject(floor), "Connection Walls", "ConnectionWall", 0, 0);

	int index = floor - startfloor;
	std::string num = ToString((int)DoorArray[index].size());
	std::string name = "Stairwell " + ToString(StairsNum) + ":Door " + ToString(floor) + ":" + num;

	Door* door = new Door(GetMeshObject(floor), DoorWrapper, name, open_sound, close_sound, open_state, texture, thickness, direction, speed, CenterX, CenterZ, width, height, voffset, tw, th);
	DoorArray[index].push_back(door);

	floorptr = 0;
	return door;
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
			floorptr->Cut(Ogre::Vector3(GetPosition().x + start.x, voffset1, GetPosition().z + start.y), Ogre::Vector3(GetPosition().x + end.x, voffset2, GetPosition().z + end.y), false, true, false);
		else
			floorptr->Cut(Ogre::Vector3(start.x, voffset1, start.y), Ogre::Vector3(end.x, voffset2, end.y), false, true, false);
		floorptr = 0;
	}

	//cut external
	voffset1 = sbs->GetFloor(startfloor)->Altitude + startvoffset;
	voffset2 = sbs->GetFloor(endfloor)->Altitude + endvoffset;
	for (size_t i = 0; i < sbs->External->Walls.size(); i++)
	{
		if (relative == true)
			sbs->Cut(sbs->External->Walls[i], Ogre::Vector3(GetPosition().x + start.x, voffset1, GetPosition().z + start.y), Ogre::Vector3(GetPosition().x + end.x, voffset2, GetPosition().z + end.y), false, true);
		else
			sbs->Cut(sbs->External->Walls[i], Ogre::Vector3(start.x, voffset1, start.y), Ogre::Vector3(end.x, voffset2, end.y), false, true);
	}
}

bool Stairs::Cut(bool relative, int floor, const Ogre::Vector3 &start, const Ogre::Vector3 &end, bool cutwalls, bool cutfloors, int checkwallnumber)
{
	//Cut through a wall segment
	//the Y values in start and end are both relative to the floor's base

	//exit with an error if floor is invalid
	if (IsValidFloor(floor) == false)
	{
		if (sbs->Verbose)
			ReportError("Cut: Floor " + ToString(floor) + " out of range");
		else
			sbs->LastError = "Cut: Floor " + ToString(floor) + " out of range";
		return false;
	}

	if (!sbs->GetFloor(floor))
		return false;

	for (size_t i = 0; i < GetMeshObject(floor)->Walls.size(); i++)
	{
		bool reset = true;
		if (i > 0)
			reset = false;

		if (relative == true)
			sbs->Cut(GetMeshObject(floor)->Walls[i], Ogre::Vector3(start.x, start.y, start.z), Ogre::Vector3(end.x, end.y, end.z), cutwalls, cutfloors, checkwallnumber, reset);
		else
			sbs->Cut(GetMeshObject(floor)->Walls[i], Ogre::Vector3(start.x - GetPosition().x, start.y, start.z - GetPosition().z), Ogre::Vector3(end.x - GetPosition().x, end.y, end.z - GetPosition().z), cutwalls, cutfloors, checkwallnumber, reset);
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

void Stairs::Report(const std::string &message)
{
	//general reporting function
	sbs->Report("Stairwell " + ToString(StairsNum) + ": " + message);
}

bool Stairs::ReportError(const std::string &message)
{
	//general reporting function
	return sbs->ReportError("Stairwell " + ToString(StairsNum) + ": " + message);
}

void Stairs::RemoveDoor(Door *door)
{
	//remove a door reference (this does not delete the object)
	for (size_t i = 0; i < DoorArray.size(); i++)
	{
		for (size_t j = 0; j < DoorArray[i].size(); j++)
		{
			if (DoorArray[i][j] == door)
			{
				DoorArray[i].erase(DoorArray[i].begin() + j);
				return;
			}
		}
	}
}

void Stairs::RemoveLight(Light *light)
{
	//remove a light reference (does not delete the object itself)
	for (size_t i = 0; i < lights.size(); i++)
	{
		for (size_t j = 0; j < lights[i].size(); j++)
		{
			if (lights[i][j] == light)
			{
				lights[i].erase(lights[i].begin() + j);
				return;
			}
		}
	}
}

void Stairs::RemoveModel(Model *model)
{
	//remove a model reference (does not delete the object itself)
	for (size_t i = 0; i < ModelArray.size(); i++)
	{
		for (size_t j = 0; j < ModelArray[i].size(); j++)
		{
			if (ModelArray[i][j] == model)
			{
				ModelArray[i].erase(ModelArray[i].begin() + j);
				return;
			}
		}
	}
}

void Stairs::RemoveControl(Control *control)
{
	//remove a control reference (does not delete the object itself)
	for (size_t i = 0; i < ControlArray.size(); i++)
	{
		for (size_t j = 0; j < ControlArray[i].size(); j++)
		{
			if (ControlArray[i][j] == control)
			{
				ControlArray[i].erase(ControlArray[i].begin() + j);
				return;
			}
		}
	}
}

void Stairs::RemoveTrigger(Trigger *trigger)
{
	//remove a trigger reference (does not delete the object itself)
	/*for (size_t i = 0; i < TriggerArray.size(); i++)
	{
		for (size_t j = 0; j < TriggerArray[i].size(); j++)
		{
			if (TriggerArray[i][j] == trigger)
			{
				TriggerArray[i].erase(TriggerArray[i].begin() + j);
				return;
			}
		}
	}*/
}

Light* Stairs::AddLight(int floor, const std::string &name, int type, Ogre::Vector3 position, Ogre::Vector3 direction, float color_r, float color_g, float color_b, float spec_color_r, float spec_color_g, float spec_color_b, float spot_inner_angle, float spot_outer_angle, float spot_falloff, float att_range, float att_constant, float att_linear, float att_quadratic)
{
	//add a global light

	//exit if floor is invalid
	if (!IsValidFloor(floor))
		return 0;

	Light* light = new Light(GetMeshObject(floor), name, type, position, direction, color_r, color_g, color_b, spec_color_r, spec_color_g, spec_color_b, spot_inner_angle, spot_outer_angle, spot_falloff, att_range, att_constant, att_linear, att_quadratic);
	lights[floor - startfloor].push_back(light);
	return light;
}

MeshObject* Stairs::GetMeshObject(int floor)
{
	//returns the mesh object for the specified floor

	if (!IsValidFloor(floor))
		return 0;

	return StairArray[floor - startfloor];
}

Model* Stairs::AddModel(int floor, const std::string &name, const std::string &filename, bool center, Ogre::Vector3 position, Ogre::Vector3 rotation, float max_render_distance, float scale_multiplier, bool enable_physics, float restitution, float friction, float mass)
{
	//add a model

	//exit if floor is invalid
	if (!IsValidFloor(floor))
		return 0;

	Model* model = new Model(GetMeshObject(floor), name, filename, center, position, rotation, max_render_distance, scale_multiplier, enable_physics, restitution, friction, mass);
	if (model->load_error == true)
	{
		delete model;
		return 0;
	}
	ModelArray[floor - startfloor].push_back(model);
	return model;
}

void Stairs::AddModel(int floor, Model *model)
{
	//add a model reference

	if (!model)
		return;

	//exit if floor is invalid
	if (!IsValidFloor(floor))
		return;

	for (size_t i = 0; i < ModelArray[floor - startfloor].size(); i++)
	{
		if (ModelArray[floor - startfloor][i] == model)
			return;
	}

	ModelArray[floor - startfloor].push_back(model);
}

Control* Stairs::AddControl(int floor, const std::string &name, const std::string &sound, const std::string &direction, float CenterX, float CenterZ, float width, float height, float voffset, int selection_position, std::vector<std::string> &action_names, std::vector<std::string> &textures)
{
	//add a control

	//exit if floor is invalid
	if (!IsValidFloor(floor))
		return 0;

	std::vector<Action*> actionnull; //not used
	Control* control = new Control(GetMeshObject(floor), name, false, sound, action_names, actionnull, textures, direction, width, height, true, selection_position);
	control->Move(CenterX, voffset, CenterZ);
	ControlArray[floor - startfloor].push_back(control);
	return control;
}

Trigger* Stairs::AddTrigger(int floor, const std::string &name, const std::string &sound_file, Ogre::Vector3 &area_min, Ogre::Vector3 &area_max, std::vector<std::string> &action_names)
{
	//triggers are disabled for now

	//add a trigger

	//exit if floor is invalid
	/*if (!IsValidFloor(floor))
		return 0;

	Trigger* trigger = new Trigger(GetMeshObject(floor), name, false, sound_file, area_min, area_max, action_names);
	TriggerArray[floor - startfloor].push_back(trigger);
	return trigger;*/
	return 0;
}

void Stairs::ReplaceTexture(const std::string &oldtexture, const std::string &newtexture)
{
	for (int i = startfloor; i <= endfloor; i++)
		GetMeshObject(i)->ReplaceTexture(oldtexture, newtexture);
}

void Stairs::OnInit()
{
	//startup initialization of stairs

	if (ShowFullStairs == 2)
		EnableWholeStairwell(true);
	else
		EnableWholeStairwell(false);
}

void Stairs::AddShowFloor(int floor)
{
	//adds a floor number to the ShowFloors array

	if (IsShowFloor(floor))
		return;

	ShowFloorsList.push_back(floor);
	std::sort(ShowFloorsList.begin(), ShowFloorsList.end());
}

void Stairs::RemoveShowFloor(int floor)
{
	//removes a floor number from the ShowFloors array

	for (size_t i = 0; i < ShowFloorsList.size(); i++)
	{
		if (ShowFloorsList[i] == floor)
		{
			ShowFloorsList.erase(ShowFloorsList.begin() + i);
			return;
		}
	}
}

bool Stairs::IsShowFloor(int floor)
{
	//return true if a floor is in the ShowFloors list

	for (size_t i = 0; i < ShowFloorsList.size(); i++)
	{
		if (ShowFloorsList[i] == floor)
			return true;
	}
	return false;
}

void Stairs::Check(Ogre::Vector3 position, int current_floor, int previous_floor)
{
	//check to see if user (camera) is in the stairwell

	SBS_PROFILE("Stairs::Check");

	if (IsInStairwell(position) == true)
	{
		if (InsideStairwell == false)
		{
			InsideStairwell = true;
			sbs->InStairwell = true;

			//turn on entire stairwell if ShowFullStairs is not 0
			if (ShowFullStairs > 0)
				EnableWholeStairwell(true);
		}

		//show specified stairwell range while in the stairwell
		if (ShowFullStairs == 0)
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
				for (size_t i = 0; i < ShowFloorsList.size(); i++)
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

		//turn off stairwell if ShowFullStairs is 1
		if (ShowFullStairs == 1)
			EnableWholeStairwell(false);

		//turn off related floors if outside stairwell
		if (ShowFloors == true)
		{
			for (size_t i = 0; i < ShowFloorsList.size(); i++)
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
		if (ShowFullStairs == 2)
		{
			//show full stairwell if specified
			EnableWholeStairwell(true);
		}
		else
		{
			//show specified stairwell range if outside the stairwell
			EnableRange(current_floor, sbs->StairsOutsideDisplayRange, true);
		}
	}
}

void Stairs::Loop()
{
	//stairwell runloop

	SBS_PROFILE("Stairs::Loop");

	LoopChildren();
}

Model* Stairs::GetModel(int floor, std::string name)
{
	//get a model by name

	//exit if floor is invalid
	if (!IsValidFloor(floor))
		return 0;

	SetCase(name, false);

	int index = floor - startfloor;

	for (size_t i = 0; i < ModelArray[index].size(); i++)
	{
		if (SetCaseCopy(ModelArray[index][i]->GetName(), false) == name)
			return ModelArray[index][i];
	}

	return 0;
}

void Stairs::SetShowFull(int value)
{
	ShowFullStairs = value;

	//force the combining of dynamic meshes, since they'll be fully shown
	if (value == 2)
	{
		dynamic_mesh->force_combine = true;
		DoorWrapper->force_combine = true;
	}
}

}
