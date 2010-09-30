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

Light::Light(const char *name, int type, csVector3 position, csVector3 direction, float radius, float max_distance, float color_r, float color_g, float color_b, float spec_color_r, float spec_color_g, float spec_color_b, bool dynamic_color, bool movable)
{
	//creates a light object

	//types are:
	//0 - point light
	//1 - directional light
	//2 - spotlight

	//if point light is used, direction and radius are ignored
	//if directional light is used, position is ignored
	//if spotlight is used, radius is ignored

	//dynamic_color determines if the light's color can change during runtime
	//movable determines if the light can move - if this is true, dynamic_color will be forced as true
	
	Type = type;
	Name = name;

	csLightDynamicType light_type = CS_LIGHT_DYNAMICTYPE_STATIC;
	if (dynamic_color == true)
		light_type = CS_LIGHT_DYNAMICTYPE_PSEUDO;
	if (movable == true)
		light_type = CS_LIGHT_DYNAMICTYPE_DYNAMIC;

	light = sbs->engine->CreateLight(name, sbs->ToRemote(position), radius, csColor(color_r, color_g, color_b), light_type);

	light->SetSpecularColor(csColor(spec_color_r, spec_color_g, spec_color_b));
	light->SetCutoffDistance(sbs->ToRemote(max_distance));

	if (type == 0)
		light->SetType(CS_LIGHT_POINTLIGHT);
	if (type == 1)
		light->SetType(CS_LIGHT_DIRECTIONAL);
	if (type == 2)
		light->SetType(CS_LIGHT_SPOTLIGHT);

	//add light to world
	sbs->area->GetLights()->Add(light);

	//setup dynamic light
	if (light_type == CS_LIGHT_DYNAMICTYPE_DYNAMIC)
		light->Setup();
}

Light::~Light()
{
	if (sbs->FastDelete == false)
		sbs->area->GetLights()->Remove(light);
}

void Light::SetDirectionalCutoffRadius(float radius)
{
	light->SetDirectionalCutoffRadius(radius);
}

void Light::SetSpotLightFalloff(float inner, float outer)
{
	light->SetSpotLightFalloff(inner, outer);
}
