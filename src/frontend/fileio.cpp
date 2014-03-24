/* $Id$ */

/*
	Skyscraper 1.9 Alpha - File I/O and Script Processing Routines
	Copyright (C)2003-2014 Ryan Thoryk
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
#include <wx/msgdlg.h>
#include "globals.h"
#include "sbs.h"
#include <OgreFileSystem.h>
#include <stdlib.h>
#include <cmath>
#include "fileio.h"
#include "skyscraper.h"
#include "camera.h"
#include "floor.h"
#include "elevator.h"
#include "shaft.h"
#include "callbutton.h"
#include "random.h"
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
#define sSkipReset 6

ScriptProcessor::ScriptProcessor()
{
	Reset();
}

ScriptProcessor::~ScriptProcessor()
{

}

void ScriptProcessor::Reset()
{
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
	tempdata.clear();
	callbutton_elevators.clear();
	FloorCheck = 0;
	RangeL = 0;
	RangeH = 0;
	RangeStart = 0;
	wall = 0;
	buffer = "";
	startpos = 0;
	getfloordata = false;
	setshaftdoors = false;
	BuildingData.clear();
	BuildingDataOrig.clear();
	MinExtent = 0;
	MaxExtent = 0;
	InFunction = 0;
	FunctionStack.clear();
	ReplaceLine = false;
	ReplaceLineData = "";
	nonexistent_files.clear();
	ReverseAxis = false;
	setkey = false;
	keyvalue = 0;
	lockvalue = 0;
	warn_deprecated = skyscraper->GetConfigBool("Skyscraper.Frontend.WarnDeprecated", false);
	show_percent = true;
	InWhile = false;
	IsFinished = false;
	progress_marker = 0;
	functions.clear();
	includes.clear();
	variables.clear();
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
}

bool ScriptProcessor::Run()
{
	//building loader/script interpreter

	int returncode = sContinue;
	IsFinished = false;

	if (line < (int)BuildingData.size())
	{
		LineData = BuildingData[line];
		TrimString(LineData);

		if (ReplaceLine == true)
		{
			LineData = ReplaceLineData;
			ReplaceLine = false;
		}

		//create a lowercase string of the line
		std::string linecheck = SetCaseCopy(LineData, false);

		//skip blank lines
		if (LineData == "")
			goto Nextline;

		//process comment markers
		temp1 = LineData.find("#", 0);
		if (temp1 > -1)
			LineData.erase(temp1);

		linecheck = SetCaseCopy(LineData, false);

		//skip blank lines
		if (LineData == "")
			goto Nextline;

		//function parameter variables
		if (InFunction > 0)
		{
			startpos = 0;
			do
			{
				//Function parameter conversion
				temp1 = LineData.find("%param", startpos);
				if (temp1 >= startpos)
				{
					temp3 = LineData.find("%", temp1 + 6);
					if (temp3 >= (int)LineData.length() || temp3 < 0)
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
					std::string str = LineData.substr(temp1 + 6, temp3 - (temp1 + 6));
					TrimString(str);
					temp2 = str;
					if (IsNumeric(temp2.c_str()) == true)
					{
						temp4 = atoi(temp2.c_str());

						//replace all occurrences of the variable with it's value
						std::string replacement;
						if (temp4 > 0 && temp4 <= (int)FunctionStack[InFunction - 1].Params.size())
							replacement = FunctionStack[InFunction - 1].Params[temp4 - 1].c_str();
						else
							replacement = "";

						ReplaceAll(LineData, std::string("%param" + temp2 + "%").c_str(), replacement.c_str());
						startpos = temp1;
					}
					else
						startpos = temp3 + 1;
				}
				else
					startpos++;
			} while (true);
		}
		else if (show_percent == true)
		{
			int percent = ((float)line / (float)BuildingData.size()) * 100.0;
			std::string percent_s = ToString(percent);
			int marker = percent / 10;
			if (marker > progress_marker)
			{
				progress_marker = marker;
				skyscraper->Report(percent_s + "%");
				skyscraper->UpdateProgress(percent);
			}
		}

		linecheck = SetCaseCopy(LineData, false);

		//////////////////////
		//Section information
		//////////////////////
		if (linecheck == "<globals>")
		{
			if (Section > 0)
			{
				ScriptError("Already within a section");
				goto Error;
			}
			Section = 1;
			Context = "Globals";
			skyscraper->Report("Processing globals...");
			goto Nextline;
		}
		if (linecheck == "<endglobals>")
		{
			if (Section != 1)
			{
				ScriptError("Not in global section");
				goto Error;
			}
			Simcore->InitMeshes();
			Section = 0;
			Context = "None";
			skyscraper->Report("Finished globals");
			goto Nextline;
		}
		if (linecheck.substr(0, 7) == "<floors")
		{
			if (Section > 0)
			{
				ScriptError("Already within a section");
				goto Error;
			}
			Section = 2;
			temp3 = linecheck.find("to", 0);
			if (temp3 < 0)
			{
				ScriptError("Syntax error");
				goto Error;

			}
			//get low and high range markers
			std::string str1 = LineData.substr(8, temp3 - 9);
			std::string str2 = LineData.substr(temp3 + 2, LineData.length() - (temp3 + 2) - 1);
			TrimString(str1);
			TrimString(str2);
			if (!IsNumeric(str1.c_str(), RangeL) || !IsNumeric(str2.c_str(), RangeH))
			{
				ScriptError("Invalid range");
				goto Error;
			}
			Context = "Floor range " + ToString2(RangeL) + " to " + ToString2(RangeH);
			Current = RangeL;
			RangeStart = line;
			skyscraper->Report("Processing floors " + ToString2(RangeL) + " to " + ToString2(RangeH) + "...");
			goto Nextline;
		}
		if (linecheck.substr(0, 7) == "<floor ")
		{
			if (Section > 0)
			{
				ScriptError("Already within a section");
				goto Error;
			}
			Section = 2;
			RangeL = 0;
			RangeH = 0;
			std::string str = LineData.substr(7, LineData.length() - 8);
			TrimString(str);
			if (!IsNumeric(str.c_str(), Current))
			{
				ScriptError("Invalid floor");
				goto Error;
			}
			Context = "Floor " + ToString2(Current);
			skyscraper->Report("Processing floor " + ToString2(Current) + "...");
			goto Nextline;
		}
		if (linecheck == "<endfloor>" && RangeL == RangeH)
		{
			if (Section != 2)
			{
				ScriptError("Not in floor section");
				goto Error;
			}
			Section = 0;
			Context = "None";
			skyscraper->Report("Finished floor");
			goto Nextline;
		}
		if (linecheck.substr(0, 10) == "<elevators")
		{
			if (Section > 0)
			{
				ScriptError("Already within a section");
				goto Error;
			}
			Section = 4;
			temp3 = linecheck.find("to", 10);
			if (temp3 < 0)
			{
				ScriptError("Syntax error");
				goto Error;
			}
			std::string str1 = LineData.substr(11, temp3 - 12);
			std::string str2 = LineData.substr(temp3 + 2, LineData.length() - (temp3 + 2) - 1);
			TrimString(str1);
			TrimString(str2);
			if (!IsNumeric(str1.c_str(), RangeL) || !IsNumeric(str2.c_str(), RangeH))
			{
				ScriptError("Invalid range");
				goto Error;
			}
			Context = "Elevator range " + ToString2(RangeL) + " to " + ToString2(RangeH);
			Current = RangeL;
			RangeStart = line;
			skyscraper->Report("Processing elevators " + ToString2(RangeL) + " to " + ToString2(RangeH) + "...");
			goto Nextline;
		}
		if (linecheck.substr(0, 10) == "<elevator ")
		{
			if (Section > 0)
			{
				ScriptError("Already within a section");
				goto Error;
			}
			Section = 4;
			RangeL = 0;
			RangeH = 0;
			std::string str = LineData.substr(10, LineData.length() - 11);
			TrimString(str);
			if (!IsNumeric(str.c_str(), Current))
			{
				ScriptError("Invalid elevator");
				goto Error;
			}
			if (Current < 1 || Current > Simcore->Elevators() + 1)
			{
				ScriptError("Invalid elevator");
				goto Error;
			}
			Context = "Elevator " + ToString2(Current);
			skyscraper->Report("Processing elevator " + ToString2(Current) + "...");
			goto Nextline;
		}
		if (linecheck == "<endelevator>" && RangeL == RangeH)
		{
			if (Section != 4)
			{
				ScriptError("Not in elevator section");
				goto Error;
			}
			Section = 0;
			Context = "None";
			skyscraper->Report("Finished elevator");
			goto Nextline;
		}
		if (linecheck.substr(0, 10) == "<textures>")
		{
			if (Section > 0)
			{
				ScriptError("Already within a section");
				goto Error;
			}
			Section = 5;
			Context = "Textures";
			skyscraper->Report("Processing textures...");
			goto Nextline;
		}
		if (linecheck.substr(0, 13) == "<endtextures>")
		{
			if (Section != 5)
			{
				ScriptError("Not in texture section");
				goto Error;
			}
			Simcore->FreeTextureImages();
			Section = 0;
			Context = "None";
			skyscraper->Report("Finished textures");
			goto Nextline;
		}
		if (linecheck.substr(0, 5) == "<end>")
		{
			Section = 0;
			Context = "None";
			skyscraper->Report("Exiting building script");
			IsFinished = true;
			show_percent = false;
			line = (int)BuildingData.size(); //jump to end of script
			return true; //exit data file parser
		}
		if (linecheck.substr(0, 7) == "<break>")
		{
			//breakpoint function for debugging scripts
breakpoint:
			skyscraper->Report("Script breakpoint reached");
			goto Nextline;
		}
		if (linecheck.substr(0, 8) == "<include")
		{
			//include another file at the current script location

			int endloc = LineData.find(">");
			std::string includefile = LineData.substr(9, endloc - 9);
			TrimString(includefile);

			//delete current line
			BuildingData.erase(BuildingData.begin() + line);

			//insert file at current line
			std::string filename = Simcore->VerifyFile(includefile.c_str());
			bool result = LoadDataFile(filename.c_str(), true, line);
			if (result == false)
			{
				ScriptError("File not found");
				goto Error;
			}
			skyscraper->Report("Inserted file " + includefile);

			line--;
			goto Nextline;
		}
		if (linecheck.substr(0, 9) == "<function")
		{
			//define a function

			if (Section != 0)
			{
				ScriptError("Cannot define a function within a section");
				goto Error;
			}

			int endloc = LineData.find(">");
			std::string function = LineData.substr(10, endloc - 10);
			TrimString(function);

			//skip the function definition and show a warning if it's already been defined
			bool defined = IsFunctionDefined(function.c_str());

			if (defined == true)
				skyscraper->Report("Function '" + function + "' already defined");
			else
			{
				//store function info in array
				FunctionInfo info;
				info.name = function;
				info.line = line;
				functions.push_back(info);
			}

			//skip to end of function
			for (int i = line + 1; i < (int)BuildingData.size(); i++)
			{
				if (SetCaseCopy(BuildingData[i].substr(0, 13), false) == "<endfunction>")
				{
					line = i;
					break;
				}
			}

			if (defined == false)
				skyscraper->Report("Defined function " + function);
			goto Nextline;
		}
		if (linecheck.substr(0, 13) == "<endfunction>" && InFunction > 0)
		{
			//end function and return to original line
			line = FunctionStack[InFunction - 1].CallLine - 1;
			ReplaceLineData = FunctionStack[InFunction - 1].LineData;
			FunctionStack.erase(FunctionStack.begin() + InFunction - 1);
			InFunction -= 1;
			ReplaceLine = true;
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
				if (temp3 >= (int)LineData.length() || temp3 < 0)
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
				temp2 = LineData.substr(temp1 + 1, temp3 - temp1 - 1);
				TrimString(temp2);

				bool found = false;
				for (int i = 0; i < (int)variables.size(); i++)
				{
					if (variables[i].name == temp2)
					{
						found = true;

						//replace all occurrences of the variable with it's value
						ReplaceAll(LineData, std::string("%" + temp2 + "%").c_str(), variables[i].value.c_str());
						startpos = temp1;
						break;
					}
				}

				if (found == false)
					startpos = temp3 + 1;
			}
			else
				startpos++;
		} while (true);

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
				goto Error;
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
			LineData = LineData.substr(0, temp1 + 1) + tempdata + LineData.substr(temp3);

			if (!IsNumeric(tempdata.c_str(), temp4))
			{
				ScriptError("Invalid floor " + tempdata);
				goto Error;
			}
			if (Simcore->IsValidFloor(temp4) == false)
			{
				ScriptError("Invalid floor " + tempdata);
				goto Error;
			}

			//fullheight parameter
			buffer = ToString(temp4);
			TrimString(buffer);
			temp6 = "floor(" + buffer + ").fullheight";
			buffer = LineData;
			SetCase(buffer, false);
			temp1 = buffer.find(temp6, 0);
			if (temp1 > 0)
			{
				buffer = ToString(Simcore->GetFloor(temp4)->FullHeight());
				TrimString(buffer);
				LineData = LineData.substr(0, temp1) + buffer + LineData.substr(temp1 + temp6.length());
			}

			//height parameter
			buffer = ToString(temp4);
			TrimString(buffer);
			temp6 = "floor(" + buffer + ").height";
			buffer = LineData;
			SetCase(buffer, false);
			temp1 = buffer.find(temp6, 0);
			if (temp1 > 0)
			{
				buffer = ToString(Simcore->GetFloor(temp4)->Height);
				TrimString(buffer);
				LineData = LineData.substr(0, temp1) + buffer + LineData.substr(temp1 + temp6.length());
			}

			//altitude parameter
			buffer = ToString(temp4);
			TrimString(buffer);
			temp6 = "floor(" + buffer + ").altitude";
			buffer = LineData;
			SetCase(buffer, false);
			temp1 = buffer.find(temp6, 0);
			if (temp1 > 0)
			{
				buffer = ToString(Simcore->GetFloor(temp4)->Altitude);
				TrimString(buffer);
				LineData = LineData.substr(0, temp1) + buffer + LineData.substr(temp1 + temp6.length());
			}

			//interfloorheight parameter
			buffer = ToString(temp4);
			TrimString(buffer);
			temp6 = "floor(" + buffer + ").interfloorheight";
			buffer = LineData;
			SetCase(buffer, false);
			temp1 = buffer.find(temp6, 0);
			if (temp1 > 0)
			{
				buffer = ToString(Simcore->GetFloor(temp4)->InterfloorHeight);
				TrimString(buffer);
				LineData = LineData.substr(0, temp1) + buffer + LineData.substr(temp1 + temp6.length());
			}
			temp5 = SetCaseCopy(LineData, false).find("floor(", 0);

			//base parameter
			buffer = ToString(temp4);
			TrimString(buffer);
			temp6 = "floor(" + buffer + ").base";
			buffer = LineData;
			SetCase(buffer, false);
			temp1 = buffer.find(temp6, 0);
			if (temp1 > 0)
			{
				buffer = ToString(Simcore->GetFloor(temp4)->GetBase());
				TrimString(buffer);
				LineData = LineData.substr(0, temp1) + buffer + LineData.substr(temp1 + temp6.length());
			}
			temp5 = SetCaseCopy(LineData, false).find("floor(", 0);
		}

		//Extent variables
		ReplaceAll(LineData, "%minx%", ToString(MinExtent.x));
		ReplaceAll(LineData, "%minz%", ToString(MinExtent.z));
		ReplaceAll(LineData, "%maxx%", ToString(MaxExtent.x));
		ReplaceAll(LineData, "%maxz%", ToString(MaxExtent.z));

		//reset return code
		returncode = sContinue;

		//Global parameters
		if (Section == 1)
			returncode = ProcGlobals();

		//Process floors
		else if (Section == 2)
		{
			//create floor if not created already
			Simcore->NewFloor(Current);
recalc:
			returncode = ProcFloors();
		}

		//process elevators
		else if (Section == 4)
			returncode = ProcElevators();

		//Process textures
		else if (Section == 5)
			returncode = ProcTextures();

		//Global commands
		if (returncode == sContinue)
			returncode = ProcCommands();

		//handle return values
		if (returncode == sError)
			goto Error;
		else if (returncode == sCheckFloors)
			goto checkfloors;
		else if (returncode == sBreak)
			goto breakpoint;
		else if (returncode == sRecalc)
			goto recalc;
		else if (returncode == sSkipReset)
			goto Nextline;

		//reset temporary states
		Simcore->TextureOverride = false;
		Simcore->FlipTexture = false;

Nextline:
		if (InWhile == true && InFunction == 0)
			InWhile = false;
		else
			line++;

		if (line == (int)BuildingData.size())
		{
			IsFinished = true;
			show_percent = false;
		}
	}

	return true;

Error:
	line++;
	return false;
}

bool ScriptProcessor::LoadDataFile(const char *filename, bool insert, int insert_line)
{
	//loads a building data file into the runtime buffer
	int location = insert_line;
	std::string Filename = filename;

	//if insert location is greater than array size, return with error
	if (insert == true)
		if (location > (int)BuildingData.size() - 1 || location < 0)
			return false;

	//make sure file exists
	if (Simcore->FileExists(Filename.c_str()) == false)
		return false;

	//Simcore->Report(Filename);

	//load file
#if OGRE_VERSION >= 0x00010900
	Ogre::FileSystemArchive filesystem(".", "FileSystem", true);
#else
	Ogre::FileSystemArchive filesystem(".", "FileSystem");
#endif

	Ogre::DataStreamPtr filedata;
	try
	{
		filedata = filesystem.open(filename, true);
	}
	catch (Ogre::Exception &e)
	{
		return false;
	}

	//exit if an error occurred while loading
	if(filedata.isNull())
		return false;

	Ogre::DataStreamPtr file(new Ogre::MemoryDataStream(filename, filedata, true, true));

	while (file->eof() == false)
	{
		//push next line of data onto the tail end of the BuildingData array
		std::string line = file->getLine(true);
		if (insert == false)
		{
			//append data to building array
			BuildingData.push_back(line);
			BuildingDataOrig.push_back(line);
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
		int end = location - 1;
		int lines = end - insert_line;
		int parent = -1;

		//adjust include end lines if included within another include
		for (int i = 0; i < (int)includes.size(); i++)
		{
			if (includes[i].start_line < line && includes[i].end_line > line)
			{
				includes[i].end_line += lines;
				parent = i;
			}
		}

		//adjust function lines
		for (int i = 0; i < (int)functions.size(); i++)
		{
			if (functions[i].line > line)
				functions[i].line += lines;
		}

		if (InFunction > 0)
		{
			for (int i = 0; i < (int)FunctionStack.size(); i++)
			{
				if (FunctionStack[i].CallLine > line)
					FunctionStack[i].CallLine += lines;
			}
		}

		//store include info in array
		IncludeInfo info;
		info.filename = filename;
		info.start_line = line;
		info.end_line = end;
		info.parent = parent;
		includes.push_back(info);
	}

	return true;
}

bool ScriptProcessor::LoadFromText(const char *text)
{
	//loads building commands from a string
	std::vector<std::string> textarray;
	SplitString(textarray, text, '\n');

	//feed each line of text into the script array
	BuildingData.reserve(BuildingData.size() + textarray.size());
	BuildingDataOrig.reserve(BuildingDataOrig.size() + textarray.size());
	for (int i = 0; i < (int)textarray.size(); i++)
	{
		//append data to building array
		BuildingData.push_back(textarray[i]);
		BuildingDataOrig.push_back(textarray[i]);
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

int ScriptProcessor::ScriptError(std::string message, bool warning)
{
	//report a script error, with line and context information.
	//reports a warning message (without window popup) if 'warning' is true

	//first see if the current line is from an included file

	int linenum = line;
	int newlinenum = line;
	int linenum_start = 0;
	int function_line = 0;
	int newfunction_line = 0;
	int function_line_start = 0;
	int included_lines = 0;
	int included_lines_f = 0;
	int parent = -1;
	int parent_f = -1;
	bool isinclude = false;
	bool isinclude_f = false;
	std::string includefile;
	std::string isinclude_f_file;

	if (InFunction > 0)
	{
		function_line = FunctionStack[InFunction - 1].CallLine;
		newfunction_line = function_line;
	}

	for (int i = 0; i < (int)includes.size(); i++)
	{
		if (linenum < includes[i].start_line)
			break;

		//line is part of an included file
		if (linenum >= includes[i].start_line && linenum <= includes[i].end_line)
		{
			isinclude = true;
			includefile = includes[i].filename;
			newlinenum = linenum - includes[i].start_line;
			parent = i;
			linenum_start = includes[i].start_line;
		}

		//function call line is part of an included file
		if (function_line >= includes[i].start_line && function_line <= includes[i].end_line)
		{
			isinclude_f = true;
			isinclude_f_file = includes[i].filename;
			newfunction_line = function_line - includes[i].start_line;
			parent_f = i;
			function_line_start = includes[i].start_line;
		}
	}

	linenum = newlinenum;
	function_line = newfunction_line;

	//calculate number of included lines before the current line
	for (int i = 0; i < (int)includes.size(); i++)
	{
		if (includes[i].parent == parent)
		{
			if (linenum + linenum_start > includes[i].end_line)
				included_lines += includes[i].end_line - includes[i].start_line;
		}

		if (includes[i].parent == parent_f)
		{
			if (function_line + function_line_start > includes[i].end_line)
				included_lines_f += includes[i].end_line - includes[i].start_line;
		}
	}

	//have line numbers start from 1 instead of 0
	linenum += 1;
	function_line += 1;

	//Script error reporting function
	std::string error;

	if (warning == false)
		error = "Script error ";
	else
		error = "Script warning ";

	if (isinclude == true)
		error += "in included file " + includefile + " ";

	error += "on line " + ToString2(linenum - included_lines) + ": " + message + "\nSection: " + ToString2(Section) + "\nContext: " + Context;

	if (InFunction == 0)
		error += "\nLine Text: " + LineData;
	else
	{
		error += "\nFunction: " + FunctionStack[InFunction - 1].Name;

		if (isinclude_f == true)
			error += "\nFunction included in file: " + isinclude_f_file;

		error += "\nFunction call line: " + ToString2(function_line - included_lines_f) + "\nLine Text: " + LineData;
	}

	skyscraper->ReportError(error.c_str());

	//show error dialog
	if (warning == false)
	{
		wxMessageDialog *dialog = new wxMessageDialog(0, wxString::FromAscii(error.c_str()), wxString::FromAscii("Skyscraper"), wxOK | wxICON_ERROR);
		dialog->ShowModal();

		delete dialog;
		dialog = 0;
	}
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

int ScriptProcessor::ScriptWarning(std::string message)
{
	return ScriptError(message, true);
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
		twindow->SetMinSize(wxSize(350, 250));
		twindow->tMain->SetMinSize(wxSize(350, 250));
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
		temp1 = LineData.find("[", 0);
		temp3 = LineData.find("]", 0);
		if (temp1 + temp3 > 0)
			temp2 = LineData.substr(temp1 + 1, temp3 - temp1 - 1);
		else
			temp2 = "";
		TrimString(temp2);
		if (IfProc(temp2.c_str()) == true)
		{
			//trim off IF/While statement
			LineData = LineData.substr(temp3 + 1);
			TrimString(LineData);

			if (IsIf == 2)
				InWhile = true;

			return sCheckFloors;
		}
		else
			return sNextLine; //skip line
	}

	if (Section != 2 && Section != 4)
	{
		//process math functions
		if (MathFunctions() == sError)
			return sError;

		//process any functions
		if (FunctionProc() == true)
			return sNextLine;
	}

	//AddTriangleWall command
	if (linecheck.substr(0, 15) == "addtrianglewall")
	{
		//get data
		int params = SplitData(LineData.c_str(), 16);

		if (params != 14)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 13; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
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

			if (Section == 2)
				wall = tmpMesh->CreateWallObject(Simcore->GetFloor(Current)->object, tempdata[1].c_str());
			else
				wall = tmpMesh->CreateWallObject(Simcore->External->object, tempdata[1].c_str());
		}
		else if (buffer == "landscape")
		{
			tmpMesh = Simcore->Landscape;
			wall = tmpMesh->CreateWallObject(Simcore->Landscape->object, tempdata[1].c_str());
		}
		else if (buffer == "buildings")
		{
			tmpMesh = Simcore->Buildings;
			wall = tmpMesh->CreateWallObject(Simcore->Buildings->object, tempdata[1].c_str());
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
		return sNextLine;
	}

	//AddWall command
	if (linecheck.substr(0, 7) == "addwall")
	{
		//get data
		int params = SplitData(LineData.c_str(), 8);

		if (params != 14)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 13; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//create wall
		StoreCommand(Simcore->AddWall(tempdata[0].c_str(), tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str())));
		return sNextLine;
	}

	//AddFloor command
	if (linecheck.substr(0, 9) == "addfloor ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 9);

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
				if (!IsNumeric(tempdata[i].c_str()))
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

				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		//create floor
		if (compat == true)
			StoreCommand(Simcore->AddFloor(tempdata[0].c_str(), tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), ReverseAxis, false, atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), true));
		else
			StoreCommand(Simcore->AddFloor(tempdata[0].c_str(), tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), Ogre::StringConverter::parseBool(tempdata[10]), Ogre::StringConverter::parseBool(tempdata[11]), atof(tempdata[12].c_str()), atof(tempdata[13].c_str())));
		return sNextLine;
	}

	//AddGround command
	if (linecheck.substr(0, 9) == "addground")
	{
		//get data
		int params = SplitData(LineData.c_str(), 10);

		if (params != 9)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 8; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//create tiled ground
		StoreCommand(Simcore->AddGround(tempdata[0].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atoi(tempdata[7].c_str()), atoi(tempdata[8].c_str())));
		return sNextLine;
	}

	//Cut command
	if (linecheck.substr(0, 4) == "cut ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 4);

		if (params != 9)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 6; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
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
		return sNextLine;
	}

	//Set command
	if (linecheck.substr(0, 4) == "set ")
	{
		temp1 = LineData.find("=", 0);
		if (temp1 < 0)
			return ScriptError("Syntax Error");

		std::string str = LineData.substr(4, temp1 - 5);
		TrimString(str);

		//reserved keywords
		if (str == "base" || str == "floor" || str == "height" || str == "interfloorheight" || str == "fullheight" || str == "elevator" || str == "minx" || str == "maxx" || str == "minz" || str == "maxz" || str == "number" || str.substr(0, 4) == "param")
			return ScriptError("Cannot use system variable name");

		//get text after equal sign
		temp2 = GetAfterEquals(LineData.c_str());

		//find existing variable by name
		int index = -1;
		for (int i = 0; i < (int)variables.size(); i++)
		{
			if (variables[i].name == str)
			{
				index = i;
				break;
			}
		}

		std::string value = Calc(temp2.c_str());

		if (index == -1)
		{
			//create new variable
			VariableMap variable;
			variable.name = str;
			variable.value = value;
			variables.push_back(variable);
			value = variable.value;
		}
		else
		{
			//set existing variable
			variables[index].name = str;
			variables[index].value = value;
			value = variables[index].value;
		}

		if (Simcore->Verbose == true)
			skyscraper->Report("Variable '" + str + "' set to " + value);
		return sNextLine;
	}

	//CreateWallBox2 command
	if (linecheck.substr(0, 14) == "createwallbox2")
	{
		//get data
		int params = SplitData(LineData.c_str(), 15);

		if (params != 15)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 10; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
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

			if (Section == 2)
				wall = tmpMesh->CreateWallObject(Simcore->GetFloor(Current)->object, tempdata[1].c_str());
			else
				wall = tmpMesh->CreateWallObject(Simcore->External->object, tempdata[1].c_str());
		}
		else if (buffer == "landscape")
		{
			tmpMesh = Simcore->Landscape;
			wall = tmpMesh->CreateWallObject(Simcore->Landscape->object, tempdata[1].c_str());
		}
		else if (buffer == "buildings")
		{
			tmpMesh = Simcore->Buildings;
			wall = tmpMesh->CreateWallObject(Simcore->Buildings->object, tempdata[1].c_str());
		}
		else
			return ScriptError("Invalid object");

		StoreCommand(wall);

		float voffset = atof(tempdata[8].c_str());
		if (Section == 2)
			voffset += Ogre::Real(Simcore->GetFloor(Current)->GetBase());

		Simcore->CreateWallBox2(wall, tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), voffset, atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), Ogre::StringConverter::parseBool(tempdata[11]), Ogre::StringConverter::parseBool(tempdata[12]), Ogre::StringConverter::parseBool(tempdata[13]), Ogre::StringConverter::parseBool(tempdata[14]));
		return sNextLine;
	}

	//CreateWallBox command
	if (linecheck.substr(0, 14) == "createwallbox ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 14);

		if (params != 15)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 10; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
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

			if (Section == 2)
				wall = tmpMesh->CreateWallObject(Simcore->GetFloor(Current)->object, tempdata[1].c_str());
			else
				wall = tmpMesh->CreateWallObject(Simcore->External->object, tempdata[1].c_str());
		}
		else if (buffer == "landscape")
		{
			tmpMesh = Simcore->Landscape;
			wall = tmpMesh->CreateWallObject(Simcore->Landscape->object, tempdata[1].c_str());
		}
		else if (buffer == "buildings")
		{
			tmpMesh = Simcore->Buildings;
			wall = tmpMesh->CreateWallObject(Simcore->Buildings->object, tempdata[1].c_str());
		}
		else
			return ScriptError("Invalid object");

		StoreCommand(wall);

		float voffset = atof(tempdata[8].c_str());
		if (Section == 2)
			voffset += Ogre::Real(Simcore->GetFloor(Current)->GetBase());

		Simcore->CreateWallBox(wall, tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), voffset, atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), Ogre::StringConverter::parseBool(tempdata[11]), Ogre::StringConverter::parseBool(tempdata[12]), Ogre::StringConverter::parseBool(tempdata[13]), Ogre::StringConverter::parseBool(tempdata[14]));
		return sNextLine;
	}

	//AddCustomWall command
	if (linecheck.substr(0, 14) == "addcustomwall ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 14);

		//check numeric values
		for (int i = 3; i < params; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
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
			tmpMesh = Simcore->GetElevator(Current)->ElevatorMesh;
			wall = tmpMesh->CreateWallObject(Simcore->GetElevator(Current)->object, tempdata[1].c_str());
		}
		else if (buffer == "external")
		{
			tmpMesh = Simcore->External;

			if (Section == 2)
				wall = tmpMesh->CreateWallObject(Simcore->GetFloor(Current)->object, tempdata[1].c_str());
			else
				wall = tmpMesh->CreateWallObject(Simcore->External->object, tempdata[1].c_str());
		}
		else if (buffer == "landscape")
		{
			tmpMesh = Simcore->Landscape;
			wall = tmpMesh->CreateWallObject(Simcore->Landscape->object, tempdata[1].c_str());
		}
		else if (buffer == "buildings")
		{
			tmpMesh = Simcore->Buildings;
			wall = tmpMesh->CreateWallObject(Simcore->Buildings->object, tempdata[1].c_str());
		}
		else
			return ScriptError("Invalid object");

		std::vector<Ogre::Vector3> varray;
		for (temp3 = 3; temp3 < params - 2; temp3 += 3)
			varray.push_back(Ogre::Vector3(atof(tempdata[temp3].c_str()), atof(tempdata[temp3 + 1].c_str()), atof(tempdata[temp3 + 2].c_str())));

		StoreCommand(wall);

		Simcore->AddCustomWall(wall, tempdata[1].c_str(), tempdata[2].c_str(), varray, atof(tempdata[params - 2].c_str()), atof(tempdata[params - 1].c_str()));
		return sNextLine;
	}

	//AddCustomFloor command
	if (linecheck.substr(0, 15) == "addcustomfloor ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 15);

		//check numeric values
		for (int i = 3; i < params; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
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
			tmpMesh = Simcore->GetElevator(Current)->ElevatorMesh;
			wall = tmpMesh->CreateWallObject(Simcore->GetElevator(Current)->object, tempdata[1].c_str());
		}
		else if (buffer == "external")
		{
			tmpMesh = Simcore->External;

			if (Section == 2)
				wall = tmpMesh->CreateWallObject(Simcore->GetFloor(Current)->object, tempdata[1].c_str());
			else
				wall = tmpMesh->CreateWallObject(Simcore->External->object, tempdata[1].c_str());
		}
		else if (buffer == "landscape")
		{
			tmpMesh = Simcore->Landscape;
			wall = tmpMesh->CreateWallObject(Simcore->Landscape->object, tempdata[1].c_str());
		}
		else if (buffer == "buildings")
		{
			tmpMesh = Simcore->Buildings;
			wall = tmpMesh->CreateWallObject(Simcore->Buildings->object, tempdata[1].c_str());
		}
		else
			return ScriptError("Invalid object");

		float altitude = atof(tempdata[params - 3].c_str());
		if (Section == 2)
			altitude += Simcore->GetFloor(Current)->GetBase();

		std::vector<Ogre::Vector2> varray;
		for (temp3 = 3; temp3 < params - 3; temp3 += 2)
			varray.push_back(Ogre::Vector2(atof(tempdata[temp3].c_str()), atof(tempdata[temp3 + 1].c_str())));

		StoreCommand(wall);

		Simcore->AddCustomFloor(wall, tempdata[1].c_str(), tempdata[2].c_str(), varray, altitude, atof(tempdata[params - 2].c_str()), atof(tempdata[params - 1].c_str()));
		return sNextLine;
	}

	//AddShaft command
	if (linecheck.substr(0, 9) == "addshaft ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 9);

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
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
		}
		else
		{
			for (int i = 0; i <= 4; i++)
			{
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		int startfloor, endfloor;
		if (compat == true)
		{
			startfloor = atoi(tempdata[4].c_str());
			endfloor = atoi(tempdata[5].c_str());
		}
		else
		{
			startfloor = atoi(tempdata[3].c_str());
			endfloor = atoi(tempdata[4].c_str());
		}

		if (startfloor < -Simcore->Basements)
			return ScriptError("Invalid starting floor");
		if (endfloor > Simcore->Floors - 1)
			return ScriptError("Invalid ending floor");

		Object *object;
		if (compat == true)
			object = Simcore->CreateShaft(atoi(tempdata[0].c_str()), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atoi(tempdata[4].c_str()), atoi(tempdata[5].c_str()));
		else
			object = Simcore->CreateShaft(atoi(tempdata[0].c_str()), atof(tempdata[1].c_str()), atof(tempdata[2].c_str()), atoi(tempdata[3].c_str()), atoi(tempdata[4].c_str()));

		if (!object)
			return ScriptError();

		StoreCommand(object);
		return sNextLine;
	}

	//ShaftCut command
	if (linecheck.substr(0, 9) == "shaftcut ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 9);

		if (params != 7)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 6; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//check for existence of shaft
		int shaftnum = atoi(tempdata[0].c_str());
		if (shaftnum < 1 || shaftnum > Simcore->Shafts())
			return ScriptError("Invalid shaft " + std::string(tempdata[0]));

		Simcore->GetShaft(shaftnum)->CutFloors(true, Ogre::Vector2(atof(tempdata[1].c_str()), atof(tempdata[2].c_str())), Ogre::Vector2(atof(tempdata[3].c_str()), atof(tempdata[4].c_str())), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()));
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
		if (!IsNumeric(str.c_str(), shaftnum))
			return ScriptError("Invalid shaft number");

		if (shaftnum < 1 || shaftnum > Simcore->Shafts())
			return ScriptError("Invalid shaft number");

		Simcore->GetShaft(shaftnum)->ShowFloors = true;

		int params = SplitAfterEquals(LineData.c_str(), false);
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
				if (!IsNumeric(tempdata[line].c_str(), showfloor))
					return ScriptError("Invalid value");
				Simcore->GetShaft(shaftnum)->AddShowFloor(showfloor);
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
		if (!IsNumeric(str.c_str(), shaftnum))
			return ScriptError("Invalid shaft number");
		if (shaftnum < 1 || shaftnum > Simcore->Shafts())
			return ScriptError("Invalid shaft number");
		Simcore->GetShaft(shaftnum)->ShowOutside = true;

		int params = SplitAfterEquals(LineData.c_str(), false);
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
				if (!IsNumeric(tempdata[line].c_str(), showfloor))
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
		if (!IsNumeric(str.c_str(), shaftnum))
			return ScriptError("Invalid shaft number");
		if (shaftnum < 1 || shaftnum > Simcore->Shafts())
			return ScriptError("Invalid shaft number");

		//get text after equal sign
		temp2 = GetAfterEquals(LineData.c_str());

		Simcore->GetShaft(shaftnum)->ShowFullShaft = Ogre::StringConverter::parseBool(temp2);
		return sNextLine;
	}

	//CreateStairwell command
	if (linecheck.substr(0, 15) == "createstairwell")
	{
		//get data
		int params = SplitData(LineData.c_str(), 16);

		if (params != 5)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 4; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		Object *object = Simcore->CreateStairwell(atoi(tempdata[0].c_str()), atof(tempdata[1].c_str()), atof(tempdata[2].c_str()), atoi(tempdata[3].c_str()), atoi(tempdata[4].c_str()));
		if (!object)
			return ScriptError();

		StoreCommand(object);
		return sNextLine;
	}

	//CutStairwell command
	if (linecheck.substr(0, 13) == "cutstairwell ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 13);

		if (params != 7)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 6; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		int stairwell = atoi(tempdata[0].c_str());
		if (!Simcore->GetStairs(stairwell))
			return ScriptError("Invalid stairwell");

		Simcore->GetStairs(stairwell)->CutFloors(true, Ogre::Vector2(atof(tempdata[1].c_str()), atof(tempdata[2].c_str())), Ogre::Vector2(atof(tempdata[3].c_str()), atof(tempdata[4].c_str())), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()));
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
		if (!IsNumeric(str.c_str(), stairnum))
			return ScriptError("Invalid stairwell number");

		if (stairnum < 1 || stairnum > Simcore->StairsNum())
			return ScriptError("Invalid stairwell number");

		Simcore->GetStairs(stairnum)->ShowFloors = true;

		int params = SplitAfterEquals(LineData.c_str(), false);
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
				if (!IsNumeric(str1.c_str(), start) || !IsNumeric(str2.c_str(), end))
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
				if (!IsNumeric(tempdata[line].c_str(), showfloor))
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
		if (!IsNumeric(str.c_str(), stairnum))
			return ScriptError("Invalid stairwell number");
		if (stairnum < 1 || stairnum > Simcore->StairsNum())
			return ScriptError("Invalid stairwell number");

		//get text after equal sign
		temp2 = GetAfterEquals(LineData.c_str());

		Simcore->GetStairs(stairnum)->ShowFullStairs = Ogre::StringConverter::parseBool(temp2);
		return sNextLine;
	}

	//WallOrientation command
	if (linecheck.substr(0, 15) == "wallorientation")
	{
		//get text after equal sign
		temp2 = GetAfterEquals(LineData.c_str());

		if (!Simcore->SetWallOrientation(temp2.c_str()))
			return ScriptError();
		return sNextLine;
	}

	//FloorOrientation command
	if (linecheck.substr(0, 16) == "floororientation")
	{
		//get text after equal sign
		temp2 = GetAfterEquals(LineData.c_str());

		if (!Simcore->SetFloorOrientation(temp2.c_str()))
			return ScriptError();
		return sNextLine;
	}

	//DrawWalls command
	if (linecheck.substr(0, 9) == "drawwalls")
	{
		int params = SplitAfterEquals(LineData.c_str());

		if (params != 6)
			return ScriptError("Incorrect number of parameters");

		Simcore->DrawWalls(Ogre::StringConverter::parseBool(tempdata[0]),
					Ogre::StringConverter::parseBool(tempdata[1]),
					Ogre::StringConverter::parseBool(tempdata[2]),
					Ogre::StringConverter::parseBool(tempdata[3]),
					Ogre::StringConverter::parseBool(tempdata[4]),
					Ogre::StringConverter::parseBool(tempdata[5]));
		return sNextLine;
	}

	//SetTextureMapping command
	if (linecheck.substr(0, 18) == "settexturemapping ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 18);

		if (params != 9)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 8; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		Simcore->SetTextureMapping(atoi(tempdata[0].c_str()), Ogre::Vector2(atof(tempdata[1].c_str()), atof(tempdata[2].c_str())),
									atoi(tempdata[3].c_str()), Ogre::Vector2(atof(tempdata[4].c_str()), atof(tempdata[5].c_str())),
									atoi(tempdata[6].c_str()), Ogre::Vector2(atof(tempdata[7].c_str()), atof(tempdata[8].c_str())));
		return sNextLine;
	}

	//SetTextureMapping2 command
	if (linecheck.substr(0, 18) == "settexturemapping2")
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
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		Simcore->SetTextureMapping2(tempdata[0].c_str(), tempdata[1].c_str(), tempdata[2].c_str(), Ogre::Vector2(atof(tempdata[3].c_str()), atof(tempdata[4].c_str())),
									tempdata[5].c_str(), tempdata[6].c_str(), tempdata[7].c_str(), Ogre::Vector2(atof(tempdata[8].c_str()), atof(tempdata[9].c_str())),
									tempdata[10].c_str(), tempdata[11].c_str(), tempdata[12].c_str(), Ogre::Vector2(atof(tempdata[13].c_str()), atof(tempdata[14].c_str())));
		return sNextLine;
	}

	//ResetTextureMapping command
	if (linecheck.substr(0, 19) == "resettexturemapping")
	{
		int temp2check = LineData.find("=", 0);
		if (temp2check < 0)
			return ScriptError("Syntax Error");

		//get text after equal sign
		temp2 = GetAfterEquals(LineData.c_str());

		Simcore->ResetTextureMapping(Ogre::StringConverter::parseBool(temp2));
		return sNextLine;
	}

	//SetPlanarMapping command
	if (linecheck.substr(0, 16) == "setplanarmapping")
	{
		//get data
		int params = SplitData(LineData.c_str(), 17);

		if (params != 4 && params != 5)
			return ScriptError("Incorrect number of parameters");

		if (params == 4)
		{
			Simcore->SetPlanarMapping(Ogre::StringConverter::parseBool(tempdata[0]),
					Ogre::StringConverter::parseBool(tempdata[1]),
					Ogre::StringConverter::parseBool(tempdata[2]),
					Ogre::StringConverter::parseBool(tempdata[3]), false);
			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
		}
		else
		{
			Simcore->SetPlanarMapping(Ogre::StringConverter::parseBool(tempdata[0]),
					Ogre::StringConverter::parseBool(tempdata[1]),
					Ogre::StringConverter::parseBool(tempdata[2]),
					Ogre::StringConverter::parseBool(tempdata[3]),
					Ogre::StringConverter::parseBool(tempdata[4]));
		}
		return sNextLine;
	}

	//ReverseAxis command
	if (linecheck.substr(0, 11) == "reverseaxis")
	{
		//backwards compatibility
		if (warn_deprecated == true)
			ScriptWarning("Command deprecated");

		int temp2check = LineData.find("=", 0);
		if (temp2check < 0)
			return ScriptError("Syntax Error");
		temp2 = GetAfterEquals(LineData.c_str());

		ReverseAxis = Ogre::StringConverter::parseBool(temp2);
		return sNextLine;
	}

	//Intersection points
	temp5 = linecheck.find("isect(", 0);
	while (temp5 > -1)
	{
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
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
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

		buffer = std::string(LineData).substr(0, temp5) + ToString2(isect.x) + std::string(", ") + ToString2(isect.y) + std::string(", ") + ToString2(isect.z) + std::string(LineData).substr(temp4 + 1);
		LineData = buffer;
		linecheck = SetCaseCopy(LineData, false);
		temp5 = linecheck.find("isect(", 0);
	}

	//Endpoint function
	temp5 = linecheck.find("endpoint(", 0);
	while (temp5 > -1)
	{
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
		if (tempdata.size() != 4)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 3; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		Ogre::Vector2 startpoint (atof(tempdata[0].c_str()), atof(tempdata[1].c_str()));
		Ogre::Vector2 endpoint = Simcore->GetEndPoint(startpoint, atof(tempdata[2].c_str()), atof(tempdata[3].c_str()));

		buffer = std::string(LineData).substr(0, temp5) + ToString2(endpoint.x) + std::string(", ") + ToString2(endpoint.y) + std::string(LineData).substr(temp4 + 1);
		LineData = buffer;
		linecheck = SetCaseCopy(LineData, false);
		temp5 = linecheck.find("endpoint(", 0);
	}

	//GetWallExtents command
	if (linecheck.substr(0, 14) == "getwallextents")
	{
		//get data
		int params = SplitData(LineData.c_str(), 15);

		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		if (!IsNumeric(tempdata[2].c_str()))
			return ScriptError("Invalid value: " + tempdata[2]);

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
		return sNextLine;
	}

	//SetAutoSize command
	if (linecheck.substr(0, 11) == "setautosize")
	{
		int params = SplitAfterEquals(LineData.c_str());

		if (params != 2)
			return ScriptError("Incorrect number of parameters");

		Simcore->SetAutoSize(Ogre::StringConverter::parseBool(tempdata[0]),
					Ogre::StringConverter::parseBool(tempdata[1]));
		return sNextLine;
	}

	//TextureOverride command
	if (linecheck.substr(0, 15) == "textureoverride")
	{
		int params = SplitData(LineData.c_str(), 16, false);

		if (params != 6)
			return ScriptError("Incorrect number of parameters");

		Simcore->SetTextureOverride(tempdata[0].c_str(), tempdata[1].c_str(), tempdata[2].c_str(), tempdata[3].c_str(), tempdata[4].c_str(), tempdata[5].c_str());
		return sSkipReset;
	}

	//TextureFlip command
	if (linecheck.substr(0, 11) == "textureflip")
	{
		int params = SplitData(LineData.c_str(), 12, false);

		if (params != 6)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 5; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		Simcore->SetTextureFlip(atoi(tempdata[0].c_str()), atoi(tempdata[1].c_str()), atoi(tempdata[2].c_str()), atoi(tempdata[3].c_str()), atoi(tempdata[4].c_str()), atoi(tempdata[5].c_str()));
		return sSkipReset;
	}

	//Mount command
	if (linecheck.substr(0, 5) == "mount")
	{
		//get data
		int params = SplitData(LineData.c_str(), 6, false);

		if (params != 2)
			return ScriptError("Incorrect number of parameters");

		if (!Simcore->Mount(tempdata[0].c_str(), tempdata[1].c_str()))
			return ScriptError();

		return sNextLine;
	}

	//AddFloorAutoArea command
	if (linecheck.substr(0, 16) == "addfloorautoarea")
	{
		//get data
		int params = SplitData(LineData.c_str(), 17);

		if (params != 6)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 5; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//create floor auto area
		Simcore->AddFloorAutoArea(Ogre::Vector3(atof(tempdata[0].c_str()), atof(tempdata[1].c_str()), atof(tempdata[2].c_str())), Ogre::Vector3(atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str())));
		return sNextLine;
	}

	//AddSound command
	if (linecheck.substr(0, 8) == "addsound")
	{
		//get data
		int params = SplitData(LineData.c_str(), 9, true, 1);

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
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}
		else
		{
			if (compat == true)
			{
				for (int i = 2; i <= 12; i++)
				{
					if (!IsNumeric(tempdata[i].c_str()))
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

					if (!IsNumeric(tempdata[i].c_str()))
						return ScriptError("Invalid value: " + tempdata[i]);
				}
			}
		}

		//check to see if file exists
		CheckFile(std::string("data/" + tempdata[1]).c_str());

		if (compat == true)
		{
			if (partial == true)
				StoreCommand(Simcore->AddSound(tempdata[0].c_str(), tempdata[1].c_str(), Ogre::Vector3(atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()))));
			else
				StoreCommand(Simcore->AddSound(tempdata[0].c_str(), tempdata[1].c_str(), Ogre::Vector3(atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str())), true, atof(tempdata[5].c_str()), atoi(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), 0.0, 360, 360, 1.0, Ogre::Vector3(atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()))));
		}
		else
		{
			if (partial == true)
				StoreCommand(Simcore->AddSound(tempdata[0].c_str(), tempdata[1].c_str(), Ogre::Vector3(atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str())), Ogre::StringConverter::parseBool(tempdata[5])));
			else
				StoreCommand(Simcore->AddSound(tempdata[0].c_str(), tempdata[1].c_str(), Ogre::Vector3(atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str())), Ogre::StringConverter::parseBool(tempdata[5]), atof(tempdata[6].c_str()), atoi(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()), Ogre::Vector3(atof(tempdata[14].c_str()), atof(tempdata[15].c_str()), atof(tempdata[16].c_str()))));
		}
		return sNextLine;
	}

	//AddModel command
	if (linecheck.substr(0, 8) == "addmodel")
	{
		//get data
		int params = SplitData(LineData.c_str(), 9, true, 1);

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
				if (!IsNumeric(tempdata[i].c_str()))
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
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		//check to see if file exists
		CheckFile(std::string("data/" + tempdata[1]).c_str());

		//create model
		Object* obj;
		if (compat == true)
			obj = Simcore->AddModel(tempdata[0].c_str(), tempdata[1].c_str(), false, Ogre::Vector3(atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str())), Ogre::Vector3(atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str())), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), Ogre::StringConverter::parseBool(tempdata[10]), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()));
		else
			obj = Simcore->AddModel(tempdata[0].c_str(), tempdata[1].c_str(), Ogre::StringConverter::parseBool(tempdata[2]), Ogre::Vector3(atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str())), Ogre::Vector3(atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str())), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), Ogre::StringConverter::parseBool(tempdata[11]), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()), atof(tempdata[14].c_str()));

		if (setkey == true && obj)
		{
			Model *mod = (Model*)obj->GetRawObject();
			mod->SetKey(keyvalue);
		}
		StoreCommand(obj);
		return sNextLine;
	}

	//AddAction command
	if (linecheck.substr(0, 10) == "addaction ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 10);

		if (params < 3)
			return ScriptError("Incorrect number of parameters");

		std::vector<Object*> objects;
		std::string tmpname = tempdata[1];
		SetCase(tmpname, false);
		if (tmpname == "global")
			objects.push_back(Simcore->object);
		else
			objects = Simcore->GetObjectRange(tempdata[1].c_str());

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
		int params = SplitData(LineData.c_str(), 16);

		if (params != 2)
			return ScriptError("Incorrect number of parameters");

		std::vector<Object*> objects;
		std::string tmpname = tempdata[1];
		SetCase(tmpname, false);
		if (tmpname == "global")
			objects.push_back(Simcore->object);
		else
			objects = Simcore->GetObjectRange(tempdata[1].c_str());

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
		int params = SplitData(LineData.c_str(), 19);

		if (params != 2)
			return ScriptError("Incorrect number of parameters");

		std::vector<Object*> objects;
		std::string tmpname = tempdata[1];
		SetCase(tmpname, false);
		if (tmpname == "global")
			objects.push_back(Simcore->object);
		else
			objects = Simcore->GetObjectRange(tempdata[1].c_str());

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
		int params = SplitData(LineData.c_str(), 17);

		if (params < 10)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 7; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

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

		Object *obj = Simcore->AddControl(tempdata[0].c_str(), tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), action_array, tex_array);

		if (obj)
		{
			Control* control = (Control*)obj->GetRawObject();
			if (lockvalue == 0)
				control->SetLocked(false, keyvalue);
			else
				control->SetLocked(true, keyvalue);
		}
		StoreCommand(obj);
		return sNextLine;
	}

	//AddTrigger command
	if (linecheck.substr(0, 10) == "addtrigger")
	{
		//get data
		int params = SplitData(LineData.c_str(), 11);

		if (params < 9)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 7; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::vector<std::string> action_array;

		//get number of action & texture parameters
		for (int i = 8; i < params; i++)
			action_array.push_back(tempdata[i]);

		//check to see if file exists
		CheckFile(std::string("data/" + tempdata[1]).c_str());

		Ogre::Vector3 min = Ogre::Vector3(atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()));
		Ogre::Vector3 max = Ogre::Vector3(atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()));
		StoreCommand(Simcore->AddTrigger(tempdata[0].c_str(), tempdata[1].c_str(), min, max, action_array));
		return sNextLine;
	}

	//SetKey command
	if (linecheck.substr(0, 6) == "setkey")
	{
		//get data
		int params = SplitData(LineData.c_str(), 7);

		if (params != 1)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		if (!IsNumeric(tempdata[0].c_str()))
			return ScriptError("Invalid value: " + tempdata[0]);

		keyvalue = atoi(tempdata[0].c_str());
		if (keyvalue < 1)
		{
			keyvalue = 0;
			return ScriptError("Incorrect key ID number");
		}

		setkey = true;
		return sNextLine;
	}

	//SetLock command
	if (linecheck.substr(0, 7) == "setlock")
	{
		//get data
		int params = SplitData(LineData.c_str(), 8);

		if (params != 2)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 1; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		lockvalue = atoi(tempdata[0].c_str());
		if (lockvalue < 0 || lockvalue > 3)
		{
			lockvalue = 0;
			keyvalue = 0;
			return ScriptError("Incorrect lock parameter");
		}

		keyvalue = atoi(tempdata[1].c_str());
		if (keyvalue < 0)
		{
			lockvalue = 0;
			keyvalue = 0;
			return ScriptError("Incorrect key ID number");
		}
		return sNextLine;
	}

	//Delete command
	if (linecheck.substr(0, 6) == "delete")
	{
		//calculate inline math
		buffer = Calc(LineData.substr(7).c_str());

		int obj;
		if (!IsNumeric(buffer.c_str(), obj))
			return ScriptError("Invalid value: " + buffer);

		//delete object
		Simcore->DeleteObject(obj);

		return sNextLine;
	}

	//RunAction command
	if (linecheck.substr(0, 9) == "runaction")
	{
		//calculate inline math
		buffer = Calc(LineData.substr(10).c_str());

		//run action
		Simcore->RunAction(buffer);

		return sNextLine;
	}

	//GotoFloor command
	if (linecheck.substr(0, 9) == "gotofloor")
	{
		//calculate inline math
		buffer = Calc(LineData.substr(10).c_str());

		int num;
		if (!IsNumeric(buffer.c_str(), num))
			return ScriptError("Invalid value: " + buffer);

		Simcore->camera->GotoFloor(num);

		return sNextLine;
	}

	//Print command
	if (linecheck.substr(0, 5) == "print")
	{
		//calculate inline math
		buffer = Calc(LineData.substr(6).c_str());

		//print line
		skyscraper->Report(buffer.c_str());

		return sNextLine;
	}

	return sContinue;
}

int ScriptProcessor::ProcGlobals()
{
	//process global parameters

	//get text after equal sign
	temp2 = GetAfterEquals(LineData.c_str());

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
		skyscraper->SkyName = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 10) == "collisions")
	{
		SetCase(temp2, false);
		if (temp2 == "false")
			Simcore->camera->EnableCollisions(false);
		else
			Simcore->camera->EnableCollisions(true);
		return sNextLine;
	}
	if (linecheck.substr(0, 7) == "gravity")
	{
		SetCase(temp2, false);
		if (temp2 == "false")
			Simcore->camera->EnableGravity(false);
		else
			Simcore->camera->EnableGravity(true);
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "camerafloor")
	{
		int data;
		if (!IsNumeric(temp2.c_str(), data))
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
		if (!IsNumeric(str1.c_str(), x) || !IsNumeric(str2.c_str(), z))
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
		if (!IsNumeric(str1.c_str(), x) || !IsNumeric(str2.c_str(), y) || !IsNumeric(str3.c_str(), z))
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
		if (!IsNumeric(str1.c_str(), x) || !IsNumeric(str2.c_str(), y) || !IsNumeric(str3.c_str(), z))
			return ScriptError("Invalid direction");

		Simcore->camera->SetStartRotation(Ogre::Vector3(x, y, z));
		return sNextLine;
	}
	if (linecheck.substr(0, 15) == "interfloorontop")
	{
		Simcore->InterfloorOnTop = Ogre::StringConverter::parseBool(temp2);
		return sNextLine;
	}
	return sContinue;
}

int ScriptProcessor::ProcFloors()
{
	//process floors

	Floor *floor = Simcore->GetFloor(Current);

	//exit with error if floor is invalid
	if (!floor)
	{
		std::string floornum;
		floornum = ToString(Current);
		return ScriptError("Invalid floor " + floornum);
	}

	//cache floor parameters
	if (cache_current != Current || floorcache_firstrun == true)
	{
		cache_current = Current;
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
	ReplaceAll(LineData, "%floor%", cache_current_s.c_str());
	ReplaceAll(LineData, "%height%", cache_height_s.c_str());
	ReplaceAll(LineData, "%fullheight%", cache_fullheight_s.c_str());
	ReplaceAll(LineData, "%interfloorheight%", cache_interfloorheight_s.c_str());
	ReplaceAll(LineData, "%base%", cache_base_s.c_str());

	if (getfloordata == true)
		return sCheckFloors;

	//IF/While statement stub (continue to global commands for processing)
	if (SetCaseCopy(LineData.substr(0, 2), false) == "if" || SetCaseCopy(LineData.substr(0, 5), false) == "while")
		return sContinue;

	//process math functions
	if (MathFunctions() == sError)
		return sError;

	//process functions
	if (FunctionProc() == true)
		return sNextLine;

	//get text after equal sign
	int temp2check = LineData.find("=", 0);
	temp2 = GetAfterEquals(LineData.c_str());

	//create a lowercase string of the line
	std::string linecheck = SetCaseCopy(LineData, false);

	//parameters
	if (linecheck.substr(0, 6) == "height")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		std::string str = Calc(temp2.c_str());
		if (!IsNumeric(str.c_str(), floor->Height))
			return ScriptError("Invalid value");
		if (FloorCheck < 2)
			FloorCheck = 1;
		else
			FloorCheck = 3;
	}
	if (linecheck.substr(0, 16) == "interfloorheight")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		std::string str = Calc(temp2.c_str());
		if (!IsNumeric(str.c_str(), floor->InterfloorHeight))
			return ScriptError("Invalid value");
		if (FloorCheck == 0)
			FloorCheck = 2;
		else
			FloorCheck = 3;
	}
	if (linecheck.substr(0, 8) == "altitude")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		std::string str = Calc(temp2.c_str());
		if (!IsNumeric(str.c_str(), floor->Altitude))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 2) == "id")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		floor->ID = Calc(temp2.c_str());
		return sNextLine;
	}
	if (linecheck.substr(0, 4) == "name")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		floor->Name = Calc(temp2.c_str());
		return sNextLine;
	}
	if (linecheck.substr(0, 4) == "type")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		floor->FloorType = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "description")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		floor->Description = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 16) == "indicatortexture")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		floor->IndicatorTexture = Calc(temp2.c_str());
		return sNextLine;
	}
	if (linecheck.substr(0, 5) == "group")
	{
		//copy string listing of group floors into array

		int params = SplitAfterEquals(LineData.c_str(), false);
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
				if (!IsNumeric(tempdata[line].c_str(), data))
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
		if (RangeL != RangeH)
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
		int params = SplitData(LineData.c_str(), 9);

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
				if (!IsNumeric(tempdata[i].c_str()))
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
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		//create floor
		if (compat == true)
			StoreCommand(floor->AddFloor(tempdata[0].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), ReverseAxis, false, atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), Ogre::StringConverter::parseBool(tempdata[11]), true));
		else
			StoreCommand(floor->AddFloor(tempdata[0].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), Ogre::StringConverter::parseBool(tempdata[9]), Ogre::StringConverter::parseBool(tempdata[10]), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), Ogre::StringConverter::parseBool(tempdata[13])));
		return sNextLine;
	}

	//AddShaftFloor command
	if (linecheck.substr(0, 13) == "addshaftfloor")
	{
		//get data
		int params = SplitData(LineData.c_str(), 14);

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
				if (!IsNumeric(tempdata[i].c_str()))
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
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		//create floor
		if (Simcore->GetShaft(atoi(tempdata[0].c_str())))
		{
			if (compat == true)
				StoreCommand(Simcore->GetShaft(atoi(tempdata[0].c_str()))->AddFloor(Current, tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), ReverseAxis, false, atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), true));
			else
				StoreCommand(Simcore->GetShaft(atoi(tempdata[0].c_str()))->AddFloor(Current, tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), Ogre::StringConverter::parseBool(tempdata[10]), Ogre::StringConverter::parseBool(tempdata[11]), atof(tempdata[12].c_str()), atof(tempdata[13].c_str())));
		}
		else
			return ScriptError("Invalid shaft");
		return sNextLine;
	}

	//AddStairsFloor command
	if (linecheck.substr(0, 14) == "addstairsfloor")
	{
		//get data
		int params = SplitData(LineData.c_str(), 14);

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
				if (!IsNumeric(tempdata[i].c_str()))
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
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		//create floor
		if (Simcore->GetStairs(atoi(tempdata[0].c_str())))
		{
			if (compat == true)
				StoreCommand(Simcore->GetStairs(atoi(tempdata[0].c_str()))->AddFloor(Current, tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), ReverseAxis, false, atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), true));
			else
				StoreCommand(Simcore->GetStairs(atoi(tempdata[0].c_str()))->AddFloor(Current, tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), Ogre::StringConverter::parseBool(tempdata[10]), Ogre::StringConverter::parseBool(tempdata[11]), atof(tempdata[12].c_str()), atof(tempdata[13].c_str())));
		}
		else
			return ScriptError("Invalid stairwell");
		return sNextLine;
	}

	//AddInterFloorFloor command
	if (linecheck.substr(0, 18) == "addinterfloorfloor")
	{
		//get data
		int params = SplitData(LineData.c_str(), 19);

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
				if (!IsNumeric(tempdata[i].c_str()))
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
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		//create floor
		if (compat == true)
			StoreCommand(floor->AddInterfloorFloor(tempdata[0].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), ReverseAxis, false, atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), true));
		else
			StoreCommand(floor->AddInterfloorFloor(tempdata[0].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), Ogre::StringConverter::parseBool(tempdata[9]), Ogre::StringConverter::parseBool(tempdata[10]), atof(tempdata[11].c_str()), atof(tempdata[12].c_str())));
		return sNextLine;
	}

	//AddWall command
	if (linecheck.substr(0, 7) == "addwall")
	{
		//get data
		int params = SplitData(LineData.c_str(), 8);

		if (params != 14)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 12; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//create wall
		StoreCommand(floor->AddWall(tempdata[0].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), Ogre::StringConverter::parseBool(tempdata[13])));
		return sNextLine;
	}

	//AddShaftWall command
	if (linecheck.substr(0, 12) == "addshaftwall")
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
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//create wall
		if (Simcore->GetShaft(atoi(tempdata[0].c_str())))
			StoreCommand(Simcore->GetShaft(atoi(tempdata[0].c_str()))->AddWall(Current, tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str())));
		else
			return ScriptError("Invalid shaft");
		return sNextLine;
	}

	//AddStairsWall command
	if (linecheck.substr(0, 13) == "addstairswall")
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
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//create wall
		if (Simcore->GetStairs(atoi(tempdata[0].c_str())))
			StoreCommand(Simcore->GetStairs(atoi(tempdata[0].c_str()))->AddWall(Current, tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str())));
		else
			return ScriptError("Invalid stairwell");
		return sNextLine;
	}

	//AddInterFloorWall command
	if (linecheck.substr(0, 17) == "addinterfloorwall")
	{
		//get data
		int params = SplitData(LineData.c_str(), 18);

		if (params != 13)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 12; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//create wall
		StoreCommand(floor->AddInterfloorWall(tempdata[0].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str())));
		return sNextLine;
	}

	//ColumnWallBox command
	if (linecheck.substr(0, 14) == "columnwallbox ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 14);

		if (params != 14)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 9; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		StoreCommand(floor->ColumnWallBox(tempdata[0].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), Ogre::StringConverter::parseBool(tempdata[10]), Ogre::StringConverter::parseBool(tempdata[11]), Ogre::StringConverter::parseBool(tempdata[12]), Ogre::StringConverter::parseBool(tempdata[13])));
		return sNextLine;
	}

	//ColumnWallBox2 command
	if (linecheck.substr(0, 14) == "columnwallbox2")
	{
		//get data
		int params = SplitData(LineData.c_str(), 15);

		if (params != 14)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 9; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		StoreCommand(floor->ColumnWallBox2(tempdata[0].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), Ogre::StringConverter::parseBool(tempdata[10]), Ogre::StringConverter::parseBool(tempdata[11]), Ogre::StringConverter::parseBool(tempdata[12]), Ogre::StringConverter::parseBool(tempdata[13])));
		return sNextLine;
	}

	//CallButtonElevators command
	if (linecheck.substr(0, 19) == "callbuttonelevators")
	{
		//construct array containing floor numbers
		int params = SplitAfterEquals(LineData.c_str(), false);
		if (params < 1)
			return ScriptError("Syntax Error");

		callbutton_elevators.clear();
		callbutton_elevators.resize(params);

		for (int line = 0; line < params; line++)
		{
			int elevnumber;
			if (!IsNumeric(tempdata[line].c_str(), elevnumber))
				return ScriptError("Invalid elevator number");
			if (elevnumber < 1 || elevnumber > Simcore->Elevators())
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
		int params = SplitData(LineData.c_str(), 18);

		bool compat = false;
		if (params == 12)
			compat = true;

		if (compat == true)
		{
			//check numeric values
			for (int i = 3; i <= 11; i++)
			{
				if (i == 6 || i == 9) //skip non-numeric parameters
					i++;
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
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
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		//create call button
		Object *obj = 0;
		if (compat == true)
			obj = floor->AddCallButtons(callbutton_elevators, tempdata[0].c_str(), tempdata[1].c_str(), tempdata[1].c_str(), tempdata[2].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), tempdata[6].c_str(), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), Ogre::StringConverter::parseBool(tempdata[9]), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()));
		else
			obj = floor->AddCallButtons(callbutton_elevators, tempdata[0].c_str(), tempdata[1].c_str(), tempdata[2].c_str(), tempdata[3].c_str(), tempdata[4].c_str(), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), tempdata[8].c_str(), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), Ogre::StringConverter::parseBool(tempdata[11]), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()));

		if (obj)
		{
			CallButton* callbutton = (CallButton*)obj->GetRawObject();
			if (lockvalue == 0)
				callbutton->SetLocked(false, keyvalue);
			else
				callbutton->SetLocked(true, keyvalue);
		}
		StoreCommand(obj);
		return sNextLine;
	}

	//AddStairs command
	if (linecheck.substr(0, 10) == "addstairs ")
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
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//create stairs
		if (Simcore->GetStairs(atoi(tempdata[0].c_str())))
			StoreCommand(Simcore->GetStairs(atoi(tempdata[0].c_str()))->AddStairs(Current, tempdata[1].c_str(), tempdata[2].c_str(), tempdata[3].c_str(), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atoi(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str())));
		else
			return ScriptError("Invalid stairwell");
		return sNextLine;
	}

	//AddDoor command
	if (linecheck.substr(0, 8) == "adddoor ")
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
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			compat = 1;
		}
		if (params == 12)
		{
			for (int i = 3; i <= 11; i++)
			{
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			compat = 2;
		}
		if (params == 13)
		{
			for (int i = 3; i <= 12; i++)
			{
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			compat = 3;
		}
		if (params == 14)
		{
			for (int i = 4; i <= 13; i++)
			{
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		//check to see if file exists
		if (compat != 1)
		{
			CheckFile(std::string("data/" + tempdata[0]).c_str());
			CheckFile(std::string("data/" + tempdata[1]).c_str());
		}

		if (compat > 0 && warn_deprecated == true)
			ScriptWarning("Syntax deprecated");

		//create door
		Object *obj;

		if (compat == 1)
			obj = floor->AddDoor("", "", false, tempdata[0].c_str(), atof(tempdata[1].c_str()), atoi(tempdata[2].c_str()), 0, atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()));
		if (compat == 2)
			obj = floor->AddDoor(tempdata[0].c_str(), tempdata[1].c_str(), false, tempdata[2].c_str(), atof(tempdata[3].c_str()), atoi(tempdata[4].c_str()), 0, atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()));
		if (compat == 3)
			obj = floor->AddDoor(tempdata[0].c_str(), tempdata[1].c_str(), false, tempdata[2].c_str(), atof(tempdata[3].c_str()), atoi(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()));
		if (compat == 0)
			obj = floor->AddDoor(tempdata[0].c_str(), tempdata[1].c_str(), Ogre::StringConverter::parseBool(tempdata[2]), tempdata[3].c_str(), atof(tempdata[4].c_str()), atoi(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()));

		if (obj)
		{
			Door* door = (Door*)obj->GetRawObject();
			door->SetLocked(lockvalue, keyvalue);
		}
		StoreCommand(obj);
		return sNextLine;
	}

	//AddStairsDoor command
	if (linecheck.substr(0, 14) == "addstairsdoor ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 14);

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
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
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
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
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
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
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
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		//check to see if file exists
		if (compat != 1)
		{
			CheckFile(std::string("data/" + tempdata[1]).c_str());
			CheckFile(std::string("data/" + tempdata[2]).c_str());
		}

		if (compat > 0 && warn_deprecated == true)
			ScriptWarning("Syntax deprecated");

		//create door
		if (Simcore->GetStairs(atoi(tempdata[0].c_str())))
		{
			Object *obj;

			if (compat == 1)
				obj = Simcore->GetStairs(atoi(tempdata[0].c_str()))->AddDoor(Current, "", "", false, tempdata[1].c_str(), atof(tempdata[2].c_str()), atoi(tempdata[3].c_str()), 0, atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()));
			if (compat == 2)
				obj = Simcore->GetStairs(atoi(tempdata[0].c_str()))->AddDoor(Current, tempdata[1].c_str(), tempdata[2].c_str(), false, tempdata[3].c_str(), atof(tempdata[4].c_str()), atoi(tempdata[5].c_str()), 0, atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()));
			if (compat == 3)
				obj = Simcore->GetStairs(atoi(tempdata[0].c_str()))->AddDoor(Current, tempdata[1].c_str(), tempdata[2].c_str(), false, tempdata[3].c_str(), atof(tempdata[4].c_str()), atoi(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()));
			if (compat == 0)
				obj = Simcore->GetStairs(atoi(tempdata[0].c_str()))->AddDoor(Current, tempdata[1].c_str(), tempdata[2].c_str(), Ogre::StringConverter::parseBool(tempdata[3]), tempdata[4].c_str(), atof(tempdata[5].c_str()), atoi(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()), atof(tempdata[14].c_str()));

			if (obj)
			{
				Door* door = (Door*)obj->GetRawObject();
				door->SetLocked(lockvalue, keyvalue);
			}
			StoreCommand(obj);
		}
		else
			return ScriptError("Invalid stairwell");
		return sNextLine;
	}

	//AddShaftStdDoor command
	if (linecheck.substr(0, 16) == "addshaftstddoor ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 14);

		if (params != 17)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 14; i++)
		{
			if (i == 1)
				i = 5; //skip non-numeric parameters
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//check to see if file exists
		CheckFile(std::string("data/" + tempdata[1]).c_str());
		CheckFile(std::string("data/" + tempdata[2]).c_str());

		//create door
		if (Simcore->GetShaft(atoi(tempdata[0].c_str())))
		{
			Object *obj;
			obj = Simcore->GetShaft(atoi(tempdata[0].c_str()))->AddDoor(Current, tempdata[1].c_str(), tempdata[2].c_str(), Ogre::StringConverter::parseBool(tempdata[3]), tempdata[4].c_str(), atof(tempdata[5].c_str()), atoi(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()), atof(tempdata[14].c_str()));

			if (obj)
			{
				Door* door = (Door*)obj->GetRawObject();
				door->SetLocked(lockvalue, keyvalue);
			}
			StoreCommand(obj);
		}
		else
			return ScriptError("Invalid shaft");
		return sNextLine;
	}

	//AddDirectionalIndicator command
	if (linecheck.substr(0, 23) == "adddirectionalindicator")
	{
		//get data
		int params = SplitData(LineData.c_str(), 24);

		if (params != 18 && params != 19)
			return ScriptError("Incorrect number of parameters");

		bool compat = false;

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
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			compat = true;
			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
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
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		if (!Simcore->GetElevator(atoi(tempdata[0].c_str())))
			return ScriptError("Invalid elevator");

		if (compat == true)
			StoreCommand(floor->AddDirectionalIndicator(atoi(tempdata[0].c_str()), Ogre::StringConverter::parseBool(tempdata[1]), false, Ogre::StringConverter::parseBool(tempdata[2]), Ogre::StringConverter::parseBool(tempdata[3]), tempdata[4].c_str(), tempdata[5].c_str(), tempdata[6].c_str(), tempdata[7].c_str(), tempdata[8].c_str(), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), tempdata[12].c_str(), atof(tempdata[13].c_str()), atof(tempdata[14].c_str()), Ogre::StringConverter::parseBool(tempdata[15]), atof(tempdata[16].c_str()), atof(tempdata[17].c_str())));
		else
			StoreCommand(floor->AddDirectionalIndicator(atoi(tempdata[0].c_str()), Ogre::StringConverter::parseBool(tempdata[1]), Ogre::StringConverter::parseBool(tempdata[2]), Ogre::StringConverter::parseBool(tempdata[3]), Ogre::StringConverter::parseBool(tempdata[4]), tempdata[5].c_str(), tempdata[6].c_str(), tempdata[7].c_str(), tempdata[8].c_str(), tempdata[9].c_str(), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), tempdata[13].c_str(), atof(tempdata[14].c_str()), atof(tempdata[15].c_str()), Ogre::StringConverter::parseBool(tempdata[16]), atof(tempdata[17].c_str()), atof(tempdata[18].c_str())));
		return sNextLine;
	}

	//AddShaftDoor command
	if (linecheck.substr(0, 13) == "addshaftdoor ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 13);

		if (params != 5 && params != 6 && params != 10)
			return ScriptError("Incorrect number of parameters");

		int compat = 0;
		if (params == 5)
			compat = 1; //1.4 compatibility mode
		if (params == 6)
			compat = 2;

		//exit if the SetShaftDoors command was never used
		if (compat > 0 && setshaftdoors == false)
			return ScriptError("SetShaftDoors must be used before AddShaftDoor");

		//check numeric values
		if (compat == 0)
		{
			for (int i = 0; i <= 9; i++)
			{
				if (i == 2)
					i = 4;
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}
		if (compat == 1)
		{
			for (int i = 0; i <= 4; i++)
			{
				if (i == 2)
					i = 3;
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}
		if (compat == 2)
		{
			for (int i = 0; i <= 5; i++)
			{
				if (i == 2)
					i = 4;
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		if (compat > 0 && warn_deprecated == true)
			ScriptWarning("Syntax deprecated");

		if (!Simcore->GetElevator(atoi(tempdata[0].c_str())))
			return ScriptError("Invalid elevator");

		if (compat == 0)
			StoreCommand(Simcore->GetElevator(atoi(tempdata[0].c_str()))->AddShaftDoor(Current, atoi(tempdata[1].c_str()), tempdata[2].c_str(), tempdata[3].c_str(), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str())));
		if (compat == 1)
			StoreCommand(Simcore->GetElevator(atoi(tempdata[0].c_str()))->AddShaftDoor(Current, atoi(tempdata[1].c_str()), tempdata[2].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str())));
		if (compat == 2)
			StoreCommand(Simcore->GetElevator(atoi(tempdata[0].c_str()))->AddShaftDoor(Current, atoi(tempdata[1].c_str()), tempdata[2].c_str(), tempdata[3].c_str(), atof(tempdata[4].c_str()), atof(tempdata[5].c_str())));
		return sNextLine;
	}

	//AddFloorIndicator command
	if (linecheck.substr(0, 17) == "addfloorindicator")
	{
		//get data
		int params = SplitData(LineData.c_str(), 18);

		if (params != 8 && params != 9)
			return ScriptError("Incorrect number of parameters");

		bool compat = false;
		if (params == 8)
			compat = true; //1.4 compatibility mode

		//check numeric values
		if (compat == true)
		{
			for (int i = 0; i <= 7; i++)
			{
				if (i == 1)
					i = 3;
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
		}
		else
		{
			for (int i = 0; i <= 8; i++)
			{
				if (i == 1)
					i = 4;
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		if (compat == false)
			StoreCommand(floor->AddFloorIndicator(atoi(tempdata[0].c_str()), Ogre::StringConverter::parseBool(tempdata[1]), tempdata[2].c_str(), tempdata[3].c_str(), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str())));
		else
			StoreCommand(floor->AddFloorIndicator(atoi(tempdata[0].c_str()), Ogre::StringConverter::parseBool(tempdata[1]), "Button", tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str())));
		return sNextLine;
	}

	//AddFillerWalls command
	if (linecheck.substr(0, 14) == "addfillerwalls")
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
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		floor->AddFillerWalls(tempdata[0].c_str(), atof(tempdata[1].c_str()), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), Ogre::StringConverter::parseBool(tempdata[7]), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()));
		return sNextLine;
	}

	//AddSound command
	if (linecheck.substr(0, 8) == "addsound")
	{
		//get data
		int params = SplitData(LineData.c_str(), 9, true, 1);

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
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}
		else
		{
			if (compat == true)
			{
				for (int i = 2; i <= 12; i++)
				{
					if (!IsNumeric(tempdata[i].c_str()))
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

					if (!IsNumeric(tempdata[i].c_str()))
						return ScriptError("Invalid value: " + tempdata[i]);
				}
			}
		}

		//check to see if file exists
		CheckFile(std::string("data/" + tempdata[1]).c_str());

		if (compat == true)
		{
			if (partial == true)
				StoreCommand(floor->AddSound(tempdata[0].c_str(), tempdata[1].c_str(), Ogre::Vector3(atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()))));
			else
				StoreCommand(floor->AddSound(tempdata[0].c_str(), tempdata[1].c_str(), Ogre::Vector3(atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str())), true, atof(tempdata[5].c_str()), atoi(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), 0.0, 360, 360, 1.0, Ogre::Vector3(atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()))));
		}
		else
		{
			if (partial == true)
				StoreCommand(floor->AddSound(tempdata[0].c_str(), tempdata[1].c_str(), Ogre::Vector3(atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str())), Ogre::StringConverter::parseBool(tempdata[5])));
			else
				StoreCommand(floor->AddSound(tempdata[0].c_str(), tempdata[1].c_str(), Ogre::Vector3(atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str())), Ogre::StringConverter::parseBool(tempdata[5]), atof(tempdata[6].c_str()), atoi(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()), Ogre::Vector3(atof(tempdata[14].c_str()), atof(tempdata[15].c_str()), atof(tempdata[16].c_str()))));
		}
		return sNextLine;
	}

	//AddShaftDoorComponent command
	if (linecheck.substr(0, 21) == "addshaftdoorcomponent")
	{
		//get data
		int params = SplitData(LineData.c_str(), 22);

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

		for (int i = 0; i <= params - 1; i++)
		{
			if (i == 2)
				i = 5;
			if (i == 6)
				i++;
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		Elevator *elev = Simcore->GetElevator(atoi(tempdata[0].c_str()));
		if (!elev)
			return ScriptError("Invalid elevator");

		if (compat == true)
			StoreCommand(elev->AddShaftDoorComponent(atoi(tempdata[1].c_str()), Current, tempdata[2].c_str(), tempdata[3].c_str(), tempdata[4].c_str(), atof(tempdata[5].c_str()), tempdata[6].c_str(), atof(tempdata[7].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()), atof(tempdata[14].c_str()), atof(tempdata[15].c_str()), atof(tempdata[16].c_str()), atof(tempdata[17].c_str())));
		else
			StoreCommand(elev->AddShaftDoorComponent(atoi(tempdata[1].c_str()), Current, tempdata[2].c_str(), tempdata[3].c_str(), tempdata[4].c_str(), atof(tempdata[5].c_str()), tempdata[6].c_str(), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()), atof(tempdata[14].c_str()), atof(tempdata[15].c_str()), atof(tempdata[16].c_str()), atof(tempdata[17].c_str()), atof(tempdata[18].c_str())));
		return sNextLine;
	}

	//FinishShaftDoor command
	if (linecheck.substr(0, 16) == "finishshaftdoor ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 16);

		if (params < 2)
			return ScriptError("Incorrect number of parameters");

		if (params > 3 && warn_deprecated == true)
			ScriptWarning("Syntax deprecated");

		bool option = false;
		if (params == 3)
			option = true;

		//check numeric values
		for (int i = 0; i <= 1; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		Elevator *elev = Simcore->GetElevator(atoi(tempdata[0].c_str()));
		if (!elev)
			return ScriptError("Invalid elevator");

		if (option == false)
			StoreCommand(elev->FinishShaftDoor(atoi(tempdata[1].c_str()), Current));
		else
			StoreCommand(elev->FinishShaftDoor(atoi(tempdata[1].c_str()), Current, Ogre::StringConverter::parseBool(tempdata[2].c_str())));
		return sNextLine;
	}

	//AddModel command
	if (linecheck.substr(0, 8) == "addmodel")
	{
		//get data
		int params = SplitData(LineData.c_str(), 9, true, 1);

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
				if (!IsNumeric(tempdata[i].c_str()))
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
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		//check to see if file exists
		CheckFile(std::string("data/" + tempdata[1]).c_str());

		//create model
		Object *obj;
		if (compat == true)
			obj = floor->AddModel(tempdata[0].c_str(), tempdata[1].c_str(), false, Ogre::Vector3(atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str())), Ogre::Vector3(atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str())), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), Ogre::StringConverter::parseBool(tempdata[10]), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()));
		else
			obj = floor->AddModel(tempdata[0].c_str(), tempdata[1].c_str(), Ogre::StringConverter::parseBool(tempdata[2]), Ogre::Vector3(atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str())), Ogre::Vector3(atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str())), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), Ogre::StringConverter::parseBool(tempdata[11]), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()), atof(tempdata[14].c_str()));

		if (setkey == true && obj)
		{
			Model *mod = (Model*)obj->GetRawObject();
			mod->SetKey(keyvalue);
		}
		StoreCommand(obj);
		return sNextLine;
	}

	//AddStairsModel command
	if (linecheck.substr(0, 14) == "addstairsmodel")
	{
		//get data
		int params = SplitData(LineData.c_str(), 15, true, 2);

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
				if (!IsNumeric(tempdata[i].c_str()))
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
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		//check to see if file exists
		CheckFile(std::string("data/" + tempdata[1]).c_str());

		//create model
		if (Simcore->GetStairs(atoi(tempdata[0].c_str())))
		{
			Object *obj;

			if (compat == true)
				obj = Simcore->GetStairs(atoi(tempdata[0].c_str()))->AddModel(Current, tempdata[1].c_str(), tempdata[2].c_str(), false, Ogre::Vector3(atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str())), Ogre::Vector3(atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str())), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), Ogre::StringConverter::parseBool(tempdata[11]), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()), atof(tempdata[14].c_str()));
			else
				obj = Simcore->GetStairs(atoi(tempdata[0].c_str()))->AddModel(Current, tempdata[1].c_str(), tempdata[2].c_str(), Ogre::StringConverter::parseBool(tempdata[3]), Ogre::Vector3(atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str())), Ogre::Vector3(atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str())), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), Ogre::StringConverter::parseBool(tempdata[12]), atof(tempdata[13].c_str()), atof(tempdata[14].c_str()), atof(tempdata[15].c_str()));

			if (setkey == true && obj)
			{
				Model *mod = (Model*)obj->GetRawObject();
				mod->SetKey(keyvalue);
			}
			StoreCommand(obj);
		}
		else
			return ScriptError("Invalid stairwell");
		return sNextLine;
	}

	//AddShaftModel command
	if (linecheck.substr(0, 13) == "addshaftmodel")
	{
		//get data
		int params = SplitData(LineData.c_str(), 14, true, 2);

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
				if (!IsNumeric(tempdata[i].c_str()))
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
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		//check to see if file exists
		CheckFile(std::string("data/" + tempdata[1]).c_str());

		//create model
		if (Simcore->GetShaft(atoi(tempdata[0].c_str())))
		{
			Object *obj;
			if (compat == true)
				obj = Simcore->GetShaft(atoi(tempdata[0].c_str()))->AddModel(Current, tempdata[1].c_str(), tempdata[2].c_str(), false, Ogre::Vector3(atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str())), Ogre::Vector3(atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str())), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), Ogre::StringConverter::parseBool(tempdata[11]), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()), atof(tempdata[14].c_str()));
			else
				obj = Simcore->GetShaft(atoi(tempdata[0].c_str()))->AddModel(Current, tempdata[1].c_str(), tempdata[2].c_str(), Ogre::StringConverter::parseBool(tempdata[3]), Ogre::Vector3(atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str())), Ogre::Vector3(atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str())), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), Ogre::StringConverter::parseBool(tempdata[12]), atof(tempdata[13].c_str()), atof(tempdata[14].c_str()), atof(tempdata[15].c_str()));

			if (setkey == true && obj)
			{
				Model *mod = (Model*)obj->GetRawObject();
				mod->SetKey(keyvalue);
			}
			StoreCommand(obj);
		}
		else
			return ScriptError("Invalid shaft");
		return sNextLine;
	}

	//AddActionControl command
	if (linecheck.substr(0, 16) == "addactioncontrol")
	{
		//get data
		int params = SplitData(LineData.c_str(), 17);

		if (params < 10)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 7; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

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

		Object *obj = floor->AddControl(tempdata[0].c_str(), tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), action_array, tex_array);

		if (obj)
		{
			Control* control = (Control*)obj->GetRawObject();
			if (lockvalue == 0)
				control->SetLocked(false, keyvalue);
			else
				control->SetLocked(true, keyvalue);
		}
		StoreCommand(obj);
		return sNextLine;
	}

	//AddShaftActionControl command
	if (linecheck.substr(0, 21) == "addshaftactioncontrol")
	{
		//get data
		int params = SplitData(LineData.c_str(), 21);

		if (params < 11)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 8; i++)
		{
			if (i == 1)
				i = 4;
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::vector<std::string> action_array, tex_array;
		int slength, parameters;

		//get number of action & texture parameters
		slength = (int)tempdata.size();
		parameters = slength - 9; //strip off main parameters

		//action & texture parameter number needs to be even
		if (IsEven(parameters) == false)
			return ScriptError("Incorrect number of parameters");

		for (temp3 = 9; temp3 < slength - (parameters / 2); temp3++)
			action_array.push_back(tempdata[temp3]);
		for (temp3 = slength - (parameters / 2); temp3 < slength; temp3++)
			tex_array.push_back(tempdata[temp3]);

		//check to see if file exists
		CheckFile(std::string("data/" + tempdata[1]).c_str());

		if (Simcore->GetShaft(atoi(tempdata[0].c_str())))
		{
			Object *obj = Simcore->GetShaft(atoi(tempdata[0].c_str()))->AddControl(Current, tempdata[1].c_str(), tempdata[2].c_str(), tempdata[3].c_str(), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), action_array, tex_array);

			if (obj)
			{
				Control* control = (Control*)obj->GetRawObject();
				if (lockvalue == 0)
					control->SetLocked(false, keyvalue);
				else
					control->SetLocked(true, keyvalue);
			}
			StoreCommand(obj);
		}
		else
			return ScriptError("Invalid shaft");
		return sNextLine;
	}

	//AddStairsActionControl command
	if (linecheck.substr(0, 21) == "addstairsactioncontrol")
	{
		//get data
		int params = SplitData(LineData.c_str(), 21);

		if (params < 11)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 8; i++)
		{
			if (i == 1)
				i = 4;
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::vector<std::string> action_array, tex_array;
		int slength, parameters;

		//get number of action & texture parameters
		slength = (int)tempdata.size();
		parameters = slength - 9; //strip off main parameters

		//action & texture parameter number needs to be even
		if (IsEven(parameters) == false)
			return ScriptError("Incorrect number of parameters");

		for (temp3 = 9; temp3 < slength - (parameters / 2); temp3++)
			action_array.push_back(tempdata[temp3]);
		for (temp3 = slength - (parameters / 2); temp3 < slength; temp3++)
			tex_array.push_back(tempdata[temp3]);

		//check to see if file exists
		CheckFile(std::string("data/" + tempdata[1]).c_str());

		if (Simcore->GetStairs(atoi(tempdata[0].c_str())))
		{
			Object *obj = Simcore->GetStairs(atoi(tempdata[0].c_str()))->AddControl(Current, tempdata[1].c_str(), tempdata[2].c_str(), tempdata[3].c_str(), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), action_array, tex_array);

			if (obj)
			{
				Control* control = (Control*)obj->GetRawObject();
				if (lockvalue == 0)
					control->SetLocked(false, keyvalue);
				else
					control->SetLocked(true, keyvalue);
			}
			StoreCommand(obj);
		}
		else
			return ScriptError("Invalid stairwell");
		return sNextLine;
	}

	//AddTrigger command
	if (linecheck.substr(0, 10) == "addtrigger")
	{
		//get data
		int params = SplitData(LineData.c_str(), 11);

		if (params < 9)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 7; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::vector<std::string> action_array;

		//get number of action & texture parameters
		for (int i = 8; i < params; i++)
			action_array.push_back(tempdata[i]);

		//check to see if file exists
		CheckFile(std::string("data/" + tempdata[1]).c_str());

		Ogre::Vector3 min = Ogre::Vector3(atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()));
		Ogre::Vector3 max = Ogre::Vector3(atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()));
		StoreCommand(floor->AddTrigger(tempdata[0].c_str(), tempdata[1].c_str(), min, max, action_array));
		return sNextLine;
	}

	//AddShaftTrigger command
	/*if (linecheck.substr(0, 15) == "addshafttrigger")
	{
		//get data
		int params = SplitData(LineData.c_str(), 16);

		if (params < 10)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 8; i++)
		{
			if (i == 1)
				i = 3;
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::vector<std::string> action_array;

		//get number of action & texture parameters
		for (int i = 9; i < params; i++)
			action_array.push_back(tempdata[i]);

		//check to see if file exists
		CheckFile(std::string("data/" + tempdata[1]).c_str());

		Ogre::Vector3 min = Ogre::Vector3(atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()));
		Ogre::Vector3 max = Ogre::Vector3(atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()));
		if (Simcore->GetShaft(atoi(tempdata[0].c_str())))
			StoreCommand(Simcore->GetShaft(atoi(tempdata[0].c_str()))->AddTrigger(Current, tempdata[1].c_str(), tempdata[2].c_str(), min, max, action_array));
		else
			return ScriptError("Invalid shaft");
		return sNextLine;
	}

	//AddStairsTrigger command
	if (linecheck.substr(0, 15) == "addstairstrigger")
	{
		//get data
		int params = SplitData(LineData.c_str(), 16);

		if (params < 10)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 8; i++)
		{
			if (i == 1)
				i = 3;
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::vector<std::string> action_array;

		//get number of action & texture parameters
		for (int i = 9; i < params; i++)
			action_array.push_back(tempdata[i]);

		//check to see if file exists
		CheckFile(std::string("data/" + tempdata[1]).c_str());

		Ogre::Vector3 min = Ogre::Vector3(atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()));
		Ogre::Vector3 max = Ogre::Vector3(atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()));
		if (Simcore->GetStairs(atoi(tempdata[0].c_str())))
			StoreCommand(Simcore->GetStairs(atoi(tempdata[0].c_str()))->AddTrigger(Current, tempdata[1].c_str(), tempdata[2].c_str(), min, max, action_array));
		else
			return ScriptError("Invalid stairwell");
		return sNextLine;
	}*/

	//Cut command
	if (linecheck.substr(0, 4) == "cut ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 4);

		if (params != 8)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 5; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//perform cut on floor
		Simcore->GetFloor(Current)->Cut(Ogre::Vector3(atof(tempdata[0].c_str()), atof(tempdata[1].c_str()), atof(tempdata[2].c_str())), Ogre::Vector3(atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str())), Ogre::StringConverter::parseBool(tempdata[6]), Ogre::StringConverter::parseBool(tempdata[7]), false);
		return sNextLine;
	}

	//CutAll command
	if (linecheck.substr(0, 6) == "cutall")
	{
		//get data
		int params = SplitData(LineData.c_str(), 7);

		if (params != 8)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 5; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//perform cut on all objects related to the current floor
		Simcore->GetFloor(Current)->CutAll(Ogre::Vector3(atof(tempdata[0].c_str()), atof(tempdata[1].c_str()), atof(tempdata[2].c_str())), Ogre::Vector3(atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str())), Ogre::StringConverter::parseBool(tempdata[6]), Ogre::StringConverter::parseBool(tempdata[7]));
		return sNextLine;
	}

	//handle floor range
	if (RangeL != RangeH && linecheck.substr(0, 9) == "<endfloor")
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

	return sContinue;
}

