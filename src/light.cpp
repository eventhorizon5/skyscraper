/* $Id$ */

/*
	Scalable Building Simulator - Light Class
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
#include "unix.h"
#include "light.h"

extern SBS *sbs; //external pointer to the SBS engine

Light::Light(const char *name, int type, Ogre::Vector3 position, Ogre::Vector3 rotation, float radius, float max_distance, float color_r, float color_g, float color_b, float spec_color_r, float spec_color_g, float spec_color_b, float directional_cutoff_radius, float spot_falloff_inner, float spot_falloff_outer)
{
	//creates a light object

	//types are:
	//0 - point light
	//1 - directional light
	//2 - spotlight

	//if point light is used, direction and radius are ignored
	//if directional light is used, position is ignored
	//if spotlight is used, radius is ignored

	//set up SBS object
	object = new Object();
	object->SetValues(this, sbs->object, "Light", name, false);

	Type = type;
	Name = name;
	Origin = position;

	/*light = sbs->engine->CreateLight(name, sbs->ToRemote(position), sbs->ToRemote(radius), csColor(color_r, color_g, color_b), CS_LIGHT_DYNAMICTYPE_DYNAMIC);

	if (type == 0)
		light->SetType(CS_LIGHT_POINTLIGHT);
	if (type == 1)
		light->SetType(CS_LIGHT_DIRECTIONAL);
	if (type == 2)
		light->SetType(CS_LIGHT_SPOTLIGHT);

	light->SetSpecularColor(csColor(spec_color_r, spec_color_g, spec_color_b));

	if (type == 1)
		light->SetDirectionalCutoffRadius(directional_cutoff_radius);
	if (type == 2)
		light->SetSpotLightFalloff(spot_falloff_inner, spot_falloff_outer);*/

	sbs->AddLightHandle(this);

	SetRotation(rotation);

	//add light to world
	//sbs->area->GetLights()->Add(light);
}

Light::~Light()
{
	if (sbs->FastDelete == false)
	{
		//sbs->area->GetLights()->Remove(light);
		sbs->DeleteLightHandle(this);
	}
	delete object;
}

void Light::Prepare()
{
	//prepare light for use
}

void Light::Move(const Ogre::Vector3 position, bool relative_x, bool relative_y, bool relative_z)
{
	//move light - this can only be done on movable lights
	/*Ogre::Vector3 pos;
	if (relative_x == false)
		pos.x = sbs->ToRemote(Origin.x + position.x);
	else
		pos.x = light->GetCenter().x + sbs->ToRemote(position.x);
	if (relative_y == false)
		pos.y = sbs->ToRemote(Origin.y + position.y);
	else
		pos.y = light->GetCenter().y + sbs->ToRemote(position.y);
	if (relative_z == false)
		pos.z = sbs->ToRemote(Origin.z + position.z);
	else
		pos.z = light->GetCenter().z + sbs->ToRemote(position.z);
	light->GetMovable()->SetPosition(pos);
	light->GetMovable()->UpdateMove();*/
}

Ogre::Vector3 Light::GetPosition()
{
	//return sbs->ToLocal(light->GetMovable()->GetPosition());
	return Ogre::Vector3(0, 0, 0);
}

void Light::SetColor(float color_r, float color_g, float color_b, float spec_color_r, float spec_color_g, float spec_color_b)
{
	//set color of light

	//light->SetColor(csColor(color_r, color_g, color_b));
	//light->SetSpecularColor(csColor(spec_color_r, spec_color_g, spec_color_b));
}

void Light::SetRotation(const Ogre::Vector3& rotation)
{
	//rotate light
	/*Ogre::Matrix3 rot = csXRotMatrix3(rotation.x) * csYRotMatrix3(rotation.y) * csZRotMatrix3(rotation.z);
	csOrthoTransform ot (rot, light->GetMovable()->GetTransform().GetOrigin());
	light->GetMovable()->SetTransform(ot);
	rotX = rotation.x;
	rotY = rotation.y;
	rotZ = rotation.z;
	light->GetMovable()->UpdateMove();*/
}

void Light::Rotate(const Ogre::Vector3& rotation, float speed)
{
	//rotates light in a relative amount
	rotX += rotation.x * speed;
	rotY += rotation.y * speed;
	rotZ += rotation.z * speed;
	SetRotation(Ogre::Vector3(rotX, rotY, rotZ));
}

Ogre::Vector3 Light::GetRotation()
{
	return Ogre::Vector3(rotX, rotY, rotZ);
}
