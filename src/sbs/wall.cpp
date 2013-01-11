/* $Id$ */

/*
	Scalable Building Simulator - Wall Object Class
	The Skyscraper Project - Version 1.8 Alpha
	Copyright (C)2004-2013 Ryan Thoryk
	http://www.skyscrapersim.com
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@tliquest.net

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
#include "wall.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

WallObject::WallObject(MeshObject* wrapper, Object *proxy, bool temporary) : Object(temporary)
{
	//wall object constructor
	meshwrapper = wrapper;
	parent_array = 0;

	//if proxy object is set, set object's number as proxy object's number
	if (proxy)
		Number = proxy->GetNumber();
	sbs->WallCount++;
}

WallObject::~WallObject()
{
	//wall object destructor

	if (sbs->FastDelete == false && parent_array && parent_deleting == false && Temporary == false)
	{
		for (int i = 0; i < (int)parent_array->size(); i++)
		{
			if (parent_array->at(i) == this)
				parent_array->erase(parent_array->begin() + i);
		}
	}

	sbs->WallCount--;
	sbs->PolygonCount -= handles.size();
	handles.clear();
}

WallPolygon* WallObject::AddQuad(const char *name, const char *texture, const Ogre::Vector3 &v1, const Ogre::Vector3 &v2, const Ogre::Vector3 &v3, const Ogre::Vector3 &v4, float tw, float th, bool autosize)
{
	//add a quad

	std::vector<std::vector<Ogre::Vector3> > array;
	array.resize(1);
	array[0].reserve(4);
	array[0].push_back(v1);
	array[0].push_back(v2);
	array[0].push_back(v3);
	array[0].push_back(v4);
	std::string name2 = ProcessName(name);
	Ogre::Matrix3 tm;
	Ogre::Vector3 tv;
	std::vector<Ogre::Vector2> index_extents;
	std::vector<TriangleType> triangles;
	if (!meshwrapper->PolyMesh(name2.c_str(), texture, array[0], tw, th, autosize, tm, tv, index_extents, triangles))
	{
		sbs->ReportError("Error creating wall " + name2);
		return 0;
	}

	if (triangles.size() == 0)
		return 0;

	bool result;
	std::string material = sbs->GetTextureMaterial(texture, result, name2.c_str());

	//compute plane from first 3 vertices
	Ogre::Plane plane(array[0][0], array[0][1], array[0][2]);

	int index = CreateHandle(triangles, index_extents, tm, tv, material, name2.c_str(), plane);
	return &handles[index];
}

WallPolygon* WallObject::AddPolygon(const char *name, const char *texture, std::vector<Ogre::Vector3> &vertices, float tw, float th, bool autosize)
{
	//create a generic polygon
	std::string name2 = ProcessName(name);
	Ogre::Matrix3 tm;
	Ogre::Vector3 tv;
	std::vector<Ogre::Vector2> index_extents;
	std::vector<TriangleType> triangles;
	if (!meshwrapper->PolyMesh(name2.c_str(), texture, vertices, tw, th, autosize, tm, tv, index_extents, triangles))
	{
		sbs->ReportError("Error creating wall " + name2);
		return 0;
	}

	if (triangles.size() == 0)
		return 0;

	bool result;
	std::string material = sbs->GetTextureMaterial(texture, result, name2.c_str());

	//compute plane from first 3 vertices
	Ogre::Plane plane(vertices[0], vertices[1], vertices[2]);

	int index = CreateHandle(triangles, index_extents, tm, tv, material, name2.c_str(), plane);
	return &handles[index];
}

WallPolygon* WallObject::AddPolygon(const char *name, std::string material, std::vector<std::vector<Ogre::Vector3> > &vertices, Ogre::Matrix3 &tex_matrix, Ogre::Vector3 &tex_vector)
{
	//add a set of polygons, providing the original material and texture mapping
	std::string name2 = ProcessName(name);
	std::vector<Ogre::Vector2> index_extents;
	std::vector<TriangleType> triangles;
	if (!meshwrapper->PolyMesh(name2.c_str(), material, vertices, tex_matrix, tex_vector, index_extents, triangles, 0, 0))
	{
		sbs->ReportError("Error creating wall " + name2);
		return 0;
	}

	if (triangles.size() == 0)
		return 0;

	//compute plane from first 3 vertices
	Ogre::Plane plane(vertices[0][0], vertices[0][1], vertices[0][2]);

	int index = CreateHandle(triangles, index_extents, tex_matrix, tex_vector, material, name2.c_str(), plane);
	return &handles[index];
}

int WallObject::CreateHandle(std::vector<TriangleType> &triangles, std::vector<Ogre::Vector2> &index_extents, Ogre::Matrix3 &tex_matrix, Ogre::Vector3 &tex_vector, std::string material, const char *name, Ogre::Plane &plane)
{
	//create a polygon handle
	int i = (int)handles.size();
	handles.resize(handles.size() + 1);
	sbs->PolygonCount++;
	handles[i].mesh = meshwrapper;
	handles[i].index_extents = index_extents;
	handles[i].t_matrix = tex_matrix;
	handles[i].t_vector = tex_vector;
	handles[i].material = material;
	handles[i].plane = plane;
	handles[i].triangles = triangles;
	SetPolygonName(i, name);

	return i;
}

std::string WallObject::ProcessName(const char *name)
{
	//process name for use
	std::string name_modified = name;

	//strip off object ID from name if it exists
	if ((int)name_modified.find("(") == 0)
		name_modified.erase(0, (int)name_modified.find(")") + 1);

	//construct name
	std::string newname = "(";
	std::string num = Ogre::StringConverter::toString(Number);
	newname.append(num + ")");
	newname.append(name_modified);
	return newname;
}

void WallObject::DeletePolygons(bool recreate_collider)
{
	//delete polygons and handles
	
	for (int i = 0; i < (int)handles.size(); i++)
		DeletePolygon(i, false);

	//recreate colliders
	if (recreate_collider == true)
	{
		//prepare mesh
		meshwrapper->Prepare();

		meshwrapper->DeleteCollider();
		meshwrapper->CreateCollider();
	}
}

void WallObject::DeletePolygon(int index, bool recreate_colliders)
{
	//delete a single polygon

	if (index > -1 && index < (int)handles.size())
	{
		//delete triangles
		meshwrapper->ProcessSubMesh(handles[index].triangles, handles[index].material, handles[index].name.c_str(), false);

		//delete related mesh vertices
		meshwrapper->DeleteVertices(*parent_array, handles[index].triangles);

		//delete polygon
		handles.erase(handles.begin() + index);

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

int WallObject::GetHandleCount()
{
	return (int)handles.size();
}

WallPolygon* WallObject::GetHandle(int index)
{
	if (index > -1 && index < (int)handles.size())
		return &handles[index];
	return 0;
}

int WallObject::FindPolygon(const char *name)
{
	//find a polygon object by name

	SBS_PROFILE("WallObject::FindPolygon");

	std::string name2 = name;
	for (int i = 0; i < (int)handles.size(); i++)
	{
		std::string tmpname = handles[i].name;
		if ((int)tmpname.find("(") == 0)
		{
			//strip object number
			int loc = tmpname.find(")");
			tmpname.erase(0, loc + 1);
		}
		if (name2 == tmpname)
			return i;
	}
	return -1;
}

void WallObject::GetGeometry(int index, std::vector<std::vector<Ogre::Vector3> > &vertices, bool firstonly)
{
	//gets vertex geometry using mesh's vertex extent arrays; returns vertices in 'vertices'

	if (index < 0 || index >= (int)handles.size())
		return;

	handles[index].GetGeometry(meshwrapper, vertices, firstonly);
}

void WallObject::SetPolygonName(int index, const char *name)
{
		if (index < 0 || index >= (int)handles.size())
			return;

		//set polygon name
        std::string name_modified = name;

        //strip off object ID from name if it exists
        if ((int)name_modified.find("(") == 0)
                name_modified.erase(0, (int)name_modified.find(")") + 1);

        //construct name
        std::string newname = "(";
        std::string num = Ogre::StringConverter::toString(Number);
        newname.append(num + ")");
        newname.append(name_modified);

        //set polygon name
        handles[index].name = newname;
}

bool WallObject::IsPointOnWall(const Ogre::Vector3 &point, bool convert)
{
	//check through polygons to see if the specified point is on this wall object

	SBS_PROFILE("WallObject::IsPointOnWall");
	bool checkplane = false;
	for (int i = 0; i < (int)handles.size(); i++)
	{
		if (i == 0)
			checkplane = true;
		bool check = handles[i].PointInside(meshwrapper, point, checkplane, convert);
		if (check == true)
			return true;
	}
	return false;
}

bool WallObject::IntersectsWall(const Ogre::Vector3 &start, const Ogre::Vector3 &end, Ogre::Vector3 &isect, bool convert)
{
	//check through polygons to see if the specified point is on this wall object

	SBS_PROFILE("WallObject::IntersectsWall");
	float pr, best_pr = 2000000000.;
	int best_i = -1;
	Ogre::Vector3 cur_isect;

	for (int i = 0; i < (int)handles.size(); i++)
	{
		if (handles[i].IntersectSegment(meshwrapper, start, end, cur_isect, &pr, convert))
		{
			if (pr < best_pr)
			{
				best_pr = pr;
				best_i = i;
				isect = cur_isect;
			}
		}
	}

	if (best_i > -1)
		return true;

	return false;
}
