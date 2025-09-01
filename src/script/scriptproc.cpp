/*
	Skyscraper 2.1 - File I/O and Script Processing Routines
	Copyright (C)2003-2025 Ryan Thoryk
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
#include "utility.h"
#ifdef USING_WX
#include "wx/wx.h"
#endif
#include <OgreFileSystem.h>
#include <OgreArchive.h>
#include <OgreArchiveManager.h>
#include <OgreException.h>
#include <stdlib.h>
#include <cmath>
#include "vm.h"
#include "sky.h"
#include "enginecontext.h"
#include "texman.h"
#include "floor.h"
#include "camera.h"
#include "random.h"
#include "scriptproc.h"
#include "section.h"

using namespace SBS;

namespace Skyscraper {

ScriptProcessor::ScriptProcessor(EngineContext *instance)
{
	if (!instance)
		return;

	engine = instance;
	Simcore = instance->GetSystem();

	//create configuration handler
	config = new ConfigHandler();

	//create section objects
	globals_section = new GlobalsSection(this);
	buildings_section = new BuildingsSection(this);
	textures_section = new TexturesSection(this);
	commands_section = new CommandsSection(this);
	floor_section = new FloorSection(this);
	elevator_section = new ElevatorSection(this);
	elevatorcar_section = new ElevatorCarSection(this);
	vehicle_section = new VehicleSection(this);
	controller_section = new ControllerSection(this);
	callstation_section = new CallStationSection(this);

	NoModels = false;
	Reset();
}

ScriptProcessor::~ScriptProcessor()
{
	if (config)
		delete config;
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
	if (vehicle_section)
		delete vehicle_section;
	if (controller_section)
		delete controller_section;
	if (callstation_section)
		delete callstation_section;
}

void ScriptProcessor::Reset(bool full)
{
	line = 0; //line number
	LineData = "";  //line contents
	wall = 0;
	startpos = 0;
	getfloordata = false;
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
	in_runloop = false;
	processed_runloop = false;

	if (full == true)
	{
		BuildingData.clear();
		BuildingDataOrig.clear();
		BuildingData.reserve(1024);
		BuildingDataOrig.reserve(1024);
	}

	//reset configuration
	config->Reset();

	//set CheckScript state
	config->CheckScript = engine->GetVM()->CheckScript;

	//reset section objects
	globals_section->Reset();
	buildings_section->Reset();
	textures_section->Reset();
	commands_section->Reset();
	floor_section->Reset();
	elevator_section->Reset();
	elevatorcar_section->Reset();
	vehicle_section->Reset();
	controller_section->Reset();
	callstation_section->Reset();
}

bool ScriptProcessor::Run()
{
	//building loader/script interpreter

	bool status = false;
	int returncode = sContinue;
	IsFinished = false;

	if (engine->IsRunning() == true && processed_runloop == false)
		ProcessRunloop();

	//wait until end of script before processing runloop again
	if (processed_runloop == true)
		processed_runloop = false;

	if (line < (int)BuildingData.size() && line >= 0)
	{
		if (InRunloop() == false)
			engine->ResetPrepare(); //reset prepare flag
		LineData = BuildingData[line];
		TrimString(LineData);

		if (ReplaceLine == true)
		{
			LineData = ReplaceLineData;
			ReplaceLine = false;
		}

		//skip blank lines
		if (LineData == "")
			goto Nextline;

		//process comment markers
		{
		int marker = LineData.find("#", 0);
		if (marker > -1)
			LineData.erase(marker);
		}

		//skip blank lines
		if (LineData == "")
			goto Nextline;

		//expand runloop variables
		if (in_runloop == true)
		{
			ReplaceAll(LineData, "%uptime%", ToString((int)Simcore->GetRunTime()));
			struct tm datetime = engine->GetVM()->GetDateTime();
			int hour, minute, second;

			hour = datetime.tm_hour;
			minute = datetime.tm_min;
			second = datetime.tm_sec;
			ReplaceAll(LineData, "%real_hour%", ToString(hour));
			ReplaceAll(LineData, "%real_minute%", ToString(minute));
			ReplaceAll(LineData, "%real_second%", ToString(second));

			engine->GetVM()->GetSkySystem()->GetTime(hour, minute, second);
			ReplaceAll(LineData, "%hour%", ToString(hour));
			ReplaceAll(LineData, "%minute%", ToString(minute));
			ReplaceAll(LineData, "%second%", ToString(second));
		}

		//process function parameters
		status = ProcessFunctionParameters();

		//handle cancel if requested
		if (status == false)
			goto Error;

		//process user variables
		ProcessUserVariables();

		//process sections
		returncode = ProcessSections();
		if (returncode != sContinue)
			goto handlecodes;

		//process floor object conversions
checkfloors:
		returncode = ProcessFloorObjects();
		if (returncode != sContinue)
			goto handlecodes;

		//process extent variables
		ProcessExtents();

		//process For loops
		returncode = ProcessForLoops();
		if (returncode != sContinue)
			goto handlecodes;

		//reset return code
		returncode = sContinue;

		//Global parameters
		if (config->SectionNum == SECTION_GLOBAL)
			returncode = globals_section->Run(LineData);

		//Process floors
		else if (config->SectionNum == SECTION_FLOOR)
		{
			//create floor if not created already
			Simcore->NewFloor(config->Current);
recalc:
			returncode = floor_section->Run(LineData);
		}

		//Process external buildings
		else if (config->SectionNum == SECTION_BUILDINGS)
			returncode = buildings_section->Run(LineData);

		//process elevators
		else if (config->SectionNum == SECTION_ELEVATOR)
			returncode = elevator_section->Run(LineData);

		//Process textures
		else if (config->SectionNum == SECTION_TEXTURE)
			returncode = textures_section->Run(LineData);

		//process elevator cars
		else if (config->SectionNum == SECTION_ELEVATORCAR)
			returncode = elevatorcar_section->Run(LineData);

		//process vehicles
		else if (config->SectionNum == SECTION_VEHICLE)
			returncode = vehicle_section->Run(LineData);

		//process controllers
		else if (config->SectionNum == SECTION_CONTROLLER)
			returncode = controller_section->Run(LineData);

		//process call stations
		else if (config->SectionNum == SECTION_CALLSTATION)
			returncode = callstation_section->Run(LineData);

		//Global commands
		if (returncode == sContinue)
			returncode = commands_section->Run(LineData);

		//handle return values
handlecodes:
		if (returncode == sError)
			goto Error;
		else if (returncode == sCheckFloors)
			goto checkfloors;
		else if (returncode == sBreak)
		{
			Breakpoint();
			goto Nextline;
		}
		else if (returncode == sRecalc)
			goto recalc;
		else if (returncode == sSkipReset)
			goto Nextline;
		else if (returncode == sExit)
			return true;
		else if (returncode == sLoopFor)
		{
			if (ForLoops.size() > 0)
			{
				ForInfo &info = ForLoops.back();
				line = info.line;
			}
			goto Nextline;
		}

		//reset temporary states
		Simcore->GetTextureManager()->TextureOverride = false;
		Simcore->GetTextureManager()->FlipTexture = false;

Nextline:
		if (config->InWhile == true && InFunction == 0)
			config->InWhile = false;
		else
			line++;

		if (line == (int)BuildingData.size())
		{
			//free text texture memory
			Simcore->GetTextureManager()->FreeTextureBoxes();

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
	std::string Filename = Simcore->GetUtility()->VerifyFile(filename);

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
	if (Simcore->GetUtility()->FileExists(Filename) == false)
	{
		if (insert == false)
			engine->ReportFatalError("Error loading building file:\nFile '" + Filename + "' does not exist");
		else
			ScriptError("File not found");
		return false;
	}

	if (Simcore->Verbose)
		Simcore->Report("Filename: '" + Filename + "'");

	Ogre::Archive *filesystem = 0;
	Ogre::ArchiveManager::ArchiveMapIterator it = Ogre::ArchiveManager::getSingleton().getArchiveIterator();
	Ogre::DataStreamPtr filedata;
	while(it.hasMoreElements())
	{
		//search each filesystem entry for the file
		const std::string& key = it.peekNextKey();
		filesystem = it.getNext();

		if (!filesystem)
			return "";

		//check for a mount point
		std::string shortname;
		std::string group = Simcore->GetUtility()->GetMountPath(Filename, shortname);

		if (group == "General")
		{
			//for the General group, check the native filesystem
			if (filesystem->exists(Filename) == true)
			{
				try
				{
					filedata = filesystem->open(Filename, true);
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
			}
		}
	}

	//exit if an error occurred while loading
	if(!filedata)
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
			BuildingData.emplace_back(line);
			BuildingDataOrig.emplace_back(line);
		}
		else
		{
			//otherwise add data to new array, and insert into buildings array later
			insert_data.emplace_back(line);
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
		includes.emplace_back(info);
	}

	return true;
}

bool ScriptProcessor::LoadFromText(const std::string &text)
{
	//loads building commands from a string

	if (text.size() == 0)
		return false;

	std::vector<std::string> textarray;
	SplitString(textarray, text, '\n');

	//feed each line of text into the script array

	for (size_t i = 0; i < textarray.size(); i++)
	{
		//append data to building array
		BuildingData.emplace_back(textarray[i]);
		BuildingDataOrig.emplace_back(textarray[i]);
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

	error += "on line " + ToString(LineNumber) + ": " + message + "\n\nFilename: " + engine->GetFilename() + "\nContext: " + config->Context;

	if (config->RangeL != config->RangeH)
		error += "\nIteration Number: " + ToString(config->Current);

	if (InFunction == 0)
		error += "\nLine Text: " + LineData;
	else
	{
		error += "\nFunction: " + FunctionName;

		if (IsIncludeFunction == true)
			error += "\nFunction included in file: " + IncludeFunctionFile;

		error += "\nFunction call line: " + ToString(FunctionLine) + "\nLine Text: " + LineData;
	}

	if (engine->GetVM()->GetEngineCount() > 1)
		error += "\nEngine context: " + ToString(engine->GetNumber());

	engine->ReportError(error);

	//show error dialog
	if (warning == false)
	{
#ifdef USING_WX
		wxMessageDialog dialog (0, error, "Skyscraper", wxOK | wxICON_ERROR);
		dialog.ShowModal();
#endif
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
			Real first = ToFloat(one);
			Real second = ToFloat(two);
			Real tmpnum = first + second;
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
			Real first = ToFloat(one);
			Real second = ToFloat(two);
			if (second == 0)
			{
				ScriptError("Division by zero in math operation: '" + tmpcalc + "' (might be nested)");
				CalcError = true;
				return tmpcalc;
			}
			Real tmpnum = first / second;
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
			Real first = ToFloat(one);
			Real second = ToFloat(two);
			Real tmpnum = first * second;
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
			Real first = ToFloat(one);
			Real second = ToFloat(two);
			Real tmpnum = powf(first, second);
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
			Real first = ToFloat(one);
			Real second = ToFloat(two);
			Real tmpnum = first - second;
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
	object->context = config->Context;
	std::string current;
	current = ToString(config->Current);
	if (config->SectionNum == SECTION_FLOOR)
		object->context = "Floor " + current;
	if (config->SectionNum == SECTION_ELEVATOR)
		object->context = "Elevator " + current;
	if (config->SectionNum == SECTION_ELEVATORCAR)
		object->context = "Elevator " + ToString(config->CurrentOld) + " Car " + current;
	if (config->SectionNum == SECTION_VEHICLE)
		object->context = "Vehicle " + current;
	if (config->SectionNum == SECTION_CONTROLLER)
		object->context = "Controller " + current;
	if (config->SectionNum == SECTION_CALLSTATION)
		object->context = "Call Station " + current;
}

bool ScriptProcessor::FunctionProc()
{
	//process functions
	for (size_t i = 0; i < functions.size(); i++)
	{
		if (functions[i].name == "runloop")
			continue;

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
			int end_loc = LineData.find_last_of(")");
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
				data.Params.emplace_back(buffer);
			}

			//remove function statement
			LineData = LineData.substr(0, location) + LineData.substr(end_loc + 1);

			//switch to function line
			data.LineData = LineData;
			line = functions[i].line;
			FunctionStack.emplace_back(data);
			return true;
		}
	}
	return false;
}

void ScriptProcessor::ProcessRunloop()
{
	//process runloop

	//exit if already in a runloop
	if (in_runloop == true)
		return;

	//add function to function stack, to run
	for (int i = 0; i < functions.size(); i++)
	{
		if (functions[i].name == "runloop")
		{
			//store info
			InFunction += 1;

			FunctionData data;
			data.CallLine = BuildingData.size();
			data.Name = "runloop";

			in_runloop = true;
			line = functions[i].line + 1;
			FunctionStack.emplace_back(data);
		}
	}
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

	if (Simcore->GetUtility()->FileExists(file) == false)
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
			nonexistent_files.emplace_back(file);
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
	output.append("Context: " + config->Context + "\n");

	if (config->RangeL != config->RangeH)
		output.append("Iteration Number: " + ToString(config->Current) + "\n");

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

bool ScriptProcessor::ProcessFunctionParameters()
{
	//////////////////////////////
	//Function parameter variables
	//////////////////////////////
	if (InFunction > 0)
	{
		startpos = 0;
		int loc1 = 0;
		int loc2 = 0;
		do
		{
			//Function parameter conversion
			loc1 = LineData.find("%param", startpos);
			loc2 = 0;
			if (loc1 >= startpos)
			{
				loc2 = LineData.find("%", loc1 + 6);
				if (loc2 >= (int)LineData.length() || loc2 < 0)
				{
					loc1 = 0;
					loc2 = 0;
					break;
				}
			}
			else
			{
				//none (or no more) variables found
				loc1 = 0;
				loc2 = 0;
				break;
			}

			if (loc1 + loc2 > 0)
			{
				std::string str = LineData.substr(loc1 + 6, loc2 - (loc1 + 6));
				TrimString(str);
				if (IsNumeric(str) == true)
				{
					int index = ToInt(str);

					//replace all occurrences of the variable with it's value
					std::string replacement;
					if (index > 0 && index <= (int)FunctionStack[InFunction - 1].Params.size())
						replacement = FunctionStack[InFunction - 1].Params[index - 1];
					else
						replacement = "";

					ReplaceAll(LineData, "%param" + str + "%", replacement);
					startpos = loc1;
				}
				else
					startpos = loc2 + 1;
			}
			else
				startpos++;
		} while (true);
	}
	else if (show_percent == true)
	{
		int percent = ((Real)line / (Real)BuildingData.size()) * 100.0;
		std::string percent_s = ToString(percent);
		int marker = percent / 10;
		if (marker > progress_marker)
		{
			progress_marker = marker;
			engine->Report(percent_s + "%");
			return engine->UpdateProgress(percent);
		}
	}
	return true;
}

void ScriptProcessor::ProcessUserVariables()
{
	//////////////////
	//User variables
	//////////////////
	startpos = 0;
	do
	{
		//User variable conversion
		int loc1 = LineData.find("%", startpos);
		int loc2 = 0;
		std::string str;
		if (loc1 >= startpos)
		{
			loc2 = LineData.find("%", loc1 + 1);
			if (loc2 >= (int)LineData.length() || loc2 < 0)
			{
				loc1 = 0;
				loc2 = 0;
				break;
			}
		}
		else
		{
			//none (or no more) variables found
			loc1 = 0;
			loc2 = 0;
			break;
		}

		if (loc1 + loc2 > 0)
		{
			str = LineData.substr(loc1 + 1, loc2 - loc1 - 1);
			TrimString(str);

			bool found = false;
			for (size_t i = 0; i < variables.size(); i++)
			{
				if (variables[i].name == str)
				{
					found = true;

					//replace all occurrences of the variable with it's value
					ReplaceAll(LineData, "%" + str + "%", variables[i].value);
					startpos = loc1;
					break;
				}
			}

			if (found == false)
				startpos = loc2 + 1;
		}
		else
			startpos++;
	} while (true);
}

int ScriptProcessor::ProcessSections()
{
	//////////////////////
	//Section information
	//////////////////////
	if (StartsWithNoCase(LineData, "<globals>"))
	{
		if (config->SectionNum != SECTION_NONE)
		{
			ScriptError("Already within a section");
			return sError;
		}
		config->SectionNum = SECTION_GLOBAL;
		config->Context = "Globals";
		engine->Report("Processing globals...");
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "<end>"))
	{
		config->SectionNum = SECTION_NONE;
		config->Context = "None";
		engine->Report("Exiting building script");
		IsFinished = true;
		show_percent = false;
		line = (int)BuildingData.size(); //jump to end of script
		return sExit; //exit data file parser
	}
	if (StartsWithNoCase(LineData, "<break>"))
	{
		Breakpoint();
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "<include"))
	{
		//include another file at the current script location

		int endloc = LineData.find(">");

		if (endloc == -1)
		{
			ScriptError("Syntax error");
			return sError;
		}

		std::string includefile = LineData.substr(9, endloc - 9);
		TrimString(includefile);

		//delete current line
		BuildingData.erase(BuildingData.begin() + line);

		//insert file at current line
		std::string filename = Simcore->GetUtility()->VerifyFile(includefile);
		bool result = LoadDataFile(filename, true, line);
		if (result == false)
			return sError;
		engine->Report("Inserted file " + includefile);

		//reset progress
		progress_marker = 0;

		line--;
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "<function"))
	{
		//define a function

		if (config->SectionNum != SECTION_NONE)
		{
			ScriptError("Cannot define a function within a section");
			return sError;
		}

		int endloc = LineData.find(">");

		if (endloc == -1)
		{
			ScriptError("Syntax error");
			return sError;
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
			functions.emplace_back(info);
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
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "<textures>"))
	{
		if (config->SectionNum != SECTION_NONE)
		{
			ScriptError("Already within a section");
			return sError;
		}
		config->SectionNum = SECTION_TEXTURE;
		config->Context = "Textures";
		engine->Report("Processing textures...");
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "<endfunction>") && InFunction > 0)
	{
		//end function and return to original line
		line = FunctionStack[InFunction - 1].CallLine - 1;
		ReplaceLineData = FunctionStack[InFunction - 1].LineData;
		FunctionData &data = FunctionStack[InFunction - 1];

		if (in_runloop == false)
			ReplaceLine = true;

		if (data.Name == "runloop")
		{
			in_runloop = false;
			processed_runloop = true;
		}
		FunctionStack.erase(FunctionStack.begin() + InFunction - 1);
		InFunction -= 1;
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "<floors"))
	{
		if (config->SectionNum != SECTION_NONE)
		{
			ScriptError("Already within a section");
			return sError;
		}
		config->SectionNum = SECTION_FLOOR;
		std::string linecheck = SetCaseCopy(LineData, false);
		int loc = linecheck.find("to", 0);
		if (loc < 0)
		{
			ScriptError("Syntax error");
			return sError;

		}
		//get low and high range markers
		std::string str1 = LineData.substr(8, loc - 9);
		std::string str2 = LineData.substr(loc + 2, LineData.length() - (loc + 2) - 1);
		TrimString(str1);
		TrimString(str2);
		if (!IsNumeric(str1, config->RangeL) || !IsNumeric(str2, config->RangeH))
		{
			ScriptError("Invalid range");
			return sError;
		}
		config->Context = "Floor range " + ToString(config->RangeL) + " to " + ToString(config->RangeH);
		config->Current = config->RangeL;
		config->RangeStart = line;
		if (InRunloop() == false)
			engine->Report("Processing floors " + ToString(config->RangeL) + " to " + ToString(config->RangeH) + "...");
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "<floor "))
	{
		if (config->SectionNum != SECTION_NONE)
		{
			ScriptError("Already within a section");
			return sError;
		}
		config->SectionNum = SECTION_FLOOR;
		config->RangeL = 0;
		config->RangeH = 0;
		std::string str = LineData.substr(7, LineData.length() - 8);
		TrimString(str);
		if (!IsNumeric(str, config->Current))
		{
			ScriptError("Invalid floor");
			return sError;
		}
		config->Context = "Floor " + ToString(config->Current);
		if (InRunloop() == false)
			engine->Report("Processing floor " + ToString(config->Current) + "...");
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "<elevators"))
	{
		if (config->SectionNum != SECTION_NONE)
		{
			ScriptError("Already within a section");
			return sError;
		}
		config->SectionNum = SECTION_ELEVATOR;
		std::string linecheck = SetCaseCopy(LineData, false);
		int loc = linecheck.find("to", 10);
		if (loc < 0)
		{
			ScriptError("Syntax error");
			return sError;
		}
		std::string str1 = LineData.substr(11, loc - 12);
		std::string str2 = LineData.substr(loc + 2, LineData.length() - (loc + 2) - 1);
		TrimString(str1);
		TrimString(str2);
		if (!IsNumeric(str1, config->RangeL) || !IsNumeric(str2, config->RangeH))
		{
			ScriptError("Invalid range");
			return sError;
		}
		config->Context = "Elevator range " + ToString(config->RangeL) + " to " + ToString(config->RangeH);
		config->Current = config->RangeL;
		config->RangeStart = line;
		if (InRunloop() == false)
			engine->Report("Processing elevators " + ToString(config->RangeL) + " to " + ToString(config->RangeH) + "...");
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "<elevator "))
	{
		if (config->SectionNum != SECTION_NONE)
		{
			ScriptError("Already within a section");
			return sError;
		}
		config->SectionNum = SECTION_ELEVATOR;
		config->RangeL = 0;
		config->RangeH = 0;
		std::string str = LineData.substr(10, LineData.length() - 11);
		TrimString(str);
		if (!IsNumeric(str, config->Current))
		{
			ScriptError("Invalid elevator");
			return sError;
		}
		if (config->Current < 1 || config->Current > Simcore->GetElevatorCount() + 1)
		{
			ScriptError("Invalid elevator");
			return sError;
		}
		config->Context = "Elevator " + ToString(config->Current);
		if (InRunloop() == false)
			engine->Report("Processing elevator " + ToString(config->Current) + "...");
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "<buildings>"))
	{
		//skip this section if reloading
		if (engine->IsReloading() == true)
			return sNextLine;

		if (config->SectionNum != SECTION_NONE)
		{
			ScriptError("Already within a section");
			return sError;
		}
		config->SectionNum = SECTION_BUILDINGS;
		config->Context = "Buildings";
		if (InRunloop() == false)
			engine->Report("Loading other buildings...");
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "<cars") && config->SectionNum == SECTION_ELEVATOR)
	{
		config->SectionNum = SECTION_ELEVATORCAR;
		std::string linecheck = SetCaseCopy(LineData, false);
		int loc = linecheck.find("to", 5);
		if (loc < 0)
		{
			ScriptError("Syntax error");
			return sError;
		}

		//store previous elevator section values
		config->CurrentOld = config->Current;
		config->RangeLOld = config->RangeL;
		config->RangeHOld = config->RangeH;
		config->RangeStartOld = config->RangeStart;
		config->ContextOld = config->Context;

		std::string str1 = LineData.substr(6, loc - 7);
		std::string str2 = LineData.substr(loc + 2, LineData.length() - (loc + 2) - 1);
		TrimString(str1);
		TrimString(str2);
		if (!IsNumeric(str1, config->RangeL) || !IsNumeric(str2, config->RangeH))
		{
			ScriptError("Invalid range");
			return sError;
		}

		//verify elevator
		if (!Simcore->GetElevator(config->CurrentOld))
		{
			ScriptError("Invalid elevator");
			return sError;
		}

		config->Context = "Elevator " + ToString(config->CurrentOld) + " Car range " + ToString(config->RangeL) + " to " + ToString(config->RangeH);
		config->Current = config->RangeL;
		config->RangeStart = line;
		if (InRunloop() == false)
			engine->Report("Processing elevator " + ToString(config->CurrentOld) + " cars " + ToString(config->RangeL) + " to " + ToString(config->RangeH) + "...");
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "<car ") && config->SectionNum == SECTION_ELEVATOR)
	{
		config->SectionNum = SECTION_ELEVATORCAR;

		//store previous elevator section values
		config->CurrentOld = config->Current;
		config->RangeLOld = config->RangeL;
		config->RangeHOld = config->RangeH;
		config->RangeStartOld = config->RangeStart;
		config->ContextOld = config->Context;

		config->RangeL = 0;
		config->RangeH = 0;
		std::string str = LineData.substr(5, LineData.length() - 6);
		TrimString(str);
		if (!IsNumeric(str, config->Current))
		{
			ScriptError("Invalid car number");
			return sError;
		}

		//verify elevator
		if (!Simcore->GetElevator(config->CurrentOld))
		{
			ScriptError("Invalid elevator");
			return sError;
		}

		config->Context = "Elevator " + ToString(config->CurrentOld) + " Car " + ToString(config->Current);
		if (InRunloop() == false)
			engine->Report("Processing elevator " + ToString(config->CurrentOld) + " car " + ToString(config->Current) + "...");
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "<vehicles"))
	{
		if (config->SectionNum != SECTION_NONE)
		{
			ScriptError("Already within a section");
			return sError;
		}
		config->SectionNum = SECTION_VEHICLE;
		std::string linecheck = SetCaseCopy(LineData, false);
		int loc = linecheck.find("to", 9);
		if (loc < 0)
		{
			ScriptError("Syntax error");
			return sError;
		}
		std::string str1 = LineData.substr(10, loc - 11);
		std::string str2 = LineData.substr(loc + 2, LineData.length() - (loc + 2) - 1);
		TrimString(str1);
		TrimString(str2);
		if (!IsNumeric(str1, config->RangeL) || !IsNumeric(str2, config->RangeH))
		{
			ScriptError("Invalid range");
			return sError;
		}
		config->Context = "Vehicle range " + ToString(config->RangeL) + " to " + ToString(config->RangeH);
		config->Current = config->RangeL;
		config->RangeStart = line;
		if (InRunloop() == false)
			engine->Report("Processing vehicles " + ToString(config->RangeL) + " to " + ToString(config->RangeH) + "...");
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "<vehicle "))
	{
		if (config->SectionNum != SECTION_NONE)
		{
			ScriptError("Already within a section");
			return sError;
		}
		config->SectionNum = SECTION_VEHICLE;
		config->RangeL = 0;
		config->RangeH = 0;
		std::string str = LineData.substr(9, LineData.length() - 10);
		TrimString(str);
		if (!IsNumeric(str, config->Current))
		{
			ScriptError("Invalid vehicle");
			return sError;
		}
		if (config->Current < 1 || config->Current > Simcore->GetVehicleCount() + 1)
		{
			ScriptError("Invalid vehicle");
			return sError;
		}
		config->Context = "Vehicle " + ToString(config->Current);
		engine->Report("Processing vehicle " + ToString(config->Current) + "...");
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "<controllers"))
	{
		if (config->SectionNum != SECTION_NONE)
		{
			ScriptError("Already within a section");
			return sError;
		}
		config->SectionNum = SECTION_CONTROLLER;
		std::string linecheck = SetCaseCopy(LineData, false);
		int loc = linecheck.find("to", 12);
		if (loc < 0)
		{
			ScriptError("Syntax error");
			return sError;
		}
		std::string str1 = LineData.substr(13, loc - 14);
		std::string str2 = LineData.substr(loc + 2, LineData.length() - (loc + 2) - 1);
		TrimString(str1);
		TrimString(str2);
		if (!IsNumeric(str1, config->RangeL) || !IsNumeric(str2, config->RangeH))
		{
			ScriptError("Invalid range");
			return sError;
		}
		config->Context = "Controller range " + ToString(config->RangeL) + " to " + ToString(config->RangeH);
		config->Current = config->RangeL;
		config->RangeStart = line;
		if (InRunloop() == false)
			engine->Report("Processing controllers " + ToString(config->RangeL) + " to " + ToString(config->RangeH) + "...");
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "<controller "))
	{
		if (config->SectionNum != SECTION_NONE)
		{
			ScriptError("Already within a section");
			return sError;
		}
		config->SectionNum = SECTION_CONTROLLER;
		config->RangeL = 0;
		config->RangeH = 0;
		std::string str = LineData.substr(12, LineData.length() - 13);
		TrimString(str);
		if (!IsNumeric(str, config->Current))
		{
			ScriptError("Invalid controller");
			return sError;
		}
		if (config->Current < 1 || config->Current > Simcore->GetControllerCount() + 1)
		{
			ScriptError("Invalid controller");
			return sError;
		}
		config->Context = "Controller " + ToString(config->Current);
		if (InRunloop() == false)
			engine->Report("Processing controller " + ToString(config->Current) + "...");
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "<callstations") && config->SectionNum == SECTION_FLOOR)
	{
		config->SectionNum = SECTION_CALLSTATION;
		std::string linecheck = SetCaseCopy(LineData, false);
		int loc = linecheck.find("to", 13);
		if (loc < 0)
		{
			ScriptError("Syntax error");
			return sError;
		}

		//store previous elevator section values
		config->CurrentOld = config->Current;
		config->RangeLOld = config->RangeL;
		config->RangeHOld = config->RangeH;
		config->RangeStartOld = config->RangeStart;
		config->ContextOld = config->Context;

		std::string str1 = LineData.substr(14, loc - 15);
		std::string str2 = LineData.substr(loc + 2, LineData.length() - (loc + 2) - 1);
		TrimString(str1);
		TrimString(str2);
		if (!IsNumeric(str1, config->RangeL) || !IsNumeric(str2, config->RangeH))
		{
			ScriptError("Invalid range");
			return sError;
		}

		//verify floor
		if (!Simcore->GetFloor(config->CurrentOld))
		{
			ScriptError("Invalid floor");
			return sError;
		}

		config->Context = "Floor " + ToString(config->CurrentOld) + " Call Station range " + ToString(config->RangeL) + " to " + ToString(config->RangeH);
		config->Current = config->RangeL;
		config->RangeStart = line;
		if (Simcore->Verbose && InRunloop() == false)
			engine->Report("Processing floor " + ToString(config->CurrentOld) + " call stations " + ToString(config->RangeL) + " to " + ToString(config->RangeH) + "...");
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "<callstation ") && config->SectionNum == SECTION_FLOOR)
	{
		config->SectionNum = SECTION_CALLSTATION;

		//store previous elevator section values
		config->CurrentOld = config->Current;
		config->RangeLOld = config->RangeL;
		config->RangeHOld = config->RangeH;
		config->RangeStartOld = config->RangeStart;
		config->ContextOld = config->Context;

		config->RangeL = 0;
		config->RangeH = 0;
		std::string str = LineData.substr(13, LineData.length() - 14);
		TrimString(str);
		if (!IsNumeric(str, config->Current))
		{
			ScriptError("Invalid call station number");
			return sError;
		}

		//verify floor
		if (!Simcore->GetFloor(config->CurrentOld))
		{
			ScriptError("Invalid floor");
			return sError;
		}

		config->Context = "Floor " + ToString(config->CurrentOld) + " Call Station " + ToString(config->Current);
		if (Simcore->Verbose && InRunloop() == false)
			engine->Report("Processing floor " + ToString(config->CurrentOld) + " call station " + ToString(config->Current) + "...");
		return sNextLine;
	}

	return sContinue;
}

int ScriptProcessor::ProcessFloorObjects()
{
	//////////////////////////
	//Floor object conversion
	//////////////////////////
	int exists = SetCaseCopy(LineData, false).find("floor(", 0);
	while (exists > -1)
	{
		int loc1 = LineData.find("(", exists);
		int loc2 = LineData.find(")", exists);
		if (loc2 < 0)
		{
			ScriptError("Syntax error");
			return sError;
		}
		if (config->SectionNum == SECTION_FLOOR && getfloordata == false)
		{
			//process floor-specific variables if in a floor section
			getfloordata = true;
			return sRecalc;
		}
		else
			getfloordata = false;
		std::string tempdata = Calc(LineData.substr(loc1 + 1, loc2 - loc1 - 1));
		LineData = LineData.substr(0, loc1 + 1) + tempdata + LineData.substr(loc2);

		int floor = 0;
		if (!IsNumeric(tempdata, floor))
		{
			ScriptError("Invalid floor " + tempdata);
			return sError;
		}
		if (Simcore->IsValidFloor(floor) == false)
		{
			ScriptError("Invalid floor " + tempdata);
			return sError;
		}

		//fullheight parameter
		std::string buffer = ToString(floor);
		TrimString(buffer);
		std::string name = "floor(" + buffer + ").fullheight";
		buffer = LineData;
		SetCase(buffer, false);
		loc1 = buffer.find(name, 0);
		if (loc1 > 0)
		{
			buffer = ToString(Simcore->GetFloor(floor)->FullHeight());
			TrimString(buffer);
			LineData = LineData.substr(0, loc1) + buffer + LineData.substr(loc1 + name.length());
		}

		//height parameter
		buffer = ToString(floor);
		TrimString(buffer);
		name = "floor(" + buffer + ").height";
		buffer = LineData;
		SetCase(buffer, false);
		loc1 = buffer.find(name, 0);
		if (loc1 > 0)
		{
			buffer = ToString(Simcore->GetFloor(floor)->Height);
			TrimString(buffer);
			LineData = LineData.substr(0, loc1) + buffer + LineData.substr(loc1 + name.length());
		}

		//altitude parameter
		buffer = ToString(floor);
		TrimString(buffer);
		name = "floor(" + buffer + ").altitude";
		buffer = LineData;
		SetCase(buffer, false);
		loc1 = buffer.find(name, 0);
		if (loc1 > 0)
		{
			buffer = ToString(Simcore->GetFloor(floor)->Altitude);
			TrimString(buffer);
			LineData = LineData.substr(0, loc1) + buffer + LineData.substr(loc1 + name.length());
		}

		//interfloorheight parameter
		buffer = ToString(floor);
		TrimString(buffer);
		name = "floor(" + buffer + ").interfloorheight";
		buffer = LineData;
		SetCase(buffer, false);
		loc1 = buffer.find(name, 0);
		if (loc1 > 0)
		{
			buffer = ToString(Simcore->GetFloor(floor)->InterfloorHeight);
			TrimString(buffer);
			LineData = LineData.substr(0, loc1) + buffer + LineData.substr(loc1 + name.length());
		}
		exists = SetCaseCopy(LineData, false).find("floor(", 0);

		//base parameter
		buffer = ToString(floor);
		TrimString(buffer);
		name = "floor(" + buffer + ").base";
		buffer = LineData;
		SetCase(buffer, false);
		loc1 = buffer.find(name, 0);
		if (loc1 > 0)
		{
			buffer = ToString(Simcore->GetFloor(floor)->GetBase());
			TrimString(buffer);
			LineData = LineData.substr(0, loc1) + buffer + LineData.substr(loc1 + name.length());
		}
		exists = SetCaseCopy(LineData, false).find("floor(", 0);
	}

	return sContinue;
}

void ScriptProcessor::Breakpoint()
{
	//breakpoint function for debugging scripts
	engine->Report("Script breakpoint reached");
}

void ScriptProcessor::ProcessExtents()
{
	//Extent variables
	ReplaceAll(LineData, "%minx%", ToString(config->MinExtent.x));
	ReplaceAll(LineData, "%minz%", ToString(config->MinExtent.z));
	ReplaceAll(LineData, "%maxx%", ToString(config->MaxExtent.x));
	ReplaceAll(LineData, "%maxz%", ToString(config->MaxExtent.z));
}

ScriptProcessor::ConfigHandler* ScriptProcessor::GetConfigHandler()
{
	return config;
}

int ScriptProcessor::ProcessForLoops()
{
	if (StartsWithNoCase(LineData, "<for "))
	{
		//process a For loop

		std::string linecheck = SetCaseCopy(LineData, false);
		int loc = linecheck.find("to", 0);
		if (loc < 0)
		{
			ScriptError("Syntax error");
			return sError;
		}

		//get iterator
		int loc2 = linecheck.find(" ", 5);
		if (loc < 0)
		{
			ScriptError("Syntax error");
			return sError;
		}
		std::string it = LineData.substr(5, loc2 - 5);
		TrimString(it);

		//check for existence of iterator variable
		for (int i = 0; i < variables.size(); i++)
		{
			if (variables[i].name == it)
			{
				ScriptError("Iterator variable in use");
				return sError;
			}
		}

		//get low and high range markers
		std::string str1 = LineData.substr(loc2, loc - (loc2 + 1));
		std::string str2 = LineData.substr(loc + 2, LineData.length() - (loc + 2) - 1);
		TrimString(str1);
		TrimString(str2);
		int RangeL, RangeH;
		if (!IsNumeric(str1, RangeL) || !IsNumeric(str2, RangeH))
		{
			ScriptError("Invalid range");
			return sError;
		}

		//set new iterator variable
		VariableMap var;
		var.name = it;
		var.value = ToString(RangeL);
		variables.emplace_back(var);

		//set up for loop
		ForInfo info;
		info.iterator = it;
		info.line = line;
		info.i = RangeL;
		info.start = RangeL;
		info.end = RangeH;

		ForLoops.emplace_back(info);

		return sNextLine;
	}
	else if (StartsWithNoCase(LineData, "<endfor>"))
	{
		ForInfo &info = ForLoops.back();

		bool end = false;
		if (info.start < info.end)
		{
			info.i++;
			if (info.i > info.end)
				end = true;
		}
		else
		{
			info.i--;
			if (info.i < info.end)
				end = true;
		}

		for (int i = 0; i < variables.size(); i++)
		{
			if (variables[i].name == info.iterator)
			{
				if (end == false)
				{
					//put iterator into variable
					variables[i].value = ToString(info.i);
					break;
				}
				else
				{
					//remove iterator variable
					variables.erase(variables.begin() + i);
					break;
				}
			}
		}

		if (end == true)
		{
			//process end of For loop
			ForLoops.pop_back();
			return sNextLine;
		}
		else
			return sLoopFor;
	}

	return sContinue;
}

bool ScriptProcessor::HasRunloop()
{
	//return true if this building has a runloop function

	for (int i = 0; i < functions.size(); i++)
	{
		if (functions[i].name == "runloop")
		{
			return true;
		}
	}
	return false;
}

void ScriptProcessor::LoadDefaults()
{
	Simcore->BuildingName = "Default";
	Simcore->BuildingDesigner = "Me";
	Simcore->SkyName = "noon";
	//Simcore->camera->EnableCollisions(false);
	//Simcore->camera->EnableGravity(false);
}

void ScriptProcessor::Start()
{
	IsFinished = true;
	show_percent = false;
}

size_t ScriptProcessor::GetFunctionCount()
{
	return functions.size();
}

ScriptProcessor::FunctionInfo ScriptProcessor::GetFunctionInfo(size_t index)
{
	FunctionInfo info;
	info.line = 0;
	info.name = "";

	if (index >= functions.size())
		return info;

	return functions[index];
}

}
