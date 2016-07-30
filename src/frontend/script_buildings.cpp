/* $Id$ */

/*
	Skyscraper 1.10 Alpha - Script Processor - Buildings Section
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
#include "scriptprocessor.h"
#include "script_section.h"

using namespace SBS;

namespace Skyscraper {

ScriptProcessor::BuildingsSection::BuildingsSection(ScriptProcessor *parent) : Section(parent)
{

}

int ScriptProcessor::BuildingsSection::Run(std::string &LineData)
{
	//Load additional buildings

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
	int temp2check = LineData.find("=", 0);
	std::string value = GetAfterEquals(LineData);

	//create a lowercase string of the line
	std::string linecheck = SetCaseCopy(LineData, false);

	//parameters
	if (linecheck.substr(0, 15) == "concurrentloads")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		engine->GetFrontend()->ConcurrentLoads = ToBool(value);
		return sNextLine;
	}
	if (linecheck.substr(0, 12) == "cutlandscape")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		engine->GetFrontend()->CutLandscape = ToBool(value);
		return sNextLine;
	}
	if (linecheck.substr(0, 12) == "cutbuildings")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		engine->GetFrontend()->CutBuildings = ToBool(value);
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "cutexternal")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		engine->GetFrontend()->CutExternal = ToBool(value);
		return sNextLine;
	}
	if (linecheck.substr(0, 9) == "cutfloors")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		engine->GetFrontend()->CutFloors = ToBool(value);
		return sNextLine;
	}

	//Load command
	if (linecheck.substr(0, 4) == "load")
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

		Ogre::Vector3 position (Ogre::Vector3::ZERO);
		float rotation = 0.0f;
		Ogre::Vector3 min (Ogre::Vector3::ZERO);
		Ogre::Vector3 max (Ogre::Vector3::ZERO);

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

		bool result = engine->GetFrontend()->Load(tempdata[0], engine, position, rotation, min, max);

		return sNextLine;
	}

	return sContinue;
}

}
