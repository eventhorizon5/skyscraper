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

#include <cmath>
#include "globals.h"
#include "sbs.h"
#include "polymesh.h"
#include "mesh.h"
#include "polygon.h"
#include "polymesh.h"
#include "triangle.h"
#include "texman.h"
#include "profiler.h"
#include "utility.h"
#include "wall.h"

namespace SBS {

Wall::Wall(MeshObject* wrapper, const std::string &name) : Object(wrapper)
{
	//wall object constructor

	SetValues("Wall", name, false, true);
	meshwrapper = wrapper;
	polymesh = sbs->GetPolyMesh();
	parent_array = 0;

	if (!meshwrapper)
		return;

	SetParentArray(meshwrapper->Walls);

	polymesh->WallCount++;
}

Wall::~Wall()
{
	//wall object destructor

	if (sbs->FastDelete == false && parent_array && parent_deleting == false)
	{
		for (size_t i = 0; i < parent_array->size(); i++)
		{
			if (parent_array->at(i) == this)
			{
				parent_array->erase(parent_array->begin() + i);
				break;
			}
		}
	}

	polymesh->WallCount--;

	//delete polygons
	DeletePolygons(!sbs->FastDelete);
}

Polygon* Wall::AddQuad(const std::string &name, const std::string &texture, const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, const Vector3 &v4, Real tw, Real th, bool autosize)
{
	//add a quad

	PolyArray vertices;
	vertices.reserve(4);
	vertices.emplace_back(v1);
	vertices.emplace_back(v2);
	vertices.emplace_back(v3);
	vertices.emplace_back(v4);

	return AddPolygon(name, texture, vertices, tw, th, autosize);
}

Polygon* Wall::AddTriangle(const std::string &name, const std::string &texture, const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, Real tw, Real th, bool autosize)
{
	//add a triangle

	PolyArray vertices;
	vertices.reserve(3);
	vertices.emplace_back(v1);
	vertices.emplace_back(v2);
	vertices.emplace_back(v3);

	return AddPolygon(name, texture, vertices, tw, th, autosize);
}

Polygon* Wall::AddPolygon(const std::string &name, const std::string &texture, PolyArray &vertices, Real tw, Real th, bool autosize)
{
	//create a generic polygon

	Matrix3 tm;
	Vector3 tv;
	//std::vector<Extents> index_extents;
	std::vector<std::vector<Polygon::Geometry> > geometry;
	std::vector<Triangle> triangles;
	PolygonSet converted_vertices;
	if (!polymesh->CreateMesh(meshwrapper, name, texture, vertices, tw, th, autosize, tm, tv, geometry, triangles, converted_vertices))
	{
		ReportError("Error creating wall '" + name + "'");
		return 0;
	}

	if (triangles.size() == 0)
		return 0;

	bool result;
	std::string material = sbs->GetTextureManager()->GetTextureMaterial(texture, result, true, name);

	//compute plane
	Plane plane = sbs->GetPolyMesh()->ComputePlane(converted_vertices[0]);

	Polygon* poly = new Polygon(this, name, meshwrapper, geometry, triangles, tm, tv, material, plane);
	polygons.emplace_back(poly);
	return poly;
}

Polygon* Wall::AddPolygon(const std::string &name, const std::string &texture, PolygonSet &vertices, std::vector<std::vector<Vector2>> &uvMap, std::vector<Triangle> &triangles, Real tw, Real th, bool autosize)
{
	//create a generic polygon, providing the UV texture mapping and triangles

	Matrix3 tm;
	Vector3 tv;
	//std::vector<Extents> index_extents;
	std::vector<std::vector<Polygon::Geometry> > geometry;
	PolygonSet converted_vertices;
	if (!polymesh->CreateMesh(meshwrapper, name, texture, vertices, uvMap, geometry, converted_vertices, tw, th))
	{
		ReportError("Error creating wall '" + name + "'");
		return 0;
	}

	if (triangles.size() == 0)
		return 0;

	bool result;
	std::string material = sbs->GetTextureManager()->GetTextureMaterial(texture, result, true, name);

	//compute plane
	Plane plane = sbs->GetPolyMesh()->ComputePlane(converted_vertices[0]);

	Polygon* poly = new Polygon(this, name, meshwrapper, geometry, triangles, tm, tv, material, plane);
	polygons.emplace_back(poly);
	return poly;
}

Polygon* Wall::AddPolygonSet(const std::string &name, const std::string &material, PolygonSet &vertices, Matrix3 &tex_matrix, Vector3 &tex_vector)
{
	//create a set of polygons, providing the original material and texture mapping

	std::vector<std::vector<Polygon::Geometry> > geometry;
	std::vector<Triangle> triangles;
	PolygonSet converted_vertices;
	if (!polymesh->CreateMesh(meshwrapper, name, material, vertices, tex_matrix, tex_vector, geometry, triangles, converted_vertices, 0, 0))
	{
		ReportError("Error creating wall '" + name + "'");
		return 0;
	}

	if (triangles.size() == 0)
		return 0;

	//compute plane
	Plane plane = sbs->GetPolyMesh()->ComputePlane(converted_vertices[0]);

	Polygon* poly = new Polygon(this, name, meshwrapper, geometry, triangles, tex_matrix, tex_vector, material, plane);
	polygons.emplace_back(poly);

	return poly;
}

void Wall::DeletePolygons(bool recreate_collider)
{
	//delete polygons

	if (!meshwrapper)
		return;

	for (size_t i = polygons.size() - 1; i < polygons.size(); --i)
	{
		delete polygons[i];
	}
	polygons.clear();

	//recreate colliders
	if (recreate_collider == true)
	{
		//prepare mesh
		meshwrapper->Prepare();

		meshwrapper->DeleteCollider();
		meshwrapper->CreateCollider();
	}
}

void Wall::DeletePolygon(int index, bool recreate_colliders)
{
	//delete a single polygon

	if (!meshwrapper)
		return;

	if (index > -1 && index < (int)polygons.size())
	{
		//delete polygon
		delete polygons[index];
		polygons.erase(polygons.begin() + index);

		//recreate colliders if specified
		if (recreate_colliders == true)
		{
			meshwrapper->Prepare();
			meshwrapper->DeleteCollider();
			meshwrapper->CreateCollider();
		}
	}
}

int Wall::GetPolygonCount()
{
	return (int)polygons.size();
}

Polygon* Wall::GetPolygon(int index)
{
	if (index > -1 && index < (int)polygons.size())
	{
		if (polygons[index])
			return polygons[index];
	}
	return 0;
}

int Wall::FindPolygon(const std::string &name)
{
	//find a polygon object by name

	SBS_PROFILE("Wall::FindPolygon");

	for (size_t i = 0; i < polygons.size(); i++)
	{
		if (!polygons[i])
			continue;

		if (name == polygons[i]->GetName())
			return (int)i;
	}
	return -1;
}

bool Wall::IntersectsWall(Vector3 start, Vector3 end, Vector3 &isect, bool convert)
{
	//check through polygons to see if the specified line intersects with this wall object

	SBS_PROFILE("Wall::IntersectsWall");
	Real pr, best_pr = 2000000000.;
	int best_i = -1;
	Vector3 cur_isect, normal;

	if (convert == true)
	{
		start = sbs->ToRemote(start);
		end = sbs->ToRemote(end);
	}

	for (size_t i = 0; i < polygons.size(); i++)
	{
		if (!polygons[i])
			continue;

		if (polygons[i]->IntersectSegment(start, end, cur_isect, &pr, normal))
		{
			if (pr < best_pr)
			{
				best_pr = pr;
				best_i = (int)i;
				isect = cur_isect;
			}
		}
	}

	if (best_i > -1)
		return true;

	return false;
}

void Wall::Move(const Vector3 &vector, Real speed)
{
	//move a wall object

	if (!meshwrapper)
		return;

	//move base object
	Object::Move(vector, speed);

	//move polygons
	for (size_t i = 0; i < polygons.size(); i++)
	{
		if (polygons[i])
			polygons[i]->Move(vector, speed);
	}

	//prepare mesh
	if (meshwrapper->UsingDynamicBuffers() == false)
		meshwrapper->Prepare(true);
	meshwrapper->DeleteCollider();
	meshwrapper->CreateCollider();
}

MeshObject* Wall::GetMesh()
{
	return meshwrapper;
}

void Wall::SetParentArray(std::vector<Wall*> &array)
{
	parent_array = &array;
}

Vector3 Wall::GetPoint(const Vector3 &start, const Vector3 &end)
{
	//do a line intersection with this wall, and return the intersection point

	Vector3 isect;
	Real distance = 2000000000.;
	Vector3 normal = Vector3::ZERO;

	Wall *result = polymesh->FindWallIntersect(meshwrapper, sbs->ToRemote(start), sbs->ToRemote(end), isect, distance, normal, this);

	if (result)
		return sbs->ToLocal(isect);

	return Vector3(0, 0, 0);
}

Vector3 Wall::GetWallExtents(Real altitude, bool get_max)
{
	//return the X and Z extents of this wall object at a specific altitude, by doing a double plane cut

	PolyMesh* polymesh = sbs->GetPolyMesh();

	for (int i = 0; i < GetPolygonCount(); i++)
	{
		if (!polygons[i])
			continue;

		PolyArray poly, tmp1, tmp2;
		for (size_t j = 0; j < polygons[i]->geometry[0].size(); j++)
		{
			poly.emplace_back(sbs->ToLocal(polygons[i]->geometry[0][j].vertex));
		}

		//if given altitude is outside of polygon's range, return 0
		Vector2 yextents = polymesh->GetExtents(poly, 2);
		Real tmpaltitude = altitude;
		if (tmpaltitude < yextents.x || tmpaltitude > yextents.y)
			return Vector3(0, 0, 0);

		//get upper
		polymesh->SplitWithPlane(1, poly, tmp1, tmp2, tmpaltitude - 0.001);

		//get lower part of upper
		polymesh->SplitWithPlane(1, tmp2, poly, tmp1, tmpaltitude + 0.001);

		Vector3 result;
		if (get_max == false)
		{
			//get minimum extents
			result.x = polymesh->GetExtents(poly, 1).x;
			result.z = polymesh->GetExtents(poly, 3).x;
		}
		else
		{
			//get maximum extents
			result.x = polymesh->GetExtents(poly, 1).y;
			result.z = polymesh->GetExtents(poly, 3).y;
		}
		result.y = altitude;
		return result; //only check the first polygon for now
	}

	return Vector3(0, 0, 0);
}

void Wall::ChangeHeight(Real newheight)
{
	//change height of a wall object

	if (!meshwrapper)
		return;

	for (size_t i = 0; i < polygons.size(); i++)
	{
		if (polygons[i])
			polygons[i]->ChangeHeight(newheight);
	}

	//prepare mesh
	if (meshwrapper->UsingDynamicBuffers() == false)
		meshwrapper->Prepare(true);
	meshwrapper->DeleteCollider();
	meshwrapper->CreateCollider();
}

unsigned int Wall::GetVertexCount()
{
	unsigned int total = 0;
	for (size_t i = 0; i < polygons.size(); i++)
	{
		if (polygons[i])
			total += polygons[i]->vertex_count;
	}
	return total;
}

unsigned int Wall::GetTriangleCount()
{
	unsigned int total = 0;
	for (size_t i = 0; i < polygons.size(); i++)
	{
		if (polygons[i])
			total += polygons[i]->triangles.size();
	}
	return total;
}

bool Wall::ReplaceTexture(const std::string &oldtexture, const std::string &newtexture)
{
	bool found = false;

	for (int i = 0; i < GetPolygonCount(); i++)
	{
		Polygon *poly = GetPolygon(i);
		if (!poly)
			continue;

		bool result = poly->ReplaceTexture(oldtexture, newtexture);
		if (result == true)
			found = true;
	}
	return found;
}

bool Wall::ChangeTexture(const std::string &texture, bool matcheck)
{
	bool found = false;

	for (int i = 0; i < GetPolygonCount(); i++)
	{
		Polygon *poly = GetPolygon(i);
		if (!poly)
			continue;

		bool result = poly->ChangeTexture(texture, matcheck);
		if (result == true)
			found = true;
	}
	return found;
}

void Wall::CreateSphere(const std::string &name, const std::string &texture, Real radius, int latSteps, int lonSteps, Real tw, Real th, bool autosize)
{
	//generates a sphere on the given wall object

	PolygonSet result;
	std::vector<std::vector<Vector2>> uvMapSet;

	for (int lat = 0; lat < latSteps; ++lat)
	{
		float v1 = float(lat) / latSteps;
		float v2 = float(lat + 1) / latSteps;

		float theta1 = pi * (v1 - 0.5f);
		float theta2 = pi * (v2 - 0.5f);

		for (int lon = 0; lon < lonSteps; ++lon)
		{
			float u1 = float(lon) / lonSteps;
			float u2 = float(lon + 1) / lonSteps;

			float phi1 = 2.0f * pi * u1;
			float phi2 = 2.0f * pi * u2;

			Vector3 p1 = radius * Vector3(cosf(theta1) * cosf(phi1), sinf(theta1), cosf(theta1) * sinf(phi1));
			Vector3 p2 = radius * Vector3(cosf(theta2) * cosf(phi1), sinf(theta2), cosf(theta2) * sinf(phi1));
			Vector3 p3 = radius * Vector3(cosf(theta2) * cosf(phi2), sinf(theta2), cosf(theta2) * sinf(phi2));
			Vector3 p4 = radius * Vector3(cosf(theta1) * cosf(phi2), sinf(theta1), cosf(theta1) * sinf(phi2));

			std::vector<Vector2> uvMap;
			uvMap.reserve(6);

			// First triangle
			PolyArray tri1;
			tri1.reserve(3);
			tri1.emplace_back(p1);
			uvMap.emplace_back(Vector2(u1, v1));

			tri1.emplace_back(p2);
			uvMap.emplace_back(Vector2(u1, v2));

			tri1.emplace_back(p3);
			uvMap.emplace_back(Vector2(u2, v2));

			result.emplace_back(tri1);
			uvMapSet.emplace_back(uvMap);

			// Second triangle
			PolyArray tri2;
			uvMap.clear();
			tri2.reserve(3);
			tri2.emplace_back(p1);
			uvMap.emplace_back(Vector2(u1, v1));

			tri2.emplace_back(p3);
			uvMap.emplace_back(Vector2(u2, v2));

			tri2.emplace_back(p4);
			uvMap.emplace_back(Vector2(u2, v1));

			result.emplace_back(tri2);
			uvMapSet.emplace_back(uvMap);
		}
	}

	//create polygons from the generated sphere segments
	//AddPolygonMesh(name, texture, result, uvMapSet);
}

void Wall::CreateBox(const std::string &name, const std::string &texture, Real width, Real height, Real depth, Real tw, Real th, bool autosize)
{
	Real hw = width * 0.5;
	Real hh = height * 0.5;
	Real hd = depth * 0.5;

	// 8 corners of the box
	Vector3 p000(-hw, -hh, -hd); // left bottom back
	Vector3 p001(-hw, -hh,  hd); // left bottom front
	Vector3 p010(-hw,  hh, -hd); // left top back
	Vector3 p011(-hw,  hh,  hd); // left top front
	Vector3 p100( hw, -hh, -hd); // right bottom back
	Vector3 p101( hw, -hh,  hd); // right bottom front
	Vector3 p110( hw,  hh, -hd); // right top back
	Vector3 p111( hw,  hh,  hd); // right top front

	auto addQuad = [&](const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d,
			const Vector2& uvA, const Vector2& uvB, const Vector2& uvC, const Vector2& uvD)
	{
		PolygonSet result;
		std::vector<std::vector<Vector2>> uvMapSet;

		std::vector<Vector2> uvMap;
		uvMap.reserve(2);
		std::vector<Triangle> triangles;
		triangles.reserve(2);

		PolyArray poly1;
		Triangle tri1;
		poly1.emplace_back(a);
		uvMap.emplace_back(uvA);
		tri1.a = 2;

		poly1.emplace_back(b);
		uvMap.emplace_back(uvB);
		tri1.b = 1;

		poly1.emplace_back(c);
		uvMap.emplace_back(uvC);
		tri1.c = 0;

		result.emplace_back(poly1);
		uvMapSet.emplace_back(uvMap);
		triangles.emplace_back(tri1);

		uvMap.clear();
		PolyArray poly2;
		Triangle tri2;
		poly2.emplace_back(a);
		uvMap.emplace_back(uvA);
		tri2.a = 5;

		poly2.emplace_back(c);
		uvMap.emplace_back(uvC);
		tri2.b = 4;

		poly2.emplace_back(d);
		uvMap.emplace_back(uvD);
		tri2.c = 3;

		result.emplace_back(poly2);
		uvMapSet.emplace_back(uvMap);
		triangles.emplace_back(tri2);

		//create polygon from the generated triangles
		AddPolygon(name, texture, result, uvMapSet, triangles, tw, th, autosize);
	};

	//back face (+Z)
	addQuad(p101, p001, p011, p111, {1, 0}, {0, 0}, {0, 1}, {1, 1});

	//front face (-Z)
	addQuad(p100, p110, p010, p000, {1, 0}, {1, 1}, {0, 1}, {0, 0});

	//left face (-X)
	addQuad(p001, p000, p010, p011, {1, 0}, {0, 0}, {0, 1}, {1, 1});

	//right face (+X)
	addQuad(p100, p101, p111, p110, {0, 0}, {1, 0}, {1, 1}, {0, 1});

	//top face (+Y)
	addQuad(p011, p010, p110, p111, {0, 1}, {0, 0}, {1, 0}, {1, 1});

	//bottom face (-Y)
	addQuad(p100, p000, p001, p101, {1, 1}, {0, 1}, {0, 0}, {1, 0});
}

}
