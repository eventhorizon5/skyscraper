/* $Id$ */

/*
	Scalable Building Simulator - Call Button Object
	The Skyscraper Project - Version 1.10 Alpha
	Copyright (C)2004-2015 Ryan Thoryk
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

#include <OgreMaterial.h>
#include "globals.h"
#include "sbs.h"
#include "callbutton.h"
#include "camera.h"
#include "elevator.h"
#include "unix.h"

namespace SBS {

CallButton::CallButton(Object *parent, std::vector<int> &elevators, int floornum, int number, const std::string &sound_file, const std::string &BackTexture, const std::string &UpButtonTexture, const std::string &UpButtonTexture_Lit, const std::string &DownButtonTexture, const std::string &DownButtonTexture_Lit, float CenterX, float CenterZ, float voffset, const std::string &direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th) : Object(parent)
{
	//create a set of call buttons

	floor = sbs->GetFloor(floornum);
	if (!floor)
		return;

	//set up SBS object
	SetValues("CallButton", "", false);

	is_enabled = true;
	Elevators.resize(elevators.size());
	for (size_t i = 0; i < elevators.size(); i++)
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
	UpExists = false;
	DownExists = false;
	elevator_arrived = -1;
	elevator_direction = false;

	//create object mesh
	std::string base = "Call Panel " + ToString(floornum) + ":" + ToString(number);
	SetName(base);
	CallButtonMeshBack = new MeshObject(this, base, "", sbs->GetConfigFloat("Skyscraper.SBS.MaxSmallRenderDistance", 100));
	CallButtonMeshUp = new MeshObject(this, base + ":Up", "", sbs->GetConfigFloat("Skyscraper.SBS.MaxSmallRenderDistance", 100));
	CallButtonMeshDown = new MeshObject(this, base + ":Down", "", sbs->GetConfigFloat("Skyscraper.SBS.MaxSmallRenderDistance", 100));

	//set variables
	Number = number;
	Direction = direction;
	SetCase(Direction, false);
	TrimString(Direction);
	Locked = false;
	KeyID = 0;

	//create sound object
	sound = new Sound(this, "Call Button Sound", true);
	sound->Load(sound_file);

	sbs->ResetTextureMapping(true);

	//create panel
	if (ShowBack == true)
	{
		if (sbs->Verbose)
			Report("Creating back panel");

		if (Direction == "front" || Direction == "back")
		{
			float x1 = -BackWidth / 2;
			float x2 = BackWidth / 2;
			if (Direction == "front")
				sbs->DrawWalls(true, false, false, false, false, false);
			else
				sbs->DrawWalls(false, true, false, false, false, false);

			WallObject *wall;
			wall = CallButtonMeshBack->CreateWallObject("Call Button Panel");
			sbs->AddWallMain(wall, "Call Button Panel", BackTexture, 0, x1, 0, x2, 0, BackHeight, BackHeight, 0, 0, tw, th, false);
		}
		if (Direction == "left" || Direction == "right")
		{
			float z1 = -BackWidth / 2;
			float z2 = BackWidth / 2;
			if (Direction == "left")
				sbs->DrawWalls(true, false, false, false, false, false);
			else
				sbs->DrawWalls(false, true, false, false, false, false);

			WallObject *wall;
			wall = CallButtonMeshBack->CreateWallObject("Call Button Panel");
			sbs->AddWallMain(wall, "Call Button Panel", BackTexture, 0, 0, z1, 0, z2, BackHeight, BackHeight, 0, 0, tw, th, false);
		}
		sbs->ResetWalls();
	}

	//determine floor range of associated elevators
	int bottomfloor = 0;
	int topfloor = 0;
	bool firstrun = true;

	for (int i = 0; i < (int)Elevators.size(); i++)
	{
		Elevator *elev = sbs->GetElevator(Elevators[i]);
		if (elev)
		{
			int tmpbottom = elev->GetBottomFloor();
			int tmptop = elev->GetTopFloor();
			if (tmpbottom < bottomfloor || firstrun == true)
				bottomfloor = tmpbottom;
			if (tmptop > topfloor || firstrun == true)
				topfloor = tmptop;
			firstrun = false;
		}
	}

	//create buttons
	if (sbs->Verbose)
		Report("Creating buttons");

	sbs->TexelOverride = true;
	if (Direction == "front" || Direction == "back")
	{
		float x1 = -BackWidth / 4;
		float x2 = BackWidth / 4;
		float offset;
		if (Direction == "front")
		{
			offset = -0.01f;
			sbs->DrawWalls(true, false, false, false, false, false);
		}
		else
		{
			offset = 0.01f;
			sbs->DrawWalls(false, true, false, false, false, false);
		}

		if (floornum > bottomfloor && floornum < topfloor)
		{
			float height = (BackHeight / 7) * 2;
			float altitude = (BackHeight / 7) * 4;
			float altitude2 = BackHeight / 7;
			WallObject *wall;
			wall = CallButtonMeshUp->CreateWallObject("Call Button Up");
			sbs->AddWallMain(wall, "Call Button Up", UpButtonTexture, 0, x1, offset, x2, offset, height, height, altitude, altitude, 1, 1, false);
			UpExists = true;

			wall = CallButtonMeshDown->CreateWallObject("Call Button Down");
			sbs->AddWallMain(wall, "Call Button Down", DownButtonTexture, 0, x1, offset, x2, offset, height, height, altitude2, altitude2, 1, 1, false);
			DownExists = true;
		}
		else
		{
			float height = (BackHeight / 7) * 2;
			float altitude = ((BackHeight / 7) * 3) - (BackHeight / 14);
			WallObject *wall;
			if (floornum < topfloor)
			{
				wall = CallButtonMeshUp->CreateWallObject("Call Button Up");
				sbs->AddWallMain(wall, "Call Button Up", UpButtonTexture, 0, x1, offset, x2, offset, height, height, altitude, altitude, 1, 1, false);
				UpExists = true;
			}
			if (floornum > bottomfloor)
			{
				wall = CallButtonMeshDown->CreateWallObject("Call Button Down");
				sbs->AddWallMain(wall, "Call Button Down", DownButtonTexture, 0, x1, offset, x2, offset, height, height, altitude, altitude, 1, 1, false);
				DownExists = true;
			}
		}
		sbs->ResetWalls();
	}
	else
	{
		float z1 = -BackWidth / 4;
		float z2 = BackWidth / 4;
		float offset;
		if (Direction == "left")
		{
			offset = -0.01f;
			sbs->DrawWalls(true, false, false, false, false, false);
		}
		else
		{
			offset = 0.01f;
			sbs->DrawWalls(false, true, false, false, false, false);
		}
		if (floornum > bottomfloor && floornum < topfloor)
		{
			float height = (BackHeight / 7) * 2;
			float altitude = (BackHeight / 7) * 4;
			float altitude2 = BackHeight / 7;
			WallObject *wall;
			wall = CallButtonMeshUp->CreateWallObject("Call Button Up");
			sbs->AddWallMain(wall, "Call Button Up", UpButtonTexture, 0, offset, z1, offset, z2, height, height, altitude, altitude, 1, 1, false);
			UpExists = true;

			wall = CallButtonMeshDown->CreateWallObject("Call Button Down");
			sbs->AddWallMain(wall, "Call Button Down", DownButtonTexture, 0, offset, z1, offset, z2, height, height, altitude2, altitude2, 1, 1, false);
			DownExists = true;
		}
		else
		{
			float height = (BackHeight / 7) * 2;
			float altitude = ((BackHeight / 7) * 3) - (BackHeight / 14);
			WallObject *wall;
			if (floornum < topfloor)
			{
				wall = CallButtonMeshUp->CreateWallObject("Call Button Up");
				sbs->AddWallMain(wall, "Call Button Up", UpButtonTexture, 0, offset, z1, offset, z2, height, height, altitude, altitude, 1, 1, false);
				UpExists = true;
			}
			if (floornum > bottomfloor)
			{
				wall = CallButtonMeshDown->CreateWallObject("Call Button Down");
				sbs->AddWallMain(wall, "Call Button Down", DownButtonTexture, 0, offset, z1, offset, z2, height, height, altitude, altitude, 1, 1, false);
				DownExists = true;
			}
		}
		sbs->ResetWalls();
	}
	sbs->ResetTextureMapping();
	sbs->TexelOverride = false;

	//set position of object
	Move(CenterX, floor->GetBase(true) + voffset, CenterZ);

	if (sbs->Verbose)
		Report("Created");
}

CallButton::~CallButton()
{
	if (CallButtonMeshDown)
	{
		CallButtonMeshDown->parent_deleting = true;
		delete CallButtonMeshDown;
	}
	CallButtonMeshDown = 0;
	if (CallButtonMeshUp)
	{
		CallButtonMeshUp->parent_deleting = true;
		delete CallButtonMeshUp;
	}
	CallButtonMeshUp = 0;
	if (CallButtonMeshBack)
	{
		CallButtonMeshBack->parent_deleting = true;
		delete CallButtonMeshBack;
	}
	CallButtonMeshBack = 0;

	if (sound)
	{
		sound->parent_deleting = true;
		delete sound;
	}
	sound = 0;

	//unregister with parent floor object
	if (sbs->FastDelete == false)
	{
		if (parent_deleting == false)
			floor->RemoveCallButton(this);

		sbs->UnregisterCallButtonCallback(this);
	}
}

void CallButton::Enabled(bool value)
{
	//turns call buttons on/off
	if (is_enabled == value)
		return;

	CallButtonMeshBack->Enable(value);
	CallButtonMeshUp->Enable(value);
	CallButtonMeshDown->Enable(value);
	is_enabled = value;

	if (sbs->Verbose)
	{
		if (value == true)
			Report("Enabled");
		else
			Report("Disabled");
	}
}

bool CallButton::Call(bool direction)
{
	//calls the closest elevator in the elevator array list to the current floor,
	//and also depending on the direction it's traveling

	//check lock state
	if (IsLocked() == true)
		return ReportError("Call button is locked");

	//exit if direction not possible
	if (direction == true && UpExists == false)
		return false;
	if (direction == false && DownExists == false)
		return false;

	//exit if call has already been made
	if (direction == true && UpStatus == true)
		return ReportError("Up call has already been made");
	if (direction == false && DownStatus == true)
		return ReportError("Down call has already been made");

	//check to make sure elevator objects are valid
	bool isvalid = false;
	for (int i = 0; i < (int)Elevators.size(); i++)
	{
		Elevator *elevator = sbs->GetElevator(Elevators[i]);
		if (elevator)
		{
			isvalid = true;
			break;
		}
	}
	if (isvalid == false)
		return ReportError("No valid elevators found");

	//play sound
	sound->SetLoopState(false);
	sound->Play();

	//turn on button lights
	if (direction == true)
		UpLight(true);
	else
		DownLight(true);

	//reset processed state
	if (direction == true)
		ProcessedUp = false;
	else
		ProcessedDown = false;

	//reset elevator arrival state
	elevator_arrived = -1;

	//register callback for this button
	if (sbs->Verbose)
		Report("Registering callback");

	sbs->RegisterCallButtonCallback(this);
	return true;
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
		SetLightsGroup(1, 0);
	else
		SetLightsGroup(2, 0);
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
		SetLightsGroup(0, 1);
	else
		SetLightsGroup(0, 2);
}

void CallButton::SetLights(int up, int down)
{
	//set status of call button lights
	//values are 0 for no change, 1 for on, and 2 for off

	if (up == 1 && CallButtonMeshUp)
	{
		if (sbs->Verbose)
			Report("SetLights: turning on up light");

		CallButtonMeshUp->ChangeTexture(UpTextureLit);
		UpStatus = true;
	}
	if (up == 2 && CallButtonMeshUp)
	{
		if (sbs->Verbose)
			Report("SetLights: turning off up light");

		CallButtonMeshUp->ChangeTexture(UpTexture);
		UpStatus = false;
	}
	if (down == 1 && CallButtonMeshDown)
	{
		if (sbs->Verbose)
			Report("SetLights: turning on down light");

		CallButtonMeshDown->ChangeTexture(DownTextureLit);
		DownStatus = true;
	}
	if (down == 2 && CallButtonMeshDown)
	{
		if (sbs->Verbose)
			Report("SetLights: turning off down light");

		CallButtonMeshDown->ChangeTexture(DownTexture);
		DownStatus = false;
	}

	/*if (up > 0 && !CallButtonMeshUp && sbs->Verbose)
		Report("SetLights: cannot change up light status");
	if (down > 0 && !CallButtonMeshDown && sbs->Verbose)
		Report("SetLights: cannot change down light status");*/
}

