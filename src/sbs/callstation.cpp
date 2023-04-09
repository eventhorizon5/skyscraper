/*
	Scalable Building Simulator - Call Station Object
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
#include "buttonpanel.h"
#include "floor.h"
#include "controller.h"
#include "indicator.h"
#include "callstation.h"

namespace SBS {

CallStation::CallStation(Object *parent, int floornum, int number) : Object(parent), Lock(this)
{
	floor = sbs->GetFloor(floornum);
	if (!floor)
		return;

	//set up SBS object
	SetValues("CallStation", "", false);

	is_enabled = true;

	std::string base = "Floor " + ToString(floornum) + ":Call Station " + ToString(number);
	SetName(base);

	//set variables
	Number = number;
	Locked = false;
	KeyID = 0;
	panel = 0;
	controller = 0;
	indicator = 0;

	if (sbs->Verbose)
		Report("Created");
}

CallStation::~CallStation()
{
	RemovePanel();

	if (indicator)
		delete indicator;
	indicator = 0;

	if (sbs->FastDelete == false)
	{
		//unregister with controller
		if (GetController())
			GetController()->UnregisterCallStation(this);

		//unregister with parent floor object
		if (parent_deleting == false)
			floor->RemoveCallStation(this);
	}
}

ButtonPanel* CallStation::CreateButtonPanel(const std::string &texture, int rows, int columns, const std::string &direction, Real buttonwidth, Real buttonheight, Real spacingX, Real spacingY, Real tw, Real th)
{
	//create a new button panel object

	if (panel)
		return 0;

	if (sbs->Verbose)
		Report("Creating button panel");

	panel = new ButtonPanel(this, 0, texture, rows, columns, direction, 0, 0, buttonwidth, buttonheight, spacingX, spacingY, 0, tw, th);
	return panel;
}

void CallStation::Enabled(bool value)
{
	//turns station on/off
	if (is_enabled == value)
		return;

	is_enabled = value;

	//enable or disable the button panel
	if (panel)
		panel->Enabled(value);

	if (sbs->Verbose)
	{
		if (value == true)
			Report("Enabled");
		else
			Report("Disabled");
	}
}

void CallStation::Report(const std::string &message)
{
	//general reporting function
	std::string msg = "Call Station " + ToString(GetFloor()) + ":" + ToString(Number) + " - " + message;
	Object::Report(msg);
}

bool CallStation::ReportError(const std::string &message)
{
	//general reporting function
	std::string msg = "Call Station " + ToString(GetFloor()) + ":" + ToString(Number) + " - " + message;
	return Object::ReportError(msg);
}

int CallStation::GetFloor()
{
	//return floor number this call button is on

	return floor->Number;
}

ButtonPanel* CallStation::GetPanel()
{
	return panel;
}

void CallStation::RemovePanel()
{
	if (panel)
	{
		panel->parent_deleting = true;
		delete panel;
	}
	panel = 0;
}

bool CallStation::SelectFloor(int floor)
{
	//check lock state
	if (IsLocked() == true)
		return ReportError("Call station is locked");

	Report("Selecting floor " + ToString(floor));

	if (GetController())
		return GetController()->RequestRoute(this, GetFloor(), floor);
	return false;
}

void CallStation::SetController(int number)
{
	//assign this call station to a controller, and register with it

	controller = number;

	if (GetController())
		GetController()->RegisterCallStation(this);
}

DispatchController* CallStation::GetController()
{
	return sbs->GetController(controller);
}

void CallStation::SetPosition(Ogre::Vector3 &position)
{
	position.y += sbs->GetFloor(GetFloor())->GetBase();
	Object::SetPosition(position);
}

bool CallStation::ServicesElevator(int elevator)
{
	if (GetController())
		return GetController()->ServicesElevator(elevator);
	return false;
}

int CallStation::GetElevatorArrived(int starting_floor, int destination_floor)
{
	//return the number of the elevator that has arrived, for the specified route
	//return 0 if no elevator has arrived yet

	if (!GetController())
		return 0;

	return GetController()->GetElevatorArrived(starting_floor, destination_floor);
}

bool CallStation::FireService(int value)
{
	if (GetController())
		return GetController()->FireService(value);
	return false;
}

Indicator* CallStation::AddIndicator(const std::string &texture_prefix, const std::string &blank_texture, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, Real timer_duration)
{
	if (indicator)
		return 0;

	indicator = new Indicator(this, texture_prefix, blank_texture, direction, CenterX, CenterZ, width, height, voffset, timer_duration);

	return indicator;
}

void CallStation::UpdateIndicator(std::string &text)
{
	if (indicator)
		indicator->Update(text);
}

}
