/* $Id$ */

/*
	Scalable Building Simulator - Elevator Object
	The Skyscraper Project - Version 1.10 Alpha
	Copyright (C)2004-2015 Ryan Thoryk
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

#include <OgreBulletCollisionsRay.h>
#include "globals.h"
#include "random.h"
#include "sbs.h"
#include "elevator.h"
#include "camera.h"
#include "shaft.h"
#include "unix.h"

#include <time.h>

namespace SBS {

Elevator::Elevator(int number)
{
	//set up SBS object
	SetValues(sbs, "Elevator", "", false);

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
	AccelJerk = sbs->GetConfigFloat("Skyscraper.SBS.Elevator.AccelJerk", 1);
	DecelJerk = sbs->GetConfigFloat("Skyscraper.SBS.Elevator.DecelJerk", 1);
	ElevatorStart = 0;
	ElevatorFloor = 0;
	Direction = 0;
	DistanceToTravel = 0;
	Destination = 0;
	ElevatorRate = 0;
	StoppingDistance = 0;
	CalculateStoppingDistance = false;
	Brakes = false;
	EmergencyStop = 0;
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
	CarUpStartSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.CarUpStartSound", "");
	CarDownStartSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.CarDownStartSound", "");
	CarUpMoveSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.CarUpMoveSound", "");
	CarDownMoveSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.CarDownMoveSound", "");
	CarUpStopSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.CarUpStopSound", "");
	CarDownStopSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.CarDownStopSound", "");
	CarIdleSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.CarIdleSound", "elevidle.wav");
	MotorUpStartSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.MotorUpStartSound", "motor_start.wav");
	MotorUpRunSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.MotorUpRunSound", "motor_running.wav");
	MotorUpStopSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.MotorUpStopSound", "motor_stop.wav");
	MotorDownStartSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.MotorDownStartSound", "motor_start.wav");
	MotorDownRunSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.MotorDownRunSound", "motor_running.wav");
	MotorDownStopSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.MotorDownStopSound", "motor_stop.wav");
	MotorIdleSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.MotorIdleSound", "");
	AlarmSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.AlarmSound", "bell1.wav");
	AlarmSoundStop = sbs->GetConfigString("Skyscraper.SBS.Elevator.AlarmSoundStop", "bell1-stop.wav");
	UseFloorSkipText = false;
	ACP = sbs->GetConfigBool("Skyscraper.SBS.Elevator.ACP", false);
	ACPFloor = sbs->GetConfigInt("Skyscraper.SBS.Elevator.ACPFloor", 0);
	UpPeak = sbs->GetConfigBool("Skyscraper.SBS.Elevator.UpPeak", false);
	DownPeak = sbs->GetConfigBool("Skyscraper.SBS.Elevator.DownPeak", false);
	IndependentService = sbs->GetConfigBool("Skyscraper.SBS.Elevator.IndependentService", false);
	InspectionService = sbs->GetConfigBool("Skyscraper.SBS.Elevator.InspectionService", false);
	FireServicePhase1 = sbs->GetConfigInt("Skyscraper.SBS.Elevator.FireService1", 0);
	FireServicePhase2 = sbs->GetConfigInt("Skyscraper.SBS.Elevator.FireService2", 0);
	RecallFloor = 0;
	RecallFloorAlternate = 0;
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
	UseDirMessageSounds = false;
	UseDoorMessageSounds = false;
	MotorPosition = 0;
	ActiveCallFloor = 0;
	ActiveCallDirection = 0;
	ActiveCallType = 0;
	lastdoor_result = 0;
	lastdoor_number = 0;
	QueueResets = sbs->GetConfigBool("Skyscraper.SBS.Elevator.QueueResets", false);
	FirstRun = true;
	CameraOffset = 0;
	ParkingFloor = 0;
	ParkingDelay = 0;
	Leveling = false;
	LevelingSpeed = sbs->GetConfigFloat("Skyscraper.SBS.Elevator.LevelingSpeed", 0.2f);
	LevelingOffset = sbs->GetConfigFloat("Skyscraper.SBS.Elevator.LevelingOffset", 0.5f);
	LevelingOpen = sbs->GetConfigFloat("Skyscraper.SBS.Elevator.LevelingOpen", 0);
	tmpDecelJerk = 0;
	FinishedMove = false;
	WaitForDoors = false;
	ActiveDirection = 0;
	RandomActivity = sbs->GetConfigBool("Skyscraper.SBS.Elevator.RandomActivity", false);
	RandomProbability = sbs->GetConfigInt("Skyscraper.SBS.Elevator.RandomProbability", 10);
	RandomFrequency = sbs->GetConfigFloat("Skyscraper.SBS.Elevator.RandomFrequency", 3);
	RandomLobby = 0;
	RandomLobbySet = false;
	carsound = 0;
	idlesound = 0;
	motorsound = 0;
	motoridlesound = 0;
	alarm = 0;
	floorbeep = 0;
	StartingFloor = 0;
	Fan = true;
	NotifyEarly = sbs->GetConfigInt("Skyscraper.SBS.Elevator.NotifyEarly", 0);
	Running = sbs->GetConfigBool("Skyscraper.SBS.Elevator.Run", true);
	Notified = false;
	Parking = false;
	MusicPosition = 0;
	Music = sbs->GetConfigString("Skyscraper.SBS.Elevator.Music", "");
	MusicOn = sbs->GetConfigBool("Skyscraper.SBS.Elevator.MusicOn", true);
	MusicOnMove = sbs->GetConfigBool("Skyscraper.SBS.Elevator.MusicOnMove", false);
	DepartureDelay = sbs->GetConfigFloat("Skyscraper.SBS.Elevator.DepartureDelay", 0.0f);
	ArrivalDelay = sbs->GetConfigFloat("Skyscraper.SBS.Elevator.ArrivalDelay", 0.0f);
	WaitForTimer = false;
	SoundsQueued = false;
	HeightSet = false;
	announcesnd = 0;
	musicsound = 0;
	elevposition = 0;
	lastposition = 0;
	ManualUp = false;
	ManualDown = false;
	InspectionSpeed = sbs->GetConfigFloat("Skyscraper.SBS.Elevator.InspectionSpeed", 0.6f);
	LimitQueue = sbs->GetConfigBool("Skyscraper.SBS.Elevator.LimitQueue", false);
	UpQueueEmpty = false;
	DownQueueEmpty = false;
	UpCall = false;
	DownCall = false;
	QueuePending = false;
	AutoEnable = sbs->GetConfigBool("Skyscraper.SBS.Elevator.AutoEnable", true);
	ReOpen = sbs->GetConfigBool("Skyscraper.SBS.Elevator.ReOpen", true);
	LastChimeDirection = 0;
	AutoDoors = sbs->GetConfigBool("Skyscraper.SBS.Elevator.AutoDoors", true);
	OpenOnStart = sbs->GetConfigBool("Skyscraper.SBS.Elevator.OpenOnStart", false);
	ManualMove = 0;
	doorhold_direction = 0;
	doorhold_whichdoors = 0;
	doorhold_floor = 0;
	doorhold_manual = 0;
	Interlocks = sbs->GetConfigBool("Skyscraper.SBS.Elevator.Interlocks", true);
	GoActive = false;
	GoActiveFloor = 0;
	FloorHold = sbs->GetConfigBool("Skyscraper.SBS.Elevator.FloorHold", false);
	GoPending = false;
	EmergencyStopSpeed = sbs->GetConfigFloat("Skyscraper.SBS.Elevator.EmergencyStopSpeed", 3.0f);
	CarEmergencyStopSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.CarEmergencyStopSound", "");
	MotorEmergencyStopSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.MotorEmergencyStopSound", "");
	AutoAdjustSound = sbs->GetConfigBool("Skyscraper.SBS.Elevator.AutoAdjustSound", false);
	SkipFloorSound = false;
	DirMessageSound = false;
	DoorMessageSound = false;
	ControlPressActive = false;

	//create timers
	parking_timer = new Timer("Parking Timer", this, 0);
	random_timer = new Timer("Random Timer", this, 1);
	arrival_delay = new Timer("Arrival Delay Timer", this, 2);
	departure_delay = new Timer("Departure Delay Timer", this,3);

	//create object meshes
	std::string name = "Elevator " + ToString(Number);
	SetName(name);
	ElevatorMesh = new MeshObject(this, name);

	if (sbs->Verbose)
		Report("elevator object created");
}

Elevator::~Elevator()
{
	//delete controls
	for (int i = 0; i < (int)ControlArray.size(); i++)
	{
		if (ControlArray[i])
		{
			ControlArray[i]->parent_deleting = true;
			delete ControlArray[i];
		}
		ControlArray[i] = 0;
	}

	//delete triggers
	for (int i = 0; i < (int)TriggerArray.size(); i++)
	{
		if (TriggerArray[i])
		{
			TriggerArray[i]->parent_deleting = true;
			delete TriggerArray[i];
		}
		TriggerArray[i] = 0;
	}

	//delete models
	for (int i = 0; i < (int)ModelArray.size(); i++)
	{
		if (ModelArray[i])
		{
			ModelArray[i]->parent_deleting = true;
			delete ModelArray[i];
		}
		ModelArray[i] = 0;
	}

	//delete lights
	for (int i = 0; i < (int)lights.size(); i++)
	{
		if (lights[i])
		{
			lights[i]->parent_deleting = true;
			delete lights[i];
		}
		lights[i] = 0;
	}

	//delete timers
	if (sbs->Verbose)
		Report("deleting timers");

	if (parking_timer)
	{
		parking_timer->parent_deleting = true;
		delete parking_timer;
	}
	parking_timer = 0;

	if (random_timer)
	{
		random_timer->parent_deleting = true;
		delete random_timer;
	}
	random_timer = 0;

	if (arrival_delay)
	{
		arrival_delay->parent_deleting = true;
		delete arrival_delay;
	}
	arrival_delay = 0;

	if (departure_delay)
	{
		departure_delay->parent_deleting = true;
		delete departure_delay;
	}
	departure_delay = 0;

	//delete directional indicators
	if (sbs->Verbose)
		Report("deleting interior directional indicators");

	for (int i = 0; i < (int)DirIndicatorArray.size(); i++)
	{
		if (DirIndicatorArray[i])
		{
			DirIndicatorArray[i]->parent_deleting = true;
			delete DirIndicatorArray[i];
		}
		DirIndicatorArray[i] = 0;
	}

	//delete doors
	if (sbs->Verbose)
		Report("deleting doors");

	if (DoorArray.size() > 0)
	{
		for (int i = 0; i < (int)DoorArray.size(); i++)
		{
			if (DoorArray[i])
			{
				DoorArray[i]->parent_deleting = true;
				delete DoorArray[i];
			}
			DoorArray[i] = 0;
		}
	}

	//delete floor indicators
	if (sbs->Verbose)
		Report("deleting floor indicators");

	for (int i = 0; i < (int)FloorIndicatorArray.size(); i++)
	{
		if (FloorIndicatorArray[i])
		{
			FloorIndicatorArray[i]->parent_deleting = true;
			delete FloorIndicatorArray[i];
		}
		FloorIndicatorArray[i] = 0;
	}

	//delete panels
	if (sbs->Verbose)
		Report("deleting button panels");

	for (int i = 0; i < (int)PanelArray.size(); i++)
	{
		if (PanelArray[i])
		{
			PanelArray[i]->parent_deleting = true;
			delete PanelArray[i];
		}
		PanelArray[i] = 0;
	}

	//delete doors
	if (sbs->Verbose)
		Report("deleting standard doors");

	for (int i = 0; i < (int)StdDoorArray.size(); i++)
	{
		if (StdDoorArray[i])
		{
			StdDoorArray[i]->parent_deleting = true;
			delete StdDoorArray[i];
		}
		StdDoorArray[i] = 0;
	}

	//Destructor
	if (sbs->Verbose)
		Report("deleting objects");
	if (carsound)
	{
		carsound->parent_deleting = true;
		delete carsound;
	}
	carsound = 0;
	if (alarm)
	{
		alarm->parent_deleting = true;
		delete alarm;
	}
	alarm = 0;
	if (floorbeep)
	{
		floorbeep->parent_deleting = true;
		delete floorbeep;
	}
	floorbeep = 0;
	if (motorsound)
	{
		motorsound->parent_deleting = true;
		delete motorsound;
	}
	motorsound = 0;
	if (idlesound)
	{
		idlesound->parent_deleting = true;
		delete idlesound;
	}
	idlesound = 0;
	if (motoridlesound)
	{
		motoridlesound->parent_deleting = true;
		delete motoridlesound;
	}
	motoridlesound = 0;
	if (announcesnd)
	{
		announcesnd->parent_deleting = true;
		delete announcesnd;
	}
	announcesnd = 0;
	if (musicsound)
	{
		musicsound->parent_deleting = true;
		delete musicsound;
	}
	musicsound = 0;

	//delete sounds
	if (sbs->Verbose)
		Report("deleting sounds");

	for (int i = 0; i < (int)sounds.size(); i++)
	{
		if (sounds[i])
		{
			sounds[i]->parent_deleting = true;
			delete sounds[i];
		}
		sounds[i] = 0;
	}

	if (ElevatorMesh)
	{
		ElevatorMesh->parent_deleting = true;
		delete ElevatorMesh;
	}
	ElevatorMesh = 0;

	//unregister from parent
	if (sbs->FastDelete == false && parent_deleting == false)
		sbs->RemoveElevator(this);
}

bool Elevator::CreateElevator(bool relative, float x, float z, int floor)
{
	//Creates elevator at specified location and floor
	//x and z are the center coordinates
	//if relative is true, then x and z coordinates are relative
	//to the assigned shaft's center

	if (Created == true)
		return ReportError("Has already been created");

	//make sure required values are set
	if (ElevatorSpeed <= 0)
		return ReportError("Speed not set or invalid");

	if (Acceleration <= 0)
		return ReportError("Acceleration not set or invalid");

	if (Deceleration <= 0)
		return ReportError("Deceleration not set or invalid");

	if (NumDoors < 0)
		return ReportError("Number of doors invalid");

	if (AccelJerk <= 0)
		return ReportError("Invalid value for AccelJerk");

	if (DecelJerk <= 0)
		return ReportError("Invalid value for DecelJerk");

	if (AssignedShaft <= 0)
		return ReportError("Not assigned to a shaft");

	if (!GetShaft())
		return ReportError("Shaft " + ToString(AssignedShaft) + " doesn't exist");

	if (floor < GetShaft()->startfloor || floor > GetShaft()->endfloor)
		return ReportError("Invalid starting floor " + ToString(floor));

	//add elevator's starting floor to serviced floor list - this also ensures that the list is populated to prevent errors
	if (IsServicedFloor(floor) == false)
		AddServicedFloor(floor);

	//ensure that serviced floors are valid for the shaft
	for (int i = 0; i < (int)ServicedFloors.size(); i++)
	{
		if (GetShaft()->IsValidFloor(ServicedFloors[i]) == false)
		{
			std::string snum = ToString(AssignedShaft);
			std::string num = ToString(ServicedFloors[i]);
			return ReportError("Floor " + num + " not valid for shaft " + snum);
		}
	}

	//set data
	if (!sbs->GetFloor(floor))
		return ReportError("Floor " + ToString(floor) + " doesn't exist");

	//set starting position
	Ogre::Vector3 position = Ogre::Vector3::ZERO;

	position.y = sbs->GetFloor(floor)->GetBase();
	if (relative == false)
	{
		position.x = x;
		position.z = z;
	}
	else
	{
		position.x = GetShaft()->GetPosition().x + x;
		position.z = GetShaft()->GetPosition().z + z;
	}
	StartingFloor = floor;

	//add elevator to associated shaft's list
	GetShaft()->AddElevator(Number);

	//set recall/ACP floors if not already set
	if (RecallSet == false)
		SetRecallFloor(GetBottomFloor());
	if (RecallAltSet == false)
		SetAlternateRecallFloor(GetTopFloor());
	if (ACPFloorSet == false)
		SetACPFloor(GetBottomFloor());

	//move objects to positions
	if (sbs->Verbose)
		Report("moving elevator to origin position");
	SetPosition(position);
	elevposition = GetPosition();

	//create door objects
	if (sbs->Verbose)
		Report("creating doors");
	if (NumDoors > 0)
	{
		for (int i = 1; i <= NumDoors; i++)
			DoorArray.push_back(new ElevatorDoor(i, this));
	}

	//create sound objects
	if (sbs->Verbose)
		Report("creating sound objects");
	carsound = new Sound(this, "Car", true);
	idlesound = new Sound(this, "Idle", true);

	std::string motorname = "Motor " + ToString(Number);
	motorsound = new Sound(GetShaft(), motorname, true);
	motorname += " Idle";
	motoridlesound = new Sound(GetShaft(), motorname, true);

	//move motor to top of shaft if location not specified, or to location
	if (MotorPosition != Ogre::Vector3(0, 0, 0))
		motorsound->SetPosition(MotorPosition.x + GetPosition().x, MotorPosition.y, MotorPosition.z + GetPosition().z);
	else
	{
		Floor *floor = sbs->GetFloor(GetShaft()->endfloor);
		if (floor)
			motorsound->SetPositionY(floor->GetBase());
	}
	MotorPosition = Ogre::Vector3(motorsound->GetPosition().x - GetPosition().x, motorsound->GetPosition().y, motorsound->GetPosition().z - GetPosition().z);
	motoridlesound->SetPosition(motorsound->GetPosition());
	alarm = new Sound(this, "Alarm", true);
	floorbeep = new Sound(this, "Floor Beep", true);
	announcesnd = new Sound(this, "Announcement Sound", true);
	musicsound = new Sound(this, "Music Sound", true);
	musicsound->Move(MusicPosition);

	//set elevator's floor
	ElevatorFloor = floor;

	//create test light
	//AddLight("light", 0, Ogre::Vector3(0, 6, 0), Ogre::Vector3(0, 0, 0), 1, 1, 1, 1, 1, 1, 0, 0, 0, 1000, 1, 1, 1);

	Created = true;

	Report("created at " + sbs->TruncateNumber(position.x, 4) + ", " + sbs->TruncateNumber(position.z, 4) + ", " + ToString(floor));
	return true;
}

bool Elevator::AddRoute(int floor, int direction, int call_type)
{
	//Add call route to elevator routing table, in sorted order
	//directions are either 1 for up, or -1 for down
	//call type is 0 for a car call, 1 for a hall call, and 2 for a system call

	if (Running == false)
		return ReportError("Elevator not running");

	Floor *floorobj = sbs->GetFloor(floor);

	if (!floorobj)
		return ReportError("Invalid floor " + ToString(floor));

	//if doors are open or moving in independent service mode, quit
	if (IndependentService == true && (AreDoorsOpen() == false || AreDoorsMoving() != 0))
		return ReportError("floor button must be pressed before closing doors while in independent service");

	//do not add routes if in inspection service or fire phase 1 modes
	if (InspectionService == true)
		return ReportError("cannot add route while in inspection service mode");

	if (FireServicePhase2 == 2)
		return ReportError("cannot add route while hold is enabled");

	//discard route if direction opposite queue search direction
	if (LimitQueue == true && direction != QueuePositionDirection && QueuePositionDirection != 0)
	{
		//only allow if any queue entries exist
		if ((QueuePositionDirection == 1 && UpQueue.size() > 0) || (QueuePositionDirection == -1 && DownQueue.size() > 0))
			return ReportError("cannot add route in opposite direction of queue search");
	}

	//add route in related direction queue
	if (direction == 1)
	{
		int loc = -1;
		for (int i = 0; i < (int)UpQueue.size(); i++)
		{
			if (UpQueue[i].floor == floor)
			{
				loc = i;
				break;
			}
		}

		if (loc != -1)
		{
			//exit if entry already exits
			return ReportError("route to floor " + ToString(floor) + " (" + floorobj->ID + ") already exists");
		}

		//add floor to up queue
		UpQueue.push_back(QueueEntry(floor, call_type));
		std::sort(UpQueue.begin(), UpQueue.end());
		QueuePending = true;

		LastQueueFloor[0] = floor;
		LastQueueFloor[1] = 1;
		Report("adding route to floor " + ToString(floor) + " (" + floorobj->ID + ") direction up");
	}
	else
	{
		int loc = -1;
		for (int i = 0; i < (int)DownQueue.size(); i++)
		{
			if (DownQueue[i].floor == floor)
			{
				loc = i;
				break;
			}
		}

		//exit if entry already exits
		if (loc != -1)
			return ReportError("route to floor " + ToString(floor) + " (" + floorobj->ID + ") already exists");

		//add floor to down queue
		DownQueue.push_back(QueueEntry(floor, call_type));
		std::sort(DownQueue.begin(), DownQueue.end());
		QueuePending = true;

		LastQueueFloor[0] = floor;
		LastQueueFloor[1] = -1;
		Report("adding route to floor " + ToString(floor) + " (" + floorobj->ID + ") direction down");
	}

	//turn on button lights
	if (call_type == 0)
		ChangeLight(floor, true);

	//go to ACP floor if ACP mode is enabled
	if (ACP == true && floor != ACPFloor)
	{
		//only add ACP route if original route will pass ACP floor
		if ((GetFloor() < ACPFloor && floor > ACPFloor) || (GetFloor() > ACPFloor && floor < ACPFloor))
		{
			Report("adding ACP route");
			AddRoute(ACPFloor, direction, 2);
		}
	}

	return true;
}

bool Elevator::DeleteRoute(int floor, int direction)
{
	//Delete call route from elevator routing table
	//directions are either 1 for up, or -1 for down

	if (Running == false)
		return ReportError("Elevator not running");

	Floor *floorobj = sbs->GetFloor(floor);

	if (!floorobj)
		return ReportError("Invalid floor " + ToString(floor));

	if (direction == 1)
	{
		//delete floor entry from up queue
		for (int i = 0; i < (int)UpQueue.size(); i++)
		{
			if (UpQueue[i].floor == floor)
			{
				Report("deleting route to floor " + ToString(floor) + " (" + floorobj->ID + ") direction up");
				UpQueue.erase(UpQueue.begin() + i);
				break;
			}
		}
		if (UpQueue.size() == 0)
			UpQueueEmpty = true;
	}
	else
	{
		//delete floor entry from down queue
		for (int i = 0; i < (int)DownQueue.size(); i++)
		{
			if (DownQueue[i].floor == floor)
			{
				Report("deleting route to floor " + ToString(floor) + " (" + floorobj->ID + ") direction down");
				DownQueue.erase(DownQueue.begin() + i);
				break;
			}
		}
		if (DownQueue.size() == 0)
			DownQueueEmpty = true;
	}

	//turn off button lights
	if (sbs->Verbose)
		Report("DeleteRoute: turning off button lights for floor " + ToString(floor));
	ChangeLight(floor, false);
	return true;
}

bool Elevator::CallCancel()
{
	//cancels the last added route
	//LastQueueFloor holds the floor and direction of the last route; array element 0 is the floor and 1 is the direction

	if (Running == false)
		return ReportError("Elevator not running");

	if (LastQueueFloor[1] == 0)
	{
		if (sbs->Verbose)
			ReportError("CallCancel: route not valid");
		return false;
	}

	Report("cancelled last call");
	DeleteRoute(LastQueueFloor[0], LastQueueFloor[1]);
	LastQueueFloor[0] = 0;
	LastQueueFloor[1] = 0;
	return true;
}

void Elevator::Alarm()
{
	//elevator alarm code

	if (AlarmActive == false)
	{
		//ring alarm
		AlarmActive = true;
		Report("alarm on");
		if (AlarmSound != "")
		{
			alarm->Load(AlarmSound);
			alarm->SetLoopState(true);
			alarm->Play();
		}
	}
	else if (AlarmActive == true && sbs->camera->MouseDown == false)
	{
		//stop alarm
		AlarmActive = false;
		if (AlarmSound != "")
		{
			alarm->Stop();
			alarm->Load(AlarmSoundStop);
			alarm->SetLoopState(false);
			alarm->Play();
		}
		Report("alarm off");
	}
}

bool Elevator::Stop(bool emergency)
{
	//Tells elevator to stop moving, no matter where it is

	//exit if in inspection mode
	if (InspectionService == true && emergency == true)
		return ReportError("cannot stop while in inspection service mode");

	//exit if in fire service phase 1 recall
	if (FireServicePhase1 == 1 && FireServicePhase2 == 0)
		return ReportError("cannot stop while in fire service 1 recall mode");

	if (IsMoving == false)
	{
		if (sbs->Verbose)
			ReportError("Stop: not moving");
		return false;
	}

	if (emergency == true)
	{
		EmergencyStop = 2;
		Report("emergency stop");

		//clear elevator queues
		ResetQueue(true, true);
	}
	else
	{
		EmergencyStop = 1;
		Report("stopping elevator");
	}
	return true;
}

void Elevator::OpenHatch()
{
	//Opens the elevator's upper escape hatch, allowing access to the shaft

	Report("opening hatch");
}

void Elevator::ProcessCallQueue()
{
	//Processes the elevator's call queue, and sends elevators to called floors
	SBS_PROFILE("Elevator::ProcessCallQueue");

	//exit if elevator is not running
	if (Running == false)
		return;

	//exit if in inspection service mode
	if (InspectionService == true)
		return;

	//exit if stopping
	if (EmergencyStop > 0)
		return;

	//exit if moving manually
	if (ManualMove > 0)
		return;

	//exit if Go function is active
	if (GoPending == true)
		return;

	//if both queues are empty
	if (UpQueue.size() == 0 && DownQueue.size() == 0)
	{
		int TopFloor = GetTopFloor();
		int BottomFloor = GetBottomFloor();

		UpQueueEmpty = false;
		DownQueueEmpty = false;

		if (DownPeak == true || UpPeak == true)
		{
			//if DownPeak mode is active, send elevator to the top serviced floor if not already there
			if (GetFloor() != TopFloor && DownPeak == true && IsMoving == false)
			{
				if (sbs->Verbose)
					Report("ProcessCallQueue: sending elevator to top floor for DownPeak mode");
				AddRoute(TopFloor, 1, 2);
				return;
			}
			//if UpPeak mode is active, send elevator to the bottom serviced floor if not already there
			else if (GetFloor() != BottomFloor && UpPeak == true && IsMoving == false)
			{
				if (sbs->Verbose)
					Report("ProcessCallQueue: sending elevator to bottom floor for UpPeak mode");
				AddRoute(BottomFloor, -1, 2);
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
		if (UpQueue.size() != 0)
		{
			if (sbs->Verbose)
				Report("ProcessCallQueue: setting search direction to up");
			QueuePositionDirection = 1;
		}
		else if (DownQueue.size() != 0)
		{
			if (sbs->Verbose)
				Report("ProcessCallQueue: setting search direction to down");
			QueuePositionDirection = -1;
		}
		LastQueueDirection = 0;
	}

	//reverse queues if related queue empty flag is set
	if (QueuePositionDirection == 1 && UpQueueEmpty == true && DownQueue.size() > 0 && NotifyEarly == 0)
	{
		if (UpCall == false)
		{
			if (sbs->Verbose)
				Report("ProcessCallQueue: setting search direction to down");
			LastQueueDirection = QueuePositionDirection;
			QueuePositionDirection = -1;
		}
	}
	if (QueuePositionDirection == -1 && DownQueueEmpty == true && UpQueue.size() > 0 && NotifyEarly == 0)
	{
		if (DownCall == false)
		{
			if (sbs->Verbose)
				Report("ProcessCallQueue: setting search direction to up");
			LastQueueDirection = QueuePositionDirection;
			QueuePositionDirection = 1;
		}
	}

	UpQueueEmpty = false;
	DownQueueEmpty = false;
	UpCall = false;
	DownCall = false;

	//set search direction to 0 if any related queue is empty, and if doors are not open or moving
	if (AreDoorsOpen() == false && AreDoorsMoving() == 0)
	{
		if (QueuePositionDirection == 1 && UpQueue.size() == 0)
		{
			if (sbs->Verbose)
				Report("ProcessCallQueue: resetting search direction due to empty up queue");
			QueuePositionDirection = 0;
			LastQueueDirection = 1;
		}
		if (QueuePositionDirection == -1 && DownQueue.size() == 0)
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
		for (int i = 0; i < (int)UpQueue.size(); i++)
		{
			//if the queued floor number is a higher floor, dispatch the elevator to that floor
			if (UpQueue[i].floor >= ElevatorFloor)
			{
				if (MoveElevator == false)
				{
					if (sbs->Verbose)
						Report("ProcessCallQueue up: standard dispatch, floor " + ToString(UpQueue[i].floor));
					ActiveCallFloor = UpQueue[i].floor;
					ActiveCallType = UpQueue[i].call_type;
					ActiveCallDirection = 1;
					GotoFloor = UpQueue[i].floor;
					if (FireServicePhase2 == 0 || UpPeak == true || DownPeak == true)
					{
						CloseDoors();
						WaitForDoors = true;
					}
					MoveElevator = true;
					LastQueueDirection = 1;
					QueuePending = false;
				}
				else if (Leveling == false && ActiveDirection == 1)
				{
					//if elevator is moving and not leveling, change destination floor if not beyond decel marker of that floor
					if (GotoFloor != UpQueue[i].floor)
					{
						float tmpdestination = GetDestinationAltitude(UpQueue[i].floor);
						if (BeyondDecelMarker(1, tmpdestination) == false && sbs->GetFloor(GotoFloor))
						{
							ActiveCallFloor = UpQueue[i].floor;
							ActiveCallType = UpQueue[i].call_type;
							GotoFloor = UpQueue[i].floor;
							Destination = tmpdestination;
							Report("changing destination floor to " + ToString(GotoFloor) + " (" + sbs->GetFloor(GotoFloor)->ID + ")");
						}
						else if (sbs->Verbose)
							Report("ProcessCallQueue up: cannot change destination floor to " + ToString(UpQueue[i].floor));
					}
				}
				return;
			}
			//if the queued floor number is a lower floor
			if (UpQueue[i].floor < ElevatorFloor && MoveElevator == false)
			{
				//dispatch elevator if it's idle
				if (IsIdle() == true && LastQueueDirection == 0)
				{
					if (sbs->Verbose)
						Report("ProcessCallQueue up: dispatching idle lower elevator, floor " + ToString(UpQueue[i].floor));
					ActiveCallFloor = UpQueue[i].floor;
					ActiveCallType = UpQueue[i].call_type;
					ActiveCallDirection = 1;
					GotoFloor = UpQueue[i].floor;
					if (FireServicePhase2 == 0 || UpPeak == true || DownPeak == true)
					{
						CloseDoors();
						WaitForDoors = true;
					}
					MoveElevator = true;
					LastQueueDirection = 1;
					QueuePending = false;
					return;
				}
				//reset search direction if it's the last entry and idle
				if (i == UpQueue.size() - 1 && IsIdle() == true && QueuePositionDirection != 0)
				{
					if (sbs->Verbose)
						Report("ProcessCallQueue up: resetting search direction since last entry is lower");
					LastQueueDirection = QueuePositionDirection;
					QueuePositionDirection = 0;
					return;
				}
				//otherwise skip it if it's not the last entry
				if (sbs->Verbose)
					Report("ProcessCallQueue up: skipping floor entry " + ToString(UpQueue[i].floor));
			}
		}
	}
	else if (QueuePositionDirection == -1)
	{
		//search through down queue (search order is reversed since calls need to be processed in descending order)
		for (int i = (int)DownQueue.size() - 1; i >= 0; i--)
		{
			//if the queued floor number is a lower floor, dispatch the elevator to that floor
			if (DownQueue[i].floor <= ElevatorFloor)
			{
				if (MoveElevator == false)
				{
					if (sbs->Verbose)
						Report("ProcessCallQueue down: standard dispatch, floor " + ToString(DownQueue[i].floor));
					ActiveCallFloor = DownQueue[i].floor;
					ActiveCallType = DownQueue[i].call_type;
					ActiveCallDirection = -1;
					GotoFloor = DownQueue[i].floor;
					if (FireServicePhase2 == 0 || UpPeak == true || DownPeak == true)
					{
						CloseDoors();
						WaitForDoors = true;
					}
					MoveElevator = true;
					LastQueueDirection = -1;
					QueuePending = false;
				}
				else if (Leveling == false && ActiveDirection == -1)
				{
					//if elevator is moving and not leveling, change destination floor if not beyond decel marker of that floor
					if (GotoFloor != DownQueue[i].floor)
					{
						float tmpdestination = GetDestinationAltitude(DownQueue[i].floor);
						if (BeyondDecelMarker(-1, tmpdestination) == false && sbs->GetFloor(GotoFloor))
						{
							ActiveCallFloor = DownQueue[i].floor;
							ActiveCallType = DownQueue[i].call_type;
							GotoFloor = DownQueue[i].floor;
							Destination = tmpdestination;
							Report("changing destination floor to " + ToString(GotoFloor) + " (" + sbs->GetFloor(GotoFloor)->ID + ")");
						}
						else if (sbs->Verbose)
							Report("ProcessCallQueue down: cannot change destination floor to " + ToString(DownQueue[i].floor));
					}
				}
				return;
			}
			//if the queued floor number is an upper floor
			if (DownQueue[i].floor > ElevatorFloor && MoveElevator == false)
			{
				//dispatch elevator if idle
				if (IsIdle() == true && LastQueueDirection == 0)
				{
					if (sbs->Verbose)
						Report("ProcessCallQueue down: dispatching idle higher elevator, floor " + ToString(DownQueue[i].floor));
					ActiveCallFloor = DownQueue[i].floor;
					ActiveCallType = DownQueue[i].call_type;
					ActiveCallDirection = -1;
					GotoFloor = DownQueue[i].floor;
					if (FireServicePhase2 == 0 || UpPeak == true || DownPeak == true)
					{
						CloseDoors();
						WaitForDoors = true;
					}
					MoveElevator = true;
					LastQueueDirection = -1;
					QueuePending = false;
					return;
				}
				//reset search direction if it's the last entry and idle
				if (i == 0 && IsIdle() == true && QueuePositionDirection != 0)
				{
					if (sbs->Verbose)
						Report("ProcessCallQueue down: resetting search direction since last entry is higher");
					LastQueueDirection = QueuePositionDirection;
					QueuePositionDirection = 0;
					return;
				}
				//otherwise skip it if it's not the last entry
				if (sbs->Verbose)
					Report("ProcessCallQueue down: skipping floor entry " + ToString(DownQueue[i].floor));
			}
		}
	}
}

int Elevator::GetFloor()
{
	//Determine floor that the elevator is on

	//SBS_PROFILE("Elevator::GetFloor");

	int newlastfloor;

	if (lastfloorset == true)
		newlastfloor = sbs->GetFloorNumber(GetPosition().y, lastfloor, true);
	else
		newlastfloor = sbs->GetFloorNumber(GetPosition().y);

	lastfloor = newlastfloor;
	lastfloorset = true;
	return lastfloor;
}

void Elevator::Loop()
{
	//Monitors elevator and starts actions if needed

	SBS_PROFILE("Elevator::Loop");

	if (Created == false)
		return;

	ControlPressActive = false;

	//make sure height value is set
	if (HeightSet == false)
	{
		Height = 0;
		//search through mesh geometry to find actual height
		for (int i = 0; i < (int)ElevatorMesh->MeshGeometry.size(); i++)
		{
			//set height value
			if (sbs->ToLocal(ElevatorMesh->MeshGeometry[i].vertex.y) > Height)
				Height = sbs->ToLocal(ElevatorMesh->MeshGeometry[i].vertex.y);
		}
		HeightSet = true;

		//position sounds at top of elevator car
		Ogre::Vector3 top = Ogre::Vector3(0, Height, 0);
		idlesound->SetPositionRelative(top);
		alarm->SetPositionRelative(top);
		floorbeep->SetPositionRelative(top);
		announcesnd->SetPositionRelative(top);

		//set default music position to elevator height
		if (MusicPosition == Ogre::Vector3(0, 0, 0) && Height > 0)
			MusicPosition = top;
		musicsound->SetPositionRelative(MusicPosition);
	}

	//set random lobby level if not set
	if (RandomLobbySet == false)
		SetRandomLobby(GetBottomFloor());

	//perform first-run tasks
	if (FirstRun == true && Running == true)
	{
		FirstRun = false;

		if (UpPeak == true)
		{
			UpPeak = false;
			EnableUpPeak(true);
		}
		if (DownPeak == true)
		{
			DownPeak = false;
			EnableDownPeak(true);
		}
		if (IndependentService == true)
		{
			IndependentService = false;
			EnableIndependentService(true);
		}
		if (InspectionService == true)
		{
			InspectionService = false;
			EnableInspectionService(true);
		}
		if (FireServicePhase1 > 0)
		{
			int value = FireServicePhase1;
			FireServicePhase1 = 0;
			EnableFireService1(value);
		}
		if (FireServicePhase2 > 0)
		{
			int value = FireServicePhase2;
			FireServicePhase2 = 0;
			EnableFireService2(value);
		}
		if (ACP == true)
		{
			ACP = false;
			EnableACP(true);
		}
		if (ACPFloor != 0)
		{
			int tmp = ACPFloor;
			ACPFloor = 0;
			SetACPFloor(tmp);
		}
		if (OpenOnStart == true)
			OpenDoors();

		UpdateFloorIndicators();
	}

	//play car idle sound if in elevator, or if doors open
	if (CarIdleSound != "")
	{
		if (idlesound->IsLoaded() == false)
			idlesound->Load(CarIdleSound);

		if (idlesound->IsPlaying() == false && Fan == true)
		{
			if (InElevator() == true || AreDoorsOpen() == true || AreDoorsMoving(0, true, false) != 0)
			{
				if (sbs->Verbose)
					Report("playing car idle sound");
				idlesound->SetLoopState(true);
				idlesound->Play();
			}
		}
		else
		{
			if (Fan == false && idlesound->IsPlaying() == true)
			{
				if (sbs->Verbose)
					Report("stopping car idle sound");
				idlesound->Stop();
			}
			else if (InElevator() == false && AreDoorsOpen() == false && AreDoorsMoving() == 0)
			{
				if (sbs->Verbose)
					Report("stopping car idle sound");
				idlesound->Stop();
			}
		}
	}

	if (MotorIdleSound != "")
	{
		if (motoridlesound->IsLoaded() == false)
			motoridlesound->Load(MotorIdleSound);

		//play motor idle sound
		if (motoridlesound->IsPlaying() == false && Running == true)
		{
			if (sbs->Verbose)
				Report("playing motor idle sound");
			motoridlesound->SetLoopState(true);
			motoridlesound->Play();
		}

		//stop motor sound if elevator is stopped and not running
		if (motoridlesound->IsPlaying() == true && Running == false)
		{
			if (sbs->Verbose)
				Report("stopping motor idle sound");
			motoridlesound->Stop();
		}
	}

	//play music sound if in elevator, or if doors open
	if (Music != "")
	{
		if (musicsound->IsLoaded() == false)
			musicsound->Load(Music);

		if (musicsound->IsPlaying() == false && MusicOn == true && ((MusicOnMove == true && IsMoving == true) || MusicOnMove == false))
		{
			if (InServiceMode() == false)
			{
				if (InElevator() == true || AreDoorsOpen() == true || AreDoorsMoving() != 0)
				{
					if (sbs->Verbose)
						Report("playing music");

					musicsound->SetLoopState(true);
					musicsound->Play(false);
				}
			}
		}
		else
		{
			if ((MusicOn == false || InServiceMode() == true || (MusicOnMove == true && IsMoving == false)) && musicsound->IsPlaying() == true)
			{
				if (sbs->Verbose)
					Report("stopping music");
				musicsound->Pause();
			}
			else if (InElevator() == false && AreDoorsOpen() == false && AreDoorsMoving() == 0)
			{
				if (sbs->Verbose)
					Report("stopping music");
				musicsound->Pause();
			}
		}
	}

	//process alarm
	if (AlarmActive == true)
		Alarm();

	//process up/down buttons
	if (ManualMove == 1)
		Up();
	if (ManualMove == -1)
		Down();

	//process door open/close holds
	if (doorhold_direction > 0)
		OpenDoors();
	if (doorhold_direction < 0)
		CloseDoors();

	//process Go function hold
	if (GoActive == true)
		Go(GoActiveFloor, true);

	//call queue processor
	ProcessCallQueue();

	//door operations
	for (int i = 1; i <= NumDoors; i++)
	{
		ElevatorDoor *door = GetDoor(i);
		if (door)
			door->Loop();

		//reset door timer if peak mode is enabled and a movement is pending
		if ((UpPeak == true || DownPeak == true))
		{
			if ((UpQueue.size() != 0 || DownQueue.size() != 0) && (AreDoorsOpen() == true && AreDoorsMoving() == 0))
			{
				if (door)
				{
					if (door->TimerIsRunning() == false)
						door->Reset();
				}
			}
		}
	}

	//enable auto-park timer if specified
	if (parking_timer->IsRunning() == false && ParkingDelay > 0 && Running == true && IsIdle() == true && InServiceMode() == false && AutoDoors == true)
		parking_timer->Start(int(ParkingDelay * 1000), true);

	//enable random call timer
	if (random_timer->IsRunning() == false && RandomActivity == true && Running == true && InServiceMode() == false && AutoDoors == true)
		random_timer->Start(int(RandomFrequency * 1000), false);

	if (IsEnabled == true)
	{
		//process triggers
		for (int i = 0; i < (int)TriggerArray.size(); i++)
		{
			if (TriggerArray[i])
				TriggerArray[i]->Loop();
		}

		//process models
		for (int i = 0; i < (int)ModelArray.size(); i++)
		{
			if (ModelArray[i])
				ModelArray[i]->Loop();
		}
	}

	//process door sensors
	for (int i = 0; i < (int)DoorArray.size(); i++)
	{
		if (DoorArray[i])
			DoorArray[i]->CheckSensor();
	}

	//process queued sounds
	announcesnd->ProcessQueue();

	//reset message sound status
	DirMessageSound = false;
	DoorMessageSound = false;

	//elevator movement
	if (MoveElevator == true)
		MoveElevatorToFloor();
}

void Elevator::MoveElevatorToFloor()
{
	//Main processing routine; sends elevator to floor specified in GotoFloor
	//if InspectionService or manual movements are enabled, this function ignores GotoFloor values, since the elevator is manually moved

	SBS_PROFILE("Elevator::MoveElevatorToFloor");

	Ogre::Vector3 movement = Ogre::Vector3(0, 0, 0);
	bool StartLeveling = false;

	//wait until doors are fully closed if WaitForDoors is true
	if (WaitForDoors == true)
	{
		if (AreDoorsOpen() == true || AreDoorsMoving() != 0)
			return;
		else
			WaitForDoors = false;
	}

	//exit if waiting for arrival or departure timers
	if (WaitForTimer == true)
		return;

	if (ElevatorIsRunning == false)
	{
		if (Running == false)
		{
			ReportError("Elevator not running");
			return;
		}

		if (sbs->Verbose)
			Report("starting elevator movement procedure");

		ElevatorIsRunning = true;
		FinishedMove = false;
		std::string dir_string;

		Notified = false;

		//get elevator's current altitude
		elevposition = GetPosition();
		ElevatorStart = elevposition.y;

		//get elevator's current floor
		ElevatorFloor = GetFloor();
		oldfloor = ElevatorFloor;

		//switch off directional indicators on current floor if not already done so
		SetDirectionalIndicators(ElevatorFloor, false, false);

		//exit if floor doesn't exist
		if (!sbs->GetFloor(GotoFloor) && ManualMove == 0)
		{
			ReportError("Destination floor does not exist");
			MoveElevator = false;
			ElevatorIsRunning = false;
			DeleteActiveRoute();
			return;
		}

		//if destination floor is not a serviced floor, reset and exit
		if (IsServicedFloor(GotoFloor) == false && InspectionService == false && ManualMove == 0)
		{
			ReportError("Destination floor not in ServicedFloors list");
			MoveElevator = false;
			ElevatorIsRunning = false;
			DeleteActiveRoute();
			return;
		}

		//If elevator is already on specified floor, open doors and exit
		if (ElevatorFloor == GotoFloor && InspectionService == false && IsLeveled() == true && ManualMove == 0)
		{
			ReportError("Elevator already on specified floor");
			MoveElevator = false;
			ElevatorIsRunning = false;
			SkipFloorSound = true; //don't play floor announcement if on same floor
			DeleteActiveRoute();
			goto finish; //skip main processing and run cleanup section
		}

		//exit if doors are not fully closed while interlocks enabled
		if (Interlocks == true && (AreDoorsOpen() == true || AreShaftDoorsClosed() == false || DoorsStopped() == true))
		{
			ReportError("Doors must be closed before moving when interlocks are enabled");
			MoveElevator = false;
			ElevatorIsRunning = false;
			Direction = 0;
			DeleteActiveRoute();
			return;
		}

		//Determine direction
		if (InspectionService == false && ManualMove == 0)
		{
			Destination = GetDestinationAltitude(GotoFloor);
			if (Destination < ElevatorStart)
			{
				Direction = -1;
				dir_string = "down";
			}
			if (Destination > ElevatorStart)
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

		//Determine distance to destination floor
		if (InspectionService == false && ManualMove == 0)
			DistanceToTravel = fabsf(fabsf(Destination) - fabsf(ElevatorStart));
		else
		{
			//otherwise if inspection service is on, choose the altitude of the top/bottom floor
			if (Direction == 1)
			{
				Destination = GetDestinationAltitude(GetTopFloor());
				if (ElevatorStart >= Destination)
				{
					//don't go above top floor
					ReportError("cannot go above top floor");
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
				Destination = GetDestinationAltitude(GetBottomFloor());
				if (ElevatorStart <= Destination)
				{
					//don't go below bottom floor
					ReportError("cannot go below bottom floor");
					Destination = 0;
					Direction = 0;
					MoveElevator = false;
					ElevatorIsRunning = false;
					DeleteActiveRoute();
					return;
				}
			}
			DistanceToTravel = fabsf(fabsf(Destination) - fabsf(ElevatorStart));
		}
		CalculateStoppingDistance = true;

		//If user is riding this elevator, then turn off objects
		if (sbs->ElevatorSync == true && sbs->ElevatorNumber == Number && InspectionService == false && ManualMove == 0)
		{
			if (sbs->Verbose)
				Report("user in elevator - turning off objects");

			//turn off floor
			if (GetShaft()->ShowFloors == 0)
			{
				sbs->GetFloor(sbs->camera->CurrentFloor)->Enabled(false);
				sbs->GetFloor(sbs->camera->CurrentFloor)->EnableGroup(false);
			}
			else if (GetShaft()->IsShowFloor(sbs->camera->CurrentFloor) == false)
			{
				sbs->GetFloor(sbs->camera->CurrentFloor)->Enabled(false);
				sbs->GetFloor(sbs->camera->CurrentFloor)->EnableGroup(false);
			}

			//Turn off sky, buildings, and landscape
			if (GetShaft()->ShowOutside == false)
			{
				sbs->EnableSkybox(false);
				sbs->EnableBuildings(false);
				sbs->EnableLandscape(false);
				sbs->EnableExternal(false);
			}
			else if (GetShaft()->IsShowOutside(sbs->camera->CurrentFloor) == false)
			{
				sbs->EnableSkybox(false);
				sbs->EnableBuildings(false);
				sbs->EnableLandscape(false);
				sbs->EnableExternal(false);
			}

			//reset shaft doors
			ResetShaftDoors(GetFloor());
		}

		//set interior directional indicators
		UpdateDirectionalIndicators();

		//set external active-direction indicators
		if (sbs->GetFloor(sbs->camera->CurrentFloor))
			sbs->GetFloor(sbs->camera->CurrentFloor)->UpdateDirectionalIndicators(Number);

		//notify about movement
		if (InspectionService == false && ManualMove == 0)
			Report("moving " + dir_string + " to floor " + ToString(GotoFloor) + " (" + sbs->GetFloor(GotoFloor)->ID + ")");
		else
			Report("moving " + dir_string);
		IsMoving = true;
		OnFloor = false;
		SoundsQueued = true;

		//start departure timer
		if (DepartureDelay > 0 && WaitForTimer == false)
		{
			if (sbs->Verbose)
				Report("started departure delay");
			WaitForTimer = true;
			departure_delay->Start(int(DepartureDelay * 1000), false);
			return;
		}
	}

	if (SoundsQueued == true)
	{
		SoundsQueued = false;

		if (DepartureDelay > 0)
		{
			if (sbs->Verbose)
				Report("finished departure delay");
			departure_delay->Stop();
		}

		PlayStartingSounds();
	}

	if (EmergencyStop > 0 && Brakes == false)
	{
		//stop
		if (sbs->Verbose)
		{
			if (EmergencyStop == 2)
				Report("handling emergency stop");
			else
				Report("handling stop");
		}
		CalculateStoppingDistance = false;

		if (EmergencyStop == 2)
			TempDeceleration = Deceleration * EmergencyStopSpeed;
		else
			TempDeceleration = Deceleration;

		if (Direction == 1)
			Direction = -1;
		else
			Direction = 1;
		Brakes = true;

		//play stopping sounds
		if (EmergencyStop == 1)
			PlayStoppingSounds();
		else
			PlayStoppingSounds(true);
	}

	if (Brakes == false)
		PlayMovingSounds();

	//move elevator objects and camera
	movement.y = ElevatorRate * sbs->delta;

	MoveObjects(movement.y);

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
			limit = ElevatorSpeed * InspectionSpeed;

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

		float tmppos = JerkPos * (AccelJerk / DecelJerk);
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
			//stopping distance is the distance the elevator has traveled (usually to reach max speed), times
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

				//recalculate deceleration value based on distance from marker, and store result in TempDeceleration
				if ((Destination - LevelingOffset) > elevposition.y)
					TempDeceleration = Deceleration * (StoppingDistance / ((Destination - LevelingOffset) - elevposition.y));
				//if elevator is beyond leveling offset, ignore the offset
				else if (Destination > elevposition.y)
					TempDeceleration = Deceleration * (StoppingDistance / (Destination - elevposition.y));
				else
				{
					//if elevator is at destination
					TempDeceleration = 0;
					ElevatorRate = 0;
				}

				//start deceleration
				Direction = -1;
				Brakes = true;
				if (InspectionService == false)
					ElevatorRate -= ElevatorSpeed * ((TempDeceleration * JerkRate) * sbs->delta);
				else
					ElevatorRate -= (ElevatorSpeed * InspectionSpeed) * ((TempDeceleration * JerkRate) * sbs->delta);
			}
			//down movement
			else if (Direction == -1)
			{
				CalculateStoppingDistance = false;

				//recalculate deceleration value based on distance from marker, and store result in TempDeceleration
				if ((Destination + LevelingOffset) < elevposition.y)
					TempDeceleration = Deceleration * (StoppingDistance / (elevposition.y - (Destination + LevelingOffset)));
				//if elevator is beyond leveling offset, ignore the offset
				else if (Destination < elevposition.y)
					TempDeceleration = Deceleration * (StoppingDistance / (elevposition.y - Destination));
				else
				{
					//if elevator is at destination
					TempDeceleration = 0;
					ElevatorRate = 0;
				}

				//start deceleration
				Direction = 1;
				Brakes = true;
				if (InspectionService == false)
					ElevatorRate += ElevatorSpeed * ((TempDeceleration * JerkRate) * sbs->delta);
				else
					ElevatorRate += (ElevatorSpeed * InspectionSpeed) * ((TempDeceleration * JerkRate) * sbs->delta);
			}

			if (ElevatorRate != 0.0f)
			{
				PlayStoppingSounds();

				if (NotifyEarly == 2 && Parking == false)
					NotifyArrival(GotoFloor);
			}
		}
	}
	else if (Leveling == false && EmergencyStop == 0)
	{
		if (fabsf(ElevatorRate) <= LevelingSpeed)
		{
			//turn on leveling if elevator's rate is less than or equal to the leveling speed value
			if (sbs->Verbose)
				Report("leveling enabled");
			Leveling = true;
			StartLeveling = true;

			if (NotifyEarly == 1 && Parking == false)
				NotifyArrival(GotoFloor);
		}
	}

	//play floor beep and update indicators, if passing by or arriving at a floor
	if ((GetFloor() != oldfloor && Leveling == false) || StartLeveling == true)
	{
		float alt = sbs->GetFloor(GetFloor())->Altitude;
		bool pass = false;

		//determine if elevator will pass floor, only for down movement
		if (ActiveDirection == -1 && elevposition.y >= alt && elevposition.y + movement.y < alt)
			pass = true;
		if (ActiveDirection == 1)
			pass = true;

		//if elevator hasn't started leveling, and is about to arrive at the destination, cancel any update
		if (GetFloor() == GotoFloor && StartLeveling == false)
			pass = false;

		if (pass == true || StartLeveling == true)
		{
			if (sbs->Verbose)
				Report("on floor " + ToString(GetFloor()));

			//play floor beep sound if floor is a serviced floor
			if (IsServicedFloor(GetFloor()) == true)
				PlayFloorBeep();

			//update floor indicators
			UpdateFloorIndicators();

			//update floor indicators on current camera floor
			if (sbs->GetFloor(sbs->camera->CurrentFloor))
				sbs->GetFloor(sbs->camera->CurrentFloor)->UpdateFloorIndicators(Number);

			oldfloor = GetFloor();
		}
		StartLeveling = false;
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
		if (LevelingOpen > 0 && FinishedMove == false && ArrivalDelay == 0)
		{
			if (Direction == -1 && (Destination - elevposition.y) < LevelingOpen)
				FinishMove();
			else if (Direction == 1 && (elevposition.y - Destination) < LevelingOpen)
				FinishMove();
		}
	}

	//exit if elevator's running
	if (fabsf(ElevatorRate) != 0)
		return;

	//start arrival timer
	if (ArrivalDelay > 0)
	{
		if (WaitForTimer == false && arrival_delay->IsRunning() == false)
		{
			if (sbs->Verbose)
				Report("started arrival delay");
			WaitForTimer = true;
			arrival_delay->Start(int(ArrivalDelay * 1000), false);
			return;
		}
		else
		{
			if (sbs->Verbose)
				Report("finished arrival delay");
			arrival_delay->Stop();
		}
	}

	//finish move
	if (EmergencyStop == 0)
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

		MoveObjects(Destination - GetPosition().y);
	}

	//reset values if at destination floor
finish:
	if (sbs->Verbose)
		Report("resetting elevator motion values");

	if (GoPending == true)
	{
		GoPending = false;
		ChangeLight(GetFloor(), false);
	}
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
	Leveling = false;
	carsound->Stop();
	motorsound->Stop();
	tmpDecelJerk = 0;

	if (FinishedMove == false)
		FinishMove();
}

void Elevator::MoveObjects(float offset)
{
	//move elevator and objects vertically

	Ogre::Vector3 vector (0, offset, 0);

	Move(vector);
	elevposition.y = GetPosition().y;

	//move camera
	if (sbs->ElevatorSync == true && sbs->ElevatorNumber == Number)
		sbs->camera->MovePosition(vector);
}

void Elevator::SetFloor(int floor)
{
	//set elevator's altitude to specified floor

	float altitude = 0;
	if (!sbs->GetFloor(floor))
		return;

	altitude = GetDestinationAltitude(floor);
	MoveObjects(altitude - GetPosition().y);
}

void Elevator::FinishMove()
{
	//post-move operations, such as chimes, opening doors, indicator updates, etc

	//manualstop is true if elevator is stopped within 18 inches of the nearest landing
	bool manualstop = (InServiceMode() == false && EmergencyStop == 1 && fabsf(GetDestinationAltitude(GetFloor()) - GetPosition().y) < 1.5);

	if (EmergencyStop == 0 || manualstop == true)
	{
		if (manualstop == true)
			GotoFloor = GetFloor();

		//the elevator is now stopped on a valid floor; set OnFloor to true
		OnFloor = true;
		Report("arrived at floor " + ToString(GotoFloor) + " (" + sbs->GetFloor(GotoFloor)->ID + ")");

		//dequeue floor route
		if (manualstop == false)
			DeleteActiveRoute();
	}

	//turn off interior directional indicators
	ActiveDirection = 0;
	UpdateDirectionalIndicators();

	//update external active-direction indicators
	if (sbs->GetFloor(sbs->camera->CurrentFloor))
		sbs->GetFloor(sbs->camera->CurrentFloor)->UpdateDirectionalIndicators(Number);

	if ((EmergencyStop == 0 || manualstop == true) && InspectionService == false)
	{
		//update floor indicators on current camera floor
		if (sbs->GetFloor(sbs->camera->CurrentFloor))
			sbs->GetFloor(sbs->camera->CurrentFloor)->UpdateFloorIndicators(Number);

		UpdateFloorIndicators();

		//turn on objects if user is in elevator
		if (sbs->ElevatorSync == true && sbs->ElevatorNumber == Number && CameraOffset < Height)
		{
			if (sbs->Verbose)
				Report("user in elevator - turning on objects");

			//turn on floor
			sbs->GetFloor(GotoFloor)->Enabled(true);
			sbs->GetFloor(GotoFloor)->EnableGroup(true);

			//Turn on sky, buildings, and landscape
			sbs->EnableSkybox(true);
			sbs->EnableBuildings(true);
			sbs->EnableLandscape(true);
			sbs->EnableExternal(true);

			//reset shaft doors
			ResetShaftDoors(GotoFloor);
		}
		else if (sbs->Verbose)
			Report("user not in elevator - not turning on objects");

		//notify arrival and disable call button light
		if (InServiceMode() == false)
		{
			//notify on arrival
			if ((NotifyEarly == 0 || Notified == false) && Parking == false)
				NotifyArrival(GotoFloor);

			//get status of call buttons before switching off
			GetCallButtonStatus(GotoFloor, UpCall, DownCall);

			//disable call button lights
			SetCallButtons(GotoFloor, GetArrivalDirection(GotoFloor), false);
		}

		//reset queues if specified
		if (QueueResets == true)
		{
			//if last entry in current queue, reset opposite queue
			if (QueuePositionDirection == 1 && UpQueue.size() == 0 && DownQueue.size() > 0)
				ResetQueue(false, true);
			else if (QueuePositionDirection == -1 && DownQueue.size() == 0 && UpQueue.size() > 0)
				ResetQueue(true, false);
		}

		//reverse queues if at either top or bottom of serviced floors
		if (QueuePositionDirection == 1 && GotoFloor == GetTopFloor())
		{
			if (sbs->Verbose)
				Report("at top floor; setting queue search direction to down");
			LastQueueDirection = QueuePositionDirection;
			QueuePositionDirection = -1;
		}
		else if (QueuePositionDirection == -1 && GotoFloor == GetBottomFloor())
		{
			if (sbs->Verbose)
				Report("at bottom floor; setting queue search direction to up");
			LastQueueDirection = QueuePositionDirection;
			QueuePositionDirection = 1;
		}

		//open doors
		//do not automatically open doors if fire service phase 2 is on
		if (FireServicePhase2 != 1 || OnRecallFloor() == true)
		{
			if (Parking == false)
				if (AutoDoors == true)
					OpenDoors();
		}
	}
	else
	{
		if (sbs->Verbose)
			Report("stop complete");

		//reset shaft doors
		if (sbs->ElevatorSync == true && sbs->ElevatorNumber == Number)
			ResetShaftDoors(GotoFloor);
	}

	//update elevator's floor number
	ElevatorFloor = GotoFloor;

	EmergencyStop = 0;
	SkipFloorSound = false;
	Parking = false;
	FinishedMove = true;
}

WallObject* Elevator::AddWall(const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th)
{
	//Adds a wall with the specified dimensions

	WallObject *wall = ElevatorMesh->CreateWallObject(name);
	sbs->AddWallMain(wall, name, texture, thickness, x1, z1, x2, z2, height1, height2, voffset1, voffset2, tw, th, true);
	return wall;
}

WallObject* Elevator::AddFloor(const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, bool reverse_axis, bool texture_direction, float tw, float th, bool legacy_behavior)
{
	//Adds a floor with the specified dimensions and vertical offset

	WallObject *wall = ElevatorMesh->CreateWallObject(name);
	sbs->AddFloorMain(wall, name, texture, thickness, x1, z1, x2, z2, voffset1, voffset2, reverse_axis, texture_direction, tw, th, true, legacy_behavior);
	return wall;
}

FloorIndicator* Elevator::AddFloorIndicator(const std::string &texture_prefix, const std::string &direction, float CenterX, float CenterZ, float width, float height, float voffset)
{
	//Creates a floor indicator at the specified location

	FloorIndicator* indicator = new FloorIndicator(this, Number, texture_prefix, direction, CenterX, CenterZ, width, height, voffset);
	FloorIndicatorArray.push_back(indicator);
	return indicator;
}

void Elevator::DumpQueues()
{
	//dump both (up and down) elevator queues

	sbs->Report("\n--- Elevator " + ToString(Number) + " Queues ---\n");

	if (UpQueue.size() > 0)
		sbs->Report("Up:");

	for (int i = 0; i < (int)UpQueue.size(); i++)
	{
		std::string type = "Car";
		if (UpQueue[i].call_type == 1)
			type = "Hall";
		if (UpQueue[i].call_type == 2)
			type = "System";
		sbs->Report("Entry: " + ToString(i) + "\t-\tFloor: " + ToString(UpQueue[i].floor) + "\t-\tCall type: " + type);
	}

	if (DownQueue.size() > 0)
		sbs->Report("Down:");

	for (int i = 0; i < (int)DownQueue.size(); i++)
	{
		std::string type = "Car";
		if (DownQueue[i].call_type == 1)
			type = "Hall";
		if (DownQueue[i].call_type == 2)
			type = "System";
		sbs->Report("Entry: " + ToString(i) + "\t-\tFloor: " + ToString(DownQueue[i].floor) + "\t-\tCall type: " + type);
	}
	sbs->Report("");
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

	ElevatorMesh->Enable(value);
	EnableDoors(value);
	IsEnabled = value;

	//floor indicators
	for (int i = 0; i < (int)FloorIndicatorArray.size(); i++)
	{
		if (FloorIndicatorArray[i])
			FloorIndicatorArray[i]->Enabled(value);
	}

	//interior directional indicators
	EnableDirectionalIndicators(value);
}

void Elevator::EnableObjects(bool value)
{
	//enable or disable interior objects, such as floor indicators and button panels

	if (AutoEnable == false)
		return;

	//SBS_PROFILE("Elevator::EnableObjects");
	if (sbs->Verbose)
	{
		if (value == true)
			Report("enabling objects");
		else
			Report("disabling objects");
	}

	//floor indicators
	/*for (int i = 0; i < FloorIndicatorArray.size(); i++)
	{
		if (FloorIndicatorArray[i])
			FloorIndicatorArray[i]->Enabled(value);
	}*/

	//interior directional indicators
	//EnableDirectionalIndicators(value);

	//controls
	for (size_t i = 0; i < (int)ControlArray.size(); i++)
	{
		if (ControlArray[i])
			ControlArray[i]->Enabled(value);
	}

	//triggers
	for (size_t i = 0; i < (int)TriggerArray.size(); i++)
	{
		if (TriggerArray[i])
			TriggerArray[i]->Enabled(value);
	}

	//models
	for (size_t i = 0; i < (int)ModelArray.size(); i++)
	{
		if (ModelArray[i])
			ModelArray[i]->Enable(value);
	}

	//panels
	for (int i = 0; i < (int)PanelArray.size(); i++)
		PanelArray[i]->Enabled(value);

	//sounds
	for (int i = 0; i < (int)sounds.size(); i++)
	{
		if (sounds[i])
		{
			if (sounds[i]->GetLoopState() == true)
			{
				if (value == false)
					sounds[i]->Stop();
				else
					sounds[i]->Play();
			}
		}
	}
}

