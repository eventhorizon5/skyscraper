/*
	Skyscraper 1.12 Alpha - Script Processor - Globals Section
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
#include "camera.h"
#include "scriptproc.h"
#include "section.h"

using namespace SBS;

namespace Skyscraper {

ScriptProcessor::GlobalsSection::GlobalsSection(ScriptProcessor *parent) : Section(parent)
{

}

int ScriptProcessor::GlobalsSection::Run(std::string &LineData)
{
	//process global parameters

	//get text after equal sign
	bool equals;
	std::string value = GetAfterEquals(LineData, equals);

	//store variable values

	//Name parameter
	if (StartsWithNoCase(LineData, "name"))
	{
		Simcore->BuildingName = value;
		return sNextLine;
	}
	//Designer parameter
	if (StartsWithNoCase(LineData, "designer"))
	{
		Simcore->BuildingDesigner = value;
		return sNextLine;
	}
	//Location parameter
	if (StartsWithNoCase(LineData, "location"))
	{
		Simcore->BuildingLocation = value;
		return sNextLine;
	}
	//Description parameter
	if (StartsWithNoCase(LineData, "description"))
	{
		Simcore->BuildingDescription = value;
		return sNextLine;
	}
	//Version parameter
	if (StartsWithNoCase(LineData, "version"))
	{
		Simcore->BuildingVersion = value;
		return sNextLine;
	}
	//Sky parameter
	if (StartsWithNoCase(LineData, "sky"))
	{
		Simcore->SkyName = value;
		return sNextLine;
	}
	//DynamicSky parameter
	if (StartsWithNoCase(LineData, "dynamicsky"))
	{
		engine->GetFrontend()->SkyName = value;
		return sNextLine;
	}
	//Collisions parameter
	if (StartsWithNoCase(LineData, "collisions"))
	{
		Simcore->camera->EnableCollisions(ToBool(value));
		return sNextLine;
	}
	//Gravity parameter
	if (StartsWithNoCase(LineData, "gravity"))
	{
		Simcore->camera->EnableGravity(ToBool(value));
		return sNextLine;
	}
	//CameraFloor parameter
	if (StartsWithNoCase(LineData, "camerafloor"))
	{
		int data;
		std::string str = Calc(value);
		if (!IsNumeric(str, data))
			return ScriptError("Invalid floor");

		Simcore->camera->StartFloor = data;
		return sNextLine;
	}
	//CameraPosition parameter
	if (StartsWithNoCase(LineData, "cameraposition"))
	{
		Real x, z;
		std::string str1 = value.substr(0, value.find(",", 0));
		std::string str2 = value.substr(value.find(",", 0) + 1);
		str1 = Calc(str1);
		str2 = Calc(str2);
		if (!IsNumeric(str1, x) || !IsNumeric(str2, z))
			return ScriptError("Invalid position");

		Simcore->camera->StartPositionX  = x;
		Simcore->camera->StartPositionZ  = z;
		return sNextLine;
	}
	//CameraDirection parameter
	if (StartsWithNoCase(LineData, "cameradirection"))
	{
		int loc1 = value.find(",", 0);
		int loc2 = value.find(",", loc1 + 1);
		Real x, y, z;
		std::string str1 = value.substr(0, loc1);
		std::string str2 = value.substr(loc1 + 1, loc2 - loc1 - 1);
		std::string str3 = value.substr(loc2 + 1);
		str1 = Calc(str1);
		str2 = Calc(str2);
		str3 = Calc(str3);
		if (!IsNumeric(str1, x) || !IsNumeric(str2, y) || !IsNumeric(str3, z))
			return ScriptError("Invalid direction");

		Simcore->camera->SetStartDirection(Vector3(x, y, z));
		return sNextLine;
	}
	//CameraRotation parameter
	if (StartsWithNoCase(LineData, "camerarotation"))
	{
		int loc1 = value.find(",", 0);
		int loc2 = value.find(",", loc1 + 1);
		Real x, y, z;
		std::string str1 = value.substr(0, loc1);
		std::string str2 = value.substr(loc1 + 1, loc2 - loc1 - 1);
		std::string str3 = value.substr(loc2 + 1);
		str1 = Calc(str1);
		str2 = Calc(str2);
		str3 = Calc(str3);
		if (!IsNumeric(str1, x) || !IsNumeric(str2, y) || !IsNumeric(str3, z))
			return ScriptError("Invalid direction");

		Simcore->camera->SetStartRotation(Vector3(x, y, z));
		return sNextLine;
	}
	//InterfloorOnTop parameter
	if (StartsWithNoCase(LineData, "interfloorontop"))
	{
		Simcore->InterfloorOnTop = ToBool(value);
		return sNextLine;
	}
	//Coordinates parameter
	if (StartsWithNoCase(LineData, "coordinates"))
	{
		int loc = value.find(",", 0);
		Real latitude, longitude;
		std::string str1 = value.substr(0, loc);
		std::string str2 = value.substr(loc + 1);
		str1 = Calc(str1);
		str2 = Calc(str2);
		if (!IsNumeric(str1, latitude) || !IsNumeric(str2, longitude))
			return ScriptError("Invalid latitude");

		engine->GetFrontend()->SetLocation(latitude, longitude);
		return sNextLine;
	}
	//DateTime parameter
	if (StartsWithNoCase(LineData, "datetime"))
	{
		if (value == "now")
			engine->GetFrontend()->SetDateTimeNow();
		else
		{
			double data;
			if (!IsNumeric(value, data))
				return ScriptError("Invalid Julian date/time");

			engine->GetFrontend()->SetDateTime(data);
		}
		return sNextLine;
	}
	//TimeScale parameter
	if (StartsWithNoCase(LineData, "timescale"))
	{
		int data;
		std::string str = Calc(value);
		if (!IsNumeric(str, data))
			return ScriptError("Invalid time scale value");

		engine->GetFrontend()->SkyMult = data;
	}
	//Position parameter
	if (StartsWithNoCase(LineData, "position"))
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

		Vector3 position;
		position.x = ToFloat(tempdata[0]);
		position.y = ToFloat(tempdata[1]);
		position.z = ToFloat(tempdata[2]);

		if (engine->Moved == false)
		{
			engine->Move(position, true);
			engine->Moved = true;
		}
		return sNextLine;
	}
	//Rotation parameter
	if (StartsWithNoCase(LineData, "rotation"))
	{
		int rotation;
		std::string str = Calc(value);
		if (!IsNumeric(str, rotation))
			return ScriptError("Invalid rotation");

		Simcore->Rotate(0.0, rotation, 0.0);
		return sNextLine;
	}
	//Bounds parameter
	if (StartsWithNoCase(LineData, "bounds"))
	{
		int params = SplitAfterEquals(LineData);
		if (params != 6)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 5; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		Vector3 min, max;
		min.x = ToFloat(tempdata[0]);
		min.y = ToFloat(tempdata[1]);
		min.z = ToFloat(tempdata[2]);
		max.x = ToFloat(tempdata[3]);
		max.y = ToFloat(tempdata[4]);
		max.z = ToFloat(tempdata[5]);

		if (Simcore->HasBounds() == false)
			Simcore->SetBounds(min, max);
		return sNextLine;
	}

	//handle end of globals section
	if (StartsWithNoCase(LineData, "<endglobals>"))
	{
		config->SectionNum = 0;
		config->Context = "None";
		engine->Report("Finished globals");
		return sNextLine;
	}

	return sContinue;
}

}
