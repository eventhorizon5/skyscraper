/*
	Scalable Building Simulator - Shaft Object
	The Skyscraper Project - Version 2.1
	Copyright (C)2004-2025 Ryan Thoryk
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
#include "polymesh.h"
#include "floor.h"
#include "elevator.h"
#include "elevatorcar.h"
#include "dynamicmesh.h"
#include "mesh.h"
#include "polymesh.h"
#include "sound.h"
#include "door.h"
#include "model.h"
#include "primitive.h"
#include "custom.h"
#include "light.h"
#include "camera.h"
#include "control.h"
#include "profiler.h"
#include "cameratexture.h"
#include "utility.h"
#include "trigger.h"
#include "shape.h"
#include "shaft.h"

namespace SBS {

Shaft::Shaft(Object *parent, int number, Real CenterX, Real CenterZ, int startfloor, int endfloor) : Object(parent)
{
	//creates a shaft in the location specified by CenterX and CenterZ
	//and that spans the floor range specified by startfloor and endfloor

	//set up SBS object
	SetValues("Shaft", "", false);

	ShaftNumber = number;
	this->startfloor = startfloor;
	this->endfloor = endfloor;

	//make sure start and ending floors are within a valid range
	if (startfloor < -sbs->Basements)
		return;
	if (endfloor > sbs->Floors - 1)
		return;

	InsideShaft = false;
	IsEnabled = true;
	top = sbs->GetFloor(endfloor)->Altitude + sbs->GetFloor(endfloor)->FullHeight();
	bottom = sbs->GetFloor(startfloor)->Altitude;
	cutstart = Vector2::ZERO;
	cutend = Vector2::ZERO;
	ShowFloors = 0;
	ShowOutside = false;
	ShowInterfloors = false;
	ShowFullShaft = false;
	EnableCheck = false;
	lastcheckresult = false;
	checkfirstrun = true;
	lastposition = Vector3::ZERO;
	InElevator = false;
	ShowFloorsFull_Enabled = false;

	std::string name = "Shaft " + ToString(number);

	SetName(name);
	SetPosition(CenterX, sbs->GetFloor(startfloor)->Altitude, CenterZ);

	dynamic_mesh = new DynamicMesh(this, GetSceneNode(), name);

	for (int i = startfloor; i <= endfloor; i++)
	{
		Levels.emplace_back(new Level(this, i));
	}

	//create a dynamic mesh for doors
	DoorWrapper = new DynamicMesh(this, GetSceneNode(), GetName() + " Door Container", 0, true);

	//create a dynamic mesh for doors
	ShaftDoorContainer = new DynamicMesh(this, GetSceneNode(), name + " Shaft Door Container", 0, true);

	Report("created at " + TruncateNumber(CenterX, 4) + ", " + TruncateNumber(CenterZ, 4));

	EnableLoop(true);
}

Shaft::~Shaft()
{
	//destructor

	//delete levels
	for (size_t i = 0; i < Levels.size(); i++)
	{
		if (Levels[i])
			delete Levels[i];
		Levels[i] = 0;
	}

	if (DoorWrapper)
		delete DoorWrapper;
	DoorWrapper = 0;

	if (ShaftDoorContainer)
		delete ShaftDoorContainer;
	ShaftDoorContainer = 0;

	//delete dynamic mesh
	if (dynamic_mesh)
		delete dynamic_mesh;
	dynamic_mesh = 0;

	//unregister from parent
	if (sbs->FastDelete == false && parent_deleting == false)
		sbs->RemoveShaft(this);
}

Shaft::Level* Shaft::GetLevel(int floor)
{
	for (size_t i = 0; i < Levels.size(); i++)
	{
		if (Levels[i]->GetFloor() == floor)
			return Levels[i];
	}
	return 0;
}

bool Shaft::EnableWhole(bool value, bool EnableShaftDoors, bool force)
{
	//turn on/off entire shaft

	if (value == false && ShowFullShaft == true)
		return true;

	if (force == true)
		IsEnabled = !value;

	bool status = true;

	if (IsEnabled == !value && EnableCheck == false)
	{
		for (int i = startfloor; i <= endfloor; i++)
		{
			if (force == true)
				GetLevel(i)->enabled = !value;
			bool result = GetLevel(i)->Enabled(value, EnableShaftDoors);
			if (!result)
				status = false;
		}
	}

	//enable/disable dynamic meshes
	bool result = dynamic_mesh->Enabled(value);
	if (!result)
		status = false;
	result = ShaftDoorContainer->Enabled(value);
	if (!result)
		status = false;
	result = DoorWrapper->Enabled(value);
	if (!result)
		status = false;

	IsEnabled = value;
	if (ShowFullShaft == true)
		EnableCheck = true;

	return status;
}

bool Shaft::IsInside(const Vector3 &position)
{
	//SBS_PROFILE("Shaft::IsInShaft");

	//if last position is the same as new, return previous result
	if (position.positionEquals(lastposition) == true && checkfirstrun == false)
		return lastcheckresult;

	checkfirstrun = false;

	if (position.y > bottom && position.y < top && Levels.size() > 0)
	{
		//first determine if camera has X and Z values within the first shaft floor's bounding box
		if (Levels[0]->GetMeshObject()->InBoundingBox(position, false) == true)
		{
			//do a hit beam test from the position to the bottom of the shaft, to see if it hits a shaft floor
			bool result = (Levels[0]->GetMeshObject()->HitBeam(position, Vector3::NEGATIVE_UNIT_Y, position.y - (bottom - 1)) >= 0);

			//cache values
			lastcheckresult = result;
			lastposition = position;

			return result;
		}
	}

	//cache values
	lastcheckresult = false;
	lastposition = position;

	return false;
}

void Shaft::CutFloors(bool relative, const Vector2 &start, const Vector2 &end, Real startvoffset, Real endvoffset)
{
	//Cut through floor/ceiling polygons on all associated levels, within the voffsets

	Report("cutting...");

	Real voffset1, voffset2;

	if (relative == true)
	{
		cutstart = start;
		cutend = end;
	}
	else
	{
		cutstart = Vector2(start.x - GetPosition().x, start.y - GetPosition().z);
		cutend =  Vector2(end.x - GetPosition().x, end.y - GetPosition().z);
	}

	if (!sbs->GetFloor(startfloor) || !sbs->GetFloor(endfloor))
		return;

	for (int i = startfloor; i <= endfloor; i++)
	{
		Floor *floorptr = sbs->GetFloor(i);
		if (!floorptr)
			continue;

		voffset1 = 0;
		voffset2 = floorptr->FullHeight() + 1;

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
			if (!sbs->External->Walls[i])
				continue;

			if (relative == true)
				sbs->GetPolyMesh()->Cut(sbs->External->Walls[i], Vector3(GetPosition().x + start.x, voffset1, GetPosition().z + start.y), Vector3(GetPosition().x + end.x, voffset2, GetPosition().z + end.y), false, true);
			else
				sbs->GetPolyMesh()->Cut(sbs->External->Walls[i], Vector3(start.x, voffset1, start.y), Vector3(end.x, voffset2, end.y), false, true);
		}
	}
}

void Shaft::EnableRange(int floor, int range, bool value, bool EnableShaftDoors)
{
	//turn on/off a range of floors
	//if range is 3, show shaft on current floor (floor), and 1 floor below and above (3 total floors)
	//if range is 1, show only the current floor (floor)

	//exit if ShowFullShaft is true
	if (ShowFullShaft == true)
		return;

	if (!sbs->GetFloor(floor))
		return;

	SBS_PROFILE("Shaft::EnableRange");

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
				GetLevel(floor - additionalfloors - 1)->Enabled(false, EnableShaftDoors);
		}
		if (floor + additionalfloors + 1 >= startfloor && floor + additionalfloors + 1 <= endfloor)
		{
			if (sbs->GetFloor(floor)->IsInGroup(floor + additionalfloors + 1) == false) //only disable if not in group
				GetLevel(floor + additionalfloors + 1)->Enabled(false, EnableShaftDoors);
		}
	}

	//enable floors within range
	for (int i = floor - additionalfloors; i <= floor + additionalfloors; i++)
	{
		if (i >= startfloor && i <= endfloor)
			GetLevel(i)->Enabled(value, EnableShaftDoors);
	}
}

void Shaft::AddShowFloor(int floor)
{
	//adds a floor number to the ShowFloors list

	if (IsShowFloor(floor))
		return;

	ShowFloorsList.emplace_back(floor);
	std::sort(ShowFloorsList.begin(), ShowFloorsList.end());
}

void Shaft::RemoveShowFloor(int floor)
{
	//removes a floor number from the ShowFloors list

	for (size_t i = 0; i < ShowFloorsList.size(); i++)
	{
		if (ShowFloorsList[i] == floor)
		{
			ShowFloorsList.erase(ShowFloorsList.begin() + i);
			return;
		}
	}
}

bool Shaft::IsShowFloor(int floor)
{
	//return true if a floor is in the ShowFloors list

	for (size_t i = 0; i < ShowFloorsList.size(); i++)
	{
		if (ShowFloorsList[i] == floor)
			return true;
	}
	return false;
}

void Shaft::AddShowOutside(int floor)
{
	//adds a floor number to the ShowOutside list

	if (IsShowOutside(floor))
		return;

	ShowOutsideList.emplace_back(floor);
	std::sort(ShowOutsideList.begin(), ShowOutsideList.end());
}

void Shaft::RemoveShowOutside(int floor)
{
	//removes a floor number from the ShowOutside list

	for (size_t i = 0; i < ShowOutsideList.size(); i++)
	{
		if (ShowOutsideList[i] == floor)
		{
			ShowOutsideList.erase(ShowOutsideList.begin() + i);
			return;
		}
	}
}

bool Shaft::IsShowOutside(int floor)
{
	//return true if a floor is in the ShowOutside list

	for (size_t i = 0; i < ShowOutsideList.size(); i++)
	{
		if (ShowOutsideList[i] == floor)
			return true;
	}
	return false;
}

void Shaft::AddShowInterfloor(int floor)
{
	//adds a floor number to the ShowInterfloors list

	if (IsShowInterfloor(floor))
		return;

	ShowInterfloorsList.emplace_back(floor);
	std::sort(ShowInterfloorsList.begin(), ShowInterfloorsList.end());
}

void Shaft::RemoveShowInterfloor(int floor)
{
	//removes a floor number from the ShowInterfloors list

	for (size_t i = 0; i < ShowInterfloorsList.size(); i++)
	{
		if (ShowInterfloorsList[i] == floor)
		{
			ShowInterfloorsList.erase(ShowInterfloorsList.begin() + i);
			return;
		}
	}
}

bool Shaft::IsShowInterfloor(int floor)
{
	//return true if a floor is in the ShowInterfloors list

	for (size_t i = 0; i < ShowInterfloorsList.size(); i++)
	{
		if (ShowInterfloorsList[i] == floor)
			return true;
	}
	return false;
}

bool Shaft::IsValidFloor(int floor)
{
	//return true if the shaft services the specified floor

	if (floor < startfloor || floor > endfloor)
		return false;

	if (!GetLevel(floor))
		return false;

	return true;
}

void Shaft::AddElevator(int number)
{
	//add specified elevator to list

	for (size_t i = 0; i < elevators.size(); i++)
	{
		if (elevators[i] == number)
			return;
	}

	elevators.emplace_back(number);
	std::sort(elevators.begin(), elevators.end());
}

void Shaft::RemoveElevator(int number)
{
	//remove specified elevator from list
	for (size_t i = 0; i < elevators.size(); i++)
	{
		if (elevators[i] == number)
		{
			elevators.erase(elevators.begin() + i);
			return;
		}
	}
}

void Shaft::Report(const std::string &message)
{
	//general reporting function
	Object::Report("Shaft " + ToString(ShaftNumber) + ": " + message);
}

bool Shaft::ReportError(const std::string &message)
{
	//general reporting function
	return Object::ReportError("Shaft " + ToString(ShaftNumber) + ": " + message);
}

void Shaft::ReplaceTexture(const std::string &oldtexture, const std::string &newtexture)
{
	for (size_t i = 0; i < Levels.size(); i++)
		Levels[i]->ReplaceTexture(oldtexture, newtexture);
}

bool Shaft::OnInit()
{
	//startup initialization of shafts

	if (ShowFullShaft == false)
		EnableWhole(false, true);
	else
		EnableWhole(true, true, true);

	return true;
}

void Shaft::Check(Vector3 position, int current_floor)
{
	Elevator *elevator = sbs->GetElevator(sbs->ElevatorNumber);

	if (!elevator)
		return;

	ElevatorCar *car = elevator->GetCar(sbs->CarNumber);

	if (!car)
		return;

	SBS_PROFILE("Shaft::Check");

	if (IsInside(position) == true)
	{
		if (InsideShaft == false && sbs->InElevator == false)
		{
			//user is in the shaft
			InsideShaft = true;
			sbs->InShaft = true;
			InElevator = false;

			//turn on entire shaft
			EnableWhole(true, true);
		}
		else if (InsideShaft == true && sbs->InElevator == true)
		{
			//user has moved from the shaft to an elevator
			InsideShaft = false;
			sbs->InShaft = false;
			InElevator = true;

			EnableWhole(ShowFullShaft, true);
		}
		else if (InsideShaft == false && sbs->InElevator == true && ShowFullShaft == false)
		{
			//if user is in an elevator, show a range of the shaft at a time (while it's moving)
			EnableRange(current_floor, sbs->ShaftDisplayRange, true, false);
			car->ShaftDoorsEnabledRange(0, current_floor, sbs->ShaftDisplayRange);
		}

		//turn on related floors if ShowFloors is true
		//display a selected range of floors in the floor list if the user is in a moving elevator
		if (InsideShaft == false && sbs->InElevator == true && elevator->IsMoving == true && elevator->Leveling == false)
		{
			if (ShowFloors == 1)
				sbs->EnableFloorRange(current_floor, sbs->FloorDisplayRange, true, true, ShaftNumber);

			if (ShowOutside == true)
			{
				if (IsShowOutside(current_floor) == true)
				{
					sbs->EnableSkybox(true);
					sbs->EnableBuildings(true);
					sbs->EnableLandscape(true);
					sbs->EnableExternal(true);
				}
				else
				{
					sbs->EnableSkybox(false);
					sbs->EnableBuildings(false);
					sbs->EnableLandscape(false);
					sbs->EnableExternal(false);
				}
			}
		}

		//display the full range of floors in the floor list
		if (ShowFloors == 2 && ShowFloorsFull_Enabled == false)
		{
			ShowFloorsFull_Enabled = true;
			for (size_t i = 0; i < ShowFloorsList.size(); i++)
			{
				Floor *floor = sbs->GetFloor(ShowFloorsList[i]);
				if (floor->IsEnabled() == false)
				{
					floor->Enabled(true);
					//floor->EnableGroup(true);
				}
			}
		}

		//display interfloors
		if (ShowInterfloors == true)
		{
			for (size_t i = 0; i < ShowInterfloorsList.size(); i++)
			{
				Floor *floor = sbs->GetFloor(ShowInterfloorsList[i]);
				if (floor->IsInterfloorEnabled == false)
					floor->EnableInterfloor(true);
			}
		}

	}
	else if (InsideShaft == true || InElevator == true)
	{
		//user has moved out of the shaft
		InsideShaft = false;
		sbs->InShaft = false;
		InElevator = false;

		//turn off shaft
		EnableWhole(false, true, true);

		//disable floors listed in ShowFloors list, when "full" mode is enabled
		if (ShowFloors == 2 && ShowFloorsFull_Enabled == true)
		{
			ShowFloorsFull_Enabled = false;
			for (size_t i = 0; i < ShowFloorsList.size(); i++)
			{
				Floor *floor = sbs->GetFloor(ShowFloorsList[i]);
				if (floor->IsEnabled() == true && sbs->camera->CurrentFloor != floor->Number)
				{
					//don't disable floors that were originally enabled as part of the camera floor's group
					if ((floor->EnabledGroup == true && floor->EnabledGroup_Floor == sbs->camera->CurrentFloor) == false)
					{
						//only disable floor if not part of the camera floor's group
						if (floor->IsInGroup(sbs->camera->CurrentFloor) == false)
						{
							floor->Enabled(false);
							//floor->EnableGroup(false);
						}
					}
				}
			}
		}

		//disable interfloors
		if (ShowInterfloors == true)
		{
			for (size_t i = 0; i < ShowInterfloorsList.size(); i++)
			{
				Floor *floor = sbs->GetFloor(ShowInterfloorsList[i]);
				if (floor->IsInterfloorEnabled == true && floor->IsEnabled() == false)
					floor->EnableInterfloor(false);
			}
		}
	}
	else if (InsideShaft == false)
	{
		//show specified shaft range if outside the shaft
		EnableRange(current_floor, sbs->ShaftOutsideDisplayRange, true, true);
	}
}

bool Shaft::Loop()
{
	//shaft runloop

	return LoopChildren();
}

DynamicMesh* Shaft::GetDynamicMesh()
{
	return dynamic_mesh;
}

DynamicMesh* Shaft::GetDoorWrapper()
{
	return DoorWrapper;
}

void Shaft::SetShowFull(bool value)
{
	ShowFullShaft = value;

	//force the combining of dynamic meshes, since they'll be fully shown
	dynamic_mesh->force_combine = value;
	DoorWrapper->force_combine = value;
	ShaftDoorContainer->force_combine = value;
}

Shaft::Level::Level(Shaft *parent, int number) : Object(parent)
{
	//set up SBS object
	SetValues("Shaft Level", "", true);

	enabled = true;
	floornum = number;
	this->parent = parent;

	std::string name;
	name = "Shaft " + ToString(parent->ShaftNumber) + ": Level " + ToString(number);
	SetName(name);

	//Create level mesh
	mesh = new MeshObject(this, parent->GetName() + ":" + ToString(floornum), parent->GetDynamicMesh());
	SetPositionY(sbs->GetFloor(number)->Altitude);

	EnableLoop(true);
}

Shaft::Level::~Level()
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
	for (size_t i = 0; i < TriggerArray.size(); i++)
	{
		if (TriggerArray[i])
		{
			TriggerArray[i]->parent_deleting = true;
			delete TriggerArray[i];
		}
		TriggerArray[i] = 0;
	}

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

	//delete primitives
	for (size_t i = 0; i < PrimArray.size(); i++)
	{
		if (PrimArray[i])
		{
			PrimArray[i]->parent_deleting = true;
			delete PrimArray[i];
		}
		PrimArray[i] = 0;
	}

	//delete custom objects
	for (size_t i = 0; i < CustomObjectArray.size(); i++)
	{
		if (CustomObjectArray[i])
		{
			CustomObjectArray[i]->parent_deleting = true;
			delete CustomObjectArray[i];
		}
		CustomObjectArray[i] = 0;
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

Wall* Shaft::Level::AddWall(const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real height1, Real height2, Real voffset1, Real voffset2, Real tw, Real th)
{
	//exit with an error if floor is invalid
	/*if (IsValidFloor(floor) == false)
	{
		ReportError("AddWall: Floor " + ToString(floor) + " out of range");
		return 0;
	}*/

	Wall *wall = mesh->CreateWallObject(name);
	AddWall(wall, name, texture, thickness, x1, z1, x2, z2, height1, height2, voffset1, voffset2, tw, th);
	return wall;
}

