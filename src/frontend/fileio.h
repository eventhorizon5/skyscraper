/* $Id$ */

/*
	Skyscraper 1.8 Alpha - File I/O and Script Processing Routines
	Copyright (C)2004-2011 Ryan Thoryk
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

class ScriptProcessor;

class ScriptProcessor
{
	public:
	ScriptProcessor();
	~ScriptProcessor();
	bool LoadBuilding();
	bool LoadDataFile(const char *filename, bool insert = false, int insert_line = 0);
	bool LoadFromText(const char *text);
	bool ReportMissingFiles();
	int SplitData(const char *string, int start, bool calc = true);
	int SplitAfterEquals(const char *string, bool calc = true);
	std::string GetAfterEquals(const char *string);

	private:
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
	WallObject *wall;
	char intbuffer[65];
	std::string buffer;
	int startpos;
	bool getfloordata;
	bool setshaftdoors;
	std::vector<std::string> BuildingData;
	std::vector<std::string> UserVariable;
	Ogre::Vector3 MinExtent;
	Ogre::Vector3 MaxExtent;
	bool InFunction;
	int FunctionCallLine;
	std::string FunctionCallLineData;
	std::vector<std::string> FunctionParams;
	bool ReplaceLine;
	std::vector<std::string> nonexistent_files;
	bool CalcError;

	int ScriptError(std::string message);
	int ScriptError();
	std::string Calc(const char *expression);
	bool IfProc(const char *expression);
	void StoreCommand(Object *object);
	int ProcCommands();
	int ProcGlobals();
	int ProcFloors();
	int ProcElevators();
	int ProcTextures();
	bool FunctionProc();
	void CheckFile(const char *filename);

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
	};

	std::vector<IncludeInfo> includes; //stored include mappings
};

#endif
