/* $Id$ */

/*
	Scalable Building Simulator - Door Class
	The Skyscraper Project - Version 1.8 Alpha
	Copyright (C)2004-2010 Ryan Thoryk
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
	csString Name; //door name
	int Direction;
	bool OpenState;
	csVector3 origin; //door origin
	bool IsMoving; //is door moving?
	bool OpenDoor; //open or close door?
	bool Clockwise; //if door should rotate clockwise
	bool IsEnabled;
	float Speed; //rotation speed
	csString OpenSound; //opening sound
	csString CloseSound; //closing sound

	Door(Object *parent, const char *name, const char *open_sound, const char *close_sound, bool open_state, const char *texture, float thickness, int direction, float speed, float CenterX, float CenterZ, float width, float height, float altitude, float tw, float th);
	~Door();
	void Open(bool playsound = true);
	void Close(bool playsound = true);
	bool IsOpen();
	void Enabled(bool value);
	void MoveDoor();
	void Move(const csVector3 position, bool relative_x, bool relative_y, bool relative_z);
	csVector3 GetPosition();

private:
	MeshObject* DoorMesh; //door mesh

	//sound object
	Sound *sound;

	float rotation;

	char intbuffer[65];
	char buffer[20];
};

#endif
