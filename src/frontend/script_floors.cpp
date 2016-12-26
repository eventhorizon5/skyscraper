/* $Id$ */

/*
	Skyscraper 1.11 Alpha - Script Processor - Floor Section
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
#include "floor.h"
#include "wall.h"
#include "model.h"
#include "cameratexture.h"
#include "trigger.h"
#include "callbutton.h"
#include "shaft.h"
#include "stairs.h"
#include "control.h"
#include "elevator.h"
#include "elevatorcar.h"
#include "sound.h"
#include "floorindicator.h"
#include "door.h"
#include "revolvingdoor.h"
#include "directional.h"
#include "escalator.h"
#include "scriptprocessor.h"
#include "script_section.h"

using namespace SBS;

namespace Skyscraper {

ScriptProcessor::FloorSection::FloorSection(ScriptProcessor *parent) : Section(parent)
{
	floorcache_firstrun = true;
	cache_current = 0;
	cache_current_s = "";
	cache_height = 0;
	cache_height_s = "";
	cache_fullheight = 0;
	cache_fullheight_s = "";
	cache_interfloorheight = 0;
	cache_interfloorheight_s = "";
	cache_base = 0;
	cache_base_s = "";

	Reset();
}

void ScriptProcessor::FloorSection::Reset()
{
	FloorCheck = 0;
	callbutton_elevators.clear();
}

int ScriptProcessor::FloorSection::Run(std::string &LineData)
{
	//process floors

	Floor *floor = Simcore->GetFloor(config->Current);

	//exit with error if floor is invalid
	if (!floor)
	{
		std::string floornum;
		floornum = ToString(config->Current);
		return ScriptError("Invalid floor " + floornum);
	}

	//cache floor parameters
	if (cache_current != config->Current || floorcache_firstrun == true)
	{
		cache_current = config->Current;
		cache_current_s = ToString(cache_current);
	}
	if (cache_height != floor->Height || floorcache_firstrun == true)
	{
		cache_height = floor->Height;
		cache_height_s = ToString(cache_height);
	}
	if (cache_fullheight != floor->FullHeight() || floorcache_firstrun == true)
	{
		cache_fullheight = floor->FullHeight();
		cache_fullheight_s = ToString(cache_fullheight);
	}
	if (cache_interfloorheight != floor->InterfloorHeight || floorcache_firstrun == true)
	{
		cache_interfloorheight = floor->InterfloorHeight;
		cache_interfloorheight_s = ToString(cache_interfloorheight);
	}
	if (cache_base != floor->GetBase() || floorcache_firstrun == true)
	{
		cache_base = floor->GetBase();
		cache_base_s = ToString(cache_base);
	}

	floorcache_firstrun = false;

	//replace variables with actual values
	ReplaceAll(LineData, "%floor%", cache_current_s);
	ReplaceAll(LineData, "%height%", cache_height_s);
	ReplaceAll(LineData, "%fullheight%", cache_fullheight_s);
	ReplaceAll(LineData, "%interfloorheight%", cache_interfloorheight_s);
	ReplaceAll(LineData, "%base%", cache_base_s);
	ReplaceAll(LineData, "%floorid%", floor->ID);
	ReplaceAll(LineData, "%floorname%", floor->Name);
	ReplaceAll(LineData, "%floortype%", floor->FloorType);
	ReplaceAll(LineData, "%description%", floor->Description);

	if (parent->getfloordata == true)
		return sCheckFloors;

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

	//create a lowercase string of the line
	std::string linecheck = SetCaseCopy(LineData, false);

	//parameters
	if (linecheck.substr(0, 6) == "height")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, floor->Height))
			return ScriptError("Invalid value");
		if (FloorCheck < 2)
			FloorCheck = 1;
		else
			FloorCheck = 3;
	}
	if (linecheck.substr(0, 16) == "interfloorheight")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, floor->InterfloorHeight))
			return ScriptError("Invalid value");
		if (FloorCheck == 0)
			FloorCheck = 2;
		else
			FloorCheck = 3;
	}
	if (linecheck.substr(0, 8) == "altitude")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		float alt;
		if (!IsNumeric(str, alt))
			return ScriptError("Invalid value");
		floor->SetAltitude(alt);
		return sNextLine;
	}
	if (linecheck.substr(0, 2) == "id")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		floor->ID = Calc(value);
		return sNextLine;
	}
	if (linecheck.substr(0, 4) == "name")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		floor->Name = Calc(value);
		return sNextLine;
	}
	if (linecheck.substr(0, 4) == "type")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		floor->FloorType = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "description")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		floor->Description = value;
		return sNextLine;
	}
	if (linecheck.substr(0, 16) == "indicatortexture")
	{
		if (equals == false)
			return ScriptError("Syntax error");
		floor->IndicatorTexture = Calc(value);
		return sNextLine;
	}
	if (linecheck.substr(0, 5) == "group")
	{
		//copy string listing of group floors into array

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
					floor->AddGroupFloor(k);
			}
			else
			{
				int data;
				if (!IsNumeric(tempdata[line], data))
					return ScriptError("Invalid value");
				floor->AddGroupFloor(data);
			}
		}
		return sNextLine;
	}

	//calculate altitude
	if (FloorCheck == 3)
	{
		FloorCheck = 0;
		if (floor->CalculateAltitude() == false)
			return ScriptError();
		return sNextLine;
	}

	//Exit command
	if (linecheck.substr(0, 4) == "exit")
	{
		if (config->RangeL != config->RangeH)
			LineData = "<endfloors>";
		else
			LineData = "<endfloor>";

		//update linecheck
		linecheck = SetCaseCopy(LineData, false);
	}

	//AddFloor command
	if (linecheck.substr(0, 9) == "addfloor ")
	{
		//get data
		int params = SplitData(LineData, 9);

		if (params != 12 && params != 14)
			return ScriptError("Incorrect number of parameters");

		bool compat = false;
		if (params == 12)
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
			StoreCommand(floor->AddFloor(tempdata[0], tempdata[1], ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), config->ReverseAxis, false, ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToBool(tempdata[11]), true));
		else
			StoreCommand(floor->AddFloor(tempdata[0], tempdata[1], ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToBool(tempdata[9]), ToBool(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToBool(tempdata[13])));
		return sNextLine;
	}

	//AddShaftFloor command
	if (linecheck.substr(0, 13) == "addshaftfloor")
	{
		//get data
		int params = SplitData(LineData, 14);

		if (params != 12 && params != 14)
			return ScriptError("Incorrect number of parameters");

		bool compat = false;
		if (params == 12)
			compat = true;

		//check numeric values
		if (compat == true)
		{
			for (int i = 3; i <= 11; i++)
			{
				if (i == 1)
					i = 3; //skip non-numeric parameters
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
		}
		else
		{
			for (int i = 3; i <= 13; i++)
			{
				if (i == 1)
					i = 3; //skip non-numeric parameters
				if (i == 10)
					i = 12;
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		//create floor
		if (Simcore->GetShaft(ToInt(tempdata[0])))
		{
			//stop here if in Check mode
			if (config->CheckScript == true)
				return sNextLine;

			if (compat == true)
				StoreCommand(Simcore->GetShaft(ToInt(tempdata[0]))->AddFloor(config->Current, tempdata[1], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), config->ReverseAxis, false, ToFloat(tempdata[10]), ToFloat(tempdata[11]), true));
			else
				StoreCommand(Simcore->GetShaft(ToInt(tempdata[0]))->AddFloor(config->Current, tempdata[1], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToBool(tempdata[10]), ToBool(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13])));
		}
		else
			return ScriptError("Invalid shaft");
		return sNextLine;
	}

	//AddStairsFloor command
	if (linecheck.substr(0, 14) == "addstairsfloor")
	{
		//get data
		int params = SplitData(LineData, 14);

		if (params != 12 && params != 14)
			return ScriptError("Incorrect number of parameters");

		bool compat = false;
		if (params == 12)
			compat = true;

		//check numeric values
		if (compat == true)
		{
			for (int i = 3; i <= 11; i++)
			{
				if (i == 1)
					i = 3; //skip non-numeric parameters
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
		}
		else
		{
			for (int i = 3; i <= 13; i++)
			{
				if (i == 1)
					i = 3; //skip non-numeric parameters
				if (i == 10)
					i = 12;
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		//create floor
		if (Simcore->GetStairs(ToInt(tempdata[0])))
		{
			//stop here if in Check mode
			if (config->CheckScript == true)
				return sNextLine;

			if (compat == true)
				StoreCommand(Simcore->GetStairs(ToInt(tempdata[0]))->AddFloor(config->Current, tempdata[1], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), config->ReverseAxis, false, ToFloat(tempdata[10]), ToFloat(tempdata[11]), true));
			else
				StoreCommand(Simcore->GetStairs(ToInt(tempdata[0]))->AddFloor(config->Current, tempdata[1], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToBool(tempdata[10]), ToBool(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13])));
		}
		else
			return ScriptError("Invalid stairwell");
		return sNextLine;
	}

	//AddInterFloorFloor command
	if (linecheck.substr(0, 18) == "addinterfloorfloor")
	{
		//get data
		int params = SplitData(LineData, 19);

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
			StoreCommand(floor->AddInterfloorFloor(tempdata[0], tempdata[1], ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), config->ReverseAxis, false, ToFloat(tempdata[9]), ToFloat(tempdata[10]), true));
		else
			StoreCommand(floor->AddInterfloorFloor(tempdata[0], tempdata[1], ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToBool(tempdata[9]), ToBool(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12])));
		return sNextLine;
	}

	//AddWall command
	if (linecheck.substr(0, 7) == "addwall")
	{
		//get data
		int params = SplitData(LineData, 8);

		if (params != 14)
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
		StoreCommand(floor->AddWall(tempdata[0], tempdata[1], ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToBool(tempdata[13])));
		return sNextLine;
	}

	//AddShaftWall command
	if (linecheck.substr(0, 12) == "addshaftwall")
	{
		//get data
		int params = SplitData(LineData, 13);

		if (params != 14)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 13; i++)
		{
			if (i == 1)
				i = 3; //skip non-numeric parameters
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//create wall
		if (Simcore->GetShaft(ToInt(tempdata[0])))
		{
			//stop here if in Check mode
			if (config->CheckScript == true)
				return sNextLine;

			StoreCommand(Simcore->GetShaft(ToInt(tempdata[0]))->AddWall(config->Current, tempdata[1], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13])));
		}
		else
			return ScriptError("Invalid shaft");
		return sNextLine;
	}

	//AddStairsWall command
	if (linecheck.substr(0, 13) == "addstairswall")
	{
		//get data
		int params = SplitData(LineData, 14);

		if (params != 14)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 13; i++)
		{
			if (i == 1)
				i = 3; //skip non-numeric parameters
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//create wall
		if (Simcore->GetStairs(ToInt(tempdata[0])))
		{
			//stop here if in Check mode
			if (config->CheckScript == true)
				return sNextLine;

			StoreCommand(Simcore->GetStairs(ToInt(tempdata[0]))->AddWall(config->Current, tempdata[1], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13])));
		}
		else
			return ScriptError("Invalid stairwell");
		return sNextLine;
	}

	//AddInterFloorWall command
	if (linecheck.substr(0, 17) == "addinterfloorwall")
	{
		//get data
		int params = SplitData(LineData, 18);

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
		StoreCommand(floor->AddInterfloorWall(tempdata[0], tempdata[1], ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12])));
		return sNextLine;
	}

	//ColumnWallBox command
	if (linecheck.substr(0, 14) == "columnwallbox ")
	{
		//get data
		int params = SplitData(LineData, 14);

		if (params != 14)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 9; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		StoreCommand(floor->ColumnWallBox(tempdata[0], tempdata[1], ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToBool(tempdata[10]), ToBool(tempdata[11]), ToBool(tempdata[12]), ToBool(tempdata[13])));
		return sNextLine;
	}

	//ColumnWallBox2 command
	if (linecheck.substr(0, 14) == "columnwallbox2")
	{
		//get data
		int params = SplitData(LineData, 15);

		if (params != 14)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 9; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		StoreCommand(floor->ColumnWallBox2(tempdata[0], tempdata[1], ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToBool(tempdata[10]), ToBool(tempdata[11]), ToBool(tempdata[12]), ToBool(tempdata[13])));
		return sNextLine;
	}

	//CallButtonElevators command
	if (linecheck.substr(0, 19) == "callbuttonelevators")
	{
		//construct array containing floor numbers
		int params = SplitAfterEquals(LineData, false);
		if (params < 1)
			return ScriptError("Syntax Error");

		callbutton_elevators.clear();
		callbutton_elevators.resize(params);

		for (int line = 0; line < params; line++)
		{
			int elevnumber;
			if (!IsNumeric(tempdata[line], elevnumber))
				return ScriptError("Invalid elevator number");
			if (elevnumber < 1 || elevnumber > Simcore->GetElevatorCount())
				return ScriptError("Invalid elevator number");
			callbutton_elevators[line] = elevnumber;
		}
		return sNextLine;
	}

	//CreateCallButtons command
	if (linecheck.substr(0, 17) == "createcallbuttons")
	{
		if (callbutton_elevators.size() == 0)
			return ScriptError("No elevators specified");

		//get data
		int params = SplitData(LineData, 18);

		int compat = 0;
		if (params == 12)
			compat = 1;
		if (params == 14)
			compat = 2;

		//check numeric values
		if (compat == 1)
		{
			for (int i = 3; i <= 11; i++)
			{
				if (i == 6 || i == 9) //skip non-numeric parameters
					i++;
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
		}
		else if (compat == 2)
		{
			for (int i = 5; i <= 13; i++)
			{
				if (i == 8 || i == 11) //skip non-numeric parameters
					i++;
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
		}
		else
		{
			if (params != 15)
				return ScriptError("Incorrect number of parameters");

			for (int i = 6; i <= 14; i++)
			{
				if (i == 9 || i == 12) //skip non-numeric parameters
					i++;
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//create call button
		CallButton* callbutton = 0;
		if (compat == 1)
			callbutton = floor->AddCallButtons(callbutton_elevators, "", tempdata[0], tempdata[1], tempdata[1], tempdata[2], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), tempdata[6], ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToBool(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]));
		else if (compat == 2)
			callbutton = floor->AddCallButtons(callbutton_elevators, "", tempdata[0], tempdata[1], tempdata[2], tempdata[3], tempdata[4], ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), tempdata[8], ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToBool(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]));
		else
			callbutton = floor->AddCallButtons(callbutton_elevators, tempdata[0], tempdata[1], tempdata[2], tempdata[3], tempdata[4], tempdata[5], ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), tempdata[9], ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToBool(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]));

		if (callbutton)
		{
			if (config->lockvalue == 0)
				callbutton->SetLocked(false, config->keyvalue);
			else
				callbutton->SetLocked(true, config->keyvalue);
		}
		StoreCommand(callbutton);
		return sNextLine;
	}

	//AddStairs command
	if (linecheck.substr(0, 10) == "addstairs ")
	{
		//get data
		int params = SplitData(LineData, 10);

		if (params != 13)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 12; i++)
		{
			if (i == 1)
				i = 4; //skip non-numeric parameters
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//create stairs
		if (Simcore->GetStairs(ToInt(tempdata[0])))
		{
			//stop here if in Check mode
			if (config->CheckScript == true)
				return sNextLine;

			StoreCommand(Simcore->GetStairs(ToInt(tempdata[0]))->AddStairs(config->Current, tempdata[1], tempdata[2], tempdata[3], ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToInt(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12])));
		}
		else
			return ScriptError("Invalid stairwell");
		return sNextLine;
	}

	//AddDoor command
	if (linecheck.substr(0, 8) == "adddoor ")
	{
		//get data
		int params = SplitData(LineData, 8);

		if (params < 10 || params == 11 || params > 14)
			return ScriptError("Incorrect number of parameters");

		int compat = 0;

		//check numeric values
		if (params == 10)
		{
			for (int i = 1; i <= 9; i++)
			{
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			compat = 1;
		}
		if (params == 12)
		{
			for (int i = 3; i <= 11; i++)
			{
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			compat = 2;
		}
		if (params == 13)
		{
			for (int i = 3; i <= 12; i++)
			{
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			compat = 3;
		}
		if (params == 14)
		{
			for (int i = 4; i <= 13; i++)
			{
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		//check to see if file exists
		if (compat != 1)
		{
			parent->CheckFile("data/" + tempdata[0]);
			parent->CheckFile("data/" + tempdata[1]);
		}

		if (compat > 0 && warn_deprecated == true)
			ScriptWarning("Syntax deprecated");

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//create door
		Door* door;

		if (compat == 1)
			door = floor->AddDoor("", "", false, tempdata[0], ToFloat(tempdata[1]), ToInt(tempdata[2]), 0, ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]));
		if (compat == 2)
			door = floor->AddDoor(tempdata[0], tempdata[1], false, tempdata[2], ToFloat(tempdata[3]), ToInt(tempdata[4]), 0, ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]));
		if (compat == 3)
			door = floor->AddDoor(tempdata[0], tempdata[1], false, tempdata[2], ToFloat(tempdata[3]), ToInt(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]));
		if (compat == 0)
			door = floor->AddDoor(tempdata[0], tempdata[1], ToBool(tempdata[2]), tempdata[3], ToFloat(tempdata[4]), ToInt(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]));

		if (door)
			door->SetLocked(config->lockvalue, config->keyvalue);

		StoreCommand(door);
		return sNextLine;
	}

	//AddStairsDoor command
	if (linecheck.substr(0, 14) == "addstairsdoor ")
	{
		//get data
		int params = SplitData(LineData, 14);

		if (params < 11 || params == 12 || params > 15)
			return ScriptError("Incorrect number of parameters");

		int compat = 0;

		//check numeric values
		if (params == 11)
		{
			for (int i = 0; i <= 10; i++)
			{
				if (i == 1)
					i = 2; //skip non-numeric parameters
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			compat = 1;
		}
		if (params == 13)
		{
			for (int i = 0; i <= 12; i++)
			{
				if (i == 1)
					i = 4; //skip non-numeric parameters
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			compat = 2;
		}
		if (params == 14)
		{
			for (int i = 0; i <= 13; i++)
			{
				if (i == 1)
					i = 4; //skip non-numeric parameters
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			compat = 3;
		}
		if (params == 15)
		{
			for (int i = 0; i <= 14; i++)
			{
				if (i == 1)
					i = 5; //skip non-numeric parameters
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		//check to see if file exists
		if (compat != 1)
		{
			parent->CheckFile("data/" + tempdata[1]);
			parent->CheckFile("data/" + tempdata[2]);
		}

		if (compat > 0 && warn_deprecated == true)
			ScriptWarning("Syntax deprecated");

		//create door
		if (Simcore->GetStairs(ToInt(tempdata[0])))
		{
			//stop here if in Check mode
			if (config->CheckScript == true)
				return sNextLine;

			Door* door;

			if (compat == 1)
				door = Simcore->GetStairs(ToInt(tempdata[0]))->AddDoor(config->Current, "", "", false, tempdata[1], ToFloat(tempdata[2]), ToInt(tempdata[3]), 0, ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]));
			if (compat == 2)
				door = Simcore->GetStairs(ToInt(tempdata[0]))->AddDoor(config->Current, tempdata[1], tempdata[2], false, tempdata[3], ToFloat(tempdata[4]), ToInt(tempdata[5]), 0, ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]));
			if (compat == 3)
				door = Simcore->GetStairs(ToInt(tempdata[0]))->AddDoor(config->Current, tempdata[1], tempdata[2], false, tempdata[3], ToFloat(tempdata[4]), ToInt(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]));
			if (compat == 0)
				door = Simcore->GetStairs(ToInt(tempdata[0]))->AddDoor(config->Current, tempdata[1], tempdata[2], ToBool(tempdata[3]), tempdata[4], ToFloat(tempdata[5]), ToInt(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]));

			if (door)
				door->SetLocked(config->lockvalue, config->keyvalue);

			StoreCommand(door);
		}
		else
			return ScriptError("Invalid stairwell");
		return sNextLine;
	}

	//AddShaftStdDoor command
	if (linecheck.substr(0, 16) == "addshaftstddoor ")
	{
		//get data
		int params = SplitData(LineData, 16);

		if (params != 17)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 14; i++)
		{
			if (i == 1)
				i = 5; //skip non-numeric parameters
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//check to see if file exists
		parent->CheckFile("data/" + tempdata[1]);
		parent->CheckFile("data/" + tempdata[2]);

		//create door
		if (Simcore->GetShaft(ToInt(tempdata[0])))
		{
			//stop here if in Check mode
			if (config->CheckScript == true)
				return sNextLine;

			Door* door = Simcore->GetShaft(ToInt(tempdata[0]))->AddDoor(config->Current, tempdata[1], tempdata[2], ToBool(tempdata[3]), tempdata[4], ToFloat(tempdata[5]), ToInt(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]));

			if (door)
				door->SetLocked(config->lockvalue, config->keyvalue);

			StoreCommand(door);
		}
		else
			return ScriptError("Invalid shaft");
		return sNextLine;
	}

	//AddExternalDoor command
	if (linecheck.substr(0, 16) == "addexternaldoor ")
	{
		//get data
		int params = SplitData(LineData, 16);

		if (params != 14)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 4; i <= 13; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//check to see if file exists
		parent->CheckFile("data/" + tempdata[0]);
		parent->CheckFile("data/" + tempdata[1]);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//create door
		Door* door = floor->AddDoor(tempdata[0], tempdata[1], ToBool(tempdata[2]), tempdata[3], ToFloat(tempdata[4]), ToInt(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), true);

		if (door)
			door->SetLocked(config->lockvalue, config->keyvalue);

		StoreCommand(door);
		return sNextLine;
	}

	//AddDirectionalIndicator command
	if (linecheck.substr(0, 23) == "adddirectionalindicator")
	{
		//get data
		int params = SplitData(LineData, 24);

		if (params != 18 && params != 19)
			return ScriptError("Incorrect number of parameters");

		bool compat = false;

		//check numeric values
		if (params == 18)
		{
			for (int i = 9; i <= 17; i++)
			{
				if (i == 12)
					i = 13;
				if (i == 15)
					i = 16;
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			compat = true;
			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
		}
		if (params == 19)
		{
			for (int i = 10; i <= 18; i++)
			{
				if (i == 13)
					i = 14;
				if (i == 16)
					i = 17;
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		int elevator, car;
		if (!GetElevatorCar(tempdata[0], floor->Number, elevator, car))
			return sError;

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		if (compat == true)
			StoreCommand(floor->AddDirectionalIndicator(elevator, car, ToBool(tempdata[1]), false, ToBool(tempdata[2]), ToBool(tempdata[3]), tempdata[4], tempdata[5], tempdata[6], tempdata[7], tempdata[8], ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), tempdata[12], ToFloat(tempdata[13]), ToFloat(tempdata[14]), ToBool(tempdata[15]), ToFloat(tempdata[16]), ToFloat(tempdata[17])));
		else
			StoreCommand(floor->AddDirectionalIndicator(elevator, car, ToBool(tempdata[1]), ToBool(tempdata[2]), ToBool(tempdata[3]), ToBool(tempdata[4]), tempdata[5], tempdata[6], tempdata[7], tempdata[8], tempdata[9], ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), tempdata[13], ToFloat(tempdata[14]), ToFloat(tempdata[15]), ToBool(tempdata[16]), ToFloat(tempdata[17]), ToFloat(tempdata[18])));
		return sNextLine;
	}

	//AddShaftDoor command
	if (linecheck.substr(0, 13) == "addshaftdoor ")
	{
		//get data
		int params = SplitData(LineData, 13);

		if (params != 5 && params != 6 && params != 10)
			return ScriptError("Incorrect number of parameters");

		int compat = 0;
		if (params == 5)
			compat = 1; //1.4 compatibility mode
		if (params == 6)
			compat = 2;

		//exit if the SetShaftDoors command was never used
		if (compat > 0 && config->setshaftdoors == false)
			return ScriptError("SetShaftDoors must be used before AddShaftDoor");

		//check numeric values
		if (compat == 0)
		{
			for (int i = 1; i <= 9; i++)
			{
				if (i == 2)
					i = 4;
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}
		if (compat == 1)
		{
			for (int i = 1; i <= 4; i++)
			{
				if (i == 2)
					i = 3;
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}
		if (compat == 2)
		{
			for (int i = 1; i <= 5; i++)
			{
				if (i == 2)
					i = 4;
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		if (compat > 0 && warn_deprecated == true)
			ScriptWarning("Syntax deprecated");

		int elevator, carnum;
		if (!GetElevatorCar(tempdata[0], floor->Number, elevator, carnum))
			return sError;

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		Elevator *elev = Simcore->GetElevator(elevator);
		ElevatorCar *car = elev->GetCar(carnum);

		if (compat == 0)
			StoreCommand(car->AddShaftDoor(config->Current, ToInt(tempdata[1]), tempdata[2], tempdata[3], ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9])));
		if (compat == 1)
			StoreCommand(car->AddShaftDoor(config->Current, ToInt(tempdata[1]), tempdata[2], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4])));
		if (compat == 2)
			StoreCommand(car->AddShaftDoor(config->Current, ToInt(tempdata[1]), tempdata[2], tempdata[3], ToFloat(tempdata[4]), ToFloat(tempdata[5])));
		return sNextLine;
	}

	//AddFloorIndicator command
	if (linecheck.substr(0, 17) == "addfloorindicator")
	{
		//get data
		int params = SplitData(LineData, 18);

		if (params != 8 && params != 9)
			return ScriptError("Incorrect number of parameters");

		bool compat = false;
		if (params == 8)
			compat = true; //1.4 compatibility mode

		//check numeric values
		if (compat == true)
		{
			for (int i = 3; i <= 7; i++)
			{
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
		}
		else
		{
			for (int i = 4; i <= 8; i++)
			{
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		int elevator, car;
		if (!GetElevatorCar(tempdata[0], floor->Number, elevator, car))
			return sError;

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		if (compat == false)
			StoreCommand(floor->AddFloorIndicator(elevator, car, ToBool(tempdata[1]), tempdata[2], tempdata[3], ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8])));
		else
			StoreCommand(floor->AddFloorIndicator(elevator, car, ToBool(tempdata[1]), "Button", tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7])));
		return sNextLine;
	}

	//AddFillerWalls command
	if (linecheck.substr(0, 14) == "addfillerwalls")
	{
		//get data
		int params = SplitData(LineData, 15);

		if (params < 10 || params > 11)
			return ScriptError("Incorrect number of parameters");

		bool compat = false;

		if (params == 10)
			compat = true;

		//check numeric values
		for (int i = 1; i <= 9; i++)
		{
			if (i == 7)
				i = 8;
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		if (compat == true)
			floor->AddFillerWalls(tempdata[0], ToFloat(tempdata[1]), ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToBool(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), false);
		else
			floor->AddFillerWalls(tempdata[0], ToFloat(tempdata[1]), ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToBool(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToBool(tempdata[10]));

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

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		if (compat == true)
		{
			if (partial == true)
				StoreCommand(floor->AddSound(tempdata[0], tempdata[1], Ogre::Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]))));
			else
				StoreCommand(floor->AddSound(tempdata[0], tempdata[1], Ogre::Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4])), true, ToFloat(tempdata[5]), ToInt(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), 0.0, 360, 360, 1.0, Ogre::Vector3(ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]))));
		}
		else
		{
			if (partial == true)
				StoreCommand(floor->AddSound(tempdata[0], tempdata[1], Ogre::Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4])), ToBool(tempdata[5])));
			else
				StoreCommand(floor->AddSound(tempdata[0], tempdata[1], Ogre::Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4])), ToBool(tempdata[5]), ToFloat(tempdata[6]), ToInt(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), Ogre::Vector3(ToFloat(tempdata[14]), ToFloat(tempdata[15]), ToFloat(tempdata[16]))));
		}
		return sNextLine;
	}

	//AddShaftDoorComponent command
	if (linecheck.substr(0, 21) == "addshaftdoorcomponent")
	{
		//get data
		int params = SplitData(LineData, 22);

		if (params != 18 && params != 19)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		bool compat = false;
		if (params == 18)
		{
			compat = true;
			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
		}

		for (int i = 1; i <= params - 1; i++)
		{
			if (i == 2)
				i = 5;
			if (i == 6)
				i++;
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		int elevator, carnum;
		if (!GetElevatorCar(tempdata[0], floor->Number, elevator, carnum))
			return sError;

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		Elevator *elev = Simcore->GetElevator(elevator);
		ElevatorCar *car = elev->GetCar(carnum);

		if (compat == true)
			StoreCommand(car->AddShaftDoorComponent(ToInt(tempdata[1]), config->Current, tempdata[2], tempdata[3], tempdata[4], ToFloat(tempdata[5]), tempdata[6], ToFloat(tempdata[7]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]), ToFloat(tempdata[15]), ToFloat(tempdata[16]), ToFloat(tempdata[17])));
		else
			StoreCommand(car->AddShaftDoorComponent(ToInt(tempdata[1]), config->Current, tempdata[2], tempdata[3], tempdata[4], ToFloat(tempdata[5]), tempdata[6], ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]), ToFloat(tempdata[15]), ToFloat(tempdata[16]), ToFloat(tempdata[17]), ToFloat(tempdata[18])));
		return sNextLine;
	}

	//FinishShaftDoor command
	if (linecheck.substr(0, 16) == "finishshaftdoor ")
	{
		//get data
		int params = SplitData(LineData, 16);

		bool legacy = false;

		if (params < 2)
			return ScriptError("Incorrect number of parameters");

		if (params > 3)
		{
			if (IsNumeric(tempdata[2]) == true)
			{
				if (warn_deprecated == true)
					ScriptWarning("Syntax deprecated");
				legacy = true;
			}
		}

		//check numeric values
		for (int i = 1; i <= 1; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		int elevator, carnum;
		if (!GetElevatorCar(tempdata[0], floor->Number, elevator, carnum))
			return sError;

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		Elevator *elev = Simcore->GetElevator(elevator);
		ElevatorCar *car = elev->GetCar(carnum);

		if (params == 2 || legacy == true)
			StoreCommand(car->FinishShaftDoor(ToInt(tempdata[1]), config->Current));
		else if (params == 3)
			StoreCommand(car->FinishShaftDoor(ToInt(tempdata[1]), config->Current, ToBool(tempdata[2])));
		else
			StoreCommand(car->FinishShaftDoor(ToInt(tempdata[1]), config->Current, ToBool(tempdata[2]), ToBool(tempdata[3])));
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

		//stop here if in Check mode
		if (config->CheckScript == true)
		{
			config->setkey = false;
			return sNextLine;
		}

		//create model
		Model *model;
		if (compat == true)
			model = floor->AddModel(tempdata[0], tempdata[1], false, Ogre::Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4])), Ogre::Vector3(ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7])), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToBool(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]));
		else
			model = floor->AddModel(tempdata[0], tempdata[1], ToBool(tempdata[2]), Ogre::Vector3(ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5])), Ogre::Vector3(ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8])), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToBool(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]));

		if (config->setkey == true && model)
			model->SetKey(config->keyvalue);
		config->setkey = false;

		StoreCommand(model);
		return sNextLine;
	}

	//AddStairsModel command
	if (linecheck.substr(0, 14) == "addstairsmodel")
	{
		//get data
		int params = SplitData(LineData, 15, true);

		if (params != 15 && params != 16)
			return ScriptError("Incorrect number of parameters");

		bool compat = false;
		if (params == 15)
			compat = true;

		//check numeric values
		if (compat == true)
		{
			for (int i = 3; i <= 14; i++)
			{
				if (i == 11)
					i++;
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
		}
		else
		{
			for (int i = 4; i <= 15; i++)
			{
				if (i == 12)
					i++;
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		//check to see if file exists
		parent->CheckFile("data/" + tempdata[2]);

		//create model
		if (Simcore->GetStairs(ToInt(tempdata[0])))
		{
			//stop here if in Check mode
			if (config->CheckScript == true)
			{
				config->setkey = false;
				return sNextLine;
			}

			Model *model;

			if (compat == true)
				model = Simcore->GetStairs(ToInt(tempdata[0]))->AddModel(config->Current, tempdata[1], tempdata[2], false, Ogre::Vector3(ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5])), Ogre::Vector3(ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8])), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToBool(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]));
			else
				model = Simcore->GetStairs(ToInt(tempdata[0]))->AddModel(config->Current, tempdata[1], tempdata[2], ToBool(tempdata[3]), Ogre::Vector3(ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6])), Ogre::Vector3(ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9])), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToBool(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]), ToFloat(tempdata[15]));

			if (config->setkey == true && model)
				model->SetKey(config->keyvalue);
			config->setkey = false;

			StoreCommand(model);
		}
		else
			return ScriptError("Invalid stairwell");
		return sNextLine;
	}

	//AddShaftModel command
	if (linecheck.substr(0, 13) == "addshaftmodel")
	{
		//get data
		int params = SplitData(LineData, 14, true);

		if (params != 15 && params != 16)
			return ScriptError("Incorrect number of parameters");

		bool compat = false;
		if (params == 15)
			compat = true;

		//check numeric values
		if (compat == true)
		{
			for (int i = 3; i <= 14; i++)
			{
				if (i == 11)
					i++;
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
		}
		else
		{
			for (int i = 4; i <= 15; i++)
			{
				if (i == 12)
					i++;
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		//check to see if file exists
		parent->CheckFile("data/" + tempdata[2]);

		//create model
		if (Simcore->GetShaft(ToInt(tempdata[0])))
		{
			//stop here if in Check mode
			if (config->CheckScript == true)
			{
				config->setkey = false;
				return sNextLine;
			}

			Model *model;
			if (compat == true)
				model = Simcore->GetShaft(ToInt(tempdata[0]))->AddModel(config->Current, tempdata[1], tempdata[2], false, Ogre::Vector3(ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5])), Ogre::Vector3(ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8])), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToBool(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]));
			else
				model = Simcore->GetShaft(ToInt(tempdata[0]))->AddModel(config->Current, tempdata[1], tempdata[2], ToBool(tempdata[3]), Ogre::Vector3(ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6])), Ogre::Vector3(ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9])), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToBool(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]), ToFloat(tempdata[15]));

			if (config->setkey == true && model)
				model->SetKey(config->keyvalue);
			config->setkey = false;

			StoreCommand(model);
		}
		else
			return ScriptError("Invalid shaft");
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

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		Control* control = 0;
		if (compat == true)
			control = floor->AddControl(tempdata[0], tempdata[1], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), 1, action_array, tex_array);
		else
			control = floor->AddControl(tempdata[0], tempdata[1], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToInt(tempdata[8]), action_array, tex_array);

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

	//AddShaftActionControl command
	if (linecheck.substr(0, 21) == "addshaftactioncontrol")
	{
		//get data
		int params = SplitData(LineData, 21);

		if (params < 11)
			return ScriptError("Incorrect number of parameters");

		//set backwards compatibility
		bool compat = false;
		if (IsNumeric(tempdata[9]) == false)
			compat = true;

		int end = 9;
		if (compat == true)
			end = 8;

		//check numeric values
		for (int i = 0; i <= end; i++)
		{
			if (i == 1)
				i = 4;
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

		if (Simcore->GetShaft(ToInt(tempdata[0])))
		{
			//stop here if in Check mode
			if (config->CheckScript == true)
				return sNextLine;

			Control* control = 0;
			if (compat == true)
				control = Simcore->GetShaft(ToInt(tempdata[0]))->AddControl(config->Current, tempdata[1], tempdata[2], tempdata[3], ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), 1, action_array, tex_array);
			else
				control = Simcore->GetShaft(ToInt(tempdata[0]))->AddControl(config->Current, tempdata[1], tempdata[2], tempdata[3], ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToInt(tempdata[9]), action_array, tex_array);

			if (control)
			{
				if (config->lockvalue == 0)
					control->SetLocked(false, config->keyvalue);
				else
					control->SetLocked(true, config->keyvalue);
			}
			StoreCommand(control);
		}
		else
			return ScriptError("Invalid shaft");
		return sNextLine;
	}

	//AddStairsActionControl command
	if (linecheck.substr(0, 21) == "addstairsactioncontrol")
	{
		//get data
		int params = SplitData(LineData, 21);

		if (params < 11)
			return ScriptError("Incorrect number of parameters");

		//set backwards compatibility
		bool compat = false;
		if (IsNumeric(tempdata[9]) == false)
			compat = true;

		int end = 9;
		if (compat == true)
			end = 8;

		//check numeric values
		for (int i = 0; i <= end; i++)
		{
			if (i == 1)
				i = 4;
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

		if (Simcore->GetStairs(ToInt(tempdata[0])))
		{
			//stop here if in Check mode
			if (config->CheckScript == true)
				return sNextLine;

			Control* control = 0;
			if (compat == true)
				control = Simcore->GetStairs(ToInt(tempdata[0]))->AddControl(config->Current, tempdata[1], tempdata[2], tempdata[3], ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), 1, action_array, tex_array);
			else
				control = Simcore->GetStairs(ToInt(tempdata[0]))->AddControl(config->Current, tempdata[1], tempdata[2], tempdata[3], ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToInt(tempdata[9]), action_array, tex_array);

			if (control)
			{
				if (config->lockvalue == 0)
					control->SetLocked(false, config->keyvalue);
				else
					control->SetLocked(true, config->keyvalue);
			}
			StoreCommand(control);
		}
		else
			return ScriptError("Invalid stairwell");
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

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		Ogre::Vector3 min = Ogre::Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]));
		Ogre::Vector3 max = Ogre::Vector3(ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]));
		StoreCommand(floor->AddTrigger(tempdata[0], tempdata[1], min, max, action_array));
		return sNextLine;
	}

	//AddShaftTrigger command
	/*if (linecheck.substr(0, 15) == "addshafttrigger")
	{
		//get data
		int params = SplitData(LineData, 16);

		if (params < 10)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 8; i++)
		{
			if (i == 1)
				i = 3;
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::vector<std::string> action_array;

		//get number of action & texture parameters
		for (int i = 9; i < params; i++)
			action_array.push_back(tempdata[i]);

		//check to see if file exists
		parent->CheckFile("data/" + tempdata[1]);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		Ogre::Vector3 min = Ogre::Vector3(ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]));
		Ogre::Vector3 max = Ogre::Vector3(ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]));
		if (Simcore->GetShaft(ToInt(tempdata[0])))
			StoreCommand(Simcore->GetShaft(ToInt(tempdata[0]))->AddTrigger(Current, tempdata[1], tempdata[2], min, max, action_array));
		else
			return ScriptError("Invalid shaft");
		return sNextLine;
	}

	//AddStairsTrigger command
	if (linecheck.substr(0, 15) == "addstairstrigger")
	{
		//get data
		int params = SplitData(LineData, 16);

		if (params < 10)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 8; i++)
		{
			if (i == 1)
				i = 3;
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::vector<std::string> action_array;

		//get number of action & texture parameters
		for (int i = 9; i < params; i++)
			action_array.push_back(tempdata[i]);

		//check to see if file exists
		parent->CheckFile("data/" + tempdata[1]);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		Ogre::Vector3 min = Ogre::Vector3(ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]));
		Ogre::Vector3 max = Ogre::Vector3(ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]));
		if (Simcore->GetStairs(ToInt(tempdata[0])))
			StoreCommand(Simcore->GetStairs(ToInt(tempdata[0]))->AddTrigger(Current, tempdata[1], tempdata[2], min, max, action_array));
		else
			return ScriptError("Invalid stairwell");
		return sNextLine;
	}*/

	//AddCameraTexture command
	if (linecheck.substr(0, 16) == "addcameratexture")
	{
		//get data
		int params = SplitData(LineData, 17);

		if (params != 11)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 10; i++)
		{
			if (i == 7)
				i++;

			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		StoreCommand(floor->AddCameraTexture(tempdata[0], ToBool(tempdata[1]), ToInt(tempdata[2]), ToFloat(tempdata[3]), Ogre::Vector3(ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6])), ToBool(tempdata[7]), Ogre::Vector3(ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]))));
		return sNextLine;
	}

	//Cut command
	if (linecheck.substr(0, 4) == "cut ")
	{
		//get data
		int params = SplitData(LineData, 4);

		if (params != 8)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 5; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//perform cut on floor
		floor->Cut(Ogre::Vector3(ToFloat(tempdata[0]), ToFloat(tempdata[1]), ToFloat(tempdata[2])), Ogre::Vector3(ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5])), ToBool(tempdata[6]), ToBool(tempdata[7]), false);
		return sNextLine;
	}

	//CutAll command
	if (linecheck.substr(0, 6) == "cutall")
	{
		//get data
		int params = SplitData(LineData, 7);

		if (params != 8)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 5; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//perform cut on all objects related to the current floor
		floor->CutAll(Ogre::Vector3(ToFloat(tempdata[0]), ToFloat(tempdata[1]), ToFloat(tempdata[2])), Ogre::Vector3(ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5])), ToBool(tempdata[6]), ToBool(tempdata[7]));
		return sNextLine;
	}

	//AddRevolvingDoor command
	if (linecheck.substr(0, 17) == "addrevolvingdoor ")
	{
		//get data
		int params = SplitData(LineData, 17);

		if (params != 15)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 13; i++)
		{
			if (i == 3)
				i++;

			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//check to see if file exists
		parent->CheckFile("data/" + tempdata[0]);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//create door
		RevolvingDoor* door = floor->AddRevolvingDoor(tempdata[0], tempdata[1], ToFloat(tempdata[2]), ToBool(tempdata[3]), ToInt(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToBool(tempdata[14]));

		if (door)
		{
			if (config->lockvalue == 0)
				door->SetLocked(false, config->keyvalue);
			else
				door->SetLocked(true, config->keyvalue);
		}

		StoreCommand(door);
		return sNextLine;
	}

	//AddEscalator command
	if (linecheck.substr(0, 13) == "addescalator ")
	{
		//get data
		int params = SplitData(LineData, 13);

		if (params != 15)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 14; i++)
		{
			if (i == 2)
				i = 6; //skip non-numeric parameters
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//create escalator

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		StoreCommand(floor->AddEscalator(tempdata[0], ToInt(tempdata[1]), ToFloat(tempdata[2]), tempdata[3], tempdata[4], tempdata[5], ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToInt(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14])));

		return sNextLine;
	}

	//handle end of floor section
	if (linecheck == "<endfloor>" && config->RangeL == config->RangeH)
	{
		//when finishing a floor, make sure the altitude is valid
		if (floor->AltitudeSet == false)
			return ScriptError("Floor altitude or height has not been set");

		config->SectionNum = 0;
		config->Context = "None";
		engine->Report("Finished floor");
		return sNextLine;
	}

	//handle floor range
	if (config->RangeL != config->RangeH && linecheck.substr(0, 9) == "<endfloor")
	{
		//when finishing a floor, make sure the altitude is valid
		if (floor->AltitudeSet == false)
			return ScriptError("Floor altitude is invalid");

		if (config->RangeL < config->RangeH)
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
				engine->Report("Finished floors");
				return sNextLine;
			}
		}
		else
		{
			if (config->Current > config->RangeH)
			{
				config->Current--;
				parent->line = config->RangeStart; //loop back
				return sNextLine;
			}
			else
			{
				config->SectionNum = 0; //break out of loop
				config->Context = "None";
				config->RangeL = 0;
				config->RangeH = 0;
				engine->Report("Finished floors");
				return sNextLine;
			}
		}
	}

	return sContinue;
}

}