int ScriptProcessor::ProcElevators()
{
	//Process elevators

	//create elevator if not created already
	Simcore->NewElevator(Current);

	//replace variables with actual values
	buffer = ToString(Current);
	ReplaceAll(LineData, "%elevator%", buffer.c_str());

	//IF/While statement stub (continue to global commands for processing)
	if (SetCaseCopy(LineData.substr(0, 2), false) == "if" || SetCaseCopy(LineData.substr(0, 5), false) == "while")
		return sContinue;

	//process math functions
	if (MathFunctions() == sError)
		return sError;

	//process functions
	if (FunctionProc() == true)
		return sNextLine;

	//get text after equal sign
	int temp2check = LineData.find("=", 0);
	temp2 = GetAfterEquals(LineData.c_str());

	Elevator *elev = Simcore->GetElevator(Current);

	//create a lowercase string of the line
	std::string linecheck = SetCaseCopy(LineData, false);

	//parameters
	if (linecheck.substr(0, 4) == "name")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->Name = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 5) == "speed")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (!IsNumeric(temp2.c_str(), elev->ElevatorSpeed))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 12) == "acceleration")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (!IsNumeric(temp2.c_str(), elev->Acceleration))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 12) == "deceleration")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (!IsNumeric(temp2.c_str(), elev->Deceleration))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 9) == "openspeed")
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
		return sNextLine;
	}
	if (linecheck.substr(0, 5) == "doors")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (!IsNumeric(temp2.c_str(), elev->NumDoors))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 9) == "acceljerk")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (!IsNumeric(temp2.c_str(), elev->AccelJerk))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 9) == "deceljerk")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (!IsNumeric(temp2.c_str(), elev->DecelJerk))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 14) == "servicedfloors")
	{
		//copy string listing of serviced floors into array
		int params = SplitAfterEquals(LineData.c_str(), false);
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
				if (!IsNumeric(tempdata[line].c_str(), data))
					return ScriptError("Invalid value");
				if (!elev->AddServicedFloor(data))
					return ScriptError();
			}
		}
		return sNextLine;
	}
	if (linecheck.substr(0, 13) == "displayfloors")
	{
		//copy string listing of serviced floors into array
		int params = SplitAfterEquals(LineData.c_str(), false);
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
				if (!IsNumeric(tempdata[line].c_str(), data))
					return ScriptError("Invalid value");
				elev->AddDisplayFloor(data);
			}
		}
		return sNextLine;
	}
	if (linecheck.substr(0, 13) == "assignedshaft")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (!IsNumeric(temp2.c_str(), elev->AssignedShaft))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 9) == "doortimer")
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
		return sNextLine;
	}
	if (linecheck.substr(0, 10) == "quickclose")
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
		return sNextLine;
	}
	if (linecheck.substr(0, 10) == "nudgetimer")
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
		return sNextLine;
	}
	if (linecheck.substr(0, 9) == "slowspeed")
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
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "manualspeed")
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
		return sNextLine;
	}
	if (linecheck.substr(0, 9) == "opensound")
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
		return sNextLine;
	}
	if (linecheck.substr(0, 10) == "closesound")
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
		return sNextLine;
	}
	if (linecheck.substr(0, 10) == "nudgesound")
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
		return sNextLine;
	}
	if (linecheck.substr(0, 10) == "startsound")
	{
		//backwards compatibility
		if (temp2check < 0)
			return ScriptError("Syntax error");

		if (warn_deprecated == true)
			ScriptWarning("Command deprecated");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->CarUpStartSound = temp2;
		elev->CarDownStartSound = temp2;

		//turn off motor sounds
		elev->MotorUpStartSound = "";
		elev->MotorDownStartSound = "";
		elev->MotorUpRunSound = "";
		elev->MotorDownRunSound = "";
		elev->MotorUpStopSound = "";
		elev->MotorDownStopSound = "";
		elev->MotorIdleSound = "";
		return sNextLine;
	}
	if (linecheck.substr(0, 9) == "movesound")
	{
		//backwards compatibility
		if (temp2check < 0)
			return ScriptError("Syntax error");

		if (warn_deprecated == true)
			ScriptWarning("Command deprecated");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->CarUpMoveSound = temp2;
		elev->CarDownMoveSound = temp2;

		//turn off motor sounds
		elev->MotorUpStartSound = "";
		elev->MotorDownStartSound = "";
		elev->MotorUpRunSound = "";
		elev->MotorDownRunSound = "";
		elev->MotorUpStopSound = "";
		elev->MotorDownStopSound = "";
		elev->MotorIdleSound = "";
		return sNextLine;
	}
	if (linecheck.substr(0, 9) == "stopsound")
	{
		//backwards compatibility
		if (temp2check < 0)
			return ScriptError("Syntax error");

		if (warn_deprecated == true)
			ScriptWarning("Command deprecated");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->CarUpStopSound = temp2;
		elev->CarDownStopSound = temp2;

		//turn off motor sounds
		elev->MotorUpStartSound = "";
		elev->MotorDownStartSound = "";
		elev->MotorUpRunSound = "";
		elev->MotorDownRunSound = "";
		elev->MotorUpStopSound = "";
		elev->MotorDownStopSound = "";
		elev->MotorIdleSound = "";
		return sNextLine;
	}
	if (linecheck.substr(0, 9) == "idlesound")
	{
		//backwards compatibility
		if (temp2check < 0)
			return ScriptError("Syntax error");

		if (warn_deprecated == true)
			ScriptWarning("Command deprecated");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->CarIdleSound = temp2;

		//turn off motor sounds
		elev->MotorUpStartSound = "";
		elev->MotorDownStartSound = "";
		elev->MotorUpRunSound = "";
		elev->MotorDownRunSound = "";
		elev->MotorUpStopSound = "";
		elev->MotorDownStopSound = "";
		elev->MotorIdleSound = "";
		return sNextLine;
	}
	if (linecheck.substr(0, 13) == "carstartsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->CarUpStartSound = temp2;
		elev->CarDownStartSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 15) == "carupstartsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->CarUpStartSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 17) == "cardownstartsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->CarDownStartSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 12) == "carmovesound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->CarUpMoveSound = temp2;
		elev->CarDownMoveSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 14) == "carupmovesound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->CarUpMoveSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 16) == "cardownmovesound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->CarDownMoveSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 12) == "carstopsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->CarUpStopSound = temp2;
		elev->CarDownStopSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 14) == "carupstopsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->CarUpStopSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 16) == "cardownstopsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->CarDownStopSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 12) == "caridlesound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->CarIdleSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 15) == "motorstartsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->MotorUpStartSound = temp2;
		elev->MotorDownStartSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 17) == "motorupstartsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->MotorUpStartSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 19) == "motordownstartsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->MotorDownStartSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 13) == "motorrunsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->MotorUpRunSound = temp2;
		elev->MotorDownRunSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 15) == "motoruprunsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->MotorUpRunSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 17) == "motordownrunsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->MotorDownRunSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 14) == "motorstopsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->MotorUpStopSound = temp2;
		elev->MotorDownStopSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 16) == "motorupstopsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->MotorUpStopSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 18) == "motordownstopsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->MotorDownStopSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 10) == "chimesound")
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
		return sNextLine;
	}
	if (linecheck.substr(0, 12) == "upchimesound")
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
		return sNextLine;
	}
	if (linecheck.substr(0, 14) == "downchimesound")
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
		return sNextLine;
	}
	if (linecheck.substr(0, 10) == "alarmsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->AlarmSound = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 14) == "alarmsoundstop")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->AlarmSoundStop = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 9) == "beepsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->SetBeepSound(temp2.c_str());
		return sNextLine;
	}
	if (linecheck.substr(0, 10) == "floorsound")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->SetFloorSound(temp2.c_str());
		return sNextLine;
	}
	if (linecheck.substr(0, 9) == "upmessage")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->SetMessageSound(true, true, temp2.c_str());
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "downmessage")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->SetMessageSound(true, false, temp2.c_str());
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "openmessage")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->SetMessageSound(false, true, temp2.c_str());
		return sNextLine;
	}
	if (linecheck.substr(0, 12) == "closemessage")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->SetMessageSound(false, false, temp2.c_str());
		return sNextLine;
	}
	if (linecheck.substr(0, 6) == "music ")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(std::string("data/" + temp2).c_str());

		elev->Music = temp2;
		return sNextLine;
	}
	if (linecheck.substr(0, 8) == "musicon ")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		elev->MusicOn = Ogre::StringConverter::parseBool(temp2);
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "musiconmove")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		elev->MusicOnMove = Ogre::StringConverter::parseBool(temp2);
	}
	if (linecheck.substr(0, 13) == "floorskiptext")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->SetFloorSkipText(temp2.c_str());
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "recallfloor")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		int floortemp;
		if (!IsNumeric(temp2.c_str(), floortemp))
			return ScriptError("Invalid value");
		elev->SetRecallFloor(floortemp);
		return sNextLine;
	}
	if (linecheck.substr(0, 20) == "alternaterecallfloor")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		int floortemp;
		if (!IsNumeric(temp2.c_str(), floortemp))
			return ScriptError("Invalid value");
		elev->SetAlternateRecallFloor(floortemp);
		return sNextLine;
	}
	if (linecheck.substr(0, 8) == "acpfloor")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		int floortemp;
		if (!IsNumeric(temp2.c_str(), floortemp))
			return ScriptError("Invalid value");
		elev->SetACPFloor(floortemp);
		return sNextLine;
	}
	if (linecheck.substr(0, 13) == "motorposition")
	{
		int params = SplitAfterEquals(LineData.c_str());
		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 2; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		elev->MotorPosition = Ogre::Vector3(atof(tempdata[0].c_str()), atof(tempdata[1].c_str()), atof(tempdata[2].c_str()));
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "queueresets")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->QueueResets = Ogre::StringConverter::parseBool(temp2);
		return sNextLine;
	}
	if (linecheck.substr(0, 10) == "limitqueue")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->LimitQueue = Ogre::StringConverter::parseBool(temp2);
		return sNextLine;
	}
	if (linecheck.substr(0, 3) == "acp")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->ACP = Ogre::StringConverter::parseBool(temp2);
		return sNextLine;
	}
	if (linecheck.substr(0, 6) == "uppeak")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->UpPeak = Ogre::StringConverter::parseBool(temp2);
		return sNextLine;
	}
	if (linecheck.substr(0, 8) == "downpeak")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->DownPeak = Ogre::StringConverter::parseBool(temp2);
		return sNextLine;
	}
	if (linecheck.substr(0, 18) == "independentservice")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->IndependentService = Ogre::StringConverter::parseBool(temp2);
		return sNextLine;
	}
	if (linecheck.substr(0, 17) == "inspectionservice")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->InspectionService = Ogre::StringConverter::parseBool(temp2);
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "fireservice1")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->FireServicePhase1 = Ogre::StringConverter::parseInt(temp2);
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "fireservice2")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->FireServicePhase2 = Ogre::StringConverter::parseInt(temp2);
		return sNextLine;
	}
	if (linecheck.substr(0, 7) == "parking")
	{
		int params = SplitAfterEquals(LineData.c_str());
		if (params != 2)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 1; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		elev->ParkingFloor = atoi(tempdata[0].c_str());
		elev->ParkingDelay = atof(tempdata[1].c_str());
		return sNextLine;
	}
	if (linecheck.substr(0, 13) == "levelingspeed")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		float leveling;
		if (!IsNumeric(temp2.c_str(), leveling))
			return ScriptError("Invalid value");
		elev->LevelingSpeed = leveling;
		return sNextLine;
	}
	if (linecheck.substr(0, 14) == "levelingoffset")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		float leveling;
		if (!IsNumeric(temp2.c_str(), leveling))
			return ScriptError("Invalid value");
		elev->LevelingOffset = leveling;
		return sNextLine;
	}
	if (linecheck.substr(0, 12) == "levelingopen")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		float leveling;
		if (!IsNumeric(temp2.c_str(), leveling))
			return ScriptError("Invalid value");
		elev->LevelingOpen = leveling;
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "notifyearly")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		float notify;
		if (!IsNumeric(temp2.c_str(), notify))
			return ScriptError("Invalid value");
		elev->NotifyEarly = notify;
		return sNextLine;
	}
	if (linecheck.substr(0, 14) == "departuredelay")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		float delay;
		if (!IsNumeric(temp2.c_str(), delay))
			return ScriptError("Invalid value");
		elev->DepartureDelay = delay;
		return sNextLine;
	}
	if (linecheck.substr(0, 12) == "arrivaldelay")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		float delay;
		if (!IsNumeric(temp2.c_str(), delay))
			return ScriptError("Invalid value");
		elev->ArrivalDelay = delay;
		return sNextLine;
	}
	if (linecheck.substr(0, 13) == "musicposition")
	{
		int params = SplitAfterEquals(LineData.c_str());
		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 2; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		elev->MusicPosition = Ogre::Vector3(atof(tempdata[0].c_str()), atof(tempdata[1].c_str()), atof(tempdata[2].c_str()));
		return sNextLine;
	}
	if (linecheck.substr(0, 15) == "inspectionspeed")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (!IsNumeric(temp2.c_str(), elev->InspectionSpeed))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (linecheck.substr(0, 10) == "autoenable")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		elev->AutoEnable = Ogre::StringConverter::parseBool(temp2);
		return sNextLine;
	}
	if (linecheck.substr(0, 9) == "autodoors")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		elev->AutoDoors = Ogre::StringConverter::parseBool(temp2);
		return sNextLine;
	}
	if (linecheck.substr(0, 11) == "openonstart")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		elev->OpenOnStart = Ogre::StringConverter::parseBool(temp2);
		return sNextLine;
	}
	if (linecheck.substr(0, 10) == "interlocks")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		elev->Interlocks = Ogre::StringConverter::parseBool(temp2);
		return sNextLine;
	}
	if (linecheck.substr(0, 9) == "floorhold")
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		elev->FloorHold = Ogre::StringConverter::parseBool(temp2);
		return sNextLine;
	}

	//CreateElevator command
	if (linecheck.substr(0, 14) == "createelevator")
	{
		//get data
		int params = SplitData(LineData.c_str(), 15);

		if (params != 4)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 3; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		Object *object = elev->CreateElevator(Ogre::StringConverter::parseBool(tempdata[0]), atof(tempdata[1].c_str()), atof(tempdata[2].c_str()), atoi(tempdata[3].c_str()));
		if (!object)
			return ScriptError();
		StoreCommand(object);
		return sNextLine;
	}

	//AddFloor command
	if (linecheck.substr(0, 9) == "addfloor ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 9);

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
				if (!IsNumeric(tempdata[i].c_str()))
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
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		//create floor
		if (compat == true)
			StoreCommand(elev->AddFloor(tempdata[0].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), ReverseAxis, false, atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), true));
		else
			StoreCommand(elev->AddFloor(tempdata[0].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), Ogre::StringConverter::parseBool(tempdata[9]), Ogre::StringConverter::parseBool(tempdata[10]), atof(tempdata[11].c_str()), atof(tempdata[12].c_str())));
		return sNextLine;
	}

	//AddWall command
	if (linecheck.substr(0, 7) == "addwall")
	{
		//get data
		int params = SplitData(LineData.c_str(), 8);

		if (params != 13)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 12; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//create wall
		StoreCommand(elev->AddWall(tempdata[0].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str())));
		return sNextLine;
	}

	//AddDoors command
	if (linecheck.substr(0, 8) == "adddoors")
	{
		//get data
		int params = SplitData(LineData.c_str(), 9);

		if (params != 10 && params != 11)
			return ScriptError("Incorrect number of parameters");

		bool compat = false;
		if (params == 10)
			compat = true; //1.4 compatibility mode

		//check numeric values
		if (compat == true)
		{
			for (int i = 0; i <= 9; i++)
			{
				if (i == 1)
					i = 2;
				if (i == 7)
					i = 8;
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
		}
		else
		{
			for (int i = 0; i <= 10; i++)
			{
				if (i == 1)
					i = 3;
				if (i == 8)
					i = 9;
			if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		if (compat == false)
			StoreCommand(elev->AddDoors(atoi(tempdata[0].c_str()), tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), Ogre::StringConverter::parseBool(tempdata[8]), atof(tempdata[9].c_str()), atof(tempdata[10].c_str())));
		else
			StoreCommand(elev->AddDoors(atoi(tempdata[0].c_str()), tempdata[1].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), Ogre::StringConverter::parseBool(tempdata[7]), atof(tempdata[8].c_str()), atof(tempdata[9].c_str())));
		return sNextLine;
	}

	//SetShaftDoors command
	if (linecheck.substr(0, 13) == "setshaftdoors")
	{
		//backwards compatibility
		if (warn_deprecated == true)
			ScriptWarning("Command deprecated");

		//get data
		int params = SplitData(LineData.c_str(), 14);

		if (params != 4)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 3; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		elev->SetShaftDoors(atoi(tempdata[0].c_str()), atof(tempdata[1].c_str()), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()));
		setshaftdoors = true;
		return sNextLine;
	}

	//AddShaftDoors command
	if (linecheck.substr(0, 14) == "addshaftdoors ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 14);

		if (params < 7 || params > 9)
			return ScriptError("Incorrect number of parameters");

		int compat = 0;
		if (params == 7)
			compat = 1; //1.4 compatibility mode
		if (params == 8)
			compat = 2;

		//check numeric values
		if (compat == 0)
		{
			for (int i = 0; i <= 8; i++)
			{
				if (i == 1)
					i = 3;
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}
		if (compat == 1)
		{
			for (int i = 0; i <= 6; i++)
			{
				if (i == 1)
					i = 2;
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}
		if (compat == 2)
		{
			for (int i = 0; i <= 7; i++)
			{
				if (i == 1)
					i = 3;
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}
		
		if (compat > 0 && warn_deprecated == true)
			ScriptWarning("Syntax deprecated");

		bool result;
		if (compat == 0)
			result = elev->AddShaftDoors(atoi(tempdata[0].c_str()), tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()));
		if (compat == 1)
			result = elev->AddShaftDoors(atoi(tempdata[0].c_str()), tempdata[1].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), 0, atof(tempdata[5].c_str()), atof(tempdata[6].c_str()));
		if (compat == 2)
			result = elev->AddShaftDoors(atoi(tempdata[0].c_str()), tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), 0, atof(tempdata[6].c_str()), atof(tempdata[7].c_str()));

		if (result == false)
			return ScriptError();
		return sNextLine;
	}

	//CreatePanel command
	if (linecheck.substr(0, 11) == "createpanel")
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
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		StoreCommand(elev->CreateButtonPanel(tempdata[0].c_str(), atoi(tempdata[1].c_str()), atoi(tempdata[2].c_str()), tempdata[3].c_str(), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str())));
		return sNextLine;
	}

	//AddFloorButton command
	if (linecheck.substr(0, 14) == "addfloorbutton")
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
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			compat = 1;
		}
		if (params == 9)
		{
			if (IsNumeric(tempdata[2].c_str()) == true)
			{
				//1.5 compatibility mode
				//check numeric values
				for (int i = 0; i <= 8; i++)
				{
					if (i == 1 || i == 4)
						i++;

					if (!IsNumeric(tempdata[i].c_str()))
						return ScriptError("Invalid value: " + tempdata[i]);
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
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
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
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			if (params == 11)
			{
				hoffset = atof(tempdata[9].c_str());
				voffset = atof(tempdata[10].c_str());
			}
		}

		if (!elev->GetPanel(atoi(tempdata[0].c_str())))
			return ScriptError("Invalid panel number");

		if (compat > 0 && warn_deprecated == true)
			ScriptWarning("Syntax deprecated");

		Object *obj;

		if (compat == 0)
		{
			CheckFile(std::string("data/" + tempdata[1]).c_str());
			obj = elev->GetPanel(atoi(tempdata[0].c_str()))->AddButton(tempdata[1].c_str(), tempdata[2].c_str(), tempdata[3].c_str(), atoi(tempdata[4].c_str()), atoi(tempdata[5].c_str()), tempdata[6].c_str(), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), hoffset, voffset);
		}
		if (compat == 1)
			obj = elev->GetPanel(atoi(tempdata[0].c_str()))->AddButton("", tempdata[1].c_str(), tempdata[1].c_str(), atoi(tempdata[2].c_str()), atoi(tempdata[3].c_str()), tempdata[4].c_str(), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), hoffset, voffset);
		if (compat == 2)
			obj = elev->GetPanel(atoi(tempdata[0].c_str()))->AddButton("", tempdata[1].c_str(), tempdata[2].c_str(), atoi(tempdata[3].c_str()), atoi(tempdata[4].c_str()), tempdata[5].c_str(), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), hoffset, voffset);

		if (obj)
		{
			Control* control = (Control*)obj->GetRawObject();
			if (lockvalue == 0)
				control->SetLocked(false, keyvalue);
			else
				control->SetLocked(true, keyvalue);
		}
		StoreCommand(obj);
		return sNextLine;
	}

	//AddControlButton command
	if (linecheck.substr(0, 16) == "addcontrolbutton")
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
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			compat = 1;
		}
		if (params == 9)
		{
			if (IsNumeric(tempdata[2].c_str()) == true)
			{
				//1.5 compatibility mode
				//check numeric values
				for (int i = 1; i <= 8; i++)
				{
					if (i == 1 || i == 4)
						i++;
					if (!IsNumeric(tempdata[i].c_str()))
						return ScriptError("Invalid value: " + tempdata[i]);
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
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
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
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			if (params == 11)
			{
				hoffset = atof(tempdata[9].c_str());
				voffset = atof(tempdata[10].c_str());
			}
		}

		if (!elev->GetPanel(atoi(tempdata[0].c_str())))
			return ScriptError("Invalid panel number");

		if (compat > 0 && warn_deprecated == true)
			ScriptWarning("Syntax deprecated");

		Object *obj;

		if (compat == 0)
		{
			CheckFile(std::string("data/" + tempdata[1]).c_str());
			obj = elev->GetPanel(atoi(tempdata[0].c_str()))->AddButton(tempdata[1].c_str(), tempdata[2].c_str(), tempdata[3].c_str(), atoi(tempdata[4].c_str()), atoi(tempdata[5].c_str()), tempdata[6].c_str(), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), hoffset, voffset);
		}
		if (compat == 1)
			obj = elev->GetPanel(atoi(tempdata[0].c_str()))->AddButton("", tempdata[1].c_str(), tempdata[1].c_str(), atoi(tempdata[2].c_str()), atoi(tempdata[3].c_str()), tempdata[4].c_str(), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), hoffset, voffset);
		if (compat == 2)
			obj = elev->GetPanel(atoi(tempdata[0].c_str()))->AddButton("", tempdata[1].c_str(), tempdata[2].c_str(), atoi(tempdata[3].c_str()), atoi(tempdata[4].c_str()), tempdata[5].c_str(), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), hoffset, voffset);

		if (obj)
		{
			Control* control = (Control*)obj->GetRawObject();
			if (lockvalue == 0)
				control->SetLocked(false, keyvalue);
			else
				control->SetLocked(true, keyvalue);
		}
		StoreCommand(obj);
		return sNextLine;
	}

	//AddButton command
	if (linecheck.substr(0, 10) == "addbutton ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 10);

		if (params != 9 && params != 11)
			return ScriptError("Incorrect number of parameters");

		float hoffset = 0, voffset = 0;

		//check numeric values
		for (int i = 1; i <= 8; i++)
		{
			if (i == 1)
				i = 4;
			if (i == 6)
				i++;
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
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

		Object *obj = elev->GetPanel(atoi(tempdata[0].c_str()))->AddButton(tempdata[1].c_str(), tempdata[2].c_str(), tempdata[3].c_str(), atoi(tempdata[4].c_str()), atoi(tempdata[5].c_str()), tempdata[6].c_str(), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), hoffset, voffset);

		if (obj)
		{
			Control* control = (Control*)obj->GetRawObject();
			if (lockvalue == 0)
				control->SetLocked(false, keyvalue);
			else
				control->SetLocked(true, keyvalue);
		}
		StoreCommand(obj);
		return sNextLine;
	}

	//AddControl command
	if (linecheck.substr(0, 11) == "addcontrol ")
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
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
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

		Object *obj = elev->GetPanel(atoi(tempdata[0].c_str()))->AddControl(tempdata[1].c_str(), atoi(tempdata[2].c_str()), atoi(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), action_array, tex_array);

		if (obj)
		{
			Control* control = (Control*)obj->GetRawObject();
			if (lockvalue == 0)
				control->SetLocked(false, keyvalue);
			else
				control->SetLocked(true, keyvalue);
		}
		StoreCommand(obj);
		return sNextLine;
	}

	//AddFloorIndicator command
	if (linecheck.substr(0, 17) == "addfloorindicator")
	{
		//get data
		int params = SplitData(LineData.c_str(), 18);

		if (params != 6 && params != 7)
			return ScriptError("Incorrect number of parameters");

		bool compat = false;
		if (params == 6)
			compat = true; //1.4 compatibility mode

		//check numeric values
		if (compat == true)
		{
			for (int i = 1; i <= 5; i++)
			{
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
		}
		else
		{
			for (int i = 2; i <= 6; i++)
			{
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		if (compat == false)
			StoreCommand(elev->AddFloorIndicator(tempdata[0].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str())));
		else
			StoreCommand(elev->AddFloorIndicator("Button", tempdata[0].c_str(), atof(tempdata[1].c_str()), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str())));
		return sNextLine;
	}

	//AddDirectionalIndicators command
	if (linecheck.substr(0, 24) == "adddirectionalindicators")
	{
		//get data
		int params = SplitData(LineData.c_str(), 25);

		if (params != 17 && params != 18)
			return ScriptError("Incorrect number of parameters");

		bool compat = false;
		if (params == 17)
		{
			//check numeric values
			for (int i = 8; i <= 16; i++)
			{
				if (i == 11 || i == 14)
					i++;
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			compat = true;
			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
		}
		else
		{
			//check numeric values
			for (int i = 9; i <= 17; i++)
			{
				if (i == 12 || i == 15)
					i++;
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		if (compat == false)
			elev->AddDirectionalIndicators(Ogre::StringConverter::parseBool(tempdata[0]), Ogre::StringConverter::parseBool(tempdata[1]), Ogre::StringConverter::parseBool(tempdata[2]), Ogre::StringConverter::parseBool(tempdata[3]), tempdata[4].c_str(), tempdata[5].c_str(), tempdata[6].c_str(), tempdata[7].c_str(), tempdata[8].c_str(), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), tempdata[12].c_str(), atof(tempdata[13].c_str()), atof(tempdata[14].c_str()), Ogre::StringConverter::parseBool(tempdata[15]), atof(tempdata[16].c_str()), atof(tempdata[17].c_str()));
		else
			elev->AddDirectionalIndicators(Ogre::StringConverter::parseBool(tempdata[0]), false, Ogre::StringConverter::parseBool(tempdata[1]), Ogre::StringConverter::parseBool(tempdata[2]), tempdata[3].c_str(), tempdata[4].c_str(), tempdata[5].c_str(), tempdata[6].c_str(), tempdata[7].c_str(), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), tempdata[11].c_str(), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()), Ogre::StringConverter::parseBool(tempdata[14]), atof(tempdata[15].c_str()), atof(tempdata[16].c_str()));
		return sNextLine;
	}

	//AddFloorSigns command
	if (linecheck.substr(0, 13) == "addfloorsigns")
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
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}
		else if (compat == 1)
		{
			for (int i = 2; i <= 6; i++)
			{
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}
		else if (compat == 2)
		{
			for (int i = 3; i <= 7; i++)
			{
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		if (compat > 0 && warn_deprecated == true)
			ScriptWarning("Syntax deprecated");

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
		return sNextLine;
	}

	//AddSound command
	if (linecheck.substr(0, 8) == "addsound")
	{
		//get data
		int params = SplitData(LineData.c_str(), 9, true, 1);

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
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}
		else
		{
			if (compat == true)
			{
				for (int i = 2; i <= 12; i++)
				{
					if (!IsNumeric(tempdata[i].c_str()))
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

					if (!IsNumeric(tempdata[i].c_str()))
						return ScriptError("Invalid value: " + tempdata[i]);
				}
			}
		}

		//check to see if file exists
		CheckFile(std::string("data/" + tempdata[1]).c_str());

		if (compat == true)
		{
			if (partial == true)
				StoreCommand(elev->AddSound(tempdata[0].c_str(), tempdata[1].c_str(), Ogre::Vector3(atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()))));
			else
				StoreCommand(elev->AddSound(tempdata[0].c_str(), tempdata[1].c_str(), Ogre::Vector3(atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str())), true, atof(tempdata[5].c_str()), atoi(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), 0.0, 360, 360, 1.0, Ogre::Vector3(atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()))));
		}
		else
		{
			if (partial == true)
				StoreCommand(elev->AddSound(tempdata[0].c_str(), tempdata[1].c_str(), Ogre::Vector3(atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str())), Ogre::StringConverter::parseBool(tempdata[5])));
			else
				StoreCommand(elev->AddSound(tempdata[0].c_str(), tempdata[1].c_str(), Ogre::Vector3(atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str())), Ogre::StringConverter::parseBool(tempdata[5]), atof(tempdata[6].c_str()), atoi(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()), Ogre::Vector3(atof(tempdata[14].c_str()), atof(tempdata[15].c_str()), atof(tempdata[16].c_str()))));
		}
		return sNextLine;
	}

	//AddDoorComponent command
	if (linecheck.substr(0, 16) == "adddoorcomponent")
	{
		//get data
		int params = SplitData(LineData.c_str(), 17);

		if (params != 17 && params != 18)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		bool compat = false;
		if (params == 17)
		{
			compat = true;
			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
		}

		for (int i = 0; i <= params - 1; i++)
		{
			if (i == 1)
				i = 4;
			if (i == 5)
				i++;
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		if (compat == true)
			StoreCommand(elev->AddDoorComponent(atoi(tempdata[0].c_str()), tempdata[1].c_str(), tempdata[2].c_str(), tempdata[3].c_str(), atof(tempdata[4].c_str()), tempdata[5].c_str(), atof(tempdata[6].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()), atof(tempdata[14].c_str()), atof(tempdata[15].c_str()), atof(tempdata[16].c_str())));
		else
			StoreCommand(elev->AddDoorComponent(atoi(tempdata[0].c_str()), tempdata[1].c_str(), tempdata[2].c_str(), tempdata[3].c_str(), atof(tempdata[4].c_str()), tempdata[5].c_str(), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()), atof(tempdata[14].c_str()), atof(tempdata[15].c_str()), atof(tempdata[16].c_str()), atof(tempdata[17].c_str())));
		return sNextLine;
	}

	//AddShaftDoorsComponent command
	if (linecheck.substr(0, 22) == "addshaftdoorscomponent")
	{
		//get data
		int params = SplitData(LineData.c_str(), 23);

		if (params != 17 && params != 18)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		bool compat = false;
		if (params == 17)
		{
			compat = true;
			if (warn_deprecated == true)
				ScriptWarning("Syntax deprecated");
		}

		for (int i = 0; i <= params - 1; i++)
		{
			if (i == 1)
				i = 4;
			if (i == 5)
				i++;
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		if (compat == true)
			elev->AddShaftDoorsComponent(atoi(tempdata[0].c_str()), tempdata[1].c_str(), tempdata[2].c_str(), tempdata[3].c_str(), atof(tempdata[4].c_str()), tempdata[5].c_str(), atof(tempdata[6].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()), atof(tempdata[14].c_str()), atof(tempdata[15].c_str()), atof(tempdata[16].c_str()));
		else
			elev->AddShaftDoorsComponent(atoi(tempdata[0].c_str()), tempdata[1].c_str(), tempdata[2].c_str(), tempdata[3].c_str(), atof(tempdata[4].c_str()), tempdata[5].c_str(), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()), atof(tempdata[14].c_str()), atof(tempdata[15].c_str()), atof(tempdata[16].c_str()), atof(tempdata[17].c_str()));
		return sNextLine;
	}

	//FinishDoors command
	if (linecheck.substr(0, 11) == "finishdoors")
	{
		//get data
		int params = SplitData(LineData.c_str(), 12);

		if (params < 1)
			return ScriptError("Incorrect number of parameters");

		if (params > 2 && warn_deprecated == true)
			ScriptWarning("Syntax deprecated");

		bool option = false;
		if (params == 2)
			option = true;

		//check numeric values
		if (!IsNumeric(tempdata[0].c_str()))
			return ScriptError("Invalid value: " + tempdata[0]);

		if (option == false)
			StoreCommand(elev->FinishDoors(atoi(tempdata[0].c_str())));
		else
			StoreCommand(elev->FinishDoors(atoi(tempdata[0].c_str()), Ogre::StringConverter::parseBool(tempdata[1].c_str())));
		return sNextLine;
	}

	//FinishShaftDoors command
	if (linecheck.substr(0, 16) == "finishshaftdoors")
	{
		//get data
		int params = SplitData(LineData.c_str(), 17);

		if (params < 1)
			return ScriptError("Incorrect number of parameters");

		if (params > 2 && warn_deprecated == true)
			ScriptWarning("Syntax deprecated");

		bool option = false;
		if (params == 2)
			option = true;

		//check numeric values
		if (!IsNumeric(tempdata[0].c_str()))
			return ScriptError("Invalid value: " + tempdata[0]);

		bool result;
		if (option == false)
			result = elev->FinishShaftDoors(atoi(tempdata[0].c_str()));
		else
			result = elev->FinishShaftDoors(atoi(tempdata[0].c_str()), Ogre::StringConverter::parseBool(tempdata[1].c_str()));

		if (result == false)
			return ScriptError();
		return sNextLine;
	}

	//AddDirectionalIndicator command
	if (linecheck.substr(0, 24) == "adddirectionalindicator ")
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
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		StoreCommand(elev->AddDirectionalIndicator(Ogre::StringConverter::parseBool(tempdata[0]), Ogre::StringConverter::parseBool(tempdata[1]), Ogre::StringConverter::parseBool(tempdata[2]), tempdata[3].c_str(), tempdata[4].c_str(), tempdata[5].c_str(), tempdata[6].c_str(), tempdata[7].c_str(), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), tempdata[11].c_str(), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()), Ogre::StringConverter::parseBool(tempdata[14]), atof(tempdata[15].c_str()), atof(tempdata[16].c_str())));
		return sNextLine;
	}

	//AddDoor command
	if (linecheck.substr(0, 8) == "adddoor ")
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
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			compat = 1;
		}
		if (params == 13)
		{
			for (int i = 3; i <= 12; i++)
			{
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
			compat = 2;
		}
		if (params == 14)
		{
			for (int i = 4; i <= 13; i++)
			{
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		CheckFile(std::string("data/" + tempdata[0]).c_str());
		CheckFile(std::string("data/" + tempdata[1]).c_str());

		if (compat > 0 && warn_deprecated == true)
			ScriptWarning("Syntax deprecated");

		//create door
		Object *obj;

		if (compat == 1)
			obj = elev->AddDoor(tempdata[0].c_str(), tempdata[1].c_str(), false, tempdata[2].c_str(), atof(tempdata[3].c_str()), atoi(tempdata[4].c_str()), 0, atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()));
		else if (compat == 2)
			obj = elev->AddDoor(tempdata[0].c_str(), tempdata[1].c_str(), false, tempdata[2].c_str(), atof(tempdata[3].c_str()), atoi(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()));
		else
			obj = elev->AddDoor(tempdata[0].c_str(), tempdata[1].c_str(), Ogre::StringConverter::parseBool(tempdata[2]), tempdata[3].c_str(), atof(tempdata[4].c_str()), atoi(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()));

		if (obj)
		{
			Door* door = (Door*)obj->GetRawObject();
			door->SetLocked(lockvalue, keyvalue);
		}
		StoreCommand(obj);
		return sNextLine;
	}

	//AddModel command
	if (linecheck.substr(0, 8) == "addmodel")
	{
		//get data
		int params = SplitData(LineData.c_str(), 9, true, 1);

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
				if (!IsNumeric(tempdata[i].c_str()))
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
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}

		//check to see if file exists
		CheckFile(std::string("data/" + tempdata[1]).c_str());

		//create model
		Object *obj;
		if (compat == true)
			obj = elev->AddModel(tempdata[0].c_str(), tempdata[1].c_str(), false, Ogre::Vector3(atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str())), Ogre::Vector3(atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str())), atof(tempdata[8].c_str()), atof(tempdata[9].c_str()), Ogre::StringConverter::parseBool(tempdata[10]), atof(tempdata[11].c_str()), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()));
		else
			obj = elev->AddModel(tempdata[0].c_str(), tempdata[1].c_str(), Ogre::StringConverter::parseBool(tempdata[2]), Ogre::Vector3(atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str())), Ogre::Vector3(atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str())), atof(tempdata[9].c_str()), atof(tempdata[10].c_str()), Ogre::StringConverter::parseBool(tempdata[11]), atof(tempdata[12].c_str()), atof(tempdata[13].c_str()), atof(tempdata[14].c_str()));

		if (setkey == true && obj)
		{
			Model *mod = (Model*)obj->GetRawObject();
			mod->SetKey(keyvalue);
		}
		StoreCommand(obj);
		return sNextLine;
	}

	//AddActionControl command
	if (linecheck.substr(0, 16) == "addactioncontrol")
	{
		//get data
		int params = SplitData(LineData.c_str(), 17);

		if (params < 10)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 7; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

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

		Object *obj = elev->AddControl(tempdata[0].c_str(), tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), action_array, tex_array);

		if (obj)
		{
			Control* control = (Control*)obj->GetRawObject();
			if (lockvalue == 0)
				control->SetLocked(false, keyvalue);
			else
				control->SetLocked(true, keyvalue);
		}
		StoreCommand(obj);
		return sNextLine;
	}

	//AddTrigger command
	if (linecheck.substr(0, 10) == "addtrigger")
	{
		//get data
		int params = SplitData(LineData.c_str(), 11);

		if (params < 9)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 7; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::vector<std::string> action_array;

		//get number of action & texture parameters
		for (int i = 8; i < params; i++)
			action_array.push_back(tempdata[i]);

		//check to see if file exists
		CheckFile(std::string("data/" + tempdata[1]).c_str());

		Ogre::Vector3 min = Ogre::Vector3(atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()));
		Ogre::Vector3 max = Ogre::Vector3(atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()));
		StoreCommand(elev->AddTrigger(tempdata[0].c_str(), tempdata[1].c_str(), min, max, action_array));
		return sNextLine;
	}

	//handle elevator range
	if (RangeL != RangeH && linecheck.substr(0, 12) == "<endelevator")
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
	return sContinue;
}