bool Shaft::Level::AddWall(Wall *wall, const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real height1, Real height2, Real voffset1, Real voffset2, Real tw, Real th)
{
	//exit with an error if floor is invalid
	/*if (IsValidFloor(floor) == false)
		return ReportError("AddWall: Floor " + ToString(floor) + " out of range");*/

	return sbs->GetPolyMesh()->AddWallMain(wall, name, texture, thickness, x1, z1, x2, z2, height1, height2, voffset1, voffset2, tw, th, true);
}

Wall* Shaft::Level::AddFloor(const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real voffset1, Real voffset2, bool reverse_axis, bool texture_direction, Real tw, Real th, bool legacy_behavior)
{
	//exit with an error if floor is invalid
	/*if (IsValidFloor(floor) == false)
	{
		ReportError("AddFloor: Floor " + ToString(floor) + " out of range");
		return 0;
	}*/

	Wall *wall = mesh->CreateWallObject(name);
	AddFloor(wall, name, texture, thickness, x1, z1, x2, z2, voffset1, voffset2, reverse_axis, texture_direction, tw, th, legacy_behavior);
	return wall;
}

bool Shaft::Level::AddFloor(Wall *wall, const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real voffset1, Real voffset2, bool reverse_axis, bool texture_direction, Real tw, Real th, bool legacy_behavior)
{
	//exit with an error if floor is invalid
	//if (IsValidFloor(floor) == false)
		//return ReportError("AddFloor: Floor " + ToString(floor) + " out of range");

	//get shaft extents
	Real altitude = sbs->GetFloor(floornum)->Altitude;

	//recalculate shaft extents if needed
	if (altitude + voffset1 < parent->bottom)
		parent->bottom = altitude + voffset1;
	if (altitude + voffset2 < parent->bottom)
		parent->bottom = altitude + voffset2;
	if (altitude + voffset1 > parent->top)
		parent->top = altitude + voffset1;
	if (altitude + voffset2 > parent->top)
		parent->top = altitude + voffset2;

	return sbs->GetPolyMesh()->AddFloorMain(wall, name, texture, thickness, x1, z1, x2, z2, voffset1, voffset2, reverse_axis, texture_direction, tw, th, true, legacy_behavior);
}

