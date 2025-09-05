/*
	Scalable Building Simulator - PolyMesh Geometry Processor
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

#include "globals.h"
#include "sbs.h"
#include "trigger.h"
#include "camera.h"
#include "wall.h"
#include "texman.h"
#include "profiler.h"
#include "utility.h"
#include "polymesh.h"

namespace SBS {

PolyMesh::PolyMesh(Object *parent) : ObjectBase(parent)
{
	WallCount = 0;
	PolygonCount = 0;
	wall_orientation = 1;
	floor_orientation = 2;
	DrawMainN = true;
	DrawMainP = true;
	DrawSideN = false;
	DrawSideP = false;
	DrawTop = false;
	DrawBottom = false;
	DrawMainNOld = true;
	DrawMainPOld = true;
	DrawSideNOld = false;
	DrawSidePOld = false;
	DrawTopOld = false;
	DrawBottomOld = false;

	ResetDoorwayWalls();
}

PolyMesh::~PolyMesh()
{
}

Wall* PolyMesh::FindWallIntersect(MeshObject *mesh, const Vector3 &start, const Vector3 &end, Vector3 &isect, Real &distance, Vector3 &normal, Wall *wall)
{
	//find a wall from a 3D point
	//positions need to be in remote (Ogre) positioning

	SBS_PROFILE("PolyMesh::FindWallIntersect");

    Real best_dist = 2000000000.;
    int best_i = -1;
    Vector3 cur_isect;
    Vector3 tmpnormal;

    for (size_t i = 0; i < mesh->Walls.size(); i++)
    {
        if (!mesh->Walls[i])
            continue;

        if (wall && mesh->Walls[i] != wall)
            continue;

        for (int j = 0; j < mesh->Walls[i]->GetPolygonCount(); j++)
        {
            Polygon *poly = mesh->Walls[i]->GetPolygon(j);
            if (!poly)
                continue;

            Real pr;
            if (poly->IntersectSegment(start, end, cur_isect, &pr, tmpnormal) == true)
            {
                Real dist = start.distance(cur_isect);
                if (dist < best_dist)
                {
                    best_dist = dist;
                    best_i = (int)i;
                    isect = cur_isect;
                    normal = tmpnormal;
                    distance = dist;
                }
            }
        }
    }

    if (best_i >= 0)
        return mesh->Walls[best_i];
    else
        return 0;
}

bool PolyMesh::CreateMesh(MeshObject *mesh, Wall* ownerWall, Polygon* ownerPoly, const std::string &name, const std::string &texture, PolyArray &vertices, Real tw, Real th, bool autosize, Matrix3 &t_matrix, Vector3 &t_vector, GeometrySet &geometry, std::vector<Triangle> &triangles, PolygonSet &converted_vertices)
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
		converted_vertices[0].emplace_back(sbs->ToRemote(vertices[i]));

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

	return CreateMesh(mesh, ownerWall, ownerPoly, name, material, converted_vertices, t_matrix, t_vector, geometry, triangles, converted_vertices, tw2, th2, false);
}

bool PolyMesh::CreateMesh(MeshObject *mesh, Wall* ownerWall, Polygon* ownerPoly, const std::string &name, const std::string &material, PolygonSet &vertices, Matrix3 &tex_matrix, Vector3 &tex_vector, GeometrySet &geometry, std::vector<Triangle> &triangles, PolygonSet &converted_vertices, Real tw, Real th, bool convert_vertices)
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
				converted_vertices[i].emplace_back(sbs->ToRemote(vertices[i][j]));
		}
	}
	else
		converted_vertices = vertices;

	//texture mapping
	size_t texel_count;
	Vector2 *table = GetTexels(tex_matrix, tex_vector, converted_vertices, tw, th, texel_count);

	//append to flattened pick buffers
	uint32_t base = static_cast<uint32_t>(mesh->pickPositions.size());
	for (size_t i = 0; i < converted_vertices.size(); ++i)
	{
		for (const auto& p : converted_vertices[i])
		{
			mesh->pickPositions.emplace_back(p); //remote space
		}
	}

	//triangulate mesh
	TriangleIndices *trimesh = new TriangleIndices[converted_vertices.size()];
	size_t trimesh_size = converted_vertices.size();

	for (size_t i = 0; i < trimesh_size; i++)
	{
		//do a (very) simple triangulation
		//this method also somewhat works with non-planar polygons
		trimesh[i].triangles.reserve(converted_vertices[i].size() - 2);
		for (size_t j = 2; j < converted_vertices[i].size(); j++)
			trimesh[i].triangles.emplace_back(Triangle(0, j - 1, j));
	}

	//initialize geometry arrays
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
			Vector3 normal = ComputePlane(converted_vertices[i], false).normal;

			geometry[i][j].vertex = converted_vertices[i][j];
			geometry[i][j].normal = normal;

			if (k >= texel_count)
				return ReportError("PolyMesh: invalid texel index");
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
			triangles.emplace_back(tri);

			//also append to pick index buffer + ownership in the same order
			mesh->pickIndices.push_back(base + tri.a);
			mesh->pickIndices.push_back(base + tri.b);
			mesh->pickIndices.push_back(base + tri.c);

			//append ownership information
			mesh->triOwners.push_back({ownerWall, ownerPoly});
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

bool PolyMesh::CreateMesh(MeshObject *mesh, Wall* ownerWall, Polygon* ownerPoly, const std::string &name, const std::string &material, PolygonSet &vertices, std::vector<std::vector<Vector2>> &uvMap, GeometrySet &geometry, std::vector<Triangle> &triangles, PolygonSet &converted_vertices, Real tw, Real th, bool convert_vertices)
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
				converted_vertices[i].emplace_back(sbs->ToRemote(vertices[i][j]));
		}
	}
	else
		converted_vertices = vertices;

	//append to flattened pick buffers
	uint32_t base = static_cast<uint32_t>(mesh->pickPositions.size());
	for (size_t i = 0; i < converted_vertices.size(); ++i)
	{
		for (const auto& p : converted_vertices[i])
		{
			mesh->pickPositions.emplace_back(p); //remote space
		}
	}

	//triangulate mesh
	TriangleIndices *trimesh = new TriangleIndices[converted_vertices.size()];
	size_t trimesh_size = converted_vertices.size();

	for (size_t i = 0; i < trimesh_size; i++)
	{
		//do a (very) simple triangulation
		//this method also somewhat works with non-planar polygons
		trimesh[i].triangles.reserve(converted_vertices[i].size() - 2);
		for (size_t j = 2; j < converted_vertices[i].size(); j++)
			trimesh[i].triangles.emplace_back(Triangle(0, j - 1, j));
	}

	//initialize geometry arrays
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
			Vector3 normal = ComputePlane(converted_vertices[i], false).normal;

			geometry[i][j].vertex = converted_vertices[i][j];
			geometry[i][j].normal = normal;

			if (i >= uvMap.size())
				return ReportError("PolyMesh: invalid polygon index for texel");
			if (j >= uvMap[i].size())
				return ReportError("PolyMesh: invalid texel index");
			geometry[i][j].texel = uvMap[i][j];
			k++;
		}
	}

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
			triangles.emplace_back(tri);

			//also append to pick index buffer + ownership in the same order
			mesh->pickIndices.push_back(base + tri.a);
			mesh->pickIndices.push_back(base + tri.b);
			mesh->pickIndices.push_back(base + tri.c);

			//append ownership information
			mesh->triOwners.push_back({ownerWall, ownerPoly});
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

Vector2* PolyMesh::GetTexels(Matrix3 &tex_matrix, Vector3 &tex_vector, PolygonSet &vertices, Real tw, Real th, size_t &texel_count)
{
	//return texel array for specified texture transformation matrix and vector

	texel_count = 0;

	if (sbs->TexelOverride == false)
	{
		//create array for texel map
		for (size_t i = 0; i < vertices.size(); i++)
			texel_count += vertices[i].size();
		Vector2 *texels = new Vector2[texel_count];

		//transform matrix into texel map
		size_t index = 0;
		Vector3 texel;
		for (size_t i = 0; i < vertices.size(); i++)
		{
			for (size_t j = 0; j < vertices[i].size(); j++)
			{
				texel = tex_matrix * (vertices[i][j] - tex_vector);
				texels[index].x = texel.x;
				texels[index].y = texel.y;
				index++;
			}
		}
		return texels;
	}
	else
	{
		texel_count = 4;
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

bool PolyMesh::AddWallMain(Wall* wallobject, const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real height_in1, Real height_in2, Real altitude1, Real altitude2, Real tw, Real th, bool autosize, bool report)
{
	//Adds a wall with the specified dimensions

	//if report is true, report if trying to create a wall outside of the engine boundary

	//exit if coordinates are invalid
	if (x1 == x2 && z1 == z2)
		return ReportError("Invalid coordinates for wall '" + name + "'");

	if (height_in1 == 0.0f && height_in2 == 0.0)
		return ReportError("No wall height specified for wall '" + name + "'");

	//determine axis of wall
	int axis = 0;
	if (std::abs(x1 - x2) > (std::abs(z1 - z2) + 0.00001))
		//x axis
		axis = 1;
	else
		//z axis
		axis = 2;

	//convert to clockwise coordinates if on x-axis, or counterclockwise if on z-axis
	if ((x1 > x2 && axis == 1) || (z1 < z2 && axis == 2))
	{
		//reverse coordinates
		std::swap(x1, x2);
		std::swap(z1, z2);
		std::swap(altitude1, altitude2);
		std::swap(height_in1, height_in2);
	}

	//map coordinates
	Vector3 v1 (x1, altitude1 + height_in1, z1); //left top
	Vector3 v2 (x2, altitude2 + height_in2, z2); //right top
	Vector3 v3 (x2, altitude2, z2); //right base
	Vector3 v4 (x1, altitude1, z1); //left base

	Vector3 v5 = v1;
	Vector3 v6 = v2;
	Vector3 v7 = v3;
	Vector3 v8 = v4;

	//exit if outside of the engine boundaries
	if (sbs->GetAreaTrigger())
	{
		Vector3 v1x = wallobject->GetMesh()->GetPosition() + v1;
		Vector3 v2x = wallobject->GetMesh()->GetPosition() + v3;
		if (sbs->GetAreaTrigger()->IsOutside(v1x, v2x) == true)
		{
			if (report == true)
				ReportError("Cannot create wall outside of engine boundaries");
			return false;
		}
	}

	//expand to specified thickness
	if (axis == 1)
	{
		//x axis
		if (wall_orientation == 0)
		{
			//left
			v5.z += thickness;
			v6.z += thickness;
			v7.z += thickness;
			v8.z += thickness;
		}
		if (wall_orientation == 1)
		{
			//center
			Real half = thickness / 2;
			v1.z -= half;
			v2.z -= half;
			v3.z -= half;
			v4.z -= half;
			v5.z += half;
			v6.z += half;
			v7.z += half;
			v8.z += half;
		}
		if (wall_orientation == 2)
		{
			//right
			v1.z -= thickness;
			v2.z -= thickness;
			v3.z -= thickness;
			v4.z -= thickness;
		}
	}
	else
	{
		//z axis
		if (wall_orientation == 0)
		{
			//left
			v5.x += thickness;
			v6.x += thickness;
			v7.x += thickness;
			v8.x += thickness;
		}
		if (wall_orientation == 1)
		{
			//center
			Real half = thickness / 2;
			v1.x -= half;
			v2.x -= half;
			v3.x -= half;
			v4.x -= half;
			v5.x += half;
			v6.x += half;
			v7.x += half;
			v8.x += half;
		}
		if (wall_orientation == 2)
		{
			//right
			v1.x -= thickness;
			v2.x -= thickness;
			v3.x -= thickness;
			v4.x -= thickness;
		}
	}

	//create polygons and set names
	std::string NewName, texture2 = texture;
	Real tw2 = tw, th2 = th;

	TextureManager *texturemanager = sbs->GetTextureManager();
	bool FlipTexture = texturemanager->FlipTexture;
	bool TextureOverride = texturemanager->TextureOverride;

	if (FlipTexture == true)
		texturemanager->ProcessTextureFlip(tw, th);

	if (DrawMainN == true)
	{
		if (FlipTexture == true)
		{
			tw2 = texturemanager->widthscale[0];
			th2 = texturemanager->heightscale[0];
		}
		if (TextureOverride == true)
			texture2 = texturemanager->mainnegtex;

		NewName = name;
		if (GetDrawWallsCount() > 1)
			NewName.append(":front");
		wallobject->AddQuad(NewName, texture2, v1, v2, v3, v4, tw2, th2, autosize); //front wall
	}

	if (DrawMainP == true)
	{
		if (FlipTexture == true)
		{
			tw2 = texturemanager->widthscale[1];
			th2 = texturemanager->heightscale[1];
		}
		if (TextureOverride == true)
			texture2 = texturemanager->mainpostex;

		NewName = name;
		NewName.append(":back");
		wallobject->AddQuad(NewName, texture2, v6, v5, v8, v7, tw2, th2, autosize); //back wall
	}

	if (thickness != 0.0)
	{
		if (DrawSideN == true)
		{
			if (FlipTexture == true)
			{
				tw2 = texturemanager->widthscale[2];
				th2 = texturemanager->heightscale[2];
			}
			if (TextureOverride == true)
				texture2 = texturemanager->sidenegtex;

			NewName = name;
			NewName.append(":left");
			if (axis == 1)
				wallobject->AddQuad(NewName, texture2, v5, v1, v4, v8, tw2, th2, autosize); //left wall
			else
				wallobject->AddQuad(NewName, texture2, v2, v6, v7, v3, tw2, th2, autosize); //left wall
		}

		if (DrawSideP == true)
		{
			if (FlipTexture == true)
			{
				tw2 = texturemanager->widthscale[3];
				th2 = texturemanager->heightscale[3];
			}
			if (TextureOverride == true)
				texture2 = texturemanager->sidepostex;

			NewName = name;
			NewName.append(":right");
			if (axis == 1)
				wallobject->AddQuad(NewName, texture2, v2, v6, v7, v3, tw2, th2, autosize); //right wall
			else
				wallobject->AddQuad(NewName, texture2, v5, v1, v4, v8, tw2, th2, autosize); //right wall
		}

		if (DrawTop == true)
		{
			if (FlipTexture == true)
			{
				tw2 = texturemanager->widthscale[4];
				th2 = texturemanager->heightscale[4];
			}
			if (TextureOverride == true)
				texture2 = texturemanager->toptex;

			NewName = name;
			NewName.append(":top");
			wallobject->AddQuad(NewName, texture2, v5, v6, v2, v1, tw2, th2, autosize); //top wall
		}

		if (DrawBottom == true)
		{
			if (FlipTexture == true)
			{
				tw2 = texturemanager->widthscale[5];
				th2 = texturemanager->heightscale[5];
			}
			if (TextureOverride == true)
				texture2 = texturemanager->bottomtex;

			NewName = name;
			NewName.append(":bottom");
			wallobject->AddQuad(NewName, texture2, v4, v3, v7, v8, tw2, th2, autosize); //bottom wall
		}
	}

	return true;
}

bool PolyMesh::AddFloorMain(Wall* wallobject, const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real altitude1, Real altitude2, bool reverse_axis, bool texture_direction, Real tw, Real th, bool autosize, bool legacy_behavior, bool report)
{
	//Adds a floor with the specified dimensions and vertical offset

	//direction determines the direction of slope (for different altitude values):
	//false - left/right from altitude1 to altitude2, or legacy (broken) "ReverseAxis = false" behavior if legacy_behavior is true
	//true - back/forwards from altitude1 to altitude2, or legacy (broken) "ReverseAxis = true" behavior if legacy_behavior is true

	//if report is true, report if trying to create a floor outside of the engine boundary

	//exit if coordinates are invalid
	if (x1 == x2 || z1 == z2)
		return ReportError("Invalid coordinates for floor '" + name + "'");

	//convert to clockwise coordinates

	//determine axis of floor
	int axis = 0;
	if (std::abs(x1 - x2) > (std::abs(z1 - z2) + 0.00001))
		//x axis
		axis = 1;
	else
		//z axis
		axis = 2;

	if (legacy_behavior == false)
	{
		//current behavior

		if (x1 > x2)
		{
			std::swap(x1, x2);

			if (reverse_axis == true)
				std::swap(altitude1, altitude2);
		}
		if (z1 > z2)
		{
			std::swap(z1, z2);

			if (reverse_axis == false)
				std::swap(altitude1, altitude2);
		}
	}
	else
	{
		//legacy (broken) behavior, for compatibility with previous versions

		if ((x1 > x2 && axis == 1) || (z1 > z2 && axis == 2))
		{
			//reverse coordinates if the difference between x or z coordinates is greater
			std::swap(x1, x2);
			std::swap(z1, z2);
			std::swap(altitude1, altitude2);
		}
	}

	//map coordinates
	Vector3 v1, v2, v3, v4;

	if (reverse_axis == false)
	{
		v1 = Vector3(x1, altitude1, z1); //bottom left
		v2 = Vector3(x2, altitude1, z1); //bottom right
		v3 = Vector3(x2, altitude2, z2); //top right
		v4 = Vector3(x1, altitude2, z2); //top left
	}
	else
	{
		if (legacy_behavior == true)
		{
			v1 = Vector3(x1, altitude1, z1); //bottom left
			v2 = Vector3(x1, altitude1, z2); //top left
			v3 = Vector3(x2, altitude2, z2); //top right
			v4 = Vector3(x2, altitude2, z1); //bottom right
		}
		else
		{
			v1 = Vector3(x2, altitude2, z1); //bottom right
			v2 = Vector3(x2, altitude2, z2); //top right
			v3 = Vector3(x1, altitude1, z2); //top left
			v4 = Vector3(x1, altitude1, z1); //bottom left
		}
	}

	Vector3 v5 = v1;
	Vector3 v6 = v2;
	Vector3 v7 = v3;
	Vector3 v8 = v4;

	//exit if outside of the engine boundaries
	if (sbs->GetAreaTrigger())
	{
		Vector3 v1x = wallobject->GetMesh()->GetPosition() + v1;
		Vector3 v2x = wallobject->GetMesh()->GetPosition() + v3;
		if (sbs->GetAreaTrigger()->IsOutside(v1x, v2x) == true)
		{
			if (report == true)
				ReportError("Cannot create floor outside of engine boundaries");
			return false;
		}
	}

	//expand to specified thickness
	if (floor_orientation == 0)
	{
		//bottom
		v5.y += thickness;
		v6.y += thickness;
		v7.y += thickness;
		v8.y += thickness;
	}
	if (floor_orientation == 1)
	{
		//center
		Real half = thickness / 2;
		v1.y -= half;
		v2.y -= half;
		v3.y -= half;
		v4.y -= half;
		v5.y += half;
		v6.y += half;
		v7.y += half;
		v8.y += half;
	}
	if (floor_orientation == 2)
	{
		//top
		v1.y -= thickness;
		v2.y -= thickness;
		v3.y -= thickness;
		v4.y -= thickness;
	}

	//create polygons and set names
	std::string NewName, texture2 = texture;
	Real tw2 = tw, th2 = th;

	TextureManager *texturemanager = sbs->GetTextureManager();
	bool FlipTexture = texturemanager->FlipTexture;
	bool TextureOverride = texturemanager->TextureOverride;

	if (FlipTexture == true)
		texturemanager->ProcessTextureFlip(tw, th);

	//turn on rotation if set
	bool old_planarrotate = texturemanager->GetPlanarRotate();
	texturemanager->SetPlanarRotate(texture_direction);

	if (DrawMainN == true)
	{
		if (FlipTexture == true)
		{
			tw2 = texturemanager->widthscale[0];
			th2 = texturemanager->heightscale[0];
		}
		if (TextureOverride == true)
			texture2 = texturemanager->mainnegtex;

		NewName = name;
		if (GetDrawWallsCount() > 1)
			NewName.append(":front");
		wallobject->AddQuad(NewName, texture2, v1, v2, v3, v4, tw2, th2, autosize); //bottom wall
	}

	if (DrawMainP == true)
	{
		if (FlipTexture == true)
		{
			tw2 = texturemanager->widthscale[1];
			th2 = texturemanager->heightscale[1];
		}
		if (TextureOverride == true)
			texture2 = texturemanager->mainpostex;

		NewName = name;
		NewName.append(":back");
		wallobject->AddQuad(NewName, texture2, v8, v7, v6, v5, tw2, th2, autosize); //top wall
	}

	if (thickness != 0.0)
	{
		if (DrawSideN == true)
		{
			if (FlipTexture == true)
			{
				tw2 = texturemanager->widthscale[2];
				th2 = texturemanager->heightscale[2];
			}
			if (TextureOverride == true)
				texture2 = texturemanager->sidenegtex;

			NewName = name;
			NewName.append(":left");
			wallobject->AddQuad(NewName, texture2, v8, v5, v1, v4, tw2, th2, autosize); //left wall
		}

		if (DrawSideP == true)
		{
			if (FlipTexture == true)
			{
				tw2 = texturemanager->widthscale[3];
				th2 = texturemanager->heightscale[3];
			}
			if (TextureOverride == true)
				texture2 = texturemanager->sidepostex;

			NewName = name;
			NewName.append(":right");
			wallobject->AddQuad(NewName, texture2, v6, v7, v3, v2, tw2, th2, autosize); //right wall
		}

		if (DrawTop == true)
		{
			if (FlipTexture == true)
			{
				tw2 = texturemanager->widthscale[4];
				th2 = texturemanager->heightscale[4];
			}
			if (TextureOverride == true)
				texture2 = texturemanager->toptex;

			NewName = name;
			NewName.append(":top");
			wallobject->AddQuad(NewName, texture2, v5, v6, v2, v1, tw2, th2, autosize); //front wall
		}

		if (DrawBottom == true)
		{
			if (FlipTexture == true)
			{
				tw2 = texturemanager->widthscale[5];
				th2 = texturemanager->heightscale[5];
			}
			if (TextureOverride == true)
				texture2 = texturemanager->bottomtex;

			NewName = name;
			NewName.append(":bottom");
			wallobject->AddQuad(NewName, texture2, v7, v8, v4, v3, tw2, th2, autosize); //back wall
		}
	}

	texturemanager->SetPlanarRotate(old_planarrotate);

	return true;
}

Wall* PolyMesh::CreateWallBox(MeshObject* mesh, const std::string &name, const std::string &texture, Real x1, Real x2, Real z1, Real z2, Real height_in, Real voffset, Real tw, Real th, bool inside, bool outside, bool top, bool bottom, bool autosize, bool report)
{
	//create 4 walls

	if (!mesh)
		return 0;

	//exit if coordinates are invalid
	if (x1 == x2 && z1 == z2)
	{
		ReportError("Invalid coordinates for wall '" + name + "'");
		return 0;
	}

	//create wall object
	Wall *wall = mesh->CreateWallObject(name);

	bool x_thickness = false, z_thickness = false;
	std::string NewName, texture2 = texture;

	if (x1 != x2)
		x_thickness = true;
	if (z1 != z2)
		z_thickness = true;

	//swap values if the first is greater than the second
	if (x1 > x2)
		std::swap(x1, x2);

	if (z1 > z2)
		std::swap(z1, z2);

	//exit if outside of the engine boundaries
	if (sbs->GetAreaTrigger())
	{
		if (sbs->GetAreaTrigger()->IsOutside(Vector3(x1, voffset, z1), Vector3(x2, voffset + height_in, z2)) == true)
		{
			if (report == true)
				ReportError("Cannot create wall box outside of engine boundaries");
			return 0;
		}
	}

	TextureManager *texturemanager = sbs->GetTextureManager();
	bool TextureOverride = texturemanager->TextureOverride;

	if (inside == true)
	{
		//generate a box visible from the inside

		NewName = name;
		NewName.append(":inside");

		if (x_thickness == true)
		{
			if (TextureOverride == true)
				texture2 = texturemanager->mainnegtex;

			wall->AddQuad( //front
					NewName,
					texture2,
					Vector3(x1, voffset, z1),
					Vector3(x2, voffset, z1),
					Vector3(x2, voffset + height_in, z1),
					Vector3(x1, voffset + height_in, z1), tw, th, autosize);

			if (TextureOverride == true)
				texture2 = texturemanager->mainpostex;

			wall->AddQuad( //back
					NewName,
					texture2,
					Vector3(x2, voffset, z2),
					Vector3(x1, voffset, z2),
					Vector3(x1, voffset + height_in, z2),
					Vector3(x2, voffset + height_in, z2), tw, th, autosize);
		}
		if (z_thickness == true)
		{
			if (TextureOverride == true)
				texture2 = texturemanager->sidepostex;

			wall->AddQuad( //right
					NewName,
					texture2,
					Vector3(x2, voffset, z1),
					Vector3(x2, voffset, z2),
					Vector3(x2, voffset + height_in, z2),
					Vector3(x2, voffset + height_in, z1), tw, th, autosize);

			if (TextureOverride == true)
				texture2 = texturemanager->sidenegtex;

			wall->AddQuad( //left
					NewName,
					texture2,
					Vector3(x1, voffset, z2),
					Vector3(x1, voffset, z1),
					Vector3(x1, voffset + height_in, z1),
					Vector3(x1, voffset + height_in, z2), tw, th, autosize);
		}
		if (x_thickness == true && z_thickness == true)
		{
			if (bottom == true)
			{
				if (TextureOverride == true)
					texture2 = texturemanager->bottomtex;

				wall->AddQuad( //bottom
						NewName,
						texture2,
						Vector3(x1, voffset, z2),
						Vector3(x2, voffset, z2),
						Vector3(x2, voffset, z1),
						Vector3(x1, voffset, z1), tw, th, autosize);
			}

			if (top == true)
			{
				if (TextureOverride == true)
					texture2 = texturemanager->toptex;

				wall->AddQuad( //top
						NewName,
						texture2,
						Vector3(x1, voffset + height_in, z1),
						Vector3(x2, voffset + height_in, z1),
						Vector3(x2, voffset + height_in, z2),
						Vector3(x1, voffset + height_in, z2), tw, th, autosize);
			}
		}
	}

	if (outside == true)
	{
		NewName = name;
		NewName.append(":outside");

		if (x_thickness == true)
		{
			if (TextureOverride == true)
				texture2 = texturemanager->mainnegtex;

			wall->AddQuad( //front
					NewName,
					texture2,
					Vector3(x1, voffset + height_in, z1),
					Vector3(x2, voffset + height_in, z1),
					Vector3(x2, voffset, z1),
					Vector3(x1, voffset, z1), tw, th, autosize);

			if (TextureOverride == true)
				texture2 = texturemanager->mainpostex;

			wall->AddQuad( //back
					NewName,
					texture2,
					Vector3(x2, voffset + height_in, z2),
					Vector3(x1, voffset + height_in, z2),
					Vector3(x1, voffset, z2),
					Vector3(x2, voffset, z2), tw, th, autosize);
		}
		if (z_thickness == true)
		{
			if (TextureOverride == true)
				texture2 = texturemanager->sidepostex;

			wall->AddQuad( //right
					NewName,
					texture2,
					Vector3(x2, voffset + height_in, z1),
					Vector3(x2, voffset + height_in, z2),
					Vector3(x2, voffset, z2),
					Vector3(x2, voffset, z1), tw, th, autosize);

			if (TextureOverride == true)
				texture2 = texturemanager->sidenegtex;

			wall->AddQuad( //left
					NewName,
					texture2,
					Vector3(x1, voffset + height_in, z2),
					Vector3(x1, voffset + height_in, z1),
					Vector3(x1, voffset, z1),
					Vector3(x1, voffset, z2), tw, th, autosize);
		}
		if (x_thickness == true && z_thickness == true)
		{
			if (bottom == true)
			{
				if (TextureOverride == true)
					texture2 = texturemanager->bottomtex;

				wall->AddQuad( //bottom
						NewName,
						texture2,
						Vector3(x1, voffset, z1),
						Vector3(x2, voffset, z1),
						Vector3(x2, voffset, z2),
						Vector3(x1, voffset, z2), tw, th, autosize);
			}
			if (top == true)
			{
				if (TextureOverride == true)
					texture2 = texturemanager->toptex;

				wall->AddQuad( //top
						NewName,
						texture2,
						Vector3(x1, voffset + height_in, z2),
						Vector3(x2, voffset + height_in, z2),
						Vector3(x2, voffset + height_in, z1),
						Vector3(x1, voffset + height_in, z1), tw, th, autosize);
			}
		}
	}
	return wall;
}

Wall* PolyMesh::CreateWallBox2(MeshObject* mesh, const std::string &name, const std::string &texture, Real CenterX, Real CenterZ, Real WidthX, Real LengthZ, Real height_in, Real voffset, Real tw, Real th, bool inside, bool outside, bool top, bool bottom, bool autosize)
{
	//create 4 walls from a central point

	Real x1 = CenterX - (WidthX / 2);
	Real x2 = CenterX + (WidthX / 2);
	Real z1 = CenterZ - (LengthZ / 2);
	Real z2 = CenterZ + (LengthZ / 2);

	return CreateWallBox(mesh, name, texture, x1, x2, z1, z2, height_in, voffset, tw, th, inside, outside, top, bottom, autosize);
}

void PolyMesh::AddPolygon(Wall* wallobject, const std::string &texture, const std::string &side_texture, Real thickness, PolyArray &varray, Real tw, Real th, Real side_tw, Real side_th, bool autosize, bool report)
{
	//creates a polygon in the specified wall object

	if (!wallobject)
		return;

	PolyArray varray1 = varray;
	PolyArray varray2;

	//get number of stored vertices
	size_t num = varray.size();

	//create a second array with reversed vertices
	varray2.reserve(num);
	for (size_t i = num - 1; i < num; --i)
		varray2.emplace_back(varray1[i]);

	//create 2 polygons (front and back) from the vertex array

	//get polygon native direction
	Vector3 direction = GetPolygonDirection(varray);

	//if the polygon is facing right, down or to the back, reverse faces
	//to keep the vertices clockwise
	if (direction.x == 1 || direction.y == -1 || direction.z == 1)
		std::swap(varray1, varray2);

	std::string name = wallobject->GetName();
	TextureManager *texturemanager = sbs->GetTextureManager();

	//exit if outside of the engine boundaries
	if (sbs->GetAreaTrigger())
	{
		for (size_t i = 0; i < varray1.size(); i++)
		{
			if (sbs->GetAreaTrigger()->IsOutside(varray1[i]) == true)
			{
				if (report == true)
					ReportError("Cannot create polygon outside of engine boundaries");
				return;
			}
		}
	}

	//extrude given polygon
	PolygonSet polyset;
	ExtrudePolygon(varray1, thickness, polyset);

	if (polyset.size() < 2)
		return;

	//add the polygons
	if (DrawMainN == true)
	{
		std::string NewName = name;
		if (DrawMainP == true)
			NewName.append(":0");
		std::string final_texture = texture;
		if (texturemanager->TextureOverride == true)
			final_texture = texturemanager->mainnegtex;
		wallobject->AddPolygon(NewName, final_texture, polyset[0], tw, th, autosize);
	}
	if (DrawMainP == true)
	{
		std::string NewName = name;
		if (DrawMainN == true)
			NewName.append(":1");
		std::string final_texture = texture;
		if (texturemanager->TextureOverride == true)
			final_texture = texturemanager->mainpostex;
		wallobject->AddPolygon(NewName, final_texture, polyset[1], tw, th, autosize);
	}
	if ((DrawSideN || DrawSideP) && polyset.size() > 2 && thickness > 0.0)
	{
		for (size_t i = 2; i < polyset.size(); ++i)
		{
			std::string NewName = name;
			if (DrawMainN == true)
				NewName.append(":" + ToString(i));
			std::string final_texture = side_texture;
			if (texturemanager->TextureOverride == true)
			{
				if (DrawSideN == true)
					final_texture = texturemanager->sidenegtex;
				else
					final_texture = texturemanager->sidepostex;
			}
			wallobject->AddPolygon(NewName, final_texture, polyset[i], side_tw, side_th, autosize);
		}
	}
}

Wall* PolyMesh::AddCustomWall(MeshObject* mesh, const std::string &name, const std::string &texture, const std::string &side_texture, Real thickness, PolyArray &varray, Real tw, Real th, Real side_tw, Real side_th, bool autosize)
{
	//Adds a wall from a specified array of 3D vectors

	if (!mesh)
		return 0;

	//create wall object
	Wall *wall = mesh->CreateWallObject(name);

	//create polygon in wall object
	AddPolygon(wall, texture, side_texture, thickness, varray, tw, th, side_tw, side_th, autosize);

	return wall;
}

Wall* PolyMesh::AddCustomFloor(MeshObject* mesh, const std::string &name, const std::string &texture, const std::string &side_texture, Real thickness, std::vector<Vector2> &varray, Real altitude, Real tw, Real th, Real side_tw, Real side_th, bool autosize)
{
	//Same as AddCustomWall, with only one altitude value value
	PolyArray varray3;

	//set up 3D vertex array
	varray3.reserve(varray.size());
	for (size_t i = 0; i < varray.size(); i++)
	{
		varray3.emplace_back(Vector3(varray[i].x, altitude, varray[i].y));
	}

	//pass data on to AddCustomWall function
	return AddCustomWall(mesh, name, texture, side_texture, thickness, varray3, tw, th, side_tw, side_th, autosize);
}

Wall* PolyMesh::AddTriangleWall(MeshObject* mesh, const std::string &name, const std::string &texture, const std::string &side_texture, Real thickness, Real x1, Real y1, Real z1, Real x2, Real y2, Real z2, Real x3, Real y3, Real z3, Real tw, Real th, Real side_tw, Real side_th, bool autosize)
{
	//Adds a triangular wall with the specified dimensions
	PolyArray varray;

	//set up temporary vertex array
	varray.reserve(3);
	varray.emplace_back(Vector3(x1, y1, z1));
	varray.emplace_back(Vector3(x2, y2, z2));
	varray.emplace_back(Vector3(x3, y3, z3));

	//pass data on to AddCustomWall function
	return AddCustomWall(mesh, name, texture, side_texture, thickness, varray, tw, th, side_tw, side_th, autosize);
}

bool PolyMesh::SetWallOrientation(std::string direction)
{
	//changes internal wall orientation parameter.
	//direction can either be "left" (negative), "center" (0), or "right" (positive).
	//default on startup is 1, or center.
	//the parameter is used to determine the location of the wall's
	//x1/x2 or z1/z2 coordinates in relation to the thickness extents

	SetCase(direction, false);

	if (direction == "left")
		wall_orientation = 0;
	else if (direction == "center")
		wall_orientation = 1;
	else if (direction == "right")
		wall_orientation = 2;
	else
		return ReportError("SetWallOrientation: Invalid wall orientation");
	return true;
}

int PolyMesh::GetWallOrientation()
{
	return wall_orientation;
}

bool PolyMesh::SetFloorOrientation(std::string direction)
{
	//changes internal floor orientation parameter.
	//direction can either be "bottom" (negative), "center" (0), or "top" (positive).
	//default on startup is 2, or top.
	//the parameter is used to determine the location of the floor's
	//x1/x2 or z1/z2 coordinates in relation to the thickness extents

	SetCase(direction, false);

	if (direction == "bottom")
		floor_orientation = 0;
	else if (direction == "center")
		floor_orientation = 1;
	else if (direction == "top")
		floor_orientation = 2;
	else
		return ReportError("SetFloorOrientation: Invalid floor orientation");
	return true;
}

int PolyMesh::GetFloorOrientation()
{
	return floor_orientation;
}

void PolyMesh::DrawWalls(bool MainN, bool MainP, bool SideN, bool SideP, bool Top, bool Bottom)
{
	//sets which walls should be drawn

	//first backup old parameters
	DrawMainNOld = DrawMainN;
	DrawMainPOld = DrawMainP;
	DrawSideNOld = DrawSideN;
	DrawSidePOld = DrawSideP;
	DrawTopOld = DrawTop;
	DrawBottomOld = DrawBottom;

	//now set new parameters
	DrawMainN = MainN;
	DrawMainP = MainP;
	DrawSideN = SideN;
	DrawSideP = SideP;
	DrawTop = Top;
	DrawBottom = Bottom;
}

void PolyMesh::ResetWalls(bool ToDefaults)
{
	//if ToDefaults is true, this resets the DrawWalls data to the defaults.
	//if ToDefaults is false, this reverts the DrawWalls data to the previous settings.

	if (ToDefaults == true)
		DrawWalls(true, true, false, false, false, false);
	else
		DrawWalls(DrawMainNOld, DrawMainPOld, DrawSideNOld, DrawSidePOld, DrawTopOld, DrawBottomOld);
}

int PolyMesh::GetDrawWallsCount()
{
	//gets the number of wall polygons enabled

	int sides = 0;

	if (DrawMainN == true)
		sides++;
	if (DrawMainP == true)
		sides++;
	if (DrawSideN == true)
		sides++;
	if (DrawSideP == true)
		sides++;
	if (DrawTop == true)
		sides++;
	if (DrawBottom == true)
		sides++;

	return sides;
}

Wall* PolyMesh::AddWall(MeshObject* mesh, const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real height_in1, Real height_in2, Real altitude1, Real altitude2, Real tw, Real th)
{
	//Adds a wall with the specified dimensions, to the specified mesh object

	if (!mesh)
		return 0;

	Wall *wall = mesh->CreateWallObject(name);

	AddWallMain(wall, name, texture, thickness, x1, z1, x2, z2, height_in1, height_in2, altitude1, altitude2, tw, th, true);
	return wall;
}

Wall* PolyMesh::AddFloor(MeshObject* mesh, const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real altitude1, Real altitude2, bool reverse_axis, bool texture_direction, Real tw, Real th, bool legacy_behavior)
{
	//Adds a floor with the specified dimensions and vertical offset, to the specified mesh object

	if (!mesh)
		return 0;

	Wall *wall = mesh->CreateWallObject(name);

	AddFloorMain(wall, name, texture, thickness, x1, z1, x2, z2, altitude1, altitude2, reverse_axis, texture_direction, tw, th, true, legacy_behavior);
	return wall;
}

Wall* PolyMesh::AddGround(const std::string &name, const std::string &texture, Real x1, Real z1, Real x2, Real z2, Real altitude, int tile_x, int tile_z)
{
	//Adds ground based on a tiled-floor layout, with the specified dimensions and vertical offset
	//this does not support thickness

	Vector3 v1, v2, v3, v4;

	Real minx, minz, maxx, maxz;

	//get min and max values
	if (x1 < x2)
	{
		minx = x1;
		maxx = x2;
	}
	else
	{
		minx = x2;
		maxx = x1;
	}
	if (z1 < z2)
	{
		minz = z1;
		maxz = z2;
	}
	else
	{
		minz = z2;
		maxz = z1;
	}

	Wall *wall = sbs->GetLandscapeMesh()->CreateWallObject(name);

	Report("Creating ground...");

	//create polygon tiles
	for (Real i = minx; i < maxx; i += tile_x)
	{
		Real sizex, sizez;

		if (i + tile_x > maxx)
			sizex = maxx - i;
		else
			sizex = (Real)tile_x;

		for (Real j = minz; j < maxz; j += tile_z)
		{
			if (j + tile_z > maxz)
				sizez = maxz - i;
			else
				sizez = (Real)tile_z;

			DrawWalls(true, true, false, false, false, false);
			AddFloorMain(wall, name, texture, 0, i, j, i + sizex, j + sizez, altitude, altitude, false, false, 1, 1, false, false, false);
			ResetWalls(false);
		}
	}
	Report("Finished ground");
	return wall;
}

int PolyMesh::GetWallCount()
{
	//return total number of registered walls
	return WallCount;
}

int PolyMesh::GetPolygonCount()
{
	//return total number of registered polygons
	return PolygonCount;
}

void PolyMesh::ExtrudePolygon(PolyArray &polygon, Real thickness, PolygonSet &output_faces)
{
	//extrude the given polygon to the specified thickness
	//output_faces will contain the resulting polygons

	Real d;
	Vector3 normal = ComputeNormal(polygon, d);
	PolyArray polygon2;
	for (const auto& v : polygon)
	{
		polygon2.emplace_back(v + normal * thickness);
	}

	//add top face (original polygon)
	output_faces.emplace_back(polygon);

	//bottom face (extruded polygon) must be clockwise, so reverse the order of vertices
    PolyArray reversed(polygon2.rbegin(), polygon2.rend());
    output_faces.emplace_back(reversed);

	//exit if no side faces needed
	if (thickness == 0)
		return;

	//add side faces
	size_t n = polygon.size();
	for (size_t i = 0; i < n; ++i)
	{
		Vector3 v0 = polygon[i];
		Vector3 v1 = polygon[(i + 1) % n];
		Vector3 v2 = polygon2[(i + 1) % n];
		Vector3 v3 = polygon2[i];

		//side face as quad: v0, v1, v2, v3, reversed
		PolyArray array {v3, v2, v1, v0};
		output_faces.emplace_back(array);
	}
}

Vector2 PolyMesh::GetExtents(PolyArray &varray, int coord, bool flip_z)
{
	//returns the smallest and largest values from a specified coordinate type
	//(x, y, or z) from a vertex array (polygon).
	//first parameter must be a vertex array
	//second must be either 1 (for x), 2 (for y) or 3 (for z)

	Real esmall = 0;
	Real ebig = 0;
	Real tempnum = 0;
	size_t i = 0;

	//return 0,0 if coord value is out of range
	if (coord < 1 || coord > 3)
		return Vector2(0, 0);

	for (i = 0; i < varray.size(); i++)
	{
		if (coord == 1)
			tempnum = varray[i].x;
		if (coord == 2)
			tempnum = varray[i].y;
		if (coord == 3)
		{
			if (flip_z == false)
				tempnum = varray[i].z;
			else
				tempnum = -varray[i].z;
		}

		if (i == 0)
		{
			esmall = tempnum;
			ebig = tempnum;
		}
		else
		{
			if (tempnum < esmall)
				esmall = tempnum;
			if (tempnum > ebig)
				ebig = tempnum;
		}
	}

	return Vector2(esmall, ebig);
}

void PolyMesh::CutOrig(Wall *wall, Vector3 start, Vector3 end, bool cutwalls, bool cutfloors, int checkwallnumber, bool reset_check)
{
	//cuts a rectangular hole in the polygons within the specified range
	//this function only works on flat polygons

	if (cutwalls == false && cutfloors == false)
		return;

	//swap values if the first is greater than the second
	if (start.x > end.x)
		std::swap(start.x, end.x);
	if (start.y > end.y)
		std::swap(start.y, end.y);
	if (start.z > end.z)
		std::swap(start.z, end.z);

	bool polycheck = false;

	if (reset_check == true)
	{
		if (checkwallnumber == 1)
		{
			wall1a = false;
			wall1b = false;
		}
		if (checkwallnumber == 2)
		{
			wall2a = false;
			wall2b = false;
		}
	}

	//step through each polygon
	int polycount = wall->GetPolygonCount();
	for (int i = 0; i < polycount; i++)
	{
		//get name
		Polygon *polygon = wall->GetPolygon(i);

		PolygonSet newpolys;

		//skip empty polygons
		if (polygon->geometry.size() == 0)
			continue;

		//cut all polygons within range
		for (size_t j = 0; j < polygon->geometry.size(); j++)
		{
			//skip null geometry
			if (polygon->geometry[j].size() == 0)
				continue;

			PolyArray temppoly, temppoly2, temppoly3, temppoly4, temppoly5, worker;

			Vector2 extentsx, extentsy, extentsz;
			Ogre::AxisAlignedBox bounds (start, end);
			Ogre::AxisAlignedBox polybounds;
			bool polycheck2 = false;

			//copy source polygon vertices
			for (size_t k = 0; k < polygon->geometry[j].size(); k++)
			{
				Ogre::Vector3 vertex = sbs->ToLocal(polygon->geometry[j][k].vertex);
				temppoly.emplace_back(vertex);
				polybounds.merge(vertex);
			}

			//skip if the polygon is completely inside the bounding box
			/*if (bounds.contains(polybounds) == true)
			{
				polycheck = true;
				continue;
			}*/

			//make sure the polygon intersects bounds (is not outside the cut area)
			if (bounds.intersects(polybounds) == true)
			{
				extentsx = GetExtents(temppoly, 1);
				extentsy = GetExtents(temppoly, 2);
				extentsz = GetExtents(temppoly, 3);

				//is polygon a wall?
				if (extentsy.x != extentsy.y)
				{
					if (cutwalls == true)
					{
						//wall
						if (std::abs(extentsx.x - extentsx.y) > std::abs(extentsz.x - extentsz.y))
						{
							//wall is facing forward/backward

							//get left side
							worker = temppoly;
							SplitWithPlane(0, worker, temppoly, temppoly2, start.x);
							worker.clear();

							//get right side
							if (temppoly2.size() > 0)
								worker = temppoly2;
							else
								worker = temppoly;
							SplitWithPlane(0, worker, temppoly3, temppoly2, end.x);
							worker.clear();

							//get lower
							if (temppoly3.size() > 0)
								worker = temppoly3;
							else if (temppoly2.size() > 0)
								worker = temppoly2;
							else if (temppoly.size() > 0)
								worker = temppoly;
							SplitWithPlane(1, worker, temppoly3, temppoly4, start.y);
							worker.clear();

							//get upper
							if (temppoly4.size() > 0)
								worker = temppoly4;
							else if (temppoly3.size() > 0)
								worker = temppoly3;
							else if (temppoly2.size() > 0)
								worker = temppoly2;
							else if (temppoly.size() > 0)
								worker = temppoly;
							SplitWithPlane(1, worker, temppoly5, temppoly4, end.y);
							worker.clear();
						}
						else
						{
							//wall is facing left/right

							//get left side
							worker = temppoly;
							SplitWithPlane(2, worker, temppoly, temppoly2, start.z);
							worker.clear();

							//get right side
							if (temppoly2.size() > 0)
								worker = temppoly2;
							else
								worker = temppoly;
							SplitWithPlane(2, worker, temppoly3, temppoly2, end.z);
							worker.clear();

							//get lower
							if (temppoly3.size() > 0)
								worker = temppoly3;
							else if (temppoly2.size() > 0)
								worker = temppoly2;
							else if (temppoly.size() > 0)
								worker = temppoly;
							SplitWithPlane(1, worker, temppoly3, temppoly4, start.y);
							worker.clear();

							//get upper
							if (temppoly4.size() > 0)
								worker = temppoly4;
							else if (temppoly3.size() > 0)
								worker = temppoly3;
							else if (temppoly2.size() > 0)
								worker = temppoly2;
							else if (temppoly.size() > 0)
								worker = temppoly;
							SplitWithPlane(1, worker, temppoly5, temppoly4, end.y);
							worker.clear();
						}
						polycheck = true;
						polycheck2 = true;

						//store extents of temppoly5 for door sides if needed
						GetDoorwayExtents(wall->GetMesh(), checkwallnumber, temppoly5);
					}
				}
				else if (cutfloors == true)
				{
					//floor

					//get left side
					worker = temppoly;
					SplitWithPlane(0, worker, temppoly, temppoly2, start.x);
					worker.clear();

					//get right side
					if (temppoly2.size() > 0)
						worker = temppoly2;
					else
						worker = temppoly;
					SplitWithPlane(0, worker, temppoly3, temppoly2, end.x);
					worker.clear();

					//get lower
					if (temppoly3.size() > 0)
						worker = temppoly3;
					else if (temppoly2.size() > 0)
						worker = temppoly2;
					else if (temppoly.size() > 0)
						worker = temppoly;
					SplitWithPlane(2, worker, temppoly3, temppoly4, start.z);
					worker.clear();

					//get upper
					if (temppoly4.size() > 0)
						worker = temppoly4;
					else if (temppoly3.size() > 0)
						worker = temppoly3;
					else if (temppoly2.size() > 0)
						worker = temppoly2;
					else if (temppoly.size() > 0)
						worker = temppoly;
					SplitWithPlane(2, worker, temppoly5, temppoly4, end.z);
					worker.clear();
					temppoly5.clear();

					polycheck = true;
					polycheck2 = true;
				}

				//create split polygons
				if (polycheck2 == true)
				{
					if (temppoly.size() > 2)
					{
						newpolys.resize(newpolys.size() + 1);
						if (newpolys[newpolys.size() - 1].capacity() < temppoly.size())
							newpolys[newpolys.size() - 1].reserve(temppoly.size());
						for (size_t k = 0; k < temppoly.size(); k++)
							newpolys[newpolys.size() - 1].emplace_back(temppoly[k]);
					}
					if (temppoly2.size() > 2)
					{
						newpolys.resize(newpolys.size() + 1);
						if (newpolys[newpolys.size() - 1].capacity() < temppoly2.size())
							newpolys[newpolys.size() - 1].reserve(temppoly2.size());
						for (size_t k = 0; k < temppoly2.size(); k++)
							newpolys[newpolys.size() - 1].emplace_back(temppoly2[k]);
					}
					if (temppoly3.size() > 2)
					{
						newpolys.resize(newpolys.size() + 1);
						if (newpolys[newpolys.size() - 1].capacity() < temppoly3.size())
							newpolys[newpolys.size() - 1].reserve(temppoly3.size());
						for (size_t k = 0; k < temppoly3.size(); k++)
							newpolys[newpolys.size() - 1].emplace_back(temppoly3[k]);
					}
					if (temppoly4.size() > 2)
					{
						newpolys.resize(newpolys.size() + 1);
						if (newpolys[newpolys.size() - 1].capacity() < temppoly4.size())
							newpolys[newpolys.size() - 1].reserve(temppoly4.size());
						for (size_t k = 0; k < temppoly4.size(); k++)
							newpolys[newpolys.size() - 1].emplace_back(temppoly4[k]);
					}

					temppoly.clear();
					temppoly2.clear();
					temppoly3.clear();
					temppoly4.clear();
				}
			}
			else
			{
				//otherwise put original polygon into array (will only be used if the related submesh is recreated)
				PolyArray poly;
				for (size_t k = 0; k < polygon->geometry[j].size(); k++)
				{
					poly.emplace_back(sbs->ToLocal(polygon->geometry[j][k].vertex));
				}
				newpolys.emplace_back(poly);
			}
		}

		//create new polygon
		if (polycheck == true)
		{
			std::string oldmat;
			Vector3 oldvector;
			Matrix3 mapping;
			std::string name = polygon->GetName();

			if (newpolys.size() > 0)
			{
				//get texture data from original polygon
				oldmat = polygon->material;
				polygon->GetTextureMapping(mapping, oldvector);
			}

			//delete original polygon
			wall->DeletePolygon(i, false);
			polygon = 0;

			//create new polygon
			if (newpolys.size() > 0)
				wall->AddPolygonSet(name, oldmat, newpolys, mapping, oldvector);

			//reset search position
			i--;
			polycount--;
			polycheck = false;
		}
	}
}

