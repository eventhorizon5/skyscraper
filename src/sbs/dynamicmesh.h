/* $Id$ */

/*
	Scalable Building Simulator - Dynamic Mesh
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

#ifndef _SBS_DYNAMICMESH_H
#define _SBS_DYNAMICMESH_H

namespace SBS {

class MeshObject;

class SBSIMPEXP DynamicMesh : public ObjectBase
{
public:

	DynamicMesh(Object *parent, SceneNode *node, const std::string &name, float max_render_distance = 0, bool dynamic_buffers = false);
	~DynamicMesh();
	void Enable(bool value, MeshObject *client = 0);
	void ChangeTexture(const std::string &old_texture, const std::string &new_texture, MeshObject *client = 0);
	void EnableDebugView(bool value, MeshObject *client = 0);
	bool IsVisible(MeshObject *client = 0);
	bool IsVisible(Ogre::Camera *camera, MeshObject *client = 0);
	bool IsVisible(Ogre::Camera *camera, int mesh_index);
	void Prepare(MeshObject *client = 0);
	bool LoadFromFile(const std::string &filename, const std::string &path);
	void AddClient(MeshObject *mesh);
	void RemoveClient(MeshObject *mesh);
	MeshObject* GetClient(int number);
	int GetClientIndex(MeshObject *client);
	int GetClientCount() { return (int)clients.size(); }
	void NeedsUpdate(MeshObject *client = 0);
	int GetMaterials(std::vector<std::string> &materials, int client = -1);
	int GetMaterialCount(int client = -1);
	unsigned int GetVertexCount(int client = -1);
	unsigned int GetTriangleCount(const std::string &material, int client = -1);
	unsigned int GetIndexOffset(MeshObject *client);
	bool UseDynamicBuffers() { return dynamic_buffers; }
	void UpdateVertices(MeshObject *client, unsigned int index = 0, bool single = false);
	void DetachClient(MeshObject *client);
	int GetMeshCount() { return (int)meshes.size(); }
	int GetSubMeshCount(int mesh_index);
	std::string GetMeshName(int mesh_index);

private:

	//raw mesh wrapper
	struct SBSIMPEXP Mesh
	{
		Mesh(DynamicMesh *parent, const std::string &name, SceneNode *node, float max_render_distance, const std::string &filename = "", const std::string &path = "");
		~Mesh();
		void Enable(bool value);
		void ChangeTexture(const std::string &old_texture, const std::string &new_texture);
		int FindMatchingSubMesh(const std::string &material);
		Ogre::SubMesh* CreateSubMesh(const std::string &material);
		void Prepare(int client = -1);
		void EnableDebugView(bool value);
		bool IsVisible();
		bool IsVisible(Ogre::Camera *camera);
		int GetSubMeshCount();
		void UpdateVertices(int client, unsigned int index = 0, bool single = false);
		void Detach();

		std::string name;
		Ogre::MeshPtr MeshWrapper; //mesh
		std::vector<Ogre::SubMesh*> Submeshes; //submeshes (per-material mesh)
		std::vector<unsigned int> offset_table;
		std::vector<unsigned int> vertex_counts;
		Ogre::Entity *Movable;
		SceneNode *node;
		DynamicMesh *Parent;
		::SBS::SBS *sbs;
		bool enabled;
		bool prepared;
	};

	std::vector<Mesh*> meshes;
	SceneNode *node;
	float render_distance;
	bool file_model;
	std::vector<MeshObject*> clients;
	bool prepared;
	bool dynamic_buffers;
};

}

#endif
