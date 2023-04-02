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

#ifndef _SBS_DESTINATION_H
#define _SBS_DESTINATION_H

namespace SBS {

class SBSIMPEXP DestinationController : public Object
{
public:

	int Number;

	//functions
	DestinationController(Object *parent, int number, std::vector<int> &elevators, int elevator_range);
	~DestinationController();
	void Loop();
	bool RequestRoute(int starting_floor, int destination_floor);
	bool AddElevator(int elevator);
	bool RemoveElevator(int elevator);
	bool ServicesElevator(int elevator);
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	void ElevatorArrived(int number);

private:

	int FindClosestElevator(int starting_floor, int destination_floor);

	std::vector<int> Elevators;
	int ActiveElevator;
	int ElevatorRange;

	class Timer; //internal timer class

	//timer object
	Timer *timer;
};

}

#endif
