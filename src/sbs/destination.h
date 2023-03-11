/*
	Scalable Building Simulator - Destination Dispatch Controller
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

#ifndef _SBS_DESTINATION_H
#define _SBS_DESTINATION_H

#include "lock.h"

namespace SBS {

class SBSIMPEXP Destination : public Object, public Lock
{
public:

	//functions
	Destination(Object *parent, std::vector<int> &elevators, int floornum, int number);
	~Destination();
	void Enabled(bool value);
	bool Call(int floor);
	bool ServicesElevator(int elevator);
	void Loop();
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	bool FireService(int value);
	int GetFloor();
	bool AddElevator(int elevator);
	bool RemoveElevator(int elevator);
	bool GetStatus();
	bool IsEnabled() { return is_enabled; }
	void ElevatorArrived(int number);
	int GetElevatorArrived();
	int FindClosestElevator();

private:
	void Process();

	int Number; //call button index number (on the specified floor)
	bool is_enabled;
	int elevator_arrived;

	std::vector<int> Elevators; //elevators this call button set is assigned to

	Floor *floor; //floor this call button set is on

	bool Processed;
	bool Locked;
	int KeyID;
	int ActiveElevator; //elevator the call is waiting for
	bool Status; //is a call active?
	int DestinationFloor;

	class Timer; //internal timer class

	//timer object
	Timer *timer;
};

}

#endif
