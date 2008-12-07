/* $Id$ */

/*
	Scalable Building Simulator - Shaft Subsystem Class
	The Skyscraper Project - Version 1.1 Alpha
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

#include "globals.h"
#include "shaft.h"
#include "sbs.h"
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

	ShaftType = type;
	ShaftNumber = number;
	startfloor = _startfloor;
	endfloor = _endfloor;
	origin = csVector3(CenterX, sbs->GetFloor(_startfloor)->Altitude, CenterZ);
	InsideShaft = false;
	IsEnabled = true;
	top = sbs->GetFloor(endfloor)->Altitude + sbs->GetFloor(endfloor)->FullHeight();
	bottom = sbs->GetFloor(startfloor)->Altitude;
	cutstart = 0;
	cutend = 0;

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
		ShaftArray[i - startfloor]->SetRenderPriority(sbs->engine->GetAlphaRenderPriority());
		ShaftArray[i - startfloor]->GetMeshObject()->SetMixMode(CS_FX_ALPHA);
		EnableArray[i - startfloor] = true;
	}
}

Shaft::~Shaft()
{
	//destructor

}

int Shaft::AddWall(int floor, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th)
{
	float tw2 = tw;
	float th2;
	float tempw1;
	float tempw2;

	//Set horizontal scaling
	x1 = x1 * sbs->HorizScale;
	x2 = x2 * sbs->HorizScale;
	z1 = z1 * sbs->HorizScale;
	z2 = z2 * sbs->HorizScale;

	//Call texture autosizing formulas
	if (z1 == z2)
		tw2 = sbs->AutoSize(texture, x1, x2, true, tw);
	if (x1 == x2)
		tw2 = sbs->AutoSize(texture, z1, z2, true, tw);
	if ((z1 != z2) && (x1 != x2))
	{
		//calculate diagonals
		if (x1 > x2)
			tempw1 = x1 - x2;
		else
			tempw1 = x2 - x1;
		if (z1 > z2)
			tempw2 = z1 - z2;
		else
			tempw2 = z2 - z1;
		tw2 = sbs->AutoSize(texture, 0, sqrt(pow(tempw1, 2) + pow(tempw2, 2)), true, tw);
	}
	th2 = sbs->AutoSize(texture, 0, height1, false, th);

	return sbs->AddWallMain(ShaftArray_state[floor - startfloor], name, texture, thickness, origin.x + x1, origin.z + z1, origin.x + x2, origin.z + z2, height1, height2, sbs->GetFloor(floor)->Altitude + voffset1, sbs->GetFloor(floor)->Altitude + voffset2, tw2, th2);
}

int Shaft::AddFloor(int floor, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, float tw, float th)
{
	float tw2;
	float th2;
	
	//Set horizontal scaling
	x1 = x1 * sbs->HorizScale;
	x2 = x2 * sbs->HorizScale;
	z1 = z1 * sbs->HorizScale;
	z2 = z2 * sbs->HorizScale;

	//Call texture autosizing formulas
	tw2 = sbs->AutoSize(texture, x1, x2, true, tw);
	th2 = sbs->AutoSize(texture, z1, z2, false, th);

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

	return sbs->AddFloorMain(ShaftArray_state[floor - startfloor], name, texture, thickness, origin.x + x1, origin.z + z1, origin.x + x2, origin.z + z2, altitude + voffset1, altitude + voffset2, tw2, th2);
}

void Shaft::Enabled(int floor, bool value, bool EnableShaftDoors)
{
	if (IsEnabledFloor(floor) != value && floor >= startfloor && floor <= endfloor)
	{
		//turns shaft on/off for a specific floor
		if (value == true)
		{
			ShaftArray[floor - startfloor]->GetFlags().Reset (CS_ENTITY_INVISIBLEMESH);
			ShaftArray[floor - startfloor]->GetFlags().Reset (CS_ENTITY_NOSHADOWS);
			ShaftArray[floor - startfloor]->GetFlags().Reset (CS_ENTITY_NOHITBEAM);
			EnableArray[floor - startfloor] = true;
		}
		else
		{
			//leave bottom and top on
			if (floor != startfloor && floor != endfloor)
			{
				ShaftArray[floor - startfloor]->GetFlags().Set (CS_ENTITY_INVISIBLEMESH);
				ShaftArray[floor - startfloor]->GetFlags().Set (CS_ENTITY_NOSHADOWS);
				ShaftArray[floor - startfloor]->GetFlags().Set (CS_ENTITY_NOHITBEAM);
				EnableArray[floor - startfloor] = false;
			}
			else
				return;
		}
		if (EnableShaftDoors == true)
		{
			for (size_t i = 0; i < elevators.GetSize(); i++)
			{
				for(size_t j = 0; j < sbs->GetElevator(elevators[i])->ServicedFloors.GetSize(); j++)
				{
					if (sbs->GetElevator(elevators[i])->ServicedFloors[j] == floor)
						sbs->GetElevator(elevators[i])->ShaftDoorsEnabled(sbs->GetElevator(elevators[i])->ServicedFloors[j], value);
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

void Shaft::EnableWholeShaft(bool value, bool EnableShaftDoors)
{
	//turn on/off entire shaft
	if ((value == false && IsEnabled == true) || (value == true && IsEnabled == false))
	{
		for (int i = startfloor; i <= endfloor; i++)
			Enabled(i, value, EnableShaftDoors);
	}
	IsEnabled = value;
}

bool Shaft::IsInShaft(const csVector3 &position)
{
	if (position.y > bottom && position.y < top)
	{
		csHitBeamResult result = ShaftArray[0]->HitBeam(position, csVector3(position.x, position.y - (top - bottom), position.z));
		return result.hit;
	}
	return false;
}

void Shaft::CutFloors(bool relative, csVector2 start, csVector2 end, float startvoffset, float endvoffset)
{
	//Cut through floor/ceiling polygons on all associated levels, within the voffsets
	
	sbs->Report("Cutting for shaft " + csString(_itoa(ShaftNumber, intbuffer, 10)) + "...");
	
	float voffset1, voffset2;
	cutstart = start;
	cutend = end;

	for (int i = startfloor; i <= endfloor; i++)
	{
		voffset1 = 0;
		voffset2 = sbs->GetFloor(i)->FullHeight() + 1;

		if (i == startfloor)
			voffset1 = startvoffset;
		else if (i == endfloor)
			voffset2 = endvoffset;

		if (relative == true)
			sbs->GetFloor(i)->Cut(csVector3(origin.x + start.x, voffset1, origin.z + start.y), csVector3(origin.x + end.x, voffset2, origin.z + end.y), false, true, false);
		else
			sbs->GetFloor(i)->Cut(csVector3(start.x, voffset1, start.y), csVector3(end.x, voffset2, end.y), false, true, false);
	}
}

void Shaft::CutWall(bool relative, int floor, csVector3 start, csVector3 end, int checkwallnumber, const char *checkstring)
{
	//Cut through a wall segment
	//the Y values in start and end are both relative to the floor's altitude

	float base = sbs->GetFloor(floor)->Altitude;

	if (relative == true)
		sbs->Cut(ShaftArray_state[floor - startfloor], csVector3(origin.x + start.x, base + start.y, origin.z + start.z), csVector3(origin.x + end.x, base + end.y, origin.z + end.z), true, false, csVector3(0, 0, 0), origin, checkwallnumber, checkstring);
	else
		sbs->Cut(ShaftArray_state[floor - startfloor], csVector3(start.x, base + start.y, start.z), csVector3(end.x, base + end.y, end.z), true, false, csVector3(0, 0, 0), origin, checkwallnumber, checkstring);
}

void Shaft::EnableRange(int floor, int range, bool value, bool EnableShaftDoors)
{
	//turn on/off a range of floors
	//if range is 3, show shaft on current floor (floor), and 1 floor below and above (3 total floors)
	//if range is 1, show only the current floor (floor)

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
