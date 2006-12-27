/* $Id$ */

/*
	Scalable Building Simulator - Elevator Subsystem Class
	The Skyscraper Project - Version 1.1 Alpha
	Copyright (C)2005-2006 Ryan Thoryk
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
#include "buttonpanel.h"

class Elevator
{
public:

	int Number; //elevator number
	csString Name; //elevator name
	int QueuePositionDirection; //queue processing direction
	bool PauseQueueSearch; //pause queue processor
	int LastQueueFloor[1]; //last route added to either queue
	double ElevatorSpeed; //maximum elevator speed
	bool MoveElevator; //Tells elevator to start going to specified floor
	int MoveElevatorFloor; //floor to move elevator to
	int GotoFloor; //floor to go to
	double Acceleration; //percentage of speed increase
	double Deceleration; //deceleration value; may be removed
	double OpenSpeed; //elevator opening/closing speed
	int OriginFloor; //elevator starting floor
	csVector3 Origin; //3D elevator origin vector
	bool DoorDirection; //if direction is false, doors are on the left/right side
	double DoorAcceleration; //door acceleration
	double TempDeceleration; //temporary deceleration value, used in overrun correction
	double ErrorOffset;
	double DistanceToTravel; //distance in Y to travel
	double ElevatorRate;
	double DoorWidth; //elevator door width
	double DoorHeight; //elevator door height
	csVector3 DoorOrigin; //elevator door origin
	csVector3 ShaftDoorOrigin; //shaft door origin
	csArray<int> ServicedFloors; //list of floors this elevator services
	int AssignedShaft; //shaft number this elevator is assigned to
	bool IsEnabled; //true if elevator is enabled
	int Direction; //-1=down, 1=up, 0=stopped
	double Height; //elevator height
	ButtonPanel *Panel; //elevator button panel object

	//functions
	Elevator(int number);
	~Elevator();
	void CreateElevator(double x, double z, int floor);
	void AddRoute(int floor, int direction);
	void DeleteRoute(int floor, int direction);
	void CancelLastRoute();
	void Alarm();
	void StopElevator();
	void OpenHatch();
	void OpenDoorsEmergency(int whichdoors = 1, int floor = 0);
	void ProcessCallQueue();
	int GetFloor();
	void MonitorLoop();
	void CloseDoorsEmergency(int whichdoors = 1, int floor = 0);
	const csVector3 GetPosition();
	void OpenDoors(int whichdoors = 1, int floor = 0);
	void CloseDoors(int whichdoors = 1, int floor = 0);
	int AddWall(const char *name, const char *texture, double x1, double z1, double x2, double z2, double height1, double height2, double voffset1, double voffset2, double thickness, double tw, double th, bool revX, bool revY, bool revZ);
	int AddFloor(const char *name, const char *texture, double x1, double z1, double x2, double z2, double voffset1, double voffset2, double thickness, double tw, double th);
	int AddFloorIndicator(const char *direction, double CenterX, double CenterZ, double width, double height, double voffset);
	int AddDoors(const char *texture, double CenterX, double CenterZ, double width, double height, bool direction, double thickness, double tw, double th);
	int AddShaftDoors(const char *texture, double CenterX, double CenterZ, double thickness, double tw, double th);
	int AddPlaque(const char *texture, double x1, double z1, double x2, double z2, double height, double voffset, double tw, double th);
	void CreateButtonPanel(const char *texture, int rows, int columns, const char *direction, double CenterX, double CenterZ, double width, double height, double voffset, double spacing, double tw, double th);
	void DumpQueues();
	void Enabled(bool value);
	void ShaftDoorsEnabled(int floor, bool value);
	bool IsElevator(csRef<iMeshWrapper> test);
	bool IsInElevator(const csVector3 &position);
	csHitBeamResult HitBeam(const csVector3 &start, const csVector3 &end);
	double GetElevatorStart();
	bool AreDoorsOpen();
	double GetDestination();
	double GetStoppingDistance();
	bool GetBrakeStatus();
	double GetCurrentDoorSpeed();
	bool GetEmergencyStopStatus();
	void DumpServicedFloors();
	void AddServicedFloor(int number);
	void RemoveServicedFloor(int number);
	void UpdateFloorIndicators();

private:
	csRef<iMeshWrapper> ElevatorMesh; //elevator mesh object
		csRef<iThingFactoryState> Elevator_state;
		csRef<iMovable> Elevator_movable;
	csRef<iMeshWrapper> FloorIndicator; //floor indicator object
		csRef<iThingFactoryState> FloorIndicator_state;
		csRef<iMovable> FloorIndicator_movable;
	csRef<iMeshWrapper> ElevatorDoorL; //left inside door
		csRef<iThingFactoryState> ElevatorDoorL_state;
		csRef<iMovable> ElevatorDoorL_movable;
	csRef<iMeshWrapper> ElevatorDoorR; //right inside door
		csRef<iThingFactoryState> ElevatorDoorR_state;
		csRef<iMovable> ElevatorDoorR_movable;
	csRef<iMeshWrapper> Plaque; //plaque object
		csRef<iThingFactoryState> Plaque_state;
		csRef<iMovable> Plaque_movable;

	//Internal elevator simulation data
	csArray<int> UpQueue; //up call queue
	csArray<int> DownQueue; //down call queue
	double ElevatorStart; //elevator vertical starting location
	int ElevatorFloor; //current elevator floor
	bool DoorsOpen; //elevator door state
	csArray<bool> ShaftDoorsOpen; //shaft door state
	int OpenDoor; //1=open doors, -1=close doors
	double Destination; //elevator destination Y value
	double StoppingDistance; //distance needed to stop the elevator
	bool CalculateStoppingDistance;
	bool Brakes;
	double ElevatorDoorSpeed;
	bool ElevWait;
	double FPSModifierStatic;
	bool EmergencyStop;
	int WhichDoors;
	int ShaftDoorFloor;
	csRefArray<iMeshWrapper> ShaftDoorL; //shaft door array
	csRefArray<iThingFactoryState> ShaftDoorL_state; //shaft door array state
	csRefArray<iMeshWrapper> ShaftDoorR; //shaft door array
	csRefArray<iThingFactoryState> ShaftDoorR_state; //shaft door array state
	csRef<iMaterialWrapper> orig_indicator;

	//functions
	void MoveElevatorToFloor();
	void MoveDoors(bool open, bool emergency);

	char intbuffer[65];
	char buffer[20];

};

#endif
