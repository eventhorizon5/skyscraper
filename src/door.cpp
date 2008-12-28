/* $Id$ */

/*
	Scalable Building Simulator - Door Class
	The Skyscraper Project - Version 1.2 Alpha
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
#include "door.h"
#include "sbs.h"
#include "camera.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

Door::Door(const char *name, const char *texture, float thickness, int direction, float CenterX, float CenterZ, float width, float height, float altitude, float tw, float th)
{
	//creates a door, and performs a cut operation on the area that it takes up.
	//this must be used *after* the wall is created

	//direction table:
	//1 = faces left, opens left
	//2 = faces left, opens right
	//3 = faces right, opens right
	//4 = faces right, opens left
	//5 = faces front, opens front
	//6 = faces front, opens back
	//7 = faces back, opens back
	//8 = faces back, opens front

	Name = name;
	Direction = direction;
	IsOpen = false;
	float x1, z1, x2, z2;
	origin = csVector3(CenterX, altitude, CenterZ);

	//set up coordinates
	if (direction < 5)
	{
		x1 = CenterX;
		x2 = CenterX;
		z1 = CenterZ - (width / 2);
		z2 = CenterZ + (width / 2);
	}
	else
	{
		x1 = CenterX - (width / 2);
		x2 = CenterX + (width / 2);
		z1 = CenterZ;
		z2 = CenterZ;
	}

	//Create mesh
	DoorMesh = sbs->engine->CreateSectorWallsMesh (sbs->area, Name.GetData());
	DoorMesh_state = scfQueryInterface<iThingFactoryState> (DoorMesh->GetMeshObject()->GetFactory());
	DoorMesh->SetZBufMode(CS_ZBUF_USE);
	DoorMesh->SetRenderPriority(sbs->engine->GetAlphaRenderPriority());
	DoorMesh->GetMeshObject()->SetMixMode(CS_FX_ALPHA);

	//create door
	sbs->DrawWalls(true, true, true, true, true, true);
	sbs->ReverseExtents(false, false, false);
	//sbs->AddWallMain(DoorMesh_state, name.GetData(), texture, thickness, x1, z1, x2, z2, height, height, altitude, altitude, tw, th);
	sbs->ResetWalls();
	sbs->ResetExtents();

	//disable on creation
	Enabled(false);
}

Door::~Door()
{
	//destructor

}

void Door::OpenDoor()
{
	sbs->Report("Opening door " + Name);
}

void Door::CloseDoor()
{
	sbs->Report("Closing door " + Name);
}

void Door::Enabled(bool value)
{
	if (value == true)
	{
		DoorMesh->GetFlags().Reset (CS_ENTITY_INVISIBLEMESH);
		DoorMesh->GetFlags().Reset (CS_ENTITY_NOSHADOWS);
		DoorMesh->GetFlags().Reset (CS_ENTITY_NOHITBEAM);
	}
	else
	{
		DoorMesh->GetFlags().Set (CS_ENTITY_INVISIBLEMESH);
		DoorMesh->GetFlags().Set (CS_ENTITY_NOSHADOWS);
		DoorMesh->GetFlags().Set (CS_ENTITY_NOHITBEAM);
	}
}
