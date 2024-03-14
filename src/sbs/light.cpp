/*
	Scalable Building Simulator - Light Object
	The Skyscraper Project - Version 1.12 Alpha
	Copyright (C)2004-2024 Ryan Thoryk
	https://www.skyscrapersim.net
	https://sourceforge.net/projects/skyscraper/
	Contact - ryan@skyscrapersim.net

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

#include <OgreSceneManager.h>
#include <OgreLight.h>
#include "globals.h"
#include "sbs.h"
#include "floor.h"
#include "elevatorcar.h"
#include "shaft.h"
#include "stairs.h"
#include "scenenode.h"
#include "light.h"

namespace SBS {

Light::Light(Object *parent, const std::string &name, int type) : Object(parent)
{
	//creates a light object

	//types are:
	//0 - point light
	//1 - directional light
	//2 - spotlight

	//set up SBS object
	SetValues("Light", name, false);

	Type = type;

	try
	{
		light = sbs->mSceneManager->createLight(GetSceneNode()->GetFullName());
		GetSceneNode()->AttachObject(light);

		if (type == 0)
			light->setType(Ogre::Light::LT_POINT);
		if (type == 1)
			light->setType(Ogre::Light::LT_DIRECTIONAL);
		if (type == 2)
			light->setType(Ogre::Light::LT_SPOTLIGHT);

		SetRenderingDistance(100);
	}
	catch (Ogre::Exception &e)
	{
		ReportError("Error creating light:\n" + e.getDescription());
	}

	std::string type_name = "point";
	if (type == 1)
		type_name = "directional";
	if (type == 2)
		type_name = "spotlight";

	if (sbs->Verbose == true)
		Report("Light '" + name + "' created as type " + type_name);
}

Light::~Light()
{
	if (light)
		GetSceneNode()->DetachObject(light);
	sbs->mSceneManager->destroyLight(GetSceneNode()->GetFullName());

	//unregister from parent
	if (sbs->FastDelete == false && parent_deleting == false)
	{
		std::string type = GetParent()->GetType();

		if (type == "ElevatorCar")
			static_cast<ElevatorCar*>(GetParent())->RemoveLight(this);
		else if (type == "Floor")
			static_cast<Floor*>(GetParent())->RemoveLight(this);
		else if (type == "Shaft Level")
			static_cast<Shaft::Level*>(GetParent())->RemoveLight(this);
		else if (type == "Stairwell Level")
			static_cast<Stairwell::Level*>(GetParent())->RemoveLight(this);
		else if (type == "SBS")
			sbs->RemoveLight(this);
	}
}

void Light::SetColor(Real color_r, Real color_g, Real color_b)
{
	//set color of light
	light->setDiffuseColour(color_r, color_g, color_b);
}

void Light::SetSpecularColor(Real color_r, Real color_g, Real color_b)
{
	light->setSpecularColour(color_r, color_g, color_b);
}

void Light::SetAttenuation(Real att_range, Real att_constant, Real att_linear, Real att_quadratic)
{
	light->setAttenuation(sbs->ToRemote(att_range), att_constant, att_linear, att_quadratic);
}

void Light::SetSpotlightRange(Real spot_inner_angle, Real spot_outer_angle, Real spot_falloff)
{
	if (Type == 2)
		light->setSpotlightRange(Degree(spot_inner_angle), Degree(spot_outer_angle), spot_falloff);
}

void Light::SetDirection(const Vector3 &direction)
{
	GetSceneNode()->SetDirection(direction);
}

void Light::SetRenderingDistance(Real distance)
{
	light->setRenderingDistance(sbs->ToRemote(distance));
}

void Light::Enabled(bool value)
{
	light->setVisible(value);
}

bool Light::IsEnabled()
{
	return light->getVisible();
}

}
