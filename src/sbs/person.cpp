/* $Id$ */

/*
	Scalable Building Simulator - Person Object
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
#include "floor.h"
#include "elevator.h"
#include "callbutton.h"
#include "unix.h"
#include "person.h"

namespace SBS {

Person::Person(Object *parent, const std::string &name, int floor, bool service_access) : Object(parent)
{
	//creates a person object, used for random traffic simulations

	//set up SBS object
	SetValues("Person", name, false);

	current_floor = floor;
	dest_floor = 0;
	this->service_access = service_access;
	RandomProbability = sbs->GetConfigInt("Skyscraper.SBS.Person.RandomProbability", 20);
	RandomFrequency = sbs->GetConfigFloat("Skyscraper.SBS.Person.RandomFrequency", 5);

	//initialize random number generators
	rnd_time = new RandomGen(time(0) + GetNumber());
	rnd_dest = new RandomGen(time(0) + GetNumber() + 1);

	//create timer
	random_timer = new Timer("Random Timer", this);

	SetFloor(floor);
}

Person::~Person()
{
	//delete timer
	if (random_timer)
	{
		random_timer->parent_deleting = true;
		delete random_timer;
	}
	random_timer = 0;

	//delete routes
	Stop();

	//delete random number generators
	if (rnd_time)
		delete rnd_time;
	rnd_time = 0;

	if (rnd_dest)
		delete rnd_dest;
	rnd_dest = 0;

	if (sbs->FastDelete == false && parent_deleting == false)
		sbs->RemovePerson(this);
}

void Person::EnableRandomActivity(bool value)
{
	//enable or disable timer
	if (value == true)
	{
		random_timer->Start(int(RandomFrequency * 1000), false);
		Report("Random activity enabled");
	}
	else
	{
		random_timer->Stop();
		Report("Random activity disabled");
	}
}

int Person::GetRandomFloor()
{
	return (int)rnd_dest->Get(sbs->GetTotalFloors() - 1) - sbs->Basements;
}

void Person::GotoFloor(int floor)
{
	//exit if a route is already active
	if (IsRouteActive() == true)
		return;

	Floor *origfloor = sbs->GetFloor(current_floor);
	Floor *newfloor = sbs->GetFloor(floor);

	//verify floors
	if (!origfloor || !newfloor)
		return;

	//make sure we're going to a different floor
	if (floor == current_floor)
		return;

	dest_floor = floor;

	Report("Heading to floor " + newfloor->ID);

	//get route to floor, as a list of elevators
	std::vector<ElevatorRoute*> elevators = sbs->GetRouteToFloor(current_floor, dest_floor, service_access);

	if (elevators.empty() == true)
	{
		Report("No route found to floor " + newfloor->ID);
		return;
	}

	if (sbs->Verbose == true)
		Report("Routing table:");

	//create a new route table entry for each elevator in list
	for (int i = 0; i < (int)elevators.size(); i++)
	{
		if (sbs->Verbose == true)
			Report(ToString(i) + ": Elevator " + ToString(elevators[i]->elevator->Number) + " - floor selection " + ToString(elevators[i]->floor_selection) + " - elevator name: " + elevators[i]->elevator->Name);

		RouteEntry route_entry;
		route_entry.elevator_route = elevators[i];
		route_entry.callbutton = 0;
		route_entry.call_made = 0;
		route_entry.floor_selected = false;
		route.push_back(route_entry);
	}
}

void Person::Loop()
{
	ProcessRoute();
}

void Person::ProcessRoute()
{
	if (IsRouteActive() == false)
		return;

	Floor *floor_obj = sbs->GetFloor(current_floor);

	if (!floor_obj)
		return;

	Elevator *elevator = route[0].elevator_route->elevator;
	int floor_selection = route[0].elevator_route->floor_selection;

	if (!elevator)
		return;

	//if a call has not been made, press first elevator's associated call button
	if (route[0].call_made == 0)
	{
		route[0].callbutton = floor_obj->GetCallButton(elevator->Number);
		CallButton *button = route[0].callbutton;

		bool result = false;

		if (button)
		{
			Report("Pressing call button for elevator " + ToString(elevator->Number));

			if (floor_selection > current_floor)
			{
				result = button->Call(true);
				route[0].call_made = 1;
			}
			else
			{
				result = button->Call(false);
				route[0].call_made = -1;
			}
		}

		//stop route if call can't be made
		if (result == false)
		{
			Report("Can't press call button for elevator " +  ToString(elevator->Number));
			Stop();
		}

		return;
	}

	//if a call has been made, wait for an elevator to arrive to press floor button
	if (route[0].floor_selected == false)
	{
		CallButton *button = route[0].callbutton;

		if (!button)
			return;

		bool direction = (floor_selection > current_floor);
		int number = button->GetElevatorArrived(direction);

		if (number > 0)
		{
			//if elevator has arrived at the called floor, press related floor button

			Elevator *elevator = sbs->GetElevator(number);
			if (elevator)
			{
				//have elevator route use arrived elevator
				route[0].elevator_route->elevator = elevator;

				//wait for elevator doors to open before pressing button
				if (elevator->AreDoorsOpen() == true)
				{
					Floor *floor = sbs->GetFloor(floor_selection);

					if (!floor)
						return;

					Report("Pressing elevator button for floor " + floor->ID);

					Control *control = elevator->GetFloorButton(floor_selection);

					if (control)
					{
						if (control->IsLocked() == false)
						{
							//press floor button
							control->Press();
							route[0].floor_selected = true;
							return;
						}
					}

					//stop route if floor button is locked, or does not exist
					Report("Can't press elevator button for floor " + floor->ID);
					Stop();
					return;
				}
			}
		}
		else
		{
			//if call has become invalid, stop route
			if ((direction == true && button->GetUpStatus() == false) ||
					(direction == false && button->GetDownStatus() == false))
			{
				Stop();
				return;
			}

		}
	}
	else
	{
		//wait for the elevator to arrive at the selected floor

		if (elevator->OnFloor == true && elevator->GetFloor() == floor_selection && elevator->AreDoorsOpen() == true)
		{
			std::string floor_status;

			if (floor_selection == dest_floor)
				floor_status = "destination";
			else if (route[0].call_made == 2)
				floor_status = "recall";
			else
				floor_status = "intermediate";

			Floor *floor = sbs->GetFloor(floor_selection);
			if (!floor)
				return;

			Report("Arrived at " + floor_status + " floor " + floor->ID);
			current_floor = floor_selection;

			if (elevator->FireServicePhase1 != 1)
			{
				//erase first route entry
				delete route[0].elevator_route;
				route.erase(route.begin());
			}
			else
			{
				//if fire phase 1 is enabled, stop all routes and exit
				Stop();
			}
			return;
		}
		else if (elevator->InServiceMode() == true)
		{
			if (elevator->FireServicePhase1 == 1)
			{
				//if fire phase 1 mode is enabled, change floor selection to recall floor
				//in order to exit the elevator at the recall floor
				if (floor_selection != elevator->GetActiveRecallFloor())
				{
					route[0].elevator_route->floor_selection = elevator->GetActiveRecallFloor();
					route[0].call_made = 2;
				}
			}
			else
			{
				//otherwise exit at current floor and try another elevator
				current_floor = elevator->GetFloor();
				route[0].floor_selected = false;
				route[0].call_made = 0;
			}
			return;
		}
		else if (elevator->IsMoving == true)
		{
			if (current_floor != elevator->GetFloor())
				current_floor = elevator->GetFloor();
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

void Person::Timer::Notify()
{
	//timer for random destination activity

	if (parent->IsRouteActive() == true)
		return;

	int result = (int)parent->rnd_time->Get(parent->RandomProbability - 1);
	if (result == 0)
		parent->GotoFloor(parent->GetRandomFloor());
}

void Person::SetFloor(int value)
{
	if (IsRouteActive() == true)
		return;

	if (!sbs->GetFloor(value))
		return;

	current_floor = value;
	Report("On floor " +  ToString(current_floor));
}

void Person::Stop()
{
	//stop route if active

	//delete routes
	if (route.empty() == false)
	{
		for (int i = 0; i < (int)route.size(); i++)
		{
			delete route[i].elevator_route;
		}
	}
	route.clear();
}

std::string Person::GetStatus()
{
	//return a string describing the person's status

	if (IsRouteActive() == false)
	{
		Floor *floor = sbs->GetFloor(current_floor);
		if (!floor)
			return "";

		return "Idle on floor " + floor->ID;
	}

	Elevator *elevator = route[0].elevator_route->elevator;
	int floor_selection = route[0].elevator_route->floor_selection;

	Floor *floor = sbs->GetFloor(floor_selection);
	if (!floor)
		return "";

	if (route[0].floor_selected == true && elevator)
	{
		if (elevator->AreDoorsOpen() == true)
			return "Pressed " + floor->ID + " in elevator " + ToString(elevator->Number);
		else
		{
			if (route[0].call_made != 2)
			{
				std::string direction;
				if (route[0].call_made == 1)
					direction = "Up";
				else
					direction = "Down";
				return direction + " to floor " + floor->ID + " in elevator " + ToString(elevator->Number);
			}
			else
				return "Proceeding to recall floor";
		}
	}

	if (route[0].call_made == 1)
		return "Call button Up pressed";

	if (route[0].call_made == -1)
		return "Call button Down pressed";

	return "";
}

}
