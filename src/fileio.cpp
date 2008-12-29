/* $Id$ */

/*
	Skyscraper 1.1 Alpha - File I/O Processing Routines
	Copyright (C)2005-2008 Ryan Thoryk
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

float AltitudeCheck;
extern SBS *Simcore;

int Skyscraper::LoadBuilding(const char * filename)
{
	//building loader/script interpreter

	//load building into buffer
	if (LoadDataFile(filename) > 0)
		return 1;

	long i = 0; //line number
	csString LineData = "";  //line contents
	int temp1 = 0;
	csString temp2 = "";
	int temp3 = 0;
	int temp4 = 0;
	int temp5 = 0;
	csString temp6 = "";
	csString temp7 = "";
	csStringArray tempdata;
	csArray<int> callbutton_elevators;
	int Current = 0;
	int FloorCheck = 0;
	int RangeL = 0;
	int RangeH = 0;
	long RangeStart = 0;
	int Section = 0;
	csRef<iThingFactoryState> tmpMesh;
	csString Context = "None";
	char intbuffer[65];
	csString buffer;
	int startpos = 0;
	bool getfloordata = false;

	while (i < BuildingData.GetSize() - 1)
	{
		LineData = BuildingData[i];
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
			Section = 1;
			Context = "Globals";
			Simcore->Report("Processing globals...");
			goto Nextline;
		}
		if (LineData.CompareNoCase("<endglobals>") == true)
		{
			Simcore->InitMeshes();
			Section = 0;
			Context = "None";
			Simcore->Report("Finished globals");
			goto Nextline;
		}
		if (LineData.Slice(0, 7).CompareNoCase("<floors") == true)
		{
			Section = 2;
			temp3 = csString(LineData).Downcase().Find("to", 0);
			RangeL = atoi(LineData.Slice(8, temp3 - 9).GetData());
			RangeH = atoi(LineData.Slice(temp3 + 2).GetData());
			Context = "Floor range " + csString(_itoa(RangeL, intbuffer, 10)) + " to " + csString(_itoa(RangeH, intbuffer, 10));
			//if (RangeL < -Basements !! RangeH > TotalFloors)
				//Err.Raise 1004;
			Current = RangeL;
			RangeStart = i;
			Simcore->Report("Processing floors " + csString(_itoa(RangeL, intbuffer, 10)) + " to " + csString(_itoa(RangeH, intbuffer, 10)) + "...");
			goto Nextline;
		}
		if (LineData.Slice(0, 7).CompareNoCase("<floor ") == true)
		{
			Section = 2;
			Context = "Floor";
			RangeL = 0;
			RangeH = 0;
			Current = atoi(LineData.Slice(7, LineData.Length() - 7).GetData());
			//if (Current < -Basements !! Current > TotalFloors)
				//Err.Raise 1005
			Simcore->Report("Processing floor " + csString(_itoa(Current, intbuffer, 10)) + "...");
			goto Nextline;
		}
		if (LineData.CompareNoCase("<endfloor>") == true)
		{
			Section = 0;
			Context = "None";
			Simcore->Report("Finished floor");
			goto Nextline;
		}
		if (LineData.Slice(0, 10).CompareNoCase("<elevators") == true)
		{
			Section = 4;
			temp3 = csString(LineData).Downcase().Find("to", 10);
			RangeL = atoi(LineData.Slice(11, temp3 - 12).GetData());
			RangeH = atoi(LineData.Slice(temp3 + 2).GetData());
			Context = "Elevator range " + csString(_itoa(RangeL, intbuffer, 10)) + " to " + csString(_itoa(RangeH, intbuffer, 10));
			Current = RangeL;
			RangeStart = i;
			Simcore->Report("Processing elevators " + csString(_itoa(RangeL, intbuffer, 10)) + " to " + csString(_itoa(RangeH, intbuffer, 10)) + "...");
			goto Nextline;
		}
		if (LineData.Slice(0, 10).CompareNoCase("<elevator ") == true)
		{
			Section = 4;
			Context = "Elevator";
			RangeL = 0;
			RangeH = 0;
			Current = atoi(LineData.Slice(10, LineData.Length() - 10).GetData());
			Simcore->Report("Processing elevator " + csString(_itoa(Current, intbuffer, 10)) + "...");
			goto Nextline;
		}
		if (LineData.CompareNoCase("<endelevator>") == true)
		{
			Section = 0;
			Context = "None";
			Simcore->Report("Finished elevator");
			goto Nextline;
		}
		if (LineData.Slice(0, 10).CompareNoCase("<textures>") == true)
		{
			Section = 5;
			Context = "Textures";
			Simcore->Report("Processing textures...");
			goto Nextline;
		}
		if (LineData.Slice(0, 13).CompareNoCase("<endtextures>") == true)
		{
			Section = 0;
			Context = "None";
			Simcore->Report("Finished textures");
			goto Nextline;
		}
		if (LineData.Slice(0, 5).CompareNoCase("<end>") == true)
		{
			Section = 0;
			Context = "None";
			Simcore->Report("Exiting building script");
			break; //exit data file parser
		}
		if (LineData.Slice(0, 7).CompareNoCase("<break>") == true)
		{
			//breakpoint function for debugging scripts
breakpoint:
			Simcore->Report("Script breakpoint reached");
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
					//if (temp2 < 0 !! temp2 > UBound(Simcore->UserVariable))
						//Err.Raise 1001
					LineData.ReplaceAll("%" + temp2 + "%", Simcore->UserVariable[atoi(temp2.GetData())]);
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
			if (Section == 2 && getfloordata == false)
			{
				//process floor-specific variables if in a floor section
				getfloordata = true;
				goto recalc;
			}
			else
				getfloordata = false;
			csString tempdata = Simcore->Calc(LineData.Slice(temp1 + 1, temp3 - temp1 - 1).GetData());
			LineData = LineData.Slice(0, temp1 + 1) + tempdata + LineData.Slice(temp3);

			temp4 = atoi(tempdata.GetData());
			//if (temp4 < -Basements !! temp4 > TotalFloors)
				//Err.Raise 1005;

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
			//get data
			tempdata.SplitString(LineData.Slice(16).GetData(), ",");

			//calculate inline math
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Simcore->Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer);
			}
			//if (tempdata.GetSize() < 13)
				//Err.Raise 1003;
			//If IsNumeric(tempdata(1)) = False Or IsNumeric(tempdata(2)) = False Or IsNumeric(tempdata(3)) = False Or IsNumeric(tempdata(4)) = False Or IsNumeric(tempdata(5)) = False Or IsNumeric(tempdata(6)) = False Or IsNumeric(tempdata(7)) = False Or IsNumeric(tempdata(8)) = False Or IsNumeric(tempdata(9)) = False Or IsNumeric(tempdata(10)) = False Or IsNumeric(tempdata(11)) = False Or IsNumeric(tempdata(12)) = False Then Err.Raise 1000

			if (csString(tempdata[0]).CompareNoCase("floor") == true)
				tmpMesh = Simcore->GetFloor(Current)->Level_state;
			else
			{
			if (Section == 2)
				{
					buffer = Simcore->GetFloor(Current)->Altitude + atof(tempdata[4]);
					tempdata.Put(4, buffer);
					buffer = Simcore->GetFloor(Current)->Altitude + atof(tempdata[7]);
					tempdata.Put(7, buffer);
				}
			}
			buffer = tempdata[0];
			buffer.Downcase();
			if (buffer == "external")
				tmpMesh = Simcore->External_state;
			if (buffer == "landscape")
				tmpMesh = Simcore->Landscape_state;
			if (buffer == "buildings")
				tmpMesh = Simcore->Buildings_state;
			if (buffer == "columnframe")
				tmpMesh = Simcore->ColumnFrame_state;

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
				buffer = Simcore->Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer);
			}
			//if (tempdata.GetSize() < 11)
				//Err.Raise 1003;
			//if IsNumeric(tempdata(1)) = False Or IsNumeric(tempdata(2)) = False Or IsNumeric(tempdata(3)) = False Or IsNumeric(tempdata(4)) = False Or IsNumeric(tempdata(5)) = False Or IsNumeric(tempdata(6)) = False Or IsNumeric(tempdata(7)) = False Or IsNumeric(tempdata(8)) = False Or IsNumeric(tempdata(9)) = False Or IsNumeric(tempdata(10)) = False Then Err.Raise 1000

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
				buffer = Simcore->Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer);
			}
			//if (tempdata.GetSize() < 7)
				//Err.Raise 1003;
			//If IsNumeric(tempdata(1)) = False Or IsNumeric(tempdata(2)) = False Or IsNumeric(tempdata(3)) = False Or IsNumeric(tempdata(4)) = False Or IsNumeric(tempdata(5)) = False Or IsNumeric(tempdata(6)) = False Or IsNumeric(tempdata(7)) = False Then Err.Raise 1000

			//create floor
			Simcore->AddFloor(tempdata[0], tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]));
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
				buffer = Simcore->Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer);
			}

			buffer = tempdata[0];
			buffer.Downcase();
			if (buffer == "external")
				tmpMesh = Simcore->External_state;
			if (buffer == "landscape")
				tmpMesh = Simcore->Landscape_state;
			if (buffer == "buildings")
				tmpMesh = Simcore->Buildings_state;
			if (buffer == "columnframe")
				tmpMesh = Simcore->ColumnFrame_state;

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
			//if (temp3 < 0 !! temp3 > UBound(Simcore->UserVariable))
				//Err.Raise 1001
			Simcore->UserVariable[temp3] = Simcore->Calc(temp2);
			//Simcore->Report("Variable " + csString(_itoa(temp3, intbuffer, 10)) + " set to " + Simcore->UserVariable[temp3]);
		}

		//CreateWallBox2 command
		if (LineData.Slice(0, 14).CompareNoCase("createwallbox2") == true)
		{
			tempdata.SplitString(LineData.Slice(15).GetData(), ",");
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Simcore->Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer);
			}
			//if (tempdata.GetSize < 9)
				//Err.Raise 1003;
			if (csString(tempdata[0]).CompareNoCase("floor") == true)
				tmpMesh = Simcore->GetFloor(Current)->Level_state;
			else
			{
			if (Section == 2)
				{
					buffer = Simcore->GetFloor(Current)->Altitude + atof(tempdata[8]);
					tempdata.Put(8, buffer);
				}
			}
			buffer = tempdata[0];
			buffer.Downcase();
			if (buffer == "external")
				tmpMesh = Simcore->External_state;
			if (buffer == "landscape")
				tmpMesh = Simcore->Landscape_state;
			if (buffer == "buildings")
				tmpMesh = Simcore->Buildings_state;
			if (buffer == "columnframe")
				tmpMesh = Simcore->ColumnFrame_state;
			//if IsNumeric(tempdata(2)) = False Or IsNumeric(tempdata(3)) = False Or IsNumeric(tempdata(4)) = False Or IsNumeric(tempdata(5)) = False Or IsNumeric(tempdata(6)) = False Or IsNumeric(tempdata(7)) = False Or IsNumeric(tempdata(8)) = False Or IsNumeric(tempdata(9)) = False Then Err.Raise 1000
			Simcore->CreateWallBox2(tmpMesh, tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]));
			tempdata.DeleteAll();
		}

		//CreateWallBox command
		if (LineData.Slice(0, 14).CompareNoCase("createwallbox ") == true)
		{
			tempdata.SplitString(LineData.Slice(15).GetData(), ",");
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Simcore->Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer);
			}
			//if (tempdata.GetSize < 9)
				//Err.Raise 1003
			if (csString(tempdata[0]).CompareNoCase("floor") == true)
				tmpMesh = Simcore->GetFloor(Current)->Level_state;
			else
			{
				if (Section == 2)
				{
					buffer = Simcore->GetFloor(Current)->Altitude + atof(tempdata[8]);
					tempdata.Put(8, buffer);
				}
			}
			buffer = tempdata[0];
			buffer.Downcase();
			if (buffer == "external")
				tmpMesh = Simcore->External_state;
			if (buffer == "landscape")
				tmpMesh = Simcore->Landscape_state;
			if (buffer == "buildings")
				tmpMesh = Simcore->Buildings_state;
			if (buffer == "columnframe")
				tmpMesh = Simcore->ColumnFrame_state;
			//If IsNumeric(tempdata(2)) = False Or IsNumeric(tempdata(3)) = False Or IsNumeric(tempdata(4)) = False Or IsNumeric(tempdata(5)) = False Or IsNumeric(tempdata(6)) = False Or IsNumeric(tempdata(7)) = False Or IsNumeric(tempdata(8)) = False Or IsNumeric(tempdata(9)) = False Then Err.Raise 1000
			Simcore->CreateWallBox(tmpMesh, tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]));
			tempdata.DeleteAll();
		}

		//AddCustomWall command
		if (LineData.Slice(0, 14).CompareNoCase("addcustomwall ") == true)
		{
			tempdata.SplitString(LineData.Slice(14).GetData(), ",");
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Simcore->Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer);
			}

			buffer = tempdata[0];
			buffer.Downcase();
			if (buffer == "floor")
				tmpMesh = Simcore->GetFloor(Current)->Level_state;
			if (buffer == "external")
				tmpMesh = Simcore->External_state;
			if (buffer == "landscape")
				tmpMesh = Simcore->Landscape_state;
			if (buffer == "buildings")
				tmpMesh = Simcore->Buildings_state;
			if (buffer == "columnframe")
				tmpMesh = Simcore->ColumnFrame_state;

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
			tempdata.SplitString(LineData.Slice(15).GetData(), ",");
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Simcore->Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer);
			}

			buffer = tempdata[0];
			buffer.Downcase();
			if (buffer == "floor")
			tmpMesh = Simcore->GetFloor(Current)->Level_state;
			if (buffer == "external")
				tmpMesh = Simcore->External_state;
			if (buffer == "landscape")
				tmpMesh = Simcore->Landscape_state;
			if (buffer == "buildings")
				tmpMesh = Simcore->Buildings_state;
			if (buffer == "columnframe")
				tmpMesh = Simcore->ColumnFrame_state;

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
				buffer = Simcore->Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer);
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
				buffer = Simcore->Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer);
			}

			Simcore->GetShaft(atoi(tempdata[0]))->CutFloors(true, csVector2(atof(tempdata[1]), atof(tempdata[2])), csVector2(atof(tempdata[3]), atof(tempdata[4])), atof(tempdata[5]), atof(tempdata[6]));

			tempdata.DeleteAll();
		}

		//CreateStairwell command
		if (LineData.Slice(0, 15).CompareNoCase("createstairwell") == true)
		{
			tempdata.SplitString(LineData.Slice(16).GetData(), ",");
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Simcore->Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer);
			}

			if (!Simcore->GetStairs(atoi(tempdata[0])))
				Simcore->CreateStairwell(atoi(tempdata[0]), atof(tempdata[1]), atof(tempdata[2]), atoi(tempdata[3]), atoi(tempdata[4]));

			tempdata.DeleteAll();
		}

		//CutStairwell command
		if (LineData.Slice(0, 13).CompareNoCase("cutstairwell ") == true)
		{
			tempdata.SplitString(LineData.Slice(13).GetData(), ",");
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Simcore->Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer);
			}

			Simcore->GetStairs(atoi(tempdata[0]))->CutFloors(true, csVector2(atof(tempdata[1]), atof(tempdata[2])), csVector2(atof(tempdata[3]), atof(tempdata[4])), atof(tempdata[5]), atof(tempdata[6]));

			tempdata.DeleteAll();
		}

		//SetWallOrientation command
		if (LineData.Slice(0, 15).CompareNoCase("wallorientation") == true)
		{
			//get text after equal sign
			temp2 = LineData.Slice(LineData.Find("=", 0) + 1);
			temp2.Trim();

			Simcore->SetWallOrientation(temp2.GetData());
		}

		//SetFloorOrientation command
		if (LineData.Slice(0, 16).CompareNoCase("floororientation") == true)
		{
			//get text after equal sign
			temp2 = LineData.Slice(LineData.Find("=", 0) + 1);
			temp2.Trim();

			Simcore->SetFloorOrientation(temp2.GetData());
		}

		//DrawWalls command
		if (LineData.Slice(0, 9).CompareNoCase("drawwalls") == true)
		{
			//get text after equal sign
			temp2 = LineData.Slice(LineData.Find("=", 0) + 1);
			temp2.Trim();

			tempdata.SplitString(temp2.GetData(), ",");
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Simcore->Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer);
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
				buffer = Simcore->Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer);
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
			temp2 = LineData.Slice(LineData.Find("=", 0) + 1);
			temp2.Trim();

			Simcore->ReverseAxis(temp2.CompareNoCase("true"));
		}

		//Intersection points
		temp5 = csString(LineData).Downcase().Find("isect(", 0);
		while (temp5 > -1)
		{
			temp1 = LineData.Find("(", 0);
			temp3 = LineData.Find(")", 0);
			tempdata.SplitString(LineData.Slice(temp1 + 1, temp3 - temp1 - 1).GetData(), ",");
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Simcore->Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer);
			}

			buffer = tempdata[0];
			buffer.Downcase();
			if (buffer == "floor")
			tmpMesh = Simcore->GetFloor(Current)->Level_state;
			if (buffer == "external")
				tmpMesh = Simcore->External_state;
			if (buffer == "landscape")
				tmpMesh = Simcore->Landscape_state;
			if (buffer == "buildings")
				tmpMesh = Simcore->Buildings_state;
			if (buffer == "columnframe")
				tmpMesh = Simcore->ColumnFrame_state;

			csVector3 isect = Simcore->GetPoint(tmpMesh, tempdata[1], csVector3(atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4])), csVector3(atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7])));
			tempdata.DeleteAll();

			buffer = csString(LineData).Slice(0, temp5 - 1) + csString(wxVariant(isect.x).GetString().ToAscii()) + csString(wxVariant(isect.y).GetString().ToAscii()) + csString(wxVariant(isect.z).GetString().ToAscii()) + csString(LineData).Slice(temp3 + 1);
			LineData = buffer.GetData();

		}

		//SetAutoSize command
		if (LineData.Slice(0, 11).CompareNoCase("setautosize") == true)
		{
			//get text after equal sign
			temp2 = LineData.Slice(LineData.Find("=", 0) + 1);
			temp2.Trim();

			tempdata.SplitString(temp2.GetData(), ",");
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Simcore->Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer);
			}

			Simcore->SetAutoSize(csString(tempdata[0]).CompareNoCase("true"),
						csString(tempdata[1]).CompareNoCase("true"));

			tempdata.DeleteAll();
		}

		//TextureOverride command
		if (LineData.Slice(0, 15).CompareNoCase("textureoverride") == true)
		{
			tempdata.SplitString(LineData.Slice(16).GetData(), ",");

			Simcore->SetTextureOverride(tempdata[0], tempdata[1], tempdata[2], tempdata[3], tempdata[4], tempdata[5]);

			tempdata.DeleteAll();
			goto Nextline;
		}

		//Process globals
		if (Section == 1)
		{
			//get text after equal sign
			temp2 = LineData.Slice(LineData.Find("=", 0) + 1);
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
				//if (IsNumeric(temp2) == false)
					//Err.Raise 1000;
				Simcore->camera->StartFloor = atoi(temp2.GetData());
			}
			if (LineData.Slice(0, 10).CompareNoCase("horizscale") == true)
			{
				//if (IsNumeric(temp2) == false)
					//Err.Raise 1000;
				Simcore->HorizScale = atof(temp2.GetData());
			}
			if (LineData.Slice(0, 14).CompareNoCase("cameraposition") == true)
			{
				Simcore->camera->StartPositionX  = atof(temp2.Slice(0, temp2.Find(",", 0)).GetData());
				Simcore->camera->StartPositionZ  = atof(temp2.Slice(temp2.Find(",", 0) + 1).GetData());
			}
			if (LineData.Slice(0, 15).CompareNoCase("cameradirection") == true)
			{
				temp3 = temp2.Find(",", 0);
				temp4 = temp2.Find(",", temp3 + 1);
				Simcore->camera->SetStartDirection(csVector3(atof(temp2.Slice(0, temp3).GetData()), atof(temp2.Slice(temp3 + 1, temp4 - temp3 - 1).GetData()), atof(temp2.Slice(temp4 + 1).GetData())));
			}
			if (LineData.Slice(0, 14).CompareNoCase("camerarotation") == true)
			{
				temp3 = temp2.Find(",", 0);
				temp4 = temp2.Find(",", temp3 + 1);
				Simcore->camera->SetStartRotation(csVector3(atof(temp2.Slice(1, temp3).GetData()), atof(temp2.Slice(temp3 + 1, temp4 - temp3 - 1).GetData()), atof(temp2.Slice(temp4 + 1).GetData())));
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
			temp2 = LineData.Slice(LineData.Find("=", 0) + 1);
			temp2.Trim();

			//parameters
			if (LineData.Slice(0, 6).CompareNoCase("height") == true)
			{
				//If IsNumeric(temp2) = False Then Err.Raise 1000
				Simcore->GetFloor(Current)->Height = atof(temp2.GetData());
				if (FloorCheck < 2)
					FloorCheck = 1;
				else
					FloorCheck = 3;
			}
			if (LineData.Slice(0, 16).CompareNoCase("interfloorheight") == true)
			{
				//If IsNumeric(temp2) = False Then Err.Raise 1000
				Simcore->GetFloor(Current)->InterfloorHeight = atof(temp2.GetData());
				if (FloorCheck == 0)
					FloorCheck = 2;
				else
					FloorCheck = 3;
			}
			if (LineData.Slice(0, 8).CompareNoCase("altitude") == true)
			{
				//If IsNumeric(temp2) = False Then Err.Raise 1000
				Simcore->GetFloor(Current)->Altitude = atof(temp2.GetData());
			}
			if (LineData.Slice(0, 2).CompareNoCase("id") == true)
				Simcore->GetFloor(Current)->ID = Simcore->Calc(temp2);
			if (LineData.Slice(0, 4).CompareNoCase("name") == true)
				Simcore->GetFloor(Current)->Name = temp2;
			if (LineData.Slice(0, 4).CompareNoCase("type") == true)
				Simcore->GetFloor(Current)->FloorType = temp2;
			if (LineData.Slice(0, 11).CompareNoCase("description") == true)
				Simcore->GetFloor(Current)->Description = temp2;
			if (LineData.Slice(0, 16).CompareNoCase("indicatortexture") == true)
				Simcore->GetFloor(Current)->IndicatorTexture = temp2;
			if (LineData.Slice(0, 5).CompareNoCase("group") == true)
			{
				//copy string listing of group floors into array
				tempdata.SplitString(temp2.GetData(), ",");
				for (int i = 0; i < tempdata.GetSize(); i++)
				{
					csString tmpstring = tempdata[i];
					tmpstring.Trim();
					if (tmpstring.Find("-") > 0)
					{
						//found a range marker
						int start = atoi(tmpstring.Slice(0, tmpstring.Find("-")));
						int end = atoi(tmpstring.Slice(tmpstring.Find("-") + 1));
						for (int k = start; k <= end; k++)
							Simcore->GetFloor(Current)->AddGroupFloor(k);
					}
					else
						Simcore->GetFloor(Current)->AddGroupFloor(atoi(tempdata[i]));
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
				if (Simcore->IfProc(temp2) == true)
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
			if (LineData.Slice(0, 8).CompareNoCase("addfloor") == true)
			{
				//get data
				tempdata.SplitString(LineData.Slice(9).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Simcore->Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				//if (tempdata.GetSize() < 8)
					//Err.Raise 1003;
				//If IsNumeric(tempdata(1)) = False Or IsNumeric(tempdata(2)) = False Or IsNumeric(tempdata(3)) = False Or IsNumeric(tempdata(4)) = False Or IsNumeric(tempdata(5)) = False Or IsNumeric(tempdata(6)) = False Or IsNumeric(tempdata(7)) = False Then Err.Raise 1000

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
					buffer = Simcore->Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				//if (tempdata.GetSize() < 8)
					//Err.Raise 1003;
				//If IsNumeric(tempdata(1)) = False Or IsNumeric(tempdata(2)) = False Or IsNumeric(tempdata(3)) = False Or IsNumeric(tempdata(4)) = False Or IsNumeric(tempdata(5)) = False Or IsNumeric(tempdata(6)) = False Or IsNumeric(tempdata(7)) = False Then Err.Raise 1000

				//create floor
				Simcore->GetShaft(atoi(tempdata[0]))->AddFloor(Current, tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]));

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
					buffer = Simcore->Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				//if (tempdata.GetSize() < 8)
					//Err.Raise 1003;
				//If IsNumeric(tempdata(1)) = False Or IsNumeric(tempdata(2)) = False Or IsNumeric(tempdata(3)) = False Or IsNumeric(tempdata(4)) = False Or IsNumeric(tempdata(5)) = False Or IsNumeric(tempdata(6)) = False Or IsNumeric(tempdata(7)) = False Then Err.Raise 1000

				//create floor
				if (Simcore->GetStairs(atoi(tempdata[0])))
					Simcore->GetStairs(atoi(tempdata[0]))->AddFloor(Current, tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]));

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
					buffer = Simcore->Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				//if (tempdata.GetSize() < 7)
				//	Err.Raise 1003;
				//If IsNumeric(tempdata(1)) = False Or IsNumeric(tempdata(2)) = False Or IsNumeric(tempdata(3)) = False Or IsNumeric(tempdata(4)) = False Or IsNumeric(tempdata(5)) = False Or IsNumeric(tempdata(6)) = False Or IsNumeric(tempdata(7)) = False Then Err.Raise 1000

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
					buffer = Simcore->Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				//if (tempdata.GetSize() < 11)
					//Err.Raise 1003;
				//If IsNumeric(tempdata(1)) = False Or IsNumeric(tempdata(2)) = False Or IsNumeric(tempdata(3)) = False Or IsNumeric(tempdata(4)) = False Or IsNumeric(tempdata(5)) = False Or IsNumeric(tempdata(6)) = False Or IsNumeric(tempdata(7)) = False Or IsNumeric(tempdata(8)) = False Or IsNumeric(tempdata(9)) = False Or IsNumeric(tempdata(10)) = False Then Err.Raise 1000

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
					buffer = Simcore->Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				//if (tempdata.GetSize() < 11)
					//Err.Raise 1003;
				//If IsNumeric(tempdata(1)) = False Or IsNumeric(tempdata(2)) = False Or IsNumeric(tempdata(3)) = False Or IsNumeric(tempdata(4)) = False Or IsNumeric(tempdata(5)) = False Or IsNumeric(tempdata(6)) = False Or IsNumeric(tempdata(7)) = False Or IsNumeric(tempdata(8)) = False Or IsNumeric(tempdata(9)) = False Or IsNumeric(tempdata(10)) = False Then Err.Raise 1000

				//create wall
				Simcore->GetShaft(atoi(tempdata[0]))->AddWall(Current, tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12]), atof(tempdata[13]));

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
					buffer = Simcore->Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				//if (tempdata.GetSize() < 11)
					//Err.Raise 1003;
				//If IsNumeric(tempdata(1)) = False Or IsNumeric(tempdata(2)) = False Or IsNumeric(tempdata(3)) = False Or IsNumeric(tempdata(4)) = False Or IsNumeric(tempdata(5)) = False Or IsNumeric(tempdata(6)) = False Or IsNumeric(tempdata(7)) = False Or IsNumeric(tempdata(8)) = False Or IsNumeric(tempdata(9)) = False Or IsNumeric(tempdata(10)) = False Then Err.Raise 1000

				//create wall
				if (Simcore->GetStairs(atoi(tempdata[0])))
					Simcore->GetStairs(atoi(tempdata[0]))->AddWall(Current, tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12]), atof(tempdata[13]));

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
					buffer = Simcore->Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				//if (tempdata.GetSize() < 10)
					//Err.Raise 1003;
				//If IsNumeric(tempdata(1)) = False Or IsNumeric(tempdata(2)) = False Or IsNumeric(tempdata(3)) = False Or IsNumeric(tempdata(4)) = False Or IsNumeric(tempdata(5)) = False Or IsNumeric(tempdata(6)) = False Or IsNumeric(tempdata(7)) = False Or IsNumeric(tempdata(8)) = False Or IsNumeric(tempdata(9)) = False Or IsNumeric(tempdata(10)) = False Then Err.Raise 1000

				//create wall
				Simcore->GetFloor(Current)->AddInterfloorWall(tempdata[0], tempdata[1], atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12]));
				tempdata.DeleteAll();
			}

			//Set command
			if (LineData.Slice(0, 4).CompareNoCase("set ") == true)
			{
				temp1 = LineData.Find("=", 0);
				temp3 = atoi(LineData.Slice(4, temp1 - 5));
				temp2 = LineData.Slice(temp1 + 1);
				//if (temp3 < 0 !! temp3 > UBound(Simcore->UserVariable))
					//Err.Raise 1001
				Simcore->UserVariable[temp3] = Simcore->Calc(temp2);
				//Report("Variable " + csString(_itoa(temp3, intbuffer, 10)) + " set to " + Simcore->UserVariable[temp3]);
			}

			//CallButtonElevators command
			if (LineData.Slice(0, 19).CompareNoCase("callbuttonelevators") == true)
			{
				//get text after equal sign
				temp2 = LineData.Slice(LineData.Find("=", 0) + 1);

				//construct array containing floor numbers
				tempdata.SplitString(temp2.GetData(), ",");
				callbutton_elevators.DeleteAll();
				callbutton_elevators.SetSize(tempdata.GetSize());

				for (int i = 0; i < tempdata.GetSize(); i++)
					callbutton_elevators[i] = atoi(tempdata[i]);

				tempdata.DeleteAll();
			}

			//CreateCallButtons command
			if (LineData.Slice(0, 17).CompareNoCase("createcallbuttons") == true)
			{
				if (callbutton_elevators.GetSize() == 0)
				{
					Simcore->Report("Error: Trying to create call buttons, but no elevators specified");
					goto Nextline;
				}

				tempdata.SplitString(LineData.Slice(18).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Simcore->Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}

				//create call button
				Simcore->GetFloor(Current)->AddCallButtons(callbutton_elevators, tempdata[0], tempdata[1], tempdata[2], atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), tempdata[6], atof(tempdata[7]), atof(tempdata[8]), csString(tempdata[9]).CompareNoCase("true"), atof(tempdata[10]), atof(tempdata[11]));
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
					buffer = Simcore->Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}

				//create stairs
				if (Simcore->GetStairs(atoi(tempdata[0])))
					Simcore->GetStairs(atoi(tempdata[0]))->AddStairs(Current, tempdata[1], tempdata[2], tempdata[3], atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atoi(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12]));
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
					buffer = Simcore->Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
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
					buffer = Simcore->Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}

				//create door
				if (Simcore->GetStairs(atoi(tempdata[0])))
					Simcore->GetStairs(atoi(tempdata[0]))->AddDoor(Current, tempdata[1], atof(tempdata[2]), atoi(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]));
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
					buffer = Simcore->Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
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
						i = RangeStart;  //loop back
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
						i = RangeStart; //loop back
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
			temp2 = LineData.Slice(LineData.Find("=", 0) + 1);

			//parameters
			if (LineData.Slice(0, 5).CompareNoCase("speed") == true)
			{
				//If IsNumeric(temp2) = False Then Err.Raise 1000
				Simcore->GetElevator(Current)->ElevatorSpeed = atof(temp2.GetData());
			}
			if (LineData.Slice(0, 12).CompareNoCase("acceleration") == true)
			{
				//If IsNumeric(temp2) = False Then Err.Raise 1000
				Simcore->GetElevator(Current)->Acceleration = atof(temp2.GetData());
			}
			if (LineData.Slice(0, 12).CompareNoCase("deceleration") == true)
			{
				//If IsNumeric(temp2) = False Then Err.Raise 1000
				Simcore->GetElevator(Current)->Deceleration = atof(temp2.GetData());
			}
			if (LineData.Slice(0, 9).CompareNoCase("openspeed") == true)
			{
				//If IsNumeric(temp2) = False Then Err.Raise 1000
				Simcore->GetElevator(Current)->OpenSpeed = atof(temp2.GetData());
			}
			if (LineData.Slice(0, 9).CompareNoCase("acceljerk") == true)
			{
				//If IsNumeric(temp2) = False Then Err.Raise 1000
				Simcore->GetElevator(Current)->AccelJerk = atof(temp2.GetData());
			}
			if (LineData.Slice(0, 9).CompareNoCase("deceljerk") == true)
			{
				//If IsNumeric(temp2) = False Then Err.Raise 1000
				Simcore->GetElevator(Current)->DecelJerk = atof(temp2.GetData());
			}
			if (LineData.Slice(0, 14).CompareNoCase("servicedfloors") == true)
			{
				//copy string listing of serviced floors into array
				tempdata.SplitString(temp2.GetData(), ",");
				for (int i = 0; i < tempdata.GetSize(); i++)
				{
					csString tmpstring = tempdata[i];
					tmpstring.Trim();
					int searchpos = tmpstring.Find("-");
					if (searchpos > 0)
					{
						//found a range marker
						int start = atoi(tmpstring.Slice(0, tmpstring.Find("-")));
						int end = atoi(tmpstring.Slice(tmpstring.Find("-") + 1));
						for (int k = start; k <= end; k++)
							Simcore->GetElevator(Current)->AddServicedFloor(k);
					}
					else
						Simcore->GetElevator(Current)->AddServicedFloor(atoi(tempdata[i]));
				}
				tempdata.DeleteAll();
			}
			if (LineData.Slice(0, 13).CompareNoCase("assignedshaft") == true)
			{
				//If IsNumeric(temp2) = False Then Err.Raise 1000
				Simcore->GetElevator(Current)->AssignedShaft = atoi(temp2.GetData());
			}
			if (LineData.Slice(0, 9).CompareNoCase("doortimer") == true)
			{
				//If IsNumeric(temp2) = False Then Err.Raise 1000
				Simcore->GetElevator(Current)->DoorTimer = atof(temp2.GetData());
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
				if (Simcore->IfProc(temp2) == true)
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
					buffer = Simcore->Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				//if (tempdata.GetSize() < 3)
					//Err.Raise 1003;
				//If IsNumeric(tempdata(1)) = False Or IsNumeric(tempdata(2)) = False Or IsNumeric(tempdata(3)) = False Then Err.Raise 1000
				Simcore->GetElevator(Current)->CreateElevator(atof(tempdata[0]), atof(tempdata[1]), atoi(tempdata[2]));
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
					buffer = Simcore->Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				//if (tempdata.GetSize() < 8)
					//Err.Raise 1003;
				//If IsNumeric(tempdata(1)) = False Or IsNumeric(tempdata(2)) = False Or IsNumeric(tempdata(3)) = False Or IsNumeric(tempdata(4)) = False Or IsNumeric(tempdata(5)) = False Or IsNumeric(tempdata(6)) = False Or IsNumeric(tempdata(7)) = False Then Err.Raise 1000

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
					buffer = Simcore->Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				//if (tempdata.GetSize() < 11)
					//Err.Raise 1003;
				//If IsNumeric(tempdata(1)) = False Or IsNumeric(tempdata(2)) = False Or IsNumeric(tempdata(3)) = False Or IsNumeric(tempdata(4)) = False Or IsNumeric(tempdata(5)) = False Or IsNumeric(tempdata(6)) = False Or IsNumeric(tempdata(7)) = False Or IsNumeric(tempdata(8)) = False Or IsNumeric(tempdata(9)) = False Or IsNumeric(tempdata(10)) = False Then Err.Raise 1000

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
					buffer = Simcore->Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				//if (tempdata.GetSize() < 11)
					//Err.Raise 1003;
				//If IsNumeric(tempdata(1)) = False Or IsNumeric(tempdata(2)) = False Or IsNumeric(tempdata(3)) = False Or IsNumeric(tempdata(4)) = False Or IsNumeric(tempdata(5)) = False Or IsNumeric(tempdata(6)) = False Or IsNumeric(tempdata(7)) = False Or IsNumeric(tempdata(8)) = False Or IsNumeric(tempdata(9)) = False Or IsNumeric(tempdata(10)) = False Then Err.Raise 1000

				Simcore->GetElevator(Current)->AddDoors(tempdata[0], atof(tempdata[1]), atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), csString(tempdata[6]).CompareNoCase("true"), atof(tempdata[7]), atof(tempdata[8]));

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
					buffer = Simcore->Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
				//if (tempdata.GetSize() < 11)
					//Err.Raise 1003;
				//If IsNumeric(tempdata(1)) = False Or IsNumeric(tempdata(2)) = False Or IsNumeric(tempdata(3)) = False Or IsNumeric(tempdata(4)) = False Or IsNumeric(tempdata(5)) = False Or IsNumeric(tempdata(6)) = False Or IsNumeric(tempdata(7)) = False Or IsNumeric(tempdata(8)) = False Or IsNumeric(tempdata(9)) = False Or IsNumeric(tempdata(10)) = False Then Err.Raise 1000

				Simcore->GetElevator(Current)->AddShaftDoors(tempdata[0], atof(tempdata[1]), atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]));

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
					buffer = Simcore->Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}

				Simcore->GetElevator(Current)->CreateButtonPanel(tempdata[0], atoi(tempdata[1]), atoi(tempdata[2]), tempdata[3], atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), atof(tempdata[12]));

				tempdata.DeleteAll();
			}

			//AddFloorButton command
			if (LineData.Slice(0, 14).CompareNoCase("addfloorbutton") == true)
			{
				if (!Simcore->GetElevator(Current)->Panel)
				{
					Simcore->Report("Elevator " + csString(_itoa(Current, intbuffer, 10)) + ": cannot add button");
					goto Nextline;
				}

				//get data
				tempdata.SplitString(LineData.Slice(15).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Simcore->Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}

				Simcore->GetElevator(Current)->Panel->AddFloorButton(tempdata[0], atoi(tempdata[1]), atoi(tempdata[2]), atoi(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]));

				tempdata.DeleteAll();
			}

			//AddControlButton command
			if (LineData.Slice(0, 16).CompareNoCase("addcontrolbutton") == true)
			{
				if (!Simcore->GetElevator(Current)->Panel)
				{
					Simcore->Report("Elevator " + csString(_itoa(Current, intbuffer, 10)) + ": cannot add button");
					goto Nextline;
				}

				//get data
				tempdata.SplitString(LineData.Slice(17).GetData(), ",");

				//calculate inline math
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Simcore->Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}

				Simcore->GetElevator(Current)->Panel->AddControlButton(tempdata[0], atoi(tempdata[1]), atoi(tempdata[2]), tempdata[3], atof(tempdata[4]), atof(tempdata[5]));

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
					buffer = Simcore->Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}

				Simcore->GetElevator(Current)->AddFloorIndicator(tempdata[0], atof(tempdata[1]), atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]));

				tempdata.DeleteAll();
			}


			//Set command
			if (LineData.Slice(0, 4).CompareNoCase("set ") == true)
			{
				temp1 = LineData.Find("=", 0);
				temp3 = atoi(LineData.Slice(4, temp1 - 5));
				temp2 = LineData.Slice(temp1 + 1);
				//if (temp3 < 0 !! temp3 > UBound(Simcore->UserVariable))
					//Err.Raise 1001
				Simcore->UserVariable[temp3] = Simcore->Calc(temp2);
				Simcore->Report("Variable " + csString(_itoa(temp3, intbuffer, 10)) + " set to " + Simcore->UserVariable[temp3]);
			}

			//handle elevator range
			if (RangeL != RangeH && LineData.Slice(0, 14).CompareNoCase("<endelevators>") == true)
			{
				if (Current < RangeH)
				{
					Current++;
					i = RangeStart;  //loop back
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
				//if (tempdata.GetSize() < 1)
					//Err.Raise 1003;
				buffer = tempdata[0];
				buffer.Insert(0, "/root/");
				Simcore->LoadTexture(buffer.GetData(), tempdata[1], atof(tempdata[2]), atof(tempdata[3]));
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
				//if (tempdata.GetSize() < 1)
					//Err.Raise 1003;
				//If IsNumeric(tempdata(0)) = False Or IsNumeric(tempdata(1)) = False Then Err.Raise 1000
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

Nextline:
		i++;
	}

	return 0;
}

int Skyscraper::LoadDataFile(const char * filename)
{
	//loads a building data file into the runtime buffer
	bool streamnotfinished = true;
	char buffer[1000];

	//make sure file exists
	if (Simcore->vfs->Exists(filename) == false)
		return 1;

	//load file
	csRef<iFile> file (Simcore->vfs->Open(filename, VFS_FILE_READ));

	//exit if an error occurred while loading
	if (!file)
		return 1;

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

	return 0;
}
