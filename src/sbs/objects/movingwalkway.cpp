/*
	Scalable Building Simulator - Moving Walkway Object
	The Skyscraper Project - Version 2.1
	Copyright (C)2004-2025 Ryan Thoryk
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
#include "polymesh.h"
#include "camera.h"
#include "mesh.h"
#include "floor.h"
#include "sound.h"
#include "texture.h"
#include "profiler.h"
#include "dynamicmesh.h"
#include "step.h"
#include "movingwalkway.h"

namespace SBS {

MovingWalkway::MovingWalkway(Object *parent, const std::string &name, int run, Real speed, const std::string &sound_file, const std::string &texture, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real treadsize, int num_steps, Real voffset, Real tw, Real th) : Object(parent)
{
	//create a new moving walkway object
	//run is either 1 for forward motion, -1 for reverse motion, 0 for stop
	//direction is where the step base is - front, back, left, or right.

	//set up SBS object
	SetValues("MovingWalkway", "", false);

	std::string Name;
	Floor *floor = dynamic_cast<Floor*>(parent);
	if (floor)
		Name = "Floor" + ToString(floor->Number) + ":"+ name;
	else
		Name = name;
	SetName(Name);

	is_enabled = true;
	SetRun(run);
	Speed = speed;
	start = Vector3::ZERO;
	end = Vector3::ZERO;

	//register with engine
	sbs->RegisterMovingWalkway(this);

	//create sound object
	sound = new Sound(this, name, true);
	sound->Load(sound_file);

	//move object
	Move(CenterX, voffset, CenterZ);

	//create step meshes
	for (int i = 0; i < num_steps; i++)
	{
		Step *mesh = new Step(this, "Step " + ToString(i + 1), 0, 100);
		Steps.emplace_back(mesh);
	}

	//create steps
	CreateSteps(texture, direction, width, treadsize, tw, th);
}

MovingWalkway::~MovingWalkway()
{
	if (sound)
	{
		sound->parent_deleting = true;
		delete sound;
	}
	sound = 0;

	//remove step meshes
	for (size_t i = 0; i < Steps.size(); i++)
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
		sbs->UnregisterMovingWalkway(this);

		//unregister from parent
		if (parent_deleting == false)
		{
			std::string type = GetParent()->GetType();

			if (type == "Floor")
				static_cast<Floor*>(GetParent())->RemoveMovingWalkway(this);
		}
	}
}

void MovingWalkway::Enabled(bool value)
{
	//enable or disable walkway

	if (is_enabled == value)
		return;

	EnableLoop(value);

	for (size_t i = 0; i < Steps.size(); i++)
		Steps[i]->Enabled(value);

	if (value == false && sound->IsPlaying() == true)
		sound->Stop();

	is_enabled = value;
}

void MovingWalkway::SetRun(int value)
{
	if (value == 0)
	{
		for (size_t i = 0; i < Steps.size(); i++)
		{
			Steps[i]->vector = Vector3::ZERO;
			Steps[i]->speed = 0;
		}
	}

	Run = value;
}

void MovingWalkway::Report(const std::string &message)
{
	//general reporting function
	Object::Report("Moving Walkway " + GetName() + ": " + message);
}

bool MovingWalkway::ReportError(const std::string &message)
{
	//general reporting function
	return Object::ReportError("Moving Walkway " + GetName() + ": " + message);
}

void MovingWalkway::Loop()
{
	//run loop

	SBS_PROFILE("MovingWalkway::Loop");

	//only run if power is enabled
	if (sbs->GetPower() == false)
	{
		sound->Stop();
		return;
	}

	if (!IsEnabled() || Run == 0)
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

	MoveSteps();
}

void MovingWalkway::CreateSteps(const std::string &texture, const std::string &direction, Real width, Real treadsize, Real tw, Real th)
{
	//create steps
	std::string Name = GetName();
	TrimString(Name);
	Direction = direction;
	this->treadsize = treadsize;
	SetCase(Direction, false);
	int num_steps = (int)Steps.size();

	sbs->GetTextureManager()->ResetTextureMapping(true);
	if (Direction == "right" || Direction == "back")
		sbs->GetPolyMesh()->SetWallOrientation("right");
	if (Direction == "left" || Direction == "front")
		sbs->GetPolyMesh()->SetWallOrientation("left");

	for (int i = 1; i <= num_steps; i++)
	{
		Real pos = 0;
		std::string base = Name + ":" + ToString(i);

		//create wall object
		Wall *wall = Steps[i - 1]->CreateWallObject(base);

		Real thickness = treadsize;

		sbs->GetPolyMesh()->DrawWalls(false, true, false, false, false, false);

		if (Direction == "right")
		{
			pos = ((treadsize * num_steps + 1) / 2) - (treadsize * i);
			sbs->GetPolyMesh()->AddFloorMain(wall, base, texture, 0, 0, -(width / 2), treadsize, width / 2, 0, 0, false, false, tw, th, true);
			Steps[i - 1]->Move(Vector3(pos, 0, 0));
		}
		if (Direction == "left")
		{
			pos = -((treadsize * num_steps + 1) / 2) + (treadsize * i);
			sbs->GetPolyMesh()->AddFloorMain(wall, base, texture, 0, -treadsize, -(width / 2), 0, width / 2, 0, 0, false, false, tw, th, true);
			Steps[i - 1]->Move(Vector3(pos, 0, 0));
		}
		if (Direction == "back")
		{
			pos = ((treadsize * num_steps + 1) / 2) - (treadsize * i);
			sbs->GetPolyMesh()->AddFloorMain(wall, base, texture, 0, -(width / 2), 0, width / 2, treadsize, 0, 0, false, false, tw, th, true);
			Steps[i - 1]->Move(Vector3(0, 0, pos));
		}
		if (Direction == "front")
		{
			pos = -((treadsize * num_steps + 1) / 2) + (treadsize * i);
			sbs->GetPolyMesh()->AddFloorMain(wall, base, texture, 0, -(width / 2), -treadsize, width / 2, 0, 0, 0, false, false, tw, th, true);
			Steps[i - 1]->Move(Vector3(0, 0, pos));
		}
		Steps[i - 1]->vector = Vector3::ZERO;
		Steps[1 - 1]->speed = 0;

		if (i == 1)
			start = Steps[i - 1]->GetPosition();
		if (i == num_steps)
			end = Steps[i - 1]->GetPosition();
		Steps[i - 1]->start = Steps[i - 1]->GetPosition();
	}

	sbs->GetPolyMesh()->ResetWalls(true);
	sbs->GetTextureManager()->ResetTextureMapping();
}

void MovingWalkway::MoveSteps()
{
	if (GetPosition().distance(sbs->camera->GetPosition()) > 100)
		return;

	for (size_t i = 0; i < Steps.size(); i++)
	{
		if (Run == 1)
		{
			if (Direction == "right")
			{
				Real pos = Steps[i]->GetPosition().x;
				if (pos < end.x - treadsize)
					Steps[i]->SetPosition(start);
				else
					Steps[i]->Move(Vector3(-Run, 0, 0), Speed * sbs->delta);
			}
			if (Direction == "left")
			{
				Real pos = Steps[i]->GetPosition().x;
				if (pos > end.x + treadsize)
					Steps[i]->SetPosition(start);
				else
					Steps[i]->Move(Vector3(Run, 0, 0), Speed * sbs->delta);
			}
			if (Direction == "back")
			{
				Real pos = Steps[i]->GetPosition().z;
				if (pos < end.z - treadsize)
					Steps[i]->SetPosition(start);
				else
					Steps[i]->Move(Vector3(0, 0, -Run), Speed * sbs->delta);
			}
			if (Direction == "front")
			{
				Real pos = Steps[i]->GetPosition().z;
				if (pos > end.z + treadsize)
					Steps[i]->SetPosition(start);
				else
					Steps[i]->Move(Vector3(0, 0, Run), Speed * sbs->delta);
			}
		}
		else if (Run == -1)
		{
			if (Direction == "right")
			{
				Real pos = Steps[i]->GetPosition().x;
				if (pos > start.x)
					Steps[i]->SetPosition(Vector3(end.x - treadsize, end.y, end.z));
				else
					Steps[i]->Move(Vector3(-Run, 0, 0), Speed * sbs->delta);
			}
			if (Direction == "left")
			{
				Real pos = Steps[i]->GetPosition().x;
				if (pos < start.x)
					Steps[i]->SetPosition(Vector3(end.x + treadsize, end.y, end.z));
				else
					Steps[i]->Move(Vector3(Run, 0, 0), Speed * sbs->delta);
			}
			if (Direction == "back")
			{
				Real pos = Steps[i]->GetPosition().z;
				if (pos > start.z)
					Steps[i]->SetPosition(Vector3(end.x, end.y, end.z - treadsize));
				else
					Steps[i]->Move(Vector3(0, 0, -Run), Speed * sbs->delta);
			}
			if (Direction == "front")
			{
				Real pos = Steps[i]->GetPosition().z;
				if (pos < start.z)
					Steps[i]->SetPosition(Vector3(end.x, end.y, end.z + treadsize));
				else
					Steps[i]->Move(Vector3(0, 0, Run), Speed * sbs->delta);
			}
		}
	}
}

void MovingWalkway::OnClick(Vector3 &position, bool shift, bool ctrl, bool alt, bool right)
{
	//cycle run stages if shift-click is performed

	//only run if power is enabled
	if (sbs->GetPower() == false)
		return;

	if (shift == true)
	{
		if (Run == 1)
		{
			Run = 0;
			for (size_t i = 0; i < Steps.size(); i++)
			{
				Steps[i]->vector = Vector3::ZERO;
				Steps[i]->speed = 0;
			}
		}
		else if (Run == 0)
			Run = -1;
		else if (Run == -1)
			Run = 1;
	}
}

void MovingWalkway::ResetState()
{
	//reset walkway state

	Run = 0;
	for (size_t i = 0; i < Steps.size(); i++)
	{
		Steps[i]->SetPosition(Steps[i]->start);
	}
}

}
