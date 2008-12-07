/* $Id$ */

/*
	Scalable Building Simulator - Elevator Button Panel Class
	The Skyscraper Project - Version 1.1 Alpha
	Copyright (C)2005-2008 Ryan Thoryk
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
#include "buttonpanel.h"
#include "elevator.h"
#include "sbs.h"
#include "camera.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

ButtonPanel::ButtonPanel(int _elevator, const char *texture, int columns, int rows, const char *direction, float CenterX, float CenterZ, float width, float height, float voffset, float spacingX, float spacingY, float tw, float th)
{
	//Create an elevator button panel

	elevator = _elevator;
	Direction = direction;
	Origin.x = sbs->GetElevator(elevator)->Origin.x + CenterX;
	Origin.y = voffset;
	Origin.z = sbs->GetElevator(elevator)->Origin.z + CenterZ;
	Width = width;
	Height = height;
	Rows = rows;
	Columns = columns;

	//first get the amount of button units the total space the spacers will take up
	//to do this, you take the # of rows/colums, add 1 for the end spacer,
	//and multiply by the percentage specified earlier
	SpacingX = (Columns + 1) * spacingX;
	SpacingY = (Rows + 1) * spacingY;

	//next calculate the size of each button unit
	GridSize.x = Width / (Columns + SpacingX);
	GridSize.y = Height / (Rows + SpacingY);

	//finally get the spacer unit size
	SpacingX = GridSize.x * spacingX;
	SpacingY = GridSize.y * spacingY;

	//create mesh
	csString buffer, buffer2;
	buffer2 = elevator;
	buffer = "Button Panel " + buffer2;
	buffer.Trim();
	ButtonPanelMesh = sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData());
	ButtonPanel_state = scfQueryInterface<iThingFactoryState> (ButtonPanelMesh->GetMeshObject()->GetFactory());
	ButtonPanelMesh->SetZBufMode(CS_ZBUF_USE);
	ButtonPanelMesh->SetRenderPriority(sbs->engine->GetAlphaRenderPriority());
	ButtonPanelMesh->GetMeshObject()->SetMixMode(CS_FX_ALPHA);

	//move
	SetToElevatorAltitude();

	//create panel back
	sbs->ReverseExtents(false, false, false);
	if (Direction == "front")
	{
		sbs->DrawWalls(true, false, false, false, false, false);
		sbs->AddWallMain(ButtonPanel_state, "Panel", texture, 0, Origin.x - (Width / 2), Origin.z, Origin.x + (Width / 2), Origin.z, Height, Height, Origin.y, Origin.y, tw, th);
	}
	if (Direction == "back")
	{
		sbs->DrawWalls(false, true, false, false, false, false);
		sbs->AddWallMain(ButtonPanel_state, "Panel", texture, 0, Origin.x - (Width / 2), Origin.z, Origin.x + (Width / 2), Origin.z, Height, Height, Origin.y, Origin.y, tw, th);
	}
	if (Direction == "left")
	{
		sbs->DrawWalls(true, false, false, false, false, false);
		sbs->AddWallMain(ButtonPanel_state, "Panel", texture, 0, Origin.x, Origin.z - (Width / 2), Origin.x, Origin.z + (Width / 2), Height, Height, Origin.y, Origin.y, tw, th);
	}
	if (Direction == "right")
	{
		sbs->DrawWalls(false, true, false, false, false, false);
		sbs->AddWallMain(ButtonPanel_state, "Panel", texture, 0, Origin.x, Origin.z - (Width / 2), Origin.x, Origin.z + (Width / 2), Height, Height, Origin.y, Origin.y, tw, th);
	}
	sbs->ResetWalls();
	sbs->ResetExtents();
	Enabled(false); //disable mesh at startup
}

ButtonPanel::~ButtonPanel()
{

}

void ButtonPanel::AddFloorButton(const char *texture, int row, int column, int floor, float width, float height)
{
	//create a standard floor button at specified row/column position
	//width and height are the gridsize percentage that the button should be; default is 1 for each.
	//if 0 is used for one, then use the size of the other value

	csString floornum;
	floornum = floor;
	AddButton(floornum, texture, row, column, width, height);
}

void ButtonPanel::AddControlButton(const char *texture, int row, int column, const char *type, float width, float height)
{
	//create a control button at specified row/column position
	//width and height are the gridsize percentage that the button should be; default is 1 for each.
	//if 0 is used for one, then use the size of the other value

	//type is one of these:
	//open = Open Doors
	//close = Close Doors
	//cancel = Call Cancel
	//stop = Stop
	//alarm = Alarm

	csString name = type;
	name.Downcase();

	AddButton(name.GetData(), texture, row, column, width, height);
}

void ButtonPanel::AddButton(const char *name, const char *texture, int row, int column, float bwidth, float bheight)
{
	//create the button polygon
	float xpos, ypos, zpos;
	float heightmod = 0, widthmod = 0;


	if (bwidth == 0 && bheight == 0)
	{
		//use defaults if both are 0
		bwidth = 1;
		bheight = 1;
	}
	else
	{
		if (bwidth == 0)
		{
			//use height value (through modifier) as width
			bwidth = 1;
			widthmod = (GridSize.y * bheight) - GridSize.x;
		}

		if (bheight == 0)
		{
			//use width value (through modifier) as height
			bheight = 1;
			heightmod = (GridSize.x * bwidth) - GridSize.y;
		}
	}

	//vertical position is the top of the panel, minus the total spacing above it,
	//minus the total button spaces above (and including) it, minus half of the extra height,
	//minus half of the modified height
	ypos = (Origin.y + Height) - (SpacingY * row) - (GridSize.y * row) - ((bheight - 1) / 2) - (heightmod / 2);
	sbs->ReverseExtents(false, false, false);
	if (Direction == "front" || Direction == "back")
	{
		if (Direction == "front")
		{
			sbs->DrawWalls(true, false, false, false, false, false);
			//hoizontal position is the left-most edge of the panel (origin aka center minus
			//half the width), plus total spacing to the left of it, plus total button spaces
			//to the left of it, plus half of the extra width, plus half of the modified width
			xpos = (Origin.x - (Width / 2)) + (SpacingX * column) + (GridSize.x * (column - 1)) + ((bwidth - 1) / 2) + (widthmod / 2);
			zpos = Origin.z - 0.001;
			sbs->AddWallMain(ButtonPanel_state, name, texture, 0, xpos, zpos, xpos + GridSize.x + (widthmod / 2), zpos, GridSize.y, GridSize.y, ypos, ypos, 1, 1);
		}
		else
		{
			//back
			sbs->DrawWalls(false, true, false, false, false, false);
			xpos = (Origin.x + (Width / 2)) - (SpacingX * column) - (GridSize.x * (column - 1)) - ((bwidth - 1) / 2) - (widthmod / 2);
			zpos = Origin.z + 0.001;
			sbs->AddWallMain(ButtonPanel_state, name, texture, 0, xpos, zpos, xpos - GridSize.x - (widthmod / 2), zpos, GridSize.y, GridSize.y, ypos, ypos, 1, 1);
		}
	}
	else
	{
		if (Direction == "left")
		{
			sbs->DrawWalls(true, false, false, false, false, false);
			xpos = Origin.x - 0.001;
			zpos = (Origin.z + (Width / 2))  - (SpacingX * column) - (GridSize.x * (column - 1)) - ((bwidth - 1) / 2) - (widthmod / 2);
			sbs->AddWallMain(ButtonPanel_state, name, texture, 0, xpos, zpos, xpos, zpos - GridSize.x - (widthmod / 2), GridSize.y, GridSize.y, ypos, ypos, 1, 1);
		}
		else
		{
			//right
			sbs->DrawWalls(false, true, false, false, false, false);
			xpos = Origin.x + 0.001;
			zpos = (Origin.z - (Width / 2)) + (SpacingX * column) + (GridSize.x * (column - 1)) + ((bwidth - 1) / 2) + (widthmod / 2);
			sbs->AddWallMain(ButtonPanel_state, name, texture, 0, xpos, zpos, xpos, zpos + GridSize.x + (widthmod / 2), GridSize.y, GridSize.y, ypos, ypos, 1, 1);
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
	csString name2 = name;
	if (name.Find(":") > 0)
		name = name2.Slice(0, name.Find(":"));

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

void ButtonPanel::Enabled(bool value)
{
	//enable or disable button panel
	if (value == true)
	{
		ButtonPanelMesh->GetFlags().Reset (CS_ENTITY_INVISIBLEMESH);
		ButtonPanelMesh->GetFlags().Reset (CS_ENTITY_NOSHADOWS);
		ButtonPanelMesh->GetFlags().Reset (CS_ENTITY_NOHITBEAM);
	}
	else
	{
		ButtonPanelMesh->GetFlags().Set (CS_ENTITY_INVISIBLEMESH);
		ButtonPanelMesh->GetFlags().Set (CS_ENTITY_NOSHADOWS);
		ButtonPanelMesh->GetFlags().Set (CS_ENTITY_NOHITBEAM);
	}
}

