/* $Id$ */

/*
	Scalable Building Simulator - Elevator Door Class
	The Skyscraper Project - Version 1.6 Alpha
	Copyright (C)2005-2009 Ryan Thoryk
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
#include "elevatordoor.h"
#include "sbs.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

ElevatorDoor::ElevatorDoor(int number, Elevator* elevator)
{
	//create a new elevator door
	Number = number + 1;
	elev = elevator;
	OpenDoor = 0;
	OpenSpeed = sbs->confman->GetFloat("Skyscraper.SBS.Elevator.Door.OpenSpeed", 0.3);
	DoorsOpen = false;
	ElevatorDoorSpeed = 0;
	WhichDoors = 0;
	ShaftDoorFloor = 0;
	DoorAcceleration = 0;
	DoorTimer = sbs->confman->GetInt("Skyscraper.SBS.Elevator.Door.Timer", 5000);
	DoorIsRunning = false;
	OpenChange = 0;
	marker1 = 0;
	marker2 = 0;
	index = 0;
	stopping_distance = 0;
	temp_change = 0;
	accelerating = false;
	previous_open = false;
	door_changed = false;
	door_section = 0;
	quick_close = false;
	OpenSound = sbs->confman->GetStr("Skyscraper.SBS.Elevator.Door.OpenSound", "elevatoropen.wav");
	CloseSound = sbs->confman->GetStr("Skyscraper.SBS.Elevator.Door.CloseSound", "elevatorclose.wav");
	UpChimeSound = sbs->confman->GetStr("Skyscraper.SBS.Elevator.Door.UpChimeSound", "chime1-up.wav");
	DownChimeSound = sbs->confman->GetStr("Skyscraper.SBS.Elevator.Door.DownChimeSound", "chime1-down.wav");
	doors_stopped = false;
	IsEnabled = true;
	ShaftDoorThickness = 0;

	//create object meshes
	csString elevnumber, doornumber, buffer;
	elevnumber = elev->Number;
	elevnumber.Trim();
	doornumber = Number;
	doornumber.Trim();
	buffer = "ElevatorDoorL " + elevnumber + ":" + doornumber;
	buffer.Trim();
	ElevatorDoorL = sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData());
	ElevatorDoorL_state = scfQueryInterface<iThingFactoryState> (ElevatorDoorL->GetMeshObject()->GetFactory());
	ElevatorDoorL_movable = ElevatorDoorL->GetMovable();
	ElevatorDoorL->SetZBufMode(CS_ZBUF_USE);
	ElevatorDoorL->SetRenderPriority(sbs->engine->GetObjectRenderPriority());

	elevnumber = elev->Number;
	elevnumber.Trim();
	doornumber = Number;
	doornumber.Trim();
	buffer = "ElevatorDoorR " + elevnumber + ":" + doornumber;
	buffer.Trim();
	ElevatorDoorR = sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData());
	ElevatorDoorR_state = scfQueryInterface<iThingFactoryState> (ElevatorDoorR->GetMeshObject()->GetFactory());
	ElevatorDoorR_movable = ElevatorDoorR->GetMovable();
	ElevatorDoorR->SetZBufMode(CS_ZBUF_USE);
	ElevatorDoorR->SetRenderPriority(sbs->engine->GetObjectRenderPriority());

	//create timer
	timer = new Timer(this, elev);

	//move objects to positions
	ElevatorDoorL_movable->SetPosition(elev->Origin);
	ElevatorDoorL_movable->UpdateMove();
	ElevatorDoorR_movable->SetPosition(elev->Origin);
	ElevatorDoorR_movable->UpdateMove();

	//resize shaft door arrays
	ShaftDoorL.SetSize(elev->ServicedFloors.GetSize());
	ShaftDoorR.SetSize(elev->ServicedFloors.GetSize());
	ShaftDoorL_state.SetSize(elev->ServicedFloors.GetSize());
	ShaftDoorR_state.SetSize(elev->ServicedFloors.GetSize());
	ShaftDoorsOpen.SetSize(elev->ServicedFloors.GetSize());
	ShaftDoorsState.SetSize(elev->ServicedFloors.GetSize());

	//create sound object
	doorsound = new Sound("Door Sound");
	doorsound->SetPosition(elevator->Origin);
	chime = new Sound("Chime");
}

ElevatorDoor::~ElevatorDoor()
{
	//delete mesh array objects
	for (int i = 0; i < ShaftDoorL_state.GetSize(); i++)
		ShaftDoorL_state[i] = 0;
	for (int i = 0; i < ShaftDoorL.GetSize(); i++)
		ShaftDoorL[i] = 0;
	for (int i = 0; i < ShaftDoorR_state.GetSize(); i++)
		ShaftDoorR_state[i] = 0;
	for (int i = 0; i < ShaftDoorR.GetSize(); i++)
		ShaftDoorR[i] = 0;
	ShaftDoorL_state.DeleteAll();
	ShaftDoorL.DeleteAll();
	ShaftDoorR_state.DeleteAll();
	ShaftDoorR.DeleteAll();

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

	ElevatorDoorR_movable = 0;
	ElevatorDoorR_state = 0;
	ElevatorDoorR = 0;
	ElevatorDoorL_movable = 0;
	ElevatorDoorL_state = 0;
	ElevatorDoorL = 0;
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
	if (DoorsOpen == true && whichdoors != 3 && OpenDoor == 0 && doors_stopped == false)
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
	if (OpenDoor = 2 && DoorIsRunning == true)
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
		return;

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

	//do not close doors while in fire service mode 1 (in on position)
	if (manual == false && elev->FireServicePhase1 == 1)
	{
		sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + ": cannot close doors" + doornumber + " while Fire Service Phase 1 is on");
		return;
	}

	//if called while doors are opening, set quick_close (causes door timer to trigger faster)
	if (OpenDoor != 0 && manual == false)
	{
		sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + ": will close doors" + doornumber + " one second after staying open");
		quick_close = true;
		return;
	}

	//check if elevator doors are already closed
	if (DoorsOpen == false && whichdoors != 3 && OpenDoor == 0 && doors_stopped == false)
	{
		sbs->Report("Elevator " + csString(_itoa(elev->Number, intbuffer, 10)) + ": doors" + doornumber + " already closed");
		return;
	}

	//exit if doors are manually closing
	if (OpenDoor = -2 && DoorIsRunning == true)
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
		return;

	//turn off directional indicators
	if (whichdoors == 1)
	{
		DirectionalIndicator *indicator = elev->GetIndicator(elev->GetFloor());
		if (indicator)
		{
			indicator->DownLight(false);
			indicator->UpLight(false);
		}
	}

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

		DoorIsRunning = false;
		OpenDoor = 0;
		ElevatorDoorSpeed = 0;
		WhichDoors = 0;
		door_section = 0;
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
	//currently only supports doors on either the left/right or front/back
	//diagonal doors will be done later, by possibly using relative plane movement

	//WhichDoors is the doors to move:
	//1 = both shaft and elevator doors
	//2 = only elevator doors
	//3 = only shaft doors

	//ShaftDoorFloor is the floor the shaft doors are on - only has effect if whichdoors is 3

	//this door system is based on offsets from the door origin (usually 0).
	//the right (positive side) door is used as a reference, with the leftmost side of it being the position.
	//when opening, the door starts and the origin, accelerates to marker 1, moves at a constant
	//rate to marker 2, and then decelerates after marker 2.
	//this offset system is not used if manual is true (in that case, it simply sets a speed value, and moves
	//the doors until they reach the ends

	csString doornumber;
	if (elev->NumDoors > 1)
	{
		doornumber = " ";
		doornumber = doornumber + _itoa(Number, intbuffer, 10);
	}

	//stop timer
	timer->Stop();

	//get movable object reference
	csRef<iMovable> tmpMovable;
	if (WhichDoors < 3)
		tmpMovable = ElevatorDoorR_movable;
	else
		tmpMovable = ShaftDoorR[index]->GetMovable();

	float tempposition, temporigin;
	if (DoorDirection == false)
	{
		tempposition = tmpMovable->GetPosition().z;
		temporigin = DoorOrigin.z;
	}
	else
	{
		tempposition = tmpMovable->GetPosition().x;
		temporigin = DoorOrigin.x;
	}

	//debug - show current section as function is running
	//sbs->Report("Door section: " + csString(_itoa(door_section, intbuffer, 10)));

	if (DoorIsRunning == false || (manual == true && previous_open != open))
	{
		//initialization code

		DoorIsRunning = true;
		door_changed = false;
		door_section = 0;

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
			OpenChange = OpenSpeed / 50;
			marker1 = DoorWidth / 8;
			marker2 = (DoorWidth / 2) - (DoorWidth / 8);
			ElevatorDoorSpeed = 0;
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
		else
		{
			marker1 = 0;
			marker2 = DoorWidth / 2;
			if (open == true)
				ElevatorDoorSpeed = 0.2;
			else
				ElevatorDoorSpeed = -0.2;
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
		//only change directions immediately if re-opening (closing, then opening)
		if (open == true)
		{
			//relocate marker 1 to the door's current position, in order to stop it
			float offset = marker1 - (tempposition - temporigin);
			if (tempposition - temporigin >= marker1)
				//place marker at door position
				marker1 = tempposition - temporigin;
			else
				//place marker to the right based on the offset, to bring door back to full speed
				marker1 = tempposition - temporigin + offset;
		}
	}

	//update call status (previous_open detects call changes during movement)
	previous_open = open;

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

	//Speed up doors (only if manual is false)
	if (manual == false)
	{
		//if door is opening and is left of marker 1
		//or if door is closing and is to the right of marker 2
		if ((tempposition - temporigin <= marker1 && open == true) || (tempposition - temporigin > marker2 && open == false))
		{
			accelerating = true;
			if (door_changed == false)
			{
				//normal door acceleration
				if (open == true)
					ElevatorDoorSpeed += OpenChange;
				else
					ElevatorDoorSpeed -= OpenChange;
			}
			else
			{
				//reverse movement if transitioning from close to open
				//this will trigger if door is closing, and is told to open while left of relocated marker 1
				if (tempposition - temporigin <= marker1)
					ElevatorDoorSpeed += OpenChange;
			}

			if (elevdoors == true)
			{
				if (DoorDirection == false)
				{
					//move elevator doors
					ElevatorDoorL_movable->MovePosition(csVector3(0, 0, -ElevatorDoorSpeed * sbs->delta));
					ElevatorDoorL_movable->UpdateMove();
					ElevatorDoorR_movable->MovePosition(csVector3(0, 0, ElevatorDoorSpeed * sbs->delta));
					ElevatorDoorR_movable->UpdateMove();
				}
				else
				{
					//move elevator doors
					ElevatorDoorL_movable->MovePosition(csVector3(-ElevatorDoorSpeed * sbs->delta, 0, 0));
					ElevatorDoorL_movable->UpdateMove();
					ElevatorDoorR_movable->MovePosition(csVector3(ElevatorDoorSpeed * sbs->delta, 0, 0));
					ElevatorDoorR_movable->UpdateMove();
				}
			}

			if (shaftdoors == true && ShaftDoorL[index])
			{
				if (DoorDirection == false)
				{
					//move shaft doors
					ShaftDoorL[index]->GetMovable()->MovePosition(csVector3(0, 0, -ElevatorDoorSpeed * sbs->delta));
					ShaftDoorL[index]->GetMovable()->UpdateMove();
					ShaftDoorR[index]->GetMovable()->MovePosition(csVector3(0, 0, ElevatorDoorSpeed * sbs->delta));
					ShaftDoorR[index]->GetMovable()->UpdateMove();
				}
				else
				{
					//move shaft doors
					ShaftDoorL[index]->GetMovable()->MovePosition(csVector3(-ElevatorDoorSpeed * sbs->delta, 0, 0));
					ShaftDoorL[index]->GetMovable()->UpdateMove();
					ShaftDoorR[index]->GetMovable()->MovePosition(csVector3(ElevatorDoorSpeed * sbs->delta, 0, 0));
					ShaftDoorR[index]->GetMovable()->UpdateMove();
				}
			}

			//get stopping distance
			stopping_distance = tempposition - temporigin;

			door_section = 1;
			return;
		}
	}

	door_section = 2;

	//Normal door movement
	if ((tempposition - temporigin <= marker2 && open == true) || (tempposition - temporigin > marker1 && open == false))
	{
		if (elevdoors == true)
		{
			if (DoorDirection == false)
			{
				//move elevator doors
				ElevatorDoorL_movable->MovePosition(csVector3(0, 0, -ElevatorDoorSpeed * sbs->delta));
				ElevatorDoorL_movable->UpdateMove();
				ElevatorDoorR_movable->MovePosition(csVector3(0, 0, ElevatorDoorSpeed * sbs->delta));
				ElevatorDoorR_movable->UpdateMove();
			}
			else
			{
				//move elevator doors
				ElevatorDoorL_movable->MovePosition(csVector3(-ElevatorDoorSpeed * sbs->delta, 0, 0));
				ElevatorDoorL_movable->UpdateMove();
				ElevatorDoorR_movable->MovePosition(csVector3(ElevatorDoorSpeed * sbs->delta, 0, 0));
				ElevatorDoorR_movable->UpdateMove();
			}
		}

		if (shaftdoors == true && ShaftDoorL[index])
		{
			if (DoorDirection == false)
			{
				//move shaft doors
				ShaftDoorL[index]->GetMovable()->MovePosition(csVector3(0, 0, -ElevatorDoorSpeed * sbs->delta));
				ShaftDoorL[index]->GetMovable()->UpdateMove();
				ShaftDoorR[index]->GetMovable()->MovePosition(csVector3(0, 0, ElevatorDoorSpeed * sbs->delta));
				ShaftDoorR[index]->GetMovable()->UpdateMove();
			}
			else
			{
				//move shaft doors
				ShaftDoorL[index]->GetMovable()->MovePosition(csVector3(-ElevatorDoorSpeed * sbs->delta, 0, 0));
				ShaftDoorL[index]->GetMovable()->UpdateMove();
				ShaftDoorR[index]->GetMovable()->MovePosition(csVector3(ElevatorDoorSpeed * sbs->delta, 0, 0));
				ShaftDoorR[index]->GetMovable()->UpdateMove();
			}
		}
		door_section = 3;
		return;
	}

	accelerating = false;

	//slow down doors (only if manual is false)
	if (manual == false)
	{
		if ((ElevatorDoorSpeed > 0 && open == true) || (ElevatorDoorSpeed < 0 && open == false))
		{
			if (open == true)
				ElevatorDoorSpeed -= OpenChange;
			else
				ElevatorDoorSpeed += OpenChange;
			if (DoorDirection == false)
			{
				if (elevdoors == true)
				{
					//move elevator doors
					ElevatorDoorL_movable->MovePosition(csVector3(0, 0, -ElevatorDoorSpeed * sbs->delta));
					ElevatorDoorL_movable->UpdateMove();
					ElevatorDoorR_movable->MovePosition(csVector3(0, 0, ElevatorDoorSpeed * sbs->delta));
					ElevatorDoorR_movable->UpdateMove();
				}

				if (shaftdoors == true && ShaftDoorL[index])
				{
					//move shaft doors
					ShaftDoorL[index]->GetMovable()->MovePosition(csVector3(0, 0, -ElevatorDoorSpeed * sbs->delta));
					ShaftDoorL[index]->GetMovable()->UpdateMove();
					ShaftDoorR[index]->GetMovable()->MovePosition(csVector3(0, 0, ElevatorDoorSpeed * sbs->delta));
					ShaftDoorR[index]->GetMovable()->UpdateMove();
				}
			}
			else
			{
				if (elevdoors == true)
				{
					//move elevator doors
					ElevatorDoorL_movable->MovePosition(csVector3(-ElevatorDoorSpeed * sbs->delta, 0, 0));
					ElevatorDoorL_movable->UpdateMove();
					ElevatorDoorR_movable->MovePosition(csVector3(ElevatorDoorSpeed * sbs->delta, 0, 0));
					ElevatorDoorR_movable->UpdateMove();
				}

				if (shaftdoors == true && ShaftDoorL[index])
				{
					//move shaft doors
					ShaftDoorL[index]->GetMovable()->MovePosition(csVector3(-ElevatorDoorSpeed * sbs->delta, 0, 0));
					ShaftDoorL[index]->GetMovable()->UpdateMove();
					ShaftDoorR[index]->GetMovable()->MovePosition(csVector3(ElevatorDoorSpeed * sbs->delta, 0, 0));
					ShaftDoorR[index]->GetMovable()->UpdateMove();
				}
			}
			door_section = 4;
			return;
		}
	}

	//report on what section preceded the finishing code (should be 4)
	//sbs->Report("Door section: " + csString(_itoa(door_section, intbuffer, 10)));

	//place doors in positions (fixes any overrun errors)
	if (open == true)
	{
		if (DoorDirection == false)
		{
			if (elevdoors == true)
			{
				//move elevator doors
				ElevatorDoorL_movable->SetPosition(csVector3(elev->Origin.x, elev->GetPosition().y, elev->Origin.z - (DoorWidth / 2)));
				ElevatorDoorL_movable->UpdateMove();
				ElevatorDoorR_movable->SetPosition(csVector3(elev->Origin.x, elev->GetPosition().y, elev->Origin.z + (DoorWidth / 2)));
				ElevatorDoorR_movable->UpdateMove();
			}

			if (shaftdoors == true && ShaftDoorL[index])
			{
				//move shaft doors
				ShaftDoorL[index]->GetMovable()->SetPosition(csVector3(elev->Origin.x, 0, elev->Origin.z - (DoorWidth / 2)));
				ShaftDoorL[index]->GetMovable()->UpdateMove();
				ShaftDoorR[index]->GetMovable()->SetPosition(csVector3(elev->Origin.x, 0, elev->Origin.z + (DoorWidth / 2)));
				ShaftDoorR[index]->GetMovable()->UpdateMove();
			}
		}
		else
		{
			if (elevdoors == true)
			{
				//move elevator doors
				ElevatorDoorL_movable->SetPosition(csVector3(elev->Origin.x - (DoorWidth / 2), elev->GetPosition().y, elev->Origin.z));
				ElevatorDoorL_movable->UpdateMove();
				ElevatorDoorR_movable->SetPosition(csVector3(elev->Origin.x + (DoorWidth / 2), elev->GetPosition().y, elev->Origin.z));
				ElevatorDoorR_movable->UpdateMove();
			}

			if (shaftdoors == true && ShaftDoorL[index])
			{
				//move shaft doors
				ShaftDoorL[index]->GetMovable()->SetPosition(csVector3(elev->Origin.x - (DoorWidth / 2), 0, elev->Origin.z));
				ShaftDoorL[index]->GetMovable()->UpdateMove();
				ShaftDoorR[index]->GetMovable()->SetPosition(csVector3(elev->Origin.x + (DoorWidth / 2), 0, elev->Origin.z));
				ShaftDoorR[index]->GetMovable()->UpdateMove();
			}
		}
	}
	else
	{
		if (DoorDirection == false)
		{
			if (elevdoors == true)
			{
				//move elevator doors
				ElevatorDoorL_movable->SetPosition(csVector3(elev->Origin.x, elev->GetPosition().y, elev->Origin.z));
				ElevatorDoorL_movable->UpdateMove();
				ElevatorDoorR_movable->SetPosition(csVector3(elev->Origin.x, elev->GetPosition().y, elev->Origin.z));
				ElevatorDoorR_movable->UpdateMove();
			}

			if (shaftdoors == true && ShaftDoorL[index])
			{
				//move shaft doors
				ShaftDoorL[index]->GetMovable()->SetPosition(csVector3(elev->Origin.x, 0, elev->Origin.z));
				ShaftDoorL[index]->GetMovable()->UpdateMove();
				ShaftDoorR[index]->GetMovable()->SetPosition(csVector3(elev->Origin.x, 0, elev->Origin.z));
				ShaftDoorR[index]->GetMovable()->UpdateMove();
			}
		}
		else
		{
			if (elevdoors == true)
			{
				//move elevator doors
				ElevatorDoorL_movable->SetPosition(csVector3(elev->Origin.x, elev->GetPosition().y, elev->Origin.z));
				ElevatorDoorL_movable->UpdateMove();
				ElevatorDoorR_movable->SetPosition(csVector3(elev->Origin.x, elev->GetPosition().y, elev->Origin.z));
				ElevatorDoorR_movable->UpdateMove();
			}

			if (shaftdoors == true && ShaftDoorL[index])
			{
				//move shaft doors
				ShaftDoorL[index]->GetMovable()->SetPosition(csVector3(elev->Origin.x, 0, elev->Origin.z));
				ShaftDoorL[index]->GetMovable()->UpdateMove();
				ShaftDoorR[index]->GetMovable()->SetPosition(csVector3(elev->Origin.x, 0, elev->Origin.z));
				ShaftDoorR[index]->GetMovable()->UpdateMove();
			}
		}
	}

	//the doors are open or closed now
	if (WhichDoors != 3)
	{
		if (open == true)
			DoorsOpen = true;
		else
		{
			DoorsOpen = false;
			//move elevator if in pending state
			if (elev->MovePending == true)
			{
				elev->MoveElevator = true;
				elev->MovePending = false;
			}
		}
	}
	else
	{
		if (open == true)
			ShaftDoorsOpen[elev->ServicedFloors.Find(ShaftDoorFloor)] = true;
		else
			ShaftDoorsOpen[elev->ServicedFloors.Find(ShaftDoorFloor)] = false;

		//turn off related floor
		if (open == false && (sbs->InShaft == true || sbs->InElevator == true))
		{
			sbs->GetFloor(ShaftDoorFloor)->Enabled(false);
			sbs->GetFloor(ShaftDoorFloor)->EnableGroup(false);
		}
	}

	//reset values
	ElevatorDoorSpeed = 0;
	OpenDoor = 0;
	WhichDoors = 0;
	door_section = 0;
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
			timer->Start(1000, true);
		quick_close = false;
	}

	DoorIsRunning = false;
}

int ElevatorDoor::AddDoors(const char *lefttexture, const char *righttexture, float thickness, float CenterX, float CenterZ, float width, float height, bool direction, float tw, float th)
{
	//adds elevator doors specified at a relative central position (off of elevator origin)
	//if direction is false, doors are on the left/right side; otherwise front/back
	float x1, x2, x3, x4;
	float z1, z2, z3, z4;
	float spacing = 0.025f; //spacing between doors

	if (tw == 0)
		tw = 1;
	if (th == 0)
		th = 1;

	//set door parameters
	DoorDirection = direction;
	DoorWidth = width;
	DoorHeight = height;
	DoorOrigin = csVector3(elev->Origin.x + CenterX, elev->Origin.y, elev->Origin.z + CenterZ);

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

	//create doors
	sbs->DrawWalls(true, true, true, true, true, true);
	sbs->ResetTextureMapping(true);
	int firstidx = sbs->AddWallMain(ElevatorDoorL, "Door", lefttexture, thickness, x1, z1, x2, z2, height, height, 0, 0, tw, th);
	sbs->AddWallMain(ElevatorDoorR, "Door", righttexture, thickness, x3, z3, x4, z4, height, height, 0, 0, tw, th);
	//create connection pieces
	sbs->AddWallMain(elev->ElevatorMesh, "DoorF1", "Connection", thickness, x1, z1, x4, z4, 1, 1, -1.001, -1.001, 0, 0);
	sbs->AddWallMain(elev->ElevatorMesh, "DoorF2", "Connection", thickness, x1, z1, x4, z4, 1, 1, height + 0.001, height + 0.001, 0, 0);
	sbs->ResetWalls();
	sbs->ResetTextureMapping();
	//relocate sound object
	doorsound->SetPosition(csVector3(DoorOrigin.x, DoorOrigin.y + (DoorHeight / 2), DoorOrigin.z));
	return firstidx;
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

bool ElevatorDoor::AddShaftDoor(int floor, const char *lefttexture, const char *righttexture, float tw, float th)
{
	//adds a single elevator shaft door, with position and thickness parameters first specified
	//by the SetShaftDoors command.
	//uses some parameters (width, height, direction) from AddDoor/AddDoors function

	//exit if floor is not serviced by the elevator
	if (!elev->IsServicedFloor(floor))
		return false;

	float x1, x2, x3, x4;
	float z1, z2, z3, z4;
	float base, base2;

	if (tw == 0)
		tw = 1;
	if (th == 0)
		th = 1;

	//set up coordinates
	if (DoorDirection == false)
	{
		x1 = ShaftDoorOrigin.x - elev->Origin.x;
		x2 = ShaftDoorOrigin.x - elev->Origin.x;
		x3 = ShaftDoorOrigin.x - elev->Origin.x;
		x4 = ShaftDoorOrigin.x - elev->Origin.x;
		z1 = ShaftDoorOrigin.z - elev->Origin.z - (DoorWidth / 2);
		z2 = ShaftDoorOrigin.z - elev->Origin.z;
		z3 = ShaftDoorOrigin.z - elev->Origin.z;
		z4 = ShaftDoorOrigin.z - elev->Origin.z + (DoorWidth / 2);
	}
	else
	{
		x1 = ShaftDoorOrigin.x - elev->Origin.x - (DoorWidth / 2);
		x2 = ShaftDoorOrigin.x - elev->Origin.x;
		x3 = ShaftDoorOrigin.x - elev->Origin.x;
		x4 = ShaftDoorOrigin.x - elev->Origin.x + (DoorWidth / 2);
		z1 = ShaftDoorOrigin.z - elev->Origin.z;
		z2 = ShaftDoorOrigin.z - elev->Origin.z;
		z3 = ShaftDoorOrigin.z - elev->Origin.z;
		z4 = ShaftDoorOrigin.z - elev->Origin.z;
	}

	csString buffer, buffer2, buffer3, buffer4, buffer5;

	sbs->DrawWalls(true, true, true, true, true, true);
	sbs->ResetTextureMapping(true);

	//create doors
	Floor *floorobj = sbs->GetFloor(floor);
	Shaft *shaft = sbs->GetShaft(elev->AssignedShaft);
	base = floorobj->InterfloorHeight; //relative to floor
	base2 = floorobj->Altitude + base; //absolute

	//cut shaft and floor walls
	if (DoorDirection == false)
	{
		if (!shaft->CutWall(false, floor, csVector3(elev->Origin.x + x1 - 2, base, elev->Origin.z + z1), csVector3(elev->Origin.x + x1 + 2, base + DoorHeight, elev->Origin.z + z4), 1, "Shaft"))
			return false;
		floorobj->Cut(csVector3(elev->Origin.x + x1 - 2, base, elev->Origin.z + z1), csVector3(elev->Origin.x + x1 + 2, base + DoorHeight, elev->Origin.z + z4), true, false, true, 2, "Shaft");
	}
	else
	{
		if (!shaft->CutWall(false, floor, csVector3(elev->Origin.x + x1, base, elev->Origin.z + z1 - 2), csVector3(elev->Origin.x + x4, base + DoorHeight, elev->Origin.z + z1 + 2), 1, "Shaft"))
			return false;
		floorobj->Cut(csVector3(elev->Origin.x + x1, base, elev->Origin.z + z1 - 2), csVector3(elev->Origin.x + x4, base + DoorHeight, elev->Origin.z + z1 + 2), true, false, true, 2, "Shaft");
	}

	//create doorway walls
	sbs->AddDoorwayWalls(floorobj->Level, "ConnectionWall", 0, 0);

	//create meshes
	buffer3 = elev->Number;
	buffer4 = floor;
	buffer5 = Number;
	buffer = "Elevator " + buffer3 + ": Shaft Door " + buffer5 + ":" + buffer4 + "L";
	buffer2 = "Elevator " + buffer3 + ": Shaft Door " + buffer5 + ":" + buffer4 + "R";
	buffer.Trim();
	buffer2.Trim();
	csRef<iMeshWrapper> tmpmesh;
	csRef<iThingFactoryState> tmpstate;
	int index = elev->ServicedFloors.Find(floor);

	//door L
	tmpmesh = sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData());
	ShaftDoorL[index] = tmpmesh;
	tmpstate = scfQueryInterface<iThingFactoryState> (ShaftDoorL[index]->GetMeshObject()->GetFactory());
	ShaftDoorL_state[index] = tmpstate;
	ShaftDoorL[index]->SetZBufMode(CS_ZBUF_USE);
	ShaftDoorL[index]->SetRenderPriority(sbs->engine->GetObjectRenderPriority());

	//door R
	tmpmesh = sbs->engine->CreateSectorWallsMesh (sbs->area, buffer2.GetData());
	ShaftDoorR[index] = tmpmesh;
	tmpstate = scfQueryInterface<iThingFactoryState> (ShaftDoorR[index]->GetMeshObject()->GetFactory());
	ShaftDoorR_state[index] = tmpstate;
	ShaftDoorR[index]->SetZBufMode(CS_ZBUF_USE);
	ShaftDoorR[index]->SetRenderPriority(sbs->engine->GetObjectRenderPriority());

	//reposition meshes
	ShaftDoorL[index]->GetMovable()->SetPosition(csVector3(elev->Origin.x, 0, elev->Origin.z));
	ShaftDoorL[index]->GetMovable()->UpdateMove();
	ShaftDoorR[index]->GetMovable()->SetPosition(csVector3(elev->Origin.x, 0, elev->Origin.z));
	ShaftDoorR[index]->GetMovable()->UpdateMove();

	//create doors
	sbs->AddWallMain(ShaftDoorL[index], "Door", lefttexture, ShaftDoorThickness, x1, z1, x2, z2, DoorHeight, DoorHeight, base2, base2, tw, th);
	sbs->AddWallMain(ShaftDoorR[index], "Door", righttexture, ShaftDoorThickness, x3, z3, x4, z4, DoorHeight, DoorHeight, base2, base2, tw, th);

	//create connection pieces
	float xoffset = elev->Origin.x - shaft->origin.x;
	float zoffset = elev->Origin.z - shaft->origin.z;
	shaft->AddWall(floor, "ShaftDoorF1", "Connection", ShaftDoorThickness, xoffset + x1, zoffset + z1, xoffset + x4, zoffset + z4, 1, 1, base - 1.001, base - 1.001, 0, 0);
	shaft->AddWall(floor, "ShaftDoorF2", "Connection", ShaftDoorThickness, xoffset + x1, zoffset + z1, xoffset + x4, zoffset + z4, 1, 1, base + DoorHeight + 0.001, base + DoorHeight + 0.001, 0, 0);

	//make doors invisible on start
	sbs->EnableMesh(ShaftDoorL[index], false);
	sbs->EnableMesh(ShaftDoorR[index], false);

	floorobj = 0;
	shaft = 0;

	sbs->ResetWalls();
	sbs->ResetTextureMapping();

	//relocate chime sound object
	chime->SetPosition(csVector3(ShaftDoorOrigin.x, ShaftDoorOrigin.y + DoorHeight, ShaftDoorOrigin.z));

	return true;
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

	size_t index = elev->ServicedFloors.Find(floor);
	//exit if elevator doesn't service the requested floor
	if (index == csArrayItemNotFound)
		return;

	//exit if value is the same as the current state
	if (ShaftDoorsState[index] == value)
		return;

	//exit if the specified floor has no shaft doors
	if (!ShaftDoorL[index])
		return;

	sbs->EnableMesh(ShaftDoorL[index], value);
	sbs->EnableMesh(ShaftDoorR[index], value);

	//set enabled state
	ShaftDoorsState[index] = value;
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
	return DoorsOpen;
}

bool ElevatorDoor::AreShaftDoorsOpen(int floor)
{
	//returns the internal door state
	if (ShaftDoorsExist(floor))
		return ShaftDoorsOpen[elev->ServicedFloors.Find(floor)];
	return false;
}

float ElevatorDoor::GetCurrentDoorSpeed()
{
	//returns the internal door speed value
	return ElevatorDoorSpeed;
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
	chime->SetPositionY(sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + DoorHeight);
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
	csVector3 pos;
	if (relative_x == false)
		pos.x = position.x;
	else
		pos.x = ElevatorDoorL_movable->GetPosition().x + position.x;
	if (relative_y == false)
		pos.y = position.y;
	else
		pos.y = ElevatorDoorL_movable->GetPosition().y + position.y;
	if (relative_z == false)
		pos.z = position.z;
	else
		pos.z = ElevatorDoorL_movable->GetPosition().z + position.z;
	ElevatorDoorL_movable->SetPosition(pos);
	ElevatorDoorL_movable->UpdateMove();

	if (relative_x == false)
		pos.x = position.x;
	else
		pos.x = ElevatorDoorR_movable->GetPosition().x + position.x;
	if (relative_y == false)
		pos.y = position.y;
	else
		pos.y = ElevatorDoorR_movable->GetPosition().y + position.y;
	if (relative_z == false)
		pos.z = position.z;
	else
		pos.z = ElevatorDoorR_movable->GetPosition().z + position.z;
	ElevatorDoorR_movable->SetPosition(pos);
	ElevatorDoorR_movable->UpdateMove();
}

void ElevatorDoor::Enabled(bool value)
{
	sbs->EnableMesh(ElevatorDoorL, value);
	sbs->EnableMesh(ElevatorDoorR, value);
	IsEnabled = value;
}

bool ElevatorDoor::GetDoorsOpen()
{
	return DoorsOpen;
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
		pos.x = DoorOrigin.x + position.x;
	if (relative_y == false)
		pos.y = position.y + (DoorHeight / 2);
	else
		pos.y = DoorOrigin.y + position.y;
	if (relative_z == false)
		pos.z = position.z;
	else
		pos.z = DoorOrigin.z + position.z;
	doorsound->SetPosition(pos);
}

bool ElevatorDoor::ShaftDoorsExist(int floor)
{
	//return true if shaft doors have been created for this door on the specified floor

	int index = elev->ServicedFloors.Find(floor);
	if (index != -1 && ShaftDoorL[index])
		return true;
	return false;
}
