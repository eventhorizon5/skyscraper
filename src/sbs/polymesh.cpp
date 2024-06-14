/*
	Scalable Building Simulator - PolyMesh Geometry Processor
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

#include <OgreBulletDynamicsRigidBody.h>
#include <Shapes/OgreBulletCollisionsTrimeshShape.h>
#include "globals.h"
#include "sbs.h"
#include "camera.h"
#include "wall.h"
#include "texture.h"
#include "profiler.h"
#include "scenenode.h"
#include "dynamicmesh.h"
#include "mesh.h"
#include "polygon.h"
#include "polymesh.h"

//this file includes function implementations of the low-level SBS geometry processing code

namespace SBS {

Vector2 SBS::GetExtents(PolyArray &varray, int coord, bool flip_z)
{
	//returns the smallest and largest values from a specified coordinate type
	//(x, y, or z) from a vertex array (polygon).
	//first parameter must be a vertex array
	//second must be either 1 (for x), 2 (for y) or 3 (for z)

	Real esmall = 0;
	Real ebig = 0;
	Real tempnum = 0;
	size_t i = 0;

	//return 0,0 if coord value is out of range
	if (coord < 1 || coord > 3)
		return Vector2(0, 0);

	for (i = 0; i < varray.size(); i++)
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

	return Vector2(esmall, ebig);
}

void SBS::Cut(Wall *wall, Vector3 start, Vector3 end, bool cutwalls, bool cutfloors, int checkwallnumber, bool reset_check)
{
	//cuts a rectangular hole in the polygons within the specified range

	if (cutwalls == false && cutfloors == false)
		return;

	//swap values if the first is greater than the second
	if (start.x > end.x)
		std::swap(start.x, end.x);
	if (start.y > end.y)
		std::swap(start.y, end.y);
	if (start.z > end.z)
		std::swap(start.z, end.z);

	PolyArray temppoly, temppoly2, temppoly3, temppoly4, temppoly5, worker;

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
		PolygonSet origpolys;
		PolygonSet newpolys;
		wall->GetGeometry(i, origpolys);

		//skip empty meshes
		if (origpolys.size() == 0)
			continue;

		//cut all polygons within range
		for (size_t j = 0; j < origpolys.size(); j++)
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
			Vector2 extentsx, extentsy, extentsz;
			Ogre::AxisAlignedBox bounds (start, end);
			Ogre::AxisAlignedBox polybounds = Ogre::AxisAlignedBox::BOX_NULL;
			bool polycheck2 = false;

			//copy source polygon vertices
			for (size_t k = 0; k < origpolys[j].size(); k++)
			{
				temppoly.push_back(origpolys[j][k]);
				polybounds.merge(origpolys[j][k]);
			}

			//skip if the polygon is completely inside the bounding box
			/*if (bounds.contains(polybounds) == true)
			{
				polycheck = true;
				continue;
			}*/

			//make sure the polygon intersects bounds (is not outside the cut area)
			if (bounds.intersects(polybounds) == true)
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
						if (std::abs(extentsx.x - extentsx.y) > std::abs(extentsz.x - extentsz.y))
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
						for (size_t k = 0; k < temppoly.size(); k++)
							newpolys[newpolys.size() - 1].push_back(temppoly[k]);
					}
					if (temppoly2.size() > 2)
					{
						newpolys.resize(newpolys.size() + 1);
						if (newpolys[newpolys.size() - 1].capacity() < temppoly2.size())
							newpolys[newpolys.size() - 1].reserve(temppoly2.size());
						for (size_t k = 0; k < temppoly2.size(); k++)
							newpolys[newpolys.size() - 1].push_back(temppoly2[k]);
					}
					if (temppoly3.size() > 2)
					{
						newpolys.resize(newpolys.size() + 1);
						if (newpolys[newpolys.size() - 1].capacity() < temppoly3.size())
							newpolys[newpolys.size() - 1].reserve(temppoly3.size());
						for (size_t k = 0; k < temppoly3.size(); k++)
							newpolys[newpolys.size() - 1].push_back(temppoly3[k]);
					}
					if (temppoly4.size() > 2)
					{
						newpolys.resize(newpolys.size() + 1);
						if (newpolys[newpolys.size() - 1].capacity() < temppoly4.size())
							newpolys[newpolys.size() - 1].reserve(temppoly4.size());
						for (size_t k = 0; k < temppoly4.size(); k++)
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
			Vector3 oldvector;
			Matrix3 mapping;
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
				wall->AddPolygonSet(name, oldmat, newpolys, mapping, oldvector);

			//reset search position
			i--;
			polycount--;
			polycheck = false;
		}
	}
}

