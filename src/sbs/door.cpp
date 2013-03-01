/* $Id$ */

/*
	Scalable Building Simulator - Door Class
	The Skyscraper Project - Version 1.9 Alpha
	Copyright (C)2004-2013 Ryan Thoryk
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
	Locked = 0;
	KeyID = 0;

	//set speed to default value if invalid
	if (Speed <= 0)
		Speed = sbs->GetConfigFloat("Skyscraper.SBS.DoorSpeed", 75.0);

	if (Speed <= 0)
		Speed = 75;

	//set origin to location of the door's hinge/pivot point and set up door coordinates
	if (Direction == 1 || Direction == 2)
	{
		origin = Ogre::Vector3(CenterX, altitude, CenterZ - (width / 2)); //front
		x1 = 0;
		x2 = 0;
		z1 = 0;
		z2 = width;
	}
	if (Direction == 3 || Direction == 4)
	{
		origin = Ogre::Vector3(CenterX, altitude, CenterZ + (width / 2)); //back
		x1 = 0;
		x2 = 0;
		z1 = -width;
		z2 = 0;
	}
	if (Direction == 5 || Direction == 6)
	{
		origin = Ogre::Vector3(CenterX + (width / 2), altitude, CenterZ); //right
		x1 = -width;
		x2 = 0;
		z1 = 0;
		z2 = 0;
	}
	if (Direction == 7 || Direction == 8)
	{
		origin = Ogre::Vector3(CenterX - (width / 2), altitude, CenterZ); //left
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
	DoorMesh = new MeshObject(object, Name.c_str());
	DoorMesh->Move(origin, false, false, false);

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

	WallObject *wall;
	wall = DoorMesh->CreateWallObject(this->object, name);
	sbs->AddWallMain(wall, name, texture, thickness, x1, z1, x2, z2, height, height, 0, 0, tw, th, false);
	sbs->ResetWalls();
	sbs->ResetTextureMapping();

	//open door on startup (without sound) if specified
	if (open_state == true)
		Open(origin, false, true);
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

	if (DoorMesh)
		delete DoorMesh;
	DoorMesh = 0;

	//unregister from parent
	if (sbs->FastDelete == false)
	{
		if (object->parent_deleting == false)
		{
			if (std::string(object->GetParent()->GetType()) == "Elevator")
				((Elevator*)object->GetParent()->GetRawObject())->RemoveDoor(this);
			if (std::string(object->GetParent()->GetType()) == "Floor")
				((Floor*)object->GetParent()->GetRawObject())->RemoveDoor(this);
			if (std::string(object->GetParent()->GetType()) == "Stairs")
				((Stairs*)object->GetParent()->GetRawObject())->RemoveDoor(this);
		}
		sbs->UnregisterDoorCallback(this);
	}

	delete object;
}

void Door::Open(Ogre::Vector3 &position, bool playsound, bool force)
{
	//position is the camera position, used to check if the door is locked
	//if force is true, locking/position check will be bypassed

	sbs->Report(std::string("Opening door " + Name).c_str());

	if (!sbs->RegisterDoorCallback(this))
		return;

	if (force == false)
	{
		//check lock state
		if (IsLocked(position) == true)
		{
			sbs->Report(std::string("Door " + Name + " is locked").c_str());
			return;
		}
	}

	if (IsMoving == false)
		OpenDoor = true;

	if (playsound == true)
	{
		sound->Load(OpenSound.c_str());
		sound->Play();
	}
	IsMoving = true;
}

void Door::Close(bool playsound)
{
	sbs->Report(std::string("Closing door " + Name).c_str());

	if (!sbs->RegisterDoorCallback(this))
		return;

	if (IsMoving == false)
		OpenDoor = false;

	if (playsound == true)
	{
		sound->Load(CloseSound.c_str());
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

	DoorMesh->Enable(value);
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

	DoorMesh->SetRotation(Ogre::Vector3(0, rotation, 0));
}

void Door::Move(const Ogre::Vector3 position, bool relative_x, bool relative_y, bool relative_z)
{
	//moves door

	DoorMesh->Move(position, relative_x, relative_y, relative_z);
	origin = GetPosition();

}

Ogre::Vector3 Door::GetPosition()
{
	//return the door's position

	return DoorMesh->GetPosition();
}

void Door::SetLocked(int side, int keyid)
{
	//lock table:
	//0 = unlocked
	//1 = negative (left/front) side locked
	//2 = positive (right/back) side locked
	//3 = both sides locked

	if (side < 0 || side > 3)
		return;

	Locked = side;
	KeyID = keyid;
}

void Door::ToggleLock(const Ogre::Vector3 &position, bool force)
{
	//toggle lock state of the related door side
	//if force is true, bypass key check

	bool replocked = false;

	//quit if user doesn't have key, if force is false
	if (KeyID != 0)
	{
		if (sbs->CheckKey(KeyID) == false && force == false)
		{
			char intbuffer[10];
			std::string id = _itoa(KeyID, intbuffer, 10);
			sbs->Report(std::string("Need key " + id + " to lock/unlock door " + Name).c_str());
			return;
		}
	}

	if (GetSide(position) == false)
	{
		if (Locked == 0)
		{
			Locked = 1;
			replocked = true;
		}
		else if (Locked == 1)
		{
			Locked = 0;
			replocked = false;
		}
		else if (Locked == 2)
		{
			Locked = 3;
			replocked = true;
		}
	}
	else
	{
		if (Locked == 0)
		{
			Locked = 2;
			replocked = true;
		}
		else if (Locked == 1)
		{
			Locked = 3;
			replocked = true;
		}
		else if (Locked == 2)
		{
			Locked = 1;
			replocked = false;
		}
	}

	if (replocked == true)
		sbs->Report(std::string("Locked door " + Name).c_str());
	else
		sbs->Report(std::string("Unlocked door " + Name).c_str());
}

bool Door::GetSide(const Ogre::Vector3 &position)
{
	//return which side of the door the position is (false for negative/left/front, true for positive/right/back)

	if ((Direction >= 1 && Direction <= 4) && position.x > origin.x)
		return true;

	if ((Direction >= 5 && Direction <= 8) && position.z > origin.z)
		return true;

	return false;
}

bool Door::IsLocked(const Ogre::Vector3 &position)
{
	//returns if the door's side (in relation to the given position) is locked or not

	if (Locked == 0)
		return false;

	if (Locked == 3)
		return true;

	bool side = GetSide(position);

	if ((Locked == 1 && side == false) || (Locked == 2 && side == true))
		return true;
	return false;
}

int Door::GetKeyID()
{
	return KeyID;
}
