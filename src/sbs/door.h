/*
	Scalable Building Simulator - Door Object
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

#ifndef _SBS_DOOR_H
#define _SBS_DOOR_H

#include "lock.h"

namespace SBS {

struct DoorWrapper;

class SBSIMPEXP Door : public Object, public DoorLock
{
public:

	bool DoorDirection;
	bool OpenState;
	bool IsMoving; //is door moving?
	bool OpenDoor; //open or close door?
	std::string OpenSound; //opening sound
	std::string CloseSound; //closing sound

	Door(Object *parent, DynamicMesh *wrapper, const std::string &name, const std::string &open_sound, const std::string &close_sound, bool rotate);
	~Door();
	bool Open(Vector3 &position, bool playsound = true, bool force = false);
	void Close(bool playsound = true);
	bool IsOpen();
	void Enabled(bool value);
	bool IsEnabled() { return is_enabled; }
	void Loop();
	void ClickDoor(Vector3 &position);
	void OnClick(Vector3 &position, bool shift, bool ctrl, bool alt, bool right);
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	bool GetDoorChanged();
	bool GetPreviousOpen();
	DoorWrapper* CreateDoor(bool open_state, const std::string &texture, const std::string &side_texture, Real thickness, const std::string &face_direction, const std::string &open_direction, Real open_speed, Real close_speed, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, Real tw, Real th, Real side_tw, Real side_th);
	DoorWrapper* AddDoorComponent(const std::string &name, const std::string &texture, const std::string &sidetexture, Real thickness, const std::string &face_direction, const std::string &open_direction, bool OpenClockwise, Real OpenSpeed, Real CloseSpeed, Real x1, Real z1, Real x2, Real z2, Real height, Real voffset, Real tw, Real th, Real side_tw, Real side_th);
	DoorWrapper* FinishDoor(bool open_state);
	void AutoClose(int interval);

private:
	void MoveDoor();

	DoorWrapper *door; //door wrapper
	DynamicMesh *wrapper;

	//sound object
	Sound *sound;

	bool is_enabled;
	bool door_changed;
	bool previous_open;
	bool running;

	//timer object
	class Timer; //internal timer class
	Timer* timer;
	int timer_interval;
};

}

#endif
