/* $Id$ */

/*
	Scalable Building Simulator - Call Button Subsystem Class
	The Skyscraper Project - Version 1.2 Alpha
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
#include "callbutton.h"
#include "sbs.h"
#include "camera.h"
#include "elevator.h"

extern SBS *sbs; //external pointer to the SBS engine

CallButton::CallButton(csArray<int> &elevators, int floornum, int number, const char *BackTexture, const char *UpButtonTexture, const char *DownButtonTexture, float CenterX, float CenterZ, float voffset, const char *direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th)
{
	//create a set of call buttons

	IsEnabled = true;
	Elevators.SetSize(elevators.GetSize());
	for (size_t i = 0; i < elevators.GetSize(); i++)
		Elevators[i] = elevators[i];

	//create object mesh
	csString buffer, buffer2, buffer3;
	buffer2 = floornum;
	buffer3 = number;
	buffer = "Call Button " + buffer2 + ":" + buffer3;
	buffer.Trim();
	CallButtonMesh = sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData());
	CallButton_state = scfQueryInterface<iThingFactoryState> (CallButtonMesh->GetMeshObject()->GetFactory());
	CallButtonMesh->SetZBufMode(CS_ZBUF_USE);
	CallButtonMesh->SetRenderPriority(sbs->engine->GetAlphaRenderPriority());
	CallButtonMesh->GetMeshObject()->SetMixMode(CS_FX_ALPHA);

	//set variables
	floor = floornum;
	Number = number;
	Direction = direction;
	Direction.Downcase();
	Direction.Trim();

	sbs->ReverseExtents(false, false, false);

	//create panel
	if (ShowBack == true)
	{
		if (Direction == "front" || Direction == "back")
		{
			if (Direction == "front")
				sbs->DrawWalls(true, false, false, false, false, false);
			else
				sbs->DrawWalls(false, true, false, false, false, false);
			sbs->AddWallMain(CallButton_state, "Panel", BackTexture, 0, CenterX - (BackWidth / 2), CenterZ, CenterX + (BackWidth / 2), CenterZ, BackHeight, BackHeight, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset, tw, th);
			sbs->ResetWalls();
		}
		if (Direction == "left" || Direction == "right")
		{
			if (Direction == "left")
				sbs->DrawWalls(true, false, false, false, false, false);
			else
				sbs->DrawWalls(false, true, false, false, false, false);
			sbs->AddWallMain(CallButton_state, "Panel", BackTexture, 0, CenterX, CenterZ + (BackWidth / 2), CenterX, CenterZ - (BackWidth / 2), BackHeight, BackHeight, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset, tw, th);
			sbs->ResetWalls();
		}
	}

	//create buttons
	int bottomfloor = sbs->GetElevator(Elevators[0])->ServicedFloors[0];
	int topfloor = sbs->GetElevator(Elevators[0])->ServicedFloors[sbs->GetElevator(Elevators[0])->ServicedFloors.GetSize() - 1];

	if (Direction == "front" || Direction == "back")
	{
		float offset;
		if (Direction == "front")
		{
			sbs->DrawWalls(true, false, false, false, false, false);
			offset = -0.01f;
		}
		else
		{
			sbs->DrawWalls(false, true, false, false, false, false);
			offset = 0.01f;
		}
		if (floornum > bottomfloor && floornum < topfloor)
		{
			sbs->AddWallMain(CallButton_state, "Up", UpButtonTexture, 0, CenterX - (BackWidth / 4), CenterZ + offset, CenterX + (BackWidth / 4), CenterZ + offset, (BackHeight / 7) * 2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + ((BackHeight / 7) * 4), sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + ((BackHeight / 7) * 4), 1, 1);
			sbs->AddWallMain(CallButton_state, "Down", DownButtonTexture, 0, CenterX - (BackWidth / 4), CenterZ + offset, CenterX + (BackWidth / 4), CenterZ + offset, (BackHeight / 7) * 2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (BackHeight / 7), sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (BackHeight / 7), 1, 1);
		}
		else
		{
			if (floornum < topfloor)
				sbs->AddWallMain(CallButton_state, "Up", UpButtonTexture, 0, CenterX - (BackWidth / 4), CenterZ + offset, CenterX + (BackWidth / 4), CenterZ + offset, (BackHeight / 7) * 2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (((BackHeight / 7) * 3) - (BackHeight / 14)), sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (((BackHeight / 7) * 3) - (BackHeight / 14)), 1, 1);
			if (floornum > bottomfloor)
				sbs->AddWallMain(CallButton_state, "Down", DownButtonTexture, 0, CenterX - (BackWidth / 4), CenterZ + offset, CenterX + (BackWidth / 4), CenterZ + offset, (BackHeight / 7) * 2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (((BackHeight / 7) * 3) - (BackHeight / 14)), sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (((BackHeight / 7) * 3) - (BackHeight / 14)), 1, 1);
		}

		sbs->ResetWalls();
	}
	else
	{
		float offset;
		if (Direction == "left")
		{
			sbs->DrawWalls(true, false, false, false, false, false);
			offset = -0.01f;
		}
		else
		{
			//right
			sbs->DrawWalls(false, true, false, false, false, false);
			offset = 0.01f;
		}
		if (floornum > bottomfloor && floornum < topfloor)
		{
			sbs->AddWallMain(CallButton_state, "Up", UpButtonTexture, 0, CenterX + offset, CenterZ - (BackWidth / 4), CenterX + offset, CenterZ + (BackWidth / 4), (BackHeight / 7) * 2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + ((BackHeight / 7) * 4), sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + ((BackHeight / 7) * 4), 1, 1);
			sbs->AddWallMain(CallButton_state, "Down", DownButtonTexture, 0, CenterX + offset, CenterZ - (BackWidth / 4), CenterX + offset, CenterZ + (BackWidth / 4), (BackHeight / 7) * 2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (BackHeight / 7), sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (BackHeight / 7), 1, 1);
		}
		else
		{
			if (floornum < topfloor)
				sbs->AddWallMain(CallButton_state, "Up", UpButtonTexture, 0, CenterX + offset, CenterZ - (BackWidth / 4), CenterX + offset, CenterZ + (BackWidth / 4), (BackHeight / 7) * 2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (((BackHeight / 7) * 3) - (BackHeight / 14)), sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (((BackHeight / 7) * 3) - (BackHeight / 14)), 1, 1);
			if (floornum > bottomfloor)
				sbs->AddWallMain(CallButton_state, "Down", DownButtonTexture, 0, CenterX + offset, CenterZ - (BackWidth / 4), CenterX + offset, CenterZ + (BackWidth / 4), (BackHeight / 7) * 2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (((BackHeight / 7) * 3) - (BackHeight / 14)), sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (((BackHeight / 7) * 3) - (BackHeight / 14)), 1, 1);
		}

		sbs->ResetWalls();
	}
	sbs->ResetExtents();
}

CallButton::~CallButton()
{

}

void CallButton::Enabled(bool value)
{
	//turns panel on/off
	if (value == true)
	{
		CallButtonMesh->GetFlags().Reset (CS_ENTITY_INVISIBLEMESH);
		CallButtonMesh->GetFlags().Reset (CS_ENTITY_NOSHADOWS);
		CallButtonMesh->GetFlags().Reset (CS_ENTITY_NOHITBEAM);
		IsEnabled = true;
	}
	else
	{
		CallButtonMesh->GetFlags().Set (CS_ENTITY_INVISIBLEMESH);
		CallButtonMesh->GetFlags().Set (CS_ENTITY_NOSHADOWS);
		CallButtonMesh->GetFlags().Set (CS_ENTITY_NOHITBEAM);
		IsEnabled = false;
	}
}

void CallButton::Call(int direction)
{
	//calls the closest elevator in the elevator array list to the current floor,
	//and also depending on the direction it's travelling

	int closest = sbs->GetElevator(Elevators[0])->GetFloor();
	int closest_elev = 0;

	for (size_t i = Elevators[0]; i < Elevators.GetSize(); i++)
	{
		int current = sbs->GetElevator(Elevators[i])->GetFloor();

		//if elevator is closer than the previously checked one
		if (abs(current - floor) < closest)
		{
			//and if it's above the current floor and should be called down, or below the
			//current floor and called up
			if ((current > floor && direction == -1) || (current < floor && direction == 1))
			{
				//and if it's either going the same direction as the call or not moving at all
				if (sbs->GetElevator(Elevators[i])->Direction == direction || sbs->GetElevator(Elevators[i])->Direction == 0)
				{
					closest = current;
					closest_elev = i;
				}
			}
		}
	}

	if (sbs->GetElevator(Elevators[closest_elev])->GetFloor() == floor)
		//open elevator if it's on the same floor
		sbs->GetElevator(Elevators[closest_elev])->OpenDoors();
	else
		//otherwise add a route entry to this floor
		sbs->GetElevator(Elevators[closest_elev])->AddRoute(floor, direction);
}

void CallButton::Press(int index)
{
	//Press selected button

	if (index == -1)
		return;

	csString name = CallButton_state->GetPolygonName(index);
	if (name.Slice(0, 2) == "Up")
		Call(1);
	if (name.Slice(0, 4) == "Down")
		Call(-1);
}