int ScriptProcessor::ProcTextures()
{
	//Process Textures

	//create a lowercase string of the line
	std::string linecheck = SetCaseCopy(LineData, false);

	//Load command
	if (linecheck.substr(0, 5) == "load ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 5, false);

		if (params != 4 && params != 5)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 3; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}
		CheckFile(tempdata[0].c_str());
		if (params == 4)
			Simcore->LoadTexture(tempdata[0].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()));
		else
			Simcore->LoadTexture(tempdata[0].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), true, Ogre::StringConverter::parseBool(tempdata[4]));
		return sNextLine;
	}

	//LoadAnimated command
	if (linecheck.substr(0, 12) == "loadanimated")
	{
		//get data
		int params = SplitData(LineData.c_str(), 12, false);

		if (params < 7)
			return ScriptError("Incorrect number of parameters");

		bool force;
		if (IsNumeric(tempdata[params - 1].c_str()) == true && IsNumeric(tempdata[params - 2].c_str()) == true)
			force = false;
		else
			force = true;

		//check numeric values
		if (force == true)
		{
			for (int i = (params - 4); i <= (params - 2); i++)
			{
				if (!IsNumeric(tempdata[i].c_str()))
					return ScriptError("Invalid value: " + tempdata[i]);
			}
		}
		else
		{
			for (int i = (params - 3); i <= (params - 1); i++)
			{
				if (!IsNumeric(tempdata[i].c_str()))
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
		for (int i = 0; i < (int)filenames.size(); i++)
			CheckFile(filenames[i].c_str());

		if (force == false)
			Simcore->LoadAnimatedTexture(filenames, tempdata[params - 4].c_str(), atof(tempdata[params - 3].c_str()), atof(tempdata[params - 2].c_str()), atof(tempdata[params - 1].c_str()));
		else
			Simcore->LoadAnimatedTexture(filenames, tempdata[params - 5].c_str(), atof(tempdata[params - 4].c_str()), atof(tempdata[params - 3].c_str()), atof(tempdata[params - 2].c_str()), true, Ogre::StringConverter::parseBool(tempdata[params - 1]));
		return sNextLine;
	}

	//LoadAlphaBlend command
	if (linecheck.substr(0, 14) == "loadalphablend")
	{
		//get data
		int params = SplitData(LineData.c_str(), 14, false);

		if (params != 7 && params != 8)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 5; i <= 6; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		//check existence of files
		CheckFile(tempdata[0].c_str());
		CheckFile(tempdata[1].c_str());
		CheckFile(tempdata[2].c_str());

		if (params == 7)
			Simcore->LoadAlphaBlendTexture(tempdata[0].c_str(), tempdata[1].c_str(), tempdata[2].c_str(), tempdata[3].c_str(), Ogre::StringConverter::parseBool(tempdata[4]), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()));
		else
			Simcore->LoadAlphaBlendTexture(tempdata[0].c_str(), tempdata[1].c_str(), tempdata[2].c_str(), tempdata[3].c_str(), Ogre::StringConverter::parseBool(tempdata[4]), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()), true, Ogre::StringConverter::parseBool(tempdata[7]));
		return sNextLine;
	}

	//LoadMaterial command
	if (linecheck.substr(0, 12) == "loadmaterial")
	{
		//get data
		int params = SplitData(LineData.c_str(), 12, false);

		if (params != 4 && params != 5)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 3; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}
		if (params == 4)
			Simcore->LoadMaterial(tempdata[0].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()));
		else
			Simcore->LoadMaterial(tempdata[0].c_str(), tempdata[1].c_str(), atof(tempdata[2].c_str()), atof(tempdata[3].c_str()), true, Ogre::StringConverter::parseBool(tempdata[4]));
		return sNextLine;
	}

	//LoadRange command
	if (linecheck.substr(0, 9) == "loadrange")
	{
		//get data
		int params = SplitData(LineData.c_str(), 9, false);

		if (params != 6 && params != 7)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 5; i++)
		{
			if (i == 2)
				i = 4;
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}
		RangeL = atoi(tempdata[0].c_str());
		RangeH = atoi(tempdata[1].c_str());
		for (Current = RangeL; Current <= RangeH; Current++)
		{
			temp2 = tempdata[2];
			buffer = ToString(Current);
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
		return sNextLine;
	}

	//AddText command
	if (linecheck.substr(0, 8) == "addtext ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 8, false);

		if (params != 14 && params != 15)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 13; i++)
		{
			if (i == 4)
				i = 5;
			if (i == 9)
				i = 11;

			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}
		buffer = tempdata[2];
		TrimString(buffer);
		buffer.insert(0, "data/fonts/");
		CheckFile(buffer.c_str());
		if (params == 14)
			Simcore->AddTextToTexture(tempdata[0].c_str(), tempdata[1].c_str(), buffer.c_str(), atof(tempdata[3].c_str()), tempdata[4].c_str(), atoi(tempdata[5].c_str()), atoi(tempdata[6].c_str()), atoi(tempdata[7].c_str()), atoi(tempdata[8].c_str()), tempdata[9].c_str(), tempdata[10].c_str(), atoi(tempdata[11].c_str()), atoi(tempdata[12].c_str()), atoi(tempdata[13].c_str()));
		else
			Simcore->AddTextToTexture(tempdata[0].c_str(), tempdata[1].c_str(), buffer.c_str(), atof(tempdata[3].c_str()), tempdata[4].c_str(), atoi(tempdata[5].c_str()), atoi(tempdata[6].c_str()), atoi(tempdata[7].c_str()), atoi(tempdata[8].c_str()), tempdata[9].c_str(), tempdata[10].c_str(), atoi(tempdata[11].c_str()), atoi(tempdata[12].c_str()), atoi(tempdata[13].c_str()), true, Ogre::StringConverter::parseBool(tempdata[14]));
		return sNextLine;
	}

	//AddTextRange command
	if (linecheck.substr(0, 12) == "addtextrange")
	{
		//get data
		int params = SplitData(LineData.c_str(), 13, false);

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

			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}
		RangeL = atoi(tempdata[0].c_str());
		RangeH = atoi(tempdata[1].c_str());
		temp6 = LineData;
		for (Current = RangeL; Current <= RangeH; Current++)
		{
			buffer = ToString(Current);
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
		linecheck = SetCaseCopy(LineData, false);
		return sNextLine;
	}

	//LoadCropped command
	if (linecheck.substr(0, 11) == "loadcropped")
	{
		//get data
		int params = SplitData(LineData.c_str(), 12, false);

		if (params != 8 && params != 9)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 7; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}
		CheckFile(tempdata[0].c_str());
		if (params == 8)
			Simcore->LoadTextureCropped(tempdata[0].c_str(), tempdata[1].c_str(), atoi(tempdata[2].c_str()), atoi(tempdata[3].c_str()), atoi(tempdata[4].c_str()), atoi(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()));
		else
			Simcore->LoadTextureCropped(tempdata[0].c_str(), tempdata[1].c_str(), atoi(tempdata[2].c_str()), atoi(tempdata[3].c_str()), atoi(tempdata[4].c_str()), atoi(tempdata[5].c_str()), atof(tempdata[6].c_str()), atof(tempdata[7].c_str()), Ogre::StringConverter::parseBool(tempdata[8]));
		return sNextLine;
	}

	//AddOverlay command
	if (linecheck.substr(0, 10) == "addoverlay")
	{
		//get data
		int params = SplitData(LineData.c_str(), 11, false);

		if (params != 9 && params != 10)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 8; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}
		if (params == 9)
			Simcore->AddTextureOverlay(tempdata[0].c_str(), tempdata[1].c_str(), tempdata[2].c_str(), atoi(tempdata[3].c_str()), atoi(tempdata[4].c_str()), atoi(tempdata[5].c_str()), atoi(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()));
		else
			Simcore->AddTextureOverlay(tempdata[0].c_str(), tempdata[1].c_str(), tempdata[2].c_str(), atoi(tempdata[3].c_str()), atoi(tempdata[4].c_str()), atoi(tempdata[5].c_str()), atoi(tempdata[6].c_str()), atof(tempdata[7].c_str()), atof(tempdata[8].c_str()), true, Ogre::StringConverter::parseBool(tempdata[9]));
		return sNextLine;
	}

	//SetLighting command
	if (linecheck.substr(0, 11) == "setlighting")
	{
		//get data
		int params = SplitData(LineData.c_str(), 12, false);

		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 2; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}
		Simcore->SetLighting(atof(tempdata[0].c_str()), atof(tempdata[1].c_str()), atof(tempdata[2].c_str()));
		return sNextLine;
	}

	//Rotate command
	if (linecheck.substr(0, 7) == "rotate ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 7, false);

		if (params != 2)
			return ScriptError("Incorrect number of parameters");

		//check numeric value
		if (!IsNumeric(tempdata[1].c_str()))
			return ScriptError("Invalid value: " + tempdata[1]);

		Simcore->RotateTexture(tempdata[0].c_str(), atof(tempdata[1].c_str()));
		return sNextLine;
	}

	//RotateAnim command
	if (linecheck.substr(0, 10) == "rotateanim")
	{
		//get data
		int params = SplitData(LineData.c_str(), 10, false);

		if (params != 2)
			return ScriptError("Incorrect number of parameters");

		//check numeric value
		if (!IsNumeric(tempdata[1].c_str()))
			return ScriptError("Invalid value: " + tempdata[1]);

		Simcore->RotateAnimTexture(tempdata[0].c_str(), atof(tempdata[1].c_str()));
		return sNextLine;
	}

	//Scroll command
	if (linecheck.substr(0, 7) == "scroll ")
	{
		//get data
		int params = SplitData(LineData.c_str(), 7, false);

		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 2; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		Simcore->ScrollTexture(tempdata[0].c_str(), atof(tempdata[1].c_str()), atof(tempdata[2].c_str()));
		return sNextLine;
	}

	//ScrollAnim command
	if (linecheck.substr(0, 10) == "scrollanim")
	{
		//get data
		int params = SplitData(LineData.c_str(), 10, false);

		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 2; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		Simcore->ScrollAnimTexture(tempdata[0].c_str(), atof(tempdata[1].c_str()), atof(tempdata[2].c_str()));
		return sNextLine;
	}

	//Scale command
	if (linecheck.substr(0, 5) == "scale")
	{
		//get data
		int params = SplitData(LineData.c_str(), 5, false);

		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 2; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		Simcore->ScaleTexture(tempdata[0].c_str(), atof(tempdata[1].c_str()), atof(tempdata[2].c_str()));
		return sNextLine;
	}

	//Transform command
	if (linecheck.substr(0, 9) == "transform")
	{
		//get data
		int params = SplitData(LineData.c_str(), 9, false);

		if (params != 7)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 6; i++)
		{
			if (!IsNumeric(tempdata[i].c_str()))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		Simcore->TransformTexture(tempdata[0].c_str(), tempdata[1].c_str(), tempdata[2].c_str(), atof(tempdata[3].c_str()), atof(tempdata[4].c_str()), atof(tempdata[5].c_str()), atof(tempdata[6].c_str()));
		return sNextLine;
	}

	return sContinue;
}

