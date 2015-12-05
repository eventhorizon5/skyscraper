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

	//check all express and local elevators if they directly serve destination floor
	{
		ElevatorRoute *route = GetDirectRoute(start_floor, DestinationFloor, service_access);

		if (route)
		{
			result.push_back(route);
			return result;
		}
	}

	//otherwise check express elevator floors, to see if any have a direct route

	if (service_access == true)
	{
		result = GetIndirectRoute("Service", StartingFloor, DestinationFloor, service_access);

		if (result.empty() == false)
			return result;
	}

	result = GetIndirectRoute("Express", StartingFloor, DestinationFloor, service_access);

	if (result.empty() == false)
		return result;


	//check routes with two express elevators in between

	if (service_access == true)
	{
		result = GetIndirectRoute("Service", StartingFloor, DestinationFloor, service_access, true);

		if (result.empty() == false)
			return result;
	}

	result = GetIndirectRoute("Express", StartingFloor, DestinationFloor, service_access, true);

	if (result.empty() == false)
		return result;

	return result;
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

std::vector<ElevatorRoute*> SBS::GetIndirectRoute(std::string ElevatorType, int StartingFloor, int DestinationFloor, bool service_access, bool recursion)
{
	//get a route to a destination floor, via elevator serviced floors

	std::vector<ElevatorRoute*> result;

	Floor *start_floor = GetFloor(StartingFloor);

	if (!start_floor)
		return result;

	std::vector<int> list;
	start_floor->GetElevatorList(list, ElevatorType);

	for (int i = 0; i < (int)list.size(); i++)
	{
		Elevator *elev = sbs->GetElevator(list[i]);

		if (elev)
		{
			for (int j = 0; j < elev->GetServicedFloorCount(); j++)
			{
				int number = elev->GetServicedFloor(j);

				if (number != StartingFloor)
				{
					if (recursion == true)
					{
						std::vector<ElevatorRoute*> result2 = GetIndirectRoute(ElevatorType, number, DestinationFloor, service_access);

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
						Floor *floor = sbs->GetFloor(number);
						if (floor)
						{
							ElevatorRoute *result2 = GetDirectRoute(floor, DestinationFloor, service_access);

							if (result2)
							{
								ElevatorRoute *first = new ElevatorRoute(elev, number);
								result.push_back(first);
								result.push_back(result2);
								return result;
							}
						}
					}
				}
			}
		}
	}

	return result;
}

}
