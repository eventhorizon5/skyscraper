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

#ifndef _SBS_CALLSTATION_H
#define _SBS_CALLSTATION_H

#include "lock.h"

namespace SBS {

class SBSIMPEXP CallStation : public Object, public Lock
{
public:

	std::string Name;
	int Number;
	std::vector<std::string> InvalidInput;
	Real TimerDelay;

	//functions
	CallStation(Object *parent, int floornum, int number);
	~CallStation();
	ButtonPanel* CreateButtonPanel(const std::string &texture, int rows, int columns, const std::string &direction, Real width, Real height, Real spacingX, Real spacingY, Real tw, Real th);
	void Enabled(bool value);
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	int GetFloor();
	ButtonPanel* GetPanel();
	void RemovePanel();
	bool SelectFloor(int floor);
	void SetController(int number);
	DispatchController* GetController();
	void SetPosition(Vector3 &position);
	bool ServicesElevator(int elevator);
	int GetElevatorArrived(int starting_floor, int destination_floor);
	bool FireService(int value);
	Indicator* AddIndicator(const std::string &sound, const std::string &texture_prefix, const std::string &blank_texture, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, Real timer_duration);
	void UpdateIndicator(const std::string &text, bool play_sound = true);
	bool Input(const std::string &text);
	void ProcessCache();
	void Error(bool type = 0);
	int GetRecallFloor();
	void ReportElevator(Elevator *elevator);
	bool Call(bool direction);
	bool GetFloorFromID(const std::string &floor, int &result);
	void SetLightsGroup(int up, int down);
	void UpLight(bool value);
	void DownLight(bool value);
	void SetLights(int up, int down);
	bool GetUpStatus();
	bool GetDownStatus();
	Control* GetUpControl();
	Control* GetDownControl();
	bool Press(bool up);
	bool CreateCallButtons(const std::string &sound_file_up, const std::string &sound_file_down, std::string BackTexture, const std::string &UpButtonTexture, const std::string &UpButtonTexture_Lit, const std::string &DownButtonTexture, const std::string &DownButtonTexture_Lit, const std::string &direction, Real BackWidth, Real BackHeight, bool ShowBack, Real tw, Real th);
	int GetElevatorArrivedStandard(int floor, bool direction);

private:
	ButtonPanel* panel; //button panel object
	int controller; //dispatch controller

	bool is_enabled;

	Floor *floor; //floor this call button set is on

	Indicator* indicator;

	//for keypad
	std::string InputCache;

	bool Locked;
	int KeyID;

	class Timer; //internal timer class

	//timer object
	Timer *timer;
};

}

#endif
