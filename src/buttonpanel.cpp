/* $Id$ */

/*
	Scalable Building Simulator - Elevator Button Panel Class
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
#include "buttonpanel.h"
#include "elevator.h"
#include "camera.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

ButtonPanel::ButtonPanel(int _elevator, int index, const char *texture, int rows, int columns, const char *direction, float CenterX, float CenterZ, float buttonwidth, float buttonheight, float spacingX, float spacingY, float voffset, float tw, float th)
{
	//Create an elevator button panel
	//index is for specifying multiple panels within the same elevator

	//set up SBS object
	object = new Object();
	object->SetValues(this, sbs->GetElevator(_elevator)->object, "ButtonPanel", false);

	IsEnabled = true;
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
	delete object;
}

void ButtonPanel::AddButton(const char *sound, const char *texture, const char *texture_lit, int row, int column, const char *type, float width, float height, float hoffset, float voffset)
{
	//create a standard button at specified row/column position
	//width and height are the button size percentage that the button should be (divided by 100); default is 1 for each, aka 100%.
	//hoffset and voffset are the optional horizontal and vertical offsets to place the button (in order to place it outside of the default grid position)

	//type is one of these:
	//floor number
	//open = Open Doors
	//close = Close Doors
	//cancel = Call Cancel
	//stop = Stop
	//alarm = Alarm

	csArray<csString> textures, names;
	int positions = 1;
	textures.Push(texture);
	textures.Push(texture_lit);
	if (IsNumeric(type) == true)
	{
		names.Push("off");
		names.Push(type);
	}
	else
		names.Push(type);
	AddControl(sound, row, column, width, height, hoffset, voffset, names, textures);

}

void ButtonPanel::AddControl(const char *sound, int row, int column, float bwidth, float bheight, float hoffset, float voffset, csArray<csString> action_names, csArray<csString> &textures)
{
	//create an elevator control (button, switch, knob)

	//Supported action names:
	//Off
	//(floor number)
	//Open = Open Doors
	//Close = Close Doors
	//Cancel = Call Cancel
	//Stop
	//Alarm
	//Fire1Off
	//Fire1On
	//Fire1Bypass
	//Fire2Off
	//Fire2On
	//Fire2Hold
	//UpPeakOn
	//UpPeakOff
	//DownPeakOn
	//DownPeakOff
	//IndOn
	//IndOff
	//InsOn
	//InsOff
	//AcpOn
	//AcpOff
	//FanOn
	//FanOff

	float xpos = 0, ypos = 0, zpos = 0;

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
	controls[control_index] = new Control(this->object, buffer, sound, action_names, textures, Direction, ButtonWidth * bwidth, ButtonHeight * bheight, ypos);
	//move control
	controls[control_index]->SetPosition(csVector3(xpos, sbs->GetElevator(elevator)->GetPosition().y, zpos));
}

void ButtonPanel::DeleteButton(int row, int column)
{

}

void ButtonPanel::Press(int index)
{
	//Press selected button

	if (index == -1)
		return;

	Elevator *elev = sbs->GetElevator(elevator);

	//exit if index is invalid
	if (index < 0 || index > controls.GetSize() - 1)
		return;

	//play button sound
	controls[index]->PlaySound();

	//get action name of next position state
	csString name = controls[index]->GetPositionAction(controls[index]->GetNextSelectPosition());

	if (IsNumeric(name) == true)
	{
		//exit if in inspection mode or in fire service phase 1 mode
		if (elev->InspectionService == true || elev->FireServicePhase1 == 1)
			return;

		int floor = atoi(name);
		int elev_floor = elev->GetFloor();

		//if elevator is processing a queue, add floor to the queue (if auto queue resets are active)
		if (elev->IsQueueActive() && elev->QueueResets == true)
			elev->AddRoute(floor, elev->QueuePositionDirection, true);
		else
		{
			//elevator is above floor
			if (elev_floor > floor)
				elev->AddRoute(floor, -1, true);

			//elevator is below floor
			if (elev_floor < floor)
				elev->AddRoute(floor, 1, true);

			//elevator is on floor
			if (elev_floor == floor)
			{
				if (elev->Direction == 0)
				{
					//stopped - play chime and open doors
					if (elev->InServiceMode() == false)
					{
						if (elev->LastQueueDirection == -1)
							elev->Chime(0, floor, false);
						else if (elev->LastQueueDirection == 1)
							elev->Chime(0, floor, true);
					}
					if (elev->FireServicePhase2 == 0)
						elev->OpenDoors();
				}
				else
				{
					//add a route to the current floor if elevator is moving
					elev->AddRoute(floor, -elev->Direction, true);
				}
			}
		}

		return;
	}

	name.Downcase();
	if (name == "off")
		return;

	//change to next control position
	controls[index]->NextSelectPosition();

	if (name.StartsWith("open", false) == true && elev->Direction == 0)
	{
		int number = 0;
		if (name.Length() > 4)
			number = atoi(name.Slice(4, name.Length() - 4));
		elev->OpenDoors(number);
	}
	if (name.StartsWith("close", false) == true && elev->Direction == 0)
	{
		int number = 0;
		if (name.Length() > 5)
			number = atoi(name.Slice(5, name.Length() - 5));
		elev->CloseDoors(number);
	}
	if (name == "cancel")
		elev->CancelLastRoute();
	//if (name == "run")
	if (name == "stop")
		elev->StopElevator();
	if (name == "alarm")
		elev->Alarm();
	if (name == "fire1off")
		elev->EnableFireService1(0);
	if (name == "fire1on")
		elev->EnableFireService1(1);
	if (name == "fire1bypass")
		elev->EnableFireService1(2);
	if (name == "fire2off")
		elev->EnableFireService2(0);
	if (name == "fire2on")
		elev->EnableFireService2(1);
	if (name == "fire2hold")
		elev->EnableFireService2(2);
	if (name == "uppeakon")
		elev->EnableUpPeak(true);
	if (name == "uppeakoff")
		elev->EnableUpPeak(false);
	if (name == "downpeakon")
		elev->EnableDownPeak(true);
	if (name == "downpeakoff")
		elev->EnableDownPeak(false);
	if (name == "indon")
		elev->EnableIndependentService(true);
	if (name == "indoff")
		elev->EnableIndependentService(false);
	if (name == "inson")
		elev->EnableInspectionService(true);
	if (name == "insoff")
		elev->EnableInspectionService(false);
	if (name == "acpon")
		elev->EnableACP(true);
	if (name == "acpoff")
		elev->EnableACP(false);
	if (name == "fanon")
		elev->Fan = true;
	if (name == "fanoff")
		elev->Fan = false;
	if (name.StartsWith("hold", false) == true && elev->Direction == 0)
	{
		int number = 0;
		if (name.Length() > 4)
			number = atoi(name.Slice(4, name.Length() - 4));
		elev->HoldDoors(number);
	}
}

void ButtonPanel::Move(const csVector3 &position)
{
	//relative movement
	ButtonPanelMesh->GetMovable()->MovePosition(sbs->ToRemote(position));
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

	if (IsEnabled == value)
		return;

	sbs->EnableMesh(ButtonPanelMesh, value);

	for (int i = 0; i < controls.GetSize(); i++)
	{
		controls[i]->Enabled(value);
	}
	IsEnabled = value;
}

int ButtonPanel::AddWall(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th)
{
	//Adds a wall with the specified dimensions

	//Set horizontal scaling
	x1 = x1 * sbs->HorizScale;
	x2 = x2 * sbs->HorizScale;
	z1 = z1 * sbs->HorizScale;
	z2 = z2 * sbs->HorizScale;

	//calculate autosizing
	float tmpheight;
	if (height1 > height2)
		tmpheight = height1;
	else
		tmpheight = height2;
	csVector2 sizing = sbs->CalculateSizing(texture, csVector2(x1, x2), csVector2(0, tmpheight), csVector2(z1, z2), tw, th);

	return sbs->AddWallMain(object, ButtonPanelMesh, name, texture, thickness, Origin.x + x1, Origin.z + z1, Origin.x + x2, Origin.z + z2, height1, height2, Origin.y + voffset1, Origin.y + voffset2, sizing.x, sizing.y);
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
		int index = controls[i]->FindActionPosition(floornum);
		int index2 = controls[i]->FindActionPosition("off");
		if (index > 0 && index2 > 0)
		{
			if (value == false)
				controls[i]->ChangeSelectPosition(index2);
			else
				controls[i]->ChangeSelectPosition(index);
		}
	}
}

int ButtonPanel::GetFloorButtonIndex(int floor)
{
	//return the index number of a floor button

	csString floornum;
	floornum = floor;

	if (controls.GetSize() > 0)
	{
		for (int i = 0; i < controls.GetSize(); i++)
		{
			if (controls[i]->FindActionPosition(floornum))
				return i;
		}
	}
	return -1;
}