void PolyMesh::Cut(Wall *wall, Vector3 start, Vector3 end, bool cutwalls, bool cutfloors, int checkwallnumber, bool reset_check)
{
	//cuts a rectangular hole in the polygons within the specified range
	//this function works on 3D shapes, and also preserves the UV texture mapping

	if (!cutwalls && !cutfloors)
		return;

	//normalize bounds
	if (start.x > end.x)
		std::swap(start.x, end.x);
	if (start.y > end.y)
		std::swap(start.y, end.y);
	if (start.z > end.z)
		std::swap(start.z, end.z);

	if (reset_check)
	{
		if (checkwallnumber == 1)
		{
			wall1a = false;
			wall1b = false;
		}
		if (checkwallnumber == 2)
		{
			wall2a = false;
			wall2b = false;
		}
	}

	const Real EPS = SMALL_EPSILON;

	auto computeNormal = [&](const GeometryArray& p)->Vector3
	{
		for (size_t i = 0; i + 2 < p.size(); ++i)
		{
			Vector3 n = (p[i + 1].vertex - p[i].vertex).crossProduct(p[i + 2].vertex - p[i + 1].vertex);
			if (n.squaredLength() > EPS * EPS)
				return n.normalisedCopy();
		}
		return Vector3(0, 1, 0);
	};

	auto extentsAxis = [](const GeometryArray& p, int axis)->Vector2
	{
		Real mn = std::numeric_limits<Real>::infinity();
		Real mx = -mn;
		for (const auto& v : p)
		{
			Real c = (axis == 0) ? v.vertex.x : (axis == 1) ? v.vertex.y : v.vertex.z;
			if (c < mn)
				mn = c;
			if (c > mx)
				mx = c;
		}
		return Vector2(mn, mx);
	};

	//2D early-outs for floors (XZ)
	auto outside_all_floor_strict = [&](const GeometryArray& p)->bool
	{
		bool xl = true, xr = true, zb = true, zf = true;
		for (const auto& v: p)
		{
			const Real x = v.vertex.x, z = v.vertex.z;
			xl &= (x <= start.x - EPS);
			xr &= (x >= end.x + EPS);
			zb &= (z <= start.z - EPS);
			zf &= (z >= end.z + EPS);
		}
		return xl || xr || zb || zf;
	};
	auto outside_all_floor_relaxed = [&](const GeometryArray& p)->bool
	{
		bool xl = true, xr = true, zb = true, zf = true;
		for (const auto& v: p)
		{
			const Real x = v.vertex.x, z = v.vertex.z;
			xl &= (x <= start.x + EPS);
			xr &= (x >= end.x - EPS);
			zb &= (z <= start.z + EPS);
			zf &= (z >= end.z - EPS);
		}
		return xl || xr || zb || zf;
	};

	auto inside_all_floor_strict = [&](const GeometryArray& p)->bool
	{
		for (const auto& v: p)
		{
			const Real x = v.vertex.x, z = v.vertex.z;
			if (x < start.x + EPS || x > end.x - EPS ||
				z < start.z + EPS || z > end.z - EPS)
				return false;
		}
		return true;
	};

	auto inside_all_floor_relaxed = [&](const GeometryArray& p)->bool
	{
		for (const auto& v: p)
		{
			const Real x = v.vertex.x, z = v.vertex.z;
			if (x < start.x - EPS || x > end.x + EPS ||
				z < start.z - EPS || z > end.z + EPS)
				return false;
		}
		return true;
	};

	//3D early-outs for walls (use full AABB)
	auto outside_all_wall_strict = [&](const GeometryArray& p)->bool
	{
		bool xl = true, xr = true, yl = true, yh = true, zb = true, zf = true;
		for (const auto& v: p)
		{
			const Real x = v.vertex.x, y = v.vertex.y, z = v.vertex.z;
			xl &= (x <= start.x - EPS);
			xr &= (x >= end.x + EPS);
			yl &= (y <= start.y - EPS);
			yh &= (y >= end.y + EPS);
			zb &= (z <= start.z - EPS);
			zf &= (z >= end.z + EPS);
		}
		return xl || xr || yl || yh || zb || zf;
	};

	auto inside_all_wall_strict = [&](const GeometryArray& p)->bool
	{
		for (const auto& v: p)
		{
			const Real x = v.vertex.x, y = v.vertex.y, z = v.vertex.z;
			if (x < start.x + EPS || x > end.x - EPS ||
				y < start.y + EPS || y > end.y - EPS ||
				z < start.z + EPS || z > end.z - EPS)
				return false;
		}
		return true;
	};

	//ring XZ bbox + Y thickness
	auto ring_metrics = [&](const GeometryArray& p)
	{
		Vector2 mnXZ( std::numeric_limits<Real>::infinity()), mxXZ(-std::numeric_limits<Real>::infinity());
		Real yMin =  std::numeric_limits<Real>::infinity();
		Real yMax = -std::numeric_limits<Real>::infinity();
		for (const auto& g : p)
		{
			mnXZ.x = std::min(mnXZ.x, g.vertex.x);
			mxXZ.x = std::max(mxXZ.x, g.vertex.x);
			mnXZ.y = std::min(mnXZ.y, g.vertex.z);
			mxXZ.y = std::max(mxXZ.y, g.vertex.z);
			yMin   = std::min(yMin, g.vertex.y);
			yMax   = std::max(yMax, g.vertex.y);
		}
		return std::tuple<Vector2,Vector2,Real>(mnXZ, mxXZ, (yMax - yMin));
	};

	//small heuristic: likely a terrain/ground tile?
	auto likely_terrain = [&](const GeometryArray& p)->bool
	{
		auto [mn,mx,yt] = ring_metrics(p);
		const Real dx = mx.x - mn.x;
		const Real dz = mx.y - mn.y;
		const Real extent = std::max(dx, dz);
		const Real Y_FLAT = SMALL_EPSILON * 64;
		//probably terrain/landscape
		return (extent >= 50.0f && yt <= Y_FLAT);
	};

	//axis-aligned bbox of a ring
	auto ring_aabb = [&](const GeometryArray& p)
	{
		Vector3 mn(  std::numeric_limits<Real>::infinity());
		Vector3 mx(- std::numeric_limits<Real>::infinity());
		for (const auto& g : p)
		{
			mn.x = std::min(mn.x, g.vertex.x); mx.x = std::max(mx.x, g.vertex.x);
			mn.y = std::min(mn.y, g.vertex.y); mx.y = std::max(mx.y, g.vertex.y);
			mn.z = std::min(mn.z, g.vertex.z); mx.z = std::max(mx.z, g.vertex.z);
		}
		return std::pair<Vector3,Vector3>(mn, mx);
	};

	//1D interval overlap with tolerance
	auto overlaps1D = [&](Real mn, Real mx, Real a, Real b, Real eps)->bool
	{
		if (a > b)
			std::swap(a, b);
		// true if [mn,mx] intersects [a,b] allowing boundary contact within eps
		return !(mx < a - eps || mn > b + eps);
	};

	//full 3D AABB vs cut-box overlap
	auto overlaps_cut_volume = [&](const GeometryArray& ring, const Vector3& start, const Vector3& end, Real eps)->bool
	{
		auto [mn, mx] = ring_aabb(ring);
		return overlaps1D(mn.x, mx.x, start.x, end.x, eps) &&
			   overlaps1D(mn.y, mx.y, start.y, end.y, eps) &&
			   overlaps1D(mn.z, mx.z, start.z, end.z, eps);
	};

	int polycount = wall->GetPolygonCount();
	for (int i = 0; i < polycount; ++i)
	{
		Polygon* polygon = wall->GetPolygon(i);
		if (!polygon)
			continue;

		//skip empty poly
		if (polygon->geometry.empty())
			continue;

		GeometrySet rebuilt;
		bool touchedAny = false;

		for (size_t j = 0; j < polygon->geometry.size(); ++j)
		{
			const auto& src = polygon->geometry[j];
			if (src.empty())
				continue;

			//build local-space geometry ring
			GeometryArray ring;
			ring.reserve(src.size());
			for (const auto& g : src)
			{
				Geometry v;
				v.vertex = sbs->ToLocal(g.vertex);
				v.texel  = g.texel;
				v.normal = g.normal;
				ring.emplace_back(v);
			}

			//classify by normal: floor if near-horizontal
			Vector3 nrm = computeNormal(ring);
			const bool isFloor = (std::abs(nrm.y) > Real(0.70710678)); // > ~45° from vertical
			const bool isWall  = !isFloor;

			//respect cut flags
			if ((isWall  && !cutwalls) || (isFloor && !cutfloors))
			{
				rebuilt.emplace_back(ring);
				continue;
			}

			//skip if the ring's bbox doesn't intersect the cut volume at all
			if (!overlaps_cut_volume(ring, start, end, EPS))
			{
				rebuilt.emplace_back(ring);
				continue;
			}

			//early-outs per orientation
			if (isFloor)
			{
				if (outside_all_floor_strict(ring))
				{
					rebuilt.emplace_back(ring);
					continue;
				}
				if (inside_all_floor_strict(ring))
				{
					touchedAny = true;
					continue;
				}

				if (likely_terrain(ring))
				{
					if (outside_all_floor_relaxed(ring))
					{
						rebuilt.emplace_back(ring);
						continue;
					}
					if (inside_all_floor_relaxed(ring))
					{
						touchedAny = true;
						continue;
					}
				}
			}
			else
			{
				if (outside_all_wall_strict(ring))
				{
					rebuilt.emplace_back(ring);
					continue;
				}
				if (inside_all_wall_strict(ring))
				{
					touchedAny = true;
					continue;
				}
			}

			// 6-plane cut procedure

			// planes that define the inside of the cut box
			struct Plane { int axis; Real value; bool keepGE; };
			// inside predicates (order doesn’t matter)
			Plane planes[6] = {
				{0, start.x, true },  // x >= start.x
				{0, end.x,   false},  // x <= end.x
				{2, start.z, true },  // z >= start.z
				{2, end.z,   false},  // z <= end.z
				{1, start.y, true },  // y >= start.y
				{1, end.y,   false}   // y <= end.y
			};

			// seeds start with the original ring (local space)
			std::vector<GeometryArray> seeds;
			seeds.push_back(ring);

			// where we accumulate final outside pieces
			std::vector<GeometryArray> pieces;
			pieces.reserve(8);

			GeometryArray le, ge;

			for (int p = 0; p < 6; ++p)
			{
				const Plane& P = planes[p];
				std::vector<GeometryArray> next;
				next.reserve(seeds.size());

				for (auto& poly : seeds)
				{
					if (poly.size() < 3)
						continue;

					le.clear();
					ge.clear();

					//bias toward the *inside* side so shared boundaries go to inside
					SplitWithPlaneUV(P.axis, poly, le, ge, P.value, /*bias=*/P.keepGE);

					//inside satisfies the plane’s inside predicate; outside fails it
					GeometryArray& inside  = P.keepGE ? ge : le;
					GeometryArray& outside = P.keepGE ? le : ge;

					if (outside.size() >= 3)
						pieces.emplace_back(outside);   // emit once and done for this branch

					if (inside.size() >= 3)
						next.emplace_back(inside);      // continue trimming inside through remaining planes
				}

				seeds.swap(next);
			}

			//'pieces' holds all disjoint fragments strictly outside the AABB (to keep)
			//'seeds' holds zero or more inside fragments (the hole)
			if (!seeds.empty())
			{
				// choose the largest inside fragment (or first) for doorway extents
				const auto& insidePoly = seeds.front();
				if (!insidePoly.empty())
				{
					PolyArray hole; hole.reserve(insidePoly.size());
					for (const auto& v : insidePoly) hole.emplace_back(v.vertex);
						GetDoorwayExtents(wall->GetMesh(), checkwallnumber, hole);
				}
			}

			// add kept outside pieces back
			if (!pieces.empty())
			{
				for (auto& p : pieces)
					rebuilt.emplace_back(p);
				touchedAny = true;
			}
			else
			{
				// No outside survived - ring was fully inside the box: it’s removed
				touchedAny = true;
			}
		}

		if (touchedAny)
		{
			//get material and mapping from the original polygon
			const std::string name = polygon->GetName();
			const std::string oldmat = polygon->material;

			//delete original polygon
			wall->DeletePolygon(i, false);
			--i;
			--polycount;

			//add new polygon set to replace original
			if (!rebuilt.empty())
				wall->AddPolygonSet(name, oldmat, rebuilt);
		}
	}
}

