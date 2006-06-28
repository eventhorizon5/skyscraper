/*
	Skyscraper 1.1 Alpha - File I/O Processing Routines
	Copyright ©2005-2006 Ryan Thoryk
	http://www.tliquest.net/skyscraper
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

#include <crystalspace.h>
#include <stdlib.h>
#include "sbs.h"
#include "camera.h"
#include "floor.h"
#include "elevator.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine
extern Camera *c; //external pointer to the camera

double AltitudeCheck;

int SBS::LoadBuilding(const char * filename)
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
	//char dbuffer[CVTBUFSIZE];
    bool revX;
	bool revY;
	bool revZ;

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
            Report("Processing globals...");
            goto Nextline;
		}
        if (LineData.CompareNoCase("<endglobals>") == true)
		{
            InitMeshes();
            Section = 0;
            Context = "None";
            Report("Finished globals");
            goto Nextline;
		}
        if (LineData.CompareNoCase("<external>") == true)
		{
            Section = 3;
            Context = "External";
            Report("Processing external objects...");
            goto Nextline;
		}
		if (LineData.CompareNoCase("<endexternal>") == true)
		{
            Section = 0;
            Context = "None";
            Report("Finished external");
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
            Report("Processing floors " + csString(_itoa(RangeL, intbuffer, 10)) + " to " + csString(_itoa(RangeH, intbuffer, 10)) + "...");
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
            Report("Processing floor " + csString(_itoa(Current, intbuffer, 10)) + "...");
            goto Nextline;
		}
        if (LineData.CompareNoCase("<endfloor>") == true)
		{
            Section = 0;
            Context = "None";
            Report("Finished floor");
            goto Nextline;
		}
        if (LineData.Slice(0, 10).CompareNoCase("<elevators") == true)
		{
            Section = 4;
            temp3 = csString(LineData).Downcase().Find("to", 10);
			RangeL = atoi(LineData.Slice(11, temp3 - 12).GetData());
            RangeH = atoi(LineData.Slice(temp3 + 2).GetData());
            Context = "Elevator range " + csString(_itoa(RangeL, intbuffer, 10)) + " to " + csString(_itoa(RangeH, intbuffer, 10));
            //if (RangeL < 1 !! RangeH > Elevators)
				//Err.Raise 1006;
            Current = RangeL;
            RangeStart = i;
            Report("Processing elevators " + csString(_itoa(RangeL, intbuffer, 10)) + " to " + csString(_itoa(RangeH, intbuffer, 10)) + "...");
            goto Nextline;
		}
        if (LineData.Slice(0, 10).CompareNoCase("<elevator ") == true)
		{
            Section = 4;
            Context = "Elevator";
            RangeL = 0;
            RangeH = 0;
            Current = atoi(LineData.Slice(10, LineData.Length() - 10).GetData());
            //if (Current < 1 !! Current > Elevators)
				//Err.Raise 1007;
            Report("Processing elevator " + csString(_itoa(Current, intbuffer, 10)) + "...");
            goto Nextline;
		}
        if (LineData.CompareNoCase("<endelevator>") == true)
		{
            Section = 0;
            Context = "None";
            Report("Finished elevator");
            goto Nextline;
		}
        if (LineData.Slice(0, 10).CompareNoCase("<textures>") == true)
		{        
			Section = 5;
            Context = "Textures";
            Report("Processing textures...");
            goto Nextline;
		}
        if (LineData.Slice(0, 13).CompareNoCase("<endtextures>") == true)
		{        
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
		
        
        //User variable conversion
        temp1 = LineData.Find("%", 0);
		if (temp1 > 0)
            temp3 = LineData.Find("%", temp1 + 1);
        else
            temp3 = 0;

		if (temp1 + temp3 > 0)
		{
			temp2 = LineData.Slice(temp1 + 1, temp3 - temp1 - 1).Trim();
			if (IsNumeric(temp2.GetData()) == true)
			{
				//if (temp2 < 0 !! temp2 > UBound(UserVariable))
					//Err.Raise 1001
				while (temp1 + temp3 > 0)
				{
	                LineData.ReplaceAll("%" + temp2 + "%", UserVariable[atoi(temp2.GetData())]);
					temp1 = LineData.Find("%", 0);
					if (temp1 > 0)
	                    temp3 = LineData.Find("%", temp1 + 1);
					else
	                    temp3 = 0;
					if (temp1 + temp3 > 0)
						temp2 = LineData.Slice(temp1 + 1, temp3 - temp1 - 1).Trim();
				}
			}
		}

        //Floor object conversion
		temp5 = csString(LineData).Downcase().Find("floor(", 0);
        while (temp5 > -1)
		{
            temp1 = LineData.Find("(", 0);
            temp3 = LineData.Find(")", 0);
            temp4 = atoi(LineData.Slice(temp1 + 1, temp3 - temp1 - 1).GetData());
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
				buffer = FloorArray[temp4]->FullHeight();
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
				buffer = FloorArray[temp4]->Altitude;
                LineData = LineData.Slice(0, temp1) + buffer.Trim() + LineData.Slice(temp1 + temp6.Length());
			}
			temp5 = csString(LineData).Downcase().Find("floor(", 0);
        }
        
        //Set command
        if (LineData.Slice(0, 4).CompareNoCase("set ") == true)
		{
            temp1 = LineData.Find("=", 0);
            temp3 = atoi(LineData.Slice(4, temp1 - 5));
            temp2 = LineData.Slice(temp1 + 1);
            //if (temp3 < 0 !! temp3 > UBound(UserVariable))
				//Err.Raise 1001
            UserVariable[temp3] = Calc(temp2);
            Report("Variable " + csString(_itoa(temp3, intbuffer, 10)) + " set to " + UserVariable[temp3]);
		}
        
        //CreateWallBox2 command
        if (LineData.Slice(0, 14).CompareNoCase("createwallbox2") == true)
		{
            tempdata.SplitString(LineData.Slice(15).GetData(), ",");
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer);
			}
            //if (tempdata.GetSize < 9)
				//Err.Raise 1003;
            if (csString(tempdata[0]).CompareNoCase("floor") == true)
				tmpMesh = sbs->FloorArray[Current]->Level_state;
			else
			{
                if (Section == 2)
				{
					buffer = FloorArray[Current]->Altitude + atof(tempdata[7]);
					tempdata.Put(7, buffer);
				}
			}
            buffer = tempdata[0];
			buffer.Downcase();
			if (buffer == "external")
				tmpMesh = sbs->External_state;
            if (buffer == "landscape")
				tmpMesh = sbs->Landscape_state;
            if (buffer == "buildings")
				tmpMesh = sbs->Buildings_state;
            if (buffer == "columnframe")
				tmpMesh = sbs->ColumnFrame_state;
            //if IsNumeric(tempdata(2)) = False Or IsNumeric(tempdata(3)) = False Or IsNumeric(tempdata(4)) = False Or IsNumeric(tempdata(5)) = False Or IsNumeric(tempdata(6)) = False Or IsNumeric(tempdata(7)) = False Or IsNumeric(tempdata(8)) = False Or IsNumeric(tempdata(9)) = False Then Err.Raise 1000
            CreateWallBox2(tmpMesh, tempdata[1], atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]));
			tempdata.DeleteAll();
		}
            
        //CreateWallBox command
        if (LineData.Slice(0, 14).CompareNoCase("createwallbox ") == true)
		{
            tempdata.SplitString(LineData.Slice(15).GetData(), ",");
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer);
			}
            //if (tempdata.GetSize < 9)
				//Err.Raise 1003
            if (csString(tempdata[0]).CompareNoCase("floor") == true)
                tmpMesh = sbs->FloorArray[Current]->Level_state;
			else
			{
                if (Section == 2)
				{
					buffer = FloorArray[Current]->Altitude + atof(tempdata[7]);
					tempdata.Put(7, buffer);
				}
			}
            buffer = tempdata[0];
			buffer.Downcase();
			if (buffer == "external")
				tmpMesh = sbs->External_state;
            if (buffer == "landscape")
				tmpMesh = sbs->Landscape_state;
            if (buffer == "buildings")
				tmpMesh = sbs->Buildings_state;
            if (buffer == "columnframe")
				tmpMesh = sbs->ColumnFrame_state;
            //If IsNumeric(tempdata(2)) = False Or IsNumeric(tempdata(3)) = False Or IsNumeric(tempdata(4)) = False Or IsNumeric(tempdata(5)) = False Or IsNumeric(tempdata(6)) = False Or IsNumeric(tempdata(7)) = False Or IsNumeric(tempdata(8)) = False Or IsNumeric(tempdata(9)) = False Then Err.Raise 1000
            CreateWallBox(tmpMesh, tempdata[1], atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]));
			tempdata.DeleteAll();
		}

		//AddCustomWall command
        if (LineData.Slice(0, 14).CompareNoCase("addcustomwall ") == true)
		{
            bool extcheck = false;
			bool revX;
			bool revY;
			bool revZ;
			tempdata.SplitString(LineData.Slice(14).GetData(), ",");
			for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
			{
				buffer = Calc(tempdata[temp3]);
				tempdata.Put(temp3, buffer);
			}
            if (csString(tempdata[0]).CompareNoCase("floor") == true)
                tmpMesh = sbs->FloorArray[Current]->Level_state;
			else
			{
                if (Section == 2)
				{
					buffer = FloorArray[Current]->Altitude + atof(tempdata[7]);
					tempdata.Put(7, buffer);
				}
			}
            buffer = tempdata[0];
			buffer.Downcase();
			if (buffer == "external")
			{
				tmpMesh = sbs->External_state;
				extcheck = true;
			}
            if (buffer == "landscape")
				tmpMesh = sbs->Landscape_state;
            if (buffer == "buildings")
				tmpMesh = sbs->Buildings_state;
            if (buffer == "columnframe")
				tmpMesh = sbs->ColumnFrame_state;
			
			csPoly3D varray;
			int alength;
			alength = tempdata.Length();
			for (temp3 = 2; temp3 < alength - 5; temp3 += 3)
				varray.AddVertex(atof(tempdata[temp3]), atof(tempdata[temp3 + 1]), atof(tempdata[temp3 + 2]));
			
			if (csString(tempdata[alength - 3]).CompareNoCase("true") == true)
				revX = true;
			else
				revX = false;
			if (csString(tempdata[alength - 2]).CompareNoCase("true") == true)
				revY = true;
			else
				revY = false;
			if (csString(tempdata[alength - 1]).CompareNoCase("true") == true)
				revZ = true;
			else
				revZ = false;
			
			AddCustomWall(tmpMesh, tempdata[1], varray, atof(tempdata[alength - 5]), atof(tempdata[alength - 4]), revX, revY, revZ, extcheck);
			tempdata.DeleteAll();
		}
        
        //Process globals
        if (Section == 1)
		{
            //get text after equal sign
            temp2 = LineData.Slice(LineData.Find("=", 0) + 1);
			temp2.Trim();

            //store variable values
            if (LineData.Slice(0, 4).CompareNoCase("name") == true)
				BuildingName = temp2;
            if (LineData.Slice(0, 8).CompareNoCase("designer") == true)
				BuildingDesigner = temp2;
            if (LineData.Slice(0, 8).CompareNoCase("location") == true)
				BuildingLocation = temp2;
            if (LineData.Slice(0, 11).CompareNoCase("description") == true)
				BuildingDescription = temp2;
            if (LineData.Slice(0, 7).CompareNoCase("version") == true)
				BuildingVersion = temp2;
            if (LineData.Slice(0, 14).CompareNoCase("cameraaltitude") == true)
			{
                //if (IsNumeric(temp2) == false)
					//Err.Raise 1000;
                c->DefaultAltitude = atof(temp2.GetData());
			}
            if (LineData.Slice(0, 14).CompareNoCase("elevatorshafts") == true)
			{
                //if (IsNumeric(temp2) == false)
					//Err.Raise 1000;
                ElevatorShafts = atoi(temp2.GetData());
			}
            if (LineData.Slice(0, 6).CompareNoCase("floors") == true)
			{
                //if (IsNumeric(temp2) == false)
					//Err.Raise 1000;
                TotalFloors = atoi(temp2.GetData());
			}
            if (LineData.Slice(0, 9).CompareNoCase("basements") == true)
			{
                //if (IsNumeric(temp2) == false)
					//Err.Raise 1000;
                Basements = atoi(temp2.GetData());
			}
            if (LineData.Slice(0, 9).CompareNoCase("elevators") == true)
			{
                //if (IsNumeric(temp2) == false)
					//Err.Raise 1000;
                Elevators = atoi(temp2.GetData());
			}
            if (LineData.Slice(0, 10).CompareNoCase("pipeshafts") == true)
			{
                //if (IsNumeric(temp2) == false)
					//Err.Raise 1000;
                PipeShafts = atoi(temp2.GetData());
			}
            if (LineData.Slice(0, 6).CompareNoCase("stairs") == true)
			{
                //if (IsNumeric(temp2) == false)
					//Err.Raise 1000;
                StairsNum = atoi(temp2.GetData());
			}
            if (LineData.Slice(0, 11).CompareNoCase("camerafloor") == true)
			{
                //if (IsNumeric(temp2) == false)
					//Err.Raise 1000;
                c->StartFloor = atoi(temp2.GetData());
			}
            if (LineData.Slice(0, 10).CompareNoCase("horizscale") == true)
			{
                //if (IsNumeric(temp2) == false)
					//Err.Raise 1000;
                HorizScale = atof(temp2.GetData());
			}
            if (LineData.Slice(0, 14).CompareNoCase("cameraposition") == true)
			{
                c->StartPositionX  = atof(temp2.Slice(0, temp2.Find(",", 0)).GetData());
                c->StartPositionZ  = atof(temp2.Slice(temp2.Find(",", 0) + 1).GetData());
			}
            if (LineData.Slice(0, 15).CompareNoCase("cameradirection") == true)
			{
                temp3 = temp2.Find(",", 0);
                temp4 = temp2.Find(",", temp3 + 1);
				c->SetStartDirection(csVector3(atof(temp2.Slice(0, temp3).GetData()), atof(temp2.Slice(temp3 + 1, temp4 - temp3 - 1).GetData()), atof(temp2.Slice(temp4 + 1).GetData())));
			}
            if (LineData.Slice(0, 14).CompareNoCase("camerarotation") == true)
			{
                temp3 = temp2.Find(",", 0);
				temp4 = temp2.Find(",", temp3 + 1);
                c->SetStartRotation(csVector3(atof(temp2.Slice(1, temp3).GetData()), atof(temp2.Slice(temp3 + 1, temp4 - temp3 - 1).GetData()), atof(temp2.Slice(temp4 + 1).GetData())));
			}
		}

        //Process floors
        if (Section == 2)
		{

recalc:
            //replace variables with actual values
			buffer = Current;
            LineData.ReplaceAll("%floor%", buffer);
			buffer = FloorArray[Current]->Height;
            LineData.ReplaceAll("%height%", buffer);
			buffer = FloorArray[Current]->FullHeight();
            LineData.ReplaceAll("%fullheight%", buffer);
			buffer = FloorArray[Current]->InterfloorHeight;
            LineData.ReplaceAll("%interfloorheight%", buffer);
			buffer = FloorArray[Current]->DoorHeight;
            LineData.ReplaceAll("%doorheight%", buffer);
			buffer = FloorArray[Current]->DoorWidth;
            LineData.ReplaceAll("%doorwidth%", buffer);
            
            //get text after equal sign
			temp2 = LineData.Slice(LineData.Find("=", 0) + 1);
            temp2.Trim();

            //parameters
            if (LineData.Slice(0, 6).CompareNoCase("height") == true)
			{
                //If IsNumeric(temp2) = False Then Err.Raise 1000
				FloorArray[Current]->Height = atof(temp2.GetData());
                if (FloorCheck < 2)
					FloorCheck = 1;
				else
					FloorCheck = 3;
			}
            if (LineData.Slice(0, 16).CompareNoCase("interfloorheight") == true)
			{
                //If IsNumeric(temp2) = False Then Err.Raise 1000
                FloorArray[Current]->InterfloorHeight = atof(temp2.GetData());
                if (FloorCheck == 0)
					FloorCheck = 2;
				else
					FloorCheck = 3;
			}
            if (LineData.Slice(0, 10).CompareNoCase("doorheight") == true)
			{
                //If IsNumeric(temp2) = False Then Err.Raise 1000
                FloorArray[Current]->DoorHeight = atof(temp2.GetData());
			}
            if (LineData.Slice(0, 9).CompareNoCase("doorwidth") == true)
			{
                //If IsNumeric(temp2) = False Then Err.Raise 1000
                FloorArray[Current]->DoorWidth = atof(temp2.GetData());
			}
            if (LineData.Slice(0, 2).CompareNoCase("id") == true)
				FloorArray[Current]->ID = Calc(temp2);
            if (LineData.Slice(0, 4).CompareNoCase("name") == true)
				FloorArray[Current]->Name = temp2;
			if (LineData.Slice(0, 4).CompareNoCase("type") == true)
				FloorArray[Current]->FloorType = temp2;
			if (LineData.Slice(0, 11).CompareNoCase("description") == true)
				FloorArray[Current]->Description = temp2;
			
            //calculate altitude
            if (FloorCheck == 3)
			{
                FloorCheck = 0;
                if (Current == 0)
					FloorArray[Current]->Altitude = 0;
                if (Current > 0)
					FloorArray[Current]->Altitude = FloorArray[Current - 1]->Altitude + FloorArray[Current - 1]->FullHeight();
                if (Current == -1)
					FloorArray[Current]->Altitude = -FloorArray[Current]->FullHeight();
                if (Current < -1)
					FloorArray[Current]->Altitude = FloorArray[Current + 1]->Altitude - FloorArray[Current]->FullHeight();
			}
            
            //IF statement
            if (LineData.Slice(0, 2).CompareNoCase("if") == true)
			{
                temp1 = LineData.Find("(", 0);
				temp3 = LineData.Find(")", 0);
                if (temp1 + temp3 > 0)
					temp2 = LineData.Slice(temp1 + 1, temp3 - temp1 - 1);
				else
					temp2 = "";
				temp2.Trim();
                if (Calc(temp2) == "true")
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
            
            //AddFloor command
            if (LineData.Slice(0, 8).CompareNoCase("addfloor") == true)
			{
                //get data
                tempdata.SplitString(LineData.Slice(9).GetData(), ",");
				
                //calculate inline math
                for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
                    buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
                //if (tempdata.GetSize() < 8)
					//Err.Raise 1003;
                //If IsNumeric(tempdata(1)) = False Or IsNumeric(tempdata(2)) = False Or IsNumeric(tempdata(3)) = False Or IsNumeric(tempdata(4)) = False Or IsNumeric(tempdata(5)) = False Or IsNumeric(tempdata(6)) = False Or IsNumeric(tempdata(7)) = False Then Err.Raise 1000
                
                //create floor
				buffer = tempdata[9];
				if (buffer.CompareNoCase("true") == true)
					FloorArray[Current]->AddFloor(tempdata[0], atof(tempdata[1]), atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), true);
				else
					FloorArray[Current]->AddFloor(tempdata[0], atof(tempdata[1]), atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), false);

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
                //if (tempdata.GetSize() < 7)
				//	Err.Raise 1003;
                //If IsNumeric(tempdata(1)) = False Or IsNumeric(tempdata(2)) = False Or IsNumeric(tempdata(3)) = False Or IsNumeric(tempdata(4)) = False Or IsNumeric(tempdata(5)) = False Or IsNumeric(tempdata(6)) = False Or IsNumeric(tempdata(7)) = False Then Err.Raise 1000
                
                //create floor
				FloorArray[Current]->AddInterfloorFloor(tempdata[0], atof(tempdata[1]), atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]));
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
                //if (tempdata.GetSize() < 11)
					//Err.Raise 1003;
                //If IsNumeric(tempdata(1)) = False Or IsNumeric(tempdata(2)) = False Or IsNumeric(tempdata(3)) = False Or IsNumeric(tempdata(4)) = False Or IsNumeric(tempdata(5)) = False Or IsNumeric(tempdata(6)) = False Or IsNumeric(tempdata(7)) = False Or IsNumeric(tempdata(8)) = False Or IsNumeric(tempdata(9)) = False Or IsNumeric(tempdata(10)) = False Then Err.Raise 1000
                
				if (csString(tempdata[11]).CompareNoCase("true") == true)
					revX = true;
				else
					revX = false;
				if (csString(tempdata[12]).CompareNoCase("true") == true)
					revY = true;
				else
					revY = false;
				if (csString(tempdata[13]).CompareNoCase("true") == true)
					revZ = true;
				else
					revZ = false;

                //create wall
				buffer = tempdata[14];
				if (buffer.CompareNoCase("true") == true)
					FloorArray[Current]->AddWall(tempdata[0], atof(tempdata[1]), atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), revX, revY, revZ, true);
				else
					FloorArray[Current]->AddWall(tempdata[0], atof(tempdata[1]), atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), revX, revY, revZ, false);

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
                //if (tempdata.GetSize() < 10)
					//Err.Raise 1003;
                //If IsNumeric(tempdata(1)) = False Or IsNumeric(tempdata(2)) = False Or IsNumeric(tempdata(3)) = False Or IsNumeric(tempdata(4)) = False Or IsNumeric(tempdata(5)) = False Or IsNumeric(tempdata(6)) = False Or IsNumeric(tempdata(7)) = False Or IsNumeric(tempdata(8)) = False Or IsNumeric(tempdata(9)) = False Or IsNumeric(tempdata(10)) = False Then Err.Raise 1000
                
                //create wall
                FloorArray[Current]->AddInterfloorWall(tempdata[0], atof(tempdata[1]), atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]));
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
        
        //Process external
        if (Section == 3)
		{
        
            //AddTriangleWall command
            if (LineData.Slice(0, 15).CompareNoCase("addtrianglewall") == true)
			{
				//get data
                tempdata.SplitString(LineData.Slice(16).GetData(), ",");
				
                //calculate inline math
                for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
                //if (tempdata.GetSize() < 13)
					//Err.Raise 1003;
                //If IsNumeric(tempdata(1)) = False Or IsNumeric(tempdata(2)) = False Or IsNumeric(tempdata(3)) = False Or IsNumeric(tempdata(4)) = False Or IsNumeric(tempdata(5)) = False Or IsNumeric(tempdata(6)) = False Or IsNumeric(tempdata(7)) = False Or IsNumeric(tempdata(8)) = False Or IsNumeric(tempdata(9)) = False Or IsNumeric(tempdata(10)) = False Or IsNumeric(tempdata(11)) = False Or IsNumeric(tempdata(12)) = False Then Err.Raise 1000

				if (csString(tempdata[12]).CompareNoCase("true") == true)
					revX = true;
				else
					revX = false;
				if (csString(tempdata[13]).CompareNoCase("true") == true)
					revY = true;
				else
					revY = false;
				if (csString(tempdata[14]).CompareNoCase("true") == true)
					revZ = true;
				else
					revZ = false;
                
                //create triangle wall
                AddTriangleWall(External_state, tempdata[0], atof(tempdata[1]), atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), atof(tempdata[11]), revX, revY, revZ, true);
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
                //if (tempdata.GetSize() < 11)
					//Err.Raise 1003;
                //if IsNumeric(tempdata(1)) = False Or IsNumeric(tempdata(2)) = False Or IsNumeric(tempdata(3)) = False Or IsNumeric(tempdata(4)) = False Or IsNumeric(tempdata(5)) = False Or IsNumeric(tempdata(6)) = False Or IsNumeric(tempdata(7)) = False Or IsNumeric(tempdata(8)) = False Or IsNumeric(tempdata(9)) = False Or IsNumeric(tempdata(10)) = False Then Err.Raise 1000
                
				if (csString(tempdata[11]).CompareNoCase("true") == true)
					revX = true;
				else
					revX = false;
				if (csString(tempdata[12]).CompareNoCase("true") == true)
					revY = true;
				else
					revY = false;
				if (csString(tempdata[13]).CompareNoCase("true") == true)
					revZ = true;
				else
					revZ = false;

				//create wall
                AddWallMain(External_state, tempdata[0], atof(tempdata[1]), atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]), atof(tempdata[9]), atof(tempdata[10]), revX, revY, revZ);
				tempdata.DeleteAll();
			}
            
            //AddFloor
            if (LineData.Slice(0, 8).CompareNoCase("addfloor") == true)
			{
                //get data
                tempdata.SplitString(LineData.Slice(9).GetData(), ",");
				
                //calculate inline math
                for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
                //if (tempdata.GetSize() < 7)
					//Err.Raise 1003;
                //If IsNumeric(tempdata(1)) = False Or IsNumeric(tempdata(2)) = False Or IsNumeric(tempdata(3)) = False Or IsNumeric(tempdata(4)) = False Or IsNumeric(tempdata(5)) = False Or IsNumeric(tempdata(6)) = False Or IsNumeric(tempdata(7)) = False Then Err.Raise 1000
                
                //create floor
                AddFloorMain(External_state, tempdata[0], atof(tempdata[1]), atof(tempdata[2]), atof(tempdata[3]), atof(tempdata[4]), atof(tempdata[5]), atof(tempdata[6]), atof(tempdata[7]), atof(tempdata[8]));
				tempdata.DeleteAll();
			}
		}
        
        //Process elevators
        if (Section == 4)
		{
            
            //get text after equal sign
            temp2 = LineData.Slice(LineData.Find("=", 0) + 1);
            
            //parameters
            if (LineData.Slice(0, 5).CompareNoCase("speed") == true)
			{
                //If IsNumeric(temp2) = False Then Err.Raise 1000
                ElevatorArray[Current]->ElevatorSpeed = atof(temp2.GetData());
			}
            if (LineData.Slice(0, 12).CompareNoCase("acceleration") == true)
			{
                //If IsNumeric(temp2) = False Then Err.Raise 1000
                ElevatorArray[Current]->Acceleration = atof(temp2.GetData());
			}
            if (LineData.Slice(0, 12).CompareNoCase("deceleration") == true)
			{
                //If IsNumeric(temp2) = False Then Err.Raise 1000
                ElevatorArray[Current]->Deceleration = atof(temp2.GetData());
			}
            if (LineData.Slice(0, 9).CompareNoCase("openspeed") == true)
			{
                //If IsNumeric(temp2) = False Then Err.Raise 1000
                ElevatorArray[Current]->OpenSpeed = atof(temp2.GetData());
			}
            
            //replace variables with actual values
			buffer = Current;
            LineData.ReplaceAll("%elevator%", buffer);
            
            //IF statement
            if (LineData.Slice(0, 2).CompareNoCase("if") == true)
			{
                temp1 = LineData.Find("(", 0);
                temp3 = LineData.Find(")", 0);
                if (temp1 + temp3 > 0)
					temp2 = LineData.Slice(temp1 + 1, temp3 - temp1 - 1);
				else
					temp2 = "";
				temp2.Trim();
                if (Calc(temp2) == "true")
                    LineData = LineData.Slice(temp3 + 1).Trim(); //trim off IF statement
                else
                    goto Nextline; //skip line
			}
            
            //CreateElevator command
            if (LineData.Slice(0, 14).CompareNoCase("createelevator") == true)
			{
                tempdata.SplitString(LineData.Slice(15).GetData(), ",");
				for (temp3 = 0; temp3 < tempdata.GetSize(); temp3++)
				{
					buffer = Calc(tempdata[temp3]);
					tempdata.Put(temp3, buffer);
				}
                //if (tempdata.GetSize() < 3)
					//Err.Raise 1003;
                //If IsNumeric(tempdata(1)) = False Or IsNumeric(tempdata(2)) = False Or IsNumeric(tempdata(3)) = False Then Err.Raise 1000
                ElevatorArray[Current]->CreateElevator(atof(tempdata[0]), atof(tempdata[1]), atof(tempdata[2]), atof(tempdata[3]));
				tempdata.DeleteAll();
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
				LoadTexture(buffer.GetData(), tempdata[1]);
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
                    //DoEvents
                    temp2 = tempdata[2];
					buffer = Current;
					temp2.ReplaceAll("%number%", buffer.Trim());
					temp6 = tempdata[3];
					temp6.ReplaceAll("%number%", buffer.Trim());
                    LoadTexture("/root/" + temp2, temp6);
				}
				tempdata.DeleteAll();
			}
		}
Nextline:
		i++;
	}

	return 0;
}

int SBS::LoadDataFile(const char * filename)
{
	//loads a building data file into the runtime buffer
	bool streamnotfinished = true;
	char buffer[1000];

	//make sure file exists
	if (vfs->Exists(filename) == false)
		return 1;
	
	//load file
	csRef<iFile> file (vfs->Open(filename, VFS_FILE_READ));

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
