/* $Id$ */

/*
	Scalable Building Simulator - Mesh and Polygon Classes
	The Skyscraper Project - Version 1.8 Alpha
	Copyright (C)2004-2010 Ryan Thoryk
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

#include "globals.h"
#include "sbs.h"
#include "unix.h"

//this file includes function implementations of the low-level SBS geometry and mesh code

extern SBS *sbs; //external pointer to the SBS engine

void SBS::DumpVertices(WallObject* wallobject)
{
	//dumps a list of vertices from a mesh object to the console/logfile

	Report("--- Polygon Vertex Dump ---\n");
	/*for (int i = 0; i < mesh->getVertexCount(); i++)
	{
		Ogre::Vector3 vertex = mesh->GetVertices()[i];
		Report(Ogre::String(_itoa(i, intbuffer, 10)) + ": " + Ogre::String(_gcvt(vertex.x, 6, buffer)) + ", " + Ogre::String(_gcvt(vertex.y, 6, buffer)) + ", " + Ogre::String(_gcvt(vertex.z, 6, buffer)));
	}*/
}

Ogre::Vector2 SBS::GetExtents(std::vector<Ogre::Vector3> &varray, int coord)
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
			tempnum = varray[i].z;

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

Ogre::Vector2 SBS::GetExtents(MeshObject* mesh, int coord)
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
			tempnum = mesh->MeshGeometry[i].vertex.z;

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
		float dist;
		std::vector<Ogre::Vector3> original;
		std::vector<std::vector<Ogre::Vector3> > origpolys;
		wallarray[index]->GetGeometry(index2, origpolys, true);

		for (int i = 0; i < (int)origpolys[0].size(); i++)
			original.push_back(origpolys[0][i]);

		//TODO: Fix for OGRE
		//csIntersect3::SegmentPlane(ToRemote(start), ToRemote(end), Ogre::Plane(original[0], original[1], original[2]), isect, dist);
		//return ToLocal(isect);
	}
	return Ogre::Vector3(0, 0, 0);
}

void SBS::AddGenWall(MeshObject *mesh, const char *texture, float x1, float z1, float x2, float z2, float height, float altitude, float tw, float th)
{
	//add a simple wall in a general mesh (currently only used for objects that change textures)

	//get texture
	Ogre::String texname = texture;
	bool result;
	Ogre::String material = GetTextureMaterial(texture, result, mesh->MeshWrapper->getName().c_str());
	if (!result)
		texname = "Default";

	if (tw == 0)
		tw = 1;
	if (th == 0)
		th = 1;

	float tw2 = tw, th2 = th;

	float mw, mh;
	if (GetTextureTiling(texname.c_str(), mw, mh))
	{
		//multiply the tiling parameters (tw and th) by
		//the stored multipliers for that texture
		tw2 = tw * mw;
		th2 = th * mh;
	}

	//create texture mapping table
	Ogre::Vector2 table[] = {Ogre::Vector2(tw2, th2), Ogre::Vector2(0, th2), Ogre::Vector2(tw2, 0), Ogre::Vector2(0, 0)};

	//create a quad, map the texture, and append to the mesh
	/*CS::Geometry::TesselatedQuad wall (Ogre::Vector3(ToRemote(x2), ToRemote(altitude), ToRemote(z2)), Ogre::Vector3(ToRemote(x1), ToRemote(altitude), ToRemote(z1)), Ogre::Vector3(ToRemote(x2), ToRemote(altitude + height), ToRemote(z2)));
	CS::Geometry::TableTextureMapper mapper(table);
	wall.SetMapper(&mapper);
	wall.append(mesh->GetFactory());*/

	//set lighting factor
	//mesh->GetMeshObject()->SetColor(csColor(1, 1, 1));

	//set texture
	//mesh->GetMeshObject()->SetMaterialWrapper(material);

	//recreate colliders if specified
	if (RecreateColliders == true)
	{
		DeleteColliders(mesh);
		CreateColliders(mesh);
	}
}