bool Shaft::Level::Enabled(bool value, bool EnableShaftDoors)
{
	SBS_PROFILE("Shaft::Enabled");
	bool status = true;
	Utility *utility = sbs->GetUtility();

	if (IsEnabled() != value && parent->EnableCheck == false)
	{
		//turns shaft on/off for a specific floor

		bool result = mesh->Enabled(value);
		if (!result)
			status = false;
		enabled = value;

		//doors
		result = EnableArray(DoorArray, value);
		if (!result)
			status = false;

		//controls
		result = EnableArray(ControlArray, value);
		if (!result)
			status = false;

		//triggers
		result = EnableArray(TriggerArray, value);
		if (!result)
			status = false;

		//models
		result = EnableArray(ModelArray, value);
		if (!result)
			status = false;

		//primitives
		result = EnableArray(PrimArray, value);
		if (!result)
			status = false;

		//custom objects
		result = EnableArray(CustomObjectArray, value);
		if (!result)
			status = false;

		//lights
		result = EnableArray(lights, value);
		if (!result)
			status = false;

		if (EnableShaftDoors == true)
		{
			for (size_t i = 0; i < parent->elevators.size(); i++)
			{
				Elevator *elevator = sbs->GetElevator(parent->elevators[i]);
				if (elevator)
				{
					for (size_t j = 1; j <= elevator->GetCarCount(); j++)
					{
						ElevatorCar *car = elevator->GetCar(j);

						for (size_t k = 0; k < car->ServicedFloors.size(); k++)
						{
							if (car->ServicedFloors[k] == floornum)
								car->ShaftDoorsEnabled(0, car->ServicedFloors[k], value);
						}
					}
				}
			}
		}
	}

	return status;
}

