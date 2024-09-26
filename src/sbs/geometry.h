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

namespace SBS {

class MeshObject;

class SBSIMPEXP GeometryController : public Object
{
public:
	GeometryController(Object *parent);
	~GeometryController();
	Ogre::MeshPtr CreatePlane(unsigned int segments_x, unsigned int segments_y, Real size_x, Real size_y, Real utile, Real vtile, const std::string& name);
	Ogre::MeshPtr CreateSphere(Real radius, Real utile, Real vtile, unsigned int rings, unsigned int segments, const std::string& name);
	Ogre::MeshPtr CreateCylinder(Real radius, Real height, Real utile, Real vtile, unsigned int segments_base, unsigned int segments_height, bool capped, const std::string& name);
	Ogre::MeshPtr CreateTorus(Real radius, Real section_radius, Real utile, Real vtile, const std::string& name);
	Ogre::MeshPtr CreateCone(Real radius, Real height, Real utile, Real vtile, unsigned int segments_base, unsigned int segments_height, const std::string& name);
	Ogre::MeshPtr CreateTube(Real inner_radius, Real outer_radius, Real height, Real utile, Real vtile, unsigned int segments_base, unsigned int segments_height, const std::string& name);
	Ogre::MeshPtr CreateBox(Real size_x, Real size_y, Real size_z, Real utile, Real vtile, unsigned int segments_x, unsigned int segments_y, unsigned int segments_z, const std::string& name);
	Ogre::MeshPtr CreateCapsule(Real radius, Real height, unsigned int rings, Real utile, Real vtile, unsigned int segments, unsigned int segments_height, bool capped, const std::string& name);
	Ogre::MeshPtr CreateTorusKnot(Real radius, Real section_radius, Real utile, Real vtile, unsigned int segments_circle, unsigned int seg_section, int p, int q, const std::string& name);
	Ogre::MeshPtr CreateIcoSphere(Real radius, Real utile, Real vtile, unsigned int iterations, const std::string& name);
	Ogre::MeshPtr CreateRoundedBox(Real size_x, Real size_y, Real size_z, Real chamfer_size, Real utile, Real vtile, unsigned int segments_x, unsigned int segments_y, unsigned int segments_z, bool capped, const std::string& name);
	Ogre::MeshPtr CreateSpring(Real radius_circle, Real radius_helix, Real height, Real round, Real utile, Real vtile, unsigned int segments_circle, unsigned int segments_path, bool capped, const std::string& name);

private:
	MeshObject* mesh;
};

}
