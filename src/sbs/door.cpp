/* $Id$ */

/*
	Scalable Building Simulator - Door Object
	The Skyscraper Project - Version 1.10 Alpha
	Copyright (C)2004-2016 Ryan Thoryk
	http://www.skyscrapersim.com
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@skyscrapersim.com

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
#include "manager.h"
#include "mesh.h"
#include "floor.h"
#include "elevatorcar.h"
#include "shaft.h"
#include "stairs.h"
#include "texture.h"
#include "sound.h"
#include "door.h"

namespace SBS {

Door::Door(Object *parent, DynamicMesh *wrapper, const std::string &name, const std::string &open_sound, const std::string &close_sound, bool open_state, const std::string &texture, float thickness, int direction, float speed, float CenterX, float CenterZ, float width, float height, float voffset, float tw, float th) : Object(parent)
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
	SetValues("Door", name, false);

	is_enabled = true;
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
	sound = 0;

	//set speed to default value if invalid
	if (Speed <= 0)
		Speed = sbs->GetConfigFloat("Skyscraper.SBS.DoorSpeed", 75.0);

	if (Speed <= 0)
		Speed = 75;

	Ogre::Vector3 position;

	//set origin to location of the door's hinge/pivot point and set up door coordinates
	if (Direction == 1 || Direction == 2)
	{
		position = Ogre::Vector3(CenterX, voffset, CenterZ - (width / 2)); //front
		x1 = 0;
		x2 = 0;
		z1 = 0;
		z2 = width;
	}
	if (Direction == 3 || Direction == 4)
	{
		position = Ogre::Vector3(CenterX, voffset, CenterZ + (width / 2)); //back
		x1 = 0;
		x2 = 0;
		z1 = -width;
		z2 = 0;
	}
	if (Direction == 5 || Direction == 6)
	{
		position = Ogre::Vector3(CenterX + (width / 2), voffset, CenterZ); //right
		x1 = -width;
		x2 = 0;
		z1 = 0;
		z2 = 0;
	}
	if (Direction == 7 || Direction == 8)
	{
		position = Ogre::Vector3(CenterX - (width / 2), voffset, CenterZ); //left
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
	DoorMesh = new MeshObject(this, name, wrapper);
	Move(position);

	//create sound object
	if (open_sound != "" || close_sound != "")
		sound = new Sound(this, "DoorSound", true);

	//create door
	sbs->DrawWalls(true, true, true, true, true, true);
	sbs->GetTextureManager()->ResetTextureMapping(true);
	if (Direction == 1 || Direction == 2 || Direction == 5 || Direction == 6)
		sbs->GetTextureManager()->SetTextureFlip(0, 1, 0, 0, 0, 0); //flip texture on rear side of door
	if (Direction == 3 || Direction == 4 || Direction == 7 || Direction == 8)
		sbs->GetTextureManager()->SetTextureFlip(1, 0, 0, 0, 0, 0); //flip texture on rear side of door

	WallObject *wall;
	wall = DoorMesh->CreateWallObject(name);
	sbs->AddWallMain(wall, name, texture, thickness, x1, z1, x2, z2, height, height, 0, 0, tw, th, false);
	sbs->ResetWalls();
	sbs->GetTextureManager()->ResetTextureMapping();

	//open door on startup (without sound) if specified
	if (open_state == true)
		Open(position, false, true);
}

Door::~Door()
{
	//destructor

	if (sound)
	{
		sound->parent_deleting = true;
		delete sound;
	}
	sound = 0;

	if (DoorMesh)
	{
		DoorMesh->parent_deleting = true;
		delete DoorMesh;
	}
	DoorMesh = 0;

	//unregister from parent
	if (sbs->FastDelete == false)
	{
		if (parent_deleting == false)
		{
			std::string type = GetParent()->GetType();

			if (type == "ElevatorCar")
				static_cast<ElevatorCar*>(GetParent())->RemoveDoor(this);
			else if (type == "Floor")
				static_cast<Floor*>(GetParent())->RemoveDoor(this);
			else if (type == "Shaft")
				static_cast<Shaft*>(GetParent())->RemoveDoor(this);
			else if (type == "Stairs")
				static_cast<Stairs*>(GetParent())->RemoveDoor(this);
			else if (type == "SBS")
				sbs->GetDoorManager()->RemoveDoor(this);
		}
	}
}

bool Door::Open(Ogre::Vector3 &position, bool playsound, bool force)
{
	//position is the camera position, used to check if the door is locked
	//if force is true, locking/position check will be bypassed

	sbs->Report("Opening door '" + GetName() + "'");

	EnableLoop(true);

	if (force == false)
	{
		//check lock state
		if (IsLocked(position) == true)
			return sbs->ReportError("Door '" + GetName() + "' is locked");
	}

	OpenDoor = true;

	if (playsound == true && sound)
	{
		sound->Load(OpenSound);
		sound->Play();
	}

	OpenState = false;
	IsMoving = true;

	return true;
}

void Door::Close(bool playsound)
{
	sbs->Report("Closing door '" + GetName() + "'");

	EnableLoop(true);

	OpenDoor = false;

	if (playsound == true && sound)
	{
		sound->Load(CloseSound);
		sound->Play();
	}

	OpenState = true;
	IsMoving = true;
}

bool Door::IsOpen()
{
	return OpenState;
}

void Door::Enabled(bool value)
{
	if (is_enabled == value)
		return;

	DoorMesh->Enable(value);
	is_enabled = value;
}

void Door::Loop()
{
	if (IsMoving == true)
		MoveDoor();
	else
		EnableLoop(false);
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

	SetRotation(0, rotation, 0);
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

bool Door::ToggleLock(const Ogre::Vector3 &position, bool force)
{
	//toggle lock state of the related door side
	//if force is true, bypass key check

	bool replocked = false;

	//quit if user doesn't have key, if force is false
	if (KeyID != 0)
	{
		if (sbs->CheckKey(KeyID) == false && force == false)
			return sbs->ReportError("Need key " + ToString(KeyID) + " to lock/unlock door '" + GetName() + "'");
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
		sbs->Report("Locked door '" + GetName() + "'");
	else
		sbs->Report("Unlocked door '" + GetName() + "'");

	return true;
}

bool Door::GetSide(const Ogre::Vector3 &position)
{
	//return which side of the door the position is (false for negative/left/front, true for positive/right/back)

	if ((Direction >= 1 && Direction <= 4) && position.x > GetPosition().x)
		return true;

	if ((Direction >= 5 && Direction <= 8) && position.z > GetPosition().z)
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

void Door::ClickDoor(Ogre::Vector3 &position)
{
	//code that runs when a user clicks on a door

	//open and close doors
	if (IsOpen() == false)
	{
		if (IsMoving == false)
			Open(position);
		else
			Close();
	}
	else
	{
		if (IsMoving == false)
			Close();
		else
			Open(position);
	}
}

void Door::OnClick(Ogre::Vector3 &position, bool shift, bool ctrl, bool alt, bool right)
{
	if (right == false)
	{
		//toggle lock status if ctrl and shift are pressed
		if (ctrl == true && shift == true)
			ToggleLock(position);
		else
			ClickDoor(position);
	}
}

}