void PolyMesh::GetDoorwayExtents(MeshObject *mesh, int checknumber, PolyArray &polygon)
{
	//calculate doorway extents, for use with AddDoorwayWalls function
	//checknumber is 1 when checking shaft walls, and 2 when checking floor walls

	if (checknumber > 0 && checknumber < 3)
	{
		Vector3 mesh_position = mesh->GetPosition();
		Real extent;

		if (wall2a == false || wall2b == false)
		{
			if (checknumber == 2)
			{
				//level walls
				if (wall2a == true)
					wall2b = true;
				wall2a = true;
				extent = GetExtents(polygon, 1).x + mesh_position.x;
				if (wall2b == false || (wall2b == true && std::abs(extent - mesh_position.x) > std::abs(wall_extents_x.x - mesh_position.x)))
					wall_extents_x.x = extent;
				extent = GetExtents(polygon, 3).x + mesh_position.z;
				if (wall2b == false || (wall2b == true && std::abs(extent - mesh_position.z) > std::abs(wall_extents_z.x - mesh_position.z)))
					wall_extents_z.x = extent;
				wall_extents_y = GetExtents(polygon, 2) + mesh_position.y;
			}
			else
			{
				//shaft walls
				if (wall1a == true)
					wall1b = true;
				wall1a = true;
				extent = GetExtents(polygon, 1).y + mesh_position.x;
				if (wall1b == false || (wall1b == true && std::abs(extent - mesh_position.x) < std::abs(wall_extents_x.y - mesh_position.x)))
					wall_extents_x.y = extent;
				extent = GetExtents(polygon, 3).y + mesh_position.z;
				if (wall1b == false || (wall1b == true && std::abs(extent - mesh_position.z) < std::abs(wall_extents_z.y - mesh_position.z)))
					wall_extents_z.y = extent;
			}
		}
	}
}

