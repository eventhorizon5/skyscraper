/* $Id$ */

/*
	Skyscraper 1.8 Alpha - File I/O and Script Processing Routines
	Copyright (C)2003-2010 Ryan Thoryk
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

#include <wx/variant.h>
#include "globals.h"
#include "sbs.h"
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

#include <csutil/filereadhelper.h>

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
	UserVariable.SetSize(256);
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
	nonexistent_files.DeleteAll();

	while (line < BuildingData.GetSize() - 1)
	{
		if (Simcore->GetFloor(0) == 0)
			LineData.Trim();

		LineData = BuildingData[line];
		LineData.Trim();

		if (ReplaceLine == true)
		{
			LineData = FunctionCallLineData;
			ReplaceLine = false;
		}

		//skip blank lines
		if (LineData == "")
			goto Nextline;

		//process comment markers
		temp1 = LineData.Find("#", 0);
		if (temp1 > -1)
			LineData.Truncate(temp1);

		//skip blank lines
		if (LineData == "")
			goto Nextline;

		//function parameter variables
		if (InFunction == true)
		{
			for (int i = 0; i < FunctionParams.GetSize(); i++)
			{
				csString num = _itoa(i + 1, intbuffer, 10);
				LineData.ReplaceAll("%param" + num + "%", FunctionParams[i]);
			}
		}

		//////////////////////
		//Section information
		//////////////////////
		if (LineData.CompareNoCase("<globals>") == true)
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
		if (LineData.CompareNoCase("<endglobals>") == true)
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
		if (LineData.Slice(0, 7).CompareNoCase("<floors") == true)
		{
			if (Section > 0)
			{
				ScriptError("Already within a section");
				return false;
			}
			Section = 2;
			temp3 = csString(LineData).Downcase().Find("to", 0);
			if (temp3 < 0)
			{
				ScriptError("Syntax error");
				return false;

			}
			//get low and high range markers
			if (!IsNumeric(LineData.Slice(8, temp3 - 9).Trim().GetData(), RangeL) || !IsNumeric(LineData.Slice(temp3 + 2, LineData.Length() - (temp3 + 2) - 1).Trim().GetData(), RangeH))
			{
				ScriptError("Invalid range");
				return false;
			}
			Context = "Floor range " + csString(_itoa(RangeL, intbuffer, 10)) + " to " + csString(_itoa(RangeH, intbuffer, 10));
			Current = RangeL;
			RangeStart = line;
			skyscraper->Report("Processing floors " + csString(_itoa(RangeL, intbuffer, 10)) + " to " + csString(_itoa(RangeH, intbuffer, 10)) + "...");
			goto Nextline;
		}
		if (LineData.Slice(0, 7).CompareNoCase("<floor ") == true)
		{
			if (Section > 0)
			{
				ScriptError("Already within a section");
				return false;
			}
			Section = 2;
			RangeL = 0;
			RangeH = 0;
			if (!IsNumeric(LineData.Slice(7, LineData.Length() - 8).Trim().GetData(), Current))
			{
				ScriptError("Invalid floor");
				return false;
			}
			Context = "Floor " + csString(_itoa(Current, intbuffer, 10));
			skyscraper->Report("Processing floor " + csString(_itoa(Current, intbuffer, 10)) + "...");
			goto Nextline;
		}
		if (LineData.CompareNoCase("<endfloor>") == true && RangeL == RangeH)
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
		if (LineData.Slice(0, 10).CompareNoCase("<elevators") == true)
		{
			if (Section > 0)
			{
				ScriptError("Already within a section");
				return false;
			}
			Section = 4;
			temp3 = csString(LineData).Downcase().Find("to", 10);
			if (temp3 < 0)
			{
				ScriptError("Syntax error");
				return false;
			}
			if (!IsNumeric(LineData.Slice(11, temp3 - 12).Trim().GetData(), RangeL) || !IsNumeric(LineData.Slice(temp3 + 2, LineData.Length() - (temp3 + 2) - 1).Trim().GetData(), RangeH))
			{
				ScriptError("Invalid range");
				return false;
			}
			Context = "Elevator range " + csString(_itoa(RangeL, intbuffer, 10)) + " to " + csString(_itoa(RangeH, intbuffer, 10));
			Current = RangeL;
			RangeStart = line;
			skyscraper->Report("Processing elevators " + csString(_itoa(RangeL, intbuffer, 10)) + " to " + csString(_itoa(RangeH, intbuffer, 10)) + "...");
			goto Nextline;
		}
		if (LineData.Slice(0, 10).CompareNoCase("<elevator ") == true)
		{
			if (Section > 0)
			{
				ScriptError("Already within a section");
				return false;
			}
			Section = 4;
			RangeL = 0;
			RangeH = 0;
			if (!IsNumeric(LineData.Slice(10, LineData.Length() - 11).Trim().GetData(), Current))
			{
				ScriptError("Invalid elevator");
				return false;
			}
			if (Current < 1 || Current > Simcore->Elevators() + 1)
			{
				ScriptError("Invalid elevator");
				return false;
			}
			Context = "Elevator " + csString(_itoa(Current, intbuffer, 10));
			skyscraper->Report("Processing elevator " + csString(_itoa(Current, intbuffer, 10)) + "...");
			goto Nextline;
		}
		if (LineData.CompareNoCase("<endelevator>") == true && RangeL == RangeH)
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
		if (LineData.Slice(0, 10).CompareNoCase("<textures>") == true)
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
		if (LineData.Slice(0, 13).CompareNoCase("<endtextures>") == true)
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
		if (LineData.Slice(0, 5).CompareNoCase("<end>") == true)
		{
			Section = 0;
			Context = "None";
			skyscraper->Report("Exiting building script");
			break; //exit data file parser
		}
		if (LineData.Slice(0, 7).CompareNoCase("<break>") == true)
		{
			//breakpoint function for debugging scripts
breakpoint:
			skyscraper->Report("Script breakpoint reached");
			goto Nextline;
		}
		if (LineData.Slice(0, 8).CompareNoCase("<include") == true)
		{
			//include another file at the current script location

			int endloc = LineData.Find(">");
			csString includefile = LineData.Slice(9, endloc - 9).Trim();

			//delete current line
			BuildingData.DeleteIndex(line);

			//insert file at current line
			LoadDataFile(includefile, true, line);

			skyscraper->Report("Inserted file " + includefile);
			line--;
			goto Nextline;
		}
		if (LineData.Slice(0, 9).CompareNoCase("<function") == true && Section == 0)
		{
			//define a function (only available outside sections)

			int endloc = LineData.Find(">");
			csString function = LineData.Slice(10, endloc - 10).Trim();

			//store function info in array
			FunctionInfo info;
			info.name = function;
			info.line = line;
			functions.Push(info);

			//skip to end of function
			for (int i = line + 1; i < BuildingData.GetSize(); i++)
			{
				if (BuildingData[i].Slice(0, 13).CompareNoCase("<endfunction>") == true)
				{
					line = i;
					break;
				}
			}

			skyscraper->Report("Defined function " + function);
			goto Nextline;
		}
		if (LineData.Slice(0, 13).CompareNoCase("<endfunction>") == true && InFunction == true)
		{
			//end function and return to original line
			InFunction = false;
			FunctionParams.DeleteAll();
			ReplaceLine = true;
			line = FunctionCallLine - 1;
			goto Nextline;
		}

		startpos = 0;
		do
		{
			//User variable conversion
			temp1 = LineData.Find("%", startpos);
			if (temp1 >= startpos)
			{
				temp3 = LineData.Find("%", temp1 + 1);
				if (temp3 >= LineData.Length())
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
				temp2 = LineData.Slice(temp1 + 1, temp3 - temp1 - 1).Trim();
				if (IsNumeric(temp2.GetData()) == true)
				{
					temp4 = atoi(temp2.GetData());
					if (temp4 < 0 || temp4 > UserVariable.GetSize() - 1)
					{
						ScriptError("Invalid variable number");
						return false;
					}
					//replace all occurrences of the variable with it's value
					LineData.ReplaceAll("%" + temp2 + "%", UserVariable[temp4]);
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
		temp5 = csString(LineData).Downcase().Find("floor(", 0);
		while (temp5 > -1)
		{
			temp1 = LineData.Find("(", temp5);
			temp3 = LineData.Find(")", temp5);
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
			csString tempdata = Calc(LineData.Slice(temp1 + 1, temp3 - temp1 - 1)).Trim();
			LineData = LineData.Slice(0, temp1 + 1) + tempdata + LineData.Slice(temp3);

			if (!IsNumeric(tempdata.GetData(), temp4))
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
			buffer = temp4;
			temp6 = "floor(" + buffer.Trim() + ").fullheight";
			buffer = LineData;
			buffer.Downcase();
			temp1 = buffer.Find(temp6.GetData(), 0);
			if (temp1 > 0)
			{
				buffer = Simcore->GetFloor(temp4)->FullHeight();
				LineData = LineData.Slice(0, temp1) + buffer.Trim() + LineData.Slice(temp1 + temp6.Length());
			}
			//height parameter
			buffer = temp4;
			temp6 = "floor(" + buffer.Trim() + ").height";
			buffer = LineData;
			buffer.Downcase();
			temp1 = buffer.Find(temp6.GetData(), 0);
			if (temp1 > 0)
			{
				buffer = Simcore->GetFloor(temp4)->Height;
				LineData = LineData.Slice(0, temp1) + buffer.Trim() + LineData.Slice(temp1 + temp6.Length());
			}
			//altitude parameter
			buffer = temp4;
			temp6 = "floor(" + buffer.Trim() + ").altitude";
			buffer = LineData;
			buffer.Downcase();
			temp1 = buffer.Find(temp6.GetData(), 0);
			if (temp1 > 0)
			{
				buffer = Simcore->GetFloor(temp4)->Altitude;
				LineData = LineData.Slice(0, temp1) + buffer.Trim() + LineData.Slice(temp1 + temp6.Length());
			}
			//interfloorheight parameter
			buffer = temp4;
			temp6 = "floor(" + buffer.Trim() + ").interfloorheight";
			buffer = LineData;
			buffer.Downcase();
			temp1 = buffer.Find(temp6.GetData(), 0);
			if (temp1 > 0)
			{
				buffer = Simcore->GetFloor(temp4)->InterfloorHeight;
				LineData = LineData.Slice(0, temp1) + buffer.Trim() + LineData.Slice(temp1 + temp6.Length());
			}
			temp5 = csString(LineData).Downcase().Find("floor(", 0);
			//base parameter
			buffer = temp4;
			temp6 = "floor(" + buffer.Trim() + ").base";
			buffer = LineData;
			buffer.Downcase();
			temp1 = buffer.Find(temp6.GetData(), 0);
			if (temp1 > 0)
			{
				buffer = Simcore->GetFloor(temp4)->GetBase();
				LineData = LineData.Slice(0, temp1) + buffer.Trim() + LineData.Slice(temp1 + temp6.Length());
			}
			temp5 = csString(LineData).Downcase().Find("floor(", 0);
		}

		//Extent variables
		LineData.ReplaceAll("%minx%", _gcvt(MinExtent.x, 12, buffer2));
		LineData.ReplaceAll("%minz%", _gcvt(MinExtent.z, 12, buffer2));
		LineData.ReplaceAll("%maxx%", _gcvt(MaxExtent.x, 12, buffer2));
		LineData.ReplaceAll("%maxz%", _gcvt(MaxExtent.z, 12, buffer2));

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
	bool streamnotfinished = true;
	char buffer[1000];
	int location = insert_line;
	csString Filename = filename;

	Filename.Insert(0, "/root/");

	//if insert location is greater than array size, return with error
	if (insert == true)
		if (location > BuildingData.GetSize() - 1 || location < 0)
			return false;

	//make sure file exists
	if (Simcore->FileExists(Filename) == false)
		return false;

	//load file
	csRef<iFile> file (Simcore->vfs->Open(Filename, VFS_FILE_READ));

	//exit if an error occurred while loading
	if (!file)
		return false;

	csFileReadHelper file_r(file);

	//clear array
	if (insert == false)
		BuildingData.DeleteAll();

	while (streamnotfinished == true)
	{
		//clear buffer
		for (int i = 0; i < 1000; i++)
			buffer[i] = ' ';

		//read each line into the buffer
		streamnotfinished = file_r.GetString(buffer, 1000, true);

		//push buffer onto the tail end of the BuildingData array
		if (insert == false)
			BuildingData.Push(buffer);
		else
		{
			//otherwise insert data into building array
			BuildingData.Insert(location, buffer);
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
		includes.Push(info);
	}
	
	return true;
}

bool ScriptProcessor::LoadFromText(const char *text)
{
	//loads building commands from a string
	csStringArray textarray;
	textarray.SplitString(text, "\n");

	//clear building data
	BuildingData.DeleteAll();

	//feed each line of text into the script array
	for (int i = 0; i < textarray.GetSize(); i++)
	{
		BuildingData.Push(textarray[i]);
	}
	return true;
}

bool ScriptProcessor::IfProc(const char *expression)
{
	//IF statement processor

	int temp1;
	csString tmpcalc = expression;
	csString one;
	csString two;
	int start, end;
	bool check;

	//first remove all whitespace from the string
	tmpcalc.ReplaceAll(" ", "");

	//first check for bad and/or character sets
	if (int(tmpcalc.Find("&&")) >= 0 || int(tmpcalc.Find("||")) >= 0 || int(tmpcalc.Find("==")) >= 0 || int(tmpcalc.Find("!=")) >= 0)
	{
		ScriptError("Syntax error in IF operation: '" + tmpcalc + "' (might be nested)");
		return false;
	}

	//find parenthesis
	do
	{
		start = tmpcalc.Find("(", 0);
		if (start >= 0)
		{
			//find matching parenthesis
			int match = 1;
			int end = -1;
			for (int i = start + 1; i < tmpcalc.Length(); i++)
			{
				if (tmpcalc.GetAt(i) == '(')
					match++;
				if (tmpcalc.GetAt(i) == ')')
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
				csString newdata;
				if (IfProc(tmpcalc.Slice(start + 1, end - start - 1)) == true)
					newdata = "true";
				else
					newdata = "false";
				//construct new string
				one = tmpcalc.Slice(0, start);
				if (end < tmpcalc.Length() - 1)
					two = tmpcalc.Slice(end + 1);
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
		for (int i = 1; i < tmpcalc.Length(); i++)
		{
			if (tmpcalc.GetAt(i) == '=' || tmpcalc.GetAt(i) == '!' || tmpcalc.GetAt(i) == '<' || tmpcalc.GetAt(i) == '>')
			{
				operators++;
			}
			if (tmpcalc.GetAt(i) == '&' || tmpcalc.GetAt(i) == '|')
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
					end = tmpcalc.Length();
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
			csString newdata;
			if (IfProc(tmpcalc.Slice(start, end - start)) == true)
				newdata = "true";
			else
				newdata = "false";
			//construct new string
			one = tmpcalc.Slice(0, start);
			two = tmpcalc.Slice(end);
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
	temp1 = tmpcalc.Find("=", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.Slice(0, temp1);
		two = tmpcalc.Slice(temp1 + 1);
		if (one == two)
			return true;
		else
			return false;
	}
	temp1 = tmpcalc.Find("!", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.Slice(0, temp1);
		two = tmpcalc.Slice(temp1 + 1);
		if (one != two)
			return true;
		else
			return false;
	}
	temp1 = tmpcalc.Find("<", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.Slice(0, temp1);
		two = tmpcalc.Slice(temp1 + 1);
		if (IsNumeric(one.GetData()) == true && IsNumeric(two.GetData()) == true)
		{
			if(atof(one.GetData()) < atof(two.GetData()))
				return true;
			else
				return false;
		}
		else
			return false;
	}
	temp1 = tmpcalc.Find(">", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.Slice(0, temp1);
		two = tmpcalc.Slice(temp1 + 1);
		if (IsNumeric(one.GetData()) == true && IsNumeric(two.GetData()) == true)
		{
			if(atof(one.GetData()) > atof(two.GetData()))
				return true;
			else
				return false;
		}
		else
			return false;
	}
	temp1 = tmpcalc.Find("&", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.Slice(0, temp1);
		two = tmpcalc.Slice(temp1 + 1);
		if (one == "true" && two == "true")
			return true;
		else
			return false;
	}
	temp1 = tmpcalc.Find("|", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.Slice(0, temp1);
		two = tmpcalc.Slice(temp1 + 1);
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

int ScriptProcessor::ScriptError(const char *message)
{
	//first see if the current line is from an included file

	int linenum = line;
	int included_lines = 0;
	bool isinclude = false;
	csString includefile;

	for (int i = 0; i < includes.GetSize(); i++)
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
	csString error;
	if (isinclude == false)
		error = "Script error on line " + csString(_itoa(linenum - included_lines + 1, intbuffer, 10)) + ": " + csString(message) + "\nSection: " + csString(_itoa(Section, intbuffer, 10)) + "\nContext: " + Context + "\nLine Text: " + LineData;
	else
		error = "Script error in included file " + includefile + " on line " + csString(_itoa(linenum, intbuffer, 10)) + ": " + csString(message) + "\nSection: " + csString(_itoa(Section, intbuffer, 10)) + "\nContext: " + Context + "\nLine Text: " + LineData;

	skyscraper->ReportError(error);

	//show error dialog
	wxMessageDialog *dialog = new wxMessageDialog(skyscraper->wxwin->GetWindow(), wxString::FromAscii(error.GetData()), wxString::FromAscii("Skyscraper"), wxOK | wxICON_ERROR);
	dialog->ShowModal();

	delete dialog;
	dialog = 0;
	return sError;
}

int ScriptProcessor::ScriptError()
{
	//return automatic error message from contents of the simulator engine's LastError string

	csString message = Simcore->LastError;
	int loc = message.FindLast(":");

	return ScriptError(message.Slice(loc + 1).Trim());
}

bool ScriptProcessor::ReportMissingFiles()
{
	//report on missing files
	//returns true if any files are missing

	if (nonexistent_files.GetSize() > 0)
	{
		nonexistent_files.Sort();
		for (int i = 0; i < nonexistent_files.GetSize(); i++)
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
		for (int i = 0; i < nonexistent_files.GetSize(); i++)
		{
			message.Append(wxString::FromAscii(nonexistent_files[i]));
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
	if (LineData.Slice(0, 5).CompareNoCase("print") == true && Section != 2 && Section != 4)
	{
		//calculate inline math
		buffer = Calc(LineData.Slice(6));

		//print line
		skyscraper->Report(buffer);
	}

	//AddTriangleWall command
	if (LineData.Slice(0, 15).CompareNoCase("addtrianglewall") == true)
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
		int params = SplitData(LineData, 16);

		if (params < 14 || params > 14)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 13; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}
		if (Section == 2)
		{
			 buffer = Simcore->GetFloor(Current)->GetBase() + atof(tempdata[4]);
			 tempdata.Put(4, buffer);
			 buffer = Simcore->GetFloor(Current)->GetBase() + atof(tempdata[7]);
			 tempdata.Put(7, buffer);
		}
		buffer = tempdata[0];
		buffer.Downcase();
		MeshObject* tmpMesh;

		if (buffer == "floor")
		{
			tmpMesh = Simcore->GetFloor(Current)->Level;
			wall = tmpMesh->CreateWallObject(Simcore->GetFloor(Current)->object, tempdata[1]);
		}
		else if (buffer == "elevator")
		{
			return sNextLine;
			tmpMesh = Simcore->GetElevator(Current)->ElevatorMesh;
			wall = tmpMesh->CreateWallObject(Simcore->GetElevator(Current)->object, tempdata[1]);
		}
		else if (buffer == "external")
		{
			tmpMesh = Simcore->External;
			wall = tmpMesh->CreateWallObject(Simcore->object, tempdata[1]);
		}
		else if (buffer == "landscape")
		{
			tmpMesh = Simcore->Landscape;
			wall = tmpMesh->CreateWallObject(Simcore->object, tempdata[1]);
		}
		else if (buffer == "buildings")
		{
			tmpMesh = Simcore->Buildings;
			wall = tmpMesh->CreateWallObject(Simcore->object, tempdata[1]);
		}
		else
			return ScriptError("Invalid object");

		//store command and line info in object
		StoreCommand(wall);

		//create triangle wall
		Simcore->AddTriangleWall(wall, tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12]), atof(tempdata[13]));
	}

	//AddWall command
	if (LineData.Slice(0, 7).CompareNoCase("addwall") == true && Section != 2 && Section != 4)
	{
		//get data
		int params = SplitData(LineData, 8);

		if (params < 14 || params > 14)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 13; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		//create wall
		StoreCommand(Simcore->AddWall(tempdata[0], tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12]), atof(tempdata[13])));
	}

	//AddFloor
	if (LineData.Slice(0, 9).CompareNoCase("addfloor ") == true && Section != 2 && Section != 4)
	{
		//get data
		int params = SplitData(LineData, 9);

		if (params < 12 || params > 12)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 11; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		//create floor
		StoreCommand(Simcore->AddFloor(tempdata[0], tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11])));
	}

	//AddGround
	if (LineData.Slice(0, 9).CompareNoCase("addground") == true)
	{
		//get data
		int params = SplitData(LineData, 10);

		if (params < 9 || params > 9)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 8; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		//create tiled ground
		StoreCommand(Simcore->AddGround(tempdata[0], tempdata[1], atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atoi(tempdata[7]), atoi(tempdata[8])));
	}

	//Cut command
	if (LineData.Slice(0, 4).CompareNoCase("cut ") == true && Section != 2 && Section != 4)
	{
		//get data
		int params = SplitData(LineData, 4);

		if (params != 9)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 6; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		buffer = tempdata[0];
		buffer.Downcase();
		csArray<WallObject*> *wallarray;

		if (buffer == "external")
			wallarray = &Simcore->External->Walls;
		else if (buffer == "landscape")
			wallarray = &Simcore->Landscape->Walls;
		else if (buffer == "buildings")
			wallarray = &Simcore->Buildings->Walls;
		else
			return ScriptError("Invalid object");

		//perform cut
		for (int i = 0; i < wallarray->GetSize(); i++)
			Simcore->Cut(wallarray->Get(i), csVector3(atof(tempdata[1]), atof(tempdata[2]), atof(tempdata[3])), csVector3(atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6])), csString(tempdata[7]).CompareNoCase("true"), csString(tempdata[8]).CompareNoCase("true"), csVector3(0, 0, 0), csVector3(0, 0, 0));
	}

	//Set command
	if (LineData.Slice(0, 4).CompareNoCase("set ") == true && Section != 2 && Section != 4)
	{
		temp1 = LineData.Find("=", 0);
		temp3 = atoi(LineData.Slice(4, temp1 - 5));

		//get text after equal sign
		temp2 = GetAfterEquals(LineData);

		if (temp3 < 0 || temp3 > UserVariable.GetSize() - 1)
			return ScriptError("Invalid variable number");

		UserVariable[temp3] = Calc(temp2);
		if (Simcore->Verbose == true)
			skyscraper->Report("Variable " + csString(_itoa(temp3, intbuffer, 10)) + " set to " + UserVariable[temp3]);
	}

	//CreateWallBox2 command
	if (LineData.Slice(0, 14).CompareNoCase("createwallbox2") == true)
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
		int params = SplitData(LineData, 15);

		if (params != 15)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 10; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		if (Section == 2)
		{
			buffer = Simcore->GetFloor(Current)->GetBase() + atof(tempdata[8]);
			tempdata.Put(8, buffer);
		}
		buffer = tempdata[0];
		buffer.Downcase();
		MeshObject* tmpMesh;

		if (buffer == "floor")
		{
			tmpMesh = Simcore->GetFloor(Current)->Level;
			wall = tmpMesh->CreateWallObject(Simcore->GetFloor(Current)->object, tempdata[1]);
		}
		else if (buffer == "elevator")
		{
			return sNextLine;
			tmpMesh = Simcore->GetElevator(Current)->ElevatorMesh;
			wall = tmpMesh->CreateWallObject(Simcore->GetElevator(Current)->object, tempdata[1]);
		}
		else if (buffer == "external")
		{
			tmpMesh = Simcore->External;
			wall = tmpMesh->CreateWallObject(Simcore->object, tempdata[1]);
		}
		else if (buffer == "landscape")
		{
			tmpMesh = Simcore->Landscape;
			wall = tmpMesh->CreateWallObject(Simcore->object, tempdata[1]);
		}
		else if (buffer == "buildings")
		{
			tmpMesh = Simcore->Buildings;
			wall = tmpMesh->CreateWallObject(Simcore->object, tempdata[1]);
		}
		else
			return ScriptError("Invalid object");

		StoreCommand(wall);

		Simcore->CreateWallBox2(wall, tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), csString(tempdata[11]).CompareNoCase("true"), csString(tempdata[12]).CompareNoCase("true"), csString(tempdata[13]).CompareNoCase("true"), csString(tempdata[14]).CompareNoCase("true"));
	}

	//CreateWallBox command
	if (LineData.Slice(0, 14).CompareNoCase("createwallbox ") == true)
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
		int params = SplitData(LineData, 14);

		if (params != 15)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 10; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		if (Section == 2)
		{
			buffer = Simcore->GetFloor(Current)->GetBase() + atof(tempdata[8]);
			tempdata.Put(8, buffer);
		}
		buffer = tempdata[0];
		buffer.Downcase();
		MeshObject* tmpMesh;

		if (buffer == "floor")
		{
			tmpMesh = Simcore->GetFloor(Current)->Level;
			wall = tmpMesh->CreateWallObject(Simcore->GetFloor(Current)->object, tempdata[1]);
		}
		else if (buffer == "elevator")
		{
			return sNextLine;
			tmpMesh = Simcore->GetElevator(Current)->ElevatorMesh;
			wall = tmpMesh->CreateWallObject(Simcore->GetElevator(Current)->object, tempdata[1]);
		}
		else if (buffer == "external")
		{
			tmpMesh = Simcore->External;
			wall = tmpMesh->CreateWallObject(Simcore->object, tempdata[1]);
		}
		else if (buffer == "landscape")
		{
			tmpMesh = Simcore->Landscape;
			wall = tmpMesh->CreateWallObject(Simcore->object, tempdata[1]);
		}
		else if (buffer == "buildings")
		{
			tmpMesh = Simcore->Buildings;
			wall = tmpMesh->CreateWallObject(Simcore->object, tempdata[1]);
		}
		else
			return ScriptError("Invalid object");

		StoreCommand(wall);

		Simcore->CreateWallBox(wall, tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), csString(tempdata[11]).CompareNoCase("true"), csString(tempdata[12]).CompareNoCase("true"), csString(tempdata[13]).CompareNoCase("true"), csString(tempdata[14]).CompareNoCase("true"));
	}

	//AddCustomWall command
	if (LineData.Slice(0, 14).CompareNoCase("addcustomwall ") == true)
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
		int params = SplitData(LineData, 14);

		//check numeric values
		for (int i = 3; i < params - 2; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		buffer = tempdata[0];
		buffer.Downcase();
		MeshObject* tmpMesh;

		if (buffer == "floor")
		{
			tmpMesh = Simcore->GetFloor(Current)->Level;
			wall = tmpMesh->CreateWallObject(Simcore->GetFloor(Current)->object, tempdata[1]);
		}
		else if (buffer == "elevator")
		{
			return sNextLine;
			tmpMesh = Simcore->GetElevator(Current)->ElevatorMesh;
			wall = tmpMesh->CreateWallObject(Simcore->GetElevator(Current)->object, tempdata[1]);
		}
		else if (buffer == "external")
		{
			tmpMesh = Simcore->External;
			wall = tmpMesh->CreateWallObject(Simcore->object, tempdata[1]);
		}
		else if (buffer == "landscape")
		{
			tmpMesh = Simcore->Landscape;
			wall = tmpMesh->CreateWallObject(Simcore->object, tempdata[1]);
		}
		else if (buffer == "buildings")
		{
			tmpMesh = Simcore->Buildings;
			wall = tmpMesh->CreateWallObject(Simcore->object, tempdata[1]);
		}
		else
			return ScriptError("Invalid object");

		csPoly3D varray;
		for (temp3 = 3; temp3 < params - 2; temp3 += 3)
			varray.AddVertex(atof(tempdata[temp3]), atof(tempdata[temp3 + 1]), atof(tempdata[temp3 + 2]));

		StoreCommand(wall);

		Simcore->AddCustomWall(wall, tempdata[1], tempdata[2], varray, atof(tempdata[params - 2]), atof(tempdata[params - 1]));
	}

	//AddShaft command
	if (LineData.Slice(0, 9).CompareNoCase("addshaft ") == true)
	{
		//get data
		int params = SplitData(LineData, 9);

		if (params != 6)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 5; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		if (atoi(tempdata[4]) < -Simcore->Basements)
			return ScriptError("Invalid starting floor");
		if (atoi(tempdata[5]) > Simcore->Floors - 1)
			return ScriptError("Invalid ending floor");

		Object *object = Simcore->CreateShaft(atoi(tempdata[0]), atoi(tempdata[1]), atof(tempdata[2]), atof(tempdata[3]), atoi(tempdata[4]), atoi(tempdata[5]));
		if (!object)
			return ScriptError();

		StoreCommand(object);
	}

	//ShaftCut command
	if (LineData.Slice(0, 9).CompareNoCase("shaftcut ") == true)
	{
		//get data
		int params = SplitData(LineData, 9);

		if (params != 7)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 6; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		//check for existence of shaft
		int shaftnum = atoi(tempdata[0]);
		if (shaftnum < 1 || shaftnum > Simcore->Shafts())
			return ScriptError("Invalid shaft " + csString(tempdata[0]));

		Simcore->GetShaft(shaftnum)->CutFloors(true, csVector2(atof(tempdata[1]), atof(tempdata[2])), csVector2(atof(tempdata[3]), atof(tempdata[4])), atof(tempdata[5]), atof(tempdata[6]));
	}

	//ShaftShowFloors command
	if (LineData.Slice(0, 15).CompareNoCase("shaftshowfloors") == true)
	{
		//get shaft number
		int loc = LineData.Find("=");
		if (loc < 0)
			return ScriptError("Syntax error");
		int shaftnum;
		if (!IsNumeric(LineData.Slice(15, loc - 16).Trim().GetData(), shaftnum))
			return ScriptError("Invalid shaft number");

		if (shaftnum < 1 || shaftnum > Simcore->Shafts())
			return ScriptError("Invalid shaft number");

		Simcore->GetShaft(shaftnum)->ShowFloors = true;

		int params = SplitAfterEquals(LineData, false);
		if (params == -1)
			return ScriptError("Syntax Error");

		for (int line = 0; line < params; line++)
		{
			csString tmpstring = tempdata[line];
			tmpstring.Trim();
			if (tmpstring.Find("-", 1) > 0)
			{
				int start, end;
				//found a range marker
				if (!IsNumeric(tmpstring.Slice(0, tmpstring.Find("-", 1)).Trim().GetData(), start) || !IsNumeric(tmpstring.Slice(tmpstring.Find("-", 1) + 1).Trim().GetData(), end))
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
				if (!IsNumeric(csString(tempdata[line]).Trim().GetData(), showfloor))
					return ScriptError("Invalid value");
				Simcore->GetShaft(shaftnum)->AddShowFloor(showfloor);
			}
		}
	}

	//ShaftShowOutside command
	if (LineData.Slice(0, 16).CompareNoCase("shaftshowoutside") == true)
	{
		//get shaft number
		int loc = LineData.Find("=");
		if (loc < 0)
			return ScriptError("Syntax error");
		int shaftnum;
		if (!IsNumeric(LineData.Slice(16, loc - 17).Trim().GetData(), shaftnum))
			return ScriptError("Invalid shaft number");
		if (shaftnum < 1 || shaftnum > Simcore->Shafts())
			return ScriptError("Invalid shaft number");
		Simcore->GetShaft(shaftnum)->ShowOutside = true;

		int params = SplitAfterEquals(LineData, false);
		if (params == -1)
			return ScriptError("Syntax Error");

		for (int line = 0; line < params; line++)
		{
			csString tmpstring = tempdata[line];
			tmpstring.Trim();
			if (tmpstring.Find("-", 1) > 0)
			{
				int start, end;
				//found a range marker
				if (!IsNumeric(tmpstring.Slice(0, tmpstring.Find("-", 1)).Trim().GetData(), start) || !IsNumeric(tmpstring.Slice(tmpstring.Find("-", 1) + 1).Trim().GetData(), end))
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
				if (!IsNumeric(csString(tempdata[line]).Trim().GetData(), showfloor))
					return ScriptError("Invalid value");
				Simcore->GetShaft(shaftnum)->AddShowOutside(showfloor);
			}
		}
	}

	//ShowFullShaft command
	if (LineData.Slice(0, 13).CompareNoCase("showfullshaft") == true)
	{
		//get shaft number
		int loc = LineData.Find("=");
		if (loc < 0)
			return ScriptError("Syntax error");
		int shaftnum;
		if (!IsNumeric(LineData.Slice(13, loc - 14).Trim().GetData(), shaftnum))
			return ScriptError("Invalid shaft number");
		if (shaftnum < 1 || shaftnum > Simcore->Shafts())
			return ScriptError("Invalid shaft number");

		//get text after equal sign
		temp2 = GetAfterEquals(LineData);

		Simcore->GetShaft(shaftnum)->ShowFullShaft = csString(temp2).CompareNoCase("true");
	}

	//CreateStairwell command
	if (LineData.Slice(0, 15).CompareNoCase("createstairwell") == true)
	{
		//get data
		int params = SplitData(LineData, 16);

		if (params != 5)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 4; i++)
		{
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		Object *object = Simcore->CreateStairwell(atoi(tempdata[0]), atof(tempdata[1]), atof(tempdata[2]), atoi(tempdata[3]), atoi(tempdata[4]));
		if (!object)
			return ScriptError();

		StoreCommand(object);
	}

	//CutStairwell command
	if (LineData.Slice(0, 13).CompareNoCase("cutstairwell ") == true)
	{
		//get data
		int params = SplitData(LineData, 13);

		if (params != 7)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 6; i++)
		{
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		int stairwell = atoi(tempdata[0]);
		if (!Simcore->GetStairs(stairwell))
			return ScriptError("Invalid stairwell");

		Simcore->GetStairs(stairwell)->CutFloors(true, csVector2(atof(tempdata[1]), atof(tempdata[2])), csVector2(atof(tempdata[3]), atof(tempdata[4])), atof(tempdata[5]), atof(tempdata[6]));
	}

	//WallOrientation command
	if (LineData.Slice(0, 15).CompareNoCase("wallorientation") == true)
	{
		//get text after equal sign
		temp2 = GetAfterEquals(LineData);

		if (!Simcore->SetWallOrientation(temp2.GetData()))
			return ScriptError();
	}

	//FloorOrientation command
	if (LineData.Slice(0, 16).CompareNoCase("floororientation") == true)
	{
		//get text after equal sign
		temp2 = GetAfterEquals(LineData);

		if (!Simcore->SetFloorOrientation(temp2.GetData()))
			return ScriptError();
	}

	//DrawWalls command
	if (LineData.Slice(0, 9).CompareNoCase("drawwalls") == true)
	{
		int params = SplitAfterEquals(LineData);
		if (params == -1)
			return ScriptError("Syntax Error");
		if (params != 6)
			return ScriptError("Incorrect number of parameters");

		Simcore->DrawWalls(csString(tempdata[0]).CompareNoCase("true"),
					csString(tempdata[1]).CompareNoCase("true"),
					csString(tempdata[2]).CompareNoCase("true"),
					csString(tempdata[3]).CompareNoCase("true"),
					csString(tempdata[4]).CompareNoCase("true"),
					csString(tempdata[5]).CompareNoCase("true"));
	}

	//SetTextureMapping command
	if (LineData.Slice(0, 18).CompareNoCase("settexturemapping ") == true)
	{
		//get data
		int params = SplitData(LineData, 18);

		if (params != 9)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 8; i++)
		{
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		Simcore->SetTextureMapping(atoi(tempdata[0]), csVector2(atof(tempdata[1]), atof(tempdata[2])),
									atoi(tempdata[3]), csVector2(atof(tempdata[4]), atof(tempdata[5])),
									atoi(tempdata[6]), csVector2(atof(tempdata[7]), atof(tempdata[8])));
	}

	//SetTextureMapping2 command
	if (LineData.Slice(0, 18).CompareNoCase("settexturemapping2") == true)
	{
		//get data
		int params = SplitData(LineData, 19);

		if (params != 15)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 14; i++)
		{
			if (i == 5)
				i = 8;
			if (i == 10)
				i = 13;
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		Simcore->SetTextureMapping2(tempdata[0], tempdata[1], tempdata[2], csVector2(atof(tempdata[3]), atof(tempdata[4])),
									tempdata[5], tempdata[6], tempdata[7], csVector2(atof(tempdata[8]), atof(tempdata[9])),
									tempdata[10], tempdata[11], tempdata[12], csVector2(atof(tempdata[13]), atof(tempdata[14])));
	}

	//ResetTextureMapping command
	if (LineData.Slice(0, 19).CompareNoCase("resettexturemapping") == true)
	{
		int temp2check = LineData.Find("=", 0);
		if (temp2check < 0)
			return ScriptError("Syntax Error");

		//get text after equal sign
		temp2 = GetAfterEquals(LineData);

		Simcore->ResetTextureMapping(temp2.CompareNoCase("true"));
	}

	//SetPlanarMapping command
	if (LineData.Slice(0, 16).CompareNoCase("setplanarmapping") == true)
	{
		//get data
		int params = SplitData(LineData, 17);

		if (params != 4)
			return ScriptError("Incorrect number of parameters");

		Simcore->SetPlanarMapping(csString(tempdata[0]).CompareNoCase("true"),
					csString(tempdata[1]).CompareNoCase("true"),
					csString(tempdata[2]).CompareNoCase("true"),
					csString(tempdata[3]).CompareNoCase("true"));
	}

	//ReverseAxis command
	if (LineData.Slice(0, 11).CompareNoCase("reverseaxis") == true)
	{
		int temp2check = LineData.Find("=", 0);
		if (temp2check < 0)
			return ScriptError("Syntax Error");
		temp2 = GetAfterEquals(LineData);

		Simcore->ReverseAxis(temp2.CompareNoCase("true"));
	}

	//Intersection points
	temp5 = csString(LineData).Downcase().Find("isect(", 0);
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

		temp1 = LineData.Find("(", 0);
		temp4 = LineData.Find(")", 0);
		if (temp1 < 0 || temp4 < 0)
			return ScriptError("Syntax error");
		tempdata.DeleteAll();
		tempdata.SplitString(LineData.Slice(temp1 + 1, temp4 - temp1 - 1).GetData(), ",");
		for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
		{
			buffer = Calc(tempdata[temp3]);
			tempdata.Put(temp3, buffer);
		}
		if (tempdata.GetSize() < 8 || tempdata.GetSize() > 8)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 7; i++)
		{
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		buffer = tempdata[0];
		buffer.Downcase();
		csArray<WallObject*> *wall_array;
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

		csVector3 isect = Simcore->GetPoint(*wall_array, tempdata[1], csVector3(atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4])), csVector3(atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7])));

		buffer = csString(LineData).Slice(0, temp5) + csString(wxVariant(isect.x).GetString().ToAscii()) + csString(", ") + csString(wxVariant(isect.y).GetString().ToAscii()) + csString(", ") + csString(wxVariant(isect.z).GetString().ToAscii()) + csString(LineData).Slice(temp4 + 1);
		LineData = buffer.GetData();
	}

	//GetWallExtents command
	if (LineData.Slice(0, 14).CompareNoCase("getwallextents") == true)
	{
		//get data
		int params = SplitData(LineData, 15);

		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		if (!IsNumeric(csString(tempdata[2]).Trim().GetData()))
			return ScriptError("Invalid value: " + csString(tempdata[2]));

		buffer = tempdata[0];
		buffer.Downcase();
		csArray<WallObject*> *wall_array;
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

		MinExtent = Simcore->GetWallExtents(*wall_array, tempdata[1], atof(tempdata[2]), false);
		MaxExtent = Simcore->GetWallExtents(*wall_array, tempdata[1], atof(tempdata[2]), true);
	}

	//GetWallExtents function
	temp5 = csString(LineData).Downcase().Find("getwallextents(", 0);
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

		temp1 = LineData.Find("(", 0);
		temp4 = LineData.Find(")", 0);
		if (temp1 < 0 || temp4 < 0)
			return ScriptError("Syntax error");
		tempdata.DeleteAll();
		tempdata.SplitString(LineData.Slice(temp1 + 1, temp4 - temp1 - 1).GetData(), ",");
		for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
		{
			buffer = Calc(tempdata[temp3]);
			tempdata.Put(temp3, buffer);
		}
		if (tempdata.GetSize() < 4 || tempdata.GetSize() > 4)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		if (!IsNumeric(csString(tempdata[2]).Trim().GetData()))
			return ScriptError("Invalid value: " + csString(tempdata[2]));

		buffer = tempdata[0];
		buffer.Downcase();
		csArray<WallObject*> *wall_array;
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

		csVector3 result = Simcore->GetWallExtents(*wall_array, tempdata[1], atof(tempdata[2]), csString(tempdata[3]).CompareNoCase("true"));

		buffer = csString(LineData).Slice(0, temp5) + csString(wxVariant(result.x).GetString().ToAscii()) + csString(", ") + csString(wxVariant(result.y).GetString().ToAscii()) + csString(", ") + csString(wxVariant(result.z).GetString().ToAscii()) + csString(LineData).Slice(temp4 + 1);
		LineData = buffer.GetData();
	}

	//SetAutoSize command
	if (LineData.Slice(0, 11).CompareNoCase("setautosize") == true)
	{
		int params = SplitAfterEquals(LineData);
		if (params == -1)
			return ScriptError("Syntax Error");
		if (params != 2)
			return ScriptError("Incorrect number of parameters");

		Simcore->SetAutoSize(csString(tempdata[0]).CompareNoCase("true"),
					csString(tempdata[1]).CompareNoCase("true"));
	}

	//TextureOverride command
	if (LineData.Slice(0, 15).CompareNoCase("textureoverride") == true)
	{
		int params = SplitData(LineData, 16, false);

		if (params != 6)
			return ScriptError("Incorrect number of parameters");

		Simcore->SetTextureOverride(tempdata[0], tempdata[1], tempdata[2], tempdata[3], tempdata[4], tempdata[5]);
		return sNextLine;
	}

	//TextureFlip command
	if (LineData.Slice(0, 11).CompareNoCase("textureflip") == true)
	{
		int params = SplitData(LineData, 12, false);

		if (params != 6)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 5; i++)
		{
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		Simcore->SetTextureFlip(atoi(tempdata[0]), atoi(tempdata[1]), atoi(tempdata[2]), atoi(tempdata[3]), atoi(tempdata[4]), atoi(tempdata[5]));
		return sNextLine;
	}

	//Mount command
	if (LineData.Slice(0, 5).CompareNoCase("mount") == true)
	{
		//get data
		int params = SplitData(LineData, 6, false);

		if (params != 2)
			return ScriptError("Incorrect number of parameters");

		buffer = tempdata[1];
		buffer.Insert(0, "/root/");
		if (!Simcore->Mount(tempdata[0], buffer))
			return ScriptError();

		return sNextLine;
	}

	//AddFloorAutoArea command
	if (LineData.Slice(0, 16).CompareNoCase("addfloorautoarea") == true)
	{
		//get data
		int params = SplitData(LineData, 17);

		if (params != 6)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 5; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		//create floor auto area
		Simcore->AddFloorAutoArea(csVector3(atof(tempdata[0]), atof(tempdata[1]), atof(tempdata[2])), csVector3(atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5])));
	}

	//AddSound
	if (LineData.Slice(0, 8).CompareNoCase("addsound") == true)
	{
		//get data
		int params = SplitData(LineData, 9);

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
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
		}
		else
		{
			for (int i = 2; i <= 12; i++)
			{
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
		}

		//check to see if file exists
		CheckFile(csString("data/") + tempdata[1], true);

		if (partial == true)
			StoreCommand(Simcore->AddSound(tempdata[0], tempdata[1], csVector3(atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]))));
		else
			StoreCommand(Simcore->AddSound(tempdata[0], tempdata[1], csVector3(atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4])), atoi(tempdata[5]), atoi(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), csVector3(atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12])))); 
	}

	//AddModel command
	if (LineData.Slice(0, 8).CompareNoCase("addmodel") == true)
	{
		if (Section == 2)
			return 0;

		//get data
		int params = SplitData(LineData, 9);

		if (params != 10)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 9; i++)
		{
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		//check to see if file exists
		CheckFile(csString("data/") + tempdata[1], true);

		//create model
		StoreCommand(Simcore->AddModel(tempdata[0], tempdata[1], csVector3(atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4])), csVector3(atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7])), atof(tempdata[8]), atof(tempdata[9])));
	}

	return 0;
}

int ScriptProcessor::ProcGlobals()
{
	//process global parameters

	//get text after equal sign
	temp2 = GetAfterEquals(LineData);

	//store variable values
	if (LineData.Slice(0, 4).CompareNoCase("name") == true)
		Simcore->BuildingName = temp2;
	if (LineData.Slice(0, 8).CompareNoCase("designer") == true)
		Simcore->BuildingDesigner = temp2;
	if (LineData.Slice(0, 8).CompareNoCase("location") == true)
		Simcore->BuildingLocation = temp2;
	if (LineData.Slice(0, 11).CompareNoCase("description") == true)
		Simcore->BuildingDescription = temp2;
	if (LineData.Slice(0, 7).CompareNoCase("version") == true)
		Simcore->BuildingVersion = temp2;
	if (LineData.Slice(0, 3).CompareNoCase("sky") == true)
		Simcore->SkyName = temp2;
	if (LineData.Slice(0, 11).CompareNoCase("camerafloor") == true)
	{
		int data;
		if (!IsNumeric(temp2.GetData(), data))
			return ScriptError("Invalid floor");

		Simcore->camera->StartFloor = data;
	}
	if (LineData.Slice(0, 14).CompareNoCase("cameraposition") == true)
	{
		float x, z;
		if (!IsNumeric(temp2.Slice(0, temp2.Find(",", 0)).Trim().GetData(), x) || !IsNumeric(temp2.Slice(temp2.Find(",", 0) + 1).Trim().GetData(), z))
			return ScriptError("Invalid position");

		Simcore->camera->StartPositionX  = x;
		Simcore->camera->StartPositionZ  = z;
	}
	if (LineData.Slice(0, 15).CompareNoCase("cameradirection") == true)
	{
		temp3 = temp2.Find(",", 0);
		temp4 = temp2.Find(",", temp3 + 1);
		float x, y, z;
		if (!IsNumeric(temp2.Slice(0, temp3).Trim().GetData(), x) || !IsNumeric(temp2.Slice(temp3 + 1, temp4 - temp3 - 1).Trim().GetData(), y) || !IsNumeric(temp2.Slice(temp4 + 1).Trim().GetData(), z))
			return ScriptError("Invalid direction");

		Simcore->camera->SetStartDirection(csVector3(x, y, z));
	}
	if (LineData.Slice(0, 14).CompareNoCase("camerarotation") == true)
	{
		temp3 = temp2.Find(",", 0);
		temp4 = temp2.Find(",", temp3 + 1);
		float x, y, z;
		if (!IsNumeric(temp2.Slice(0, temp3).Trim().GetData(), x) || !IsNumeric(temp2.Slice(temp3 + 1, temp4 - temp3 - 1).Trim().GetData(), y) || !IsNumeric(temp2.Slice(temp4 + 1).Trim().GetData(), z))
			return ScriptError("Invalid direction");

		Simcore->camera->SetStartRotation(csVector3(x, y, z));
	}
	if (LineData.Slice(0, 15).CompareNoCase("interfloorontop") == true)
	{
		Simcore->InterfloorOnTop = csString(temp2).CompareNoCase("true");
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
		csString floornum;
		floornum = Current;
		return ScriptError("Invalid floor " + floornum);
	}

	//replace variables with actual values
	buffer = Current;
	LineData.ReplaceAll("%floor%", buffer);
	buffer = floor->Height;
	LineData.ReplaceAll("%height%", buffer);
	buffer = floor->FullHeight();
	LineData.ReplaceAll("%fullheight%", buffer);
	buffer = floor->InterfloorHeight;
	LineData.ReplaceAll("%interfloorheight%", buffer);
	buffer = floor->GetBase();
	LineData.ReplaceAll("%base%", buffer);

	if (getfloordata == true)
		return sCheckFloors;

	//IF statement
	if (LineData.Slice(0, 2).CompareNoCase("if") == true)
	{
		temp1 = LineData.Find("[", 0);
		temp3 = LineData.Find("]", 0);
		if (temp1 + temp3 > 0)
			temp2 = LineData.Slice(temp1 + 1, temp3 - temp1 - 1);
		else
			temp2 = "";
		temp2.Trim();
		if (IfProc(temp2) == true)
		{
			LineData = LineData.Slice(temp3 + 1).Trim(); //trim off IF statement
			return sCheckFloors;
		}
		else
			return sNextLine; //skip line
	}

	//process functions
	if (FunctionProc() == true)
		return sNextLine;

	//get text after equal sign
	int temp2check = LineData.Find("=", 0);
	temp2 = GetAfterEquals(LineData);

	//parameters
	if (LineData.Slice(0, 6).CompareNoCase("height") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (!IsNumeric(Calc(temp2.GetData()).Trim().GetData(), floor->Height))
			return ScriptError("Invalid value");
		if (FloorCheck < 2)
			FloorCheck = 1;
		else
			FloorCheck = 3;
	}
	if (LineData.Slice(0, 16).CompareNoCase("interfloorheight") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (!IsNumeric(Calc(temp2.GetData()).Trim().GetData(), floor->InterfloorHeight))
			return ScriptError("Invalid value");
		if (FloorCheck == 0)
			FloorCheck = 2;
		else
			FloorCheck = 3;
	}
	if (LineData.Slice(0, 8).CompareNoCase("altitude") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (!IsNumeric(Calc(temp2.GetData()).Trim().GetData(), floor->Altitude))
			return ScriptError("Invalid value");
	}
	if (LineData.Slice(0, 2).CompareNoCase("id") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		floor->ID = Calc(temp2);
	}
	if (LineData.Slice(0, 4).CompareNoCase("name") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		floor->Name = Calc(temp2);
	}
	if (LineData.Slice(0, 4).CompareNoCase("type") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		floor->FloorType = temp2;
	}
	if (LineData.Slice(0, 11).CompareNoCase("description") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		floor->Description = temp2;
	}
	if (LineData.Slice(0, 16).CompareNoCase("indicatortexture") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		floor->IndicatorTexture = Calc(temp2);
	}
	if (LineData.Slice(0, 5).CompareNoCase("group") == true)
	{
		//copy string listing of group floors into array

		int params = SplitAfterEquals(LineData, false);
		if (params == -1)
			return ScriptError("Syntax Error");

		for (int line = 0; line < params; line++)
		{
			csString tmpstring = tempdata[line];
			tmpstring.Trim();
			if (tmpstring.Find("-", 1) > 0)
			{
				int start, end;
				//found a range marker
				if (!IsNumeric(tmpstring.Slice(0, tmpstring.Find("-", 1)).Trim().GetData(), start) || !IsNumeric(tmpstring.Slice(tmpstring.Find("-", 1) + 1).Trim().GetData(), end))
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
				if (!IsNumeric(csString(tempdata[line]).Trim().GetData(), data))
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
	if (LineData.Slice(0, 5).CompareNoCase("print") == true)
	{
		//calculate inline math
		buffer = Calc(LineData.Slice(6));

		//print line
		skyscraper->Report(buffer);
	}

	//Exit command
	if (LineData.Slice(0, 4).CompareNoCase("exit") == true)
	{
		if (RangeL != RangeH)
			LineData = "<endfloors>";
		else
			LineData = "<endfloor>";
	}

	//Break command
	if (LineData.Slice(0, 7).CompareNoCase("<break>") == true)
		return sBreak;

	//AddFloor command
	if (LineData.Slice(0, 9).CompareNoCase("addfloor ") == true)
	{
		//get data
		int params = SplitData(LineData, 9);

		if (params != 12)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 10; i++)
		{
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		//create floor
		StoreCommand(floor->AddFloor(tempdata[0], tempdata[1], atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), csString(tempdata[11]).CompareNoCase("true")));
	}

	//AddShaftFloor command
	if (LineData.Slice(0, 13).CompareNoCase("addshaftfloor") == true)
	{
		//get data
		int params = SplitData(LineData, 14);

		if (params != 12)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 11; i++)
		{
			if (i == 1)
				i = 3; //skip non-numeric parameters
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		//create floor
		if (Simcore->GetShaft(atoi(tempdata[0])))
			StoreCommand(Simcore->GetShaft(atoi(tempdata[0]))->AddFloor(Current, tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11])));
		else
			return ScriptError("Invalid shaft");
	}

	//AddStairsFloor command
	if (LineData.Slice(0, 14).CompareNoCase("addstairsfloor") == true)
	{
		//get data
		int params = SplitData(LineData, 15);

		if (params != 12)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 11; i++)
		{
			if (i == 1)
				i = 3; //skip non-numeric parameters
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		//create floor
		if (Simcore->GetStairs(atoi(tempdata[0])))
			StoreCommand(Simcore->GetStairs(atoi(tempdata[0]))->AddFloor(Current, tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11])));
		else
			return ScriptError("Invalid stairwell");
	}

	//AddInterFloorFloor command
	if (LineData.Slice(0, 18).CompareNoCase("addinterfloorfloor") == true)
	{
		//get data
		int params = SplitData(LineData, 19);

		if (params != 11)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 10; i++)
		{
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		//create floor
		StoreCommand(floor->AddInterfloorFloor(tempdata[0], tempdata[1], atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10])));
	}

	//AddWall command
	if (LineData.Slice(0, 7).CompareNoCase("addwall") == true)
	{
		//get data
		int params = SplitData(LineData, 8);

		if (params != 14)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 12; i++)
		{
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		//create wall
		StoreCommand(floor->AddWall(tempdata[0], tempdata[1], atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12]), csString(tempdata[13]).CompareNoCase("true")));
	}

	//AddShaftWall command
	if (LineData.Slice(0, 12).CompareNoCase("addshaftwall") == true)
	{
		//get data
		int params = SplitData(LineData, 13);

		if (params != 14)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 13; i++)
		{
			if (i == 1)
				i = 3; //skip non-numeric parameters
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		//create wall
		if (Simcore->GetShaft(atoi(tempdata[0])))
			StoreCommand(Simcore->GetShaft(atoi(tempdata[0]))->AddWall(Current, tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12]), atof(tempdata[13])));
		else
			return ScriptError("Invalid shaft");
	}

	//AddStairsWall command
	if (LineData.Slice(0, 13).CompareNoCase("addstairswall") == true)
	{
		//get data
		int params = SplitData(LineData, 14);

		if (params != 14)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 13; i++)
		{
			if (i == 1)
				i = 3; //skip non-numeric parameters
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		//create wall
		if (Simcore->GetStairs(atoi(tempdata[0])))
			StoreCommand(Simcore->GetStairs(atoi(tempdata[0]))->AddWall(Current, tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12]), atof(tempdata[13])));
		else
			return ScriptError("Invalid stairwell");
	}

	//AddInterFloorWall command
	if (LineData.Slice(0, 17).CompareNoCase("addinterfloorwall") == true)
	{
		//get data
		int params = SplitData(LineData, 18);

		if (params != 13)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 12; i++)
		{
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		//create wall
		StoreCommand(floor->AddInterfloorWall(tempdata[0], tempdata[1], atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12])));
	}

	//ColumnWallBox command
	if (LineData.Slice(0, 14).CompareNoCase("columnwallbox ") == true)
	{
		//get data
		int params = SplitData(LineData, 14);

		if (params != 14)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 9; i++)
		{
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		StoreCommand(floor->ColumnWallBox(tempdata[0], tempdata[1], atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), csString(tempdata[10]).CompareNoCase("true"), csString(tempdata[11]).CompareNoCase("true"), csString(tempdata[12]).CompareNoCase("true"), csString(tempdata[13]).CompareNoCase("true")));
	}

	//ColumnWallBox2 command
	if (LineData.Slice(0, 14).CompareNoCase("columnwallbox2") == true)
	{
		//get data
		int params = SplitData(LineData, 15);

		if (params != 14)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 9; i++)
		{
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		StoreCommand(floor->ColumnWallBox2(tempdata[0], tempdata[1], atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), csString(tempdata[10]).CompareNoCase("true"), csString(tempdata[11]).CompareNoCase("true"), csString(tempdata[12]).CompareNoCase("true"), csString(tempdata[13]).CompareNoCase("true")));
	}

	//Set command
	if (LineData.Slice(0, 4).CompareNoCase("set ") == true)
	{
		temp1 = LineData.Find("=", 0);
		if (temp1 < 0)
			return ScriptError("Syntax Error");
		if (!IsNumeric(LineData.Slice(4, temp1 - 5).Trim().GetData(), temp3))
			return ScriptError("Invalid variable number");

		//get text after equal sign
		temp2 = GetAfterEquals(LineData);

		if (temp3 < 0 || temp3 > UserVariable.GetSize() - 1)
			return ScriptError("Invalid variable number");
		UserVariable[temp3] = Calc(temp2);
		if (Simcore->Verbose == true)
			skyscraper->Report("Variable " + csString(_itoa(temp3, intbuffer, 10)) + " set to " + UserVariable[temp3]);
	}

	//CallButtonElevators command
	if (LineData.Slice(0, 19).CompareNoCase("callbuttonelevators") == true)
	{
		//construct array containing floor numbers
		int params = SplitAfterEquals(LineData, false);
		if (params == -1)
			return ScriptError("Syntax Error");

		callbutton_elevators.DeleteAll();
		callbutton_elevators.SetSize(params);

		for (int line = 0; line < params; line++)
		{
			int elevnumber;
			if (!IsNumeric(csString(tempdata[line]).Trim().GetData(), elevnumber))
				return ScriptError("Invalid elevator number");
			if (elevnumber < 1 || elevnumber > Simcore->Elevators())
				return ScriptError("Invalid elevator number");
			callbutton_elevators[line] = elevnumber;
		}
	}

	//CreateCallButtons command
	if (LineData.Slice(0, 17).CompareNoCase("createcallbuttons") == true)
	{
		if (callbutton_elevators.GetSize() == 0)
			return ScriptError("No elevators specified");

		//get data
		int params = SplitData(LineData, 18);

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
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
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
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
		}

		//create call button
		if (compatibility == true)
			StoreCommand(floor->AddCallButtons(callbutton_elevators, tempdata[0], tempdata[1], tempdata[1], tempdata[2], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), tempdata[6], atof(tempdata[7]), atof(tempdata[8]), csString(tempdata[9]).CompareNoCase("true"), atof(tempdata[10]), atof(tempdata[11])));
		else
			StoreCommand(floor->AddCallButtons(callbutton_elevators, tempdata[0], tempdata[1], tempdata[2], tempdata[3], tempdata[4], atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), tempdata[8], atof(tempdata[9]), atof(tempdata[10]), csString(tempdata[11]).CompareNoCase("true"), atof(tempdata[12]), atof(tempdata[13])));
	}

	//AddStairs command
	if (LineData.Slice(0, 10).CompareNoCase("addstairs ") == true)
	{
		//get data
		int params = SplitData(LineData, 10);

		if (params != 13)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 12; i++)
		{
			if (i == 1)
				i = 4; //skip non-numeric parameters
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		//create stairs
		if (Simcore->GetStairs(atoi(tempdata[0])))
			StoreCommand(Simcore->GetStairs(atoi(tempdata[0]))->AddStairs(Current, tempdata[1], tempdata[2], tempdata[3], atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atoi(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12])));
		else
			return ScriptError("Invalid stairwell");
	}

	//AddDoor command
	if (LineData.Slice(0, 8).CompareNoCase("adddoor ") == true)
	{
		//get data
		int params = SplitData(LineData, 8);

		if (params < 10 || params > 14)
			return ScriptError("Incorrect number of parameters");

		int compat = 0;

		//check numeric values
		if (params == 10)
		{
			for (int i = 1; i <= 9; i++)
			{
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
			compat = 1;
		}
		if (params == 12)
		{
			for (int i = 3; i <= 11; i++)
			{
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
			compat = 2;
		}
		if (params == 13)
		{
			for (int i = 3; i <= 12; i++)
			{
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
			compat = 3;
		}
		if (params == 14)
		{
			for (int i = 4; i <= 13; i++)
			{
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
		}

		//check to see if file exists
		if (compat != 1)
		{
			CheckFile(csString("data/") + tempdata[0], true);
			CheckFile(csString("data/") + tempdata[1], true);
		}

		//create door
		if (compat == 1)
			StoreCommand(floor->AddDoor("", "", false, tempdata[0], atof(tempdata[1]), atoi(tempdata[2]), 0, atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9])));
		if (compat == 2)
			StoreCommand(floor->AddDoor(tempdata[0], tempdata[1], false, tempdata[2], atof(tempdata[3]), atoi(tempdata[4]), 0, atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11])));
		if (compat == 3)
			StoreCommand(floor->AddDoor(tempdata[0], tempdata[1], false, tempdata[2], atof(tempdata[3]), atoi(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12])));
		if (compat == 0)
			StoreCommand(floor->AddDoor(tempdata[0], tempdata[1], csString(tempdata[2]).CompareNoCase("true"), tempdata[3], atof(tempdata[4]), atoi(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12]), atof(tempdata[13])));
	}

	//AddStairsDoor command
	if (LineData.Slice(0, 14).CompareNoCase("addstairsdoor ") == true)
	{
		//get data
		int params = SplitData(LineData, 14);

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
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
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
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
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
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
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
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
		}

		//check to see if file exists
		if (compat != 1)
		{
			CheckFile(csString("data/") + tempdata[1], true);
			CheckFile(csString("data/") + tempdata[2], true);
		}

		//create door
		if (Simcore->GetStairs(atoi(tempdata[0])))
		{
			if (compat == 1)
				StoreCommand(Simcore->GetStairs(atoi(tempdata[0]))->AddDoor(Current, "", "", false, tempdata[1], atof(tempdata[2]), atoi(tempdata[3]), 0, atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10])));
			if (compat == 2)
				StoreCommand(Simcore->GetStairs(atoi(tempdata[0]))->AddDoor(Current, tempdata[1], tempdata[2], false, tempdata[3], atof(tempdata[4]), atoi(tempdata[5]), 0, atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12])));
			if (compat == 3)
				StoreCommand(Simcore->GetStairs(atoi(tempdata[0]))->AddDoor(Current, tempdata[1], tempdata[2], false, tempdata[3], atof(tempdata[4]), atoi(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12]), atof(tempdata[13])));
			if (compat == 0)
				StoreCommand(Simcore->GetStairs(atoi(tempdata[0]))->AddDoor(Current, tempdata[1], tempdata[2], csString(tempdata[3]).CompareNoCase("true"), tempdata[4], atof(tempdata[5]), atoi(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12]), atof(tempdata[13]), atof(tempdata[14])));
		}
		else
			return ScriptError("Invalid stairwell");
	}

	//AddDirectionalIndicator command
	if (LineData.Slice(0, 23).CompareNoCase("adddirectionalindicator") == true)
	{
		//get data
		int params = SplitData(LineData, 24);

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
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
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
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
		}

		if (!Simcore->GetElevator(atoi(tempdata[0])))
			return ScriptError("Invalid elevator");

		if (compatibility == true)
			StoreCommand(floor->AddDirectionalIndicator(atoi(tempdata[0]), csString(tempdata[1]).CompareNoCase("true"), false, csString(tempdata[2]).CompareNoCase("true"), csString(tempdata[3]).CompareNoCase("true"), tempdata[4], tempdata[5], tempdata[6], tempdata[7], tempdata[8], atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), tempdata[12], atof(tempdata[13]), atof(tempdata[14]), csString(tempdata[15]).CompareNoCase("true"), atof(tempdata[16]), atof(tempdata[17])));
		else
			StoreCommand(floor->AddDirectionalIndicator(atoi(tempdata[0]), csString(tempdata[1]).CompareNoCase("true"), csString(tempdata[2]).CompareNoCase("true"), csString(tempdata[3]).CompareNoCase("true"), csString(tempdata[4]).CompareNoCase("true"), tempdata[5], tempdata[6], tempdata[7], tempdata[8], tempdata[9], atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12]), tempdata[13], atof(tempdata[14]), atof(tempdata[15]), csString(tempdata[16]).CompareNoCase("true"), atof(tempdata[17]), atof(tempdata[18])));
	}

	//AddShaftDoor command
	if (LineData.Slice(0, 13).CompareNoCase("addshaftdoor ") == true)
	{
		//exit if the SetShaftDoors command was never used
		if (setshaftdoors == false)
			return ScriptError("SetShaftDoors must be used before AddShaftDoor");

		//get data
		int params = SplitData(LineData, 13);

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
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
		}
		else
		{
			for (int i = 0; i <= 4; i++)
			{
				if (i == 2)
					i = 3;
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
		}

		if (!Simcore->GetElevator(atoi(tempdata[0])))
			return ScriptError("Invalid elevator");

		if (compat == false)
			StoreCommand(Simcore->GetElevator(atoi(tempdata[0]))->AddShaftDoor(Current, atoi(tempdata[1]), tempdata[2], tempdata[3], atof(tempdata[4]), atof(tempdata[5])));
		else
			StoreCommand(Simcore->GetElevator(atoi(tempdata[0]))->AddShaftDoor(Current, atoi(tempdata[1]), tempdata[2], tempdata[2], atof(tempdata[3]), atof(tempdata[4])));
	}

	//AddFloorIndicator command
	if (LineData.Slice(0, 17).CompareNoCase("addfloorindicator") == true)
	{
		//get data
		int params = SplitData(LineData, 18);

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
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
		}
		else
		{
			for (int i = 0; i <= 7; i++)
			{
				if (i == 1)
					i = 3;
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
		}

		if (compat == false)
			StoreCommand(floor->AddFloorIndicator(atoi(tempdata[0]), csString(tempdata[1]).CompareNoCase("true"), tempdata[2], tempdata[3], atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8])));
		else
			StoreCommand(floor->AddFloorIndicator(atoi(tempdata[0]), csString(tempdata[1]).CompareNoCase("true"), "Button", tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7])));
	}

	//AddFillerWalls command
	if (LineData.Slice(0, 14).CompareNoCase("addfillerwalls") == true)
	{
		//get data
		int params = SplitData(LineData, 15);

		if (params != 10)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 9; i++)
		{
			if (i == 7)
				i = 8;
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		floor->AddFillerWalls(tempdata[0], atof(tempdata[1]), atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), csString(tempdata[7]).CompareNoCase("true"), atof(tempdata[8]), atof(tempdata[9]));
	}

	//AddSound
	if (LineData.Slice(0, 8).CompareNoCase("addsound") == true)
	{
		//get data
		int params = SplitData(LineData, 9);

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
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
		}
		else
		{
			for (int i = 2; i <= 12; i++)
			{
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
		}

		//check to see if file exists
		CheckFile(csString("data/") + tempdata[1], true);

		if (partial == true)
			StoreCommand(floor->AddSound(tempdata[0], tempdata[1], csVector3(atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]))));
		else
			StoreCommand(floor->AddSound(tempdata[0], tempdata[1], csVector3(atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4])), atoi(tempdata[5]), atoi(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), csVector3(atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12])))); 
	}

	//AddShaftDoorComponent command
	if (LineData.Slice(0, 21).CompareNoCase("addshaftdoorcomponent") == true)
	{
		//get data
		int params = SplitData(LineData, 22);

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
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		Elevator *elev = Simcore->GetElevator(atoi(tempdata[0]));
		if (!elev)
			return ScriptError("Invalid elevator");

		if (compat == true)
			StoreCommand(elev->AddShaftDoorComponent(atoi(tempdata[1]), Current, tempdata[2], tempdata[3], tempdata[4], atof(tempdata[5]), tempdata[6], atof(tempdata[7]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12]), atof(tempdata[13]), atof(tempdata[14]), atof(tempdata[15]), atof(tempdata[16]), atof(tempdata[17])));
		else
			StoreCommand(elev->AddShaftDoorComponent(atoi(tempdata[1]), Current, tempdata[2], tempdata[3], tempdata[4], atof(tempdata[5]), tempdata[6], atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12]), atof(tempdata[13]), atof(tempdata[14]), atof(tempdata[15]), atof(tempdata[16]), atof(tempdata[17]), atof(tempdata[18])));
	}

	//FinishShaftDoor command
	if (LineData.Slice(0, 16).CompareNoCase("finishshaftdoor ") == true)
	{
		//get data
		int params = SplitData(LineData, 16);

		if (params < 2)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 1; i++)
		{
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		Elevator *elev = Simcore->GetElevator(atoi(tempdata[0]));
		if (!elev)
			return ScriptError("Invalid elevator");

		StoreCommand(elev->FinishShaftDoor(atoi(tempdata[1]), Current));
	}

	//AddModel command
	if (LineData.Slice(0, 8).CompareNoCase("addmodel") == true)
	{
		//get data
		int params = SplitData(LineData, 9);

		if (params != 10)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 9; i++)
		{
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		//check to see if file exists
		CheckFile(csString("data/") + tempdata[1], true);

		//create model
		StoreCommand(floor->AddModel(tempdata[0], tempdata[1], csVector3(atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4])), csVector3(atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7])), atof(tempdata[8]), atof(tempdata[9])));
	}

	//AddStairsModel command
	if (LineData.Slice(0, 14).CompareNoCase("addstairsmodel") == true)
	{
		//get data
		int params = SplitData(LineData, 15);

		if (params != 11)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 10; i++)
		{
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		//check to see if file exists
		CheckFile(csString("data/") + tempdata[1], true);

		//create model
		if (Simcore->GetStairs(atoi(tempdata[0])))
			StoreCommand(Simcore->GetStairs(atoi(tempdata[0]))->AddModel(Current, tempdata[1], tempdata[2], csVector3(atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5])), csVector3(atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8])), atof(tempdata[9]), atof(tempdata[10])));
		else
			return ScriptError("Invalid stairwell");
	}

	//AddShaftModel command
	if (LineData.Slice(0, 13).CompareNoCase("addshaftmodel") == true)
	{
		//get data
		int params = SplitData(LineData, 14);

		if (params != 11)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 10; i++)
		{
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		//check to see if file exists
		CheckFile(csString("data/") + tempdata[1], true);

		//create model
		if (Simcore->GetShaft(atoi(tempdata[0])))
			StoreCommand(Simcore->GetShaft(atoi(tempdata[0]))->AddModel(Current, tempdata[1], tempdata[2], csVector3(atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5])), csVector3(atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8])), atof(tempdata[9]), atof(tempdata[10])));
		else
			return ScriptError("Invalid shaft");
	}

	//Cut command
	if (LineData.Slice(0, 4).CompareNoCase("cut ") == true)
	{
		//get data
		int params = SplitData(LineData, 4);

		if (params != 8)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 5; i++)
		{
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		//perform cut on floor
		Simcore->GetFloor(Current)->Cut(csVector3(atof(tempdata[0]), atof(tempdata[1]), atof(tempdata[2])), csVector3(atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5])), csString(tempdata[6]).CompareNoCase("true"), csString(tempdata[7]).CompareNoCase("true"), false);
	}

	//CutAll command
	if (LineData.Slice(0, 6).CompareNoCase("cutall") == true)
	{
		//get data
		int params = SplitData(LineData, 7);

		if (params != 8)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 5; i++)
		{
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		//perform cut on all objects related to the current floor
		Simcore->GetFloor(Current)->CutAll(csVector3(atof(tempdata[0]), atof(tempdata[1]), atof(tempdata[2])), csVector3(atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5])), csString(tempdata[6]).CompareNoCase("true"), csString(tempdata[7]).CompareNoCase("true"));
	}

	//handle floor range
	if (RangeL != RangeH && LineData.Slice(0, 9).CompareNoCase("<endfloor") == true)
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
	buffer = Current;
	LineData.ReplaceAll("%elevator%", buffer);

	//IF statement
	if (LineData.Slice(0, 2).CompareNoCase("if") == true)
	{
		temp1 = LineData.Find("[", 0);
		temp3 = LineData.Find("]", 0);
		if (temp1 + temp3 > 0)
			temp2 = LineData.Slice(temp1 + 1, temp3 - temp1 - 1);
		else
			temp2 = "";
		temp2.Trim();
		if (IfProc(temp2) == true)
		{
			LineData = LineData.Slice(temp3 + 1).Trim(); //trim off IF statement
			return sCheckFloors;
		}
		else
			return sNextLine; //skip line
	}

	//process functions
	if (FunctionProc() == true)
		return sNextLine;

	//get text after equal sign
	int temp2check = LineData.Find("=", 0);
	temp2 = GetAfterEquals(LineData);

	Elevator *elev = Simcore->GetElevator(Current);

	//parameters
	if (LineData.Slice(0, 4).CompareNoCase("name") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->Name = temp2.Trim();
	}
	if (LineData.Slice(0, 5).CompareNoCase("speed") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (!IsNumeric(temp2.GetData(), elev->ElevatorSpeed))
			return ScriptError("Invalid value");
	}
	if (LineData.Slice(0, 12).CompareNoCase("acceleration") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (!IsNumeric(temp2.GetData(), elev->Acceleration))
			return ScriptError("Invalid value");
	}
	if (LineData.Slice(0, 12).CompareNoCase("deceleration") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (!IsNumeric(temp2.GetData(), elev->Deceleration))
			return ScriptError("Invalid value");
	}
	if (LineData.Slice(0, 9).CompareNoCase("openspeed") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (!IsNumeric(LineData.Slice(9, LineData.Find("=", 0) - 9).Trim(), temp3))
			return ScriptError("No door specified");
		if (temp3 == 0 || temp3 > elev->NumDoors)
			return ScriptError("Invalid door number");
		if (!IsNumeric(temp2.GetData(), elev->GetDoor(temp3)->OpenSpeed))
			return ScriptError("Invalid value");
	}
	if (LineData.Slice(0, 5).CompareNoCase("doors") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (!IsNumeric(temp2.GetData(), elev->NumDoors))
			return ScriptError("Invalid value");
	}
	if (LineData.Slice(0, 9).CompareNoCase("acceljerk") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (!IsNumeric(temp2.GetData(), elev->AccelJerk))
			return ScriptError("Invalid value");
	}
	if (LineData.Slice(0, 9).CompareNoCase("deceljerk") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (!IsNumeric(temp2.GetData(), elev->DecelJerk))
			return ScriptError("Invalid value");
	}
	if (LineData.Slice(0, 14).CompareNoCase("servicedfloors") == true)
	{
		//copy string listing of serviced floors into array
		int params = SplitAfterEquals(LineData, false);
		if (params == -1)
			return ScriptError("Syntax Error");

		for (int line = 0; line < params; line++)
		{
			csString tmpstring = tempdata[line];
			tmpstring.Trim();
			if (tmpstring.Find("-", 1) > 0)
			{
				int start, end;
				//found a range marker
				if (!IsNumeric(tmpstring.Slice(0, tmpstring.Find("-", 1)).Trim().GetData(), start) || !IsNumeric(tmpstring.Slice(tmpstring.Find("-", 1) + 1).Trim().GetData(), end))
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
				if (!IsNumeric(csString(tempdata[line]).Trim().GetData(), data))
					return ScriptError("Invalid value");
				if (!elev->AddServicedFloor(data))
					return ScriptError();
			}
		}
	}
	if (LineData.Slice(0, 13).CompareNoCase("assignedshaft") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (!IsNumeric(temp2.GetData(), elev->AssignedShaft))
			return ScriptError("Invalid value");
	}
	if (LineData.Slice(0, 9).CompareNoCase("doortimer") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (!IsNumeric(LineData.Slice(9, LineData.Find("=", 0) - 9).Trim(), temp3))
			return ScriptError("No door specified");
		if (temp3 == 0 || temp3 > elev->NumDoors)
			return ScriptError("Invalid door number");
		if (!IsNumeric(temp2.GetData(), elev->GetDoor(temp3)->DoorTimer))
			return ScriptError("Invalid value");
	}
	if (LineData.Slice(0, 10).CompareNoCase("quickclose") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (!IsNumeric(LineData.Slice(10, LineData.Find("=", 0) - 10).Trim(), temp3))
			return ScriptError("No door specified");
		if (temp3 == 0 || temp3 > elev->NumDoors)
			return ScriptError("Invalid door number");
		if (!IsNumeric(temp2.GetData(), elev->GetDoor(temp3)->QuickClose))
			return ScriptError("Invalid value");
	}
	if (LineData.Slice(0, 10).CompareNoCase("nudgetimer") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (!IsNumeric(LineData.Slice(10, LineData.Find("=", 0) - 10).Trim(), temp3))
			return ScriptError("No door specified");
		if (temp3 == 0 || temp3 > elev->NumDoors)
			return ScriptError("Invalid door number");
		if (!IsNumeric(temp2.GetData(), elev->GetDoor(temp3)->NudgeTimer))
			return ScriptError("Invalid value");
	}
	if (LineData.Slice(0, 9).CompareNoCase("slowspeed") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (!IsNumeric(LineData.Slice(9, LineData.Find("=", 0) - 9).Trim(), temp3))
			return ScriptError("No door specified");
		if (temp3 == 0 || temp3 > elev->NumDoors)
			return ScriptError("Invalid door number");
		if (!IsNumeric(temp2.GetData(), elev->GetDoor(temp3)->SlowSpeed))
			return ScriptError("Invalid value");
	}
	if (LineData.Slice(0, 11).CompareNoCase("manualspeed") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (!IsNumeric(LineData.Slice(11, LineData.Find("=", 0) - 11).Trim(), temp3))
			return ScriptError("No door specified");
		if (temp3 == 0 || temp3 > elev->NumDoors)
			return ScriptError("Invalid door number");
		if (!IsNumeric(temp2.GetData(), elev->GetDoor(temp3)->ManualSpeed))
			return ScriptError("Invalid value");
	}
	if (LineData.Slice(0, 9).CompareNoCase("opensound") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (!IsNumeric(LineData.Slice(9, LineData.Find("=", 0) - 9).Trim(), temp3))
			return ScriptError("No door specified");
		if (temp3 == 0 || temp3 > elev->NumDoors)
			return ScriptError("Invalid door number");

		//check to see if file exists
		CheckFile(csString("data/") + temp2, true);

		elev->GetDoor(temp3)->OpenSound = temp2;
	}
	if (LineData.Slice(0, 10).CompareNoCase("closesound") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (!IsNumeric(LineData.Slice(10, LineData.Find("=", 0) - 10).Trim(), temp3))
			return ScriptError("No door specified");
		if (temp3 == 0 || temp3 > elev->NumDoors)
			return ScriptError("Invalid door number");

		//check to see if file exists
		CheckFile(csString("data/") + temp2, true);

		elev->GetDoor(temp3)->CloseSound = temp2;
	}
	if (LineData.Slice(0, 10).CompareNoCase("nudgesound") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (!IsNumeric(LineData.Slice(10, LineData.Find("=", 0) - 10).Trim(), temp3))
			return ScriptError("No door specified");
		if (temp3 == 0 || temp3 > elev->NumDoors)
			return ScriptError("Invalid door number");

		//check to see if file exists
		CheckFile(csString("data/") + temp2, true);

		elev->GetDoor(temp3)->NudgeSound = temp2;
	}
	if (LineData.Slice(0, 10).CompareNoCase("startsound") == true)
	{
		//backwards compatibility
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(csString("data/") + temp2, true);

		elev->CarStartSound = temp2;
		//turn off motor sounds
		elev->MotorStartSound = "";
		elev->MotorRunSound = "";
		elev->MotorStopSound = "";
		elev->MotorIdleSound = "";
	}
	if (LineData.Slice(0, 9).CompareNoCase("movesound") == true)
	{
		//backwards compatibility
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(csString("data/") + temp2, true);

		elev->CarMoveSound = temp2;
		//turn off motor sounds
		elev->MotorStartSound = "";
		elev->MotorRunSound = "";
		elev->MotorStopSound = "";
		elev->MotorIdleSound = "";
	}
	if (LineData.Slice(0, 9).CompareNoCase("stopsound") == true)
	{
		//backwards compatibility
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(csString("data/") + temp2, true);

		elev->CarStopSound = temp2;
		//turn off motor sounds
		elev->MotorStartSound = "";
		elev->MotorRunSound = "";
		elev->MotorStopSound = "";
		elev->MotorIdleSound = "";
	}
	if (LineData.Slice(0, 9).CompareNoCase("idlesound") == true)
	{
		//backwards compatibility
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(csString("data/") + temp2, true);

		elev->CarIdleSound = temp2;
		//turn off motor sounds
		elev->MotorStartSound = "";
		elev->MotorRunSound = "";
		elev->MotorStopSound = "";
		elev->MotorIdleSound = "";
	}
	if (LineData.Slice(0, 13).CompareNoCase("carstartsound") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(csString("data/") + temp2, true);

		elev->CarStartSound = temp2;
	}
	if (LineData.Slice(0, 12).CompareNoCase("carmovesound") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(csString("data/") + temp2, true);

		elev->CarMoveSound = temp2;
	}
	if (LineData.Slice(0, 12).CompareNoCase("carstopsound") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(csString("data/") + temp2, true);

		elev->CarStopSound = temp2;
	}
	if (LineData.Slice(0, 12).CompareNoCase("caridlesound") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(csString("data/") + temp2, true);

		elev->CarIdleSound = temp2;
	}
	if (LineData.Slice(0, 15).CompareNoCase("motorstartsound") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(csString("data/") + temp2, true);

		elev->MotorStartSound = temp2;
	}
	if (LineData.Slice(0, 13).CompareNoCase("motorrunsound") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(csString("data/") + temp2, true);

		elev->MotorRunSound = temp2;
	}
	if (LineData.Slice(0, 14).CompareNoCase("motorstopsound") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(csString("data/") + temp2, true);

		elev->MotorStopSound = temp2;
	}
	if (LineData.Slice(0, 10).CompareNoCase("chimesound") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (!IsNumeric(LineData.Slice(10, LineData.Find("=", 0) - 10).Trim(), temp3))
			return ScriptError("No door specified");
		if (temp3 == 0 || temp3 > elev->NumDoors)
			return ScriptError("Invalid door number");

		//check to see if file exists
		CheckFile(csString("data/") + temp2, true);

		elev->GetDoor(temp3)->UpChimeSound = temp2;
		elev->GetDoor(temp3)->DownChimeSound = temp2;
	}
	if (LineData.Slice(0, 12).CompareNoCase("upchimesound") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (!IsNumeric(LineData.Slice(12, LineData.Find("=", 0) - 12).Trim(), temp3))
			return ScriptError("No door specified");
		if (temp3 == 0 || temp3 > elev->NumDoors)
			return ScriptError("Invalid door number");

		//check to see if file exists
		CheckFile(csString("data/") + temp2, true);

		elev->GetDoor(temp3)->UpChimeSound = temp2;
	}
	if (LineData.Slice(0, 14).CompareNoCase("downchimesound") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		if (elev->Created == false)
			return ScriptError("Elevator not created yet");
		if (!IsNumeric(LineData.Slice(14, LineData.Find("=", 0) - 14).Trim(), temp3))
			return ScriptError("No door specified");
		if (temp3 == 0 || temp3 > elev->NumDoors)
			return ScriptError("Invalid door number");

		//check to see if file exists
		CheckFile(csString("data/") + temp2, true);

		elev->GetDoor(temp3)->DownChimeSound = temp2;
	}
	if (LineData.Slice(0, 10).CompareNoCase("alarmsound") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(csString("data/") + temp2, true);

		elev->AlarmSound = temp2;
	}
	if (LineData.Slice(0, 14).CompareNoCase("alarmsoundstop") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(csString("data/") + temp2, true);

		elev->AlarmSoundStop = temp2;
	}
	if (LineData.Slice(0, 9).CompareNoCase("beepsound") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(csString("data/") + temp2, true);

		elev->SetBeepSound(temp2);
	}
	if (LineData.Slice(0, 10).CompareNoCase("floorsound") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(csString("data/") + temp2, true);

		elev->SetFloorSound(temp2);
	}
	if (LineData.Slice(0, 9).CompareNoCase("upmessage") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(csString("data/") + temp2, true);

		elev->SetMessageSound(true, temp2);
	}
	if (LineData.Slice(0, 11).CompareNoCase("downmessage") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(csString("data/") + temp2, true);

		elev->SetMessageSound(false, temp2);
	}
	if (LineData.Slice(0, 5).CompareNoCase("music") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");

		//check to see if file exists
		CheckFile(csString("data/") + temp2, true);

		elev->Music = temp2;
	}
	if (LineData.Slice(0, 13).CompareNoCase("floorskiptext") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->SetFloorSkipText(temp2);
	}
	if (LineData.Slice(0, 11).CompareNoCase("recallfloor") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		int floortemp;
		if (!IsNumeric(temp2.GetData(), floortemp))
			return ScriptError("Invalid value");
		elev->SetRecallFloor(floortemp);
	}
	if (LineData.Slice(0, 20).CompareNoCase("alternaterecallfloor") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		int floortemp;
		if (!IsNumeric(temp2.GetData(), floortemp))
			return ScriptError("Invalid value");
		elev->SetAlternateRecallFloor(floortemp);
	}
	if (LineData.Slice(0, 8).CompareNoCase("acpfloor") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		int floortemp;
		if (!IsNumeric(temp2.GetData(), floortemp))
			return ScriptError("Invalid value");
		elev->SetACPFloor(floortemp);
	}
	if (LineData.Slice(0, 13).CompareNoCase("motorposition") == true)
	{
		int params = SplitAfterEquals(LineData);
		if (params == -1)
			return ScriptError("Syntax Error");
		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 2; i++)
		{
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		elev->MotorPosition = csVector3(atof(tempdata[0]), atof(tempdata[1]), atof(tempdata[2]));
	}
	if (LineData.Slice(0, 11).CompareNoCase("queueresets") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->QueueResets = csString(temp2).CompareNoCase("true");
	}
	if (LineData.Slice(0, 3).CompareNoCase("acp") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->ACP = csString(temp2).CompareNoCase("true");
	}
	if (LineData.Slice(0, 6).CompareNoCase("uppeak") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->UpPeak = csString(temp2).CompareNoCase("true");
	}
	if (LineData.Slice(0, 8).CompareNoCase("downpeak") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->DownPeak = csString(temp2).CompareNoCase("true");
	}
	if (LineData.Slice(0, 18).CompareNoCase("independentservice") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->IndependentService = csString(temp2).CompareNoCase("true");
	}
	if (LineData.Slice(0, 17).CompareNoCase("inspectionservice") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		elev->InspectionService = csString(temp2).CompareNoCase("true");
	}
	if (LineData.Slice(0, 7).CompareNoCase("parking") == true)
	{
		int params = SplitAfterEquals(LineData);
		if (params == -1)
			return ScriptError("Syntax Error");
		if (params != 2)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 1; i++)
		{
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		elev->ParkingFloor = atoi(tempdata[0]);
		elev->ParkingDelay = atof(tempdata[1]);
	}
	if (LineData.Slice(0, 13).CompareNoCase("levelingspeed") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		float leveling;
		if (!IsNumeric(temp2.GetData(), leveling))
			return ScriptError("Invalid value");
		elev->LevelingSpeed = leveling;
	}
	if (LineData.Slice(0, 14).CompareNoCase("levelingoffset") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		float leveling;
		if (!IsNumeric(temp2.GetData(), leveling))
			return ScriptError("Invalid value");
		elev->LevelingOffset = leveling;
	}
	if (LineData.Slice(0, 12).CompareNoCase("levelingopen") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		float leveling;
		if (!IsNumeric(temp2.GetData(), leveling))
			return ScriptError("Invalid value");
		elev->LevelingOpen = leveling;
	}
	if (LineData.Slice(0, 11).CompareNoCase("notifyearly") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		float notify;
		if (!IsNumeric(temp2.GetData(), notify))
			return ScriptError("Invalid value");
		elev->NotifyEarly = notify;
	}
	if (LineData.Slice(0, 14).CompareNoCase("departuredelay") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		float delay;
		if (!IsNumeric(temp2.GetData(), delay))
			return ScriptError("Invalid value");
		elev->DepartureDelay = delay;
	}
	if (LineData.Slice(0, 12).CompareNoCase("arrivaldelay") == true)
	{
		if (temp2check < 0)
			return ScriptError("Syntax error");
		float delay;
		if (!IsNumeric(temp2.GetData(), delay))
			return ScriptError("Invalid value");
		elev->ArrivalDelay = delay;
	}
	if (LineData.Slice(0, 13).CompareNoCase("musicposition") == true)
	{
		int params = SplitAfterEquals(LineData);
		if (params == -1)
			return ScriptError("Syntax Error");
		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 2; i++)
		{
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		elev->MusicPosition = csVector3(atof(tempdata[0]), atof(tempdata[1]), atof(tempdata[2]));
	}

	//Print command
	if (LineData.Slice(0, 5).CompareNoCase("print") == true)
	{
		//calculate inline math
		buffer = Calc(LineData.Slice(6));

		//print line
		skyscraper->Report(buffer);
	}

	if (LineData.Slice(0, 7).CompareNoCase("<break>") == true)
		return sBreak;

	//CreateElevator command
	if (LineData.Slice(0, 14).CompareNoCase("createelevator") == true)
	{
		//get data
		int params = SplitData(LineData, 15);

		if (params != 4)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 3; i++)
		{
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		Object *object = elev->CreateElevator(csString(tempdata[0]).CompareNoCase("true"), atof(tempdata[1]), atof(tempdata[2]), atoi(tempdata[3]));
		if (!object)
			return ScriptError();
		StoreCommand(object);
	}

	//AddFloor command
	if (LineData.Slice(0, 9).CompareNoCase("addfloor ") == true)
	{
		//get data
		int params = SplitData(LineData, 9);

		if (params != 11)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 10; i++)
		{
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		//create floor
		StoreCommand(elev->AddFloor(tempdata[0], tempdata[1], atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10])));
	}

	//AddWall command
	if (LineData.Slice(0, 7).CompareNoCase("addwall") == true)
	{
		//get data
		int params = SplitData(LineData, 8);

		if (params != 13)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 12; i++)
		{
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		//create wall
		StoreCommand(elev->AddWall(tempdata[0], tempdata[1], atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12])));
	}

	//AddDoors command
	if (LineData.Slice(0, 8).CompareNoCase("adddoors") == true)
	{
		//get data
		int params = SplitData(LineData, 9);

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
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
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
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
		}

		if (compat == false)
			StoreCommand(elev->AddDoors(atoi(tempdata[0]), tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), csString(tempdata[8]).CompareNoCase("true"), atof(tempdata[9]), atof(tempdata[10])));
		else
			StoreCommand(elev->AddDoors(atoi(tempdata[0]), tempdata[1], tempdata[1], atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), csString(tempdata[7]).CompareNoCase("true"), atof(tempdata[8]), atof(tempdata[9])));
	}

	//SetShaftDoors command
	if (LineData.Slice(0, 13).CompareNoCase("setshaftdoors") == true)
	{
		//get data
		int params = SplitData(LineData, 14);

		if (params != 4)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 3; i++)
		{
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		elev->SetShaftDoors(atoi(tempdata[0]), atof(tempdata[1]), atof(tempdata[2]), atof(tempdata[3]));
		setshaftdoors = true;
	}

	//AddShaftDoors command
	if (LineData.Slice(0, 14).CompareNoCase("addshaftdoors ") == true)
	{
		//get data
		int params = SplitData(LineData, 14);

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
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
		}
		else
		{
			for (int i = 0; i <= 6; i++)
			{
				if (i == 1)
					i = 2;
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
		}
		
		bool result;
		if (compat == false)
			result = elev->AddShaftDoors(atoi(tempdata[0]), tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]));
		else
			result = elev->AddShaftDoors(atoi(tempdata[0]), tempdata[1], tempdata[1], atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]));

		if (result == false)
			return ScriptError();
	}

	//CreatePanel command
	if (LineData.Slice(0, 11).CompareNoCase("createpanel") == true)
	{
		//get data
		int params = SplitData(LineData, 12);

		if (params != 13)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 12; i++)
		{
			if (i == 3)
				i = 4;
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		StoreCommand(elev->CreateButtonPanel(tempdata[0], atoi(tempdata[1]), atoi(tempdata[2]), tempdata[3], atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12])));
	}

	//AddFloorButton command
	if (LineData.Slice(0, 14).CompareNoCase("addfloorbutton") == true)
	{
		//get data
		int params = SplitData(LineData, 15);

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
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
			compat = 1;
		}
		if (params == 9)
		{
			if (IsNumeric(csString(tempdata[2]).Trim().GetData()) == true)
			{
				//1.5 compatibility mode
				//check numeric values
				for (int i = 0; i <= 8; i++)
				{
					if (i == 1 || i == 4)
						i++;

					if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
						return ScriptError("Invalid value: " + csString(tempdata[i]));
				}
				hoffset = atof(tempdata[7]);
				voffset = atof(tempdata[8]);
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
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
			if (params == 10)
			{
				hoffset = atof(tempdata[8]);
				voffset = atof(tempdata[9]);
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
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
			if (params == 11)
			{
				hoffset = atof(tempdata[9]);
				voffset = atof(tempdata[10]);
			}
		}

		if (!elev->GetPanel(atoi(tempdata[0])))
			return ScriptError("Invalid panel number");

		if (compat == 0)
		{
			CheckFile(csString("data/") + tempdata[1], true);
			elev->GetPanel(atoi(tempdata[0]))->AddButton(tempdata[1], tempdata[2], tempdata[3], atoi(tempdata[4]), atoi(tempdata[5]), tempdata[6], atof(tempdata[7]), atof(tempdata[8]), hoffset, voffset);
		}
		if (compat == 1)
			elev->GetPanel(atoi(tempdata[0]))->AddButton("", tempdata[1], tempdata[1], atoi(tempdata[2]), atoi(tempdata[3]), tempdata[4], atof(tempdata[5]), atof(tempdata[6]), hoffset, voffset);
		if (compat == 2)
			elev->GetPanel(atoi(tempdata[0]))->AddButton("", tempdata[1], tempdata[2], atoi(tempdata[3]), atoi(tempdata[4]), tempdata[5], atof(tempdata[6]), atof(tempdata[7]), hoffset, voffset);
	}

	//AddControlButton command
	if (LineData.Slice(0, 16).CompareNoCase("addcontrolbutton") == true)
	{
		//get data
		int params = SplitData(LineData, 17);

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
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
			compat = 1;
		}
		if (params == 9)
		{
			if (IsNumeric(csString(tempdata[2]).Trim().GetData()) == true)
			{
				//1.5 compatibility mode
				//check numeric values
				for (int i = 1; i <= 8; i++)
				{
					if (i == 1 || i == 4)
						i++;
					if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
						return ScriptError("Invalid value: " + csString(tempdata[i]));
				}
				hoffset = atof(tempdata[7]);
				voffset = atof(tempdata[8]);
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
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
			if (params == 10)
			{
				hoffset = atof(tempdata[8]);
				voffset = atof(tempdata[9]);
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
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
			if (params == 11)
			{
				hoffset = atof(tempdata[9]);
				voffset = atof(tempdata[10]);
			}
		}

		if (!elev->GetPanel(atoi(tempdata[0])))
			return ScriptError("Invalid panel number");

		if (compat == 0)
		{
			CheckFile(csString("data/") + tempdata[1], true);
			elev->GetPanel(atoi(tempdata[0]))->AddButton(tempdata[1], tempdata[2], tempdata[3], atoi(tempdata[4]), atoi(tempdata[5]), tempdata[6], atof(tempdata[7]), atof(tempdata[8]), hoffset, voffset);
		}
		if (compat == 1)
			elev->GetPanel(atoi(tempdata[0]))->AddButton("", tempdata[1], tempdata[1], atoi(tempdata[2]), atoi(tempdata[3]), tempdata[4], atof(tempdata[5]), atof(tempdata[6]), hoffset, voffset);
		if (compat == 2)
			elev->GetPanel(atoi(tempdata[0]))->AddButton("", tempdata[1], tempdata[2], atoi(tempdata[3]), atoi(tempdata[4]), tempdata[5], atof(tempdata[6]), atof(tempdata[7]), hoffset, voffset);
	}

	//AddButton command
	if (LineData.Slice(0, 10).CompareNoCase("addbutton ") == true)
	{
		//get data
		int params = SplitData(LineData, 10);

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
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}
		if (params == 11)
		{
			hoffset = atof(tempdata[9]);
			voffset = atof(tempdata[10]);
		}

		if (!elev->GetPanel(atoi(tempdata[0])))
			return ScriptError("Invalid panel number");

		//check to see if file exists
		CheckFile(csString("data/") + tempdata[1], true);

		elev->GetPanel(atoi(tempdata[0]))->AddButton(tempdata[1], tempdata[2], tempdata[3], atoi(tempdata[4]), atoi(tempdata[5]), tempdata[6], atof(tempdata[7]), atof(tempdata[8]), hoffset, voffset);
	}

	//AddControl command
	if (LineData.Slice(0, 11).CompareNoCase("addcontrol ") == true)
	{
		//get data
		int params = SplitData(LineData, 11);

		if (params < 10)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 7; i++)
		{
			if (i == 1)
				i++;
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		if (!elev->GetPanel(atoi(tempdata[0])))
			return ScriptError("Invalid panel number");

		csArray<csString> action_array, tex_array;
		int slength, parameters;

		//get number of action & texture parameters
		slength = tempdata.GetSize();
		parameters = slength - 8; //strip off main parameters

		//action & texture parameter number needs to be even
		if (IsEven(parameters) == false)
			return ScriptError("Incorrect number of parameters");

		for (temp3 = 8; temp3 < slength - (parameters / 2); temp3++)
			action_array.Push(tempdata[temp3]);
		for (temp3 = slength - (parameters / 2); temp3 < slength; temp3++)
			tex_array.Push(tempdata[temp3]);

		//check to see if file exists
		CheckFile(csString("data/") + tempdata[1], true);

		elev->GetPanel(atoi(tempdata[0]))->AddControl(tempdata[1], atoi(tempdata[2]), atoi(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), action_array, tex_array);
	}

	//AddFloorIndicator command
	if (LineData.Slice(0, 17).CompareNoCase("addfloorindicator") == true)
	{
		//get data
		int params = SplitData(LineData, 18);

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
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
		}
		else
		{
			for (int i = 1; i <= 5; i++)
			{
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
		}

		if (compat == false)
			StoreCommand(elev->AddFloorIndicator(tempdata[0], tempdata[1], atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6])));
		else
			StoreCommand(elev->AddFloorIndicator("Button", tempdata[0], atof(tempdata[1]), atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5])));
	}

	//AddDirectionalIndicators command
	if (LineData.Slice(0, 24).CompareNoCase("adddirectionalindicators") == true)
	{
		//get data
		int params = SplitData(LineData, 25);

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
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
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
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
		}

		if (compat == false)
			elev->AddDirectionalIndicators(csString(tempdata[0]).CompareNoCase("true"), csString(tempdata[1]).CompareNoCase("true"), csString(tempdata[2]).CompareNoCase("true"), csString(tempdata[3]).CompareNoCase("true"), tempdata[4], tempdata[5], tempdata[6], tempdata[7], tempdata[8], atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), tempdata[12], atof(tempdata[13]), atof(tempdata[14]), csString(tempdata[15]).CompareNoCase("true"), atof(tempdata[16]), atof(tempdata[17]));
		else
			elev->AddDirectionalIndicators(csString(tempdata[0]).CompareNoCase("true"), false, csString(tempdata[1]).CompareNoCase("true"), csString(tempdata[2]).CompareNoCase("true"), tempdata[3], tempdata[4], tempdata[5], tempdata[6], tempdata[7], atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), tempdata[11], atof(tempdata[12]), atof(tempdata[13]), csString(tempdata[14]).CompareNoCase("true"), atof(tempdata[15]), atof(tempdata[16]));
	}

	//AddFloorSigns command
	if (LineData.Slice(0, 13).CompareNoCase("addfloorsigns") == true)
	{
		//get data
		int params = SplitData(LineData, 14);

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
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
		}
		else if (compat == 1)
		{
			for (int i = 2; i <= 6; i++)
			{
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
		}
		else if (compat == 2)
		{
			for (int i = 3; i <= 7; i++)
			{
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
		}

		if (compat == 0)
		{
			bool result;
			result = elev->AddFloorSigns(atoi(tempdata[0]), csString(tempdata[1]).CompareNoCase("true"), tempdata[2], tempdata[3], atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]));
			if (result == false)
				return ScriptError();
		}
		else if (compat == 1)
			elev->AddFloorSigns(0, csString(tempdata[0]).CompareNoCase("true"), "Button", tempdata[1], atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]));
		else if (compat == 2)
			elev->AddFloorSigns(0, csString(tempdata[0]).CompareNoCase("true"), tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]));
	}

	//AddSound
	if (LineData.Slice(0, 8).CompareNoCase("addsound") == true)
	{
		//get data
		int params = SplitData(LineData, 9);

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
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
		}
		else
		{
			for (int i = 2; i <= 12; i++)
			{
				if (!IsNumeric(tempdata[i]))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
		}

		//check to see if file exists
		CheckFile(csString("data/") + tempdata[1], true);

		if (partial == true)
			StoreCommand(elev->AddSound(tempdata[0], tempdata[1], csVector3(atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]))));
		else
			StoreCommand(elev->AddSound(tempdata[0], tempdata[1], csVector3(atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4])), atoi(tempdata[5]), atoi(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), csVector3(atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12])))); 
	}

	//AddDoorComponent command
	if (LineData.Slice(0, 16).CompareNoCase("adddoorcomponent") == true)
	{
		//get data
		int params = SplitData(LineData, 17);

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
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		if (compat == true)
			StoreCommand(elev->AddDoorComponent(atoi(tempdata[0]), tempdata[1], tempdata[2], tempdata[3], atof(tempdata[4]), tempdata[5], atof(tempdata[6]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12]), atof(tempdata[13]), atof(tempdata[14]), atof(tempdata[15]), atof(tempdata[16])));
		else
			StoreCommand(elev->AddDoorComponent(atoi(tempdata[0]), tempdata[1], tempdata[2], tempdata[3], atof(tempdata[4]), tempdata[5], atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12]), atof(tempdata[13]), atof(tempdata[14]), atof(tempdata[15]), atof(tempdata[16]), atof(tempdata[17])));
	}

	//AddShaftDoorsComponent command
	if (LineData.Slice(0, 22).CompareNoCase("addshaftdoorscomponent") == true)
	{
		//get data
		int params = SplitData(LineData, 23);

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
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		if (compat == true)
			elev->AddShaftDoorsComponent(atoi(tempdata[0]), tempdata[1], tempdata[2], tempdata[3], atof(tempdata[4]), tempdata[5], atof(tempdata[6]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12]), atof(tempdata[13]), atof(tempdata[14]), atof(tempdata[15]), atof(tempdata[16]));
		else
			elev->AddShaftDoorsComponent(atoi(tempdata[0]), tempdata[1], tempdata[2], tempdata[3], atof(tempdata[4]), tempdata[5], atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12]), atof(tempdata[13]), atof(tempdata[14]), atof(tempdata[15]), atof(tempdata[16]), atof(tempdata[17]));
	}

	//FinishDoors command
	if (LineData.Slice(0, 11).CompareNoCase("finishdoors") == true)
	{
		//get data
		int params = SplitData(LineData, 12);

		if (params < 1)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		if (!IsNumeric(csString(tempdata[0]).Trim().GetData()))
			return ScriptError("Invalid value: " + csString(tempdata[0]));

		StoreCommand(elev->FinishDoors(atoi(tempdata[0])));
	}

	//FinishShaftDoors command
	if (LineData.Slice(0, 16).CompareNoCase("finishshaftdoors") == true)
	{
		//get data
		int params = SplitData(LineData, 17);

		if (params < 1)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		if (!IsNumeric(csString(tempdata[0]).Trim().GetData()))
			return ScriptError("Invalid value: " + csString(tempdata[0]));

		bool result;
		result = elev->FinishShaftDoors(atoi(tempdata[0]));

		if (result == false)
			return ScriptError();
	}

	//AddDirectionalIndicator command
	if (LineData.Slice(0, 24).CompareNoCase("adddirectionalindicator ") == true)
	{
		//get data
		int params = SplitData(LineData, 24);

		if (params != 17)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 8; i <= 16; i++)
		{
			if (i == 11 || i == 14)
				i++;
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		StoreCommand(elev->AddDirectionalIndicator(csString(tempdata[0]).CompareNoCase("true"), csString(tempdata[1]).CompareNoCase("true"), csString(tempdata[2]).CompareNoCase("true"), tempdata[3], tempdata[4], tempdata[5], tempdata[6], tempdata[7], atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), tempdata[11], atof(tempdata[12]), atof(tempdata[13]), csString(tempdata[14]).CompareNoCase("true"), atof(tempdata[15]), atof(tempdata[16])));
	}

	//AddDoor command
	if (LineData.Slice(0, 8).CompareNoCase("adddoor ") == true)
	{
		//get data
		int params = SplitData(LineData, 8);

		if (params < 12 || params > 14)
			return ScriptError("Incorrect number of parameters");

		int compat = 0;

		//check numeric values
		if (params == 12)
		{
			for (int i = 3; i <= 11; i++)
			{
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
			compat = 1;
		}
		if (params == 13)
		{
			for (int i = 3; i <= 12; i++)
			{
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
			compat = 2;
		}
		if (params == 14)
		{
			for (int i = 4; i <= 13; i++)
			{
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					return ScriptError("Invalid value: " + csString(tempdata[i]));
			}
		}

		CheckFile(csString("data/") + tempdata[0], true);
		CheckFile(csString("data/") + tempdata[1], true);

		//create door
		if (compat == 1)
			StoreCommand(elev->AddDoor(tempdata[0], tempdata[1], false, tempdata[2], atof(tempdata[3]), atoi(tempdata[4]), 0, atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11])));
		else if (compat == 2)
			StoreCommand(elev->AddDoor(tempdata[0], tempdata[1], false, tempdata[2], atof(tempdata[3]), atoi(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12])));
		else
			StoreCommand(elev->AddDoor(tempdata[0], tempdata[1], csString(tempdata[2]).CompareNoCase("true"), tempdata[3], atof(tempdata[4]), atoi(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12]), atof(tempdata[13])));
	}

	//AddModel command
	if (LineData.Slice(0, 8).CompareNoCase("addmodel") == true)
	{
		//get data
		int params = SplitData(LineData, 9);

		if (params != 10)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 9; i++)
		{
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}

		//check to see if file exists
		CheckFile(csString("data/") + tempdata[1], true);

		//create model
		StoreCommand(elev->AddModel(tempdata[0], tempdata[1], csVector3(atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4])), csVector3(atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7])), atof(tempdata[8]), atof(tempdata[9])));
	}

	//Set command
	if (LineData.Slice(0, 4).CompareNoCase("set ") == true)
	{
		temp1 = LineData.Find("=", 0);
		if (temp1 < 0)
			return ScriptError("Syntax error");

		if (!IsNumeric(LineData.Slice(4, temp1 - 5).Trim().GetData(), temp3))
			return ScriptError("Invalid variable number");

		//get text after equal sign
		temp2 = GetAfterEquals(LineData);

		if (temp3 < 0 || temp3 > UserVariable.GetSize() - 1)
			return ScriptError("Invalid variable number");

		UserVariable[temp3] = Calc(temp2);
		if (Simcore->Verbose == true)
			skyscraper->Report("Variable " + csString(_itoa(temp3, intbuffer, 10)) + " set to " + UserVariable[temp3]);
	}

	//handle elevator range
	if (RangeL != RangeH && LineData.Slice(0, 12).CompareNoCase("<endelevator") == true)
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

	if (LineData.Slice(0, 5).CompareNoCase("load ") == true)
	{
		//get data
		int params = SplitData(LineData, 5, false);

		if (params < 4 || params > 5)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 3; i++)
		{
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}
		buffer = tempdata[0];
		buffer.Insert(0, "/root/");
		CheckFile(buffer);
		if (params == 4)
			Simcore->LoadTexture(buffer.GetData(), tempdata[1], atof(tempdata[2]), atof(tempdata[3]));
		else
			Simcore->LoadTexture(buffer.GetData(), tempdata[1], atof(tempdata[2]), atof(tempdata[3]), true, csString(tempdata[4]).CompareNoCase("true"));
	}
	if (LineData.Slice(0, 9).CompareNoCase("loadrange") == true)
	{
		//get data
		int params = SplitData(LineData, 9, false);

		if (params < 6 || params > 7)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 5; i++)
		{
			if (i == 2)
				i = 4;
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}
		RangeL = atoi(tempdata[0]);
		RangeH = atoi(tempdata[1]);
		for (Current = RangeL; Current <= RangeH; Current++)
		{
			temp2 = tempdata[2];
			buffer = Current;
			temp2.ReplaceAll("%number%", buffer.Trim());
			temp6 = tempdata[3];
			temp6.ReplaceAll("%number%", buffer.Trim());
			CheckFile(temp2, true);
			if (params == 6)
				Simcore->LoadTexture("/root/" + temp2, temp6, atof(tempdata[4]), atof(tempdata[5]));
			else
				Simcore->LoadTexture("/root/" + temp2, temp6, atof(tempdata[4]), atof(tempdata[5]), true, csString(tempdata[6]).CompareNoCase("true"));
		}
	}
	if (LineData.Slice(0, 8).CompareNoCase("addtext ") == true)
	{
		//get data
		int params = SplitData(LineData, 8, false);

		if (params < 14 || params > 15)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 13; i++)
		{
			if (i == 4)
				i = 5;
			if (i == 9)
				i = 11;

			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}
		buffer = tempdata[2];
		buffer.Insert(0, "/root/data/fonts/");
		CheckFile(buffer);
		if (params == 14)
			Simcore->AddTextToTexture(tempdata[0], tempdata[1], buffer, atof(tempdata[3]), tempdata[4], atoi(tempdata[5]), atoi(tempdata[6]), atoi(tempdata[7]), atoi(tempdata[8]), tempdata[9], tempdata[10], atoi(tempdata[11]), atoi(tempdata[12]), atoi(tempdata[13]));
		else
			Simcore->AddTextToTexture(tempdata[0], tempdata[1], buffer, atof(tempdata[3]), tempdata[4], atoi(tempdata[5]), atoi(tempdata[6]), atoi(tempdata[7]), atoi(tempdata[8]), tempdata[9], tempdata[10], atoi(tempdata[11]), atoi(tempdata[12]), atoi(tempdata[13]), true, csString(tempdata[14]).CompareNoCase("true"));
	}
	if (LineData.Slice(0, 12).CompareNoCase("addtextrange") == true)
	{
		//get data
		int params = SplitData(LineData, 13, false);

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

			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}
		RangeL = atoi(tempdata[0]);
		RangeH = atoi(tempdata[1]);
		temp6 = LineData;
		for (Current = RangeL; Current <= RangeH; Current++)
		{
			buffer = Current;
			LineData = temp6;
			LineData.ReplaceAll("%number%", buffer.Trim());
		
			//get data
			int params = SplitData(LineData, 13, false);

			buffer = tempdata[4];
			buffer.Insert(0, "/root/data/fonts/");
			CheckFile(buffer);
			if (params == 16)
				Simcore->AddTextToTexture(tempdata[2], tempdata[3], buffer, atof(tempdata[5]), tempdata[6], atoi(tempdata[7]), atoi(tempdata[8]), atoi(tempdata[9]), atoi(tempdata[10]), tempdata[11], tempdata[12], atoi(tempdata[13]), atoi(tempdata[14]), atoi(tempdata[15]));
			else
				Simcore->AddTextToTexture(tempdata[2], tempdata[3], buffer, atof(tempdata[5]), tempdata[6], atoi(tempdata[7]), atoi(tempdata[8]), atoi(tempdata[9]), atoi(tempdata[10]), tempdata[11], tempdata[12], atoi(tempdata[13]), atoi(tempdata[14]), atoi(tempdata[15]), true, csString(tempdata[16]).CompareNoCase("true"));
		}
	}
	if (LineData.Slice(0, 11).CompareNoCase("loadcropped") == true)
	{
		//get data
		int params = SplitData(LineData, 12, false);

		if (params < 8 || params > 9)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 7; i++)
		{
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}
		buffer = tempdata[0];
		buffer.Insert(0, "/root/");
		CheckFile(buffer);
		if (params == 8)
			Simcore->LoadTextureCropped(buffer, tempdata[1], atoi(tempdata[2]), atoi(tempdata[3]), atoi(tempdata[4]), atoi(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]));
		else
			Simcore->LoadTextureCropped(buffer, tempdata[1], atoi(tempdata[2]), atoi(tempdata[3]), atoi(tempdata[4]), atoi(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), csString(tempdata[8]).CompareNoCase("true"));
	}
	if (LineData.Slice(0, 10).CompareNoCase("addoverlay") == true)
	{
		//get data
		int params = SplitData(LineData, 11, false);

		if (params < 9 || params > 10)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 3; i <= 8; i++)
		{
			if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				return ScriptError("Invalid value: " + csString(tempdata[i]));
		}
		if (params == 9)
			Simcore->AddTextureOverlay(tempdata[0], tempdata[1], tempdata[2], atoi(tempdata[3]), atoi(tempdata[4]), atoi(tempdata[5]), atoi(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]));
		else
			Simcore->AddTextureOverlay(tempdata[0], tempdata[1], tempdata[2], atoi(tempdata[3]), atoi(tempdata[4]), atoi(tempdata[5]), atoi(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), true, csString(tempdata[9]).CompareNoCase("true"));
	}
	return 0;
}

csString ScriptProcessor::Calc(const char *expression)
{
	//performs a calculation operation on a string
	//for example, the string "1 + 1" would output to "2"
	//supports multiple and nested operations (within parenthesis)

	int temp1;
	csString tmpcalc = expression;
	csString one;
	csString two;
	int start, end;
	CalcError = false;

	//first remove all whitespace from the string
	tmpcalc.ReplaceAll(" ", "");

	//find parenthesis
	do
	{
		start = tmpcalc.Find("(", 0);
		if (start >= 0)
		{
			//find matching parenthesis
			int match = 1;
			int end = -1;
			for (int i = start + 1; i < tmpcalc.Length(); i++)
			{
				if (tmpcalc.GetAt(i) == '(')
					match++;
				if (tmpcalc.GetAt(i) == ')')
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
				csString newdata;
				newdata = Calc(tmpcalc.Slice(start + 1, end - start - 1));

				if (CalcError == true)
					return newdata;

				//construct new string
				one = tmpcalc.Slice(0, start);
				if (end < tmpcalc.Length() - 1)
					two = tmpcalc.Slice(end + 1);
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
		for (int i = 1; i < tmpcalc.Length(); i++)
		{
			if (tmpcalc.GetAt(i) == '+' || tmpcalc.GetAt(i) == '/' || tmpcalc.GetAt(i) == '*')
			{
				operators++;
				if (operators == 2)
					end = i;
			}
			if (tmpcalc.GetAt(i) == '-' && tmpcalc.GetAt(i - 1) != '-' && tmpcalc.GetAt(i - 1) != '+' && tmpcalc.GetAt(i - 1) != '/' && tmpcalc.GetAt(i - 1) != '*')
			{
				operators++;
				if (operators == 2)
					end = i;
			}
		}
		if (end >= tmpcalc.Length() - 1 && operators > 0)
		{
			ScriptError("Syntax error in math operation: '" + tmpcalc + "' (might be nested)");
			CalcError = true;
			return "false";
		}
		if (operators > 1)
		{
			csString newdata;
			newdata = Calc(tmpcalc.Slice(0, end));

			if (CalcError == true)
				return newdata;

			//construct new string
			two = tmpcalc.Slice(end);
			tmpcalc = newdata + two;
		}
		else
			break;
	} while (1 == 1);

	//return value if none found
	if (operators == 0)
		return tmpcalc.GetData();

	//otherwise perform math
	temp1 = tmpcalc.Find("+", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.Slice(0, temp1);
		two = tmpcalc.Slice(temp1 + 1);
		if (IsNumeric(one.GetData()) == true && IsNumeric(two.GetData()) == true)
		{
			float tmpnum = atof(one.GetData()) + atof(two.GetData());
			tmpcalc = Simcore->TruncateNumber(tmpnum, 6);
			return tmpcalc.GetData();
		}
		else
		{
			/*if (IsNumeric(one.GetData()) == false)
				ScriptError("Syntax error in math operation: '" + one + "' is not numeric");
			else
				ScriptError("Syntax error in math operation: '" + two + "' is not numeric");
			CalcError = true;
			return "false";*/
		}
	}
	temp1 = tmpcalc.Find("-", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.Slice(0, temp1);
		two = tmpcalc.Slice(temp1 + 1);
		if (IsNumeric(one.GetData()) == true && IsNumeric(two.GetData()) == true)
		{
			float tmpnum = atof(one.GetData()) - atof(two.GetData());
			tmpcalc = Simcore->TruncateNumber(tmpnum, 6);
			return tmpcalc.GetData();
		}
		else
		{
			/*if (IsNumeric(one.GetData()) == false)
				ScriptError("Syntax error in math operation: '" + one + "' is not numeric");
			else
				ScriptError("Syntax error in math operation: '" + two + "' is not numeric");
			CalcError = true;
			return "false";*/
		}
	}
	temp1 = tmpcalc.Find("/", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.Slice(0, temp1);
		two = tmpcalc.Slice(temp1 + 1);
		if (IsNumeric(one.GetData()) == true && IsNumeric(two.GetData()) == true)
		{
			float tmpnum = atof(one.GetData()) / atof(two.GetData());
			tmpcalc = Simcore->TruncateNumber(tmpnum, 6);
			return tmpcalc.GetData();
		}
		else
		{
			/*if (IsNumeric(one.GetData()) == false)
				ScriptError("Syntax error in math operation: '" + one + "' is not numeric");
			else
				ScriptError("Syntax error in math operation: '" + two + "' is not numeric");
			CalcError = true;
			return "false";*/
		}
	}
	temp1 = tmpcalc.Find("*", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.Slice(0, temp1);
		two = tmpcalc.Slice(temp1 + 1);
		if (IsNumeric(one.GetData()) == true && IsNumeric(two.GetData()) == true)
		{
			float tmpnum = atof(one.GetData()) * atof(two.GetData());
			tmpcalc = Simcore->TruncateNumber(tmpnum, 6);
			return tmpcalc.GetData();
		}
		else
		{
			/*if (IsNumeric(one.GetData()) == false)
				ScriptError("Syntax error in math operation: '" + one + "' is not numeric");
			else
				ScriptError("Syntax error in math operation: '" + two + "' is not numeric");
			CalcError = true;
			return "false";*/
		}
	}
	return tmpcalc.GetData();
}

