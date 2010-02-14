/* $Id$ */

/*
	Scalable Building Simulator - Control Object Class
	The Skyscraper Project - Version 1.6 Alpha
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

#include <iengine/movable.h>
#include <cstool/genmeshbuilder.h>

extern SBS *sbs; //external pointer to the SBS engine

Control::Control(Object *parent, int type, const char *name, const char *action_name, const char *sound_file, const char *texture, const char *texture_lit, const char *direction, float width, float height, float voffset)
{
	//create a control at the specified location
	//type is either:
	//1: button
	//2: switch
	//3: knob
	//the parent object is responsible for control actions via a click callback.
	//action_name is simply a second name used to determine the control's action, instead of
	//needing it embedded in the mesh object

	//set up SBS object
	object = new Object();
	object->SetValues(this, parent, "Control", false);

	if (type == 1)
	{
		Type = "Button";
		positions = 2;
	}
	if (type == 2)
	{
		Type = "Switch";
		positions = 2;
	}
	if (type == 3)
	{
		Type = "Knob";
		positions = 3;
	}
	Name = name;
	ActionName = action_name;
	Direction = direction;
	TextureUnlit = texture;
	TextureLit = texture_lit;
	LightStatus = false;
	IsEnabled = true;

	//create object mesh
	ControlMesh = CS::Geometry::GeneralMeshBuilder::CreateFactoryAndMesh(sbs->engine, sbs->area, Name, Name + " factory");
	ControlMesh->SetZBufMode(CS_ZBUF_USE);
	ControlMesh->SetRenderPriority(sbs->engine->GetObjectRenderPriority());
	csRef<iMaterialWrapper> mat = sbs->engine->GetMaterialList()->FindByName(texture);
	ControlMesh->GetMeshObject()->SetMaterialWrapper(mat);

	if (Direction == "front")
		sbs->AddGenWall(ControlMesh, texture, 0, 0, width, 0, height, voffset, 1, 1);
	if (Direction == "back")
		sbs->AddGenWall(ControlMesh, texture, 0, 0, -width, 0, height, voffset, 1, 1);
	if (Direction == "left")
		sbs->AddGenWall(ControlMesh, texture, 0, 0, 0, -width, height, voffset, 1, 1);
	if (Direction == "right")
		sbs->AddGenWall(ControlMesh, texture, 0, 0, 0, width, height, voffset, 1, 1);

	//create sound object
	sound = new Sound(this->object, "Control");
	sound->Load(sound_file);
}

Control::~Control()
{
	if (sound)
		delete sound;
	sound = 0;
	ControlMesh = 0;
	delete object;
}

void Control::Enabled(bool value)
{
	//enable or disable control

	if (IsEnabled == value)
		return;

	sbs->EnableMesh(ControlMesh, value);
	IsEnabled = value;
}

void Control::SetLight(bool value)
{
	//set light without checking state
	LightStatus = value;

	if (value == true)
		sbs->ChangeTexture(ControlMesh, TextureLit);
	else
		sbs->ChangeTexture(ControlMesh, TextureUnlit);
}

void Control::ChangeLight(bool value)
{
	//change light status

	if (value == LightStatus)
		return;

	//exit if control is disabled
//	if (IsEnabled == false)
//		return;

	//set light status
	SetLight(value);
}

csVector3 Control::GetPosition()
{
	//return current position
	return sbs->ToLocal(ControlMesh->GetMovable()->GetPosition());
}

void Control::SetPosition(const csVector3 &position)
{
	//set control position
	sound->SetPosition(position);
	ControlMesh->GetMovable()->SetPosition(sbs->ToRemote(position));
	ControlMesh->GetMovable()->UpdateMove();
}

void Control::SetPositionY(float position)
{
	//set control position
	csVector3 pos = GetPosition();
	pos.y = position;
	SetPosition(pos);
}

void Control::Move(const csVector3 &position)
{
	//relative movement
	ControlMesh->GetMovable()->MovePosition(sbs->ToRemote(position));
	ControlMesh->GetMovable()->UpdateMove();

	//move sound
	sound->SetPosition(GetPosition());
}

void Control::SetKnobPosition(int position)
{
	//knob position change
}

void Control::PlaySound()
{
	//play associated button sound
	sound->Loop(false);
	sound->Play();
}
