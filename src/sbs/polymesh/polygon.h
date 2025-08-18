/*
	Scalable Building Simulator - Polygon Object
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

#ifndef _SBS_POLYGON_H
#define _SBS_POLYGON_H

#include <OgrePlane.h>
#include <OgreMatrix3.h>
#include "mesh.h"
#include "triangle.h"

namespace SBS {

struct Geometry
{
	//basic 3D geometry
	Vector3 vertex;
	Vector2 texel;
	Vector3 normal;
};

typedef std::vector<std::vector<Geometry> > GeometrySet;
typedef std::vector<Geometry> GeometryArray;

class SBSIMPEXP Polygon : public ObjectBase
{
public:

	MeshObject* mesh;
	GeometrySet geometry; //mesh geometry (vertices/texels/normals) container
	int vertex_count;
	std::vector<Triangle> triangles;
	Plane plane; //plane in remote (Ogre) form, relative positioning

	//texture mapping matrix and vector
	Matrix3 t_matrix;
	Vector3 t_vector;

	std::string material; //polygon material

	size_t size; //size in bytes of polygon

	Polygon(Object *parent, const std::string &name, MeshObject *meshwrapper);
	~Polygon();
	void Create(GeometrySet &geometry, std::vector<Triangle> &triangles, Matrix3 &tex_matrix, Vector3 &tex_vector, const std::string &material, Plane &plane);
	void Create(std::vector<Geometry> &geometry, std::vector<Triangle> &triangles, Matrix3 &tex_matrix, Vector3 &tex_vector, const std::string &material, Plane &plane);
	void GetTextureMapping(Matrix3 &t_matrix, Vector3 &t_vector);
	bool IntersectSegment(const Vector3 &start, const Vector3 &end, Vector3 &isect, Real *pr, Vector3 &normal);
	bool IntersectSegmentPlane(const Vector3 &start, const Vector3 &end, Vector3 &isect, Real *pr, Vector3 &normal);
	void Move(const Vector3 &vector, Real speed = 1.0);
	Plane GetAbsolutePlane();
	Vector2 GetExtents(int coord);
	void ChangeHeight(Real newheight);
	bool ReplaceTexture(const std::string &oldtexture, const std::string &newtexture);
	bool ChangeTexture(const std::string &texture, bool matcheck = true);
	Vector3 GetVertex(int index);
	bool IntersectRay(const Vector3& rayOrigin, const Vector3& rayDir, Vector3& hitPoint);
};

}

#endif
