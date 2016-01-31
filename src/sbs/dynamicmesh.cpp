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

#include <OgreSubMesh.h>
#include <OgreMeshManager.h>
#include <OgreEntity.h>
#include "globals.h"
#include "sbs.h"
#include "unix.h"
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
	sbs->RegisterDynamicMesh(this);
}

DynamicMesh::~DynamicMesh()
{
	for (int i = 0; i < (int)meshes.size(); i++)
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

/*void DynamicMesh::Build()
{
	//extra code from previous system

	MeshWrapper->load();

	//if a mesh was attached and was empty, it needs to be reattached to be visible
	if (count == 0 && IsEnabled() == true)
	{
		GetSceneNode()->DetachObject(Movable);
		GetSceneNode()->AttachObject(Movable);
	}
}*/

void DynamicMesh::Enable(bool value, MeshObject *client)
{
	if (client == 0 || meshes.size() == 1)
	{
		//enable all meshes if no client specified

		for (int i = 0; i < (int)meshes.size(); i++)
			meshes[i]->Enable(value);
	}
	else if (meshes.size() > 1)
	{
		//enable a per-client mesh if specified

		int index = GetClientIndex(client);

		if (index >= 0)
			meshes[index]->Enable(value);
	}
}

void DynamicMesh::ChangeTexture(const std::string &old_texture, const std::string &new_texture, MeshObject *client)
{
	if (client == 0 || meshes.size() == 1)
	{
		for (int i = 0; i < (int)meshes.size(); i++)
			meshes[i]->ChangeTexture(old_texture, new_texture);
	}
	else if (meshes.size() > 1)
	{
		int index = GetClientIndex(client);

		if (index >= 0)
			meshes[index]->ChangeTexture(old_texture, new_texture);
	}
}

void DynamicMesh::EnableDebugView(bool value, MeshObject *client)
{
	if (client == 0 || meshes.size() == 1)
	{
		for (int i = 0; i < (int)meshes.size(); i++)
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

			for (int i = 0; i < (int)clients.size(); i++)
			{
				if (i == limit)
					break;

				separate_total += clients[i]->GetSubmeshCount();
			}

			//if combined submesh/material count is less than three separate meshes
			if (total < separate_total || total <= 10)
				meshes_to_create = 1; //create a single combined mesh for all clients
			else
				meshes_to_create = (int)clients.size(); //create separate meshes for each client

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
		for (int i = 0; i < (int)meshes.size(); i++)
		{
			meshes[i]->prepared = false;
			if (meshes.size() > 1)
				meshes[i]->Prepare(i);
			else
				meshes[i]->Prepare();
		}
	}
	else if (meshes.size() > 1)
	{
		int index = GetClientIndex(client);

		if (index >= 0)
			return meshes[index]->Prepare(index);
	}

	prepared = true;
}

void DynamicMesh::AddClient(MeshObject *mesh)
{
	//add a client mesh object to this dynamic mesh

	clients.push_back(mesh);
}

void DynamicMesh::RemoveClient(MeshObject *mesh)
{
	//remove a client mesh from this dynamic mesh

	for (int i = 0; i < (int)clients.size(); i++)
	{
		if (clients[i] == mesh)
		{
			clients.erase(clients.begin() + i);
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

	for (int i = 0; i < (int)clients.size(); i++)
	{
		if (clients[i] == client)
			return i;
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
			meshes[index]->prepared = false;
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
		for (int j = 0; j < (int)clients[i]->Submeshes.size(); j++)
		{
			std::string material = clients[i]->Submeshes[j].Name;

			//find material in current list
			bool found = false;
			for (int k = 0; k < (int)materials.size(); k++)
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

unsigned int DynamicMesh::GetVertexCount(int client)
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
		total += clients[i]->GetVertexCount();
	}

	return total;
}

unsigned int DynamicMesh::GetTriangleCount(const std::string &material, int client)
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

	for (int i = start; i <= end; i++)
	{
		int index = clients[i]->FindMatchingSubMesh(material);

		if (index >= 0)
			total += clients[i]->GetTriangleCount(index);
	}

	return total;
}

unsigned int DynamicMesh::GetIndexOffset(MeshObject *client)
{
	//get vertex index offset of specific client mesh
	//if multiple geometry sets are combined together, each set has a starting index number

	unsigned int index = 0;

	//return 0 if using separate meshes
	if (meshes.size() > 1)
		return index;

	for (int i = 0; i < (int)clients.size(); i++)
	{
		//if found, return current index value
		if (clients[i] == client)
			return index;

		//if not found, increment by client's vertex count
		index += clients[i]->GetVertexCount();
	}

	return index;
}

void DynamicMesh::UpdateVertices(MeshObject *client, unsigned int index, bool single)
{
	int client_index = GetClientIndex(client);

	if (client_index == -1 || meshes.empty())
		return;

	if (meshes.size() == 1)
		meshes[0]->UpdateVertices(client_index, index, single);
	else
		meshes[client_index]->UpdateVertices(client_index, index, single);
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
		//create mesh
		std::string Name = node->GetNameBase() + name;
		MeshWrapper = Ogre::MeshManager::getSingleton().createManual(Name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	}
	else
	{
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
	Enable(true);

	//set maximum render distance
	Movable->setRenderingDistance(sbs->ToRemote(max_render_distance));
}

DynamicMesh::Mesh::~Mesh()
{
	if (Movable)
	{
		node->DetachObject(Movable);
		sbs->mSceneManager->destroyEntity(Movable);
	}
	Movable = 0;

	if (MeshWrapper.get())
		Ogre::MeshManager::getSingleton().remove(MeshWrapper->getHandle());
	MeshWrapper.setNull();
}

void DynamicMesh::Mesh::Enable(bool value)
{
	//attach or detach from scenegraph

	if (enabled == value)
		return;

	if (value == false)
		node->DetachObject(Movable);
	else
		node->AttachObject(Movable);

	enabled = value;
}

void DynamicMesh::Mesh::ChangeTexture(const std::string &old_texture, const std::string &new_texture)
{
	int submesh = FindMatchingSubMesh(old_texture);

	if (submesh == -1)
		return;

	MeshWrapper->getSubMesh(submesh)->setMaterialName(ToString(sbs->InstanceNumber) + ":" + new_texture);

	//apply changes (refresh mesh state)
	MeshWrapper->_dirtyState();
}

int DynamicMesh::Mesh::FindMatchingSubMesh(const std::string &material)
{
	//find a submesh with a matching material
	//returns array index

	std::string full_name = ToString(sbs->InstanceNumber) + ":" + material;

	for (int i = 0; i < (int)Submeshes.size(); i++)
	{
		if (Submeshes[i]->getMaterialName() == full_name)
			return i;
	}
	return -1;
}

Ogre::SubMesh* DynamicMesh::Mesh::CreateSubMesh(const std::string &material)
{
	//create and add submesh
	Ogre::SubMesh *submesh = MeshWrapper->createSubMesh(node->GetFullName() + ":" + ToString(GetSubMeshCount()));
	submesh->useSharedVertices = true;
	Submeshes.push_back(submesh);

	//bind material
	submesh->setMaterialName(ToString(sbs->InstanceNumber) + ":" + material);

	return submesh;
}

void DynamicMesh::Mesh::Prepare(int client)
{
	//prepare mesh object

	//this function collects stored geometry and triangle data from all associated client meshes,
	//uses those to build GPU vertex and index render buffers, and prepares the dynamic mesh for rendering
	//geometry arrays must be populated correctly before this function is called

	//all submeshes share mesh vertex data, but triangle indices are stored in each submesh
	//each submesh represents a portion of the mesh that uses the same material

	if (prepared == true)
		return;

	unsigned int vertex_count = Parent->GetVertexCount(client);

	std::vector<std::string> materials;
	int submesh_count = Parent->GetMaterials(materials, client);

	//clear vertex data and exit if there's no associated submesh or geometry data
	if (submesh_count == 0 || vertex_count == 0)
	{
		if (MeshWrapper->sharedVertexData)
			delete MeshWrapper->sharedVertexData;
		MeshWrapper->sharedVertexData = new Ogre::VertexData();
		return;
	}

	Ogre::Real radius = 0;
	Ogre::AxisAlignedBox box;

	//set up vertex buffer
	if (MeshWrapper->sharedVertexData)
		delete MeshWrapper->sharedVertexData;
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

	int start = 0;
	int end = Parent->GetClientCount() - 1;

	if (client > -1)
	{
		start = client;
		end = client;
	}

	//clear vertex offset and counts tables
	offset_table.clear();
	vertex_counts.clear();

	for (int num = start; num <= end; num++)
	{
		MeshObject *mesh = Parent->GetClient(num);

		//add current client's vertex index to offset table
		offset_table.push_back(vindex);

		//get mesh's offset of associated scene node
		Ogre::Vector3 offset = sbs->ToRemote(mesh->GetPosition() - node->GetPosition());

		//fill array with mesh's geometry data
		for (size_t i = 0; i < mesh->GetVertexCount(); i++)
		{
			MeshObject::Geometry &element = mesh->MeshGeometry[i];

			//make mesh's vertex relative to this scene node
			Ogre::Vector3 vertex;
			if (client == -1)
			{
				Ogre::Vector3 raw_vertex = mesh->GetOrientation() * element.vertex; //add mesh's rotation
				vertex = (node->GetOrientation().Inverse() * raw_vertex) + offset; //remove node's rotation and add mesh offset
			}
			else
				vertex = mesh->MeshGeometry[i].vertex;

			//add elements to array
			mVertexElements[loc] = vertex.x;
			mVertexElements[loc + 1] = vertex.y;
			mVertexElements[loc + 2] = vertex.z;
			mVertexElements[loc + 3] = element.normal.x;
			mVertexElements[loc + 4] = element.normal.y;
			mVertexElements[loc + 5] = element.normal.z;
			mVertexElements[loc + 6] = element.texel.x;
			mVertexElements[loc + 7] = element.texel.y;
			box.merge(vertex);
			radius = std::max(radius, vertex.length());
			loc += 8;
		}

		//add client vertex count to list
		vertex_counts.push_back(mesh->GetVertexCount());
		vindex += mesh->GetVertexCount();
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

	//process index arrays for each submesh
	for (int index = 0; index < submesh_count; index++)
	{
		std::string material = materials[index];
		Ogre::SubMesh *submesh = 0;
		unsigned int triangle_count = Parent->GetTriangleCount(material, client);

		//skip if no triangles found
		if (triangle_count == 0)
			continue;

		//get submesh index
		int match = FindMatchingSubMesh(material);

		//if a match is not found, create a new submesh
		if (match == -1)
			submesh = CreateSubMesh(material);
		else
			submesh = Submeshes[match];

		//set up index data array
		unsigned int isize = triangle_count * 3;
		Ogre::HardwareIndexBufferSharedPtr ibuffer;

		//if the number of vertices is greater than what can fit in a 16-bit index, use 32-bit indexes instead
		if (vertex_count > 65536)
		{
			//set up a 32-bit index buffer
			unsigned int *mIndices = new unsigned int[isize];

			//create array of triangle indices
			loc = 0;
			start = 0;
			end = Parent->GetClientCount() - 1;

			if (client > -1)
			{
				start = client;
				end = client;
			}

			for (int num = start; num <= end; num++)
			{
				MeshObject *mesh = Parent->GetClient(num);
				int index = mesh->FindMatchingSubMesh(material);

				if (index >= 0)
				{
					//get index offset of mesh
					unsigned int offset = Parent->GetIndexOffset(mesh);

					//add mesh's triangles to array and adjust for offset
					for (size_t i = 0; i < mesh->GetTriangleCount(index); i++)
					{
						Triangle &tri = mesh->Submeshes[index].Triangles[i];
						mIndices[loc] = tri.a + offset;
						mIndices[loc + 1] = tri.b + offset;
						mIndices[loc + 2] = tri.c + offset;
						loc += 3;
					}
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
			loc = 0;
			start = 0;
			end = Parent->GetClientCount() - 1;

			if (client > -1)
			{
				start = client;
				end = client;
			}

			for (int num = start; num <= end; num++)
			{
				MeshObject *mesh = Parent->GetClient(num);
				int index = mesh->FindMatchingSubMesh(material);

				if (index >= 0)
				{
					//get index offset of mesh
					unsigned int offset = Parent->GetIndexOffset(mesh);

					//add mesh's triangles to array and adjust for offset
					for (size_t i = 0; i < mesh->GetTriangleCount(index); i++)
					{
						Triangle &tri = mesh->Submeshes[index].Triangles[i];
						mIndices[loc] = tri.a + offset;
						mIndices[loc + 1] = tri.b + offset;
						mIndices[loc + 2] = tri.c + offset;
						loc += 3;
					}
				}
			}

			//create index hardware buffer
			ibuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(Ogre::HardwareIndexBuffer::IT_16BIT, isize, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

			//write data to index buffer
			ibuffer->writeData(0, ibuffer->getSizeInBytes(), mIndices, true);
			delete [] mIndices;
		}

		//delete any old index data
		if (submesh->indexData)
		{
			delete submesh->indexData;
			submesh->indexData = new Ogre::IndexData();
		}

		//bind index data to submesh
		submesh->indexData->indexCount = isize;
		submesh->indexData->indexBuffer = ibuffer;
		submesh->indexData->indexStart = 0;
	}

	//mark ogre mesh as dirty to update changes
	MeshWrapper->_dirtyState();

	MeshWrapper->_setBounds(box);
	MeshWrapper->_setBoundingSphereRadius(radius);

	prepared = true;
}

void DynamicMesh::Mesh::EnableDebugView(bool value)
{
	//enable or disable debug view of mesh
	Movable->setDebugDisplayEnabled(value);
}

bool DynamicMesh::Mesh::IsVisible()
{
	//returns true if this mesh is currently visible
	return Movable->isVisible();
}

void DynamicMesh::Mesh::UpdateVertices(int client, unsigned int index, bool single)
{
	//update/write all vertices (or a single vertex) to the render buffer, if a dynamic mesh

	if (Parent->UseDynamicBuffers() == false)
		return;

	if (!Parent->GetClient(client))
		return;

	//get client's offset from offset table
	unsigned int loc = offset_table[client];

	//adjust if using a single vertex
	if (single == true)
		loc += index;

	MeshObject *mesh = Parent->GetClient(client);

	//get mesh's offset of associated scene node
	Ogre::Vector3 offset = sbs->ToRemote(mesh->GetPosition() - node->GetPosition());

	std::vector<MeshObject::Geometry> &geometry = mesh->MeshGeometry;

	//set up vertex data arrays
	float *mVertexElements;
	if (single == true)
		mVertexElements = new float[8];
	else
		mVertexElements = new float[geometry.size() * 8];

	unsigned int start = 0;
	unsigned int end = geometry.size() - 1;

	if (single == true)
	{
		start = index;
		end = index;
	}
	else if (vertex_counts[client] != geometry.size())
		return; //make sure vertex count is the same

	//fill array with vertex's data
	unsigned int pos = 0;
	unsigned int add = 0;
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
		pos += 8;
		add += 1;
	}

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
}

}
