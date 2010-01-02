/* $Id$ */

/*
	Scalable Building Simulator - Elevator Subsystem Class
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
#include "sbs.h"
#include "elevator.h"
#include "camera.h"
#include "shaft.h"
#include "unix.h"

#include <iengine/movable.h>

extern SBS *sbs; //external pointer to the SBS engine

Elevator::Elevator(int number)
{
	//set up SBS object
	object = new Object();
	object->SetValues(this, sbs->object, "Elevator", false);

	csString buffer;

	//set elevator number
	Number = number;

	//init variables
	Name = "";
	QueuePositionDirection = 0;
	LastQueueDirection = 0;
	LastQueueFloor[0] = 0;
	LastQueueFloor[1] = 0;
	ElevatorSpeed = 0;
	MoveElevator = false;
	GotoFloor = 0;
	Acceleration = 0;
	Deceleration = 0;
	AccelJerk = sbs->confman->GetFloat("Skyscraper.SBS.Elevator.AccelJerk", 1);
	DecelJerk = sbs->confman->GetFloat("Skyscraper.SBS.Elevator.DecelJerk", 1);
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
	CarStartSound = sbs->confman->GetStr("Skyscraper.SBS.Elevator.CarStartSound", "");
	CarMoveSound = sbs->confman->GetStr("Skyscraper.SBS.Elevator.CarMoveSound", "");
	CarStopSound = sbs->confman->GetStr("Skyscraper.SBS.Elevator.CarStopSound", "");
	CarIdleSound = sbs->confman->GetStr("Skyscraper.SBS.Elevator.CarIdleSound", "elevidle.wav");
	MotorStartSound = sbs->confman->GetStr("Skyscraper.SBS.Elevator.MotorStartSound", "motor_start.wav");
	MotorRunSound = sbs->confman->GetStr("Skyscraper.SBS.Elevator.MotorRunSound", "motor_running.wav");
	MotorStopSound = sbs->confman->GetStr("Skyscraper.SBS.Elevator.MotorStopSound", "motor_stop.wav");
	MotorIdleSound = sbs->confman->GetStr("Skyscraper.SBS.Elevator.MotorIdleSound", "");
	AlarmSound = sbs->confman->GetStr("Skyscraper.SBS.Elevator.AlarmSound", "bell1.wav");
	AlarmSoundStop = sbs->confman->GetStr("Skyscraper.SBS.Elevator.AlarmSoundStop", "bell1-stop.wav");
	UseFloorSkipText = false;
	ACP = sbs->confman->GetBool("Skyscraper.SBS.Elevator.ACP", false);
	ACPFloor = sbs->confman->GetInt("Skyscraper.SBS.Elevator.ACPFloor", 0);
	UpPeak = sbs->confman->GetBool("Skyscraper.SBS.Elevator.UpPeak", false);
	DownPeak = sbs->confman->GetBool("Skyscraper.SBS.Elevator.DownPeak", false);
	IndependentService = sbs->confman->GetBool("Skyscraper.SBS.Elevator.IndependentService", false);
	InspectionService = sbs->confman->GetBool("Skyscraper.SBS.Elevator.InspectionService", false);
	FireServicePhase1 = 0;
	FireServicePhase2 = 0;
	RecallFloor = 0;
	RecallFloorAlternate = 0;
	ResetUpQueue = false;
	ResetDownQueue = false;
	OnFloor = true;
	RecallSet = false;
	RecallAltSet = false;
	ACPFloorSet = false;
	RecallUnavailable = false;
	ManualGo = false;
	AlarmActive = false;
	NumDoors = 1;
	Created = false;
	lastcheckresult = false;
	checkfirstrun = true;
	UseFloorBeeps = false;
	UseFloorSounds = false;
	MotorPosition = csVector3(0, 0, 0);
	ActiveCallFloor = 0;
	ActiveCallDirection = 0;
	lastdoor_result = 0;
	lastdoor_number = 0;
	QueueResets = sbs->confman->GetBool("Skyscraper.SBS.Elevator.QueueResets", false);
	FirstRun = true;

	//create object meshes
	buffer = Number;
	buffer.Insert(0, "Elevator ");
	buffer.Trim();
	ElevatorMesh = sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData());
	Elevator_state = scfQueryInterface<iThingFactoryState> (ElevatorMesh->GetMeshObject()->GetFactory());
	Elevator_movable = ElevatorMesh->GetMovable();
	ElevatorMesh->SetZBufMode(CS_ZBUF_USE);
	ElevatorMesh->SetRenderPriority(sbs->engine->GetObjectRenderPriority());

	if (sbs->Verbose)
		Report("elevator object created");
}

Elevator::~Elevator()
{
	//delete directional indicators
	if (sbs->Verbose)
		Report("deleting directional indicators");

	for (int i = 0; i < IndicatorArray.GetSize(); i++)
	{
		if (IndicatorArray[i])
			delete IndicatorArray[i];
	}
	IndicatorArray.DeleteAll();

	//delete doors
	if (sbs->Verbose)
		Report("deleting doors");

	if (DoorArray.GetSize() > 0)
	{
		for (int i = 0; i < DoorArray.GetSize(); i++)
		{
			if (DoorArray[i])
				delete DoorArray[i];
		}
	}

	//delete floor indicators
	if (sbs->Verbose)
		Report("deleting floor indicators");

	for (int i = 0; i < FloorIndicatorArray.GetSize(); i++)
	{
		if (FloorIndicatorArray[i])
			delete FloorIndicatorArray[i];
	}
	FloorIndicatorArray.DeleteAll();

	//Destructor
	if (sbs->Verbose)
		Report("deleting objects");
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
	if (floorsound)
		delete floorsound;
	floorsound = 0;
	if (motorsound)
		delete motorsound;
	motorsound = 0;
	if (idlesound)
		delete idlesound;
	idlesound = 0;

	//delete sounds
	if (sbs->Verbose)
		Report("deleting sounds");

	for (int i = 0; i < sounds.GetSize(); i++)
	{
		if (sounds[i])
			delete sounds[i];
		sounds[i] = 0;
	}
	sounds.DeleteAll();

	//delete polygon objects
	for (int i = 0; i < elevator_walls.GetSize(); i++)
	{
		if (elevator_walls[i])
			delete elevator_walls[i];
		elevator_walls[i] = 0;
	}

	Elevator_movable = 0;
	Elevator_state = 0;
	ElevatorMesh = 0;
	delete object;
}

bool Elevator::CreateElevator(bool relative, float x, float z, int floor)
{
	//Creates elevator at specified location and floor
	//x and z are the center coordinates
	//if relative is true, then x and z coordinates are relative
	//to the assigned shaft's center

	if (Created == true)
	{
		ReportError("Has already been created");
		return false;
	}

	//make sure required values are set
	if (ElevatorSpeed <= 0)
	{
		ReportError("Speed not set or invalid");
		return false;
	}
	if (Acceleration <= 0)
	{
		ReportError("Acceleration not set or invalid");
		return false;
	}
	if (Deceleration <= 0)
	{
		ReportError("Deceleration not set or invalid");
		return false;
	}
	if (NumDoors < 0)
	{
		ReportError("Number of doors invalid");
		return false;
	}
	if (AccelJerk <= 0)
	{
		ReportError("Invalid value for AccelJerk");
		return false;
	}
	if (DecelJerk <= 0)
	{
		ReportError("Invalid value for DecelJerk");
		return false;
	}
	if (AssignedShaft <= 0)
	{
		ReportError("Not assigned to a shaft");
		return false;
	}
	if (floor < sbs->GetShaft(AssignedShaft)->startfloor || floor > sbs->GetShaft(AssignedShaft)->endfloor)
	{
		ReportError("Invalid starting floor");
		return false;
	}

	//add elevator's starting floor to serviced floor list - this also ensures that the list is populated to prevent errors
	if (IsServicedFloor(floor) == false)
		AddServicedFloor(floor);

	//set data
	Origin.y = sbs->GetFloor(floor)->GetBase();
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
	sbs->GetShaft(AssignedShaft)->AddElevator(Number);

	//set recall/ACP floors if not already set
	if (RecallSet == false)
		SetRecallFloor(GetBottomFloor());
	if (RecallAltSet == false)
		SetAlternateRecallFloor(GetTopFloor());
	if (ACPFloorSet == false)
		SetACPFloor(GetBottomFloor());

	//create door objects
	if (sbs->Verbose)
		Report("creating doors");
	if (NumDoors > 0)
	{
		DoorArray.SetSize(NumDoors);
		for (int i = 0; i < NumDoors; i++)
			DoorArray[i] = new ElevatorDoor(i, this);
	}

	//move objects to positions
	if (sbs->Verbose)
		Report("moving elevator to origin position");
	Elevator_movable->SetPosition(sbs->ToRemote(Origin));
	Elevator_movable->UpdateMove();

	//resize directional indicator array
	IndicatorArray.SetSize(ServicedFloors.GetSize());

	//create sound objects
	if (sbs->Verbose)
		Report("creating sound objects");
	mainsound = new Sound(this->object, "Main");
	mainsound->SetPosition(Origin);
	idlesound = new Sound(this->object, "Idle");
	idlesound->SetPosition(Origin);
	idlesound->Load(CarIdleSound.GetData());
	motorsound = new Sound(this->object, "Motor");
	motorsound->SetPosition(Origin);
	//move motor to top of shaft if location not specified, or to location
	if (MotorPosition != csVector3(0, 0, 0))
		motorsound->SetPosition(csVector3(MotorPosition.x + Origin.x, MotorPosition.y, MotorPosition.z + Origin.z));
	else
		motorsound->SetPositionY(sbs->GetFloor(sbs->GetShaft(AssignedShaft)->endfloor)->GetBase());
	MotorPosition = csVector3(motorsound->GetPosition().x - Origin.x, motorsound->GetPosition().y, motorsound->GetPosition().z - Origin.z);
	alarm = new Sound(this->object, "Alarm");
	alarm->SetPosition(Origin);
	floorbeep = new Sound(this->object, "Floor Beep");
	floorbeep->SetPosition(Origin);
	floorsound = new Sound(this->object, "Floor Sound");
	floorsound->SetPosition(Origin);

	//set elevator's floor
	ElevatorFloor = floor;

	Created = true;

	Report("created at " + csString(_gcvt(x, 12, buffer)) + ", " + csString(_gcvt(z, 12, buffer)) + ", " + csString(_itoa(floor, buffer, 12)));
	return true;
}

void Elevator::AddRoute(int floor, int direction, bool change_light)
{
	//Add call route to elevator routing table, in sorted order
	//directions are either 1 for up, or -1 for down

	//if doors are open or moving in independent service mode, quit
	if (IndependentService == true && (AreDoorsOpen() == false || CheckOpenDoor() == true))
	{
		Report("floor button must be pressed before closing doors while in independent service");
		return;
	}

	//do not add routes if in inspection service or fire phase 1 modes
	if (InspectionService == true)
	{
		Report("cannot add route while in inspection service mode");
		return;
	}
	if (FireServicePhase1 == 1)
	{
		Report("cannot add route while in fire service phase 1 mode");
		return;
	}
	if (FireServicePhase2 == 2)
	{
		Report("cannot add route while in held state");
		return;
	}

	if (direction == 1)
	{
		if (UpQueue.Find(floor) != csArrayItemNotFound)
		{
			//exit if entry already exits
			Report("route to floor " + csString(_itoa(floor, intbuffer, 10)) + " (" + sbs->GetFloor(floor)->ID + ") already exists");
			return;
		}
		UpQueue.InsertSorted(floor);
		LastQueueFloor[0] = floor;
		LastQueueFloor[1] = 1;
		Report("adding route to floor " + csString(_itoa(floor, intbuffer, 10)) + " (" + sbs->GetFloor(floor)->ID + ") direction up");
	}
	else
	{
		if (DownQueue.Find(floor) != csArrayItemNotFound)
		{
			//exit if entry already exits
			Report("route to floor " + csString(_itoa(floor, intbuffer, 10)) + " (" + sbs->GetFloor(floor)->ID + ") already exists");
			return;
		}
		DownQueue.InsertSorted(floor);
		LastQueueFloor[0] = floor;
		LastQueueFloor[1] = -1;
		Report("adding route to floor " + csString(_itoa(floor, intbuffer, 10)) + " (" + sbs->GetFloor(floor)->ID + ") direction down");
	}

	//turn on button lights
	if (change_light == true)
	{
		if (sbs->Verbose)
			Report("AddRoute: turning on button lights for floor " + csString(_itoa(floor, intbuffer, 10)));
		if (Panel)
			Panel->ChangeLight(floor, true);
		if (Panel2)
			Panel2->ChangeLight(floor, true);
	}

	//add ACP route recursively if mode is enabled
	if (ACP == true && floor != ACPFloor)
	{
		//only add ACP route if original route will pass ACP floor
		if ((GetFloor() < ACPFloor && floor > ACPFloor) || (GetFloor() > ACPFloor && floor < ACPFloor))
		{
			if (sbs->Verbose)
				Report("Adding ACP route");
			AddRoute(ACPFloor, direction, false);
		}
	}
}

void Elevator::DeleteRoute(int floor, int direction)
{
	//Delete call route from elevator routing table
	//directions are either 1 for up, or -1 for down

	if (direction == 1)
	{
		//delete floor entry from up queue
		UpQueue.Delete(floor);
		Report("deleting route to floor " + csString(_itoa(floor, intbuffer, 10)) + " (" + sbs->GetFloor(floor)->ID + ") direction up");
	}
	else
	{
		//delete floor entry from down queue
		DownQueue.Delete(floor);
		Report("deleting route to floor " + csString(_itoa(floor, intbuffer, 10)) + " (" + sbs->GetFloor(floor)->ID + ") direction down");
	}

	//turn off button lights
	if (sbs->Verbose)
		Report("DeleteRoute: turning off button lights for floor " + csString(_itoa(floor, intbuffer, 10)));
	if (Panel)
		Panel->ChangeLight(floor, false);
	if (Panel2)
		Panel2->ChangeLight(floor, false);
}

void Elevator::CancelLastRoute()
{
	//cancels the last added route

	//LastQueueFloor holds the floor and direction of the last route; array element 0 is the floor and 1 is the direction

	if (LastQueueFloor[1] == 0)
	{
		if (sbs->Verbose)
			Report("CancelLastRoute: route not valid");
		return;
	}

	Report("cancelling last route");
	DeleteRoute(LastQueueFloor[0], LastQueueFloor[1]);
	LastQueueFloor[0] = 0;
	LastQueueFloor[1] = 0;
}

void Elevator::Alarm()
{
	//elevator alarm code

	if (AlarmActive == false)
	{
		//ring alarm
		AlarmActive = true;
		Report("alarm on");
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
		Report("alarm off");
	}
}

void Elevator::StopElevator()
{
	//Tells elevator to stop moving, no matter where it is

	//exit if in inspection mode
	if (InspectionService == true)
	{
		if (sbs->Verbose)
			Report("cannot stop while in inspection service");
		return;
	}

	Report("emergency stop");

	EmergencyStop = true;

	//clear elevator queues
	QueueReset();
}

void Elevator::OpenHatch()
{
	//Opens the elevator's upper escape hatch, allowing access to the shaft

	Report("opening hatch");
}

void Elevator::ProcessCallQueue()
{
	//Processes the elevator's call queue, and sends elevators to called floors

	//if queue reset is requested, clear requested queue, open doors, and pause queue
	if (ResetUpQueue == true || ResetDownQueue == true)
	{
		if (ResetUpQueue == true)
		{
			if (sbs->Verbose)
				Report("ProcessCallQueue: resetting up queue");
			UpQueue.DeleteAll();
		}
		if (ResetDownQueue == true)
		{
			if (sbs->Verbose)
				Report("ProcessCallQueue: resetting down queue");
			DownQueue.DeleteAll();
		}
		ResetUpQueue = false;
		ResetDownQueue = false;

		//turn off button lights
		if (sbs->Verbose)
			Report("ProcessCallQueue: turning off button lights for queue reset");
		for (int i = 0; i < ServicedFloors.GetSize(); i++)
		{
			if (Panel)
				Panel->ChangeLight(ServicedFloors[i], false);
			if (Panel2)
				Panel2->ChangeLight(ServicedFloors[i], false);
		}
		return;
	}

	//exit if in inspection service or fire phase 1 modes
	if (InspectionService == true || FireServicePhase1 == 1)
		return;

	//if both queues are empty
	if (UpQueue.GetSize() == 0 && DownQueue.GetSize() == 0)
	{
		int TopFloor = GetTopFloor();
		int BottomFloor = GetBottomFloor();

		if (DownPeak == true || UpPeak == true)
		{
			//if DownPeak mode is active, send elevator to the top serviced floor if not already there
			if (GetFloor() != TopFloor && DownPeak == true && IsMoving == false)
			{
				if (sbs->Verbose)
					Report("ProcessCallQueue: sending elevator to top floor for DownPeak mode");
				AddRoute(TopFloor, 1, false);
				return;
			}
			//if UpPeak mode is active, send elevator to the bottom serviced floor if not already there
			else if (GetFloor() != BottomFloor && UpPeak == true && IsMoving == false)
			{
				if (sbs->Verbose)
					Report("ProcessCallQueue: sending elevator to bottom floor for UpPeak mode");
				AddRoute(BottomFloor, -1, false);
				return;
			}
		}

		if (IsIdle() == true && QueuePositionDirection != 0)
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
		LastQueueDirection = 0;

		if (UpQueue.GetSize() != 0)
		{
			if (sbs->Verbose)
				Report("ProcessCallQueue: setting search direction to up");
			QueuePositionDirection = 1;
		}
		if (DownQueue.GetSize() != 0)
		{
			if (sbs->Verbose)
				Report("ProcessCallQueue: setting search direction to down");
			QueuePositionDirection = -1;
		}
	}

	//set search direction to 0 if any related queue is empty, and if doors are not open or moving
	if (AreDoorsOpen() == false && CheckOpenDoor() == false)
	{
		if (QueuePositionDirection == 1 && UpQueue.GetSize() == 0)
		{
			if (sbs->Verbose)
				Report("ProcessCallQueue: resetting search direction due to empty up queue");
			QueuePositionDirection = 0;
			LastQueueDirection = 1;
		}
		if (QueuePositionDirection == -1 && DownQueue.GetSize() == 0)
		{
			if (sbs->Verbose)
				Report("ProcessCallQueue: resetting search direction due to empty down queue");
			QueuePositionDirection = 0;
			LastQueueDirection = -1;
		}
	}
	else if (UpPeak == false && DownPeak == false)
		return; //don't process the main queue code if doors are open or moving

	//Search through queue lists and find next valid floor call
	if (QueuePositionDirection == 1)
	{
		//search through up queue
		for (int i = 0; i < UpQueue.GetSize(); i++)
		{
			//if the queued floor number is a higher floor, dispatch the elevator to that floor
			if (UpQueue[i] >= ElevatorFloor)
			{
				if (MoveElevator == false)
				{
					if (sbs->Verbose)
						Report("ProcessCallQueue up: standard dispatch, floor " + csString(_itoa(UpQueue[i], intbuffer, 10)));
					ActiveCallFloor = UpQueue[i];
					ActiveCallDirection = 1;
					GotoFloor = UpQueue[i];
					if (FireServicePhase2 == 0)
						CloseDoors();
					MoveElevator = true;
					LastQueueDirection = 1;
				}
				else
				{
					//if elevator is moving, change destination floor if not beyond decel marker of that floor
					if (GotoFloor != UpQueue[i])
					{
						float tmpdestination = sbs->GetFloor(UpQueue[i])->GetBase();
						if (BeyondDecelMarker(1, tmpdestination) == false)
						{
							ActiveCallFloor = UpQueue[i];
							GotoFloor = UpQueue[i];
							Destination = tmpdestination;
							Report("changing destination floor to " + csString(_itoa(GotoFloor, intbuffer, 10)) + " (" + sbs->GetFloor(GotoFloor)->ID + ")");
						}
						else if (sbs->Verbose)
							Report("ProcessCallQueue up: cannot change destination floor to " + csString(_itoa(UpQueue[i], intbuffer, 10)));
					}
				}
				return;
			}
			//if the queued floor number is a lower floor
			if (UpQueue[i] < ElevatorFloor && MoveElevator == false)
			{
				//dispatch elevator if it's idle
				if (IsIdle() == true && LastQueueDirection == 0)
				{
					if (sbs->Verbose)
						Report("ProcessCallQueue up: dispatching idle lower elevator, floor " + csString(_itoa(UpQueue[i], intbuffer, 10)));
					ActiveCallFloor = UpQueue[i];
					ActiveCallDirection = 1;
					GotoFloor = UpQueue[i];
					if (FireServicePhase2 == 0)
						CloseDoors();
					MoveElevator = true;
					LastQueueDirection = 1;
					return;
				}
				//reset queue if it's the last entry
				if (i == UpQueue.GetSize() - 1 && QueueResets == true)
				{
					if (sbs->Verbose)
						Report("ProcessCallQueue up: last entry (" + csString(_itoa(UpQueue[i], intbuffer, 10)) + ") is lower; resetting queue");
					ResetUpQueue = true;
					return;
				}
				//otherwise skip it if it's not the last entry
				if (sbs->Verbose)
					Report("ProcessCallQueue up: skipping floor entry " + csString(_itoa(UpQueue[i], intbuffer, 10)));
			}
		}
	}
	else if (QueuePositionDirection == -1)
	{
		//search through down queue (search order is reversed since calls need to be processed in decending order)
		for (int i = DownQueue.GetSize() - 1; i >= 0; i--)
		{
			//if the queued floor number is a lower floor, dispatch the elevator to that floor
			if (DownQueue[i] <= ElevatorFloor)
			{
				if (MoveElevator == false)
				{
					if (sbs->Verbose)
						Report("ProcessCallQueue down: standard dispatch, floor " + csString(_itoa(DownQueue[i], intbuffer, 10)));
					ActiveCallFloor = DownQueue[i];
					ActiveCallDirection = -1;
					GotoFloor = DownQueue[i];
					if (FireServicePhase2 == 0)
						CloseDoors();
					MoveElevator = true;
					LastQueueDirection = -1;
				}
				else
				{
					//if elevator is moving, change destination floor if not beyond decel marker of that floor
					if (GotoFloor != DownQueue[i])
					{
						float tmpdestination = sbs->GetFloor(DownQueue[i])->GetBase();
						if (BeyondDecelMarker(-1, tmpdestination) == false)
						{
							ActiveCallFloor = DownQueue[i];
							GotoFloor = DownQueue[i];
							Destination = tmpdestination;
							Report("changing destination floor to " + csString(_itoa(GotoFloor, intbuffer, 10)) + " (" + sbs->GetFloor(GotoFloor)->ID + ")");
						}
						else if (sbs->Verbose)
							Report("ProcessCallQueue down: cannot change destination floor to " + csString(_itoa(DownQueue[i], intbuffer, 10)));
					}
				}
				return;
			}
			//if the queued floor number is an upper floor
			if (DownQueue[i] > ElevatorFloor && MoveElevator == false)
			{
				//dispatch elevator if idle
				if (IsIdle() == true && LastQueueDirection == 0)
				{
					if (sbs->Verbose)
						Report("ProcessCallQueue down: dispatching idle higher elevator, floor " + csString(_itoa(DownQueue[i], intbuffer, 10)));
					ActiveCallFloor = DownQueue[i];
					ActiveCallDirection = -1;
					GotoFloor = DownQueue[i];
					if (FireServicePhase2 == 0)
						CloseDoors();
					MoveElevator = true;
					LastQueueDirection = -1;
					return;
				}
				//reset queue if it's the first entry
				if (i == 0 && QueueResets == true)
				{
					if (sbs->Verbose)
						Report("ProcessCallQueue down: last entry (" + csString(_itoa(DownQueue[i], intbuffer, 10)) + ") is higher; resetting queue");
					ResetDownQueue = true;
					return;
				}
				//otherwise skip it if it's not the last entry
				if (sbs->Verbose)
					Report("ProcessCallQueue down: skipping floor entry " + csString(_itoa(DownQueue[i], intbuffer, 10)));
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
			if (sbs->ToLocal(Elevator_state->GetVertex(i).y) > Height)
				Height = sbs->ToLocal(Elevator_state->GetVertex(i).y);
		}
	}

	//perform first-run tasks
	if (FirstRun == true)
	{
		FirstRun = false;

		if (UpPeak == true)
			EnableUpPeak(true);
		if (DownPeak == true)
			EnableDownPeak(true);
		if (IndependentService == true)
			EnableIndependentService(true);
		if (InspectionService == true)
			EnableInspectionService(true);
		if (ACP == true)
			EnableACP(true);
		if (ACPFloor != 0)
		{
			int tmp = ACPFloor;
			ACPFloor = 0;
			SetACPFloor(tmp);
		}
	}

	//play idle sound if in elevator, or if doors open
	if (idlesound->IsPlaying() == false)
	{
		if ((sbs->InElevator == true && sbs->ElevatorNumber == Number) || AreDoorsOpen() == true || CheckOpenDoor() == true)
		{
			if (sbs->Verbose)
				Report("playing idle sound");
			idlesound->Loop(true);
			idlesound->Play();
		}
	}
	else
	{
		if ((sbs->InElevator == false || sbs->ElevatorNumber != Number) && AreDoorsOpen() == false && CheckOpenDoor() == false)
		{
			if (sbs->Verbose)
				Report("stopping idle sound");
			idlesound->Stop();
		}
	}

	//process alarm
	if (AlarmActive == true)
		Alarm();

	//call queue processor
	ProcessCallQueue();

	//door operations
	for (int i = 1; i <= NumDoors; i++)
	{
		ElevatorDoor *door = GetDoor(i);
		if (door)
			door->Loop();
	}

	//elevator movement
	if (MoveElevator == true && (AreDoorsOpen() == false || InspectionService == true))
		MoveElevatorToFloor();

}

void Elevator::MoveElevatorToFloor()
{
	//Main processing routine; sends elevator to floor specified in GotoFloor
	//if InspectionService is enabled, this function ignores GotoFloor values, since the elevator is manually moved

	//exit if doors are open or moving
	if (AreDoorsOpen() == true || CheckOpenDoor() == true)
		return;

	if (ElevatorIsRunning == false)
	{
		if (sbs->Verbose)
			Report("starting elevator movement procedure");

		ElevatorIsRunning = true;
		csString dir_string;

		//get elevator's current altitude
		ElevatorStart = GetPosition().y;

		//get elevator's current floor
		ElevatorFloor = GetFloor();
		oldfloor = ElevatorFloor;

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

		//If elevator is already on specified floor, open doors and exit
		if (ElevatorFloor == GotoFloor && InspectionService == false)
		{
			sbs->Report("Elevator already on specified floor");
			DeleteActiveRoute();
			goto finish; //skip main processing and run cleanup section
		}

		//if destination floor is not a serviced floor, reset and exit
		if (IsServicedFloor(GotoFloor) == false && InspectionService == false)
		{
			sbs->Report("Destination floor not in ServicedFloors list");
			MoveElevator = false;
			ElevatorIsRunning = false;
			DeleteActiveRoute();
			return;
		}

		//Determine distance to destination floor
		if (InspectionService == false)
		{
			Destination = sbs->GetFloor(GotoFloor)->GetBase();
			DistanceToTravel = fabs(fabs(Destination) - fabs(ElevatorStart));
		}
		else
		{
			//otherwise if inspection service is on, choose the altitude of the top/bottom floor
			if (Direction == 1)
			{
				Destination = sbs->GetFloor(GetTopFloor())->GetBase();
				if (ElevatorStart >= Destination)
				{
					//don't go above top floor
					Report("cannot go above top floor");
					Destination = 0;
					Direction = 0;
					MoveElevator = false;
					ElevatorIsRunning = false;
					DeleteActiveRoute();
					return;
				}
			}
			else
			{
				Destination = sbs->GetFloor(GetBottomFloor())->GetBase();
				if (ElevatorStart <= Destination)
				{
					//don't go below bottom floor
					Report("cannot go below bottom floor");
					Destination = 0;
					Direction = 0;
					MoveElevator = false;
					ElevatorIsRunning = false;
					DeleteActiveRoute();
					return;
				}
			}
			DistanceToTravel = fabs(fabs(Destination) - fabs(ElevatorStart));
		}
		CalculateStoppingDistance = true;

		//If user is riding this elevator, then turn off objects
		if (sbs->ElevatorSync == true && sbs->ElevatorNumber == Number)
		{
			if (sbs->Verbose)
				Report("user in elevator - turning off objects");

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

		//Play starting sounds
		if (sbs->Verbose)
			Report("playing starting sounds");
		mainsound->Stop();
		mainsound->Load(CarStartSound.GetData());
		mainsound->Loop(false);
		mainsound->Play();
		motorsound->Stop();
		motorsound->Load(MotorStartSound.GetData());
		motorsound->Loop(false);
		motorsound->Play();

		//notify about movement
		if (InspectionService == false)
			Report("moving " + dir_string + " to floor " + csString(_itoa(GotoFloor, intbuffer, 10)) + " (" + sbs->GetFloor(GotoFloor)->ID + ")");
		else
			Report("moving " + dir_string);
		IsMoving = true;
		OnFloor = false;
	}

	if (EmergencyStop == true && Brakes == false)
	{
		//emergency stop
		if (sbs->Verbose)
			Report("handling emergency stop");
		CalculateStoppingDistance = false;
		TempDeceleration = Deceleration;
		if (Direction == 1)
			Direction = -1;
		else
			Direction = 1;
		Brakes = true;
		//stop sounds
		mainsound->Stop();
		motorsound->Stop();
		//play stopping sounds
		if (sbs->Verbose)
			Report("playing stopping sounds");
		mainsound->Load(CarStopSound.GetData());
		mainsound->Loop(false);
		mainsound->Play();
		motorsound->Load(MotorStopSound.GetData());
		motorsound->Loop(false);
		motorsound->Play();
	}

	if (mainsound->IsPlaying() == false && Brakes == false)
	{
		//Movement sound
		if (sbs->Verbose)
			Report("playing car movement sound");
		mainsound->Load(CarMoveSound.GetData());
		mainsound->Loop(true);
		mainsound->Play();
	}

	if (motorsound->IsPlaying() == false && Brakes == false)
	{
		//Motor sound
		if (sbs->Verbose)
			Report("playing motor run sound");
		motorsound->Load(MotorRunSound.GetData());
		motorsound->Loop(true);
		motorsound->Play();
	}

	//move elevator objects and camera
	Elevator_movable->MovePosition(csVector3(0, sbs->ToRemote(ElevatorRate * sbs->delta), 0));
	Elevator_movable->UpdateMove();
	if (sbs->ElevatorSync == true && sbs->ElevatorNumber == Number)
		sbs->camera->SetPosition(csVector3(sbs->camera->GetPosition().x, GetPosition().y + sbs->camera->cfg_legs_height + sbs->camera->cfg_body_height, sbs->camera->GetPosition().z));
	MoveDoors(0, csVector3(0, ElevatorRate * sbs->delta, 0), true, true, true);
	for (int i = 0; i < FloorIndicatorArray.GetSize(); i++)
	{
		if (FloorIndicatorArray[i])
			FloorIndicatorArray[i]->MovePosition(csVector3(0, ElevatorRate * sbs->delta, 0));
	}
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
	floorsound->SetPosition(GetPosition());
	for (int i = 0; i < sounds.GetSize(); i++)
	{
		if (sounds[i])
			sounds[i]->SetPositionY(GetPosition().y + sounds[i]->PositionOffset.y);
	}

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

	//determine if next jump altitude is over deceleration marker
	if (Brakes == false)
	{
		if (BeyondDecelMarker(Direction, Destination) == true)
		{
			if (sbs->Verbose)
				Report("beyond deceleration marker - slowing down");

			//up movement
			if (Direction == 1)
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
			}
			//down movement
			else if (Direction == -1)
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
			}

			//stop sounds
			mainsound->Stop();
			motorsound->Stop();
			//play elevator stopping sounds
			if (sbs->Verbose)
				Report("playing stopping sounds");
			mainsound->Load(CarStopSound.GetData());
			mainsound->Loop(false);
			mainsound->Play();
			motorsound->Load(MotorStopSound.GetData());
			motorsound->Loop(false);
			motorsound->Play();
		}
	}

	if (GetFloor() != oldfloor)
	{
		if (sbs->Verbose)
			Report("on floor " + csString(_itoa(GetFloor(), intbuffer, 10)));

		//play floor beep sound if not empty
		if (BeepSound != "" && IsServicedFloor(GetFloor()) == true && UseFloorBeeps == true)
		{
			if (sbs->Verbose)
				Report("playing floor beep sound");
			floorbeep->Stop();
			floorbeep->Load(BeepSound);
			floorbeep->Loop(false);
			floorbeep->Play();
		}

		//update floor indicators
		if (sbs->ElevatorSync == true && sbs->ElevatorNumber == Number)
			UpdateFloorIndicators();

		//update floor indicators on current camera floor
		sbs->GetFloor(sbs->camera->CurrentFloor)->UpdateFloorIndicators(Number);
	}

	oldfloor = GetFloor();

	//exit if elevator's running
	if (fabs(ElevatorRate) != 0)
		return;

	//finish move
	if (EmergencyStop == false)
	{
		if (sbs->Verbose)
			Report("storing error offset");

		//store error offset value
		if (Direction == -1)
			ErrorOffset = GetPosition().y - Destination;
		else if (Direction == 1)
			ErrorOffset = Destination - GetPosition().y;
		else
			ErrorOffset = 0;

		//set elevator and objects to floor altitude (corrects offset errors)
		//move elevator objects
		if (sbs->Verbose)
			Report("setting elevator to floor altitude");
		Elevator_movable->SetPosition(sbs->ToRemote(csVector3(GetPosition().x, Destination, GetPosition().z)));
		Elevator_movable->UpdateMove();
		if (sbs->ElevatorSync == true && sbs->ElevatorNumber == Number)
			sbs->camera->SetPosition(csVector3(sbs->camera->GetPosition().x, GetPosition().y + sbs->camera->cfg_legs_height + sbs->camera->cfg_body_height, sbs->camera->GetPosition().z));
		MoveDoors(0, csVector3(0, Destination, 0), true, false, true);
		for (int i = 0; i < FloorIndicatorArray.GetSize(); i++)
		{
			if (FloorIndicatorArray[i])
				FloorIndicatorArray[i]->SetPosition(csVector3(FloorIndicatorArray[i]->GetPosition().x, Destination, FloorIndicatorArray[i]->GetPosition().z));
		}
		if (Panel)
			Panel->SetToElevatorAltitude();
		if (Panel2)
			Panel2->SetToElevatorAltitude();

		//move sounds
		mainsound->SetPosition(GetPosition());
		alarm->SetPosition(GetPosition());
		for (int i = 0; i < sounds.GetSize(); i++)
		{
			if (sounds[i])
				sounds[i]->SetPositionY(GetPosition().y + sounds[i]->PositionOffset.y);
		}

		//the elevator is now stopped on a valid floor; set OnFloor to true
		OnFloor = true;
		Report("arrived at floor " + csString(_itoa(GotoFloor, intbuffer, 10)) + " (" + sbs->GetFloor(GotoFloor)->ID + ")");

		//dequeue floor route
		DeleteActiveRoute();
	}

	//reset values if at destination floor
finish:
	if (sbs->Verbose)
		Report("resetting elevator motion values");
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
	motorsound->Stop();

	if (EmergencyStop == false && InspectionService == false)
	{
		//update floor indicators on current camera floor
		sbs->GetFloor(sbs->camera->CurrentFloor)->UpdateFloorIndicators(Number);

		//Turn on objects if user is in elevator
		if (sbs->ElevatorSync == true && sbs->ElevatorNumber == Number)
		{
			if (sbs->Verbose)
				Report("user in elevator - turning on objects");

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
		else if (sbs->Verbose)
			Report("user not in elevator - not turning on objects");

		//change directional indicator and disable call button light
		if (InServiceMode() == false)
		{
			//reverse queue if at end of current queue, and if elevator was moving in the correct direction (not moving up for a down call, etc)
			if ((QueuePositionDirection == 1 && UpQueue.GetSize() == 0 && ElevatorFloor < GotoFloor) || (QueuePositionDirection == -1 && DownQueue.GetSize() == 0 && ElevatorFloor > GotoFloor))
			{
				csArray<int> buttons = sbs->GetFloor(GotoFloor)->GetCallButtons(Number);
				if (buttons.GetSize() > 0)
				{
					CallButton *button =  sbs->GetFloor(GotoFloor)->CallButtonArray[buttons[0]];
					//only reverse the queue direction if no related active call is on the floor
					if ((button->UpStatus == false && QueuePositionDirection == 1) || (button->DownStatus == false && QueuePositionDirection == -1))
					{
						if (sbs->Verbose)
							Report("reversing queue search direction");
						LastQueueDirection = QueuePositionDirection;
						QueuePositionDirection = -QueuePositionDirection;
					}
				}
			}

			bool LightDirection = false; //true for up, false for down

			if (GetFloor() == GetTopFloor())
				LightDirection = false; //turn on down light if on top floor
			else if (GetFloor() == GetBottomFloor())
				LightDirection = true; //turn on up light if on bottom floor
			else if (QueuePositionDirection == 1)
				LightDirection = true; //turn on up light if queue direction is up
			else if (QueuePositionDirection == -1)
				LightDirection = false; //turn on down light if queue direction is down

			//play chime sound and change indicator
			if (LightDirection == true)
			{
				Chime(0, GetFloor(), true);
				SetDirectionalIndicator(GetFloor(), true, false);
			}
			else
			{
				Chime(0, GetFloor(), false);
				SetDirectionalIndicator(GetFloor(), false, true);
			}

			//disable call button lights
			SetCallButtons(GetFloor(), LightDirection, false);
		}

		//open doors
		//do not automatically open doors if in fire service phase 2
		if (FireServicePhase2 == 0)
		{
			OpenDoors();

			//play floor sound if not empty
			if (FloorSound != "" && UseFloorSounds == true)
			{
				if (sbs->Verbose)
					Report("playing floor sound");
				csString newsound = FloorSound;
				//change the asterisk into the current floor number
				newsound.ReplaceAll("*", csString(_itoa(GotoFloor, intbuffer, 10)).Trim());
				floorsound->Stop();
				floorsound->Load(newsound);
				floorsound->Loop(false);
				floorsound->Play();
			}
		}
	}
	else
	{
		if (sbs->Verbose)
			Report("emergency stop complete");
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

	//update elevator's floor number
	ElevatorFloor = GetFloor();
}

int Elevator::AddWall(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th)
{
	//Adds a wall with the specified dimensions

	//Set horizontal scaling
	x1 = x1 * sbs->HorizScale;
	x2 = x2 * sbs->HorizScale;
	z1 = z1 * sbs->HorizScale;
	z2 = z2 * sbs->HorizScale;

	//calculate autosizing
	float tmpheight;
	if (height1 > height2)
		tmpheight = height1;
	else
		tmpheight = height2;
	csVector2 sizing = sbs->CalculateSizing(texture, csVector2(x1, x2), csVector2(0, tmpheight), csVector2(z1, z2), tw, th);

	WallObject *wall = sbs->CreateWallObject(elevator_walls, ElevatorMesh, this->object, name);
	return sbs->AddWallMain(wall, name, texture, thickness, x1, z1, x2, z2, height1, height2, voffset1, voffset2, sizing.x, sizing.y);
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

	//get texture force value
	bool force_enable, force_mode;
	sbs->GetTextureForce(texture, force_enable, force_mode);

	//Call texture autosizing formulas
	tw2 = sbs->AutoSize(x1, x2, true, tw, force_enable, force_mode);
	th2 = sbs->AutoSize(z1, z2, false, th, force_enable, force_mode);

	WallObject *wall = sbs->CreateWallObject(elevator_walls, ElevatorMesh, this->object, name);
	return sbs->AddFloorMain(wall, name, texture, thickness, x1, z1, x2, z2, voffset1, voffset2, tw2, th2);
}

void Elevator::AddFloorIndicator(const char *texture_prefix, const char *direction, float CenterX, float CenterZ, float width, float height, float voffset)
{
	//Creates a floor indicator at the specified location

	int size = FloorIndicatorArray.GetSize();
	FloorIndicatorArray.SetSize(size + 1);
	FloorIndicatorArray[size] = new FloorIndicator(Number, texture_prefix, direction, CenterX, CenterZ, width, height, voffset);
	FloorIndicatorArray[size]->SetPosition(Origin);
}

const csVector3 Elevator::GetPosition()
{
	//returns the elevator's position
	return sbs->ToLocal(Elevator_movable->GetPosition());
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

	if (IsEnabled == value)
		return;

	if (sbs->Verbose)
	{
		if (value == true)
			Report("enabling elevator");
		else
			Report("disabling elevator");
	}

	sbs->EnableMesh(ElevatorMesh, value);
	EnableDoors(value);
	IsEnabled = value;
}

void Elevator::EnableObjects(bool value)
{
	//enable or disable interior objects, such as floor indicators and button panels

	if (sbs->Verbose)
	{
		if (value == true)
			Report("enabling objects");
		else
			Report("disabling objects");
	}

	//floor indicators
	for (int i = 0; i < FloorIndicatorArray.GetSize(); i++)
	{
		if (FloorIndicatorArray[i])
			FloorIndicatorArray[i]->Enabled(value);
	}

	//panels
	if (Panel)
		Panel->Enabled(value);
	if (Panel2)
		Panel2->Enabled(value);

	//sounds
	for (int i = 0; i < sounds.GetSize(); i++)
	{
		if (sounds[i])
		{
			if (value == false)
				sounds[i]->Stop();
			else
				sounds[i]->Play();
		}
	}
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
	return ElevatorMesh->HitBeam(sbs->ToRemote(start), sbs->ToRemote(end));
}

bool Elevator::IsInElevator(const csVector3 &position)
{
	//if last position is the same as new, return previous result
	if (position == lastposition && checkfirstrun == false)
		return lastcheckresult;

	checkfirstrun = false;

	if (position.y > GetPosition().y && position.y < GetPosition().y + Height)
	{
		csHitBeamResult result = HitBeam(position, csVector3(position.x, position.y - Height, position.z));

		//cache values
		lastcheckresult = result.hit;
		lastposition = position;

		return result.hit;
	}

	//cache values
	lastcheckresult = false;
	lastposition = position;

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
	if (sbs->Verbose)
		Report("adding serviced floor " + csString(_itoa(number, intbuffer, 10)));
	if (IsServicedFloor(number) == false)
		ServicedFloors.InsertSorted(number);
}

void Elevator::RemoveServicedFloor(int number)
{
	if (sbs->Verbose)
		Report("removing serviced floor " + csString(_itoa(number, intbuffer, 10)));
	if (IsServicedFloor(number) == true)
		ServicedFloors.Delete(number);
}

void Elevator::CreateButtonPanel(const char *texture, int rows, int columns, const char *direction, float CenterX, float CenterZ, float buttonwidth, float buttonheight, float spacingX, float spacingY, float voffset, float tw, float th)
{
	//create a new button panel object and store the pointer
	if (!Panel)
	{
		if (sbs->Verbose)
			Report("creating button panel 1");
		Panel = new ButtonPanel(Number, 1, texture, rows, columns, direction, CenterX, CenterZ, buttonwidth, buttonheight, spacingX, spacingY, voffset, tw, th);
	}
	else if (!Panel2)
	{
		if (sbs->Verbose)
			Report("creating button panel 2");
		Panel2 = new ButtonPanel(Number, 2, texture, rows, columns, direction, CenterX, CenterZ, buttonwidth, buttonheight, spacingX, spacingY, voffset, tw, th);
	}
	else
		ReportError("Button panels already exist");
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

	if (sbs->Verbose)
		Report("setting floor skip text to " + csString(id));
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
	{
		if (sbs->Verbose)
			Report("Floor " + csString(_itoa(floor, intbuffer, 10)) + " is not a serviced floor");
		return false;
	}
	else
	{
		if (sbs->Verbose)
			Report("Floor " + csString(_itoa(floor, intbuffer, 10)) + " is a serviced floor");
		return true;
	}
}

bool Elevator::InServiceMode()
{
	//report if an elevator is in a service mode
	if (IndependentService == true || InspectionService == true || FireServicePhase1 == 1 || FireServicePhase2 != 0)
		return true;
	else
		return false;
}

void Elevator::Go(int floor)
{
	//go to specified floor, without using the queuing system

	//exit if in inspection mode
	if (InspectionService == true)
	{
		if (sbs->Verbose)
			Report("Go: in inspection mode");
		return;
	}

	if (sbs->Verbose)
		Report("Go: proceeding to floor " + csString(_itoa(floor, intbuffer, 10)));
	GotoFloor = floor;
	MoveElevator = true;
}

void Elevator::GoToRecallFloor()
{
	//for fire service modes; tells the elevator to go to the recall floor (or the alternate recall floor
	//if the other is not available)

	if (RecallUnavailable == false)
	{
		Report("Proceeding to recall floor");
		Go(RecallFloor);
	}
	else
	{
		Report("Proceeding to alternate recall floor");
		Go(RecallFloorAlternate);
	}
}

void Elevator::EnableACP(bool value)
{
	//enable Anti-Crime Protection (ACP) mode

	//exit if no change
	if (ACP == value)
	{
		if (sbs->Verbose)
			Report("EnableACP: mode is the same");
		return;
	}

	ACP = value;

	if (value == true)
	{
		EnableUpPeak(false);
		EnableDownPeak(false);
		EnableIndependentService(false);
		EnableInspectionService(false);
		EnableFireService1(0);
		EnableFireService2(0);
		Report("ACP mode enabled");
	}
	else
		Report("ACP mode disabled");

}

void Elevator::EnableUpPeak(bool value)
{
	//enable Up-Peak mode

	//exit if no change
	if (UpPeak == value)
	{
		if (sbs->Verbose)
			Report("EnableUpPeak: mode is the same");
		return;
	}

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
		{
			SetDirectionalIndicator(GetFloor(), true, false);
			OpenDoors();
		}
		Report("Up Peak mode enabled");
	}
	else
	{
		ResetDoorTimer();
		Report("Up Peak mode disabled");
	}
}

void Elevator::EnableDownPeak(bool value)
{
	//enable Down-Peak mode

	//exit if no change
	if (DownPeak == value)
	{
		if (sbs->Verbose)
			Report("EnableDownPeak: mode is the same");
		return;
	}

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
		{
			SetDirectionalIndicator(GetFloor(), false, true);
			OpenDoors();
		}
		Report("Down Peak mode enabled");
	}
	else
	{
		ResetDoorTimer();
		Report("Down Peak mode disabled");
	}
}

void Elevator::EnableIndependentService(bool value)
{
	//enable Independent Service (ISC) mode

	//exit if no change
	if (IndependentService == value)
	{
		if (sbs->Verbose)
			Report("EnableIndependentService: mode is the same");
		return;
	}

	IndependentService = value;

	if (value == true)
	{
		EnableACP(false);
		EnableUpPeak(false);
		EnableDownPeak(false);
		EnableInspectionService(false);
		EnableFireService1(0);
		EnableFireService2(0);
		ResetUpQueue = true;
		ResetDownQueue = true;
		if (IsMoving == false)
			OpenDoors();
		Report("Independent Service mode enabled");
	}
	else
	{
		ResetDoorTimer();
		Report("Independent Service mode disabled");
	}
}

void Elevator::EnableInspectionService(bool value)
{
	//enable Inspection Service (INS) mode

	//exit if no change
	if (InspectionService == value)
	{
		if (sbs->Verbose)
			Report("EnableInspectionService: mode is the same");
		return;
	}

	if (value == true)
	{
		EnableACP(false);
		EnableUpPeak(false);
		EnableDownPeak(false);
		EnableIndependentService(false);
		EnableFireService1(0);
		EnableFireService2(0);
		ResetUpQueue = true;
		ResetDownQueue = true;
		if (IsMoving == true)
			StopElevator();
		Report("Inspection Service mode enabled");
	}
	else
	{
		ResetDoorTimer();
		Report("Inspection Service mode disabled");
	}

	InspectionService = value;
}

void Elevator::EnableFireService1(int value)
{
	//enable Fire Service Phase 1 mode
	//valid values are 0 (off), 1 (on) or 2 (bypass)

	//exit if no change
	if (FireServicePhase1 == value)
	{
		if (sbs->Verbose)
			Report("EnableFireService1: mode is the same");
		return;
	}

	if (value >= 0 && value <= 2)
		FireServicePhase1 = value;
	else
	{
		if (sbs->Verbose)
			Report("EnableFireService1: invalid value");
		return;
	}

	if (value > 0)
	{
		EnableACP(false);
		EnableUpPeak(false);
		EnableDownPeak(false);
		EnableIndependentService(false);
		EnableInspectionService(false);
		EnableFireService2(0);
		ResetUpQueue = true;
		ResetDownQueue = true;
		if (value == 1)
		{
			Report("Fire Service Phase 1 mode set to On");
			GoToRecallFloor();
		}
		else
			Report("Fire Service Phase 1 mode set to Bypass");
	}
	else
	{
		ResetDoorTimer();
		Report("Fire Service Phase 1 mode set to Off");
	}
}

void Elevator::EnableFireService2(int value)
{
	//enable Fire Service Phase 2 mode
	//valid values are 0 (off), 1 (on) or 2 (hold)

	//exit if no change
	if (FireServicePhase2 == value)
	{
		if (sbs->Verbose)
			Report("EnableFireService2: mode is the same");
		return;
	}

	if (value >= 0 && value <= 2)
		FireServicePhase2 = value;
	else
	{
		if (sbs->Verbose)
			Report("EnableFireService2: invalid value");
		return;
	}

	if (value > 0)
	{
		EnableACP(false);
		EnableUpPeak(false);
		EnableDownPeak(false);
		EnableIndependentService(false);
		EnableInspectionService(false);
		EnableFireService1(0);
		ResetUpQueue = true;
		ResetDownQueue = true;
		if (value == 1)
			Report("Fire Service Phase 2 mode set to On");
		else
		{
			if (IsMoving == false)
				OpenDoors();
			Report("Fire Service Phase 2 mode set to Hold");
		}
	}
	else
	{
		ResetDoorTimer();
		Report("Fire Service Phase 2 mode set to Off");
		GoToRecallFloor();
	}
}

bool Elevator::SetRecallFloor(int floor)
{
	//set elevator's fire recall floor
	if (ServicedFloors.GetSize() == 0)
	{
		ReportError("No serviced floors assigned");
		return false;
	}
	if (IsServicedFloor(floor) == false)
	{
		ReportError("Invalid recall floor");
		return false;
	}

	if (sbs->Verbose)
		Report("setting recall floor to " + csString(_itoa(floor, intbuffer, 10)));
	RecallFloor = floor;
	RecallSet = true;
	return true;
}

bool Elevator::SetAlternateRecallFloor(int floor)
{
	//set elevator's alternate fire recall floor
	if (ServicedFloors.GetSize() == 0)
	{
		ReportError("No serviced floors assigned");
		return false;
	}
	if (IsServicedFloor(floor) == false)
	{
		ReportError("Invalid alternate recall floor");
		return false;
	}

	if (sbs->Verbose)
		Report("setting alternate recall floor to " + csString(_itoa(floor, intbuffer, 10)));
	RecallFloorAlternate = floor;
	RecallAltSet = true;
	return true;
}

bool Elevator::SetACPFloor(int floor)
{
	//set elevator's ACP floor
	if (ServicedFloors.GetSize() == 0)
	{
		ReportError("No serviced floors assigned");
		return false;
	}
	if (IsServicedFloor(floor) == false)
	{
		ReportError("Invalid ACP floor");
		return false;
	}

	if (sbs->Verbose)
		Report("setting ACP floor to " + csString(_itoa(floor, intbuffer, 10)));
	ACPFloor = floor;
	ACPFloorSet = true;
	return true;
}

bool Elevator::MoveUp()
{
	//moves elevator upwards if in Inspection Service mode
	if (InspectionService == false)
	{
		Report("Not in inspection service mode");
		return false;
	}

	//make sure Go button is on
	if (ManualGo == false)
	{
		if (sbs->Verbose)
			Report("MoveUp: go button is off");
		return false;
	}

	if (IsMoving == true)
	{
		if (sbs->Verbose)
			Report("MoveUp: already moving");
		return false;
	}

	//set direction
	Direction = 1;
	MoveElevator = true;
	if (sbs->Verbose)
		Report("MoveUp: moving elevator");
	return true;
}

bool Elevator::MoveDown()
{
	//moves elevator downwards if in Inspection Service mode
	if (InspectionService == false)
	{
		Report("Not in inspection service mode");
		return false;
	}

	//make sure Go button is on
	if (ManualGo == false)
	{
		if (sbs->Verbose)
			Report("MoveDown: go button is off");
		return false;
	}

	if (IsMoving == true)
	{
		if (sbs->Verbose)
			Report("MoveDown: already moving");
		return false;
	}

	//set direction
	Direction = -1;
	MoveElevator = true;
	if (sbs->Verbose)
		Report("MoveDown: moving elevator");
	return true;
}

bool Elevator::StopMove()
{
	//stops elevator movement if in Inspection Service mode
	//this is almost identical to the StopElevator() function, except it reports differently. This is also to
	//separate it from the regular Stop button

	if (InspectionService == false)
	{
		Report("Not in inspection service mode");
		return false;
	}

	if (IsMoving == false)
	{
		if (sbs->Verbose)
			Report("StopMove: not moving");
		return false;
	}

	EmergencyStop = true;
	Report("Stopping elevator");
	return true;
}

void Elevator::SetGoButton(bool value)
{
	//sets the value of the Go button (for Inspection Service mode)

	if (ManualGo == true && value == false)
		StopMove();

	ManualGo = value;

	if (sbs->Verbose)
	{
		if (value == true)
			Report("setting go button status to true");
		else
			Report("setting go button status to false");
	}
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

	if (sbs->Verbose)
		Report("adding directional indicators");

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

	if (sbs->Verbose)
		Report("adding directional indicator for floor " + csString(_itoa(floor, intbuffer, 10)));

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
	{
		/*if (sbs->Verbose)
			Report("EnableDirectionalIndicator: elevator does not service floor " + csString(_itoa(floor, intbuffer, 10)));*/
		return;
	}

	if (IndicatorArray[index])
		IndicatorArray[index]->Enabled(value);
}

