/* $Id$ */

/*
	Scalable Building Simulator - Door Class
	The Skyscraper Project - Version 1.5 Alpha
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
#include "door.h"
#include "sbs.h"
#include "camera.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

Door::Door(const char *name, const char *texture, float thickness, int direction, float CenterX, float CenterZ, float width, float height, float altitude, float tw, float th)
{
	//creates a door
	//wall cuts must be performed by the calling (parent) function

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
	OpenState = false;
	IsMoving = false;
	float x1, z1, x2, z2;
	rotation = 0;
	OpenDoor = false;

	//set origin to location of the door's hinge/pivot point and set up door coordinates
	if (Direction == 1 || Direction == 2)
	{
		origin = csVector3(CenterX, altitude, CenterZ - (width / 2)); //front
		x1 = 0;
		x2 = 0;
		z1 = 0;
		z2 = width;
	}
	if (Direction == 3 || Direction == 4)
	{
		origin = csVector3(CenterX, altitude, CenterZ + (width / 2)); //back
		x1 = 0;
		x2 = 0;
		z1 = -width;
		z2 = 0;
	}
	if (Direction == 5 || Direction == 6)
	{
		origin = csVector3(CenterX + (width / 2), altitude, CenterZ); //right
		x1 = -width;
		x2 = 0;
		z1 = 0;
		z2 = 0;
	}
	if (Direction == 7 || Direction == 8)
	{
		origin = csVector3(CenterX - (width / 2), altitude, CenterZ); //left
		x1 = 0;
		x2 = width;
		z1 = 0;
		z2 = 0;
	}

	if (Direction == 1 || Direction == 3 || Direction == 5 || Direction == 7)
		Clockwise = false;
	else
		Clockwise = true;

	//Create mesh
	DoorMesh = sbs->engine->CreateSectorWallsMesh (sbs->area, Name.GetData());
	DoorMesh_state = scfQueryInterface<iThingFactoryState> (DoorMesh->GetMeshObject()->GetFactory());
	DoorMesh_movable = DoorMesh->GetMovable();
	DoorMesh->SetZBufMode(CS_ZBUF_USE);
	DoorMesh->SetRenderPriority(sbs->engine->GetObjectRenderPriority());
	DoorMesh_movable->SetPosition(origin);
	DoorMesh_movable->UpdateMove();

	//create door
	sbs->DrawWalls(true, true, true, true, true, true);
	sbs->ResetTextureMapping(true);
	if (Direction == 1 || Direction == 2 || Direction == 5 || Direction == 6)
		sbs->SetTextureFlip(0, 1, 0, 0, 0, 0); //flip texture on rear side of door
	if (Direction == 3 || Direction == 4 || Direction == 7 || Direction == 8)
		sbs->SetTextureFlip(1, 0, 0, 0, 0, 0); //flip texture on rear side of door
	sbs->AddWallMain(DoorMesh_state, name, texture, thickness, x1, z1, x2, z2, height, height, 0, 0, tw, th);
	sbs->ResetWalls();
	sbs->ResetTextureMapping();

	//disable on creation
	Enabled(false);
}

Door::~Door()
{
	//destructor

}

void Door::Open()
{
	sbs->Report("Opening door " + Name);
	if (!sbs->RegisterDoorCallback(this))
		return;
	if (IsMoving == false)
		OpenDoor = true;
	IsMoving = true;
}

void Door::Close()
{
	sbs->Report("Closing door " + Name);
	if (!sbs->RegisterDoorCallback(this))
		return;
	if (IsMoving == false)
		OpenDoor = false;
	IsMoving = true;
}

bool Door::IsOpen()
{
	return OpenState;
}

void Door::Enabled(bool value)
{
	sbs->EnableMesh(DoorMesh, value);
}

void Door::MoveDoor()
{
	//door movement callback function
	if (Clockwise == true)
	{
		if (OpenDoor == true)
			rotation += 75 * sbs->delta;
		else
			rotation -= 75 * sbs->delta;
	}
	else
	{
		if (OpenDoor == true)
			rotation -= 75 * sbs->delta;
		else
			rotation += 75 * sbs->delta;
	}

	//if opened fully, set state to opened
	if (rotation >= 90 || rotation <= -90)
	{
		OpenState = true;
		IsMoving = false;
		if (rotation >= 90)
			rotation = 90;
		else
			rotation = -90;
	}

	//if closed fully, set state to closed
	if ((Clockwise == true && rotation <= 0) || (Clockwise == false && rotation >= 0))
	{
		OpenState = false;
		IsMoving = false;
		rotation = 0;
	}

	csMatrix3 rot = csYRotMatrix3(DegreesToRadians(rotation));
	csOrthoTransform ot (rot, origin);
	DoorMesh_movable->SetTransform(ot);
	DoorMesh_movable->UpdateMove();
}