void SBS::GetDoorwayExtents(MeshObject *mesh, int checknumber, PolyArray &polygon)
{
	//calculate doorway extents, for use with AddDoorwayWalls function
	//checknumber is 1 when checking shaft walls, and 2 when checking floor walls

	if (checknumber > 0 && checknumber < 3)
	{
		Vector3 mesh_position = mesh->GetPosition();
		Real extent;

		if (wall2a == false || wall2b == false)
		{
			if (checknumber == 2)
			{
				//level walls
				if (wall2a == true)
					wall2b = true;
				wall2a = true;
				extent = GetExtents(polygon, 1).x + mesh_position.x;
				if (wall2b == false || (wall2b == true && std::abs(extent - mesh_position.x) > std::abs(wall_extents_x.x - mesh_position.x)))
					wall_extents_x.x = extent;
				extent = GetExtents(polygon, 3).x + mesh_position.z;
				if (wall2b == false || (wall2b == true && std::abs(extent - mesh_position.z) > std::abs(wall_extents_z.x - mesh_position.z)))
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
				if (wall1b == false || (wall1b == true && std::abs(extent - mesh_position.x) < std::abs(wall_extents_x.y - mesh_position.x)))
					wall_extents_x.y = extent;
				extent = GetExtents(polygon, 3).y + mesh_position.z;
				if (wall1b == false || (wall1b == true && std::abs(extent - mesh_position.z) < std::abs(wall_extents_z.y - mesh_position.z)))
					wall_extents_z.y = extent;
			}
		}
	}
}

Vector3 SBS::GetPolygonDirection(PolyArray &polygon)
{
	//returns the direction the polygon faces, in a 3D vector
	//for example, <-1, 0, 0> means that the wall faces left.

	//get largest normal

	//convert to remote values for precision compatibility with Alpha 7 and earlier
	PolyArray newpoly;
	for (size_t i = 0; i < polygon.size(); i++)
		newpoly.push_back(ToRemote(polygon[i], true, false));

	Vector3 normal = ComputePlane(newpoly, false).normal;

	int largest_normal = 0; //x

	if (std::abs(normal.y) >= std::abs(normal.x) && std::abs(normal.y) >= std::abs(normal.z))
		largest_normal = 1; //y biggest
	else if (std::abs(normal.z) > std::abs(normal.x))
		largest_normal = 2; //z biggest

	if (largest_normal == 0)
	{
		if (normal.x < 0)
			return Vector3(1, 0, 0);
		else
			return Vector3(-1, 0, 0);
	}

	if (largest_normal == 1)
	{
		if (normal.y < 0)
			return Vector3(0, 1, 0);
		else
			return Vector3(0, -1, 0);
	}

	if (largest_normal == 2)
	{
		if (normal.z < 0)
			return Vector3(0, 0, 1);
		else
			return Vector3(0, 0, -1);
	}
	return Vector3(0, 0, 0);
}

Vector2 SBS::GetEndPoint(const Vector2 &StartPoint, Real angle, Real distance)
{
	//calculate 2D endpoint from given starting point, along with angle and distance (magnitude)
	//angle is in degrees

	angle -= 90;
	Real newangle = DegreesToRadians(angle);
	Real x = StartPoint.x + distance * cos(newangle);
	Real y = -StartPoint.y + distance * sin(newangle);
	Vector2 result (x, -y);
	return result;

}

Plane SBS::ComputePlane(PolyArray &vertices, bool flip_normal)
{
	//compute plane from a set of given vertices

	Real det;
	Vector3 normal;
	if (flip_normal == true)
		normal = -ComputeNormal(vertices, det);
	else
		normal = ComputeNormal(vertices, det);
	normal.normalise();
	return Plane(normal, det);
}

PolyMesh::PolyMesh(MeshObject *mesh) : ObjectBase(mesh)
{
	this->mesh = mesh;
}

PolyMesh::~PolyMesh()
{
}

