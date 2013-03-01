/* $Id$ */

/*
	Scalable Building Simulator - Elevator Door Class
	The Skyscraper Project - Version 1.9 Alpha
	Copyright (C)2004-2013 Ryan Thoryk
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

#include <wx/timer.h>
#include "sound.h"

class Elevator;

class SBSIMPEXP ElevatorDoor
{
public:
	//door component object
	struct DoorWrapper;
	struct DoorObject
	{
		DoorObject(const char *doorname, DoorWrapper *Wrapper, const char *direction, float OpenSpeed, float CloseSpeed);
		~DoorObject();
		void MoveDoors(bool open, bool manual);
		void Move();

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
	};

	//wrapper that represents the entire set of doors
	struct DoorWrapper
	{
		DoorWrapper(ElevatorDoor *parentobject, bool shaftdoor);
		~DoorWrapper();

		DoorObject* CreateDoor(const char *doorname, const char *Direction, float OpenSpeed, float CloseSpeed);
		void Enable(bool value);
		bool CheckDoorsOpen();
		bool IsFinished();
		void MoveDoors(bool open, bool manual);
		void StopDoors();

		Object *object;
		std::vector<DoorObject*> doors;
		std::string name;
		bool Open;
		bool Enabled;
		Ogre::Vector3 Origin;
		float Width;
		float Height;
		float Thickness;
		ElevatorDoor *parent;
		bool IsShaftDoor;
		float Shift;
		float altitude;
	};

	Object *object; //SBS object
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
	int OpenDoor; //1=open doors, -1=close doors
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
	void Hold();
	void ShaftDoorsEnabled(int floor, bool value);
	void ShaftDoorsEnabledRange(int floor, int range);
	bool AreDoorsOpen();
	bool AreShaftDoorsOpen(int floor);
	void ResetDoorTimer();
	bool DoorsStopped();
	Object* AddDoors(const char *lefttexture, const char *righttexture, float thickness, float CenterX, float CenterZ, float width, float height, bool direction, float tw, float th);
	bool AddShaftDoors(const char *lefttexture, const char *righttexture, float thickness, float CenterX, float CenterZ, float voffset, float tw, float th);
	Object* AddShaftDoor(int floor, const char *lefttexture, const char *righttexture, float tw, float th);
	Object* AddShaftDoor(int floor, const char *lefttexture, const char *righttexture, float thickness, float CenterX, float CenterZ, float voffset, float tw, float th);
	void Chime(int floor, bool direction);
	void Loop();
	void Move(const Ogre::Vector3 &position, bool relative_x, bool relative_y, bool relative_z);
	void MoveSound(const Ogre::Vector3 &position, bool relative_x, bool relative_y, bool relative_z);
	void Enabled(bool value);
	bool IsEnabled();
	bool GetDoorsOpen();
	void SetShaftDoors(float thickness, float CenterX, float CenterZ);
	bool ShaftDoorsExist(int floor);
	int GetWhichDoors();
	void AddDoorComponent(DoorWrapper *wrapper, const char *name, const char *meshname, const char *texture, const char *sidetexture, float thickness, const char *direction, float OpenSpeed, float CloseSpeed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th);
	Object* AddDoorComponent(const char *name, const char *texture, const char *sidetexture, float thickness, const char *direction, float OpenSpeed, float CloseSpeed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th);
	Object* AddShaftDoorComponent(int floor, const char *name, const char *texture, const char *sidetexture, float thickness, const char *direction, float OpenSpeed, float CloseSpeed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th);
	void AddShaftDoorsComponent(const char *name, const char *texture, const char *sidetexture, float thickness, const char *direction, float OpenSpeed, float CloseSpeed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th);
	Object* FinishDoors(DoorWrapper *wrapper, int floor, bool ShaftDoor);
	Object* FinishDoors();
	Object* FinishShaftDoor(int floor);
	bool FinishShaftDoors();
	DoorWrapper* GetDoorWrapper();
	DoorWrapper* GetShaftDoorWrapper(int floor);
	bool TimerIsRunning();
	void EnableNudgeMode(bool value);
	bool GetNudgeStatus();
	int GetManualIndex(int floor);
	float GetShaftDoorAltitude(int floor);

private:

	//elevator doors
	DoorWrapper *Doors;

	//Internal door simulation data
	int WhichDoors;
	int ShaftDoorFloor;
	std::vector<DoorWrapper*> ShaftDoors; //shaft doors
	std::vector<int> ManualFloors; //list of floors that use manual shaft doors

	void MoveDoors(bool open, bool manual);

	char intbuffer[65];
	char buffer[20];

	//door autoclose timer
	class Timer : public wxTimer
	{
	public:
		ElevatorDoor *door;
		Elevator *elevator;
		int type; //0 = autoclose, 1 = nudge
		Timer(ElevatorDoor *parent, Elevator *elev, int Type)
		{
			door = parent;
			elevator = elev;
			type = Type;
		};
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
};

#endif