Vector3 PolyMesh::GetPolygonDirection(PolyArray &polygon)
{
	//returns the direction the polygon faces, in a 3D vector
	//for example, <-1, 0, 0> means that the wall faces left.

	//get largest normal

	//convert to remote values for precision compatibility with Alpha 7 and earlier
	PolyArray newpoly;
	for (size_t i = 0; i < polygon.size(); i++)
		newpoly.emplace_back(sbs->ToRemote(polygon[i], true, false));

	Vector3 normal = ComputePlane(newpoly, false).normal;

	int largest_normal = 0; //x

	if (std::abs(normal.y) >= std::abs(normal.x) && std::abs(normal.y) >= std::abs(normal.z))
		largest_normal = 1; //y biggest
	else if (std::abs(normal.z) > std::abs(normal.x))
		largest_normal = 2; //z biggest

	if (largest_normal == 0)
	{
		if (normal.x < 0)
			return Vector3(1, 0, 0);
		else
			return Vector3(-1, 0, 0);
	}

	if (largest_normal == 1)
	{
		if (normal.y < 0)
			return Vector3(0, 1, 0);
		else
			return Vector3(0, -1, 0);
	}

	if (largest_normal == 2)
	{
		if (normal.z < 0)
			return Vector3(0, 0, 1);
		else
			return Vector3(0, 0, -1);
	}
	return Vector3(0, 0, 0);
}

