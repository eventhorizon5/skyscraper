/*
	Scalable Building Simulator - Reverb Object
	The Skyscraper Project - Version 2.1
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

#include <fmod.hpp>
#include <fmod_errors.h>
#include "globals.h"
#include "sbs.h"
#include "floor.h"
#include "elevatorcar.h"
#include "soundsystem.h"
#include "reverb.h"

namespace SBS {

Reverb::Reverb(Object *parent, const std::string &name, std::string type, const Vector3 &position, Real min_distance, Real max_distance, bool permanent) : Object(parent)
{
	//set up SBS object
	SetValues("Reverb", name, permanent);

	//first set default values
	soundsys = sbs->GetSoundSystem();
	MaxDistance = max_distance;
	MinDistance = min_distance;
	reverb = 0;

	//convert type to lowercase
	SetCase(type, false);

	//create reverb object

	FMOD_RESULT result = soundsys->GetFmodSystem()->createReverb3D(&reverb);
	if (result != FMOD_OK)
	{
		std::string fmod_result = FMOD_ErrorString(result);
		ReportError("Can't create reverb:\n" + fmod_result);
		return;
	}

	//set reverb type
	FMOD_REVERB_PROPERTIES prop;

	if (type == "off")
		prop = FMOD_PRESET_OFF;
	else if (type == "generic")
		prop = FMOD_PRESET_GENERIC;
	else if (type == "paddedcell")
		prop = FMOD_PRESET_PADDEDCELL;
	else if (type == "room")
		prop = FMOD_PRESET_ROOM;
	else if (type == "bathroom")
		prop = FMOD_PRESET_BATHROOM;
	else if (type == "livingroom")
		prop = FMOD_PRESET_LIVINGROOM;
	else if (type == "stoneroom")
		prop = FMOD_PRESET_STONEROOM;
	else if (type == "auditorium")
		prop = FMOD_PRESET_AUDITORIUM;
	else if (type == "concerthall")
		prop = FMOD_PRESET_CONCERTHALL;
	else if (type == "cave")
		prop = FMOD_PRESET_CAVE;
	else if (type == "arena")
		prop = FMOD_PRESET_ARENA;
	else if (type == "hangar")
		prop = FMOD_PRESET_HANGAR;
	else if (type == "carpettedhallway")
		prop = FMOD_PRESET_CARPETTEDHALLWAY;
	else if (type == "hallway")
		prop = FMOD_PRESET_HALLWAY;
	else if (type == "stonecorridor")
		prop = FMOD_PRESET_STONECORRIDOR;
	else if (type == "alley")
		prop = FMOD_PRESET_ALLEY;
	else if (type == "forest")
		prop = FMOD_PRESET_FOREST;
	else if (type == "city")
		prop = FMOD_PRESET_CITY;
	else if (type == "mountains")
		prop = FMOD_PRESET_MOUNTAINS;
	else if (type == "quarry")
		prop = FMOD_PRESET_QUARRY;
	else if (type == "plain")
		prop = FMOD_PRESET_PLAIN;
	else if (type == "parkinglot")
		prop = FMOD_PRESET_PARKINGLOT;
	else if (type == "sewerpipe")
		prop = FMOD_PRESET_SEWERPIPE;
	else if (type == "underwater")
		prop = FMOD_PRESET_UNDERWATER;

	reverb->setProperties(&prop);

	//set reverb position
	FMOD_VECTOR pos;
	Vector3 global_pos = sbs->ToGlobal(position);
	pos.x = global_pos.x;
	pos.y = global_pos.y;
	pos.z = global_pos.z;
	this->position = global_pos;

	FMOD_RESULT result2 = reverb->set3DAttributes(&pos, min_distance, max_distance);
	if (result != FMOD_OK)
	{
		std::string fmod_result = FMOD_ErrorString(result);
		ReportError("Can't set reverb attributes:\n" + fmod_result);
		//delete reverb;
		return;
	}

	sbs->IncrementReverbCount();
	Report("Reverb '" + name + "' created with type: " + type);
}

Reverb::~Reverb()
{
	sbs->DecrementReverbCount();

	if (soundsys)
	{
		if (sbs->FastDelete == false)
		{
		}
	}

	//unregister from parent
	if (sbs->FastDelete == false)
	{
		if (parent_deleting == false)
		{
			std::string type = GetParent()->GetType();

			if (type == "ElevatorCar")
				static_cast<ElevatorCar*>(GetParent())->RemoveReverb();
			else if (type == "Floor")
				static_cast<Floor*>(GetParent())->RemoveReverb(this);
			else if (type == "SBS")
				sbs->RemoveReverb(this);
		}
	}
}

void Reverb::OnMove(bool parent)
{
	if (!reverb)
		return;

	Vector3 global_position = sbs->ToGlobal(GetPosition());

	FMOD_VECTOR pos = {(float)global_position.x, (float)global_position.y, (float)global_position.z};

	position = GetPosition();

	//note - do not use ToRemote for positioning
	FMOD_RESULT result = reverb->set3DAttributes(&pos, MinDistance, MaxDistance);
	if (result != FMOD_OK)
	{
		std::string fmod_result = FMOD_ErrorString(result);
		ReportError("Can't set reverb attributes:\n" + fmod_result);
	}
}

Real Reverb::GetMinimumDistance()
{
	return MinDistance;
}

Real Reverb::GetMaximumDistance()
{
	return MaxDistance;
}

void Reverb::Report(const std::string &message)
{
	Object::Report("Reverb '" + GetName() + "', parent '" + GetParent()->GetName() + "': " + message);
}

bool Reverb::ReportError(const std::string &message)
{
	return Object::ReportError("Reverb '" + GetName() + "', parent '" + GetParent()->GetName() + "': " + message);
}

void Reverb::Enable(bool value)
{
	if (reverb)
		reverb->setActive(value);
}

}
