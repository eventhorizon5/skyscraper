/* $Id$ */

/*
	Scalable Building Simulator - Revolving Door Object
	The Skyscraper Project - Version 1.11 Alpha
	Copyright (C)2004-2022 Ryan Thoryk
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
#include "texture.h"
#include "sound.h"
#include "revolvingdoor.h"

namespace SBS {

RevolvingDoor::RevolvingDoor(Object *parent, DynamicMesh *wrapper, const std::string &name, const std::string &soundfile, const std::string &texture, Real thickness, bool clockwise, int segments, Real speed, Real rotation, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, Real tw, Real th) : Object(parent), Lock(this)
{
	//creates a revolving door
	//wall cuts must be performed by the calling (parent) function
	//segments can be 2-4

	//set up SBS object
	SetValues("RevolvingDoor", name, false);

	is_enabled = true;
	IsMoving = false;
	this->rotation = 0;
	this->soundfile = soundfile;
	Speed = speed;
	sound = 0;
	brake = false;

	//set speed to default value if invalid
	if (Speed <= 0)
		Speed = sbs->GetConfigFloat("Skyscraper.SBS.RevolvingDoorSpeed", 75.0);

	if (Speed <= 0)
		Speed = 75;

	Ogre::Vector3 position (CenterX, voffset, CenterZ);

	Clockwise = clockwise;

	//Create mesh
	DoorMesh = new MeshObject(this, name, wrapper);
	Move(position);

	//create sound object
	if (soundfile != "")
		sound = new Sound(this, "DoorSound", true);

	//create door
	sbs->DrawWalls(true, true, true, true, true, true);
	sbs->GetTextureManager()->ResetTextureMapping(true);

	Wall *wall = DoorMesh->CreateWallObject(name);

	Segments = segments;
	width = width / 2;
	if (Segments == 3)
	{
		sbs->GetTextureManager()->SetTextureFlip(1, 0, 0, 0, 0, 0); //flip texture on rear side of door
		sbs->AddWallMain(wall, name, texture, thickness, -width, -width, 0, 0, height, height, 0, 0, tw, th, false);
		sbs->AddWallMain(wall, name, texture, thickness, 0, 0, width, -width, height, height, 0, 0, tw, th, false);
		sbs->GetTextureManager()->SetTextureFlip(0, 1, 0, 0, 0, 0); //flip texture on rear side of door
		sbs->AddWallMain(wall, name, texture, thickness, 0, 0, 0, width, height, height, 0, 0, tw, th, false);
	}
	else
	{
		sbs->GetTextureManager()->SetTextureFlip(0, 1, 0, 0, 0, 0); //flip texture on rear side of door
		sbs->AddWallMain(wall, name, texture, thickness, -width, 0, 0, 0, height, height, 0, 0, tw, th, false);
		sbs->GetTextureManager()->SetTextureFlip(1, 0, 0, 0, 0, 0); //flip texture on rear side of door
		sbs->AddWallMain(wall, name, texture, thickness, 0, 0, width, 0, height, height, 0, 0, tw, th, false);
		if (Segments == 4)
		{
			sbs->GetTextureManager()->SetTextureFlip(1, 0, 0, 0, 0, 0); //flip texture on rear side of door
			sbs->AddWallMain(wall, name, texture, thickness, 0, -width, 0, 0, height, height, 0, 0, tw, th, false);
			sbs->GetTextureManager()->SetTextureFlip(0, 1, 0, 0, 0, 0); //flip texture on rear side of door
			sbs->AddWallMain(wall, name, texture, thickness, 0, 0, 0, width, height, height, 0, 0, tw, th, false);
		}
	}
	sbs->ResetWalls();
	sbs->GetTextureManager()->ResetTextureMapping();

	Rotate(GetRotation().x, rotation, GetRotation().z);
}

RevolvingDoor::~RevolvingDoor()
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

			if (type == "Floor")
				static_cast<Floor*>(GetParent())->RemoveRevolvingDoor(this);
			else if (type == "RevolvingDoorManager")
				sbs->GetRevolvingDoorManager()->RemoveDoor(this);
		}
	}
}

void RevolvingDoor::OnHit()
{
	if (sbs->Verbose)
		Report("Moving");

	EnableLoop(true);

	//check lock state
	if (IsLocked() == true)
	{
		ReportError("Is locked");
		return;
	}

	if (sound)
	{
		sound->Load(soundfile);
		sound->Play();
	}

	IsMoving = true;
	brake = false;
}

void RevolvingDoor::Enabled(bool value)
{
	if (is_enabled == value)
		return;

	DoorMesh->Enabled(value);
	is_enabled = value;
}

void RevolvingDoor::Loop()
{
	if (IsMoving == true)
		MoveDoor();
	else
		EnableLoop(false);
}

void RevolvingDoor::MoveDoor()
{
	//door movement callback function
	if ((Clockwise == true && brake == false) || (Clockwise == false && brake == true))
	{
		if (rotation < Speed)
			rotation += Speed * sbs->delta;
		else
			brake = !brake;
	}
	else if ((Clockwise == false && brake == false) || (Clockwise == true && brake == true))
	{
		if (rotation > -Speed)
			rotation -= Speed * sbs->delta;
		else
			brake = !brake;
	}

	if (brake == true)
	{
		if ((Clockwise == true && rotation <= 0) || (Clockwise == false && rotation >= 0))
			IsMoving = false;
	}

	Rotate(0, rotation, 0);
}

void RevolvingDoor::OnClick(Ogre::Vector3 &position, bool shift, bool ctrl, bool alt, bool right)
{
	if (right == false)
	{
		//toggle lock status if ctrl and shift are pressed
		if (ctrl == true && shift == true)
			ToggleLock();
	}
}

void RevolvingDoor::Report(const std::string &message)
{
	//general reporting function
	Object::Report("Revolving Door " + GetName() + ": " + message);
}

bool RevolvingDoor::ReportError(const std::string &message)
{
	//general error reporting function
	return Object::ReportError("Revolving Door " + GetName() + ": " + message);
}

}
