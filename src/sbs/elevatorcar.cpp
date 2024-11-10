/*
	Scalable Building Simulator - Elevator Car Object
	The Skyscraper Project - Version 2.1
	Copyright (C)2004-2024 Ryan Thoryk
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
#include "mesh.h"
#include "polymesh.h"
#include "primitive.h"
#include "custom.h"
#include "floor.h"
#include "elevator.h"
#include "elevatordoor.h"
#include "control.h"
#include "trigger.h"
#include "sound.h"
#include "model.h"
#include "light.h"
#include "door.h"
#include "buttonpanel.h"
#include "directional.h"
#include "floorindicator.h"
#include "shaft.h"
#include "camera.h"
#include "dynamicmesh.h"
#include "texture.h"
#include "profiler.h"
#include "cameratexture.h"
#include "elevatorcar.h"
#include "manager.h"
#include "indicator.h"
#include "timer.h"
#include "controller.h"
#include "reverb.h"

#include <time.h>

namespace SBS {

	class ElevatorCar::KeypadTimer : public TimerObject
	{
	public:
		ElevatorCar* parent;
		KeypadTimer(const std::string& name, ElevatorCar* parent) : TimerObject(parent, name)
		{
			this->parent = parent;
		}
		virtual void Notify();
	};


ElevatorCar::ElevatorCar(Elevator *parent, int number) : Object(parent)
{
	//set up SBS object
	SetValues("ElevatorCar", "", false);

	//init variables
	Name = "";
	this->parent = parent;
	Number = number;
	carsound = 0;
	idlesound = 0;
	alarm = 0;
	floorbeep = 0;
	doorhold_direction = 0;
	doorhold_whichdoors = 0;
	doorhold_floor = 0;
	doorhold_manual = 0;
	announcesnd = 0;
	musicsound = 0;
	DirMessageSound = false;
	DoorMessageSound = false;
	Fan = true;
	MusicPosition = Vector3::ZERO;
	Height = 0;
	HeightSet = false;
	lastdoor_result = 0;
	lastdoor_number = 0;
	NumDoors = 1;
	ControlPressActive = false;
	IsEnabled = true;
	UpStartSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.Car.UpStartSound", "");
	DownStartSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.Car.DownStartSound", "");
	UpMoveSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.Car.UpMoveSound", "");
	DownMoveSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.Car.DownMoveSound", "");
	UpStopSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.Car.UpStopSound", "");
	DownStopSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.Car.DownStopSound", "");
	IdleSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.Car.IdleSound", "elevidle.wav");
	AlarmSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.Car.AlarmSound", "bell1.wav");
	AlarmSoundStop = sbs->GetConfigString("Skyscraper.SBS.Elevator.Car.AlarmSoundStop", "bell1-stop.wav");
	EmergencyStopSound = sbs->GetConfigString("Skyscraper.SBS.Elevator.Car.EmergencyStopSound", "");
	AlarmActive = false;
	UseFloorBeeps = false;
	UseFloorSounds = false;
	UseDirMessageSounds = false;
	UseDoorMessageSounds = false;
	MusicUp = sbs->GetConfigString("Skyscraper.SBS.Elevator.Car.MusicUp", "");
	MusicDown = sbs->GetConfigString("Skyscraper.SBS.Elevator.Car.MusicDown", "");
	MusicOn = sbs->GetConfigBool("Skyscraper.SBS.Elevator.Car.MusicOn", true);
	MusicOnMove = sbs->GetConfigBool("Skyscraper.SBS.Elevator.Car.MusicOnMove", false);
	MusicAlwaysOn = sbs->GetConfigBool("Skyscraper.SBS.Elevator.Car.MusicAlwaysOn", false);
	AutoEnable = sbs->GetConfigBool("Skyscraper.SBS.Elevator.Car.AutoEnable", true);
	CameraOffset = 0;
	FirstRun = true;
	lastcheckresult = false;
	checkfirstrun = true;
	lastposition = Vector3::ZERO;
	lastfloor = 0;
	lastfloorset = false;
	StartingFloor = 0;
	CurrentFloor = 0;
	Created = false;
	Offset = 0;
	GotoFloor = false;
	LateDirection = 0;
	last_music_direction = 0;
	MessageOnMove = false;
	MessageOnStart = false;
	MessageOnClose = false;
	indicator = 0;
	TimerDelay = 2;
	reverb = 0;

	std::string name = parent->GetName() + ":Car " + ToString(number);
	SetName(name);

	Mesh = new MeshObject(this, name);

	if (sbs->Verbose)
		parent->Report("created car " + ToString(number));

	keypad_timer = new KeypadTimer("Input Timeout Timer", this);
}

ElevatorCar::~ElevatorCar()
{
	if (sbs->Verbose)
		parent->Report("deleting objects");

	//delete indicator
	if (indicator)
		delete indicator;
	indicator = 0;

	//delete controls
	for (size_t i = 0; i < ControlArray.size(); i++)
	{
		if (ControlArray[i])
		{
			ControlArray[i]->parent_deleting = true;
			delete ControlArray[i];
		}
		ControlArray[i] = 0;
	}

	//delete triggers
	for (size_t i = 0; i < TriggerArray.size(); i++)
	{
		if (TriggerArray[i])
		{
			TriggerArray[i]->parent_deleting = true;
			delete TriggerArray[i];
		}
		TriggerArray[i] = 0;
	}

	//delete models
	for (size_t i = 0; i < ModelArray.size(); i++)
	{
		if (ModelArray[i])
		{
			ModelArray[i]->parent_deleting = true;
			delete ModelArray[i];
		}
		ModelArray[i] = 0;
	}

	//delete primitives
	for (size_t i = 0; i < PrimArray.size(); i++)
	{
		if (PrimArray[i])
		{
			PrimArray[i]->parent_deleting = true;
			delete PrimArray[i];
		}
		PrimArray[i] = 0;
	}

	//delete custom objects
	for (size_t i = 0; i < CustomObjectArray.size(); i++)
	{
		if (CustomObjectArray[i])
		{
			CustomObjectArray[i]->parent_deleting = true;
			delete CustomObjectArray[i];
		}
		CustomObjectArray[i] = 0;
	}

	//delete lights
	for (size_t i = 0; i < lights.size(); i++)
	{
		if (lights[i])
		{
			lights[i]->parent_deleting = true;
			delete lights[i];
		}
		lights[i] = 0;
	}

	//delete directional indicators
	if (sbs->Verbose)
		parent->Report("deleting interior directional indicators");

	for (size_t i = 0; i < DirIndicatorArray.size(); i++)
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
		parent->Report("deleting doors");

	if (DoorArray.size() > 0)
	{
		for (size_t i = 0; i < DoorArray.size(); i++)
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
		parent->Report("deleting floor indicators");

	for (size_t i = 0; i < FloorIndicatorArray.size(); i++)
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
		parent->Report("deleting button panels");

	for (size_t i = 0; i < PanelArray.size(); i++)
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
		parent->Report("deleting standard doors");

	for (size_t i = 0; i < StdDoorArray.size(); i++)
	{
		if (StdDoorArray[i])
		{
			StdDoorArray[i]->parent_deleting = true;
			delete StdDoorArray[i];
		}
		StdDoorArray[i] = 0;
	}

	//delete camera textures
	for (size_t i = 0; i < CameraTextureArray.size(); i++)
	{
		if (CameraTextureArray[i])
		{
			CameraTextureArray[i]->parent_deleting = true;
			delete CameraTextureArray[i];
		}
		CameraTextureArray[i] = 0;
	}

	if (sbs->Verbose)
		parent->Report("deleting sounds");
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

	if (idlesound)
	{
		idlesound->parent_deleting = true;
		delete idlesound;
	}
	idlesound = 0;
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

	for (size_t i = 0; i < sounds.size(); i++)
	{
		if (sounds[i])
		{
			sounds[i]->parent_deleting = true;
			delete sounds[i];
		}
		sounds[i] = 0;
	}

	if (reverb)
		delete reverb;
	reverb = 0;

	//delete mesh object
	if (Mesh)
	{
		Mesh->parent_deleting = true;
		delete Mesh;
	}
	Mesh = 0;

	if (sbs->FastDelete == false)
	{
		//unregister from parent
		if (parent_deleting == false)
			parent->RemoveCar(this);
	}
}

bool ElevatorCar::CreateCar(int floor)
{
	//used with CreateElevator function; this is the car-specific code

	if (Created == true)
		return ReportError("Has already been created");

	if (parent->Created == false)
		return ReportError("Elevator has not been created yet");

	StartingFloor = floor;

	if (Number > 1)
	{
		Offset = floor - parent->GetCar(Number - 1)->StartingFloor;

		//make sure starting floor is above previous car
		if (StartingFloor < parent->GetCar(Number - 1)->StartingFloor)
			return ReportError("Car " + ToString(Number) + " must be above car " + ToString(Number - 1));
	}

	//check if starting floor is valid
	if (!sbs->GetFloor(StartingFloor))
		return ReportError("Floor " + ToString(StartingFloor) + " doesn't exist");

	//make sure starting floor is within associated shaft's range
	if (StartingFloor < parent->GetShaft()->startfloor || StartingFloor > parent->GetShaft()->endfloor)
		return ReportError("Invalid starting floor " + ToString(StartingFloor));

	//check door count
	if (NumDoors < 0)
		return ReportError("Number of doors invalid");

	//add car's starting floor to serviced floor list
	//this also ensures that the list is populated to prevent errors
	if (IsServicedFloor(StartingFloor) == false)
		AddServicedFloor(StartingFloor);

	//ensure that serviced floors are valid for the shaft
	if (CheckServicedFloors() == false)
		return false;

	//set starting position
	if (Number == 1)
		parent->SetPositionY(sbs->GetFloor(StartingFloor)->GetBase()); //set elevator's position if first car
	else
		SetPositionY(sbs->GetFloor(StartingFloor)->GetBase()); //set car's offset if not first car

	//create door objects
	if (sbs->Verbose)
		Report("creating doors");
	if (NumDoors > 0)
	{
		for (int i = 1; i <= NumDoors; i++)
			DoorArray.emplace_back(new ElevatorDoor(i, this));
	}

	//create sound objects
	if (sbs->Verbose)
		Report("creating sound objects");
	carsound = new Sound(this, "Car", true);
	idlesound = new Sound(this, "Idle", true);
	alarm = new Sound(this, "Alarm", true);
	floorbeep = new Sound(this, "Floor Beep", true);
	announcesnd = new Sound(this, "Announcement Sound", true);
	musicsound = new Sound(this, "Music Sound", true);
	musicsound->Move(MusicPosition);

	//set current floor
	CurrentFloor = StartingFloor;

	Created = true;

	Report("created on floor " + ToString(StartingFloor));
	return true;
}

Elevator* ElevatorCar::GetElevator()
{
	return parent;
}

void ElevatorCar::Report(const std::string &message)
{
	//general reporting function

	if (parent->GetCarCount() > 1)
		parent->Report("Car " + ToString(Number) + ": " + message);
	else
		parent->Report(message);
}

bool ElevatorCar::ReportError(const std::string &message)
{
	//general reporting function

	if (parent->GetCarCount() > 1)
		return parent->ReportError("Car " + ToString(Number) + ": " + message);

	return parent->ReportError(message);
}

Wall* ElevatorCar::AddWall(const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real height1, Real height2, Real voffset1, Real voffset2, Real tw, Real th)
{
	//Adds a wall with the specified dimensions

	Wall *wall = Mesh->CreateWallObject(name);
	sbs->AddWallMain(wall, name, texture, thickness, x1, z1, x2, z2, height1, height2, voffset1, voffset2, tw, th, true);
	return wall;
}

Wall* ElevatorCar::AddFloor(const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real voffset1, Real voffset2, bool reverse_axis, bool texture_direction, Real tw, Real th, bool legacy_behavior)
{
	//Adds a floor with the specified dimensions and vertical offset

	Wall *wall = Mesh->CreateWallObject(name);
	sbs->AddFloorMain(wall, name, texture, thickness, x1, z1, x2, z2, voffset1, voffset2, reverse_axis, texture_direction, tw, th, true, legacy_behavior);
	return wall;
}

FloorIndicator* ElevatorCar::AddFloorIndicator(const std::string &texture_prefix, const std::string &blank_texture, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset)
{
	//Creates a floor indicator at the specified location

	FloorIndicator* indicator = new FloorIndicator(this, parent->Number, Number, texture_prefix, blank_texture, direction, CenterX, CenterZ, width, height, voffset);
	FloorIndicatorArray.emplace_back(indicator);
	return indicator;
}

ButtonPanel* ElevatorCar::CreateButtonPanel(const std::string &texture, int rows, int columns, const std::string &direction, Real CenterX, Real CenterZ, Real buttonwidth, Real buttonheight, Real spacingX, Real spacingY, Real voffset, Real tw, Real th)
{
	//create a new button panel object and store the pointer

	int index = (int)PanelArray.size();

	if (sbs->Verbose)
		Report("creating button panel " + ToString(index + 1));

	ButtonPanel* panel = new ButtonPanel(this, index + 1, texture, rows, columns, direction, CenterX, CenterZ, buttonwidth, buttonheight, spacingX, spacingY, voffset, tw, th);
	PanelArray.emplace_back(panel);
	return panel;
}

void ElevatorCar::DumpServicedFloors()
{
	//dump serviced floors list

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return;

	if (parent->GetCarCount() == 1)
		Object::Report("\n--- Elevator " + ToString(parent->Number) + "'s Serviced Floors ---\n");
	else
		Object::Report("\n--- Elevator " + ToString(parent->Number) + " Car " + ToString(Number) + "'s Serviced Floors ---\n");

	for (size_t i = 0; i < ServicedFloors.size(); i++)
		Object::Report(ToString((int)i) + " - " + ToString(ServicedFloors[i]));
	Object::Report("");
}

bool ElevatorCar::AddServicedFloor(int number, bool create_shaft_door)
{
	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	if (sbs->Verbose)
		Report("adding serviced floor " + ToString(number));

	//check if floor is outside valid floor range
	if (sbs->IsValidFloor(number) == false)
		return ReportError("AddServicedFloor: Invalid floor " + ToString(number));

	if (Created == true)
	{
		if (CheckServicedFloors() == false)
			return false;
	}

	if (IsServicedFloor(number) == false)
	{
		//only add floor if not serviced by another car
		if (parent->IsServicedFloor(number) == false)
		{
			ServicedFloors.emplace_back(number);
			std::sort(ServicedFloors.begin(), ServicedFloors.end());

			//add serviced floors to doors, if needed
			if (Created == true && create_shaft_door == true)
			{
				for (size_t i = 0; i < DoorArray.size(); i++)
				{
					DoorArray[i]->AddServicedFloor(number);
				}
			}
		}
		else
			return ReportError("AddServicedFloor: Floor " + ToString(number) + " already serviced");
	}

	return true;
}

void ElevatorCar::RemoveServicedFloor(int number, bool remove_shaft_door)
{
	//only run if power is enabled
	if (sbs->GetPower() == false)
		return;

	if (sbs->Verbose)
		Report("removing serviced floor " + ToString(number));

	int index = GetFloorIndex(number);
	if (index > -1)
		ServicedFloors.erase(ServicedFloors.begin() + index);

	//remove serviced floors from doors
	if (Created == true && remove_shaft_door == true)
	{
		for (size_t i = 0; i < DoorArray.size(); i++)
		{
			DoorArray[i]->RemoveServicedFloor(number);
		}
	}
}

bool ElevatorCar::IsServicedFloor(int floor, bool report)
{
	//returns true if floor is in serviced floor list, otherwise false

	int index = GetFloorIndex(floor);
	if (index == -1)
	{
		if (sbs->Verbose && report == true)
			Report("Floor " + ToString(floor) + " is not a serviced floor");
		return false;
	}
	else
	{
		if (sbs->Verbose && report == true)
			Report("Floor " + ToString(floor) + " is a serviced floor");
		return true;
	}
}

int ElevatorCar::GetFloorIndex(int floor)
{
	//return array index of the specified floor

	for (size_t i = 0; i < ServicedFloors.size(); i++)
	{
		if (ServicedFloors[i] == floor)
			return (int)i;
	}

	return -1;
}

int ElevatorCar::GetServicedFloorCount()
{
	//return number of serviced floors
	return (int)ServicedFloors.size();
}

int ElevatorCar::GetServicedFloor(int index)
{
	//get a specific serviced floor
	if (index >= 0 && index < (int)ServicedFloors.size())
		return ServicedFloors[index];
	return 0;
}

int ElevatorCar::GetServicedFloorIndex(int floor)
{
	//get the index of the specified serviced floor

	for (size_t i = 0; i < ServicedFloors.size(); i++)
	{
		if (ServicedFloors[i] == floor)
			return i;
	}
	return -1;
}

bool ElevatorCar::CheckServicedFloors()
{
	//ensure serviced floors are valid for the shaft

	for (size_t i = 0; i < ServicedFloors.size(); i++)
	{
		if (parent->GetShaft()->IsValidFloor(ServicedFloors[i]) == false)
		{
			std::string snum = ToString(parent->AssignedShaft);
			std::string num = ToString(ServicedFloors[i]);
			return ReportError("Floor " + num + " not valid for shaft " + snum);
		}
	}
	return true;
}

void ElevatorCar::Alarm()
{
	//elevator alarm code

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return;

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
	else if (AlarmActive == true && sbs->camera->MouseDown() == false)
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

void ElevatorCar::OpenHatch()
{
	//Opens the elevator's upper escape hatch, allowing access to the shaft

	Report("opening hatch");
}

void ElevatorCar::OnInit()
{
	//startup car initialization

	//exit if not created properly
	if (Created == false)
		return;

	//disable objects
	EnableObjects(false);

	//move car (and possibly elevator) to starting position
	SetFloor(StartingFloor, (Number == 1));
}

void ElevatorCar::Loop()
{
	//elevator car monitor loop

	SBS_PROFILE("ElevatorCar::Loop");

	//only run if power is enabled
	if (sbs->GetPower() == false)
	{
		idlesound->Stop();
		musicsound->Stop();
	}

	ControlPressActive = false;

	//perform first-run tasks
	if (FirstRun == true)
	{
		FirstRun = false;

		if (parent->OpenOnStart == true)
			OpenDoors();
	}

	//play car idle sound if in elevator, or if doors open
	if (IdleSound != "" && sbs->GetPower() == true)
	{
		if (idlesound->IsPlaying() == false && Fan == true)
		{
			if (InCar() == true || AreDoorsOpen() == true || AreDoorsMoving(0, true, false) != 0)
			{
				if (sbs->Verbose)
					Report("playing car idle sound");

				if (idlesound->IsLoaded() == false)
					idlesound->Load(IdleSound);

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
			else if (InCar() == false && AreDoorsOpen() == false && AreDoorsMoving() == 0)
			{
				if (sbs->Verbose)
					Report("stopping car idle sound");
				idlesound->Stop();
			}
		}
	}

	//music processing
	if ((MusicUp != "" || MusicDown != "") && sbs->GetPower() == true)
	{
		if (MusicAlwaysOn == false) //standard mode
		{
			//play music sound if in elevator, or if doors open
			if (musicsound->IsPlaying() == false && MusicOn == true && ((MusicOnMove == true && parent->IsMoving == true) || MusicOnMove == false))
			{
				if (parent->InServiceMode() == false)
				{
					if (InCar() == true || AreDoorsOpen() == true || AreDoorsMoving() != 0)
					{
						if (sbs->Verbose)
							Report("playing music");

						int direction = parent->ActiveDirection;

						//load music if not already loaded, or if music direction has changed
						if (musicsound->IsLoaded() == false || (last_music_direction != direction && MusicOn == true))
						{
							if (direction == 0 && last_music_direction != 0)
								direction = last_music_direction;

							if (direction >= 0)
								musicsound->Load(MusicUp);
							else
								musicsound->Load(MusicDown);

							last_music_direction = direction;
						}

						musicsound->SetLoopState(true);
						musicsound->Play(false);
					}
				}
			}
			else
			{
				if ((MusicOn == false || parent->InServiceMode() == true || (MusicOnMove == true && parent->IsMoving == false)) && musicsound->IsPlaying() == true)
				{
					if (sbs->Verbose)
						Report("stopping music");
					musicsound->Pause();
				}
				else if (InCar() == false && AreDoorsOpen() == false && AreDoorsMoving() == 0)
				{
					//turn off music if outside elevator car and doors are closed
					if (sbs->Verbose)
						Report("stopping music");
					musicsound->Pause();
				}
				else if (MusicOn == true && MusicOnMove == false && musicsound->IsPlaying() == true)
				{
					//stop music if elevator changes direction while music is on
					if (last_music_direction != parent->ActiveDirection && parent->ActiveDirection != 0)
						musicsound->Pause();
				}
			}
		}
		else if (MusicAlwaysOn == true) //always-on mode
		{
			if (musicsound->IsPlaying() == false && MusicOn == true && MusicOnMove == false)
			{
				//enable music if in always-on mode

				if (parent->InServiceMode() == false)
				{
					if (sbs->Verbose)
						Report("playing music");

					int direction = parent->ActiveDirection;

					//load music if not already loaded, or if music direction has changed
					if (musicsound->IsLoaded() == false || (last_music_direction != direction && MusicOn == true))
					{
						if (direction == 0 && last_music_direction != 0)
							direction = last_music_direction;

						if (direction >= 0)
							musicsound->Load(MusicUp);
						else
							musicsound->Load(MusicDown);

						last_music_direction = direction;
					}

					musicsound->SetLoopState(true);
					musicsound->SetVolume(0.0);
					musicsound->Play(false);
				}
			}
			else if (MusicOn == true && MusicOnMove == false && musicsound->IsPlaying() == true)
			{
				if (last_music_direction != parent->ActiveDirection && parent->ActiveDirection != 0)
				{
					//stop music if elevator changes direction while music is on
					musicsound->Pause();
				}
				else
				{
					//manage music volume state
					//(turn on volume only if inside the elevator, or if the doors are open)
					if (InCar() == true || AreDoorsOpen() == true || AreDoorsMoving() != 0)
						musicsound->SetVolume(1.0);
					else
						musicsound->SetVolume(0.0);
				}
			}
		}
	}

	//turn on lights if in elevator, or if doors are open
	for (size_t i = 0; i < lights.size(); i++)
	{
		if (lights[i]->IsEnabled() == false)
		{
			if (InCar() == true || AreDoorsOpen() == true || AreDoorsMoving(0, true, false) != 0)
			{
				if (sbs->Verbose)
					Report("enabling lights");

				lights[i]->Enabled(true);
			}
		}
		else
		{
			if (InCar() == false && AreDoorsOpen() == false && AreDoorsMoving() == 0)
			{
				if (sbs->Verbose)
					Report("disabling lights");
				lights[i]->Enabled(false);
			}
		}
	}

	//process alarm
	if (AlarmActive == true)
		Alarm();

	//process door open/close holds
	if (doorhold_direction > 0)
		OpenDoors();
	if (doorhold_direction < 0)
		CloseDoors();

	//door operations
	for (int i = 1; i <= NumDoors; i++)
	{
		ElevatorDoor *door = GetDoor(i);
		if (door)
			door->Loop();

		//reset door timer if peak mode is enabled and a movement is pending
		if ((parent->UpPeak == true || parent->DownPeak == true))
		{
			if ((parent->UpQueue.empty() == false || parent->DownQueue.empty() == false) && (AreDoorsOpen() == true && AreDoorsMoving() == 0))
			{
				if (door)
				{
					if (door->TimerIsRunning() == false)
						door->Reset();
				}
			}
		}
	}

	//run child object runloops
	if (IsEnabled == true)
		LoopChildren();

	//process door sensors
	for (size_t i = 0; i < DoorArray.size(); i++)
	{
		if (DoorArray[i])
			DoorArray[i]->CheckSensor();
	}

	//process queued sounds
	announcesnd->ProcessQueue();

	//reset message sound status
	DirMessageSound = false;
	DoorMessageSound = false;
}

void ElevatorCar::Enabled(bool value)
{
	//shows/hides elevator car

	if (IsEnabled == value)
		return;

	if (sbs->Verbose)
	{
		if (value == true)
			Report("enabling car");
		else
			Report("disabling car");
	}

	Mesh->Enabled(value);
	EnableDoors(value);
	IsEnabled = value;

	//floor indicators
	for (size_t i = 0; i < FloorIndicatorArray.size(); i++)
	{
		if (FloorIndicatorArray[i])
			FloorIndicatorArray[i]->Enabled(value);
	}

	//interior directional indicators
	EnableDirectionalIndicators(value);

	if (value == false)
		EnableObjects(false);
}

void ElevatorCar::EnableObjects(bool value)
{
	//enable or disable interior objects, such as floor indicators and button panels

	if (AutoEnable == false)
		return;

	//SBS_PROFILE("ElevatorCar::EnableObjects");
	if (sbs->Verbose)
	{
		if (value == true)
			Report("enabling objects");
		else
			Report("disabling objects");
	}

	//floor indicators
	/*for (size_t i = 0; i < FloorIndicatorArray.size(); i++)
	{
		if (FloorIndicatorArray[i])
			FloorIndicatorArray[i]->Enabled(value);
	}*/

	//interior directional indicators
	//EnableDirectionalIndicators(value);

	//controls
	for (size_t i = 0; i < ControlArray.size(); i++)
	{
		if (ControlArray[i])
			ControlArray[i]->Enabled(value);
	}

	//triggers
	for (size_t i = 0; i < TriggerArray.size(); i++)
	{
		if (TriggerArray[i])
			TriggerArray[i]->Enabled(value);
	}

	//models
	for (size_t i = 0; i < ModelArray.size(); i++)
	{
		if (ModelArray[i])
			ModelArray[i]->Enabled(value);
	}

	//panels
	for (size_t i = 0; i < PanelArray.size(); i++)
		PanelArray[i]->Enabled(value);

	//primitives
	for (size_t i = 0; i < PrimArray.size(); i++)
	{
		if (PrimArray[i])
			PrimArray[i]->Enabled(value);
	}

	//custom objects
	for (size_t i = 0; i < CustomObjectArray.size(); i++)
	{
		if (CustomObjectArray[i])
			CustomObjectArray[i]->Enabled(value);
	}

	//sounds
	for (size_t i = 0; i < sounds.size(); i++)
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

	//reverb
	if (reverb)
		reverb->Enabled(value);
}

