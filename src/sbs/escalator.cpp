/* $Id$ */

/*
	Scalable Building Simulator - Escalator Object
	The Skyscraper Project - Version 1.10 Alpha
	Copyright (C)2004-2015 Ryan Thoryk
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
#include "escalator.h"
#include "unix.h"

namespace SBS {

Escalator::Escalator(Object *parent, const std::string &name, bool run, bool run_direction, const std::string &sound_file, const std::string &texture, const std::string &direction, float CenterX, float CenterZ, float width, float risersize, float treadsize, int num_steps, float voffset, float tw, float th)
{
	//create a new escalator object
	//num_stairs is subtracted by 1 since it includes the floor platform above, but not below
	//direction is where the stair base is - front, back, left, or right.

	//set up SBS object
	SetValues(parent, "Escalator", name, false);

	is_enabled = true;
	Run = run;
	RunDirection = run_direction;
	sbs->IncrementEscalatorCount();

	//move object
	Move(CenterX, voffset, CenterZ);

	//create step meshes
	for (int i = 0; i < num_steps; i++)
	{
		MeshObject *mesh = new MeshObject(this, name);
		Steps.push_back(mesh);
	}

	//create sound object
	sound = new Sound(this, name, true);
	sound->Load(sound_file);

	//create steps
	CreateSteps(texture, direction, width, risersize, treadsize, tw, th);
}

Escalator::~Escalator()
{
	if (sound)
	{
		sound->parent_deleting = true;
		delete sound;
	}
	sound = 0;

	//remove step meshes
	for (int i = 0; i < (int)Steps.size(); i++)
	{
		if (Steps[i])
		{
			Steps[i]->parent_deleting = true;
			delete Steps[i];
		}
		Steps[i] = 0;
	}

	//unregister from parent
	if (sbs->FastDelete == false)
	{
		sbs->DecrementEscalatorCount();

		//unregister from parent
		if (parent_deleting == false)
		{
			std::string type = GetParent()->GetType();

			if (type == "Floor")
				static_cast<Floor*>(GetParent())->RemoveEscalator(this);
		}
	}
}

void Escalator::Enabled(bool value)
{
	//enable or disable escalator

	if (is_enabled == value)
		return;

	for (int i = 0; i < (int)Steps.size(); i++)
		Steps[i]->Enable(value);

	if (value == false && sound->IsPlaying() == true)
		sound->Stop();

	is_enabled = value;
}

void Escalator::Report(const std::string &message)
{
	//general reporting function
	sbs->Report("Escalator " + GetName() + ": " + message);
}

bool Escalator::ReportError(const std::string &message)
{
	//general reporting function
	return sbs->ReportError("Escalator " + GetName() + ": " + message);
}

void Escalator::Loop()
{
	//run loop

	if (!IsEnabled() || !Run)
	{
		if (sound->IsPlaying() == true)
			sound->Stop();
		return;
	}

	if (sound->IsPlaying() == false)
	{
		sound->SetLoopState(true);
		sound->Play();
	}
}

void Escalator::CreateSteps(const std::string &texture, const std::string &direction, float width, float risersize, float treadsize, float tw, float th)
{
	//create steps
	std::string Name = GetName();
	TrimString(Name);
	std::string Direction = direction;
	SetCase(Direction, false);
	int num_steps = (int)Steps.size();

	sbs->ResetTextureMapping(true);
	if (Direction == "right" || Direction == "back")
		sbs->SetWallOrientation("right");
	if (Direction == "left" || Direction == "front")
		sbs->SetWallOrientation("left");

	for (int i = 1; i <= num_steps; i++)
	{
		float pos = 0;
		std::string base = Name + " " + ToString(i);
		std::string buffer;

		//create wall object
		WallObject *wall = Steps[i]->CreateWallObject(base);

		float thickness = 0;
		if (i < num_steps - 1)
			thickness = treadsize * 2;
		if (i == num_steps - 1)
			thickness = treadsize;
		if (i == num_steps)
			thickness = 0;

		if (Direction == "right")
		{
			pos = ((treadsize * (num_steps - 1)) / 2) - (treadsize * i);
			buffer = base + "-riser";
			if (i != num_steps)
				sbs->DrawWalls(true, true, true, true, false, true);
			else
				sbs->DrawWalls(true, true, false, false, false, false);
			sbs->AddWallMain(wall, buffer, texture, thickness, pos + treadsize, -(width / 2), pos + treadsize, width / 2, risersize, risersize, risersize * (i - 1), risersize * (i - 1), tw, th, true);
			buffer = base + "-tread";
			if (i != num_steps)
			{
				sbs->DrawWalls(false, true, false, false, false, false);
				sbs->AddFloorMain(wall, buffer, texture, 0, pos, -(width / 2), pos + treadsize, width / 2, risersize * i, risersize * i, false, false, tw, th, true);
			}
		}
		if (Direction == "left")
		{
			pos = -((treadsize * (num_steps - 1)) / 2) + (treadsize * i);
			buffer = base + "-riser";
			if (i != num_steps)
				sbs->DrawWalls(true, true, true, true, false, true);
			else
				sbs->DrawWalls(true, true, false, false, false, false);
			sbs->AddWallMain(wall, buffer, texture, thickness, pos - treadsize, width / 2, pos - treadsize, -(width / 2), risersize, risersize, risersize * (i - 1), risersize * (i - 1), tw, th, true);
			buffer = base + "-tread";
			if (i != num_steps)
			{
				sbs->DrawWalls(false, true, false, false, false, false);
				sbs->AddFloorMain(wall, buffer, texture, 0, pos - treadsize, -(width / 2), pos, width / 2, risersize * i, risersize * i, false, false, tw, th, true);
			}
		}
		if (Direction == "back")
		{
			pos = ((treadsize * (num_steps - 1)) / 2) - (treadsize * i);
			buffer = base + "-riser";
			if (i != num_steps)
				sbs->DrawWalls(true, true, true, true, false, true);
			else
				sbs->DrawWalls(true, true, false, false, false, false);
			sbs->AddWallMain(wall, buffer, texture, thickness, width / 2, pos + treadsize, -(width / 2), pos + treadsize, risersize, risersize, risersize * (i - 1), risersize * (i - 1), tw, th, true);
			buffer = base + "-tread";
			if (i != num_steps)
			{
				sbs->DrawWalls(false, true, false, false, false, false);
				sbs->AddFloorMain(wall, buffer, texture, 0, -(width / 2), pos, width / 2, pos + treadsize, risersize * i, risersize * i, false, false, tw, th, true);
			}
		}
		if (Direction == "front")
		{
			pos = -((treadsize * (num_steps - 1)) / 2) + (treadsize * i);
			buffer = base + "-riser";
			if (i != num_steps)
				sbs->DrawWalls(true, true, true, true, false, true);
			else
				sbs->DrawWalls(true, true, false, false, false, false);
			sbs->AddWallMain(wall, buffer, texture, thickness, -(width / 2), pos - treadsize, width / 2, pos - treadsize, risersize, risersize, risersize * (i - 1), risersize * (i - 1), tw, th, true);
			buffer = base + "-tread";
			if (i != num_steps)
			{
				sbs->DrawWalls(false, true, false, false, false, false);
				sbs->AddFloorMain(wall, buffer, texture, 0, -(width / 2), pos - treadsize, width / 2, pos, risersize * i, risersize * i, false, false, tw, th, true);
			}
		}
	}
	sbs->ResetWalls(true);
	sbs->ResetTextureMapping();
}

}
