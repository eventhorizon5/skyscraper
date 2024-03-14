/*
	Scalable Building Simulator - Indicator Object
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

#ifndef _SBS_INDICATOR_H
#define _SBS_INDICATOR_H

namespace SBS {

class SBSIMPEXP Indicator : public Object
{
public:

	std::string Prefix; //texture name prefix
	std::string Blank; //blank texture name

	//functions
	Indicator(Object *parent, const std::string &sound, const std::string &texture_prefix, const std::string &blank_texture, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, Real timer_duration);
	~Indicator();
	void Enabled(bool value);
	void Update(const std::string &text, bool play_sound = true);
	bool IsEnabled() { return is_enabled; }
	void Off();
	bool PlaySound();

private:
	MeshObject* Mesh; //mesh object
	bool is_enabled;

	class Timer; //internal timer class

	Sound *sound; //sound object
	std::string soundfile;
	std::string active_text;

	//timer object
	Timer *timer;
	Real timer_duration;
};

}

#endif
