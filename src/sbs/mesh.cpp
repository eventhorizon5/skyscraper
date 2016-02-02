/* $Id$ */

/*
	Scalable Building Simulator - Mesh Object
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

#include <OgreResourceGroupManager.h>
#include <OgreMaterialManager.h>
#include <OgreBulletDynamicsRigidBody.h>
#include <OgreMath.h>
#include <Shapes/OgreBulletCollisionsTrimeshShape.h>
#include <Shapes/OgreBulletCollisionsBoxShape.h>
#include <math.h>
#include "globals.h"
#include "sbs.h"
#include "unix.h"

//this file includes function implementations of the low-level SBS geometry and mesh code

namespace SBS {

Ogre::Vector2 SBS::GetExtents(std::vector<Ogre::Vector3> &varray, int coord, bool flip_z)
{
	//returns the smallest and largest values from a specified coordinate type
	//(x, y, or z) from a vertex array (polygon).
	//first parameter must be a vertex array
	//second must be either 1 (for x), 2 (for y) or 3 (for z)

	float esmall = 0;
	float ebig = 0;
	float tempnum = 0;
	int i = 0;

	//return 0,0 if coord value is out of range
	if (coord < 1 || coord > 3)
		return Ogre::Vector2(0, 0);

	for (i = 0; i < (int)varray.size(); i++)
	{
		if (coord == 1)
			tempnum = varray[i].x;
		if (coord == 2)
			tempnum = varray[i].y;
		if (coord == 3)
		{
			if (flip_z == false)
				tempnum = varray[i].z;
			else
				tempnum = -varray[i].z;
		}

		if (i == 0)
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

	return Ogre::Vector2(esmall, ebig);
}

void SBS::Cut(WallObject *wall, Ogre::Vector3 start, Ogre::Vector3 end, bool cutwalls, bool cutfloors, int checkwallnumber, bool reset_check)
{
	//cuts a rectangular hole in the polygons within the specified range

	if (cutwalls == false && cutfloors == false)
		return;

	//swap values if the first is greater than the second
	if (start.x > end.x)
	{
		float tmp = start.x;
		start.x = end.x;
		end.x = tmp;
	}
	if (start.y > end.y)
	{
		float tmp = start.y;
		start.y = end.y;
		end.y = tmp;
	}
	if (start.z > end.z)
	{
		float tmp = start.z;
		start.z = end.z;
		end.z = tmp;
	}

	std::vector<Ogre::Vector3> temppoly, temppoly2, temppoly3, temppoly4, temppoly5, worker;

	temppoly.reserve(32);
	temppoly2.reserve(32);
	temppoly3.reserve(32);
	temppoly4.reserve(32);
	temppoly5.reserve(32);

	bool polycheck = false;

	if (reset_check == true)
	{
		if (checkwallnumber == 1)
		{
			wall1a = false;
			wall1b = false;
		}
		if (checkwallnumber == 2)
		{
			wall2a = false;
			wall2b = false;
		}
	}

	//step through each polygon
	int polycount = wall->GetPolygonCount();
	for (int i = 0; i < polycount; i++)
	{
		//get name
		Polygon *polygon = wall->GetPolygon(i);

		//get original vertices
		std::vector<std::vector<Ogre::Vector3> > origpolys;
		std::vector<std::vector<Ogre::Vector3> > newpolys;
		wall->GetGeometry(i, origpolys);

		//skip empty meshes
		if (origpolys.size() == 0)
			continue;

		//cut all polygons within range
		for (int j = 0; j < (int)origpolys.size(); j++)
		{
			//skip null geometry
			if (origpolys[j].size() == 0)
				continue;

			temppoly.clear();
			temppoly2.clear();
			temppoly3.clear();
			temppoly4.clear();
			temppoly5.clear();
			worker.clear();
			Ogre::Vector2 extentsx, extentsy, extentsz;
			bool polycheck2 = false;

			//copy source polygon vertices
			for (int k = 0; k < (int)origpolys[j].size(); k++)
				temppoly.push_back(origpolys[j][k]);

			//make sure the polygon is not outside the cut area
			if (Classify(0, temppoly, start.x) != 1 &&
					Classify(0, temppoly, end.x) != 2 &&
					Classify(1, temppoly, start.y) != 1 &&
					Classify(1, temppoly, end.y) != 2 &&
					Classify(2, temppoly, start.z) != 1 &&
					Classify(2, temppoly, end.z) != 2)
			{
				extentsx = GetExtents(temppoly, 1);
				extentsy = GetExtents(temppoly, 2);
				extentsz = GetExtents(temppoly, 3);

				//is polygon a wall?
				if (extentsy.x != extentsy.y)
				{
					if (cutwalls == true)
					{
						//wall
						if (fabsf(extentsx.x - extentsx.y) > fabsf(extentsz.x - extentsz.y))
						{
							//wall is facing forward/backward

							//get left side
							worker = temppoly;
							SplitWithPlane(0, worker, temppoly, temppoly2, start.x);
							worker.clear();

							//get right side
							if (temppoly2.size() > 0)
								worker = temppoly2;
							else
								worker = temppoly;
							SplitWithPlane(0, worker, temppoly3, temppoly2, end.x);
							worker.clear();

							//get lower
							if (temppoly3.size() > 0)
								worker = temppoly3;
							else if (temppoly2.size() > 0)
								worker = temppoly2;
							else if (temppoly.size() > 0)
								worker = temppoly;
							SplitWithPlane(1, worker, temppoly3, temppoly4, start.y);
							worker.clear();

							//get upper
							if (temppoly4.size() > 0)
								worker = temppoly4;
							else if (temppoly3.size() > 0)
								worker = temppoly3;
							else if (temppoly2.size() > 0)
								worker = temppoly2;
							else if (temppoly.size() > 0)
								worker = temppoly;
							SplitWithPlane(1, worker, temppoly5, temppoly4, end.y);
							worker.clear();
						}
						else
						{
							//wall is facing left/right

							//get left side
							worker = temppoly;
							SplitWithPlane(2, worker, temppoly, temppoly2, start.z);
							worker.clear();

							//get right side
							if (temppoly2.size() > 0)
								worker = temppoly2;
							else
								worker = temppoly;
							SplitWithPlane(2, worker, temppoly3, temppoly2, end.z);
							worker.clear();

							//get lower
							if (temppoly3.size() > 0)
								worker = temppoly3;
							else if (temppoly2.size() > 0)
								worker = temppoly2;
							else if (temppoly.size() > 0)
								worker = temppoly;
							SplitWithPlane(1, worker, temppoly3, temppoly4, start.y);
							worker.clear();

							//get upper
							if (temppoly4.size() > 0)
								worker = temppoly4;
							else if (temppoly3.size() > 0)
								worker = temppoly3;
							else if (temppoly2.size() > 0)
								worker = temppoly2;
							else if (temppoly.size() > 0)
								worker = temppoly;
							SplitWithPlane(1, worker, temppoly5, temppoly4, end.y);
							worker.clear();
						}
						polycheck = true;
						polycheck2 = true;

						//store extents of temppoly5 for door sides if needed
						GetDoorwayExtents(wall->GetMesh(), checkwallnumber, temppoly5);
					}
				}
				else if (cutfloors == true)
				{
					//floor

					//get left side
					worker = temppoly;
					SplitWithPlane(0, worker, temppoly, temppoly2, start.x);
					worker.clear();

					//get right side
					if (temppoly2.size() > 0)
						worker = temppoly2;
					else
						worker = temppoly;
					SplitWithPlane(0, worker, temppoly3, temppoly2, end.x);
					worker.clear();

					//get lower
					if (temppoly3.size() > 0)
						worker = temppoly3;
					else if (temppoly2.size() > 0)
						worker = temppoly2;
					else if (temppoly.size() > 0)
						worker = temppoly;
					SplitWithPlane(2, worker, temppoly3, temppoly4, start.z);
					worker.clear();

					//get upper
					if (temppoly4.size() > 0)
						worker = temppoly4;
					else if (temppoly3.size() > 0)
						worker = temppoly3;
					else if (temppoly2.size() > 0)
						worker = temppoly2;
					else if (temppoly.size() > 0)
						worker = temppoly;
					SplitWithPlane(2, worker, temppoly5, temppoly4, end.z);
					worker.clear();
					temppoly5.clear();

					polycheck = true;
					polycheck2 = true;
				}

				//create split polygons
				if (polycheck2 == true)
				{
					if (temppoly.size() > 2)
					{
						newpolys.resize(newpolys.size() + 1);
						if (newpolys[newpolys.size() - 1].capacity() < temppoly.size())
							newpolys[newpolys.size() - 1].reserve(temppoly.size());
						for (int k = 0; k < (int)temppoly.size(); k++)
							newpolys[newpolys.size() - 1].push_back(temppoly[k]);
					}
					if (temppoly2.size() > 2)
					{
						newpolys.resize(newpolys.size() + 1);
						if (newpolys[newpolys.size() - 1].capacity() < temppoly2.size())
							newpolys[newpolys.size() - 1].reserve(temppoly2.size());
						for (int k = 0; k < (int)temppoly2.size(); k++)
							newpolys[newpolys.size() - 1].push_back(temppoly2[k]);
					}
					if (temppoly3.size() > 2)
					{
						newpolys.resize(newpolys.size() + 1);
						if (newpolys[newpolys.size() - 1].capacity() < temppoly3.size())
							newpolys[newpolys.size() - 1].reserve(temppoly3.size());
						for (int k = 0; k < (int)temppoly3.size(); k++)
							newpolys[newpolys.size() - 1].push_back(temppoly3[k]);
					}
					if (temppoly4.size() > 2)
					{
						newpolys.resize(newpolys.size() + 1);
						if (newpolys[newpolys.size() - 1].capacity() < temppoly4.size())
							newpolys[newpolys.size() - 1].reserve(temppoly4.size());
						for (int k = 0; k < (int)temppoly4.size(); k++)
							newpolys[newpolys.size() - 1].push_back(temppoly4[k]);
					}

					temppoly.clear();
					temppoly2.clear();
					temppoly3.clear();
					temppoly4.clear();
				}
			}
			else
			{
				//otherwise put original polygon into array (will only be used if the related submesh is recreated)
				newpolys.push_back(origpolys[j]);
			}
		}

		//create new polygon
		if (polycheck == true)
		{
			std::string oldmat;
			Ogre::Vector3 oldvector;
			Ogre::Matrix3 mapping;
			std::string name = polygon->GetName();

			if (newpolys.size() > 0)
			{
				//get texture data from original polygon
				oldmat = polygon->material;
				polygon->GetTextureMapping(mapping, oldvector);
			}

			//delete original polygon
			wall->DeletePolygon(i, false);
			polygon = 0;

			//create new polygon
			if (newpolys.size() > 0)
				wall->AddPolygon(name, oldmat, newpolys, mapping, oldvector);

			//reset search position
			i--;
			polycount--;
			polycheck = false;
		}
	}
}

void SBS::GetDoorwayExtents(MeshObject *mesh, int checknumber, std::vector<Ogre::Vector3> &polygon)
{
	//calculate doorway extents, for use with AddDoorwayWalls function
	//checknumber is 1 when checking shaft walls, and 2 when checking floor walls

	if (checknumber > 0 && checknumber < 3)
	{
		Ogre::Vector3 mesh_position = mesh->GetPosition();
		float extent;

		if (wall2a == false || wall2b == false)
		{
			if (checknumber == 2)
			{
				//level walls
				if (wall2a == true)
					wall2b = true;
				wall2a = true;
				extent = GetExtents(polygon, 1).x + mesh_position.x;
				if (wall2b == false || (wall2b == true && fabsf(extent - mesh_position.x) > fabsf(wall_extents_x.x - mesh_position.x)))
					wall_extents_x.x = extent;
				extent = GetExtents(polygon, 3).x + mesh_position.z;
				if (wall2b == false || (wall2b == true && fabsf(extent - mesh_position.z) > fabsf(wall_extents_z.x - mesh_position.z)))
					wall_extents_z.x = extent;
				wall_extents_y = GetExtents(polygon, 2) + mesh_position.y;
			}
			else
			{
				//shaft walls
				if (wall1a == true)
					wall1b = true;
				wall1a = true;
				extent = GetExtents(polygon, 1).y + mesh_position.x;
				if (wall1b == false || (wall1b == true && fabsf(extent - mesh_position.x) < fabsf(wall_extents_x.y - mesh_position.x)))
					wall_extents_x.y = extent;
				extent = GetExtents(polygon, 3).y + mesh_position.z;
				if (wall1b == false || (wall1b == true && fabsf(extent - mesh_position.z) < fabsf(wall_extents_z.y - mesh_position.z)))
					wall_extents_z.y = extent;
			}
		}
	}
}

Ogre::Vector3 SBS::GetPolygonDirection(std::vector<Ogre::Vector3> &polygon)
{
	//returns the direction the polygon faces, in a 3D vector
	//for example, <-1, 0, 0> means that the wall faces left.

	//get largest normal

	//convert to remote values for precision compatibility with Alpha 7 and earlier
	std::vector<Ogre::Vector3> newpoly;
	for (int i = 0; i < (int)polygon.size(); i++)
		newpoly.push_back(ToRemote(polygon[i], true, false));

	float D = 0;
	Ogre::Vector3 normal = ComputeNormal(newpoly, D);

	int largest_normal = 0; //x

	if (fabsf(normal.y) >= fabsf(normal.x) && fabsf(normal.y) >= fabsf(normal.z))
		largest_normal = 1; //y biggest
	else if (fabsf(normal.z) > fabsf(normal.x))
		largest_normal = 2; //z biggest

	if (largest_normal == 0)
	{
		if (normal.x < 0)
			return Ogre::Vector3(1, 0, 0);
		else
			return Ogre::Vector3(-1, 0, 0);
	}

	if (largest_normal == 1)
	{
		if (normal.y < 0)
			return Ogre::Vector3(0, 1, 0);
		else
			return Ogre::Vector3(0, -1, 0);
	}

	if (largest_normal == 2)
	{
		if (normal.z < 0)
			return Ogre::Vector3(0, 0, 1);
		else
			return Ogre::Vector3(0, 0, -1);
	}
	return Ogre::Vector3(0, 0, 0);
}

Ogre::Vector2 SBS::GetEndPoint(const Ogre::Vector2 &StartPoint, float angle, float distance)
{
	//calculate 2D endpoint from given starting point, along with angle and distance (magnitude)
	//angle is in degrees

	angle -= 90;
	float newangle = DegreesToRadians(angle);
	float x = StartPoint.x + distance * cosf(newangle);
	float y = -StartPoint.y + distance * sinf(newangle);
	Ogre::Vector2 result (x, -y);
	return result;

}

Ogre::Plane SBS::ComputePlane(std::vector<Ogre::Vector3> &vertices)
{
	//compute plane from a set of given vertices

	float det;
	Ogre::Vector3 normal = -ComputeNormal(vertices, det);
	normal.normalise();
	return Ogre::Plane(normal, det);
}

MeshObject::MeshObject(Object* parent, const std::string &name, DynamicMesh* wrapper, const std::string &filename, float max_render_distance, float scale_multiplier, bool enable_physics, float restitution, float friction, float mass) : Object(parent)
{
	//set up SBS object
	SetValues("Mesh", name, true);

	enabled = true;
	mBody = 0;
	mShape = 0;
	prepared = false;
	is_physical = enable_physics;
	this->restitution = restitution;
	this->friction = friction;
	this->mass = mass;
	no_collider = false;
	MeshGeometry.reserve(128); //reserve vertex space
	collider_node = 0;
	Filename = filename;
	remove_on_disable = true;
	wrapper_selfcreate = false;
	model_loaded = false;

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
		MeshWrapper = new DynamicMesh(this, GetSceneNode(), name, max_render_distance);
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
	if (filename != "")
	{
		if (collidermesh.get())
		{
			//create collider based on provided mesh collider
			int vertex_count, index_count;
			Ogre::Vector3* vertices;
			long unsigned int* indices;
			Ogre::AxisAlignedBox box;
			GetMeshInformation(collidermesh.getPointer(), vertex_count, vertices, index_count, indices, box);
			CreateColliderFromModel(vertex_count, vertices, index_count, indices);
			delete[] vertices;
			delete[] indices;
			Ogre::MeshManager::getSingleton().remove(collidermesh->getHandle());
		}
		else
		{
			//create generic box collider if separate mesh collider isn't available
			Bounds = MeshWrapper->GetBounds(this);
			CreateBoxCollider();
		}
	}
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
}

void MeshObject::Enable(bool value)
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

	MeshWrapper->Enable(value, this);

	EnableCollider(value);

	enabled = value;
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

WallObject* MeshObject::CreateWallObject(const std::string &name)
{
	//create a new wall object in the given array

	WallObject *wall = new WallObject(this);
	wall->SetParentArray(Walls);
	wall->SetValues("Wall", name, false, false);
	Walls.push_back(wall);
	return wall;
}

WallObject* MeshObject::GetWallByName(std::string name)
{
	//find a wall object by name

	SetCase(name, false);

	for (int i = 0; i < (int)Walls.size(); i++)
	{
		if (name == SetCaseCopy(Walls[i]->GetName(), false) == true)
			return Walls[i];
	}
	return 0;
}

bool MeshObject::ChangeTexture(const std::string &texture, bool matcheck, int submesh)
{
	//changes a texture
	//if matcheck is true, exit if old and new textures are the same

	SBS_PROFILE("MeshObject::ChangeTexture");
	std::string material = texture;
	TrimString(material);

	if (submesh < 0 || submesh >= (int)Submeshes.size())
		return false;

	//exit if old and new materials are the same
	if (matcheck == true)
	{
		if (Submeshes[submesh].Name == material)
			return false;
	}

	bool result = MeshWrapper->ChangeTexture(Submeshes[submesh].Name, material, this);

	if (result == false)
		return false;

	Submeshes[submesh].Name = material;
	return true;
}

bool MeshObject::ReplaceTexture(const std::string &oldtexture, const std::string &newtexture)
{
	//replace submesh materials named oldtexture with newtexture
	int submesh = FindMatchingSubMesh(oldtexture);
	bool result = false;
	if (submesh >= 0)
		result = ChangeTexture(newtexture, true, submesh);
	return result;
}

WallObject* MeshObject::FindWall(const Ogre::Vector3 &point, bool convert)
{
	//find a wall from a 3D point

	SBS_PROFILE("MeshObject::FindWall");
	for (int i = 0; i < (int)Walls.size(); i++)
	{
		if (Walls[i]->IsPointOnWall(point, convert) == true)
			return Walls[i];
	}
	return 0;
}

WallObject* MeshObject::FindWallIntersect(const Ogre::Vector3 &start, const Ogre::Vector3 &end, Ogre::Vector3 &isect, float &distance, Ogre::Vector3 &normal, bool convert, bool rescale)
{
	//find a wall from a 3D point

	SBS_PROFILE("MeshObject::FindWallIntersect");
	float pr, best_pr = 2000000000.;
	float dist, best_dist = 2000000000.;
	int best_i = -1;
	Ogre::Vector3 cur_isect;
	Ogre::Vector3 tmpnormal;

	for (int i = 0; i < (int)Walls.size(); i++)
	{
		for (int j = 0; j < Walls[i]->GetPolygonCount(); j++)
		{
			if (Walls[i]->GetPolygon(j)->IntersectSegment(start, end, cur_isect, &pr, tmpnormal, convert, rescale) == true)
			{
				if (pr < best_pr)
				{
					//currently test against previous camera intersection test to fix some weird issues
					Ogre::Vector3 orig_start = sbs->ToRemote(sbs->camera->HitPosition);
					dist = orig_start.distance(cur_isect);

					if (dist < best_dist)
					{
						//calculate distance to intersection
						distance = start.distance(cur_isect);

						best_dist = dist;
						best_pr = pr;
						best_i = i;
						isect = cur_isect;
						normal = tmpnormal;
					}
				}
			}
		}
	}

	if (best_i >= 0)
		return Walls[best_i];
	else
		return 0;
}

bool MeshObject::IsEnabled()
{
	return enabled;
}

void MeshObject::AddVertex(Geometry &vertex_geom)
{
	//add a vertex to the mesh
	MeshGeometry.push_back(vertex_geom);
	prepared = false; //need to re-prepare mesh
}

void MeshObject::AddTriangle(int submesh, Triangle &triangle)
{
	//add a triangle to the mesh
	Submeshes[submesh].Triangles.push_back(triangle);
	prepared = false; //need to re-prepare mesh
}

void MeshObject::RemoveTriangle(int submesh, int index)
{
	//remove a triangle from the mesh

	if (index < 0 || index >= (int)Submeshes[submesh].Triangles.size())
		return;

	Submeshes[submesh].Triangles.erase(Submeshes[submesh].Triangles.begin() + index);
	prepared = false; //need to re-prepare mesh
}

bool MeshObject::PolyMesh(const std::string &name, const std::string &texture, std::vector<Ogre::Vector3> &vertices, float tw, float th, bool autosize, Ogre::Matrix3 &t_matrix, Ogre::Vector3 &t_vector, std::vector<Extents> &mesh_indices, std::vector<Triangle> &triangles)
{
	//create custom mesh geometry, apply a texture map and material, and return the created submesh

	//exit if this mesh is a loaded file
	if (Filename != "")
		return sbs->ReportError("PolyMesh: Cannot create geometry in a file-loaded mesh");

	//get texture material
	std::string texname = texture;
	bool result;
	std::string material = sbs->GetTextureMaterial(texture, result, true, name);
	if (!result)
		texname = "Default";

	//convert to remote positioning
	std::vector<std::vector<Ogre::Vector3> > vertices2;
	vertices2.resize(1);

	vertices2[0].reserve(vertices.size());
	for (int i = 0; i < (int)vertices.size(); i++)
		vertices2[0].push_back(sbs->ToRemote(vertices[i]));

	//texture mapping
	Ogre::Vector3 v1, v2, v3;
	int direction;

	//get texture mapping coordinates
	if (!sbs->GetTextureMapping(vertices2[0], v1, v2, v3, direction))
		return sbs->ReportError("PolyMesh: Texture mapping error");

	if (tw == 0)
		tw = 1;
	if (th == 0)
		th = 1;

	//get autosize information
	Ogre::Vector2 sizing (tw, th);

	if (autosize == true)
		sizing = sbs->CalculateSizing(texture, sbs->ToLocal(v1), sbs->ToLocal(v2), sbs->ToLocal(v3), direction, tw, th);

	//get texture tiling information
	float tw2 = sizing.x, th2 = sizing.y;
	float mw, mh;
	if (sbs->GetTextureTiling(texname, mw, mh))
	{
		//multiply the tiling parameters (tw and th) by
		//the stored multipliers for that texture
		tw2 = sizing.x * mw;
		th2 = sizing.y * mh;
	}

	if (!ComputeTextureMap(t_matrix, t_vector, vertices2[0],
			v1,
			Ogre::Vector2 (sbs->MapUV[0].x * tw2, sbs->MapUV[0].y * th2),
			v2,
			Ogre::Vector2 (sbs->MapUV[1].x * tw2, sbs->MapUV[1].y * th2),
			v3,
			Ogre::Vector2 (sbs->MapUV[2].x * tw2, sbs->MapUV[2].y * th2)))
		return false;

	return PolyMesh(name, material, vertices2, t_matrix, t_vector, mesh_indices, triangles, tw2, th2, false);
}

bool MeshObject::PolyMesh(const std::string &name, const std::string &material, std::vector<std::vector<Ogre::Vector3> > &vertices, Ogre::Matrix3 &tex_matrix, Ogre::Vector3 &tex_vector, std::vector<Extents> &mesh_indices, std::vector<Triangle> &triangles, float tw, float th, bool convert_vertices)
{
	//create custom geometry, apply a texture map and material, and return the created submesh
	//tw and th are only used when overriding texel map

	//convert to remote positioning
	std::vector<std::vector<Ogre::Vector3> > vertices2;
	if (convert_vertices == true)
	{
		vertices2.resize(vertices.size());
		for (int i = 0; i < (int)vertices.size(); i++)
		{
			vertices2[i].reserve(vertices[i].size());
			for (int j = 0; j < (int)vertices[i].size(); j++)
				vertices2[i].push_back(sbs->ToRemote(vertices[i][j]));
		}
	}
	else
		vertices2 = vertices;

	//texture mapping
	Ogre::Vector2 *table = GetTexels(tex_matrix, tex_vector, vertices2, tw, th);

	//triangulate mesh
	TriangleIndices *trimesh = new TriangleIndices[vertices2.size()];
	int trimesh_size = (int)vertices2.size();

	for (int i = 0; i < trimesh_size; i++)
	{
		//do a (very) simple triangulation
		//this method also somewhat works with non-planar polygons
		trimesh[i].triangles.reserve(vertices2[i].size() - 2);
		for (int j = 2; j < (int)vertices2[i].size(); j++)
			trimesh[i].triangles.push_back(Triangle(0, j - 1, j));
	}

	//set up geometry array
	Geometry *mesh_geometry = 0;

	//initialize geometry arrays
	int size = 0;
	for (int i = 0; i < trimesh_size; i++)
		size += (int)vertices2[i].size();
	mesh_geometry = new Geometry[size];

	//get number of existing vertices
	int count = (int)MeshGeometry.size();

	//populate vertices, normals, and texels for mesh data
	int k = 0;

	if (mesh_indices.capacity() < mesh_indices.size() + trimesh_size)
		mesh_indices.reserve(mesh_indices.size() + trimesh_size);

	for (int i = 0; i < trimesh_size; i++)
	{
		int min = count + k;
		for (int j = 0; j < (int)vertices2[i].size(); j++)
		{
			mesh_geometry[k].normal = mesh_geometry[k].vertex = vertices2[i][j];
			mesh_geometry[k].normal.normalise();
			mesh_geometry[k].texel = table[k];

			//add geometry to mesh
			AddVertex(mesh_geometry[k]);
			k++;
		}
		int max = count + k - 1;
		mesh_indices.push_back(Extents(min, max));
	}

	//delete texel array
	if (table)
		delete [] table;
	table = 0;

	//delete geometry array
	delete [] mesh_geometry;
	mesh_geometry = 0;

	//add triangles to single array, to be passed to the submesh
	int location = 0;
	for (int i = 0; i < trimesh_size; i++)
	{
		if (triangles.capacity() < trimesh[i].triangles.size())
			triangles.reserve(trimesh[i].triangles.size());
		for (int j = 0; j < (int)trimesh[i].triangles.size(); j++)
		{
			Triangle tri = trimesh[i].triangles[j];
			tri += count + location;
			triangles.push_back(tri);
		}
		location += (int)vertices2[i].size();
	}

	//delete trimesh array
	delete [] trimesh;
	trimesh = 0;

	//create submesh and set material
	ProcessSubMesh(triangles, material, true);

	//recreate colliders if specified
	if (sbs->DeleteColliders == true)
		DeleteCollider();

	if (sbs->RenderOnStartup == true)
		Prepare();

	return true;
}

Ogre::Vector2* MeshObject::GetTexels(Ogre::Matrix3 &tex_matrix, Ogre::Vector3 &tex_vector, std::vector<std::vector<Ogre::Vector3> > &vertices, float tw, float th)
{
	//return texel array for specified texture transformation matrix and vector

	if (sbs->TexelOverride == false)
	{
		//create array for texel map
		int texel_count = 0;
		for (int i = 0; i < (int)vertices.size(); i++)
			texel_count += (int)vertices[i].size();
		Ogre::Vector2 *texels = new Ogre::Vector2[texel_count];

		//transform matrix into texel map
		int index = 0;
		Ogre::Vector3 texel_temp;
		for (int i = 0; i < (int)vertices.size(); i++)
		{
			for (int j = 0; j < (int)vertices[i].size(); j++)
			{
				texel_temp = tex_matrix * (vertices[i][j] - tex_vector);
				texels[index].x = texel_temp.x;
				texels[index].y = texel_temp.y;
				index++;
			}
		}
		return texels;
	}
	else
	{
		Ogre::Vector2 *texels = new Ogre::Vector2[4];
		texels[0].x = 0;
		texels[0].y = 0;
		texels[1].x = tw;
		texels[1].y = 0;
		texels[2].x = tw;
		texels[2].y = th;
		texels[3].x = 0;
		texels[3].y = th;
		return texels;
	}
	return 0;
}

int MeshObject::ProcessSubMesh(std::vector<Triangle> &indices, const std::string &material, bool add)
{
	//processes submeshes for new or removed geometry
	//the Prepare() function must be called when the mesh is ready to view

	int index_count = (int)indices.size();
	Triangle *indexarray = new Triangle[index_count];

	for (int i = 0; i < index_count; i++)
		indexarray[i] = indices[i];

	//first get related submesh
	int index = FindMatchingSubMesh(material);
	bool createnew = false;

	//exit if trying to remove indices from non-existing submesh
	if (index == -1 && add == false)
		return -1;

	//create a new submesh if needed
	if (index == -1)
	{
		index = (int)Submeshes.size();
		Submeshes.resize(Submeshes.size() + 1);
		createnew = true;
	}

	//delete submesh and exit if it's going to be emptied
	if (createnew == false && add == false)
	{
		if (Submeshes[index].Triangles.size() - index_count <= 0)
		{
			Submeshes.erase(Submeshes.begin() + index);
			delete [] indexarray;
			return -1;
		}
	}

	//add triangles
	if (add == true)
	{
		//reserve triangles
		int size = (int)Submeshes[index].Triangles.size() + index_count;
		int capacity = (int)Submeshes[index].Triangles.capacity();
		if (capacity < size)
			Submeshes[index].Triangles.reserve(capacity * 2);

		for (int i = 0; i < index_count; i++)
			AddTriangle(index, indexarray[i]);
	}
	else
	{
		//remove triangles
		for (int i = 0; i < (int)Submeshes[index].Triangles.size(); i++)
		{
			Triangle &triangle = Submeshes[index].Triangles[i];
			for (int j = 0; j < index_count; j++)
			{
				if (triangle == indexarray[j])
				{
					//delete match
					RemoveTriangle(index, i);
					i--;
					break;
				}
			}
		}
	}

	//bind material
	Submeshes[index].Name = material;

	delete [] indexarray;
	return index;
}

void MeshObject::Prepare(bool force)
{
	//prepare mesh object

	if (prepared == true && force == false)
		return;

	//set up bounding box
	if (model_loaded == false)
	{
		for (int i = 0; i < (int)MeshGeometry.size(); i++)
			Bounds.merge(MeshGeometry[i].vertex);
	}

	//update dynamic mesh
	MeshWrapper->NeedsUpdate(this);

	prepared = true;
}

int MeshObject::FindMatchingSubMesh(const std::string &material)
{
	//find a submesh with a matching material
	//returns array index

	for (int i = 0; i < (int)Submeshes.size(); i++)
	{
		if (Submeshes[i].Name == material)
			return i;
	}
	return -1;
}

void MeshObject::DeleteVertices(std::vector<Triangle> &deleted_indices)
{
	//delete related mesh vertices using provided index array
	//then reindex all mesh triangle indices in all submeshes.
	//this should be done after a polygon is deleted

	//exit if index array is empty
	if (deleted_indices.size() == 0)
		return;

	//construct new sorted and compressed index array
	std::vector<int> deleted_v;
	deleted_v.reserve(deleted_indices.size() * 3);
	for (int i = 0; i < (int)deleted_indices.size(); i++)
	{
		if (find(deleted_v.begin(), deleted_v.end(), deleted_indices[i].a) == deleted_v.end())
			deleted_v.push_back(deleted_indices[i].a);
		if (find(deleted_v.begin(), deleted_v.end(), deleted_indices[i].b) == deleted_v.end())
			deleted_v.push_back(deleted_indices[i].b);
		if (find(deleted_v.begin(), deleted_v.end(), deleted_indices[i].c) == deleted_v.end())
			deleted_v.push_back(deleted_indices[i].c);
	}
	sort(deleted_v.begin(), deleted_v.end());

	//copy vector index array into simple array
	int deleted_size = (int)deleted_v.size();
	int *deleted = new int[deleted_size];
	for (int i = 0; i < deleted_size; i++)
		deleted[i] = deleted_v[i];

	//delete specified vertices
	for (int i = deleted_size - 1; i >= 0; i--)
		MeshGeometry.erase(MeshGeometry.begin() + deleted[i]);

	//reindex triangle indices in all submeshes
	for (int submesh = 0; submesh < (int)Submeshes.size(); submesh++)
	{
		std::vector<Triangle> &triangles = Submeshes[submesh].Triangles;

		int elements_size = (int)triangles.size() * 3;
		int *elements = new int[elements_size];

		int elements_pos = 0;
		for (int i = 0; i < (int)triangles.size(); i++)
		{
			elements[elements_pos] = triangles[i].a;
			elements_pos++;
			elements[elements_pos] = triangles[i].b;
			elements_pos++;
			elements[elements_pos] = triangles[i].c;
			elements_pos++;

			for (int j = deleted_size - 1; j >= 0; j--)
			{
				if (elements[elements_pos - 3] >= deleted[j])
					elements[elements_pos - 3]--;
				if (elements[elements_pos - 2] >= deleted[j])
					elements[elements_pos - 2]--;
				if (elements[elements_pos - 1] >= deleted[j])
					elements[elements_pos - 1]--;
			}
		}

		int element = 0;
		int size = (int)triangles.size();
		triangles.clear();
		if ((int)triangles.capacity() < size)
			triangles.reserve(size);

		for (int i = 0; i < size; i++)
		{
			//check if triangle indices are valid
			if (elements[element] >= 0 || elements[element + 1] >= 0 || elements[element + 2] >= 0)
				triangles.push_back(Triangle(elements[element], elements[element + 1], elements[element + 2]));
			element += 3;
		}
		delete [] elements;
	}

	//reindex triangle indices in all wall objects
	for (int i = 0; i < (int)Walls.size(); i++)
	{
		if (!Walls[i])
			continue;

		for (int j = 0; j < Walls[i]->GetPolygonCount(); j++)
		{
			//reindex triangle indices

			Polygon *poly = Walls[i]->GetPolygon(j);

			int elements_size = (int)poly->triangles.size() * 3;
			int *elements = new int[elements_size];

			int elements_pos = 0;
			for (int k = 0; k < (int)poly->triangles.size(); k++)
			{
				elements[elements_pos] = poly->triangles[k].a;
				elements_pos++;
				elements[elements_pos] = poly->triangles[k].b;
				elements_pos++;
				elements[elements_pos] = poly->triangles[k].c;
				elements_pos++;

				for (int j = deleted_size - 1; j >= 0; j--)
				{
					if (elements[elements_pos - 3] >= deleted[j])
						elements[elements_pos - 3]--;
					if (elements[elements_pos - 2] >= deleted[j])
						elements[elements_pos - 2]--;
					if (elements[elements_pos - 1] >= deleted[j])
						elements[elements_pos - 1]--;
				}
			}

			int element = 0;
			int size = (int)poly->triangles.size();
			poly->triangles.clear();
			if ((int)poly->triangles.capacity() < size)
				poly->triangles.reserve(size);

			for (int ii = 0; ii < size; ii++)
			{
				//check if triangle indices are valid
				if (elements[element] >= 0 || elements[element + 1] >= 0 || elements[element + 2] >= 0)
					poly->triangles.push_back(Triangle(elements[element], elements[element + 1], elements[element + 2]));
				element += 3;
			}

			//reindex extents, used for getting original geometry
			for (int k = deleted_size - 1; k >= 0; k--)
			{
				int size = (int)poly->index_extents.size();
				for (int m = 0; m < size; m++)
				{
					Extents extents = poly->index_extents[m];
					if (deleted[k] < extents.x)
						extents.x--;
					if (deleted[k] < extents.y)
						extents.y--;
					poly->index_extents[m] = extents;
				}
			}
			delete [] elements;
		}
	}
	prepared = false; //need to re-prepare mesh
	delete [] deleted;
}

void MeshObject::EnableDebugView(bool value)
{
	//enable or disable debug view of mesh
	MeshWrapper->EnableDebugView(value, this);
}

void MeshObject::CreateCollider()
{
	//set up triangle collider based on raw SBS mesh geometry

	//exit if collider already exists
	if (mBody)
		return;

	if (!GetSceneNode())
		return;

	//exit if mesh is empty
	if (MeshGeometry.size() == 0 || Submeshes.size() == 0)
		return;

	int tricount = 0;
	for (int i = 0; i < (int)Submeshes.size(); i++)
		tricount += (int)Submeshes[i].Triangles.size();

	try
	{
		//initialize collider shape
		OgreBulletCollisions::TriangleMeshCollisionShape* shape = new OgreBulletCollisions::TriangleMeshCollisionShape((int)MeshGeometry.size(), tricount * 3);

		float scale = GetSceneNode()->GetScale();

		//add vertices to shape
		for (int i = 0; i < (int)Submeshes.size(); i++)
		{
			for (int j = 0; j < (int)Submeshes[i].Triangles.size(); j++)
			{
				const Triangle &tri = Submeshes[i].Triangles[j];

				Ogre::Vector3 a = MeshGeometry[tri.a].vertex;
				Ogre::Vector3 b = MeshGeometry[tri.b].vertex;
				Ogre::Vector3 c = MeshGeometry[tri.c].vertex;

				if (scale != 1.0f)
				{
					a *= scale;
					b *= scale;
					c *= scale;
				}

				shape->AddTriangle(a, b, c);
			}
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

		if (sbs->DeleteColliders == true)
		{
			bool revert = false;
			if (remove_on_disable == false)
			{
				remove_on_disable = true;
				revert = true;
			}

			Enable(false);
			Enable(true);

			if (revert == true)
				remove_on_disable = false;
		}
	}
	catch (Ogre::Exception &e)
	{
		sbs->ReportError("Error creating collider for '" + name + "'\n" + e.getDescription());
	}
}

void MeshObject::DeleteCollider()
{
	//delete mesh collider

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

void MeshObject::CreateColliderFromModel(int &vertex_count, Ogre::Vector3* &vertices, int &index_count, unsigned long* &indices)
{
	//set up triangle collider based on loaded model geometry

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
		sbs->ReportError("Error creating model collider for '" + name + "'\n" + e.getDescription());
	}
}

void MeshObject::CreateBoxCollider()
{
	//set up a box collider for full extents of a mesh

	//exit of collider already exists
	if (mBody)
		return;

	if (!GetSceneNode())
		return;

	try
	{
		//initialize collider shape
		float scale = GetSceneNode()->GetScale();

		if (Bounds.isNull() == true)
			return;

		Ogre::Vector3 bounds = Bounds.getHalfSize() * scale;
		OgreBulletCollisions::BoxCollisionShape* shape = new OgreBulletCollisions::BoxCollisionShape(bounds);

		//create a new scene node for this collider, and center the collider accordingly
		Ogre::Vector3 collider_center = sbs->ToLocal(Bounds.getCenter());
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
		sbs->ReportError("Error creating box collider for '" + name + "'\n" + e.getDescription());
	}
}

float MeshObject::HitBeam(const Ogre::Vector3 &origin, const Ogre::Vector3 &direction, float max_distance)
{
	//cast a ray and return the collision distance to the mesh
	//return -1 if no hit

	//cast a ray from the camera position downwards
	SBS_PROFILE("MeshObject::HitBeam");

	Ogre::Vector3 position = sbs->ToRemote(origin - GetPosition());
	Ogre::Ray ray (position, sbs->ToRemote(direction, false));

	for (int i = 0; i < (int)Submeshes.size(); i++)
	{
		for (int j = 0; j < (int)Submeshes[i].Triangles.size(); j++)
		{
			const Triangle &tri = Submeshes[i].Triangles[j];
			Ogre::Vector3 tri_a, tri_b, tri_c;
			tri_a = MeshGeometry[tri.a].vertex;
			tri_b = MeshGeometry[tri.b].vertex;
			tri_c = MeshGeometry[tri.c].vertex;

			std::pair<bool, float> result = Ogre::Math::intersects(ray, tri_a, tri_b, tri_c);
			if (result.first == true)
			{
				if (result.second <= sbs->ToRemote(max_distance))
					return sbs->ToLocal(result.second);
			}
		}
	}
	return -1;
}

bool MeshObject::InBoundingBox(const Ogre::Vector3 &pos, bool check_y)
{
	//determine if position 'pos' is inside the mesh's bounding box

	Ogre::Vector3 position = sbs->ToRemote(pos - GetPosition());

	if (Bounds.isNull() == true)
		return false;

	Ogre::Vector3 min = Bounds.getMinimum();
	Ogre::Vector3 max = Bounds.getMaximum();

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

void MeshObject::GetMeshInformation(const Ogre::Mesh* const mesh, int &vertex_count, Ogre::Vector3* &vertices, int &index_count, unsigned long* &indices, Ogre::AxisAlignedBox &extents)
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
	vertices = new Ogre::Vector3[vertex_count];
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
				Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
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

void MeshObject::DeleteWalls()
{
	//delete all wall objects

	for (int i = 0; i < (int)Walls.size(); i++)
	{
		WallObject *wall = Walls[i];
		if (wall)
		{
			wall->parent_deleting = true;
			delete wall;
			Walls[i] = 0;
		}
	}
}

void MeshObject::DeleteWalls(Object *parent)
{
	//delete walls of specified parent object

	for (int i = 0; i < (int)Walls.size(); i++)
	{
		WallObject *wall = Walls[i];
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

Ogre::Vector3 MeshObject::GetPoint(const std::string &wallname, const Ogre::Vector3 &start, const Ogre::Vector3 &end)
{
	//do a line intersection with a specified wall associated with this mesh object,
	//and return the intersection point

	WallObject *wall = GetWallByName(wallname);

	if (wall)
		return wall->GetPoint(start, end);

	return Ogre::Vector3(0, 0, 0);
}

Ogre::Vector3 MeshObject::GetWallExtents(const std::string &name, float altitude, bool get_max)
{
	//return the X and Z extents of a standard wall (by name) at a specific altitude, by doing a double plane cut

	WallObject *wall = GetWallByName(name);

	if (wall)
		return wall->GetWallExtents(altitude, get_max);

	return Ogre::Vector3(0, 0, 0);
}

Ogre::Vector2 MeshObject::GetExtents(int coord, bool flip_z)
{
	//returns the smallest and largest values from a specified coordinate type
	//(x, y, or z) from the polygons of this mesh object.
	//first parameter must be either 1 (for x), 2 (for y) or 3 (for z)

	float esmall = 0;
	float ebig = 0;
	float tempnum = 0;
	int i = 0;

	//return 0,0 if coord value is out of range
	if (coord < 1 || coord > 3)
		return Ogre::Vector2(0, 0);

	for (i = 0; i < (int)MeshGeometry.size(); i++)
	{
		if (coord == 1)
			tempnum = MeshGeometry[i].vertex.x;
		if (coord == 2)
			tempnum = MeshGeometry[i].vertex.y;
		if (coord == 3)
		{
			if (flip_z == false)
				tempnum = MeshGeometry[i].vertex.z;
			else
				tempnum = -MeshGeometry[i].vertex.z;
		}

		if (i == 0)
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

	return Ogre::Vector2(esmall, ebig);
}

WallObject* MeshObject::FindPolygon(const std::string &name, int &index)
{
	//finds a polygon by name in all associated wall objects
	//returns associated wall object and polygon index

	for (int i = 0; i < (int)Walls.size(); i++)
	{
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

int MeshObject::GetSubmeshCount()
{
	return (int)Submeshes.size();
}

bool MeshObject::IsVisible(Ogre::Camera *camera)
{
	//returns if this mesh object is visible in the provided camera's view frustom or not

	if (IsEnabled() == false || !camera)
		return false;

	//if beyond the max render distance
	if (MeshWrapper->IsVisible(this) == false)
		return false;

	if (GetSubmeshCount() == 0)
		return false;

	if (Bounds.isNull() == true)
		return false;

	Ogre::Vector3 min = Bounds.getMinimum();
	Ogre::Vector3 max = Bounds.getMaximum();
	Ogre::Vector3 pos = sbs->ToRemote(GetPosition());
	Ogre::AxisAlignedBox global_box (pos + min, pos + max);

	return camera->isVisible(global_box);
}

bool MeshObject::IsPhysical()
{
	return is_physical;
}

Ogre::Vector3 MeshObject::GetOffset()
{
	//for models, return bounding box offset value, used to center the mesh

	Bounds.scale(GetSceneNode()->GetRawSceneNode()->getScale());

	if (Bounds.isNull() == true)
		return Ogre::Vector3::ZERO;

	Ogre::Vector3 vec = Bounds.getCenter();
	Ogre::Vector3 min = Bounds.getMinimum();
	Ogre::Vector3 offset (vec.x, -Bounds.getMinimum().y, -vec.z);
	return sbs->ToLocal(offset);
}

void MeshObject::Cut(Ogre::Vector3 start, Ogre::Vector3 end, bool cutwalls, bool cutfloors, int checkwallnumber, bool reset_check)
{
	//cut all walls in this mesh object

	for (int i = 0; i < (int)Walls.size(); i++)
		sbs->Cut(Walls[i], start, end, cutwalls, cutfloors, checkwallnumber, reset_check);
}

void MeshObject::CutOutsideBounds(Ogre::Vector3 start, Ogre::Vector3 end, bool cutwalls, bool cutfloors)
{
	float limit = 1000000;

	Ogre::Vector3 left_min (-limit, -limit, -limit);
	Ogre::Vector3 left_max (start.x, limit, limit);
	Ogre::Vector3 right_min (end.x, -limit, -limit);
	Ogre::Vector3 right_max (limit, limit, limit);
	Ogre::Vector3 front_min (-limit, -limit, -limit);
	Ogre::Vector3 front_max (limit, limit, start.z);
	Ogre::Vector3 back_min (-limit, -limit, end.z);
	Ogre::Vector3 back_max (limit, limit, limit);

	Cut(left_min, left_max, cutwalls, cutfloors);
	Cut(right_min, right_max, cutwalls, cutfloors);
	Cut(front_min, front_max, cutwalls, cutfloors);
	Cut(back_min, back_max, cutwalls, cutfloors);
}

bool MeshObject::LoadFromFile(const std::string &filename, Ogre::MeshPtr &collidermesh)
{
	//load mesh object from a file

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
		sbs->Report("Loading material script " + matname2);
		Ogre::MaterialManager::getSingleton().parseScript(stream, path);

		if(!stream.isNull())
		{
			stream->seek(0);
			while(!stream->eof())
			{
				std::string line = stream->getLine();
				TrimString(line);
				if (StartsWith(line, "material", true) == true)
				{
					Ogre::vector<Ogre::String>::type vec = Ogre::StringUtil::split(line," \t:");
					for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it)
					{
						std::string match = (*it);
						TrimString(match);
						if (!match.empty())
						{
							Ogre::MaterialPtr materialPtr = Ogre::MaterialManager::getSingleton().getByName(match, path);
							if (!materialPtr.isNull())
							{
								sbs->Report("Loading material " + match);
								//materialPtr->compile();
								materialPtr->load();
								materialPtr->setLightingEnabled(false);
								//materialPtr->setAmbient(sbs->AmbientR, sbs->AmbientG, sbs->AmbientB);
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
		sbs->ReportError("Error loading material file " + matname + "\n" + e.getDescription());
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
			std::string colname = filename2.substr(0, filename2.length() - 5) + ".collider";
			colname2 = sbs->VerifyFile(colname);
			collidermesh = Ogre::MeshManager::getSingleton().load(colname2, path);
		}
		catch (Ogre::Exception &e)
		{
			sbs->ReportError("No collider model for " + colname2 + "\n" + e.getDescription());
		}
	}

	model_loaded = true;
	return true;
}

unsigned int MeshObject::GetVertexCount()
{
	return MeshGeometry.size();
}

unsigned int MeshObject::GetTriangleCount(int submesh)
{
	if (submesh < 0 || submesh >= (int)Submeshes.size())
		return 0;

	return Submeshes[submesh].Triangles.size();
}

}
