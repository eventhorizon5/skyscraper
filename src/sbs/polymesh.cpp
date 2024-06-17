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

#include "globals.h"
#include "sbs.h"
#include "camera.h"
#include "wall.h"
#include "texture.h"
#include "profiler.h"
#include "polymesh.h"

namespace SBS {

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

	/*if (sbs->Headless == true)
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
	}*/

	return true;
}

bool PolyMesh::ReplaceTexture(const std::string &oldtexture, const std::string &newtexture)
{
	//replace submesh materials named oldtexture with newtexture
	//int submesh = FindMatchingSubMesh(oldtexture);
	int submesh = -1;
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

bool PolyMesh::CreateMesh(const std::string &name, const std::string &texture, PolyArray &vertices, Real tw, Real th, bool autosize, Matrix3 &t_matrix, Vector3 &t_vector, std::vector<std::vector<Polygon::Geometry> > &geometry, std::vector<Triangle> &triangles, PolygonSet &converted_vertices)
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

	return CreateMesh(name, material, converted_vertices, t_matrix, t_vector, geometry, triangles, converted_vertices, tw2, th2, false);
}

bool PolyMesh::CreateMesh(const std::string &name, const std::string &material, PolygonSet &vertices, Matrix3 &tex_matrix, Vector3 &tex_vector, std::vector<std::vector<Polygon::Geometry> > &geometry, std::vector<Triangle> &triangles, PolygonSet &converted_vertices, Real tw, Real th, bool convert_vertices)
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

	//initialize geometry arrays
	size_t size = 0;
	geometry.resize(trimesh_size);

	for (size_t i = 0; i < trimesh_size; i++)
	{
		geometry[i].resize(converted_vertices[i].size());
	}

	//populate vertices, normals, and texels for mesh data
	unsigned int k = 0;

	for (size_t i = 0; i < trimesh_size; i++)
	{
		for (size_t j = 0; j < converted_vertices[i].size(); j++)
		{
			//calculate normal
			Vector3 normal = sbs->ComputePlane(converted_vertices[i], false).normal;

			geometry[i][j].vertex = converted_vertices[i][j];
			geometry[i][j].normal = normal;
			geometry[i][j].texel = table[k];
			k++;
		}
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
	//int index = Process(geometry, triangles, material, true);

	//recreate colliders if specified
	if (sbs->DeleteColliders == true)
		mesh->DeleteCollider();

	//if (sbs->RenderOnStartup == true)
		//mesh->Prepare();

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

/*int PolyMesh::Process(std::vector<Polygon::Geometry> &vertices, std::vector<Triangle> &indices, const std::string &material, bool add)
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
		createnew = true;
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
}*/

/*int PolyMesh::FindMatchingSubMesh(const std::string &material)
{
	//find a submesh with a matching material
	//returns array index

	for (size_t i = 0; i < Submeshes.size(); i++)
	{
		if (Submeshes[i].Name == material)
			return (int)i;
	}
	return -1;
}*/

/*void PolyMesh::GetMeshInformation(const Ogre::Mesh* const mesh, int &vertex_count, Vector3* &vertices, int &index_count, unsigned long* &indices, Ogre::AxisAlignedBox &extents)
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
}*/

unsigned int PolyMesh::GetVertexCount()
{
	unsigned int total = 0;

	for (int i = 0; i < mesh->Walls.size(); i++)
	{
		total += mesh->Walls[i]->GetVertexCount();
	}

	return total;
}

unsigned int PolyMesh::GetTriangleCount(const std::string &material)
{
	unsigned int total = 0;

	for (int i = 0; i < mesh->Walls.size(); i++)
	{
		for (int j = 0; j < mesh->Walls[i]->GetPolygonCount(); j++)
		{
			Polygon *poly = mesh->Walls[i]->GetPolygon(j);

			if (poly->material == material || material == "")
				total += poly->triangles.size();
		}
	}

	return total;
}

}
