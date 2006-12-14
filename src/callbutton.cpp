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

	//create panel
	if (ShowBack == true)
	{
		if (Direction == "front" || Direction == "back")
			sbs->AddWallMain(CallButton_state, "Panel", BackTexture, CenterX - (BackWidth / 2), CenterZ, CenterX + (BackWidth / 2), CenterZ, BackHeight, BackHeight, sbs->FloorArray[floor]->Altitude + voffset, sbs->FloorArray[floor]->Altitude + voffset, tw, th, false, false, false);
		else
			sbs->AddWallMain(CallButton_state, "Panel", BackTexture, CenterX, CenterZ - (BackWidth / 2), CenterX, CenterZ + (BackWidth / 2), BackHeight, BackHeight, sbs->FloorArray[floor]->Altitude + voffset, sbs->FloorArray[floor]->Altitude + voffset, tw, th, false, false, false);
	}

	//create buttons
	if (Direction == "front" || Direction == "back")
	{
		bool reverse;
		int offset;
		int index;
		if (Direction == "front")
		{
			reverse = true;
			offset = -0.1;
		}
		else
		{
			reverse = false;
			offset = 0.1;
		}
		index = sbs->AddWallMain(CallButton_state, "Up", UpButtonTexture, CenterX - (BackWidth / 4), CenterZ + offset, CenterX + (BackWidth / 4), CenterZ + offset, BackHeight / 4, BackHeight / 4, sbs->FloorArray[floor]->Altitude + voffset + (BackHeight * 0.75), sbs->FloorArray[floor]->Altitude + voffset + (BackHeight * 0.75), 1, 1, reverse, false, false);
		index = sbs->AddWallMain(CallButton_state, "Down", DownButtonTexture, CenterX - (BackWidth / 4), CenterZ + offset, CenterX + (BackWidth / 4), CenterZ + offset, BackHeight / 4, BackHeight / 4, sbs->FloorArray[floor]->Altitude + voffset + (BackHeight * 0.25), sbs->FloorArray[floor]->Altitude + voffset + (BackHeight * 0.25), 1, 1, reverse, false, false);
	}
	else
	{
		bool reverse;
		int offset;
		int index;
		if (Direction == "left")
		{
			reverse = true;
			offset = -0.1;
		}
		else
		{
			reverse = false;
			offset = 0.1;
		}
		index = sbs->AddWallMain(CallButton_state, "Up", UpButtonTexture, CenterX + offset, CenterZ - (BackWidth / 4), CenterX + offset, CenterZ + (BackWidth / 4), BackHeight / 4, BackHeight / 4, sbs->FloorArray[floor]->Altitude + voffset + (BackHeight * 0.75), sbs->FloorArray[floor]->Altitude + voffset + (BackHeight * 0.75), 1, 1, false, false, reverse);
		index = sbs->AddWallMain(CallButton_state, "Down", DownButtonTexture, CenterX + offset, CenterZ - (BackWidth / 4), CenterX + offset, CenterZ + (BackWidth / 4), BackHeight / 4, BackHeight / 4, sbs->FloorArray[floor]->Altitude + voffset + (BackHeight * 0.25), sbs->FloorArray[floor]->Altitude + voffset + (BackHeight * 0.25), 1, 1, false, false, reverse);
	}
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

	int closest = sbs->ElevatorArray[Elevators[0]]->GetFloor();
	int closest_elev = 0;

	for (int i = Elevators[0]; i < Elevators.GetSize(); i++)
	{
		int current = sbs->ElevatorArray[Elevators[i]]->GetFloor();

		//if elevator is closer than the previously checked one
		if (abs(current) < closest)
		{
			//and if it's above the current floor and should be called down, or below the
			//current floor and called up
			if ((current > floor && direction == -1) || (current < floor && direction == 1))
			{
				//and if it's either going the same direction as the call or not moving at all
				if (sbs->ElevatorArray[Elevators[i]]->Direction == direction || sbs->ElevatorArray[Elevators[i]]->Direction == 0)
				{
					closest = current;
					closest_elev = i;
				}
			}
		}
	}

	if (sbs->ElevatorArray[Elevators[closest_elev]]->GetFloor() == floor)
		//open elevator if it's on the same floor
		sbs->ElevatorArray[Elevators[closest_elev]]->OpenDoors();
	else
		//otherwise add a route entry to this floor
		sbs->ElevatorArray[Elevators[closest_elev]]->AddRoute(floor, direction);
}

void CallButton::Press(csVector3 isect)
{
	//check clicked location against button polygons

	if (CallButton_state->PointOnPolygon(CallButton_state->FindPolygonByName("Up"), isect) == true)
		Call (1);
	if (CallButton_state->PointOnPolygon(CallButton_state->FindPolygonByName("Down"), isect) == true)
		Call (-1);
}
