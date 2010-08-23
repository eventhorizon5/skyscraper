/* $Id$ */

/*
	Scalable Building Simulator - Wall Object Class
	The Skyscraper Project - Version 1.7 Alpha
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

	CS::Geometry::csContour3 array;
	array.Push(v1);
	array.Push(v2);
	array.Push(v3);
	array.Push(v4);
	csString name2 = ProcessName(name);
	csMatrix3 tm;
	csVector3 tv;
	iGeneralMeshSubMesh *handle = sbs->PolyMesh(meshwrapper, name2, texture, array, tw, th, autosize, tm, tv);
	return CreateHandle(handle, array, tm, tv);
}

int WallObject::AddPolygon(const char *name, const char *texture, csVector3 *vertices, int num, float tw, float th, bool autosize)
{
	//create a generic polygon
	CS::Geometry::csContour3 array;
	for (int i = 0; i < num; i++)
		array.Push(vertices[i]);
	csString name2 = ProcessName(name);
	csMatrix3 tm;
	csVector3 tv;
	iGeneralMeshSubMesh *handle = sbs->PolyMesh(meshwrapper, name2, texture, array, tw, th, autosize, tm, tv);
	return CreateHandle(handle, array, tm, tv);
}

int WallObject::AddPolygon(const char *name, csRef<iMaterialWrapper> material, csVector3 *vertices, int num, csMatrix3 &tex_matrix, csVector3 &tex_vector)
{
	//create a generic polygon
	CS::Geometry::csContour3 array;
	for (int i = 0; i < num; i++)
		array.Push(vertices[i]);
	csString name2 = ProcessName(name);
	iGeneralMeshSubMesh *handle = sbs->PolyMesh(meshwrapper, name2, material, array, tex_matrix, tex_vector);
	return CreateHandle(handle, array, tex_matrix, tex_vector);
}

int WallObject::CreateHandle(iGeneralMeshSubMesh* handle, CS::Geometry::csContour3 &vertices, csMatrix3 &tex_matrix, csVector3 &tex_vector)
{
	//create a polygon handle
	csPrintf("Added handle: %p - %s\n", handle, handle->GetName());
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
		{
			state->DeleteSubMesh(handles[i]);
			//int tmphandle = handles[i];
			handles[i] = 0;
			geometry[i].DeleteAll();
			//ReindexPolygons(tmphandle);
		}
	}
	handles.DeleteAll();
	geometry.DeleteAll();
	t_matrix.DeleteAll();
	t_vector.DeleteAll();
	state->Invalidate();

	//recreate colliders
	sbs->DeleteColliders(meshwrapper);
	sbs->CreateColliders(meshwrapper);
}

void WallObject::DeletePolygon(int index, bool recreate_colliders)
{
	//delete a single polygon

	if (index > -1 && index < handles.GetSize())
	{
		state->DeleteSubMesh(GetHandle(index));
		handles[index] = 0;
		geometry[index].DeleteAll();
		//ReindexPolygons(index);
		handles.DeleteIndex(index);
		geometry.DeleteIndex(index);
		t_matrix.DeleteIndex(index);
		t_vector.DeleteIndex(index);
		state->Invalidate();

		//recreate colliders if specified
		if (recreate_colliders == true)
		{
			sbs->DeleteColliders(meshwrapper);
			sbs->CreateColliders(meshwrapper);
		}
	}
}

void WallObject::ReindexPolygons(int deleted_index)
{
	//reindex all polygon indices in the given wall array

	/*for (int i = 0; i < parent_array->GetSize(); i++)
	{
		for (int j = 0; j < parent_array->Get(i)->handles.GetSize(); j++)
		{
			if (parent_array->Get(i)->handles[j] >= deleted_index)
				parent_array->Get(i)->handles[j]--;
		}
	}*/
}

CS::Geometry::csContour3* WallObject::GetGeometry(iGeneralMeshSubMesh *handle)
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
