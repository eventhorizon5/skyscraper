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

#include <crystalspace.h>
#include "globals.h"
#include "sbs.h"
#include "unix.h"

//this file includes function implementations of the low-level SBS geometry and mesh code

extern SBS *sbs; //external pointer to the SBS engine

void SBS::DumpVertices(WallObject* wallobject)
{
	//dumps a list of vertices from a mesh object to the console/logfile

	Report("--- Polygon Vertex Dump ---\n");
	csRef<iGeneralFactoryState> state = scfQueryInterface<iGeneralFactoryState>(wallobject->meshwrapper->GetFactory()->GetMeshObjectFactory());
	for (int i = 0; i < state->GetVertexCount(); i++)
	{
		csVector3 vertex = state->GetVertices()[i];
		Report(csString(_itoa(i, intbuffer, 10)) + ": " + csString(_gcvt(vertex.x, 6, buffer)) + ", " + csString(_gcvt(vertex.y, 6, buffer)) + ", " + csString(_gcvt(vertex.z, 6, buffer)));
	}
}

csVector2 SBS::GetExtents(const csVector3 *varray, int count, int coord)
{
	//returns the smallest and largest values from a specified coordinate type
	//(x, y, or z) from a vertex array (polygon).
	//first parameter must be a vertex array object
	//second must be either 1 (for x), 2 (for y) or 3 (for z)

	float esmall = 0;
	float ebig = 0;
	float tempnum = 0;
	int i = 0;

	//return 0,0 if coord value is out of range
	if (coord < 1 || coord > 3)
		return csVector2(0, 0);

	for (i = 0; i < count; i++)
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

	return csVector2(esmall, ebig);
}

csVector2 SBS::GetExtents(csPoly3D &varray, int coord)
{
	return GetExtents(varray.GetVertices(), varray.GetVertexCount(), coord);
}

csVector2 SBS::GetExtents(csRef<iMeshWrapper> mesh, int coord)
{
	csRef<iGeneralFactoryState> state = scfQueryInterface<iGeneralFactoryState>(mesh->GetFactory()->GetMeshObjectFactory());
	return GetExtents(state->GetVertices(), state->GetVertexCount(), coord);
}

csVector3 SBS::GetPoint(csArray<WallObject*> &wallarray, const char *polyname, const csVector3 &start, const csVector3 &end)
{
	//do a line intersection with a specified mesh, and return
	//the intersection point

	int index = -1;
	int index2 = -1;
	for (int i = 0; i < wallarray.GetSize(); i++)
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
		csVector3 isect;
		float dist;
		csPoly3D original;
		csArray<CS::Geometry::csContour3> origpolys;
		wallarray[index]->GetGeometry(index2, origpolys, true);

		for (int i = 0; i < origpolys[0].GetSize(); i++)
			original.AddVertex(origpolys[0][i]);

		csIntersect3::SegmentPlane(ToRemote(start), ToRemote(end), original.ComputePlane(), isect, dist);
		return ToLocal(isect);
	}
	return 0;
}

iMeshWrapper* SBS::AddGenWall(csRef<iMeshWrapper> mesh, const char *texture, float x1, float z1, float x2, float z2, float height, float altitude, float tw, float th)
{
	//add a simple wall in a general mesh (currently only used for objects that change textures)

	//get texture
	csString texname = texture;
	bool result;
	csRef<iMaterialWrapper> material = GetTextureMaterial(texture, result, mesh->QueryObject()->GetName());
	if (!result)
		texname = "Default";

	if (tw == 0)
		tw = 1;
	if (th == 0)
		th = 1;

	float tw2 = tw, th2 = th;

	float mw, mh;
	if (GetTextureTiling(texname.GetData(), mw, mh))
	{
		//multiply the tiling parameters (tw and th) by
		//the stored multipliers for that texture
		tw2 = tw * mw;
		th2 = th * mh;
	}

	//create texture mapping table
	csVector2 table[] = {csVector2(tw2, th2), csVector2(0, th2), csVector2(tw2, 0), csVector2(0, 0)};

	//create a quad, map the texture, and append to the mesh
	CS::Geometry::TesselatedQuad wall (csVector3(ToRemote(x2), ToRemote(altitude), ToRemote(z2)), csVector3(ToRemote(x1), ToRemote(altitude), ToRemote(z1)), csVector3(ToRemote(x2), ToRemote(altitude + height), ToRemote(z2)));
	CS::Geometry::TableTextureMapper mapper(table);
	wall.SetMapper(&mapper);
	wall.Append(mesh->GetFactory());

	//set lighting factor
	mesh->GetMeshObject()->SetColor(csColor(1, 1, 1));

	//set texture
	mesh->GetMeshObject()->SetMaterialWrapper(material);

	//recreate colliders if specified
	if (RecreateColliders == true)
	{
		DeleteColliders(mesh);
		CreateColliders(mesh);
	}

	return mesh;
}


