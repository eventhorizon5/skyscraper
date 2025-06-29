/*
	Scalable Building Simulator - Action Interface Class
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
#include "elevator.h"
#include "elevatorcar.h"
#include "shaft.h"
#include "stairs.h"
#include "camera.h"
#include "callstation.h"
#include "sound.h"
#include "mesh.h"
#include "escalator.h"
#include "movingwalkway.h"
#include "cameratexture.h"
#include "light.h"
#include "door.h"
#include "revolvingdoor.h"
#include "profiler.h"
#include "action.h"

namespace SBS {

Action::Action(Object *parent, const std::string &name, std::vector<Object*> &action_parents, const std::string &command, const std::vector<std::string> &parameters) : ObjectBase(parent)
{
	//create an action

	command_name = command;
	SetName(name);
	TrimString(command_name);
	SetCase(command_name, false);

	command_parameters.resize(parameters.size());
	for (size_t i = 0; i < parameters.size(); i++)
	{
		command_parameters[i] = parameters[i];
		TrimString(command_parameters[i]);
	}
	parent_objects = action_parents;
}

Action::Action(Object *parent, const std::string &name, std::vector<Object*> &action_parents, const std::string &command) : ObjectBase(parent)
{
	//create an action

	command_name = command;
	SetName(name);
	TrimString(command_name);
	SetCase(command_name, false);

	parent_objects = action_parents;
}

Action::~Action()
{
	if (sbs->FastDelete == false)
		Report("Deleted action '" + GetName() + "'");
}

std::string Action::GetCommandName()
{
	return command_name;
}

bool Action::DoAction(Object *caller, bool &hold)
{
	//run action on all registered parents
	//returns true if at least one action succeeded

	bool result = false;
	size_t count = parent_objects.size();
	for (size_t i = 0; i < count; i++)
	{
		if (!parent_objects[i])
			continue;

		bool result2 = Run(caller, parent_objects[i], hold);
		if (result2 == true)
			result = true;
	}
	return result;
}

bool Action::Run(Object *caller, Object *parent, bool &hold)
{
	//Supported action names:

	////General actions:
	//ChangeTexture  - param1: oldtexture param2: newtexture
	//PlaySound - param1: sound name, param2: loop (true/false)
	//StopSound - param1: sound name
	//Teleport - param1: X coordinate, param2: Y coordinate, param3; Z coordinate
	//GotoFloor - param1: number

	////Elevator actions:
	//Off
	//(floor number)
	//Open = Open Doors
	//Close = Close Doors
	//OpenInt
	//CloseInt
	//OpenExt
	//CloseExt
	//OpenManual
	//CloseManual
	//OpenIntManual
	//CloseIntManual
	//OpenExtManual
	//CloseExtManual
	//OpenShaftDoor
	//CloseShaftDoor
	//OpenShaftDoorManual
	//CloseShaftDoorManual
	//StopDoors
	//Cancel = Call Cancel
	//Run
	//Stop
	//EStop (emergency stop)
	//Alarm
	//Fire1Off
	//Fire1On
	//Fire1Bypass
	//Fire2Off
	//Fire2On
	//Fire2Hold
	//UpPeakOn
	//UpPeakOff
	//DownPeakOn
	//DownPeakOff
	//PeakOff
	//IndOn
	//IndOff
	//InsOn
	//InsOff
	//AcpOn
	//AcpOff
	//FanOn
	//FanOff
	//MusicOn
	//MusicOff
	//Hold
	//UpOn
	//UpOff
	//DownOn
	//DownOff
	//GoOn
	//GoOff
	//Return
	//Up
	//Down
	//InterlocksOn
	//InterlocksOff
	//Sensor
	//Reset
	//SensorOn
	//SensorOff
	//SensorReset
	//AccessDown
	//AccessUp
	//AccessOff
	//Input1
	//Input2
	//Input3
	//Input4
	//Input5
	//Input6
	//Input7
	//Input8
	//Input9
	//Input0
	//InputMinus
	//InputStar
	//InputBackspace

	//CallStation actions:
	//Off
	//(floor number)
	//FireOff
	//FireOn
	//FireBypass
	//Input1
	//Input2
	//Input3
	//Input4
	//Input5
	//Input6
	//Input7
	//Input8
	//Input9
	//Input0
	//InputMinus
	//InputStar
	//InputBackspace
	//Up
	//Down
	//PressUp
	//PressDown

	//Escalator and MovingWalkway actions:
	//Forward
	//Reverse
	//Stop

	//CameraTexture actions:
	//Enable
	//Disable

	//Light actions:
	//On
	//Off

	//Door actions:
	//Open
	//Close
	//AutoClose

	//RevolvingDoor actions:
	//On
	//Off

	SBS_PROFILE("Action::Run");

	Elevator *elevator = dynamic_cast<Elevator*>(parent);
	ElevatorCar *car = dynamic_cast<ElevatorCar*>(parent);
	Floor *floor = dynamic_cast<Floor*>(parent);
	Shaft *shaft = dynamic_cast<Shaft*>(parent);
	Stairwell *stairs = dynamic_cast<Stairwell*>(parent);
	CallStation *callstation = dynamic_cast<CallStation*>(parent);
	Escalator *escalator = dynamic_cast<Escalator*>(parent);
	MovingWalkway *walkway = dynamic_cast<MovingWalkway*>(parent);
	CameraTexture *camtex = dynamic_cast<CameraTexture*>(parent);
	Light *light = dynamic_cast<Light*>(parent);
	Door *door = dynamic_cast<Door*>(parent);
	RevolvingDoor *revdoor = dynamic_cast<RevolvingDoor*>(parent);

	std::string caller_name = caller->GetName();
	std::string caller_type = caller->GetType();

	std::string parent_name = parent->GetName();
	std::string parent_type = parent->GetType();

	hold = false;

	//report the action used
	Report("Action '" + GetName() + "': object '" + parent_name + "' using command '" + command_name + "'");

	//if parent is an elevator object, also use default (first) car as car object
	if (elevator)
		car = elevator->GetCar(1);
	//if parent is an elevator car, get parent elevator object
	else if (car)
		elevator = car->GetElevator();

	//elevator-specific commands
	if (elevator && car)
	{
		//numeric commands for elevator floor selections
		if (IsNumeric(command_name) == true)
		{
			int floor = ToInt(command_name);
			return elevator->SelectFloor(floor);
		}

		//get first call station on recall floor
		CallStation *station = elevator->GetPrimaryCallStation();

		//if called from a control and mouse button is held down, notify elevator
		if (caller_type == "Control" && sbs->camera->MouseDown() == true)
			car->ControlPressActive = true;

		if (command_name == "off")
			return true;

		//if (!(elevator->FireServicePhase1 == 1 && elevator->FireServicePhase2 == 0))
		//{
			if (StartsWith(command_name, "openintmanual", false) == true && elevator->Direction == 0)
			{
				int number = 0;
				if (command_name.length() > 13)
					number = ToInt(command_name.substr(13, command_name.length() - 13));
				return car->OpenDoors(number, 2, 0, true);
			}
			if (StartsWith(command_name, "closeintmanual", false) == true && elevator->Direction == 0)
			{
				int number = 0;
				if (command_name.length() > 14)
					number = ToInt(command_name.substr(14, command_name.length() - 14));
				car->CloseDoors(number, 2, 0, true);
				return true;
			}
			if (StartsWith(command_name, "openextmanual", false) == true && elevator->Direction == 0)
			{
				int number = 0;
				if (command_name.length() > 13)
					number = ToInt(command_name.substr(13, command_name.length() - 13));
				return car->OpenDoors(number, 3, 0, true);
			}
			if (StartsWith(command_name, "closeextmanual", false) == true && elevator->Direction == 0)
			{
				int number = 0;
				if (command_name.length() > 14)
					number = ToInt(command_name.substr(14, command_name.length() - 14));
				car->CloseDoors(number, 3, 0, true);
				return true;
			}
			if (StartsWith(command_name, "openmanual", false) == true && elevator->Direction == 0)
			{
				int number = 0;
				if (command_name.length() > 10)
					number = ToInt(command_name.substr(10, command_name.length() - 10));
				return car->OpenDoors(number, 1, 0, true);
			}
			if (StartsWith(command_name, "closemanual", false) == true && elevator->Direction == 0)
			{
				int number = 0;
				if (command_name.length() > 11)
					number = ToInt(command_name.substr(11, command_name.length() - 11));
				car->CloseDoors(number, 1, 0, true);
				return true;
			}
			if (StartsWith(command_name, "openint", false) == true && elevator->Direction == 0)
			{
				int number = 0;
				if (command_name.length() > 7)
					number = ToInt(command_name.substr(7, command_name.length() - 7));
				return car->OpenDoors(number, 2, 0, false);
			}
			if (StartsWith(command_name, "closeint", false) == true && elevator->Direction == 0)
			{
				int number = 0;
				if (command_name.length() > 8)
					number = ToInt(command_name.substr(8, command_name.length() - 8));
				car->CloseDoors(number, 2, 0, false);
				return true;
			}
			if (StartsWith(command_name, "openext", false) == true && elevator->Direction == 0)
			{
				int number = 0;
				if (command_name.length() > 7)
					number = ToInt(command_name.substr(7, command_name.length() - 7));
				return car->OpenDoors(number, 3, car->GetFloor(), false);
			}
			if (StartsWith(command_name, "closeext", false) == true && elevator->Direction == 0)
			{
				int number = 0;
				if (command_name.length() > 8)
					number = ToInt(command_name.substr(8, command_name.length() - 8));
				car->CloseDoors(number, 3, car->GetFloor(), false);
				return true;
			}
			if (StartsWith(command_name, "open", false) == true && elevator->Direction == 0)
			{
				int number = 0;
				if (command_name.length() > 4)
					number = ToInt(command_name.substr(4, command_name.length() - 4));
				return car->OpenDoors(number);
			}
			if (StartsWith(command_name, "close", false) == true && elevator->Direction == 0)
			{
				int number = 0;
				if (command_name.length() > 5)
					number = ToInt(command_name.substr(5, command_name.length() - 5));
				car->CloseDoors(number);
				return true;
			}
			if (StartsWith(command_name, "stopdoors", false) == true && elevator->Direction == 0)
			{
				int number = 0;
				if (command_name.length() > 9)
					number = ToInt(command_name.substr(9, command_name.length() - 9));
				car->StopDoors(number);
				return true;
			}
		//}
		if (command_name == "cancel")
		{
			if (elevator->FireServicePhase2 == 1)
				return elevator->CallCancelAll();
			else if (elevator->IndependentService == true)
				return elevator->CallCancelAll();
			else
				return elevator->CallCancel();
		}
		if (command_name == "run")
		{
			elevator->SetRunState(true);
			return true;
		}
		if (command_name == "stop")
		{
			elevator->SetRunState(false);
			return true;
		}
		if (command_name == "estop")
			return elevator->Stop(true);
		if (command_name == "alarm")
		{
			car->Alarm();
			return true;
		}
		if (command_name == "fire2off")
			return elevator->EnableFireService2(0, car->Number);
		if (command_name == "fire2on")
			return elevator->EnableFireService2(1, car->Number);
		if (command_name == "fire2hold")
			return elevator->EnableFireService2(2, car->Number);
		if (command_name == "uppeakon")
			return elevator->EnableUpPeak(true);
		if (command_name == "uppeakoff")
			return elevator->EnableUpPeak(false);
		if (command_name == "downpeakon")
			return elevator->EnableDownPeak(true);
		if (command_name == "downpeakoff")
			return elevator->EnableDownPeak(false);
		if (command_name == "peakoff")
		{
			elevator->EnableDownPeak(false);
			elevator->EnableUpPeak(false);
			return true;
		}
		if (command_name == "indon")
			return elevator->EnableIndependentService(true, car->Number);
		if (command_name == "indoff")
			return elevator->EnableIndependentService(false, car->Number);
		if (command_name == "inson")
			return elevator->EnableInspectionService(true);
		if (command_name == "insoff")
			return elevator->EnableInspectionService(false);
		if (command_name == "acpon")
			return elevator->EnableACP(true);
		if (command_name == "acpoff")
			return elevator->EnableACP(false);
		if (command_name == "fanon")
		{
			car->Fan = true;
			return true;
		}
		if (command_name == "fanoff")
		{
			car->Fan = false;
			return true;
		}
		if (command_name == "musicon")
		{
			car->MusicOn = true;
			return true;
		}
		if (command_name == "musicoff")
		{
			car->MusicOn = false;
			return true;
		}
		if (command_name == "upon")
		{
			if (elevator->InspectionService == true)
				return elevator->SetUpButton(true);
			else
				return elevator->Up(true);
		}
		if (command_name == "upoff")
		{
			if (elevator->InspectionService == true)
				return elevator->SetUpButton(false);
			else
				return elevator->Up(false);
		}
		if (command_name == "downon")
		{
			if (elevator->InspectionService == true)
				return elevator->SetDownButton(true);
			else
				return elevator->Down(true);
		}
		if (command_name == "downoff")
		{
			if (elevator->InspectionService == true)
				return elevator->SetDownButton(false);
			else
				return elevator->Down(false);
		}
		if (elevator->InspectionService == true)
		{
			if (command_name == "insupon")
				return elevator->SetUpButton(true);
			if (command_name == "insupoff")
				return elevator->SetUpButton(false);
			if (command_name == "insdownon")
				return elevator->SetDownButton(true);
			if (command_name == "insdownoff")
				return elevator->SetDownButton(false);
		}
		if (command_name == "goon")
			return elevator->SetGoButton(true);
		if (command_name == "gooff")
			return elevator->SetGoButton(false);
		if (command_name == "return")
			return elevator->ReturnToNearestFloor();
		if (command_name == "up")
			return elevator->Up();
		if (command_name == "down")
			return elevator->Down();
		if (command_name == "interlockson")
		{
			elevator->Interlocks = true;
			return true;
		}
		if (command_name == "interlocksoff")
		{
			elevator->Interlocks = false;
			return true;
		}

		if (station)
		{
			if (command_name == "fire1off")
				return station->FireService(0);
			if (command_name == "fire1on")
				return station->FireService(1);
			if (command_name == "fire1bypass")
				return station->FireService(2);
		}

		if (StartsWith(command_name, "hold", false) == true && elevator->Direction == 0)
		{
			int number = 0;
			if (command_name.length() > 4)
				number = ToInt(command_name.substr(4, command_name.length() - 4));
			car->HoldDoors(number);
			return true;
		}
		if (StartsWith(command_name, "sensoron", false) == true)
		{
			int number = 0;
			if (command_name.length() > 8)
				number = ToInt(command_name.substr(8, command_name.length() - 8));
			car->EnableSensor(true, number);
			return true;
		}
		if (StartsWith(command_name, "sensoroff", false) == true)
		{
			int number = 0;
			if (command_name.length() > 9)
				number = ToInt(command_name.substr(9, command_name.length() - 9));
			car->EnableSensor(false, number);
			return true;
		}
		if (StartsWith(command_name, "sensorreset", false) == true && elevator->Direction == 0)
		{
			int number = 0;
			if (command_name.length() > 11)
				number = ToInt(command_name.substr(11, command_name.length() - 11));
			car->ResetDoors(number, true);
			return true;
		}
		if (StartsWith(command_name, "sensor", false) == true && elevator->Direction == 0)
		{
			int number = 0;
			if (command_name.length() > 6)
				number = ToInt(command_name.substr(6, command_name.length() - 6));
			car->OpenDoors(number);
			car->HoldDoors(number, true);
			return true;
		}
		if (StartsWith(command_name, "reset", false) == true && elevator->Direction == 0)
		{
			int number = 0;
			if (command_name.length() > 5)
				number = ToInt(command_name.substr(5, command_name.length() - 5));
			car->ResetDoors(number);
			return true;
		}
		if (command_name == "openshaftdoor")
		{
			if ((int)command_parameters.size() == 2)
			{
				int param1 = 0, param2 = 0;
				if (IsNumeric(command_parameters[0], param1) && IsNumeric(command_parameters[1], param2))
					return car->OpenDoors(param1, 3, param2, false);
			}
			return false;
		}
		if (command_name == "closeshaftdoor")
		{
			if ((int)command_parameters.size() == 2)
			{
				int param1 = 0, param2 = 0;
				if (IsNumeric(command_parameters[0], param1) && IsNumeric(command_parameters[1], param2))
				{
					car->CloseDoors(param1, 3, param2, false);
					return true;
				}
			}
			return false;
		}
		if (command_name == "openshaftdoormanual")
		{
			if ((int)command_parameters.size() == 2)
			{
				int param1 = 0, param2 = 0;
				if (IsNumeric(command_parameters[0], param1) && IsNumeric(command_parameters[1], param2))
					return car->OpenDoors(param1, 3, param2, true);
			}
			return false;
		}
		if (command_name == "closeshaftdoormanual")
		{
			if ((int)command_parameters.size() == 2)
			{
				int param1 = 0, param2 = 0;
				if (IsNumeric(command_parameters[0], param1) && IsNumeric(command_parameters[1], param2))
				{
					car->CloseDoors(param1, 3, param2, true);
					return true;
				}
			}
			return false;
		}
		if (command_name == "accessdown")
		{
			if ((int)command_parameters.size() == 1)
			{
				int param = 0;
				if (IsNumeric(command_parameters[0], param))
				{
					hold = elevator->HoistwayAccessHold;
					return elevator->SetHoistwayAccess(param, -1);
				}
			}
			return false;
		}
		if (command_name == "accessup")
		{
			if ((int)command_parameters.size() == 1)
			{
				int param = 0;
				if (IsNumeric(command_parameters[0], param))
				{
					hold = elevator->HoistwayAccessHold;
					return elevator->SetHoistwayAccess(param, 1);
				}
			}
			return false;
		}
		if (command_name == "accessoff")
		{
			if ((int)command_parameters.size() == 1)
			{
				int param = 0;
				if (IsNumeric(command_parameters[0], param))
					return elevator->SetHoistwayAccess(param, 0);
			}
			return false;
		}

		if (command_name == "input1")
			return car->Input("1");
		if (command_name == "input2")
			return car->Input("2");
		if (command_name == "input3")
			return car->Input("3");
		if (command_name == "input4")
			return car->Input("4");
		if (command_name == "input5")
			return car->Input("5");
		if (command_name == "input6")
			return car->Input("6");
		if (command_name == "input7")
			return car->Input("7");
		if (command_name == "input8")
			return car->Input("8");
		if (command_name == "input9")
			return car->Input("9");
		if (command_name == "input0")
			return car->Input("0");
		if (command_name == "inputminus")
			return car->Input("-");
		if (command_name == "inputstar")
			return car->Input("*");
		if (command_name == "inputbackspace")
			return car->Input("<");
	}

	//if parent is a call station, get parent floor object
	if (callstation)
		floor = sbs->GetFloor(callstation->GetFloor());

	//callstation-specific commands
	if (floor && callstation)
	{
		if (command_name == "off")
			return false;
		//numeric commands for station floor selections
		if (IsNumeric(command_name) == true)
		{
			int floor = ToInt(command_name);
			return callstation->SelectFloor(floor);
		}
		if (command_name == "fireoff")
			return callstation->FireService(0);
		if (command_name == "fireon")
			return callstation->FireService(1);
		if (command_name == "firebypass")
			return callstation->FireService(2);
		if (command_name == "input1")
			return callstation->Input("1");
		if (command_name == "input2")
			return callstation->Input("2");
		if (command_name == "input3")
			return callstation->Input("3");
		if (command_name == "input4")
			return callstation->Input("4");
		if (command_name == "input5")
			return callstation->Input("5");
		if (command_name == "input6")
			return callstation->Input("6");
		if (command_name == "input7")
			return callstation->Input("7");
		if (command_name == "input8")
			return callstation->Input("8");
		if (command_name == "input9")
			return callstation->Input("9");
		if (command_name == "input0")
			return callstation->Input("0");
		if (command_name == "inputminus")
			return callstation->Input("-");
		if (command_name == "inputstar")
			return callstation->Input("*");
		if (command_name == "inputbackspace")
			return callstation->Input("<");
		if (command_name == "up")
			return callstation->Call(true);
		if (command_name == "down")
			return callstation->Call(false);
		if (command_name == "pressup")
			return callstation->Press(true);
		if (command_name == "pressdown")
			return callstation->Press(false);
	}

	//escalator-specific commands
	if (escalator)
	{
		if (command_name == "forward")
		{
			escalator->SetRun(1);
			return true;
		}
		if (command_name == "reverse")
		{
			escalator->SetRun(-1);
			return true;
		}
		if (command_name == "stop")
		{
			escalator->SetRun(0);
			return true;
		}
	}

	//moving walkway-specific commands
	if (walkway)
	{
		if (command_name == "forward")
		{
			walkway->SetRun(1);
			return true;
		}
		if (command_name == "reverse")
		{
			walkway->SetRun(-1);
			return true;
		}
		if (command_name == "stop")
		{
			walkway->SetRun(0);
			return true;
		}
	}

	//cameratexture-specific commands
	if (camtex)
	{
		if (command_name == "enable")
		{
			camtex->Enabled(true);
			return true;
		}
		if (command_name == "disable")
		{
			camtex->Enabled(false);
			return true;
		}
	}

	//light-specific commands
	if (light)
	{
		if (command_name == "on")
		{
			light->Enabled(true);
			return true;
		}
		if (command_name == "off")
		{
			light->Enabled(false);
			return true;
		}
	}

	//door-specific commands
	if (door)
	{
		if (command_name == "open")
		{
			Vector3 pos = door->GetPosition();
			door->Open(pos);
			return true;
		}
		if (command_name == "close")
		{
			door->Close();
			return true;
		}
		if (command_name == "autoclose")
		{
			if ((int)command_parameters.size() == 1)
			{
				int param = 0;
				if (IsNumeric(command_parameters[0], param))
					door->AutoClose(param);
			}
			return true;
		}
	}

	//revolvingdoor-specific commands
	if (revdoor)
	{
		if (command_name == "on")
		{
			revdoor->Run(true);
			return true;
		}
		if (command_name == "off")
		{
			revdoor->Run(false);
			return true;
		}
	}

	if (command_name == "changetexture")
	{
		if ((int)command_parameters.size() == 2)
		{
			if (parent_type == "Mesh")
			{
				if (parent_name == "External" && sbs->External)
					return sbs->External->ReplaceTexture(command_parameters[0], command_parameters[1]);
				if (parent_name == "Landscape")
					return sbs->Landscape->ReplaceTexture(command_parameters[0], command_parameters[1]);
				if (parent_name == "Buildings")
					return sbs->Buildings->ReplaceTexture(command_parameters[0], command_parameters[1]);
				return false;
			}
			if (parent_type == "Floor")
			{
				if (floor)
				{
					floor->ReplaceTexture(command_parameters[0], command_parameters[1]);
					return true;
				}
				return false;
			}
			if (parent_type == "Elevator" || parent_type == "ElevatorCar")
			{
				if (elevator && car)
					return car->ReplaceTexture(command_parameters[0], command_parameters[1]);
				return false;
			}
			if (parent_type == "Shaft")
			{
				if (shaft)
				{
					shaft->ReplaceTexture(command_parameters[0], command_parameters[1]);
					return true;
				}
				return false;
			}
			if (parent_type == "Stairwell")
			{
				if (stairs)
				{
					stairs->ReplaceTexture(command_parameters[0], command_parameters[1]);
					return true;
				}
				return false;
			}
		}
		return false;
	}

	if (command_name == "playsound")
	{
		if ((int)command_parameters.size() == 2)
		{
			std::vector<Sound*> soundlist;

			if (parent_type == "SBS")
				soundlist = sbs->GetSound(command_parameters[0]);
			else if (parent_type == "Floor")
			{
				if (floor)
					soundlist = floor->GetSound(command_parameters[0]);
				else
					return false;
			}
			else if (parent_type == "Elevator" || parent_type == "ElevatorCar")
			{
				if (elevator && car)
					soundlist = car->GetSound(command_parameters[0]);
				else
					return false;
			}

			if ((int)soundlist.size() > 0)
			{
				for (size_t i = 0; i < soundlist.size(); i++)
				{
					if (soundlist[i])
					{
						soundlist[i]->SetLoopState(ToBool(command_parameters[1]));
						bool result = soundlist[i]->Play();

						if ((int)soundlist.size() == 1)
							return result;
					}
				}
				return true;
			}
		}
		return false;
	}

	if (command_name == "stopsound")
	{
		if ((int)command_parameters.size() == 1)
		{
			std::vector<Sound*> soundlist;

			if (parent_type == "SBS")
				soundlist = sbs->GetSound(command_parameters[0]);
			else if (parent_type == "Floor")
			{
				if (floor)
					soundlist = floor->GetSound(command_parameters[0]);
				else
					return false;
			}
			else if (parent_type == "Elevator" || parent_type == "ElevatorCar")
			{
				if (elevator && car)
					soundlist = car->GetSound(command_parameters[0]);
				else
					return false;
			}
			else
				return false;

			for (size_t i = 0; i < soundlist.size(); i++)
			{
				if (soundlist[i])
					soundlist[i]->Stop();
			}
			return true;
		}
		return false;
	}

	if (command_name == "teleport")
	{
		if ((int)command_parameters.size() == 3)
		{
			sbs->camera->Teleport(ToFloat(command_parameters[0]), ToFloat(command_parameters[1]), ToFloat(command_parameters[2]));
			return true;
		}
		return false;
	}

	if (command_name == "gotofloor")
	{
		if ((int)command_parameters.size() == 1)
		{
			sbs->camera->GotoFloor(ToInt(command_parameters[0]));
			return true;
		}
		return false;
	}

	return false;
}

int Action::GetParentCount()
{
	return (int)parent_objects.size();
}

const Object* Action::GetParent(int number)
{
	if (number < 0 || number >= (int)parent_objects.size())
		return 0;

	return parent_objects[number];
}

std::string Action::GetParentName(int number)
{
	if (number < 0 || number >= (int)parent_objects.size())
		return "";

	if (parent_objects[number])
		return parent_objects[number]->GetName();
	return "";
}

std::string Action::GetParentType(int number)
{
	if (number < 0 || number >= (int)parent_objects.size())
		return "";

	if (parent_objects[number])
		return parent_objects[number]->GetType();
	return "";
}

int Action::GetParameterCount()
{
	return (int)command_parameters.size();
}

std::string Action::GetParameter(int index)
{
	if (index >= 0 && index < (int)command_parameters.size())
		return command_parameters[index];
	return "";
}

bool Action::AddParent(Object *parent)
{
	if (!parent)
		return false;

	for (size_t i = 0; i < parent_objects.size(); i++)
	{
		if (parent_objects[i] == parent)
			return false;
	}

	parent_objects.emplace_back(parent);
	return true;
}

bool Action::RemoveParent(Object *parent)
{
	if (!parent)
		return false;

	for (size_t i = 0; i < parent_objects.size(); i++)
	{
		if (parent_objects[i] == parent)
		{
			parent_objects.erase(parent_objects.begin() + i);
			return true;
		}
	}
	return false;
}

}