bool CallButton::ServicesElevator(int elevator)
{
	//return true if this call button services the specified elevator
	for (int i = 0; i < (int)Elevators.size(); i++)
	{
		if (Elevators[i] == elevator)
		{
			if (sbs->Verbose)
				Report("Services elevator " + ToString(elevator));
			return true;
		}
	}
	return false;
}

void CallButton::Loop()
{
	//this function runs for every registered call button via callback

	//process up calls
	Process(1);

	//process down calls
	Process(-1);
}

void CallButton::Process(int direction)
{
	//process call button
	//direction is the call direction to process

	SBS_PROFILE("CallButton::Process");

	//unregister callback if inactive
	if (UpStatus == false && DownStatus == false)
	{
		if (sbs->Verbose)
			Report("Unregistering callback");
		sbs->UnregisterCallButtonCallback(this);
		return;
	}

	//first exit if no call button is not processing a call for the current direction
	//or if a call has already been processed
	if ((UpStatus == false && direction == 1) || (ProcessedUp == true && direction == 1))
		return;
	if ((DownStatus == false && direction == -1) || (ProcessedDown == true && direction == -1))
		return;

	//initialize values
	int closest = 0;
	int closest_elev = 0;
	bool check = false;
	int errors = 0;

	int count = (int)Elevators.size();

	//exit if no elevators are associated
	if (count == 0)
		return;

	//search through elevator list
	if (sbs->Verbose && count > 1)
		Report("Finding nearest available elevator...");

	//check each elevator associated with this call button to find the closest available one
	for (int i = 0; i < count; i++)
	{
		Elevator *elevator = sbs->GetElevator(Elevators[i]);
		if (elevator)
		{
			if (sbs->Verbose)
				Report("Checking elevator " + ToString(elevator->Number));

			//if elevator is closer than the previously checked one or we're starting the checks
			if (abs(elevator->GetFloor() - GetFloor()) < closest || check == false)
			{
				//see if elevator is available for the call
				int result = elevator->AvailableForCall(GetFloor(), direction);

				if (result == 1) //available
				{
					if (sbs->Verbose && count > 1)
						Report("Marking - closest so far");
					closest = abs(elevator->GetFloor() - GetFloor());
					closest_elev = i;
					check = true;
				}
				else if (result == 2) //elevator won't service the call
					errors++;
			}
		}
	}

	if (errors == count)
	{
		//exit if all elevators are in a service mode or return errors

		std::string item;
		if (count > 1)
			item = "All elevators";
		else
			item = "Elevator";

		Report(item + " unavailable due to service modes or errors");

		//turn off button lights
		if (direction == 1)
			UpLight(false);
		else
			DownLight(false);

		return;
	}

	if (check == false)
	{
		//exit if no elevator found

		if (sbs->Verbose)
			Report("No elevator found");
		return;
	}

	//change processed state
	if (direction == 1)
		ProcessedUp = true;
	else
		ProcessedDown = true;

	Elevator* elevator = sbs->GetElevator(Elevators[closest_elev]);

	//if selected elevator is in a service mode, exit
	if (elevator->InServiceMode() == true)
		return;

	if (sbs->Verbose)
		Report("Using elevator " + ToString(elevator->Number));

	//if closest elevator is already on the called floor, if call direction is the same, and if elevator is not idle
	if (elevator->GetFloor() == GetFloor() && elevator->QueuePositionDirection == direction && elevator->IsIdle() == false && elevator->IsMoving == false)
	{
		if (sbs->Verbose)
			Report("Elevator active on current floor - opening");

		//turn off button lights
		if (direction == 1)
			UpLight(false);
		else
			DownLight(false);

		if (direction == -1)
		{
			//turn on directional indicator
			elevator->SetDirectionalIndicators(GetFloor(), false, true);
			//play chime sound
			elevator->Chime(0, GetFloor(), false);
		}
		else
		{
			//turn on directional indicator
			elevator->SetDirectionalIndicators(GetFloor(), true, false);
			//play chime sound
			elevator->Chime(0, GetFloor(), true);
		}
		//open elevator if it's on the same floor
		elevator->OpenDoors();
	}
	else
		//otherwise add a route entry to this floor
		elevator->AddRoute(GetFloor(), direction, 1);
}

