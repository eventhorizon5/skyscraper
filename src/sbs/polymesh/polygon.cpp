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

#include "globals.h"
#include "sbs.h"
#include "polymesh.h"
#include "dynamicmesh.h"
#include "triangle.h"
#include "mesh.h"
#include "polymesh.h"
#include "texman.h"
#include "utility.h"
#include "polygon.h"

namespace SBS {

Polygon::Polygon(Object *parent, const std::string &name, MeshObject *meshwrapper) : ObjectBase(parent)
{
	mesh = meshwrapper;
	t_matrix = Matrix3::ZERO;
	t_vector = Vector3::ZERO;
	SetName(name);
	size = 0;

	sbs->GetPolyMesh()->PolygonCount++;
}

Polygon::~Polygon()
{
	if (material != "" && sbs->FastDelete == false)
		sbs->GetTextureManager()->DecrementTextureUsage(material);

	mesh->ResetPrepare();

	sbs->GetPolyMesh()->PolygonCount--;
}

void Polygon::Create(GeometrySet &geometry, std::vector<Triangle> &triangles, Matrix3 &tex_matrix, Vector3 &tex_vector, const std::string &material, Plane &plane)
{
	//create a polygon

	t_matrix = tex_matrix;
	t_vector = tex_vector;
	this->material = material;
	this->plane = plane;
	this->geometry = geometry;
	this->triangles = triangles;
	size = triangles.size() * (sizeof(unsigned int) * 3);

	vertex_count = 0;
	for (size_t i = 0; i < geometry.size(); i++)
	{
		vertex_count += geometry[i].size();
		size += geometry[i].size() * sizeof(Geometry);
	}

	mesh->ResetPrepare();

	//register texture usage
	if (material != "")
		sbs->GetTextureManager()->IncrementTextureUsage(material);
}

void Polygon::Create(std::vector<Geometry> &geometry, std::vector<Triangle> &triangles, Matrix3 &tex_matrix, Vector3 &tex_vector, const std::string &material, Plane &plane)
{
	t_matrix = tex_matrix;
	t_vector = tex_vector;
	this->material = material;
	this->plane = plane;
	this->geometry.emplace_back(geometry);
	this->triangles = triangles;
	size = triangles.size() * (sizeof(unsigned int) * 3);

	vertex_count = geometry.size();
	size = sizeof(Geometry) * vertex_count;

	mesh->ResetPrepare();

	//register texture usage
	if (material != "")
		sbs->GetTextureManager()->IncrementTextureUsage(material);
}

void Polygon::GetTextureMapping(Matrix3 &tm, Vector3 &tv)
{
	//return texture mapping matrix and vector
	tm = t_matrix;
	tv = t_vector;
}

void Polygon::Move(const Vector3 &vector, Real speed)
{
	bool dynamic = mesh->UsingDynamicBuffers();

	for (size_t i = 0; i < geometry.size(); i++)
	{
		for (size_t j = 0; j < geometry[i].size(); j++)
		{
			Geometry &data = geometry[i][j];
			data.vertex += sbs->ToRemote(vector * speed);
		}
	}

	//update vertices in render buffer, if using dynamic buffers
	if (dynamic == true)
		mesh->MeshWrapper->UpdateVertices(mesh, material, this, true);
}

Plane Polygon::GetAbsolutePlane()
{
	//convert to an absolute plane
	Plane plane2(plane.normal, sbs->ToRemote(mesh->GetPosition()));
	return Plane(plane.normal, -(plane.d + plane2.d));
}

Vector2 Polygon::GetExtents(int coord)
{
	//returns the extents of a polygon
	//coord must be either 1 (for x), 2 (for y) or 3 (for z)

	//return 0,0 if coord value is out of range
	if (coord < 1 || coord > 3)
		return Vector2(0, 0);

	//get polygon extents
	PolyArray poly;
	for (size_t i = 0; i < geometry.size(); i++)
	{
		for (size_t j = 0; j < geometry[i].size(); j++)
		{
			poly.emplace_back(geometry[i][j].vertex);
		}
	}

	Vector2 extents = sbs->GetPolyMesh()->GetExtents(poly, coord);

	return extents;
}

void Polygon::ChangeHeight(Real newheight)
{
	bool dynamic = mesh->UsingDynamicBuffers();

	Vector2 extents = GetExtents(2);

	for (size_t i = 0; i < geometry.size(); i++)
	{
		for (size_t j = 0; j < geometry[i].size(); j++)
		{
			Geometry &data = geometry[i][j];
			if (data.vertex.y == extents.y)
				data.vertex.y = sbs->ToRemote(newheight);
		}
	}

	//update vertices in render buffer, if using dynamic buffers
	if (dynamic == true)
		mesh->MeshWrapper->UpdateVertices(mesh, material, this, true);
}

bool Polygon::ReplaceTexture(const std::string &oldtexture, const std::string &newtexture)
{
	//replace material named oldtexture with newtexture

	if (oldtexture == newtexture)
		return false;

	if (material == oldtexture)
	{
		std::string old = material;
		sbs->GetTextureManager()->DecrementTextureUsage(oldtexture);
		material = newtexture;
		sbs->GetTextureManager()->IncrementTextureUsage(newtexture);

		bool result = mesh->GetDynamicMesh()->ChangeTexture(oldtexture, newtexture, mesh);
		if (result == false)
		{
			material = old; //revert name
			return false;
		}

		//mesh->ResetPrepare();
		return true;
	}
	return false;
}

bool Polygon::ChangeTexture(const std::string &texture, bool matcheck)
{
	//changes a texture
	//if matcheck is true, exit if old and new textures are the same

	if (matcheck == true)
	{
		if (material == texture)
			return false;
	}

	std::string old = material;
	sbs->GetTextureManager()->DecrementTextureUsage(material);
	material = texture;
	sbs->GetTextureManager()->IncrementTextureUsage(texture);

	bool result = mesh->GetDynamicMesh()->ChangeTexture(old, texture, mesh);
	if (result == false)
	{
		material = old; //revert name
		return false;
	}

	//mesh->ResetPrepare();
	return true;
}

Vector3 Polygon::GetVertex(int index)
{
	int offset = 0;
	for (size_t i = 0; i < geometry.size(); i++)
	{
		for (size_t j = 0; j < geometry[i].size(); j++)
		{
			if (index == offset + j)
				return geometry[i][j].vertex;
		}
		offset += geometry[i].size();
	}
	return Vector3::ZERO;
}

bool Polygon::IntersectRay(const Vector3& rayOrigin, const Vector3& rayDir, Vector3& hitPoint)
{
	//simple convex polygon ray intersection

	//get polygon plane
	Plane plane = GetAbsolutePlane();

	//compute intersection of ray with plane
	Real denom = plane.normal.dotProduct(rayDir);
	if (std::abs(denom) < SMALL_EPSILON)
		return false; //ray is parallel to plane

	Real t = -(plane.normal.dotProduct(rayOrigin) + plane.d) / denom;
	if (t < 0)
		return false; //intersection is behind the ray origin

	//compute intersection point
	hitPoint = rayOrigin + rayDir * t;

	//point-in-polygon test (convex polygons)
	for (size_t i = 0; i < geometry.size(); i++)
	{
		const auto& verts = geometry[i];
		size_t n = verts.size();
		for (size_t i = 0; i < n; ++i)
		{
			Vector3 v0 = verts[i].vertex;
			Vector3 v1 = verts[(i + 1) % n].vertex;
			Vector3 edge = v1 - v0;
			Vector3 toPoint = hitPoint - v0;
			Vector3 cross = edge.crossProduct(toPoint);
			if (plane.normal.dotProduct(cross) < -SMALL_EPSILON)
				return false; //outside polygon
		}
	}

	return true; //ray hits polygon
}

bool Polygon::IntersectSegmentPlane(const Vector3 &start, const Vector3 &end, Vector3 &isect, Real *pr, Vector3 &normal)
{
	//computes the intersection point between a line segment and a polygon's plane
	//returns true if the intersection point lies within the segment (between start and end)

	//get the plane of the polygon
	Plane plane = GetAbsolutePlane();

	//get direction vector of the segment
	Vector3 dir = end - start;

	//compute denominator for intersection formula
	Real denom = plane.normal.dotProduct(dir);

	//check if the segment is parallel to the plane
	if (std::abs(denom) < SMALL_EPSILON)
	{
		if (pr) *pr = -1;
		return false;
	}

	//compute intersection parameter r
	Real num = -(plane.normal.dotProduct(start) + plane.d);
	Real r = num / denom;

	//output r if requested
	if (pr) *pr = r;

	//compute intersection point
	isect = start + dir * r;

	//accept intersection if r is within [0, 1] (with epsilon tolerance)
	if (r < -SMALL_EPSILON || r > 1.0 + SMALL_EPSILON)
		return false;

	normal = plane.normal;
	return true;
}

bool Polygon::IntersectSegment(const Vector3 &start, const Vector3 &end, Vector3 &isect, Real *pr, Vector3 &normal)
{
	//checks if a line segment between two points intersects a convex polygon
	//positions need to be in remote (Ogre) values

	//ensure direction is correct
	Vector3 dir = end - start;
	if (dir.length() < SMALL_EPSILON)
		return false; //degenerate segment

	//normalize direction
	dir.normalise();

	//check if the ray hits the polygon
	Vector3 hit_point;
	if (!IntersectRay(start, dir, hit_point))
		return false;

	//check if intersection is within the segment
	bool plane_hit = IntersectSegmentPlane(start, end, isect, pr, normal);
	return plane_hit;
}

}
