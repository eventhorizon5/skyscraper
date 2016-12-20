/* $Id$ */

/*
	Skyscraper 1.11 Alpha - Script Processor - Elevator Section
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
#include "scriptprocessor.h"
#include "script_section.h"

using namespace SBS;

namespace Skyscraper {

ScriptProcessor::ElevatorSection::ElevatorSection(ScriptProcessor *parent) : Section(parent)
{

}

int ScriptProcessor::ElevatorSection::Run(std::string &LineData)
{
	//Process elevators

	//create elevator if not created already
	Simcore->NewElevator(config->Current);

	//replace variables with actual values
	ReplaceAll(LineData, "%elevator%", ToString(config->Current));

	//IF/While statement stub (continue to global commands for processing)
	if (SetCaseCopy(LineData.substr(0, 2), false) == "if" || SetCaseCopy(LineData.substr(0, 5), false) == "while")
		return sContinue;

	//process math functions
	if (MathFunctions(LineData) == sError)
		return sError;

	//process functions
	if (parent->FunctionProc() == true)
		return sNextLine;

	//get text after equal sign
	bool equals = true;
	if ((int)LineData.find("=", 0) == -1)
		equals = false;
	std::string value = GetAfterEquals(LineData);

	//get elevator object
	Elevator *elev = Simcore->GetElevator(config->Current);

	//create a lowercase string of the line
	std::string linecheck = SetCaseCopy(LineData, false);

	//parameters
	if (linecheck.substr(0, 4) == "name")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		elev->Name = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 4) == "type")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		elev->Type = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 5) == "speed")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, elev->ElevatorSpeed))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 12) == "acceleration")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, elev->Acceleration))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 12) == "deceleration")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, elev->Deceleration))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 9) == "acceljerk")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, elev->AccelJerk))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 9) == "deceljerk")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, elev->DecelJerk))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 13) == "assignedshaft")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, elev->AssignedShaft))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 15) == "motorstartsound")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		elev->MotorUpStartSound = value;
		elev->MotorDownStartSound = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 17) == "motorupstartsound")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		elev->MotorUpStartSound = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 19) == "motordownstartsound")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		elev->MotorDownStartSound = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 13) == "motorrunsound")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		elev->MotorUpRunSound = value;
		elev->MotorDownRunSound = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 15) == "motoruprunsound")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		elev->MotorUpRunSound = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 17) == "motordownrunsound")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		elev->MotorDownRunSound = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 14) == "motorstopsound")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		elev->MotorUpStopSound = value;
		elev->MotorDownStopSound = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 16) == "motorupstopsound")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		elev->MotorUpStopSound = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 18) == "motordownstopsound")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		elev->MotorDownStopSound = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 14) == "motoridlesound")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		elev->MotorIdleSound = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 13) == "floorskiptext")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		elev->SetFloorSkipText(value);
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "recallfloor")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		int floortemp;
		std::string str = Calc(value);
		if (!IsNumeric(str, floortemp))
			return ScriptError("Invalid value");
		elev->SetRecallFloor(floortemp);
		return sNextLine;
	}
	if (linecheck.substr(0, 20) == "alternaterecallfloor")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		int floortemp;
		std::string str = Calc(value);
		if (!IsNumeric(str, floortemp))
			return ScriptError("Invalid value");
		elev->SetAlternateRecallFloor(floortemp);
		return sNextLine;
	}
	if (linecheck.substr(0, 8) == "acpfloor")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		int floortemp;
		std::string str = Calc(value);
		if (!IsNumeric(str, floortemp))
			return ScriptError("Invalid value");
		elev->SetACPFloor(floortemp);
		return sNextLine;
	}
	if (linecheck.substr(0, 13) == "motorposition")
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

		elev->MotorPosition = Ogre::Vector3(ToFloat(tempdata[0]), ToFloat(tempdata[1]), ToFloat(tempdata[2]));
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "queueresets")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		elev->QueueResets = ToBool(value);
		return sNextLine;
	}
	if (linecheck.substr(0, 10) == "limitqueue")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		elev->LimitQueue = ToBool(value);
		return sNextLine;
	}
	if (linecheck.substr(0, 3) == "acp")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		elev->ACP = ToBool(value);
		return sNextLine;
	}
	if (linecheck.substr(0, 6) == "uppeak")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		elev->UpPeak = ToBool(value);
		return sNextLine;
	}
	if (linecheck.substr(0, 8) == "downpeak")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		elev->DownPeak = ToBool(value);
		return sNextLine;
	}
	if (linecheck.substr(0, 17) == "inspectionservice")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		elev->InspectionService = ToBool(value);
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "fireservice1")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		elev->FireServicePhase1 = ToInt(value);
		return sNextLine;
	}
	if (linecheck.substr(0, 7) == "parking")
	{
		int params = SplitAfterEquals(LineData);
		if (params != 2)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 1; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		elev->ParkingFloor = ToInt(tempdata[0]);
		elev->ParkingDelay = ToFloat(tempdata[1]);
		return sNextLine;
	}
	if (linecheck.substr(0, 13) == "levelingspeed")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		float leveling;
		std::string str = Calc(value);
		if (!IsNumeric(str, leveling))
			return ScriptError("Invalid value");
		elev->LevelingSpeed = leveling;
		return sNextLine;
	}
	if (linecheck.substr(0, 14) == "levelingoffset")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		float leveling;
		std::string str = Calc(value);
		if (!IsNumeric(str, leveling))
			return ScriptError("Invalid value");
		elev->LevelingOffset = leveling;
		return sNextLine;
	}
	if (linecheck.substr(0, 12) == "levelingopen")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		float leveling;
		std::string str = Calc(value);
		if (!IsNumeric(str, leveling))
			return ScriptError("Invalid value");
		elev->LevelingOpen = leveling;
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "notifyearly")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		float notify;
		std::string str = Calc(value);
		if (!IsNumeric(str, notify))
			return ScriptError("Invalid value");
		elev->NotifyEarly = notify;
		return sNextLine;
	}
	if (linecheck.substr(0, 14) == "departuredelay")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		float delay;
		std::string str = Calc(value);
		if (!IsNumeric(str, delay))
			return ScriptError("Invalid value");
		elev->DepartureDelay = delay;
		return sNextLine;
	}
	if (linecheck.substr(0, 12) == "arrivaldelay")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		float delay;
		std::string str = Calc(value);
		if (!IsNumeric(str, delay))
			return ScriptError("Invalid value");
		elev->ArrivalDelay = delay;
		return sNextLine;
	}
	if (linecheck.substr(0, 15) == "inspectionspeed")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, elev->InspectionSpeed))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 9) == "autodoors")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		elev->AutoDoors = ToBool(value);
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "openonstart")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		elev->OpenOnStart = ToBool(value);
		return sNextLine;
	}
	if (linecheck.substr(0, 10) == "interlocks")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		elev->Interlocks = ToBool(value);
		return sNextLine;
	}
	if (linecheck.substr(0, 9) == "floorhold")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		elev->FloorHold = ToBool(value);
		return sNextLine;
	}
	if (linecheck.substr(0, 23) == "motoremergencystopsound")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		elev->MotorEmergencyStopSound = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 18) == "emergencystopspeed")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, elev->EmergencyStopSpeed))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 14) == "chimeonarrival")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		elev->ChimeOnArrival = ToBool(value);
		return sNextLine;
	}
	if (linecheck.substr(0, 6) == "reopen")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		elev->ReOpen = ToBool(value);
		return sNextLine;
	}
	if (linecheck.substr(0, 18) == "hoistwayaccesshold")
	{
		if (equals == false)
			return ScriptError("Syntax error");

		elev->HoistwayAccessHold = ToBool(value);
		return sNextLine;
	}

	//CreateElevator command
	if (linecheck.substr(0, 14) == "createelevator")
	{
		//get data
		int params = SplitData(LineData, 15);

		if (params != 4)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 3; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		bool result = elev->CreateElevator(ToBool(tempdata[0]), ToFloat(tempdata[1]), ToFloat(tempdata[2]), ToInt(tempdata[3]));
		if (result == false)
			return ScriptError();
		StoreCommand(elev);
		return sNextLine;
	}

	//process elevator car commands for default car
	int result = parent->GetElevatorCarSection()->Run(LineData);
	if (result != sContinue)
		return result;

	//handle end of elevator section
	if (linecheck == "<endelevator>" && config->RangeL == config->RangeH)
	{
		config->SectionNum = 0;
		config->Context = "None";
		engine->Report("Finished elevator");
		return sNextLine;
	}

	//handle elevator range
	if (config->RangeL != config->RangeH && linecheck.substr(0, 12) == "<endelevator")
	{
		if (config->Current < config->RangeH)
		{
			config->Current++;
			parent->line = config->RangeStart;  //loop back
			return sNextLine;
		}
		else
		{
			config->SectionNum = 0; //break out of loop
			config->Context = "None";
			config->RangeL = 0;
			config->RangeH = 0;
			engine->Report("Finished elevators");
			return sNextLine;
		}
	}

	return sContinue;
}

}