Vector2 PolyMesh::GetEndPoint(const Vector2 &StartPoint, Real angle, Real distance)
{
	//calculate 2D endpoint from given starting point, along with angle and distance (magnitude)
	//angle is in degrees

	angle -= 90;
	Real newangle = DegreesToRadians(angle);
	Real x = StartPoint.x + distance * cos(newangle);
	Real y = -StartPoint.y + distance * sin(newangle);
	Vector2 result (x, -y);
	return result;

}

Plane PolyMesh::ComputePlane(PolyArray &vertices, bool flip_normal)
{
	//compute plane from a set of given vertices

	Real det;
	Vector3 normal;
	if (flip_normal == true)
		normal = -ComputeNormal(vertices, det);
	else
		normal = ComputeNormal(vertices, det);
	normal.normalise();
	return Plane(normal, det);
}

void PolyMesh::ResetDoorwayWalls()
{
	wall1a = false;
	wall1b = false;
	wall2a = false;
	wall2b = false;
	wall_extents_x = Vector2::ZERO;
	wall_extents_y = Vector2::ZERO;
	wall_extents_z = Vector2::ZERO;
}

Wall* PolyMesh::AddDoorwayWalls(MeshObject* mesh, const std::string &wallname, const std::string &texture, Real tw, Real th)
{
	//add joining doorway polygons if needed

	if (!mesh)
		return 0;

	if (wall1a == true && wall2a == true)
	{
		Wall *wall = mesh->CreateWallObject(wallname);

		//convert extents to relative positioning
		Vector2 extents_x = wall_extents_x - wall->GetMesh()->GetPosition().x;
		Vector2 extents_y = wall_extents_y - wall->GetMesh()->GetPosition().y;
		Vector2 extents_z = wall_extents_z - wall->GetMesh()->GetPosition().z;

		//true if doorway is facing forward/backward
		//false if doorway is facing left/right
		bool direction = std::abs(extents_x.x - extents_x.y) > std::abs(extents_z.x - extents_z.y);

		PolyMesh* polymesh = sbs->GetPolyMesh();

		polymesh->DrawWalls(false, true, false, false, false, false);
		if (direction == true)
			polymesh->AddWallMain(wall, "DoorwayLeft", texture, 0, extents_x.x, extents_z.x, extents_x.x, extents_z.y, extents_y.y - extents_y.x, extents_y.y - extents_y.x, extents_y.x, extents_y.x, tw, th, true);
		else
			polymesh->AddWallMain(wall, "DoorwayLeft", texture, 0, extents_x.x, extents_z.x, extents_x.y, extents_z.x, extents_y.y - extents_y.x, extents_y.y - extents_y.x, extents_y.x, extents_y.x, tw, th, true);
		polymesh->ResetWalls();

		polymesh->DrawWalls(true, false, false, false, false, false);
		if (direction == true)
			polymesh->AddWallMain(wall, "DoorwayRight", texture, 0, extents_x.y, extents_z.x, extents_x.y, extents_z.y, extents_y.y - extents_y.x, extents_y.y - extents_y.x, extents_y.x, extents_y.x, tw, th, true);
		else
			polymesh->AddWallMain(wall, "DoorwayRight", texture, 0, extents_x.x, extents_z.y, extents_x.y, extents_z.y, extents_y.y - extents_y.x, extents_y.y - extents_y.x, extents_y.x, extents_y.x, tw, th, true);

		polymesh->AddFloorMain(wall, "DoorwayTop", texture, 0, extents_x.x, extents_z.x, extents_x.y, extents_z.y, extents_y.y, extents_y.y, false, false, tw, th, true);
		polymesh->ResetWalls();

		ResetDoorwayWalls();

		return wall;
	}

	return 0;
}

