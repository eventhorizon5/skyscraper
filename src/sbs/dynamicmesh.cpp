/* $Id$ */

/*
	Scalable Building Simulator - Dynamic Mesh
	The Skyscraper Project - Version 1.10 Alpha
	Copyright (C)2004-2016 Ryan Thoryk
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

#include <OgreSceneManager.h>
#include <OgreSubMesh.h>
#include <OgreMeshManager.h>
#include <OgreMesh.h>
#include <OgreEntity.h>
#include <OgreCamera.h>
#include "globals.h"
#include "sbs.h"
#include "mesh.h"
#include "triangle.h"
#include "texture.h"
#include "scenenode.h"
#include "dynamicmesh.h"

//this file includes function implementations of the low-level SBS geometry and mesh code

namespace SBS {

DynamicMesh::DynamicMesh(Object* parent, SceneNode *node, const std::string &name, float max_render_distance, bool dynamic_buffers) : ObjectBase(parent)
{
	//creates a new Dynamic Mesh object, which manages multiple sets of geometry data for efficiency.
	//A MeshObject is a client of this object, which submits it's geometry data to be handled automatically

	//if "dynamic_buffers" is true, a MeshObject's geometry can be part of a larger combined mesh, yet still be movable
	//and rotated independently like a regular mesh, by using dynamic (writable) render buffers, becoming a virtual mesh

	SetName(name);
	this->node = node;
	render_distance = max_render_distance;
	file_model = false;
	prepared = false;
	this->dynamic_buffers = dynamic_buffers;
	force_combine = false;
	auto_combine = sbs->GetConfigBool("Skyscraper.SBS.DynamicMesh.AutoCombine", true);
	sbs->RegisterDynamicMesh(this);
}

DynamicMesh::~DynamicMesh()
{
	for (size_t i = 0; i < meshes.size(); i++)
		delete meshes[i];
	meshes.clear();

	if (sbs->FastDelete == false)
		sbs->UnregisterDynamicMesh(this);
}

bool DynamicMesh::LoadFromFile(const std::string &filename, const std::string &path)
{
	if (meshes.empty() == false)
		return false;

	Mesh* mesh = new Mesh(this, "", node, render_distance, filename, path);

	//if load failed
	if (mesh->MeshWrapper.isNull())
	{
		delete mesh;
		return false;
	}

	meshes.push_back(mesh);
	file_model = true;
	return true;
}

void DynamicMesh::Enabled(bool value, MeshObject *client)
{
	if (client == 0 || meshes.size() == 1)
	{
		//manage client enabled status
		if (client != 0)
		{
			int index = GetClientIndex(client);
			if (index >= 0)
				client_enable[index] = value;

			//don't disable mesh if another client has it enabled
			if (value == false && (int)client_enable.size() > 1)
			{
				for (size_t i = 0; i < client_enable.size(); i++)
				{
					if (i == index)
						continue;

					if (client_enable[i] == true)
						return;
				}
			}
		}

		//enable all meshes if no client specified
		for (size_t i = 0; i < meshes.size(); i++)
			meshes[i]->Enabled(value);
	}
	else if (meshes.size() > 1)
	{
		//enable a per-client mesh if specified

		int index = GetClientIndex(client);

		if (index >= 0)
			meshes[index]->Enabled(value);
	}
}

bool DynamicMesh::ChangeTexture(const std::string &old_texture, const std::string &new_texture, MeshObject *client)
{
	if (client == 0 || meshes.size() == 1)
	{
		bool result = true;
		for (size_t i = 0; i < meshes.size(); i++)
		{
			bool change = meshes[i]->ChangeTexture(old_texture, new_texture);
			if (change == false)
				result = false;
		}
		return result;
	}
	else if (meshes.size() > 1)
	{
		int index = GetClientIndex(client);

		if (index >= 0)
			return meshes[index]->ChangeTexture(old_texture, new_texture);
	}
	return false;
}

void DynamicMesh::EnableDebugView(bool value, MeshObject *client)
{
	if (client == 0 || meshes.size() == 1)
	{
		for (size_t i = 0; i < meshes.size(); i++)
			meshes[i]->EnableDebugView(value);
	}
	else if (meshes.size() > 1)
	{
		int index = GetClientIndex(client);

		if (index >= 0)
			meshes[index]->EnableDebugView(value);
	}
}

bool DynamicMesh::IsVisible(MeshObject *client)
{
	if (meshes.empty() == true)
		return false;

	if (client == 0 || meshes.size() == 1)
		return meshes[0]->IsVisible();
	else if (meshes.size() > 1)
	{
		int index = GetClientIndex(client);

		if (index >= 0)
			return meshes[index]->IsVisible();
	}
	return false;
}

bool DynamicMesh::IsVisible(Ogre::Camera *camera, MeshObject *client)
{
	if (meshes.empty() == true)
		return false;

	if (client == 0 || meshes.size() == 1)
		return IsVisible(camera, 0);
	else if (meshes.size() > 1)
	{
		int index = GetClientIndex(client);

		if (index >= 0)
			return IsVisible(camera, index);
	}
	return false;
}

bool DynamicMesh::IsVisible(Ogre::Camera *camera, int mesh_index)
{
	if (meshes.empty() == true)
		return false;

	if (mesh_index < 0 || mesh_index >= (int)meshes.size())
		return false;

	return meshes[mesh_index]->IsVisible(camera);
}

void DynamicMesh::Prepare(MeshObject *client)
{
	if (file_model == true || prepared == true)
		return;

	if (clients.empty() == true)
		return;

	//determine if meshes should be combined or separated
	if (meshes.empty() == true)
	{
		int meshes_to_create = 0;

		if (clients.size() > 1)
		{
			int total = GetMaterialCount();
			int separate_total = 0;

			int limit = 3; //compare against 3 client meshes totaled together

			for (size_t i = 0; i < clients.size(); i++)
			{
				if (i == limit)
					break;

				separate_total += clients[i]->GetSubmeshCount();
			}

			//if combined submesh/material count is less than three separate meshes
			if (((total < separate_total || total <= 10) && auto_combine == true) || force_combine == true)
				meshes_to_create = 1; //create a single combined mesh for all clients
			else
			{
				meshes_to_create = (int)clients.size(); //create separate meshes for each client
				dynamic_buffers = false; //don't use dynamic buffers if keeping separate
			}

			std::string status;
			if (meshes_to_create == 1)
				status = "using combined";
			else
				status = "using separate";

			//print optimization report
			//if (sbs->Verbose)
				sbs->Report(this->GetName() + ": Combined: " + ToString(total) + "  - Separate (3): " + ToString(separate_total) + " - " + status);
		}
		else
			meshes_to_create = 1;

		//create mesh objects
		for (int i = 0; i < meshes_to_create; i++)
		{
			Mesh *mesh = 0;

			if (meshes_to_create == 1)
				mesh = new Mesh(this, GetName(), node, render_distance);
			else
				mesh = new Mesh(this, clients[i]->GetName(), clients[i]->GetSceneNode(), render_distance);

			meshes.push_back(mesh);
		}
	}

	if (client == 0 || meshes.size() == 1)
	{
		for (size_t i = 0; i < meshes.size(); i++)
		{
			meshes[i]->prepared = false;
			if (meshes.size() > 1)
				meshes[i]->Prepare(true, (int)i);
			else
				meshes[i]->Prepare();
		}
	}
	else if (meshes.size() > 1)
	{
		int index = GetClientIndex(client);

		if (index >= 0)
			meshes[index]->Prepare(true, index);
	}

	prepared = true;
}

void DynamicMesh::AddClient(MeshObject *mesh)
{
	//add a client mesh object to this dynamic mesh

	clients.push_back(mesh);
	client_enable.push_back(true);
}

void DynamicMesh::RemoveClient(MeshObject *mesh)
{
	//remove a client mesh from this dynamic mesh

	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i] == mesh)
		{
			clients.erase(clients.begin() + i);
			client_enable.erase(client_enable.begin() + i);
			return;
		}
	}
}

MeshObject* DynamicMesh::GetClient(int number)
{
	if (number < 0 || number >= (int)clients.size())
		return 0;

	return clients[number];
}

int DynamicMesh::GetClientIndex(MeshObject *client)
{
	if (!client)
		return -1;

	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i] == client)
			return (int)i;
	}
	return -1;
}

void DynamicMesh::NeedsUpdate(MeshObject *client)
{
	if (prepared == false)
		return;

	if (client == 0 || meshes.size() == 1)
		prepared = false;
	else if (meshes.size() > 1)
	{
		int index = GetClientIndex(client);

		if (index >= 0)
		{
			meshes[index]->prepared = false;
			prepared = false;
		}
	}

	Prepare(client);
}

int DynamicMesh::GetMaterials(std::vector<std::string> &materials, int client)
{
	//calculate the total number of materials used by all clients

	int start = 0;
	int end = (int)clients.size() - 1;

	if (client >= 0)
	{
		start = client;
		end = client;
	}

	//for each client
	for (int i = start; i <= end; i++)
	{
		//for each client submesh entry
		for (size_t j = 0; j < clients[i]->Submeshes.size(); j++)
		{
			std::string material = clients[i]->Submeshes[j].Name;

			//find material in current list
			bool found = false;
			for (size_t k = 0; k < materials.size(); k++)
			{
				if (materials[k] == material)
				{
					found = true;
					break;
				}
			}

			if (found == false)
				materials.push_back(material);
		}
	}

	return (int)materials.size();
}

int DynamicMesh::GetMaterialCount(int client)
{
	//calculate the number of materials needed for all clients or a specific client

	std::vector<std::string> materials;
	int total = GetMaterials(materials, client);
	return total;
}

unsigned int DynamicMesh::GetVertexCount(const std::string &material, int client)
{
	//calculate combined vertex count for all clients

	int start = 0;
	int end = (int)clients.size() - 1;

	if (client >= 0)
	{
		start = client;
		end = client;
	}

	unsigned int total = 0;

	for (int i = start; i <= end; i++)
	{
		if (material != "")
		{
			int index = clients[i]->FindMatchingSubMesh(material);

			if (index >= 0)
				total += clients[i]->GetVertexCount(index);
		}
		else
			total += clients[i]->GetVertexCount();
	}

	return total;
}

unsigned int DynamicMesh::GetTriangleCount(const std::string &material, int &client_count, int client)
{
	//calculate combined triangle count for all clients

	int start = 0;
	int end = (int)clients.size() - 1;

	if (client >= 0)
	{
		start = client;
		end = client;
	}

	unsigned int total = 0;
	client_count = 0;

	for (int i = start; i <= end; i++)
	{
		int index = clients[i]->FindMatchingSubMesh(material);

		if (index >= 0)
		{
			total += clients[i]->GetTriangleCount(index);
			client_count += 1;
		}
	}

	return total;
}

unsigned int DynamicMesh::GetIndexOffset(int submesh, MeshObject *client)
{
	//get vertex index offset of specific client mesh
	//if multiple geometry sets are combined together, each set has a starting index number

	unsigned int index = 0;

	if (submesh < 0 || submesh >= client->GetSubmeshCount())
		return 0;

	//get per-submesh index offset
	for (int i = 0; i < submesh; i++)
	{
		index += client->GetVertexCount(i);
	}

	//return value if using separate meshes
	if (meshes.size() > 1)
		return index;

	for (size_t i = 0; i < clients.size(); i++)
	{
		//if found, return current index value
		if (clients[i] == client)
			return index;

		//if not found, increment by client's vertex count
		index += clients[i]->GetVertexCount();
	}

	return index;
}

void DynamicMesh::UpdateVertices(MeshObject *client, const std::string &material, unsigned int index, bool single)
{
	int client_index = GetClientIndex(client);

	if (client_index == -1 || meshes.empty())
		return;

	if (meshes.size() == 1)
		meshes[0]->UpdateVertices(client_index, material, index, single);
	else
		meshes[client_index]->UpdateVertices(client_index, material, index, single);
}

void DynamicMesh::DetachClient(MeshObject *client)
{
	if (meshes.size() > 1)
	{
		int index = GetClientIndex(client);

		if (index >= 0)
			meshes[index]->Detach();
	}
}

int DynamicMesh::GetSubMeshCount(int mesh_index)
{
	if (meshes.empty() == true)
		return -1;

	if (mesh_index < 0 || mesh_index >= (int)meshes.size())
		return -1;

	return meshes[mesh_index]->GetSubMeshCount();
}

std::string DynamicMesh::GetMeshName(int mesh_index)
{
	if (meshes.empty() == true)
		return "";

	if (mesh_index < 0 || mesh_index >= (int)meshes.size())
		return "";

	return meshes[mesh_index]->name;
}

Ogre::AxisAlignedBox DynamicMesh::GetBounds(MeshObject *client)
{
	if (meshes.empty() == true)
		return Ogre::AxisAlignedBox::BOX_NULL;

	if (client == 0 || meshes.size() == 1)
		return meshes[0]->MeshWrapper->getBounds();
	else if (meshes.size() > 1)
	{
		int index = GetClientIndex(client);

		if (index >= 0)
			return meshes[index]->MeshWrapper->getBounds();
	}
	return Ogre::AxisAlignedBox::BOX_NULL;
}

DynamicMesh::Mesh::Mesh(DynamicMesh *parent, const std::string &name, SceneNode *node, float max_render_distance, const std::string &filename, const std::string &path)
{
	Parent = parent;
	sbs = Parent->GetRoot();
	this->node = node;
	enabled = false;
	prepared = false;
	Movable = 0;

	if (filename == "")
	{
		this->name = name;

		//create mesh
		MeshWrapper = Ogre::MeshManager::getSingleton().createManual(node->GetNameBase() + name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	}
	else
	{
		this->name = filename;

		//load model
		try
		{
			MeshWrapper = Ogre::MeshManager::getSingleton().load(filename, path);
		}
		catch (Ogre::Exception &e)
		{
			sbs->ReportError("Error loading model " + filename + "\n" + e.getDescription());
			MeshWrapper.setNull();
			return;
		}
	}

	//create and enable movable
	Movable = sbs->mSceneManager->createEntity(MeshWrapper);
	//Movable->setCastShadows(true);
	Enabled(true);

	//set maximum render distance
	Movable->setRenderingDistance(sbs->ToRemote(max_render_distance));
}

DynamicMesh::Mesh::~Mesh()
{
	Detach();

	for (size_t i = 0; i < client_entries.size(); i++)
	{
		delete client_entries[i].bounds;
	}
	client_entries.clear();

	if (MeshWrapper.get())
		Ogre::MeshManager::getSingleton().remove(MeshWrapper->getHandle());
	MeshWrapper.setNull();
}

void DynamicMesh::Mesh::Enabled(bool value)
{
	//attach or detach from scenegraph

	if (enabled == value || !node)
		return;

	if (value == false)
		node->DetachObject(Movable);
	else
		node->AttachObject(Movable);

	enabled = value;
}

bool DynamicMesh::Mesh::ChangeTexture(const std::string &old_texture, const std::string &new_texture)
{
	//change a texture on this mesh

	//if multiple clients are referencing the matching submesh,
	//re-prepare mesh to move client's triangles into the proper new submesh,
	//to prevent changing (and interfering with) other clients' textures

	//get new material
	Ogre::MaterialPtr newmat = sbs->GetTextureManager()->GetMaterialByName(new_texture, "General");

	if (!newmat.get())
		return sbs->ReportError("ChangeTexture: Invalid texture '" + new_texture + "'");

	int submesh = FindMatchingSubMesh(old_texture);

	if (submesh == -1)
		return false;

	bool reprepare = false;

	if (Submeshes[submesh].clients > 1)
		reprepare = true; //re-prepare if breaking out of shared mesh
	else
	{
		int existing = FindMatchingSubMesh(new_texture);

		if (existing > -1)
			reprepare = true; //re-prepare if integrating into an existing shared mesh
	}

	if (reprepare == true)
	{
		//re-prepare mesh
		prepared = false;
		Prepare(false);
		return true;
	}

	//set material if valid
	Submeshes[submesh].object->setMaterialName(ToString(sbs->InstanceNumber) + ":" + new_texture);

	//apply changes (refresh mesh state)
	MeshWrapper->_dirtyState();

	return true;
}

int DynamicMesh::Mesh::FindMatchingSubMesh(const std::string &material)
{
	//find a submesh with a matching material
	//returns array index

	std::string full_name = ToString(sbs->InstanceNumber) + ":" + material;

	for (size_t i = 0; i < Submeshes.size(); i++)
	{
		if (Submeshes[i].object->getMaterialName() == full_name)
			return (int)i;
	}
	return -1;
}

DynamicMesh::Mesh::Submesh* DynamicMesh::Mesh::CreateSubMesh(const std::string &material)
{
	if (!node)
		return 0;

	int index = (int)Submeshes.size();

	//create and add submesh
	Submesh submesh;
	submesh.object = MeshWrapper->createSubMesh(node->GetFullName() + ":" + ToString(GetSubMeshCount()));
	submesh.object->useSharedVertices = true;
	submesh.clients = 0;

	//bind material
	submesh.object->setMaterialName(ToString(sbs->InstanceNumber) + ":" + material);
	submesh.material = material;

	Submeshes.push_back(submesh);
	return &Submeshes[index];
}

void DynamicMesh::Mesh::DeleteSubMesh(int index)
{
	//delete a submesh
	//if no index is provided, delete any empty submeshes

	if (index == -1)
	{
		for (size_t i = 0; i < Submeshes.size(); i++)
		{
			bool used = false;

			for (int j = 0; j < Parent->GetClientCount(); j++)
			{
				if (Parent->GetClient(j)->FindMatchingSubMesh(Submeshes[i].material) >= 0)
				{
					used = true;
					break;
				}
			}

			if (used == false)
			{
				MeshWrapper->destroySubMesh(i);
				Submeshes.erase(Submeshes.begin() + i);
				i--;
			}
		}
	}
	else if (index >= 0 && index < (int)Submeshes.size())
	{
		MeshWrapper->destroySubMesh(index);
		Submeshes.erase(Submeshes.begin() + index);
	}
}

void DynamicMesh::Mesh::Prepare(bool process_vertices, int client)
{
	//prepare mesh object

	//this function collects stored geometry and triangle data from all associated client meshes,
	//uses those to build GPU vertex and index render buffers, and prepares the dynamic mesh for rendering
	//geometry arrays must be populated correctly before this function is called

	//all submeshes share mesh vertex data, but triangle indices are stored in each submesh
	//each submesh represents a portion of the mesh that uses the same material

	if (prepared == true || !node)
		return;

	unsigned int vertex_count = Parent->GetVertexCount("", client);

	std::vector<std::string> materials;
	int submesh_count = Parent->GetMaterials(materials, client);

	//clear vertex data and exit if there's no associated submesh or geometry data
	if (submesh_count == 0 || vertex_count == 0)
	{
		if (MeshWrapper->sharedVertexData)
			delete MeshWrapper->sharedVertexData;
		MeshWrapper->sharedVertexData = new Ogre::VertexData();

		//delete any existing submeshes
		for (size_t i = 0; i < Submeshes.size(); i++)
		{
			DeleteSubMesh((int)i);
		}

		return;
	}

	//if removing existing submeshes, use existing submesh count
	if (submesh_count < (int)Submeshes.size())
		submesh_count = (int)Submeshes.size();

	int start = 0;
	int end = Parent->GetClientCount() - 1;

	if (client > -1)
	{
		start = client;
		end = client;
	}

	size_t previous_count = 0;

	if (process_vertices == true)
	{
		//set up vertex buffer
		if (MeshWrapper->sharedVertexData)
		{
			previous_count = MeshWrapper->sharedVertexData->vertexCount;
			delete MeshWrapper->sharedVertexData;
		}
		Ogre::VertexData* data = new Ogre::VertexData();
		MeshWrapper->sharedVertexData = data;
		data->vertexCount = vertex_count;
		Ogre::VertexDeclaration* decl = data->vertexDeclaration;

		//set up vertex data elements
		size_t offset = 0;
		decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION); //vertices
		offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
		decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL); //normals
		offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
		decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES); //texels

		//set up vertex data arrays
		float *mVertexElements = new float[vertex_count * 8];

		//populate array with vertex geometry from each client mesh
		unsigned int loc = 0;
		unsigned int vindex = 0;

		//clear vertex offset, counts, and bounds tables
		for (size_t i = 0; i < client_entries.size(); i++)
		{
			delete client_entries[i].bounds;
		}
		client_entries.clear();

		for (int num = start; num <= end; num++)
		{
			MeshObject *mesh = Parent->GetClient(num);
			Ogre::AxisAlignedBox client_box;
			Ogre::Real radius = 0;

			ClientEntry entry;

			//add current client's vertex index to offset table
			entry.vertex_offset = vindex;

			//get mesh's offset of associated scene node
			Ogre::Vector3 offset = sbs->ToRemote(mesh->GetPosition() - node->GetPosition());

			//fill array with mesh's geometry data, from each submesh
			for (int index = 0; index < mesh->GetSubmeshCount(); index++)
			{
				for (size_t i = 0; i < mesh->GetVertexCount(index); i++)
				{
					MeshObject::Geometry &element = mesh->Submeshes[index].MeshGeometry[i];

					//make mesh's vertex relative to this scene node
					Ogre::Vector3 vertex;
					if (client == -1)
					{
						Ogre::Vector3 raw_vertex = mesh->GetOrientation() * element.vertex; //add mesh's rotation
						vertex = (node->GetOrientation().Inverse() * raw_vertex) + offset; //remove node's rotation and add mesh offset
					}
					else
						vertex = mesh->Submeshes[index].MeshGeometry[i].vertex;

					//add elements to array
					mVertexElements[loc] = vertex.x;
					mVertexElements[loc + 1] = vertex.y;
					mVertexElements[loc + 2] = vertex.z;
					mVertexElements[loc + 3] = element.normal.x;
					mVertexElements[loc + 4] = element.normal.y;
					mVertexElements[loc + 5] = element.normal.z;
					mVertexElements[loc + 6] = element.texel.x;
					mVertexElements[loc + 7] = element.texel.y;
					client_box.merge(vertex);
					radius = std::max(radius, vertex.length());
					loc += 8;
				}
			}

			//store client bounding box and radius
			entry.bounds = new Ogre::AxisAlignedBox(client_box);
			entry.radius = radius;

			//add client vertex count to list
			entry.vertex_count = mesh->GetVertexCount();
			vindex += entry.vertex_count;

			//store client information
			client_entries.push_back(entry);
		}

		//create vertex hardware buffer
		Ogre::HardwareVertexBufferSharedPtr vbuffer;
		if (Parent->UseDynamicBuffers() == false)
			vbuffer = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(decl->getVertexSize(0), vertex_count, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
		else
			vbuffer = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(decl->getVertexSize(0), vertex_count, Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);

		vbuffer->writeData(0, vbuffer->getSizeInBytes(), mVertexElements, true);
		delete [] mVertexElements;

		//bind vertex data to mesh
		data->vertexBufferBinding->setBinding(0, vbuffer);
	}

	//process index arrays for each submesh
	for (int index = 0; index < submesh_count; index++)
	{
		std::string material;
		if (index < (int)materials.size())
			material = materials[index];
		else
			material = "";

		Submesh *submesh;
		int client_count;
		unsigned int triangle_count = Parent->GetTriangleCount(material, client_count, client);

		//get submesh index
		int match = FindMatchingSubMesh(material);

		//skip if no triangles found
		if (triangle_count == 0)
		{
			//delete submesh if needed
			DeleteSubMesh();
			continue;
		}

		//if a match is not found, create a new submesh
		if (match == -1)
			submesh = CreateSubMesh(material);
		else
			submesh = &Submeshes[match];

		if (!submesh)
			continue;

		//skip this submesh, if old and new client counts are the same, and vertices weren't processed
		if (client_count == submesh->clients && process_vertices == false)
			continue;

		//reset submesh's client reference count
		submesh->clients = 0;

		//set up index data array
		unsigned int isize = triangle_count * 3;
		Ogre::HardwareIndexBufferSharedPtr ibuffer;

		//if the number of vertices is greater than what can fit in a 16-bit index, use 32-bit indexes instead
		if (vertex_count > 65536)
		{
			//set up a 32-bit index buffer
			unsigned int *mIndices = new unsigned int[isize];

			//create array of triangle indices
			unsigned int loc = 0;

			//for each client, get triangles for a matching client submesh
			for (int num = start; num <= end; num++)
			{
				MeshObject *mesh = Parent->GetClient(num);
				int index = mesh->FindMatchingSubMesh(material);

				if (index >= 0)
				{
					//get index offset of mesh
					unsigned int offset = Parent->GetIndexOffset(index, mesh);

					//add mesh's triangles to array and adjust for offset
					for (size_t i = 0; i < mesh->GetTriangleCount(index); i++)
					{
						Triangle &tri = mesh->Submeshes[index].Triangles[i];
						mIndices[loc] = tri.a + offset;
						mIndices[loc + 1] = tri.b + offset;
						mIndices[loc + 2] = tri.c + offset;
						loc += 3;
					}

					//increment submesh's client reference count
					submesh->clients += 1;
				}
			}

			//create index hardware buffer
			ibuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(Ogre::HardwareIndexBuffer::IT_32BIT, isize, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

			//write data to index buffer
			ibuffer->writeData(0, ibuffer->getSizeInBytes(), mIndices, true);
			delete [] mIndices;
		}
		else
		{
			//set up a 16-bit index buffer
			unsigned short *mIndices = new unsigned short[isize];

			//create array of triangle indices
			unsigned int loc = 0;

			//for each client, get triangles for a matching client submesh
			for (int num = start; num <= end; num++)
			{
				MeshObject *mesh = Parent->GetClient(num);
				int index = mesh->FindMatchingSubMesh(material);

				if (index >= 0)
				{
					//get index offset of mesh
					unsigned int offset = Parent->GetIndexOffset(index, mesh);

					//add mesh's triangles to array and adjust for offset
					for (size_t i = 0; i < mesh->GetTriangleCount(index); i++)
					{
						Triangle &tri = mesh->Submeshes[index].Triangles[i];
						mIndices[loc] = tri.a + offset;
						mIndices[loc + 1] = tri.b + offset;
						mIndices[loc + 2] = tri.c + offset;
						loc += 3;
					}

					//increment submesh's client reference count
					submesh->clients += 1;
				}
			}

			//create index hardware buffer
			ibuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(Ogre::HardwareIndexBuffer::IT_16BIT, isize, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

			//write data to index buffer
			ibuffer->writeData(0, ibuffer->getSizeInBytes(), mIndices, true);
			delete [] mIndices;
		}

		//delete any old index data
		if (submesh->object->indexData)
		{
			delete submesh->object->indexData;
			submesh->object->indexData = new Ogre::IndexData();
		}

		//bind index data to submesh
		submesh->object->indexData->indexCount = isize;
		submesh->object->indexData->indexBuffer = ibuffer;
		submesh->object->indexData->indexStart = 0;
	}

	//mark ogre mesh as dirty to update changes
	MeshWrapper->_dirtyState();

	if (process_vertices == true)
	{
		UpdateBoundingBox();

		MeshWrapper->load();

		//if a mesh was attached and was empty, it needs to be reattached to be visible
		if (previous_count == 0 && enabled == true)
		{
			Enabled(false);
			Enabled(true);
		}
	}

	prepared = true;
}

void DynamicMesh::Mesh::EnableDebugView(bool value)
{
	//enable or disable debug view of mesh
	Movable->setDebugDisplayEnabled(value);
}

bool DynamicMesh::Mesh::IsVisible()
{
	//returns true if this mesh is currently visible (enabled, and within the rendering distance)
	return Movable->isVisible();
}

bool DynamicMesh::Mesh::IsVisible(Ogre::Camera *camera)
{
	//returns if this mesh is visible in the provided camera's view frustum or not

	if (enabled == false || !camera)
		return false;

	//if beyond the max render distance
	if (IsVisible() == false)
		return false;

	if (GetSubMeshCount() == 0)
		return false;

	Ogre::AxisAlignedBox Bounds = MeshWrapper->getBounds();
	if (Bounds.isNull() == true)
		return false;

	Ogre::Vector3 min = Bounds.getMinimum();
	Ogre::Vector3 max = Bounds.getMaximum();
	Ogre::Vector3 pos = sbs->ToRemote(node->GetPosition());
	Ogre::AxisAlignedBox global_box (pos + min, pos + max);

	return camera->isVisible(global_box);
}

void DynamicMesh::Mesh::UpdateVertices(int client, const std::string &material, unsigned int index, bool single)
{
	//update/write all vertices (or a single vertex) to the render buffer, if a dynamic mesh

	if (Parent->UseDynamicBuffers() == false || !node)
		return;

	if (prepared == false)
		return;

	if (!Parent->GetClient(client))
		return;

	bool combined = true;
	if (Parent->GetMeshCount() > 1)
		combined = false;

	//get client's offset from offset table
	unsigned int loc = 0;
	if (combined == true)
		loc = client_entries[client].vertex_offset;

	//adjust if using a single vertex
	if (single == true)
		loc += index;

	MeshObject *mesh = Parent->GetClient(client);

	//get mesh's offset of associated scene node
	Ogre::Vector3 offset = sbs->ToRemote(mesh->GetPosition() - node->GetPosition());

	unsigned int vertex_count = mesh->GetVertexCount();

	//exit if client mesh is empty, or if no submeshes have been defined
	if (vertex_count == 0 || mesh->Submeshes.empty() == true)
		return;

	//set up vertex data arrays
	float *mVertexElements;
	if (single == true)
		mVertexElements = new float[8];
	else
		mVertexElements = new float[vertex_count * 8];

	if (single == false)
	{
		unsigned int count;

		if (combined == true)
			count = client_entries[client].vertex_count;
		else
			count = client_entries[0].vertex_count;

		if (count != vertex_count)
		{
			delete [] mVertexElements;
			return; //make sure vertex count is the same
		}
	}

	Ogre::AxisAlignedBox box;

	//fill array with vertex's data
	unsigned int pos = 0;
	unsigned int add = 0;

	for (size_t submesh = 0; submesh < mesh->Submeshes.size(); submesh++)
	{
		unsigned int start;
		unsigned int end;

		if (single == true)
		{
			if (mesh->Submeshes[submesh].Name != material)
				continue;

			start = index;
			end = index;
		}
		else
		{
			start = 0;
			end = mesh->Submeshes[submesh].MeshGeometry.size() - 1;
		}

		std::vector<MeshObject::Geometry> &geometry = mesh->Submeshes[submesh].MeshGeometry;

		for (unsigned int i = start; i <= end; i++)
		{
			MeshObject::Geometry &element = geometry[i];

			//make mesh's vertex relative to this scene node
			Ogre::Vector3 raw_vertex = mesh->GetOrientation() * element.vertex; //add mesh's rotation
			Ogre::Vector3 vertex2 = (node->GetOrientation().Inverse() * raw_vertex) + offset; //remove node's rotation and add mesh offset

			//add elements to array
			mVertexElements[pos] = vertex2.x;
			mVertexElements[pos + 1] = vertex2.y;
			mVertexElements[pos + 2] = vertex2.z;
			mVertexElements[pos + 3] = element.normal.x;
			mVertexElements[pos + 4] = element.normal.y;
			mVertexElements[pos + 5] = element.normal.z;
			mVertexElements[pos + 6] = element.texel.x;
			mVertexElements[pos + 7] = element.texel.y;
			box.merge(vertex2);
			pos += 8;
			add += 1;
		}
	}

	//store updated bounding box
	if (combined == true)
		*client_entries[client].bounds = box;
	else
		*client_entries[0].bounds = box;

	//get vertex data
	Ogre::VertexData* data = MeshWrapper->sharedVertexData;

	//get vertex buffer and size
	Ogre::HardwareVertexBufferSharedPtr vbuffer = data->vertexBufferBinding->getBuffer(0);
	size_t vsize = data->vertexDeclaration->getVertexSize(0);

	/*
	//lock vertex buffer for writing
	float *vdata = static_cast<float*>(vbuffer->lock(vsize * loc, vsize * add, Ogre::HardwareBuffer::HBL_NORMAL));

	//write elements
	for (int i = 0; i < add; i++)
	{
		vdata[i] = mVertexElements[i];
	}

	//unlock vertex buffer
	vbuffer->unlock();
	*/

	//write data to buffer
	vbuffer->writeData(vsize * loc, vsize * add, mVertexElements, false);

	delete [] mVertexElements;

	//update mesh bounding box
	UpdateBoundingBox();
}