void Elevator::SetDirectionalIndicator(int floor, bool UpLight, bool DownLight)
{
	//set light status of directional indicator

	int index = ServicedFloors.Find(floor);

	if (index == -1)
	{
		/*if (sbs->Verbose)
			Report("SetDirectionalIndicator: elevator does not service floor " + csString(_itoa(floor, intbuffer, 10)));*/
		return;
	}

	if (IndicatorArray[index])
	{
		IndicatorArray[index]->DownLight(DownLight);
		IndicatorArray[index]->UpLight(UpLight);
	}
}

void Elevator::EnableDirectionalIndicators(bool value)
{
	//turn on/off all directional indicators

	if (sbs->Verbose)
	{
		if (value == true)
			Report("enabling directional indicators");
		else
			Report("disabling directional indicators");
	}

	for (size_t i = 0; i < ServicedFloors.GetSize(); i++)
	{
		if (IndicatorArray[i])
			IndicatorArray[i]->Enabled(value);
	}
}

ElevatorDoor* Elevator::GetDoor(int number)
{
	//get elevator door object

	//return cached check if number is the same
	if (lastdoor_number == number)
		return lastdoor_result;

	if (number > 0 && number <= DoorArray.GetSize())
	{
		if (DoorArray[number - 1])
		{
			lastdoor_result = DoorArray[number - 1];
			lastdoor_number = number;
			return lastdoor_result;
		}
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
			Report("Invalid door " + csString(_itoa(i, intbuffer, 10)));
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
			Report("Invalid door " + csString(_itoa(i, intbuffer, 10)));
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
			Report("Invalid door " + csString(_itoa(i, intbuffer, 10)));
	}
}

