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

DynamicMesh::DynamicMesh(Object* parent, SceneNode *node, const std::string &name, float max_render_distance) : ObjectBase(parent)
{
	SetName(name);
	this->node = node;
	render_distance = max_render_distance;
	file_model = false;
	prepared = false;
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

Ogre::Entity* DynamicMesh::GetMovable()
{
	if (meshes.empty())
		return 0;

	return meshes[0]->Movable;
}

void DynamicMesh::Build()
{
	if (!node)
		return;

	//MeshWrapper->load();

	//for (int i = 0; i < (int)meshes.size(); i++)

	//if a mesh was attached and was empty, it needs to be reattached to be visible
	/*if (count == 0 && IsEnabled() == true)
	{
		GetSceneNode()->DetachObject(Movable);
		GetSceneNode()->AttachObject(Movable);
	}*/
}

void DynamicMesh::Enable(bool value)
{
	for (int i = 0; i < (int)meshes.size(); i++)
		meshes[i]->Enable(value);
}

void DynamicMesh::ChangeTexture(const std::string &old_texture, const std::string &new_texture)
{
	for (int i = 0; i < (int)meshes.size(); i++)
		meshes[i]->ChangeTexture(old_texture, new_texture);
}

void DynamicMesh::EnableDebugView(bool value)
{
	for (int i = 0; i < (int)meshes.size(); i++)
		meshes[i]->EnableDebugView(value);
}

bool DynamicMesh::IsVisible()
{
	if (meshes.empty() == true)
		return false;

	return meshes[0]->IsVisible();
}

void DynamicMesh::Prepare()
{
	if (file_model == true || prepared == true)
		return;

	for (int i = 0; i < (int)meshes.size(); i++)
		meshes[i]->Prepare();

	prepared = true;
}

void DynamicMesh::AddClient(MeshObject *mesh)
{
	//add a client mesh object to this dynamic mesh

	clients.push_back(mesh);

	//for now, create a single mesh if not created yet
	if (meshes.empty() == true)
	{
		Mesh *mesh = new Mesh(this, GetName(), node, render_distance);
		meshes.push_back(mesh);
	}
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

void DynamicMesh::NeedsUpdate()
{
	if (prepared == false)
		return;

	prepared = false;
	Prepare();
}

int DynamicMesh::GetMaterials(std::vector<std::string> &materials)
{
	//calculate the total number of materials used by all clients

	//for each client
	for (int i = 0; i < (int)clients.size(); i++)
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

unsigned int DynamicMesh::GetVertexCount()
{
	//calculate combined vertex count for all clients

	unsigned int total = 0;

	for (int i = 0; i < (int)clients.size(); i++)
	{
		total += clients[i]->GetVertexCount();
	}

	return total;
}

unsigned int DynamicMesh::GetTriangleCount(const std::string &material)
{
	//calculate combined triangle count for all clients

	unsigned int total = 0;

	for (int i = 0; i < (int)clients.size(); i++)
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

DynamicMesh::Mesh::Mesh(DynamicMesh *parent, const std::string &name, SceneNode *node, float max_render_distance, const std::string &filename, const std::string &path)
{
	Parent = parent;
	sbs = Parent->GetRoot();
	this->node = node;

	if (filename == "")
	{
		//create mesh
		MeshWrapper = Ogre::MeshManager::getSingleton().createManual(name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
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

	//create and attach movable
	Movable = sbs->mSceneManager->createEntity(MeshWrapper);
	//Movable->setCastShadows(true);
	node->AttachObject(Movable);

	//set maximum render distance
	Movable->setRenderingDistance(sbs->ToRemote(max_render_distance));
}

DynamicMesh::Mesh::~Mesh()
{
	if (MeshWrapper.get())
		Ogre::MeshManager::getSingleton().remove(MeshWrapper->getHandle());
	MeshWrapper.setNull();

	if (Movable)
	{
		node->DetachObject(Movable);
		sbs->mSceneManager->destroyEntity(Movable);
	}
	Movable = 0;
}

void DynamicMesh::Mesh::Enable(bool value)
{
	//attach or detach from scenegraph

	if (value == false)
		node->DetachObject(Movable);
	else
		node->AttachObject(Movable);
}

void DynamicMesh::Mesh::ChangeTexture(const std::string &old_texture, const std::string &new_texture)
{
	int submesh = FindMatchingSubMesh(old_texture);

	if (submesh == -1)
		return;

	MeshWrapper->getSubMesh(submesh)->setMaterialName(new_texture);

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

void DynamicMesh::Mesh::Prepare()
{
	//prepare mesh object

	//this function collects stored geometry and triangle data from all associated client meshes,
	//uses those to build GPU vertex and index render buffers, and prepares the dynamic mesh for rendering
	//geometry arrays must be populated correctly before this function is called

	//all submeshes share mesh vertex data, but triangle indices are stored in each submesh
	//each submesh represents a portion of the mesh that uses the same material

	unsigned int vertex_count = Parent->GetVertexCount();

	std::vector<std::string> materials;
	int submesh_count = Parent->GetMaterials(materials);

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
	unsigned int vsize = (unsigned int)vertex_count;
	float *mVertexElements = new float[vsize * 8];

	//populate array with vertex geometry from each client mesh
	unsigned int loc = 0;

	for (int num = 0; num < Parent->GetClientCount(); num++)
	{
		MeshObject *mesh = Parent->GetClient(num);

		//get mesh's offset of associated scene node
		Ogre::Vector3 offset = mesh->GetPosition() - node->GetPosition();

		//fill array with mesh's geometry data
		for (size_t i = 0; i < mesh->GetVertexCount(); i++)
		{
			//make mesh's vertex relative to this scene node
			Ogre::Vector3 vertex = (node->GetOrientation().Inverse() * mesh->MeshGeometry[i].vertex) + offset;

			//add elements to array
			mVertexElements[loc] = vertex.x;
			mVertexElements[loc + 1] = vertex.y;
			mVertexElements[loc + 2] = vertex.z;
			mVertexElements[loc + 3] = mesh->MeshGeometry[i].normal.x;
			mVertexElements[loc + 4] = mesh->MeshGeometry[i].normal.y;
			mVertexElements[loc + 5] = mesh->MeshGeometry[i].normal.z;
			mVertexElements[loc + 6] = -mesh->MeshGeometry[i].texel.x;
			mVertexElements[loc + 7] = mesh->MeshGeometry[i].texel.y;
			box.merge(mesh->MeshGeometry[i].vertex);
			radius = std::max(radius, mesh->MeshGeometry[i].vertex.length());
			loc += 8;
		}
	}

	//create vertex hardware buffer
	Ogre::HardwareVertexBufferSharedPtr vbuffer = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(decl->getVertexSize(0), vsize, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
	vbuffer->writeData(0, vbuffer->getSizeInBytes(), mVertexElements, true);
	delete [] mVertexElements;

	//bind vertex data to mesh
	data->vertexBufferBinding->setBinding(0, vbuffer);

	//process index arrays for each submesh
	for (int index = 0; index < submesh_count; index++)
	{
		std::string material = materials[index];
		Ogre::SubMesh *submesh = 0;
		unsigned int triangle_count = Parent->GetTriangleCount(material);

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
		if (vsize > 65536)
		{
			//set up a 32-bit index buffer
			unsigned int *mIndices = new unsigned int[isize];

			//create array of triangle indices
			loc = 0;
			for (int num = 0; num < Parent->GetClientCount(); num++)
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
						mIndices[loc] = mesh->Submeshes[index].Triangles.triangles[i].a + offset;
						mIndices[loc + 1] = mesh->Submeshes[index].Triangles.triangles[i].b + offset;
						mIndices[loc + 2] = mesh->Submeshes[index].Triangles.triangles[i].c + offset;
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
			for (int num = 0; num < Parent->GetClientCount(); num++)
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
						mIndices[loc] = mesh->Submeshes[index].Triangles.triangles[i].a + offset;
						mIndices[loc + 1] = mesh->Submeshes[index].Triangles.triangles[i].b + offset;
						mIndices[loc + 2] = mesh->Submeshes[index].Triangles.triangles[i].c + offset;
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

}
