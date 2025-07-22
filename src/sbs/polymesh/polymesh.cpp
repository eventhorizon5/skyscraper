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
#include "texture.h"
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
}

PolyMesh::~PolyMesh()
{
}

Wall* PolyMesh::FindWallIntersect(MeshObject *mesh, const Vector3 &start, const Vector3 &end, Vector3 &isect, Real &distance, Vector3 &normal, Wall *wall)
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
		if (!mesh->Walls[i])
			continue;

		if (wall && mesh->Walls[i] != wall)
			continue;

		for (int j = 0; j < mesh->Walls[i]->GetPolygonCount(); j++)
		{
			Polygon *poly = mesh->Walls[i]->GetPolygon(j);
			if (!poly)
				continue;

			if (poly->IntersectSegment(start, end, cur_isect, &pr, tmpnormal) == true)
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

bool PolyMesh::CreateMesh(MeshObject *mesh, const std::string &name, const std::string &texture, PolyArray &vertices, Real tw, Real th, bool autosize, Matrix3 &t_matrix, Vector3 &t_vector, std::vector<std::vector<Polygon::Geometry> > &geometry, std::vector<Triangle> &triangles, PolygonSet &converted_vertices)
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

	return CreateMesh(mesh, name, material, converted_vertices, t_matrix, t_vector, geometry, triangles, converted_vertices, tw2, th2, false);
}

bool PolyMesh::CreateMesh(MeshObject *mesh, const std::string &name, const std::string &material, PolygonSet &vertices, Matrix3 &tex_matrix, Vector3 &tex_vector, std::vector<std::vector<Polygon::Geometry> > &geometry, std::vector<Triangle> &triangles, PolygonSet &converted_vertices, Real tw, Real th, bool convert_vertices)
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
			Vector3 normal = sbs->GetUtility()->ComputePlane(converted_vertices[i], false).normal;

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

void PolyMesh::AddPolygon(Wall* wallobject, const std::string &texture, PolyArray &varray, Real tw, Real th, bool report)
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
	Vector3 direction = sbs->GetUtility()->GetPolygonDirection(varray1);

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

	//add the polygons
	if (DrawMainN == true)
	{
		std::string NewName = name;
		if (DrawMainP == true)
			NewName.append(":0");
		std::string final_texture = texture;
		if (texturemanager->TextureOverride == true)
			final_texture = texturemanager->mainnegtex;
		wallobject->AddPolygon(NewName, final_texture, varray1, tw, th, true);
	}
	if (DrawMainP == true)
	{
		std::string NewName = name;
		if (DrawMainN == true)
			NewName.append(":1");
		std::string final_texture = texture;
		if (texturemanager->TextureOverride == true)
			final_texture = texturemanager->mainpostex;
		wallobject->AddPolygon(NewName, final_texture, varray2, tw, th, true);
	}
}

Wall* PolyMesh::AddCustomWall(MeshObject* mesh, const std::string &name, const std::string &texture, PolyArray &varray, Real tw, Real th)
{
	//Adds a wall from a specified array of 3D vectors

	if (!mesh)
		return 0;

	//create wall object
	Wall *wall = mesh->CreateWallObject(name);

	//create polygon in wall object
	AddPolygon(wall, texture, varray, tw, th);

	return wall;
}

Wall* PolyMesh::AddCustomFloor(MeshObject* mesh, const std::string &name, const std::string &texture, std::vector<Vector2> &varray, Real altitude, Real tw, Real th)
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
	return AddCustomWall(mesh, name, texture, varray3, tw, th);
}

Wall* PolyMesh::AddTriangleWall(MeshObject* mesh, const std::string &name, const std::string &texture, Real x1, Real y1, Real z1, Real x2, Real y2, Real z2, Real x3, Real y3, Real z3, Real tw, Real th)
{
	//Adds a triangular wall with the specified dimensions
	PolyArray varray;

	//set up temporary vertex array
	varray.reserve(3);
	varray.emplace_back(Vector3(x1, y1, z1));
	varray.emplace_back(Vector3(x2, y2, z2));
	varray.emplace_back(Vector3(x3, y3, z3));

	//pass data on to AddCustomWall function
	return AddCustomWall(mesh, name, texture, varray, tw, th);
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

}
