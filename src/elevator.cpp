/* $Id$ */

/*
	Scalable Building Simulator - Elevator Subsystem Class
	The Skyscraper Project - Version 1.5 Alpha
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
#include "sbs.h"
#include "camera.h"
#include "shaft.h"
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
	LastQueueDirection = 0;
	LastQueueFloor[0] = 0;
	LastQueueFloor[1] = 0;
	PauseQueueSearch = true;
	ElevatorSpeed = 0;
	MoveElevator = false;
	GotoFloor = 0;
	Acceleration = 0;
	Deceleration = 0;
	AccelJerk = 1;
	DecelJerk = 1;
	ElevatorStart = 0;
	ElevatorFloor = 0;
	Direction = 0;
	DistanceToTravel = 0;
	Destination = 0;
	ElevatorRate = 0;
	StoppingDistance = 0;
	CalculateStoppingDistance = false;
	Brakes = false;
	EmergencyStop = false;
	AssignedShaft = 0;
	IsEnabled = true;
	Height = 0;
	Panel = 0;
	Panel2 = 0;
	TempDeceleration = 0;
	ErrorOffset = 0;
	JerkRate = 0;
	JerkPos = 0;
	ElevatorIsRunning = false;
	oldfloor = 0;
	IsMoving = false;
	lastfloor = 0;
	lastfloorset = false;
	StartSound = "elevstart.wav";
	MoveSound = "elevmove.wav";
	StopSound = "elevstop.wav";
	IdleSound = "elevidle.wav";
	AlarmSound = "bell1.wav";
	AlarmSoundStop = "bell1-stop.wav";
	UseFloorSkipText = false;
	ACP = false;
	ACPFloor = 0;
	UpPeak = false;
	DownPeak = false;
	IndependentService = false;
	InspectionService = false;
	FireServicePhase1 = 0;
	FireServicePhase2 = 0;
	RecallFloor = 0;
	RecallFloorAlternate = 0;
	ResetQueues = false;
	OnFloor = true;
	RecallSet = false;
	RecallAltSet = false;
	ACPFloorSet = false;
	RecallUnavailable = false;
	ManualGo = false;
	AlarmActive = false;
	NumDoors = 1;
	MovePending = false;

	//create object meshes
	buffer = Number;
	buffer.Insert(0, "Elevator ");
	buffer.Trim();
	ElevatorMesh = sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData());
	Elevator_state = scfQueryInterface<iThingFactoryState> (ElevatorMesh->GetMeshObject()->GetFactory());
	Elevator_movable = ElevatorMesh->GetMovable();
	ElevatorMesh->SetZBufMode(CS_ZBUF_USE);
	ElevatorMesh->SetRenderPriority(sbs->engine->GetObjectRenderPriority());

	buffer = Number;
	buffer.Insert(0, "Plaque ");
	buffer.Trim();
	Plaque = sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData());
	Plaque_state = scfQueryInterface<iThingFactoryState> (Plaque->GetMeshObject()->GetFactory());
	Plaque_movable = Plaque->GetMovable();
	Plaque->SetZBufMode(CS_ZBUF_USE);
	Plaque->SetRenderPriority(sbs->engine->GetObjectRenderPriority());
}

Elevator::~Elevator()
{
	//delete directional indicators
	for (int i = 0; i < IndicatorArray.GetSize(); i++)
	{
		if (IndicatorArray[i])
			delete IndicatorArray[i];
	}
	IndicatorArray.DeleteAll();

	//delete doors
	if (DoorArray.GetSize() > 0)
	{
		for (int i = 0; i < DoorArray.GetSize(); i++)
		{
			if (DoorArray[i])
				delete DoorArray[i];
		}
	}

	//delete floor indicators
	for (int i = 0; i < FloorIndicatorArray.GetSize(); i++)
	{
		if (FloorIndicatorArray[i])
			delete FloorIndicatorArray[i];
	}
	FloorIndicatorArray.DeleteAll();

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
	if (alarm)
		delete alarm;
	alarm = 0;
	if (floorbeep)
		delete floorbeep;
	floorbeep = 0;
}

bool Elevator::CreateElevator(bool relative, float x, float z, int floor)
{
	//Creates elevator at specified location and floor
	//x and z are the center coordinates
	//if relative is true, then x and z coordinates are relative
	//to the assigned shaft's center

	//make sure required values are set
	if (ElevatorSpeed <= 0)
	{
		sbs->ReportError("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Speed not set or invalid");
		return false;
	}
	if (Acceleration <= 0)
	{
		sbs->ReportError("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Acceleration not set or invalid");
		return false;
	}
	if (Deceleration <= 0)
	{
		sbs->ReportError("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Deceleration not set or invalid");
		return false;
	}
	if (NumDoors < 0)
	{
		sbs->ReportError("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Number of doors invalid");
		return false;
	}
	if (AccelJerk <= 0)
	{
		sbs->ReportError("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Invalid value for AccelJerk");
		return false;
	}
	if (DecelJerk <= 0)
	{
		sbs->ReportError("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Invalid value for DecelJerk");
		return false;
	}
	if (AssignedShaft <= 0)
	{
		sbs->ReportError("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Not assigned to a shaft");
		return false;
	}

	//add elevator's starting floor to serviced floor list - this also ensures that the list is populated to prevent errors
	if (IsServicedFloor(floor) == false)
		AddServicedFloor(floor);

	//set data
	Origin.y = sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight;
	if (relative == false)
	{
		Origin.x = x;
		Origin.z = z;
	}
	else
	{
		Origin.x = sbs->GetShaft(AssignedShaft)->origin.x + x;
		Origin.z = sbs->GetShaft(AssignedShaft)->origin.z + z;
	}
	OriginFloor = floor;

	//add elevator to associated shaft's list
	sbs->GetShaft(AssignedShaft)->elevators.InsertSorted(Number);

	//set recall/ACP floors if not already set
	if (RecallSet == false)
		SetRecallFloor(GetBottomFloor());
	if (RecallAltSet == false)
		SetAlternateRecallFloor(GetTopFloor());
	if (ACPFloorSet == false)
		SetACPFloor(GetBottomFloor());

	//create door objects
	if (NumDoors > 0)
	{
		DoorArray.SetSize(NumDoors);
		for (int i = 0; i < NumDoors; i++)
			DoorArray[i] = new ElevatorDoor(i, this);
	}

	//move objects to positions
	Elevator_movable->SetPosition(Origin);
	Elevator_movable->UpdateMove();
	Plaque_movable->SetPosition(Origin);
	Plaque_movable->UpdateMove();

	//resize directional indicator array
	IndicatorArray.SetSize(ServicedFloors.GetSize());

	//create sound objects
	mainsound = new Sound();
	mainsound->SetPosition(Origin);
	idlesound = new Sound();
	idlesound->SetPosition(Origin);
	idlesound->Load(IdleSound.GetData());
	alarm = new Sound();
	alarm->SetPosition(Origin);
	floorbeep = new Sound();
	floorbeep->SetPosition(Origin);

	sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": created at " + csString(_gcvt(x, 12, buffer)) + ", " + csString(_gcvt(z, 12, buffer)) + ", " + csString(_itoa(floor, buffer, 12)));
	return true;
}

void Elevator::AddRoute(int floor, int direction)
{
	//Add call route to elevator routing table, in sorted order
	//directions are either 1 for up, or -1 for down

	//if in independent service or fire service phase 2 (on) mode, go to floor in pending state
	if (IndependentService == true || FireServicePhase2 == 1)
	{
		GoPending(floor);
		return;
	}

	//do not add routes if in any other service mode
	if (InServiceMode() == true)
	{
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": cannot add route while in a service mode");
		return;
	}

	if (direction == 1)
	{
		if (UpQueue.GetSize() == 0 && QueuePositionDirection == 0)
			PauseQueueSearch = false;
		if (UpQueue.Find(floor) != csArrayItemNotFound)
		{
			//exit if entry already exits
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": route to floor " + csString(_itoa(floor, intbuffer, 10)) + " already exists");
			return;
		}
		UpQueue.InsertSorted(floor);
		LastQueueFloor[0] = floor;
		LastQueueFloor[1] = 1;
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": adding route to floor " + csString(_itoa(floor, intbuffer, 10)) + " direction up");
	}
	else
	{
		if (DownQueue.GetSize() == 0 && QueuePositionDirection == 0)
			PauseQueueSearch = false;
		if (DownQueue.Find(floor) != csArrayItemNotFound)
		{
			//exit if entry already exits
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": route to floor " + csString(_itoa(floor, intbuffer, 10)) + " already exists");
			return;
		}
		DownQueue.InsertSorted(floor);
		LastQueueFloor[0] = floor;
		LastQueueFloor[1] = -1;
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": adding route to floor " + csString(_itoa(floor, intbuffer, 10)) + " direction down");
	}

}

void Elevator::DeleteRoute(int floor, int direction)
{
	//Delete call route from elevator routing table
	//directions are either 1 for up, or -1 for down

	//if MovePending is on clear pending information (since normal routing is turned off) and quit
	if (MovePending == true)
	{
		GotoFloor = 0;
		MovePending = false;
		return;
	}

	if (direction == 1)
	{
		//delete floor entry from up queue
		UpQueue.Delete(floor);
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": deleting route to floor " + csString(_itoa(floor, intbuffer, 10)) + " direction up");
	}
	else
	{
		//delete floor entry from down queue
		DownQueue.Delete(floor);
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": deleting route to floor " + csString(_itoa(floor, intbuffer, 10)) + " direction down");
	}
}

void Elevator::CancelLastRoute()
{
	//cancels the last added route

	//LastQueueFloor holds the floor and direction of the last route; array element 0 is the floor and 1 is the direction

	if (LastQueueFloor[1] != 1 || LastQueueFloor[1] != -1)
		return;

	DeleteRoute(LastQueueFloor[0], LastQueueFloor[1]);
	LastQueueFloor[0] = 0;
	LastQueueFloor[1] = 0;
	sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": cancelling last route");
}

void Elevator::Alarm()
{
	//elevator alarm code

	if (AlarmActive == false)
	{
		//ring alarm
		AlarmActive = true;
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": alarm on");
		alarm->Load(AlarmSound.GetData());
		alarm->Loop(true);
		alarm->Play();
	}
	if (AlarmActive == true && sbs->camera->MouseDown == false)
	{
		//stop alarm
		AlarmActive = false;
		alarm->Stop();
		alarm->Load(AlarmSoundStop.GetData());
		alarm->Loop(false);
		alarm->Play();
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": alarm off");
	}
}

void Elevator::StopElevator()
{
	//Tells elevator to stop moving, no matter where it is

	//exit if in inspection mode
	if (InspectionService == true)
		return;

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

	//if ResetQueues is set, clear both queues open doors, and pause queue
	if (ResetQueues == true)
	{
		UpQueue.DeleteAll();
		DownQueue.DeleteAll();
		PauseQueueSearch = true;
		ResetQueues = false;
		return;
	}

	//exit if in a service mode
	if (InServiceMode() == true)
		return;

	//set queue search direction if queues aren't empty
	if (QueuePositionDirection == 0)
	{
		if (UpQueue.GetSize() != 0)
		{
			QueuePositionDirection = 1;
			LastQueueDirection = 1;
		}
		if (DownQueue.GetSize() != 0)
		{
			QueuePositionDirection = -1;
			LastQueueDirection = -1;
		}
	}

	//if both queues are empty
	if (UpQueue.GetSize() == 0 && DownQueue.GetSize() == 0)
	{
		int TopFloor = GetTopFloor();
		int BottomFloor = GetBottomFloor();

		//if DownPeak mode is active, send elevator to the top serviced floor if not already there
		if (GetFloor() != TopFloor && DownPeak == true && IsMoving == false)
		{
			AddRoute(TopFloor, 1);
			return;
		}
		//if UpPeak mode is active, send elevator to the bottom serviced floor if not already there
		else if (GetFloor() != BottomFloor && UpPeak == true && IsMoving == false)
		{
			AddRoute(BottomFloor, -1);
			return;
		}

		//otherwise pause queue search
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

	//if elevator is stopped and doors are closed, unpause queue
	if (QueuePositionDirection != 0 && MoveElevator == false && AreDoorsOpen() == false && CheckOpenDoor() == false)
		PauseQueueSearch = false;

	//exit if queue is paused
	if (PauseQueueSearch == true)
		return;

	//Search through queue lists and find next valid floor call
	if (QueuePositionDirection == 1)
	{
		//search through up queue
		for (size_t i = 0; i < UpQueue.GetSize(); i++)
		{
			//if the queued floor number is a different floor, dispatch the elevator to that floor
			if (UpQueue[i] > GetFloor() || (UpQueue[i] < GetFloor() && UpQueue.GetSize() == 1))
			{
				PauseQueueSearch = true;
				GotoFloor = UpQueue[i];
				CloseDoors();
				MoveElevator = true;
				DeleteRoute(UpQueue[i], 1);
				return;
			}
			//if the queued floor is the current elevator's floor, open doors and turn off related call buttons
			if (UpQueue[i] == GetFloor())
			{
				OpenDoors();
				DeleteRoute(UpQueue[i], 1);
				//turn off up call buttons if on
				SetCallButtons(GetFloor(), true, false);
			}
		}
	}
	else if (QueuePositionDirection == -1)
	{
		//search through down queue
		for (size_t i = 0; i < DownQueue.GetSize(); i++)
		{
			//if the queued floor number is a different floor, dispatch the elevator to that floor
			if (DownQueue[i] < GetFloor() || (DownQueue[i] > GetFloor() && DownQueue.GetSize() == 1))
			{
				PauseQueueSearch = true;
				GotoFloor = DownQueue[i];
				CloseDoors();
				MoveElevator = true;
				DeleteRoute(DownQueue[i], -1);
				return;
			}
			//if the queued floor is the current elevator's floor, open doors and turn off related call buttons
			if (DownQueue[i] == GetFloor())
			{
				OpenDoors();
				DeleteRoute(DownQueue[i], -1);
				//turn off down call buttons if on
				SetCallButtons(GetFloor(), false, false);
			}
		}
	}
}

int Elevator::GetFloor()
{
	//Determine floor that the elevator is on

	int newlastfloor;

	if (lastfloorset == true)
		newlastfloor = sbs->GetFloorNumber(GetPosition().y, lastfloor, true);
	else
		newlastfloor = sbs->GetFloorNumber(GetPosition().y);

	lastfloor = newlastfloor;
	lastfloorset = true;
	return lastfloor;
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
	if (((sbs->InElevator == true && sbs->ElevatorNumber == Number) || AreDoorsOpen() == true || CheckOpenDoor() == true) && idlesound->IsPlaying() == false)
	{
		idlesound->Loop(true);
		idlesound->Play();
	}
	else if (((sbs->InElevator == false || sbs->ElevatorNumber != Number) && AreDoorsOpen() == false && CheckOpenDoor() == false) && idlesound->IsPlaying() == true)
		idlesound->Stop();

	//process alarm
	if (AlarmActive == true)
		Alarm();

	//call queue processor
	ProcessCallQueue();

	//door operations
	for (int i = 1; i <= NumDoors; i++)
	{
		if (GetDoor(i))
			GetDoor(i)->Loop();
	}

	//elevator movement
	if (MoveElevator == true && (AreDoorsOpen() == false || InspectionService == true))
		MoveElevatorToFloor();

}

void Elevator::MoveElevatorToFloor()
{
	//Main processing routine; sends elevator to floor specified in GotoFloor
	//if InspectionService is enabled, this function ignores GotoFloor values, since the elevator is manually moved

	//exit if doors are moving
	if (CheckOpenDoor() == true)
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
		if (ElevatorFloor == GotoFloor && InspectionService == false)
		{
			sbs->Report("Elevator already on specified floor");
			MoveElevator = false;
			ElevatorIsRunning = false;
			//do not automatically open doors if in fire service phase 2
			if (FireServicePhase2 == 0)
				OpenDoors();
			return;
		}

		//if destination floor is not a serviced floor, reset and exit
		if (IsServicedFloor(GotoFloor) == false && InspectionService == false)
		{
			sbs->Report("Destination floor not in ServicedFloors list");
			MoveElevator = false;
			ElevatorIsRunning = false;
			return;
		}

		//Determine direction
		if (InspectionService == false)
		{
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
		}
		else
		{
			if (Direction == -1)
				dir_string = "down";
			else if (Direction == 1)
				dir_string = "up";
		}

		//Determine distance to destination floor
		if (InspectionService == false)
		{
			Destination = sbs->GetFloor(GotoFloor)->Altitude + sbs->GetFloor(GotoFloor)->InterfloorHeight;
			DistanceToTravel = fabs(fabs(Destination) - fabs(ElevatorStart));
		}
		else
		{
			//otherwise if inspection service is on, choose the altitude of the top/bottom floor
			if (Direction == 1)
			{
				Destination = sbs->GetFloor(GetTopFloor())->Altitude + sbs->GetFloor(GetTopFloor())->InterfloorHeight;
				if (ElevatorStart >= Destination)
				{
					//don't go above top floor
					Destination = 0;
					MoveElevator = false;
					ElevatorIsRunning = false;
					Direction = 0;
					return;
				}
			}
			else
			{
				Destination = sbs->GetFloor(GetBottomFloor())->Altitude + sbs->GetFloor(GetBottomFloor())->InterfloorHeight;
				if (ElevatorStart <= Destination)
				{
					//don't go below bottom floor
					Destination = 0;
					MoveElevator = false;
					ElevatorIsRunning = false;
					Direction = 0;
					return;
				}
			}
			DistanceToTravel = fabs(fabs(Destination) - fabs(ElevatorStart));
		}
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
		if (InspectionService == false)
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": moving " + dir_string + " to floor " + csString(_itoa(GotoFloor, intbuffer, 10)));
		else
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": moving " + dir_string);
		IsMoving = true;
		OnFloor = false;
	}

	if (EmergencyStop == true && Brakes == false)
	{
		//emergency stop
		CalculateStoppingDistance = false;
		TempDeceleration = Deceleration;
		if (Direction == 1)
			Direction = -1;
		else
			Direction = 1;
		Brakes = true;
		//stop sounds
		mainsound->Stop();
		//play stopping sound
		mainsound->Load(StopSound.GetData());
		mainsound->Loop(false);
		mainsound->Play();
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
	MoveDoors(0, csVector3(0, ElevatorRate * sbs->delta, 0), true, true, true);
	for (int i = 0; i < FloorIndicatorArray.GetSize(); i++)
	{
		if (FloorIndicatorArray[i])
			FloorIndicatorArray[i]->MovePosition(csVector3(0, ElevatorRate * sbs->delta, 0));
	}
	Plaque_movable->MovePosition(csVector3(0, ElevatorRate * sbs->delta, 0));
	Plaque_movable->UpdateMove();
	if (Panel)
		Panel->Move(csVector3(0, ElevatorRate * sbs->delta, 0));
	if (Panel2)
		Panel2->Move(csVector3(0, ElevatorRate * sbs->delta, 0));

	//move sounds
	mainsound->SetPosition(GetPosition());
	idlesound->SetPosition(GetPosition());
	MoveDoorSound(0, csVector3(0, GetPosition().y, 0), true, false, true);
	alarm->SetPosition(GetPosition());
	floorbeep->SetPosition(GetPosition());

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

	//limit speed to ElevatorSpeed value
	if (InspectionService == false)
	{
		if (Direction == 1 && ElevatorRate > ElevatorSpeed)
		{
			ElevatorRate = ElevatorSpeed;
			CalculateStoppingDistance = false;
		}
		if (Direction == -1 && ElevatorRate < -ElevatorSpeed)
		{
			ElevatorRate = -ElevatorSpeed;
			CalculateStoppingDistance = false;
		}
	}
	else
	{
		if (Direction == 1 && (ElevatorRate > ElevatorSpeed * 0.6))
		{
			ElevatorRate = ElevatorSpeed * 0.6;
			CalculateStoppingDistance = false;
		}
		if (Direction == -1 && (ElevatorRate < -ElevatorSpeed * 0.6))
		{
			ElevatorRate = -ElevatorSpeed * 0.6;
			CalculateStoppingDistance = false;
		}
	}

	//prevent the rate from going beyond 0
	if (Direction == 1 && Brakes == true && ElevatorRate > 0)
		ElevatorRate = 0;
	if (Direction == -1 && Brakes == true && ElevatorRate < 0)
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
	if (Brakes == false && Direction == 1)
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
			if (InspectionService == false)
				ElevatorRate -= ElevatorSpeed * ((TempDeceleration * JerkRate) * sbs->delta);
			else
				ElevatorRate -= (ElevatorSpeed * 0.6) * ((TempDeceleration * JerkRate) * sbs->delta);
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
			if (InspectionService == false)
				ElevatorRate += ElevatorSpeed * ((TempDeceleration * JerkRate) * sbs->delta);
			else
				ElevatorRate += (ElevatorSpeed * 0.6) * ((TempDeceleration * JerkRate) * sbs->delta);
			//stop sounds
			mainsound->Stop();
			//play stopping sound
			mainsound->Load(StopSound.GetData());
			mainsound->Loop(false);
			mainsound->Play();
		}
	}

	if (GetFloor() != oldfloor)
	{
		//play floor beep sound if not empty
		if (BeepSound != "" && IsServicedFloor(GetFloor()) == true)
		{
			floorbeep->Stop();
			floorbeep->Load(BeepSound);
			floorbeep->Loop(false);
			floorbeep->Play();
		}

		//update floor indicators
		if (sbs->ElevatorSync == true && sbs->ElevatorNumber == Number)
			UpdateFloorIndicators();
	}

	oldfloor = GetFloor();

	//exit if elevator's running
	if (fabs(ElevatorRate) != 0)
		return;

	//finish move
	if (EmergencyStop == false)
	{
		//play chime sound
		if (InServiceMode() == false)
		{
			if (GetFloor() == GetTopFloor())
				Chime(0, GotoFloor, false); //chime down if on top floor
			else if (GetFloor() == GetBottomFloor())
				Chime(0, GotoFloor, true); //chime up if on bottom floor
			else if (QueuePositionDirection == 1 || LastQueueDirection == 1)
				Chime(0, GotoFloor, true); //chime up if queue direction is or was up
			else if (QueuePositionDirection == -1 || LastQueueDirection == -1)
				Chime(0, GotoFloor, false); //chime down if queue direction is or was down
		}

		//store error offset value
		if (Direction == -1)
			ErrorOffset = GetPosition().y - Destination;
		else if (Direction == 1)
			ErrorOffset = Destination - GetPosition().y;
		else
			ErrorOffset = 0;

		//set elevator and objects to floor altitude (corrects offset errors)
		//move elevator objects
		Elevator_movable->SetPosition(csVector3(GetPosition().x, Destination, GetPosition().z));
		Elevator_movable->UpdateMove();
		if (sbs->ElevatorSync == true && sbs->ElevatorNumber == Number)
			sbs->camera->SetPosition(csVector3(sbs->camera->GetPosition().x, GetPosition().y + sbs->camera->cfg_legs_height + sbs->camera->cfg_body_height, sbs->camera->GetPosition().z));
		MoveDoors(0, csVector3(0, Destination, 0), true, false, true);
		for (int i = 0; i < FloorIndicatorArray.GetSize(); i++)
		{
			if (FloorIndicatorArray[i])
				FloorIndicatorArray[i]->SetPosition(csVector3(FloorIndicatorArray[i]->GetPosition().x, Destination, FloorIndicatorArray[i]->GetPosition().z));
		}
		Plaque_movable->SetPosition(csVector3(Plaque_movable->GetPosition().x, Destination, Plaque_movable->GetPosition().z));
		Plaque_movable->UpdateMove();
		if (Panel)
			Panel->SetToElevatorAltitude();
		if (Panel2)
			Panel2->SetToElevatorAltitude();

		//move sounds
		mainsound->SetPosition(GetPosition());
		alarm->SetPosition(GetPosition());

		//the elevator is now stopped on a valid floor; set OnFloor to true
		OnFloor = true;
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

	if (EmergencyStop == false && InspectionService == false)
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

		//change directional indicator and disable call button light
		if (InServiceMode() == false)
		{
			bool LightDirection = false; //true for up, false for down
			
			if (GetFloor() == GetTopFloor())
				LightDirection = false; //turn on down light if on top floor
			else if (GetFloor() == GetBottomFloor())
				LightDirection = true; //turn on up light if on bottom floor
			else if (QueuePositionDirection == 1 || LastQueueDirection == 1)
				LightDirection = true; //turn on up light if queue direction is or was up
			else if (QueuePositionDirection == -1 || LastQueueDirection == -1)
				LightDirection = false; //turn on down light if queue direction is or was down
			
			//change indicator
			SetDirectionalIndicator(GetFloor(), LightDirection, false);

			//disable call button lights
			SetCallButtons(GetFloor(), LightDirection, false);
		}

		//open doors
		//do not automatically open doors if in fire service phase 2
		if (FireServicePhase2 == 0)
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

void Elevator::AddFloorIndicator(const char *direction, float CenterX, float CenterZ, float width, float height, float voffset)
{
	//Creates a floor indicator at the specified location

	int size = FloorIndicatorArray.GetSize();
	FloorIndicatorArray.SetSize(size + 1);
	FloorIndicatorArray[size] = new FloorIndicator(Number, direction, CenterX, CenterZ, width, height, voffset);
	FloorIndicatorArray[size]->SetPosition(Origin);
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

	sbs->EnableMesh(ElevatorMesh, value);
	EnableDoors(value);
	IsEnabled = value;
}

void Elevator::EnableObjects(bool value)
{
	//enable or disable interior objects, such as floor indicators, button panel and plaque

	for (int i = 0; i < FloorIndicatorArray.GetSize(); i++)
	{
		if (FloorIndicatorArray[i])
			FloorIndicatorArray[i]->Enabled(value);
	}

	sbs->EnableMesh(Plaque, value);

	if (Panel)
		Panel->Enabled(value);
	if (Panel2)
		Panel2->Enabled(value);
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
	if (IsServicedFloor(number) == false)
		ServicedFloors.InsertSorted(number);
}

void Elevator::RemoveServicedFloor(int number)
{
	if (IsServicedFloor(number) == true)
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
		sbs->ReportError("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Button panels already exist");
}

void Elevator::UpdateFloorIndicators()
{
	//changes the number texture on the floor indicators to the elevator's current floor

	csString value;
	if (UseFloorSkipText == true && IsServicedFloor(GetFloor()) == false)
		value = FloorSkipText;
	else
		value = sbs->GetFloor(GetFloor())->ID;
	value.Trim();

	for (int i = 0; i < FloorIndicatorArray.GetSize(); i++)
	{
		if (FloorIndicatorArray[i])
			FloorIndicatorArray[i]->Update(value);
	}
}

float Elevator::GetJerkRate()
{
	return JerkRate;
}

float Elevator::GetJerkPosition()
{
	return JerkPos;
}

void Elevator::SetFloorSkipText(const char *id)
{
	//sets the text shown in the floor indicator while skipping floors (an express zone)
	UseFloorSkipText = true;
	FloorSkipText = id;
	FloorSkipText.Trim();
}

const char* Elevator::GetFloorSkipText()
{
	//get the floor skip text
	return FloorSkipText.GetData();
}

bool Elevator::IsServicedFloor(int floor)
{
	//returns true if floor is in serviced floor list, otherwise false
	if (ServicedFloors.Find(floor) == csArrayItemNotFound)
		return false;
	else
		return true;
}

bool Elevator::InServiceMode()
{
	//report if an elevator is in a service mode
	if (IndependentService == true || InspectionService == true || FireServicePhase1 != 0 || FireServicePhase2 != 0)
		return true;
	else
		return false;
}

void Elevator::Go(int floor)
{
	//go to specified floor, without using the queuing system

	//exit if in inspection mode
	if (InspectionService == true)
		return;

	GotoFloor = floor;
	MoveElevator = true;
}

void Elevator::GoPending(int floor)
{
	//go to specified floor, without using the queuing system and turn on pending value
	//(doors need to be manually closed to proceed)

	//exit if in inspection mode
	if (InspectionService == true)
		return;

	GotoFloor = floor;
	MovePending = true;
	sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": pending move to floor " + csString(_itoa(GotoFloor, intbuffer, 10)));
}

void Elevator::GoToRecallFloor()
{
	//for fire service modes; tells the elevator to go to the recall floor (or the alternate recall floor
	//if the other is not available)

	if (RecallUnavailable == false)
	{
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Proceeding to recall floor");
		Go(RecallFloor);
	}
	else
	{
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Proceeding to alternate recall floor");
		Go(RecallFloorAlternate);
	}
}

void Elevator::EnableACP(bool value)
{
	//enable Anti-Crime Protection (ACP) mode

	//exit if no change
	if (ACP == value)
		return;

	ACP = value;

	if (value == true)
	{
		EnableUpPeak(false);
		EnableDownPeak(false);
		EnableIndependentService(false);
		EnableInspectionService(false);
		EnableFireService1(0);
		EnableFireService2(0);
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": ACP mode enabled");
	}
	else
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": ACP mode disabled");

}

void Elevator::EnableUpPeak(bool value)
{
	//enable Up-Peak mode

	//exit if no change
	if (UpPeak == value)
		return;

	UpPeak = value;

	if (value == true)
	{
		EnableACP(false);
		EnableDownPeak(false);
		EnableIndependentService(false);
		EnableInspectionService(false);
		EnableFireService1(0);
		EnableFireService2(0);
		if (IsMoving == false && GetFloor() == GetBottomFloor())
			OpenDoors();
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Up Peak mode enabled");
	}
	else
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Up Peak mode disabled");
}

void Elevator::EnableDownPeak(bool value)
{
	//enable Down-Peak mode

	//exit if no change
	if (DownPeak == value)
		return;

	DownPeak = value;

	if (value == true)
	{
		EnableACP(false);
		EnableUpPeak(false);
		EnableIndependentService(false);
		EnableInspectionService(false);
		EnableFireService1(0);
		EnableFireService2(0);
		if (IsMoving == false && GetFloor() == GetTopFloor())
			OpenDoors();
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Down Peak mode enabled");
	}
	else
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Down Peak mode disabled");
}

void Elevator::EnableIndependentService(bool value)
{
	//enable Independent Service (ISC) mode

	//exit if no change
	if (IndependentService == value)
		return;

	IndependentService = value;

	if (value == true)
	{
		EnableACP(false);
		EnableUpPeak(false);
		EnableDownPeak(false);
		EnableInspectionService(false);
		EnableFireService1(0);
		EnableFireService2(0);
		ResetQueues = true;
		if (IsMoving == false)
			OpenDoors();
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Independent Service mode enabled");
	}
	else
	{
		ResetDoorTimer();
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Independent Service mode disabled");
	}
}

void Elevator::EnableInspectionService(bool value)
{
	//enable Inspection Service (INS) mode

	//exit if no change
	if (InspectionService == value)
		return;

	if (value == true)
	{
		EnableACP(false);
		EnableUpPeak(false);
		EnableDownPeak(false);
		EnableIndependentService(false);
		EnableFireService1(0);
		EnableFireService2(0);
		ResetQueues = true;
		if (IsMoving == true)
			StopElevator();
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Inspection Service mode enabled");
	}
	else
	{
		ResetDoorTimer();
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Inspection Service mode disabled");
	}

	InspectionService = value;
}

void Elevator::EnableFireService1(int value)
{
	//enable Fire Service Phase 1 mode
	//valid values are 0 (off), 1 (on) or 2 (bypass)

	//exit if no change
	if (FireServicePhase1 == value)
		return;

	if (value >= 0 && value <= 2)
		FireServicePhase1 = value;
	else
		return;

	if (value > 0)
	{
		EnableACP(false);
		EnableUpPeak(false);
		EnableDownPeak(false);
		EnableIndependentService(false);
		EnableInspectionService(false);
		EnableFireService2(0);
		ResetQueues = true;
		if (value == 1)
		{
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Fire Service Phase 1 mode set to On");
			GoToRecallFloor();
		}
		else
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Fire Service Phase 1 mode set to Bypass");
	}
	else
	{
		ResetDoorTimer();
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Fire Service Phase 1 mode set to Off");
	}
}

void Elevator::EnableFireService2(int value)
{
	//enable Fire Service Phase 2 mode
	//valid values are 0 (off), 1 (on) or 2 (hold)

	//exit if no change
	if (FireServicePhase2 == value)
		return;

	if (value >= 0 && value <= 2)
		FireServicePhase2 = value;
	else
		return;

	if (value > 0)
	{
		EnableACP(false);
		EnableUpPeak(false);
		EnableDownPeak(false);
		EnableIndependentService(false);
		EnableInspectionService(false);
		EnableFireService1(0);
		ResetQueues = true;
		if (value == 1)
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Fire Service Phase 2 mode set to On");
		else
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Fire Service Phase 2 mode set to Hold");
	}
	else
	{
		ResetDoorTimer();
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Fire Service Phase 2 mode set to Off");
		GoToRecallFloor();
	}
}

bool Elevator::SetRecallFloor(int floor)
{
	//set elevator's fire recall floor
	if (ServicedFloors.GetSize() == 0)
	{
		sbs->ReportError("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": No serviced floors assigned");
		return false;
	}
	if (IsServicedFloor(floor) == false)
	{
		sbs->ReportError("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Invalid recall floor");
		return false;
	}
	RecallFloor = floor;
	RecallSet = true;
	return true;
}

bool Elevator::SetAlternateRecallFloor(int floor)
{
	//set elevator's alternate fire recall floor
	if (ServicedFloors.GetSize() == 0)
	{
		sbs->ReportError("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": No serviced floors assigned");
		return false;
	}
	if (IsServicedFloor(floor) == false)
	{
		sbs->ReportError("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Invalid alternate recall floor");
		return false;
	}
	RecallFloorAlternate = floor;
	RecallAltSet = true;
	return true;
}

bool Elevator::SetACPFloor(int floor)
{
	//set elevator's ACP floor
	if (ServicedFloors.GetSize() == 0)
	{
		sbs->ReportError("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": No serviced floors assigned");
		return false;
	}
	if (IsServicedFloor(floor) == false)
	{
		sbs->ReportError("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Invalid ACP floor");
		return false;
	}
	ACPFloor = floor;
	ACPFloorSet = true;
	return true;
}

bool Elevator::MoveUp()
{
	//moves elevator upwards if in Inspection Service mode
	if (InspectionService == false)
	{
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Not in inspection service mode");
		return false;
	}

	//make sure Go button is on
	if (ManualGo == false)
		return false;

	if (IsMoving == true)
		return false;

	//set direction
	Direction = 1;
	MoveElevator = true;
	return true;
}

bool Elevator::MoveDown()
{
	//moves elevator downwards if in Inspection Service mode
	if (InspectionService == false)
	{
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Not in inspection service mode");
		return false;
	}

	//make sure Go button is on
	if (ManualGo == false)
		return false;

	if (IsMoving == true)
		return false;

	//set direction
	Direction = -1;
	MoveElevator = true;
	return true;
}

bool Elevator::StopMove()
{
	//stops elevator movement if in Inspection Service mode
	//this is almost identical to the StopElevator() function, except it reports differently. This is also to
	//separate it from the regular Stop button

	if (InspectionService == false)
	{
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Not in inspection service mode");
		return false;
	}

	if (IsMoving == false)
		return false;

	EmergencyStop = true;
	sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Stopping elevator");
	return true;
}

void Elevator::SetGoButton(bool value)
{
	//sets the value of the Go button (for Inspection Service mode)

	if (ManualGo == true && value == false)
		StopMove();

	ManualGo = value;
}

int Elevator::GetTopFloor()
{
	//returns highest serviced floor
	return ServicedFloors[ServicedFloors.GetSize() - 1];
}

int Elevator::GetBottomFloor()
{
	//returns lowest serviced floor
	return ServicedFloors[0];
}

void Elevator::AddDirectionalIndicators(bool relative, bool single, bool vertical, const char *BackTexture, const char *uptexture, const char *uptexture_lit, const char *downtexture, const char *downtexture_lit, float CenterX, float CenterZ, float voffset, const char *direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th)
{
	//create all directional indicators

	float x, z;
	if (relative == true)
	{
		x = Origin.x + CenterX;
		z = Origin.z + CenterZ;
	}
	else
	{
		x = CenterX;
		z = CenterZ;
	}
	for (size_t i = 0; i < ServicedFloors.GetSize(); i++)
		IndicatorArray[i] = new DirectionalIndicator(Number, ServicedFloors[i], single, vertical, BackTexture, uptexture, uptexture_lit, downtexture, downtexture_lit, x, z, voffset, direction, BackWidth, BackHeight, ShowBack, tw, th);
}

bool Elevator::AddDirectionalIndicator(int floor, bool relative, bool single, bool vertical, const char *BackTexture, const char *uptexture, const char *uptexture_lit, const char *downtexture, const char *downtexture_lit, float CenterX, float CenterZ, float voffset, const char *direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th)
{
	//create a directional indicator on a single floor

	float x, z;
	if (relative == true)
	{
		x = Origin.x + CenterX;
		z = Origin.z + CenterZ;
	}
	else
	{
		x = CenterX;
		z = CenterZ;
	}

	int index = ServicedFloors.Find(floor);
	if (index != -1)
		IndicatorArray[index] = new DirectionalIndicator(Number, floor, single, vertical, BackTexture, uptexture, uptexture_lit, downtexture, downtexture_lit, x, z, voffset, direction, BackWidth, BackHeight, ShowBack, tw, th);
	else
		return false;
	return true;
}

void Elevator::EnableDirectionalIndicator(int floor, bool value)
{
	//enable/disable the specified directional indicator

	int index = ServicedFloors.Find(floor);

	if (index == -1)
		return;

	if (IndicatorArray[index])
		IndicatorArray[index]->Enabled(value);
}

void Elevator::SetDirectionalIndicator(int floor, bool UpLight, bool DownLight)
{
	//set light status of directional indicator

	int index = ServicedFloors.Find(floor);

	if (index == -1)
		return;

	if (IndicatorArray[index])
	{
		IndicatorArray[index]->DownLight(DownLight);
		IndicatorArray[index]->UpLight(UpLight);
	}
}

void Elevator::EnableDirectionalIndicators(bool value)
{
	//turn on/off all directional indicators
	for (size_t i = 0; i < ServicedFloors.GetSize(); i++)
	{
		if (IndicatorArray[i])
			IndicatorArray[i]->Enabled(value);
	}
}

ElevatorDoor* Elevator::GetDoor(int number)
{
	//get elevator door object

	if (number > 0 && number <= DoorArray.GetSize())
	{
		if (DoorArray[number - 1])
			return DoorArray[number - 1];
	}
	return 0;
}

void Elevator::OpenDoorsEmergency(int number, int whichdoors, int floor)
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

void Elevator::CloseDoorsEmergency(int number, int whichdoors, int floor)
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

void Elevator::OpenDoors(int number, int whichdoors, int floor, bool manual)
{
	//Opens elevator doors

	//if manual is true, then it simulates manually prying doors open,
	//Slowly opens the elevator doors no matter where elevator is,
	//and if lined up with shaft doors, then opens the shaft doors also

	//WhichDoors is the doors to move:
	//1 = both shaft and elevator doors
	//2 = only elevator doors
	//3 = only shaft doors

	int start, end;
	if (number == 0)
	{
		start = 1;
		end = NumDoors;
	}
	else
	{
		start = number;
		end = number;
	}
	for (int i = start; i <= end; i++)
	{
		if (GetDoor(i))
			GetDoor(i)->OpenDoors(whichdoors, floor, manual);
		else
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Invalid door " + csString(_itoa(i, intbuffer, 10)));
	}
}

void Elevator::CloseDoors(int number, int whichdoors, int floor, bool manual)
{
	//Closes elevator doors

	//WhichDoors is the doors to move:
	//1 = both shaft and elevator doors
	//2 = only elevator doors
	//3 = only shaft doors

	int start, end;
	if (number == 0)
	{
		start = 1;
		end = NumDoors;
	}
	else
	{
		start = number;
		end = number;
	}
	for (int i = start; i <= end; i++)
	{
		if (GetDoor(i))
			GetDoor(i)->CloseDoors(whichdoors, floor, manual);
		else
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Invalid door " + csString(_itoa(i, intbuffer, 10)));
	}
}

void Elevator::StopDoors(int number)
{
	//stops doors that are currently moving; can only be used for manual/emergency movements
	//this basically just resets the door internals

	int start, end;
	if (number == 0)
	{
		start = 1;
		end = NumDoors;
	}
	else
	{
		start = number;
		end = number;
	}
	for (int i = start; i <= end; i++)
	{
		if (GetDoor(i))
			GetDoor(i)->StopDoors();
		else
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Invalid door " + csString(_itoa(i, intbuffer, 10)));
	}
}

int Elevator::AddDoors(int number, const char *lefttexture, const char *righttexture, float thickness, float CenterX, float CenterZ, float width, float height, bool direction, float tw, float th)
{
	//adds elevator doors specified at a relative central position (off of elevator origin)
	//if direction is false, doors are on the left/right side; otherwise front/back

	if (GetDoor(number))
		return GetDoor(number)->AddDoors(lefttexture, righttexture, thickness, CenterX, CenterZ, width, height, direction, tw, th);
	else
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Invalid door " + csString(_itoa(number, intbuffer, 10)));
	return 0;
}

int Elevator::AddShaftDoors(int number, const char *lefttexture, const char *righttexture, float thickness, float CenterX, float CenterZ, float tw, float th)
{
	//adds shaft's elevator doors specified at a relative central position (off of elevator origin)
	//uses some parameters (width, height, direction) from AddDoors function

	if (GetDoor(number))
		return GetDoor(number)->AddShaftDoors(lefttexture, righttexture, thickness, CenterX, CenterZ, tw, th);
	else
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Invalid door " + csString(_itoa(number, intbuffer, 10)));
	return 0;
}

bool Elevator::AddShaftDoor(int floor, int number, const char *lefttexture, const char *righttexture, float tw, float th)
{
	//adds a single elevator shaft door on the specified floor, with position and thickness parameters first specified
	//by the SetShaftDoors command.

	int index = ServicedFloors.Find(floor);
	if (index != -1 && GetDoor(number))
		return GetDoor(number)->AddShaftDoor(floor, lefttexture, righttexture, tw, th);
	else
		return false;
	return true;
}

void Elevator::ShaftDoorsEnabled(int number, int floor, bool value)
{
	//turns shaft elevator doors on/off

	int start, end;
	if (number == 0)
	{
		start = 1;
		end = NumDoors;
	}
	else
	{
		start = number;
		end = number;
	}
	for (int i = start; i <= end; i++)
	{
		if (GetDoor(i))
			GetDoor(i)->ShaftDoorsEnabled(floor, value);
		else
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Invalid door " + csString(_itoa(i, intbuffer, 10)));
	}
}

void Elevator::ShaftDoorsEnabledRange(int number, int floor, int range)
{
	//turn on a range of floors
	//if range is 3, show shaft door on current floor (floor), and 1 floor below and above (3 total floors)
	//if range is 1, show door on only the current floor (floor)

	int start, end;
	if (number == 0)
	{
		start = 1;
		end = NumDoors;
	}
	else
	{
		start = number;
		end = number;
	}
	for (int i = start; i <= end; i++)
	{
		if (GetDoor(i))
			GetDoor(i)->ShaftDoorsEnabledRange(floor, range);
		else
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Invalid door " + csString(_itoa(i, intbuffer, 10)));
	}
}

bool Elevator::AreDoorsOpen(int number)
{
	//returns the internal door state

	int start, end;
	if (number == 0)
	{
		start = 1;
		end = NumDoors;
	}
	else
	{
		start = number;
		end = number;
	}
	for (int i = start; i <= end; i++)
	{
		if (GetDoor(i))
		{
			if (GetDoor(i)->AreDoorsOpen() == true)
				return true;
		}
		else
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Invalid door " + csString(_itoa(i, intbuffer, 10)));
	}
	return false;
}

bool Elevator::AreShaftDoorsOpen(int number, int floor)
{
	//returns the internal shaft door state

	if (GetDoor(number))
		return GetDoor(number)->AreShaftDoorsOpen(floor);
	else
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Invalid door " + csString(_itoa(number, intbuffer, 10)));
	return false;
}

float Elevator::GetCurrentDoorSpeed(int number)
{
	//returns the internal door speed value

	if (GetDoor(number))
		return GetDoor(number)->GetCurrentDoorSpeed();
	else
		sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Invalid door " + csString(_itoa(number, intbuffer, 10)));
	return 0;
}

void Elevator::Chime(int number, int floor, bool direction)
{
	//play chime sound on specified floor

	int start, end;
	if (number == 0)
	{
		start = 1;
		end = NumDoors;
	}
	else
	{
		start = number;
		end = number;
	}
	for (int i = start; i <= end; i++)
	{
		if (GetDoor(i))
			GetDoor(i)->Chime(floor, direction);
		else
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Invalid door " + csString(_itoa(i, intbuffer, 10)));
	}
}

void Elevator::ResetDoorTimer(int number)
{
	//reset elevator door timer

	int start, end;
	if (number == 0)
	{
		start = 1;
		end = NumDoors;
	}
	else
	{
		start = number;
		end = number;
	}
	for (int i = start; i <= end; i++)
	{
		if (GetDoor(i))
			GetDoor(i)->ResetDoorTimer();
		else
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Invalid door " + csString(_itoa(i, intbuffer, 10)));
	}
}

bool Elevator::DoorsStopped(int number)
{
	int start, end;
	if (number == 0)
	{
		start = 1;
		end = NumDoors;
	}
	else
	{
		start = number;
		end = number;
	}
	for (int i = start; i <= end; i++)
	{
		if (GetDoor(i))
			return GetDoor(i)->DoorsStopped();
		else
			sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": Invalid door " + csString(_itoa(i, intbuffer, 10)));
	}
	return false;
}

bool Elevator::CheckOpenDoor()
{
	//check each door's OpenDoor value and return true if any are not zero, or
	//false if all are zero

	for (int i = 1; i <= NumDoors; i++)
	{
		if (GetDoor(i))
		{
			if (GetDoor(i)->OpenDoor != 0)
				return true;
		}
	}
	return false;
}

void Elevator::MoveDoors(int number, const csVector3 position, bool relative_x, bool relative_y, bool relative_z)
{
	//move all doors

	if (number == 0)
	{
		for (int i = 1; i <= NumDoors; i++)
		{
			if (GetDoor(i))
				GetDoor(i)->Move(position, relative_x, relative_y, relative_z);
		}
	}
	else
	{
		if (GetDoor(number))
			GetDoor(number)->Move(position, relative_x, relative_y, relative_z);
	}
}

void Elevator::EnableDoors(bool value)
{
	//move all doors

	for (int i = 1; i <= NumDoors; i++)
	{
		if (GetDoor(i))
			GetDoor(i)->Enabled(value);
	}
}

DirectionalIndicator* Elevator::GetIndicator(int floor)
{
	//get a directional indicator for a floor
	int index = ServicedFloors.Find(floor);
	if (IndicatorArray[index])
		return IndicatorArray[index];
	else
		return 0;
}

void Elevator::MoveDoorSound(int number, const csVector3 position, bool relative_x, bool relative_y, bool relative_z)
{
	//move all doors

	if (number == 0)
	{
		for (int i = 1; i <= NumDoors; i++)
		{
			if (GetDoor(i))
				GetDoor(i)->MoveSound(position, relative_x, relative_y, relative_z);
		}
	}
	else
	{
		if (GetDoor(number))
			GetDoor(number)->MoveSound(position, relative_x, relative_y, relative_z);
	}
}

void Elevator::SetShaftDoors(int number, float thickness, float CenterX, float CenterZ)
{
	if (number == 0)
	{
		for (int i = 1; i <= NumDoors; i++)
		{
			if (GetDoor(i))
				GetDoor(i)->SetShaftDoors(thickness, CenterX, CenterZ);
		}
	}
	else
	{
		if (GetDoor(number))
			GetDoor(number)->SetShaftDoors(thickness, CenterX, CenterZ);
	}
}

void Elevator::AddFloorSigns(bool relative, const char *direction, float CenterX, float CenterZ, float width, float height, float voffset)
{
	//adds floor signs at the specified position and direction for each serviced floor

	float x, z;
	if (relative == true)
	{
		x = Origin.x + CenterX;
		z = Origin.z + CenterZ;
	}
	else
	{
		x = CenterX;
		z = CenterZ;
	}

	csVector2 autosize = sbs->GetAutoSize();
	sbs->SetAutoSize(false, false);

	for (int i = 0; i < ServicedFloors.GetSize(); i++)
	{
		csString texture = "Button" + sbs->GetFloor(ServicedFloors[i])->ID;
		csString tmpdirection = direction;
		tmpdirection.Downcase();

		if (tmpdirection == "front" || tmpdirection == "left")
			sbs->DrawWalls(true, false, false, false, false, false);
		else
			sbs->DrawWalls(false, true, false, false, false, false);

		if (tmpdirection == "front" || tmpdirection == "back")
			sbs->GetFloor(ServicedFloors[i])->AddWall("Floor Sign", texture, 0, x - (width / 2), z, x + (width / 2), z, height, height, voffset, voffset, 1, 1, false);
		else
			sbs->GetFloor(ServicedFloors[i])->AddWall("Floor Sign", texture, 0, x, z - (width / 2), x, z + (width / 2), height, height, voffset, voffset, 1, 1, false);
		sbs->ResetWalls();
	}
	sbs->SetAutoSize(autosize.x, autosize.y);
}

void Elevator::SetCallButtons(int floor, bool direction, bool value)
{
	//sets light status of all associated call buttons on the specified floor
	//for direction, true is up and false is down

	//get call buttons associated with this elevator
	csArray<int> buttons = sbs->GetFloor(GetFloor())->GetCallButtons(Number);

	for (int i = 0; i < buttons.GetSize(); i++)
	{
		CallButton *button = sbs->GetFloor(floor)->CallButtonArray[i];
		if (button)
		{
			if (direction == true)
				button->UpLight(value);
			else
				button->DownLight(value);
		}
	}

}
