/*
	Scalable Building Simulator - Stairwell Object
	The Skyscraper Project - Version 1.12 Alpha
	Copyright (C)2004-2024 Ryan Thoryk
	https://www.skyscrapersim.net
	https://sourceforge.net/projects/skyscraper/
	Contact - ryan@skyscrapersim.net

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
#include "polymesh.h"
#include "control.h"
#include "sound.h"
#include "door.h"
#include "model.h"
#include "texture.h"
#include "light.h"
#include "profiler.h"
#include "cameratexture.h"
#include "stairs.h"

namespace SBS {

Stairwell::Stairwell(Object *parent, int number, Real CenterX, Real CenterZ, int startfloor, int endfloor) : Object(parent)
{
	//creates a stairwell in the location specified by CenterX and CenterZ
	//and that spans the floor range specified by startfloor and endfloor

	//set up SBS object
	SetValues("Stairwell", "", false);

	StairsNum = number;
	this->startfloor = startfloor;
	this->endfloor = endfloor;
	cutstart = Vector2::ZERO;
	cutend = Vector2::ZERO;
	Inside = false;
	IsEnabled = true;
	lastfloor = 0;
	lastfloorset = false;
	lastcheckresult = false;
	checkfirstrun = true;
	lastposition = Vector3::ZERO;
	ShowFloors = false;
	ShowFullStairs = 0;

	std::string name;
	name = "Stairwell " + ToString(number);
	SetName(name);
	SetPosition(CenterX, sbs->GetFloor(startfloor)->GetBase(), CenterZ);

	dynamic_mesh = new DynamicMesh(this, GetSceneNode(), name);

	for (int i = startfloor; i <= endfloor; i++)
	{
		Levels.push_back(new Level(this, i));
	}

	//create a dynamic mesh for doors
	DoorWrapper = new DynamicMesh(this, GetSceneNode(), GetName() + " Door Container", 0, true);

	Report("created at " + TruncateNumber(CenterX, 4) + ", " + TruncateNumber(CenterZ, 4));

	EnableLoop(true);
}

Stairwell::~Stairwell()
{
	//delete levels
	for (int i = 0; i < Levels.size(); i++)
	{
		if (Levels[i])
			delete Levels[i];
		Levels[i] = 0;
	}

	if (DoorWrapper)
		delete DoorWrapper;
	DoorWrapper = 0;

	//delete dynamic mesh
	if (dynamic_mesh)
		delete dynamic_mesh;
	dynamic_mesh = 0;

	//unregister from parent
	if (sbs->FastDelete == false && parent_deleting == false)
		sbs->RemoveStairwell(this);
}

Stairwell::Level* Stairwell::GetLevel(int floor)
{
	for (int i = 0; i < Levels.size(); i++)
	{
		if (Levels[i]->GetFloor() == floor)
			return Levels[i];
	}
	return 0;
}

void Stairwell::SetShowFull(int value)
{
	ShowFullStairs = value;

	//force the combining of dynamic meshes, since they'll be fully shown
	if (value == 2)
	{
		dynamic_mesh->force_combine = true;
		DoorWrapper->force_combine = true;
	}
}

void Stairwell::EnableWhole(bool value, bool force)
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
				GetLevel(i)->enabled = !value;
			GetLevel(i)->Enabled(value);
		}
	}

	//enable/disable dynamic meshes
	dynamic_mesh->Enabled(value);
	DoorWrapper->Enabled(value);

	IsEnabled = value;
}

bool Stairwell::IsInside(const Vector3 &position)
{
	//determine if user is in the stairwell

	//SBS_PROFILE("Stairwell::IsInStairwell");

	//if last position is the same as new, return previous result
	if (position.positionEquals(lastposition) == true && checkfirstrun == false)
		return lastcheckresult;

	if (!sbs->GetFloor(startfloor) || !sbs->GetFloor(endfloor))
		return false;

	bool hit = false;
	Real bottom = sbs->GetFloor(startfloor)->GetBase();
	Real top = sbs->GetFloor(endfloor)->Altitude + sbs->GetFloor(endfloor)->FullHeight();

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
		Real distance = floorptr->FullHeight();
		if (GetLevel(floor))
			hit = GetLevel(floor)->GetMeshObject()->GetPolyMesh()->HitBeam(position, Vector3::NEGATIVE_UNIT_Y, distance) >= 0;

		//if no hit, check hit against lower floor
		if (hit == false && sbs->GetFloor(floor - 1) && floor > startfloor)
		{
			distance = position.y - sbs->GetFloor(floor - 1)->Altitude;
			if (GetLevel(floor - 1))
				hit = GetLevel(floor - 1)->GetMeshObject()->GetPolyMesh()->HitBeam(position, Vector3::NEGATIVE_UNIT_Y, distance) >= 0;
		}

		//if no hit, check hit against starting floor
		if (hit == false && sbs->GetFloor(startfloor))
		{
			distance = position.y - sbs->GetFloor(startfloor)->Altitude;
			hit = GetLevel(startfloor)->GetMeshObject()->GetPolyMesh()->HitBeam(position, Vector3::NEGATIVE_UNIT_Y, distance) >= 0;
		}
	}
	floorptr = 0;

	//cache values
	checkfirstrun = false;
	lastcheckresult = hit;
	lastposition = position;

	return hit;
}

void Stairwell::CutFloors(bool relative, const Vector2 &start, const Vector2 &end, Real startvoffset, Real endvoffset)
{
	//Cut through floor/ceiling polygons on all associated levels, within the voffsets

	Report("cutting...");

	Real voffset1, voffset2;
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
			floorptr->Cut(Vector3(GetPosition().x + start.x, voffset1, GetPosition().z + start.y), Vector3(GetPosition().x + end.x, voffset2, GetPosition().z + end.y), false, true, false);
		else
			floorptr->Cut(Vector3(start.x, voffset1, start.y), Vector3(end.x, voffset2, end.y), false, true, false);
		floorptr = 0;
	}

	//cut external
	voffset1 = sbs->GetFloor(startfloor)->Altitude + startvoffset;
	voffset2 = sbs->GetFloor(endfloor)->Altitude + endvoffset;

	if (sbs->External)
	{
		for (size_t i = 0; i < sbs->External->Walls.size(); i++)
		{
			if (relative == true)
				sbs->Cut(sbs->External->Walls[i], Vector3(GetPosition().x + start.x, voffset1, GetPosition().z + start.y), Vector3(GetPosition().x + end.x, voffset2, GetPosition().z + end.y), false, true);
			else
				sbs->Cut(sbs->External->Walls[i], Vector3(start.x, voffset1, start.y), Vector3(end.x, voffset2, end.y), false, true);
		}
	}
}

void Stairwell::EnableRange(int floor, int range, bool value)
{
	//turn on a range of floors
	//if range is 3, show stairwell on current floor (floor), and 1 floor below and above (3 total floors)
	//if range is 1, show only the current floor (floor)

	if (!sbs->GetFloor(floor))
		return;

	SBS_PROFILE("Stairwell::EnableRange");

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
				GetLevel(floor - additionalfloors - 1)->Enabled(false);
		}
		if (floor + additionalfloors + 1 >= startfloor && floor + additionalfloors + 1 <= endfloor)
		{
			if (sbs->GetFloor(floor)->IsInGroup(floor + additionalfloors + 1) == false) //only disable if not in group
				GetLevel(floor + additionalfloors + 1)->Enabled(false);
		}
	}

	//enable floors within range
	for (int i = floor - additionalfloors; i <= floor + additionalfloors; i++)
	{
		if (i >= startfloor && i <= endfloor)
			GetLevel(i)->Enabled(value);
	}
}

bool Stairwell::IsValidFloor(int floor)
{
	//return true if the stairwell services the specified floor

	if (floor < startfloor || floor > endfloor)
		return false;

	if (!GetLevel(floor))
		return false;

	return true;
}

void Stairwell::Report(const std::string &message)
{
	//general reporting function
	Object::Report("Stairwell " + ToString(StairsNum) + ": " + message);
}

bool Stairwell::ReportError(const std::string &message)
{
	//general reporting function
	return Object::ReportError("Stairwell " + ToString(StairsNum) + ": " + message);
}

void Stairwell::ReplaceTexture(const std::string &oldtexture, const std::string &newtexture)
{
	for (int i = 0; i < Levels.size(); i++)
		Levels[i]->ReplaceTexture(oldtexture, newtexture);
}

void Stairwell::OnInit()
{
	//startup initialization of stairs

	if (ShowFullStairs == 2)
		EnableWhole(true);
	else
		EnableWhole(false);
}

void Stairwell::AddShowFloor(int floor)
{
	//adds a floor number to the ShowFloors array

	if (IsShowFloor(floor))
		return;

	ShowFloorsList.push_back(floor);
	std::sort(ShowFloorsList.begin(), ShowFloorsList.end());
}

void Stairwell::RemoveShowFloor(int floor)
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

bool Stairwell::IsShowFloor(int floor)
{
	//return true if a floor is in the ShowFloors list

	for (size_t i = 0; i < ShowFloorsList.size(); i++)
	{
		if (ShowFloorsList[i] == floor)
			return true;
	}
	return false;
}

void Stairwell::Check(Vector3 position, int current_floor, int previous_floor)
{
	//check to see if user (camera) is in the stairwell

	SBS_PROFILE("Stairwell::Check");

	if (IsInside(position) == true)
	{
		if (Inside == false)
		{
			Inside = true;
			sbs->InStairwell = true;

			//turn on entire stairwell if ShowFullStairs is not 0
			if (ShowFullStairs > 0)
				EnableWhole(true);
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
	else if (Inside == true)
	{
		Inside = false;
		sbs->InStairwell = false;

		//turn off stairwell if ShowFullStairs is 1
		if (ShowFullStairs == 1)
			EnableWhole(false);

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
	else if (Inside == false)
	{
		if (ShowFullStairs == 2)
		{
			//show full stairwell if specified
			EnableWhole(true);
		}
		else
		{
			//show specified stairwell range if outside the stairwell
			EnableRange(current_floor, sbs->StairsOutsideDisplayRange, true);
		}
	}
}

void Stairwell::Loop()
{
	//stairwell runloop

	SBS_PROFILE("Stairwell::Loop");

	LoopChildren();
}

DynamicMesh* Stairwell::GetDynamicMesh()
{
	return dynamic_mesh;
}

Stairwell::Level::Level(Stairwell *parent, int number) : Object(parent)
{
	//set up SBS object
	SetValues("Stairwell Level", "", true);

	enabled = true;
	floornum = number;
	this->parent = parent;

	std::string name;
	name = "Stairwell " + ToString(parent->StairsNum) + ": Level " + ToString(number);
	SetName(name);

	//Create level mesh
	mesh = new MeshObject(this, parent->GetName() + ":" + ToString(floornum), parent->GetDynamicMesh());
	SetPositionY(sbs->GetFloor(number)->GetBase());

	EnableLoop(true);
}

Stairwell::Level::~Level()
{
	//delete controls
	for (size_t i = 0; i < ControlArray.size(); i++)
	{
		if (ControlArray[i])
		{
			ControlArray[i]->parent_deleting = true;
			delete ControlArray[i];
		}
		ControlArray[i] = 0;
	}

	//delete triggers
	/*for (size_t i = 0; i < TriggerArray.size(); i++)
	{
		if (TriggerArray[i])
		{
			TriggerArray[i]->parent_deleting = true;
			delete TriggerArray[i];
		}
		TriggerArray[i] = 0;
	}*/

	//delete models
	for (size_t i = 0; i < ModelArray.size(); i++)
	{
		if (ModelArray[i])
		{
			ModelArray[i]->parent_deleting = true;
			delete ModelArray[i];
		}
		ModelArray[i] = 0;
	}

	//delete lights
	for (size_t i = 0; i < lights.size(); i++)
	{
		if (lights[i])
		{
			lights[i]->parent_deleting = true;
			delete lights[i];
		}
		lights[i] = 0;
	}

	//delete doors
	for (size_t i = 0; i < DoorArray.size(); i++)
	{
		if (DoorArray[i])
		{
			DoorArray[i]->parent_deleting = true;
			delete DoorArray[i];
		}
		DoorArray[i] = 0;
	}

	//delete camera textures
	for (size_t i = 0; i < CameraTextureArray.size(); i++)
	{
		if (CameraTextureArray[i])
		{
			CameraTextureArray[i]->parent_deleting = true;
			delete CameraTextureArray[i];
		}
		CameraTextureArray[i] = 0;
	}

	if (mesh)
		delete mesh;
	mesh = 0;
}

