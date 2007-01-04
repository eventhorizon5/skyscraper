/* $Id$ */

/*
	Scalable Building Simulator - Call Button Subsystem Class
	The Skyscraper Project - Version 1.1 Alpha
	Copyright (C)2005-2006 Ryan Thoryk
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

#include <crystalspace.h>
#include "callbutton.h"
#include "sbs.h"
#include "camera.h"
#include "elevator.h"

extern SBS *sbs; //external pointer to the SBS engine

CallButton::CallButton(csArray<int> &elevators, int floornum, int number, const char *BackTexture, const char *UpButtonTexture, const char *DownButtonTexture, double CenterX, double CenterZ, double voffset, const char *direction, double BackWidth, double BackHeight, bool ShowBack, double tw, double th)
{
	//create a set of call buttons

	IsEnabled = true;
	Elevators.SetSize(elevators.GetSize());
	for (int i = 0; i < elevators.GetSize(); i++)
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

	//set variables
	floor = floornum;
	Number = number;
	Direction = direction;
	Direction.Downcase();
	Direction.Trim();

	sbs->DrawWalls(true, false, false, false, false, false);
	sbs->ReverseExtents(false, false, false);

	//create panel
	if (ShowBack == true)
	{
		if (Direction == "front")
			sbs->AddWallMain(CallButton_state, "Panel", BackTexture, 0, CenterX - (BackWidth / 2), CenterZ, CenterX + (BackWidth / 2), CenterZ, BackHeight, BackHeight, sbs->GetFloor(floor)->Altitude + voffset, sbs->GetFloor(floor)->Altitude + voffset, tw, th);
		if (Direction == "back")
			sbs->AddWallMain(CallButton_state, "Panel", BackTexture, 0, CenterX + (BackWidth / 2), CenterZ, CenterX - (BackWidth / 2), CenterZ, BackHeight, BackHeight, sbs->GetFloor(floor)->Altitude + voffset, sbs->GetFloor(floor)->Altitude + voffset, tw, th);
		if (Direction == "left")
			sbs->AddWallMain(CallButton_state, "Panel", BackTexture, 0, CenterX, CenterZ + (BackWidth / 2), CenterX, CenterZ - (BackWidth / 2), BackHeight, BackHeight, sbs->GetFloor(floor)->Altitude + voffset, sbs->GetFloor(floor)->Altitude + voffset, tw, th);
		if (Direction == "right")
			sbs->AddWallMain(CallButton_state, "Panel", BackTexture, 0, CenterX, CenterZ - (BackWidth / 2), CenterX, CenterZ + (BackWidth / 2), BackHeight, BackHeight, sbs->GetFloor(floor)->Altitude + voffset, sbs->GetFloor(floor)->Altitude + voffset, tw, th);
	}

	//create buttons
	if (Direction == "front" || Direction == "back")
	{
		double offset;
		int index;
		if (Direction == "front")
		{
			offset = -0.01;
			index = sbs->AddWallMain(CallButton_state, "Up", UpButtonTexture, 0, CenterX - (BackWidth / 4), CenterZ + offset, CenterX + (BackWidth / 4), CenterZ + offset, BackHeight / 4, BackHeight / 4, sbs->GetFloor(floor)->Altitude + voffset + (BackHeight * 0.75), sbs->GetFloor(floor)->Altitude + voffset + (BackHeight * 0.75), 1, 1);
			index = sbs->AddWallMain(CallButton_state, "Down", DownButtonTexture, 0, CenterX - (BackWidth / 4), CenterZ + offset, CenterX + (BackWidth / 4), CenterZ + offset, BackHeight / 4, BackHeight / 4, sbs->GetFloor(floor)->Altitude + voffset + (BackHeight * 0.25), sbs->GetFloor(floor)->Altitude + voffset + (BackHeight * 0.25), 1, 1);
		}
		else
		{
			//back
			offset = 0.01;
			index = sbs->AddWallMain(CallButton_state, "Up", UpButtonTexture, 0, CenterX + (BackWidth / 4), CenterZ + offset, CenterX - (BackWidth / 4), CenterZ + offset, BackHeight / 4, BackHeight / 4, sbs->GetFloor(floor)->Altitude + voffset + (BackHeight * 0.75), sbs->GetFloor(floor)->Altitude + voffset + (BackHeight * 0.75), 1, 1);
			index = sbs->AddWallMain(CallButton_state, "Down", DownButtonTexture, 0, CenterX + (BackWidth / 4), CenterZ + offset, CenterX - (BackWidth / 4), CenterZ + offset, BackHeight / 4, BackHeight / 4, sbs->GetFloor(floor)->Altitude + voffset + (BackHeight * 0.25), sbs->GetFloor(floor)->Altitude + voffset + (BackHeight * 0.25), 1, 1);
		}
	}
	else
	{
		double offset;
		int index;
		if (Direction == "left")
		{
			offset = -0.01;
			index = sbs->AddWallMain(CallButton_state, "Up", UpButtonTexture, 0, CenterX + offset, CenterZ + (BackWidth / 4), CenterX + offset, CenterZ - (BackWidth / 4), BackHeight / 4, BackHeight / 4, sbs->GetFloor(floor)->Altitude + voffset + (BackHeight * 0.75), sbs->GetFloor(floor)->Altitude + voffset + (BackHeight * 0.75), 1, 1);
			index = sbs->AddWallMain(CallButton_state, "Down", DownButtonTexture, 0, CenterX + offset, CenterZ + (BackWidth / 4), CenterX + offset, CenterZ - (BackWidth / 4), BackHeight / 4, BackHeight / 4, sbs->GetFloor(floor)->Altitude + voffset + (BackHeight * 0.25), sbs->GetFloor(floor)->Altitude + voffset + (BackHeight * 0.25), 1, 1);
		}
		else
		{
			//right
			offset = 0.01;
			index = sbs->AddWallMain(CallButton_state, "Up", UpButtonTexture, 0, CenterX + offset, CenterZ - (BackWidth / 4), CenterX + offset, CenterZ + (BackWidth / 4), BackHeight / 4, BackHeight / 4, sbs->GetFloor(floor)->Altitude + voffset + (BackHeight * 0.75), sbs->GetFloor(floor)->Altitude + voffset + (BackHeight * 0.75), 1, 1);
			index = sbs->AddWallMain(CallButton_state, "Down", DownButtonTexture, 0, CenterX + offset, CenterZ - (BackWidth / 4), CenterX + offset, CenterZ + (BackWidth / 4), BackHeight / 4, BackHeight / 4, sbs->GetFloor(floor)->Altitude + voffset + (BackHeight * 0.25), sbs->GetFloor(floor)->Altitude + voffset + (BackHeight * 0.25), 1, 1);
		}
	}
	sbs->ResetWalls();
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

	for (int i = Elevators[0]; i < Elevators.GetSize(); i++)
	{
		int current = sbs->GetElevator(Elevators[i])->GetFloor();

		//if elevator is closer than the previously checked one
		if (abs(current) < closest)
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
	if (name == "Up")
		Call (1);
	if (name == "Down")
		Call (-1);
}
