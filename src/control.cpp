/* $Id$ */

/*
	Scalable Building Simulator - Control Object Class
	The Skyscraper Project - Version 1.6 Alpha
	Copyright (C)2005-2009 Ryan Thoryk
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
#include "control.h"
#include "sbs.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

Control::Control(int type, const char *name, const char *action_name, const char *texture, const char *texture_lit, const char *direction, float x, float z, float width, float height, float voffset)
{
	//create a control at the specified location
	//type is either:
	//1: button
	//2: switch
	//3: knob
	//the parent object is responsible for control actions via a click callback.
	//action_name is simply a second name used to determine the control's action, instead of
	//needing it embedded in the mesh object

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
		sbs->AddGenWall(ControlMesh, texture, x, z, x + width, z, height, voffset, 1, 1);
	if (Direction == "back")
		sbs->AddGenWall(ControlMesh, texture, x, z, x - width, z, height, voffset, 1, 1);
	if (Direction == "left")
		sbs->AddGenWall(ControlMesh, texture, x, z, x, z - width, height, voffset, 1, 1);
	if (Direction == "right")
		sbs->AddGenWall(ControlMesh, texture, x, z, x, z + width, height, voffset, 1, 1);
		
}

Control::~Control()
{
	ControlMesh = 0;
}

void Control::Enabled(bool value)
{
	//enable or disable control
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
	if (IsEnabled == false)
		return;

	//set light status
	SetLight(value);
}

csVector3 Control::GetPosition()
{
	//return current position
	return ControlMesh->GetMovable()->GetPosition();
}

void Control::SetPosition(const csVector3 &position)
{
	//set control position
	ControlMesh->GetMovable()->SetPosition(position);
	ControlMesh->GetMovable()->UpdateMove();
}

void Control::SetPositionY(float position)
{
	//set control position
	csVector3 pos = ControlMesh->GetMovable()->GetPosition();
	ControlMesh->GetMovable()->SetPosition(csVector3(pos.x, position, pos.z));
	ControlMesh->GetMovable()->UpdateMove();
}

void Control::Move(const csVector3 &position)
{
	//relative movement
	ControlMesh->GetMovable()->MovePosition(position);
	ControlMesh->GetMovable()->UpdateMove();
}

void Control::SetKnobPosition(int position)
{
	//knob position change
}
