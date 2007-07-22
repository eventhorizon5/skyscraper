/* $Id$ */

/*
	Scalable Building Simulator - Shaft Subsystem Class
	The Skyscraper Project - Version 1.1 Alpha
	Copyright (C)2005-2007 Ryan Thoryk
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
	top = 0;
	bottom = 0;
	cutstart = 0;
	cutend = 0;

	csString buffer, buffer2, buffer3;

	ShaftArray.SetSize(endfloor - startfloor + 1);
	ShaftArray_state.SetSize(endfloor - startfloor + 1);

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
		ShaftArray[i - startfloor]->GetMovable()->SetPosition(origin);
		ShaftArray[i - startfloor]->GetMovable()->UpdateMove();
	}
}

Shaft::~Shaft()
{
	//destructor

}

int Shaft::AddWall(int floor, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th)
{
	return sbs->AddWallMain(ShaftArray_state[floor - startfloor], name, texture, thickness, x1, z1, x2, z2, height1, height2, sbs->GetFloor(floor)->Altitude + voffset1, sbs->GetFloor(floor)->Altitude + voffset2, tw, th);
}

int Shaft::AddFloor(int floor, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, float tw, float th)
{
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

	return sbs->AddFloorMain(ShaftArray_state[floor - startfloor], name, texture, thickness, x1, z1, x2, z2, sbs->GetFloor(floor)->Altitude + voffset1, sbs->GetFloor(floor)->Altitude + voffset2, tw, th);
}

void Shaft::Enabled(int floor, bool value)
{
	//turns shaft on/off for a specific floor
	if (value == true)
	{
		ShaftArray[floor - startfloor]->GetFlags().Reset (CS_ENTITY_INVISIBLEMESH);
		ShaftArray[floor - startfloor]->GetFlags().Reset (CS_ENTITY_NOSHADOWS);
		ShaftArray[floor - startfloor]->GetFlags().Reset (CS_ENTITY_NOHITBEAM);
	}
	else
	{
		//leave bottom and top on
		if (floor != startfloor && floor != endfloor)
		{
			ShaftArray[floor - startfloor]->GetFlags().Set (CS_ENTITY_INVISIBLEMESH);
			ShaftArray[floor - startfloor]->GetFlags().Set (CS_ENTITY_NOSHADOWS);
			ShaftArray[floor - startfloor]->GetFlags().Set (CS_ENTITY_NOHITBEAM);
		}
	}

	for (size_t i = elevators[0]; i < elevators.GetSize(); i++)
	{
		for(size_t j = sbs->GetElevator(i)->ServicedFloors[0]; j < sbs->GetElevator(i)->ServicedFloors.GetSize(); j++)
			sbs->GetElevator(i)->ShaftDoorsEnabled(j, value);
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

void Shaft::EnableWholeShaft(bool value)
{
	//turn on/off entire shaft
	for (int i = startfloor; i <= endfloor; i++)
		Enabled(i, value);
}

bool Shaft::IsInShaft(const csVector3 &position)
{
	if (position.y > bottom && position.y < top)
	{
		csHitBeamResult result = ShaftArray[startfloor]->HitBeam(position, csVector3(position.x, position.y - (top - bottom), position.z));
		return result.hit;
	}
	return false;
}

void Shaft::CutFloors(csVector2 start, csVector2 end, float startvoffset, float endvoffset)
{
	//Cut through floor/ceiling polygons on all associated levels, within the voffsets
	
	sbs->Report("Cutting for shaft " + csString(_itoa(ShaftNumber, intbuffer, 10)) + "...");
	
	float voffset1, voffset2;
	cutstart = start;
	cutend = end;

	for (int i = startfloor; i <= endfloor; i++)
	{
		voffset1 = 0;
		voffset2 = sbs->GetFloor(i)->FullHeight();

		if (i == startfloor)
			voffset1 = startvoffset;
		else if (i == endfloor)
			voffset2 = endvoffset;

		sbs->GetFloor(i)->Cut(csVector3(origin.x + start.x, voffset1, origin.z + start.y), csVector3(origin.x + end.x, voffset2, origin.z + end.y), false, true, false);
	}
}

void Shaft::CutWall(int floor, csVector3 start, csVector3 end)
{
	//Cut through a wall segment
	//the Y values in start and end are both relative to the floor's altitude + interfloor

	float base = sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight;
	sbs->Cut(ShaftArray_state[floor - startfloor], csVector3(start.x, base + start.y, start.z), csVector3(end.x, base + end.y, end.z), true, false);
}
