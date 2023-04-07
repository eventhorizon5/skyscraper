/*
	Scalable Building Simulator - Call Station Object
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

#ifndef _SBS_CALLSTATION_H
#define _SBS_CALLSTATION_H

#include "lock.h"

namespace SBS {

class SBSIMPEXP CallStation : public Object, public Lock
{
public:

	std::string Name;

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
	void SetPosition(Ogre::Vector3 &position);
	bool ServicesElevator(int elevator);
	int GetElevatorArrived(int starting_floor, int destination_floor);
	bool FireService(int value);
	Indicator* AddIndicator(const std::string &texture_prefix, const std::string &blank_texture, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset);
	void UpdateIndicator(std::string &text);

private:
	ButtonPanel* panel; //button panel object
	DispatchController *controller; //dispatch controller object

	int Number; //call button index number (on the specified floor)
	bool is_enabled;

	Floor *floor; //floor this call button set is on

	Indicator* indicator;

	bool Locked;
	int KeyID;
};

}

#endif
