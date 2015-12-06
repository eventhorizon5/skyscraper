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

	RandomGen rnd_dest(time(0) + GetNumber());
	int floor = rnd_dest.Get(sbs->GetTotalFloors() - 1) - sbs->Basements;

	GotoFloor(floor);
}

Person::~Person()
{
	if (sbs->FastDelete == false && parent_deleting == false)
		sbs->RemovePerson(this);
}

void Person::GotoFloor(int floor)
{
	Floor *floor_obj = sbs->GetFloor(current_floor);
	dest_floor = floor;

	if (!floor_obj)
		return;

	Report("Heading to floor " + ToString(floor));

	//get route to floor, as a list of elevators
	std::vector<ElevatorRoute*> elevators = sbs->GetRouteToFloor(current_floor, dest_floor, service_access);

	if (sbs->Verbose == true)
		Report("Routing table:");

	//create a new route table entry for each elevator in list
	for (int i = 0; i < (int)elevators.size(); i++)
	{
		if (sbs->Verbose == true)
			Report(ToString(i) + ": Elevator " + ToString(elevators[i]->elevator->Number) + " - floor selection " + ToString(elevators[i]->floor_selection) + " - elevator name: " + elevators[i]->elevator->Name);

		RouteEntry route_entry;
		route_entry.elevator_route = elevators[i];
		route_entry.call_made = false;
		route_entry.floor_selected = false;
		route.push_back(route_entry);
	}
}

void Person::Loop()
{
	if (route.empty() == true)
		return;

	Floor *floor_obj = sbs->GetFloor(current_floor);

	if (!floor_obj)
		return;

	Elevator *elevator = route[0].elevator_route->elevator;
	int floor_selection = route[0].elevator_route->floor_selection;

	if (!elevator)
		return;

	//if a call has not been made, press first elevator's associated call button
	if (route[0].call_made == false)
	{
		CallButton *button = floor_obj->GetCallButton(elevator->Number);

		if (button)
		{
			Report("Pressing call button for elevator " + ToString(elevator->Number));

			if (floor_selection > current_floor)
				button->Call(true);
			else
				button->Call(false);

			route[0].call_made = true;
		}
		return;
	}

	//if a call has been made, wait for an elevator to arrive to press floor button
	if (route[0].floor_selected == false)
	{
		int number = 0;
		bool direction = false;

		CallButton *button = floor_obj->GetCallButton(elevator->Number);

		if (!button)
			return;

		bool status = button->GetElevatorArrived(number, direction);
		bool direction_valid = ((dest_floor > current_floor && direction == true) || (dest_floor < current_floor && direction == false) || dest_floor == current_floor);

		if (status == true && direction_valid == true)
		{
			//if elevator has arrived at the called floor, press related floor button

			Report("Pressing elevator button for floor " + ToString(floor_selection));

			Elevator *elevator = sbs->GetElevator(number);
			route[0].floor_selected = elevator->SelectFloor(floor_selection);
		}
	}
	else
	{
		//wait for the elevator to arrive at the selected floor

		if (elevator->OnFloor == true && elevator->GetFloor() == floor_selection)
		{
			std::string floor_status;

			if (floor_selection == dest_floor)
				floor_status = "destination";
			else
				floor_status = "intermediate";

			Report("Arrived at " + floor_status + " floor " + ToString(floor_selection));
			current_floor = floor_selection;

			//erase first route entry
			delete route[0].elevator_route;
			route.erase(route.begin());
			return;
		}
	}
}

void Person::Report(const std::string &message)
{
	//general reporting function
	sbs->Report("Person " + GetName() + ": " + message);
}

bool Person::ReportError(const std::string &message)
{
	//general reporting function
	return sbs->ReportError("Person " + GetName() + ": " + message);
}

}
