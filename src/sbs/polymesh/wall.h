/*
	Scalable Building Simulator - Wall Object
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

#ifndef _SBS_WALL_H
#define _SBS_WALL_H

#include "polygon.h"
#include "triangle.h"
#include "mesh.h"

namespace SBS {

class PolyMesh;

class SBSIMPEXP Wall : public Object
{
	friend class MeshObject;

public:

	//functions
	Wall(MeshObject* wrapper, const std::string &name);
	~Wall();
	Polygon* AddQuad(const std::string &name, const std::string &texture, const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, const Vector3 &v4, Real tw, Real th, bool autosize);
	Polygon* AddTriangle(const std::string &name, const std::string &texture, const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, Real tw, Real th, bool autosize);
	Polygon* AddPolygon(const std::string &name, const std::string &texture, PolyArray &vertices, Real tw, Real th, bool autosize);
	Polygon* AddPolygon(const std::string &name, const std::string &texture, PolygonSet &vertices, std::vector<std::vector<Vector2>> &uvMap, std::vector<Triangle> &triangles, Real tw, Real th, bool autosize);
	Polygon* AddPolygonSet(const std::string &name, const std::string &material, PolygonSet &vertices, Matrix3 &tex_matrix, Vector3 &tex_vector);
	Polygon* AddPolygonSet(const std::string& name, const std::string& material, const GeometrySet &polys);
	void DeletePolygons(bool recreate_collider = true);
	void DeletePolygon(int index, bool recreate_colliders);
	int GetPolygonCount();
	Polygon* GetPolygon(int index);
	int FindPolygon(const std::string &name);
	bool IntersectsWall(Vector3 start, Vector3 end, Vector3 &isect, bool convert = true);
	void Move(const Vector3 &vector, Real speed = 1.0);
	MeshObject* GetMesh();
	void SetParentArray(std::vector<Wall*> &array);
	Vector3 GetPoint(const Vector3 &start, const Vector3 &end);
	Vector3 GetWallExtents(Real altitude, bool get_max);
	void ChangeHeight(Real newheight);
	unsigned int GetVertexCount();
	unsigned int GetTriangleCount();
	bool ReplaceTexture(const std::string &oldtexture, const std::string &newtexture);
	bool ChangeTexture(const std::string &texture, bool matcheck = true);

private:
	//mesh wrapper
	MeshObject* meshwrapper;

	//polymesh system
	PolyMesh* polymesh;

	//polygon array
	std::vector<Polygon*> polygons;

	//pointer to parent array
	std::vector<Wall*> *parent_array;
};

}

#endif
