/* $Id$ */

/*
	Scalable Building Simulator - Elevator Object
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

#ifndef _SBS_ELEVATOR_H
#define _SBS_ELEVATOR_H

#include "shaft.h"
#include "buttonpanel.h"
#include "sound.h"
#include "directional.h"
#include "elevatordoor.h"
#include "floorindicator.h"
#include "door.h"
#include "model.h"
#include "timer.h"

namespace SBS {

class SBSIMPEXP Elevator : public Object
{
	friend class ElevatorDoor;

public:
	int Number; //elevator number
	std::string Name; //elevator name
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
	int StartingFloor; //elevator starting floor
	float TempDeceleration; //temporary deceleration value, used in overrun correction
	float ErrorOffset;
	float DistanceToTravel; //distance in Y to travel
	float ElevatorRate;
	std::vector<int> ServicedFloors; //list of floors this elevator services
	int AssignedShaft; //shaft number this elevator is assigned to
	bool IsEnabled; //true if elevator is enabled
	int Direction; //-1=down, 1=up, 0=stopped
	float Height; //elevator height
	bool HeightSet;
	bool IsMoving; //is elevator moving?
	std::string CarUpStartSound; //elevator up start/speedup sound
	std::string CarDownStartSound; //elevator down start/speedup sound
	std::string CarUpMoveSound; //elevator up ove sound
	std::string CarDownMoveSound; //elevator down move sound
	std::string CarUpStopSound; //elevator up stop/slowdown sound
	std::string CarDownStopSound; //elevator down stop/slowdown sound
	std::string CarIdleSound; //elevator idle/fan sound
	std::string MotorUpStartSound; //motor up start/speedup sound
	std::string MotorUpRunSound; //motor up move sound
	std::string MotorUpStopSound; //motor up stop/slowdown sound
	std::string MotorDownStartSound; //motor down start/speedup sound
	std::string MotorDownRunSound; //motor down move sound
	std::string MotorDownStopSound; //motor down stop/slowdown sound
	std::string MotorIdleSound; //motor idle sound
	std::string AlarmSound; //alarm sound (loop)
	std::string AlarmSoundStop; //alarm stopping sound
	std::string BeepSound; //floor beep sound (played when changing floors)
	std::string FloorSound; //prefix of sounds played when reaching a certain floor; usually used for voices
	std::string UpMessageSound; //sound to play with message of elevator's direction if going up
	std::string DownMessageSound; //sound to play with message of elevator's direction if going down
	std::string OpenMessageSound; //sound to play with message of elevator's doors are opening
	std::string CloseMessageSound; //sound to play with message of elevator's doors are closing
	std::string Music; //elevator music sound to play
	std::string CarEmergencyStopSound; //car emergency stop sound
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
	bool AlarmActive; //true if alarm is active
	bool UseFloorBeeps; //true if floor beeps should be used
	bool UseFloorSounds; //true if floor sounds should be used
	bool UseDirMessageSounds; //true if direction message sounds should be used
	bool UseDoorMessageSounds; //true if door message sounds should be used
	Ogre::Vector3 MotorPosition; //position of motor sound emitter
	bool QueueResets; //true if system should use automatic queue resets for normal operation
	float CameraOffset; //camera vertical offset
	int ParkingFloor; //floor to automatically park the elevator on when idle
	float ParkingDelay; //time to wait in idle mode before parking
	bool Leveling; //is elevator in leveling mode?
	float LevelingSpeed; //leveling speed
	float LevelingOffset; //leveling offset factor
	float LevelingOpen; //leveling door open offset
	bool WaitForDoors; //set to true for the MoveElevatorToFloor() function to wait for the doors to close before running
	int ActiveDirection; //variant of Direction that doesn't change during slowdown
	bool RandomActivity; //enables/disables random call activity
	int RandomProbability; //probability ratio of random calls, starting with 1 - higher is less frequent
	float RandomFrequency; //speed in seconds to make each random call
	bool Fan; //fan enabled status
	int NotifyEarly; //perform arrival notification earlier (0 for normal, 1 for at start of leveling, 2 for at start of decel)
	bool Notified; //true if arrival notification has been performed
	bool Parking; //is elevator parking?
	Ogre::Vector3 MusicPosition; //music emitter position, relative of elevator center
	bool MusicOn; //music enabled status
	bool MusicOnMove; //true if music should only play during move
	float DepartureDelay; //delay in seconds between door closure and elevator departure
	float ArrivalDelay; //delay in seconds between elevator arrival and door opening
	bool WaitForTimer; //true if elevator is waiting for the arrival/departure timers to finish before moving
	std::vector<int> DisplayFloors; //list of floors to only display when updating floor indicators
	float InspectionSpeed; //inspection service speed multiplier
	bool LimitQueue; //true to only allow floor selections in the same queue direction
	bool AutoEnable; //true if interior objects should automatically be enabled/disabled
	bool ReOpen; //true if elevator should reopen doors if the same floor is selected
	int LastChimeDirection; //direction of last arrival chime
	bool AutoDoors; //true if doors should be automatic (automatically open when reaching destination and refuse to open if off floor)
	bool OpenOnStart; //true if doors should automatically open on simulator start
	int ManualMove; //0 if manual movement is off; -1 for down, 1 for up
	bool Interlocks; //lock doors during movement
	bool GoActive; //true if go function is in use (mouse hold is active)
	bool FloorHold; //true if floor buttons need to be held for the elevator to proceed to the floor (modern manual mode)
	bool GoPending; //true if Go() function is active
	float EmergencyStopSpeed; //emergency stop deceleration multiplier
	bool AutoAdjustSound; //auto-adjust stopping sounds
	bool SkipFloorSound; //skip playing floor sound, for use in FinishMove()
	bool ControlPressActive; //true if a control associated with this elevator has just been pressed

	MeshObject* ElevatorMesh; //elevator mesh object

	//functions
	Elevator(int number);
	~Elevator();
	bool CreateElevator(bool relative, float x, float z, int floor);
	bool AddRoute(int floor, int direction, int call_type);
	bool DeleteRoute(int floor, int direction);
	bool CallCancel();
	void Alarm();
	bool Stop(bool emergency = false);
	void OpenHatch();
	void ProcessCallQueue();
	int GetFloor();
	void Loop();
	WallObject* AddWall(const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th);
	WallObject* AddFloor(const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, bool reverse_axis, bool texture_direction, float tw, float th, bool legacy_behavior = false);
	FloorIndicator* AddFloorIndicator(const std::string &texture_prefix, const std::string &direction, float CenterX, float CenterZ, float width, float height, float voffset);
	ButtonPanel* CreateButtonPanel(const std::string &texture, int rows, int columns, const std::string &direction, float CenterX, float CenterZ, float width, float height, float voffset, float spacingX, float spacingY, float tw, float th);
	void DumpQueues();
	void Enabled(bool value);
	void EnableObjects(bool value);
	bool IsElevator(Ogre::MeshPtr test);
	bool IsInElevator(const Ogre::Vector3 &position, bool camera = false);
	float GetElevatorStart();
	float GetDestination();
	float GetStoppingDistance();
	bool GetBrakeStatus();
	int GetEmergencyStopStatus();
	void DumpServicedFloors();
	bool AddServicedFloor(int number);
	void RemoveServicedFloor(int number);
	void UpdateFloorIndicators();
	float GetJerkRate();
	float GetJerkPosition();
	void SetFloorSkipText(const std::string &id);
	std::string GetFloorSkipText();
	bool IsServicedFloor(int floor);
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
	void AddDirectionalIndicators(bool relative, bool active_direction, bool single, bool vertical, const std::string &BackTexture, const std::string &uptexture, const std::string &uptexture_lit, const std::string &downtexture, const std::string &downtexture_lit, float CenterX, float CenterZ, float voffset, const std::string &direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th);
	DirectionalIndicator* AddDirectionalIndicator(bool active_direction, bool single, bool vertical, const std::string &BackTexture, const std::string &uptexture, const std::string &uptexture_lit, const std::string &downtexture, const std::string &downtexture_lit, float CenterX, float CenterZ, float voffset, const std::string &direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th);
	void SetDirectionalIndicators(int floor, bool UpLight, bool DownLight);
	void UpdateDirectionalIndicators();
	void EnableDirectionalIndicators(bool value);
	ElevatorDoor* GetDoor(int number);
	bool OpenDoorsEmergency(int number = 0, int whichdoors = 1, int floor = 0, bool hold = false);
	void CloseDoorsEmergency(int number = 0, int whichdoors = 1, int floor = 0, bool hold = false);
	bool OpenDoors(int number = 0, int whichdoors = 1, int floor = 0, bool manual = false, bool hold = false);
	void CloseDoors(int number = 0, int whichdoors = 1, int floor = 0, bool manual = false, bool hold = false);
	void StopDoors(int number = 0);
	void HoldDoors(int number = 0, bool sensor = false);
	void ShaftDoorsEnabled(int number, int floor, bool value);
	void ShaftDoorsEnabledRange(int number, int floor, int range);
	bool AreDoorsOpen(int number = 0);
	bool AreShaftDoorsOpen(int number, int floor);
	bool AreShaftDoorsClosed();
	void ResetDoors(int number = 0, bool sensor = false);
	bool DoorsStopped(int number = 0);
	ElevatorDoor::DoorWrapper* AddDoors(int number, const std::string &lefttexture, const std::string &righttexture, float thickness, float CenterX, float CenterZ, float width, float height, bool direction, float tw, float th);
	bool AddShaftDoors(int number, const std::string &lefttexture, const std::string &righttexture, float thickness, float CenterX, float CenterZ, float voffset, float tw, float th);
	ElevatorDoor::DoorWrapper* AddShaftDoor(int floor, int number, const std::string &lefttexture, const std::string &righttexture, float tw, float th);
	ElevatorDoor::DoorWrapper* AddShaftDoor(int floor, int number, const std::string &lefttexture, const std::string &righttexture, float thickness, float CenterX, float CenterZ, float voffset, float tw, float th);
	void Chime(int number, int floor, bool direction);
	void EnableDoors(bool value);
	void SetShaftDoors(int number, float thickness, float CenterX, float CenterZ);
	bool AddFloorSigns(int door_number, bool relative, const std::string &texture_prefix, const std::string &direction, float CenterX, float CenterZ, float width, float height, float voffset);
	void SetCallButtons(int floor, bool direction, bool value);
	bool IsIdle();
	void ResetQueue(bool up, bool down);
	void SetBeepSound(const std::string &filename);
	void SetFloorSound(const std::string &prefix);
	void SetMessageSound(bool type, bool direction, const std::string &filename);
	void SetMusic(const std::string &filename);
	Sound* AddSound(const std::string &name, const std::string &filename, Ogre::Vector3 position, bool loop = true, float volume = 1.0, int speed = 100, float min_distance = 1.0, float max_distance = -1.0, float doppler_level = 0.0, float cone_inside_angle = 360, float cone_outside_angle = 360, float cone_outside_volume = 1.0, Ogre::Vector3 direction = Ogre::Vector3(0, 0, 0));
	void DeleteActiveRoute();
	bool IsQueueActive();
	bool BeyondDecelMarker(int direction, float destination);
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	ElevatorDoor::DoorWrapper* AddDoorComponent(int number, const std::string &name, const std::string &texture, const std::string &sidetexture, float thickness, const std::string &direction, float OpenSpeed, float CloseSpeed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th);
	ElevatorDoor::DoorWrapper* AddShaftDoorComponent(int number, int floor, const std::string &name, const std::string &texture, const std::string &sidetexture, float thickness, const std::string &direction, float OpenSpeed, float CloseSpeed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th);
	void AddShaftDoorsComponent(int number, const std::string &name, const std::string &texture, const std::string &sidetexture, float thickness, const std::string &direction, float OpenSpeed, float CloseSpeed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th);
	ElevatorDoor::DoorWrapper* FinishDoors(int number, bool DoorWalls = true, bool TrackWalls = true);
	ElevatorDoor::DoorWrapper* FinishShaftDoor(int number, int floor, bool DoorWalls = true, bool TrackWalls = true);
	bool FinishShaftDoors(int number, bool DoorWalls = true, bool TrackWalls = true);
	ButtonPanel* GetPanel(int index);
	int GetRandomLobby();
	void SetRandomLobby(int floor);
	void PressFloorButton(int floor);
	bool IsQueued(int floor);
	Door* AddDoor(const std::string &open_sound, const std::string &close_sound, bool open_state, const std::string &texture, float thickness, int direction, float speed, float CenterX, float CenterZ, float width, float height, float voffset, float tw, float th);
	Door* GetStdDoor(int number);
	void RemovePanel(ButtonPanel* panel);
	void RemoveDirectionalIndicator(DirectionalIndicator *indicator);
	void RemoveElevatorDoor(ElevatorDoor *door);
	void RemoveFloorIndicator(FloorIndicator *indicator);
	void RemoveDoor(Door *door);
	void RemoveSound(Sound *sound);
	void RemoveLight(Light *light);
	void RemoveModel(Model *model);
	void RemoveControl(Control *control);
	void RemoveTrigger(Trigger *trigger);
	void NotifyArrival(int floor);
	void SetRunState(bool value);
	bool IsRunning();
	bool GetArrivalDirection(int floor);
	bool PlayFloorBeep();
	bool PlayFloorSound();
	bool PlayMessageSound(bool type);
	bool DoorExists(int number);
	bool IsNudgeModeActive(int number = 0);
	void EnableNudgeMode(bool value, int number = 0);
	Light* AddLight(const std::string &name, int type, Ogre::Vector3 position, Ogre::Vector3 direction, float color_r, float color_g, float color_b, float spec_color_r, float spec_color_g, float spec_color_b, float spot_inner_angle, float spot_outer_angle, float spot_falloff, float att_range, float att_constant, float att_linear, float att_quadratic);
	Model* AddModel(const std::string &name, const std::string &filename, bool center, Ogre::Vector3 position, Ogre::Vector3 rotation, float max_render_distance = 0, float scale_multiplier = 1, bool enable_physics = false, float restitution = 0, float friction = 0, float mass = 0);
	void AddModel(Model *model);
	void AddDisplayFloor(int floor);
	Control* AddControl(const std::string &name, const std::string &sound, const std::string &direction, float CenterX, float CenterZ, float width, float height, float voffset, std::vector<std::string> &action_names, std::vector<std::string> &textures);
	Trigger* AddTrigger(const std::string &name, const std::string &sound_file, Ogre::Vector3 &area_min, Ogre::Vector3 &area_max, std::vector<std::string> &action_names);
	bool ReplaceTexture(const std::string &oldtexture, const std::string &newtexture);
	std::vector<Sound*> GetSound(const std::string &name);
	int GetFloorIndex(int floor);
	float GetDestinationAltitude(int floor);
	float GetDestinationOffset(int floor);
	void MoveObjects(float offset);
	void SetFloor(int floor);
	void OnInit();
	bool GetCallButtonStatus(int floor, bool &Up, bool &Down);
	int AvailableForCall(int floor, int direction);
	bool SelectFloor(int floor);
	bool Check(Ogre::Vector3 position);
	int GetNearestServicedFloor();
	bool ReturnToNearestFloor();
	bool IsLeveled();
	void Up();
	void Down();
	Shaft* GetShaft();
	CallButton* GetPrimaryCallButton();
	int GetActiveCallFloor();
	int GetActiveCallDirection();
	int GetActiveCallType();
	void ResetLights();
	void ChangeLight(int floor, bool value);
	int AreDoorsMoving(int number = 0, bool car_doors = true, bool shaft_doors = true);
	bool AreDoorsOpening(int number = 0, bool car_doors = true, bool shaft_doors = true);
	bool AreDoorsClosing(int number = 0, bool car_doors = true, bool shaft_doors = true);
	bool InElevator();
	void EnableSensor(bool value, int number = 0);
	bool GetSensorStatus(int number = 0);
	int GetServicedFloorCount();
	int GetServicedFloor(int index);
	void ResetShaftDoors(int floor);
	std::string GetFloorDisplay();
	bool GetHoldStatus(int number = 0);
	void ResetNudgeTimer(bool start = true, int number = 0);
	bool ShaftDoorsExist(int number, int floor);
	void ResetDoorState(int number = 0);
	bool PeakWaiting();
	bool OnRecallFloor();

private:

	//elevator parking timer
	class Timer : public TimerObject
	{
	public:
		Elevator *elevator;
		int type; //0 = parking timer, 1 = random timer, 2 = arrival, 3 = departure
		Timer(const std::string &name, Elevator *elev, int Type) : TimerObject(elev, name, false)
		{
			elevator = elev;
			type = Type;
		}
		virtual void Notify();
	};

	struct QueueEntry
	{
		int floor; //floor number
		int call_type; //0 = car call, 1 = hall call, 2 = system call

		QueueEntry(int floor, int call_type)
		{
			this->floor = floor;
			this->call_type = call_type;
		}

		bool operator < (const QueueEntry& element) const
		{
			return floor < element.floor;
		}
	};

	//parking timer object
	Timer *parking_timer;

	//random call timer object
	Timer *random_timer;

	//arrival and departure delay timers
	Timer *arrival_delay;
	Timer *departure_delay;

	//Internal elevator simulation data
	std::vector<QueueEntry> UpQueue; //up call queue
	std::vector<QueueEntry> DownQueue; //down call queue
	float ElevatorStart; //elevator vertical starting location
	int ElevatorFloor; //current elevator floor
	float Destination; //elevator destination Y value
	float StoppingDistance; //distance needed to stop the elevator
	bool CalculateStoppingDistance;
	bool Brakes; //internal brake status
	int EmergencyStop; //internal stop status; 0 for off, 1 for standard stop, 2 for emergency stop
	float JerkRate; //current jerk value, used as an acceleration/deceleration multiplier
	float JerkPos; //temporary storage for the elevator rate at the end of the jerkrate increments
	int ActiveCallFloor; //floor number of active call (that the elevator's currently responding too)
	int ActiveCallDirection; //direction of active call (that the elevator's currently responding too)
	int ActiveCallType; //type of active call (0 = car, 1 = hall, 2 = system)
	bool FirstRun; //used for setting first-run items in the run loop
	int RandomLobby; //lobby level of elevator to use for random predictions
	bool Running; //is elevator in run mode?
	bool UpQueueEmpty;
	bool DownQueueEmpty;
	bool UpCall;
	bool DownCall;
	bool QueuePending; //true if either queue has changed, and needs to be processed
	int GoActiveFloor; //associated floor number for GoActive flag

	//functions
	void MoveElevatorToFloor();
	void FinishMove();
	void PlayStartingSounds();
	void PlayStoppingSounds(bool emergency = false);
	void PlayMovingSounds();

	//sound objects
	Sound *carsound;
	Sound *idlesound;
	Sound *motorsound;
	Sound *motoridlesound;
	Sound *alarm;
	Sound *floorbeep;
	std::vector<Sound*> sounds; //generic sounds
	Sound *announcesnd;
	Sound *musicsound;

	//interior directional indicators
	std::vector<DirectionalIndicator*> DirIndicatorArray;

	//doors and shaft doors
	std::vector<ElevatorDoor*> DoorArray;

	//floor indicators
	std::vector<FloorIndicator*> FloorIndicatorArray;

	//button panel array
	std::vector<ButtonPanel*> PanelArray; //elevator button panel objects

	//standard door array
	std::vector<Door*> StdDoorArray; //pointer array to standard door objects

	//light array
	std::vector<Light*> lights;

	//Models
	std::vector<Model*> ModelArray;

	//Controls
	std::vector<Control*> ControlArray;

	//Triggers
	std::vector<Trigger*> TriggerArray;

	//elevator misc internals
	bool ElevatorIsRunning;
	int oldfloor;
	int lastfloor;
	bool lastfloorset;
	std::string FloorSkipText;
	bool RecallSet;
	bool RecallAltSet;
	bool ACPFloorSet;
	bool RecallUnavailable; //true if recall floor is unavailable (alarm has been triggered on that floor)
	ElevatorDoor* lastdoor_result;
	int lastdoor_number;
	Ogre::Vector3 elevposition;
	float tmpDecelJerk;
	bool FinishedMove;
	bool RandomLobbySet;
	bool SoundsQueued;
	bool DirMessageSound; //true if a directional message sound is queued, to prevent repeats
	bool DoorMessageSound; //true if a door message sound is queued, to prevent repeats

	//internal data for door open/close hold feature
	int doorhold_direction;
	int doorhold_whichdoors;
	int doorhold_floor;
	bool doorhold_manual;

	//cache objects for IsInElevator()
	Ogre::Vector3 lastposition;
	bool lastcheckresult;
	bool checkfirstrun;
};

}

#endif
