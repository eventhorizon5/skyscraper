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
	this->service_access = service_access;

	RandomGen rnd_dest(time(0) + GetNumber());
	int dest_floor = rnd_dest.Get(sbs->GetTotalFloors() - 1) - sbs->Basements;

	GotoFloor(dest_floor);
}

Person::~Person()
{

}

void Person::GotoFloor(int floor)
{
	Floor *floor_obj = sbs->GetFloor(current_floor);

	if (!floor_obj)
		return;

	std::vector<Elevator*> route = sbs->GetRouteToFloor(current_floor, floor, service_access);

	if (route.empty() == false)
	{
		CallButton *button = floor_obj->GetCallButton(route[0]->Number);

		if (button)
		{
			if (floor > current_floor)
				button->Call(true);
			else
				button->Call(false);
		}
	}
}

}
