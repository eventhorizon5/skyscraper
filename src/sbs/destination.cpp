/*
	Scalable Building Simulator - Destination Controller Object
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
#include "timer.h"
#include "elevator.h"
#include "elevatorcar.h"
#include "destination.h"

namespace SBS {

class DestinationController::Timer : public TimerObject
{
public:
	DestinationController *parent;
	Timer(const std::string &name, DestinationController *parent) : TimerObject(parent, name)
	{
		this->parent = parent;
	}
	virtual void Notify();
};

DestinationController::DestinationController(Object *parent, int number, std::vector<int> &elevators, int elevator_range) : Object(parent)
{
	//create a destination controller object

	//set up SBS object
	SetValues("DestinationController", "", false);

	ElevatorRange = elevator_range;
	Number = number;

	//add elevators
	Elevators.resize(elevators.size());
	for (size_t i = 0; i < elevators.size(); i++)
	{
		Map elev;
		elev.number = elevators[i];
		elev.arrived = false;
		elev.arrival_floor = 0;
		Elevators[i] = elev;
	}

	//create timer
	timer = new Timer("Destination Timer", this);
	timer->Start(1000);

	Report("Created");
}

DestinationController::~DestinationController()
{
	if (timer)
	{
		timer->parent_deleting = true;
		delete timer;
	}
	timer = 0;

}

void DestinationController::Timer::Notify()
{
	parent->Loop();
}

void DestinationController::Loop()
{
	for (int i = 0; i < Elevators.size(); i++)
	{
		if (Elevators[i].arrived == true)
		{
			for (int j = 0; j < Requests.size(); j++)
			{
				int direction = 0;
				if (Requests[j].starting_floor < Requests[j].destination_floor)
					direction = 1;
				else
					direction = -1;
				if (Requests[j].starting_floor == Elevators[i].arrival_floor)
					DispatchElevator(Elevators[i].number, Requests[j].destination_floor, direction);
			}
		}
	}
}

bool DestinationController::RequestRoute(int starting_floor, int destination_floor)
{
	//request a destination dispatch route

	Report("Requesting route from origin floor " + ToString(starting_floor) + " to destination floor " + ToString(destination_floor));

	//check to make sure elevator objects are valid
	bool isvalid = false;
	for (size_t i = 0; i < Elevators.size(); i++)
	{
		Elevator *elevator = sbs->GetElevator(Elevators[i].number);
		if (elevator)
		{
			isvalid = true;
			break;
		}
	}
	if (isvalid == false)
		return ReportError("No valid elevators found");

	//get closest elevator
	int closest = FindClosestElevator(starting_floor, destination_floor);

	//if none found, exit
	if (closest == -1)
		return false;

	//return the letter of the elevator
	Elevator* elevator = sbs->GetElevator(Elevators[closest].number);
	//get letter

	Report("Using elevator " + ToString(elevator->Number));

	ElevatorCar *car = elevator->GetCarForFloor(starting_floor);
	if (!car)
		return false;

	int direction = 0;
	if (destination_floor > starting_floor)
		direction = 1;
	else
		direction = -1;

	//add a route entry to this floor
	elevator->AddRoute(starting_floor, direction, 1);
	//ActiveElevator = elevator->Number;

	car->DestinationFloor = destination_floor;
	car->UseDestination = true;

	return true;
}

bool DestinationController::AddElevator(int elevator)
{
	//add an elevator to this controller

	for (size_t i = 0; i < Elevators.size(); i++)
	{
		if (Elevators[i].number == elevator)
			return false;
	}

	Map newelevator;
	newelevator.number = elevator;
	newelevator.arrived = false;
	newelevator.arrival_floor = 0;

	Elevators.push_back(newelevator);
	return true;
}

bool DestinationController::RemoveElevator(int elevator)
{
	//remove an elevator from this controller

	for (size_t i = 0; i < Elevators.size(); i++)
	{
		if (Elevators[i].number == elevator)
		{
			Elevators.erase(Elevators.begin() + i);
			return true;
		}
	}
	return false;
}

bool DestinationController::ServicesElevator(int elevator)
{
	//return true if this controller services the specified elevator
	for (size_t i = 0; i < Elevators.size(); i++)
	{
		if (Elevators[i].number == elevator)
		{
			if (sbs->Verbose)
				Report("Services elevator " + ToString(elevator));
			return true;
		}
	}
	return false;
}

int DestinationController::FindClosestElevator(int starting_floor, int destination_floor)
{
	//initialize values
	int closest = 0;
	int closest_elev = 0;
	bool check = false;
	int errors = 0;
	int result = 0;
	bool direction = 0;

	if (destination_floor > starting_floor)
		direction = 1;
	else
		direction = -1;

	int ActiveElevator = 0;

	//if an elevator is already servicing the call, set recheck to true
	bool recheck = false;
	if (ActiveElevator > 0)
		recheck = true;

	int count = (int)Elevators.size();

	//exit if no elevators are associated
	if (count == 0)
		return -1;

	//search through elevator list
	if (sbs->Verbose && count > 1 && recheck == false)
		Report("Finding nearest available elevator...");

	//check each elevator associated with this controller to find the closest available one
	for (int i = 0; i < count; i++)
	{
		Elevator *elevator = sbs->GetElevator(Elevators[i].number);
		if (elevator)
		{
			ElevatorCar *car = elevator->GetCarForFloor(starting_floor);
			if (car)
			{
				if (sbs->Verbose && recheck == false)
					Report("Checking elevator " + ToString(elevator->Number) + " car " + ToString(car->Number));

				//if elevator is closer than the previously checked one or we're starting the checks
				if (abs(car->GetFloor() - starting_floor) < closest || check == false)
				{
					//see if elevator is available for the call
					if (recheck == true && elevator->Number == ActiveElevator)
					{
						if (elevator->Error == true)
						{
							Report("Failing active elevator " + ToString(elevator->Number) + " due to movement error");
							result = 2; //fail active elevator if it had a movement processing error
							ActiveElevator = 0;
						}
						else
							result = 1; //if rechecking elevators, consider the active one
					}
					else
						result = elevator->AvailableForCall(starting_floor, direction, !recheck);

					if (result == 1) //available
					{
						if (sbs->Verbose && count > 1 && recheck == false)
							Report("Marking - closest so far");
						closest = abs(car->GetFloor() - starting_floor);
						closest_elev = i;
						check = true;
					}
					else if (result == 2) //elevator won't service the call
						errors++;
				}
			}
		}
	}

	if (errors == count)
	{
		//exit if all elevators are in a service mode or return errors

		std::string item;
		if (count > 1)
			item = "All elevators";
		else
			item = "Elevator";

		Report(item + " unavailable due to service modes or errors");

		//turn off lights
		/*if (direction == 1)
			UpLight(false);
		else
			DownLight(false);*/

		return -1;
	}

	if (check == false)
	{
		//exit if no elevator found

		if (sbs->Verbose)
			Report("No elevator found");
		return -1;
	}

	return closest_elev;
}

void DestinationController::Report(const std::string &message)
{
	//general reporting function
	std::string msg = "Destination Controller " + ToString(Number) + ": " + message;
	Object::Report(msg);
}

bool DestinationController::ReportError(const std::string &message)
{
	//general reporting function
	std::string msg = "Destination Controller " + ToString(Number) + ": " + message;
	return Object::ReportError(msg);
}

void DestinationController::ElevatorArrived(int number, int floor)
{
	//notify controller about an elevator arrival

	for (int i = 0; i < Elevators.size(); i++)
	{
		if (Elevators[i].number == number && Elevators[i].arrived == true)
			return;

		Elevators[i].arrived = true;
		Elevators[i].arrival_floor = floor;
	}
}

void DestinationController::DispatchElevator(int number, int destination_floor, int direction)
{
	Elevator *elevator = sbs->GetElevator(number);

	if (elevator)
	{
		elevator->AddRoute(destination_floor, direction, 2);
	}
}

}
