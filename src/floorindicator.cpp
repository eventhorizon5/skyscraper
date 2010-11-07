/* $Id$ */

/*
	Scalable Building Simulator - Floor Indicator Subsystem Class
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

#include <OgreMaterial.h>
#include "globals.h"
#include "sbs.h"
#include "floorindicator.h"
#include "elevator.h"
#include "floor.h"

extern SBS *sbs; //external pointer to the SBS engine

FloorIndicator::FloorIndicator(Object *parent, int elevator, const char *texture_prefix, const char *direction, float CenterX, float CenterZ, float width, float height, float altitude)
{
	//creates a new floor indicator at the specified position

	//set up SBS object
	object = new Object();
	object->SetValues(this, parent, "FloorIndicator", "", false);

	IsEnabled = true;
	elev = elevator;
	Prefix = texture_prefix;

	Ogre::String buffer = Ogre::StringConverter::toString(elevator);
	object->SetName(Ogre::String("Floor Indicator " + buffer).c_str());
	TrimString(buffer);
	FloorIndicatorMesh = new MeshObject(object, buffer.c_str(), 0, sbs->GetConfigFloat("Skyscraper.SBS.MaxSmallRenderDistance", 100));

	Ogre::String texture = "Button" + sbs->GetFloor(sbs->GetElevator(elevator)->OriginFloor)->ID;
	Ogre::String tmpdirection = direction;
	SetCase(tmpdirection, false);

	if (tmpdirection == "front" || tmpdirection == "back")
	{
		if (tmpdirection == "front")
			sbs->DrawWalls(true, false, false, false, false, false);
		else
			sbs->DrawWalls(false, true, false, false, false, false);

		sbs->AddWallMain(object, FloorIndicatorMesh, "Floor Indicator", texture.c_str(), 0, CenterX - (width / 2), CenterZ, CenterX + (width / 2), CenterZ, height, height, altitude, altitude, 1, 1, false);
	}
	if (tmpdirection == "left" || tmpdirection == "right")
	{
		if (tmpdirection == "left")
			sbs->DrawWalls(true, false, false, false, false, false);
		else
			sbs->DrawWalls(false, true, false, false, false, false);

		sbs->AddWallMain(object, FloorIndicatorMesh, "Floor Indicator", texture.c_str(), 0, CenterX, CenterZ + (width / 2), CenterX, CenterZ - (width / 2), height, height, altitude, altitude, 1, 1, false);
	}
	sbs->ResetWalls();
}

FloorIndicator::~FloorIndicator()
{
	if (FloorIndicatorMesh)
		delete FloorIndicatorMesh;
	FloorIndicatorMesh = 0;

	//unregister from parent
	if (sbs->FastDelete == false)
	{
		if (object->parent_deleting == false)
		{
			if (Ogre::String(object->GetParent()->GetType()) == "Elevator")
				((Elevator*)object->GetParent()->GetRawObject())->RemoveFloorIndicator(this);
			if (Ogre::String(object->GetParent()->GetType()) == "Floor")
				((Floor*)object->GetParent()->GetRawObject())->RemoveFloorIndicator(this);
		}
	}
	delete object;
}

void FloorIndicator::Enabled(bool value)
{
	//turns indicator on/off

	if (IsEnabled == value)
		return;

	FloorIndicatorMesh->Enable(value);
	IsEnabled = value;
}

void FloorIndicator::SetPosition(const Ogre::Vector3& position)
{
	//set position of indicator
	FloorIndicatorMesh->Move(position, false, false, false);
}

void FloorIndicator::MovePosition(const Ogre::Vector3& position)
{
	//move indicator by a relative amount
	FloorIndicatorMesh->Move(position, true, true, true);
}

void FloorIndicator::Update(const char *value)
{
	//update display with a new texture value, such as a floor number

	Ogre::String texture;
	texture = value;
	texture.insert(0, Prefix);

	FloorIndicatorMesh->ChangeTexture(texture.c_str());
}

Ogre::Vector3 FloorIndicator::GetPosition()
{
	//return current position of the indicator
	return FloorIndicatorMesh->GetPosition();
}
