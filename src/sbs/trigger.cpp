/* $Id$ */

/*
	Scalable Building Simulator - Proximity Trigger Object
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
#include "trigger.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

Trigger::Trigger(Object *parent, const char *name, bool permanent, const char *sound_file, Ogre::Vector3 &area_min, Ogre::Vector3 &area_max, const std::vector<std::string> &action_names)
{
	//create a proximity trigger at the specified location

	//set up SBS object
	SetValues(this, parent, "Trigger", name, permanent);

	Actions = action_names;
	IsEnabled = true;
	current_position = 1;
	this->area_min = area_min;
	this->area_max = area_max;
	is_inside = false;

	//create sound object
	sound = new Sound(this, name, true);
	sound->Load(sound_file);
}

Trigger::~Trigger()
{
	if (sound)
	{
		sound->parent_deleting = true;
		delete sound;
	}
	sound = 0;

	//unregister from parent
	if (sbs->FastDelete == false)
	{
		if (parent_deleting == false)
		{
			if (std::string(GetParent()->GetType()) == "Elevator")
				((Elevator*)GetParent()->GetRawObject())->RemoveTrigger(this);
			if (std::string(GetParent()->GetType()) == "Floor")
				((Floor*)GetParent()->GetRawObject())->RemoveTrigger(this);
			if (std::string(GetParent()->GetType()) == "Shaft")
				((Shaft*)GetParent()->GetRawObject())->RemoveTrigger(this);
			if (std::string(GetParent()->GetType()) == "Stairs")
				((Stairs*)GetParent()->GetRawObject())->RemoveTrigger(this);
			if (std::string(GetParent()->GetType()) == "SBS")
				sbs->RemoveTrigger(this);
		}
	}
}

void Trigger::Enabled(bool value)
{
	//enable or disable trigger

	if (IsEnabled == value)
		return;

	IsEnabled = value;
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
	sound->Loop(false);
	sound->Play();
}

int Trigger::GetSelectPosition()
{
	//return selection position
	return current_position;
}

const char* Trigger::GetPositionAction(int position)
{
	//return action name associated with the specified selection position

	if ((int)Actions.size() == 0)
		return "";

	std::vector<Action*> actionlist;
	actionlist = sbs->GetAction(Actions[position - 1]);

	//return command of first action in list
	if ((int)actionlist.size() > 0)
	{
		if (actionlist[0])
			return actionlist[0]->GetCommandName();
	}
	return "";
}

const char* Trigger::GetSelectPositionAction()
{
	//return action name associated with current selection position
	return GetPositionAction(current_position);
}

int Trigger::GetPositions()
{
	//return number of available positions, based on size of Actions array
	return (int)Actions.size();
}

int Trigger::FindActionPosition(const char *name)
{
	//returns the position number that the specified action name is associated with.
	//otherwise 0 if not found

	for (int i = 1; i <= GetPositions(); i++)
	{
		if (std::string(GetPositionAction(i)) == std::string(name))
			return i;
	}

	return 0;
}

bool Trigger::DoAction()
{
	//perform trigger's action
	//result is true if at least one action in the list succeeded

	if ((int)Actions.size() == 0)
		return sbs->ReportError("No available actions for trigger '" + std::string(GetName()) + "'");

	std::vector<Action*> actionlist = sbs->GetAction(Actions[current_position - 1]);

	bool result = false;

	for (int i = 0; i < (int)actionlist.size(); i++)
	{
		bool result2 = false;

		if (actionlist[i])
			result2 = actionlist[i]->DoAction(this);

		if (result2 == true)
			result = true;
	}
	return result;
}

bool Trigger::Check()
{
	//check for action; should be called in a loop by the parent object

	Ogre::Vector3 cam = sbs->camera->GetPosition();
	bool changed = false;
	if (cam > (GetPosition() + area_min) && cam < (GetPosition() + area_max))
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
				sbs->Report("Inside trigger area '" + std::string(GetName()) + "', parent '" + std::string(GetParent()->GetName()) + "'");
			else
				sbs->Report("Outside trigger area '" + std::string(GetName()) + "', parent '" + std::string(GetParent()->GetName()) + "'");
		}

		//get action name of next position state
		std::string name = GetPositionAction(GetNextSelectPosition());

		//exit without changing position if floor button is currently selected
		if (name == "off" && IsNumeric(GetSelectPositionAction()) == true)
			return false;

		//change to next control position
		NextSelectPosition();

		//perform selected action
		bool result = DoAction();

		//play sound if action succeeded
		//or always if the name is numeric (elevator is on the same floor, and doors are reopened)
		if (result == true || IsNumeric(name.c_str()))
			PlaySound();

		//change back to original selection if result is false
		if (result == false)
			PreviousSelectPosition();

		return result;
	}
	return false;
}

bool Trigger::IsInside()
{
	//return inside status

	return is_inside;
}
