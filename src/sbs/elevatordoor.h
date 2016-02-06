/* $Id$ */

/*
	Scalable Building Simulator - Elevator Door Object
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

#ifndef _SBS_ELEVATORDOOR_H
#define _SBS_ELEVATORDOOR_H

#include "timer.h"

namespace SBS {

class SBSIMPEXP ElevatorDoor : public Object
{
public:
	//door component object
	struct DoorWrapper;
	struct DoorObject
	{
		DoorObject(const std::string &doorname, DoorWrapper *Wrapper, const std::string &direction, float OpenSpeed, float CloseSpeed);
		~DoorObject();
		void MoveDoors(bool open, bool manual);
		void Move();
		void Reset(bool open);

		MeshObject* mesh;
		int direction; //direction is either 0 for up, 1 for down, 2 for left/forward and 3 for right/backward
		float open_speed;
		float close_speed;
		float active_speed;
		float openchange;
		std::string name;
		DoorWrapper *wrapper; //associated wrapper
		ElevatorDoor *parent;
		float marker1;
		float marker2;
		int door_section; //door movement section; used for both reversal tracking and debugging
		float stopping_distance;
		float temp_change;
		bool accelerating;
		bool is_open;
		bool finished;
		Ogre::Vector3 extents_min;
		Ogre::Vector3 extents_max;
		bool sign_changed;
		float old_difference;
		float offset;
		bool recheck_difference;
		bool reversed;
	};

	//wrapper that represents the entire set of doors
	struct DoorWrapper : public Object
	{
		DoorWrapper(Object *parent_obj, ElevatorDoor *door_object, bool shaftdoor, int shaftdoor_floor = 0);
		~DoorWrapper();

		DoorObject* CreateDoor(const std::string &doorname, const std::string &Direction, float OpenSpeed, float CloseSpeed);
		void Enable(bool value);
		bool CheckDoorsOpen();
		bool IsFinished();
		void MoveDoors(bool open, bool manual);
		void StopDoors();
		void ResetState();

		std::vector<DoorObject*> doors;
		std::string name;
		bool Open;
		bool Enabled;
		float Width;
		float Height;
		float Thickness;
		ElevatorDoor *parent;
		bool IsShaftDoor;
		float Shift;
		float voffset;
		int floor;
	};

	int Number; //door instance number
	Elevator *elev; //pointer to associated elevator object
	float OpenSpeed; //door opening/closing speed (for backwards-compatibility only)
	bool DoorDirection; //if direction is false, doors are on the left/right side
	int DoorTimer; //door autoclose timer value, in milliseconds
	int QuickClose; //door quick close autotimer value, in milliseconds
	float NudgeTimer; //wait time in seconds before enabling nudge mode
	std::string OpenSound; //door open sound
	std::string CloseSound; //door close sound
	std::string UpChimeSound; //elevator up chime sound
	std::string DownChimeSound; //elevator down chime sound
	std::string NudgeSound; //nudge mode sound
	std::string SensorSound; //door sensor sound
	int OpenDoor; //1=open doors, -1=close doors, 2=open manual, -2=close manual
	float ShaftDoorThickness; //thickness of shaft doors (used with AddShaftDoor command) - deprecated
	Ogre::Vector3 ShaftDoorOrigin; //shaft door origin (deprecated)
	float ManualSpeed; //manual speed multiplier
	float SlowSpeed; //slow speed multiplier, mainly for nudge mode

	ElevatorDoor(int number, Elevator* elevator);
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
	bool AreShaftDoorsClosed();
	void Reset(bool sensor = false);
	bool DoorsStopped();
	DoorWrapper* AddDoors(const std::string &lefttexture, const std::string &righttexture, float thickness, float CenterX, float CenterZ, float width, float height, bool direction, float tw, float th);
	bool AddShaftDoors(const std::string &lefttexture, const std::string &righttexture, float thickness, float CenterX, float CenterZ, float voffset, float tw, float th);
	DoorWrapper* AddShaftDoor(int floor, const std::string &lefttexture, const std::string &righttexture, float tw, float th);
	DoorWrapper* AddShaftDoor(int floor, const std::string &lefttexture, const std::string &righttexture, float thickness, float CenterX, float CenterZ, float voffset, float tw, float th);
	void Chime(int floor, bool direction);
	void Loop();
	void MoveSound(const Ogre::Vector3 &position, bool relative_x, bool relative_y, bool relative_z);
	void Enabled(bool value);
	bool IsEnabled();
	void SetShaftDoors(float thickness, float CenterX, float CenterZ);
	bool ShaftDoorsExist(int floor);
	int GetWhichDoors();
	DoorObject* AddDoorComponent(DoorWrapper *wrapper, const std::string &name, const std::string &meshname, const std::string &texture, const std::string &sidetexture, float thickness, const std::string &direction, float OpenSpeed, float CloseSpeed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th);
	DoorWrapper* AddDoorComponent(const std::string &name, const std::string &texture, const std::string &sidetexture, float thickness, const std::string &direction, float OpenSpeed, float CloseSpeed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th);
	DoorWrapper* AddShaftDoorComponent(int floor, const std::string &name, const std::string &texture, const std::string &sidetexture, float thickness, const std::string &direction, float OpenSpeed, float CloseSpeed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th);
	void AddShaftDoorsComponent(const std::string &name, const std::string &texture, const std::string &sidetexture, float thickness, const std::string &direction, float OpenSpeed, float CloseSpeed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th);
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
	float GetShaftDoorAltitude(int floor);
	void CheckSensor();
	void CreateSensor(Ogre::Vector3 &area_min, Ogre::Vector3 &area_max);
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

private:

	//elevator doors
	DoorWrapper *Doors;

	//Internal door simulation data
	int WhichDoors; //which doors are in use; 1 for both, 2 for elevator doors, 3 for shaft doors
	int ShaftDoorFloor; //floor the active shaft doors are on; only used if WhichDoors is 3
	std::vector<DoorWrapper*> ShaftDoors; //shaft doors
	std::vector<int> ManualFloors; //list of floors that use manual shaft doors

	void MoveDoors(bool open, bool manual);

	//door autoclose timer
	class Timer : public TimerObject
	{
	public:
		ElevatorDoor *door;
		Elevator *elevator;
		int type; //0 = autoclose, 1 = nudge
		Timer(const std::string &name, ElevatorDoor *parent, Elevator *elev, int Type) : TimerObject(parent, name)
		{
			door = parent;
			elevator = elev;
			type = Type;
		}
		virtual void Notify();
	};

	//autoclose timer object
	Timer *timer;

	//nudge mode timer
	Timer *nudgetimer;

	//sound objects
	Sound *doorsound;
	Sound *chime;
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
	bool sensor_enabled; //enable door sensor (active state)
	bool sensor_status; //persistent sensor on/off state

	//door sensor
	Trigger *sensor;
	Action *sensor_action;
	Action *reset_action;
};

}

#endif
