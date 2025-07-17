/*
	Scalable Building Simulator - Elevator Object
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

#ifndef _SBS_ELEVATOR_H
#define _SBS_ELEVATOR_H

namespace SBS {

class SBSIMPEXP Elevator : public Object
{
	friend class ElevatorDoor;
	friend class ElevatorCar;
	friend class RouteController;

public:
	int Number; //elevator number
	std::string Name; //elevator name
	std::string ID; //identifier for Destination Dispatch
	std::string Type; //type of elevator: standard, express, service, freight
	bool Created; //has elevator been created with the CreateElevator function?
	Real UpSpeed; //maximum up elevator speed
	Real DownSpeed; //maximum down elevator speed
	bool MoveElevator; //tells elevator to start going to specified floor
	int GotoFloor; //floor to move elevator to
	int GotoFloorCar; //car that requested movement
	Real Acceleration; //percentage of speed increase
	Real Deceleration; //deceleration value; may be removed
	Real AccelJerk; //acceleration jerk rate (rate of change in acceleration; by percentage)
	Real DecelJerk;
	Real TempDeceleration; //temporary deceleration value, used in overrun correction
	Real ErrorOffset;
	Real DistanceToTravel; //distance in Y to travel
	Real ElevatorRate;
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
	int IndependentServiceCar; //car number for Independent Service mode
	bool InspectionService; //Inspection Service (INS) mode
	int FireServicePhase1; //Fire service (EFS) phase 1 modes: 0 for off, 1 for on, and 2 for bypass
	int FireServicePhase2; //Fire service (EFS) phase 2 modes: 0 for off, 1 for on, and 2 for hold
	int FireServicePhase2Car; //car number for Fire service phase 2 mode
	int RecallFloor; //Fire service recall floor
	int RecallFloorAlternate; //Fire service alternate recall floor
	bool OnFloor; //true if elevator is stopped on a floor, false if not
	bool ManualGo; //go toggle for inspection service mode
	bool ManualUp; //up toggle for inspection service mode
	bool ManualDown;  //down toggle for inspection service mode
	Vector3 MotorPosition; //position of motor sound emitter
	int ParkingFloor; //floor to automatically park the elevator on when idle
	Real ParkingDelay; //time to wait in idle mode before parking
	bool Leveling; //is elevator in leveling mode?
	Real LevelingSpeed; //leveling speed
	Real LevelingOffset; //leveling offset factor
	Real LevelingOpen; //leveling door open offset
	bool WaitForDoors; //set to true for the MoveElevatorToFloor() function to wait for the doors to close before running
	int ActiveDirection; //variant of Direction that doesn't change during slowdown
	int NotifyEarly; //perform arrival notification earlier (-1 for off, 0 for normal, 1 for at start of leveling, 2 for at start of decel, 3 for both options 0 and 2)
	bool NotifyLate; //perform arrival notification after doors have opened
	bool Notified; //true if arrival notification has been performed
	bool Parking; //is elevator parking?
	Real DepartureDelay; //delay in seconds between door closure and elevator departure
	Real ArrivalDelay; //delay in seconds between elevator arrival and door opening
	bool WaitForTimer; //true if elevator is waiting for the arrival/departure timers to finish before moving
	Real InspectionSpeed; //inspection service speed multiplier
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
	Real EmergencyStopSpeed; //emergency stop deceleration multiplier
	bool AutoAdjustSound; //auto-adjust stopping sounds
	bool SkipFloorSound; //skip playing floor sound, for use in FinishMove()
	bool ChimeOnArrival; //set to true for elevator to always chime on arrival, instead of just when responding to a hall call
	bool HoistwayAccessHold; //if true, the user needs to hold the mouse button on the control, in Hoistway Access mode
	Vector3 RopePosition; //position of rope
	std::string RopeTexture; //texture for rope
	std::string CounterweightStartSound; //counterweight starting sound
	std::string CounterweightMoveSound; //counterweight moving sound
	std::string CounterweightStopSound; //counterweight stopping sound
	bool Counterweight; //if true, elevator has a counterweight
	bool Error; //true if an error occurred during movement processing

	//functions
	Elevator(Object *parent, int number);
	~Elevator();
	bool CreateElevator(bool relative, Real x, Real z, int floor);
	Wall* CreateCounterweight(const std::string &frame_texture, const std::string &weight_texture, Real x, Real z, const Vector3 &size, Real weight_voffset);
	bool AddRails(const std::string &main_texture, const std::string &edge_texture, Real x, Real z, bool orientation, Real rail_distance, Real rail_width);
	bool AddRoute(int floor, int direction, int call_type);
	bool DeleteRoute(int floor, int direction);
	bool RouteExists(bool direction, int floor);
	bool CallCancel();
	bool CallCancelAll();
	bool Stop(bool emergency = false);
	void Loop();
	void DumpQueues();
	void Enabled(bool value);
	ElevatorCar* IsInElevator(const Vector3 &position, bool camera = false);
	Real GetElevatorStart();
	Real GetDestination();
	Real GetStoppingDistance();
	bool GetBrakeStatus();
	int GetEmergencyStopStatus();
	void UpdateFloorIndicators();
	Real GetJerkRate();
	Real GetJerkPosition();
	void SetFloorSkipText(const std::string &id);
	std::string GetFloorSkipText();
	bool InServiceMode();
	bool Go(int floor, bool hold = false);
	bool EnableACP(bool value);
	bool EnableUpPeak(bool value);
	bool EnableDownPeak(bool value);
	bool EnableIndependentService(bool value, int car_number = 0);
	bool EnableInspectionService(bool value);
	bool EnableFireService1(int value);
	bool EnableFireService2(int value, int car_number = 0, bool force = false);
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
	bool IsIdle();
	bool BeyondDecelMarker(int direction, Real destination);
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	void SetRunState(bool value);
	bool IsRunning();
	bool GetArrivalDirection(int floor);
	void MoveObjects(Real offset);
	void OnInit();
	int AvailableForCall(bool destination, int floor, int direction, bool report_on_failure = true);
	bool SelectFloor(int floor);
	bool Check(Vector3 position);
	bool ReturnToNearestFloor(bool parking = true, int car = 1);
	bool ReturnToBottomFloor();
	bool IsLeveled();
	bool Up();
	bool Down();
	bool Up(bool value);
	bool Down(bool value);
	Shaft* GetShaft();
	CallStation* GetPrimaryCallStation();
	bool InElevator();
	bool PeakWaiting();
	bool OnPeakFloor();
	bool OnRecallFloor();
	int GetActiveRecallFloor();
	std::vector<Floor*> GetLobbies();
	bool IsStopped();
	void CancelHallCall(int floor, int direction);
	bool IsManuallyStopped();
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
	bool IsNudgeModeActive();
	void ResetShaftDoors(int floor);
	void ResetDoors();
	void ResetNudgeTimers(bool start = true);
	void HoldDoors();
	void EnableNudgeMode(bool value);
	bool OnParkingFloor();
	bool SetHoistwayAccess(int floor, int access);
	void SetControls(const std::string &action_name);
	bool GetDestinationDispatch();
	int GetRecallFloor();
	void AddController(int controller);
	void RemoveController(int controller);
	bool GetCallStatus(int floor, bool &up, bool &down);
	int GetMotorRoom();
	void EnableMalfunctions(bool value);
	void Malfunction();
	RouteController* GetRouteController();

	ElevatorCar* AddCar();
	ElevatorCar* GetCar(int number);
	void RemoveCar(ElevatorCar* car);
	int GetCarCount();
	ElevatorCar* GetCarForFloor(int number, bool report_on_failure = false);
	int GetFloorForCar(int car, int number);
	Real GetCarOffset(int number);

private:

	RouteController *route_controller; //elevator route controller

	class Timer; //internal timer class

	//parking timer object
	Timer *parking_timer;

	//arrival and departure delay timers
	Timer *arrival_delay;
	Timer *departure_delay;

	//random malfunctions timer
	Timer *malfunction_timer;
	int RandomProbability; //probability ratio of random activity, starting with 1 - higher is less frequent
	Real RandomFrequency; //speed in seconds to make each random action

	//Internal elevator simulation data
	std::vector<ElevatorCar*> Cars; //car objects
	Real ElevatorStart; //elevator vertical starting location
	Real Destination; //elevator destination Y value
	Real StoppingDistance; //distance needed to stop the elevator
	bool CalculateStoppingDistance;
	bool Brakes; //internal brake status
	int EmergencyStop; //internal stop status; 0 for off, 1 for standard stop, 2 for emergency stop
	Real JerkRate; //current jerk value, used as an acceleration/deceleration multiplier
	Real JerkPos; //temporary storage for the elevator rate at the end of the jerkrate increments
	bool FirstRun; //used for setting first-run items in the run loop
	bool Running; //is elevator in run mode?
	int GoActiveFloor; //associated floor number for GoActive flag
	bool ManualStop; //true if elevator has been manually stopped
	int HoistwayAccess; //INS mode Hoistway Access; -1 for Down, 0 for Off, 1 for Up
	int HoistwayAccessFloor; //floor used for Hoistway Access
	std::vector<int> Controllers; //dispatch controllers this elevator is assigned to
	RandomGen *rnd_time, *rnd_type;

	//functions
	void MoveElevatorToFloor();
	void FinishMove();
	void PlayStartingSounds();
	void PlayStoppingSounds(bool emergency = false);
	void PlayMovingSounds();
	void StopSounds();
	Real GetDestinationAltitude(int floor);
	Real GetDestinationOffset(int floor);
	void DirectionalIndicatorsOff();
	void NotifyArrival(bool early = false);
	void ProcessGotoFloor(int floor, int direction);
	void DoSetControls();
	void SameFloorArrival(int floor, int direction);

	//motor sound objects
	Sound *motorsound;
	Sound *motoridlesound;

	//counterweight sound
	Sound *counterweightsound;

	//mesh container for elevator doors (not shaft doors)
	DynamicMesh *DoorContainer;

	std::queue<std::string> ControlQueue;

	MeshObject* WeightMesh; //counterweight mesh object
	MeshObject* WeightRopeMesh; //counterweight rope mesh
	Vector3 weight_size; //counterweight size
	MeshObject* RopeMesh; //rope mesh

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
	Vector3 elevposition;
	Real tmpDecelJerk;
	bool FinishedMove;
	bool SoundsQueued;
};

}

#endif
