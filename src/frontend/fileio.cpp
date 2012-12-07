/* $Id$ */

/*
	Skyscraper 1.8 Alpha - File I/O and Script Processing Routines
	Copyright (C)2003-2012 Ryan Thoryk
	http://www.skyscrapersim.com
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@tliquest.net

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

#include <wx/app.h>
#include <wx/variant.h>
#include <wx/msgdlg.h>
#include "globals.h"
#include "sbs.h"
#include <OgreFileSystem.h>
#include <stdlib.h>
#include "fileio.h"
#include "skyscraper.h"
#include "camera.h"
#include "floor.h"
#include "elevator.h"
#include "shaft.h"
#include "callbutton.h"
#include "unix.h"
#include "textwindow.h"

extern SBS *Simcore;
extern Skyscraper *skyscraper;

#define sContinue 0
#define sNextLine 1
#define sError 2
#define sCheckFloors 3
#define sBreak 4
#define sRecalc 5

ScriptProcessor::ScriptProcessor()
{
	//set variable array size
	UserVariable.resize(256);
}

ScriptProcessor::~ScriptProcessor()
{

}

bool ScriptProcessor::LoadBuilding()
{
	//building loader/script interpreter

	line = 0; //line number
	LineData = "";  //line contents
	Current = 0;
	Section = 0;
	Context = "None";
	temp1 = 0;
	temp2 = "";
	temp3 = 0;
	temp4 = 0;
	temp5 = 0;
	temp6 = "";
	FloorCheck = 0;
	RangeL = 0;
	RangeH = 0;
	RangeStart = 0;
	startpos = 0;
	getfloordata = false;
	setshaftdoors = false;
	int returncode = 0;
	MinExtent = 0;
	MaxExtent = 0;
	char buffer2[20];
	InFunction = false;
	FunctionCallLine = 0;
	ReplaceLine = false;
	nonexistent_files.clear();

	while (line < (int)BuildingData.size())
	{
		if (Simcore->GetFloor(0) == 0)
			TrimString(LineData);

		LineData = BuildingData[line];
		TrimString(LineData);

		if (ReplaceLine == true)
		{
			LineData = FunctionCallLineData;
			ReplaceLine = false;
		}

		//skip blank lines
		if (LineData == "")
			goto Nextline;

		//process comment markers
		temp1 = LineData.find("#", 0);
		if (temp1 > -1)
			LineData.erase(temp1);

		//skip blank lines
		if (LineData == "")
			goto Nextline;

		//function parameter variables
		if (InFunction == true)
		{
			for (int i = 0; i < (int)FunctionParams.size(); i++)
			{
				std::string num = _itoa(i + 1, intbuffer, 10);
				ReplaceAll(LineData, std::string("%param" + num + "%").c_str(), FunctionParams[i].c_str());
			}
		}

		//////////////////////
		//Section information
		//////////////////////
		if (SetCaseCopy(LineData, false) == "<globals>")
		{
			if (Section > 0)
			{
				ScriptError("Already within a section");
				return false;
			}
			Section = 1;
			Context = "Globals";
			skyscraper->Report("Processing globals...");
			goto Nextline;
		}
		if (SetCaseCopy(LineData, false) == "<endglobals>")
		{
			if (Section != 1)
			{
				ScriptError("Not in global section");
				return false;
			}
			Simcore->InitMeshes();
			Section = 0;
			Context = "None";
			skyscraper->Report("Finished globals");
			goto Nextline;
		}
		if (SetCaseCopy(LineData.substr(0, 7), false) == "<floors")
		{
			if (Section > 0)
			{
				ScriptError("Already within a section");
				return false;
			}
			Section = 2;
			temp3 = SetCaseCopy(LineData, false).find("to", 0);
			if (temp3 < 0)
			{
				ScriptError("Syntax error");
				return false;

			}
			//get low and high range markers
			std::string str1 = LineData.substr(8, temp3 - 9);
			std::string str2 = LineData.substr(temp3 + 2, LineData.length() - (temp3 + 2) - 1);
			TrimString(str1);
			TrimString(str2);
			if (!IsNumeric(str1.c_str(), RangeL) || !IsNumeric(str2.c_str(), RangeH))
			{
				ScriptError("Invalid range");
				return false;
			}
			Context = "Floor range " + std::string(_itoa(RangeL, intbuffer, 10)) + " to " + std::string(_itoa(RangeH, intbuffer, 10));
			Current = RangeL;
			RangeStart = line;
			skyscraper->Report("Processing floors " + std::string(_itoa(RangeL, intbuffer, 10)) + " to " + std::string(_itoa(RangeH, intbuffer, 10)) + "...");
			goto Nextline;
		}
		if (SetCaseCopy(LineData.substr(0, 7), false) == "<floor ")
		{
			if (Section > 0)
			{
				ScriptError("Already within a section");
				return false;
			}
			Section = 2;
			RangeL = 0;
			RangeH = 0;
			std::string str = LineData.substr(7, LineData.length() - 8);
			TrimString(str);
			if (!IsNumeric(str.c_str(), Current))
			{
				ScriptError("Invalid floor");
				return false;
			}
			Context = "Floor " + std::string(_itoa(Current, intbuffer, 10));
			skyscraper->Report("Processing floor " + std::string(_itoa(Current, intbuffer, 10)) + "...");
			goto Nextline;
		}
		if (SetCaseCopy(LineData, false) == "<endfloor>" && RangeL == RangeH)
		{
			if (Section != 2)
			{
				ScriptError("Not in floor section");
				return false;
			}
			Section = 0;
			Context = "None";
			skyscraper->Report("Finished floor");
			goto Nextline;
		}
		if (SetCaseCopy(LineData.substr(0, 10), false) == "<elevators")
		{
			if (Section > 0)
			{
				ScriptError("Already within a section");
				return false;
			}
			Section = 4;
			temp3 = SetCaseCopy(LineData, false).find("to", 10);
			if (temp3 < 0)
			{
				ScriptError("Syntax error");
				return false;
			}
			std::string str1 = LineData.substr(11, temp3 - 12);
			std::string str2 = LineData.substr(temp3 + 2, LineData.length() - (temp3 + 2) - 1);
			TrimString(str1);
			TrimString(str2);
			if (!IsNumeric(str1.c_str(), RangeL) || !IsNumeric(str2.c_str(), RangeH))
			{
				ScriptError("Invalid range");
				return false;
			}
			Context = "Elevator range " + std::string(_itoa(RangeL, intbuffer, 10)) + " to " + std::string(_itoa(RangeH, intbuffer, 10));
			Current = RangeL;
			RangeStart = line;
			skyscraper->Report("Processing elevators " + std::string(_itoa(RangeL, intbuffer, 10)) + " to " + std::string(_itoa(RangeH, intbuffer, 10)) + "...");
			goto Nextline;
		}
		if (SetCaseCopy(LineData.substr(0, 10), false) == "<elevator ")
		{
			if (Section > 0)
			{
				ScriptError("Already within a section");
				return false;
			}
			Section = 4;
			RangeL = 0;
			RangeH = 0;
			std::string str = LineData.substr(10, LineData.length() - 11);
			TrimString(str);
			if (!IsNumeric(str.c_str(), Current))
			{
				ScriptError("Invalid elevator");
				return false;
			}
			if (Current < 1 || Current > Simcore->Elevators() + 1)
			{
				ScriptError("Invalid elevator");
				return false;
			}
			Context = "Elevator " + std::string(_itoa(Current, intbuffer, 10));
			skyscraper->Report("Processing elevator " + std::string(_itoa(Current, intbuffer, 10)) + "...");
			goto Nextline;
		}
		if (SetCaseCopy(LineData, false) == "<endelevator>" && RangeL == RangeH)
		{
			if (Section != 4)
			{
				ScriptError("Not in elevator section");
				return false;
			}
			Section = 0;
			Context = "None";
			skyscraper->Report("Finished elevator");
			goto Nextline;
		}
		if (SetCaseCopy(LineData.substr(0, 10), false) == "<textures>")
		{
			if (Section > 0)
			{
				ScriptError("Already within a section");
				return false;
			}
			Section = 5;
			Context = "Textures";
			skyscraper->Report("Processing textures...");
			goto Nextline;
		}
		if (SetCaseCopy(LineData.substr(0, 13), false) == "<endtextures>")
		{
			if (Section != 5)
			{
				ScriptError("Not in texture section");
				return false;
			}
			Simcore->FreeTextureImages();
			Section = 0;
			Context = "None";
			skyscraper->Report("Finished textures");
			goto Nextline;
		}
		if (SetCaseCopy(LineData.substr(0, 5), false) == "<end>")
		{
			Section = 0;
			Context = "None";
			skyscraper->Report("Exiting building script");
			break; //exit data file parser
		}
		if (SetCaseCopy(LineData.substr(0, 7), false) == "<break>")
		{
			//breakpoint function for debugging scripts
breakpoint:
			skyscraper->Report("Script breakpoint reached");
			goto Nextline;
		}
		if (SetCaseCopy(LineData.substr(0, 8), false) == "<include")
		{
			//include another file at the current script location

			int endloc = LineData.find(">");
			std::string includefile = LineData.substr(9, endloc - 9);
			TrimString(includefile);

			//delete current line
			BuildingData.erase(BuildingData.begin() + line);

			//insert file at current line
			bool result = LoadDataFile(includefile.c_str(), true, line);
			if (result == false)
			{
				ScriptError("File not found");
				return false;
			}

			skyscraper->Report("Inserted file " + includefile);
			line--;
			goto Nextline;
		}
		if (SetCaseCopy(LineData.substr(0, 9), false) == "<function" && Section == 0)
		{
			//define a function (only available outside sections)

			int endloc = LineData.find(">");
			std::string function = LineData.substr(10, endloc - 10);
			TrimString(function);

			//store function info in array
			FunctionInfo info;
			info.name = function;
			info.line = line;
			functions.push_back(info);

			//skip to end of function
			for (int i = line + 1; i < (int)BuildingData.size(); i++)
			{
				if (SetCaseCopy(BuildingData[i].substr(0, 13), false) == "<endfunction>")
				{
					line = i;
					break;
				}
			}

			skyscraper->Report("Defined function " + function);
			goto Nextline;
		}
		if (SetCaseCopy(LineData.substr(0, 13), false) == "<endfunction>" && InFunction == true)
		{
			//end function and return to original line
			InFunction = false;
			FunctionParams.clear();
			ReplaceLine = true;
			line = FunctionCallLine - 1;
			goto Nextline;
		}

		startpos = 0;
		do
		{
			//User variable conversion
			temp1 = LineData.find("%", startpos);
			if (temp1 >= startpos)
			{
				temp3 = LineData.find("%", temp1 + 1);
				if (temp3 >= (int)LineData.length())
				{
					temp1 = 0;
					temp3 = 0;
					break;
				}
			}
			else
			{
				//none (or no more) variables found
				temp1 = 0;
				temp3 = 0;
				break;
			}

			if (temp1 + temp3 > 0)
			{
				std::string str = LineData.substr(temp1 + 1, temp3 - temp1 - 1);
				TrimString(str);
				temp2 = str;
				if (IsNumeric(temp2.c_str()) == true)
				{
					temp4 = atoi(temp2.c_str());
					if (temp4 < 0 || temp4 > (int)UserVariable.size() - 1)
					{
						ScriptError("Invalid variable number");
						return false;
					}
					//replace all occurrences of the variable with it's value
					ReplaceAll(LineData, std::string("%" + temp2 + "%").c_str(), UserVariable[temp4].c_str());
					startpos = temp1;
				}
				else
					startpos = temp3 + 1;
			}
			else
				startpos++;
		} while (1 == 1);

		//////////////////////////
		//Floor object conversion
		//////////////////////////
checkfloors:
		temp5 = SetCaseCopy(LineData, false).find("floor(", 0);
		while (temp5 > -1)
		{
			temp1 = LineData.find("(", temp5);
			temp3 = LineData.find(")", temp5);
			if (temp3 < 0)
			{
				ScriptError("Syntax error");
				return false;
			}
			if (Section == 2 && getfloordata == false)
			{
				//process floor-specific variables if in a floor section
				getfloordata = true;
				goto recalc;
			}
			else
				getfloordata = false;
			std::string tempdata = Calc(LineData.substr(temp1 + 1, temp3 - temp1 - 1).c_str());
			TrimString(tempdata);
			LineData = LineData.substr(0, temp1 + 1) + tempdata + LineData.substr(temp3);

			if (!IsNumeric(tempdata.c_str(), temp4))
			{
				ScriptError("Invalid floor " + tempdata);
				return false;
			}
			if (Simcore->IsValidFloor(temp4) == false)
			{
				ScriptError("Invalid floor " + tempdata);
				return false;
			}

			//fullheight parameter
			buffer = Ogre::StringConverter::toString(temp4);
			TrimString(buffer);
			temp6 = "floor(" + buffer + ").fullheight";
			buffer = LineData;
			SetCase(buffer, false);
			temp1 = buffer.find(temp6, 0);
			if (temp1 > 0)
			{
				buffer = Ogre::StringConverter::toString(Simcore->GetFloor(temp4)->FullHeight());
				TrimString(buffer);
				LineData = LineData.substr(0, temp1) + buffer + LineData.substr(temp1 + temp6.length());
			}

			//height parameter
			buffer = Ogre::StringConverter::toString(temp4);
			TrimString(buffer);
			temp6 = "floor(" + buffer + ").height";
			buffer = LineData;
			SetCase(buffer, false);
			temp1 = buffer.find(temp6, 0);
			if (temp1 > 0)
			{
				buffer = Ogre::StringConverter::toString(Simcore->GetFloor(temp4)->Height);
				TrimString(buffer);
				LineData = LineData.substr(0, temp1) + buffer + LineData.substr(temp1 + temp6.length());
			}

			//altitude parameter
			buffer = Ogre::StringConverter::toString(temp4);
			TrimString(buffer);
			temp6 = "floor(" + buffer + ").altitude";
			buffer = LineData;
			SetCase(buffer, false);
			temp1 = buffer.find(temp6, 0);
			if (temp1 > 0)
			{
				buffer = Ogre::StringConverter::toString(Simcore->GetFloor(temp4)->Altitude);
				TrimString(buffer);
				LineData = LineData.substr(0, temp1) + buffer + LineData.substr(temp1 + temp6.length());
			}

			//interfloorheight parameter
			buffer = Ogre::StringConverter::toString(temp4);
			TrimString(buffer);
			temp6 = "floor(" + buffer + ").interfloorheight";
			buffer = LineData;
			SetCase(buffer, false);
			temp1 = buffer.find(temp6, 0);
			if (temp1 > 0)
			{
				buffer = Ogre::StringConverter::toString(Simcore->GetFloor(temp4)->InterfloorHeight);
				TrimString(buffer);
				LineData = LineData.substr(0, temp1) + buffer + LineData.substr(temp1 + temp6.length());
			}
			temp5 = SetCaseCopy(LineData, false).find("floor(", 0);

			//base parameter
			buffer = Ogre::StringConverter::toString(temp4);
			TrimString(buffer);
			temp6 = "floor(" + buffer + ").base";
			buffer = LineData;
			SetCase(buffer, false);
			temp1 = buffer.find(temp6, 0);
			if (temp1 > 0)
			{
				buffer = Ogre::StringConverter::toString(Simcore->GetFloor(temp4)->GetBase());
				TrimString(buffer);
				LineData = LineData.substr(0, temp1) + buffer + LineData.substr(temp1 + temp6.length());
			}
			temp5 = SetCaseCopy(LineData, false).find("floor(", 0);
		}

		//Extent variables
		ReplaceAll(LineData, "%minx%", _gcvt(MinExtent.x, 12, buffer2));
		ReplaceAll(LineData, "%minz%", _gcvt(MinExtent.z, 12, buffer2));
		ReplaceAll(LineData, "%maxx%", _gcvt(MaxExtent.x, 12, buffer2));
		ReplaceAll(LineData, "%maxz%", _gcvt(MaxExtent.z, 12, buffer2));

		//Global commands
		returncode = ProcCommands();

		//handle return values
		if (returncode == sNextLine)
			goto Nextline;
		if (returncode == sError)
			return false;
		if (returncode == sCheckFloors)
			goto checkfloors;
		if (returncode == sBreak)
			goto breakpoint;
		if (returncode == sRecalc)
			goto recalc;

		//Global parameters
		if (Section == 1)
		{
			returncode = ProcGlobals();

			//handle return values
			if (returncode == sNextLine)
				goto Nextline;
			if (returncode == sError)
				return false;
			if (returncode == sCheckFloors)
				goto checkfloors;
			if (returncode == sBreak)
				goto breakpoint;
			if (returncode == sRecalc)
				goto recalc;
		}

		//Process floors
		if (Section == 2)
		{
			//create floor if not created already
			Simcore->NewFloor(Current);
recalc:
			returncode = ProcFloors();

			//handle return values
			if (returncode == sNextLine)
				goto Nextline;
			if (returncode == sError)
				return false;
			if (returncode == sCheckFloors)
				goto checkfloors;
			if (returncode == sBreak)
				goto breakpoint;
			if (returncode == sRecalc)
				goto recalc;
		}

		//process elevators
		if (Section == 4)
		{
			returncode = ProcElevators();

			//handle return values
			if (returncode == sNextLine)
				goto Nextline;
			if (returncode == sError)
				return false;
			if (returncode == sCheckFloors)
				goto checkfloors;
			if (returncode == sBreak)
				goto breakpoint;
			if (returncode == sRecalc)
				goto recalc;
		}

		//Process textures
		if (Section == 5)
		{
			returncode = ProcTextures();

			//handle return values
			if (returncode == sNextLine)
				goto Nextline;
			if (returncode == sError)
				return false;
			if (returncode == sCheckFloors)
				goto checkfloors;
			if (returncode == sBreak)
				goto breakpoint;
			if (returncode == sRecalc)
				goto recalc;
		}

		//reset texture override status
		Simcore->TextureOverride = false;

		//reset texture flip status
		Simcore->FlipTexture = false;

Nextline:
		line++;
	}

	return true;
}

bool ScriptProcessor::LoadDataFile(const char *filename, bool insert, int insert_line)
{
	//loads a building data file into the runtime buffer
	int location = insert_line;
	std::string Filename = filename;

	//if insert location is greater than array size, return with error
	if (insert == true)
		if (location > BuildingData.size() - 1 || location < 0)
			return false;

	//make sure file exists
	if (Simcore->FileExists(Filename.c_str()) == false)
		return false;

	//Simcore->Report(Filename);

	//load file
	Ogre::FileSystemArchive filesystem(".","FileSystem");
	Ogre::DataStreamPtr filedata = filesystem.open(filename, true);

	//exit if an error occurred while loading
	if(filedata.isNull())
		return false;

	Ogre::DataStreamPtr file(OGRE_NEW Ogre::MemoryDataStream(filename, filedata));

	//clear array
	if (insert == false)
		BuildingData.clear();

	while (file->eof() == false)
	{
		//push next line of data onto the tail end of the BuildingData array
		std::string line = file->getLine(true);
		if (insert == false)
		{
			BuildingData.push_back(line);
			skyscraper->runtime_script.push_back(line);
		}
		else
		{
			//otherwise insert data into building array
			BuildingData.insert(BuildingData.begin() + location, line);
			location++;
		}
	}

	if (insert == true)
	{
		//store include info in array
		IncludeInfo info;
		info.filename = filename;
		info.start_line = line;
		info.end_line = location - 1;
		includes.push_back(info);
	}

	return true;
}

bool ScriptProcessor::LoadFromText(const char *text)
{
	//loads building commands from a string
	std::vector<std::string> textarray;
	SplitString(textarray, text, '\n');

	//clear building data
	BuildingData.clear();

	//feed each line of text into the script array
	BuildingData.reserve(textarray.size());
	for (int i = 0; i < (int)textarray.size(); i++)
	{
		BuildingData.push_back(textarray[i]);
		skyscraper->runtime_script.push_back(textarray[i]);
	}
	return true;
}

bool ScriptProcessor::IfProc(const char *expression)
{
	//IF statement processor

	int temp1;
	std::string tmpcalc = expression;
	std::string one;
	std::string two;
	int start, end;
	bool check;

	//first remove all whitespace from the string
	ReplaceAll(tmpcalc, " ", "");

	//first check for bad and/or character sets
	if (int(tmpcalc.find("&&")) >= 0 || int(tmpcalc.find("||")) >= 0 || int(tmpcalc.find("==")) >= 0 || int(tmpcalc.find("!=")) >= 0)
	{
		ScriptError("Syntax error in IF operation: '" + tmpcalc + "' (might be nested)");
		return false;
	}

	//find parenthesis
	do
	{
		start = tmpcalc.find("(", 0);
		if (start >= 0)
		{
			//find matching parenthesis
			int match = 1;
			int end = -1;
			for (int i = start + 1; i < (int)tmpcalc.length(); i++)
			{
				if (tmpcalc.at(i) == '(')
					match++;
				if (tmpcalc.at(i) == ')')
					match--;
				if (match == 0)
				{
					end = i;
					break;
				}
			}
			if (end != -1)
			{
				//call function recursively
				std::string newdata;
				if (IfProc(tmpcalc.substr(start + 1, end - start - 1).c_str()) == true)
					newdata = "true";
				else
					newdata = "false";
				//construct new string
				one = tmpcalc.substr(0, start);
				if (end < (int)tmpcalc.length() - 1)
					two = tmpcalc.substr(end + 1);
				else
					two = "";
				tmpcalc = one + newdata + two;
			}
			else
			{
				ScriptError("Syntax error in IF operation: '" + tmpcalc + "' (might be nested)");
				return false;
			}
		}
		else
			break;
	} while (1 == 1);
	//find number of operators and recurse if multiple found
	int operators;
	int operators2;
	do
	{
		operators = 0;
		operators2 = 0;
		start = 0;
		end = 0;
		check = false;
		for (int i = 1; i < (int)tmpcalc.length(); i++)
		{
			if (tmpcalc.at(i) == '=' || tmpcalc.at(i) == '!' || tmpcalc.at(i) == '<' || tmpcalc.at(i) == '>')
			{
				operators++;
			}
			if (tmpcalc.at(i) == '&' || tmpcalc.at(i) == '|')
			{
				check = true;
				operators2++;
				if (operators == 1 && operators2 == 2)
				{
					//handle 2 and/if operators
					end = i;
					start = 0;
					operators = 2;
					break;
				}
				if (operators == 1)
				{
					operators = 2;
					end = i;
				}
				else if (operators == 0)
				{
					operators = 1;
					start = i + 1;
					end = (int)tmpcalc.length();
				}
			}
		}
		//return error if multiple standard operators are found, but no and/or operator (ex. if[5 = 5 = 5])
		if (operators > 1 && check == false)
		{
			ScriptError("Syntax error in IF operation: '" + tmpcalc + "' (might be nested)");
			return false;
		}
		if (operators > 1)
		{
			std::string newdata;
			if (IfProc(tmpcalc.substr(start, end - start).c_str()) == true)
				newdata = "true";
			else
				newdata = "false";
			//construct new string
			one = tmpcalc.substr(0, start);
			two = tmpcalc.substr(end);
			tmpcalc = one + newdata + two;
		}
		else
			break;
	} while (1 == 1);
	//return value if none found
	if (operators == 0)
	{
		if (tmpcalc == "true")
			return true;
		else
			return false;
	}

	//otherwise perform comparisons
	temp1 = tmpcalc.find("=", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.substr(0, temp1);
		two = tmpcalc.substr(temp1 + 1);
		if (one == two)
			return true;
		else
			return false;
	}
	temp1 = tmpcalc.find("!", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.substr(0, temp1);
		two = tmpcalc.substr(temp1 + 1);
		if (one != two)
			return true;
		else
			return false;
	}
	temp1 = tmpcalc.find("<", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.substr(0, temp1);
		two = tmpcalc.substr(temp1 + 1);
		if (IsNumeric(one.c_str()) == true && IsNumeric(two.c_str()) == true)
		{
			if(atof(one.c_str()) < atof(two.c_str()))
				return true;
			else
				return false;
		}
		else
			return false;
	}
	temp1 = tmpcalc.find(">", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.substr(0, temp1);
		two = tmpcalc.substr(temp1 + 1);
		if (IsNumeric(one.c_str()) == true && IsNumeric(two.c_str()) == true)
		{
			if(atof(one.c_str()) > atof(two.c_str()))
				return true;
			else
				return false;
		}
		else
			return false;
	}
	temp1 = tmpcalc.find("&", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.substr(0, temp1);
		two = tmpcalc.substr(temp1 + 1);
		if (one == "true" && two == "true")
			return true;
		else
			return false;
	}
	temp1 = tmpcalc.find("|", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.substr(0, temp1);
		two = tmpcalc.substr(temp1 + 1);
		if (one == "true" || two == "true")
			return true;
		else
			return false;
	}
	if (tmpcalc == "true")
		return true;
	else
		return false;
}

int ScriptProcessor::ScriptError(std::string message)
{
	//first see if the current line is from an included file

	int linenum = line;
	int included_lines = 0;
	bool isinclude = false;
	std::string includefile;

	for (int i = 0; i < (int)includes.size(); i++)
	{
		if (linenum < includes[i].start_line)
			break;

		//keep track of original script position (keep a count of how many lines are "included" lines)
		if (linenum > includes[i].end_line)
			included_lines += (includes[i].end_line - includes[i].start_line);

		//line is part of an included file
		if (linenum >= includes[i].start_line && linenum <= includes[i].end_line)
		{
			isinclude = true;
			includefile = includes[i].filename;
			linenum = linenum - includes[i].start_line + 1;
		}
	}

	//Script error reporting function
	char intbuffer[65];
	std::string error;
	if (isinclude == false)
		error = "Script error on line " + std::string(_itoa(linenum - included_lines + 1, intbuffer, 10)) + ": " + message + "\nSection: " + std::string(_itoa(Section, intbuffer, 10)) + "\nContext: " + Context + "\nLine Text: " + LineData;
	else
		error = "Script error in included file " + includefile + " on line " + std::string(_itoa(linenum, intbuffer, 10)) + ": " + std::string(message) + "\nSection: " + std::string(_itoa(Section, intbuffer, 10)) + "\nContext: " + Context + "\nLine Text: " + LineData;

	skyscraper->ReportError(error.c_str());

	//show error dialog
	wxMessageDialog *dialog = new wxMessageDialog(0, wxString::FromAscii(error.c_str()), wxString::FromAscii("Skyscraper"), wxOK | wxICON_ERROR);
	dialog->ShowModal();

	delete dialog;
	dialog = 0;
	return sError;
}

int ScriptProcessor::ScriptError()
{
	//return automatic error message from contents of the simulator engine's LastError string

	std::string message = Simcore->LastError;
	int loc = message.find_last_of(":");

	std::string result = message.substr(loc + 1);
	TrimString(result);
	return ScriptError(result);
}

bool ScriptProcessor::ReportMissingFiles()
{
	//report on missing files
	//returns true if any files are missing

	if (nonexistent_files.size() > 0)
	{
		sort(nonexistent_files.begin(), nonexistent_files.end());
		for (int i = 0; i < (int)nonexistent_files.size(); i++)
			Simcore->Report(nonexistent_files[i]);

		//create text window
		TextWindow *twindow = new TextWindow(NULL, -1);
		twindow->SetMinSize(wxSize(300, 200));
		twindow->tMain->SetMinSize(wxSize(300, 200));
		twindow->Fit();
		twindow->Center();
		twindow->SetTitle(wxT("Missing Files"));
		twindow->Show(true);
		wxString message;
		message = wxT("Skyscraper was unable to load the following files.\nThis will result in texture and/or sound problems:\n\n");
		for (int i = 0; i < (int)nonexistent_files.size(); i++)
		{
			message.Append(wxString::FromAscii(nonexistent_files[i].c_str()));
			message.Append(wxT("\n"));
		}
		twindow->tMain->WriteText(message);
		twindow->tMain->SetInsertionPoint(0);
		return true;
	}
	else
		return false;
}

int ScriptProcessor::ProcCommands()
{
	//process global commands

	if (Section != 2 && Section != 4)
	{
		//process any functions first
		if (FunctionProc() == true)
			return sNextLine;
	}

	//Print command
	if (SetCaseCopy(LineData.substr(0, 5), false) == "print" && Section != 2 && Section != 4)
	{
		//calculate inline math
		buffer = Calc(LineData.substr(6).c_str());

		//print line
		skyscraper->Report(buffer.c_str());
	}

	//AddTriangleWall command
	if (SetCaseCopy(LineData.substr(0, 15), false) == "addtrianglewall")
	{
		if (Section == 2 && getfloordata == false)
		{
			//process floor-specific variables if in a floor section
			getfloordata = true;
			return sRecalc;
		}
		else
			getfloordata = false;

		//get data
		int params = SplitData(LineData.c_str(), 16);

		if (params < 14 || params > 14)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 13; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		buffer = tempdata[0];
		SetCase(buffer, false);
		MeshObject* tmpMesh;

		if (buffer == "floor")
		{
			tmpMesh = Simcore->GetFloor(Current)->Level;
			wall = tmpMesh->CreateWallObject(Simcore->GetFloor(Current)->object, tempdata[1].c_str());
		}
		else if (buffer == "elevator")
		{
			return sNextLine;
			tmpMesh = Simcore->GetElevator(Current)->ElevatorMesh;
			wall = tmpMesh->CreateWallObject(Simcore->GetElevator(Current)->object, tempdata[1].c_str());
		}
		else if (buffer == "external")
		{
			tmpMesh = Simcore->External;
			wall = tmpMesh->CreateWallObject(Simcore->object, tempdata[1].c_str());
		}
		else if (buffer == "landscape")
		{
			tmpMesh = Simcore->Landscape;
			wall = tmpMesh->CreateWallObject(Simcore->object, tempdata[1].c_str());
		}
		else if (buffer == "buildings")
		{
			tmpMesh = Simcore->Buildings;
			wall = tmpMesh->CreateWallObject(Simcore->object, tempdata[1].c_str());
		}
		else
			return ScriptError("Invalid object");

		//store command and line info in object
		StoreCommand(wall);

		float voffset1 = atof(tempdata[4].c_str());
		float voffset2 = atof(tempdata[7].c_str());
		if (Section == 2)
		{
			voffset1 += Ogre::Real(Simcore->GetFloor(Current)->GetBase());
			voffset2 += Ogre::Real(Simcore->GetFloor(Current)->GetBase());
		}

		//create triangle wall
		Simcore->AddTriangleWall(wall, tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), voffset1, atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), voffset2, atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()));
	}

	//AddWall command
	if (SetCaseCopy(LineData.substr(0, 7), false) == "addwall" && Section != 2 && Section != 4)
	{
		//get data
		int params = SplitData(LineData.c_str(), 8);

		if (params < 14 || params > 14)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 13; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		//create wall
		StoreCommand(Simcore->AddWall(tempdata[0].c_str(), tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str())));
	}

	//AddFloor
	if (SetCaseCopy(LineData.substr(0, 9), false) == "addfloor " && Section != 2 && Section != 4)
	{
		//get data
		int params = SplitData(LineData.c_str(), 9);

		if (params < 12 || params > 12)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 11; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		//create floor
		StoreCommand(Simcore->AddFloor(tempdata[0].c_str(), tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str())));
	}

	//AddGround
	if (SetCaseCopy(LineData.substr(0, 9), false) == "addground")
	{
		//get data
		int params = SplitData(LineData.c_str(), 10);

		if (params < 9 || params > 9)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 8; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		//create tiled ground
		StoreCommand(Simcore->AddGround(tempdata[0].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atoi(tempdata[7].c_str()), atoi(tempdata[8].c_str())));
	}

	//Cut command
	if (SetCaseCopy(LineData.substr(0, 4), false) == "cut " && Section != 2 && Section != 4)
	{
		//get data
		int params = SplitData(LineData.c_str(), 4);

		if (params != 9)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 6; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		buffer = tempdata[0];
		SetCase(buffer, false);
		std::vector<WallObject*> *wallarray;

		if (buffer == "external")
			wallarray = &Simcore->External->Walls;
		else if (buffer == "landscape")
			wallarray = &Simcore->Landscape->Walls;
		else if (buffer == "buildings")
			wallarray = &Simcore->Buildings->Walls;
		else
			return ScriptError("Invalid object");

		//perform cut
		for (int i = 0; i < (int)wallarray->size(); i++)
			Simcore->Cut(wallarray->at(i), Ogre::Vector3(atof(tempdata[1].c_str()), atof(tempdata[2].c_str()), atof(tempdata[3].c_str())), Ogre::Vector3(atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str())), Ogre::StringConverter::parseBool(tempdata[7]), Ogre::StringConverter::parseBool(tempdata[8]), Ogre::Vector3(0, 0, 0), Ogre::Vector3(0, 0, 0));
	}

	//Set command
	if (SetCaseCopy(LineData.substr(0, 4), false) == "set " && Section != 2 && Section != 4)
	{
		temp1 = LineData.find("=", 0);
		temp3 = atoi(LineData.substr(4, temp1 - 5).c_str());

		//get text after equal sign
		temp2 = GetAfterEquals(LineData.c_str());

		if (temp3 < 0 || temp3 > UserVariable.size() - 1)
			return ScriptError("Invalid variable number");

		UserVariable[temp3] = Calc(temp2.c_str());
		if (Simcore->Verbose == true)
			skyscraper->Report("Variable " + std::string(_itoa(temp3, intbuffer, 10)) + " set to " + UserVariable[temp3]);
	}

	//CreateWallBox2 command
	if (SetCaseCopy(LineData.substr(0, 14), false) == "createwallbox2")
	{
		if (Section == 2 && getfloordata == false)
		{
			//process floor-specific variables if in a floor section
			getfloordata = true;
			return sRecalc;
		}
		else
			getfloordata = false;

		//get data
		int params = SplitData(LineData.c_str(), 15);

		if (params != 15)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 10; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		buffer = tempdata[0];
		SetCase(buffer, false);
		MeshObject* tmpMesh;

		if (buffer == "floor")
		{
			tmpMesh = Simcore->GetFloor(Current)->Level;
			wall = tmpMesh->CreateWallObject(Simcore->GetFloor(Current)->object, tempdata[1].c_str());
		}
		else if (buffer == "elevator")
		{
			return sNextLine;
			tmpMesh = Simcore->GetElevator(Current)->ElevatorMesh;
			wall = tmpMesh->CreateWallObject(Simcore->GetElevator(Current)->object, tempdata[1].c_str());
		}
		else if (buffer == "external")
		{
			tmpMesh = Simcore->External;
			wall = tmpMesh->CreateWallObject(Simcore->object, tempdata[1].c_str());
		}
		else if (buffer == "landscape")
		{
			tmpMesh = Simcore->Landscape;
			wall = tmpMesh->CreateWallObject(Simcore->object, tempdata[1].c_str());
		}
		else if (buffer == "buildings")
		{
			tmpMesh = Simcore->Buildings;
			wall = tmpMesh->CreateWallObject(Simcore->object, tempdata[1].c_str());
		}
		else
			return ScriptError("Invalid object");

		StoreCommand(wall);

		float voffset = atof(tempdata[8].c_str());
		if (Section == 2)
			voffset += Ogre::Real(Simcore->GetFloor(Current)->GetBase());

		Simcore->CreateWallBox2(wall, tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), voffset, atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), Ogre::StringConverter::parseBool(tempdata[11]), Ogre::StringConverter::parseBool(tempdata[12]), Ogre::StringConverter::parseBool(tempdata[13]), Ogre::StringConverter::parseBool(tempdata[14]));
	}

	//CreateWallBox command
	if (SetCaseCopy(LineData.substr(0, 14), false) == "createwallbox ")
	{
		if (Section == 2 && getfloordata == false)
		{
			//process floor-specific variables if in a floor section
			getfloordata = true;
			return sRecalc;
		}
		else
			getfloordata = false;

		//get data
		int params = SplitData(LineData.c_str(), 14);

		if (params != 15)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 10; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		buffer = tempdata[0];
		SetCase(buffer, false);
		MeshObject* tmpMesh;

		if (buffer == "floor")
		{
			tmpMesh = Simcore->GetFloor(Current)->Level;
			wall = tmpMesh->CreateWallObject(Simcore->GetFloor(Current)->object, tempdata[1].c_str());
		}
		else if (buffer == "elevator")
		{
			return sNextLine;
			tmpMesh = Simcore->GetElevator(Current)->ElevatorMesh;
			wall = tmpMesh->CreateWallObject(Simcore->GetElevator(Current)->object, tempdata[1].c_str());
		}
		else if (buffer == "external")
		{
			tmpMesh = Simcore->External;
			wall = tmpMesh->CreateWallObject(Simcore->object, tempdata[1].c_str());
		}
		else if (buffer == "landscape")
		{
			tmpMesh = Simcore->Landscape;
			wall = tmpMesh->CreateWallObject(Simcore->object, tempdata[1].c_str());
		}
		else if (buffer == "buildings")
		{
			tmpMesh = Simcore->Buildings;
			wall = tmpMesh->CreateWallObject(Simcore->object, tempdata[1].c_str());
		}
		else
			return ScriptError("Invalid object");

		StoreCommand(wall);

		float voffset = atof(tempdata[8].c_str());
		if (Section == 2)
			voffset += Ogre::Real(Simcore->GetFloor(Current)->GetBase());

		Simcore->CreateWallBox(wall, tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), voffset, atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), Ogre::StringConverter::parseBool(tempdata[11]), Ogre::StringConverter::parseBool(tempdata[12]), Ogre::StringConverter::parseBool(tempdata[13]), Ogre::StringConverter::parseBool(tempdata[14]));
	}

	//AddCustomWall command
	if (SetCaseCopy(LineData.substr(0, 14), false) == "addcustomwall ")
	{
		if (Section == 2 && getfloordata == false)
		{
			//process floor-specific variables if in a floor section
			getfloordata = true;
			return sRecalc;
		}
		else
			getfloordata = false;

		//get data
		int params = SplitData(LineData.c_str(), 14);

		//check numeric values
		for (int i = 3; i < params - 2; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		buffer = tempdata[0];
		SetCase(buffer, false);
		MeshObject* tmpMesh;

		if (buffer == "floor")
		{
			tmpMesh = Simcore->GetFloor(Current)->Level;
			wall = tmpMesh->CreateWallObject(Simcore->GetFloor(Current)->object, tempdata[1].c_str());
		}
		else if (buffer == "elevator")
		{
			return sNextLine;
			tmpMesh = Simcore->GetElevator(Current)->ElevatorMesh;
			wall = tmpMesh->CreateWallObject(Simcore->GetElevator(Current)->object, tempdata[1].c_str());
		}
		else if (buffer == "external")
		{
			tmpMesh = Simcore->External;
			wall = tmpMesh->CreateWallObject(Simcore->object, tempdata[1].c_str());
		}
		else if (buffer == "landscape")
		{
			tmpMesh = Simcore->Landscape;
			wall = tmpMesh->CreateWallObject(Simcore->object, tempdata[1].c_str());
		}
		else if (buffer == "buildings")
		{
			tmpMesh = Simcore->Buildings;
			wall = tmpMesh->CreateWallObject(Simcore->object, tempdata[1].c_str());
		}
		else
			return ScriptError("Invalid object");

		std::vector<Ogre::Vector3> varray;
		for (temp3 = 3; temp3 < params - 2; temp3 += 3)
			varray.push_back(Ogre::Vector3(atof(tempdata[temp3].c_str()), atof(tempdata[temp3 + 1].c_str()), atof(tempdata[temp3 + 2].c_str())));

		StoreCommand(wall);

		Simcore->AddCustomWall(wall, tempdata[1].c_str(), tempdata[2].c_str(), varray, atof(tempdata[params - 2].c_str()), atof(tempdata[params - 1].c_str()));
	}

	//AddShaft command
	if (SetCaseCopy(LineData.substr(0, 9), false) == "addshaft ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 9);

		if (params != 6)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 5; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		if (atoi(tempdata[4].c_str()) < -Simcore->Basements)
			return ScriptError("Invalid starting floor");
		if (atoi(tempdata[5].c_str()) > Simcore->Floors - 1)
			return ScriptError("Invalid ending floor");

		Object *object = Simcore->CreateShaft(atoi(tempdata[0].c_str()), atoi(tempdata[1].c_str()), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atoi(tempdata[4].c_str()), atoi(tempdata[5].c_str()));
		if (!object)
			return ScriptError();

		StoreCommand(object);
	}

	//ShaftCut command
	if (SetCaseCopy(LineData.substr(0, 9), false) == "shaftcut ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 9);

		if (params != 7)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 6; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		//check for existence of shaft
		int shaftnum = atoi(tempdata[0].c_str());
		if (shaftnum < 1 || shaftnum > Simcore->Shafts())
			return ScriptError("Invalid shaft " + std::string(tempdata[0]));

		Simcore->GetShaft(shaftnum)->CutFloors(true, Ogre::Vector2(atof(tempdata[1].c_str()), atof(tempdata[2].c_str())), Ogre::Vector2(atof(tempdata[3].c_str()), atof(tempdata[4].c_str())), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()));
	}

	//ShaftShowFloors command
	if (SetCaseCopy(LineData.substr(0, 15), false) == "shaftshowfloors")
	{
		//get shaft number
		int loc = LineData.find("=");
		if (loc < 0)
			return ScriptError("Syntax error");
		int shaftnum;
		std::string str = LineData.substr(15, loc - 16);
		TrimString(str);
		if (!IsNumeric(str.c_str(), shaftnum))
			return ScriptError("Invalid shaft number");

		if (shaftnum < 1 || shaftnum > Simcore->Shafts())
			return ScriptError("Invalid shaft number");

		Simcore->GetShaft(shaftnum)->ShowFloors = true;

		int params = SplitAfterEquals(LineData.c_str(), false);
		if (params == -1)
			return ScriptError("Syntax Error");

		for (int line = 0; line < params; line++)
		{
			std::string tmpstring = tempdata[line];
			TrimString(tmpstring);
			if (tmpstring.find("-", 1) > 0)
			{
				int start, end;
				//found a range marker
				std::string str1 = tmpstring.substr(0, tmpstring.find("-", 1));
				std::string str2 = tmpstring.substr(tmpstring.find("-", 1) + 1);
				TrimString(str1);
				TrimString(str2);
				if (!IsNumeric(str1.c_str(), start) || !IsNumeric(str2.c_str(), end))
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
				std::string str = tempdata[line];
				TrimString(str);
				if (!IsNumeric(str.c_str(), showfloor))
					return ScriptError("Invalid value");
				Simcore->GetShaft(shaftnum)->AddShowFloor(showfloor);
			}
		}
	}

	//ShaftShowOutside command
	if (SetCaseCopy(LineData.substr(0, 16), false) == "shaftshowoutside")
	{
		//get shaft number
		int loc = LineData.find("=");
		if (loc < 0)
			return ScriptError("Syntax error");
		int shaftnum;
		std::string str = LineData.substr(16, loc - 17);
		TrimString(str);
		if (!IsNumeric(str.c_str(), shaftnum))
			return ScriptError("Invalid shaft number");
		if (shaftnum < 1 || shaftnum > Simcore->Shafts())
			return ScriptError("Invalid shaft number");
		Simcore->GetShaft(shaftnum)->ShowOutside = true;

		int params = SplitAfterEquals(LineData.c_str(), false);
		if (params == -1)
			return ScriptError("Syntax Error");

		for (int line = 0; line < params; line++)
		{
			std::string tmpstring = tempdata[line];
			TrimString(tmpstring);
			if (tmpstring.find("-", 1) > 0)
			{
				int start, end;
				//found a range marker
				std::string str1 = tmpstring.substr(0, tmpstring.find("-", 1));
				std::string str2 = tmpstring.substr(tmpstring.find("-", 1) + 1);
				TrimString(str1);
				TrimString(str2);
				if (!IsNumeric(str1.c_str(), start) || !IsNumeric(str2.c_str(), end))
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
				std::string str = tempdata[line];
				TrimString(str);
				if (!IsNumeric(str.c_str(), showfloor))
					return ScriptError("Invalid value");
				Simcore->GetShaft(shaftnum)->AddShowOutside(showfloor);
			}
		}
	}

	//ShowFullShaft command
	if (SetCaseCopy(LineData.substr(0, 13), false) == "showfullshaft")
	{
		//get shaft number
		int loc = LineData.find("=");
		if (loc < 0)
			return ScriptError("Syntax error");
		int shaftnum;
		std::string str = LineData.substr(13, loc - 14);
		TrimString(str);
		if (!IsNumeric(str.c_str(), shaftnum))
			return ScriptError("Invalid shaft number");
		if (shaftnum < 1 || shaftnum > Simcore->Shafts())
			return ScriptError("Invalid shaft number");

		//get text after equal sign
		temp2 = GetAfterEquals(LineData.c_str());

		Simcore->GetShaft(shaftnum)->ShowFullShaft = Ogre::StringConverter::parseBool(temp2);
	}

	//CreateStairwell command
	if (SetCaseCopy(LineData.substr(0, 15), false) == "createstairwell")
	{
		//get data
		int params = SplitData(LineData.c_str(), 16);

		if (params != 5)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 4; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		Object *object = Simcore->CreateStairwell(atoi(tempdata[0].c_str()), atof(tempdata[1].c_str()), atof(tempdata[2].c_str()), atoi(tempdata[3].c_str()), atoi(tempdata[4].c_str()));
		if (!object)
			return ScriptError();

		StoreCommand(object);
	}

	//CutStairwell command
	if (SetCaseCopy(LineData.substr(0, 13), false) == "cutstairwell ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 13);

		if (params != 7)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 6; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		int stairwell = atoi(tempdata[0].c_str());
		if (!Simcore->GetStairs(stairwell))
			return ScriptError("Invalid stairwell");

		Simcore->GetStairs(stairwell)->CutFloors(true, Ogre::Vector2(atof(tempdata[1].c_str()), atof(tempdata[2].c_str())), Ogre::Vector2(atof(tempdata[3].c_str()), atof(tempdata[4].c_str())), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()));
	}

	//WallOrientation command
	if (SetCaseCopy(LineData.substr(0, 15), false) == "wallorientation")
	{
		//get text after equal sign
		temp2 = GetAfterEquals(LineData.c_str());

		if (!Simcore->SetWallOrientation(temp2.c_str()))
			return ScriptError();
	}

	//FloorOrientation command
	if (SetCaseCopy(LineData.substr(0, 16), false) == "floororientation")
	{
		//get text after equal sign
		temp2 = GetAfterEquals(LineData.c_str());

		if (!Simcore->SetFloorOrientation(temp2.c_str()))
			return ScriptError();
	}

	//DrawWalls command
	if (SetCaseCopy(LineData.substr(0, 9), false) == "drawwalls")
	{
		int params = SplitAfterEquals(LineData.c_str());
		if (params == -1)
			return ScriptError("Syntax Error");
		if (params != 6)
			return ScriptError("Incorrect number of parameters");

		Simcore->DrawWalls(Ogre::StringConverter::parseBool(tempdata[0]),
					Ogre::StringConverter::parseBool(tempdata[1]),
					Ogre::StringConverter::parseBool(tempdata[2]),
					Ogre::StringConverter::parseBool(tempdata[3]),
					Ogre::StringConverter::parseBool(tempdata[4]),
					Ogre::StringConverter::parseBool(tempdata[5]));
	}

	//SetTextureMapping command
	if (SetCaseCopy(LineData.substr(0, 18), false) == "settexturemapping ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 18);

		if (params != 9)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 8; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		Simcore->SetTextureMapping(atoi(tempdata[0].c_str()), Ogre::Vector2(atof(tempdata[1].c_str()), atof(tempdata[2].c_str())),
									atoi(tempdata[3].c_str()), Ogre::Vector2(atof(tempdata[4].c_str()), atof(tempdata[5].c_str())),
									atoi(tempdata[6].c_str()), Ogre::Vector2(atof(tempdata[7].c_str()), atof(tempdata[8].c_str())));
	}

	//SetTextureMapping2 command
	if (SetCaseCopy(LineData.substr(0, 18), false) == "settexturemapping2")
	{
		//get data
		int params = SplitData(LineData.c_str(), 19);

		if (params != 15)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 14; i++)
		{
			if (i == 5)
				i = 8;
			if (i == 10)
				i = 13;
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		Simcore->SetTextureMapping2(tempdata[0].c_str(), tempdata[1].c_str(), tempdata[2].c_str(), Ogre::Vector2(atof(tempdata[3].c_str()), atof(tempdata[4].c_str())),
									tempdata[5].c_str(), tempdata[6].c_str(), tempdata[7].c_str(), Ogre::Vector2(atof(tempdata[8].c_str()), atof(tempdata[9].c_str())),
									tempdata[10].c_str(), tempdata[11].c_str(), tempdata[12].c_str(), Ogre::Vector2(atof(tempdata[13].c_str()), atof(tempdata[14].c_str())));
	}

	//ResetTextureMapping command
	if (SetCaseCopy(LineData.substr(0, 19), false) == "resettexturemapping")
	{
		int temp2check = LineData.find("=", 0);
		if (temp2check < 0)
			return ScriptError("Syntax Error");

		//get text after equal sign
		temp2 = GetAfterEquals(LineData.c_str());

		Simcore->ResetTextureMapping(Ogre::StringConverter::parseBool(temp2));
	}

	//SetPlanarMapping command
	if (SetCaseCopy(LineData.substr(0, 16), false) == "setplanarmapping")
	{
		//get data
		int params = SplitData(LineData.c_str(), 17);

		if (params != 4)
			return ScriptError("Incorrect number of parameters");

		Simcore->SetPlanarMapping(Ogre::StringConverter::parseBool(tempdata[0]),
					Ogre::StringConverter::parseBool(tempdata[1]),
					Ogre::StringConverter::parseBool(tempdata[2]),
					Ogre::StringConverter::parseBool(tempdata[3]));
	}

	//ReverseAxis command
	if (SetCaseCopy(LineData.substr(0, 11), false) == "reverseaxis")
	{
		int temp2check = LineData.find("=", 0);
		if (temp2check < 0)
			return ScriptError("Syntax Error");
		temp2 = GetAfterEquals(LineData.c_str());

		Simcore->ReverseAxis(Ogre::StringConverter::parseBool(temp2));
	}

	//Intersection points
	temp5 = SetCaseCopy(LineData, false).find("isect(", 0);
	while (temp5 > -1)
	{
		if (Section == 2 && getfloordata == false)
		{
			//process floor-specific variables if in a floor section
			getfloordata = true;
			return sRecalc;
		}
		else
			getfloordata = false;

		temp1 = LineData.find("(", 0);
		temp4 = LineData.find(")", 0);
		if (temp1 < 0 || temp4 < 0)
			return ScriptError("Syntax error");

		SplitString(tempdata, LineData.substr(temp1 + 1, temp4 - temp1 - 1).c_str(), ',');
		for (temp3 = 0; temp3 < (int)tempdata.size(); temp3++)
		{
			buffer = Calc(tempdata[temp3].c_str());
			tempdata[temp3] = buffer;
		}
		if (tempdata.size() < 8 || tempdata.size() > 8)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 7; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		buffer = tempdata[0];
		SetCase(buffer, false);
		std::vector<WallObject*> *wall_array;
		if (buffer == "floor")
			wall_array = &Simcore->GetFloor(Current)->Level->Walls;
		else if (buffer == "elevator")
			wall_array = &Simcore->GetElevator(Current)->ElevatorMesh->Walls;
		else if (buffer == "external")
			wall_array = &Simcore->External->Walls;
		else if (buffer == "landscape")
			wall_array = &Simcore->Landscape->Walls;
		else if (buffer == "buildings")
			wall_array = &Simcore->Buildings->Walls;
		else
			return ScriptError("Invalid object");

		Ogre::Vector3 isect = Simcore->GetPoint(*wall_array, tempdata[1].c_str(), Ogre::Vector3(atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str())), Ogre::Vector3(atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str())));

		buffer = std::string(LineData).substr(0, temp5) + std::string(wxVariant(isect.x).GetString().ToAscii()) + std::string(", ") + std::string(wxVariant(isect.y).GetString().ToAscii()) + std::string(", ") + std::string(wxVariant(isect.z).GetString().ToAscii()) + std::string(LineData).substr(temp4 + 1);
		LineData = buffer;
	}

	//GetWallExtents command
	if (SetCaseCopy(LineData.substr(0, 14), false) == "getwallextents")
	{
		//get data
		int params = SplitData(LineData.c_str(), 15);

		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		std::string str = tempdata[2];
		TrimString(str);
		if (!IsNumeric(str.c_str()))
			return ScriptError("Invalid value: " + std::string(tempdata[2]));

		buffer = tempdata[0];
		SetCase(buffer, false);
		std::vector<WallObject*> *wall_array;
		if (buffer == "floor")
			wall_array = &Simcore->GetFloor(Current)->Level->Walls;
		else if (buffer == "elevator")
			wall_array = &Simcore->GetElevator(Current)->ElevatorMesh->Walls;
		else if (buffer == "external")
			wall_array = &Simcore->External->Walls;
		else if (buffer == "landscape")
			wall_array = &Simcore->Landscape->Walls;
		else if (buffer == "buildings")
			wall_array = &Simcore->Buildings->Walls;
		else
			return ScriptError("Invalid object");

		MinExtent = Simcore->GetWallExtents(*wall_array, tempdata[1].c_str(), atof(tempdata[2].c_str()), false);
		MaxExtent = Simcore->GetWallExtents(*wall_array, tempdata[1].c_str(), atof(tempdata[2].c_str()), true);
	}

	//GetWallExtents function
	temp5 = SetCaseCopy(LineData, false).find("getwallextents(", 0);
	while (temp5 > -1)
	{
		if (Section == 2 && getfloordata == false)
		{
			//process floor-specific variables if in a floor section
			getfloordata = true;
			return sRecalc;
		}
		else
			getfloordata = false;

		temp1 = LineData.find("(", 0);
		temp4 = LineData.find(")", 0);
		if (temp1 < 0 || temp4 < 0)
			return ScriptError("Syntax error");
		
		SplitString(tempdata, LineData.substr(temp1 + 1, temp4 - temp1 - 1).c_str(), ',');
		for (temp3 = 0; temp3 < (int)tempdata.size(); temp3++)
		{
			buffer = Calc(tempdata[temp3].c_str());
			tempdata[temp3] = buffer;
		}
		if (tempdata.size() < 4 || tempdata.size() > 4)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		std::string str = tempdata[2];
		TrimString(str);
		if (!IsNumeric(str.c_str()))
			return ScriptError("Invalid value: " + std::string(tempdata[2]));

		buffer = tempdata[0];
		SetCase(buffer, false);
		std::vector<WallObject*> *wall_array;
		if (buffer == "floor")
			wall_array = &Simcore->GetFloor(Current)->Level->Walls;
		else if (buffer == "elevator")
			wall_array = &Simcore->GetElevator(Current)->ElevatorMesh->Walls;
		else if (buffer == "external")
			wall_array = &Simcore->External->Walls;
		else if (buffer == "landscape")
			wall_array = &Simcore->Landscape->Walls;
		else if (buffer == "buildings")
			wall_array = &Simcore->Buildings->Walls;
		else
			return ScriptError("Invalid object");

		Ogre::Vector3 result = Simcore->GetWallExtents(*wall_array, tempdata[1].c_str(), atof(tempdata[2].c_str()), Ogre::StringConverter::parseBool(tempdata[3]));

		buffer = std::string(LineData).substr(0, temp5) + std::string(wxVariant(result.x).GetString().ToAscii()) + std::string(", ") + std::string(wxVariant(result.y).GetString().ToAscii()) + std::string(", ") + std::string(wxVariant(result.z).GetString().ToAscii()) + std::string(LineData).substr(temp4 + 1);
		LineData = buffer;
	}

	//SetAutoSize command
	if (SetCaseCopy(LineData.substr(0, 11), false) == "setautosize")
	{
		int params = SplitAfterEquals(LineData.c_str());
		if (params == -1)
			return ScriptError("Syntax Error");
		if (params != 2)
			return ScriptError("Incorrect number of parameters");

		Simcore->SetAutoSize(Ogre::StringConverter::parseBool(tempdata[0]),
					Ogre::StringConverter::parseBool(tempdata[1]));
	}

	//TextureOverride command
	if (SetCaseCopy(LineData.substr(0, 15), false) == "textureoverride")
	{
		int params = SplitData(LineData.c_str(), 16, false);

		if (params != 6)
			return ScriptError("Incorrect number of parameters");

		Simcore->SetTextureOverride(tempdata[0].c_str(), tempdata[1].c_str(), tempdata[2].c_str(), tempdata[3].c_str(), tempdata[4].c_str(), tempdata[5].c_str());
		return sNextLine;
	}

	//TextureFlip command
	if (SetCaseCopy(LineData.substr(0, 11), false) == "textureflip")
	{
		int params = SplitData(LineData.c_str(), 12, false);

		if (params != 6)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 5; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		Simcore->SetTextureFlip(atoi(tempdata[0].c_str()), atoi(tempdata[1].c_str()), atoi(tempdata[2].c_str()), atoi(tempdata[3].c_str()), atoi(tempdata[4].c_str()), atoi(tempdata[5].c_str()));
		return sNextLine;
	}

	//Mount command
	if (SetCaseCopy(LineData.substr(0, 5), false) == "mount")
	{
		//get data
		int params = SplitData(LineData.c_str(), 6, false);

		if (params != 2)
			return ScriptError("Incorrect number of parameters");

		buffer = tempdata[1];
		if (!Simcore->Mount(tempdata[0].c_str(), buffer.c_str()))
			return ScriptError();

		return sNextLine;
	}

	//AddFloorAutoArea command
	if (SetCaseCopy(LineData.substr(0, 16), false) == "addfloorautoarea")
	{
		//get data
		int params = SplitData(LineData.c_str(), 17);

		if (params != 6)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 5; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		//create floor auto area
		Simcore->AddFloorAutoArea(Ogre::Vector3(atof(tempdata[0].c_str()), atof(tempdata[1].c_str()), atof(tempdata[2].c_str())), Ogre::Vector3(atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str())));
	}

	//AddSound
	if (SetCaseCopy(LineData.substr(0, 8), false) == "addsound")
	{
		//get data
		int params = SplitData(LineData.c_str(), 9);

		if (params != 5 && params != 13)
			return ScriptError("Incorrect number of parameters");

		bool partial = false;
		if (params == 5)
			partial = true;

		//check numeric values
		if (partial == true)
		{
			for (int i = 2; i <= 4; i++)
			{
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
		}
		else
		{
			for (int i = 2; i <= 12; i++)
			{
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
		}

		//check to see if file exists
		CheckFile(std::string("data/" + tempdata[1]).c_str());

		if (partial == true)
			StoreCommand(Simcore->AddSound(tempdata[0].c_str(), tempdata[1].c_str(), Ogre::Vector3(atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()))));
		else
			StoreCommand(Simcore->AddSound(tempdata[0].c_str(), tempdata[1].c_str(), Ogre::Vector3(atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str())), atoi(tempdata[5].c_str()), atoi(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), Ogre::Vector3(atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str())))); 
	}

	//AddModel command
	if (SetCaseCopy(LineData.substr(0, 8), false) == "addmodel")
	{
		if (Section == 2 || Section == 4)
			return 0;

		//get data
		int params = SplitData(LineData.c_str(), 9);

		if (params != 14)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 3; i++)
		{
			if (i == 10)
				i++;
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		//check to see if file exists
		CheckFile(std::string("data/" + tempdata[1]).c_str());

		//create model
		StoreCommand(Simcore->AddModel(tempdata[0].c_str(), tempdata[1].c_str(), Ogre::Vector3(atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str())), Ogre::Vector3(atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str())), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), Ogre::StringConverter::parseBool(tempdata[10]), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str())));
	}

	return 0;
}

int ScriptProcessor::ProcGlobals()
{
	//process global parameters

	//get text after equal sign
	temp2 = GetAfterEquals(LineData.c_str());

	//store variable values
	if (SetCaseCopy(LineData.substr(0, 4), false) == "name")
		Simcore->BuildingName = temp2;
	if (SetCaseCopy(LineData.substr(0, 8), false) == "designer")
		Simcore->BuildingDesigner = temp2;
	if (SetCaseCopy(LineData.substr(0, 8), false) == "location")
		Simcore->BuildingLocation = temp2;
	if (SetCaseCopy(LineData.substr(0, 11), false) == "description")
		Simcore->BuildingDescription = temp2;
	if (SetCaseCopy(LineData.substr(0, 7), false) == "version")
		Simcore->BuildingVersion = temp2;
	if (SetCaseCopy(LineData.substr(0, 3), false) == "sky")
		Simcore->SkyName = temp2;
	if (SetCaseCopy(LineData.substr(0, 10), false) == "dynamicsky")
		skyscraper->SkyName = temp2;
	if (SetCaseCopy(LineData.substr(0, 10), false) == "collisions")
	{
		SetCase(temp2, false);
		if (temp2 == "false")
			Simcore->camera->EnableCollisions(false);
		else
			Simcore->camera->EnableCollisions(true);
	}
	if (SetCaseCopy(LineData.substr(0, 7), false) == "gravity")
	{
		SetCase(temp2, false);
		if (temp2 == "false")
			Simcore->camera->EnableGravity(false);
		else
			Simcore->camera->EnableGravity(true);
	}
	if (SetCaseCopy(LineData.substr(0, 11), false) == "camerafloor")
	{
		int data;
		if (!IsNumeric(temp2.c_str(), data))
			return ScriptError("Invalid floor");

		Simcore->camera->StartFloor = data;
	}
	if (SetCaseCopy(LineData.substr(0, 14), false) == "cameraposition")
	{
		float x, z;
		std::string str1 = temp2.substr(0, temp2.find(",", 0));
		std::string str2 = temp2.substr(temp2.find(",", 0) + 1);
		TrimString(str1);
		TrimString(str2);
		if (!IsNumeric(str1.c_str(), x) || !IsNumeric(str2.c_str(), z))
			return ScriptError("Invalid position");

		Simcore->camera->StartPositionX  = x;
		Simcore->camera->StartPositionZ  = z;
	}
	if (SetCaseCopy(LineData.substr(0, 15), false) == "cameradirection")
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
		if (!IsNumeric(str1.c_str(), x) || !IsNumeric(str2.c_str(), y) || !IsNumeric(str3.c_str(), z))
			return ScriptError("Invalid direction");

		Simcore->camera->SetStartDirection(Ogre::Vector3(x, y, z));
	}
	if (SetCaseCopy(LineData.substr(0, 14), false) == "camerarotation")
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
		if (!IsNumeric(str1.c_str(), x) || !IsNumeric(str2.c_str(), y) || !IsNumeric(str3.c_str(), z))
			return ScriptError("Invalid direction");

		Simcore->camera->SetStartRotation(Ogre::Vector3(x, y, z));
	}
	if (SetCaseCopy(LineData.substr(0, 15), false) == "interfloorontop")
	{
		Simcore->InterfloorOnTop = Ogre::StringConverter::parseBool(temp2);
	}
	return 0;
}

int ScriptProcessor::ProcFloors()
{
	//process floors

	Floor *floor = Simcore->GetFloor(Current);

	//exit with error if floor is invalid
	if (!floor)
	{
		std::string floornum;
		floornum = Ogre::StringConverter::toString(Current);
		return ScriptError("Invalid floor " + floornum);
	}

	//replace variables with actual values
	buffer = Ogre::StringConverter::toString(Current);
	ReplaceAll(LineData, "%floor%", buffer.c_str());
	buffer = Ogre::StringConverter::toString(floor->Height);
	ReplaceAll(LineData, "%height%", buffer.c_str());
	buffer = Ogre::StringConverter::toString(floor->FullHeight());
	ReplaceAll(LineData, "%fullheight%", buffer.c_str());
	buffer = Ogre::StringConverter::toString(floor->InterfloorHeight);
	ReplaceAll(LineData, "%interfloorheight%", buffer.c_str());
	buffer = Ogre::StringConverter::toString(floor->GetBase());
	ReplaceAll(LineData, "%base%", buffer.c_str());

	if (getfloordata == true)
		return sCheckFloors;

	//IF statement
	if (SetCaseCopy(LineData.substr(0, 2), false) == "if")
	{
		temp1 = LineData.find("[", 0);
		temp3 = LineData.find("]", 0);
		if (temp1 + temp3 > 0)
			temp2 = LineData.substr(temp1 + 1, temp3 - temp1 - 1);
		else
			temp2 = "";
		TrimString(temp2);
		if (IfProc(temp2.c_str()) == true)
		{
			//trim off IF statement
			LineData = LineData.substr(temp3 + 1);
			TrimString(LineData);
			return sCheckFloors;
		}
		else
			return sNextLine; //skip line
	}

	//process functions
	if (FunctionProc() == true)
		return sNextLine;

	//get text after equal sign
	int temp2check = LineData.find("=", 0);
	temp2 = GetAfterEquals(LineData.c_str());

	//parameters
	if (SetCaseCopy(LineData.substr(0, 6), false) == "height")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		std::string str = Calc(temp2.c_str());
		TrimString(str);
		if (!IsNumeric(str.c_str(), floor->Height))
			return ScriptError("Invalid value");
		if (FloorCheck < 2)
			FloorCheck = 1;
		else
			FloorCheck = 3;
	}
	if (SetCaseCopy(LineData.substr(0, 16), false) == "interfloorheight")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		std::string str = Calc(temp2.c_str());
		TrimString(str);
		if (!IsNumeric(str.c_str(), floor->InterfloorHeight))
			return ScriptError("Invalid value");
		if (FloorCheck == 0)
			FloorCheck = 2;
		else
			FloorCheck = 3;
	}
	if (SetCaseCopy(LineData.substr(0, 8), false) == "altitude")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		std::string str = Calc(temp2.c_str());
		TrimString(str);
		if (!IsNumeric(str.c_str(), floor->Altitude))
			return ScriptError("Invalid value");
	}
	if (SetCaseCopy(LineData.substr(0, 2), false) == "id")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		floor->ID = Calc(temp2.c_str());
	}
	if (SetCaseCopy(LineData.substr(0, 4), false) == "name")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		floor->Name = Calc(temp2.c_str());
	}
	if (SetCaseCopy(LineData.substr(0, 4), false) == "type")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		floor->FloorType = temp2;
	}
	if (SetCaseCopy(LineData.substr(0, 11), false) == "description")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		floor->Description = temp2;
	}
	if (SetCaseCopy(LineData.substr(0, 16), false) == "indicatortexture")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		floor->IndicatorTexture = Calc(temp2.c_str());
	}
	if (SetCaseCopy(LineData.substr(0, 5), false) == "group")
	{
		//copy string listing of group floors into array

		int params = SplitAfterEquals(LineData.c_str(), false);
		if (params == -1)
			return ScriptError("Syntax Error");

		for (int line = 0; line < params; line++)
		{
			std::string tmpstring = tempdata[line];
			TrimString(tmpstring);
			if (tmpstring.find("-", 1) > 0)
			{
				int start, end;
				//found a range marker
				std::string str1 = tmpstring.substr(0, tmpstring.find("-", 1));
				std::string str2 = tmpstring.substr(tmpstring.find("-", 1) + 1);
				TrimString(str1);
				TrimString(str2);
				if (!IsNumeric(str1.c_str(), start) || !IsNumeric(str2.c_str(), end))
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
				std::string str = tempdata[line];
				TrimString(str);
				if (!IsNumeric(str.c_str(), data))
					return ScriptError("Invalid value");
				floor->AddGroupFloor(data);
			}
		}
	}

	//calculate altitude
	if (FloorCheck == 3)
	{
		FloorCheck = 0;
		if (floor->CalculateAltitude() == false)
			return ScriptError();
	}

	//Print command
	if (SetCaseCopy(LineData.substr(0, 5), false) == "print")
	{
		//calculate inline math
		buffer = Calc(LineData.substr(6).c_str());

		//print line
		skyscraper->Report(buffer);
	}

	//Exit command
	if (SetCaseCopy(LineData.substr(0, 4), false) == "exit")
	{
		if (RangeL != RangeH)
			LineData = "<endfloors>";
		else
			LineData = "<endfloor>";
	}

	//Break command
	if (SetCaseCopy(LineData.substr(0, 7), false) == "<break>")
		return sBreak;

	//AddFloor command
	if (SetCaseCopy(LineData.substr(0, 9), false) == "addfloor ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 9);

		if (params != 12)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 10; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		//create floor
		StoreCommand(floor->AddFloor(tempdata[0].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), Ogre::StringConverter::parseBool(tempdata[11])));
	}

	//AddShaftFloor command
	if (SetCaseCopy(LineData.substr(0, 13), false) == "addshaftfloor")
	{
		//get data
		int params = SplitData(LineData.c_str(), 14);

		if (params != 12)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 11; i++)
		{
			if (i == 1)
				i = 3; //skip non-numeric parameters
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		//create floor
		if (Simcore->GetShaft(atoi(tempdata[0].c_str())))
			StoreCommand(Simcore->GetShaft(atoi(tempdata[0].c_str()))->AddFloor(Current, tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str())));
		else
			return ScriptError("Invalid shaft");
	}

	//AddStairsFloor command
	if (SetCaseCopy(LineData.substr(0, 14), false) == "addstairsfloor")
	{
		//get data
		int params = SplitData(LineData.c_str(), 15);

		if (params != 12)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 11; i++)
		{
			if (i == 1)
				i = 3; //skip non-numeric parameters
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		//create floor
		if (Simcore->GetStairs(atoi(tempdata[0].c_str())))
			StoreCommand(Simcore->GetStairs(atoi(tempdata[0].c_str()))->AddFloor(Current, tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str())));
		else
			return ScriptError("Invalid stairwell");
	}

	//AddInterFloorFloor command
	if (SetCaseCopy(LineData.substr(0, 18), false) == "addinterfloorfloor")
	{
		//get data
		int params = SplitData(LineData.c_str(), 19);

		if (params != 11)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 10; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		//create floor
		StoreCommand(floor->AddInterfloorFloor(tempdata[0].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str())));
	}

	//AddWall command
	if (SetCaseCopy(LineData.substr(0, 7), false) == "addwall")
	{
		//get data
		int params = SplitData(LineData.c_str(), 8);

		if (params != 14)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 12; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		//create wall
		StoreCommand(floor->AddWall(tempdata[0].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), Ogre::StringConverter::parseBool(tempdata[13])));
	}

	//AddShaftWall command
	if (SetCaseCopy(LineData.substr(0, 12), false) == "addshaftwall")
	{
		//get data
		int params = SplitData(LineData.c_str(), 13);

		if (params != 14)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 13; i++)
		{
			if (i == 1)
				i = 3; //skip non-numeric parameters
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		//create wall
		if (Simcore->GetShaft(atoi(tempdata[0].c_str())))
			StoreCommand(Simcore->GetShaft(atoi(tempdata[0].c_str()))->AddWall(Current, tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str())));
		else
			return ScriptError("Invalid shaft");
	}

	//AddStairsWall command
	if (SetCaseCopy(LineData.substr(0, 13), false) == "addstairswall")
	{
		//get data
		int params = SplitData(LineData.c_str(), 14);

		if (params != 14)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 13; i++)
		{
			if (i == 1)
				i = 3; //skip non-numeric parameters
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		//create wall
		if (Simcore->GetStairs(atoi(tempdata[0].c_str())))
			StoreCommand(Simcore->GetStairs(atoi(tempdata[0].c_str()))->AddWall(Current, tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str())));
		else
			return ScriptError("Invalid stairwell");
	}

	//AddInterFloorWall command
	if (SetCaseCopy(LineData.substr(0, 17), false) == "addinterfloorwall")
	{
		//get data
		int params = SplitData(LineData.c_str(), 18);

		if (params != 13)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 12; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		//create wall
		StoreCommand(floor->AddInterfloorWall(tempdata[0].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str())));
	}

	//ColumnWallBox command
	if (SetCaseCopy(LineData.substr(0, 14), false) == "columnwallbox ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 14);

		if (params != 14)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 9; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		StoreCommand(floor->ColumnWallBox(tempdata[0].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), Ogre::StringConverter::parseBool(tempdata[10]), Ogre::StringConverter::parseBool(tempdata[11]), Ogre::StringConverter::parseBool(tempdata[12]), Ogre::StringConverter::parseBool(tempdata[13])));
	}

	//ColumnWallBox2 command
	if (SetCaseCopy(LineData.substr(0, 14), false) == "columnwallbox2")
	{
		//get data
		int params = SplitData(LineData.c_str(), 15);

		if (params != 14)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 9; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		StoreCommand(floor->ColumnWallBox2(tempdata[0].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), Ogre::StringConverter::parseBool(tempdata[10]), Ogre::StringConverter::parseBool(tempdata[11]), Ogre::StringConverter::parseBool(tempdata[12]), Ogre::StringConverter::parseBool(tempdata[13])));
	}

	//Set command
	if (SetCaseCopy(LineData.substr(0, 4), false) == "set ")
	{
		temp1 = LineData.find("=", 0);
		if (temp1 < 0)
			return ScriptError("Syntax Error");
		std::string str = LineData.substr(4, temp1 - 5);
		TrimString(str);
		if (!IsNumeric(str.c_str(), temp3))
			return ScriptError("Invalid variable number");

		//get text after equal sign
		temp2 = GetAfterEquals(LineData.c_str());

		if (temp3 < 0 || temp3 > (int)UserVariable.size() - 1)
			return ScriptError("Invalid variable number");
		UserVariable[temp3] = Calc(temp2.c_str());
		if (Simcore->Verbose == true)
			skyscraper->Report("Variable " + std::string(_itoa(temp3, intbuffer, 10)) + " set to " + UserVariable[temp3]);
	}

	//CallButtonElevators command
	if (SetCaseCopy(LineData.substr(0, 19), false) == "callbuttonelevators")
	{
		//construct array containing floor numbers
		int params = SplitAfterEquals(LineData.c_str(), false);
		if (params == -1)
			return ScriptError("Syntax Error");

		callbutton_elevators.clear();
		callbutton_elevators.resize(params);

		for (int line = 0; line < params; line++)
		{
			int elevnumber;
			std::string str = tempdata[line];
			TrimString(str);
			if (!IsNumeric(str.c_str(), elevnumber))
				return ScriptError("Invalid elevator number");
			if (elevnumber < 1 || elevnumber > Simcore->Elevators())
				return ScriptError("Invalid elevator number");
			callbutton_elevators[line] = elevnumber;
		}
	}

	//CreateCallButtons command
	if (SetCaseCopy(LineData.substr(0, 17), false) == "createcallbuttons")
	{
		if (callbutton_elevators.size() == 0)
			return ScriptError("No elevators specified");

		//get data
		int params = SplitData(LineData.c_str(), 18);

		bool compatibility = false;
		if (params == 12)
			compatibility = true;

		if (compatibility == true)
		{
			//check numeric values
			for (int i = 3; i <= 11; i++)
			{
				if (i == 6 || i == 9) //skip non-numeric parameters
					i++;
				std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
		}
		else
		{
			if (params != 14)
				return ScriptError("Incorrect number of parameters");

			//check numeric values
			for (int i = 5; i <= 13; i++)
			{
				if (i == 8 || i == 11) //skip non-numeric parameters
					i++;
				std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
		}

		//create call button
		if (compatibility == true)
			StoreCommand(floor->AddCallButtons(callbutton_elevators, tempdata[0].c_str(), tempdata[1].c_str(), tempdata[1].c_str(), tempdata[2].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), tempdata[6].c_str(), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), Ogre::StringConverter::parseBool(tempdata[9]), atof(tempdata[10].c_str()), atof(tempdata[11].c_str())));
		else
			StoreCommand(floor->AddCallButtons(callbutton_elevators, tempdata[0].c_str(), tempdata[1].c_str(), tempdata[2].c_str(), tempdata[3].c_str(), tempdata[4].c_str(), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), tempdata[8].c_str(), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), Ogre::StringConverter::parseBool(tempdata[11]), atof(tempdata[12].c_str()), atof(tempdata[13].c_str())));
	}

	//AddStairs command
	if (SetCaseCopy(LineData.substr(0, 10), false) == "addstairs ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 10);

		if (params != 13)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 12; i++)
		{
			if (i == 1)
				i = 4; //skip non-numeric parameters
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		//create stairs
		if (Simcore->GetStairs(atoi(tempdata[0].c_str())))
			StoreCommand(Simcore->GetStairs(atoi(tempdata[0].c_str()))->AddStairs(Current, tempdata[1].c_str(), tempdata[2].c_str(), tempdata[3].c_str(), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atoi(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str())));
		else
			return ScriptError("Invalid stairwell");
	}

	//AddDoor command
	if (SetCaseCopy(LineData.substr(0, 8), false) == "adddoor ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 8);

		if (params < 10 || params == 11 || params > 14)
			return ScriptError("Incorrect number of parameters");

		int compat = 0;

		//check numeric values
		if (params == 10)
		{
			for (int i = 1; i <= 9; i++)
			{
				std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
			compat = 1;
		}
		if (params == 12)
		{
			for (int i = 3; i <= 11; i++)
			{
				std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
			compat = 2;
		}
		if (params == 13)
		{
			for (int i = 3; i <= 12; i++)
			{
				std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
			compat = 3;
		}
		if (params == 14)
		{
			for (int i = 4; i <= 13; i++)
			{
				std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
		}

		//check to see if file exists
		if (compat != 1)
		{
			CheckFile(std::string("data/" + tempdata[0]).c_str());
			CheckFile(std::string("data/" + tempdata[1]).c_str());
		}

		//create door
		if (compat == 1)
			StoreCommand(floor->AddDoor("", "", false, tempdata[0].c_str(), atof(tempdata[1].c_str()), atoi(tempdata[2].c_str()), 0, atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str())));
		if (compat == 2)
			StoreCommand(floor->AddDoor(tempdata[0].c_str(), tempdata[1].c_str(), false, tempdata[2].c_str(), atof(tempdata[3].c_str()), atoi(tempdata[4].c_str()), 0, atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str())));
		if (compat == 3)
			StoreCommand(floor->AddDoor(tempdata[0].c_str(), tempdata[1].c_str(), false, tempdata[2].c_str(), atof(tempdata[3].c_str()), atoi(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str())));
		if (compat == 0)
			StoreCommand(floor->AddDoor(tempdata[0].c_str(), tempdata[1].c_str(), Ogre::StringConverter::parseBool(tempdata[2]), tempdata[3].c_str(), atof(tempdata[4].c_str()), atoi(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str())));
	}

	//AddStairsDoor command
	if (SetCaseCopy(LineData.substr(0, 14), false) == "addstairsdoor ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 14);

		if (params < 11 || params > 15)
			return ScriptError("Incorrect number of parameters");

		int compat = 0;

		//check numeric values
		if (params == 11)
		{
			for (int i = 0; i <= 10; i++)
			{
				if (i == 1)
					i = 2; //skip non-numeric parameters
				std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
			compat = 1;
		}
		//check numeric values
		if (params == 13)
		{
			for (int i = 0; i <= 12; i++)
			{
				if (i == 1)
					i = 4; //skip non-numeric parameters
				std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
			compat = 2;
		}
		//check numeric values
		if (params == 14)
		{
			for (int i = 0; i <= 13; i++)
			{
				if (i == 1)
					i = 4; //skip non-numeric parameters
				std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
			compat = 3;
		}
		//check numeric values
		if (params == 15)
		{
			for (int i = 0; i <= 14; i++)
			{
				if (i == 1)
					i = 5; //skip non-numeric parameters
				std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
		}

		//check to see if file exists
		if (compat != 1)
		{
			CheckFile(std::string("data/" + tempdata[1]).c_str());
			CheckFile(std::string("data/" + tempdata[2]).c_str());
		}

		//create door
		if (Simcore->GetStairs(atoi(tempdata[0].c_str())))
		{
			if (compat == 1)
				StoreCommand(Simcore->GetStairs(atoi(tempdata[0].c_str()))->AddDoor(Current, "", "", false, tempdata[1].c_str(), atof(tempdata[2].c_str()), atoi(tempdata[3].c_str()), 0, atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str())));
			if (compat == 2)
				StoreCommand(Simcore->GetStairs(atoi(tempdata[0].c_str()))->AddDoor(Current, tempdata[1].c_str(), tempdata[2].c_str(), false, tempdata[3].c_str(), atof(tempdata[4].c_str()), atoi(tempdata[5].c_str()), 0, atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str())));
			if (compat == 3)
				StoreCommand(Simcore->GetStairs(atoi(tempdata[0].c_str()))->AddDoor(Current, tempdata[1].c_str(), tempdata[2].c_str(), false, tempdata[3].c_str(), atof(tempdata[4].c_str()), atoi(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str())));
			if (compat == 0)
				StoreCommand(Simcore->GetStairs(atoi(tempdata[0].c_str()))->AddDoor(Current, tempdata[1].c_str(), tempdata[2].c_str(), Ogre::StringConverter::parseBool(tempdata[3]), tempdata[4].c_str(), atof(tempdata[5].c_str()), atoi(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()), atof(tempdata[14].c_str())));
		}
		else
			return ScriptError("Invalid stairwell");
	}

	//AddDirectionalIndicator command
	if (SetCaseCopy(LineData.substr(0, 23), false) == "adddirectionalindicator")
	{
		//get data
		int params = SplitData(LineData.c_str(), 24);

		if (params < 18 || params > 19)
			return ScriptError("Incorrect number of parameters");

		bool compatibility = false;

		//check numeric values
		if (params == 18)
		{
			for (int i = 0; i <= 17; i++)
			{
				if (i == 1)
					i = 9;
				if (i == 12)
					i = 13;
				if (i == 15)
					i = 16;
				std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
			compatibility = true;
		}
		if (params == 19)
		{
			for (int i = 0; i <= 18; i++)
			{
				if (i == 1)
					i = 10;
				if (i == 13)
					i = 14;
				if (i == 16)
					i = 17;
				std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
		}

		if (!Simcore->GetElevator(atoi(tempdata[0].c_str())))
			return ScriptError("Invalid elevator");

		if (compatibility == true)
			StoreCommand(floor->AddDirectionalIndicator(atoi(tempdata[0].c_str()), Ogre::StringConverter::parseBool(tempdata[1]), false, Ogre::StringConverter::parseBool(tempdata[2]), Ogre::StringConverter::parseBool(tempdata[3]), tempdata[4].c_str(), tempdata[5].c_str(), tempdata[6].c_str(), tempdata[7].c_str(), tempdata[8].c_str(), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), tempdata[12].c_str(), atof(tempdata[13].c_str()), atof(tempdata[14].c_str()), Ogre::StringConverter::parseBool(tempdata[15]), atof(tempdata[16].c_str()), atof(tempdata[17].c_str())));
		else
			StoreCommand(floor->AddDirectionalIndicator(atoi(tempdata[0].c_str()), Ogre::StringConverter::parseBool(tempdata[1]), Ogre::StringConverter::parseBool(tempdata[2]), Ogre::StringConverter::parseBool(tempdata[3]), Ogre::StringConverter::parseBool(tempdata[4]), tempdata[5].c_str(), tempdata[6].c_str(), tempdata[7].c_str(), tempdata[8].c_str(), tempdata[9].c_str(), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), tempdata[13].c_str(), atof(tempdata[14].c_str()), atof(tempdata[15].c_str()), Ogre::StringConverter::parseBool(tempdata[16]), atof(tempdata[17].c_str()), atof(tempdata[18].c_str())));
	}

	//AddShaftDoor command
	if (SetCaseCopy(LineData.substr(0, 13), false) == "addshaftdoor ")
	{
		//exit if the SetShaftDoors command was never used
		if (setshaftdoors == false)
			return ScriptError("SetShaftDoors must be used before AddShaftDoor");

		//get data
		int params = SplitData(LineData.c_str(), 13);

		if (params < 5 || params > 6)
			return ScriptError("Incorrect number of parameters");

		bool compat = false;
		if (params == 5)
			compat = true; //1.4 compatibility mode

		//check numeric values
		if (compat == false)
		{
			for (int i = 0; i <= 5; i++)
			{
				if (i == 2)
					i = 4;
				std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
		}
		else
		{
			for (int i = 0; i <= 4; i++)
			{
				if (i == 2)
					i = 3;
				std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
		}

		if (!Simcore->GetElevator(atoi(tempdata[0].c_str())))
			return ScriptError("Invalid elevator");

		if (compat == false)
			StoreCommand(Simcore->GetElevator(atoi(tempdata[0].c_str()))->AddShaftDoor(Current, atoi(tempdata[1].c_str()), tempdata[2].c_str(), tempdata[3].c_str(), atof(tempdata[4].c_str()), atof(tempdata[5].c_str())));
		else
			StoreCommand(Simcore->GetElevator(atoi(tempdata[0].c_str()))->AddShaftDoor(Current, atoi(tempdata[1].c_str()), tempdata[2].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str())));
	}

	//AddFloorIndicator command
	if (SetCaseCopy(LineData.substr(0, 17), false) == "addfloorindicator")
	{
		//get data
		int params = SplitData(LineData.c_str(), 18);

		if (params < 8 || params > 9)
			return ScriptError("Incorrect number of parameters");

		bool compat = false;
		if (params == 8)
			compat = true; //1.4 compatibility mode

		//check numeric values
		if (compat == false)
		{
			for (int i = 0; i <= 8; i++)
			{
				if (i == 1)
					i = 4;
				std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
		}
		else
		{
			for (int i = 0; i <= 7; i++)
			{
				if (i == 1)
					i = 3;
				std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
		}

		if (compat == false)
			StoreCommand(floor->AddFloorIndicator(atoi(tempdata[0].c_str()), Ogre::StringConverter::parseBool(tempdata[1]), tempdata[2].c_str(), tempdata[3].c_str(), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str())));
		else
			StoreCommand(floor->AddFloorIndicator(atoi(tempdata[0].c_str()), Ogre::StringConverter::parseBool(tempdata[1]), "Button", tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str())));
	}

	//AddFillerWalls command
	if (SetCaseCopy(LineData.substr(0, 14), false) == "addfillerwalls")
	{
		//get data
		int params = SplitData(LineData.c_str(), 15);

		if (params != 10)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 9; i++)
		{
			if (i == 7)
				i = 8;
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		floor->AddFillerWalls(tempdata[0].c_str(), atof(tempdata[1].c_str()), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), Ogre::StringConverter::parseBool(tempdata[7]), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()));
	}

	//AddSound
	if (SetCaseCopy(LineData.substr(0, 8), false) == "addsound")
	{
		//get data
		int params = SplitData(LineData.c_str(), 9);

		if (params != 5 && params != 13)
			return ScriptError("Incorrect number of parameters");

		bool partial = false;
		if (params == 5)
			partial = true;

		//check numeric values
		if (partial == true)
		{
			for (int i = 2; i <= 4; i++)
			{
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
		}
		else
		{
			for (int i = 2; i <= 12; i++)
			{
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
		}

		//check to see if file exists
		CheckFile(std::string("data/" + tempdata[1]).c_str());

		if (partial == true)
			StoreCommand(floor->AddSound(tempdata[0].c_str(), tempdata[1].c_str(), Ogre::Vector3(atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()))));
		else
			StoreCommand(floor->AddSound(tempdata[0].c_str(), tempdata[1].c_str(), Ogre::Vector3(atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str())), atoi(tempdata[5].c_str()), atoi(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), Ogre::Vector3(atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str())))); 
	}

	//AddShaftDoorComponent command
	if (SetCaseCopy(LineData.substr(0, 21), false) == "addshaftdoorcomponent")
	{
		//get data
		int params = SplitData(LineData.c_str(), 22);

		if (params < 18 || params > 19)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		bool compat = false;
		if (params == 18)
			compat = true;

		for (int i = 0; i <= params - 1; i++)
		{
			if (i == 2)
				i = 5;
			if (i == 6)
				i++;
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		Elevator *elev = Simcore->GetElevator(atoi(tempdata[0].c_str()));
		if (!elev)
			return ScriptError("Invalid elevator");

		if (compat == true)
			StoreCommand(elev->AddShaftDoorComponent(atoi(tempdata[1].c_str()), Current, tempdata[2].c_str(), tempdata[3].c_str(), tempdata[4].c_str(), atof(tempdata[5].c_str()), tempdata[6].c_str(), atof(tempdata[7].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()), atof(tempdata[14].c_str()), atof(tempdata[15].c_str()), atof(tempdata[16].c_str()), atof(tempdata[17].c_str())));
		else
			StoreCommand(elev->AddShaftDoorComponent(atoi(tempdata[1].c_str()), Current, tempdata[2].c_str(), tempdata[3].c_str(), tempdata[4].c_str(), atof(tempdata[5].c_str()), tempdata[6].c_str(), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()), atof(tempdata[14].c_str()), atof(tempdata[15].c_str()), atof(tempdata[16].c_str()), atof(tempdata[17].c_str()), atof(tempdata[18].c_str())));
	}

	//FinishShaftDoor command
	if (SetCaseCopy(LineData.substr(0, 16), false) == "finishshaftdoor ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 16);

		if (params < 2)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 1; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		Elevator *elev = Simcore->GetElevator(atoi(tempdata[0].c_str()));
		if (!elev)
			return ScriptError("Invalid elevator");

		StoreCommand(elev->FinishShaftDoor(atoi(tempdata[1].c_str()), Current));
	}

	//AddModel command
	if (SetCaseCopy(LineData.substr(0, 8), false) == "addmodel")
	{
		//get data
		int params = SplitData(LineData.c_str(), 9);

		if (params != 14)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 13; i++)
		{
			if (i == 10)
				i++;
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		//check to see if file exists
		CheckFile(std::string("data/" + tempdata[1]).c_str());

		//create model
		StoreCommand(floor->AddModel(tempdata[0].c_str(), tempdata[1].c_str(), Ogre::Vector3(atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str())), Ogre::Vector3(atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str())), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), Ogre::StringConverter::parseBool(tempdata[10]), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str())));
	}

	//AddStairsModel command
	if (SetCaseCopy(LineData.substr(0, 14), false) == "addstairsmodel")
	{
		//get data
		int params = SplitData(LineData.c_str(), 15);

		if (params != 15)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 14; i++)
		{
			if (i == 11)
				i++;
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		//check to see if file exists
		CheckFile(std::string("data/" + tempdata[1]).c_str());

		//create model
		if (Simcore->GetStairs(atoi(tempdata[0].c_str())))
			StoreCommand(Simcore->GetStairs(atoi(tempdata[0].c_str()))->AddModel(Current, tempdata[1].c_str(), tempdata[2].c_str(), Ogre::Vector3(atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str())), Ogre::Vector3(atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str())), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), Ogre::StringConverter::parseBool(tempdata[11]), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()), atof(tempdata[14].c_str())));
		else
			return ScriptError("Invalid stairwell");
	}

	//AddShaftModel command
	if (SetCaseCopy(LineData.substr(0, 13), false) == "addshaftmodel")
	{
		//get data
		int params = SplitData(LineData.c_str(), 14);

		if (params != 15)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 14; i++)
		{
			if (i == 11)
				i++;
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		//check to see if file exists
		CheckFile(std::string("data/" + tempdata[1]).c_str());

		//create model
		if (Simcore->GetShaft(atoi(tempdata[0].c_str())))
			StoreCommand(Simcore->GetShaft(atoi(tempdata[0].c_str()))->AddModel(Current, tempdata[1].c_str(), tempdata[2].c_str(), Ogre::Vector3(atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str())), Ogre::Vector3(atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str())), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), Ogre::StringConverter::parseBool(tempdata[11]), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()), atof(tempdata[14].c_str())));
		else
			return ScriptError("Invalid shaft");
	}

	//Cut command
	if (SetCaseCopy(LineData.substr(0, 4), false) == "cut ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 4);

		if (params != 8)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 5; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		//perform cut on floor
		Simcore->GetFloor(Current)->Cut(Ogre::Vector3(atof(tempdata[0].c_str()), atof(tempdata[1].c_str()), atof(tempdata[2].c_str())), Ogre::Vector3(atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str())), Ogre::StringConverter::parseBool(tempdata[6]), Ogre::StringConverter::parseBool(tempdata[7]), false);
	}

	//CutAll command
	if (SetCaseCopy(LineData.substr(0, 6), false) == "cutall")
	{
		//get data
		int params = SplitData(LineData.c_str(), 7);

		if (params != 8)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 5; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		//perform cut on all objects related to the current floor
		Simcore->GetFloor(Current)->CutAll(Ogre::Vector3(atof(tempdata[0].c_str()), atof(tempdata[1].c_str()), atof(tempdata[2].c_str())), Ogre::Vector3(atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str())), Ogre::StringConverter::parseBool(tempdata[6]), Ogre::StringConverter::parseBool(tempdata[7]));
	}

	//handle floor range
	if (RangeL != RangeH && SetCaseCopy(LineData.substr(0, 9), false) == "<endfloor")
	{
		if (RangeL < RangeH)
		{
			if (Current < RangeH)
			{
				Current++;
				line = RangeStart;  //loop back
				return sNextLine;
			}
			else
			{
				Section = 0; //break out of loop
				Context = "None";
				RangeL = 0;
				RangeH = 0;
				return sNextLine;
			}
		}
		else
		{
			if (Current > RangeH)
			{
				Current--;
				line = RangeStart; //loop back
				return sNextLine;
			}
			else
			{
				Section = 0; //break out of loop
				Context = "None";
				RangeL = 0;
				RangeH = 0;
				return sNextLine;
			}
		}
	}
	return 0;
}

int ScriptProcessor::ProcElevators()
{
	//Process elevators

	//create elevator if not created already
	Simcore->NewElevator(Current);

	//replace variables with actual values
	buffer = Ogre::StringConverter::toString(Current);
	ReplaceAll(LineData, "%elevator%", buffer.c_str());

	//IF statement
	if (SetCaseCopy(LineData.substr(0, 2), false) == "if")
	{
		temp1 = LineData.find("[", 0);
		temp3 = LineData.find("]", 0);
		if (temp1 + temp3 > 0)
			temp2 = LineData.substr(temp1 + 1, temp3 - temp1 - 1);
		else
			temp2 = "";
		TrimString(temp2);
		if (IfProc(temp2.c_str()) == true)
		{
			std::string str = LineData.substr(temp3 + 1);
			TrimString(str);
			LineData = str; //trim off IF statement
			return sCheckFloors;
		}
		else
			return sNextLine; //skip line
	}

	//process functions
	if (FunctionProc() == true)
		return sNextLine;

	//get text after equal sign
	int temp2check = LineData.find("=", 0);
	temp2 = GetAfterEquals(LineData.c_str());

	Elevator *elev = Simcore->GetElevator(Current);

	//parameters
	if (SetCaseCopy(LineData.substr(0, 4), false) == "name")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		TrimString(temp2);
		elev->Name = temp2;
	}
	if (SetCaseCopy(LineData.substr(0, 5), false) == "speed")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (!IsNumeric(temp2.c_str(), elev->ElevatorSpeed))
			return ScriptError("Invalid value");
	}
	if (SetCaseCopy(LineData.substr(0, 12), false) == "acceleration")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (!IsNumeric(temp2.c_str(), elev->Acceleration))
			return ScriptError("Invalid value");
	}
	if (SetCaseCopy(LineData.substr(0, 12), false) == "deceleration")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (!IsNumeric(temp2.c_str(), elev->Deceleration))
			return ScriptError("Invalid value");
	}
	if (SetCaseCopy(LineData.substr(0, 9), false) == "openspeed")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		std::string str = LineData.substr(9, LineData.find("=", 0) - 9);
		TrimString(str);
		if (!IsNumeric(str.c_str(), temp3))
			return ScriptError("No door specified");
		if (temp3 == 0 || temp3 > elev->NumDoors)
			return ScriptError("Invalid door number");
		if (!IsNumeric(temp2.c_str(), elev->GetDoor(temp3)->OpenSpeed))
			return ScriptError("Invalid value");
	}
	if (SetCaseCopy(LineData.substr(0, 5), false) == "doors")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (!IsNumeric(temp2.c_str(), elev->NumDoors))
			return ScriptError("Invalid value");
	}
	if (SetCaseCopy(LineData.substr(0, 9), false) == "acceljerk")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (!IsNumeric(temp2.c_str(), elev->AccelJerk))
			return ScriptError("Invalid value");
	}
	if (SetCaseCopy(LineData.substr(0, 9), false) == "deceljerk")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (!IsNumeric(temp2.c_str(), elev->DecelJerk))
			return ScriptError("Invalid value");
	}
	if (SetCaseCopy(LineData.substr(0, 14), false) == "servicedfloors")
	{
		//copy string listing of serviced floors into array
		int params = SplitAfterEquals(LineData.c_str(), false);
		if (params == -1)
			return ScriptError("Syntax Error");

		for (int line = 0; line < params; line++)
		{
			std::string tmpstring = tempdata[line];
			TrimString(tmpstring);
			if (tmpstring.find("-", 1) > 0)
			{
				int start, end;
				//found a range marker
				std::string str1 = tmpstring.substr(0, tmpstring.find("-", 1));
				std::string str2 = tmpstring.substr(tmpstring.find("-", 1) + 1);
				TrimString(str1);
				TrimString(str2);
				if (!IsNumeric(str1.c_str(), start) || !IsNumeric(str2.c_str(), end))
					return ScriptError("Invalid value");
				if (end < start)
				{
					int temp = start;
					start = end;
					end = temp;
				}

				for (int k = start; k <= end; k++)
				{
					if (!elev->AddServicedFloor(k))
						return ScriptError();
				}
			}
			else
			{
				int data;
				std::string str = tempdata[line];
				TrimString(str);
				if (!IsNumeric(str.c_str(), data))
					return ScriptError("Invalid value");
				if (!elev->AddServicedFloor(data))
					return ScriptError();
			}
		}
	}
	if (SetCaseCopy(LineData.substr(0, 13), false) == "displayfloors")
	{
		//copy string listing of serviced floors into array
		int params = SplitAfterEquals(LineData.c_str(), false);
		if (params == -1)
			return ScriptError("Syntax Error");

		for (int line = 0; line < params; line++)
		{
			std::string tmpstring = tempdata[line];
			TrimString(tmpstring);
			if (tmpstring.find("-", 1) > 0)
			{
				int start, end;
				//found a range marker
				std::string str1 = tmpstring.substr(0, tmpstring.find("-", 1));
				std::string str2 = tmpstring.substr(tmpstring.find("-", 1) + 1);
				TrimString(str1);
				TrimString(str2);
				if (!IsNumeric(str1.c_str(), start) || !IsNumeric(str2.c_str(), end))
					return ScriptError("Invalid value");
				if (end < start)
				{
					int temp = start;
					start = end;
					end = temp;
				}

				for (int k = start; k <= end; k++)
				{
					elev->AddDisplayFloor(k);
				}
			}
			else
			{
				int data;
				std::string str = tempdata[line];
				TrimString(str);
				if (!IsNumeric(str.c_str(), data))
					return ScriptError("Invalid value");
				elev->AddDisplayFloor(data);
			}
		}
	}
	if (SetCaseCopy(LineData.substr(0, 13), false) == "assignedshaft")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (!IsNumeric(temp2.c_str(), elev->AssignedShaft))
			return ScriptError("Invalid value");
	}
	if (SetCaseCopy(LineData.substr(0, 9), false) == "doortimer")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		std::string str = LineData.substr(9, LineData.find("=", 0) - 9);
		TrimString(str);
		if (!IsNumeric(str.c_str(), temp3))
			return ScriptError("No door specified");
		if (temp3 == 0 || temp3 > elev->NumDoors)
			return ScriptError("Invalid door number");
		if (!IsNumeric(temp2.c_str(), elev->GetDoor(temp3)->DoorTimer))
			return ScriptError("Invalid value");
	}
	if (SetCaseCopy(LineData.substr(0, 10), false) == "quickclose")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		std::string str = LineData.substr(10, LineData.find("=", 0) - 10);
		TrimString(str);
		if (!IsNumeric(str.c_str(), temp3))
			return ScriptError("No door specified");
		if (temp3 == 0 || temp3 > elev->NumDoors)
			return ScriptError("Invalid door number");
		if (!IsNumeric(temp2.c_str(), elev->GetDoor(temp3)->QuickClose))
			return ScriptError("Invalid value");
	}
	if (SetCaseCopy(LineData.substr(0, 10), false) == "nudgetimer")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		std::string str = LineData.substr(10, LineData.find("=", 0) - 10);
		TrimString(str);
		if (!IsNumeric(str.c_str(), temp3))
			return ScriptError("No door specified");
		if (temp3 == 0 || temp3 > elev->NumDoors)
			return ScriptError("Invalid door number");
		if (!IsNumeric(temp2.c_str(), elev->GetDoor(temp3)->NudgeTimer))
			return ScriptError("Invalid value");
	}
	if (SetCaseCopy(LineData.substr(0, 9), false) == "slowspeed")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		std::string str = LineData.substr(9, LineData.find("=", 0) - 9);
		TrimString(str);
		if (!IsNumeric(str.c_str(), temp3))
			return ScriptError("No door specified");
		if (temp3 == 0 || temp3 > elev->NumDoors)
			return ScriptError("Invalid door number");
		if (!IsNumeric(temp2.c_str(), elev->GetDoor(temp3)->SlowSpeed))
			return ScriptError("Invalid value");
	}
	if (SetCaseCopy(LineData.substr(0, 11), false) == "manualspeed")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		std::string str = LineData.substr(11, LineData.find("=", 0) - 11);
		TrimString(str);
		if (!IsNumeric(str.c_str(), temp3))
			return ScriptError("No door specified");
		if (temp3 == 0 || temp3 > elev->NumDoors)
			return ScriptError("Invalid door number");
		if (!IsNumeric(temp2.c_str(), elev->GetDoor(temp3)->ManualSpeed))
			return ScriptError("Invalid value");
	}
	if (SetCaseCopy(LineData.substr(0, 9), false) == "opensound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		std::string str = LineData.substr(9, LineData.find("=", 0) - 9);
		TrimString(str);
		if (!IsNumeric(str.c_str(), temp3))
			return ScriptError("No door specified");
		if (temp3 == 0 || temp3 > elev->NumDoors)
			return ScriptError("Invalid door number");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->GetDoor(temp3)->OpenSound = temp2;
	}
	if (SetCaseCopy(LineData.substr(0, 10), false) == "closesound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		std::string str = LineData.substr(10, LineData.find("=", 0) - 10);
		TrimString(str);
		if (!IsNumeric(str.c_str(), temp3))
			return ScriptError("No door specified");
		if (temp3 == 0 || temp3 > elev->NumDoors)
			return ScriptError("Invalid door number");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->GetDoor(temp3)->CloseSound = temp2;
	}
	if (SetCaseCopy(LineData.substr(0, 10), false) == "nudgesound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		std::string str = LineData.substr(10, LineData.find("=", 0) - 10);
		TrimString(str);
		if (!IsNumeric(str.c_str(), temp3))
			return ScriptError("No door specified");
		if (temp3 == 0 || temp3 > elev->NumDoors)
			return ScriptError("Invalid door number");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->GetDoor(temp3)->NudgeSound = temp2;
	}
	if (SetCaseCopy(LineData.substr(0, 10), false) == "startsound")
	{
		//backwards compatibility
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->CarStartSound = temp2;
		//turn off motor sounds
		elev->MotorStartSound = "";
		elev->MotorRunSound = "";
		elev->MotorStopSound = "";
		elev->MotorIdleSound = "";
	}
	if (SetCaseCopy(LineData.substr(0, 9), false) == "movesound")
	{
		//backwards compatibility
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->CarMoveSound = temp2;
		//turn off motor sounds
		elev->MotorStartSound = "";
		elev->MotorRunSound = "";
		elev->MotorStopSound = "";
		elev->MotorIdleSound = "";
	}
	if (SetCaseCopy(LineData.substr(0, 9), false) == "stopsound")
	{
		//backwards compatibility
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->CarStopSound = temp2;
		//turn off motor sounds
		elev->MotorStartSound = "";
		elev->MotorRunSound = "";
		elev->MotorStopSound = "";
		elev->MotorIdleSound = "";
	}
	if (SetCaseCopy(LineData.substr(0, 9), false) == "idlesound")
	{
		//backwards compatibility
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->CarIdleSound = temp2;
		//turn off motor sounds
		elev->MotorStartSound = "";
		elev->MotorRunSound = "";
		elev->MotorStopSound = "";
		elev->MotorIdleSound = "";
	}
	if (SetCaseCopy(LineData.substr(0, 13), false) == "carstartsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->CarStartSound = temp2;
	}
	if (SetCaseCopy(LineData.substr(0, 12), false) == "carmovesound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->CarMoveSound = temp2;
	}
	if (SetCaseCopy(LineData.substr(0, 12), false) == "carstopsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->CarStopSound = temp2;
	}
	if (SetCaseCopy(LineData.substr(0, 12), false) == "caridlesound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->CarIdleSound = temp2;
	}
	if (SetCaseCopy(LineData.substr(0, 15), false) == "motorstartsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->MotorStartSound = temp2;
	}
	if (SetCaseCopy(LineData.substr(0, 13), false) == "motorrunsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->MotorRunSound = temp2;
	}
	if (SetCaseCopy(LineData.substr(0, 14), false) == "motorstopsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->MotorStopSound = temp2;
	}
	if (SetCaseCopy(LineData.substr(0, 10), false) == "chimesound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		std::string str = LineData.substr(10, LineData.find("=", 0) - 10);
		TrimString(str);
		if (!IsNumeric(str.c_str(), temp3))
			return ScriptError("No door specified");
		if (temp3 == 0 || temp3 > elev->NumDoors)
			return ScriptError("Invalid door number");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->GetDoor(temp3)->UpChimeSound = temp2;
		elev->GetDoor(temp3)->DownChimeSound = temp2;
	}
	if (SetCaseCopy(LineData.substr(0, 12), false) == "upchimesound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		std::string str = LineData.substr(12, LineData.find("=", 0) - 12);
		TrimString(str);
		if (!IsNumeric(str.c_str(), temp3))
			return ScriptError("No door specified");
		if (temp3 == 0 || temp3 > elev->NumDoors)
			return ScriptError("Invalid door number");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->GetDoor(temp3)->UpChimeSound = temp2;
	}
	if (SetCaseCopy(LineData.substr(0, 14), false) == "downchimesound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		std::string str = LineData.substr(14, LineData.find("=", 0) - 14);
		TrimString(str);
		if (!IsNumeric(str.c_str(), temp3))
			return ScriptError("No door specified");
		if (temp3 == 0 || temp3 > elev->NumDoors)
			return ScriptError("Invalid door number");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->GetDoor(temp3)->DownChimeSound = temp2;
	}
	if (SetCaseCopy(LineData.substr(0, 10), false) == "alarmsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->AlarmSound = temp2;
	}
	if (SetCaseCopy(LineData.substr(0, 14), false) == "alarmsoundstop")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->AlarmSoundStop = temp2;
	}
	if (SetCaseCopy(LineData.substr(0, 9), false) == "beepsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->SetBeepSound(temp2.c_str());
	}
	if (SetCaseCopy(LineData.substr(0, 10), false) == "floorsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->SetFloorSound(temp2.c_str());
	}
	if (SetCaseCopy(LineData.substr(0, 9), false) == "upmessage")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->SetMessageSound(true, temp2.c_str());
	}
	if (SetCaseCopy(LineData.substr(0, 11), false) == "downmessage")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->SetMessageSound(false, temp2.c_str());
	}
	if (SetCaseCopy(LineData.substr(0, 6), false) == "music ")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->Music = temp2;
	}
	if (SetCaseCopy(LineData.substr(0, 8), false) == "musicon ")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		elev->MusicOn = Ogre::StringConverter::parseBool(temp2);
	}
	if (SetCaseCopy(LineData.substr(0, 11), false) == "musiconmove")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		elev->MusicOnMove = Ogre::StringConverter::parseBool(temp2);
	}
	if (SetCaseCopy(LineData.substr(0, 13), false) == "floorskiptext")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->SetFloorSkipText(temp2.c_str());
	}
	if (SetCaseCopy(LineData.substr(0, 11), false) == "recallfloor")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		int floortemp;
		if (!IsNumeric(temp2.c_str(), floortemp))
			return ScriptError("Invalid value");
		elev->SetRecallFloor(floortemp);
	}
	if (SetCaseCopy(LineData.substr(0, 20), false) == "alternaterecallfloor")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		int floortemp;
		if (!IsNumeric(temp2.c_str(), floortemp))
			return ScriptError("Invalid value");
		elev->SetAlternateRecallFloor(floortemp);
	}
	if (SetCaseCopy(LineData.substr(0, 8), false) == "acpfloor")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		int floortemp;
		if (!IsNumeric(temp2.c_str(), floortemp))
			return ScriptError("Invalid value");
		elev->SetACPFloor(floortemp);
	}
	if (SetCaseCopy(LineData.substr(0, 13), false) == "motorposition")
	{
		int params = SplitAfterEquals(LineData.c_str());
		if (params == -1)
			return ScriptError("Syntax Error");
		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 2; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		elev->MotorPosition = Ogre::Vector3(atof(tempdata[0].c_str()), atof(tempdata[1].c_str()), atof(tempdata[2].c_str()));
	}
	if (SetCaseCopy(LineData.substr(0, 11), false) == "queueresets")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->QueueResets = Ogre::StringConverter::parseBool(temp2);
	}
	if (SetCaseCopy(LineData.substr(0, 3), false) == "acp")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->ACP = Ogre::StringConverter::parseBool(temp2);
	}
	if (SetCaseCopy(LineData.substr(0, 6), false) == "uppeak")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->UpPeak = Ogre::StringConverter::parseBool(temp2);
	}
	if (SetCaseCopy(LineData.substr(0, 8), false) == "downpeak")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->DownPeak = Ogre::StringConverter::parseBool(temp2);
	}
	if (SetCaseCopy(LineData.substr(0, 18), false) == "independentservice")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->IndependentService = Ogre::StringConverter::parseBool(temp2);
	}
	if (SetCaseCopy(LineData.substr(0, 17), false) == "inspectionservice")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->InspectionService = Ogre::StringConverter::parseBool(temp2);
	}
	if (SetCaseCopy(LineData.substr(0, 11), false) == "fireservice1")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->FireServicePhase1 = Ogre::StringConverter::parseInt(temp2);
	}
	if (SetCaseCopy(LineData.substr(0, 11), false) == "fireservice2")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->FireServicePhase2 = Ogre::StringConverter::parseInt(temp2);
	}
	if (SetCaseCopy(LineData.substr(0, 7), false) == "parking")
	{
		int params = SplitAfterEquals(LineData.c_str());
		if (params == -1)
			return ScriptError("Syntax Error");
		if (params != 2)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 1; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		elev->ParkingFloor = atoi(tempdata[0].c_str());
		elev->ParkingDelay = atof(tempdata[1].c_str());
	}
	if (SetCaseCopy(LineData.substr(0, 13), false) == "levelingspeed")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		float leveling;
		if (!IsNumeric(temp2.c_str(), leveling))
			return ScriptError("Invalid value");
		elev->LevelingSpeed = leveling;
	}
	if (SetCaseCopy(LineData.substr(0, 14), false) == "levelingoffset")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		float leveling;
		if (!IsNumeric(temp2.c_str(), leveling))
			return ScriptError("Invalid value");
		elev->LevelingOffset = leveling;
	}
	if (SetCaseCopy(LineData.substr(0, 12), false) == "levelingopen")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		float leveling;
		if (!IsNumeric(temp2.c_str(), leveling))
			return ScriptError("Invalid value");
		elev->LevelingOpen = leveling;
	}
	if (SetCaseCopy(LineData.substr(0, 11), false) == "notifyearly")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		float notify;
		if (!IsNumeric(temp2.c_str(), notify))
			return ScriptError("Invalid value");
		elev->NotifyEarly = notify;
	}
	if (SetCaseCopy(LineData.substr(0, 14), false) == "departuredelay")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		float delay;
		if (!IsNumeric(temp2.c_str(), delay))
			return ScriptError("Invalid value");
		elev->DepartureDelay = delay;
	}
	if (SetCaseCopy(LineData.substr(0, 12), false) == "arrivaldelay")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		float delay;
		if (!IsNumeric(temp2.c_str(), delay))
			return ScriptError("Invalid value");
		elev->ArrivalDelay = delay;
	}
	if (SetCaseCopy(LineData.substr(0, 13), false) == "musicposition")
	{
		int params = SplitAfterEquals(LineData.c_str());
		if (params == -1)
			return ScriptError("Syntax Error");
		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 2; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		elev->MusicPosition = Ogre::Vector3(atof(tempdata[0].c_str()), atof(tempdata[1].c_str()), atof(tempdata[2].c_str()));
	}

	//Print command
	if (SetCaseCopy(LineData.substr(0, 5), false) == "print")
	{
		//calculate inline math
		buffer = Calc(LineData.substr(6).c_str());

		//print line
		skyscraper->Report(buffer);
	}

	if (SetCaseCopy(LineData.substr(0, 7), false) == "<break>")
		return sBreak;

	//CreateElevator command
	if (SetCaseCopy(LineData.substr(0, 14), false) == "createelevator")
	{
		//get data
		int params = SplitData(LineData.c_str(), 15);

		if (params != 4)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 3; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		Object *object = elev->CreateElevator(Ogre::StringConverter::parseBool(tempdata[0]), atof(tempdata[1].c_str()), atof(tempdata[2].c_str()), atoi(tempdata[3].c_str()));
		if (!object)
			return ScriptError();
		StoreCommand(object);
	}

	//AddFloor command
	if (SetCaseCopy(LineData.substr(0, 9), false) == "addfloor ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 9);

		if (params != 11)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 10; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		//create floor
		StoreCommand(elev->AddFloor(tempdata[0].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str())));
	}

	//AddWall command
	if (SetCaseCopy(LineData.substr(0, 7), false) == "addwall")
	{
		//get data
		int params = SplitData(LineData.c_str(), 8);

		if (params != 13)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 12; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		//create wall
		StoreCommand(elev->AddWall(tempdata[0].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str())));
	}

	//AddDoors command
	if (SetCaseCopy(LineData.substr(0, 8), false) == "adddoors")
	{
		//get data
		int params = SplitData(LineData.c_str(), 9);

		if (params < 10 || params > 11)
			return ScriptError("Incorrect number of parameters");

		bool compat = false;
		if (params == 10)
			compat = true; //1.4 compatibility mode

		//check numeric values
		if (compat == false)
		{
			for (int i = 0; i <= 10; i++)
			{
				if (i == 1)
					i = 3;
				if (i == 8)
					i = 9;
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
		}
		else
		{
			for (int i = 0; i <= 9; i++)
			{
				if (i == 1)
					i = 2;
				if (i == 7)
					i = 8;
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
		}

		if (compat == false)
			StoreCommand(elev->AddDoors(atoi(tempdata[0].c_str()), tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), Ogre::StringConverter::parseBool(tempdata[8]), atof(tempdata[9].c_str()), atof(tempdata[10].c_str())));
		else
			StoreCommand(elev->AddDoors(atoi(tempdata[0].c_str()), tempdata[1].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), Ogre::StringConverter::parseBool(tempdata[7]), atof(tempdata[8].c_str()), atof(tempdata[9].c_str())));
	}

	//SetShaftDoors command
	if (SetCaseCopy(LineData.substr(0, 13), false) == "setshaftdoors")
	{
		//get data
		int params = SplitData(LineData.c_str(), 14);

		if (params != 4)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 3; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		elev->SetShaftDoors(atoi(tempdata[0].c_str()), atof(tempdata[1].c_str()), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()));
		setshaftdoors = true;
	}

	//AddShaftDoors command
	if (SetCaseCopy(LineData.substr(0, 14), false) == "addshaftdoors ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 14);

		if (params < 7 || params > 8)
			return ScriptError("Incorrect number of parameters");

		bool compat = false;
		if (params == 7)
			compat = true; //1.4 compatibility mode

		//check numeric values
		if (compat == false)
		{
			for (int i = 0; i <= 7; i++)
			{
				if (i == 1)
					i = 3;
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
		}
		else
		{
			for (int i = 0; i <= 6; i++)
			{
				if (i == 1)
					i = 2;
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
		}
		
		bool result;
		if (compat == false)
			result = elev->AddShaftDoors(atoi(tempdata[0].c_str()), tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()));
		else
			result = elev->AddShaftDoors(atoi(tempdata[0].c_str()), tempdata[1].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()));

		if (result == false)
			return ScriptError();
	}

	//CreatePanel command
	if (SetCaseCopy(LineData.substr(0, 11), false) == "createpanel")
	{
		//get data
		int params = SplitData(LineData.c_str(), 12);

		if (params != 13)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 12; i++)
		{
			if (i == 3)
				i = 4;
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		StoreCommand(elev->CreateButtonPanel(tempdata[0].c_str(), atoi(tempdata[1].c_str()), atoi(tempdata[2].c_str()), tempdata[3].c_str(), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str())));
	}

	//AddFloorButton command
	if (SetCaseCopy(LineData.substr(0, 14), false) == "addfloorbutton")
	{
		//get data
		int params = SplitData(LineData.c_str(), 15);

		if (params < 7 || params > 11)
			return ScriptError("Incorrect number of parameters");

		float hoffset = 0, voffset = 0;
		int compat = 0;

		if (params == 7)
		{
			//1.4 compatibility mode
			//check numeric values
			for (int i = 0; i <= 6; i++)
			{
				if (i == 1)
					i = 2;
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
			compat = 1;
		}
		if (params == 9)
		{
			std::string str = tempdata[2];
			TrimString(str);
			if (IsNumeric(str.c_str()) == true)
			{
				//1.5 compatibility mode
				//check numeric values
				for (int i = 0; i <= 8; i++)
				{
					if (i == 1 || i == 4)
						i++;

			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
						return ScriptError("Invalid value: " + std::string(tempdata[i]));
				}
				hoffset = atof(tempdata[7].c_str());
				voffset = atof(tempdata[8].c_str());
				compat = 1;
			}
		}
		if (params == 8 || params == 10)
		{
			//pre-Alpha 6 compatibility
			//check numeric values
			for (int i = 0; i <= 7; i++)
			{
				if (i == 1)
					i = 3;
				if (i == 5)
					i++;
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
			if (params == 10)
			{
				hoffset = atof(tempdata[8].c_str());
				voffset = atof(tempdata[9].c_str());
			}
			compat = 2;
		}
		if (params == 9 || params == 11)
		{
			//check numeric values
			for (int i = 0; i <= 8; i++)
			{
				if (i == 1)
					i = 4;
				if (i == 6)
					i++;
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
			if (params == 11)
			{
				hoffset = atof(tempdata[9].c_str());
				voffset = atof(tempdata[10].c_str());
			}
		}

		if (!elev->GetPanel(atoi(tempdata[0].c_str())))
			return ScriptError("Invalid panel number");

		if (compat == 0)
		{
			CheckFile(std::string("data/" + tempdata[1]).c_str());
			elev->GetPanel(atoi(tempdata[0].c_str()))->AddButton(tempdata[1].c_str(), tempdata[2].c_str(), tempdata[3].c_str(), atoi(tempdata[4].c_str()), atoi(tempdata[5].c_str()), tempdata[6].c_str(), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), hoffset, voffset);
		}
		if (compat == 1)
			elev->GetPanel(atoi(tempdata[0].c_str()))->AddButton("", tempdata[1].c_str(), tempdata[1].c_str(), atoi(tempdata[2].c_str()), atoi(tempdata[3].c_str()), tempdata[4].c_str(), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), hoffset, voffset);
		if (compat == 2)
			elev->GetPanel(atoi(tempdata[0].c_str()))->AddButton("", tempdata[1].c_str(), tempdata[2].c_str(), atoi(tempdata[3].c_str()), atoi(tempdata[4].c_str()), tempdata[5].c_str(), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), hoffset, voffset);
	}

	//AddControlButton command
	if (SetCaseCopy(LineData.substr(0, 16), false) == "addcontrolbutton")
	{
		//get data
		int params = SplitData(LineData.c_str(), 17);

		if (params < 7 || params > 11)
			return ScriptError("Incorrect number of parameters");

		float hoffset = 0, voffset = 0;
		int compat = 0;

		if (params == 7)
		{
			//1.4 compatibility mode
			//check numeric values
			for (int i = 1; i <= 6; i++)
			{
				if (i == 1 || i == 4)
					i++;
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
			compat = 1;
		}
		if (params == 9)
		{
			std::string str = tempdata[2];
			TrimString(str);
			if (IsNumeric(str.c_str()) == true)
			{
				//1.5 compatibility mode
				//check numeric values
				for (int i = 1; i <= 8; i++)
				{
					if (i == 1 || i == 4)
						i++;
					std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
						return ScriptError("Invalid value: " + std::string(tempdata[i]));
				}
				hoffset = atof(tempdata[7].c_str());
				voffset = atof(tempdata[8].c_str());
				compat = 1;
			}
		}
		if (params == 8 || params == 10)
		{
			//pre-Alpha 6 compatibility
			//check numeric values
			for (int i = 1; i <= 7; i++)
			{
				if (i == 1)
					i = 3;
				if (i == 5)
					i++;
				std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
			if (params == 10)
			{
				hoffset = atof(tempdata[8].c_str());
				voffset = atof(tempdata[9].c_str());
			}
			compat = 2;
		}
		if (params == 9 || params == 11)
		{
			//check numeric values
			for (int i = 1; i <= 8; i++)
			{
				if (i == 1)
					i = 4;
				if (i == 6)
					i++;
				std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
			if (params == 11)
			{
				hoffset = atof(tempdata[9].c_str());
				voffset = atof(tempdata[10].c_str());
			}
		}

		if (!elev->GetPanel(atoi(tempdata[0].c_str())))
			return ScriptError("Invalid panel number");

		if (compat == 0)
		{
			CheckFile(std::string("data/" + tempdata[1]).c_str());
			elev->GetPanel(atoi(tempdata[0].c_str()))->AddButton(tempdata[1].c_str(), tempdata[2].c_str(), tempdata[3].c_str(), atoi(tempdata[4].c_str()), atoi(tempdata[5].c_str()), tempdata[6].c_str(), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), hoffset, voffset);
		}
		if (compat == 1)
			elev->GetPanel(atoi(tempdata[0].c_str()))->AddButton("", tempdata[1].c_str(), tempdata[1].c_str(), atoi(tempdata[2].c_str()), atoi(tempdata[3].c_str()), tempdata[4].c_str(), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), hoffset, voffset);
		if (compat == 2)
			elev->GetPanel(atoi(tempdata[0].c_str()))->AddButton("", tempdata[1].c_str(), tempdata[2].c_str(), atoi(tempdata[3].c_str()), atoi(tempdata[4].c_str()), tempdata[5].c_str(), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), hoffset, voffset);
	}

	//AddButton command
	if (SetCaseCopy(LineData.substr(0, 10), false) == "addbutton ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 10);

		if (params < 9 || params > 11)
			return ScriptError("Incorrect number of parameters");

		float hoffset = 0, voffset = 0;

		//check numeric values
		for (int i = 1; i <= 8; i++)
		{
			if (i == 1)
				i = 4;
			if (i == 6)
				i++;
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}
		if (params == 11)
		{
			hoffset = atof(tempdata[9].c_str());
			voffset = atof(tempdata[10].c_str());
		}

		if (!elev->GetPanel(atoi(tempdata[0].c_str())))
			return ScriptError("Invalid panel number");

		//check to see if file exists
		CheckFile(std::string("data/" + tempdata[1]).c_str());

		elev->GetPanel(atoi(tempdata[0].c_str()))->AddButton(tempdata[1].c_str(), tempdata[2].c_str(), tempdata[3].c_str(), atoi(tempdata[4].c_str()), atoi(tempdata[5].c_str()), tempdata[6].c_str(), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), hoffset, voffset);
	}

	//AddControl command
	if (SetCaseCopy(LineData.substr(0, 11), false) == "addcontrol ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 11);

		if (params < 10)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 7; i++)
		{
			if (i == 1)
				i++;
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		if (!elev->GetPanel(atoi(tempdata[0].c_str())))
			return ScriptError("Invalid panel number");

		std::vector<std::string> action_array, tex_array;
		int slength, parameters;

		//get number of action & texture parameters
		slength = (int)tempdata.size();
		parameters = slength - 8; //strip off main parameters

		//action & texture parameter number needs to be even
		if (IsEven(parameters) == false)
			return ScriptError("Incorrect number of parameters");

		for (temp3 = 8; temp3 < slength - (parameters / 2); temp3++)
			action_array.push_back(tempdata[temp3]);
		for (temp3 = slength - (parameters / 2); temp3 < slength; temp3++)
			tex_array.push_back(tempdata[temp3]);

		//check to see if file exists
		CheckFile(std::string("data/" + tempdata[1]).c_str());

		elev->GetPanel(atoi(tempdata[0].c_str()))->AddControl(tempdata[1].c_str(), atoi(tempdata[2].c_str()), atoi(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), action_array, tex_array);
	}

	//AddFloorIndicator command
	if (SetCaseCopy(LineData.substr(0, 17), false) == "addfloorindicator")
	{
		//get data
		int params = SplitData(LineData.c_str(), 18);

		if (params < 6 || params > 7)
			return ScriptError("Incorrect number of parameters");

		bool compat = false;
		if (params == 6)
			compat = true; //1.4 compatibility mode

		//check numeric values
		if (compat == false)
		{
			for (int i = 2; i <= 6; i++)
			{
				std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
		}
		else
		{
			for (int i = 1; i <= 5; i++)
			{
				std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
		}

		if (compat == false)
			StoreCommand(elev->AddFloorIndicator(tempdata[0].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str())));
		else
			StoreCommand(elev->AddFloorIndicator("Button", tempdata[0].c_str(), atof(tempdata[1].c_str()), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str())));
	}

	//AddDirectionalIndicators command
	if (SetCaseCopy(LineData.substr(0, 24), false) == "adddirectionalindicators")
	{
		//get data
		int params = SplitData(LineData.c_str(), 25);

		if (params < 17 || params > 18)
			return ScriptError("Incorrect number of parameters");

		bool compat = false;
		if (params == 17)
		{
			//check numeric values
			for (int i = 8; i <= 16; i++)
			{
				if (i == 11 || i == 14)
					i++;
				std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
			compat = true;
		}
		else
		{
			//check numeric values
			for (int i = 9; i <= 17; i++)
			{
				if (i == 12 || i == 15)
					i++;
				std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
		}

		if (compat == false)
			elev->AddDirectionalIndicators(Ogre::StringConverter::parseBool(tempdata[0]), Ogre::StringConverter::parseBool(tempdata[1]), Ogre::StringConverter::parseBool(tempdata[2]), Ogre::StringConverter::parseBool(tempdata[3]), tempdata[4].c_str(), tempdata[5].c_str(), tempdata[6].c_str(), tempdata[7].c_str(), tempdata[8].c_str(), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), tempdata[12].c_str(), atof(tempdata[13].c_str()), atof(tempdata[14].c_str()), Ogre::StringConverter::parseBool(tempdata[15]), atof(tempdata[16].c_str()), atof(tempdata[17].c_str()));
		else
			elev->AddDirectionalIndicators(Ogre::StringConverter::parseBool(tempdata[0]), false, Ogre::StringConverter::parseBool(tempdata[1]), Ogre::StringConverter::parseBool(tempdata[2]), tempdata[3].c_str(), tempdata[4].c_str(), tempdata[5].c_str(), tempdata[6].c_str(), tempdata[7].c_str(), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), tempdata[11].c_str(), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()), Ogre::StringConverter::parseBool(tempdata[14]), atof(tempdata[15].c_str()), atof(tempdata[16].c_str()));
	}

	//AddFloorSigns command
	if (SetCaseCopy(LineData.substr(0, 13), false) == "addfloorsigns")
	{
		//get data
		int params = SplitData(LineData.c_str(), 14);

		if (params < 7 || params > 9)
			return ScriptError("Incorrect number of parameters");

		int compat = 0;
		if (params == 7)
			compat = 1; //1.4 compatibility mode
		if (params == 8)
			compat = 2; //1.5 compatibility mode

		//check numeric values
		if (compat == 0)
		{
			for (int i = 0; i <= 8; i++)
			{
				if (i == 1)
					i = 4;
				std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
		}
		else if (compat == 1)
		{
			for (int i = 2; i <= 6; i++)
			{
				std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
		}
		else if (compat == 2)
		{
			for (int i = 3; i <= 7; i++)
			{
				std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
		}

		if (compat == 0)
		{
			bool result;
			result = elev->AddFloorSigns(atoi(tempdata[0].c_str()), Ogre::StringConverter::parseBool(tempdata[1]), tempdata[2].c_str(), tempdata[3].c_str(), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()));
			if (result == false)
				return ScriptError();
		}
		else if (compat == 1)
			elev->AddFloorSigns(0, Ogre::StringConverter::parseBool(tempdata[0]), "Button", tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()));
		else if (compat == 2)
			elev->AddFloorSigns(0, Ogre::StringConverter::parseBool(tempdata[0]), tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()));
	}

	//AddSound
	if (SetCaseCopy(LineData.substr(0, 8), false) == "addsound")
	{
		//get data
		int params = SplitData(LineData.c_str(), 9);

		if (params != 5 && params != 13)
			return ScriptError("Incorrect number of parameters");

		bool partial = false;
		if (params == 5)
			partial = true;

		//check numeric values
		if (partial == true)
		{
			for (int i = 2; i <= 4; i++)
			{
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
		}
		else
		{
			for (int i = 2; i <= 12; i++)
			{
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
		}

		//check to see if file exists
		CheckFile(std::string("data/" + tempdata[1]).c_str());

		if (partial == true)
			StoreCommand(elev->AddSound(tempdata[0].c_str(), tempdata[1].c_str(), Ogre::Vector3(atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()))));
		else
			StoreCommand(elev->AddSound(tempdata[0].c_str(), tempdata[1].c_str(), Ogre::Vector3(atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str())), atoi(tempdata[5].c_str()), atoi(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), Ogre::Vector3(atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str())))); 
	}

	//AddDoorComponent command
	if (SetCaseCopy(LineData.substr(0, 16), false) == "adddoorcomponent")
	{
		//get data
		int params = SplitData(LineData.c_str(), 17);

		if (params < 17 || params > 18)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		bool compat = false;
		if (params == 17)
			compat = true;

		for (int i = 0; i <= params - 1; i++)
		{
			if (i == 1)
				i = 4;
			if (i == 5)
				i++;
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		if (compat == true)
			StoreCommand(elev->AddDoorComponent(atoi(tempdata[0].c_str()), tempdata[1].c_str(), tempdata[2].c_str(), tempdata[3].c_str(), atof(tempdata[4].c_str()), tempdata[5].c_str(), atof(tempdata[6].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()), atof(tempdata[14].c_str()), atof(tempdata[15].c_str()), atof(tempdata[16].c_str())));
		else
			StoreCommand(elev->AddDoorComponent(atoi(tempdata[0].c_str()), tempdata[1].c_str(), tempdata[2].c_str(), tempdata[3].c_str(), atof(tempdata[4].c_str()), tempdata[5].c_str(), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()), atof(tempdata[14].c_str()), atof(tempdata[15].c_str()), atof(tempdata[16].c_str()), atof(tempdata[17].c_str())));
	}

	//AddShaftDoorsComponent command
	if (SetCaseCopy(LineData.substr(0, 22), false) == "addshaftdoorscomponent")
	{
		//get data
		int params = SplitData(LineData.c_str(), 23);

		if (params < 17 || params > 18)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		bool compat = false;
		if (params == 17)
			compat = true;

		for (int i = 0; i <= params - 1; i++)
		{
			if (i == 1)
				i = 4;
			if (i == 5)
				i++;
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		if (compat == true)
			elev->AddShaftDoorsComponent(atoi(tempdata[0].c_str()), tempdata[1].c_str(), tempdata[2].c_str(), tempdata[3].c_str(), atof(tempdata[4].c_str()), tempdata[5].c_str(), atof(tempdata[6].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()), atof(tempdata[14].c_str()), atof(tempdata[15].c_str()), atof(tempdata[16].c_str()));
		else
			elev->AddShaftDoorsComponent(atoi(tempdata[0].c_str()), tempdata[1].c_str(), tempdata[2].c_str(), tempdata[3].c_str(), atof(tempdata[4].c_str()), tempdata[5].c_str(), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()), atof(tempdata[14].c_str()), atof(tempdata[15].c_str()), atof(tempdata[16].c_str()), atof(tempdata[17].c_str()));
	}

	//FinishDoors command
	if (SetCaseCopy(LineData.substr(0, 11), false) == "finishdoors")
	{
		//get data
		int params = SplitData(LineData.c_str(), 12);

		if (params < 1)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		std::string str = tempdata[0];
		TrimString(str);
		if (!IsNumeric(str.c_str()))
			return ScriptError("Invalid value: " + std::string(tempdata[0]));

		StoreCommand(elev->FinishDoors(atoi(tempdata[0].c_str())));
	}

	//FinishShaftDoors command
	if (SetCaseCopy(LineData.substr(0, 16), false) == "finishshaftdoors")
	{
		//get data
		int params = SplitData(LineData.c_str(), 17);

		if (params < 1)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		std::string str = tempdata[0];
		TrimString(str);
		if (!IsNumeric(str.c_str()))
			return ScriptError("Invalid value: " + std::string(tempdata[0]));

		bool result;
		result = elev->FinishShaftDoors(atoi(tempdata[0].c_str()));

		if (result == false)
			return ScriptError();
	}

	//AddDirectionalIndicator command
	if (SetCaseCopy(LineData.substr(0, 24), false) == "adddirectionalindicator ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 24);

		if (params != 17)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 8; i <= 16; i++)
		{
			if (i == 11 || i == 14)
				i++;
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		StoreCommand(elev->AddDirectionalIndicator(Ogre::StringConverter::parseBool(tempdata[0]), Ogre::StringConverter::parseBool(tempdata[1]), Ogre::StringConverter::parseBool(tempdata[2]), tempdata[3].c_str(), tempdata[4].c_str(), tempdata[5].c_str(), tempdata[6].c_str(), tempdata[7].c_str(), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), tempdata[11].c_str(), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()), Ogre::StringConverter::parseBool(tempdata[14]), atof(tempdata[15].c_str()), atof(tempdata[16].c_str())));
	}

	//AddDoor command
	if (SetCaseCopy(LineData.substr(0, 8), false) == "adddoor ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 8);

		if (params < 12 || params > 14)
			return ScriptError("Incorrect number of parameters");

		int compat = 0;

		//check numeric values
		if (params == 12)
		{
			for (int i = 3; i <= 11; i++)
			{
				std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
			compat = 1;
		}
		if (params == 13)
		{
			for (int i = 3; i <= 12; i++)
			{
				std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
			compat = 2;
		}
		if (params == 14)
		{
			for (int i = 4; i <= 13; i++)
			{
				std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
		}

		CheckFile(std::string("data/" + tempdata[0]).c_str());
		CheckFile(std::string("data/" + tempdata[1]).c_str());

		//create door
		if (compat == 1)
			StoreCommand(elev->AddDoor(tempdata[0].c_str(), tempdata[1].c_str(), false, tempdata[2].c_str(), atof(tempdata[3].c_str()), atoi(tempdata[4].c_str()), 0, atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str())));
		else if (compat == 2)
			StoreCommand(elev->AddDoor(tempdata[0].c_str(), tempdata[1].c_str(), false, tempdata[2].c_str(), atof(tempdata[3].c_str()), atoi(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str())));
		else
			StoreCommand(elev->AddDoor(tempdata[0].c_str(), tempdata[1].c_str(), Ogre::StringConverter::parseBool(tempdata[2]), tempdata[3].c_str(), atof(tempdata[4].c_str()), atoi(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str())));
	}

	//AddModel command
	if (SetCaseCopy(LineData.substr(0, 8), false) == "addmodel")
	{
		//get data
		int params = SplitData(LineData.c_str(), 9);

		if (params != 14)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 13; i++)
		{
			if (i == 10)
				i++;
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		//check to see if file exists
		CheckFile(std::string("data/" + tempdata[1]).c_str());

		//create model
		StoreCommand(elev->AddModel(tempdata[0].c_str(), tempdata[1].c_str(), Ogre::Vector3(atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str())), Ogre::Vector3(atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str())), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), Ogre::StringConverter::parseBool(tempdata[10]), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str())));
	}

	//Set command
	if (SetCaseCopy(LineData.substr(0, 4), false) == "set ")
	{
		temp1 = LineData.find("=", 0);
		if (temp1 < 0)
			return ScriptError("Syntax error");

		std::string str = LineData.substr(4, temp1 - 5);
		TrimString(str);
		if (!IsNumeric(str.c_str(), temp3))
			return ScriptError("Invalid variable number");

		//get text after equal sign
		temp2 = GetAfterEquals(LineData.c_str());

		if (temp3 < 0 || temp3 > (int)UserVariable.size() - 1)
			return ScriptError("Invalid variable number");

		UserVariable[temp3] = Calc(temp2.c_str());
		if (Simcore->Verbose == true)
			skyscraper->Report("Variable " + std::string(_itoa(temp3, intbuffer, 10)) + " set to " + UserVariable[temp3]);
	}

	//handle elevator range
	if (RangeL != RangeH && SetCaseCopy(LineData.substr(0, 12), false) == "<endelevator")
	{
		if (Current < RangeH)
		{
			Current++;
			line = RangeStart;  //loop back
			return sNextLine;
		}
		else
		{
			Section = 0; //break out of loop
			Context = "None";
			RangeL = 0;
			RangeH = 0;
			return sNextLine;
		}
	}
	return 0;
}

int ScriptProcessor::ProcTextures()
{
	//Process Textures

	if (SetCaseCopy(LineData.substr(0, 5), false) == "load ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 5, false);

		if (params < 4 || params > 5)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 3; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}
		buffer = tempdata[0];
		CheckFile(buffer.c_str());
		if (params == 4)
			Simcore->LoadTexture(buffer.c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()));
		else
			Simcore->LoadTexture(buffer.c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), true, Ogre::StringConverter::parseBool(tempdata[4]));
	}
	if (SetCaseCopy(LineData.substr(0, 12), false) == "loadanimated")
	{
		//get data
		int params = SplitData(LineData.c_str(), 12, false);

		if (params < 7)
			return ScriptError("Incorrect number of parameters");

		bool force;
		std::string temp = tempdata[params - 1];
		std::string temp2 = tempdata[params - 2];
		TrimString(temp);
		TrimString(temp2);
		if (IsNumeric(temp.c_str()) == true && IsNumeric(temp2.c_str()) == true)
			force = false;
		else
			force = true;

		//check numeric values
		if (force == true)
		{
			for (int i = (params - 4); i <= (params - 2); i++)
			{
				std::string str = tempdata[i];
				TrimString(str);
				if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
		}
		else
		{
			for (int i = (params - 3); i <= (params - 1); i++)
			{
				std::string str = tempdata[i];
				TrimString(str);
				if (!IsNumeric(str.c_str()))
					return ScriptError("Invalid value: " + std::string(tempdata[i]));
			}
		}

		buffer = tempdata[0];
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
		for (int i = 0; i < filenames.size(); i++)
			CheckFile(filenames[i].c_str());

		if (force == false)
			Simcore->LoadAnimatedTexture(filenames, tempdata[params - 4].c_str(), atof(tempdata[params - 3].c_str()), atof(tempdata[params - 2].c_str()), atof(tempdata[params - 1].c_str()));
		else
			Simcore->LoadAnimatedTexture(filenames, tempdata[params - 5].c_str(), atof(tempdata[params - 4].c_str()), atof(tempdata[params - 3].c_str()), atof(tempdata[params - 2].c_str()), true, Ogre::StringConverter::parseBool(tempdata[params - 1]));
	}
	if (SetCaseCopy(LineData.substr(0, 14), false) == "loadalphablend")
	{
		//get data
		int params = SplitData(LineData.c_str(), 14, false);

		if (params < 7 || params > 8)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 5; i <= 6; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}
		buffer = tempdata[0];

		//check existence of files
		CheckFile(buffer.c_str());
		CheckFile(tempdata[1].c_str());
		CheckFile(tempdata[2].c_str());

		if (params == 7)
			Simcore->LoadAlphaBlendTexture(buffer.c_str(), tempdata[1].c_str(), tempdata[2].c_str(), tempdata[3].c_str(), Ogre::StringConverter::parseBool(tempdata[4]), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()));
		else
			Simcore->LoadAlphaBlendTexture(buffer.c_str(), tempdata[1].c_str(), tempdata[2].c_str(), tempdata[3].c_str(), Ogre::StringConverter::parseBool(tempdata[4]), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), true, Ogre::StringConverter::parseBool(tempdata[7]));
	}
	if (SetCaseCopy(LineData.substr(0, 12), false) == "loadmaterial")
	{
		//get data
		int params = SplitData(LineData.c_str(), 12, false);

		if (params < 4 || params > 5)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 3; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}
		buffer = tempdata[0];
		if (params == 4)
			Simcore->LoadMaterial(buffer.c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()));
		else
			Simcore->LoadMaterial(buffer.c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), true, Ogre::StringConverter::parseBool(tempdata[4]));
	}
	if (SetCaseCopy(LineData.substr(0, 9), false) == "loadrange")
	{
		//get data
		int params = SplitData(LineData.c_str(), 9, false);

		if (params < 6 || params > 7)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 5; i++)
		{
			if (i == 2)
				i = 4;
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}
		RangeL = atoi(tempdata[0].c_str());
		RangeH = atoi(tempdata[1].c_str());
		for (Current = RangeL; Current <= RangeH; Current++)
		{
			temp2 = tempdata[2];
			buffer = Ogre::StringConverter::toString(Current);
			TrimString(buffer);
			ReplaceAll(temp2, "%number%", buffer.c_str());
			temp6 = tempdata[3];
			ReplaceAll(temp6, "%number%", buffer.c_str());
			CheckFile(temp2.c_str());
			if (params == 6)
				Simcore->LoadTexture(temp2.c_str(), temp6.c_str(), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()));
			else
				Simcore->LoadTexture(temp2.c_str(), temp6.c_str(), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), true, Ogre::StringConverter::parseBool(tempdata[6]));
		}
	}
	if (SetCaseCopy(LineData.substr(0, 8), false) == "addtext ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 8, false);

		if (params < 14 || params > 15)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 13; i++)
		{
			if (i == 4)
				i = 5;
			if (i == 9)
				i = 11;

			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}
		buffer = tempdata[2];
		TrimString(buffer);
		buffer.insert(0, "data/fonts/");
		CheckFile(buffer.c_str());
		if (params == 14)
			Simcore->AddTextToTexture(tempdata[0].c_str(), tempdata[1].c_str(), buffer.c_str(), atof(tempdata[3].c_str()), tempdata[4].c_str(), atoi(tempdata[5].c_str()), atoi(tempdata[6].c_str()), atoi(tempdata[7].c_str()), atoi(tempdata[8].c_str()), tempdata[9].c_str(), tempdata[10].c_str(), atoi(tempdata[11].c_str()), atoi(tempdata[12].c_str()), atoi(tempdata[13].c_str()));
		else
			Simcore->AddTextToTexture(tempdata[0].c_str(), tempdata[1].c_str(), buffer.c_str(), atof(tempdata[3].c_str()), tempdata[4].c_str(), atoi(tempdata[5].c_str()), atoi(tempdata[6].c_str()), atoi(tempdata[7].c_str()), atoi(tempdata[8].c_str()), tempdata[9].c_str(), tempdata[10].c_str(), atoi(tempdata[11].c_str()), atoi(tempdata[12].c_str()), atoi(tempdata[13].c_str()), true, Ogre::StringConverter::parseBool(tempdata[14]));
	}
	if (SetCaseCopy(LineData.substr(0, 12), false) == "addtextrange")
	{
		//get data
		int params = SplitData(LineData.c_str(), 13, false);

		if (params < 16 || params > 17)
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

			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}
		RangeL = atoi(tempdata[0].c_str());
		RangeH = atoi(tempdata[1].c_str());
		temp6 = LineData;
		for (Current = RangeL; Current <= RangeH; Current++)
		{
			buffer = Ogre::StringConverter::toString(Current);
			TrimString(buffer);
			LineData = temp6;
			ReplaceAll(LineData, "%number%", buffer.c_str());
		
			//get data
			int params = SplitData(LineData.c_str(), 13, false);

			buffer = tempdata[4];
			TrimString(buffer);
			buffer.insert(0, "data/fonts/");
			CheckFile(buffer.c_str());
			if (params == 16)
				Simcore->AddTextToTexture(tempdata[2].c_str(), tempdata[3].c_str(), buffer.c_str(), atof(tempdata[5].c_str()), tempdata[6].c_str(), atoi(tempdata[7].c_str()), atoi(tempdata[8].c_str()), atoi(tempdata[9].c_str()), atoi(tempdata[10].c_str()), tempdata[11].c_str(), tempdata[12].c_str(), atoi(tempdata[13].c_str()), atoi(tempdata[14].c_str()), atoi(tempdata[15].c_str()));
			else
				Simcore->AddTextToTexture(tempdata[2].c_str(), tempdata[3].c_str(), buffer.c_str(), atof(tempdata[5].c_str()), tempdata[6].c_str(), atoi(tempdata[7].c_str()), atoi(tempdata[8].c_str()), atoi(tempdata[9].c_str()), atoi(tempdata[10].c_str()), tempdata[11].c_str(), tempdata[12].c_str(), atoi(tempdata[13].c_str()), atoi(tempdata[14].c_str()), atoi(tempdata[15].c_str()), true, Ogre::StringConverter::parseBool(tempdata[16]));
		}
	}
	if (SetCaseCopy(LineData.substr(0, 11), false) == "loadcropped")
	{
		//get data
		int params = SplitData(LineData.c_str(), 12, false);

		if (params < 8 || params > 9)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 7; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}
		buffer = tempdata[0];
		TrimString(buffer);
		CheckFile(buffer.c_str());
		if (params == 8)
			Simcore->LoadTextureCropped(buffer.c_str(), tempdata[1].c_str(), atoi(tempdata[2].c_str()), atoi(tempdata[3].c_str()), atoi(tempdata[4].c_str()), atoi(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()));
		else
			Simcore->LoadTextureCropped(buffer.c_str(), tempdata[1].c_str(), atoi(tempdata[2].c_str()), atoi(tempdata[3].c_str()), atoi(tempdata[4].c_str()), atoi(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), Ogre::StringConverter::parseBool(tempdata[8]));
	}
	if (SetCaseCopy(LineData.substr(0, 10), false) == "addoverlay")
	{
		//get data
		int params = SplitData(LineData.c_str(), 11, false);

		if (params < 9 || params > 10)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 8; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}
		if (params == 9)
			Simcore->AddTextureOverlay(tempdata[0].c_str(), tempdata[1].c_str(), tempdata[2].c_str(), atoi(tempdata[3].c_str()), atoi(tempdata[4].c_str()), atoi(tempdata[5].c_str()), atoi(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()));
		else
			Simcore->AddTextureOverlay(tempdata[0].c_str(), tempdata[1].c_str(), tempdata[2].c_str(), atoi(tempdata[3].c_str()), atoi(tempdata[4].c_str()), atoi(tempdata[5].c_str()), atoi(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), true, Ogre::StringConverter::parseBool(tempdata[9]));
	}
	if (SetCaseCopy(LineData.substr(0, 11), false) == "setlighting")
	{
		//get data
		int params = SplitData(LineData.c_str(), 12, false);

		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 2; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}
		Simcore->SetLighting(atof(tempdata[0].c_str()), atof(tempdata[1].c_str()), atof(tempdata[2].c_str()));
	}
	if (SetCaseCopy(LineData.substr(0, 7), false) == "rotate ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 7, false);

		if (params != 2)
			return ScriptError("Incorrect number of parameters");

		//check numeric value
		std::string str = tempdata[1];
		TrimString(str);
		if (!IsNumeric(str.c_str()))
			return ScriptError("Invalid value: " + std::string(tempdata[1]));

		buffer = tempdata[0];
		Simcore->RotateTexture(buffer.c_str(), atof(tempdata[1].c_str()));
	}
	if (SetCaseCopy(LineData.substr(0, 10), false) == "rotateanim")
	{
		//get data
		int params = SplitData(LineData.c_str(), 10, false);

		if (params != 2)
			return ScriptError("Incorrect number of parameters");

		//check numeric value
		std::string str = tempdata[1];
		TrimString(str);
		if (!IsNumeric(str.c_str()))
			return ScriptError("Invalid value: " + std::string(tempdata[1]));

		buffer = tempdata[0];
		Simcore->RotateAnimTexture(buffer.c_str(), atof(tempdata[1].c_str()));
	}
	if (SetCaseCopy(LineData.substr(0, 7), false) == "scroll ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 7, false);

		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 2; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		buffer = tempdata[0];
		Simcore->ScrollTexture(buffer.c_str(), atof(tempdata[1].c_str()), atof(tempdata[2].c_str()));
	}
	if (SetCaseCopy(LineData.substr(0, 10), false) == "scrollanim")
	{
		//get data
		int params = SplitData(LineData.c_str(), 10, false);

		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 2; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		buffer = tempdata[0];
		Simcore->ScrollAnimTexture(buffer.c_str(), atof(tempdata[1].c_str()), atof(tempdata[2].c_str()));
	}
	if (SetCaseCopy(LineData.substr(0, 5), false) == "scale")
	{
		//get data
		int params = SplitData(LineData.c_str(), 5, false);

		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 2; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		buffer = tempdata[0];
		Simcore->ScaleTexture(buffer.c_str(), atof(tempdata[1].c_str()), atof(tempdata[2].c_str()));
	}
	if (SetCaseCopy(LineData.substr(0, 9), false) == "transform")
	{
		//get data
		int params = SplitData(LineData.c_str(), 9, false);

		if (params != 7)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 6; i++)
		{
			std::string str = tempdata[i];
			TrimString(str);
			if (!IsNumeric(str.c_str()))
				return ScriptError("Invalid value: " + std::string(tempdata[i]));
		}

		buffer = tempdata[0];
		Simcore->TransformTexture(buffer.c_str(), tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()));
	}
	return 0;
}

std::string ScriptProcessor::Calc(const char *expression)
{
	//performs a calculation operation on a string
	//for example, the string "1 + 1" would output to "2"
	//supports multiple and nested operations (within parenthesis)

	int temp1;
	std::string tmpcalc = expression;
	std::string one;
	std::string two;
	int start, end;
	CalcError = false;

	//first remove all whitespace from the string
	ReplaceAll(tmpcalc, " ", "");

	//find parenthesis
	do
	{
		start = tmpcalc.find("(", 0);
		if (start >= 0)
		{
			//find matching parenthesis
			int match = 1;
			int end = -1;
			for (int i = start + 1; i < (int)tmpcalc.length(); i++)
			{
				if (tmpcalc.at(i) == '(')
					match++;
				if (tmpcalc.at(i) == ')')
					match--;
				if (match == 0)
				{
					end = i;
					break;
				}
			}
			if (end != -1)
			{
				//call function recursively
				std::string newdata;
				newdata = Calc(tmpcalc.substr(start + 1, end - start - 1).c_str());

				if (CalcError == true)
					return newdata;

				//construct new string
				one = tmpcalc.substr(0, start);
				if (end < (int)tmpcalc.length() - 1)
					two = tmpcalc.substr(end + 1);
				else
					two = "";
				tmpcalc = one + newdata + two;
			}
			else
			{
				ScriptError("Syntax error in math operation: '" + tmpcalc + "' (might be nested)");
				CalcError = true;
				return "false";
			}
		}
		else
			break;
	} while (1 == 1);

	//find number of operators and recurse if multiple found
	int operators;
	do
	{
		operators = 0;
		end = 0;
		for (int i = 1; i < (int)tmpcalc.length(); i++)
		{
			if (tmpcalc.at(i) == '+' || tmpcalc.at(i) == '/' || tmpcalc.at(i) == '*')
			{
				operators++;
				if (operators == 2)
					end = i;
			}
			if (tmpcalc.at(i) == '-' && tmpcalc.at(i - 1) != '-' && tmpcalc.at(i - 1) != '+' && tmpcalc.at(i - 1) != '/' && tmpcalc.at(i - 1) != '*')
			{
				operators++;
				if (operators == 2)
					end = i;
			}
		}
		if (end >= (int)tmpcalc.length() - 1 && operators > 0)
		{
			ScriptError("Syntax error in math operation: '" + tmpcalc + "' (might be nested)");
			CalcError = true;
			return "false";
		}
		if (operators > 1)
		{
			std::string newdata;
			newdata = Calc(tmpcalc.substr(0, end).c_str());

			if (CalcError == true)
				return newdata;

			//construct new string
			two = tmpcalc.substr(end);
			tmpcalc = newdata + two;
		}
		else
			break;
	} while (1 == 1);

	//return value if none found
	if (operators == 0)
		return tmpcalc;

	//otherwise perform math
	temp1 = tmpcalc.find("+", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.substr(0, temp1);
		two = tmpcalc.substr(temp1 + 1);
		if (IsNumeric(one.c_str()) == true && IsNumeric(two.c_str()) == true)
		{
			float tmpnum = atof(one.c_str()) + atof(two.c_str());
			tmpcalc = Simcore->TruncateNumber(tmpnum, 6);
			return tmpcalc;
		}
		else
		{
			/*if (IsNumeric(one.c_str()) == false)
				ScriptError("Syntax error in math operation: '" + one + "' is not numeric");
			else
				ScriptError("Syntax error in math operation: '" + two + "' is not numeric");
			CalcError = true;
			return "false";*/
		}
	}
	temp1 = tmpcalc.find("-", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.substr(0, temp1);
		two = tmpcalc.substr(temp1 + 1);
		if (IsNumeric(one.c_str()) == true && IsNumeric(two.c_str()) == true)
		{
			float tmpnum = atof(one.c_str()) - atof(two.c_str());
			tmpcalc = Simcore->TruncateNumber(tmpnum, 6);
			return tmpcalc;
		}
		else
		{
			/*if (IsNumeric(one.c_str()) == false)
				ScriptError("Syntax error in math operation: '" + one + "' is not numeric");
			else
				ScriptError("Syntax error in math operation: '" + two + "' is not numeric");
			CalcError = true;
			return "false";*/
		}
	}
	temp1 = tmpcalc.find("/", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.substr(0, temp1);
		two = tmpcalc.substr(temp1 + 1);
		if (IsNumeric(one.c_str()) == true && IsNumeric(two.c_str()) == true)
		{
			float tmpnum = atof(one.c_str()) / atof(two.c_str());
			tmpcalc = Simcore->TruncateNumber(tmpnum, 6);
			return tmpcalc;
		}
		else
		{
			/*if (IsNumeric(one.c_str()) == false)
				ScriptError("Syntax error in math operation: '" + one + "' is not numeric");
			else
				ScriptError("Syntax error in math operation: '" + two + "' is not numeric");
			CalcError = true;
			return "false";*/
		}
	}
	temp1 = tmpcalc.find("*", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.substr(0, temp1);
		two = tmpcalc.substr(temp1 + 1);
		if (IsNumeric(one.c_str()) == true && IsNumeric(two.c_str()) == true)
		{
			float tmpnum = atof(one.c_str()) * atof(two.c_str());
			tmpcalc = Simcore->TruncateNumber(tmpnum, 6);
			return tmpcalc;
		}
		else
		{
			/*if (IsNumeric(one.c_str()) == false)
				ScriptError("Syntax error in math operation: '" + one + "' is not numeric");
			else
				ScriptError("Syntax error in math operation: '" + two + "' is not numeric");
			CalcError = true;
			return "false";*/
		}
	}
	return tmpcalc;
}