bool Shaft::Level::Cut(bool relative, const Vector3 &start, const Vector3 &end, bool cutwalls, bool cutfloors, int checkwallnumber)
{
	//Cut through a wall segment
	//the Y values in start and end are both relative to the floor's altitude

	//exit with an error if floor is invalid
	/*if (IsValidFloor(floor) == false)
	{
		if (sbs->Verbose)
			ReportError("Cut: Floor " + ToString(floor) + " out of range");
		else
			sbs->LastError = "Cut: Floor " + ToString(floor) + " out of range";
		return false;
	}*/

	if (!sbs->GetFloor(floornum))
		return false;

	for (size_t i = 0; i < mesh->Walls.size(); i++)
	{
		if (!mesh->Walls[i])
			continue;

		bool reset = true;
		if (i > 0)
			reset = false;

		if (relative == true)
			sbs->GetPolyMesh()->Cut(mesh->Walls[i], Vector3(start.x, start.y, start.z), Vector3(end.x, end.y, end.z), cutwalls, cutfloors, checkwallnumber, reset);
		else
			sbs->GetPolyMesh()->Cut(mesh->Walls[i], Vector3(start.x - GetPosition().x, start.y, start.z - GetPosition().z), Vector3(end.x - GetPosition().x, end.y, end.z - GetPosition().z), cutwalls, cutfloors, checkwallnumber, reset);
	}
	return true;
}

