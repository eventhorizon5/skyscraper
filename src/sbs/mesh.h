/*
	Scalable Building Simulator - Mesh Object
	The Skyscraper Project - Version 1.12 Alpha
	Copyright (C)2004-2023 Ryan Thoryk
	https://www.skyscrapersim.net
	https://sourceforge.net/projects/skyscraper/
	Contact - ryan@thoryk.com

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

#include "triangle.h"

namespace SBS {

struct Extents
{
	unsigned int min;
	unsigned int max;

	Extents(unsigned int min = 0, unsigned int max = 0)
	{
		this->min = min;
		this->max = max;
	}
};

class SBSIMPEXP MeshObject : public Object
{
public:
	std::string name; //mesh name
	bool create_collider; //set to false if collider shouldn't be automatically generated
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
	};
	struct SubMesh
	{
		std::vector<Geometry> MeshGeometry; //mesh geometry (vertices/texels/normals) container
		std::vector<Triangle> Triangles; //per-submesh triangles
		std::string Name;
	};

	MeshObject(Object* parent, const std::string &name, DynamicMesh* wrapper = 0, const std::string &filename = "", Real max_render_distance = 0, Real scale_multiplier = 1, bool enable_physics = false, Real restitution = 0, Real friction = 0, Real mass = 0, bool create_collider = true, bool dynamic_buffers = false);
	virtual ~MeshObject();
	void Enabled(bool value);
	void EnableCollider(bool value);
	bool IsEnabled();
	Wall* CreateWallObject(const std::string &name);
	Wall* GetWallByName(std::string name);
	bool ChangeTexture(const std::string &texture, bool matcheck = true, int submesh = 0);
	bool ReplaceTexture(const std::string &oldtexture, const std::string &newtexture);
	Wall* FindWallIntersect(const Ogre::Vector3 &start, const Ogre::Vector3 &end, Ogre::Vector3 &isect, Real &distance, Ogre::Vector3 &normal, Wall *wall = 0);
	bool PolyMesh(const std::string &name, const std::string &texture, std::vector<Ogre::Vector3> &vertices, Real tw, Real th, bool autosize, Ogre::Matrix3 &tex_matrix, Ogre::Vector3 &tex_vector, Extents &mesh_indices, std::vector<Triangle> &triangles, std::vector<Ogre::Vector3> &converted_vertices);
	bool PolyMesh(const std::string &name, const std::string &material, std::vector<Ogre::Vector3> &vertices, Ogre::Matrix3 &tex_matrix, Ogre::Vector3 &tex_vector, Extents &mesh_indices, std::vector<Triangle> &triangles, std::vector<Ogre::Vector3> &converted_vertices, Real tw, Real th, bool convert_vertices = true);
	Ogre::Vector2* GetTexels(Ogre::Matrix3 &tex_matrix, Ogre::Vector3 &tex_vector, std::vector<Ogre::Vector3> &vertices, Real tw, Real th);
	int ProcessSubMesh(std::vector<Geometry> &vertices, std::vector<Triangle> &indices, const std::string &material, bool add);
	int FindMatchingSubMesh(const std::string &material);
	void DeleteVertices(int submesh, std::vector<Triangle> &deleted_indices);
	void Prepare(bool force = false);
	void EnableDebugView(bool value);
	void CreateCollider();
	void DeleteCollider();
	Real HitBeam(const Ogre::Vector3 &origin, const Ogre::Vector3 &direction, Real max_distance);
	bool InBoundingBox(const Ogre::Vector3 &pos, bool check_y);
	void GetMeshInformation(const Ogre::Mesh* const mesh, int &vertex_count, Ogre::Vector3* &vertices, int &index_count, unsigned long* &indices, Ogre::AxisAlignedBox &extents);
	void CreateColliderFromModel(int &vertex_count, Ogre::Vector3* &vertices, int &index_count, unsigned long* &indices);
	void CreateBoxCollider();
	void DeleteWalls();
	void DeleteWalls(Object *parent);
	Ogre::Vector3 GetPoint(const std::string &wallname, const Ogre::Vector3 &start, const Ogre::Vector3 &end);
	Ogre::Vector3 GetWallExtents(const std::string &name, Real altitude, bool get_max);
	Ogre::Vector2 GetExtents(int coord, bool flip_z = false);
	Wall* FindPolygon(const std::string &name, int &index);
	void OnMove(bool parent);
	void OnRotate(bool parent);
	int GetSubmeshCount();
	bool IsVisible(Ogre::Camera *camera);
	bool IsPhysical();
	Ogre::Vector3 GetOffset();
	void Cut(Ogre::Vector3 start, Ogre::Vector3 end, bool cutwalls, bool cutfloors, int checkwallnumber = 0, bool reset_check = true);
	void CutOutsideBounds(Ogre::Vector3 start, Ogre::Vector3 end, bool cutwalls, bool cutfloors);
	unsigned int GetVertexCount(int submesh = -1);
	unsigned int GetTriangleCount(int submesh = -1);
	bool UsingDynamicBuffers();
	void GetBounds();
	void ChangeHeight(Real newheight);
	void EnableShadows(bool value);

	DynamicMesh *MeshWrapper; //dynamic mesh this mesh object uses
	std::vector<SubMesh> Submeshes; //submeshes
	std::vector<Wall*> Walls; //associated wall (polygon container) objects

	SceneNode *collider_node; //collider scenenode for box collider offsets

	Ogre::AxisAlignedBox *Bounds; //mesh bounds

	OgreBulletDynamics::RigidBody* mBody;
	OgreBulletCollisions::CollisionShape* mShape;

	std::string Filename; //filename, if a loaded model
	bool model_loaded; //true if a model was loaded successfully

private:
	bool enabled;
	bool is_physical;
	Real restitution, friction, mass;
	bool prepared;
	bool wrapper_selfcreate;
	bool LoadFromFile(const std::string &filename, Ogre::MeshPtr &collidermesh);
};

}

#endif
