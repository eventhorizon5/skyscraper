/* $Id$ */

/*
	Scalable Building Simulator - Shaft Subsystem Class
	The Skyscraper Project - Version 1.8 Alpha
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

#include "globals.h"
#include "sbs.h"
#include "shaft.h"
#include "camera.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

Shaft::Shaft(int number, int type, float CenterX, float CenterZ, int _startfloor, int _endfloor)
{
	//constructor
	//creates a shaft in the location specified by x1, x2, z1, and z2
	//and that spans the altitude range specified by startalt and endalt
	//types are currently:
	//1 = pipe/utility shaft
	//2 = elevator shaft
	//3 = stairwell shaft

	//set up SBS object
	object = new Object();
	object->SetValues(this, sbs->object, "Shaft", "", false);

	ShaftType = type;
	ShaftNumber = number;
	startfloor = _startfloor;
	endfloor = _endfloor;

	//make sure start and ending floors are within a valid range
	if (startfloor < -sbs->Basements)
		return;
	if (endfloor > sbs->Floors - 1)
		return;

	origin = csVector3(CenterX, sbs->GetFloor(_startfloor)->Altitude, CenterZ);
	InsideShaft = false;
	IsEnabled = true;
	top = sbs->GetFloor(endfloor)->Altitude + sbs->GetFloor(endfloor)->FullHeight();
	bottom = sbs->GetFloor(startfloor)->Altitude;
	cutstart = 0;
	cutend = 0;
	ShowFloors = false;
	ShowOutside = false;
	ShowFullShaft = false;
	EnableCheck = false;
	lastcheckresult = false;
	checkfirstrun = true;

	csString buffer, buffer2, buffer3;

	buffer = number;
	object->SetName("Shaft " + buffer);

	ShaftArray.SetSize(endfloor - startfloor + 1);
	EnableArray.SetSize(endfloor - startfloor + 1);
	shaft_walls.SetSize(endfloor - startfloor + 1);
	shaft_submeshes.SetSize(endfloor - startfloor + 1);
	lights.SetSize(endfloor - startfloor + 1);

	for (int i = startfloor; i <= endfloor; i++)
	{
		//Create shaft meshes
		buffer2 = number;
		buffer3 = i;
		buffer = "Shaft " + buffer2 + ":" + buffer3;
		buffer.Trim();
		csRef<iMeshWrapper> tmpmesh;
		tmpmesh = sbs->CreateMesh(buffer);
		ShaftArray[i - startfloor] = tmpmesh;
		EnableArray[i - startfloor] = true;
	}
}

Shaft::~Shaft()
{
	//destructor

	//delete lights
	for (int i = 0; i < lights.GetSize(); i++)
	{
		for (int j = 0; j < lights[i].GetSize(); j++)
		{
			if (lights[i][j])
				delete lights[i][j];
			lights[i][j] = 0;
		}
	}

	//delete wall objects
	for (int i = 0; i < shaft_walls.GetSize(); i++)
	{
		for (int j = 0; j < shaft_walls[i].GetSize(); j++)
		{
			if (shaft_walls[i][j])
			{
				shaft_walls[i][j]->parent_deleting = true;
				delete shaft_walls[i][j];
			}
			shaft_walls[i][j] = 0;
		}
	}

	//delete mesh array objects
	for (int i = 0; i < ShaftArray.GetSize(); i++)
	{
		if (sbs->FastDelete == false)
			sbs->engine->WantToDie(ShaftArray[i]);
		ShaftArray[i] = 0;
	}
	ShaftArray.DeleteAll();

	//unregister from parent
	if (sbs->FastDelete == false && object->parent_deleting == false)
		sbs->RemoveShaft(this);

	delete object;
}

WallObject* Shaft::AddWall(int floor, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th)
{
	//exit with an error if floor is invalid
	if (IsValidFloor(floor) == false)
	{
		ReportError("AddWall: Floor " + csString(_itoa(floor, intbuffer, 10)) + " out of range");
		return 0;
	}

	WallObject *wall = sbs->CreateWallObject(shaft_walls[floor - startfloor], GetMeshWrapper(floor), shaft_submeshes[floor - startfloor], this->object, name);
	sbs->AddWallMain(wall, name, texture, thickness, origin.x + x1, origin.z + z1, origin.x + x2, origin.z + z2, height1, height2, sbs->GetFloor(floor)->Altitude + voffset1, sbs->GetFloor(floor)->Altitude + voffset2, tw, th, true);
	return wall;
}

WallObject* Shaft::AddFloor(int floor, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, float tw, float th)
{
	//exit with an error if floor is invalid
	if (IsValidFloor(floor) == false)
	{
		sbs->ReportError("Shaft " + csString(_itoa(ShaftNumber, intbuffer, 10)) + " - AddFloor: Floor " + csString(_itoa(floor, intbuffer, 10)) + " out of range");
		return 0;
	}

	//get shaft extents
	float altitude = sbs->GetFloor(floor)->Altitude;

	if (altitude + voffset1 < bottom)
		bottom = altitude + voffset1;
	if (altitude + voffset2 < bottom)
		bottom = altitude + voffset2;
	if (altitude + voffset1 > top)
		top = altitude + voffset1;
	if (altitude + voffset2 > top)
		top = altitude + voffset2;

	WallObject *wall = sbs->CreateWallObject(shaft_walls[floor - startfloor], GetMeshWrapper(floor), shaft_submeshes[floor - startfloor], this->object, name);
	sbs->AddFloorMain(wall, name, texture, thickness, origin.x + x1, origin.z + z1, origin.x + x2, origin.z + z2, altitude + voffset1, altitude + voffset2, tw, th, true);
	return wall;
}

void Shaft::Enabled(int floor, bool value, bool EnableShaftDoors)
{
	if (IsEnabledFloor(floor) != value && floor >= startfloor && floor <= endfloor && EnableCheck == false)
	{
		//turns shaft on/off for a specific floor
		if (value == true)
		{
			sbs->EnableMesh(GetMeshWrapper(floor), value);
			EnableArray[floor - startfloor] = true;
		}
		else
		{
			//leave bottom and top on
			if (floor != startfloor && floor != endfloor)
			{
				sbs->EnableMesh(GetMeshWrapper(floor), value);
				EnableArray[floor - startfloor] = false;
			}
			else
				return;
		}
		if (value == false && (floor == startfloor || floor == endfloor))
			return;

		if (EnableShaftDoors == true)
		{
			for (size_t i = 0; i < elevators.GetSize(); i++)
			{
				Elevator *elevator = sbs->GetElevator(elevators[i]);
				for(size_t j = 0; j < elevator->ServicedFloors.GetSize(); j++)
				{
					if (elevator->ServicedFloors[j] == floor)
						elevator->ShaftDoorsEnabled(0, elevator->ServicedFloors[j], value);
				}
			}
		}
	}
}

bool Shaft::IsShaft(csRef<iMeshWrapper> test)
{
	for (size_t i = 0; i < ShaftArray.GetSize(); i++)
	{
		if (test == ShaftArray[i])
			return true;
	}
	return false;
}

void Shaft::EnableWholeShaft(bool value, bool EnableShaftDoors, bool force)
{
	//turn on/off entire shaft
	
	if (force == true)
		IsEnabled = false;
	
	if (((value == false && IsEnabled == true) || (value == true && IsEnabled == false)) && EnableCheck == false)
	{
		for (int i = startfloor; i <= endfloor; i++)
		{
			if (force == true)
				EnableArray[i - startfloor] = false;
			Enabled(i, value, EnableShaftDoors);
		}
	}
	IsEnabled = value;
	if (ShowFullShaft == true)
		EnableCheck = true;
}

bool Shaft::IsInShaft(const csVector3 &position)
{
	//if last position is the same as new, return previous result
	if (position == lastposition && checkfirstrun == false)
		return lastcheckresult;

	checkfirstrun = false;

	if (position.y > bottom && position.y < top)
	{
		csHitBeamResult result = ShaftArray[0]->HitBeam(sbs->ToRemote(position), sbs->ToRemote(csVector3(position.x, position.y - (top - bottom), position.z)));

		//cache values
		lastcheckresult = result.hit;
		lastposition = position;
		
		return result.hit;
	}
	
	//cache values
	lastcheckresult = false;
	lastposition = position;

	return false;
}

void Shaft::CutFloors(bool relative, const csVector2 &start, const csVector2 &end, float startvoffset, float endvoffset)
{
	//Cut through floor/ceiling polygons on all associated levels, within the voffsets

	sbs->Report("Cutting for shaft " + csString(_itoa(ShaftNumber, intbuffer, 10)) + "...");

	float voffset1, voffset2;
	cutstart = start;
	cutend = end;

	for (int i = startfloor; i <= endfloor; i++)
	{
		Floor *floorptr = sbs->GetFloor(i);
		voffset1 = 0;
		voffset2 = floorptr->FullHeight() + 1;

		if (i == startfloor)
			voffset1 = startvoffset;
		else if (i == endfloor)
			voffset2 = endvoffset;

		if (relative == true)
			floorptr->Cut(csVector3(origin.x + start.x, voffset1, origin.z + start.y), csVector3(origin.x + end.x, voffset2, origin.z + end.y), false, true, false);
		else
			floorptr->Cut(csVector3(start.x, voffset1, start.y), csVector3(end.x, voffset2, end.y), false, true, false);
		floorptr = 0;
	}

	//cut external
	voffset1 = sbs->GetFloor(startfloor)->Altitude + startvoffset;
	voffset2 = sbs->GetFloor(endfloor)->Altitude + sbs->GetFloor(endfloor)->FullHeight() + endvoffset;

	for (int i = 0; i < sbs->External_walls.GetSize(); i++)
	{
		if (relative == true)
			sbs->Cut(sbs->External_walls[i], csVector3(origin.x + start.x, voffset1, origin.z + start.y), csVector3(origin.x + end.x, voffset2, origin.z + end.y), false, true, csVector3(0, 0, 0), csVector3(0, 0, 0));
		else
			sbs->Cut(sbs->External_walls[i], csVector3(start.x, voffset1, start.y), csVector3(end.x, voffset2, end.y), false, true, csVector3(0, 0, 0), csVector3(0, 0, 0));
	}
}

bool Shaft::CutWall(bool relative, int floor, const csVector3 &start, const csVector3 &end, int checkwallnumber, const char *checkstring)
{
	//Cut through a wall segment
	//the Y values in start and end are both relative to the floor's altitude

	//exit with an error if floor is invalid
	if (IsValidFloor(floor) == false)
	{
		ReportError("CutWall: Floor " + csString(_itoa(floor, intbuffer, 10)) + " out of range");
		return false;
	}

	float base = sbs->GetFloor(floor)->Altitude;

	for (int i = 0; i < shaft_walls[floor - startfloor].GetSize(); i++)
	{
		bool reset = true;
		if (i > 0)
			reset = false;

		if (relative == true)
			sbs->Cut(shaft_walls[floor - startfloor][i], csVector3(origin.x + start.x, base + start.y, origin.z + start.z), csVector3(origin.x + end.x, base + end.y, origin.z + end.z), true, false, csVector3(0, 0, 0), origin, checkwallnumber, checkstring, reset);
		else
			sbs->Cut(shaft_walls[floor - startfloor][i], csVector3(start.x, base + start.y, start.z), csVector3(end.x, base + end.y, end.z), true, false, csVector3(0, 0, 0), origin, checkwallnumber, checkstring, reset);
	}
	return true;
}

void Shaft::EnableRange(int floor, int range, bool value, bool EnableShaftDoors)
{
	//turn on/off a range of floors
	//if range is 3, show shaft on current floor (floor), and 1 floor below and above (3 total floors)
	//if range is 1, show only the current floor (floor)

	//exit if ShowFullShaft is true
	if (ShowFullShaft == true)
		return;

	//range must be greater than 0
	if (range < 1)
		range = 1;

	//range must be an odd number; if it's even, then add 1
	if (IsEven(range) == true)
		range++;

	int additionalfloors;
	if (range > 1)
		additionalfloors = (range - 1) / 2;
	else
		additionalfloors = 0;

	//disable floors 1 floor outside of range
	if (value == true)
	{
		if (floor - additionalfloors - 1 >= startfloor && floor - additionalfloors - 1 <= endfloor)
		{
			if (sbs->GetFloor(floor)->IsInGroup(floor - additionalfloors - 1) == false) //only disable if not in group
				Enabled(floor - additionalfloors - 1, false, EnableShaftDoors);
		}
		if (floor + additionalfloors + 1 >= startfloor && floor + additionalfloors + 1 <= endfloor)
		{
			if (sbs->GetFloor(floor)->IsInGroup(floor + additionalfloors + 1) == false) //only disable if not in group
				Enabled(floor + additionalfloors + 1, false, EnableShaftDoors);
		}
	}

	//enable floors within range
	for (int i = floor - additionalfloors; i <= floor + additionalfloors; i++)
	{
		if (i >= startfloor && i <= endfloor)
			Enabled(i, value, EnableShaftDoors);
	}
}

bool Shaft::IsEnabledFloor(int floor)
{
	if (floor >= startfloor && floor <= endfloor)
		return EnableArray[floor - startfloor];
	else
		return false;
}

void Shaft::AddShowFloor(int floor)
{
	//adds a floor number to the ShowFloors array

	if (ShowFloorsList.Find(floor) == csArrayItemNotFound)
		ShowFloorsList.InsertSorted(floor);
}

void Shaft::RemoveShowFloor(int floor)
{
	//removes a floor number from the ShowFloors array

	if (ShowFloorsList.Find(floor) != csArrayItemNotFound)
		ShowFloorsList.Delete(floor);
}

void Shaft::AddShowOutside(int floor)
{
	//adds a floor number to the ShowFloors array

	if (ShowOutsideList.Find(floor) == csArrayItemNotFound)
		ShowOutsideList.InsertSorted(floor);
}

void Shaft::RemoveShowOutside(int floor)
{
	//removes a floor number from the ShowFloors array

	if (ShowOutsideList.Find(floor) != csArrayItemNotFound)
		ShowOutsideList.Delete(floor);
}

bool Shaft::IsValidFloor(int floor)
{
	//return true if the shaft services the specified floor

	if (floor < startfloor || floor > endfloor)
		return false;

	if (!ShaftArray[floor - startfloor])
		return false;

	return true;
}

void Shaft::AddElevator(int number)
{
	//add specified elevator to list
	elevators.InsertSorted(number);
}

void Shaft::RemoveElevator(int number)
{
	//remove specified elevator from list
	elevators.Delete(number);
}

csRef<iMeshWrapper> Shaft::GetMeshWrapper(int floor)
{
	//returns the mesh wrapper for the specified floor

	if (!IsValidFloor(floor))
		return 0;

	return ShaftArray[floor - startfloor];
}

void Shaft::Report(const char *message)
{
	//general reporting function
	sbs->Report("Shaft " + csString(_itoa(ShaftNumber, intbuffer, 10)) + ": " + message);
}

bool Shaft::ReportError(const char *message)
{
	//general reporting function
	return sbs->ReportError("Shaft " + csString(_itoa(ShaftNumber, intbuffer, 10)) + ": " + message);
}

Light* Shaft::AddLight(int floor, const char *name, int type, csVector3 position, csVector3 direction, float radius, float max_distance, float color_r, float color_g, float color_b, float spec_color_r, float spec_color_g, float spec_color_b, float directional_cutoff_radius, float spot_falloff_inner, float spot_falloff_outer, bool dynamic_color, bool movable)
{
	//add a global light

	//exit if floor is invalid
	if (!IsValidFloor(floor))
		return 0;

	Light* light = new Light(name, type, position + csVector3(origin.x, sbs->GetFloor(floor)->Altitude, origin.z), direction, radius, max_distance, color_r, color_g, color_b, spec_color_r, spec_color_g, spec_color_b, directional_cutoff_radius, spot_falloff_inner, spot_falloff_outer, dynamic_color, movable);
	lights[floor - startfloor].Push(light);
	return light;
}