bool Elevator::IsElevator(Ogre::MeshPtr test)
{
	if (test == ElevatorMesh->MeshWrapper)
		return true;

	return false;
}

bool Elevator::IsInElevator(const Ogre::Vector3 &position, bool camera)
{
	//determine if the given 3D position is inside the elevator

	//first checks to see if camera is within an elevator's height range, and then
	//checks for a collision with the elevator's floor below

	//if camera is true, set associated camera offset

	//SBS_PROFILE("Elevator::IsInElevator");
	bool inelevator = false;

	//if last position is the same as new, return previous result
	if ((position.x >= (lastposition.x - 0.01)) &&
		(position.y >= (lastposition.y - 0.01)) &&
		(position.z >= (lastposition.z - 0.01)) &&
		(position.x <= (lastposition.x + 0.01)) &&
		(position.y <= (lastposition.y + 0.01)) &&
		(position.z <= (lastposition.z + 0.01)) &&
		checkfirstrun == false)
		return lastcheckresult;

	checkfirstrun = false;

	if (position.y >= GetPosition().y && position.y < GetPosition().y + (Height * 2))
	{
		if (ElevatorMesh->InBoundingBox(position, false) == true)
		{
			if (ElevatorMesh->HitBeam(position, Ogre::Vector3::NEGATIVE_UNIT_Y, Height) >= 0)
			{
				if (camera == true)
					CameraOffset = position.y - GetPosition().y;
				inelevator = true;
			}
		}
		else if (camera == true)
			CameraOffset = 0;

		if (position.y < GetPosition().y + Height)
		{
			//cache values
			lastcheckresult = inelevator;
			lastposition = position;

			return inelevator;
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

int  Elevator::GetEmergencyStopStatus()
{
	//returns the internal emergency stop status
	return EmergencyStop;
}

void Elevator::DumpServicedFloors()
{
	//dump serviced floors list

	sbs->Report("\n--- Elevator " + ToString(Number) + "'s Serviced Floors ---\n");
	for (int i = 0; i < (int)ServicedFloors.size(); i++)
		sbs->Report(ToString(i) + " - " + ToString(ServicedFloors[i]));
	sbs->Report("");
}

bool Elevator::AddServicedFloor(int number)
{
	if (sbs->Verbose)
		Report("adding serviced floor " + ToString(number));

	//check if floor is outside valid floor range
	if (sbs->IsValidFloor(number) == false)
		return ReportError("AddServicedFloor: Invalid floor " + ToString(number));

	if (IsServicedFloor(number) == false)
	{
		ServicedFloors.push_back(number);
		std::sort(ServicedFloors.begin(), ServicedFloors.end());
	}
	return true;
}

void Elevator::RemoveServicedFloor(int number)
{
	if (sbs->Verbose)
		Report("removing serviced floor " + ToString(number));
	if (IsServicedFloor(number) == true)
	{
		int index = GetFloorIndex(number);
		if (index > -1)
			ServicedFloors.erase(ServicedFloors.begin() + index);
	}
}

ButtonPanel* Elevator::CreateButtonPanel(const std::string &texture, int rows, int columns, const std::string &direction, float CenterX, float CenterZ, float buttonwidth, float buttonheight, float spacingX, float spacingY, float voffset, float tw, float th)
{
	//create a new button panel object and store the pointer

	int index = (int)PanelArray.size();

	if (sbs->Verbose)
		Report("creating button panel " + ToString(index + 1));

	ButtonPanel* panel = new ButtonPanel(Number, index + 1, texture, rows, columns, direction, CenterX, CenterZ, buttonwidth, buttonheight, spacingX, spacingY, voffset, tw, th);
	PanelArray.push_back(panel);
	return panel;
}

void Elevator::UpdateFloorIndicators()
{
	//updates all floor indicators

	for (int i = 0; i < (int)FloorIndicatorArray.size(); i++)
	{
		if (FloorIndicatorArray[i])
			FloorIndicatorArray[i]->Update();
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

void Elevator::SetFloorSkipText(const std::string &id)
{
	//sets the text shown in the floor indicator while skipping floors (an express zone)

	std::string text = id;
	TrimString(text);

	if (text == "")
	{
		UseFloorSkipText = false;
		FloorSkipText = text;
		return;
	}

	if (sbs->Verbose)
		Report("setting floor skip text to " + text);
	UseFloorSkipText = true;
	FloorSkipText = id;
}

std::string Elevator::GetFloorSkipText()
{
	//get the floor skip text
	return FloorSkipText;
}

bool Elevator::IsServicedFloor(int floor)
{
	//returns true if floor is in serviced floor list, otherwise false

	int index = GetFloorIndex(floor);
	if (index == -1)
	{
		if (sbs->Verbose)
			Report("Floor " + ToString(floor) + " is not a serviced floor");
		return false;
	}
	else
	{
		if (sbs->Verbose)
			Report("Floor " + ToString(floor) + " is a serviced floor");
		return true;
	}
}

bool Elevator::InServiceMode()
{
	//report if an elevator is in a service mode
	if (IndependentService == true || InspectionService == true || FireServicePhase1 == 1 || FireServicePhase2 > 0)
		return true;
	else
		return false;
}

bool Elevator::Go(int floor, bool hold)
{
	//go to specified floor, bypassing the queuing system

	if (Running == false)
		return ReportError("Elevator not running");

	if (!sbs->GetFloor(floor))
		return ReportError("Invalid floor " + ToString(floor));

	//exit if in inspection mode
	if (InspectionService == true)
	{
		if (sbs->Verbose)
			ReportError("Go: in inspection mode");
		return false;
	}

	if (GoActive == false || hold == false)
	{
		//exit if elevator is moving
		if (MoveElevator == true)
			return false;

		if (hold == true)
		{
			GoActive = true;
			GoActiveFloor = floor;
		}
		Report("Go: proceeding to floor " + ToString(floor) + " (" + sbs->GetFloor(floor)->ID + ")");
		ChangeLight(floor, true);
		GotoFloor = floor;
		GoPending = true;
		if (AutoDoors == true)
		{
			WaitForDoors = true;
			CloseDoors();
		}
		MoveElevator = true;
	}
	else if (GoActive == true && hold == true && sbs->camera->MouseDown == false)
	{
		//stop go movement
		GoActive = false;
		GoActiveFloor = 0;
		Stop();
		ChangeLight(floor, false);
	}
	return true;
}

void Elevator::GoToRecallFloor()
{
	//for fire service modes; tells the elevator to go to the recall floor (or the alternate recall floor
	//if the other is not available)

	if (Running == false)
	{
		ReportError("Elevator not running");
		return;
	}

	//stop elevator if moving
	if (IsMoving == true)
		Stop();

	//reset queues
	ResetQueue(true, true);

	if (OnRecallFloor() == true)
	{
		if (RecallUnavailable == false)
			Report("On recall floor");
		else
			Report("On alternate recall floor");
		if (AutoDoors == true)
			OpenDoors();
		return;
	}

	if (RecallUnavailable == false)
	{
		Report("Proceeding to recall floor");
		if (RecallFloor > GetFloor())
			AddRoute(RecallFloor, 1, 2);
		else
			AddRoute(RecallFloor, -1, 2);
	}
	else
	{
		Report("Proceeding to alternate recall floor");
		if (RecallFloorAlternate > GetFloor())
			AddRoute(RecallFloorAlternate, 1, 2);
		else
			AddRoute(RecallFloorAlternate, -1, 2);
	}
}

bool Elevator::EnableACP(bool value)
{
	//enable Anti-Crime Protection (ACP) mode

	if (Running == false)
		return ReportError("Elevator not running");

	//exit if no change
	if (ACP == value)
	{
		if (sbs->Verbose)
			ReportError("EnableACP: mode is the same");
		return false;
	}

	if (value == true)
	{
		if (IndependentService == true)
			return ReportError("EnableACP: cannot enable while in independent service mode");
		if (InspectionService == true)
			return ReportError("EnableACP: cannot enable while in inspection service mode");
		if (FireServicePhase1 > 0 || FireServicePhase2 > 0)
			return ReportError("EnableACP: cannot enable while in a fire service mode");
	}

	ACP = value;

	if (value == true)
		Report("ACP mode enabled");
	else
		Report("ACP mode disabled");

	return true;
}

bool Elevator::EnableUpPeak(bool value)
{
	//enable Up-Peak mode

	if (Running == false)
		return ReportError("Elevator not running");

	//exit if no change
	if (UpPeak == value)
	{
		if (sbs->Verbose)
			ReportError("EnableUpPeak: mode is the same");
		return false;
	}

	if (value == true)
	{
		if (IndependentService == true)
			return ReportError("EnableUpPeak: cannot enable while in independent service mode");
		if (InspectionService == true)
			return ReportError("EnableUpPeak: cannot enable while in inspection service mode");
		if (FireServicePhase1 > 0 || FireServicePhase2 > 0)
			return ReportError("EnableUpPeak: cannot enable while in a fire service mode");
	}

	UpPeak = value;

	if (value == true)
	{
		EnableDownPeak(false);
		if (IsMoving == false && GetFloor() == GetBottomFloor() && sbs->GetFloor(GetFloor()))
		{
			SetDirectionalIndicators(GetFloor(), true, false);
			if (AutoDoors == true)
				OpenDoors();
		}
		Report("Up Peak mode enabled");
	}
	else
	{
		ResetDoors();
		ResetNudgeTimer();
		Report("Up Peak mode disabled");
	}

	return true;
}

bool Elevator::EnableDownPeak(bool value)
{
	//enable Down-Peak mode

	if (Running == false)
		return ReportError("Elevator not running");

	//exit if no change
	if (DownPeak == value)
	{
		if (sbs->Verbose)
			ReportError("EnableDownPeak: mode is the same");
		return false;
	}

	if (value == true)
	{
		if (IndependentService == true)
			return ReportError("EnableDownPeak: cannot enable while in independent service mode");
		if (InspectionService == true)
			return ReportError("EnableDownPeak: cannot enable while in inspection service mode");
		if (FireServicePhase1 > 0 || FireServicePhase2 > 0)
			return ReportError("EnableDownPeak: cannot enable while in a fire service mode");
	}

	DownPeak = value;

	if (value == true)
	{
		EnableUpPeak(false);
		if (IsMoving == false && GetFloor() == GetTopFloor() && sbs->GetFloor(GetFloor()))
		{
			SetDirectionalIndicators(GetFloor(), false, true);
			if (AutoDoors == true)
				OpenDoors();
		}
		Report("Down Peak mode enabled");
	}
	else
	{
		ResetDoors();
		ResetNudgeTimer();
		Report("Down Peak mode disabled");
	}

	return true;
}

bool Elevator::EnableIndependentService(bool value)
{
	//enable Independent Service (ISC) mode

	if (Running == false)
		return ReportError("Elevator not running");

	//exit if no change
	if (IndependentService == value)
	{
		if (sbs->Verbose)
			ReportError("EnableIndependentService: mode is the same");
		return false;
	}

	if (value == true)
	{
		if (InspectionService == true)
			return ReportError("EnableIndependentService: cannot enable while in inspection service mode");
		if (FireServicePhase1 > 0 || FireServicePhase2 > 0)
			return ReportError("EnableIndependentService: cannot enable while in a fire service mode");
	}

	IndependentService = value;

	if (value == true)
	{
		EnableACP(false);
		EnableUpPeak(false);
		EnableDownPeak(false);
		ResetQueue(true, true);
		HoldDoors(); //turn off door timers
		ResetNudgeTimer(false); //switch off nudge timer
		SetDirectionalIndicators(ElevatorFloor, false, false); //switch off directional indicators on current floor
		if (IsMoving == false)
			if (AutoDoors == true)
				OpenDoors();
		Report("Independent Service mode enabled");
	}
	else
	{
		ResetDoors();
		ResetNudgeTimer();
		Report("Independent Service mode disabled");
	}

	return true;
}

bool Elevator::EnableInspectionService(bool value)
{
	//enable Inspection Service (INS) mode

	//exit if no change
	if (InspectionService == value)
	{
		if (sbs->Verbose)
			ReportError("EnableInspectionService: mode is the same");
		return false;
	}

	if (value == true)
	{
		EnableACP(false);
		EnableUpPeak(false);
		EnableDownPeak(false);
		EnableIndependentService(false);
		EnableFireService1(0);
		EnableFireService2(0, true);
		ResetQueue(true, true);
		HoldDoors(); //turn off door timers
		ResetNudgeTimer(false); //switch off nudge timer
		SetDirectionalIndicators(ElevatorFloor, false, false); //switch off directional indicators on current floor
		if (IsMoving == true)
			Stop();
		Report("Inspection Service mode enabled");
		InspectionService = true;
	}
	else
	{
		ResetDoors();
		ResetNudgeTimer();
		Report("Inspection Service mode disabled");

		UpdateFloorIndicators();

		//turn on objects if user is in elevator
		if (sbs->ElevatorSync == true && sbs->ElevatorNumber == Number && IsMoving == false)
		{
			if (sbs->Verbose)
				Report("user in elevator - turning on objects");

			//turn on floor
			if (sbs->GetFloor(GetFloor()))
			{
				sbs->GetFloor(GetFloor())->Enabled(true);
				sbs->GetFloor(GetFloor())->EnableGroup(true);
			}

			//Turn on sky, buildings, and landscape
			sbs->EnableSkybox(true);
			sbs->EnableBuildings(true);
			sbs->EnableLandscape(true);
			sbs->EnableExternal(true);

			//reset shaft doors
			ResetShaftDoors(GetFloor());
		}

		InspectionService = false;

		if (IsMoving == true)
			Stop();
		else
			ReturnToNearestFloor();
	}

	return true;
}

bool Elevator::EnableFireService1(int value)
{
	//enable Fire Service Phase 1 mode
	//valid values are 0 (off), 1 (on) or 2 (bypass)

	if (Running == false)
		return ReportError("Elevator not running");

	//exit if no change
	if (FireServicePhase1 == value)
	{
		if (sbs->Verbose)
			ReportError("EnableFireService1: mode is the same");
		return false;
	}

	//exit if in inspection mode
	if (InspectionService == true && value > 0)
		return ReportError("EnableFireService1: cannot enable while in inspection service mode");

	if (value < 0 || value > 2)
		return ReportError("EnableFireService1: invalid value");

	if (value > 0)
	{
		EnableACP(false);
		EnableUpPeak(false);
		EnableDownPeak(false);
		EnableIndependentService(false);
		if (value == 1)
		{
			Report("Fire Service Phase 1 mode set to On");

			//switch off directional indicators on current floor
			SetDirectionalIndicators(ElevatorFloor, false, false);

			//recall elevator if not in phase 2 hold
			if (FireServicePhase2 != 2)
			{
				//turn off all door timers
				HoldDoors();
				ResetNudgeTimer(false); //switch off nudge timer

				//enable nudge mode on all doors if any are open
				if (OnRecallFloor() == false)
					EnableNudgeMode(true);

				//goto recall floor
				GoToRecallFloor();
			}
		}
		else
		{
			if (FireServicePhase2 == 0)
			{
				ResetDoors(); //enable door timers
				ResetNudgeTimer();
			}
			Report("Fire Service Phase 1 mode set to Bypass");
		}
	}
	else
	{
		if (FireServicePhase2 == 0)
		{
			ResetDoors(); //enable door timers
			ResetNudgeTimer();
		}
		Report("Fire Service Phase 1 mode set to Off");
	}

	FireServicePhase1 = value;

	return true;
}

bool Elevator::EnableFireService2(int value, bool force)
{
	//enable Fire Service Phase 2 mode
	//valid values are 0 (off), 1 (on) or 2 (hold)
	//if force is true, does not require doors to be opened to change value

	if (Running == false)
		return ReportError("Elevator not running");

	if (value > 0)
	{
		//exit if in inspection mode
		if (InspectionService == true)
			return ReportError("EnableFireService2: cannot enable while in inspection service mode");

		//require fire service phase 1 to be enabled first
		if (FireServicePhase1 != 1 && FireServicePhase2 == 0)
			return ReportError("EnableFireService2: not in fire service phase 1 mode");
	}

	//require doors to be open to change modes
	if (AreDoorsOpen() == false && force == false)
		return ReportError("EnableFireService2: doors must be open to change phase 2 modes");

	//exit if no change
	if (FireServicePhase2 == value)
	{
		if (sbs->Verbose)
			ReportError("EnableFireService2: mode is the same");
		return false;
	}

	if (value >= 0 && value <= 2)
		FireServicePhase2 = value;
	else
		return ReportError("EnableFireService2: invalid value");

	if (value > 0)
	{
		EnableACP(false);
		EnableUpPeak(false);
		EnableDownPeak(false);
		EnableIndependentService(false);
		ResetQueue(true, true);
		HoldDoors(); //disable all door timers
		ResetNudgeTimer(false); //switch off nudge timer
		if (value == 1)
			Report("Fire Service Phase 2 mode set to On");
		else
			Report("Fire Service Phase 2 mode set to Hold");
	}
	else
	{
		Report("Fire Service Phase 2 mode set to Off");

		if (FireServicePhase1 == 0)
		{
			ResetDoors(); //enable door timers
			ResetNudgeTimer();
		}
		else if (FireServicePhase1 == 1 && OnRecallFloor() == false)
		{
			//enable nudge mode on all doors if any are open
			EnableNudgeMode(true);

			//recall elevator
			GoToRecallFloor();
		}
	}

	return true;
}

bool Elevator::SetRecallFloor(int floor)
{
	//set elevator's fire recall floor

	if (ServicedFloors.size() == 0)
		return ReportError("No serviced floors assigned");
	if (IsServicedFloor(floor) == false)
		return ReportError("Invalid recall floor");

	if (sbs->Verbose)
		Report("setting recall floor to " + ToString(floor));
	RecallFloor = floor;
	RecallSet = true;
	return true;
}

bool Elevator::SetAlternateRecallFloor(int floor)
{
	//set elevator's alternate fire recall floor

	if (ServicedFloors.size() == 0)
		return ReportError("No serviced floors assigned");
	if (IsServicedFloor(floor) == false)
		return ReportError("Invalid alternate recall floor");

	if (sbs->Verbose)
		Report("setting alternate recall floor to " + ToString(floor));
	RecallFloorAlternate = floor;
	RecallAltSet = true;
	return true;
}

bool Elevator::SetACPFloor(int floor)
{
	//set elevator's ACP floor

	if (ServicedFloors.size() == 0)
		return ReportError("No serviced floors assigned");
	if (IsServicedFloor(floor) == false)
		return ReportError("Invalid ACP floor");

	if (sbs->Verbose)
		Report("setting ACP floor to " + ToString(floor));
	ACPFloor = floor;
	ACPFloorSet = true;
	return true;
}

bool Elevator::MoveUp()
{
	//manual up movement for inspection service mode

	if (Running == false)
		return ReportError("Elevator not running");

	//moves elevator upwards if in Inspection Service mode
	if (InspectionService == false)
		return ReportError("Not in inspection service mode");

	//make sure Go button is on
	if (ManualGo == false)
	{
		if (sbs->Verbose)
			ReportError("MoveUp: go button is off");
		return false;
	}

	if (IsMoving == true)
	{
		if (sbs->Verbose)
			ReportError("MoveUp: already moving");
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
	//manual down movement for inspection service mode

	if (Running == false)
		return ReportError("Elevator not running");

	//moves elevator downwards if in Inspection Service mode
	if (InspectionService == false)
		return ReportError("Not in inspection service mode");

	//make sure Go button is on
	if (ManualGo == false)
	{
		if (sbs->Verbose)
			ReportError("MoveDown: go button is off");
		return false;
	}

	if (IsMoving == true)
	{
		if (sbs->Verbose)
			ReportError("MoveDown: already moving");
		return false;
	}

	//set direction
	Direction = -1;
	MoveElevator = true;
	if (sbs->Verbose)
		Report("MoveDown: moving elevator");
	return true;
}

bool Elevator::SetGoButton(bool value)
{
	//sets the value of the Go button (for Inspection Service mode)

	if (Running == false)
		return ReportError("Elevator not running");

	if (InspectionService == false)
		return false;

	if (ManualGo == true && value == false)
		Stop();

	ManualGo = value;

	if (sbs->Verbose)
	{
		if (value == true)
			Report("setting go button status to true");
		else
			Report("setting go button status to false");
	}

	if (ManualGo == true)
	{
		if (ManualUp == true)
			MoveUp();
		if (ManualDown == true)
			MoveDown();
	}

	return true;
}

bool Elevator::SetUpButton(bool value)
{
	//sets the value of the Up button (for Inspection Service mode)

	if (Running == false)
		return ReportError("Elevator not running");

	if (InspectionService == false)
		return false;

	if (ManualUp == true && value == false)
		Stop();

	ManualUp = value;

	if (sbs->Verbose)
	{
		if (value == true)
			Report("setting up button status to true");
		else
			Report("setting up button status to false");
	}

	if (ManualGo == true && value == true)
		MoveUp();

	return true;
}

bool Elevator::SetDownButton(bool value)
{
	//sets the value of the Go button (for Inspection Service mode)

	if (Running == false)
		return ReportError("Elevator not running");

	if (InspectionService == false)
		return false;

	if (ManualDown == true && value == false)
		Stop();

	ManualDown = value;

	if (sbs->Verbose)
	{
		if (value == true)
			Report("setting down button status to true");
		else
			Report("setting down button status to false");
	}

	if (ManualGo == true && value == true)
		MoveDown();

	return true;
}

int Elevator::GetTopFloor()
{
	//returns highest serviced floor
	return ServicedFloors[ServicedFloors.size() - 1];
}

int Elevator::GetBottomFloor()
{
	//returns lowest serviced floor
	return ServicedFloors[0];
}

void Elevator::AddDirectionalIndicators(bool relative, bool active_direction, bool single, bool vertical, const std::string &BackTexture, const std::string &uptexture, const std::string &uptexture_lit, const std::string &downtexture, const std::string &downtexture_lit, float CenterX, float CenterZ, float voffset, const std::string &direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th)
{
	//create external directional indicators on all serviced floors

	if (sbs->Verbose)
		Report("adding directional indicators");

	for (size_t i = 0; i < ServicedFloors.size(); i++)
	{
		if (sbs->GetFloor(ServicedFloors[i]))
			sbs->GetFloor(ServicedFloors[i])->AddDirectionalIndicator(Number, relative, active_direction, single, vertical, BackTexture, uptexture, uptexture_lit, downtexture, downtexture_lit, CenterX, CenterZ, voffset, direction, BackWidth, BackHeight, ShowBack, tw, th);
	}
}

DirectionalIndicator* Elevator::AddDirectionalIndicator(bool active_direction, bool single, bool vertical, const std::string &BackTexture, const std::string &uptexture, const std::string &uptexture_lit, const std::string &downtexture, const std::string &downtexture_lit, float CenterX, float CenterZ, float voffset, const std::string &direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th)
{
	//create a directional indicator inside the elevator

	if (sbs->Verbose)
		Report("adding interior directional indicator");

	DirectionalIndicator *indicator = new DirectionalIndicator(this, Number, 0, active_direction, single, vertical, BackTexture, uptexture, uptexture_lit, downtexture, downtexture_lit, CenterX, CenterZ, voffset, direction, BackWidth, BackHeight, ShowBack, tw, th);
	DirIndicatorArray.push_back(indicator);
	return indicator;
}

void Elevator::SetDirectionalIndicators(int floor, bool UpLight, bool DownLight)
{
	//set light status of exterior and interior directional indicators
	//for interior indicators, the value of floor is passed to the indicator for checks

	//exterior indicators
	if (sbs->GetFloor(floor))
		sbs->GetFloor(floor)->SetDirectionalIndicators(Number, UpLight, DownLight);

	//interior indicators
	for (int i = 0; i < (int)DirIndicatorArray.size(); i++)
	{
		DirectionalIndicator *indicator = DirIndicatorArray[i];

		if (indicator)
		{
			if (indicator->ActiveDirection == false)
			{
				indicator->floor = floor;
				indicator->DownLight(DownLight);
				indicator->UpLight(UpLight);
			}
		}
	}
}

void Elevator::UpdateDirectionalIndicators()
{
	//updates all interior active direction indicators

	for (int i = 0; i < (int)DirIndicatorArray.size(); i++)
	{
		DirectionalIndicator *indicator = DirIndicatorArray[i];

		if (indicator)
		{
			if (indicator->ActiveDirection == true)
			{
				if (ActiveDirection == 1)
				{
					indicator->DownLight(false);
					indicator->UpLight(true);
				}
				if (ActiveDirection == 0)
				{
					indicator->DownLight(false);
					indicator->UpLight(false);
				}
				if (ActiveDirection == -1)
				{
					indicator->DownLight(true);
					indicator->UpLight(false);
				}
			}
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

	for (int i = 0; i < (int)DirIndicatorArray.size(); i++)
	{
		if (DirIndicatorArray[i])
			DirIndicatorArray[i]->Enabled(value);
	}
}

ElevatorDoor* Elevator::GetDoor(int number)
{
	//get elevator door object

	//return cached check if number is the same
	if (lastdoor_number == number && lastdoor_result)
		return lastdoor_result;

	if (number > 0 && number <= (int)DoorArray.size())
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

bool Elevator::OpenDoorsEmergency(int number, int whichdoors, int floor, bool hold)
{
	//Simulates manually prying doors open.
	//Slowly opens the elevator doors no matter where elevator is.
	//If lined up with shaft doors, then opens the shaft doors also

	//WhichDoors is the doors to move:
	//1 = both shaft and elevator doors
	//2 = only elevator doors
	//3 = only shaft doors

	return OpenDoors(number, whichdoors, floor, true, hold);
}

void Elevator::CloseDoorsEmergency(int number, int whichdoors, int floor, bool hold)
{
	//Simulates manually closing doors.
	//Slowly closes the elevator doors no matter where elevator is.
	//If lined up with shaft doors, then closes the shaft doors also

	//WhichDoors is the doors to move:
	//1 = both shaft and elevator doors
	//2 = only elevator doors
	//3 = only shaft doors

	CloseDoors(number, whichdoors, floor, true, hold);
}

bool Elevator::OpenDoors(int number, int whichdoors, int floor, bool manual, bool hold)
{
	//Opens elevator doors

	//if manual is true, then it simulates manually prying doors open,
	//Slowly opens the elevator doors no matter where elevator is,
	//and if lined up with shaft doors, then opens the shaft doors also.
	//if hold is true, sets 'hold' state requiring button to be held to keep doors opening

	//WhichDoors is the doors to move:
	//1 = both shaft and elevator doors
	//2 = only elevator doors
	//3 = only shaft doors

	//require open button to be held for fire service phase 2 if not on recall floor
	if (FireServicePhase2 == 1 && OnRecallFloor() == false)
		hold = true;

	if (Interlocks == true)
	{
		if (IsMoving == true && OnFloor == false)
			return ReportError("Cannot open doors while moving if interlocks are enabled");

		if (OnFloor == false || (whichdoors == 3 && floor != GetFloor()))
			return ReportError("Cannot open doors if not stopped within a landing zone if interlocks are enabled");
	}

	int start = number, end = number;
	if (number == 0)
	{
		start = 1;
		end = NumDoors;
	}
	if (doorhold_direction == 0)
	{
		if (ControlPressActive == true && AutoDoors == true && InServiceMode() == false && hold == false && manual == false && whichdoors != 3 && DoorsStopped(number) == false)
		{
			doorhold_direction = 2;

			if (AreDoorsOpen(number) == true && AreDoorsMoving(number) == false)
				return true; //exit to skip an extra open door call
		}

		if (hold == true)
			doorhold_direction = 1;

		if (doorhold_direction > 0)
		{
			//set persistent values
			doorhold_whichdoors = whichdoors;
			doorhold_floor = floor;
			doorhold_manual = manual;
		}

		for (int i = start; i <= end; i++)
		{
			if (GetDoor(i))
				GetDoor(i)->OpenDoors(whichdoors, floor, manual);
			else
				ReportError("Invalid door " + ToString(i));
		}
	}
	else if (doorhold_direction == 1 && sbs->camera->MouseDown == false)
	{
		//require button to be held down to open doors

		bool closedstate = false;

		for (int i = start; i <= end; i++)
		{
			//check door states first
			if (GetDoor(i))
			{
				if (GetDoor(i)->AreDoorsOpen() == false)
				{
					closedstate = true;
					break;
				}
			}
			else
				ReportError("Invalid door " + ToString(i));
		}

		for (int i = start; i <= end; i++)
		{
			//close doors using persistent values, if button is released before doors are fully open
			if (GetDoor(i))
			{
				if (closedstate == true)
					GetDoor(i)->CloseDoors(doorhold_whichdoors, doorhold_floor, doorhold_manual);
			}
			else
				ReportError("Invalid door " + ToString(i));
		}

		//reset persistent values
		doorhold_direction = 0;
		doorhold_whichdoors = 0;
		doorhold_floor = 0;
		doorhold_manual = false;
	}
	else if (doorhold_direction == 2)
	{
		//hold doors while button is held

		if (AreDoorsOpen(number) == true && AreDoorsMoving(number) == false)
		{
			if (sbs->camera->MouseDown == true)
			{
				//hold doors while button is held down
				HoldDoors(number);
				return true;
			}
			else
			{
				//run door open again to reset doors (turn off hold) if button is released
				for (int i = start; i <= end; i++)
				{
					//open doors using persistent values
					if (GetDoor(i))
						GetDoor(i)->OpenDoors(doorhold_whichdoors, doorhold_floor, doorhold_manual);
					else
						ReportError("Invalid door " + ToString(i));
				}
			}
		}

		if (sbs->camera->MouseDown == false)
		{
			//reset persistent values
			doorhold_direction = 0;
			doorhold_whichdoors = 0;
			doorhold_floor = 0;
			doorhold_manual = false;
		}
	}

	return true;
}

void Elevator::CloseDoors(int number, int whichdoors, int floor, bool manual, bool hold)
{
	//Closes elevator doors
	//if hold is true, sets 'hold' state requiring button to be held to keep doors closing

	//WhichDoors is the doors to move:
	//1 = both shaft and elevator doors
	//2 = only elevator doors
	//3 = only shaft doors

	//turn on hold option for certain modes
	if (IndependentService == true || FireServicePhase2 == 1)
		hold = true;

	int start = number, end = number;
	if (number == 0)
	{
		start = 1;
		end = NumDoors;
	}
	if (doorhold_direction == 0)
	{
		for (int i = start; i <= end; i++)
		{
			if (GetDoor(i))
				GetDoor(i)->CloseDoors(whichdoors, floor, manual);
			else
				ReportError("Invalid door " + ToString(i));
		}

		if (hold == true)
		{
			//set persistent values
			doorhold_direction = -1;
			doorhold_whichdoors = whichdoors;
			doorhold_floor = floor;
			doorhold_manual = manual;
		}
	}
	else if (doorhold_direction == -1 && sbs->camera->MouseDown == false)
	{
		bool openstate = false;
		for (int i = start; i <= end; i++)
		{
			//check door states first
			if (GetDoor(i))
			{
				if (GetDoor(i)->AreDoorsOpen() == true)
				{
					openstate = true;
					break;
				}
			}
			else
				ReportError("Invalid door " + ToString(i));
		}

		if (openstate == true)
		{
			for (int i = start; i <= end; i++)
			{
				//open doors using persistent values, if button is released before doors are fully closed
				if (GetDoor(i))
					GetDoor(i)->OpenDoors(doorhold_whichdoors, doorhold_floor, doorhold_manual);
				else
					ReportError("Invalid door " + ToString(i));
			}
		}

		//reset persistent values
		doorhold_direction = 0;
		doorhold_whichdoors = 0;
		doorhold_floor = 0;
		doorhold_manual = false;
	}
}

void Elevator::StopDoors(int number)
{
	//stops doors that are currently moving; can only be used for manual/emergency movements
	//this basically just resets the door internals

	int start = number, end = number;
	if (number == 0)
	{
		start = 1;
		end = NumDoors;
	}
	for (int i = start; i <= end; i++)
	{
		if (GetDoor(i))
			GetDoor(i)->StopDoors();
		else
			ReportError("Invalid door " + ToString(i));
	}
}

ElevatorDoor::DoorWrapper* Elevator::AddDoors(int number, const std::string &lefttexture, const std::string &righttexture, float thickness, float CenterX, float CenterZ, float width, float height, bool direction, float tw, float th)
{
	//adds elevator doors specified at a relative central position (off of elevator origin)
	//if direction is false, doors are on the left/right side; otherwise front/back

	if (GetDoor(number))
		return GetDoor(number)->AddDoors(lefttexture, righttexture, thickness, CenterX, CenterZ, width, height, direction, tw, th);
	else
		ReportError("Invalid door " + ToString(number));
	return 0;
}

bool Elevator::AddShaftDoors(int number, const std::string &lefttexture, const std::string &righttexture, float thickness, float CenterX, float CenterZ, float voffset, float tw, float th)
{
	//adds shaft's elevator doors specified at a relative central position (off of elevator origin)
	//uses some parameters (width, height, direction) from AddDoors function

	if (GetDoor(number))
		return GetDoor(number)->AddShaftDoors(lefttexture, righttexture, thickness, CenterX, CenterZ, voffset, tw, th);
	else
		ReportError("Invalid door " + ToString(number));
	return false;
}

ElevatorDoor::DoorWrapper* Elevator::AddShaftDoor(int floor, int number, const std::string &lefttexture, const std::string &righttexture, float tw, float th)
{
	//adds a single elevator shaft door on the specified floor, with position and thickness parameters first specified
	//by the SetShaftDoors command.

	if (IsServicedFloor(floor) == true && GetDoor(number))
		return GetDoor(number)->AddShaftDoor(floor, lefttexture, righttexture, tw, th);
	else
		return 0;
}

ElevatorDoor::DoorWrapper* Elevator::AddShaftDoor(int floor, int number, const std::string &lefttexture, const std::string &righttexture, float thickness, float CenterX, float CenterZ, float voffset, float tw, float th)
{
	//adds a single elevator shaft door on the specified floor, with position and thickness parameters first specified
	//by the SetShaftDoors command.

	if (IsServicedFloor(floor) == true && GetDoor(number))
		return GetDoor(number)->AddShaftDoor(floor, lefttexture, righttexture, thickness, CenterX, CenterZ, voffset, tw, th);
	else
		return 0;
}

void Elevator::ShaftDoorsEnabled(int number, int floor, bool value)
{
	//turns shaft elevator doors on/off

	SBS_PROFILE("Elevator::ShaftDoorsEnabled");

	int start = number, end = number;
	if (number == 0)
	{
		start = 1;
		end = NumDoors;
	}
	for (int i = start; i <= end; i++)
	{
		ElevatorDoor *door = GetDoor(i);
		if (door)
			door->ShaftDoorsEnabled(floor, value);
		else
			ReportError("Invalid door " + ToString(i));
	}
}

void Elevator::ShaftDoorsEnabledRange(int number, int floor, int range)
{
	//turn on a range of floors
	//if range is 3, show shaft door on current floor (floor), and 1 floor below and above (3 total floors)
	//if range is 1, show door on only the current floor (floor)

	SBS_PROFILE("Elevator::ShaftDoorsEnabledRange");

	int start = number, end = number;
	if (number == 0)
	{
		start = 1;
		end = NumDoors;
	}
	for (int i = start; i <= end; i++)
	{
		ElevatorDoor *door = GetDoor(i);
		if (door)
			door->ShaftDoorsEnabledRange(floor, range);
		else
			ReportError("Invalid door " + ToString(i));
	}
}

bool Elevator::AreDoorsOpen(int number)
{
	//returns the internal door state

	SBS_PROFILE("Elevator::AreDoorsOpen");

	int start = number, end = number;
	if (number == 0)
	{
		start = 1;
		end = NumDoors;
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
			ReportError("Invalid door " + ToString(i));
	}
	return false;
}

bool Elevator::AreShaftDoorsOpen(int number, int floor)
{
	//returns the internal shaft door state

	SBS_PROFILE("Elevator::AreShaftDoorsOpen");
	ElevatorDoor *door = GetDoor(number);
	if (door)
		return door->AreShaftDoorsOpen(floor);
	else
		ReportError("Invalid door " + ToString(number));
	return false;
}

bool Elevator::AreShaftDoorsClosed()
{
	//return true if all shaft doors are closed and not moving

	for (int i = 0; i < (int)DoorArray.size(); i++)
	{
		if (DoorArray[i])
		{
			if (DoorArray[i]->AreShaftDoorsClosed() == false)
				return false;
		}
	}
	return true;
}

void Elevator::Chime(int number, int floor, bool direction)
{
	//play chime sound on specified floor

	SBS_PROFILE("Elevator::Chime");

	int start = number, end = number;
	if (number == 0)
	{
		start = 1;
		end = NumDoors;
	}
	for (int i = start; i <= end; i++)
	{
		ElevatorDoor *door = GetDoor(i);
		if (door)
			door->Chime(floor, direction);
		else
			ReportError("Invalid door " + ToString(i));
	}
	if (direction == true)
		LastChimeDirection = 1;
	else
		LastChimeDirection = -1;
}

void Elevator::ResetDoors(int number, bool sensor)
{
	//reset elevator door timer

	int start = number, end = number;
	if (number == 0)
	{
		start = 1;
		end = NumDoors;
	}
	for (int i = start; i <= end; i++)
	{
		ElevatorDoor *door = GetDoor(i);
		if (door)
			door->Reset(sensor);
		else
			ReportError("Invalid door " + ToString(i));
	}
}

bool Elevator::DoorsStopped(int number)
{
	//return true if doors are stopped

	int start = number, end = number;
	if (number == 0)
	{
		start = 1;
		end = NumDoors;
	}
	for (int i = start; i <= end; i++)
	{
		ElevatorDoor *door = GetDoor(i);
		if (door)
		{
			if (door->DoorsStopped() == true)
				return true;
		}
		else
			ReportError("Invalid door " + ToString(i));
	}
	return false;
}

int Elevator::AreDoorsMoving(int number, bool car_doors, bool shaft_doors)
{
	//returns 1 if doors are opening (2 manual), -1 if doors are closing (-2 manual), or 0 if doors are not moving
	//if the type of door is specified, returns true if that type of door is moving

	int start = number, end = number;
	if (number == 0)
	{
		start = 1;
		end = NumDoors;
	}
	for (int i = start; i <= end; i++)
	{
		ElevatorDoor *door = GetDoor(i);
		if (door)
		{
			if (door->AreDoorsMoving(0, car_doors, shaft_doors) == true)
				return door->OpenDoor;
		}
		else
			ReportError("Invalid door " + ToString(i));
	}
	return 0;
}

bool Elevator::AreDoorsOpening(int number, bool car_doors, bool shaft_doors)
{
	//returns true if doors are opening

	if (AreDoorsMoving(number, car_doors, shaft_doors) == 1)
		return true;
	return false;
}

bool Elevator::AreDoorsClosing(int number, bool car_doors, bool shaft_doors)
{
	//returns true if doors are closing

	if (AreDoorsMoving(number, car_doors, shaft_doors) == -1)
		return true;
	return false;
}

void Elevator::EnableDoors(bool value)
{
	//enable/disable all doors

	SBS_PROFILE("Elevator::EnableDoors");
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

void Elevator::SetShaftDoors(int number, float thickness, float CenterX, float CenterZ)
{
	int start = number, end = number;
	if (number == 0)
	{
		start = 1;
		end = NumDoors;
	}
	for (int i = start; i <= end; i++)
	{
		if (GetDoor(i))
			GetDoor(i)->SetShaftDoors(thickness, CenterX, CenterZ);
		else
			ReportError("Invalid door " + ToString(i));
	}
}

bool Elevator::AddFloorSigns(int door_number, bool relative, const std::string &texture_prefix, const std::string &direction, float CenterX, float CenterZ, float width, float height, float voffset)
{
	//adds floor signs at the specified position and direction for each serviced floor,
	//depending on if the given door number services the floor or not (unless door_number is 0)

	float x, z;
	if (relative == true)
	{
		x = GetPosition().x + CenterX;
		z = GetPosition().z + CenterZ;
	}
	else
	{
		x = CenterX;
		z = CenterZ;
	}

	//make sure specified door exists before continuing
	if (door_number != 0)
	{
		if (DoorExists(door_number) == false)
			return ReportError("AddFloorSigns: door " + ToString(door_number) + " does not exist");
	}

	bool autosize_x, autosize_y;
	sbs->GetAutoSize(autosize_x, autosize_y);
	sbs->SetAutoSize(false, false);

	for (int i = 0; i < (int)ServicedFloors.size(); i++)
	{
		bool door_result = false;
		int floor = ServicedFloors[i];
		float base = GetDestinationOffset(floor);

		if (door_number != 0)
			door_result = ShaftDoorsExist(door_number, floor);

		if ((door_number == 0 || door_result == true) && sbs->GetFloor(floor))
		{
			std::string texture = texture_prefix + sbs->GetFloor(floor)->ID;
			std::string tmpdirection = direction;
			SetCase(tmpdirection, false);

			if (tmpdirection == "front" || tmpdirection == "left")
				sbs->DrawWalls(true, false, false, false, false, false);
			else
				sbs->DrawWalls(false, true, false, false, false, false);

			if (tmpdirection == "front" || tmpdirection == "back")
				sbs->GetFloor(floor)->AddWall("Floor Sign", texture, 0, x - (width / 2), z, x + (width / 2), z, height, height, base + voffset, base + voffset, 1, 1, false);
			else
				sbs->GetFloor(floor)->AddWall("Floor Sign", texture, 0, x, z - (width / 2), x, z + (width / 2), height, height, base + voffset, base + voffset, 1, 1, false);
			sbs->ResetWalls();
		}
	}
	sbs->SetAutoSize(autosize_x, autosize_y);
	return true;
}

void Elevator::SetCallButtons(int floor, bool direction, bool value)
{
	//sets light status of all associated call buttons on the specified floor
	//for direction, true is up and false is down

	//get call buttons associated with this elevator
	if (sbs->Verbose)
		Report("SetCallButtons: getting associated call buttons");

	if (!sbs->GetFloor(floor))
		return;

	std::vector<int> buttons = sbs->GetFloor(floor)->GetCallButtons(Number);

	for (int i = 0; i < (int)buttons.size(); i++)
	{
		CallButton *button = 0;
		if ((int)sbs->GetFloor(floor)->CallButtonArray.size() > buttons[i])
			button = sbs->GetFloor(floor)->CallButtonArray[buttons[i]];
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
	if (MoveElevator == false && (AreDoorsOpen() == false || UpPeak == true || DownPeak == true) && AreDoorsMoving() == 0 && Running == true)
		return true;
	else
		return false;
}

void Elevator::ResetQueue(bool up, bool down)
{
	//reset queues

	QueuePending = false;

	if (up == true)
	{
		if (sbs->Verbose)
			Report("QueueReset: resetting up queue");
		UpQueue.clear();
	}
	if (down == true)
	{
		if (sbs->Verbose)
			Report("QueueReset: resetting down queue");
		DownQueue.clear();
	}

	ResetLights();
}

void Elevator::ResetLights()
{
	//turn off button lights
	if (sbs->Verbose)
		Report("turning off button lights");

	for (int i = 0; i < (int)PanelArray.size(); i++)
		PanelArray[i]->ChangeAllLights(false);
}

void Elevator::ChangeLight(int floor, bool value)
{
	//turn on or off specified button lights

	if (value == true)
	{
		if (sbs->Verbose)
			Report("turning on button lights for floor " + ToString(floor));
	}
	else
	{
		if (sbs->Verbose)
			Report("turning off button lights for floor " + ToString(floor));
	}

	for (int i = 0; i < (int)PanelArray.size(); i++)
		PanelArray[i]->ChangeLight(floor, value);
}

void Elevator::SetBeepSound(const std::string &filename)
{
	//set sound used for floor beeps
	if (sbs->Verbose)
		Report("setting beep sound");
	BeepSound = filename;
	TrimString(BeepSound);
	UseFloorBeeps = true;
}

void Elevator::SetFloorSound(const std::string &prefix)
{
	//set prefix of floor sound
	if (sbs->Verbose)
		Report("setting floor sound");
	FloorSound = prefix;
	TrimString(FloorSound);
	UseFloorSounds = true;
}

void Elevator::SetMessageSound(bool type, bool direction, const std::string &filename)
{
	//if type is true, sets up and down messages.  If false, sets open and close messages
	//if direction is true, set up message sound; otherwise set down message sound

	if (type == true)
	{
		if (direction == true)
		{
			if (sbs->Verbose)
				Report("setting up message sound");
			UpMessageSound = filename;
			TrimString(UpMessageSound);
		}
		else
		{
			if (sbs->Verbose)
				Report("setting down message sound");
			DownMessageSound = filename;
			TrimString(DownMessageSound);
		}
		UseDirMessageSounds = true;
	}
	else
	{
		if (direction == true)
		{
			if (sbs->Verbose)
				Report("setting open message sound");
			OpenMessageSound = filename;
			TrimString(OpenMessageSound);
		}
		else
		{
			if (sbs->Verbose)
				Report("setting close message sound");
			CloseMessageSound = filename;
			TrimString(CloseMessageSound);
		}
		UseDoorMessageSounds = true;
	}
}

Sound* Elevator::AddSound(const std::string &name, const std::string &filename, Ogre::Vector3 position, bool loop, float volume, int speed, float min_distance, float max_distance, float doppler_level, float cone_inside_angle, float cone_outside_angle, float cone_outside_volume, Ogre::Vector3 direction)
{
	//create a sound object
	Sound *sound = new Sound(this, name, false);
	sounds.push_back(sound);

	//set parameters and play sound
	sound->Move(position);
	sound->SetDirection(direction);
	sound->SetVolume(volume);
	sound->SetSpeed(speed);
	sound->SetDistances(min_distance, max_distance);
	sound->SetDirection(direction);
	sound->SetDopplerLevel(doppler_level);
	sound->SetConeSettings(cone_inside_angle, cone_outside_angle, cone_outside_volume);
	sound->Load(filename);
	sound->SetLoopState(loop);
	if (loop && sbs->IsRunning == true && InElevator() == true)
		sound->Play();

	return sound;
}

void Elevator::DeleteActiveRoute()
{
	if (Running == false)
	{
		ReportError("Elevator not running");
		return;
	}

	//deletes the active route
	if (sbs->Verbose)
		Report("deleting active route");
	DeleteRoute(ActiveCallFloor, ActiveCallDirection);
	ActiveCallFloor = 0;
	ActiveCallDirection = 0;
}

bool Elevator::IsQueueActive()
{
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

void Elevator::Report(const std::string &message)
{
	//general reporting function
	sbs->Report("Elevator " + ToString(Number) + ": " + message);
}

bool Elevator::ReportError(const std::string &message)
{
	//general reporting function
	return sbs->ReportError("Elevator " + ToString(Number) + ": " + message);
}

ElevatorDoor::DoorWrapper* Elevator::AddDoorComponent(int number, const std::string &name, const std::string &texture, const std::string &sidetexture, float thickness, const std::string &direction, float OpenSpeed, float CloseSpeed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th)
{
	//adds an elevator door component to the specified door at a relative central position (off of elevator origin)

	if (GetDoor(number))
		return GetDoor(number)->AddDoorComponent(name, texture, sidetexture, thickness, direction, OpenSpeed, CloseSpeed, x1, z1, x2, z2, height, voffset, tw, th, side_tw, side_th);
	else
		ReportError("Invalid door " + ToString(number));
	return 0;
}

ElevatorDoor::DoorWrapper* Elevator::AddShaftDoorComponent(int number, int floor, const std::string &name, const std::string &texture, const std::string &sidetexture, float thickness, const std::string &direction, float OpenSpeed, float CloseSpeed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th)
{
	//adds a single elevator shaft door component on the specified floor

	if (IsServicedFloor(floor) == true && GetDoor(number))
		return GetDoor(number)->AddShaftDoorComponent(floor, name, texture, sidetexture, thickness, direction, OpenSpeed, CloseSpeed, x1, z1, x2, z2, height, voffset, tw, th, side_tw, side_th);
	else
		return 0;
}

void Elevator::AddShaftDoorsComponent(int number, const std::string &name, const std::string &texture, const std::string &sidetexture, float thickness, const std::string &direction, float OpenSpeed, float CloseSpeed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th)
{
	//adds shaft's elevator door components specified at a relative central position (off of elevator origin)

	if (GetDoor(number))
		GetDoor(number)->AddShaftDoorsComponent(name, texture, sidetexture, thickness, direction, OpenSpeed, CloseSpeed, x1, z1, x2, z2, height, voffset, tw, th, side_tw, side_th);
	else
		ReportError("Invalid door " + ToString(number));
}

ElevatorDoor::DoorWrapper* Elevator::FinishDoors(int number, bool DoorWalls, bool TrackWalls)
{
	//finishes elevator door

	if (GetDoor(number))
		return GetDoor(number)->FinishDoors(DoorWalls, TrackWalls);
	else
		ReportError("Invalid door " + ToString(number));
	return 0;
}

ElevatorDoor::DoorWrapper* Elevator::FinishShaftDoor(int number, int floor, bool DoorWalls, bool TrackWalls)
{
	//finishes a single shaft door

	if (IsServicedFloor(floor) == true && GetDoor(number))
		return GetDoor(number)->FinishShaftDoor(floor, DoorWalls, TrackWalls);
	else
		return 0;
}

bool Elevator::FinishShaftDoors(int number, bool DoorWalls, bool TrackWalls)
{
	//finishes all shaft doors

	if (GetDoor(number))
		return GetDoor(number)->FinishShaftDoors(DoorWalls, TrackWalls);
	else
		ReportError("Invalid door " + ToString(number));
	return false;
}

void Elevator::Timer::Notify()
{
	if (elevator->IsRunning() == false)
		return;

	SBS_PROFILE("Elevator::Timer::Notify");
	if (type == 0)
	{
		//parking timer

		if (elevator->ParkingDelay > 0 && elevator->IsIdle() == true && elevator->InServiceMode() == false)
		{
			int floor = elevator->GetFloor();
			if (elevator->ParkingFloor != floor)
			{
				elevator->Report("parking to floor " + ToString(elevator->ParkingFloor));
				elevator->Parking = true;
			}

			if (elevator->ParkingFloor > floor)
				elevator->AddRoute(elevator->ParkingFloor, 1, 2);
			else if (elevator->ParkingFloor < floor)
				elevator->AddRoute(elevator->ParkingFloor, -1, 2);

			Stop();
		}
		else if (elevator->InServiceMode() == true)
			Stop(); //stop timer if in service mode
	}
	else if (type == 1)
	{
		//random call timer

		if (elevator->RandomActivity == true && elevator->InServiceMode() == false)
		{
			RandomGen rnd_main(time(0) + elevator->Number);
			RandomGen rnd_floor(sbs->GetRunTime() + elevator->Number);

			int num, floor;

			//get call probability
			if (elevator->RandomProbability > 1)
				num = rnd_main.Get(elevator->RandomProbability - 1);
			else
				num = 0;

			//get call floor
			int index = rnd_floor.Get(elevator->ServicedFloors.size());
			floor = elevator->ServicedFloors[index];

			//if probability number matched, press selected floor button
			if (num == 0 && elevator->IsQueued(floor) == false && floor != elevator->GetFloor())
				elevator->PressFloorButton(floor);
		}
		else if (elevator->InServiceMode() == true)
			Stop(); //stop timer if in service mode
	}
	else if (type > 1)
	{
		//arrival and departure timers
		elevator->WaitForTimer = false;
	}
}

ButtonPanel* Elevator::GetPanel(int index)
{
	//get a button panel object

	if (index > (int)PanelArray.size() || index < 1)
		return 0;

	return PanelArray[index - 1];
}

int Elevator::GetRandomLobby()
{
	//return random lobby floor value
	return RandomLobby;
}

void Elevator::SetRandomLobby(int floor)
{
	//set random lobby floor
	RandomLobby = floor;
	RandomLobbySet = true;
}

void Elevator::PressFloorButton(int floor)
{
	//press a floor button

	if (Running == false)
	{
		ReportError("Elevator not running");
		return;
	}

	int index = 0;

	if (PanelArray.size() > 0)
	{
		for (int i = 0; i < (int)PanelArray.size(); i++)
		{
			index = PanelArray[i]->GetFloorButtonIndex(floor);
			if (index >= 0)
			{
				PanelArray[i]->Press(index);
				return;
			}
		}
	}
}

bool Elevator::IsQueued(int floor)
{
	//return true if the given floor is in either queue

	for (int i = 0; i < (int)UpQueue.size(); i++)
	{
		if (UpQueue[i].floor == floor)
			return true;
	}

	for (int i = 0; i < (int)DownQueue.size(); i++)
	{
		if (DownQueue[i].floor == floor)
			return true;
	}

	return false;
}

void Elevator::HoldDoors(int number, bool sensor)
{
	//hold specified door, or all if "0" is given

	int start = number, end = number;
	if (number == 0)
	{
		start = 1;
		end = NumDoors;
	}
	for (int i = start; i <= end; i++)
	{
		if (GetDoor(i))
			GetDoor(i)->Hold(sensor);
		else
			ReportError("Invalid door " + ToString(i));
	}
}

Door* Elevator::AddDoor(const std::string &open_sound, const std::string &close_sound, bool open_state, const std::string &texture, float thickness, int direction, float speed, float CenterX, float CenterZ, float width, float height, float voffset, float tw, float th)
{
	//interface to the SBS AddDoor function

	if (direction > 8 || direction < 1)
	{
		ReportError("Door direction out of range");
		return 0;
	}

	/*float x1, z1, x2, z2;
	//set up coordinates
	if (direction < 5)
	{
		x1 = CenterX;
		x2 = CenterX;
		z1 = CenterZ - (width / 2);
		z2 = CenterZ + (width / 2);
	}
	else
	{
		x1 = CenterX - (width / 2);
		x2 = CenterX + (width / 2);
		z1 = CenterZ;
		z2 = CenterZ;
	}

	//cut area
	if (direction < 5)
		CutAll(Ogre::Vector3(x1 - 1, GetBase(true) + voffset, z1), Ogre::Vector3(x2 + 1, GetBase(true) + voffset + height, z2), true, false);
	else
		CutAll(Ogre::Vector3(x1, GetBase(true) + voffset, z1 - 1), Ogre::Vector3(x2, GetBase(true) + voffset + height, z2 + 1), true, false);
	*/

	std::string elevnum = ToString(Number);
	std::string num = ToString((int)StdDoorArray.size());
	std::string name = "Elevator " + elevnum + ":Door " + num;
	Door* door = new Door(this, name, open_sound, close_sound, open_state, texture, thickness, direction, speed, CenterX, CenterZ, width, height, voffset, tw, th);
	StdDoorArray.push_back(door);
	return door;
}

Door* Elevator::GetStdDoor(int number)
{
	//get door object
	if (number < (int)StdDoorArray.size())
	{
		if (StdDoorArray[number])
			return StdDoorArray[number];
	}

	return 0;
}
void Elevator::RemovePanel(ButtonPanel* panel)
{
	//remove a button panel reference (does not delete the object itself)
	for (int i = 0; i < (int)PanelArray.size(); i++)
	{
		if (PanelArray[i] == panel)
		{
			PanelArray.erase(PanelArray.begin() + i);
			return;
		}
	}
}

void Elevator::RemoveDirectionalIndicator(DirectionalIndicator* indicator)
{
	//remove a directional indicator reference (does not delete the object itself)
	for (int i = 0; i < (int)DirIndicatorArray.size(); i++)
	{
		if (DirIndicatorArray[i] == indicator)
		{
			DirIndicatorArray.erase(DirIndicatorArray.begin() + i);
			return;
		}
	}
}

void Elevator::RemoveElevatorDoor(ElevatorDoor* door)
{
	//remove an elevator door reference (does not delete the object itself)
	for (int i = 0; i < (int)DoorArray.size(); i++)
	{
		if (DoorArray[i] == door)
		{
			DoorArray.erase(DoorArray.begin() + i);
			NumDoors--;

			//reset cache values
			lastdoor_number = 0;
			lastdoor_result = 0;
			return;
		}
	}
}

void Elevator::RemoveFloorIndicator(FloorIndicator* indicator)
{
	//remove a floor indicator reference (does not delete the object itself)
	for (int i = 0; i < (int)FloorIndicatorArray.size(); i++)
	{
		if (FloorIndicatorArray[i] == indicator)
		{
			FloorIndicatorArray.erase(FloorIndicatorArray.begin() + i);
			return;
		}
	}
}

void Elevator::RemoveDoor(Door* door)
{
	//remove a door reference (does not delete the object itself)
	for (int i = 0; i < (int)StdDoorArray.size(); i++)
	{
		if (StdDoorArray[i] == door)
		{
			StdDoorArray.erase(StdDoorArray.begin() + i);
			return;
		}
	}
}

void Elevator::RemoveSound(Sound *sound)
{
	//remove a sound reference (does not delete the object itself)
	for (int i = 0; i < (int)sounds.size(); i++)
	{
		if (sounds[i] == sound)
		{
			sounds.erase(sounds.begin() + i);
			return;
		}
	}
}

void Elevator::RemoveLight(Light *light)
{
	//remove a light reference (does not delete the object itself)
	for (int i = 0; i < (int)lights.size(); i++)
	{
		if (lights[i] == light)
		{
			lights.erase(lights.begin() + i);
			return;
		}
	}
}

void Elevator::RemoveModel(Model *model)
{
	//remove a model reference (does not delete the object itself)
	for (int i = 0; i < (int)ModelArray.size(); i++)
	{
		if (ModelArray[i] == model)
		{
			ModelArray.erase(ModelArray.begin() + i);
			return;
		}
	}
}

void Elevator::RemoveControl(Control *control)
{
	//remove a control reference (does not delete the object itself)
	for (int i = 0; i < (int)ControlArray.size(); i++)
	{
		if (ControlArray[i] == control)
		{
			ControlArray.erase(ControlArray.begin() + i);
			return;
		}
	}
}

void Elevator::RemoveTrigger(Trigger *trigger)
{
	//remove a trigger reference (does not delete the object itself)
	for (int i = 0; i < (int)TriggerArray.size(); i++)
	{
		if (TriggerArray[i] == trigger)
		{
			TriggerArray.erase(TriggerArray.begin() + i);
			return;
		}
	}
}

void Elevator::NotifyArrival(int floor)
{
	//notify on elevator arrival (play chime and turn on related directional indicator lantern)

	//do not notify if in a service mode
	if (InServiceMode() == true)
		return;

	//get call button status
	bool up = false, down = false;
	GetCallButtonStatus(floor, up, down);

	//play chime sound and change indicator
	if (GetArrivalDirection(floor) == true)
	{
		if (up == true)
			Chime(0, floor, true);
		SetDirectionalIndicators(floor, true, false);
		LastChimeDirection = 1;
	}
	else
	{
		if (down == true)
			Chime(0, floor, false);
		SetDirectionalIndicators(floor, false, true);
		LastChimeDirection = -1;
	}

	if (FireServicePhase1 == 0 && FireServicePhase2 == 0)
		PlayFloorSound();

	Notified = true;
}

bool Elevator::GetArrivalDirection(int floor)
{
	//determine if the directional lantern should show up or down on arrival to the specified floor
	//true for up, false for down

	int newfloor = floor;

	if (floor == GetTopFloor())
		return false; //turn on down light if on top floor
	if (floor == GetBottomFloor())
		return true; //turn on up light if on bottom floor

	//chime queue direction if queue resets are on
	if (QueueResets == true)
	{
		if (QueuePositionDirection == 1)
			return true;
		else if (QueuePositionDirection == -1)
			return false;
	}

	//check for active hall calls
	bool UpStatus, DownStatus;
	GetCallButtonStatus(floor, UpStatus, DownStatus);
	if (UpStatus == true && QueuePositionDirection == 1)
		return true;
	if (DownStatus == true && QueuePositionDirection == -1)
		return false;

	if (NotifyEarly == 0)
	{
		if (QueuePositionDirection == 1 && UpQueue.size() > 0 && UpQueueEmpty == false)
			newfloor = UpQueue[0].floor;

		if (QueuePositionDirection == -1 && DownQueue.size() > 0 && DownQueueEmpty == false)
			newfloor = DownQueue[DownQueue.size() - 1].floor;

		if (QueuePositionDirection == 1 && DownQueue.size() > 0 && UpQueueEmpty == true)
			newfloor = DownQueue[DownQueue.size() - 1].floor;

		if (QueuePositionDirection == -1 && UpQueue.size() > 0 && DownQueueEmpty == true)
			newfloor = UpQueue[0].floor;
	}
	else
	{
		if (QueuePositionDirection == 1 && UpQueue.size() > 1)
			newfloor = UpQueue[1].floor;

		if (QueuePositionDirection == -1 && DownQueue.size() > 1)
			newfloor = DownQueue[DownQueue.size() - 2].floor;

		if (QueuePositionDirection == 1 && UpQueue.size() == 1)
		{
			if (DownQueue.size() > 0)
				newfloor = DownQueue[DownQueue.size() - 1].floor;
			else
				return true;
		}

		if (QueuePositionDirection == -1 && DownQueue.size() == 1)
		{
			if (UpQueue.size() > 0)
				newfloor = UpQueue[0].floor;
			else
				return false;
		}
	}

	if (QueuePositionDirection == 1 && UpQueue.size() == 0 && DownQueue.size() == 0)
		return true;

	if (QueuePositionDirection == -1 && UpQueue.size() == 0 && DownQueue.size() == 0)
		return false;

	if (newfloor >= floor)
		return true; //turn on up for entry above current floor
	else
		return false; //turn on down for entry below current floor
}

void Elevator::SetRunState(bool value)
{
	//set elevator run state (true for run, false for stop)

	if (Running == value)
		return;

	if (value == false && IsMoving == true)
	{
		if (InspectionService == true)
			Stop();
		else
			Stop(true);
	}

	//switch off directional indicators on current floor
	if (value == false && IsMoving == false)
		SetDirectionalIndicators(ElevatorFloor, false, false);

	if (value == false)
		Report("Elevator stopped");
	else
		Report("Elevator running");

	Running = value;
}

bool Elevator::IsRunning()
{
	//return elevator running state
	return Running;
}

bool Elevator::PlayFloorBeep()
{
	//play floor beep sound

	if (InServiceMode() == true || BeepSound == "" || UseFloorBeeps == false)
		return false;

	if (sbs->Verbose)
		Report("playing floor beep sound");

	std::string newsound = BeepSound;
	//change the asterisk into the current floor number
	ReplaceAll(newsound, "*", ToString(GetFloor()));
	TrimString(newsound);
	floorbeep->Stop();
	floorbeep->Load(newsound);
	floorbeep->SetLoopState(false);
	floorbeep->Play();
	return true;
}

bool Elevator::PlayFloorSound()
{
	//play floor sound

	if (InServiceMode() == true || FloorSound == "" || UseFloorSounds == false || SkipFloorSound == true)
		return false;

	if (sbs->Verbose)
		Report("playing floor sound");

	std::string newsound = FloorSound;
	//change the asterisk into the current floor number
	ReplaceAll(newsound, "*", ToString(GotoFloor));
	TrimString(newsound);
	announcesnd->PlayQueued(newsound, false, false);
	return true;
}

bool Elevator::PlayMessageSound(bool type)
{
	//play message sound
	//if type is true, play directional up/down sounds, otherwise play door open/close sounds
	//if direction is true, play up sound; otherwise play down sound

	if (InServiceMode() == true)
		return false;

	if (IsQueueActive() == false && type == true)
		return false;

	std::string newsound;

	if (type == true)
	{
		//exit if directional message sounds are off, or one has already been queued
		if (UseDirMessageSounds == false || DirMessageSound == true)
			return false;

		int direction = LastChimeDirection;

		if (LastChimeDirection == 0)
			direction = LastQueueDirection;

		if (direction == 1)
		{
			if (UpMessageSound == "")
				return false;

			if (sbs->Verbose)
				Report("playing up message sound");

			newsound = UpMessageSound;
		}
		else
		{
			if (DownMessageSound == "")
				return false;

			if (sbs->Verbose)
				Report("playing down message sound");

			newsound = DownMessageSound;
		}

		DirMessageSound = true;
	}
	else
	{
		//exit if door message sounds are off, or one has already been queued
		if (UseDoorMessageSounds == false || DoorMessageSound == true)
			return false;

		if (AreDoorsOpening() == true)
		{
			if (OpenMessageSound == "")
				return false;

			if (sbs->Verbose)
				Report("playing open message sound");

			newsound = OpenMessageSound;
		}
		else if (AreDoorsClosing() == true)
		{
			if (CloseMessageSound == "")
				return false;

			if (sbs->Verbose)
				Report("playing close message sound");

			newsound = CloseMessageSound;
		}
		else
			return false;

		DoorMessageSound = true;
	}

	//change the asterisk into the current floor number
	ReplaceAll(newsound, "*", ToString(GetFloor()));
	TrimString(newsound);
	announcesnd->PlayQueued(newsound, false, false);
	return true;
}

bool Elevator::DoorExists(int number)
{
	//check if the specified door exists
	//if number is 0, return true if any door exists

	int start = number, end = number;
	if (number == 0)
	{
		start = 1;
		end = NumDoors;
	}
	for (int i = start; i <= end; i++)
	{
		if (GetDoor(i))
			return true;
	}
	return false;
}

bool Elevator::IsNudgeModeActive(int number)
{
	//checks doors and returns true if any (or the specified door) have nudge mode active

	int start = number, end = number;
	if (number == 0)
	{
		start = 1;
		end = NumDoors;
	}
	for (int i = start; i <= end; i++)
	{
		ElevatorDoor *door = GetDoor(i);
		if (door)
		{
			if (door->GetNudgeStatus() == true)
				return true;
		}
		else
			ReportError("Invalid door " + ToString(i));
	}

	return false;
}

void Elevator::EnableNudgeMode(bool value, int number)
{
	//enables nudge mode on all doors or the specified door

	int start = number, end = number;
	if (number == 0)
	{
		start = 1;
		end = NumDoors;
	}
	for (int i = start; i <= end; i++)
	{
		ElevatorDoor *door = GetDoor(i);
		if (door)
			door->EnableNudgeMode(value);
		else
			ReportError("Invalid door " + ToString(i));
	}
}

void Elevator::ResetNudgeTimer(bool start, int number)
{
	//resets and optionally starts nudge timer on the specified door

	int start2 = number, end = number;
	if (number == 0)
	{
		start2 = 1;
		end = NumDoors;
	}
	for (int i = start2; i <= end; i++)
	{
		ElevatorDoor *door = GetDoor(i);
		if (door)
		{
			door->ResetNudgeTimer(start);
		}
		else
			ReportError("Invalid door " + ToString(i));
	}
}

Light* Elevator::AddLight(const std::string &name, int type, Ogre::Vector3 position, Ogre::Vector3 direction, float color_r, float color_g, float color_b, float spec_color_r, float spec_color_g, float spec_color_b, float spot_inner_angle, float spot_outer_angle, float spot_falloff, float att_range, float att_constant, float att_linear, float att_quadratic)
{
	//add a global light

	Light* light = new Light(this, name, type, position, direction, color_r, color_g, color_b, spec_color_r, spec_color_g, spec_color_b, spot_inner_angle, spot_outer_angle, spot_falloff, att_range, att_constant, att_linear, att_quadratic);
	lights.push_back(light);
	return light;
}

Model* Elevator::AddModel(const std::string &name, const std::string &filename, bool center, Ogre::Vector3 position, Ogre::Vector3 rotation, float max_render_distance, float scale_multiplier, bool enable_physics, float restitution, float friction, float mass)
{
	//add a model
	Model* model = new Model(this, name, filename, center, position, rotation, max_render_distance, scale_multiplier, enable_physics, restitution, friction, mass);
	if (model->load_error == true)
	{
		delete model;
		return 0;
	}
	ModelArray.push_back(model);
	return model;
}

void Elevator::AddModel(Model *model)
{
	//add a model reference

	if (!model)
		return;

	for (int i = 0; i < (int)ModelArray.size(); i++)
	{
		if (ModelArray[i] == model)
			return;
	}

	ModelArray.push_back(model);
}

void Elevator::AddDisplayFloor(int floor)
{
	//add a floor to the display floors list
	DisplayFloors.push_back(floor);
}

std::string Elevator::GetFloorDisplay()
{
	//returns the current floor's indicator display string

	std::string value;
	int floornum = GetFloor();
	Floor *floor = sbs->GetFloor(floornum);

	if (!floor)
		return value;

	if (UseFloorSkipText == true && IsServicedFloor(floornum) == false)
		value = FloorSkipText;
	else
	{
		if (DisplayFloors.size() > 0)
		{
			for (int i = 0; i < (int)DisplayFloors.size(); i++)
			{
				if (floornum == DisplayFloors[i])
				{
					value = floor->ID;
					break;
				}
			}
		}
		else
			value = floor->ID;
	}
	return value;
}

Control* Elevator::AddControl(const std::string &name, const std::string &sound, const std::string &direction, float CenterX, float CenterZ, float width, float height, float voffset, std::vector<std::string> &action_names, std::vector<std::string> &textures)
{
	//add a control
	std::vector<Action*> actionnull; //not used
	Control* control = new Control(this, name, false, sound, action_names, actionnull, textures, direction, width, height, true);
	control->Move(Ogre::Vector3(CenterX, voffset, CenterZ));
	ControlArray.push_back(control);
	return control;
}

Trigger* Elevator::AddTrigger(const std::string &name, const std::string &sound_file, Ogre::Vector3 &area_min, Ogre::Vector3 &area_max, std::vector<std::string> &action_names)
{
	//add a trigger
	Trigger* trigger = new Trigger(this, name, false, sound_file, area_min, area_max, action_names);
	TriggerArray.push_back(trigger);
	return trigger;
}

bool Elevator::ReplaceTexture(const std::string &oldtexture, const std::string &newtexture)
{
	return ElevatorMesh->ReplaceTexture(oldtexture, newtexture);
}

std::vector<Sound*> Elevator::GetSound(const std::string &name)
{
	//get sound by name

	std::string findname = name;
	SetCase(findname, false);
	std::vector<Sound*> soundlist;
	for (int i = 0; i < (int)sounds.size(); i++)
	{
		if (sounds[i])
		{
			std::string name2 = sounds[i]->GetName();
			SetCase(name2, false);
			if (findname == name2)
				soundlist.push_back(sounds[i]);
		}
	}
	return soundlist;
}

int Elevator::GetFloorIndex(int floor)
{
	//return array index of the specified floor

	for (int i = 0; i < (int)ServicedFloors.size(); i++)
	{
		if (ServicedFloors[i] == floor)
			return i;
	}

	return -1;
}

float Elevator::GetDestinationAltitude(int floor)
{
	//returns the destination altitude of the specified floor, based on shaft door positioning

	if (IsServicedFloor(floor) == false)
		return 0.0f;

	float result = 0;
	bool found = false;
	for (int i = 0; i < (int)DoorArray.size(); i++)
	{
		if (DoorArray[i]->ShaftDoorsExist(floor) == true)
		{
			float altitude = DoorArray[i]->GetShaftDoorAltitude(floor);

			if (altitude > result || found == false)
			{
				result = altitude;
				found = true;
			}
		}
	}

	if (found == false)
	{
		if (sbs->GetFloor(floor))
			return sbs->GetFloor(floor)->GetBase();
	}
	return result;
}

float Elevator::GetDestinationOffset(int floor)
{
	//returns the offset distance from the floor's base altitude the elevator destination is

	if (IsServicedFloor(floor) == false)
		return 0.0f;

	if (sbs->GetFloor(floor))
		return GetDestinationAltitude(floor) - sbs->GetFloor(floor)->GetBase();

	return 0.0f;
}

void Elevator::Init()
{
	//startup elevator initialization

	//exit if not created properly
	if (Created == false)
		return;

	bool enable_elevators = sbs->GetConfigBool("Skyscraper.SBS.Elevator.IsEnabled", true);

	//turn on shaft doors
	ShaftDoorsEnabled(0, sbs->camera->StartFloor, true);

	//disable objects
	EnableObjects(false);
	if (enable_elevators == false)
		Enabled(false);

	//move elevator to starting position
	SetFloor(StartingFloor);
}

bool Elevator::GetCallButtonStatus(int floor, bool &Up, bool &Down)
{
	//returns status of associated call buttons on specified floor in UpStatus and DownStatus variables

	if (sbs->GetFloor(floor))
	{
		std::vector<int> buttons = sbs->GetFloor(floor)->GetCallButtons(Number);
		if (buttons.size() > 0)
		{
			CallButton *button =  sbs->GetFloor(floor)->CallButtonArray[buttons[0]];
			if (button)
			{
				Up = button->UpStatus;
				Down = button->DownStatus;
				return true;
			}
		}
	}

	Up = false;
	Down = false;
	return false;
}

int Elevator::AvailableForCall(int floor, int direction)
{
	//determines if the elevator is available for the specified hall call

	//return codes:
	//0 - busy and will eventually be available
	//1 - available for call
	//2 - unavailable due to a service mode or error

	//if floor is a serviced floor
	if (IsServicedFloor(floor) == true)
	{
		//if direction doesn't go beyond elevator's range
		if ((direction == 1 && floor < GetTopFloor()) || (direction == -1 && floor > GetBottomFloor()))
		{
			//if elevator is running
			if (IsRunning() == true)
			{
				//and if it's not in any service mode
				if (InServiceMode() == false)
				{
					//and if no queue changes are pending
					if (QueuePending == false)
					{
						//and if elevator either has limitqueue off, or has limitqueue on and queue direction is the same
						if (LimitQueue == false || (LimitQueue == true && (QueuePositionDirection == direction || QueuePositionDirection == 0)))
						{
							//and if elevator either has queueresets off, or has queueresets on and queue direction is the same
							if (QueueResets == false || (QueueResets == true && (QueuePositionDirection == direction || QueuePositionDirection == 0)))
							{
								//and if doors are not being held or elevator is waiting in a peak mode
								if (GetHoldStatus() == false || PeakWaiting() == true)
								{
									//and if nudge mode is off on all doors
									if (IsNudgeModeActive() == false)
									{
										//and if it's above the current floor and should be called down, or below the
										//current floor and called up, or on the same floor and not moving, or idle
										if ((GetFloor() > floor && direction == -1) || (GetFloor() < floor && direction == 1) || (GetFloor() == floor && MoveElevator == false) || IsIdle())
										{
											//and if it's either going the same direction as the call, on either the highest/lowest (terminal) floor, or idle
											if (QueuePositionDirection == direction || IsIdle())
											{
												if (sbs->Verbose)
													Report("Available for call");
												return 1;
											}
											else
											{
												if (sbs->Verbose == true)
													Report("Not available for call - going a different direction and is not idle");
												return 0;
											}
										}
										else
										{
											if (sbs->Verbose == true)
												Report("Not available for call - position/direction wrong for call and is not idle");
											return 0;
										}
									}
									else
									{
										if (sbs->Verbose == true)
											Report("Not available for call - in nudge mode");
										return 0;
									}
								}
								else
								{
									if (sbs->Verbose == true)
										Report("Not available for call - door hold is enabled");
									return 0;
								}
							}
							else
							{
								if (sbs->Verbose == true)
									Report("Not available for call - queueresets is on and opposite queue direction is active");
								return 0;
							}
						}
						else
						{
							if (sbs->Verbose == true)
								Report("Not available for call - limitqueue is on and queue is active");
							return 0;
						}
					}
					else
					{
						if (sbs->Verbose == true)
							Report("Not available for call - queue change is pending");
						return 0;
					}
				}
				else
				{
					if (sbs->Verbose == true)
						Report("Not available for call - in service mode");
					return 2;
				}
			}
			else
			{
				if (sbs->Verbose == true)
					Report("Not available for call - elevator not running");
				return 2;
			}
		}
		else
		{
			if (sbs->Verbose == true)
				Report("Not available for call - direction beyond serviced range");
			return 2;
		}
	}

	if (sbs->Verbose == true)
		Report("Not available for call - not a serviced floor");
	return 2;
}

bool Elevator::SelectFloor(int floor)
{
	//select a floor (in-elevator floor selections)

	//exit if in inspection mode or in fire service mode or is not running
	if (InspectionService == true)
		return ReportError("Cannot select floor while in inspection mode");
	else if (FireServicePhase1 == 1 && FireServicePhase2 == 0)
		return ReportError("Cannot select floor while in fire service recall mode");
	else if (Running == false)
		return false;

	//use Go routine instead if floorhold parameter is enabled
	if (FloorHold == true)
		return Go(floor, true);

	bool result = false;

	//elevator is above floor
	if (GetFloor() > floor)
		result = AddRoute(floor, -1, 0);

	//elevator is below floor
	if (GetFloor() < floor)
		result = AddRoute(floor, 1, 0);

	//elevator is on floor
	if (GetFloor() == floor)
	{
		if (Direction == 0)
		{
			//stopped - play chime and reopen doors
			if (ReOpen == true)
			{
				if (InServiceMode() == false)
				{
					int dir = 0;

					if (IsQueueActive() == true)
					{
						dir = LastChimeDirection;
						if (dir == 0)
							dir = LastQueueDirection;
					}

					if (dir == -1)
						Chime(0, floor, false);
					else if (dir == 1)
						Chime(0, floor, true);
				}
				if (FireServicePhase2 == 0)
					if (AutoDoors == true)
						OpenDoors();
				return false;
			}
		}
		else
		{
			//add a route to the current floor if elevator is moving
			result = AddRoute(floor, -Direction, 0);
		}
	}

	return result;
}

bool Elevator::Check(Ogre::Vector3 position)
{
	//check to see if user (camera) is in the elevator

	if (IsInElevator(position, true) == true)
	{
		if (InElevator() == false)
		{
			EnableObjects(true);
			UpdateFloorIndicators();
		}
		sbs->InElevator = true;
		sbs->ElevatorNumber = Number;
		sbs->ElevatorSync = true;
		return true;
	}

	//turn off objects if user has moved outside the checked elevator
	else if (InElevator() == true)
		EnableObjects(false);

	//if camera is within vertical elevator range, turn on syncing to allow things like elevator surfing
	else if (CameraOffset > Height && CameraOffset < Height * 2)
	{
		sbs->ElevatorNumber = Number;
		sbs->ElevatorSync = true;
		return true;
	}
	return false;
}

int Elevator::GetNearestServicedFloor()
{
	//return number of closest serviced floor

	if (IsServicedFloor(GetFloor()) == true)
		return GetFloor();

	if (ServicedFloors.size() == 0)
		return 0;

	bool firstrun = true;
	int nearest = 0;
	float nearest_difference = 0;

	for (int i = 0; i < (int)ServicedFloors.size() - 1; i++)
	{
		if (firstrun == true)
		{
			if (sbs->GetFloor(ServicedFloors[i]))
			{
				nearest_difference = fabsf(GetPosition().y - GetDestinationOffset(ServicedFloors[i]));
				nearest = i;
				firstrun = false;
			}
		}
		else
		{
			if (sbs->GetFloor(ServicedFloors[i]))
			{
				float difference = fabsf(GetPosition().y - GetDestinationOffset(ServicedFloors[i]));
				if (difference < nearest_difference)
				{
					//mark closest
					nearest_difference = difference;
					nearest = i;
				}
			}
		}
	}

	return ServicedFloors[nearest];
}

bool Elevator::ReturnToNearestFloor()
{
	//return and relevel to nearest floor

	if (IsIdle() == true && InServiceMode() == false)
	{
		int floor = GetNearestServicedFloor();
		Report("returning to nearest floor");
		Parking = true; //enable parking mode to prevent arrival notification

		if (floor >= GetFloor())
			AddRoute(floor, 1, 2);
		else
			AddRoute(floor, -1, 2);
		return true;
	}
	return false;
}

bool Elevator::IsLeveled()
{
	//return true if elevator is leveled at a serviced floor

	int floor = GetFloor();
	if (IsServicedFloor(floor) == true)
	{
		if (GetPosition().y == GetDestinationAltitude(floor))
			return true;
	}
	return false;
}

void Elevator::Up()
{
	//move elevator up (manual)
	//this function also stops the up movement when button is depressed

	if (Running == false)
	{
		ReportError("Elevator not running");
		return;
	}

	if (ManualMove == 0)
	{
		ManualMove = 1;

		//set direction
		Direction = 1;
		MoveElevator = true;
		if (sbs->Verbose)
			Report("Up: moving elevator");
		return;
	}
	else if (ManualMove == 1 && sbs->camera->MouseDown == false)
	{
		//stop movement
		ManualMove = 0;
		Stop();
	}
}

void Elevator::Down()
{
	//move elevator down (manual)
	//this function also stops the down movement when button is depressed

	if (Running == false)
	{
		ReportError("Elevator not running");
		return;
	}

	if (ManualMove == 0)
	{
		ManualMove = -1;

		//set direction
		Direction = -1;
		MoveElevator = true;
		if (sbs->Verbose)
			Report("Up: moving elevator");
		return;
	}
	else if (ManualMove == -1 && sbs->camera->MouseDown == false)
	{
		//stop movement
		ManualMove = 0;
		Stop();
	}
}

Shaft* Elevator::GetShaft()
{
	//get associated shaft object
	return sbs->GetShaft(AssignedShaft);
}

CallButton* Elevator::GetPrimaryCallButton()
{
	//returns the first call button associated with this elevator, on the fire recall (lobby) floor
	Floor *floor = sbs->GetFloor(RecallFloor);
	if (floor)
		return floor->GetCallButton(Number);
	return 0;
}

int Elevator::GetActiveCallFloor()
{
	return ActiveCallFloor;
}

int Elevator::GetActiveCallDirection()
{
	return ActiveCallDirection;
}

int Elevator::GetActiveCallType()
{
	return ActiveCallType;
}

bool Elevator::InElevator()
{
	//return true if user is currently in elevator

	return (sbs->InElevator == true && sbs->ElevatorNumber == Number);
}

void Elevator::PlayStartingSounds()
{
	//play elevator starting sounds

	//car sound
	carsound->Stop();
	if (Direction == 1 && CarUpStartSound.empty() == false && CarUpStartSound != "")
	{
		if (sbs->Verbose)
			Report("playing car up start sound");

		carsound->Load(CarUpStartSound);
		carsound->SetLoopState(false);
		carsound->Play();
	}
	if (Direction == -1 && CarDownStartSound.empty() == false && CarDownStartSound != "")
	{
		if (sbs->Verbose)
			Report("playing car down start sound");

		carsound->Load(CarDownStartSound);
		carsound->SetLoopState(false);
		carsound->Play();
	}

	//motor sound
	motorsound->Stop();
	if (Direction == 1 && MotorUpStartSound.empty() == false && MotorUpStartSound != "")
	{
		if (sbs->Verbose)
			Report("playing motor up start sound");

		motorsound->Load(MotorUpStartSound);
		motorsound->SetLoopState(false);
		motorsound->Play();
	}
	if (Direction == -1 && MotorDownStartSound.empty() == false && MotorDownStartSound != "")
	{
		if (sbs->Verbose)
			Report("playing motor down start sound");

		motorsound->Load(MotorDownStartSound);
		motorsound->SetLoopState(false);
		motorsound->Play();
	}
}

void Elevator::PlayStoppingSounds(bool emergency)
{
	//play elevator stopping sounds
	//if emergency is true, plays emergency stop sounds with a fallback to standard sounds

	bool carsound_play = false;
	bool motorsound_play = false;
	std::string CarSoundFile, MotorSoundFile;

	if (emergency == true)
	{
		//car sound
		if (CarEmergencyStopSound.empty() == false && CarEmergencyStopSound != "")
		{
			if (sbs->Verbose)
				Report("playing car emergency stop sound");

			CarSoundFile = CarEmergencyStopSound;
			carsound_play = true;
		}

		//motor sound
		if (MotorEmergencyStopSound.empty() == false && MotorEmergencyStopSound != "")
		{
			if (sbs->Verbose)
				Report("playing motor emergency stop sound");

			MotorSoundFile = MotorEmergencyStopSound;
			motorsound_play = true;
		}
	}

	//car sound
	if (carsound_play == false)
	{
		if (Direction == -1 && CarUpStopSound.empty() == false && CarUpStopSound != "")
		{
			if (sbs->Verbose)
				Report("playing car up stop sound");

			CarSoundFile = CarUpStopSound;
			carsound_play = true;
		}
		if (Direction == 1 && CarDownStopSound.empty() == false && CarDownStopSound != "")
		{
			if (sbs->Verbose)
				Report("playing car down stop sound");

			CarSoundFile = CarDownStopSound;
			carsound_play = true;
		}
	}

	//motor sound
	if (motorsound_play == false)
	{
		if (Direction == -1 && MotorUpStopSound.empty() == false && MotorUpStopSound != "")
		{
			if (sbs->Verbose)
				Report("playing motor up stop sound");

			MotorSoundFile = MotorUpStopSound;
			motorsound_play = true;
		}
		if (Direction == 1 && MotorDownStopSound.empty() == false && MotorDownStopSound != "")
		{
			if (sbs->Verbose)
				Report("playing motor down stop sound");

			MotorSoundFile = MotorDownStopSound;
			motorsound_play = true;
		}
	}

	carsound->Stop();
	motorsound->Stop();

	if (carsound_play == true)
	{
		carsound->Load(CarSoundFile);
		carsound->SetLoopState(false);

		//set play position to current percent of the total speed
		if (AutoAdjustSound == true)
			carsound->SetPlayPosition(1 - (ElevatorRate / ElevatorSpeed));
		else
			carsound->Reset();

		carsound->Play(false);

	}
	if (motorsound_play == true)
	{
		motorsound->Load(MotorSoundFile);
		motorsound->SetLoopState(false);

		//set play position to current percent of the total speed
		if (AutoAdjustSound == true)
			motorsound->SetPlayPosition(1 - (ElevatorRate / ElevatorSpeed));
		else
			motorsound->Reset();

		motorsound->Play(false);
	}
}

void Elevator::PlayMovingSounds()
{
	//play elevator movement sounds

	//car sound
	if (carsound->IsPlaying() == false)
	{
		if (Direction == 1 && CarUpMoveSound.empty() == false && CarUpMoveSound != "")
		{
			if (sbs->Verbose)
				Report("playing car up movement sound");

			carsound->Load(CarUpMoveSound);
			carsound->SetLoopState(true);
			carsound->Play();
		}
		else if (Direction == -1 && CarDownMoveSound.empty() == false && CarDownMoveSound != "")
		{
			if (sbs->Verbose)
				Report("playing car down movement sound");

			carsound->Load(CarDownMoveSound);
			carsound->SetLoopState(true);
			carsound->Play();
		}
	}

	//motor sound
	if (motorsound->IsPlaying() == false)
	{
		if (Direction == 1 && MotorUpRunSound.empty() == false && MotorUpRunSound != "")
		{
			if (sbs->Verbose)
				Report("playing motor up run sound");

			motorsound->Load(MotorUpRunSound);
			motorsound->SetLoopState(true);
			motorsound->Play();
		}
		else if (Direction == -1 && MotorDownRunSound.empty() == false && MotorDownRunSound != "")
		{
			if (sbs->Verbose)
				Report("playing motor down run sound");

			motorsound->Load(MotorDownRunSound);
			motorsound->SetLoopState(true);
			motorsound->Play();
		}
	}
}

bool Elevator::GetSensorStatus(int number)
{
	//checks doors and returns true if any (or the specified door) have their door sensor active

	int start = number, end = number;
	if (number == 0)
	{
		start = 1;
		end = NumDoors;
	}
	for (int i = start; i <= end; i++)
	{
		ElevatorDoor *door = GetDoor(i);
		if (door)
		{
			if (door->GetSensorStatus() == true)
				return true;
		}
		else
			ReportError("Invalid door " + ToString(i));
	}

	return false;
}

void Elevator::EnableSensor(bool value, int number)
{
	//enables door sensor on all doors or the specified door

	int start = number, end = number;
	if (number == 0)
	{
		start = 1;
		end = NumDoors;
	}
	for (int i = start; i <= end; i++)
	{
		ElevatorDoor *door = GetDoor(i);
		if (door)
			door->EnableSensor(value);
		else
			ReportError("Invalid door " + ToString(i));
	}
}

int Elevator::GetServicedFloorCount()
{
	//return number of serviced floors
	return (int)ServicedFloors.size();
}

int Elevator::GetServicedFloor(int index)
{
	//get a specific serviced floor
	if (index >= 0 && index < (int)ServicedFloors.size())
		return ServicedFloors[index];
	return 0;
}

void Elevator::ResetShaftDoors(int floor)
{
	//reset shaft doors

	//this might not be needed, due to addition of full-shaft enable check to
	//floor object's EnableGroup function, needs testing

	for (int i = 1; i <= sbs->Shafts(); i++)
	{
		Shaft *shaft = sbs->GetShaft(i);
		if (shaft)
		{
			if (shaft->IsEnabled == false)
			{
				shaft->EnableRange(floor, sbs->ShaftDisplayRange, false, true);
				shaft->EnableRange(floor, sbs->ShaftDisplayRange, true, true);
			}
		}
	}
}

bool Elevator::GetHoldStatus(int number)
{
	//checks doors and returns true if any (or the specified door) have door hold enabled

	int start = number, end = number;
	if (number == 0)
	{
		start = 1;
		end = NumDoors;
	}
	for (int i = start; i <= end; i++)
	{
		ElevatorDoor *door = GetDoor(i);
		if (door)
		{
			if (door->GetHoldStatus() == true)
				return true;
		}
		else
			ReportError("Invalid door " + ToString(i));
	}

	return false;
}

bool Elevator::ShaftDoorsExist(int number, int floor)
{
	//return true if shaft doors exist on the specified floor

	int start = number, end = number;
	if (number == 0)
	{
		start = 1;
		end = NumDoors;
	}
	for (int i = start; i <= end; i++)
	{
		ElevatorDoor *door = GetDoor(i);
		if (door)
		{
			if (door->ShaftDoorsExist(floor) == true)
				return true;
		}
	}
	return false;
}

void Elevator::ResetDoorState(int number)
{
	//reset elevator internal door state, in case of door malfunction

	int start = number, end = number;
	if (number == 0)
	{
		start = 1;
		end = NumDoors;
	}
	for (int i = start; i <= end; i++)
	{
		ElevatorDoor *door = GetDoor(i);
		if (door)
			door->ResetState();
		else
			ReportError("Invalid door " + ToString(i));
	}
}

bool Elevator::PeakWaiting()
{
	//returns true if elevator is waiting in UpPeak or DownPeak mode

	if (GetHoldStatus() == false)
		return false;

	if ((GetFloor() == GetTopFloor() && DownPeak == true && IsMoving == false) ||
		(GetFloor() == GetBottomFloor() && UpPeak == true && IsMoving == false))
		return true;
	return false;
}

bool Elevator::OnRecallFloor()
{
	//returns false if not on a recall floor,
	//returns true if on the standard recall floor and normal recall is available
	//returns true if on the alternate recall floor and normal recall is unavailable

	if (RecallUnavailable == false)
	{
		if (GetFloor() == RecallFloor)
			return true;
	}
	else
	{
		if (GetFloor() == RecallFloorAlternate)
			return true;
	}

	return 0;
}

}