void ElevatorCar::UpdateFloorIndicators()
{
	//updates all floor indicators

	for (size_t i = 0; i < FloorIndicatorArray.size(); i++)
	{
		if (FloorIndicatorArray[i])
			FloorIndicatorArray[i]->Update();
	}
}

int ElevatorCar::GetTopFloor()
{
	//returns highest serviced floor

	if (ServicedFloors.empty() == true)
		return 0;

	return ServicedFloors[ServicedFloors.size() - 1];
}

int ElevatorCar::GetBottomFloor()
{
	//returns lowest serviced floor

	if (ServicedFloors.empty() == true)
		return 0;

	return ServicedFloors[0];
}

void ElevatorCar::AddDirectionalIndicators(bool relative, bool active_direction, bool single, bool vertical, const std::string &BackTexture, const std::string &uptexture, const std::string &uptexture_lit, const std::string &downtexture, const std::string &downtexture_lit, Real CenterX, Real CenterZ, Real voffset, const std::string &direction, Real BackWidth, Real BackHeight, bool ShowBack, Real tw, Real th)
{
	//create external directional indicators on all serviced floors

	if (sbs->Verbose)
		Report("adding directional indicators");

	for (size_t i = 0; i < ServicedFloors.size(); i++)
	{
		if (sbs->GetFloor(ServicedFloors[i]))
			sbs->GetFloor(ServicedFloors[i])->AddDirectionalIndicator(parent->Number, Number, relative, active_direction, single, vertical, BackTexture, uptexture, uptexture_lit, downtexture, downtexture_lit, CenterX, CenterZ, voffset, direction, BackWidth, BackHeight, ShowBack, tw, th);
	}
}