bool Shaft::Level::IsEnabled()
{
	return enabled;
}

void Shaft::Level::RemoveLight(Light *light)
{
	//remove a light reference (does not delete the object itself)

	RemoveArrayElement(lights, light);
}

void Shaft::Level::RemoveModel(Model *model)
{
	//remove a model reference (does not delete the object itself)

	AddArrayElement(ModelArray, model);
}

void Shaft::Level::RemovePrimitive(Primitive *prim)
{
	//remove a prim reference (does not delete the object itself)

	AddArrayElement(PrimArray, prim);
}

void Shaft::Level::RemoveCustomObject(CustomObject *object)
{
	//remove a custom object reference (does not delete the object itself)

	AddArrayElement(CustomObjectArray, object);
}

void Shaft::Level::RemoveControl(Control *control)
{
	//remove a control reference (does not delete the object itself)

	AddArrayElement(ControlArray, control);
}

void Shaft::Level::RemoveTrigger(Trigger *trigger)
{
	//remove a trigger reference (does not delete the object itself)

	AddArrayElement(TriggerArray, trigger);
}

MeshObject* Shaft::Level::GetMeshObject()
{
	//returns the mesh object for the specified floor

	return mesh;
}

Light* Shaft::Level::AddLight(const std::string &name, int type)
{
	//add a global light

	Light* light = new Light(this, name, type);
	lights.emplace_back(light);
	return light;
}