void ScriptProcessor::StoreCommand(Object *object)
{
	//store command and line info in object
	if (object)
	{
		object->command = BuildingData[line].Trim();
		object->command_processed = LineData;
		object->linenum = line + 1;
		object->context = Context;
		csString current;
		current = Current;
		if (Section == 2)
			object->context = "Floor " + current;
		if (Section == 4)
			object->context = "Elevator " + current;
	}
}

bool ScriptProcessor::FunctionProc()
{
	//process functions
	for (int i = 0; i < functions.GetSize(); i++)
	{
		int location = LineData.Find(functions[i].name + "(");
		if (location >= 0)
		{
			//found a function

			//store info
			InFunction = true;
			FunctionCallLine = line;

			//get function parameters
			int location2 = location + functions[i].name.Length() + 1;
			int end_loc = LineData.Find(")", location);
			csString newdata = LineData.Slice(location2, end_loc - location2);
			tempdata.DeleteAll();
			tempdata.SplitString(newdata, ",");

			//calculate inline math
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Calc(tempdata[temp3]);
				FunctionParams.Push(buffer.Trim());
			}

			//remove function statement
			LineData = LineData.Slice(0, location) + LineData.Slice(end_loc + 1);

			//switch to function line
			FunctionCallLineData = LineData;
			line = functions[i].line;
			return true;
		}
	}
	return false;
}

