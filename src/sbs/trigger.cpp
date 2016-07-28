/* $Id$ */

/*
	Scalable Building Simulator - Proximity Trigger Object
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

#include "OgreAxisAlignedBox.h"
#include "globals.h"
#include "sbs.h"
#include "floor.h"
#include "elevatorcar.h"
#include "shaft.h"
#include "stairs.h"
#include "camera.h"
#include "sound.h"
#include "action.h"
#include "profiler.h"
#include "trigger.h"

namespace SBS {

Trigger::Trigger(Object *parent, const std::string &name, bool permanent, const std::string &sound_file, Ogre::Vector3 area_min, Ogre::Vector3 area_max, const std::vector<std::string> &action_names) : Object(parent)
{
	//create a proximity trigger at the specified location

	//set up SBS object
	SetValues("Trigger", name, permanent);

	Actions = action_names;
	is_enabled = true;
	current_position = 1;
	if (area_min.y == 0 && area_max.y == 0)
	{
		area_min.y = -999999.0;
		area_max.y = 999999.0;
	}
	area_box = new Ogre::AxisAlignedBox(area_min, area_max);
	is_inside = false;
	sound = 0;

	//create sound object
	if (sound_file != "")
	{
		sound = new Sound(this, name, true);
		sound->Load(sound_file);
	}
}

Trigger::~Trigger()
{
	if (sound)
	{
		sound->parent_deleting = true;
		delete sound;
	}
	sound = 0;

	if (area_box)
		delete area_box;
	area_box = 0;

	//unregister from parent
	if (sbs->FastDelete == false)
	{
		if (parent_deleting == false)
		{
			std::string type = GetParent()->GetType();

			if (type == "ElevatorCar")
				static_cast<ElevatorCar*>(GetParent())->RemoveTrigger(this);
			else if (type == "Floor")
				static_cast<Floor*>(GetParent())->RemoveTrigger(this);
			else if (type == "Shaft")
				static_cast<Shaft*>(GetParent())->RemoveTrigger(this);
			else if (type == "Stairs")
				static_cast<Stairs*>(GetParent())->RemoveTrigger(this);
			else if (type == "SBS")
				sbs->RemoveTrigger(this);
		}
	}
}

void Trigger::Enabled(bool value)
{
	//enable or disable trigger

	if (is_enabled == value)
		return;

	is_enabled = value;
}

bool Trigger::SetSelectPosition(int position)
{
	//set selection position without checking state

	if (position < 1 || position > GetPositions())
		return false;

	current_position = position;

	return true;
}

bool Trigger::ChangeSelectPosition(int position)
{
	//set selection position

	if (position == current_position)
		return false;

	return SetSelectPosition(position);
}

bool Trigger::NextSelectPosition(bool check_state)
{
	//change to the next available trigger selection position

	int position = GetNextSelectPosition();

	if (check_state == false)
		return SetSelectPosition(position);
	else
		return ChangeSelectPosition(position);
}

int Trigger::GetNextSelectPosition()
{
	//determine next selection position
	int pos = current_position;
	pos++;
	if (pos > GetPositions())
		pos = 1;
	return pos;
}

bool Trigger::PreviousSelectPosition(bool check_state)
{
	//change to the next available trigger selection position

	int position = GetPreviousSelectPosition();

	if (check_state == false)
		return SetSelectPosition(position);
	else
		return ChangeSelectPosition(position);
}

int Trigger::GetPreviousSelectPosition()
{
	//determine previous selection position
	int pos = current_position;
	pos--;
	if (pos < 1)
		pos = GetPositions();
	return pos;
}

void Trigger::PlaySound()
{
	//play associated sound

	if (sound)
	{
		sound->SetLoopState(false);
		sound->Play();
	}
}

int Trigger::GetSelectPosition()
{
	//return selection position
	return current_position;
}

std::string Trigger::GetPositionAction(int position)
{
	//return action name associated with the specified selection position

	if (Actions.empty() == true)
		return "";

	std::vector<Action*> actionlist;
	actionlist = sbs->GetAction(Actions[position - 1]);

	//return command of first action in list
	if (actionlist.empty() == false)
	{
		if (actionlist[0])
			return actionlist[0]->GetCommandName();
	}
	return "";
}

std::string Trigger::GetSelectPositionAction()
{
	//return action name associated with current selection position
	return GetPositionAction(current_position);
}

int Trigger::GetPositions()
{
	//return number of available positions, based on size of Actions array
	return (int)Actions.size();
}

int Trigger::FindActionPosition(const std::string &name)
{
	//returns the position number that the specified action name is associated with.
	//otherwise 0 if not found

	for (int i = 1; i <= GetPositions(); i++)
	{
		if (GetPositionAction(i) == name)
			return i;
	}

	return 0;
}

bool Trigger::DoAction()
{
	//perform trigger's action
	//result is true if at least one action in the list succeeded

	if (Actions.empty() == true)
		return sbs->ReportError("No available actions for trigger '" + GetName() + "'");

	std::vector<Action*> actionlist = sbs->GetAction(Actions[current_position - 1]);

	bool result = false;

	for (size_t i = 0; i < actionlist.size(); i++)
	{
		bool result2 = false;

		if (actionlist[i])
			result2 = actionlist[i]->DoAction(this);

		if (result2 == true)
			result = true;
	}
	return result;
}

void Trigger::Loop()
{
	//check for action; should be called in a loop by the parent object

	if (is_enabled == false)
		return;

	SBS_PROFILE("Trigger::Loop");

	Ogre::Vector3 cam = sbs->camera->GetPosition();
	bool changed = false;
	if (IsInside(cam) == true)
	{
		if (is_inside == false)
			changed = true;
		is_inside = true;
	}
	else
	{
		if (is_inside == true)
			changed = true;
		is_inside = false;
	}

	if (changed == true)
	{
		if (sbs->Verbose == true)
		{
			if (is_inside == true)
				sbs->Report("Inside trigger area '" + GetName() + "', parent '" + GetParent()->GetName() + "'");
			else
				sbs->Report("Outside trigger area '" + GetName() + "', parent '" + GetParent()->GetName() + "'");
		}

		//get action name of next position state
		std::string name = GetPositionAction(GetNextSelectPosition());

		//exit without changing position if floor button is currently selected
		if (name == "off" && IsNumeric(GetSelectPositionAction()) == true)
			return;

		//change to next control position
		NextSelectPosition();

		//perform selected action
		bool result = DoAction();

		//play sound if action succeeded
		//or always if the name is numeric (elevator is on the same floor, and doors are reopened)
		if (result == true || IsNumeric(name))
			PlaySound();

		//change back to original selection if result is false
		if (result == false)
			PreviousSelectPosition();
	}
}

bool Trigger::IsInside()
{
	//return inside status

	return is_inside;
}

bool Trigger::IsInside(const Ogre::Vector3 &position)
{
	//return true if the given absolute position is inside the trigger area

	return area_box->contains(position - GetPosition());
}

Ogre::AxisAlignedBox Trigger::GetBounds(bool relative)
{
	//get bounds information for this trigger

	Ogre::Vector3 min = area_box->getMinimum();
	Ogre::Vector3 max = area_box->getMaximum();

	if (relative == false)
	{
		min += GetPosition();
		max += GetPosition();
	}

	return Ogre::AxisAlignedBox(min, max);
}

bool Trigger::IsOutside(Ogre::Vector3 v1, Ogre::Vector3 v2)
{
	//return true if the given rectangle is outside of the trigger area

	v1 -= GetPosition();
	v2 -= GetPosition();
	Ogre::Vector3 min = area_box->getMinimum();
	Ogre::Vector3 max = area_box->getMaximum();

	if ((v1.x < min.x && v2.x < min.x) ||
		(v1.y < min.y && v2.y < min.y) ||
		(v1.z < min.z && v2.z < min.z) ||
		(v1.x > max.x && v2.x > max.x) ||
		(v1.y > max.y && v2.y > max.y) ||
		(v1.z > max.z && v2.z > max.z))
		return true;
	return false;
}

Ogre::Vector3 Trigger::GetMin()
{
	return area_box->getMinimum();
}

Ogre::Vector3 Trigger::GetMax()
{
	return area_box->getMaximum();
}

}
