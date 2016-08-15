/* $Id$ */

/*
	Skyscraper 1.10 Alpha - Script Processor - Elevator Car Section
	Copyright (C)2003-2016 Ryan Thoryk
	http://www.skyscrapersim.com
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@skyscrapersim.com

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
#include "skyscraper.h"
#include "enginecontext.h"
#include "elevator.h"
#include "elevatorcar.h"
#include "trigger.h"
#include "control.h"
#include "door.h"
#include "buttonpanel.h"
#include "directional.h"
#include "floorindicator.h"
#include "model.h"
#include "sound.h"
#include "wall.h"
#include "scriptprocessor.h"
#include "script_section.h"

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
		if (SetCaseCopy(LineData.substr(0, 2), false) == "if" || SetCaseCopy(LineData.substr(0, 5), false) == "while")
			return sContinue;

		//process math functions
		if (MathFunctions(LineData) == sError)
			return sError;

		//process functions
		if (parent->FunctionProc() == true)
			return sNextLine;
	}

	//get text after equal sign
	bool equals = true;
	if ((int)LineData.find("=", 0) == -1)
		equals = false;
	std::string value = GetAfterEquals(LineData);

	//create a lowercase string of the line
	std::string linecheck = SetCaseCopy(LineData, false);

	//parameters
	if (linecheck.substr(0, 4) == "name")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		car->Name = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 9) == "openspeed")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (car->Created == false)
			return ScriptError("Car not created yet");
		std::string str = LineData.substr(9, LineData.find("=", 0) - 9);
		str = Calc(str);
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
	if (linecheck.substr(0, 6) == "doors ")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, car->NumDoors))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 14) == "servicedfloors")
	{
		//copy string listing of serviced floors into array
		int params = SplitAfterEquals(LineData, false);
		if (params < 1)
			return ScriptError("Syntax Error");

		for (int line = 0; line < params; line++)
		{
			if (tempdata[line].find("-", 1) > 0)
			{
				int start, end;
				//found a range marker
				std::string str1 = tempdata[line].substr(0, tempdata[line].find("-", 1));
				std::string str2 = tempdata[line].substr(tempdata[line].find("-", 1) + 1);
				TrimString(str1);
				TrimString(str2);
				if (!IsNumeric(str1, start) || !IsNumeric(str2, end))
					return ScriptError("Invalid value");
				if (end < start)
				{
					int temp = start;
					start = end;
					end = temp;
				}

				for (int k = start; k <= end; k++)
				{
					if (!car->AddServicedFloor(k))
						return ScriptError();
				}
			}
			else
			{
				int data;
				if (!IsNumeric(tempdata[line], data))
					return ScriptError("Invalid value");
				if (!car->AddServicedFloor(data))
					return ScriptError();
			}
		}
		return sNextLine;
	}
	if (linecheck.substr(0, 13) == "displayfloors")
	{
		//copy string listing of serviced floors into array
		int params = SplitAfterEquals(LineData, false);
		if (params < 1)
			return ScriptError("Syntax Error");

		for (int line = 0; line < params; line++)
		{
			if (tempdata[line].find("-", 1) > 0)
			{
				int start, end;
				//found a range marker
				std::string str1 = tempdata[line].substr(0, tempdata[line].find("-", 1));
				std::string str2 = tempdata[line].substr(tempdata[line].find("-", 1) + 1);
				TrimString(str1);
				TrimString(str2);
				if (!IsNumeric(str1, start) || !IsNumeric(str2, end))
					return ScriptError("Invalid value");
				if (end < start)
				{
					int temp = start;
					start = end;
					end = temp;
				}

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
	if (linecheck.substr(0, 9) == "doortimer")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (car->Created == false)
			return ScriptError("Car not created yet");
		std::string str = LineData.substr(9, LineData.find("=", 0) - 9);
		str = Calc(str);
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
	if (linecheck.substr(0, 10) == "quickclose")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (car->Created == false)
			return ScriptError("Car not created yet");
		std::string str = LineData.substr(10, LineData.find("=", 0) - 10);
		str = Calc(str);
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
	if (linecheck.substr(0, 10) == "nudgetimer")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (car->Created == false)
			return ScriptError("Car not created yet");
		std::string str = LineData.substr(10, LineData.find("=", 0) - 10);
		str = Calc(str);
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
	if (linecheck.substr(0, 9) == "slowspeed")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (car->Created == false)
			return ScriptError("Car not created yet");
		std::string str = LineData.substr(9, LineData.find("=", 0) - 9);
		str = Calc(str);
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
	if (linecheck.substr(0, 11) == "manualspeed")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (car->Created == false)
			return ScriptError("Car not created yet");
		std::string str = LineData.substr(11, LineData.find("=", 0) - 11);
		str = Calc(str);
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
	if (linecheck.substr(0, 9) == "opensound")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (car->Created == false)
			return ScriptError("Car not created yet");
		std::string str = LineData.substr(9, LineData.find("=", 0) - 9);
		str = Calc(str);
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
	if (linecheck.substr(0, 10) == "closesound")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (car->Created == false)
			return ScriptError("Car not created yet");
		std::string str = LineData.substr(10, LineData.find("=", 0) - 10);
		str = Calc(str);
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
	if (linecheck.substr(0, 10) == "nudgesound")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (car->Created == false)
			return ScriptError("Car not created yet");
		std::string str = LineData.substr(10, LineData.find("=", 0) - 10);
		str = Calc(str);
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
	if (linecheck.substr(0, 10) == "startsound")
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
	if (linecheck.substr(0, 9) == "movesound")
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
	if (linecheck.substr(0, 9) == "stopsound")
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
	if (linecheck.substr(0, 9) == "idlesound")
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
	if (linecheck.substr(0, 13) == "carstartsound")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->UpStartSound = value;
		car->DownStartSound = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 15) == "carupstartsound")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->UpStartSound = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 17) == "cardownstartsound")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->DownStartSound = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 12) == "carmovesound")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->UpMoveSound = value;
		car->DownMoveSound = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 14) == "carupmovesound")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->UpMoveSound = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 16) == "cardownmovesound")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->DownMoveSound = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 12) == "carstopsound")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->UpStopSound = value;
		car->DownStopSound = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 14) == "carupstopsound")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->UpStopSound = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 16) == "cardownstopsound")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->DownStopSound = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 12) == "caridlesound")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->IdleSound = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 10) == "chimesound")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (car->Created == false)
			return ScriptError("Car not created yet");
		std::string str = LineData.substr(10, LineData.find("=", 0) - 10);
		str = Calc(str);
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
	if (linecheck.substr(0, 12) == "upchimesound")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (car->Created == false)
			return ScriptError("Car not created yet");
		std::string str = LineData.substr(12, LineData.find("=", 0) - 12);
		str = Calc(str);
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
	if (linecheck.substr(0, 14) == "downchimesound")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (car->Created == false)
			return ScriptError("Car not created yet");
		std::string str = LineData.substr(14, LineData.find("=", 0) - 14);
		str = Calc(str);
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
	if (linecheck.substr(0, 10) == "alarmsound")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->AlarmSound = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 14) == "alarmsoundstop")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->AlarmSoundStop = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 9) == "beepsound")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->SetBeepSound(value);
		return sNextLine;
	}
	if (linecheck.substr(0, 10) == "floorsound")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->SetFloorSound(value);
		return sNextLine;
	}
	if (linecheck.substr(0, 9) == "upmessage")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->SetMessageSound(true, true, value);
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "downmessage")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->SetMessageSound(true, false, value);
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "openmessage")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->SetMessageSound(false, true, value);
		return sNextLine;
	}
	if (linecheck.substr(0, 12) == "closemessage")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->SetMessageSound(false, false, value);
		return sNextLine;
	}
	if (linecheck.substr(0, 6) == "music ")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->Music = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 8) == "musicon ")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		car->MusicOn = ToBool(value);
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "musiconmove")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		car->MusicOnMove = ToBool(value);
	}
	if (linecheck.substr(0, 13) == "musicposition")
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

		car->MusicPosition = Ogre::Vector3(ToFloat(tempdata[0]), ToFloat(tempdata[1]), ToFloat(tempdata[2]));
		return sNextLine;
	}
	if (linecheck.substr(0, 10) == "autoenable")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		car->AutoEnable = ToBool(value);
		return sNextLine;
	}
	if (linecheck.substr(0, 10) == "doorsensor")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (car->Created == false)
			return ScriptError("Car not created yet");

		std::string str = LineData.substr(10, LineData.find("=", 0) - 10);
		str = Calc(str);
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
	if (linecheck.substr(0, 21) == "caremergencystopsound")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		car->EmergencyStopSound = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 18) == "independentservice")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		elev->IndependentService = ToBool(value);
		elev->IndependentServiceCar = car->Number;
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "fireservice2")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		elev->FireServicePhase2 = ToInt(value);
		elev->FireServicePhase2Car = car->Number;
		return sNextLine;
	}

	//AddFloor command
	if (linecheck.substr(0, 9) == "addfloor ")
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

		//create floor
		if (compat == true)
			StoreCommand(car->AddFloor(tempdata[0], tempdata[1], ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), config->ReverseAxis, false, ToFloat(tempdata[9]), ToFloat(tempdata[10]), true));
		else
			StoreCommand(car->AddFloor(tempdata[0], tempdata[1], ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToBool(tempdata[9]), ToBool(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12])));
		return sNextLine;
	}

	//AddWall command
	if (linecheck.substr(0, 7) == "addwall")
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

		//create wall
		StoreCommand(car->AddWall(tempdata[0], tempdata[1], ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12])));
		return sNextLine;
	}

	//AddDoors command
	if (linecheck.substr(0, 8) == "adddoors")
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

		if (compat == false)
			StoreCommand(car->AddDoors(ToInt(tempdata[0]), tempdata[1], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToBool(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10])));
		else
			StoreCommand(car->AddDoors(ToInt(tempdata[0]), tempdata[1], tempdata[1], ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToBool(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9])));
		return sNextLine;
	}

	//SetShaftDoors command
	if (linecheck.substr(0, 13) == "setshaftdoors")
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

		car->SetShaftDoors(ToInt(tempdata[0]), ToFloat(tempdata[1]), ToFloat(tempdata[2]), ToFloat(tempdata[3]));
		config->setshaftdoors = true;
		return sNextLine;
	}

	//AddShaftDoors command
	if (linecheck.substr(0, 14) == "addshaftdoors ")
	{
		//get data
		int params = SplitData(LineData, 14);

		if (params < 7 || params > 9)
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

		bool result;
		if (compat == 0)
			result = car->AddShaftDoors(ToInt(tempdata[0]), tempdata[1], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]));
		if (compat == 1)
			result = car->AddShaftDoors(ToInt(tempdata[0]), tempdata[1], tempdata[1], ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]), 0, ToFloat(tempdata[5]), ToFloat(tempdata[6]));
		if (compat == 2)
			result = car->AddShaftDoors(ToInt(tempdata[0]), tempdata[1], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), 0, ToFloat(tempdata[6]), ToFloat(tempdata[7]));

		if (result == false)
			return ScriptError();
		return sNextLine;
	}

	//CreatePanel command
	if (linecheck.substr(0, 11) == "createpanel")
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
	if (linecheck.substr(0, 14) == "addfloorbutton")
	{
		//get data
		int params = SplitData(LineData, 15);

		if (params < 7 || params > 11)
			return ScriptError("Incorrect number of parameters");

		float hoffset = 0, voffset = 0;
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

		Control* control;

		if (compat == 0)
		{
			parent->CheckFile("data/" + tempdata[1]);
			control = car->GetPanel(ToInt(tempdata[0]))->AddButton(tempdata[1], tempdata[2], tempdata[3], ToInt(tempdata[4]), ToInt(tempdata[5]), tempdata[6], ToFloat(tempdata[7]), ToFloat(tempdata[8]), hoffset, voffset);
		}
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
	if (linecheck.substr(0, 16) == "addcontrolbutton")
	{
		//get data
		int params = SplitData(LineData, 17);

		if (params < 7 || params > 11)
			return ScriptError("Incorrect number of parameters");

		float hoffset = 0, voffset = 0;
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

		Control* control;

		if (compat == 0)
		{
			parent->CheckFile("data/" + tempdata[1]);
			control = car->GetPanel(ToInt(tempdata[0]))->AddButton(tempdata[1], tempdata[2], tempdata[3], ToInt(tempdata[4]), ToInt(tempdata[5]), tempdata[6], ToFloat(tempdata[7]), ToFloat(tempdata[8]), hoffset, voffset);
		}
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
	if (linecheck.substr(0, 10) == "addbutton ")
	{
		//get data
		int params = SplitData(LineData, 10);

		if (params != 9 && params != 11)
			return ScriptError("Incorrect number of parameters");

		float hoffset = 0, voffset = 0;

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
	if (linecheck.substr(0, 11) == "addcontrol ")
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
			end = 7;

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
			action_array.push_back(tempdata[i]);
		for (int i = slength - (parameters / 2); i < slength; i++)
			tex_array.push_back(tempdata[i]);

		//check to see if file exists
		parent->CheckFile("data/" + tempdata[1]);

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
	if (linecheck.substr(0, 17) == "addfloorindicator")
	{
		//get data
		int params = SplitData(LineData, 18);

		if (params != 6 && params != 7)
			return ScriptError("Incorrect number of parameters");

		bool compat = false;
		if (params == 6)
			compat = true; //1.4 compatibility mode

		//check numeric values
		if (compat == true)
		{
			for (int i = 1; i <= 5; i++)
			{
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
		}
		else
		{
			for (int i = 2; i <= 6; i++)
			{
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		if (compat == false)
			StoreCommand(car->AddFloorIndicator(tempdata[0], tempdata[1], ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6])));
		else
			StoreCommand(car->AddFloorIndicator("Button", tempdata[0], ToFloat(tempdata[1]), ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5])));
		return sNextLine;
	}

	//AddDirectionalIndicators command
	if (linecheck.substr(0, 24) == "adddirectionalindicators")
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

		if (compat == false)
			car->AddDirectionalIndicators(ToBool(tempdata[0]), ToBool(tempdata[1]), ToBool(tempdata[2]), ToBool(tempdata[3]), tempdata[4], tempdata[5], tempdata[6], tempdata[7], tempdata[8], ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), tempdata[12], ToFloat(tempdata[13]), ToFloat(tempdata[14]), ToBool(tempdata[15]), ToFloat(tempdata[16]), ToFloat(tempdata[17]));
		else
			car->AddDirectionalIndicators(ToBool(tempdata[0]), false, ToBool(tempdata[1]), ToBool(tempdata[2]), tempdata[3], tempdata[4], tempdata[5], tempdata[6], tempdata[7], ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), tempdata[11], ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToBool(tempdata[14]), ToFloat(tempdata[15]), ToFloat(tempdata[16]));
		return sNextLine;
	}

	//AddFloorSigns command
	if (linecheck.substr(0, 13) == "addfloorsigns")
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
	if (linecheck.substr(0, 8) == "addsound")
	{
		//get data
		int params = SplitData(LineData, 9, true);

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

		if (compat == true)
		{
			if (partial == true)
				StoreCommand(car->AddSound(tempdata[0], tempdata[1], Ogre::Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]))));
			else
				StoreCommand(car->AddSound(tempdata[0], tempdata[1], Ogre::Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4])), true, ToFloat(tempdata[5]), ToInt(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), 0.0, 360, 360, 1.0, Ogre::Vector3(ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]))));
		}
		else
		{
			if (partial == true)
				StoreCommand(car->AddSound(tempdata[0], tempdata[1], Ogre::Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4])), ToBool(tempdata[5])));
			else
				StoreCommand(car->AddSound(tempdata[0], tempdata[1], Ogre::Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4])), ToBool(tempdata[5]), ToFloat(tempdata[6]), ToInt(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), Ogre::Vector3(ToFloat(tempdata[14]), ToFloat(tempdata[15]), ToFloat(tempdata[16]))));
		}
		return sNextLine;
	}

	//AddDoorComponent command
	if (linecheck.substr(0, 16) == "adddoorcomponent")
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

		if (compat == true)
			StoreCommand(car->AddDoorComponent(ToInt(tempdata[0]), tempdata[1], tempdata[2], tempdata[3], ToFloat(tempdata[4]), tempdata[5], ToFloat(tempdata[6]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]), ToFloat(tempdata[15]), ToFloat(tempdata[16])));
		else
			StoreCommand(car->AddDoorComponent(ToInt(tempdata[0]), tempdata[1], tempdata[2], tempdata[3], ToFloat(tempdata[4]), tempdata[5], ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]), ToFloat(tempdata[15]), ToFloat(tempdata[16]), ToFloat(tempdata[17])));
		return sNextLine;
	}

	//AddShaftDoorsComponent command
	if (linecheck.substr(0, 22) == "addshaftdoorscomponent")
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

		if (compat == true)
			car->AddShaftDoorsComponent(ToInt(tempdata[0]), tempdata[1], tempdata[2], tempdata[3], ToFloat(tempdata[4]), tempdata[5], ToFloat(tempdata[6]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]), ToFloat(tempdata[15]), ToFloat(tempdata[16]));
		else
			car->AddShaftDoorsComponent(ToInt(tempdata[0]), tempdata[1], tempdata[2], tempdata[3], ToFloat(tempdata[4]), tempdata[5], ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]), ToFloat(tempdata[15]), ToFloat(tempdata[16]), ToFloat(tempdata[17]));
		return sNextLine;
	}

	//FinishDoors command
	if (linecheck.substr(0, 11) == "finishdoors")
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

		if (params == 1 || legacy == true)
			StoreCommand(car->FinishDoors(ToInt(tempdata[0])));
		else if (params == 2)
			StoreCommand(car->FinishDoors(ToInt(tempdata[0]), ToBool(tempdata[1])));
		else
			StoreCommand(car->FinishDoors(ToInt(tempdata[0]), ToBool(tempdata[1]), ToBool(tempdata[2])));

		return sNextLine;
	}

	//FinishShaftDoors command
	if (linecheck.substr(0, 16) == "finishshaftdoors")
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
	if (linecheck.substr(0, 24) == "adddirectionalindicator ")
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

		StoreCommand(car->AddDirectionalIndicator(ToBool(tempdata[0]), ToBool(tempdata[1]), ToBool(tempdata[2]), tempdata[3], tempdata[4], tempdata[5], tempdata[6], tempdata[7], ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), tempdata[11], ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToBool(tempdata[14]), ToFloat(tempdata[15]), ToFloat(tempdata[16])));
		return sNextLine;
	}

	//AddDoor command
	if (linecheck.substr(0, 8) == "adddoor ")
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

		//create door
		Door* door;

		if (compat == 1)
			door = car->AddDoor(tempdata[0], tempdata[1], false, tempdata[2], ToFloat(tempdata[3]), ToInt(tempdata[4]), 0, ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]));
		else if (compat == 2)
			door = car->AddDoor(tempdata[0], tempdata[1], false, tempdata[2], ToFloat(tempdata[3]), ToInt(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]));
		else
			door = car->AddDoor(tempdata[0], tempdata[1], ToBool(tempdata[2]), tempdata[3], ToFloat(tempdata[4]), ToInt(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]));

		if (door)
			door->SetLocked(config->lockvalue, config->keyvalue);

		StoreCommand(door);
		return sNextLine;
	}

	//AddModel command
	if (linecheck.substr(0, 8) == "addmodel")
	{
		//get data
		int params = SplitData(LineData, 9, true);

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

		//create model
		Model *model;
		if (compat == true)
			model = car->AddModel(tempdata[0], tempdata[1], false, Ogre::Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4])), Ogre::Vector3(ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7])), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToBool(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]));
		else
			model = car->AddModel(tempdata[0], tempdata[1], ToBool(tempdata[2]), Ogre::Vector3(ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5])), Ogre::Vector3(ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8])), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToBool(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]));

		if (config->setkey == true && model)
			model->SetKey(config->keyvalue);
		config->setkey = false;

		StoreCommand(model);
		return sNextLine;
	}

	//AddActionControl command
	if (linecheck.substr(0, 16) == "addactioncontrol")
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
			end = 7;

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
			action_array.push_back(tempdata[i]);
		for (int i = slength - (parameters / 2); i < slength; i++)
			tex_array.push_back(tempdata[i]);

		//check to see if file exists
		parent->CheckFile("data/" + tempdata[1]);

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
	if (linecheck.substr(0, 10) == "addtrigger")
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
			action_array.push_back(tempdata[i]);

		//check to see if file exists
		parent->CheckFile("data/" + tempdata[1]);

		Ogre::Vector3 min = Ogre::Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]));
		Ogre::Vector3 max = Ogre::Vector3(ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]));
		StoreCommand(car->AddTrigger(tempdata[0], tempdata[1], min, max, action_array));
		return sNextLine;
	}

	//CreateCar command
	if (linecheck.substr(0, 9) == "createcar")
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

	//handle end of car section
	if (linecheck == "<endcar>" && config->RangeL == config->RangeH)
	{
		//return to elevator section
		config->SectionNum = 4;
		config->Context = config->ContextOld;
		config->Current = config->CurrentOld;
		config->RangeL = config->RangeLOld;
		config->RangeH = config->RangeHOld;
		config->RangeStart = config->RangeStartOld;

		engine->Report("Finished car");
		return sNextLine;
	}

	//handle car range
	if (config->RangeL != config->RangeH && linecheck.substr(0, 7) == "<endcar")
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
			engine->Report("Finished cars");
			return sNextLine;
		}
	}

	return sContinue;
}

}
