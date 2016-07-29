/* $Id$ */

/*
	Skyscraper 1.10 Alpha - Script Processor - Globals Section
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
#include "camera.h"
#include "scriptprocessor.h"

using namespace SBS;

namespace Skyscraper {

int ScriptProcessor::ProcGlobals()
{
	//process global parameters

	//get text after equal sign
	temp2 = GetAfterEquals(LineData);

	//create a lowercase string of the line
	std::string linecheck = SetCaseCopy(LineData, false);

	//store variable values
	if (linecheck.substr(0, 4) == "name")
	{
		Simcore->BuildingName = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 8) == "designer")
	{
		Simcore->BuildingDesigner = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 8) == "location")
	{
		Simcore->BuildingLocation = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "description")
	{
		Simcore->BuildingDescription = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 7) == "version")
	{
		Simcore->BuildingVersion = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 3) == "sky")
	{
		Simcore->SkyName = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 10) == "dynamicsky")
	{
		engine->GetFrontend()->SkyName = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 10) == "collisions")
	{
		Simcore->camera->EnableCollisions(ToBool(temp2));
		return sNextLine;
	}
	if (linecheck.substr(0, 7) == "gravity")
	{
		Simcore->camera->EnableGravity(ToBool(temp2));
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "camerafloor")
	{
		int data;
		if (!IsNumeric(temp2, data))
			return ScriptError("Invalid floor");

		Simcore->camera->StartFloor = data;
		return sNextLine;
	}
	if (linecheck.substr(0, 14) == "cameraposition")
	{
		float x, z;
		std::string str1 = temp2.substr(0, temp2.find(",", 0));
		std::string str2 = temp2.substr(temp2.find(",", 0) + 1);
		TrimString(str1);
		TrimString(str2);
		if (!IsNumeric(str1, x) || !IsNumeric(str2, z))
			return ScriptError("Invalid position");

		Simcore->camera->StartPositionX  = x;
		Simcore->camera->StartPositionZ  = z;
		return sNextLine;
	}
	if (linecheck.substr(0, 15) == "cameradirection")
	{
		temp3 = temp2.find(",", 0);
		temp4 = temp2.find(",", temp3 + 1);
		float x, y, z;
		std::string str1 = temp2.substr(0, temp3);
		std::string str2 = temp2.substr(temp3 + 1, temp4 - temp3 - 1);
		std::string str3 = temp2.substr(temp4 + 1);
		TrimString(str1);
		TrimString(str2);
		TrimString(str3);
		if (!IsNumeric(str1, x) || !IsNumeric(str2, y) || !IsNumeric(str3, z))
			return ScriptError("Invalid direction");

		Simcore->camera->SetStartDirection(Ogre::Vector3(x, y, z));
		return sNextLine;
	}
	if (linecheck.substr(0, 14) == "camerarotation")
	{
		temp3 = temp2.find(",", 0);
		temp4 = temp2.find(",", temp3 + 1);
		float x, y, z;
		std::string str1 = temp2.substr(0, temp3);
		std::string str2 = temp2.substr(temp3 + 1, temp4 - temp3 - 1);
		std::string str3 = temp2.substr(temp4 + 1);
		TrimString(str1);
		TrimString(str2);
		TrimString(str3);
		if (!IsNumeric(str1, x) || !IsNumeric(str2, y) || !IsNumeric(str3, z))
			return ScriptError("Invalid direction");

		Simcore->camera->SetStartRotation(Ogre::Vector3(x, y, z));
		return sNextLine;
	}
	if (linecheck.substr(0, 15) == "interfloorontop")
	{
		Simcore->InterfloorOnTop = ToBool(temp2);
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "coordinates")
	{
		temp3 = temp2.find(",", 0);
		float latitude, longitude;
		std::string str1 = temp2.substr(0, temp3);
		std::string str2 = temp2.substr(temp3 + 1);
		TrimString(str1);
		TrimString(str2);
		if (!IsNumeric(str1, latitude) || !IsNumeric(str2, longitude))
			return ScriptError("Invalid latitude");

		engine->GetFrontend()->SetLocation(latitude, longitude);
		return sNextLine;
	}
	if (linecheck.substr(0, 8) == "datetime")
	{
		double data;
		if (!IsNumeric(temp2, data))
			return ScriptError("Invalid Julian date/time");

		engine->GetFrontend()->SetDateTime(data);
		return sNextLine;
	}
	if (linecheck.substr(0, 8) == "position")
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

		Ogre::Vector3 position;
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
	if (linecheck.substr(0, 8) == "rotation")
	{
		float rotation;
		if (!IsNumeric(temp2, rotation))
			return ScriptError("Invalid rotation");

		Simcore->Rotate(0.0f, rotation, 0.0f);
		return sNextLine;
	}
	if (linecheck.substr(0, 6) == "bounds")
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

		Ogre::Vector3 min, max;
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
	return sContinue;
}

}
