/* $Id$ */

/*
	Skyscraper 1.10 Alpha - File I/O and Script Processing Routines
	Copyright (C)2004-2016 Ryan Thoryk
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

#ifndef FILEIO_H
#define FILEIO_H

namespace Skyscraper {

class EngineContext;

class ScriptProcessor
{
public:
	ScriptProcessor(EngineContext *instance);
	~ScriptProcessor();
	bool Run();
	bool LoadDataFile(const std::string &filename, bool insert = false, int insert_line = 0);
	bool LoadFromText(const std::string &text);
	bool ReportMissingFiles();
	int SplitData(const std::string &string, int start, bool calc = true);
	int SplitAfterEquals(const std::string &string, bool calc = true);
	std::string GetAfterEquals(const std::string &string);
	void Reset();
	std::vector<std::string> *GetBuildingData();
	int MathFunctions();
	bool IsFunctionDefined(const std::string &name);
	SBS::MeshObject* GetMeshObject(std::string name);

	bool IsFinished;

private:

	SBS::SBS *Simcore;
	EngineContext *engine;

	struct FunctionData
	{
		std::string Name;
		int CallLine;
		std::string LineData;
		std::vector<std::string> Params;
	};

	int line; //line number
	std::string LineData; //line text
	int Current; //current range iteration
	int Section; //current section number
	std::string Context; //section context
	int temp1;
	std::string temp2;
	int temp3;
	int temp4;
	int temp5;
	std::string temp6;
	std::vector<std::string> tempdata;
	std::vector<int> callbutton_elevators;
	int FloorCheck;
	int RangeL;
	int RangeH;
	long RangeStart;
	SBS::WallObject *wall;
	std::string buffer;
	int startpos;
	bool getfloordata;
	bool setshaftdoors;
	std::vector<std::string> BuildingData;
	std::vector<std::string> BuildingDataOrig;
	Ogre::Vector3 MinExtent;
	Ogre::Vector3 MaxExtent;
	int InFunction;
	std::vector<FunctionData> FunctionStack;
	bool ReplaceLine;
	std::string ReplaceLineData;
	std::vector<std::string> nonexistent_files;
	bool CalcError;
	bool ReverseAxis;
	bool setkey;
	int keyvalue;
	int lockvalue;
	bool warn_deprecated;
	bool show_percent;
	bool InWhile;
	int progress_marker;

	int ScriptError(std::string message, bool warning = false);
	int ScriptError();
	int ScriptWarning(std::string message);
	std::string Calc(const std::string &expression);
	bool IfProc(const std::string &expression);
	void StoreCommand(SBS::Object *object);
	int ProcCommands();
	int ProcGlobals();
	int ProcFloors();
	int ProcElevators();
	int ProcTextures();
	int ProcBuildings();
	bool FunctionProc();
	void CheckFile(const std::string &filename);
	void GetLineInformation(bool CheckFunctionCall, int &LineNumber, int &FunctionLine, bool &IsInclude, std::string &IncludeFile, bool &IsIncludeFunction, std::string &IncludeFunctionFile);

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

	struct VariableMap
	{
		std::string name;
		std::string value;
	};

	std::vector<VariableMap> variables; //named user variables

	bool floorcache_firstrun;
	int cache_current;
	std::string cache_current_s;
	float cache_height;
	std::string cache_height_s;
	float cache_fullheight;
	std::string cache_fullheight_s;
	float cache_interfloorheight;
	std::string cache_interfloorheight_s;
	float cache_base;
	std::string cache_base_s;
};

}

#endif