void SBS::Cut(WallObject *wall, const Ogre::Vector3& start, const Ogre::Vector3& end, bool cutwalls, bool cutfloors, const Ogre::Vector3& mesh_origin, const Ogre::Vector3& object_origin, int checkwallnumber, const char *checkstring, bool reset_check)
{
	//cuts a rectangular hole in the polygons within the specified range
	//mesh_origin is a modifier for meshes with relative polygon coordinates (used only for calculating door positions) - in this you specify the mesh's global position
	//object_origin is for the object's (such as a shaft) central position, used for calculating wall offsets

	if (cutwalls == false && cutfloors == false)
		return;

	std::vector<Ogre::Vector3> temppoly, temppoly2, temppoly3, temppoly4, temppoly5, worker;
	std::vector<WallPolygon*> ignore_list;

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
	for (int i = 0; i < wall->GetHandleCount(); i++)
	{
		//skip created submeshes
		bool ignorecheck = false;
		for (int j = 0; j < (int)ignore_list.size(); j++)
		{
			if (ignore_list[j] == wall->GetHandle(i))
			{
				ignorecheck = true;
				break;
			}
		}
		if (ignorecheck == true)
			continue;

		//get name
		Ogre::String name = wall->GetHandle(i)->name;

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
							if (name.find(checkstring) >= 0)
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
						for (int k = 0; k < (int)temppoly.size(); k++)
							newpolys[newpolys.size() - 1].push_back(temppoly[k]);
					}
					if (temppoly2.size() > 2)
					{
						newpolys.resize(newpolys.size() + 1);
						for (int k = 0; k < (int)temppoly2.size(); k++)
							newpolys[newpolys.size() - 1].push_back(temppoly2[k]);
					}
					if (temppoly3.size() > 2)
					{
						newpolys.resize(newpolys.size() + 1);
						for (int k = 0; k < (int)temppoly3.size(); k++)
							newpolys[newpolys.size() - 1].push_back(temppoly3[k]);
					}
					if (temppoly4.size() > 2)
					{
						newpolys.resize(newpolys.size() + 1);
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
		if (polycheck == true && newpolys.size() > 0)
		{
			//get texture data from original polygon
			Ogre::String oldmat = wall->GetHandle(i)->material;
			Ogre::Vector3 oldvector;
			Ogre::Matrix3 mapping;
			wall->GetHandle(i)->GetTextureMapping(mapping, oldvector);

			//delete original polygon
			wall->DeletePolygon(i, false);

			//create new polygon
			WallPolygon* handle = 0;
			handle = wall->AddPolygon(name.c_str(), oldmat, newpolys, mapping, oldvector);
			ignore_list.push_back(handle);

			//reset search position
			i--;
			polycheck = false;
		}
	}

	//recreate colliders if specified
	if (RecreateColliders == true)
	{
		//DeleteColliders(wall);
		//CreateColliders(wall);
	}
}

Ogre::Vector3 SBS::GetWallExtents(std::vector<WallObject*> &wallarray, const char *name, float altitude, bool get_max)
{
	//return the X and Z extents of a standard wall (by name) at a specific altitude, by doing a double plane cut

	Ogre::String newname;
	Ogre::String name2 = name;
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

	Ogre::Vector3 normal = sbs->ComputeNormal(polygon);

	int largest_normal = 0; //x

	if (fabsf(normal.y) > fabsf(normal.x) && fabsf(normal.y) > fabsf(normal.z))
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
	sbs->PolygonCount++;
}

WallPolygon::~WallPolygon()
{
	sbs->PolygonCount--;
}

void WallPolygon::GetGeometry(MeshObject *mesh, std::vector<std::vector<Ogre::Vector3> > &vertices, bool firstonly)
{
	//gets vertex geometry using mesh's vertex extent arrays; returns vertices in 'vertices'

	vertices.resize(index_extents.size());

	for (int i = 0; i < (int)index_extents.size(); i++)
	{
		int min = index_extents[i].x;
		int max = index_extents[i].y;
		for (int j = min; j <= max; j++)
			vertices[i].push_back(sbs->ToLocal(mesh->MeshGeometry[j].vertex));
		if (firstonly == true)
			return;
	}
}