DirectionalIndicator* ElevatorCar::AddDirectionalIndicator(bool active_direction, bool single, bool vertical, const std::string &BackTexture, const std::string &uptexture, const std::string &uptexture_lit, const std::string &downtexture, const std::string &downtexture_lit, Real CenterX, Real CenterZ, Real voffset, const std::string &direction, Real BackWidth, Real BackHeight, bool ShowBack, Real tw, Real th)
{
	//create a directional indicator inside the elevator

	if (sbs->Verbose)
		Report("adding interior directional indicator");

	DirectionalIndicator *indicator = new DirectionalIndicator(this, parent->Number, Number, 0, active_direction, single, vertical, BackTexture, uptexture, uptexture_lit, downtexture, downtexture_lit, CenterX, CenterZ, voffset, direction, BackWidth, BackHeight, ShowBack, tw, th);
	DirIndicatorArray.emplace_back(indicator);
	return indicator;
}

void ElevatorCar::SetDirectionalIndicators(int floor, bool UpLight, bool DownLight)
{
	//set light status of exterior and interior directional indicators
	//for interior indicators, the value of floor is passed to the indicator for checks

	//exterior indicators
	if (sbs->GetFloor(floor))
		sbs->GetFloor(floor)->SetDirectionalIndicators(parent->Number, Number, UpLight, DownLight);

	//interior indicators
	for (size_t i = 0; i < DirIndicatorArray.size(); i++)
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

void ElevatorCar::UpdateDirectionalIndicators()
{
	//updates all interior active direction indicators

	for (size_t i = 0; i < DirIndicatorArray.size(); i++)
	{
		DirectionalIndicator *indicator = DirIndicatorArray[i];

		if (indicator)
		{
			if (indicator->ActiveDirection == true)
			{
				if (parent->ActiveDirection == 1)
				{
					indicator->DownLight(false);
					indicator->UpLight(true);
				}
				if (parent->ActiveDirection == 0)
				{
					indicator->DownLight(false);
					indicator->UpLight(false);
				}
				if (parent->ActiveDirection == -1)
				{
					indicator->DownLight(true);
					indicator->UpLight(false);
				}
			}
		}
	}
}

void ElevatorCar::EnableDirectionalIndicators(bool value)
{
	//turn on/off all interior directional indicators

	if (sbs->Verbose)
	{
		if (value == true)
			Report("enabling interior directional indicators");
		else
			Report("disabling interior directional indicators");
	}

	for (size_t i = 0; i < DirIndicatorArray.size(); i++)
	{
		if (DirIndicatorArray[i])
			DirIndicatorArray[i]->Enabled(value);
	}
}

ElevatorDoor* ElevatorCar::GetDoor(int number)
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

bool ElevatorCar::OpenDoorsEmergency(int number, int whichdoors, int floor, bool hold)
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

void ElevatorCar::CloseDoorsEmergency(int number, int whichdoors, int floor, bool hold)
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

bool ElevatorCar::OpenDoors(int number, int whichdoors, int floor, bool manual, bool hold)
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

	//only run if power is enabled
	if (sbs->GetPower() == false && manual == false)
		return false;

	//require open button to be held for fire service phase 2 if not on recall floor
	if (FirePhase2Active() == 1 && parent->OnRecallFloor() == false && manual == false)
		hold = true;

	if (parent->Interlocks == true)
	{
		//if elevator is moving and interlocks are enabled, stop elevator
		if (parent->IsMoving == true && parent->OnFloor == false)
			parent->Stop(true);
	}

	int start = number, end = number;
	if (number == 0)
	{
		start = 1;
		end = NumDoors;
	}
	if (doorhold_direction == 0)
	{
		if (ControlPressActive == true && parent->AutoDoors == true && parent->InServiceMode() == false && hold == false && manual == false && whichdoors != 3 && DoorsStopped(number) == false)
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
	else if (doorhold_direction == 1 && sbs->camera->MouseDown() == false)
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
			if (sbs->camera->MouseDown() == true)
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

		if (sbs->camera->MouseDown() == false)
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

void ElevatorCar::CloseDoors(int number, int whichdoors, int floor, bool manual, bool hold)
{
	//Closes elevator doors
	//if hold is true, sets 'hold' state requiring button to be held to keep doors closing

	//WhichDoors is the doors to move:
	//1 = both shaft and elevator doors
	//2 = only elevator doors
	//3 = only shaft doors

	//only run if power is enabled
	if (sbs->GetPower() == false && manual == false)
		return;

	//turn on hold option for certain modes
	if ((IndependentServiceActive() == true || FirePhase2Active() == 1) && manual == false)
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
	else if (doorhold_direction == -1 && sbs->camera->MouseDown() == false)
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
				{
					if (GetDoor(i)->AreDoorsMoving(0) == true)
						GetDoor(i)->OpenDoors(doorhold_whichdoors, doorhold_floor, doorhold_manual);
				}
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

void ElevatorCar::StopDoors(int number)
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

DoorWrapper* ElevatorCar::AddDoors(int number, const std::string &lefttexture, const std::string &righttexture, Real thickness, Real CenterX, Real CenterZ, Real width, Real height, bool direction, Real tw, Real th)
{
	//adds elevator doors specified at a relative central position (off of elevator origin)
	//if direction is false, doors are on the left/right side; otherwise front/back

	if (GetDoor(number))
		return GetDoor(number)->AddDoors(lefttexture, righttexture, thickness, CenterX, CenterZ, width, height, direction, tw, th);
	else
		ReportError("Invalid door " + ToString(number));
	return 0;
}

bool ElevatorCar::AddShaftDoors(int number, const std::string &lefttexture, const std::string &righttexture, Real thickness, Real CenterX, Real CenterZ, Real voffset, Real tw, Real th)
{
	//adds shaft's elevator doors specified at a relative central position (off of elevator origin)
	//uses some parameters (width, height, direction) from AddDoors function

	if (GetDoor(number))
	{
		Report("Adding shaft doors...");
		return GetDoor(number)->AddShaftDoors(lefttexture, righttexture, thickness, CenterX, CenterZ, voffset, tw, th);
	}
	else
		ReportError("Invalid door " + ToString(number));
	return false;
}

DoorWrapper* ElevatorCar::AddShaftDoor(int floor, int number, const std::string &lefttexture, const std::string &righttexture, Real tw, Real th)
{
	//adds a single elevator shaft door on the specified floor, with position and thickness parameters first specified
	//by the SetShaftDoors command.

	if (IsServicedFloor(floor) == true && GetDoor(number))
		return GetDoor(number)->AddShaftDoor(floor, lefttexture, righttexture, tw, th);
	else
		return 0;
}

DoorWrapper* ElevatorCar::AddShaftDoor(int floor, int number, const std::string &lefttexture, const std::string &righttexture, Real thickness, Real CenterX, Real CenterZ, Real voffset, Real tw, Real th)
{
	//adds a single elevator shaft door on the specified floor, with position and thickness parameters first specified
	//by the SetShaftDoors command.

	if (IsServicedFloor(floor) == true && GetDoor(number))
		return GetDoor(number)->AddShaftDoor(floor, lefttexture, righttexture, thickness, CenterX, CenterZ, voffset, tw, th);
	else
		return 0;
}

void ElevatorCar::ShaftDoorsEnabled(int number, int floor, bool value)
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

void ElevatorCar::ShaftDoorsEnabledRange(int number, int floor, int range)
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

bool ElevatorCar::AreDoorsOpen(int number)
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

bool ElevatorCar::AreShaftDoorsOpen(int number, int floor)
{
	//returns the internal shaft door state

	SBS_PROFILE("Elevator::AreShaftDoorsOpen");

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
			if (door->AreShaftDoorsOpen(floor) == true)
				return true;
		}
		else
			ReportError("Invalid door " + ToString(i));
	}
	return false;
}

bool ElevatorCar::AreShaftDoorsClosed(bool skip_current_floor)
{
	//return true if all shaft doors are closed and not moving

	for (size_t i = 0; i < DoorArray.size(); i++)
	{
		if (DoorArray[i])
		{
			if (DoorArray[i]->AreShaftDoorsClosed(skip_current_floor) == false)
				return false;
		}
	}
	return true;
}

void ElevatorCar::Chime(int number, int floor, bool direction, bool early)
{
	//play chime sound on specified floor

	SBS_PROFILE("Elevator::Chime");

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return;

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
			if (early == false)
				door->Chime(floor, direction);
			else
				door->EarlyChime(floor, direction);
		}
		else
			ReportError("Invalid door " + ToString(i));
	}
	if (direction == true)
		parent->LastChimeDirection = 1;
	else
		parent->LastChimeDirection = -1;
}

