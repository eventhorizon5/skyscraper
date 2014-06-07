/* $Id$ */

/*
	Scalable Building Simulator - Control Object Class
	The Skyscraper Project - Version 1.9 Alpha
	Copyright (C)2004-2014 Ryan Thoryk
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

#include <OgreMaterial.h>
#include "globals.h"
#include "sbs.h"
#include "control.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

Control::Control(Object *parent, const char *name, const char *sound_file, const std::vector<std::string> &action_names, const std::vector<Action*> &actions, std::vector<std::string> &textures, const char *direction, float width, float height, float voffset, bool center)
{
	//create a control at the specified location

	//actions can either be given as a name list (dynamic action lists) or pointer list (static action lists) - don't use both

	//set up SBS object
	object = new Object();
	object->SetValues(this, parent, "Control", name, false);

	std::string objnum = ToString(object->GetNumber());
	//Name = "(" + objnum + ")" + name;
	Name = name;
	std::string Name2 = Name;
	if ((int)Name.find("Control", 0) == -1)
		Name2 = "Control " + Name;

	ActionNames = action_names;
	Actions = actions;
	Direction = direction;
	IsEnabled = true;
	TextureArray = textures;
	current_position = 1;
	Locked = false;
	KeyID = 0;
	light_status = false;

	//create object mesh
	ControlMesh = new MeshObject(object, Name2.c_str(), 0, sbs->GetConfigFloat("Skyscraper.SBS.MaxSmallRenderDistance", 100));

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
		wall = ControlMesh->CreateWallObject(object, Name.c_str());
		sbs->AddWallMain(wall, name, textures[0].c_str(), 0, x, 0, y, 0, height, height, voffset, voffset, 1, 1, false);
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
		wall = ControlMesh->CreateWallObject(object, Name.c_str());
		sbs->AddWallMain(wall, name, textures[0].c_str(), 0, x, 0, y, 0, height, height, voffset, voffset, 1, 1, false);
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
		wall = ControlMesh->CreateWallObject(object, Name.c_str());
		sbs->AddWallMain(wall, name, textures[0].c_str(), 0, 0, x, 0, y, height, height, voffset, voffset, 1, 1, false);
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
		wall = ControlMesh->CreateWallObject(object, Name.c_str());
		sbs->AddWallMain(wall, name, textures[0].c_str(), 0, 0, x, 0, y, height, height, voffset, voffset, 1, 1, false);
	}
	sbs->ResetWalls();
	sbs->TexelOverride = false;

	//create sound object
	sound = new Sound(object, name, true);
	sound->Load(sound_file);

	//register control
	sbs->RegisterControl(this);
}

Control::~Control()
{
	TextureArray.clear();
	if (sound)
	{
		sound->object->parent_deleting = true;
		delete sound;
	}
	sound = 0;

	if (ControlMesh)
		delete ControlMesh;
	ControlMesh = 0;

	//unregister from parent
	if (sbs->FastDelete == false)
	{
		//unregister control
		sbs->UnregisterControl(this);

		//unregister from parent
		if (object->parent_deleting == false)
		{
			if (std::string(object->GetParent()->GetType()) == "ButtonPanel")
				((ButtonPanel*)object->GetParent()->GetRawObject())->RemoveControl(this);
			if (std::string(object->GetParent()->GetType()) == "Elevator")
				((Elevator*)object->GetParent()->GetRawObject())->RemoveControl(this);
			if (std::string(object->GetParent()->GetType()) == "Floor")
				((Floor*)object->GetParent()->GetRawObject())->RemoveControl(this);
			if (std::string(object->GetParent()->GetType()) == "Shaft")
				((Shaft*)object->GetParent()->GetRawObject())->RemoveControl(this);
			if (std::string(object->GetParent()->GetType()) == "Stairs")
				((Stairs*)object->GetParent()->GetRawObject())->RemoveControl(this);
			if (std::string(object->GetParent()->GetType()) == "SBS")
				sbs->RemoveControl(this);
		}
	}

	delete object;
}

void Control::Enabled(bool value)
{
	//enable or disable control

	if (IsEnabled == value)
		return;

	ControlMesh->Enable(value);
	IsEnabled = value;
}

Ogre::Vector3 Control::GetPosition()
{
	//return current position
	return ControlMesh->GetPosition();
}

void Control::SetPosition(const Ogre::Vector3 &position)
{
	//set control position
	sound->SetPosition(position);
	ControlMesh->Move(position, false, false, false);
}

void Control::SetPositionY(float position)
{
	//set control position
	Ogre::Vector3 pos = GetPosition();
	pos.y = position;
	SetPosition(pos);
}

void Control::Move(const Ogre::Vector3 &position)
{
	//relative movement
	ControlMesh->Move(position, true, true, true);

	//move sound
	sound->SetPosition(GetPosition());
}

bool Control::SetSelectPosition(int position)
{
	//set selection position without checking state

	if (position < 1 || position > GetPositions())
		return false;

	current_position = position;

	if (ControlMesh->ChangeTexture(TextureArray[position - 1].c_str()) == true)
		return true;
	return false;
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

	if (check_state == true)
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

	if (check_state == true)
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
	sound->Loop(false);
	sound->Play();
}

int Control::GetSelectPosition()
{
	//return control selection position
	return current_position;
}

const char* Control::GetPositionAction(int position)
{
	//return action's command name associated with the specified selection position

	std::vector<Action*> actionlist;

	if (ActionNames.size() > 0)
		actionlist = sbs->GetAction(ActionNames[position - 1]);
	else
		actionlist.push_back(Actions[position - 1]);

	//return command of first action in list
	if (actionlist.size() > 0)
	{
		if (actionlist[0])
			return actionlist[0]->GetCommandName();
	}
	return "";
}

const char* Control::GetSelectPositionAction()
{
	//return action name associated with current selection position
	return GetPositionAction(current_position);
}

void Control::SetTexture(int position, const char *texture)
{
	//bind a texture to a specific position

	if (position < 1 || position > GetPositions())
		return;

	TextureArray[position - 1] = texture;
}

int Control::GetPositions()
{
	//return number of available positions, based on size of Actions array
	if (ActionNames.size() > 0)
		return (int)ActionNames.size();
	else
		return (int)Actions.size();
}

int Control::FindActionPosition(const char *name)
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

	if (ActionNames.size() > 0)
		actionlist = sbs->GetAction(ActionNames[current_position - 1]);
	else
		actionlist.push_back(Actions[current_position - 1]);

	bool result = false;
	for (int i = 0; i < (int)actionlist.size(); i++)
	{
		bool result2 = false;

		if (actionlist[i])
			result2 = actionlist[i]->DoAction();

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
		return sbs->ReportError(std::string("Control " + Name + " is locked").c_str());

	//get action name of next position state
	std::string name;
	if (reverse == false)
		name = GetPositionAction(GetNextSelectPosition());
	else
		name = GetPositionAction(GetPreviousSelectPosition());

	//exit without changing position if floor button is currently selected
	if (name == "off" && IsNumeric(GetSelectPositionAction()) == true)
		return false;

	//change to next control position
	if (reverse == false)
		NextSelectPosition();
	else
		PreviousSelectPosition();

	//run action
	bool result = DoAction();

	//play sound if action succeeded
	//or always if the name is numeric (elevator is on the same floor, and doors are reopened)
	if (result == true || IsNumeric(name.c_str()))
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
	int index = FindActionPosition(floornum.c_str());
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

void Control::Move(const Ogre::Vector3 position, bool relative_x, bool relative_y, bool relative_z)
{
	//relative movement
	ControlMesh->Move(position, relative_x, relative_y, relative_z);

	//move sound
	sound->SetPosition(GetPosition());
}

void Control::SetLocked(bool value, int keyid)
{
	//set locked state
	Locked = value;
	KeyID = keyid;
}

void Control::ToggleLock(bool force)
{
	//toggle lock state
	//if force is true, bypass key check

	bool replocked = false;

	//quit if user doesn't have key, if force is false
	if (KeyID != 0)
	{
		if (sbs->CheckKey(KeyID) == false && force == false)
		{
			std::string id = ToString(KeyID);
			sbs->Report(std::string("Need key " + id + " to lock/unlock control " + Name).c_str());
			return;
		}
	}

	Locked = !Locked;

	if (Locked == true)
		sbs->Report(std::string("Locked control " + Name).c_str());
	else
		sbs->Report(std::string("Unlocked control " + Name).c_str());
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
	for (int i = 0; i < (int)Actions.size(); i++)
	{
		if (Actions[i] == action)
			Actions.erase(Actions.begin() + i);
	}
}
