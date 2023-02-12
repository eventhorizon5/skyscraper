/*
	Scalable Building Simulator - Dynamic Mesh
	The Skyscraper Project - Version 1.11 Alpha
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

#ifndef _SBS_DYNAMICMESH_H
#define _SBS_DYNAMICMESH_H

namespace SBS {

class SBSIMPEXP DynamicMesh : public ObjectBase
{
public:

	bool force_combine; //if set to true, forces combined meshes

	DynamicMesh(Object *parent, SceneNode *node, const std::string &name, Real max_render_distance = 0, bool dynamic_buffers = false);
	~DynamicMesh();
	void Enabled(bool value, MeshObject *client = 0);
	bool ChangeTexture(const std::string &old_texture, const std::string &new_texture, MeshObject *client = 0);
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
	unsigned int GetVertexCount(const std::string &material = "", int client = -1);
	unsigned int GetTriangleCount(const std::string &material, int &client_count, int client = -1);
	unsigned int GetIndexOffset(int submesh, MeshObject *client);
	bool UseDynamicBuffers() { return dynamic_buffers; }
	void UpdateVertices(MeshObject *client, const std::string &material = "", unsigned int index = 0, bool single = false);
	void DetachClient(MeshObject *client);
	int GetMeshCount() { return (int)meshes.size(); }
	int GetSubMeshCount(int mesh_index);
	std::string GetMeshName(int mesh_index);
	Ogre::AxisAlignedBox GetBounds(MeshObject *client = 0);
	void EnableShadows(bool value);

private:

	//raw mesh wrapper
	struct Mesh
	{
		struct Submesh;

		Mesh(DynamicMesh *parent, const std::string &name, SceneNode *node, Real max_render_distance, const std::string &filename = "", const std::string &path = "");
		~Mesh();
		void Enabled(bool value);
		bool ChangeTexture(const std::string &old_texture, const std::string &new_texture);
		int FindMatchingSubMesh(const std::string &material);
		Submesh* CreateSubMesh(const std::string &material);
		void DeleteSubMesh(int index = -1);
		void Prepare(bool process_vertices = true, int client = -1);
		void EnableDebugView(bool value);
		bool IsVisible();
		bool IsVisible(Ogre::Camera *camera);
		int GetSubMeshCount();
		void UpdateVertices(int client, const std::string &material, unsigned int index = 0, bool single = false);
		void Detach();
		void UpdateBoundingBox();
		void EnableShadows(bool value);

		struct ClientEntry
		{
			unsigned int vertex_offset;
			unsigned int vertex_count;
			Ogre::AxisAlignedBox* bounds;
			Ogre::Real radius;
		};

		struct Submesh
		{
			Ogre::SubMesh* object;
			int clients;
			std::string material;
		};

		std::string name;
		Ogre::MeshPtr MeshWrapper; //mesh
		std::vector<Submesh> Submeshes; //submeshes (per-material mesh)
		std::vector<ClientEntry> client_entries; //per-client information
		Ogre::Entity *Movable;
		SceneNode *node;
		DynamicMesh *Parent;
		::SBS::SBS *sbs;
		bool enabled;
		bool prepared;
		bool auto_shadows;
		bool parent_deleting;
	};

	std::vector<Mesh*> meshes;
	SceneNode *node;
	Real render_distance;
	bool file_model;
	std::vector<MeshObject*> clients;
	std::vector<bool> client_enable;
	bool prepared;
	bool dynamic_buffers;
	bool auto_combine;
};

}

#endif
