/* $Id$ */

/*
	Skyscraper 1.4 Alpha - File I/O and Script Processing Routines
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

#include <wx/wx.h>
#include <wx/variant.h>
#include "globals.h"
#include <stdlib.h>
#include "skyscraper.h"
#include "sbs.h"
#include "camera.h"
#include "floor.h"
#include "elevator.h"
#include "shaft.h"
#include "callbutton.h"
#include "unix.h"

extern SBS *Simcore;

bool Skyscraper::LoadBuilding(const char *filename)
{
	//building loader/script interpreter

	//load building into buffer
	if (!LoadDataFile(filename))
		return false;

	line = 0; //line number
	LineData = "";  //line contents
	Current = 0;
	Section = 0;
	Context = "None";
	int temp1 = 0;
	csString temp2 = "";
	int temp3 = 0;
	int temp4 = 0;
	int temp5 = 0;
	csString temp6 = "";
	csStringArray tempdata;
	csArray<int> callbutton_elevators;
	int FloorCheck = 0;
	int RangeL = 0;
	int RangeH = 0;
	long RangeStart = 0;
	csRef<iThingFactoryState> tmpMesh;
	char intbuffer[65];
	csString buffer;
	int startpos = 0;
	bool getfloordata = false;
	bool setshaftdoors = false;

	while (line < BuildingData.GetSize() - 1)
	{
		LineData = BuildingData[line];
		LineData.Trim();

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

		//Section information
		if (LineData.CompareNoCase("<globals>") == true)
		{
			if (Section > 0)
			{
				ScriptError("Already within a section");
				return false;
			}
			Section = 1;
			Context = "Globals";
			Report("Processing globals...");
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
			Report("Finished globals");
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
			Report("Processing floors " + csString(_itoa(RangeL, intbuffer, 10)) + " to " + csString(_itoa(RangeH, intbuffer, 10)) + "...");
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
			Context = "Floor";
			RangeL = 0;
			RangeH = 0;
			if (!IsNumeric(LineData.Slice(7, LineData.Length() - 8).Trim().GetData(), Current))
			{
				ScriptError("Invalid floor");
				return false;
			}
			Report("Processing floor " + csString(_itoa(Current, intbuffer, 10)) + "...");
			goto Nextline;
		}
		if (LineData.CompareNoCase("<endfloor>") == true)
		{
			if (Section != 2 || Context != "Floor")
			{
				ScriptError("Not in floor section");
				return false;
			}
			Section = 0;
			Context = "None";
			Report("Finished floor");
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
			Report("Processing elevators " + csString(_itoa(RangeL, intbuffer, 10)) + " to " + csString(_itoa(RangeH, intbuffer, 10)) + "...");
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
			Context = "Elevator";
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
			Report("Processing elevator " + csString(_itoa(Current, intbuffer, 10)) + "...");
			goto Nextline;
		}
		if (LineData.CompareNoCase("<endelevator>") == true)
		{
			if (Section != 4 || Context != "Elevator")
			{
				ScriptError("Not in elevator section");
				return false;
			}
			Section = 0;
			Context = "None";
			Report("Finished elevator");
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
			Report("Processing textures...");
			goto Nextline;
		}
		if (LineData.Slice(0, 13).CompareNoCase("<endtextures>") == true)
		{
			if (Section != 5)
			{
				ScriptError("Not in texture section");
				return false;
			}
			Section = 0;
			Context = "None";
			Report("Finished textures");
			goto Nextline;
		}
		if (LineData.Slice(0, 5).CompareNoCase("<end>") == true)
		{
			Section = 0;
			Context = "None";
			Report("Exiting building script");
			break; //exit data file parser
		}
		if (LineData.Slice(0, 7).CompareNoCase("<break>") == true)
		{
			//breakpoint function for debugging scripts
breakpoint:
			Report("Script breakpoint reached");
			goto Nextline;
		}

		startpos = 0;
		do
		{
			//User variable conversion
			temp1 = LineData.Find("%", startpos);
			if (temp1 > startpos)
			{
				temp3 = LineData.Find("%", temp1 + 1);
				if (temp3 < LineData.Length())
					startpos = temp3 + 1;
				else
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
					startpos -= 1;
				}
			}
		} while (1 == 1);

		//Floor object conversion
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
			if (temp4 < -Simcore->Basements || temp4 > Simcore->Floors - 1)
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
		}

		//Global commands

		//AddTriangleWall command
		if (LineData.Slice(0, 15).CompareNoCase("addtrianglewall") == true)
		{
			if (Section == 2 && getfloordata == false)
			{
				//process floor-specific variables if in a floor section
				getfloordata = true;
				goto recalc;
			}
			else
				getfloordata = false;

			//get data
			tempdata.SplitString(LineData.Slice(16).GetData(), ",");

			//calculate inline math
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer.Trim());
			}
			if (tempdata.GetSize() < 14 || tempdata.GetSize() > 14)
			{
				ScriptError("Incorrect number of parameters");
				return false;
			}

			//check numeric values
			for (int i = 3; i <= 13; i++)
			{
				if (!IsNumeric(tempdata[i]))
				{
					ScriptError("Invalid value: " + csString(tempdata[i]));
					return false;
				}
			}
			if (Section == 2)
			{
				 buffer = Simcore->GetFloor(Current)->Altitude + atof(tempdata[4]);
				 tempdata.Put(4, buffer);
				 buffer = Simcore->GetFloor(Current)->Altitude + Simcore->GetFloor(Current)->InterfloorHeight + atof(tempdata[7]);
				 tempdata.Put(7, buffer);
			}
			buffer = tempdata[0];
			buffer.Downcase();
			if (buffer == "floor")
				tmpMesh = Simcore->GetFloor(Current)->Level_state;
			else if (buffer == "external")
				tmpMesh = Simcore->External_state;
			else if (buffer == "landscape")
				tmpMesh = Simcore->Landscape_state;
			else if (buffer == "buildings")
				tmpMesh = Simcore->Buildings_state;
			else
			{
				ScriptError("Invalid object");
				return false;
			}

			//create triangle wall
			Simcore->AddTriangleWall(tmpMesh, tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12]), atof(tempdata[13]));
			tempdata.DeleteAll();
		}

		//AddWall command
		if (LineData.Slice(0, 7).CompareNoCase("addwall") == true && Section != 2 && Section != 4)
		{
			//get data
			tempdata.SplitString(LineData.Slice(8).GetData(), ",");

			//calculate inline math
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer.Trim());
			}
			if (tempdata.GetSize() < 14 || tempdata.GetSize() > 14)
			{
				ScriptError("Incorrect number of parameters");
				return false;
			}
			//check numeric values
			for (int i = 3; i <= 13; i++)
			{
				if (!IsNumeric(tempdata[i]))
				{
					ScriptError("Invalid value: " + csString(tempdata[i]));
					return false;
				}
			}

			//create wall
			Simcore->AddWall(tempdata[0], tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12]), atof(tempdata[13]));
			tempdata.DeleteAll();
		}

		//AddFloor
		if (LineData.Slice(0, 8).CompareNoCase("addfloor") == true && Section != 2 && Section != 4)
		{
			//get data
			tempdata.SplitString(LineData.Slice(9).GetData(), ",");

			//calculate inline math
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer.Trim());
			}
			if (tempdata.GetSize() < 12 || tempdata.GetSize() > 12)
			{
				ScriptError("Incorrect number of parameters");
				return false;
			}
			//check numeric values
			for (int i = 3; i <= 11; i++)
			{
				if (!IsNumeric(tempdata[i]))
				{
					ScriptError("Invalid value: " + csString(tempdata[i]));
					return false;
				}
			}

			//create floor
			Simcore->AddFloor(tempdata[0], tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]));
			tempdata.DeleteAll();
		}

		//AddGround
		if (LineData.Slice(0, 9).CompareNoCase("addground") == true)
		{
			//get data
			tempdata.SplitString(LineData.Slice(10).GetData(), ",");

			//calculate inline math
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer.Trim());
			}
			if (tempdata.GetSize() < 9 || tempdata.GetSize() > 9)
			{
				ScriptError("Incorrect number of parameters");
				return false;
			}
			//check numeric values
			for (int i = 2; i <= 8; i++)
			{
				if (!IsNumeric(tempdata[i]))
				{
					ScriptError("Invalid value: " + csString(tempdata[i]));
					return false;
				}
			}

			//create tiled ground
			Simcore->AddGround(tempdata[0], tempdata[1], atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atoi(tempdata[7]), atoi(tempdata[8]));
			tempdata.DeleteAll();
		}

		//Cut command
		if (LineData.Slice(0, 4).CompareNoCase("cut ") == true && Section != 2 && Section != 4)
		{
			//get data
			tempdata.SplitString(LineData.Slice(4).GetData(), ",");

			//calculate inline math
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer.Trim());
			}
			if (tempdata.GetSize() < 9 || tempdata.GetSize() > 9)
			{
				ScriptError("Incorrect number of parameters");
				return false;
			}
			//check numeric values
			for (int i = 1; i <= 6; i++)
			{
				if (!IsNumeric(tempdata[i]))
				{
					ScriptError("Invalid value: " + csString(tempdata[i]));
					return false;
				}
			}

			buffer = tempdata[0];
			buffer.Downcase();
			if (buffer == "external")
				tmpMesh = Simcore->External_state;
			else if (buffer == "landscape")
				tmpMesh = Simcore->Landscape_state;
			else if (buffer == "buildings")
				tmpMesh = Simcore->Buildings_state;
			else
			{
				ScriptError("Invalid object");
				return false;
			}

			//perform cut
			Simcore->Cut(tmpMesh, csVector3(atof(tempdata[1]), atof(tempdata[2]), atof(tempdata[3])), csVector3(atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6])), csString(tempdata[7]).CompareNoCase("true"), csString(tempdata[8]).CompareNoCase("true"), csVector3(0, 0, 0), csVector3(0, 0, 0));
			tempdata.DeleteAll();
		}

		//Set command
		if (LineData.Slice(0, 4).CompareNoCase("set ") == true && Section != 2 && Section != 4)
		{
			temp1 = LineData.Find("=", 0);
			temp3 = atoi(LineData.Slice(4, temp1 - 5));
			temp2 = LineData.Slice(temp1 + 1);
			if (temp3 < 0 || temp3 > UserVariable.GetSize() - 1)
			{
				ScriptError("Invalid variable number");
				return false;
			}
			UserVariable[temp3] = Calc(temp2);
			//Report("Variable " + csString(_itoa(temp3, intbuffer, 10)) + " set to " + Simcore->UserVariable[temp3]);
		}

		//CreateWallBox2 command
		if (LineData.Slice(0, 14).CompareNoCase("createwallbox2") == true)
		{
			if (Section == 2 && getfloordata == false)
			{
				//process floor-specific variables if in a floor section
				getfloordata = true;
				goto recalc;
			}
			else
				getfloordata = false;

			tempdata.SplitString(LineData.Slice(15).GetData(), ",");
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer.Trim());
			}
			if (tempdata.GetSize() < 15 || tempdata.GetSize() > 15)
			{
				ScriptError("Incorrect number of parameters");
				return false;
			}

			//check numeric values
			for (int i = 3; i <= 10; i++)
			{
				if (!IsNumeric(tempdata[i]))
				{
					ScriptError("Invalid value: " + csString(tempdata[i]));
					return false;
				}
			}

			if (Section == 2)
			{
				buffer = Simcore->GetFloor(Current)->Altitude + Simcore->GetFloor(Current)->InterfloorHeight + atof(tempdata[8]);
				tempdata.Put(8, buffer);
			}
			buffer = tempdata[0];
			buffer.Downcase();
			if (buffer == "floor")
				tmpMesh = Simcore->GetFloor(Current)->Level_state;
			else if (buffer == "external")
				tmpMesh = Simcore->External_state;
			else if (buffer == "landscape")
				tmpMesh = Simcore->Landscape_state;
			else if (buffer == "buildings")
				tmpMesh = Simcore->Buildings_state;
			else
			{
				ScriptError("Invalid object");
				return false;
			}

			Simcore->CreateWallBox2(tmpMesh, tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), csString(tempdata[11]).CompareNoCase("true"), csString(tempdata[12]).CompareNoCase("true"), csString(tempdata[13]).CompareNoCase("true"), csString(tempdata[14]).CompareNoCase("true"));
			tempdata.DeleteAll();
		}

		//CreateWallBox command
		if (LineData.Slice(0, 14).CompareNoCase("createwallbox ") == true)
		{
			if (Section == 2 && getfloordata == false)
			{
				//process floor-specific variables if in a floor section
				getfloordata = true;
				goto recalc;
			}
			else
				getfloordata = false;

			tempdata.SplitString(LineData.Slice(14).GetData(), ",");
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer.Trim());
			}
			if (tempdata.GetSize() < 15 || tempdata.GetSize() > 15)
			{
				ScriptError("Incorrect number of parameters");
				return false;
			}

			//check numeric values
			for (int i = 3; i <= 10; i++)
			{
				if (!IsNumeric(tempdata[i]))
				{
					ScriptError("Invalid value: " + csString(tempdata[i]));
					return false;
				}
			}

			if (Section == 2)
			{
				buffer = Simcore->GetFloor(Current)->Altitude + Simcore->GetFloor(Current)->InterfloorHeight + atof(tempdata[8]);
				tempdata.Put(8, buffer);
			}
			buffer = tempdata[0];
			buffer.Downcase();
			if (buffer == "floor")
				tmpMesh = Simcore->GetFloor(Current)->Level_state;
			else if (buffer == "external")
				tmpMesh = Simcore->External_state;
			else if (buffer == "landscape")
				tmpMesh = Simcore->Landscape_state;
			else if (buffer == "buildings")
				tmpMesh = Simcore->Buildings_state;
			else
			{
				ScriptError("Invalid object");
				return false;
			}

			Simcore->CreateWallBox(tmpMesh, tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), csString(tempdata[11]).CompareNoCase("true"), csString(tempdata[12]).CompareNoCase("true"), csString(tempdata[13]).CompareNoCase("true"), csString(tempdata[14]).CompareNoCase("true"));
			tempdata.DeleteAll();
		}

		//AddCustomWall command
		if (LineData.Slice(0, 14).CompareNoCase("addcustomwall ") == true)
		{
			if (Section == 2 && getfloordata == false)
			{
				//process floor-specific variables if in a floor section
				getfloordata = true;
				goto recalc;
			}
			else
				getfloordata = false;

			tempdata.SplitString(LineData.Slice(14).GetData(), ",");
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer.Trim());
			}

			//check numeric values
			for (int i = 3; i < tempdata.GetSize() - 2; i++)
			{
				if (!IsNumeric(tempdata[i]))
				{
					ScriptError("Invalid value: " + csString(tempdata[i]));
					return false;
				}
			}

			buffer = tempdata[0];
			buffer.Downcase();
			if (buffer == "floor")
				tmpMesh = Simcore->GetFloor(Current)->Level_state;
			else if (buffer == "external")
				tmpMesh = Simcore->External_state;
			else if (buffer == "landscape")
				tmpMesh = Simcore->Landscape_state;
			else if (buffer == "buildings")
				tmpMesh = Simcore->Buildings_state;
			else
			{
				ScriptError("Invalid object");
				return false;
			}

			csPoly3D varray;
			int alength;
			alength = tempdata.GetSize();
			for (temp3 = 3; temp3 < alength - 2; temp3 += 3)
				varray.AddVertex(atof(tempdata[temp3]), atof(tempdata[temp3 + 1]), atof(tempdata[temp3 + 2]));

			Simcore->AddCustomWall(tmpMesh, tempdata[1], tempdata[2], varray, atof(tempdata[alength - 2]), atof(tempdata[alength - 1]));
			tempdata.DeleteAll();
		}

		//AddCustomFloor command
		if (LineData.Slice(0, 15).CompareNoCase("addcustomfloor ") == true)
		{
			if (Section == 2 && getfloordata == false)
			{
				//process floor-specific variables if in a floor section
				getfloordata = true;
				goto recalc;
			}
			else
				getfloordata = false;

			tempdata.SplitString(LineData.Slice(15).GetData(), ",");
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer.Trim());
			}

			//check numeric values
			for (int i = 3; i < tempdata.GetSize() - 2; i++)
			{
				if (!IsNumeric(tempdata[i]))
				{
					ScriptError("Invalid value: " + csString(tempdata[i]));
					return false;
				}
			}

			buffer = tempdata[0];
			buffer.Downcase();
			if (buffer == "floor")
				tmpMesh = Simcore->GetFloor(Current)->Level_state;
			else if (buffer == "external")
				tmpMesh = Simcore->External_state;
			else if (buffer == "landscape")
				tmpMesh = Simcore->Landscape_state;
			else if (buffer == "buildings")
				tmpMesh = Simcore->Buildings_state;
			else
			{
				ScriptError("Invalid object");
				return false;
			}

			csPoly3D varray;
			int alength;
			alength = tempdata.GetSize();
			for (temp3 = 3; temp3 < alength - 2; temp3 += 3)
				varray.AddVertex(atof(tempdata[temp3]), atof(tempdata[temp3 + 1]), atof(tempdata[temp3 + 2]));

			Simcore->AddCustomFloor(tmpMesh, tempdata[1], tempdata[2], varray, atof(tempdata[alength - 2]), atof(tempdata[alength - 1]));

			tempdata.DeleteAll();
		}

		//AddShaft command
		if (LineData.Slice(0, 9).CompareNoCase("addshaft ") == true)
		{
			tempdata.SplitString(LineData.Slice(9).GetData(), ",");
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer.Trim());
			}
			if (tempdata.GetSize() < 6 || tempdata.GetSize() > 6)
			{
				ScriptError("Incorrect number of parameters");
				return false;
			}

			//check numeric values
			for (int i = 0; i <= 5; i++)
			{
				if (!IsNumeric(tempdata[i]))
				{
					ScriptError("Invalid value: " + csString(tempdata[i]));
					return false;
				}
			}

			if (atoi(tempdata[4]) < -Simcore->Basements)
			{
				ScriptError("Invalid starting floor");
				return false;
			}
			if (atoi(tempdata[5]) > Simcore->Floors - 1)
			{
				ScriptError("Invalid ending floor");
				return false;
			}


			Simcore->CreateShaft(atoi(tempdata[0]), atoi(tempdata[1]), atof(tempdata[2]), atof(tempdata[3]), atoi(tempdata[4]), atoi(tempdata[5]));

			tempdata.DeleteAll();
		}

		//ShaftCut command
		if (LineData.Slice(0, 9).CompareNoCase("shaftcut ") == true)
		{
			tempdata.SplitString(LineData.Slice(9).GetData(), ",");
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer.Trim());
			}
			if (tempdata.GetSize() < 7 || tempdata.GetSize() > 7)
			{
				ScriptError("Incorrect number of parameters");
				return false;
			}

			//check numeric values
			for (int i = 0; i <= 6; i++)
			{
				if (!IsNumeric(tempdata[i]))
				{
					ScriptError("Invalid value: " + csString(tempdata[i]));
					return false;
				}
			}

			Simcore->GetShaft(atoi(tempdata[0]))->CutFloors(true, csVector2(atof(tempdata[1]), atof(tempdata[2])), csVector2(atof(tempdata[3]), atof(tempdata[4])), atof(tempdata[5]), atof(tempdata[6]));

			tempdata.DeleteAll();
		}

		//ShaftShowFloors command
		if (LineData.Slice(0, 15).CompareNoCase("shaftshowfloors") == true)
		{
			//get shaft number
			int loc = LineData.Find("=");
			if (loc < 0)
			{
				ScriptError("Syntax error");
				return false;
			}
			int shaftnum;
			if (!IsNumeric(LineData.Slice(15, loc - 16).Trim().GetData(), shaftnum))
			{
				ScriptError("Invalid shaft number");
				return false;
			}
			if (shaftnum < 1 || shaftnum > Simcore->Shafts())
			{
				ScriptError("Invalid shaft number");
				return false;
			}
			Simcore->GetShaft(shaftnum)->ShowFloors = true;

			//get text after equal sign
			temp2 = LineData.Slice(loc + 1);
			temp2.Trim();

			//copy string listing of floors into array
			tempdata.SplitString(temp2.GetData(), ",");
			for (int line = 0; line < tempdata.GetSize(); line++)
			{
				csString tmpstring = tempdata[line];
				tmpstring.Trim();
				if (tmpstring.Find("-", 1) > 0)
				{
					int start, end;
					//found a range marker
					if (!IsNumeric(tmpstring.Slice(0, tmpstring.Find("-", 1)).Trim().GetData(), start) || !IsNumeric(tmpstring.Slice(tmpstring.Find("-", 1) + 1).Trim().GetData(), end))
					{
						ScriptError("Invalid value");
						return false;
					}
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
					{
						ScriptError("Invalid value");
						return false;
					}
					Simcore->GetShaft(shaftnum)->AddShowFloor(showfloor);
				}
			}
			tempdata.DeleteAll();
		}

		//ShaftShowOutside command
		if (LineData.Slice(0, 16).CompareNoCase("shaftshowoutside") == true)
		{
			//get shaft number
			int loc = LineData.Find("=");
			if (loc < 0)
			{
				ScriptError("Syntax error");
				return false;
			}
			int shaftnum;
			if (!IsNumeric(LineData.Slice(16, loc - 17).Trim().GetData(), shaftnum))
			{
				ScriptError("Invalid shaft number");
				return false;
			}
			if (shaftnum < 1 || shaftnum > Simcore->Shafts())
			{
				ScriptError("Invalid shaft number");
				return false;
			}
			Simcore->GetShaft(shaftnum)->ShowOutside = true;

			//get text after equal sign
			temp2 = LineData.Slice(loc + 1);
			temp2.Trim();

			//copy string listing of floors into array
			tempdata.SplitString(temp2.GetData(), ",");
			for (int line = 0; line < tempdata.GetSize(); line++)
			{
				csString tmpstring = tempdata[line];
				tmpstring.Trim();
				if (tmpstring.Find("-", 1) > 0)
				{
					int start, end;
					//found a range marker
					if (!IsNumeric(tmpstring.Slice(0, tmpstring.Find("-", 1)).Trim().GetData(), start) || !IsNumeric(tmpstring.Slice(tmpstring.Find("-", 1) + 1).Trim().GetData(), end))
					{
						ScriptError("Invalid value");
						return false;
					}
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
					{
						ScriptError("Invalid value");
						return false;
					}
					Simcore->GetShaft(shaftnum)->AddShowOutside(showfloor);
				}
			}
			tempdata.DeleteAll();
		}

		//ShowFullShaft command
		if (LineData.Slice(0, 13).CompareNoCase("showfullshaft") == true)
		{
			//get shaft number
			int loc = LineData.Find("=");
			if (loc < 0)
			{
				ScriptError("Syntax error");
				return false;
			}
			int shaftnum;
			if (!IsNumeric(LineData.Slice(13, loc - 14).Trim().GetData(), shaftnum))
			{
				ScriptError("Invalid shaft number");
				return false;
			}
			if (shaftnum < 1 || shaftnum > Simcore->Shafts())
			{
				ScriptError("Invalid shaft number");
				return false;
			}

			//get text after equal sign
			temp2 = LineData.Slice(loc + 1);
			temp2.Trim();

			Simcore->GetShaft(shaftnum)->ShowFullShaft = csString(temp2).CompareNoCase("true");
		}

		//CreateStairwell command
		if (LineData.Slice(0, 15).CompareNoCase("createstairwell") == true)
		{
			tempdata.SplitString(LineData.Slice(16).GetData(), ",");
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer);
			}
			if (tempdata.GetSize() < 5 || tempdata.GetSize() > 5)
			{
				ScriptError("Incorrect number of parameters");
				return false;
			}

			//check numeric values
			for (int i = 0; i <= 4; i++)
			{
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				{
					ScriptError("Invalid value: " + csString(tempdata[i]));
					return false;
				}
			}

			if (!Simcore->GetStairs(atoi(tempdata[0])))
				Simcore->CreateStairwell(atoi(tempdata[0]), atof(tempdata[1]), atof(tempdata[2]), atoi(tempdata[3]), atoi(tempdata[4]));
			else
			{
				ScriptError("Stairwell " + csString(tempdata[0]).Trim() + " already exists");
				return false;
			}

			tempdata.DeleteAll();
		}

		//CutStairwell command
		if (LineData.Slice(0, 13).CompareNoCase("cutstairwell ") == true)
		{
			tempdata.SplitString(LineData.Slice(13).GetData(), ",");
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer);
			}
			if (tempdata.GetSize() < 7 || tempdata.GetSize() > 7)
			{
				ScriptError("Incorrect number of parameters");
				return false;
			}

			//check numeric values
			for (int i = 0; i <= 6; i++)
			{
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				{
					ScriptError("Invalid value: " + csString(tempdata[i]));
					return false;
				}
			}

			int stairwell = atoi(tempdata[0]);
			if (!Simcore->GetStairs(stairwell))
			{
				ScriptError("Invalid stairwell");
				return false;
			}

			Simcore->GetStairs(stairwell)->CutFloors(true, csVector2(atof(tempdata[1]), atof(tempdata[2])), csVector2(atof(tempdata[3]), atof(tempdata[4])), atof(tempdata[5]), atof(tempdata[6]));

			tempdata.DeleteAll();
		}

		//WallOrientation command
		if (LineData.Slice(0, 15).CompareNoCase("wallorientation") == true)
		{
			//get text after equal sign
			temp2 = LineData.Slice(LineData.Find("=", 0) + 1);
			temp2.Trim();

			if (!Simcore->SetWallOrientation(temp2.GetData()))
			{
				ScriptError("Invalid wall orientation");
				return false;
			}
		}

		//FloorOrientation command
		if (LineData.Slice(0, 16).CompareNoCase("floororientation") == true)
		{
			//get text after equal sign
			temp2 = LineData.Slice(LineData.Find("=", 0) + 1);
			temp2.Trim();

			if (!Simcore->SetFloorOrientation(temp2.GetData()))
			{
				ScriptError("Invalid floor orientation");
				return false;
			}
		}

		//DrawWalls command
		if (LineData.Slice(0, 9).CompareNoCase("drawwalls") == true)
		{
			//get text after equal sign
			int loc = LineData.Find("=", 0);
			if (loc < 0)
			{
				ScriptError("Syntax error");
				return false;
			}
			temp2 = LineData.Slice(loc + 1);
			temp2.Trim();

			tempdata.SplitString(temp2.GetData(), ",");
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer);
			}
			if (tempdata.GetSize() < 6 || tempdata.GetSize() > 6)
			{
				ScriptError("Incorrect number of parameters");
				return false;
			}

			Simcore->DrawWalls(csString(tempdata[0]).CompareNoCase("true"),
						csString(tempdata[1]).CompareNoCase("true"),
						csString(tempdata[2]).CompareNoCase("true"),
						csString(tempdata[3]).CompareNoCase("true"),
						csString(tempdata[4]).CompareNoCase("true"),
						csString(tempdata[5]).CompareNoCase("true"));

			tempdata.DeleteAll();
		}

		//ReverseExtents command
		if (LineData.Slice(0, 14).CompareNoCase("reverseextents") == true)
		{
			tempdata.SplitString(LineData.Slice(15).GetData(), ",");
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer);
			}
			if (tempdata.GetSize() < 3 || tempdata.GetSize() > 3)
			{
				ScriptError("Incorrect number of parameters");
				return false;
			}

			Simcore->ReverseExtents(csString(tempdata[0]).CompareNoCase("true"),
						csString(tempdata[1]).CompareNoCase("true"),
						csString(tempdata[2]).CompareNoCase("true"));

			tempdata.DeleteAll();
		}

		//ReverseAxis command
		if (LineData.Slice(0, 11).CompareNoCase("reverseaxis") == true)
		{
			//get text after equal sign
			int loc = LineData.Find("=", 0);
			if (loc < 0)
			{
				ScriptError("Syntax error");
				return false;
			}
			temp2 = LineData.Slice(loc + 1);
			temp2.Trim();

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
				goto recalc;
			}
			else
				getfloordata = false;

			temp1 = LineData.Find("(", 0);
			temp3 = LineData.Find(")", 0);
			if (temp3 < 0)
			{
				ScriptError("Syntax error");
				return false;
			}
			tempdata.SplitString(LineData.Slice(temp1 + 1, temp3 - temp1 - 1).GetData(), ",");
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer);
			}
			if (tempdata.GetSize() < 8 || tempdata.GetSize() > 8)
			{
				ScriptError("Incorrect number of parameters");
				return false;
			}

			//check numeric values
			for (int i = 2; i <= 7; i++)
			{
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				{
					ScriptError("Invalid value: " + csString(tempdata[i]));
					return false;
				}
			}

			buffer = tempdata[0];
			buffer.Downcase();
			if (buffer == "floor")
				tmpMesh = Simcore->GetFloor(Current)->Level_state;
			else if (buffer == "external")
				tmpMesh = Simcore->External_state;
			else if (buffer == "landscape")
				tmpMesh = Simcore->Landscape_state;
			else if (buffer == "buildings")
				tmpMesh = Simcore->Buildings_state;
			else
			{
				ScriptError("Invalid object");
				return false;
			}

			csVector3 isect = Simcore->GetPoint(tmpMesh, tempdata[1], csVector3(atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4])), csVector3(atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7])));
			tempdata.DeleteAll();

			buffer = csString(LineData).Slice(0, temp5 - 1) + csString(wxVariant(isect.x).GetString().ToAscii()) + csString(wxVariant(isect.y).GetString().ToAscii()) + csString(wxVariant(isect.z).GetString().ToAscii()) + csString(LineData).Slice(temp3 + 1);
			LineData = buffer.GetData();

		}

		//SetAutoSize command
		if (LineData.Slice(0, 11).CompareNoCase("setautosize") == true)
		{
			//get text after equal sign
			int loc = LineData.Find("=", 0);
			if (loc < 0)
			{
				ScriptError("Syntax error");
				return false;
			}
			temp2 = LineData.Slice(loc + 1);
			temp2.Trim();

			tempdata.SplitString(temp2.GetData(), ",");
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer);
			}
			if (tempdata.GetSize() < 2 || tempdata.GetSize() > 2)
			{
				ScriptError("Incorrect number of parameters");
				return false;
			}

			Simcore->SetAutoSize(csString(tempdata[0]).CompareNoCase("true"),
						csString(tempdata[1]).CompareNoCase("true"));

			tempdata.DeleteAll();
		}

		//TextureOverride command
		if (LineData.Slice(0, 15).CompareNoCase("textureoverride") == true)
		{
			tempdata.SplitString(LineData.Slice(16).GetData(), ",");

			if (tempdata.GetSize() < 6 || tempdata.GetSize() > 6)
			{
				ScriptError("Incorrect number of parameters");
				return false;
			}

			Simcore->SetTextureOverride(tempdata[0], tempdata[1], tempdata[2], tempdata[3], tempdata[4], tempdata[5]);

			tempdata.DeleteAll();
			goto Nextline;
		}

		//TextureFlip command
		if (LineData.Slice(0, 11).CompareNoCase("textureflip") == true)
		{
			tempdata.SplitString(LineData.Slice(12).GetData(), ",");

			if (tempdata.GetSize() < 6 || tempdata.GetSize() > 6)
			{
				ScriptError("Incorrect number of parameters");
				return false;
			}

			//check numeric values
			for (int i = 0; i <= 5; i++)
			{
				if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
				{
					ScriptError("Invalid value: " + csString(tempdata[i]));
					return false;
				}
			}

			Simcore->SetTextureFlip(atoi(tempdata[0]), atoi(tempdata[1]), atoi(tempdata[2]), atoi(tempdata[3]), atoi(tempdata[4]), atoi(tempdata[5]));

			tempdata.DeleteAll();
			goto Nextline;
		}

		if (LineData.Slice(0, 5).CompareNoCase("mount") == true)
		{
			tempdata.SplitString(LineData.Slice(6).GetData(), ",");
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = tempdata[temp3];
				tempdata.Put(temp3, buffer.Trim());
			}
			if (tempdata.GetSize() < 2 || tempdata.GetSize() > 2)
			{
				ScriptError("Incorrect number of parameters");
				return false;
			}

			buffer = tempdata[1];
			buffer.Insert(0, "/root/");
			if (!Simcore->Mount(tempdata[0], buffer))
			{
					ScriptError("Error mounting file");
					return false;
			}

			tempdata.DeleteAll();
			goto Nextline;
		}

		//Process globals
		if (Section == 1)
		{
			//get text after equal sign
			int loc = LineData.Find("=", 0);
			if (loc < 0)
			{
				ScriptError("Syntax error");
				return false;
			}
			temp2 = LineData.Slice(loc + 1);
			temp2.Trim();

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
				{
					ScriptError("Invalid floor");
					return false;
				}
				Simcore->camera->StartFloor = data;
			}
			if (LineData.Slice(0, 10).CompareNoCase("horizscale") == true)
			{
				float data;
				if (!IsNumeric(temp2.GetData(), data))
				{
					ScriptError("Invalid scale");
					return false;
				}
				Simcore->HorizScale = data;
			}
			if (LineData.Slice(0, 14).CompareNoCase("cameraposition") == true)
			{
				float x, z;
				if (!IsNumeric(temp2.Slice(0, temp2.Find(",", 0)).Trim().GetData(), x) || !IsNumeric(temp2.Slice(temp2.Find(",", 0) + 1).Trim().GetData(), z))
				{
					ScriptError("Invalid position");
					return false;
				}
				Simcore->camera->StartPositionX  = x;
				Simcore->camera->StartPositionZ  = z;
			}
			if (LineData.Slice(0, 15).CompareNoCase("cameradirection") == true)
			{
				temp3 = temp2.Find(",", 0);
				temp4 = temp2.Find(",", temp3 + 1);
				float x, y, z;
				if (!IsNumeric(temp2.Slice(0, temp3).Trim().GetData(), x) || !IsNumeric(temp2.Slice(temp3 + 1, temp4 - temp3 - 1).Trim().GetData(), y) || !IsNumeric(temp2.Slice(temp4 + 1).Trim().GetData(), z)){
					ScriptError("Invalid direction");
					return false;
				}
				Simcore->camera->SetStartDirection(csVector3(x, y, z));
			}
			if (LineData.Slice(0, 14).CompareNoCase("camerarotation") == true)
			{
				temp3 = temp2.Find(",", 0);
				temp4 = temp2.Find(",", temp3 + 1);
				float x, y, z;
				if (!IsNumeric(temp2.Slice(0, temp3).Trim().GetData(), x) || !IsNumeric(temp2.Slice(temp3 + 1, temp4 - temp3 - 1).Trim().GetData(), y) || !IsNumeric(temp2.Slice(temp4 + 1).Trim().GetData(), z)){
					ScriptError("Invalid direction");
					return false;
				}
				Simcore->camera->SetStartRotation(csVector3(x, y, z));
			}
		}

		//Process floors
		if (Section == 2)
		{

			//create floor if not created already
			Simcore->NewFloor(Current);
recalc:
			//replace variables with actual values
			buffer = Current;
			LineData.ReplaceAll("%floor%", buffer);
			buffer = Simcore->GetFloor(Current)->Height;
			LineData.ReplaceAll("%height%", buffer);
			buffer = Simcore->GetFloor(Current)->FullHeight();
			LineData.ReplaceAll("%fullheight%", buffer);
			buffer = Simcore->GetFloor(Current)->InterfloorHeight;
			LineData.ReplaceAll("%interfloorheight%", buffer);

			if (getfloordata == true)
				goto checkfloors;

			//get text after equal sign
			int temp2check = LineData.Find("=", 0);
			temp2 = LineData.Slice(temp2check + 1);
			temp2.Trim();

			//parameters
			if (LineData.Slice(0, 6).CompareNoCase("height") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				if (!IsNumeric(Calc(temp2.GetData()).Trim().GetData(), Simcore->GetFloor(Current)->Height))
				{
					ScriptError("Invalid value");
					return false;
				}
				if (FloorCheck < 2)
					FloorCheck = 1;
				else
					FloorCheck = 3;
			}
			if (LineData.Slice(0, 16).CompareNoCase("interfloorheight") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				if (!IsNumeric(Calc(temp2.GetData()).Trim().GetData(), Simcore->GetFloor(Current)->InterfloorHeight))
				{
					ScriptError("Invalid value");
					return false;
				}
				if (FloorCheck == 0)
					FloorCheck = 2;
				else
					FloorCheck = 3;
			}
			if (LineData.Slice(0, 8).CompareNoCase("altitude") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				if (!IsNumeric(Calc(temp2.GetData()).Trim().GetData(), Simcore->GetFloor(Current)->Altitude))
				{
					ScriptError("Invalid value");
					return false;
				}
			}
			if (LineData.Slice(0, 2).CompareNoCase("id") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				Simcore->GetFloor(Current)->ID = Calc(temp2);
			}
			if (LineData.Slice(0, 4).CompareNoCase("name") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				Simcore->GetFloor(Current)->Name = Calc(temp2);
			}
			if (LineData.Slice(0, 4).CompareNoCase("type") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				Simcore->GetFloor(Current)->FloorType = temp2;
			}
			if (LineData.Slice(0, 11).CompareNoCase("description") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				Simcore->GetFloor(Current)->Description = temp2;
			}
			if (LineData.Slice(0, 16).CompareNoCase("indicatortexture") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				Simcore->GetFloor(Current)->IndicatorTexture = Calc(temp2);
			}
			if (LineData.Slice(0, 5).CompareNoCase("group") == true)
			{
				//copy string listing of group floors into array
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				tempdata.SplitString(temp2.GetData(), ",");
				for (int line = 0; line < tempdata.GetSize(); line++)
				{
					csString tmpstring = tempdata[line];
					tmpstring.Trim();
					if (tmpstring.Find("-", 1) > 0)
					{
						int start, end;
						//found a range marker
						if (!IsNumeric(tmpstring.Slice(0, tmpstring.Find("-", 1)).Trim().GetData(), start) || !IsNumeric(tmpstring.Slice(tmpstring.Find("-", 1) + 1).Trim().GetData(), end))
						{
							ScriptError("Invalid value");
							return false;
						}
						if (end < start)
						{
							int temp = start;
							start = end;
							end = temp;
						}

						for (int k = start; k <= end; k++)
							Simcore->GetFloor(Current)->AddGroupFloor(k);
					}
					else
					{
						int data;
						if (!IsNumeric(csString(tempdata[line]).Trim().GetData(), data))
						{
							ScriptError("Invalid value");
							return false;
						}
						Simcore->GetFloor(Current)->AddGroupFloor(data);
					}
				}
				tempdata.DeleteAll();
			}

			//calculate altitude
			if (FloorCheck == 3)
			{
				FloorCheck = 0;
				Simcore->GetFloor(Current)->CalculateAltitude();
			}

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
					goto recalc;
				}
				else
					goto Nextline; //skip line
			}

			//Exit command
			if (LineData.Slice(0, 4).CompareNoCase("exit") == true)
			{
				if (RangeL != RangeH)
					LineData = "<endfloors>";
				else
					LineData = "<endfloor>";
			}

			if (LineData.Slice(0, 7).CompareNoCase("<break>") == true)
				goto breakpoint;

			//AddFloor command
			if (LineData.Slice(0, 9).CompareNoCase("addfloor ") == true)
			{
				//get data
				tempdata.SplitString(LineData.Slice(9).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				if (tempdata.GetSize() < 12 || tempdata.GetSize() > 12)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}
				//check numeric values
				for (int i = 2; i <= 10; i++)
				{
					if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					{
						ScriptError("Invalid value: " + csString(tempdata[i]));
						return false;
					}
				}

				//create floor
				Simcore->GetFloor(Current)->AddFloor(tempdata[0], tempdata[1], atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), csString(tempdata[11]).CompareNoCase("true"));

				tempdata.DeleteAll();
			}

			//AddShaftFloor command
			if (LineData.Slice(0, 13).CompareNoCase("addshaftfloor") == true)
			{
				//get data
				tempdata.SplitString(LineData.Slice(14).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				if (tempdata.GetSize() < 12 || tempdata.GetSize() > 12)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}
				//check numeric values
				for (int i = 3; i <= 11; i++)
				{
					if (i == 1)
						i = 3; //skip non-numeric parameters
					if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					{
						ScriptError("Invalid value: " + csString(tempdata[i]));
						return false;
					}
				}

				//create floor
				if (Simcore->GetShaft(atoi(tempdata[0])))
					Simcore->GetShaft(atoi(tempdata[0]))->AddFloor(Current, tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]));
				else
				{
					ScriptError("Invalid shaft");
					return false;
				}

				tempdata.DeleteAll();
			}

			//AddStairsFloor command
			if (LineData.Slice(0, 14).CompareNoCase("addstairsfloor") == true)
			{
				//get data
				tempdata.SplitString(LineData.Slice(15).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				if (tempdata.GetSize() < 12 || tempdata.GetSize() > 12)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}
				//check numeric values
				for (int i = 0; i <= 11; i++)
				{
					if (i == 1)
						i = 3; //skip non-numeric parameters
					if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					{
						ScriptError("Invalid value: " + csString(tempdata[i]));
						return false;
					}
				}

				//create floor
				if (Simcore->GetStairs(atoi(tempdata[0])))
					Simcore->GetStairs(atoi(tempdata[0]))->AddFloor(Current, tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]));
				else
				{
					ScriptError("Invalid stairwell");
					return false;
				}

				tempdata.DeleteAll();
			}

			//AddInterFloorFloor command
			if (LineData.Slice(0, 18).CompareNoCase("addinterfloorfloor") == true)
			{
				//get data
				tempdata.SplitString(LineData.Slice(19).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				if (tempdata.GetSize() < 11 || tempdata.GetSize() > 11)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}
				//check numeric values
				for (int i = 2; i <= 10; i++)
				{
					if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					{
						ScriptError("Invalid value: " + csString(tempdata[i]));
						return false;
					}
				}

				//create floor
				Simcore->GetFloor(Current)->AddInterfloorFloor(tempdata[0], tempdata[1], atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]));
				tempdata.DeleteAll();
			}

			//AddWall command
			if (LineData.Slice(0, 7).CompareNoCase("addwall") == true)
			{
				//get data
				tempdata.SplitString(LineData.Slice(8).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				if (tempdata.GetSize() < 14 || tempdata.GetSize() > 14)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}
				//check numeric values
				for (int i = 2; i <= 12; i++)
				{
					if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					{
						ScriptError("Invalid value: " + csString(tempdata[i]));
						return false;
					}
				}

				//create wall
				Simcore->GetFloor(Current)->AddWall(tempdata[0], tempdata[1], atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12]), csString(tempdata[13]).CompareNoCase("true"));

				tempdata.DeleteAll();
			}

			//AddShaftWall command
			if (LineData.Slice(0, 12).CompareNoCase("addshaftwall") == true)
			{
				//get data
				tempdata.SplitString(LineData.Slice(13).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				if (tempdata.GetSize() < 14 || tempdata.GetSize() > 14)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}
				//check numeric values
				for (int i = 0; i <= 13; i++)
				{
					if (i == 1)
						i = 3; //skip non-numeric parameters
					if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					{
						ScriptError("Invalid value: " + csString(tempdata[i]));
						return false;
					}
				}

				//create wall
				if (Simcore->GetShaft(atoi(tempdata[0])))
					Simcore->GetShaft(atoi(tempdata[0]))->AddWall(Current, tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12]), atof(tempdata[13]));
				else
				{
					ScriptError("Invalid shaft");
					return false;
				}

				tempdata.DeleteAll();
			}

			//AddStairsWall command
			if (LineData.Slice(0, 13).CompareNoCase("addstairswall") == true)
			{
				//get data
				tempdata.SplitString(LineData.Slice(14).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				if (tempdata.GetSize() < 14 || tempdata.GetSize() > 14)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}
				//check numeric values
				for (int i = 0; i <= 13; i++)
				{
					if (i == 1)
						i = 3; //skip non-numeric parameters
					if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					{
						ScriptError("Invalid value: " + csString(tempdata[i]));
						return false;
					}
				}

				//create wall
				if (Simcore->GetStairs(atoi(tempdata[0])))
					Simcore->GetStairs(atoi(tempdata[0]))->AddWall(Current, tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12]), atof(tempdata[13]));
				else
				{
					ScriptError("Invalid stairwell");
					return false;
				}

				tempdata.DeleteAll();
			}

			//AddInterFloorWall command
			if (LineData.Slice(0, 17).CompareNoCase("addinterfloorwall") == true)
			{
				//get data
				tempdata.SplitString(LineData.Slice(18).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				if (tempdata.GetSize() < 13 || tempdata.GetSize() > 13)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}
				//check numeric values
				for (int i = 2; i <= 12; i++)
				{
					if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					{
						ScriptError("Invalid value: " + csString(tempdata[i]));
						return false;
					}
				}

				//create wall
				Simcore->GetFloor(Current)->AddInterfloorWall(tempdata[0], tempdata[1], atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12]));
				tempdata.DeleteAll();
			}

			//ColumnWallBox command
			if (LineData.Slice(0, 14).CompareNoCase("columnwallbox ") == true)
			{
				tempdata.SplitString(LineData.Slice(14).GetData(), ",");
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				if (tempdata.GetSize() < 14 || tempdata.GetSize() > 14)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}
				//check numeric values
				for (int i = 2; i <= 9; i++)
				{
					if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					{
						ScriptError("Invalid value: " + csString(tempdata[i]));
						return false;
					}
				}

				Simcore->GetFloor(Current)->ColumnWallBox(tempdata[0], tempdata[1], atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), csString(tempdata[10]).CompareNoCase("true"), csString(tempdata[11]).CompareNoCase("true"), csString(tempdata[12]).CompareNoCase("true"), csString(tempdata[13]).CompareNoCase("true"));
				tempdata.DeleteAll();
			}

			//ColumnWallBox2 command
			if (LineData.Slice(0, 14).CompareNoCase("columnwallbox2") == true)
			{
				tempdata.SplitString(LineData.Slice(15).GetData(), ",");
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				if (tempdata.GetSize() < 14 || tempdata.GetSize() > 14)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}
				//check numeric values
				for (int i = 2; i <= 9; i++)
				{
					if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					{
						ScriptError("Invalid value: " + csString(tempdata[i]));
						return false;
					}
				}

				Simcore->GetFloor(Current)->ColumnWallBox2(tempdata[0], tempdata[1], atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), csString(tempdata[10]).CompareNoCase("true"), csString(tempdata[11]).CompareNoCase("true"), csString(tempdata[12]).CompareNoCase("true"), csString(tempdata[13]).CompareNoCase("true"));
				tempdata.DeleteAll();
			}

			//Set command
			if (LineData.Slice(0, 4).CompareNoCase("set ") == true)
			{
				temp1 = LineData.Find("=", 0);
				if (temp1 < 0)
				{
					ScriptError("Syntax Error");
					return false;
				}
				if (!IsNumeric(LineData.Slice(4, temp1 - 5).Trim().GetData(), temp3))
				{
					ScriptError("Invalid variable number");
					return false;
				}
				temp2 = LineData.Slice(temp1 + 1);
				if (temp3 < 0 || temp3 > UserVariable.GetSize() - 1)
				{
					ScriptError("Invalid variable number");
					return false;
				}
				UserVariable[temp3] = Calc(temp2);
				//Report("Variable " + csString(_itoa(temp3, intbuffer, 10)) + " set to " + Simcore->UserVariable[temp3]);
			}

			//CallButtonElevators command
			if (LineData.Slice(0, 19).CompareNoCase("callbuttonelevators") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				//construct array containing floor numbers
				tempdata.SplitString(temp2.GetData(), ",");
				callbutton_elevators.DeleteAll();
				callbutton_elevators.SetSize(tempdata.GetSize());

				for (int line = 0; line < tempdata.GetSize(); line++)
				{
					int elevnumber;
					if (!IsNumeric(csString(tempdata[line]).Trim().GetData(), elevnumber))
					{
						ScriptError("Invalid elevator number");
						return false;
					}
					if (elevnumber < 1 || elevnumber > Simcore->Elevators())
					{
						ScriptError("Invalid elevator number");
						return false;
					}
					callbutton_elevators[line] = elevnumber;
				}
				tempdata.DeleteAll();
			}

			//CreateCallButtons command
			if (LineData.Slice(0, 17).CompareNoCase("createcallbuttons") == true)
			{
				if (callbutton_elevators.GetSize() == 0)
				{
					ScriptError("No elevators specified");
					return false;
				}

				tempdata.SplitString(LineData.Slice(18).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				bool compatibility = false;
				if (tempdata.GetSize() == 12)
					compatibility = true;

				if (compatibility == true)
				{
					//check numeric values
					for (int i = 3; i <= 11; i++)
					{
						if (i == 6 || i == 9) //skip non-numeric parameters
							i++;
						if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
						{
							ScriptError("Invalid value: " + csString(tempdata[i]));
							return false;
						}
					}
				}
				else
				{
					if (tempdata.GetSize() < 14 || tempdata.GetSize() > 14)
					{
						ScriptError("Incorrect number of parameters");
						return false;
					}

					//check numeric values
					for (int i = 5; i <= 13; i++)
					{
						if (i == 8 || i == 11) //skip non-numeric parameters
							i++;
						if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
						{
							ScriptError("Invalid value: " + csString(tempdata[i]));
							return false;
						}
					}
				}

				//create call button
				if (compatibility == true)
					Simcore->GetFloor(Current)->AddCallButtons(callbutton_elevators, tempdata[0], tempdata[1], tempdata[1], tempdata[2], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), tempdata[6], atof(tempdata[7]), atof(tempdata[8]), csString(tempdata[9]).CompareNoCase("true"), atof(tempdata[10]), atof(tempdata[11]));
				else
					Simcore->GetFloor(Current)->AddCallButtons(callbutton_elevators, tempdata[0], tempdata[1], tempdata[2], tempdata[3], tempdata[4], atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), tempdata[8], atof(tempdata[9]), atof(tempdata[10]), csString(tempdata[11]).CompareNoCase("true"), atof(tempdata[12]), atof(tempdata[13]));

				tempdata.DeleteAll();
			}

			//AddStairs command
			if (LineData.Slice(0, 10).CompareNoCase("addstairs ") == true)
			{
				//get data
				tempdata.SplitString(LineData.Slice(10).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				if (tempdata.GetSize() < 13 || tempdata.GetSize() > 13)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}
				//check numeric values
				for (int i = 0; i <= 12; i++)
				{
					if (i == 1)
						i = 4; //skip non-numeric parameters
					if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					{
						ScriptError("Invalid value: " + csString(tempdata[i]));
						return false;
					}
				}

				//create stairs
				if (Simcore->GetStairs(atoi(tempdata[0])))
					Simcore->GetStairs(atoi(tempdata[0]))->AddStairs(Current, tempdata[1], tempdata[2], tempdata[3], atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atoi(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12]));
				else
				{
					ScriptError("Invalid stairwell");
					return false;
				}
				tempdata.DeleteAll();
			}

			//AddDoor command
			if (LineData.Slice(0, 8).CompareNoCase("adddoor ") == true)
			{
				//get data
				tempdata.SplitString(LineData.Slice(8).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				if (tempdata.GetSize() < 10 || tempdata.GetSize() > 10)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}
				//check numeric values
				for (int i = 1; i <= 9; i++)
				{
					if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					{
						ScriptError("Invalid value: " + csString(tempdata[i]));
						return false;
					}
				}

				//create door
				Simcore->GetFloor(Current)->AddDoor(tempdata[0], atof(tempdata[1]), atoi(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]));
				tempdata.DeleteAll();
			}

			//AddStairsDoor command
			if (LineData.Slice(0, 14).CompareNoCase("addstairsdoor ") == true)
			{
				//get data
				tempdata.SplitString(LineData.Slice(14).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				if (tempdata.GetSize() < 11 || tempdata.GetSize() > 11)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}
				//check numeric values
				for (int i = 0; i <= 10; i++)
				{
					if (i == 1)
						i = 2; //skip non-numeric parameters
					if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					{
						ScriptError("Invalid value: " + csString(tempdata[i]));
						return false;
					}
				}

				//create door
				if (Simcore->GetStairs(atoi(tempdata[0])))
					Simcore->GetStairs(atoi(tempdata[0]))->AddDoor(Current, tempdata[1], atof(tempdata[2]), atoi(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]));
				else
				{
					ScriptError("Invalid stairwell");
					return false;
				}
				tempdata.DeleteAll();
			}

			//AddDirectionalIndicator command
			if (LineData.Slice(0, 23).CompareNoCase("adddirectionalindicator") == true)
			{
				//get data
				tempdata.SplitString(LineData.Slice(24).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				if (tempdata.GetSize() < 18 || tempdata.GetSize() > 18)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}
				//check numeric values
				for (int i = 0; i <= 17; i++)
				{
					if (i == 1)
						i = 9;
					if (i == 12)
						i = 13;
					if (i == 15)
						i = 16;
					if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					{
						ScriptError("Invalid value: " + csString(tempdata[i]));
						return false;
					}
				}

				if (!Simcore->GetElevator(atoi(tempdata[0])))
				{
					ScriptError("Invalid elevator");
					return false;
				}
				Simcore->GetElevator(atoi(tempdata[0]))->AddDirectionalIndicator(Current, csString(tempdata[1]).CompareNoCase("true"), csString(tempdata[2]).CompareNoCase("true"), csString(tempdata[3]).CompareNoCase("true"), tempdata[4], tempdata[5], tempdata[6], tempdata[7], tempdata[8], atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), tempdata[12], atof(tempdata[13]), atof(tempdata[14]), csString(tempdata[15]).CompareNoCase("true"), atof(tempdata[16]), atof(tempdata[17]));

				tempdata.DeleteAll();
			}

			//AddShaftDoor command
			if (LineData.Slice(0, 12).CompareNoCase("addshaftdoor") == true)
			{
				//exit if the SetShaftDoors command was never used
				if (setshaftdoors == false)
				{
					ScriptError("SetShaftDoors must be used before AddShaftDoor");
					return false;
				}

				//get data
				tempdata.SplitString(LineData.Slice(13).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				if (tempdata.GetSize() < 5 || tempdata.GetSize() > 6)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}

				bool compat = false;
				if (tempdata.GetSize() == 5)
					compat = true; //1.4 compatibility mode

				//check numeric values
				if (compat == false)
				{
					for (int i = 0; i <= 5; i++)
					{
						if (i == 2)
							i = 4;
						if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
						{
							ScriptError("Invalid value: " + csString(tempdata[i]));
							return false;
						}
					}
				}
				else
				{
					for (int i = 0; i <= 4; i++)
					{
						if (i == 2)
							i = 3;
						if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
						{
							ScriptError("Invalid value: " + csString(tempdata[i]));
							return false;
						}
					}
				}

				if (!Simcore->GetElevator(atoi(tempdata[0])))
				{
					ScriptError("Invalid elevator");
					return false;
				}
				if (compat == false)
					Simcore->GetElevator(atoi(tempdata[0]))->AddShaftDoor(Current, atoi(tempdata[1]), tempdata[2], tempdata[3], atof(tempdata[4]), atof(tempdata[5]));
				else
					Simcore->GetElevator(atoi(tempdata[0]))->AddShaftDoor(Current, atoi(tempdata[1]), tempdata[2], tempdata[2], atof(tempdata[3]), atof(tempdata[4]));

				tempdata.DeleteAll();
			}

			//AddFloorIndicator command
			if (LineData.Slice(0, 17).CompareNoCase("addfloorindicator") == true)
			{
				//get data
				tempdata.SplitString(LineData.Slice(18).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				if (tempdata.GetSize() < 8 || tempdata.GetSize() > 8)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}
				//check numeric values
				for (int i = 0; i <= 7; i++)
				{
					if (i == 1)
						i = 3;
					if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					{
						ScriptError("Invalid value: " + csString(tempdata[i]));
						return false;
					}
				}

				Simcore->GetFloor(Current)->AddFloorIndicator(atoi(tempdata[0]), csString(tempdata[1]).CompareNoCase("true"), tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]));

				tempdata.DeleteAll();
			}

			//Cut command
			if (LineData.Slice(0, 3).CompareNoCase("cut") == true)
			{
				//get data
				tempdata.SplitString(LineData.Slice(4).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				if (tempdata.GetSize() < 8 || tempdata.GetSize() > 8)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}
				//check numeric values
				for (int i = 0; i <= 5; i++)
				{
					if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					{
						ScriptError("Invalid value: " + csString(tempdata[i]));
						return false;
					}
				}

				//perform cut on floor
				Simcore->GetFloor(Current)->Cut(csVector3(atof(tempdata[0]), atof(tempdata[1]), atof(tempdata[2])), csVector3(atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5])), csString(tempdata[6]).CompareNoCase("true"), csString(tempdata[7]).CompareNoCase("true"), false);
				tempdata.DeleteAll();
			}

			//handle floor range
			if (RangeL != RangeH && LineData.Slice(0, 11).CompareNoCase("<endfloors>") == true)
			{
				if (RangeL < RangeH)
				{
					if (Current < RangeH)
					{
						Current++;
						line = RangeStart;  //loop back
						goto Nextline;
					}
					else
					{
						Section = 0; //break out of loop
						goto Nextline;
					}
				}
				else
				{
					if (Current > RangeH)
					{
						Current--;
						line = RangeStart; //loop back
						goto Nextline;
					}
					else
					{
						Section = 0; //break out of loop
						Context = "None";
						goto Nextline;
					}
				}
			}
		}

		//Process elevators
		if (Section == 4)
		{

			//create elevator if not created already
			Simcore->NewElevator(Current);

			//get text after equal sign
			int temp2check = LineData.Find("=", 0);
			temp2 = LineData.Slice(temp2check + 1).Trim();

			//parameters
			if (LineData.Slice(0, 5).CompareNoCase("speed") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				if (!IsNumeric(temp2.GetData(), Simcore->GetElevator(Current)->ElevatorSpeed))
				{
					ScriptError("Invalid value");
					return false;
				}
			}
			if (LineData.Slice(0, 12).CompareNoCase("acceleration") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				if (!IsNumeric(temp2.GetData(), Simcore->GetElevator(Current)->Acceleration))
				{
					ScriptError("Invalid value");
					return false;
				}
			}
			if (LineData.Slice(0, 12).CompareNoCase("deceleration") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				if (!IsNumeric(temp2.GetData(), Simcore->GetElevator(Current)->Deceleration))
				{
					ScriptError("Invalid value");
					return false;
				}
			}
			if (LineData.Slice(0, 9).CompareNoCase("openspeed") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				if (Simcore->GetElevator(Current)->Created == false)
				{
					ScriptError("Elevator not created yet");
					return false;
				}
				if (!IsNumeric(LineData.Slice(9, LineData.Find("=", 0) - 9).Trim(), temp3))
				{
					ScriptError("No door specified");
					return false;
				}
				if (temp3 == 0 || temp3 > Simcore->GetElevator(Current)->NumDoors)
				{
					ScriptError("Invalid door number");
					return false;
				}
				if (!IsNumeric(temp2.GetData(), Simcore->GetElevator(Current)->GetDoor(temp3)->OpenSpeed))
				{
					ScriptError("Invalid value");
					return false;
				}
			}
			if (LineData.Slice(0, 5).CompareNoCase("doors") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				if (!IsNumeric(temp2.GetData(), Simcore->GetElevator(Current)->NumDoors))
				{
					ScriptError("Invalid value");
					return false;
				}
			}
			if (LineData.Slice(0, 9).CompareNoCase("acceljerk") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				if (!IsNumeric(temp2.GetData(), Simcore->GetElevator(Current)->AccelJerk))
				{
					ScriptError("Invalid value");
					return false;
				}
			}
			if (LineData.Slice(0, 9).CompareNoCase("deceljerk") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				if (!IsNumeric(temp2.GetData(), Simcore->GetElevator(Current)->DecelJerk))
				{
					ScriptError("Invalid value");
					return false;
				}
			}
			if (LineData.Slice(0, 14).CompareNoCase("servicedfloors") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				//copy string listing of serviced floors into array
				tempdata.SplitString(temp2.GetData(), ",");
				for (int line = 0; line < tempdata.GetSize(); line++)
				{
					csString tmpstring = tempdata[line];
					tmpstring.Trim();
					if (tmpstring.Find("-", 1) > 0)
					{
						int start, end;
						//found a range marker
						if (!IsNumeric(tmpstring.Slice(0, tmpstring.Find("-", 1)).Trim().GetData(), start) || !IsNumeric(tmpstring.Slice(tmpstring.Find("-", 1) + 1).Trim().GetData(), end))
						{
							ScriptError("Invalid value");
							return false;
						}
						if (end < start)
						{
							int temp = start;
							start = end;
							end = temp;
						}

						for (int k = start; k <= end; k++)
							Simcore->GetElevator(Current)->AddServicedFloor(k);
					}
					else
					{
						int data;
						if (!IsNumeric(csString(tempdata[line]).Trim().GetData(), data))
						{
							ScriptError("Invalid value");
							return false;
						}
						Simcore->GetElevator(Current)->AddServicedFloor(data);
					}
				}
				tempdata.DeleteAll();
			}
			if (LineData.Slice(0, 13).CompareNoCase("assignedshaft") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				if (!IsNumeric(temp2.GetData(), Simcore->GetElevator(Current)->AssignedShaft))
				{
					ScriptError("Invalid value");
					return false;
				}
			}
			if (LineData.Slice(0, 9).CompareNoCase("doortimer") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				if (Simcore->GetElevator(Current)->Created == false)
				{
					ScriptError("Elevator not created yet");
					return false;
				}
				if (!IsNumeric(LineData.Slice(9, LineData.Find("=", 0) - 9).Trim(), temp3))
				{
					ScriptError("No door specified");
					return false;
				}
				if (temp3 == 0 || temp3 > Simcore->GetElevator(Current)->NumDoors)
				{
					ScriptError("Invalid door number");
					return false;
				}
				if (!IsNumeric(temp2.GetData(), Simcore->GetElevator(Current)->GetDoor(temp3)->DoorTimer))
				{
					ScriptError("Invalid value");
					return false;
				}
			}
			if (LineData.Slice(0, 9).CompareNoCase("opensound") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				if (Simcore->GetElevator(Current)->Created == false)
				{
					ScriptError("Elevator not created yet");
					return false;
				}
				if (!IsNumeric(LineData.Slice(9, LineData.Find("=", 0) - 9).Trim(), temp3))
				{
					ScriptError("No door specified");
					return false;
				}
				if (temp3 == 0 || temp3 > Simcore->GetElevator(Current)->NumDoors)
				{
					ScriptError("Invalid door number");
					return false;
				}
				Simcore->GetElevator(Current)->GetDoor(temp3)->OpenSound = temp2;
			}
			if (LineData.Slice(0, 10).CompareNoCase("closesound") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				if (Simcore->GetElevator(Current)->Created == false)
				{
					ScriptError("Elevator not created yet");
					return false;
				}
				if (!IsNumeric(LineData.Slice(10, LineData.Find("=", 0) - 10).Trim(), temp3))
				{
					ScriptError("No door specified");
					return false;
				}
				if (temp3 == 0 || temp3 > Simcore->GetElevator(Current)->NumDoors)
				{
					ScriptError("Invalid door number");
					return false;
				}
				Simcore->GetElevator(Current)->GetDoor(temp3)->CloseSound = temp2;
			}
			if (LineData.Slice(0, 10).CompareNoCase("startsound") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				Simcore->GetElevator(Current)->StartSound = temp2;
			}
			if (LineData.Slice(0, 9).CompareNoCase("movesound") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				Simcore->GetElevator(Current)->MoveSound = temp2;
			}
			if (LineData.Slice(0, 9).CompareNoCase("stopsound") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				Simcore->GetElevator(Current)->StopSound = temp2;
			}
			if (LineData.Slice(0, 9).CompareNoCase("idlesound") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				Simcore->GetElevator(Current)->IdleSound = temp2;
			}
			if (LineData.Slice(0, 10).CompareNoCase("chimesound") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				if (Simcore->GetElevator(Current)->Created == false)
				{
					ScriptError("Elevator not created yet");
					return false;
				}
				if (!IsNumeric(LineData.Slice(10, LineData.Find("=", 0) - 10).Trim(), temp3))
				{
					ScriptError("No door specified");
					return false;
				}
				if (temp3 == 0 || temp3 > Simcore->GetElevator(Current)->NumDoors)
				{
					ScriptError("Invalid door number");
					return false;
				}
				Simcore->GetElevator(Current)->GetDoor(temp3)->UpChimeSound = temp2;
				Simcore->GetElevator(Current)->GetDoor(temp3)->DownChimeSound = temp2;
			}
			if (LineData.Slice(0, 12).CompareNoCase("upchimesound") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				if (Simcore->GetElevator(Current)->Created == false)
				{
					ScriptError("Elevator not created yet");
					return false;
				}
				if (!IsNumeric(LineData.Slice(12, LineData.Find("=", 0) - 12).Trim(), temp3))
				{
					ScriptError("No door specified");
					return false;
				}
				if (temp3 == 0 || temp3 > Simcore->GetElevator(Current)->NumDoors)
				{
					ScriptError("Invalid door number");
					return false;
				}
				Simcore->GetElevator(Current)->GetDoor(temp3)->UpChimeSound = temp2;
			}
			if (LineData.Slice(0, 14).CompareNoCase("downchimesound") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				if (Simcore->GetElevator(Current)->Created == false)
				{
					ScriptError("Elevator not created yet");
					return false;
				}
				if (!IsNumeric(LineData.Slice(14, LineData.Find("=", 0) - 14).Trim(), temp3))
				{
					ScriptError("No door specified");
					return false;
				}
				if (temp3 == 0 || temp3 > Simcore->GetElevator(Current)->NumDoors)
				{
					ScriptError("Invalid door number");
					return false;
				}
				Simcore->GetElevator(Current)->GetDoor(temp3)->DownChimeSound = temp2;
			}
			if (LineData.Slice(0, 10).CompareNoCase("alarmsound") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				Simcore->GetElevator(Current)->AlarmSound = temp2;
			}
			if (LineData.Slice(0, 14).CompareNoCase("alarmsoundstop") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				Simcore->GetElevator(Current)->AlarmSoundStop = temp2;
			}
			if (LineData.Slice(0, 9).CompareNoCase("beepsound") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				Simcore->GetElevator(Current)->BeepSound = temp2;
			}
			if (LineData.Slice(0, 13).CompareNoCase("floorskiptext") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				Simcore->GetElevator(Current)->SetFloorSkipText(temp2);
			}
			if (LineData.Slice(0, 11).CompareNoCase("recallfloor") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				int floortemp;
				if (!IsNumeric(temp2.GetData(), floortemp))
				{
					ScriptError("Invalid value");
					return false;
				}
				Simcore->GetElevator(Current)->SetRecallFloor(floortemp);
			}
			if (LineData.Slice(0, 20).CompareNoCase("alternaterecallfloor") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				int floortemp;
				if (!IsNumeric(temp2.GetData(), floortemp))
				{
					ScriptError("Invalid value");
					return false;
				}
				Simcore->GetElevator(Current)->SetAlternateRecallFloor(floortemp);
			}
			if (LineData.Slice(0, 8).CompareNoCase("acpfloor") == true)
			{
				if (temp2check < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				int floortemp;
				if (!IsNumeric(temp2.GetData(), floortemp))
				{
					ScriptError("Invalid value");
					return false;
				}
				Simcore->GetElevator(Current)->SetACPFloor(floortemp);
			}

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
					LineData = LineData.Slice(temp3 + 1).Trim(); //trim off IF statement
				else
					goto Nextline; //skip line
			}

			if (LineData.Slice(0, 7).CompareNoCase("<break>") == true)
				goto breakpoint;

			//CreateElevator command
			if (LineData.Slice(0, 14).CompareNoCase("createelevator") == true)
			{
				tempdata.SplitString(LineData.Slice(15).GetData(), ",");
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				if (tempdata.GetSize() < 4 || tempdata.GetSize() > 4)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}
				//check numeric values
				for (int i = 1; i <= 3; i++)
				{
					if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					{
						ScriptError("Invalid value: " + csString(tempdata[i]));
						return false;
					}
				}

				if (!Simcore->GetElevator(Current)->CreateElevator(csString(tempdata[0]).CompareNoCase("true"), atof(tempdata[1]), atof(tempdata[2]), atoi(tempdata[3])))
				{
					ScriptError("An error occurred while creating the elevator.  See the console output for more information");
					return false;
				}
				tempdata.DeleteAll();
			}

			//AddFloor command
			if (LineData.Slice(0, 9).CompareNoCase("addfloor ") == true)
			{
				//get data
				tempdata.SplitString(LineData.Slice(10).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				if (tempdata.GetSize() < 11 || tempdata.GetSize() > 11)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}
				//check numeric values
				for (int i = 2; i <= 10; i++)
				{
					if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					{
						ScriptError("Invalid value: " + csString(tempdata[i]));
						return false;
					}
				}

				//create floor
				Simcore->GetElevator(Current)->AddFloor(tempdata[0], tempdata[1], atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]));

				tempdata.DeleteAll();
			}

			//AddWall command
			if (LineData.Slice(0, 7).CompareNoCase("addwall") == true)
			{
				//get data
				tempdata.SplitString(LineData.Slice(8).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				if (tempdata.GetSize() < 13 || tempdata.GetSize() > 13)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}
				//check numeric values
				for (int i = 2; i <= 12; i++)
				{
					if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					{
						ScriptError("Invalid value: " + csString(tempdata[i]));
						return false;
					}
				}

				//create wall
				Simcore->GetElevator(Current)->AddWall(tempdata[0], tempdata[1], atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12]));

				tempdata.DeleteAll();
			}

			//AddDoors command
			if (LineData.Slice(0, 8).CompareNoCase("adddoors") == true)
			{
				//get data
				tempdata.SplitString(LineData.Slice(9).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				if (tempdata.GetSize() < 10 || tempdata.GetSize() > 11)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}

				bool compat = false;
				if (tempdata.GetSize() == 10)
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
						{
							ScriptError("Invalid value: " + csString(tempdata[i]));
							return false;
						}
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
						{
							ScriptError("Invalid value: " + csString(tempdata[i]));
							return false;
						}
					}
				}

				if (compat == false)
					Simcore->GetElevator(Current)->AddDoors(atoi(tempdata[0]), tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), csString(tempdata[8]).CompareNoCase("true"), atof(tempdata[9]), atof(tempdata[10]));
				else
					Simcore->GetElevator(Current)->AddDoors(atoi(tempdata[0]), tempdata[1], tempdata[1], atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), csString(tempdata[7]).CompareNoCase("true"), atof(tempdata[8]), atof(tempdata[9]));

				tempdata.DeleteAll();
			}

			//SetShaftDoors command
			if (LineData.Slice(0, 13).CompareNoCase("setshaftdoors") == true)
			{
				//get data
				tempdata.SplitString(LineData.Slice(14).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				if (tempdata.GetSize() < 4 || tempdata.GetSize() > 4)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}
				//check numeric values
				for (int i = 0; i <= 3; i++)
				{
					if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					{
						ScriptError("Invalid value: " + csString(tempdata[i]));
						return false;
					}
				}

				Simcore->GetElevator(Current)->SetShaftDoors(atoi(tempdata[0]), atof(tempdata[1]), atof(tempdata[2]), atof(tempdata[3]));
				setshaftdoors = true;

				tempdata.DeleteAll();
			}

			//AddShaftDoors command
			if (LineData.Slice(0, 13).CompareNoCase("addshaftdoors") == true)
			{
				//get data
				tempdata.SplitString(LineData.Slice(14).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				if (tempdata.GetSize() < 7 || tempdata.GetSize() > 8)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}

				bool compat = false;
				if (tempdata.GetSize() == 7)
					compat = true; //1.4 compatibility mode

				//check numeric values
				if (compat == false)
				{
					for (int i = 0; i <= 7; i++)
					{
						if (i == 1)
							i = 3;
						if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
						{
							ScriptError("Invalid value: " + csString(tempdata[i]));
							return false;
						}
					}
				}
				else
				{
					for (int i = 0; i <= 6; i++)
					{
						if (i == 1)
							i = 2;
						if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
						{
							ScriptError("Invalid value: " + csString(tempdata[i]));
							return false;
						}
					}
				}

				if (compat == false)
					Simcore->GetElevator(Current)->AddShaftDoors(atoi(tempdata[0]), tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]));
				else
					Simcore->GetElevator(Current)->AddShaftDoors(atoi(tempdata[0]), tempdata[1], tempdata[1], atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]));

				tempdata.DeleteAll();
			}

			//CreatePanel command
			if (LineData.Slice(0, 11).CompareNoCase("createpanel") == true)
			{
				//get data
				tempdata.SplitString(LineData.Slice(12).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				if (tempdata.GetSize() < 13 || tempdata.GetSize() > 13)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}
				//check numeric values
				for (int i = 1; i <= 12; i++)
				{
					if (i == 3)
						i = 4;
					if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					{
						ScriptError("Invalid value: " + csString(tempdata[i]));
						return false;
					}
				}

				Simcore->GetElevator(Current)->CreateButtonPanel(tempdata[0], atoi(tempdata[1]), atoi(tempdata[2]), tempdata[3], atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12]));

				tempdata.DeleteAll();
			}

			//AddFloorButton command
			if (LineData.Slice(0, 14).CompareNoCase("addfloorbutton") == true)
			{
				//get data
				tempdata.SplitString(LineData.Slice(15).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				if (tempdata.GetSize() < 7 || tempdata.GetSize() > 9)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}

				float hoffset = 0, voffset = 0;

				if (tempdata.GetSize() == 7)
				{
					//check numeric values
					for (int i = 0; i <= 6; i++)
					{
						if (i == 1)
							i = 2;
						if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
						{
							ScriptError("Invalid value: " + csString(tempdata[i]));
							return false;
						}
					}
				}
				else
				{
					//check numeric values
					for (int i = 0; i <= 8; i++)
					{
						if (i == 1)
							i = 2;
						if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
						{
							ScriptError("Invalid value: " + csString(tempdata[i]));
							return false;
						}
					}
					hoffset = atof(tempdata[7]);
					voffset = atof(tempdata[8]);
				}

				if (atoi(tempdata[0]) == 1)
				{
					if (!Simcore->GetElevator(Current)->Panel)
					{
						Report("Elevator " + csString(_itoa(Current, intbuffer, 10)) + ": cannot add button");
						goto Nextline;
					}
					Simcore->GetElevator(Current)->Panel->AddFloorButton(tempdata[1], atoi(tempdata[2]), atoi(tempdata[3]), atoi(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), hoffset, voffset);
				}
				else if (atoi(tempdata[0]) == 2)
				{
					if (!Simcore->GetElevator(Current)->Panel2)
					{
						Report("Elevator " + csString(_itoa(Current, intbuffer, 10)) + ": cannot add button");
						goto Nextline;
					}
					Simcore->GetElevator(Current)->Panel2->AddFloorButton(tempdata[1], atoi(tempdata[2]), atoi(tempdata[3]), atoi(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), hoffset, voffset);
				}
				else
				{
					ScriptError("Invalid panel number");
					return false;
				}

				tempdata.DeleteAll();
			}

			//AddControlButton command
			if (LineData.Slice(0, 16).CompareNoCase("addcontrolbutton") == true)
			{
				//get data
				tempdata.SplitString(LineData.Slice(17).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				if (tempdata.GetSize() < 7 || tempdata.GetSize() > 9)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}

				float hoffset = 0, voffset = 0;

				if (tempdata.GetSize() == 7)
				{
					//check numeric values
					for (int i = 1; i <= 6; i++)
					{
						if (i == 1 || i == 4)
							i++;
						if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
						{
							ScriptError("Invalid value: " + csString(tempdata[i]));
							return false;
						}
					}
				}
				else
				{
					//check numeric values
					for (int i = 1; i <= 8; i++)
					{
						if (i == 1 || i == 4)
							i++;
						if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
						{
							ScriptError("Invalid value: " + csString(tempdata[i]));
							return false;
						}
					}
					hoffset = atof(tempdata[7]);
					voffset = atof(tempdata[8]);
				}

				if (atoi(tempdata[0]) == 1)
				{
					if (!Simcore->GetElevator(Current)->Panel)
					{
						Report("Elevator " + csString(_itoa(Current, intbuffer, 10)) + ": cannot add button");
						goto Nextline;
					}
					Simcore->GetElevator(Current)->Panel->AddControlButton(tempdata[1], atoi(tempdata[2]), atoi(tempdata[3]), tempdata[4], atof(tempdata[5]), atof(tempdata[6]), hoffset, voffset);
				}
				else if (atoi(tempdata[0]) == 2)
				{
					if (!Simcore->GetElevator(Current)->Panel2)
					{
						Report("Elevator " + csString(_itoa(Current, intbuffer, 10)) + ": cannot add button");
						goto Nextline;
					}
					Simcore->GetElevator(Current)->Panel2->AddControlButton(tempdata[1], atoi(tempdata[2]), atoi(tempdata[3]), tempdata[4], atof(tempdata[5]), atof(tempdata[6]), hoffset, voffset);
				}
				else
				{
					ScriptError("Invalid panel number");
					return false;
				}

				tempdata.DeleteAll();
			}

			//AddFloorIndicator command
			if (LineData.Slice(0, 17).CompareNoCase("addfloorindicator") == true)
			{
				//get data
				tempdata.SplitString(LineData.Slice(18).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				if (tempdata.GetSize() < 6 || tempdata.GetSize() > 6)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}
				//check numeric values
				for (int i = 1; i <= 5; i++)
				{
					if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					{
						ScriptError("Invalid value: " + csString(tempdata[i]));
						return false;
					}
				}

				Simcore->GetElevator(Current)->AddFloorIndicator(tempdata[0], atof(tempdata[1]), atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]));

				tempdata.DeleteAll();
			}

			//AddDirectionalIndicators command
			if (LineData.Slice(0, 24).CompareNoCase("adddirectionalindicators") == true)
			{
				//get data
				tempdata.SplitString(LineData.Slice(25).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				if (tempdata.GetSize() < 17 || tempdata.GetSize() > 17)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}
				//check numeric values
				for (int i = 8; i <= 16; i++)
				{
					if (i == 11)
						i = 12;
					if (i == 14)
						i = 15;
					if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					{
						ScriptError("Invalid value: " + csString(tempdata[i]));
						return false;
					}
				}

				Simcore->GetElevator(Current)->AddDirectionalIndicators(csString(tempdata[0]).CompareNoCase("true"), csString(tempdata[1]).CompareNoCase("true"), csString(tempdata[2]).CompareNoCase("true"), tempdata[3], tempdata[4], tempdata[5], tempdata[6], tempdata[7], atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), tempdata[11], atof(tempdata[12]), atof(tempdata[13]), csString(tempdata[14]).CompareNoCase("true"), atof(tempdata[15]), atof(tempdata[16]));

				tempdata.DeleteAll();
			}

			//AddFloorSigns command
			if (LineData.Slice(0, 13).CompareNoCase("addfloorsigns") == true)
			{
				//get data
				tempdata.SplitString(LineData.Slice(14).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				if (tempdata.GetSize() < 7 || tempdata.GetSize() > 7)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}
				//check numeric values
				for (int i = 2; i <= 6; i++)
				{
					if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					{
						ScriptError("Invalid value: " + csString(tempdata[i]));
						return false;
					}
				}

				Simcore->GetElevator(Current)->AddFloorSigns(csString(tempdata[0]).CompareNoCase("true"), tempdata[1], atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]));

				tempdata.DeleteAll();
			}


			//Set command
			if (LineData.Slice(0, 4).CompareNoCase("set ") == true)
			{
				temp1 = LineData.Find("=", 0);
				if (temp1 < 0)
				{
					ScriptError("Syntax error");
					return false;
				}
				if (!IsNumeric(LineData.Slice(4, temp1 - 5).Trim().GetData(), temp3))
				{
					ScriptError("Invalid variable number");
					return false;
				}
				temp2 = LineData.Slice(temp1 + 1);
				if (temp3 < 0 || temp3 > UserVariable.GetSize() - 1)
				{
					ScriptError("Invalid variable number");
					return false;
				}
				UserVariable[temp3] = Calc(temp2);
				Report("Variable " + csString(_itoa(temp3, intbuffer, 10)) + " set to " + UserVariable[temp3]);
			}

			//handle elevator range
			if (RangeL != RangeH && LineData.Slice(0, 14).CompareNoCase("<endelevators>") == true)
			{
				if (Current < RangeH)
				{
					Current++;
					line = RangeStart;  //loop back
					goto Nextline;
				}
				else
				{
					Section = 0; //break out of loop
					Context = "None";
					goto Nextline;
				}
			}
		}

		//Process textures
		if (Section == 5)
		{
			if (LineData.Slice(0, 5).CompareNoCase("load ") == true)
			{
				tempdata.SplitString(LineData.Slice(5).GetData(), ",");
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = tempdata[temp3];
					tempdata.Put(temp3, buffer.Trim());
				}
				if (tempdata.GetSize() < 4 || tempdata.GetSize() > 4)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}
				//check numeric values
				for (int i = 2; i <= 3; i++)
				{
					if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					{
						ScriptError("Invalid value: " + csString(tempdata[i]));
						return false;
					}
				}
				buffer = tempdata[0];
				buffer.Insert(0, "/root/");
				//if (buffer != "/root/data/brick1.jpg")
					Simcore->LoadTexture(buffer.GetData(), tempdata[1], atof(tempdata[2]), atof(tempdata[3]));
				//else
					//Simcore->AddTextToTexture(buffer.GetData(), tempdata[1], atof(tempdata[2]), atof(tempdata[3]), "a", "test,", 1, 1, 100, 100, true, true);
				tempdata.DeleteAll();
			}
			if (LineData.Slice(0, 9).CompareNoCase("loadrange") == true)
			{
				tempdata.SplitString(LineData.Slice(9).GetData(), ",");
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = tempdata[temp3];
					tempdata.Put(temp3, buffer.Trim());
				}
				if (tempdata.GetSize() < 6 || tempdata.GetSize() > 6)
				{
					ScriptError("Incorrect number of parameters");
					return false;
				}
				//check numeric values
				for (int i = 0; i <= 5; i++)
				{
					if (i == 2)
						i = 4;
					if (!IsNumeric(csString(tempdata[i]).Trim().GetData()))
					{
						ScriptError("Invalid value: " + csString(tempdata[i]));
						return false;
					}
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
					Simcore->LoadTexture("/root/" + temp2, temp6, atof(tempdata[4]), atof(tempdata[5]));
				}
				tempdata.DeleteAll();
			}
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

bool Skyscraper::LoadDataFile(const char *filename)
{
	//loads a building data file into the runtime buffer
	bool streamnotfinished = true;
	char buffer[1000];

	//make sure file exists
	if (Simcore->vfs->Exists(filename) == false)
		return false;

	//load file
	csRef<iFile> file (Simcore->vfs->Open(filename, VFS_FILE_READ));

	//exit if an error occurred while loading
	if (!file)
		return false;

	csFileReadHelper file_r(file);

	//clear array
	BuildingData.DeleteAll();

	while (streamnotfinished == true)
	{
		//clear buffer
		for (int i = 0; i < 1000; i++)
			buffer[i] = ' ';

		//read each line into the buffer
		streamnotfinished = file_r.GetString(buffer, 1000, true);

		//push buffer onto the tail end of the BuildingData array
		BuildingData.Push(buffer);
	}

	return true;
}

csString Skyscraper::Calc(const char *expression)
{
	//performs a calculation operation on a string
	//for example, the string "1 + 1" would output to "2"
	//supports multiple and nested operations (within parenthesis)

	int temp1;
	csString tmpcalc = expression;
	char buffer[20];
	csString one;
	csString two;
	int start, end;

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
				ReportError("Syntax error in math operation: '" + tmpcalc + "' (might be nested)");
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
			ReportError("Syntax error in math operation: '" + tmpcalc + "' (might be nested)");
			return "false";
		}
		if (operators > 1)
		{
			csString newdata;
			newdata = Calc(tmpcalc.Slice(0, end));
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
			tmpcalc = _gcvt(atof(one.GetData()) + atof(two.GetData()), 12, buffer);
			if (tmpcalc.GetAt(tmpcalc.Length() - 1) == '.')
				tmpcalc = tmpcalc.Slice(0, tmpcalc.Length() - 1); //strip of extra decimal point if even
			return tmpcalc.GetData();
		}
	}
	temp1 = tmpcalc.Find("-", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.Slice(0, temp1);
		two = tmpcalc.Slice(temp1 + 1);
		if (IsNumeric(one.GetData()) == true && IsNumeric(two.GetData()) == true)
		{
			tmpcalc = _gcvt(atof(one.GetData()) - atof(two.GetData()), 12, buffer);
			if (tmpcalc.GetAt(tmpcalc.Length() - 1) == '.')
				tmpcalc = tmpcalc.Slice(0, tmpcalc.Length() - 1); //strip of extra decimal point if even
			return tmpcalc.GetData();
		}
	}
	temp1 = tmpcalc.Find("/", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.Slice(0, temp1);
		two = tmpcalc.Slice(temp1 + 1);
		if (IsNumeric(one.GetData()) == true && IsNumeric(two.GetData()) == true)
		{
			tmpcalc = _gcvt(atof(one.GetData()) / atof(two.GetData()), 12, buffer);
			if (tmpcalc.GetAt(tmpcalc.Length() - 1) == '.')
				tmpcalc = tmpcalc.Slice(0, tmpcalc.Length() - 1); //strip of extra decimal point if even
			return tmpcalc.GetData();
		}
	}
	temp1 = tmpcalc.Find("*", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.Slice(0, temp1);
		two = tmpcalc.Slice(temp1 + 1);
		if (IsNumeric(one.GetData()) == true && IsNumeric(two.GetData()) == true)
		{
			tmpcalc = _gcvt(atof(one.GetData()) * atof(two.GetData()), 12, buffer);
			if (tmpcalc.GetAt(tmpcalc.Length() - 1) == '.')
				tmpcalc = tmpcalc.Slice(0, tmpcalc.Length() - 1); //strip of extra decimal point if even
			return tmpcalc.GetData();
		}
	}
	return tmpcalc.GetData();
}

bool Skyscraper::IfProc(const char *expression)
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
		ReportError("Syntax error in IF operation: '" + tmpcalc + "' (might be nested)");
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
				ReportError("Syntax error in IF operation: '" + tmpcalc + "' (might be nested)");
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
			ReportError("Syntax error in IF operation: '" + tmpcalc + "' (might be nested)");
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

void Skyscraper::ScriptError(const char *message)
{
	//Script error reporting function
	char intbuffer[65];
	csString error = "Script error on line " + csString(_itoa(line + 1, intbuffer, 10)) + ": " + csString(message) + "\nSection: " + csString(_itoa(Section, intbuffer, 10)) + "\nContext: " + Context + "\nLine Text: " + LineData;

	ReportError(error);

	//show error dialog
	wxMessageDialog *dialog = new wxMessageDialog(wxwin->GetWindow(), wxString::FromAscii(error.GetData()), wxString::FromAscii("Skyscraper"), wxOK | wxICON_ERROR);
	dialog->ShowModal();

	delete dialog;
	dialog = 0;
}
