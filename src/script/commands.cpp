/*
	Skyscraper 1.12 Alpha - Script Processor - Global Commands
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
#include "light.h"
#include "elevator.h"
#include "elevatorcar.h"
#include "cameratexture.h"
#include "door.h"
#include "manager.h"
#include "scriptproc.h"
#include "section.h"

using namespace SBS;

namespace Skyscraper {

ScriptProcessor::CommandsSection::CommandsSection(ScriptProcessor *parent) : Section(parent)
{

}

int ScriptProcessor::CommandsSection::Run(std::string &LineData)
{
	//process global commands

	//IF/While statement
	int IsIf = 0;
	if (StartsWithNoCase(LineData, "if"))
		IsIf = 1;
	if (StartsWithNoCase(LineData, "while"))
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
	if (StartsWithNoCase(LineData, "addtrianglewall"))
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
	if (StartsWithNoCase(LineData, "addwall"))
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
	if (StartsWithNoCase(LineData, "addfloor "))
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
	if (StartsWithNoCase(LineData, "addground"))
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
	if (StartsWithNoCase(LineData, "cut "))
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
		mesh->Cut(Vector3(ToFloat(tempdata[1]), ToFloat(tempdata[2]), ToFloat(tempdata[3])), Vector3(ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6])), ToBool(tempdata[7]), ToBool(tempdata[8]));
		return sNextLine;
	}

	//Set command
	if (StartsWithNoCase(LineData, "set "))
	{
		int loc = LineData.find("=", 0);
		if (loc < 0)
			return ScriptError("Syntax Error");

		std::string str = GetBeforeEquals(LineData, false);

		//reserved keywords
		if (str == "base" || str == "floor" || str == "height" || str == "interfloorheight" || str == "fullheight" || str == "elevator" || str == "minx" || str == "maxx" || str == "minz" || str == "maxz" || str == "number" || str.substr(0, 4) == "param" || str == "floorname" || str == "floortype" || str == "floorid" || str == "description")
			return ScriptError("Cannot use system variable name");

		//get text after equal sign
		bool equals;
		std::string value = Calc(GetAfterEquals(LineData, equals));

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
	if (StartsWithNoCase(LineData, "createwallbox2"))
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
				voffset += Real(Simcore->GetFloor(config->Current)->GetBase(true));
			else if (meshname == "external" || meshname == "landscape" || meshname == "buildings")
				voffset += Real(Simcore->GetFloor(config->Current)->GetBase());
		}

		StoreCommand(Simcore->CreateWallBox2(mesh, tempdata[1], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), voffset, ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToBool(tempdata[11]), ToBool(tempdata[12]), ToBool(tempdata[13]), ToBool(tempdata[14])));

		return sNextLine;
	}

	//CreateWallBox command
	if (StartsWithNoCase(LineData, "createwallbox "))
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
				voffset += Real(Simcore->GetFloor(config->Current)->GetBase(true));
			else if (meshname == "external" || meshname == "landscape" || meshname == "buildings")
				voffset += Real(Simcore->GetFloor(config->Current)->GetBase());
		}

		StoreCommand(Simcore->CreateWallBox(mesh, tempdata[1], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), voffset, ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToBool(tempdata[11]), ToBool(tempdata[12]), ToBool(tempdata[13]), ToBool(tempdata[14])));

		return sNextLine;
	}

	//AddCustomWall command
	if (StartsWithNoCase(LineData, "addcustomwall "))
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
					voffset += Real(Simcore->GetFloor(config->Current)->GetBase(true));
				else if (meshname == "external" || meshname == "landscape" || meshname == "buildings")
					voffset += Real(Simcore->GetFloor(config->Current)->GetBase());
			}
			else if (relative_option == false)
			{
				if (meshname == "floor" && config->SectionNum == 2)
					voffset -= mesh->GetPosition().y; //subtract altitude for new positioning model
			}
		}

		PolyArray varray;
		for (int i = start; i < params - 2; i += 3)
			varray.push_back(Vector3(ToFloat(tempdata[i]), ToFloat(tempdata[i + 1]) + voffset, ToFloat(tempdata[i + 2])));

		StoreCommand(Simcore->AddCustomWall(mesh, tempdata[1], tempdata[2], varray, ToFloat(tempdata[params - 2]), ToFloat(tempdata[params - 1])));

		return sNextLine;
	}

	//AddCustomFloor command
	if (StartsWithNoCase(LineData, "addcustomfloor "))
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

		std::vector<Vector2> varray;
		for (int i = 3; i < params - 3; i += 2)
			varray.push_back(Vector2(ToFloat(tempdata[i]), ToFloat(tempdata[i + 1])));

		StoreCommand(Simcore->AddCustomFloor(mesh, tempdata[1], tempdata[2], varray, altitude, ToFloat(tempdata[params - 2]), ToFloat(tempdata[params - 1])));

		return sNextLine;
	}

	//AddPolygon command
	if (StartsWithNoCase(LineData, "addpolygon"))
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

		PolyArray varray;
		for (int i = 3; i < params - 2; i += 3)
			varray.push_back(Vector3(ToFloat(tempdata[i]), ToFloat(tempdata[i + 1]) + voffset, ToFloat(tempdata[i + 2])));

		Simcore->AddPolygon(wall, tempdata[2], varray, ToFloat(tempdata[params - 2]), ToFloat(tempdata[params - 1]));

		return sNextLine;
	}

	//AddShaft command
	if (StartsWithNoCase(LineData, "addshaft "))
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
	if (StartsWithNoCase(LineData, "shaftcut "))
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

		Simcore->GetShaft(shaftnum)->CutFloors(true, Vector2(ToFloat(tempdata[1]), ToFloat(tempdata[2])), Vector2(ToFloat(tempdata[3]), ToFloat(tempdata[4])), ToFloat(tempdata[5]), ToFloat(tempdata[6]));
		return sNextLine;
	}

	//ShaftShowFloors command
	if (StartsWithNoCase(LineData, "shaftshowfloors"))
	{
		//get shaft number
		int loc = LineData.find("=");
		if (loc < 0)
			return ScriptError("Syntax error");
		int shaftnum;
		std::string str = GetBeforeEquals(LineData);
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
	if (StartsWithNoCase(LineData, "shaftshowinterfloors"))
	{
		//get shaft number
		int loc = LineData.find("=");
		if (loc < 0)
			return ScriptError("Syntax error");
		int shaftnum;
		std::string str = GetBeforeEquals(LineData);
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
			int start, end;
			if (GetRange(tempdata[line], start, end))
			{
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
	if (StartsWithNoCase(LineData, "shaftshowoutside"))
	{
		//get shaft number
		int loc = LineData.find("=");
		if (loc < 0)
			return ScriptError("Syntax error");
		int shaftnum;
		std::string str = GetBeforeEquals(LineData);
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
			int start, end;
			if (GetRange(tempdata[line], start, end))
			{
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
	if (StartsWithNoCase(LineData, "showfullshaft"))
	{
		//get shaft number
		int loc = LineData.find("=");
		if (loc < 0)
			return ScriptError("Syntax error");
		int shaftnum;
		std::string str = GetBeforeEquals(LineData);
		if (!IsNumeric(str, shaftnum))
			return ScriptError("Invalid shaft number");
		if (shaftnum < 1 || shaftnum > Simcore->GetShaftCount())
			return ScriptError("Invalid shaft number");

		//get text after equal sign
		bool equals;
		std::string value = GetAfterEquals(LineData, equals);

		Simcore->GetShaft(shaftnum)->SetShowFull(ToBool(value));
		return sNextLine;
	}

	//CreateStairwell command
	if (StartsWithNoCase(LineData, "createstairwell"))
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

		Stairwell *stairs = Simcore->CreateStairwell(ToInt(tempdata[0]), ToFloat(tempdata[1]), ToFloat(tempdata[2]), ToInt(tempdata[3]), ToInt(tempdata[4]));
		if (!stairs)
			return ScriptError();

		StoreCommand(stairs);
		return sNextLine;
	}

	//CutStairwell command
	if (StartsWithNoCase(LineData, "cutstairwell "))
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
		if (!Simcore->GetStairwell(stairwell))
			return ScriptError("Invalid stairwell " + tempdata[0]);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		Simcore->GetStairwell(stairwell)->CutFloors(true, Vector2(ToFloat(tempdata[1]), ToFloat(tempdata[2])), Vector2(ToFloat(tempdata[3]), ToFloat(tempdata[4])), ToFloat(tempdata[5]), ToFloat(tempdata[6]));
		return sNextLine;
	}

	//StairsShowFloors command
	if (StartsWithNoCase(LineData, "stairsshowfloors"))
	{
		//get stairwell number
		int loc = LineData.find("=");
		if (loc < 0)
			return ScriptError("Syntax error");
		int stairnum;
		std::string str = GetBeforeEquals(LineData);
		if (!IsNumeric(str, stairnum))
			return ScriptError("Invalid stairwell number");

		if (stairnum < 1 || stairnum > Simcore->GetStairwellCount())
			return ScriptError("Invalid stairwell number");

		Simcore->GetStairwell(stairnum)->ShowFloors = true;

		int params = SplitAfterEquals(LineData, false);
		if (params < 1)
			return ScriptError("Syntax Error");

		for (int line = 0; line < params; line++)
		{
			int start, end;
			if (GetRange(tempdata[line], start, end))
			{
				for (int k = start; k <= end; k++)
					Simcore->GetStairwell(stairnum)->AddShowFloor(k);
			}
			else
			{
				int showfloor;
				if (!IsNumeric(tempdata[line], showfloor))
					return ScriptError("Invalid value");
				Simcore->GetStairwell(stairnum)->AddShowFloor(showfloor);
			}
		}
		return sNextLine;
	}

	//ShowFullStairs command
	if (StartsWithNoCase(LineData, "showfullstairs"))
	{
		//get shaft number
		int loc = LineData.find("=");
		if (loc < 0)
			return ScriptError("Syntax error");
		int stairnum;
		std::string str = GetBeforeEquals(LineData);
		if (!IsNumeric(str, stairnum))
			return ScriptError("Invalid stairwell number");
		if (stairnum < 1 || stairnum > Simcore->GetStairwellCount())
			return ScriptError("Invalid stairwell number");

		//get text after equal sign
		bool equals;
		std::string strvalue = GetAfterEquals(LineData, equals);
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

		Simcore->GetStairwell(stairnum)->SetShowFull(value);
		return sNextLine;
	}

	//WallOrientation command
	if (StartsWithNoCase(LineData, "wallorientation"))
	{
		//get text after equal sign
		bool equals;
		std::string value = GetAfterEquals(LineData, equals);

		if (!Simcore->SetWallOrientation(value))
			return ScriptError();
		return sNextLine;
	}

	//FloorOrientation command
	if (StartsWithNoCase(LineData, "floororientation"))
	{
		//get text after equal sign
		bool equals;
		std::string value = GetAfterEquals(LineData, equals);

		if (!Simcore->SetFloorOrientation(value))
			return ScriptError();
		return sNextLine;
	}

	//DrawWalls command
	if (StartsWithNoCase(LineData, "drawwalls"))
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
	if (StartsWithNoCase(LineData, "settexturemapping "))
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

		texturemanager->SetTextureMapping(ToInt(tempdata[0]), Vector2(ToFloat(tempdata[1]), ToFloat(tempdata[2])),
									ToInt(tempdata[3]), Vector2(ToFloat(tempdata[4]), ToFloat(tempdata[5])),
									ToInt(tempdata[6]), Vector2(ToFloat(tempdata[7]), ToFloat(tempdata[8])));
		return sNextLine;
	}

	//SetTextureMapping2 command
	if (StartsWithNoCase(LineData, "settexturemapping2"))
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

		texturemanager->SetTextureMapping2(tempdata[0], tempdata[1], tempdata[2], Vector2(ToFloat(tempdata[3]), ToFloat(tempdata[4])),
									tempdata[5], tempdata[6], tempdata[7], Vector2(ToFloat(tempdata[8]), ToFloat(tempdata[9])),
									tempdata[10], tempdata[11], tempdata[12], Vector2(ToFloat(tempdata[13]), ToFloat(tempdata[14])));
		return sNextLine;
	}

	//ResetTextureMapping command
	if (StartsWithNoCase(LineData, "resettexturemapping"))
	{
		int check = (int)LineData.find("=", 0);
		if (check < 0)
			return ScriptError("Syntax Error");

		//get text after equal sign
		bool equals;
		std::string value = GetAfterEquals(LineData, equals);

		texturemanager->ResetTextureMapping(ToBool(value));
		return sNextLine;
	}

	//SetPlanarMapping command
	if (StartsWithNoCase(LineData, "setplanarmapping"))
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
	if (StartsWithNoCase(LineData, "reverseaxis"))
	{
		//backwards compatibility
		if (warn_deprecated == true)
			ScriptWarning("Command deprecated");

		int check = (int)LineData.find("=", 0);
		if (check < 0)
			return ScriptError("Syntax Error");
		bool equals;
		std::string value = GetAfterEquals(LineData, equals);

		config->ReverseAxis = ToBool(value);
		return sNextLine;
	}

	//Intersection points
	std::string linecheck = SetCaseCopy(LineData, false);
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

		Vector3 isect = mesh->GetPoint(tempdata[1], Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4])), Vector3(ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7])));

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

		Vector2 startpoint (ToFloat(tempdata[0]), ToFloat(tempdata[1]));
		Vector2 endpoint = Simcore->GetEndPoint(startpoint, ToFloat(tempdata[2]), ToFloat(tempdata[3]));

		buffer = LineData.substr(0, found) + ToString(endpoint.x) + ", " + ToString(endpoint.y) + LineData.substr(loc2 + 1);
		LineData = buffer;
		linecheck = SetCaseCopy(LineData, false);
		found = linecheck.find("endpoint(", 0);
	}

	//GetWallExtents command
	if (StartsWithNoCase(LineData, "getwallextents"))
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
	if (StartsWithNoCase(LineData, "setautosize"))
	{
		int params = SplitAfterEquals(LineData);

		if (params != 2)
			return ScriptError("Incorrect number of parameters");

		texturemanager->SetAutoSize(ToBool(tempdata[0]),
					ToBool(tempdata[1]));
		return sNextLine;
	}

	//TextureOverride command
	if (StartsWithNoCase(LineData, "textureoverride"))
	{
		int params = SplitData(LineData, 16, false);

		if (params != 6)
			return ScriptError("Incorrect number of parameters");

		texturemanager->SetTextureOverride(tempdata[0], tempdata[1], tempdata[2], tempdata[3], tempdata[4], tempdata[5]);
		return sSkipReset;
	}

	//TextureFlip command
	if (StartsWithNoCase(LineData, "textureflip"))
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
	if (StartsWithNoCase(LineData, "mount"))
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
	if (StartsWithNoCase(LineData, "addfloorautoarea"))
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
		Simcore->AddFloorAutoArea(Vector3(ToFloat(tempdata[0]), ToFloat(tempdata[1]), ToFloat(tempdata[2])), Vector3(ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5])));
		return sNextLine;
	}

	//AddSound command
	if (StartsWithNoCase(LineData, "addsound"))
	{
		//get data
		int params = SplitData(LineData, 9);

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
				StoreCommand(Simcore->AddSound(tempdata[0], tempdata[1], Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]))));
			else
				StoreCommand(Simcore->AddSound(tempdata[0], tempdata[1], Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4])), true, ToFloat(tempdata[5]), ToInt(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), 0.0, 360, 360, 1.0, Vector3(ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]))));
		}
		else
		{
			if (partial == true)
				StoreCommand(Simcore->AddSound(tempdata[0], tempdata[1], Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4])), ToBool(tempdata[5])));
			else
				StoreCommand(Simcore->AddSound(tempdata[0], tempdata[1], Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4])), ToBool(tempdata[5]), ToFloat(tempdata[6]), ToInt(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), Vector3(ToFloat(tempdata[14]), ToFloat(tempdata[15]), ToFloat(tempdata[16]))));
		}
		return sNextLine;
	}

	//AddModel command
	if (StartsWithNoCase(LineData, "addmodel"))
	{
		//get data
		int params = SplitData(LineData, 9);

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
			model = Simcore->AddModel(tempdata[0], tempdata[1], false, Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4])), Vector3(ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7])), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToBool(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]));
		else
			model = Simcore->AddModel(tempdata[0], tempdata[1], ToBool(tempdata[2]), Vector3(ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5])), Vector3(ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8])), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToBool(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]));

		if (config->setkey == true && model)
			model->SetKey(config->keyvalue);
		config->setkey = false;

		StoreCommand(model);
		return sNextLine;
	}

	//AddAction command
	if (StartsWithNoCase(LineData, "addaction "))
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
	if (StartsWithNoCase(LineData, "addactionparent "))
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
	if (StartsWithNoCase(LineData, "removeactionparent "))
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
	if (StartsWithNoCase(LineData, "addactioncontrol"))
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
	if (StartsWithNoCase(LineData, "addtrigger"))
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

		Vector3 min = Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]));
		Vector3 max = Vector3(ToFloat(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]));
		StoreCommand(Simcore->AddTrigger(tempdata[0], tempdata[1], min, max, action_array));
		return sNextLine;
	}

	//AddLight command
	if (StartsWithNoCase(LineData, "addlight "))
	{
		//get data
		int params = SplitData(LineData, 9);

		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		if (!IsNumeric(tempdata[2]))
			return ScriptError("Invalid value: " + tempdata[1]);

		//int floor;
		std::string name = tempdata[0];
		TrimString(name);
		Object *obj = Simcore->GetObject(name);

		if (!obj)
			return ScriptError("Invalid object " + name);

		Floor *floorobj = 0;
		Elevator *elevatorobj = 0;
		ElevatorCar *elevatorcarobj = 0;
		Shaft::Level *shaftobj = 0;
		Stairwell::Level *stairsobj = 0;

		//get parent object of light
		if (obj->GetType() == "Floor")
			floorobj = static_cast<Floor*>(obj);
		if (obj->GetType() == "Elevator")
			elevatorobj = static_cast<Elevator*>(obj);
		if (obj->GetType() == "ElevatorCar")
			elevatorcarobj = static_cast<ElevatorCar*>(obj);
		if (obj->GetType() == "Shaft Level")
			shaftobj = static_cast<Shaft::Level*>(obj);
		if (obj->GetType() == "Stairwell Level")
			stairsobj = static_cast<Stairwell::Level*>(obj);

		if (elevatorobj)
			elevatorcarobj = elevatorobj->GetCar(0);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//create light
		if (floorobj)
			StoreCommand(floorobj->AddLight(tempdata[1], ToInt(tempdata[2])));
		else if (elevatorcarobj)
			StoreCommand(elevatorcarobj->AddLight(tempdata[1], ToInt(tempdata[2])));
		else if (shaftobj)
			StoreCommand(shaftobj->AddLight(tempdata[1], ToInt(tempdata[2])));
		else if (stairsobj)
			StoreCommand(stairsobj->AddLight(tempdata[1], ToInt(tempdata[2])));
		else
			return ScriptError("Invalid object " + name);

		return sNextLine;
	}

	//SetLightColor command
	if (StartsWithNoCase(LineData, "setlightcolor "))
	{
		//get data
		int params = SplitData(LineData, 14);

		if (params != 5)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 4; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::string name = tempdata[0];
		TrimString(name);
		Object *obj = Simcore->GetObject(name);

		if (!obj)
			return ScriptError("Invalid object " + name);

		Floor *floorobj = 0;
		Elevator *elevatorobj = 0;
		ElevatorCar *elevatorcarobj = 0;
		Shaft::Level *shaftobj = 0;
		Stairwell::Level *stairsobj = 0;

		//get parent object
		if (obj->GetType() == "Floor")
			floorobj = static_cast<Floor*>(obj);
		if (obj->GetType() == "Elevator")
			elevatorobj = static_cast<Elevator*>(obj);
		if (obj->GetType() == "ElevatorCar")
			elevatorcarobj = static_cast<ElevatorCar*>(obj);
		if (obj->GetType() == "Shaft Level")
			shaftobj = static_cast<Shaft::Level*>(obj);
		if (obj->GetType() == "Stairwell Level")
			stairsobj = static_cast<Stairwell::Level*>(obj);

		if (elevatorobj)
			elevatorcarobj = elevatorobj->GetCar(0);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//get light object
		Light *light = 0;
		if (floorobj)
			light = floorobj->GetLight(tempdata[1]);
		if (elevatorcarobj)
			light = elevatorcarobj->GetLight(tempdata[1]);
		if (shaftobj)
			light = shaftobj->GetLight(tempdata[1]);
		if (stairsobj)
			light = stairsobj->GetLight(tempdata[1]);

		if (!light)
			return ScriptError("Invalid light " + tempdata[1] + " in " + name);

		//modify light
		light->SetColor(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]));

		return sNextLine;
	}

	//SetLightSpecular command
	if (StartsWithNoCase(LineData, "setlightspecular "))
	{
		//get data
		int params = SplitData(LineData, 17);

		if (params != 5)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 4; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::string name = tempdata[0];
		TrimString(name);
		Object *obj = Simcore->GetObject(name);

		if (!obj)
			return ScriptError("Invalid object " + name);

		Floor *floorobj = 0;
		Elevator *elevatorobj = 0;
		ElevatorCar *elevatorcarobj = 0;
		Shaft::Level *shaftobj = 0;
		Stairwell::Level *stairsobj = 0;

		//get parent object
		if (obj->GetType() == "Floor")
			floorobj = static_cast<Floor*>(obj);
		if (obj->GetType() == "Elevator")
			elevatorobj = static_cast<Elevator*>(obj);
		if (obj->GetType() == "ElevatorCar")
			elevatorcarobj = static_cast<ElevatorCar*>(obj);
		if (obj->GetType() == "Shaft Level")
			shaftobj = static_cast<Shaft::Level*>(obj);
		if (obj->GetType() == "Stairwell Level")
			stairsobj = static_cast<Stairwell::Level*>(obj);

		if (elevatorobj)
			elevatorcarobj = elevatorobj->GetCar(0);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//get light object
		Light *light = 0;
		if (floorobj)
			light = floorobj->GetLight(tempdata[1]);
		if (elevatorcarobj)
			light = elevatorcarobj->GetLight(tempdata[1]);
		if (shaftobj)
			light = shaftobj->GetLight(tempdata[1]);
		if (stairsobj)
			light = stairsobj->GetLight(tempdata[1]);

		if (!light)
			return ScriptError("Invalid light " + tempdata[1] + " in " + name);

		//modify light
		light->SetSpecularColor(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]));

		return sNextLine;
	}

	//SetLightAttenuation command
	if (StartsWithNoCase(LineData, "setlightattenuation "))
	{
		//get data
		int params = SplitData(LineData, 20);

		if (params != 6)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 5; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::string name = tempdata[0];
		TrimString(name);
		Object *obj = Simcore->GetObject(name);

		if (!obj)
			return ScriptError("Invalid object " + name);

		Floor *floorobj = 0;
		Elevator *elevatorobj = 0;
		ElevatorCar *elevatorcarobj = 0;
		Shaft::Level *shaftobj = 0;
		Stairwell::Level *stairsobj = 0;

		//get parent object
		if (obj->GetType() == "Floor")
			floorobj = static_cast<Floor*>(obj);
		if (obj->GetType() == "Elevator")
			elevatorobj = static_cast<Elevator*>(obj);
		if (obj->GetType() == "ElevatorCar")
			elevatorcarobj = static_cast<ElevatorCar*>(obj);
		if (obj->GetType() == "Shaft Level")
			shaftobj = static_cast<Shaft::Level*>(obj);
		if (obj->GetType() == "Stairwell Level")
			stairsobj = static_cast<Stairwell::Level*>(obj);

		if (elevatorobj)
			elevatorcarobj = elevatorobj->GetCar(0);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//get light object
		Light *light = 0;
		if (floorobj)
			light = floorobj->GetLight(tempdata[1]);
		if (elevatorcarobj)
			light = elevatorcarobj->GetLight(tempdata[1]);
		if (shaftobj)
			light = shaftobj->GetLight(tempdata[1]);
		if (stairsobj)
			light = stairsobj->GetLight(tempdata[1]);

		if (!light)
			return ScriptError("Invalid light " + tempdata[1] + " in " + name);

		//modify light
		light->SetAttenuation(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]));

		return sNextLine;
	}

	//SetSpotlightRange command
	if (StartsWithNoCase(LineData, "setspotlightrange "))
	{
		//get data
		int params = SplitData(LineData, 18);

		if (params != 5)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 4; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::string name = tempdata[0];
		TrimString(name);
		Object *obj = Simcore->GetObject(name);

		if (!obj)
			return ScriptError("Invalid object " + name);

		Floor *floorobj = 0;
		Elevator *elevatorobj = 0;
		ElevatorCar *elevatorcarobj = 0;
		Shaft::Level *shaftobj = 0;
		Stairwell::Level *stairsobj = 0;

		//get parent object
		if (obj->GetType() == "Floor")
			floorobj = static_cast<Floor*>(obj);
		if (obj->GetType() == "Elevator")
			elevatorobj = static_cast<Elevator*>(obj);
		if (obj->GetType() == "ElevatorCar")
			elevatorcarobj = static_cast<ElevatorCar*>(obj);
		if (obj->GetType() == "Shaft Level")
			shaftobj = static_cast<Shaft::Level*>(obj);
		if (obj->GetType() == "Stairwell Level")
			stairsobj = static_cast<Stairwell::Level*>(obj);

		if (elevatorobj)
			elevatorcarobj = elevatorobj->GetCar(0);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//get light object
		Light *light = 0;
		if (floorobj)
			light = floorobj->GetLight(tempdata[1]);
		if (elevatorcarobj)
			light = elevatorcarobj->GetLight(tempdata[1]);
		if (shaftobj)
			light = shaftobj->GetLight(tempdata[1]);
		if (stairsobj)
			light = stairsobj->GetLight(tempdata[1]);

		if (!light)
			return ScriptError("Invalid light " + tempdata[1] + " in " + name);

		//modify light
		light->SetSpotlightRange(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]));

		return sNextLine;
	}

	//SetLightDirection command
	if (StartsWithNoCase(LineData, "setlightdirection "))
	{
		//get data
		int params = SplitData(LineData, 18);

		if (params != 5)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 4; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::string name = tempdata[0];
		TrimString(name);
		Object *obj = Simcore->GetObject(name);

		if (!obj)
			return ScriptError("Invalid object " + name);

		Floor *floorobj = 0;
		Elevator *elevatorobj = 0;
		ElevatorCar *elevatorcarobj = 0;
		Shaft::Level *shaftobj = 0;
		Stairwell::Level *stairsobj = 0;

		//get parent object
		if (obj->GetType() == "Floor")
			floorobj = static_cast<Floor*>(obj);
		if (obj->GetType() == "Elevator")
			elevatorobj = static_cast<Elevator*>(obj);
		if (obj->GetType() == "ElevatorCar")
			elevatorcarobj = static_cast<ElevatorCar*>(obj);
		if (obj->GetType() == "Shaft Level")
			shaftobj = static_cast<Shaft::Level*>(obj);
		if (obj->GetType() == "Stairwell Level")
			stairsobj = static_cast<Stairwell::Level*>(obj);

		if (elevatorobj)
			elevatorcarobj = elevatorobj->GetCar(0);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//get light object
		Light *light = 0;
		if (floorobj)
			light = floorobj->GetLight(tempdata[1]);
		if (elevatorcarobj)
			light = elevatorcarobj->GetLight(tempdata[1]);
		if (shaftobj)
			light = shaftobj->GetLight(tempdata[1]);
		if (stairsobj)
			light = stairsobj->GetLight(tempdata[1]);

		if (!light)
			return ScriptError("Invalid light " + tempdata[1] + " in " + name);

		//modify light
		light->SetDirection(Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4])));

		return sNextLine;
	}

	//MoveLight command
	if (StartsWithNoCase(LineData, "movelight "))
	{
		//get data
		int params = SplitData(LineData, 10);

		if (params != 5)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 4; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::string name = tempdata[0];
		TrimString(name);
		Object *obj = Simcore->GetObject(name);

		if (!obj)
			return ScriptError("Invalid object " + name);

		Floor *floorobj = 0;
		Elevator *elevatorobj = 0;
		ElevatorCar *elevatorcarobj = 0;
		Shaft::Level *shaftobj = 0;
		Stairwell::Level *stairsobj = 0;

		//get parent object
		if (obj->GetType() == "Floor")
			floorobj = static_cast<Floor*>(obj);
		if (obj->GetType() == "Elevator")
			elevatorobj = static_cast<Elevator*>(obj);
		if (obj->GetType() == "ElevatorCar")
			elevatorcarobj = static_cast<ElevatorCar*>(obj);
		if (obj->GetType() == "Shaft Level")
			shaftobj = static_cast<Shaft::Level*>(obj);
		if (obj->GetType() == "Stairwell Level")
			stairsobj = static_cast<Stairwell::Level*>(obj);

		if (elevatorobj)
			elevatorcarobj = elevatorobj->GetCar(0);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//get light object
		Light *light = 0;
		if (floorobj)
			light = floorobj->GetLight(tempdata[1]);
		if (elevatorcarobj)
			light = elevatorcarobj->GetLight(tempdata[1]);
		if (shaftobj)
			light = shaftobj->GetLight(tempdata[1]);
		if (stairsobj)
			light = stairsobj->GetLight(tempdata[1]);

		if (!light)
			return ScriptError("Invalid light " + tempdata[1] + " in " + name);

		//move light
		light->Move(Vector3(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4])));

		return sNextLine;
	}

	//AddCameraTexture command
	if (StartsWithNoCase(LineData, "addcameratexture"))
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

		std::string name = tempdata[0];
		TrimString(name);
		Object *obj = Simcore->GetObject(name);

		if (!obj)
			return ScriptError("Invalid object " + name);

		Floor *floorobj = 0;
		Elevator *elevatorobj = 0;
		ElevatorCar *elevatorcarobj = 0;
		Shaft::Level *shaftobj = 0;
		Stairwell::Level *stairsobj = 0;

		//get parent object
		if (obj->GetType() == "Floor")
			floorobj = static_cast<Floor*>(obj);
		if (obj->GetType() == "Elevator")
			elevatorobj = static_cast<Elevator*>(obj);
		if (obj->GetType() == "ElevatorCar")
			elevatorcarobj = static_cast<ElevatorCar*>(obj);
		if (obj->GetType() == "Shaft Level")
			shaftobj = static_cast<Shaft::Level*>(obj);
		if (obj->GetType() == "Stairwell Level")
			stairsobj = static_cast<Stairwell::Level*>(obj);

		if (elevatorobj)
			elevatorcarobj = elevatorobj->GetCar(0);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		if (floorobj)
			StoreCommand(floorobj->AddCameraTexture(tempdata[1], ToInt(tempdata[2]), ToFloat(tempdata[3]), Vector3(ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6])), ToBool(tempdata[7]), Vector3(ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]))));
		else if (elevatorcarobj)
			StoreCommand(elevatorcarobj->AddCameraTexture(tempdata[1], ToInt(tempdata[2]), ToFloat(tempdata[3]), Vector3(ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6])), ToBool(tempdata[7]), Vector3(ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]))));
		else if (shaftobj)
			StoreCommand(shaftobj->AddCameraTexture(tempdata[1], ToInt(tempdata[2]), ToFloat(tempdata[3]), Vector3(ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6])), ToBool(tempdata[7]), Vector3(ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]))));
		else if (stairsobj)
			StoreCommand(stairsobj->AddCameraTexture(tempdata[1], ToInt(tempdata[2]), ToFloat(tempdata[3]), Vector3(ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6])), ToBool(tempdata[7]), Vector3(ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]))));
		else
			return ScriptError("Invalid object");

		return sNextLine;
	}

	//AddSlidingDoor command
	if (StartsWithNoCase(LineData, "addslidingdoor"))
	{
		//get data
		int params = SplitData(LineData, 15);

		if (params != 21)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 7; i <= 20; i++)
		{
			if (i == 8)
				i = 10;

			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::string name = tempdata[0];
		TrimString(name);
		Object *obj = Simcore->GetObject(name);

		if (!obj)
			return ScriptError("Invalid object " + name);

		Floor *floorobj = 0;
		Elevator *elevatorobj = 0;
		ElevatorCar *elevatorcarobj = 0;
		Shaft::Level *shaftobj = 0;
		Stairwell::Level *stairsobj = 0;
		DoorManager *managerobj = 0;

		//get parent object
		if (obj->GetType() == "Floor")
			floorobj = static_cast<Floor*>(obj);
		if (obj->GetType() == "Elevator")
			elevatorobj = static_cast<Elevator*>(obj);
		if (obj->GetType() == "ElevatorCar")
			elevatorcarobj = static_cast<ElevatorCar*>(obj);
		if (obj->GetType() == "Shaft Level")
			shaftobj = static_cast<Shaft::Level*>(obj);
		if (obj->GetType() == "Stairwell Level")
			stairsobj = static_cast<Stairwell::Level*>(obj);
		if (obj->GetType() == "DoorManager")
			managerobj = static_cast<DoorManager*>(obj);

		if (elevatorobj)
			elevatorcarobj = elevatorobj->GetCar(0);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//create door
		Door* door = 0;

		if (floorobj)
			door = floorobj->AddDoor(tempdata[1], tempdata[2], tempdata[3], ToBool(tempdata[4]), tempdata[5], tempdata[6], ToFloat(tempdata[7]), tempdata[8], tempdata[9], false, ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]), ToFloat(tempdata[15]), ToFloat(tempdata[16]), ToFloat(tempdata[17]), ToFloat(tempdata[18]), ToFloat(tempdata[19]), ToFloat(tempdata[20]));
		else if (elevatorcarobj)
			door = elevatorcarobj->AddDoor(tempdata[1], tempdata[2], tempdata[3], ToBool(tempdata[4]), tempdata[5], tempdata[6], ToFloat(tempdata[7]), tempdata[8], tempdata[9], false, ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]), ToFloat(tempdata[15]), ToFloat(tempdata[16]), ToFloat(tempdata[17]), ToFloat(tempdata[18]), ToFloat(tempdata[19]), ToFloat(tempdata[20]));
		else if (shaftobj)
			door = shaftobj->AddDoor(tempdata[1], tempdata[2], tempdata[3], ToBool(tempdata[4]), tempdata[5], tempdata[6], ToFloat(tempdata[7]), tempdata[8], tempdata[9], false, ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]), ToFloat(tempdata[15]), ToFloat(tempdata[16]), ToFloat(tempdata[17]), ToFloat(tempdata[18]), ToFloat(tempdata[19]), ToFloat(tempdata[20]));
		else if (stairsobj)
			door = stairsobj->AddDoor(tempdata[1], tempdata[2], tempdata[3], ToBool(tempdata[4]), tempdata[5], tempdata[6], ToFloat(tempdata[7]), tempdata[8], tempdata[9], false, ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]), ToFloat(tempdata[15]), ToFloat(tempdata[16]), ToFloat(tempdata[17]), ToFloat(tempdata[18]), ToFloat(tempdata[19]), ToFloat(tempdata[20]));
		else if (managerobj)
			door = managerobj->AddDoor(tempdata[1], tempdata[2], tempdata[3], ToBool(tempdata[4]), tempdata[5], tempdata[6], ToFloat(tempdata[7]), tempdata[8], tempdata[9], false, ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]), ToFloat(tempdata[15]), ToFloat(tempdata[16]), ToFloat(tempdata[17]), ToFloat(tempdata[18]), ToFloat(tempdata[19]), ToFloat(tempdata[20]));
		else
			return ScriptError("Invalid object");

		if (door)
			door->SetLocked(config->lockvalue, config->keyvalue);

		StoreCommand(door);
		return sNextLine;
	}

	//AddStdDoor command
	if (StartsWithNoCase(LineData, "addstddoor"))
	{
		//get data
		int params = SplitData(LineData, 11);

		if (params != 21)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 7; i <= 20; i++)
		{
			if (i == 8)
				i = 10;

			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::string name = tempdata[0];
		TrimString(name);
		Object *obj = Simcore->GetObject(name);

		if (!obj)
			return ScriptError("Invalid object " + name);

		Floor *floorobj = 0;
		Elevator *elevatorobj = 0;
		ElevatorCar *elevatorcarobj = 0;
		Shaft::Level *shaftobj = 0;
		Stairwell::Level *stairsobj = 0;
		DoorManager *managerobj = 0;

		//get parent object
		if (obj->GetType() == "Floor")
			floorobj = static_cast<Floor*>(obj);
		if (obj->GetType() == "Elevator")
			elevatorobj = static_cast<Elevator*>(obj);
		if (obj->GetType() == "ElevatorCar")
			elevatorcarobj = static_cast<ElevatorCar*>(obj);
		if (obj->GetType() == "Shaft Level")
			shaftobj = static_cast<Shaft::Level*>(obj);
		if (obj->GetType() == "Stairwell Level")
			stairsobj = static_cast<Stairwell::Level*>(obj);
		if (obj->GetType() == "DoorManager")
			managerobj = static_cast<DoorManager*>(obj);

		if (elevatorobj)
			elevatorcarobj = elevatorobj->GetCar(0);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//create door
		Door* door = 0;

		if (floorobj)
			door = floorobj->AddDoor(tempdata[1], tempdata[2], tempdata[3], ToBool(tempdata[4]), tempdata[5], tempdata[6], ToFloat(tempdata[7]), tempdata[8], tempdata[9], true, ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]), ToFloat(tempdata[15]), ToFloat(tempdata[16]), ToFloat(tempdata[17]), ToFloat(tempdata[18]), ToFloat(tempdata[19]), ToFloat(tempdata[20]));
		else if (elevatorcarobj)
			door = elevatorcarobj->AddDoor(tempdata[1], tempdata[2], tempdata[3], ToBool(tempdata[4]), tempdata[5], tempdata[6], ToFloat(tempdata[7]), tempdata[8], tempdata[9], true, ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]), ToFloat(tempdata[15]), ToFloat(tempdata[16]), ToFloat(tempdata[17]), ToFloat(tempdata[18]), ToFloat(tempdata[19]), ToFloat(tempdata[20]));
		else if (shaftobj)
			door = shaftobj->AddDoor(tempdata[1], tempdata[2], tempdata[3], ToBool(tempdata[4]), tempdata[5], tempdata[6], ToFloat(tempdata[7]), tempdata[8], tempdata[9], true, ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]), ToFloat(tempdata[15]), ToFloat(tempdata[16]), ToFloat(tempdata[17]), ToFloat(tempdata[18]), ToFloat(tempdata[19]), ToFloat(tempdata[20]));
		else if (stairsobj)
			door = stairsobj->AddDoor(tempdata[1], tempdata[2], tempdata[3], ToBool(tempdata[4]), tempdata[5], tempdata[6], ToFloat(tempdata[7]), tempdata[8], tempdata[9], true, ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]), ToFloat(tempdata[15]), ToFloat(tempdata[16]), ToFloat(tempdata[17]), ToFloat(tempdata[18]), ToFloat(tempdata[19]), ToFloat(tempdata[20]));
		else if (managerobj)
			door = managerobj->AddDoor(tempdata[1], tempdata[2], tempdata[3], ToBool(tempdata[4]), tempdata[5], tempdata[6], ToFloat(tempdata[7]), tempdata[8], tempdata[9], true, ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]), ToFloat(tempdata[15]), ToFloat(tempdata[16]), ToFloat(tempdata[17]), ToFloat(tempdata[18]), ToFloat(tempdata[19]), ToFloat(tempdata[20]));
		else
			return ScriptError("Invalid object");

		if (door)
			door->SetLocked(config->lockvalue, config->keyvalue);

		StoreCommand(door);
		return sNextLine;
	}

	//CreateCustomDoor command
	if (StartsWithNoCase(LineData, "createcustomdoor"))
	{
		//get data
		int params = SplitData(LineData, 17);

		if (params != 5)
			return ScriptError("Incorrect number of parameters");

		std::string name = tempdata[0];
		TrimString(name);
		Object *obj = Simcore->GetObject(name);

		if (!obj)
			return ScriptError("Invalid object " + name);

		Floor *floorobj = 0;
		Elevator *elevatorobj = 0;
		ElevatorCar *elevatorcarobj = 0;
		Shaft::Level *shaftobj = 0;
		Stairwell::Level *stairsobj = 0;
		DoorManager *managerobj = 0;

		//get parent object
		if (obj->GetType() == "Floor")
			floorobj = static_cast<Floor*>(obj);
		if (obj->GetType() == "Elevator")
			elevatorobj = static_cast<Elevator*>(obj);
		if (obj->GetType() == "ElevatorCar")
			elevatorcarobj = static_cast<ElevatorCar*>(obj);
		if (obj->GetType() == "Shaft Level")
			shaftobj = static_cast<Shaft::Level*>(obj);
		if (obj->GetType() == "Stairwell Level")
			stairsobj = static_cast<Stairwell::Level*>(obj);
		if (obj->GetType() == "DoorManager")
			managerobj = static_cast<DoorManager*>(obj);

		if (elevatorobj)
			elevatorcarobj = elevatorobj->GetCar(0);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//create door
		Door* door = 0;

		if (floorobj)
			door = floorobj->CreateDoor(tempdata[1], tempdata[2], tempdata[3], ToBool(tempdata[4]));
		else if (elevatorcarobj)
			door = elevatorcarobj->CreateDoor(tempdata[1], tempdata[2], tempdata[3], ToBool(tempdata[4]));
		else if (shaftobj)
			door = shaftobj->CreateDoor(tempdata[1], tempdata[2], tempdata[3], ToBool(tempdata[4]));
		else if (stairsobj)
			door = stairsobj->CreateDoor(tempdata[1], tempdata[2], tempdata[3], ToBool(tempdata[4]));
		else if (managerobj)
			door = managerobj->CreateDoor(tempdata[1], tempdata[2], tempdata[3], ToBool(tempdata[4]));
		else
			return ScriptError("Invalid object");

		StoreCommand(door);
		return sNextLine;
	}

	//CustomDoorComponent command
	if (StartsWithNoCase(LineData, "customdoorcomponent"))
	{
		//get data
		int params = SplitData(LineData, 20);

		if (params != 21)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 5; i <= 20; i++)
		{
			if (i == 6)
				i = 9;

			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::string name = tempdata[0];
		TrimString(name);
		Object *obj = Simcore->GetObject(name);

		if (!obj)
			return ScriptError("Invalid object " + name);

		Floor *floorobj = 0;
		Elevator *elevatorobj = 0;
		ElevatorCar *elevatorcarobj = 0;
		Shaft::Level *shaftobj = 0;
		Stairwell::Level *stairsobj = 0;
		DoorManager *managerobj = 0;

		//get parent object
		if (obj->GetType() == "Floor")
			floorobj = static_cast<Floor*>(obj);
		if (obj->GetType() == "Elevator")
			elevatorobj = static_cast<Elevator*>(obj);
		if (obj->GetType() == "ElevatorCar")
			elevatorcarobj = static_cast<ElevatorCar*>(obj);
		if (obj->GetType() == "Shaft Level")
			shaftobj = static_cast<Shaft::Level*>(obj);
		if (obj->GetType() == "Stairwell Level")
			stairsobj = static_cast<Stairwell::Level*>(obj);
		if (obj->GetType() == "DoorManager")
			managerobj = static_cast<DoorManager*>(obj);

		if (elevatorobj)
			elevatorcarobj = elevatorobj->GetCar(0);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//get door object
		Door *door = 0;
		if (floorobj)
			door = floorobj->GetDoor(tempdata[1]);
		if (elevatorcarobj)
			door = elevatorcarobj->GetDoor(tempdata[1]);
		if (shaftobj)
			door = shaftobj->GetDoor(tempdata[1]);
		if (stairsobj)
			door = stairsobj->GetDoor(tempdata[1]);
		if (managerobj)
			door = managerobj->GetDoor(tempdata[1]);

		if (!door)
			return ScriptError("Invalid door " + tempdata[1] + " in " + name);

		door->AddDoorComponent(tempdata[2], tempdata[3], tempdata[4], ToFloat(tempdata[5]), tempdata[6], tempdata[7], ToBool(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]), ToFloat(tempdata[12]), ToFloat(tempdata[13]), ToFloat(tempdata[14]), ToFloat(tempdata[15]), ToFloat(tempdata[16]), ToFloat(tempdata[17]), ToFloat(tempdata[18]), ToFloat(tempdata[19]), ToFloat(tempdata[20]));

		return sNextLine;
	}

	//FinishDoor command
	if (StartsWithNoCase(LineData, "finishdoor "))
	{
		//get data
		int params = SplitData(LineData, 11);

		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		std::string name = tempdata[0];
		TrimString(name);
		Object *obj = Simcore->GetObject(name);

		if (!obj)
			return ScriptError("Invalid object " + name);

		Floor *floorobj = 0;
		Elevator *elevatorobj = 0;
		ElevatorCar *elevatorcarobj = 0;
		Shaft::Level *shaftobj = 0;
		Stairwell::Level *stairsobj = 0;
		DoorManager *managerobj = 0;

		//get parent object
		if (obj->GetType() == "Floor")
			floorobj = static_cast<Floor*>(obj);
		if (obj->GetType() == "Elevator")
			elevatorobj = static_cast<Elevator*>(obj);
		if (obj->GetType() == "ElevatorCar")
			elevatorcarobj = static_cast<ElevatorCar*>(obj);
		if (obj->GetType() == "Shaft Level")
			shaftobj = static_cast<Shaft::Level*>(obj);
		if (obj->GetType() == "Stairwell Level")
			stairsobj = static_cast<Stairwell::Level*>(obj);
		if (obj->GetType() == "DoorManager")
			managerobj = static_cast<DoorManager*>(obj);

		if (elevatorobj)
			elevatorcarobj = elevatorobj->GetCar(0);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//get door object
		Door *door = 0;
		if (floorobj)
			door = floorobj->GetDoor(tempdata[1]);
		if (elevatorcarobj)
			door = elevatorcarobj->GetDoor(tempdata[1]);
		if (shaftobj)
			door = shaftobj->GetDoor(tempdata[1]);
		if (stairsobj)
			door = stairsobj->GetDoor(tempdata[1]);
		if (managerobj)
			door = managerobj->GetDoor(tempdata[1]);

		if (!door)
			return ScriptError("Invalid door " + tempdata[1] + " in " + name);

		//finish door
		door->FinishDoor(ToBool(tempdata[2]));

		door->SetLocked(config->lockvalue, config->keyvalue);

		return sNextLine;
	}

	//MoveDoor command
	if (StartsWithNoCase(LineData, "movedoor"))
	{
		//get data
		int params = SplitData(LineData, 9);

		if (params != 5)
			return ScriptError("Incorrect number of parameters");

		std::string name = tempdata[0];
		TrimString(name);
		Object *obj = Simcore->GetObject(name);

		if (!obj)
			return ScriptError("Invalid object " + name);

		Floor *floorobj = 0;
		Elevator *elevatorobj = 0;
		ElevatorCar *elevatorcarobj = 0;
		Shaft::Level *shaftobj = 0;
		Stairwell::Level *stairsobj = 0;
		DoorManager *managerobj = 0;

		//get parent object
		if (obj->GetType() == "Floor")
			floorobj = static_cast<Floor*>(obj);
		if (obj->GetType() == "Elevator")
			elevatorobj = static_cast<Elevator*>(obj);
		if (obj->GetType() == "ElevatorCar")
			elevatorcarobj = static_cast<ElevatorCar*>(obj);
		if (obj->GetType() == "Shaft Level")
			shaftobj = static_cast<Shaft::Level*>(obj);
		if (obj->GetType() == "Stairwell Level")
			stairsobj = static_cast<Stairwell::Level*>(obj);
		if (obj->GetType() == "DoorManager")
			managerobj = static_cast<DoorManager*>(obj);

		if (elevatorobj)
			elevatorcarobj = elevatorobj->GetCar(0);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//get door object
		Door *door = 0;
		if (floorobj)
			door = floorobj->GetDoor(tempdata[1]);
		if (elevatorcarobj)
			door = elevatorcarobj->GetDoor(tempdata[1]);
		if (shaftobj)
			door = shaftobj->GetDoor(tempdata[1]);
		if (stairsobj)
			door = stairsobj->GetDoor(tempdata[1]);
		if (managerobj)
			door = managerobj->GetDoor(tempdata[1]);

		if (!door)
			return ScriptError("Invalid door " + tempdata[1] + " in " + name);

		//move door
		door->Move(ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]));

		return sNextLine;
	}

	//SetAutoClose command
	if (StartsWithNoCase(LineData, "setautoclose"))
	{
		//get data
		int params = SplitData(LineData, 13);

		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		std::string name = tempdata[0];
		TrimString(name);
		Object *obj = Simcore->GetObject(name);

		if (!obj)
			return ScriptError("Invalid object " + name);

		Floor *floorobj = 0;
		Elevator *elevatorobj = 0;
		ElevatorCar *elevatorcarobj = 0;
		Shaft::Level *shaftobj = 0;
		Stairwell::Level *stairsobj = 0;
		DoorManager *managerobj = 0;

		//get parent object
		if (obj->GetType() == "Floor")
			floorobj = static_cast<Floor*>(obj);
		if (obj->GetType() == "Elevator")
			elevatorobj = static_cast<Elevator*>(obj);
		if (obj->GetType() == "ElevatorCar")
			elevatorcarobj = static_cast<ElevatorCar*>(obj);
		if (obj->GetType() == "Shaft Level")
			shaftobj = static_cast<Shaft::Level*>(obj);
		if (obj->GetType() == "Stairwell Level")
			stairsobj = static_cast<Stairwell::Level*>(obj);
		if (obj->GetType() == "DoorManager")
			managerobj = static_cast<DoorManager*>(obj);

		if (elevatorobj)
			elevatorcarobj = elevatorobj->GetCar(0);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//get door object
		Door *door = 0;
		if (floorobj)
			door = floorobj->GetDoor(tempdata[1]);
		if (elevatorcarobj)
			door = elevatorcarobj->GetDoor(tempdata[1]);
		if (shaftobj)
			door = shaftobj->GetDoor(tempdata[1]);
		if (stairsobj)
			door = stairsobj->GetDoor(tempdata[1]);
		if (managerobj)
			door = managerobj->GetDoor(tempdata[1]);

		if (!door)
			return ScriptError("Invalid door " + tempdata[1] + " in " + name);

		//set autoclose on a door
		door->AutoClose(ToInt(tempdata[2]));

		return sNextLine;
	}

	//Rotate command
	/*if (StartsWithNoCase(LineData, "rotate "))
	{
		//get data
		int params = SplitData(LineData, 7);

		if (params != 5)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 4; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::string name = tempdata[0];
		TrimString(name);
		Object *obj = Simcore->GetObject(name);

		if (!obj)
			return ScriptError("Invalid object " + name);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//rotate object
		obj->Rotate(ToFloat(tempdata[1]), ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]));

		return sNextLine;
	}

	//SetRotation command
	if (StartsWithNoCase(LineData, "setrotation"))
	{
		//get data
		int params = SplitData(LineData, 12);

		if (params != 4)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 3; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::string name = tempdata[0];
		TrimString(name);
		Object *obj = Simcore->GetObject(name);

		if (!obj)
			return ScriptError("Invalid object " + name);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//rotate object
		obj->SetRotation(ToFloat(tempdata[1]), ToFloat(tempdata[2]), ToFloat(tempdata[3]));

		return sNextLine;
	}

	//Move command
	if (StartsWithNoCase(LineData, "move "))
	{
		//get data
		int params = SplitData(LineData, 5);

		if (params != 5)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 4; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::string name = tempdata[0];
		TrimString(name);
		Object *obj = Simcore->GetObject(name);

		if (!obj)
			return ScriptError("Invalid object " + name);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//rotate object
		obj->Move(ToFloat(tempdata[1]), ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToFloat(tempdata[4]));

		return sNextLine;
	}

	//SetPosition command
	if (StartsWithNoCase(LineData, "setposition " && config->SectionNum != 9)
	{
		//get data
		int params = SplitData(LineData, 12);

		if (params != 4)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 3; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::string name = tempdata[0];
		TrimString(name);
		Object *obj = Simcore->GetObject(name);

		if (!obj)
			return ScriptError("Invalid object " + name);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//rotate object
		obj->SetPosition(ToFloat(tempdata[1]), ToFloat(tempdata[2]), ToFloat(tempdata[3]));

		return sNextLine;
	}

	//SetPositionY command
	if (StartsWithNoCase(LineData, "setpositiony"))
	{
		//get data
		int params = SplitData(LineData, 13);

		if (params != 2)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		if (!IsNumeric(tempdata[1]))
			return ScriptError("Invalid value: " + tempdata[1]);

		std::string name = tempdata[0];
		TrimString(name);
		Object *obj = Simcore->GetObject(name);

		if (!obj)
			return ScriptError("Invalid object " + name);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//rotate object
		obj->SetPositionY(ToFloat(tempdata[1]));

		return sNextLine;
	}

	//SetPositionRelative command
	if (linecheck.substr(0, 19) == "setpositionrelative"))
	{
		//get data
		int params = SplitData(LineData, 20);

		if (params != 4)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 3; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::string name = tempdata[0];
		TrimString(name);
		Object *obj = Simcore->GetObject(name);

		if (!obj)
			return ScriptError("Invalid object " + name);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		//rotate object
		obj->SetPositionRelative(ToFloat(tempdata[1]), ToFloat(tempdata[2]), ToFloat(tempdata[3]));

		return sNextLine;
	}*/

	//CreateWallObject command
	if (StartsWithNoCase(LineData, "createwallobject"))
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
	if (StartsWithNoCase(LineData, "setkey"))
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
	if (StartsWithNoCase(LineData, "setlock"))
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
	if (StartsWithNoCase(LineData, "delete"))
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
	if (StartsWithNoCase(LineData, "runaction"))
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
	if (StartsWithNoCase(LineData, "teleport"))
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
	if (StartsWithNoCase(LineData, "gotofloor"))
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
	if (StartsWithNoCase(LineData, "floorinfo"))
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
	if (StartsWithNoCase(LineData, "listtextures"))
	{
		Simcore->Report(texturemanager->ListTextures(true));
		return sNextLine;
	}

	//ListVisibleMeshes command
	if (StartsWithNoCase(LineData, "listvisiblemeshes"))
	{
		Simcore->ListVisibleMeshes();
		return sNextLine;
	}

	//ShowLoadedSounds command
	if (StartsWithNoCase(LineData, "showloadedsounds"))
	{
		if (Simcore->GetSoundSystem())
			Simcore->GetSoundSystem()->ShowLoadedSounds();
		return sNextLine;
	}

	//ShowPlayingSounds command
	if (StartsWithNoCase(LineData, "showplayingsounds"))
	{
		if (Simcore->GetSoundSystem())
			Simcore->GetSoundSystem()->ShowPlayingSounds();
		return sNextLine;
	}

	//Print command
	if (StartsWithNoCase(LineData, "print"))
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
