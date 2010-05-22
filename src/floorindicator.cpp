/* $Id$ */

/*
	Scalable Building Simulator - Floor Indicator Subsystem Class
	The Skyscraper Project - Version 1.7 Alpha
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
#include "floorindicator.h"
#include "elevator.h"
#include "floor.h"

#include <iengine/movable.h>
#include <cstool/genmeshbuilder.h>

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

	csString buffer, buffer2;
	buffer2 = object->GetNumber();
	buffer = elevator;
	object->SetName("Floor Indicator " + buffer);
	buffer.Insert(0, "(" + buffer2 + ")FloorIndicator ");
	buffer.Trim();
	FloorIndicatorMesh = CS::Geometry::GeneralMeshBuilder::CreateFactoryAndMesh(sbs->engine, sbs->area, buffer, buffer + " factory");
	FloorIndicatorMesh->SetZBufMode(CS_ZBUF_USE);
	FloorIndicatorMesh->SetRenderPriority(sbs->engine->GetObjectRenderPriority());
	FloorIndicator_movable = FloorIndicatorMesh->GetMovable();

	csString texture = "Button" + sbs->GetFloor(sbs->GetElevator(elevator)->OriginFloor)->ID;
	csString tmpdirection = direction;
	tmpdirection.Downcase();

	if (tmpdirection == "front")
		sbs->AddGenWall(FloorIndicatorMesh, texture, CenterX - (width / 2), CenterZ, CenterX + (width / 2), CenterZ, height, altitude, 1, 1);
	if (tmpdirection == "back")
		sbs->AddGenWall(FloorIndicatorMesh, texture, CenterX + (width / 2), CenterZ, CenterX - (width / 2), CenterZ, height, altitude, 1, 1);
	if (tmpdirection == "left")
		sbs->AddGenWall(FloorIndicatorMesh, texture, CenterX, CenterZ + (width / 2), CenterX, CenterZ - (width / 2), height, altitude, 1, 1);
	if (tmpdirection == "right")
		sbs->AddGenWall(FloorIndicatorMesh, texture, CenterX, CenterZ - (width / 2), CenterX, CenterZ + (width / 2), height, altitude, 1, 1);
}

FloorIndicator::~FloorIndicator()
{
	FloorIndicator_movable = 0;
	if (sbs->FastDelete == false)
	{
		sbs->engine->WantToDie(FloorIndicatorMesh);

		//unregister from parent
		if (object->parent_deleting == false)
		{
			if (csString(object->GetParent()->GetType()) == "Elevator")
				((Elevator*)object->GetParent()->GetRawObject())->RemoveFloorIndicator(this);
			if (csString(object->GetParent()->GetType()) == "Floor")
				((Floor*)object->GetParent()->GetRawObject())->RemoveFloorIndicator(this);
		}
	}
	FloorIndicatorMesh = 0;
	delete object;
}

void FloorIndicator::Enabled(bool value)
{
	//turns indicator on/off

	if (IsEnabled == value)
		return;

	sbs->EnableMesh(FloorIndicatorMesh, value);
	IsEnabled = value;
}

void FloorIndicator::SetPosition(csVector3 position)
{
	//set position of indicator
	FloorIndicator_movable->SetPosition(sbs->ToRemote(position));
	FloorIndicator_movable->UpdateMove();
}

void FloorIndicator::MovePosition(csVector3 position)
{
	//move indicator by a relative amount
	FloorIndicator_movable->MovePosition(sbs->ToRemote(position));
	FloorIndicator_movable->UpdateMove();
}

void FloorIndicator::Update(const char *value)
{
	//update display with a new texture value, such as a floor number

	csString texture;
	texture = value;
	texture.Insert(0, Prefix);

	sbs->ChangeTexture(FloorIndicatorMesh, texture.GetData());
}

csVector3 FloorIndicator::GetPosition()
{
	//return current position of the indicator
	return sbs->ToLocal(FloorIndicator_movable->GetPosition());
}
