/* $Id$ */

/*
	Scalable Building Simulator - Timer Object
	The Skyscraper Project - Version 1.10 Alpha
	Copyright (C)2004-2015 Ryan Thoryk
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

#include "globals.h"
#include "sbs.h"
#include "unix.h"
#include "timer.h"

namespace SBS {

TimerObject::TimerObject(Object *parent, const std::string &name, bool permanent)
{
	//set up SBS object
	SetValues(parent, "Timer", name, permanent);

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
	Interval = milliseconds;
	OneShot = oneshot;
	Running = true;
	StartTime = sbs->GetCurrentTime();
	LastHit = 0;
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

void TimerObject::Loop()
{
	if (Running == false)
		return;

	CurrentTime = sbs->GetCurrentTime() - StartTime;

	if (CurrentTime - LastHit >= (unsigned long)Interval)
	{
		if (sbs->Verbose)
			Report("Notify");

		Notify();
		LastHit = CurrentTime;

		if (OneShot == true)
			Stop();
	}
}

void TimerObject::Notify()
{

}

unsigned long TimerObject::GetCurrentTime()
{
	return CurrentTime;
}

void TimerObject::Report(const std::string &message)
{
	sbs->Report("Timer '" + GetName() + "', parent '" + GetParent()->GetName() + "': " + message);
}

}
