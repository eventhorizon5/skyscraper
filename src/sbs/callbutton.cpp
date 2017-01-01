/* $Id$ */

/*
	Scalable Building Simulator - Call Button Object
	The Skyscraper Project - Version 1.11 Alpha
	Copyright (C)2004-2016 Ryan Thoryk
	http://www.skyscrapersim.com
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@skyscrapersim.com

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
#include "profiler.h"
#include "timer.h"
#include "buttonpanel.h"
#include "control.h"
#include "callbutton.h"

namespace SBS {

class CallButton::Timer : public TimerObject
{
public:
	CallButton *parent;
	Timer(const std::string &name, CallButton *parent) : TimerObject(parent, name)
	{
		this->parent = parent;
	}
	virtual void Notify();
};

CallButton::CallButton(Object *parent, std::vector<int> &elevators, int floornum, int number, const std::string &sound_file, std::string BackTexture, const std::string &UpButtonTexture, const std::string &UpButtonTexture_Lit, const std::string &DownButtonTexture, const std::string &DownButtonTexture_Lit, float CenterX, float CenterZ, float voffset, const std::string &direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th) : Object(parent)
{
	//create a set of call buttons

	floor = sbs->GetFloor(floornum);
	if (!floor)
		return;

	//set up SBS object
	SetValues("CallButton", "", false);

	is_enabled = true;
	Elevators.resize(elevators.size());
	for (size_t i = 0; i < elevators.size(); i++)
		Elevators[i] = elevators[i];

	ProcessedUp = false;
	ProcessedDown = false;
	UpExists = false;
	DownExists = false;
	ActiveElevatorUp = 0;
	ActiveElevatorDown = 0;
	elevator_arrived_up = 0;
	elevator_arrived_down = 0;

	//create object mesh
	std::string base = "Floor " + ToString(floornum) + ":Call Panel " + ToString(number);
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

	if (ShowBack == false)
		BackTexture = "";

	if (sbs->Verbose)
		Report("Creating button panel");

	if (floornum > bottomfloor && floornum < topfloor)
	{
		UpExists = true;
		DownExists = true;
	}
	else if (floornum == bottomfloor)
		UpExists = true;
	else if (floornum == topfloor)
		DownExists = true;

	int rows = 0;
	if (UpExists == true)
		rows++;
	if (DownExists == true)
		rows++;

	//create button panel
	float button_height = BackHeight / 3.5f;
	float button_width = BackWidth / 2;
	float h_spacing = 0.5;
	float v_spacing = 1.25;
	if (UpExists == true && DownExists == true)
		v_spacing = 0.5;

	panel = new ButtonPanel(this, 1, BackTexture, rows, 1, direction, 0, 0, button_width, button_height, h_spacing, v_spacing, BackHeight / 2, tw, th);

	//create controls
	if (sbs->Verbose)
		Report("Creating controls");

	if (UpExists == true)
	{
		int row = 1;
		std::vector<std::string> names, textures;

		textures.push_back(UpButtonTexture);
		textures.push_back(UpButtonTexture_Lit);
		names.push_back("off");
		names.push_back("up");

		panel->AddControl(sound_file, row, 1, 1, 1, 0, 0, 1, names, textures);
	}
	if (DownExists == true)
	{
		int row = 1;
		if (UpExists == true)
			row = 2;

		std::vector<std::string> names, textures;

		textures.push_back(DownButtonTexture);
		textures.push_back(DownButtonTexture_Lit);
		names.push_back("off");
		names.push_back("down");

		panel->AddControl(sound_file, row, 1, 1, 1, 0, 0, 1, names, textures);
	}

	//set position of object
	Move(CenterX, floor->GetBase(true) + voffset, CenterZ);

	//create timer
	timer = new Timer("Call Timer", this);

	//reset light status
	UpLight(false);
	DownLight(false);

	if (sbs->Verbose)
		Report("Created");
}

CallButton::~CallButton()
{
	if (timer)
	{
		timer->parent_deleting = true;
		delete timer;
	}
	timer = 0;

	if (panel)
	{
		panel->parent_deleting = true;
		delete panel;
	}

	//unregister with parent floor object
	if (sbs->FastDelete == false)
	{
		if (parent_deleting == false)
			floor->RemoveCallButton(this);
	}
}

void CallButton::Enabled(bool value)
{
	//turns call buttons on/off
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

bool CallButton::Call(bool direction)
{
	//calls the closest elevator in the elevator array list to the current floor,
	//and also depending on the direction it's traveling

	//check lock state
	if (IsLocked() == true)
		return ReportError("Call button is locked");

	//exit if direction not possible
	if (direction == true && UpExists == false)
		return false;
	if (direction == false && DownExists == false)
		return false;

	//exit if call has already been made
	if (timer->IsRunning() == true)
	{
		if (direction == true && ProcessedUp == true)
		{
			Report("Up call has already been made");
			return true;
		}
		if (direction == false && ProcessedDown == true)
		{
			Report("Down call has already been made");
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

	//turn on button lights
	if (direction == true)
		UpLight(true);
	else
		DownLight(true);

	//reset processed state
	if (direction == true)
		ProcessedUp = false;
	else
		ProcessedDown = false;

	//reset elevator arrival state
	if (direction == 1)
		elevator_arrived_up = 0;
	else
		elevator_arrived_down = 0;

	if (sbs->Verbose)
		Report("Starting timer");

	//start timer
	timer->Start(50, true);

	return true;
}

void CallButton::UpLight(bool value)
{
	//turn on the 'up' directional light

	//set light status
	if (value == true)
		SetLightsGroup(1, 0);
	else
		SetLightsGroup(2, 0);
}

void CallButton::DownLight(bool value)
{
	//turn on the 'down' directional light

	//set light status
	if (value == true)
		SetLightsGroup(0, 1);
	else
		SetLightsGroup(0, 2);
}

void CallButton::SetLights(int up, int down)
{
	//set status of call button lights
	//values are 0 for no change, 1 for on, and 2 for off

	if (UpExists == true)
	{
		if (up == 1)
		{
			if (GetUpStatus() == true)
			{
				if (sbs->Verbose)
					Report("SetLights: up light already in requested status");
				return;
			}

			if (sbs->Verbose)
				Report("SetLights: turning on up light");

			if (GetUpControl())
				GetUpControl()->SetSelectPosition(2);
		}
		if (up == 2)
		{
			if (GetUpStatus() == false)
			{
				if (sbs->Verbose)
					Report("SetLights: up light already in requested status");
				return;
			}

			if (sbs->Verbose)
				Report("SetLights: turning off up light");

			if (GetUpControl())
				GetUpControl()->SetSelectPosition(1);
		}
	}

	if (DownExists == true)
	{
		if (down == 1)
		{
			if (GetDownStatus() == true)
			{
				if (sbs->Verbose)
					Report("SetLights: down light already in requested status");
				return;
			}

			if (sbs->Verbose)
				Report("SetLights: turning on down light");

			if (GetDownControl())
				GetDownControl()->SetSelectPosition(2);
		}
		if (down == 2)
		{
			if (GetDownStatus() == false)
			{
				if (sbs->Verbose)
					Report("SetLights: down light already in requested status");
				return;
			}

			if (sbs->Verbose)
				Report("SetLights: turning off down light");

			if (GetDownControl())
				GetDownControl()->SetSelectPosition(1);
		}
	}
}

bool CallButton::ServicesElevator(int elevator)
{
	//return true if this call button services the specified elevator
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

void CallButton::Loop()
{
	//this function runs for every registered call button via timer callback

	//process up calls
	Process(1);

	//process down calls
	Process(-1);
}

void CallButton::Process(int direction)
{
	//process call button
	//direction is the call direction to process

	SBS_PROFILE("CallButton::Process");

	//reset active elevators
	if (GetUpStatus() == false && direction == 1)
		ActiveElevatorUp = 0;
	if (GetDownStatus() == false && direction == -1)
		ActiveElevatorDown = 0;

	//stop timer if inactive
	if (GetUpStatus() == false && GetDownStatus() == false)
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

	int ActiveElevator = 0;
	if (direction == 1)
		ActiveElevator = ActiveElevatorUp;
	else
		ActiveElevator = ActiveElevatorDown;

	//if active elevator becomes unavailable during call wait, reprocess call
	bool reprocess = false;
	if (ActiveElevator > 0 && ((ProcessedUp == true && direction == 1) || (ProcessedDown == true && direction == -1)))
	{
		Elevator *elev = sbs->GetElevator(ActiveElevator);
		if (elev->InServiceMode() == true || elev->IsRunning() == false || elev->IsStopped() == true)
		{
			reprocess = true;
			ActiveElevator = 0;

			//reset processed and active elevator states
			if (direction == 1)
			{
				ProcessedUp = false;
				ActiveElevatorUp = 0;
			}
			else
			{
				ProcessedDown = false;
				ActiveElevatorDown = 0;
			}
		}
	}

	//first exit if call button is not processing a call for the current direction
	if (reprocess == false)
	{
		if ((GetUpStatus() == false && direction == 1) || (GetDownStatus() == false && direction == -1))
			return;
	}

	//get closest elevator
	int closest = FindClosestElevator(direction);

	//if none found, exit
	if (closest == -1)
		return;

	//if a closer elevator has been found than the elevator actively serving the call,
	//reprocess the call
	if (ActiveElevator > 0 && Elevators[closest] != ActiveElevator)
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
	}

	//exit if a call has already been processed
	if (ProcessedUp == true && direction == 1)
		return;
	if (ProcessedDown == true && direction == -1)
		return;

	//change processed state
	if (direction == 1)
		ProcessedUp = true;
	else
		ProcessedDown = true;

	Elevator* elevator = sbs->GetElevator(Elevators[closest]);

	if (sbs->Verbose)
		Report("Using elevator " + ToString(elevator->Number));

	ElevatorCar *car = elevator->GetCarForFloor(GetFloor());
	if (!car)
		return;

	//if closest elevator is already on the called floor
	if (car->IsOnFloor(GetFloor()) && (elevator->QueuePositionDirection == direction || elevator->QueuePositionDirection == 0))
	{
		if (sbs->Verbose)
			Report("Elevator active on current floor - opening");

		if (direction == -1)
		{
			//update arrival information
			elevator->NotifyCallButtons(GetFloor(), false);

			//turn on directional indicator
			car->SetDirectionalIndicators(GetFloor(), false, true);
			//play chime sound
			car->Chime(0, GetFloor(), false);
		}
		else
		{
			//update arrival information
			elevator->NotifyCallButtons(GetFloor(), true);

			//turn on directional indicator
			car->SetDirectionalIndicators(GetFloor(), true, false);
			//play chime sound
			car->Chime(0, GetFloor(), true);
		}
		//open elevator if it's on the same floor
		car->OpenDoors();
	}
	else
	{
		//otherwise add a route entry to this floor
		elevator->AddRoute(GetFloor(), direction, 1);

		if (direction == 1)
			ActiveElevatorUp = elevator->Number;
		else
			ActiveElevatorDown = elevator->Number;
	}
}

void CallButton::Report(const std::string &message)
{
	//general reporting function
	std::string msg = "Call button " + ToString(GetFloor()) + ":" + ToString(Number) + " - " + message;
	Object::Report(msg);
}

bool CallButton::ReportError(const std::string &message)
{
	//general reporting function
	std::string msg = "Call button " + ToString(GetFloor()) + ":" + ToString(Number) + " - " + message;
	return Object::ReportError(msg);
}

void CallButton::SetLocked(bool value, int keyid)
{
	//set locked state
	Locked = value;
	KeyID = keyid;
}

bool CallButton::ToggleLock(bool force)
{
	//toggle lock state
	//if force is true, bypass key check

	//quit if user doesn't have key, if force is false
	if (KeyID != 0)
	{
		if (sbs->CheckKey(KeyID) == false && force == false)
			return ReportError("Need key " + ToString(KeyID) + "to lock/unlock");
	}

	Locked = !Locked;

	if (Locked == true)
		Report("Locked");
	else
		Report("Unlocked");

	return true;
}

bool CallButton::IsLocked()
{
	return Locked;
}

int CallButton::GetKeyID()
{
	return KeyID;
}

bool CallButton::FireService(int value)
{
	//enables fire service phase 1 on all elevators associated with this call button

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

int CallButton::GetFloor()
{
	//return floor number this call button is on

	return floor->Number;
}

void CallButton::SetLightsGroup(int up, int down)
{
	//set status of call button lights for whole group
	//values are 0 for no change, 1 for on, and 2 for off

	if (sbs->Verbose)
		Report("Call: finding grouped call buttons");

	//this call will return at least this call button
	std::vector<int> buttons = floor->GetCallButtons(Elevators[0]);

	//set status on each call button
	for (size_t i = 0; i < buttons.size(); i++)
	{
		floor->CallButtonArray[buttons[i]]->SetLights(up, down);
	}
}

bool CallButton::AddElevator(int elevator)
{
	//add an elevator to this call button

	for (size_t i = 0; i < Elevators.size(); i++)
	{
		if (Elevators[i] == elevator)
			return false;
	}

	Elevators.push_back(elevator);
	return true;
}

bool CallButton::RemoveElevator(int elevator)
{
	//remove an elevator from this call button

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

void CallButton::ElevatorArrived(int number, bool direction)
{
	//notify call button about an elevator arrival
	//this also turns off the related direction light

	if (GetUpStatus() == true && direction == true)
		elevator_arrived_up = number;
	else if (GetDownStatus() == true && direction == false)
		elevator_arrived_down = number;

	if (direction == true)
	{
		UpLight(false);
		ProcessedUp = false;
	}
	else
	{
		DownLight(false);
		ProcessedDown = false;
	}
}

int CallButton::GetElevatorArrived(bool direction)
{
	//return the number of the elevator that has arrived, for the specified direction
	//return 0 if no elevator has arrived yet

	if (direction == true)
		return elevator_arrived_up;

	return elevator_arrived_down;
}

int CallButton::FindClosestElevator(int direction)
{
	//initialize values
	int closest = 0;
	int closest_elev = 0;
	bool check = false;
	int errors = 0;
	int result = 0;

	int ActiveElevator = 0;
	if (direction == 1)
		ActiveElevator = ActiveElevatorUp;
	else
		ActiveElevator = ActiveElevatorDown;

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

	//check each elevator associated with this call button to find the closest available one
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
					if (recheck == true && elevator->Number == ActiveElevator)
						result = 1; //if rechecking elevators, consider the active one
					else
						result = elevator->AvailableForCall(GetFloor(), direction, !recheck);

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

		//turn off button lights
		if (direction == 1)
			UpLight(false);
		else
			DownLight(false);

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

void CallButton::Timer::Notify()
{
	parent->Loop();
}

bool CallButton::GetUpStatus()
{
	if (GetUpControl())
		return (GetUpControl()->GetSelectPosition() == 2);
	return false;
}

bool CallButton::GetDownStatus()
{
	if (GetDownControl())
		return (GetDownControl()->GetSelectPosition() == 2);
	return false;
}

Control* CallButton::GetUpControl()
{
	return panel->GetControl(0);
}

Control* CallButton::GetDownControl()
{
	if (DownExists == false)
		return 0;

	if (UpExists == true)
		return panel->GetControl(1);
	else
		return panel->GetControl(0);
}

}
