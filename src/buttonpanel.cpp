/* $Id$ */

/*
	Scalable Building Simulator - Elevator Button Panel Class
	The Skyscraper Project - Version 1.4 Alpha
	Copyright (C)2005-2009 Ryan Thoryk
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

ButtonPanel::ButtonPanel(int _elevator, int index, const char *texture, int rows, int columns, const char *direction, float CenterX, float CenterZ, float buttonwidth, float buttonheight, float spacingX, float spacingY, float voffset, float tw, float th)
{
	//Create an elevator button panel
	//index is for specifying multiple panels within the same elevator

	elevator = _elevator;
	Direction = direction;
	Origin.x = sbs->GetElevator(elevator)->Origin.x + CenterX;
	Origin.z = sbs->GetElevator(elevator)->Origin.z + CenterZ;
	ButtonWidth = buttonwidth;
	ButtonHeight = buttonheight;
	Rows = rows;
	Columns = columns;
	SpacingX = ButtonWidth * spacingX;
	SpacingY = ButtonHeight * spacingY;

	//total spacing plus total button widths
	Width = ((Columns + 1) * SpacingX) + (Columns * ButtonWidth);
	Height = ((Rows + 1) * SpacingY) + (Rows * ButtonHeight);

	//get vertical
	Origin.y = voffset - (Height / 2);

	//create mesh
	csString buffer, buffer2, buffer3;
	buffer2 = elevator;
	buffer3 = index;
	buffer = "Button Panel " + buffer2 + ":" + buffer3;
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
		AddWall("Panel", texture, 0, -(Width / 2), 0, Width / 2, 0, Height, Height, 0, 0, tw, th);
	}
	if (Direction == "back")
	{
		sbs->DrawWalls(false, true, false, false, false, false);
		AddWall("Panel", texture, 0, -(Width / 2), 0, Width / 2, 0, Height, Height, 0, 0, tw, th);
	}
	if (Direction == "left")
	{
		sbs->DrawWalls(true, false, false, false, false, false);
		AddWall("Panel", texture, 0, 0, -(Width / 2), 0, Width / 2, Height, Height, 0, 0, tw, th);
	}
	if (Direction == "right")
	{
		sbs->DrawWalls(false, true, false, false, false, false);
		AddWall("Panel", texture, 0, 0, -(Width / 2), 0, Width / 2, Height, Height, 0, 0, tw, th);
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
	//width and height are the button size percentage that the button should be (divided by 100); default is 1 for each, aka 100%.

	csString floornum;
	floornum = floor;
	AddButton(floornum, texture, row, column, width, height);
}

void ButtonPanel::AddControlButton(const char *texture, int row, int column, const char *type, float width, float height)
{
	//create a control button at specified row/column position
	//width and height are the button size percentage that the button should be (divided by 100); default is 1 for each, aka 100%

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

	//set to default if value is 0
	if (bwidth == 0)
		bwidth = 1;

	if (bheight == 0)
		bheight = 1;

	//vertical position is the top of the panel, minus the total spacing above it,
	//minus the total button spaces above (and including) it, minus half of the extra height
	ypos = (Origin.y + Height) - (SpacingY * row) - (ButtonHeight * row) - ((bheight - 1) / 2);
	sbs->ReverseExtents(false, false, false);
	if (Direction == "front" || Direction == "back")
	{
		if (Direction == "front")
		{
			sbs->DrawWalls(true, false, false, false, false, false);
			//hoizontal position is the left-most edge of the panel (origin aka center minus
			//half the width), plus total spacing to the left of it, plus total button spaces
			//to the left of it, plus half of the extra width
			xpos = (Origin.x - (Width / 2)) + (SpacingX * column) + (ButtonWidth * (column - 1)) + ((bwidth - 1) / 2);
			zpos = Origin.z - 0.001;
			sbs->AddWallMain(ButtonPanel_state, name, texture, 0, xpos, zpos, xpos + ButtonWidth, zpos, ButtonHeight, ButtonHeight, ypos, ypos, 1, 1);
		}
		else
		{
			//back
			sbs->DrawWalls(false, true, false, false, false, false);
			xpos = (Origin.x + (Width / 2)) - (SpacingX * column) - (ButtonWidth * (column - 1)) - ((bwidth - 1) / 2);
			zpos = Origin.z + 0.001;
			sbs->AddWallMain(ButtonPanel_state, name, texture, 0, xpos, zpos, xpos - ButtonWidth, zpos, ButtonHeight, ButtonHeight, ypos, ypos, 1, 1);
		}
	}
	else
	{
		if (Direction == "left")
		{
			sbs->DrawWalls(true, false, false, false, false, false);
			xpos = Origin.x - 0.001;
			zpos = (Origin.z + (Width / 2))  - (SpacingX * column) - (ButtonWidth * (column - 1)) - ((bwidth - 1) / 2);
			sbs->AddWallMain(ButtonPanel_state, name, texture, 0, xpos, zpos, xpos, zpos - ButtonWidth, ButtonHeight, ButtonHeight, ypos, ypos, 1, 1);
		}
		else
		{
			//right
			sbs->DrawWalls(false, true, false, false, false, false);
			xpos = Origin.x + 0.001;
			zpos = (Origin.z - (Width / 2)) + (SpacingX * column) + (ButtonWidth * (column - 1)) + ((bwidth - 1) / 2);
			sbs->AddWallMain(ButtonPanel_state, name, texture, 0, xpos, zpos, xpos, zpos + ButtonWidth, ButtonHeight, ButtonHeight, ypos, ypos, 1, 1);
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

	//exit if in inspection mode or in fire service phase 1 mode
	if (sbs->GetElevator(elevator)->InspectionService == true || sbs->GetElevator(elevator)->FireServicePhase1 == 1)
		return;

	csString name = ButtonPanel_state->GetPolygonName(index);
	csString name2 = name;

	//strip off any text after and including a colon
	int colon = name.Find(":");
	if (colon > 0)
		name = name2.Slice(0, colon);

	if (IsNumeric(name) == true)
	{
		int floor = atoi(name.GetData());
		int elev_floor = sbs->GetElevator(elevator)->GetFloor();
		//int elev_dir = sbs->GetElevator(elevator)->Direction;

		//elevator is above floor
		if (elev_floor > floor)
			sbs->GetElevator(elevator)->AddRoute(floor, -1);

		//elevator is below floor
		if (elev_floor < floor)
			sbs->GetElevator(elevator)->AddRoute(floor, 1);

		//elevator is on floor
		if (elev_floor == floor)
		{
			if (sbs->GetElevator(elevator)->Direction == 0)
			{
				//stopped - play chime and open doors
				sbs->GetElevator(elevator)->Chime(0, floor);
				sbs->GetElevator(elevator)->OpenDoors();
			}
			else if (sbs->GetElevator(elevator)->Direction == -1)
			{
				//elevator is on floor but already moving down; add an upward route
				sbs->GetElevator(elevator)->AddRoute(floor, 1);
			}
			else if (sbs->GetElevator(elevator)->Direction == 1)
			{
				//elevator is on floor but already moving up; add a downward route
				sbs->GetElevator(elevator)->AddRoute(floor, -1);
			}
		}
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

void ButtonPanel::Move(const csVector3 &position)
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

	sbs->EnableMesh(ButtonPanelMesh, value);
}

int ButtonPanel::AddWall(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th)
{
	//Adds a wall with the specified dimensions
	float tw2 = tw;
	float th2;
	float tempw1;
	float tempw2;

	//Set horizontal scaling
	x1 = x1 * sbs->HorizScale;
	x2 = x2 * sbs->HorizScale;
	z1 = z1 * sbs->HorizScale;
	z2 = z2 * sbs->HorizScale;

	//Call texture autosizing formulas
	if (z1 == z2)
		tw2 = sbs->AutoSize(x1, x2, true, tw);
	if (x1 == x2)
		tw2 = sbs->AutoSize(z1, z2, true, tw);
	if ((z1 != z2) && (x1 != x2))
	{
		//calculate diagonals
		if (x1 > x2)
			tempw1 = x1 - x2;
		else
			tempw1 = x2 - x1;
		if (z1 > z2)
			tempw2 = z1 - z2;
		else
			tempw2 = z2 - z1;
		tw2 = sbs->AutoSize(0, sqrt(pow(tempw1, 2) + pow(tempw2, 2)), true, tw);
	}
	th2 = sbs->AutoSize(0, height1, false, th);

	return sbs->AddWallMain(ButtonPanel_state, name, texture, thickness, Origin.x + x1, Origin.z + z1, Origin.x + x2, Origin.z + z2, height1, height2, Origin.y + voffset1, Origin.y + voffset2, tw2, th2);
}
