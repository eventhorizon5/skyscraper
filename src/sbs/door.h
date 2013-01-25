/* $Id$ */

/*
	Scalable Building Simulator - Door Class
	The Skyscraper Project - Version 1.8 Alpha
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

#ifndef _SBS_DOOR_H
#define _SBS_DOOR_H

#include "sound.h"

class SBSIMPEXP Door
{
public:

	Object *object; //SBS object
	std::string Name; //door name
	int Direction;
	bool OpenState;
	Ogre::Vector3 origin; //door origin
	bool IsMoving; //is door moving?
	bool OpenDoor; //open or close door?
	bool Clockwise; //if door should rotate clockwise
	bool IsEnabled;
	float Speed; //rotation speed
	std::string OpenSound; //opening sound
	std::string CloseSound; //closing sound

	Door(Object *parent, const char *name, const char *open_sound, const char *close_sound, bool open_state, const char *texture, float thickness, int direction, int locked, float speed, float CenterX, float CenterZ, float width, float height, float altitude, float tw, float th);
	~Door();
	void Open(Ogre::Vector3 &position, bool playsound = true, bool force = false);
	void Close(bool playsound = true);
	bool IsOpen();
	void Enabled(bool value);
	void MoveDoor();
	void Move(const Ogre::Vector3 position, bool relative_x, bool relative_y, bool relative_z);
	Ogre::Vector3 GetPosition();
	void SetLocked(int side);
	void Lock(const Ogre::Vector3 &position);
	void Unlock(const Ogre::Vector3 &position);
	bool GetSide(const Ogre::Vector3 &position);
	bool IsLocked(const Ogre::Vector3 &position);

private:
	MeshObject* DoorMesh; //door mesh

	//sound object
	Sound *sound;

	float rotation;
	int Locked;

	char intbuffer[65];
	char buffer[20];
};

struct DoorMap
{
	int floornumber; //associated floor number
	Door *object; //door object reference
};

#endif
