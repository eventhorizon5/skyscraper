/* $Id$ */

/*
	Scalable Building Simulator - Control Object Class
	The Skyscraper Project - Version 1.8 Alpha
	Copyright (C)2004-2010 Ryan Thoryk
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
#include "control.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

Control::Control(Object *parent, const char *name, const char *sound_file, std::vector<Ogre::String> &action_names, std::vector<Ogre::String> &textures, const char *direction, float width, float height, float voffset)
{
	//create a control at the specified location

	//the parent object is responsible for control actions via a click callback.
	//action_name is simply a second name used to determine the control's action, instead of
	//needing it embedded in the mesh object
	//'texture' is the texture for the first selection position

	//set up SBS object
	object = new Object();
	object->SetValues(this, parent, "Control", name, false);

	Ogre::String objnum = Ogre::StringConverter::toString(object->GetNumber());
	Name = "(" + objnum + ")" + name;
	Actions = action_names;
	Direction = direction;
	IsEnabled = true;
	TextureArray = textures;
	current_position = 1;

	//create object mesh
	ControlMesh = new MeshObject(object, name, 0, sbs->GetConfigFloat("Skyscraper.SBS.MaxSmallRenderDistance", 100));

	if (Direction == "front" || Direction == "back")
	{
		if (Direction == "front")
			sbs->DrawWalls(true, false, false, false, false, false);
		else
			sbs->DrawWalls(false, true, false, false, false, false);

		sbs->AddWallMain(object, ControlMesh, "Floor Indicator", textures[0].c_str(), 0, 0, 0, width, 0, height, height, voffset, voffset, 1, 1, false);
	}
	if (Direction == "left" || Direction == "right")
	{
		if (Direction == "left")
			sbs->DrawWalls(true, false, false, false, false, false);
		else
			sbs->DrawWalls(false, true, false, false, false, false);

		sbs->AddWallMain(object, ControlMesh, "Floor Indicator", textures[0].c_str(), 0, 0, 0, 0, width, height, height, voffset, voffset, 1, 1, false);
	}
	sbs->ResetWalls();

	//create sound object
	sound = new Sound(this->object, "Control", true);
	sound->Load(sound_file);
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
		if (object->parent_deleting == false)
		{
			if (Ogre::String(object->GetParent()->GetType()) == "ButtonPanel")
				((ButtonPanel*)object->GetParent()->GetRawObject())->RemoveControl(this);
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

	if (ControlMesh->ChangeTexture(TextureArray[position - 1].c_str()).get())
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

	current_position = GetNextSelectPosition();

	if (check_state == true)
		return SetSelectPosition(current_position);
	else
		return ChangeSelectPosition(current_position);
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

	current_position = GetPreviousSelectPosition();

	if (check_state == true)
		return SetSelectPosition(current_position);
	else
		return ChangeSelectPosition(current_position);
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
	//return action name associated with the specified selection position
	return Actions[position - 1].c_str();
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
	return (int)Actions.size();
}

int Control::FindActionPosition(const char *name)
{
	//returns the position number that the specified action name is associated with.
	//otherwise 0 if not found

	for (int i = 1; i <= GetPositions(); i++)
	{
		if (Ogre::String(GetPositionAction(i)) == Ogre::String(name))
			return i;
	}

	return 0;
}
