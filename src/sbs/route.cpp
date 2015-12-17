/* $Id$ */

/*
	Scalable Building Simulator - Routing Objects
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

#include "globals.h"
#include "sbs.h"
#include "unix.h"
#include "route.h"

namespace SBS {

ElevatorRoute::ElevatorRoute(Elevator *elevator, int floor_selection)
{
	this->elevator = elevator;
	this->floor_selection = floor_selection;
}

std::vector<ElevatorRoute*> SBS::GetRouteToFloor(int StartingFloor, int DestinationFloor, bool service_access)
{
	//get a path from a starting floor to a desination floor, as a list of elevators to ride
	//if service_access is true, include service elevators in checks

	//this function uses designated skylobbies to connect elevators;
	//connection floors must have a type of "Lobby" or "Skylobby"

	std::vector<ElevatorRoute*> result;

	Floor *start_floor = GetFloor(StartingFloor);
	Floor *dest_floor = GetFloor(DestinationFloor);

	if (!start_floor || !dest_floor || start_floor == dest_floor)
		return result;

	std::vector<int> checked_floors;

	//Direct check
	//check all express and local elevators if they directly serve destination floor
	{
		ElevatorRoute *route = GetDirectRoute(start_floor, DestinationFloor, service_access);

		if (route)
		{
			result.push_back(route);
			return result;
		}

		checked_floors.push_back(StartingFloor);
	}

	//Indirect check
	return GetIndirectRoute(checked_floors, StartingFloor, DestinationFloor, service_access, 4);
}

ElevatorRoute* SBS::GetDirectRoute(Floor *floor, int DestinationFloor, bool service_access)
{
	ElevatorRoute *route;

	if (service_access == true)
	{
		route = floor->GetDirectRoute(DestinationFloor, "Service");
		if (route)
			return route;
	}

	route = floor->GetDirectRoute(DestinationFloor, "Express");
	if (route)
		return route;

	route = floor->GetDirectRoute(DestinationFloor, "Local");
	if (route)
		return route;

	return route;
}

std::vector<ElevatorRoute*> SBS::GetIndirectRoute(std::vector<int> &checked_floors, int StartingFloor, int DestinationFloor, bool service_access, int recursion_levels)
{
	//get a route to a destination floor, via elevator serviced floors

	//this function will find an indirect route to a floor, while prioritizing service and express elevators

	std::vector<ElevatorRoute*> result;

	Floor *start_floor = GetFloor(StartingFloor);

	if (!start_floor)
		return result;

	std::vector<int> list, list_express, list_local;

	//get service elevators
	if (service_access == true)
		start_floor->GetElevatorList(list, false, false, true);

	//get express elevators
	start_floor->GetElevatorList(list_express, false, true, false);
	list.insert(list.end(), list_express.begin(), list_express.end());

	bool locals_checked = false;
	bool recurse = false;

	while (true)
	{
		for (int i = 0; i < (int)list.size(); i++)
		{
			Elevator *elev = GetElevator(list[i]);

			if (elev)
			{
				//floor list needs to be checked in the direction opposite of the travel direction,
				//if the checked elevator doesn't go beyond the destination floor

				std::vector<int> floor_list;
				if ((DestinationFloor > StartingFloor && elev->GetTopFloor() < DestinationFloor) ||
						(DestinationFloor < StartingFloor && elev->GetBottomFloor() < DestinationFloor))
				{
					for (int j = elev->GetServicedFloorCount() - 1; j >= 0; j--)
					{
						floor_list.push_back(elev->GetServicedFloor(j));
					}
				}
				else
				{
					for (int j = 0; j < elev->GetServicedFloorCount(); j++)
					{
						floor_list.push_back(elev->GetServicedFloor(j));
					}
				}

				for (int j = 0; j < (int)floor_list.size(); j++)
				{
					int number = floor_list[j];

					if (number != StartingFloor)
					{
						if (recurse == true)
						{
							std::vector<ElevatorRoute*> result2 = GetIndirectRoute(checked_floors, number, DestinationFloor, service_access);

							if (result2.empty() == false)
							{
								ElevatorRoute *first = new ElevatorRoute(elev, number);
								result.push_back(first);

								for (int i = 0; i < (int)result2.size(); i++)
								{
									result.push_back(result2[i]);
								}
								return result;
							}
						}
						else
						{
							Floor *floor = GetFloor(number);

							//see if this floor has already been checked
							bool checked = false;
							for (int i = 0; i < (int)checked_floors.size(); i++)
							{
								if (checked_floors[i] == number)
								{
									checked = true;
									break;
								}
							}

							if (floor && checked == false)
							{
								ElevatorRoute *result2 = GetDirectRoute(floor, DestinationFloor, service_access);

								if (result2)
								{
									ElevatorRoute *first = new ElevatorRoute(elev, number);
									result.push_back(first);
									result.push_back(result2);
									return result;
								}

								checked_floors.push_back(number);
							}
						}
					}
				}
			}
		}

		if (recursion_levels == 0)
			return result;
		else
		{
			if (locals_checked == true && recurse == true)
				return GetIndirectRoute(checked_floors, StartingFloor, DestinationFloor, service_access, --recursion_levels);

			//if still going, check local elevators

			//get local elevators
			if (locals_checked == false && recurse == true)
			{
				list.clear();
				start_floor->GetElevatorList(list_local, true, false, false);
				list.insert(list.end(), list_local.begin(), list_local.end());
				locals_checked = true;
				recurse = false;
			}
			else if (recurse == false)
				recurse = true;
		}
	}

	return result;
}

}
