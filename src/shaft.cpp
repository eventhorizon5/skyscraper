/* $Id$ */

/*
	Scalable Building Simulator - Shaft Subsystem Class
	The Skyscraper Project - Version 1.1 Alpha
	Copyright (C)2005-2006 Ryan Thoryk
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

//#include <wx/wx.h>
#include <crystalspace.h>
#include "shaft.h"
#include "sbs.h"
#include "camera.h"

extern SBS *sbs; //external pointer to the SBS engine

Shaft::Shaft(int number, int type, double CenterX, double CenterZ, int _startfloor, int _endfloor)
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
	origin = csVector3(CenterX, sbs->FloorArray[_startfloor]->Altitude, CenterZ);

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

int Shaft::AddWall(int floor, const char *texture, double x1, double z1, double x2, double z2, double height1, double height2, double voffset1, double voffset2, double tw, double th, bool revX, bool revY, bool revZ)
{
	return sbs->AddWallMain(ShaftArray_state[floor - startfloor], texture, x1, z1, x2, z2, height1, height2, sbs->FloorArray[floor]->Altitude + voffset1, sbs->FloorArray[floor]->Altitude + voffset2, tw, th, revX, revY, revZ);
}

int Shaft::AddFloor(int floor, const char *texture, double x1, double z1, double x2, double z2, double voffset1, double voffset2, double tw, double th)
{
   	return sbs->AddFloorMain(ShaftArray_state[floor - startfloor], texture, x1, z1, x2, z2, sbs->FloorArray[floor]->Altitude + voffset1, sbs->FloorArray[floor]->Altitude + voffset2, tw, th);
}

void Shaft::Enabled(int floor, bool value)
{
	//turns shaft elevator doors on/off
	if (value == true)
	{
		ShaftArray[floor - startfloor]->GetFlags().Reset (CS_ENTITY_INVISIBLEMESH);
		ShaftArray[floor - startfloor]->GetFlags().Reset (CS_ENTITY_NOSHADOWS);
		ShaftArray[floor - startfloor]->GetFlags().Reset (CS_ENTITY_NOHITBEAM);
	}
	else
	{
		ShaftArray[floor - startfloor]->GetFlags().Set (CS_ENTITY_INVISIBLEMESH);
		ShaftArray[floor - startfloor]->GetFlags().Set (CS_ENTITY_NOSHADOWS);
		ShaftArray[floor - startfloor]->GetFlags().Set (CS_ENTITY_NOHITBEAM);
	}
}
