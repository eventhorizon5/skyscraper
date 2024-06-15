/*
	Scalable Building Simulator - PolyMesh Geometry Processor
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

#ifndef _SBS_POLYMESH_H
#define _SBS_POLYMESH_H

#include "polygon.h"

namespace SBS {

typedef std::vector<Polygon*> Polygons;

class SBSIMPEXP PolyMesh : public ObjectBase
{
public:

	//define geometry type
	struct TriangleIndices
	{
		//per-submesh triangle indices
		std::vector<Triangle> triangles; //triangle data, in A B C values
	};

	//functions

	PolyMesh(MeshObject *mesh);
	~PolyMesh();
	bool CreateMesh(const std::string &name, const std::string &texture, PolyArray &vertices, Real tw, Real th, bool autosize, Matrix3 &tex_matrix, Vector3 &tex_vector, std::vector<std::vector<Polygon::Geometry> > &geometry, std::vector<Triangle> &triangles, PolygonSet &converted_vertices);
	bool CreateMesh(const std::string &name, const std::string &material, PolygonSet &vertices, Matrix3 &tex_matrix, Vector3 &tex_vector, std::vector<std::vector<Polygon::Geometry> > &geometry, std::vector<Triangle> &triangles, PolygonSet &converted_vertices, Real tw, Real th, bool convert_vertices = true);
	bool ChangeTexture(const std::string &texture, bool matcheck = true, int submesh = 0);
	bool ReplaceTexture(const std::string &oldtexture, const std::string &newtexture);
	Wall* FindWallIntersect(const Vector3 &start, const Vector3 &end, Vector3 &isect, Real &distance, Vector3 &normal, Wall *wall = 0);
	Vector2* GetTexels(Matrix3 &tex_matrix, Vector3 &tex_vector, PolygonSet &vertices, Real tw, Real th);
	int Process(std::vector<Polygon::Geometry> &vertices, std::vector<Triangle> &indices, const std::string &material, bool add);
	//int FindMatchingSubMesh(const std::string &material);
	//void DeleteVertices(int submesh, std::vector<Triangle> &deleted_indices);
	//void GetMeshInformation(const Ogre::Mesh* const mesh, int &vertex_count, Vector3* &vertices, int &index_count, unsigned long* &indices, Ogre::AxisAlignedBox &extents);
	Vector2 GetExtents(int coord, bool flip_z = false);
	//int GetSubmeshCount();

private:

	MeshObject *mesh;
};

}

#endif
