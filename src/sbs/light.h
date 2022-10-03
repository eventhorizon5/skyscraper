/*
	Scalable Building Simulator - Light Object
	The Skyscraper Project - Version 1.11 Alpha
	Copyright (C)2004-2022 Ryan Thoryk
	https://www.skyscrapersim.net
	https://sourceforge.net/projects/skyscraper/
	Contact - ryan@thoryk.com

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

#ifndef _SBS_LIGHT_H
#define _SBS_LIGHT_H

namespace SBS {

class SBSIMPEXP Light : public Object
{
public:

	int Type;

	//functions
	Light(Object *parent, const std::string &name, int type, Ogre::Vector3 position, Ogre::Vector3 direction, Real color_r, Real color_g, Real color_b, Real spec_color_r, Real spec_color_g, Real spec_color_b, Real spot_inner_angle = 30.f, Real spot_outer_angle = 40.f, Real spot_falloff = 1.f, Real att_range = 100000.f, Real att_constant = 1.f, Real att_linear = 0.f, Real att_quadratic = 0.f);
	~Light();
	void SetColor(Real color_r, Real color_g, Real color_b, Real spec_color_r, Real spec_color_g, Real spec_color_b);

private:

	Ogre::Light* light;
};

}

#endif
