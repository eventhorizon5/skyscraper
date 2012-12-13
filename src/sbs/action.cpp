/* $Id$ */

/*
        Scalable Building Simulator - Action Interface Class
        The Skyscraper Project - Version 1.8 Alpha
        Copyright (C)2004-2012 Ryan Thoryk
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

Action::Action(const std::string name, Object *action_parent, const std::string &command, const std::vector<std::string> &parameters)
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
	parent_object = action_parent;
}

Action::Action(const std::string name, Object* action_parent, const std::string &command)
{
	//create an action

	command_name = command;
	this->name = name;
	TrimString(command_name);
	SetCase(command_name, false);

	state = 0;
	parent_object = action_parent;
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

	Elevator *elevator = 0;
	Floor *floor = 0;
	Shaft *shaft = 0;
	Stairs *stairs = 0;

	std::string parent_name = parent_object->GetName();
	std::string parent_type = parent_object->GetType();
	if (parent_type == "Floor")
		floor = (Floor*)parent_object->GetRawObject();
	if (parent_type == "Elevator")
		elevator = (Elevator*)parent_object->GetRawObject();
	if (parent_type == "Shaft")
		shaft = (Shaft*)parent_object->GetRawObject();
	if (parent_type == "Stairs")
		stairs = (Stairs*)parent_object->GetRawObject();

	//report the action used
	sbs->Report("Action '" + name + "': object '" + parent_name + "' using command '" + command_name + "'");

	//numeric commands for elevator routes
	if (IsNumeric(command_name.c_str()) == true && elevator)
	{
		//exit if in inspection mode or in fire service phase 1 mode
		if (elevator->InspectionService == true || elevator->FireServicePhase1 == 1)
			return false;

		int floor = atoi(command_name.c_str());
		int elev_floor = elevator->GetFloor();

		//if elevator is processing a queue, add floor to the queue (if auto queue resets are active)
		if (elevator->IsQueueActive() && elevator->QueueResets == true)
			elevator->AddRoute(floor, elevator->QueuePositionDirection, true);
		else
		{
			//elevator is above floor
			if (elev_floor > floor)
				elevator->AddRoute(floor, -1, true);

			//elevator is below floor
			if (elev_floor < floor)
				elevator->AddRoute(floor, 1, true);

			//elevator is on floor
			if (elev_floor == floor)
			{
				if (elevator->Direction == 0)
				{
					//stopped - play chime and open doors
					if (elevator->InServiceMode() == false)
					{
						if (elevator->LastQueueDirection == -1)
							elevator->Chime(0, floor, false);
						else if (elevator->LastQueueDirection == 1)
							elevator->Chime(0, floor, true);
					}
					if (elevator->FireServicePhase2 == 0)
						elevator->OpenDoors();
				}
				else
				{
					//add a route to the current floor if elevator is moving
					elevator->AddRoute(floor, -elevator->Direction, true);
				}
			}
		}

		return true;
	}

	if (elevator)
	{
		//elevator-specific commands

		if (command_name == "off")
			return true;

		if (StartsWith(command_name, "open", false) == true && elevator->Direction == 0)
		{
			int number = 0;
			if (command_name.length() > 4)
				number = atoi(command_name.substr(4, command_name.length() - 4).c_str());
			elevator->OpenDoors(number);
		}
		if (StartsWith(command_name, "close", false) == true && elevator->Direction == 0)
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

		if (StartsWith(command_name, "hold", false) == true && elevator->Direction == 0)
		{
			int number = 0;
			if (command_name.length() > 4)
				number = atoi(command_name.substr(4, command_name.length() - 4).c_str());
			elevator->HoldDoors(number);
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
			Sound *sound = 0;

			if (parent_type == "SBS")
				sound = sbs->GetSound(command_parameters[0].c_str());

			if (parent_type == "Floor")
			{
				if (floor)
					sound = floor->GetSound(command_parameters[0].c_str());
				else
					return false;
			}
			if (parent_type == "Elevator")
			{
				if (elevator)
					sound = elevator->GetSound(command_parameters[0].c_str());
				else
					return false;
			}

			if (sound)
			{
				std::string loop = command_parameters[1];
				SetCase(loop, false);
				if (loop == "true")
					sound->Loop(true);
				else
					sound->Loop(false);
				sound->Play();
			}
		}
		else
			return false;
	}

	if (command_name == "stopsound")
	{
		if (command_parameters.size() == 1)
		{
			Sound *sound = 0;

			if (parent_type == "SBS")
				sound = sbs->GetSound(command_parameters[0].c_str());

			if (parent_type == "Floor")
			{
				if (floor)
					sound = floor->GetSound(command_parameters[0].c_str());
				else
					return false;
			}
			if (parent_type == "Elevator")
			{
				if (elevator)
					sound = elevator->GetSound(command_parameters[0].c_str());
				else
					return false;
			}

			if (sound)
				sound->Stop();
		}
		else
			return false;
	}

	return true;
}

const Object* Action::GetParent()
{
	return parent_object;
}

const char* Action::GetParentName()
{
	if (parent_object)
		return parent_object->GetName();
	return 0;
}

const char* Action::GetParentType()
{
	if (parent_object)
		return parent_object->GetType();
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
}
