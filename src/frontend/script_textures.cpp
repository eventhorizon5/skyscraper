/* $Id$ */

/*
	Skyscraper 1.10 Alpha - Script Processor - Textures Section
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
#include "texture.h"
#include "scriptprocessor.h"

using namespace SBS;

namespace Skyscraper {

int ScriptProcessor::ProcTextures()
{
	//Process Textures

	//create a lowercase string of the line
	std::string linecheck = SetCaseCopy(LineData, false);

	TextureManager *texturemanager = Simcore->GetTextureManager();

	//Load command
	if (linecheck.substr(0, 5) == "load ")
	{
		//get data
		int params = SplitData(LineData, 5, false);

		if (params != 4 && params != 5)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 3; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}
		CheckFile(tempdata[0]);
		if (params == 4)
			texturemanager->LoadTexture(tempdata[0], tempdata[1], ToFloat(tempdata[2]), ToFloat(tempdata[3]));
		else
			texturemanager->LoadTexture(tempdata[0], tempdata[1], ToFloat(tempdata[2]), ToFloat(tempdata[3]), true, ToBool(tempdata[4]));
		return sNextLine;
	}

	//LoadAnimated command
	if (linecheck.substr(0, 12) == "loadanimated")
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
			CheckFile(filenames[i]);

		if (force == false)
			texturemanager->LoadAnimatedTexture(filenames, tempdata[params - 4], ToFloat(tempdata[params - 3]), ToFloat(tempdata[params - 2]), ToFloat(tempdata[params - 1]));
		else
			texturemanager->LoadAnimatedTexture(filenames, tempdata[params - 5], ToFloat(tempdata[params - 4]), ToFloat(tempdata[params - 3]), ToFloat(tempdata[params - 2]), true, ToBool(tempdata[params - 1]));
		return sNextLine;
	}

	//LoadAlphaBlend command
	if (linecheck.substr(0, 14) == "loadalphablend")
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
		CheckFile(tempdata[0]);
		CheckFile(tempdata[1]);
		CheckFile(tempdata[2]);

		if (params == 7)
			texturemanager->LoadAlphaBlendTexture(tempdata[0], tempdata[1], tempdata[2], tempdata[3], ToBool(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]));
		else
			texturemanager->LoadAlphaBlendTexture(tempdata[0], tempdata[1], tempdata[2], tempdata[3], ToBool(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]), true, ToBool(tempdata[7]));
		return sNextLine;
	}

	//LoadMaterial command
	if (linecheck.substr(0, 12) == "loadmaterial")
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
	if (linecheck.substr(0, 9) == "loadrange")
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
		RangeL = ToInt(tempdata[0]);
		RangeH = ToInt(tempdata[1]);
		for (Current = RangeL; Current <= RangeH; Current++)
		{
			temp2 = tempdata[2];
			buffer = ToString(Current);
			TrimString(buffer);
			ReplaceAll(temp2, "%number%", buffer);
			temp6 = tempdata[3];
			ReplaceAll(temp6, "%number%", buffer);
			CheckFile(temp2);
			if (params == 6)
				texturemanager->LoadTexture(temp2, temp6, ToFloat(tempdata[4]), ToFloat(tempdata[5]));
			else
				texturemanager->LoadTexture(temp2, temp6, ToFloat(tempdata[4]), ToFloat(tempdata[5]), true, ToBool(tempdata[6]));
		}
		return sNextLine;
	}

	//AddText command
	if (linecheck.substr(0, 8) == "addtext ")
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
		buffer = tempdata[2];
		TrimString(buffer);
		buffer.insert(0, "data/fonts/");
		CheckFile(buffer);
		if (params == 14)
			texturemanager->AddTextToTexture(tempdata[0], tempdata[1], buffer, ToFloat(tempdata[3]), tempdata[4], ToInt(tempdata[5]), ToInt(tempdata[6]), ToInt(tempdata[7]), ToInt(tempdata[8]), tempdata[9], tempdata[10], ToInt(tempdata[11]), ToInt(tempdata[12]), ToInt(tempdata[13]));
		else
			texturemanager->AddTextToTexture(tempdata[0], tempdata[1], buffer, ToFloat(tempdata[3]), tempdata[4], ToInt(tempdata[5]), ToInt(tempdata[6]), ToInt(tempdata[7]), ToInt(tempdata[8]), tempdata[9], tempdata[10], ToInt(tempdata[11]), ToInt(tempdata[12]), ToInt(tempdata[13]), true, ToBool(tempdata[14]));
		return sNextLine;
	}

	//AddTextRange command
	if (linecheck.substr(0, 12) == "addtextrange")
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
		RangeL = ToInt(tempdata[0]);
		RangeH = ToInt(tempdata[1]);
		temp6 = LineData;
		for (Current = RangeL; Current <= RangeH; Current++)
		{
			buffer = ToString(Current);
			TrimString(buffer);
			LineData = temp6;
			ReplaceAll(LineData, "%number%", buffer);

			//get data
			int params = SplitData(LineData, 13, false);

			buffer = tempdata[4];
			TrimString(buffer);
			buffer.insert(0, "data/fonts/");
			CheckFile(buffer);
			if (params == 16)
				texturemanager->AddTextToTexture(tempdata[2], tempdata[3], buffer, ToFloat(tempdata[5]), tempdata[6], ToInt(tempdata[7]), ToInt(tempdata[8]), ToInt(tempdata[9]), ToInt(tempdata[10]), tempdata[11], tempdata[12], ToInt(tempdata[13]), ToInt(tempdata[14]), ToInt(tempdata[15]));
			else
				texturemanager->AddTextToTexture(tempdata[2], tempdata[3], buffer, ToFloat(tempdata[5]), tempdata[6], ToInt(tempdata[7]), ToInt(tempdata[8]), ToInt(tempdata[9]), ToInt(tempdata[10]), tempdata[11], tempdata[12], ToInt(tempdata[13]), ToInt(tempdata[14]), ToInt(tempdata[15]), true, ToBool(tempdata[16]));
		}
		linecheck = SetCaseCopy(LineData, false);
		return sNextLine;
	}

	//LoadCropped command
	if (linecheck.substr(0, 11) == "loadcropped")
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
		CheckFile(tempdata[0]);
		if (params == 8)
			texturemanager->LoadTextureCropped(tempdata[0], tempdata[1], ToInt(tempdata[2]), ToInt(tempdata[3]), ToInt(tempdata[4]), ToInt(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]));
		else
			texturemanager->LoadTextureCropped(tempdata[0], tempdata[1], ToInt(tempdata[2]), ToInt(tempdata[3]), ToInt(tempdata[4]), ToInt(tempdata[5]), ToFloat(tempdata[6]), ToFloat(tempdata[7]), ToBool(tempdata[8]));
		return sNextLine;
	}

	//AddOverlay command
	if (linecheck.substr(0, 10) == "addoverlay")
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
	if (linecheck.substr(0, 11) == "setlighting")
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
		Simcore->SetLighting(ToFloat(tempdata[0]), ToFloat(tempdata[1]), ToFloat(tempdata[2]));
		return sNextLine;
	}

	//Rotate command
	if (linecheck.substr(0, 7) == "rotate ")
	{
		//get data
		int params = SplitData(LineData, 7, false);

		if (params != 2)
			return ScriptError("Incorrect number of parameters");

		//check numeric value
		if (!IsNumeric(tempdata[1]))
			return ScriptError("Invalid value: " + tempdata[1]);

		texturemanager->RotateTexture(tempdata[0], ToFloat(tempdata[1]));
		return sNextLine;
	}

	//RotateAnim command
	if (linecheck.substr(0, 10) == "rotateanim")
	{
		//get data
		int params = SplitData(LineData, 10, false);

		if (params != 2)
			return ScriptError("Incorrect number of parameters");

		//check numeric value
		if (!IsNumeric(tempdata[1]))
			return ScriptError("Invalid value: " + tempdata[1]);

		texturemanager->RotateAnimTexture(tempdata[0], ToFloat(tempdata[1]));
		return sNextLine;
	}

	//Scroll command
	if (linecheck.substr(0, 7) == "scroll ")
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

		texturemanager->ScrollTexture(tempdata[0], ToFloat(tempdata[1]), ToFloat(tempdata[2]));
		return sNextLine;
	}

	//ScrollAnim command
	if (linecheck.substr(0, 10) == "scrollanim")
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

		texturemanager->ScrollAnimTexture(tempdata[0], ToFloat(tempdata[1]), ToFloat(tempdata[2]));
		return sNextLine;
	}

	//Scale command
	if (linecheck.substr(0, 5) == "scale")
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

		texturemanager->ScaleTexture(tempdata[0], ToFloat(tempdata[1]), ToFloat(tempdata[2]));
		return sNextLine;
	}

	//Transform command
	if (linecheck.substr(0, 9) == "transform")
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

		texturemanager->TransformTexture(tempdata[0], tempdata[1], tempdata[2], ToFloat(tempdata[3]), ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]));
		return sNextLine;
	}

	return sContinue;
}

}