void SBS::Cut(WallObject *wall, csVector3 start, csVector3 end, bool cutwalls, bool cutfloors, csVector3 mesh_origin, csVector3 object_origin, int checkwallnumber, const char *checkstring, bool reset_check)
{
	//cuts a rectangular hole in the polygons within the specified range
	//mesh_origin is a modifier for meshes with relative polygon coordinates (used only for calculating door positions) - in this you specify the mesh's global position
	//object_origin is for the object's (such as a shaft) central position, used for calculating wall offsets

	if (cutwalls == false && cutfloors == false)
		return;

	csPoly3D temppoly, temppoly2, temppoly3, temppoly4, temppoly5, worker;
	csArray<WallPolygon*> ignore_list;

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
		for (int j = 0; j < ignore_list.GetSize(); j++)
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
		csString name = wall->GetHandle(i)->name;

		//get original vertices
		csArray<CS::Geometry::csContour3> origpolys;
		csArray<CS::Geometry::csContour3> newpolys;
		wall->GetGeometry(i, origpolys);

		//skip empty meshes
		if (origpolys.GetSize() == 0)
			continue;

		//cut all polygons within range
		for (int j = 0; j < origpolys.GetSize(); j++)
		{
			//skip null geometry
			if (origpolys[j].GetSize() == 0)
				continue;

			temppoly.MakeEmpty();
			temppoly2.MakeEmpty();
			temppoly3.MakeEmpty();
			temppoly4.MakeEmpty();
			temppoly5.MakeEmpty();
			worker.MakeEmpty();
			csVector2 extentsx, extentsy, extentsz;
			bool polycheck2 = false;

			//copy source polygon vertices
			for (int k = 0; k < origpolys[j].GetSize(); k++)
				temppoly.AddVertex(origpolys[j][k]);

			//make sure the polygon is not outside the cut area
			if (temppoly.ClassifyX(start.x) != CS_POL_FRONT &&
					temppoly.ClassifyX(end.x) != CS_POL_BACK &&
					temppoly.ClassifyY(start.y) != CS_POL_FRONT &&
					temppoly.ClassifyY(end.y) != CS_POL_BACK &&
					temppoly.ClassifyZ(start.z) != CS_POL_FRONT &&
					temppoly.ClassifyZ(end.z) != CS_POL_BACK)
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
							worker.SplitWithPlaneX(temppoly, temppoly2, start.x);
							worker.MakeEmpty();

							//get right side
							if (temppoly2.GetVertexCount() > 0)
								worker = temppoly2;
							else
								worker = temppoly;
							worker.SplitWithPlaneX(temppoly3, temppoly2, end.x);
							worker.MakeEmpty();

							//get lower
							if (temppoly3.GetVertexCount() > 0)
								worker = temppoly3;
							else if (temppoly2.GetVertexCount() > 0)
								worker = temppoly2;
							else if (temppoly.GetVertexCount() > 0)
								worker = temppoly;
							worker.SplitWithPlaneY(temppoly3, temppoly4, start.y);
							worker.MakeEmpty();

							//get upper
							if (temppoly4.GetVertexCount() > 0)
								worker = temppoly4;
							else if (temppoly3.GetVertexCount() > 0)
								worker = temppoly3;
							else if (temppoly2.GetVertexCount() > 0)
								worker = temppoly2;
							else if (temppoly.GetVertexCount() > 0)
								worker = temppoly;
							worker.SplitWithPlaneY(temppoly5, temppoly4, end.y);
							worker.MakeEmpty();
						}
						else
						{
							//wall is facing left/right

							//get left side
							worker = temppoly;
							worker.SplitWithPlaneZ(temppoly, temppoly2, start.z);
							worker.MakeEmpty();

							//get right side
							if (temppoly2.GetVertexCount() > 0)
								worker = temppoly2;
							else
								worker = temppoly;
							worker.SplitWithPlaneZ(temppoly3, temppoly2, end.z);
							worker.MakeEmpty();

							//get lower
							if (temppoly3.GetVertexCount() > 0)
								worker = temppoly3;
							else if (temppoly2.GetVertexCount() > 0)
								worker = temppoly2;
							else if (temppoly.GetVertexCount() > 0)
								worker = temppoly;
							worker.SplitWithPlaneY(temppoly3, temppoly4, start.y);
							worker.MakeEmpty();

							//get upper
							if (temppoly4.GetVertexCount() > 0)
								worker = temppoly4;
							else if (temppoly3.GetVertexCount() > 0)
								worker = temppoly3;
							else if (temppoly2.GetVertexCount() > 0)
								worker = temppoly2;
							else if (temppoly.GetVertexCount() > 0)
								worker = temppoly;
							worker.SplitWithPlaneY(temppoly5, temppoly4, end.y);
							worker.MakeEmpty();
						}
						polycheck = true;
						polycheck2 = true;
						//store extents of temppoly5 for door sides if needed
						if (checkwallnumber > 0 && checkwallnumber < 3)
						{
							if (name.Find(checkstring) >= 0)
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
					worker.SplitWithPlaneX(temppoly, temppoly2, start.x);
					worker.MakeEmpty();

					//get right side
					if (temppoly2.GetVertexCount() > 0)
						worker = temppoly2;
					else
						worker = temppoly;
					worker.SplitWithPlaneX(temppoly3, temppoly2, end.x);
					worker.MakeEmpty();

					//get lower
					if (temppoly3.GetVertexCount() > 0)
						worker = temppoly3;
					else if (temppoly2.GetVertexCount() > 0)
						worker = temppoly2;
					else if (temppoly.GetVertexCount() > 0)
						worker = temppoly;
					worker.SplitWithPlaneZ(temppoly3, temppoly4, start.z);
					worker.MakeEmpty();

					//get upper
					if (temppoly4.GetVertexCount() > 0)
						worker = temppoly4;
					else if (temppoly3.GetVertexCount() > 0)
						worker = temppoly3;
					else if (temppoly2.GetVertexCount() > 0)
						worker = temppoly2;
					else if (temppoly.GetVertexCount() > 0)
						worker = temppoly;
					worker.SplitWithPlaneZ(temppoly5, temppoly4, end.z);
					worker.MakeEmpty();
					temppoly5.MakeEmpty();

					polycheck = true;
					polycheck2 = true;
				}

				//create split polygons
				if (polycheck2 == true)
				{
					if (temppoly.GetVertexCount() > 2)
					{
						newpolys.SetSize(newpolys.GetSize() + 1);
						for (int k = 0; k < temppoly.GetVertexCount(); k++)
							newpolys[newpolys.GetSize() - 1].Push(temppoly[k]);
					}
					if (temppoly2.GetVertexCount() > 2)
					{
						newpolys.SetSize(newpolys.GetSize() + 1);
						for (int k = 0; k < temppoly2.GetVertexCount(); k++)
							newpolys[newpolys.GetSize() - 1].Push(temppoly2[k]);
					}
					if (temppoly3.GetVertexCount() > 2)
					{
						newpolys.SetSize(newpolys.GetSize() + 1);
						for (int k = 0; k < temppoly3.GetVertexCount(); k++)
							newpolys[newpolys.GetSize() - 1].Push(temppoly3[k]);
					}
					if (temppoly4.GetVertexCount() > 2)
					{
						newpolys.SetSize(newpolys.GetSize() + 1);
						for (int k = 0; k < temppoly4.GetVertexCount(); k++)
							newpolys[newpolys.GetSize() - 1].Push(temppoly4[k]);
					}

					temppoly.MakeEmpty();
					temppoly2.MakeEmpty();
					temppoly3.MakeEmpty();
					temppoly4.MakeEmpty();
				}
			}
			else
			{
				//otherwise put original polygon into array (will only be used if the related submesh is recreated)
				newpolys.Push(origpolys[j]);
			}
		}

		//create new polygon
		if (polycheck == true && newpolys.GetSize() > 0)
		{
			//get texture data from original polygon
			iMaterialWrapper *oldmat = wall->GetHandle(i)->material;
			csVector3 oldvector;
			csMatrix3 mapping;
			wall->GetHandle(i)->GetTextureMapping(mapping, oldvector);

			//delete original polygon
			wall->DeletePolygon(i, false);

			//create new polygon
			WallPolygon* handle = 0;
			handle = wall->AddPolygon(name, oldmat, newpolys, mapping, oldvector);
			ignore_list.Push(handle);

			//reset search position
			i--;
			polycheck = false;
		}
	}

	//recreate colliders if specified
	if (RecreateColliders == true)
	{
		DeleteColliders(wall->meshwrapper);
		CreateColliders(wall->meshwrapper);
	}
}

