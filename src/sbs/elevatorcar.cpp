/* $Id$ */

/*
Scalable Building Simulator - Elevator Car Object
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
#include "mesh.h"
#include "elevator.h"
#include "control.h"
#include "trigger.h"
#include "sound.h"
#include "model.h"
#include "light.h"
#include "door.h"
#include "buttonpanel.h"
#include "directional.h"
#include "floorindicator.h"
#include "elevatorcar.h"

namespace SBS {

ElevatorCar::ElevatorCar(Elevator *parent, int number) : Object(parent)
{
	//set up SBS object
	SetValues("ElevatorCar", "", false);

	this->parent = parent;
	this->number = number;
	carsound = 0;
	idlesound = 0;
	alarm = 0;
	floorbeep = 0;
	doorhold_direction = 0;
	doorhold_whichdoors = 0;
	doorhold_floor = 0;
	doorhold_manual = 0;
	announcesnd = 0;
	musicsound = 0;
	DirMessageSound = false;
	DoorMessageSound = false;
	Fan = true;
	MusicPosition = 0;
	Height = 0;
	HeightSet = false;

	std::string name = "Car " + ToString(number);
	SetName(name);

	Mesh = new MeshObject(this, name);

	if (sbs->Verbose)
		parent->Report("created car " + ToString(number));
}

ElevatorCar::~ElevatorCar()
{
	if (sbs->Verbose)
		parent->Report("deleting car objects");

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

	//delete directional indicators
	if (sbs->Verbose)
		parent->Report("deleting car interior directional indicators");

	for (size_t i = 0; i < DirIndicatorArray.size(); i++)
	{
		if (DirIndicatorArray[i])
		{
			DirIndicatorArray[i]->parent_deleting = true;
			delete DirIndicatorArray[i];
		}
		DirIndicatorArray[i] = 0;
	}

	//delete doors
	if (sbs->Verbose)
		parent->Report("deleting car doors");

	if (DoorArray.size() > 0)
	{
		for (size_t i = 0; i < DoorArray.size(); i++)
		{
			if (DoorArray[i])
			{
				DoorArray[i]->parent_deleting = true;
				delete DoorArray[i];
			}
			DoorArray[i] = 0;
		}
	}

	//delete floor indicators
	if (sbs->Verbose)
		parent->Report("deleting car floor indicators");

	for (size_t i = 0; i < FloorIndicatorArray.size(); i++)
	{
		if (FloorIndicatorArray[i])
		{
			FloorIndicatorArray[i]->parent_deleting = true;
			delete FloorIndicatorArray[i];
		}
		FloorIndicatorArray[i] = 0;
	}

	//delete panels
	if (sbs->Verbose)
		parent->Report("deleting car button panels");

	for (size_t i = 0; i < PanelArray.size(); i++)
	{
		if (PanelArray[i])
		{
			PanelArray[i]->parent_deleting = true;
			delete PanelArray[i];
		}
		PanelArray[i] = 0;
	}

	//delete doors
	if (sbs->Verbose)
		parent->Report("deleting car standard doors");

	for (size_t i = 0; i < StdDoorArray.size(); i++)
	{
		if (StdDoorArray[i])
		{
			StdDoorArray[i]->parent_deleting = true;
			delete StdDoorArray[i];
		}
		StdDoorArray[i] = 0;
	}

	if (sbs->Verbose)
		parent->Report("deleting car sounds");
	if (carsound)
	{
		carsound->parent_deleting = true;
		delete carsound;
	}
	carsound = 0;
	if (alarm)
	{
		alarm->parent_deleting = true;
		delete alarm;
	}
	alarm = 0;
	if (floorbeep)
	{
		floorbeep->parent_deleting = true;
		delete floorbeep;
	}
	floorbeep = 0;

	if (idlesound)
	{
		idlesound->parent_deleting = true;
		delete idlesound;
	}
	idlesound = 0;
	if (announcesnd)
	{
		announcesnd->parent_deleting = true;
		delete announcesnd;
	}
	announcesnd = 0;
	if (musicsound)
	{
		musicsound->parent_deleting = true;
		delete musicsound;
	}
	musicsound = 0;

	for (size_t i = 0; i < sounds.size(); i++)
	{
		if (sounds[i])
		{
			sounds[i]->parent_deleting = true;
			delete sounds[i];
		}
		sounds[i] = 0;
	}

	//delete mesh object
	if (Mesh)
	{
		Mesh->parent_deleting = true;
		delete Mesh;
	}
	Mesh = 0;
}

Elevator* ElevatorCar::GetElevator()
{
	return parent;
}

void ElevatorCar::Report(const std::string &message)
{
	//general reporting function

	if (parent->GetCarCount() > 1)
		parent->Report("Car " + ToString(number) + ": " + message);
	else
		parent->Report(message);
}

bool ElevatorCar::ReportError(const std::string &message)
{
	//general reporting function

	if (parent->GetCarCount() > 1)
		return parent->ReportError("Car " + ToString(number) + ": " + message);

	return parent->ReportError(message);
}

WallObject* ElevatorCar::AddWall(const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th)
{
	//Adds a wall with the specified dimensions

	WallObject *wall = Mesh->CreateWallObject(name);
	sbs->AddWallMain(wall, name, texture, thickness, x1, z1, x2, z2, height1, height2, voffset1, voffset2, tw, th, true);
	return wall;
}

WallObject* ElevatorCar::AddFloor(const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, bool reverse_axis, bool texture_direction, float tw, float th, bool legacy_behavior)
{
	//Adds a floor with the specified dimensions and vertical offset

	WallObject *wall = Mesh->CreateWallObject(name);
	sbs->AddFloorMain(wall, name, texture, thickness, x1, z1, x2, z2, voffset1, voffset2, reverse_axis, texture_direction, tw, th, true, legacy_behavior);
	return wall;
}

FloorIndicator* ElevatorCar::AddFloorIndicator(const std::string &texture_prefix, const std::string &direction, float CenterX, float CenterZ, float width, float height, float voffset)
{
	//Creates a floor indicator at the specified location

	FloorIndicator* indicator = new FloorIndicator(parent, parent->Number, texture_prefix, direction, CenterX, CenterZ, width, height, voffset);
	FloorIndicatorArray.push_back(indicator);
	return indicator;
}

ButtonPanel* ElevatorCar::CreateButtonPanel(const std::string &texture, int rows, int columns, const std::string &direction, float CenterX, float CenterZ, float buttonwidth, float buttonheight, float spacingX, float spacingY, float voffset, float tw, float th)
{
	//create a new button panel object and store the pointer

	int index = (int)PanelArray.size();

	if (sbs->Verbose)
		Report("creating button panel " + ToString(index + 1));

	ButtonPanel* panel = new ButtonPanel(parent, index + 1, texture, rows, columns, direction, CenterX, CenterZ, buttonwidth, buttonheight, spacingX, spacingY, voffset, tw, th);
	PanelArray.push_back(panel);
	return panel;
}

void ElevatorCar::DumpServicedFloors()
{
	//dump serviced floors list

	if (parent->GetCarCount() > 1)
		sbs->Report("\n--- Elevator " + ToString(parent->Number) + "'s Serviced Floors ---\n");
	else
		sbs->Report("\n--- Elevator " + ToString(parent->Number) + " Car " + ToString(number) + "'s Serviced Floors ---\n");

	for (size_t i = 0; i < ServicedFloors.size(); i++)
		sbs->Report(ToString((int)i) + " - " + ToString(ServicedFloors[i]));
	sbs->Report("");
}

bool ElevatorCar::AddServicedFloor(int number)
{
	if (sbs->Verbose)
		Report("adding serviced floor " + ToString(number));

	//check if floor is outside valid floor range
	if (sbs->IsValidFloor(number) == false)
		return ReportError("AddServicedFloor: Invalid floor " + ToString(number));

	if (IsServicedFloor(number) == false)
	{
		ServicedFloors.push_back(number);
		std::sort(ServicedFloors.begin(), ServicedFloors.end());
	}
	return true;
}

void ElevatorCar::RemoveServicedFloor(int number)
{
	if (sbs->Verbose)
		Report("removing serviced floor " + ToString(number));
	if (IsServicedFloor(number) == true)
	{
		int index = GetFloorIndex(number);
		if (index > -1)
			ServicedFloors.erase(ServicedFloors.begin() + index);
	}
}

bool ElevatorCar::IsServicedFloor(int floor, bool report)
{
	//returns true if floor is in serviced floor list, otherwise false

	int index = GetFloorIndex(floor);
	if (index == -1)
	{
		if (sbs->Verbose && report == true)
			Report("Floor " + ToString(floor) + " is not a serviced floor");
		return false;
	}
	else
	{
		if (sbs->Verbose && report == true)
			Report("Floor " + ToString(floor) + " is a serviced floor");
		return true;
	}
}

int ElevatorCar::GetFloorIndex(int floor)
{
	//return array index of the specified floor

	for (size_t i = 0; i < ServicedFloors.size(); i++)
	{
		if (ServicedFloors[i] == floor)
			return (int)i;
	}

	return -1;
}

int ElevatorCar::GetServicedFloorCount()
{
	//return number of serviced floors
	return (int)ServicedFloors.size();
}

int ElevatorCar::GetServicedFloor(int index)
{
	//get a specific serviced floor
	if (index >= 0 && index < (int)ServicedFloors.size())
		return ServicedFloors[index];
	return 0;
}

}
