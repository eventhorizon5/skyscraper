/*
	Skyscraper 2.1 - Script Processor - Elevator Car Section
	Copyright (C)2003-2025 Ryan Thoryk
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
#include "enginecontext.h"
#include "elevator.h"
#include "elevatorcar.h"
#include "elevatordoor.h"
#include "trigger.h"
#include "control.h"
#include "door.h"
#include "buttonpanel.h"
#include "directional.h"
#include "floorindicator.h"
#include "model.h"
#include "sound.h"
#include "reverb.h"
#include "wall.h"
#include "scriptproc.h"
#include "section.h"
#include "indicator.h"

using namespace SBS;

namespace Skyscraper {

ScriptProcessor::ElevatorCarSection::ElevatorCarSection(ScriptProcessor *parent) : Section(parent)
{

}

int ScriptProcessor::ElevatorCarSection::Run(std::string &LineData)
{
	//Process elevator cars

	//get car object
	ElevatorCar *car = 0;
	Elevator *elev = 0;
	if (config->SectionNum == 6)
	{
		elev = Simcore->GetElevator(config->CurrentOld);
		if (elev)
			car = elev->GetCar(config->Current);
	}
	else
	{
		//get default car if not in a separate car section
		elev = Simcore->GetElevator(config->Current);
		if (elev)
			car = elev->GetCar(1);
	}

	if (!elev)
		return sError;

	//create car if not created already
	if (!car)
		car = elev->AddCar();

	if (!car)
		return sError;

	//replace variables with actual values
	if (config->SectionNum == 6) //only run if not being called from elevator function
	{
		ReplaceAll(LineData, "%elevator%", ToString(config->CurrentOld));
		ReplaceAll(LineData, "%car%", ToString(config->Current));

		//IF/While statement stub (continue to global commands for processing)
		if (StartsWithNoCase(LineData, "if") || StartsWithNoCase(LineData, "while"))
			return sContinue;

		//process math functions
		if (MathFunctions(LineData) == sError)
			return sError;

		//process functions
		if (parent->FunctionProc() == true)
			return sNextLine;
	}

	//get text after equal sign
	bool equals;
	std::string value = GetAfterEquals(LineData, equals);

	//parameters

	//Name parameter
	if (StartsWithNoCase(LineData, "name"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		car->Name = value;
		return sNextLine;
	}
	//OpenSpeed parameter
	if (StartsWithNoCase(LineData, "openspeed"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (car->Created == false)
			return ScriptError("Car not created yet");
		std::string str = GetBeforeEquals(LineData);
		int door = 0;
		if (!IsNumeric(str, door))
			return ScriptError("No door specified");
		if (door == 0 || door > car->NumDoors)
			return ScriptError("Invalid door number");
		std::string str2 = Calc(value);
		if (!IsNumeric(str2, car->GetDoor(door)->OpenSpeed))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	//Doors parameter
	if (StartsWithNoCase(LineData, "doors "))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, car->NumDoors))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	//ServicedFloors parameter
	if (StartsWithNoCase(LineData, "servicedfloors"))
	{
		//copy string listing of serviced floors into array
		int params = SplitAfterEquals(LineData, false);
		if (params < 1)
			return ScriptError("Syntax Error");

		for (int line = 0; line < params; line++)
		{
			int start, end;
			if (GetRange(tempdata[line], start, end) == true)
			{
				for (int k = start; k <= end; k++)
				{
					if (!car->AddServicedFloor(k))
						return ScriptError();
				}
			}
			else
			{
				int data;
				std::string str = Calc(tempdata[line]);
				if (!IsNumeric(str, data))
					return ScriptError("Invalid value");

				if (!car->AddServicedFloor(data))
					return ScriptError();
			}
		}
		return sNextLine;
	}
	//DisplayFloors parameter
	if (StartsWithNoCase(LineData, "displayfloors"))
	{
		//copy string listing of serviced floors into array
		int params = SplitAfterEquals(LineData, false);
		if (params < 1)
			return ScriptError("Syntax Error");

		for (int line = 0; line < params; line++)
		{
			int start, end;
			if (GetRange(tempdata[line], start, end) == true)
			{
				for (int k = start; k <= end; k++)
				{
					car->AddDisplayFloor(k);
				}
			}
			else
			{
				int data;
				if (!IsNumeric(tempdata[line], data))
					return ScriptError("Invalid value");
				car->AddDisplayFloor(data);
			}
		}
		return sNextLine;
	}
	//DoorTimer parameter
	if (StartsWithNoCase(LineData, "doortimer"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (car->Created == false)
			return ScriptError("Car not created yet");
		std::string str = GetBeforeEquals(LineData);
		int door = 0;
		if (!IsNumeric(str, door))
			return ScriptError("No door specified");
		if (door == 0 || door > car->NumDoors)
			return ScriptError("Invalid door number");
		std::string str2 = Calc(value);
		if (!IsNumeric(str2, car->GetDoor(door)->DoorTimer))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	//QuickClose parameter
	if (StartsWithNoCase(LineData, "quickclose"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (car->Created == false)
			return ScriptError("Car not created yet");
		std::string str = GetBeforeEquals(LineData);
		int door = 0;
		if (!IsNumeric(str, door))
			return ScriptError("No door specified");
		if (door == 0 || door > car->NumDoors)
			return ScriptError("Invalid door number");
		std::string str2 = Calc(value);
		if (!IsNumeric(str2, car->GetDoor(door)->QuickClose))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	//NudgeTimer parameter
	if (StartsWithNoCase(LineData, "nudgetimer"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (car->Created == false)
			return ScriptError("Car not created yet");
		std::string str = GetBeforeEquals(LineData);
		int door = 0;
		if (!IsNumeric(str, door))
			return ScriptError("No door specified");
		if (door == 0 || door > car->NumDoors)
			return ScriptError("Invalid door number");
		std::string str2 = Calc(value);
		if (!IsNumeric(str2, car->GetDoor(door)->NudgeTimer))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	//SlowSpeed parameter
	if (StartsWithNoCase(LineData, "slowspeed"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (car->Created == false)
			return ScriptError("Car not created yet");
		std::string str = GetBeforeEquals(LineData);
		int door = 0;
		if (!IsNumeric(str, door))
			return ScriptError("No door specified");
		if (door == 0 || door > car->NumDoors)
			return ScriptError("Invalid door number");
		std::string str2 = Calc(value);
		if (!IsNumeric(str2, car->GetDoor(door)->SlowSpeed))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	//ManualSpeed parameter
	if (StartsWithNoCase(LineData, "manualspeed"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (car->Created == false)
			return ScriptError("Car not created yet");
		std::string str = GetBeforeEquals(LineData);
		int door = 0;
		if (!IsNumeric(str, door))
			return ScriptError("No door specified");
		if (door == 0 || door > car->NumDoors)
			return ScriptError("Invalid door number");
		std::string str2 = Calc(value);
		if (!IsNumeric(str2, car->GetDoor(door)->ManualSpeed))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	//OpenSound parameter
	if (StartsWithNoCase(LineData, "opensound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (car->Created == false)
			return ScriptError("Car not created yet");
		std::string str = GetBeforeEquals(LineData);
		int door = 0;
		if (!IsNumeric(str, door))
			return ScriptError("No door specified");
		if (door == 0 || door > car->NumDoors)
			return ScriptError("Invalid door number");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->GetDoor(door)->OpenSound = value;
		return sNextLine;
	}
	//CloseSound parameter
	if (StartsWithNoCase(LineData, "closesound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (car->Created == false)
			return ScriptError("Car not created yet");
		std::string str = GetBeforeEquals(LineData);
		int door = 0;
		if (!IsNumeric(str, door))
			return ScriptError("No door specified");
		if (door == 0 || door > car->NumDoors)
			return ScriptError("Invalid door number");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->GetDoor(door)->CloseSound = value;
		return sNextLine;
	}
	//NudgeSound parameter
	if (StartsWithNoCase(LineData, "nudgesound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (car->Created == false)
			return ScriptError("Car not created yet");
		std::string str = GetBeforeEquals(LineData);
		int door = 0;
		if (!IsNumeric(str, door))
			return ScriptError("No door specified");
		if (door == 0 || door > car->NumDoors)
			return ScriptError("Invalid door number");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->GetDoor(door)->NudgeSound = value;
		return sNextLine;
	}
	//StartSound parameter
	if (StartsWithNoCase(LineData, "startsound"))
	{
		//backwards compatibility
		if (equals == false)
			return ScriptError("Syntax error");

		if (warn_deprecated == true)
			ScriptWarning("Command deprecated");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->UpStartSound = value;
		car->DownStartSound = value;

		//turn off motor sounds
		elev->MotorUpStartSound = "";
		elev->MotorDownStartSound = "";
		elev->MotorUpRunSound = "";
		elev->MotorDownRunSound = "";
		elev->MotorUpStopSound = "";
		elev->MotorDownStopSound = "";
		elev->MotorIdleSound = "";
		return sNextLine;
	}
	//MoveSound parameter
	if (StartsWithNoCase(LineData, "movesound"))
	{
		//backwards compatibility
		if (equals == false)
			return ScriptError("Syntax error");

		if (warn_deprecated == true)
			ScriptWarning("Command deprecated");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->UpMoveSound = value;
		car->DownMoveSound = value;

		//turn off motor sounds
		elev->MotorUpStartSound = "";
		elev->MotorDownStartSound = "";
		elev->MotorUpRunSound = "";
		elev->MotorDownRunSound = "";
		elev->MotorUpStopSound = "";
		elev->MotorDownStopSound = "";
		elev->MotorIdleSound = "";
		return sNextLine;
	}
	//StopSound parameter
	if (StartsWithNoCase(LineData, "stopsound"))
	{
		//backwards compatibility
		if (equals == false)
			return ScriptError("Syntax error");

		if (warn_deprecated == true)
			ScriptWarning("Command deprecated");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->UpStopSound = value;
		car->DownStopSound = value;

		//turn off motor sounds
		elev->MotorUpStartSound = "";
		elev->MotorDownStartSound = "";
		elev->MotorUpRunSound = "";
		elev->MotorDownRunSound = "";
		elev->MotorUpStopSound = "";
		elev->MotorDownStopSound = "";
		elev->MotorIdleSound = "";
		return sNextLine;
	}
	//IdleSound parameter
	if (StartsWithNoCase(LineData, "idlesound"))
	{
		//backwards compatibility
		if (equals == false)
			return ScriptError("Syntax error");

		if (warn_deprecated == true)
			ScriptWarning("Command deprecated");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->IdleSound = value;

		//turn off motor sounds
		elev->MotorUpStartSound = "";
		elev->MotorDownStartSound = "";
		elev->MotorUpRunSound = "";
		elev->MotorDownRunSound = "";
		elev->MotorUpStopSound = "";
		elev->MotorDownStopSound = "";
		elev->MotorIdleSound = "";
		return sNextLine;
	}
	//CarStartSound parameter
	if (StartsWithNoCase(LineData, "carstartsound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->UpStartSound = value;
		car->DownStartSound = value;
		return sNextLine;
	}
	//CarUpStartSound parameter
	if (StartsWithNoCase(LineData, "carupstartsound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->UpStartSound = value;
		return sNextLine;
	}
	//CarDownStartSound parameter
	if (StartsWithNoCase(LineData, "cardownstartsound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->DownStartSound = value;
		return sNextLine;
	}
	//CarMoveSound parameter
	if (StartsWithNoCase(LineData, "carmovesound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->UpMoveSound = value;
		car->DownMoveSound = value;
		return sNextLine;
	}
	//CarUpMoveSound parameter
	if (StartsWithNoCase(LineData, "carupmovesound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->UpMoveSound = value;
		return sNextLine;
	}
	//CarDownMoveSound parameter
	if (StartsWithNoCase(LineData, "cardownmovesound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->DownMoveSound = value;
		return sNextLine;
	}
	//CarStopSound parameter
	if (StartsWithNoCase(LineData, "carstopsound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->UpStopSound = value;
		car->DownStopSound = value;
		return sNextLine;
	}
	//CarUpStopSound parameter
	if (StartsWithNoCase(LineData, "carupstopsound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->UpStopSound = value;
		return sNextLine;
	}
	//CarDownStopSound parameter
	if (StartsWithNoCase(LineData, "cardownstopsound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->DownStopSound = value;
		return sNextLine;
	}
	//CarIdleSound parameter
	if (StartsWithNoCase(LineData, "caridlesound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->IdleSound = value;
		return sNextLine;
	}
	//ChimeSound parameter
	if (StartsWithNoCase(LineData, "chimesound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (car->Created == false)
			return ScriptError("Car not created yet");
		std::string str = GetBeforeEquals(LineData);
		int door = 0;
		if (!IsNumeric(str, door))
			return ScriptError("No door specified");
		if (door == 0 || door > car->NumDoors)
			return ScriptError("Invalid door number");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->GetDoor(door)->UpChimeSound = value;
		car->GetDoor(door)->DownChimeSound = value;
		return sNextLine;
	}
	//UpChimeSound parameter
	if (StartsWithNoCase(LineData, "upchimesound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (car->Created == false)
			return ScriptError("Car not created yet");
		std::string str = GetBeforeEquals(LineData);
		int door = 0;
		if (!IsNumeric(str, door))
			return ScriptError("No door specified");
		if (door == 0 || door > car->NumDoors)
			return ScriptError("Invalid door number");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->GetDoor(door)->UpChimeSound = value;
		return sNextLine;
	}
	//DownChimeSound parameter
	if (StartsWithNoCase(LineData, "downchimesound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (car->Created == false)
			return ScriptError("Car not created yet");
		std::string str = GetBeforeEquals(LineData);
		int door = 0;
		if (!IsNumeric(str, door))
			return ScriptError("No door specified");
		if (door == 0 || door > car->NumDoors)
			return ScriptError("Invalid door number");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->GetDoor(door)->DownChimeSound = value;
		return sNextLine;
	}
	//EarlyUpChimeSound parameter
	if (StartsWithNoCase(LineData, "earlyupchimesound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (car->Created == false)
			return ScriptError("Car not created yet");
		std::string str = GetBeforeEquals(LineData);
		int door = 0;
		if (!IsNumeric(str, door))
			return ScriptError("No door specified");
		if (door == 0 || door > car->NumDoors)
			return ScriptError("Invalid door number");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->GetDoor(door)->EarlyUpChimeSound = value;
		car->GetDoor(door)->EarlyUpSet = true;
		return sNextLine;
	}
	//EarlyDownChimeSound parameter
	if (StartsWithNoCase(LineData, "earlydownchimesound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (car->Created == false)
			return ScriptError("Car not created yet");
		std::string str = GetBeforeEquals(LineData);
		int door = 0;
		if (!IsNumeric(str, door))
			return ScriptError("No door specified");
		if (door == 0 || door > car->NumDoors)
			return ScriptError("Invalid door number");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->GetDoor(door)->EarlyDownChimeSound = value;
		car->GetDoor(door)->EarlyDownSet = true;
		return sNextLine;
	}
	//AlarmSoundStop parameter
	if (StartsWithNoCase(LineData, "alarmsoundstop"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->AlarmSoundStop = value;
		return sNextLine;
	}
	//AlarmSound parameter
	if (StartsWithNoCase(LineData, "alarmsound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->AlarmSound = value;
		return sNextLine;
	}
	//BeepSound parameter
	if (StartsWithNoCase(LineData, "beepsound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->SetBeepSound(value);
		return sNextLine;
	}
	//FloorSound parameter
	if (StartsWithNoCase(LineData, "floorsound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->SetFloorSound(value);
		return sNextLine;
	}
	//UpMessage parameter
	if (StartsWithNoCase(LineData, "upmessage"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->SetMessageSound(true, true, value);
		return sNextLine;
	}
	//DownMessage parameter
	if (StartsWithNoCase(LineData, "downmessage"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->SetMessageSound(true, false, value);
		return sNextLine;
	}
	//MessageOnMove parameter
	if (StartsWithNoCase(LineData, "messageonmove"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		car->MessageOnMove = ToBool(value);
		return sNextLine;
	}
	//MessageOnStart parameter
	if (StartsWithNoCase(LineData, "messageonstart"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		car->MessageOnStart = ToBool(value);
		return sNextLine;
	}
	//MessageOnClose parameter
	if (StartsWithNoCase(LineData, "messageonclose"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		car->MessageOnClose = ToBool(value);
		return sNextLine;
	}
	//OpenMessage parameter
	if (StartsWithNoCase(LineData, "openmessage"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->SetMessageSound(false, true, value);
		return sNextLine;
	}
	//CloseMessage parameter
	if (StartsWithNoCase(LineData, "closemessage"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->SetMessageSound(false, false, value);
		return sNextLine;
	}
	//Music parameter
	if (StartsWithNoCase(LineData, "music "))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->MusicUp = value;
		car->MusicDown = value;
		return sNextLine;
	}
	//MusicUp parameter
	if (StartsWithNoCase(LineData, "musicup"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->MusicUp = value;
		return sNextLine;
	}
	//MusicDown parameter
	if (StartsWithNoCase(LineData, "musicdown"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->MusicDown = value;
		return sNextLine;
	}
	//MusicOn parameter
	if (StartsWithNoCase(LineData, "musicon "))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		car->MusicOn = ToBool(value);
		return sNextLine;
	}
	//MusicAlwaysOn parameter
	if (StartsWithNoCase(LineData, "musicalwayson"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		car->MusicAlwaysOn = ToBool(value);
		return sNextLine;
	}
	//MusicOnMove parameter
	if (StartsWithNoCase(LineData, "musiconmove"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		car->MusicOnMove = ToBool(value);
	}
	//MusicPosition parameter
	if (StartsWithNoCase(LineData, "musicposition"))
	{
		int params = SplitAfterEquals(LineData);
		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 2; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		car->MusicPosition = Vector3(ToFloat(tempdata[0]), ToFloat(tempdata[1]), ToFloat(tempdata[2]));
		return sNextLine;
	}
	//InvalidInput parameter
	if (StartsWithNoCase(LineData, "invalidinput"))
	{
		//copy string listing of elevators into array
		int params = SplitAfterEquals(LineData);
		if (params < 1)
			return ScriptError("Syntax Error");

		std::vector<std::string> inputs;

		for (int line = 0; line < params; line++)
			inputs.emplace_back(tempdata[line]);

		car->InvalidInput = inputs;

		return sNextLine;
	}
	//TimerDelay parameter
	if (StartsWithNoCase(LineData, "timerdelay"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		float num = 0;
		if (!IsNumeric(str, num))
			return ScriptError("Invalid value");
		car->TimerDelay = num;
		return sNextLine;
	}
	//AutoEnable parameter
	if (StartsWithNoCase(LineData, "autoenable"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		car->AutoEnable = ToBool(value);
		return sNextLine;
	}
	//DoorSensor parameter
	if (StartsWithNoCase(LineData, "doorsensor"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (car->Created == false)
			return ScriptError("Car not created yet");

		std::string str = GetBeforeEquals(LineData);
		int door = 0;
		if (!IsNumeric(str, door))
			return ScriptError("No door specified");
		if (door == 0 || door > car->NumDoors)
			return ScriptError("Invalid door number");

		int params = SplitAfterEquals(LineData);
		if (params < 1 || params > 2)
			return ScriptError("Incorrect number of parameters");

		car->GetDoor(door)->EnableSensor(ToBool(tempdata[0]));
		if (params == 2)
		{
			//check to see if file exists
			parent->CheckFile("data/" + tempdata[1]);
			//set door sound
			car->GetDoor(door)->SensorSound = tempdata[1];
		}
		return sNextLine;
	}
	//CarEmergencyStopSound parameter
	if (StartsWithNoCase(LineData, "caremergencystopsound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->EmergencyStopSound = value;
		return sNextLine;
	}
	//IndependentService parameter
	if (StartsWithNoCase(LineData, "independentservice"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		elev->IndependentService = ToBool(value);
		elev->IndependentServiceCar = car->Number;
		return sNextLine;
	}
	//FireService2 parameter
	if (StartsWithNoCase(LineData, "fireservice2"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		std::string str = Calc(value);
		int mode;
		if (!IsNumeric(str, mode))
			return ScriptError("Invalid value: " + str);

		elev->FireServicePhase2 = mode;
		elev->FireServicePhase2Car = car->Number;
		return sNextLine;
	}

	//AddFloor command
	if (StartsWithNoCase(LineData, "addfloor "))
	{
		//get data
		int params = SplitData(LineData, 9);

		if (params != 11 && params != 13)
			return ScriptError("Incorrect number of parameters");

		bool compat = false;
		if (params == 11)
			compat = true;

		//check numeric values
		if (compat == true)
		{
			for (int i = 2; i <= 10; i++)
			{
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
		}
		else
		{
			for (int i = 2; i <= 12; i++)
			{
				if (i == 9)
					i = 11;
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//create floor
		if (compat == true)
			StoreCommand(car->AddFloor(tempdata[0], tempdata[1], ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), config->ReverseAxis, false, ToFloat(tempdata[9]), ToFloat(tempdata[10]), true));
		else
			StoreCommand(car->AddFloor(tempdata[0], tempdata[1], ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToBool(tempdata[9]), ToBool(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12])));
		return sNextLine;
	}

	//AddWall command
	if (StartsWithNoCase(LineData, "addwall"))
	{
		//get data
		int params = SplitData(LineData, 8);

		if (params != 13)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 12; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//create wall
		StoreCommand(car->AddWall(tempdata[0], tempdata[1], ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12])));
		return sNextLine;
	}

	//AddDoors command
	if (StartsWithNoCase(LineData, "adddoors"))
	{
		//get data
		int params = SplitData(LineData, 9);

		if (params != 10 && params != 11)
			return ScriptError("Incorrect number of parameters");

		bool compat = false;
		if (params == 10)
			compat = true; //1.4 compatibility mode

		//check numeric values
		if (compat == true)
		{
			for (int i = 0; i <= 9; i++)
			{
				if (i == 1)
					i = 2;
				if (i == 7)
					i = 8;
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
		}
		else
		{
			for (int i = 0; i <= 10; i++)
			{
				if (i == 1)
					i = 3;
				if (i == 8)
					i = 9;
			if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		if (compat == false)
			StoreCommand(car->AddDoors(ToInt(tempdata[0]), tempdata[1], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToBool(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10])));
		else
			StoreCommand(car->AddDoors(ToInt(tempdata[0]), tempdata[1], tempdata[1], ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToBool(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9])));
		return sNextLine;
	}

	//SetShaftDoors command
	if (StartsWithNoCase(LineData, "setshaftdoors"))
	{
		//backwards compatibility
		if (warn_deprecated == true)
			ScriptWarning("Command deprecated");

		//get data
		int params = SplitData(LineData, 14);

		if (params != 4)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 3; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		config->setshaftdoors = true;

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		car->SetShaftDoors(ToInt(tempdata[0]), ToFloat(tempdata[1]), ToFloat(tempdata[2]), ToFloat(tempdata[3]));
		return sNextLine;
	}

	//AddShaftDoors command
	if (StartsWithNoCase(LineData, "addshaftdoors "))
	{
		//get data
		int params = SplitData(LineData, 14);

		if (params < 7 || params > 10)
			return ScriptError("Incorrect number of parameters");

		int compat = 0;
		if (params == 7)
			compat = 1; //1.4 compatibility mode
		if (params == 8)
			compat = 2;

		//check numeric values
		if (compat == 0)
		{
			for (int i = 0; i <= 8; i++)
			{
				if (i == 1)
					i = 3;
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}
		if (compat == 1)
		{
			for (int i = 0; i <= 6; i++)
			{
				if (i == 1)
					i = 2;
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}
		if (compat == 2)
		{
			for (int i = 0; i <= 7; i++)
			{
				if (i == 1)
					i = 3;
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		if (compat > 0 && warn_deprecated == true)
			ScriptWarning("Syntax deprecated");

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		bool result;
		if (compat == 0 && params == 9)
			result = car->AddShaftDoors(ToInt(tempdata[0]), 0.0, tempdata[1], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]));
		else if (compat == 0 && params == 10)
			result = car->AddShaftDoors(ToInt(tempdata[0]), ToFloat(tempdata[9]), tempdata[1], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]));
		else if (compat == 1)
			result = car->AddShaftDoors(ToInt(tempdata[0]), 0.0, tempdata[1], tempdata[1], ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]), 0, ToFloat(tempdata[5]), ToFloat(tempdata[6]));
		else if (compat == 2)
			result = car->AddShaftDoors(ToInt(tempdata[0]), 0.0, tempdata[1], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), 0, ToFloat(tempdata[6]), ToFloat(tempdata[7]));

		if (result == false)
			return ScriptError();
		return sNextLine;
	}

	//CreatePanel command
	if (StartsWithNoCase(LineData, "createpanel"))
	{
		//get data
		int params = SplitData(LineData, 12);

		if (params != 13)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 12; i++)
		{
			if (i == 3)
				i = 4;
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		StoreCommand(car->CreateButtonPanel(tempdata[0], ToInt(tempdata[1]), ToInt(tempdata[2]), tempdata[3], ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12])));
		return sNextLine;
	}

	//AddFloorButton command
	if (StartsWithNoCase(LineData, "addfloorbutton"))
	{
		//get data
		int params = SplitData(LineData, 15);

		if (params < 7 || params > 11)
			return ScriptError("Incorrect number of parameters");

		Real hoffset = 0, voffset = 0;
		int compat = 0;

		//check numeric values
		if (params == 7)
		{
			//1.4 compatibility mode
			for (int i = 0; i <= 6; i++)
			{
				if (i == 1)
					i = 2;
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			compat = 1;
		}
		if (params == 9)
		{
			if (IsNumeric(tempdata[2]) == true)
			{
				//1.5 compatibility mode
				for (int i = 0; i <= 8; i++)
				{
					if (i == 1 || i == 4)
						i++;

					if (!IsNumeric(tempdata[i]))
						return ScriptError("Invalid value: " + tempdata[i]);
				}
				hoffset = ToFloat(tempdata[7]);
				voffset = ToFloat(tempdata[8]);
				compat = 1;
			}
		}
		if (params == 8 || params == 10)
		{
			//pre-Alpha 6 compatibility
			for (int i = 0; i <= 7; i++)
			{
				if (i == 1)
					i = 3;
				if (i == 5)
					i++;
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			if (params == 10)
			{
				hoffset = ToFloat(tempdata[8]);
				voffset = ToFloat(tempdata[9]);
			}
			compat = 2;
		}
		if (params == 9 || params == 11)
		{
			for (int i = 0; i <= 8; i++)
			{
				if (i == 1)
					i = 4;
				if (i == 6)
					i++;
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			if (params == 11)
			{
				hoffset = ToFloat(tempdata[9]);
				voffset = ToFloat(tempdata[10]);
			}
		}

		if (!car->GetPanel(ToInt(tempdata[0])))
			return ScriptError("Invalid panel number");

		if (compat > 0 && warn_deprecated == true)
			ScriptWarning("Syntax deprecated");

		if (compat == 0)
			parent->CheckFile("data/" + tempdata[1]);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		Control* control;

		if (compat == 0)
			control = car->GetPanel(ToInt(tempdata[0]))->AddButton(tempdata[1], tempdata[2], tempdata[3], ToInt(tempdata[4]), ToInt(tempdata[5]), tempdata[6], ToFloat(tempdata[7]), ToFloat(tempdata[8]), hoffset, voffset);
		if (compat == 1)
			control = car->GetPanel(ToInt(tempdata[0]))->AddButton("", tempdata[1], tempdata[1], ToInt(tempdata[2]), ToInt(tempdata[3]), tempdata[4], ToFloat(tempdata[5]), ToFloat(tempdata[6]), hoffset, voffset);
		if (compat == 2)
			control = car->GetPanel(ToInt(tempdata[0]))->AddButton("", tempdata[1], tempdata[2], ToInt(tempdata[3]), ToInt(tempdata[4]), tempdata[5], ToFloat(tempdata[6]), ToFloat(tempdata[7]), hoffset, voffset);

		if (control)
		{
			if (config->lockvalue == 0)
				control->SetLocked(false, config->keyvalue);
			else
				control->SetLocked(true, config->keyvalue);
		}
		StoreCommand(control);
		return sNextLine;
	}

	//AddControlButton command
	if (StartsWithNoCase(LineData, "addcontrolbutton"))
	{
		//get data
		int params = SplitData(LineData, 17);

		if (params < 7 || params > 11)
			return ScriptError("Incorrect number of parameters");

		Real hoffset = 0, voffset = 0;
		int compat = 0;

		//check numeric values
		if (params == 7)
		{
			//1.4 compatibility mode
			for (int i = 1; i <= 6; i++)
			{
				if (i == 1 || i == 4)
					i++;
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			compat = 1;
		}
		if (params == 9)
		{
			if (IsNumeric(tempdata[2]) == true)
			{
				//1.5 compatibility mode
				for (int i = 1; i <= 8; i++)
				{
					if (i == 1 || i == 4)
						i++;
					if (!IsNumeric(tempdata[i]))
						return ScriptError("Invalid value: " + tempdata[i]);
				}
				hoffset = ToFloat(tempdata[7]);
				voffset = ToFloat(tempdata[8]);
				compat = 1;
			}
		}
		if (params == 8 || params == 10)
		{
			//pre-Alpha 6 compatibility
			for (int i = 1; i <= 7; i++)
			{
				if (i == 1)
					i = 3;
				if (i == 5)
					i++;
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			if (params == 10)
			{
				hoffset = ToFloat(tempdata[8]);
				voffset = ToFloat(tempdata[9]);
			}
			compat = 2;
		}
		if (params == 9 || params == 11)
		{
			for (int i = 1; i <= 8; i++)
			{
				if (i == 1)
					i = 4;
				if (i == 6)
					i++;
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			if (params == 11)
			{
				hoffset = ToFloat(tempdata[9]);
				voffset = ToFloat(tempdata[10]);
			}
		}

		if (!car->GetPanel(ToInt(tempdata[0])))
			return ScriptError("Invalid panel number");

		if (compat > 0 && warn_deprecated == true)
			ScriptWarning("Syntax deprecated");

		if (compat == 0)
			parent->CheckFile("data/" + tempdata[1]);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		Control* control;

		if (compat == 0)
			control = car->GetPanel(ToInt(tempdata[0]))->AddButton(tempdata[1], tempdata[2], tempdata[3], ToInt(tempdata[4]), ToInt(tempdata[5]), tempdata[6], ToFloat(tempdata[7]), ToFloat(tempdata[8]), hoffset, voffset);
		if (compat == 1)
			control = car->GetPanel(ToInt(tempdata[0]))->AddButton("", tempdata[1], tempdata[1], ToInt(tempdata[2]), ToInt(tempdata[3]), tempdata[4], ToFloat(tempdata[5]), ToFloat(tempdata[6]), hoffset, voffset);
		if (compat == 2)
			control = car->GetPanel(ToInt(tempdata[0]))->AddButton("", tempdata[1], tempdata[2], ToInt(tempdata[3]), ToInt(tempdata[4]), tempdata[5], ToFloat(tempdata[6]), ToFloat(tempdata[7]), hoffset, voffset);

		if (control)
		{
			if (config->lockvalue == 0)
				control->SetLocked(false, config->keyvalue);
			else
				control->SetLocked(true, config->keyvalue);
		}
		StoreCommand(control);
		return sNextLine;
	}

	//AddButton command
	if (StartsWithNoCase(LineData, "addbutton "))
	{
		//get data
		int params = SplitData(LineData, 10);

		if (params != 9 && params != 11)
			return ScriptError("Incorrect number of parameters");

		Real hoffset = 0, voffset = 0;

		//check numeric values
		for (int i = 1; i <= 8; i++)
		{
			if (i == 1)
				i = 4;
			if (i == 6)
				i++;
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}
		if (params == 11)
		{
			hoffset = ToFloat(tempdata[9]);
			voffset = ToFloat(tempdata[10]);
		}

		if (!car->GetPanel(ToInt(tempdata[0])))
			return ScriptError("Invalid panel number");

		//check to see if file exists
		parent->CheckFile("data/" + tempdata[1]);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		Control* control = car->GetPanel(ToInt(tempdata[0]))->AddButton(tempdata[1], tempdata[2], tempdata[3], ToInt(tempdata[4]), ToInt(tempdata[5]), tempdata[6], ToFloat(tempdata[7]), ToFloat(tempdata[8]), hoffset, voffset);

		if (control)
		{
			if (config->lockvalue == 0)
				control->SetLocked(false, config->keyvalue);
			else
				control->SetLocked(true, config->keyvalue);
		}
		StoreCommand(control);
		return sNextLine;
	}

	//AddControl command
	if (StartsWithNoCase(LineData, "addcontrol "))
	{
		//get data
		int params = SplitData(LineData, 11);

		if (params < 10)
			return ScriptError("Incorrect number of parameters");

		//set backwards compatibility
		bool compat = false;
		if (IsNumeric(tempdata[8]) == false)
			compat = true;

		int end = 8;
		if (compat == true)
		{
			end = 7;

			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
		}

		//check numeric values
		for (int i = 1; i <= end; i++)
		{
			if (i == 1)
				i++;
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		if (!car->GetPanel(ToInt(tempdata[0])))
			return ScriptError("Invalid panel number");

		std::vector<std::string> action_array, tex_array;
		int slength, parameters;

		//get number of action & texture parameters
		slength = (int)tempdata.size();
		parameters = slength - (end + 1); //strip off main parameters

		//action & texture parameter number needs to be even
		if (IsEven(parameters) == false)
			return ScriptError("Incorrect number of parameters");

		for (int i = (end + 1); i < slength - (parameters / 2); i++)
			action_array.emplace_back(tempdata[i]);
		for (int i = slength - (parameters / 2); i < slength; i++)
			tex_array.emplace_back(tempdata[i]);

		//check to see if file exists
		parent->CheckFile("data/" + tempdata[1]);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		Control* control = 0;
		if (compat == true)
			control = car->GetPanel(ToInt(tempdata[0]))->AddControl(tempdata[1], ToInt(tempdata[2]), ToInt(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), 1, action_array, tex_array);
		else
			control = car->GetPanel(ToInt(tempdata[0]))->AddControl(tempdata[1], ToInt(tempdata[2]), ToInt(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToInt(tempdata[8]), action_array, tex_array);

		if (control)
		{
			if (config->lockvalue == 0)
				control->SetLocked(false, config->keyvalue);
			else
				control->SetLocked(true, config->keyvalue);
		}
		StoreCommand(control);
		return sNextLine;
	}

	//AddFloorIndicator command
	if (StartsWithNoCase(LineData, "addfloorindicator"))
	{
		//get data
		int params = SplitData(LineData, 18);

		if (params < 6 && params > 8)
			return ScriptError("Incorrect number of parameters");

		int compat = 0;
		if (params == 6)
			compat = 1; //1.4 compatibility mode
		if (params == 7)
			compat = 2;

		//check numeric values
		if (compat == 1)
		{
			for (int i = 1; i <= 5; i++)
			{
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
		}
		else if (compat == 2)
		{
			for (int i = 2; i <= 6; i++)
			{
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}
		else
		{
			for (int i = 3; i <= 7; i++)
			{
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		if (compat == 0)
			StoreCommand(car->AddFloorIndicator(tempdata[0], tempdata[1], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7])));
		else if (compat == 1)
			StoreCommand(car->AddFloorIndicator("Button", "", tempdata[0], ToFloat(tempdata[1]), ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5])));
		else if (compat == 2)
			StoreCommand(car->AddFloorIndicator(tempdata[0], "", tempdata[1], ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6])));
		return sNextLine;
	}

	//AddDisplay command
	if (StartsWithNoCase(LineData, "addkeypadindicator"))
	{
		//get data
		int params = SplitData(LineData, 19);

		if (params != 10)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 4; i <= 9; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		StoreCommand(car->AddKeypadIndicator(tempdata[0], tempdata[1], tempdata[2], tempdata[3], ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9])));
		return sNextLine;
	}

	//AddDirectionalIndicators command
	if (StartsWithNoCase(LineData, "adddirectionalindicators"))
	{
		//get data
		int params = SplitData(LineData, 25);

		if (params != 17 && params != 18)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		bool compat = false;
		if (params == 17)
		{
			for (int i = 8; i <= 16; i++)
			{
				if (i == 11 || i == 14)
					i++;
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			compat = true;
			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
		}
		else
		{
			for (int i = 9; i <= 17; i++)
			{
				if (i == 12 || i == 15)
					i++;
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		if (compat == false)
			car->AddDirectionalIndicators(ToBool(tempdata[0]), ToBool(tempdata[1]), ToBool(tempdata[2]), ToBool(tempdata[3]), tempdata[4], tempdata[5], tempdata[6], tempdata[7], tempdata[8], ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), tempdata[12], ToFloat(tempdata[13]), ToFloat(tempdata[14]), ToBool(tempdata[15]), ToFloat(tempdata[16]), ToFloat(tempdata[17]));
		else
			car->AddDirectionalIndicators(ToBool(tempdata[0]), false, ToBool(tempdata[1]), ToBool(tempdata[2]), tempdata[3], tempdata[4], tempdata[5], tempdata[6], tempdata[7], ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), tempdata[11], ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToBool(tempdata[14]), ToFloat(tempdata[15]), ToFloat(tempdata[16]));
		return sNextLine;
	}

	//AddFloorSigns command
	if (StartsWithNoCase(LineData, "addfloorsigns"))
	{
		//get data
		int params = SplitData(LineData, 14);

		if (params < 7 || params > 9)
			return ScriptError("Incorrect number of parameters");

		int compat = 0;
		if (params == 7)
			compat = 1; //1.4 compatibility mode
		if (params == 8)
			compat = 2; //1.5 compatibility mode

		//check numeric values
		if (compat == 0)
		{
			for (int i = 0; i <= 8; i++)
			{
				if (i == 1)
					i = 4;
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}
		else if (compat == 1)
		{
			for (int i = 2; i <= 6; i++)
			{
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}
		else if (compat == 2)
		{
			for (int i = 3; i <= 7; i++)
			{
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		if (compat > 0 && warn_deprecated == true)
			ScriptWarning("Syntax deprecated");

		if (compat == 0)
		{
			bool result;
			result = car->AddFloorSigns(ToInt(tempdata[0]), ToBool(tempdata[1]), tempdata[2], tempdata[3], ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]));
			if (result == false)
				return ScriptError();
		}
		else if (compat == 1)
			car->AddFloorSigns(0, ToBool(tempdata[0]), "Button", tempdata[1], ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]));
		else if (compat == 2)
			car->AddFloorSigns(0, ToBool(tempdata[0]), tempdata[1], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]));
		return sNextLine;
	}

	//AddSound command
	if (StartsWithNoCase(LineData, "addsound"))
	{
		//get data
		int params = SplitData(LineData, 9);

		if (params != 5 && params != 6 && params != 13 && params != 17)
			return ScriptError("Incorrect number of parameters");

		bool partial = false;
		bool compat = false;
		if (params == 5 || params == 6)
			partial = true;
		if (params == 5 || params == 13)
			compat = true;

		//check numeric values
		if (partial == true)
		{
			for (int i = 2; i <= 4; i++)
			{
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}
		else
		{
			if (compat == true)
			{
				for (int i = 2; i <= 12; i++)
				{
					if (!IsNumeric(tempdata[i]))
						return ScriptError("Invalid value: " + tempdata[i]);
				}
				if (warn_deprecated == true)
					ScriptWarning("Syntax deprecated");
			}
			else
			{
				for (int i = 2; i <= 16; i++)
				{
					if (i == 5)
						i = 6;

					if (!IsNumeric(tempdata[i]))
						return ScriptError("Invalid value: " + tempdata[i]);
				}
			}
		}

		//check to see if file exists
		parent->CheckFile("data/" + tempdata[1]);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		if (compat == true)
		{
			if (partial == true)
				StoreCommand(car->AddSound(tempdata[0], tempdata[1], Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]))));
			else
				StoreCommand(car->AddSound(tempdata[0], tempdata[1], Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4])), true, ToFloat(tempdata[5]), ToInt(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), 0.0, 360, 360, 1.0, Vector3(ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]))));
		}
		else
		{
			if (partial == true)
				StoreCommand(car->AddSound(tempdata[0], tempdata[1], Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4])), ToBool(tempdata[5])));
			else
				StoreCommand(car->AddSound(tempdata[0], tempdata[1], Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4])), ToBool(tempdata[5]), ToFloat(tempdata[6]), ToInt(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), Vector3(ToFloat(tempdata[14]), ToFloat(tempdata[15]), ToFloat(tempdata[16]))));
		}
		return sNextLine;
	}

	//AddReverb command
	if (StartsWithNoCase(LineData, "addreverb"))
	{
		if (car->GetReverb())
			return ScriptError("Cannot create more than one reverb in an elevator car");

		//get data
		int params = SplitData(LineData, 9);

		if (params != 7)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 6; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		StoreCommand(car->AddReverb(tempdata[0], tempdata[1], Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4])), ToFloat(tempdata[5]), ToFloat(tempdata[6])));
		return sNextLine;
	}

	//AddDoorComponent command
	if (StartsWithNoCase(LineData, "adddoorcomponent"))
	{
		//get data
		int params = SplitData(LineData, 17);

		if (params != 17 && params != 18)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		bool compat = false;
		if (params == 17)
		{
			compat = true;
			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
		}

		for (int i = 0; i <= params - 1; i++)
		{
			if (i == 1)
				i = 4;
			if (i == 5)
				i++;
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		if (compat == true)
			StoreCommand(car->AddDoorComponent(ToInt(tempdata[0]), tempdata[1], tempdata[2], tempdata[3], ToFloat(tempdata[4]), tempdata[5], ToFloat(tempdata[6]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]), ToFloat(tempdata[15]), ToFloat(tempdata[16])));
		else
			StoreCommand(car->AddDoorComponent(ToInt(tempdata[0]), tempdata[1], tempdata[2], tempdata[3], ToFloat(tempdata[4]), tempdata[5], ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]), ToFloat(tempdata[15]), ToFloat(tempdata[16]), ToFloat(tempdata[17])));
		return sNextLine;
	}

	//AddShaftDoorsComponent command
	if (StartsWithNoCase(LineData, "addshaftdoorscomponent"))
	{
		//get data
		int params = SplitData(LineData, 23);

		if (params != 17 && params != 18)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		bool compat = false;
		if (params == 17)
		{
			compat = true;
			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
		}

		for (int i = 0; i <= params - 1; i++)
		{
			if (i == 1)
				i = 4;
			if (i == 5)
				i++;
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		if (compat == true)
			car->AddShaftDoorsComponent(ToInt(tempdata[0]), tempdata[1], tempdata[2], tempdata[3], ToFloat(tempdata[4]), tempdata[5], ToFloat(tempdata[6]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]), ToFloat(tempdata[15]), ToFloat(tempdata[16]));
		else
			car->AddShaftDoorsComponent(ToInt(tempdata[0]), tempdata[1], tempdata[2], tempdata[3], ToFloat(tempdata[4]), tempdata[5], ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]), ToFloat(tempdata[15]), ToFloat(tempdata[16]), ToFloat(tempdata[17]));
		return sNextLine;
	}

	//FinishDoors command
	if (StartsWithNoCase(LineData, "finishdoors"))
	{
		//get data
		int params = SplitData(LineData, 12);

		bool legacy = false;

		if (params < 1)
			return ScriptError("Incorrect number of parameters");

		if (params > 2)
		{
			if (IsNumeric(tempdata[1]) == true)
			{
				if (warn_deprecated == true)
					ScriptWarning("Syntax deprecated");
				legacy = true;
			}
		}

		//check numeric values
		if (!IsNumeric(tempdata[0]))
			return ScriptError("Invalid value: " + tempdata[0]);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		DoorWrapper *wrapper = 0;

		if (params == 1 || legacy == true)
			wrapper = car->FinishDoors(ToInt(tempdata[0]));
		else if (params == 2)
			wrapper = car->FinishDoors(ToInt(tempdata[0]), ToBool(tempdata[1]));
		else
			wrapper = car->FinishDoors(ToInt(tempdata[0]), ToBool(tempdata[1]), ToBool(tempdata[2]));

		if (!wrapper)
			return ScriptError();

		StoreCommand(wrapper);
		return sNextLine;
	}

	//FinishShaftDoors command
	if (StartsWithNoCase(LineData, "finishshaftdoors"))
	{
		//get data
		int params = SplitData(LineData, 17);

		bool legacy = false;

		if (params < 1)
			return ScriptError("Incorrect number of parameters");

		if (params > 2)
		{
			if (IsNumeric(tempdata[1]) == true)
			{
				if (warn_deprecated == true)
					ScriptWarning("Syntax deprecated");
				legacy = true;
			}
		}

		//check numeric values
		if (!IsNumeric(tempdata[0]))
			return ScriptError("Invalid value: " + tempdata[0]);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		bool result;
		if (params == 1 || legacy == true)
			result = car->FinishShaftDoors(ToInt(tempdata[0]));
		else if (params == 2)
			result = car->FinishShaftDoors(ToInt(tempdata[0]), ToBool(tempdata[1]));
		else
			result = car->FinishShaftDoors(ToInt(tempdata[0]), ToBool(tempdata[1]), ToBool(tempdata[2]));

		if (result == false)
			return ScriptError();
		return sNextLine;
	}

	//AddDirectionalIndicator command
	if (StartsWithNoCase(LineData, "adddirectionalindicator "))
	{
		//get data
		int params = SplitData(LineData, 24);

		if (params != 17)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 8; i <= 16; i++)
		{
			if (i == 11 || i == 14)
				i++;
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		StoreCommand(car->AddDirectionalIndicator(ToBool(tempdata[0]), ToBool(tempdata[1]), ToBool(tempdata[2]), tempdata[3], tempdata[4], tempdata[5], tempdata[6], tempdata[7], ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), tempdata[11], ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToBool(tempdata[14]), ToFloat(tempdata[15]), ToFloat(tempdata[16])));
		return sNextLine;
	}

	//AddDoor command
	if (StartsWithNoCase(LineData, "adddoor "))
	{
		//get data
		int params = SplitData(LineData, 8);

		if (params < 12 || params > 14)
			return ScriptError("Incorrect number of parameters");

		int compat = 0;

		//check numeric values
		if (params == 12)
		{
			for (int i = 3; i <= 11; i++)
			{
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			compat = 1;
		}
		if (params == 13)
		{
			for (int i = 3; i <= 12; i++)
			{
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			compat = 2;
		}
		if (params == 14)
		{
			for (int i = 4; i <= 13; i++)
			{
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		parent->CheckFile("data/" + tempdata[0]);
		parent->CheckFile("data/" + tempdata[1]);

		if (compat > 0 && warn_deprecated == true)
			ScriptWarning("Syntax deprecated");

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//get directions
		std::string face_direction;
		std::string open_direction;
		int direction = 0;
		if (compat >= 1)
			direction = ToInt(tempdata[4]);
		else
			direction = ToInt(tempdata[5]);
		GetDirectionStrings(direction, face_direction, open_direction);

		//create door
		Door* door;

		if (compat == 1)
			door = car->AddDoor("", tempdata[0], tempdata[1], false, tempdata[2], tempdata[2], ToFloat(tempdata[3]), face_direction, open_direction, true, 0, 0, ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), 0, 0);
		else if (compat == 2)
			door = car->AddDoor("", tempdata[0], tempdata[1], false, tempdata[2], tempdata[2], ToFloat(tempdata[3]), face_direction, open_direction, true, ToFloat(tempdata[5]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), 0, 0);
		else
			door = car->AddDoor("", tempdata[0], tempdata[1], ToBool(tempdata[2]), tempdata[3], tempdata[3], ToFloat(tempdata[4]), face_direction, open_direction, true, ToFloat(tempdata[6]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), 0, 0);

		if (door)
			door->SetLocked(config->lockvalue, config->keyvalue);

		StoreCommand(door);
		return sNextLine;
	}

	//AddModel command
	if (StartsWithNoCase(LineData, "addmodel"))
	{
		if (parent->NoModels == true)
			return sNextLine;

		//get data
		int params = SplitData(LineData, 9);

		if (params != 14 && params != 15)
			return ScriptError("Incorrect number of parameters");

		bool compat = false;
		if (params == 14)
			compat = true;

		//check numeric values
		if (compat == true)
		{
			for (int i = 2; i <= 13; i++)
			{
				if (i == 10)
					i++;
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
		}
		else
		{
			for (int i = 3; i <= 14; i++)
			{
				if (i == 11)
					i++;
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		//check to see if file exists
		parent->CheckFile("data/" + tempdata[1]);

		//stop here if in Check mode
		if (config->CheckScript == true)
		{
			config->setkey = false;
			return sNextLine;
		}

		//create model
		Model *model;
		if (compat == true)
			model = car->AddModel(tempdata[0], tempdata[1], false, Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4])), Vector3(ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7])), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToBool(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]));
		else
			model = car->AddModel(tempdata[0], tempdata[1], ToBool(tempdata[2]), Vector3(ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5])), Vector3(ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8])), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToBool(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]));

		if (config->setkey == true && model)
			model->SetKey(config->keyvalue);
		config->setkey = false;

		StoreCommand(model);
		return sNextLine;
	}

	//AddActionControl command
	if (StartsWithNoCase(LineData, "addactioncontrol"))
	{
		//get data
		int params = SplitData(LineData, 17);

		if (params < 10)
			return ScriptError("Incorrect number of parameters");

		//set backwards compatibility
		bool compat = false;
		if (IsNumeric(tempdata[8]) == false)
			compat = true;

		int end = 8;
		if (compat == true)
		{
			end = 7;

			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
		}

		//check numeric values
		for (int i = 3; i <= end; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::vector<std::string> action_array, tex_array;
		int slength, parameters;

		//get number of action & texture parameters
		slength = (int)tempdata.size();
		parameters = slength - (end + 1); //strip off main parameters

		//action & texture parameter number needs to be even
		if (IsEven(parameters) == false)
			return ScriptError("Incorrect number of parameters");

		for (int i = (end + 1); i < slength - (parameters / 2); i++)
			action_array.emplace_back(tempdata[i]);
		for (int i = slength - (parameters / 2); i < slength; i++)
			tex_array.emplace_back(tempdata[i]);

		//check to see if file exists
		parent->CheckFile("data/" + tempdata[1]);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		Control* control = 0;
		if (compat == true)
			control = car->AddControl(tempdata[0], tempdata[1], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), 1, action_array, tex_array);
		else
			control = car->AddControl(tempdata[0], tempdata[1], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToInt(tempdata[8]), action_array, tex_array);

		if (control)
		{
			if (config->lockvalue == 0)
				control->SetLocked(false, config->keyvalue);
			else
				control->SetLocked(true, config->keyvalue);
		}
		StoreCommand(control);
		return sNextLine;
	}

	//AddTrigger command
	if (StartsWithNoCase(LineData, "addtrigger"))
	{
		//get data
		int params = SplitData(LineData, 11);

		if (params < 9)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 7; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::vector<std::string> action_array;

		//get number of action & texture parameters
		for (int i = 8; i < params; i++)
			action_array.emplace_back(tempdata[i]);

		//check to see if file exists
		parent->CheckFile("data/" + tempdata[1]);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		Vector3 min = Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]));
		Vector3 max = Vector3(ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]));
		StoreCommand(car->AddTrigger(tempdata[0], tempdata[1], min, max, action_array));
		return sNextLine;
	}

	//CreateCar command
	if (StartsWithNoCase(LineData, "createcar"))
	{
		//get data
		int params = SplitData(LineData, 10);

		if (params != 1)
			return ScriptError("Incorrect number of parameters");

		bool result = car->CreateCar(ToInt(tempdata[0]));
		if (result == false)
			return ScriptError();

		StoreCommand(car);
		return sNextLine;
	}

	//AddElevatorIDSigns command
	if (StartsWithNoCase(LineData, "addelevatoridsigns"))
	{
		//get data
		int params = SplitData(LineData, 19);

		if (params != 9)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 8; i++)
		{
			if (i == 1)
				i = 4;
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		bool result = car->AddElevatorIDSigns(ToInt(tempdata[0]), ToBool(tempdata[1]), tempdata[2], tempdata[3], ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]));

		if (result == false)
			return ScriptError();
		return sNextLine;
	}

	//handle end of car section
	if (StartsWithNoCase(LineData, "<endcar>") == true && config->RangeL == config->RangeH)
	{
		//return to elevator section
		config->SectionNum = 4;
		config->Context = config->ContextOld;
		config->Current = config->CurrentOld;
		config->RangeL = config->RangeLOld;
		config->RangeH = config->RangeHOld;
		config->RangeStart = config->RangeStartOld;

		if (parent->InRunloop() == false)
			engine->Report("Finished car");
		return sNextLine;
	}

	//handle car range
	if (config->RangeL != config->RangeH && StartsWithNoCase(LineData, "<endcar"))
	{
		if (config->Current < config->RangeH)
		{
			config->Current++;
			parent->line = config->RangeStart;  //loop back
			return sNextLine;
		}
		else
		{
			config->SectionNum = 4; //break out of loop
			config->Context = config->ContextOld;
			config->RangeL = config->RangeLOld;
			config->RangeH = config->RangeHOld;
			config->RangeStart = config->RangeStartOld;
			config->Current = config->CurrentOld;

			if (parent->InRunloop() == false)
				engine->Report("Finished cars");
			return sNextLine;
		}
	}

	return sContinue;
}

}
