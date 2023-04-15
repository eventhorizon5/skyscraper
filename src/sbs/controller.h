/*
	Scalable Building Simulator - Dispatch Controller Object
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

#ifndef _SBS_CONTROLLER_H
#define _SBS_CONTROLLER_H

namespace SBS {

class SBSIMPEXP DispatchController : public Object
{
public:

	int Number;
	std::string Name; //controller name
	bool DestinationDispatch; //enable or disable Destination Dispatch
	bool Hybrid; //if true, allow both standard and destination dispatch modes
	int Range; //elevator selection range
	int MaxPassengers; //maximum passengers per route

	//functions
	DispatchController(Object *parent, int number);
	~DispatchController();
	void Loop();
	bool RequestRoute(CallStation *station, int starting_floor, int destination_floor);
	bool AddElevator(int elevator);
	bool RemoveElevator(int elevator);
	bool ServicesElevator(int elevator);
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	void ElevatorArrived(int number, int floor, bool direction);
	void ElevatorMoving(int number);
	bool IsElevatorAssigned(int number, int destination_floor);
	bool IsElevatorAssignedToOther(int number, int destination_floor);
	void AssignElevator(int number, int destination_floor);
	void RegisterCallStation(CallStation *station);
	void UnregisterCallStation(CallStation *station);
	int GetElevatorArrived(int starting_floor, int destination_floor);
	bool IsServicedFloor(int floor);
	bool FireService(int value);
	bool AtMaxRequests(int elevator, int destination_floor);
	void StationError(CallStation *station);

private:

	struct Route;

	int FindClosestElevator(bool destination, int starting_floor, int destination_floor);
	void DispatchElevator(int number, int destination_floor, int direction, bool call);
	void RemoveRoute(Route &route);
	void Process(int direction);
	void ProcessDestinationDispatch();
	void ProcessRoutes();
	void GetFloorRange();

	int bottom_floor;
	int top_floor;

	struct ElevMap
	{
		int number; //elevator number
		bool arrived; //has elevator arrived at call floor?
		int arrival_floor; //arrival floor
		bool arrival_direction; //arrival direction
		std::vector<int> assigned_destination;
		bool assigned;
		int destination_floor;
		int call_floor;
		bool use_call_floor;
	};

	std::vector<ElevMap> Elevators; //controller object array

	struct Route
	{
		int starting_floor;
		int destination_floor;
		int requests;
		bool processed;
		int assigned_elevator;
		CallStation* station;
	};

	std::vector<Route> Routes; //destination dispatch requests

	std::vector<CallStation*> CallStations; //call station registrations
};

}

#endif
