/*
	Scalable Building Simulator - Floor Indicator Object
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
#include "mesh.h"
#include "elevator.h"
#include "elevatorcar.h"
#include "profiler.h"
#include "floor.h"
#include "timer.h"
#include "texman.h"
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

FloorIndicator::FloorIndicator(Object *parent, int index, int elevator, int car, const std::string &texture_prefix, const std::string &blank_texture, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset) : Object(parent)
{
	//creates a new floor indicator at the specified position

	//set up SBS object
	SetValues("FloorIndicator", "", false);

	is_enabled = true;
	elev = elevator;
	this->car = car;
	Prefix = texture_prefix;
	Blank = blank_texture;
	off = false;

	//move object
	Move(CenterX, voffset, CenterZ);

	std::string ext;
	if (index > 0)
		ext = ":" + ToString(index + 1);
	std::string name = "Floor Indicator " + ToString(elevator) + ext;
	SetName(name);

	FloorIndicatorMesh = new MeshObject(this, name, 0, "", "", sbs->GetConfigFloat("Skyscraper.SBS.MaxSmallRenderDistance", 100));

	//exit if elevator is invalid
	if (!sbs->GetElevator(elev))
		return;

	//exit if the elevator car is invalid
	ElevatorCar *Car = sbs->GetElevator(elev)->GetCar(car);
	if (!Car)
		return;

	//exit if the car starting floor is invalid
	if (!sbs->GetFloor(Car->StartingFloor))
		return;

	std::string texture = Prefix + sbs->GetFloor(Car->StartingFloor)->ID;
	sbs->GetTextureManager()->EnableLighting(texture, false);
	std::string tmpdirection = direction;
	SetCase(tmpdirection, false);

	PolyMesh* polymesh = sbs->GetPolyMesh();
	Wall *wall = FloorIndicatorMesh->CreateWallObject("Floor Indicator");
	if (tmpdirection == "front" || tmpdirection == "back")
	{
		if (tmpdirection == "front")
			polymesh->DrawWalls(true, false, false, false, false, false);
		else
			polymesh->DrawWalls(false, true, false, false, false, false);

		polymesh->AddWallMain(wall, "Floor Indicator", texture, 0, -width / 2, 0, width / 2, 0, height, height, 0, 0, 1, 1, false);
	}
	else if (tmpdirection == "left" || tmpdirection == "right")
	{
		if (tmpdirection == "left")
			polymesh->DrawWalls(true, false, false, false, false, false);
		else
			polymesh->DrawWalls(false, true, false, false, false, false);

		polymesh->AddWallMain(wall, "Floor Indicator", texture, 0, 0, width / 2, 0, -width / 2, height, height, 0, 0, 1, 1, false);
	}
	polymesh->ResetWalls();

	flash_timer = new Timer("Flash Timer", this);

	EnableLoop(true);
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

bool FloorIndicator::Enabled(bool value)
{
	//turns indicator on/off

	if (is_enabled == value)
		return true;

	bool status = FloorIndicatorMesh->Enabled(value);
	is_enabled = value;
	return status;
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

void FloorIndicator::Off()
{
	//turn off indicator

	if (off == true)
		return;

	off = true;
	Update(true);
}

void FloorIndicator::On()
{
	//turn on indicator

	if (off == false)
		return;

	off = false;
	Update();
}

bool FloorIndicator::Loop()
{
	if (sbs->GetPower() == false)
		Off();
	else
		On();

	return true;
}

}
