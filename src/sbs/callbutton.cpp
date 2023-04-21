/*
	Scalable Building Simulator - Call Button Object
	The Skyscraper Project - Version 1.12 Alpha
	Copyright (C)2004-2023 Ryan Thoryk
	https://www.skyscrapersim.net
	https://sourceforge.net/projects/skyscraper/
	Contact - ryan@thoryk.com

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
#include "floor.h"
#include "buttonpanel.h"
#include "control.h"
#include "texture.h"
#include "controller.h"
#include "callbutton.h"

namespace SBS {

CallButton::CallButton(Object *parent, int controller, int floornum, int number, const std::string &sound_file_up, const std::string &sound_file_down, std::string BackTexture, const std::string &UpButtonTexture, const std::string &UpButtonTexture_Lit, const std::string &DownButtonTexture, const std::string &DownButtonTexture_Lit, Real CenterX, Real CenterZ, Real voffset, const std::string &direction, Real BackWidth, Real BackHeight, bool ShowBack, Real tw, Real th) : Object(parent), Lock(this)
{
	//create a set of call buttons

	floor = sbs->GetFloor(floornum);
	if (!floor)
		return;

	//set up SBS object
	SetValues("CallButton", "", false);

	is_enabled = true;

	if (sbs->GetController(controller) == 0)
		return;

	SetController(controller);

	UpExists = false;
	DownExists = false;

	//create object mesh
	std::string base = "Floor " + ToString(floornum) + ":Call Panel " + ToString(number);
	SetName(base);

	//set variables
	Number = number;
	Locked = false;
	KeyID = 0;

	if (ShowBack == false)
		BackTexture = "";

	if (sbs->Verbose)
		Report("Creating button panel");

	int topfloor = GetController()->GetTopFloor();
	int bottomfloor = GetController()->GetBottomFloor();

	if (floornum > bottomfloor && floornum < topfloor)
	{
		UpExists = true;
		DownExists = true;
	}
	else if (floornum == bottomfloor)
		UpExists = true;
	else if (floornum == topfloor)
		DownExists = true;

	int rows = 0;
	if (UpExists == true)
		rows++;
	if (DownExists == true)
		rows++;

	//create button panel
	Real button_height = BackHeight / 3.5;
	Real button_width = BackWidth / 2;
	Real h_spacing = 0.5;
	Real v_spacing = 1.25;
	if (UpExists == true && DownExists == true)
		v_spacing = 0.5;

	panel = new ButtonPanel(this, 1, BackTexture, rows, 1, direction, 0, 0, button_width, button_height, h_spacing, v_spacing, BackHeight / 2, tw, th, false);

	//create controls
	if (sbs->Verbose)
		Report("Creating controls");

	if (UpExists == true)
	{
		int row = 1;
		std::vector<std::string> names, textures;

		textures.push_back(UpButtonTexture);
		textures.push_back(UpButtonTexture_Lit);
		sbs->GetTextureManager()->EnableLighting(UpButtonTexture_Lit, false);
		names.push_back("off");
		names.push_back("up");

		panel->AddControl(sound_file_up, row, 1, 1, 1, 0, 0, 1, names, textures);
	}
	if (DownExists == true)
	{
		int row = 1;
		if (UpExists == true)
			row = 2;

		std::vector<std::string> names, textures;

		textures.push_back(DownButtonTexture);
		textures.push_back(DownButtonTexture_Lit);
		sbs->GetTextureManager()->EnableLighting(DownButtonTexture_Lit, false);
		names.push_back("off");
		names.push_back("down");

		panel->AddControl(sound_file_down, row, 1, 1, 1, 0, 0, 1, names, textures);
	}

	//set position of object
	Move(CenterX, floor->GetBase(true) + voffset, CenterZ);

	//reset light status
	//UpLight(false);
	//DownLight(false);

	if (sbs->Verbose)
		Report("Created");
}

CallButton::~CallButton()
{
	if (panel)
	{
		panel->parent_deleting = true;
		delete panel;
	}

	//unregister with parent floor object
	if (sbs->FastDelete == false)
	{
		if (parent_deleting == false)
			floor->RemoveCallButton(this);
	}
}

void CallButton::Enabled(bool value)
{
	//turns call buttons on/off
	if (is_enabled == value)
		return;

	is_enabled = value;

	//enable or disable the button panel
	panel->Enabled(value);

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
	//submits a call request to the associated controller

	//check lock state
	if (IsLocked() == true)
		return ReportError("Call button is locked");

	if (!GetController())
		return ReportError("No controller");

	//exit if direction not possible
	if (direction == true && UpExists == false)
		return false;
	if (direction == false && DownExists == false)
		return false;

	//turn on button lights
	//if (direction == true)
		//UpLight(true);
	//else
		//DownLight(true);

	return GetController()->CallElevator(0, this, direction);
}

bool CallButton::ServicesElevator(int elevator)
{
	//return true if this call button services the specified elevator

	if (GetController())
		return GetController()->ServicesElevator(elevator);
	return false;
}

void CallButton::Report(const std::string &message)
{
	//general reporting function
	std::string msg = "Call button " + ToString(GetFloor()) + ":" + ToString(Number) + " - " + message;
	Object::Report(msg);
}

bool CallButton::ReportError(const std::string &message)
{
	//general reporting function
	std::string msg = "Call button " + ToString(GetFloor()) + ":" + ToString(Number) + " - " + message;
	return Object::ReportError(msg);
}

bool CallButton::FireService(int value)
{
	//enables fire service phase 1 on all elevators associated with this call button

	if (GetController())
		return GetController()->FireService(value);
	return false;
}

int CallButton::GetFloor()
{
	//return floor number this call button is on

	return floor->Number;
}

bool CallButton::AddElevator(int elevator)
{
	//add an elevator to this call button

	if (GetController())
		return GetController()->AddElevator(elevator);
	return false;
}

bool CallButton::RemoveElevator(int elevator)
{
	//remove an elevator from this call button

	if (GetController())
		return GetController()->RemoveElevator(elevator);
	return false;
}

int CallButton::GetElevatorArrived(bool direction)
{
	//return the number of the elevator that has arrived, for the specified direction
	//return 0 if no elevator has arrived yet

	if (GetController())
		return GetController()->GetElevatorArrivedStandard(GetFloor(), direction);
	return 0;
}

void CallButton::SetController(int number)
{
	//assign this call button panel to a controller, and register with it

	controller = number;

	if (GetController())
		GetController()->RegisterCallButton(this);
}

DispatchController* CallButton::GetController()
{
	return sbs->GetController(controller);
}

}
