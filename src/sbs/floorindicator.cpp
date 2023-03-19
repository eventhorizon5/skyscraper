/*
	Scalable Building Simulator - Floor Indicator Object
	The Skyscraper Project - Version 1.12 Alpha
	Copyright (C)2004-2023 Ryan Thoryk
	https://www.skyscrapersim.net
	https://sourceforge.net/projects/skyscraper/
	Contact - ryan@thoryk.com

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
#include "elevatorcar.h"
#include "profiler.h"
#include "floor.h"
#include "timer.h"
#include "texture.h"
#include "floorindicator.h"

namespace SBS {

class FloorIndicator::Timer : public TimerObject
{
public:
	FloorIndicator *indicator;
	bool on;
	Timer(const std::string &name, FloorIndicator *parent) : TimerObject(parent, name)
	{
		indicator = parent;
		on = false;
	}
	virtual void Notify();
};

FloorIndicator::FloorIndicator(Object *parent, int elevator, int car, const std::string &texture_prefix, const std::string &blank_texture, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset) : Object(parent)
{
	//creates a new floor indicator at the specified position

	//set up SBS object
	SetValues("FloorIndicator", "", false);

	is_enabled = true;
	elev = elevator;
	this->car = car;
	Prefix = texture_prefix;
	Blank = blank_texture;

	//move object
	Move(CenterX, voffset, CenterZ);

	std::string name = "Floor Indicator " + ToString(elevator);
	SetName(name);

	FloorIndicatorMesh = new MeshObject(this, name, 0, "", sbs->GetConfigFloat("Skyscraper.SBS.MaxSmallRenderDistance", 100));

	//exit if elevator is invalid
	if (!sbs->GetElevator(elev))
		return;

	//exit if the elevator car is invalid
	ElevatorCar *Car = sbs->GetElevator(elev)->GetCar(car);
	if (!Car)
		return;

	std::string texture = Prefix + sbs->GetFloor(Car->StartingFloor)->ID;
	sbs->GetTextureManager()->EnableLighting(texture, false);
	std::string tmpdirection = direction;
	SetCase(tmpdirection, false);

	if (tmpdirection == "front" || tmpdirection == "back")
	{
		if (tmpdirection == "front")
			sbs->DrawWalls(true, false, false, false, false, false);
		else
			sbs->DrawWalls(false, true, false, false, false, false);

		sbs->AddWallMain(this, FloorIndicatorMesh, "Floor Indicator", texture, 0, -width / 2, 0, width / 2, 0, height, height, 0, 0, 1, 1, false);
	}
	if (tmpdirection == "left" || tmpdirection == "right")
	{
		if (tmpdirection == "left")
			sbs->DrawWalls(true, false, false, false, false, false);
		else
			sbs->DrawWalls(false, true, false, false, false, false);

		sbs->AddWallMain(this, FloorIndicatorMesh, "Floor Indicator", texture, 0, 0, width / 2, 0, -width / 2, height, height, 0, 0, 1, 1, false);
	}
	sbs->ResetWalls();

	flash_timer = new Timer("Flash Timer", this);
}

FloorIndicator::~FloorIndicator()
{
	//delete timer
	if (flash_timer)
		delete flash_timer;
	flash_timer = 0;

	if (FloorIndicatorMesh)
	{
		FloorIndicatorMesh->parent_deleting = true;
		delete FloorIndicatorMesh;
	}
	FloorIndicatorMesh = 0;

	//unregister from parent
	if (sbs->FastDelete == false && parent_deleting == false)
	{
		std::string type = GetParent()->GetType();

		if (type == "ElevatorCar")
			static_cast<ElevatorCar*>(GetParent())->RemoveFloorIndicator(this);
		else if (type == "Floor")
			static_cast<Floor*>(GetParent())->RemoveFloorIndicator(this);
	}
}

void FloorIndicator::Enabled(bool value)
{
	//turns indicator on/off

	if (is_enabled == value)
		return;

	FloorIndicatorMesh->Enabled(value);
	is_enabled = value;
}

void FloorIndicator::Update(bool blank)
{
	//update indicator display with elevator's current floor identifier

	SBS_PROFILE("FloorIndicator::Update");

	if (blank == true && Blank != "")
	{
		FloorIndicatorMesh->ChangeTexture(Blank);
		return;
	}

	Elevator *elevator = sbs->GetElevator(elev);

	if (!elevator)
		return;

	ElevatorCar *car = elevator->GetCar(this->car);

	if (!car)
		return;

	std::string texture = car->GetFloorDisplay();

	//don't update texture if no value
	if (texture == "")
		return;

	texture.insert(0, Prefix);

	FloorIndicatorMesh->ChangeTexture(texture);
	sbs->GetTextureManager()->EnableLighting(texture, false);
}

void FloorIndicator::Flash(bool enabled)
{
	if (enabled == true)
		flash_timer->Start(500, false);
	else
	{
		flash_timer->Stop();
		Update(false); //refresh indicator
	}
}

void FloorIndicator::Timer::Notify()
{
	if (on == false)
	{
		indicator->Update(false);
		on = true;
	}
	else
	{
		indicator->Update(true);
		on = false;
	}

}

}
