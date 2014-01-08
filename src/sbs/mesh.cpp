/* $Id$ */

/*
	Scalable Building Simulator - Mesh and Polygon Classes
	The Skyscraper Project - Version 1.9 Alpha
	Copyright (C)2004-2014 Ryan Thoryk
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
#include <OgreResourceGroupManager.h>
#include <OgreSceneManager.h>
#include <OgreMaterialManager.h>
#include <OgreEntity.h>
#include <OgreBulletDynamicsRigidBody.h>
#include <OgreMath.h>
#include <Shapes/OgreBulletCollisionsTrimeshShape.h>
#include <Shapes/OgreBulletCollisionsBoxShape.h>
#include "globals.h"
#include "sbs.h"
#include "unix.h"

//this file includes function implementations of the low-level SBS geometry and mesh code

extern SBS *sbs; //external pointer to the SBS engine

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

Ogre::Vector2 SBS::GetExtents(MeshObject* mesh, int coord, bool flip_z)
{
	//returns the smallest and largest values from a specified coordinate type
	//(x, y, or z) from a vertex array (polygon).
	//first parameter must be a mesh object
	//second must be either 1 (for x), 2 (for y) or 3 (for z)

	float esmall = 0;
	float ebig = 0;
	float tempnum = 0;
	int i = 0;

	//return 0,0 if coord value is out of range
	if (coord < 1 || coord > 3)
		return Ogre::Vector2(0, 0);

	for (i = 0; i < (int)mesh->MeshGeometry.size(); i++)
	{
		if (coord == 1)
			tempnum = mesh->MeshGeometry[i].vertex.x;
		if (coord == 2)
			tempnum = mesh->MeshGeometry[i].vertex.y;
		if (coord == 3)
		{
			if (flip_z == false)
				tempnum = mesh->MeshGeometry[i].vertex.z;
			else
				tempnum = -mesh->MeshGeometry[i].vertex.z;
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

Ogre::Vector3 SBS::GetPoint(std::vector<WallObject*> &wallarray, const char *polyname, const Ogre::Vector3 &start, const Ogre::Vector3 &end)
{
	//do a line intersection with a specified mesh, and return
	//the intersection point

	int index = -1;
	int index2 = -1;
	for (int i = 0; i < (int)wallarray.size(); i++)
	{
		index2 = wallarray[i]->FindPolygon(polyname);
		if (index2 > -1)
		{
			index = i;
			break;
		}
	}

	if (index >= 0)
	{
		//do a plane intersection with a line
		Ogre::Vector3 isect;
		float dist = 0;
		std::vector<Ogre::Vector3> original;
		std::vector<std::vector<Ogre::Vector3> > origpolys;
		wallarray[index]->GetGeometry(index2, origpolys, true);

		original.reserve(origpolys[0].size());
		for (int i = 0; i < (int)origpolys[0].size(); i++)
			original.push_back(origpolys[0][i]);

		//TODO: Fix for OGRE
		//csIntersect3::SegmentPlane(ToRemote(start), ToRemote(end), Ogre::Plane(original[0], original[1], original[2]), isect, dist);
		//return ToLocal(isect);
	}
	return Ogre::Vector3(0, 0, 0);
}

void SBS::Cut(WallObject *wall, const Ogre::Vector3& start, const Ogre::Vector3& end, bool cutwalls, bool cutfloors, const Ogre::Vector3& mesh_origin, const Ogre::Vector3& object_origin, int checkwallnumber, const char *checkstring, bool reset_check)
{
	//cuts a rectangular hole in the polygons within the specified range
	//mesh_origin is a modifier for meshes with relative polygon coordinates (used only for calculating door positions) - in this you specify the mesh's global position
	//object_origin is for the object's (such as a shaft) central position, used for calculating wall offsets

	if (cutwalls == false && cutfloors == false)
		return;

	std::vector<Ogre::Vector3> temppoly, temppoly2, temppoly3, temppoly4, temppoly5, worker;

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
	int polycount = wall->GetHandleCount();
	for (int i = 0; i < polycount; i++)
	{
		//get name
		std::string name = wall->GetHandle(i)->name;

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
			if (temppoly.capacity() < origpolys[j].size())
				temppoly.reserve(origpolys[j].size());
			for (int k = 0; k < (int)origpolys[j].size(); k++)
				temppoly.push_back(origpolys[j][k]);

			//make sure the polygon is not outside the cut area
			if (sbs->Classify(0, temppoly, start.x) != 1 &&
					sbs->Classify(0, temppoly, end.x) != 2 &&
					sbs->Classify(1, temppoly, start.y) != 1 &&
					sbs->Classify(1, temppoly, end.y) != 2 &&
					sbs->Classify(2, temppoly, start.z) != 1 &&
					sbs->Classify(2, temppoly, end.z) != 2)
			{
				if (Verbose)
					Report("Cutting polygon " + name);

				extentsx = GetExtents(temppoly, 1);
				extentsy = GetExtents(temppoly, 2);
				extentsz = GetExtents(temppoly, 3);

				//is polygon a wall?
				if (extentsy.x != extentsy.y)
				{
					if (cutwalls == true)
					{
						//wall
						if (fabs(extentsx.x - extentsx.y) > fabs(extentsz.x - extentsz.y))
						{
							//wall is facing forward/backward

							//get left side
							worker = temppoly;
							sbs->SplitWithPlane(0, worker, temppoly, temppoly2, start.x);
							worker.clear();

							//get right side
							if (temppoly2.size() > 0)
								worker = temppoly2;
							else
								worker = temppoly;
							sbs->SplitWithPlane(0, worker, temppoly3, temppoly2, end.x);
							worker.clear();

							//get lower
							if (temppoly3.size() > 0)
								worker = temppoly3;
							else if (temppoly2.size() > 0)
								worker = temppoly2;
							else if (temppoly.size() > 0)
								worker = temppoly;
							sbs->SplitWithPlane(1, worker, temppoly3, temppoly4, start.y);
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
							sbs->SplitWithPlane(1, worker, temppoly5, temppoly4, end.y);
							worker.clear();
						}
						else
						{
							//wall is facing left/right

							//get left side
							worker = temppoly;
							sbs->SplitWithPlane(2, worker, temppoly, temppoly2, start.z);
							worker.clear();

							//get right side
							if (temppoly2.size() > 0)
								worker = temppoly2;
							else
								worker = temppoly;
							sbs->SplitWithPlane(2, worker, temppoly3, temppoly2, end.z);
							worker.clear();

							//get lower
							if (temppoly3.size() > 0)
								worker = temppoly3;
							else if (temppoly2.size() > 0)
								worker = temppoly2;
							else if (temppoly.size() > 0)
								worker = temppoly;
							sbs->SplitWithPlane(1, worker, temppoly3, temppoly4, start.y);
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
							sbs->SplitWithPlane(1, worker, temppoly5, temppoly4, end.y);
							worker.clear();
						}
						polycheck = true;
						polycheck2 = true;
						//store extents of temppoly5 for door sides if needed
						if (checkwallnumber > 0 && checkwallnumber < 3)
						{
							if ((int)name.find(checkstring) >= 0)
							{
								float extent;
								if (checkwallnumber == 2 && (wall2a == false || wall2b == false))
								{
									//level walls
									if (wall2a == true)
										wall2b = true;
									wall2a = true;
									extent = GetExtents(temppoly5, 1).x + mesh_origin.x;
									if (wall2b == false || (wall2b == true && fabs(extent - object_origin.x) > fabs(wall_extents_x.x - object_origin.x)))
										wall_extents_x.x = extent;
									extent = GetExtents(temppoly5, 3).x + mesh_origin.z;
									if (wall2b == false || (wall2b == true && fabs(extent - object_origin.z) > fabs(wall_extents_z.x - object_origin.z)))
										wall_extents_z.x = extent;
									wall_extents_y = GetExtents(temppoly5, 2) + mesh_origin.y;
								}
								else if (wall1a == false || wall1b == false)
								{
									//shaft walls
									if (wall1a == true)
										wall1b = true;
									wall1a = true;
									extent = GetExtents(temppoly5, 1).y + mesh_origin.x;
									if (wall1b == false || (wall1b == true && fabs(extent - object_origin.x) < fabs(wall_extents_x.y - object_origin.x)))
										wall_extents_x.y = extent;
									extent = GetExtents(temppoly5, 3).y + mesh_origin.z;
									if (wall1b == false || (wall1b == true && fabs(extent - object_origin.z) < fabs(wall_extents_z.y - object_origin.z)))
										wall_extents_z.y = extent;
								}
							}
						}
					}
				}
				else if (cutfloors == true)
				{
					//floor

					//get left side
					worker = temppoly;
					sbs->SplitWithPlane(0, worker, temppoly, temppoly2, start.x);
					worker.clear();

					//get right side
					if (temppoly2.size() > 0)
						worker = temppoly2;
					else
						worker = temppoly;
					sbs->SplitWithPlane(0, worker, temppoly3, temppoly2, end.x);
					worker.clear();

					//get lower
					if (temppoly3.size() > 0)
						worker = temppoly3;
					else if (temppoly2.size() > 0)
						worker = temppoly2;
					else if (temppoly.size() > 0)
						worker = temppoly;
					sbs->SplitWithPlane(2, worker, temppoly3, temppoly4, start.z);
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
					sbs->SplitWithPlane(2, worker, temppoly5, temppoly4, end.z);
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

			if (newpolys.size() > 0)
			{
				//get texture data from original polygon
				oldmat = wall->GetHandle(i)->material;
				wall->GetHandle(i)->GetTextureMapping(mapping, oldvector);
			}

			//delete original polygon
			wall->DeletePolygon(i, false);

			if (newpolys.size() > 0)
			{
				//create new polygon
				WallPolygon* handle = 0;
				handle = wall->AddPolygon(name.c_str(), oldmat, newpolys, mapping, oldvector);
			}

			//reset search position
			i--;
			polycount--;
			polycheck = false;
		}
	}
}

Ogre::Vector3 SBS::GetWallExtents(std::vector<WallObject*> &wallarray, const char *name, float altitude, bool get_max)
{
	//return the X and Z extents of a standard wall (by name) at a specific altitude, by doing a double plane cut

	std::string newname;
	std::string name2 = name;
	for (int i = 0; i < 6; i++)
	{
		if (i == 0)
			newname = name2;
		if (i == 1)
			newname = name2 + ":0";
		if (i == 2)
			newname = name2 + ":1";
		if (i == 3)
			newname = name2 + ":front";
		if (i == 4)
			newname = name2 + ":back";
		if (i == 5)
			newname = name2 + ":left";
		if (i == 6)
			newname = name2 + ":right";

		int index = -1;
		int index2 = -1;
		for (int j = 0; j < (int)wallarray.size(); j++)
		{
			index2 = wallarray[j]->FindPolygon(newname.c_str());
			if (index2 > -1)
			{
				index = j;
				break;
			}
		}

		if (index >= 0)
		{
			std::vector<std::vector<Ogre::Vector3> > origpolys;
			wallarray[index]->GetGeometry(index2, origpolys, true);

			std::vector<Ogre::Vector3> original, tmp1, tmp2;
			original.reserve(origpolys[0].size());
			for (int i = 0; i < (int)origpolys[0].size(); i++)
				original.push_back(origpolys[0][i]);

			//if given altitude is outside of polygon's range, return 0
			Ogre::Vector2 yextents = GetExtents(original, 2);
			float tmpaltitude = altitude;
			if (tmpaltitude < yextents.x || tmpaltitude > yextents.y)
				return Ogre::Vector3(0, 0, 0);

			//get upper
			sbs->SplitWithPlane(1, original, tmp1, tmp2, tmpaltitude - 0.001);

			//get lower part of upper
			sbs->SplitWithPlane(1, tmp2, original, tmp1, tmpaltitude + 0.001);

			Ogre::Vector3 result;
			if (get_max == false)
			{
				//get minimum extents
				result.x = GetExtents(original, 1).x;
				result.z = GetExtents(original, 3).x;
			}
			else
			{
				//get maximum extents
				result.x = GetExtents(original, 1).y;
				result.z = GetExtents(original, 3).y;
			}
			result.y = altitude;
			return result;
		}
	}
	return Ogre::Vector3(0, 0, 0);
}

Ogre::Vector3 SBS::GetPolygonDirection(std::vector<Ogre::Vector3> &polygon)
{
	//returns the direction the polygon faces, in a 3D vector
	//for example, <-1, 0, 0> means that the wall faces left.

	//get largest normal

	//convert to remote values for precision compatibility with Alpha 7 and earlier
	std::vector<Ogre::Vector3> newpoly;
	for (int i = 0; i < polygon.size(); i++)
		newpoly.push_back(Ogre::Vector3(ToRemote(polygon[i].x), ToRemote(polygon[i].y), ToRemote(polygon[i].z)));

	float D = 0;
	Ogre::Vector3 normal = sbs->ComputeNormal(newpoly, D);

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

void WallPolygon::GetTextureMapping(Ogre::Matrix3 &tm, Ogre::Vector3 &tv)
{
	//return texture mapping matrix and vector
	tm = t_matrix;
	tv = t_vector;
}

Ogre::SubMesh* WallPolygon::GetSubMesh()
{
	//return the submesh this polygon is in
	int index = mesh->FindMatchingSubMesh(material);
	return mesh->Submeshes[index];
}

WallPolygon::WallPolygon()
{
}

WallPolygon::~WallPolygon()
{
}

void WallPolygon::GetGeometry(MeshObject *mesh, std::vector<std::vector<Ogre::Vector3> > &vertices, bool firstonly, bool convert, bool rescale, bool relative, bool reverse)
{
	//gets vertex geometry using mesh's vertex extent arrays; returns vertices in 'vertices'

	vertices.resize(index_extents.size());

	for (int i = 0; i < (int)index_extents.size(); i++)
	{
		int min = index_extents[i].x;
		int max = index_extents[i].y;
		int newsize = vertices[i].size() + max - min + 1;
		vertices[i].reserve(newsize);
		if (reverse == false)
		{
			for (int j = min; j <= max; j++)
			{
				if (relative == true)
				{
					if (convert == true)
						vertices[i].push_back(sbs->ToLocal(mesh->MeshGeometry[j].vertex, rescale));
					else
						vertices[i].push_back(mesh->MeshGeometry[j].vertex);
				}
				else
				{
					if (convert == true)
						vertices[i].push_back(sbs->ToLocal(mesh->MeshGeometry[j].vertex + mesh->GetPosition(), rescale));
					else
						vertices[i].push_back(mesh->MeshGeometry[j].vertex + mesh->GetPosition());
				}
			}
		}
		else
		{
			for (int j = max; j >= min; j--)
			{
				if (relative == true)
				{
					if (convert == true)
						vertices[i].push_back(sbs->ToLocal(mesh->MeshGeometry[j].vertex, rescale));
					else
						vertices[i].push_back(mesh->MeshGeometry[j].vertex);
				}
				else
				{
					if (convert == true)
						vertices[i].push_back(sbs->ToLocal(mesh->MeshGeometry[j].vertex + mesh->GetPosition(), rescale));
					else
						vertices[i].push_back(mesh->MeshGeometry[j].vertex + mesh->GetPosition());
				}
			}
		}
		if (firstonly == true)
			return;
	}
}

bool WallPolygon::PointInside(MeshObject *mesh, const Ogre::Vector3 &point, bool plane_check, bool convert)
{
	//check if a point is inside the polygon

	std::vector<std::vector<Ogre::Vector3> > vertices;
	GetGeometry(mesh, vertices, false, convert);

	for (int i = 0; i < (int)vertices.size(); i++)
	{
		if (sbs->InPolygon(vertices[i], point))
			return true;
	}

	return false;
}

MeshObject::MeshObject(Object* parent, const char *name, const char *filename, float max_render_distance, float scale_multiplier, bool enable_physics, float restitution, float friction, float mass)
{
	//set up SBS object
	object = new Object();
	object->SetValues(this, parent, "Mesh", name, false);

	enabled = true;
	mBody = 0;
	mShape = 0;
	SceneNode = 0;
	Movable = 0;
	prepared = false;
	IsPhysical = enable_physics;
	this->restitution = restitution;
	this->friction = friction;
	this->mass = mass;
	no_collider = false;
	tricollider = true;
	MeshGeometry.reserve(128); //reserve vertex space

	Ogre::MeshPtr collidermesh;

	std::string buffer;
	std::string Name = name;
	buffer = ToString(object->GetNumber());
	Name.insert(0, "(" + buffer + ")");
	this->name = Name;
	std::string filename2;

	if (!filename)
	{
		//create mesh
		MeshWrapper = Ogre::MeshManager::getSingleton().createManual(Name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	}
	else
	{
		//load mesh object from a file
		std::string filename1 = "data/";
		filename1.append(filename);
		filename2 = sbs->VerifyFile(filename1.c_str());
		std::string path = sbs->GetMountPath(filename2.c_str(), filename2);
		std::string matname;

		//load material file
		try
		{
			matname = filename2.substr(0, filename2.length() - 5) + ".material";
			std::string matname2 = sbs->VerifyFile(matname.c_str());
			Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(matname2, path);
			sbs->Report("Loading material script " + matname2);
			Ogre::MaterialManager::getSingleton().parseScript(stream, path);

			if(!stream.isNull())
			{
				stream->seek(0);
				while(!stream->eof())
				{
					std::string line = stream->getLine();
					Ogre::StringUtil::trim(line);
					if (Ogre::StringUtil::startsWith(line, "material"))
					{
						Ogre::vector<Ogre::String>::type vec = Ogre::StringUtil::split(line," \t:");
						for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it)
						{
							std::string match = (*it);
							Ogre::StringUtil::trim(match);
							if (!match.empty())
							{
								Ogre::MaterialPtr materialPtr = Ogre::MaterialManager::getSingleton().getByName(match, path);
								if (!materialPtr.isNull())
				                {
									sbs->Report("Loading material " + match);
				                    //materialPtr->compile();
				                    materialPtr->load();
									materialPtr->setLightingEnabled(true);
									materialPtr->setAmbient(sbs->AmbientR, sbs->AmbientG, sbs->AmbientB);
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
		try
		{
			MeshWrapper = Ogre::MeshManager::getSingleton().load(filename2, path);
		}
		catch (Ogre::Exception &e)
		{
			sbs->ReportError("Error loading model " + filename2 + "\n" + e.getDescription());
			return;
		}

		std::string colname2;

		//load collider model
		try
		{
			std::string colname = filename2.substr(0, filename2.length() - 5) + ".collider";
			colname2 = sbs->VerifyFile(colname.c_str());
			collidermesh = Ogre::MeshManager::getSingleton().load(colname2, path);
		}
		catch (Ogre::Exception &e)
		{
			sbs->ReportError("No collider model for " + colname2 + "\n" + e.getDescription());
		}
	}

	//exit if mesh wrapper wasn't created
	if (!MeshWrapper.get())
		return;

	//set render priority to "object" by default
	//MeshWrapper->SetRenderPriority(sbs->engine->GetObjectRenderPriority());

	//create a default material (otherwise the system complains if a mesh is used without a material)
	//if (!MeshWrapper->GetMeshObject()->GetMaterialWrapper())
		//MeshWrapper->GetMeshObject()->SetMaterialWrapper(sbs->engine->GetMaterialList()->FindByName("Default"));

	//create movable
	if (!filename)
		Movable = sbs->mSceneManager->createEntity(Name);
	else
		Movable = sbs->mSceneManager->createEntity(filename2);
	//Movable->setCastShadows(true);
	SceneNode = sbs->mSceneManager->getRootSceneNode()->createChildSceneNode(Name);
	SceneNode->attachObject(Movable);

	//rescale if a loaded model
	if (filename)
		SceneNode->setScale(sbs->ToRemote(scale_multiplier), sbs->ToRemote(scale_multiplier), sbs->ToRemote(scale_multiplier));

	//set maximum render distance
	if (max_render_distance > 0)
		Movable->setRenderingDistance(sbs->ToRemote(max_render_distance));

	sbs->AddMeshHandle(this);

	//set up collider for model (if mesh loaded from a filename)
	if (filename)
	{
		if (collidermesh.get())
		{
			//create collider based on provided mesh collider
			size_t vertex_count, index_count;
			Ogre::Vector3* vertices;
			long unsigned int* indices;
			Ogre::AxisAlignedBox box;
			GetMeshInformation(collidermesh.getPointer(), vertex_count, vertices, index_count, indices, sbs->ToRemote(scale_multiplier), box);
			CreateColliderFromModel(vertex_count, vertices, index_count, indices);
			delete[] vertices;
			delete[] indices;
			Ogre::MeshManager::getSingleton().remove(collidermesh->getName());
		}
		else
		{
			//create generic box collider if separate mesh collider isn't available
			CreateBoxCollider(sbs->ToRemote(scale_multiplier));
		}
	}

	//State->SetLighting(false);
	//State->SetShadowCasting(false);
	//State->SetShadowReceiving(true);
	//State->SetManualColors(false);
}

MeshObject::~MeshObject()
{
	//delete physics/collider components
	DeleteCollider();

	//delete wall objects
	for (int i = 0; i < (int)Walls.size(); i++)
	{
		if (Walls[i])
		{
			Walls[i]->parent_deleting = true;
			delete Walls[i];
		}
		Walls[i] = 0;
	}

	std::string nodename;
	if (SceneNode)
	{
		SceneNode->detachAllObjects();
		SceneNode->getParent()->removeChild(SceneNode);
		sbs->mSceneManager->destroySceneNode(SceneNode->getName());
		sbs->mSceneManager->destroyEntity(Movable->getName());
	}
	SceneNode = 0;
	Movable = 0;

	if (sbs->FastDelete == false)
	{
		sbs->DeleteMeshHandle(this);
		Ogre::MeshManager::getSingleton().remove(name);
		MeshWrapper.setNull();
	}

	delete object;
}

void MeshObject::Enable(bool value, bool remove)
{
	//enables or disables the mesh

	if (value == enabled)
		return;

	SBS_PROFILE("MeshObject::Enable");

	//attach or detach from scenegraph
	if (value == false)
		SceneNode->detachObject(Movable);
	else
		SceneNode->attachObject(Movable);

	//enable or disable collision detection
	if (mBody)
	{
		if (remove == false)
		{
			//if removed from world and 'remove' is false, add to world
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

	//show scenenode bounding box
	//SceneNode->showBoundingBox(value);

	enabled = value;
}

WallObject* MeshObject::CreateWallObject(Object *parent, const char *name)
{
	//create a new wall object in the given array

	WallObject *wall = new WallObject(this);
	wall->name = name;
	wall->parent_array = &Walls;
	wall->SetValues(wall, parent, "Wall", name, false);
	Walls.push_back(wall);
	return wall;
}

Ogre::MaterialPtr MeshObject::ChangeTexture(const char *texture, bool matcheck, int submesh)
{
	//changes a texture
	//if matcheck is true, exit if old and new textures are the same

	SBS_PROFILE("MeshObject::ChangeTexture");
	std::string tex = sbs->VerifyFile(texture);
	std::string path = sbs->GetMountPath(texture, tex);
	TrimString(tex);

	if (MeshWrapper->getNumSubMeshes() == 0)
		return Ogre::MaterialPtr(0);

	//exit if old and new materials are the same
	if (matcheck == true)
	{
		if (MeshWrapper->getSubMesh(submesh)->getMaterialName() == tex)
			return Ogre::MaterialPtr(0);
	}

	//get new material
	Ogre::MaterialPtr newmat = Ogre::MaterialManager::getSingleton().getByName(tex, path);
	if (!newmat.get())
	{
		sbs->ReportError("ChangeTexture: Invalid texture '" + tex + "'");
		return Ogre::MaterialPtr(0);
	}

	//set material if valid
	MeshWrapper->getSubMesh(submesh)->setMaterialName(tex, path);

	//apply changes (refresh mesh state)
	MeshWrapper->_dirtyState();

	return newmat;
}

bool MeshObject::ReplaceTexture(const std::string &oldtexture, const std::string &newtexture)
{
	//replace submesh materials named oldtexture with newtexture
	int submesh = FindMatchingSubMesh(oldtexture);
	if (submesh >= 0)
	{
		Ogre::MaterialPtr result = ChangeTexture(newtexture.c_str(), true, submesh);
		if (!result.isNull())
			return true;
	}
	return false;
}

int MeshObject::FindWall(const Ogre::Vector3 &point, bool convert)
{
	//find a wall from a 3D point

	SBS_PROFILE("MeshObject::FindWall");
	for (int i = 0; i < (int)Walls.size(); i++)
	{
		if (Walls[i]->IsPointOnWall(point, convert) == true)
			return i;
	}
	return -1;
}

int MeshObject::FindWallIntersect(const Ogre::Vector3 &start, const Ogre::Vector3 &end, Ogre::Vector3 &isect, float &distance, Ogre::Vector3 &normal, bool convert, bool rescale)
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
		for (int j = 0; j < (int)Walls[i]->handles.size(); j++)
		{
			if (Walls[i]->handles[j].IntersectSegment(this, start, end, cur_isect, &pr, tmpnormal, convert, rescale) == true)
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

	return best_i;
}

void MeshObject::RescaleVertices(float multiplier)
{
	//rescale all mesh vertices to the default SBS value (using ToRemote()), times the given multiplier

	//multiply vertex data
	for (int i = 0; i < (int)MeshGeometry.size(); i++)
		MeshGeometry[i].vertex *= multiplier;
}

bool MeshObject::IsEnabled()
{
	return enabled;
}

void MeshObject::Move(const Ogre::Vector3 position, bool relative_x, bool relative_y, bool relative_z, Ogre::Vector3 origin)
{
	SBS_PROFILE("MeshObject::Move");
	Ogre::Vector3 pos;
	if (relative_x == false)
		pos.x = sbs->ToRemote(origin.x + position.x);
	else
		pos.x = SceneNode->getPosition().x + sbs->ToRemote(position.x);
	if (relative_y == false)
		pos.y = sbs->ToRemote(origin.y + position.y);
	else
		pos.y = SceneNode->getPosition().y + sbs->ToRemote(position.y);
	if (relative_z == false)
		pos.z = sbs->ToRemote(-(origin.z + position.z));
	else
		pos.z = SceneNode->getPosition().z + sbs->ToRemote(-position.z);
	SceneNode->setPosition(pos);
	if (mBody)
		mBody->updateTransform(true, false, false);
}

Ogre::Vector3 MeshObject::GetPosition()
{
	return sbs->ToLocal(SceneNode->getPosition());
}

void MeshObject::SetRotation(const Ogre::Vector3 rotation)
{
	//rotate mesh
	Ogre::Quaternion x(Ogre::Degree(rotation.x), Ogre::Vector3::UNIT_X);
	Ogre::Quaternion y(Ogre::Degree(rotation.y), Ogre::Vector3::NEGATIVE_UNIT_Y);
	Ogre::Quaternion z(Ogre::Degree(rotation.z), Ogre::Vector3::UNIT_Z);
	Ogre::Quaternion rot = x * y * z;
	SceneNode->setOrientation(rot);
	rotX = rotation.x;
	rotY = rotation.y;
	rotZ = rotation.z;
	if (mBody)
		mBody->updateTransform(false, true, false);
}

void MeshObject::Rotate(const Ogre::Vector3 rotation, float speed)
{
	//rotates mesh in a relative amount
	rotX += rotation.x * speed;
	rotY += rotation.y * speed;
	rotZ += rotation.z * speed;
	if (rotX > 360)
		rotX = rotX - 360;
	if (rotY > 360)
		rotY = rotY - 360;
	if (rotZ > 360)
		rotZ = rotZ - 360;
	if (rotX < 0)
		rotX = rotX + 360;
	if (rotY < 0)
		rotY = rotY + 360;
	if (rotZ < 0)
		rotZ = rotZ + 360;
	SetRotation(Ogre::Vector3(rotX, rotY, rotZ));
}

Ogre::Vector3 MeshObject::GetRotation()
{
	return Ogre::Vector3(rotX, rotY, rotZ);
}

void MeshObject::AddVertex(Geometry &vertex_geom)
{
	//add a vertex to the mesh
	MeshGeometry.push_back(vertex_geom);
	prepared = false; //need to re-prepare mesh
}

void MeshObject::RemoveVertex(int index)
{
	//remove a vertex from the mesh

	if (index < 0 || index >= (int)MeshGeometry.size())
		return;

	MeshGeometry.erase(MeshGeometry.begin() + index);

	//TODO: reindex triangles here

	prepared = false; //need to re-prepare mesh
}

void MeshObject::AddTriangle(int submesh, TriangleType &triangle)
{
	//add a triangle to the mesh
	Triangles[submesh].triangles.push_back(triangle);
	prepared = false; //need to re-prepare mesh
}

void MeshObject::RemoveTriangle(int submesh, int index)
{
	//remove a triangle from the mesh

	if (index < 0 || index >= (int)Triangles[submesh].triangles.size())
		return;

	Triangles[submesh].triangles.erase(Triangles[submesh].triangles.begin() + index);
	prepared = false; //need to re-prepare mesh
}

bool MeshObject::PolyMesh(const char *name, const char *texture, std::vector<Ogre::Vector3> &vertices, float tw, float th, bool autosize, Ogre::Matrix3 &t_matrix, Ogre::Vector3 &t_vector, std::vector<Ogre::Vector2> &mesh_indices, std::vector<TriangleType> &triangles)
{
	//create custom mesh geometry, apply a texture map and material, and return the created submesh

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
	sbs->GetTextureMapping(vertices2[0], v1, v2, v3, direction);

	if (tw == 0)
		tw = 1;
	if (th == 0)
		th = 1;

	//get autosize information
	Ogre::Vector2 sizing;
	sizing.x = tw;
	sizing.y = th;

	if (autosize == true)
		sizing = sbs->CalculateSizing(texture, sbs->ToLocal(v1), sbs->ToLocal(v2), sbs->ToLocal(v3), direction, tw, th);

	//get texture tiling information
	float tw2 = sizing.x, th2 = sizing.y;
	float mw, mh;
	if (sbs->GetTextureTiling(texname.c_str(), mw, mh))
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

bool MeshObject::PolyMesh(const char *name, std::string &material, std::vector<std::vector<Ogre::Vector3> > &vertices, Ogre::Matrix3 &tex_matrix, Ogre::Vector3 &tex_vector, std::vector<Ogre::Vector2> &mesh_indices, std::vector<TriangleType> &triangles, float tw, float th, bool convert_vertices)
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
	TriangleMesh *trimesh = new TriangleMesh[vertices2.size()];
	int trimesh_size = vertices2.size();

	for (int i = 0; i < trimesh_size; i++)
	{
		//first fill triangle mesh with polygon's vertices
		trimesh[i].vertices.reserve(vertices2[i].size());
		for (int j = 0; j < (int)vertices2[i].size(); j++)
			trimesh[i].vertices.push_back(vertices2[i][j]);

		//then do a (very) simple triangulation
		//this method also somewhat works with non-planar polygons
		trimesh[i].triangles.reserve(vertices2[i].size() - 2);
		for (int j = 2; j < (int)vertices2[i].size(); j++)
			trimesh[i].triangles.push_back(TriangleType(0, j - 1, j));
	}

	//set up geometry array
	Geometry *mesh_geometry = 0;

	//initialize geometry arrays
	int size = 0;
	for (int i = 0; i < trimesh_size; i++)
		size += (int)trimesh[i].vertices.size();
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
		for (int j = 0; j < (int)trimesh[i].vertices.size(); j++)
		{
			mesh_geometry[k].normal = mesh_geometry[k].vertex = trimesh[i].vertices[j];
			mesh_geometry[k].normal.normalise();
			mesh_geometry[k].texel = table[k];

			//add geometry to mesh	
			AddVertex(mesh_geometry[k]);
			k++;
		}
		int max = count + k - 1;
		mesh_indices.push_back(Ogre::Vector2(min, max));
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
			TriangleType tri = trimesh[i].triangles[j];
			tri.x += count + location;
			tri.y += count + location;
			tri.z += count + location;
			triangles.push_back(tri);
		}
		location += (int)trimesh[i].vertices.size();
	}

	//delete trimesh array
	delete [] trimesh;
	trimesh = 0;

	//create submesh and set material
	ProcessSubMesh(triangles, material, name, true);

	MeshWrapper->load();

	//recreate colliders if specified
	if (sbs->DeleteColliders == true)
		DeleteCollider();

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
				texels[index].x = -texel_temp.x; //flip X for compatibility with right-hand coordinate system
				texels[index].y = texel_temp.y;
				index++;
			}
		}
		return texels;
	}
	else
	{
		Ogre::Vector2 *texels = new Ogre::Vector2[4];
		texels[0].x = tw;
		texels[0].y = 0;
		texels[1].x = 0;
		texels[1].y = 0;
		texels[2].x = 0;
		texels[2].y = th;
		texels[3].x = tw;
		texels[3].y = th;
		return texels;
	}
	return 0;
}

int MeshObject::ProcessSubMesh(std::vector<TriangleType> &indices, std::string &material, const char *name, bool add)
{
	//processes submeshes for new or removed geometry
	//the Prepare() function must be called when the mesh is ready to view, in order to upload data to graphics card

	int index_count = (int)indices.size();
	TriangleType *indexarray = new TriangleType[index_count];

	for (int i = 0; i < index_count; i++)
		indexarray[i] = indices[i];

	//first get related submesh
	int index = FindMatchingSubMesh(material);
	Ogre::SubMesh *submesh = 0;
	bool createnew = false;

	//get existing submesh pointer
	if (index == -1)
	{
		createnew = true;
		
		//create submesh
		submesh = MeshWrapper->createSubMesh(name);
		submesh->useSharedVertices = true;
		Submeshes.push_back(submesh);
		index = (int)Submeshes.size() - 1;
		Triangles.resize(Triangles.size() + 1);
	}
	else
		submesh = Submeshes[index];

	//delete submesh and exit if it's going to be emptied
	if (createnew == false && add == false)
	{
		if (Triangles[index].triangles.size() - index_count <= 0)
		{
			MeshWrapper->destroySubMesh(index);
			Submeshes.erase(Submeshes.begin() + index);
			Triangles.erase(Triangles.begin() + index);
			delete [] indexarray;
			return -1;
		}
	}

	//add triangles
	if (add == true)
	{
		//reserve triangles
		int size = Triangles[index].triangles.size() + index_count;
		if (Triangles[index].triangles.capacity() < size)
			Triangles[index].triangles.reserve(size);

		for (int i = 0; i < index_count; i++)
			AddTriangle(index, indexarray[i]);
	}
	else
	{
		//remove triangles
		for (int i = 0; i < (int)Triangles[index].triangles.size(); i++)
		{
			TriangleType *triangle = &Triangles[index].triangles[i];
			for (int j = 0; j < index_count; j++)
			{
				if (triangle->x == indexarray[j].x && triangle->y == indexarray[j].y && triangle->z == indexarray[j].z)
				{
					//delete match
					RemoveTriangle(index, i);
					break;
				}
			}
		}
	}

	//bind material
	submesh->setMaterialName(material);

	delete [] indexarray;
	return index;
}

void MeshObject::Prepare()
{
	//prepare mesh object
	//collects and uploads geometry and triangle data to graphics card, and prepares mesh for rendering
	//arrays must be populated correctly before this function is called

	//All submeshes share mesh vertex data, but triangle indices are stored in each submesh
	//each submesh represents a portion of the mesh that uses the same material

	//exit if mesh has already been prepared
	if (prepared == true)
		return;

	//exit if there's no submesh data
	if (Submeshes.size() == 0)
		return;

	Ogre::Real radius = 0;
	Ogre::AxisAlignedBox box;

	//set up vertex buffer
	OGRE_DELETE MeshWrapper->sharedVertexData;
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
	for (int index = 0; index < Submeshes.size(); index++)
	{
		//get submesh object
		Ogre::SubMesh *submesh = MeshWrapper->getSubMesh(index);

		//set up index data array
		unsigned int isize = (unsigned int)Triangles[index].triangles.size() * 3;
		unsigned short *mIndices = new unsigned short[isize];

		//create array of triangle indices
		loc = 0;
		for (size_t i = 0; i < Triangles[index].triangles.size(); i++)
		{
			mIndices[loc] = Triangles[index].triangles[i].x;
			mIndices[loc + 1] = Triangles[index].triangles[i].y;
			mIndices[loc + 2] = Triangles[index].triangles[i].z;
			loc += 3;
		}

		//create index hardware buffer
		Ogre::HardwareIndexBufferSharedPtr ibuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(Ogre::HardwareIndexBuffer::IT_16BIT, isize, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
		ibuffer->writeData(0, ibuffer->getSizeInBytes(), mIndices, true);
		delete [] mIndices;

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
	MeshWrapper->_setBoundingSphereRadius(radius);
}

int MeshObject::FindMatchingSubMesh(std::string material)
{
	//find a submesh with a matching material
	//returns array index

	for (int i = 0; i < (int)Submeshes.size(); i++)
	{
		if (Submeshes[i]->getMaterialName() == material)
			return i;
	}
	return -1;
}

void MeshObject::DeleteVertices(std::vector<TriangleType> &deleted_indices)
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
		if (find(deleted_v.begin(), deleted_v.end(), deleted_indices[i].x) == deleted_v.end())
			deleted_v.push_back(deleted_indices[i].x);
		if (find(deleted_v.begin(), deleted_v.end(), deleted_indices[i].y) == deleted_v.end())
			deleted_v.push_back(deleted_indices[i].y);
		if (find(deleted_v.begin(), deleted_v.end(), deleted_indices[i].z) == deleted_v.end())
			deleted_v.push_back(deleted_indices[i].z);
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
	for (int submesh = 0; submesh < (int)Triangles.size(); submesh++)
	{
		TriangleIndices *indiceslist = &Triangles[submesh];

		int elements_size = indiceslist->triangles.size() * 3;
		int *elements = new int[elements_size];

		int elements_pos = 0;
		for (int i = 0; i < (int)indiceslist->triangles.size(); i++)
		{
			elements[elements_pos] = indiceslist->triangles[i].x;
			elements_pos++;
			elements[elements_pos] = indiceslist->triangles[i].y;
			elements_pos++;
			elements[elements_pos] = indiceslist->triangles[i].z;
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
		int size = (int)indiceslist->triangles.size();
		indiceslist->triangles.clear();
		if (indiceslist->triangles.capacity() < size)
			indiceslist->triangles.reserve(size);

		for (int i = 0; i < size; i++)
		{
			//check if triangle indices are valid
			if (elements[element] >= 0 || elements[element + 1] >= 0 || elements[element + 2] >= 0)
				indiceslist->triangles.push_back(TriangleType(elements[element], elements[element + 1], elements[element + 2]));
			element += 3;
		}
		delete [] elements;
	}

	//reindex triangle indices in all wall objects
	for (int i = 0; i < (int)Walls.size(); i++)
	{
		if (!Walls[i])
			continue;

		for (int j = 0; j < (int)Walls[i]->handles.size(); j++)
		{
			//reindex triangle indices

			WallPolygon *poly = &Walls[i]->handles[j];

			int elements_size = poly->triangles.size() * 3;
			int *elements = new int[elements_size];

			int elements_pos = 0;
			for (int k = 0; k < (int)poly->triangles.size(); k++)
			{
				elements[elements_pos] = poly->triangles[k].x;
				elements_pos++;
				elements[elements_pos] = poly->triangles[k].y;
				elements_pos++;
				elements[elements_pos] = poly->triangles[k].z;
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
			if (poly->triangles.capacity() < size)
				poly->triangles.reserve(size);

			for (int ii = 0; ii < size; ii++)
			{
				//check if triangle indices are valid
				if (elements[element] >= 0 || elements[element + 1] >= 0 || elements[element + 2] >= 0)
					poly->triangles.push_back(TriangleType(elements[element], elements[element + 1], elements[element + 2]));
				element += 3;
			}

			//reindex extents, used for getting original geometry
			for (int k = deleted_size - 1; k >= 0; k--)
			{
				int size = (int)poly->index_extents.size();
				for (int m = 0; m < size; m++)
				{
					Ogre::Vector2 extents = poly->index_extents[m];
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
	Movable->setDebugDisplayEnabled(value);
}

void MeshObject::CreateCollider()
{
	//set up triangle collider based on raw SBS mesh geometry

	//exit if collider already exists
	if (mBody)
		return;

	//exit if mesh is empty
	if (MeshGeometry.size() == 0 || Triangles.size() == 0)
		return;

	int tricount = 0;
	for (int i = 0; i < Triangles.size(); i++)
		tricount += Triangles[i].triangles.size();

	try
	{
		//initialize collider shape
		OgreBulletCollisions::TriangleMeshCollisionShape* shape = new OgreBulletCollisions::TriangleMeshCollisionShape(MeshGeometry.size(), tricount * 3);

		//add vertices to shape
		for (int i = 0; i < Submeshes.size(); i++)
		{
			for (int j = 0; j < Triangles[i].triangles.size(); j++)
			{
				TriangleType tri(0, 0, 0);
				tri.x = Triangles[i].triangles[j].x;
				tri.y = Triangles[i].triangles[j].y;
				tri.z = Triangles[i].triangles[j].z;
				shape->AddTriangle(MeshGeometry[tri.x].vertex, MeshGeometry[tri.y].vertex, MeshGeometry[tri.z].vertex);
			}
		}

		//finalize shape
		shape->Finish();
		std::string name = MeshWrapper->getName();

		//physics is not supported on triangle meshes; use CreateBoxCollider instead
		mBody = new OgreBulletDynamics::RigidBody(name, sbs->mWorld);
		mBody->setStaticShape(SceneNode, shape, 0.1, 0.5, false);
		mShape = shape;

		if (sbs->DeleteColliders == true)
		{
			Enable(false, false);
			Enable(true, false);
		}
	}
	catch (Ogre::Exception &e)
	{
		sbs->ReportError("Error creating collider for " + name + "\n" + e.getDescription());
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

void MeshObject::CreateColliderFromModel(size_t &vertex_count, Ogre::Vector3* &vertices, size_t &index_count, unsigned long* &indices)
{
	//set up triangle collider based on loaded model geometry

	//exit of collider already exists
	if (mBody)
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
		std::string name = SceneNode->getName();

		//physics is not supported on triangle meshes; use CreateBoxCollider instead
		mBody = new OgreBulletDynamics::RigidBody(name, sbs->mWorld);
		mBody->setStaticShape(SceneNode, shape, 0.1, 0.5, false);
		mShape = shape;
	}
	catch (Ogre::Exception &e)
	{
		sbs->ReportError("Error creating model collider for " + name + "\n" + e.getDescription());
	}
}

void MeshObject::CreateBoxCollider(float scale_multiplier)
{
	//set up a box collider for full extents of a mesh

	//exit of collider already exists
	if (mBody)
		return;

	try
	{
		//initialize collider shape
		Ogre::Vector3 bounds = MeshWrapper->getBounds().getHalfSize() * scale_multiplier;
		OgreBulletCollisions::BoxCollisionShape* shape = new OgreBulletCollisions::BoxCollisionShape(bounds);
		std::string name = SceneNode->getName();

		Ogre::AxisAlignedBox box = MeshWrapper->getBounds();
		box.scale(Ogre::Vector3(scale_multiplier, scale_multiplier, scale_multiplier));

		mBody = new OgreBulletDynamics::RigidBody(name, sbs->mWorld);
		if (IsPhysical == false)
			mBody->setStaticShape(SceneNode, shape, 0.1, 0.5, false);
		else
			mBody->setShape(SceneNode, shape, restitution, friction, mass);
		mShape = shape;
	}
	catch (Ogre::Exception &e)
	{
		sbs->ReportError("Error creating box collider for " + name + "\n" + e.getDescription());
	}
}

int MeshObject::HitBeam(const Ogre::Vector3 &origin, const Ogre::Vector3 &direction, int max_distance)
{
	//cast a ray and return the collision distance to the mesh
	//return -1 if no hit

	//cast a ray from the camera position downwards
	SBS_PROFILE("MeshObject::HitBeam");
	Ogre::Ray ray (sbs->ToRemote(origin) -  SceneNode->getPosition(), direction);
	for (int i = 0; i < Triangles.size(); i++)
	{
		for (int j = 0; j < Triangles[i].triangles.size(); j++)
		{
			Ogre::Vector3 tri_a, tri_b, tri_c;
			tri_a = MeshGeometry[Triangles[i].triangles[j].x].vertex;
			tri_b = MeshGeometry[Triangles[i].triangles[j].y].vertex;
			tri_c = MeshGeometry[Triangles[i].triangles[j].z].vertex;

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

	Ogre::Vector3 pos2 = sbs->ToRemote(pos);
	pos2 -= SceneNode->getPosition();
	Ogre::Vector3 min = MeshWrapper->getBounds().getMinimum();
	Ogre::Vector3 max = MeshWrapper->getBounds().getMaximum();
	if (pos2.x >= min.x && pos2.x <= max.x && pos2.z >= min.z && pos2.z <= max.z)
	{
		if (check_y == false)
			return true;
		else
		{
			if (pos2.y >= min.y && pos2.y <= max.y)
				return true;
		}
	}
	return false;
}

void MeshObject::GetMeshInformation(const Ogre::Mesh* const mesh, size_t &vertex_count, Ogre::Vector3* &vertices, size_t &index_count, unsigned long* &indices, float scale_multiplier, Ogre::AxisAlignedBox &extents)
{
    bool added_shared = false;
    size_t current_offset = 0;
    size_t shared_offset = 0;
    size_t next_offset = 0;
    size_t index_offset = 0;

    vertex_count = index_count = 0;

    // Calculate how many vertices and indices we're going to need
    for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);

        // We only need to add the shared vertices once
        if(submesh->useSharedVertices)
        {
            if( !added_shared )
            {
                vertex_count += mesh->sharedVertexData->vertexCount;
                added_shared = true;
            }
        }
        else
        {
            vertex_count += submesh->vertexData->vertexCount;
        }
        // Add the indices
        index_count += submesh->indexData->indexCount;
    }

    // Allocate space for the vertices and indices
    vertices = new Ogre::Vector3[vertex_count];
    indices = new unsigned long[index_count];

    added_shared = false;

    // Run through the submeshes again, adding the data into the arrays
    for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
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

            // There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
            //  as second argument. So make it float, to avoid trouble when Ogre::Real will
            //  be comiled/typedefed as double:
            //Ogre::Real* pReal;
            float* pReal;

            for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
            {
                posElem->baseVertexPointerToElement(vertex, &pReal);
                Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
                //vertices[current_offset + j] = (orient * (pt * scale)) + position;
                vertices[current_offset + j] = pt * scale_multiplier;
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

        if ( use32bitindexes )
        {
            for ( size_t k = 0; k < numTris*3; ++k)
            {
                indices[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
            }
        }
        else
        {
            for ( size_t k = 0; k < numTris*3; ++k)
            {
                indices[index_offset++] = static_cast<unsigned long>(pShort[k]) +
                                          static_cast<unsigned long>(offset);
            }
        }

        ibuf->unlock();
        current_offset = next_offset;
    }
}