Vector3 PolyMesh::ComputeNormal2(const PolyArray &vertices, Real &D)
{
    //calculate the normal of a given polygon

    Vector3 normal(0, 0, 0);
    size_t n = vertices.size();
    if (n < 3)
        return normal; //not a polygon

    for (size_t i = 0; i < n; ++i)
    {
        const Vector3 &current = vertices[i];
        const Vector3 &next = vertices[(i + 1) % n];
        normal.x += (current.y - next.y) * (current.z + next.z);
        normal.y += (current.z - next.z) * (current.x + next.x);
        normal.z += (current.x - next.x) * (current.y + next.y);
    }

    Real length = normal.length();
    if (length < SMALL_EPSILON)
        normal = Vector3(0, 0, 1); //default normal if degenerate
    else
        normal /= length;

    //plane equation: Ax + By + Cz + D = 0, solve for D
    D = -normal.x * vertices[0].x - normal.y * vertices[0].y - normal.z * vertices[0].z;

	//return flipped normal
    return -normal;
}

void PolyMesh::SplitWithPlane(int axis, const PolyArray &orig, PolyArray &poly1, PolyArray &poly2, Real value)
{
	//splits the given polygon into two polygons, on the desired plane (defined by the axis and value parameters)
	//axis is 0 for X, 1 for Y, 2 for Z

	//clear output polygons
	poly1.clear();
	poly2.clear();

	//helper lambda to get coordinate by axis
	auto getCoord = [axis](const Vector3& v) -> Real {
		if (axis == 0) return v.x;
		if (axis == 1) return v.y;
		return v.z;
	};

	size_t n = orig.size();
	if (n < 2)
		return; //not enough vertices

	//preallocate memory for a worst-case scenario
	poly1.reserve(orig.size());
	poly2.reserve(orig.size());

	Vector3 prev = orig[n - 1];
	Real prevSide = getCoord(prev) - value;
	if (std::abs(prevSide) < SMALL_EPSILON)
		prevSide = 0;

	for (size_t i = 0; i < n; ++i)
	{
		Vector3 curr = orig[i];
		Real currSide = getCoord(curr) - value;
		if (std::abs(currSide) < SMALL_EPSILON)
			currSide = 0;

		//if edge crosses the plane, compute intersection
		if ((prevSide < 0 && currSide > 0) || (prevSide > 0 && currSide < 0))
		{
			Real t = prevSide / (prevSide - currSide);
			Vector3 intersect = prev + (curr - prev) * t;
			poly1.emplace_back(intersect);
			poly2.emplace_back(intersect);
		}

		//add current vertex to appropriate polygon(s)
		if (currSide >= 0)
			poly2.emplace_back(curr);
		if (currSide <= 0)
			poly1.emplace_back(curr);

		prev = curr;
		prevSide = currSide;
	}
}

