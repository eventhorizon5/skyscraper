/* $Id: escalator.h 5271 2018-01-22 03:32:04Z ryan $ */

/*
	Scalable Building Simulator - Escalator Object
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

#ifndef _SBS_ESCALATOR_H
#define _SBS_ESCALATOR_H

namespace SBS {

class SBSIMPEXP Escalator : public Object
{
public:
	Real Speed;

	Escalator(Object *parent, const std::string &name, int run, Real speed, const std::string &sound_file, const std::string &riser_texture, const std::string &tread_texture, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real risersize, Real treadsize, int num_steps, Real voffset, Real tw, Real th);
	~Escalator();
	void Enabled(bool value);
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	void Loop();
	bool IsEnabled() { return is_enabled; }
	void OnClick(Ogre::Vector3 &position, bool shift, bool ctrl, bool alt, bool right);
	void ResetState();
	void SetRun(int value);
	int GetRun() { return Run; }

private:
	Sound *sound; //sound object
	int Run; //-1 is reverse, 0 is stop, 1 is forward
	bool is_enabled;
	Ogre::Vector3 start, end;
	std::string Direction;
	Real treadsize;
	Real risersize;
	int buffer_zone_steps;

	std::vector<Step*> Steps;

	void CreateSteps(const std::string &riser_texture, const std::string &tread_texture, const std::string &direction, Real width, Real risersize, Real treadsize, Real tw, Real th);
	void MoveSteps();
};

}

#endif
