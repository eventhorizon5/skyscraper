/*
	Scalable Building Simulator - Geometry Controller
	The Skyscraper Project - Version 2.1
	Copyright (C)2004-2025 Ryan Thoryk
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
#include "texman.h"
#include "mesh.h"
#include "geometry.h"

namespace SBS {

GeometryController::GeometryController(Object *parent) : Object(parent)
{
	//set up SBS object
	SetValues("GeometryController", "Geometry Controller", true, false);
}

GeometryController::~GeometryController()
{

}

Ogre::MeshPtr GeometryController::CreatePlane(Object* parent, const std::string& name, Real size_x, Real size_y, unsigned int segments_x, unsigned int segments_y, Real utile, Real vtile)
{
	return Procedural::PlaneGenerator().setScale(1.0 / sbs->GetUnitScale()).setNumSegX(segments_x).setNumSegY(segments_y).setSizeX(size_x).setSizeY(size_y).setUTile(utile).setVTile(vtile).realizeMesh(parent->GetNameBase() + name);
}

Ogre::MeshPtr GeometryController::CreateSphere(Object* parent, const std::string& name, Real radius, Real utile, Real vtile, unsigned int rings, unsigned int segments)
{
	return Procedural::SphereGenerator().setScale(1.0 / sbs->GetUnitScale()).setRadius(radius).setUTile(utile).setVTile(vtile).setNumRings(rings).setNumSegments(segments).realizeMesh(parent->GetNameBase() + name);
}

Ogre::MeshPtr GeometryController::CreateCylinder(Object* parent, const std::string& name, Real radius, Real height, Real utile, Real vtile, unsigned int segments_base, unsigned int segments_height, bool capped)
{
	return Procedural::CylinderGenerator().setScale(1.0 / sbs->GetUnitScale()).setHeight(height).setRadius(radius).setUTile(utile).setVTile(vtile).setNumSegBase(segments_base).setNumSegHeight(segments_height).setCapped(capped).realizeMesh(parent->GetNameBase() + name);
}

Ogre::MeshPtr GeometryController::CreateTorus(Object* parent, const std::string& name, Real radius, Real section_radius, Real utile, Real vtile)
{
	return Procedural::TorusGenerator().setScale(1.0 / sbs->GetUnitScale()).setRadius(radius).setSectionRadius(section_radius).setUTile(utile).setVTile(vtile).realizeMesh(parent->GetNameBase() + name);
}

Ogre::MeshPtr GeometryController::CreateCone(Object* parent, const std::string& name, Real radius, Real height, Real utile, Real vtile, unsigned int segments_base, unsigned int segments_height)
{
	return Procedural::ConeGenerator().setScale(1.0 / sbs->GetUnitScale()).setRadius(radius).setHeight(height).setNumSegBase(segments_base).setNumSegHeight(segments_height).setUTile(utile).setVTile(vtile).realizeMesh(parent->GetNameBase() + name);
}

Ogre::MeshPtr GeometryController::CreateTube(Object* parent, const std::string& name, Real inner_radius, Real outer_radius, Real height, Real utile, Real vtile, unsigned int segments_base, unsigned int segments_height)
{
	return Procedural::TubeGenerator().setScale(1.0 / sbs->GetUnitScale()).setHeight(height).setUTile(utile).setVTile(vtile).setNumSegBase(segments_base).setNumSegHeight(segments_height).setInnerRadius(inner_radius).setOuterRadius(outer_radius).realizeMesh(parent->GetNameBase() + name);
}

Ogre::MeshPtr GeometryController::CreateBox(Object* parent, const std::string& name, Real size_x, Real size_y, Real size_z, Real utile, Real vtile, unsigned int segments_x, unsigned int segments_y, unsigned int segments_z)
{
	return Procedural::BoxGenerator().setScale(1.0 / sbs->GetUnitScale()).setSizeX(size_x).setSizeY(size_y).setSizeZ(size_z).setNumSegX(segments_x).setNumSegY(segments_y).setNumSegZ(segments_z).setUTile(utile).setVTile(vtile).realizeMesh(parent->GetNameBase() + name);
}

Ogre::MeshPtr GeometryController::CreateCapsule(Object* parent, const std::string& name, Real radius, Real height, unsigned int rings, Real utile, Real vtile, unsigned int segments, unsigned int segments_height, bool capped)
{
	return Procedural::CapsuleGenerator().setScale(1.0 / sbs->GetUnitScale()).setRadius(radius).setHeight(height).setNumRings(rings).setNumSegHeight(segments_height).setNumSegments(segments).setUTile(utile).setVTile(vtile).realizeMesh(parent->GetNameBase() + name);
}

Ogre::MeshPtr GeometryController::CreateTorusKnot(Object* parent, const std::string& name, Real radius, Real section_radius, Real utile, Real vtile, unsigned int segments_circle, unsigned int seg_section, int p, int q)
{
	return Procedural::TorusKnotGenerator().setScale(1.0 / sbs->GetUnitScale()).setRadius(radius).setSectionRadius(section_radius).setUTile(utile).setVTile(vtile).setNumSegCircle(segments_circle).setNumSegSection(seg_section).setP(p).setQ(q).realizeMesh(parent->GetNameBase() + name);
}

Ogre::MeshPtr GeometryController::CreateIcoSphere(Object* parent, const std::string& name, Real radius, Real utile, Real vtile, unsigned int iterations)
{
	return Procedural::IcoSphereGenerator().setScale(1.0 / sbs->GetUnitScale()).setRadius(radius).setNumIterations(iterations).setUTile(utile).setVTile(vtile).realizeMesh(parent->GetNameBase() + name);
}

Ogre::MeshPtr GeometryController::CreateRoundedBox(Object* parent, const std::string& name, Real size_x, Real size_y, Real size_z, Real chamfer_size, Real utile, Real vtile, unsigned int segments_x, unsigned int segments_y, unsigned int segments_z, bool capped)
{
	return Procedural::RoundedBoxGenerator().setScale(1.0 / sbs->GetUnitScale()).setSizeX(size_x).setSizeY(size_y).setSizeZ(size_z).setChamferSize(chamfer_size).setNumSegX(segments_x).setNumSegY(segments_y).setNumSegZ(segments_z).setUTile(utile).setVTile(vtile).realizeMesh(parent->GetNameBase() + name);
}

Ogre::MeshPtr GeometryController::CreateSpring(Object* parent, const std::string& name, Real radius_circle, Real radius_helix, Real height, Real round, Real utile, Real vtile, unsigned int segments_circle, unsigned int segments_path, bool capped)
{
	return Procedural::SpringGenerator().setScale(1.0 / sbs->GetUnitScale()).setHeight(height).setNumRound(round).setRadiusCircle(radius_circle).setRadiusHelix(radius_helix).setNumSegCircle(segments_circle).setNumSegPath(segments_path).setUTile(utile).setVTile(vtile).realizeMesh(parent->GetNameBase() + name);
}

Ogre::MeshPtr GeometryController::CreatePrism(Object* parent, const std::string& name, Real radius, Real height, unsigned int sides, unsigned int segments_height, bool capped)
{
	return Procedural::PrismGenerator().setScale(1.0 / sbs->GetUnitScale()).setRadius(radius).setHeight(height).setNumSides(sides).setNumSegHeight(segments_height).setCapped(capped).realizeMesh(parent->GetNameBase() + name);
}

}