void PolyMesh::SplitWithPlaneUV(int axis, const GeometryArray &orig, GeometryArray &polyLE, GeometryArray &polyGE, Real value, bool bias /*true => boundary to GE*/)
{
	//splits the given polygon into two polygons, on the desired plane (defined by the axis and value parameters)
	//axis is 0 for X, 1 for Y, 2 for Z

	polyLE.clear();
	polyGE.clear();

	// adaptive EPS from polygon scale
	auto bboxOf = [&](const GeometryArray& g)
	{
		Vector3 mn( std::numeric_limits<Real>::max(),
					std::numeric_limits<Real>::max(),
					std::numeric_limits<Real>::max());
		Vector3 mx(-std::numeric_limits<Real>::max(),
					-std::numeric_limits<Real>::max(),
					-std::numeric_limits<Real>::max());
		for (const auto& it : g)
		{
			mn.makeFloor(it.vertex);
			mx.makeCeil(it.vertex);
		}
		return std::make_pair(mn, mx);
	};
	auto [mnBox, mxBox] = bboxOf(orig);
	Real scale = std::max({ mxBox.x - mnBox.x, mxBox.y - mnBox.y, mxBox.z - mnBox.z, Real(1) });
	const Real EPS = std::max<Real>(SMALL_EPSILON, std::max<Real>(Real(1e-6) * scale, Real(1e-7)));

	auto coord = [&](const Vector3& v)->Real
	{
		return (axis == 0) ? v.x : (axis == 1) ? v.y : v.z;
	};
	auto dist = [&](const Vector3& v)->Real
	{
		return coord(v) - value;
	};

	auto lerpGeom = [](const Geometry& a, const Geometry& b, Real t)->Geometry
	{
		Geometry r;
		r.vertex = a.vertex + (b.vertex - a.vertex) * t;
		r.texel  = a.texel  + (b.texel  - a.texel ) * t;
		r.normal = a.normal + (b.normal - a.normal) * t;
		return r;
	};

	auto pushUnique = [&](GeometryArray& poly, const Geometry& g)
	{
		if (!poly.empty())
		{
			const Vector3& p = poly.back().vertex;
			const Real d2 = (g.vertex - p).squaredLength();
			const Real tol  = std::max<Real>(EPS * Real(0.25), Real(1e-9)); // tight de-dup
			if (d2 <= tol * tol)
				return;
		}
		poly.emplace_back(g);
	};

	auto sanitize = [&](GeometryArray& poly)
	{
		if (poly.size() >= 2 && (poly.front().vertex - poly.back().vertex).squaredLength() <= (EPS * EPS))
			poly.pop_back();
		if (poly.size() < 3)
			poly.clear();
	};

	// coplanar special case
	// If all vertices are on the plane (|d|<=EPS), route the *entire* ring to one side.
	bool hasPos = false, hasNeg = false, hasOn = false;
	for (const auto& g : orig)
	{
		Real d = dist(g.vertex);
		if (d >  EPS)
			hasPos = true;
		else if (d < -EPS)
			hasNeg = true;
		else
			hasOn = true;
		if (hasPos && hasNeg)
			break; // not coplanar
	}

	if (!hasPos && !hasNeg && hasOn)
	{
		// The whole ring lies on the plane.
		// Give ownership to the biased side, leave the other empty.
		GeometryArray* target = bias ? &polyGE : &polyLE;
		target->reserve(orig.size());
		for (const auto& g : orig)
		{
			pushUnique(*target, g);
		}
		sanitize(*target);
		return;
	}

	auto clipOne = [&](bool keepGE, GeometryArray& out)
	{
		out.clear();
		const size_t n = orig.size();
		if (n < 3)
			return;

		auto inside = [&](Real d)->bool
		{
			// Inclusive: GE uses d >= -EPS; LE uses d <= +EPS
			return keepGE ? (d >= -EPS) : (d <= +EPS);
		};

		Geometry A = orig[n - 1];
		Real dA = dist(A.vertex);
		bool inA = inside(dA);

		for (size_t i = 0; i < n; ++i)
		{
			Geometry B = orig[i];
			Real dB = dist(B.vertex);
			bool inB = inside(dB);

			if (inA && inB)
			{
				// I -> I
				pushUnique(out, B);
			}
			else if (inA && !inB)
			{
				// I -> O : add intersection
				Real denom = (dA - dB);
				Real t = (denom != 0) ? (dA / (dA - dB)) : Real(0.5);
				t = std::clamp(t, Real(0), Real(1));
				pushUnique(out, lerpGeom(A, B, t));
			}
			else if (!inA && inB)
			{
				// O -> I : add intersection, then B
				Real denom = (dA - dB);
				Real t = (denom != 0) ? (dA / (dA - dB)) : Real(0.5);
				t = std::clamp(t, Real(0), Real(1));
				pushUnique(out, lerpGeom(A, B, t));
				pushUnique(out, B);
			}

			// O -> O : add nothing

			A = B; dA = dB; inA = inB;
		}

		sanitize(out);
	};

	//compute both sides independently
	clipOne(/*keepGE=*/false, polyLE); // coord <= value (+EPS)
	clipOne(/*keepGE=*/true,  polyGE); // coord >= value (-EPS)
}