void CallButton::Report(const std::string &message)
{
	//general reporting function
	std::string msg = "Call button " + ToString(GetFloor()) + ":" + ToString(Number) + " - " + message;
	sbs->Report(msg);
}

bool CallButton::ReportError(const std::string &message)
{
	//general reporting function
	std::string msg = "Call button " + ToString(GetFloor()) + ":" + ToString(Number) + " - " + message;
	return sbs->ReportError(msg);
}

void CallButton::SetLocked(bool value, int keyid)
{
	//set locked state
	Locked = value;
	KeyID = keyid;
}

bool CallButton::ToggleLock(bool force)
{
	//toggle lock state
	//if force is true, bypass key check

	//quit if user doesn't have key, if force is false
	if (KeyID != 0)
	{
		if (sbs->CheckKey(KeyID) == false && force == false)
			return ReportError("Need key " + ToString(KeyID) + "to lock/unlock");
	}

	Locked = !Locked;

	if (Locked == true)
		Report("Locked");
	else
		Report("Unlocked");

	return true;
}

bool CallButton::IsLocked()
{
	return Locked;
}

int CallButton::GetKeyID()
{
	return KeyID;
}

void CallButton::FireService(int value)
{
	//enables fire service phase 1 on all elevators associated with this call button

	for (int i = 0; i < (int)Elevators.size(); i++)
	{
		Elevator *elevator = sbs->GetElevator(Elevators[i]);
		if (elevator)
			elevator->EnableFireService1(value);
	}
}

