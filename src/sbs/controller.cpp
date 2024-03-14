/*
	Scalable Building Simulator - Dispatch Controller Object
	The Skyscraper Project - Version 1.12 Alpha
	Copyright (C)2004-2024 Ryan Thoryk
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
	Reprocess = false;
	bottom_floor = 0;
	top_floor = 0;
	recheck = 0;

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

	//process pending requests
	ProcessRoutes();

	//check arrivals
	CheckArrivals();
}

void DispatchController::CheckArrivals()
{
	//check elevators to see if any have arrived
	//for DD mode, dispatch to destination floor
	//for standard mode, turn off call buttons and remove route

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
				int direction_dd = 0;
				bool direction_standard = false;
				if (Routes[j].destination == true)
				{
					if (Routes[j].starting_floor < Routes[j].destination_floor)
						direction_dd = 1;
					else
						direction_dd = -1;
				}
				else
				{
					if (Routes[j].direction == 1)
						direction_standard = true;
					else
						direction_standard = false;
				}

				//if route matches elevator
				if (Routes[j].processed == true && Routes[j].assigned_elevator == Elevators[i].number)
				{
					if (Routes[j].destination == true)
					{
						//destination dispatch mode

						bool route_direction = false;
						if (direction_dd == 1)
							route_direction = true;

						//if request matches the elevator arrival, assignment and direction
						if (Routes[j].starting_floor == Elevators[i].arrival_floor && Elevators[i].assigned == true && route_direction == Elevators[i].arrival_direction)
						{
							bool result = false;

							//dispatch elevator to destination floor for each registered assignment
							for (int k = 0; k < Elevators[i].assigned_destination.size(); k++)
							{
								if (Routes[j].destination_floor == Elevators[i].assigned_destination[k])
								{
									DispatchElevator(true, Elevators[i].number, Routes[j].destination_floor, direction_dd, false);
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
					else
					{
						//standard mode

						//if request matches the elevator arrival and assignment
						if (Routes[j].starting_floor == Elevators[i].arrival_floor && Elevators[i].arrival_direction == direction_standard)
						{
							//turn off related call buttons
							if (Routes[j].station)
							{
								if (direction_standard == true)
									Routes[j].station->UpLight(false);
								else
									Routes[j].station->DownLight(false);
							}

							//remove route from table
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
			Report("Removing route " + ToString(i) + " from floors " + ToString(route.starting_floor) + " to " + ToString(route.destination_floor));
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
		if (station)
			station->Error();
		ReportError("RequestRoute: Destination Dispatch not enabled");
		return false;
	}

	if (starting_floor == destination_floor)
	{
		if (station)
			station->Error();
		ReportError("RequestRoute: Floors are the same");
		return false;
	}

	Report("Requesting route from origin floor " + ToString(starting_floor) + " to destination floor " + ToString(destination_floor));

	//make sure floors are valid
	if (!sbs->GetFloor(starting_floor) || !sbs->GetFloor(destination_floor))
	{
		if (station)
			station->Error(1);
		return ReportError("Invalid floor");
	}

	//make sure this controller has elevators that serve the specified floors
	if (IsServicedFloor(starting_floor) == false)
	{
		if (station)
			station->Error(1);
		return ReportError("No elevators found for floor " + ToString(starting_floor));
	}
	if (IsServicedFloor(destination_floor) == false)
	{
		if (station)
			station->Error(1);
		return ReportError("No elevators found for floor " + ToString(destination_floor));
	}

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
	{
		if (station)
			station->Error();
		return ReportError("No valid elevators found");
	}

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

			//report on indicator for new request
			if (Routes[i].assigned_elevator > 0)
			{
				Elevator *e = sbs->GetElevator(Routes[i].assigned_elevator);

				if (Routes[i].station && e)
					Routes[i].station->ReportElevator(e);
			}
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
	route.direction = 0;
	route.destination = true;
	Report("Adding destination route " + ToString((int)Routes.size()) + " from floor " + ToString(starting_floor) + " to floor " + ToString(destination_floor));
	Routes.push_back(route);

	return true;
}

bool DispatchController::CallElevator(CallStation *station, bool direction)
{
	//call an elevator (request a standard route)

	if (DestinationDispatch == true && Hybrid == false)
	{
		if (station)
			station->Error();
		ReportError("RequestRoute: Destination Dispatch is enabled");
		return false;
	}

	if (!station)
		return false;

	int floor = 0;
	floor = station->GetFloor();

	Report("Calling an elevator from origin floor " + ToString(floor));

	//make sure this controller has elevators that serve the specified floor
	if (IsServicedFloor(floor) == false)
	{
		if (station)
			station->Error(1);
		return ReportError("No elevators found for floor " + ToString(floor));
	}

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
	{
		if (station)
			station->Error();
		return ReportError("No valid elevators found");
	}

	int dir = 0;
	if (direction == true)
		dir = 1;
	else
		dir = -1;

	//use existing entry if found
	for (int i = 0; i < Routes.size(); i++)
	{
		if (Routes[i].starting_floor == floor && Routes[i].direction == dir)
		{
			//report on indicator for new request
			if (Routes[i].assigned_elevator > 0)
			{
				Elevator *e = sbs->GetElevator(Routes[i].assigned_elevator);

				if (Routes[i].station && e)
					Routes[i].station->ReportElevator(e);
			}
			return true;
		}
	}

	//otherwise add route request to table
	Route route;
	route.starting_floor = floor;
	route.destination_floor = 0;
	route.requests = 1;
	route.processed = false;
	route.station = station;
	route.assigned_elevator = 0;
	route.direction = dir;
	route.destination = false;
	Report("Adding standard route " + ToString((int)Routes.size()) + " to floor " + ToString(floor));
	Routes.push_back(route);

	return true;
}

void DispatchController::ProcessRoutes()
{
	//process routes

	for (int i = 0; i < Routes.size(); i++)
	{
		if (Routes[i].processed == true)
		{
			if (ElevatorUnavailable(Routes[i].assigned_elevator) == true)
			{
				if (Reprocess == true || Routes[i].destination == false)
				{
					//if active elevator becomes unavailable during call wait, reprocess route
					Report("Assigned elevator " + ToString(Routes[i].assigned_elevator) + " became unavailable, reprocessing route");
					Routes[i].processed = false;
					Routes[i].assigned_elevator = 0;
				}
				else
				{
					//otherwise drop the route
					Report("Dropping route " + ToString(i) + " for elevator " + ToString(Routes[i].assigned_elevator));
					Routes.erase(Routes.begin() + i);
					return;
				}
			}
			else
			{
				//if destination dispatch is off, check to see if there's a closer elevator
				if (Routes[i].destination == false)
				{
					recheck++;
					if (recheck == 100) //don't recheck elevator every frame
					{
						recheck = 0;
						int closest = -1;
						bool busy = false;
						closest = FindClosestElevator(busy, false, Routes[i].starting_floor, 0, Routes[i].direction);

						//if a closer elevator has been found than the elevator actively serving the call,
						//reprocess the call
						if (Elevators[closest].number != Routes[i].assigned_elevator && busy == false)
						{
							//make sure elevator is idle before continuing
							Elevator *newelevator = sbs->GetElevator(Elevators[closest].number);
							if (newelevator->IsIdle() == true)
							{
								Elevator *elevator = sbs->GetElevator(Routes[i].assigned_elevator);
								if (elevator)
									elevator->CancelHallCall(Routes[i].starting_floor, Routes[i].direction);

								Report("Switching to closer elevator " + ToString(newelevator->Number));

								//unassign elevator
								Routes[i].assigned_elevator = 0;

								//reset processed state
								Routes[i].processed = false;
							}
						}
						else
							continue; //skip route if elevator is still closest
					}
					else
						continue; //skip route if not checking
				}
				else
					continue; //skip route if elevator is still available
			}
		}

		int starting_floor = Routes[i].starting_floor;
		int destination_floor = Routes[i].destination_floor;

		//get closest elevator
		int closest = -1;
		bool busy = false;
		if (Routes[i].direction == 0)
			closest = FindClosestElevator(busy, true, starting_floor, destination_floor); //destination dispatch mode
		else
			closest = FindClosestElevator(busy, false, starting_floor, destination_floor, Routes[i].direction); //standard mode

		//if none found, report an error and withdraw the route
		if (closest == -1)
		{
			Report("ProcessRoutes: No available elevators found");
			if (Routes[i].station)
				Routes[i].station->Error();

			//turn off button lights
			if (Routes[i].station)
			{
				if (Routes[i].direction == 1)
					Routes[i].station->UpLight(false);
				else
					Routes[i].station->DownLight(false);
			}

			Report("Withdrawing route " + ToString(i));
			Routes.erase(Routes.begin() + i);
			return;
		}

		//standard mode
		if (Routes[i].destination == false && Routes[i].assigned_elevator > 0)
		{
			//wait for elevator if busy
			if (busy == true)
				return;
			else
			{
				//update elevator's call information
				for (int j = 0; j < Elevators.size(); j++)
				{
					if (Elevators[j].number == Routes[i].assigned_elevator)
					{
						Call call;
						call.floor = starting_floor;
						call.direction = false;
						if (Routes[i].direction == 1)
							call.direction = true;
						Elevators[j].calls.push_back(call);
						break;
					}
				}

				//dispatch elevator if available
				DispatchElevator(false, Routes[i].assigned_elevator, starting_floor, Routes[i].direction, true);
				Routes[i].processed = true;
				return;
			}
		}

		//get elevator and car for route
		Elevator* elevator = sbs->GetElevator(Elevators[closest].number);

		Report("Using elevator " + ToString(elevator->Number));

		ElevatorCar *car = elevator->GetCarForFloor(starting_floor);
		if (!car)
			return;

		int direction = Routes[i].direction;
		if (direction == 0)
		{
			//determine direction for destination dispatch mode
			if (destination_floor > starting_floor)
				direction = 1;
			else
				direction = -1;
		}

		//update destination indicator with elevator ID
		if (Routes[i].station)
			Routes[i].station->ReportElevator(elevator);

		//update elevator's call information
		if (Routes[i].destination == true)
		{
			for (int j = 0; j < Elevators.size(); j++)
			{
				if (Elevators[j].number == elevator->Number)
				{
					Elevators[j].destination_floor = destination_floor;
					Call call;
					call.floor = starting_floor;
					call.direction = false;
					if (direction == 1)
						call.direction = true;
					Elevators[j].calls.push_back(call);
					break;
				}
			}
		}

		//assign and dispatch elevator to starting floor
		AssignElevator(elevator->Number, destination_floor, Routes[i].direction);

		//dispatch elevator in DD mode, dispatch later in standard mode
		if (Routes[i].destination == true)
		{
			DispatchElevator(true, elevator->Number, starting_floor, direction, true);
			Routes[i].processed = true;
		}

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

	//create a new elevator map
	ElevMap newelevator;
	newelevator.number = elevator;
	newelevator.arrived = false;
	newelevator.arrival_floor = 0;
	newelevator.arrival_direction = false;
	newelevator.assigned = false;
	newelevator.destination_floor = 0;
	newelevator.calls.clear();

	//assign controller to elevator
	sbs->GetElevator(elevator)->AddController(Number);

	Report ("Elevator " + ToString(elevator) + " added to dispatch controller " + ToString(Number));
	Elevators.push_back(newelevator);

	//process floor range
	GetFloorRange();

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
				sbs->GetElevator(elevator)->RemoveController(Number);

			Elevators.erase(Elevators.begin() + i);

			//process floor range
			GetFloorRange();

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

int DispatchController::FindClosestElevator(bool &busy, bool destination, int starting_floor, int destination_floor, int direction)
{
	//finds closest elevator
	//if destination is true, use Destination Dispatch mode, otherwise use Standard Mode

	//initialize values
	int closest = 0;
	int closest_busy = -1;
	int closest_notbusy = -1;
	bool check = false;
	int errors = 0;
	int result = 0;

	if (destination == true)
	{
		if (destination_floor > starting_floor)
			direction = 1;
		else
			direction = -1;
	}

	int count = (int)Elevators.size();

	//exit if no elevators are associated
	if (count == 0)
		return -1;

	//search through elevator list
	if (sbs->Verbose && count > 1)
		Report("Finding nearest available elevator...");

	//check each elevator associated with this controller to find the closest available one
	for (int i = 0; i < count; i++)
	{
		Elevator *elevator = sbs->GetElevator(Elevators[i].number);
		if (elevator)
		{
			//skip elevator if it doesn't serve the destination floor
			if (destination == true)
			{
				bool serviced = elevator->IsServicedFloor(destination_floor, true);
				if (serviced == false)
					continue;
			}

			ElevatorCar *car = elevator->GetCarForFloor(starting_floor);
			if (car)
			{
				if (sbs->Verbose)
					Report("Checking elevator " + ToString(elevator->Number) + " car " + ToString(car->Number));

				if (destination == true)
				{
					//skip elevator if serving a route with the maximum number of requests
					if (AtMaxRequests(Elevators[i].number, destination_floor) == true)
					{
						if (sbs->Verbose)
							Report("Skipping elevator " + ToString(elevator->Number) + " due to reached max passengers value");
						continue;
					}
				}

				//if elevator is closer than the previously checked one or we're starting the checks
				if (abs(car->GetFloor() - starting_floor) < closest || check == false || closest_busy >= 0)
				{
					//see if elevator is available for the call
					result = elevator->AvailableForCall(destination, starting_floor, direction, true);

					//if an elevator is not busy and available, reset closest_busy value
					if (closest_busy >= 0 && result == 1)
					{
						closest_busy = -1;
					}

					if (result < 2) //available or busy
					{
						//skip if elevator has already been assigned for another destination
						//outside of the serviced floor range but only if multiple elevators are assigned
						//or direction if in standard mode
						if (destination == true)
						{
							if (IsElevatorAssignedToOther(elevator->Number, destination_floor, 0) == true)
							{
								closest_busy = i;
								continue;
							}
						}

						//mark as closest busy elevator
						if (result == 0)
						{
							if (sbs->Verbose && count > 1)
								Report("Marking - closest so far as busy");
							closest = abs(car->GetFloor() - starting_floor);
							closest_busy = i;
						}
						else
						{
							//mark as closest elevator
							if (sbs->Verbose && count > 1)
								Report("Marking - closest so far");
							closest = abs(car->GetFloor() - starting_floor);
							closest_notbusy = i;
						}
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

	if (closest_notbusy >= 0)
	{
		busy = false;
		return closest_notbusy;
	}
	else
	{
		busy = true;
		return closest_busy;
	}
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
		//exit if elevator already arrived
		if (Elevators[i].number == number && Elevators[i].arrived == true)
			return;

		Floor *floorobj = sbs->GetFloor(floor);
		if (!floorobj)
			return;

		if (Elevators[i].number == number)
		{
			Report("Elevator " + ToString(number) + " arrived at floor " + ToString(floor) + " (" + floorobj->ID + ")");

			//only set values if arriving at starting floor
			for (int j = 0; j < (int)Elevators[i].calls.size(); j++)
			{
				if (Elevators[i].calls[j].floor == floor && Elevators[i].calls[j].direction == direction)
				{
					Elevators[i].arrived = true;
					Elevators[i].arrival_floor = floor;
					Elevators[i].arrival_direction = direction;
					Elevators[i].calls.erase(Elevators[i].calls.begin() + j);
					break;
				}
			}

			if (Elevators[i].assigned == true)
			{
				//unassign route from elevator in destination dispatch mode
				for (int j = 0; j < Elevators[i].assigned_destination.size(); j++)
				{
					if (Elevators[i].assigned_destination[j] == floor)
					{
						Elevators[i].assigned_destination.erase(Elevators[i].assigned_destination.begin() + j);
						break;
					}
				}

				//unassign route from elevator in standard mode
				for (int j = 0; j < Elevators[i].assigned_directions.size(); j++)
				{
					int direction2 = -1;
					if (direction == true)
						direction2 = 1;

					if (Elevators[i].assigned_directions[j] == direction2)
					{
						Elevators[i].assigned_directions.erase(Elevators[i].assigned_directions.begin() + j);
						break;
					}
				}

				//unassign elevator if no more routes
				if (Elevators[i].assigned_destination.size() == 0 && Elevators[i].assigned_directions.size() == 0)
					Elevators[i].assigned = false;
			}
			break;
		}
	}
}

void DispatchController::DispatchElevator(bool destination, int number, int destination_floor, int direction, bool call)
{
	//dispatch an elevator to the given destination floor

	Elevator *elevator = sbs->GetElevator(number);
	Floor *floor = sbs->GetFloor(destination_floor);

	if (elevator && floor)
	{
		Report("Dispatching elevator " + ToString(number) + " to floor " + ToString(destination_floor) + " (" + floor->ID + ")");

		int type = 0;
		if (call == true)
			type = 1;
		else if (destination == false)
			type = 2;

		//add elevator route
		elevator->AddRoute(destination_floor, direction, type);
	}
}

bool DispatchController::IsElevatorAssigned(int number, int destination_floor, int direction)
{
	//return true if the specified elevator is assigned to the specified destination floor or direction

	for (int i = 0; i < Elevators.size(); i++)
	{
		if (Elevators[i].number == number)
		{
			if (direction == 0)
			{
				//destination dispatch mode
				for (int j = 0; j < Elevators[i].assigned_destination.size(); j++)
				{
					if (Elevators[i].assigned_destination[j] == destination_floor && Elevators[i].assigned == true)
						return true;
				}
			}
			else
			{
				//standard mode
				for (int j = 0; j < Elevators[i].assigned_directions.size(); j++)
				{
					if (Elevators[i].assigned_directions[j] == direction && Elevators[i].assigned == true)
						return true;
				}
			}
			break;
		}
	}
	return false;
}

bool DispatchController::IsElevatorAssignedToOther(int number, int destination_floor, int direction)
{
	//return true if the specified elevator is assigned to a destination floor or direction other than
	//the specified one, while also taking the serviced floor range into account if more than one elevator is assigned

	for (int i = 0; i < Elevators.size(); i++)
	{
		if (Elevators[i].number == number)
		{
			if (direction == 0)
			{
				//destination dispatch mode
				if (Elevators[i].assigned == false || Elevators[i].assigned_destination.size() == 0)
					return false;

				if (Elevators[i].assigned_destination[0] != destination_floor && Elevators[i].assigned == true &&
						((abs(Elevators[i].assigned_destination[0] - destination_floor) > Range) && Elevators.size() > 1))
					return true;
				else
					break;
			}
			else
			{
				//standard mode
				if (Elevators[i].assigned == false || Elevators[i].assigned_directions.size() == 0)
					return false;

				if (Elevators[i].assigned_directions[0] != direction && Elevators[i].assigned == true)
					return true;
				else
					break;
			}
		}
	}
	return false;
}

void DispatchController::AssignElevator(int number, int destination_floor, int direction)
{
	//assign an elevator to the specified destination floor

	for (int i = 0; i < Elevators.size(); i++)
	{
		if (Elevators[i].number == number)
		{
			Elevators[i].assigned = true;

			if (direction == 0) //destination dispatch mode
			{
				//add floor to assignment list
				Elevators[i].assigned_destination.push_back(destination_floor);
			}
			else //standard mode
			{
				//exit if elevator has already been assigned to the direction
				for (int j = 0; j < Elevators[i].assigned_directions.size(); j++)
				{
					if (Elevators[i].assigned_directions[j] == direction)
					{
						Report("Elevator already assigned to direction");
						return;
					}
				}

				//add direction to assignment list
				Elevators[i].assigned_directions.push_back(direction);
			}
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

std::vector<CallStation*> DispatchController::GetCallStations(int floor)
{
	//returns list of callstations for the specified floor

	std::vector<CallStation*> stationlist;

	for (size_t i = 0; i < CallStations.size(); i++)
	{
		if (CallStations[i]->GetFloor() == floor)
		{
			stationlist.push_back(CallStations[i]);
		}
	}
	return stationlist;
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

int DispatchController::GetElevatorArrivedStandard(int floor, bool direction)
{
	//return the number of the elevator that has arrived, for the specified direction
	//return 0 if no elevator has arrived yet

	for (int i = 0; i < Elevators.size(); i++)
	{
		if (Elevators[i].arrived == true)
		{
			if (Elevators[i].arrival_floor == floor && Elevators[i].arrival_direction == direction)
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

	if (DestinationDispatch == false)
		return false;

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

void DispatchController::ResetArrival(int number)
{
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
}

int DispatchController::GetRecallFloor()
{
	if (Elevators.empty())
		return 0;

	Elevator *elevator = sbs->GetElevator(Elevators[0].number);

	//return recall floor of first elevator in group
	if (elevator)
		return elevator->GetRecallFloor();

	return 0;
}

bool DispatchController::ElevatorUnavailable(int elevator)
{
	//returns true if an elevator has become unavailable during dispatch

	Elevator *elev = sbs->GetElevator(elevator);

	if (elev->InServiceMode() == true)
	{
		Report("Elevator " + ToString(elevator) + " in service mode");
		return true;
	}
	if (elev->IsRunning() == false)
	{
		Report("Elevator " + ToString(elevator) + " not running");
		return true;
	}
	if (elev->IsStopped() == true)
	{
		Report("Elevator " + ToString(elevator) + " stopped");
		return true;
	}
	/*if (elev->Error == true)
	{
		Report("Elevator " + ToString(elevator) + " movement processing error");
		return true;
	}*/

	return false;
}

