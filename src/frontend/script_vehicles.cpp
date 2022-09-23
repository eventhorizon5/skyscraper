/* $Id$ */

/*
	Skyscraper 1.11 Alpha - Script Processor - Vehicle Section
	Copyright (C)2003-2018 Ryan Thoryk
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
#include "vehicle.h"
#include "scriptprocessor.h"
#include "script_section.h"

using namespace SBS;

namespace Skyscraper {

ScriptProcessor::VehicleSection::VehicleSection(ScriptProcessor *parent) : Section(parent)
{

}

int ScriptProcessor::VehicleSection::Run(std::string &LineData)
{
	//Process vehicles

	//create vehicle if not created already
	Simcore->NewVehicle(config->Current);

	//replace variables with actual values
	ReplaceAll(LineData, "%vehicle%", ToString(config->Current));

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

	//get vehicle object
	Vehicle *v = Simcore->GetVehicle(config->Current);

	//create a lowercase string of the line
	std::string linecheck = SetCaseCopy(LineData, false);

	//parameters
	if (linecheck.substr(0, 4) == "name")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		v->Name = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 14) == "maxengineforce")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, v->MaxEngineForce))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 17) == "steeringincrement")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, v->SteeringIncrement))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 13) == "steeringclamp")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, v->SteeringClamp))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 13) == "wheelfriction")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, v->WheelFriction))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 19) == "suspensionstiffness")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, v->SuspensionStiffness))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 17) == "suspensiondamping")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, v->SuspensionDamping))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 21) == "suspensioncompression")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, v->SuspensionCompression))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 13) == "rollinfluence")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, v->RollInfluence))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 20) == "suspensionrestlength")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, v->SuspensionRestLength))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 21) == "maxsuspensiontravelcm")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, v->MaxSuspensionTravelCm))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 12) == "frictionslip")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, v->FrictionSlip))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 12) == "chassisshift")
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

		v->ChassisShift = Ogre::Vector3(ToFloat(tempdata[0]), ToFloat(tempdata[1]), ToFloat(tempdata[2]));
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "chassismesh")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		v->ChassisMesh = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 9) == "wheelmesh")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		v->WheelMesh = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 12) == "chassisscale")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, v->ChassisScale))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 10) == "wheelscale")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, v->WheelScale))
			return ScriptError("Invalid value");
		return sNextLine;
	}

	//CreateVehicle command
	if (linecheck.substr(0, 13) == "createvehicle")
	{
		//get data
		int params = SplitData(LineData, 14);

		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 2; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		Ogre::Vector3 position = Ogre::Vector3(ToFloat(tempdata[0]), ToFloat(tempdata[1]), ToInt(tempdata[2]));
		bool result = v->Create(position);
		if (result == false)
			return ScriptError();
		StoreCommand(v);
		return sNextLine;
	}

	//CreateChassis command
	if (linecheck.substr(0, 13) == "createchassis")
	{
		//get data
		int params = SplitData(LineData, 14);

		if (params != 5)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 4; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		bool result = v->CreateChassis(ToFloat(tempdata[0]), ToFloat(tempdata[1]), ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]));
		if (result == false)
			return ScriptError();
		return sNextLine;
	}

	//AddWheel command
	if (linecheck.substr(0, 8) == "addwheel")
	{
		//get data
		int params = SplitData(LineData, 9);

		if (params != 7 && params != 13)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i < params; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		Ogre::Vector3 connection = Ogre::Vector3(ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]));
		Ogre::Vector3 direction, axle;
		if (params > 7)
		{
			direction = Ogre::Vector3(ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]));
			axle = Ogre::Vector3(ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]));
		}
		bool result;
		if (params == 7)
			result = v->AddWheel(ToBool(tempdata[0]), ToBool(tempdata[1]), ToBool(tempdata[2]), ToFloat(tempdata[3]), connection);
		else
			result = v->AddWheel(ToBool(tempdata[0]), ToBool(tempdata[1]), ToBool(tempdata[2]), ToFloat(tempdata[3]), connection, direction, axle);

		if (result == false)
			return ScriptError();
		return sNextLine;
	}

	//handle end of vehicle section
	if (linecheck == "<endvehicle>" && config->RangeL == config->RangeH)
	{
		config->SectionNum = 0;
		config->Context = "None";
		engine->Report("Finished vehicle");
		return sNextLine;
	}

	//handle vehicle range
	if (config->RangeL != config->RangeH && linecheck.substr(0, 12) == "<endvehicle")
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
			engine->Report("Finished vehicles");
			return sNextLine;
		}
	}

	return sContinue;
}

}
