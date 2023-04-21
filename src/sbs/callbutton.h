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

#ifndef _SBS_CALLBUTTON_H
#define _SBS_CALLBUTTON_H

#include "lock.h"

namespace SBS {

class SBSIMPEXP CallButton : public Object, public Lock
{
public:

	//functions
	CallButton(Object *parent, int controller, int floornum, int number, const std::string &sound_file_up, const std::string &sound_file_down, std::string BackTexture, const std::string &UpButtonTexture, const std::string &UpButtonTexture_Lit, const std::string &DownButtonTexture, const std::string &DownButtonTexture_Lit, Real CenterX, Real CenterZ, Real voffset, const std::string &direction, Real BackWidth, Real BackHeight, bool ShowBack, Real tw, Real th);
	~CallButton();
	void Enabled(bool value);
	bool Call(bool direction); //true is up, false is down
	bool ServicesElevator(int elevator);
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	bool FireService(int value);
	int GetFloor();
	bool AddElevator(int elevator);
	bool RemoveElevator(int elevator);
	bool IsEnabled() { return is_enabled; }
	int GetElevatorArrived(bool direction);
	void SetController(int number);
	DispatchController* GetController();

private:

	ButtonPanel* panel; //button panel object

	int Number; //call button index number (on the specified floor)
	bool is_enabled;

	int controller;

	Floor *floor; //floor this call button set is on

	bool Locked;
	int KeyID;
	bool UpExists; //does up button exist?
	bool DownExists; //does down button exist?

};

}

#endif