Light* Shaft::Level::GetLight(const std::string &name)
{
	for (size_t i = 0; i < lights.size(); i++)
	{
		if (lights[i]->GetName() == name)
			return lights[i];
	}
	return 0;
}

Model* Shaft::Level::AddModel(const std::string &name, const std::string &filename, bool center, Vector3 position, Vector3 rotation, Real max_render_distance, Real scale_multiplier, bool enable_physics, Real restitution, Real friction, Real mass)
{
	//add a model

	Model* model = new Model(this, name, filename, center, position, rotation, max_render_distance, scale_multiplier, enable_physics, restitution, friction, mass);
	if (model->load_error == true)
	{
		delete model;
		return 0;
	}
	ModelArray.emplace_back(model);
	return model;
}

void Shaft::Level::AddModel(Model *model)
{
	//add a model reference

	if (!model)
		return;

	AddArrayElement(ModelArray, model);
}

Primitive* Shaft::Level::AddPrimitive(const std::string &name)
{
	//add a prim
	Primitive* prim = new Primitive(this, name);
	PrimArray.emplace_back(prim);
	return prim;
}

void Shaft::Level::AddPrimitive(Primitive *primitive)
{
	//add a model reference

	if (!primitive)
		return;

	AddArrayElement(PrimArray, primitive);
}

