/* $Id$ */

/*
        Scalable Building Simulator - Action Interface Class
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
	for (int i = 0; i < parameters.size(); i++)
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

}

const char *Action::GetName()
{
	return name.c_str();
}

const char *Action::GetCommandName()
{
	return command_name.c_str();
}

bool Action::DoAction()
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

	for (int i = 0; i < parent_objects.size(); i++)
	{
		Elevator *elevator = 0;
		Floor *floor = 0;
		Shaft *shaft = 0;
		Stairs *stairs = 0;

		if (!parent_objects[i])
			continue;

		std::string parent_name = parent_objects[i]->GetName();
		std::string parent_type = parent_objects[i]->GetType();
		if (parent_type == "Floor")
			floor = (Floor*)parent_objects[i]->GetRawObject();
		if (parent_type == "Elevator")
			elevator = (Elevator*)parent_objects[i]->GetRawObject();
		if (parent_type == "Shaft")
			shaft = (Shaft*)parent_objects[i]->GetRawObject();
		if (parent_type == "Stairs")
			stairs = (Stairs*)parent_objects[i]->GetRawObject();

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

			if (command_name == "off")
				return true;

			bool mark = false;
			if (StartsWith(command_name, "openintmanual", false) == true && elevator->Direction == 0 && mark == false)
			{
				int number = 0;
				if (command_name.length() > 4)
					number = atoi(command_name.substr(4, command_name.length() - 4).c_str());
				elevator->OpenDoors(number, 2, 0, true);
				mark = true;
			}
			if (StartsWith(command_name, "closeintmanual", false) == true && elevator->Direction == 0 && mark == false)
			{
				int number = 0;
				if (command_name.length() > 5)
					number = atoi(command_name.substr(5, command_name.length() - 5).c_str());
				elevator->CloseDoors(number, 2, 0, true);
				mark = true;
			}
			if (StartsWith(command_name, "openextmanual", false) == true && elevator->Direction == 0 && mark == false)
			{
				int number = 0;
				if (command_name.length() > 4)
					number = atoi(command_name.substr(4, command_name.length() - 4).c_str());
				elevator->OpenDoors(number, 3, 0, true);
				mark = true;
			}
			if (StartsWith(command_name, "closeextmanual", false) == true && elevator->Direction == 0 && mark == false)
			{
				int number = 0;
				if (command_name.length() > 5)
					number = atoi(command_name.substr(5, command_name.length() - 5).c_str());
				elevator->CloseDoors(number, 3, 0, true);
				mark = true;
			}
			if (StartsWith(command_name, "openmanual", false) == true && elevator->Direction == 0 && mark == false)
			{
				int number = 0;
				if (command_name.length() > 4)
					number = atoi(command_name.substr(4, command_name.length() - 4).c_str());
				elevator->OpenDoors(number, 1, 0, true);
				mark = true;
			}
			if (StartsWith(command_name, "closemanual", false) == true && elevator->Direction == 0 && mark == false)
			{
				int number = 0;
				if (command_name.length() > 5)
					number = atoi(command_name.substr(5, command_name.length() - 5).c_str());
				elevator->CloseDoors(number, 1, 0, true);
				mark = true;
			}
			if (StartsWith(command_name, "openint", false) == true && elevator->Direction == 0 && mark == false)
			{
				int number = 0;
				if (command_name.length() > 4)
					number = atoi(command_name.substr(4, command_name.length() - 4).c_str());
				elevator->OpenDoors(number, 2, 0, false);
				mark = true;
			}
			if (StartsWith(command_name, "closeint", false) == true && elevator->Direction == 0 && mark == false)
			{
				int number = 0;
				if (command_name.length() > 5)
					number = atoi(command_name.substr(5, command_name.length() - 5).c_str());
				elevator->CloseDoors(number, 2, 0, false);
				mark = true;
			}
			if (StartsWith(command_name, "openext", false) == true && elevator->Direction == 0 && mark == false)
			{
				int number = 0;
				if (command_name.length() > 4)
					number = atoi(command_name.substr(4, command_name.length() - 4).c_str());
				elevator->OpenDoors(number, 3, elevator->GetFloor(), false);
				mark = true;
			}
			if (StartsWith(command_name, "closeext", false) == true && elevator->Direction == 0 && mark == false)
			{
				int number = 0;
				if (command_name.length() > 5)
					number = atoi(command_name.substr(5, command_name.length() - 5).c_str());
				elevator->CloseDoors(number, 3, elevator->GetFloor(), false);
				mark = true;
			}
			if (StartsWith(command_name, "open", false) == true && elevator->Direction == 0 && mark == false)
			{
				int number = 0;
				if (command_name.length() > 4)
					number = atoi(command_name.substr(4, command_name.length() - 4).c_str());
				elevator->OpenDoors(number);
			}
			if (StartsWith(command_name, "close", false) == true && elevator->Direction == 0 && mark == false)
			{
				int number = 0;
				if (command_name.length() > 5)
					number = atoi(command_name.substr(5, command_name.length() - 5).c_str());
				elevator->CloseDoors(number);
			}
			if (command_name == "cancel" && elevator->FireServicePhase2 == 1)
				elevator->CancelLastRoute();
			if (command_name == "run")
				elevator->SetRunState(true);
			if (command_name == "stop")
				elevator->SetRunState(false);
			if (command_name == "estop")
				elevator->Stop(true);
			if (command_name == "alarm")
				elevator->Alarm();
			if (command_name == "fire1off")
				elevator->EnableFireService1(0);
			if (command_name == "fire1on")
				elevator->EnableFireService1(1);
			if (command_name == "fire1bypass")
				elevator->EnableFireService1(2);
			if (command_name == "fire2off")
				elevator->EnableFireService2(0);
			if (command_name == "fire2on")
				elevator->EnableFireService2(1);
			if (command_name == "fire2hold")
				elevator->EnableFireService2(2);
			if (command_name == "uppeakon")
				elevator->EnableUpPeak(true);
			if (command_name == "uppeakoff")
				elevator->EnableUpPeak(false);
			if (command_name == "downpeakon")
				elevator->EnableDownPeak(true);
			if (command_name == "downpeakoff")
				elevator->EnableDownPeak(false);
			if (command_name == "indon")
				elevator->EnableIndependentService(true);
			if (command_name == "indoff")
				elevator->EnableIndependentService(false);
			if (command_name == "inson")
				elevator->EnableInspectionService(true);
			if (command_name == "insoff")
				elevator->EnableInspectionService(false);
			if (command_name == "acpon")
				elevator->EnableACP(true);
			if (command_name == "acpoff")
				elevator->EnableACP(false);
			if (command_name == "fanon")
				elevator->Fan = true;
			if (command_name == "fanoff")
				elevator->Fan = false;
			if (command_name == "musicon")
				elevator->MusicOn = true;
			if (command_name == "musicoff")
				elevator->MusicOn = false;
			if (command_name == "upon")
				elevator->SetUpButton(true);
			if (command_name == "upoff")
				elevator->SetUpButton(false);
			if (command_name == "downon")
				elevator->SetDownButton(true);
			if (command_name == "downoff")
				elevator->SetDownButton(false);
			if (command_name == "goon")
				elevator->SetGoButton(true);
			if (command_name == "gooff")
				elevator->SetGoButton(false);
			if (command_name == "return")
				elevator->ReturnToNearestFloor();

			if (StartsWith(command_name, "hold", false) == true && elevator->Direction == 0)
			{
				int number = 0;
				if (command_name.length() > 4)
					number = atoi(command_name.substr(4, command_name.length() - 4).c_str());
				elevator->HoldDoors(number);
			}
			if (command_name == "openshaftdoor")
			{
				if (command_parameters.size() == 2)
				{
					int param1 = 0, param2 = 0;
					if (IsNumeric(command_parameters[0].c_str(), param1) && IsNumeric(command_parameters[1].c_str(), param2))
						elevator->OpenDoors(param1, 3, param2, false);
				}
			}
			if (command_name == "closeshaftdoor")
			{
				if (command_parameters.size() == 2)
				{
					int param1 = 0, param2 = 0;
					if (IsNumeric(command_parameters[0].c_str(), param1) && IsNumeric(command_parameters[1].c_str(), param2))
						elevator->CloseDoors(param1, 3, param2, false);
				}
			}
			if (command_name == "openshaftdoormanual")
			{
				if (command_parameters.size() == 2)
				{
					int param1 = 0, param2 = 0;
					if (IsNumeric(command_parameters[0].c_str(), param1) && IsNumeric(command_parameters[1].c_str(), param2))
						elevator->OpenDoors(param1, 3, param2, true);
				}
			}
			if (command_name == "closeshaftdoormanual")
			{
				if (command_parameters.size() == 2)
				{
					int param1 = 0, param2 = 0;
					if (IsNumeric(command_parameters[0].c_str(), param1) && IsNumeric(command_parameters[1].c_str(), param2))
						elevator->CloseDoors(param1, 3, param2, true);
				}
			}
		}

		if (command_name == "changetexture")
		{
			if (command_parameters.size() == 2)
			{
				if (parent_type == "Mesh")
				{
					if (parent_name == "External")
						sbs->External->ReplaceTexture(command_parameters[0], command_parameters[1]);
					if (parent_name == "Landscape")
						sbs->Landscape->ReplaceTexture(command_parameters[0], command_parameters[1]);
					if (parent_name == "Buildings")
						sbs->Buildings->ReplaceTexture(command_parameters[0], command_parameters[1]);
				}

				if (parent_type == "Floor")
				{
					if (floor)
						floor->ReplaceTexture(command_parameters[0], command_parameters[1]);
					else
						return false;
				}
				if (parent_type == "Elevator")
				{
					if (elevator)
						elevator->ReplaceTexture(command_parameters[0], command_parameters[1]);
					else
						return false;
				}
				if (parent_type == "Shaft")
				{
					if (shaft)
						shaft->ReplaceTexture(command_parameters[0], command_parameters[1]);
					else
						return false;
				}
				if (parent_type == "Stairs")
				{
					if (stairs)
						stairs->ReplaceTexture(command_parameters[0], command_parameters[1]);
					else
						return false;
				}

			}
			else
				return false;
		}

		if (command_name == "playsound")
		{
			if (command_parameters.size() == 2)
			{
				std::vector<Sound*> soundlist;

				if (parent_type == "SBS")
					soundlist = sbs->GetSound(command_parameters[0].c_str());

				if (parent_type == "Floor")
				{
					if (floor)
						soundlist = floor->GetSound(command_parameters[0].c_str());
					else
						return false;
				}
				if (parent_type == "Elevator")
				{
					if (elevator)
						soundlist = elevator->GetSound(command_parameters[0].c_str());
					else
						return false;
				}

				if (soundlist.size() > 0)
				{
					std::string loop = command_parameters[1];
					SetCase(loop, false);

					for (int i = 0; i < soundlist.size(); i++)
					{
						if (soundlist[i])
						{
							if (loop == "true")
								soundlist[i]->Loop(true);
							else
								soundlist[i]->Loop(false);
							soundlist[i]->Play();
						}
					}
				}
			}
			else
				return false;
		}

		if (command_name == "stopsound")
		{
			if (command_parameters.size() == 1)
			{
				std::vector<Sound*> soundlist;

				if (parent_type == "SBS")
					soundlist = sbs->GetSound(command_parameters[0].c_str());

				if (parent_type == "Floor")
				{
					if (floor)
						soundlist = floor->GetSound(command_parameters[0].c_str());
					else
						return false;
				}
				if (parent_type == "Elevator")
				{
					if (elevator)
						soundlist = elevator->GetSound(command_parameters[0].c_str());
					else
						return false;
				}

				for (int i = 0; i < soundlist.size(); i++)
				{
					if (soundlist[i])
						soundlist[i]->Stop();
				}
			}
			else
				return false;
		}
	}

	return true;
}

int Action::GetParentCount()
{
	return parent_objects.size();
}

const Object* Action::GetParent(int number)
{
	if (number < 0 || number >= parent_objects.size())
		return 0;

	return parent_objects[number];
}

const char* Action::GetParentName(int number)
{
	if (number < 0 || number >= parent_objects.size())
		return 0;

	if (parent_objects[number])
		return parent_objects[number]->GetName();
	return 0;
}

const char* Action::GetParentType(int number)
{
	if (number < 0 || number >= parent_objects.size())
		return 0;

	if (parent_objects[number])
		return parent_objects[number]->GetType();
	return 0;
}

int Action::GetParameterCount()
{
	return command_parameters.size();
}

const char *Action::GetParameter(int index)
{
	if (index >= 0 && index < command_parameters.size())
		return command_parameters[index].c_str();
	return 0;
}
