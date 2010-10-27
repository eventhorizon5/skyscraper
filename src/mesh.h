/* $Id$ */

/*
	Scalable Building Simulator - Mesh and Polygon Classes
	The Skyscraper Project - Version 1.8 Alpha
	Copyright (C)2004-2010 Ryan Thoryk
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

#ifndef _SBS_MESH_H
#define _SBS_MESH_H

class WallObject;

class SBSIMPEXP MeshObject
{
public:
	Object *object; //SBS object
	Ogre::String name; //mesh name

	MeshObject(Object* parent, const char *name, const char *filename = 0, float max_render_distance = 0, float scale_multiplier = 1);
	~MeshObject();
	void Enable(bool value);
	bool IsEnabled();
	WallObject* CreateWallObject(Object *parent, const char *name);
	Ogre::Material* ChangeTexture(const char *texture, bool matcheck = true);
	int FindWall(const Ogre::Vector3 &point);
	void RescaleVertices(float multiplier);
	bool LoadColladaFile(const char *filename, const char *name);
	void Move(const Ogre::Vector3 position, bool relative_x, bool relative_y, bool relative_z, Ogre::Vector3 origin = Ogre::Vector3(0, 0, 0));
	Ogre::Vector3 GetPosition();
	void Rotate(const Ogre::Vector3 rotation, float speed);
	void SetRotation(const Ogre::Vector3 rotation);
	Ogre::Vector3 GetRotation();
	
	Ogre::Mesh* MeshWrapper; //building mesh
	std::vector<Ogre::SubMesh> Submeshes;
	std::vector<WallObject*> Walls;
	Ogre::Entity *Movable;

private:
	bool enabled;
	//csRef<iCollection> collection;
	float rotX, rotY, rotZ;
};

class SBSIMPEXP WallPolygon
{
public:
	std::vector<Ogre::SubMesh> *submeshes;
	Ogre::HardwareIndexBuffer* triangles;
	Ogre::Plane plane;

	//array holding index extents, to get original geometry
	std::vector<Ogre::Vector2> index_extents;

	//texture mapping matrix and vector
	Ogre::Matrix3 t_matrix;
	Ogre::Vector3 t_vector;

	Ogre::Material* material; //polygon materials
	Ogre::String name; //polygon names

	WallPolygon();
	~WallPolygon();
	void GetTextureMapping(Ogre::Matrix3 &t_matrix, Ogre::Vector3 &t_vector);
	Ogre::SubMesh* GetSubMesh();
	void GetGeometry(Ogre::Mesh* meshwrapper, std::vector<Ogre::Polygon> &vertices, bool firstonly);
	bool PointInside(Ogre::Mesh* meshwrapper, const Ogre::Vector3 &point, bool plane_check = true);
};

#endif