std::string ScriptProcessor::Calc(const char *expression)
{
	//performs a calculation operation on a string
	//for example, the string "1 + 1" would output to "2"
	//supports multiple and nested operations (within parenthesis)
	//^ character is used as a 'power of' operator

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
			if (tmpcalc.at(i) == '+' || tmpcalc.at(i) == '/' || tmpcalc.at(i) == '*' || tmpcalc.at(i) == '^')
			{
				operators++;
				if (operators == 2)
					end = i;
			}
			if (tmpcalc.at(i) == '-' && tmpcalc.at(i - 1) != '-' && tmpcalc.at(i - 1) != '+' && tmpcalc.at(i - 1) != '/' && tmpcalc.at(i - 1) != '*' && tmpcalc.at(i - 1) != '^')
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
	{
		TrimString(tmpcalc);
		return tmpcalc;
	}

	//otherwise perform math
	temp1 = tmpcalc.find("+", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.substr(0, temp1);
		two = tmpcalc.substr(temp1 + 1);
		if (IsNumeric(one.c_str()) == true && IsNumeric(two.c_str()) == true)
		{
			float first = atof(one.c_str());
			float second = atof(two.c_str());
			float tmpnum = first + second;
			tmpcalc = Simcore->TruncateNumber(tmpnum, 6);
			TrimString(tmpcalc);
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
			float first = atof(one.c_str());
			float second = atof(two.c_str());
			float tmpnum = first - second;
			tmpcalc = Simcore->TruncateNumber(tmpnum, 6);
			TrimString(tmpcalc);
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
			float first = atof(one.c_str());
			float second = atof(two.c_str());
			if (second == 0)
			{
				ScriptError("Division by zero in math operation: '" + tmpcalc + "' (might be nested)");
				CalcError = true;
				return "false";
			}
			float tmpnum = first / second;
			tmpcalc = Simcore->TruncateNumber(tmpnum, 6);
			TrimString(tmpcalc);
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
			float first = atof(one.c_str());
			float second = atof(two.c_str());
			float tmpnum = first * second;
			tmpcalc = Simcore->TruncateNumber(tmpnum, 6);
			TrimString(tmpcalc);
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
	temp1 = tmpcalc.find("^", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.substr(0, temp1);
		two = tmpcalc.substr(temp1 + 1);
		if (IsNumeric(one.c_str()) == true && IsNumeric(two.c_str()) == true)
		{
			float first = atof(one.c_str());
			float second = atof(two.c_str());
			float tmpnum = powf(first, second);
			tmpcalc = Simcore->TruncateNumber(tmpnum, 6);
			TrimString(tmpcalc);
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
		current = ToString(Current);
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
			InFunction += 1;

			FunctionData data;
			data.CallLine = line;
			data.Name = functions[i].name;

			//get function parameters
			int location2 = location + (int)data.Name.length() + 1;
			int end_loc = LineData.find(")", location);
			std::string newdata = LineData.substr(location2, end_loc - location2);
			SplitString(tempdata, newdata.c_str(), ',');

			//calculate inline math
			data.Params.reserve(tempdata.size());
			for (temp3 = 0; temp3 < (int)tempdata.size(); temp3++)
			{
				buffer = Calc(tempdata[temp3].c_str());
				TrimString(buffer);
				data.Params.push_back(buffer);
			}

			//remove function statement
			LineData = LineData.substr(0, location) + LineData.substr(end_loc + 1);

			//switch to function line
			data.LineData = LineData;
			line = functions[i].line;
			FunctionStack.push_back(data);
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
		for (int i = 0; i < (int)nonexistent_files.size(); i++)
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

int ScriptProcessor::SplitData(const char *string, int start, bool calc, int calc_skip)
{
	//split data into separate strings starting at the "start" character
	//delimeter is a comma ","
	//returns the number of parameters found
	//if calc is on, calc_skip can be used to specify an index that does not cooperate properly with calculations, such as filenames

	std::string data = string;
	std::string stringbuffer;
	SplitString(tempdata, data.substr(start).c_str(), ',');
	if (calc == true)
	{
		for (int i = 0; i < (int)tempdata.size(); i++)
		{
			if (i != calc_skip)
			{
				stringbuffer = Calc(tempdata[i].c_str());
				tempdata[i] = stringbuffer;
			}
		}
	}
	return (int)tempdata.size();
}

int ScriptProcessor::SplitAfterEquals(const char *string, bool calc)
{
	//get and split data after equal sign
	//returns -1 if equal sign not found
	//returns 0 if no parameters found

	std::string data = string;
	int loc = data.find("=", 0);
	if (loc < 0)
		return -1;

	std::string temp = data.substr(loc + 1);
	TrimString(temp);

	if (temp == "")
		return 0;

	SplitString(tempdata, temp.c_str(), ',');
	if (calc == true)
	{
		for (int i = 0; i < (int)tempdata.size(); i++)
		{
			std::string buffer = Calc(tempdata[i].c_str());
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

std::vector<std::string> *ScriptProcessor::GetBuildingData()
{
	return &BuildingDataOrig;
}

int ScriptProcessor::MathFunctions()
{
	//functions for advanced math

	int start, first, last;
	float value, result;
	std::string tempdata;

	int check = LineData.find("(", 0);

	if (check < 0)
		return true;

	//calculate cosine
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("cos(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1).c_str());
		if (!IsNumeric(tempdata.c_str(), value))
			return ScriptError("Invalid value: " + tempdata);

		result = cosf(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate sine
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("sin(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1).c_str());
		if (!IsNumeric(tempdata.c_str(), value))
			return ScriptError("Invalid value: " + tempdata);

		result = sinf(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate tangent
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("tan(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1).c_str());
		if (!IsNumeric(tempdata.c_str(), value))
			return ScriptError("Invalid value: " + tempdata);

		result = tanf(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate arc cosine
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("acos(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1).c_str());
		if (!IsNumeric(tempdata.c_str(), value))
			return ScriptError("Invalid value: " + tempdata);

		if (value < -1 || value > 1)
			return ScriptError("Invalid value: " + tempdata);

		result = acosf(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate arc sine
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("asin(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1).c_str());
		if (!IsNumeric(tempdata.c_str(), value))
			return ScriptError("Invalid value: " + tempdata);

		if (value < -1 || value > 1)
			return ScriptError("Invalid value: " + tempdata);

		result = asinf(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate arc tangent
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("atan(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1).c_str());
		if (!IsNumeric(tempdata.c_str(), value))
			return ScriptError("Invalid value: " + tempdata);

		result = atanf(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate arc tangent with 2 parameters
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("atan2(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		int mid = LineData.find(",", first);
		last = LineData.find(")", start);
		if (last < 0 || mid < 0)
			return ScriptError("Syntax error");

		std::string tempdata1 = Calc(LineData.substr(first + 1, mid - first - 1).c_str());
		std::string tempdata2 = Calc(LineData.substr(mid + 1, last - mid - 1).c_str());

		float value1, value2;
		if (!IsNumeric(tempdata1.c_str(), value1))
			return ScriptError("Invalid value: " + tempdata1);
		if (!IsNumeric(tempdata2.c_str(), value2))
			return ScriptError("Invalid value: " + tempdata2);

		result = atan2f(value1, value2);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate square root
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("sqrt(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1).c_str());
		if (!IsNumeric(tempdata.c_str(), value))
			return ScriptError("Invalid value: " + tempdata);

		result = sqrtf(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate absolute value
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("abs(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1).c_str());
		if (!IsNumeric(tempdata.c_str(), value))
			return ScriptError("Invalid value: " + tempdata);

		result = fabsf(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate exponential function
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("exp(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1).c_str());
		if (!IsNumeric(tempdata.c_str(), value))
			return ScriptError("Invalid value: " + tempdata);

		result = expf(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate natural logarithm
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("log(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1).c_str());
		if (!IsNumeric(tempdata.c_str(), value))
			return ScriptError("Invalid value: " + tempdata);

		if (value <= 0)
			return ScriptError("Invalid value: " + tempdata);

		result = logf(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate common logarithm
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("log10(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1).c_str());
		if (!IsNumeric(tempdata.c_str(), value))
			return ScriptError("Invalid value: " + tempdata);

		if (value <= 0)
			return ScriptError("Invalid value: " + tempdata);

		result = log10f(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate binary logarithm
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("log2(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1).c_str());
		if (!IsNumeric(tempdata.c_str(), value))
			return ScriptError("Invalid value: " + tempdata);

		if (value <= 0)
			return ScriptError("Invalid value: " + tempdata);

		result = Log2(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate remainder
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("mod(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		int mid = LineData.find(",", first);
		last = LineData.find(")", start);
		if (last < 0 || mid < 0)
			return ScriptError("Syntax error");

		std::string tempdata1 = Calc(LineData.substr(first + 1, mid - first - 1).c_str());
		std::string tempdata2 = Calc(LineData.substr(mid + 1, last - mid - 1).c_str());

		float value1, value2;
		if (!IsNumeric(tempdata1.c_str(), value1))
			return ScriptError("Invalid value: " + tempdata1);
		if (!IsNumeric(tempdata2.c_str(), value2))
			return ScriptError("Invalid value: " + tempdata2);

		if (value2 == 0)
			return ScriptError("Invalid value: " + tempdata2);

		result = fmodf(value1, value2);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate hypotenuse
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("hypot(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		int mid = LineData.find(",", first);
		last = LineData.find(")", start);
		if (last < 0 || mid < 0)
			return ScriptError("Syntax error");

		std::string tempdata1 = Calc(LineData.substr(first + 1, mid - first - 1).c_str());
		std::string tempdata2 = Calc(LineData.substr(mid + 1, last - mid - 1).c_str());

		float value1, value2;
		if (!IsNumeric(tempdata1.c_str(), value1))
			return ScriptError("Invalid value: " + tempdata1);
		if (!IsNumeric(tempdata2.c_str(), value2))
			return ScriptError("Invalid value: " + tempdata2);

		if (value2 == 0)
			return ScriptError("Invalid value: " + tempdata2);

		result = sqrtf(powf(value1, 2) + powf(value2, 2));
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate ceiling
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("ceil(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1).c_str());
		if (!IsNumeric(tempdata.c_str(), value))
			return ScriptError("Invalid value: " + tempdata);

		if (value <= 0)
			return ScriptError("Invalid value: " + tempdata);

		result = ceilf(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate floor
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("flr(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1).c_str());
		if (!IsNumeric(tempdata.c_str(), value))
			return ScriptError("Invalid value: " + tempdata);

		if (value <= 0)
			return ScriptError("Invalid value: " + tempdata);

		result = floorf(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate random number
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("rnd(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1).c_str());
		if (!IsNumeric(tempdata.c_str(), value))
			return ScriptError("Invalid value: " + tempdata);

		if (value <= 0)
			return ScriptError("Invalid value: " + tempdata);

		RandomGen rnd(time(0));
		result = rnd.Get(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//round a number
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("round(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1).c_str());
		if (!IsNumeric(tempdata.c_str(), value))
			return ScriptError("Invalid value: " + tempdata);

		if (value <= 0)
			return ScriptError("Invalid value: " + tempdata);

		result = Round(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	return true;
}

bool ScriptProcessor::IsFunctionDefined(const char *name)
{
	//return true if a function of the specified name has already been defined

	for (int i = 0; i < (int)functions.size(); i++)
	{
		if (functions[i].name == name)
			return true;
	}
	return false;
}
