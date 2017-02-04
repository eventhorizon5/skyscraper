/* $Id$ */

/*
	Scalable Building Simulator - Wall Object
	The Skyscraper Project - Version 1.11 Alpha
	Copyright (C)2004-2017 Ryan Thoryk
	http://www.skyscrapersim.com
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@skyscrapersim.com

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
#include "mesh.h"
#include "triangle.h"
#include "texture.h"
#include "profiler.h"
#include "wall.h"

namespace SBS {

Wall::Wall(MeshObject* wrapper, Object *proxy, bool temporary) : Object(wrapper, temporary)
{
	//wall object constructor
	meshwrapper = wrapper;
	parent_array = 0;

	//if proxy object is set, set object's number as proxy object's number
	if (proxy)
		SetNumber(proxy->GetNumber());
	sbs->WallCount++;
}

Wall::~Wall()
{
	//wall object destructor

	if (sbs->FastDelete == false && parent_array && parent_deleting == false && IsTemporary() == false)
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

	sbs->WallCount--;
	sbs->PolygonCount -= (int)polygons.size();
	polygons.clear();
}

Polygon* Wall::AddQuad(const std::string &name, const std::string &texture, const Ogre::Vector3 &v1, const Ogre::Vector3 &v2, const Ogre::Vector3 &v3, const Ogre::Vector3 &v4, float tw, float th, bool autosize)
{
	//add a quad

	std::vector<Ogre::Vector3> vertices;
	vertices.reserve(4);
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);
	vertices.push_back(v4);

	return AddPolygon(name, texture, vertices, tw, th, autosize);
}

Polygon* Wall::AddPolygon(const std::string &name, const std::string &texture, std::vector<Ogre::Vector3> &vertices, float tw, float th, bool autosize)
{
	//create a generic polygon

	Ogre::Matrix3 tm;
	Ogre::Vector3 tv;
	std::vector<Extents> index_extents;
	std::vector<Triangle> triangles;
	std::vector<std::vector<Ogre::Vector3> > converted_vertices;
	if (!meshwrapper->PolyMesh(name, texture, vertices, tw, th, autosize, tm, tv, index_extents, triangles, converted_vertices))
	{
		ReportError("Error creating wall '" + name + "'");
		return 0;
	}

	if (triangles.size() == 0)
		return 0;

	bool result;
	std::string material = sbs->GetTextureManager()->GetTextureMaterial(texture, result, true, name);

	//compute plane
	Ogre::Plane plane = sbs->ComputePlane(converted_vertices[0]);

	int index = CreatePolygon(triangles, index_extents, tm, tv, material, name, plane);
	return &polygons[index];
}

Polygon* Wall::AddPolygon(const std::string &name, const std::string &material, std::vector<std::vector<Ogre::Vector3> > &vertices, Ogre::Matrix3 &tex_matrix, Ogre::Vector3 &tex_vector)
{
	//add a set of polygons, providing the original material and texture mapping

	std::vector<Extents> index_extents;
	std::vector<Triangle> triangles;
	std::vector<std::vector<Ogre::Vector3> > converted_vertices;
	if (!meshwrapper->PolyMesh(name, material, vertices, tex_matrix, tex_vector, index_extents, triangles, converted_vertices, 0, 0))
	{
		ReportError("Error creating wall '" + name + "'");
		return 0;
	}

	if (triangles.size() == 0)
		return 0;

	//compute plane
	Ogre::Plane plane = sbs->ComputePlane(converted_vertices[0]);

	int index = CreatePolygon(triangles, index_extents, tex_matrix, tex_vector, material, name, plane);
	return &polygons[index];
}

int Wall::CreatePolygon(std::vector<Triangle> &triangles, std::vector<Extents> &index_extents, Ogre::Matrix3 &tex_matrix, Ogre::Vector3 &tex_vector, const std::string &material, const std::string &name, Ogre::Plane &plane)
{
	//create a polygon handle

	Polygon polygon(this, name, meshwrapper, triangles, index_extents, tex_matrix, tex_vector, material, plane);
	polygons.push_back(polygon);
	sbs->PolygonCount++;

	return (int)polygons.size() - 1;
}

void Wall::DeletePolygons(bool recreate_collider)
{
	//delete polygons

	for (size_t i = polygons.size() - 1; i < polygons.size(); --i)
		DeletePolygon((int)i, false);

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

	if (index > -1 && index < (int)polygons.size())
	{
		//delete polygon
		polygons[index].Delete();
		polygons.erase(polygons.begin() + index);

		sbs->PolygonCount--;

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
		return &polygons[index];
	return 0;
}

int Wall::FindPolygon(const std::string &name)
{
	//find a polygon object by name

	SBS_PROFILE("Wall::FindPolygon");

	for (size_t i = 0; i < polygons.size(); i++)
	{
		if (name == polygons[i].GetName())
			return (int)i;
	}
	return -1;
}

void Wall::GetGeometry(int index, std::vector<std::vector<Ogre::Vector3> > &vertices, bool firstonly, bool convert, bool rescale, bool relative, bool reverse)
{
	//gets vertex geometry using mesh's vertex extent arrays; returns vertices in 'vertices'

	//if firstonly is true, only return first result
	//if convert is true, converts vertices from remote Ogre positions to local SBS positions
	//if rescale is true (along with convert), rescales vertices with UnitScale multiplier
	//if relative is true, vertices are relative of mesh center, otherwise they use absolute/global positioning
	//if reverse is false, process extents table in ascending order, otherwise descending order

	if (index < 0 || index >= (int)polygons.size())
		return;

	polygons[index].GetGeometry(vertices, firstonly, convert, rescale, relative, reverse);
}

bool Wall::IntersectsWall(Ogre::Vector3 start, Ogre::Vector3 end, Ogre::Vector3 &isect, bool convert)
{
	//check through polygons to see if the specified line intersects with this wall object

	SBS_PROFILE("Wall::IntersectsWall");
	float pr, best_pr = 2000000000.;
	int best_i = -1;
	Ogre::Vector3 cur_isect, normal;

	if (convert == true)
	{
		start = sbs->ToRemote(start);
		end = sbs->ToRemote(end);
	}

	for (size_t i = 0; i < polygons.size(); i++)
	{
		if (polygons[i].IntersectSegment(start, end, cur_isect, &pr, normal))
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

void Wall::Move(const Ogre::Vector3 &position, float speed)
{
	//move a wall object

	for (size_t i = 0; i < polygons.size(); i++)
	{
		polygons[i].Move(position, speed);
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

Ogre::Vector3 Wall::GetPoint(const Ogre::Vector3 &start, const Ogre::Vector3 &end)
{
	//do a line intersection with this wall, and return the intersection point

	Ogre::Vector3 isect;
	float distance = 2000000000.;
	Ogre::Vector3 normal = Ogre::Vector3::ZERO;

	Wall *result = meshwrapper->FindWallIntersect(sbs->ToRemote(start), sbs->ToRemote(end), isect, distance, normal, this);

	if (result)
		return sbs->ToLocal(isect);

	return Ogre::Vector3(0, 0, 0);
}

Ogre::Vector3 Wall::GetWallExtents(float altitude, bool get_max)
{
	//return the X and Z extents of this wall object at a specific altitude, by doing a double plane cut

	for (int i = 0; i < GetPolygonCount(); i++)
	{
		std::vector<std::vector<Ogre::Vector3> > origpolys;
		GetGeometry(i, origpolys, true);

		std::vector<Ogre::Vector3> original, tmp1, tmp2;
		original.reserve(origpolys[0].size());
		for (size_t i = 0; i < origpolys[0].size(); i++)
			original.push_back(origpolys[0][i]);

		//if given altitude is outside of polygon's range, return 0
		Ogre::Vector2 yextents = sbs->GetExtents(original, 2);
		float tmpaltitude = altitude;
		if (tmpaltitude < yextents.x || tmpaltitude > yextents.y)
			return Ogre::Vector3(0, 0, 0);

		//get upper
		sbs->SplitWithPlane(1, original, tmp1, tmp2, tmpaltitude - 0.001f);

		//get lower part of upper
		sbs->SplitWithPlane(1, tmp2, original, tmp1, tmpaltitude + 0.001f);

		Ogre::Vector3 result;
		if (get_max == false)
		{
			//get minimum extents
			result.x = sbs->GetExtents(original, 1).x;
			result.z = sbs->GetExtents(original, 3).x;
		}
		else
		{
			//get maximum extents
			result.x = sbs->GetExtents(original, 1).y;
			result.z = sbs->GetExtents(original, 3).y;
		}
		result.y = altitude;
		return result; //only check the first polygon for now
	}

	return Ogre::Vector3(0, 0, 0);
}

}
