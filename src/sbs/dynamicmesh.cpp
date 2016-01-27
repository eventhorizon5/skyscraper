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
}

DynamicMesh::~DynamicMesh()
{
	for (int i = 0; i < (int)meshes.size(); i++)
		delete meshes[i];
	meshes.clear();
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

void DynamicMesh::Prepare(bool force)
{
	for (int i = 0; i < (int)meshes.size(); i++)
		meshes[i]->Prepare(force);
}

DynamicMesh::Mesh::Mesh(DynamicMesh *parent, const std::string &name, SceneNode *node, float max_render_distance)
{
	Parent = parent;
	sbs = Parent->GetRoot();

	//create mesh
	MeshWrapper = Ogre::MeshManager::getSingleton().createManual(name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	//create and attach movable
	Movable = sbs->mSceneManager->createEntity(MeshWrapper);
	//Movable->setCastShadows(true);
	node->AttachObject(Movable);
	this->node = node;

	//set maximum render distance
	Movable->setRenderingDistance(sbs->ToRemote(max_render_distance));
}

DynamicMesh::Mesh::~Mesh()
{
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

	if (submesh >= 0)
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
	Ogre::SubMesh *submesh = MeshWrapper->createSubMesh(node->GetFullName() + ":" + ToString((int)Submeshes.size()));
	submesh->useSharedVertices = true;
	Submeshes.push_back(submesh);

	//bind material
	submesh->setMaterialName(ToString(sbs->InstanceNumber) + ":" + material);

	return submesh;
}

void DynamicMesh::Mesh::Prepare(bool force)
{
	//prepare mesh object

	//this function collects stored geometry and triangle data,
	//uses those to build GPU vertex and index render buffers, and prepares the mesh for rendering
	//geometry arrays must be populated correctly before this function is called

	//all submeshes share mesh vertex data, but triangle indices are stored in each submesh
	//each submesh represents a portion of the mesh that uses the same material
/*
	//exit if mesh has already been prepared
	if (prepared == true && force == false)
		return;

	//clear vertex data and exit if there's no associated submesh or geometry data
	if (Submeshes.size() == 0 || MeshGeometry.size() == 0)
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
	data->vertexCount = MeshGeometry.size();
	Ogre::VertexDeclaration* decl = data->vertexDeclaration;

	//set up vertex data elements
	size_t offset = 0;
	decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION); //vertices
	offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
	decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL); //normals
	offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
	decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES); //texels

	//set up vertex data arrays
	unsigned int vsize = (unsigned int)MeshGeometry.size();
	float *mVertexElements = new float[vsize * 8];

	//populate array with vertex geometry
	unsigned int loc = 0;
	for (size_t i = 0; i < MeshGeometry.size(); i++)
	{
		mVertexElements[loc] = MeshGeometry[i].vertex.x;
		mVertexElements[loc + 1] = MeshGeometry[i].vertex.y;
		mVertexElements[loc + 2] = MeshGeometry[i].vertex.z;
		mVertexElements[loc + 3] = MeshGeometry[i].normal.x;
		mVertexElements[loc + 4] = MeshGeometry[i].normal.y;
		mVertexElements[loc + 5] = MeshGeometry[i].normal.z;
		mVertexElements[loc + 6] = -MeshGeometry[i].texel.x;
		mVertexElements[loc + 7] = MeshGeometry[i].texel.y;
		box.merge(MeshGeometry[i].vertex);
		radius = std::max(radius, MeshGeometry[i].vertex.length());
		loc += 8;
	}

	//create vertex hardware buffer
	Ogre::HardwareVertexBufferSharedPtr vbuffer = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(decl->getVertexSize(0), vsize, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
	vbuffer->writeData(0, vbuffer->getSizeInBytes(), mVertexElements, true);
	delete [] mVertexElements;

	//bind vertex data to mesh
	data->vertexBufferBinding->setBinding(0, vbuffer);

	//process index arrays for each submesh
	for (int index = 0; index < (int)Submeshes.size(); index++)
	{
		//get submesh object
		Ogre::SubMesh *submesh = MeshWrapper->getSubMesh(index);

		//set up index data array
		unsigned int isize = (unsigned int)Triangles[index].triangles.size() * 3;
		Ogre::HardwareIndexBufferSharedPtr ibuffer;

		//if the number of vertices is greater than what can fit in a 16-bit index, use 32-bit indexes instead
		if (vsize > 65536)
		{
			//set up a 32-bit index buffer
			unsigned int *mIndices = new unsigned int[isize];

			//create array of triangle indices
			loc = 0;
			for (size_t i = 0; i < Triangles[index].triangles.size(); i++)
			{
				mIndices[loc] = Triangles[index].triangles[i].a;
				mIndices[loc + 1] = Triangles[index].triangles[i].b;
				mIndices[loc + 2] = Triangles[index].triangles[i].c;
				loc += 3;
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
			for (size_t i = 0; i < Triangles[index].triangles.size(); i++)
			{
				mIndices[loc] = Triangles[index].triangles[i].a;
				mIndices[loc + 1] = Triangles[index].triangles[i].b;
				mIndices[loc + 2] = Triangles[index].triangles[i].c;
				loc += 3;
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

	prepared = true;
	MeshWrapper->_setBounds(box);
	MeshWrapper->_setBoundingSphereRadius(radius);*/
}

void DynamicMesh::Mesh::EnableDebugView(bool value)
{
	//enable or disable debug view of mesh
	Movable->setDebugDisplayEnabled(value);
}

bool DynamicMesh::Mesh::IsVisible()
{
	return Movable->isVisible();
}

}