int DispatchController::GetBottomFloor()
{
	return bottom_floor;
}

int DispatchController::GetTopFloor()
{
	return top_floor;
}

int DispatchController::GetElevator(int index)
{
	//return number of the specified elevator index

	if (Elevators.size() == 0)
		return 0;

	if (index < 0 || index > Elevators.size() - 1)
		return 0;
	return Elevators[index].number;
}

bool DispatchController::SameElevators(const std::vector<int> &elevators)
{
	//return true if the list of elevators matches this controller's elevators

	if (Elevators.size() != elevators.size())
		return false;

	for (int i = 0; i < (int)Elevators.size(); i++)
	{
		if (Elevators[i].number != elevators[i])
			return false;
	}
	return true;
}

bool DispatchController::GetCallStatus(int elevator, int floor, bool &up, bool &down)
{
	//returns call status for the specified elevator and floor
	//returns true if a matching call was found

	up = false;
	down = false;
	bool result = false;

	for (int i = 0; i < Elevators.size(); i++)
	{
		if (Elevators[i].number == elevator)
		{
			for (int j = 0; j < (int)Elevators[i].calls.size(); j++)
			{
				if (Elevators[i].calls[j].floor == floor)
				{
					if (Elevators[i].calls[j].direction == true)
						up = true;
					else
						down = true;

					result = true;
				}
			}
			break;
		}
	}

	return result;
}

bool DispatchController::ServesFloor(int floor)
{
	//return true if an associated elevator serves the specified floor

	for (int i = 0; i < Elevators.size(); i++)
	{
		Elevator *elev = sbs->GetElevator(Elevators[i].number);
		if (elev)
		{
			if (elev->IsServicedFloor(floor) == true)
				return true;
		}
	}

	return false;
}

}
