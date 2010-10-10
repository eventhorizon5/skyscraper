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

#include "globals.h"
#include "sbs.h"
#include "floorindicator.h"
#include "elevator.h"
#include "floor.h"

#include <iengine/movable.h>

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

	csString buffer;
	buffer = elevator;
	object->SetName("Floor Indicator " + buffer);
	buffer.Trim();
	FloorIndicatorMesh = new MeshObject(object, buffer, sbs->confman->GetFloat("Skyscraper.SBS.MaxSmallRenderDistance", 100));

	csString texture = "Button" + sbs->GetFloor(sbs->GetElevator(elevator)->OriginFloor)->ID;
	csString tmpdirection = direction;
	tmpdirection.Downcase();

	if (tmpdirection == "front")
		sbs->AddGenWall(FloorIndicatorMesh->MeshWrapper, texture, CenterX - (width / 2), CenterZ, CenterX + (width / 2), CenterZ, height, altitude, 1, 1);
	if (tmpdirection == "back")
		sbs->AddGenWall(FloorIndicatorMesh->MeshWrapper, texture, CenterX + (width / 2), CenterZ, CenterX - (width / 2), CenterZ, height, altitude, 1, 1);
	if (tmpdirection == "left")
		sbs->AddGenWall(FloorIndicatorMesh->MeshWrapper, texture, CenterX, CenterZ + (width / 2), CenterX, CenterZ - (width / 2), height, altitude, 1, 1);
	if (tmpdirection == "right")
		sbs->AddGenWall(FloorIndicatorMesh->MeshWrapper, texture, CenterX, CenterZ - (width / 2), CenterX, CenterZ + (width / 2), height, altitude, 1, 1);
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
			if (csString(object->GetParent()->GetType()) == "Elevator")
				((Elevator*)object->GetParent()->GetRawObject())->RemoveFloorIndicator(this);
			if (csString(object->GetParent()->GetType()) == "Floor")
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

void FloorIndicator::SetPosition(const csVector3& position)
{
	//set position of indicator
	FloorIndicatorMesh->Movable->SetPosition(sbs->ToRemote(position));
	FloorIndicatorMesh->Movable->UpdateMove();
}

void FloorIndicator::MovePosition(const csVector3& position)
{
	//move indicator by a relative amount
	FloorIndicatorMesh->Movable->MovePosition(sbs->ToRemote(position));
	FloorIndicatorMesh->Movable->UpdateMove();
}

void FloorIndicator::Update(const char *value)
{
	//update display with a new texture value, such as a floor number

	csString texture;
	texture = value;
	texture.Insert(0, Prefix);

	FloorIndicatorMesh->ChangeTexture(texture);
}

csVector3 FloorIndicator::GetPosition()
{
	//return current position of the indicator
	return sbs->ToLocal(FloorIndicatorMesh->Movable->GetPosition());
}
