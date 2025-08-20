/*
	Scalable Building Simulator - Mesh Object
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

#ifndef _SBS_MESH_H
#define _SBS_MESH_H

#include "triangle.h"

namespace SBS {

struct Extents
{
	unsigned int min;
	unsigned int max;

	Extents(unsigned int min, unsigned int max)
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

	MeshObject(Object* parent, const std::string &name, DynamicMesh* wrapper = 0, const std::string &filename = "", const std::string &meshname = "", Real max_render_distance = 0, Real scale_multiplier = 1, bool create_collider = true, bool dynamic_buffers = false);
	virtual ~MeshObject();
	bool Enabled(bool value);
	void EnableCollider(bool value);
	bool IsEnabled();
	Wall* CreateWallObject(const std::string &name);
	Wall* GetWallByName(std::string name);
	void Prepare(bool force = false);
	void EnableDebugView(bool value);
	void CreateColliderFromModel(int &vertex_count, Vector3* &vertices, int &index_count, unsigned long* &indices);
	void CreateBoxCollider();
	void DeleteWalls();
	void DeleteWalls(Object *parent);
	Vector3 GetPoint(const std::string &wallname, const Vector3 &start, const Vector3 &end);
	Vector3 GetWallExtents(const std::string &name, Real altitude, bool get_max);
	void OnMove(bool parent);
	void OnRotate(bool parent);
	bool IsVisible(Ogre::Camera *camera);
	bool IsPhysical();
	Vector3 GetOffset();
	void Cut(Vector3 start, Vector3 end, bool cutwalls, bool cutfloors, int checkwallnumber = 0, bool reset_check = true);
	void CutOutsideBounds(Vector3 start, Vector3 end, bool cutwalls, bool cutfloors);
	bool UsingDynamicBuffers();
	void GetBounds();
	void ChangeHeight(Real newheight);
	void EnableShadows(bool value);
	bool IsPrepared();
	void ResetPrepare();
	bool ReplaceTexture(const std::string &oldtexture, const std::string &newtexture);
	bool ChangeTexture(const std::string &texture, bool matcheck = true);
	Vector2 GetExtents(int coord, bool flip_z = false);
	Real GetHeight();
	Real HitBeam(const Vector3 &origin, const Vector3 &direction, Real max_distance);
	void CreateCollider();
	void DeleteCollider();
	Wall* FindPolygon(const std::string &name, int &index);
	bool InBoundingBox(const Vector3 &pos, bool check_y);
	DynamicMesh* GetDynamicMesh();
	void GetMeshInformation(const Ogre::Mesh* const mesh, int &vertex_count, Vector3* &vertices, int &index_count, unsigned long* &indices, Ogre::AxisAlignedBox &extents);
	unsigned int GetVertexCount();
	unsigned int GetTriangleCount(const std::string &material, bool total);
	bool LoadFromMesh(const std::string &meshname);
	void SetMaterial(const std::string& material);
	void EnablePhysics(bool value, Real restitution = 0, Real friction = 0, Real mass = 0);
	size_t GetSize();
	void RemoveTriOwner(Wall *wall);
	void RemoveTriOwner(Polygon *poly);
	void RemoveTriOwner(size_t triIndex);
	void RemoveTriOwnerFast(size_t triIndex);
	void AddTriOwner(Wall *wall, Polygon *poly);

	DynamicMesh *MeshWrapper; //dynamic mesh this mesh object uses
	std::vector<Wall*> Walls; //associated wall (polygon container) objects

	SceneNode *collider_node; //collider scenenode for box collider offsets
	Ogre::AxisAlignedBox *Bounds; //mesh bounds

	OgreBulletDynamics::RigidBody* mBody;
	OgreBulletCollisions::CollisionShape* mShape;

	std::string Filename; //filename, if a loaded model
	std::string Meshname; //name of loaded Ogre mesh (used for primitives)
	bool model_loaded; //true if a model was loaded successfully

	struct TriOwner
	{
		Wall* wall;      //owning wall
		Polygon* poly;   //owning polygon
	};

	std::vector<Vector3>   pickPositions; //flattened positions
	std::vector<uint32_t>  pickIndices;   //3 indices per triangle
	std::vector<TriOwner>  triOwners;     //size == pickIndices.size() / 3

private:
	bool enabled;
	bool is_physical;
	Real restitution, friction, mass;
	bool prepared;
	bool wrapper_selfcreate;

	bool LoadFromFile(const std::string &filename);
	bool LoadColliderModel(Ogre::MeshPtr &collidermesh);
	void CreateBoundingBox();

	Ogre::MeshPtr collidermesh;
	size_t size;
};

}

#endif
