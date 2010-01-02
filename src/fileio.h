/* $Id$ */

/*
	Skyscraper 1.6 Alpha - File I/O and Script Processing Routines
	Copyright (C)2005-2009 Ryan Thoryk
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
	bool LoadDataFile(const char *filename);
	bool LoadFromText(const char *text);

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
	iMeshWrapper *tmpMesh;
	WallObject *wall;
	char intbuffer[65];
	csString buffer;
	int startpos;
	bool getfloordata;
	bool setshaftdoors;
	csArray<csString> BuildingData;
	csArray<csString> UserVariable;
	csVector3 Extent;

	bool ScriptError(const char *message);
	csString Calc(const char *expression);
	bool IfProc(const char *expression);
	int ProcCommands();
	int ProcGlobals();
	int ProcFloors();
	int ProcElevators();
	int ProcTextures();
};

#endif