void ElevatorCar::ResetDoors(int number, bool sensor)
{
	//reset elevator door timer

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return;

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

bool ElevatorCar::DoorsStopped(int number)
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

int ElevatorCar::AreDoorsMoving(int number, bool car_doors, bool shaft_doors)
{
	//returns 1 if doors are opening (2 manual), -1 if doors are closing (-2 manual), or 0 if doors are not moving
	//if the type of door is specified, returns value if that type of door is moving

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

bool ElevatorCar::AreDoorsOpening(int number, bool car_doors, bool shaft_doors)
{
	//returns true if doors are opening

	if (AreDoorsMoving(number, car_doors, shaft_doors) == 1)
		return true;
	return false;
}

bool ElevatorCar::AreDoorsClosing(int number, bool car_doors, bool shaft_doors)
{
	//returns true if doors are closing

	if (AreDoorsMoving(number, car_doors, shaft_doors) == -1)
		return true;
	return false;
}

void ElevatorCar::EnableDoors(bool value)
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

	parent->DoorContainer->Enabled(value);
}

void ElevatorCar::SetShaftDoors(int number, Real thickness, Real CenterX, Real CenterZ)
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

bool ElevatorCar::AddFloorSigns(int door_number, bool relative, const std::string &texture_prefix, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset)
{
	//adds floor signs at the specified position and direction for each serviced floor,
	//depending on if the given door number services the floor or not (unless door_number is 0)

	Real x, z;
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
	sbs->GetTextureManager()->GetAutoSize(autosize_x, autosize_y);
	sbs->GetTextureManager()->SetAutoSize(false, false);

	for (size_t i = 0; i < ServicedFloors.size(); i++)
	{
		bool door_result = false;
		int floor = ServicedFloors[i];
		Real base = GetDestinationOffset(floor);

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
	sbs->GetTextureManager()->SetAutoSize(autosize_x, autosize_y);
	return true;
}

bool ElevatorCar::DoorExists(int number)
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

bool ElevatorCar::ShaftDoorsExist(int number, int floor, bool include_nonserviced)
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
			if (door->ShaftDoorsExist(floor, include_nonserviced) == true)
				return true;
		}
	}
	return false;
}

Sound* ElevatorCar::AddSound(const std::string &name, const std::string &filename, Vector3 position, bool loop, Real volume, int speed, Real min_distance, Real max_distance, Real doppler_level, Real cone_inside_angle, Real cone_outside_angle, Real cone_outside_volume, Vector3 direction)
{
	//create a sound object
	Sound *sound = new Sound(this, name, false);
	sounds.emplace_back(sound);

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
	if (loop && sbs->IsRunning == true && InCar() == true)
		sound->Play();

	return sound;
}

void ElevatorCar::ResetLights()
{
	//turn off button lights
	if (sbs->Verbose)
		Report("turning off button lights");

	for (size_t i = 0; i < PanelArray.size(); i++)
		PanelArray[i]->ChangeAllLights(false);
}

void ElevatorCar::ChangeLight(int floor, bool value)
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

	for (size_t i = 0; i < PanelArray.size(); i++)
		PanelArray[i]->ChangeLight(floor, value);
}

