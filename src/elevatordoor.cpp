/* $Id$ */

/*
	Scalable Building Simulator - Elevator Door Class
	The Skyscraper Project - Version 1.4 Alpha
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

ElevatorDoor::ElevatorDoor(Elevator* elevator)
{
	//create a new elevator door
	Number = number;
	elev = elevator;
	OpenDoor = 0;
	OpenSpeed = 0.3;
	DoorsOpen = false;
	ElevatorDoorSpeed = 0;
	WhichDoors = 0;
	ShaftDoorFloor = 0;
	DoorAcceleration = 0;
	DoorTimer = 5000;
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
	OpenSound = "elevatoropen.wav";
	CloseSound = "elevatorclose.wav";
	ChimeSound = "chime1.wav";
	doors_stopped = false;

	//create object meshes
	buffer = Number;
	buffer.Insert(0, "ElevatorDoorL ");
	buffer.Trim();
	ElevatorDoorL = sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData());
	ElevatorDoorL_state = scfQueryInterface<iThingFactoryState> (ElevatorDoorL->GetMeshObject()->GetFactory());
	ElevatorDoorL_movable = ElevatorDoorL->GetMovable();
	ElevatorDoorL->SetZBufMode(CS_ZBUF_USE);
	ElevatorDoorL->SetRenderPriority(sbs->engine->GetAlphaRenderPriority());
	ElevatorDoorL->GetMeshObject()->SetMixMode(CS_FX_ALPHA);

	buffer = Number;
	buffer.Insert(0, "ElevatorDoorR ");
	buffer.Trim();
	ElevatorDoorR = sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData());
	ElevatorDoorR_state = scfQueryInterface<iThingFactoryState> (ElevatorDoorR->GetMeshObject()->GetFactory());
	ElevatorDoorR_movable = ElevatorDoorR->GetMovable();
	ElevatorDoorR->SetZBufMode(CS_ZBUF_USE);
	ElevatorDoorR->SetRenderPriority(sbs->engine->GetAlphaRenderPriority());
	ElevatorDoorR->GetMeshObject()->SetMixMode(CS_FX_ALPHA);

	//create timer
	timer = new Timer(this);

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

	//create sound object
	doorsound = new Sound();
	doorsound->SetPosition(Origin);
	chime = new Sound();
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

	OpenDoors(number, whichdoors, floor, true);
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

	CloseDoors(number, whichdoors, floor, true);
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

	//exit if trying to open doors while stopped
	if (manual == false && doors_stopped == true)
	{
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": cannot open doors; doors manually stopped");
		return;
	}

	//exit if direction changed (if doors have already switched from close to open)
	if (door_changed == true)
		return;

	//don't open doors if emergency stop is enabled
	if (OnFloor == false && whichdoors != 3 && manual == false)
	{
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": cannot open doors; emergency stop enabled");
		return;
	}

	//check if elevator doors are already open
	if (DoorsOpen == true && whichdoors != 3 && OpenDoor == 0 && doors_stopped == false)
	{
		//reset timer if not in a service mode
		if (InServiceMode() == false)
		{
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": doors already open; resetting timer");
			ResetDoorTimer();
		}
		else
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": doors already open");
		return;
	}

	//check if shaft doors are already open
	if (whichdoors == 3)
	{
		//first make sure the shaft doors are valid
		if (ServicedFloors.Find(floor) == csArrayItemNotFound)
		{
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": invalid shaft doors");
			return;
		}
		if (ShaftDoorsOpen[ServicedFloors.Find(floor)] == true)
		{
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": shaft doors already open on floor " + csString(_itoa(floor, intbuffer, 10)));
			return;
		}
		else if (manual == false)
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": opening shaft doors on floor " + csString(_itoa(floor, intbuffer, 10)));
		else
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": manually opening shaft doors on floor " + csString(_itoa(floor, intbuffer, 10)));
	}
	else if (manual == false)
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": opening doors");
	else
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": manually opening doors");

	if (manual == false)
		OpenDoor = 1;
	else
		OpenDoor = 2;

	//if both doors are selected but elevator is not on a floor, only open elevator doors
	if (whichdoors == 1 && OnFloor == false)
		whichdoors = 2;

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

	//exit if trying to open doors while stopped
	if (manual == false && doors_stopped == true)
	{
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": cannot close doors; doors manually stopped");
		return;
	}

	//do not close doors while in fire service mode 1 (in on position)
	if (manual == false && FireServicePhase1 == 1)
	{
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": cannot close doors while Fire Service Phase 1 is on");
		return;
	}

	//if called while doors are opening, set quick_close (causes door timer to trigger faster)
	if (OpenDoor != 0 && manual == false)
	{
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": will close doors 1 second after staying open");
		quick_close = true;
		return;
	}

	//check if elevator doors are already closed
	if (DoorsOpen == false && whichdoors != 3 && OpenDoor == 0 && doors_stopped == false)
	{
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": doors already closed");
		return;
	}

	//check if shaft doors are already closed
	if (whichdoors == 3)
	{
		//first make sure the shaft doors are valid
		if (ServicedFloors.Find(floor) == csArrayItemNotFound)
		{
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": invalid shaft doors");
			return;
		}
		if (ShaftDoorsOpen[ServicedFloors.Find(floor)] == false && whichdoors == 3)
		{
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": shaft doors already closed on floor " + csString(_itoa(floor, intbuffer, 10)));
			return;
		}
		else if (manual == false)
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": closing shaft doors on floor " + csString(_itoa(floor, intbuffer, 10)));
		else
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": manually closing shaft doors on floor " + csString(_itoa(floor, intbuffer, 10)));
	}
	else if (manual == false)
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": closing doors");
	else
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": manually closing doors");

	if (manual == false)
		OpenDoor = -1;
	else
		OpenDoor = -2;

	//if both doors are selected but elevator is not on a floor, only close elevator doors
	if (whichdoors == 1 && OnFloor == false)
		whichdoors = 2;

	//turn off directional indicators
	if (whichdoors == 1)
	{
		int index = ServicedFloors.Find(GetFloor());
		if (IndicatorArray[index])
		{
			IndicatorArray[index]->DownLight(false);
			IndicatorArray[index]->UpLight(false);
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

	if (OpenDoor == -2 || OpenDoor == 2)
	{
		if (WhichDoors == 3)
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": stopping shaft doors on floor " + csString(_itoa(ShaftDoorFloor, intbuffer, 10)));
		else
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": stopping doors");

		DoorIsRunning = false;
		OpenDoor = 0;
		ElevatorDoorSpeed = 0;
		WhichDoors = 0;
		door_section = 0;
		door_changed = false;
		doors_stopped = true;
	}
	else if (OpenDoor != 0)
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": can only stop doors in manual/emergency mode");
	else
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": cannot stop doors; no doors moving");
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
			index = ServicedFloors.Find(ShaftDoorFloor);
		else
			index = ServicedFloors.Find(GetFloor());
		if (index == -1)
		{
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": No shaft doors on current floor");
			if (WhichDoors == 3)
			{
				//reset and exit if only shaft doors selected for opening
				OpenDoor = 0;
				WhichDoors = 0;
				return;
			}
			//only open elevator doors if no shaft doors available
			WhichDoors = 2;
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
				ElevatorDoorL_movable->SetPosition(csVector3(Origin.x, GetPosition().y, Origin.z - (DoorWidth / 2)));
				ElevatorDoorL_movable->UpdateMove();
				ElevatorDoorR_movable->SetPosition(csVector3(Origin.x, GetPosition().y, Origin.z + (DoorWidth / 2)));
				ElevatorDoorR_movable->UpdateMove();
			}

			if (shaftdoors == true && ShaftDoorL[index])
			{
				//move shaft doors
				ShaftDoorL[index]->GetMovable()->SetPosition(csVector3(Origin.x, 0, Origin.z - (DoorWidth / 2)));
				ShaftDoorL[index]->GetMovable()->UpdateMove();
				ShaftDoorR[index]->GetMovable()->SetPosition(csVector3(Origin.x, 0, Origin.z + (DoorWidth / 2)));
				ShaftDoorR[index]->GetMovable()->UpdateMove();
			}
		}
		else
		{
			if (elevdoors == true)
			{
				//move elevator doors
				ElevatorDoorL_movable->SetPosition(csVector3(Origin.x - (DoorWidth / 2), GetPosition().y, Origin.z));
				ElevatorDoorL_movable->UpdateMove();
				ElevatorDoorR_movable->SetPosition(csVector3(Origin.x + (DoorWidth / 2), GetPosition().y, Origin.z));
				ElevatorDoorR_movable->UpdateMove();
			}

			if (shaftdoors == true && ShaftDoorL[index])
			{
				//move shaft doors
				ShaftDoorL[index]->GetMovable()->SetPosition(csVector3(Origin.x - (DoorWidth / 2), 0, Origin.z));
				ShaftDoorL[index]->GetMovable()->UpdateMove();
				ShaftDoorR[index]->GetMovable()->SetPosition(csVector3(Origin.x + (DoorWidth / 2), 0, Origin.z));
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
				ElevatorDoorL_movable->SetPosition(csVector3(Origin.x, GetPosition().y, Origin.z));
				ElevatorDoorL_movable->UpdateMove();
				ElevatorDoorR_movable->SetPosition(csVector3(Origin.x, GetPosition().y, Origin.z));
				ElevatorDoorR_movable->UpdateMove();
			}

			if (shaftdoors == true && ShaftDoorL[index])
			{
				//move shaft doors
				ShaftDoorL[index]->GetMovable()->SetPosition(csVector3(Origin.x, 0, Origin.z));
				ShaftDoorL[index]->GetMovable()->UpdateMove();
				ShaftDoorR[index]->GetMovable()->SetPosition(csVector3(Origin.x, 0, Origin.z));
				ShaftDoorR[index]->GetMovable()->UpdateMove();
			}
		}
		else
		{
			if (elevdoors == true)
			{
				//move elevator doors
				ElevatorDoorL_movable->SetPosition(csVector3(Origin.x, GetPosition().y, Origin.z));
				ElevatorDoorL_movable->UpdateMove();
				ElevatorDoorR_movable->SetPosition(csVector3(Origin.x, GetPosition().y, Origin.z));
				ElevatorDoorR_movable->UpdateMove();
			}

			if (shaftdoors == true && ShaftDoorL[index])
			{
				//move shaft doors
				ShaftDoorL[index]->GetMovable()->SetPosition(csVector3(Origin.x, 0, Origin.z));
				ShaftDoorL[index]->GetMovable()->UpdateMove();
				ShaftDoorR[index]->GetMovable()->SetPosition(csVector3(Origin.x, 0, Origin.z));
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
			if (MovePending == true)
			{
				MoveElevator = true;
				MovePending = false;
			}
		}
	}
	else
	{
		if (open == true)
			ShaftDoorsOpen[ServicedFloors.Find(ShaftDoorFloor)] = true;
		else
			ShaftDoorsOpen[ServicedFloors.Find(ShaftDoorFloor)] = false;
	}

	//reset values
	ElevatorDoorSpeed = 0;
	OpenDoor = 0;
	WhichDoors = 0;
	door_section = 0;
	door_changed = false;
	doors_stopped = false;

	//turn on autoclose timer
	if (manual == false && InServiceMode() == false &&
		(UpPeak == false || GetFloor() != GetBottomFloor()) &&
		(DownPeak == false || GetFloor() != GetTopFloor()))
	{
		if (quick_close == false)
			timer->Start(DoorTimer, true);
		else
			timer->Start(1000, true);
		quick_close = false;
	}

	DoorIsRunning = false;
}

int ElevatorDoor::AddDoors(const char *texture, float thickness, float CenterX, float CenterZ, float width, float height, bool direction, float tw, float th)
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
	DoorOrigin = csVector3(Origin.x + CenterX, Origin.y, Origin.z + CenterZ);

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
	sbs->ReverseExtents(false, false, false);
	int firstidx = sbs->AddWallMain(ElevatorDoorL_state, "Door", texture, thickness, x1, z1, x2, z2, height, height, 0, 0, tw, th);
	sbs->AddWallMain(ElevatorDoorR_state, "Door", texture, thickness, x3, z3, x4, z4, height, height, 0, 0, tw, th);
	//create connection pieces
	sbs->AddWallMain(Elevator_state, "DoorF1", "Connection", thickness, x1, z1, x4, z4, 1, 1, -1.001, -1.001, 0, 0);
	sbs->AddWallMain(Elevator_state, "DoorF2", "Connection", thickness, x1, z1, x4, z4, 1, 1, height + 0.001, height + 0.001, 0, 0);
	sbs->ResetWalls();
	sbs->ResetExtents();
	//relocate sound object
	doorsound->SetPosition(csVector3(DoorOrigin.x, DoorOrigin.y + (DoorHeight / 2), DoorOrigin.z));
	return firstidx;
}

int ElevatorDoor::AddShaftDoors(const char *texture, float thickness, float CenterX, float CenterZ, float tw, float th)
{
	//adds shaft's elevator doors specified at a relative central position (off of elevator origin)
	//uses some parameters (width, height, direction) from AddDoors function
	float x1, x2, x3, x4;
	float z1, z2, z3, z4;
	float base, base2;

	if (tw == 0)
		tw = 1;
	if (th == 0)
		th = 1;

	//set door parameters
	ShaftDoorOrigin = csVector3(Origin.x + CenterX, 0, Origin.z + CenterZ);

	//set up coordinates
	if (DoorDirection == false)
	{
		x1 = CenterX;
		x2 = CenterX;
		x3 = CenterX;
		x4 = CenterX;
		z1 = CenterZ - (DoorWidth / 2);
		z2 = CenterZ;
		z3 = CenterZ;
		z4 = CenterZ + (DoorWidth / 2);
	}
	else
	{
		x1 = CenterX - (DoorWidth / 2);
		x2 = CenterX;
		x3 = CenterX;
		x4 = CenterX + (DoorWidth / 2);
		z1 = CenterZ;
		z2 = CenterZ;
		z3 = CenterZ;
		z4 = CenterZ;
	}

	csString buffer, buffer2, buffer3, buffer4;

	sbs->DrawWalls(true, true, true, true, true, true);
	sbs->ReverseExtents(false, false, false);

	//create doors
	for (size_t i = 0; i < ServicedFloors.GetSize(); i++)
	{
		Floor *floor = sbs->GetFloor(ServicedFloors[i]);
		Shaft *shaft = sbs->GetShaft(AssignedShaft);
		base = floor->InterfloorHeight; //relative to floor
		base2 = floor->Altitude + base; //absolute

		//cut shaft and floor walls
		if (DoorDirection == false)
		{
			shaft->CutWall(false, ServicedFloors[i], csVector3(Origin.x + x1 - 2, base, Origin.z + z1), csVector3(Origin.x + x1 + 2, base + DoorHeight, Origin.z + z4), 1, "Shaft");
			floor->Cut(csVector3(Origin.x + x1 - 2, base, Origin.z + z1), csVector3(Origin.x + x1 + 2, base + DoorHeight, Origin.z + z4), true, false, true, 2, "Shaft");
		}
		else
		{
			shaft->CutWall(false, ServicedFloors[i], csVector3(Origin.x + x1, base, Origin.z + z1 - 2), csVector3(Origin.x + x4, base + DoorHeight, Origin.z + z1 + 2), 1, "Shaft");
			floor->Cut(csVector3(Origin.x + x1, base, Origin.z + z1 - 2), csVector3(Origin.x + x4, base + DoorHeight, Origin.z + z1 + 2), true, false, true, 2, "Shaft");
		}

		//create doorway walls
		sbs->AddDoorwayWalls(floor->Level_state, "ConnectionWall", 0, 0);

		//create meshes
		buffer3 = Number;
		buffer4 = i;
		buffer = "Elevator " + buffer3 + ": Shaft Door " + buffer4 + "L";
		buffer2 = "Elevator " + buffer3 + ": Shaft Door " + buffer4 + "R";
		buffer.Trim();
		buffer2.Trim();
		csRef<iMeshWrapper> tmpmesh;
		csRef<iThingFactoryState> tmpstate;

		//door L
		tmpmesh = sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData());
		ShaftDoorL[i] = tmpmesh;
		tmpstate = scfQueryInterface<iThingFactoryState> (ShaftDoorL[i]->GetMeshObject()->GetFactory());
		ShaftDoorL_state[i] = tmpstate;
		ShaftDoorL[i]->SetZBufMode(CS_ZBUF_USE);
		ShaftDoorL[i]->SetRenderPriority(sbs->engine->GetAlphaRenderPriority());
		ShaftDoorL[i]->GetMeshObject()->SetMixMode(CS_FX_ALPHA);

		//door R
		tmpmesh = sbs->engine->CreateSectorWallsMesh (sbs->area, buffer2.GetData());
		ShaftDoorR[i] = tmpmesh;
		tmpstate = scfQueryInterface<iThingFactoryState> (ShaftDoorR[i]->GetMeshObject()->GetFactory());
		ShaftDoorR_state[i] = tmpstate;
		ShaftDoorR[i]->SetZBufMode(CS_ZBUF_USE);
		ShaftDoorR[i]->SetRenderPriority(sbs->engine->GetAlphaRenderPriority());
		ShaftDoorR[i]->GetMeshObject()->SetMixMode(CS_FX_ALPHA);

		//reposition meshes
		ShaftDoorL[i]->GetMovable()->SetPosition(csVector3(Origin.x, 0, Origin.z));
		ShaftDoorL[i]->GetMovable()->UpdateMove();
		ShaftDoorR[i]->GetMovable()->SetPosition(csVector3(Origin.x, 0, Origin.z));
		ShaftDoorR[i]->GetMovable()->UpdateMove();

		//create doors
		sbs->AddWallMain(ShaftDoorL_state[i], "Door", texture, thickness, x1, z1, x2, z2, DoorHeight, DoorHeight, base2, base2, tw, th);
		sbs->AddWallMain(ShaftDoorR_state[i], "Door", texture, thickness, x3, z3, x4, z4, DoorHeight, DoorHeight, base2, base2, tw, th);

		//create connection pieces
		float xoffset = Origin.x - shaft->origin.x;
		float zoffset = Origin.z - shaft->origin.z;
		shaft->AddWall(ServicedFloors[i], "ShaftDoorF1", "Connection", thickness, xoffset + x1, zoffset + z1, xoffset + x4, zoffset + z4, 1, 1, base - 1.001, base - 1.001, 0, 0);
		shaft->AddWall(ServicedFloors[i], "ShaftDoorF2", "Connection", thickness, xoffset + x1, zoffset + z1, xoffset + x4, zoffset + z4, 1, 1, base + DoorHeight + 0.001, base + DoorHeight + 0.001, 0, 0);

		//make doors invisible on start
		sbs->EnableMesh(ShaftDoorL[i], false);
		sbs->EnableMesh(ShaftDoorR[i], false);

		floor = 0;
		shaft = 0;
	}
	sbs->ResetWalls();
	sbs->ResetExtents();

	//relocate chime sound object
	chime->SetPosition(csVector3(ShaftDoorOrigin.x, ShaftDoorOrigin.y + DoorHeight, ShaftDoorOrigin.z));

	return 0;
}

void ElevatorDoor::ShaftDoorsEnabled(int floor, bool value)
{
	//turns shaft elevator doors on/off

	//exit if shaft's ShowFullShaft is set
	if (sbs->GetShaft(AssignedShaft)->ShowFullShaft == true && value == false)
		return;

	//leave top and bottom on
	if ((floor == sbs->GetShaft(AssignedShaft)->startfloor || floor == sbs->GetShaft(AssignedShaft)->endfloor) && value == false)
		return;

	size_t index = ServicedFloors.Find(floor);
	if (index == csArrayItemNotFound)
		return;

	//exit if the specified floor has no shaft doors
	if (ShaftDoorL[index] == 0)
		return;

	sbs->EnableMesh(ShaftDoorL[index], value);
	sbs->EnableMesh(ShaftDoorR[index], value);
}

void ElevatorDoor::ShaftDoorsEnabledRange(int floor, int range)
{
	//turn on a range of floors
	//if range is 3, show shaft door on current floor (floor), and 1 floor below and above (3 total floors)
	//if range is 1, show door on only the current floor (floor)

	//exit if shaft's ShowFullShaft is set
	if (sbs->GetShaft(AssignedShaft)->ShowFullShaft == true)
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

bool ElevatorDoor::AreDoorsOpen(int number)
{
	//returns the internal door state
	return DoorsOpen;
}

float ElevatorDoor::GetCurrentDoorSpeed(int number)
{
	//returns the internal door speed value
	return ElevatorDoorSpeed;
}

void ElevatorDoor::Timer::Notify()
{
	//door autoclose timer

	//close doors if open
	if (elevator->AreDoorsOpen() == true && elevator->InServiceMode() == false)
		elevator->CloseDoors();
}

void ElevatorDoor::Chime(int floor)
{
	//play chime sound on specified floor
	chime->Load(ChimeSound.GetData());
	chime->Loop(false);
	chime->SetPositionY(sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + DoorHeight);
	chime->Play();
}

void ElevatorDoor::ResetDoorTimer(int number)
{
	//reset elevator door timer
	timer->Stop();
	timer->Start(DoorTimer, true);
}

bool ElevatorDoor::DoorsStopped(int number)
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

void Move(const csVector3 position, bool relative_x, bool relative_y, bool relative_z)
{
	//moves doors
	csVector3 pos;
	if (relative_x == false)
		pos.x = position.x;
	else
		pos.x = ElevatorDoorL_movable->GetPosition().x;
	if (relative_y == false)
		pos.y = position.y;
	else
		pos.y = ElevatorDoorL_movable->GetPosition().y;
	if (relative_z == false)
		pos.z = position.z;
	else
		pos.z = ElevatorDoorL_movable->GetPosition().z;
	ElevatorDoorL_movable->MovePosition(pos);
	ElevatorDoorL_movable->UpdateMove();

	if (relative_x == false)
		pos.x = position.x;
	else
		pos.x = ElevatorDoorR_movable->GetPosition().x;
	if (relative_y == false)
		pos.y = position.y;
	else
		pos.y = ElevatorDoorR_movable->GetPosition().y;
	if (relative_z == false)
		pos.z = position.z;
	else
		pos.z = ElevatorDoorR_movable->GetPosition().z;
	ElevatorDoorR_movable->MovePosition(pos);
	ElevatorDoorR_movable->UpdateMove();
}
