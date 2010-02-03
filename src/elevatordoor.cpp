/* $Id$ */

/*
	Scalable Building Simulator - Elevator Door Class
	The Skyscraper Project - Version 1.6 Alpha
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

#include <iengine/movable.h>

extern SBS *sbs; //external pointer to the SBS engine

ElevatorDoor::ElevatorDoor(int number, Elevator* elevator)
{
	//set up SBS object
	object = new Object();
	object->SetValues(this, elevator->object, "ElevatorDoor", false);

	//create a new elevator door
	Number = number + 1;
	elev = elevator;
	OpenDoor = 0;
	OpenSpeed = sbs->confman->GetFloat("Skyscraper.SBS.Elevator.Door.OpenSpeed", 0.3);
	WhichDoors = 0;
	ShaftDoorFloor = 0;
	DoorTimer = sbs->confman->GetInt("Skyscraper.SBS.Elevator.Door.Timer", 5000);
	DoorIsRunning = false;
	index = 0;
	previous_open = false;
	door_changed = false;
	quick_close = false;
	OpenSound = sbs->confman->GetStr("Skyscraper.SBS.Elevator.Door.OpenSound", "elevatoropen.wav");
	CloseSound = sbs->confman->GetStr("Skyscraper.SBS.Elevator.Door.CloseSound", "elevatorclose.wav");
	UpChimeSound = sbs->confman->GetStr("Skyscraper.SBS.Elevator.Door.UpChimeSound", "chime1-up.wav");
	DownChimeSound = sbs->confman->GetStr("Skyscraper.SBS.Elevator.Door.DownChimeSound", "chime1-down.wav");
	doors_stopped = false;
	ShaftDoorThickness = 0;
	ShaftDoorOrigin = 0;

	//create main door object
	Doors = new DoorWrapper(this, false);

	//create timer
	timer = new Timer(this, elev);

	//create shaft door objects
	ShaftDoors.SetSize(elev->ServicedFloors.GetSize());
	for (int i = 0; i < ShaftDoors.GetSize(); i++)
		ShaftDoors[i] = new DoorWrapper(this, true);

	//create sound object
	doorsound = new Sound(this->object, "Door Sound");
	doorsound->SetPosition(elevator->Origin);
	chime = new Sound(this->object, "Chime");
}

ElevatorDoor::~ElevatorDoor()
{
	//delete shaft door objects
	for (int i = 0; i < ShaftDoors.GetSize(); i++)
	{
		if (ShaftDoors[i])
			delete ShaftDoors[i];
		ShaftDoors[i] = 0;
	}

	//Destructor
	if (timer)
	{
		timer->Stop();
		delete timer;
	}
	timer = 0;
	if (doorsound)
		delete doorsound;
	doorsound = 0;
	if (chime)
		delete chime;
	chime = 0;

	//delete main doors
	if (Doors)
		delete Doors;
	Doors = 0;

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

	csString doornumber;
	if (elev->NumDoors > 1)
	{
		doornumber = " ";
		doornumber = doornumber + _itoa(Number, intbuffer, 10);
	}

	//exit if trying to open doors while stopped
	if (manual == false && doors_stopped == true)
	{
		sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + ": cannot open doors" + doornumber + "; doors manually stopped");
		return;
	}

	//exit if direction changed (if doors have already switched from close to open)
	if (door_changed == true)
		return;

	//don't open doors if emergency stop is enabled
	if (elev->OnFloor == false && whichdoors != 3 && manual == false)
	{
		sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + ": cannot open doors" + doornumber + "; emergency stop enabled");
		return;
	}

	//check if elevator doors are already open
	if (Doors->Open == true && whichdoors != 3 && OpenDoor == 0 && doors_stopped == false)
	{
		//reset timer if not in a service mode
		if (elev->InServiceMode() == false)
		{
			sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + ": doors" + doornumber + " already open; resetting timer");
			ResetDoorTimer();
		}
		else
			sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + ": doors" + doornumber + " already open");
		return;
	}

	//exit if doors are manually opening
	if (OpenDoor == 2 && DoorIsRunning == true)
	{
		sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + ": doors" + doornumber + " in use");
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
			sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + " Doors" + doornumber + ": invalid shaft doors");
			return;
		}
		if (AreShaftDoorsOpen(floor) == true)
		{
			sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + ": shaft doors" + doornumber + " already open on floor " + csString(_itoa(floor, intbuffer, 10)) + " (" + sbs->GetFloor(floor)->ID + ")");
			return;
		}
		else if (manual == false)
			sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + ": opening shaft doors" + doornumber + " on floor " + csString(_itoa(floor, intbuffer, 10)) + " (" + sbs->GetFloor(floor)->ID + ")");
		else
			sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + ": manually opening shaft doors" + doornumber + " on floor " + csString(_itoa(floor, intbuffer, 10)) + " (" + sbs->GetFloor(floor)->ID + ")");
	}
	else if (manual == false)
		sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + ": opening doors" + doornumber);
	else
		sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + ": manually opening doors" + doornumber);

	if (manual == false)
		OpenDoor = 1;
	else
		OpenDoor = 2;

	//if both doors are selected but elevator is not on a floor, only open elevator doors
	if (whichdoors == 1 && elev->OnFloor == false)
		whichdoors = 2;

	//if opening both doors, exit if shaft doors don't exist
	if (whichdoors == 1 && ShaftDoorsExist(elev->GetFloor()) == false)
	{
		sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + ": can't open doors" + doornumber + " - no shaft doors");
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

	csString doornumber;
	if (elev->NumDoors > 1)
	{
		doornumber = " ";
		doornumber = doornumber + _itoa(Number, intbuffer, 10);
	}

	//exit if trying to open doors while stopped
	if (manual == false && doors_stopped == true)
	{
		sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + ": cannot close doors" + doornumber + "; doors manually stopped");
		return;
	}

	//do not close doors while fire service mode 1 is on
	if (manual == false && elev->FireServicePhase1 == 1)
	{
		sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + ": cannot close doors" + doornumber + " while Fire Service Phase 1 is on");
		return;
	}

	//do not close doors while fire service mode 2 is set to hold
	if (manual == false && elev->FireServicePhase2 == 2)
	{
		sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + ": cannot close doors" + doornumber + " while Fire Service Phase 2 is set to hold");
		return;
	}

	//if called while doors are opening, set quick_close (causes door timer to trigger faster)
	if (OpenDoor != 0 && manual == false)
	{
		sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + ": will close doors" + doornumber + " three seconds after staying open");
		quick_close = true;
		return;
	}

	//check if elevator doors are already closed
	if (Doors->Open == false && whichdoors != 3 && OpenDoor == 0 && doors_stopped == false)
	{
		sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + ": doors" + doornumber + " already closed");
		return;
	}

	//exit if doors are manually closing
	if (OpenDoor == -2 && DoorIsRunning == true)
	{
		sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + ": doors" + doornumber + " in use");
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
			sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + " Doors" + doornumber + ": invalid shaft doors");
			return;
		}
		if (AreShaftDoorsOpen(floor) == false && whichdoors == 3)
		{
			sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + ": shaft doors" + doornumber + "already closed on floor " + csString(_itoa(floor, intbuffer, 10)) + " (" + sbs->GetFloor(floor)->ID + ")");
			return;
		}
		else if (manual == false)
			sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + ": closing shaft doors" + doornumber + " on floor " + csString(_itoa(floor, intbuffer, 10)) + " (" + sbs->GetFloor(floor)->ID + ")");
		else
			sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + ": manually closing shaft doors" + doornumber + " on floor " + csString(_itoa(floor, intbuffer, 10)) + " (" + sbs->GetFloor(floor)->ID + ")");
	}
	else if (manual == false)
		sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + ": closing doors" + doornumber);
	else
		sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + ": manually closing doors" + doornumber);

	if (manual == false)
		OpenDoor = -1;
	else
		OpenDoor = -2;

	//if both doors are selected but elevator is not on a floor, only close elevator doors
	if (whichdoors == 1 && elev->OnFloor == false)
		whichdoors = 2;

	//if closing both doors, exit if shaft doors don't exist
	if (whichdoors == 1 && ShaftDoorsExist(elev->GetFloor()) == false)
	{
		sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + ": can't close doors" + doornumber + " - no shaft doors");
		OpenDoor = 0;
		return;
	}

	//turn off directional indicators
	if (whichdoors == 1)
		sbs->GetFloor(elev->GetFloor())->SetDirectionalIndicators(elev->Number, false, false);

	WhichDoors = whichdoors;
	ShaftDoorFloor = floor;
	MoveDoors(false, manual);
}

void ElevatorDoor::StopDoors()
{
	//stops doors that are currently moving; can only be used for manual/emergency movements
	//this basically just resets the door internals

	csString doornumber;
	if (elev->NumDoors > 1)
	{
		doornumber = " ";
		doornumber = doornumber + _itoa(Number, intbuffer, 10);
	}

	if (OpenDoor == -2 || OpenDoor == 2)
	{
		if (WhichDoors == 3)
			sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + ": stopping shaft doors" + doornumber + " on floor " + csString(_itoa(ShaftDoorFloor, intbuffer, 10)) + " (" + sbs->GetFloor(ShaftDoorFloor)->ID + ")");
		else
			sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + ": stopping doors" + doornumber);

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
		sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + ": can only stop doors" + doornumber + " in manual/emergency mode");
	else
		sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + ": cannot stop doors" + doornumber + "; no doors moving");
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

	if (DoorIsRunning == false || (manual == true && previous_open != open))
	{
		//initialization code

		//stop timer
		timer->Stop();

		DoorIsRunning = true;
		door_changed = false;

		int checkfloor;
		if (WhichDoors == 3)
			checkfloor = ShaftDoorFloor;
		else
			checkfloor = elev->GetFloor();
		index = elev->ServicedFloors.Find(checkfloor);
		if (ShaftDoorsExist(checkfloor) == false)
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
				doorsound->Load(OpenSound.GetData());
				doorsound->Play();
			}
			else
			{
				//play elevator closing sound
				doorsound->Load(CloseSound.GetData());
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
	}
	else if (previous_open != open && manual == false && door_changed == false)
	{
		//if a different direction was specified during movement
		door_changed = true;
	}

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

	//perform door movement and get open state of each door
	if (elevdoors == true)
	{
		for (int i = 0; i < Doors->doors.GetSize(); i++)
		{
			Doors->doors[i]->MoveDoors(open, manual);
		}
	}

	if (shaftdoors == true)
	{
		for (int i = 0; i < ShaftDoors[index]->doors.GetSize(); i++)
		{
			ShaftDoors[index]->doors[i]->MoveDoors(open, manual);
		}
	}

	//update call status (previous_open detects call changes during movement)
	previous_open = open;

	//wait until the doors are finished moving
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

	//reset values
	OpenDoor = 0;
	WhichDoors = 0;
	door_changed = false;
	doors_stopped = false;

	//turn on autoclose timer
	if (manual == false && elev->InServiceMode() == false &&
		(elev->UpPeak == false || elev->GetFloor() != elev->GetBottomFloor()) &&
		(elev->DownPeak == false || elev->GetFloor() != elev->GetTopFloor()))
	{
		if (quick_close == false)
			timer->Start(DoorTimer, true);
		else
			timer->Start(3000, true);
		quick_close = false;
	}

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
	sbs->AddWallMain(wrapper->object, door->mesh, name, texture, thickness, x1, z1, x2, z2, height, height, voffset, voffset, tw, th);
	sbs->ResetWalls();

	//add side walls
	sbs->DrawWalls(false, false, true, true, true, true);
	sbs->AddWallMain(wrapper->object, door->mesh, name, sidetexture, thickness, x1, z1, x2, z2, height, height, voffset, voffset, side_tw, side_th);
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

	csString elevnumber, doornumber, Name, buffer;
	elevnumber = elev->Number;
	elevnumber.Trim();
	doornumber = Number;
	doornumber.Trim();
	Name = name;
	Name.Trim();
	buffer = "ElevatorDoor " + elevnumber + ":" + doornumber + ":" + Name;
	buffer.Trim();

	AddDoorComponent(Doors, name, buffer, texture, sidetexture, thickness, direction, speed, x1, z1, x2, z2, height, voffset, tw, th, side_tw, side_th);
	return Doors->object;
}

Object* ElevatorDoor::AddShaftDoorComponent(int floor, const char *name, const char *texture, const char *sidetexture, float thickness, const char *direction, float speed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th)
{
	//adds a shaft door component; remake of AddShaftDoor command

	//exit if floor is not serviced by the elevator
	if (!elev->IsServicedFloor(floor))
		return 0;

	int index = elev->ServicedFloors.Find(floor);
	csString elevnumber, floornumber, doornumber, Name, buffer;
	elevnumber = elev->Number;
	elevnumber.Trim();
	floornumber = floor;
	floornumber.Trim();
	doornumber = Number;
	doornumber.Trim();
	Name = name;
	Name.Trim();
	buffer = "Elevator " + elevnumber + ": Shaft Door " + doornumber + ":" + floornumber + ":" + Name;

	Floor *floorobj = sbs->GetFloor(floor);

	AddDoorComponent(ShaftDoors[index], name, buffer, texture, sidetexture, thickness, direction, speed, x1, z1, x2, z2, height, floorobj->Altitude + floorobj->GetBase(true) + voffset, tw, th, side_tw, side_th);
	return ShaftDoors[index]->object;
}

void ElevatorDoor::AddShaftDoorsComponent(const char *name, const char *texture, const char *sidetexture, float thickness, const char *direction, float speed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th)
{
	//adds shaft door components for all serviced floors; remake of AddShaftDoors command

	csString elevnumber, floornumber, doornumber, Name, buffer;
	elevnumber = elev->Number;
	elevnumber.Trim();
	doornumber = Number;
	doornumber.Trim();
	Name = name;
	Name.Trim();

	//create doors
	for (size_t i = 0; i < elev->ServicedFloors.GetSize(); i++)
	{
		int floor = elev->ServicedFloors[i];
		floornumber = floor;
		floornumber.Trim();
		buffer = "Elevator " + elevnumber + ": Shaft Door " + doornumber + ":" + floornumber + ":" + Name;
		AddShaftDoorComponent(floor, name, texture, sidetexture, thickness, direction, speed, x1, z1, x2, z2, height, voffset, tw, th, side_tw, side_th);
	}
}

Object* ElevatorDoor::FinishDoors(DoorWrapper *wrapper, int floor, bool ShaftDoor, float voffset)
{
	//finishes a door creation

	//set door parameters
	wrapper->Origin = csVector3(elev->Origin.x, voffset, elev->Origin.z);

	if (ShaftDoor == false)
		wrapper->Origin.y += elev->Origin.y;

	//get full width and height of doors
	float x1 = 0, x2 = 0, y1 = 0, y2 = 0, z1 = 0, z2 = 0;
	bool firstrun = true;
	for (int i = 0; i < wrapper->doors.GetSize(); i++)
	{
		for (int j = 1; j <= 3; j++)
		{
			csVector2 extents = sbs->GetExtents(wrapper->doors[i]->state, j);
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
			if (!shaft->CutWall(false, floor, csVector3(elev->Origin.x + x1 - 2, base, elev->Origin.z + z1), csVector3(elev->Origin.x + x1 + 2, base + Doors->Height, elev->Origin.z + z2), 1, "Shaft"))
				return 0;
			floorobj->Cut(csVector3(elev->Origin.x + x1 - 2, base, elev->Origin.z + z1), csVector3(elev->Origin.x + x1 + 2, base + Doors->Height, elev->Origin.z + z2), true, false, true, 2, "Shaft");
		}
		else
		{
			if (!shaft->CutWall(false, floor, csVector3(elev->Origin.x + x1, base, elev->Origin.z + z1 - 2), csVector3(elev->Origin.x + x2, base + Doors->Height, elev->Origin.z + z1 + 2), 1, "Shaft"))
				return 0;
			floorobj->Cut(csVector3(elev->Origin.x + x1, base, elev->Origin.z + z1 - 2), csVector3(elev->Origin.x + x2, base + Doors->Height, elev->Origin.z + z1 + 2), true, false, true, 2, "Shaft");
		}

		//create doorway walls
		WallObject *wall = sbs->CreateWallObject(floorobj->level_walls, floorobj->Level, floorobj->object, "Connection Walls");
		sbs->AddDoorwayWalls(wall, "ConnectionWall", 0, 0);

		sbs->ResetWalls();
		sbs->ResetTextureMapping();
	}

	//create connection pieces
	sbs->ResetTextureMapping(true);
	WallObject *wall;
	csString name1, name2;
	if (ShaftDoor == false)
	{
		wall = new WallObject(elev->ElevatorMesh, elev->object, true);
		name1 = "DoorF1";
		name2 = "DoorF2";
	}
	else
	{
		Shaft *shaft = sbs->GetShaft(elev->AssignedShaft);
		wall = new WallObject(shaft->GetMeshWrapper(floor), shaft->object, true);
		name1 = "ShaftDoorF1";
		name2 = "ShaftDoorF2";
		x1 += elev->Origin.x;
		x2 += elev->Origin.x;
		z1 += elev->Origin.z;
		z2 += elev->Origin.z;
	}
	sbs->CreateWallBox(wall, name1, "Connection", x1, x2, z1, z2, 1, -1.001 + voffset, 0, 0, false, true, true, true);
	sbs->CreateWallBox(wall, name2, "Connection", x1, x2, z1, z2, 1, wrapper->Height + 0.001 + voffset, 0, 0, false, true, true, true);
	delete wall;

	sbs->ResetTextureMapping();

	//relocate sound object
	if (ShaftDoor == false)
		doorsound->SetPosition(csVector3(wrapper->Origin.x, wrapper->Origin.y + (wrapper->Height / 2), wrapper->Origin.z));
	else
		chime->SetPosition(csVector3(wrapper->Origin.x, wrapper->Origin.y + (wrapper->Height / 2), wrapper->Origin.z));

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

	//exit if floor is not serviced by the elevator
	if (!elev->IsServicedFloor(floor))
		return 0;

	Floor *floorobj = sbs->GetFloor(floor);
	return FinishDoors(ShaftDoors[elev->ServicedFloors.Find(floor)], floor, true, floorobj->Altitude + floorobj->GetBase(true));
}

bool ElevatorDoor::FinishShaftDoors()
{
	//finish all shaft doors

	for (size_t i = 0; i < elev->ServicedFloors.GetSize(); i++)
	{
		if (!FinishShaftDoor(elev->ServicedFloors[i]))
			return false;
	}
	return true;
}

bool ElevatorDoor::AddShaftDoors(const char *lefttexture, const char *righttexture, float thickness, float CenterX, float CenterZ, float tw, float th)
{
	//adds shaft's elevator doors specified at a relative central position (off of elevator origin)
	//uses some parameters (width, height, direction) from AddDoor/AddDoors function

	//set door parameters
	ShaftDoorOrigin = csVector3(elev->Origin.x + CenterX, 0, elev->Origin.z + CenterZ);
	ShaftDoorThickness = thickness;

	//create doors
	for (size_t i = 0; i < elev->ServicedFloors.GetSize(); i++)
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
	int index = elev->ServicedFloors.Find(floor);

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

	csString buffer, buffer2, buffer3, buffer4, buffer5;

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
	ShaftDoorOrigin = csVector3(elev->Origin.x + CenterX, 0, elev->Origin.z + CenterZ);
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
	size_t index = elev->ServicedFloors.Find(floor);
	if (index == csArrayItemNotFound)
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
		return ShaftDoors[elev->ServicedFloors.Find(floor)]->Open;
	return false;
}

float ElevatorDoor::GetCurrentDoorSpeed()
{
	//returns the internal door speed value
	//return ElevatorDoorSpeed;
	return 0;
}

void ElevatorDoor::Timer::Notify()
{
	//door autoclose timer

	//close doors if open
	if (door->AreDoorsOpen() == true && elevator->InServiceMode() == false)
		door->CloseDoors();
}

void ElevatorDoor::Chime(int floor, bool direction)
{
	//play chime sound on specified floor
	if (direction == false)
		chime->Load(DownChimeSound);
	else
		chime->Load(UpChimeSound);
	chime->Loop(false);
	chime->SetPositionY(sbs->GetFloor(floor)->GetBase() + Doors->Height);
	chime->Play();
}

void ElevatorDoor::ResetDoorTimer()
{
	//reset elevator door timer
	timer->Stop();
	timer->Start(DoorTimer, true);
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

void ElevatorDoor::Move(const csVector3 position, bool relative_x, bool relative_y, bool relative_z)
{
	//moves doors

	for (int i = 0; i < Doors->doors.GetSize(); i++)
	{
		csVector3 pos;
		if (relative_x == false)
			pos.x = sbs->ToRemote(position.x);
		else
			pos.x = Doors->doors[i]->movable->GetPosition().x + sbs->ToRemote(position.x);
		if (relative_y == false)
			pos.y = sbs->ToRemote(position.y);
		else
			pos.y = Doors->doors[i]->movable->GetPosition().y + sbs->ToRemote(position.y);
		if (relative_z == false)
			pos.z = sbs->ToRemote(position.z);
		else
			pos.z = Doors->doors[i]->movable->GetPosition().z + sbs->ToRemote(position.z);
		Doors->doors[i]->movable->SetPosition(pos);
		Doors->doors[i]->movable->UpdateMove();
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

void ElevatorDoor::MoveSound(const csVector3 position, bool relative_x, bool relative_y, bool relative_z)
{
	//move sound
	csVector3 pos;
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
}

bool ElevatorDoor::ShaftDoorsExist(int floor)
{
	//return true if shaft doors have been created for this door on the specified floor

	int index = elev->ServicedFloors.Find(floor);
	if (index != -1 && ShaftDoors[index]->doors.GetSize() > 0)
		return true;
	return false;
}

ElevatorDoor::DoorObject::DoorObject(const char *doorname, DoorWrapper *Wrapper, const char *Direction, float Speed)
{
	name = doorname;
	wrapper = Wrapper;
	parent = wrapper->parent;

	//create object mesh
	mesh = sbs->engine->CreateSectorWallsMesh (sbs->area, doorname);
	state = scfQueryInterface<iThingFactoryState> (mesh->GetMeshObject()->GetFactory());
	movable = mesh->GetMovable();
	mesh->SetZBufMode(CS_ZBUF_USE);
	mesh->SetRenderPriority(sbs->engine->GetObjectRenderPriority());
	
	csString direction_check = Direction;
	direction_check.Downcase().Trim();

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
	//clear references
	movable = 0;
	state = 0;
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
	object->SetValues(this, parent->object, "DoorWrapper", false);
}

ElevatorDoor::DoorWrapper::~DoorWrapper()
{
	for (int i = 0; i < doors.GetSize(); i++)
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
	
	doors.SetSize(doors.GetSize() + 1);
	int index = doors.GetSize() - 1;
	doors[index] = new DoorObject(doorname, this, direction, speed);

	//move object to positions
	if (IsShaftDoor == false)
		doors[index]->movable->SetPosition(sbs->ToRemote(parent->elev->Origin));
	else
		doors[index]->movable->SetPosition(sbs->ToRemote(csVector3(parent->elev->Origin.x, 0, parent->elev->Origin.z)));
	doors[index]->movable->UpdateMove();

	return doors[index];
}

void ElevatorDoor::DoorWrapper::Enable(bool value)
{
	//enable/disable door meshes

	if (value == Enabled)
		return;

	for (int i = 0; i < doors.GetSize(); i++)
		sbs->EnableMesh(doors[i]->mesh, value);
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

	if (direction > 1)
	{
		if (parent->DoorDirection == false)
		{
			tempposition = sbs->ToLocal(movable->GetPosition().z) + wrapper->Shift;
			temporigin = wrapper->Origin.z + wrapper->Shift;
		}
		else
		{
			tempposition = sbs->ToLocal(movable->GetPosition().x) + wrapper->Shift;
			temporigin = wrapper->Origin.x + wrapper->Shift;
		}
	}
	else
	{
		tempposition = sbs->ToLocal(movable->GetPosition().y);
		if (wrapper->IsShaftDoor == false)
			temporigin = parent->elev->GetPosition().y;
		else
			temporigin = 0;
	}

	float difference = fabs(tempposition - temporigin);

	if (old_difference != 0 && manual == true && recheck_difference == true)
	{
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
	//sbs->Report("Door section: " + csString(_itoa(door_section, parent->intbuffer, 10)));

	if (parent->door_changed == false && door_section == 0)
	{
		//initialization code

		finished = false;
		recheck_difference = false;

		//marker1 is the position to stop accelerating at
		//marker2 is the position to start decelerating at
		if (manual == false)
		{
			openchange = speed / 50;
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
				float newwidth = width + offset;
				marker1 = newwidth / 4;
				marker2 = (mainwidth + (width - mainwidth)) - (newwidth / 4) + offset;
			}
			else
			{
				float height = fabs(extents_max.y - extents_min.y);
				float mainheight = wrapper->Height / 2;
				offset = extents_min.y - wrapper->Origin.y;
				if (direction == 0)
					offset = wrapper->Height - (extents_max.y - wrapper->Origin.y);
				else
					offset = extents_min.y - wrapper->Origin.y;
				float newheight = height + offset;
				marker1 = newheight / 4;
				marker2 = (mainheight + (height - mainheight)) - (newheight / 4) + offset;
			}
			active_speed = 0;
		}
		else
		{
			//manual movement speed
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
					offset = wrapper->Height - (extents_max.y - wrapper->Origin.y);
				else
					offset = extents_min.y - wrapper->Origin.y;
				marker1 = 0;
				marker2 = mainheight + (height - mainheight) + offset;
			}

			if (open == true)
				active_speed = 0.2;
			else
				active_speed = -0.2;
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
	//sbs->Report("Door section: " + csString(_itoa(door_section, parent->intbuffer, 10)));

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
					movable->SetPosition(sbs->ToRemote(csVector3(parent->elev->Origin.x, ypos, parent->elev->Origin.z - (mainwidth + (width - mainwidth) + offset))));
				else
					movable->SetPosition(sbs->ToRemote(csVector3(parent->elev->Origin.x, ypos, parent->elev->Origin.z + (mainwidth + (width - mainwidth) + offset))));
			}
			else
			{
				float width = fabs(extents_max.x - extents_min.x);
				if (direction == 2)
					movable->SetPosition(sbs->ToRemote(csVector3(parent->elev->Origin.x - (mainwidth + (width - mainwidth) + offset), ypos, parent->elev->Origin.z)));
				else
					movable->SetPosition(sbs->ToRemote(csVector3(parent->elev->Origin.x + (mainwidth + (width - mainwidth) + offset), ypos, parent->elev->Origin.z)));
			}
		}
		else
		{
			float mainheight = wrapper->Height / 2;
			float height = fabs(extents_max.y - extents_min.y);
			if (direction == 0)
				movable->SetPosition(sbs->ToRemote(csVector3(parent->elev->Origin.x, ypos + (mainheight + (height - mainheight) + offset), parent->elev->Origin.z)));
			else
				movable->SetPosition(sbs->ToRemote(csVector3(parent->elev->Origin.x, ypos - (mainheight + (height - mainheight) + offset), parent->elev->Origin.z)));
		}
	}
	else
	{
		movable->SetPosition(sbs->ToRemote(csVector3(parent->elev->Origin.x, ypos, parent->elev->Origin.z)));
	}
	movable->UpdateMove();

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
			movable->MovePosition(csVector3(0, sbs->ToRemote(active_speed * sbs->delta), 0));

		//down movement
		if (direction == 1)
			movable->MovePosition(csVector3(0, sbs->ToRemote(-active_speed * sbs->delta), 0));

		if (parent->DoorDirection == false)
		{
			//left movement
			if (direction == 2)
				movable->MovePosition(csVector3(0, 0, sbs->ToRemote(-active_speed * sbs->delta)));

			//right movement
			if (direction == 3)
				movable->MovePosition(csVector3(0, 0, sbs->ToRemote(active_speed * sbs->delta)));
		}
		else
		{
			//left movement
			if (direction == 2)
				movable->MovePosition(csVector3(sbs->ToRemote(-active_speed * sbs->delta), 0, 0));

			//right movement
			if (direction == 3)
				movable->MovePosition(csVector3(sbs->ToRemote(active_speed * sbs->delta), 0, 0));
		}

		movable->UpdateMove();
}

void ElevatorDoor::DoorWrapper::MoveDoors(bool open, bool manual)
{
	//calls per-door move function
	for (int i = 0; i < doors.GetSize(); i++)
		doors[i]->MoveDoors(open, manual);
}

bool ElevatorDoor::DoorWrapper::CheckDoorsOpen()
{
	//checks to see if doors are open or closed, and returns true if the status changed

	for (int i = 0; i < doors.GetSize(); i++)
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

	for (int i = 0; i < doors.GetSize(); i++)
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

	for (int i = 0; i < doors.GetSize(); i++)
	{
		doors[i]->active_speed = 0;
		doors[i]->door_section = 0;
	}
}
