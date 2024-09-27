/*
	Scalable Building Simulator - Geometry Controller
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

#include <OgreProcedural/Procedural.h>
#include "globals.h"
#include "sbs.h"
#include "texture.h"
#include "mesh.h"
#include "geometry.h"

namespace SBS {

GeometryController::GeometryController(Object *parent) : Object(parent)
{
	Ogre::MeshPtr mesh = Procedural::SphereGenerator().setRadius(2.f).setUTile(5.).setVTile(5.).realizeMesh("sphereMesh");

	this->mesh = new MeshObject(this, "sphereMesh", 0, "", "sphereMesh");
	this->mesh->SetMaterial("Default");
}

GeometryController::~GeometryController()
{

}

}