bool PolyMesh::ChangeTexture(const std::string &texture, bool matcheck, int submesh)
{
	//changes a texture
	//if matcheck is true, exit if old and new textures are the same

	if (sbs->Headless == true)
		return true;

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

	std::string old = Submeshes[submesh].Name;

	sbs->GetTextureManager()->DecrementTextureUsage(old);

	//update submesh
	Submeshes[submesh].Name = material;

	sbs->GetTextureManager()->IncrementTextureUsage(material);

	bool result = mesh->MeshWrapper->ChangeTexture(old, material, mesh);

	if (result == false)
	{
		Submeshes[submesh].Name = old; //revert name
		return false;
	}

	//update associated polygons
	for (size_t i = 0; i < mesh->Walls.size(); i++)
	{
		for (int j = 0; j < mesh->Walls[i]->GetPolygonCount(); j++)
		{
			Polygon *poly = mesh->Walls[i]->GetPolygon(j);

			if (poly->material == old)
				poly->material = material;
		}
	}

	return true;
}

bool PolyMesh::ReplaceTexture(const std::string &oldtexture, const std::string &newtexture)
{
	//replace submesh materials named oldtexture with newtexture
	int submesh = FindMatchingSubMesh(oldtexture);
	bool result = false;
	if (submesh >= 0)
		result = ChangeTexture(newtexture, true, submesh);
	return result;
}

Wall* PolyMesh::FindWallIntersect(const Vector3 &start, const Vector3 &end, Vector3 &isect, Real &distance, Vector3 &normal, Wall *wall)
{
	//find a wall from a 3D point
	//positions need to be in remote (Ogre) positioning
	//if wall_number is 0 or greater, this will only check that specified wall

	SBS_PROFILE("MeshObject::FindWallIntersect");
	Real pr, best_pr = 2000000000.;
	Real dist, best_dist = 2000000000.;
	int best_i = -1;
	Vector3 cur_isect;
	Vector3 tmpnormal;

	for (size_t i = 0; i < mesh->Walls.size(); i++)
	{
		if (wall && mesh->Walls[i] != wall)
			continue;

		for (int j = 0; j < mesh->Walls[i]->GetPolygonCount(); j++)
		{
			if (mesh->Walls[i]->GetPolygon(j)->IntersectSegment(start, end, cur_isect, &pr, tmpnormal) == true)
			{
				if (pr < best_pr)
				{
					//currently test against previous camera intersection test to fix some weird issues
					Vector3 orig_start = sbs->ToRemote(sbs->camera->HitPosition);
					dist = orig_start.distance(cur_isect);

					if (dist < best_dist)
					{
						//calculate distance to intersection
						distance = start.distance(cur_isect);

						best_dist = dist;
						best_pr = pr;
						best_i = (int)i;
						isect = cur_isect;
						normal = tmpnormal;
					}
				}
			}
		}
	}

	if (best_i >= 0)
		return mesh->Walls[best_i];
	else
		return 0;
}

bool PolyMesh::CreateMesh(const std::string &name, const std::string &texture, PolyArray &vertices, Real tw, Real th, bool autosize, Matrix3 &t_matrix, Vector3 &t_vector, std::vector<Extents> &mesh_indices, std::vector<Triangle> &triangles, PolygonSet &converted_vertices)
{
	//create custom mesh geometry, apply a texture map and material, and return the created submesh

	//exit if this mesh is a loaded file
	if (mesh->Filename != "")
		return ReportError("PolyMesh: Cannot create geometry in a file-loaded mesh");

	//get texture material
	std::string texname = texture;
	bool result;
	std::string material = sbs->GetTextureManager()->GetTextureMaterial(texture, result, true, name);
	if (!result)
		texname = "Default";

	//convert to remote positioning
	converted_vertices.resize(1);

	converted_vertices[0].reserve(vertices.size());
	for (size_t i = 0; i < vertices.size(); i++)
		converted_vertices[0].push_back(sbs->ToRemote(vertices[i]));

	//texture mapping
	Vector3 v1, v2, v3;
	int direction;

	//get texture mapping coordinates
	if (!sbs->GetTextureManager()->GetTextureMapping(converted_vertices[0], v1, v2, v3, direction))
		return ReportError("PolyMesh: Texture mapping error");

	if (tw == 0)
		tw = 1;
	if (th == 0)
		th = 1;

	//get autosize information
	Vector2 sizing (tw, th);

	if (autosize == true)
		sizing = sbs->GetTextureManager()->CalculateSizing(texname, sbs->ToLocal(v1), sbs->ToLocal(v2), sbs->ToLocal(v3), direction, tw, th);

	//get texture tiling information
	Real tw2 = sizing.x, th2 = sizing.y;
	Real mw, mh;
	if (sbs->GetTextureManager()->GetTextureTiling(texname, mw, mh))
	{
		//multiply the tiling parameters (tw and th) by
		//the stored multipliers for that texture
		tw2 = sizing.x * mw;
		th2 = sizing.y * mh;
	}

	if (!sbs->GetTextureManager()->ComputeTextureMap(t_matrix, t_vector, converted_vertices[0], v1, v2, v3, tw2, th2))
		return false;

	return CreateMesh(name, material, converted_vertices, t_matrix, t_vector, mesh_indices, triangles, converted_vertices, tw2, th2, false);
}

