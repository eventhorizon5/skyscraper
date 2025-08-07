/*
	Scalable Building Simulator - Elevator Door Object
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
#include "polymesh.h"
#include "floor.h"
#include "elevator.h"
#include "elevatorcar.h"
#include "shaft.h"
#include "mesh.h"
#include "camera.h"
#include "texman.h"
#include "trigger.h"
#include "profiler.h"
#include "sound.h"
#include "timer.h"
#include "doorsystem.h"
#include "utility.h"
#include "elevatordoor.h"

namespace SBS {

//door autoclose timer
class ElevatorDoor::Timer : public TimerObject
{
public:
	ElevatorDoor *door;
	ElevatorCar *car;
	Elevator *elevator;
	int type; //0 = autoclose, 1 = nudge

	Timer(const std::string &name, ElevatorDoor *parent, ElevatorCar *car, int Type) : TimerObject(parent, name)
	{
		door = parent;
		this->car = car;
		elevator = car->GetElevator();
		type = Type;
	}
	virtual void Notify();
};

ElevatorDoor::ElevatorDoor(int number, ElevatorCar* car) : Object(car)
{
	//set up SBS object
	SetValues("ElevatorDoor", "", false);

	std::string name = "Elevator Door " + ToString(number);
	SetName(name);

	//create a new elevator door
	Number = number;
	this->car = car;
	elev = car->GetElevator();
	OpenDoor = 0;
	OpenSpeed = sbs->GetConfigFloat("Skyscraper.SBS.Elevator.Door.OpenSpeed", 0.3);
	WhichDoors = 0;
	ShaftDoorFloor = 0;
	DoorTimer = sbs->GetConfigInt("Skyscraper.SBS.Elevator.Door.Timer", 5000);
	DoorIsRunning = false;
	index = 0;
	previous_open = false;
	door_changed = false;
	quick_close = false;
	OpenSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.Door.OpenSound", "elevatoropen.wav");
	CloseSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.Door.CloseSound", "elevatorclose.wav");
	UpChimeSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.Door.UpChimeSound", "chime1-up.wav");
	DownChimeSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.Door.DownChimeSound", "chime1-down.wav");
	EarlyUpChimeSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.Door.EarlyUpChimeSound", "chime1-up.wav");
	EarlyDownChimeSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.Door.EarlyUpChimeSound", "chime1-down.wav");
	NudgeSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.Door.NudgeSound", "buzz.wav");
	doors_stopped = false;
	ShaftDoorThickness = 0;
	ShaftDoorOrigin = Vector3::ZERO;
	nudge_enabled = false;
	ManualSpeed = sbs->GetConfigFloat("Skyscraper.SBS.Elevator.Door.ManualSpeed", 0.2);
	SlowSpeed = sbs->GetConfigFloat("Skyscraper.SBS.Elevator.Door.SlowSpeed", 0.5);
	QuickClose = sbs->GetConfigInt("Skyscraper.SBS.Elevator.Door.QuickClose", 3000);
	NudgeTimer = sbs->GetConfigFloat("Skyscraper.SBS.Elevator.Door.NudgeTimer", 30);
	nudgesound_loaded = false;
	chimesound_loaded = 0;
	earlychimesound_loaded = 0;
	sensor = 0;
	sensor_action = 0;
	reset_action = 0;
	sensor_enabled = false;
	sensor_status = false;
	EnableSensor(sbs->GetConfigBool("Skyscraper.SBS.Elevator.Door.Sensor", true));
	SensorSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.Door.SensorSound", "");
	DoorDirection = false;
	EarlyUpSet = false;
	EarlyDownSet = false;

	//create main door object
	Doors = new DoorWrapper(this, this, false);

	//create timers
	timer = new Timer("Door Close Timer", this, car, 0);
	nudgetimer = new Timer("Nudge Timer", this, car, 1);

	//initialize shaft door array
	ShaftDoors.resize(car->ServicedFloors.size());
	for (size_t i = 0; i < ShaftDoors.size(); i++)
		ShaftDoors[i] = 0;

	//create sound object
	doorsound = new Sound(this, "Door Sound", true);
	chime = new Sound(this, "Chime", true);
	earlychime = new Sound(this, "EarlyChime", true);
	nudgesound = new Sound(this, "Nudge Sound", true);
}

ElevatorDoor::~ElevatorDoor()
{
	//delete shaft door objects
	for (size_t i = 0; i < ShaftDoors.size(); i++)
	{
		if (ShaftDoors[i])
		{
			ShaftDoors[i]->parent_deleting = true;
			delete ShaftDoors[i];
		}
		ShaftDoors[i] = 0;
	}

	//Destructor
	if (timer)
	{
		timer->parent_deleting = true;
		delete timer;
	}
	timer = 0;

	if (nudgetimer)
	{
		nudgetimer->parent_deleting = true;
		delete nudgetimer;
	}
	nudgetimer = 0;

	if (sensor)
	{
		sensor->parent_deleting = true;
		delete sensor;
	}
	sensor = 0;

	if (doorsound)
	{
		doorsound->parent_deleting = true;
		delete doorsound;
	}
	doorsound = 0;
	if (chime)
	{
		chime->parent_deleting = true;
		delete chime;
	}
	chime = 0;
	if (earlychime)
	{
		earlychime->parent_deleting = true;
		delete earlychime;
	}
	if (nudgesound)
	{
		nudgesound->parent_deleting = true;
		delete nudgesound;
	}
	nudgesound = 0;

	//delete main doors
	if (Doors)
	{
		Doors->parent_deleting = true;
		delete Doors;
	}
	Doors = 0;

	if (sbs->FastDelete == false)
	{
		//delete sensor actions
		sbs->RemoveAction(sensor_action);
		sbs->RemoveAction(reset_action);

		//unregister from parent
		if (parent_deleting == false)
			car->RemoveElevatorDoor(this);
	}
}

void ElevatorDoor::AddServicedFloor(int floor)
{
	//add serviced floor to door

	DoorWrapper *wrapper = 0;

	if (ShaftDoors.empty() == true)
	{
		ShaftDoors.resize(1);
		ShaftDoors[0] = 0;
		return;
	}

	for (size_t i = 0; i < ShaftDoors.size(); i++)
	{
		if (ShaftDoors[i])
		{
			if (ShaftDoors[i]->floor > floor)
			{
				//insert inside
				ShaftDoors.insert(ShaftDoors.begin() + i, wrapper);
				return;
			}
		}
	}

	//insert at top
	ShaftDoors.emplace_back(wrapper);
}

void ElevatorDoor::RemoveServicedFloor(int floor)
{
	//remove serviced floor from door

	for (size_t i = 0; i < ShaftDoors.size(); i++)
	{
		if (ShaftDoors[i])
		{
			if (ShaftDoors[i]->floor == floor)
			{
				//delete the matching element
				delete ShaftDoors[i];
				ShaftDoors.erase(ShaftDoors.begin() + i);
				return;
			}
		}
	}
}

void ElevatorDoor::OpenDoorsEmergency(int whichdoors, int floor)
{
	//Simulates manually prying doors open.
	//Slowly opens the elevator doors no matter where elevator is.
	//If lined up with shaft doors, then opens the shaft doors also

	//WhichDoors is the doors to move:
	//1 = both shaft and elevator doors
	//2 = only elevator doors
	//3 = only shaft doors

	OpenDoors(whichdoors, floor, true);
}

void ElevatorDoor::CloseDoorsEmergency(int whichdoors, int floor)
{
	//Simulates manually closing doors.
	//Slowly closes the elevator doors no matter where elevator is.
	//If lined up with shaft doors, then closes the shaft doors also

	//WhichDoors is the doors to move:
	//1 = both shaft and elevator doors
	//2 = only elevator doors
	//3 = only shaft doors

	CloseDoors(whichdoors, floor, true);
}

void ElevatorDoor::OpenDoors(int whichdoors, int floor, bool manual)
{
	//Opens elevator doors

	//if manual is true, then it simulates manually prying doors open,
	//Slowly opens the elevator doors no matter where elevator is,
	//and if lined up with shaft doors, then opens the shaft doors also

	//WhichDoors is the doors to move:
	//1 = both shaft and elevator doors
	//2 = only elevator doors
	//3 = only shaft doors

	if (sbs->GetPower() == false && manual == false)
		return;

	//exit if trying to open doors while stopped
	if (manual == false && doors_stopped == true)
	{
		car->ReportError("cannot open doors" + GetNumberText() + "; doors manually stopped");
		return;
	}

	//exit if in nudge mode
	if (GetNudgeStatus() == true)
	{
		car->ReportError("cannot open doors" + GetNumberText() + "; nudge mode enabled");
		return;
	}

	//exit if direction changed (if doors have already switched from close to open)
	if (door_changed == true)
		return;

	//don't open doors if emergency stop is enabled
	if (elev->OnFloor == false && whichdoors != 3 && manual == false && elev->AutoDoors == true)
	{
		car->ReportError("cannot open doors" + GetNumberText() + "; emergency stop enabled");
		return;
	}

	//exit if doors are manually moving, or automatically moving and a manual open is requested
	if (DoorIsRunning == true && (AreDoorsMoving(2) == true || (AreDoorsMoving(1) == true && manual == true)))
	{
		car->ReportError("doors" + GetNumberText() + " in use");
		return;
	}

	//check if elevator doors are already open
	if (AreDoorsOpen() == true && whichdoors != 3 && AreDoorsMoving() == false && doors_stopped == false)
	{
		//reset timer if not in a service mode or waiting in a peak mode
		if (elev->InServiceMode() == false && elev->PeakWaiting() == false)
		{
			car->Report("doors" + GetNumberText() + " already open; resetting timer");
			Reset();
		}
		else
			car->Report("doors" + GetNumberText() + " already open");
		return;
	}

	//check if shaft doors are already open
	if (whichdoors == 3)
	{
		//exit if other doors are running (this doesn't yet support multiple shaft door movements)
		if (DoorIsRunning == true)
			return;

		//first make sure the shaft doors are valid
		if (ShaftDoorsExist(floor) == false)
		{
			car->ReportError("Doors" + GetNumberText() + ": invalid shaft doors");
			return;
		}
		if (AreShaftDoorsOpen(floor) == true)
		{
			car->Report("shaft doors" + GetNumberText() + " already open on floor " + ToString(floor) + " (" + sbs->GetFloor(floor)->ID + ")");
			return;
		}
		else if (manual == false)
			car->Report("opening shaft doors" + GetNumberText() + " on floor " + ToString(floor) + " (" + sbs->GetFloor(floor)->ID + ")");
		else
			car->Report("manually opening shaft doors" + GetNumberText() + " on floor " + ToString(floor) + " (" + sbs->GetFloor(floor)->ID + ")");
	}
	else if (manual == false)
		car->Report("opening doors" + GetNumberText());
	else
		car->Report("manually opening doors" + GetNumberText());

	if (manual == false)
		OpenDoor = 1;
	else
		OpenDoor = 2;

	//if both doors are selected but elevator is not on a floor, only open elevator doors
	if (whichdoors == 1 && elev->OnFloor == false)
		whichdoors = 2;

	//change floor to elevator's destination floor if it's currently leveling to that floor
	//this fixes an issue where the elevator might not yet be on the destination floor when this function is called
	if (whichdoors == 1)
	{
		if (elev->MoveElevator == true && elev->Leveling == true)
			floor = elev->GetFloorForCar(car->Number, elev->GotoFloor);
		else
			floor = car->GetFloor();
	}

	//if opening both doors, exit if shaft doors don't exist
	int index = GetManualIndex(floor);
	if (whichdoors == 1 && ShaftDoorsExist(floor) == false && index == -1)
	{
		car->ReportError("can't open doors" + GetNumberText() + " - no shaft doors on " + ToString(floor));
		OpenDoor = 0;
		return;
	}

	WhichDoors = whichdoors;
	ShaftDoorFloor = floor;
	MoveDoors(true, manual);
}

void ElevatorDoor::CloseDoors(int whichdoors, int floor, bool manual)
{
	//Closes elevator doors

	//WhichDoors is the doors to move:
	//1 = both shaft and elevator doors
	//2 = only elevator doors
	//3 = only shaft doors

	if (sbs->GetPower() == false && manual == false)
		return;

	//exit if trying to open doors while stopped
	if (manual == false && doors_stopped == true)
	{
		car->ReportError("cannot close doors" + GetNumberText() + "; doors manually stopped");
		return;
	}

	//do not close doors while fire service mode 1 is in recall mode and the elevator is waiting at the recall floor
	if (manual == false && elev->FireServicePhase1 == 1 && elev->FireServicePhase2 == 0 && elev->WaitForDoors == false && elev->OnRecallFloor() == true)
	{
		car->ReportError("cannot close doors" + GetNumberText() + " while Fire Service Phase 1 is in recall mode");
		return;
	}

	//do not close doors while fire service mode 2 is set to hold
	if (manual == false && elev->FireServicePhase2 == 2)
	{
		car->ReportError("cannot close doors" + GetNumberText() + " while Fire Service Phase 2 is set to hold");
		return;
	}

	//exit if doors are manually moving, or automatically moving and a manual close is requested
	if (DoorIsRunning == true && (AreDoorsMoving(2) == true || (AreDoorsMoving(1) == true && manual == true)))
	{
		car->ReportError("doors" + GetNumberText() + " in use");
		return;
	}

	//if called while doors are opening, set quick_close (causes door timer to trigger faster)
	if (AreDoorsMoving() == true && manual == false && elev->FireServicePhase2 == 0)
	{
		car->Report("will close doors" + GetNumberText() + " in quick-close mode");
		quick_close = true;
		return;
	}

	//check if elevator doors are already closed
	if (AreDoorsOpen() == false && whichdoors != 3 && AreDoorsMoving() == false && doors_stopped == false)
	{
		car->Report("doors" + GetNumberText() + " already closed");
		return;
	}

	//check if shaft doors are already closed
	if (whichdoors == 3)
	{
		//exit if other doors are running (this doesn't yet support multiple shaft door movements)
		if (DoorIsRunning == true)
			return;

		//first make sure the shaft doors are valid
		if (ShaftDoorsExist(floor) == false)
		{
			car->ReportError("Doors" + GetNumberText() + ": invalid shaft doors");
			return;
		}
		if (AreShaftDoorsOpen(floor) == false && whichdoors == 3)
		{
			car->ReportError("shaft doors" + GetNumberText() + " already closed on floor " + ToString(floor) + " (" + sbs->GetFloor(floor)->ID + ")");
			return;
		}
		else if (manual == false)
			car->Report("closing shaft doors" + GetNumberText() + " on floor " + ToString(floor) + " (" + sbs->GetFloor(floor)->ID + ")");
		else
			car->Report("manually closing shaft doors" + GetNumberText() + " on floor " + ToString(floor) + " (" + sbs->GetFloor(floor)->ID + ")");
	}
	else if (manual == false)
		car->Report("closing doors" + GetNumberText());
	else
		car->Report("manually closing doors" + GetNumberText());

	if (manual == false)
		OpenDoor = -1;
	else
		OpenDoor = -2;

	//if both doors are selected but elevator is not on a floor, only close elevator doors
	if (whichdoors == 1 && elev->OnFloor == false)
		whichdoors = 2;

	if (whichdoors != 3)
		floor = car->GetFloor();

	//if closing both doors, exit if shaft doors don't exist
	int index = GetManualIndex(floor);
	if (whichdoors == 1 && ShaftDoorsExist(floor) == false && index == -1)
	{
		car->ReportError("can't close doors" + GetNumberText() + " - no shaft doors on " + ToString(floor));
		OpenDoor = 0;
		return;
	}

	//turn off directional indicators
	car->SetDirectionalIndicators(floor, false, false);

	WhichDoors = whichdoors;
	ShaftDoorFloor = floor;
	MoveDoors(false, manual);
}

void ElevatorDoor::StopDoors()
{
	//stops doors that are currently moving; can only be used for manual/emergency movements
	//this basically just resets the door internals

	if (AreDoorsMoving(2) == true)
	{
		if (WhichDoors == 3)
			car->Report("stopping shaft doors" + GetNumberText() + " on floor " + ToString(ShaftDoorFloor) + " (" + sbs->GetFloor(ShaftDoorFloor)->ID + ")");
		else
			car->Report("stopping doors" + GetNumberText());

		if (WhichDoors == 1 || WhichDoors == 2)
			Doors->StopDoors();
		if (WhichDoors == 1 || WhichDoors == 3)
		{
			if (index >= 0)
			{
				if (ShaftDoors[index])
					ShaftDoors[index]->StopDoors();
			}
		}

		DoorIsRunning = false;
		OpenDoor = 0;
		WhichDoors = 0;
		door_changed = false;
		doors_stopped = true;

		//disable nudge mode
		ResetNudgeTimer(false);
	}
	else if (AreDoorsMoving() == true)
		car->Report("can only stop doors" + GetNumberText() + " in manual/emergency mode");
	else
		car->Report("cannot stop doors" + GetNumberText() + "; no doors moving");
}

void ElevatorDoor::MoveDoors(bool open, bool manual)
{
	//opens or closes elevator doors

	//WhichDoors is the doors to move:
	//1 = both shaft and elevator doors
	//2 = only elevator doors
	//3 = only shaft doors

	//ShaftDoorFloor is the floor the shaft doors are on - only has effect if whichdoors is 3

	//this is the parent controller function that runs the movement function for each
	//individual door component.

	SBS_PROFILE("ElevatorDoor::MoveDoors");

	//find which doors should be moved
	bool elevdoors = false, shaftdoors = false, playsounds = false;
	if (WhichDoors == 1)
	{
		elevdoors = true;
		shaftdoors = true;
	}
	if (WhichDoors == 2)
		elevdoors = true;
	if (WhichDoors == 3)
		shaftdoors = true;

	if (DoorIsRunning == false || (manual == true && previous_open != open))
	{
		//initialization code

		//stop timer
		timer->Stop();

		DoorIsRunning = true;
		door_changed = false;

		index = GetIndex(ShaftDoorFloor);
		int index2 = GetManualIndex(ShaftDoorFloor);

		if (ShaftDoorsExist(ShaftDoorFloor) == false && index2 == -1)
		{
			if (WhichDoors != 2)
			{
				//reset and exit if shaft doors selected for opening
				OpenDoor = 0;
				WhichDoors = 0;
				DoorIsRunning = false;
				return;
			}
		}

		if (manual == false)
			playsounds = true;

		if (WhichDoors == 3)
		{
			//turn on related floor before opening shaft doors
			if (sbs->InShaft == true || sbs->InElevator == true)
			{
				sbs->GetFloor(ShaftDoorFloor)->Enabled(true);
				sbs->GetFloor(ShaftDoorFloor)->EnableGroup(true);
				sbs->EnableSkybox(true);
				sbs->EnableBuildings(true);
				sbs->EnableLandscape(true);
				sbs->EnableExternal(true);
			}
		}
		//reset finished states
		if (elevdoors == true)
			Doors->ResetFinished();

		if (shaftdoors == true && index >= 0)
		{
			if (ShaftDoors[index])
				ShaftDoors[index]->ResetFinished();
		}

		//play direction message sound
		if (car->MessageOnMove == false)
		{
			//play sound when doors start opening
			if (car->MessageOnStart == true && open == true && car->MessageOnClose == false)
				car->PlayMessageSound(true);

			//play sound when doors start closing
			if (car->MessageOnClose == true && open == false)
				car->PlayMessageSound(true);
		}

	}
	else if (previous_open != open && manual == false && door_changed == false)
	{
		//if a different direction was specified during movement
		door_changed = true;
		playsounds = true;
	}

	//reset values if power is off
	if (sbs->GetPower() == false && manual == false)
	{
		OpenDoor = 0;
		WhichDoors = 0;
		door_changed = false;
		doors_stopped = false;
	}

	//play door sounds
	if (playsounds == true)
	{
		if (open == true)
		{
			//play elevator opening sound
			doorsound->Load(OpenSound);
			doorsound->Play();
		}
		else
		{
			//play elevator closing sound
			doorsound->Load(CloseSound);
			doorsound->Play();
		}
		car->PlayMessageSound(false);
	}

	//perform door movement and get open state of each door
	if (elevdoors == true)
		Doors->MoveDoors(open, manual);

	if (shaftdoors == true && index >= 0)
	{
		if (ShaftDoors[index])
			ShaftDoors[index]->MoveDoors(open, manual);
	}

	//update call status (previous_open detects call changes during movement)
	previous_open = open;

	//wait until all door components are finished moving
	if (elevdoors == true)
	{
		Doors->CheckDoorsOpen();
		if (Doors->IsFinished() == false)
			return;
	}
	if (shaftdoors == true && index >= 0)
	{
		if (ShaftDoors[index])
		{
			ShaftDoors[index]->CheckDoorsOpen();
			if (ShaftDoors[index]->IsFinished() == false)
				return;
		}
	}

	//the doors are open or closed now
	if (WhichDoors == 3)
	{
		//turn off related floor

		if (open == false && (sbs->InShaft == true || sbs->InElevator == true))
		{
			if (elev->GetShaft()->ShowFloors == 0)
			{
				sbs->GetFloor(ShaftDoorFloor)->Enabled(false);
				sbs->GetFloor(ShaftDoorFloor)->EnableGroup(false);
			}
			else if (elev->GetShaft()->IsShowFloor(ShaftDoorFloor) == false)
			{
				sbs->GetFloor(ShaftDoorFloor)->Enabled(false);
				sbs->GetFloor(ShaftDoorFloor)->EnableGroup(false);
			}
		}
	}

	//reset values
	OpenDoor = 0;
	WhichDoors = 0;
	door_changed = false;
	doors_stopped = false;

	if (manual == false && open == true)
	{
		//reset and enable nudge timer
		if (elev->FireServicePhase2 == 0)
			ResetNudgeTimer();

		//turn on autoclose timer
		if ((elev->InServiceMode() == false || elev->WaitForDoors == true) &&
				(elev->UpPeak == false || ShaftDoorFloor != car->GetBottomFloor()) &&
				(elev->DownPeak == false || ShaftDoorFloor != car->GetTopFloor()))
		{
			if (IsSensorBlocked() == false)
				Reset();
			else
				car->Report("not resetting timer for door" + GetNumberText() + " due to blocked sensor");
		}

		//play late arrival notification if specified
		if (elev->NotifyLate == true)
			car->NotifyArrival(car->GetFloor());

		//play direction message sound
		if (car->MessageOnMove == false && car->MessageOnStart == false && car->MessageOnClose == false)
			car->PlayMessageSound(true);
	}
	else
	{
		//turn off nudge timer
		ResetNudgeTimer(false);
	}

	//deactivate sensor if doors are closed
	if (open == false)
		EnableSensor(false, false);

	DoorIsRunning = false;
}

DoorWrapper* ElevatorDoor::AddDoors(const std::string &lefttexture, const std::string &righttexture, Real thickness, Real CenterX, Real CenterZ, Real width, Real height, bool direction, Real tw, Real th)
{
	//adds elevator doors specified at a relative central position (off of elevator position)
	//if direction is false, doors are on the left/right side; otherwise front/back
	Real x1, x2, x3, x4;
	Real z1, z2, z3, z4;
	Real spacing = 0.01; //spacing between doors

	//set up coordinates
	if (direction == false)
	{
		x1 = CenterX;
		x2 = CenterX;
		x3 = CenterX;
		x4 = CenterX;
		z1 = CenterZ - (width / 2);
		z2 = CenterZ - spacing;
		z3 = CenterZ + spacing;
		z4 = CenterZ + (width / 2);
	}
	else
	{
		x1 = CenterX - (width / 2);
		x2 = CenterX - spacing;
		x3 = CenterX + spacing;
		x4 = CenterX + (width / 2);
		z1 = CenterZ;
		z2 = CenterZ;
		z3 = CenterZ;
		z4 = CenterZ;
	}

	//create left door
	AddDoorComponent("Left", lefttexture, lefttexture, thickness, "Left", OpenSpeed, OpenSpeed * 0.75, x1, z1, x2, z2, height, 0, tw, th, tw, th);

	//create right door
	AddDoorComponent("Right", righttexture, righttexture, thickness, "Right", OpenSpeed, OpenSpeed * 0.75, x3, z3, x4, z4, height, 0, tw, th, tw, th);

	//finish doors
	return FinishDoors();
}

DoorComponent* ElevatorDoor::AddDoorComponent(DoorWrapper *wrapper, const std::string &name, const std::string &meshname, const std::string &texture, const std::string &sidetexture, Real thickness, const std::string &direction, Real OpenSpeed, Real CloseSpeed, Real x1, Real z1, Real x2, Real z2, Real height, Real voffset, Real tw, Real th, Real side_tw, Real side_th)
{
	//creates a door component - finish with FinishDoor()

	//create door object
	DoorComponent *door = 0;
	if (wrapper->IsShaftDoor == false)
		door = wrapper->CreateDoor(meshname, direction, false, OpenSpeed, CloseSpeed, elev->GetDoorContainer());
	else
		door = wrapper->CreateDoor(meshname, direction, false, OpenSpeed, CloseSpeed, elev->GetShaft()->GetShaftDoorContainer());

	if (tw == 0)
		tw = 1;
	if (th == 0)
		th = 1;
	if (side_tw == 0)
		side_tw = 1;
	if (side_th == 0)
		side_th = 1;

	sbs->GetTextureManager()->ResetTextureMapping(true);

	PolyMesh* polymesh = sbs->GetPolyMesh();

	//add main walls
	polymesh->DrawWalls(true, true, false, false, false, false);
	Wall *wall;
	wall = door->mesh->CreateWallObject(name);
	polymesh->AddWallMain(wall, name, texture, thickness, x1, z1, x2, z2, height, height, voffset, voffset, tw, th, false);
	polymesh->ResetWalls();

	//add side walls
	polymesh->DrawWalls(false, false, true, true, true, true);
	wall = door->mesh->CreateWallObject(name);
	polymesh->AddWallMain(wall, name, sidetexture, thickness, x1, z1, x2, z2, height, height, voffset, voffset, side_tw, side_th, false);
	polymesh->ResetWalls();

	//store extents
	if (x1 < x2)
	{
		door->extents_min.x = x1;
		door->extents_max.x = x2;
	}
	else
	{
		door->extents_min.x = x2;
		door->extents_max.x = x1;
	}
	if (z1 < z2)
	{
		door->extents_min.z = z1;
		door->extents_max.z = z2;
	}
	else
	{
		door->extents_min.z = z2;
		door->extents_max.z = z1;
	}
	door->extents_min.y = voffset;
	door->extents_max.y = voffset + height;

	sbs->GetTextureManager()->ResetTextureMapping();

	return door;
}

DoorWrapper* ElevatorDoor::AddDoorComponent(const std::string &name, const std::string &texture, const std::string &sidetexture, Real thickness, const std::string &direction, Real OpenSpeed, Real CloseSpeed, Real x1, Real z1, Real x2, Real z2, Real height, Real voffset, Real tw, Real th, Real side_tw, Real side_th)
{
	//adds an elevator door component; remake of AddDoors command

	std::string Name, buffer;
	Name = name;
	TrimString(Name);
	buffer = "ElevatorDoor " + ToString(elev->Number) + ":" + ToString(car->Number) + ":" + ToString(Number) + ":" + Name;

	AddDoorComponent(Doors, name, buffer, texture, sidetexture, thickness, direction, OpenSpeed, CloseSpeed, x1, z1, x2, z2, height, voffset, tw, th, side_tw, side_th);
	return Doors;
}

DoorWrapper* ElevatorDoor::AddShaftDoorComponent(int floor, const std::string &name, const std::string &texture, const std::string &sidetexture, Real thickness, const std::string &direction, Real OpenSpeed, Real CloseSpeed, Real x1, Real z1, Real x2, Real z2, Real height, Real voffset, Real tw, Real th, Real side_tw, Real side_th)
{
	//adds a shaft door component; remake of AddShaftDoor command

	//exit if floor is not serviced by the elevator
	if (!car->IsServicedFloor(floor))
		return 0;

	int index = car->GetFloorIndex(floor);

	if (index == -1)
		return 0;

	//create shaft door wrapper if not already created
	if (!ShaftDoors[index])
	{
		if (elev->GetShaft()->GetLevel(floor))
			ShaftDoors[index] = new DoorWrapper(elev->GetShaft()->GetLevel(floor)->GetMeshObject(), this, true, floor);
	}

	std::string Name, buffer;
	Name = name;
	TrimString(Name);
	buffer = "Elevator " + ToString(elev->Number) + ": Shaft Door " + ToString(car->Number) + ":" + ToString(Number) + ":" + ToString(floor) + ":" + Name;

	AddDoorComponent(ShaftDoors[index], name, buffer, texture, sidetexture, thickness, direction, OpenSpeed, CloseSpeed, x1, z1, x2, z2, height, voffset, tw, th, side_tw, side_th);
	return ShaftDoors[index];
}

void ElevatorDoor::AddShaftDoorsComponent(const std::string &name, const std::string &texture, const std::string &sidetexture, Real thickness, const std::string &direction, Real OpenSpeed, Real CloseSpeed, Real x1, Real z1, Real x2, Real z2, Real height, Real voffset, Real tw, Real th, Real side_tw, Real side_th)
{
	//adds shaft door components for all serviced floors; remake of AddShaftDoors command

	//create doors
	for (size_t i = 0; i < car->ServicedFloors.size(); i++)
	{
		int floor = car->ServicedFloors[i];
		AddShaftDoorComponent(floor, name, texture, sidetexture, thickness, direction, OpenSpeed, CloseSpeed, x1, z1, x2, z2, height, voffset, tw, th, side_tw, side_th);
	}
}

DoorWrapper* ElevatorDoor::FinishDoors(DoorWrapper *wrapper, int floor, bool ShaftDoor, bool DoorWalls, bool TrackWalls)
{
	//finishes a door creation
	//DoorWalls determines if the connection walls (on the sides, and above the doors) should be made
	//TrackWalls determines if door track connection walls (below and above doors) should be made

	//add floor to manual shaft door list if wrapper doesn't exist and exit
	if (!wrapper)
	{
		if (ShaftDoor == true)
			ManualFloors.emplace_back(floor);
		return 0;
	}

	//exit if no doors exist
	if (wrapper->doors.empty() == true)
	{
		car->ReportError("FinishDoors: no door components have been created");
		return 0;
	}

	//get full width and height of doors
	Real x1 = 0, x2 = 0, y1 = 0, y2 = 0, z1 = 0, z2 = 0;
	bool firstrun = true;
	for (size_t i = 0; i < wrapper->doors.size(); i++)
	{
		for (int j = 1; j <= 3; j++)
		{
			Vector2 extents = wrapper->doors[i]->mesh->GetExtents(j, true);
			extents.x = sbs->ToLocal(extents.x);
			extents.y = sbs->ToLocal(extents.y);

			if (j == 1)
			{
				if (extents.x < x1 || firstrun == true)
					x1 = extents.x;
				if (extents.y > x2 || firstrun == true)
					x2 = extents.y;
			}
			if (j == 2)
			{
				if (extents.x < y1 || firstrun == true)
					y1 = extents.x;
				if (extents.y > y2 || firstrun == true)
					y2 = extents.y;
			}
			if (j == 3)
			{
				if (extents.x < z1 || firstrun == true)
					z1 = extents.x;
				if (extents.y > z2 || firstrun == true)
					z2 = extents.y;
				firstrun = false;
			}
		}
	}

	if (x2 - x1 > z2 - z1)
	{
		if (ShaftDoor == false)
			DoorDirection = true;
		wrapper->Width = x2 - x1;
		wrapper->Thickness = z2 - z1;
		wrapper->Shift = x2 - (wrapper->Width / 2);
	}
	else
	{
		if (ShaftDoor == false)
			DoorDirection = false;
		wrapper->Width = z2 - z1;
		wrapper->Thickness = x2 - x1;
		wrapper->Shift = z2 - (wrapper->Width / 2);
	}
	wrapper->Height = y2 - y1;
	if (y2 < y1)
		wrapper->voffset = y2;
	else
		wrapper->voffset = y1;

	Real base = wrapper->voffset;

	if (ShaftDoor == true)
	{
		//if shaft door, cut walls for door

		//create doors
		Shaft *shaft = elev->GetShaft();
		Floor *floorobj = sbs->GetFloor(floor);
		base += floorobj->GetBase(true);

		//cut shaft and floor walls
		sbs->GetPolyMesh()->ResetDoorwayWalls();
		if (DoorDirection == false)
		{
			if (shaft->GetLevel(floor))
			{
				if (!shaft->GetLevel(floor)->Cut(false, Vector3(elev->GetPosition().x + x1 - 2, base, elev->GetPosition().z + z1), Vector3(elev->GetPosition().x + x1 + 2, base + Doors->Height, elev->GetPosition().z + z2), true, false, 1))
					return 0;
			}
			floorobj->Cut(Vector3(elev->GetPosition().x + x1 - 2, base, elev->GetPosition().z + z1), Vector3(elev->GetPosition().x + x1 + 2, base + Doors->Height, elev->GetPosition().z + z2), true, false, true, 2);
		}
		else
		{
			if (shaft->GetLevel(floor))
			{
				if (!shaft->GetLevel(floor)->Cut(false, Vector3(elev->GetPosition().x + x1, base, elev->GetPosition().z + z1 - 2), Vector3(elev->GetPosition().x + x2, base + Doors->Height, elev->GetPosition().z + z1 + 2), true, false, 1))
					return 0;
			}
			floorobj->Cut(Vector3(elev->GetPosition().x + x1, base, elev->GetPosition().z + z1 - 2), Vector3(elev->GetPosition().x + x2, base + Doors->Height, elev->GetPosition().z + z1 + 2), true, false, true, 2);
		}

		//create doorway walls
		if (DoorWalls == true)
		{
			sbs->GetTextureManager()->ResetTextureMapping(true);
			sbs->GetPolyMesh()->AddDoorwayWalls(floorobj->Level, "Connection Walls", "ConnectionWall", 0, 0);
			sbs->GetTextureManager()->ResetTextureMapping();
		}
	}

	//create connection track walls
	if (TrackWalls == true)
	{
		sbs->GetTextureManager()->ResetTextureMapping(true);
		std::string name1, name2;
		PolyMesh* polymesh = sbs->GetPolyMesh();

		if (ShaftDoor == false)
		{
			name1 = "Door" + ToString(car->Number) + GetNumberText() + ":F1";
			name2 = "Door" + ToString(car->Number) + GetNumberText() + ":F2";
			polymesh->CreateWallBox(car->Mesh, name1, "Connection", x1, x2, z1, z2, 1, -1.001f + base, 0, 0, false, true, true, true, false);
			polymesh->CreateWallBox(car->Mesh, name2, "Connection", x1, x2, z1, z2, 1, wrapper->Height + 0.001f + base, 0, 0, false, true, true, true, false);
		}
		else
		{
			Shaft *shaft = elev->GetShaft();
			MeshObject *mesh = 0;
			if (shaft->GetLevel(floor))
				mesh = shaft->GetLevel(floor)->GetMeshObject();

			Vector3 position (car->GetPosition() - shaft->GetPosition());
			name1 = "ShaftDoor" + ToString(elev->Number) + ":" + ToString(car->Number) + ":" + ToString(Number) + ":F1";
			name2 = "ShaftDoor" + ToString(elev->Number) + ":" + ToString(car->Number) + ":" + ToString(Number) + ":F2";
			polymesh->CreateWallBox(mesh, name1, "Connection", position.x + x1, position.x + x2, position.z + z1, position.z + z2, 1, -1.001f + base, 0, 0, false, true, true, true, false);
			polymesh->CreateWallBox(mesh, name2, "Connection", position.x + x1, position.x + x2, position.z + z1, position.z + z2, 1, wrapper->Height + 0.001f + base, 0, 0, false, true, true, true, false);
		}

		sbs->GetTextureManager()->ResetTextureMapping();
	}

	Vector3 center;
	center.x = wrapper->GetPosition().x + x1 + ((x2 - x1) / 2);
	center.y = wrapper->GetPosition().y + (wrapper->Height / 2);
	center.z = wrapper->GetPosition().z + z1 + ((z2 - z1) / 2);

	//relocate sound object
	if (ShaftDoor == false)
	{
		doorsound->SetPosition(center);
		nudgesound->SetPosition(center);
	}
	else
	{
		chime->SetPosition(center);
		earlychime->SetPosition(center);
	}

	//create door sensor
	if (GetSensorStatus() == true && ShaftDoor == false && elev->AutoDoors == true)
	{
		Real sensor_width = sbs->camera->cfg_body_width / 4;
		Vector3 min (x1 - sensor_width, wrapper->voffset, z1 - sensor_width);
		Vector3 max (x2 + sensor_width, wrapper->voffset + wrapper->Height, z2 + sensor_width);
		CreateSensor(min, max);
	}

	return wrapper;
}

DoorWrapper* ElevatorDoor::FinishDoors(bool DoorWalls, bool TrackWalls)
{
	//finish elevator doors

	return FinishDoors(Doors, 0, false, DoorWalls, TrackWalls);
}

DoorWrapper* ElevatorDoor::FinishShaftDoor(int floor, bool DoorWalls, bool TrackWalls)
{
	//finish shaft door on a specified floor

	std::string floornum = ToString(floor);

	//exit if floor is not serviced by the elevator
	if (!car->IsServicedFloor(floor))
	{
		car->ReportError("Floor " + floornum + " not a serviced floor");
		return 0;
	}

	if (!sbs->GetFloor(floor))
	{
		car->ReportError("Floor " + floornum + " does not exist");
		return 0;
	}

	DoorWrapper *wrapper = 0;
	int index = car->GetFloorIndex(floor);

	if (index > -1)
		wrapper = ShaftDoors[index];

	return FinishDoors(wrapper, floor, true, DoorWalls, TrackWalls);
}

bool ElevatorDoor::FinishShaftDoors(bool DoorWalls, bool TrackWalls)
{
	//finish all shaft doors

	for (size_t i = 0; i < car->ServicedFloors.size(); i++)
		FinishShaftDoor(car->ServicedFloors[i], DoorWalls, TrackWalls);

	return true;
}

bool ElevatorDoor::AddShaftDoors(Real rotation, const std::string &lefttexture, const std::string &righttexture, Real thickness, Real CenterX, Real CenterZ, Real voffset, Real tw, Real th)
{
	//adds shaft's elevator doors specified at a relative central position (off of elevator origin)
	//uses some parameters (width, height, direction) from AddDoor/AddDoors function

	//set door parameters
	ShaftDoorOrigin = Vector3(CenterX, 0, CenterZ);
	ShaftDoorThickness = thickness;

	//create doors
	for (size_t i = 0; i < car->ServicedFloors.size(); i++)
	{
		if (!AddShaftDoor(car->ServicedFloors[i], rotation, lefttexture, righttexture, ShaftDoorThickness, ShaftDoorOrigin.x, ShaftDoorOrigin.z, voffset, tw, th))
			return false;
	}

	return true;
}

DoorWrapper* ElevatorDoor::AddShaftDoor(int floor, Real rotation, const std::string &lefttexture, const std::string &righttexture, Real tw, Real th)
{
	//compatibility version of AddShaftDoor; please use newer implementation instead

	return AddShaftDoor(floor, rotation, lefttexture, righttexture, ShaftDoorThickness, ShaftDoorOrigin.x, ShaftDoorOrigin.z, 0, tw, th);
}

DoorWrapper* ElevatorDoor::AddShaftDoor(int floor, Real rotation, const std::string &lefttexture, const std::string &righttexture, Real thickness, Real CenterX, Real CenterZ, Real voffset, Real tw, Real th)
{
	//adds a single elevator shaft door, with position and thickness parameters first specified
	//by the SetShaftDoors command.
	//uses some parameters (width, height, direction) from AddDoor/AddDoors function

	//exit if floor is not serviced by the elevator
	if (!car->IsServicedFloor(floor))
		return 0;

	Real x1, x2, x3, x4;
	Real z1, z2, z3, z4;
	int index = car->GetFloorIndex(floor);

	if (index == -1)
		return 0;

	//set up coordinates
	if (DoorDirection == false)
	{
		x1 = CenterX;
		x2 = CenterX;
		x3 = CenterX;
		x4 = CenterX;
		z1 = CenterZ - (Doors->Width / 2);
		z2 = CenterZ;
		z3 = CenterZ;
		z4 = CenterZ + (Doors->Width / 2);
	}
	else
	{
		x1 = CenterX - (Doors->Width / 2);
		x2 = CenterX;
		x3 = CenterX;
		x4 = CenterX + (Doors->Width / 2);
		z1 = CenterZ;
		z2 = CenterZ;
		z3 = CenterZ;
		z4 = CenterZ;
	}

	//create doors

	//create left door
	AddShaftDoorComponent(floor, "Left", lefttexture, lefttexture, thickness, "Left", OpenSpeed, OpenSpeed * 0.75, x1, z1, x2, z2, Doors->Height, voffset, tw, th, tw, th);

	//create right door
	AddShaftDoorComponent(floor, "Right", righttexture, righttexture, thickness, "Right", OpenSpeed, OpenSpeed * 0.75, x3, z3, x4, z4, Doors->Height, voffset, tw, th, tw, th);

	//finish doors
	DoorWrapper *wrapper = FinishShaftDoor(floor);
	wrapper->Rotate(Vector3(0, rotation, 0));
	return wrapper;
}

void ElevatorDoor::SetShaftDoors(Real thickness, Real CenterX, Real CenterZ)
{
	//notice - this function is deprecated - use newer AddShaftDoor command instead

	//pre-set shaft door parameters for the AddShaftDoor command.
	//not needed if using the AddShaftDoors command
	//the Center values are relative offsets from the associated elevator's center

	ShaftDoorThickness = thickness;
	ShaftDoorOrigin = Vector3(CenterX, 0, CenterZ);
}

void ElevatorDoor::ShaftDoorsEnabled(int floor, bool value)
{
	//turns shaft elevator doors on/off

	SBS_PROFILE("ElevatorDoor::ShaftDoorsEnabled");

	//exit if shaft's ShowFullShaft is set
	if (elev->GetShaft()->GetShowFull() == true && value == false)
		return;

	//exit if elevator doesn't service the requested floor
	int index = GetIndex(floor);

	if (index == -1)
		return;

	//exit if the specified floor has no shaft doors
	if (ShaftDoorsExist(floor) == false)
		return;

	ShaftDoors[index]->Enabled(value);
}

void ElevatorDoor::ShaftDoorsEnabledRange(int floor, int range)
{
	//turn on a range of floors
	//if range is 3, show shaft door on current floor (floor), and 1 floor below and above (3 total floors)
	//if range is 1, show door on only the current floor (floor)

	SBS_PROFILE("ElevatorDoor::ShaftDoorsEnabledRange");

	//exit if shaft's ShowFullShaft is set
	if (elev->GetShaft()->GetShowFull() == true)
		return;

	//range must be greater than 0
	if (range < 1)
		range = 1;

	//range must be an odd number; if it's even, then add 1
	if (IsEven(range) == true)
		range++;

	int additionalfloors;
	if (range > 1)
		additionalfloors = (range - 1) / 2;
	else
		additionalfloors = 0;

	//disable doors 1 floor outside of range
	ShaftDoorsEnabled(floor - additionalfloors - 1, false);
	ShaftDoorsEnabled(floor + additionalfloors + 1, false);

	//enable doors within range
	for (int i = floor - additionalfloors; i <= floor + additionalfloors; i++)
		ShaftDoorsEnabled(i, true);
}

bool ElevatorDoor::AreDoorsOpen()
{
	//returns the internal door state
	return Doors->Open;
}

bool ElevatorDoor::AreShaftDoorsOpen(int floor)
{
	//returns the internal door state
	if (ShaftDoorsExist(floor))
	{
		int index = GetIndex(floor);

		if (index > -1)
		{
			if (ShaftDoors[index])
				return ShaftDoors[index]->Open;
		}
	}
	return false;
}

bool ElevatorDoor::AreShaftDoorsClosed(bool skip_current_floor)
{
	//returns true if all shaft doors are closed and not moving

	if (AreDoorsMoving() == true && skip_current_floor == false)
		return false;

	for (size_t i = 0; i < ShaftDoors.size(); i++)
	{
		DoorWrapper* door = ShaftDoors[i];

		if (door)
		{
			if (skip_current_floor == true)
			{
				//skip elevator's floor if specified
				if (door->floor == car->GetFloor())
					continue;
			}

			if (door->Open == true)
				return false;
		}
	}
	return true;
}

void ElevatorDoor::Timer::Notify()
{
	if (sbs->GetPower() == false)
		return;

	if (type == 0)
	{
		//door autoclose timer

		//close doors if open
		if (door->AreDoorsOpen() == true && (elevator->InServiceMode() == false || elevator->WaitForDoors == true || car->IndependentServiceOnOtherCar() == true))
			door->CloseDoors();
	}
	if (type == 1)
	{
		//nudge mode timer
		door->EnableNudgeMode(true);
	}
}

void ElevatorDoor::Chime(int floor, bool direction)
{
	if (sbs->GetPower() == false)
		return;

	//play chime sound on specified floor
	if (direction == false && chimesound_loaded != -1)
	{
		chime->Load(DownChimeSound);
		chimesound_loaded = -1;
	}
	else if (direction == true && chimesound_loaded != 1)
	{
		chime->Load(UpChimeSound);
		chimesound_loaded = 1;
	}
	chime->SetLoopState(false);
	chime->SetPositionY(sbs->GetFloor(floor)->GetBase() + Doors->Height);
	chime->Play();
}

void ElevatorDoor::EarlyChime(int floor, bool direction)
{
	if (sbs->GetPower() == false)
		return;

	//play chime sound on specified floor
	if (direction == false && earlychimesound_loaded != -1)
	{
		if (EarlyDownSet == true)
			earlychime->Load(EarlyDownChimeSound);
		else
			earlychime->Load(DownChimeSound);
		earlychimesound_loaded = -1;
	}
	else if (direction == true && earlychimesound_loaded != 1)
	{
		if (EarlyUpSet == true)
			earlychime->Load(EarlyUpChimeSound);
		else
			earlychime->Load(UpChimeSound);
		earlychimesound_loaded = 1;
	}
	earlychime->SetLoopState(false);
	earlychime->SetPositionY(sbs->GetFloor(floor)->GetBase() + Doors->Height);
	earlychime->Play();
}

void ElevatorDoor::Reset(bool sensor)
{
	//reset elevator doors (timer and optionally sensor)

	if (elev->AutoDoors == false)
		return;

	if (sbs->Verbose == true)
	{
		if (sensor == true)
			car->Report("sensor resetting doors" + GetNumberText());
		else
			car->Report("resetting doors" + GetNumberText());
	}

	if (quick_close == false)
	{
		if (DoorTimer > 0)
			timer->Start(DoorTimer, true);
	}
	else
	{
		if (QuickClose > 0)
			timer->Start(QuickClose, true);
	}
	quick_close = false;

	if (sensor == false)
		EnableSensor(true, false);
}

bool ElevatorDoor::TimerIsRunning()
{
	//return status of timer
	return timer->IsRunning();
}

bool ElevatorDoor::DoorsStopped()
{
	return doors_stopped;
}

bool ElevatorDoor::Loop()
{
	//main loop
	SBS_PROFILE("ElevatorDoor::Loop");

	if (sbs->GetPower() == false)
		nudgesound->Stop();

	if (OpenDoor == 1)
		MoveDoors(true, false);
	if (OpenDoor == -1)
		MoveDoors(false, false);
	if (OpenDoor == 2)
		MoveDoors(true, true);
	if (OpenDoor == -2)
		MoveDoors(false, true);

	return true;
}

bool ElevatorDoor::Enabled(bool value)
{
	return Doors->Enabled(value);
}

bool ElevatorDoor::IsEnabled()
{
	//are doors enabled?
	return Doors->IsEnabled();
}

int ElevatorDoor::GetWhichDoors()
{
	//return value of WhichDoors
	return WhichDoors;
}

Real ElevatorDoor::GetShaftDoorAltitude(int floor)
{
	//returns altitude of the shaft door on the specified floor

	int index = GetIndex(floor);

	if (index == -1)
		return 0;

	DoorWrapper *wrapper = ShaftDoors[index];

	if (!wrapper)
		return 0;

	return wrapper->GetPosition().y + wrapper->voffset;
}

void ElevatorDoor::MoveSound(const Vector3 &position, bool relative_x, bool relative_y, bool relative_z)
{
	//move sound
	Vector3 pos = doorsound->GetPosition();
	if (relative_x == false)
		pos.x = position.x;
	else
		pos.x += position.x;
	if (relative_y == false)
		pos.y = position.y + (Doors->Height / 2);
	else
		pos.y += position.y;
	if (relative_z == false)
		pos.z = position.z;
	else
		pos.z += position.z;
	doorsound->SetPosition(pos);
	nudgesound->SetPosition(pos);
}

bool ElevatorDoor::ShaftDoorsExist(int floor, bool include_nonserviced)
{
	//return true if shaft doors have been created for this door on the specified floor

	int index = GetIndex(floor);

	if (index != -1)
	{
		//if shaft doors exist on a serviced floor
		if (ShaftDoors[index])
		{
			if (ShaftDoors[index]->doors.size() > 0)
				return true;
		}
	}
	else if (include_nonserviced == true)
	{
		//if shaft doors exist on a non-serviced floor
		for (size_t i = 0; i < ShaftDoors.size(); i++)
		{
			if (ShaftDoors[i])
			{
				if (ShaftDoors[i]->floor == floor)
					return true;
			}
		}
	}
	return false;
}

DoorWrapper* ElevatorDoor::GetDoorWrapper()
{
	//return door wrapper object
	return Doors;
}

DoorWrapper* ElevatorDoor::GetShaftDoorWrapper(int floor)
{
	//return shaft door wrapper object for the specified floor

	int index = GetIndex(floor);

	if (index == -1)
		return 0;

	return ShaftDoors[index];
}

int ElevatorDoor::GetManualIndex(int floor)
{
	//return manual array index of the specified floor

	for (size_t i = 0; i < ManualFloors.size(); i++)
	{
		if (ManualFloors[i] == floor)
			return (int)i;
	}

	return -1;
}

void ElevatorDoor::Hold(bool sensor)
{
	//hold door (basically turn off timer)
	//disable nudge mode timer if specified

	if (sbs->GetPower() == false)
		return;

	//exit if timer is already stopped
	if (timer->IsRunning() == false)
		return;

	//exit if nudge mode is active
	if (GetNudgeStatus() == true)
		return;

	if (sensor == true)
		car->Report("sensor holding doors" + GetNumberText());
	else
		car->Report("holding doors" + GetNumberText());

	timer->Stop();

	if (sensor == false)
		EnableSensor(false, false);
}

void ElevatorDoor::EnableNudgeMode(bool value)
{
	//enable or disable nudge mode

	if (sbs->GetPower() == false)
		return;

	if (value == true && AllowNudgeMode() == true)
	{
		car->Report("Doors" + GetNumberText() + ": nudge mode activated");
		nudge_enabled = true;
		if (nudgesound_loaded == false)
			nudgesound->Load(NudgeSound);
		nudgesound_loaded = true;
		nudgesound->SetLoopState(true);
		nudgesound->Play();
		CloseDoors();
		car->FlashIndicators(true);
	}
	else if (nudge_enabled == true)
	{
		car->Report("Doors" + GetNumberText() + ": nudge mode disabled");
		nudge_enabled = false;
		nudgesound->Stop();
		car->FlashIndicators(false);
	}
}

bool ElevatorDoor::GetNudgeStatus()
{
	//get status of nudge mode

	return nudge_enabled;
}

void ElevatorDoor::CheckSensor()
{
	if (sbs->GetPower() == false)
		return;

	if (GetSensorStatus(false) == true && sensor && (AreDoorsOpen() == true || AreDoorsMoving() == true) && elev->PeakWaiting() == false)
		sensor->Loop();
}

void ElevatorDoor::CreateSensor(Vector3 &area_min, Vector3 &area_max)
{
	//create action for elevator door
	std::string action_name1 = "sensor " + ToString(Number);
	std::string action_name2 = "sensorreset " + ToString(Number);
	std::string full_name1 = car->GetName() + ":" + action_name1;
	std::string full_name2 = car->GetName() + ":" + action_name2;

	std::vector<Object*> parents;
	parents.emplace_back(car);
	sensor_action = sbs->AddAction(full_name1, parents, action_name1);
	reset_action = sbs->AddAction(full_name2, parents, action_name2);

	std::vector<std::string> actions;
	actions.emplace_back(full_name2);
	actions.emplace_back(full_name1);

	//create new trigger
	sensor = new Trigger(this, "Sensor", true, SensorSound, area_min, area_max, actions);
}

bool ElevatorDoor::AreDoorsMoving(int doors, bool car_doors, bool shaft_doors)
{
	//return true if doors are moving

	//if doors is 0, check if doors are moving normally or manually
	//if doors is 1, check if doors are moving normally
	//if doors is 2, check if doors are moving manually

	bool result = false;

	if (doors == 0)
		result = (OpenDoor != 0);
	if (doors == 1)
		result = (abs(OpenDoor) == 1);
	if (doors == 2)
		result = (abs(OpenDoor) == 2);

	if (car_doors == true && (WhichDoors == 1 || WhichDoors == 2))
		return result;

	if (shaft_doors == true && (WhichDoors == 1 || WhichDoors == 3))
		return result;

	return false;
}

void ElevatorDoor::EnableSensor(bool value, bool persistent)
{
	//enable or disable door sensor

	//set persistent to false if sensor should be temporarily activated/deactivated only
	//and persistent state left untouched

	if (sbs->GetPower() == false)
		return;

	if (value == sensor_enabled)
		return;

	//if deactivate_only is true, perform operation only is persistent state (sensor_status) is true
	if (persistent == false && sensor_status == false)
		return;

	//only enable sensor if using automatic doors
	if (elev->AutoDoors == false)
		return;

	//if not temporary, change persistent status
	if (persistent == true)
	{
		if (sbs->IsRunning == true || sbs->Verbose)
		{
			if (value == true)
				car->Report("Doors" + GetNumberText() + ": enabling sensor");
			else
				car->Report("Doors" + GetNumberText() + ": disabling sensor");
		}

		sensor_status = value;
	}

	if (sbs->IsRunning == true || sbs->Verbose)
	{
		if (value == true)
			car->Report("Doors" + GetNumberText() + ": activating sensor");
		else
			car->Report("Doors" + GetNumberText() + ": deactivating sensor");
	}

	sensor_enabled = value;
}

bool ElevatorDoor::GetSensorStatus(bool persistent)
{
	//get status of door sensor

	//if persistent is true, return persistent state (enabled/disabled)
	//if persistent is false, return temporary state (activated/deactivated)

	if (persistent == false)
		return sensor_enabled;

	return sensor_status;
}

bool ElevatorDoor::IsSensorBlocked()
{
	//return true if sensor is currently blocked/triggered

	if (sensor)
		return sensor->IsInside();

	return false;
}

bool ElevatorDoor::GetHoldStatus()
{
	//returns true if door hold is activated (doors are open and timer has been disabled)
	if (AreDoorsOpen() == true && AreDoorsMoving() == false && TimerIsRunning() == false)
		return true;

	return false;
}

void ElevatorDoor::ResetNudgeTimer(bool start)
{
	if (sbs->GetPower() == false)
		return;

	if (AreDoorsOpen() == false || doors_stopped == true)
	{
		//switch off nudge mode timer if on
		if (nudgetimer->IsRunning() == true)
			nudgetimer->Stop();

		//switch off nudge mode if on
		if (GetNudgeStatus() == true)
			EnableNudgeMode(false);
	}

	//turn on nudge mode timer if doors are open
	if (start == true)
	{
		if (AllowNudgeMode() == true && NudgeTimer > 0 && nudgetimer->IsRunning() == false)
			nudgetimer->Start(int(NudgeTimer * 1000), true);
	}
}

bool ElevatorDoor::AllowNudgeMode()
{
	//return true if current situation allows nudge mode to be enabled

	//allow nudge mode if fire phase 1 is on (phase 2 is not on Hold) and elevator is not at recall floor
	bool firelobby = elev->FireServicePhase1 == 1 && elev->FireServicePhase2 != 2 && elev->OnRecallFloor() == false;

	if (GetNudgeStatus() == false && AreDoorsOpen() == true && elev->AutoDoors == true && (elev->InServiceMode() == false || firelobby == true))
	{
		if ((elev->UpPeak == true && car->GetFloor() == car->GetBottomFloor()) || (elev->DownPeak == true && car->GetFloor() == car->GetTopFloor()))
			return false;

		return true;
	}
	return false;
}

std::string ElevatorDoor::GetNumberText()
{
	//return a text string representing the door number, or blank if it's the only door

	std::string doornumber;
	if (car->NumDoors > 1)
		doornumber = " " + ToString(Number);

	return doornumber;
}

void ElevatorDoor::ResetState()
{
	//reset door state in case of an internal malfunction

	OpenDoor = 0;
	WhichDoors = 0;
	door_changed = false;
	doors_stopped = false;
	DoorIsRunning = false;

	//first reset timers
	timer->Stop();
	nudgetimer->Stop();
	EnableNudgeMode(false);

	Doors->ResetState();

	for (size_t i = 0; i < ShaftDoors.size(); i++)
	{
		if (ShaftDoors[i])
			ShaftDoors[i]->ResetState();
	}
}

void ElevatorDoor::RemoveShaftDoor(DoorWrapper *door)
{
	if (door)
	{
		for (size_t i = 0; i < ShaftDoors.size(); i++)
		{
			if (ShaftDoors[i] == door)
			{
				ShaftDoors[i] = 0;
				return;
			}
		}
	}
}

bool ElevatorDoor::GetDoorChanged()
{
	return door_changed;
}

bool ElevatorDoor::GetPreviousOpen()
{
	return previous_open;
}

int ElevatorDoor::GetIndex(int floor)
{
	for (size_t i = 0; i < ShaftDoors.size(); i++)
	{
		if (ShaftDoors[i])
		{
			if (ShaftDoors[i]->floor == floor)
				return (int)i;
		}
	}
	return -1;
}

}
