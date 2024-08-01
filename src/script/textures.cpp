/*
	Skyscraper 2.1 - Script Processor - Textures Section
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
#include "texture.h"
#include "scriptproc.h"
#include "section.h"

using namespace SBS;

namespace Skyscraper {

ScriptProcessor::TexturesSection::TexturesSection(ScriptProcessor *parent) : Section(parent)
{

}

int ScriptProcessor::TexturesSection::Run(std::string &LineData)
{
	//Process Textures

	TextureManager *texturemanager = Simcore->GetTextureManager();

	//Load command
	if (StartsWithNoCase(LineData, "load "))
	{
		//get data
		int params = SplitData(LineData, 5, false);

		if (params != 4 && params != 5)
			return ScriptError("Incorrect number of )parameters");

		//check numeric values
		for (int i = 2; i <= 3; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}
		parent->CheckFile(tempdata[0]);
		if (params == 4)
			texturemanager->LoadTexture(tempdata[0], tempdata[1], ToFloat(tempdata[2]), ToFloat(tempdata[3]));
		else
			texturemanager->LoadTexture(tempdata[0], tempdata[1], ToFloat(tempdata[2]), ToFloat(tempdata[3]), true, ToBool(tempdata[4]));
		return sNextLine;
	}

	//LoadAnimated command
	if (StartsWithNoCase(LineData, "loadanimated"))
	{
		//get data
		int params = SplitData(LineData, 12, false);

		if (params < 7)
			return ScriptError("Incorrect number of parameters");

		bool force;
		if (IsNumeric(tempdata[params - 1]) == true && IsNumeric(tempdata[params - 2]) == true)
			force = false;
		else
			force = true;

		//check numeric values
		if (force == true)
		{
			for (int i = (params - 4); i <= (params - 2); i++)
			{
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}
		else
		{
			for (int i = (params - 3); i <= (params - 1); i++)
			{
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		std::vector<std::string> filenames;
		if (force == true)
		{
			for (int i = 0; i < params - 6; i++)
				filenames.push_back(tempdata[i]);
		}
		else
		{
			for (int i = 0; i < params - 5; i++)
				filenames.push_back(tempdata[i]);
		}

		//check existence of files
		for (size_t i = 0; i < filenames.size(); i++)
			parent->CheckFile(filenames[i]);

		if (force == false)
			texturemanager->LoadAnimatedTexture(filenames, tempdata[params - 4], ToFloat(tempdata[params - 3]), ToFloat(tempdata[params - 2]), ToFloat(tempdata[params - 1]));
		else
			texturemanager->LoadAnimatedTexture(filenames, tempdata[params - 5], ToFloat(tempdata[params - 4]), ToFloat(tempdata[params - 3]), ToFloat(tempdata[params - 2]), true, ToBool(tempdata[params - 1]));
		return sNextLine;
	}

	//LoadAlphaBlend command
	if (StartsWithNoCase(LineData, "loadalphablend"))
	{
		//get data
		int params = SplitData(LineData, 14, false);

		if (params != 7 && params != 8)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 5; i <= 6; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//check existence of files
		parent->CheckFile(tempdata[0]);
		parent->CheckFile(tempdata[1]);
		parent->CheckFile(tempdata[2]);

		if (params == 7)
			texturemanager->LoadAlphaBlendTexture(tempdata[0], tempdata[1], tempdata[2], tempdata[3], ToBool(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]));
		else
			texturemanager->LoadAlphaBlendTexture(tempdata[0], tempdata[1], tempdata[2], tempdata[3], ToBool(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), true, ToBool(tempdata[7]));
		return sNextLine;
	}

	//LoadMaterial command
	if (StartsWithNoCase(LineData, "loadmaterial"))
	{
		//get data
		int params = SplitData(LineData, 12, false);

		if (params != 4 && params != 5)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 3; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}
		if (params == 4)
			texturemanager->LoadMaterial(tempdata[0], tempdata[1], ToFloat(tempdata[2]), ToFloat(tempdata[3]));
		else
			texturemanager->LoadMaterial(tempdata[0], tempdata[1], ToFloat(tempdata[2]), ToFloat(tempdata[3]), true, ToBool(tempdata[4]));
		return sNextLine;
	}

	//LoadRange command
	if (StartsWithNoCase(LineData, "loadrange"))
	{
		//get data
		int params = SplitData(LineData, 9, false);

		if (params != 6 && params != 7)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 5; i++)
		{
			if (i == 2)
				i = 4;
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}
		int RangeL = ToInt(tempdata[0]);
		int RangeH = ToInt(tempdata[1]);
		std::string filename, buffer, name;
		for (int Current = RangeL; Current <= RangeH; Current++)
		{
			filename = tempdata[2];
			buffer = ToString(Current);
			TrimString(buffer);
			ReplaceAll(filename, "%number%", buffer);
			name = tempdata[3];
			ReplaceAll(name, "%number%", buffer);
			parent->CheckFile(filename);
			if (params == 6)
				texturemanager->LoadTexture(filename, name, ToFloat(tempdata[4]), ToFloat(tempdata[5]));
			else
				texturemanager->LoadTexture(filename, name, ToFloat(tempdata[4]), ToFloat(tempdata[5]), true, ToBool(tempdata[6]));
		}
		return sNextLine;
	}

	//AddText command
	if (StartsWithNoCase(LineData, "addtext "))
	{
		//get data
		int params = SplitData(LineData, 8, false);

		if (params != 14 && params != 15)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 13; i++)
		{
			if (i == 4)
				i = 5;
			if (i == 9)
				i = 11;

			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}
		std::string filename = tempdata[2];
		TrimString(filename);
		filename.insert(0, "data/fonts/");
		parent->CheckFile(filename);
		if (params == 14)
			texturemanager->AddTextToTexture(tempdata[0], tempdata[1], filename, ToFloat(tempdata[3]), tempdata[4], ToInt(tempdata[5]), ToInt(tempdata[6]), ToInt(tempdata[7]), ToInt(tempdata[8]), tempdata[9], tempdata[10], ToInt(tempdata[11]), ToInt(tempdata[12]), ToInt(tempdata[13]));
		else
			texturemanager->AddTextToTexture(tempdata[0], tempdata[1], filename, ToFloat(tempdata[3]), tempdata[4], ToInt(tempdata[5]), ToInt(tempdata[6]), ToInt(tempdata[7]), ToInt(tempdata[8]), tempdata[9], tempdata[10], ToInt(tempdata[11]), ToInt(tempdata[12]), ToInt(tempdata[13]), true, ToBool(tempdata[14]));
		return sNextLine;
	}

	//AddTextRange command
	if (StartsWithNoCase(LineData, "addtextrange"))
	{
		//get data
		int params = SplitData(LineData, 13, false);

		if (params != 16 && params != 17)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 15; i++)
		{
			if (i == 2)
				i = 5;
			if (i == 6)
				i = 7;
			if (i == 11)
				i = 13;

			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}
		int RangeL = ToInt(tempdata[0]);
		int RangeH = ToInt(tempdata[1]);
		std::string orig_line = LineData;
		for (int Current = RangeL; Current <= RangeH; Current++)
		{
			std::string num = ToString(Current);
			TrimString(num);
			LineData = orig_line;
			ReplaceAll(LineData, "%number%", num);

			//get data
			int params = SplitData(LineData, 13, false);

			std::string filename = tempdata[4];
			TrimString(filename);
			filename.insert(0, "data/fonts/");
			parent->CheckFile(filename);
			if (params == 16)
				texturemanager->AddTextToTexture(tempdata[2], tempdata[3], filename, ToFloat(tempdata[5]), tempdata[6], ToInt(tempdata[7]), ToInt(tempdata[8]), ToInt(tempdata[9]), ToInt(tempdata[10]), tempdata[11], tempdata[12], ToInt(tempdata[13]), ToInt(tempdata[14]), ToInt(tempdata[15]));
			else
				texturemanager->AddTextToTexture(tempdata[2], tempdata[3], filename, ToFloat(tempdata[5]), tempdata[6], ToInt(tempdata[7]), ToInt(tempdata[8]), ToInt(tempdata[9]), ToInt(tempdata[10]), tempdata[11], tempdata[12], ToInt(tempdata[13]), ToInt(tempdata[14]), ToInt(tempdata[15]), true, ToBool(tempdata[16]));
		}
		return sNextLine;
	}

	//LoadCropped command
	if (StartsWithNoCase(LineData, "loadcropped"))
	{
		//get data
		int params = SplitData(LineData, 12, false);

		if (params != 8 && params != 9)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 7; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}
		parent->CheckFile(tempdata[0]);
		if (params == 8)
			texturemanager->LoadTextureCropped(tempdata[0], tempdata[1], ToInt(tempdata[2]), ToInt(tempdata[3]), ToInt(tempdata[4]), ToInt(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]));
		else
			texturemanager->LoadTextureCropped(tempdata[0], tempdata[1], ToInt(tempdata[2]), ToInt(tempdata[3]), ToInt(tempdata[4]), ToInt(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToBool(tempdata[8]));
		return sNextLine;
	}

	//AddOverlay command
	if (StartsWithNoCase(LineData, "addoverlay"))
	{
		//get data
		int params = SplitData(LineData, 11, false);

		if (params != 9 && params != 10)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 8; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}
		if (params == 9)
			texturemanager->AddTextureOverlay(tempdata[0], tempdata[1], tempdata[2], ToInt(tempdata[3]), ToInt(tempdata[4]), ToInt(tempdata[5]), ToInt(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]));
		else
			texturemanager->AddTextureOverlay(tempdata[0], tempdata[1], tempdata[2], ToInt(tempdata[3]), ToInt(tempdata[4]), ToInt(tempdata[5]), ToInt(tempdata[6]), ToFloat(tempdata[7]), ToFloat(tempdata[8]), true, ToBool(tempdata[9]));
		return sNextLine;
	}

	//SetLighting command
	if (StartsWithNoCase(LineData, "setlighting"))
	{
		//get data
		int params = SplitData(LineData, 12, false);

		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 2; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		Simcore->SetLighting(ToFloat(tempdata[0]), ToFloat(tempdata[1]), ToFloat(tempdata[2]));
		return sNextLine;
	}

	//Rotate command
	if (StartsWithNoCase(LineData, "rotate "))
	{
		//get data
		int params = SplitData(LineData, 7, false);

		if (params != 2)
			return ScriptError("Incorrect number of parameters");

		//check numeric value
		if (!IsNumeric(tempdata[1]))
			return ScriptError("Invalid value: " + tempdata[1]);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		texturemanager->RotateTexture(tempdata[0], ToFloat(tempdata[1]));
		return sNextLine;
	}

	//RotateAnim command
	if (StartsWithNoCase(LineData, "rotateanim"))
	{
		//get data
		int params = SplitData(LineData, 10, false);

		if (params != 2)
			return ScriptError("Incorrect number of parameters");

		//check numeric value
		if (!IsNumeric(tempdata[1]))
			return ScriptError("Invalid value: " + tempdata[1]);

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		texturemanager->RotateAnimTexture(tempdata[0], ToFloat(tempdata[1]));
		return sNextLine;
	}

	//Scroll command
	if (StartsWithNoCase(LineData, "scroll "))
	{
		//get data
		int params = SplitData(LineData, 7, false);

		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 2; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		texturemanager->ScrollTexture(tempdata[0], ToFloat(tempdata[1]), ToFloat(tempdata[2]));
		return sNextLine;
	}

	//ScrollAnim command
	if (StartsWithNoCase(LineData, "scrollanim"))
	{
		//get data
		int params = SplitData(LineData, 10, false);

		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 2; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		texturemanager->ScrollAnimTexture(tempdata[0], ToFloat(tempdata[1]), ToFloat(tempdata[2]));
		return sNextLine;
	}

	//Scale command
	if (StartsWithNoCase(LineData, "scale"))
	{
		//get data
		int params = SplitData(LineData, 5, false);

		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 2; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		texturemanager->ScaleTexture(tempdata[0], ToFloat(tempdata[1]), ToFloat(tempdata[2]));
		return sNextLine;
	}

	//Transform command
	if (StartsWithNoCase(LineData, "transform"))
	{
		//get data
		int params = SplitData(LineData, 9, false);

		if (params != 7)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 6; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//stop here if in Check mode
		if (config->CheckScript == true)
			return sNextLine;

		texturemanager->TransformTexture(tempdata[0], tempdata[1], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]));
		return sNextLine;
	}

	//handle end of textures section
	if (StartsWithNoCase(LineData, "<endtextures>"))
	{
		Simcore->GetTextureManager()->FreeTextureImages();
		config->SectionNum = 0;
		config->Context = "None";
		engine->Report("Finished textures");
		return sNextLine;
	}

	return sContinue;
}

}
