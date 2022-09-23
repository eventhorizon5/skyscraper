/* $Id$ */

/*
	Skyscraper 1.11 Alpha - Script Processor - Global Commands
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
#include "floor.h"
#include "camera.h"
#include "texture.h"
#include "mesh.h"
#include "soundsystem.h"
#include "sound.h"
#include "wall.h"
#include "trigger.h"
#include "control.h"
#include "model.h"
#include "stairs.h"
#include "shaft.h"
#include "scriptprocessor.h"
#include "script_section.h"

using namespace SBS;

namespace Skyscraper {

ScriptProcessor::CommandsSection::CommandsSection(ScriptProcessor *parent) : Section(parent)
{

}

int ScriptProcessor::CommandsSection::Run(std::string &LineData)
{
	//process global commands

	//create a lowercase string of the line
	std::string linecheck = SetCaseCopy(LineData, false);

	//IF/While statement
	int IsIf = 0;
	if (SetCaseCopy(LineData.substr(0, 2), false) == "if")
		IsIf = 1;
	if (SetCaseCopy(LineData.substr(0, 5), false) == "while")
		IsIf = 2;
	if (IsIf > 0)
	{
		int loc1 = LineData.find("[", 0);
		int loc2 = LineData.find("]", 0);
		std::string str;
		if (loc1 + loc2 > 0)
			str = LineData.substr(loc1 + 1, loc2 - loc1 - 1);
		else
			str = "";
		TrimString(str);
		if (IfProc(str) == true)
		{
			//trim off IF/While statement
			LineData = LineData.substr(loc2 + 1);
			TrimString(LineData);

			if (IsIf == 2)
				config->InWhile = true;

			return sCheckFloors;
		}
		else
			return sNextLine; //skip line
	}

	if (config->SectionNum != 2 && config->SectionNum != 4)
	{
		//process math functions
		if (MathFunctions(LineData) == sError)
			return sError;

		//process any functions
		if (parent->FunctionProc() == true)
			return sNextLine;
	}

	TextureManager *texturemanager = Simcore->GetTextureManager();

	//AddTriangleWall command
	if (linecheck.substr(0, 15) == "addtrianglewall")
	{
		//get data
		int params = SplitData(LineData, 16);

		if (params != 14)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 13; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::string meshname = SetCaseCopy(tempdata[0], false);

		MeshObject *mesh = GetMeshObject(meshname);

		if (!mesh)
			return ScriptError("Invalid object");

		Real voffset1 = ToFloat(tempdata[4]);
		Real voffset2 = ToFloat(tempdata[7]);
		Real voffset3 = ToFloat(tempdata[10]);

		if (config->SectionNum == 2)
		{
			if (meshname == "floor")
			{
				Real base = Simcore->GetFloor(config->Current)->GetBase(true);
				voffset1 += base;
				voffset2 += base;
				voffset3 += base;
			}
			else if (meshname == "external" || meshname == "landscape" || meshname == "buildings")
			{
				Real base = Simcore->GetFloor(config->Current)->GetBase();
				voffset1 += base;
				voffset2 += base;
				voffset3 += base;
			}
		}

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//create triangle wall
		StoreCommand(Simcore->AddTriangleWall(mesh, tempdata[1], tempdata[2], ToFloat(tempdata[3]), voffset1, ToFloat(tempdata[5]), ToFloat(tempdata[6]), voffset2, ToFloat(tempdata[8]), ToFloat(tempdata[9]), voffset3, ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13])));

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
		for (int i = 3; i <= 13; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::string meshname = SetCaseCopy(tempdata[0], false);

		MeshObject *mesh = GetMeshObject(meshname);

		if (!mesh)
			return ScriptError("Invalid object");

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//create wall
		StoreCommand(Simcore->AddWall(mesh, tempdata[1], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13])));
		return sNextLine;
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
			for (int i = 3; i <= 11; i++)
			{
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
				if (i == 10)
					i = 12;

				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		std::string meshname = SetCaseCopy(tempdata[0], false);

		MeshObject *mesh = GetMeshObject(meshname);

		if (!mesh)
			return ScriptError("Invalid object");

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//create floor
		if (compat == true)
			StoreCommand(Simcore->AddFloor(mesh, tempdata[1], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), config->ReverseAxis, false, ToFloat(tempdata[10]), ToFloat(tempdata[11]), true));
		else
			StoreCommand(Simcore->AddFloor(mesh, tempdata[1], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToBool(tempdata[10]), ToBool(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13])));
		return sNextLine;
	}

	//AddGround command
	if (linecheck.substr(0, 9) == "addground")
	{
		//get data
		int params = SplitData(LineData, 10);

		if (params != 9)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 8; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//create tiled ground
		StoreCommand(Simcore->AddGround(tempdata[0], tempdata[1], ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToInt(tempdata[7]), ToInt(tempdata[8])));
		return sNextLine;
	}

	//Cut command
	if (linecheck.substr(0, 4) == "cut ")
	{
		//get data
		int params = SplitData(LineData, 4);

		if (params != 9)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 6; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		MeshObject *mesh = GetMeshObject(tempdata[0]);

		if (!mesh)
			return ScriptError("Invalid object");

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//perform cut
		mesh->Cut(Ogre::Vector3(ToFloat(tempdata[1]), ToFloat(tempdata[2]), ToFloat(tempdata[3])), Ogre::Vector3(ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6])), ToBool(tempdata[7]), ToBool(tempdata[8]));
		return sNextLine;
	}

	//Set command
	if (linecheck.substr(0, 4) == "set ")
	{
		int loc = LineData.find("=", 0);
		if (loc < 0)
			return ScriptError("Syntax Error");

		std::string str = LineData.substr(4, loc - 5);
		TrimString(str);

		//reserved keywords
		if (str == "base" || str == "floor" || str == "height" || str == "interfloorheight" || str == "fullheight" || str == "elevator" || str == "minx" || str == "maxx" || str == "minz" || str == "maxz" || str == "number" || str.substr(0, 4) == "param" || str == "floorname" || str == "floortype" || str == "floorid" || str == "description")
			return ScriptError("Cannot use system variable name");

		//get text after equal sign
		std::string value = Calc(GetAfterEquals(LineData));

		//find existing variable by name
		int index = -1;
		for (int i = 0; i < (int)parent->variables.size(); i++)
		{
			if (parent->variables[i].name == str)
			{
				index = i;
				break;
			}
		}

		if (index == -1)
		{
			//create new variable
			VariableMap variable;
			variable.name = str;
			variable.value = value;
			parent->variables.push_back(variable);
			value = variable.value;
		}
		else
		{
			//set existing variable
			parent->variables[index].name = str;
			parent->variables[index].value = value;
			value = parent->variables[index].value;
		}

		if (Simcore->Verbose == true)
			engine->Report("Variable '" + str + "' set to " + value);
		return sNextLine;
	}

	//CreateWallBox2 command
	if (linecheck.substr(0, 14) == "createwallbox2")
	{
		//get data
		int params = SplitData(LineData, 15);

		if (params != 15)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 10; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::string meshname = SetCaseCopy(tempdata[0], false);

		MeshObject *mesh = GetMeshObject(meshname);

		if (!mesh)
			return ScriptError("Invalid object");

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		Real voffset = ToFloat(tempdata[8]);

		if (config->SectionNum == 2)
		{
			if (meshname == "floor")
				voffset += Ogre::Real(Simcore->GetFloor(config->Current)->GetBase(true));
			else if (meshname == "external" || meshname == "landscape" || meshname == "buildings")
				voffset += Ogre::Real(Simcore->GetFloor(config->Current)->GetBase());
		}

		StoreCommand(Simcore->CreateWallBox2(mesh, tempdata[1], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), voffset, ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToBool(tempdata[11]), ToBool(tempdata[12]), ToBool(tempdata[13]), ToBool(tempdata[14])));

		return sNextLine;
	}

	//CreateWallBox command
	if (linecheck.substr(0, 14) == "createwallbox ")
	{
		//get data
		int params = SplitData(LineData, 14);

		if (params != 15)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 10; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::string meshname = SetCaseCopy(tempdata[0], false);

		MeshObject *mesh = GetMeshObject(meshname);

		if (!mesh)
			return ScriptError("Invalid object");

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		Real voffset = ToFloat(tempdata[8]);

		if (config->SectionNum == 2)
		{
			if (meshname == "floor")
				voffset += Ogre::Real(Simcore->GetFloor(config->Current)->GetBase(true));
			else if (meshname == "external" || meshname == "landscape" || meshname == "buildings")
				voffset += Ogre::Real(Simcore->GetFloor(config->Current)->GetBase());
		}

		StoreCommand(Simcore->CreateWallBox(mesh, tempdata[1], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), voffset, ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToBool(tempdata[11]), ToBool(tempdata[12]), ToBool(tempdata[13]), ToBool(tempdata[14])));

		return sNextLine;
	}

	//AddCustomWall command
	if (linecheck.substr(0, 14) == "addcustomwall ")
	{
		//get data
		int params = SplitData(LineData, 14);

		if (params < 14)
			return ScriptError("Incorrect number of parameters");

		bool relative_option = false, relative = false;
		if (IsNumeric(tempdata[3]) == false)
		{
			relative_option = true;
			relative = ToBool(tempdata[3]);
		}

		//check numeric values
		int start = 3;
		if (relative_option == true)
			start = 4;

		for (int i = start; i < params; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::string meshname = SetCaseCopy(tempdata[0], false);

		MeshObject *mesh = GetMeshObject(meshname);

		if (!mesh)
			return ScriptError("Invalid object");

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		Real voffset = 0;

		if (config->SectionNum == 2)
		{
			if (relative == true)
			{
				if (meshname == "floor")
					voffset += Ogre::Real(Simcore->GetFloor(config->Current)->GetBase(true));
				else if (meshname == "external" || meshname == "landscape" || meshname == "buildings")
					voffset += Ogre::Real(Simcore->GetFloor(config->Current)->GetBase());
			}
			else if (relative_option == false)
			{
				if (meshname == "floor" && config->SectionNum == 2)
					voffset -= mesh->GetPosition().y; //subtract altitude for new positioning model
			}
		}

		std::vector<Ogre::Vector3> varray;
		for (int i = start; i < params - 2; i += 3)
			varray.push_back(Ogre::Vector3(ToFloat(tempdata[i]), ToFloat(tempdata[i + 1]) + voffset, ToFloat(tempdata[i + 2])));

		StoreCommand(Simcore->AddCustomWall(mesh, tempdata[1], tempdata[2], varray, ToFloat(tempdata[params - 2]), ToFloat(tempdata[params - 1])));

		return sNextLine;
	}

	//AddCustomFloor command
	if (linecheck.substr(0, 15) == "addcustomfloor ")
	{
		//get data
		int params = SplitData(LineData, 15);

		//check numeric values
		for (int i = 3; i < params; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::string meshname = SetCaseCopy(tempdata[0], false);

		MeshObject *mesh = GetMeshObject(meshname);

		if (!mesh)
			return ScriptError("Invalid object");

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		Real altitude = ToFloat(tempdata[params - 3]);

		if (config->SectionNum == 2)
		{
			if (meshname == "floor")
				altitude += Simcore->GetFloor(config->Current)->GetBase(true);
			else if (meshname == "external" || meshname == "landscape" || meshname == "buildings")
				altitude += Simcore->GetFloor(config->Current)->GetBase();
		}

		std::vector<Ogre::Vector2> varray;
		for (int i = 3; i < params - 3; i += 2)
			varray.push_back(Ogre::Vector2(ToFloat(tempdata[i]), ToFloat(tempdata[i + 1])));

		StoreCommand(Simcore->AddCustomFloor(mesh, tempdata[1], tempdata[2], varray, altitude, ToFloat(tempdata[params - 2]), ToFloat(tempdata[params - 1])));

		return sNextLine;
	}

	//AddPolygon command
	if (linecheck.substr(0, 10) == "addpolygon")
	{
		//get data
		int params = SplitData(LineData, 11);

		//check numeric values
		for (int i = 3; i < params; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::string meshname = SetCaseCopy(tempdata[0], false);

		MeshObject *mesh = GetMeshObject(meshname);

		if (!mesh)
			return ScriptError("Invalid mesh object");

		Wall *wall = mesh->GetWallByName(tempdata[1]);

		if (!wall)
			return ScriptError("Invalid wall object");

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		Real voffset = 0.0;

		if (config->SectionNum == 2)
		{
			if (meshname == "floor")
				voffset += Simcore->GetFloor(config->Current)->GetBase(true);
			else if (meshname == "external" || meshname == "landscape" || meshname == "buildings")
				voffset += Simcore->GetFloor(config->Current)->GetBase();
		}

		std::vector<Ogre::Vector3> varray;
		for (int i = 3; i < params - 2; i += 3)
			varray.push_back(Ogre::Vector3(ToFloat(tempdata[i]), ToFloat(tempdata[i + 1]) + voffset, ToFloat(tempdata[i + 2])));

		Simcore->AddPolygon(wall, tempdata[2], varray, ToFloat(tempdata[params - 2]), ToFloat(tempdata[params - 1]));

		return sNextLine;
	}

	//AddShaft command
	if (linecheck.substr(0, 9) == "addshaft ")
	{
		//get data
		int params = SplitData(LineData, 9);

		if (params != 5 && params != 6)
			return ScriptError("Incorrect number of parameters");

		bool compat = false;
		if (params == 6)
			compat = true;

		//check numeric values
		if (compat == true)
		{
			for (int i = 0; i <= 5; i++)
			{
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
		}
		else
		{
			for (int i = 0; i <= 4; i++)
			{
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		int startfloor, endfloor;
		if (compat == true)
		{
			startfloor = ToInt(tempdata[4]);
			endfloor = ToInt(tempdata[5]);
		}
		else
		{
			startfloor = ToInt(tempdata[3]);
			endfloor = ToInt(tempdata[4]);
		}

		if (startfloor < -Simcore->Basements)
			return ScriptError("Invalid starting floor");
		if (endfloor > Simcore->Floors - 1)
			return ScriptError("Invalid ending floor");

		Shaft *shaft;
		if (compat == true)
			shaft = Simcore->CreateShaft(ToInt(tempdata[0]), ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToInt(tempdata[4]), ToInt(tempdata[5]));
		else
			shaft = Simcore->CreateShaft(ToInt(tempdata[0]), ToFloat(tempdata[1]), ToFloat(tempdata[2]), ToInt(tempdata[3]), ToInt(tempdata[4]));

		if (!shaft)
			return ScriptError();

		StoreCommand(shaft);
		return sNextLine;
	}

	//ShaftCut command
	if (linecheck.substr(0, 9) == "shaftcut ")
	{
		//get data
		int params = SplitData(LineData, 9);

		if (params != 7)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 6; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//check for existence of shaft
		int shaftnum = ToInt(tempdata[0]);
		if (shaftnum < 1 || shaftnum > Simcore->GetShaftCount())
			return ScriptError("Invalid shaft " + tempdata[0]);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		Simcore->GetShaft(shaftnum)->CutFloors(true, Ogre::Vector2(ToFloat(tempdata[1]), ToFloat(tempdata[2])), Ogre::Vector2(ToFloat(tempdata[3]), ToFloat(tempdata[4])), ToFloat(tempdata[5]), ToFloat(tempdata[6]));
		return sNextLine;
	}

	//ShaftShowFloors command
	if (linecheck.substr(0, 15) == "shaftshowfloors")
	{
		//get shaft number
		int loc = LineData.find("=");
		if (loc < 0)
			return ScriptError("Syntax error");
		int shaftnum;
		std::string str = LineData.substr(15, loc - 16);
		TrimString(str);
		if (!IsNumeric(str, shaftnum))
			return ScriptError("Invalid shaft number");

		if (shaftnum < 1 || shaftnum > Simcore->GetShaftCount())
			return ScriptError("Invalid shaft number");

		int params = SplitAfterEquals(LineData, false);
		if (params < 1)
			return ScriptError("Syntax Error");

		Simcore->GetShaft(shaftnum)->ShowFloors = 1;

		//determine if last parameter is a boolean, used for full floor selection on/off
		if (IsBoolean(tempdata[params - 1]) == true)
		{
			if (ToBool(tempdata[params - 1]) == true)
				Simcore->GetShaft(shaftnum)->ShowFloors = 2;
			params--;
		}

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
					Simcore->GetShaft(shaftnum)->AddShowFloor(k);
			}
			else
			{
				int showfloor;
				if (!IsNumeric(tempdata[line], showfloor))
					return ScriptError("Invalid value");
				Simcore->GetShaft(shaftnum)->AddShowFloor(showfloor);
			}
		}
		return sNextLine;
	}

	//ShaftShowInterfloors command
	if (linecheck.substr(0, 20) == "shaftshowinterfloors")
	{
		//get shaft number
		int loc = LineData.find("=");
		if (loc < 0)
			return ScriptError("Syntax error");
		int shaftnum;
		std::string str = LineData.substr(20, loc - 21);
		TrimString(str);
		if (!IsNumeric(str, shaftnum))
			return ScriptError("Invalid shaft number");
		if (shaftnum < 1 || shaftnum > Simcore->GetShaftCount())
			return ScriptError("Invalid shaft number");
		Simcore->GetShaft(shaftnum)->ShowInterfloors = true;

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
					Simcore->GetShaft(shaftnum)->AddShowInterfloor(k);
			}
			else
			{
				int showfloor;
				if (!IsNumeric(tempdata[line], showfloor))
					return ScriptError("Invalid value");
				Simcore->GetShaft(shaftnum)->AddShowInterfloor(showfloor);
			}
		}
		return sNextLine;
	}

	//ShaftShowOutside command
	if (linecheck.substr(0, 16) == "shaftshowoutside")
	{
		//get shaft number
		int loc = LineData.find("=");
		if (loc < 0)
			return ScriptError("Syntax error");
		int shaftnum;
		std::string str = LineData.substr(16, loc - 17);
		TrimString(str);
		if (!IsNumeric(str, shaftnum))
			return ScriptError("Invalid shaft number");
		if (shaftnum < 1 || shaftnum > Simcore->GetShaftCount())
			return ScriptError("Invalid shaft number");
		Simcore->GetShaft(shaftnum)->ShowOutside = true;

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
					Simcore->GetShaft(shaftnum)->AddShowOutside(k);
			}
			else
			{
				int showfloor;
				if (!IsNumeric(tempdata[line], showfloor))
					return ScriptError("Invalid value");
				Simcore->GetShaft(shaftnum)->AddShowOutside(showfloor);
			}
		}
		return sNextLine;
	}

	//ShowFullShaft command
	if (linecheck.substr(0, 13) == "showfullshaft")
	{
		//get shaft number
		int loc = LineData.find("=");
		if (loc < 0)
			return ScriptError("Syntax error");
		int shaftnum;
		std::string str = LineData.substr(13, loc - 14);
		TrimString(str);
		if (!IsNumeric(str, shaftnum))
			return ScriptError("Invalid shaft number");
		if (shaftnum < 1 || shaftnum > Simcore->GetShaftCount())
			return ScriptError("Invalid shaft number");

		//get text after equal sign
		std::string value = GetAfterEquals(LineData);

		Simcore->GetShaft(shaftnum)->SetShowFull(ToBool(value));
		return sNextLine;
	}

	//CreateStairwell command
	if (linecheck.substr(0, 15) == "createstairwell")
	{
		//get data
		int params = SplitData(LineData, 16);

		if (params != 5)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 4; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		Stairs *stairs = Simcore->CreateStairwell(ToInt(tempdata[0]), ToFloat(tempdata[1]), ToFloat(tempdata[2]), ToInt(tempdata[3]), ToInt(tempdata[4]));
		if (!stairs)
			return ScriptError();

		StoreCommand(stairs);
		return sNextLine;
	}

	//CutStairwell command
	if (linecheck.substr(0, 13) == "cutstairwell ")
	{
		//get data
		int params = SplitData(LineData, 13);

		if (params != 7)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 6; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		int stairwell = ToInt(tempdata[0]);
		if (!Simcore->GetStairs(stairwell))
			return ScriptError("Invalid stairwell");

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		Simcore->GetStairs(stairwell)->CutFloors(true, Ogre::Vector2(ToFloat(tempdata[1]), ToFloat(tempdata[2])), Ogre::Vector2(ToFloat(tempdata[3]), ToFloat(tempdata[4])), ToFloat(tempdata[5]), ToFloat(tempdata[6]));
		return sNextLine;
	}

	//StairsShowFloors command
	if (linecheck.substr(0, 16) == "stairsshowfloors")
	{
		//get stairwell number
		int loc = LineData.find("=");
		if (loc < 0)
			return ScriptError("Syntax error");
		int stairnum;
		std::string str = LineData.substr(16, loc - 17);
		TrimString(str);
		if (!IsNumeric(str, stairnum))
			return ScriptError("Invalid stairwell number");

		if (stairnum < 1 || stairnum > Simcore->GetStairsCount())
			return ScriptError("Invalid stairwell number");

		Simcore->GetStairs(stairnum)->ShowFloors = true;

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
					Simcore->GetStairs(stairnum)->AddShowFloor(k);
			}
			else
			{
				int showfloor;
				if (!IsNumeric(tempdata[line], showfloor))
					return ScriptError("Invalid value");
				Simcore->GetStairs(stairnum)->AddShowFloor(showfloor);
			}
		}
		return sNextLine;
	}

	//ShowFullStairs command
	if (linecheck.substr(0, 14) == "showfullstairs")
	{
		//get shaft number
		int loc = LineData.find("=");
		if (loc < 0)
			return ScriptError("Syntax error");
		int stairnum;
		std::string str = LineData.substr(14, loc - 15);
		TrimString(str);
		if (!IsNumeric(str, stairnum))
			return ScriptError("Invalid stairwell number");
		if (stairnum < 1 || stairnum > Simcore->GetStairsCount())
			return ScriptError("Invalid stairwell number");

		//get text after equal sign
		std::string strvalue = GetAfterEquals(LineData);
		SetCase(strvalue, false);

		int value = 0;

		if (IsBoolean(strvalue) == true)
		{
			if (ToBool(strvalue) == true)
				value = 1;
		}
		else
		{
			if (strvalue == "inside")
				value = 1;
			else if (strvalue == "always")
				value = 2;
			else
				return ScriptError("Invalid value: " + strvalue);
		}

		Simcore->GetStairs(stairnum)->SetShowFull(value);
		return sNextLine;
	}

	//WallOrientation command
	if (linecheck.substr(0, 15) == "wallorientation")
	{
		//get text after equal sign
		std::string value = GetAfterEquals(LineData);

		if (!Simcore->SetWallOrientation(value))
			return ScriptError();
		return sNextLine;
	}

	//FloorOrientation command
	if (linecheck.substr(0, 16) == "floororientation")
	{
		//get text after equal sign
		std::string value = GetAfterEquals(LineData);

		if (!Simcore->SetFloorOrientation(value))
			return ScriptError();
		return sNextLine;
	}

	//DrawWalls command
	if (linecheck.substr(0, 9) == "drawwalls")
	{
		int params = SplitAfterEquals(LineData);

		if (params != 6)
			return ScriptError("Incorrect number of parameters");

		Simcore->DrawWalls(ToBool(tempdata[0]),
					ToBool(tempdata[1]),
					ToBool(tempdata[2]),
					ToBool(tempdata[3]),
					ToBool(tempdata[4]),
					ToBool(tempdata[5]));
		return sNextLine;
	}

	//SetTextureMapping command
	if (linecheck.substr(0, 18) == "settexturemapping ")
	{
		//get data
		int params = SplitData(LineData, 18);

		if (params != 9)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 8; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		texturemanager->SetTextureMapping(ToInt(tempdata[0]), Ogre::Vector2(ToFloat(tempdata[1]), ToFloat(tempdata[2])),
									ToInt(tempdata[3]), Ogre::Vector2(ToFloat(tempdata[4]), ToFloat(tempdata[5])),
									ToInt(tempdata[6]), Ogre::Vector2(ToFloat(tempdata[7]), ToFloat(tempdata[8])));
		return sNextLine;
	}

	//SetTextureMapping2 command
	if (linecheck.substr(0, 18) == "settexturemapping2")
	{
		//get data
		int params = SplitData(LineData, 19);

		if (params != 15)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 14; i++)
		{
			if (i == 5)
				i = 8;
			if (i == 10)
				i = 13;
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		texturemanager->SetTextureMapping2(tempdata[0], tempdata[1], tempdata[2], Ogre::Vector2(ToFloat(tempdata[3]), ToFloat(tempdata[4])),
									tempdata[5], tempdata[6], tempdata[7], Ogre::Vector2(ToFloat(tempdata[8]), ToFloat(tempdata[9])),
									tempdata[10], tempdata[11], tempdata[12], Ogre::Vector2(ToFloat(tempdata[13]), ToFloat(tempdata[14])));
		return sNextLine;
	}

	//ResetTextureMapping command
	if (linecheck.substr(0, 19) == "resettexturemapping")
	{
		int check = (int)LineData.find("=", 0);
		if (check < 0)
			return ScriptError("Syntax Error");

		//get text after equal sign
		std::string value = GetAfterEquals(LineData);

		texturemanager->ResetTextureMapping(ToBool(value));
		return sNextLine;
	}

	//SetPlanarMapping command
	if (linecheck.substr(0, 16) == "setplanarmapping")
	{
		//get data
		int params = SplitData(LineData, 17);

		if (params != 4 && params != 5)
			return ScriptError("Incorrect number of parameters");

		if (params == 4)
		{
			texturemanager->SetPlanarMapping(ToBool(tempdata[0]),
					ToBool(tempdata[1]),
					ToBool(tempdata[2]),
					ToBool(tempdata[3]), false);
			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
		}
		else
		{
			texturemanager->SetPlanarMapping(ToBool(tempdata[0]),
					ToBool(tempdata[1]),
					ToBool(tempdata[2]),
					ToBool(tempdata[3]),
					ToBool(tempdata[4]));
		}
		return sNextLine;
	}

	//ReverseAxis command
	if (linecheck.substr(0, 11) == "reverseaxis")
	{
		//backwards compatibility
		if (warn_deprecated == true)
			ScriptWarning("Command deprecated");

		int check = (int)LineData.find("=", 0);
		if (check < 0)
			return ScriptError("Syntax Error");
		std::string value = GetAfterEquals(LineData);

		config->ReverseAxis = ToBool(value);
		return sNextLine;
	}

	//Intersection points
	int found = (int)linecheck.find("isect(", 0);
	while (found > -1)
	{
		int loc1 = LineData.find("(", 0);
		int loc2 = LineData.find(")", 0);
		if (loc1 < 0 || loc2 < 0)
			return ScriptError("Syntax error");

		std::string buffer;
		SplitString(tempdata, LineData.substr(loc1 + 1, loc2 - loc1 - 1), ',');
		for (int i = 0; i < (int)tempdata.size(); i++)
		{
			buffer = Calc(tempdata[i]);
			tempdata[i] = buffer;
		}
		if (tempdata.size() < 8 || tempdata.size() > 8)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 7; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		MeshObject *mesh = GetMeshObject(tempdata[0]);

		if (!mesh)
			return ScriptError("Invalid object");

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		Ogre::Vector3 isect = mesh->GetPoint(tempdata[1], Ogre::Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4])), Ogre::Vector3(ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7])));

		buffer = LineData.substr(0, found) + ToString(isect.x) + ", " + ToString(isect.y) + ", " + ToString(isect.z) + LineData.substr(loc2 + 1);
		LineData = buffer;
		linecheck = SetCaseCopy(LineData, false);
		found = linecheck.find("isect(", 0);
	}

	//Endpoint function
	found = linecheck.find("endpoint(", 0);
	while (found > -1)
	{
		int loc1 = LineData.find("(", 0);
		int loc2 = LineData.find(")", 0);
		if (loc1 < 0 || loc2 < 0)
			return ScriptError("Syntax error");

		std::string buffer;
		SplitString(tempdata, LineData.substr(loc1 + 1, loc2 - loc1 - 1), ',');
		for (int i = 0; i < (int)tempdata.size(); i++)
		{
			buffer = Calc(tempdata[i]);
			tempdata[i] = buffer;
		}
		if (tempdata.size() != 4)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 3; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		Ogre::Vector2 startpoint (ToFloat(tempdata[0]), ToFloat(tempdata[1]));
		Ogre::Vector2 endpoint = Simcore->GetEndPoint(startpoint, ToFloat(tempdata[2]), ToFloat(tempdata[3]));

		buffer = LineData.substr(0, found) + ToString(endpoint.x) + ", " + ToString(endpoint.y) + LineData.substr(loc2 + 1);
		LineData = buffer;
		linecheck = SetCaseCopy(LineData, false);
		found = linecheck.find("endpoint(", 0);
	}

	//GetWallExtents command
	if (linecheck.substr(0, 14) == "getwallextents")
	{
		//get data
		int params = SplitData(LineData, 15);

		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		if (!IsNumeric(tempdata[2]))
			return ScriptError("Invalid value: " + tempdata[2]);

		Real offset = 0;

		std::string meshname = SetCaseCopy(tempdata[0], false);

		MeshObject *mesh = GetMeshObject(meshname);

		if (!mesh)
			return ScriptError("Invalid object");

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		if (meshname == "floor" && config->SectionNum == 2)
			offset = mesh->GetPosition().y;

		Real alt = ToFloat(tempdata[2]);

		//GetWallExtents command requires relative position, so subtract altitude from value
		alt -= offset;

		config->MinExtent = mesh->GetWallExtents(tempdata[1], alt, false);
		config->MaxExtent = mesh->GetWallExtents(tempdata[1], alt, true);
		return sNextLine;
	}

	//SetAutoSize command
	if (linecheck.substr(0, 11) == "setautosize")
	{
		int params = SplitAfterEquals(LineData);

		if (params != 2)
			return ScriptError("Incorrect number of parameters");

		texturemanager->SetAutoSize(ToBool(tempdata[0]),
					ToBool(tempdata[1]));
		return sNextLine;
	}

	//TextureOverride command
	if (linecheck.substr(0, 15) == "textureoverride")
	{
		int params = SplitData(LineData, 16, false);

		if (params != 6)
			return ScriptError("Incorrect number of parameters");

		texturemanager->SetTextureOverride(tempdata[0], tempdata[1], tempdata[2], tempdata[3], tempdata[4], tempdata[5]);
		return sSkipReset;
	}

	//TextureFlip command
	if (linecheck.substr(0, 11) == "textureflip")
	{
		int params = SplitData(LineData, 12, false);

		if (params != 6)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 5; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		texturemanager->SetTextureFlip(ToInt(tempdata[0]), ToInt(tempdata[1]), ToInt(tempdata[2]), ToInt(tempdata[3]), ToInt(tempdata[4]), ToInt(tempdata[5]));
		return sSkipReset;
	}

	//Mount command
	if (linecheck.substr(0, 5) == "mount")
	{
		//get data
		int params = SplitData(LineData, 6, false);

		if (params != 2)
			return ScriptError("Incorrect number of parameters");

		if (!Simcore->Mount(tempdata[0], tempdata[1]))
			return ScriptError();

		return sNextLine;
	}

	//AddFloorAutoArea command
	if (linecheck.substr(0, 16) == "addfloorautoarea")
	{
		//get data
		int params = SplitData(LineData, 17);

		if (params != 6)
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

		//create floor auto area
		Simcore->AddFloorAutoArea(Ogre::Vector3(ToFloat(tempdata[0]), ToFloat(tempdata[1]), ToFloat(tempdata[2])), Ogre::Vector3(ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5])));
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
				StoreCommand(Simcore->AddSound(tempdata[0], tempdata[1], Ogre::Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]))));
			else
				StoreCommand(Simcore->AddSound(tempdata[0], tempdata[1], Ogre::Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4])), true, ToFloat(tempdata[5]), ToInt(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), 0.0, 360, 360, 1.0, Ogre::Vector3(ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]))));
		}
		else
		{
			if (partial == true)
				StoreCommand(Simcore->AddSound(tempdata[0], tempdata[1], Ogre::Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4])), ToBool(tempdata[5])));
			else
				StoreCommand(Simcore->AddSound(tempdata[0], tempdata[1], Ogre::Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4])), ToBool(tempdata[5]), ToFloat(tempdata[6]), ToInt(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), Ogre::Vector3(ToFloat(tempdata[14]), ToFloat(tempdata[15]), ToFloat(tempdata[16]))));
		}
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
		Model* model;
		if (compat == true)
			model = Simcore->AddModel(tempdata[0], tempdata[1], false, Ogre::Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4])), Ogre::Vector3(ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7])), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToBool(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]));
		else
			model = Simcore->AddModel(tempdata[0], tempdata[1], ToBool(tempdata[2]), Ogre::Vector3(ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5])), Ogre::Vector3(ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8])), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToBool(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]));

		if (config->setkey == true && model)
			model->SetKey(config->keyvalue);
		config->setkey = false;

		StoreCommand(model);
		return sNextLine;
	}

	//AddAction command
	if (linecheck.substr(0, 10) == "addaction ")
	{
		//get data
		int params = SplitData(LineData, 10);

		if (params < 3)
			return ScriptError("Incorrect number of parameters");

		std::vector<Object*> objects;
		std::string tmpname = tempdata[1];
		SetCase(tmpname, false);
		if (tmpname == "global")
			objects.push_back(Simcore);
		else
			objects = Simcore->GetObjectRange(tempdata[1]);

		std::vector<std::string> actparams;
		if (params > 3)
		{
			for (int i = 3; i < params; i++)
			{
				actparams.push_back(tempdata[i]);
			}
		}

		if (objects.size() > 0)
		{
			if (params > 3)
				Simcore->AddAction(tempdata[0], objects, tempdata[2], actparams);
			else
				Simcore->AddAction(tempdata[0], objects, tempdata[2]);
		}
		else
			return ScriptError("Invalid parent object(s)");
		return sNextLine;
	}

	//AddActionParent command
	if (linecheck.substr(0, 16) == "addactionparent ")
	{
		//get data
		int params = SplitData(LineData, 16);

		if (params != 2)
			return ScriptError("Incorrect number of parameters");

		std::vector<Object*> objects;
		std::string tmpname = tempdata[1];
		SetCase(tmpname, false);
		if (tmpname == "global")
			objects.push_back(Simcore);
		else
			objects = Simcore->GetObjectRange(tempdata[1]);

		if (objects.size() > 0)
			Simcore->AddActionParent(tempdata[0], objects);
		else
			return ScriptError("Invalid parent object(s)");
		return sNextLine;
	}

	//RemoveActionParent command
	if (linecheck.substr(0, 19) == "removeactionparent ")
	{
		//get data
		int params = SplitData(LineData, 19);

		if (params != 2)
			return ScriptError("Incorrect number of parameters");

		std::vector<Object*> objects;
		std::string tmpname = tempdata[1];
		SetCase(tmpname, false);
		if (tmpname == "global")
			objects.push_back(Simcore);
		else
			objects = Simcore->GetObjectRange(tempdata[1]);

		if (objects.size() > 0)
			Simcore->RemoveActionParent(tempdata[0], objects);
		else
			return ScriptError("Invalid parent object(s)");
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
		{
			end = 7;

			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
		}

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
			control = Simcore->AddControl(tempdata[0], tempdata[1], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), 1, action_array, tex_array);
		else
			control = Simcore->AddControl(tempdata[0], tempdata[1], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToInt(tempdata[8]), action_array, tex_array);

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
		StoreCommand(Simcore->AddTrigger(tempdata[0], tempdata[1], min, max, action_array));
		return sNextLine;
	}

	//CreateWallObject command
	if (linecheck.substr(0, 16) == "createwallobject")
	{
		//get data
		int params = SplitData(LineData, 17);

		if (params != 2)
			return ScriptError("Incorrect number of parameters");

		MeshObject *mesh = GetMeshObject(tempdata[0]);

		if (!mesh)
			return ScriptError("Invalid object");

		StoreCommand(mesh->CreateWallObject(tempdata[1]));

		return sNextLine;
	}

	//SetKey command
	if (linecheck.substr(0, 6) == "setkey")
	{
		//get data
		int params = SplitData(LineData, 7);

		if (params != 1)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		if (!IsNumeric(tempdata[0]))
			return ScriptError("Invalid value: " + tempdata[0]);

		config->keyvalue = ToInt(tempdata[0]);
		if (config->keyvalue < 1)
		{
			config->keyvalue = 0;
			return ScriptError("Incorrect key ID number");
		}

		config->setkey = true;
		return sNextLine;
	}

	//SetLock command
	if (linecheck.substr(0, 7) == "setlock")
	{
		//get data
		int params = SplitData(LineData, 8);

		if (params != 2)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 1; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		config->lockvalue = ToInt(tempdata[0]);
		if (config->lockvalue < 0 || config->lockvalue > 3)
		{
			config->lockvalue = 0;
			config->keyvalue = 0;
			return ScriptError("Incorrect lock parameter");
		}

		config->keyvalue = ToInt(tempdata[1]);
		if (config->keyvalue < 0)
		{
			config->lockvalue = 0;
			config->keyvalue = 0;
			return ScriptError("Incorrect key ID number");
		}
		return sNextLine;
	}

	//Delete command
	if (linecheck.substr(0, 6) == "delete")
	{
		if (LineData.size() == 6)
			return ScriptError("Incorrect number of parameters");

		//calculate inline math
		std::string value = Calc(LineData.substr(7));

		int obj;
		if (!IsNumeric(value, obj))
			return ScriptError("Invalid value: " + value);

		//delete object
		Simcore->DeleteObject(obj);

		return sNextLine;
	}

	//RunAction command
	if (linecheck.substr(0, 9) == "runaction")
	{
		if (LineData.size() == 9)
			return ScriptError("Incorrect number of parameters");

		//calculate inline math
		std::string value = Calc(LineData.substr(10));

		//run action
		Simcore->RunAction(value);

		return sNextLine;
	}

	//Teleport command
	if (linecheck.substr(0, 8) == "teleport")
	{
		//get data
		int params = SplitData(LineData, 9);

		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 2; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		Simcore->camera->Teleport(ToFloat(tempdata[0]), ToFloat(tempdata[1]), ToFloat(tempdata[2]));

		return sNextLine;
	}

	//GotoFloor command
	if (linecheck.substr(0, 9) == "gotofloor")
	{
		if (LineData.size() == 9)
			return ScriptError("Incorrect number of parameters");

		//calculate inline math
		std::string value = Calc(LineData.substr(10));

		int num;
		if (!IsNumeric(value, num))
			return ScriptError("Invalid value: " + value);

		Simcore->camera->GotoFloor(num);

		return sNextLine;
	}

	//FloorInfo command
	if (linecheck.substr(0, 9) == "floorinfo")
	{
		if (LineData.size() == 9)
			Simcore->ShowFloorList();
		else
		{
			//calculate inline math
			std::string value = Calc(LineData.substr(10));

			int num;
			if (!IsNumeric(value, num))
				return ScriptError("Invalid value: " + value);

			Floor *floor = Simcore->GetFloor(num);
			if (floor)
				floor->ShowInfo();
		}
		return sNextLine;
	}

	//ListTextures command
	if (linecheck.substr(0, 12) == "listtextures")
	{
		Simcore->Report(texturemanager->ListTextures(true));
		return sNextLine;
	}

	//ListVisibleMeshes command
	if (linecheck.substr(0, 17) == "listvisiblemeshes")
	{
		Simcore->ListVisibleMeshes();
		return sNextLine;
	}

	//ShowLoadedSounds command
	if (linecheck.substr(0, 16) == "showloadedsounds")
	{
		if (Simcore->GetSoundSystem())
			Simcore->GetSoundSystem()->ShowLoadedSounds();
		return sNextLine;
	}

	//ShowPlayingSounds command
	if (linecheck.substr(0, 17) == "showplayingsounds")
	{
		if (Simcore->GetSoundSystem())
			Simcore->GetSoundSystem()->ShowPlayingSounds();
		return sNextLine;
	}

	//Print command
	if (linecheck.substr(0, 5) == "print")
	{
		//calculate inline math
		std::string value = Calc(LineData.substr(5));

		//print line
		engine->Report(value);

		return sNextLine;
	}

	return sContinue;
}

}
