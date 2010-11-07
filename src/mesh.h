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

struct TriangleType
{
	unsigned int x, y, z;
	TriangleType(unsigned int a, unsigned int b, unsigned int c)
	{
		x = a;
		y = b;
		z = c;
	}
};

class SBSIMPEXP MeshObject
{
public:
	Object *object; //SBS object
	Ogre::String name; //mesh name

	//define geometry type
	struct Geometry
	{
		//basic 3D geometry
		Ogre::Vector3 vertex;
		Ogre::Vector2 texel;
		Ogre::Vector3 normal;
	};
	struct TriangleIndices
	{
		//per-submesh triangle indices
		std::vector<TriangleType> triangles; //triangle data, in A B C values
		Ogre::IndexData *databuffer; //used to find the related submesh
	};

	MeshObject(Object* parent, const char *name, const char *filename = 0, float max_render_distance = 0, float scale_multiplier = 1);
	~MeshObject();
	void Enable(bool value);
	bool IsEnabled();
	WallObject* CreateWallObject(Object *parent, const char *name);
	Ogre::MaterialPtr ChangeTexture(const char *texture, bool matcheck = true, int submesh = 0);
	int FindWall(const Ogre::Vector3 &point);
	void RescaleVertices(float multiplier);
	bool LoadColladaFile(const char *filename, const char *name);
	void Move(const Ogre::Vector3 position, bool relative_x, bool relative_y, bool relative_z, Ogre::Vector3 origin = Ogre::Vector3(0, 0, 0));
	Ogre::Vector3 GetPosition();
	void Rotate(const Ogre::Vector3 rotation, float speed);
	void SetRotation(const Ogre::Vector3 rotation);
	Ogre::Vector3 GetRotation();
	void AddVertex(Geometry &vertex_geom);
	void RemoveVertex(int index);
	void AddTriangle(int submesh, TriangleType &triangle);
	void RemoveTriangle(int submesh, int index);
	bool PolyMesh(const char *name, const char *texture, std::vector<Ogre::Vector3> &vertices, float tw, float th, bool autosize, Ogre::Matrix3 &tex_matrix, Ogre::Vector3 &tex_vector, std::vector<Ogre::Vector2> &mesh_indices, std::vector<TriangleType> &triangles);
	bool PolyMesh(const char *name, Ogre::String &material, std::vector<std::vector<Ogre::Vector3> > &vertices, Ogre::Matrix3 &tex_matrix, Ogre::Vector3 &tex_vector, std::vector<Ogre::Vector2> &mesh_indices, std::vector<TriangleType> &triangles, bool convert_vertices = true);
	bool ComputeTextureMap(Ogre::Matrix3 &t_matrix, Ogre::Vector3 &t_vector, std::vector<Ogre::Vector3> &vertices, const Ogre::Vector3 &p1, const Ogre::Vector2 &uv1, const Ogre::Vector3 &p2, const Ogre::Vector2 &uv2, const Ogre::Vector3 &p3, const Ogre::Vector2 &uv3);
	Ogre::Vector2* GetTexels(Ogre::Matrix3 &tex_matrix, Ogre::Vector3 &tex_vector, std::vector<std::vector<Ogre::Vector3> > &vertices);
	int ProcessSubMesh(std::vector<TriangleType> &indices, Ogre::String &material, const char *name, bool add);
	int FindMatchingSubMesh(Ogre::String material);
	void DeleteVertices(std::vector<WallObject*> &wallarray, std::vector<TriangleType> &deleted_indices);
	
	Ogre::MeshPtr MeshWrapper; //mesh
	std::vector<Geometry> MeshGeometry;
	std::vector<TriangleIndices> Triangles;
	std::vector<Ogre::SubMesh*> Submeshes;
	std::vector<WallObject*> Walls;
	Ogre::Entity *Movable;
	Ogre::SceneNode *SceneNode;

private:
	bool enabled;
	//csRef<iCollection> collection;
	float rotX, rotY, rotZ;
	bool ComputeTextureSpace(Ogre::Matrix3 &m, Ogre::Vector3 &v, const Ogre::Vector3 &v_orig, const Ogre::Vector3 &v1, float len1, const Ogre::Vector3 &v2, float len2);

	struct TriangleMesh
	{
		std::vector<TriangleType> triangles; //triangles have a, b and c components (each a vertex index)
		std::vector<Ogre::Vector3> vertices; //vertices have x, y and z components
	};
};

class SBSIMPEXP WallPolygon
{
public:
	MeshObject* mesh;
	std::vector<TriangleType> triangles;
	Ogre::Plane plane;

	//array holding index extents, to get original geometry
	std::vector<Ogre::Vector2> index_extents;

	//texture mapping matrix and vector
	Ogre::Matrix3 t_matrix;
	Ogre::Vector3 t_vector;

	Ogre::String material; //polygon material
	Ogre::String name; //polygon names

	WallPolygon();
	~WallPolygon();
	void GetTextureMapping(Ogre::Matrix3 &t_matrix, Ogre::Vector3 &t_vector);
	Ogre::SubMesh* GetSubMesh();
	void GetGeometry(MeshObject *mesh, std::vector<std::vector<Ogre::Vector3> > &vertices, bool firstonly);
	bool PointInside(MeshObject *mesh, const Ogre::Vector3 &point, bool plane_check = true);
};

#endif
