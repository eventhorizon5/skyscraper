/*
	Scalable Building Simulator - Dispatch Controller Object
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
#include "elevator.h"
#include "elevatorcar.h"
#include "floor.h"
#include "callstation.h"
#include "controller.h"

namespace SBS {

DispatchController::DispatchController(Object *parent, int number) : Object(parent)
{
	//create a dispatch controller object

	//set up SBS object
	SetValues("DispatchController", "Dispatch Controller " + ToString(number), false);

	Number = number;
	Range = 5;
	DestinationDispatch = false;
	MaxPassengers = 5;
	Hybrid = false;

	EnableLoop(true);

	Report("Created");
}

DispatchController::~DispatchController()
{
	//unregister from parent
	if (sbs->FastDelete == false && parent_deleting == false)
		sbs->RemoveController(this);
}

void DispatchController::Loop()
{
	//this function runs for every registered dispatch controller via timer callback

	if (DestinationDispatch == false || Hybrid == true)
	{
		//this is reserved for future use

		//process up calls
		Process(1);

		//process down calls
		Process(-1);
	}

	if (DestinationDispatch == true)
		ProcessDestinationDispatch();
}

void DispatchController::Process(int direction)
{
	//this is reserved for future use
}

void DispatchController::ProcessDestinationDispatch()
{
	//destination dispatch processing

	//process pending requests
	ProcessRoutes();

	//check elevators to see if any have arrived, and if so, dispatch to destination floor
	for (int i = 0; i < Elevators.size(); i++)
	{
		Elevator *elev = sbs->GetElevator(Elevators[i].number);

		//determine if elevator has arrived
		if (Elevators[i].arrived == true)
		{
			//step through requests table and find a request that matches the arrived elevator
			for (int j = 0; j < Routes.size(); j++)
			{
				//get request's direction of travel
				int direction = 0;
				if (Routes[j].starting_floor < Routes[j].destination_floor)
					direction = 1;
				else
					direction = -1;

				//if route matches elevator
				if (Routes[j].processed == true && Routes[j].assigned_elevator == Elevators[i].number)
				{
					//if request matches the elevator arrival and assignment
					if (Routes[j].starting_floor == Elevators[i].arrival_floor && Elevators[i].assigned == true)
					{
						bool result = false;

						//dispatch elevator to destination floor for each registered assignment
						for (int k = 0; k < Elevators[i].assigned_destination.size(); k++)
						{
							if (Routes[j].destination_floor == Elevators[i].assigned_destination[k])
							{
								DispatchElevator(Elevators[i].number, Routes[j].destination_floor, direction, false);
								result = true;
							}
						}

						//remove route from table
						if (result == true)
						{
							RemoveRoute(Routes[j]);
							j--;
						}
					}
				}
			}
		}
	}
}

void DispatchController::RemoveRoute(Route &route)
{
	//remove a destination dispatch route

	for (int i = 0; i < Routes.size(); i++)
	{
		if (Routes[i].starting_floor == route.starting_floor && Routes[i].destination_floor == route.destination_floor)
		{
			if (sbs->Verbose)
				Report("Removing route from " + ToString(route.starting_floor) + " to " + ToString(route.destination_floor));
			Routes.erase(Routes.begin() + i);
			return;
		}
	}
}

bool DispatchController::RequestRoute(CallStation *station, int starting_floor, int destination_floor)
{
	//request a destination dispatch route

	if (DestinationDispatch == false)
	{
		ReportError("RequestRoute: Destination Dispatch not enabled");
		return false;
	}

	if (starting_floor == destination_floor)
	{
		ReportError("RequestRoute: Floors are the same");
		return false;
	}

	Report("Requesting route from origin floor " + ToString(starting_floor) + " to destination floor " + ToString(destination_floor));

	//make sure floors are valid
	if (!sbs->GetFloor(starting_floor) || !sbs->GetFloor(destination_floor))
		return ReportError("Invalid floor");

	//make sure this controller has elevators that serve the specified floors
	if (IsServicedFloor(starting_floor) == false)
		return ReportError("No elevators found for floor " + ToString(starting_floor));
	if (IsServicedFloor(destination_floor) == false)
		return ReportError("No elevators found for floor " + ToString(destination_floor));

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

	//use existing entry if found
	for (int i = 0; i < Routes.size(); i++)
	{
		if (Routes[i].starting_floor == starting_floor && Routes[i].destination_floor == destination_floor && Routes[i].requests < MaxPassengers)
		{
			//create a new route if max passengers has been reached
			if (Routes[i].requests >= MaxPassengers && Elevators.size() > 1)
				break;

			Routes[i].requests++;
			Report("Route has " + ToString(Routes[i].requests) + " total requests");
			return true;
		}
	}

	//otherwise add route request to table
	Route route;
	route.starting_floor = starting_floor;
	route.destination_floor = destination_floor;
	route.requests = 1;
	route.processed = false;
	route.station = station;
	route.assigned_elevator = 0;
	Routes.push_back(route);

	return true;
}

void DispatchController::ProcessRoutes()
{
	//process destination dispatch routes

	for (int i = 0; i < Routes.size(); i++)
	{
		if (Routes[i].processed == true)
			continue;

		int starting_floor = Routes[i].starting_floor;
		int destination_floor = Routes[i].destination_floor;

		//get closest elevator
		int closest = FindClosestElevator(starting_floor, destination_floor);

		//if none found, exit
		if (closest == -1)
		{
			Report("ProcessRoutes: No available elevators found, will try again");
			return;
		}

		//get elevator and car for route
		Elevator* elevator = sbs->GetElevator(Elevators[closest].number);

		Report("Using elevator " + ToString(elevator->Number));

		ElevatorCar *car = elevator->GetCarForFloor(starting_floor);
		if (!car)
			return;

		int direction = 0;
		if (destination_floor > starting_floor)
			direction = 1;
		else
			direction = -1;

		//update destination indicator with elevator ID
		if (Routes[i].station)
			Routes[i].station->UpdateIndicator(elevator->ID);

		//update elevator's call information
		for (int i = 0; i < Elevators.size(); i++)
		{
			if (Elevators[i].number == elevator->Number)
			{
				Elevators[i].destination_floor = destination_floor;
				Elevators[i].call_floor = starting_floor;
				Elevators[i].use_call_floor = true;
			}
		}

		//assign and dispatch elevator to starting floor
		AssignElevator(elevator->Number, destination_floor);
		DispatchElevator(elevator->Number, starting_floor, direction, true);
		//ActiveElevator = elevator->Number;

		Routes[i].processed = true;
		Routes[i].assigned_elevator = elevator->Number;
	}
}

bool DispatchController::AddElevator(int elevator)
{
	//add an elevator to this controller

	if (!sbs->GetElevator(elevator))
		return false;

	//exit if already in table
	for (size_t i = 0; i < Elevators.size(); i++)
	{
		if (Elevators[i].number == elevator)
			return false;
	}

	//exit if elevator is already assigned to another controller
	if (sbs->GetElevator(elevator)->Controller)
		return ReportError("Elevator " + ToString(elevator) + " already assigned to a controller");

	//create a new elevator map
	ElevMap newelevator;
	newelevator.number = elevator;
	newelevator.arrived = false;
	newelevator.arrival_floor = 0;
	newelevator.arrival_direction = false;
	newelevator.assigned = false;
	newelevator.destination_floor = 0;
	newelevator.call_floor = 0;
	newelevator.use_call_floor = false;

	//assign controller to elevator
	sbs->GetElevator(elevator)->Controller = Number;

	//process floor range
	GetFloorRange();

	Report ("Elevator " + ToString(elevator) + " added to dispatch controller " + ToString(Number));
	Elevators.push_back(newelevator);
	return true;
}

bool DispatchController::RemoveElevator(int elevator)
{
	//remove an elevator from this controller

	for (size_t i = 0; i < Elevators.size(); i++)
	{
		if (Elevators[i].number == elevator)
		{
			Report ("Elevator " + ToString(elevator) + " removed from dispatch controller " + ToString(Number));

			if (sbs->GetElevator(elevator))
				sbs->GetElevator(elevator)->Controller = 0;

			Elevators.erase(Elevators.begin() + i);
			return true;
		}
	}
	return false;
}

bool DispatchController::ServicesElevator(int elevator)
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

int DispatchController::FindClosestElevator(int starting_floor, int destination_floor)
{
	//finds closest elevator for destination dispatch mode

	//initialize values
	int closest = 0;
	int closest_elev = 0;
	bool closest_busy = false;
	bool check = false;
	int errors = 0;
	int result = 0;
	int direction = 0;

	if (destination_floor > starting_floor)
		direction = 1;
	else
		direction = -1;

	int ActiveElevator = 0;

	//***NOTE - recheck and ActiveElevator need to be rewritten for Destination Dispatch

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
			//skip elevator if it doesn't serve the destination floor
			bool serviced = elevator->IsServicedFloor(destination_floor, true);
			if (serviced == false)
				continue;

			ElevatorCar *car = elevator->GetCarForFloor(starting_floor);
			if (car)
			{
				if (sbs->Verbose && recheck == false)
					Report("Checking elevator " + ToString(elevator->Number) + " car " + ToString(car->Number));

				//skip elevator if serving a route with the maximum number of requests
				if (AtMaxRequests(Elevators[i].number, destination_floor) == true)
				{
					if (sbs->Verbose)
						Report("Skipping elevator " + ToString(elevator->Number) + " due to reached max passengers value");
					continue;
				}

				//if elevator is closer than the previously checked one or we're starting the checks
				if (abs(car->GetFloor() - starting_floor) < closest || check == false || closest_busy == true)
				{
					//see if elevator is available for the call

					/*if (recheck == true && elevator->Number == ActiveElevator)
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
					else*/
						result = elevator->AvailableForCall(starting_floor, direction, !recheck);

					if (closest_busy == true && result == 1)
					{
						closest_busy = false;
					}

					if (result < 2) //available or busy
					{
						//skip if elevator has already been assigned for another destination
						//outside of the serviced floor range, but only if multiple elevators are assigned
						if (IsElevatorAssignedToOther(elevator->Number, destination_floor) == true)
						{
							closest_busy = true;
							continue;
						}

						//mark as closest elevator
						if (sbs->Verbose && count > 1 && recheck == false)
							Report("Marking - closest so far");
						closest = abs(car->GetFloor() - starting_floor);
						closest_elev = i;
						if (result == 0)
							closest_busy = true; //mark that it's also busy
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

void DispatchController::Report(const std::string &message)
{
	//general reporting function
	std::string msg = "Dispatch Controller " + ToString(Number) + ": " + message;
	Object::Report(msg);
}

bool DispatchController::ReportError(const std::string &message)
{
	//general error reporting function
	std::string msg = "Dispatch Controller " + ToString(Number) + ": " + message;
	return Object::ReportError(msg);
}

void DispatchController::ElevatorArrived(int number, int floor, bool direction)
{
	//notify controller about an elevator arrival

	for (int i = 0; i < Elevators.size(); i++)
	{
		if (Elevators[i].number == number && Elevators[i].arrived == true)
			return;

		Floor *floorobj = sbs->GetFloor(floor);
		if (!floorobj)
			return;

		if (Elevators[i].number == number)
		{
			Report("Elevator " + ToString(number) + " arrived at floor " + ToString(floor) + " (" + floorobj->ID + ")");

			//only set values if arriving at starting floor
			if (Elevators[i].call_floor == floor && Elevators[i].use_call_floor == true)
			{
				Elevators[i].arrived = true;
				Elevators[i].arrival_floor = floor;
				Elevators[i].arrival_direction = direction;
				Elevators[i].call_floor = 0;
				Elevators[i].use_call_floor = false;
			}

			if (Elevators[i].assigned == true)
			{
				//unassign route from elevator
				for (int j = 0; j < Elevators[i].assigned_destination.size(); j++)
				{
					if (Elevators[i].assigned_destination[j] == floor)
					{
						Elevators[i].assigned_destination.erase(Elevators[i].assigned_destination.begin() + j);
						break;
					}
				}

				//unassign elevator if no more routes
				if (Elevators[i].assigned_destination.size() == 0)
					Elevators[i].assigned = false;
			}
			break;
		}
	}
}

void DispatchController::DispatchElevator(int number, int destination_floor, int direction, bool call)
{
	//dispatch an elevator to the given destination floor

	Elevator *elevator = sbs->GetElevator(number);
	Floor *floor = sbs->GetFloor(destination_floor);

	if (elevator && floor)
	{
		Report("Dispatching elevator " + ToString(number) + " to floor " + ToString(destination_floor) + " (" + floor->ID + ")");

		int type = 0;
		if (call == true && DestinationDispatch == true)
			type = 1;
		else if (DestinationDispatch == false)
			type = 2;

		//reset arrival status
		for (int i = 0; i < Elevators.size(); i++)
		{
			if (Elevators[i].number == number)
			{
				Elevators[i].arrived = false;
				Elevators[i].arrival_floor = 0;
				Elevators[i].arrival_direction = false;
				break;
			}
		}

		//add elevator route
		elevator->AddRoute(destination_floor, direction, type);
	}
}

bool DispatchController::IsElevatorAssigned(int number, int destination_floor)
{
	//return true if the specified elevator is assigned to the destination floor

	for (int i = 0; i < Elevators.size(); i++)
	{
		if (Elevators[i].number == number)
		{
			for (int j = 0; j < Elevators[i].assigned_destination.size(); j++)
			{
				if (Elevators[i].assigned_destination[j] == destination_floor && Elevators[i].assigned == true)
					return true;
			}
			break;
		}
	}
	return false;
}

bool DispatchController::IsElevatorAssignedToOther(int number, int destination_floor)
{
	//return true if the specified elevator is assigned to a destination floor other than the specified one
	//while also taking the serviced floor range into account if more than one elevator is assigned

	for (int i = 0; i < Elevators.size(); i++)
	{
		if (Elevators[i].number == number)
		{
			if (Elevators[i].assigned == false || Elevators[i].assigned_destination.size() == 0)
				return false;

			if (Elevators[i].assigned_destination[0] != destination_floor && Elevators[i].assigned == true &&
				((abs(Elevators[i].assigned_destination[0] - destination_floor) > Range) && Elevators.size() > 1))
				return true;
			else
				break;
		}
	}
	return false;
}

void DispatchController::AssignElevator(int number, int destination_floor)
{
	//assign an elevator to the specified destination floor

	for (int i = 0; i < Elevators.size(); i++)
	{
		if (Elevators[i].number == number)
		{
			Elevators[i].assigned = true;

			//exit if elevator has already been assigned to the floor
			for (int j = 0; j < Elevators[i].assigned_destination.size(); j++)
			{
				if (Elevators[i].assigned_destination[j] == destination_floor)
				{
					Report("Elevator already assigned to destination floor " + ToString(destination_floor));
					return;
				}
			}

			//add floor to assignment list
			Elevators[i].assigned_destination.push_back(destination_floor);
		}
	}
}

void DispatchController::RegisterCallStation(CallStation *station)
{
	//register the specified call station

	//exit if already registered
	for (int i = 0; i < CallStations.size(); i++)
	{
		if (CallStations[i] == station)
			return;
	}

	CallStations.push_back(station);
}

void DispatchController::UnregisterCallStation(CallStation *station)
{
	//unregister the specified call station

	for (size_t i = 0; i < CallStations.size(); i++)
	{
		if (CallStations[i] == station)
		{
			CallStations.erase(CallStations.begin() + i);
			return;
		}
	}
}

int DispatchController::GetElevatorArrived(int starting_floor, int destination_floor)
{
	//return the number of the elevator that has arrived, for the specified route
	//return 0 if no elevator has arrived yet

	for (int i = 0; i < Elevators.size(); i++)
	{
		if (Elevators[i].arrived == true)
		{
			if (Elevators[i].arrival_floor == starting_floor && Elevators[i].destination_floor == destination_floor)
				return Elevators[i].number;
		}
	}
	return 0;
}

void DispatchController::GetFloorRange()
{
	//determine floor range of associated elevators

	bool firstrun = true;

	for (size_t i = 0; i < Elevators.size(); i++)
	{
		Elevator *elev = sbs->GetElevator(Elevators[i].number);
		if (elev)
		{
			int carnum = elev->GetCarCount();
			for (int j = 1; j <= carnum; j++)
			{
				ElevatorCar *car = elev->GetCar(j);
				if (car)
				{
					int tmpbottom = car->GetBottomFloor();
					int tmptop = car->GetTopFloor();
					if (tmpbottom < bottom_floor || firstrun == true)
						bottom_floor = tmpbottom;
					if (tmptop > top_floor || firstrun == true)
						top_floor = tmptop;
					firstrun = false;
				}
			}
		}
	}
}

bool DispatchController::IsServicedFloor(int floor)
{
	//return true if this controller serves the specified floor

	for (size_t i = 0; i < Elevators.size(); i++)
	{
		bool tmp = false;
		Elevator *elev = sbs->GetElevator(Elevators[i].number);
		if (elev)
			tmp = elev->IsServicedFloor(floor);
		if (tmp == true)
			return true;
	}
	return false;
}

bool DispatchController::FireService(int value)
{
	//enables fire service phase 1 on all elevators associated with this controller

	bool status = false, status2 = false;

	for (size_t i = 0; i < Elevators.size(); i++)
	{
		Elevator *elevator = sbs->GetElevator(Elevators[i].number);
		if (elevator)
			status2 = elevator->EnableFireService1(value);
		if (status2 == true)
			status = true;
	}
	return status;
}

bool DispatchController::AtMaxRequests(int elevator, int destination_floor)
{
	//return true if the specified elevator is at the maximum requests level for the specified destination floor

	if (!sbs->GetElevator(elevator))
		return false;

	//get elevator index
	int index = -1;
	for (int i = 0; i < Elevators.size(); i++)
	{
		if (Elevators[i].number == elevator)
		{
			index = i;
			break;
		}
	}

	//return true if an elevator's assigned DD route has requests at the MaxPassengers value
	for (int i = 0; i < Routes.size(); i++)
	{
		for (int j = 0; j < Elevators[index].assigned_destination.size(); j++)
		{
			if (Elevators[index].assigned_destination[j] == destination_floor && Elevators[index].assigned == true)
			{
				if (Routes[i].requests == MaxPassengers)
					return true;
			}
		}
	}

	return false;
}

}
