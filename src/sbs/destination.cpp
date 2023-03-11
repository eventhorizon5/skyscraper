/*
	Scalable Building Simulator - Destination Dispatch Controller
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
#include "floor.h"
#include "elevator.h"
#include "elevatorcar.h"
#include "timer.h"
#include "destination.h"

namespace SBS {

class Destination::Timer : public TimerObject
{
public:
	Destination *parent;
	Timer(const std::string &name, Destination *parent) : TimerObject(parent, name)
	{
		this->parent = parent;
	}
	virtual void Notify();
};

Destination::Destination(Object *parent, std::vector<int> &elevators, int floornum, int number) : Object(parent), Lock(this)
{
	//create a destination dispatch controller

	floor = sbs->GetFloor(floornum);
	if (!floor)
		return;

	//set up SBS object
	SetValues("Destination", "", false);

	is_enabled = true;
	Elevators.resize(elevators.size());
	for (size_t i = 0; i < elevators.size(); i++)
		Elevators[i] = elevators[i];

	Processed = false;
	Status = false;
	ActiveElevator = 0;
	elevator_arrived = 0;
	DestinationFloor = 0;

	//create object mesh
	std::string base = "Floor " + ToString(floornum) + ":Destination Controller " + ToString(number);
	SetName(base);

	//set variables
	Number = number;
	Locked = false;
	KeyID = 0;

	//determine floor range of associated elevators
	int bottomfloor = 0;
	int topfloor = 0;
	bool firstrun = true;

	for (size_t i = 0; i < Elevators.size(); i++)
	{
		Elevator *elev = sbs->GetElevator(Elevators[i]);
		if (elev)
		{
			ElevatorCar *car = elev->GetCarForFloor(floornum);
			if (car)
			{
				int tmpbottom = car->GetBottomFloor();
				int tmptop = car->GetTopFloor();
				if (tmpbottom < bottomfloor || firstrun == true)
					bottomfloor = tmpbottom;
				if (tmptop > topfloor || firstrun == true)
					topfloor = tmptop;
				firstrun = false;
			}
		}
	}

	//create timer
	timer = new Timer("Call Timer", this);

	if (sbs->Verbose)
		Report("Created");
}

Destination::~Destination()
{
	if (timer)
	{
		timer->parent_deleting = true;
		delete timer;
	}
	timer = 0;

	//unregister with parent floor object
	/*if (sbs->FastDelete == false)
	{
		if (parent_deleting == false)
			floor->RemoveCallButton(this);
	}*/
}

void Destination::Enabled(bool value)
{
	//turns controller on/off
	if (is_enabled == value)
		return;

	is_enabled = value;

	if (sbs->Verbose)
	{
		if (value == true)
			Report("Enabled");
		else
			Report("Disabled");
	}
}

bool Destination::Call(int floor)
{
	//calls the closest elevator in the elevator array list to the current floor,
	//and also depending on the destination floor

	//check lock state
	if (IsLocked() == true)
		return ReportError("Controller is locked");

	//exit if direction not possible
	/*if (direction == true && UpExists == false)
		return false;
	if (direction == false && DownExists == false)
		return false;*/

	//exit if call has already been made
	if (timer->IsRunning() == true)
	{
		if (DestinationFloor == floor && Processed == true)
		{
			Report("Call has already been made");
			return true;
		}
	}

	//check to make sure elevator objects are valid
	bool isvalid = false;
	for (size_t i = 0; i < Elevators.size(); i++)
	{
		Elevator *elevator = sbs->GetElevator(Elevators[i]);
		if (elevator)
		{
			isvalid = true;
			break;
		}
	}
	if (isvalid == false)
		return ReportError("No valid elevators found");

	//reset processed state
	Processed = false;

	//reset elevator arrival state
	elevator_arrived = 0;

	DestinationFloor = floor;

	if (sbs->Verbose)
		Report("Starting timer");

	//start timer
	timer->Start(50, true);

	return true;
}

bool Destination::ServicesElevator(int elevator)
{
	//return true if this controller services the specified elevator
	for (size_t i = 0; i < Elevators.size(); i++)
	{
		if (Elevators[i] == elevator)
		{
			if (sbs->Verbose)
				Report("Services elevator " + ToString(elevator));
			return true;
		}
	}
	return false;
}

void Destination::Loop()
{
	//this function runs for every registered controller via timer callback

	//process calls
	Process();
}