DoorWrapper* ElevatorCar::AddDoorComponent(int number, const std::string &name, const std::string &texture, const std::string &sidetexture, Real thickness, const std::string &direction, Real OpenSpeed, Real CloseSpeed, Real x1, Real z1, Real x2, Real z2, Real height, Real voffset, Real tw, Real th, Real side_tw, Real side_th)
{
	//adds an elevator door component to the specified door at a relative central position (off of elevator origin)

	if (GetDoor(number))
		return GetDoor(number)->AddDoorComponent(name, texture, sidetexture, thickness, direction, OpenSpeed, CloseSpeed, x1, z1, x2, z2, height, voffset, tw, th, side_tw, side_th);
	else
		ReportError("Invalid door " + ToString(number));
	return 0;
}

DoorWrapper* ElevatorCar::AddShaftDoorComponent(int number, int floor, const std::string &name, const std::string &texture, const std::string &sidetexture, Real thickness, const std::string &direction, Real OpenSpeed, Real CloseSpeed, Real x1, Real z1, Real x2, Real z2, Real height, Real voffset, Real tw, Real th, Real side_tw, Real side_th)
{
	//adds a single elevator shaft door component on the specified floor

	if (IsServicedFloor(floor) == true && GetDoor(number))
		return GetDoor(number)->AddShaftDoorComponent(floor, name, texture, sidetexture, thickness, direction, OpenSpeed, CloseSpeed, x1, z1, x2, z2, height, voffset, tw, th, side_tw, side_th);
	else
		return 0;
}

void ElevatorCar::AddShaftDoorsComponent(int number, const std::string &name, const std::string &texture, const std::string &sidetexture, Real thickness, const std::string &direction, Real OpenSpeed, Real CloseSpeed, Real x1, Real z1, Real x2, Real z2, Real height, Real voffset, Real tw, Real th, Real side_tw, Real side_th)
{
	//adds shaft's elevator door components specified at a relative central position (off of elevator origin)

	if (GetDoor(number))
	{
		Report("Adding shaft doors component...");
		GetDoor(number)->AddShaftDoorsComponent(name, texture, sidetexture, thickness, direction, OpenSpeed, CloseSpeed, x1, z1, x2, z2, height, voffset, tw, th, side_tw, side_th);
	}
	else
		ReportError("Invalid door " + ToString(number));
}

DoorWrapper* ElevatorCar::FinishDoors(int number, bool DoorWalls, bool TrackWalls)
{
	//finishes elevator door

	if (GetDoor(number))
		return GetDoor(number)->FinishDoors(DoorWalls, TrackWalls);
	else
		ReportError("Invalid door " + ToString(number));
	return 0;
}

DoorWrapper* ElevatorCar::FinishShaftDoor(int number, int floor, bool DoorWalls, bool TrackWalls)
{
	//finishes a single shaft door

	if (IsServicedFloor(floor) == true && GetDoor(number))
		return GetDoor(number)->FinishShaftDoor(floor, DoorWalls, TrackWalls);
	else
		return 0;
}

bool ElevatorCar::FinishShaftDoors(int number, bool DoorWalls, bool TrackWalls)
{
	//finishes all shaft doors

	if (GetDoor(number))
		return GetDoor(number)->FinishShaftDoors(DoorWalls, TrackWalls);
	else
		ReportError("Invalid door " + ToString(number));
	return false;
}

ButtonPanel* ElevatorCar::GetPanel(int index)
{
	//get a button panel object

	if (index > (int)PanelArray.size() || index < 1)
		return 0;

	return PanelArray[index - 1];
}

Control* ElevatorCar::GetFloorButton(int floor)
{
	//get a floor button

	if (parent->Running == false)
	{
		ReportError("Elevator not running");
		return 0;
	}

	if (PanelArray.empty() == false)
	{
		for (size_t i = 0; i < PanelArray.size(); i++)
		{
			Control *control = PanelArray[i]->GetFloorButton(floor);
			if (control)
				return control;
		}
	}
	return 0;
}

void ElevatorCar::HoldDoors(int number, bool sensor)
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

Door* ElevatorCar::AddDoor(std::string name, const std::string &open_sound, const std::string &close_sound, bool open_state, const std::string &texture, const std::string &side_texture, Real thickness, const std::string &face_direction, const std::string &open_direction, bool rotate, Real open_speed, Real close_speed, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, Real tw, Real th, Real side_tw, Real side_th)
{
	//interface to the SBS AddDoor function

	//set up coordinates
	/*Real x1, z1, x2, z2;
	if (face_direction == "left" || face_direction == "right")
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
	if (face_direction == "left" || face_direction == "right")
		CutAll(Vector3(x1 - 1, GetBase(true) + voffset, z1), Vector3(x2 + 1, GetBase(true) + voffset + height, z2), true, false);
	else
		CutAll(Vector3(x1, GetBase(true) + voffset, z1 - 1), Vector3(x2, GetBase(true) + voffset + height, z2 + 1), true, false);
	*/

	std::string number = ToString((int)StdDoorArray.size());
	if (name == "")
		name = "Door " + number;

	Door* door = new Door(this, 0, name, open_sound, close_sound, rotate);
	door->CreateDoor(open_state, texture, side_texture, thickness, face_direction, open_direction, open_speed, close_speed, CenterX, CenterZ, width, height, voffset, tw, th, side_tw, side_th);
	StdDoorArray.emplace_back(door);
	return door;
}

Door* ElevatorCar::CreateDoor(std::string name, const std::string &open_sound, const std::string &close_sound, bool rotate)
{
	//start creation of a manual door
	//since the door is unfinished, AddDoorComponent and FinishDoor need to be run on the returned Door object

	std::string number = ToString((int)StdDoorArray.size());
	if (name == "")
		name = "Door " + number;

	Door* door = new Door(this, 0, name, open_sound, close_sound, rotate);
	StdDoorArray.emplace_back(door);
	return door;
}

Door* ElevatorCar::GetStdDoor(int number)
{
	//get door object
	if (number < (int)StdDoorArray.size())
	{
		if (StdDoorArray[number])
			return StdDoorArray[number];
	}

	return 0;
}

Door* ElevatorCar::GetDoor(const std::string &name)
{
	for (size_t i = 0; i < StdDoorArray.size(); i++)
	{
		if (StdDoorArray[i]->GetName() == name)
			return StdDoorArray[i];
	}
	return 0;
}

void ElevatorCar::RemovePanel(ButtonPanel* panel)
{
	//remove a button panel reference (does not delete the object itself)
	for (size_t i = 0; i < PanelArray.size(); i++)
	{
		if (PanelArray[i] == panel)
		{
			PanelArray.erase(PanelArray.begin() + i);
			return;
		}
	}
}

void ElevatorCar::RemoveDirectionalIndicator(DirectionalIndicator* indicator)
{
	//remove a directional indicator reference (does not delete the object itself)
	for (size_t i = 0; i < DirIndicatorArray.size(); i++)
	{
		if (DirIndicatorArray[i] == indicator)
		{
			DirIndicatorArray.erase(DirIndicatorArray.begin() + i);
			return;
		}
	}
}