bool WallPolygon::PointInside(MeshObject *mesh, const Ogre::Vector3 &point, bool plane_check)
{
	//check if a point is inside the polygon

	std::vector<std::vector<Ogre::Vector3> > vertices;
	GetGeometry(mesh, vertices, false);

	for (int i = 0; i < (int)vertices.size(); i++)
	{
		if (sbs->InPolygon(vertices[i], point))
			return true;
	}

	return false;
}

MeshObject::MeshObject(Object* parent, const char *name, const char *filename, float max_render_distance, float scale_multiplier)
{
	//set up SBS object
	object = new Object();
	object->SetValues(this, parent, "Mesh", name, false);

	enabled = true;

	Ogre::String buffer;
	Ogre::String Name = name;
	buffer = Ogre::StringConverter::toString(object->GetNumber());
	Name.insert(0, "(" + buffer + ")");

	if (!filename)
	{
		//create mesh
		MeshWrapper = Ogre::MeshManager::getSingleton().createManual(Name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	}
	else
	{
		//load mesh object from collada file
		LoadColladaFile(filename, name);
	}

	//exit if mesh wrapper wasn't created
	if (!MeshWrapper.get())
		return;

	//set zbuf mode to "USE" by default
	//MeshWrapper->SetZBufMode(CS_ZBUF_USE);

	//set render priority to "object" by default
	//MeshWrapper->SetRenderPriority(sbs->engine->GetObjectRenderPriority());

	//create a default material (otherwise the system complains if a mesh is used without a material)
	//if (!MeshWrapper->GetMeshObject()->GetMaterialWrapper())
		//MeshWrapper->GetMeshObject()->SetMaterialWrapper(sbs->engine->GetMaterialList()->FindByName("Default"));

	//set maximum render distance
	//if (max_render_distance > 0)
		//MeshWrapper->SetMaximumRenderDistance(sbs->ToRemote(max_render_distance));

	//rescale if a loaded model
	if (filename)
		RescaleVertices(scale_multiplier);

	//create movable
	Movable = sbs->mSceneManager->createEntity(MeshWrapper->getName());
	SceneNode = sbs->mSceneManager->getRootSceneNode()->createChildSceneNode();
	SceneNode->attachObject(Movable);

	sbs->AddMeshHandle(this);

	//State->SetLighting(false);
	//State->SetShadowCasting(false);
	//State->SetShadowReceiving(true);
	//State->SetManualColors(false);
}

MeshObject::~MeshObject()
{
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

	SceneNode->detachObject(Movable);

	if (sbs->FastDelete == false)
	{
		sbs->DeleteMeshHandle(this);
		//sbs->engine->WantToDie(MeshWrapper);
		MeshWrapper.setNull();
	}

	delete object;
}

void MeshObject::Enable(bool value)
{
	//enables or disables the mesh

	if (value == enabled)
		return;

	/*if (value == true)
	{
		MeshWrapper->GetFlags().Reset(CS_ENTITY_INVISIBLEMESH);
		MeshWrapper->GetFlags().Reset(CS_ENTITY_NOSHADOWS);
		MeshWrapper->GetFlags().Reset(CS_ENTITY_NOHITBEAM);
	}
	else
	{
		MeshWrapper->GetFlags().Set(CS_ENTITY_INVISIBLEMESH);
		MeshWrapper->GetFlags().Set(CS_ENTITY_NOSHADOWS);
		MeshWrapper->GetFlags().Set(CS_ENTITY_NOHITBEAM);
	}*/
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

Ogre::MaterialPtr MeshObject::ChangeTexture(const char *texture, bool matcheck)
{
	//changes a texture
	//if matcheck is true, exit if old and new textures are the same

	//exit if old and new materials are the same
	/*if (matcheck == true)
	{
		if (MeshWrapper->getSubMesh(name)->getMaterialName() == Ogre::String(texture))
			return Ogre::MaterialPtr(0);
	}

	//get new material
	Ogre::MaterialPtr newmat = Ogre::MaterialManager::getSingleton().getByName(texture);

	//set material if valid
	if (newmat.get())
	{
		MeshWrapper->getSubMesh(name)->setMaterialName(texture);
		return newmat;
	}
	else //otherwise report error
		sbs->ReportError("ChangeTexture: Invalid texture '" + Ogre::String(texture) + "'");
*/
	return Ogre::MaterialPtr(0);
}

int MeshObject::FindWall(const Ogre::Vector3 &point)
{
	//find a wall from a 3D point

	for (int i = 0; i < (int)Walls.size(); i++)
	{
		if (Walls[i]->IsPointOnWall(point) == true)
			return i;
	}
	return -1;
}

void MeshObject::RescaleVertices(float multiplier)
{
	//rescale all mesh vertices to the default SBS value (using ToRemote()), times the given multiplier

	//multiply vertex data
	for (int i = 0; i < (int)MeshGeometry.size(); i++)
		MeshGeometry[i].vertex *= multiplier;
}

bool MeshObject::LoadColladaFile(const char *filename, const char *name)
{
	//load a collada file into a new mesh, by converting to native Crystal Space format

	//first verify the filename
	/*Ogre::String File = sbs->VerifyFile(filename);;
	sbs->Report("Loading Collada model file " + File);
	File.insert(0, "data/");

	#define COLLADA_VERSION "1.4.1"
	csRef<iColladaConvertor> collada = csQueryRegistryOrLoad<iColladaConvertor>(sbs->object_reg, "crystalspace.utilities.colladaconvertor");
	if(!collada)
	{
		printf("Collada plugin failed to load\n");
		return false;
	}
	collada->SetOutputFiletype(CS_LIBRARY_FILE);
	if (collada->Load(File) != 0)
		return sbs->ReportError("LoadColladaFile: Error loading collada file");

	if (collada->Convert() != 0)
		return sbs->ReportError("LoadColladaFile: Error converting collada file");
	csRef<iDocument> doc = collada->GetCrystalDocument();

	collection = sbs->engine->CreateCollection(name);
	csLoadResult rc = sbs->loader->Load(doc->GetRoot(), collection, false, true);

	if (!rc.success)
		return sbs->ReportError("LoadColladaFile: Error parsing model");

	csRef<iMeshFactoryWrapper> factory;
	if (rc.result == 0)
	{
		// Library file. Find the last factory in our region.
		iMeshFactoryList* factories = sbs->engine->GetMeshFactories();
		int i;
		for (i = factories->GetCount() - 1 ; i >= 0 ; --i)
		{
			iMeshFactoryWrapper* f = factories->Get(i);
			if (collection->IsParentOf(f->QueryObject()))
			{
				factory = f;
				break;
			}
		}
	}
	else
	{
		factory = scfQueryInterface<iMeshFactoryWrapper> (rc.result);
	}

	if (!factory)
		return sbs->ReportError("LoadColladaFile: Error creating mesh factory");

	//create mesh wrapper and factory
	MeshWrapper = sbs->engine->CreateMeshWrapper(factory, name, sbs->area);
	sbs->Report("Model file loaded");*/
	return true;
}

bool MeshObject::IsEnabled()
{
	return enabled;
}

void MeshObject::Move(const Ogre::Vector3 position, bool relative_x, bool relative_y, bool relative_z, Ogre::Vector3 origin)
{
	//move light - this can only be done on movable lights
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
		pos.z = sbs->ToRemote(origin.z + position.z);
	else
		pos.z = SceneNode->getPosition().z + sbs->ToRemote(position.z);
	SceneNode->setPosition(pos);
}

Ogre::Vector3 MeshObject::GetPosition()
{
	return sbs->ToLocal(SceneNode->getPosition());
}

void MeshObject::SetRotation(const Ogre::Vector3 rotation)
{
	//rotate light
	Ogre::Quaternion x(Ogre::Degree(rotation.x), Ogre::Vector3::UNIT_X);
	Ogre::Quaternion y(Ogre::Degree(rotation.y), Ogre::Vector3::UNIT_Y);
	Ogre::Quaternion z(Ogre::Degree(rotation.z), Ogre::Vector3::UNIT_Z);
	Ogre::Quaternion rot = x * y * z;
	SceneNode->setOrientation(rot);
	rotX = rotation.x;
	rotY = rotation.y;
	rotZ = rotation.z;
}

void MeshObject::Rotate(const Ogre::Vector3 rotation, float speed)
{
	//rotates light in a relative amount
	rotX += rotation.x * speed;
	rotY += rotation.y * speed;
	rotZ += rotation.z * speed;
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
}

void MeshObject::RemoveVertex(int index)
{
	//remove a vertex from the mesh

	if (index < 0 || index >= (int)MeshGeometry.size())
		return;

	MeshGeometry.erase(MeshGeometry.begin() + index);

	//TODO: reindex triangles here
}

void MeshObject::AddTriangle(int submesh, TriangleType &triangle)
{
	//add a triangle to the mesh
	Triangles[submesh].triangles.push_back(triangle);
}

void MeshObject::RemoveTriangle(int submesh, int index)
{
	//remove a triangle from the mesh

	if (index < 0 || index >= (int)Triangles[submesh].triangles.size())
		return;

	Triangles[submesh].triangles.erase(Triangles[submesh].triangles.begin() + index);
}

bool MeshObject::PolyMesh(const char *name, const char *texture, std::vector<Ogre::Vector3> &vertices, float tw, float th, bool autosize, Ogre::Matrix3 &t_matrix, Ogre::Vector3 &t_vector, std::vector<Ogre::Vector2> &mesh_indices, std::vector<TriangleType> &triangles)
{
	//create custom genmesh geometry, apply a texture map and material, and return the created submesh

	//get texture material
	Ogre::String texname = texture;
	bool result;
	Ogre::String material = sbs->GetTextureMaterial(texture, result, name);
	if (!result)
		texname = "Default";

	if (tw == 0)
		tw = 1;
	if (th == 0)
		th = 1;

	/*if (Shaders == true)
	{
		csRef<iStringSet> strset = csQueryRegistryTagInterface<iStringSet> (object_reg, "crystalspace.shared.stringset");
		csRef<iShaderManager> shadermgr = csQueryRegistry<iShaderManager> (object_reg);
		iMaterial* mat = material->GetMaterial();
		//Ogre::StringID t = strset->Request("ambient");
		Ogre::StringID t = strset->Request("diffuse");
		//iShader* sh = shadermgr->GetShader("fullbright");
		//iShader* sh = shadermgr->GetShader("ambient");
		iShader* sh = shadermgr->GetShader("light");
		mat->SetShader(t, sh);
	}*/

	//get autosize information
	Ogre::Vector2 xextents = sbs->GetExtents(vertices, 1);
	Ogre::Vector2 yextents = sbs->GetExtents(vertices, 2);
	Ogre::Vector2 zextents = sbs->GetExtents(vertices, 3);

	Ogre::Vector2 sizing;
	sizing.x = tw;
	sizing.y = th;

	if (autosize == true)
		sizing = sbs->CalculateSizing(texture, xextents, yextents, zextents, tw, th);

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

	//convert to remote positioning
	std::vector<std::vector<Ogre::Vector3> > vertices2;
	vertices2.resize(1);

	for (int i = 0; i < (int)vertices.size(); i++)
		vertices2[0].push_back(sbs->ToRemote(vertices[i]));

	//texture mapping
	Ogre::Vector3 v1, v2, v3;
	sbs->GetTextureMapping(vertices2[0], v1, v2, v3);
	if (!ComputeTextureMap(t_matrix, t_vector, vertices2[0],
		v1,
		Ogre::Vector2 (sbs->MapUV[0].x * tw2, sbs->MapUV[0].y * th2),
		v2,
		Ogre::Vector2 (sbs->MapUV[1].x * tw2, sbs->MapUV[1].y * th2),
		v3,
		Ogre::Vector2 (sbs->MapUV[2].x * tw2, sbs->MapUV[2].y * th2)))
		return false;

	return PolyMesh(name, material, vertices2, t_matrix, t_vector, mesh_indices, triangles, false);
}

bool MeshObject::PolyMesh(const char *name, Ogre::String &material, std::vector<std::vector<Ogre::Vector3> > &vertices, Ogre::Matrix3 &tex_matrix, Ogre::Vector3 &tex_vector, std::vector<Ogre::Vector2> &mesh_indices, std::vector<TriangleType> &triangles, bool convert_vertices)
{
	//create custom genmesh geometry, apply a texture map and material, and return the created submesh

	//convert to remote positioning
	std::vector<std::vector<Ogre::Vector3> > vertices2;
	if (convert_vertices == true)
	{
		vertices2.resize(vertices.size());
		for (int i = 0; i < (int)vertices.size(); i++)
		{
			for (int j = 0; j < (int)vertices[i].size(); j++)
				vertices2[i].push_back(sbs->ToRemote(vertices[i][j]));
		}
	}
	else
		vertices2 = vertices;

	//texture mapping
	Ogre::Vector2 *table = GetTexels(tex_matrix, tex_vector, vertices2);

	//triangulate mesh
	std::vector<TriangleMesh> trimesh;
	trimesh.resize(vertices2.size());

	for (int i = 0; i < (int)trimesh.size(); i++)
	{
		//first fill triangle mesh with polygon's vertices
		for (int j = 0; j < (int)vertices2[i].size(); j++)
			trimesh[i].vertices.push_back(vertices2[i][j]);

		//then do a (very) simple triangulation
		//this method also works (sort of) with non-planar polygons
		for (int j = 2; j < (int)vertices2[i].size(); j++)
			trimesh[i].triangles.push_back(TriangleType(0, j - 1, j));
	}

	//set up geometry array
	std::vector<Geometry> mesh_geometry;

	//initialize geometry arrays
	int size = 0;
	for (int i = 0; i < (int)trimesh.size(); i++)
		size += (int)trimesh[i].vertices.size();
	mesh_geometry.resize(size);

	//get number of existing vertices
	int count = (int)MeshGeometry.size();

	//populate vertices, normals, and texels for mesh data
	int k = 0;

	for (int i = 0; i < (int)trimesh.size(); i++)
	{
		for (int j = 0; j < (int)trimesh[i].vertices.size(); j++)
		{
			mesh_geometry[k].normal = mesh_geometry[k].vertex = trimesh[i].vertices[j];
			mesh_geometry[k].normal.normalise();
			mesh_geometry[k].texel = table[k];

			//add geometry to mesh	
			AddVertex(mesh_geometry[k]);
			k++;
		}
	}

	//delete texel array
	if (table)
		delete table;
	table = 0;

	//delete geometry array
	mesh_geometry.clear();

	//add triangles to single array, to be passed to the submesh
	int location = 0;
	for (int i = 0; i < (int)trimesh.size(); i++)
	{
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

	//create submesh and set material
	ProcessSubMesh(triangles, material, name, true);

	//MeshWrapper->_setBounds(Ogre::AxisAlignedBox(xMin, yMin, zMin, xMax, yMax, zMax));
	//MeshWrapper->_setBoundingSphereRadius(std::max(xMax - xMin, std::max(yMax - yMin, zMax - zMin)) / 2.0f);
	MeshWrapper->load();

	//recreate colliders if specified
	if (sbs->RecreateColliders == true)
	{
		//DeleteColliders(mesh);
		//CreateColliders(mesh);
	}

	return true;
}

Ogre::Vector2* MeshObject::GetTexels(Ogre::Matrix3 &tex_matrix, Ogre::Vector3 &tex_vector, std::vector<std::vector<Ogre::Vector3> > &vertices)
{
	//return texel array for specified texture transformation matrix and vector

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

int MeshObject::ProcessSubMesh(std::vector<TriangleType> &indices, Ogre::String &material, const char *name, bool add)
{
	//processes submeshes for new or removed geometry
	//(uploads vertex and index arrays to graphics card, and applies material)

	//arrays must be populated correctly before this function is called

	float radius = 0;
	Ogre::AxisAlignedBox box;
	std::vector<float> mVertexElements;
	std::vector<unsigned int> mIndices;

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
	if (createnew == false)
	{
		if (Triangles[index].triangles.size() - indices.size() <= 0)
		{
			MeshWrapper->destroySubMesh(index);
			Submeshes.erase(Submeshes.begin() + index);
			Triangles.erase(Triangles.begin() + index);
			return -1;
		}
	}

	//add triangles
	if (add == true)
	{
		for (int i = 0; i < (int)indices.size(); i++)
			AddTriangle(index, indices[i]);
	}
	else
	{
		//remove triangles
		for (int i = 0; i < (int)Triangles[index].triangles.size(); i++)
		{
			for (int j = 0; j < (int)indices.size(); j++)
			{
				if (Triangles[index].triangles[i].x == indices[j].x && Triangles[index].triangles[i].y == indices[j].y && Triangles[index].triangles[i].z == indices[j].z)
				{
					//delete match
					RemoveTriangle(index, i);
					break;
				}
			}
		}
	}

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

	//populate buffer with vertex geometry
	for (size_t i = 0; i < MeshGeometry.size(); i++)
	{
		mVertexElements.push_back(MeshGeometry[i].vertex.x);
		mVertexElements.push_back(MeshGeometry[i].vertex.y);
		mVertexElements.push_back(MeshGeometry[i].vertex.z);
		mVertexElements.push_back(MeshGeometry[i].normal.x);
		mVertexElements.push_back(MeshGeometry[i].normal.y);
		mVertexElements.push_back(MeshGeometry[i].normal.z);
		mVertexElements.push_back(MeshGeometry[i].texel.x);
		mVertexElements.push_back(MeshGeometry[i].texel.y);
		box.merge(MeshGeometry[i].vertex);
		radius = std::max(radius, MeshGeometry[i].vertex.length());
	}
	//create vertex hardware buffer
	Ogre::HardwareVertexBufferSharedPtr vbuffer = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(decl->getVertexSize(0), mVertexElements.size(), Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
	vbuffer->writeData(0, vbuffer->getSizeInBytes(), &mVertexElements, false);

	//bind vertex data to mesh
	data->vertexBufferBinding->setBinding(0, vbuffer);

	//create array of triangle indices
	for (size_t i = 0; i < Triangles[index].triangles.size(); i++)
	{
		mIndices.push_back(Triangles[index].triangles[i].x);
		mIndices.push_back(Triangles[index].triangles[i].y);
		mIndices.push_back(Triangles[index].triangles[i].z);
	}

	//create index hardware buffer
	Ogre::HardwareIndexBufferSharedPtr ibuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(Ogre::HardwareIndexBuffer::IT_32BIT, mIndices.size(), Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
	ibuffer->writeData(0, ibuffer->getSizeInBytes(), &mIndices, false);

	if (createnew == false)
	{
		//delete old submesh
		TriangleIndices tris = Triangles[index];
		MeshWrapper->destroySubMesh(index);
		Submeshes.erase(Submeshes.begin() + index);
		Triangles.erase(Triangles.begin() + index);

		//create new submesh
		submesh = MeshWrapper->createSubMesh(name);
		submesh->useSharedVertices = true;
		Submeshes.push_back(submesh);
		Triangles.push_back(tris);
		index = (int)Submeshes.size() - 1;
	}

	//bind index data to submesh
	submesh->indexData->indexCount = mIndices.size();
	submesh->indexData->indexBuffer = ibuffer;
	submesh->indexData->indexStart = 0;

	//bind material
	//submesh->setMaterialName("Default");

	MeshWrapper->_setBounds(box);
	MeshWrapper->_setBoundingSphereRadius(radius);
	MeshWrapper->reload();
	return index;
}

int MeshObject::FindMatchingSubMesh(Ogre::String material)
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

void MeshObject::DeleteVertices(std::vector<WallObject*> &wallarray, std::vector<TriangleType> &deleted_indices)
{
	//delete related mesh vertices using provided index array
	//then reindex all mesh triangle indices in all submeshes.
	//this should be done after a polygon is deleted
	//also, all wall objects in wall arrays must have the same mesh state

	//exit if wall array is empty
	if (wallarray.size() == 0)
		return;

	//construct new sorted and compressed index array
	std::vector<int> deleted;
	for (int i = 0; i < (int)deleted_indices.size(); i++)
	{
		deleted.push_back(deleted_indices[i].x);
		deleted.push_back(deleted_indices[i].y);
		deleted.push_back(deleted_indices[i].z);
	}
	sort(deleted.begin(), deleted.end());

	//delete specified vertices
	for (int i = (int)deleted.size() - 1; i >= 0; i--)
	{
		MeshGeometry.erase(MeshGeometry.begin() + deleted[i]);
	}

	//TODO - need to reconstruct vertex hardware buffer here

	//reindex triangle indices in all submeshes
	for (int submesh = 0; submesh < (int)Triangles.size(); submesh++)
	{
		std::vector<int> elements;
		for (int i = 0; i < (int)Triangles[submesh].triangles.size(); i++)
		{
			elements.push_back(Triangles[submesh].triangles[i].x);
			elements.push_back(Triangles[submesh].triangles[i].y);
			elements.push_back(Triangles[submesh].triangles[i].z);
		}
		for (int e = 0; e < (int)elements.size(); e++)
		{
			for (int i = (int)deleted.size() - 1; i >= 0; i--)
			{
				if (elements[e] >= deleted[i])
					elements[e]--;
			}
		}
		int element = 0;
		int size = (int)Triangles[submesh].triangles.size();
		Triangles[submesh].triangles.clear();

		for (int i = 0; i < size; i++)
		{
			Triangles[submesh].triangles.push_back(TriangleType(elements[element], elements[element + 1], elements[element + 2]));
			element += 3;
		}
		elements.clear();
	}

	//TODO - need to reconstruct triangle hardware buffer here

	//reindex triangle indices in all wall objects
	for (int i = 0; i < (int)wallarray.size(); i++)
	{
		if (!wallarray[i])
			continue;

		for (int j = 0; j < (int)wallarray[i]->handles.size(); j++)
		{
			//reindex triangle indices

			std::vector<int> elements;
			for (int k = 0; k < (int)wallarray[i]->handles[j].triangles.size(); k++)
			{
				elements.push_back(wallarray[i]->handles[j].triangles[k].x);
				elements.push_back(wallarray[i]->handles[j].triangles[k].y);
				elements.push_back(wallarray[i]->handles[j].triangles[k].z);
			}
			for (int e = 0; e < (int)elements.size(); e++)
			{
				for (int i = (int)deleted.size() - 1; i >= 0; i--)
				{
					if (elements[e] >= deleted[i])
						elements[e]--;
				}
			}
			int element = 0;
			int size = (int)wallarray[i]->handles[j].triangles.size();
			wallarray[i]->handles[j].triangles.clear();

			for (int ii = 0; ii < size; ii++)
			{
				wallarray[ii]->handles[j].triangles.push_back(TriangleType(elements[element], elements[element + 1], elements[element + 2]));
				element += 3;
			}
			elements.clear();

			//reindex extents, used for getting original geometry
			for (int k = (int)deleted.size() - 1; k >= 0; k--)
			{
				for (int m = 0; m < (int)wallarray[i]->handles[j].index_extents.size(); m++)
				{
					Ogre::Vector2 extents = wallarray[i]->handles[j].index_extents[m];
					if (deleted[k] < extents.x)
						extents.x--;
					if (deleted[k] < extents.y)
						extents.y--;
					wallarray[i]->handles[j].index_extents[m] = extents;
				}
			}
		}
	}
}
