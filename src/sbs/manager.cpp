/* $Id$ */

/*
	Scalable Building Simulator - Manager Classes
	The Skyscraper Project - Version 1.10 Alpha
	Copyright (C)2004-2016 Ryan Thoryk
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
#include "unix.h"
#include "dynamicmesh.h"
#include "manager.h"

namespace SBS {

FloorManager::FloorManager(Object* parent) : Object(parent)
{
	//set up SBS object
	SetValues("FloorManager", "Floor Manager", true, false);

	get_result = 0;
	get_number = 0;
	floors = new DynamicMesh(this, sbs->GetSceneNode(), "Floor Container");
	interfloors = new DynamicMesh(this, sbs->GetSceneNode(), "Interfloor Container");
	columnframes = new DynamicMesh(this, sbs->GetSceneNode(), "Columnframe Container");
}

FloorManager::~FloorManager()
{
	//delete floors
	for (int i = 0; i < (int)Array.size(); i++)
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
	floor.object = new Floor(sbs, this, number);
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

	for (size_t i = 0; i < (int)Array.size(); i++)
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
	for (int i = 0; i < (int)Array.size(); i++)
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
	for (int i = 0; i < (int)Array.size(); i++)
		Array[i].object->Enabled(value);
}

ElevatorManager::ElevatorManager(Object* parent) : Object(parent)
{
	//set up SBS object
	SetValues("ElevatorManager", "Elevator Manager", true, false);

	get_result = 0;
	get_number = 0;
}

ElevatorManager::~ElevatorManager()
{
	//delete elevators
	for (int i = 0; i < (int)Array.size(); i++)
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
	elev.object = new Elevator(sbs, number);
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
	for (int i = 0; i < (int)Array.size(); i++)
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

	for (int i = 0; i < (int)Array.size(); i++)
	{
		Shaft *shaft = Array[i].object->GetShaft();

		if (value == false)
			value = shaft->ShowFullShaft;

		Array[i].object->Enabled(value);
	}
}

ShaftManager::ShaftManager(Object* parent) : Object(parent)
{
	//set up SBS object
	SetValues("ShaftManager", "Shaft Manager", true, false);

	get_result = 0;
	get_number = 0;
}

ShaftManager::~ShaftManager()
{
	//delete shafts
	for (int i = 0; i < (int)Array.size(); i++)
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
	shaft.object = new Shaft(sbs, number, CenterX, CenterZ, _startfloor, _endfloor);
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
	for (int i = 0; i < (int)Array.size(); i++)
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
	for (int i = 0; i < (int)Array.size(); i++)
		Array[i].object->EnableWholeShaft(value, true, true);
}

StairsManager::StairsManager(Object* parent) : Object(parent)
{
	//set up SBS object
	SetValues("StairsManager", "Stairs Manager", true, false);

	get_result = 0;
	get_number = 0;
}

StairsManager::~StairsManager()
{
	//delete stairs
	for (int i = 0; i < (int)Array.size(); i++)
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
	stairs.object = new Stairs(sbs, number, CenterX, CenterZ, _startfloor, _endfloor);
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
	for (int i = 0; i < (int)Array.size(); i++)
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
	for (int i = 0; i < (int)Array.size(); i++)
		Array[i].object->EnableWholeStairwell(value, true);
}

}