void DynamicMesh::Mesh::Detach()
{
	//detach entity and scene node from this mesh object

	if (Movable)
	{
		node->DetachObject(Movable);
		sbs->mSceneManager->destroyEntity(Movable);
	}
	Movable = 0;
	node = 0;
}

int DynamicMesh::Mesh::GetSubMeshCount()
{
	return MeshWrapper->getNumSubMeshes();
}

void DynamicMesh::Mesh::UpdateBoundingBox()
{
	//set mesh's bounding box

	if (client_entries.empty() == true)
		return;

	if (Parent->GetMeshCount() == 1)
	{
		Ogre::AxisAlignedBox box;
		Ogre::Real radius = 0;

		if (Parent->GetClientCount() != (int)client_entries.size())
			return;

		for (int i = 0; i < Parent->GetClientCount(); i++)
		{
			box.merge(*client_entries[i].bounds);
			radius = std::max(radius, client_entries[i].radius);
		}

		MeshWrapper->_setBounds(box);
		MeshWrapper->_setBoundingSphereRadius(radius);
	}
	else if (Parent->GetMeshCount() > 1)
	{
		MeshWrapper->_setBounds(*client_entries[0].bounds);
		MeshWrapper->_setBoundingSphereRadius(client_entries[0].radius);
	}
}

}
