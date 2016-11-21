/* $Id$ */

/*
	Scalable Building Simulator - Manager Classes
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
#include "elevator.h"
#include "shaft.h"
#include "stairs.h"
#include "door.h"
#include "revolvingdoor.h"
#include "dynamicmesh.h"
#include "manager.h"

namespace SBS {

FloorManager::FloorManager(Object* parent) : Object(parent)
{
	//set up SBS object
	SetValues("FloorManager", "Floor Manager", true);

	get_result = 0;
	get_number = 0;
	floors = new DynamicMesh(this, GetSceneNode(), "Floor Container");
	interfloors = new DynamicMesh(this, GetSceneNode(), "Interfloor Container");
	columnframes = new DynamicMesh(this, GetSceneNode(), "Columnframe Container");
}

FloorManager::~FloorManager()
{
	//delete floors
	for (size_t i = 0; i < Array.size(); i++)
	{
		if (Array[i].object)
		{
			Array[i].object->parent_deleting = true;
			delete Array[i].object;
		}
		Array[i].object = 0;
	}

	//delete dynamic meshes
	if (floors)
		delete floors;
	floors = 0;
	if (interfloors)
		delete interfloors;
	interfloors = 0;
	if (columnframes)
		delete columnframes;
	columnframes = 0;
}

Floor* FloorManager::Create(int number)
{
	//create a new floor object

	if (Get(number))
		return 0;

	Map floor;
	floor.number = number;
	floor.object = new Floor(this, this, number);
	Array.push_back(floor);

	if (number < 0)
		sbs->Basements++;
	else
		sbs->Floors++;
	return floor.object;
}

int FloorManager::GetCount()
{
	//return the number of floors, including basements
	return (int)Array.size();
}

Floor* FloorManager::Get(int number)
{
	//return pointer to floor object

	//return previous cached entry if the same
	if (get_number == number && get_result)
		return get_result;

	if (Array.size() > 0)
	{
		//quick prediction
		if (sbs->Basements + number < (int)Array.size() && sbs->Basements + number >= 0)
		{
			Map floor = Array[sbs->Basements + number];
			if (floor.number == number)
			{
				if (floor.object)
				{
					get_number = number;
					get_result = floor.object;
					return floor.object;
				}
				else
				{
					get_number = 0;
					get_result = 0;
					return 0;
				}
			}
			else if (number < 0)
			{
				if (-(number + 1) < (int)Array.size())
				{
					floor = Array[-(number + 1)];
					if (floor.number == number)
					{
						if (floor.object)
						{
							get_number = number;
							get_result = floor.object;
							return floor.object;
						}
						else
						{
							get_number = 0;
							get_result = 0;
							return 0;
						}
					}
				}
			}
		}
	}

	for (size_t i = 0; i < Array.size(); i++)
	{
		if (Array[i].number == number)
		{
			get_number = number;
			get_result = Array[i].object;
			return get_result;
		}
	}

	get_number = 0;
	get_result = 0;
	return 0;
}

Floor* FloorManager::GetIndex(int index)
{
	if (index < 0 || index >= (int)Array.size())
		return 0;

	return Array[index].object;
}

void FloorManager::Remove(Floor *floor)
{
	//remove a floor (does not delete the object)
	for (size_t i = 0; i < Array.size(); i++)
	{
		if (Array[i].object == floor)
		{
			if (Array[i].object->Number < 0)
				sbs->Basements--;
			else
				sbs->Floors--;

			Array.erase(Array.begin() + i);

			//clear cached values
			get_result = 0;
			get_number = 0;
			return;
		}
	}
}

void FloorManager::EnableAll(bool value)
{
	//enable or disable all floors
	for (size_t i = 0; i < Array.size(); i++)
		Array[i].object->Enabled(value);

	//enable/disable dynamic meshes
	floors->Enable(value);
	interfloors->Enable(value);
	columnframes->Enable(value);
}

ElevatorManager::ElevatorManager(Object* parent) : Object(parent)
{
	//set up SBS object
	SetValues("ElevatorManager", "Elevator Manager", true);

	get_result = 0;
	get_number = 0;
	EnableLoop(true);
}

ElevatorManager::~ElevatorManager()
{
	//delete elevators
	for (size_t i = 0; i < Array.size(); i++)
	{
		if (Array[i].object)
		{
			Array[i].object->parent_deleting = true;
			delete Array[i].object;
		}
		Array[i].object = 0;
	}
}

Elevator* ElevatorManager::Create(int number)
{
	//create a new elevator object

	if (Get(number))
		return 0;

	Map elev;
	elev.number = number;
	elev.object = new Elevator(this, number);
	Array.push_back(elev);
	return elev.object;
}

int ElevatorManager::GetCount()
{
	//return the number of elevators
	return (int)Array.size();
}

Elevator* ElevatorManager::Get(int number)
{
	//return pointer to elevator object

	if (number < 1 || number > GetCount())
		return 0;

	if (get_number == number && get_result)
		return get_result;

	if ((int)Array.size() > number - 1)
	{
		//quick prediction
		if (Array[number - 1].number == number)
		{
			if (Array[number - 1].object)
			{
				get_number = number;
				get_result = Array[number - 1].object;
				return get_result;
			}
			else
			{
				get_number = 0;
				get_result = 0;
				return 0;
			}
		}
	}

	for (size_t i = 0; i < Array.size(); i++)
	{
		if (Array[i].number == number)
		{
			get_number = number;
			get_result = Array[i].object;
			return get_result;
		}
	}

	get_number = 0;
	get_result = 0;
	return 0;
}

Elevator* ElevatorManager::GetIndex(int index)
{
	if (index < 0 || index >= (int)Array.size())
		return 0;

	return Array[index].object;
}

void ElevatorManager::Remove(Elevator *elevator)
{
	//remove an elevator (does not delete the object)
	for (size_t i = 0; i < Array.size(); i++)
	{
		if (Array[i].object == elevator)
		{
			Array.erase(Array.begin() + i);

			//clear cached values
			get_result = 0;
			get_number = 0;
			return;
		}
	}
}

void ElevatorManager::EnableAll(bool value)
{
	//turn off elevators, if the related shaft is only partially shown

	for (size_t i = 0; i < Array.size(); i++)
	{
		Shaft *shaft = Array[i].object->GetShaft();

		if (value == false)
			value = shaft->GetShowFull();

		Array[i].object->Enabled(value);
	}
}

void ElevatorManager::Loop()
{
	if (sbs->ProcessElevators == true)
		LoopChildren();
}

ShaftManager::ShaftManager(Object* parent) : Object(parent)
{
	//set up SBS object
	SetValues("ShaftManager", "Shaft Manager", true, false);

	get_result = 0;
	get_number = 0;
	EnableLoop(true);
}

ShaftManager::~ShaftManager()
{
	//delete shafts
	for (size_t i = 0; i < Array.size(); i++)
	{
		if (Array[i].object)
		{
			Array[i].object->parent_deleting = true;
			delete Array[i].object;
		}
		Array[i].object = 0;
	}
}

Shaft* ShaftManager::Create(int number, float CenterX, float CenterZ, int _startfloor, int _endfloor)
{
	//create a shaft object

	for (size_t i = 0; i < Array.size(); i++)
	{
		if (Array[i].number == number)
		{
			std::string num = ToString(number);
			sbs->ReportError("Shaft " + num + " already exists");
			return 0;
		}
	}

	//verify floor range
	if (_startfloor > _endfloor)
	{
		sbs->ReportError("CreateShaft: starting floor is greater than ending floor");
		return 0;
	}

	if (sbs->IsValidFloor(_startfloor) == false)
	{
		std::string num = ToString(_startfloor);
		sbs->ReportError("CreateShaft: Invalid starting floor " + num);
		return 0;
	}
	if (sbs->IsValidFloor(_endfloor) == false)
	{
		std::string num = ToString(_endfloor);
		sbs->ReportError("CreateShaft: Invalid ending floor " + num);
		return 0;
	}

	Map shaft;
	shaft.number = number;
	shaft.object = new Shaft(this, number, CenterX, CenterZ, _startfloor, _endfloor);
	Array.push_back(shaft);
	return shaft.object;
}

int ShaftManager::GetCount()
{
	//return the number of shafts
	return (int)Array.size();
}

Shaft* ShaftManager::Get(int number)
{
	//return pointer to shaft object

	if (number < 1 || number > GetCount())
		return 0;

	if (get_number == number && get_result)
		return get_result;

	if ((int)Array.size() > number - 1)
	{
		//quick prediction
		if (Array[number - 1].number == number)
		{
			if (Array[number - 1].object)
			{
				get_number = number;
				get_result = Array[number - 1].object;
				return get_result;
			}
			else
			{
				get_number = 0;
				get_result = 0;
				return 0;
			}
		}
	}

	for (size_t i = 0; i < Array.size(); i++)
	{
		if (Array[i].number == number)
		{
			get_number = number;
			get_result = Array[i].object;
			return get_result;
		}
	}

	get_number = 0;
	get_result = 0;
	return 0;
}

Shaft* ShaftManager::GetIndex(int index)
{
	if (index < 0 || index >= (int)Array.size())
		return 0;

	return Array[index].object;
}

void ShaftManager::Remove(Shaft *shaft)
{
	//remove a shaft (does not delete the object)
	for (size_t i = 0; i < Array.size(); i++)
	{
		if (Array[i].object == shaft)
		{
			Array.erase(Array.begin() + i);

			//clear cached values
			get_result = 0;
			get_number = 0;
			return;
		}
	}
}

void ShaftManager::EnableAll(bool value)
{
	//enable or disable all shafts
	for (size_t i = 0; i < Array.size(); i++)
		Array[i].object->EnableWholeShaft(value, true, true);
}

void ShaftManager::Loop()
{
	LoopChildren();
}

StairsManager::StairsManager(Object* parent) : Object(parent)
{
	//set up SBS object
	SetValues("StairsManager", "Stairs Manager", true, false);

	get_result = 0;
	get_number = 0;
	EnableLoop(true);
}

StairsManager::~StairsManager()
{
	//delete stairs
	for (size_t i = 0; i < Array.size(); i++)
	{
		if (Array[i].object)
		{
			Array[i].object->parent_deleting = true;
			delete Array[i].object;
		}
		Array[i].object = 0;
	}
}

Stairs* StairsManager::Create(int number, float CenterX, float CenterZ, int _startfloor, int _endfloor)
{
	//create a stairwell object

	for (size_t i = 0; i < Array.size(); i++)
	{
		if (Array[i].number == number)
		{
			std::string num = ToString(number);
			sbs->ReportError("Stairwell " + num + " already exists");
			return 0;
		}
	}

	//verify floor range
	if (_startfloor > _endfloor)
	{
		sbs->ReportError("CreateStairwell: starting floor is greater than ending floor");
		return 0;
	}
	if (sbs->IsValidFloor(_startfloor) == false)
	{
		std::string num = ToString(_startfloor);
		sbs->ReportError("CreateStairwell: Invalid starting floor " + num);
		return 0;
	}
	if (sbs->IsValidFloor(_endfloor) == false)
	{
		std::string num = ToString(_endfloor);
		sbs->ReportError("CreateStairwell: Invalid ending floor " + num);
		return 0;
	}

	Map stairs;
	stairs.number = number;
	stairs.object = new Stairs(this, number, CenterX, CenterZ, _startfloor, _endfloor);
	Array.push_back(stairs);
	return stairs.object;
}

int StairsManager::GetCount()
{
	//return the number of stairwells
	return (int)Array.size();
}

Stairs* StairsManager::Get(int number)
{
	//return pointer to stairs object

	if (number < 1 || number > GetCount())
		return 0;

	if (get_number == number && get_result)
		return get_result;

	if ((int)Array.size() > number - 1)
	{
		//quick prediction
		if (Array[number - 1].number == number)
		{
			if (Array[number - 1].object)
			{
				get_number = number;
				get_result = Array[number - 1].object;
				return get_result;
			}
			else
			{
				get_number = 0;
				get_result = 0;
				return 0;
			}
		}
	}

	for (size_t i = 0; i < Array.size(); i++)
	{
		if (Array[i].number == number)
		{
			get_number = number;
			get_result = Array[i].object;
			return get_result;
		}
	}

	get_number = 0;
	get_result = 0;
	return 0;
}

Stairs* StairsManager::GetIndex(int index)
{
	if (index < 0 || index >= (int)Array.size())
		return 0;

	return Array[index].object;
}

void StairsManager::Remove(Stairs *stairs)
{
	//remove a stairs object (does not delete the object)
	for (size_t i = 0; i < Array.size(); i++)
	{
		if (Array[i].object == stairs)
		{
			Array.erase(Array.begin() + i);

			//clear cached values
			get_result = 0;
			get_number = 0;
			return;
		}
	}
}

void StairsManager::EnableAll(bool value)
{
	//enable or disable all stairwells
	for (size_t i = 0; i < Array.size(); i++)
		Array[i].object->EnableWholeStairwell(value, true);
}

void StairsManager::Loop()
{
	LoopChildren();
}

DoorManager::DoorManager(Object* parent) : Object(parent)
{
	//set up SBS object
	SetValues("DoorManager", "Door Manager", true);

	//create a dynamic mesh for doors
	wrapper = new DynamicMesh(this, GetSceneNode(), "Door Container", 0, true);
	wrapper->force_combine = true;
	EnableLoop(true);
}

DoorManager::~DoorManager()
{
	//delete doors
	for (size_t i = 0; i < Array.size(); i++)
	{
		if (Array[i])
		{
			Array[i]->parent_deleting = true;
			delete Array[i];
		}
		Array[i] = 0;
	}

	if (wrapper)
		delete wrapper;
	wrapper = 0;
}

Door* DoorManager::AddDoor(const std::string &open_sound, const std::string &close_sound, bool open_state, const std::string &texture, float thickness, int direction, float speed, float CenterX, float CenterZ, float width, float height, float voffset, float tw, float th)
{
	int number = (int)Array.size();
	std::string name = "Door " + ToString(number);
	Door* door = new Door(this, wrapper, name, open_sound, close_sound, open_state, texture, thickness, direction, speed, CenterX, CenterZ, width, height, voffset, tw, th);
	Array.push_back(door);
	return door;
}

void DoorManager::RemoveDoor(Door *door)
{
	//remove a door from the array
	//this does not delete the object
	for (size_t i = 0; i < Array.size(); i++)
	{
		if (Array[i] == door)
		{
			Array.erase(Array.begin() + i);
			return;
		}
	}
}

int DoorManager::GetCount()
{
	//return the number of doors
	return (int)Array.size();
}

Door* DoorManager::GetIndex(int index)
{
	if (index < 0 || index >= (int)Array.size())
		return 0;

	return Array[index];
}

void DoorManager::Loop()
{
	LoopChildren();
}

RevolvingDoorManager::RevolvingDoorManager(Object* parent) : Object(parent)
{
	//set up SBS object
	SetValues("RevolvingDoorManager", "Revolving Door Manager", true);

	//create a dynamic mesh for doors
	wrapper = new DynamicMesh(this, GetSceneNode(), "Revolving Door Container", 0, true);
	wrapper->force_combine = true;
	EnableLoop(true);
}

RevolvingDoorManager::~RevolvingDoorManager()
{
	//delete doors
	for (size_t i = 0; i < Array.size(); i++)
	{
		if (Array[i])
		{
			Array[i]->parent_deleting = true;
			delete Array[i];
		}
		Array[i] = 0;
	}

	if (wrapper)
		delete wrapper;
	wrapper = 0;
}

RevolvingDoor* RevolvingDoorManager::AddDoor(const std::string &soundfile, const std::string &texture, float thickness, bool clockwise, int segments, float speed, float rotation, float CenterX, float CenterZ, float width, float height, float voffset, float tw, float th)
{
	int number = (int)Array.size();
	std::string name = "Door " + ToString(number);
	RevolvingDoor* door = new RevolvingDoor(this, wrapper, name, soundfile, texture, thickness, clockwise, segments, speed, rotation, CenterX, CenterZ, width, height, voffset, tw, th);
	Array.push_back(door);
	return door;
}

void RevolvingDoorManager::RemoveDoor(RevolvingDoor *door)
{
	//remove a door from the array
	//this does not delete the object
	for (size_t i = 0; i < Array.size(); i++)
	{
		if (Array[i] == door)
		{
			Array.erase(Array.begin() + i);
			return;
		}
	}
}

int RevolvingDoorManager::GetCount()
{
	//return the number of doors
	return (int)Array.size();
}

RevolvingDoor* RevolvingDoorManager::GetIndex(int index)
{
	if (index < 0 || index >= (int)Array.size())
		return 0;

	return Array[index];
}

void RevolvingDoorManager::Loop()
{
	LoopChildren();
}

}
