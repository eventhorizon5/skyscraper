/*
	Scalable Building Simulator - Door Object
	The Skyscraper Project - Version 1.12 Alpha
	Copyright (C)2004-2024 Ryan Thoryk
	https://www.skyscrapersim.net
	https://sourceforge.net/projects/skyscraper/
	Contact - ryan@skyscrapersim.net

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
#include "timer.h"
#include "doorsystem.h"
#include "door.h"

namespace SBS {

//auto-close timer
class Door::Timer : public TimerObject
{
public:
	Door *door;
	Timer(const std::string &name, Door *parent) : TimerObject(parent, name)
	{
		door = parent;
	}
	virtual void Notify();
};

Door::Door(Object *parent, DynamicMesh *wrapper, const std::string &name, const std::string &open_sound, const std::string &close_sound, bool rotate) : Object(parent), DoorLock(this)
{
	//creates a door, the 'direction' parameter is for the lock direction
	//wall cuts must be performed by the calling (parent) function

	//set up SBS object
	SetValues("Door", name, false);

	is_enabled = true;

	OpenState = false;
	IsMoving = false;
	OpenDoor = false;
	OpenSound = open_sound;
	CloseSound = close_sound;
	sound = 0;
	door_changed = false;
	previous_open = false;
	this->wrapper = wrapper;
	running = false;
	DoorDirection = false;
	timer = new Timer("Auto-close Timer", this);
	timer_interval = 0;

	//create door wrapper
	door = new DoorWrapper(this, rotate);

	//create sound object
	if (open_sound != "" || close_sound != "")
		sound = new Sound(this, "DoorSound", true);
}

Door::~Door()
{
	//destructor

	if (timer)
	{
		timer->parent_deleting = true;
		delete timer;
	}
	timer = 0;

	if (sound)
	{
		sound->parent_deleting = true;
		delete sound;
	}
	sound = 0;

	if (door)
	{
		door->parent_deleting = true;
		delete door;
	}
	door = 0;

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
			else if (type == "Shaft Level")
				static_cast<Shaft::Level*>(GetParent())->RemoveDoor(this);
			else if (type == "Stairwell Level")
				static_cast<Stairwell::Level*>(GetParent())->RemoveDoor(this);
			else if (type == "DoorManager")
				sbs->GetDoorManager()->RemoveDoor(this);
		}
	}
}

bool Door::Open(Vector3 &position, bool playsound, bool force)
{
	//position is the camera position, used to check if the door is locked
	//if force is true, locking/position check will be bypassed

	Report("Opening");

	EnableLoop(true);

	if (force == false)
	{
		//check lock state
		if (IsLocked(position) == true)
			return ReportError("Is locked");
	}

	OpenDoor = true;

	if (playsound == true && sound)
	{
		sound->Load(OpenSound);
		sound->Play();
	}

	OpenState = false;
	IsMoving = true;
	door->ResetFinished();

	//enable autoclose timer if needed
	AutoClose(timer_interval);

	return true;
}

void Door::Close(bool playsound)
{
	Report("Closing");

	EnableLoop(true);

	OpenDoor = false;

	if (playsound == true && sound)
	{
		sound->Load(CloseSound);
		sound->Play();
	}

	OpenState = true;
	IsMoving = true;
	door->ResetFinished();
}

bool Door::IsOpen()
{
	return OpenState;
}

void Door::Enabled(bool value)
{
	if (is_enabled == value)
		return;

	door->Enabled(value);
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

	//if a different direction was specified during movement
	if (running == true && previous_open != OpenDoor && door_changed == false)
		door_changed = true;

	running = true;

	//perform door movement and get open state of each door
	door->MoveDoors(OpenDoor);

	previous_open = OpenDoor;

	//wait until all door components are finished moving
	door->CheckDoorsOpen();
	if (door->IsFinished() == false)
		return;

	//the doors are open or closed now
	door_changed = false;
	IsMoving = false;
	running = false;

	if (OpenDoor == true)
		OpenState = true;
	else
		OpenState = false;
}

void Door::ClickDoor(Vector3 &position)
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

void Door::OnClick(Vector3 &position, bool shift, bool ctrl, bool alt, bool right)
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

void Door::Report(const std::string &message)
{
	//general reporting function
	Object::Report("Door " + GetName() + ": " + message);
}

bool Door::ReportError(const std::string &message)
{
	//general error reporting function
	return Object::ReportError("Door " + GetName() + ": " + message);
}

bool Door::GetDoorChanged()
{
	return door_changed;
}

bool Door::GetPreviousOpen()
{
	return previous_open;
}

DoorWrapper* Door::CreateDoor(bool open_state, const std::string &texture, const std::string &side_texture, Real thickness, const std::string &face_direction, const std::string &open_direction, Real open_speed, Real close_speed, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, Real tw, Real th, Real side_tw, Real side_th)
{
	//create a door

	Vector3 position;
	Real x1 = 0, z1 = 0, x2 = 0, z2 = 0;

	//set speed to default value if invalid
	if (open_speed <= 0)
		open_speed = sbs->GetConfigFloat("Skyscraper.SBS.DoorSpeed", 75.0);

	if (open_speed <= 0)
		open_speed = 75;

	if (close_speed <= 0)
		close_speed = open_speed;

	//set origin to location of the door's hinge/pivot point and set up door coordinates
	if (face_direction == "left")
	{
		position = Vector3(CenterX, voffset, CenterZ - (width / 2)); //front
		x1 = 0;
		x2 = 0;
		z1 = 0;
		z2 = width;
	}
	if (face_direction == "right")
	{
		position = Vector3(CenterX, voffset, CenterZ + (width / 2)); //back
		x1 = 0;
		x2 = 0;
		z1 = -width;
		z2 = 0;
	}
	if (face_direction == "front")
	{
		position = Vector3(CenterX + (width / 2), voffset, CenterZ); //right
		x1 = -width;
		x2 = 0;
		z1 = 0;
		z2 = 0;
	}
	if (face_direction == "back")
	{
		position = Vector3(CenterX - (width / 2), voffset, CenterZ); //left
		x1 = 0;
		x2 = width;
		z1 = 0;
		z2 = 0;
	}

	bool Clockwise = true;
	if (face_direction == open_direction)
		Clockwise = false;

	//create door
	AddDoorComponent(GetName(), texture, side_texture, thickness, face_direction, open_direction, Clockwise, open_speed, close_speed, x1, z1, x2, z2, height, 0, tw, th, 0, 0);
	FinishDoor(open_state);
	Move(position);

	return door;
}

DoorWrapper* Door::AddDoorComponent(const std::string &name, const std::string &texture, const std::string &sidetexture, Real thickness, const std::string &face_direction, const std::string &open_direction, bool OpenClockwise, Real OpenSpeed, Real CloseSpeed, Real x1, Real z1, Real x2, Real z2, Real height, Real voffset, Real tw, Real th, Real side_tw, Real side_th)
{
	//create door component

	bool direction = false;
	if (face_direction == "right" || face_direction == "back")
		direction = true;

	DoorComponent *component = door->CreateDoor(name, open_direction, OpenClockwise, OpenSpeed, CloseSpeed, wrapper);

	sbs->GetTextureManager()->ResetTextureMapping(true);
	if (direction == false)
		sbs->GetTextureManager()->SetTextureFlip(0, 1, 0, 0, 0, 0); //flip texture on rear side of door
	else
		sbs->GetTextureManager()->SetTextureFlip(1, 0, 0, 0, 0, 0); //flip texture on rear side of door

	//add main walls
	sbs->DrawWalls(true, true, false, false, false, false);
	Wall *wall;
	wall = component->mesh->CreateWallObject(name);
	sbs->AddWallMain(wall, name, texture, thickness, x1, z1, x2, z2, height, height, voffset, voffset, tw, th, false);
	sbs->ResetWalls();
	sbs->GetTextureManager()->ResetTextureMapping();

	//add side walls
	sbs->DrawWalls(false, false, true, true, true, true);
	wall = component->mesh->CreateWallObject(name);
	sbs->AddWallMain(wall, name, sidetexture, thickness, x1, z1, x2, z2, height, height, voffset, voffset, side_tw, side_th, false);
	sbs->ResetWalls();

	//store extents
	if (x1 < x2)
	{
		component->extents_min.x = x1;
		component->extents_max.x = x2;
	}
	else
	{
		component->extents_min.x = x2;
		component->extents_max.x = x1;
	}
	if (z1 < z2)
	{
		component->extents_min.z = z1;
		component->extents_max.z = z2;
	}
	else
	{
		component->extents_min.z = z2;
		component->extents_max.z = z1;
	}
	component->extents_min.y = voffset;
	component->extents_max.y = voffset + height;

	return door;
}

DoorWrapper* Door::FinishDoor(bool open_state)
{
	//finishes a door creation

	//exit if no doors exist
	if (door->doors.empty() == true)
	{
		ReportError("FinishDoors: no door components have been created");
		return 0;
	}

	//get full width and height of doors
	Real x1 = 0, x2 = 0, y1 = 0, y2 = 0, z1 = 0, z2 = 0;
	bool firstrun = true;
	for (size_t i = 0; i < door->doors.size(); i++)
	{
		for (int j = 1; j <= 3; j++)
		{
			Vector2 extents = door->doors[i]->mesh->GetExtents(j, true);
			extents.x = sbs->ToLocal(extents.x);
			extents.y = sbs->ToLocal(extents.y);

			if (j == 1)
			{
				if (extents.x < x1 || firstrun == true)
					x1 = extents.x;
				if (extents.y > x2 || firstrun == true)
					x2 = extents.y;
			}
			if (j == 2)
			{
				if (extents.x < y1 || firstrun == true)
					y1 = extents.x;
				if (extents.y > y2 || firstrun == true)
					y2 = extents.y;
			}
			if (j == 3)
			{
				if (extents.x < z1 || firstrun == true)
					z1 = extents.x;
				if (extents.y > z2 || firstrun == true)
					z2 = extents.y;
				firstrun = false;
			}
		}
	}

	if (x2 - x1 > z2 - z1)
	{
		door->Width = x2 - x1;
		door->Thickness = z2 - z1;
		door->Shift = x2 - (door->Width / 2);
		SetDirection(true);
		DoorDirection = true;
	}
	else
	{
		door->Width = z2 - z1;
		door->Thickness = x2 - x1;
		door->Shift = z2 - (door->Width / 2);
		SetDirection(false);
		DoorDirection = false;
	}
	door->Height = y2 - y1;
	if (y2 < y1)
		door->voffset = y2;
	else
		door->voffset = y1;

	//open door on startup (without sound) if specified
	if (open_state == true)
	{
		Vector3 pos = GetPosition();
		Open(pos, false, true);
	}

	return door;
}

void Door::AutoClose(int interval)
{
	//start or stop autoclose timer

	if (interval <= 0)
		timer->Stop();
	else
		timer->Start(interval * 1000, true);

	timer_interval = interval;
}

void Door::Timer::Notify()
{
	//when auto-close timer triggers, close door

	//close door
	door->Close();
}

}
