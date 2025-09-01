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
#include "texman.h"
#include "profiler.h"
#include "scenenode.h"
#include "dynamicmesh.h"
#include "polymesh.h"
#include "polygon.h"
#include "utility.h"
#include "mesh.h"

namespace SBS {

MeshObject::MeshObject(Object* parent, const std::string &name, DynamicMesh* wrapper, const std::string &filename, const std::string &meshname, Real max_render_distance, Real scale_multiplier, bool create_collider, bool dynamic_buffers) : Object(parent)
{
	//set up SBS object
	SetValues("Mesh", name, true);

	//initialize mesh object
	enabled = true;
	mBody = 0;
	mShape = 0;
	prepared = false;
	is_physical = false;
	restitution = 0;
	friction = 0;
	mass = 0;
	this->create_collider = create_collider;
	collider_node = 0;
	Filename = filename;
	Meshname = meshname;
	remove_on_disable = true;
	wrapper_selfcreate = false;
	model_loaded = false;
	Bounds = new Ogre::AxisAlignedBox();
	collidermesh = 0;
	size = 0;
	tricollider = true;

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

	if (filename != "")
	{
		//load mesh from a file if specified
		bool result = LoadFromFile(filename);

		if (result == false)
			return;
	}
	else if (meshname != "")
	{
		//load mesh from a preexisting name if specified
		bool result = LoadFromMesh(meshname);

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
}

MeshObject::~MeshObject()
{
	//delete physics/collider components
	DeleteCollider();

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
}

void MeshObject::GetBounds()
{
	*Bounds = MeshWrapper->GetBounds(this);
}

bool MeshObject::Enabled(bool value)
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
		return true;
	}

	SBS_PROFILE("MeshObject::Enable");

	bool status = MeshWrapper->Enabled(value, this);

	EnableCollider(value);

	enabled = value;
	return status;
}

void MeshObject::EnableCollider(bool value)
{
	//enable or disable collision detection

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

	Wall *wall = new Wall(this, name);
	Walls.emplace_back(wall);
	return wall;
}

