/* $Id$ */

/*
	Skyscraper 1.8 Alpha - File I/O and Script Processing Routines
	Copyright (C)2004-2010 Ryan Thoryk
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

#include <csutil/stringarray.h>

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
	csString GetAfterEquals(const char *string);

	private:
	int line; //line number
	csString LineData; //line text
	int Current; //current range iteration
	int Section; //current section number
	csString Context; //section context
	int temp1;
	csString temp2;
	int temp3;
	int temp4;
	int temp5;
	csString temp6;
	csStringArray tempdata;
	csArray<int> callbutton_elevators;
	int FloorCheck;
	int RangeL;
	int RangeH;
	long RangeStart;
	WallObject *wall;
	char intbuffer[65];
	csString buffer;
	int startpos;
	bool getfloordata;
	bool setshaftdoors;
	csArray<csString> BuildingData;
	csArray<csString> UserVariable;
	csVector3 MinExtent;
	csVector3 MaxExtent;
	bool InFunction;
	int FunctionCallLine;
	csString FunctionCallLineData;
	csArray<csString> FunctionParams;
	bool ReplaceLine;
	csArray<csString> nonexistent_files;
	bool CalcError;

	int ScriptError(const char *message);
	int ScriptError();
	csString Calc(const char *expression);
	bool IfProc(const char *expression);
	void StoreCommand(Object *object);
	int ProcCommands();
	int ProcGlobals();
	int ProcFloors();
	int ProcElevators();
	int ProcTextures();
	bool FunctionProc();
	void CheckFile(const char *filename, bool relative = false);

	struct FunctionInfo
	{
		csString name;
		int line;
	};

	csArray<FunctionInfo> functions; //stored functions

	struct IncludeInfo
	{
		csString filename;
		int start_line;
		int end_line;
	};

	csArray<IncludeInfo> includes; //stored include mappings
};

#endif
