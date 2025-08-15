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

namespace SBS {

class MeshObject;

class SBSIMPEXP GeometryController : public Object
{
public:
	explicit GeometryController(Object *parent);
	~GeometryController();
	Ogre::MeshPtr CreatePlane(Object* parent, const std::string& name, Real size_x, Real size_y, unsigned int segments_x, unsigned int segments_y, Real utile, Real vtile);
	Ogre::MeshPtr CreateSphere(Object* parent, const std::string& name, Real radius, Real utile, Real vtile, unsigned int rings, unsigned int segments);
	Ogre::MeshPtr CreateCylinder(Object* parent, const std::string& name, Real radius, Real height, Real utile, Real vtile, unsigned int segments_base, unsigned int segments_height, bool capped);
	Ogre::MeshPtr CreateTorus(Object* parent, const std::string& name, Real radius, Real section_radius, Real utile, Real vtile);
	Ogre::MeshPtr CreateCone(Object* parent, const std::string& name, Real radius, Real height, Real utile, Real vtile, unsigned int segments_base, unsigned int segments_height);
	Ogre::MeshPtr CreateTube(Object* parent, const std::string& name, Real inner_radius, Real outer_radius, Real height, Real utile, Real vtile, unsigned int segments_base, unsigned int segments_height);
	Ogre::MeshPtr CreateBox(Object* parent, const std::string& name, Real size_x, Real size_y, Real size_z, Real utile, Real vtile, unsigned int segments_x, unsigned int segments_y, unsigned int segments_z);
	Ogre::MeshPtr CreateCapsule(Object* parent, const std::string& name, Real radius, Real height, unsigned int rings, Real utile, Real vtile, unsigned int segments, unsigned int segments_height, bool capped);
	Ogre::MeshPtr CreateTorusKnot(Object* parent, const std::string& name, Real radius, Real section_radius, Real utile, Real vtile, unsigned int segments_circle, unsigned int seg_section, int p, int q);
	Ogre::MeshPtr CreateIcoSphere(Object* parent, const std::string& name, Real radius, Real utile, Real vtile, unsigned int iterations);
	Ogre::MeshPtr CreateRoundedBox(Object* parent, const std::string& name, Real size_x, Real size_y, Real size_z, Real chamfer_size, Real utile, Real vtile, unsigned int segments_x, unsigned int segments_y, unsigned int segments_z, bool capped);
	Ogre::MeshPtr CreateSpring(Object* parent, const std::string& name, Real radius_circle, Real radius_helix, Real height, Real round, Real utile, Real vtile, unsigned int segments_circle, unsigned int segments_path, bool capped);
	Ogre::MeshPtr CreatePrism(Object* parent, const std::string& name, Real radius, Real height, unsigned int sides, unsigned int segments_height, bool capped);

private:

};

}
