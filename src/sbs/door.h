/* $Id$ */

/*
	Scalable Building Simulator - Door Object
	The Skyscraper Project - Version 1.10 Alpha
	Copyright (C)2004-2016 Ryan Thoryk
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

namespace SBS {

class SBSIMPEXP Door : public Object
{
public:

	int Direction;
	bool OpenState;
	bool IsMoving; //is door moving?
	bool OpenDoor; //open or close door?
	bool Clockwise; //if door should rotate clockwise
	float Speed; //rotation speed
	std::string OpenSound; //opening sound
	std::string CloseSound; //closing sound

	Door(Object *parent, DynamicMesh *wrapper, const std::string &name, const std::string &open_sound, const std::string &close_sound, bool open_state, const std::string &texture, float thickness, int direction, float speed, float CenterX, float CenterZ, float width, float height, float voffset, float tw, float th);
	~Door();
	bool Open(Ogre::Vector3 &position, bool playsound = true, bool force = false);
	void Close(bool playsound = true);
	bool IsOpen();
	void Enabled(bool value);
	void MoveDoor();
	void SetLocked(int side, int keyid);
	bool GetSide(const Ogre::Vector3 &position);
	bool IsLocked(const Ogre::Vector3 &position);
	bool ToggleLock(const Ogre::Vector3 &position, bool force = false);
	int GetKeyID();
	bool IsEnabled() { return is_enabled; }
	void Loop();
	void ClickDoor(Ogre::Vector3 &position);

private:
	MeshObject* DoorMesh; //door mesh

	//sound object
	Sound *sound;

	bool is_enabled;
	float rotation;
	int Locked;
	int KeyID;
};

}

#endif
