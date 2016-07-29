/* $Id$ */

/*
	Skyscraper 1.10 Alpha - Script Processor - Elevator Section
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

using namespace SBS;

namespace Skyscraper {

int ScriptProcessor::ProcElevators()
{
	//Process elevators

	//create elevator if not created already
	Simcore->NewElevator(Current);

	//replace variables with actual values
	ReplaceAll(LineData, "%elevator%", ToString(Current));

	//IF/While statement stub (continue to global commands for processing)
	if (SetCaseCopy(LineData.substr(0, 2), false) == "if" || SetCaseCopy(LineData.substr(0, 5), false) == "while")
		return sContinue;

	//process math functions
	if (MathFunctions() == sError)
		return sError;

	//process functions
	if (FunctionProc() == true)
		return sNextLine;

	//get text after equal sign
	int temp2check = LineData.find("=", 0);
	temp2 = GetAfterEquals(LineData);

	//get elevator object
	Elevator *elev = Simcore->GetElevator(Current);

	//create a lowercase string of the line
	std::string linecheck = SetCaseCopy(LineData, false);

	//parameters
	if (linecheck.substr(0, 4) == "name")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->Name = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 4) == "type")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->Type = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 5) == "speed")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		std::string str = Calc(temp2);
		if (!IsNumeric(str, elev->ElevatorSpeed))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 12) == "acceleration")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		std::string str = Calc(temp2);
		if (!IsNumeric(str, elev->Acceleration))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 12) == "deceleration")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		std::string str = Calc(temp2);
		if (!IsNumeric(str, elev->Deceleration))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 9) == "acceljerk")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		std::string str = Calc(temp2);
		if (!IsNumeric(str, elev->AccelJerk))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 9) == "deceljerk")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		std::string str = Calc(temp2);
		if (!IsNumeric(str, elev->DecelJerk))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 13) == "assignedshaft")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		std::string str = Calc(temp2);
		if (!IsNumeric(str, elev->AssignedShaft))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 15) == "motorstartsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile("data/" + temp2);

		elev->MotorUpStartSound = temp2;
		elev->MotorDownStartSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 17) == "motorupstartsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile("data/" + temp2);

		elev->MotorUpStartSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 19) == "motordownstartsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile("data/" + temp2);

		elev->MotorDownStartSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 13) == "motorrunsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile("data/" + temp2);

		elev->MotorUpRunSound = temp2;
		elev->MotorDownRunSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 15) == "motoruprunsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile("data/" + temp2);

		elev->MotorUpRunSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 17) == "motordownrunsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile("data/" + temp2);

		elev->MotorDownRunSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 14) == "motorstopsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile("data/" + temp2);

		elev->MotorUpStopSound = temp2;
		elev->MotorDownStopSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 16) == "motorupstopsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile("data/" + temp2);

		elev->MotorUpStopSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 18) == "motordownstopsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile("data/" + temp2);

		elev->MotorDownStopSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 14) == "motoridlesound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile("data/" + temp2);

		elev->MotorIdleSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 13) == "floorskiptext")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->SetFloorSkipText(temp2);
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "recallfloor")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		int floortemp;
		std::string str = Calc(temp2);
		if (!IsNumeric(str, floortemp))
			return ScriptError("Invalid value");
		elev->SetRecallFloor(floortemp);
		return sNextLine;
	}
	if (linecheck.substr(0, 20) == "alternaterecallfloor")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		int floortemp;
		std::string str = Calc(temp2);
		if (!IsNumeric(str, floortemp))
			return ScriptError("Invalid value");
		elev->SetAlternateRecallFloor(floortemp);
		return sNextLine;
	}
	if (linecheck.substr(0, 8) == "acpfloor")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		int floortemp;
		std::string str = Calc(temp2);
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
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->QueueResets = ToBool(temp2);
		return sNextLine;
	}
	if (linecheck.substr(0, 10) == "limitqueue")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->LimitQueue = ToBool(temp2);
		return sNextLine;
	}
	if (linecheck.substr(0, 3) == "acp")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->ACP = ToBool(temp2);
		return sNextLine;
	}
	if (linecheck.substr(0, 6) == "uppeak")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->UpPeak = ToBool(temp2);
		return sNextLine;
	}
	if (linecheck.substr(0, 8) == "downpeak")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->DownPeak = ToBool(temp2);
		return sNextLine;
	}
	if (linecheck.substr(0, 17) == "inspectionservice")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->InspectionService = ToBool(temp2);
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "fireservice1")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->FireServicePhase1 = ToInt(temp2);
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
		if (temp2check < 0)
			return ScriptError("Syntax error");
		float leveling;
		std::string str = Calc(temp2);
		if (!IsNumeric(str, leveling))
			return ScriptError("Invalid value");
		elev->LevelingSpeed = leveling;
		return sNextLine;
	}
	if (linecheck.substr(0, 14) == "levelingoffset")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		float leveling;
		std::string str = Calc(temp2);
		if (!IsNumeric(str, leveling))
			return ScriptError("Invalid value");
		elev->LevelingOffset = leveling;
		return sNextLine;
	}
	if (linecheck.substr(0, 12) == "levelingopen")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		float leveling;
		std::string str = Calc(temp2);
		if (!IsNumeric(str, leveling))
			return ScriptError("Invalid value");
		elev->LevelingOpen = leveling;
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "notifyearly")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		float notify;
		std::string str = Calc(temp2);
		if (!IsNumeric(str, notify))
			return ScriptError("Invalid value");
		elev->NotifyEarly = notify;
		return sNextLine;
	}
	if (linecheck.substr(0, 14) == "departuredelay")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		float delay;
		std::string str = Calc(temp2);
		if (!IsNumeric(str, delay))
			return ScriptError("Invalid value");
		elev->DepartureDelay = delay;
		return sNextLine;
	}
	if (linecheck.substr(0, 12) == "arrivaldelay")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		float delay;
		std::string str = Calc(temp2);
		if (!IsNumeric(str, delay))
			return ScriptError("Invalid value");
		elev->ArrivalDelay = delay;
		return sNextLine;
	}
	if (linecheck.substr(0, 15) == "inspectionspeed")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		std::string str = Calc(temp2);
		if (!IsNumeric(str, elev->InspectionSpeed))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 9) == "autodoors")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		elev->AutoDoors = ToBool(temp2);
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "openonstart")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		elev->OpenOnStart = ToBool(temp2);
		return sNextLine;
	}
	if (linecheck.substr(0, 10) == "interlocks")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		elev->Interlocks = ToBool(temp2);
		return sNextLine;
	}
	if (linecheck.substr(0, 9) == "floorhold")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		elev->FloorHold = ToBool(temp2);
		return sNextLine;
	}
	if (linecheck.substr(0, 23) == "motoremergencystopsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile("data/" + temp2);

		elev->MotorEmergencyStopSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 18) == "emergencystopspeed")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		std::string str = Calc(temp2);
		if (!IsNumeric(str, elev->EmergencyStopSpeed))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 14) == "chimeonarrival")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->ChimeOnArrival = ToBool(temp2);
		return sNextLine;
	}
	if (linecheck.substr(0, 6) == "reopen")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		elev->ReOpen = ToBool(temp2);
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
	int value = ProcElevatorCars();
	if (value != sContinue)
		return value;

	//handle elevator range
	if (RangeL != RangeH && linecheck.substr(0, 12) == "<endelevator")
	{
		if (Current < RangeH)
		{
			Current++;
			line = RangeStart;  //loop back
			return sNextLine;
		}
		else
		{
			Section = 0; //break out of loop
			Context = "None";
			RangeL = 0;
			RangeH = 0;
			engine->Report("Finished elevators");
			return sNextLine;
		}
	}

	return sContinue;
}

}