bool PolyMesh::CreateMesh(const std::string &name, const std::string &material, PolygonSet &vertices, Matrix3 &tex_matrix, Vector3 &tex_vector, std::vector<Extents> &mesh_indices, std::vector<Triangle> &triangles, PolygonSet &converted_vertices, Real tw, Real th, bool convert_vertices)
{
	//create custom geometry, apply a texture map and material, and return the created submesh
	//tw and th are only used when overriding texel map

	//convert to remote positioning
	if (convert_vertices == true)
	{
		converted_vertices.resize(vertices.size());
		for (size_t i = 0; i < vertices.size(); i++)
		{
			converted_vertices[i].reserve(vertices[i].size());
			for (size_t j = 0; j < vertices[i].size(); j++)
				converted_vertices[i].push_back(sbs->ToRemote(vertices[i][j]));
		}
	}
	else
		converted_vertices = vertices;

	//texture mapping
	Vector2 *table = GetTexels(tex_matrix, tex_vector, converted_vertices, tw, th);

	//triangulate mesh
	TriangleIndices *trimesh = new TriangleIndices[converted_vertices.size()];
	size_t trimesh_size = converted_vertices.size();

	for (size_t i = 0; i < trimesh_size; i++)
	{
		//do a (very) simple triangulation
		//this method also somewhat works with non-planar polygons
		trimesh[i].triangles.reserve(converted_vertices[i].size() - 2);
		for (size_t j = 2; j < converted_vertices[i].size(); j++)
			trimesh[i].triangles.push_back(Triangle(0, j - 1, j));
	}

	//set up geometry array
	std::vector<Geometry> geometry;

	//initialize geometry arrays
	size_t size = 0;
	for (size_t i = 0; i < trimesh_size; i++)
		size += converted_vertices[i].size();
	geometry.resize(size);

	//populate vertices, normals, and texels for mesh data
	unsigned int k = 0;

	if (mesh_indices.capacity() < mesh_indices.size() + trimesh_size)
		mesh_indices.reserve(mesh_indices.size() + trimesh_size);

	for (size_t i = 0; i < trimesh_size; i++)
	{
		unsigned int min = k;
		for (size_t j = 0; j < converted_vertices[i].size(); j++)
		{
			//calculate normal
			Vector3 normal = sbs->ComputePlane(converted_vertices[i], false).normal;

			geometry[k].vertex = converted_vertices[i][j];
			geometry[k].normal = normal;
			geometry[k].texel = table[k];
			k++;
		}
		unsigned int max = k - 1;
		mesh_indices.push_back(Extents(min, max));
	}

	//delete texel array
	if (table)
		delete [] table;
	table = 0;

	//add triangles to single array, to be passed to the submesh
	size_t location = 0;
	for (size_t i = 0; i < trimesh_size; i++)
	{
		if (triangles.capacity() < trimesh[i].triangles.size())
			triangles.reserve(trimesh[i].triangles.size());
		for (size_t j = 0; j < trimesh[i].triangles.size(); j++)
		{
			Triangle tri = trimesh[i].triangles[j];
			tri += location;
			triangles.push_back(tri);
		}
		location += converted_vertices[i].size();
	}

	//delete trimesh array
	delete [] trimesh;
	trimesh = 0;

	//create submesh and set material
	int index = ProcessSubMesh(geometry, triangles, material, true);

	if (index >= 0)
	{
		for (size_t i = 0; i < mesh_indices.size(); i++)
		{
			unsigned int size = Submeshes[index].MeshGeometry.size() - geometry.size();
			mesh_indices[i].min += size;
			mesh_indices[i].max += size;
		}
	}

	//recreate colliders if specified
	if (sbs->DeleteColliders == true)
		DeleteCollider();

	if (sbs->RenderOnStartup == true)
		mesh->Prepare();

	return true;
}

