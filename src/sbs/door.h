/* $Id: door.h 5263 2018-01-12 00:59:01Z ryan $ */

/*
	Scalable Building Simulator - Door Object
	The Skyscraper Project - Version 1.11 Alpha
	Copyright (C)2004-2018 Ryan Thoryk
	http://www.skyscrapersim.com
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@skyscrapersim.com

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

class SBSIMPEXP Door : public Object, public DoorLock
{
public:

	int Direction;
	bool OpenState;
	bool IsMoving; //is door moving?
	bool OpenDoor; //open or close door?
	bool Clockwise; //if door should rotate clockwise
	Real Speed; //rotation speed
	std::string OpenSound; //opening sound
	std::string CloseSound; //closing sound

	Door(Object *parent, DynamicMesh *wrapper, const std::string &name, const std::string &open_sound, const std::string &close_sound, bool open_state, const std::string &texture, Real thickness, int direction, Real speed, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, Real tw, Real th);
	~Door();
	bool Open(Ogre::Vector3 &position, bool playsound = true, bool force = false);
	void Close(bool playsound = true);
	bool IsOpen();
	void Enabled(bool value);
	void MoveDoor();
	bool IsEnabled() { return is_enabled; }
	void Loop();
	void ClickDoor(Ogre::Vector3 &position);
	void OnClick(Ogre::Vector3 &position, bool shift, bool ctrl, bool alt, bool right);
	void Report(const std::string &message);
	bool ReportError(const std::string &message);

private:
	MeshObject* DoorMesh; //door mesh

	//sound object
	Sound *sound;

	bool is_enabled;
	Real rotation;
};

}

#endif
