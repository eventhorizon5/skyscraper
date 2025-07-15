/*
	Scalable Building Simulator - Reverb Object
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

#ifndef DISABLE_SOUND
	#include <fmod.hpp>
	#include <fmod_errors.h>
#endif
#include "globals.h"
#include "sbs.h"
#include "utility.h"
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
#ifndef DISABLE_SOUND
	reverb = 0;
#endif

	//convert type to lowercase
	SetCase(type, false);

	//create reverb object

#ifndef DISABLE_SOUND
	FMOD_RESULT result = soundsys->GetFmodSystem()->createReverb3D(&reverb);
	if (result != FMOD_OK)
	{
		std::string fmod_result = FMOD_ErrorString(result);
		ReportError("Can't create reverb:\n" + fmod_result);
		reverb = 0;
		return;
	}

	//set reverb type
	FMOD_REVERB_PROPERTIES prop;

	if (type == "generic")
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
	else if (type == "carpetedhallway")
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
	FMOD_VECTOR fmod_pos;
	Vector3 global_pos = sbs->GetUtility()->ToGlobal(position);
	fmod_pos.x = global_pos.x;
	fmod_pos.y = global_pos.y;
	fmod_pos.z = global_pos.z;
	Move(position);

	FMOD_RESULT result2 = reverb->set3DAttributes(&fmod_pos, min_distance, max_distance);
	if (result2 != FMOD_OK)
	{
		std::string fmod_result = FMOD_ErrorString(result);
		ReportError("Can't set reverb attributes:\n" + fmod_result);
		reverb->release();
		reverb = 0;
		return;
	}

	sbs->IncrementReverbCount();
	Report("Reverb '" + name + "' created with type: " + type);
#endif
}

Reverb::~Reverb()
{
	sbs->DecrementReverbCount();

#ifndef DISABLE_SOUND
	//release reverb instance
	if (reverb)
		reverb->release();
#endif

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
	//reposition FMOD reverb on object move

#ifndef DISABLE_SOUND
	if (!reverb)
		return;

	Vector3 global_position = sbs->GetUtility()->ToGlobal(GetPosition());

	FMOD_VECTOR pos = {(float)global_position.x, (float)global_position.y, (float)global_position.z};

	//note - do not use ToRemote for positioning
	FMOD_RESULT result = reverb->set3DAttributes(&pos, MinDistance, MaxDistance);
	if (result != FMOD_OK)
	{
		std::string fmod_result = FMOD_ErrorString(result);
		ReportError("Can't set reverb attributes:\n" + fmod_result);
	}
#endif
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
#ifndef DISABLE_SOUND
	if (reverb)
		reverb->setActive(value);
#endif
}

}