int Elevator::AddDoors(int number, const char *lefttexture, const char *righttexture, float thickness, float CenterX, float CenterZ, float width, float height, bool direction, float tw, float th)
{
	//adds elevator doors specified at a relative central position (off of elevator origin)
	//if direction is false, doors are on the left/right side; otherwise front/back

	if (GetDoor(number))
		return GetDoor(number)->AddDoors(lefttexture, righttexture, thickness, CenterX, CenterZ, width, height, direction, tw, th);
	else
		Report("Invalid door " + csString(_itoa(number, intbuffer, 10)));
	return 0;
}

int Elevator::AddShaftDoors(int number, const char *lefttexture, const char *righttexture, float thickness, float CenterX, float CenterZ, float tw, float th)
{
	//adds shaft's elevator doors specified at a relative central position (off of elevator origin)
	//uses some parameters (width, height, direction) from AddDoors function

	if (GetDoor(number))
		return GetDoor(number)->AddShaftDoors(lefttexture, righttexture, thickness, CenterX, CenterZ, tw, th);
	else
		Report("Invalid door " + csString(_itoa(number, intbuffer, 10)));
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
		ElevatorDoor *door = GetDoor(i);
		if (door)
			door->ShaftDoorsEnabled(floor, value);
		else
			Report("Invalid door " + csString(_itoa(i, intbuffer, 10)));
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
		ElevatorDoor *door = GetDoor(i);
		if (door)
			door->ShaftDoorsEnabledRange(floor, range);
		else
			Report("Invalid door " + csString(_itoa(i, intbuffer, 10)));
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
		ElevatorDoor *door = GetDoor(i);
		if (door)
		{
			if (door->AreDoorsOpen() == true)
				return true;
		}
		else
			Report("Invalid door " + csString(_itoa(i, intbuffer, 10)));
	}
	return false;
}

