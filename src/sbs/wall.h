/* $Id$ */

/*
	Scalable Building Simulator - Wall Object Class
	The Skyscraper Project - Version 1.8 Alpha
	Copyright (C)2004-2012 Ryan Thoryk
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

#ifndef _SBS_WALL_H
#define _SBS_WALL_H

class SBSIMPEXP WallObject : public Object
{
public:

	//functions
	WallObject(MeshObject* wrapper, Object *proxy = 0, bool temporary = false);
	~WallObject();
	WallPolygon* AddQuad(const char *name, const char *texture, const Ogre::Vector3 &v1, const Ogre::Vector3 &v2, const Ogre::Vector3 &v3, const Ogre::Vector3 &v4, float tw, float th, bool autosize);
	WallPolygon* AddPolygon(const char *name, const char *texture, std::vector<Ogre::Vector3> &vertices, float tw, float th, bool autosize);
	WallPolygon* AddPolygon(const char *name, std::string material, std::vector<std::vector<Ogre::Vector3> > &vertices, Ogre::Matrix3 &tex_matrix, Ogre::Vector3 &tex_vector);
	int CreateHandle(std::vector<TriangleType> &triangles, std::vector<Ogre::Vector2> &index_extents, Ogre::Matrix3 &tex_matrix, Ogre::Vector3 &tex_vector, std::string material, const char *name, Ogre::Plane &plane);
	void DeletePolygons();
	void DeletePolygon(int index, bool recreate_colliders);
	void DeleteVertices(std::vector<int> &deleted_indices);
	std::string ProcessName(const char *name);
	int GetHandleCount();
	WallPolygon* GetHandle(int index);
	int FindPolygon(const char *name);
	void GetGeometry(int index, std::vector<std::vector<Ogre::Vector3> > &vertices, bool firstonly = false);
	void SetPolygonName(int index, const char *name);
	bool IsPointOnWall(const Ogre::Vector3 &point, bool convert = true);
	bool IntersectsWall(const Ogre::Vector3 &start, const Ogre::Vector3 &end, Ogre::Vector3 &isect, bool convert = true);

	//mesh wrapper
	MeshObject* meshwrapper;

	//name
	std::string name;

	//parent array
	std::vector<WallObject*> *parent_array;

	std::vector<WallPolygon> handles;
};

#endif
