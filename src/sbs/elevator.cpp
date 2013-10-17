/* $Id$ */

/*
	Scalable Building Simulator - Elevator Subsystem Class
	The Skyscraper Project - Version 1.9 Alpha
	Copyright (C)2004-2013 Ryan Thoryk
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

extern SBS *sbs; //external pointer to the SBS engine

Elevator::Elevator(int number)
{
	//set up SBS object
	object = new Object();
	object->SetValues(this, sbs->object, "Elevator", "", false);

	std::string buffer;

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
	UseMessageSounds = false;
	MotorPosition = 0;
	ActiveCallFloor = 0;
	ActiveCallDirection = 0;
	lastdoor_result = 0;
	lastdoor_number = 0;
	QueueResets = sbs->GetConfigBool("Skyscraper.SBS.Elevator.QueueResets", false);
	FirstRun = true;
	CameraOffset = 0;
	ParkingFloor = 0;
	ParkingDelay = 0;
	Leveling = false;
	LevelingSpeed = sbs->GetConfigFloat("Skyscraper.SBS.Elevator.LevelingSpeed", 0.2);
	LevelingOffset = sbs->GetConfigFloat("Skyscraper.SBS.Elevator.LevelingOffset", 0.5);
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
	mainsound = 0;
	idlesound = 0;
	motorsound = 0;
	alarm = 0;
	floorbeep = 0;
	floorsound = 0;
	OriginFloor = 0;
	Fan = true;
	NotifyEarly = sbs->GetConfigInt("Skyscraper.SBS.Elevator.NotifyEarly", 0);
	Running = sbs->GetConfigBool("Skyscraper.SBS.Elevator.Run", true);
	Notified = false;
	Parking = false;
	MusicPosition = 0;
	Music = sbs->GetConfigString("Skyscraper.SBS.Elevator.Music", "");
	MusicOn = sbs->GetConfigBool("Skyscraper.SBS.Elevator.MusicOn", true);;
	MusicOnMove = sbs->GetConfigBool("Skyscraper.SBS.Elevator.MusicOnMove", false);;
	DepartureDelay = sbs->GetConfigFloat("Skyscraper.SBS.Elevator.DepartureDelay", 0.0);
	ArrivalDelay = sbs->GetConfigFloat("Skyscraper.SBS.Elevator.ArrivalDelay", 0.0);
	WaitForTimer = false;
	SoundsQueued = false;
	HeightSet = false;
	messagesnd = 0;
	musicsound = 0;
	elevposition = 0;
	lastposition = 0;
	ManualUp = false;
	ManualDown = false;
	InspectionSpeed = sbs->GetConfigFloat("Skyscraper.SBS.Elevator.InspectionSpeed", 0.6);
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

	//create timers
	parking_timer = new Timer(this, 0);
	random_timer = new Timer(this, 1);
	arrival_delay = new Timer(this, 2);
	departure_delay = new Timer(this,3);

	//create object meshes
	buffer = ToString(Number);
	buffer.insert(0, "Elevator ");
	TrimString(buffer);
	object->SetName(buffer.c_str());
	ElevatorMesh = new MeshObject(object, buffer.c_str());

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
			ControlArray[i]->object->parent_deleting = true;
			delete ControlArray[i];
		}
		ControlArray[i] = 0;
	}

	//delete triggers
	for (int i = 0; i < (int)TriggerArray.size(); i++)
	{
		if (TriggerArray[i])
		{
			TriggerArray[i]->object->parent_deleting = true;
			delete TriggerArray[i];
		}
		TriggerArray[i] = 0;
	}

	//delete models
	for (int i = 0; i < (int)ModelArray.size(); i++)
	{
		if (ModelArray[i])
		{
			ModelArray[i]->object->parent_deleting = true;
			delete ModelArray[i];
		}
		ModelArray[i] = 0;
	}

	//delete lights
	for (int i = 0; i < (int)lights.size(); i++)
	{
		if (lights[i])
		{
			lights[i]->object->parent_deleting = true;
			delete lights[i];
		}
		lights[i] = 0;
	}

	//delete timers
	if (sbs->Verbose)
		Report("deleting timers");

	if (parking_timer)
	{
		parking_timer->Stop();
		delete parking_timer;
	}
	parking_timer = 0;

	if (random_timer)
	{
		random_timer->Stop();
		delete random_timer;
	}
	random_timer = 0;

	if (arrival_delay)
	{
		arrival_delay->Stop();
		delete arrival_delay;
	}
	arrival_delay = 0;

	if (departure_delay)
	{
		departure_delay->Stop();
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
			DirIndicatorArray[i]->object->parent_deleting = true;
			delete DirIndicatorArray[i];
		}
	}
	DirIndicatorArray.clear();

	//delete doors
	if (sbs->Verbose)
		Report("deleting doors");

	if (DoorArray.size() > 0)
	{
		for (int i = 0; i < (int)DoorArray.size(); i++)
		{
			if (DoorArray[i])
			{
				DoorArray[i]->object->parent_deleting = true;
				delete DoorArray[i];
			}
		}
	}

	//delete floor indicators
	if (sbs->Verbose)
		Report("deleting floor indicators");

	for (int i = 0; i < (int)FloorIndicatorArray.size(); i++)
	{
		if (FloorIndicatorArray[i])
		{
			FloorIndicatorArray[i]->object->parent_deleting = true;
			delete FloorIndicatorArray[i];
		}
	}
	FloorIndicatorArray.clear();

	//delete panels
	if (sbs->Verbose)
		Report("deleting button panels");

	for (int i = 0; i < (int)PanelArray.size(); i++)
	{
		if (PanelArray[i])
		{
			PanelArray[i]->object->parent_deleting = true;
			delete PanelArray[i];
		}
		PanelArray[i] = 0;
	}
	PanelArray.clear();

	//delete doors
	if (sbs->Verbose)
		Report("deleting standard doors");

	for (int i = 0; i < (int)StdDoorArray.size(); i++)
	{
		if (StdDoorArray[i])
		{
			StdDoorArray[i]->object->parent_deleting = true;
			delete StdDoorArray[i];
		}
		StdDoorArray[i] = 0;
	}
	StdDoorArray.clear();

	//Destructor
	if (sbs->Verbose)
		Report("deleting objects");
	if (mainsound)
	{
		mainsound->object->parent_deleting = true;
		delete mainsound;
	}
	mainsound = 0;
	if (alarm)
	{
		alarm->object->parent_deleting = true;
		delete alarm;
	}
	alarm = 0;
	if (floorbeep)
	{
		floorbeep->object->parent_deleting = true;
		delete floorbeep;
	}
	floorbeep = 0;
	if (floorsound)
	{
		floorsound->object->parent_deleting = true;
		delete floorsound;
	}
	floorsound = 0;
	if (motorsound)
	{
		motorsound->object->parent_deleting = true;
		delete motorsound;
	}
	motorsound = 0;
	if (idlesound)
	{
		idlesound->object->parent_deleting = true;
		delete idlesound;
	}
	idlesound = 0;
	if (messagesnd)
	{
		messagesnd->object->parent_deleting = true;
		delete messagesnd;
	}
	messagesnd = 0;
	if (musicsound)
	{
		musicsound->object->parent_deleting = true;
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
			sounds[i]->object->parent_deleting = true;
			delete sounds[i];
		}
		sounds[i] = 0;
	}
	sounds.clear();

	//delete wall objects
	for (int i = 0; i < (int)elevator_walls.size(); i++)
	{
		if (elevator_walls[i])
		{
			elevator_walls[i]->parent_deleting = true;
			delete elevator_walls[i];
		}
		elevator_walls[i] = 0;
	}

	if (ElevatorMesh)
		delete ElevatorMesh;
	ElevatorMesh = 0;

	//unregister from parent
	if (sbs->FastDelete == false && object->parent_deleting == false)
		sbs->RemoveElevator(this);

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
		std::string num = ToString(AssignedShaft);
		ReportError(std::string("Shaft " + num + " doesn't exist").c_str());
		return 0;
	}
	if (floor < sbs->GetShaft(AssignedShaft)->startfloor || floor > sbs->GetShaft(AssignedShaft)->endfloor)
	{
		std::string num = ToString(floor);
		ReportError(std::string("Invalid starting floor " + num).c_str());
		return 0;
	}

	//add elevator's starting floor to serviced floor list - this also ensures that the list is populated to prevent errors
	if (IsServicedFloor(floor) == false)
		AddServicedFloor(floor);

	//set data
	if (!sbs->GetFloor(floor))
	{
		std::string num = ToString(floor);
		ReportError(std::string("Floor " + num + " doesn't exist").c_str());
		return 0;
	}

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
		DoorArray.resize(NumDoors);
		for (int i = 0; i < NumDoors; i++)
			DoorArray[i] = new ElevatorDoor(i, this);
	}

	//move objects to positions
	if (sbs->Verbose)
		Report("moving elevator to origin position");
	ElevatorMesh->Move(Origin, false, false, false);
	elevposition = GetPosition();

	//create sound objects
	if (sbs->Verbose)
		Report("creating sound objects");
	mainsound = new Sound(this->object, "Main", true);
	mainsound->SetPosition(Origin);
	idlesound = new Sound(this->object, "Idle", true);
	idlesound->SetPosition(Origin);
	idlesound->Load(CarIdleSound.c_str());
	motorsound = new Sound(this->object, "Motor", true);
	motorsound->SetPosition(Origin);
	//move motor to top of shaft if location not specified, or to location
	if (MotorPosition != Ogre::Vector3(0, 0, 0))
		motorsound->SetPosition(Ogre::Vector3(MotorPosition.x + Origin.x, MotorPosition.y, MotorPosition.z + Origin.z));
	else
	{
		Shaft* shaft = sbs->GetShaft(AssignedShaft);
		if (shaft)
		{
			Floor *floor = sbs->GetFloor(shaft->endfloor);
			if (floor)
				motorsound->SetPositionY(floor->GetBase());
		}
	}
	MotorPosition = Ogre::Vector3(motorsound->GetPosition().x - Origin.x, motorsound->GetPosition().y, motorsound->GetPosition().z - Origin.z);
	alarm = new Sound(this->object, "Alarm", true);
	alarm->SetPosition(Origin);
	floorbeep = new Sound(this->object, "Floor Beep", true);
	floorbeep->SetPosition(Origin);
	floorsound = new Sound(this->object, "Floor Sound", true);
	floorsound->SetPosition(Origin);
	messagesnd = new Sound(this->object, "Message Sound", true);
	messagesnd->SetPosition(Origin);
	musicsound = new Sound(this->object, "Music Sound", true);
	musicsound->SetPosition(Origin + MusicPosition);
	musicsound->Load(Music.c_str());

	//set elevator's floor
	ElevatorFloor = floor;

	//create test light
	//AddLight("light", 0, Ogre::Vector3(0, 6, 0), Ogre::Vector3(0, 0, 0), 1, 1, 1, 1, 1, 1, 0, 0, 0, 1000, 1, 1, 1);

	Created = true;

	Report("created at " + sbs->TruncateNumber(x, 4) + ", " + sbs->TruncateNumber(x, 4) + ", " + ToString2(floor));
	return object;
}

bool Elevator::AddRoute(int floor, int direction, bool change_light)
{
	//Add call route to elevator routing table, in sorted order
	//directions are either 1 for up, or -1 for down

	//SBS_PROFILE("Elevator::AddRoute");

	if (Running == false)
	{
		Report("Elevator not running");
		return false;
	}

	Floor *floorobj = sbs->GetFloor(floor);

	if (!floorobj)
	{
		Report("Invalid floor " + ToString2(floor));
		return false;
	}

	//if doors are open or moving in independent service mode, quit
	if (IndependentService == true && (AreDoorsOpen() == false || CheckOpenDoor() == true))
	{
		Report("floor button must be pressed before closing doors while in independent service");
		return false;
	}

	//do not add routes if in inspection service or fire phase 1 modes
	if (InspectionService == true)
	{
		Report("cannot add route while in inspection service mode");
		return false;
	}
	if (FireServicePhase2 == 2)
	{
		Report("cannot add route while hold is enabled");
		return false;
	}

	//discard route if direction opposite queue search direction
	if (LimitQueue == true && direction != QueuePositionDirection && QueuePositionDirection != 0)
	{
		//only allow if any queue entries exist
		if ((QueuePositionDirection == 1 && UpQueue.size() > 0) || (QueuePositionDirection == -1 && DownQueue.size() > 0))
		{
			Report("cannot add route in opposite direction of queue search");
			return false;
		}
	}

	//add route in related direction queue
	if (direction == 1)
	{
		int loc = -1;
		for (int i = 0; i < (int)UpQueue.size(); i++)
		{
			if (UpQueue[i] == floor)
				loc = i;
		}

		if (loc != -1)
		{
			//exit if entry already exits
			Report("route to floor " + ToString2(floor) + " (" + floorobj->ID + ") already exists");
			return false;
		}

		//add floor to up queue
		UpQueue.push_back(floor);
		std::sort(UpQueue.begin(), UpQueue.end());
		QueuePending = true;

		LastQueueFloor[0] = floor;
		LastQueueFloor[1] = 1;
		Report("adding route to floor " + ToString2(floor) + " (" + floorobj->ID + ") direction up");
	}
	else
	{
		int loc = -1;
		for (int i = 0; i < (int)DownQueue.size(); i++)
		{
			if (DownQueue[i] == floor)
				loc = i;
		}

		if (loc != -1)
		{
			//exit if entry already exits
			Report("route to floor " + ToString2(floor) + " (" + floorobj->ID + ") already exists");
			return false;
		}
		
		//add floor to down queue
		DownQueue.push_back(floor);
		std::sort(DownQueue.begin(), DownQueue.end());
		QueuePending = true;
		
		LastQueueFloor[0] = floor;
		LastQueueFloor[1] = -1;
		Report("adding route to floor " + ToString2(floor) + " (" + floorobj->ID + ") direction down");
	}

	//turn on button lights
	if (change_light == true)
	{
		if (sbs->Verbose)
			Report("AddRoute: turning on button lights for floor " + ToString2(floor));
		ChangeLight(floor, true);
	}

	//go to ACP floor if ACP mode is enabled
	if (ACP == true && floor != ACPFloor)
	{
		//only add ACP route if original route will pass ACP floor
		if ((GetFloor() < ACPFloor && floor > ACPFloor) || (GetFloor() > ACPFloor && floor < ACPFloor))
		{
			if (sbs->Verbose)
				Report("Adding ACP route");
			Go(ACPFloor, false);
		}
	}

	return true;
}

bool Elevator::DeleteRoute(int floor, int direction)
{
	//Delete call route from elevator routing table
	//directions are either 1 for up, or -1 for down

	//SBS_PROFILE("Elevator::DeleteRoute");

	if (Running == false)
	{
		Report("Elevator not running");
		return false;
	}

	Floor *floorobj = sbs->GetFloor(floor);

	if (!floorobj)
	{
		Report("Invalid floor " + ToString2(floor));
		return false;
	}

	if (direction == 1)
	{
		//delete floor entry from up queue
		for (int i = 0; i < (int)UpQueue.size(); i++)
		{
			if (UpQueue[i] == floor)
				UpQueue.erase(UpQueue.begin() + i);
		}
		Report("deleting route to floor " + ToString2(floor) + " (" + floorobj->ID + ") direction up");
		if (UpQueue.size() == 0)
			UpQueueEmpty = true;
	}
	else
	{
		//delete floor entry from down queue
		for (int i = 0; i < (int)DownQueue.size(); i++)
		{
			if (DownQueue[i] == floor)
				DownQueue.erase(DownQueue.begin() + i);
		}
		Report("deleting route to floor " + ToString2(floor) + " (" + floorobj->ID + ") direction down");
		if (DownQueue.size() == 0)
			DownQueueEmpty = true;
	}

	//turn off button lights
	if (sbs->Verbose)
		Report("DeleteRoute: turning off button lights for floor " + ToString2(floor));
	ChangeLight(floor, false);
	return true;
}

bool Elevator::CallCancel()
{
	//cancels the last added route
	//LastQueueFloor holds the floor and direction of the last route; array element 0 is the floor and 1 is the direction

	if (Running == false)
	{
		Report("Elevator not running");
		return false;
	}

	if (LastQueueFloor[1] == 0)
	{
		if (sbs->Verbose)
			Report("CallCancel: route not valid");
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
			alarm->Load(AlarmSound.c_str());
			alarm->Loop(true);
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
			alarm->Load(AlarmSoundStop.c_str());
			alarm->Loop(false);
			alarm->Play();
		}
		Report("alarm off");
	}
}

void Elevator::Stop(bool emergency)
{
	//Tells elevator to stop moving, no matter where it is

	//exit if in inspection mode
	if (InspectionService == true && emergency == true)
	{
		if (sbs->Verbose)
			Report("cannot stop while in inspection service");
		return;
	}

	//exit if in fire service phase 1 recall
	if (FireServicePhase1 == 1 && FireServicePhase2 == 0)
	{
		Report("cannot stop while in fire service 1 recall mode");
		return;
	}

	if (IsMoving == false)
	{
		if (sbs->Verbose)
			Report("Stop: not moving");
		return;
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
	if (AreDoorsOpen() == false && CheckOpenDoor() == false)
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
			if (UpQueue[i] >= ElevatorFloor)
			{
				if (MoveElevator == false)
				{
					if (sbs->Verbose)
						Report("ProcessCallQueue up: standard dispatch, floor " + ToString2(UpQueue[i]));
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
					QueuePending = false;
				}
				else if (Leveling == false && ActiveDirection == 1)
				{
					//if elevator is moving and not leveling, change destination floor if not beyond decel marker of that floor
					if (GotoFloor != UpQueue[i])
					{
						float tmpdestination = GetDestinationAltitude(UpQueue[i]);
						if (BeyondDecelMarker(1, tmpdestination) == false && sbs->GetFloor(GotoFloor))
						{
							ActiveCallFloor = UpQueue[i];
							GotoFloor = UpQueue[i];
							Destination = tmpdestination;
							Report("changing destination floor to " + ToString2(GotoFloor) + " (" + sbs->GetFloor(GotoFloor)->ID + ")");
						}
						else if (sbs->Verbose)
							Report("ProcessCallQueue up: cannot change destination floor to " + ToString2(UpQueue[i]));
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
						Report("ProcessCallQueue up: dispatching idle lower elevator, floor " + ToString2(UpQueue[i]));
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
					Report("ProcessCallQueue up: skipping floor entry " + ToString2(UpQueue[i]));
			}
		}
	}
	else if (QueuePositionDirection == -1)
	{
		//search through down queue (search order is reversed since calls need to be processed in descending order)
		for (int i = (int)DownQueue.size() - 1; i >= 0; i--)
		{
			//if the queued floor number is a lower floor, dispatch the elevator to that floor
			if (DownQueue[i] <= ElevatorFloor)
			{
				if (MoveElevator == false)
				{
					if (sbs->Verbose)
						Report("ProcessCallQueue down: standard dispatch, floor " + ToString2(DownQueue[i]));
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
					QueuePending = false;
				}
				else if (Leveling == false && ActiveDirection == -1)
				{
					//if elevator is moving and not leveling, change destination floor if not beyond decel marker of that floor
					if (GotoFloor != DownQueue[i])
					{
						float tmpdestination = GetDestinationAltitude(DownQueue[i]);
						if (BeyondDecelMarker(-1, tmpdestination) == false && sbs->GetFloor(GotoFloor))
						{
							ActiveCallFloor = DownQueue[i];
							GotoFloor = DownQueue[i];
							Destination = tmpdestination;
							Report("changing destination floor to " + ToString2(GotoFloor) + " (" + sbs->GetFloor(GotoFloor)->ID + ")");
						}
						else if (sbs->Verbose)
							Report("ProcessCallQueue down: cannot change destination floor to " + ToString2(DownQueue[i]));
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
						Report("ProcessCallQueue down: dispatching idle higher elevator, floor " + ToString2(DownQueue[i]));
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
					Report("ProcessCallQueue down: skipping floor entry " + ToString2(DownQueue[i]));
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

void Elevator::MonitorLoop()
{
	//Monitors elevator and starts actions if needed

	SBS_PROFILE("Elevator::MonitorLoop");

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

	//play idle sound if in elevator, or if doors open
	if (CarIdleSound != "")
	{
		if (idlesound->IsPlaying() == false && Fan == true)
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
			if (Fan == false && idlesound->IsPlaying() == true)
			{
				if (sbs->Verbose)
					Report("stopping idle sound");
				idlesound->Stop();
			}
			else if ((sbs->InElevator == false || sbs->ElevatorNumber != Number) && AreDoorsOpen() == false && CheckOpenDoor() == false)
			{
				if (sbs->Verbose)
					Report("stopping idle sound");
				idlesound->Stop();
			}
		}
	}

	//play music sound if in elevator, or if doors open
	if (Music != "")
	{
		if (musicsound->IsPlaying() == false && MusicOn == true && ((MusicOnMove == true && IsMoving == true) || MusicOnMove == false))
		{
			if (InServiceMode() == false)
			{
				if ((sbs->InElevator == true && sbs->ElevatorNumber == Number) || AreDoorsOpen() == true || CheckOpenDoor() == true)
				{
					if (sbs->Verbose)
						Report("playing music");

					if (MusicPosition == Ogre::Vector3(0, 0, 0) && Height > 0)
						MusicPosition = Ogre::Vector3(0, Height, 0); //set default music position to elevator height

					musicsound->SetPosition(GetPosition() + MusicPosition);
					musicsound->Loop(true);
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
			else if ((sbs->InElevator == false || sbs->ElevatorNumber != Number) && AreDoorsOpen() == false && CheckOpenDoor() == false)
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
	if (doorhold_direction == 1)
		OpenDoors();
	if (doorhold_direction == -1)
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
			if ((UpQueue.size() != 0 || DownQueue.size() != 0) && (AreDoorsOpen() == true && CheckOpenDoor() == false))
			{
				if (door)
				{
					if (door->TimerIsRunning() == false)
						door->ResetDoorTimer();
				}
			}
		}
	}

	//enable auto-park timer if specified
	if (parking_timer->IsRunning() == false && ParkingDelay > 0 && Running == true && IsIdle() == true && InServiceMode() == false && AutoDoors == true)
		parking_timer->Start(ParkingDelay * 1000, true);

	//enable random call timer
	if (random_timer->IsRunning() == false && RandomActivity == true && Running == true && InServiceMode() == false && AutoDoors == true)
		random_timer->Start(RandomFrequency * 1000, false);

	//process triggers
	if (IsEnabled == true)
	{
		for (int i = 0; i < TriggerArray.size(); i++)
		{
			if (TriggerArray[i])
				TriggerArray[i]->Check();
		}
	}

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

	//wait until doors are fully closed if WaitForDoors is true
	if (WaitForDoors == true)
	{
		if (AreDoorsOpen() == true || CheckOpenDoor() == true)
			return;
		else
			WaitForDoors = false;
	}

	//exit if waiting for arrival or departure timers
	if (WaitForTimer == true)
		return;

	//exit if doors are not fully closed while interlocks enabled
	if (Interlocks == true && AreDoorsOpen() == true)
	{
		Report("Doors must be closed before moving when interlocks are enabled");
		MoveElevator = false;
		Direction = 0;
		return;
	}

	if (ElevatorIsRunning == false)
	{
		if (Running == false)
		{
			Report("Elevator not running");
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

		//exit if floor doesn't exist
		if (!sbs->GetFloor(GotoFloor))
		{
			sbs->Report("Destination floor does not exist");
			Direction = 0;
			MoveElevator = false;
			ElevatorIsRunning = false;
			DeleteActiveRoute();
			return;
		}

		//if destination floor is not a serviced floor, reset and exit
		if (IsServicedFloor(GotoFloor) == false && InspectionService == false && ManualMove == 0)
		{
			sbs->Report("Destination floor not in ServicedFloors list");
			Direction = 0;
			MoveElevator = false;
			ElevatorIsRunning = false;
			DeleteActiveRoute();
			return;
		}

		//If elevator is already on specified floor, open doors and exit
		if (ElevatorFloor == GotoFloor && InspectionService == false && IsLeveled() == true && ManualMove == 0)
		{
			sbs->Report("Elevator already on specified floor");
			Direction = 0;
			MoveElevator = false;
			ElevatorIsRunning = false;
			DeleteActiveRoute();
			goto finish; //skip main processing and run cleanup section
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
			DistanceToTravel = fabs(fabs(Destination) - fabs(ElevatorStart));
		else
		{
			//otherwise if inspection service is on, choose the altitude of the top/bottom floor
			if (Direction == 1)
			{
				Destination = GetDestinationAltitude(GetTopFloor());
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
				Destination = GetDestinationAltitude(GetBottomFloor());
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
		if (sbs->ElevatorSync == true && sbs->ElevatorNumber == Number && InspectionService == false && ManualMove == 0)
		{
			if (sbs->Verbose)
				Report("user in elevator - turning off objects");

			//turn off floor
			if (sbs->GetShaft(AssignedShaft)->ShowFloors == false)
			{
				sbs->GetFloor(sbs->camera->CurrentFloor)->Enabled(false);
				sbs->GetFloor(sbs->camera->CurrentFloor)->EnableGroup(false);
			}
			else
			{
				int loc = -1;
				for (int i = 0; i < (int)sbs->GetShaft(AssignedShaft)->ShowFloorsList.size(); i++)
				{
					if (sbs->GetShaft(AssignedShaft)->ShowFloorsList[i] == sbs->camera->CurrentFloor)
						loc = i;
				}

				if (loc == -1)
				{
					sbs->GetFloor(sbs->camera->CurrentFloor)->Enabled(false);
					sbs->GetFloor(sbs->camera->CurrentFloor)->EnableGroup(false);
				}
			}

			//Turn off sky, buildings, and landscape
			if (sbs->GetShaft(AssignedShaft)->ShowOutside == false)
			{
				sbs->EnableSkybox(false);
				sbs->EnableBuildings(false);
				sbs->EnableLandscape(false);
				sbs->EnableExternal(false);
			}
			else
			{
				int loc = -1;
				for (int i = 0; i < (int)sbs->GetShaft(AssignedShaft)->ShowOutsideList.size(); i++)
				{
					if (sbs->GetShaft(AssignedShaft)->ShowOutsideList[i] == sbs->camera->CurrentFloor)
						loc = i;
				}
				if (loc == -1)
				{
					sbs->EnableSkybox(false);
					sbs->EnableBuildings(false);
					sbs->EnableLandscape(false);
					sbs->EnableExternal(false);
				}
			}
		}

		//set interior directional indicators
		UpdateDirectionalIndicators();

		//set external active-direction indicators
		if (sbs->GetFloor(sbs->camera->CurrentFloor))
			sbs->GetFloor(sbs->camera->CurrentFloor)->UpdateDirectionalIndicators(Number);

		//notify about movement
		if (InspectionService == false && ManualMove == 0)
			Report("moving " + dir_string + " to floor " + ToString2(GotoFloor) + " (" + sbs->GetFloor(GotoFloor)->ID + ")");
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
			departure_delay->Start(DepartureDelay * 1000, false);
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

		//Play starting sounds
		if (sbs->Verbose)
			Report("playing starting sounds");
		mainsound->Stop();
		motorsound->Stop();
		if (Direction == 1)
		{
			mainsound->Load(CarUpStartSound.c_str());
			motorsound->Load(MotorUpStartSound.c_str());
		}
		else
		{
			mainsound->Load(CarDownStartSound.c_str());
			motorsound->Load(MotorDownStartSound.c_str());
		}
		mainsound->Loop(false);
		mainsound->Play();
		motorsound->Loop(false);
		motorsound->Play();
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

		if (Direction == -1)
			mainsound->Load(CarUpStopSound.c_str());
		else
			mainsound->Load(CarDownStopSound.c_str());
		mainsound->Loop(false);

		bool adjust = sbs->GetConfigBool("Skyscraper.SBS.Elevator.AutoAdjustSound", false);
		//set play position to current percent of the total speed
		if (adjust == true)
			mainsound->SetPlayPosition(1 - (ElevatorRate / ElevatorSpeed));
		else
			mainsound->Reset();
		mainsound->Play(false);

		if (Direction == -1)
			motorsound->Load(MotorUpStopSound.c_str());
		else
			motorsound->Load(MotorDownStopSound.c_str());
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
		if (Direction == 1 && CarUpMoveSound.empty() == false && CarUpMoveSound != "")
		{
			if (sbs->Verbose)
				Report("playing car up movement sound");

			mainsound->Load(CarUpMoveSound.c_str());
			mainsound->Loop(true);
			mainsound->Play();
		}
		else if (Direction == -1 && CarDownMoveSound.empty() == false && CarDownMoveSound != "")
		{
			if (sbs->Verbose)
				Report("playing car down movement sound");

			mainsound->Load(CarDownMoveSound.c_str());
			mainsound->Loop(true);
			mainsound->Play();
		}
	}

	if (motorsound->IsPlaying() == false && Brakes == false)
	{
		if (Direction == 1 && MotorUpRunSound.empty() == false && MotorUpRunSound != "")
		{
			//Motor sound
			if (sbs->Verbose)
				Report("playing motor up run sound");

			motorsound->Load(MotorUpRunSound.c_str());
			motorsound->Loop(true);
			motorsound->Play();
		}
		else if (Direction == -1 && MotorDownRunSound.empty() == false && MotorDownRunSound != "")
		{
			//Motor sound
			if (sbs->Verbose)
				Report("playing motor down run sound");

			motorsound->Load(MotorDownRunSound.c_str());
			motorsound->Loop(true);
			motorsound->Play();
		}
	}

	//move elevator objects and camera
	movement.y = ElevatorRate * sbs->delta;

	SetAltitude(GetPosition().y + movement.y);
	if (sbs->ElevatorSync == true && sbs->ElevatorNumber == Number)
		sbs->camera->SetPosition(Ogre::Vector3(sbs->camera->GetPosition().x, elevposition.y + CameraOffset, sbs->camera->GetPosition().z));

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
				//recalculate deceleration value based on distance from marker, and store result in tempdeceleration
				if ((Destination - LevelingOffset) >= elevposition.y)
					TempDeceleration = Deceleration * (StoppingDistance / ((Destination - LevelingOffset) - elevposition.y));
				else //if elevator is beyond leveling offset, ignore the offset
					TempDeceleration = Deceleration * (StoppingDistance / (Destination - elevposition.y));

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
				//recalculate deceleration value based on distance from marker, and store result in tempdeceleration
				if ((Destination + LevelingOffset) <= elevposition.y)
					TempDeceleration = Deceleration * (StoppingDistance / (elevposition.y - (Destination + LevelingOffset)));
				else //if elevator is beyond leveling offset, ignore the offset
					TempDeceleration = Deceleration * (StoppingDistance / (elevposition.y - Destination));

				//start deceleration
				Direction = 1;
				Brakes = true;
				if (InspectionService == false)
					ElevatorRate += ElevatorSpeed * ((TempDeceleration * JerkRate) * sbs->delta);
				else
					ElevatorRate += (ElevatorSpeed * InspectionSpeed) * ((TempDeceleration * JerkRate) * sbs->delta);
			}

			//stop sounds
			mainsound->Stop();
			motorsound->Stop();
			//play elevator stopping sounds
			if (sbs->Verbose)
				Report("playing stopping sounds");
			bool adjust = sbs->GetConfigBool("Skyscraper.SBS.Elevator.AutoAdjustSound", false);
			if (Direction == -1)
				mainsound->Load(CarUpStopSound.c_str());
			else
				mainsound->Load(CarDownStopSound.c_str());
			mainsound->Loop(false);

			//set play position to current percent of the total speed
			if (adjust == true)
				mainsound->SetPlayPosition(1 - (ElevatorRate / ElevatorSpeed));
			else
				mainsound->Reset();
			mainsound->Play(false);

			if (Direction == -1)
				motorsound->Load(MotorUpStopSound.c_str());
			else
				motorsound->Load(MotorDownStopSound.c_str());
			motorsound->Loop(false);
			if (adjust == true)
				motorsound->SetPlayPosition(1 - (ElevatorRate / ElevatorSpeed));
			else
				motorsound->Reset();
			motorsound->Play(false);

			if (NotifyEarly == 2 && Parking == false)
				NotifyArrival(GotoFloor);
		}
	}
	else if (Leveling == false && EmergencyStop == 0)
	{
		if (fabs(ElevatorRate) <= LevelingSpeed)
		{
			//turn on leveling if elevator's rate is less than or equal to the leveling speed value
			if (sbs->Verbose)
				Report("leveling enabled");
			Leveling = true;

			if (NotifyEarly == 1 && Parking == false)
				NotifyArrival(GotoFloor);
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
		if (LevelingOpen > 0 && FinishedMove == false && ArrivalDelay == 0)
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
			Report("on floor " + ToString2(GetFloor()));

		//play floor beep sound if floor is a serviced floor
		if (IsServicedFloor(GetFloor()) == true)
			PlayFloorBeep();

		//update floor indicators
		UpdateFloorIndicators();

		//update floor indicators on current camera floor
		if (sbs->GetFloor(sbs->camera->CurrentFloor))
			sbs->GetFloor(sbs->camera->CurrentFloor)->UpdateFloorIndicators(Number);
	}

	oldfloor = GetFloor();

	//exit if elevator's running
	if (fabs(ElevatorRate) != 0)
		return;

	//start arrival timer
	if (ArrivalDelay > 0)
	{
		if (WaitForTimer == false && arrival_delay->IsRunning() == false)
		{
			if (sbs->Verbose)
				Report("started arrival delay");
			WaitForTimer = true;
			arrival_delay->Start(ArrivalDelay * 1000, false);
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

		SetAltitude(Destination);

		if (sbs->ElevatorSync == true && sbs->ElevatorNumber == Number)
			sbs->camera->SetPosition(Ogre::Vector3(sbs->camera->GetPosition().x, GetPosition().y + CameraOffset, sbs->camera->GetPosition().z));
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
	mainsound->Stop();
	motorsound->Stop();
	tmpDecelJerk = 0;

	if (FinishedMove == false)
		FinishMove();
}

void Elevator::SetAltitude(float altitude)
{
	//set vertical position of elevator and objects

	ElevatorMesh->Move(Ogre::Vector3(elevposition.x, altitude, elevposition.z), false, false, false);
	elevposition.y = altitude;
	MoveDoors(0, Ogre::Vector3(0, elevposition.y, 0), true, false, true);
	MoveObjects(Ogre::Vector3(0, elevposition.y, 0), true, false, true);
	for (int i = 0; i < (int)FloorIndicatorArray.size(); i++)
	{
		if (FloorIndicatorArray[i])
			FloorIndicatorArray[i]->SetPosition(Ogre::Vector3(FloorIndicatorArray[i]->GetPosition().x, elevposition.y, FloorIndicatorArray[i]->GetPosition().z));
	}
	for (int i = 0; i < (int)PanelArray.size(); i++)
	{
		if (PanelArray[i])
			PanelArray[i]->SetToElevatorAltitude();
	}
	for (int i = 0; i < (int)DirIndicatorArray.size(); i++)
	{
		if (DirIndicatorArray[i])
			DirIndicatorArray[i]->SetPosition(Ogre::Vector3(DirIndicatorArray[i]->GetPosition().x, elevposition.y, DirIndicatorArray[i]->GetPosition().z));
	}
	for (int i = 0; i < (int)StdDoorArray.size(); i++)
	{
		if (StdDoorArray[i])
			StdDoorArray[i]->Move(Ogre::Vector3(0, elevposition.y, 0), true, false, true);
	}

	//move sounds
	mainsound->SetPosition(elevposition);
	idlesound->SetPosition(elevposition);
	MoveDoorSound(0, elevposition, false, false, false);
	alarm->SetPosition(elevposition);
	floorbeep->SetPosition(elevposition);
	floorsound->SetPosition(elevposition);
	messagesnd->SetPosition(elevposition);
	musicsound->SetPosition(elevposition + MusicPosition);
	for (int i = 0; i < (int)sounds.size(); i++)
	{
		if (sounds[i])
			sounds[i]->SetPositionY(elevposition.y + sounds[i]->PositionOffset.y);
	}
}

void Elevator::SetFloor(int floor)
{
	//set elevator's altitude to specified floor

	float altitude = 0;
	if (!sbs->GetFloor(floor))
		return;

	altitude = GetDestinationAltitude(floor);
	SetAltitude(altitude);
}

void Elevator::FinishMove()
{
	//post-move operations, such as chimes, opening doors, indicator updates, etc

	//manualstop is true if elevator is stopped within 18 inches of the nearest landing
	bool manualstop = EmergencyStop == 1 && fabs(GetDestinationAltitude(GetFloor()) - GetPosition().y) < 1.5;

	if (EmergencyStop == 0 || manualstop == true)
	{
		if (manualstop == true)
			GotoFloor = GetFloor();

		//the elevator is now stopped on a valid floor; set OnFloor to true
		OnFloor = true;
		Report("arrived at floor " + ToString2(GotoFloor) + " (" + sbs->GetFloor(GotoFloor)->ID + ")");

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
			for (int i = 1; i <= sbs->Shafts(); i++)
			{
				if (sbs->GetShaft(i))
				{
					sbs->GetShaft(i)->EnableRange(GotoFloor, sbs->ShaftDisplayRange, false, true);
					sbs->GetShaft(i)->EnableRange(GotoFloor, sbs->ShaftDisplayRange, true, true);
				}
			}
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
		if (FireServicePhase2 != 1 || GetFloor() == RecallFloor || GetFloor() == RecallFloorAlternate)
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

		if (sbs->ElevatorSync == true && sbs->ElevatorNumber == Number)
		{
			//reset shaft doors
			for (int i = 1; i <= sbs->Shafts(); i++)
			{
				if (sbs->GetShaft(i))
				{
					sbs->GetShaft(i)->EnableRange(GotoFloor, sbs->ShaftDisplayRange, false, true);
					sbs->GetShaft(i)->EnableRange(GotoFloor, sbs->ShaftDisplayRange, true, true);
				}
			}
		}
	}

	//update elevator's floor number
	ElevatorFloor = GotoFloor;

	EmergencyStop = 0;
	Parking = false;
	FinishedMove = true;
}

WallObject* Elevator::AddWall(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th)
{
	//Adds a wall with the specified dimensions

	WallObject *wall = ElevatorMesh->CreateWallObject(this->object, name);
	sbs->AddWallMain(wall, name, texture, thickness, x1, z1, x2, z2, height1, height2, voffset1, voffset2, tw, th, true);
	return wall;
}

WallObject* Elevator::AddFloor(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, bool reverse_axis, bool texture_direction, float tw, float th, bool legacy_behavior)
{
	//Adds a floor with the specified dimensions and vertical offset

	WallObject *wall = ElevatorMesh->CreateWallObject(this->object, name);
	sbs->AddFloorMain(wall, name, texture, thickness, x1, z1, x2, z2, voffset1, voffset2, reverse_axis, texture_direction, tw, th, true, legacy_behavior);
	return wall;
}

Object* Elevator::AddFloorIndicator(const char *texture_prefix, const char *direction, float CenterX, float CenterZ, float width, float height, float voffset)
{
	//Creates a floor indicator at the specified location

	int size = (int)FloorIndicatorArray.size();
	FloorIndicatorArray.resize(size + 1);
	FloorIndicatorArray[size] = new FloorIndicator(object, Number, texture_prefix, direction, CenterX, CenterZ, width, height, voffset);
	FloorIndicatorArray[size]->SetPosition(Origin);
	return FloorIndicatorArray[size]->object;
}

const Ogre::Vector3 Elevator::GetPosition()
{
	//returns the elevator's position
	return ElevatorMesh->GetPosition();
}

void Elevator::DumpQueues()
{
	//dump both (up and down) elevator queues

	sbs->Report("--- Elevator " + ToString2(Number) + " Queues ---\n");
	sbs->Report("Up:");
	for (int i = 0; i < (int)UpQueue.size(); i++)
		sbs->Report(ToString2(i) + " - " + ToString2(UpQueue[i]));
	sbs->Report("Down:");
	for (int i = 0; i < (int)DownQueue.size(); i++)
		sbs->Report(ToString2(i) + " - " + ToString2(DownQueue[i]));
}

void Elevator::Enabled(bool value)
{
	//shows/hides elevator

	if (IsEnabled == value)
		return;

	//SBS_PROFILE("Elevator::Enabled");
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

bool Elevator::IsInElevator(const Ogre::Vector3 &position)
{
	//determine if the given 3D position is inside the elevator

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

	if (position.y > GetPosition().y && position.y < GetPosition().y + (Height * 2))
	{
		if (ElevatorMesh->InBoundingBox(position, false) == true)
		{
			if (ElevatorMesh->HitBeam(position, Ogre::Vector3::NEGATIVE_UNIT_Y, Height) >= 0)
			{
				if (IsMoving == false)
				{
					//store camera offset if elevator is not moving
					CameraOffset = position.y - GetPosition().y;
				}
				else if (CameraOffset == 0)
				{
					//reposition camera with a moving elevator if the offset is 0
					if (position.y < GetPosition().y + Height)
						CameraOffset = sbs->camera->GetHeight(); //if below ceiling, set to camera height
					else
						CameraOffset = Height + sbs->camera->GetHeight(); //if above ceiling, set to elev height + camera height
				}
				inelevator = true;
			}
		}
		else
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

	sbs->Report("--- Elevator " + ToString2(Number) + "'s Serviced Floors ---\n");
	for (int i = 0; i < (int)ServicedFloors.size(); i++)
		sbs->Report(ToString2(i) + " - " + ToString2(ServicedFloors[i]));
}

bool Elevator::AddServicedFloor(int number)
{
	if (sbs->Verbose)
		Report("adding serviced floor " + ToString2(number));

	//check if floor is outside valid floor range
	if (sbs->IsValidFloor(number) == false)
	{
		std::string floor = ToString(number);
		return ReportError("AddServicedFloor: Invalid floor " + floor);
	}

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
		Report("removing serviced floor " + ToString2(number));
	if (IsServicedFloor(number) == true)
	{
		int index = GetFloorIndex(number);
		if (index > -1)
			ServicedFloors.erase(ServicedFloors.begin() + index);
	}
}

Object* Elevator::CreateButtonPanel(const char *texture, int rows, int columns, const char *direction, float CenterX, float CenterZ, float buttonwidth, float buttonheight, float spacingX, float spacingY, float voffset, float tw, float th)
{
	//create a new button panel object and store the pointer

	int index = (int)PanelArray.size();
	std::string number = ToString(index + 1);
	TrimString(number);
	PanelArray.resize(index + 1);

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

	SBS_PROFILE("Elevator::UpdateFloorIndicators");
	std::string value;
	if (UseFloorSkipText == true && IsServicedFloor(GetFloor()) == false)
		value = FloorSkipText;
	else
	{
		if (DisplayFloors.size() > 0)
		{
			for (int i = 0; i < DisplayFloors.size(); i++)
			{
				if (GetFloor() == DisplayFloors[i] && sbs->GetFloor(GetFloor()))
					value = sbs->GetFloor(GetFloor())->ID;
			}
		}
		else
		{
			if (sbs->GetFloor(GetFloor()))
				value = sbs->GetFloor(GetFloor())->ID;
		}
	}
	TrimString(value);

	if (value == "")
		value = "null";

	for (int i = 0; i < (int)FloorIndicatorArray.size(); i++)
	{
		if (FloorIndicatorArray[i])
			FloorIndicatorArray[i]->Update(value.c_str());
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
		Report("setting floor skip text to " + std::string(id));
	UseFloorSkipText = true;
	FloorSkipText = id;
	TrimString(FloorSkipText);
}

const char* Elevator::GetFloorSkipText()
{
	//get the floor skip text
	return FloorSkipText.c_str();
}

bool Elevator::IsServicedFloor(int floor)
{
	//returns true if floor is in serviced floor list, otherwise false

	//SBS_PROFILE("Elevator::IsServicedFloor");
	int index = GetFloorIndex(floor);
	if (index == -1)
	{
		if (sbs->Verbose)
			Report("Floor " + ToString2(floor) + " is not a serviced floor");
		return false;
	}
	else
	{
		if (sbs->Verbose)
			Report("Floor " + ToString2(floor) + " is a serviced floor");
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
	{
		Report("Elevator not running");
		return false;
	}

	//exit if in inspection mode
	if (InspectionService == true)
	{
		if (sbs->Verbose)
			Report("Go: in inspection mode");
		return false;
	}

	if (GoActive == false || hold == false)
	{
		if (hold == true)
		{
			GoActive = true;
			GoActiveFloor = floor;
		}
		Report("Go: proceeding to floor " + ToString2(floor));
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
		Report("Elevator not running");
		return;
	}

	//stop elevator if moving
	if (IsMoving == true)
		Stop();

	//reset queues
	ResetQueue(true, true);

	if (RecallFloor == GetFloor())
		return;

	if (RecallUnavailable == false)
	{
		Report("Proceeding to recall floor");
		if (RecallFloor > GetFloor())
			AddRoute(RecallFloor, 1, false);
		else
			AddRoute(RecallFloor, -1, false);
	}
	else
	{
		Report("Proceeding to alternate recall floor");
		if (RecallFloor > GetFloor())
			AddRoute(RecallFloorAlternate, 1, false);
		else
			AddRoute(RecallFloorAlternate, -1, false);
	}
}

void Elevator::EnableACP(bool value)
{
	//enable Anti-Crime Protection (ACP) mode

	if (Running == false)
	{
		Report("Elevator not running");
		return;
	}

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

	if (Running == false)
	{
		Report("Elevator not running");
		return;
	}

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
		if (IsMoving == false && GetFloor() == GetBottomFloor() && sbs->GetFloor(GetFloor()))
		{
			sbs->GetFloor(GetFloor())->SetDirectionalIndicators(Number, true, false);
			SetDirectionalIndicators(true, false);
			if (AutoDoors == true)
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

	if (Running == false)
	{
		Report("Elevator not running");
		return;
	}

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
		if (IsMoving == false && GetFloor() == GetTopFloor() && sbs->GetFloor(GetFloor()))
		{
			sbs->GetFloor(GetFloor())->SetDirectionalIndicators(Number, false, true);
			SetDirectionalIndicators(false, true);
			if (AutoDoors == true)
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

	if (Running == false)
	{
		Report("Elevator not running");
		return;
	}

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
		ResetQueue(true, true);
		EnableNudgeMode(false);
		HoldDoors(); //turn off door timers
		if (IsMoving == false)
			if (AutoDoors == true)
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
		EnableNudgeMode(false);
		ResetQueue(true, true);
		HoldDoors(); //turn off door timers
		if (IsMoving == true)
			Stop();
		Report("Inspection Service mode enabled");
		InspectionService = true;
	}
	else
	{
		ResetDoorTimer();
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
			for (int i = 1; i <= sbs->Shafts(); i++)
			{
				if (sbs->GetShaft(i))
				{
					sbs->GetShaft(i)->EnableRange(GetFloor(), sbs->ShaftDisplayRange, false, true);
					sbs->GetShaft(i)->EnableRange(GetFloor(), sbs->ShaftDisplayRange, true, true);
				}
			}
		}

		InspectionService = false;

		if (IsMoving == true)
			Stop();
		else
			ReturnToNearestFloor();
	}
}

void Elevator::EnableFireService1(int value)
{
	//enable Fire Service Phase 1 mode
	//valid values are 0 (off), 1 (on) or 2 (bypass)

	if (Running == false)
	{
		Report("Elevator not running");
		return;
	}

	//exit if no change
	if (FireServicePhase1 == value)
	{
		if (sbs->Verbose)
			Report("EnableFireService1: mode is the same");
		return;
	}

	//exit if in inspection modeno change
	if (InspectionService == true)
	{
		Report("EnableFireService1: cannot enable while in inspection mode");
		return;
	}

	if (value < 0 || value > 2)
	{
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
		if (value == 1)
		{
			Report("Fire Service Phase 1 mode set to On");
			
			//recall elevator if not in phase 2 hold
			if (FireServicePhase2 != 2)
			{
				//turn off all door timers
				HoldDoors();

				//enable nudge mode on all doors if any are open
				if (GetFloor() != RecallFloor && GetFloor() != RecallFloorAlternate)
					EnableNudgeMode(true);

				//goto recall floor
				GoToRecallFloor();
			}
		}
		else
		{
			if (FireServicePhase2 == 0)
				ResetDoorTimer(); //enable door timers
			Report("Fire Service Phase 1 mode set to Bypass");
		}
	}
	else
	{
		if (FireServicePhase2 == 0)
			ResetDoorTimer(); //enable door timers
		Report("Fire Service Phase 1 mode set to Off");
	}

	FireServicePhase1 = value;
}

void Elevator::EnableFireService2(int value)
{
	//enable Fire Service Phase 2 mode
	//valid values are 0 (off), 1 (on) or 2 (hold)

	if (Running == false)
	{
		Report("Elevator not running");
		return;
	}

	//exit if in inspection mode
	if (InspectionService == true)
	{
		Report("EnableFireService2: cannot enable while in inspection mode");
		return;
	}

	//require fire service phase 1 to be enabled first
	if (FireServicePhase1 != 1 && FireServicePhase2 == 0 && value > 0)
	{
		Report("EnableFireService2: not in fire service phase 1 mode");
		return;
	}

	//require doors to be open to change modes
	if (AreDoorsOpen() == false)
	{
		Report("EnableFireService2: doors must be open to change phase 2 modes");
		return;
	}

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
		EnableNudgeMode(false);
		ResetQueue(true, true);
		HoldDoors(); //disable all door timers
		if (value == 1)
			Report("Fire Service Phase 2 mode set to On");
		else
			Report("Fire Service Phase 2 mode set to Hold");
	}
	else
	{
		Report("Fire Service Phase 2 mode set to Off");
		if (FireServicePhase1 == 0)
			ResetDoorTimer(); //enable door timers
		else if (FireServicePhase1 == 1 && GetFloor() != RecallFloor && GetFloor() != RecallFloorAlternate)
		{
			//enable nudge mode on all doors if any are open
			EnableNudgeMode(true);
			//recall elevator
			GoToRecallFloor();
		}
	}
}

bool Elevator::SetRecallFloor(int floor)
{
	//set elevator's fire recall floor
	if (ServicedFloors.size() == 0)
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
		Report("setting recall floor to " + ToString2(floor));
	RecallFloor = floor;
	RecallSet = true;
	return true;
}

bool Elevator::SetAlternateRecallFloor(int floor)
{
	//set elevator's alternate fire recall floor
	if (ServicedFloors.size() == 0)
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
		Report("setting alternate recall floor to " + ToString2(floor));
	RecallFloorAlternate = floor;
	RecallAltSet = true;
	return true;
}

bool Elevator::SetACPFloor(int floor)
{
	//set elevator's ACP floor
	if (ServicedFloors.size() == 0)
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
		Report("setting ACP floor to " + ToString2(floor));
	ACPFloor = floor;
	ACPFloorSet = true;
	return true;
}

bool Elevator::MoveUp()
{
	//manual up movement for inspection service mode

	if (Running == false)
	{
		Report("Elevator not running");
		return false;
	}

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
	//manual down movement for inspection service mode

	if (Running == false)
	{
		Report("Elevator not running");
		return false;
	}

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

void Elevator::SetGoButton(bool value)
{
	//sets the value of the Go button (for Inspection Service mode)

	if (Running == false)
	{
		Report("Elevator not running");
		return;
	}

	if (InspectionService == false)
		return;

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
}

void Elevator::SetUpButton(bool value)
{
	//sets the value of the Up button (for Inspection Service mode)

	if (Running == false)
	{
		Report("Elevator not running");
		return;
	}

	if (InspectionService == false)
		return;

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
}

void Elevator::SetDownButton(bool value)
{
	//sets the value of the Go button (for Inspection Service mode)

	if (Running == false)
	{
		Report("Elevator not running");
		return;
	}

	if (InspectionService == false)
		return;

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

void Elevator::AddDirectionalIndicators(bool relative, bool active_direction, bool single, bool vertical, const char *BackTexture, const char *uptexture, const char *uptexture_lit, const char *downtexture, const char *downtexture_lit, float CenterX, float CenterZ, float voffset, const char *direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th)
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

Object* Elevator::AddDirectionalIndicator(bool active_direction, bool single, bool vertical, const char *BackTexture, const char *uptexture, const char *uptexture_lit, const char *downtexture, const char *downtexture_lit, float CenterX, float CenterZ, float voffset, const char *direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th)
{
	//create a directional indicator inside the elevator

	if (sbs->Verbose)
		Report("adding interior directional indicator");

	float x = Origin.x + CenterX;
	float z = Origin.z + CenterZ;

	int index = (int)DirIndicatorArray.size();
	DirIndicatorArray.resize(index + 1);
	DirIndicatorArray[index] = new DirectionalIndicator(object, Number, 0, active_direction, single, vertical, BackTexture, uptexture, uptexture_lit, downtexture, downtexture_lit, x, z, voffset, direction, BackWidth, BackHeight, ShowBack, tw, th);
	DirIndicatorArray[index]->SetPosition(Ogre::Vector3(DirIndicatorArray[index]->GetPosition().x, sbs->GetFloor(OriginFloor)->GetBase(), DirIndicatorArray[index]->GetPosition().z));
	return DirIndicatorArray[index]->object;
}

void Elevator::SetDirectionalIndicators(bool UpLight, bool DownLight)
{
	//set light status of interior directional indicators

	for (int i = 0; i < (int)DirIndicatorArray.size(); i++)
	{
		if (DirIndicatorArray[i])
		{
			if (DirIndicatorArray[i]->ActiveDirection == false)
			{
				DirIndicatorArray[i]->DownLight(DownLight);
				DirIndicatorArray[i]->UpLight(UpLight);
			}
		}
	}
}

void Elevator::UpdateDirectionalIndicators()
{
	//updates all interior active direction indicators

	for (int i = 0; i < (int)DirIndicatorArray.size(); i++)
	{
		if (DirIndicatorArray[i])
		{
			if (DirIndicatorArray[i]->ActiveDirection == true)
			{
				if (ActiveDirection == 1)
				{
					DirIndicatorArray[i]->DownLight(false);
					DirIndicatorArray[i]->UpLight(true);
				}
				if (ActiveDirection == 0)
				{
					DirIndicatorArray[i]->DownLight(false);
					DirIndicatorArray[i]->UpLight(false);
				}
				if (ActiveDirection == -1)
				{
					DirIndicatorArray[i]->DownLight(true);
					DirIndicatorArray[i]->UpLight(false);
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
	if (lastdoor_number == number)
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

void Elevator::OpenDoorsEmergency(int number, int whichdoors, int floor, bool hold)
{
	//Simulates manually prying doors open.
	//Slowly opens the elevator doors no matter where elevator is.
	//If lined up with shaft doors, then opens the shaft doors also

	//WhichDoors is the doors to move:
	//1 = both shaft and elevator doors
	//2 = only elevator doors
	//3 = only shaft doors

	OpenDoors(number, whichdoors, floor, true, hold);
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

void Elevator::OpenDoors(int number, int whichdoors, int floor, bool manual, bool hold)
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
	if (FireServicePhase2 == 1 && (GetFloor() != RecallFloor) && (GetFloor() != RecallFloorAlternate))
		hold = true;

	if (Interlocks == true)
	{
		if (IsMoving == true && OnFloor == false)
		{
			Report("Cannot open doors while moving if interlocks are enabled");
			return;
		}
		if (OnFloor == false || (whichdoors == 3 && floor != GetFloor()))
		{
			Report("Cannot open doors if not stopped within a landing zone if interlocks are enabled");
			return;
		}
	}

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
	if (doorhold_direction == 0)
	{
		for (int i = start; i <= end; i++)
		{
			if (GetDoor(i))
				GetDoor(i)->OpenDoors(whichdoors, floor, manual);
			else
				Report("Invalid door " + ToString2(i));
		}

		if (hold == true)
		{
			//set persistent values
			doorhold_direction = 1;
			doorhold_whichdoors = whichdoors;
			doorhold_floor = floor;
			doorhold_manual = manual;
		}
	}
	else if (doorhold_direction == 1 && sbs->camera->MouseDown == false)
	{
		bool closedstate = false;

		for (int i = start; i <= end; i++)
		{
			//check door states first
			if (GetDoor(i))
			{
				if (GetDoor(i)->AreDoorsOpen() == false)
					closedstate = true;
			}
			else
				Report("Invalid door " + ToString2(i));
		}

		for (int i = start; i <= end; i++)
		{
			//close doors using persistent values, if button is released before doors are fully closed
			if (GetDoor(i))
			{
				if (closedstate == true)
					GetDoor(i)->CloseDoors(doorhold_whichdoors, doorhold_floor, doorhold_manual);
				else
					GetDoor(i)->Hold();
			}
			else
				Report("Invalid door " + ToString2(i));
		}

		//reset persistent values
		doorhold_direction = 0;
		doorhold_whichdoors = 0;
		doorhold_floor = 0;
		doorhold_manual = false;
	}
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
	if (doorhold_direction == 0)
	{
		for (int i = start; i <= end; i++)
		{
			if (GetDoor(i))
				GetDoor(i)->CloseDoors(whichdoors, floor, manual);
			else
				Report("Invalid door " + ToString2(i));
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
					openstate = true;
			}
			else
				Report("Invalid door " + ToString2(i));
		}

		if (openstate == true)
		{
			for (int i = start; i <= end; i++)
			{
				//close doors using persistent values, if button is released before doors are fully open
				if (GetDoor(i))
					GetDoor(i)->OpenDoors(doorhold_whichdoors, doorhold_floor, doorhold_manual);
				else
					Report("Invalid door " + ToString2(i));
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
			Report("Invalid door " + ToString2(i));
	}
}

Object* Elevator::AddDoors(int number, const char *lefttexture, const char *righttexture, float thickness, float CenterX, float CenterZ, float width, float height, bool direction, float tw, float th)
{
	//adds elevator doors specified at a relative central position (off of elevator origin)
	//if direction is false, doors are on the left/right side; otherwise front/back

	if (GetDoor(number))
		return GetDoor(number)->AddDoors(lefttexture, righttexture, thickness, CenterX, CenterZ, width, height, direction, tw, th);
	else
		ReportError("Invalid door " + ToString2(number));
	return 0;
}

bool Elevator::AddShaftDoors(int number, const char *lefttexture, const char *righttexture, float thickness, float CenterX, float CenterZ, float voffset, float tw, float th)
{
	//adds shaft's elevator doors specified at a relative central position (off of elevator origin)
	//uses some parameters (width, height, direction) from AddDoors function

	if (GetDoor(number))
		return GetDoor(number)->AddShaftDoors(lefttexture, righttexture, thickness, CenterX, CenterZ, voffset, tw, th);
	else
		ReportError("Invalid door " + ToString2(number));
	return false;
}

Object* Elevator::AddShaftDoor(int floor, int number, const char *lefttexture, const char *righttexture, float tw, float th)
{
	//adds a single elevator shaft door on the specified floor, with position and thickness parameters first specified
	//by the SetShaftDoors command.

	int index = GetFloorIndex(floor);

	if (index != -1 && GetDoor(number))
		return GetDoor(number)->AddShaftDoor(floor, lefttexture, righttexture, tw, th);
	else
		return 0;
}

Object* Elevator::AddShaftDoor(int floor, int number, const char *lefttexture, const char *righttexture, float thickness, float CenterX, float CenterZ, float voffset, float tw, float th)
{
	//adds a single elevator shaft door on the specified floor, with position and thickness parameters first specified
	//by the SetShaftDoors command.

	int index = GetFloorIndex(floor);

	if (index != -1 && GetDoor(number))
		return GetDoor(number)->AddShaftDoor(floor, lefttexture, righttexture, thickness, CenterX, CenterZ, voffset, tw, th);
	else
		return 0;
}

void Elevator::ShaftDoorsEnabled(int number, int floor, bool value)
{
	//turns shaft elevator doors on/off

	SBS_PROFILE("Elevator::ShaftDoorsEnabled");
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
			Report("Invalid door " + ToString2(i));
	}
}

void Elevator::ShaftDoorsEnabledRange(int number, int floor, int range)
{
	//turn on a range of floors
	//if range is 3, show shaft door on current floor (floor), and 1 floor below and above (3 total floors)
	//if range is 1, show door on only the current floor (floor)

	SBS_PROFILE("Elevator::ShaftDoorsEnabledRange");
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
			Report("Invalid door " + ToString2(i));
	}
}

bool Elevator::AreDoorsOpen(int number)
{
	//returns the internal door state

	SBS_PROFILE("Elevator::AreDoorsOpen");
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
			Report("Invalid door " + ToString2(i));
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
		Report("Invalid door " + ToString2(number));
	return false;
}

void Elevator::Chime(int number, int floor, bool direction)
{
	//play chime sound on specified floor

	SBS_PROFILE("Elevator::Chime");
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
			Report("Invalid door " + ToString2(i));
	}
	if (direction == true)
		LastChimeDirection = 1;
	else
		LastChimeDirection = -1;
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
			Report("Invalid door " + ToString2(i));
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
			Report("Invalid door " + ToString2(i));
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

void Elevator::MoveDoors(int number, const Ogre::Vector3 position, bool relative_x, bool relative_y, bool relative_z)
{
	//move all doors

	SBS_PROFILE("Elevator::MoveDoors");
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

void Elevator::MoveDoorSound(int number, const Ogre::Vector3 position, bool relative_x, bool relative_y, bool relative_z)
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

bool Elevator::AddFloorSigns(int door_number, bool relative, const char *texture_prefix, const char *direction, float CenterX, float CenterZ, float width, float height, float voffset)
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

	//make sure specified door exists before continuing
	if (door_number != 0)
	{
		if (DoorExists(door_number) == false)
		{
			std::string doornum = ToString(door_number);
			ReportError("AddFloorSigns: door " + doornum + " does not exist");
			return false;
		}
	}

	bool autosize_x, autosize_y;
	sbs->GetAutoSize(autosize_x, autosize_y);
	sbs->SetAutoSize(false, false);

	for (int i = 0; i < (int)ServicedFloors.size(); i++)
	{
		bool door_result = false;
		if (door_number != 0)
			door_result = GetDoor(door_number)->ShaftDoorsExist(ServicedFloors[i]);

		if ((door_number == 0 || door_result == true) && sbs->GetFloor(ServicedFloors[i]))
		{
			std::string texture = texture_prefix + sbs->GetFloor(ServicedFloors[i])->ID;
			std::string tmpdirection = direction;
			SetCase(tmpdirection, false);

			if (tmpdirection == "front" || tmpdirection == "left")
				sbs->DrawWalls(true, false, false, false, false, false);
			else
				sbs->DrawWalls(false, true, false, false, false, false);

			if (tmpdirection == "front" || tmpdirection == "back")
				sbs->GetFloor(ServicedFloors[i])->AddWall("Floor Sign", texture.c_str(), 0, x - (width / 2), z, x + (width / 2), z, height, height, voffset, voffset, 1, 1, false);
			else
				sbs->GetFloor(ServicedFloors[i])->AddWall("Floor Sign", texture.c_str(), 0, x, z - (width / 2), x, z + (width / 2), height, height, voffset, voffset, 1, 1, false);
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
	if (MoveElevator == false && (AreDoorsOpen() == false || UpPeak == true || DownPeak == true) && CheckOpenDoor() == false && Running == true)
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
	//turn off specified button lights
	for (int i = 0; i < (int)PanelArray.size(); i++)
		PanelArray[i]->ChangeLight(floor, value);
}

void Elevator::SetBeepSound(const char *filename)
{
	//set sound used for floor beeps
	if (sbs->Verbose)
		Report("setting beep sound");
	BeepSound = filename;
	TrimString(BeepSound);
	UseFloorBeeps = true;
}

void Elevator::SetFloorSound(const char *prefix)
{
	//set prefix of floor sound
	if (sbs->Verbose)
		Report("setting floor sound");
	FloorSound = prefix;
	TrimString(FloorSound);
	UseFloorSounds = true;
}

void Elevator::SetMessageSound(bool direction, const char *filename)
{
	//if direction is true, set up message sound; otherwise set down message sound

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
	UseMessageSounds = true;
}

Object* Elevator::AddSound(const char *name, const char *filename, Ogre::Vector3 position, bool loop, float volume, int speed, float min_distance, float max_distance, float doppler_level, float cone_inside_angle, float cone_outside_angle, float cone_outside_volume, Ogre::Vector3 direction)
{
	//create a looping sound object
	Sound *sound = new Sound(object, name, false);
	sounds.push_back(sound);

	//set parameters and play sound
	sound->SetPosition(position);
	sound->SetDirection(direction);
	sound->SetVolume(volume);
	sound->SetSpeed(speed);
	sound->SetDistances(min_distance, max_distance);
	sound->SetDirection(direction);
	sound->SetDopplerLevel(doppler_level);
	sound->SetConeSettings(cone_inside_angle, cone_outside_angle, cone_outside_volume);
	sound->Load(filename);
	sound->Loop(loop);
	if (loop)
		sound->Play();

	return sound->object;
}

void Elevator::DeleteActiveRoute()
{
	if (Running == false)
	{
		Report("Elevator not running");
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

void Elevator::Report(std::string message)
{
	//general reporting function
	sbs->Report("Elevator " + ToString2(Number) + ": " + message);
}

bool Elevator::ReportError(std::string message)
{
	//general reporting function
	return sbs->ReportError("Elevator " + ToString2(Number) + ": " + message);
}

Object* Elevator::AddDoorComponent(int number, const char *name, const char *texture, const char *sidetexture, float thickness, const char *direction, float OpenSpeed, float CloseSpeed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th)
{
	//adds an elevator door component to the specified door at a relative central position (off of elevator origin)

	if (GetDoor(number))
		return GetDoor(number)->AddDoorComponent(name, texture, sidetexture, thickness, direction, OpenSpeed, CloseSpeed, x1, z1, x2, z2, height, voffset, tw, th, side_tw, side_th);
	else
		Report("Invalid door " + ToString2(number));
	return 0;
}

Object* Elevator::AddShaftDoorComponent(int number, int floor, const char *name, const char *texture, const char *sidetexture, float thickness, const char *direction, float OpenSpeed, float CloseSpeed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th)
{
	//adds a single elevator shaft door component on the specified floor

	int index = GetFloorIndex(floor);

	if (index != -1 && GetDoor(number))
		return GetDoor(number)->AddShaftDoorComponent(floor, name, texture, sidetexture, thickness, direction, OpenSpeed, CloseSpeed, x1, z1, x2, z2, height, voffset, tw, th, side_tw, side_th);
	else
		return 0;
}

void Elevator::AddShaftDoorsComponent(int number, const char *name, const char *texture, const char *sidetexture, float thickness, const char *direction, float OpenSpeed, float CloseSpeed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th)
{
	//adds shaft's elevator door components specified at a relative central position (off of elevator origin)

	if (GetDoor(number))
		GetDoor(number)->AddShaftDoorsComponent(name, texture, sidetexture, thickness, direction, OpenSpeed, CloseSpeed, x1, z1, x2, z2, height, voffset, tw, th, side_tw, side_th);
	else
		Report("Invalid door " + ToString2(number));
}

Object* Elevator::FinishDoors(int number)
{
	//finishes elevator door

	if (GetDoor(number))
		return GetDoor(number)->FinishDoors();
	else
		Report("Invalid door " + ToString2(number));
	return 0;
}

Object* Elevator::FinishShaftDoor(int number, int floor)
{
	//finishes a single shaft door

	int index = GetFloorIndex(floor);

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
		ReportError("Invalid door " + ToString2(number));
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
				elevator->Report("parking to floor " + ToString2(elevator->ParkingFloor));
				elevator->Parking = true;
			}

			if (elevator->ParkingFloor > floor)
				elevator->AddRoute(elevator->ParkingFloor, 1, false);
			else if (elevator->ParkingFloor < floor)
				elevator->AddRoute(elevator->ParkingFloor, -1, false);

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
		Report("Elevator not running");
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
	
	int index = -1;
	for (int i = 0; i < (int)UpQueue.size(); i++)
	{
		if (UpQueue[i] == floor)
			index = i;
	}
	if (index > -1)
		return true;

	index = -1;
	for (int i = 0; i < (int)DownQueue.size(); i++)
	{
		if (DownQueue[i] == floor)
			index = i;
	}
	if (index > -1)
		return true;
	return false;
}

void Elevator::HoldDoors(int number)
{
	//hold specified door, or all if "0" is given

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
			GetDoor(i)->Hold();
		else
			Report("Invalid door " + ToString2(i));
	}
}

Object* Elevator::AddDoor(const char *open_sound, const char *close_sound, bool open_state, const char *texture, float thickness, int direction, float speed, float CenterX, float CenterZ, float width, float height, float voffset, float tw, float th)
{
	//interface to the SBS AddDoor function

	if (direction > 8 || direction < 1)
	{
		ReportError("Door direction out of range");
		return 0;
	}

	float x1, z1, x2, z2;
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
	/*if (direction < 5)
		CutAll(Ogre::Vector3(x1 - 1, GetBase(true) + voffset, z1), Ogre::Vector3(x2 + 1, GetBase(true) + voffset + height, z2), true, false);
	else
		CutAll(Ogre::Vector3(x1, GetBase(true) + voffset, z1 - 1), Ogre::Vector3(x2, GetBase(true) + voffset + height, z2 + 1), true, false);
	*/

	StdDoorArray.resize(StdDoorArray.size() + 1);
	std::string elevnum = ToString(Number);
	std::string num = ToString((int)StdDoorArray.size() - 1);
	StdDoorArray[StdDoorArray.size() - 1] = new Door(this->object, std::string("Elevator " + elevnum + ":Door " + num).c_str(), open_sound, close_sound, open_state, texture, thickness, direction, speed, GetPosition().x + CenterX, GetPosition().z + CenterZ, width, height, voffset + GetPosition().y, tw, th);
	return StdDoorArray[StdDoorArray.size() - 1]->object;
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

	//play chime sound and change indicator
	if (GetArrivalDirection(floor) == true)
	{
		Chime(0, floor, true);
		if (sbs->GetFloor(floor))
			sbs->GetFloor(floor)->SetDirectionalIndicators(Number, true, false);
		SetDirectionalIndicators(true, false);
	}
	else
	{
		Chime(0, floor, false);
		if (sbs->GetFloor(floor))
			sbs->GetFloor(floor)->SetDirectionalIndicators(Number, false, true);
		SetDirectionalIndicators(false, true);
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
			newfloor = UpQueue[0];

		if (QueuePositionDirection == -1 && DownQueue.size() > 0 && DownQueueEmpty == false)
			newfloor = DownQueue[(int)DownQueue.size() - 1];

		if (QueuePositionDirection == 1 && DownQueue.size() > 0 && UpQueueEmpty == true)
			newfloor = DownQueue[(int)DownQueue.size() - 1];

		if (QueuePositionDirection == -1 && UpQueue.size() > 0 && DownQueueEmpty == true)
			newfloor = UpQueue[0];
	}
	else
	{
		if (QueuePositionDirection == 1 && UpQueue.size() > 1)
			newfloor = UpQueue[1];

		if (QueuePositionDirection == -1 && DownQueue.size() > 1)
			newfloor = DownQueue[(int)DownQueue.size() - 2];

		if (QueuePositionDirection == 1 && UpQueue.size() == 1)
		{
			if (DownQueue.size() > 0)
				newfloor = DownQueue[(int)DownQueue.size() - 1];
			else
				return true;
		}

		if (QueuePositionDirection == -1 && DownQueue.size() == 1)
		{
			if (UpQueue.size() > 0)
				newfloor = UpQueue[0];
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

	if (BeepSound == "" || UseFloorBeeps == false)
		return false;

	if (sbs->Verbose)
		Report("playing floor beep sound");

	std::string newsound = BeepSound;
	//change the asterisk into the current floor number
	ReplaceAll(newsound, "*", ToString(GetFloor()));
	TrimString(newsound);
	floorbeep->Stop();
	floorbeep->Load(newsound.c_str());
	floorbeep->Loop(false);
	floorbeep->Play();
	return true;
}

bool Elevator::PlayFloorSound()
{
	//play floor sound

	if (FloorSound == "" || UseFloorSounds == false)
		return false;

	if (sbs->Verbose)
		Report("playing floor sound");

	std::string newsound = FloorSound;
	//change the asterisk into the current floor number
	ReplaceAll(newsound, "*", ToString(GotoFloor));
	TrimString(newsound);
	floorsound->Stop();
	floorsound->Load(newsound.c_str());
	floorsound->Loop(false);
	floorsound->Play();
	return true;
}

bool Elevator::PlayMessageSound()
{
	//play message sound
	//if direction is true, play up sound; otherwise play down sound

	if (InServiceMode() == true || IsQueueActive() == false || UseMessageSounds == false)
		return false;

	int direction = LastChimeDirection;

	if (LastChimeDirection == 0)
		direction = LastQueueDirection;

	if (direction == 1 && UpMessageSound == "")
		return false;
	if (direction == -1 && DownMessageSound == "")
		return false;

	std::string newsound;

	if (direction == 1)
	{
		if (sbs->Verbose)
			Report("playing up message sound");

		newsound = UpMessageSound;
	}
	else
	{
		if (sbs->Verbose)
			Report("playing down message sound");

		newsound = DownMessageSound;
	}

	//change the asterisk into the current floor number
	ReplaceAll(newsound, "*", ToString(GetFloor()));
	TrimString(newsound);
	messagesnd->Stop();
	messagesnd->Load(newsound.c_str());
	messagesnd->Loop(false);
	messagesnd->Play();
	return true;
}

bool Elevator::DoorExists(int number)
{
	//check if the specified door exists
	//if number is 0, return true if any door exists

	if (number > 0)
		return (GetDoor(number) > 0);

	if (number == 0)
	{
		for (int i = 0; i < (int)DoorArray.size(); i++)
		{
			if (GetDoor(i))
				return true;
		}
	}
	return false;
}

bool Elevator::IsNudgeModeActive(int number)
{
	//checks doors and returns true if any (or the specified door) have nudge mode active

	if (number > 0 && number < (int)DoorArray.size())
	{
		if (DoorArray[number])
			return DoorArray[number]->GetNudgeStatus();
	}
	else if (number == 0)
	{
		for (int i = 0; i < (int)DoorArray.size(); i++)
		{
			if (DoorArray[number]->GetNudgeStatus() == true)
				return true;
		}
	}
	return false;
}

void Elevator::EnableNudgeMode(bool value, int number)
{
	//enables nudge mode on all doors or the specified door

	if (number > 0 && number < (int)DoorArray.size())
	{
		if (DoorArray[number])
			DoorArray[number]->EnableNudgeMode(value);
	}
	else if (number == 0)
	{
		for (int i = 0; i < (int)DoorArray.size(); i++)
			DoorArray[number]->EnableNudgeMode(value);
	}
}

Object* Elevator::AddLight(const char *name, int type, Ogre::Vector3 position, Ogre::Vector3 direction, float color_r, float color_g, float color_b, float spec_color_r, float spec_color_g, float spec_color_b, float spot_inner_angle, float spot_outer_angle, float spot_falloff, float att_range, float att_constant, float att_linear, float att_quadratic)
{
	//add a global light

	Light* light = new Light(object, name, type, position + Origin, direction, color_r, color_g, color_b, spec_color_r, spec_color_g, spec_color_b, spot_inner_angle, spot_outer_angle, spot_falloff, att_range, att_constant, att_linear, att_quadratic);
	lights.push_back(light);
	return light->object;
}

Object* Elevator::AddModel(const char *name, const char *filename, bool center, Ogre::Vector3 position, Ogre::Vector3 rotation, float max_render_distance, float scale_multiplier, bool enable_physics, float restitution, float friction, float mass)
{
	//add a model
	Model* model = new Model(object, name, filename, center, position + Origin, rotation, max_render_distance, scale_multiplier, enable_physics, restitution, friction, mass);
	if (model->load_error == true)
	{
		delete model;
		return 0;
	}
	ModelArray.push_back(model);
	return model->object;
}

void Elevator::MoveObjects(Ogre::Vector3 position, bool relative_x, bool relative_y, bool relative_z)
{
	//move controls
	for (int i = 0; i < (int)ControlArray.size(); i++)
		ControlArray[i]->Move(position, relative_x, relative_y, relative_z);

	//move triggers
	for (int i = 0; i < (int)TriggerArray.size(); i++)
		TriggerArray[i]->Move(position, relative_x, relative_y, relative_z);

	//move models
	for (int i = 0; i < (int)ModelArray.size(); i++)
		ModelArray[i]->Move(position, relative_x, relative_y, relative_z);

	//move lights
	for (int i = 0; i < (int)lights.size(); i++)
		lights[i]->Move(position, relative_x, relative_y, relative_z);
}

void Elevator::AddDisplayFloor(int floor)
{
	//add a floor to the display floors list
	DisplayFloors.push_back(floor);
}

Object* Elevator::AddControl(const char *name, const char *sound, const char *direction, float CenterX, float CenterZ, float width, float height, float voffset, std::vector<std::string> &action_names, std::vector<std::string> &textures)
{
	//add a control
	std::vector<Action*> actionnull; //not used
	Control* control = new Control(object, name, sound, action_names, actionnull, textures, direction, width, height, voffset, true);
	control->SetPosition(Ogre::Vector3(CenterX + Origin.x, Origin.y, CenterZ + Origin.z));
	ControlArray.push_back(control);
	return control->object;
}

Object* Elevator::AddTrigger(const char *name, const char *sound_file, Ogre::Vector3 &area_min, Ogre::Vector3 &area_max, std::vector<std::string> &action_names)
{
	//add a trigger
	Trigger* trigger = new Trigger(object, name, sound_file, area_min, area_max, action_names);
	TriggerArray.push_back(trigger);
	trigger->SetPosition(Origin);
	return trigger->object;
}

void Elevator::ReplaceTexture(const std::string &oldtexture, const std::string &newtexture)
{
	ElevatorMesh->ReplaceTexture(oldtexture, newtexture);
}

std::vector<Sound*> Elevator::GetSound(const char *name)
{
	//get sound by name

	std::string findname = name;
	SetCase(findname, false);
	std::vector<Sound*> soundlist;
	for (int i = 0; i < sounds.size(); i++)
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

	int index = GetFloorIndex(floor);

	if (index == -1)
		return 0;

	float result = 0;
	bool found = false;
	for (int i = 0; i < DoorArray.size(); i++)
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

void Elevator::Init()
{
	//startup elevator initialization

	bool enable_elevators = sbs->GetConfigBool("Skyscraper.SBS.Elevator.IsEnabled", true);

	//turn on shaft doors
	ShaftDoorsEnabled(0, sbs->camera->StartFloor, true);
	ShaftDoorsEnabled(0, sbs->GetShaft(AssignedShaft)->startfloor, true);
	ShaftDoorsEnabled(0, sbs->GetShaft(AssignedShaft)->endfloor, true);

	//disable objects
	EnableObjects(false);
	if (enable_elevators == false)
		Enabled(false);

	//move elevator to starting position
	SetFloor(OriginFloor);
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

bool Elevator::AvailableForCall(int floor, int direction)
{
	//return true or false if the elevator is available for the specified hall call

	//if elevator is running
	if (IsRunning() == true)
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
					//and if it's above the current floor and should be called down, or below the
					//current floor and called up, or on the same floor and not moving, or idle
					if ((GetFloor() > floor && direction == -1) || (GetFloor() < floor && direction == 1) || (GetFloor() == floor && MoveElevator == false) || IsIdle())
					{
						//and if it's either going the same direction as the call, on either the highest/lowest (terminal) floor, or idle
						if (QueuePositionDirection == direction || IsIdle())
						{
							//and if nudge mode is off on all doors
							if (IsNudgeModeActive() == false)
							{
								//and if it's not in any service mode
								if (InServiceMode() == false)
								{
									if (sbs->Verbose)
										Report("Available for call");
									return true;
								}
								else if (sbs->Verbose == true)
									Report("Not available for call - in service mode");
							}
							else if (sbs->Verbose == true)
								Report("Not available for call - in nudge mode");
						}
						else if (sbs->Verbose == true)
							Report("Not available for call - going a different direction and is not idle");
					}
					else if (sbs->Verbose == true)
						Report("Not available for call - position/direction wrong for call");
				}
				else if (sbs->Verbose == true)
					Report("Not available for call - queueresets is on and opposite queue direction is active");
			}
			else if (sbs->Verbose == true)
				Report("Not available for call - limitqueue is on and queue is active");
		}
		else if (sbs->Verbose == true)
			Report("Not available for call - queue change is pending");
	}
	else if (sbs->Verbose == true)
		Report("Not available for call - elevator not running");

	return false;
}

