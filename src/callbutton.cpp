/* $Id$ */

/*
	Scalable Building Simulator - Call Button Subsystem Class
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
#include "callbutton.h"
#include "sbs.h"
#include "camera.h"
#include "elevator.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

CallButton::CallButton(csArray<int> &elevators, int floornum, int number, const char *BackTexture, const char *UpButtonTexture, const char *UpButtonTexture_Lit, const char *DownButtonTexture, const char *DownButtonTexture_Lit, float CenterX, float CenterZ, float voffset, const char *direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th)
{
	//create a set of call buttons

	//set up SBS object
	object = new Object();
	object->SetValues(this, sbs->GetFloor(floornum)->object, "CallButton", false);

	IsEnabled = true;
	Elevators.SetSize(elevators.GetSize());
	for (size_t i = 0; i < elevators.GetSize(); i++)
		Elevators[i] = elevators[i];

	//save texture names
	UpTexture = UpButtonTexture;
	UpTextureLit = UpButtonTexture_Lit;
	DownTexture = DownButtonTexture;
	DownTextureLit = DownButtonTexture_Lit;

	UpStatus = false;
	DownStatus = false;
	ProcessedUp = false;
	ProcessedDown = false;

	//create object mesh
	csString buffer, buffer2, buffer3;
	buffer2 = floornum;
	buffer3 = number;
	buffer = "Call Panel " + buffer2 + ":" + buffer3;
	buffer.Trim();
	CallButtonBackMesh = sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData());
	CallButton_back_state = scfQueryInterface<iThingFactoryState> (CallButtonBackMesh->GetMeshObject()->GetFactory());
	CallButtonBackMesh->SetZBufMode(CS_ZBUF_USE);
	CallButtonBackMesh->SetRenderPriority(sbs->engine->GetObjectRenderPriority());

	buffer = "Call Button " + buffer2 + ":" + buffer3 + ":Up";
	CallButtonMeshUp = CS::Geometry::GeneralMeshBuilder::CreateFactoryAndMesh(sbs->engine, sbs->area, buffer, buffer + " factory");
	CallButtonMeshUp->SetZBufMode(CS_ZBUF_USE);
	CallButtonMeshUp->SetRenderPriority(sbs->engine->GetObjectRenderPriority());
	csRef<iMaterialWrapper> mat = sbs->engine->GetMaterialList()->FindByName(UpTexture);
	CallButtonMeshUp->GetMeshObject()->SetMaterialWrapper(mat);

	buffer = "Call Button " + buffer2 + ":" + buffer3 + ":Down";
	CallButtonMeshDown = CS::Geometry::GeneralMeshBuilder::CreateFactoryAndMesh(sbs->engine, sbs->area, buffer, buffer + " factory");
	CallButtonMeshDown->SetZBufMode(CS_ZBUF_USE);
	CallButtonMeshDown->SetRenderPriority(sbs->engine->GetObjectRenderPriority());
	mat = sbs->engine->GetMaterialList()->FindByName(DownTexture);
	CallButtonMeshDown->GetMeshObject()->SetMaterialWrapper(mat);

	//set variables
	floor = floornum;
	Number = number;
	Direction = direction;
	Direction.Downcase();
	Direction.Trim();

	if (sbs->Verbose)
		Report("Created");

	sbs->ResetTextureMapping(true);

	//create panel
	if (ShowBack == true)
	{
		if (sbs->Verbose)
			Report("Creating back panel");

		if (Direction == "front" || Direction == "back")
		{
			if (Direction == "front")
				sbs->DrawWalls(true, false, false, false, false, false);
			else
				sbs->DrawWalls(false, true, false, false, false, false);
			sbs->AddWallMain(CallButtonBackMesh, "Panel", BackTexture, 0, CenterX - (BackWidth / 2), CenterZ, CenterX + (BackWidth / 2), CenterZ, BackHeight, BackHeight, sbs->GetFloor(floor)->GetBase() + voffset, sbs->GetFloor(floor)->GetBase() + voffset, tw, th);
			sbs->ResetWalls();
		}
		if (Direction == "left" || Direction == "right")
		{
			if (Direction == "left")
				sbs->DrawWalls(true, false, false, false, false, false);
			else
				sbs->DrawWalls(false, true, false, false, false, false);
			sbs->AddWallMain(CallButtonBackMesh, "Panel", BackTexture, 0, CenterX, CenterZ + (BackWidth / 2), CenterX, CenterZ - (BackWidth / 2), BackHeight, BackHeight, sbs->GetFloor(floor)->GetBase() + voffset, sbs->GetFloor(floor)->GetBase() + voffset, tw, th);
			sbs->ResetWalls();
		}
	}

	//create buttons
	int bottomfloor = sbs->GetElevator(Elevators[0])->GetBottomFloor();
	int topfloor = sbs->GetElevator(Elevators[0])->GetTopFloor();

	if (sbs->Verbose)
		Report("Creating buttons");

	if (Direction == "front" || Direction == "back")
	{
		float x1, x2;
		float offset;
		if (Direction == "front")
		{
			offset = -0.01f;
			x1 = CenterX - (BackWidth / 4);
			x2 = CenterX + (BackWidth / 4);
		}
		else
		{
			offset = 0.01f;
			x2 = CenterX - (BackWidth / 4);
			x1 = CenterX + (BackWidth / 4);
		}

		if (floornum > bottomfloor && floornum < topfloor)
		{
			sbs->AddGenWall(CallButtonMeshUp, UpButtonTexture, x1, CenterZ + offset, x2, CenterZ + offset, (BackHeight / 7) * 2, sbs->GetFloor(floor)->GetBase() + voffset + ((BackHeight / 7) * 4), 1, 1);
			sbs->AddGenWall(CallButtonMeshDown, DownButtonTexture, x1, CenterZ + offset, x2, CenterZ + offset, (BackHeight / 7) * 2, sbs->GetFloor(floor)->GetBase() + voffset + (BackHeight / 7), 1, 1);
		}
		else
		{
			if (floornum < topfloor)
				sbs->AddGenWall(CallButtonMeshUp, UpButtonTexture, x1, CenterZ + offset, x2, CenterZ + offset, (BackHeight / 7) * 2, sbs->GetFloor(floor)->GetBase() + voffset + (((BackHeight / 7) * 3) - (BackHeight / 14)), 1, 1);
			if (floornum > bottomfloor)
				sbs->AddGenWall(CallButtonMeshDown, DownButtonTexture, x1, CenterZ + offset, x2, CenterZ + offset, (BackHeight / 7) * 2, sbs->GetFloor(floor)->GetBase() + voffset + (((BackHeight / 7) * 3) - (BackHeight / 14)), 1, 1);
		}

		sbs->ResetWalls();
	}
	else
	{
		float z1, z2;
		float offset;
		if (Direction == "left")
		{
			offset = -0.01f;
			z2 = CenterZ - (BackWidth / 4);
			z1 = CenterZ + (BackWidth / 4);
		}
		else
		{
			offset = 0.01f;
			z1 = CenterZ - (BackWidth / 4);
			z2 = CenterZ + (BackWidth / 4);
		}
		if (floornum > bottomfloor && floornum < topfloor)
		{
			sbs->AddGenWall(CallButtonMeshUp, UpButtonTexture, CenterX + offset, z1, CenterX + offset, z2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->GetBase() + voffset + ((BackHeight / 7) * 4), 1, 1);
			sbs->AddGenWall(CallButtonMeshDown, DownButtonTexture, CenterX + offset, z1, CenterX + offset, z2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->GetBase() + voffset + (BackHeight / 7), 1, 1);
		}
		else
		{
			if (floornum < topfloor)
				sbs->AddGenWall(CallButtonMeshUp, UpButtonTexture, CenterX + offset, z1, CenterX + offset, z2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->GetBase() + voffset + (((BackHeight / 7) * 3) - (BackHeight / 14)), 1, 1);
			if (floornum > bottomfloor)
				sbs->AddGenWall(CallButtonMeshDown, DownButtonTexture, CenterX + offset, z1, CenterX + offset, z2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->GetBase() + voffset + (((BackHeight / 7) * 3) - (BackHeight / 14)), 1, 1);
		}

		sbs->ResetWalls();
	}
	sbs->ResetTextureMapping();
}

CallButton::~CallButton()
{
	CallButton_back_state = 0;
	CallButtonMeshDown = 0;
	CallButtonMeshUp = 0;
	CallButtonBackMesh = 0;
	delete object;
}

void CallButton::Enabled(bool value)
{
	//turns call buttons on/off
	if (IsEnabled == value)
		return;

	sbs->EnableMesh(CallButtonBackMesh, value);
	sbs->EnableMesh(CallButtonMeshUp, value);
	sbs->EnableMesh(CallButtonMeshDown, value);
	IsEnabled = value;

	if (sbs->Verbose)
	{
		if (value == true)
			Report("Enabled");
		else
			Report("Disabled");
	}
}

void CallButton::Call(bool direction)
{
	//calls the closest elevator in the elevator array list to the current floor,
	//and also depending on the direction it's traveling

	//exit if call has already been made
	if ((direction == true && UpStatus == true) || (direction == false && DownStatus == true))
	{
		if (sbs->Verbose)
			sbs->Report("Call has already been made");
		return;
	}

	//set light and direction value

	if (sbs->Verbose)
		Report("Call: finding grouped call buttons");

	//this call will return at least this call button
	csArray<int> buttons = sbs->GetFloor(floor)->GetCallButtons(Elevators[0]);

	for (int i = 0; i < buttons.GetSize(); i++)
	{
		if (direction == true)
		{
			sbs->GetFloor(floor)->CallButtonArray[buttons[i]]->UpLight(true);
			ProcessedUp = false;
		}
		else
		{
			sbs->GetFloor(floor)->CallButtonArray[buttons[i]]->DownLight(true);
			ProcessedDown = false;
		}
	}

	//register callback for this button
	if (sbs->Verbose)
		Report("Registering callback");

	sbs->RegisterCallButtonCallback(this);
}

void CallButton::UpLight(bool value)
{
	//turn on the 'up' directional light
	if (value == UpStatus)
	{
		if (sbs->Verbose)
			Report("UpLight: already in requested status");
		return;
	}

	//set light status
	if (value == true)
		SetLights(1, 0);
	else
		SetLights(2, 0);

	UpStatus = value;
}

void CallButton::DownLight(bool value)
{
	//turn on the 'down' directional light
	if (value == DownStatus)
	{
		if (sbs->Verbose)
			Report("DownLight: already in requested status");
		return;
	}

	//set light status
	if (value == true)
		SetLights(0, 1);
	else
		SetLights(0, 2);

	DownStatus = value;
}

void CallButton::SetLights(int up, int down)
{
	//set status of call button lights
	//values are 0 for no change, 1 for on, and 2 for off

	if (up == 1 && CallButtonMeshUp)
	{
		if (sbs->Verbose)
			Report("SetLights: turning on up light");

		sbs->ChangeTexture(CallButtonMeshUp, UpTextureLit);
	}
	if (up == 2 && CallButtonMeshUp)
	{
		if (sbs->Verbose)
			Report("SetLights: turning off up light");

		sbs->ChangeTexture(CallButtonMeshUp, UpTexture);
	}
	if (down == 1 && CallButtonMeshDown)
	{
		if (sbs->Verbose)
			Report("SetLights: turning on down light");

		sbs->ChangeTexture(CallButtonMeshDown, DownTextureLit);
	}
	if (down == 2 && CallButtonMeshDown)
	{
		if (sbs->Verbose)
			Report("SetLights: turning off down light");

		sbs->ChangeTexture(CallButtonMeshDown, DownTexture);
	}

	if (up > 0 && !CallButtonMeshUp && sbs->Verbose)
		Report("SetLights: cannot change up light status");
	if (down > 0 && !CallButtonMeshDown && sbs->Verbose)
		Report("SetLights: cannot change down light status");
}

bool CallButton::ServicesElevator(int elevator)
{
	//return true if this call button services the specified elevator
	for (int i = 0; i < Elevators.GetSize(); i++)
	{
		if (Elevators[i] == elevator)
		{
			if (sbs->Verbose)
				Report("Services elevator " + csString(_itoa(elevator, intbuffer, 10)));
			return true;
		}
	}
	return false;
}

void CallButton::Loop(bool direction)
{
	//call button main loop
	//this function runs for every registered call button via callback
	//direction is the call direction to process

	//first exit if no call button is not processing a call for the current direction
	//or if a call has already been processed
	if ((UpStatus == false && direction == true) || (ProcessedUp == true && direction == true))
		return;
	if ((DownStatus == false && direction == false) || (ProcessedDown == true && direction == false))
		return;

	//initialize values
	int closest = 0;
	int closest_elev = 0;
	int tmpdirection = 0;
	bool check = false;

	if (direction == true)
		tmpdirection = 1;
	else
		tmpdirection = -1;

	if (Elevators.GetSize() > 1)
	{
		//search through elevator list if call button serves more than 1 elevator
		if (sbs->Verbose)
			Report("Finding nearest available elevator...");

		//check each elevator associated with this call button to find the closest available one
		for (size_t i = 0; i < Elevators.GetSize(); i++)
		{
			Elevator *elevator = sbs->GetElevator(Elevators[i]);
			int current = elevator->GetFloor();

			if (sbs->Verbose)
				Report("Checking elevator " + csString(_itoa(elevator->Number, intbuffer, 10)));

			//if elevator is closer than the previously checked one or we're starting the checks
			if (abs(current - floor) < closest || check == false)
			{
				//and if it's above the current floor and should be called down, or below the
				//current floor and called up, or on the same floor, or idle
				if ((current > floor && direction == false) || (current < floor && direction == true) || current == floor || elevator->IsIdle())
				{
					//and if it's either going the same direction as the call or idle
					if (elevator->QueuePositionDirection == tmpdirection || elevator->IsIdle())
					{
						//and if it's not in any service mode
						if (sbs->GetElevator(Elevators[i])->InServiceMode() == false)
						{
							if (sbs->Verbose)
								Report("Marking - closest so far");
							closest = abs(current - floor);
							closest_elev = i;
							check = true;
						}
						else if (sbs->Verbose == true)
							Report("Skipping - in service mode");
					}
					else if (sbs->Verbose == true)
						Report("Skipping - going a different direction and is not idle");
				}
				else if (sbs->Verbose == true)
					Report("Skipping - position/direction wrong for call");
			}
			else if (sbs->Verbose == true)
				Report("Skipping - not closer than previous");
		}
	}
	else
	{
		//set elevator to first elevator if call button only serves one
		closest_elev = 0;
		check = true;
	}

	if (check == false)
	{
		//exit if no elevator found

		if (sbs->Verbose)
			Report("No elevator found");
		return;
	}

	//change processed state
	if (direction == true)
		ProcessedUp = true;
	else
		ProcessedDown = true;

	Elevator* elevator = sbs->GetElevator(Elevators[closest_elev]);

	//if selected elevator is in a service mode, exit
	if (elevator->InServiceMode() == true)
		return;

	if (sbs->Verbose)
		Report("Using elevator " + csString(_itoa(elevator->Number, intbuffer, 10)));

	//if closest elevator is already on the called floor, if call direction is the same, and if elevator is not idle
	if (elevator->GetFloor() == floor && elevator->QueuePositionDirection == tmpdirection && elevator->IsIdle() == false)
	{
		if (sbs->Verbose)
			Report("Elevator active on current floor - opening");

		//this call will return at least this call button
		csArray<int> buttons = sbs->GetFloor(floor)->GetCallButtons(Elevators[0]);

		//turn off all button lights in the group
		for (int i = 0; i < buttons.GetSize(); i++)
		{
			if (direction == true)
				sbs->GetFloor(floor)->CallButtonArray[buttons[i]]->UpLight(false);
			else
				sbs->GetFloor(floor)->CallButtonArray[buttons[i]]->DownLight(false);
		}

		if (direction == false)
		{
			//turn on directional indicator
			elevator->SetDirectionalIndicator(floor, false, true);
			//play chime sound
			elevator->Chime(0, floor, false);
		}
		else
		{
			//turn on directional indicator
			elevator->SetDirectionalIndicator(floor, true, false);
			//play chime sound
			elevator->Chime(0, floor, true);
		}
		//open elevator if it's on the same floor
		elevator->OpenDoors();
	}
	else
		//otherwise add a route entry to this floor
		elevator->AddRoute(floor, direction, false);

	//unregister callback if inactive
	if (UpStatus == false && DownStatus == false)
	{
		if (sbs->Verbose)
			Report("Unregistering callback");
		sbs->UnregisterCallButtonCallback(this);
	}
}

void CallButton::Report(const char *message)
{
	//general reporting function
	sbs->Report("Call button " + csString(_itoa(floor, intbuffer, 10)) + ":" + csString(_itoa(Number, intbuffer, 10)) + " - " + message);

}

void CallButton::ReportError(const char *message)
{
	//general reporting function
	sbs->ReportError("Call button " + csString(_itoa(floor, intbuffer, 10)) + ":" + csString(_itoa(Number, intbuffer, 10)) + " - " + message);
}