int CallButton::GetFloor()
{
	//return floor number this call button is on

	return floor->Number;
}

void CallButton::SetLightsGroup(int up, int down)
{
	//set status of call button lights for whole group
	//values are 0 for no change, 1 for on, and 2 for off

	if (sbs->Verbose)
		Report("Call: finding grouped call buttons");

	//this call will return at least this call button
	std::vector<int> buttons = floor->GetCallButtons(Elevators[0]);

	//set status on each call button
	for (int i = 0; i < (int)buttons.size(); i++)
	{
		floor->CallButtonArray[buttons[i]]->SetLights(up, down);
	}
}

bool CallButton::AddElevator(int elevator)
{
	//add an elevator to this call button

	for (int i = 0; i < (int)Elevators.size(); i++)
	{
		if (Elevators[i] == elevator)
			return false;
	}

	Elevators.push_back(elevator);
	return true;
}

bool CallButton::RemoveElevator(int elevator)
{
	//remove an elevator from this call button

	for (int i = 0; i < (int)Elevators.size(); i++)
	{
		if (Elevators[i] == elevator)
		{
			Elevators.erase(Elevators.begin() + i);
			return true;
		}
	}
	return false;
}

void CallButton::ElevatorArrived(int number, bool direction)
{
	//notify call button about an elevator arrival
	//this also turns off the related direction light

	if ((UpStatus == true && direction == true) || (DownStatus == true && direction == false))
	{
		elevator_arrived = number;
		elevator_direction = direction;
	}

	if (direction == true)
		UpLight(false);
	else
		DownLight(false);
}

bool CallButton::GetElevatorArrived(int &number, bool &direction)
{
	number = elevator_arrived;
	direction = elevator_direction;

	if (number > -1)
		return true;
	return false;
}

}
