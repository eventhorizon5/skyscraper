/* $Id$ */

/*
	Scalable Building Simulator - Elevator Subsystem Class
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
	CameraOffset = 0;
	ParkingFloor = 0;
	ParkingDelay = 0;
	Leveling = false;
	LevelingSpeed = sbs->confman->GetFloat("Skyscraper.SBS.Elevator.LevelingSpeed", 0.2);
	LevelingOffset = sbs->confman->GetFloat("Skyscraper.SBS.Elevator.LevelingOffset", 0.5);
	LevelingOpen = sbs->confman->GetFloat("Skyscraper.SBS.Elevator.LevelingOpen", 0);
	tmpDecelJerk = 0;
	FinishedMove = false;
	WaitForDoors = false;
	ActiveDirection = 0;

	//create timer
	timer = new Timer(this);

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
	//delete timer
	if (sbs->Verbose)
		Report("deleting timer");

	if (timer)
	{
		timer->Stop();
		delete timer;
	}
	timer = 0;

	//delete directional indicators
	if (sbs->Verbose)
		Report("deleting interior directional indicators");

	for (int i = 0; i < DirIndicatorArray.GetSize(); i++)
	{
		if (DirIndicatorArray[i])
			delete DirIndicatorArray[i];
	}
	DirIndicatorArray.DeleteAll();

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

	//delete panels
	if (sbs->Verbose)
		Report("deleting button panels");

	for (int i = 0; i < PanelArray.GetSize(); i++)
	{
		if (PanelArray[i])
			delete PanelArray[i];
		PanelArray[i] = 0;
	}
	PanelArray.DeleteAll();

	//Destructor
	if (sbs->Verbose)
		Report("deleting objects");
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

	//delete wall objects
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

Object* Elevator::CreateElevator(bool relative, float x, float z, int floor)
{
	//Creates elevator at specified location and floor
	//x and z are the center coordinates
	//if relative is true, then x and z coordinates are relative
	//to the assigned shaft's center

	if (Created == true)
	{
		ReportError("Has already been created");
		return 0;
	}

	//make sure required values are set
	if (ElevatorSpeed <= 0)
	{
		ReportError("Speed not set or invalid");
		return 0;
	}
	if (Acceleration <= 0)
	{
		ReportError("Acceleration not set or invalid");
		return 0;
	}
	if (Deceleration <= 0)
	{
		ReportError("Deceleration not set or invalid");
		return 0;
	}
	if (NumDoors < 0)
	{
		ReportError("Number of doors invalid");
		return 0;
	}
	if (AccelJerk <= 0)
	{
		ReportError("Invalid value for AccelJerk");
		return 0;
	}
	if (DecelJerk <= 0)
	{
		ReportError("Invalid value for DecelJerk");
		return 0;
	}
	if (AssignedShaft <= 0)
	{
		ReportError("Not assigned to a shaft");
		return 0;
	}
	if (!sbs->GetShaft(AssignedShaft))
	{
		csString num;
		num = AssignedShaft;
		ReportError("Shaft " + num + " doesn't exist");
		return 0;
	}
	if (floor < sbs->GetShaft(AssignedShaft)->startfloor || floor > sbs->GetShaft(AssignedShaft)->endfloor)
	{
		csString num;
		num = floor;
		ReportError("Invalid starting floor " + num);
		return 0;
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

	Report("created at " + csString(sbs->TruncateNumber(x, 4)) + ", " + csString(sbs->TruncateNumber(x, 4)) + ", " + csString(_itoa(floor, buffer, 12)));
	return object;
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
		for (int i = 0; i < PanelArray.GetSize(); i++)
			PanelArray[i]->ChangeLight(floor, true);
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
	for (int i = 0; i < PanelArray.GetSize(); i++)
		PanelArray[i]->ChangeLight(floor, false);
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
			for (int j = 0; j < PanelArray.GetSize(); j++)
				PanelArray[j]->ChangeLight(ServicedFloors[i], false);
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
					if (FireServicePhase2 == 0 || UpPeak == true || DownPeak == true)
					{
						CloseDoors();
						WaitForDoors = true;
					}
					MoveElevator = true;
					LastQueueDirection = 1;
				}
				else if (Leveling == false)
				{
					//if elevator is moving and not leveling, change destination floor if not beyond decel marker of that floor
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
					if (FireServicePhase2 == 0 || UpPeak == true || DownPeak == true)
					{
						CloseDoors();
						WaitForDoors = true;
					}
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
					if (FireServicePhase2 == 0 || UpPeak == true || DownPeak == true)
					{
						CloseDoors();
						WaitForDoors = true;
					}
					MoveElevator = true;
					LastQueueDirection = -1;
				}
				else if (Leveling == false)
				{
					//if elevator is moving and not leveling, change destination floor if not beyond decel marker of that floor
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
					if (FireServicePhase2 == 0 || UpPeak == true || DownPeak == true)
					{
						CloseDoors();
						WaitForDoors = true;
					}
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

	//enable auto-park timer if specified
	if (ParkingDelay > 0)
	{
		if (timer->IsRunning() == false)
			timer->Start(ParkingDelay * 1000, true);
	}

	//elevator movement
	//if (MoveElevator == true && (AreDoorsOpen() == false || InspectionService == true))
	if (MoveElevator == true)
		MoveElevatorToFloor();

}

void Elevator::MoveElevatorToFloor()
{
	//Main processing routine; sends elevator to floor specified in GotoFloor
	//if InspectionService is enabled, this function ignores GotoFloor values, since the elevator is manually moved

	csVector3 movement = 0;

	//wait until doors are fully closed if WaitForDoors is true
	if (WaitForDoors == true)
	{
		if (AreDoorsOpen() == true || CheckOpenDoor() == true)
			return;
		else
			WaitForDoors = false;
	}

	if (ElevatorIsRunning == false)
	{
		if (sbs->Verbose)
			Report("starting elevator movement procedure");

		ElevatorIsRunning = true;
		FinishedMove = false;
		csString dir_string;

		//get elevator's current altitude
		elevposition = GetPosition();
		ElevatorStart = elevposition.y;

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

		ActiveDirection = Direction;

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
		if (sbs->ElevatorSync == true && sbs->ElevatorNumber == Number && InspectionService == false)
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

		//set interior directional indicators
		if (Direction == 1)
			SetDirectionalIndicators(true, false);
		else
			SetDirectionalIndicators(false, true);

		//set external active-direction indicators
		sbs->GetFloor(sbs->camera->CurrentFloor)->UpdateDirectionalIndicators(Number);

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
		bool adjust = sbs->confman->GetBool("Skyscraper.SBS.Elevator.AutoAdjustSound", false);
		//set play position to current percent of the total speed
		if (adjust == true)
			mainsound->SetPlayPosition(1 - (ElevatorRate / ElevatorSpeed));
		else
			mainsound->Reset();
		mainsound->Play(false);
		motorsound->Load(MotorStopSound.GetData());
		motorsound->Loop(false);
		if (adjust == true)
			motorsound->SetPlayPosition(1 - (ElevatorRate / ElevatorSpeed));
		else
			motorsound->Reset();
		motorsound->Play(false);
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
	movement.y = ElevatorRate * sbs->delta;

	Elevator_movable->MovePosition(csVector3(0, sbs->ToRemote(movement.y), 0));
	Elevator_movable->UpdateMove();
	elevposition = GetPosition();
	if (sbs->ElevatorSync == true && sbs->ElevatorNumber == Number)
		sbs->camera->SetPosition(csVector3(sbs->camera->GetPosition().x, elevposition.y + CameraOffset, sbs->camera->GetPosition().z));
	MoveDoors(0, movement, true, true, true);
	for (int i = 0; i < FloorIndicatorArray.GetSize(); i++)
	{
		if (FloorIndicatorArray[i])
			FloorIndicatorArray[i]->MovePosition(movement);
	}
	for (int i = 0; i < PanelArray.GetSize(); i++)
	{
		if (PanelArray[i])
			PanelArray[i]->Move(movement);
	}
	for (int i = 0; i < DirIndicatorArray.GetSize(); i++)
	{
		if (DirIndicatorArray[i])
			DirIndicatorArray[i]->Move(movement);
	}

	//move sounds
	mainsound->SetPosition(elevposition);
	idlesound->SetPosition(elevposition);
	MoveDoorSound(0, elevposition, false, false, false);
	alarm->SetPosition(elevposition);
	floorbeep->SetPosition(elevposition);
	floorsound->SetPosition(elevposition);
	for (int i = 0; i < sounds.GetSize(); i++)
	{
		if (sounds[i])
			sounds[i]->SetPositionY(elevposition.y + sounds[i]->PositionOffset.y);
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

		//regular motion
		float limit = 0;
		if (InspectionService == false)
			limit = ElevatorSpeed;
		else
			limit = ElevatorSpeed * 0.6;

		if (Direction == 1 && ElevatorRate < limit)
			ElevatorRate += ElevatorSpeed * ((Acceleration * JerkRate) * sbs->delta);
		else if (Direction == -1 && ElevatorRate > -limit)
			ElevatorRate -= ElevatorSpeed * ((Acceleration * JerkRate) * sbs->delta);
		else
			CalculateStoppingDistance = false;
	}
	else if (Leveling == false)
	{
		//slow down

		//calculate jerk rate
		//check if the elevator rate is less than the amount that was stored in JerkPos
		//(the elevator rate at the end of the JerkRate increments), adjusted to the ratio of acceljerk to deceljerk

		double tmppos = JerkPos * (AccelJerk / DecelJerk);
		if ((Direction == -1 && ElevatorRate <= tmppos) || (Direction == 1 && ElevatorRate >= tmppos))
		{
			if (tmpDecelJerk == 0)
				tmpDecelJerk = DecelJerk * (tmppos / ElevatorRate);
			JerkRate -= tmpDecelJerk * sbs->delta;
		}
		//prevent jerkrate from reaching 0
		if (JerkRate < 0)
		{
			JerkRate = 0;
			ElevatorRate = 0;
		}

		if (Direction == 1)
			ElevatorRate += ElevatorSpeed * ((TempDeceleration * JerkRate) * sbs->delta);
		if (Direction == -1)
			ElevatorRate -= ElevatorSpeed * ((TempDeceleration * JerkRate) * sbs->delta);
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
			StoppingDistance = (elevposition.y - ElevatorStart) * (Acceleration / Deceleration);
		else if (Direction == -1)
			StoppingDistance = (ElevatorStart - elevposition.y) * (Acceleration / Deceleration);
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
				//TempDeceleration = Deceleration;
				TempDeceleration = Deceleration * (StoppingDistance / ((Destination - LevelingOffset) - elevposition.y));
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
				//TempDeceleration = Deceleration;
				TempDeceleration = Deceleration * (StoppingDistance / (elevposition.y - (Destination + LevelingOffset)));
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
			bool adjust = sbs->confman->GetBool("Skyscraper.SBS.Elevator.AutoAdjustSound", false);
			mainsound->Load(CarStopSound.GetData());
			mainsound->Loop(false);
			//set play position to current percent of the total speed
			if (adjust == true)
				mainsound->SetPlayPosition(1 - (ElevatorRate / ElevatorSpeed));
			else
				mainsound->Reset();
			mainsound->Play(false);
			motorsound->Load(MotorStopSound.GetData());
			motorsound->Loop(false);
			if (adjust == true)
				motorsound->SetPlayPosition(1 - (ElevatorRate / ElevatorSpeed));
			else
				motorsound->Reset();
			motorsound->Play(false);
		}
	}
	else if (Leveling == false && EmergencyStop == false)
	{
		if (fabs(ElevatorRate) <= LevelingSpeed)
		{
			//turn on leveling if elevator's rate is less than or equal to the leveling speed value
			if (sbs->Verbose)
				Report("leveling enabled");
			Leveling = true;
		}
	}

	if (Leveling == true)
	{
		//floor leveling routine
		if (Direction == -1 && (Destination - elevposition.y) > 0)
			ElevatorRate = LevelingSpeed;
		else if (Direction == 1 && (elevposition.y - Destination) > 0)
			ElevatorRate = -LevelingSpeed;
		else
		{
			if (sbs->Verbose)
				Report("arrived at floor");
			ElevatorRate = 0; //stop if on floor
		}

		//open doors if leveling open offset is not zero
		if (LevelingOpen > 0 && FinishedMove == false)
		{
			if (Direction == -1 && (Destination - elevposition.y) < LevelingOpen)
				FinishMove();
			else if (Direction == 1 && (elevposition.y - Destination) < LevelingOpen)
				FinishMove();
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
			ErrorOffset = elevposition.y - Destination;
		else if (Direction == 1)
			ErrorOffset = Destination - elevposition.y;
		else
			ErrorOffset = 0;

		//set elevator and objects to floor altitude (corrects offset errors)
		//move elevator objects
		if (sbs->Verbose)
			Report("setting elevator to floor altitude");
		Elevator_movable->SetPosition(sbs->ToRemote(csVector3(elevposition.x, Destination, elevposition.z)));
		Elevator_movable->UpdateMove();
		if (sbs->ElevatorSync == true && sbs->ElevatorNumber == Number)
			sbs->camera->SetPosition(csVector3(sbs->camera->GetPosition().x, GetPosition().y + CameraOffset, sbs->camera->GetPosition().z));
		MoveDoors(0, csVector3(0, Destination, 0), true, false, true);
		for (int i = 0; i < FloorIndicatorArray.GetSize(); i++)
		{
			if (FloorIndicatorArray[i])
				FloorIndicatorArray[i]->SetPosition(csVector3(FloorIndicatorArray[i]->GetPosition().x, Destination, FloorIndicatorArray[i]->GetPosition().z));
		}
		for (int i = 0; i < PanelArray.GetSize(); i++)
		{
			if (PanelArray[i])
				PanelArray[i]->SetToElevatorAltitude();
		}
		for (int i = 0; i < DirIndicatorArray.GetSize(); i++)
		{
			if (DirIndicatorArray[i])
				DirIndicatorArray[i]->SetPosition(csVector3(DirIndicatorArray[i]->GetPosition().x, Destination, DirIndicatorArray[i]->GetPosition().z));
		}

		//move sounds
		mainsound->SetPosition(GetPosition());
		idlesound->SetPosition(GetPosition());
		MoveDoorSound(0, GetPosition(), false, false, false);
		alarm->SetPosition(GetPosition());
		floorbeep->SetPosition(GetPosition());
		floorsound->SetPosition(GetPosition());
		for (int i = 0; i < sounds.GetSize(); i++)
		{
			if (sounds[i])
				sounds[i]->SetPositionY(GetPosition().y + sounds[i]->PositionOffset.y);
		}
	}

	//reset values if at destination floor
finish:
	if (sbs->Verbose)
		Report("resetting elevator motion values");
	ElevatorRate = 0;
	JerkRate = 0;
	Direction = 0;
	ActiveDirection = 0;
	Brakes = false;
	Destination = 0;
	DistanceToTravel = 0;
	ElevatorStart = 0;
	ElevatorIsRunning = false;
	MoveElevator = false;
	IsMoving = false;
	Leveling = false;
	mainsound->Stop();
	motorsound->Stop();
	tmpDecelJerk = 0;

	if (FinishedMove == false)
		FinishMove();
}

void Elevator::FinishMove()
{
	//post-move operations, such as chimes, opening doors, indicator updates, etc

	if (EmergencyStop == false)
	{
		//the elevator is now stopped on a valid floor; set OnFloor to true
		OnFloor = true;
		Report("arrived at floor " + csString(_itoa(GotoFloor, intbuffer, 10)) + " (" + sbs->GetFloor(GotoFloor)->ID + ")");

		//dequeue floor route
		DeleteActiveRoute();
	}

	//turn off interior directional indicators
	SetDirectionalIndicators(false, false);

	//update external active-direction indicators
	sbs->GetFloor(sbs->camera->CurrentFloor)->UpdateDirectionalIndicators(Number);

	if (EmergencyStop == false && InspectionService == false)
	{
		//update floor indicators on current camera floor
		sbs->GetFloor(sbs->camera->CurrentFloor)->UpdateFloorIndicators(Number);

		//turn on objects if user is in elevator
		if (sbs->ElevatorSync == true && sbs->ElevatorNumber == Number && CameraOffset < Height)
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

			if (GotoFloor == GetTopFloor())
				LightDirection = false; //turn on down light if on top floor
			else if (GotoFloor == GetBottomFloor())
				LightDirection = true; //turn on up light if on bottom floor
			else if (QueuePositionDirection == 1)
				LightDirection = true; //turn on up light if queue direction is up
			else if (QueuePositionDirection == -1)
				LightDirection = false; //turn on down light if queue direction is down

			//play chime sound and change indicator
			if (LightDirection == true)
			{
				Chime(0, GotoFloor, true);
				sbs->GetFloor(GotoFloor)->SetDirectionalIndicators(Number, true, false);
			}
			else
			{
				Chime(0, GotoFloor, false);
				sbs->GetFloor(GotoFloor)->SetDirectionalIndicators(Number, false, true);
			}

			//disable call button lights
			SetCallButtons(GotoFloor, LightDirection, false);
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
	ElevatorFloor = GotoFloor;

	FinishedMove = true;
}

WallObject* Elevator::AddWall(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th)
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
	sbs->AddWallMain(wall, name, texture, thickness, x1, z1, x2, z2, height1, height2, voffset1, voffset2, sizing.x, sizing.y);
	return wall;
}

WallObject* Elevator::AddFloor(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, float tw, float th)
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
	sbs->AddFloorMain(wall, name, texture, thickness, x1, z1, x2, z2, voffset1, voffset2, tw2, th2);
	return wall;
}

Object* Elevator::AddFloorIndicator(const char *texture_prefix, const char *direction, float CenterX, float CenterZ, float width, float height, float voffset)
{
	//Creates a floor indicator at the specified location

	int size = FloorIndicatorArray.GetSize();
	FloorIndicatorArray.SetSize(size + 1);
	FloorIndicatorArray[size] = new FloorIndicator(Number, texture_prefix, direction, CenterX, CenterZ, width, height, voffset);
	FloorIndicatorArray[size]->SetPosition(Origin);
	return FloorIndicatorArray[size]->object;
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

	//interior directional indicators
	EnableDirectionalIndicators(value);

	//panels
	for (int i = 0; i < PanelArray.GetSize(); i++)
		PanelArray[i]->Enabled(value);

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
	//determine if the given 3D position is inside the elevator

	//if last position is the same as new, return previous result
	if (position == lastposition && checkfirstrun == false)
		return lastcheckresult;

	checkfirstrun = false;

	if (position.y > GetPosition().y && position.y < GetPosition().y + (Height * 2))
	{
		csHitBeamResult result = HitBeam(position, csVector3(position.x, position.y - Height, position.z));

		if (IsMoving == false && result.hit == true)
		{
			//store camera offset if elevator is not moving
			CameraOffset = position.y - GetPosition().y;
		}
		else if (result.hit == false)
			CameraOffset = 0;

		if (position.y < GetPosition().y + Height)
		{
			//cache values
			lastcheckresult = result.hit;
			lastposition = position;

			return result.hit;
		}
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

Object* Elevator::CreateButtonPanel(const char *texture, int rows, int columns, const char *direction, float CenterX, float CenterZ, float buttonwidth, float buttonheight, float spacingX, float spacingY, float voffset, float tw, float th)
{
	//create a new button panel object and store the pointer

	int index = PanelArray.GetSize();
	csString number;
	number = index + 1;
	number.Trim();
	PanelArray.SetSize(index + 1);

	if (sbs->Verbose)
		Report("creating button panel " + number);

	PanelArray[index] = new ButtonPanel(Number, index + 1, texture, rows, columns, direction, CenterX, CenterZ, buttonwidth, buttonheight, spacingX, spacingY, voffset, tw, th);
	if (PanelArray[index])
		return PanelArray[index]->object;
	else
		return 0;
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

double Elevator::GetJerkRate()
{
	return JerkRate;
}

double Elevator::GetJerkPosition()
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
	//go to specified floor, bypassing the queuing system

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
	WaitForDoors = true;
	CloseDoors();
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
		EnableDownPeak(false);
		EnableIndependentService(false);
		EnableInspectionService(false);
		EnableFireService1(0);
		EnableFireService2(0);
		if (IsMoving == false && GetFloor() == GetBottomFloor())
		{
			sbs->GetFloor(GetFloor())->SetDirectionalIndicators(Number, true, false);
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
		EnableUpPeak(false);
		EnableIndependentService(false);
		EnableInspectionService(false);
		EnableFireService1(0);
		EnableFireService2(0);
		if (IsMoving == false && GetFloor() == GetTopFloor())
		{
			sbs->GetFloor(GetFloor())->SetDirectionalIndicators(Number, false, true);
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

		//turn on objects if user is in elevator
		if (sbs->ElevatorSync == true && sbs->ElevatorNumber == Number)
		{
			if (sbs->Verbose)
				Report("user in elevator - turning on objects");

			UpdateFloorIndicators();

			//turn on floor
			sbs->GetFloor(GetFloor())->Enabled(true);
			sbs->GetFloor(GetFloor())->EnableGroup(true);

			//Turn on sky, buildings, and landscape
			sbs->EnableSkybox(true);
			sbs->EnableBuildings(true);
			sbs->EnableLandscape(true);
			sbs->EnableExternal(true);

			//reset shaft doors
			for (int i = 1; i <= sbs->Shafts(); i++)
			{
				sbs->GetShaft(i)->EnableRange(GetFloor(), sbs->ShaftDisplayRange, false, true);
				sbs->GetShaft(i)->EnableRange(GetFloor(), sbs->ShaftDisplayRange, true, true);
			}
		}
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

void Elevator::AddDirectionalIndicators(bool relative, bool active_direction, bool single, bool vertical, const char *BackTexture, const char *uptexture, const char *uptexture_lit, const char *downtexture, const char *downtexture_lit, float CenterX, float CenterZ, float voffset, const char *direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th)
{
	//create external directional indicators on all serviced floors

	if (sbs->Verbose)
		Report("adding directional indicators");

	for (size_t i = 0; i < ServicedFloors.GetSize(); i++)
		sbs->GetFloor(ServicedFloors[i])->AddDirectionalIndicator(Number, relative, active_direction, single, vertical, BackTexture, uptexture, uptexture_lit, downtexture, downtexture_lit, CenterX, CenterZ, voffset, direction, BackWidth, BackHeight, ShowBack, tw, th);
}

Object* Elevator::AddDirectionalIndicator(bool active_direction, bool single, bool vertical, const char *BackTexture, const char *uptexture, const char *uptexture_lit, const char *downtexture, const char *downtexture_lit, float CenterX, float CenterZ, float voffset, const char *direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th)
{
	//create a directional indicator inside the elevator

	if (sbs->Verbose)
		Report("adding interior directional indicator");

	float x = Origin.x + CenterX;
	float z = Origin.z + CenterZ;

	int index = DirIndicatorArray.GetSize();
	DirIndicatorArray.SetSize(index + 1);
	DirIndicatorArray[index] = new DirectionalIndicator(Number, 0, active_direction, single, vertical, BackTexture, uptexture, uptexture_lit, downtexture, downtexture_lit, x, z, voffset, direction, BackWidth, BackHeight, ShowBack, tw, th);
	return DirIndicatorArray[index]->object;
}

void Elevator::SetDirectionalIndicators(bool UpLight, bool DownLight)
{
	//set light status of interior directional indicators

	for (int i = 0; i < DirIndicatorArray.GetSize(); i++)
	{
		if (DirIndicatorArray[i])
		{
			DirIndicatorArray[i]->DownLight(DownLight);
			DirIndicatorArray[i]->UpLight(UpLight);
		}
	}
}

void Elevator::EnableDirectionalIndicators(bool value)
{
	//turn on/off all interior directional indicators

	if (sbs->Verbose)
	{
		if (value == true)
			Report("enabling interior directional indicators");
		else
			Report("disabling interior directional indicators");
	}

	for (int i = 0; i < DirIndicatorArray.GetSize(); i++)
	{
		if (DirIndicatorArray[i])
			DirIndicatorArray[i]->Enabled(value);
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

Object* Elevator::AddDoors(int number, const char *lefttexture, const char *righttexture, float thickness, float CenterX, float CenterZ, float width, float height, bool direction, float tw, float th)
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

Object* Elevator::AddShaftDoor(int floor, int number, const char *lefttexture, const char *righttexture, float tw, float th)
{
	//adds a single elevator shaft door on the specified floor, with position and thickness parameters first specified
	//by the SetShaftDoors command.

	int index = ServicedFloors.Find(floor);
	if (index != -1 && GetDoor(number))
		return GetDoor(number)->AddShaftDoor(floor, lefttexture, righttexture, tw, th);
	else
		return 0;
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

Object* Elevator::AddSound(const char *name, const char *filename, csVector3 position, int volume, int speed, float min_distance, float max_distance, float dir_radiation, csVector3 direction)
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

	return sound->object;
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

Object* Elevator::AddDoorComponent(int number, const char *name, const char *texture, const char *sidetexture, float thickness, const char *direction, float speed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th)
{
	//adds an elevator door component to the specified door at a relative central position (off of elevator origin)

	if (GetDoor(number))
		return GetDoor(number)->AddDoorComponent(name, texture, sidetexture, thickness, direction, speed, x1, z1, x2, z2, height, voffset, tw, th, side_tw, side_th);
	else
		Report("Invalid door " + csString(_itoa(number, intbuffer, 10)));
	return 0;
}

Object* Elevator::AddShaftDoorComponent(int number, int floor, const char *name, const char *texture, const char *sidetexture, float thickness, const char *direction, float speed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th)
{
	//adds a single elevator shaft door component on the specified floor

	int index = ServicedFloors.Find(floor);
	if (index != -1 && GetDoor(number))
		return GetDoor(number)->AddShaftDoorComponent(floor, name, texture, sidetexture, thickness, direction, speed, x1, z1, x2, z2, height, voffset, tw, th, side_tw, side_th);
	else
		return 0;
}

void Elevator::AddShaftDoorsComponent(int number, const char *name, const char *texture, const char *sidetexture, float thickness, const char *direction, float speed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th)
{
	//adds shaft's elevator door components specified at a relative central position (off of elevator origin)

	if (GetDoor(number))
		GetDoor(number)->AddShaftDoorsComponent(name, texture, sidetexture, thickness, direction, speed, x1, z1, x2, z2, height, voffset, tw, th, side_tw, side_th);
	else
		Report("Invalid door " + csString(_itoa(number, intbuffer, 10)));
}

Object* Elevator::FinishDoors(int number)
{
	//finishes elevator door
	if (GetDoor(number))
		return GetDoor(number)->FinishDoors();
	else
		Report("Invalid door " + csString(_itoa(number, intbuffer, 10)));
	return 0;
}

Object* Elevator::FinishShaftDoor(int number, int floor)
{
	//finishes a single shaft door
	int index = ServicedFloors.Find(floor);
	if (index != -1 && GetDoor(number))
		return GetDoor(number)->FinishShaftDoor(floor);
	else
		return 0;
}

bool Elevator::FinishShaftDoors(int number)
{
	//finishes all shaft doors
	if (GetDoor(number))
		return GetDoor(number)->FinishShaftDoors();
	else
		Report("Invalid door " + csString(_itoa(number, intbuffer, 10)));
	return false;
}

void Elevator::Timer::Notify()
{
	//parking timer

	if (elevator->ParkingDelay > 0)
	{
		int floor = elevator->GetFloor();
		if (elevator->ParkingFloor > floor)
			elevator->AddRoute(elevator->ParkingFloor, 1, false);
		else
			elevator->AddRoute(elevator->ParkingFloor, -1, false);
	}
}

ButtonPanel* Elevator::GetPanel(int index)
{
	//get a button panel object

	if (index > PanelArray.GetSize() || index < 1)
		return 0;

	return PanelArray[index - 1];
}