Vector2* PolyMesh::GetTexels(Matrix3 &tex_matrix, Vector3 &tex_vector, PolygonSet &vertices, Real tw, Real th)
{
	//return texel array for specified texture transformation matrix and vector

	if (sbs->TexelOverride == false)
	{
		//create array for texel map
		size_t texel_count = 0;
		for (size_t i = 0; i < vertices.size(); i++)
			texel_count += vertices[i].size();
		Vector2 *texels = new Vector2[texel_count];

		//transform matrix into texel map
		size_t index = 0;
		Vector3 texel_temp;
		for (size_t i = 0; i < vertices.size(); i++)
		{
			for (size_t j = 0; j < vertices[i].size(); j++)
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
		Vector2 *texels = new Vector2[4];
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

int PolyMesh::ProcessSubMesh(std::vector<Geometry> &vertices, std::vector<Triangle> &indices, const std::string &material, bool add)
{
	//processes submeshes for new or removed geometry
	//the Prepare() function must be called when the mesh is ready to view
	//the 'vertices' data is not needed if removing triangles

	//first get related submesh
	int index = FindMatchingSubMesh(material);
	bool createnew = false;

	//exit if trying to remove indices from non-existing submesh
	if (index == -1 && add == false)
		return -1;

	//create a new submesh if needed
	if (index == -1)
	{
		//exit if vertex count is not valid
		if (vertices.size() < 3)
			return -1;

		index = (int)Submeshes.size();
		Submeshes.resize(Submeshes.size() + 1);
		Submeshes[index].MeshGeometry.reserve(64); //reserve vertex space
		Submeshes[index].Triangles.reserve(16); //reserve triangle space
		createnew = true;
	}

	//delete submesh and exit if it's going to be emptied
	if (createnew == false && add == false)
	{
		if ((int)Submeshes[index].Triangles.size() - (int)indices.size() <= 0)
		{
			//unregister texture usage
			if (Submeshes[index].Name != "")
				sbs->GetTextureManager()->DecrementTextureUsage(Submeshes[index].Name);

			//remove submesh
			Submeshes.erase(Submeshes.begin() + index);
			mesh->ResetPrepare(); //need to re-prepare mesh
			return -1;
		}
	}

	if (add == true)
	{
		//add triangles
		for (size_t i = 0; i < indices.size(); i++)
		{
			indices[i] += Submeshes[index].MeshGeometry.size();
			Submeshes[index].Triangles.push_back(indices[i]);
		}

		//add vertices
		for (size_t i = 0; i < vertices.size(); i++)
			Submeshes[index].MeshGeometry.push_back(vertices[i]);
	}
	else
	{
		//remove triangles
		for (size_t i = 0; i < Submeshes[index].Triangles.size(); i++)
		{
			Triangle &triangle = Submeshes[index].Triangles[i];
			for (size_t j = 0; j < indices.size(); j++)
			{
				if (triangle == indices[j])
				{
					//delete match
					Submeshes[index].Triangles.erase(Submeshes[index].Triangles.begin() + i);
					i--;
					break;
				}
			}
		}

		//delete related vertices
		DeleteVertices(index, indices);
	}

	//bind material
	Submeshes[index].Name = material;
	mesh->ResetPrepare(); //need to re-prepare mesh

	//register texture usage
	if (createnew == true && material != "")
		sbs->GetTextureManager()->IncrementTextureUsage(Submeshes[index].Name);

	return index;
}

int PolyMesh::FindMatchingSubMesh(const std::string &material)
{
	//find a submesh with a matching material
	//returns array index

	for (size_t i = 0; i < Submeshes.size(); i++)
	{
		if (Submeshes[i].Name == material)
			return (int)i;
	}
	return -1;
}

void PolyMesh::DeleteVertices(int submesh, std::vector<Triangle> &deleted_indices)
{
	//delete related mesh vertices using provided index array
	//then reindex all mesh triangle indices in all submeshes.
	//this should be done after a polygon is deleted

	//exit if index array is empty
	if (deleted_indices.size() == 0)
		return;

	if (submesh < 0 || submesh > (int)Submeshes.size())
		return;

	//construct new sorted and compressed index array
	std::vector<int> deleted_v;
	deleted_v.reserve(deleted_indices.size() * 3);
	for (size_t i = 0; i < deleted_indices.size(); i++)
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
	unsigned int *deleted = new unsigned int[deleted_size];
	for (int i = 0; i < deleted_size; i++)
		deleted[i] = deleted_v[i];

	//delete specified vertices
	for (int i = deleted_size - 1; i >= 0; i--)
		Submeshes[submesh].MeshGeometry.erase(Submeshes[submesh].MeshGeometry.begin() + deleted[i]);

	//reindex triangle indices
	{
		std::vector<Triangle> &triangles = Submeshes[submesh].Triangles;

		int elements_size = (int)triangles.size() * 3;
		unsigned int *elements = new unsigned int[elements_size];
		bool *valid = new bool[elements_size];

		int elements_pos = 0;
		for (size_t i = 0; i < triangles.size(); i++)
		{
			//copy triangle data
			elements[elements_pos] = triangles[i].a;
			elements[elements_pos + 1] = triangles[i].b;
			elements[elements_pos + 2] = triangles[i].c;

			//set validity flags
			valid[elements_pos] = true;
			valid[elements_pos + 1] = true;
			valid[elements_pos + 2] = true;

			elements_pos += 3;

			for (int j = deleted_size - 1; j >= 0; j--)
			{
				if (elements[elements_pos - 3] >= deleted[j])
				{
					//invalidate value if going off the end of the array
					if (elements[elements_pos - 3] == 0)
						valid[elements_pos - 3] = false;
					else
						elements[elements_pos - 3]--; //adjust triangle index for deleted value
				}
				if (elements[elements_pos - 2] >= deleted[j])
				{
					//invalidate value if going off the end of the array
					if (elements[elements_pos - 2] == 0)
						valid[elements_pos - 2] = false;
					else
						elements[elements_pos - 2]--; //adjust triangle index for deleted value
				}
				if (elements[elements_pos - 1] >= deleted[j])
				{
					//invalidate value if going off the end of the array
					if (elements[elements_pos - 1] == 0)
						valid[elements_pos - 1] = false;
					else
						elements[elements_pos - 1]--; //adjust triangle index for deleted value
				}
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
			if (valid[element] == true && valid[element + 1] == true && valid[element + 2] == true)
				triangles.push_back(Triangle(elements[element], elements[element + 1], elements[element + 2]));
			element += 3;
		}
		delete [] elements;
		delete [] valid;
	}

	//reindex triangle indices in all wall objects
	for (size_t i = 0; i < mesh->Walls.size(); i++)
	{
		if (!mesh->Walls[i])
			continue;

		for (int j = 0; j < mesh->Walls[i]->GetPolygonCount(); j++)
		{
			//reindex triangle indices

			Polygon *poly = mesh->Walls[i]->GetPolygon(j);

			if (poly->material != Submeshes[submesh].Name)
				continue;

			size_t elements_size = poly->triangles.size() * 3;
			unsigned int *elements = new unsigned int[elements_size];
			bool *valid = new bool[elements_size];

			int elements_pos = 0;
			for (size_t k = 0; k < poly->triangles.size(); k++)
			{
				//copy triangle data
				elements[elements_pos] = poly->triangles[k].a;
				elements[elements_pos + 1] = poly->triangles[k].b;
				elements[elements_pos + 2] = poly->triangles[k].c;

				//set validity flags
				valid[elements_pos] = true;
				valid[elements_pos + 1] = true;
				valid[elements_pos + 2] = true;

				elements_pos += 3;

				for (int j = deleted_size - 1; j >= 0; j--)
				{
					if (elements[elements_pos - 3] >= deleted[j])
					{
						//invalidate value if going off the end of the array
						if (elements[elements_pos - 3] == 0)
							valid[elements_pos - 3] = false;
						else
							elements[elements_pos - 3]--; //adjust triangle index for deleted value
					}
					if (elements[elements_pos - 2] >= deleted[j])
					{
						//invalidate value if going off the end of the array
						if (elements[elements_pos - 2] == 0)
							valid[elements_pos - 2] = false;
						else
							elements[elements_pos - 2]--; //adjust triangle index for deleted value
					}
					if (elements[elements_pos - 1] >= deleted[j])
					{
						//invalidate value if going off the end of the array
						if (elements[elements_pos - 1] == 0)
							valid[elements_pos - 1] = false;
						else
							elements[elements_pos - 1]--; //adjust triangle index for deleted value
					}
				}
			}

			int element = 0;
			size_t size = poly->triangles.size();
			poly->triangles.clear();
			if ((int)poly->triangles.capacity() < size)
				poly->triangles.reserve(size);

			for (size_t ii = 0; ii < size; ii++)
			{
				//check if triangle indices are valid
				if (valid[element] == true && valid[element + 1] == true && valid[element + 2] == true)
					poly->triangles.push_back(Triangle(elements[element], elements[element + 1], elements[element + 2]));
				element += 3;
			}

			//reindex extents, used for getting original geometry
			for (int k = deleted_size - 1; k >= 0; k--)
			{
				size_t size = poly->index_extents.size();
				for (size_t m = 0; m < size; m++)
				{
					Extents extents = poly->index_extents[m];
					if (deleted[k] < extents.min)
						extents.min--;
					if (deleted[k] < extents.max)
						extents.max--;
					poly->index_extents[m] = extents;
				}
			}
			delete [] elements;
			delete [] valid;
		}
	}
	mesh->ResetPrepare(); //need to re-prepare mesh
	delete [] deleted;
}

void PolyMesh::CreateCollider()
{
	//set up triangle collider based on raw SBS mesh geometry

	SBS_PROFILE("MeshObject::CreateCollider");

	if (mesh->create_collider == false)
		return;

	//exit if collider already exists
	if (mesh->mBody)
		return;

	if (!mesh->GetSceneNode())
		return;

	//exit if mesh is empty
	if (Submeshes.size() == 0)
		return;

	unsigned int tricount = 0;
	unsigned int vcount = 0;
	for (size_t i = 0; i < Submeshes.size(); i++)
	{
		tricount += Submeshes[i].Triangles.size();
		vcount += Submeshes[i].MeshGeometry.size();
	}

	try
	{
		//initialize collider shape
		OgreBulletCollisions::TriangleMeshCollisionShape* shape = new OgreBulletCollisions::TriangleMeshCollisionShape(vcount, tricount * 3);

		Real scale = mesh->GetSceneNode()->GetScale();

		//add vertices to shape
		for (size_t i = 0; i < Submeshes.size(); i++)
		{
			for (size_t j = 0; j < Submeshes[i].Triangles.size(); j++)
			{
				const Triangle &tri = Submeshes[i].Triangles[j];

				Vector3 a = Submeshes[i].MeshGeometry[tri.a].vertex;
				Vector3 b = Submeshes[i].MeshGeometry[tri.b].vertex;
				Vector3 c = Submeshes[i].MeshGeometry[tri.c].vertex;

				if (scale != 1.0)
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
		if (!mesh->collider_node)
			mesh->collider_node = mesh->GetSceneNode()->CreateChild(GetName() + " collider");

		//physics is not supported on triangle meshes; use CreateBoxCollider instead
		mesh->mBody = new OgreBulletDynamics::RigidBody(mesh->name, sbs->mWorld);
		mesh->mBody->setStaticShape(mesh->collider_node->GetRawSceneNode(), shape, 0.1f, 0.5f, false);
		mesh->mShape = shape;

		if (sbs->DeleteColliders == true)
		{
			bool revert = false;
			if (mesh->remove_on_disable == false)
			{
				mesh->remove_on_disable = true;
				revert = true;
			}

			mesh->Enabled(false);
			mesh->Enabled(true);

			if (revert == true)
				mesh->remove_on_disable = false;
		}
	}
	catch (Ogre::Exception &e)
	{
		mesh->ReportError("Error creating collider for '" + mesh->name + "'\n" + e.getDescription());
	}
}

void PolyMesh::DeleteCollider()
{
	//delete mesh collider

	SBS_PROFILE("MeshObject::DeleteCollider");

	//exit if collider doesn't exist
	if (!mesh->mBody)
		return;

	//remove collider from world
	mesh->mBody->removeFromWorld();

	//delete collider object
	delete mesh->mBody;
	mesh->mBody = 0;
	mesh->mShape = 0;
}

Real PolyMesh::HitBeam(const Vector3 &origin, const Vector3 &direction, Real max_distance)
{
	//cast a ray and return the collision distance to the mesh
	//return -1 if no hit

	//cast a ray from the camera position downwards
	SBS_PROFILE("MeshObject::HitBeam");

	Vector3 position = sbs->ToRemote(origin - mesh->GetPosition());
	Ray ray (position, sbs->ToRemote(direction, false));

	for (size_t i = 0; i < Submeshes.size(); i++)
	{
		for (size_t j = 0; j < Submeshes[i].Triangles.size(); j++)
		{
			const Triangle &tri = Submeshes[i].Triangles[j];
			Vector3 &tri_a = Submeshes[i].MeshGeometry[tri.a].vertex;
			Vector3 &tri_b = Submeshes[i].MeshGeometry[tri.b].vertex;
			Vector3 &tri_c = Submeshes[i].MeshGeometry[tri.c].vertex;

			std::pair<bool, Real> result = Ogre::Math::intersects(ray, tri_a, tri_b, tri_c);
			if (result.first == true)
			{
				if (result.second <= sbs->ToRemote(max_distance))
					return sbs->ToLocal(result.second);
			}
		}
	}
	return -1;
}

bool PolyMesh::InBoundingBox(const Vector3 &pos, bool check_y)
{
	//determine if position 'pos' is inside the mesh's bounding box

	Vector3 position = sbs->ToRemote(pos - mesh->GetPosition());

	if (mesh->Bounds->isNull() == true)
		return false;

	Vector3 min = mesh->Bounds->getMinimum();
	Vector3 max = mesh->Bounds->getMaximum();

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

void PolyMesh::GetMeshInformation(const Ogre::Mesh* const mesh, int &vertex_count, Vector3* &vertices, int &index_count, unsigned long* &indices, Ogre::AxisAlignedBox &extents)
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
				vertices[current_offset + j] = pt * sbs->ToRemote(this->mesh->GetSceneNode()->GetScale());
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

Vector2 PolyMesh::GetExtents(int coord, bool flip_z)
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

	for (size_t i = 0; i < Submeshes.size(); i++)
	{
		for (size_t j = 0; j < Submeshes[i].MeshGeometry.size(); j++)
		{
			const Vector3 &vertex = Submeshes[i].MeshGeometry[j].vertex;

			if (coord == 1)
				tempnum = vertex.x;
			if (coord == 2)
				tempnum = vertex.y;
			if (coord == 3)
			{
				if (flip_z == false)
					tempnum = vertex.z;
				else
					tempnum = -vertex.z;
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

	return Vector2(esmall, ebig);
}

Wall* PolyMesh::FindPolygon(const std::string &name, int &index)
{
	//finds a polygon by name in all associated wall objects
	//returns associated wall object and polygon index

	for (size_t i = 0; i < mesh->Walls.size(); i++)
	{
		int polygon = mesh->Walls[i]->FindPolygon(name);
		if (polygon > -1)
		{
			index = polygon;
			return mesh->Walls[i];
		}
	}
	index = -1;
	return 0;
}

int PolyMesh::GetSubmeshCount()
{
	return (int)Submeshes.size();
}

unsigned int PolyMesh::GetVertexCount(int submesh)
{
	if (submesh < 0 || submesh >= (int)Submeshes.size())
	{
		//return total vertex count if a submesh hasn't been specified
		unsigned int total = 0;

		for (size_t i = 0; i < Submeshes.size(); i++)
			total += Submeshes[i].MeshGeometry.size();

		return total;
	}

	return Submeshes[submesh].MeshGeometry.size();
}

unsigned int PolyMesh::GetTriangleCount(int submesh)
{
	if (submesh < 0 || submesh >= (int)Submeshes.size())
	{
		//return total triangle count if a submesh hasn't been specified
		unsigned int total = 0;

		for (size_t i = 0; i < Submeshes.size(); i++)
			total += Submeshes[i].Triangles.size();

		return total;
	}

	return Submeshes[submesh].Triangles.size();
}

unsigned int PolyMesh::GetSize()
{
	//get general size of mesh object

	//get size of geometry tables
	unsigned int geo_size = sizeof(Geometry);
	unsigned int tri_size = sizeof(Triangle);
	unsigned long geometry = 0;
	unsigned long triangles = 0;

	for (int i = 0; i < Submeshes.size(); i++)
	{
		geometry += geo_size * Submeshes[i].MeshGeometry.size();
		triangles += tri_size * Submeshes[i].Triangles.size();
	}

	unsigned int total = geometry + triangles;
	return total;
}

}
