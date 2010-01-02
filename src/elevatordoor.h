/* $Id$ */

/*
	Scalable Building Simulator - Elevator Door Class
	The Skyscraper Project - Version 1.6 Alpha
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

#ifndef _SBS_ELEVATORDOOR_H
#define _SBS_ELEVATORDOOR_H

#include <wx/timer.h>
#include "sound.h"

class Elevator;

class SBSIMPEXP ElevatorDoor
{
public:
	Object *object; //SBS object
	csArray<Object*> shaftdoor_objects; //shaft door objects
	int Number; //door instance number
	Elevator *elev; //pointer to associated elevator object
	float OpenSpeed; //door opening/closing speed
	bool DoorDirection; //if direction is false, doors are on the left/right side
	float DoorWidth; //elevator door width
	float DoorHeight; //elevator door height
	csVector3 DoorOrigin; //elevator door origin
	csVector3 ShaftDoorOrigin; //shaft door origin
	int DoorTimer; //door autoclose timer value, in milliseconds
	csString OpenSound; //door open sound
	csString CloseSound; //door close sound
	csString UpChimeSound; //elevator up chime sound
	csString DownChimeSound; //elevator down chime sound
	int OpenDoor; //1=open doors, -1=close doors
	bool IsEnabled; //are doors enabled?
	float ShaftDoorThickness; //thickness of shaft doors (used with AddShaftDoor command)

	ElevatorDoor(int number, Elevator* elevator);
	~ElevatorDoor();
	void OpenDoorsEmergency(int whichdoors = 1, int floor = 0);
	void CloseDoorsEmergency(int whichdoors = 1, int floor = 0);
	void OpenDoors(int whichdoors = 1, int floor = 0, bool manual = false);
	void CloseDoors(int whichdoors = 1, int floor = 0, bool manual = false);
	void StopDoors();
	void ShaftDoorsEnabled(int floor, bool value);
	void ShaftDoorsEnabledRange(int floor, int range);
	bool AreDoorsOpen();
	bool AreShaftDoorsOpen(int floor);
	float GetCurrentDoorSpeed();
	void ResetDoorTimer();
	bool DoorsStopped();
	int AddDoors(const char *lefttexture, const char *righttexture, float thickness, float CenterX, float CenterZ, float width, float height, bool direction, float tw, float th);
	bool AddShaftDoors(const char *lefttexture, const char *righttexture, float thickness, float CenterX, float CenterZ, float tw, float th);
	bool AddShaftDoor(int floor, const char *lefttexture, const char *righttexture, float tw, float th);
	void Chime(int floor, bool direction);
	void Loop();
	void Move(const csVector3 position, bool relative_x, bool relative_y, bool relative_z);
	void MoveSound(const csVector3 position, bool relative_x, bool relative_y, bool relative_z);
	void Enabled(bool value);
	bool GetDoorsOpen();
	void SetShaftDoors(float thickness, float CenterX, float CenterZ);
	bool ShaftDoorsExist(int floor);
	int GetWhichDoors();

private:
	csRef<iMeshWrapper> ElevatorDoorL; //left inside door
		csRef<iThingFactoryState> ElevatorDoorL_state;
		csRef<iMovable> ElevatorDoorL_movable;
	csRef<iMeshWrapper> ElevatorDoorR; //right inside door
		csRef<iThingFactoryState> ElevatorDoorR_state;
		csRef<iMovable> ElevatorDoorR_movable;

	//Internal door simulation data
	bool DoorsOpen; //elevator door state
	csArray<bool> ShaftDoorsOpen; //shaft door state
	csArray<bool> ShaftDoorsState; //shaft door enabled state
	float ElevatorDoorSpeed;
	int WhichDoors;
	int ShaftDoorFloor;
	csRefArray<iMeshWrapper> ShaftDoorL; //shaft door array
	csRefArray<iThingFactoryState> ShaftDoorL_state; //shaft door array state
	csRefArray<iMeshWrapper> ShaftDoorR; //shaft door array
	csRefArray<iThingFactoryState> ShaftDoorR_state; //shaft door array state

	void MoveDoors(bool open, bool manual);

	char intbuffer[65];
	char buffer[20];

	//door autoclose timer
	class Timer : public wxTimer
	{
	public:
		ElevatorDoor *door;
		Elevator *elevator;
		Timer(ElevatorDoor *parent, Elevator *elev)
		{
			door = parent;
			elevator = elev;
		};
		virtual void Notify();
	};

	//timer object
	Timer *timer;

	//sound objects
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
};

#endif
