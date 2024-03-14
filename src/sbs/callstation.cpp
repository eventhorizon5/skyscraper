/*
	Scalable Building Simulator - Call Station Object
	The Skyscraper Project - Version 1.12 Alpha
	Copyright (C)2004-2024 Ryan Thoryk
	https://www.skyscrapersim.net
	https://sourceforge.net/projects/skyscraper/
	Contact - ryan@skyscrapersim.net

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
#include "floor.h"
#include "controller.h"
#include "indicator.h"
#include "timer.h"
#include "manager.h"
#include "texture.h"
#include "control.h"
#include "callstation.h"
#include "elevator.h"

namespace SBS {

class CallStation::Timer : public TimerObject
{
public:
	CallStation *parent;
	Timer(const std::string &name, CallStation *parent) : TimerObject(parent, name)
	{
		this->parent = parent;
	}
	virtual void Notify();
};

CallStation::CallStation(Object *parent, int floornum, int number) : Object(parent), Lock(this)
{
	floor = sbs->GetFloor(floornum);
	if (!floor)
		return;

	//set up SBS object
	SetValues("CallStation", "", false);

	is_enabled = true;

	std::string base = "Call Station " + ToString(number);
	SetName(base);

	//set variables
	Number = number;
	Locked = false;
	KeyID = 0;
	panel = 0;
	controller = 0;
	indicator = 0;
	TimerDelay = 2;

	//create timer
	timer = new Timer("Input Timeout Timer", this);

	if (sbs->Verbose)
		Report("Created");
}

CallStation::~CallStation()
{
	RemovePanel();

	if (indicator)
		delete indicator;
	indicator = 0;

	if (sbs->FastDelete == false)
	{
		//unregister with controller
		if (GetController())
			GetController()->UnregisterCallStation(this);

		//unregister with parent floor object
		if (parent_deleting == false)
			floor->RemoveCallStation(this);
	}
}

void CallStation::Timer::Notify()
{
	//input timeout timer

	parent->ProcessCache();
}

ButtonPanel* CallStation::CreateButtonPanel(const std::string &texture, int rows, int columns, const std::string &direction, Real buttonwidth, Real buttonheight, Real spacingX, Real spacingY, Real tw, Real th)
{
	//create a new button panel object

	if (panel)
		return 0;

	if (sbs->Verbose)
		Report("Creating button panel");

	panel = new ButtonPanel(this, 1, texture, rows, columns, direction, 0, 0, buttonwidth, buttonheight, spacingX, spacingY, 0, tw, th);
	return panel;
}

void CallStation::Enabled(bool value)
{
	//turns station on/off
	if (is_enabled == value)
		return;

	is_enabled = value;

	//enable or disable the button panel
	if (panel)
		panel->Enabled(value);

	if (sbs->Verbose)
	{
		if (value == true)
			Report("Enabled");
		else
			Report("Disabled");
	}
}

void CallStation::Report(const std::string &message)
{
	//general reporting function
	std::string msg = "Call Station " + ToString(GetFloor()) + ":" + ToString(Number) + " - " + message;
	Object::Report(msg);
}

bool CallStation::ReportError(const std::string &message)
{
	//general reporting function
	std::string msg = "Call Station " + ToString(GetFloor()) + ":" + ToString(Number) + " - " + message;
	return Object::ReportError(msg);
}

int CallStation::GetFloor()
{
	//return floor number this call station is on

	return floor->Number;
}

ButtonPanel* CallStation::GetPanel()
{
	return panel;
}

void CallStation::RemovePanel()
{
	if (panel)
	{
		panel->parent_deleting = true;
		delete panel;
	}
	panel = 0;
}

bool CallStation::SelectFloor(int floor)
{
	//check lock state
	if (IsLocked() == true)
		return ReportError("Call station is locked");

	Report("Selecting floor " + ToString(floor));

	if (GetController())
		return GetController()->RequestRoute(this, GetFloor(), floor);
	else
		return ReportError("No controller");
	return false;
}

void CallStation::SetController(int number)
{
	//assign this call station to a controller, and register with it

	controller = number;

	if (GetController())
		GetController()->RegisterCallStation(this);
}

DispatchController* CallStation::GetController()
{
	return sbs->GetController(controller);
}

void CallStation::SetPosition(Vector3 &position)
{
	position.y += sbs->GetFloor(GetFloor())->GetBase();
	Object::SetPosition(position);
}

bool CallStation::ServicesElevator(int elevator)
{
	if (GetController())
		return GetController()->ServicesElevator(elevator);
	return false;
}

int CallStation::GetElevatorArrived(int starting_floor, int destination_floor)
{
	//return the number of the elevator that has arrived, for the specified route
	//return 0 if no elevator has arrived yet

	if (!GetController())
		return 0;

	return GetController()->GetElevatorArrived(starting_floor, destination_floor);
}

int CallStation::GetElevatorArrivedStandard(int floor, bool direction)
{
	//return the number of the elevator that has arrived, for the specified route
	//return 0 if no elevator has arrived yet

	if (!GetController())
		return 0;

	return GetController()->GetElevatorArrivedStandard(floor, direction);
}

bool CallStation::FireService(int value)
{
	if (GetController())
		return GetController()->FireService(value);
	else
		return ReportError("No controller");
	return false;
}

Indicator* CallStation::AddIndicator(const std::string &sound, const std::string &texture_prefix, const std::string &blank_texture, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, Real timer_duration)
{
	if (indicator)
		return 0;

	indicator = new Indicator(this, sound, texture_prefix, blank_texture, direction, CenterX, CenterZ, width, height, voffset, timer_duration);

	return indicator;
}

void CallStation::UpdateIndicator(const std::string &text, bool play_sound)
{
	if (indicator)
		indicator->Update(text, play_sound);
}

bool CallStation::Input(const std::string &text)
{
	//input a keypad character

	//only allow single characters
	if (text.length() != 1)
		return false;

	//check lock state
	if (IsLocked() == true)
		return ReportError("Call station is locked");

	//erase last character if specified
	if (text == "<" && InputCache.size() >= 1)
		InputCache.pop_back();

	//add text to cache
	InputCache += text;

	//automatically error if multiple special characters were entered and/or combined with numbers
	int StarCount = std::count(InputCache.begin(), InputCache.end(), '*');
	int MinCount  = std::count(InputCache.begin(), InputCache.end(), '-');
	if ((StarCount > 1 || MinCount > 1) && InputCache.length() > 1)
	{
		timer->Stop();
		InputCache = "";
		Error(1);
		return true;
	}

	//update indicator display
	UpdateIndicator(InputCache, false);

	//verify that the floor entry is valid, error if not
	int result = 0;
	if (GetFloorFromID(InputCache, result) == false && InputCache != "*" && InputCache != "-")
	{
		timer->Stop();
		InputCache = "";
		Error(1);
		return true;
	}

	//start timeout timer
	timer->Start(TimerDelay * 1000.0f, true);

	return true;
}

void CallStation::ProcessCache()
{
	//process and clear input cache

	//select recall floor if "0"
	if (InputCache == "0" || InputCache == "*")
	{
		SelectFloor(GetRecallFloor());
		InputCache = "";
		return;
	}

	if (!IsNumeric(InputCache))
	{
		InputCache = "";
		Error();
		return;
	}

	//don't allow input values in the InvalidInput list
	for (int i = 0; i < (int)InvalidInput.size(); i++)
	{
		if (InputCache == InvalidInput[i])
		{
			InputCache = "";
			Error();
			return;
		}
	}

	int floor = 0;
	GetFloorFromID(InputCache, floor);
	SelectFloor(floor);

	InputCache = "";
}

bool CallStation::GetFloorFromID(const std::string &floor, int &result)
{
	if (!IsNumeric(floor))
		return false;

	int rawfloor = ToInt(floor);

	//convert back to string, to strip off any leading 0's
	std::string converted = ToString(rawfloor);

	Floor *floorobj = sbs->GetFloorManager()->GetByNumberID(converted);
	Floor *floorobj2 = sbs->GetFloorManager()->GetByID(converted);
	Floor *floorobj3 = sbs->GetFloorManager()->Get(rawfloor);

	if (floorobj)
	{
		result = floorobj->Number; //get by number ID first
		return true;
	}
	else if (floorobj2)
	{
		result = floorobj2->Number; //next try floor ID
		return true;
	}
	else if (floorobj3)
	{
		result = rawfloor; //and last, get by raw floor number
		return true;
	}

	return false;
}

void CallStation::Error(bool type)
{
	//if type is 0, standard error
	//if type is 1, invalid floor

	std::string message = "XX";
	if (type == 1)
		message = "??";

	UpdateIndicator(message);
}

int CallStation::GetRecallFloor()
{
	//get recall floor of elevator group from controller

	if (GetController())
		return GetController()->GetRecallFloor();
	return 0;
}

void CallStation::ReportElevator(Elevator *elevator)
{
	//report which elevator is assigned, on indicator

	//update indicator with direction of elevator
	std::string Direction = this->GetPanel()->Direction;
	Vector3 ButtonPos = this->GetPanel()->GetPosition();
	Vector3 ShaftPos  = elevator->GetPosition();

	if (Direction == "front")
	{
		if (ButtonPos.x > ShaftPos.x)
			return UpdateIndicator((ButtonPos.z < ShaftPos.z ? "<" : "[]") + elevator->ID);
		else
			return UpdateIndicator(elevator->ID + (ButtonPos.z < ShaftPos.z ? ">" : "[]"));
	}
	else if (Direction == "back")
	{
		if (ButtonPos.x < ShaftPos.x)
			return UpdateIndicator((ButtonPos.z > ShaftPos.z ? "<" : "[]") + elevator->ID);
		else
			return UpdateIndicator(elevator->ID + (ButtonPos.z > ShaftPos.z ? ">" : "[]"));
	}
	else if (Direction == "left")
	{
		if (ButtonPos.z < ShaftPos.z)
			return UpdateIndicator((ButtonPos.x < ShaftPos.x ? "<" : "[]") + elevator->ID);
		else
			return UpdateIndicator(elevator->ID + (ButtonPos.x < ShaftPos.x ? ">" : "[]"));
	}
	else if (Direction == "right")
	{
		if (ButtonPos.z > ShaftPos.z)
			return UpdateIndicator((ButtonPos.x > ShaftPos.x ? "<" : "[]") + elevator->ID);
		else
			return UpdateIndicator(elevator->ID + (ButtonPos.x > ShaftPos.x ? ">" : "[]"));
	}

	//otherwise update indicator with just elevator ID
	UpdateIndicator(elevator->ID);
	return;
}

bool CallStation::Call(bool direction)
{
	//call elevator in the specified direction

	//check lock state
	if (IsLocked() == true)
		return ReportError("Call station is locked");

	if (!GetController())
		return ReportError("No controller");

	//turn on button lights
	if (direction == true)
		UpLight(true);
	else
		DownLight(true);

	if (GetController())
		return GetController()->CallElevator(this, direction);

	return false;
}

void CallStation::SetLightsGroup(int up, int down)
{
	//set status of call button lights for whole group
	//values are 0 for no change, 1 for on, and 2 for off

	if (sbs->Verbose)
		Report("Call: finding grouped call buttons");

	//this call will return at least this call button
	std::vector<CallStation*> stations;
	if (GetController())
		stations = GetController()->GetCallStations(floor->Number);

	//set status on each call button
	for (size_t i = 0; i < stations.size(); i++)
	{
		if (stations[i])
			stations[i]->SetLights(up, down);
	}
}

void CallStation::UpLight(bool value)
{
	//turn on the 'up' directional light

	//set light status
	if (value == true)
		SetLightsGroup(1, 0);
	else
		SetLightsGroup(2, 0);
}

void CallStation::DownLight(bool value)
{
	//turn on the 'down' directional light

	//set light status
	if (value == true)
		SetLightsGroup(0, 1);
	else
		SetLightsGroup(0, 2);
}

void CallStation::SetLights(int up, int down)
{
	//set status of call button lights
	//values are 0 for no change, 1 for on, and 2 for off

	if (up == 1)
	{
		if (GetUpStatus() == true)
		{
			if (sbs->Verbose)
				Report("SetLights: up light already in requested status");
			return;
		}

		if (sbs->Verbose)
			Report("SetLights: turning on up light");

		if (GetUpControl())
			GetUpControl()->SetSelectPosition(2);
	}

	if (up == 2)
	{
		if (GetUpStatus() == false)
		{
			if (sbs->Verbose)
				Report("SetLights: up light already in requested status");
			return;
		}

		if (sbs->Verbose)
			Report("SetLights: turning off up light");

		if (GetUpControl())
			GetUpControl()->SetSelectPosition(1);
	}

	if (down == 1)
	{
		if (GetDownStatus() == true)
		{
			if (sbs->Verbose)
				Report("SetLights: down light already in requested status");
			return;
		}

		if (sbs->Verbose)
			Report("SetLights: turning on down light");

		if (GetDownControl())
			GetDownControl()->SetSelectPosition(2);
	}
	if (down == 2)
	{
		if (GetDownStatus() == false)
		{
			if (sbs->Verbose)
				Report("SetLights: down light already in requested status");
			return;
		}

		if (sbs->Verbose)
			Report("SetLights: turning off down light");

		if (GetDownControl())
			GetDownControl()->SetSelectPosition(1);
	}
}

bool CallStation::GetUpStatus()
{
	if (GetUpControl())
		return (GetUpControl()->GetSelectPosition() == 2);
	return false;
}

bool CallStation::GetDownStatus()
{
	if (GetDownControl())
		return (GetDownControl()->GetSelectPosition() == 2);
	return false;
}

Control* CallStation::GetUpControl()
{
	return panel->GetControl("up");
}

Control* CallStation::GetDownControl()
{
	return panel->GetControl("down");
}

bool CallStation::Press(bool up)
{
	//press the related call button (the control object)
	//which also initiates the call via the Call() function

	bool result = false;

	if (up == true && GetUpControl())
		result = GetUpControl()->Press();
	if (up == false && GetDownControl())
		result = GetDownControl()->Press();

	return result;
}

bool CallStation::CreateCallButtons(const std::string &sound_file_up, const std::string &sound_file_down, std::string BackTexture, const std::string &UpButtonTexture, const std::string &UpButtonTexture_Lit, const std::string &DownButtonTexture, const std::string &DownButtonTexture_Lit, const std::string &direction, Real BackWidth, Real BackHeight, bool ShowBack, Real tw, Real th)
{
	//creates a panel and call buttons for this call station

	if (!GetController())
		return false;

	if (ShowBack == false)
		BackTexture = "";

	int topfloor = GetController()->GetTopFloor();
	int bottomfloor = GetController()->GetBottomFloor();
	bool UpExists = false;
	bool DownExists = false;

	if (floor->Number > bottomfloor && floor->Number < topfloor)
	{
		UpExists = true;
		DownExists = true;
	}
	else if (floor->Number == bottomfloor)
		UpExists = true;
	else if (floor->Number == topfloor)
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

	return true;
}

}