bool Elevator::SelectFloor(int floor)
{
	//select a floor (in-elevator floor selections)

	//exit if in inspection mode or in fire service mode or is not running
	if (InspectionService == true)
	{
		Report("Cannot select floor while in inspection mode");
		return false;
	}
	else if (FireServicePhase1 == 1 && FireServicePhase2 == 0)
	{
		Report("Cannot select floor while in fire service recall mode");
		return false;
	}
	else if (Running == false)
		return false;

	//use Go routine instead if floorhold parameter is enabled
	if (FloorHold == true)
	{
		Go(floor, true);
		return true;
	}

	bool result = false;

	//elevator is above floor
	if (GetFloor() > floor)
		result = AddRoute(floor, -1, true);

	//elevator is below floor
	if (GetFloor() < floor)
		result = AddRoute(floor, 1, true);

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
			result = AddRoute(floor, -Direction, true);
		}
	}

	return result;
}

bool Elevator::Check(Ogre::Vector3 position)
{
	//check to see if user (camera) is in the elevator

	if (IsInElevator(position) == true)
	{
		if (sbs->InElevator == false || sbs->ElevatorNumber != Number)
		{
			EnableObjects(true);
			UpdateFloorIndicators();
		}
		sbs->InElevator = true;
		sbs->ElevatorNumber = Number;
		sbs->ElevatorSync = true;
		return true;
	}
	else if (sbs->InElevator == true && sbs->ElevatorNumber == Number)
		EnableObjects(false); //turn off objects if user is not in the checked elevator
	else if (CameraOffset > Height && CameraOffset < Height * 2)
	{
		//if camera is within vertical elevator range, turn on syncing to allow things like elevator surfing
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
				nearest_difference = fabs(GetPosition().y - sbs->GetFloor(ServicedFloors[i])->Altitude);
				nearest = i;
				firstrun = false;
			}
		}
		else
		{
			if (sbs->GetFloor(ServicedFloors[i]))
			{
				float difference = fabs(GetPosition().y - sbs->GetFloor(ServicedFloors[i])->Altitude);
				if (difference < nearest_difference)
				{
					//mark closest
					nearest_difference = fabs(GetPosition().y - sbs->GetFloor(ServicedFloors[i])->Altitude);
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
			AddRoute(floor, 1, false);
		else
			AddRoute(floor, -1, false);
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
		if (sbs->GetFloor(floor))
		{
			if (GetPosition().y == sbs->GetFloor(floor)->Altitude)
				return true;
		}
	}
	return false;
}

void Elevator::Up()
{
	//move elevator up (manual)
	//this function also stops the up movement when button is depressed

	if (Running == false)
	{
		Report("Elevator not running");
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
		Report("Elevator not running");
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
	Shaft *shaft = sbs->GetShaft(AssignedShaft);
	return shaft;
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
