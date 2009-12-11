/* $Id$ */

/*
	Scalable Building Simulator - Elevator Button Panel Class
	The Skyscraper Project - Version 1.6 Alpha
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
	Index = index;
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
	ButtonPanelMesh->SetRenderPriority(sbs->engine->GetObjectRenderPriority());

	//move
	SetToElevatorAltitude();

	//create panel back
	sbs->ResetTextureMapping(true);
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
	sbs->ResetTextureMapping();
	Enabled(false); //disable mesh at startup
}

ButtonPanel::~ButtonPanel()
{
	//delete controls
	for (int i = 0; i < controls.GetSize(); i++)
	{
		if (controls[i])
			delete controls[i];
		controls[i] = 0;
	}
	//delete panel
	ButtonPanel_state = 0;
	ButtonPanelMesh = 0;
}

void ButtonPanel::AddFloorButton(const char *texture, const char *texture_lit, int row, int column, int floor, float width, float height, float hoffset, float voffset)
{
	//create a standard floor button at specified row/column position
	//width and height are the button size percentage that the button should be (divided by 100); default is 1 for each, aka 100%.
	//hoffset and voffset are the optional horizontal and vertical offsets to place the button (in order to place it outside of the default grid position)

	csString floornum;
	floornum = floor;
	AddButton(floornum, texture, texture_lit, row, column, width, height, hoffset, voffset);
}

void ButtonPanel::AddControlButton(const char *texture, const char *texture_lit, int row, int column, const char *type, float width, float height, float hoffset, float voffset)
{
	//create a control button at specified row/column position
	//width and height are the button size percentage that the button should be (divided by 100); default is 1 for each, aka 100%
	//hoffset and voffset are the optional horizontal and vertical offsets to place the button (in order to place it outside of the default grid position)

	//type is one of these:
	//open = Open Doors
	//close = Close Doors
	//cancel = Call Cancel
	//stop = Stop
	//alarm = Alarm

	csString name = type;
	name.Downcase();

	AddButton(name.GetData(), texture, texture_lit, row, column, width, height, hoffset, voffset);
}

void ButtonPanel::AddButton(const char *name, const char *texture, const char *texture_lit, int row, int column, float bwidth, float bheight, float hoffset, float voffset)
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
	ypos += voffset * ButtonHeight;

	if (Direction == "front")
	{
		//horizontal position is the left-most edge of the panel (origin aka center minus
		//half the width), plus total spacing to the left of it, plus total button spaces
		//to the left of it, plus half of the extra width
		xpos = (Origin.x - (Width / 2)) + (SpacingX * column) + (ButtonWidth * (column - 1)) + ((bwidth - 1) / 2);
		zpos = Origin.z - 0.001;
		xpos += hoffset * ButtonWidth;
	}
	if (Direction == "back")
	{
		//back
		xpos = (Origin.x + (Width / 2)) - (SpacingX * column) - (ButtonWidth * (column - 1)) - ((bwidth - 1) / 2);
		zpos = Origin.z + 0.001;
		xpos -= hoffset * ButtonWidth;
	}
	if (Direction == "left")
	{
		xpos = Origin.x - 0.001;
		zpos = (Origin.z + (Width / 2))  - (SpacingX * column) - (ButtonWidth * (column - 1)) - ((bwidth - 1) / 2);
		zpos -= hoffset * ButtonWidth;
	}
	if (Direction == "right")
	{
		//right
		xpos = Origin.x + 0.001;
		zpos = (Origin.z - (Width / 2)) + (SpacingX * column) + (ButtonWidth * (column - 1)) + ((bwidth - 1) / 2);
		zpos += hoffset * ButtonWidth;
	}

	//create control object
	controls.SetSize(controls.GetSize() + 1);
	int control_index = controls.GetSize() - 1;
	csString buffer, buffer2, buffer3, buffer4;
	buffer2 = elevator;
	buffer3 = Index;
	buffer4 = control_index;
	buffer = "Button Panel " + buffer2 + ":" + buffer3 + " Control " + buffer4;
	buffer.Trim();
	controls[control_index] = new Control(1, buffer, name, texture, texture_lit, Direction, xpos, zpos, ButtonWidth * bwidth, ButtonHeight * bheight, ypos);
	//move control
	controls[control_index]->SetPositionY(sbs->GetElevator(elevator)->GetPosition().y);
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

	//exit if index is invalid
	if (index < 0 || index > controls.GetSize() - 1)
		return;

	//get action name of button
	csString name = controls[index]->ActionName;

	if (IsNumeric(name) == true)
	{
		int floor = atoi(name);
		int elev_floor = sbs->GetElevator(elevator)->GetFloor();

		//elevator is above floor
		if (elev_floor > floor)
			sbs->GetElevator(elevator)->AddRoute(floor, -1, true);

		//elevator is below floor
		if (elev_floor < floor)
			sbs->GetElevator(elevator)->AddRoute(floor, 1, true);

		//elevator is on floor
		if (elev_floor == floor)
		{
			if (sbs->GetElevator(elevator)->Direction == 0)
			{
				//stopped - play chime and open doors
				if (sbs->GetElevator(elevator)->QueuePositionDirection == -1 || sbs->GetElevator(elevator)->LastQueueDirection == -1)
					sbs->GetElevator(elevator)->Chime(0, floor, false);
				else if (sbs->GetElevator(elevator)->QueuePositionDirection == 1 || sbs->GetElevator(elevator)->LastQueueDirection == 1)
					sbs->GetElevator(elevator)->Chime(0, floor, true);
				sbs->GetElevator(elevator)->OpenDoors();
			}
			else if (sbs->GetElevator(elevator)->Direction == -1)
			{
				//elevator is on floor but already moving down; add an upward route
				sbs->GetElevator(elevator)->AddRoute(floor, 1, true);
			}
			else if (sbs->GetElevator(elevator)->Direction == 1)
			{
				//elevator is on floor but already moving up; add a downward route
				sbs->GetElevator(elevator)->AddRoute(floor, -1, true);
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

	//move controls
	for (int i = 0; i < controls.GetSize(); i++)
	{
		controls[i]->Move(position);
	}
}

void ButtonPanel::SetToElevatorAltitude()
{
	csVector3 pos = sbs->ToLocal(ButtonPanelMesh->GetMovable()->GetPosition());
	csVector3 pos_new = csVector3(pos.x, sbs->GetElevator(elevator)->GetPosition().y, pos.z);
	ButtonPanelMesh->GetMovable()->SetPosition(sbs->ToRemote(pos_new));
	ButtonPanelMesh->GetMovable()->UpdateMove();

	//move controls
	for (int i = 0; i < controls.GetSize(); i++)
	{
		controls[i]->SetPositionY(pos_new.y);
	}
}

void ButtonPanel::Enabled(bool value)
{
	//enable or disable button panel

	sbs->EnableMesh(ButtonPanelMesh, value);

	for (int i = 0; i < controls.GetSize(); i++)
	{
		controls[i]->Enabled(value);
	}
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

	//get texture force value
	bool force_enable, force_mode;
	sbs->GetTextureForce(texture, force_enable, force_mode);

	//Call texture autosizing formulas
	if (z1 == z2)
		tw2 = sbs->AutoSize(x1, x2, true, tw, force_enable, force_mode);
	if (x1 == x2)
		tw2 = sbs->AutoSize(z1, z2, true, tw, force_enable, force_mode);
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
		tw2 = sbs->AutoSize(0, sqrt(pow(tempw1, 2) + pow(tempw2, 2)), true, tw, force_enable, force_mode);
	}
	th2 = sbs->AutoSize(0, height1, false, th, force_enable, force_mode);

	return sbs->AddWallMain(ButtonPanelMesh, name, texture, thickness, Origin.x + x1, Origin.z + z1, Origin.x + x2, Origin.z + z2, height1, height2, Origin.y + voffset1, Origin.y + voffset2, tw2, th2);
}

Control* ButtonPanel::GetControl(int index)
{
	//return pointer to control object
	return controls[index];
}

void ButtonPanel::ChangeLight(int floor, bool value)
{
	//change light status for all buttons that call the specified floor

	csString floornum;
	floornum = floor;
	for (int i = 0; i < controls.GetSize(); i++)
	{
		if (controls[i]->ActionName == floornum)
			controls[i]->ChangeLight(value);
	}
}