void ScriptProcessor::CheckFile(const char *filename, bool relative)
{
	//check to see if the specified file exists.
	//if not, add to nonexistent_files array

	csString file = filename;

	if (file == "")
		return;

	int loc = file.FindLast("/");
	if (loc > 0)
	{
		if (file.Length() == loc + 1)
			return;
	}
	loc = file.FindLast("\\");
	if (loc > 0)
	{
		if (file.Length() == loc + 1)
			return;
	}

	file.ReplaceAll("\\", "/");

	if (Simcore->FileExists(file, relative) == false)
	{
		file.ReplaceAll("/root/", "");
		nonexistent_files.PushSmart(file);
	}
}

int ScriptProcessor::SplitData(const char *string, int start, bool calc)
{
	//split data into separate strings starting at the "start" character
	//delimeter is a comma ","
	//returns the number of parameters found

	csString data = string;
	csString stringbuffer;
	tempdata.DeleteAll();
	tempdata.SplitString(data.Slice(start).GetData(), ",");

	for (int i = 0; i < tempdata.GetSize(); i++)
	{
		if (calc == true)
			stringbuffer = Calc(tempdata[i]);
		else
			stringbuffer = tempdata[i];
		tempdata.Put(i, stringbuffer.Trim());
	}
	return tempdata.GetSize();
}

int ScriptProcessor::SplitAfterEquals(const char *string, bool calc)
{
	//get and split data after equal sign
	//returns -1 if equal sign not found

	csString data = string;
	int loc = data.Find("=", 0);
	if (loc < 0)
		return -1;

	csString temp = data.Slice(loc + 1);
	temp.Trim();

	tempdata.DeleteAll();
	tempdata.SplitString(temp, ",");
	for (int i = 0; i < tempdata.GetSize(); i++)
	{
		csString buffer;
		if (calc == true)
			buffer = Calc(tempdata[i]);
		else
			buffer = tempdata[i];
		tempdata.Put(i, buffer.Trim());
	}
	return tempdata.GetSize();
}

csString ScriptProcessor::GetAfterEquals(const char *string)
{
	//return data after equal sign

	csString data = string;
	int loc = data.Find("=", 0);
	if (loc < 0)
		return "";

	csString temp = data.Slice(loc + 1);
	return temp.Trim();
}

