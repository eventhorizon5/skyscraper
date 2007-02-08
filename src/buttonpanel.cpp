/* $Id$ */

/*
	Scalable Building Simulator - Elevator Button Panel Class
	The Skyscraper Project - Version 1.1 Alpha
	Copyright (C)2005-2007 Ryan Thoryk
	http://www.tliquest.net/skyscraper
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

#include <wx/wx.h>
#include <crystalspace.h>
#include "buttonpanel.h"
#include "elevator.h"
#include "sbs.h"
#include "camera.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

ButtonPanel::ButtonPanel(int _elevator, const char *texture, int rows, int columns, const char *direction, float CenterX, float CenterZ, float width, float height, float voffset, float spacing, float tw, float th)
{
	//Create an elevator button panel

	elevator = _elevator;
	Direction = direction;
	Origin.x = sbs->GetElevator(elevator)->Origin.x + CenterX;
	Origin.y = sbs->GetElevator(elevator)->Origin.y + voffset;
	Origin.z = sbs->GetElevator(elevator)->Origin.z + CenterZ;
	Width = width;
	Height = height;
	Rows = rows;
	Columns = columns;
	GridSize.x = Width / (Columns + 2); //leave 1 empty grid space on both sides
	GridSize.y = Height / (Rows + 2); //leave 1 empty grid space on both sides
	Spacing = GridSize.x * spacing;

	//create mesh
	csString buffer, buffer2;
	buffer2 = elevator;
	buffer = "Button Panel " + buffer2;
	buffer.Trim();
	ButtonPanelMesh = sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData());
	ButtonPanel_state = scfQueryInterface<iThingFactoryState> (ButtonPanelMesh->GetMeshObject()->GetFactory());
	ButtonPanelMesh->SetZBufMode(CS_ZBUF_USE);

	//create panel back
	sbs->DrawWalls(true, false, false, false, false, false);
	sbs->ReverseExtents(false, false, false);
	if (Direction == "front")
		sbs->AddWallMain(ButtonPanel_state, "Panel", texture, 0, Origin.x - (Width / 2), Origin.z, Origin.x + (Width / 2), Origin.z, Height, Height, Origin.y, Origin.y, tw, th);
	if (Direction == "back")
		sbs->AddWallMain(ButtonPanel_state, "Panel", texture, 0, Origin.x + (Width / 2), Origin.z, Origin.x - (Width / 2), Origin.z, Height, Height, Origin.y, Origin.y, tw, th);
	if (Direction == "left")
		sbs->AddWallMain(ButtonPanel_state, "Panel", texture, 0, Origin.x, Origin.z + (Width / 2), Origin.x, Origin.z - (Width / 2), Height, Height, Origin.y, Origin.y, tw, th);
	if (Direction == "right")
		sbs->AddWallMain(ButtonPanel_state, "Panel", texture, 0, Origin.x, Origin.z - (Width / 2), Origin.x, Origin.z + (Width / 2), Height, Height, Origin.y, Origin.y, tw, th);
	sbs->ResetWalls();
	sbs->ResetExtents();
}

ButtonPanel::~ButtonPanel()
{

}

void ButtonPanel::AddFloorButton(const char *texture, int row, int column, int floor)
{
	//create a standard floor button at specified row/column position

	csString floornum;
	floornum = floor;
	AddButton(floornum, texture, row, column);
}

void ButtonPanel::AddControlButton(const char *texture, int row, int column, const char *type)
{
	//create a control button at specified row/column position

	//type is one of these:
	//open = Open Doors
	//close = Close Doors
	//cancel = Call Cancel
	//stop = Stop
	//alarm = Alarm

	csString name = type;
	name.Downcase();

	AddButton(name.GetData(), texture, row, column);
}

void ButtonPanel::AddButton(const char *name, const char *texture, int row, int column)
{
	//create the button polygon
	float xpos, ypos, zpos;
	ypos = (Origin.y + Height) - (GridSize.y * (row + 1));
	sbs->DrawWalls(true, false, false, false, false, false);
	sbs->ReverseExtents(false, false, false);
	if (Direction == "front" || Direction == "back")
	{
		if (Direction == "front")
		{
		xpos = (Origin.x - (Width / 2)) + (GridSize.x * column);
			zpos = Origin.z - 0.01;
			sbs->AddWallMain(ButtonPanel_state, name, texture, 0, xpos, zpos, xpos + GridSize.x - Spacing, zpos, GridSize.y - Spacing, GridSize.y - Spacing, ypos, ypos, 1, 1);
		}
		else
		{
			//back
			xpos = (Origin.x + (Width / 2)) - (GridSize.x * column);
			zpos = Origin.z + 0.01;
			sbs->AddWallMain(ButtonPanel_state, name, texture, 0, xpos, zpos, xpos - GridSize.x + Spacing, zpos, GridSize.y - Spacing, GridSize.y - Spacing, ypos, ypos, 1, 1);
		}
	}
	else
	{
		if (Direction == "left")
		{
			xpos = Origin.x - 0.01;
			zpos = (Origin.z + (Width / 2)) - (GridSize.x * column);
			sbs->AddWallMain(ButtonPanel_state, name, texture, 0, xpos, zpos, xpos, zpos - GridSize.x + Spacing, GridSize.y - Spacing, GridSize.y - Spacing, ypos, ypos, 1, 1);
		}
		else
		{
			//right
			xpos = Origin.x + 0.01;
			zpos = (Origin.z - (Width / 2)) + (GridSize.x * column);
			sbs->AddWallMain(ButtonPanel_state, name, texture, 0, xpos, zpos, xpos, zpos + GridSize.x - Spacing, GridSize.y - Spacing, GridSize.y - Spacing, ypos, ypos, 1, 1);
		}
	}
	sbs->ResetWalls();
	sbs->ResetExtents();
}

void ButtonPanel::DeleteButton(int row, int column)
{

}

void ButtonPanel::Press(int index)
{
	//Press selected button

	if (index == -1)
		return;

	csString name = ButtonPanel_state->GetPolygonName(index);

	if (IsNumeric(name) == true)
	{
		int floor = atoi(name.GetData());
		int elev_floor = sbs->GetElevator(elevator)->GetFloor();
		//int elev_dir = sbs->GetElevator(elevator)->Direction;
		
		//elevator is above floor
		if (elev_floor > floor)
			sbs->GetElevator(elevator)->AddRoute(floor, -1);

		//elevator is on or below floor
		if (elev_floor <= floor)
			sbs->GetElevator(elevator)->AddRoute(floor, 1);
	}
	else
	{
		name.Downcase();
		if (name == "open" && sbs->GetElevator(elevator)->Direction == 0)
			sbs->GetElevator(elevator)->OpenDoors();
		if (name == "close" && sbs->GetElevator(elevator)->Direction == 0)
			sbs->GetElevator(elevator)->CloseDoors();
		if (name == "cancel")
			sbs->GetElevator(elevator)->CancelLastRoute();
		if (name == "stop")
			sbs->GetElevator(elevator)->StopElevator();
		if (name == "alarm")
			sbs->GetElevator(elevator)->Alarm();
	}
}

void ButtonPanel::Move(csVector3 position)
{
	//relative movement
	ButtonPanelMesh->GetMovable()->MovePosition(position);
	ButtonPanelMesh->GetMovable()->UpdateMove();
}

void ButtonPanel::SetToElevatorAltitude()
{
	csVector3 pos = ButtonPanelMesh->GetMovable()->GetPosition();
	ButtonPanelMesh->GetMovable()->SetPosition(csVector3(pos.x, sbs->GetElevator(elevator)->GetPosition().y, pos.z));
	ButtonPanelMesh->GetMovable()->UpdateMove();
}
