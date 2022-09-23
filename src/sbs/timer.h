/* $Id$ */

/*
	Scalable Building Simulator - Timer Object
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

#ifndef _SBS_TIMER_H
#define _SBS_TIMER_H

namespace SBS {

class SBSIMPEXP TimerObject : public Object
{
public:

	TimerObject(Object *parent, const std::string &name);
	virtual ~TimerObject();
	void Start(int milliseconds = -1, bool oneshot = false);
	void Stop();
	virtual void Notify() {}
	bool IsRunning();
	void Loop();
	unsigned long GetCurrentTime();
	void Report(const std::string &message);

private:
	int Interval;
	bool OneShot;
	unsigned long CurrentTime;
	unsigned long LastHit;
	unsigned long StartTime;
	bool Running;
};

}

#endif