csVector3 SBS::GetWallExtents(csArray<WallObject*> &wallarray, const char *name, float altitude, bool get_max)
{
	//return the X and Z extents of a standard wall (by name) at a specific altitude, by doing a double plane cut

	csString newname;
	csString name2 = name;
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
		for (int j = 0; j < wallarray.GetSize(); j++)
		{
			index2 = wallarray[j]->FindPolygon(newname);
			if (index2 > -1)
			{
				index = j;
				break;
			}
		}

		if (index >= 0)
		{
			csArray<CS::Geometry::csContour3> origpolys;
			wallarray[index]->GetGeometry(index2, origpolys, true);

			csPoly3D original, tmp1, tmp2;
			for (int i = 0; i < origpolys[0].GetSize(); i++)
				original.AddVertex(origpolys[0][i]);

			//if given altitude is outside of polygon's range, return 0
			csVector2 yextents = GetExtents(original, 2);
			float tmpaltitude = altitude;
			if (tmpaltitude < yextents.x || tmpaltitude > yextents.y)
				return 0;

			//get upper
			original.SplitWithPlaneY(tmp1, tmp2, tmpaltitude - 0.001);

			//get lower part of upper
			tmp2.SplitWithPlaneY(original, tmp1, tmpaltitude + 0.001);

			csVector3 result;
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
	return 0;
}

csVector3 SBS::GetPolygonDirection(csPoly3D &polygon)
{
	//returns the direction the polygon faces, in a 3D vector
	//for example, <-1, 0, 0> means that the wall faces left.

	//get largest normal

	csVector3 normal = polygon.ComputeNormal();

	int largest_normal = 0; //x

	if (fabsf(normal.y) > fabsf(normal.x) && fabsf(normal.y) > fabsf(normal.z))
		largest_normal = 1; //y biggest
	else if (fabsf(normal.z) > fabsf(normal.x))
		largest_normal = 2; //z biggest

	if (largest_normal == 0)
	{
		if (normal.x < 0)
			return csVector3(1, 0, 0);
		else
			return csVector3(-1, 0, 0);
	}

	if (largest_normal == 1)
	{
		if (normal.y < 0)
			return csVector3(0, 1, 0);
		else
			return csVector3(0, -1, 0);
	}

	if (largest_normal == 2)
	{
		if (normal.z < 0)
			return csVector3(0, 0, 1);
		else
			return csVector3(0, 0, -1);
	}
	return csVector3(0, 0, 0);
}

csRef<iRenderBuffer> SBS::PolyMesh(csRef<iMeshWrapper> mesh, csRefArray<iGeneralMeshSubMesh> &submeshes, const char *name, const char *texture, CS::Geometry::csContour3 &vertices, float tw, float th, bool autosize, csMatrix3 &t_matrix, csVector3 &t_vector, csArray<csVector2> &mesh_indices)
{
	//create custom genmesh geometry, apply a texture map and material, and return the created submesh

	//get texture material
	csString texname = texture;
	bool result;
	csRef<iMaterialWrapper> material = GetTextureMaterial(texture, result, mesh->QueryObject()->GetName());
	if (!result)
		texname = "Default";

	if (tw == 0)
		tw = 1;
	if (th == 0)
		th = 1;

	//get autosize information
	csVector2 xextents = GetExtents(vertices.GetArray(), vertices.GetSize(), 1);
	csVector2 yextents = GetExtents(vertices.GetArray(), vertices.GetSize(), 2);
	csVector2 zextents = GetExtents(vertices.GetArray(), vertices.GetSize(), 3);

	csVector2 sizing;
	sizing.x = tw;
	sizing.y = th;

	if (autosize == true)
		sizing = sbs->CalculateSizing(texture, xextents, yextents, zextents, tw, th);

	//get texture tiling information
	float tw2 = sizing.x, th2 = sizing.y;
	float mw, mh;
	if (GetTextureTiling(texname.GetData(), mw, mh))
	{
		//multiply the tiling parameters (tw and th) by
		//the stored multipliers for that texture
		tw2 = sizing.x * mw;
		th2 = sizing.y * mh;
	}

	//create texture mapping table
	//convert to remote positioning

	csArray<CS::Geometry::csContour3> vertices2;
	vertices2.SetSize(1);

	for (int i = 0; i < vertices.GetSize(); i++)
		vertices2[0].Push(ToRemote(vertices[i]));

	//texture mapping
	csVector3 v1, v2, v3;
	GetTextureMapping(vertices2[0], v1, v2, v3);
	if (!ComputeTextureMap(t_matrix, t_vector, vertices2[0],
		v1,
		csVector2 (MapUV[0].x * tw2, MapUV[0].y * th2),
		v2,
		csVector2 (MapUV[1].x * tw2, MapUV[1].y * th2),
		v3,
		csVector2 (MapUV[2].x * tw2, MapUV[2].y * th2)))
		return 0;

	return PolyMesh(mesh, submeshes, name, material, vertices2, t_matrix, t_vector, mesh_indices, false);
}

csRef<iRenderBuffer> SBS::PolyMesh(csRef<iMeshWrapper> mesh, csRefArray<iGeneralMeshSubMesh> &submeshes, const char *name, csRef<iMaterialWrapper> material, csArray<CS::Geometry::csContour3> &vertices, csMatrix3 &tex_matrix, csVector3 &tex_vector, csArray<csVector2> &mesh_indices, bool convert_vertices)
{
	//create custom genmesh geometry, apply a texture map and material, and return the created submesh

	//get genmesh factory state
	csRef<iGeneralFactoryState> state = scfQueryInterface<iGeneralFactoryState>(mesh->GetFactory()->GetMeshObjectFactory());

	//convert to remote positioning

	csArray<CS::Geometry::csContour3> vertices2;
	if (convert_vertices == true)
	{
		vertices2.SetSize(vertices.GetSize());
		for (int i = 0; i < vertices.GetSize(); i++)
		{
			for (int j = 0; j < vertices[i].GetSize(); j++)
				vertices2[i].Push(ToRemote(vertices[i][j]));
		}
	}
	else
		vertices2 = vertices;

	//texture mapping
	csVector2 *table = GetTexels(tex_matrix, tex_vector, vertices2);
	CS::Geometry::TableTextureMapper mapper(table);

	//triangulate mesh
	csArray<csTriangleMesh> trimesh;
	trimesh.SetSize(vertices2.GetSize());

	for (int i = 0; i < trimesh.GetSize(); i++)
	{
		//first fill triangle mesh with polygon's vertices
		for (int j = 0; j < vertices2[i].GetSize(); j++)
			trimesh[i].AddVertex(vertices2[i][j]);

		//then do a (very) simple triangulation
		//this method is used because it works with non-planar polygons, and the main
		//Crystal Space triangulation system requires planar polygons
		for (int j = 2; j < vertices2[i].GetSize(); j++)
			trimesh[i].AddTriangle(0, j - 1, j);
	}

	//set up geometry arrays
	csDirtyAccessArray<csVector3> mesh_vertices;
	csDirtyAccessArray<csVector2> mesh_texels;
	csDirtyAccessArray<csVector3> mesh_normals;

	//initialize geometry arrays
	int size = 0;
	for (int i = 0; i < trimesh.GetSize(); i++)
		size += trimesh[i].GetVertexCount();
	mesh_vertices.SetSize(size);
	mesh_texels.SetSize(size);
	mesh_normals.SetSize(size);

	//get number of existing vertices
	int count = state->GetVertexCount();

	//populate vertices, normals, and texels for mesh data
	int k = 0;

	for (int i = 0; i < trimesh.GetSize(); i++)
	{
		int min = count + k;

		for (int j = 0; j < trimesh[i].GetVertexCount(); j++)
		{
			mesh_normals[k] = mesh_vertices[k] = trimesh[i].GetVertices()[j];
			mesh_normals[k].Normalize();
			mesh_texels[k] = mapper.Map(mesh_vertices[k], mesh_normals[k], k);

			state->AddVertex(mesh_vertices[k], mesh_texels[k], mesh_normals[k], csColor4(1, 1, 1));
			//state->AddVertex(mesh_vertices[k], mesh_texels[k], mesh_normals[k], csColor4(0, 0, 0)); //for lighted

			int a = k - 1;
			if (a == -1)
			a = size - 1;
			int c = k + 1;
			if (c == size)
			c = 0;

			k++;
		}

		int max = count + k - 1;
		mesh_indices.Push(csVector2(min, max));
	}

	//delete texel array
	if (table)
		delete table;
	table = 0;

	//delete arrays
	mesh_vertices.DeleteAll();
	mesh_texels.DeleteAll();
	mesh_normals.DeleteAll();

	//set up triangle buffer
	int tricount = 0;
	for (int i = 0; i < trimesh.GetSize(); i++)
		tricount += trimesh[i].GetTriangleCount();

	csRef<iRenderBuffer> buffer = csRenderBuffer::CreateIndexRenderBuffer(tricount * 3, CS_BUF_STATIC, CS_BUFCOMP_UNSIGNED_INT, count, count + size - 1);
	csTriangle *triangleData = (csTriangle*)buffer->Lock(CS_BUF_LOCK_NORMAL);

	//add triangles to mesh
	int location = 0;
	int location2 = 0;
	for (int i = 0; i < trimesh.GetSize(); i++)
	{
		for (int j = 0; j < trimesh[i].GetTriangleCount(); j++)
		{
			csTriangle tri = trimesh[i].GetTriangle(j);
			tri.a += count + location2;
			tri.b += count + location2;
			tri.c += count + location2;
			triangleData[location] = tri; //add triangle to submesh buffer
			location++;
		}
		location2 += trimesh[i].GetVertexCount();
	}

	//finish with submesh buffer
	buffer->Release();

	//reprocess mesh
	state->Invalidate();

	//create submesh and set material
	ReindexSubMesh(state, submeshes, buffer, material, name, true);

	//recreate colliders if specified
	if (RecreateColliders == true)
	{
		DeleteColliders(mesh);
		CreateColliders(mesh);
	}

	return buffer;
}

bool SBS::ComputeTextureMap(csMatrix3 &t_matrix, csVector3 &t_vector, CS::Geometry::csContour3 &vertices, const csVector3 &p1, const csVector2 &uv1, const csVector3 &p2, const csVector2 &uv2, const csVector3 &p3, const csVector2 &uv3)
{
	//this is modified code from the Crystal Space thingmesh system, from the "plugins/mesh/thing/object/polygon.cpp" file.
	//given an array of vertices, this returns the texture transformation matrix and vector

	//original description:
	// Some explanation. We have three points for
	// which we know the uv coordinates. This gives:
	//     P1 -> UV1
	//     P2 -> UV2
	//     P3 -> UV3
	// P1, P2, and P3 are on the same plane so we can write:
	//     P = P1 + lambda * (P2-P1) + mu * (P3-P1)
	// For the same lambda and mu we can write:
	//     UV = UV1 + lambda * (UV2-UV1) + mu * (UV3-UV1)
	// What we want is Po, Pu, and Pv (also on the same
	// plane) so that the following uv coordinates apply:
	//     Po -> 0,0
	//     Pu -> 1,0
	//     Pv -> 0,1
	// The UV equation can be written as follows:
	//     U = U1 + lambda * (U2-U1) + mu * (U3-U1)
	//     V = V1 + lambda * (V2-V1) + mu * (V3-V1)
	// This is a matrix equation (2x2 matrix):
	//     UV = UV1 + M * PL
	// We have UV in this case and we need PL so we
	// need to invert this equation:
	//     (1/M) * (UV - UV1) = PL

	csMatrix2 m (uv2.x - uv1.x, uv3.x - uv1.x, uv2.y - uv1.y, uv3.y - uv1.y);
	float det = m.Determinant();

	if (ABS(det) < 0.0001f)
	{
		ReportError("Warning: bad UV coordinates");

		/*if (!((p1-p2) < SMALL_EPSILON))
			SetTextureSpace(p1, p2, 1);
		else if (!((p1-p3) < SMALL_EPSILON))
			SetTextureSpace(p1, p3, 1);*/
		return false;
	}
	else
		m.Invert();

	csVector2 pl;
	csVector3 po, pu, pv;

	// For (0,0) and Po
	pl = m * (csVector2(0, 0) - uv1);
	po = p1 + pl.x * (p2 - p1) + pl.y * (p3 - p1);

	// For (1,0) and Pu
	pl = m * (csVector2(1, 0) - uv1);
	pu = p1 + pl.x * (p2 - p1) + pl.y * (p3 - p1);

	// For (0,1) and Pv
	pl = m * (csVector2(0, 1) - uv1);
	pv = p1 + pl.x * (p2 - p1) + pl.y * (p3 - p1);

	ComputeTextureSpace(t_matrix, t_vector, po, pu, (pu - po).Norm(), pv, (pv - po).Norm());
	return true;
}

bool SBS::ComputeTextureSpace(csMatrix3 &m, csVector3 &v, const csVector3 &v_orig, const csVector3 &v1, float len1, const csVector3 &v2, float len2)
{
	//from CS textrans.cpp
	
	float d = csSquaredDist::PointPoint(v_orig, v1);
	float invl1 = csQisqrt(d);

	d = csSquaredDist::PointPoint(v_orig, v2);
	float invl2 = (d) ? csQisqrt (d) : 0;

	csVector3 v_u = (v1 - v_orig) * len1 * invl1;
	csVector3 v_v = (v2 - v_orig) * len2 * invl2;
	csVector3 v_w = v_u % v_v;

	m.m11 = v_u.x;
	m.m12 = v_v.x;
	m.m13 = v_w.x;
	m.m21 = v_u.y;
	m.m22 = v_v.y;
	m.m23 = v_w.y;
	m.m31 = v_u.z;
	m.m32 = v_v.z;
	m.m33 = v_w.z;

	v = v_orig;

	float det = m.Determinant ();
	if (ABS (det) < SMALL_EPSILON)
	{
		m.Identity();
		return false;
	}
	else
		m.Invert ();

	return true;
}

csVector2* SBS::GetTexels(csMatrix3 &tex_matrix, csVector3 &tex_vector, csArray<CS::Geometry::csContour3> &vertices)
{
	//return texel array for specified texture transformation matrix and vector

	csTransform transform(tex_matrix, tex_vector);

	//create array for texel map
	int texel_count = 0;
	for (int i = 0; i < vertices.GetSize(); i++)
		texel_count += vertices[i].GetSize();
	csVector2 *texels = new csVector2[texel_count];

	//transform matrix into texel map
	int index = 0;
	csVector3 texel_temp;
	for (int i = 0; i < vertices.GetSize(); i++)
	{
		for (int j = 0; j < vertices[i].GetSize(); j++)
		{
			texel_temp = transform.Other2This(vertices[i][j]);
			texels[index].x = texel_temp.x;
			texels[index].y = texel_temp.y;
			index++;
		}
	}
	return texels;
}

int SBS::ReindexSubMesh(iGeneralFactoryState* state, csRefArray<iGeneralMeshSubMesh> &submeshes, csRef<iRenderBuffer> indices, iMaterialWrapper* material, const char *name, bool add)
{
	//adds or removes triangle indices to a submesh in the list with a matching material
	//if append is true, adds indices; otherwise removes them

	//first get related submesh
	int index = FindMatchingSubMesh(submeshes, material);

	//create submesh if it doesn't exist, and exit
	if (index == -1)
	{
		csRef<iGeneralMeshSubMesh> newsubmesh = state->AddSubMesh(indices, material, name);
		return submeshes.Push(newsubmesh);
	}

	csRef<iGeneralMeshSubMesh> submesh = submeshes[index];

	//set up buffer to original triangle indices
	iRenderBuffer *buffer = submesh->GetIndices();

	//get triangle counts
	int buffercount = buffer->GetElementCount() / 3;
	int indicescount = indices->GetElementCount() / 3;

	//get new triangle count
	int tricount;
	if (add == true)
		tricount = buffercount + indicescount;
	else
		tricount = buffercount - indicescount;

	//delete submesh and exit if it's going to be emptied
	if (tricount <= 0)
	{
		buffer = 0;
		state->DeleteSubMesh(submesh);
		submeshes.DeleteIndex(index);
		return -1;
	}

	//get index range limits
	int rangestart = 0, rangeend = 0;
	if (buffer->GetRangeStart() < indices->GetRangeStart())
		rangestart = buffer->GetRangeStart();
	else
		rangestart = indices->GetRangeStart();

	if (buffer->GetRangeEnd() > indices->GetRangeEnd())
		rangeend = buffer->GetRangeEnd();
	else
		rangeend = indices->GetRangeEnd();

	//set up new buffer for modified indices
	csRef<iRenderBuffer> newbuffer = csRenderBuffer::CreateIndexRenderBuffer(tricount * 3, CS_BUF_STATIC, CS_BUFCOMP_UNSIGNED_INT, rangestart, rangeend);

	if (add == true)
	{
		//copy old triangle indices into new buffer
		int *triangleData = (int*)buffer->Lock(CS_BUF_LOCK_NORMAL);
		newbuffer->CopyInto(triangleData, buffer->GetElementCount());
		buffer->Release();
	}
	
	//set up buffer for old data
	csTriangle *triangleData = (csTriangle*)buffer->Lock(CS_BUF_LOCK_NORMAL);

	//set up triangle buffer for new data
	csTriangle *newtriangleData = (csTriangle*)newbuffer->Lock(CS_BUF_LOCK_NORMAL);

	//set up buffer with triangle indices to add/remove
	csTriangle *triangleData2 = (csTriangle*)indices->Lock(CS_BUF_LOCK_NORMAL);

	if (add == true)
	{
		//add triangles

		//append new triangle indices into new buffer
		for (int i = 0; i < indicescount; i++)
			newtriangleData[i + buffercount] = triangleData2[i];
	}
	else
	{
		//remove triangles

		int newindex = 0;
		bool skip = false;
		for (int i = 0; i < buffercount; i++)
		{
			for (int j = 0; j < indicescount; j++)
			{
				if (triangleData[i].a == triangleData2[j].a && triangleData[i].b == triangleData2[j].b && triangleData[i].c == triangleData2[j].c)
				{
					//skip matching data
					skip = true;
					break;
				}
			}
			if (skip == false)
			{
				if (newindex > tricount - 1)
					break; //removal match not found - this shouldn't happen
				newtriangleData[newindex] = triangleData[i]; //fill new buffer with non-matching triangles
				newindex++;
			}
			skip = false;
		}
	}

	//release buffers
	buffer->Release();
	indices->Release();
	newbuffer->Release();

	//delete old submesh
	state->DeleteSubMesh(submesh);
	submeshes.DeleteIndex(index);

	//create submesh
	csRef<iGeneralMeshSubMesh> newsubmesh = state->AddSubMesh(newbuffer, material, name);
	index = submeshes.Push(newsubmesh);
	return index;
}

int SBS::FindMatchingSubMesh(csRefArray<iGeneralMeshSubMesh> &submeshes, iMaterialWrapper *material)
{
	//find a submesh with a matching material
	//returns array index

	for (int i = 0; i < submeshes.GetSize(); i++)
	{
		if (submeshes[i]->GetMaterial() == material)
			return i;
	}
	return -1;
}

void WallPolygon::GetTextureMapping(csMatrix3 &tm, csVector3 &tv)
{
	//return texture mapping matrix and vector
	tm = t_matrix;
	tv = t_vector;
}

iGeneralMeshSubMesh* WallPolygon::GetSubMesh()
{
	//return the submesh this polygon is in
	int index = sbs->FindMatchingSubMesh(*submeshes, material);
	return submeshes->Get(index);
}

void SBS::DeleteVertices(csArray<WallObject*> &wallarray, iRenderBuffer *deleted_indices)
{
	//delete related mesh vertices using provided index array
	//then reindex all mesh triangle indices in all submeshes.
	//this should be done after a polygon is deleted
	//also, all wall objects in wall arrays must have the same mesh state

	//exit if wall array is empty
	if (wallarray.GetSize() == 0)
		return;

	//get integer array of triangle indices
	csArray<int> indices;
	int* buffer2 = (int*)deleted_indices->Lock(CS_BUF_LOCK_NORMAL);
	for (int i = 0; i < deleted_indices->GetElementCount(); i++)
		indices.Push(buffer2[i]);
	deleted_indices->Release();

	iGeneralFactoryState *state = wallarray[0]->state;

	//set up new geometry arrays
	csDirtyAccessArray<csVector3> mesh_vertices;
	csDirtyAccessArray<csVector2> mesh_texels;
	csDirtyAccessArray<csVector3> mesh_normals;
	csDirtyAccessArray<csColor4> mesh_colors;

	//copy mesh data
	for (int i = 0; i < state->GetVertexCount(); i++)
	{
		mesh_vertices.Push(state->GetVertices()[i]);
		mesh_texels.Push(state->GetTexels()[i]);
		mesh_normals.Push(state->GetNormals()[i]);
		mesh_colors.Push(state->GetColors()[i]);
	}

	//construct new sorted and compressed index array
	csArray<int> deleted2;
	for (int i = 0; i < indices.GetSize(); i++)
		deleted2.PushSmart(indices[i]);
	deleted2.Sort();

	indices.DeleteAll();

	//delete specified vertices
	for (int i = deleted2.GetSize() - 1; i >= 0; i--)
	{
		int index = deleted2[i];
		mesh_vertices.DeleteIndex(index);
		mesh_texels.DeleteIndex(index);
		mesh_normals.DeleteIndex(index);
		mesh_colors.DeleteIndex(index);
	}

	//refill original mesh data
	state->SetVertexCount(mesh_vertices.GetSize());
	for (int i = 0; i < mesh_vertices.GetSize(); i++)
	{
		state->GetVertices()[i] = mesh_vertices[i];
		state->GetTexels()[i] = mesh_texels[i];
		state->GetNormals()[i] = mesh_normals[i];
		state->GetColors()[i] = mesh_colors[i];
	}

	mesh_vertices.DeleteAll();
	mesh_texels.DeleteAll();
	mesh_normals.DeleteAll();
	mesh_colors.DeleteAll();

	//reindex triangle indices in all submeshes
	for (int i = 0; i < state->GetSubMeshCount(); i++)
	{
		iRenderBuffer *indices = state->GetSubMesh(i)->GetIndices();

		if (!indices)
			continue;

		int* indices2 = (int*)indices->Lock(CS_BUF_LOCK_NORMAL);

		for (int j = 0; j < indices->GetElementCount(); j++)
		{
			for (int k = deleted2.GetSize() - 1; k >= 0; k--)
			{
				if (indices2[j] >= deleted2[k])
					indices2[j]--;
			}
		}
		indices->Release();
	}

	//reindex triangle indices in all wall objects
	for (int i = 0; i < wallarray.GetSize(); i++)
	{
		if (!wallarray[i])
			continue;

		for (int j = 0; j < wallarray[i]->handles.GetSize(); j++)
		{
			csRef<iRenderBuffer> indices = wallarray[i]->handles[j].triangles;

			if (!indices)
				continue;

			int* indices2 = (int*)indices->Lock(CS_BUF_LOCK_NORMAL);

			//reindex triangle indices
			for (int k = 0; k < indices->GetElementCount(); k++)
			{
				for (int m = deleted2.GetSize() - 1; m >= 0; m--)
				{
					if (indices2[k] >= deleted2[m])
						indices2[k]--;
				}
			}
			indices->Release();

			//reindex extents, used for getting original geometry
			for (int k = deleted2.GetSize() - 1; k >= 0; k--)
			{
				for (int m = 0; m < wallarray[i]->handles[j].index_extents.GetSize(); m++)
				{
					csVector2 extents = wallarray[i]->handles[j].index_extents[m];
					if (deleted2[k] < extents.x)
						extents.x--;
					if (deleted2[k] < extents.y)
						extents.y--;
					wallarray[i]->handles[j].index_extents[m] = extents;
				}
			}
		}
	}
}

WallPolygon::WallPolygon()
{
	sbs->PolygonCount++;
}

WallPolygon::~WallPolygon()
{
	sbs->PolygonCount--;
}

void WallPolygon::GetGeometry(csRef<iMeshWrapper> meshwrapper, csArray<CS::Geometry::csContour3> &vertices, bool firstonly)
{
	//gets vertex geometry using mesh's vertex extent arrays; returns vertices in 'vertices'

	vertices.SetSize(index_extents.GetSize());
	csRef<iGeneralFactoryState> state = scfQueryInterface<iGeneralFactoryState>(meshwrapper->GetFactory()->GetMeshObjectFactory());

	for (int i = 0; i < index_extents.GetSize(); i++)
	{
		int min = index_extents[i].x;
		int max = index_extents[i].y;
		for (int j = min; j <= max; j++)
			vertices[i].Push(sbs->ToLocal(state->GetVertices()[j]));
		if (firstonly == true)
			return;
	}
}

bool WallPolygon::PointInside(csRef<iMeshWrapper> meshwrapper, csVector3 point, bool plane_check)
{
	//check if point is on the polygon's plane
	if (plane_check == true)
	{
		float dot = plane.D() + plane.A() * point.x + plane.B() * point.y + plane.C() * point.z;
		if (ABS(dot) >= EPSILON) return false;
	}

	//get original geometry
	csArray<CS::Geometry::csContour3> vertices;
	GetGeometry(meshwrapper, vertices, false);

	//check if the point is on the same side of all polygon edges
	/*int i, i1;
	bool neg = false, pos = false;
	i1 = polygon_data.num_vertices - 1;
	for (i = 0; i < polygon_data.num_vertices; i++)
	{
	float ar = csMath3::Direction3 (v, Vobj (i1), Vobj (i));
	if (ar < 0)
	  neg = true;
	else if (ar > 0)
	  pos = true;
	if (neg && pos) return false;
	i1 = i;
	}*/

	return true;
}

MeshObject::MeshObject(Object* parent, const char *name, float max_render_distance)
{
	//set up SBS object
	object = new Object();
	object->SetValues(this, parent, "Mesh", name, false);

	enabled = true;
	//create a new mesh wrapper and factory
	csString factname = name;
	factname.Append(" factory");

	csString buffer;
	csString Name = name;
	buffer = object->GetNumber();
	Name.Insert(0, "(" + buffer + ")");

	//create mesh wrapper and factory
	MeshWrapper = CS::Geometry::GeneralMeshBuilder::CreateFactoryAndMesh(sbs->engine, sbs->area, Name, factname);

	//set zbuf mode to "USE" by default
	MeshWrapper->SetZBufMode(CS_ZBUF_USE);

	//set render priority to "object" by default
	MeshWrapper->SetRenderPriority(sbs->engine->GetObjectRenderPriority());

	//create a default material (otherwise the system complains if a mesh is used without a material)
	MeshWrapper->GetMeshObject()->SetMaterialWrapper(sbs->engine->GetMaterialList()->FindByName("Default"));

	//set maximum render distance
	if (max_render_distance > 0)
		MeshWrapper->SetMaximumRenderDistance(sbs->ToRemote(max_render_distance));

	State = scfQueryInterface<iGeneralFactoryState>(MeshWrapper->GetFactory()->GetMeshObjectFactory());
	Movable = MeshWrapper->GetMovable();

	/*state->SetLighting(false);
	state->SetShadowCasting(false);
	state->SetShadowReceiving(false);
	state->SetManualColors(false);*/
}

MeshObject::~MeshObject()
{
	//delete wall objects
	for (int i = 0; i < Walls.GetSize(); i++)
	{
		if (Walls[i])
		{
			Walls[i]->parent_deleting = true;
			delete Walls[i];
		}
		Walls[i] = 0;
	}

	if (sbs->FastDelete == false)
		sbs->engine->WantToDie(MeshWrapper);

	MeshWrapper = 0;
	delete object;
}

void MeshObject::Enable(bool value)
{
	//enables or disables the mesh

	if (value == enabled)
		return;

	if (value == true)
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
	}
	enabled = value;
}

