/* $Id$ */

/*
	Scalable Building Simulator - Control Object
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

#include "globals.h"
#include "sbs.h"
#include "action.h"
#include "floor.h"
#include "elevatorcar.h"
#include "shaft.h"
#include "stairs.h"
#include "buttonpanel.h"
#include "texture.h"
#include "sound.h"
#include "mesh.h"
#include "control.h"

namespace SBS {

Control::Control(Object *parent, const std::string &name, bool permanent, const std::string &sound_file, const std::vector<std::string> &action_names, const std::vector<Action*> &actions, std::vector<std::string> &textures, const std::string &direction, float width, float height, bool center, int selection_position) : Object(parent)
{
	//create a control at the specified location

	//actions can either be given as a name list (dynamic action lists) or pointer list (static action lists) - don't use both

	//set up SBS object
	SetValues("Control", name, permanent);

	std::string name2 = name;
	if (name.find("Control", 0) == std::string::npos)
		name2 = "Control " + name;

	ActionNames = action_names;
	Actions = actions;
	Direction = direction;
	is_enabled = true;
	TextureArray = textures;
	Locked = false;
	KeyID = 0;
	light_status = false;
	sound = 0;
	action_hold = false;

	//create object mesh
	ControlMesh = new MeshObject(this, name2, 0, "", sbs->GetConfigFloat("Skyscraper.SBS.MaxSmallRenderDistance", 100));

	//set selection position to starting value
	SetSelectPosition(selection_position);

	std::string texture = GetTexture(selection_position);

	sbs->TexelOverride = true;
	WallObject *wall;
	if (Direction == "front")
	{
		float x = 0, y = width;
		if (center == true)
		{
			x = -width / 2;
			y = width / 2;
		}
		sbs->DrawWalls(true, false, false, false, false, false);
		wall = ControlMesh->CreateWallObject(name);
		sbs->AddWallMain(wall, name, texture, 0, x, 0, y, 0, height, height, 0, 0, 1, 1, false);
	}
	if (Direction == "back")
	{
		float x = 0, y = -width;
		if (center == true)
		{
			x = width / 2;
			y = -width / 2;
		}
		sbs->DrawWalls(false, true, false, false, false, false);
		wall = ControlMesh->CreateWallObject(name);
		sbs->AddWallMain(wall, name, texture, 0, x, 0, y, 0, height, height, 0, 0, 1, 1, false);
	}
	if (Direction == "left")
	{
		float x = 0, y = -width;
		if (center == true)
		{
			x = width / 2;
			y = -width / 2;
		}
		sbs->DrawWalls(true, false, false, false, false, false);
		wall = ControlMesh->CreateWallObject(name);
		sbs->AddWallMain(wall, name, texture, 0, 0, x, 0, y, height, height, 0, 0, 1, 1, false);
	}
	if (Direction == "right")
	{
		float x = 0, y = width;
		if (center == true)
		{
			x = -width / 2;
			y = width / 2;
		}
		sbs->DrawWalls(false, true, false, false, false, false);
		wall = ControlMesh->CreateWallObject(name);
		sbs->AddWallMain(wall, name, texture, 0, 0, x, 0, y, height, height, 0, 0, 1, 1, false);
	}
	sbs->ResetWalls();
	sbs->TexelOverride = false;

	//create sound object
	if (sound_file != "")
	{
		sound = new Sound(this, name, true);
		sound->Load(sound_file);
	}

	//register control
	sbs->RegisterControl(this);
}

Control::~Control()
{
	if (sound)
	{
		sound->parent_deleting = true;
		delete sound;
	}
	sound = 0;

	if (ControlMesh)
	{
		ControlMesh->parent_deleting = true;
		delete ControlMesh;
	}
	ControlMesh = 0;

	//unregister from parent
	if (sbs->FastDelete == false)
	{
		//unregister control
		sbs->UnregisterControl(this);

		//unregister from parent
		if (parent_deleting == false)
		{
			std::string type = GetParent()->GetType();

			if (type == "ButtonPanel")
				static_cast<ButtonPanel*>(GetParent())->RemoveControl(this);
			else if (type == "ElevatorCar")
				static_cast<ElevatorCar*>(GetParent())->RemoveControl(this);
			else if (type == "Floor")
				static_cast<Floor*>(GetParent())->RemoveControl(this);
			else if (type == "Shaft")
				static_cast<Shaft*>(GetParent())->RemoveControl(this);
			else if (type == "Stairs")
				static_cast<Stairs*>(GetParent())->RemoveControl(this);
			else if (type == "SBS")
				sbs->RemoveControl(this);
		}
	}
}

void Control::Enabled(bool value)
{
	//enable or disable control

	if (is_enabled == value)
		return;

	ControlMesh->Enable(value);
	is_enabled = value;
}

bool Control::SetSelectPosition(int position)
{
	//set selection position without checking state

	if (position < 1 || position > GetPositions())
		return false;

	current_position = position;

	return ControlMesh->ChangeTexture(GetTexture(position));
}

bool Control::ChangeSelectPosition(int position)
{
	//set selection position

	if (position == current_position)
		return false;

	return SetSelectPosition(position);
}

bool Control::NextSelectPosition(bool check_state)
{
	//change to the next available control selection position

	int position = GetNextSelectPosition();

	if (check_state == false)
		return SetSelectPosition(position);
	else
		return ChangeSelectPosition(position);
}

int Control::GetNextSelectPosition()
{
	//determine next selection position
	int pos = current_position;
	pos++;
	if (pos > GetPositions())
		pos = 1;
	return pos;
}

bool Control::PreviousSelectPosition(bool check_state)
{
	//change to the next available control selection position

	int position = GetPreviousSelectPosition();

	if (check_state == false)
		return SetSelectPosition(position);
	else
		return ChangeSelectPosition(position);
}

int Control::GetPreviousSelectPosition()
{
	//determine previous selection position
	int pos = current_position;
	pos--;
	if (pos < 1)
		pos = GetPositions();
	return pos;
}

void Control::PlaySound()
{
	//play associated button sound

	if (sound)
	{
		sound->SetLoopState(false);
		sound->Play();
	}
}

int Control::GetSelectPosition()
{
	//return control selection position
	return current_position;
}

std::string Control::GetPositionAction(int position)
{
	//return action's command name associated with the specified selection position

	std::vector<Action*> actionlist;

	if (ActionNames.empty() == false)
		actionlist = sbs->GetAction(ActionNames[position - 1]);
	else if (Actions.empty() == false)
		actionlist.push_back(Actions[position - 1]);
	else
		return "";

	//return command of first action in list
	if (actionlist.empty() == false)
	{
		if (actionlist[0])
			return actionlist[0]->GetCommandName();
	}
	return "";
}

std::string Control::GetSelectPositionAction()
{
	//return action name associated with current selection position
	return GetPositionAction(current_position);
}

void Control::SetTexture(int position, const std::string &texture)
{
	//bind a texture to a specific position

	if (position < 1 || position > GetPositions())
		return;

	TextureArray[position - 1] = texture;
}

std::string Control::GetTexture(int position)
{
	//get texture for specified position

	if (position < 1 || position > GetPositions())
		return "";

	return TextureArray[position - 1];

}

int Control::GetPositions()
{
	//return number of available positions, based on size of Actions array
	if (ActionNames.empty() == false)
		return (int)ActionNames.size();
	else
		return (int)Actions.size();
}

int Control::FindActionPosition(const std::string &name)
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

int Control::FindNumericActionPosition()
{
	//returns the position number that the first numeric action
	//otherwise 0 if not found

	for (int i = 1; i <= GetPositions(); i++)
	{
		if (IsNumeric(GetPositionAction(i)))
			return i;
	}

	return 0;
}

bool Control::DoAction()
{
	//perform object's action
	//result is true if at least one action in the list succeeded

	std::vector<Action*> actionlist;

	if (ActionNames.empty() == false)
		actionlist = sbs->GetAction(ActionNames[current_position - 1]);
	else if (Actions.empty() == false)
		actionlist.push_back(Actions[current_position - 1]);
	else
		return sbs->ReportError("No available actions for control '" + GetName() + "'");

	bool result = false;
	for (size_t i = 0; i < actionlist.size(); i++)
	{
		bool result2 = false;

		if (actionlist[i])
			result2 = actionlist[i]->DoAction(this, action_hold);

		if (result2 == true)
			result = true;
	}
	return result;
}

bool Control::Press(bool reverse)
{
	//press button/control
	//if reverse is true, select in opposite direction

	//check lock state
	if (IsLocked() == true)
		return sbs->ReportError("Control '" + GetName() + "' is locked");

	//get action name of next position state
	std::string name;
	if (reverse == false)
		name = GetPositionAction(GetNextSelectPosition());
	else
		name = GetPositionAction(GetPreviousSelectPosition());

	//exit without changing position if floor button is currently selected
	if (name == "off" && IsNumeric(GetSelectPositionAction()) == true)
		return true;

	//change to next control position
	if (reverse == false)
		NextSelectPosition();
	else
		PreviousSelectPosition();

	//run action
	bool result = DoAction();

	//play sound if action succeeded
	//or always if the name is numeric (elevator is on the same floor, and doors are reopened)
	if (result == true || IsNumeric(name))
		PlaySound();

	//change back to original selection if result is false
	if (result == false)
	{
		if (reverse == false)
			PreviousSelectPosition();
		else
			NextSelectPosition();
	}

	return result;
}

void Control::ChangeFloorLight(int floor, bool value)
{
	//change light status on control if it calls the specified floor

	if (light_status == value)
		return;

	std::string floornum = ToString(floor);
	int index = FindActionPosition(floornum);
	int index2 = FindActionPosition("off");
	if (index > 0 && index2 > 0)
	{
		if (value == false)
			ChangeSelectPosition(index2);
		else
			ChangeSelectPosition(index);

		light_status = value;
	}
}

void Control::ChangeLight(bool value)
{
	//change light status on control

	if (light_status == value)
		return;

	int index = FindNumericActionPosition();
	int index2 = FindActionPosition("off");
	if (index > 0 && index2 > 0)
	{
		if (value == false)
			ChangeSelectPosition(index2);
		else
			ChangeSelectPosition(index);
	}
	light_status = value;
}

void Control::SetLocked(bool value, int keyid)
{
	//set locked state
	Locked = value;
	KeyID = keyid;
}

bool Control::ToggleLock(bool force)
{
	//toggle lock state
	//if force is true, bypass key check

	//quit if user doesn't have key, if force is false
	if (KeyID != 0)
	{
		if (sbs->CheckKey(KeyID) == false && force == false)
			return sbs->ReportError("Need key " + ToString(KeyID) + " to lock/unlock control '" + GetName() + "'");
	}

	Locked = !Locked;

	if (Locked == true)
		sbs->Report("Locked control '" + GetName() + "'");
	else
		sbs->Report("Unlocked control '" + GetName() + "'");

	return true;
}

bool Control::IsLocked()
{
	return Locked;
}

int Control::GetKeyID()
{
	return KeyID;
}

void Control::RemoveAction(Action *action)
{
	for (size_t i = 0; i < Actions.size(); i++)
	{
		if (Actions[i] == action)
		{
			Actions.erase(Actions.begin() + i);
			return;
		}
	}
}

void Control::OnClick(Ogre::Vector3 &position, bool shift, bool ctrl, bool alt, bool right)
{
	//toggle lock status if ctrl and shift are pressed
	if (ctrl == true && shift == true)
		ToggleLock();
	else
		Press(right);
}

void Control::OnUnclick(bool right)
{
	if (action_hold == false)
		return;

	if (right == false)
		PreviousSelectPosition();
	else
		NextSelectPosition();

	action_hold = false;
}

}