// Two-sided, double-precision Möller–Trumbore
bool PolyMesh::IntersectRayTri(const Vector3& ro, const Vector3& rd, const Vector3& a, const Vector3& b, const Vector3& c, double& t, double& u, double& v)
{
    const Vector3 ab = b - a;
    const Vector3 ac = c - a;
    const Vector3 p  = rd.crossProduct(ac);
    const Real det = static_cast<double>(ab.dotProduct(p));

    //two-sided: if nearly zero, the ray is parallel
    if (std::abs(det) < 1e-12)
		return false;

    const Real invDet = 1.0 / det;
    const Vector3 s = ro - a;
    u = static_cast<Real>(s.dotProduct(p)) * invDet;
    if (u < -1e-8 || u > 1.0 + 1e-8)
		return false;

    const Vector3 q = s.crossProduct(ab);
    v = static_cast<Real>(rd.dotProduct(q)) * invDet;
    if (v < -1e-8 || (u + v) > 1.0 + 1e-8)
		return false;

    t = static_cast<Real>(ac.dotProduct(q)) * invDet;
		return t > 1e-8;
}

MeshObject::TriOwner PolyMesh::FindWallIntersect_Tri(MeshObject* mesh, const Vector3& start, const Vector3& end, Vector3& isect, Real& distance, Vector3& normal)
{
    const Vector3 ro = start;
    const Vector3 rd = (end - start).normalisedCopy();
    Real bestT = std::numeric_limits<double>::infinity();
    int bestTri = -1;

    const auto& pos = mesh->pickPositions;
    const auto& idx = mesh->pickIndices;

    for (size_t i = 0; i < idx.size(); i += 3)
	{
        const Vector3& a = pos[idx[i + 0]];
        const Vector3& b = pos[idx[i + 1]];
        const Vector3& c = pos[idx[i + 2]];
        Real t, u, v;
        if (IntersectRayTri(ro, rd, a,b,c, t,u,v))
		{
            if (t < bestT)
			{
                bestT = t;
                bestTri = static_cast<int>(i / 3);
                // compute normal in the same space
                normal = (b - a).crossProduct(c - a).normalisedCopy();
                // Two-sided: make normal face the ray (optional)
                if (normal.dotProduct(rd) > 0)
					normal = -normal;
            }
        }
    }

	MeshObject::TriOwner owner;
	owner.poly = 0;
	owner.wall = 0;
    if (bestTri < 0)
		return owner;

    isect = ro + rd * static_cast<Real>(bestT);
    distance = (isect - ro).length();
    return mesh->triOwners[bestTri]; // deterministic ownership
}

}
