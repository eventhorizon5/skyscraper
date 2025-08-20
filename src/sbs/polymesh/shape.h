/*
	Scalable Building Simulator - Shape Object
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

#ifndef _SBS_SHAPE_H
#define _SBS_SHAPE_H

namespace SBS {

class SBSIMPEXP Shape : public ObjectBase
{
public:

	Vector3 origin; //origin point of the shape

	//functions
	Shape(Wall* parent);
	~Shape();
	void CreateSphere(const std::string &name, const std::string &texture, const Vector3 &center, Real radius, int latSteps, int lonSteps, Real tw, Real th, bool inside, bool autosize);
	void CreateBox(const std::string &name, const std::string &texture, Ogre::Vector3 &center, Real width, Real height, Real depth, Real tw, Real th, bool inside, bool autosize);
	void CreateCylinder(const std::string &name, const std::string &texture, const Vector3 &Center, Real radius, Real height, int slices, Real tw, Real th, bool inside, bool autosize);
	void CreateCone(const std::string &name, const std::string &texture, const Vector3 &Center, Real radius, Real height, int slices, Real tw, Real th, bool inside, bool autosize);
	void CreateCapsule(const std::string &name, const std::string &texture, const Vector3 &Center, Real radius, Real height, int latSteps, int lonSteps, Real tw, Real th, bool inside, bool autosize);
	void CreatePlane(const std::string &name, const std::string &texture, const Vector3 &Center, Real width, Real depth, int xSegments, int zSegments, Real tw, Real th, bool face_down, bool autosize);
	void CreateCircle(const std::string &name, const std::string &texture, const Vector3 &Center, Real radius, int slices, Real tw, Real th, bool face_down, bool autosize);
	void CreateTorus(const std::string &name, const std::string &texture, const Vector3 &Center, Real majorRadius, Real minorRadius, int radialSteps, int tubeSteps, Real tw, Real th, bool inside, bool autosize);
	void CreateDome(const std::string &name, const std::string &texture, const Vector3 &Center, Real radius, int latSteps, int lonSteps, Real tw, Real th, bool inside, bool autosize);
	void CreatePyramid(const std::string &name, const std::string &texture, const Vector3 &Center, Real width, Real depth, Real height, Real tw, Real th, bool inside, bool autosize);
	void CreatePrism(const std::string &name, const std::string &texture, const Vector3 &Center, Real width, Real depth, Real height, Real tw, Real th, bool inside, bool autosize);
	void CreateTetrahedron(const std::string &name, const std::string &texture, const Vector3 &Center, Real size, Real tw, Real th, bool inside, bool autosize);
	void CreateOctahedron(const std::string &name, const std::string &texture, const Vector3 &Center, Real size, Real tw, Real th, bool inside, bool autosize);
	void CreateIcosahedron(const std::string &name, const std::string &texture, const Vector3 &Center, Real radius, Real tw, Real th, bool inside, bool autosize);
	void CreateGeoSphere(const std::string &name, const std::string &texture, const Vector3 &Center, Real radius, int subdivisions, Real tw, Real th, bool inside, bool autosize);

private:
	Wall* parent;
	PolyMesh* polymesh;
};

}

#endif
