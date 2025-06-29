/*
	Scalable Building Simulator - Elevator Object
	The Skyscraper Project - Version 2.1
	Copyright (C)2004-2025 Ryan Thoryk
	https://www.skyscrapersim.net
	https://sourceforge.net/projects/skyscraper/
	Contact - ryan@skyscrapersim.net

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
#include "floor.h"
#include "dynamicmesh.h"
#include "mesh.h"
#include "camera.h"
#include "shaft.h"
#include "control.h"
#include "sound.h"
#include "elevatorcar.h"
#include "timer.h"
#include "profiler.h"
#include "texture.h"
#include "controller.h"
#include "random.h"
#include "elevator.h"

#include <time.h>

namespace SBS {

//elevator parking timer
class Elevator::Timer : public TimerObject
{
public:
	Elevator *elevator;
	int type; //0 = parking timer, 1 = arrival/departure, 2 = random malfunctions
	Timer(const std::string &name, Elevator *parent, int Type) : TimerObject(parent, name)
	{
		elevator = parent;
		type = Type;
	}
	virtual void Notify();
};

Elevator::Elevator(Object *parent, int number) : Object(parent)
{
	//set up SBS object
	SetValues("Elevator", "", false);

	//set elevator number
	Number = number;

	//init variables
	Name = "";
	Type = "Local";
	ID = ToString(Number);
	QueuePositionDirection = 0;
	LastQueueDirection = 0;
	LastQueueFloor[0] = 0;
	LastQueueFloor[1] = 0;
	UpSpeed = 0;
	DownSpeed = 0;
	MoveElevator = false;
	GotoFloor = 0;
	GotoFloorCar = 0;
	Acceleration = 0;
	Deceleration = 0;
	AccelJerk = sbs->GetConfigFloat("Skyscraper.SBS.Elevator.AccelJerk", 1);
	DecelJerk = sbs->GetConfigFloat("Skyscraper.SBS.Elevator.DecelJerk", 1);
	ElevatorStart = 0;
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
	TempDeceleration = 0;
	ErrorOffset = 0;
	JerkRate = 0;
	JerkPos = 0;
	MovementRunning = false;
	oldfloor = 0;
	IsMoving = false;
	lastfloor = 0;
	lastfloorset = false;
	MotorUpStartSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.MotorUpStartSound", "motor_start.wav");
	MotorUpRunSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.MotorUpRunSound", "motor_running.wav");
	MotorUpStopSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.MotorUpStopSound", "motor_stop.wav");
	MotorDownStartSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.MotorDownStartSound", "motor_start.wav");
	MotorDownRunSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.MotorDownRunSound", "motor_running.wav");
	MotorDownStopSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.MotorDownStopSound", "motor_stop.wav");
	MotorIdleSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.MotorIdleSound", "");
	UseFloorSkipText = false;
	ACP = sbs->GetConfigBool("Skyscraper.SBS.Elevator.ACP", false);
	ACPFloor = sbs->GetConfigInt("Skyscraper.SBS.Elevator.ACPFloor", 0);
	UpPeak = sbs->GetConfigBool("Skyscraper.SBS.Elevator.UpPeak", false);
	DownPeak = sbs->GetConfigBool("Skyscraper.SBS.Elevator.DownPeak", false);
	IndependentService = sbs->GetConfigBool("Skyscraper.SBS.Elevator.IndependentService", false);
	IndependentServiceCar = 0;
	InspectionService = sbs->GetConfigBool("Skyscraper.SBS.Elevator.InspectionService", false);
	FireServicePhase1 = sbs->GetConfigInt("Skyscraper.SBS.Elevator.FireService1", 0);
	FireServicePhase2 = sbs->GetConfigInt("Skyscraper.SBS.Elevator.FireService2", 0);
	FireServicePhase2Car = 0;
	RecallFloor = 0;
	RecallFloorAlternate = 0;
	OnFloor = true;
	RecallSet = false;
	RecallAltSet = false;
	ACPFloorSet = false;
	RecallUnavailable = false;
	ManualGo = false;
	Created = false;
	MotorPosition = Vector3::ZERO;
	QueueResets = sbs->GetConfigBool("Skyscraper.SBS.Elevator.QueueResets", false);
	FirstRun = true;
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
	motorsound = 0;
	motoridlesound = 0;
	NotifyEarly = sbs->GetConfigInt("Skyscraper.SBS.Elevator.NotifyEarly", 0);
	NotifyLate = sbs->GetConfigBool("Skyscraper.SBS.Elevator.NotifyLate", false);
	Running = sbs->GetConfigBool("Skyscraper.SBS.Elevator.Run", true);
	Notified = false;
	Parking = false;
	DepartureDelay = sbs->GetConfigFloat("Skyscraper.SBS.Elevator.DepartureDelay", 0.0);
	ArrivalDelay = sbs->GetConfigFloat("Skyscraper.SBS.Elevator.ArrivalDelay", 0.0);
	WaitForTimer = false;
	SoundsQueued = false;
	HeightSet = false;
	elevposition = Vector3::ZERO;
	ManualUp = false;
	ManualDown = false;
	InspectionSpeed = sbs->GetConfigFloat("Skyscraper.SBS.Elevator.InspectionSpeed", 0.6);
	LimitQueue = sbs->GetConfigBool("Skyscraper.SBS.Elevator.LimitQueue", false);
	UpQueueEmpty = false;
	DownQueueEmpty = false;
	UpCall = false;
	DownCall = false;
	QueuePending = false;
	ReOpen = sbs->GetConfigBool("Skyscraper.SBS.Elevator.ReOpen", true);
	LastChimeDirection = 0;
	AutoDoors = sbs->GetConfigBool("Skyscraper.SBS.Elevator.AutoDoors", true);
	OpenOnStart = sbs->GetConfigBool("Skyscraper.SBS.Elevator.OpenOnStart", false);
	ManualMove = 0;
	ManualMoveHold = false;
	Interlocks = sbs->GetConfigBool("Skyscraper.SBS.Elevator.Interlocks", true);
	GoActive = false;
	GoActiveFloor = 0;
	FloorHold = sbs->GetConfigBool("Skyscraper.SBS.Elevator.FloorHold", false);
	GoPending = false;
	EmergencyStopSpeed = sbs->GetConfigFloat("Skyscraper.SBS.Elevator.EmergencyStopSpeed", 3.0);
	MotorEmergencyStopSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.MotorEmergencyStopSound", "");
	AutoAdjustSound = sbs->GetConfigBool("Skyscraper.SBS.Elevator.AutoAdjustSound", false);
	SkipFloorSound = false;
	ManualStop = false;
	ChimeOnArrival = sbs->GetConfigBool("Skyscraper.SBS.Elevator.ChimeOnArrival", false);
	HoistwayAccess = 0;
	HoistwayAccessFloor = 0;
	HoistwayAccessHold = sbs->GetConfigBool("Skyscraper.SBS.Elevator.HoistwayAccessHold", true);
	RopePosition = Vector3::ZERO;
	CounterweightStartSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.CounterweightStartSound", "");
	CounterweightMoveSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.CounterweightMoveSound", "");
	CounterweightStopSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.CounterweightStopSound", "");
	counterweightsound = 0;
	Counterweight = false;
	WeightMesh = 0;
	WeightRopeMesh = 0;
	RopeMesh = 0;
	Error = false;
	RandomProbability = sbs->GetConfigInt("Skyscraper.SBS.Elevator.RandomProbability", 20);
	RandomFrequency = sbs->GetConfigFloat("Skyscraper.SBS.Elevator.RandomFrequency", 5);

	//initialize random number generators
	rnd_time = new RandomGen((unsigned int)(time(0) + GetNumber()));
	rnd_type = new RandomGen((unsigned int)(time(0) + GetNumber() + 1));

	//create timers
	parking_timer = new Timer("Parking Timer", this, 0);
	arrival_delay = new Timer("Arrival Delay Timer", this, 1);
	departure_delay = new Timer("Departure Delay Timer", this, 1);
	malfunction_timer = new Timer("Malfunction Timer", this, 2);

	//create object meshes
	std::string name = "Elevator " + ToString(Number);
	SetName(name);

	//create a primary car object
	AddCar();

	//create a dynamic mesh for elevator doors
	DoorContainer = new DynamicMesh(this, GetSceneNode(), name + " Door Container", 0, true);
	DoorContainer->force_combine = true;

	if (sbs->Verbose)
		Report("elevator object created");

	EnableLoop(true);
}

Elevator::~Elevator()
{
	//delete counterweight and rope meshes
	if (sbs->Verbose)
		Report("deleting meshes");

	if (counterweightsound)
	{
		counterweightsound->parent_deleting = true;
		delete counterweightsound;
	}
	counterweightsound = 0;

	if (WeightMesh)
	{
		WeightMesh->parent_deleting = true;
		delete WeightMesh;
	}
	WeightMesh = 0;

	if (WeightRopeMesh)
	{
		WeightRopeMesh->parent_deleting = true;
		delete WeightRopeMesh;
	}
	WeightRopeMesh = 0;

	if (RopeMesh)
	{
		RopeMesh->parent_deleting = true;
		delete RopeMesh;
	}
	RopeMesh = 0;

	//delete timers
	if (sbs->Verbose)
		Report("deleting timers");

	if (parking_timer)
	{
		parking_timer->parent_deleting = true;
		delete parking_timer;
	}
	parking_timer = 0;

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

	if (malfunction_timer)
	{
		malfunction_timer->parent_deleting = true;
		delete malfunction_timer;
	}
	malfunction_timer = 0;

	//delete random number generators
	if (rnd_time)
		delete rnd_time;
	rnd_time = 0;

	if (rnd_type)
		delete rnd_type;
	rnd_type = 0;

	//delete cars
	if (sbs->Verbose)
		Report("deleting cars");

	for (size_t i = 0; i < Cars.size(); i++)
	{
		if (Cars[i])
		{
			Cars[i]->parent_deleting = true;
			delete Cars[i];
		}
		Cars[i] = 0;
	}
	Cars.clear();

	//delete door container
	if (DoorContainer)
		delete DoorContainer;
	DoorContainer = 0;

	//delete sounds
	if (sbs->Verbose)
		Report("deleting motor sounds");

	if (motorsound)
	{
		motorsound->parent_deleting = true;
		delete motorsound;
	}
	motorsound = 0;
	if (motoridlesound)
	{
		motoridlesound->parent_deleting = true;
		delete motoridlesound;
	}
	motoridlesound = 0;

	//unregister from parent
	if (sbs->FastDelete == false && parent_deleting == false)
		sbs->RemoveElevator(this);
}

bool Elevator::CreateElevator(bool relative, Real x, Real z, int floor)
{
	//Creates elevator at specified location and floor
	//x and z are the center coordinates
	//if relative is true, then x and z coordinates are relative
	//to the assigned shaft's center

	if (Created == true)
		return ReportError("Has already been created");

	//make sure required values are set
	if (UpSpeed <= 0 || DownSpeed <= 0)
		return ReportError("Speed not set or invalid");

	if (Acceleration <= 0)
		return ReportError("Acceleration not set or invalid");

	if (Deceleration <= 0)
		return ReportError("Deceleration not set or invalid");

	if (AccelJerk <= 0)
		return ReportError("Invalid value for AccelJerk");

	if (DecelJerk <= 0)
		return ReportError("Invalid value for DecelJerk");

	if (AssignedShaft <= 0)
		return ReportError("Not assigned to a shaft");

	for (size_t i = 0; i < Controllers.size(); i++)
	{
		if (Controllers[i] < 0)
			return ReportError("Invalid value for Controller");
	}

	if (!GetShaft())
		return ReportError("Shaft " + ToString(AssignedShaft) + " doesn't exist");

	//set starting position
	Vector3 position = Vector3::ZERO;

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

	//add elevator to associated shaft
	GetShaft()->AddElevator(Number);

	//add elevator to associated dispatch controllers
	for (size_t i = 0; i < (size_t)Controllers.size(); i++)
	{
		if (sbs->GetController(Controllers[i]))
			sbs->GetController(Controllers[i])->AddElevator(Number);
	}

	//set recall/ACP floors if not already set
	if (RecallSet == false)
		SetRecallFloor(GetCar(1)->GetBottomFloor());
	if (RecallAltSet == false)
		SetAlternateRecallFloor(GetCar(1)->GetTopFloor());
	if (ACPFloorSet == false)
		SetACPFloor(GetCar(1)->GetBottomFloor());

	//move objects to positions
	if (sbs->Verbose)
		Report("moving elevator to origin position");
	SetPosition(position);

	//create motor sounds
	std::string motorname = "Motor " + ToString(Number);
	motorsound = new Sound(GetShaft(), motorname, true);
	motorname += " Idle";
	motoridlesound = new Sound(GetShaft(), motorname, true);

	//move motor to top of shaft if location not specified, or to location
	if (MotorPosition != Vector3(0, 0, 0))
		motorsound->SetPosition(MotorPosition.x + GetPosition().x, MotorPosition.y, MotorPosition.z + GetPosition().z);
	else
	{
		Floor *floor = sbs->GetFloor(GetShaft()->endfloor);
		if (floor)
			motorsound->SetPositionY(floor->GetBase());
	}
	MotorPosition = Vector3(motorsound->GetPosition().x - GetPosition().x, motorsound->GetPosition().y, motorsound->GetPosition().z - GetPosition().z);
	motoridlesound->SetPosition(motorsound->GetPosition());

	Created = true;

	Report("created at " + TruncateNumber(position.x, 4) + ", " + TruncateNumber(position.z, 4));

	//set up primary car
	if (!GetCar(1)->CreateCar(floor))
		return false;

	elevposition = GetPosition();

	return true;
}

Wall* Elevator::CreateCounterweight(const std::string &frame_texture, const std::string &weight_texture, Real x, Real z, const Vector3 &size, Real weight_voffset)
{
	//create a counterweight and ropes for this elevator

	for (int i = 0; i < GetCarCount(); i++)
	{
		if (GetCar(i)->Created == false)
		{
			ReportError("Car " + ToString(i) + " not created yet");
			return 0;
		}
	}

	//create counterweight and rope meshes
	WeightMesh = new MeshObject(this, GetName() + " Counterweight");
	WeightRopeMesh = new MeshObject(this, GetName() + " Counterweight Rope", 0, "", "", 0, 1, false, false);
	RopeMesh = new MeshObject(this, GetName() + " Rope", 0, "", "", 0, 1, false, false);

	//save and change autosizing
	bool autosize_x, autosize_y;
	sbs->GetTextureManager()->GetAutoSize(autosize_x, autosize_y);
	sbs->GetTextureManager()->SetAutoSize(false, false);

	Wall *frame = sbs->CreateWallBox(WeightMesh, "Counterweight Frame", frame_texture, -size.x / 2, size.x / 2, -size.z / 2, size.z / 2, size.y, 0, 1, 1, true, true, false, false, false);
	Wall *counterweight = sbs->CreateWallBox(WeightMesh, "Counterweight", weight_texture, (-size.x / 2) + 0.01, (size.x / 2) - 0.01, (-size.z / 2) + 0.01, (size.z / 2) - 0.01, size.y / 2, weight_voffset, 1, 1);

	Floor *topfloor, *carfloor, *topcarfloor, *bottomfloor;
	topfloor = sbs->GetFloor(GetTopFloor());
	carfloor = sbs->GetFloor(GetCar(1)->GetFloor());
	topcarfloor = sbs->GetFloor(GetCar(GetCarCount() - 1)->GetFloor());
	bottomfloor = sbs->GetFloor(GetBottomFloor());

	x += GetCar(1)->GetPosition().x;
	z += GetCar(1)->GetPosition().z;

	WeightMesh->SetPosition(Vector3(x, topfloor->GetBase() - (carfloor->GetBase() - bottomfloor->GetBase()), z));
	WeightRopeMesh->SetPosition(Vector3(x, WeightMesh->GetPosition().y + (size.y - 0.5), z));
	RopeMesh->SetPosition(GetCar(GetCarCount() - 1)->GetPosition() + RopePosition);

	Real counterweight_rope_height = MotorPosition.y - (WeightMesh->GetPosition().y + (size.y - 0.5));
	Wall *c_rope;
	if (size.x > size.z)
		c_rope = sbs->AddWall(WeightRopeMesh, "Counterweight Rope", RopeTexture, 0, -size.x / 4, 0, size.x / 4, 0, counterweight_rope_height, counterweight_rope_height, 0, 0, 1, 1);
	else
		c_rope = sbs->AddWall(WeightRopeMesh, "Counterweight Rope", RopeTexture, 0, 0, -size.z / 4, 0, size.z / 4, counterweight_rope_height, counterweight_rope_height, 0, 0, 1, 1);

	Real rope_height = MotorPosition.y - RopeMesh->GetPosition().y;
	Wall *rope;
	if (size.x > size.z)
		rope = sbs->AddWall(RopeMesh, "Rope", RopeTexture, 0, -size.x / 4, 0, size.x / 4, 0, rope_height, rope_height, 0, 0, 1, 1);
	else
		rope = sbs->AddWall(RopeMesh, "Rope", RopeTexture, 0, 0, -size.z / 4, 0, size.z / 4, rope_height, rope_height, 0, 0, 1, 1);

	counterweightsound = new Sound(WeightMesh, "Counterweight Sound", true);

	weight_size = size;

	//restore autosize
	sbs->GetTextureManager()->SetAutoSize(autosize_x, autosize_y);

	Counterweight = true;
	return counterweight;
}

bool Elevator::AddRails(const std::string &main_texture, const std::string &edge_texture, Real x, Real z, bool orientation, Real rail_distance, Real rail_width)
{
	//creates rails for the elevator cars or counterweight
	//if orientation is false, create rails along the X axis, otherwise the Z axis

	//check if shaft is associated
	if (!GetShaft())
		return ReportError("AddRails: no shaft available");

	Vector3 offset;
	offset = GetPosition() - GetShaft()->GetPosition();

	MeshObject *mesh = GetShaft()->GetLevel(GetShaft()->startfloor)->GetMeshObject();
	Real height = MotorPosition.y - sbs->GetFloor(GetShaft()->startfloor)->Altitude;

	if (orientation == false)
	{
		//left side
		sbs->CreateWallBox2(mesh, "Rails L1", edge_texture, -rail_distance + x + offset.x + (rail_width / 4), z + offset.z, rail_width / 2, rail_width / 8, height, 0, 1, 1);
		sbs->CreateWallBox2(mesh, "Rails L2", main_texture, -rail_distance + x + offset.x - (rail_width / 4), z + offset.z, rail_width / 2, rail_width / 8, height, 0, 1, 1);
		sbs->CreateWallBox2(mesh, "Rails L3", main_texture, -rail_distance + x + offset.x - (rail_width / 2), z + offset.z, rail_width / 8, rail_width, height, 0, 1, 1);

		//right side
		sbs->CreateWallBox2(mesh, "Rails R1", edge_texture, rail_distance + x + offset.x - (rail_width / 4), z + offset.z, rail_width / 2, rail_width / 8, height, 0, 1, 1);
		sbs->CreateWallBox2(mesh, "Rails R2", main_texture, rail_distance + x + offset.x + (rail_width / 4), z + offset.z, rail_width / 2, rail_width / 8, height, 0, 1, 1);
		sbs->CreateWallBox2(mesh, "Rails R3", main_texture, rail_distance + x + offset.x + (rail_width / 2), z + offset.z, rail_width / 8, rail_width, height, 0, 1, 1);
	}
	else
	{
		//front side
		sbs->CreateWallBox2(mesh, "Rails F1", edge_texture, x + offset.x, -rail_distance + z + offset.z + (rail_width / 4), rail_width / 8, rail_width / 2, height, 0, 1, 1);
		sbs->CreateWallBox2(mesh, "Rails F2", main_texture, x + offset.x, -rail_distance + z + offset.z - (rail_width / 4), rail_width / 8, rail_width / 2, height, 0, 1, 1);
		sbs->CreateWallBox2(mesh, "Rails F3", main_texture, x + offset.x, -rail_distance + z + offset.z - (rail_width / 2), rail_width, rail_width / 8, height, 0, 1, 1);

		//back side
		sbs->CreateWallBox2(mesh, "Rails B1", edge_texture, x + offset.x, rail_distance + z + offset.z - (rail_width / 4), rail_width / 8, rail_width / 2, height, 0, 1, 1);
		sbs->CreateWallBox2(mesh, "Rails B2", main_texture, x + offset.x, rail_distance + z + offset.z + (rail_width / 4), rail_width / 8, rail_width / 2, height, 0, 1, 1);
		sbs->CreateWallBox2(mesh, "Rails B3", main_texture, x + offset.x, rail_distance + z + offset.z + (rail_width / 2), rail_width, rail_width / 8, height, 0, 1, 1);
	}

	return true;
}

bool Elevator::AddRoute(int floor, int direction, int call_type)
{
	//Add call route to elevator routing table, in sorted order
	//directions are either 1 for up, or -1 for down
	//call type is 0 for a car call, 1 for a hall call, 2 for a system call

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

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

	//get related car number
	ElevatorCar *car = GetCarForFloor(floor);

	if (!car)
		return ReportError("floor " + ToString(floor) + " is not a serviced floor");

	//if car is on the same floor, perform an arrival
	if (car->IsOnFloor(floor))
	{
		if (QueuePositionDirection == direction ||
			QueuePositionDirection == 0 ||
			(QueuePositionDirection == 1 && UpQueue.size() == 0) ||
			(QueuePositionDirection == -1 && DownQueue.size() == 0))
		{
			SameFloorArrival(floor, direction);
			return true;
		}
	}

	//add route in related direction queue
	if (direction == 1)
	{
		//exit if entry already exists
		if (RouteExists(true, floor) == true)
			return ReportError("route to floor " + ToString(floor) + " (" + floorobj->ID + ") already exists");

		//add floor to up queue
		UpQueue.emplace_back(QueueEntry(floor, call_type, car->Number, 1));
		//sort queue
		std::sort(UpQueue.begin(), UpQueue.end());
		QueuePending = true;

		LastQueueFloor[0] = floor;
		LastQueueFloor[1] = 1;

		//add car number info if needed
		std::string car_msg = "";
		if (GetCarCount() > 1)
			car_msg = " for car " + ToString(car->Number);

		Report("adding route to floor " + ToString(floor) + " (" + floorobj->ID + ") direction Up" + car_msg);
	}
	else
	{
		//exit if entry already exists
		if (RouteExists(false, floor) == true)
			return ReportError("route to floor " + ToString(floor) + " (" + floorobj->ID + ") already exists");

		//add floor to down queue
		DownQueue.emplace_back(QueueEntry(floor, call_type, car->Number, -1));
		//sort queue
		std::sort(DownQueue.begin(), DownQueue.end());
		QueuePending = true;

		LastQueueFloor[0] = floor;
		LastQueueFloor[1] = -1;

		//add car number info if needed
		std::string car_msg = "";
		if (GetCarCount() > 1)
			car_msg = " for car " + ToString(car->Number);

		Report("adding route to floor " + ToString(floor) + " (" + floorobj->ID + ") direction Down" + car_msg);
	}

	//make sure the car's GotoFloor status is set, if this is an additional car heading the same route
	ProcessGotoFloor(floor, direction);

	//turn on button lights for a car call, or a Destination Dispatch hall call
	if (call_type == 0)
		ChangeLight(floor, true);

	//go to ACP floor if ACP mode is enabled
	if (ACP == true && floor != ACPFloor)
	{
		//only add ACP route if original route will pass ACP floor
		if ((car->GetFloor() < ACPFloor && floor > ACPFloor) || (car->GetFloor() > ACPFloor && floor < ACPFloor))
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

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	if (Running == false)
		return ReportError("Elevator not running");

	Floor *floorobj = sbs->GetFloor(floor);

	if (!floorobj)
		return ReportError("Invalid floor " + ToString(floor));

	if (direction == 1)
	{
		//delete floor entry from up queue
		for (size_t i = 0; i < UpQueue.size(); i++)
		{
			if (UpQueue[i].floor == floor)
			{
				Report("deleting route to floor " + ToString(floor) + " (" + floorobj->ID + ") direction Up");
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
		for (size_t i = 0; i < DownQueue.size(); i++)
		{
			if (DownQueue[i].floor == floor)
			{
				Report("deleting route to floor " + ToString(floor) + " (" + floorobj->ID + ") direction Down");
				DownQueue.erase(DownQueue.begin() + i);
				break;
			}
		}
		if (DownQueue.size() == 0)
			DownQueueEmpty = true;
	}

	HandleDequeue(direction);

	//turn off button lights
	if (sbs->Verbose)
		Report("DeleteRoute: turning off button lights for floor " + ToString(floor));
	ChangeLight(floor, false);
	return true;
}

bool Elevator::RouteExists(bool direction, int floor)
{
	//return true if a floor route exists in the specified directional queue

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	if (direction == true)
	{
		for (size_t i = 0; i < UpQueue.size(); i++)
		{
			if (UpQueue[i].floor == floor)
				return true;
		}
	}
	else
	{
		for (size_t i = 0; i < DownQueue.size(); i++)
		{
			if (DownQueue[i].floor == floor)
				return true;
		}
	}

	return false;
}

bool Elevator::CallCancel()
{
	//cancel the last route

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	if (Running == false)
		return ReportError("Elevator not running");

	if (LastQueueFloor[1] == 0)
	{
		if (sbs->Verbose)
			ReportError("CallCancel: no valid routes");
		return false;
	}

	DeleteRoute(LastQueueFloor[0], LastQueueFloor[1]);
	Report("cancelled last call");

	return true;
}

bool Elevator::CallCancelAll()
{
	//cancels all added routes

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	if (Running == false)
		return ReportError("Elevator not running");

	if (LastQueueFloor[1] == 0)
	{
		if (sbs->Verbose)
			ReportError("CallCancel: no valid routes");
		return false;
	}

	Report("cancelled all calls");
	ResetQueue(true, true);

	return true;
}

bool Elevator::Stop(bool emergency)
{
	//Tells elevator to stop moving, no matter where it is

	if (EmergencyStop > 0)
		return false;

	if (emergency == true)
	{
		//exit if in inspection mode
		if (InspectionService == true)
			return ReportError("cannot stop while in inspection service mode");

		//exit if in fire service phase 1 recall
		if (FireServicePhase1 == 1 && FireServicePhase2 == 0)
			return ReportError("cannot stop while in fire service 1 recall mode");
	}

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

void Elevator::ProcessCallQueue()
{
	//Processes the elevator's call queue, and sends elevators to called floors
	SBS_PROFILE("Elevator::ProcessCallQueue");

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return;

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
	if (UpQueue.empty() && DownQueue.empty())
	{
		UpQueueEmpty = false;
		DownQueueEmpty = false;

		if (DownPeak == true || UpPeak == true)
		{
			int TopFloor = GetCar(1)->GetTopFloor();
			int BottomFloor = GetCar(1)->GetBottomFloor();

			//if DownPeak mode is active, send elevator to the top serviced floor if not already there
			if (GetCar(1)->GetFloor() != TopFloor && DownPeak == true && IsMoving == false)
			{
				if (sbs->Verbose)
					Report("ProcessCallQueue: sending elevator to top floor for DownPeak mode");
				AddRoute(TopFloor, 1, 2);
				return;
			}
			//if UpPeak mode is active, send elevator to the bottom serviced floor if not already there
			else if (GetCar(1)->GetFloor() != BottomFloor && UpPeak == true && IsMoving == false)
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
		if (UpQueue.empty() == false)
		{
			if (sbs->Verbose)
				Report("ProcessCallQueue: setting search direction to up");
			QueuePositionDirection = 1;
		}
		else if (DownQueue.empty() == false)
		{
			if (sbs->Verbose)
				Report("ProcessCallQueue: setting search direction to down");
			QueuePositionDirection = -1;
		}
		LastQueueDirection = 0;
	}

	//reverse queues if related queue empty flag is set
	if (QueuePositionDirection == 1 && UpQueueEmpty == true && DownQueue.empty() == false && (NotifyEarly <= 0 || NotifyEarly == 3))
	{
		if (UpCall == false)
		{
			if (sbs->Verbose)
				Report("ProcessCallQueue: setting search direction to down");
			LastQueueDirection = QueuePositionDirection;
			QueuePositionDirection = -1;
		}
	}
	if (QueuePositionDirection == -1 && DownQueueEmpty == true && UpQueue.empty() == false && (NotifyEarly <= 0 || NotifyEarly == 3))
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
		if (QueuePositionDirection == 1 && UpQueue.empty())
		{
			if (sbs->Verbose)
				Report("ProcessCallQueue: resetting search direction due to empty up queue");
			QueuePositionDirection = 0;
			LastQueueDirection = 1;
		}
		if (QueuePositionDirection == -1 && DownQueue.empty())
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
		for (size_t i = 0; i < UpQueue.size(); i++)
		{
			ElevatorCar *car = GetCarForFloor(UpQueue[i].floor);
			if (!car)
				return;

			std::string car_msg = "";
			if (GetCarCount() > 1)
				car_msg = " for car " + ToString(car->Number);

			//if the queued floor number is a higher floor, dispatch the elevator to that floor
			if (UpQueue[i].floor >= car->CurrentFloor)
			{
				if (MoveElevator == false)
				{
					if (sbs->Verbose)
						Report("ProcessCallQueue up: standard dispatch, floor " + ToString(UpQueue[i].floor) + car_msg);
					ActiveCall = UpQueue[i];
					GotoFloor = UpQueue[i].floor;
					GotoFloorCar = car->Number;
					car->GotoFloor = true;
					if (FireServicePhase2 == 0 || UpPeak == true || DownPeak == true)
					{
						WaitForDoors = true;
						CloseDoors();
					}
					MoveElevator = true;
					LastQueueDirection = 1;
					QueuePending = false;
					ProcessGotoFloor(GotoFloor, QueuePositionDirection);
				}
				else if (Leveling == false && ActiveDirection == 1)
				{
					//if elevator is moving and not leveling, change destination floor if not beyond decel marker of that floor
					if (GotoFloor != UpQueue[i].floor)
					{
						if (car == GetCar(GotoFloorCar)) //make sure car is the same
						{
							Real tmpdestination = GetDestinationAltitude(UpQueue[i].floor);
							if (BeyondDecelMarker(1, tmpdestination) == false && sbs->GetFloor(GotoFloor))
							{
								ActiveCall = UpQueue[i];
								GotoFloor = UpQueue[i].floor;
								GotoFloorCar = car->Number;
								Destination = tmpdestination;
								Report("changing destination floor to " + ToString(GotoFloor) + " (" + sbs->GetFloor(GotoFloor)->ID + ")" + car_msg);
							}
							else if (sbs->Verbose)
								Report("ProcessCallQueue up: cannot change destination floor to " + ToString(UpQueue[i].floor) + car_msg);
						}
					}
				}
				return;
			}
			//if the queued floor number is a lower floor
			if (UpQueue[i].floor < car->CurrentFloor && MoveElevator == false)
			{
				//dispatch elevator if it's idle
				if (IsIdle() == true && LastQueueDirection == 0)
				{
					if (sbs->Verbose)
						Report("ProcessCallQueue up: dispatching idle lower elevator, floor " + ToString(UpQueue[i].floor) + car_msg);
					ActiveCall = UpQueue[i];
					GotoFloor = UpQueue[i].floor;
					GotoFloorCar = car->Number;
					car->GotoFloor = true;
					if (FireServicePhase2 == 0 || UpPeak == true || DownPeak == true)
					{
						WaitForDoors = true;
						CloseDoors();
					}
					MoveElevator = true;
					LastQueueDirection = 1;
					QueuePending = false;
					ProcessGotoFloor(GotoFloor, QueuePositionDirection);
					return;
				}
				//reset search direction if it's the last entry and idle
				if (i == UpQueue.size() - 1 && IsIdle() == true && QueuePositionDirection != 0)
				{
					if (sbs->Verbose)
						Report("ProcessCallQueue up: resetting search direction since last entry is lower" + car_msg);
					LastQueueDirection = QueuePositionDirection;
					QueuePositionDirection = 0;
					return;
				}
				//otherwise skip it if it's not the last entry
				if (sbs->Verbose)
					Report("ProcessCallQueue up: skipping floor entry " + ToString(UpQueue[i].floor) + car_msg);
			}
		}
	}
	else if (QueuePositionDirection == -1)
	{
		//search through down queue (search order is reversed since calls need to be processed in descending order)
		for (size_t i = DownQueue.size() - 1; i < DownQueue.size(); --i)
		{
			ElevatorCar *car = GetCarForFloor(DownQueue[i].floor);
			if (!car)
				return;

			std::string car_msg = "";
			if (GetCarCount() > 1)
				car_msg = " for car " + ToString(car->Number);

			//if the queued floor number is a lower floor, dispatch the elevator to that floor
			if (DownQueue[i].floor <= car->CurrentFloor)
			{
				if (MoveElevator == false)
				{
					if (sbs->Verbose)
						Report("ProcessCallQueue down: standard dispatch, floor " + ToString(DownQueue[i].floor) + car_msg);
					ActiveCall = DownQueue[i];
					GotoFloor = DownQueue[i].floor;
					GotoFloorCar = car->Number;
					car->GotoFloor = true;
					if (FireServicePhase2 == 0 || UpPeak == true || DownPeak == true)
					{
						WaitForDoors = true;
						CloseDoors();
					}
					MoveElevator = true;
					LastQueueDirection = -1;
					QueuePending = false;
					ProcessGotoFloor(GotoFloor, QueuePositionDirection);
				}
				else if (Leveling == false && ActiveDirection == -1)
				{
					//if elevator is moving and not leveling, change destination floor if not beyond decel marker of that floor
					if (GotoFloor != DownQueue[i].floor)
					{
						if (car == GetCar(GotoFloorCar)) //make sure car is the same
						{
							Real tmpdestination = GetDestinationAltitude(DownQueue[i].floor);
							if (BeyondDecelMarker(-1, tmpdestination) == false && sbs->GetFloor(GotoFloor))
							{
								ActiveCall = DownQueue[i];
								GotoFloor = DownQueue[i].floor;
								GotoFloorCar = car->Number;
								Destination = tmpdestination;
								Report("changing destination floor to " + ToString(GotoFloor) + " (" + sbs->GetFloor(GotoFloor)->ID + ")" + car_msg);
							}
							else if (sbs->Verbose)
								Report("ProcessCallQueue down: cannot change destination floor to " + ToString(DownQueue[i].floor) + car_msg);
						}
					}
				}
				return;
			}
			//if the queued floor number is an upper floor
			if (DownQueue[i].floor > car->CurrentFloor && MoveElevator == false)
			{
				//dispatch elevator if idle
				if (IsIdle() == true && LastQueueDirection == 0)
				{
					if (sbs->Verbose)
						Report("ProcessCallQueue down: dispatching idle higher elevator, floor " + ToString(DownQueue[i].floor) + car_msg);
					ActiveCall = DownQueue[i];
					GotoFloor = DownQueue[i].floor;
					GotoFloorCar = car->Number;
					car->GotoFloor = true;
					if (FireServicePhase2 == 0 || UpPeak == true || DownPeak == true)
					{
						WaitForDoors = true;
						CloseDoors();
					}
					MoveElevator = true;
					LastQueueDirection = -1;
					QueuePending = false;
					ProcessGotoFloor(GotoFloor, QueuePositionDirection);
					return;
				}
				//reset search direction if it's the last entry and idle
				if (i == 0 && IsIdle() == true && QueuePositionDirection != 0)
				{
					if (sbs->Verbose)
						Report("ProcessCallQueue down: resetting search direction since last entry is higher" + car_msg);
					LastQueueDirection = QueuePositionDirection;
					QueuePositionDirection = 0;
					return;
				}
				//otherwise skip it if it's not the last entry
				if (sbs->Verbose)
					Report("ProcessCallQueue down: skipping floor entry " + ToString(DownQueue[i].floor) + car_msg);
			}
		}
	}
}

void Elevator::Loop()
{
	//Monitors elevator and starts actions if needed

	SBS_PROFILE("Elevator::Loop");

	if (Created == false)
		return;

	//only run if power is enabled
	if (sbs->GetPower() == false)
	{
		Stop(true);
	}

	//make sure height value is set
	if (HeightSet == false)
	{
		for (int i = 1; i <= GetCarCount(); i++)
		{
			GetCar(i)->SetHeight();
		}
		HeightSet = true;
	}

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
			EnableIndependentService(true, IndependentServiceCar);
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
			EnableFireService2(value, FireServicePhase2Car);
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

		UpdateFloorIndicators();
	}

	DoSetControls();

	if (MotorIdleSound != "")
	{
		//play motor idle sound
		if (motoridlesound->IsPlaying() == false && Running == true)
		{
			if (sbs->Verbose)
				Report("playing motor idle sound");

			if (motoridlesound->IsLoaded() == false)
				motoridlesound->Load(MotorIdleSound);

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

	//process up/down buttons
	if (ManualMoveHold == true)
	{
		if (ManualMove == 1)
			Up();
		if (ManualMove == -1)
			Down();
	}

	//process Go function hold
	if (GoActive == true)
		Go(GoActiveFloor, true);

	if (HoistwayAccess != 0 && HoistwayAccessHold == true)
		SetHoistwayAccess(HoistwayAccessFloor, HoistwayAccess);

	//call queue processor
	ProcessCallQueue();

	//enable auto-park timer if specified
	if (parking_timer->IsRunning() == false && ParkingDelay > 0 && Running == true && IsIdle() == true && InServiceMode() == false && AutoDoors == true)
		parking_timer->Start(int(ParkingDelay * 1000), true);

	//run per-car loops
	for (int i = 1; i <= GetCarCount(); i++)
	{
		GetCar(i)->Loop();
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

	Vector3 movement = Vector3(0, 0, 0);
	bool StartLeveling = false;
	Error = false;

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

	//validate car object
	if (!GetCar(GotoFloorCar))
	{
		ReportError("Invalid elevator car");
		Destination = 0;
		Direction = 0;
		MoveElevator = false;
		MovementRunning = false;
		Error = true;
		DeleteActiveRoute();
		return;
	}

	if (MovementRunning == false)
	{
		if (Running == false)
		{
			ReportError("Elevator not running");
			Error = true;
			return;
		}

		if (sbs->Verbose)
			Report("starting elevator movement procedure");

		MovementRunning = true;
		FinishedMove = false;
		ManualStop = false;
		std::string dir_string;

		Notified = false;

		//reset arrival status on controllers
		for (size_t i = 0; i < Controllers.size(); i++)
		{
			if (sbs->GetController(Controllers[i]))
				sbs->GetController(Controllers[i])->ResetArrival(Number);
		}

		//get elevator's current altitude
		elevposition = GetPosition();
		ElevatorStart = elevposition.y;

		//get elevator's current floor (first car)
		GetCar(1)->CurrentFloor = GetCar(1)->GetFloor();
		oldfloor = GetCar(1)->CurrentFloor;

		//switch off directional indicators on current floor(s) if not already done so
		DirectionalIndicatorsOff();

		//exit if floor doesn't exist
		if (!sbs->GetFloor(GotoFloor) && ManualMove == 0)
		{
			ReportError("Destination floor does not exist");
			MoveElevator = false;
			MovementRunning = false;
			Error = true;
			DeleteActiveRoute();
			return;
		}

		//if destination floor is not a serviced floor, reset and exit
		if (GetCar(GotoFloorCar)->IsServicedFloor(GotoFloor) == false && InspectionService == false && ManualMove == 0)
		{
			ReportError("Destination floor not a serviced floor");
			MoveElevator = false;
			MovementRunning = false;
			Error = true;
			DeleteActiveRoute();
			return;
		}

		//if elevator is already on specified floor, open doors and finish
		if (GetCar(GotoFloorCar)->CurrentFloor == GotoFloor && InspectionService == false && IsLeveled() == true && ManualMove == 0)
		{
			ReportError("Elevator already on specified floor");
			MoveElevator = false;
			MovementRunning = false;
			SkipFloorSound = true; //don't play floor announcement if on same floor
			Error = true;
			goto finish; //skip main processing and run cleanup section
		}

		//determine direction
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

		bool skip_interlock = false;
		if (InspectionService == true && HoistwayAccess == Direction)
		{
			//skip the interlock check if Hoistway Access is enabled
			//for the floor and direction, and the shaft doors are open
			if (GetCar(GotoFloorCar)->AreShaftDoorsOpen(0, HoistwayAccessFloor) == true)
			{
				skip_interlock = true;
				Report("skipping interlock check for floor " + ToString(HoistwayAccessFloor));
			}
		}

		//exit if doors are not fully closed while interlocks enabled
		if (CheckInterlocks() == false && skip_interlock == false)
		{
			ReportError("Doors must be closed before moving when interlocks are enabled");
			MoveElevator = false;
			MovementRunning = false;
			Error = true;
			Direction = 0;
			DeleteActiveRoute();
			return;
		}

		//determine distance to destination floor
		if (InspectionService == false && ManualMove == 0)
			DistanceToTravel = std::abs(std::abs(Destination) - std::abs(ElevatorStart));
		else
		{
			//otherwise if inspection service is on, choose the altitude of the top/bottom floor
			if (Direction == 1)
			{
				Destination = GetDestinationAltitude(GetCar(1)->GetTopFloor());
				if (ElevatorStart >= Destination)
				{
					//don't go above top floor
					ReportError("cannot go above top floor");
					Destination = 0;
					Direction = 0;
					MoveElevator = false;
					MovementRunning = false;
					Error = true;
					DeleteActiveRoute();
					return;
				}
			}
			else
			{
				Destination = GetDestinationAltitude(GetCar(1)->GetBottomFloor());
				if (ElevatorStart <= Destination)
				{
					//don't go below bottom floor
					ReportError("cannot go below bottom floor");
					Destination = 0;
					Direction = 0;
					MoveElevator = false;
					MovementRunning = false;
					Error = true;
					DeleteActiveRoute();
					return;
				}
			}
			DistanceToTravel = std::abs(std::abs(Destination) - std::abs(ElevatorStart));
		}
		CalculateStoppingDistance = true;

		//if user is riding this elevator, then turn off objects
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

			//turn off sky, buildings, and landscape
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
			ResetShaftDoors(GetCar(sbs->CarNumber)->GetFloor());
		}

		//set interior directional indicators
		UpdateDirectionalIndicators();

		//set external active-direction indicators
		if (sbs->GetFloor(sbs->camera->CurrentFloor))
			sbs->GetFloor(sbs->camera->CurrentFloor)->UpdateDirectionalIndicators(Number);

		std::string car_msg = "";
		if (GetCarCount() > 1)
			car_msg = " for car " + ToString(GotoFloorCar);

		//notify about movement
		if (InspectionService == false && ManualMove == 0)
			Report("moving " + dir_string + " to floor " + ToString(GotoFloor) + " (" + sbs->GetFloor(GotoFloor)->ID + ")" + car_msg);
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

		//play directional message sound if MessageOnDoor is false
		for (size_t i = 0; i < Cars.size(); i++)
		{
			if (Cars[i]->MessageOnMove == true)
				Cars[i]->PlayMessageSound(true);
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
		Real limit = 0;
		if (InspectionService == false)
		{
			if (ActiveDirection == 1)
				limit = UpSpeed;
			else
				limit = DownSpeed;
		}
		else if (HoistwayAccess == 0)
		{
			if (ActiveDirection == 1)
				limit = UpSpeed * InspectionSpeed;
			else
				limit = DownSpeed * InspectionSpeed;
		}
		else
			limit = LevelingSpeed; //if hoistway access is on, run at leveling speed

		Real Speed;
		if (ActiveDirection == 1)
			Speed = UpSpeed;
		else
			Speed = DownSpeed;

		if (Direction == 1 && ElevatorRate < limit)
			ElevatorRate += Speed * ((Acceleration * JerkRate) * sbs->delta);
		else if (Direction == -1 && ElevatorRate > -limit)
			ElevatorRate -= Speed * ((Acceleration * JerkRate) * sbs->delta);
		else
			CalculateStoppingDistance = false;
	}
	else if (Leveling == false)
	{
		//slow down

		//calculate jerk rate
		//check if the elevator rate is less than the amount that was stored in JerkPos
		//(the elevator rate at the end of the JerkRate increments), adjusted to the ratio of acceljerk to deceljerk

		Real tmppos = JerkPos * (AccelJerk / DecelJerk);
		if ((Direction == -1 && ElevatorRate <= tmppos) || (Direction == 1 && ElevatorRate >= tmppos))
		{
			if (ElevatorRate != 0)
			{
				if (tmpDecelJerk == 0)
					tmpDecelJerk = DecelJerk * (tmppos / ElevatorRate);
				JerkRate -= tmpDecelJerk * sbs->delta;
			}
		}
		//prevent jerkrate from reaching 0
		if (JerkRate < 0)
		{
			JerkRate = 0;
			ElevatorRate = 0;
		}

		Real Speed;
		if (ActiveDirection == 1)
			Speed = UpSpeed;
		else
			Speed = DownSpeed;

		if (Direction == 1)
			ElevatorRate += Speed * ((TempDeceleration * JerkRate) * sbs->delta);
		if (Direction == -1)
			ElevatorRate -= Speed * ((TempDeceleration * JerkRate) * sbs->delta);
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
				Real Speed;
				if (ActiveDirection == 1)
					Speed = UpSpeed;
				else
					Speed = DownSpeed;

				if (InspectionService == false)
					ElevatorRate -= Speed * ((TempDeceleration * JerkRate) * sbs->delta);
				else
					ElevatorRate -= (Speed * InspectionSpeed) * ((TempDeceleration * JerkRate) * sbs->delta);
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
				Real Speed;
				if (ActiveDirection == 1)
					Speed = UpSpeed;
				else
					Speed = DownSpeed;

				if (InspectionService == false)
					ElevatorRate += Speed * ((TempDeceleration * JerkRate) * sbs->delta);
				else
					ElevatorRate += (Speed * InspectionSpeed) * ((TempDeceleration * JerkRate) * sbs->delta);
			}

			if (ElevatorRate != 0.0)
			{
				PlayStoppingSounds();

				if ((NotifyEarly == 2 || NotifyEarly == 3) && Parking == false)
					NotifyArrival(true);
			}
		}
	}
	else if (Leveling == false && EmergencyStop == 0)
	{
		if (std::abs(ElevatorRate) <= LevelingSpeed)
		{
			//turn on leveling if elevator's rate is less than or equal to the leveling speed value
			if (sbs->Verbose)
				Report("leveling enabled");
			Leveling = true;
			StartLeveling = true;

			if (NotifyEarly == 1 && Parking == false)
				NotifyArrival(true);
		}
	}

	//play floor beep and update indicators, if passing by or arriving at a floor
	if ((GetCar(1)->GetFloor() != oldfloor && Leveling == false) || StartLeveling == true)
	{
		Real alt = sbs->GetFloor(GetCar(1)->GetFloor())->Altitude;
		bool pass = false;

		//determine if elevator will pass floor, only for down movement
		if (ActiveDirection == -1 && elevposition.y >= alt && elevposition.y + movement.y < alt)
			pass = true;
		if (ActiveDirection == 1)
			pass = true;

		ElevatorCar *gotocar = GetCar(GotoFloorCar);

		//if elevator hasn't started leveling, and is about to arrive at the destination, cancel any update
		if (gotocar->GetFloor() == GotoFloor && StartLeveling == false)
			pass = false;

		if (pass == true || StartLeveling == true)
		{
			if (sbs->Verbose)
			{
				if (GotoFloorCar == 1)
					Report("on floor " + ToString(GetCar(1)->GetFloor()));
				else
					Report("on floor " + ToString(GetCar(1)->GetFloor()) + " (" + ToString(GetCar(GotoFloorCar)->GetFloor()) + " for car " + ToString(GotoFloorCar) + ")");
			}

			//play floor beep sound if floor is a serviced floor
			if (gotocar->IsServicedFloor(gotocar->GetFloor()) == true)
				gotocar->PlayFloorBeep();

			//update floor indicators
			UpdateFloorIndicators();

			//update floor indicators on current camera floor
			if (sbs->GetFloor(sbs->camera->CurrentFloor))
				sbs->GetFloor(sbs->camera->CurrentFloor)->UpdateFloorIndicators(Number);

			oldfloor = GetCar(1)->GetFloor();
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
	if (ElevatorRate != 0)
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
		GetCar(GotoFloorCar)->ChangeLight(GetCar(GotoFloorCar)->GetFloor(), false);
	}
	ElevatorRate = 0;
	JerkRate = 0;
	Direction = 0;
	Brakes = false;
	Destination = 0;
	DistanceToTravel = 0;
	ElevatorStart = 0;
	MovementRunning = false;
	MoveElevator = false;
	IsMoving = false;
	Leveling = false;
	tmpDecelJerk = 0;

	StopSounds();

	if (FinishedMove == false)
		FinishMove();
	else
		EmergencyStop = 0; //make sure emergency stop status is cleared

	//dequeue floor route
	if (EmergencyStop == 0 && IsManuallyStopped() == false)
		DeleteActiveRoute();

	//reset cars' GotoFloor states
	for (int i = 1; i <= GetCarCount(); i++)
	{
		GetCar(i)->GotoFloor = false;
	}
}

void Elevator::MoveObjects(Real offset)
{
	//move elevator and objects vertically

	SBS_PROFILE("Elevator::MoveObjects");

	Vector3 vector (0, offset, 0);

	Move(vector);
	elevposition.y = GetPosition().y;

	//move counterweight
	if (WeightMesh)
	{
		WeightMesh->Move(-vector * 2);
		WeightRopeMesh->Move(-vector * 2);
		Floor *topfloor = sbs->GetFloor(GetTopFloor());
		Real counterweight_rope_height = MotorPosition.y - (WeightMesh->GetPosition().y + (weight_size.y - 0.5));
		WeightRopeMesh->ChangeHeight(counterweight_rope_height);
		Real rope_height = MotorPosition.y - RopeMesh->GetPosition().y;
		RopeMesh->ChangeHeight(rope_height);
	}

	//move camera
	if (sbs->ElevatorSync == true && sbs->ElevatorNumber == Number)
		sbs->camera->MovePosition(vector);
}

void Elevator::FinishMove()
{
	//post-move operations, such as chimes, opening doors, indicator updates, etc

	if (IsManuallyStopped() == true || InspectionService == true)
	{
		GotoFloor = GetCar(1)->GetFloor();
		GotoFloorCar = 1;
	}

	if (EmergencyStop == 0 || IsManuallyStopped() == true)
	{
		//the elevator is now stopped on a valid floor; set OnFloor to true
		OnFloor = true;
		if (GetCarCount() == 1)
			Report("arrived at floor " + ToString(GotoFloor) + " (" + sbs->GetFloor(GotoFloor)->ID + ")");
		else
			Report("arrived at floor " + ToString(GotoFloor) + " (" + sbs->GetFloor(GotoFloor)->ID + ") in car " + ToString(GotoFloorCar));
	}

	//turn off interior directional indicators
	ActiveDirection = 0;
	UpdateDirectionalIndicators();

	//update external active-direction indicators
	if (sbs->GetFloor(sbs->camera->CurrentFloor))
		sbs->GetFloor(sbs->camera->CurrentFloor)->UpdateDirectionalIndicators(Number);

	if ((EmergencyStop == 0 || IsManuallyStopped() == true) && InspectionService == false)
	{
		//update floor indicators on current camera floor
		if (sbs->GetFloor(sbs->camera->CurrentFloor))
			sbs->GetFloor(sbs->camera->CurrentFloor)->UpdateFloorIndicators(Number);

		UpdateFloorIndicators();

		ElevatorCar *cameracar = GetCarForFloor(sbs->camera->CurrentFloor);

		if (!cameracar)
			cameracar = GetCar(GotoFloorCar);

		//turn on objects if user is in elevator
		if (sbs->ElevatorSync == true && sbs->ElevatorNumber == Number && cameracar->CameraOffset < cameracar->Height)
		{
			if (sbs->Verbose)
				Report("user in elevator - turning on objects");

			//turn on floor
			int floor = GotoFloor;
			if (GotoFloor != sbs->camera->CurrentFloor)
				floor = sbs->camera->CurrentFloor;
			sbs->GetFloor(floor)->Enabled(true);
			sbs->GetFloor(floor)->EnableGroup(true);

			//Turn on sky, buildings, and landscape
			sbs->EnableSkybox(true);
			sbs->EnableBuildings(true);
			sbs->EnableLandscape(true);
			sbs->EnableExternal(true);

			//reset shaft doors
			for (int i = 1; i <= GetCarCount(); i++)
			{
				if (GetCar(i)->GotoFloor == true)
				{
					int floor = GetFloorForCar(i, GotoFloor);
					ResetShaftDoors(floor);
				}
			}
		}
		else if (sbs->Verbose)
			Report("user not in elevator - not turning on objects");

		//notify arrival and disable call button light
		if (InServiceMode() == false)
		{
			//notify on arrival
			if (((NotifyEarly == 0 || NotifyEarly == 3) || Notified == false) && Parking == false && NotifyEarly != -1)
				NotifyArrival(false);

			//get status of call buttons before switching off
			GetCallStatus(GotoFloor, UpCall, DownCall);

			for (int i = 1; i <= GetCarCount(); i++)
			{
				if (GetCar(i)->GotoFloor == true)
				{
					int floor = GetFloorForCar(i, GotoFloor);

					//notify dispatch controllers of arrival
					for (size_t i = 0; i < Controllers.size(); i++)
					{
						if (sbs->GetController(Controllers[i]))
							sbs->GetController(Controllers[i])->ElevatorArrived(Number, floor, GetArrivalDirection(floor));
					}
				}
			}
		}

		//reset queues if specified
		if (QueueResets == true)
		{
			//if last entry in current queue, reset opposite queue
			if (QueuePositionDirection == 1 && UpQueue.size() <= 1 && DownQueue.empty() == false)
				ResetQueue(false, true, false);
			else if (QueuePositionDirection == -1 && DownQueue.size() <= 1 && UpQueue.empty() == false)
				ResetQueue(true, false, false);
		}

		//reverse queues if at either top or bottom of serviced floors
		if (QueuePositionDirection == 1 && GotoFloor == GetCar(GotoFloorCar)->GetTopFloor())
		{
			if (sbs->Verbose)
				Report("at top floor; setting queue search direction to down");
			LastQueueDirection = QueuePositionDirection;
			QueuePositionDirection = -1;
		}
		else if (QueuePositionDirection == -1 && GotoFloor == GetCar(GotoFloorCar)->GetBottomFloor())
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
				{
					if (FireServicePhase2 == 1)
					{
						if (GetCar(FireServicePhase2Car))
							GetCar(FireServicePhase2Car)->OpenDoors();
					}
					else
					{
						if ((OnRecallFloor() == true && FireServicePhase1 == 1) || OnPeakFloor() == true)
							OpenDoors(); //automatically open doors in Fire Phase 1 and Peak modes
						else
						{
							for (int i = 1; i <= GetCarCount(); i++)
							{
								if (GetCar(i)->GotoFloor == true)
									GetCar(i)->OpenDoors();
							}
						}
					}
				}
		}
	}
	else
	{
		if (sbs->Verbose)
			Report("stop complete");

		//reset shaft doors
		if (sbs->ElevatorSync == true && sbs->ElevatorNumber == Number)
		{
			for (int i = 1; i <= GetCarCount(); i++)
			{
				if (GetCar(i)->GotoFloor == true)
				{
					int floor = GetFloorForCar(i, GotoFloor);
					ResetShaftDoors(floor);
				}
			}
		}
	}

	//update car floor numbers
	for (int i = 1; i <= GetCarCount(); i++)
	{
		GetCar(i)->CurrentFloor = GetCar(i)->GetFloor();
	}

	EmergencyStop = 0;
	ManualStop = false;
	SkipFloorSound = false;
	Parking = false;
	FinishedMove = true;
}

void Elevator::DumpQueues()
{
	//dump both (up and down) elevator queues

	Object::Report("\n--- " + GetName() + "'s Queues ---\n");

	if (UpQueue.size() > 0)
		Object::Report("Up:");

	for (size_t i = 0; i < UpQueue.size(); i++)
	{
		std::string type = "Car";
		if (UpQueue[i].call_type == 1)
			type = "Hall";
		if (UpQueue[i].call_type == 2)
			type = "System";
		if (UpQueue[i].call_type == 3)
			type = "Destination";

		std::string car;
		if (GetCarCount() > 1)
			car = "\t-\tCar: " + ToString(UpQueue[i].car);

		Object::Report("Entry: " + ToString((int)i) + "\t-\tFloor: " + ToString(UpQueue[i].floor) + "\t-\tCall type: " + type + car);
	}

	if (DownQueue.size() > 0)
		Object::Report("Down:");

	for (size_t i = 0; i < DownQueue.size(); i++)
	{
		std::string type = "Car";
		if (DownQueue[i].call_type == 1)
			type = "Hall";
		if (DownQueue[i].call_type == 2)
			type = "System";
		if (DownQueue[i].call_type == 3)
			type = "Destination";

		std::string car;
		if (GetCarCount() > 1)
			car = "\t-\tCar: " + ToString(DownQueue[i].car);

		Object::Report("Entry: " + ToString((int)i) + "\t-\tFloor: " + ToString(DownQueue[i].floor) + "\t-\tCall type: " + type + car);
	}
	Object::Report("");
}

void Elevator::Enabled(bool value)
{
	//shows/hides elevator

	if (IsEnabled == value)
		return;

	EnableLoop(value);

	if (sbs->Verbose)
	{
		if (value == true)
			Report("enabling elevator");
		else
			Report("disabling elevator");
	}

	for (size_t i = 0; i < Cars.size(); i++)
	{
		Cars[i]->Enabled(value);
	}
	IsEnabled = value;
}

ElevatorCar* Elevator::IsInElevator(const Vector3 &position, bool camera)
{
	//determine if the given 3D position is inside the elevator
	//returns the related car object, or 0 if not found

	//first checks to see if camera is within a car's height range, and then
	//checks for a collision with the car's floor below

	//if camera is true, set associated camera offset

	//SBS_PROFILE("Elevator::IsInElevator");

	if (IsEnabled == false)
		return 0;

	for (size_t i = 0; i < Cars.size(); i++)
	{
		if (Cars[i]->IsInCar(position, camera) == true)
			return Cars[i];
	}
	return 0;
}

Real Elevator::GetElevatorStart()
{
	//returns the internal elevator starting position
	return ElevatorStart;
}

Real Elevator::GetDestination()
{
	//returns the internal destination value
	return Destination;
}

Real Elevator::GetStoppingDistance()
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

void Elevator::UpdateFloorIndicators()
{
	//updates all floor indicators

	SBS_PROFILE("Elevator::UpdateFloorIndicators");

	for (size_t i = 0; i < Cars.size(); i++)
	{
		Cars[i]->UpdateFloorIndicators();
	}
}

Real Elevator::GetJerkRate()
{
	return JerkRate;
}

Real Elevator::GetJerkPosition()
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

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

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

		ElevatorCar *car = GetCarForFloor(floor);
		if (!car)
			return false;

		if (hold == true)
		{
			GoActive = true;
			GoActiveFloor = floor;
		}
		Report("Go: proceeding to floor " + ToString(floor) + " (" + sbs->GetFloor(floor)->ID + ")");
		ChangeLight(floor, true);
		GotoFloor = floor;
		GotoFloorCar = car->Number;
		GoPending = true;
		if (AutoDoors == true)
		{
			WaitForDoors = true;
			CloseDoors();
		}
		MoveElevator = true;
	}
	else if (GoActive == true && hold == true && sbs->camera->MouseDown() == false)
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

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return;

	if (Running == false)
	{
		ReportError("Elevator not running");
		return;
	}

	//reset queues (this will also stop the elevator)
	ResetQueue(true, true);

	if (OnRecallFloor() == true)
	{
		if (RecallUnavailable == false)
			Report("On recall floor");
		else
			Report("On alternate recall floor");
		if (AutoDoors == true)
		{
			OpenDoors();
		}
		return;
	}

	if (RecallUnavailable == false)
	{
		Report("Proceeding to recall floor");
		if (RecallFloor > GetCar(1)->GetFloor())
			AddRoute(RecallFloor, 1, 2);
		else
			AddRoute(RecallFloor, -1, 2);
	}
	else
	{
		Report("Proceeding to alternate recall floor");
		if (RecallFloorAlternate > GetCar(1)->GetFloor())
			AddRoute(RecallFloorAlternate, 1, 2);
		else
			AddRoute(RecallFloorAlternate, -1, 2);
	}
}

int Elevator::GetRecallFloor()
{
	//returns the recall floor of the elevator

	if (RecallUnavailable == true)
		return RecallFloorAlternate;
	return RecallFloor;
}

bool Elevator::EnableACP(bool value)
{
	//enable Anti-Crime Protection (ACP) mode

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	if (Running == false)
		return ReportError("Elevator not running");

	//exit if no change
	if (ACP == value)
	{
		if (sbs->Verbose)
			ReportError("EnableACP: mode is the same");
		return true;
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
	{
		SetControls("acpon");
		Report("ACP mode enabled");
	}
	else
	{
		SetControls("acpoff");
		Report("ACP mode disabled");
	}

	return true;
}

bool Elevator::EnableUpPeak(bool value)
{
	//enable Up-Peak mode

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	if (Running == false)
		return ReportError("Elevator not running");

	//exit if no change
	if (UpPeak == value)
	{
		if (sbs->Verbose)
			ReportError("EnableUpPeak: mode is the same");
		return true;
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
		if (IsMoving == false && OnBottomFloor() == true)
		{
			//set directional indicators on all cars
			for (size_t i = 0; i < Cars.size(); i++)
			{
				if (sbs->GetFloor(Cars[i]->GetFloor()))
				{
					Cars[i]->SetDirectionalIndicators(Cars[i]->GetFloor(), true, false);

					if (AutoDoors == true)
						Cars[i]->OpenDoors();
				}
			}
		}
		SetControls("uppeakon");
		Report("Up Peak mode enabled");
	}
	else
	{
		ResetDoors();
		ResetNudgeTimers();
		SetControls("uppeakoff");
		Report("Up Peak mode disabled");
	}

	return true;
}

bool Elevator::EnableDownPeak(bool value)
{
	//enable Down-Peak mode

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	if (Running == false)
		return ReportError("Elevator not running");

	//exit if no change
	if (DownPeak == value)
	{
		if (sbs->Verbose)
			ReportError("EnableDownPeak: mode is the same");
		return true;
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
		if (IsMoving == false && OnTopFloor() == true)
		{
			//set directional indicators on all cars
			for (size_t i = 0; i < Cars.size(); i++)
			{
				if (sbs->GetFloor(Cars[i]->GetFloor()))
				{
					Cars[i]->SetDirectionalIndicators(Cars[i]->GetFloor(), false, true);

					if (AutoDoors == true)
						Cars[i]->OpenDoors();
				}
			}
		}
		SetControls("downpeakon");
		Report("Down Peak mode enabled");
	}
	else
	{
		ResetDoors();
		ResetNudgeTimers();
		SetControls("downpeakoff");
		Report("Down Peak mode disabled");
	}

	return true;
}

bool Elevator::EnableIndependentService(bool value, int car_number)
{
	//enable Independent Service (ISC) mode
	//car_number is the car number to use for independent service; only used when enabling the mode

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	if (Running == false)
		return ReportError("Elevator not running");

	//exit if mode is already active for another car
	if (IndependentService == true && car_number > 0 && IndependentServiceCar != car_number)
	{
		ReportError("EnableIndependentService: mode already active for car " + ToString(IndependentServiceCar));
		return !value; //succeed if disabling mode
	}

	//exit if no change
	if (IndependentService == value)
	{
		if (sbs->Verbose)
			ReportError("EnableIndependentService: mode is the same");
		return true;
	}

	if (car_number == 0)
		car_number = 1;

	if (value == true)
	{
		if (InspectionService == true)
			return ReportError("EnableIndependentService: cannot enable while in inspection service mode");
		if (FireServicePhase1 > 0 || FireServicePhase2 > 0)
			return ReportError("EnableIndependentService: cannot enable while in a fire service mode");

		ElevatorCar *car = GetCar(car_number);
		if (!car)
			return ReportError("EnableIndependentService: invalid car " + ToString(car_number));

		IndependentService = true;
		IndependentServiceCar = car_number;
		EnableACP(false);
		EnableUpPeak(false);
		EnableDownPeak(false);
		ResetQueue(true, true); //this will also stop the elevator
		car->HoldDoors(); //turn off door timers for selected car
		car->ResetNudgeTimer(false); //switch off nudge timer for selected car
		DirectionalIndicatorsOff(); //switch off directional indicators on current floor
		if (IsMoving == false)
			if (AutoDoors == true)
				car->OpenDoors();
		SetControls("indon");
		Report("Independent Service mode enabled for car " + ToString(car_number));
	}
	else
	{
		IndependentService = false;
		IndependentServiceCar = 0;
		ResetQueue(true, true); //this will also stop the elevator
		ResetDoors();
		ResetNudgeTimers();
		SetControls("indoff");
		Report("Independent Service mode disabled");
	}

	return true;
}

bool Elevator::EnableInspectionService(bool value)
{
	//enable Inspection Service (INS) mode

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	//exit if no change
	if (InspectionService == value)
	{
		if (sbs->Verbose)
			ReportError("EnableInspectionService: mode is the same");
		return true;
	}

	if (value == true)
	{
		EnableACP(false);
		EnableUpPeak(false);
		EnableDownPeak(false);
		EnableIndependentService(false);
		EnableFireService1(0);
		EnableFireService2(0, true);
		ResetQueue(true, true); //this will also stop the elevator
		HoldDoors(); //turn off door timers
		ResetNudgeTimers(false); //switch off nudge timer
		DirectionalIndicatorsOff(); //switch off directional indicators on current floor
		SetControls("inson");
		Report("Inspection Service mode enabled");
		HoistwayAccess = 0;
		HoistwayAccessFloor = 0;
		InspectionService = true;
	}
	else
	{
		ResetDoors();
		ResetNudgeTimers();
		SetControls("insoff");
		Report("Inspection Service mode disabled");

		UpdateFloorIndicators();

		//turn on objects if user is in elevator
		if (sbs->ElevatorSync == true && sbs->ElevatorNumber == Number && IsMoving == false && GetCar(sbs->CarNumber))
		{
			if (sbs->Verbose)
				Report("user in elevator - turning on objects");

			int carfloor = GetCar(sbs->CarNumber)->GetFloor();

			//turn on floor
			if (sbs->GetFloor(carfloor))
			{
				sbs->GetFloor(carfloor)->Enabled(true);
				sbs->GetFloor(carfloor)->EnableGroup(true);
			}

			//Turn on sky, buildings, and landscape
			sbs->EnableSkybox(true);
			sbs->EnableBuildings(true);
			sbs->EnableLandscape(true);
			sbs->EnableExternal(true);

			//reset shaft doors
			ResetShaftDoors(carfloor);
		}

		HoistwayAccess = 0;
		HoistwayAccessFloor = 0;
		InspectionService = false;

		if (IsMoving == true)
			Stop();
		else
			ReturnToNearestFloor(true);
	}

	return true;
}

bool Elevator::EnableFireService1(int value)
{
	//enable Fire Service Phase 1 mode
	//valid values are 0 (off), 1 (on) or 2 (bypass)

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	if (Running == false)
		return ReportError("Elevator not running");

	//exit if no change
	if (FireServicePhase1 == value)
	{
		if (sbs->Verbose)
			ReportError("EnableFireService1: mode is the same");
		return true;
	}

	//exit if in inspection mode
	if (InspectionService == true && value > 0)
		return ReportError("EnableFireService1: cannot enable while in inspection service mode");

	if (value < 0 || value > 2)
		return ReportError("EnableFireService1: invalid value");

	FireServicePhase1 = value;

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
			DirectionalIndicatorsOff();

			//recall elevator if not in phase 2 hold
			if (FireServicePhase2 != 2)
			{
				//turn off all door timers
				HoldDoors();
				ResetNudgeTimers(false); //switch off nudge timer

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
				ResetNudgeTimers();
			}
			Report("Fire Service Phase 1 mode set to Bypass");
		}
	}
	else
	{
		if (FireServicePhase2 == 0)
		{
			ResetDoors(); //enable door timers
			ResetNudgeTimers();
		}
		Report("Fire Service Phase 1 mode set to Off");
	}

	return true;
}

bool Elevator::EnableFireService2(int value, int car_number, bool force)
{
	//enable Fire Service Phase 2 mode
	//valid values are 0 (off), 1 (on) or 2 (hold)
	//if force is true, does not require doors to be opened to change value

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

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

	//exit if mode is already active for another car
	if (FireServicePhase2 > 0 && car_number > 0 && FireServicePhase2Car != car_number)
	{
		ReportError("EnableFireService2: mode already active for car " + ToString(FireServicePhase2Car));
		return !value; //succeed if disabling mode
	}

	//require doors to be open to change modes
	if (AreDoorsOpen() == false && force == false)
		return ReportError("EnableFireService2: doors must be open to change phase 2 modes");

	//exit if no change
	if (FireServicePhase2 == value)
	{
		if (sbs->Verbose)
			ReportError("EnableFireService2: mode is the same");
		return true;
	}

	if (car_number == 0)
		car_number = 1;

	if (value >= 0 && value <= 2)
		FireServicePhase2 = value;
	else
		return ReportError("EnableFireService2: invalid value");

	if (value > 0)
	{
		ElevatorCar *car = GetCar(car_number);
		if (!car)
			return ReportError("EnableFireService2: invalid car " + ToString(car_number));

		FireServicePhase2Car = car_number;
		EnableACP(false);
		EnableUpPeak(false);
		EnableDownPeak(false);
		EnableIndependentService(false);
		ResetQueue(true, true); //this will also stop the elevator
		car->HoldDoors(); //disable all door timers for selected car
		car->ResetNudgeTimer(false); //switch off nudge timer for selected car

		//close other doors
		for (int i = 1; i <= GetCarCount(); i++)
		{
			if (i != car_number)
				GetCar(i)->CloseDoors();
		}

		if (value == 1)
		{
			SetControls("fire2on");
			Report("Fire Service Phase 2 mode set to On for car " + ToString(car_number));
		}
		else
		{
			SetControls("fire2hold");
			Report("Fire Service Phase 2 mode set to Hold for car " + ToString(car_number));
		}
	}
	else
	{
		SetControls("fire2off");
		Report("Fire Service Phase 2 mode set to Off");

		FireServicePhase2Car = 0;

		if (FireServicePhase1 == 0)
		{
			ResetDoors(); //enable door timers
			ResetNudgeTimers();
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

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	if (GetCar(1)->IsServicedFloor(floor) == false)
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

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	if (GetCar(1)->IsServicedFloor(floor) == false)
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

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	ElevatorCar *car = GetCarForFloor(floor);
	if (!car)
		return ReportError("Invalid ACP floor");
	if (car->IsServicedFloor(floor) == false)
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

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

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

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

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

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	if (Running == false)
		return ReportError("Elevator not running");

	if (InspectionService == false)
		return false;

	if (ManualGo == true && value == false)
		Stop();

	ManualGo = value;

	if (value == true)
		SetControls("goon");
	else
		SetControls("gooff");

	if (sbs->Verbose)
	{
		if (value == true)
			Report("setting go button status to on");
		else
			Report("setting go button status to off");
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

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	if (Running == false)
		return ReportError("Elevator not running");

	if (InspectionService == false)
		return false;

	if (ManualUp == true && value == false)
		Stop();

	ManualUp = value;

	if (value == true)
		SetControls("upon");
	else
		SetControls("upoff");

	if (sbs->Verbose)
	{
		if (value == true)
			Report("setting up button status to on");
		else
			Report("setting up button status to off");
	}

	if (ManualGo == true && value == true)
		MoveUp();

	return true;
}

bool Elevator::SetDownButton(bool value)
{
	//sets the value of the Go button (for Inspection Service mode)

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	if (Running == false)
		return ReportError("Elevator not running");

	if (InspectionService == false)
		return false;

	if (ManualDown == true && value == false)
		Stop();

	ManualDown = value;

	if (value == true)
		SetControls("downon");
	else
		SetControls("downoff");

	if (sbs->Verbose)
	{
		if (value == true)
			Report("setting down button status to on");
		else
			Report("setting down button status to off");
	}

	if (ManualGo == true && value == true)
		MoveDown();

	return true;
}

int Elevator::GetTopFloor()
{
	//returns highest serviced floor

	return GetCar(GetCarCount())->GetTopFloor();
}

int Elevator::GetBottomFloor()
{
	//returns lowest serviced floor

	return GetCar(1)->GetBottomFloor();
}

void Elevator::UpdateDirectionalIndicators()
{
	//updates all interior active direction indicators

	SBS_PROFILE("Elevator::UpdateDirectionalIndicators");

	for (size_t i = 0; i < Cars.size(); i++)
	{
		Cars[i]->UpdateDirectionalIndicators();
	}
}

bool Elevator::IsIdle()
{
	//return true if elevator is idle (not moving, doors are closed (unless in a peak mode) and not moving)

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	if (MoveElevator == false && (AreDoorsOpen() == false || UpPeak == true || DownPeak == true) && AreDoorsMoving() == 0 && Running == true)
		return true;
	else
		return false;
}

void Elevator::ResetQueue(bool up, bool down, bool stop_if_empty)
{
	//reset queues
	//if stop_if_empty is true, the elevator will stop when the related queue is empty

	QueuePending = false;

	if (up == true)
	{
		if (sbs->Verbose)
			Report("QueueReset: resetting up queue");
		UpQueue.clear();
		HandleDequeue(1, stop_if_empty);
	}
	if (down == true)
	{
		if (sbs->Verbose)
			Report("QueueReset: resetting down queue");
		DownQueue.clear();
		HandleDequeue(-1, stop_if_empty);
	}

	ResetLights();
}

void Elevator::DeleteActiveRoute()
{
	//only run if power is enabled
	if (sbs->GetPower() == false)
		return;

	if (Running == false)
	{
		ReportError("Elevator not running");
		return;
	}

	//deletes the active route
	if (sbs->Verbose)
		Report("deleting active route");
	DeleteRoute(ActiveCall.floor, ActiveCall.direction);

	//delete associated routes for any other cars
	for (int i = 1; i <= GetCarCount(); i++)
	{
		int floor = GetFloorForCar(i, ActiveCall.floor);
		if (IsQueued(floor, ActiveCall.direction) == true)
			DeleteRoute(floor, ActiveCall.direction);
	}

	ActiveCall.floor = 0;
	ActiveCall.direction = 0;
	ActiveCall.call_type = 0;
	ActiveCall.car = 0;
}

bool Elevator::IsQueueActive()
{
	if (QueuePositionDirection != 0)
		return true;
	return false;
}

bool Elevator::BeyondDecelMarker(int direction, Real destination)
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
	Object::Report(GetName() + ": " + message);
}

bool Elevator::ReportError(const std::string &message)
{
	//general reporting function
	return Object::ReportError(GetName() + ": " + message);
}

void Elevator::Timer::Notify()
{
	//elevator timer loop

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return;

	if (elevator->IsRunning() == false)
		return;

	SBS_PROFILE("Elevator::Timer::Notify");
	if (type == 0)
	{
		//parking timer

		if (elevator->ParkingDelay > 0 && elevator->IsIdle() == true && elevator->InServiceMode() == false)
		{
			ElevatorCar *car = elevator->GetCarForFloor(elevator->ParkingFloor);
			if (car)
			{
				int floor = car->GetFloor();
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
		}
		else if (elevator->InServiceMode() == true)
			Stop(); //stop timer if in service mode
	}
	else if (type == 1)
	{
		//arrival and departure timers
		elevator->WaitForTimer = false;
	}
	else if (type == 2)
	{
		//malfunction timer
		int result = (int)elevator->rnd_time->Get(elevator->RandomProbability - 1);
		if (result == 0)
			elevator->Malfunction();
	}
}

bool Elevator::IsQueued(int floor, int queue)
{
	//return true if the given floor is in either queue
	//if queue is 0, check both queues; otherwise up queue with 1, and down queue with -1

	if (queue == 0 || queue == 1)
	{
		for (size_t i = 0; i < UpQueue.size(); i++)
		{
			if (UpQueue[i].floor == floor)
				return true;
		}
	}

	if (queue == 0 || queue == -1)
	{
		for (size_t i = 0; i < DownQueue.size(); i++)
		{
			if (DownQueue[i].floor == floor)
				return true;
		}
	}

	return false;
}

void Elevator::NotifyArrival(bool early)
{
	//notify on elevator arrival (play chime and turn on related directional indicator lantern)
	//for all cars

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return;

	for (int i = 1; i <= GetCarCount(); i++)
	{
		if (GetCar(i)->GotoFloor == true)
		{
			int floor = GetFloorForCar(i, GotoFloor);
			GetCar(i)->NotifyArrival(floor, early);
		}
	}

	Notified = true;
}

bool Elevator::GetArrivalDirection(int floor)
{
	//determine if the directional lantern should show up or down on arrival to the specified floor
	//true for up, false for down

	ElevatorCar *car = GetCarForFloor(floor);

	if (!car)
		return false;

	int newfloor = floor;

	if (floor == car->GetTopFloor())
		return false; //turn on down light if on top floor
	if (floor == car->GetBottomFloor())
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
	GetCallStatus(floor, UpStatus, DownStatus);
	if (UpStatus == true && QueuePositionDirection == 1)
		return true;
	if (DownStatus == true && QueuePositionDirection == -1)
		return false;

	if (NotifyEarly <= 0 || NotifyEarly == 3)
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

	//return direction of queue if floors are the same
	if (newfloor == floor)
	{
		if (QueuePositionDirection == 1 && UpQueue.size() > 0)
			return true;
		else if (QueuePositionDirection == -1 && DownQueue.size() > 0)
			return false;
	}

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
		DirectionalIndicatorsOff();

	if (value == false)
	{
		SetControls("stop");
		Report("Elevator stopped");
	}
	else
	{
		SetControls("run");
		Report("Elevator running");
	}

	Running = value;
}

bool Elevator::IsRunning()
{
	//return elevator running state
	return Running;
}

Real Elevator::GetDestinationAltitude(int floor)
{
	//returns the destination altitude of the specified floor, based on shaft door positioning
	//this adjusts the value based on the car offset, so that it returns the altitude that
	//car 1 would have to be at, to match an upper car's destination

	ElevatorCar *car = GetCarForFloor(floor);
	if (!car)
		return 0.0;

	return car->GetDestinationAltitude(floor) - GetCarOffset(car->Number);
}

Real Elevator::GetDestinationOffset(int floor)
{
	//returns the offset distance from the floor's base altitude the elevator destination is

	ElevatorCar *car = GetCarForFloor(floor);
	if (!car)
		return 0.0;

	return car->GetDestinationOffset(floor);
}

void Elevator::OnInit()
{
	//startup elevator initialization

	//exit if not created properly
	if (Created == false)
		return;

	bool enable_elevators = sbs->GetConfigBool("Skyscraper.SBS.Elevator.IsEnabled", true);

	if (enable_elevators == false)
		Enabled(false);
}

int Elevator::AvailableForCall(bool destination, int floor, int direction, bool report_on_failure)
{
	//determines if the elevator is available for the specified hall call
	//if report_on_failure is true, and verbose mode is enabled, report the reason for call rejection

	//return codes:
	//0 - busy and will eventually be available
	//1 - available for call
	//2 - unavailable due to a service mode or error

	SBS_PROFILE("Elevator::AvailableForCall");

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return 2;

	ElevatorCar *car = GetCarForFloor(floor, report_on_failure);

	//if floor is a serviced floor (valid car found)
	if (car)
	{
		//if direction doesn't go beyond elevator's range
		if ((direction == 1 && floor < car->GetTopFloor()) || (direction == -1 && floor > car->GetBottomFloor()))
		{
			//if elevator is running
			if (IsRunning() == true)
			{
				//and if it's not in any service mode
				if (InServiceMode() == false)
				{
					//and if no queue changes are pending, unless doors are open on the same floor as call
					if (QueuePending == false || ((AreDoorsOpen() == true || AreDoorsOpening() == true) && car->GetFloor() == floor))
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
									//and if the interlock check passes, unless waiting in a peak mode
									if (CheckInterlocks(true) == true || PeakWaiting() == true)
									{
										//and if nudge mode is off on all doors
										if (IsNudgeModeActive() == false)
										{
											//and if it's above the current floor and should be called down, or below the
											//current floor and called up, or on the same floor and not moving, or idle, or proceeding to call floor
											if ((car->GetFloor() > floor && direction == -1) || (car->GetFloor() < floor && direction == 1) || (car->GetFloor() == floor && MoveElevator == false) || IsIdle() || car->RespondingToCall(floor, direction) == true)
											{
												//and if it's either going the same direction as the call, or queue is not active, or idle, or on the same floor with the queues empty
												if (QueuePositionDirection == direction || QueuePositionDirection == 0 || IsIdle() ||
														(destination == true && car->GetFloor() == floor && MoveElevator == false && QueuePositionDirection != 0 && UpQueue.size() == 0 && DownQueue.size() == 0)) //this is an exception if a controller is used
												{
													if (sbs->Verbose)
														Report("Available for call");
													return 1;
												}
												else
												{
													if (sbs->Verbose == true && report_on_failure == true)
														Report("Not available for call - going a different direction and is not idle");
													return 0;
												}
											}
											else
											{
												if (sbs->Verbose == true && report_on_failure == true)
													Report("Not available for call - position/direction wrong for call and is not idle");
												return 0;
											}
										}
										else
										{
											if (sbs->Verbose == true && report_on_failure == true)
												Report("Not available for call - in nudge mode");
											return 0;
										}
									}
									else
									{
										if (sbs->Verbose == true && report_on_failure == true)
											Report("Not available for call - interlock check failed");
										return 2;
									}
								}
								else
								{
									if (sbs->Verbose == true && report_on_failure == true)
										Report("Not available for call - door hold is enabled");
									return 0;
								}
							}
							else
							{
								if (sbs->Verbose == true && report_on_failure == true)
									Report("Not available for call - queueresets is on and opposite queue direction is active");
								return 0;
							}
						}
						else
						{
							if (sbs->Verbose == true && report_on_failure == true)
								Report("Not available for call - limitqueue is on and queue is active");
							return 0;
						}
					}
					else
					{
						if (sbs->Verbose == true && report_on_failure == true)
							Report("Not available for call - queue change is pending");
						return 0;
					}
				}
				else
				{
					if (sbs->Verbose == true && report_on_failure == true)
						Report("Not available for call - in service mode");
					return 2;
				}
			}
			else
			{
				if (sbs->Verbose == true && report_on_failure == true)
					Report("Not available for call - elevator not running");
				return 2;
			}
		}
		else
		{
			if (sbs->Verbose == true && report_on_failure == true)
				Report("Not available for call - direction beyond serviced range");
			return 2;
		}
	}

	if (sbs->Verbose == true && report_on_failure == true)
		Report("Not available for call - not a serviced floor");
	return 2;
}

bool Elevator::SelectFloor(int floor)
{
	//select a floor (in-elevator floor selections)

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

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

	ElevatorCar *car = GetCarForFloor(floor);
	if (!car)
		return ReportError("Floor " + ToString(floor) + " not a serviced floor");

	//SelectFloor won't work if Destination Dispatch is enabled, if set
	if (Controllers.size() > 0)
	{
		DispatchController *controller = sbs->GetController(Controllers[0]);

		if (controller)
		{
			if (controller->DestinationDispatch == true && controller->Hybrid == false && InServiceMode() == false)
				return ReportError("Cannot select floor " + ToString(floor) + ": Hybrid mode is not enabled");
		}
	}

	bool result = false;

	//elevator is above floor
	if (car->GetFloor() > floor)
		result = AddRoute(floor, -1, 0);

	//elevator is below floor
	if (car->GetFloor() < floor)
		result = AddRoute(floor, 1, 0);

	//elevator is on floor
	if (car->GetFloor() == floor)
	{
		if (IsLeveled() == false)
		{
			//relevel elevator if needed
			ReturnToNearestFloor(false, car->Number);
		}
		else if (Direction == 0)
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

					if (ChimeOnArrival == true)
					{
						if (dir == -1)
							car->Chime(0, floor, false);
						else if (dir == 1)
							car->Chime(0, floor, true);
					}
				}
				if (FireServicePhase2 == 0)
					if (AutoDoors == true)
						car->OpenDoors();
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

bool Elevator::Check(Vector3 position)
{
	//check to see if user (camera) is in the elevator

	if (IsEnabled == false)
		return false;

	SBS_PROFILE("Elevator::Check");

	for (size_t i = Cars.size() - 1; i < Cars.size(); --i)
	{
		if (Cars[i]->Check(position) == true)
			return true;
	}

	return false;
}

bool Elevator::ReturnToNearestFloor(bool parking, int car)
{
	//return and relevel to nearest floor

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	if (!GetCar(car))
		return false;

	if (IsIdle() == true && InServiceMode() == false)
	{
		int floor = GetCar(car)->GetNearestServicedFloor();
		Report("returning to nearest floor for car " + ToString(car));
		if (parking == true)
			Parking = true; //enable parking mode to prevent arrival notification

		if (floor >= GetCar(car)->GetFloor())
			AddRoute(floor, 1, 2);
		else
			AddRoute(floor, -1, 2);
		return true;
	}
	return false;
}

bool Elevator::ReturnToBottomFloor()
{
	//return and relevel to bottom floor

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	if (IsIdle() == true && InServiceMode() == false)
	{
		int floor = GetCar(1)->GetBottomFloor();
		Report("returning to bottom floor");
		Parking = true; //enable parking mode to prevent arrival notification

		AddRoute(floor, -1, 2);
		return true;
	}
	return false;
}

bool Elevator::IsLeveled()
{
	//return true if elevator is leveled at a serviced floor

	return GetCar(1)->IsLeveled();
}

bool Elevator::Up()
{
	//move elevator up (manual)
	//this function also stops the up movement when button is depressed

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	if (ManualMove == 0)
	{
		ManualMoveHold = true;
		return Up(true);
	}
	else if (ManualMove == 1 && sbs->camera->MouseDown() == false)
	{
		ManualMoveHold = false;
		return Up(false);
	}
	return false;
}

bool Elevator::Down()
{
	//move elevator down (manual)
	//this function also stops the down movement when button is depressed

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	if (ManualMove == 0)
	{
		ManualMoveHold = true;
		return Down(true);
	}
	else if (ManualMove == -1 && sbs->camera->MouseDown() == false)
	{
		ManualMoveHold = false;
		return Down(false);
	}
	return false;
}

bool Elevator::Up(bool value)
{
	//move elevator up (manual)

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	if (Running == false)
	{
		ReportError("Elevator not running");
		return false;
	}

	if (ManualMove == 0 && value == true && IsMoving == false)
	{
		ManualMove = 1;

		//set direction
		Direction = 1;
		MoveElevator = true;
		if (sbs->Verbose)
			Report("Up: moving elevator");
		return true;
	}
	else if (value == false && ManualMove != 0)
	{
		//stop movement
		ManualMove = 0;
		ManualStop = true;
		Stop();
		return true;
	}
	return false;
}

bool Elevator::Down(bool value)
{
	//move elevator down (manual)

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	if (Running == false)
	{
		ReportError("Elevator not running");
		return false;
	}

	if (ManualMove == 0 && value == true && IsMoving == false)
	{
		ManualMove = -1;

		//set direction
		Direction = -1;
		MoveElevator = true;
		if (sbs->Verbose)
			Report("Up: moving elevator");
		return true;
	}
	else if (value == false && ManualMove != 0)
	{
		//stop movement
		ManualMove = 0;
		ManualStop = true;
		Stop();
		return true;
	}
	return false;
}

Shaft* Elevator::GetShaft()
{
	//get associated shaft object
	return sbs->GetShaft(AssignedShaft);
}

CallStation* Elevator::GetPrimaryCallStation()
{
	//returns the first call station associated with this elevator, on the fire recall (lobby) floor
	Floor *floor = sbs->GetFloor(RecallFloor);
	if (floor)
		return floor->GetCallStationForElevator(Number);
	return 0;
}

int Elevator::GetActiveCallFloor()
{
	return ActiveCall.floor;
}

int Elevator::GetActiveCallDirection()
{
	return ActiveCall.direction;
}

int Elevator::GetActiveCallType()
{
	return ActiveCall.call_type;
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
	for (size_t i = 0; i < Cars.size(); i++)
	{
		Cars[i]->PlayStartingSounds();
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

	//counterweight sound
	if (counterweightsound)
	{
		counterweightsound->Stop();
		if (CounterweightStartSound.empty() == false && CounterweightStartSound != "")
		{
			if (sbs->Verbose)
				Report("playing counterweight start sound");

			counterweightsound->Load(CounterweightStartSound);
			counterweightsound->SetLoopState(false);
			counterweightsound->Play();
		}
	}
}

void Elevator::PlayStoppingSounds(bool emergency)
{
	//play elevator stopping sounds
	//if emergency is true, plays emergency stop sounds with a fallback to standard sounds

	bool motorsound_play = false;
	std::string MotorSoundFile;

	//car sounds
	for (size_t i = 0; i < Cars.size(); i++)
	{
		Cars[i]->PlayStoppingSounds(emergency);
	}

	if (emergency == true)
	{
		//motor sound
		if (MotorEmergencyStopSound.empty() == false && MotorEmergencyStopSound != "")
		{
			if (sbs->Verbose)
				Report("playing motor emergency stop sound");

			MotorSoundFile = MotorEmergencyStopSound;
			motorsound_play = true;
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

	motorsound->Stop();

	if (motorsound_play == true)
	{
		motorsound->Load(MotorSoundFile);
		motorsound->SetLoopState(false);

		//set play position to current percent of the total speed
		if (AutoAdjustSound == true)
		{
			if (ActiveDirection == 1)
				motorsound->SetPlayPosition(1 - (ElevatorRate / UpSpeed));
			else
				motorsound->SetPlayPosition(1 - (ElevatorRate / DownSpeed));
		}
		else
			motorsound->Reset();

		motorsound->Play(false);
	}

	//counterweight sound
	if (counterweightsound)
	{
		counterweightsound->Stop();
		if (CounterweightStopSound.empty() == false && CounterweightStopSound != "")
		{
			if (sbs->Verbose)
				Report("[playing counterweight stop sound");

			counterweightsound->Load(CounterweightStopSound);
			counterweightsound->SetLoopState(false);
			counterweightsound->Play();
		}
	}
}

void Elevator::PlayMovingSounds()
{
	//play elevator movement sounds

	SBS_PROFILE("Elevator::PlayMovingSounds");

	//car sounds
	for (size_t i = 0; i < Cars.size(); i++)
	{
		Cars[i]->PlayMovingSounds();
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

	//counterweight sound
	if (counterweightsound)
	{
		if (counterweightsound->IsPlaying() == false && CounterweightMoveSound.empty() == false && CounterweightMoveSound != "")
		{
			if (sbs->Verbose)
				Report("playing counterweight move sound");
			counterweightsound->Load(CounterweightMoveSound);
			counterweightsound->SetLoopState(true);
			counterweightsound->Play();
		}
	}
}

bool Elevator::PeakWaiting()
{
	//returns true if elevator is waiting in UpPeak or DownPeak mode

	if (GetHoldStatus() == false)
		return false;

	if ((OnTopFloor() && DownPeak == true && IsMoving == false) ||
		(OnBottomFloor() && UpPeak == true && IsMoving == false))
		return true;
	return false;
}

bool Elevator::OnPeakFloor()
{
	//similar to PeakWaiting(), but returns true if elevator is on the related peak mode floor

	if ((OnTopFloor() && DownPeak == true) ||
		(OnBottomFloor() && UpPeak == true))
		return true;
	return false;
}

bool Elevator::OnRecallFloor()
{
	//returns false if not on a recall floor,
	//returns true if on the standard recall floor and normal recall is available
	//returns true if on the alternate recall floor and normal recall is unavailable

	if (RecallUnavailable == false)
		return GetCar(1)->IsOnFloor(RecallFloor, false);

	return GetCar(1)->IsOnFloor(RecallFloorAlternate, false);
}

int Elevator::GetActiveRecallFloor()
{
	//returns floor number of recall floor that's available

	if (RecallUnavailable == true)
		return RecallFloorAlternate;

	return RecallFloor;
}

std::vector<Floor*> Elevator::GetLobbies()
{
	//returns a list of lobbies/skylobbies that service this elevator

	std::vector<Floor*> list;

	for (size_t i = 0; i < Cars.size(); i++)
	{
		for (size_t j = 0; j < Cars[i]->GetServicedFloorCount(); j++)
		{
			int num = Cars[i]->GetServicedFloor(j);

			Floor *floor = sbs->GetFloor(num);
			if (floor)
			{
				std::string type = SetCaseCopy(floor->FloorType, false);
				if (type == "lobby" || type == "skylobby")
					list.emplace_back(floor);
			}
		}
	}

	if (list.size() == 0 && sbs->GetFloor(RecallFloor))
		list.emplace_back(sbs->GetFloor(RecallFloor));

	return list;
}

bool Elevator::IsStopped()
{
	//true if elevator has stopped without reaching a floor, usually due to an emergency stop

	return (IsMoving == false && OnFloor == false && FinishedMove == true);
}

void Elevator::CancelHallCall(int floor, int direction)
{
	//delete a route if the route is a hall call response, and no floor button has been pressed

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return;

	for (size_t i = 0; i < Cars.size(); i++)
	{
		Control *control = Cars[i]->GetFloorButton(floor);

		if (control)
		{
			//exit if a floor button has been pressed
			if (control->GetLightStatus() == true)
				return;
		}
	}

	DeleteRoute(floor, direction);
}

void Elevator::HandleDequeue(int direction, bool stop_if_empty)
{
	//handle elevator behavior on dequeue
	//if stop_if_empty is true, this will stop the elevator if the related queue is empty

	if (stop_if_empty == true && MoveElevator == true && EmergencyStop == 0)
	{
		if ((direction == 1 && UpQueue.empty()) ||
				(direction == -1 && DownQueue.empty()))
			Stop();
	}

	//reset active call status if queues are empty
	if (DownQueue.empty() == true && UpQueue.empty() == true)
	{
		ActiveCall.floor = 0;
		ActiveCall.direction = 0;
		ActiveCall.call_type = 0;
		ActiveCall.car = 0;
	}
}

bool Elevator::IsManuallyStopped()
{
	//this will return true if elevator is stopped within 18 inches of the nearest landing

	return (InServiceMode() == false && ManualStop == true && std::abs(GetCar(1)->GetDestinationAltitude(GetCar(1)->GetFloor()) - GetCar(1)->GetPosition().y) < 1.5);
}

bool Elevator::CheckInterlocks(bool skip_current_floor)
{
	//return true if interlock checks pass, or interlocks are disabled

	bool status;

	if (skip_current_floor == false)
		status = (Interlocks == true && (AreDoorsOpen() == true || AreShaftDoorsClosed() == false || DoorsStopped() == true));
	else
		status = (Interlocks == true && (AreShaftDoorsClosed(true) == false || DoorsStopped() == true));

	return !status;
}

ElevatorCar* Elevator::AddCar()
{
	//create a new elevator car object

	int number = (int)Cars.size() + 1;
	ElevatorCar *car = new ElevatorCar(this, number);
	Cars.emplace_back(car);
	return car;
}

ElevatorCar* Elevator::GetCar(int number)
{
	if (number < 0 || number >(int)Cars.size())
		return 0;

	if (number == 0)
		number = 1;

	return Cars[number - 1];
}

int Elevator::GetCarCount()
{
	return (int)Cars.size();
}

void Elevator::RemoveCar(ElevatorCar* car)
{
	//remove a car reference (does not delete the object itself)
	for (size_t i = 0; i < Cars.size(); i++)
	{
		if (Cars[i] == car)
		{
			Cars.erase(Cars.begin() + i);
			return;
		}
	}
}

ElevatorCar* Elevator::GetCarForFloor(int number, bool report_on_failure)
{
	//return car that services specified floor

	for (size_t i = 0; i < Cars.size(); i++)
	{
		if (Cars[i]->IsServicedFloor(number, report_on_failure) == true)
			return Cars[i];
	}

	return 0;
}

bool Elevator::AreDoorsOpen()
{
	//returns true if any door is open

	for (size_t i = 0; i < Cars.size(); i++)
	{
		if (Cars[i]->AreDoorsOpen() == true)
			return true;
	}
	return false;
}

int Elevator::AreDoorsMoving(bool car_doors, bool shaft_doors)
{
	//returns status if any door is moving

	for (size_t i = 0; i < Cars.size(); i++)
	{
		int result = Cars[i]->AreDoorsMoving(0, car_doors, shaft_doors);
		if (result != 0)
			return result;
	}
	return 0;
}

bool Elevator::AreShaftDoorsClosed(bool skip_current_floor)
{
	//returns true if all shaft doors are closed and not moving

	for (size_t i = 0; i < Cars.size(); i++)
	{
		if (Cars[i]->AreShaftDoorsClosed(skip_current_floor) == false)
			return false;
	}
	return true;
}

bool Elevator::DoorsStopped()
{
	//returns true if any of the doors are stopped

	for (size_t i = 0; i < Cars.size(); i++)
	{
		if (Cars[i]->DoorsStopped() == true)
			return true;
	}
	return false;
}

bool Elevator::AreDoorsOpening(bool car_doors, bool shaft_doors)
{
	//returns true if doors are opening

	for (size_t i = 0; i < Cars.size(); i++)
	{
		if (Cars[i]->AreDoorsOpening(0, car_doors, shaft_doors) == true)
			return true;
	}
	return false;
}

bool Elevator::AreDoorsClosing(bool car_doors, bool shaft_doors)
{
	//returns true if doors are closing

	for (size_t i = 0; i < Cars.size(); i++)
	{
		if (Cars[i]->AreDoorsClosing(0, car_doors, shaft_doors) == true)
			return true;
	}
	return false;
}

bool Elevator::IsServicedFloor(int floor, bool report)
{
	//returns true if floor is in any of the serviced floor lists, otherwise false

	ElevatorCar* car = GetCarForFloor(floor);

	bool result = false;

	if (car)
		result = true;

	if (sbs->Verbose && report == true)
	{
		if (result == false)
			Report("Floor " + ToString(floor) + " is not a serviced floor");
		else
			Report("Floor " + ToString(floor) + " is a serviced floor");
	}

	return result;
}

void Elevator::StopSounds()
{
	//stop movement sounds

	for (size_t i = 0; i < Cars.size(); i++)
	{
		Cars[i]->StopCarSound();
	}
	motorsound->Stop();
}

void Elevator::ChangeLight(int floor, bool value)
{
	//turn on or off specified button lights

	for (size_t i = 0; i < Cars.size(); i++)
	{
		Cars[i]->ChangeLight(floor, value);
	}
}

void Elevator::ResetLights()
{
	//turn off all button lights

	for (size_t i = 0; i < Cars.size(); i++)
	{
		Cars[i]->ResetLights();
	}
}

void Elevator::OpenDoors()
{
	//open all doors

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return;

	for (size_t i = 0; i < Cars.size(); i++)
	{
		Cars[i]->OpenDoors();
	}
}

void Elevator::CloseDoors()
{
	//close all doors

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return;

	for (size_t i = 0; i < Cars.size(); i++)
	{
		Cars[i]->CloseDoors();
	}
}

bool Elevator::GetHoldStatus()
{
	//checks doors and returns true if any have door hold enabled

	for (size_t i = 0; i < Cars.size(); i++)
	{
		if (Cars[i]->GetHoldStatus() == true)
			return true;
	}
	return false;
}

bool Elevator::OnTopFloor()
{
	return GetCar(1)->OnTopFloor();
}

bool Elevator::OnBottomFloor()
{
	return GetCar(1)->OnBottomFloor();
}

bool Elevator::IsNudgeModeActive()
{
	//returns true if nudge mode is active in any car

	for (size_t i = 0; i < Cars.size(); i++)
	{
		if (Cars[i]->IsNudgeModeActive() == true)
			return true;
	}
	return false;
}

void Elevator::ResetShaftDoors(int floor)
{
	//reset shaft doors

	//this might not be needed, due to addition of full-shaft enable check to
	//floor object's EnableGroup function, needs testing

	for (int i = 1; i <= sbs->GetShaftCount(); i++)
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

void Elevator::ResetDoors()
{
	//reset all doors
	for (size_t i = 0; i < Cars.size(); i++)
	{
		Cars[i]->ResetDoors();
	}
}

void Elevator::ResetNudgeTimers(bool start)
{
	//reset all nudge timers
	for (size_t i = 0; i < Cars.size(); i++)
	{
		Cars[i]->ResetNudgeTimer(start);
	}
}

void Elevator::HoldDoors()
{
	//hold all doors

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return;

	for (size_t i = 0; i < Cars.size(); i++)
	{
		Cars[i]->HoldDoors();
	}
}

void Elevator::DirectionalIndicatorsOff()
{
	//switch off all directional indicators

	for (size_t i = 0; i < Cars.size(); i++)
	{
		Cars[i]->SetDirectionalIndicators(Cars[i]->CurrentFloor, false, false);
	}
}

void Elevator::EnableNudgeMode(bool value)
{
	//enable nudge mode on all cars

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return;

	for (size_t i = 0; i < Cars.size(); i++)
	{
		Cars[i]->EnableNudgeMode(value);
	}
}

Real Elevator::GetCarOffset(int number)
{
	//get vertical offset of specified car

	if (!GetCar(GotoFloorCar))
		return 0.0;

	return GetCar(GotoFloorCar)->GetPosition().y - GetPosition().y;
}

int Elevator::GetFloorForCar(int car, int number)
{
	if (!GetCar(car))
		return 0;

	ElevatorCar *obj = GetCarForFloor(number);
	if (!obj)
		return 0;

	//subtract car's offset to get base car's value
	number -= obj->Offset;

	return number += GetCar(car)->Offset;
}

void Elevator::ProcessGotoFloor(int floor, int direction)
{
	//set GotoFloor status on other cars, if queue entries exists for those cars

	if (MoveElevator == false)
		return;

	for (int i = 1; i <= GetCarCount(); i++)
	{
		if (i == GotoFloorCar)
			continue;

		if (IsQueued(GetFloorForCar(i, floor), direction) == true)
			GetCar(i)->GotoFloor = true;
	}
}

bool Elevator::OnParkingFloor()
{
	//returns true if the elevator is on the parking floor

	ElevatorCar *car = GetCarForFloor(ParkingFloor);
	if (!car)
		return false;

	return (car->GetFloor() == ParkingFloor);
}

bool Elevator::SetHoistwayAccess(int floor, int access)
{
	//sets the hoistway access direction for the specified floor, for Inspection Service mode
	//access is -1 for Down, 0 for Off, and 1 for Up

	//with a direction enabled and held, this moves the elevator
	//at leveling speed, in that direction while the shaft doors are open

	if (Running == false)
		return ReportError("SetHoistwayAccess: Elevator not running");

	if (InspectionService == false)
		return ReportError("SetHoistwayAccess: Not in inspection service mode");

	if (access == 0 || (HoistwayAccessHold == true && HoistwayAccess != 0 && sbs->camera->MouseDown() == false))
	{
		//disable mode
		HoistwayAccess = 0;
		HoistwayAccessFloor = 0;
		if (IsMoving == true)
			Stop();

		Report("Hoistway Access set to Off for floor " + ToString(floor));
		return true;
	}
	else if (HoistwayAccess == 0)
	{
		if (IsMoving == true)
		{
			if (sbs->Verbose)
				ReportError("SetHoistwayAccess: already moving");
			return false;
		}

		ElevatorCar *car = GetCarForFloor(floor);
		if (!car)
			return ReportError("SetHoistwayAccess: no car found that services floor " + ToString(floor));

		if (car->AreShaftDoorsOpen(0, floor) == false)
			return ReportError("Shaft doors need to be open on floor " + ToString(floor) + " to enable Hoistway Access");

		//enable mode
		HoistwayAccess = access;
		HoistwayAccessFloor = floor;

		std::string direction = "Down";
		if (HoistwayAccess == 1)
			direction = "Up";
		Report("Hoistway Access set to " + direction + " for floor " + ToString(floor));

		//set direction
		Direction = HoistwayAccess;
		MoveElevator = true;

		return true;
	}

	return false;
}

void Elevator::SetControls(const std::string &action_name)
{
	//queue a setcontrols action

	ControlQueue.push(action_name);
}

void Elevator::DoSetControls()
{
	//process existing setcontrols actions

	while (ControlQueue.empty() == false)
	{
		std::string action_name = ControlQueue.front();

		for (size_t i = 0; i < Cars.size(); i++)
		{
			Cars[i]->SetControls(action_name);
		}

		ControlQueue.pop();
	}
}

bool Elevator::GetDestinationDispatch()
{
	//return true if Destination Dispatch is enabled

	if (Controllers.size() > 0)
	{
		if (sbs->GetController(Controllers[0]))
			return sbs->GetController(Controllers[0])->DestinationDispatch;
	}
	return false;
}

void Elevator::SameFloorArrival(int floor, int direction)
{
	if (sbs->Verbose)
		Report("Elevator active on current floor - opening");

	//get related car number
	ElevatorCar *car = GetCarForFloor(floor);

	if (!car)
	{
		ReportError("floor " + ToString(floor) + " is not a serviced floor");
		return;
	}

	for (size_t i = 0; i < Controllers.size(); i++)
	{
		DispatchController *controller = sbs->GetController(Controllers[i]);

		//notify controllers of movement
		if (controller)
			controller->ResetArrival(Number);

		//update arrival information
		if (direction == -1)
		{
			if (controller)
				controller->ElevatorArrived(Number, floor, false);
		}
		else
		{
			if (controller)
				controller->ElevatorArrived(Number, floor, true);
		}
	}

	//notify on arrival
	if (NotifyEarly >= 0)
		car->NotifyArrival(floor, false, direction);

	//store call direction for NotifyLate feature
	if (NotifyLate == true)
		car->LateDirection = direction;

	//open elevator if it's on the same floor
	car->OpenDoors();
}

void Elevator::AddController(int controller)
{
	//add a controller to this elevator

	if (!sbs->GetController(controller))
		return;

	//exit if already in table
	for (size_t i = 0; i < Controllers.size(); i++)
	{
		if (Controllers[i] == controller)
			return;
	}

	Controllers.emplace_back(controller);
}

void Elevator::RemoveController(int controller)
{
	//remove a controller from this elevator

	for (size_t i = 0; i < Controllers.size(); i++)
	{
		if (Controllers[i] == controller)
		{
			Controllers.erase(Controllers.begin() + i);
			return;
		}
	}
}

bool Elevator::GetCallStatus(int floor, bool &up, bool &down)
{
	//returns call status for the specified floor

	for (size_t i = 0; i < Controllers.size(); i++)
	{
		bool result = false;
		if (sbs->GetController(Controllers[i]))
			result = sbs->GetController(Controllers[i])->GetCallStatus(Number, floor, up, down);

		if (result == true)
			return result;
	}
	return false;
}

int Elevator::GetMotorRoom()
{
	return sbs->GetFloorNumber(MotorPosition.y);
}

void Elevator::EnableMalfunctions(bool value)
{
	//enable random malfunctions on this elevator

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return;

	if (!malfunction_timer)
		return;

	if (value == true)
	{
		Report("Enabling malfunctions");
		malfunction_timer->Start(int(RandomFrequency) * 1000, false);
	}
	else
	{
		Report("Disabling malfunctions");
		malfunction_timer->Stop();
	}
}

void Elevator::Malfunction()
{
	//elevator malfunction

	Report("Malfunction");

	size_t type = rnd_type->Get(2);
	if (type == 0)
	{
		//full malfunction
		Stop(true);
		SetRunState(false);
		ResetQueue(true, true);

		for (size_t i = 0; i < Cars.size(); i++)
		{
			if (Cars[i])
			{
				//turn off car fans
				Cars[i]->Fan = false;
			}
		}
	}
	else if (type == 1)
	{
		//partial malfunction (stop)
		Stop(true);
	}
	else if (type == 2)
	{
		//partial malfunction (reset queues)
		ResetQueue(true, true);
	}
}

}
