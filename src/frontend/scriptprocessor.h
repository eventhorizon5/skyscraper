/* $Id$ */

/*
	Skyscraper 1.10 Alpha - Script Processor
	Copyright (C)2004-2016 Ryan Thoryk
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

#ifndef SCRIPTPROCESSOR_H
#define SCRIPTPROCESSOR_H

namespace Skyscraper {

class ScriptProcessor
{
public:

	//sub-classes
	class Section;
	class GlobalsSection;
	class BuildingsSection;
	class TexturesSection;
	class CommandsSection;
	class FloorSection;
	class ElevatorSection;
	class ElevatorCarSection;

	ScriptProcessor(EngineContext *instance);
	~ScriptProcessor();
	bool Run();
	bool LoadDataFile(const std::string &filename, bool insert = false, int insert_line = 0);
	bool LoadFromText(const std::string &text);
	bool ReportMissingFiles();
	void Reset();
	std::vector<std::string> *GetBuildingData();
	bool IsFunctionDefined(const std::string &name);
	SBS::MeshObject* GetMeshObject(std::string name);
	std::string DumpState();
	EngineContext* GetEngine();
	ElevatorCarSection* GetElevatorCarSection();

	bool IsFinished;

	//return codes
	static const int sContinue = 0;
	static const int sNextLine = 1;
	static const int sError = 2;
	static const int sCheckFloors = 3;
	static const int sBreak = 4;
	static const int sRecalc = 5;
	static const int sSkipReset = 6;
	static const int sExit = 7;

	struct VariableMap
	{
		std::string name;
		std::string value;
	};

	std::vector<VariableMap> variables; //named user variables

	bool getfloordata;
	int line; //line number
	std::string LineData; //line text

private:

	SBS::SBS *Simcore;
	EngineContext *engine;

	GlobalsSection *globals_section;
	BuildingsSection *buildings_section;
	TexturesSection *textures_section;
	CommandsSection *commands_section;
	FloorSection *floor_section;
	ElevatorSection *elevator_section;
	ElevatorCarSection *elevatorcar_section;

	struct FunctionData
	{
		std::string Name;
		int CallLine;
		std::string LineData;
		std::vector<std::string> Params;
	};

	SBS::WallObject *wall;
	int startpos;
	std::vector<std::string> BuildingData;
	std::vector<std::string> BuildingDataOrig;
	int InFunction;
	std::vector<FunctionData> FunctionStack;
	bool ReplaceLine;
	std::string ReplaceLineData;
	std::vector<std::string> nonexistent_files;
	bool CalcError;
	bool show_percent;
	int progress_marker;

	int ScriptError(std::string message, bool warning = false);
	int ScriptError();
	int ScriptWarning(std::string message);
	std::string Calc(const std::string &expression);
	void StoreCommand(SBS::Object *object);
	bool FunctionProc();
	void CheckFile(const std::string &filename);
	void GetLineInformation(bool CheckFunctionCall, int &LineNumber, std::string &FunctionName, int &FunctionLine, bool &IsInclude, std::string &IncludeFile, bool &IsIncludeFunction, std::string &IncludeFunctionFile);
	void ProcessFunctionParameters();
	void ProcessUserVariables();
	int ProcessSections();
	int ProcessFloorObjects();
	void Breakpoint();
	void ProcessExtents();

	struct FunctionInfo
	{
		std::string name;
		int line;
	};

	std::vector<FunctionInfo> functions; //stored functions

	struct IncludeInfo
	{
		std::string filename;
		int start_line;
		int end_line;
		int parent;
	};

	std::vector<IncludeInfo> includes; //stored include mappings
};

}

#endif
