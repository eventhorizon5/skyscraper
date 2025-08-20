/*
	Scalable Building Simulator - Elevator Route Controller
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
#include "profiler.h"
#include "floor.h"
#include "elevator.h"
#include "elevatorcar.h"
#include "elevroute.h"

namespace SBS {

RouteController::RouteController(Elevator *parent) : Object(parent)
{
	//set up SBS object
	SetValues("RouteController", "Route Controller " + ToString(parent->Number), false);

	//set elevator number
	elevator = parent;

	QueuePositionDirection = 0;
	LastQueueDirection = 0;
	LastQueueFloor[0] = 0;
	LastQueueFloor[1] = 0;
	LimitQueue = sbs->GetConfigBool("Skyscraper.SBS.Elevator.LimitQueue", false);
	UpQueueEmpty = false;
	DownQueueEmpty = false;
	UpCall = false;
	DownCall = false;
	QueuePending = false;
	QueueResets = sbs->GetConfigBool("Skyscraper.SBS.Elevator.QueueResets", false);
}

RouteController::~RouteController()
{
}

bool RouteController::AddRoute(int floor, int direction, int call_type)
{
	//Add call route to elevator routing table, in sorted order
	//directions are either 1 for up, or -1 for down
	//call type is 0 for a car call, 1 for a hall call, 2 for a system call

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	if (elevator->Running == false)
		return ReportError("Elevator not running");

	Floor *floorobj = sbs->GetFloor(floor);

	if (!floorobj)
		return ReportError("Invalid floor " + ToString(floor));

	//if doors are open or moving in independent service mode, quit
	if (elevator->IndependentService == true && (elevator->AreDoorsOpen() == false || elevator->AreDoorsMoving() != 0))
		return ReportError("floor button must be pressed before closing doors while in independent service");

	//do not add routes if in inspection service or fire phase 1 modes
	if (elevator->InspectionService == true)
		return ReportError("cannot add route while in inspection service mode");

	if (elevator->FireServicePhase2 == 2)
		return ReportError("cannot add route while hold is enabled");

	//discard route if direction opposite queue search direction
	if (LimitQueue == true && direction != QueuePositionDirection && QueuePositionDirection != 0)
	{
		//only allow if any queue entries exist
		if ((QueuePositionDirection == 1 && UpQueue.size() > 0) || (QueuePositionDirection == -1 && DownQueue.size() > 0))
			return ReportError("cannot add route in opposite direction of queue search");
	}

	//get related car number
	ElevatorCar *car = elevator->GetCarForFloor(floor);

	if (!car)
		return ReportError("floor " + ToString(floor) + " is not a serviced floor");

	//if car is on the same floor, perform an arrival
	if (car->IsOnFloor(floor))
	{
		if (QueuePositionDirection == direction ||
			QueuePositionDirection == 0 ||
			(QueuePositionDirection == 1 && UpQueue.size() == 0) ||
			(QueuePositionDirection == -1 && DownQueue.size() == 0))
		{
			elevator->SameFloorArrival(floor, direction);
			return true;
		}
	}

	//add route in related direction queue
	if (direction == 1)
	{
		//exit if entry already exists
		if (RouteExists(true, floor) == true)
			return ReportError("route to floor " + ToString(floor) + " (" + floorobj->ID + ") already exists");

		//add floor to up queue
		UpQueue.emplace_back(QueueEntry(floor, call_type, car->Number, 1));
		//sort queue
		std::sort(UpQueue.begin(), UpQueue.end());
		QueuePending = true;

		LastQueueFloor[0] = floor;
		LastQueueFloor[1] = 1;

		//add car number info if needed
		std::string car_msg = "";
		if (elevator->GetCarCount() > 1)
			car_msg = " for car " + ToString(car->Number);

		Report("adding route to floor " + ToString(floor) + " (" + floorobj->ID + ") direction Up" + car_msg);
	}
	else
	{
		//exit if entry already exists
		if (RouteExists(false, floor) == true)
			return ReportError("route to floor " + ToString(floor) + " (" + floorobj->ID + ") already exists");

		//add floor to down queue
		DownQueue.emplace_back(QueueEntry(floor, call_type, car->Number, -1));
		//sort queue
		std::sort(DownQueue.begin(), DownQueue.end());
		QueuePending = true;

		LastQueueFloor[0] = floor;
		LastQueueFloor[1] = -1;

		//add car number info if needed
		std::string car_msg = "";
		if (elevator->GetCarCount() > 1)
			car_msg = " for car " + ToString(car->Number);

		Report("adding route to floor " + ToString(floor) + " (" + floorobj->ID + ") direction Down" + car_msg);
	}

	//make sure the car's GotoFloor status is set, if this is an additional car heading the same route
	elevator->ProcessGotoFloor(floor, direction);

	//turn on button lights for a car call, or a Destination Dispatch hall call
	if (call_type == 0)
		elevator->ChangeLight(floor, true);

	//go to ACP floor if ACP mode is enabled
	if (elevator->ACP == true && floor != elevator->ACPFloor)
	{
		//only add ACP route if original route will pass ACP floor
		if ((car->GetFloor() < elevator->ACPFloor && floor > elevator->ACPFloor) || (car->GetFloor() > elevator->ACPFloor && floor < elevator->ACPFloor))
		{
			Report("adding ACP route");
			AddRoute(elevator->ACPFloor, direction, 2);
		}
	}

	return true;
}

bool RouteController::DeleteRoute(int floor, int direction)
{
	//Delete call route from elevator routing table
	//directions are either 1 for up, or -1 for down

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	if (elevator->Running == false)
		return ReportError("Elevator not running");

	Floor *floorobj = sbs->GetFloor(floor);

	if (!floorobj)
		return ReportError("Invalid floor " + ToString(floor));

	if (direction == 1)
	{
		//delete floor entry from up queue
		for (size_t i = 0; i < UpQueue.size(); i++)
		{
			if (UpQueue[i].floor == floor)
			{
				Report("deleting route to floor " + ToString(floor) + " (" + floorobj->ID + ") direction Up");
				UpQueue.erase(UpQueue.begin() + i);
				break;
			}
		}
		if (UpQueue.size() == 0)
			UpQueueEmpty = true;
	}
	else
	{
		//delete floor entry from down queue
		for (size_t i = 0; i < DownQueue.size(); i++)
		{
			if (DownQueue[i].floor == floor)
			{
				Report("deleting route to floor " + ToString(floor) + " (" + floorobj->ID + ") direction Down");
				DownQueue.erase(DownQueue.begin() + i);
				break;
			}
		}
		if (DownQueue.size() == 0)
			DownQueueEmpty = true;
	}

	HandleDequeue(direction);

	//turn off button lights
	if (sbs->Verbose)
		Report("DeleteRoute: turning off button lights for floor " + ToString(floor));
	elevator->ChangeLight(floor, false);
	return true;
}

bool RouteController::RouteExists(bool direction, int floor)
{
	//return true if a floor route exists in the specified directional queue

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	if (direction == true)
	{
		for (size_t i = 0; i < UpQueue.size(); i++)
		{
			if (UpQueue[i].floor == floor)
				return true;
		}
	}
	else
	{
		for (size_t i = 0; i < DownQueue.size(); i++)
		{
			if (DownQueue[i].floor == floor)
				return true;
		}
	}

	return false;
}

bool RouteController::CallCancel()
{
	//cancel the last route

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	if (elevator->Running == false)
		return ReportError("Elevator not running");

	if (LastQueueFloor[1] == 0)
	{
		if (sbs->Verbose)
			ReportError("CallCancel: no valid routes");
		return false;
	}

	DeleteRoute(LastQueueFloor[0], LastQueueFloor[1]);
	Report("cancelled last call");

	return true;
}

bool RouteController::CallCancelAll()
{
	//cancels all added routes

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	if (elevator->Running == false)
		return ReportError("Elevator not running");

	if (LastQueueFloor[1] == 0)
	{
		if (sbs->Verbose)
			ReportError("CallCancel: no valid routes");
		return false;
	}

	Report("cancelled all calls");
	ResetQueue(true, true);

	return true;
}

void RouteController::ProcessCallQueue()
{
	//Processes the elevator's call queue, and sends elevators to called floors
	SBS_PROFILE("RouteController::ProcessCallQueue");

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return;

	//exit if elevator is not running
	if (elevator->Running == false)
		return;

	//exit if in inspection service mode
	if (elevator->InspectionService == true)
		return;

	//exit if stopping
	if (elevator->EmergencyStop > 0)
		return;

	//exit if moving manually
	if (elevator->ManualMove > 0)
		return;

	//exit if Go function is active
	if (elevator->GoPending == true)
		return;

	//if both queues are empty
	if (UpQueue.empty() && DownQueue.empty())
	{
		UpQueueEmpty = false;
		DownQueueEmpty = false;

		if (elevator->DownPeak == true || elevator->UpPeak == true)
		{
			int TopFloor = elevator->GetCar(1)->GetTopFloor();
			int BottomFloor = elevator->GetCar(1)->GetBottomFloor();

			//if DownPeak mode is active, send elevator to the top serviced floor if not already there
			if (elevator->GetCar(1)->GetFloor() != TopFloor && elevator->DownPeak == true && elevator->IsMoving == false)
			{
				if (sbs->Verbose)
					Report("ProcessCallQueue: sending elevator to top floor for DownPeak mode");
				AddRoute(TopFloor, 1, 2);
				return;
			}
			//if UpPeak mode is active, send elevator to the bottom serviced floor if not already there
			else if (elevator->GetCar(1)->GetFloor() != BottomFloor && elevator->UpPeak == true && elevator->IsMoving == false)
			{
				if (sbs->Verbose)
					Report("ProcessCallQueue: sending elevator to bottom floor for UpPeak mode");
				AddRoute(BottomFloor, -1, 2);
				return;
			}
		}

		if (elevator->IsIdle() == true && QueuePositionDirection != 0)
		{
			//set search direction to 0 if idle
			if (sbs->Verbose)
				Report("ProcessCallQueue: resetting search direction due to idle");
			LastQueueDirection = QueuePositionDirection;
			QueuePositionDirection = 0;
		}
		return;
	}
	else if (QueuePositionDirection == 0)
	{
		if (UpQueue.empty() == false)
		{
			if (sbs->Verbose)
				Report("ProcessCallQueue: setting search direction to up");
			QueuePositionDirection = 1;
		}
		else if (DownQueue.empty() == false)
		{
			if (sbs->Verbose)
				Report("ProcessCallQueue: setting search direction to down");
			QueuePositionDirection = -1;
		}
		LastQueueDirection = 0;
	}

	//reverse queues if related queue empty flag is set
	if (QueuePositionDirection == 1 && UpQueueEmpty == true && DownQueue.empty() == false && (elevator->NotifyEarly <= 0 || elevator->NotifyEarly == 3))
	{
		if (UpCall == false)
		{
			if (sbs->Verbose)
				Report("ProcessCallQueue: setting search direction to down");
			LastQueueDirection = QueuePositionDirection;
			QueuePositionDirection = -1;
		}
	}
	if (QueuePositionDirection == -1 && DownQueueEmpty == true && UpQueue.empty() == false && (elevator->NotifyEarly <= 0 || elevator->NotifyEarly == 3))
	{
		if (DownCall == false)
		{
			if (sbs->Verbose)
				Report("ProcessCallQueue: setting search direction to up");
			LastQueueDirection = QueuePositionDirection;
			QueuePositionDirection = 1;
		}
	}

	UpQueueEmpty = false;
	DownQueueEmpty = false;
	UpCall = false;
	DownCall = false;

	//set search direction to 0 if any related queue is empty, and if doors are not open or moving
	if (elevator->AreDoorsOpen() == false && elevator->AreDoorsMoving() == 0)
	{
		if (QueuePositionDirection == 1 && UpQueue.empty())
		{
			if (sbs->Verbose)
				Report("ProcessCallQueue: resetting search direction due to empty up queue");
			QueuePositionDirection = 0;
			LastQueueDirection = 1;
		}
		if (QueuePositionDirection == -1 && DownQueue.empty())
		{
			if (sbs->Verbose)
				Report("ProcessCallQueue: resetting search direction due to empty down queue");
			QueuePositionDirection = 0;
			LastQueueDirection = -1;
		}
	}
	else if (elevator->UpPeak == false && elevator->DownPeak == false)
		return; //don't process the main queue code if doors are open or moving

	//Search through queue lists and find next valid floor call
	if (QueuePositionDirection == 1)
	{
		//search through up queue
		for (size_t i = 0; i < UpQueue.size(); i++)
		{
			ElevatorCar *car = elevator->GetCarForFloor(UpQueue[i].floor);
			if (!car)
				return;

			std::string car_msg = "";
			if (elevator->GetCarCount() > 1)
				car_msg = " for car " + ToString(car->Number);

			//if the queued floor number is a higher floor, dispatch the elevator to that floor
			if (UpQueue[i].floor >= car->CurrentFloor)
			{
				if (elevator->MoveElevator == false)
				{
					if (sbs->Verbose)
						Report("ProcessCallQueue up: standard dispatch, floor " + ToString(UpQueue[i].floor) + car_msg);
					ActiveCall = UpQueue[i];
					elevator->GotoFloor = UpQueue[i].floor;
					elevator->GotoFloorCar = car->Number;
					car->GotoFloor = true;
					if (elevator->FireServicePhase2 == 0 || elevator->UpPeak == true || elevator->DownPeak == true)
					{
						elevator->WaitForDoors = true;
						elevator->CloseDoors();
					}
					elevator->MoveElevator = true;
					LastQueueDirection = 1;
					QueuePending = false;
					elevator->ProcessGotoFloor(elevator->GotoFloor, QueuePositionDirection);
				}
				else if (elevator->Leveling == false && elevator->ActiveDirection == 1)
				{
					//if elevator is moving and not leveling, change destination floor if not beyond decel marker of that floor
					if (elevator->GotoFloor != UpQueue[i].floor)
					{
						if (car == elevator->GetCar(elevator->GotoFloorCar)) //make sure car is the same
						{
							Real tmpdestination = elevator->GetDestinationAltitude(UpQueue[i].floor);
							if (elevator->BeyondDecelMarker(1, tmpdestination) == false && sbs->GetFloor(elevator->GotoFloor))
							{
								ActiveCall = UpQueue[i];
								elevator->GotoFloor = UpQueue[i].floor;
								elevator->GotoFloorCar = car->Number;
								elevator->Destination = tmpdestination;
								Report("changing destination floor to " + ToString(elevator->GotoFloor) + " (" + sbs->GetFloor(elevator->GotoFloor)->ID + ")" + car_msg);
							}
							else if (sbs->Verbose)
								Report("ProcessCallQueue up: cannot change destination floor to " + ToString(UpQueue[i].floor) + car_msg);
						}
					}
				}
				return;
			}
			//if the queued floor number is a lower floor
			if (UpQueue[i].floor < car->CurrentFloor && elevator->MoveElevator == false)
			{
				//dispatch elevator if it's idle
				if (elevator->IsIdle() == true && LastQueueDirection == 0)
				{
					if (sbs->Verbose)
						Report("ProcessCallQueue up: dispatching idle lower elevator, floor " + ToString(UpQueue[i].floor) + car_msg);
					ActiveCall = UpQueue[i];
					elevator->GotoFloor = UpQueue[i].floor;
					elevator->GotoFloorCar = car->Number;
					car->GotoFloor = true;
					if (elevator->FireServicePhase2 == 0 || elevator->UpPeak == true || elevator->DownPeak == true)
					{
						elevator->WaitForDoors = true;
						elevator->CloseDoors();
					}
					elevator->MoveElevator = true;
					LastQueueDirection = 1;
					QueuePending = false;
					elevator->ProcessGotoFloor(elevator->GotoFloor, QueuePositionDirection);
					return;
				}
				//reset search direction if it's the last entry and idle
				if (i == UpQueue.size() - 1 && elevator->IsIdle() == true && QueuePositionDirection != 0)
				{
					if (sbs->Verbose)
						Report("ProcessCallQueue up: resetting search direction since last entry is lower" + car_msg);
					LastQueueDirection = QueuePositionDirection;
					QueuePositionDirection = 0;
					return;
				}
				//otherwise skip it if it's not the last entry
				if (sbs->Verbose)
					Report("ProcessCallQueue up: skipping floor entry " + ToString(UpQueue[i].floor) + car_msg);
			}
		}
	}
	else if (QueuePositionDirection == -1)
	{
		//search through down queue (search order is reversed since calls need to be processed in descending order)
		for (size_t i = DownQueue.size() - 1; i < DownQueue.size(); --i)
		{
			ElevatorCar *car = elevator->GetCarForFloor(DownQueue[i].floor);
			if (!car)
				return;

			std::string car_msg = "";
			if (elevator->GetCarCount() > 1)
				car_msg = " for car " + ToString(car->Number);

			//if the queued floor number is a lower floor, dispatch the elevator to that floor
			if (DownQueue[i].floor <= car->CurrentFloor)
			{
				if (elevator->MoveElevator == false)
				{
					if (sbs->Verbose)
						Report("ProcessCallQueue down: standard dispatch, floor " + ToString(DownQueue[i].floor) + car_msg);
					ActiveCall = DownQueue[i];
					elevator->GotoFloor = DownQueue[i].floor;
					elevator->GotoFloorCar = car->Number;
					car->GotoFloor = true;
					if (elevator->FireServicePhase2 == 0 || elevator->UpPeak == true || elevator->DownPeak == true)
					{
						elevator->WaitForDoors = true;
						elevator->CloseDoors();
					}
					elevator->MoveElevator = true;
					LastQueueDirection = -1;
					QueuePending = false;
					elevator->ProcessGotoFloor(elevator->GotoFloor, QueuePositionDirection);
				}
				else if (elevator->Leveling == false && elevator->ActiveDirection == -1)
				{
					//if elevator is moving and not leveling, change destination floor if not beyond decel marker of that floor
					if (elevator->GotoFloor != DownQueue[i].floor)
					{
						if (car == elevator->GetCar(elevator->GotoFloorCar)) //make sure car is the same
						{
							Real tmpdestination = elevator->GetDestinationAltitude(DownQueue[i].floor);
							if (elevator->BeyondDecelMarker(-1, tmpdestination) == false && sbs->GetFloor(elevator->GotoFloor))
							{
								ActiveCall = DownQueue[i];
								elevator->GotoFloor = DownQueue[i].floor;
								elevator->GotoFloorCar = car->Number;
								elevator->Destination = tmpdestination;
								Report("changing destination floor to " + ToString(elevator->GotoFloor) + " (" + sbs->GetFloor(elevator->GotoFloor)->ID + ")" + car_msg);
							}
							else if (sbs->Verbose)
								Report("ProcessCallQueue down: cannot change destination floor to " + ToString(DownQueue[i].floor) + car_msg);
						}
					}
				}
				return;
			}
			//if the queued floor number is an upper floor
			if (DownQueue[i].floor > car->CurrentFloor && elevator->MoveElevator == false)
			{
				//dispatch elevator if idle
				if (elevator->IsIdle() == true && LastQueueDirection == 0)
				{
					if (sbs->Verbose)
						Report("ProcessCallQueue down: dispatching idle higher elevator, floor " + ToString(DownQueue[i].floor) + car_msg);
					ActiveCall = DownQueue[i];
					elevator->GotoFloor = DownQueue[i].floor;
					elevator->GotoFloorCar = car->Number;
					car->GotoFloor = true;
					if (elevator->FireServicePhase2 == 0 || elevator->UpPeak == true || elevator->DownPeak == true)
					{
						elevator->WaitForDoors = true;
						elevator->CloseDoors();
					}
					elevator->MoveElevator = true;
					LastQueueDirection = -1;
					QueuePending = false;
					elevator->ProcessGotoFloor(elevator->GotoFloor, QueuePositionDirection);
					return;
				}
				//reset search direction if it's the last entry and idle
				if (i == 0 && elevator->IsIdle() == true && QueuePositionDirection != 0)
				{
					if (sbs->Verbose)
						Report("ProcessCallQueue down: resetting search direction since last entry is higher" + car_msg);
					LastQueueDirection = QueuePositionDirection;
					QueuePositionDirection = 0;
					return;
				}
				//otherwise skip it if it's not the last entry
				if (sbs->Verbose)
					Report("ProcessCallQueue down: skipping floor entry " + ToString(DownQueue[i].floor) + car_msg);
			}
		}
	}
}

void RouteController::DumpQueues()
{
	//dump both (up and down) elevator queues

	Object::Report("\n--- " + GetName() + "'s Queues ---\n");

	if (UpQueue.size() > 0)
		Object::Report("Up:");

	for (size_t i = 0; i < UpQueue.size(); i++)
	{
		std::string type = "Car";
		if (UpQueue[i].call_type == 1)
			type = "Hall";
		if (UpQueue[i].call_type == 2)
			type = "System";
		if (UpQueue[i].call_type == 3)
			type = "Destination";

		std::string car;
		if (elevator->GetCarCount() > 1)
			car = "\t-\tCar: " + ToString(UpQueue[i].car);

		Object::Report("Entry: " + ToString((int)i) + "\t-\tFloor: " + ToString(UpQueue[i].floor) + "\t-\tCall type: " + type + car);
	}

	if (DownQueue.size() > 0)
		Object::Report("Down:");

	for (size_t i = 0; i < DownQueue.size(); i++)
	{
		std::string type = "Car";
		if (DownQueue[i].call_type == 1)
			type = "Hall";
		if (DownQueue[i].call_type == 2)
			type = "System";
		if (DownQueue[i].call_type == 3)
			type = "Destination";

		std::string car;
		if (elevator->GetCarCount() > 1)
			car = "\t-\tCar: " + ToString(DownQueue[i].car);

		Object::Report("Entry: " + ToString((int)i) + "\t-\tFloor: " + ToString(DownQueue[i].floor) + "\t-\tCall type: " + type + car);
	}
	Object::Report("");
}

void RouteController::ResetQueue(bool up, bool down, bool stop_if_empty)
{
	//reset queues
	//if stop_if_empty is true, the elevator will stop when the related queue is empty

	QueuePending = false;

	if (up == true)
	{
		if (sbs->Verbose)
			Report("QueueReset: resetting up queue");
		UpQueue.clear();
		HandleDequeue(1, stop_if_empty);
	}
	if (down == true)
	{
		if (sbs->Verbose)
			Report("QueueReset: resetting down queue");
		DownQueue.clear();
		HandleDequeue(-1, stop_if_empty);
	}

	elevator->ResetLights();
}

void RouteController::DeleteActiveRoute()
{
	//only run if power is enabled
	if (sbs->GetPower() == false)
		return;

	if (elevator->Running == false)
	{
		ReportError("Elevator not running");
		return;
	}

	//deletes the active route
	if (sbs->Verbose)
		Report("deleting active route");
	DeleteRoute(ActiveCall.floor, ActiveCall.direction);

	//delete associated routes for any other cars
	for (int i = 1; i <= elevator->GetCarCount(); i++)
	{
		int floor = elevator->GetFloorForCar(i, ActiveCall.floor);
		if (IsQueued(floor, ActiveCall.direction) == true)
			DeleteRoute(floor, ActiveCall.direction);
	}

	ActiveCall.floor = 0;
	ActiveCall.direction = 0;
	ActiveCall.call_type = 0;
	ActiveCall.car = 0;
}

bool RouteController::IsQueueActive()
{
	if (QueuePositionDirection != 0)
		return true;
	return false;
}

bool RouteController::IsQueued(int floor, int queue)
{
	//return true if the given floor is in either queue
	//if queue is 0, check both queues; otherwise up queue with 1, and down queue with -1

	if (queue == 0 || queue == 1)
	{
		for (size_t i = 0; i < UpQueue.size(); i++)
		{
			if (UpQueue[i].floor == floor)
				return true;
		}
	}

	if (queue == 0 || queue == -1)
	{
		for (size_t i = 0; i < DownQueue.size(); i++)
		{
			if (DownQueue[i].floor == floor)
				return true;
		}
	}

	return false;
}

int RouteController::GetActiveCallFloor()
{
	return ActiveCall.floor;
}

int RouteController::GetActiveCallDirection()
{
	return ActiveCall.direction;
}

int RouteController::GetActiveCallType()
{
	return ActiveCall.call_type;
}

void RouteController::HandleDequeue(int direction, bool stop_if_empty)
{
	//handle elevator behavior on dequeue
	//if stop_if_empty is true, this will stop the elevator if the related queue is empty

	if (stop_if_empty == true && elevator->MoveElevator == true && elevator->EmergencyStop == 0)
	{
		if ((direction == 1 && UpQueue.empty()) ||
				(direction == -1 && DownQueue.empty()))
			elevator->Stop();
	}

	//reset active call status if queues are empty
	if (DownQueue.empty() == true && UpQueue.empty() == true)
	{
		ActiveCall.floor = 0;
		ActiveCall.direction = 0;
		ActiveCall.call_type = 0;
		ActiveCall.car = 0;
	}
}

void RouteController::Report(const std::string &message)
{
	//general reporting function
	Object::Report(GetName() + ": " + message);
}

bool RouteController::ReportError(const std::string &message)
{
	//general reporting function
	return Object::ReportError(GetName() + ": " + message);
}

}
