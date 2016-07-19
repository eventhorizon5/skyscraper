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

ElevatorCar::ElevatorCar(Elevator *parent, int number) : ObjectBase(parent)
{
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

	std::string name = parent->Name + ":Car " + ToString(number);
	SetName(name);

	Mesh = new MeshObject(parent, name);

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

}
