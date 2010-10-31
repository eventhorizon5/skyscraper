/* $Id$ */

/*
	Scalable Building Simulator - Elevator Door Class
	The Skyscraper Project - Version 1.8 Alpha
	Copyright (C)2004-2010 Ryan Thoryk
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
#include "elevatordoor.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

ElevatorDoor::ElevatorDoor(int number, Elevator* elevator)
{
	//set up SBS object
	object = new Object();
	object->SetValues(this, elevator->object, "ElevatorDoor", "", false);

	Ogre::String buffer;
	buffer = number;
	object->SetName(Ogre::String("Elevator Door " + buffer).c_str());

	//create a new elevator door
	Number = number + 1;
	elev = elevator;
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
	NudgeSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.Door.NudgeSound", "buzz.wav");
	doors_stopped = false;
	ShaftDoorThickness = 0;
	ShaftDoorOrigin = 0;
	nudge_enabled = false;
	ManualSpeed = sbs->GetConfigFloat("Skyscraper.SBS.Elevator.Door.ManualSpeed", 0.2);
	SlowSpeed = sbs->GetConfigFloat("Skyscraper.SBS.Elevator.Door.SlowSpeed", 0.5);
	QuickClose = sbs->GetConfigInt("Skyscraper.SBS.Elevator.Door.QuickClose", 3000);
	NudgeTimer = sbs->GetConfigFloat("Skyscraper.SBS.Elevator.Door.NudgeTimer", 30);
	nudgesound_loaded = false;
	chimesound_loaded = 0;

	//create main door object
	Doors = new DoorWrapper(this, false);

	//create timers
	timer = new Timer(this, elev, 0);
	nudgetimer = new Timer(this, elev, 1);

	//create shaft door objects
	ShaftDoors.resize(elev->ServicedFloors.size());
	for (int i = 0; i < ShaftDoors.size(); i++)
		ShaftDoors[i] = new DoorWrapper(this, true);

	//create sound object
	doorsound = new Sound(this->object, "Door Sound", true);
	doorsound->SetPosition(elevator->Origin);
	chime = new Sound(this->object, "Chime", true);
	nudgesound = new Sound(this->object, "Nudge Sound", true);
	nudgesound->SetPosition(elevator->Origin);
}

ElevatorDoor::~ElevatorDoor()
{
	//delete shaft door objects
	for (int i = 0; i < ShaftDoors.size(); i++)
	{
		if (ShaftDoors[i])
		{
			ShaftDoors[i]->object->parent_deleting = true;
			delete ShaftDoors[i];
		}
		ShaftDoors[i] = 0;
	}

	//Destructor
	if (timer)
	{
		timer->Stop();
		delete timer;
	}
	timer = 0;
	if (nudgetimer)
	{
		nudgetimer->Stop();
		delete nudgetimer;
	}
	nudgetimer = 0;

	if (doorsound)
	{
		doorsound->object->parent_deleting = true;
		delete doorsound;
	}
	doorsound = 0;
	if (chime)
	{
		chime->object->parent_deleting = true;
		delete chime;
	}
	chime = 0;
	if (nudgesound)
	{
		nudgesound->object->parent_deleting = true;
		delete nudgesound;
	}
	nudgesound = 0;

	//delete main doors
	if (Doors)
	{
		Doors->object->parent_deleting = true;
		delete Doors;
	}
	Doors = 0;

	//unregister from parent
	if (object->parent_deleting == false)
		elev->RemoveElevatorDoor(this);

	delete object;
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

	Ogre::String doornumber;
	if (elev->NumDoors > 1)
	{
		doornumber = " ";
		doornumber = doornumber + _itoa(Number, intbuffer, 10);
	}

	//exit if trying to open doors while stopped
	if (manual == false && doors_stopped == true)
	{
		sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + ": cannot open doors" + doornumber + "; doors manually stopped");
		return;
	}

	//exit if in nudge mode
	if (GetNudgeStatus() == true)
	{
		sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + ": cannot open doors" + doornumber + "; nudge mode enabled");
		return;
	}

	//exit if direction changed (if doors have already switched from close to open)
	if (door_changed == true)
		return;

	//don't open doors if emergency stop is enabled
	if (elev->OnFloor == false && whichdoors != 3 && manual == false)
	{
		sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + ": cannot open doors" + doornumber + "; emergency stop enabled");
		return;
	}

	//check if elevator doors are already open
	if (Doors->Open == true && whichdoors != 3 && OpenDoor == 0 && doors_stopped == false)
	{
		//reset timer if not in a service mode
		if (elev->InServiceMode() == false)
		{
			sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + ": doors" + doornumber + " already open; resetting timer");
			ResetDoorTimer();
		}
		else
			sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + ": doors" + doornumber + " already open");
		return;
	}

	//exit if doors are manually opening
	if (OpenDoor == 2 && DoorIsRunning == true)
	{
		sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + ": doors" + doornumber + " in use");
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
			sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + " Doors" + doornumber + ": invalid shaft doors");
			return;
		}
		if (AreShaftDoorsOpen(floor) == true)
		{
			sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + ": shaft doors" + doornumber + " already open on floor " + Ogre::String(_itoa(floor, intbuffer, 10)) + " (" + sbs->GetFloor(floor)->ID + ")");
			return;
		}
		else if (manual == false)
			sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + ": opening shaft doors" + doornumber + " on floor " + Ogre::String(_itoa(floor, intbuffer, 10)) + " (" + sbs->GetFloor(floor)->ID + ")");
		else
			sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + ": manually opening shaft doors" + doornumber + " on floor " + Ogre::String(_itoa(floor, intbuffer, 10)) + " (" + sbs->GetFloor(floor)->ID + ")");
	}
	else if (manual == false)
		sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + ": opening doors" + doornumber);
	else
		sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + ": manually opening doors" + doornumber);

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
			floor = elev->GotoFloor;
		else
			floor = elev->GetFloor();
	}

	//if opening both doors, exit if shaft doors don't exist
	int index = -1;
	for (int i = 0; i < ManualFloors.size(); i++)
	{
		if (ManualFloors[i] == floor)
			index = i;
	}
	if (whichdoors == 1 && ShaftDoorsExist(floor) == false && index == -1)
	{
		sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + ": can't open doors" + doornumber + " - no shaft doors on " + Ogre::String(_itoa(floor, intbuffer, 10)));
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

	Ogre::String doornumber;
	if (elev->NumDoors > 1)
	{
		doornumber = " ";
		doornumber = doornumber + _itoa(Number, intbuffer, 10);
	}

	//exit if trying to open doors while stopped
	if (manual == false && doors_stopped == true)
	{
		sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + ": cannot close doors" + doornumber + "; doors manually stopped");
		return;
	}

	//do not close doors while fire service mode 1 is on and the elevator is waiting at the parking floor
	if (manual == false && elev->FireServicePhase1 == 1 && elev->WaitForDoors == false && elev->GetFloor() == elev->ParkingFloor)
	{
		sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + ": cannot close doors" + doornumber + " while Fire Service Phase 1 is on");
		return;
	}

	//do not close doors while fire service mode 2 is set to hold
	if (manual == false && elev->FireServicePhase2 == 2)
	{
		sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + ": cannot close doors" + doornumber + " while Fire Service Phase 2 is set to hold");
		return;
	}

	//if called while doors are opening, set quick_close (causes door timer to trigger faster)
	if (OpenDoor != 0 && manual == false)
	{
		sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + ": will close doors" + doornumber + " in quick-close mode");
		quick_close = true;
		return;
	}

	//check if elevator doors are already closed
	if (Doors->Open == false && whichdoors != 3 && OpenDoor == 0 && doors_stopped == false)
	{
		sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + ": doors" + doornumber + " already closed");
		return;
	}

	//exit if doors are manually closing
	if (OpenDoor == -2 && DoorIsRunning == true)
	{
		sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + ": doors" + doornumber + " in use");
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
			sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + " Doors" + doornumber + ": invalid shaft doors");
			return;
		}
		if (AreShaftDoorsOpen(floor) == false && whichdoors == 3)
		{
			sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + ": shaft doors" + doornumber + "already closed on floor " + Ogre::String(_itoa(floor, intbuffer, 10)) + " (" + sbs->GetFloor(floor)->ID + ")");
			return;
		}
		else if (manual == false)
			sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + ": closing shaft doors" + doornumber + " on floor " + Ogre::String(_itoa(floor, intbuffer, 10)) + " (" + sbs->GetFloor(floor)->ID + ")");
		else
			sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + ": manually closing shaft doors" + doornumber + " on floor " + Ogre::String(_itoa(floor, intbuffer, 10)) + " (" + sbs->GetFloor(floor)->ID + ")");
	}
	else if (manual == false)
		sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + ": closing doors" + doornumber);
	else
		sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + ": manually closing doors" + doornumber);

	if (manual == false)
		OpenDoor = -1;
	else
		OpenDoor = -2;

	//if both doors are selected but elevator is not on a floor, only close elevator doors
	if (whichdoors == 1 && elev->OnFloor == false)
		whichdoors = 2;

	if (whichdoors != 3)
		floor = elev->GetFloor();

	//if closing both doors, exit if shaft doors don't exist
	int index = -1;
	for (int i = 0; i < ManualFloors.size(); i++)
	{
		if (ManualFloors[i] == floor)
			index = i;
	}
	if (whichdoors == 1 && ShaftDoorsExist(floor) == false && index == -1)
	{
		sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + ": can't close doors" + doornumber + " - no shaft doors on " + Ogre::String(_itoa(floor, intbuffer, 10)));
		OpenDoor = 0;
		return;
	}

	//turn off directional indicators
	if (whichdoors == 1)
		sbs->GetFloor(floor)->SetDirectionalIndicators(elev->Number, false, false);
	elev->SetDirectionalIndicators(false, false);

	WhichDoors = whichdoors;
	ShaftDoorFloor = floor;
	MoveDoors(false, manual);
}

void ElevatorDoor::StopDoors()
{
	//stops doors that are currently moving; can only be used for manual/emergency movements
	//this basically just resets the door internals

	Ogre::String doornumber;
	if (elev->NumDoors > 1)
	{
		doornumber = " ";
		doornumber = doornumber + _itoa(Number, intbuffer, 10);
	}

	if (OpenDoor == -2 || OpenDoor == 2)
	{
		if (WhichDoors == 3)
			sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + ": stopping shaft doors" + doornumber + " on floor " + Ogre::String(_itoa(ShaftDoorFloor, intbuffer, 10)) + " (" + sbs->GetFloor(ShaftDoorFloor)->ID + ")");
		else
			sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + ": stopping doors" + doornumber);

		if (WhichDoors == 1 || WhichDoors == 2)
			Doors->StopDoors();
		if (WhichDoors == 1 || WhichDoors == 3)
			ShaftDoors[index]->StopDoors();

		DoorIsRunning = false;
		OpenDoor = 0;
		WhichDoors = 0;
		door_changed = false;
		doors_stopped = true;
	}
	else if (OpenDoor != 0)
		sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + ": can only stop doors" + doornumber + " in manual/emergency mode");
	else
		sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + ": cannot stop doors" + doornumber + "; no doors moving");
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

	//find which doors should be moved
	bool elevdoors = false, shaftdoors = false;
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

		index = -1;
		for (int i = 0; i < elev->ServicedFloors.size(); i++)
		{
			if (elev->ServicedFloors[i] == ShaftDoorFloor)
				index = i;
		}
		int index2 = -1;
		for (int i = 0; i < ManualFloors.size(); i++)
		{
			if (ManualFloors[i] == ShaftDoorFloor)
				index2 = i;
		}
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
		{
			if (open == true)
			{
				//play elevator opening sound
				doorsound->Load(OpenSound.c_str());
				doorsound->Play();
			}
			else
			{
				//play elevator closing sound
				doorsound->Load(CloseSound.c_str());
				doorsound->Play();
			}
		}

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
		{
			for (int i = 0; i < Doors->doors.size(); i++)
			{
				Doors->doors[i]->finished = false;
			}
		}

		if (shaftdoors == true)
		{
			for (int i = 0; i < ShaftDoors[index]->doors.size(); i++)
			{
				ShaftDoors[index]->doors[i]->finished = false;
			}
		}
	}
	else if (previous_open != open && manual == false && door_changed == false)
	{
		//if a different direction was specified during movement
		door_changed = true;
	}

	//perform door movement and get open state of each door
	if (elevdoors == true)
	{
		for (int i = 0; i < Doors->doors.size(); i++)
		{
			Doors->doors[i]->MoveDoors(open, manual);
		}
	}

	if (shaftdoors == true)
	{
		for (int i = 0; i < ShaftDoors[index]->doors.size(); i++)
		{
			ShaftDoors[index]->doors[i]->MoveDoors(open, manual);
		}
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
	if (shaftdoors == true)
	{
		ShaftDoors[index]->CheckDoorsOpen();
		if (ShaftDoors[index]->IsFinished() == false)
			return;
	}

	//the doors are open or closed now
	if (WhichDoors == 3)
	{
		//turn off related floor
		if (open == false && (sbs->InShaft == true || sbs->InElevator == true))
		{
			sbs->GetFloor(ShaftDoorFloor)->Enabled(false);
			sbs->GetFloor(ShaftDoorFloor)->EnableGroup(false);
		}
	}

	//switch off nudge mode timer if on
	if (open == false && nudgetimer->IsRunning() == true)
		nudgetimer->Stop();

	//switch off nudge mode if on
	if (open == false && GetNudgeStatus() == true)
		EnableNudgeMode(false);

	//turn on nudge mode timer if doors are open
	if (open == true && NudgeTimer > 0 && nudgetimer->IsRunning() == false)
		nudgetimer->Start(NudgeTimer * 1000, true);

	//reset values
	OpenDoor = 0;
	WhichDoors = 0;
	door_changed = false;
	doors_stopped = false;

	//turn on autoclose timer
	if (manual == false && (elev->InServiceMode() == false || elev->WaitForDoors == true) &&
		(elev->UpPeak == false || ShaftDoorFloor != elev->GetBottomFloor()) &&
		(elev->DownPeak == false || ShaftDoorFloor != elev->GetTopFloor()))
	{
		ResetDoorTimer();
	}

	//play direction message sound
	if (manual == false && elev->InServiceMode() == false && open == true)
		elev->PlayMessageSound(elev->GetArrivalDirection(elev->GetFloor()));

	DoorIsRunning = false;
}

Object* ElevatorDoor::AddDoors(const char *lefttexture, const char *righttexture, float thickness, float CenterX, float CenterZ, float width, float height, bool direction, float tw, float th)
{
	//adds elevator doors specified at a relative central position (off of elevator origin)
	//if direction is false, doors are on the left/right side; otherwise front/back
	float x1, x2, x3, x4;
	float z1, z2, z3, z4;
	float spacing = 0.025f; //spacing between doors

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
	AddDoorComponent("Left", lefttexture, lefttexture, thickness, "Left", OpenSpeed, x1, z1, x2, z2, height, 0, tw, th, tw, th);

	//create right door
	AddDoorComponent("Right", righttexture, righttexture, thickness, "Right", OpenSpeed, x3, z3, x4, z4, height, 0, tw, th, tw, th);

	//finish doors
	return FinishDoors();
}

void ElevatorDoor::AddDoorComponent(DoorWrapper *wrapper, const char *name, const char *meshname, const char *texture, const char *sidetexture, float thickness, const char *direction, float speed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th)
{
	//creates a door component - finish with FinishDoor()

	//create door object
	DoorObject *door = wrapper->CreateDoor(meshname, direction, speed);
	
	if (tw == 0)
		tw = 1;
	if (th == 0)
		th = 1;
	if (side_tw == 0)
		side_tw = 1;
	if (side_th == 0)
		side_th = 1;

	sbs->ResetTextureMapping(true);

	//add main walls
	sbs->DrawWalls(true, true, false, false, false, false);
	sbs->AddWallMain(wrapper->object, door->mesh->MeshWrapper, door->mesh->Submeshes, name, texture, thickness, x1, z1, x2, z2, height, height, voffset, voffset, tw, th, false);
	sbs->ResetWalls();

	//add side walls
	sbs->DrawWalls(false, false, true, true, true, true);
	sbs->AddWallMain(wrapper->object, door->mesh->MeshWrapper, door->mesh->Submeshes, name, sidetexture, thickness, x1, z1, x2, z2, height, height, voffset, voffset, side_tw, side_th, false);
	sbs->ResetWalls();

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

	sbs->ResetTextureMapping();
}

Object* ElevatorDoor::AddDoorComponent(const char *name, const char *texture, const char *sidetexture, float thickness, const char *direction, float speed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th)
{
	//adds an elevator door component; remake of AddDoors command

	Ogre::String elevnumber, doornumber, Name, buffer;
	elevnumber = elev->Number;
	TrimString(elevnumber);
	doornumber = Number;
	TrimString(doornumber);
	Name = name;
	TrimString(Name);
	buffer = "ElevatorDoor " + elevnumber + ":" + doornumber + ":" + Name;
	TrimString(buffer);

	AddDoorComponent(Doors, name, buffer.c_str(), texture, sidetexture, thickness, direction, speed, x1, z1, x2, z2, height, voffset, tw, th, side_tw, side_th);
	return Doors->object;
}

Object* ElevatorDoor::AddShaftDoorComponent(int floor, const char *name, const char *texture, const char *sidetexture, float thickness, const char *direction, float speed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th)
{
	//adds a shaft door component; remake of AddShaftDoor command

	//exit if floor is not serviced by the elevator
	if (!elev->IsServicedFloor(floor))
		return 0;

	int index = -1;
	for (int i = 0; i < elev->ServicedFloors.size(); i++)
	{
		if (elev->ServicedFloors[i] == floor)
			index = i;
	}
	Ogre::String elevnumber, floornumber, doornumber, Name, buffer;
	elevnumber = elev->Number;
	TrimString(elevnumber);
	floornumber = floor;
	TrimString(floornumber);
	doornumber = Number;
	TrimString(doornumber);
	Name = name;
	TrimString(Name);
	buffer = "Elevator " + elevnumber + ": Shaft Door " + doornumber + ":" + floornumber + ":" + Name;

	Floor *floorobj = sbs->GetFloor(floor);

	AddDoorComponent(ShaftDoors[index], name, buffer.c_str(), texture, sidetexture, thickness, direction, speed, x1, z1, x2, z2, height, floorobj->Altitude + floorobj->GetBase(true) + voffset, tw, th, side_tw, side_th);
	return ShaftDoors[index]->object;
}

void ElevatorDoor::AddShaftDoorsComponent(const char *name, const char *texture, const char *sidetexture, float thickness, const char *direction, float speed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th)
{
	//adds shaft door components for all serviced floors; remake of AddShaftDoors command

	Ogre::String elevnumber, floornumber, doornumber, Name;
	elevnumber = elev->Number;
	TrimString(elevnumber);
	doornumber = Number;
	TrimString(doornumber);
	Name = name;
	TrimString(Name);

	//create doors
	for (size_t i = 0; i < elev->ServicedFloors.size(); i++)
	{
		int floor = elev->ServicedFloors[i];
		floornumber = floor;
		TrimString(floornumber);
		AddShaftDoorComponent(floor, name, texture, sidetexture, thickness, direction, speed, x1, z1, x2, z2, height, voffset, tw, th, side_tw, side_th);
	}
}

Object* ElevatorDoor::FinishDoors(DoorWrapper *wrapper, int floor, bool ShaftDoor, float voffset)
{
	//finishes a door creation

	//add floor to manual shaft door list if wrapper doesn't exist and exit
	if (!wrapper && ShaftDoor == true)
	{
		ManualFloors.push_back(floor);
		return 0;
	}

	//set door parameters
	wrapper->Origin = Ogre::Vector3(elev->Origin.x, voffset, elev->Origin.z);

	if (ShaftDoor == false)
		wrapper->Origin.y += elev->Origin.y;

	//get full width and height of doors
	float x1 = 0, x2 = 0, y1 = 0, y2 = 0, z1 = 0, z2 = 0;
	bool firstrun = true;
	for (int i = 0; i < wrapper->doors.size(); i++)
	{
		for (int j = 1; j <= 3; j++)
		{
			Ogre::Vector2 extents = sbs->GetExtents(wrapper->doors[i]->mesh, j);
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

	//if shaft door, cut walls for door
	if (ShaftDoor == true)
	{
		sbs->DrawWalls(true, true, true, true, true, true);
		sbs->ResetTextureMapping(true);

		//create doors
		Floor *floorobj = sbs->GetFloor(floor);
		Shaft *shaft = sbs->GetShaft(elev->AssignedShaft);
		float base = floorobj->GetBase(true); //relative to floor
		float base2 = floorobj->Altitude + base; //absolute

		//cut shaft and floor walls
		if (DoorDirection == false)
		{
			if (!shaft->CutWall(false, floor, Ogre::Vector3(elev->Origin.x + x1 - 2, base, elev->Origin.z + z1), Ogre::Vector3(elev->Origin.x + x1 + 2, base + Doors->Height, elev->Origin.z + z2), 1, "Shaft"))
				return 0;
			floorobj->Cut(Ogre::Vector3(elev->Origin.x + x1 - 2, base, elev->Origin.z + z1), Ogre::Vector3(elev->Origin.x + x1 + 2, base + Doors->Height, elev->Origin.z + z2), true, false, true, 2, "Shaft");
		}
		else
		{
			if (!shaft->CutWall(false, floor, Ogre::Vector3(elev->Origin.x + x1, base, elev->Origin.z + z1 - 2), Ogre::Vector3(elev->Origin.x + x2, base + Doors->Height, elev->Origin.z + z1 + 2), 1, "Shaft"))
				return 0;
			floorobj->Cut(Ogre::Vector3(elev->Origin.x + x1, base, elev->Origin.z + z1 - 2), Ogre::Vector3(elev->Origin.x + x2, base + Doors->Height, elev->Origin.z + z1 + 2), true, false, true, 2, "Shaft");
		}

		//create doorway walls
		WallObject *wall = floorobj->Level->CreateWallObject(floorobj->object, "Connection Walls");
		sbs->AddDoorwayWalls(wall, "ConnectionWall", 0, 0);

		sbs->ResetWalls();
		sbs->ResetTextureMapping();
	}

	//create connection pieces
	sbs->ResetTextureMapping(true);
	Ogre::String name1, name2;
	if (ShaftDoor == false)
	{
		WallObject *wall;
		wall = elev->ElevatorMesh->CreateWallObject(elev->object, false);
		name1 = "DoorF1";
		name2 = "DoorF2";
		sbs->CreateWallBox(wall, name1.c_str(), "Connection", x1, x2, z1, z2, 1, -1.001 + voffset, 0, 0, false, true, true, true, false);
		sbs->CreateWallBox(wall, name2.c_str(), "Connection", x1, x2, z1, z2, 1, wrapper->Height + 0.001 + voffset, 0, 0, false, true, true, true, false);
	}
	else
	{
		WallObject *wall;
		Shaft *shaft = sbs->GetShaft(elev->AssignedShaft);
		wall = shaft->GetMeshObject(floor)->CreateWallObject(shaft->object, false);
		name1 = "ShaftDoorF1";
		name2 = "ShaftDoorF2";
		x1 += elev->Origin.x;
		x2 += elev->Origin.x;
		z1 += elev->Origin.z;
		z2 += elev->Origin.z;
		sbs->CreateWallBox(wall, name1.c_str(), "Connection", x1, x2, z1, z2, 1, -1.001 + voffset, 0, 0, false, true, true, true, false);
		sbs->CreateWallBox(wall, name2.c_str(), "Connection", x1, x2, z1, z2, 1, wrapper->Height + 0.001 + voffset, 0, 0, false, true, true, true, false);
	}

	sbs->ResetTextureMapping();

	//relocate sound object
	if (ShaftDoor == false)
	{
		doorsound->SetPosition(Ogre::Vector3(wrapper->Origin.x, wrapper->Origin.y + (wrapper->Height / 2), wrapper->Origin.z));
		nudgesound->SetPosition(doorsound->GetPosition());
	}
	else
		chime->SetPosition(Ogre::Vector3(wrapper->Origin.x, wrapper->Origin.y + (wrapper->Height / 2), wrapper->Origin.z));

	return wrapper->object;
}

Object* ElevatorDoor::FinishDoors()
{
	//finish elevator doors

	return FinishDoors(Doors, 0, false, 0);
}

Object* ElevatorDoor::FinishShaftDoor(int floor)
{
	//finish shaft door on a specified floor

	Ogre::String floornum;
	floornum = floor;

	//exit if floor is not serviced by the elevator
	if (!elev->IsServicedFloor(floor))
	{
		elev->ReportError("Floor " + floornum + " not a serviced floor");
		return 0;
	}

	Floor *floorobj = sbs->GetFloor(floor);

	if (!floorobj)
	{
		elev->ReportError("Floor " + floornum + " does not exist");
		return 0;
	}

	DoorWrapper *wrapper;

	int index = -1;
	for (int i = 0; i < elev->ServicedFloors.size(); i++)
	{
		if (elev->ServicedFloors[i] == floor)
			index = i;
	}

	if (index > -1)
		wrapper = ShaftDoors[index];
	else
		wrapper = 0;

	return FinishDoors(wrapper, floor, true, floorobj->Altitude + floorobj->GetBase(true));
}

bool ElevatorDoor::FinishShaftDoors()
{
	//finish all shaft doors

	for (size_t i = 0; i < elev->ServicedFloors.size(); i++)
		FinishShaftDoor(elev->ServicedFloors[i]);
	return true;
}

bool ElevatorDoor::AddShaftDoors(const char *lefttexture, const char *righttexture, float thickness, float CenterX, float CenterZ, float tw, float th)
{
	//adds shaft's elevator doors specified at a relative central position (off of elevator origin)
	//uses some parameters (width, height, direction) from AddDoor/AddDoors function

	//set door parameters
	ShaftDoorOrigin = Ogre::Vector3(elev->Origin.x + CenterX, 0, elev->Origin.z + CenterZ);
	ShaftDoorThickness = thickness;

	//create doors
	for (size_t i = 0; i < elev->ServicedFloors.size(); i++)
	{
		if (!AddShaftDoor(elev->ServicedFloors[i], lefttexture, righttexture, tw, th))
			return false;
	}

	return true;
}

Object* ElevatorDoor::AddShaftDoor(int floor, const char *lefttexture, const char *righttexture, float tw, float th)
{
	//adds a single elevator shaft door, with position and thickness parameters first specified
	//by the SetShaftDoors command.
	//uses some parameters (width, height, direction) from AddDoor/AddDoors function

	//exit if floor is not serviced by the elevator
	if (!elev->IsServicedFloor(floor))
		return 0;

	float x1, x2, x3, x4;
	float z1, z2, z3, z4;
	int index = -1;
	for (int i = 0; i < elev->ServicedFloors.size(); i++)
	{
		if (elev->ServicedFloors[i] == floor)
			index = i;
	}

	//set up coordinates
	if (DoorDirection == false)
	{
		x1 = ShaftDoorOrigin.x - elev->Origin.x;
		x2 = ShaftDoorOrigin.x - elev->Origin.x;
		x3 = ShaftDoorOrigin.x - elev->Origin.x;
		x4 = ShaftDoorOrigin.x - elev->Origin.x;
		z1 = ShaftDoorOrigin.z - elev->Origin.z - (Doors->Width / 2);
		z2 = ShaftDoorOrigin.z - elev->Origin.z;
		z3 = ShaftDoorOrigin.z - elev->Origin.z;
		z4 = ShaftDoorOrigin.z - elev->Origin.z + (Doors->Width / 2);
	}
	else
	{
		x1 = ShaftDoorOrigin.x - elev->Origin.x - (Doors->Width / 2);
		x2 = ShaftDoorOrigin.x - elev->Origin.x;
		x3 = ShaftDoorOrigin.x - elev->Origin.x;
		x4 = ShaftDoorOrigin.x - elev->Origin.x + (Doors->Width / 2);
		z1 = ShaftDoorOrigin.z - elev->Origin.z;
		z2 = ShaftDoorOrigin.z - elev->Origin.z;
		z3 = ShaftDoorOrigin.z - elev->Origin.z;
		z4 = ShaftDoorOrigin.z - elev->Origin.z;
	}

	Ogre::String buffer, buffer2, buffer3, buffer4, buffer5;

	//create doors

	//create left door
	AddShaftDoorComponent(floor, "Left", lefttexture, lefttexture, ShaftDoorThickness, "Left", OpenSpeed, x1, z1, x2, z2, Doors->Height, 0, tw, th, tw, th);

	//create right door
	AddShaftDoorComponent(floor, "Right", righttexture, righttexture, ShaftDoorThickness, "Right", OpenSpeed, x3, z3, x4, z4, Doors->Height, 0, tw, th, tw, th);

	//finish doors
	Object *object = FinishShaftDoor(floor);

	//make doors invisible on start
	ShaftDoors[index]->Enable(false);

	return object;
}

void ElevatorDoor::SetShaftDoors(float thickness, float CenterX, float CenterZ)
{
	//pre-set shaft door parameters for the AddShaftDoor command.
	//not needed if using the AddShaftDoors command
	//the Center values are relative offsets from the associated elevator's center

	ShaftDoorThickness = thickness;
	ShaftDoorOrigin = Ogre::Vector3(elev->Origin.x + CenterX, 0, elev->Origin.z + CenterZ);
}

void ElevatorDoor::ShaftDoorsEnabled(int floor, bool value)
{
	//turns shaft elevator doors on/off

	//exit if shaft's ShowFullShaft is set
	if (sbs->GetShaft(elev->AssignedShaft)->ShowFullShaft == true && value == false)
		return;

	//leave top and bottom on
	if ((floor == sbs->GetShaft(elev->AssignedShaft)->startfloor || floor == sbs->GetShaft(elev->AssignedShaft)->endfloor) && value == false)
		return;

	//exit if elevator doesn't service the requested floor
	int index = -1;
	for (int i = 0; i < elev->ServicedFloors.size(); i++)
	{
		if (elev->ServicedFloors[i] == floor)
			index = i;
	}

	if (index == -1)
		return;

	//exit if the specified floor has no shaft doors
	if (ShaftDoorsExist(floor) == false)
		return;

	ShaftDoors[index]->Enable(value);
}

void ElevatorDoor::ShaftDoorsEnabledRange(int floor, int range)
{
	//turn on a range of floors
	//if range is 3, show shaft door on current floor (floor), and 1 floor below and above (3 total floors)
	//if range is 1, show door on only the current floor (floor)

	//exit if shaft's ShowFullShaft is set
	if (sbs->GetShaft(elev->AssignedShaft)->ShowFullShaft == true)
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
		int index = -1;
		for (int i = 0; i < elev->ServicedFloors.size(); i++)
		{
			if (elev->ServicedFloors[i] == floor)
				index = i;
		}
		return ShaftDoors[index]->Open;
	}
	return false;
}

void ElevatorDoor::Timer::Notify()
{
	if (type == 0)
	{
		//door autoclose timer

		//close doors if open
		if (door->AreDoorsOpen() == true && (elevator->InServiceMode() == false || elevator->WaitForDoors == true))
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
	//play chime sound on specified floor
	if (direction == false && chimesound_loaded != -1)
	{
		chime->Load(DownChimeSound.c_str());
		chimesound_loaded = -1;
	}
	else if (chimesound_loaded != 1)
	{
		chime->Load(UpChimeSound.c_str());
		chimesound_loaded = 1;
	}
	chime->Loop(false);
	chime->SetPositionY(sbs->GetFloor(floor)->GetBase() + Doors->Height);
	chime->Play();
}

void ElevatorDoor::ResetDoorTimer()
{
	//reset elevator door timer
	if (quick_close == false)
		timer->Start(DoorTimer, true);
	else
		timer->Start(QuickClose, true);
	quick_close = false;
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

void ElevatorDoor::Loop()
{
	//main loop
	if (OpenDoor == 1)
		MoveDoors(true, false);
	if (OpenDoor == -1)
		MoveDoors(false, false);
	if (OpenDoor == 2)
		MoveDoors(true, true);
	if (OpenDoor == -2)
		MoveDoors(false, true);
}

void ElevatorDoor::Move(const Ogre::Vector3 &position, bool relative_x, bool relative_y, bool relative_z)
{
	//moves doors

	for (int i = 0; i < Doors->doors.size(); i++)
	{
		Doors->doors[i]->mesh->Move(position, relative_x, relative_y, relative_z);
	}
}

void ElevatorDoor::Enabled(bool value)
{
	Doors->Enable(value);
}

bool ElevatorDoor::IsEnabled()
{
	//are doors enabled?
	return Doors->Enabled;
}

bool ElevatorDoor::GetDoorsOpen()
{
	return Doors->Open;
}

int ElevatorDoor::GetWhichDoors()
{
	//return value of WhichDoors
	return WhichDoors;
}

void ElevatorDoor::MoveSound(const Ogre::Vector3 &position, bool relative_x, bool relative_y, bool relative_z)
{
	//move sound
	Ogre::Vector3 pos;
	if (relative_x == false)
		pos.x = position.x;
	else
		pos.x = Doors->Origin.x + position.x;
	if (relative_y == false)
		pos.y = position.y + (Doors->Height / 2);
	else
		pos.y = Doors->Origin.y + position.y;
	if (relative_z == false)
		pos.z = position.z;
	else
		pos.z = Doors->Origin.z + position.z;
	doorsound->SetPosition(pos);
	nudgesound->SetPosition(pos);
}

bool ElevatorDoor::ShaftDoorsExist(int floor)
{
	//return true if shaft doors have been created for this door on the specified floor

	int index = -1;
	for (int i = 0; i < elev->ServicedFloors.size(); i++)
	{
		if (elev->ServicedFloors[i] == floor)
			index = i;
	}
	if (index != -1)
	{
		if (ShaftDoors[index]->doors.size() > 0)
			return true;
	}
	return false;
}

ElevatorDoor::DoorObject::DoorObject(const char *doorname, DoorWrapper *Wrapper, const char *Direction, float Speed)
{
	name = doorname;
	wrapper = Wrapper;
	parent = wrapper->parent;

	//create object mesh
	mesh = new MeshObject(wrapper->object, doorname);
	
	Ogre::String direction_check = Direction;
	SetCase(direction_check, false);
	TrimString(direction_check);

	//set direction value from strings
	direction = 0;
	if (direction_check == "down")
		direction = 1;
	if (direction_check == "left" || direction_check == "forward")
		direction = 2;
	if (direction_check == "right" || direction_check == "back")
		direction = 3;

	speed = Speed;
	active_speed = 0;
	openchange = 0;
	marker1 = 0;
	marker2 = 0;
	door_section = 0;
	stopping_distance = 0;
	temp_change = 0;
	accelerating = false;
	is_open = false;
	finished = false;
	sign_changed = false;
	old_difference = 0;
	recheck_difference = false;
}

ElevatorDoor::DoorObject::~DoorObject()
{
	if (mesh)
		delete mesh;
	mesh = 0;
}

ElevatorDoor::DoorWrapper::DoorWrapper(ElevatorDoor *parentobject, bool shaftdoor)
{
	parent = parentobject;
	Open = false;
	Enabled = true;
	Width = 0;
	Height = 0;
	Origin = 0;
	Thickness = 0;
	IsShaftDoor = shaftdoor;
	Shift = 0;

	object = new Object();
	object->SetValues(this, parent->object, "DoorWrapper", "Door Wrapper", false);
}

ElevatorDoor::DoorWrapper::~DoorWrapper()
{
	for (int i = 0; i < doors.size(); i++)
	{
		if (doors[i])
			delete doors[i];
		doors[i] = 0;
	}
	if (object)
		delete object;
	object = 0;
}

ElevatorDoor::DoorObject* ElevatorDoor::DoorWrapper::CreateDoor(const char *doorname, const char *direction, float speed)
{
	//initialize a door component
	
	doors.resize(doors.size() + 1);
	int index = doors.size() - 1;
	doors[index] = new DoorObject(doorname, this, direction, speed);

	//move object to positions
	if (IsShaftDoor == false)
		doors[index]->mesh->Move(parent->elev->Origin, false, false, false);
	else
		doors[index]->mesh->Move(Ogre::Vector3(parent->elev->Origin.x, 0, parent->elev->Origin.z), false, false, false);

	return doors[index];
}

void ElevatorDoor::DoorWrapper::Enable(bool value)
{
	//enable/disable door meshes

	if (value == Enabled)
		return;

	for (int i = 0; i < doors.size(); i++)
		doors[i]->mesh->Enable(value);
	Enabled = value;
}

void ElevatorDoor::DoorObject::MoveDoors(bool open, bool manual)
{
	//opens or closes elevator doors
	//currently only supports doors on either the left/right or front/back
	//diagonal doors will be done later, by possibly using relative plane movement

	//this door system is based on offsets from the door origin (starting position).
	//when opening, the door starts at the origin, accelerates to marker 1, moves at a constant
	//rate to marker 2, and then decelerates after marker 2.
	//the acceleration/deceleration sections aren't used if manual is true (in that case, it simply sets a speed value, and moves
	//the doors until they reach the ends

	//direction is either 0 for up, 1 for down, 2 for left/forward and 3 for right/backward

	//first get position and origin of door, and adjust values to reflect the "edge" of the door
	float tempposition, temporigin;

	if (finished == true)
		return;

	if (direction > 1)
	{
		if (parent->DoorDirection == false)
		{
			tempposition = mesh->GetPosition().z + wrapper->Shift;
			temporigin = wrapper->Origin.z + wrapper->Shift;
		}
		else
		{
			tempposition = mesh->GetPosition().x + wrapper->Shift;
			temporigin = wrapper->Origin.x + wrapper->Shift;
		}
	}
	else
	{
		tempposition = mesh->GetPosition().y;
		if (wrapper->IsShaftDoor == false)
			temporigin = parent->elev->GetPosition().y;
		else
			temporigin = 0;
	}

	//get distance from starting point
	float difference = fabs(tempposition - temporigin);

	if (old_difference != 0 && manual == true && recheck_difference == true)
	{
		//check if the position went beyond 0
		if ((tempposition - temporigin > 0 && old_difference < 0) || (tempposition - temporigin < 0 && old_difference > 0))
			sign_changed = true;
	}

	if (recheck_difference == false && old_difference != 0)
	{
		recheck_difference = true;
		old_difference = 0;
	}
	else
		old_difference = tempposition - temporigin;

	//debug - show current section as function is running
	//sbs->Report("Door section: " + Ogre::String(_itoa(door_section, parent->intbuffer, 10)));

	if (parent->door_changed == false && door_section == 0)
	{
		//initialization code

		finished = false;
		recheck_difference = false;

		//marker1 is the position to stop accelerating at (accelerates to marker 1)
		//marker2 is the position to start decelerating at (runs full speed until marker 2)
		if (manual == false)
		{
			if (parent->GetNudgeStatus() == false || parent->SlowSpeed == 0)
				openchange = speed / 50;
			else
				openchange = (speed * parent->SlowSpeed) / 50;

			if (direction > 1)
			{
				//get width and offset values (offset is the distance the door component
				//is from the edge of the door frame)
				float width;
				float mainwidth = wrapper->Width / 2;
				if (parent->DoorDirection == false)
				{
					width = fabs(extents_max.z - extents_min.z);
					if (direction == 2)
						offset = mainwidth + (extents_min.z - wrapper->Shift);
					else
						offset = mainwidth - (extents_max.z - wrapper->Shift);
				}
				else
				{
					width = fabs(extents_max.x - extents_min.x);
					if (direction == 2)
						offset = mainwidth + (extents_min.x - wrapper->Shift);
					else
						offset = mainwidth - (extents_max.x - wrapper->Shift);
				}
				float newwidth = width + offset;
				marker1 = newwidth / 4;
				marker2 = (mainwidth + (width - mainwidth)) - marker1 + offset;
			}
			else
			{
				float height = fabs(extents_max.y - extents_min.y);
				float mainheight = wrapper->Height / 2;
				if (direction == 0)
				{
					if (wrapper->IsShaftDoor == false)
						offset = wrapper->Height - extents_max.y;
					else
						offset = wrapper->Height - (extents_max.y - wrapper->Origin.y);
				}
				else
				{
					if (wrapper->IsShaftDoor == false)
						offset = extents_min.y;
					else
						offset = extents_min.y - wrapper->Origin.y;
				}
				float newheight = height + offset;
				marker1 = newheight / 4;
				marker2 = (mainheight + (height - mainheight)) - marker1 + offset;
			}
			active_speed = 0;
		}
		else
		{
			//manual movement positioning (same as normal positioning, but the markers are at the
			//door frame extents
			if (direction > 1)
			{
				float width;
				float mainwidth = wrapper->Width / 2;
				if (parent->DoorDirection == false)
				{
					width = fabs(extents_max.z - extents_min.z);
					if (direction == 2)
						offset = mainwidth + (extents_min.z - wrapper->Shift);
					else
						offset = mainwidth - (extents_max.z - wrapper->Shift);
				}
				else
				{
					width = fabs(extents_max.x - extents_min.x);
					if (direction == 2)
						offset = mainwidth + (extents_min.x - wrapper->Shift);
					else
						offset = mainwidth - (extents_max.x - wrapper->Shift);
				}
				marker1 = 0;
				marker2 = mainwidth + (width - mainwidth) + offset;
			}
			else
			{
				float height = fabs(extents_max.y - extents_min.y);
				float mainheight = wrapper->Height / 2;
				if (direction == 0)
				{
					if (wrapper->IsShaftDoor == false)
						offset = wrapper->Height - extents_max.y;
					else
						offset = wrapper->Height - (extents_max.y - wrapper->Origin.y);
				}
				else
				{
					if (wrapper->IsShaftDoor == false)
						offset = extents_min.y;
					else
						offset = extents_min.y - wrapper->Origin.y;
				}
				marker1 = 0;
				marker2 = mainheight + (height - mainheight) + offset;
			}

			if (open == true)
				active_speed = parent->ManualSpeed;
			else
				active_speed = -parent->ManualSpeed;
		}
	}
	else if (parent->previous_open != open && parent->door_changed == true)
	{
		//if a different direction was specified during movement
		//only change directions immediately if re-opening (closing, then opening)
		if (open == true)
		{
			//relocate marker 1 to the door's current position, in order to stop it
			float offset = marker1 - difference;
			if (difference >= marker1)
				//place marker at door position
				marker1 = difference;
			else
				//place marker to the right based on the offset, to bring door back to full speed
				marker1 = difference + offset;
		}
	}

	//Speed up doors (only if manual is false)
	if (manual == false)
	{
		//if door is opening and is left of marker 1
		//or if door is closing and is to the right of marker 2
		if ((difference <= marker1 && open == true) || (difference > marker2 && open == false))
		{
			accelerating = true;
			if (parent->door_changed == false)
			{
				//normal door acceleration
				if (open == true)
					active_speed += openchange;
				else
					active_speed -= openchange;
			}
			else
			{
				//reverse movement if transitioning from close to open
				//this will trigger if door is closing, and is told to open while left of relocated marker 1
				if (difference <= marker1)
				{
					active_speed += openchange;
				}
			}

			//move elevator doors
			Move();

			//get stopping distance
			stopping_distance = difference;

			door_section = 1;
			return;
		}
	}

	door_section = 2;

	//Normal door movement
	if ((difference < marker2 && open == true) || (difference > marker1 && open == false))
	{
		if (sign_changed == false) //don't run movement code if difference went beyond 0, for manual only
		{
			Move();
			door_section = 3;
			return;
		}
	}

	accelerating = false;

	//slow down doors (only if manual is false)
	if (manual == false)
	{
		if ((active_speed > 0 && open == true) || (active_speed < 0 && open == false))
		{
			if (open == true)
				active_speed -= openchange;
			else
				active_speed += openchange;
			
			Move();
			door_section = 4;
			return;
		}
	}

	//report on what section preceded the finishing code (should be 4)
	//sbs->Report("Door section: " + Ogre::String(_itoa(door_section, parent->intbuffer, 10)));

	//place doors in positions (fixes any overrun errors)
	float ypos;
	if (wrapper->IsShaftDoor == false)
		ypos = parent->elev->GetPosition().y;
	else
		ypos = 0;

	if (open == true)
	{
		//move elevator doors
		if (direction > 1)
		{
			float mainwidth = wrapper->Width / 2;
			if (parent->DoorDirection == false)
			{
				float width = fabs(extents_max.z - extents_min.z);
				if (direction == 2)
					mesh->Move(Ogre::Vector3(parent->elev->Origin.x, ypos, parent->elev->Origin.z - (mainwidth + (width - mainwidth) + offset)), false, false, false);
				else
					mesh->Move(Ogre::Vector3(parent->elev->Origin.x, ypos, parent->elev->Origin.z + (mainwidth + (width - mainwidth) + offset)), false, false, false);
			}
			else
			{
				float width = fabs(extents_max.x - extents_min.x);
				if (direction == 2)
					mesh->Move(Ogre::Vector3(parent->elev->Origin.x - (mainwidth + (width - mainwidth) + offset), ypos, parent->elev->Origin.z), false, false, false);
				else
					mesh->Move(Ogre::Vector3(parent->elev->Origin.x + (mainwidth + (width - mainwidth) + offset), ypos, parent->elev->Origin.z), false, false, false);
			}
		}
		else
		{
			float mainheight = wrapper->Height / 2;
			float height = fabs(extents_max.y - extents_min.y);
			if (direction == 0)
				mesh->Move(Ogre::Vector3(parent->elev->Origin.x, ypos + (mainheight + (height - mainheight) + offset), parent->elev->Origin.z), false, false, false);
			else
				mesh->Move(Ogre::Vector3(parent->elev->Origin.x, ypos - (mainheight + (height - mainheight) + offset), parent->elev->Origin.z), false, false, false);
		}
	}
	else
		mesh->Move(Ogre::Vector3(parent->elev->Origin.x, ypos, parent->elev->Origin.z), false, false, false);

	//the door is open or closed now
	is_open = open;
	finished = true;

	//reset values
	active_speed = 0;
	door_section = 0;
	sign_changed = false;
	old_difference = 0;
	stopping_distance = 0;
}

void ElevatorDoor::DoorObject::Move()
{
		//move elevator doors

		//up movement
		if (direction == 0)
			mesh->Move(Ogre::Vector3(0, active_speed * sbs->delta, 0), true, true, true);

		//down movement
		if (direction == 1)
			mesh->Move(Ogre::Vector3(0, -active_speed * sbs->delta, 0), true, true, true);

		if (parent->DoorDirection == false)
		{
			//left movement
			if (direction == 2)
				mesh->Move(Ogre::Vector3(0, 0, -active_speed * sbs->delta), true, true, true);

			//right movement
			if (direction == 3)
				mesh->Move(Ogre::Vector3(0, 0, active_speed * sbs->delta), true, true, true);
		}
		else
		{
			//left movement
			if (direction == 2)
				mesh->Move(Ogre::Vector3(-active_speed * sbs->delta, 0, 0), true, true, true);

			//right movement
			if (direction == 3)
				mesh->Move(Ogre::Vector3(active_speed * sbs->delta, 0, 0), true, true, true);
		}
}

void ElevatorDoor::DoorWrapper::MoveDoors(bool open, bool manual)
{
	//calls per-door move function
	for (int i = 0; i < doors.size(); i++)
		doors[i]->MoveDoors(open, manual);
}

bool ElevatorDoor::DoorWrapper::CheckDoorsOpen()
{
	//checks to see if doors are open or closed, and returns true if the status changed

	for (int i = 0; i < doors.size(); i++)
	{
		//exit if the status is the same on any door
		if (doors[i]->is_open == Open)
			return false;
	}

	//if the status changed, set the new status and return true
	Open = !Open;
	return true;
}

bool ElevatorDoor::DoorWrapper::IsFinished()
{
	//checks to see if all of the doors are finished

	for (int i = 0; i < doors.size(); i++)
	{
		//exit if any door is not finished
		if (doors[i]->finished == false)
			return false;
	}
	return true;
}

void ElevatorDoor::DoorWrapper::StopDoors()
{
	//stop all doors

	for (int i = 0; i < doors.size(); i++)
	{
		doors[i]->active_speed = 0;
		doors[i]->door_section = 0;
	}
}

ElevatorDoor::DoorWrapper* ElevatorDoor::GetDoorWrapper()
{
	//return door wrapper object
	return Doors;
}

void ElevatorDoor::Hold()
{
	//hold door (basically turn off timer)

	Ogre::String doornumber;
        if (elev->NumDoors > 1)
        {
                doornumber = " ";
                doornumber = doornumber + _itoa(Number, intbuffer, 10);
        }

	//exit if nudge mode is active
	if (GetNudgeStatus() == true)
		return;

	sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + ": holding doors" + doornumber);

	nudgetimer->Stop();
	timer->Stop();
}

void ElevatorDoor::EnableNudgeMode(bool value)
{
	//enable or disable nudge mode

	Ogre::String doornumber;
	if (elev->NumDoors > 1)
	{
		doornumber = " ";
		doornumber = doornumber + _itoa(Number, intbuffer, 10);
	}

	if (value == true && nudge_enabled == false && AreDoorsOpen() == true && (elev->InServiceMode() == false || (elev->FireServicePhase1 == 1 && elev->GetFloor() != elev->ParkingFloor)))
	{
		if ((elev->UpPeak == true && elev->GetFloor() == elev->GetBottomFloor()) || (elev->DownPeak == true && elev->GetFloor() == elev->GetTopFloor()))
			return;
		sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + " Doors" + doornumber + ": nudge mode activated");
		nudge_enabled = true;
		if (nudgesound_loaded == false)
			nudgesound->Load(NudgeSound.c_str());
		nudgesound_loaded = true;
		nudgesound->Loop(true);
		nudgesound->Play();
		CloseDoors();
	}
	else if (nudge_enabled == true)
	{
		sbs->Report("Elevator " + Ogre::String(_itoa(elev->Number, intbuffer, 10)) + " Doors" + doornumber + ": nudge mode disabled");
		nudge_enabled = false;
		nudgesound->Stop();
	}
}

bool ElevatorDoor::GetNudgeStatus()
{
	//get status of nudge mode

	return nudge_enabled;
}
