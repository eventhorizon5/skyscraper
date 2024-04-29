/*
	Skyscraper 1.12 Alpha - Script Processor - Vehicle Section
	Copyright (C)2003-2024 Ryan Thoryk
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
#include "skyscraper.h"
#include "enginecontext.h"
#include "vehicle.h"
#include "scriptproc.h"
#include "section.h"

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
	if (StartsWithNoCase(LineData, "if") || StartsWithNoCase(LineData, "while"))
		return sContinue;

	//process math functions
	if (MathFunctions(LineData) == sError)
		return sError;

	//process functions
	if (parent->FunctionProc() == true)
		return sNextLine;

	//get text after equal sign
	bool equals;
	std::string value = GetAfterEquals(LineData, equals);

	//get vehicle object
	Vehicle *v = Simcore->GetVehicle(config->Current);

	//parameters

	//Name parameter
	if (StartsWithNoCase(LineData, "name"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		v->Name = value;
		return sNextLine;
	}
	//MaxEngineForce parameter
	if (StartsWithNoCase(LineData, "maxengineforce"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, v->MaxEngineForce))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	//SteeringIncrement parameter
	if (StartsWithNoCase(LineData, "steeringincrement"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, v->SteeringIncrement))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	//SteeringClamp parameter
	if (StartsWithNoCase(LineData, "steeringclamp"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, v->SteeringClamp))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	//WheelFriction parameter
	if (StartsWithNoCase(LineData, "wheelfriction"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, v->WheelFriction))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	//SuspensionStiffness parameter
	if (StartsWithNoCase(LineData, "suspensionstiffness"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, v->SuspensionStiffness))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	//SuspensionDamping parameter
	if (StartsWithNoCase(LineData, "suspensiondamping"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, v->SuspensionDamping))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	//SuspensionCompression parameter
	if (StartsWithNoCase(LineData, "suspensioncompression"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, v->SuspensionCompression))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	//RollInfluence parameter
	if (StartsWithNoCase(LineData, "rollinfluence"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, v->RollInfluence))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	//SuspensionRestLength parameter
	if (StartsWithNoCase(LineData, "suspensionrestlength"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, v->SuspensionRestLength))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	//MaxSuspensionTravelCm parameter
	if (StartsWithNoCase(LineData, "maxsuspensiontravelcm"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, v->MaxSuspensionTravelCm))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	//FrictionSlip parameter
	if (StartsWithNoCase(LineData, "frictionslip"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, v->FrictionSlip))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	//ChassisShift parameter
	if (StartsWithNoCase(LineData, "chassisshift"))
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

		v->ChassisShift = Vector3(ToFloat(tempdata[0]), ToFloat(tempdata[1]), ToFloat(tempdata[2]));
		return sNextLine;
	}
	//ChassisMesh parameter
	if (StartsWithNoCase(LineData, "chassismesh"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		v->ChassisMesh = value;
		return sNextLine;
	}
	//WheelMesh parameter
	if (StartsWithNoCase(LineData, "wheelmesh"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		v->WheelMesh = value;
		return sNextLine;
	}
	//ChassisScale parameter
	if (StartsWithNoCase(LineData, "chassisscale"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, v->ChassisScale))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	//WheelScale parameter
	if (StartsWithNoCase(LineData, "wheelscale"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, v->WheelScale))
			return ScriptError("Invalid value");
		return sNextLine;
	}

	//CreateVehicle command
	if (StartsWithNoCase(LineData, "createvehicle"))
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

		Vector3 position = Vector3(ToFloat(tempdata[0]), ToFloat(tempdata[1]), ToInt(tempdata[2]));
		bool result = v->Create(position);
		if (result == false)
			return ScriptError();
		StoreCommand(v);
		return sNextLine;
	}

	//CreateChassis command
	if (StartsWithNoCase(LineData, "createchassis"))
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
	if (StartsWithNoCase(LineData, "addwheel"))
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

		Vector3 connection = Vector3(ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]));
		Vector3 direction, axle;
		if (params > 7)
		{
			direction = Vector3(ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]));
			axle = Vector3(ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]));
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
	if (StartsWithNoCase(LineData, "<endvehicle>") && config->RangeL == config->RangeH)
	{
		config->SectionNum = 0;
		config->Context = "None";
		engine->Report("Finished vehicle");
		return sNextLine;
	}

	//handle vehicle range
	if (config->RangeL != config->RangeH && StartsWithNoCase(LineData, "<endvehicle"))
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
