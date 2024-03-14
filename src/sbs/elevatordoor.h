/*
	Scalable Building Simulator - Elevator Door Object
	The Skyscraper Project - Version 1.12 Alpha
	Copyright (C)2004-2024 Ryan Thoryk
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

#ifndef _SBS_ELEVATORDOOR_H
#define _SBS_ELEVATORDOOR_H

namespace SBS {

struct DoorWrapper;
struct DoorComponent;

class SBSIMPEXP ElevatorDoor : public Object
{
public:
	int Number; //door instance number
	Elevator *elev; //pointer to associated elevator object
	ElevatorCar *car; //pointer to associated elevator car object
	Real OpenSpeed; //door opening/closing speed (for backwards-compatibility only)
	bool DoorDirection; //if direction is false, doors are on the left/right side
	int DoorTimer; //door autoclose timer value, in milliseconds
	int QuickClose; //door quick close autotimer value, in milliseconds
	Real NudgeTimer; //wait time in seconds before enabling nudge mode
	std::string OpenSound; //door open sound
	std::string CloseSound; //door close sound
	std::string UpChimeSound; //elevator up chime sound
	std::string DownChimeSound; //elevator down chime sound
	std::string EarlyUpChimeSound; //elevator early up chime sound
	std::string EarlyDownChimeSound; //elevator early down chime sound
	std::string NudgeSound; //nudge mode sound
	std::string SensorSound; //door sensor sound
	int OpenDoor; //1=open doors, -1=close doors, 2=open manual, -2=close manual
	Real ShaftDoorThickness; //thickness of shaft doors (used with AddShaftDoor command) - deprecated
	Vector3 ShaftDoorOrigin; //shaft door origin (deprecated)
	Real ManualSpeed; //manual speed multiplier
	Real SlowSpeed; //slow speed multiplier, mainly for nudge mode
	bool EarlyUpSet;
	bool EarlyDownSet;

	ElevatorDoor(int number, ElevatorCar* car);
	~ElevatorDoor();
	void OpenDoorsEmergency(int whichdoors = 1, int floor = 0);
	void CloseDoorsEmergency(int whichdoors = 1, int floor = 0);
	void OpenDoors(int whichdoors = 1, int floor = 0, bool manual = false);
	void CloseDoors(int whichdoors = 1, int floor = 0, bool manual = false);
	void StopDoors();
	void Hold(bool sensor = false);
	void ShaftDoorsEnabled(int floor, bool value);
	void ShaftDoorsEnabledRange(int floor, int range);
	bool AreDoorsOpen();
	bool AreShaftDoorsOpen(int floor);
	bool AreShaftDoorsClosed(bool skip_current_floor = false);
	void Reset(bool sensor = false);
	bool DoorsStopped();
	DoorWrapper* AddDoors(const std::string &lefttexture, const std::string &righttexture, Real thickness, Real CenterX, Real CenterZ, Real width, Real height, bool direction, Real tw, Real th);
	bool AddShaftDoors(const std::string &lefttexture, const std::string &righttexture, Real thickness, Real CenterX, Real CenterZ, Real voffset, Real tw, Real th);
	DoorWrapper* AddShaftDoor(int floor, const std::string &lefttexture, const std::string &righttexture, Real tw, Real th);
	DoorWrapper* AddShaftDoor(int floor, const std::string &lefttexture, const std::string &righttexture, Real thickness, Real CenterX, Real CenterZ, Real voffset, Real tw, Real th);
	void Chime(int floor, bool direction);
	void EarlyChime(int floor, bool direction);
	void Loop();
	void MoveSound(const Vector3 &position, bool relative_x, bool relative_y, bool relative_z);
	void Enabled(bool value);
	bool IsEnabled();
	void SetShaftDoors(Real thickness, Real CenterX, Real CenterZ);
	bool ShaftDoorsExist(int floor);
	int GetWhichDoors();
	DoorComponent* AddDoorComponent(DoorWrapper *wrapper, const std::string &name, const std::string &meshname, const std::string &texture, const std::string &sidetexture, Real thickness, const std::string &direction, Real OpenSpeed, Real CloseSpeed, Real x1, Real z1, Real x2, Real z2, Real height, Real voffset, Real tw, Real th, Real side_tw, Real side_th);
	DoorWrapper* AddDoorComponent(const std::string &name, const std::string &texture, const std::string &sidetexture, Real thickness, const std::string &direction, Real OpenSpeed, Real CloseSpeed, Real x1, Real z1, Real x2, Real z2, Real height, Real voffset, Real tw, Real th, Real side_tw, Real side_th);
	DoorWrapper* AddShaftDoorComponent(int floor, const std::string &name, const std::string &texture, const std::string &sidetexture, Real thickness, const std::string &direction, Real OpenSpeed, Real CloseSpeed, Real x1, Real z1, Real x2, Real z2, Real height, Real voffset, Real tw, Real th, Real side_tw, Real side_th);
	void AddShaftDoorsComponent(const std::string &name, const std::string &texture, const std::string &sidetexture, Real thickness, const std::string &direction, Real OpenSpeed, Real CloseSpeed, Real x1, Real z1, Real x2, Real z2, Real height, Real voffset, Real tw, Real th, Real side_tw, Real side_th);
	DoorWrapper* FinishDoors(DoorWrapper *wrapper, int floor, bool ShaftDoor, bool DoorWalls = true, bool TrackWalls = true);
	DoorWrapper* FinishDoors(bool DoorWalls = true, bool TrackWalls = true);
	DoorWrapper* FinishShaftDoor(int floor, bool DoorWalls = true, bool TrackWalls = true);
	bool FinishShaftDoors(bool DoorWalls = true, bool TrackWalls = true);
	DoorWrapper* GetDoorWrapper();
	DoorWrapper* GetShaftDoorWrapper(int floor);
	bool TimerIsRunning();
	void EnableNudgeMode(bool value);
	bool GetNudgeStatus();
	int GetManualIndex(int floor);
	Real GetShaftDoorAltitude(int floor);
	void CheckSensor();
	void CreateSensor(Vector3 &area_min, Vector3 &area_max);
	bool AreDoorsMoving(int doors = 0, bool car_doors = true, bool shaft_doors = true);
	void EnableSensor(bool value, bool persistent = true);
	bool GetSensorStatus(bool persistent = true);
	bool IsSensorBlocked();
	bool GetHoldStatus();
	void ResetNudgeTimer(bool start = true);
	bool AllowNudgeMode();
	std::string GetNumberText();
	void ResetState();
	void RemoveShaftDoor(DoorWrapper *door);
	void AddServicedFloor(int floor);
	void RemoveServicedFloor(int floor);
	bool GetDoorChanged();
	bool GetPreviousOpen();

private:

	//elevator doors
	DoorWrapper *Doors;

	//Internal door simulation data
	int WhichDoors; //which doors are in use; 1 for both, 2 for elevator doors, 3 for shaft doors
	int ShaftDoorFloor; //floor the active shaft doors are on; only used if WhichDoors is 3
	std::vector<DoorWrapper*> ShaftDoors; //shaft doors
	std::vector<int> ManualFloors; //list of floors that use manual shaft doors

	void MoveDoors(bool open, bool manual);

	class Timer; //internal timer class

	//autoclose timer object
	Timer *timer;

	//nudge mode timer
	Timer *nudgetimer;

	//sound objects
	Sound *doorsound;
	Sound *chime;
	Sound *earlychime;
	Sound *nudgesound;

	//door internals
	bool DoorIsRunning;
	int index;
	bool previous_open;
	bool door_changed;
	bool quick_close; //used if user presses close button while doors are opening; results in a faster timer length
	bool doors_stopped;
	bool nudge_enabled;
	bool nudgesound_loaded;
	int chimesound_loaded;
	int earlychimesound_loaded;
	bool sensor_enabled; //enable door sensor (active state)
	bool sensor_status; //persistent sensor on/off state

	//door sensor
	Trigger *sensor;
	Action *sensor_action;
	Action *reset_action;
};

}

#endif
