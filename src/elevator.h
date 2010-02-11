/* $Id$ */

/*
	Scalable Building Simulator - Elevator Subsystem Class
	The Skyscraper Project - Version 1.6 Alpha
	Copyright (C)2004-2010 Ryan Thoryk
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
#include "elevatordoor.h"
#include "floorindicator.h"

class SBSIMPEXP Elevator
{
	friend class ElevatorDoor;

public:
	Object *object; //SBS object
	int Number; //elevator number
	csString Name; //elevator name
	int NumDoors; //number of elevator doors
	bool Created; //has elevator been created with the CreateElevator function?
	int QueuePositionDirection; //queue processing direction
	int LastQueueDirection; //last queue processing direction
	int LastQueueFloor[2]; //last route added to either queue; element 0 is the floor, and element 1 is the direction
	float ElevatorSpeed; //maximum elevator speed
	bool MoveElevator; //Tells elevator to start going to specified floor
	int GotoFloor; //floor to move elevator to
	float Acceleration; //percentage of speed increase
	float Deceleration; //deceleration value; may be removed
	float AccelJerk; //acceleration jerk rate (rate of change in acceleration; by percentage)
	float DecelJerk;
	int OriginFloor; //elevator starting floor
	csVector3 Origin; //3D elevator origin vector
	double TempDeceleration; //temporary deceleration value, used in overrun correction
	float ErrorOffset;
	float DistanceToTravel; //distance in Y to travel
	double ElevatorRate;
	csArray<int> ServicedFloors; //list of floors this elevator services
	int AssignedShaft; //shaft number this elevator is assigned to
	bool IsEnabled; //true if elevator is enabled
	int Direction; //-1=down, 1=up, 0=stopped
	float Height; //elevator height
	bool IsMoving; //is elevator moving?
	csString CarStartSound; //elevator start/speedup sound
	csString CarMoveSound; //elevator move sound
	csString CarStopSound; //elevator stop/slowdown sound
	csString CarIdleSound; //elevator idle sound
	csString MotorStartSound; //motor start/speedup sound
	csString MotorRunSound; //motor move sound
	csString MotorStopSound; //motor stop/slowdown sound
	csString MotorIdleSound; //motor idle sound
	csString AlarmSound; //alarm sound (loop)
	csString AlarmSoundStop; //alarm stopping sound
	csString BeepSound; //floor beep sound (played when changing floors)
	csString FloorSound; //prefix of sounds played when reaching a certain floor; usually used for voices
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
	bool OnFloor; //true if elevator is stopped on a floor, false if not
	bool ManualGo; //go toggle for inspection service mode
	bool AlarmActive; //true if alarm is active
	bool UseFloorBeeps; //true if floor beeps should be used
	bool UseFloorSounds; //true if floor sounds should be used
	csVector3 MotorPosition; //position of motor sound emitter
	bool QueueResets; //true if system should use automatic queue resets for normal operation
	csArray<WallObject*> elevator_walls;
	float CameraOffset; //camera vertical offset for movement
	int ParkingFloor; //floor to automatically park the elevator on when idle
	float ParkingDelay; //time to wait in idle mode before parking
	bool Leveling; //is elevator in leveling mode?
	float LevelingSpeed; //leveling speed
	float LevelingOffset; //leveling offset factor
	float LevelingOpen; //leveling door open offset
	bool WaitForDoors; //set to true for the MoveElevatorToFloor() function to wait for the doors to close before running

	//functions
	Elevator(int number);
	~Elevator();
	Object* CreateElevator(bool relative, float x, float z, int floor);
	void AddRoute(int floor, int direction, bool change_light);
	void DeleteRoute(int floor, int direction);
	void CancelLastRoute();
	void Alarm();
	void StopElevator();
	void OpenHatch();
	void ProcessCallQueue();
	int GetFloor();
	void MonitorLoop();
	const csVector3 GetPosition();
	WallObject* AddWall(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th);
	WallObject* AddFloor(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, float tw, float th);
	Object* AddFloorIndicator(const char *texture_prefix, const char *direction, float CenterX, float CenterZ, float width, float height, float voffset);
	Object* CreateButtonPanel(const char *texture, int rows, int columns, const char *direction, float CenterX, float CenterZ, float width, float height, float voffset, float spacingX, float spacingY, float tw, float th);
	void DumpQueues();
	void Enabled(bool value);
	void EnableObjects(bool value);
	bool IsElevator(csRef<iMeshWrapper> test);
	bool IsInElevator(const csVector3 &position);
	csHitBeamResult HitBeam(const csVector3 &start, const csVector3 &end);
	float GetElevatorStart();
	float GetDestination();
	float GetStoppingDistance();
	bool GetBrakeStatus();
	bool GetEmergencyStopStatus();
	void DumpServicedFloors();
	void AddServicedFloor(int number);
	void RemoveServicedFloor(int number);
	void UpdateFloorIndicators();
	double GetJerkRate();
	double GetJerkPosition();
	void SetFloorSkipText(const char *id);
	const char* GetFloorSkipText();
	bool IsServicedFloor(int floor);
	bool InServiceMode();
	void Go(int floor);
	void EnableACP(bool value);
	void EnableUpPeak(bool value);
	void EnableDownPeak(bool value);
	void EnableIndependentService(bool value);
	void EnableInspectionService(bool value);
	void EnableFireService1(int value);
	void EnableFireService2(int value);
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
	void AddDirectionalIndicators(bool relative, bool active_direction, bool single, bool vertical, const char *BackTexture, const char *uptexture, const char *uptexture_lit, const char *downtexture, const char *downtexture_lit, float CenterX, float CenterZ, float voffset, const char *direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th);
	Object* AddDirectionalIndicator(bool active_direction, bool single, bool vertical, const char *BackTexture, const char *uptexture, const char *uptexture_lit, const char *downtexture, const char *downtexture_lit, float CenterX, float CenterZ, float voffset, const char *direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th);
	void SetDirectionalIndicators(bool UpLight, bool DownLight);
	void EnableDirectionalIndicators(bool value);
	ElevatorDoor* GetDoor(int number);
	void OpenDoorsEmergency(int number = 0, int whichdoors = 1, int floor = 0);
	void CloseDoorsEmergency(int number = 0, int whichdoors = 1, int floor = 0);
	void OpenDoors(int number = 0, int whichdoors = 1, int floor = 0, bool manual = false);
	void CloseDoors(int number = 0, int whichdoors = 1, int floor = 0, bool manual = false);
	void StopDoors(int number = 0);
	void ShaftDoorsEnabled(int number, int floor, bool value);
	void ShaftDoorsEnabledRange(int number, int floor, int range);
	bool AreDoorsOpen(int number = 0);
	bool AreShaftDoorsOpen(int number, int floor);
	float GetCurrentDoorSpeed(int number = 0);
	void ResetDoorTimer(int number = 0);
	bool DoorsStopped(int number = 0);
	Object* AddDoors(int number, const char *lefttexture, const char *righttexture, float thickness, float CenterX, float CenterZ, float width, float height, bool direction, float tw, float th);
	int AddShaftDoors(int number, const char *lefttexture, const char *righttexture, float thickness, float CenterX, float CenterZ, float tw, float th);
	Object* AddShaftDoor(int floor, int number, const char *lefttexture, const char *righttexture, float tw, float th);
	void Chime(int number, int floor, bool direction);
	void MoveDoors(int number, const csVector3 position, bool relative_x, bool relative_y, bool relative_z);
	void MoveDoorSound(int number, const csVector3 position, bool relative_x, bool relative_y, bool relative_z);
	void EnableDoors(bool value);
	void SetShaftDoors(int number, float thickness, float CenterX, float CenterZ);
	void AddFloorSigns(int door_number, bool relative, const char *texture_prefix, const char *direction, float CenterX, float CenterZ, float width, float height, float voffset);
	void SetCallButtons(int floor, bool direction, bool value);
	bool IsIdle();
	void QueueReset();
	void SetBeepSound(const char *filename);
	void SetFloorSound(const char *prefix);
	Object* AddSound(const char *name, const char *filename, csVector3 position, int volume = 1.0, int speed = 100, float min_distance = 1.0, float max_distance = -1.0, float dir_radiation = 0, csVector3 direction = 0);
	void DeleteActiveRoute();
	bool IsQueueActive();
	bool BeyondDecelMarker(int direction, float destination);
	void Report(const char *message);
	void ReportError(const char *message);
	Object* AddDoorComponent(int number, const char *name, const char *texture, const char *sidetexture, float thickness, const char *direction, float speed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th);
	Object* AddShaftDoorComponent(int number, int floor, const char *name, const char *texture, const char *sidetexture, float thickness, const char *direction, float speed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th);
	void AddShaftDoorsComponent(int number, const char *name, const char *texture, const char *sidetexture, float thickness, const char *direction, float speed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th);
	Object* FinishDoors(int number);
	Object* FinishShaftDoor(int number, int floor);
	bool FinishShaftDoors(int number);
	ButtonPanel* GetPanel(int index);

private:
	csRef<iMeshWrapper> ElevatorMesh; //elevator mesh object
		csRef<iThingFactoryState> Elevator_state;
		csRef<iMovable> Elevator_movable;

	//elevator parking timer
	class Timer : public wxTimer
	{
	public:
		Elevator *elevator;
		Timer(Elevator *elev)
		{
			elevator = elev;
		};
		virtual void Notify();
	};

	//parking timer object
	Timer *timer;

	//Internal elevator simulation data
	csArray<int> UpQueue; //up call queue
	csArray<int> DownQueue; //down call queue
	float ElevatorStart; //elevator vertical starting location
	int ElevatorFloor; //current elevator floor
	float Destination; //elevator destination Y value
	float StoppingDistance; //distance needed to stop the elevator
	bool CalculateStoppingDistance;
	bool Brakes; //internal brake status
	bool EmergencyStop; //internal stop status
	double JerkRate; //current jerk value, used as an acceleration/deceleration multiplier
	double JerkPos; //temporary storage for the elevator rate at the end of the jerkrate increments
	bool ResetUpQueue; //clear up queue and open doors; usually for service mode
	bool ResetDownQueue; //clear down queue and open doors; usually for service mode
	int ActiveCallFloor; //floor number of active call (that the elevator's currently responding too)
	int ActiveCallDirection; //direction of active call (that the elevator's currently responding too)
	bool FirstRun; //used for setting first-run items in the run loop

	//functions
	void MoveElevatorToFloor();
	void FinishMove();
	bool CheckOpenDoor();

	char intbuffer[65];
	char buffer[20];

	//sound objects
	Sound *mainsound;
	Sound *idlesound;
	Sound *motorsound;
	Sound *alarm;
	Sound *floorbeep;
	Sound *floorsound;
	csArray<Sound*> sounds; //generic sounds

	//interior directional indicators
	csArray<DirectionalIndicator*> DirIndicatorArray;

	//doors and shaft doors
	csArray<ElevatorDoor*> DoorArray;

	//floor indicators
	csArray<FloorIndicator*> FloorIndicatorArray;

	//button panel array
	csArray<ButtonPanel*> PanelArray; //elevator button panel objects

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
	ElevatorDoor* lastdoor_result;
	int lastdoor_number;
	csVector3 elevposition;
	double tmpDecelJerk;
	bool FinishedMove;

	//cache objects for IsInElevator()
	csVector3 lastposition;
	bool lastcheckresult;
	bool checkfirstrun;
};

#endif
