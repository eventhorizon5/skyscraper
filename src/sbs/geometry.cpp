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
	Ogre::MeshPtr mesh = CreateSphere(2.0, 5.0, 5.0, 16, 16, "sphereMesh");
	this->mesh = new MeshObject(this, "sphereMesh", 0, "", "sphereMesh");
	this->mesh->SetMaterial("Default");
}

GeometryController::~GeometryController()
{
	if (mesh)
		delete mesh;
	mesh = 0;
}

Ogre::MeshPtr GeometryController::CreatePlane(unsigned int segments_x, unsigned int segments_y, Real size_x, Real size_y, Real utile, Real vtile, const std::string& name)
{
	return Procedural::PlaneGenerator().setNumSegX(segments_x).setNumSegY(segments_y).setSizeX(size_x).setSizeY(size_y).setUTile(utile).setVTile(vtile).realizeMesh(name);
}

Ogre::MeshPtr GeometryController::CreateSphere(Real radius, Real utile, Real vtile, unsigned int rings, unsigned int segments, const std::string& name)
{
	return Procedural::SphereGenerator().setRadius(radius).setUTile(utile).setVTile(vtile).setNumRings(rings).setNumSegments(segments).realizeMesh(name);
}

Ogre::MeshPtr GeometryController::CreateCylinder(Real radius, Real height, Real utile, Real vtile, unsigned int segments_base, unsigned int segments_height, bool capped, const std::string& name)
{
	return Procedural::CylinderGenerator().setHeight(height).setRadius(radius).setUTile(utile).setVTile(vtile).setNumSegBase(segments_base).setNumSegHeight(segments_height).setCapped(capped).realizeMesh(name);
}

Ogre::MeshPtr GeometryController::CreateTorus(Real radius, Real section_radius, Real utile, Real vtile, const std::string& name)
{
	return Procedural::TorusGenerator().setRadius(radius).setSectionRadius(section_radius).setUTile(utile).setVTile(vtile).realizeMesh(name);
}

Ogre::MeshPtr GeometryController::CreateCone(Real radius, Real height, Real utile, Real vtile, unsigned int segments_base, unsigned int segments_height, const std::string& name)
{
	return Procedural::ConeGenerator().setRadius(radius).setHeight(height).setNumSegBase(segments_base).setNumSegHeight(segments_height).setUTile(utile).setVTile(vtile).realizeMesh(name);
}

Ogre::MeshPtr GeometryController::CreateTube(Real inner_radius, Real outer_radius, Real height, Real utile, Real vtile, unsigned int segments_base, unsigned int segments_height, const std::string& name)
{
	return Procedural::TubeGenerator().setHeight(height).setUTile(utile).setVTile(vtile).setNumSegBase(segments_base).setNumSegHeight(segments_height).setInnerRadius(inner_radius).setOuterRadius(outer_radius).realizeMesh(name);
}

Ogre::MeshPtr GeometryController::CreateBox(Real size_x, Real size_y, Real size_z, Real utile, Real vtile, unsigned int segments_x, unsigned int segments_y, unsigned int segments_z, const std::string& name)
{
	return Procedural::BoxGenerator().setSizeX(size_x).setSizeY(size_y).setSizeZ(size_z).setNumSegX(segments_x).setNumSegY(segments_y).setNumSegZ(segments_z).setUTile(utile).setVTile(vtile).realizeMesh(name);
}

Ogre::MeshPtr GeometryController::CreateCapsule(Real radius, Real height, unsigned int rings, Real utile, Real vtile, unsigned int segments, unsigned int segments_height, bool capped, const std::string& name)
{
	return Procedural::CapsuleGenerator().setRadius(radius).setHeight(height).setNumRings(rings).setNumSegHeight(segments_height).setNumSegments(segments).setUTile(utile).setVTile(vtile).realizeMesh(name);
}

Ogre::MeshPtr GeometryController::CreateTorusKnot(Real radius, Real section_radius, Real utile, Real vtile, unsigned int segments_circle, unsigned int seg_section, int p, int q, const std::string& name)
{
	return Procedural::TorusKnotGenerator().setRadius(radius).setSectionRadius(section_radius).setUTile(utile).setVTile(vtile).setNumSegCircle(segments_circle).setNumSegSection(seg_section).setP(p).setQ(q).realizeMesh(name);
}

Ogre::MeshPtr GeometryController::CreateIcoSphere(Real radius, Real utile, Real vtile, unsigned int iterations, const std::string& name)
{
	return Procedural::IcoSphereGenerator().setRadius(radius).setNumIterations(iterations).setUTile(utile).setVTile(vtile).realizeMesh("icoSphereMesh");
}

Ogre::MeshPtr GeometryController::CreateRoundedBox(Real size_x, Real size_y, Real size_z, Real chamfer_size, Real utile, Real vtile, unsigned int segments_x, unsigned int segments_y, unsigned int segments_z, bool capped, const std::string& name)
{
	return Procedural::RoundedBoxGenerator().setSizeX(size_x).setSizeY(size_y).setSizeZ(size_z).setChamferSize(chamfer_size).setNumSegX(segments_x).setNumSegY(segments_y).setNumSegZ(segments_z).setUTile(utile).setVTile(vtile).realizeMesh(name);
}

Ogre::MeshPtr GeometryController::CreateSpring(Real radius_circle, Real radius_helix, Real height, Real round, Real utile, Real vtile, unsigned int segments_circle, unsigned int segments_path, bool capped, const std::string& name)
{
	return Procedural::SpringGenerator().setHeight(height).setNumRound(round).setRadiusCircle(radius_circle).setRadiusHelix(radius_helix).setNumSegCircle(segments_circle).setNumSegPath(segments_path).setUTile(utile).setVTile(vtile).realizeMesh(name);
}


}
