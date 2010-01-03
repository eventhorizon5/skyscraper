/* $Id$ */

/*
	Scalable Building Simulator - Shaft Subsystem Class
	The Skyscraper Project - Version 1.6 Alpha
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
	object->SetValues(this, sbs->object, "Shaft", false);

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

	ShaftArray.SetSize(endfloor - startfloor + 1);
	ShaftArray_state.SetSize(endfloor - startfloor + 1);
	EnableArray.SetSize(endfloor - startfloor + 1);

	for (int i = startfloor; i <= endfloor; i++)
	{
		//Create shaft meshes
		buffer2 = number;
		buffer3 = i;
		buffer = "Shaft " + buffer2 + ":" + buffer3;
		buffer.Trim();
		csRef<iMeshWrapper> tmpmesh;
		csRef<iThingFactoryState> tmpstate;
		tmpmesh = sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData());
		ShaftArray[i - startfloor] = tmpmesh;
		tmpstate = scfQueryInterface<iThingFactoryState> (ShaftArray[i - startfloor]->GetMeshObject()->GetFactory());
		ShaftArray_state[i - startfloor] = tmpstate;
		ShaftArray[i - startfloor]->SetZBufMode(CS_ZBUF_USE);
		ShaftArray[i - startfloor]->SetRenderPriority(sbs->engine->GetObjectRenderPriority());
		EnableArray[i - startfloor] = true;
	}
}

Shaft::~Shaft()
{
	//destructor

	//delete wall objects
	for (int i = 0; i < shaft_walls.GetSize(); i++)
	{
		if (shaft_walls[i])
			delete shaft_walls[i];
		shaft_walls[i] = 0;
	}

	//delete mesh array objects
	for (int i = 0; i < ShaftArray_state.GetSize(); i++)
		ShaftArray_state[i] = 0;
	for (int i = 0; i < ShaftArray.GetSize(); i++)
		ShaftArray[i] = 0;
	ShaftArray_state.DeleteAll();
	ShaftArray.DeleteAll();
	delete object;
}

WallObject* Shaft::AddWall(int floor, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th)
{
	//exit with an error if floor is invalid
	if (IsValidFloor(floor) == false)
	{
		sbs->ReportError("Shaft " + csString(_itoa(ShaftNumber, intbuffer, 10)) + " - AddWall: Floor " + csString(_itoa(floor, intbuffer, 10)) + " out of range");
		return 0;
	}

	//Set horizontal scaling
	x1 = x1 * sbs->HorizScale;
	x2 = x2 * sbs->HorizScale;
	z1 = z1 * sbs->HorizScale;
	z2 = z2 * sbs->HorizScale;

	//calculate autosizing
	float tmpheight;
	if (height1 > height1)
		tmpheight = height1;
	else
		tmpheight = height2;
	csVector2 sizing = sbs->CalculateSizing(texture, csVector2(x1, x2), csVector2(0, tmpheight), csVector2(z1, z2), tw, th);

	WallObject *wall = sbs->CreateWallObject(shaft_walls, ShaftArray[floor - startfloor], this->object, name);
	sbs->AddWallMain(wall, name, texture, thickness, origin.x + x1, origin.z + z1, origin.x + x2, origin.z + z2, height1, height2, sbs->GetFloor(floor)->Altitude + voffset1, sbs->GetFloor(floor)->Altitude + voffset2, sizing.x, sizing.y);
	return wall;
}

WallObject* Shaft::AddFloor(int floor, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, float tw, float th)
{
	float tw2;
	float th2;

	//exit with an error if floor is invalid
	if (IsValidFloor(floor) == false)
	{
		sbs->ReportError("Shaft " + csString(_itoa(ShaftNumber, intbuffer, 10)) + " - AddFloor: Floor " + csString(_itoa(floor, intbuffer, 10)) + " out of range");
		return 0;
	}

	//Set horizontal scaling
	x1 = x1 * sbs->HorizScale;
	x2 = x2 * sbs->HorizScale;
	z1 = z1 * sbs->HorizScale;
	z2 = z2 * sbs->HorizScale;

	//get texture force value
	bool force_enable, force_mode;
	sbs->GetTextureForce(texture, force_enable, force_mode);

	//Call texture autosizing formulas
	tw2 = sbs->AutoSize(x1, x2, true, tw, force_enable, force_mode);
	th2 = sbs->AutoSize(z1, z2, false, th, force_enable, force_mode);

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

	WallObject *wall = sbs->CreateWallObject(shaft_walls, ShaftArray[floor - startfloor], this->object, name);
	sbs->AddFloorMain(wall, name, texture, thickness, origin.x + x1, origin.z + z1, origin.x + x2, origin.z + z2, altitude + voffset1, altitude + voffset2, tw2, th2);
	return wall;
}

void Shaft::Enabled(int floor, bool value, bool EnableShaftDoors)
{
	if (IsEnabledFloor(floor) != value && floor >= startfloor && floor <= endfloor && EnableCheck == false)
	{
		//turns shaft on/off for a specific floor
		if (value == true)
		{
			sbs->EnableMesh(ShaftArray[floor - startfloor], value);
			EnableArray[floor - startfloor] = true;
		}
		else
		{
			//leave bottom and top on
			if (floor != startfloor && floor != endfloor)
			{
				sbs->EnableMesh(ShaftArray[floor - startfloor], value);
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
}

bool Shaft::CutWall(bool relative, int floor, const csVector3 &start, const csVector3 &end, int checkwallnumber, const char *checkstring)
{
	//Cut through a wall segment
	//the Y values in start and end are both relative to the floor's altitude

	//exit with an error if floor is invalid
	if (IsValidFloor(floor) == false)
	{
		sbs->ReportError("Shaft " + csString(_itoa(ShaftNumber, intbuffer, 10)) + " - CutWall: Floor " + csString(_itoa(floor, intbuffer, 10)) + " out of range");
		return false;
	}

	float base = sbs->GetFloor(floor)->Altitude;

	if (relative == true)
		sbs->Cut(ShaftArray[floor - startfloor], csVector3(origin.x + start.x, base + start.y, origin.z + start.z), csVector3(origin.x + end.x, base + end.y, origin.z + end.z), true, false, csVector3(0, 0, 0), origin, checkwallnumber, checkstring);
	else
		sbs->Cut(ShaftArray[floor - startfloor], csVector3(start.x, base + start.y, start.z), csVector3(end.x, base + end.y, end.z), true, false, csVector3(0, 0, 0), origin, checkwallnumber, checkstring);
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
			Enabled(floor - additionalfloors - 1, false, EnableShaftDoors);
		if (floor + additionalfloors + 1 >= startfloor && floor + additionalfloors + 1 <= endfloor)
			Enabled(floor + additionalfloors + 1, false, EnableShaftDoors);
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