void ElevatorCar::RemoveElevatorDoor(ElevatorDoor* door)
{
	//remove an elevator door reference (does not delete the object itself)
	for (size_t i = 0; i < DoorArray.size(); i++)
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

void ElevatorCar::RemoveFloorIndicator(FloorIndicator* indicator)
{
	//remove a floor indicator reference (does not delete the object itself)
	for (size_t i = 0; i < FloorIndicatorArray.size(); i++)
	{
		if (FloorIndicatorArray[i] == indicator)
		{
			FloorIndicatorArray.erase(FloorIndicatorArray.begin() + i);
			return;
		}
	}
}

void ElevatorCar::RemoveDoor(Door* door)
{
	//remove a door reference (does not delete the object itself)
	for (size_t i = 0; i < StdDoorArray.size(); i++)
	{
		if (StdDoorArray[i] == door)
		{
			StdDoorArray.erase(StdDoorArray.begin() + i);
			return;
		}
	}
}

void ElevatorCar::RemoveSound(Sound *sound)
{
	//remove a sound reference (does not delete the object itself)
	for (size_t i = 0; i < sounds.size(); i++)
	{
		if (sounds[i] == sound)
		{
			sounds.erase(sounds.begin() + i);
			return;
		}
	}
}

void ElevatorCar::RemoveLight(Light *light)
{
	//remove a light reference (does not delete the object itself)
	for (size_t i = 0; i < lights.size(); i++)
	{
		if (lights[i] == light)
		{
			lights.erase(lights.begin() + i);
			return;
		}
	}
}

void ElevatorCar::RemoveModel(Model *model)
{
	//remove a model reference (does not delete the object itself)
	for (size_t i = 0; i < ModelArray.size(); i++)
	{
		if (ModelArray[i] == model)
		{
			ModelArray.erase(ModelArray.begin() + i);
			return;
		}
	}
}

void ElevatorCar::RemovePrimitive(Primitive *prim)
{
	//remove a prim reference (does not delete the object itself)
	for (size_t i = 0; i < PrimArray.size(); i++)
	{
		if (PrimArray[i] == prim)
		{
			PrimArray.erase(PrimArray.begin() + i);
			return;
		}
	}
}

void ElevatorCar::RemoveCustomObject(CustomObject *object)
{
	//remove a custom object reference (does not delete the object itself)
	for (size_t i = 0; i < CustomObjectArray.size(); i++)
	{
		if (CustomObjectArray[i] == object)
		{
			CustomObjectArray.erase(CustomObjectArray.begin() + i);
			return;
		}
	}
}

void ElevatorCar::RemoveControl(Control *control)
{
	//remove a control reference (does not delete the object itself)
	for (size_t i = 0; i < ControlArray.size(); i++)
	{
		if (ControlArray[i] == control)
		{
			ControlArray.erase(ControlArray.begin() + i);
			return;
		}
	}
}

void ElevatorCar::RemoveTrigger(Trigger *trigger)
{
	//remove a trigger reference (does not delete the object itself)
	for (size_t i = 0; i < TriggerArray.size(); i++)
	{
		if (TriggerArray[i] == trigger)
		{
			TriggerArray.erase(TriggerArray.begin() + i);
			return;
		}
	}
}

bool ElevatorCar::IsNudgeModeActive(int number)
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

void ElevatorCar::EnableNudgeMode(bool value, int number)
{
	//enables nudge mode on all doors or the specified door

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return;

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

void ElevatorCar::ResetNudgeTimer(bool start, int number)
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

Light* ElevatorCar::AddLight(const std::string &name, int type)
{
	//add a global light

	Light* light = new Light(this, name, type);
	lights.emplace_back(light);
	light->Enabled(false);
	return light;
}

Light* ElevatorCar::GetLight(const std::string &name)
{
	for (size_t i = 0; i < lights.size(); i++)
	{
		if (lights[i]->GetName() == name)
			return lights[i];
	}
	return 0;
}

Model* ElevatorCar::AddModel(const std::string &name, const std::string &filename, bool center, Vector3 position, Vector3 rotation, Real max_render_distance, Real scale_multiplier, bool enable_physics, Real restitution, Real friction, Real mass)
{
	//add a model
	Model* model = new Model(this, name, filename, center, position, rotation, max_render_distance, scale_multiplier, enable_physics, restitution, friction, mass);
	if (model->load_error == true)
	{
		delete model;
		return 0;
	}
	ModelArray.emplace_back(model);
	return model;
}

void ElevatorCar::AddModel(Model *model)
{
	//add a model reference

	if (!model)
		return;

	for (size_t i = 0; i < ModelArray.size(); i++)
	{
		if (ModelArray[i] == model)
			return;
	}

	ModelArray.emplace_back(model);
}

Primitive* ElevatorCar::AddPrimitive(const std::string &name)
{
	//add a prim
	Primitive* prim = new Primitive(this, name);
	PrimArray.emplace_back(prim);
	return prim;
}

void ElevatorCar::AddPrimitive(Primitive *primitive)
{
	//add a model reference

	if (!primitive)
		return;

	for (size_t i = 0; i < PrimArray.size(); i++)
	{
		if (PrimArray[i] == primitive)
			return;
	}

	PrimArray.emplace_back(primitive);
}

CustomObject* ElevatorCar::AddCustomObject(const std::string &name, const Vector3 &position, const Vector3 &rotation, Real max_render_distance, Real scale_multiplier)
{
	//add a custom object
	CustomObject* object = new CustomObject(this, name, position, rotation, max_render_distance, scale_multiplier);
	CustomObjectArray.emplace_back(object);
	return object;
}

void ElevatorCar::AddCustomObject(CustomObject *object)
{
	//add a custom object reference

	if (!object)
		return;

	for (size_t i = 0; i < CustomObjectArray.size(); i++)
	{
		if (CustomObjectArray[i] == object)
			return;
	}

	CustomObjectArray.emplace_back(object);
}

void ElevatorCar::AddDisplayFloor(int floor)
{
	//add a floor to the display floors list
	DisplayFloors.emplace_back(floor);
}

std::string ElevatorCar::GetFloorDisplay()
{
	//returns the current floor's indicator display string

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return "";

	std::string value;
	int floornum = GetFloor();
	Floor *floor = sbs->GetFloor(floornum);

	if (!floor)
		return value;

	//only show floor skip text if the floor is not serviced by any car in this elevator
	if (parent->UseFloorSkipText == true && IsServicedFloor(floornum) == false && parent->IsServicedFloor(floornum) == false)
		value = parent->FloorSkipText;
	else
	{
		if (DisplayFloors.size() > 0)
		{
			for (size_t i = 0; i < DisplayFloors.size(); i++)
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

Control* ElevatorCar::AddControl(const std::string &name, const std::string &sound, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, int selection_position, std::vector<std::string> &action_names, std::vector<std::string> &textures)
{
	//add a control
	std::vector<Action*> actionnull; //not used
	Control* control = new Control(this, name, false, sound, action_names, actionnull, textures, direction, width, height, true, selection_position);
	control->Move(Vector3(CenterX, voffset, CenterZ));
	ControlArray.emplace_back(control);
	return control;
}

Trigger* ElevatorCar::AddTrigger(const std::string &name, const std::string &sound_file, Vector3 &area_min, Vector3 &area_max, std::vector<std::string> &action_names)
{
	//add a trigger
	Trigger* trigger = new Trigger(this, name, false, sound_file, area_min, area_max, action_names);
	TriggerArray.emplace_back(trigger);
	return trigger;
}

bool ElevatorCar::ReplaceTexture(const std::string &oldtexture, const std::string &newtexture)
{
	return Mesh->ReplaceTexture(oldtexture, newtexture);
}

std::vector<Sound*> ElevatorCar::GetSound(const std::string &name)
{
	//get sound by name

	std::string findname = name;
	SetCase(findname, false);
	std::vector<Sound*> soundlist;
	for (size_t i = 0; i < sounds.size(); i++)
	{
		if (sounds[i])
		{
			std::string name2 = sounds[i]->GetName();
			SetCase(name2, false);
			if (findname == name2)
				soundlist.emplace_back(sounds[i]);
		}
	}
	return soundlist;
}

bool ElevatorCar::GetSensorStatus(int number)
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

void ElevatorCar::EnableSensor(bool value, int number)
{
	//enables door sensor on all doors or the specified door

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return;

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

bool ElevatorCar::GetHoldStatus(int number)
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

void ElevatorCar::ResetDoorState(int number)
{
	//reset elevator internal door state, in case of door malfunction

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return;

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

Model* ElevatorCar::GetModel(std::string name)
{
	//get a model by name

	SetCase(name, false);

	for (size_t i = 0; i < ModelArray.size(); i++)
	{
		if (SetCaseCopy(ModelArray[i]->GetName(), false) == name)
			return ModelArray[i];
	}

	return 0;
}

Primitive* ElevatorCar::GetPrimitive(std::string name)
{
	//get a primitive by name

	SetCase(name, false);

	for (size_t i = 0; i < PrimArray.size(); i++)
	{
		if (SetCaseCopy(PrimArray[i]->GetName(), false) == name)
			return PrimArray[i];
	}

	return 0;
}

CustomObject* ElevatorCar::GetCustomObject(std::string name)
{
	//get a custom object by name

	SetCase(name, false);

	for (size_t i = 0; i < CustomObjectArray.size(); i++)
	{
		if (SetCaseCopy(CustomObjectArray[i]->GetName(), false) == name)
			return CustomObjectArray[i];
	}

	return 0;
}

void ElevatorCar::SetBeepSound(const std::string &filename)
{
	//set sound used for floor beeps
	if (sbs->Verbose)
		Report("setting beep sound");
	BeepSound = filename;
	TrimString(BeepSound);
	UseFloorBeeps = true;
}

void ElevatorCar::SetFloorSound(const std::string &prefix)
{
	//set prefix of floor sound
	if (sbs->Verbose)
		Report("setting floor sound");
	FloorSound = prefix;
	TrimString(FloorSound);
	UseFloorSounds = true;
}

void ElevatorCar::SetMessageSound(bool type, bool direction, const std::string &filename)
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

bool ElevatorCar::PlayFloorBeep()
{
	//play floor beep sound

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	if (parent->InServiceMode() == true || BeepSound == "" || UseFloorBeeps == false)
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

bool ElevatorCar::PlayFloorSound()
{
	//play floor sound

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	if (parent->InServiceMode() == true || FloorSound == "" || UseFloorSounds == false || parent->SkipFloorSound == true)
		return false;

	if (sbs->Verbose)
		Report("playing floor sound");

	std::string newsound = FloorSound;
	//change the asterisk into the current floor number
	ReplaceAll(newsound, "*", ToString(parent->GotoFloor));
	TrimString(newsound);
	announcesnd->PlayQueued(newsound, false, false);
	return true;
}

bool ElevatorCar::PlayMessageSound(bool type)
{
	//play message sound
	//if type is true, play directional up/down sounds, otherwise play door open/close sounds
	//if direction is true, play up sound; otherwise play down sound

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	if (parent->InServiceMode() == true)
		return false;

	if (parent->IsQueueActive() == false && type == true && MessageOnClose == true)
		return false;

	std::string newsound;

	if (type == true)
	{
		//exit if directional message sounds are off, or one has already been queued
		if (UseDirMessageSounds == false || DirMessageSound == true)
			return false;

		int direction = 0;

		if (MessageOnMove == false)
		{
			if (type == true)
			{
				if (parent->LastChimeDirection == 0)
					return false;

				if (parent->NotifyLate == false && parent->NotifyEarly == -1)
					return false;
			}

			direction = parent->LastChimeDirection;

			if (parent->LastChimeDirection == 0)
				direction = parent->LastQueueDirection;
		}
		else
			direction = parent->ActiveDirection;

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

void ElevatorCar::PlayStartingSounds()
{
	//play car starting sounds

	carsound->Stop();
	if (parent->Direction == 1 && UpStartSound.empty() == false && UpStartSound != "")
	{
		if (sbs->Verbose)
			Report("playing car up start sound");

		carsound->Load(UpStartSound);
		carsound->SetLoopState(false);
		carsound->Play();
	}
	if (parent->Direction == -1 && DownStartSound.empty() == false && DownStartSound != "")
	{
		if (sbs->Verbose)
			Report("playing car down start sound");

		carsound->Load(DownStartSound);
		carsound->SetLoopState(false);
		carsound->Play();
	}
}

void ElevatorCar::PlayStoppingSounds(bool emergency)
{
	//play car stopping sounds
	//if emergency is true, plays emergency stop sounds with a fallback to standard sounds

	bool play = false;
	std::string file;

	if (emergency == true)
	{
		if (EmergencyStopSound.empty() == false && EmergencyStopSound != "")
		{
			if (sbs->Verbose)
				Report("playing car emergency stop sound");

			file = EmergencyStopSound;
			play = true;
		}
	}

	if (play == false)
	{
		if (parent->Direction == -1 && UpStopSound.empty() == false && UpStopSound != "")
		{
			if (sbs->Verbose)
				Report("playing car up stop sound");

			file = UpStopSound;
			play = true;
		}
		if (parent->Direction == 1 && DownStopSound.empty() == false && DownStopSound != "")
		{
			if (sbs->Verbose)
				Report("playing car down stop sound");

			file = DownStopSound;
			play = true;
		}
	}

	carsound->Stop();

	if (play == true)
	{
		carsound->Load(file);
		carsound->SetLoopState(false);

		//set play position to current percent of the total speed
		if (parent->AutoAdjustSound == true)
		{
			if (parent->Direction == 1)
				carsound->SetPlayPosition(1 - (parent->ElevatorRate / parent->UpSpeed));
			else
				carsound->SetPlayPosition(1 - (parent->ElevatorRate / parent->DownSpeed));
		}
		else
			carsound->Reset();

		carsound->Play(false);
	}
}

void ElevatorCar::PlayMovingSounds()
{
	//play car movement sounds

	if (carsound->IsPlaying() == false)
	{
		if (parent->Direction == 1 && UpMoveSound.empty() == false && UpMoveSound != "")
		{
			if (sbs->Verbose)
				Report("playing car up movement sound");

			carsound->Load(UpMoveSound);
			carsound->SetLoopState(true);
			carsound->Play();
		}
		else if (parent->Direction == -1 && DownMoveSound.empty() == false && DownMoveSound != "")
		{
			if (sbs->Verbose)
				Report("playing car down movement sound");

			carsound->Load(DownMoveSound);
			carsound->SetLoopState(true);
			carsound->Play();
		}
	}
}

Real ElevatorCar::SetHeight()
{
	//make sure height value is set
	if (HeightSet == false)
	{
		Height = Mesh->GetHeight();
		HeightSet = true;

		//position sounds at top of elevator car
		Vector3 top = Vector3(0, Height, 0);
		idlesound->SetPositionRelative(top);
		alarm->SetPositionRelative(top);
		floorbeep->SetPositionRelative(top);
		announcesnd->SetPositionRelative(top);

		//set default music position to elevator height
		if (MusicPosition == Vector3(0, 0, 0) && Height > 0)
			MusicPosition = top;
		musicsound->SetPositionRelative(MusicPosition);
	}

	return Height;
}

bool ElevatorCar::IsInCar(const Vector3 &position, bool camera)
{
	//determine if the given 3D position is inside the car

	//first checks to see if camera is within an car height range, and then
	//checks for a collision with the car floor below

	//if camera is true, set associated camera offset

	//SBS_PROFILE("ElevatorCar::IsInCar");
	bool result = false;

	if (IsEnabled == false)
		return false;

	//if last position is the same as new, return previous result
	if (position.positionEquals(lastposition) == true && checkfirstrun == false)
		return lastcheckresult;

	checkfirstrun = false;

	Real ypos = GetPosition().y;

	if (position.y >= (ypos - 0.1) && position.y < ypos + (Height * 2))
	{
		if (Mesh->InBoundingBox(position, false) == true)
		{
			if (Mesh->HitBeam(position, Vector3::NEGATIVE_UNIT_Y, Height) >= 0)
			{
				if (camera == true)
					CameraOffset = position.y - ypos;
				result = true;
			}
		}
		else if (camera == true)
			CameraOffset = 0;

		if (position.y < ypos + Height)
		{
			//cache values
			lastcheckresult = result;
			lastposition = position;

			return result;
		}
	}
	else if (camera == true)
		CameraOffset = 0;

	//cache values
	lastcheckresult = false;
	lastposition = position;

	return false;
}

bool ElevatorCar::Check(Vector3 &position)
{
	//check to see if user (camera) is in the car

	if (IsEnabled == false)
		return false;

	//SBS_PROFILE("ElevatorCar::Check");

	if (IsInCar(position, true) == true && IsEnabled)
	{
		if (InCar() == false)
		{
			//turn on objects if moving into the car
			EnableObjects(true);
			UpdateFloorIndicators();
		}
		sbs->InElevator = true;
		sbs->ElevatorNumber = parent->Number;
		sbs->CarNumber = Number;
		sbs->ElevatorSync = true;
		return true;
	}

	//turn off objects if user has moved outside the car
	else if (InCar() == true)
	{
		EnableObjects(false);
	}

	//if camera is within vertical elevator range, turn on syncing to allow things like elevator surfing
	else if (CameraOffset > Height && CameraOffset < Height * 2)
	{
		sbs->ElevatorNumber = parent->Number;
		sbs->CarNumber = Number;
		sbs->ElevatorSync = true;
		return true;
	}
	return false;
}

void ElevatorCar::StopCarSound()
{
	//stop car sound

	carsound->Stop();
}

int ElevatorCar::GetFloor()
{
	//Determine floor that the car is on

	int newlastfloor;

	if (lastfloorset == true)
		newlastfloor = sbs->GetFloorNumber(GetPosition().y, lastfloor, true);
	else
		newlastfloor = sbs->GetFloorNumber(GetPosition().y);

	lastfloor = newlastfloor;
	lastfloorset = true;
	return lastfloor;
}

bool ElevatorCar::OnTopFloor()
{
	//returns true if car is on the highest serviced floor

	return (GetFloor() == GetTopFloor());
}

bool ElevatorCar::OnBottomFloor()
{
	//returns true if car is on the lowest serviced floor

	return (GetFloor() == GetBottomFloor());
}

bool ElevatorCar::InCar()
{
	//return true if user is currently in car

	return (sbs->InElevator == true && sbs->ElevatorNumber == parent->Number && sbs->CarNumber == Number);
}

int ElevatorCar::GetNearestServicedFloor()
{
	//return number of closest serviced floor

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return 0;

	if (IsServicedFloor(GetFloor()) == true)
		return GetFloor();

	if (ServicedFloors.size() == 0)
		return 0;

	bool firstrun = true;
	size_t nearest = 0;
	Real nearest_difference = 0;

	for (size_t i = 0; i < ServicedFloors.size() - 1; i++)
	{
		if (firstrun == true)
		{
			if (sbs->GetFloor(ServicedFloors[i]))
			{
				nearest_difference = std::abs(GetPosition().y - GetDestinationOffset(ServicedFloors[i]));
				nearest = i;
				firstrun = false;
			}
		}
		else
		{
			if (sbs->GetFloor(ServicedFloors[i]))
			{
				Real difference = std::abs(GetPosition().y - GetDestinationOffset(ServicedFloors[i]));
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

Real ElevatorCar::GetDestinationAltitude(int floor)
{
	//returns the destination altitude of the specified floor, based on shaft door positioning

	if (IsServicedFloor(floor) == false)
		return 0.0;

	Real result = 0;
	bool found = false;
	for (size_t i = 0; i < DoorArray.size(); i++)
	{
		if (DoorArray[i]->ShaftDoorsExist(floor) == true)
		{
			Real altitude = DoorArray[i]->GetShaftDoorAltitude(floor);

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

Real ElevatorCar::GetDestinationOffset(int floor)
{
	//returns the offset distance from the floor's base altitude the elevator destination is

	if (IsServicedFloor(floor) == false)
		return 0.0;

	if (sbs->GetFloor(floor))
		return GetDestinationAltitude(floor) - sbs->GetFloor(floor)->GetBase();

	return 0.0;
}

void ElevatorCar::SetFloor(int floor, bool move_parent)
{
	//set car's altitude to specified floor

	Real altitude = 0;
	if (!sbs->GetFloor(floor))
		return;

	altitude = GetDestinationAltitude(floor);
	if (move_parent == true)
	{
		//move elevator, and any other cars
		parent->MoveObjects(altitude - parent->GetPosition().y);
	}
	else
	{
		//move just this car
		Vector3 vector (0, altitude - GetPosition().y, 0);
		Move(vector);
	}
}

bool ElevatorCar::IsLeveled()
{
	//return true if car is leveled at a serviced floor

	Real tolerance = 0.005;

	int floor = GetFloor();
	if (IsServicedFloor(floor) == true)
	{
		Real altitude = GetDestinationAltitude(floor);

		if (GetPosition().y >= altitude - tolerance &&
				GetPosition().y <= altitude + tolerance)
			return true;
	}
	return false;
}

bool ElevatorCar::IsOnFloor(int floor, bool leveled)
{
	//return true if the car is on the specified floor
	//if leveled is true, returns true only if the car is leveled on that floor

	if (GetFloor() == floor)
	{
		if (leveled == false)
			return true;
		else if (IsLeveled() == true && parent->IsMoving == false)
			return true;
	}

	return false;
}

void ElevatorCar::NotifyArrival(int floor, bool early, int direction)
{
	//notify on car arrival (play chime and turn on related directional indicator lantern)

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return;

	//do not notify if in a service mode
	if (parent->InServiceMode() == true)
		return;

	//get call button status
	bool up = true, down = true;

	//if ChimeOnArrival is off, only chime if responding to a hall call
	if (parent->ChimeOnArrival == false && direction == 0)
	{
		//get call status from controller
		parent->GetCallStatus(floor, up, down);

		//in DD mode, set direction to be the call response direction
		if (parent->GetDestinationDispatch() == true)
			direction = RespondingToCall(floor);
	}

	bool new_direction = false;

	//get notification direction
	if (LateDirection != 0)
	{
		//notify is a late notification
		if (LateDirection == 1)
			new_direction = true;
		else
			new_direction = false;
		LateDirection = 0;
	}
	else
	{
		if (direction == 0)
		{
			//notify is for a standard arrival
			new_direction = parent->GetArrivalDirection(floor);
		}
		else
		{
			//notify is for a same-floor call
			if (direction == 1)
				new_direction = true;
			else
				new_direction = false;
		}
	}

	//play chime sound and change indicator
	if (new_direction == true)
	{
		if (up == true || direction == 1 || parent->NotifyLate == true || (parent->GetDestinationDispatch() == true && parent->ChimeOnArrival == true))
		{
			Chime(0, floor, true, early);
			SetDirectionalIndicators(floor, true, false);
		}
		parent->LastChimeDirection = 1;
	}
	else
	{
		if (down == true || direction == -1 || parent->NotifyLate == true || (parent->GetDestinationDispatch() == true && parent->ChimeOnArrival == true))
		{
			Chime(0, floor, false, early);
			SetDirectionalIndicators(floor, false, true);
		}
		parent->LastChimeDirection = -1;
	}

	//play floor sound
	if (parent->FireServicePhase1 == 0 && parent->FireServicePhase2 == 0 && direction == 0)
		PlayFloorSound();
}

bool ElevatorCar::IndependentServiceActive()
{
	//returns true if independent service mode is active for this car

	return (parent->IndependentService == true && parent->IndependentServiceCar == Number);
}

bool ElevatorCar::IndependentServiceOnOtherCar()
{
	//returns true if independent service mode is active on this elevator, but for a different car

	return (parent->IndependentService == true && parent->IndependentServiceCar != Number);
}

int ElevatorCar::FirePhase2Active()
{
	//returns value if fire phase 2 mode is active for this car

	if (parent->FireServicePhase2 > 0 && parent->FireServicePhase2Car == Number)
		return parent->FireServicePhase2;
	return 0;
}

bool ElevatorCar::FirePhase2OnOtherCar()
{
	//returns true if fire phase 2 mode is active on this elevator, but for a different car

	return (parent->FireServicePhase2 > 0 && parent->FireServicePhase2Car != Number);
}

void ElevatorCar::SetControls(const std::string &action_name)
{
	for (size_t i = 0; i < PanelArray.size(); i++)
	{
		PanelArray[i]->SetControls(action_name);
	}
}

void ElevatorCar::FlashIndicators(bool value)
{
	//flash all floor indicators if supported

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return;

	for (size_t i = 0; i < FloorIndicatorArray.size(); i++)
		FloorIndicatorArray[i]->Flash(value);
}

CameraTexture* ElevatorCar::AddCameraTexture(const std::string &name, int quality, Real fov, const Vector3 &position, bool use_rotation, const Vector3 &rotation)
{
	//add a camera texture
	CameraTexture* cameratexture = new CameraTexture(this, name, quality, fov, position, use_rotation, rotation);
	CameraTextureArray.emplace_back(cameratexture);
	return cameratexture;
}

bool ElevatorCar::RespondingToCall(int floor, int direction)
{
	//returns true if the car is responding to a call on the specified floor

	Elevator *e = GetElevator();

	//if proceeding to call floor for a call
	if (GotoFloor == true && e->GotoFloor == floor && e->GetActiveCallFloor() == e->GotoFloor)
	{
		//and if a hall call, with the same call direction
		if (e->GetActiveCallType() == 1 && e->GetActiveCallDirection() == direction)
			return true;
	}

	return false;
}

int ElevatorCar::RespondingToCall(int floor)
{
	//same as other function, but returns the direction of the call

	Elevator *e = GetElevator();

	//if proceeding to call floor for a call
	if (GotoFloor == true && e->GotoFloor == floor && e->GetActiveCallFloor() == e->GotoFloor)
	{
		//and if a hall call, with the same call direction
		if (e->GetActiveCallType() == 1)
			return e->GetActiveCallDirection();
	}

	return 0;
}

Indicator* ElevatorCar::AddKeypadIndicator(const std::string& sound, const std::string& texture_prefix, const std::string& blank_texture, const std::string& direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, Real timer_duration)
{
	if (indicator)
		return 0;

	indicator = new Indicator(this, sound, texture_prefix, blank_texture, direction, CenterX, CenterZ, width, height, voffset, timer_duration);

	return indicator;
}

void ElevatorCar::UpdateKeypadIndicator(const std::string& text, bool play_sound)
{
	if (indicator)
		indicator->Update(text, play_sound);
}

void ElevatorCar::Requested(int floor)
{
	//report which elevator is assigned, on indicator
	std::string message = "Requested";

	Elevator* e = GetElevator();

	//show an error if a floor hasn't been selected due to different factors
	if (e->InspectionService == true)
		KeypadError();
	else if (e->FireServicePhase1 == 1 && e->FireServicePhase2 == 0)
		KeypadError();
	else if (e->Running == false)
		KeypadError();
	else if (e->IndependentService == true && (AreDoorsOpen() == false || AreDoorsMoving() != 0))
		KeypadError();
	else if (e->FireServicePhase2 == 2)
		KeypadError();
	else if (e->LimitQueue == true && ((e->QueuePositionDirection == 1 && floor < GetFloor() && e->UpQueue.empty() == false) || (e->QueuePositionDirection == -1 && floor > GetFloor() && e->DownQueue.empty() == false)))
		KeypadError();
	else if (e->LimitQueue == true && e->IsMoving == true && floor == GetFloor())
		KeypadError();
	else
		UpdateKeypadIndicator(message);
}

bool ElevatorCar::Input(const std::string& text)
{
	//input a keypad character

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return false;

	//only allow single characters
	if (text.length() != 1)
		return false;

	//check lock state
	//if (IsLocked() == true)
		//return ReportError("Call station is locked");

	//erase last character if specified
	if (text == "<" && InputCache.size() >= 1)
		InputCache.pop_back();

	//add text to cache
	InputCache += text;

	//automatically error if multiple special characters were entered and/or combined with numbers
	int StarCount = std::count(InputCache.begin(), InputCache.end(), '*');
	int MinCount = std::count(InputCache.begin(), InputCache.end(), '-');
	if ((StarCount > 1 || MinCount > 1) && InputCache.length() > 1)
	{
		keypad_timer->Stop();
		InputCache = "";
		KeypadError(1);
		return true;

		//update indicator display
		UpdateKeypadIndicator(InputCache, false);

		//start timeout timer
		keypad_timer->Start(2000, true);

		return true;
	}


	//update indicator display
	UpdateKeypadIndicator(InputCache, false);

	//verify that the floor entry is valid, error if not
	int result = 0;
	if (GetFloorFromID(InputCache, result) == false && InputCache != "*" && InputCache != "-")
	{
		keypad_timer->Stop();
		InputCache = "";
		KeypadError(1);
		return true;
	}

	//start timeout timer
	keypad_timer->Start(TimerDelay * 1000.0f, true);

	return true;
}

void ElevatorCar::ProcessCache()
{
	//process and clear input cache

	Elevator* e = GetElevator();

	//select recall floor if "0"
	if (InputCache == "0" || InputCache == "*")
	{
		e->SelectFloor(e->GetRecallFloor());
		InputCache = "";
		Requested(e->GetRecallFloor());
		return;
	}

	if (!IsNumeric(InputCache))
	{
		InputCache = "";
		KeypadError();
		return;
	}

	//don't allow input values in the InvalidInput list
	for (size_t i = 0; i < InvalidInput.size(); i++)
	{
		if (InputCache == InvalidInput[i])
		{
			InputCache = "";
			KeypadError();
			return;
		}
	}

	int floor = 0;
	GetFloorFromID(InputCache, floor);
	Requested(floor);
	e->SelectFloor(floor);

	InputCache = "";
}

bool ElevatorCar::GetFloorFromID(const std::string& floor, int& result)
{
	if (!IsNumeric(floor))
		return false;

	int rawfloor = ToInt(floor);

	//convert back to string, to strip off any leading 0's
	std::string converted = ToString(rawfloor);

	Floor* floorobj = sbs->GetFloorManager()->GetByNumberID(converted);
	Floor* floorobj2 = sbs->GetFloorManager()->GetByID(converted);
	Floor* floorobj3 = sbs->GetFloorManager()->Get(rawfloor);

	if (floorobj)
	{
		result = floorobj->Number; //get by number ID first
		return true;
	}
	else if (floorobj2)
	{
		result = floorobj2->Number; //next try floor ID
		return true;
	}
	else if (floorobj3)
	{
		result = rawfloor; //and last, get by raw floor number
		return true;
	}

	return false;
}

void ElevatorCar::KeypadTimer::Notify()
{
	//input timeout timer

	parent->ProcessCache();
}

void ElevatorCar::KeypadError(bool type)
{
	//if type is 0, standard error
	//if type is 1, invalid floor

	std::string message = "XX";
	if (type == 1)
		message = "??";

	UpdateKeypadIndicator(message);
}

bool ElevatorCar::AddElevatorIDSigns(int door_number, bool relative, const std::string& texture_prefix, const std::string& direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset)
{
	//adds elevator ID signs at the specified position and direction for each serviced floor,
	//depending on if the given door number services the floor or not (unless door_number is 0)

	Elevator* e = GetElevator();

	Real x, z;
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
			return ReportError("AddElevatorIDSigns: door " + ToString(door_number) + " does not exist");
	}

	bool autosize_x, autosize_y;
	sbs->GetTextureManager()->GetAutoSize(autosize_x, autosize_y);
	sbs->GetTextureManager()->SetAutoSize(false, false);

	for (size_t i = 0; i < ServicedFloors.size(); i++)
	{
		bool door_result = false;
		int floor = ServicedFloors[i];
		Real base = GetDestinationOffset(floor);

		if (door_number != 0)
			door_result = ShaftDoorsExist(door_number, floor);

		if ((door_number == 0 || door_result == true) && sbs->GetFloor(floor))
		{
			std::string texture = texture_prefix + e->ID;
			std::string tmpdirection = direction;
			SetCase(tmpdirection, false);

			if (tmpdirection == "front" || tmpdirection == "left")
				sbs->DrawWalls(true, false, false, false, false, false);
			else
				sbs->DrawWalls(false, true, false, false, false, false);

			if (tmpdirection == "front" || tmpdirection == "back")
				sbs->GetFloor(floor)->AddWall("Elevator ID Sign", texture, 0, x - (width / 2), z, x + (width / 2), z, height, height, base + voffset, base + voffset, 1, 1, false);
			else
				sbs->GetFloor(floor)->AddWall("Elevator ID Sign", texture, 0, x, z - (width / 2), x, z + (width / 2), height, height, base + voffset, base + voffset, 1, 1, false);
			sbs->ResetWalls();
		}
	}
	sbs->GetTextureManager()->SetAutoSize(autosize_x, autosize_y);
	return true;
}

Reverb* ElevatorCar::AddReverb(const std::string &name, const std::string &type, const Vector3 &position, Real min_distance, Real max_distance)
{
	if (reverb)
		return 0;

	//create a reverb object
	reverb = new Reverb(this, name, type, position, min_distance, max_distance, false);
	return reverb;
}

Reverb* ElevatorCar::GetReverb()
{
	return reverb;
}

void ElevatorCar::RemoveReverb()
{
	//remove the reverb
	//this does not delete the object

	reverb = 0;
}


}
