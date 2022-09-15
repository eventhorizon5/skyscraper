/* $Id: light.cpp 5263 2018-01-12 00:59:01Z ryan $ */

/*
	Scalable Building Simulator - Light Object
	The Skyscraper Project - Version 1.11 Alpha
	Copyright (C)2004-2018 Ryan Thoryk
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

Light::Light(Object *parent, const std::string &name, int type, Ogre::Vector3 position, Ogre::Vector3 direction, Real color_r, Real color_g, Real color_b, Real spec_color_r, Real spec_color_g, Real spec_color_b, Real spot_inner_angle, Real spot_outer_angle, Real spot_falloff, Real att_range, Real att_constant, Real att_linear, Real att_quadratic) : Object(parent)
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
		light = sbs->mSceneManager->createLight(name);
		GetSceneNode()->AttachObject(light);

		if (type == 0)
			light->setType(Ogre::Light::LT_POINT);
		if (type == 1)
			light->setType(Ogre::Light::LT_DIRECTIONAL);
		if (type == 2)
			light->setType(Ogre::Light::LT_SPOTLIGHT);

		SetColor(color_r, color_g, color_b, spec_color_r, spec_color_g, spec_color_b);
		Move(position);
		light->setDirection(sbs->ToRemote(direction, false));
		if (Type == 2)
			light->setSpotlightRange(Ogre::Degree(spot_inner_angle), Ogre::Degree(spot_outer_angle), spot_falloff);
		light->setAttenuation(sbs->ToRemote(att_range), att_constant, att_linear, att_quadratic);
	}
	catch (Ogre::Exception &e)
	{
		ReportError("Error creating light:\n" + e.getDescription());
	}
}

Light::~Light()
{
	if (light)
		GetSceneNode()->DetachObject(light);
	sbs->mSceneManager->destroyLight(GetName());

	//unregister from parent
	if (sbs->FastDelete == false && parent_deleting == false)
	{
		std::string type = GetParent()->GetType();

		if (type == "ElevatorCar")
			static_cast<ElevatorCar*>(GetParent())->RemoveLight(this);
		else if (type == "Floor")
			static_cast<Floor*>(GetParent())->RemoveLight(this);
		else if (type == "Shaft")
			static_cast<Shaft*>(GetParent())->RemoveLight(this);
		else if (type == "Stairs")
			static_cast<Stairs*>(GetParent())->RemoveLight(this);
		else if (type == "SBS")
			sbs->RemoveLight(this);
	}
}

void Light::SetColor(Real color_r, Real color_g, Real color_b, Real spec_color_r, Real spec_color_g, Real spec_color_b)
{
	//set color of light
	light->setDiffuseColour(color_r, color_g, color_b);
	light->setSpecularColour(spec_color_r, spec_color_g, spec_color_b);
}

}
