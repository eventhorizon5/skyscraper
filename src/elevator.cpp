/* $Id$ */

/*
	Scalable Building Simulator - Elevator Subsystem Class
	The Skyscraper Project - Version 1.3 Alpha
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
#include "elevator.h"
#include "buttonpanel.h"
#include "sbs.h"
#include "camera.h"
#include "shaft.h"
#include "sound.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

Elevator::Elevator(int number)
{
	csString buffer;

	//set elevator number
	Number = number;

	//init variables
	Name = "";
	QueuePositionDirection = 0;
	LastQueueFloor[0] = 0;
	LastQueueFloor[1] = 0;
	PauseQueueSearch = true;
	ElevatorSpeed = 0;
	MoveElevator = false;
	GotoFloor = 0;
	OpenDoor = 0;
	Acceleration = 0;
	Deceleration = 0;
	AccelJerk = 1;
	DecelJerk = 1;
	OpenSpeed = 0;
	ElevatorStart = 0;
	ElevatorFloor = 0;
	DoorsOpen = false;
	Direction = 0;
	DistanceToTravel = 0;
	Destination = 0;
	ElevatorRate = 0;
	StoppingDistance = 0;
	CalculateStoppingDistance = false;
	Brakes = false;
	ElevatorDoorSpeed = 0;
	ElevWait = false;
	EmergencyStop = false;
	WhichDoors = 0;
	ShaftDoorFloor = 0;
	AssignedShaft = 0;
	IsEnabled = true;
	Height = 0;
	Panel = 0;
	Panel2 = 0;
	DoorAcceleration = 0;
	TempDeceleration = 0;
	ErrorOffset = 0;
	JerkRate = 0;
	JerkPos = 0;
	DoorTimer = 5000;
	DoorIsRunning = false;
	OpenChange = 0;
	marker1 = 0;
	marker2 = 0;
	index = 0;
	stopping_distance = 0;
	temp_change = 0;
	accelerating = false;
	door_error = 0;
	ElevatorIsRunning = false;
	oldfloor = 0;
	IsMoving = false;
	OpenSound = "elevatoropen.wav";
	CloseSound = "elevatorclose.wav";
	StartSound = "elevstart.wav";
	MoveSound = "elevmove.wav";
	StopSound = "elevstop.wav";
	IdleSound = "elevidle.wav";
	ChimeSound = "chime1.wav";

	//create object meshes
	buffer = Number;
	buffer.Insert(0, "Elevator ");
	buffer.Trim();
	ElevatorMesh = sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData());
	Elevator_state = scfQueryInterface<iThingFactoryState> (ElevatorMesh->GetMeshObject()->GetFactory());
	Elevator_movable = ElevatorMesh->GetMovable();
	ElevatorMesh->SetZBufMode(CS_ZBUF_USE);
	ElevatorMesh->SetRenderPriority(sbs->engine->GetAlphaRenderPriority());
	ElevatorMesh->GetMeshObject()->SetMixMode(CS_FX_ALPHA);

	buffer = Number;
	buffer.Insert(0, "FloorIndicator ");
	buffer.Trim();
	FloorIndicator = sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData());
	FloorIndicator_state = scfQueryInterface<iThingFactoryState> (FloorIndicator->GetMeshObject()->GetFactory());
	FloorIndicator_movable = FloorIndicator->GetMovable();
	FloorIndicator->SetZBufMode(CS_ZBUF_USE);
	FloorIndicator->SetRenderPriority(sbs->engine->GetAlphaRenderPriority());
	FloorIndicator->GetMeshObject()->SetMixMode(CS_FX_ALPHA);

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

	buffer = Number;
	buffer.Insert(0, "Plaque ");
	buffer.Trim();
	Plaque = sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData());
	Plaque_state = scfQueryInterface<iThingFactoryState> (Plaque->GetMeshObject()->GetFactory());
	Plaque_movable = Plaque->GetMovable();
	Plaque->SetZBufMode(CS_ZBUF_USE);
	Plaque->SetRenderPriority(sbs->engine->GetAlphaRenderPriority());
	Plaque->GetMeshObject()->SetMixMode(CS_FX_ALPHA);

	//create timer
	timer = new Timer(this);
}

Elevator::~Elevator()
{
	//Destructor
	if (Panel)
		delete Panel;
	Panel = 0;
	if (Panel2)
		delete Panel2;
	Panel2 = 0;
	if (mainsound)
		delete mainsound;
	mainsound = 0;
	if (doorsound)
		delete doorsound;
	doorsound = 0;
	if (chime)
		delete chime;
	chime = 0;
}

void Elevator::CreateElevator(float x, float z, int floor)
{
	//Creates elevator at specified location and floor
	//x and z are the center coordinates

	//set data
	Origin = csVector3(x, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight, z);
	OriginFloor = floor;

	//add elevator to associated shaft's list
	sbs->GetShaft(AssignedShaft)->elevators.InsertSorted(Number);

	//move objects to positions
	Elevator_movable->SetPosition(Origin);
	Elevator_movable->UpdateMove();
	FloorIndicator_movable->SetPosition(Origin);
	FloorIndicator_movable->UpdateMove();
	Plaque_movable->SetPosition(Origin);
	Plaque_movable->UpdateMove();
	ElevatorDoorL_movable->SetPosition(Origin);
	ElevatorDoorL_movable->UpdateMove();
	ElevatorDoorR_movable->SetPosition(Origin);
	ElevatorDoorR_movable->UpdateMove();

	//resize shaft door arrays
	ShaftDoorL.SetSize(ServicedFloors.GetSize());
	ShaftDoorR.SetSize(ServicedFloors.GetSize());
	ShaftDoorL_state.SetSize(ServicedFloors.GetSize());
	ShaftDoorR_state.SetSize(ServicedFloors.GetSize());
	ShaftDoorsOpen.SetSize(ServicedFloors.GetSize());

	//create sound object
	mainsound = new Sound();
	mainsound->SetPosition(Origin);
	idlesound = new Sound();
	idlesound->SetPosition(Origin);
	idlesound->Load(IdleSound.GetData());
	doorsound = new Sound();
	doorsound->SetPosition(Origin);
	chime = new Sound();

	sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": created at " + csString(_gcvt(x, 12, buffer)) + ", " + csString(_gcvt(z, 12, buffer)) + ", " + csString(_itoa(floor, buffer, 12)));
}

void Elevator::AddRoute(int floor, int direction)
{
	//Add call route to elevator routing table, in sorted order

	if (direction == 1)
	{
		if (UpQueue.GetSize() == 0 && QueuePositionDirection == 0)
			PauseQueueSearch = false;
		UpQueue.InsertSorted(floor);
		LastQueueFloor[0] = floor;
		LastQueueFloor[1] = 1;
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": adding route to floor " + csString(_itoa(floor, intbuffer, 10)) + " direction up");
	}
	else
	{
		if (DownQueue.GetSize() == 0 && QueuePositionDirection == 0)
			PauseQueueSearch = false;
		DownQueue.InsertSorted(floor);
		LastQueueFloor[0] = floor;
		LastQueueFloor[1] = -1;
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": adding route to floor " + csString(_itoa(floor, intbuffer, 10)) + " direction down");
	}

}

void Elevator::DeleteRoute(int floor, int direction)
{
	//Delete call route from elevator routing table

	if (direction == 1)
	{
		UpQueue.Delete(floor);
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": deleting route to floor " + csString(_itoa(floor, intbuffer, 10)) + " direction up");
	}
	else
	{
		DownQueue.Delete(floor);
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": deleting route to floor " + csString(_itoa(floor, intbuffer, 10)) + " direction down");
	}
}

void Elevator::CancelLastRoute()
{
	//cancels the last added route

	if (LastQueueFloor[1] == 1)
	{
		DeleteRoute(LastQueueFloor[0], 1);
		LastQueueFloor[0] = 0;
		LastQueueFloor[1] = 0;
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": cancelling last route");
	}
	else if (LastQueueFloor[1] == -1)
	{
		DeleteRoute(LastQueueFloor[0], -1);
		LastQueueFloor[0] = 0;
		LastQueueFloor[1] = 0;
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": cancelling last route");
	}
}

void Elevator::Alarm()
{
	//elevator alarm code

	sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": alarm on");
}

void Elevator::StopElevator()
{
	//Tells elevator to stop moving, no matter where it is

	EmergencyStop = true;
	sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": emergency stop");
}

void Elevator::OpenHatch()
{
	//Opens the elevator's upper escape hatch, allowing access to the shaft

	sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": opening hatch");
}

void Elevator::ProcessCallQueue()
{
	//Processes the elevator's call queue, and sends elevators to called floors

	//set queue search direction if queues aren't empty
	if (QueuePositionDirection == 0)
	{
		if (UpQueue.GetSize() != 0)
			QueuePositionDirection = 1;
		if (DownQueue.GetSize() != 0)
			QueuePositionDirection = -1;
	}

	//pause queue search if both queues are empty
	if (UpQueue.GetSize() == 0 && DownQueue.GetSize() == 0)
	{
		QueuePositionDirection = 0;
		PauseQueueSearch = true;
		return;
	}

	//set search direction to 0 if any related queue is empty
	if (QueuePositionDirection == 1 && UpQueue.GetSize() == 0)
		QueuePositionDirection = 0;
	if (QueuePositionDirection == -1 && DownQueue.GetSize() == 0)
		QueuePositionDirection = 0;

	//if elevator is moving, keep queue paused
	if (MoveElevator == true)
		PauseQueueSearch = true;

	//if elevator is stopped, pause queue
	if (QueuePositionDirection != 0 && MoveElevator == false)
		PauseQueueSearch = false;

	if (PauseQueueSearch == true)
		return;

	//Search through queue lists and find next valid floor call
	if (QueuePositionDirection == 1)
	{
		//search through up queue
		for (size_t i = 0; i < UpQueue.GetSize(); i++)
		{
			if (UpQueue[i] > GetFloor() || (UpQueue[i] < GetFloor() && UpQueue.GetSize() == 1))
			{
				PauseQueueSearch = true;
				CloseDoors();
				GotoFloor = UpQueue[i];
				MoveElevator = true;
				DeleteRoute(UpQueue[i], 1);
				return;
			}
		}
	}
	else if (QueuePositionDirection == -1)
	{
		//search through down queue
		for (size_t i = 0; i < DownQueue.GetSize(); i++)
		{
			if (DownQueue[i] < GetFloor() || (DownQueue[i] > GetFloor() && DownQueue.GetSize() == 1))
			{
				PauseQueueSearch = true;
				CloseDoors();
				GotoFloor = DownQueue[i];
				MoveElevator = true;
				DeleteRoute(DownQueue[i], -1);
				return;
			}
		}
	}
}

int Elevator::GetFloor()
{
	//Determine floor that the elevator is on

	return sbs->GetFloorNumber(GetPosition().y);
}

void Elevator::MonitorLoop()
{
	//Monitors elevator and starts actions if needed

	//make sure height value is set
	if (Height == 0)
	{
		for (int i = 0; i < Elevator_state->GetVertexCount(); i++)
		{
			if (Elevator_state->GetVertex(i).y > Height)
				Height = Elevator_state->GetVertex(i).y;
		}
	}

	//play idle sound if in elevator, or if doors open
	if (((sbs->InElevator == true && sbs->ElevatorNumber == Number) || DoorsOpen == true) && idlesound->IsPlaying() == false)
	{
		idlesound->Loop(true);
		idlesound->Play();
	}
	else if (((sbs->InElevator == false || sbs->ElevatorNumber != Number) && DoorsOpen == false) && idlesound->IsPlaying() == true)
		idlesound->Stop();

	//call queue processor
	ProcessCallQueue();

	//door operations
	if (OpenDoor == 1)
		MoveDoors(true, false);
	if (OpenDoor == -1)
		MoveDoors(false, false);
	if (OpenDoor == 2)
		MoveDoors(true, true);
	if (OpenDoor == -2)
		MoveDoors(false, true);

	//elevator movement
	if (MoveElevator == true)
		MoveElevatorToFloor();

}

void Elevator::OpenDoorsEmergency(int whichdoors, int floor)
{
	//Simulates manually prying doors open.
	//Slowly opens the elevator doors no matter where elevator is.
	//If lined up with shaft doors, then opens the shaft doors also

	//WhichDoors is the doors to move:
	//1 = both shaft and elevator doors
	//2 = only elevator doors
	//3 = only shaft doors

	if (OpenDoor != 0)
	{
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": doors in use");
		return;
	}

	//check if elevator doors are already open
	if (DoorsOpen == true && whichdoors != 3)
		return;

	//check if shaft doors are already open
	if (whichdoors == 3)
	{
		if (ShaftDoorsOpen[ServicedFloors.Find(floor)] == true && whichdoors == 3)
			return;
		else
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": manually opening shaft doors on floor " + csString(_itoa(floor, intbuffer, 10)));
	}
	else
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": manually opening doors");

	OpenDoor = 2;
	WhichDoors = whichdoors;
	ShaftDoorFloor = floor;
	MoveDoors(true, true);
}

void Elevator::CloseDoorsEmergency(int whichdoors, int floor)
{
	//Simulates manually closing doors.
	//Slowly closes the elevator doors no matter where elevator is.
	//If lined up with shaft doors, then closes the shaft doors also

	//WhichDoors is the doors to move:
	//1 = both shaft and elevator doors
	//2 = only elevator doors
	//3 = only shaft doors

	if (OpenDoor != 0)
	{
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": doors in use");
		return;
	}

	//check if elevator doors are already closed
	if (DoorsOpen == false && whichdoors != 3)
		return;

	//check if shaft doors are already closed
	if (whichdoors == 3)
	{
		if (ShaftDoorsOpen[ServicedFloors.Find(floor)] == false && whichdoors == 3)
			return;
		else
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": manually closing shaft doors on floor " + csString(_itoa(floor, intbuffer, 10)));
	}
	else
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": manually closing doors");

	OpenDoor = -2;
	WhichDoors = whichdoors;
	ShaftDoorFloor = floor;
	MoveDoors(false, true);
}

void Elevator::OpenDoors(int whichdoors, int floor)
{
	//Opens elevator doors

	//WhichDoors is the doors to move:
	//1 = both shaft and elevator doors
	//2 = only elevator doors
	//3 = only shaft doors

	if (OpenDoor != 0 && whichdoors != 3)
	{
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": doors in use");
		return;
	}

	//don't open doors if emergency stop is enabled
	if (EmergencyStop == true && whichdoors != 3)
	{
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": not on floor");
		return;
	}

	//check if elevator doors are already open
	if (DoorsOpen == true && whichdoors != 3)
	{
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": doors already open; resetting timer");
		//reset timer
		timer->Stop();
		timer->Start(DoorTimer, true);
		return;
	}

	//check if shaft doors are already open
	if (whichdoors == 3)
	{
		if (ShaftDoorsOpen[ServicedFloors.Find(floor)] == true)
			return;
		else
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": opening shaft doors on floor " + csString(_itoa(floor, intbuffer, 10)));
	}
	else
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": opening doors");

	OpenDoor = 1;
	WhichDoors = whichdoors;
	ShaftDoorFloor = floor;
	MoveDoors(true, false);
}

void Elevator::CloseDoors(int whichdoors, int floor)
{
	//Closes elevator doors

	//WhichDoors is the doors to move:
	//1 = both shaft and elevator doors
	//2 = only elevator doors
	//3 = only shaft doors

	if (OpenDoor != 0)
	{
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": doors in use");
		return;
	}

	//check if elevator doors are already closed
	if (DoorsOpen == false && whichdoors != 3)
	{
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": doors already closed");
		return;
	}

	//check if shaft doors are already closed
	if (whichdoors == 3)
	{
		if (ShaftDoorsOpen[ServicedFloors.Find(floor)] == false && whichdoors == 3)
			return;
		else
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": closing shaft doors on floor " + csString(_itoa(floor, intbuffer, 10)));
	}
	else
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": closing doors");

	OpenDoor = -1;
	WhichDoors = whichdoors;
	ShaftDoorFloor = floor;
	MoveDoors(false, false);
}

void Elevator::MoveDoors(bool open, bool emergency)
{
	//opens or closes elevator doors
	//currently only supports doors on either the left/right or front/back
	//diagonal doors will be done later, by possibly using relative plane movement

	//WhichDoors is the doors to move:
	//1 = both shaft and elevator doors
	//2 = only elevator doors
	//3 = only shaft doors

	//ShaftDoorFloor is the floor the shaft doors are on - only has effect if whichdoors is 3

	//todo: turn off autoclose timer

	if (DoorIsRunning == false)
	{
		//initialization code

		DoorIsRunning = true;

		if (emergency == false)
		{
			OpenChange = OpenSpeed / 50;
			marker1 = DoorWidth / 8;
			marker2 = (DoorWidth / 2) - (DoorWidth / 8);
		}
		else
		{
			OpenChange = 0.003f;
			marker1 = DoorWidth / 16;
			marker2 = (DoorWidth / 2) - (DoorWidth / 16);
		}

		if (emergency == false)
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

		ElevatorDoorSpeed = 0;
		if (WhichDoors == 3)
			index = ServicedFloors.Find(ShaftDoorFloor);
		else
			index = ServicedFloors.Find(GetFloor());
	}

	//get reference movable object
	csRef<iMovable> tmpMovable;
	if (WhichDoors < 3)
		tmpMovable = ElevatorDoorL_movable;
	else
		tmpMovable = ShaftDoorL[index]->GetMovable();

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

	//Speed up doors
	if (DoorDirection == false)
	{
		if ((DoorOrigin.z - tmpMovable->GetPosition().z <= marker1 && open == true) || (DoorOrigin.z - tmpMovable->GetPosition().z > marker2 && open == false))
		{
			accelerating = true;
			if (open == true)
				ElevatorDoorSpeed += OpenChange;
			else
				ElevatorDoorSpeed -= OpenChange;

			if (elevdoors == true)
			{
				//move elevator doors
				ElevatorDoorL_movable->MovePosition(csVector3(0, 0, -ElevatorDoorSpeed * sbs->delta));
				ElevatorDoorL_movable->UpdateMove();
				ElevatorDoorR_movable->MovePosition(csVector3(0, 0, ElevatorDoorSpeed * sbs->delta));
				ElevatorDoorR_movable->UpdateMove();
			}
			if (shaftdoors == true)
			{
				//move shaft doors
				ShaftDoorL[index]->GetMovable()->MovePosition(csVector3(0, 0, -ElevatorDoorSpeed * sbs->delta));
				ShaftDoorL[index]->GetMovable()->UpdateMove();
				ShaftDoorR[index]->GetMovable()->MovePosition(csVector3(0, 0, ElevatorDoorSpeed * sbs->delta));
				ShaftDoorR[index]->GetMovable()->UpdateMove();
			}

			//get stopping distance
			stopping_distance = DoorOrigin.z - tmpMovable->GetPosition().z;

			return;
		}
	}
	else
	{
		if ((DoorOrigin.x - tmpMovable->GetPosition().x <= marker1 && open == true) || (DoorOrigin.x - tmpMovable->GetPosition().x > marker2 && open == false))
		{
			accelerating = true;
			if (open == true)
				ElevatorDoorSpeed += OpenChange;
			else
				ElevatorDoorSpeed -= OpenChange;

			if (elevdoors == true)
			{
				//move elevator doors
				ElevatorDoorL_movable->MovePosition(csVector3(-ElevatorDoorSpeed * sbs->delta, 0, 0));
				ElevatorDoorL_movable->UpdateMove();
				ElevatorDoorR_movable->MovePosition(csVector3(ElevatorDoorSpeed * sbs->delta, 0, 0));
				ElevatorDoorR_movable->UpdateMove();
			}

			if (shaftdoors == true)
			{
				//move shaft doors
				ShaftDoorL[index]->GetMovable()->MovePosition(csVector3(-ElevatorDoorSpeed * sbs->delta, 0, 0));
				ShaftDoorL[index]->GetMovable()->UpdateMove();
				ShaftDoorR[index]->GetMovable()->MovePosition(csVector3(ElevatorDoorSpeed * sbs->delta, 0, 0));
				ShaftDoorR[index]->GetMovable()->UpdateMove();
			}

			//get stopping distance
			stopping_distance = DoorOrigin.x - tmpMovable->GetPosition().x;

			return;
		}
	}

	//Normal door movement
	if (DoorDirection == false)
	{
		if ((DoorOrigin.z - tmpMovable->GetPosition().z <= marker2 && open == true) || (DoorOrigin.z - tmpMovable->GetPosition().z > marker1 && open == false))
		{
			if (elevdoors == true)
			{
				//move elevator doors
				ElevatorDoorL_movable->MovePosition(csVector3(0, 0, -ElevatorDoorSpeed * sbs->delta));
				ElevatorDoorL_movable->UpdateMove();
				ElevatorDoorR_movable->MovePosition(csVector3(0, 0, ElevatorDoorSpeed * sbs->delta));
				ElevatorDoorR_movable->UpdateMove();
			}

			if (shaftdoors == true)
			{
				//move shaft doors
				ShaftDoorL[index]->GetMovable()->MovePosition(csVector3(0, 0, -ElevatorDoorSpeed * sbs->delta));
				ShaftDoorL[index]->GetMovable()->UpdateMove();
				ShaftDoorR[index]->GetMovable()->MovePosition(csVector3(0, 0, ElevatorDoorSpeed * sbs->delta));
				ShaftDoorR[index]->GetMovable()->UpdateMove();
			}

			return;
		}
	}
	else
	{
		if ((DoorOrigin.x - tmpMovable->GetPosition().x <= marker2 && open == true) || (DoorOrigin.x - tmpMovable->GetPosition().x > marker1 && open == false))
		{
			if (elevdoors == true)
			{
				//move elevator doors
				ElevatorDoorL_movable->MovePosition(csVector3(-ElevatorDoorSpeed * sbs->delta, 0, 0));
				ElevatorDoorL_movable->UpdateMove();
				ElevatorDoorR_movable->MovePosition(csVector3(ElevatorDoorSpeed * sbs->delta, 0, 0));
				ElevatorDoorR_movable->UpdateMove();
			}

			if (shaftdoors == true)
			{
				//move shaft doors
				ShaftDoorL[index]->GetMovable()->MovePosition(csVector3(-ElevatorDoorSpeed * sbs->delta, 0, 0));
				ShaftDoorL[index]->GetMovable()->UpdateMove();
				ShaftDoorR[index]->GetMovable()->MovePosition(csVector3(ElevatorDoorSpeed * sbs->delta, 0, 0));
				ShaftDoorR[index]->GetMovable()->UpdateMove();
			}

			return;
		}
	}

	if (accelerating == true)
	{
		accelerating = false;
		if (DoorDirection == false)
			temp_change = OpenChange * (stopping_distance / (tmpMovable->GetPosition().z - (DoorOrigin.z - (DoorWidth / 2))));
		else
			temp_change = OpenChange * (stopping_distance / (tmpMovable->GetPosition().x - (DoorOrigin.x - (DoorWidth / 2))));
	}

	//slow down doors
	if ((ElevatorDoorSpeed > 0 && open == true) || (ElevatorDoorSpeed < 0 && open == false))
	{
		if (open == true)
			ElevatorDoorSpeed -= temp_change;
		else
			ElevatorDoorSpeed += temp_change;
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

			if (shaftdoors == true)
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

			if (shaftdoors == true)
			{
				//move shaft doors
				ShaftDoorL[index]->GetMovable()->MovePosition(csVector3(-ElevatorDoorSpeed * sbs->delta, 0, 0));
				ShaftDoorL[index]->GetMovable()->UpdateMove();
				ShaftDoorR[index]->GetMovable()->MovePosition(csVector3(ElevatorDoorSpeed * sbs->delta, 0, 0));
				ShaftDoorR[index]->GetMovable()->UpdateMove();
			}
		}
		return;
	}

	//get error value
	if (open == true && DoorDirection == false)
		door_error = tmpMovable->GetPosition().z - (DoorOrigin.z - (DoorWidth / 2));
	if (open == true && DoorDirection == true)
		door_error = tmpMovable->GetPosition().x - (DoorOrigin.x - (DoorWidth / 2));
	if (open == false && DoorDirection == false)
		door_error = DoorOrigin.z - tmpMovable->GetPosition().z;
	if (open == false && DoorDirection == true)
		door_error = DoorOrigin.x - tmpMovable->GetPosition().x;

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

			if (shaftdoors == true)
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

			if (shaftdoors == true)
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

			if (shaftdoors == true)
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

			if (shaftdoors == true)
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
			DoorsOpen = false;
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

	//turn on autoclose timer
	if (emergency == false)
		timer->Start(DoorTimer, true);

	DoorIsRunning = false;
}

void Elevator::MoveElevatorToFloor()
{
	//Main processing routine; sends elevator to floor specified in GotoFloor

	//exit if doors are moving
	if (OpenDoor != 0)
		return;

	if (ElevatorIsRunning == false)
	{
		ElevatorIsRunning = true;
		csString dir_string;

		//get elevator's current altitude
		ElevatorStart = GetPosition().y;

		//get elevator's current floor
		ElevatorFloor = GetFloor();
		oldfloor = ElevatorFloor;

		//If elevator is already on specified floor, open doors and exit
		if (ElevatorFloor == GotoFloor)
		{
			sbs->Report("Elevator already on specified floor");
			MoveElevator = false;
			ElevatorIsRunning = false;
			OpenDoors();
			return;
		}

		//if destination floor is not in the ServicedFloors array, reset and exit
		if (ServicedFloors.Find(GotoFloor) == csArrayItemNotFound)
		{
			sbs->Report("Destination floor not in ServicedFloors list");
			MoveElevator = false;
			ElevatorIsRunning = false;
			return;
		}

		//close doors if open
		if (DoorsOpen == true)
			CloseDoors();

		//Determine direction
		if (GotoFloor < ElevatorFloor)
		{
			Direction = -1;
			dir_string = "down";
		}
		if (GotoFloor > ElevatorFloor)
		{
			Direction = 1;
			dir_string = "up";
		}

		//Determine distance to destination floor
		Destination = sbs->GetFloor(GotoFloor)->Altitude + sbs->GetFloor(GotoFloor)->InterfloorHeight;
		DistanceToTravel = fabs(fabs(Destination) - fabs(ElevatorStart));
		CalculateStoppingDistance = true;

		//If user is riding this elevator, then turn off objects
		if (sbs->ElevatorSync == true && sbs->ElevatorNumber == Number)
		{
			//turn off floor
			if (sbs->GetShaft(AssignedShaft)->ShowFloors == false)
			{
				sbs->GetFloor(sbs->camera->CurrentFloor)->Enabled(false);
				sbs->GetFloor(sbs->camera->CurrentFloor)->EnableGroup(false);
			}
			else if (sbs->GetShaft(AssignedShaft)->ShowFloorsList.Find(sbs->camera->CurrentFloor) == -1)
			{
				sbs->GetFloor(sbs->camera->CurrentFloor)->Enabled(false);
				sbs->GetFloor(sbs->camera->CurrentFloor)->EnableGroup(false);
			}

			//Turn off sky, buildings, and landscape
			if (sbs->GetShaft(AssignedShaft)->ShowOutside == false)
			{
				sbs->EnableSkybox(false);
				sbs->EnableBuildings(false);
				sbs->EnableLandscape(false);
				sbs->EnableExternal(false);
			}
			else if (sbs->GetShaft(AssignedShaft)->ShowOutsideList.Find(sbs->camera->CurrentFloor) == -1)
			{
				sbs->EnableSkybox(false);
				sbs->EnableBuildings(false);
				sbs->EnableLandscape(false);
				sbs->EnableExternal(false);
			}
		}

		//Play starting sound
		mainsound->Stop();
		mainsound->Load(StartSound.GetData());
		mainsound->Loop(false);
		mainsound->Play();

		//notify about movement
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": moving " + dir_string + " to floor " + csString(_itoa(GotoFloor, intbuffer, 10)));
		IsMoving = true;
	}

	if (EmergencyStop == true && Brakes == false)
	{
		CalculateStoppingDistance = false;
		TempDeceleration = Deceleration;
		if (Direction == 1)
			Direction = -1;
		else
			Direction = 1;
		Brakes = true;
	}

	if (mainsound->IsPlaying() == false && Brakes == false)
	{
		//Movement sound
		mainsound->Load(MoveSound.GetData());
		mainsound->Loop(true);
		mainsound->Play();
	}

	//move elevator objects and camera
	Elevator_movable->MovePosition(csVector3(0, ElevatorRate * sbs->delta, 0));
	Elevator_movable->UpdateMove();
	if (sbs->ElevatorSync == true && sbs->ElevatorNumber == Number)
		sbs->camera->SetPosition(csVector3(sbs->camera->GetPosition().x, GetPosition().y + sbs->camera->cfg_legs_height + sbs->camera->cfg_body_height, sbs->camera->GetPosition().z));
	ElevatorDoorL_movable->MovePosition(csVector3(0, ElevatorRate * sbs->delta, 0));
	ElevatorDoorL_movable->UpdateMove();
	ElevatorDoorR_movable->MovePosition(csVector3(0, ElevatorRate * sbs->delta, 0));
	ElevatorDoorR_movable->UpdateMove();
	FloorIndicator_movable->MovePosition(csVector3(0, ElevatorRate * sbs->delta, 0));
	FloorIndicator_movable->UpdateMove();
	Plaque_movable->MovePosition(csVector3(0, ElevatorRate * sbs->delta, 0));
	Plaque_movable->UpdateMove();
	if (Panel)
		Panel->Move(csVector3(0, ElevatorRate * sbs->delta, 0));
	if (Panel2)
		Panel2->Move(csVector3(0, ElevatorRate * sbs->delta, 0));

	//move sounds
	mainsound->SetPosition(GetPosition());
	idlesound->SetPosition(GetPosition());
	doorsound->SetPosition(csVector3(DoorOrigin.x, GetPosition().y + (DoorHeight / 2), DoorOrigin.z));

	//motion calculation
	if (Brakes == false)
	{
		//calculate jerk rate
		if (JerkRate < 1)
		{
			JerkRate += AccelJerk * sbs->delta;
			JerkPos = ElevatorRate;
		}
		if (JerkRate > 1)
			JerkRate = 1;

		//regular motion
		if (Direction == 1)
			ElevatorRate += ElevatorSpeed * ((Acceleration * JerkRate) * sbs->delta);
		if (Direction == -1)
			ElevatorRate -= ElevatorSpeed * ((Acceleration * JerkRate) * sbs->delta);
	}
	else
	{
		//slow down

		//calculate jerk rate
		//check if the elevator rate is less than the amount that was stored in JerkPos
		//(the elevator rate at the end of the JerkRate increments), adjusted to the ratio of acceljerk to deceljerk
		if (Direction == -1)
		{
			if (ElevatorRate <= (JerkPos * (AccelJerk / DecelJerk)))
				JerkRate -= DecelJerk * sbs->delta;
		}
		else
		{
			if (ElevatorRate >= (JerkPos * (AccelJerk / DecelJerk)))
				JerkRate -= DecelJerk * sbs->delta;
		}
		//prevent jerkrate from reaching 0
		if (JerkRate < 0.01)
			JerkRate = 0.01;

		if (Direction == 1)
			ElevatorRate += ElevatorSpeed * ((TempDeceleration * JerkRate) * sbs->delta);
		if (Direction == -1)
			ElevatorRate -= ElevatorSpeed * ((TempDeceleration * JerkRate) * sbs->delta);
	}

	//change speeds
	if ((Direction == 1) && (ElevatorRate > ElevatorSpeed))
	{
		ElevatorRate = ElevatorSpeed;
		CalculateStoppingDistance = false;
	}
	if ((Direction == -1) && (ElevatorRate < -ElevatorSpeed))
	{
		ElevatorRate = -ElevatorSpeed;
		CalculateStoppingDistance = false;
	}
	//prevent the rate from going beyond 0
	if ((Direction == 1) && (Brakes == true) && (ElevatorRate > 0))
		ElevatorRate = 0;
	if ((Direction == -1) && (Brakes == true) && (ElevatorRate < 0))
		ElevatorRate = 0;

	//get distance needed to stop elevator
	if (CalculateStoppingDistance == true)
	{
		if (Direction == 1)
			//stopping distance is the distance the elevator has travelled (usually to reach max speed), times
			//the ratio of acceleration to deceleration (so if the deceleration is half of the acceleration,
			//it will take twice the distance to stop)
			StoppingDistance = (GetPosition().y - ElevatorStart) * (Acceleration / Deceleration);
		else if (Direction == -1)
			StoppingDistance = (ElevatorStart - GetPosition().y) * (Acceleration / Deceleration);
	}

	//Deceleration routines with floor overrun correction (there's still problems, but it works pretty good)
	//since this function cycles at a slower/less granular rate (cycles according to frames per sec), an error factor is present where the elevator overruns the dest floor,
	//even though the algorithms are all correct. Since the elevator moves by "jumping" to a new altitude every frame - and usually jumps right over the altitude value where it is supposed to
	//start the deceleration process, causing the elevator to decelerate too late, and end up passing/overrunning the dest floor's altitude.  This code corrects this problem
	//by determining if the next "jump" will overrun the deceleration marker (which is Dest's Altitude minus Stopping Distance), and temporarily altering the deceleration rate according to how far off the mark it is
	//and then starting the deceleration process immediately.

	//up movement
	if ((Brakes == false) && (Direction == 1))
	{
		//determine if next jump altitude is over deceleration marker
		if (((GetPosition().y + (ElevatorRate * sbs->delta)) > (Destination - StoppingDistance)))
		{
			CalculateStoppingDistance = false;
			//recalculate deceleration value based on distance from marker, and store result in tempdeceleration
			TempDeceleration = Deceleration * (StoppingDistance / (Destination - GetPosition().y));
			//start deceleration
			Direction = -1;
			Brakes = true;
			ElevatorRate -= ElevatorSpeed * ((TempDeceleration * JerkRate) * sbs->delta);
			//stop sounds
			mainsound->Stop();
			//play elevator stopping sound
			mainsound->Load(StopSound.GetData());
			mainsound->Loop(false);
			mainsound->Play();
		}
	}

	//down movement
	if (Brakes == false && Direction == -1)
	{
	//determine if next jump altitude is below deceleration marker
		if (((GetPosition().y - (ElevatorRate * sbs->delta)) < (Destination + StoppingDistance)))
		{
			CalculateStoppingDistance = false;
			//recalculate deceleration value based on distance from marker, and store result in tempdeceleration
			TempDeceleration = Deceleration * (StoppingDistance / (GetPosition().y - Destination));
			//start deceleration
			Direction = 1;
			Brakes = true;
			ElevatorRate += ElevatorSpeed * ((TempDeceleration * JerkRate) * sbs->delta);
			//stop sounds
			mainsound->Stop();
			//play stopping sound
			mainsound->Load(StopSound.GetData());
			mainsound->Loop(false);
			mainsound->Play();
		}
	}

	//update floor indicators
	if (GetFloor() != oldfloor && sbs->ElevatorSync == true && sbs->ElevatorNumber == Number)
		UpdateFloorIndicators();

	oldfloor = GetFloor();

	//exit if elevator's running
	if (fabs(ElevatorRate) != 0)
		return;

	//finish move
	if (EmergencyStop == false)
	{
		//play chime sound
		Chime(GotoFloor);

		//store error offset value
		if (Direction == -1)
			ErrorOffset = GetPosition().y - Destination;
		else if (Direction == 1)
			ErrorOffset = Destination - GetPosition().y;
		else
			ErrorOffset = 0;

		//set elevator and objects to floor altitude (corrects offset errors)
		//move elevator objects and camera
		Elevator_movable->SetPosition(csVector3(GetPosition().x, Destination, GetPosition().z));
		Elevator_movable->UpdateMove();
		if (sbs->ElevatorSync == true && sbs->ElevatorNumber == Number)
			sbs->camera->SetPosition(csVector3(sbs->camera->GetPosition().x, GetPosition().y + sbs->camera->cfg_legs_height + sbs->camera->cfg_body_height, sbs->camera->GetPosition().z));
		ElevatorDoorL_movable->SetPosition(csVector3(ElevatorDoorL_movable->GetPosition().x, Destination, ElevatorDoorL_movable->GetPosition().z));
		ElevatorDoorL_movable->UpdateMove();
		ElevatorDoorR_movable->SetPosition(csVector3(ElevatorDoorR_movable->GetPosition().x, Destination, ElevatorDoorR_movable->GetPosition().z));
		ElevatorDoorR_movable->UpdateMove();
		FloorIndicator_movable->SetPosition(csVector3(FloorIndicator_movable->GetPosition().x, Destination, FloorIndicator_movable->GetPosition().z));
		FloorIndicator_movable->UpdateMove();
		Plaque_movable->SetPosition(csVector3(Plaque_movable->GetPosition().x, Destination, Plaque_movable->GetPosition().z));
		Plaque_movable->UpdateMove();
		if (Panel)
			Panel->SetToElevatorAltitude();
		if (Panel2)
			Panel2->SetToElevatorAltitude();

		//move sounds
		mainsound->SetPosition(GetPosition());
	}

	//reset values if at destination floor
	ElevatorRate = 0;
	JerkRate = 0;
	Direction = 0;
	Brakes = false;
	Destination = 0;
	DistanceToTravel = 0;
	ElevatorStart = 0;
	ElevatorIsRunning = false;
	MoveElevator = false;
	IsMoving = false;
	mainsound->Stop();

	if (EmergencyStop == false)
	{
		//update elevator's floor number
		GetFloor();

		//Turn on objects if user is in elevator
		if (sbs->ElevatorSync == true && sbs->ElevatorNumber == Number)
		{
			UpdateFloorIndicators();

			//turn on floor
			sbs->GetFloor(GotoFloor)->Enabled(true);
			sbs->GetFloor(GotoFloor)->EnableGroup(true);

			//Turn on sky, buildings, and landscape
			sbs->EnableSkybox(true);
			sbs->EnableBuildings(true);
			sbs->EnableLandscape(true);
			sbs->EnableExternal(true);

			//reset shaft doors
			for (int i = 1; i <= sbs->Shafts(); i++)
			{
				sbs->GetShaft(i)->EnableRange(GotoFloor, sbs->ShaftDisplayRange, false, true);
				sbs->GetShaft(i)->EnableRange(GotoFloor, sbs->ShaftDisplayRange, true, true);
			}
		}

		//open doors
		OpenDoors();

	}
	else
	{
		EmergencyStop = false;
		if (sbs->ElevatorSync == true && sbs->ElevatorNumber == Number)
		{
			//reset shaft doors
			for (int i = 1; i <= sbs->Shafts(); i++)
			{
				sbs->GetShaft(i)->EnableRange(GotoFloor, sbs->ShaftDisplayRange, false, true);
				sbs->GetShaft(i)->EnableRange(GotoFloor, sbs->ShaftDisplayRange, true, true);
			}
		}
	}
}

int Elevator::AddWall(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th)
{
	//Adds a wall with the specified dimensions
	float tw2 = tw;
	float th2;
	float tempw1;
	float tempw2;

	//Set horizontal scaling
	x1 = x1 * sbs->HorizScale;
	x2 = x2 * sbs->HorizScale;
	z1 = z1 * sbs->HorizScale;
	z2 = z2 * sbs->HorizScale;

	//Call texture autosizing formulas
	if (z1 == z2)
		tw2 = sbs->AutoSize(x1, x2, true, tw);
	if (x1 == x2)
		tw2 = sbs->AutoSize(z1, z2, true, tw);
	if ((z1 != z2) && (x1 != x2))
	{
		//calculate diagonals
		if (x1 > x2)
			tempw1 = x1 - x2;
		else
			tempw1 = x2 - x1;
		if (z1 > z2)
			tempw2 = z1 - z2;
		else
			tempw2 = z2 - z1;
		tw2 = sbs->AutoSize(0, sqrt(pow(tempw1, 2) + pow(tempw2, 2)), true, tw);
	}
	th2 = sbs->AutoSize(0, height1, false, th);

	return sbs->AddWallMain(Elevator_state, name, texture, thickness, x1, z1, x2, z2, height1, height2, voffset1, voffset2, tw2, th2);
}

int Elevator::AddFloor(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, float tw, float th)
{
	//Adds a floor with the specified dimensions and vertical offset
	float tw2;
	float th2;

	//Set horizontal scaling
	x1 = x1 * sbs->HorizScale;
	x2 = x2 * sbs->HorizScale;
	z1 = z1 * sbs->HorizScale;
	z2 = z2 * sbs->HorizScale;

	//Call texture autosizing formulas
	tw2 = sbs->AutoSize(x1, x2, true, tw);
	th2 = sbs->AutoSize(z1, z2, false, th);

	return sbs->AddFloorMain(Elevator_state, name, texture, thickness, x1, z1, x2, z2, voffset1, voffset2, tw2, th2);
}

int Elevator::AddFloorIndicator(const char *direction, float CenterX, float CenterZ, float width, float height, float voffset)
{
	//Creates a floor indicator at the specified location
	int index = -1;
	csString texture = "Button" + sbs->GetFloor(OriginFloor)->ID;
	csString tmpdirection = direction;
	tmpdirection.Downcase();

	sbs->ReverseExtents(false, false, false);
	if (tmpdirection == "front")
	{
		sbs->DrawWalls(true, false, false, false, false, false);
		index = sbs->AddWallMain(FloorIndicator_state, "Floor Indicator", texture.GetData(), 0, CenterX - (width / 2), CenterZ, CenterX + (width / 2), CenterZ, height, height, voffset, voffset, 1, 1);
	}
	if (tmpdirection == "back")
	{
		sbs->DrawWalls(false, true, false, false, false, false);
		index = sbs->AddWallMain(FloorIndicator_state, "Floor Indicator", texture.GetData(), 0, CenterX - (width / 2), CenterZ, CenterX + (width / 2), CenterZ, height, height, voffset, voffset, 1, 1);
	}
	if (tmpdirection == "left")
	{
		sbs->DrawWalls(true, false, false, false, false, false);
		index = sbs->AddWallMain(FloorIndicator_state, "Floor Indicator", texture.GetData(), 0, CenterX, CenterZ - (width / 2), CenterX, CenterZ + (width / 2), height, height, voffset, voffset, 1, 1);
	}
	if (tmpdirection == "right")
	{
		sbs->DrawWalls(false, true, false, false, false, false);
		index = sbs->AddWallMain(FloorIndicator_state, "Floor Indicator", texture.GetData(), 0, CenterX, CenterZ - (width / 2), CenterX, CenterZ + (width / 2), height, height, voffset, voffset, 1, 1);
	}
	sbs->ResetWalls();
	sbs->ResetExtents();

	if (index != -1 && !orig_indicator)
		orig_indicator = FloorIndicator_state->GetPolygonMaterial(index);

	return index;
}

int Elevator::AddDoors(const char *texture, float thickness, float CenterX, float CenterZ, float width, float height, bool direction, float tw, float th)
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

int Elevator::AddShaftDoors(const char *texture, float thickness, float CenterX, float CenterZ, float tw, float th)
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
		base = sbs->GetFloor(ServicedFloors[i])->InterfloorHeight; //relative to floor
		base2 = sbs->GetFloor(ServicedFloors[i])->Altitude + base; //absolute

		//cut shaft and floor walls
		if (DoorDirection == false)
		{
			sbs->GetShaft(AssignedShaft)->CutWall(false, ServicedFloors[i], csVector3(Origin.x + x1 - 2, base, Origin.z + z1), csVector3(Origin.x + x1 + 2, base + DoorHeight, Origin.z + z4), 1, "Shaft");
			sbs->GetFloor(ServicedFloors[i])->Cut(csVector3(Origin.x + x1 - 2, base, Origin.z + z1), csVector3(Origin.x + x1 + 2, base + DoorHeight, Origin.z + z4), true, false, true, 2, "Shaft");
		}
		else
		{
			sbs->GetShaft(AssignedShaft)->CutWall(false, ServicedFloors[i], csVector3(Origin.x + x1, base, Origin.z + z1 - 2), csVector3(Origin.x + x4, base + DoorHeight, Origin.z + z1 + 2), 1, "Shaft");
			sbs->GetFloor(ServicedFloors[i])->Cut(csVector3(Origin.x + x1, base, Origin.z + z1 - 2), csVector3(Origin.x + x4, base + DoorHeight, Origin.z + z1 + 2), true, false, true, 2, "Shaft");
		}

		//create doorway walls
		sbs->AddDoorwayWalls(sbs->GetFloor(ServicedFloors[i])->Level_state, "ConnectionWall", 0, 0);

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
		float xoffset = Origin.x - sbs->GetShaft(AssignedShaft)->origin.x;
		float zoffset = Origin.z - sbs->GetShaft(AssignedShaft)->origin.z;
		sbs->GetShaft(AssignedShaft)->AddWall(ServicedFloors[i], "ShaftDoorF1", "Connection", thickness, xoffset + x1, zoffset + z1, xoffset + x4, zoffset + z4, 1, 1, base - 1.001, base - 1.001, 0, 0);
		sbs->GetShaft(AssignedShaft)->AddWall(ServicedFloors[i], "ShaftDoorF2", "Connection", thickness, xoffset + x1, zoffset + z1, xoffset + x4, zoffset + z4, 1, 1, base + DoorHeight + 0.001, base + DoorHeight + 0.001, 0, 0);

		//make doors invisible on start
		ShaftDoorL[i]->GetFlags().Set (CS_ENTITY_INVISIBLEMESH);
		ShaftDoorL[i]->GetFlags().Set (CS_ENTITY_NOSHADOWS);
		ShaftDoorL[i]->GetFlags().Set (CS_ENTITY_NOHITBEAM);

		ShaftDoorR[i]->GetFlags().Set (CS_ENTITY_INVISIBLEMESH);
		ShaftDoorR[i]->GetFlags().Set (CS_ENTITY_NOSHADOWS);
		ShaftDoorR[i]->GetFlags().Set (CS_ENTITY_NOHITBEAM);
	}
	sbs->ResetWalls();
	sbs->ResetExtents();

	//relocate chime sound object
	chime->SetPosition(csVector3(ShaftDoorOrigin.x, ShaftDoorOrigin.y + DoorHeight, ShaftDoorOrigin.z));

	return 0;
}

int Elevator::AddPlaque(const char *texture, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th)
{
	sbs->DrawWalls(true, false, false, false, false, false);
	sbs->ReverseExtents(false, false, false);
	return sbs->AddWallMain(Plaque_state, "Plaque", texture, 0, x1, z1, x2, z2, height, height, voffset, voffset, tw, th);
	sbs->ResetWalls();
	sbs->ResetExtents();
}

const csVector3 Elevator::GetPosition()
{
	//returns the elevator's position
	return Elevator_movable->GetPosition();
}

void Elevator::DumpQueues()
{
	//dump both (up and down) elevator queues

	sbs->Report("--- Elevator " + csString(_itoa(Number, intbuffer, 10)) + " Queues ---\n");
	sbs->Report("Up:");
	for (size_t i = 0; i < UpQueue.GetSize(); i++)
		sbs->Report(csString(_itoa(i, intbuffer, 10)) + " - " + csString(_itoa(UpQueue[i], intbuffer, 10)));
	sbs->Report("Down:");
	for (size_t i = 0; i < DownQueue.GetSize(); i++)
		sbs->Report(csString(_itoa(i, intbuffer, 10)) + " - " + csString(_itoa(DownQueue[i], intbuffer, 10)));
}

void Elevator::Enabled(bool value)
{
	//shows/hides elevator
	if (value == true)
	{
		ElevatorMesh->GetFlags().Reset (CS_ENTITY_INVISIBLEMESH);
		ElevatorMesh->GetFlags().Reset (CS_ENTITY_NOSHADOWS);
		ElevatorMesh->GetFlags().Reset (CS_ENTITY_NOHITBEAM);

		ElevatorDoorL->GetFlags().Reset (CS_ENTITY_INVISIBLEMESH);
		ElevatorDoorL->GetFlags().Reset (CS_ENTITY_NOSHADOWS);
		ElevatorDoorL->GetFlags().Reset (CS_ENTITY_NOHITBEAM);

		ElevatorDoorR->GetFlags().Reset (CS_ENTITY_INVISIBLEMESH);
		ElevatorDoorR->GetFlags().Reset (CS_ENTITY_NOSHADOWS);
		ElevatorDoorR->GetFlags().Reset (CS_ENTITY_NOHITBEAM);

		IsEnabled = true;
	}
	else
	{
		ElevatorMesh->GetFlags().Set (CS_ENTITY_INVISIBLEMESH);
		ElevatorMesh->GetFlags().Set (CS_ENTITY_NOSHADOWS);
		ElevatorMesh->GetFlags().Set (CS_ENTITY_NOHITBEAM);

		ElevatorDoorL->GetFlags().Set (CS_ENTITY_INVISIBLEMESH);
		ElevatorDoorL->GetFlags().Set (CS_ENTITY_NOSHADOWS);
		ElevatorDoorL->GetFlags().Set (CS_ENTITY_NOHITBEAM);

		ElevatorDoorR->GetFlags().Set (CS_ENTITY_INVISIBLEMESH);
		ElevatorDoorR->GetFlags().Set (CS_ENTITY_NOSHADOWS);
		ElevatorDoorR->GetFlags().Set (CS_ENTITY_NOHITBEAM);

		IsEnabled = false;
	}
}

void Elevator::EnableObjects(bool value)
{
	//enable or disable interior objects, such as floor indicator, button panel and plaque
	if (value == true)
	{
		FloorIndicator->GetFlags().Reset (CS_ENTITY_INVISIBLEMESH);
		FloorIndicator->GetFlags().Reset (CS_ENTITY_NOSHADOWS);
		FloorIndicator->GetFlags().Reset (CS_ENTITY_NOHITBEAM);

		Plaque->GetFlags().Reset (CS_ENTITY_INVISIBLEMESH);
		Plaque->GetFlags().Reset (CS_ENTITY_NOSHADOWS);
		Plaque->GetFlags().Reset (CS_ENTITY_NOHITBEAM);

		if (Panel)
			Panel->Enabled(true);
		if (Panel2)
			Panel2->Enabled(true);
	}
	else
	{
		FloorIndicator->GetFlags().Set (CS_ENTITY_INVISIBLEMESH);
		FloorIndicator->GetFlags().Set (CS_ENTITY_NOSHADOWS);
		FloorIndicator->GetFlags().Set (CS_ENTITY_NOHITBEAM);

		Plaque->GetFlags().Set (CS_ENTITY_INVISIBLEMESH);
		Plaque->GetFlags().Set (CS_ENTITY_NOSHADOWS);
		Plaque->GetFlags().Set (CS_ENTITY_NOHITBEAM);

		if (Panel)
			Panel->Enabled(false);
		if (Panel2)
			Panel2->Enabled(false);
	}
}

void Elevator::ShaftDoorsEnabled(int floor, bool value)
{
	//turns shaft elevator doors on/off

	//leave top and bottom on
	if ((floor == sbs->GetShaft(AssignedShaft)->startfloor || floor == sbs->GetShaft(AssignedShaft)->endfloor) && value == false)
		return;

	size_t index = ServicedFloors.Find(floor);
	if (index == csArrayItemNotFound)
		return;

	//exit if the specified floor has no shaft doors
	if (ShaftDoorL[index] == 0)
		return;

	if (value == true)
	{
		ShaftDoorL[index]->GetFlags().Reset (CS_ENTITY_INVISIBLEMESH);
		ShaftDoorL[index]->GetFlags().Reset (CS_ENTITY_NOSHADOWS);
		ShaftDoorL[index]->GetFlags().Reset (CS_ENTITY_NOHITBEAM);

		ShaftDoorR[index]->GetFlags().Reset (CS_ENTITY_INVISIBLEMESH);
		ShaftDoorR[index]->GetFlags().Reset (CS_ENTITY_NOSHADOWS);
		ShaftDoorR[index]->GetFlags().Reset (CS_ENTITY_NOHITBEAM);
	}
	else
	{
		ShaftDoorL[index]->GetFlags().Set (CS_ENTITY_INVISIBLEMESH);
		ShaftDoorL[index]->GetFlags().Set (CS_ENTITY_NOSHADOWS);
		ShaftDoorL[index]->GetFlags().Set (CS_ENTITY_NOHITBEAM);

		ShaftDoorR[index]->GetFlags().Set (CS_ENTITY_INVISIBLEMESH);
		ShaftDoorR[index]->GetFlags().Set (CS_ENTITY_NOSHADOWS);
		ShaftDoorR[index]->GetFlags().Set (CS_ENTITY_NOHITBEAM);
	}
}

void Elevator::ShaftDoorsEnabledRange(int floor, int range)
{
	//turn on a range of floors
	//if range is 3, show shaft door on current floor (floor), and 1 floor below and above (3 total floors)
	//if range is 1, show door on only the current floor (floor)

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

bool Elevator::IsElevator(csRef<iMeshWrapper> test)
{
	if (test == ElevatorMesh)
		return true;

	return false;
}

csHitBeamResult Elevator::HitBeam(const csVector3 &start, const csVector3 &end)
{
	//passes info onto HitBeam function
	return ElevatorMesh->HitBeam(start, end);
}

bool Elevator::IsInElevator(const csVector3 &position)
{
	if (position.y > GetPosition().y && position.y < GetPosition().y + Height)
	{
		csHitBeamResult result = ElevatorMesh->HitBeam(position, csVector3(position.x, position.y - Height, position.z));
		return result.hit;
	}
	return false;
}

float Elevator::GetElevatorStart()
{
	//returns the internal elevator starting position
	return ElevatorStart;
}

bool Elevator::AreDoorsOpen()
{
	//returns the internal door state
	return DoorsOpen;
}

float Elevator::GetDestination()
{
	//returns the internal destination value
	return Destination;
}

float Elevator::GetStoppingDistance()
{
	//returns the internal stopping distance value
	return StoppingDistance;
}

bool Elevator::GetBrakeStatus()
{
	//returns the internal brake status value
	return Brakes;
}

float Elevator::GetCurrentDoorSpeed()
{
	//returns the internal door speed value
	return ElevatorDoorSpeed;
}

bool Elevator::GetEmergencyStopStatus()
{
	//returns the internal emergency stop status
	return EmergencyStop;
}

void Elevator::DumpServicedFloors()
{
	//dump serviced floors list

	sbs->Report("--- Elevator " + csString(_itoa(Number, intbuffer, 10)) + "'s Serviced Floors ---\n");
	for (size_t i = 0; i < ServicedFloors.GetSize(); i++)
		sbs->Report(csString(_itoa(i, intbuffer, 10)) + " - " + csString(_itoa(ServicedFloors[i], intbuffer, 10)));
}

void Elevator::AddServicedFloor(int number)
{
	if (ServicedFloors.Find(number) == csArrayItemNotFound)
		ServicedFloors.InsertSorted(number);
}

void Elevator::RemoveServicedFloor(int number)
{
	if (ServicedFloors.Find(number) != csArrayItemNotFound)
		ServicedFloors.Delete(number);
}

void Elevator::CreateButtonPanel(const char *texture, int rows, int columns, const char *direction, float CenterX, float CenterZ, float buttonwidth, float buttonheight, float spacingX, float spacingY, float voffset, float tw, float th)
{
	//create a new button panel object and store the pointer
	if (!Panel)
		Panel = new ButtonPanel(Number, 1, texture, rows, columns, direction, CenterX, CenterZ, buttonwidth, buttonheight, spacingX, spacingY, voffset, tw, th);
	else if (!Panel2)
		Panel2 = new ButtonPanel(Number, 2, texture, rows, columns, direction, CenterX, CenterZ, buttonwidth, buttonheight, spacingX, spacingY, voffset, tw, th);
	else
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Button panels already exist");
}

void Elevator::UpdateFloorIndicators()
{
	//changes the number texture on the floor indicators to the elevator's current floor

	csString texture = "Button" + sbs->GetFloor(GetFloor())->ID;

	sbs->ChangeTexture(FloorIndicator, orig_indicator, texture.GetData());
}

float Elevator::GetJerkRate()
{
	return JerkRate;
}

float Elevator::GetJerkPosition()
{
	return JerkPos;
}

void Elevator::Timer::Notify()
{
	//door autoclose timer

	//close doors if open
	if (elevator->AreDoorsOpen() == true)
		elevator->CloseDoors();
}

void Elevator::Chime(int floor)
{
	//play chime sound on specified floor
	chime->Load(ChimeSound.GetData());
	chime->Loop(false);
	chime->SetPositionY(sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + DoorHeight);
	chime->Play();
}
