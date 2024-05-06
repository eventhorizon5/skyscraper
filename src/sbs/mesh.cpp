/*
	Scalable Building Simulator - Mesh Object
	The Skyscraper Project - Version 1.12 Alpha
	Copyright (C)2004-2024 Ryan Thoryk
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

#include <OgreMesh.h>
#include <OgreResourceGroupManager.h>
#include <OgreMaterialManager.h>
#include <OgreBulletDynamicsRigidBody.h>
#include <OgreMath.h>
#include <OgreAxisAlignedBox.h>
#include <Shapes/OgreBulletCollisionsTrimeshShape.h>
#include <Shapes/OgreBulletCollisionsBoxShape.h>
#include "globals.h"
#include "sbs.h"
#include "camera.h"
#include "wall.h"
#include "texture.h"
#include "profiler.h"
#include "scenenode.h"
#include "dynamicmesh.h"
#include "polymesh.h"
#include "mesh.h"

namespace SBS {

MeshObject::MeshObject(Object* parent, const std::string &name, DynamicMesh* wrapper, const std::string &filename, Real max_render_distance, Real scale_multiplier, bool enable_physics, Real restitution, Real friction, Real mass, bool create_collider, bool dynamic_buffers) : Object(parent)
{
	//set up SBS object
	SetValues("Mesh", name, true);

	//create an instance of the geometry processor
	polymesh = new PolyMesh(this);

	//initialize mesh object
	enabled = true;
	mBody = 0;
	mShape = 0;
	prepared = false;
	is_physical = enable_physics;
	this->restitution = restitution;
	this->friction = friction;
	this->mass = mass;
	this->create_collider = create_collider;
	collider_node = 0;
	Filename = filename;
	remove_on_disable = true;
	wrapper_selfcreate = false;
	model_loaded = false;
	Bounds = new Ogre::AxisAlignedBox();

	//use box collider if physics should be enabled
	if (is_physical == true)
		tricollider = false;
	else
		tricollider = true;

	Ogre::MeshPtr collidermesh;

	std::string Name = GetSceneNode()->GetFullName();
	this->name = Name;

	if (wrapper == 0)
	{
		wrapper_selfcreate = true;
		MeshWrapper = new DynamicMesh(this, GetSceneNode(), name, max_render_distance, dynamic_buffers);
	}
	else
		MeshWrapper = wrapper;

	//add this mesh object as a client to the dynamic mesh wrapper
	MeshWrapper->AddClient(this);

	//load mesh from a file if specified
	if (filename != "")
	{
		bool result = LoadFromFile(filename, collidermesh);

		if (result == false)
			return;
	}

	//rescale mesh
	//file-loaded meshes need to be converted to a remote scale, since they're not pre-scaled
	if (filename == "")
		GetSceneNode()->SetScale(scale_multiplier);
	else
		GetSceneNode()->SetScale(sbs->ToRemote(scale_multiplier));

	sbs->AddMeshHandle(this);

	//set up collider for model (if mesh loaded from a filename)
	if (filename != "" && create_collider == true)
	{
		if (collidermesh.get())
		{
			//create collider based on provided mesh collider
			int vertex_count, index_count;
			Vector3* vertices;
			long unsigned int* indices;
			Ogre::AxisAlignedBox box = Ogre::AxisAlignedBox::BOX_NULL;
			polymesh->GetMeshInformation(collidermesh.get(), vertex_count, vertices, index_count, indices, box);
			CreateColliderFromModel(vertex_count, vertices, index_count, indices);
			delete[] vertices;
			delete[] indices;
			Ogre::MeshManager::getSingleton().remove(collidermesh->getHandle());
		}
		else
		{
			//create generic box collider if separate mesh collider isn't available
			GetBounds();
			CreateBoxCollider();
		}
	}
}

MeshObject::~MeshObject()
{
	//delete physics/collider components
	polymesh->DeleteCollider();

	//delete wall objects
	DeleteWalls();

	if (collider_node)
		delete collider_node;
	collider_node = 0;

	MeshWrapper->DetachClient(this);

	if (sbs->FastDelete == false)
	{
		MeshWrapper->RemoveClient(this);
		sbs->DeleteMeshHandle(this);
	}

	//delete dynamic mesh wrapper if needed
	if (wrapper_selfcreate == true)
		delete MeshWrapper;
	MeshWrapper = 0;

	if (Bounds)
		delete Bounds;
	Bounds = 0;

	delete polymesh;
}

void MeshObject::GetBounds()
{
	*Bounds = MeshWrapper->GetBounds(this);
}

void MeshObject::Enabled(bool value)
{
	//enables or disables the mesh

	if (value == enabled)
	{
		if (remove_on_disable == false && sbs->camera->IsActive() == false && value == false)
		{
			//if camera is detached from this engine, and mesh is disabled, detach a persistent collider
			remove_on_disable = true;
			EnableCollider(false);
			remove_on_disable = false;
		}
		return;
	}

	SBS_PROFILE("MeshObject::Enable");

	MeshWrapper->Enabled(value, this);

	EnableCollider(value);

	enabled = value;
}

void MeshObject::EnableCollider(bool value)
{
	//enable or disable collision detection//#include "polygon.h"


	if (!mBody)
		return;

	SBS_PROFILE("MeshObject::EnableCollider");

	if (remove_on_disable == false && sbs->camera->IsActive() == true)
	{
		//if removed from world and 'remove_on_disable' is false, add to world
		if (mBody->isInWorld() == false)
			mBody->addToWorld();

		//disable collisions on object only (don't remove)
		mBody->enableCollisions(value);
	}
	else
	{
		//completely remove object from dynamics world if disabled; re-add to enable
		if (value == false)
			mBody->removeFromWorld();
		else
			mBody->addToWorld();
	}
	sbs->camera->ResetCollisions();
}

Wall* MeshObject::CreateWallObject(const std::string &name)
{
	//create a new wall object in the given array

	Wall *wall = new Wall(this);
	wall->SetParentArray(Walls);
	wall->SetValues("Wall", name, false, false);
	Walls.push_back(wall);
	return wall;
}

Wall* MeshObject::GetWallByName(std::string name)
{
	//find a wall object by name

	SetCase(name, false);

	for (size_t i = 0; i < Walls.size(); i++)
	{
		if (name == SetCaseCopy(Walls[i]->GetName(), false) == true)
			return Walls[i];
	}
	return 0;
}

bool MeshObject::IsEnabled()
{
	return enabled;
}

void MeshObject::Prepare(bool force)
{
	//prepare mesh object

	if (prepared == true && force == false)
		return;

	//set up bounding box
	if (model_loaded == false)
	{
		for (size_t i = 0; i < polymesh->Submeshes.size(); i++)
		{
			for (size_t j = 0; j < polymesh->Submeshes[i].MeshGeometry.size(); j++)
				Bounds->merge(polymesh->Submeshes[i].MeshGeometry[j].vertex);
		}
	}

	//update dynamic mesh
	MeshWrapper->NeedsUpdate(this);

	prepared = true;
}

void MeshObject::EnableDebugView(bool value)
{
	//enable or disable debug view of mesh
	MeshWrapper->EnableDebugView(value, this);
}

void MeshObject::CreateColliderFromModel(int &vertex_count, Vector3* &vertices, int &index_count, unsigned long* &indices)
{
	//set up triangle collider based on loaded model geometry

	if (create_collider == false)
		return;

	//exit of collider already exists
	if (mBody)
		return;

	if (!GetSceneNode())
		return;

	try
	{
		//initialize collider shape
		OgreBulletCollisions::TriangleMeshCollisionShape* shape = new OgreBulletCollisions::TriangleMeshCollisionShape(vertex_count, index_count);

		//add vertices to shape
		for (int i = 0; i < index_count; i += 3)
		{
			shape->AddTriangle(vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]]);
		}

		//finalize shape
		shape->Finish();

		//create a collider scene node
		if (!collider_node)
			collider_node = GetSceneNode()->CreateChild(GetName() + " collider");

		//physics is not supported on triangle meshes; use CreateBoxCollider instead
		mBody = new OgreBulletDynamics::RigidBody(name, sbs->mWorld);
		mBody->setStaticShape(collider_node->GetRawSceneNode(), shape, 0.1f, 0.5f, false);
		mShape = shape;
	}
	catch (Ogre::Exception &e)
	{
		ReportError("Error creating model collider for '" + name + "'\n" + e.getDescription());
	}
}

void MeshObject::CreateBoxCollider()
{
	//set up a box collider for full extents of a mesh

	if (create_collider == false)
		return;

	//exit of collider already exists
	if (mBody)
		return;

	if (!GetSceneNode())
		return;

	try
	{
		//initialize collider shape
		Real scale = GetSceneNode()->GetScale();

		if (Bounds->isNull() == true)
			return;

		Vector3 bounds = Bounds->getHalfSize() * scale;
		OgreBulletCollisions::BoxCollisionShape* shape = new OgreBulletCollisions::BoxCollisionShape(bounds);

		//create a new scene node for this collider, and center the collider accordingly
		Vector3 collider_center = sbs->ToLocal(Bounds->getCenter());
		if (!collider_node)
			collider_node = GetSceneNode()->CreateChild(GetName() + " collider", collider_center);

		mBody = new OgreBulletDynamics::RigidBody(name, sbs->mWorld);
		if (IsPhysical() == false)
			mBody->setStaticShape(collider_node->GetRawSceneNode(), shape, 0.1f, 0.5f, false);
		else
			mBody->setShape(collider_node->GetRawSceneNode(), shape, restitution, friction, mass);
		mShape = shape;
	}
	catch (Ogre::Exception &e)
	{
		ReportError("Error creating box collider for '" + name + "'\n" + e.getDescription());
	}
}

void MeshObject::DeleteWalls()
{
	//delete all wall objects

	for (size_t i = 0; i < Walls.size(); i++)
	{
		Wall *wall = Walls[i];
		if (wall)
		{
			wall->parent_deleting = true;
			if (sbs->FastDelete == false)
				wall->DeletePolygons();
			delete wall;
			Walls[i] = 0;
		}
	}
	Walls.clear();
}

void MeshObject::DeleteWalls(Object *parent)
{
	//delete walls of specified parent object

	for (size_t i = 0; i < Walls.size(); i++)
	{
		Wall *wall = Walls[i];
		if (wall)
		{
			if (wall->GetParent() == parent)
			{
				wall->parent_deleting = true;
				wall->DeletePolygons();
				delete wall;
				Walls.erase(Walls.begin() + i);
				i--;
			}
		}
	}
}

Vector3 MeshObject::GetPoint(const std::string &wallname, const Vector3 &start, const Vector3 &end)
{
	//do a line intersection with a specified wall associated with this mesh object,
	//and return the intersection point

	Wall *wall = GetWallByName(wallname);

	if (wall)
		return wall->GetPoint(start, end);

	return Vector3(0, 0, 0);
}

Vector3 MeshObject::GetWallExtents(const std::string &name, Real altitude, bool get_max)
{
	//return the X and Z extents of a standard wall (by name) at a specific altitude, by doing a double plane cut

	Wall *wall = GetWallByName(name);

	if (wall)
		return wall->GetWallExtents(altitude, get_max);

	return Vector3(0, 0, 0);
}

void MeshObject::OnMove(bool parent)
{
	if (collider_node)
		collider_node->Update();

	if (mBody)
		mBody->updateTransform(true, false, false);

	if (UsingDynamicBuffers() == true)
		MeshWrapper->UpdateVertices(this);
}

void MeshObject::OnRotate(bool parent)
{
	if (collider_node)
		collider_node->Update();

	if (mBody)
	{
		if (parent == true)
			OnMove(parent); //update position if parent object has been rotated

		mBody->updateTransform(false, true, false);
	}

	if (UsingDynamicBuffers() == true)
		MeshWrapper->UpdateVertices(this);
}

bool MeshObject::IsVisible(Ogre::Camera *camera)
{
	//returns if this mesh object is visible in the provided camera's view frustum or not

	if (IsEnabled() == false || !camera)
		return false;

	//if beyond the max render distance
	if (MeshWrapper->IsVisible(this) == false)
		return false;

	if (polymesh->GetSubmeshCount() == 0)
		return false;

	if (Bounds->isNull() == true)
		return false;

	Vector3 min = Bounds->getMinimum();
	Vector3 max = Bounds->getMaximum();
	Vector3 pos = sbs->ToRemote(GetPosition());
	Ogre::AxisAlignedBox global_box (pos + min, pos + max);

	return camera->isVisible(global_box);
}

bool MeshObject::IsPhysical()
{
	return is_physical;
}

Vector3 MeshObject::GetOffset()
{
	//for models, return bounding box offset value, used to center the mesh

	Bounds->scale(GetSceneNode()->GetRawSceneNode()->getScale());

	if (Bounds->isNull() == true)
		return Vector3::ZERO;

	Vector3 vec = Bounds->getCenter();
	Vector3 min = Bounds->getMinimum();
	Vector3 offset (vec.x, -Bounds->getMinimum().y, -vec.z);
	return sbs->ToLocal(offset);
}

void MeshObject::Cut(Vector3 start, Vector3 end, bool cutwalls, bool cutfloors, int checkwallnumber, bool reset_check)
{
	//cut all walls in this mesh object

	for (size_t i = 0; i < Walls.size(); i++)
		sbs->Cut(Walls[i], start, end, cutwalls, cutfloors, checkwallnumber, reset_check);
}

void MeshObject::CutOutsideBounds(Vector3 start, Vector3 end, bool cutwalls, bool cutfloors)
{
	Real limit = 1000000;

	Vector3 left_min (-limit, -limit, -limit);
	Vector3 left_max (start.x, limit, limit);
	Vector3 right_min (end.x, -limit, -limit);
	Vector3 right_max (limit, limit, limit);
	Vector3 front_min (-limit, -limit, -limit);
	Vector3 front_max (limit, limit, start.z);
	Vector3 back_min (-limit, -limit, end.z);
	Vector3 back_max (limit, limit, limit);

	Cut(left_min, left_max, cutwalls, cutfloors);
	Cut(right_min, right_max, cutwalls, cutfloors);
	Cut(front_min, front_max, cutwalls, cutfloors);
	Cut(back_min, back_max, cutwalls, cutfloors);
}

bool MeshObject::LoadFromFile(const std::string &filename, Ogre::MeshPtr &collidermesh)
{
	//load mesh object from a file

	if (sbs->Headless == true)
		return true;

	std::string filename1 = "data/";
	filename1.append(filename);
	std::string filename2 = sbs->VerifyFile(filename1);
	std::string path = sbs->GetMountPath(filename2, filename2);
	std::string matname;

	//load material file
	try
	{
		matname = filename2.substr(0, filename2.length() - 5) + ".material";
		std::string matname2 = sbs->VerifyFile(matname);
		Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(matname2, path);
		Report("Loading material script " + matname2);
		Ogre::MaterialManager::getSingleton().parseScript(stream, path);

		if(stream)
		{
			stream->seek(0);
			while(!stream->eof())
			{
				std::string line = stream->getLine();
				TrimString(line);
				if (StartsWith(line, "material", true) == true)
				{
					std::vector<std::string> vec = Ogre::StringUtil::split(line," \t:");
					for (std::vector<std::string>::iterator it = vec.begin(); it < vec.end(); ++it)
					{
						std::string match = (*it);
						TrimString(match);
						if (!match.empty())
						{
							Ogre::MaterialPtr materialPtr = Ogre::MaterialManager::getSingleton().getByName(match, path);
							if (materialPtr)
							{
								Report("Loading material " + match);
								//materialPtr->compile();
								materialPtr->load();

								//set lighting
								materialPtr->setLightingEnabled(false);
								if (sbs->GetConfigBool("Skyscraper.SBS.Lighting", false) == true)
								{
									materialPtr->setLightingEnabled(true);
									materialPtr->setAmbient(sbs->AmbientR, sbs->AmbientG, sbs->AmbientB);
								}
							}
						}
					}
				}
			}
			stream->close();
		}
	}
	catch (Ogre::Exception &e)
	{
		ReportError("Error loading material file " + matname + "\n" + e.getDescription());
	}

	//load model
	bool status = MeshWrapper->LoadFromFile(filename2, path);
	if (status == false)
		return false;

	//load collider model if physics is disabled
	if (is_physical == false)
	{
		std::string colname2;

		try
		{
			std::string colname = filename2.substr(0, filename2.length() - 5) + ".collider.mesh";
			colname2 = sbs->VerifyFile(colname);
			collidermesh = Ogre::MeshManager::getSingleton().load(colname2, path);
		}
		catch (Ogre::Exception &e)
		{
			ReportError("No collider model for " + colname2 + "\n" + e.getDescription());
		}
	}

	model_loaded = true;
	return true;
}

bool MeshObject::UsingDynamicBuffers()
{
	return MeshWrapper->UseDynamicBuffers();
}

void MeshObject::ChangeHeight(Real newheight)
{
	//change height of all walls associated with this mesh object

	SBS_PROFILE("MeshObject::ChangeHeight");

	for (size_t i = 0; i < Walls.size(); i++)
	{
		Walls[i]->ChangeHeight(newheight);
	}
}

void MeshObject::EnableShadows(bool value)
{
	//enable shadows

	MeshWrapper->EnableShadows(value);
}

PolyMesh* MeshObject::GetPolyMesh()
{
	return polymesh;
}

bool MeshObject::IsPrepared()
{
	return prepared;
}

void MeshObject::ResetPrepare()
{
	prepared = false;
}

bool MeshObject::ReplaceTexture(const std::string &oldtexture, const std::string &newtexture)
{
	return polymesh->ReplaceTexture(oldtexture, newtexture);
}

bool MeshObject::ChangeTexture(const std::string &texture, bool matcheck, int submesh)
{
	return polymesh->ChangeTexture(texture, matcheck, submesh);
}

Vector2 MeshObject::GetExtents(int coord, bool flip_z)
{
	return polymesh->GetExtents(coord, flip_z);
}

}
