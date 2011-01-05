/* $Id$ */

/*
	Scalable Building Simulator - Light Class
	The Skyscraper Project - Version 1.8 Alpha
	Copyright (C)2004-2011 Ryan Thoryk
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

#ifndef _SBS_LIGHT_H
#define _SBS_LIGHT_H

class SBSIMPEXP Light
{
public:

	Object *object; //SBS object
	std::string Name;
	int Type;
	Ogre::Vector3 Origin;

	//functions
	Light(const char *name, int type, Ogre::Vector3 position, Ogre::Vector3 rotation, float radius, float max_distance, float color_r, float color_g, float color_b, float spec_color_r, float spec_color_g, float spec_color_b, float directional_cutoff_radius, float spot_falloff_inner, float spot_falloff_outer);
	~Light();
	void Prepare();
	void Move(const Ogre::Vector3 position, bool relative_x, bool relative_y, bool relative_z);
	Ogre::Vector3 GetPosition();
	void SetColor(float color_r, float color_g, float color_b, float spec_color_r, float spec_color_g, float spec_color_b);
	void SetRotation(const Ogre::Vector3& rotation);
	void Rotate(const Ogre::Vector3& rotation, float speed);
	Ogre::Vector3 GetRotation();

private:

	Ogre::Light* light;
	float rotX, rotY, rotZ;
};

#endif