WallObject* MeshObject::CreateWallObject(Object *parent, const char *name)
{
	//create a new wall object in the given array

	Walls.SetSize(Walls.GetSize() + 1);
	Walls[Walls.GetSize() - 1] = new WallObject(MeshWrapper, Submeshes);
	Walls[Walls.GetSize() - 1]->name = name;
	Walls[Walls.GetSize() - 1]->parent_array = &Walls;
	Walls[Walls.GetSize() - 1]->SetValues(Walls[Walls.GetSize() - 1], parent, "Wall", name, false);
	return Walls[Walls.GetSize() - 1];
}

iMaterialWrapper* MeshObject::ChangeTexture(const char *texture, bool matcheck)
{
	//changes a texture
	//if matcheck is true, exit if old and new textures are the same

	//exit if mesh pointer's invalid
	if (!MeshWrapper)
		return 0;

	//get new material
	csRef<iMaterialWrapper> newmat = sbs->engine->GetMaterialList()->FindByName(texture);

	//exit if old and new materials are the same
	if (matcheck == true)
	{
		if (MeshWrapper->GetMeshObject()->GetMaterialWrapper() == newmat)
			return 0;
	}

	//set material if valid
	if (newmat)
	{
		MeshWrapper->GetMeshObject()->SetMaterialWrapper(newmat);
		return newmat;
	}
	else //otherwise report error
		sbs->ReportError("ChangeTexture: Invalid texture '" + csString(texture) + "'");

	return 0;
}
