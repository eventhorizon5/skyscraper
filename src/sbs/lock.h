/* $Id$ */

/*
	Scalable Building Simulator - Lock and DoorLock Classes
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

#ifndef _SBS_LOCK_H
#define _SBS_LOCK_H

namespace SBS {

class SBSIMPEXP Lock
{
public:
	Lock(Object *parent);
	virtual ~Lock() {}
	void SetLocked(bool value, int keyid);
	bool IsLocked();
	bool ToggleLock(bool force = false);
	int GetKeyID();

private:
	bool Locked;
	int KeyID;
	Object *Parent; //parent object
};

class SBSIMPEXP DoorLock
{
public:
	DoorLock(Object *parent, int direction);
	virtual ~DoorLock() {}
	void SetLocked(int side, int keyid);
	bool GetSide(const Ogre::Vector3 &position);
	bool IsLocked(const Ogre::Vector3 &position);
	bool ToggleLock(const Ogre::Vector3 &position, bool force = false);
	int GetKeyID();

private:
	int Direction;
	int Locked;
	int KeyID;
	Object *Parent; //parent object
};

}

#endif