void ScriptProcessor::StoreCommand(Object *object)
{
	//store command and line info in object
	if (object)
	{
		TrimString(BuildingData[line]);
		object->command = BuildingData[line];
		object->command_processed = LineData;
		object->linenum = line + 1;
		object->context = Context;
		std::string current;
		current = Ogre::StringConverter::toString(Current);
		if (Section == 2)
			object->context = "Floor " + current;
		if (Section == 4)
			object->context = "Elevator " + current;
	}
}

bool ScriptProcessor::FunctionProc()
{
	//process functions
	for (int i = 0; i < (int)functions.size(); i++)
	{
		int location = LineData.find(functions[i].name + "(");
		if (location >= 0)
		{
			//found a function

			//store info
			InFunction = true;
			FunctionCallLine = line;

			//get function parameters
			int location2 = location + (int)functions[i].name.length() + 1;
			int end_loc = LineData.find(")", location);
			std::string newdata = LineData.substr(location2, end_loc - location2);
			SplitString(tempdata, newdata.c_str(), ',');

			//calculate inline math
			FunctionParams.reserve(tempdata.size());
			for (temp3 = 0; temp3 < (int)tempdata.size(); temp3++)
			{
				buffer = Calc(tempdata[temp3].c_str());
				TrimString(buffer);
				FunctionParams.push_back(buffer);
			}

			//remove function statement
			LineData = LineData.substr(0, location) + LineData.substr(end_loc + 1);

			//switch to function line
			FunctionCallLineData = LineData;
			line = functions[i].line;
			return true;
		}
	}
	return false;
}

