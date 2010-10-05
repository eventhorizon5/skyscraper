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
#include <csgfx/renderbuffer.h>

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
	sbs->WallCount++;
}

WallObject::~WallObject()
{
	//wall object destructor

	if (sbs->FastDelete == false && parent_array && parent_deleting == false && Temporary == false)
		parent_array->Delete(this);

	sbs->WallCount--;
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
	csArray<csVector2> index_extents;
	csRef<iRenderBuffer> triangles = sbs->PolyMesh(meshwrapper, *submesh_array, name2, texture, array[0], tw, th, autosize, tm, tv, index_extents);

	if (!triangles)
		return 0;

	bool result;
	iMaterialWrapper* material = sbs->GetTextureMaterial(texture, result, name2);

	csPlane3 plane = csPoly3D::ComputePlane(array[0]);
	int index = CreateHandle(triangles, index_extents, tm, tv, material, name2, plane);
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
	csArray<csVector2> index_extents;
	csRef<iRenderBuffer> triangles = sbs->PolyMesh(meshwrapper, *submesh_array, name2, texture, array[0], tw, th, autosize, tm, tv, index_extents);

	if (!triangles)
		return 0;

	bool result;
	iMaterialWrapper* material = sbs->GetTextureMaterial(texture, result, name2);

	csPlane3 plane = csPoly3D::ComputePlane(array[0]);
	int index = CreateHandle(triangles, index_extents, tm, tv, material, name2, plane);
	return &handles[index];
}

WallPolygon* WallObject::AddPolygon(const char *name, csRef<iMaterialWrapper> material, csArray<CS::Geometry::csContour3> &vertices, csMatrix3 &tex_matrix, csVector3 &tex_vector)
{
	//add a set of polygons, providing the original material and texture mapping
	csString name2 = ProcessName(name);
	csArray<csVector2> index_extents;
	csRef<iRenderBuffer> triangles = sbs->PolyMesh(meshwrapper, *submesh_array, name2, material, vertices, tex_matrix, tex_vector, index_extents);

	if (!triangles)
		return 0;

	csPlane3 plane = csPoly3D::ComputePlane(vertices[0]);
	int index = CreateHandle(triangles, index_extents, tex_matrix, tex_vector, material, name2, plane);
	return &handles[index];
}

int WallObject::CreateHandle(csRef<iRenderBuffer> triangles, csArray<csVector2> &index_extents, csMatrix3 &tex_matrix, csVector3 &tex_vector, iMaterialWrapper* material, const char *name, csPlane3 &plane)
{
	//create a polygon handle
	int i = handles.GetSize();
	handles.SetSize(handles.GetSize() + 1);
	handles[i].submeshes = submesh_array;
	handles[i].index_extents = index_extents;
	handles[i].t_matrix = tex_matrix;
	handles[i].t_vector = tex_vector;
	handles[i].material = material;
	handles[i].plane = plane;
	SetPolygonName(i, name);

	//copy triangle data into new buffer (this prevents active buffers from being stored into the handle)
	int *data = (int*)triangles->Lock(CS_BUF_LOCK_NORMAL);
	int size = triangles->GetElementCount();
	handles[i].triangles = csRenderBuffer::CreateIndexRenderBuffer(size, CS_BUF_STATIC, CS_BUFCOMP_UNSIGNED_INT, triangles->GetRangeStart(), triangles->GetRangeEnd());
	handles[i].triangles->CopyInto(data, size);
	triangles->Release();

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
		sbs->DeleteVertices(*parent_array, handles[index].triangles);

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

WallPolygon* WallObject::GetHandle(int index)
{
	if (index > -1 && index < handles.GetSize())
		return &handles[index];
	return 0;
}

int WallObject::FindPolygon(const char *name)
{
	//find a polygon object by name

	csString name2 = name;
	for (int i = 0; i < handles.GetSize(); i++)
	{
		csString tmpname = handles[i].name;
		if (tmpname.Find("(") == 0)
		{
			//strip object number
			int loc = tmpname.Find(")");
			tmpname.DeleteAt(0, loc + 1);
		}
		if (name2 == tmpname)
			return i;
	}
	return -1;
}

void WallObject::GetGeometry(int index, csArray<CS::Geometry::csContour3> &vertices, bool firstonly)
{
	//gets vertex geometry using mesh's vertex extent arrays; returns vertices in 'vertices'

	if (index < 0 || index >= handles.GetSize())
		return;

	handles[index].GetGeometry(meshwrapper, vertices, firstonly);
}

void WallObject::SetPolygonName(int index, const char *name)
{
		if (index < 0 || index >= handles.GetSize())
			return;

		//set polygon name
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

        //set polygon name
        handles[index].name = newname;
}
