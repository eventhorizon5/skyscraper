/* $Id$ */

/*
	Scalable Building Simulator - Floor Indicator Object
	The Skyscraper Project - Version 1.10 Alpha
	Copyright (C)2004-2016 Ryan Thoryk
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
#include "mesh.h"
#include "floorindicator.h"
#include "elevator.h"
#include "profiler.h"
#include "floor.h"

namespace SBS {

FloorIndicator::FloorIndicator(Object *parent, int elevator, const std::string &texture_prefix, const std::string &direction, float CenterX, float CenterZ, float width, float height, float voffset) : Object(parent)
{
	//creates a new floor indicator at the specified position

	//set up SBS object
	SetValues("FloorIndicator", "", false);

	is_enabled = true;
	elev = elevator;
	Prefix = texture_prefix;

	//move object
	Move(CenterX, voffset, CenterZ);

	std::string name = "Floor Indicator " + ToString(elevator);
	SetName(name);
	FloorIndicatorMesh = new MeshObject(this, name, 0, "", sbs->GetConfigFloat("Skyscraper.SBS.MaxSmallRenderDistance", 100));

	std::string texture = "Button" + sbs->GetFloor(sbs->GetElevator(elevator)->StartingFloor)->ID;
	std::string tmpdirection = direction;
	SetCase(tmpdirection, false);

	if (tmpdirection == "front" || tmpdirection == "back")
	{
		if (tmpdirection == "front")
			sbs->DrawWalls(true, false, false, false, false, false);
		else
			sbs->DrawWalls(false, true, false, false, false, false);

		sbs->AddWallMain(this, FloorIndicatorMesh, "Floor Indicator", texture, 0, -width / 2, 0, width / 2, 0, height, height, 0, 0, 1, 1, false);
	}
	if (tmpdirection == "left" || tmpdirection == "right")
	{
		if (tmpdirection == "left")
			sbs->DrawWalls(true, false, false, false, false, false);
		else
			sbs->DrawWalls(false, true, false, false, false, false);

		sbs->AddWallMain(this, FloorIndicatorMesh, "Floor Indicator", texture, 0, 0, width / 2, 0, -width / 2, height, height, 0, 0, 1, 1, false);
	}
	sbs->ResetWalls();
}

FloorIndicator::~FloorIndicator()
{
	if (FloorIndicatorMesh)
	{
		FloorIndicatorMesh->parent_deleting = true;
		delete FloorIndicatorMesh;
	}
	FloorIndicatorMesh = 0;

	//unregister from parent
	if (sbs->FastDelete == false && parent_deleting == false)
	{
		std::string type = GetParent()->GetType();

		if (type == "Elevator")
			static_cast<Elevator*>(GetParent())->RemoveFloorIndicator(this);
		else if (type == "Floor")
			static_cast<Floor*>(GetParent())->RemoveFloorIndicator(this);
	}
}

void FloorIndicator::Enabled(bool value)
{
	//turns indicator on/off

	if (is_enabled == value)
		return;

	FloorIndicatorMesh->Enable(value);
	is_enabled = value;
}

void FloorIndicator::Update()
{
	//update indicator display with elevator's current floor identifier

	SBS_PROFILE("FloorIndicator::Update");

	std::string texture;
	Elevator *elevator = sbs->GetElevator(elev);

	if (!elevator)
		return;

	texture = elevator->GetFloorDisplay();

	//don't update texture if no value
	if (texture == "")
		return;

	texture.insert(0, Prefix);

	FloorIndicatorMesh->ChangeTexture(texture);
}

}