void ScriptProcessor::CheckFile(const char *filename)
{
	//check to see if the specified file exists.
	//if not, add to nonexistent_files array

	std::string file = filename;

	if (file == "")
		return;

	int loc = file.find_last_of("/");
	if (loc > 0)
	{
		if (file.length() == loc + 1)
			return;
	}
	loc = file.find_last_of("\\");
	if (loc > 0)
	{
		if (file.length() == loc + 1)
			return;
	}

	ReplaceAll(file, "\\", "/");

	//skip file if a wildcard character is found
	loc = file.find_last_of("*");
	if (loc > 0)
		return;

	if (Simcore->FileExists(file.c_str()) == false)
	{
		bool exists = false;
		for (int i = 0; i < nonexistent_files.size(); i++)
		{
			if (nonexistent_files[i] == file)
			{
				exists = true;
				break;
			}
		}
		if (exists == false)
			nonexistent_files.push_back(file);
	}
}

int ScriptProcessor::SplitData(const char *string, int start, bool calc)
{
	//split data into separate strings starting at the "start" character
	//delimeter is a comma ","
	//returns the number of parameters found

	std::string data = string;
	std::string stringbuffer;
	SplitString(tempdata, data.substr(start).c_str(), ',');
	if (calc == true)
	{
		for (int i = 0; i < (int)tempdata.size(); i++)
		{
			stringbuffer = Calc(tempdata[i].c_str());
			TrimString(stringbuffer);
			tempdata[i] = stringbuffer;
		}
	}
	return (int)tempdata.size();
}

int ScriptProcessor::SplitAfterEquals(const char *string, bool calc)
{
	//get and split data after equal sign
	//returns -1 if equal sign not found

	std::string data = string;
	int loc = data.find("=", 0);
	if (loc < 0)
		return -1;

	std::string temp = data.substr(loc + 1);
	TrimString(temp);

	SplitString(tempdata, temp.c_str(), ',');
	if (calc == true)
	{
		for (int i = 0; i < (int)tempdata.size(); i++)
		{
			std::string buffer = Calc(tempdata[i].c_str());
			TrimString(buffer);
			tempdata[i] = buffer;
		}
	}
	return (int)tempdata.size();
}

std::string ScriptProcessor::GetAfterEquals(const char *string)
{
	//return data after equal sign

	std::string data = string;
	int loc = data.find("=", 0);
	if (loc < 0)
		return "";

	std::string temp = data.substr(loc + 1);
	TrimString(temp);
	return temp;
}