Wall* MeshObject::GetWallByName(std::string name)
{
	//find a wall object by name

	SetCase(name, false);

	for (size_t i = 0; i < Walls.size(); i++)
	{
		if (!Walls[i])
			continue;

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

	SBS_PROFILE("MeshObject::Prepare");

	if (prepared == true && force == false)
		return;

	//set up bounding box
	CreateBoundingBox();

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

	//if (polymesh->GetSubmeshCount() == 0)
		//return false;

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
	{
		if (!Walls[i])
			continue;

		sbs->GetPolyMesh()->Cut(Walls[i], start, end, cutwalls, cutfloors, checkwallnumber, reset_check);
	}
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

bool MeshObject::LoadFromFile(const std::string &filename)
{
	//load mesh object from a file

	std::string filename1 = "data/";
	filename1.append(filename);
	std::string filename2 = sbs->GetUtility()->VerifyFile(filename1);
	std::string path = sbs->GetUtility()->GetMountPath(filename2, filename2);
	std::string matname;

	//load material file
	try
	{
		matname = filename2.substr(0, filename2.length() - 5) + ".material";
		std::string matname2 = sbs->GetUtility()->VerifyFile(matname);
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

	model_loaded = true;
	return true;
}

bool MeshObject::LoadColliderModel(Ogre::MeshPtr &collidermesh)
{
	if (Filename == "")
		return false;

	std::string filename1 = "data/";
	filename1.append(Filename);
	std::string filename2 = sbs->GetUtility()->VerifyFile(filename1);
	std::string path = sbs->GetUtility()->GetMountPath(filename2, filename2);

	//load collider model if physics is disabled
	if (is_physical == false)
	{
		std::string colname2;

		try
		{
			std::string colname = filename2.substr(0, filename2.length() - 5) + ".collider.mesh";
			colname2 = sbs->GetUtility()->VerifyFile(colname);
			collidermesh = Ogre::MeshManager::getSingleton().load(colname2, path);
		}
		catch (Ogre::Exception &e)
		{
			return ReportError("No collider model for " + colname2 + "\n" + e.getDescription());
		}
	}
	return true;
}

bool MeshObject::LoadFromMesh(const std::string &meshname)
{
	//load mesh
	return MeshWrapper->LoadFromMesh(meshname);
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
		if (!Walls[i])
			continue;

		Walls[i]->ChangeHeight(newheight);
	}
}

void MeshObject::EnableShadows(bool value)
{
	//enable shadows

	MeshWrapper->EnableShadows(value);
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
	//replace polygon materials named oldtexture with newtexture

	//update associated polygons
	bool found = false;
	for (size_t i = 0; i < Walls.size(); i++)
	{
		if (!Walls[i])
			continue;

		bool result = Walls[i]->ReplaceTexture(oldtexture, newtexture);
		if (result == true)
			found = true;
	}
	return found;
}

bool MeshObject::ChangeTexture(const std::string &texture, bool matcheck)
{
	//changes a texture
	//if matcheck is true, exit if old and new textures are the same

	//update associated polygons
	bool found = false;
	for (size_t i = 0; i < Walls.size(); i++)
	{
		if (!Walls[i])
			continue;

		bool result = Walls[i]->ChangeTexture(texture, matcheck);
		if (result == true)
			found = true;
	}
	return found;
}

Real MeshObject::GetHeight()
{
	//returns the height of the mesh

	Real y = 0.0;
	for (size_t i = 0; i < Walls.size(); i++)
	{
		if (!Walls[i])
			continue;

		for (size_t j = 0; j < Walls[i]->GetPolygonCount(); j++)
		{
			Polygon *poly = Walls[i]->GetPolygon(j);

			if (!poly)
				continue;

			for (size_t k = 0; k < poly->geometry.size(); k++)
			{
				for (size_t l = 0; l < poly->geometry[k].size(); l++)
				{
					Real new_y = poly->geometry[k][l].vertex.y;
					if (new_y > y)
						y = new_y;
				}
			}
		}
	}

	return sbs->ToLocal(y);
}

Real MeshObject::HitBeam(const Vector3 &origin, const Vector3 &direction, Real max_distance)
{
	//cast a ray and return the collision distance to the mesh
	//return -1 if no hit

	//cast a ray from the camera position downwards
	SBS_PROFILE("MeshObject::HitBeam");

	Vector3 position = sbs->ToRemote(origin - GetPosition());
	Ray ray (position, sbs->ToRemote(direction, false));

	for (size_t i = 0; i < Walls.size(); i++)
	{
		if (!Walls[i])
			continue;

		for (size_t j = 0; j < Walls[i]->GetPolygonCount(); j++)
		{
			Polygon *poly = Walls[i]->GetPolygon(j);

			if (!poly)
				continue;

			for (size_t k = 0; k < poly->triangles.size(); k++)
			{
				const Triangle &tri = poly->triangles[k];
				Vector3 tri_a = poly->GetVertex(tri.a);
				Vector3 tri_b = poly->GetVertex(tri.b);
				Vector3 tri_c = poly->GetVertex(tri.c);

				std::pair<bool, Real> result = Ogre::Math::intersects(ray, tri_a, tri_b, tri_c);
				if (result.first == true)
				{
					if (result.second <= sbs->ToRemote(max_distance))
						return sbs->ToLocal(result.second);
				}
			}
		}
	}
	return -1;
}

void MeshObject::CreateCollider()
{
	//set up triangle collider based on raw SBS mesh geometry

	SBS_PROFILE("MeshObject::CreateCollider");

	if (create_collider == false)
		return;

	//exit if collider already exists
	if (mBody)
		return;

	if (!GetSceneNode())
		return;

	//exit if mesh is empty
	if (Walls.size() == 0)
		return;

	unsigned int tricount = GetTriangleCount("", true);
	unsigned int vcount = GetVertexCount();

	try
	{
		//initialize collider shape
		OgreBulletCollisions::TriangleMeshCollisionShape* shape = new OgreBulletCollisions::TriangleMeshCollisionShape(vcount, tricount * 3);

		Real scale = GetSceneNode()->GetScale();

		//add vertices to shape

		int additions = 0;
		for (size_t i = 0; i < Walls.size(); i++)
		{
			if (!Walls[i])
				continue;

			for (size_t j = 0; j < Walls[i]->GetPolygonCount(); j++)
			{
				Polygon *poly = Walls[i]->GetPolygon(j);

				if (!poly)
					continue;

				PolyArray polyarray;

				for (size_t k = 0; k < poly->geometry.size(); k++)
				{
					for (size_t l = 0; l < poly->geometry[k].size(); l++)
						polyarray.emplace_back(poly->geometry[k][l].vertex);
				}

				for (size_t k = 0; k < poly->triangles.size(); k++)
				{
					const Triangle &tri = poly->triangles[k];

					Vector3 a = polyarray[tri.a];
					Vector3 b = polyarray[tri.b];
					Vector3 c = polyarray[tri.c];

					if (scale != 1.0)
					{
						a *= scale;
						b *= scale;
						c *= scale;
					}

					shape->AddTriangle(a, b, c);
					additions++;
				}
			}
		}

		//exit if no geometry
		if (additions == 0)
			return;

		//finalize shape
		shape->Finish();

		//create a collider scene node
		if (!collider_node)
			collider_node = GetSceneNode()->CreateChild(GetName() + " collider");

		//physics is not supported on triangle meshes; use CreateBoxCollider instead
		mBody = new OgreBulletDynamics::RigidBody(name, sbs->mWorld);
		mBody->setStaticShape(collider_node->GetRawSceneNode(), shape, 0.1f, 0.5f, false);
		mShape = shape;

		if (sbs->DeleteColliders == true)
		{
			bool revert = false;
			if (remove_on_disable == false)
			{
				remove_on_disable = true;
				revert = true;
			}

			Enabled(false);
			Enabled(true);

			if (revert == true)
				remove_on_disable = false;
		}
	}
	catch (Ogre::Exception &e)
	{
		ReportError("Error creating collider for '" + name + "'\n" + e.getDescription());
	}
}

void MeshObject::DeleteCollider()
{
	//delete mesh collider

	SBS_PROFILE("MeshObject::DeleteCollider");

	//exit if collider doesn't exist
	if (!mBody)
		return;

	//remove collider from world
	mBody->removeFromWorld();

	//delete collider object
	delete mBody;
	mBody = 0;
	mShape = 0;
}

Vector2 MeshObject::GetExtents(int coord, bool flip_z)
{
	//returns the smallest and largest values from a specified coordinate type
	//(x, y, or z) from the polygons of this mesh object.
	//first parameter must be either 1 (for x), 2 (for y) or 3 (for z)

	Real esmall = 0;
	Real ebig = 0;
	Real tempnum = 0;

	//return 0,0 if coord value is out of range
	if (coord < 1 || coord > 3)
		return Vector2(0, 0);

	for (size_t i = 0; i < Walls.size(); i++)
	{
		if (!Walls[i])
			continue;

		for (size_t j = 0; j < Walls[i]->GetPolygonCount(); j++)
		{
			Polygon *poly = Walls[i]->GetPolygon(j);

			if (!poly)
				continue;

			for (size_t k = 0; k < poly->geometry.size(); k++)
			{
				for (size_t l = 0; l < poly->geometry[k].size(); l++)
				{
					Ogre::Vector3 vertex = poly->geometry[k][l].vertex;

					if (coord == 1)
						tempnum = poly->geometry[k][l].vertex.x;
					if (coord == 2)
						tempnum = poly->geometry[k][l].vertex.y;
					if (coord == 3)
					{
						if (flip_z == false)
							tempnum = poly->geometry[k][l].vertex.z;
						else
							tempnum = -poly->geometry[k][l].vertex.z;
					}

					if (j == 0)
					{
						esmall = tempnum;
						ebig = tempnum;
					}
					else
					{
						if (tempnum < esmall)
							esmall = tempnum;
						if (tempnum > ebig)
							ebig = tempnum;
					}
				}
			}
		}
	}

	return Vector2(esmall, ebig);
}

Wall* MeshObject::FindPolygon(const std::string &name, int &index)
{
	//finds a polygon by name in all associated wall objects
	//returns associated wall object and polygon index

	for (size_t i = 0; i < Walls.size(); i++)
	{
		if (!Walls[i])
			continue;

		int polygon = Walls[i]->FindPolygon(name);
		if (polygon > -1)
		{
			index = polygon;
			return Walls[i];
		}
	}
	index = -1;
	return 0;
}

bool MeshObject::InBoundingBox(const Vector3 &pos, bool check_y)
{
	//determine if position 'pos' is inside the mesh's bounding box

	Vector3 position = sbs->ToRemote(pos - GetPosition());

	if (Bounds->isNull() == true)
		return false;

	Vector3 min = Bounds->getMinimum();
	Vector3 max = Bounds->getMaximum();

	if (position.x >= min.x && position.x <= max.x && position.z >= min.z && position.z <= max.z)
	{
		if (check_y == false)
			return true;
		else
		{
			if (position.y >= min.y && position.y <= max.y)
				return true;
		}
	}
	return false;
}

DynamicMesh* MeshObject::GetDynamicMesh()
{
	return MeshWrapper;
}

void MeshObject::GetMeshInformation(const Ogre::Mesh* const mesh, int &vertex_count, Vector3* &vertices, int &index_count, unsigned long* &indices, Ogre::AxisAlignedBox &extents)
{
	//read hardware buffers from a loaded model mesh, and return geometry arrays

	bool added_shared = false;
	size_t current_offset = 0;
	size_t shared_offset = 0;
	size_t next_offset = 0;
	size_t index_offset = 0;

	vertex_count = index_count = 0;

	// Calculate how many vertices and indices we're going to need
	for (unsigned short i = 0; i < mesh->getNumSubMeshes(); i++)
	{
		Ogre::SubMesh* submesh = mesh->getSubMesh(i);

		// We only need to add the shared vertices once
		if (submesh->useSharedVertices)
		{
			if (!added_shared)
			{
				vertex_count += (int)mesh->sharedVertexData->vertexCount;
				added_shared = true;
			}
		}
		else
		{
			vertex_count += (int)submesh->vertexData->vertexCount;
		}

		// Add the indices
		index_count += (int)submesh->indexData->indexCount;
	}

	//ensure minimum index count
	if (index_count < 3)
		return;

	// Allocate space for the vertices and indices
	vertices = new Vector3[vertex_count];
	indices = new unsigned long[index_count];

	added_shared = false;

	// Run through the submeshes again, adding the data into the arrays
	for (unsigned short i = 0; i < mesh->getNumSubMeshes(); i++)
	{
		Ogre::SubMesh* submesh = mesh->getSubMesh(i);
		Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

		if ((!submesh->useSharedVertices) || (submesh->useSharedVertices && !added_shared))
		{
			if(submesh->useSharedVertices)
			{
				added_shared = true;
				shared_offset = current_offset;
			}

			const Ogre::VertexElement* posElem =
				vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

			Ogre::HardwareVertexBufferSharedPtr vbuf =
				vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

			unsigned char* vertex =
				static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

			float* pReal;

			for (size_t j = 0; j < vertex_data->vertexCount; j++, vertex += vbuf->getVertexSize())
			{
				posElem->baseVertexPointerToElement(vertex, &pReal);
				Vector3 pt(pReal[0], pReal[1], pReal[2]);
				//vertices[current_offset + j] = (orient * (pt * scale)) + position;
				vertices[current_offset + j] = pt * sbs->ToRemote(GetSceneNode()->GetScale());
				extents.merge(vertices[current_offset + j]);
			}

			vbuf->unlock();
			next_offset += vertex_data->vertexCount;
		}

		Ogre::IndexData* index_data = submesh->indexData;
		size_t numTris = index_data->indexCount / 3;
		Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

		bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

		unsigned long* pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
		unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);

		size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

		if (use32bitindexes)
		{
			for (size_t k = 0; k < numTris * 3; k++)
			{
				indices[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
			}
		}
		else
		{
			for (size_t k = 0; k < numTris * 3; k++)
			{
				indices[index_offset++] = static_cast<unsigned long>(pShort[k]) + static_cast<unsigned long>(offset);
			}
		}

		ibuf->unlock();
		current_offset = next_offset;
	}
}

unsigned int MeshObject::GetVertexCount()
{
	unsigned int total = 0;

	for (size_t i = 0; i < Walls.size(); i++)
	{
		if (Walls[i])
			total += Walls[i]->GetVertexCount();
	}

	return total;
}

unsigned int MeshObject::GetTriangleCount(const std::string &material, bool total)
{
	unsigned int tris = 0;

	for (size_t i = 0; i < Walls.size(); i++)
	{
		if (Walls[i])
		{
			for (int j = 0; j < Walls[i]->GetPolygonCount(); j++)
			{
				Polygon *poly = Walls[i]->GetPolygon(j);

				if (poly)
				{
					if (poly->material == material || (material == "" && total == true))
						tris += poly->triangles.size();
				}
			}
		}
	}

	return tris;
}

void MeshObject::SetMaterial(const std::string& material)
{
	//set material of this mesh object

	MeshWrapper->SetMaterial(material);
}

void MeshObject::EnablePhysics(bool value, Real restitution, Real friction, Real mass)
{
	//Enables or disables physics on the mesh, the physics properties
	//and collider will be set up on the object later during the SBS::Prepare() stage

	is_physical = value;
	this->restitution = restitution;
	this->friction = friction;
	this->mass = mass;

	//use box collider if physics should be enabled
	if (is_physical == true)
		tricollider = false;

	LoadColliderModel(collidermesh);

	//set up collider for model (if mesh loaded from a filename)
	if ((Filename != "" || Meshname != "") && create_collider == true)
	{
		if (collidermesh.get() && Filename != "")
		{
			//create collider based on provided mesh collider
			int vertex_count, index_count;
			Vector3* vertices;
			long unsigned int* indices;
			Ogre::AxisAlignedBox box;
			GetMeshInformation(collidermesh.get(), vertex_count, vertices, index_count, indices, box);
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

size_t MeshObject::GetSize()
{
	//return size in bytes of this mesh object

	if (size > 0)
		return size;

	for (size_t i = 0; i < Walls.size(); i++)
	{
		if (!Walls[i])
			continue;

		for (size_t j = 0; j < Walls[i]->GetPolygonCount(); j++)
		{
			Polygon *poly = Walls[i]->GetPolygon(j);

			if (!poly)
				continue;

			size += poly->size;
		}
	}

	return size;
}

void MeshObject::CreateBoundingBox()
{
	//create a new bounding box for this mesh object
	//based on the geometry of all associated wall objects

	if (model_loaded == true)
		return;

	Ogre::AxisAlignedBox local_box;
	Bounds->setNull();
	bool merge = false;
	for (size_t i = 0; i < Walls.size(); i++)
	{
		if (!Walls[i])
			continue;

		//step through each wall polygon, and merge it's vertices into the bounding box
		for (size_t j = 0; j < Walls[i]->polygons.size(); j++)
		{
			Polygon *poly = Walls[i]->polygons[j];

			if (!poly)
				continue;
			for (size_t k = 0; k < poly->geometry.size(); k++)
			{
				for (size_t l = 0; l < poly->geometry[k].size(); l++)
				{
					Bounds->merge(poly->geometry[k][l].vertex);
					local_box.merge(sbs->ToLocal(poly->geometry[k][l].vertex) + GetPosition());
					merge = true;
				}
			}
		}
	}

	//merge bounds into sim engine bounding box
	if (merge == true)
		sbs->MergeBounds(local_box);
}

void MeshObject::RemoveTriOwner(Wall *wall)
{
	//remove a TriOwner by wall object

	for (size_t i = 0; i < triOwners.size(); /*no++*/)
	{
		if (triOwners[i].wall == wall)
		{
			RemoveTriOwnerFast(i);
			// Don’t increment i — we just moved the last tri here
		}
		else
		{
			++i;
		}
	}
}

void MeshObject::RemoveTriOwner(Polygon *poly)
{
	//remove a TriOwner by polygon object

	for (size_t i = 0; i < triOwners.size(); /*no++*/)
	{
		if (triOwners[i].poly == poly)
		{
			RemoveTriOwnerFast(i);
			// Don’t increment i — we just moved the last tri here
		}
		else
		{
			++i;
		}
	}
}

void MeshObject::AddTriOwner(Wall *wall, Polygon *poly)
{
	//add a triangle owner

	uint32_t base = static_cast<uint32_t>(pickPositions.size() - 3);

	pickIndices.push_back(base + 0);
	pickIndices.push_back(base + 1);
	pickIndices.push_back(base + 2);

	triOwners.push_back({wall, poly});
}

void MeshObject::RemoveTriOwner(size_t triIndex)
{
	//remove a TriOwner by triangle index

	size_t idxOffset = triIndex * 3;

	//erase the three indices
	pickIndices.erase(pickIndices.begin() + idxOffset, pickIndices.begin() + idxOffset + 3);

	//erase the owner
	triOwners.erase(triOwners.begin() + triIndex);
}

void MeshObject::RemoveTriOwnerFast(size_t triIndex)
{
	//remove a TriOwner by triangle index, ignoring triangle order

	size_t triCount = triOwners.size();
	if (triIndex >= triCount)
		return;

	size_t idxOffset = triIndex * 3;
	size_t lastIdxOffset = (triCount - 1) * 3;

	if (triIndex != triCount - 1)
	{
		//move last tri’s indices into this slot
		pickIndices[idxOffset + 0] = pickIndices[lastIdxOffset + 0];
		pickIndices[idxOffset + 1] = pickIndices[lastIdxOffset + 1];
		pickIndices[idxOffset + 2] = pickIndices[lastIdxOffset + 2];

		//move last triOwner
		triOwners[triIndex] = triOwners.back();
	}

	//pop last tri
	pickIndices.resize(pickIndices.size() - 3);
	triOwners.pop_back();
}

}
