/* $Id$ */

/*
	Scalable Building Simulator - Mesh Object
	The Skyscraper Project - Version 1.10 Alpha
	Copyright (C)2004-2016 Ryan Thoryk
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

#include "dynamicmesh.h"
#include "triangle.h"

namespace SBS {

class WallObject;

struct Extents
{
	int x;
	int y;

	Extents(int min, int max)
	{
		x = min;
		y = max;
	}
};

class SBSIMPEXP MeshObject : public Object
{
public:
	std::string name; //mesh name
	bool no_collider; //set to true if collider shouldn't be automatically generated
	bool tricollider; //collider type; box if false, triangle if true
	bool remove_on_disable; //if true (the default), remove the collider from world when disabling mesh

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
		std::vector<Triangle> triangles; //triangle data, in A B C values
		Ogre::IndexData *databuffer; //used to find the related submesh
	};
	struct SubMesh
	{
		TriangleIndices Triangles; //per-submesh triangles
		std::string Name;
	};

	MeshObject(Object* parent, const std::string &name, DynamicMesh* wrapper = 0, const std::string &filename = "", float max_render_distance = 0, float scale_multiplier = 1, bool enable_physics = false, float restitution = 0, float friction = 0, float mass = 0);
	~MeshObject();
	void Enable(bool value);
	void EnableCollider(bool value);
	bool IsEnabled();
	WallObject* CreateWallObject(const std::string &name);
	WallObject* GetWallByName(std::string name);
	bool ChangeTexture(const std::string &texture, bool matcheck = true, int submesh = 0);
	bool ReplaceTexture(const std::string &oldtexture, const std::string &newtexture);
	WallObject* FindWall(const Ogre::Vector3 &point, bool convert = true);
	WallObject* FindWallIntersect(const Ogre::Vector3 &start, const Ogre::Vector3 &end, Ogre::Vector3 &isect, float &distance, Ogre::Vector3 &normal, bool convert = true, bool rescale = true);
	void AddVertex(Geometry &vertex_geom);
	void AddTriangle(int submesh, Triangle &triangle);
	void RemoveTriangle(int submesh, int index);
	bool PolyMesh(const std::string &name, const std::string &texture, std::vector<Ogre::Vector3> &vertices, float tw, float th, bool autosize, Ogre::Matrix3 &tex_matrix, Ogre::Vector3 &tex_vector, std::vector<Extents> &mesh_indices, std::vector<Triangle> &triangles);
	bool PolyMesh(const std::string &name, const std::string &material, std::vector<std::vector<Ogre::Vector3> > &vertices, Ogre::Matrix3 &tex_matrix, Ogre::Vector3 &tex_vector, std::vector<Extents> &mesh_indices, std::vector<Triangle> &triangles, float tw, float th, bool convert_vertices = true);
	bool ComputeTextureMap(Ogre::Matrix3 &t_matrix, Ogre::Vector3 &t_vector, std::vector<Ogre::Vector3> &vertices, const Ogre::Vector3 &p1, const Ogre::Vector2 &uv1, const Ogre::Vector3 &p2, const Ogre::Vector2 &uv2, const Ogre::Vector3 &p3, const Ogre::Vector2 &uv3);
	Ogre::Vector2* GetTexels(Ogre::Matrix3 &tex_matrix, Ogre::Vector3 &tex_vector, std::vector<std::vector<Ogre::Vector3> > &vertices, float tw, float th);
	int ProcessSubMesh(std::vector<Triangle> &indices, const std::string &material, bool add);
	int FindMatchingSubMesh(const std::string &material);
	void DeleteVertices(std::vector<Triangle> &deleted_indices);
	void Prepare(bool force = false);
	void EnableDebugView(bool value);
	void CreateCollider();
	void DeleteCollider();
	float HitBeam(const Ogre::Vector3 &origin, const Ogre::Vector3 &direction, float max_distance);
	bool InBoundingBox(const Ogre::Vector3 &pos, bool check_y);
	void GetMeshInformation(const Ogre::Mesh* const mesh, int &vertex_count, Ogre::Vector3* &vertices, int &index_count, unsigned long* &indices, Ogre::AxisAlignedBox &extents);
	void CreateColliderFromModel(int &vertex_count, Ogre::Vector3* &vertices, int &index_count, unsigned long* &indices);
	void CreateBoxCollider();
	void DeleteWalls();
	void DeleteWalls(Object *parent);
	Ogre::Vector3 GetPoint(const std::string &wallname, const Ogre::Vector3 &start, const Ogre::Vector3 &end);
	Ogre::Vector3 GetWallExtents(const std::string &name, float altitude, bool get_max);
	Ogre::Vector2 GetExtents(int coord, bool flip_z = false);
	WallObject* FindPolygon(const std::string &name, int &index);
	void OnMove(bool parent);
	void OnRotate(bool parent);
	int GetSubmeshCount();
	bool IsVisible(Ogre::Camera *camera);
	bool IsPhysical();
	Ogre::Vector3 GetOffset();
	void Cut(Ogre::Vector3 start, Ogre::Vector3 end, bool cutwalls, bool cutfloors, int checkwallnumber = 0, bool reset_check = true);
	void CutOutsideBounds(Ogre::Vector3 start, Ogre::Vector3 end, bool cutwalls, bool cutfloors);

	DynamicMesh *MeshWrapper; //dynamic mesh this mesh object uses
	std::vector<Geometry> MeshGeometry; //mesh geometry (vertices/texels/normals) container
	std::vector<SubMesh> Submeshes; //submeshes
	std::vector<WallObject*> Walls; //associated wall (polygon container) objects

	SceneNode *collider_node; //collider scenenode for box collider offsets

	Ogre::AxisAlignedBox Bounds; //mesh bounds

	OgreBulletDynamics::RigidBody* mBody;
	OgreBulletCollisions::CollisionShape* mShape;

	std::string Filename; //filename, if a loaded model
	bool model_loaded; //true if a model was loaded successfully

private:
	bool enabled;
	bool is_physical;
	float restitution, friction, mass;
	bool prepared;
	bool wrapper_selfcreate;
	bool ComputeTextureSpace(Ogre::Matrix3 &m, Ogre::Vector3 &v, const Ogre::Vector3 &v_orig, const Ogre::Vector3 &v1, float len1, const Ogre::Vector3 &v2, float len2);
	bool LoadFromFile(const std::string &filename, Ogre::MeshPtr &collidermesh);
};

}

#endif
