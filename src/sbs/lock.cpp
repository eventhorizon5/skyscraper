/* $Id: lock.cpp 5263 2018-01-12 00:59:01Z ryan $ */

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

#include "globals.h"
#include "sbs.h"
#include "lock.h"

namespace SBS {

Lock::Lock(Object *parent)
{
	Locked = false;
	KeyID = 0;
	this->Parent = parent;
}

void Lock::SetLocked(bool value, int keyid)
{
	//set locked state
	Locked = value;
	KeyID = keyid;
}

bool Lock::ToggleLock(bool force)
{
	//toggle lock state
	//if force is true, bypass key check

	//quit if user doesn't have key, if force is false
	if (KeyID != 0)
	{
		if (Parent->GetRoot()->CheckKey(KeyID) == false && force == false)
			return Parent->ReportError("Need key " + ToString(KeyID) + " to lock/unlock");
	}

	Locked = !Locked;

	if (Locked == true)
		Parent->Report("Locked");
	else
		Parent->Report("Unlocked");

	return true;
}

bool Lock::IsLocked()
{
	return Locked;
}

int Lock::GetKeyID()
{
	return KeyID;
}

DoorLock::DoorLock(Object *parent, int direction)
{
	Direction = direction;
	Locked = false;
	KeyID = 0;
	this->Parent = parent;
}

void DoorLock::SetLocked(int side, int keyid)
{
	//lock table:
	//0 = unlocked
	//1 = negative (left/front) side locked
	//2 = positive (right/back) side locked
	//3 = both sides locked

	if (side < 0 || side > 3)
		return;

	Locked = side;
	KeyID = keyid;
}

bool DoorLock::ToggleLock(const Ogre::Vector3 &position, bool force)
{
	//toggle lock state of the related door side
	//if force is true, bypass key check

	bool replocked = false;

	//quit if user doesn't have key, if force is false
	if (KeyID != 0)
	{
		if (Parent->GetRoot()->CheckKey(KeyID) == false && force == false)
			return Parent->ReportError("Need key " + ToString(KeyID) + " to lock/unlock");
	}

	if (GetSide(position) == false)
	{
		if (Locked == 0)
		{
			Locked = 1;
			replocked = true;
		}
		else if (Locked == 1)
		{
			Locked = 0;
			replocked = false;
		}
		else if (Locked == 2)
		{
			Locked = 3;
			replocked = true;
		}
	}
	else
	{
		if (Locked == 0)
		{
			Locked = 2;
			replocked = true;
		}
		else if (Locked == 1)
		{
			Locked = 3;
			replocked = true;
		}
		else if (Locked == 2)
		{
			Locked = 1;
			replocked = false;
		}
	}

	if (replocked == true)
		Parent->Report("Locked");
	else
		Parent->Report("Unlocked");

	return true;
}

bool DoorLock::GetSide(const Ogre::Vector3 &position)
{
	//return which side of the door the position is (false for negative/left/front, true for positive/right/back)

	if ((Direction >= 1 && Direction <= 4) && position.x > Parent->GetPosition().x)
		return true;

	if ((Direction >= 5 && Direction <= 8) && position.z > Parent->GetPosition().z)
		return true;

	return false;
}

bool DoorLock::IsLocked(const Ogre::Vector3 &position)
{
	//returns if the door's side (in relation to the given position) is locked or not

	if (Locked == 0)
		return false;

	if (Locked == 3)
		return true;

	bool side = GetSide(position);

	if ((Locked == 1 && side == false) || (Locked == 2 && side == true))
		return true;
	return false;
}

int DoorLock::GetKeyID()
{
	return KeyID;
}

}