CustomObject* Shaft::Level::AddCustomObject(const std::string &name, const Vector3 &position, const Vector3 &rotation, Real max_render_distance, Real scale_multiplier)
{
	//add a custom object
	CustomObject* object = new CustomObject(this, name, position, rotation, max_render_distance, scale_multiplier);
	CustomObjectArray.emplace_back(object);
	return object;
}

void Shaft::Level::AddCustomObject(CustomObject *object)
{
	//add a custom object reference

	if (!object)
		return;

	AddArrayElement(CustomObjectArray, object);
}

Control* Shaft::Level::AddControl(const std::string &name, const std::string &sound, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, int selection_position, std::vector<std::string> &action_names, std::vector<std::string> &textures)
{
	//add a control

	std::vector<Action*> actionnull; //not used
	Control* control = new Control(this, name, false, sound, action_names, actionnull, textures, direction, width, height, true, selection_position);
	control->Move(CenterX, voffset, CenterZ);
	ControlArray.emplace_back(control);
	return control;
}

Trigger* Shaft::Level::AddTrigger(const std::string &name, const std::string &sound_file, Vector3 &area_min, Vector3 &area_max, std::vector<std::string> &action_names)
{
	//add a trigger

	Trigger* trigger = new Trigger(this, name, false, sound_file, area_min, area_max, action_names);
	TriggerArray.emplace_back(trigger);
	return trigger;
}

Model* Shaft::Level::GetModel(std::string name)
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

