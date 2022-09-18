/* $Id$ */

/*
	Scalable Building Simulator - Step Object
	The Skyscraper Project - Version 1.11 Alpha
	Copyright (C)2004-2022 Ryan Thoryk
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

#ifndef _SBS_STEP_H
#define _SBS_STEP_H

#include "mesh.h"

namespace SBS {

class SBSIMPEXP Step : public MeshObject
{
public:
	Step(Object* parent, const std::string &name, DynamicMesh* wrapper, int max_render_distance = 0);
	~Step() {}
	void Move(const Ogre::Vector3 &vector, Real speed = 1.0f);
	void OnHit();

	Ogre::Vector3 vector;
	Real speed;
	Ogre::Vector3 start;
};

}

#endif
