/*
	Scalable Building Simulator - Timer Object
	The Skyscraper Project - Version 2.1
	Copyright (C)2004-2025 Ryan Thoryk
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
#include "timer.h"

namespace SBS {

TimerObject::TimerObject(Object *parent, const std::string &name) : Object(parent)
{
	//set up SBS object
	SetValues("Timer", name, true, false);

	Interval = 0;
	OneShot = false;
	LastHit = 0;
	Running = false;
	CurrentTime = 0;
	StartTime = 0;
}

TimerObject::~TimerObject()
{
	if (Running == true)
		Stop();
}

void TimerObject::Start(int milliseconds, bool oneshot)
{
	if (Running == true)
		return;

	Interval = milliseconds;
	OneShot = oneshot;
	Running = true;
	StartTime = sbs->GetCurrentTime();
	LastHit = 0;
	CurrentTime = 0;
	sbs->RegisterTimerCallback(this);

	if (sbs->Verbose)
		Report("Start");
}

void TimerObject::Stop()
{
	if (Running == false)
		return;

	Running = false;
	sbs->UnregisterTimerCallback(this);

	if (sbs->Verbose)
		Report("Stop");
}

bool TimerObject::IsRunning()
{
	return Running;
}

bool TimerObject::Loop()
{
	if (Running == false)
		return true;

	CurrentTime = sbs->GetCurrentTime() - StartTime;

	if (CurrentTime - LastHit >= (unsigned long)Interval)
	{
		if (sbs->Verbose)
			Report("Notify");

		if (OneShot == true)
			Stop();

		Notify();
		LastHit = CurrentTime;
	}

	return true;
}

unsigned long TimerObject::GetCurrentTime()
{
	return CurrentTime;
}

void TimerObject::Report(const std::string &message)
{
	Object::Report("Timer '" + GetName() + "', parent '" + GetParent()->GetName() + "': " + message);
}

}
