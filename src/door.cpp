/* $Id$ */

/*
	Scalable Building Simulator - Door Class
	The Skyscraper Project - Version 1.7 Alpha
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
#include "door.h"
#include "camera.h"
#include "unix.h"

#include <iengine/movable.h>

extern SBS *sbs; //external pointer to the SBS engine

Door::Door(Object *parent, const char *name, const char *open_sound, const char *close_sound, bool open_state, const char *texture, float thickness, int direction, float speed, float CenterX, float CenterZ, float width, float height, float altitude, float tw, float th)
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

	//set up SBS object
	object = new Object();
	object->SetValues(this, parent, "Door", name, false);

	IsEnabled = true;
	Name = name;
	Direction = direction;
	OpenState = false;
	IsMoving = false;
	float x1 = 0, z1 = 0, x2 = 0, z2 = 0;
	rotation = 0;
	OpenDoor = false;
	OpenSound = open_sound;
	CloseSound = close_sound;
	Speed = speed;

	//set speed to default value if invalid
	if (Speed <= 0)
		Speed = sbs->confman->GetFloat("Skyscraper.SBS.DoorSpeed", 75.0);

	if (Speed <= 0)
		Speed = 75;

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
	DoorMesh = sbs->CreateMesh(Name);
	DoorMesh_movable = DoorMesh->GetMovable();
	DoorMesh_movable->SetPosition(sbs->ToRemote(origin));
	DoorMesh_movable->UpdateMove();

	//create sound object
	sound = new Sound(object, "DoorSound", true);
	sound->SetPosition(origin);

	//create door
	sbs->DrawWalls(true, true, true, true, true, true);
	sbs->ResetTextureMapping(true);
	if (Direction == 1 || Direction == 2 || Direction == 5 || Direction == 6)
		sbs->SetTextureFlip(0, 1, 0, 0, 0, 0); //flip texture on rear side of door
	if (Direction == 3 || Direction == 4 || Direction == 7 || Direction == 8)
		sbs->SetTextureFlip(1, 0, 0, 0, 0, 0); //flip texture on rear side of door
	sbs->AddWallMain2(object, DoorMesh, name, texture, thickness, x1, z1, x2, z2, height, height, 0, 0, tw, th, false);
	sbs->ResetWalls();
	sbs->ResetTextureMapping();

	//open door on startup (without sound) if specified
	if (open_state == true)
		Open(false);
}

Door::~Door()
{
	//destructor

	if (sound)
	{
		sound->object->parent_deleting = true;
		delete sound;
	}
	sound = 0;
	DoorMesh_movable = 0;
	if (sbs->FastDelete == false)
	{
		sbs->engine->WantToDie(DoorMesh);

		//unregister from parent
		if (object->parent_deleting == false)
		{
			if (csString(object->GetParent()->GetType()) == "Elevator")
				((Elevator*)object->GetParent()->GetRawObject())->RemoveDoor(this);
			if (csString(object->GetParent()->GetType()) == "Floor")
				((Floor*)object->GetParent()->GetRawObject())->RemoveDoor(this);
			if (csString(object->GetParent()->GetType()) == "Stairs")
				((Stairs*)object->GetParent()->GetRawObject())->RemoveDoor(this);
		}
		sbs->UnregisterDoorCallback(this);
	}
	DoorMesh = 0;
	delete object;
}

void Door::Open(bool playsound)
{
	sbs->Report("Opening door " + Name);
	if (!sbs->RegisterDoorCallback(this))
		return;
	if (IsMoving == false)
		OpenDoor = true;
	if (playsound == true)
	{
		sound->Load(OpenSound);
		sound->Play();
	}
	IsMoving = true;
}

void Door::Close(bool playsound)
{
	sbs->Report("Closing door " + Name);
	if (!sbs->RegisterDoorCallback(this))
		return;
	if (IsMoving == false)
		OpenDoor = false;
	if (playsound == true)
	{
		sound->Load(CloseSound);
		sound->Play();
	}
	IsMoving = true;
}

bool Door::IsOpen()
{
	return OpenState;
}

void Door::Enabled(bool value)
{
	if (IsEnabled == value)
		return;

	sbs->EnableMesh(DoorMesh, value);
	IsEnabled = value;
}

void Door::MoveDoor()
{
	//door movement callback function
	if (Clockwise == true)
	{
		if (OpenDoor == true)
			rotation += Speed * sbs->delta;
		else
			rotation -= Speed * sbs->delta;
	}
	else
	{
		if (OpenDoor == true)
			rotation -= Speed * sbs->delta;
		else
			rotation += Speed * sbs->delta;
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
	csOrthoTransform ot (rot, sbs->ToRemote(origin));
	DoorMesh_movable->SetTransform(ot);
	DoorMesh_movable->UpdateMove();
}

void Door::Move(const csVector3 position, bool relative_x, bool relative_y, bool relative_z)
{
	//moves door

	csVector3 pos;
	if (relative_x == false)
		pos.x = sbs->ToRemote(position.x);
	else
		pos.x = DoorMesh_movable->GetPosition().x + sbs->ToRemote(position.x);
	if (relative_y == false)
		pos.y = sbs->ToRemote(position.y);
	else
		pos.y = DoorMesh_movable->GetPosition().y + sbs->ToRemote(position.y);
	if (relative_z == false)
		pos.z = sbs->ToRemote(position.z);
	else
		pos.z = DoorMesh_movable->GetPosition().z + sbs->ToRemote(position.z);
	DoorMesh_movable->SetPosition(pos);
	DoorMesh_movable->UpdateMove();
	origin = GetPosition();

}

csVector3 Door::GetPosition()
{
	//return the door's position

	return sbs->ToLocal(DoorMesh_movable->GetPosition());
}
