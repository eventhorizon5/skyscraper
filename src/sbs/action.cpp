/* $Id$ */

/*
        Scalable Building Simulator - Action Interface Class
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

#include "globals.h"
#include "sbs.h"
#include "action.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

Action::Action(const std::string name, std::vector<Object*> &action_parents, const std::string &command, const std::vector<std::string> &parameters)
{
	//create an action

	command_name = command;
	this->name = name;
	TrimString(command_name);
	SetCase(command_name, false);

	command_parameters.resize(parameters.size());
	for (int i = 0; i < (int)parameters.size(); i++)
	{
		command_parameters[i] = parameters[i];
		TrimString(command_parameters[i]);
	}
	state = 0;
	parent_objects = action_parents;
}

Action::Action(const std::string name, std::vector<Object*> &action_parents, const std::string &command)
{
	//create an action

	command_name = command;
	this->name = name;
	TrimString(command_name);
	SetCase(command_name, false);

	state = 0;
	parent_objects = action_parents;
}

Action::~Action()
{
	if (sbs->FastDelete == false)
		sbs->Report("Deleted action '" + name + "'");
}

const char *Action::GetName()
{
	return name.c_str();
}

const char *Action::GetCommandName()
{
	return command_name.c_str();
}

bool Action::DoAction(Object *caller)
{
	//run action on all registered parents
	//returns true if at least one action succeeded

	bool result = false;
	int count = (int)parent_objects.size();
	for (int i = 0; i < count; i++)
	{
		if (!parent_objects[i])
			continue;

		bool result2 = Run(caller, parent_objects[i]);
		if (result2 == true)
			result = true;
	}
	return result;
}

bool Action::Run(Object *caller, Object *parent)
{
	//Supported action names:

	////General actions:
	//ChangeTexture  - param1: oldtexture param2: newtexture
	//PlaySound - param1: sound name, param2: loop (true/false)
	//StopSound - param1: sound name

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

	Elevator *elevator = 0;
	Floor *floor = 0;
	Shaft *shaft = 0;
	Stairs *stairs = 0;

	std::string caller_name = caller->GetName();
	std::string caller_type = caller->GetType();

	std::string parent_name = parent->GetName();
	std::string parent_type = parent->GetType();
	if (parent_type == "Floor")
		floor = (Floor*)parent->GetRawObject();
	else if (parent_type == "Elevator")
		elevator = (Elevator*)parent->GetRawObject();
	else if (parent_type == "Shaft")
		shaft = (Shaft*)parent->GetRawObject();
	else if (parent_type == "Stairs")
		stairs = (Stairs*)parent->GetRawObject();

	//report the action used
	sbs->Report("Action '" + name + "': object '" + parent_name + "' using command '" + command_name + "'");

	//numeric commands for elevator floor selections
	if (IsNumeric(command_name.c_str()) == true && elevator)
	{
		int floor = atoi(command_name.c_str());
		return elevator->SelectFloor(floor);
	}

	if (elevator)
	{
		//elevator-specific commands

		//get first callbutton on recall floor
		CallButton *callbutton = elevator->GetPrimaryCallButton();

		//if called from a control and mouse button is held down, notify elevator
		if (caller_type == "Control" && sbs->camera->MouseDown == true)
			elevator->ControlPressActive = true;

		if (command_name == "off")
			return true;

		if (!(elevator->FireServicePhase1 == 1 && elevator->FireServicePhase2 == 0))
		{
			if (StartsWith(command_name, "openintmanual", false) == true && elevator->Direction == 0)
			{
				int number = 0;
				if (command_name.length() > 13)
					number = atoi(command_name.substr(13, command_name.length() - 13).c_str());
				return elevator->OpenDoors(number, 2, 0, true);
			}
			if (StartsWith(command_name, "closeintmanual", false) == true && elevator->Direction == 0)
			{
				int number = 0;
				if (command_name.length() > 14)
					number = atoi(command_name.substr(14, command_name.length() - 14).c_str());
				elevator->CloseDoors(number, 2, 0, true);
				return true;
			}
			if (StartsWith(command_name, "openextmanual", false) == true && elevator->Direction == 0)
			{
				int number = 0;
				if (command_name.length() > 13)
					number = atoi(command_name.substr(13, command_name.length() - 13).c_str());
				return elevator->OpenDoors(number, 3, 0, true);
			}
			if (StartsWith(command_name, "closeextmanual", false) == true && elevator->Direction == 0)
			{
				int number = 0;
				if (command_name.length() > 14)
					number = atoi(command_name.substr(14, command_name.length() - 14).c_str());
				elevator->CloseDoors(number, 3, 0, true);
				return true;
			}
			if (StartsWith(command_name, "openmanual", false) == true && elevator->Direction == 0)
			{
				int number = 0;
				if (command_name.length() > 10)
					number = atoi(command_name.substr(10, command_name.length() - 10).c_str());
				return elevator->OpenDoors(number, 1, 0, true);
			}
			if (StartsWith(command_name, "closemanual", false) == true && elevator->Direction == 0)
			{
				int number = 0;
				if (command_name.length() > 11)
					number = atoi(command_name.substr(11, command_name.length() - 11).c_str());
				elevator->CloseDoors(number, 1, 0, true);
				return true;
			}
			if (StartsWith(command_name, "openint", false) == true && elevator->Direction == 0)
			{
				int number = 0;
				if (command_name.length() > 7)
					number = atoi(command_name.substr(7, command_name.length() - 7).c_str());
				return elevator->OpenDoors(number, 2, 0, false);
			}
			if (StartsWith(command_name, "closeint", false) == true && elevator->Direction == 0)
			{
				int number = 0;
				if (command_name.length() > 8)
					number = atoi(command_name.substr(8, command_name.length() - 8).c_str());
				elevator->CloseDoors(number, 2, 0, false);
				return true;
			}
			if (StartsWith(command_name, "openext", false) == true && elevator->Direction == 0)
			{
				int number = 0;
				if (command_name.length() > 7)
					number = atoi(command_name.substr(7, command_name.length() - 7).c_str());
				return elevator->OpenDoors(number, 3, elevator->GetFloor(), false);
			}
			if (StartsWith(command_name, "closeext", false) == true && elevator->Direction == 0)
			{
				int number = 0;
				if (command_name.length() > 8)
					number = atoi(command_name.substr(8, command_name.length() - 8).c_str());
				elevator->CloseDoors(number, 3, elevator->GetFloor(), false);
				return true;
			}
			if (StartsWith(command_name, "open", false) == true && elevator->Direction == 0)
			{
				int number = 0;
				if (command_name.length() > 4)
					number = atoi(command_name.substr(4, command_name.length() - 4).c_str());
				return elevator->OpenDoors(number);
			}
			if (StartsWith(command_name, "close", false) == true && elevator->Direction == 0)
			{
				int number = 0;
				if (command_name.length() > 5)
					number = atoi(command_name.substr(5, command_name.length() - 5).c_str());
				elevator->CloseDoors(number);
				return true;
			}
			if (StartsWith(command_name, "stopdoors", false) == true && elevator->Direction == 0)
			{
				int number = 0;
				if (command_name.length() > 9)
					number = atoi(command_name.substr(9, command_name.length() - 9).c_str());
				elevator->StopDoors(number);
				return true;
			}
		}
		if (command_name == "cancel" && elevator->FireServicePhase2 == 1)
			return elevator->CallCancel();
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
			elevator->Alarm();
			return true;
		}
		if (command_name == "fire2off")
			return elevator->EnableFireService2(0);
		if (command_name == "fire2on")
			return elevator->EnableFireService2(1);
		if (command_name == "fire2hold")
			return elevator->EnableFireService2(2);
		if (command_name == "uppeakon")
			return elevator->EnableUpPeak(true);
		if (command_name == "uppeakoff")
			return elevator->EnableUpPeak(false);
		if (command_name == "downpeakon")
			return elevator->EnableDownPeak(true);
		if (command_name == "downpeakoff")
			return elevator->EnableDownPeak(false);
		if (command_name == "indon")
			return elevator->EnableIndependentService(true);
		if (command_name == "indoff")
			return elevator->EnableIndependentService(false);
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
			elevator->Fan = true;
			return true;
		}
		if (command_name == "fanoff")
		{
			elevator->Fan = false;
			return true;
		}
		if (command_name == "musicon")
		{
			elevator->MusicOn = true;
			return true;
		}
		if (command_name == "musicoff")
		{
			elevator->MusicOn = false;
			return true;
		}
		if (command_name == "upon")
			return elevator->SetUpButton(true);
		if (command_name == "upoff")
			return elevator->SetUpButton(false);
		if (command_name == "downon")
			return elevator->SetDownButton(true);
		if (command_name == "downoff")
			return elevator->SetDownButton(false);
		if (command_name == "goon")
			return elevator->SetGoButton(true);
		if (command_name == "gooff")
			return elevator->SetGoButton(false);
		if (command_name == "return")
			return elevator->ReturnToNearestFloor();
		if (command_name == "up")
		{
			elevator->Up();
			return true;
		}
		if (command_name == "down")
		{
			elevator->Down();
			return true;
		}
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

		if (callbutton)
		{
			if (command_name == "fire1off")
			{
				callbutton->FireService(0);
				return true;
			}
			if (command_name == "fire1on")
			{
				callbutton->FireService(1);
				return true;
			}
			if (command_name == "fire1bypass")
			{
				callbutton->FireService(2);
				return true;
			}
		}

		if (StartsWith(command_name, "hold", false) == true && elevator->Direction == 0)
		{
			int number = 0;
			if (command_name.length() > 4)
				number = atoi(command_name.substr(4, command_name.length() - 4).c_str());
			elevator->HoldDoors(number);
			return true;
		}
		if (StartsWith(command_name, "sensoron", false) == true)
		{
			int number = 0;
			if (command_name.length() > 8)
				number = atoi(command_name.substr(8, command_name.length() - 8).c_str());
			elevator->EnableSensor(true, number);
			return true;
		}
		if (StartsWith(command_name, "sensoroff", false) == true)
		{
			int number = 0;
			if (command_name.length() > 9)
				number = atoi(command_name.substr(9, command_name.length() - 9).c_str());
			elevator->EnableSensor(false, number);
			return true;
		}
		if (StartsWith(command_name, "sensorreset", false) == true && elevator->Direction == 0)
		{
			int number = 0;
			if (command_name.length() > 11)
				number = atoi(command_name.substr(11, command_name.length() - 11).c_str());
			elevator->ResetDoors(number, true);
			return true;
		}
		if (StartsWith(command_name, "sensor", false) == true && elevator->Direction == 0)
		{
			int number = 0;
			if (command_name.length() > 6)
				number = atoi(command_name.substr(6, command_name.length() - 6).c_str());
			elevator->OpenDoors(number);
			elevator->HoldDoors(number, true);
			return true;
		}
		if (StartsWith(command_name, "reset", false) == true && elevator->Direction == 0)
		{
			int number = 0;
			if (command_name.length() > 5)
				number = atoi(command_name.substr(5, command_name.length() - 5).c_str());
			elevator->ResetDoors(number);
			return true;
		}
		if (command_name == "openshaftdoor")
		{
			if ((int)command_parameters.size() == 2)
			{
				int param1 = 0, param2 = 0;
				if (IsNumeric(command_parameters[0].c_str(), param1) && IsNumeric(command_parameters[1].c_str(), param2))
					return elevator->OpenDoors(param1, 3, param2, false);
			}
			return false;
		}
		if (command_name == "closeshaftdoor")
		{
			if ((int)command_parameters.size() == 2)
			{
				int param1 = 0, param2 = 0;
				if (IsNumeric(command_parameters[0].c_str(), param1) && IsNumeric(command_parameters[1].c_str(), param2))
				{
					elevator->CloseDoors(param1, 3, param2, false);
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
				if (IsNumeric(command_parameters[0].c_str(), param1) && IsNumeric(command_parameters[1].c_str(), param2))
					return elevator->OpenDoors(param1, 3, param2, true);
			}
			return false;
		}
		if (command_name == "closeshaftdoormanual")
		{
			if ((int)command_parameters.size() == 2)
			{
				int param1 = 0, param2 = 0;
				if (IsNumeric(command_parameters[0].c_str(), param1) && IsNumeric(command_parameters[1].c_str(), param2))
				{
					elevator->CloseDoors(param1, 3, param2, true);
					return true;
				}
			}
			return false;
		}
	}

	if (command_name == "changetexture")
	{
		if ((int)command_parameters.size() == 2)
		{
			if (parent_type == "Mesh")
			{
				if (parent_name == "External")
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
			if (parent_type == "Elevator")
			{
				if (elevator)
					return elevator->ReplaceTexture(command_parameters[0], command_parameters[1]);
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
			if (parent_type == "Stairs")
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
				soundlist = sbs->GetSound(command_parameters[0].c_str());
			else if (parent_type == "Floor")
			{
				if (floor)
					soundlist = floor->GetSound(command_parameters[0].c_str());
				else
					return false;
			}
			else if (parent_type == "Elevator")
			{
				if (elevator)
					soundlist = elevator->GetSound(command_parameters[0].c_str());
				else
					return false;
			}

			if ((int)soundlist.size() > 0)
			{
				std::string loop = command_parameters[1];
				SetCase(loop, false);

				for (int i = 0; i < (int)soundlist.size(); i++)
				{
					if (soundlist[i])
					{
						if (loop == "true")
							soundlist[i]->Loop(true);
						else
							soundlist[i]->Loop(false);
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
				soundlist = sbs->GetSound(command_parameters[0].c_str());
			else if (parent_type == "Floor")
			{
				if (floor)
					soundlist = floor->GetSound(command_parameters[0].c_str());
				else
					return false;
			}
			else if (parent_type == "Elevator")
			{
				if (elevator)
					soundlist = elevator->GetSound(command_parameters[0].c_str());
				else
					return false;
			}
			else
				return false;

			for (int i = 0; i < (int)soundlist.size(); i++)
			{
				if (soundlist[i])
					soundlist[i]->Stop();
			}
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

const char* Action::GetParentName(int number)
{
	if (number < 0 || number >= (int)parent_objects.size())
		return "";

	if (parent_objects[number])
		return parent_objects[number]->GetName();
	return "";
}

const char* Action::GetParentType(int number)
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

const char *Action::GetParameter(int index)
{
	if (index >= 0 && index < (int)command_parameters.size())
		return command_parameters[index].c_str();
	return "";
}

bool Action::AddParent(Object *parent)
{
	if (!parent)
		return false;

	for (int i = 0; i < (int)parent_objects.size(); i++)
	{
		if (parent_objects[i] == parent)
			return false;
	}

	parent_objects.push_back(parent);
	return true;
}

bool Action::RemoveParent(Object *parent)
{
	if (!parent)
		return false;

	for (int i = 0; i < (int)parent_objects.size(); i++)
	{
		if (parent_objects[i] == parent)
		{
			parent_objects.erase(parent_objects.begin() + i);
			return true;
		}
	}
	return false;
}
