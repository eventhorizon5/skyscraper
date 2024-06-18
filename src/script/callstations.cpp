/*
	Skyscraper 1.12 Alpha - Script Processor - Call Stations Section
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
#include "floor.h"
#include "callstation.h"
#include "buttonpanel.h"
#include "control.h"
#include "indicator.h"
#include "scriptproc.h"
#include "section.h"

using namespace SBS;

namespace Skyscraper {

ScriptProcessor::CallStationSection::CallStationSection(ScriptProcessor *parent) : Section(parent)
{

}

int ScriptProcessor::CallStationSection::Run(std::string &LineData)
{
	//Process call stations

	//get call station object
	CallStation *station = 0;
	Floor *floor = 0;

	floor = Simcore->GetFloor(config->CurrentOld);
	if (floor)
		station = floor->GetCallStation(config->Current);
	else
		return sError;

	//create call station if not created already
	if (!station)
		station = floor->AddCallStation(config->Current);

	if (!station)
		return sError;

	//replace variables with actual values
	if (config->SectionNum == 9) //only run if not being called from floor function
	{
		ReplaceAll(LineData, "%floor%", ToString(config->CurrentOld));
		ReplaceAll(LineData, "%callstation%", ToString(config->Current));

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
		station->Name = value;
		return sNextLine;
	}
	//Controller parameter
	if (StartsWithNoCase(LineData, "controller"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		int num = 0;
		if (!IsNumeric(str, num))
			return ScriptError("Invalid value");
		station->SetController(num);
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
			inputs.push_back(tempdata[line]);

		station->InvalidInput = inputs;

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
		station->TimerDelay = num;
		return sNextLine;
	}

	//commands

	//CreatePanel command
	if (StartsWithNoCase(LineData, "createpanel"))
	{
		//get data
		int params = SplitData(LineData, 12);

		if (params != 10)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 9; i++)
		{
			if (i == 3)
				i = 4;
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		StoreCommand(station->CreateButtonPanel(tempdata[0], ToInt(tempdata[1]), ToInt(tempdata[2]), tempdata[3], ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9])));
		return sNextLine;
	}

	//AddControl command
	if (StartsWithNoCase(LineData, "addcontrol "))
	{
		//get data
		int params = SplitData(LineData, 11);

		if (params < 10)
			return ScriptError("Incorrect number of parameters");

		int end = 7;

		//check numeric values
		for (int i = 0; i <= end; i++)
		{
			if (i == 0)
				i++;
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		if (!station->GetPanel())
			return ScriptError("Panel not created yet");

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
		parent->CheckFile("data/" + tempdata[0]);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		Control* control = 0;
		control = station->GetPanel()->AddControl(tempdata[0], ToInt(tempdata[1]), ToInt(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToInt(tempdata[7]), action_array, tex_array);

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

	//SetPosition command
	if (StartsWithNoCase(LineData, "setposition "))
	{
		//get data
		int params = SplitData(LineData, 12);

		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i < 2; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		Vector3 position = Vector3(ToFloat(tempdata[0]), ToFloat(tempdata[1]), ToFloat(tempdata[2]));
		station->SetPosition(position);
	}

	//AddDisplay command
	if (StartsWithNoCase(LineData, "addindicator"))
	{
		//get data
		int params = SplitData(LineData, 13);

		if (params != 10)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 4; i <= 9; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		StoreCommand(station->AddIndicator(tempdata[0], tempdata[1], tempdata[2], tempdata[3], ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9])));
		return sNextLine;
	}

	//handle end of call station section
	if (StartsWithNoCase(LineData, "<endcallstation>") && config->RangeL == config->RangeH)
	{
		//return to floor section
		config->SectionNum = 2;
		config->Context = config->ContextOld;
		config->Current = config->CurrentOld;
		config->RangeL = config->RangeLOld;
		config->RangeH = config->RangeHOld;
		config->RangeStart = config->RangeStartOld;

		if (Simcore->Verbose)
			engine->Report("Finished call station");
		return sNextLine;
	}

	//handle call station range
	if (config->RangeL != config->RangeH && StartsWithNoCase(LineData, "<endcallstation"))
	{
		if (config->Current < config->RangeH)
		{
			config->Current++;
			parent->line = config->RangeStart;  //loop back
			return sNextLine;
		}
		else
		{
			config->SectionNum = 2; //break out of loop
			config->Context = config->ContextOld;
			config->RangeL = config->RangeLOld;
			config->RangeH = config->RangeHOld;
			config->RangeStart = config->RangeStartOld;
			config->Current = config->CurrentOld;
			if (Simcore->Verbose)
				engine->Report("Finished call stations");
			return sNextLine;
		}
	}

	return sContinue;
}

}
