/* $Id$ */

/*
	Scalable Building Simulator - Wall Object Class
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
#include "wall.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

WallObject::WallObject(csRef<iMeshWrapper> wrapper, Object *proxy, bool temporary) : Object(temporary)
{
	//wall object constructor
	meshwrapper = wrapper;
	state = scfQueryInterface<iGeneralFactoryState>(wrapper->GetFactory()->GetMeshObjectFactory());

	//if proxy object is set, set object's number as proxy object's number
	if (proxy)
		Number = proxy->GetNumber();
}

WallObject::~WallObject()
{
	//wall object destructor

	if (sbs->FastDelete == false && parent_array && parent_deleting == false && Temporary == false)
		parent_array->Delete(this);

	handles.DeleteAll();
	geometry.DeleteAll();
	t_matrix.DeleteAll();
	t_vector.DeleteAll();
}

int WallObject::AddQuad(const char *name, const char *texture, const csVector3 &v1, const csVector3 &v2, const csVector3 &v3, const csVector3 &v4, float tw, float th, bool autosize)
{
	//add a quad

	csArray<CS::Geometry::csContour3> array;
	array.SetSize(1);
	array[0].Push(v1);
	array[0].Push(v2);
	array[0].Push(v3);
	array[0].Push(v4);
	csString name2 = ProcessName(name);
	csMatrix3 tm;
	csVector3 tv;
	csRef<iGeneralMeshSubMesh> handle = sbs->PolyMesh(meshwrapper, name2, texture, array[0], tw, th, autosize, tm, tv);
	return CreateHandle(handle, array, tm, tv);
}

int WallObject::AddPolygon(const char *name, const char *texture, csVector3 *vertices, int num, float tw, float th, bool autosize)
{
	//create a generic polygon
	csArray<CS::Geometry::csContour3> array;
	array.SetSize(1);
	for (int i = 0; i < num; i++)
		array[0].Push(vertices[i]);
	csString name2 = ProcessName(name);
	csMatrix3 tm;
	csVector3 tv;
	csRef<iGeneralMeshSubMesh> handle = sbs->PolyMesh(meshwrapper, name2, texture, array[0], tw, th, autosize, tm, tv);
	return CreateHandle(handle, array, tm, tv);
}

int WallObject::AddPolygon(const char *name, csRef<iMaterialWrapper> material, csArray<CS::Geometry::csContour3> &vertices, csMatrix3 &tex_matrix, csVector3 &tex_vector)
{
	//add a set of polygons, providing the original material and texture mapping
	csString name2 = ProcessName(name);
	csRef<iGeneralMeshSubMesh> handle = sbs->PolyMesh(meshwrapper, name2, material, vertices, tex_matrix, tex_vector);
	return CreateHandle(handle, vertices, tex_matrix, tex_vector);
}

int WallObject::CreateHandle(csRef<iGeneralMeshSubMesh> handle, csArray<CS::Geometry::csContour3> &vertices, csMatrix3 &tex_matrix, csVector3 &tex_vector)
{
	//create a polygon handle
	handles.Push(handle);
	geometry.Push(vertices);
	t_matrix.Push(tex_matrix);
	t_vector.Push(tex_vector);
	return handles.GetSize() - 1;
}

csString WallObject::ProcessName(const char *name)
{
	//process name for use
	csString name_modified = name;

	//strip off object ID from name if it exists
	if (name_modified.Find("(") == 0)
		name_modified.DeleteAt(0, name_modified.Find(")") + 1);

	//construct name
	csString newname = "(";
	csString num;
	num = Number;
	newname.Append(num + ")");
	newname.Append(name_modified);
	return newname;
}

void WallObject::DeletePolygons()
{
	//delete polygons and handles
	
	for (int i = 0; i < handles.GetSize(); i++)
	{
		if (handles[i])
			DeletePolygon(i, false);
	}

	//recreate colliders
	sbs->DeleteColliders(meshwrapper);
	sbs->CreateColliders(meshwrapper);
}

void WallObject::DeletePolygon(int index, bool recreate_colliders)
{
	//delete a single polygon

	if (index > -1 && index < handles.GetSize())
	{
		iGeneralMeshSubMesh *submesh = GetHandle(index);

		//get submesh's triangle vertices (which will be deleted)
		iRenderBuffer* buffer = submesh->GetIndices();
		int* buffer2 = (int*)buffer->Lock(CS_BUF_LOCK_NORMAL);

		//copy vertex indices into index array
		csArray<int> indices;
		for (int i = 0; i < buffer->GetElementCount(); i++)
			indices.Push(buffer2[i]);
		buffer->Release();

		//delete submesh
		state->DeleteSubMesh(submesh);

		//delete related mesh vertices
		DeleteVertices(indices);

		//clean up data
		handles[index] = 0;
		handles.DeleteIndex(index);
		geometry.DeleteIndex(index);
		t_matrix.DeleteIndex(index);
		t_vector.DeleteIndex(index);

		//reprocess mesh
		state->Invalidate();

		//recreate colliders if specified
		if (recreate_colliders == true)
		{
			sbs->DeleteColliders(meshwrapper);
			sbs->CreateColliders(meshwrapper);
		}
	}
}

void WallObject::DeleteVertices(csArray<int> &deleted_indices)
{
	//delete related mesh vertices using provided index array
	//then reindex all mesh triangle indices in all submeshes
	//this should be done after a submesh is deleted

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
	for (int i = 0; i < deleted_indices.GetSize(); i++)
		deleted2.PushSmart(deleted_indices[i]);
	deleted2.Sort();

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
}

csArray<CS::Geometry::csContour3>* WallObject::GetGeometry(iGeneralMeshSubMesh *handle)
{
	//get the original geometry of the specified submesh

	for (int i = 0; i < handles.GetSize(); i++)
	{
		if (handles[i] == handle && i < geometry.GetSize())
			return &geometry[i];
	}
	return 0;
}

int WallObject::GetHandleCount()
{
	return handles.GetSize();
}

iGeneralMeshSubMesh* WallObject::GetHandle(int index)
{
	if (index > -1 && index < handles.GetSize())
		return handles[index];
}

int WallObject::FindHandleIndex(iGeneralMeshSubMesh *handle)
{
	//perform a linear search to find a handle index

	for (int i = 0; i < handles.GetSize(); i++)
	{
		if (handles[i] == handle)
			return i;
	}
}

void WallObject::GetTextureMapping(int index, csMatrix3 &tm, csVector3 &tv)
{
	//return texture mapping matrix and vector of the specified submesh
	if (index > -1 && index < handles.GetSize())
	{
		tm = t_matrix[index];
		tv = t_vector[index];
	}
}
