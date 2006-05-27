/*
	Scalable Building Simulator - Elevator Subsystem Class
	The Skyscraper Project - Version 1.1 Alpha
	Copyright ©2005-2006 Ryan Thoryk
	http://www.tliquest.net/skyscraper
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

#ifndef _SBS_ELEVATOR_H
#define _SBS_ELEVATOR_H

#include "globals.h"

class Elevator
{
public:

	int Number; //elevator number
	int QueuePositionDirection; //queue processing direction
	bool PauseQueueSearch; //pause queue processor
	double ElevatorSpeed; //maximum elevator speed
	bool ElevatorSync; //true if user should move with elevator
	bool MoveElevator; //Tells elevator to start going to specified floor
    int MoveElevatorFloor; //floor to move elevator to
	int GotoFloor; //floor to go to
	int OpenDoor; //1=open doors, -1=close doors
	double Acceleration; //percentage of speed increase
	double OpenSpeed; //elevator opening/closing speed

	//functions
	Elevator(int number);
	~Elevator();
	void CreateElevator(double x, double y, int floor, int direction);
	void AddRoute(int floor, int direction);
	void DeleteRoute(int floor, int direction);
	void Alarm();
	void CallElevator(int floor, int direction);
	void StopElevator();
	void OpenHatch();
	void OpenDoorsEmergency();
	void OpenShaftDoors(int floor);
	void ProcessCallQueue();
	int GetElevatorFloor();
	void MonitorLoop();
	void CloseDoorsEmergency();
	csVector3 GetPosition();
	void AddWall(const char *texture, double x1, double z1, double x2, double z2, double height, double voffset, double tw, double th);
	void AddFloor(const char *texture, double x1, double z1, double x2, double z2, double voffset, double tw, double th);
	void AddFloorIndicator(const char *texture, double x1, double z1, double x2, double z2, double height, double voffset, double tw, double th);
	void AddButtonPanel(const char *texture, double x1, double z1, double x2, double z2, double height, double voffset, double tw, double th);
	void AddPanels(const char *texture, double x1, double z1, double x2, double z2, double height, double voffset, double tw, double th);
	void AddDoors(const char *texture, double x1, double z1, double x2, double z2, double height, double voffset, double tw, double th);
	void AddPlaque(const char *texture, double x1, double z1, double x2, double z2, double height, double voffset, double tw, double th);
	void AddCallButtons(const char *texture, double x1, double z1, double x2, double z2, double height, double voffset, double tw, double th);

private:
	csRef<iMeshWrapper> ElevatorMesh; //elevator mesh object
		csRef<iMeshObject> Elevator_object;
		csRef<iMeshObjectFactory> Elevator_factory;
		csRef<iThingFactoryState> Elevator_state;
	csRef<iMeshWrapper> FloorIndicator; //floor indicator object
		csRef<iMeshObject> FloorIndicator_object;
		csRef<iMeshObjectFactory> FloorIndicator_factory;
		csRef<iThingFactoryState> FloorIndicator_state;
	csRef<iMeshWrapper> ElevatorDoorL; //left inside door
		csRef<iMeshObject> ElevatorDoorL_object;
		csRef<iMeshObjectFactory> ElevatorDoorL_factory;
		csRef<iThingFactoryState> ElevatorDoorL_state;
	csRef<iMeshWrapper> ElevatorDoorR; //right inside door
		csRef<iMeshObject> ElevatorDoorR_object;
		csRef<iMeshObjectFactory> ElevatorDoorR_factory;
		csRef<iThingFactoryState> ElevatorDoorR_state;
	csRef<iMeshWrapper> Plaque; //plaque object
		csRef<iMeshObject> Plaque_object;
		csRef<iMeshObjectFactory> Plaque_factory;
		csRef<iThingFactoryState> Plaque_state;
	csRef<iMeshWrapper> CallButtonsUp; //up call button
		csRef<iMeshObject> CallButtonsUp_object;
		csRef<iMeshObjectFactory> CallButtonsUp_factory;
		csRef<iThingFactoryState> CallButtonsUp_state;
	csRef<iMeshWrapper> CallButtonsDown; //down call button
		csRef<iMeshObject> CallButtonsDown_object;
		csRef<iMeshObjectFactory> CallButtonsDown_factory;
		csRef<iThingFactoryState> CallButtonsDown_state;
	csRefArray<iMeshWrapper> Buttons; //elevator button array

	//Internal elevator simulation data
	csString UpQueue; //up call queue ***Change these to sorted arrays
	csString DownQueue; //down call queue
	double ElevatorStart; //elevator vertical starting location
	int ElevatorFloor; //current elevator floor
	bool DoorsOpen; //elevator door state
	int ElevatorDirection; //-1=down, 1=up, 0=stopped
	double DistanceToTravel; //distance in Y to travel
	double Destination; //elevator destination Y value
	double ElevatorRate;
	double StoppingDistance;
	bool CalculateStoppingDistance;
	bool Brakes;
	double ElevatorDoorSpeed;
	double ElevatorDoorPos; //original elevator door position
	bool ElevWait;

	//functions
	void OpenDoors();
	void CloseDoors();
	void MoveElevatorToFloor();

};

#endif
