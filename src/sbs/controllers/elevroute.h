/*
	Scalable Building Simulator - Elevator Route Controller
	The Skyscraper Project - Version 2.1
	Copyright (C)2004-2025 Ryan Thoryk
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

#ifndef _SBS_ELEVROUTE_H
#define _SBS_ELEVROUTE_H

namespace SBS {

class SBSIMPEXP RouteController : public Object
{
	friend class Elevator;
	friend class ElevatorCar;

public:

	int QueuePositionDirection; //queue processing direction
	int LastQueueDirection; //last queue processing direction
	int LastQueueFloor[2]; //last route added to either queue; element 0 is the floor, and element 1 is the direction
	bool LimitQueue; //true to only allow floor selections in the same queue direction
	bool QueueResets; //true if system should use automatic queue resets for normal operation

	//functions
	explicit RouteController(Elevator *parent);
	~RouteController();
	bool AddRoute(int floor, int direction, int call_type);
	bool DeleteRoute(int floor, int direction);
	bool RouteExists(bool direction, int floor);
	bool CallCancel();
	bool CallCancelAll();
	void ProcessCallQueue();
	void DumpQueues();
	void ResetQueue(bool up, bool down, bool stop_if_empty = true);
	void DeleteActiveRoute();
	bool IsQueueActive();
	bool IsQueued(int floor, int queue = 0);
	int GetActiveCallFloor();
	int GetActiveCallDirection();
	int GetActiveCallType();

private:

	struct QueueEntry
	{
		int floor; //floor number
		int call_type; //0 = car call, 1 = hall call, 2 = system call
		int car; //car number
		int direction; //queue direction

		QueueEntry()
		{
			floor = 0;
			call_type = 0;
			car = 0;
			direction = 0;
		}

		QueueEntry(int floor, int call_type, int car, int direction)
		{
			this->floor = floor;
			this->call_type = call_type;
			this->car = car;
			this->direction = direction;
		}

		bool operator < (const QueueEntry& element) const
		{
			return floor < element.floor;
		}
	};


	Elevator* elevator;

	void HandleDequeue(int direction, bool stop_if_empty = true);
	void Report(const std::string &message);
	bool ReportError(const std::string &message);

	std::vector<QueueEntry> UpQueue; //up call queue
	std::vector<QueueEntry> DownQueue; //down call queue
	QueueEntry ActiveCall; //active call (that the elevator's currently responding to)
	bool UpQueueEmpty;
	bool DownQueueEmpty;
	bool UpCall;
	bool DownCall;
	bool QueuePending; //true if either queue has changed, and needs to be processed
};

}

#endif
