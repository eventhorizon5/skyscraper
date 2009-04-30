/* $Id$ */

/*
	Scalable Building Simulator - Elevator Subsystem Class
	The Skyscraper Project - Version 1.4 Alpha
	Copyright (C)2005-2009 Ryan Thoryk
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

#ifndef _SBS_ELEVATOR_H
#define _SBS_ELEVATOR_H

#include "buttonpanel.h"
#include "sound.h"
#include "directional.h"

class SBSIMPEXP Elevator
{
public:

	int Number; //elevator number
	csString Name; //elevator name
	int QueuePositionDirection; //queue processing direction
	bool PauseQueueSearch; //pause queue processor
	int LastQueueFloor[2]; //last route added to either queue; element 0 is the floor, and element 1 is the direction
	float ElevatorSpeed; //maximum elevator speed
	bool MoveElevator; //Tells elevator to start going to specified floor
	int GotoFloor; //floor to move elevator to
	float Acceleration; //percentage of speed increase
	float Deceleration; //deceleration value; may be removed
	float AccelJerk; //acceleration jerk rate (rate of change in acceleration; by percentage)
	float DecelJerk;
	float OpenSpeed; //elevator opening/closing speed
	int OriginFloor; //elevator starting floor
	csVector3 Origin; //3D elevator origin vector
	bool DoorDirection; //if direction is false, doors are on the left/right side
	float DoorAcceleration; //door acceleration
	float TempDeceleration; //temporary deceleration value, used in overrun correction
	float ErrorOffset;
	float DistanceToTravel; //distance in Y to travel
	float ElevatorRate;
	float DoorWidth; //elevator door width
	float DoorHeight; //elevator door height
	csVector3 DoorOrigin; //elevator door origin
	csVector3 ShaftDoorOrigin; //shaft door origin
	csArray<int> ServicedFloors; //list of floors this elevator services
	int AssignedShaft; //shaft number this elevator is assigned to
	bool IsEnabled; //true if elevator is enabled
	int Direction; //-1=down, 1=up, 0=stopped
	float Height; //elevator height
	ButtonPanel *Panel; //elevator button panel object
	ButtonPanel *Panel2; //elevator button panel object
	int DoorTimer; //door autoclose timer value, in milliseconds
	bool IsMoving; //is elevator moving?
	csString OpenSound; //door open sound
	csString CloseSound; //door close sound
	csString StartSound; //elevator start/speedup sound
	csString MoveSound; //elevator move sound
	csString StopSound; //elevator stop/slowdown sound
	csString IdleSound; //elevator idle sound
	csString ChimeSound; //elevator chime sound
	bool UseFloorSkipText; //true if text set in SetFloorSkipText should be used
	bool ACP; //Anti-Crime Protection mode enable/disable
	int ACPFloor; //floor to stop at in ACP mode
	bool UpPeak; //Up Peak mode
	bool DownPeak; //Down Peak mode
	bool IndependentService; //Independent Service (ISC) mode
	bool InspectionService; //Inspection Service (INS) mode
	int FireServicePhase1; //Fire service (EFS) phase 1 modes: 0 for off, 1 for on, and 2 for bypass
	int FireServicePhase2; //Fire service (EFS) phase 2 modes: 0 for off, 1 for on, and 2 for hold
	int RecallFloor; //Fire service recall floor
	int RecallFloorAlternate; //Fire service alternate recall floor
	bool MovePending; //for service and fire modes; a move request is pending (waiting for doors to be manually closed)
	bool OnFloor; //true if elevator is stopped on a floor, false if not
	bool ManualGo; //go toggle for inspection service mode

	//functions
	Elevator(int number);
	~Elevator();
	bool CreateElevator(float x, float z, int floor);
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
	void OpenDoors(int whichdoors = 1, int floor = 0, bool manual = false);
	void CloseDoors(int whichdoors = 1, int floor = 0, bool manual = false);
	void StopDoors();
	int AddWall(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th);
	int AddFloor(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, float tw, float th);
	int AddFloorIndicator(const char *direction, float CenterX, float CenterZ, float width, float height, float voffset);
	int AddDoors(const char *texture, float thickness, float CenterX, float CenterZ, float width, float height, bool direction, float tw, float th);
	int AddShaftDoors(const char *texture, float thickness, float CenterX, float CenterZ, float tw, float th);
	int AddPlaque(const char *texture, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th);
	void CreateButtonPanel(const char *texture, int rows, int columns, const char *direction, float CenterX, float CenterZ, float width, float height, float voffset, float spacingX, float spacingY, float tw, float th);
	void DumpQueues();
	void Enabled(bool value);
	void EnableObjects(bool value);
	void ShaftDoorsEnabled(int floor, bool value);
	void ShaftDoorsEnabledRange(int floor, int range);
	bool IsElevator(csRef<iMeshWrapper> test);
	bool IsInElevator(const csVector3 &position);
	csHitBeamResult HitBeam(const csVector3 &start, const csVector3 &end);
	float GetElevatorStart();
	bool AreDoorsOpen();
	float GetDestination();
	float GetStoppingDistance();
	bool GetBrakeStatus();
	float GetCurrentDoorSpeed();
	bool GetEmergencyStopStatus();
	void DumpServicedFloors();
	void AddServicedFloor(int number);
	void RemoveServicedFloor(int number);
	void UpdateFloorIndicators();
	float GetJerkRate();
	float GetJerkPosition();
	void Chime(int floor);
	void SetFloorSkipText(const char *id);
	const char* GetFloorSkipText();
	bool IsServicedFloor(int floor);
	bool InServiceMode();
	void Go(int floor);
	void GoPending(int floor);
	void EnableACP(bool value);
	void EnableUpPeak(bool value);
	void EnableDownPeak(bool value);
	void EnableIndependentService(bool value);
	void EnableInspectionService(bool value);
	void EnableFireService1(int value);
	void EnableFireService2(int value);
	void ResetDoorTimer();
	bool DoorsStopped();
	bool SetRecallFloor(int floor);
	bool SetAlternateRecallFloor(int floor);
	bool SetACPFloor(int floor);
	void GoToRecallFloor();
	bool MoveUp();
	bool MoveDown();
	bool StopMove();
	void SetGoButton(bool value);
	int GetTopFloor();
	int GetBottomFloor();
	void AddDirectionalIndicators(const char *BackTexture, const char *uptexture, const char *uptexture_lit, const char *downtexture, const char *downtexture_lit, float CenterX, float CenterZ, float voffset, const char *direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th);
	void EnableDirectionalIndicator(int floor, bool value);

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
	float ElevatorStart; //elevator vertical starting location
	int ElevatorFloor; //current elevator floor
	bool DoorsOpen; //elevator door state
	csArray<bool> ShaftDoorsOpen; //shaft door state
	int OpenDoor; //1=open doors, -1=close doors
	float Destination; //elevator destination Y value
	float StoppingDistance; //distance needed to stop the elevator
	bool CalculateStoppingDistance;
	bool Brakes; //internal brake status
	float ElevatorDoorSpeed;
	bool ElevWait;
	bool EmergencyStop; //internal stop status
	int WhichDoors;
	int ShaftDoorFloor;
	csRefArray<iMeshWrapper> ShaftDoorL; //shaft door array
	csRefArray<iThingFactoryState> ShaftDoorL_state; //shaft door array state
	csRefArray<iMeshWrapper> ShaftDoorR; //shaft door array
	csRefArray<iThingFactoryState> ShaftDoorR_state; //shaft door array state
	float JerkRate; //current jerk value, used as an acceleration/deceleration multiplier
	float JerkPos; //temporary storage for the elevator rate at the end of the jerkrate increments
	csRef<iMaterialWrapper> orig_indicator;
	bool ResetQueues; //clear queues and open doors; usually for service mode

	//functions
	void MoveElevatorToFloor();
	void MoveDoors(bool open, bool manual);

	char intbuffer[65];
	char buffer[20];

	//door autoclose timer
	class Timer : public wxTimer
	{
	public:
		Elevator *elevator;
		Timer(Elevator *parent)
		{
			elevator = parent;
		};
		virtual void Notify();
	};

	//timer object
	Timer *timer;

	//sound objects
	Sound *mainsound;
	Sound *idlesound;
	Sound *doorsound;
	Sound *chime;

	//door internals
	bool DoorIsRunning;
	float OpenChange;
	float marker1;
	float marker2;
	int index;
	float stopping_distance;
	float temp_change;
	bool accelerating;
	bool previous_open;
	bool door_changed;
	int door_section; //door movement section; used for both reversal tracking and debugging
	bool quick_close; //used if user presses close button while doors are opening; results in a faster timer length
	bool doors_stopped;

	//directional indicators
	csArray<DirectionalIndicator*> IndicatorArray;

	//elevator misc internals
	bool ElevatorIsRunning;
	int oldfloor;
	int lastfloor;
	bool lastfloorset;
	csString FloorSkipText;
	bool RecallSet;
	bool RecallAltSet;
	bool ACPFloorSet;
	bool RecallUnavailable; //true if recall floor is unavailable (alarm has been triggered on that floor)
};

#endif
