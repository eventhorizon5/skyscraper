/* $Id$ */

/*
	Skyscraper 1.10 Alpha - File I/O and Script Processing Routines
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

#include <wx/app.h>
#include <wx/msgdlg.h>
#include "globals.h"
#include "sbs.h"
#include <OgreFileSystem.h>
#include <stdlib.h>
#include <cmath>
#include "skyscraper.h"
#include "enginecontext.h"
#include "model.h"
#include "texture.h"
#include "floor.h"
#include "elevator.h"
#include "elevatorcar.h"
#include "shaft.h"
#include "stairs.h"
#include "random.h"
#include "textwindow.h"
#include "scriptprocessor.h"
#include "script_section.h"

using namespace SBS;

namespace Skyscraper {

ScriptProcessor::ScriptProcessor(EngineContext *instance)
{
	if (!instance)
		return;

	engine = instance;
	Simcore = instance->GetSystem();

	//create section objects
	globals_section = new GlobalsSection(this);
	buildings_section = new BuildingsSection(this);
	textures_section = new TexturesSection(this);
	commands_section = new CommandsSection(this);
	floor_section = new FloorSection(this);
	elevator_section = new ElevatorSection(this);
	elevatorcar_section = new ElevatorCarSection(this);

	Reset();
}

ScriptProcessor::~ScriptProcessor()
{
	if (globals_section)
		delete globals_section;
	if (buildings_section)
		delete buildings_section;
	if (textures_section)
		delete textures_section;
	if (commands_section)
		delete commands_section;
	if (floor_section)
		delete floor_section;
	if (elevator_section)
		delete elevator_section;
	if (elevatorcar_section)
		delete elevatorcar_section;
}

void ScriptProcessor::Reset()
{
	line = 0; //line number
	LineData = "";  //line contents
	wall = 0;
	startpos = 0;
	getfloordata = false;
	Section::setshaftdoors = false;
	BuildingData.clear();
	BuildingDataOrig.clear();
	BuildingData.reserve(1024);
	BuildingDataOrig.reserve(1024);
	InFunction = 0;
	FunctionStack.clear();
	ReplaceLine = false;
	ReplaceLineData = "";
	nonexistent_files.clear();
	show_percent = true;
	IsFinished = false;
	progress_marker = 0;
	functions.clear();
	includes.clear();
	variables.clear();

	//reset section objects
	globals_section->Reset();
	buildings_section->Reset();
	textures_section->Reset();
	commands_section->Reset();
	floor_section->Reset();
	elevator_section->Reset();
	elevatorcar_section->Reset();
}

bool ScriptProcessor::Run()
{
	//building loader/script interpreter

	int temp1;
	std::string temp2;
	int temp3;
	int temp4;
	std::string buffer;
	int temp5;
	std::string temp6;

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


		//////////////////////////////
		//Function parameter variables
		//////////////////////////////
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
					if (IsNumeric(temp2) == true)
					{
						temp4 = ToInt(temp2);

						//replace all occurrences of the variable with it's value
						std::string replacement;
						if (temp4 > 0 && temp4 <= (int)FunctionStack[InFunction - 1].Params.size())
							replacement = FunctionStack[InFunction - 1].Params[temp4 - 1];
						else
							replacement = "";

						ReplaceAll(LineData, "%param" + temp2 + "%", replacement);
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
				engine->Report(percent_s + "%");
				engine->UpdateProgress(percent);
			}
		}

		linecheck = SetCaseCopy(LineData, false);


		//////////////////
		//User variables
		//////////////////
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
				for (size_t i = 0; i < variables.size(); i++)
				{
					if (variables[i].name == temp2)
					{
						found = true;

						//replace all occurrences of the variable with it's value
						ReplaceAll(LineData, "%" + temp2 + "%", variables[i].value);
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


		//////////////////////
		//Section information
		//////////////////////
		if (linecheck == "<globals>")
		{
			if (Section::SectionNum > 0)
			{
				ScriptError("Already within a section");
				goto Error;
			}
			Section::SectionNum = 1;
			Section::Context = "Globals";
			engine->Report("Processing globals...");
			goto Nextline;
		}
		if (linecheck == "<endglobals>")
		{
			if (Section::SectionNum != 1)
			{
				ScriptError("Not in global section");
				goto Error;
			}
			Section::SectionNum = 0;
			Section::Context = "None";
			engine->Report("Finished globals");
			goto Nextline;
		}
		if (linecheck.substr(0, 5) == "<end>")
		{
			Section::SectionNum = 0;
			Section::Context = "None";
			engine->Report("Exiting building script");
			IsFinished = true;
			show_percent = false;
			line = (int)BuildingData.size(); //jump to end of script
			return true; //exit data file parser
		}
		if (linecheck.substr(0, 7) == "<break>")
		{
			//breakpoint function for debugging scripts
breakpoint:
			engine->Report("Script breakpoint reached");
			goto Nextline;
		}
		if (linecheck.substr(0, 8) == "<include")
		{
			//include another file at the current script location

			int endloc = LineData.find(">");

			if (endloc == -1)
			{
				ScriptError("Syntax error");
				goto Error;
			}

			std::string includefile = LineData.substr(9, endloc - 9);
			TrimString(includefile);

			//delete current line
			BuildingData.erase(BuildingData.begin() + line);

			//insert file at current line
			std::string filename = Simcore->VerifyFile(includefile);
			bool result = LoadDataFile(filename, true, line);
			if (result == false)
				goto Error;
			engine->Report("Inserted file " + includefile);

			line--;
			goto Nextline;
		}
		if (linecheck.substr(0, 9) == "<function")
		{
			//define a function

			if (Section::SectionNum != 0)
			{
				ScriptError("Cannot define a function within a section");
				goto Error;
			}

			int endloc = LineData.find(">");

			if (endloc == -1)
			{
				ScriptError("Syntax error");
				goto Error;
			}

			std::string function = LineData.substr(10, endloc - 10);
			TrimString(function);

			//skip the function definition and show a warning if it's already been defined
			bool defined = IsFunctionDefined(function);

			if (defined == true)
				engine->Report("Function '" + function + "' already defined");
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
				engine->Report("Defined function " + function);
			goto Nextline;
		}
		if (linecheck.substr(0, 10) == "<textures>")
		{
			if (Section::SectionNum > 0)
			{
				ScriptError("Already within a section");
				goto Error;
			}
			Section::SectionNum = 5;
			Section::Context = "Textures";
			engine->Report("Processing textures...");
			goto Nextline;
		}
		if (linecheck.substr(0, 13) == "<endtextures>")
		{
			if (Section::SectionNum != 5)
			{
				ScriptError("Not in texture section");
				goto Error;
			}
			Simcore->GetTextureManager()->FreeTextureImages();
			Section::SectionNum = 0;
			Section::Context = "None";
			engine->Report("Finished textures");
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
		if (linecheck.substr(0, 7) == "<floors")
		{
			if (Section::SectionNum > 0)
			{
				ScriptError("Already within a section");
				goto Error;
			}
			Section::SectionNum = 2;
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
			if (!IsNumeric(str1, Section::RangeL) || !IsNumeric(str2, Section::RangeH))
			{
				ScriptError("Invalid range");
				goto Error;
			}
			Section::Context = "Floor range " + ToString(Section::RangeL) + " to " + ToString(Section::RangeH);
			Section::Current = Section::RangeL;
			Section::RangeStart = line;
			engine->Report("Processing floors " + ToString(Section::RangeL) + " to " + ToString(Section::RangeH) + "...");
			goto Nextline;
		}
		if (linecheck.substr(0, 7) == "<floor ")
		{
			if (Section::SectionNum > 0)
			{
				ScriptError("Already within a section");
				goto Error;
			}
			Section::SectionNum = 2;
			Section::RangeL = 0;
			Section::RangeH = 0;
			std::string str = LineData.substr(7, LineData.length() - 8);
			TrimString(str);
			if (!IsNumeric(str, Section::Current))
			{
				ScriptError("Invalid floor");
				goto Error;
			}
			Section::Context = "Floor " + ToString(Section::Current);
			engine->Report("Processing floor " + ToString(Section::Current) + "...");
			goto Nextline;
		}
		if (linecheck == "<endfloor>" && Section::RangeL == Section::RangeH)
		{
			if (Section::SectionNum != 2)
			{
				ScriptError("Not in floor section");
				goto Error;
			}
			Section::SectionNum = 0;
			Section::Context = "None";
			engine->Report("Finished floor");
			goto Nextline;
		}
		if (linecheck.substr(0, 10) == "<elevators")
		{
			if (Section::SectionNum > 0)
			{
				ScriptError("Already within a section");
				goto Error;
			}
			Section::SectionNum = 4;
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
			if (!IsNumeric(str1, Section::RangeL) || !IsNumeric(str2, Section::RangeH))
			{
				ScriptError("Invalid range");
				goto Error;
			}
			Section::Context = "Elevator range " + ToString(Section::RangeL) + " to " + ToString(Section::RangeH);
			Section::Current = Section::RangeL;
			Section::RangeStart = line;
			engine->Report("Processing elevators " + ToString(Section::RangeL) + " to " + ToString(Section::RangeH) + "...");
			goto Nextline;
		}
		if (linecheck.substr(0, 10) == "<elevator ")
		{
			if (Section::SectionNum > 0)
			{
				ScriptError("Already within a section");
				goto Error;
			}
			Section::SectionNum = 4;
			Section::RangeL = 0;
			Section::RangeH = 0;
			std::string str = LineData.substr(10, LineData.length() - 11);
			TrimString(str);
			if (!IsNumeric(str, Section::Current))
			{
				ScriptError("Invalid elevator");
				goto Error;
			}
			if (Section::Current < 1 || Section::Current > Simcore->GetElevatorCount() + 1)
			{
				ScriptError("Invalid elevator");
				goto Error;
			}
			Section::Context = "Elevator " + ToString(Section::Current);
			engine->Report("Processing elevator " + ToString(Section::Current) + "...");
			goto Nextline;
		}
		if (linecheck == "<endelevator>" && Section::RangeL == Section::RangeH)
		{
			if (Section::SectionNum != 4)
			{
				ScriptError("Not in elevator section");
				goto Error;
			}
			Section::SectionNum = 0;
			Section::Context = "None";
			engine->Report("Finished elevator");
			goto Nextline;
		}
		if (linecheck.substr(0, 11) == "<buildings>")
		{
			//skip this section if reloading
			if (engine->IsReloading() == true)
				goto Nextline;

			if (Section::SectionNum > 0)
			{
				ScriptError("Already within a section");
				goto Error;
			}
			Section::SectionNum = 3;
			Section::Context = "Buildings";
			engine->Report("Loading other buildings...");
			goto Nextline;
		}
		if (linecheck.substr(0, 14) == "<endbuildings>")
		{
			//skip this section if reloading
			if (engine->IsReloading() == true)
				goto Nextline;

			if (Section::SectionNum != 3)
			{
				ScriptError("Not in buildings section");
				goto Error;
			}
			Section::SectionNum = 0;
			Section::Context = "None";
			engine->Report("Finished loading other buildings");
			goto Nextline;
		}
		if (linecheck.substr(0, 5) == "<cars" && Section::SectionNum == 4)
		{
			Section::SectionNum = 6;
			temp3 = linecheck.find("to", 5);
			if (temp3 < 0)
			{
				ScriptError("Syntax error");
				goto Error;
			}

			//store previous elevator section values
			Section::CurrentOld = Section::Current;
			Section::RangeLOld = Section::RangeL;
			Section::RangeHOld = Section::RangeH;
			Section::RangeStartOld = Section::RangeStart;
			Section::ContextOld = Section::Context;

			std::string str1 = LineData.substr(6, temp3 - 7);
			std::string str2 = LineData.substr(temp3 + 2, LineData.length() - (temp3 + 2) - 1);
			TrimString(str1);
			TrimString(str2);
			if (!IsNumeric(str1, Section::RangeL) || !IsNumeric(str2, Section::RangeH))
			{
				ScriptError("Invalid range");
				goto Error;
			}

			//verify elevator
			if (!Simcore->GetElevator(Section::CurrentOld))
			{
				ScriptError("Invalid elevator");
				goto Error;
			}

			Section::Context = "Elevator " + ToString(Section::CurrentOld) + " Car range " + ToString(Section::RangeL) + " to " + ToString(Section::RangeH);
			Section::Current = Section::RangeL;
			Section::RangeStart = line;
			engine->Report("Processing elevator " + ToString(Section::CurrentOld) + " cars " + ToString(Section::RangeL) + " to " + ToString(Section::RangeH) + "...");
			goto Nextline;
		}
		if (linecheck.substr(0, 5) == "<car " && Section::SectionNum == 4)
		{
			Section::SectionNum = 6;

			//store previous elevator section values
			Section::CurrentOld = Section::Current;
			Section::RangeLOld = Section::RangeL;
			Section::RangeHOld = Section::RangeH;
			Section::RangeStartOld = Section::RangeStart;
			Section::ContextOld = Section::Context;

			Section::RangeL = 0;
			Section::RangeH = 0;
			std::string str = LineData.substr(5, LineData.length() - 6);
			TrimString(str);
			if (!IsNumeric(str, Section::Current))
			{
				ScriptError("Invalid car number");
				goto Error;
			}

			//verify elevator
			if (!Simcore->GetElevator(Section::CurrentOld))
			{
				ScriptError("Invalid elevator");
				goto Error;
			}

			Section::Context = "Elevator " + ToString(Section::CurrentOld) + " Car " + ToString(Section::Current);
			engine->Report("Processing elevator " + ToString(Section::CurrentOld) + " car " + ToString(Section::Current) + "...");
			goto Nextline;
		}
		if (linecheck == "<endcar>" && Section::RangeL == Section::RangeH)
		{
			if (Section::SectionNum != 6)
			{
				ScriptError("Not in car section");
				goto Error;
			}

			//return to elevator section
			Section::SectionNum = 4;
			Section::Context = Section::ContextOld;
			Section::Current = Section::CurrentOld;
			Section::RangeL = Section::RangeLOld;
			Section::RangeH = Section::RangeHOld;
			Section::RangeStart = Section::RangeStartOld;

			engine->Report("Finished car");
			goto Nextline;
		}


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
			if (Section::SectionNum == 2 && getfloordata == false)
			{
				//process floor-specific variables if in a floor section
				getfloordata = true;
				goto recalc;
			}
			else
				getfloordata = false;
			std::string tempdata = Calc(LineData.substr(temp1 + 1, temp3 - temp1 - 1));
			LineData = LineData.substr(0, temp1 + 1) + tempdata + LineData.substr(temp3);

			if (!IsNumeric(tempdata, temp4))
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
		ReplaceAll(LineData, "%minx%", ToString(Section::MinExtent.x));
		ReplaceAll(LineData, "%minz%", ToString(Section::MinExtent.z));
		ReplaceAll(LineData, "%maxx%", ToString(Section::MaxExtent.x));
		ReplaceAll(LineData, "%maxz%", ToString(Section::MaxExtent.z));

		//reset return code
		returncode = sContinue;

		//Global parameters
		if (Section::SectionNum == 1)
			returncode = globals_section->Run(LineData);

		//Process floors
		else if (Section::SectionNum == 2)
		{
			//create floor if not created already
			Simcore->NewFloor(Section::Current);
recalc:
			returncode = floor_section->Run(LineData);
		}

		//Process external buildings
		else if (Section::SectionNum == 3)
			returncode = buildings_section->Run(LineData);

		//process elevators
		else if (Section::SectionNum == 4)
			returncode = elevator_section->Run(LineData);

		//Process textures
		else if (Section::SectionNum == 5)
			returncode = textures_section->Run(LineData);

		//process elevator cars
		else if (Section::SectionNum == 6)
			returncode = elevatorcar_section->Run(LineData);

		//Global commands
		if (returncode == sContinue)
			returncode = commands_section->Run(LineData);

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
		Simcore->GetTextureManager()->TextureOverride = false;
		Simcore->GetTextureManager()->FlipTexture = false;

Nextline:
		if (Section::InWhile == true && InFunction == 0)
			Section::InWhile = false;
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

bool ScriptProcessor::LoadDataFile(const std::string &filename, bool insert, int insert_line)
{
	//loads a building data file into the runtime buffer
	int location = insert_line;
	std::string Filename = Simcore->VerifyFile(filename);

	//if insert location is greater than array size, return with error
	if (insert == true)
	{
		if (location > (int)BuildingData.size() - 1 || location < 0)
		{
			ScriptError("Cannot insert file beyond end of script");
			return false;
		}
	}

	//make sure file exists
	if (Simcore->FileExists(Filename) == false)
	{
		if (insert == false)
			engine->ReportFatalError("Error loading building file:\nFile '" + Filename + "' does not exist");
		else
			ScriptError("File not found");
		return false;
	}

	if (Simcore->Verbose)
		Simcore->Report("Filename: '" + Filename + "'");

	//load file
#if OGRE_VERSION >= 0x00010900
	Ogre::FileSystemArchive filesystem(".", "FileSystem", true);
#else
	Ogre::FileSystemArchive filesystem(".", "FileSystem");
#endif

	Ogre::DataStreamPtr filedata;
	try
	{
		filedata = filesystem.open(Filename, true);
	}
	catch (Ogre::Exception &e)
	{
		std::string msg = "Error loading building file\nDetails: " + e.getDescription();
		if (insert == false)
			engine->ReportFatalError(msg);
		else
			ScriptError(msg);
		return false;
	}

	//exit if an error occurred while loading
	if(filedata.isNull())
	{
		std::string msg = "Error loading building file";
		if (insert == false)
			engine->ReportFatalError(msg);
		else
			ScriptError(msg);
		return false;
	}

	Ogre::DataStreamPtr file(new Ogre::MemoryDataStream(Filename, filedata, true, true));

	std::vector<std::string> insert_data;
	insert_data.reserve(512);

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
			//otherwise add data to new array, and insert into buildings array later
			insert_data.push_back(line);
		}
	}

	if (insert == true)
	{
		//insert new building data into array
		BuildingData.insert(BuildingData.begin() + location, insert_data.begin(), insert_data.end());
		location += (int)insert_data.size();

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
		for (size_t i = 0; i < functions.size(); i++)
		{
			if (functions[i].line > line)
				functions[i].line += lines;
		}

		if (InFunction > 0)
		{
			for (size_t i = 0; i < FunctionStack.size(); i++)
			{
				if (FunctionStack[i].CallLine > line)
					FunctionStack[i].CallLine += lines;
			}
		}

		//store include info in array
		IncludeInfo info;
		info.filename = Filename;
		info.start_line = line;
		info.end_line = end;
		info.parent = parent;
		includes.push_back(info);
	}

	return true;
}

bool ScriptProcessor::LoadFromText(const std::string &text)
{
	//loads building commands from a string
	std::vector<std::string> textarray;
	SplitString(textarray, text, '\n');

	//feed each line of text into the script array

	for (size_t i = 0; i < textarray.size(); i++)
	{
		//append data to building array
		BuildingData.push_back(textarray[i]);
		BuildingDataOrig.push_back(textarray[i]);
	}
	return true;
}

int ScriptProcessor::ScriptError(std::string message, bool warning)
{
	//report a script error, with line and context information.
	//reports a warning message (without window popup) if 'warning' is true

	std::string error;
	int LineNumber, FunctionLine;
	bool IsInclude, IsIncludeFunction;
	std::string FunctionName, IncludeFile, IncludeFunctionFile;

	GetLineInformation(true, LineNumber, FunctionName, FunctionLine, IsInclude, IncludeFile, IsIncludeFunction, IncludeFunctionFile);

	if (warning == false)
		error = "Script error ";
	else
		error = "Script warning ";

	if (IsInclude == true)
		error += "in included file " + IncludeFile + " ";

	error += "on line " + ToString(LineNumber) + ": " + message + "\n\nFilename: " + engine->GetFilename() + "\nContext: " + Section::Context;

	if (Section::RangeL != Section::RangeH)
		error += "\nIteration Number: " + ToString(Section::Current);

	if (InFunction == 0)
		error += "\nLine Text: " + LineData;
	else
	{
		error += "\nFunction: " + FunctionName;

		if (IsIncludeFunction == true)
			error += "\nFunction included in file: " + IncludeFunctionFile;

		error += "\nFunction call line: " + ToString(FunctionLine) + "\nLine Text: " + LineData;
	}

	if (engine->GetFrontend()->GetEngineCount() > 1)
		error += "\nEngine context: " + ToString(engine->GetNumber());

	engine->ReportError(error);

	//show error dialog
	if (warning == false)
	{
		wxMessageDialog dialog (0, error, "Skyscraper", wxOK | wxICON_ERROR);
		dialog.ShowModal();
	}
	return sError;
}

void ScriptProcessor::GetLineInformation(bool CheckFunctionCall, int &LineNumber, std::string &FunctionName, int &FunctionLine, bool &IsInclude, std::string &IncludeFile, bool &IsIncludeFunction, std::string &IncludeFunctionFile)
{
	//calculate line information for current script line

	//if IsInclude is true, LineNumber is the line of the included file IncludeFile
	//if in a function, the function call line is returned as FunctionLine
	//if function call line is in an included file, IsIncludeFunction is true, with IncludeFunctionFile as the file
	//to skip the function call line check, set CheckFunctionCall to false

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
	FunctionLine = 0;
	IsInclude = false;
	IsIncludeFunction = false;
	FunctionName = "";
	IncludeFile = "";
	IncludeFunctionFile = "";

	if (InFunction > 0)
	{
		FunctionName = FunctionStack[InFunction - 1].Name;
		function_line = FunctionStack[InFunction - 1].CallLine;
		newfunction_line = function_line;
	}

	//first see if the current line is from an included file
	for (int i = 0; i < (int)includes.size(); i++)
	{
		if (linenum < includes[i].start_line)
			break;

		//line is part of an included file
		if (linenum >= includes[i].start_line && linenum <= includes[i].end_line)
		{
			IsInclude = true;
			IncludeFile = includes[i].filename;
			newlinenum = linenum - includes[i].start_line;
			parent = i;
			linenum_start = includes[i].start_line;
		}

		//function call line is part of an included file
		if (CheckFunctionCall == true)
		{
			if (function_line >= includes[i].start_line && function_line <= includes[i].end_line)
			{
				IsIncludeFunction = true;
				IncludeFunctionFile = includes[i].filename;
				newfunction_line = function_line - includes[i].start_line;
				parent_f = i;
				function_line_start = includes[i].start_line;
			}
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

		if (CheckFunctionCall == true)
		{
			if (includes[i].parent == parent_f)
			{
				if (function_line + function_line_start > includes[i].end_line)
					included_lines_f += includes[i].end_line - includes[i].start_line;
			}
		}
	}

	//have line numbers start from 1 instead of 0
	linenum += 1;
	function_line += 1;

	//return values
	LineNumber = linenum - included_lines;

	if (InFunction > 0)
		FunctionLine = function_line - included_lines_f;
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
		for (size_t i = 0; i < nonexistent_files.size(); i++)
			Simcore->Report("Missing file: " + nonexistent_files[i]);

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
		for (size_t i = 0; i < nonexistent_files.size(); i++)
		{
			message.Append(nonexistent_files[i]);
			message.Append(wxT("\n"));
		}
		twindow->tMain->WriteText(message);
		twindow->tMain->SetInsertionPoint(0);
		return true;
	}
	else
		return false;
}

std::string ScriptProcessor::Calc(const std::string &expression)
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
				char &tmpchar = tmpcalc.at(i);
				if (tmpchar == '(')
					match++;
				if (tmpchar == ')')
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
				newdata = Calc(tmpcalc.substr(start + 1, end - start - 1));

				if (CalcError == true)
					return tmpcalc;

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
				return tmpcalc;
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
			char &tmpchar = tmpcalc.at(i);
			char &tmpcharprev = tmpcalc.at(i - 1);
			if (tmpchar == '+' || tmpchar == '/' || tmpchar == '*' || tmpchar == '^')
			{
				//ensure that numbers are around operator
				if (i < (int)tmpcalc.length() - 1)
				{
					char &tmpcharnext = tmpcalc.at(i + 1);
					if (IsNumeric(tmpcharprev) == true && (IsNumeric(tmpcharnext) == true || tmpcharnext == '-' || tmpcharnext == '.'))
					{
						//valid operator found
						operators++;
						if (operators == 2)
							end = i;
					}
				}
			}
			if (tmpchar == '-' && tmpcharprev != '-' && tmpcharprev != '+' && tmpcharprev != '/' && tmpcharprev != '*' && tmpcharprev != '^')
			{
				//ensure that numbers are around operator
				if (i < (int)tmpcalc.length() - 1)
				{
					char &tmpcharnext = tmpcalc.at(i + 1);
					if (IsNumeric(tmpcharprev) == true && (IsNumeric(tmpcharnext) == true || tmpcharnext == '-' || tmpcharnext == '.'))
					{
						//valid operator found
						operators++;
						if (operators == 2)
							end = i;
					}
				}
			}
		}
		if (end >= (int)tmpcalc.length() - 1 && operators > 0)
		{
			ScriptError("Syntax error in math operation: '" + tmpcalc + "' (might be nested)");
			CalcError = true;
			return tmpcalc;
		}
		if (operators > 1)
		{
			std::string newdata;
			newdata = Calc(tmpcalc.substr(0, end));

			if (CalcError == true)
				return tmpcalc;

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
		if (IsNumeric(one) == true && IsNumeric(two) == true)
		{
			float first = ToFloat(one);
			float second = ToFloat(two);
			float tmpnum = first + second;
			tmpcalc = TruncateNumber(tmpnum, 6);
			TrimString(tmpcalc);
			return tmpcalc;
		}
		else
		{
			CalcError = true;
			return tmpcalc;
		}
	}
	temp1 = tmpcalc.find("/", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.substr(0, temp1);
		two = tmpcalc.substr(temp1 + 1);
		if (IsNumeric(one) == true && IsNumeric(two) == true)
		{
			float first = ToFloat(one);
			float second = ToFloat(two);
			if (second == 0)
			{
				ScriptError("Division by zero in math operation: '" + tmpcalc + "' (might be nested)");
				CalcError = true;
				return tmpcalc;
			}
			float tmpnum = first / second;
			tmpcalc = TruncateNumber(tmpnum, 6);
			TrimString(tmpcalc);
			return tmpcalc;
		}
		else
		{
			CalcError = true;
			return tmpcalc;
		}
	}
	temp1 = tmpcalc.find("*", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.substr(0, temp1);
		two = tmpcalc.substr(temp1 + 1);
		if (IsNumeric(one) == true && IsNumeric(two) == true)
		{
			float first = ToFloat(one);
			float second = ToFloat(two);
			float tmpnum = first * second;
			tmpcalc = TruncateNumber(tmpnum, 6);
			TrimString(tmpcalc);
			return tmpcalc;
		}
		else
		{
			CalcError = true;
			return tmpcalc;
		}
	}
	temp1 = tmpcalc.find("^", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.substr(0, temp1);
		two = tmpcalc.substr(temp1 + 1);
		if (IsNumeric(one) == true && IsNumeric(two) == true)
		{
			float first = ToFloat(one);
			float second = ToFloat(two);
			float tmpnum = powf(first, second);
			tmpcalc = TruncateNumber(tmpnum, 6);
			TrimString(tmpcalc);
			return tmpcalc;
		}
		else
		{
			CalcError = true;
			return tmpcalc;
		}
	}
	temp1 = tmpcalc.find("-", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.substr(0, temp1);
		two = tmpcalc.substr(temp1 + 1);
		if (IsNumeric(one) == true && IsNumeric(two) == true)
		{
			float first = ToFloat(one);
			float second = ToFloat(two);
			float tmpnum = first - second;
			tmpcalc = TruncateNumber(tmpnum, 6);
			TrimString(tmpcalc);
			return tmpcalc;
		}
		else
		{
			CalcError = true;
			return tmpcalc;
		}
	}
	return tmpcalc;
}

void ScriptProcessor::StoreCommand(Object *object)
{
	//store command and line info in object

	if (!object)
		return;

	int LineNumber, FunctionLine;
	bool IsInclude, IsIncludeFunction;
	std::string FunctionName, IncludeFile, IncludeFunctionFile;

	GetLineInformation(false, LineNumber, FunctionName, FunctionLine, IsInclude, IncludeFile, IsIncludeFunction, IncludeFunctionFile);
	object->linenum = LineNumber;
	object->includefile = IncludeFile;

	TrimString(BuildingData[line]);
	object->command = BuildingData[line];
	object->command_processed = LineData;
	object->context = Section::Context;
	std::string current;
	current = ToString(Section::Current);
	if (Section::SectionNum == 2)
		object->context = "Floor " + current;
	if (Section::SectionNum == 4)
		object->context = "Elevator " + current;
	if (Section::SectionNum == 6)
		object->context = "Elevator " + ToString(Section::CurrentOld) + " Car " + current;
}

bool ScriptProcessor::FunctionProc()
{
	//process functions
	for (size_t i = 0; i < functions.size(); i++)
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
			std::vector<std::string> tempdata;
			SplitString(tempdata, newdata, ',');

			//calculate inline math
			data.Params.reserve(tempdata.size());
			std::string buffer;
			for (int j = 0; j < (int)tempdata.size(); j++)
			{
				buffer = Calc(tempdata[j]);
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

void ScriptProcessor::CheckFile(const std::string &filename)
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

	if (Simcore->FileExists(file) == false)
	{
		bool exists = false;
		for (size_t i = 0; i < nonexistent_files.size(); i++)
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

std::vector<std::string> *ScriptProcessor::GetBuildingData()
{
	return &BuildingDataOrig;
}

bool ScriptProcessor::IsFunctionDefined(const std::string &name)
{
	//return true if a function of the specified name has already been defined

	for (size_t i = 0; i < functions.size(); i++)
	{
		if (functions[i].name == name)
			return true;
	}
	return false;
}

MeshObject* ScriptProcessor::GetMeshObject(std::string name)
{
	//get a system mesh object, or search for a custom one by name

	SetCase(name, false);

	//get a system mesh object
	if (name == "floor")
	{
		if (Section::SectionNum == 2)
			return Simcore->GetFloor(Section::Current)->Level;
		return 0;
	}
	else if (name == "interfloor")
	{
		if (Section::SectionNum == 2)
			return Simcore->GetFloor(Section::Current)->Interfloor;
		return 0;
	}
	else if (name == "columnframe")
	{
		if (Section::SectionNum == 2)
			return Simcore->GetFloor(Section::Current)->ColumnFrame;
		return 0;
	}
	else if (name == "elevatorcar")
	{
		if (Section::SectionNum == 6)
			return Simcore->GetElevator(Section::CurrentOld)->GetCar(Section::Current)->Mesh;
		return 0;
	}
	else if (name == "elevator")
	{
		if (Section::SectionNum == 4)
			return Simcore->GetElevator(Section::Current)->GetCar(1)->Mesh;
		return 0;
	}
	else if (name == "external")
		return Simcore->External;
	else if (name == "landscape")
		return Simcore->Landscape;
	else if (name == "buildings")
		return Simcore->Buildings;
	else if (name.substr(0, 5) == "shaft")
	{
		if (Section::SectionNum == 2)
		{
			//get a shaft mesh object, or a model in a shaft

			std::string num, modelname;
			int marker = (int)name.find(":");
			if (marker > 0)
				modelname = name.substr(marker + 1);

			if (marker > 0)
				num = name.substr(5, (int)name.length() - marker - 5 - 1);
			else
				num = name.substr(5);

			TrimString(num);
			int number;
			if (!IsNumeric(num, number))
				return 0;

			Shaft *shaft = Simcore->GetShaft(number);
			if (!shaft)
				return 0;

			if (marker > 0)
			{
				Model *model = shaft->GetModel(Section::Current, modelname);
				if (model)
				{
					if (model->IsCustom() == true)
						return model->GetMeshObject();
				}
				return 0;
			}
			else
				return shaft->GetMeshObject(Section::Current);
		}
		return 0;
	}
	else if (name.substr(0, 9) == "stairwell")
	{
		if (Section::SectionNum == 2)
		{
			//get a stairwell mesh object, or a model in a stairwell

			std::string num, modelname;
			int marker = (int)name.find(":");
			if (marker > 0)
				modelname = name.substr(marker + 1);

			if (marker > 0)
				num = name.substr(9, (int)name.length() - marker - 5 - 1);
			else
				num = name.substr(9);

			TrimString(num);
			int number;
			if (!IsNumeric(num, number))
				return 0;

			Stairs *stairs = Simcore->GetStairs(number);
			if (!stairs)
				return 0;

			if (marker > 0)
			{
				Model *model = stairs->GetModel(Section::Current, modelname);
				if (model)
				{
					if (model->IsCustom() == true)
						return model->GetMeshObject();
				}
				return 0;
			}
			else
				return stairs->GetMeshObject(Section::Current);
		}
		return 0;
	}

	//get a custom model mesh

	Model* model = 0;

	if (Section::SectionNum == 2)
		model = Simcore->GetFloor(Section::Current)->GetModel(name);
	else if (Section::SectionNum == 4)
		model = Simcore->GetElevator(Section::Current)->GetCar(1)->GetModel(name);
	else if (Section::SectionNum == 6)
		model = Simcore->GetElevator(Section::CurrentOld)->GetCar(Section::Current)->GetModel(name);
	else
		model = Simcore->GetModel(name);

	if (model)
	{
		if (model->IsCustom() == true)
			return model->GetMeshObject();
	}
	return 0;
}

std::string ScriptProcessor::DumpState()
{
	//dump the basic script interpreter state to a string

	int LineNumber, FunctionLine;
	bool IsInclude, IsIncludeFunction;
	std::string FunctionName, IncludeFile, IncludeFunctionFile;

	GetLineInformation(true, LineNumber, FunctionName, FunctionLine, IsInclude, IncludeFile, IsIncludeFunction, IncludeFunctionFile);

	std::string output = "Line number: "  + ToString(LineNumber) + "\n";
	if (IsInclude == true)
		output.append("In included file: " + IncludeFile + "\n");
	output.append("Context: " + Section::Context + "\n");

	if (Section::RangeL != Section::RangeH)
		output.append("Iteration Number: " + ToString(Section::Current) + "\n");

	if (InFunction != 0)
	{
		//report function information
		output.append("Function: " + FunctionName + "\n");
		if (IsIncludeFunction == true)
			output.append("Function include file: " + IncludeFunctionFile + "\n");
		output.append("Function call line: " + ToString(FunctionLine) + "\n");
	}
	output.append("Line text: " + LineData + "\n");

	return output;
}

EngineContext* ScriptProcessor::GetEngine()
{
	return engine;
}

ScriptProcessor::ElevatorCarSection* ScriptProcessor::GetElevatorCarSection()
{
	return elevatorcar_section;
}

}
