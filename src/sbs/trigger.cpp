/* $Id$ */

/*
	Scalable Building Simulator - Proximity Trigger Class
	The Skyscraper Project - Version 1.8 Alpha
	Copyright (C)2004-2012 Ryan Thoryk
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

Trigger::Trigger(Object *parent, const char *name, const char *sound_file, Object *action_parent, std::vector<std::string> &action_names, std::vector<std::vector<std::string> > &action_parameters, Ogre::Vector3 &_area_min, Ogre::Vector3 &_area_max)
{
	//create a proximity trigger at the specified location

	//set up SBS object
	object = new Object();
	object->SetValues(this, parent, "Trigger", name, false);

	std::string objnum = Ogre::StringConverter::toString(object->GetNumber());
	//Name = "(" + objnum + ")" + name;
	Name = name;

	for (int i = 0; i < action_names.size(); i++)
	{
		Action *action;
		if (action_parameters.size() > 0)
			action = new Action(action_parent, action_names[i], action_parameters[i]);
		else
			action = new Action(action_parent, action_names[i]);
		Actions.push_back(action);
	}

	IsEnabled = true;
	current_position = 1;
	area_min = _area_min;
	area_max = _area_max;
	IsInside = false;

	//create sound object
	sound = new Sound(this->object, "Trigger", true);
	sound->Load(sound_file);
}

Trigger::~Trigger()
{
	if (sound)
	{
		sound->object->parent_deleting = true;
		delete sound;
	}
	sound = 0;

	for (int i = 0; i < Actions.size(); i++)
	{
		if (Actions[i])
			delete Actions[i];
		Actions[i] = 0;
	}

	//unregister from parent
	/*if (sbs->FastDelete == false)
	{
		if (object->parent_deleting == false)
		{
			if (std::string(object->GetParent()->GetType()) == "ButtonPanel")
				((ButtonPanel*)object->GetParent()->GetRawObject())->RemoveControl(this);
		}
	}*/

	delete object;
}

void Trigger::Enabled(bool value)
{
	//enable or disable trigger

	if (IsEnabled == value)
		return;

	IsEnabled = value;
}

Ogre::Vector3 Trigger::GetPosition()
{
	//return current position
	return pos;
}

void Trigger::SetPosition(const Ogre::Vector3 &position)
{
	//set position
	sound->SetPosition(position);
	pos = position;
}

void Trigger::SetPositionY(float position)
{
	//set Y position
	Ogre::Vector3 tmppos = GetPosition();
	tmppos.y = position;
	SetPosition(tmppos);
}

void Trigger::Move(const Ogre::Vector3 &position)
{
	//relative movement
	pos += position;

	//move sound
	sound->SetPosition(pos);
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

	current_position = GetNextSelectPosition();

	if (check_state == true)
		return SetSelectPosition(current_position);
	else
		return ChangeSelectPosition(current_position);
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

	current_position = GetPreviousSelectPosition();

	if (check_state == true)
		return SetSelectPosition(current_position);
	else
		return ChangeSelectPosition(current_position);
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
	return Actions[position - 1]->GetName();
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
	return Actions[current_position - 1]->DoAction();
}

bool Trigger::Check()
{
	//check for action; should be called in a loop by the parent object

	if (IsInside == false)
	{
		Ogre::Vector3 pos = sbs->camera->GetPosition();
		if (pos > area_min && pos < area_max)
		{
			//camera is inside trigger area
			IsInside = true;

			//play sound
			PlaySound();

			//get action name of next position state
			std::string name = GetPositionAction(GetNextSelectPosition());

			//exit without changing position if floor button is currently selected
			if (name == "off" && IsNumeric(GetSelectPositionAction()) == true)
				return false;

			//change to next control position
			NextSelectPosition();

			//perform selected action
			return DoAction();
		}
		else
			IsInside = false;
	}
	return false;
}
