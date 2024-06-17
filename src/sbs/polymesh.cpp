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

#include "globals.h"
#include "sbs.h"
#include "camera.h"
#include "wall.h"
#include "texture.h"
#include "profiler.h"
#include "polymesh.h"

namespace SBS {

PolyMesh::PolyMesh(MeshObject *mesh) : ObjectBase(mesh)
{
	this->mesh = mesh;
}

PolyMesh::~PolyMesh()
{
}

Wall* PolyMesh::FindWallIntersect(const Vector3 &start, const Vector3 &end, Vector3 &isect, Real &distance, Vector3 &normal, Wall *wall)
{
	//find a wall from a 3D point
	//positions need to be in remote (Ogre) positioning
	//if wall_number is 0 or greater, this will only check that specified wall

	SBS_PROFILE("PolyMesh::FindWallIntersect");
	Real pr, best_pr = 2000000000.;
	Real dist, best_dist = 2000000000.;
	int best_i = -1;
	Vector3 cur_isect;
	Vector3 tmpnormal;

	for (size_t i = 0; i < mesh->Walls.size(); i++)
	{
		if (wall && mesh->Walls[i] != wall)
			continue;

		for (int j = 0; j < mesh->Walls[i]->GetPolygonCount(); j++)
		{
			if (mesh->Walls[i]->GetPolygon(j)->IntersectSegment(start, end, cur_isect, &pr, tmpnormal) == true)
			{
				if (pr < best_pr)
				{
					//currently test against previous camera intersection test to fix some weird issues
					Vector3 orig_start = sbs->ToRemote(sbs->camera->HitPosition);
					dist = orig_start.distance(cur_isect);

					if (dist < best_dist)
					{
						//calculate distance to intersection
						distance = start.distance(cur_isect);

						best_dist = dist;
						best_pr = pr;
						best_i = (int)i;
						isect = cur_isect;
						normal = tmpnormal;
					}
				}
			}
		}
	}

	if (best_i >= 0)
		return mesh->Walls[best_i];
	else
		return 0;
}

bool PolyMesh::CreateMesh(const std::string &name, const std::string &texture, PolyArray &vertices, Real tw, Real th, bool autosize, Matrix3 &t_matrix, Vector3 &t_vector, std::vector<std::vector<Polygon::Geometry> > &geometry, std::vector<Triangle> &triangles, PolygonSet &converted_vertices)
{
	//create custom mesh geometry, apply a texture map and material, and return the created submesh

	//exit if this mesh is a loaded file
	if (mesh->Filename != "")
		return ReportError("PolyMesh: Cannot create geometry in a file-loaded mesh");

	//get texture material
	std::string texname = texture;
	bool result;
	std::string material = sbs->GetTextureManager()->GetTextureMaterial(texture, result, true, name);
	if (!result)
		texname = "Default";

	//convert to remote positioning
	converted_vertices.resize(1);

	converted_vertices[0].reserve(vertices.size());
	for (size_t i = 0; i < vertices.size(); i++)
		converted_vertices[0].push_back(sbs->ToRemote(vertices[i]));

	//texture mapping
	Vector3 v1, v2, v3;
	int direction;

	//get texture mapping coordinates
	if (!sbs->GetTextureManager()->GetTextureMapping(converted_vertices[0], v1, v2, v3, direction))
		return ReportError("PolyMesh: Texture mapping error");

	if (tw == 0)
		tw = 1;
	if (th == 0)
		th = 1;

	//get autosize information
	Vector2 sizing (tw, th);

	if (autosize == true)
		sizing = sbs->GetTextureManager()->CalculateSizing(texname, sbs->ToLocal(v1), sbs->ToLocal(v2), sbs->ToLocal(v3), direction, tw, th);

	//get texture tiling information
	Real tw2 = sizing.x, th2 = sizing.y;
	Real mw, mh;
	if (sbs->GetTextureManager()->GetTextureTiling(texname, mw, mh))
	{
		//multiply the tiling parameters (tw and th) by
		//the stored multipliers for that texture
		tw2 = sizing.x * mw;
		th2 = sizing.y * mh;
	}

	if (!sbs->GetTextureManager()->ComputeTextureMap(t_matrix, t_vector, converted_vertices[0], v1, v2, v3, tw2, th2))
		return false;

	return CreateMesh(name, material, converted_vertices, t_matrix, t_vector, geometry, triangles, converted_vertices, tw2, th2, false);
}

bool PolyMesh::CreateMesh(const std::string &name, const std::string &material, PolygonSet &vertices, Matrix3 &tex_matrix, Vector3 &tex_vector, std::vector<std::vector<Polygon::Geometry> > &geometry, std::vector<Triangle> &triangles, PolygonSet &converted_vertices, Real tw, Real th, bool convert_vertices)
{
	//create custom geometry, apply a texture map and material, and return the created submesh
	//tw and th are only used when overriding texel map

	//convert to remote positioning
	if (convert_vertices == true)
	{
		converted_vertices.resize(vertices.size());
		for (size_t i = 0; i < vertices.size(); i++)
		{
			converted_vertices[i].reserve(vertices[i].size());
			for (size_t j = 0; j < vertices[i].size(); j++)
				converted_vertices[i].push_back(sbs->ToRemote(vertices[i][j]));
		}
	}
	else
		converted_vertices = vertices;

	//texture mapping
	Vector2 *table = GetTexels(tex_matrix, tex_vector, converted_vertices, tw, th);

	//triangulate mesh
	TriangleIndices *trimesh = new TriangleIndices[converted_vertices.size()];
	size_t trimesh_size = converted_vertices.size();

	for (size_t i = 0; i < trimesh_size; i++)
	{
		//do a (very) simple triangulation
		//this method also somewhat works with non-planar polygons
		trimesh[i].triangles.reserve(converted_vertices[i].size() - 2);
		for (size_t j = 2; j < converted_vertices[i].size(); j++)
			trimesh[i].triangles.push_back(Triangle(0, j - 1, j));
	}

	//initialize geometry arrays
	size_t size = 0;
	geometry.resize(trimesh_size);

	for (size_t i = 0; i < trimesh_size; i++)
	{
		geometry[i].resize(converted_vertices[i].size());
	}

	//populate vertices, normals, and texels for mesh data
	unsigned int k = 0;

	for (size_t i = 0; i < trimesh_size; i++)
	{
		for (size_t j = 0; j < converted_vertices[i].size(); j++)
		{
			//calculate normal
			Vector3 normal = sbs->ComputePlane(converted_vertices[i], false).normal;

			geometry[i][j].vertex = converted_vertices[i][j];
			geometry[i][j].normal = normal;
			geometry[i][j].texel = table[k];
			k++;
		}
	}

	//delete texel array
	if (table)
		delete [] table;
	table = 0;

	//add triangles to single array, to be passed to the submesh
	size_t location = 0;
	for (size_t i = 0; i < trimesh_size; i++)
	{
		if (triangles.capacity() < trimesh[i].triangles.size())
			triangles.reserve(trimesh[i].triangles.size());
		for (size_t j = 0; j < trimesh[i].triangles.size(); j++)
		{
			Triangle tri = trimesh[i].triangles[j];
			tri += location;
			triangles.push_back(tri);
		}
		location += converted_vertices[i].size();
	}

	//delete trimesh array
	delete [] trimesh;
	trimesh = 0;

	//recreate colliders if specified
	if (sbs->DeleteColliders == true)
		mesh->DeleteCollider();

	return true;
}

Vector2* PolyMesh::GetTexels(Matrix3 &tex_matrix, Vector3 &tex_vector, PolygonSet &vertices, Real tw, Real th)
{
	//return texel array for specified texture transformation matrix and vector

	if (sbs->TexelOverride == false)
	{
		//create array for texel map
		size_t texel_count = 0;
		for (size_t i = 0; i < vertices.size(); i++)
			texel_count += vertices[i].size();
		Vector2 *texels = new Vector2[texel_count];

		//transform matrix into texel map
		size_t index = 0;
		Vector3 texel_temp;
		for (size_t i = 0; i < vertices.size(); i++)
		{
			for (size_t j = 0; j < vertices[i].size(); j++)
			{
				texel_temp = tex_matrix * (vertices[i][j] - tex_vector);
				texels[index].x = texel_temp.x;
				texels[index].y = texel_temp.y;
				index++;
			}
		}
		return texels;
	}
	else
	{
		Vector2 *texels = new Vector2[4];
		texels[0].x = 0;
		texels[0].y = 0;
		texels[1].x = tw;
		texels[1].y = 0;
		texels[2].x = tw;
		texels[2].y = th;
		texels[3].x = 0;
		texels[3].y = th;
		return texels;
	}
	return 0;
}

}
