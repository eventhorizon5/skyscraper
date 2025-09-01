/*
	Skyscraper 2.1 - Script Processor - Buildings Section
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
#include "vm.h"
#include "enginecontext.h"
#include "scriptproc.h"
#include "section.h"

using namespace SBS;

namespace Skyscraper {

ScriptProcessor::BuildingsSection::BuildingsSection(ScriptProcessor *parent) : Section(parent)
{

}

int ScriptProcessor::BuildingsSection::Run(std::string &LineData)
{
	//Load additional buildings

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

	//parameters

	if (engine->IsRoot() == true) //only run these commands if the engine is root
	{
		//ConcurrentLoads parameter
		if (StartsWithNoCase(LineData, "concurrentloads"))
		{
			if (equals == false)
				return ScriptError("Syntax error");

			engine->GetVM()->ConcurrentLoads = ToBool(value);
			return sNextLine;
		}
		//CutLandscape parameter
		if (StartsWithNoCase(LineData, "cutlandscape"))
		{
			if (equals == false)
				return ScriptError("Syntax error");

			engine->GetVM()->CutLandscape = ToBool(value);
			return sNextLine;
		}
		//CutBuildings parameter
		if (StartsWithNoCase(LineData, "cutbuildings"))
		{
			if (equals == false)
				return ScriptError("Syntax error");

			engine->GetVM()->CutBuildings = ToBool(value);
			return sNextLine;
		}
		//CutExternal parameter
		if (StartsWithNoCase(LineData, "cutexternal"))
		{
			if (equals == false)
				return ScriptError("Syntax error");

			engine->GetVM()->CutExternal = ToBool(value);
			return sNextLine;
		}
		//CutFloors parameter
		if (StartsWithNoCase(LineData, "cutfloors"))
		{
			if (equals == false)
				return ScriptError("Syntax error");

			engine->GetVM()->CutFloors = ToBool(value);
			return sNextLine;
		}
	}

	//Load command
	if (StartsWithNoCase(LineData, "load"))
	{
		//get data
		int params = SplitData(LineData, 5, false);

		if (params != 1 && params != 5 && params != 11)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		if (params > 1)
		{
			for (int i = 1; i <= params - 1; i++)
			{
				tempdata[i] = parent->Calc(tempdata[i]);
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		Vector3 position (Vector3::ZERO);
		Real rotation = 0.0;
		Vector3 min (Vector3::ZERO);
		Vector3 max (Vector3::ZERO);

		if (params > 1)
		{
			position.x = ToFloat(tempdata[1]);
			position.y = ToFloat(tempdata[2]);
			position.z = ToFloat(tempdata[3]);
			rotation = ToFloat(tempdata[4]);
		}
		if (params == 11)
		{
			min.x = ToFloat(tempdata[5]);
			min.y = ToFloat(tempdata[6]);
			min.z = ToFloat(tempdata[7]);
			max.x = ToFloat(tempdata[8]);
			max.y = ToFloat(tempdata[9]);
			max.z = ToFloat(tempdata[10]);
		}

		Vector3 rot (0, rotation, 0);
		bool result = engine->GetVM()->Load(engine->IsSystem, false, tempdata[0], engine, position, rot, min, max);

		return sNextLine;
	}

	//handle end of buildings section
	if (StartsWithNoCase(LineData, "<endbuildings>"))
	{
		config->SectionNum = SECTION_NONE;
		config->Context = "None";
		if (parent->InRunloop() == false)
			engine->Report("Finished loading other buildings");
		return sNextLine;
	}

	return sContinue;
}

}
