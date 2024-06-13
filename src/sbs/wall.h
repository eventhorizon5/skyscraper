/*
	Scalable Building Simulator - Wall Object
	The Skyscraper Project - Version 1.12 Alpha
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

#ifndef _SBS_WALL_H
#define _SBS_WALL_H

#include "triangle.h"
#include "mesh.h"

namespace SBS {

class Polygon;

class SBSIMPEXP Wall : public Object
{
public:

	//functions
	Wall(MeshObject* wrapper, Object *proxy = 0, bool temporary = false);
	~Wall();
	Polygon* AddQuad(const std::string &name, const std::string &texture, const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, const Vector3 &v4, Real tw, Real th, bool autosize);
	Polygon* AddPolygon(const std::string &name, const std::string &texture, PolyArray &vertices, Real tw, Real th, bool autosize);
	Polygon* AddPolygonSet(const std::string &name, const std::string &material, PolygonSet &vertices, Matrix3 &tex_matrix, Vector3 &tex_vector);
	Polygon* CreatePolygon(std::vector<Triangle> &triangles, std::vector<Extents> &index_extents, Matrix3 &tex_matrix, Vector3 &tex_vector, const std::string &material, const std::string &name, Plane &plane);
	void DeletePolygons(bool recreate_collider = true);
	void DeletePolygon(int index, bool recreate_colliders);
	int GetPolygonCount();
	Polygon* GetPolygon(int index);
	int FindPolygon(const std::string &name);
	void GetGeometry(int index, PolygonSet &vertices, bool firstonly = false, bool convert = true, bool rescale = true, bool relative = true, bool reverse = false);
	bool IntersectsWall(Vector3 start, Vector3 end, Vector3 &isect, bool convert = true);
	void Move(const Vector3 &position, Real speed = 1.0);
	MeshObject* GetMesh();
	void SetParentArray(std::vector<Wall*> &array);
	Vector3 GetPoint(const Vector3 &start, const Vector3 &end);
	Vector3 GetWallExtents(Real altitude, bool get_max);
	void ChangeHeight(Real newheight);

private:
	//mesh wrapper
	MeshObject* meshwrapper;

	//polygon array
	std::vector<Polygon*> polygons;

	//pointer to parent array
	std::vector<Wall*> *parent_array;
};

}

#endif
