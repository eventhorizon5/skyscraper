/*
	Scalable Building Simulator - Step Object
	The Skyscraper Project - Version 2.1
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
#include "camera.h"
#include "step.h"

namespace SBS {

Step::Step(Object* parent, const std::string &name, DynamicMesh* wrapper, int max_render_distance) : MeshObject(parent, name, wrapper, "", "", max_render_distance)
{
	vector = Vector3::ZERO;
	speed = 0;
	start = Vector3::ZERO;
}

void Step::Move(const Vector3 &vector, Real speed)
{
	//move step according to vector and speed

	MeshObject::Move(vector, speed);
	this->vector = vector;
	this->speed = speed;
}

void Step::OnHit()
{
	//move camera along with step, when user is on it

	sbs->camera->MovePosition(vector * 1.675, speed);
}

}