bool Elevator::AreShaftDoorsOpen(int number, int floor)
{
	//returns the internal shaft door state

	ElevatorDoor *door = GetDoor(number);
	if (door)
		return door->AreShaftDoorsOpen(floor);
	else
		Report("Invalid door " + csString(_itoa(number, intbuffer, 10)));
	return false;
}

float Elevator::GetCurrentDoorSpeed(int number)
{
	//returns the internal door speed value

	ElevatorDoor *door = GetDoor(number);
	if (door)
		return door->GetCurrentDoorSpeed();
	else
		Report("Invalid door " + csString(_itoa(number, intbuffer, 10)));
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
		ElevatorDoor *door = GetDoor(i);
		if (door)
			door->Chime(floor, direction);
		else
			Report("Invalid door " + csString(_itoa(i, intbuffer, 10)));
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
		ElevatorDoor *door = GetDoor(i);
		if (door)
			door->ResetDoorTimer();
		else
			Report("Invalid door " + csString(_itoa(i, intbuffer, 10)));
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
		ElevatorDoor *door = GetDoor(i);
		if (door)
			return door->DoorsStopped();
		else
			Report("Invalid door " + csString(_itoa(i, intbuffer, 10)));
	}
	return false;
}

bool Elevator::CheckOpenDoor()
{
	//check each door's OpenDoor value and return true if any are not zero, or
	//false if all are zero

	for (int i = 1; i <= NumDoors; i++)
	{
		ElevatorDoor *door = GetDoor(i);
		if (door)
		{
			if (door->OpenDoor != 0)
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
	//enable/disable all doors

	if (sbs->Verbose)
	{
		if (value == true)
			Report("enabling doors");
		else
			Report("disabling doors");
	}

	for (int i = 1; i <= NumDoors; i++)
	{
		ElevatorDoor *door = GetDoor(i);
		if (door)
			door->Enabled(value);
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

void Elevator::AddFloorSigns(int door_number, bool relative, const char *texture_prefix, const char *direction, float CenterX, float CenterZ, float width, float height, float voffset)
{
	//adds floor signs at the specified position and direction for each serviced floor,
	//depending on if the given door number services the floor or not (unless door_number is 0)

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

	bool autosize_x, autosize_y;
	sbs->GetAutoSize(autosize_x, autosize_y);
	sbs->SetAutoSize(false, false);

	for (int i = 0; i < ServicedFloors.GetSize(); i++)
	{
		bool door_result = false;
		if (door_number != 0)
			door_result = GetDoor(door_number)->ShaftDoorsExist(ServicedFloors[i]);

		if (door_number == 0 || door_result == true)
		{
			csString texture = texture_prefix + sbs->GetFloor(ServicedFloors[i])->ID;
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
	}
	sbs->SetAutoSize(autosize_x, autosize_y);
}

void Elevator::SetCallButtons(int floor, bool direction, bool value)
{
	//sets light status of all associated call buttons on the specified floor
	//for direction, true is up and false is down

	//get call buttons associated with this elevator
	if (sbs->Verbose)
		Report("SetCallButtons: getting associated call buttons");

	csArray<int> buttons = sbs->GetFloor(GetFloor())->GetCallButtons(Number);

	for (int i = 0; i < buttons.GetSize(); i++)
	{
		CallButton *button = sbs->GetFloor(floor)->CallButtonArray[buttons[i]];
		if (button)
		{
			if (direction == true)
				button->UpLight(value);
			else
				button->DownLight(value);
		}
	}

}

bool Elevator::IsIdle()
{
	//return true if elevator is idle (not moving, doors are closed (unless in a peak mode) and not moving)
	if (MoveElevator == false && (AreDoorsOpen() == false || UpPeak == true || DownPeak == true) && CheckOpenDoor() == false)
		return true;
	else
		return false;
}

void Elevator::QueueReset()
{
	//reset queues
	Report("resetting queues");
	ResetUpQueue = true;
	ResetDownQueue = true;
}

void Elevator::SetBeepSound(const char *filename)
{
	//set sound used for floor beeps
	if (sbs->Verbose)
		Report("setting beep sound");
	BeepSound = filename;
	BeepSound.Trim();
	UseFloorBeeps = true;
}

void Elevator::SetFloorSound(const char *prefix)
{
	//set prefix of floor sound
	if (sbs->Verbose)
		Report("setting floor sound");
	FloorSound = prefix;
	FloorSound.Trim();
	UseFloorSounds = true;
}

bool Elevator::AddSound(const char *name, const char *filename, csVector3 position, int volume, int speed, float min_distance, float max_distance, float dir_radiation, csVector3 direction)
{
	//create a looping sound object
	sounds.SetSize(sounds.GetSize() + 1);
	Sound *sound = sounds[sounds.GetSize() - 1];
	sound = new Sound(this->object, name);

	//set parameters and play sound
	sound->PositionOffset = position;
	sound->SetPosition(Origin + position);
	sound->SetDirection(direction);
	sound->SetVolume(volume);
	sound->SetSpeed(speed);
	sound->SetMinimumDistance(min_distance);
	sound->SetMaximumDistance(max_distance);
	sound->SetDirection(direction);
	sound->SetDirectionalRadiation(dir_radiation);
	sound->Load(filename);
	sound->Loop(true);
	sound->Play();

	return true;
}

void Elevator::DeleteActiveRoute()
{
	//deletes the active route
	if (sbs->Verbose)
		Report("deleting active route");
	DeleteRoute(ActiveCallFloor, ActiveCallDirection);
	ActiveCallFloor = 0;
	ActiveCallDirection = 0;
}

bool Elevator::IsQueueActive()
{
	//if ((QueuePositionDirection == 1 && UpQueue.GetSize() != 0) || (QueuePositionDirection == -1 && DownQueue.GetSize() != 0))
	if (QueuePositionDirection != 0)
		return true;
	return false;
}

bool Elevator::BeyondDecelMarker(int direction, float destination)
{
	//return true if elevator is beyond the deceleration marker for the specified direction
	//directions are 1 for up, -1 for down

	if (direction == 1)
	{
		if (((GetPosition().y + (ElevatorRate * sbs->delta)) > (destination - StoppingDistance)))
			return true;
	}
	if (direction == -1)
	{
		if (((GetPosition().y - (ElevatorRate * sbs->delta)) < (destination + StoppingDistance)))
			return true;
	}
	return false;
}

void Elevator::Report(const char *message)
{
	//general reporting function
	sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": " + message);

}

void Elevator::ReportError(const char *message)
{
	//general reporting function
	sbs->ReportError("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": " + message);
}