Wall* Stairwell::Level::AddStairs(const std::string &name, const std::string &riser_texture, const std::string &tread_texture, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real risersize, Real treadsize, int num_stairs, Real voffset, Real tw, Real th)
{
	//num_stairs is subtracted by 1 since it includes the floor platform above, but not below
	//direction is where the stair base is - front, back, left, or right.

	//exit with an error if floor is invalid
	/*if (IsValid() == false)
	{
		parent->ReportError("AddStairs: Floor " + ToString(floornum) + " out of range");
		return 0;
	}*/

	//create wall object
	Wall *wall = mesh->CreateWallObject(name);

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
		Real pos = 0;
		std::string base = Name + " " + ToString(i);
		std::string buffer;

		Real thickness = 0;
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
			AddWall(wall, buffer, riser_texture, thickness, pos + treadsize, -(width / 2) + CenterZ, pos + treadsize, (width / 2) + CenterZ, risersize, risersize, voffset + (risersize * (i - 1)), voffset + (risersize * (i - 1)), tw, th);
			buffer = base + "-tread";
			if (i != num_stairs)
			{
				sbs->DrawWalls(false, true, false, false, false, false);
				AddFloor(wall, buffer, tread_texture, 0, pos, -(width / 2) + CenterZ, pos + treadsize, (width / 2) + CenterZ, voffset + (risersize * i), voffset + (risersize * i), false, false, tw, th);
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
			AddWall(wall, buffer, riser_texture, thickness, pos - treadsize, (width / 2) + CenterZ, pos - treadsize, -(width / 2) + CenterZ, risersize, risersize, voffset + (risersize * (i - 1)), voffset + (risersize * (i - 1)), tw, th);
			buffer = base + "-tread";
			if (i != num_stairs)
			{
				sbs->DrawWalls(false, true, false, false, false, false);
				AddFloor(wall, buffer, tread_texture, 0, pos - treadsize, -(width / 2) + CenterZ, pos, (width / 2) + CenterZ, voffset + (risersize * i), voffset + (risersize * i), false, false, tw, th);
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
			AddWall(wall, buffer, riser_texture, thickness, (width / 2) + CenterX, pos + treadsize, -(width / 2) + CenterX, pos + treadsize, risersize, risersize, voffset + (risersize * (i - 1)), voffset + (risersize * (i - 1)), tw, th);
			buffer = base + "-tread";
			if (i != num_stairs)
			{
				sbs->DrawWalls(false, true, false, false, false, false);
				AddFloor(wall, buffer, tread_texture, 0, -(width / 2) + CenterX, pos, (width / 2) + CenterX, pos + treadsize, voffset + (risersize * i), voffset + (risersize * i), false, false, tw, th);
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
			AddWall(wall, buffer, riser_texture, thickness, -(width / 2) + CenterX, pos - treadsize, (width / 2) + CenterX, pos - treadsize, risersize, risersize, voffset + (risersize * (i - 1)), voffset + (risersize * (i - 1)), tw, th);
			buffer = base + "-tread";
			if (i != num_stairs)
			{
				sbs->DrawWalls(false, true, false, false, false, false);
				AddFloor(wall, buffer, tread_texture, 0, -(width / 2) + CenterX, pos - treadsize, (width / 2) + CenterX, pos, voffset + (risersize * i), voffset + (risersize * i), false, false, tw, th);
			}
		}
	}
	sbs->ResetWalls(true);
	sbs->GetTextureManager()->ResetTextureMapping();

	return wall;
}

Wall* Stairwell::Level::AddWall(const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real height1, Real height2, Real voffset1, Real voffset2, Real tw, Real th)
{
	//exit with an error if floor is invalid
	/*if (IsValid() == false)
	{
		parent->ReportError("AddWall: Floor " + ToString(floornum) + " out of range");
		return 0;
	}*/

	Wall *wall = mesh->CreateWallObject(name);
	AddWall(wall, name, texture, thickness, x1, z1, x2, z2, height1, height2, voffset1, voffset2, tw, th);
	return wall;
}

bool Stairwell::Level::AddWall(Wall *wall, const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real height1, Real height2, Real voffset1, Real voffset2, Real tw, Real th)
{
	//exit with an error if floor is invalid
	//if (IsValid() == false)
		//return parent->ReportError("AddWall: Floor " + ToString(floornum) + " out of range");

	return sbs->AddWallMain(wall, name, texture, thickness, x1, z1, x2, z2, height1, height2, voffset1, voffset2, tw, th, true);
}

Wall* Stairwell::Level::AddFloor(const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real voffset1, Real voffset2, bool reverse_axis, bool texture_direction, Real tw, Real th, bool legacy_behavior)
{
	//exit with an error if floor is invalid
	/*if (IsValid() == false)
	{
		parent->ReportError("AddFloor: Floor " + ToString(floornum) + " out of range");
		return 0;
	}*/

	Wall *wall = mesh->CreateWallObject(name);
	AddFloor(wall, name, texture, thickness, x1, z1, x2, z2, voffset1, voffset2, reverse_axis, texture_direction, tw, th, legacy_behavior);
	return wall;
}

bool Stairwell::Level::AddFloor(Wall *wall, const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real voffset1, Real voffset2, bool reverse_axis, bool texture_direction, Real tw, Real th, bool legacy_behavior)
{
	//exit with an error if floor is invalid
	//if (IsValid() == false)
		//return parent->ReportError("AddFloor: Floor " + ToString(floornum) + " out of range");

	return sbs->AddFloorMain(wall, name, texture, thickness, x1, z1, x2, z2, voffset1, voffset2, reverse_axis, texture_direction, tw, th, true, legacy_behavior);
}

void Stairwell::Level::Enabled(bool value)
{
	//turns stairwell on/off for a specific floor

	SBS_PROFILE("Stairwell::Level::Enabled");
	if (IsEnabled() != value)
	{
		mesh->Enabled(value);
		enabled = value;

		//doors
		for (size_t i = 0; i < DoorArray.size(); i++)
		{
			if (DoorArray[i])
				DoorArray[i]->Enabled(value);
		}

		//controls
		for (size_t i = 0; i < ControlArray.size(); i++)
		{
			if (ControlArray[i])
				ControlArray[i]->Enabled(value);
		}

		//triggers
		/*for (size_t i = 0; i < TriggerArray.size(); i++)
		{
			if (TriggerArray[i])
				TriggerArray[i]->Enabled(value);
		}*/

		//models
		for (size_t i = 0; i < ModelArray.size(); i++)
		{
			if (ModelArray[i])
				ModelArray[i]->Enabled(value);
		}

		//lights
		for (size_t i = 0; i < lights.size(); i++)
		{
			if (lights[i])
				lights[i]->Enabled(value);
		}
	}
}

Door* Stairwell::Level::AddDoor(std::string name, const std::string &open_sound, const std::string &close_sound, bool open_state, const std::string &texture, const std::string &side_texture, Real thickness, const std::string &face_direction, const std::string &open_direction, bool rotate, Real open_speed, Real close_speed, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, Real tw, Real th, Real side_tw, Real side_th)
{
	//add a door

	//exit with an error if floor is invalid
	/*if (IsValid() == false)
	{
		parent->ReportError("AddDoor: Floor " + ToString(floornum) + " out of range");
		return 0;
	}*/

	Floor *floorptr = sbs->GetFloor(floornum);
	if (!floorptr)
		return 0;

	//set up coordinates
	Real x1, z1, x2, z2;
	if (face_direction == "left" || face_direction == "right")
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
	if (face_direction == "left" || face_direction == "right")
	{
		Cut(1, Vector3(x1 - 0.5, voffset, z1), Vector3(x2 + 0.5, voffset + height, z2), true, false, 1);
		floorptr->Cut(Vector3(GetPosition().x + x1 - 0.5, floorptr->GetBase(true) + voffset, GetPosition().z + z1), Vector3(GetPosition().x + x2 + 0.5, floorptr->GetBase(true) + voffset + height, GetPosition().z + z2), true, false, true, 2);
	}
	else
	{
		Cut(1, Vector3(x1, voffset, z1 - 0.5), Vector3(x2, voffset + height, z2 + 0.5), true, false, 1);
		floorptr->Cut(Vector3(GetPosition().x + x1, floorptr->GetBase(true) + voffset, GetPosition().z + z1 - 0.5), Vector3(GetPosition().x + x2, floorptr->GetBase(true) + voffset + height, GetPosition().z + z2 + 0.5), true, false, true, 2);
	}

	//create doorway walls
	sbs->AddDoorwayWalls(mesh, "Connection Walls", "ConnectionWall", 0, 0);

	std::string num = ToString((int)DoorArray.size());
	if (name == "")
		name = "Door " + num;

	Door* door = new Door(this, parent->DoorWrapper, name, open_sound, close_sound, rotate);
	door->CreateDoor(open_state, texture, side_texture, thickness, face_direction, open_direction, open_speed, close_speed, CenterX, CenterZ, width, height, voffset, tw, th, side_tw, side_th);
	DoorArray.push_back(door);

	floorptr = 0;
	return door;
}

Door* Stairwell::Level::CreateDoor(std::string name, const std::string &open_sound, const std::string &close_sound, bool rotate)
{
	//start creation of a manual door
	//since the door is unfinished, AddDoorComponent and FinishDoor need to be run on the returned Door object

	std::string num = ToString((int)DoorArray.size());
	if (name == "")
		name = "Door " + num;

	Door* door = new Door(this, parent->DoorWrapper, name, open_sound, close_sound, rotate);
	DoorArray.push_back(door);
	return door;
}

Door* Stairwell::Level::GetDoor(const std::string &name)
{
	for (int i = 0; i < DoorArray.size(); i++)
	{
		if (DoorArray[i]->GetName() == name)
			return DoorArray[i];
	}
	return 0;
}

bool Stairwell::Level::Cut(bool relative, const Vector3 &start, const Vector3 &end, bool cutwalls, bool cutfloors, int checkwallnumber)
{
	//Cut through a wall segment
	//the Y values in start and end are both relative to the floor's base

	//exit with an error if floor is invalid
	/*if (IsValid() == false)
	{
		if (sbs->Verbose)
			parent->ReportError("Cut: Floor " + ToString(floornum) + " out of range");
		else
			sbs->LastError = "Cut: Floor " + ToString(floornum) + " out of range";
		return false;
	}*/

	if (!sbs->GetFloor(floornum))
		return false;

	for (size_t i = 0; i < mesh->Walls.size(); i++)
	{
		bool reset = true;
		if (i > 0)
			reset = false;

		if (relative == true)
			sbs->Cut(mesh->Walls[i], Vector3(start.x, start.y, start.z), Vector3(end.x, end.y, end.z), cutwalls, cutfloors, checkwallnumber, reset);
		else
			sbs->Cut(mesh->Walls[i], Vector3(start.x - GetPosition().x, start.y, start.z - GetPosition().z), Vector3(end.x - GetPosition().x, end.y, end.z - GetPosition().z), cutwalls, cutfloors, checkwallnumber, reset);
	}

	return true;
}

bool Stairwell::Level::IsEnabled()
{
	return enabled;
}

void Stairwell::Level::RemoveDoor(Door *door)
{
	//remove a door reference (this does not delete the object)
	for (size_t i = 0; i < DoorArray.size(); i++)
	{
		if (DoorArray[i] == door)
		{
			DoorArray.erase(DoorArray.begin() + i);
			return;
		}
	}
}

void Stairwell::Level::RemoveLight(Light *light)
{
	//remove a light reference (does not delete the object itself)
	for (size_t i = 0; i < lights.size(); i++)
	{
		if (lights[i] == light)
		{
			lights.erase(lights.begin() + i);
			return;
		}
	}
}

void Stairwell::Level::RemoveModel(Model *model)
{
	//remove a model reference (does not delete the object itself)
	for (size_t i = 0; i < ModelArray.size(); i++)
	{
		if (ModelArray[i] == model)
		{
			ModelArray.erase(ModelArray.begin() + i);
			return;
		}
	}
}

void Stairwell::Level::RemoveControl(Control *control)
{
	//remove a control reference (does not delete the object itself)
	for (size_t i = 0; i < ControlArray.size(); i++)
	{
		if (ControlArray[i] == control)
		{
			ControlArray.erase(ControlArray.begin() + i);
			return;
		}
	}
}

void Stairwell::Level::RemoveTrigger(Trigger *trigger)
{
	//remove a trigger reference (does not delete the object itself)
	/*for (size_t i = 0; i < TriggerArray.size(); i++)
	{
		if (TriggerArray[i] == trigger)
		{
			TriggerArray.erase(TriggerArray.begin() + i);
			return;
		}
	}*/
}

Light* Stairwell::Level::AddLight(const std::string &name, int type)
{
	//add a global light

	Light* light = new Light(mesh, name, type);
	lights.push_back(light);
	return light;
}

Light* Stairwell::Level::GetLight(const std::string &name)
{
	for (int i = 0; i < lights.size(); i++)
	{
		if (lights[i]->GetName() == name)
			return lights[i];
	}
	return 0;
}

MeshObject* Stairwell::Level::GetMeshObject()
{
	//returns the mesh object for the specified floor

	return mesh;
}

Model* Stairwell::Level::AddModel(const std::string &name, const std::string &filename, bool center, Vector3 position, Vector3 rotation, Real max_render_distance, Real scale_multiplier, bool enable_physics, Real restitution, Real friction, Real mass)
{
	//add a model

	Model* model = new Model(mesh, name, filename, center, position, rotation, max_render_distance, scale_multiplier, enable_physics, restitution, friction, mass);
	if (model->load_error == true)
	{
		delete model;
		return 0;
	}
	ModelArray.push_back(model);
	return model;
}

void Stairwell::Level::AddModel(Model *model)
{
	//add a model reference

	if (!model)
		return;

	for (size_t i = 0; i < ModelArray.size(); i++)
	{
		if (ModelArray[i] == model)
			return;
	}

	ModelArray.push_back(model);
}

Control* Stairwell::Level::AddControl(const std::string &name, const std::string &sound, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, int selection_position, std::vector<std::string> &action_names, std::vector<std::string> &textures)
{
	//add a control

	std::vector<Action*> actionnull; //not used
	Control* control = new Control(mesh, name, false, sound, action_names, actionnull, textures, direction, width, height, true, selection_position);
	control->Move(CenterX, voffset, CenterZ);
	ControlArray.push_back(control);
	return control;
}

Trigger* Stairwell::Level::AddTrigger(const std::string &name, const std::string &sound_file, Vector3 &area_min, Vector3 &area_max, std::vector<std::string> &action_names)
{
	//triggers are disabled for now

	//add a trigger

	//exit if floor is invalid
	/*if (!IsValid())
		return 0;

	Trigger* trigger = new Trigger(mesh, name, false, sound_file, area_min, area_max, action_names);
	TriggerArray.push_back(trigger);
	return trigger;*/
	return 0;
}

Model* Stairwell::Level::GetModel(std::string name)
{
	//get a model by name

	SetCase(name, false);

	for (size_t i = 0; i < ModelArray.size(); i++)
	{
		if (SetCaseCopy(ModelArray[i]->GetName(), false) == name)
			return ModelArray[i];
	}

	return 0;
}

void Stairwell::Level::ReplaceTexture(const std::string &oldtexture, const std::string &newtexture)
{
	mesh->ReplaceTexture(oldtexture, newtexture);
}

int Stairwell::Level::GetFloor()
{
	return floornum;
}

void Stairwell::Level::Loop()
{
	//level runloop

	SBS_PROFILE("Stairwell::Level::Loop");

	LoopChildren();
}

CameraTexture* Stairwell::Level::AddCameraTexture(const std::string &name, int quality, Real fov, const Vector3 &position, bool use_rotation, const Vector3 &rotation)
{
	//add a camera texture
	CameraTexture* cameratexture = new CameraTexture(this, name, quality, fov, position, use_rotation, rotation);
	CameraTextureArray.push_back(cameratexture);
	return cameratexture;
}

}
