/* $Id$ */

/*
	Scalable Building Simulator - Person Object
	The Skyscraper Project - Version 1.10 Alpha
	Copyright (C)2004-2016 Ryan Thoryk
	http://www.skyscrapersim.com
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@skyscrapersim.com

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

#ifndef _SBS_PERSON_H
#define _SBS_PERSON_H

namespace SBS {

class SBSIMPEXP Person : public Object
{
public:

	RandomGen *rnd_time, *rnd_dest;

	//functions
	Person(Object *parent, const std::string &name, int floor, bool service_access = false);
	~Person();
	void GotoFloor(int floor);
	void Loop();
	void ProcessRoute();
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	int GetRandomFloor();
	bool IsRouteActive() { return !route.empty(); }
	bool IsRandomActivityEnabled();
	void EnableRandomActivity(bool value);
	void SetFloor(int value);
	int GetFloor() { return current_floor; }
	int GetDestinationFloor() { return dest_floor; }
	void SetServiceAccess(bool value) { service_access = value; }
	bool GetServiceAccess() { return service_access; }
	void Stop();
	std::string GetStatus();

private:

	struct RouteEntry
	{
		ElevatorRoute* elevator_route;
		CallButton* callbutton;
		int call_made;
		bool floor_selected;
	};

	int current_floor;
	int dest_floor;
	bool service_access;
	std::vector<RouteEntry> route;

	class Timer; //internal timer class

	//random call timer object
	Timer *random_timer;

	int RandomProbability; //probability ratio of random activity, starting with 1 - higher is less frequent
	float RandomFrequency; //speed in seconds to make each random action
};

}

#endif
