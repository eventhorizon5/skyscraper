/*
	Scalable Building Simulator - Lock and DoorLock Classes
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

DoorLock::DoorLock(Object *parent)
{
	//direction is false for left/right, true for front/back

	Direction = false;
	Locked = false;
	KeyID = 0;
	this->Parent = parent;
}

void DoorLock::SetDirection(bool direction)
{
	Direction = direction;
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

bool DoorLock::ToggleLock(const Vector3 &position, bool force)
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

bool DoorLock::GetSide(const Vector3 &position)
{
	//return which side of the door the position is (false for negative/left/front, true for positive/right/back)

	if (Direction == false && position.x > Parent->GetPosition().x)
		return true;

	if (Direction == true && position.z > Parent->GetPosition().z)
		return true;

	return false;
}

bool DoorLock::IsLocked(const Vector3 &position)
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
