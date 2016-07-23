/* $Id$ */

/*
	Scalable Building Simulator - Elevator Object
	The Skyscraper Project - Version 1.10 Alpha
	Copyright (C)2004-2016 Ryan Thoryk
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

namespace SBS {

class SBSIMPEXP Elevator : public Object
{
	friend class ElevatorDoor;
	friend class ElevatorCar;

public:
	int Number; //elevator number
	std::string Name; //elevator name
	std::string Type; //type of elevator: standard, express, service, freight
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
	float TempDeceleration; //temporary deceleration value, used in overrun correction
	float ErrorOffset;
	float DistanceToTravel; //distance in Y to travel
	float ElevatorRate;
	int AssignedShaft; //shaft number this elevator is assigned to
	bool IsEnabled; //true if elevator is enabled
	int Direction; //-1=down, 1=up, 0=stopped
	bool HeightSet;
	bool IsMoving; //is elevator moving?
	std::string MotorUpStartSound; //motor up start/speedup sound
	std::string MotorUpRunSound; //motor up move sound
	std::string MotorUpStopSound; //motor up stop/slowdown sound
	std::string MotorDownStartSound; //motor down start/speedup sound
	std::string MotorDownRunSound; //motor down move sound
	std::string MotorDownStopSound; //motor down stop/slowdown sound
	std::string MotorIdleSound; //motor idle sound
	std::string MotorEmergencyStopSound; //motor emergency stop sound
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
	bool ManualUp; //up toggle for inspection service mode
	bool ManualDown;  //down toggle for inspection service mode
	Ogre::Vector3 MotorPosition; //position of motor sound emitter
	bool QueueResets; //true if system should use automatic queue resets for normal operation
	int ParkingFloor; //floor to automatically park the elevator on when idle
	float ParkingDelay; //time to wait in idle mode before parking
	bool Leveling; //is elevator in leveling mode?
	float LevelingSpeed; //leveling speed
	float LevelingOffset; //leveling offset factor
	float LevelingOpen; //leveling door open offset
	bool WaitForDoors; //set to true for the MoveElevatorToFloor() function to wait for the doors to close before running
	int ActiveDirection; //variant of Direction that doesn't change during slowdown
	int NotifyEarly; //perform arrival notification earlier (0 for normal, 1 for at start of leveling, 2 for at start of decel)
	bool Notified; //true if arrival notification has been performed
	bool Parking; //is elevator parking?
	float DepartureDelay; //delay in seconds between door closure and elevator departure
	float ArrivalDelay; //delay in seconds between elevator arrival and door opening
	bool WaitForTimer; //true if elevator is waiting for the arrival/departure timers to finish before moving
	float InspectionSpeed; //inspection service speed multiplier
	bool LimitQueue; //true to only allow floor selections in the same queue direction
	bool ReOpen; //true if elevator should reopen doors if the same floor is selected
	int LastChimeDirection; //direction of last arrival chime
	bool AutoDoors; //true if doors should be automatic (automatically open when reaching destination and refuse to open if off floor)
	bool OpenOnStart; //true if doors should automatically open on simulator start
	int ManualMove; //0 if manual movement is off; -1 for down, 1 for up
	bool ManualMoveHold; //true if ManualMove is in hold mode
	bool Interlocks; //lock doors during movement
	bool GoActive; //true if go function is in use (mouse hold is active)
	bool FloorHold; //true if floor buttons need to be held for the elevator to proceed to the floor (modern manual mode)
	bool GoPending; //true if Go() function is active
	float EmergencyStopSpeed; //emergency stop deceleration multiplier
	bool AutoAdjustSound; //auto-adjust stopping sounds
	bool SkipFloorSound; //skip playing floor sound, for use in FinishMove()

	//functions
	Elevator(Object *parent, int number);
	~Elevator();
	bool CreateElevator(bool relative, float x, float z, int floor);
	bool AddRoute(int floor, int direction, int call_type);
	bool DeleteRoute(int floor, int direction);
	bool CallCancel();
	bool Stop(bool emergency = false);
	void ProcessCallQueue();
	void Loop();
	void DumpQueues();
	void Enabled(bool value);
	bool IsInElevator(const Ogre::Vector3 &position, bool camera = false);
	float GetElevatorStart();
	float GetDestination();
	float GetStoppingDistance();
	bool GetBrakeStatus();
	int GetEmergencyStopStatus();
	void UpdateFloorIndicators();
	float GetJerkRate();
	float GetJerkPosition();
	void SetFloorSkipText(const std::string &id);
	std::string GetFloorSkipText();
	bool InServiceMode();
	bool Go(int floor, bool hold = false);
	bool EnableACP(bool value);
	bool EnableUpPeak(bool value);
	bool EnableDownPeak(bool value);
	bool EnableIndependentService(bool value);
	bool EnableInspectionService(bool value);
	bool EnableFireService1(int value);
	bool EnableFireService2(int value, bool force = false);
	bool SetRecallFloor(int floor);
	bool SetAlternateRecallFloor(int floor);
	bool SetACPFloor(int floor);
	void GoToRecallFloor();
	bool MoveUp();
	bool MoveDown();
	bool SetGoButton(bool value);
	bool SetUpButton(bool value);
	bool SetDownButton(bool value);
	int GetTopFloor();
	int GetBottomFloor();
	void UpdateDirectionalIndicators();
	void NotifyCallButtons(int floor, bool direction);
	bool IsIdle();
	void ResetQueue(bool up, bool down, bool stop_if_empty = true);
	void DeleteActiveRoute();
	bool IsQueueActive();
	bool BeyondDecelMarker(int direction, float destination);
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	bool IsQueued(int floor);
	void NotifyArrival(int floor);
	void SetRunState(bool value);
	bool IsRunning();
	bool GetArrivalDirection(int floor);
	float GetDestinationAltitude(int floor);
	float GetDestinationOffset(int floor);
	void MoveObjects(float offset);
	void SetFloor(int floor);
	void OnInit();
	bool GetCallButtonStatus(int floor, bool &Up, bool &Down);
	int AvailableForCall(int floor, int direction, bool report_on_failure = true);
	bool SelectFloor(int floor);
	bool Check(Ogre::Vector3 position);
	int GetNearestServicedFloor();
	bool ReturnToNearestFloor();
	bool IsLeveled();
	bool Up();
	bool Down();
	bool Up(bool value);
	bool Down(bool value);
	Shaft* GetShaft();
	CallButton* GetPrimaryCallButton();
	int GetActiveCallFloor();
	int GetActiveCallDirection();
	int GetActiveCallType();
	bool InElevator();
	bool PeakWaiting();
	bool OnRecallFloor();
	int GetActiveRecallFloor();
	std::vector<Floor*> GetLobbies();
	bool IsStopped();
	void CancelHallCall(int floor, int direction);
	bool IsManuallyStopped();
	bool IsOnFloor(int floor);
	DynamicMesh* GetDoorContainer() { return DoorContainer; }
	bool CheckInterlocks(bool skip_current_floor = false);
	bool AreDoorsOpen();
	int AreDoorsMoving(bool car_doors = true, bool shaft_doors = true);
	bool AreShaftDoorsClosed(bool skip_current_floor = false);
	bool DoorsStopped();
	bool AreDoorsOpening(bool car_doors = true, bool shaft_doors = true);
	bool AreDoorsClosing(bool car_doors = true, bool shaft_doors = true);
	bool IsServicedFloor(int floor, bool report = true);
	void ChangeLight(int floor, bool value);
	void ResetLights();
	void OpenDoors();
	void CloseDoors();
	bool GetHoldStatus();
	bool OnTopFloor();
	bool OnBottomFloor();

	ElevatorCar* CreateCar();
	ElevatorCar* GetCar(int number);
	int GetCarCount();
	ElevatorCar* GetCarForFloor(int number);

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

	class Timer; //internal timer class

	//parking timer object
	Timer *parking_timer;

	//arrival and departure delay timers
	Timer *arrival_delay;
	Timer *departure_delay;

	//Internal elevator simulation data
	std::vector<ElevatorCar*> Cars; //car objects
	std::vector<QueueEntry> UpQueue; //up call queue
	std::vector<QueueEntry> DownQueue; //down call queue
	float ElevatorStart; //elevator vertical starting location
	float Destination; //elevator destination Y value
	float StoppingDistance; //distance needed to stop the elevator
	bool CalculateStoppingDistance;
	bool Brakes; //internal brake status
	int EmergencyStop; //internal stop status; 0 for off, 1 for standard stop, 2 for emergency stop
	float JerkRate; //current jerk value, used as an acceleration/deceleration multiplier
	float JerkPos; //temporary storage for the elevator rate at the end of the jerkrate increments
	QueueEntry ActiveCall; //active call (that the elevator's currently responding to)
	bool FirstRun; //used for setting first-run items in the run loop
	bool Running; //is elevator in run mode?
	bool UpQueueEmpty;
	bool DownQueueEmpty;
	bool UpCall;
	bool DownCall;
	bool QueuePending; //true if either queue has changed, and needs to be processed
	int GoActiveFloor; //associated floor number for GoActive flag
	bool ManualStop; //true if elevator has been manually stopped

	//functions
	void MoveElevatorToFloor();
	void FinishMove();
	void PlayStartingSounds();
	void PlayStoppingSounds(bool emergency = false);
	void PlayMovingSounds();
	void HandleDequeue(int direction, bool stop_if_empty = true);
	void StopSounds();

	//motor sound objects
	Sound *motorsound;
	Sound *motoridlesound;

	//mesh container for elevator doors (not shaft doors)
	DynamicMesh *DoorContainer;

	//elevator misc internals
	bool MovementRunning;
	int oldfloor;
	int lastfloor;
	bool lastfloorset;
	std::string FloorSkipText;
	bool RecallSet;
	bool RecallAltSet;
	bool ACPFloorSet;
	bool RecallUnavailable; //true if recall floor is unavailable (alarm has been triggered on that floor)
	Ogre::Vector3 elevposition;
	float tmpDecelJerk;
	bool FinishedMove;
	bool SoundsQueued;
};

}

#endif