Primitive* Shaft::Level::GetPrimitive(std::string name)
{
	//get a primitive by name

	SetCase(name, false);

	for (size_t i = 0; i < PrimArray.size(); i++)
	{
		if (SetCaseCopy(PrimArray[i]->GetName(), false) == name)
			return PrimArray[i];
	}

	return 0;
}

CustomObject* Shaft::Level::GetCustomObject(std::string name)
{
	//get a custom object by name

	SetCase(name, false);

	for (size_t i = 0; i < CustomObjectArray.size(); i++)
	{
		if (SetCaseCopy(CustomObjectArray[i]->GetName(), false) == name)
			return CustomObjectArray[i];
	}

	return 0;
}

void Shaft::Level::ReplaceTexture(const std::string &oldtexture, const std::string &newtexture)
{
	mesh->ReplaceTexture(oldtexture, newtexture);
}

int Shaft::Level::GetFloor()
{
	return floornum;
}

bool Shaft::Level::Loop()
{
	//level runloop

	return LoopChildren();
}

Door* Shaft::Level::AddDoor(std::string name, const std::string &open_sound, const std::string &close_sound, bool open_state, const std::string &texture, const std::string &side_texture, Real thickness, const std::string &face_direction, const std::string &open_direction, bool rotate, Real open_speed, Real close_speed, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, Real tw, Real th, Real side_tw, Real side_th)
{
	//add a door

	//exit with an error if floor is invalid
	/*if (IsValidFloor(floor) == false)
	{
		ReportError("AddDoor: Floor " + ToString(floor) + " out of range");
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
	sbs->GetPolyMesh()->ResetDoorwayWalls();
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
	sbs->GetPolyMesh()->AddDoorwayWalls(mesh, "Connection Walls", "ConnectionWall", 0, 0);

	std::string num = ToString((int)DoorArray.size());
	if (name == "")
		name = "Door " + num;

	Door* door = new Door(this, parent->GetDoorWrapper(), name, open_sound, close_sound, rotate);
	door->CreateDoor(open_state, texture, side_texture, thickness, face_direction, open_direction, open_speed, close_speed, CenterX, CenterZ, width, height, floorptr->GetBase(true) + voffset, tw, th, side_tw, side_th);
	DoorArray.emplace_back(door);

	floorptr = 0;
	return door;
}

Door* Shaft::Level::CreateDoor(std::string name, const std::string &open_sound, const std::string &close_sound, bool rotate)
{
	//start creation of a manual door
	//since the door is unfinished, AddDoorComponent and FinishDoor need to be run on the returned Door object

	std::string num = ToString((int)DoorArray.size());
	if (name == "")
		name = "Door " + num;

	Door* door = new Door(this, parent->GetDoorWrapper(), name, open_sound, close_sound, rotate);
	DoorArray.emplace_back(door);
	return door;
}

Door* Shaft::Level::GetDoor(const std::string &name)
{
	for (size_t i = 0; i < DoorArray.size(); i++)
	{
		if (DoorArray[i]->GetName() == name)
			return DoorArray[i];
	}
	return 0;
}

void Shaft::Level::RemoveDoor(Door *door)
{
	//remove a door reference (this does not delete the object)

	AddArrayElement(DoorArray, door);
}

CameraTexture* Shaft::Level::AddCameraTexture(const std::string &name, int quality, Real fov, const Vector3 &position, bool use_rotation, const Vector3 &rotation)
{
	//add a camera texture
	CameraTexture* cameratexture = new CameraTexture(this, name, quality, fov, position, use_rotation, rotation);
	CameraTextureArray.emplace_back(cameratexture);
	return cameratexture;
}

void Shaft::Level::RemoveCameraTexture(CameraTexture* camtex)
{
	//remove a cameratexture reference (does not delete the object itself)

	AddArrayElement(CameraTextureArray, camtex);
}

Shape* Shaft::Level::CreateShape(Wall *wall)
{
	//Creates a shape in the specified wall object
	//returns a Shape object, which must be deleted by the caller after use

	if (!wall)
		return 0;

	Shape *shape = new Shape(wall);
	//shape->origin = Vector3(0, 0, 0);
	return shape;
}

}
