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

WallObject::WallObject(csRef<iMeshWrapper> wrapper, csRefArray<iGeneralMeshSubMesh> &submeshes, Object *proxy, bool temporary) : Object(temporary)
{
	//wall object constructor
	meshwrapper = wrapper;
	state = scfQueryInterface<iGeneralFactoryState>(wrapper->GetFactory()->GetMeshObjectFactory());
	submesh_array = &submeshes;

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
}

WallPolygon* WallObject::AddQuad(const char *name, const char *texture, const csVector3 &v1, const csVector3 &v2, const csVector3 &v3, const csVector3 &v4, float tw, float th, bool autosize)
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
	csRef<iRenderBuffer> triangles = sbs->PolyMesh(meshwrapper, *submesh_array, name2, texture, array[0], tw, th, autosize, tm, tv);

	if (!triangles)
		return 0;
	int index = CreateHandle(triangles, array, tm, tv, name2);
	return &handles[index];
}

WallPolygon* WallObject::AddPolygon(const char *name, const char *texture, csVector3 *vertices, int num, float tw, float th, bool autosize)
{
	//create a generic polygon
	csArray<CS::Geometry::csContour3> array;
	array.SetSize(1);
	for (int i = 0; i < num; i++)
		array[0].Push(vertices[i]);
	csString name2 = ProcessName(name);
	csMatrix3 tm;
	csVector3 tv;
	csRef<iRenderBuffer> triangles = sbs->PolyMesh(meshwrapper, *submesh_array, name2, texture, array[0], tw, th, autosize, tm, tv);

	if (!triangles)
		return 0;
	int index = CreateHandle(triangles, array, tm, tv, name2);
	return &handles[index];
}

WallPolygon* WallObject::AddPolygon(const char *name, csRef<iMaterialWrapper> material, csArray<CS::Geometry::csContour3> &vertices, csMatrix3 &tex_matrix, csVector3 &tex_vector)
{
	//add a set of polygons, providing the original material and texture mapping
	csString name2 = ProcessName(name);
	csRef<iRenderBuffer> triangles = sbs->PolyMesh(meshwrapper, *submesh_array, name2, material, vertices, tex_matrix, tex_vector);

	if (!triangles)
		return 0;
	int index = CreateHandle(triangles, vertices, tex_matrix, tex_vector, name2);
	return &handles[index];
}

int WallObject::CreateHandle(csRef<iRenderBuffer> triangles, csArray<CS::Geometry::csContour3> &vertices, csMatrix3 &tex_matrix, csVector3 &tex_vector, const char *name)
{
	//create a polygon handle
	int i = handles.GetSize();
	handles.SetSize(handles.GetSize() + 1);
	handles[i].submeshes = submesh_array;
	handles[i].triangles = triangles;
	handles[i].geometry = vertices;
	handles[i].t_matrix = tex_matrix;
	handles[i].t_vector = tex_vector;
	handles[i].material = submesh_array->Get(i)->GetMaterial();
	handles[i].name = name;
	return i;
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
		DeletePolygon(i, false);

	//recreate colliders
	sbs->DeleteColliders(meshwrapper);
	sbs->CreateColliders(meshwrapper);
}

void WallObject::DeletePolygon(int index, bool recreate_colliders)
{
	//delete a single polygon

	if (index > -1 && index < handles.GetSize())
	{
		//delete triangles
		sbs->ReindexSubMesh(state, *handles[index].submeshes, handles[index].triangles, handles[index].material, handles[index].name, false);

		//delete related mesh vertices
		DeleteVertices(handles[index].triangles);

		//delete polygon
		handles.DeleteIndex(index);

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

int WallObject::GetHandleCount()
{
	return handles.GetSize();
}

void WallObject::DeleteVertices(iRenderBuffer *deleted_indices)
{
	//delete related mesh vertices using provided index array
	//then reindex all mesh triangle indices in all submeshes
	//this should be done after a polygon is deleted

	//get integer array of triangle indices
	csArray<int> indices;
	int* buffer2 = (int*)deleted_indices->Lock(CS_BUF_LOCK_NORMAL);
	for (int i = 0; i < deleted_indices->GetElementCount(); i++)
		indices.Push(buffer2[i]);
	deleted_indices->Release();

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

	//reindex triangle indices in all polygons
	for (int i = 0; i < handles.GetSize(); i++)
	{
		iRenderBuffer *indices = handles[i].triangles;
		
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

WallPolygon* WallObject::GetHandle(int index)
{
	if (index > -1 && index < handles.GetSize())
		return &handles[index];
	return 0;
}
