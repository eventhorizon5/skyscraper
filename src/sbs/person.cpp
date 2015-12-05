/* $Id$ */

/*
	Scalable Building Simulator - Person Object
	The Skyscraper Project - Version 1.10 Alpha
	Copyright (C)2004-2015 Ryan Thoryk
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
#include "random.h"
#include "sbs.h"
#include "unix.h"
#include "person.h"

namespace SBS {

Person::Person(Object *parent, const std::string &name, bool service_access) : Object(parent)
{
	//creates a person object, used for random traffic simulations

	//set up SBS object
	SetValues("Person", name, false);

	current_floor = 0;
	dest_floor = 0;
	this->service_access = service_access;
	call_made = false;

	RandomGen rnd_dest(time(0) + GetNumber());
	int floor = rnd_dest.Get(sbs->GetTotalFloors() - 1) - sbs->Basements;

	GotoFloor(floor);
}

Person::~Person()
{

}

void Person::GotoFloor(int floor)
{
	Floor *floor_obj = sbs->GetFloor(current_floor);
	dest_floor = floor;

	if (!floor_obj)
		return;

	route = sbs->GetRouteToFloor(current_floor, dest_floor, service_access);

	if (route.empty() == false)
	{
		CallButton *button = floor_obj->GetCallButton(route[0]->Number);

		if (button)
		{
			if (dest_floor > current_floor)
				button->Call(true);
			else
				button->Call(false);

			call_made = true;
		}
	}
}

void Person::Loop()
{
	if (route.empty() == true)
		return;

	Floor *floor_obj = sbs->GetFloor(current_floor);

	if (!floor_obj)
		return;

	CallButton *button = floor_obj->GetCallButton(route[0]->Number);

	if (call_made == true && button)
	{
		int number = 0;
		bool direction = false;
		bool status = button->GetElevatorArrived(number, direction);

		bool direction_valid = ((dest_floor > current_floor && direction == true) || (dest_floor < current_floor && direction == false) || dest_floor == current_floor);

		if (status == true && direction_valid == true)
		{
			//if elevator has arrived at the called floor, press related floor button

			call_made = false;
			Elevator *elevator = sbs->GetElevator(number);
			bool result = elevator->SelectFloor(dest_floor);

			//erase first route entry
			route.erase(route.begin());
		}
	}
}

}
