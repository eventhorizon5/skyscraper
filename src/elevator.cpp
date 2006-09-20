/* $Id$ */

/*
	Scalable Building Simulator - Elevator Subsystem Class
	The Skyscraper Project - Version 1.1 Alpha
	Copyright (C)2005-2006 Ryan Thoryk
	http://www.tliquest.net/skyscraper
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

//#include <wx/wx.h>
#include <crystalspace.h>
#include "elevator.h"
#include "sbs.h"
#include "camera.h"
#include "shaft.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine
extern Camera *c; //external pointer to the camera

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
	ElevatorSync = false;
	MoveElevator = false;
	MoveElevatorFloor = 0;
	GotoFloor = 0;
	OpenDoor = 0;
	Acceleration = 0;
	Deceleration = 0;
	OpenSpeed = 0;
	ElevatorStart = 0;
	ElevatorFloor = 0;
	DoorsOpen = false;
	ElevatorDirection = 0;
	DistanceToTravel = 0;
	Destination = 0;
	ElevatorRate = 0;
	StoppingDistance = 0;
	CalculateStoppingDistance = false;
	Brakes = false;
	ElevatorDoorSpeed = 0;
	ElevWait = false;
	EmergencyStop = false;

	//create object meshes
	buffer = Number;
	buffer.Insert(0, "Elevator ");
	buffer.Trim();
	ElevatorMesh = (sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData()));
	Elevator_object = ElevatorMesh->GetMeshObject ();
	Elevator_factory = Elevator_object->GetFactory();
	Elevator_state = scfQueryInterface<iThingFactoryState> (Elevator_factory);
	Elevator_movable = ElevatorMesh->GetMovable();
	ElevatorMesh->SetZBufMode(CS_ZBUF_USE);

	buffer = Number;
	buffer.Insert(0, "FloorIndicator ");
	buffer.Trim();
	FloorIndicator = (sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData()));
	FloorIndicator_object = FloorIndicator->GetMeshObject ();
	FloorIndicator_factory = FloorIndicator_object->GetFactory();
	FloorIndicator_state = scfQueryInterface<iThingFactoryState> (FloorIndicator_factory);
	FloorIndicator_movable = FloorIndicator->GetMovable();
	FloorIndicator->SetZBufMode(CS_ZBUF_USE);

	buffer = Number;
	buffer.Insert(0, "ElevatorDoorL ");
	buffer.Trim();
	ElevatorDoorL = (sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData()));
	ElevatorDoorL_object = ElevatorDoorL->GetMeshObject ();
	ElevatorDoorL_factory = ElevatorDoorL_object->GetFactory();
	ElevatorDoorL_state = scfQueryInterface<iThingFactoryState> (ElevatorDoorL_factory);
	ElevatorDoorL_movable = ElevatorDoorL->GetMovable();
	ElevatorDoorL->SetZBufMode(CS_ZBUF_USE);

	buffer = Number;
	buffer.Insert(0, "ElevatorDoorR ");
	buffer.Trim();
	ElevatorDoorR = (sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData()));
	ElevatorDoorR_object = ElevatorDoorR->GetMeshObject ();
	ElevatorDoorR_factory = ElevatorDoorR_object->GetFactory();
	ElevatorDoorR_state = scfQueryInterface<iThingFactoryState> (ElevatorDoorR_factory);
	ElevatorDoorR_movable = ElevatorDoorR->GetMovable();
	ElevatorDoorR->SetZBufMode(CS_ZBUF_USE);

	buffer = Number;
	buffer.Insert(0, "Plaque ");
	buffer.Trim();
	Plaque = (sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData()));
	Plaque_object = Plaque->GetMeshObject ();
	Plaque_factory = Plaque_object->GetFactory();
	Plaque_state = scfQueryInterface<iThingFactoryState> (Plaque_factory);
	Plaque_movable = Plaque->GetMovable();
	Plaque->SetZBufMode(CS_ZBUF_USE);
}

Elevator::~Elevator()
{
	//Destructor
	UpQueue.DeleteAll();
	DownQueue.DeleteAll();
}

void Elevator::CreateElevator(double x, double z, int floor)
{
	//Creates elevator at specified location and floor
	//x and z are the center coordinates

	//set data
	Origin = csVector3(x, sbs->FloorArray[floor]->Altitude, z);
	OriginFloor = floor;

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
	
	sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": created at " + csString(_gcvt(x, 12, buffer)) + ", " + csString(_gcvt(z, 12, buffer)) + ", " + csString(_itoa(floor, buffer, 12)));
}

void Elevator::AddRoute(int floor, bool directionup)
{
	//Add call route to elevator routing table, in sorted order

	if (directionup == true)
	{
		if (UpQueue.Length() == 0 && QueuePositionDirection == 0)
			PauseQueueSearch = false;
		UpQueue.InsertSorted(floor);
		LastQueueFloor[0] = floor;
		LastQueueFloor[1] = 1;
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": adding route to floor " + csString(_itoa(floor, intbuffer, 10)) + " direction up");
	}
	else
	{
		if (DownQueue.Length() == 0 && QueuePositionDirection == 0)
			PauseQueueSearch = false;
		DownQueue.InsertSorted(floor);
		LastQueueFloor[0] = floor;
		LastQueueFloor[1] = -1;
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": adding route to floor " + csString(_itoa(floor, intbuffer, 10)) + " direction down");
	}

}

void Elevator::DeleteRoute(int floor, bool directionup)
{
	//Delete call route from elevator routing table

	if (directionup == true)
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
		DeleteRoute(LastQueueFloor[0], true);
		LastQueueFloor[0] = 0;
		LastQueueFloor[1] = 0;
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": cancelling last route");
	}
	else if (LastQueueFloor[1] == -1)
	{
		DeleteRoute(LastQueueFloor[0], false);
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

void Elevator::OpenShaftDoors(int floor)
{
	//Simulates manually opening shaft doors
	//Slowly opens the shaft's elevator doors no matter where elevator is.
	//Cannot be used with OpenDoorsEmergency.
	//This is if the elevator is not lined up with the shaft doors,
	//and the user needs to open the shaft doors, usually while on top of elevator.

}

void Elevator::ProcessCallQueue()
{
	//Processes the elevator's call queue, and sends elevators to called floors

	if (QueuePositionDirection == 0)
	{
		if (UpQueue.Length() != 0)
			QueuePositionDirection = 1;
		if (DownQueue.Length() != 0)
			QueuePositionDirection = -1;
	}
	if (UpQueue.Length() == 0 && DownQueue.Length() == 0)
	{
		QueuePositionDirection = 0;
		PauseQueueSearch = true;
		return;
	}
	if (QueuePositionDirection == 1 && UpQueue.Length() == 0)
		QueuePositionDirection = 0;
	if (QueuePositionDirection == -1 && DownQueue.Length() == 0)
		QueuePositionDirection = 0;
	
	if (PauseQueueSearch == true)
		return;

	//Search through queue lists and find next valid floor call (direction-wise)
	if (QueuePositionDirection == 1)
	{
		//search through up queue
		for (int i = 0; i < UpQueue.Length(); i++)
		{
			if (UpQueue[i] > ElevatorFloor)
			{
				PauseQueueSearch = true;
				CloseDoors();
				GotoFloor = UpQueue[i];
				//ElevatorSync = true;
				MoveElevator = true;
				DeleteRoute(UpQueue[i], true);
				return;
			}
		}
	}
	else if (QueuePositionDirection == -1)
	{
		//search through down queue
		for (int i = 0; i < DownQueue.Length(); i++)
		{
			if (DownQueue[i] > ElevatorFloor)
			{
				PauseQueueSearch = true;
				CloseDoors();
				GotoFloor = DownQueue[i];
				//ElevatorSync = true;
				MoveElevator = true;
				DeleteRoute(DownQueue[i], true);
				return;
			}
		}
	}
}

int Elevator::GetElevatorFloor()
{
	//Determine floor that the elevator is on

	int i;
	for (i = -sbs->Basements; i <= sbs->TotalFloors; i++)
	{
		if (i < sbs->TotalFloors)
			if ((GetPosition().y >= sbs->FloorArray[i]->Altitude) && (GetPosition().y < sbs->FloorArray[i + 1]->Altitude))
				return i;
			if ((i == sbs->TotalFloors) && (GetPosition().y >= sbs->FloorArray[i]->Altitude))
				return i;
	}

	return 0;
}

void Elevator::MonitorLoop()
{
	//Monitors elevator and starts actions if needed

	//check to see if person is in elevator
	CheckElevator();

	//call queue processor
	ProcessCallQueue();

	if (OpenDoor == 1)
		MoveDoors(true, false);
	if (OpenDoor == -1)
		MoveDoors(false, false);
	if (OpenDoor == 2)
		MoveDoors(true, true);
	if (OpenDoor == -2)
		MoveDoors(false, true);

	if (MoveElevator == true)
		MoveElevatorToFloor();

}

void Elevator::OpenDoorsEmergency()
{
	//Simulates manually prying doors open.
	//Slowly opens the elevator doors no matter where elevator is.
	//If lined up with shaft doors, then opens the shaft doors also

	if (OpenDoor != 0)
	{
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": doors in use");
		return;
	}

	sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": manually opening doors");
	OpenDoor = 2;
	MoveDoors(true, true);
}

void Elevator::CloseDoorsEmergency()
{
	//Simulates manually closing doors.
	//Slowly closes the elevator doors no matter where elevator is.
	//If lined up with shaft doors, then closes the shaft doors also

	if (OpenDoor != 0)
	{
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": doors in use");
		return;
	}

	sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": manually closing doors");
	OpenDoor = -2;
	MoveDoors(false, true);
}

void Elevator::OpenDoors()
{
	//Opens elevator doors

	if (OpenDoor != 0)
	{
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": doors in use");
		return;
	}

	sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": opening doors");
	OpenDoor = 1;
	MoveDoors(true, false);
}

void Elevator::CloseDoors()
{
	//Closes elevator doors

	if (OpenDoor != 0)
	{
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": doors in use");
		return;
	}

	sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": closing doors");
	OpenDoor = -1;
	MoveDoors(false, false);
}

void Elevator::MoveDoors(bool open, bool emergency)
{
	//opens or closes elevator doors
	//currently only supports doors on either the left/right or front/back
	//diagonal doors will be done later, by possibly using relative plane movement

	static bool IsRunning;
	static double OpenChange;
	static float marker1;
	static float marker2;

	//todo: turn off autoclose timer

	if (IsRunning == false)
	{
		//initialization code

		IsRunning = true;

		//get starting FPS and hold value
		FPSModifierStatic = sbs->FPSModifier;

		if (emergency == false)
		{
			OpenChange = OpenSpeed / 50;
			marker1 = DoorWidth / 8;
			marker2 = (DoorWidth / 2) - (DoorWidth / 8);
		}
		else
		{
			OpenChange = 0.0001;
			marker1 = DoorWidth / 16;
			marker2 = (DoorWidth / 2) - (DoorWidth / 16);
		}

		if (emergency == false)
		{
			//play elevator opening sound
			//"data/elevatoropen.wav"
			//"data/elevatorclose.wav"
		}

		ElevatorDoorSpeed = 0;
	}

	//Speed up doors
	if (DoorDirection == false)
	{
		if ((DoorOrigin.z - ElevatorDoorL_movable->GetPosition().z <= marker1 && open == true) || (DoorOrigin.z - ElevatorDoorL_movable->GetPosition().z > marker2 && open == false))
		{
			if (open == true)
				ElevatorDoorSpeed += OpenChange;
			else
				ElevatorDoorSpeed -= OpenChange;

			//todo: move shaft doors here
			ElevatorDoorL_movable->MovePosition(csVector3(0, 0, -ElevatorDoorSpeed * FPSModifierStatic));
			ElevatorDoorL_movable->UpdateMove();
			ElevatorDoorR_movable->MovePosition(csVector3(0, 0, ElevatorDoorSpeed * FPSModifierStatic));
			ElevatorDoorR_movable->UpdateMove();
			return;
		}
	}
	else
	{
		if ((DoorOrigin.x - ElevatorDoorL_movable->GetPosition().x <= marker1 && open == true) || (DoorOrigin.x - ElevatorDoorL_movable->GetPosition().x > marker2 && open == false))
		{
			if (open == true)
				ElevatorDoorSpeed += OpenChange;
			else
				ElevatorDoorSpeed -= OpenChange;

			//todo: move shaft doors here
			ElevatorDoorL_movable->MovePosition(csVector3(-ElevatorDoorSpeed * FPSModifierStatic, 0, 0));
			ElevatorDoorL_movable->UpdateMove();
			ElevatorDoorR_movable->MovePosition(csVector3(ElevatorDoorSpeed * FPSModifierStatic, 0, 0));
			ElevatorDoorR_movable->UpdateMove();
			return;
		}
	}

	//Normal door movement
	if (DoorDirection == false)
	{
		if ((DoorOrigin.z - ElevatorDoorL_movable->GetPosition().z <= marker2 && open == true) || (DoorOrigin.z - ElevatorDoorL_movable->GetPosition().z > marker1 && open == false))
		{
			//todo: move shaft doors here
			ElevatorDoorL_movable->MovePosition(csVector3(0, 0, -ElevatorDoorSpeed * FPSModifierStatic));
			ElevatorDoorL_movable->UpdateMove();
			ElevatorDoorR_movable->MovePosition(csVector3(0, 0, ElevatorDoorSpeed * FPSModifierStatic));
			ElevatorDoorR_movable->UpdateMove();
			return;
		}
	}
	else
	{
		if ((DoorOrigin.x - ElevatorDoorL_movable->GetPosition().x <= marker2 && open == true) || (DoorOrigin.x - ElevatorDoorL_movable->GetPosition().x > marker1 && open == false))
		{
			//todo: move shaft doors here
			ElevatorDoorL_movable->MovePosition(csVector3(-ElevatorDoorSpeed * FPSModifierStatic, 0, 0));
			ElevatorDoorL_movable->UpdateMove();
			ElevatorDoorR_movable->MovePosition(csVector3(ElevatorDoorSpeed * FPSModifierStatic, 0, 0));
			ElevatorDoorR_movable->UpdateMove();
			return;
		}
	}

	//slow down doors
	if ((ElevatorDoorSpeed > 0 && open == true) || (ElevatorDoorSpeed < 0 && open == false))
	{
		if (open == true)
			ElevatorDoorSpeed -= OpenChange;
		else
			ElevatorDoorSpeed += OpenChange;
		if (DoorDirection == false)
		{
			//todo: move shaft doors here
			ElevatorDoorL_movable->MovePosition(csVector3(0, 0, -ElevatorDoorSpeed * FPSModifierStatic));
			ElevatorDoorL_movable->UpdateMove();
			ElevatorDoorR_movable->MovePosition(csVector3(0, 0, ElevatorDoorSpeed * FPSModifierStatic));
			ElevatorDoorR_movable->UpdateMove();
		}
		else
		{
			//todo: move shaft doors here
			ElevatorDoorL_movable->MovePosition(csVector3(-ElevatorDoorSpeed * FPSModifierStatic, 0, 0));
			ElevatorDoorL_movable->UpdateMove();
			ElevatorDoorR_movable->MovePosition(csVector3(ElevatorDoorSpeed * FPSModifierStatic, 0, 0));
			ElevatorDoorR_movable->UpdateMove();
		}
		return;
	}

	//reset values
	ElevatorDoorSpeed = 0;
	OpenDoor = 0;

	//turn on autoclose timer

	//the doors are open or closed now
	if (open == true)
		DoorsOpen = true;
	else
		DoorsOpen = false;

	IsRunning = false;

}

void Elevator::MoveElevatorToFloor()
{
	//Main processing routine; sends elevator to floor specified in GotoFloor
	static bool IsRunning;

	if (IsRunning == false)
	{
		IsRunning = true;
		
		//get elevator's current altitude
		ElevatorStart = GetPosition().y;

		//get elevator's current floor
		ElevatorFloor = GetElevatorFloor();

		//If elevator is already on specified floor, open doors and exit
		if (ElevatorFloor == GotoFloor)
		{
			OpenDoors();
			return;
		}

		//close doors if open
		if (DoorsOpen == true)
			CloseDoors();

		//Determine direction
		if (GotoFloor < ElevatorFloor)
			ElevatorDirection = -1;
		if (GotoFloor > ElevatorFloor)
			ElevatorDirection = 1;

		//Determine distance to destination floor
		DistanceToTravel = abs(abs(sbs->FloorArray[GotoFloor]->Altitude) - abs(ElevatorStart));
		Destination = sbs->FloorArray[GotoFloor]->Altitude;
		CalculateStoppingDistance = true;

		//If user is riding this elevator, then turn off floor
		if (ElevatorSync == true)
			sbs->FloorArray[c->CurrentFloor]->Enabled(false);

		//Turn off sky, buildings, and landscape
		sbs->EnableSkybox(false);
		sbs->EnableBuildings(false);
		sbs->EnableLandscape(false);

		//Play starting sound
		//"\data\elevstart.wav"

		//Get first rate increment value
		ElevatorRate = ElevatorDirection * (ElevatorSpeed * Acceleration);

		//get starting frame rate and hold value
		FPSModifierStatic = sbs->FPSModifier;
	}

	if (EmergencyStop == true && Brakes == false)
	{
		CalculateStoppingDistance = false;
		TempDeceleration = Deceleration;
		if (ElevatorDirection == 1)
			ElevatorDirection = -1;
		else
			ElevatorDirection = 1;
		Brakes = true;
	}

	//Movement sound
    //"\data\elevmove.wav"

	//move elevator objects and camera
	Elevator_movable->MovePosition(csVector3(0, ElevatorRate * FPSModifierStatic, 0));
	Elevator_movable->UpdateMove();
	if (ElevatorSync == true)
		c->SetPosition(csVector3(c->GetPosition().x, GetPosition().y + c->DefaultAltitude, c->GetPosition().z));
	ElevatorDoorL_movable->MovePosition(csVector3(0, ElevatorRate * FPSModifierStatic, 0));
	ElevatorDoorL_movable->UpdateMove();
	ElevatorDoorR_movable->MovePosition(csVector3(0, ElevatorRate * FPSModifierStatic, 0));
	ElevatorDoorR_movable->UpdateMove();
	FloorIndicator_movable->MovePosition(csVector3(0, ElevatorRate * FPSModifierStatic, 0));
	FloorIndicator_movable->UpdateMove();
	Plaque_movable->MovePosition(csVector3(0, ElevatorRate * FPSModifierStatic, 0));
	Plaque_movable->UpdateMove();

	//move sounds

	//motion calculation
	if (Brakes == false)
	{
		//regular motion
		if (ElevatorDirection == 1)
			ElevatorRate = ElevatorRate + (ElevatorSpeed * Acceleration);
		if (ElevatorDirection == -1)
			ElevatorRate = ElevatorRate - (ElevatorSpeed * Acceleration);
	}
	else
	{
		//slow down
		if (ElevatorDirection == 1)
			ElevatorRate = ElevatorRate + (ElevatorSpeed * TempDeceleration);
		if (ElevatorDirection == -1)
			ElevatorRate = ElevatorRate - (ElevatorSpeed * TempDeceleration);
	}

	//change speeds
	if ((ElevatorDirection == 1) && (ElevatorRate > ElevatorSpeed))
		ElevatorRate = ElevatorSpeed;
	if ((ElevatorDirection == -1) && (ElevatorRate < -ElevatorSpeed))
		ElevatorRate = -ElevatorSpeed;
	if ((ElevatorDirection == 1) && (Brakes == true) && (ElevatorRate > 0))
		ElevatorRate = 0;
	if ((ElevatorDirection == -1) && (Brakes == true) && (ElevatorRate < 0))
		ElevatorRate = 0;

	//get distance needed to stop elevator
	if (CalculateStoppingDistance == true)
	{
		if (ElevatorDirection == 1)
			StoppingDistance = (GetPosition().y - ElevatorStart) * (Acceleration / Deceleration);
		else if (ElevatorDirection == -1)
			StoppingDistance = (ElevatorStart - GetPosition().y) * (Acceleration / Deceleration);
	}

	if (abs(ElevatorRate) == ElevatorSpeed)
		CalculateStoppingDistance = false;

	//Deceleration routines with floor overrun correction (there's still problems, but it works pretty good)
	//since this function cycles at a slower/less granular rate (cycles according to frames per sec), an error factor is present where the elevator overruns the dest floor,
	//even though the algorithms are all correct. Since the elevator moves by "jumping" to a new altitude every frame - and usually jumps right over the altitude value where it is supposed to
	//start the deceleration process, causing the elevator to decelerate too late, and end up passing/overrunning the dest floor's altitude.  This code corrects this problem
	//by determining if the next "jump" will overrun the deceleration marker (which is Dest's Altitude minus Stopping Distance), and temporarily altering the deceleration rate according to how far off the mark it is
	//and then starting the deceleration process immediately.

	//up movement
	if ((Brakes == false) && (ElevatorDirection == 1))
	{
		//determine if next jump altitude is over deceleration marker
		if (((GetPosition().y + (ElevatorRate * FPSModifierStatic)) > (Destination - StoppingDistance)) && (GetPosition().y != (Destination - StoppingDistance)))
		{
			CalculateStoppingDistance = false;
			//recalculate deceleration value based on distance from marker, and store result in tempdeceleration
			TempDeceleration = Deceleration * (StoppingDistance / (Destination - GetPosition().y));
			//start deceleration
			ElevatorDirection = -1;
			Brakes = true;
			//stop sounds
			//play elevator stopping sound
			//"\data\elevstop.wav"
		}
    
		//normal routine - this will rarely happen (only if the elevator happens to reach the exact deceleration marker)
		if (GetPosition().y == (Destination - StoppingDistance))
		{
			TempDeceleration = Deceleration;
			CalculateStoppingDistance = false;
			//slow down elevator
			ElevatorDirection = -1;
			Brakes = true;
			//stop sounds
			//play stopping sound
			//"\data\elevstop.wav"
		}
	}

	//down movement
	if (Brakes == false && ElevatorDirection == -1)
	{
        //determine if next jump altitude is below deceleration marker
		if (((GetPosition().y - (ElevatorRate * FPSModifierStatic)) < (Destination + StoppingDistance)) && (GetPosition().y != (Destination + StoppingDistance)))
		{
			CalculateStoppingDistance = false;
			//recalculate deceleration value based on distance from marker, and store result in tempdeceleration
			TempDeceleration = Deceleration * (StoppingDistance / (GetPosition().y - Destination));
			//start deceleration
			ElevatorDirection = 1;
			Brakes = true;
			//stop sounds
			//play stopping sound
            //"\data\elevstop.wav"
		}
    
		//normal routine - this will rarely happen (only if the elevator happens to reach the exact deceleration marker)
		if (GetPosition().y == (Destination + StoppingDistance))
		{
			TempDeceleration = Deceleration;
			CalculateStoppingDistance = false;
			//slow down elevator
			ElevatorDirection = 1;
			Brakes = true;
			//stop sounds
			//play stopping sound
			//"\data\elevstop.wav"
		}
	}

	//exit if elevator's running
	if (abs(ElevatorRate) != 0)
		return;

	if (EmergencyStop == false)
	{
		//store error offset value
		if (ElevatorDirection == -1)
			ErrorOffset = GetPosition().y - Destination;
		else if (ElevatorDirection == 1)
			ErrorOffset = Destination - GetPosition().y;
		else
			ErrorOffset = 0;

		//set elevator and objects to floor altitude (corrects offset errors)
		//move elevator objects and camera
		Elevator_movable->SetPosition(csVector3(GetPosition().x, Destination, GetPosition().z));
		Elevator_movable->UpdateMove();
		if (ElevatorSync == true)
			c->SetPosition(csVector3(c->GetPosition().x, GetPosition().y + c->DefaultAltitude, c->GetPosition().z));
		ElevatorDoorL_movable->SetPosition(csVector3(ElevatorDoorL_movable->GetPosition().x, Destination, ElevatorDoorL_movable->GetPosition().z));
		ElevatorDoorL_movable->UpdateMove();
		ElevatorDoorR_movable->SetPosition(csVector3(ElevatorDoorR_movable->GetPosition().x, Destination, ElevatorDoorR_movable->GetPosition().z));
		ElevatorDoorR_movable->UpdateMove();
		FloorIndicator_movable->SetPosition(csVector3(FloorIndicator_movable->GetPosition().x, Destination, FloorIndicator_movable->GetPosition().z));
		FloorIndicator_movable->UpdateMove();
		Plaque_movable->SetPosition(csVector3(Plaque_movable->GetPosition().x, Destination, Plaque_movable->GetPosition().z));
		Plaque_movable->UpdateMove();
	
		//move sounds
	}

	//reset values if at destination floor
	ElevatorRate = 0;
	ElevatorDirection = 0;
	Brakes = false;
	Destination = 0;
	DistanceToTravel = 0;
	ElevatorStart = 0;
	IsRunning = false;
	MoveElevator = false;

	if (EmergencyStop == false)
	{
		//update elevator's floor number
		GetElevatorFloor();

		//Turn on floor
		if (ElevatorSync == true)
			sbs->FloorArray[GotoFloor]->Enabled(true);

		//open doors
		OpenDoors();

		//Turn on sky, buildings, and landscape
		sbs->EnableSkybox(true);
		sbs->EnableBuildings(true);
		sbs->EnableLandscape(true);
	}
	EmergencyStop = false;
}

int Elevator::AddWall(const char *texture, double x1, double z1, double x2, double z2, double height1, double height2, double voffset1, double voffset2, double tw, double th, bool revX, bool revY, bool revZ, bool DrawBothSides)
{
	return sbs->AddWallMain(Elevator_state, texture, Origin.x + x1, Origin.z + z1, Origin.x + x2, Origin.z + z2, height1, height2, voffset1 + Origin.y, voffset2 + Origin.y, tw, th, revX, revY, revZ, DrawBothSides);
}

int Elevator::AddFloor(const char *texture, double x1, double z1, double x2, double z2, double voffset1, double voffset2, double tw, double th)
{
   	return sbs->AddFloorMain(Elevator_state, texture, Origin.x + x1, Origin.z + z1, Origin.x + x2, Origin.z + z2, voffset1 + Origin.y, voffset2 + Origin.y, tw, th);
}

int Elevator::AddFloorIndicator(const char *basename, double x1, double z1, double x2, double z2, double height, double voffset, double tw, double th)
{
	//Creates a floor indicator at the specified location
	csString texture;
	BaseName = basename;
	texture = BaseName + sbs->FloorArray[OriginFloor]->ID;
	return sbs->AddWallMain(FloorIndicator_state, texture.GetData(), Origin.x + x1, Origin.z + z1, Origin.x + x2, Origin.z + z2, height, height, voffset + Origin.y, voffset + Origin.y, 0, 0, false, false, false, false);
}

int Elevator::AddDoors(const char *texture, double CenterX, double CenterZ, double width, double height, bool direction, double tw, double th)
{
	//adds elevator doors specified at a relative central position (off of elevator origin)
	//if direction is false, doors are on the left/right side; otherwise front/back
	double x1, x2, x3, x4;
	double z1, z2, z3, z4;

	//set door parameters
	DoorDirection = direction;
	DoorWidth = width;
	DoorOrigin = csVector3(Origin.x + CenterX, Origin.y, Origin.z + CenterZ);

	//set up coordinates
	if (direction == false)
	{
		x1 = DoorOrigin.x;
		x2 = DoorOrigin.x;
		x3 = DoorOrigin.x;
		x4 = DoorOrigin.x;
		z1 = DoorOrigin.z - (width / 2);
		z2 = DoorOrigin.z;
		z3 = DoorOrigin.z;
		z4 = DoorOrigin.z + (width / 2);
	}
	else
	{
		x1 = DoorOrigin.x - (width / 2);
		x2 = DoorOrigin.x;
		x3 = DoorOrigin.x;
		x4 = DoorOrigin.x + (width / 2);
		z1 = DoorOrigin.z;
		z2 = DoorOrigin.z;
		z3 = DoorOrigin.z;
		z4 = DoorOrigin.z;
	}

	//create doors
	int firstidx = sbs->AddWallMain(ElevatorDoorL_state, texture, x1, z1, x2, z2, height, height, Origin.y, Origin.y, tw, th, false, false, false);
	sbs->AddWallMain(ElevatorDoorR_state, texture, x3, z3, x4, z4, height, height, Origin.y, Origin.y, tw, th, false, false, false);
	return firstidx;
}

int Elevator::AddPlaque(const char *texture, double x1, double z1, double x2, double z2, double height, double voffset, double tw, double th)
{
	return sbs->AddWallMain(Plaque_state, texture, Origin.x + x1, Origin.z + z1, Origin.x + x2, Origin.z + z2, height, height, voffset + Origin.y, voffset + Origin.y, tw, th, false, false, false);
}

const csVector3 Elevator::GetPosition()
{
	//returns the elevator's position
	return Elevator_movable->GetPosition();
}

void Elevator::CheckElevator()
{
	//check to see if user (camera) is in the elevator

}

void Elevator::DumpQueues()
{
	//dump both (up and down) elevator queues

	sbs->Report("--- Elevator " + csString(_itoa(Number, intbuffer, 10)) + " Queues ---\n");
	sbs->Report("Up:");
	for (int i = 0; i < UpQueue.Length(); i++)
		sbs->Report(csString(_itoa(i, intbuffer, 10)) + " - " + csString(_itoa(UpQueue[i], intbuffer, 10)));
	sbs->Report("Down:");
	for (int i = 0; i < DownQueue.Length(); i++)
		sbs->Report(csString(_itoa(i, intbuffer, 10)) + " - " + csString(_itoa(DownQueue[i], intbuffer, 10)));
}