void Destination::Process()
{
	//process controller
	//direction is the call direction to process

	//reset active elevators
	if (Status == false)
		ActiveElevator = 0;

	//stop timer if inactive
	if (GetStatus() == false)
	{
		if (sbs->Verbose)
			Report("Deactivating timer");
		timer->Stop();
		return;
	}
	else
	{
		//if timer is starting up, restart it with a longer interval for future call processing
		timer->Start(1000);
	}

	int active_elevator = 0;
	active_elevator = ActiveElevator;

	//if active elevator becomes unavailable during call wait, reprocess call
	bool reprocess = false;
	if (active_elevator > 0 && Processed == true)
	{
		Elevator *elev = sbs->GetElevator(ActiveElevator);
		if (elev->InServiceMode() == true || elev->IsRunning() == false || elev->IsStopped() == true)
		{
			reprocess = true;
			active_elevator = 0;

			//reset processed and active elevator states
			Processed = false;
			ActiveElevator = 0;
		}
	}

	//first exit if controller is not processing a call for the current direction
	if (reprocess == false)
	{
		if (GetStatus() == false)
			return;
	}

	//get closest elevator
	int closest = FindClosestElevator();

	//if none found, exit
	if (closest == -1)
		return;

	//if a closer elevator has been found than the elevator actively serving the call,
	//reprocess the call
	/*if (active_elevator > 0 && Elevators[closest] != active_elevator)
	{
		//make sure elevator is idle before continuing
		Elevator *newelevator = sbs->GetElevator(Elevators[closest]);
		if (newelevator->IsIdle() == true)
		{
			Elevator *elevator = sbs->GetElevator(ActiveElevator);
			if (elevator)
				elevator->CancelHallCall(GetFloor(), direction);

			if (sbs->Verbose)
				Report("Switching to closer elevator " + ToString(newelevator->Number));

			//reset processed state
			if (direction == 1)
				ProcessedUp = false;
			else
				ProcessedDown = false;
		}
	}*/

	//exit if a call has already been processed
	if (Processed == true)
		return;

	//change processed state
	Processed = true;

	Elevator* elevator = sbs->GetElevator(Elevators[closest]);

	if (sbs->Verbose)
		Report("Using elevator " + ToString(elevator->Number));

	ElevatorCar *car = elevator->GetCarForFloor(GetFloor());
	if (!car)
		return;

	int direction = 0;
	if (DestinationFloor > GetFloor())
		direction = 1;
	else if (DestinationFloor < GetFloor())
		direction = -1;

	//if closest elevator is already on the called floor
	if (car->IsOnFloor(GetFloor()) && (elevator->QueuePositionDirection == direction || elevator->QueuePositionDirection == 0))
	{
		if (sbs->Verbose)
			Report("Elevator active on current floor - opening");

		//update arrival information
		if (direction == -1)
			elevator->NotifyCallButtons(GetFloor(), false);
		else
			elevator->NotifyCallButtons(GetFloor(), true);

		//notify on arrival
		if (elevator->NotifyEarly >= 0)
			car->NotifyArrival(GetFloor(), false, 0);

		//store call direction for NotifyLate feature
		if (elevator->NotifyLate == true)
			car->LateDirection = direction;

		//open elevator if it's on the same floor
		car->OpenDoors();
	}
	else
	{
		//otherwise add a route entry to this floor
		elevator->AddRoute(GetFloor(), direction, 1);

		ActiveElevator = elevator->Number;
	}
}

void Destination::Report(const std::string &message)
{
	//general reporting function
	std::string msg = "Destination Controller " + ToString(GetFloor()) + ":" + ToString(Number) + " - " + message;
	Object::Report(msg);
}

bool Destination::ReportError(const std::string &message)
{
	//general reporting function
	std::string msg = "Destination Controller " + ToString(GetFloor()) + ":" + ToString(Number) + " - " + message;
	return Object::ReportError(msg);
}

bool Destination::FireService(int value)
{
	//enables fire service phase 1 on all elevators associated with this controller

	bool status = false, status2 = false;

	for (size_t i = 0; i < Elevators.size(); i++)
	{
		Elevator *elevator = sbs->GetElevator(Elevators[i]);
		if (elevator)
			status2 = elevator->EnableFireService1(value);
		if (status2 == true)
			status = true;
	}
	return status;
}

int Destination::GetFloor()
{
	//return floor number this controller is on

	return floor->Number;
}

bool Destination::AddElevator(int elevator)
{
	//add an elevator to this controller

	for (size_t i = 0; i < Elevators.size(); i++)
	{
		if (Elevators[i] == elevator)
			return false;
	}

	Elevators.push_back(elevator);
	return true;
}

bool Destination::RemoveElevator(int elevator)
{
	//remove an elevator from this controller

	for (size_t i = 0; i < Elevators.size(); i++)
	{
		if (Elevators[i] == elevator)
		{
			Elevators.erase(Elevators.begin() + i);
			return true;
		}
	}
	return false;
}

void Destination::ElevatorArrived(int number)
{
	//notify controller about an elevator arrival
	//this also turns off the related direction light

	if (GetStatus() == true)
		elevator_arrived = number;

	Status = false;
	Processed = false;
}

int Destination::GetElevatorArrived()
{
	//return the number of the elevator that has arrived, for the specified direction
	//return 0 if no elevator has arrived yet

	return elevator_arrived;
}

int Destination::FindClosestElevator()
{
	//initialize values
	int closest = 0;
	int closest_elev = 0;
	bool check = false;
	int errors = 0;
	int result = 0;

	int active_elevator = 0;
	active_elevator = ActiveElevator;

	//if an elevator is already servicing the call, set recheck to true
	bool recheck = false;
	if (active_elevator > 0)
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
		Elevator *elevator = sbs->GetElevator(Elevators[i]);
		if (elevator)
		{
			ElevatorCar *car = elevator->GetCarForFloor(GetFloor());
			if (car)
			{
				if (sbs->Verbose && recheck == false)
					Report("Checking elevator " + ToString(elevator->Number) + " car " + ToString(car->Number));

				//if elevator is closer than the previously checked one or we're starting the checks
				if (abs(car->GetFloor() - GetFloor()) < closest || check == false)
				{
					//see if elevator is available for the call
					if (recheck == true && elevator->Number == active_elevator)
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
						result = elevator->AvailableForCallDestination(GetFloor(), DestinationFloor, !recheck);

					if (result == 1) //available
					{
						if (sbs->Verbose && count > 1 && recheck == false)
							Report("Marking - closest so far");
						closest = abs(car->GetFloor() - GetFloor());
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

		Status = false;

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

void Destination::Timer::Notify()
{
	parent->Loop();
}

bool Destination::GetStatus()
{
	return Status;
}

}
