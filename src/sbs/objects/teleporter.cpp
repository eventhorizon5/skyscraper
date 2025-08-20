/*
	Scalable Building Simulator - Teleporter Object
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
#include "sound.h"
#include "action.h"
#include "trigger.h"
#include "teleporter.h"

namespace SBS {

Teleporter::Teleporter(Object *parent, const std::string &name, const std::string &idle_sound, const std::string &teleport_sound, Real width, Real height, const Vector3 &destination) : Object(parent)
{
	//set up SBS object
	SetValues("Teleporter", name, true);

	enabled = false;

	//create sound object
	sound = new Sound(this, name + " Idle", true);
	sound->Load(idle_sound);

	//set up bounding box
	Vector3 area_min (-(width / 2), 0, -(width / 2));
	Vector3 area_max ((width / 2), height, (width / 2));

	//set up command and parameters
	std::string command = "teleport";
	Ogre::StringVector parameters;
	parameters.emplace_back(ToString(destination.x));
	parameters.emplace_back(ToString(destination.y));
	parameters.emplace_back(ToString(destination.z));

	//set SBS as action parent
	std::vector<Object*> action_parents;
	action_parents.emplace_back(sbs);

	//create actions
	Action *home_action = sbs->AddAction(name + " Home", action_parents, command, parameters);
	Action *dest_action = sbs->AddAction(name + " Dest", action_parents, command, parameters);

	//create trigger object
	Ogre::StringVector action_names;
	action_names.emplace_back(home_action->GetName());
	action_names.emplace_back(dest_action->GetName());
	trigger = new Trigger(this, name + " Trigger", false, teleport_sound, area_min, area_max, action_names);
	trigger->teleporter = true;

	Enabled(true);
	EnableLoop(true);
}

Teleporter::~Teleporter()
{
	if (trigger)
	{
		trigger->parent_deleting = true;
		delete trigger;
	}
	trigger = 0;

	if (sound)
	{
		sound->parent_deleting = true;
		delete sound;
	}
	sound = 0;
}

bool Teleporter::Enabled(bool value)
{
	if (trigger)
		trigger->Enabled(value);
	if (sound)
	{
		if (value == true)
		{
			sound->SetLoopState(true);
			sound->Play();
		}
		else
			sound->Stop();
	}
	enabled = value;
	return true;
}

bool Teleporter::IsEnabled()
{
	return enabled;
}

bool Teleporter::Loop()
{
	if (enabled == false)
		return true;

	return LoopChildren();
}

void Teleporter::OnEntry()
{
	sbs->Report("Entered teleporter " + GetName());
}

void Teleporter::OnExit()
{
	sbs->Report("Exited teleporter " + GetName());
}

}
