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
	state = scfQueryInterface<iThingFactoryState> (wrapper->GetMeshObject()->GetFactory());

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

int WallObject::AddQuad(const char *name, const csVector3 &v1, const csVector3 &v2, const csVector3 &v3, const csVector3 &v4)
{
	//add a quad polygon
	int index = state->AddQuad(v1, v2, v3, v4);
	CreateHandle(index);
	SetPolygonName(index, name);
	return index;
}

int WallObject::AddPolygon(const char *name, csVector3 *vertices, int num)
{
	//create a generic polygon
	int index = state->AddPolygon(vertices, num);
	CreateHandle(index);
	SetPolygonName(index, name);
	return index;
}

void WallObject::CreateHandle(int index)
{
	//create a polygon handle
	handles.Push(index);
}

void WallObject::SetPolygonName(int index, const char *name)
{
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
	state->SetPolygonName(csPolygonRange(index, index), newname);
}

void WallObject::DeletePolygons()
{
	//delete polygons and handles
	
	for (int i = 0; i < handles.GetSize(); i++)
	{
		if (handles[i] > -1)
		{
			state->RemovePolygon(handles[i]);
			int tmphandle = handles[i];
			handles[i] = -1;
			ReindexPolygons(tmphandle);
		}
	}
	handles.DeleteAll();

	//recreate colliders
	sbs->DeleteColliders(meshwrapper);
	sbs->CreateColliders(meshwrapper);
}

void WallObject::DeletePolygon(int index, bool recreate_colliders)
{
	//delete a single polygon

	for (int i = 0; i < handles.GetSize(); i++)
	{
		if (handles[i] == index)
		{
			state->RemovePolygon(index);
			handles[i] = -1;
			ReindexPolygons(index);
			handles.DeleteIndex(i);
			return;
		}
	}

	//recreate colliders if specified
	if (recreate_colliders == true)
	{
		sbs->DeleteColliders(meshwrapper);
		sbs->CreateColliders(meshwrapper);
	}
}

void WallObject::ReindexPolygons(int deleted_index)
{
	//reindex all polygon indices in the given wall array

	for (int i = 0; i < parent_array->GetSize(); i++)
	{
		for (int j = 0; j < parent_array->Get(i)->handles.GetSize(); j++)
		{
			if (parent_array->Get(i)->handles[j] >= deleted_index)
				parent_array->Get(i)->handles[j]--;
		}
	}
}

/////////////////////////

WallObject2::WallObject2(csRef<iMeshWrapper> wrapper, Object *proxy, bool temporary) : Object(temporary)
{
	//wall object constructor
	meshwrapper = wrapper;
	state = scfQueryInterface<iGeneralFactoryState>(wrapper->GetFactory()->GetMeshObjectFactory());

	//if proxy object is set, set object's number as proxy object's number
	if (proxy)
		Number = proxy->GetNumber();
}

WallObject2::~WallObject2()
{
	//wall object destructor

	if (sbs->FastDelete == false && parent_array && parent_deleting == false && Temporary == false)
		parent_array->Delete(this);
	if (sbs->FastDelete == false)
		DeletePolygons();
}

int WallObject2::AddQuad(const char *name, const csVector3 &v1, const csVector3 &v2, const csVector3 &v3, const csVector3 &v4)
{
	//add a quad
/*
	//create texture mapping table
	csVector2 table[] = {csVector2(tw2, th2), csVector2(0, th2), csVector2(tw2, 0), csVector2(0, 0)};

	//create a quad, map the texture, and append to the mesh
	CS::Geometry::TesselatedQuad wall (csVector3(ToRemote(x2), ToRemote(altitude), ToRemote(z2)), csVector3(ToRemote(x1), ToRemote(altitude), ToRemote(z1)), csVector3(ToRemote(x2), ToRemote(altitude + height), ToRemote(z2)));
	CS::Geometry::TableTextureMapper mapper(table);
	wall.SetMapper(&mapper);
	wall.Append(mesh->GetFactory());

	csRef<iGeneralFactoryState> state = scfQueryInterface<iGeneralFactoryState>(mesh->GetFactory()->GetMeshObjectFactory());
	csRef<iRenderBuffer> buffer = csRenderBuffer::CreateIndexRenderBuffer(state->GetTriangleCount() * 3, CS_BUF_STATIC, CS_BUFCOMP_UNSIGNED_INT, 0, state->GetVertexCount());
	csTriangle *triangleData = (csTriangle*)buffer->Lock(CS_BUF_LOCK_NORMAL);
	for (int i = 0; i < state->GetTriangleCount(); i++)
	{
		triangleData[i] = state->GetTriangles()[i];
	}
	buffer->Release();
	csRef<iGeneralMeshSubMesh> submesh = state->AddSubMesh(buffer, material, "");

	int index = state->AddQuad(v1, v2, v3, v4);
	CreateHandle(index);
	SetPolygonName(index, name);
	return index;*/
	return 0;
}

int WallObject2::AddPolygon(const char *name, csVector3 *vertices, int num)
{/*
	//create a generic polygon
	int index = state->AddPolygon(vertices, num);
	CreateHandle(index);
	SetPolygonName(index, name);
	return index;*/
	return 0;
}

void WallObject2::CreateHandle(int index)
{
	//create a polygon handle
	//handles.Push(index);
}

void WallObject2::SetPolygonName(int index, const char *name)
{/*
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
	state->SetPolygonName(csPolygonRange(index, index), newname);*/
}

void WallObject2::DeletePolygons()
{/*
	//delete polygons and handles
	
	for (int i = 0; i < handles.GetSize(); i++)
	{
		if (handles[i] > -1)
		{
			state->RemovePolygon(handles[i]);
			int tmphandle = handles[i];
			handles[i] = -1;
			ReindexPolygons(tmphandle);
		}
	}
	handles.DeleteAll();

	//recreate colliders
	sbs->DeleteColliders(meshwrapper);
	sbs->CreateColliders(meshwrapper);*/
}

void WallObject2::DeletePolygon(int index, bool recreate_colliders)
{
	//delete a single polygon
/*
	for (int i = 0; i < handles.GetSize(); i++)
	{
		if (handles[i] == index)
		{
			state->RemovePolygon(index);
			handles[i] = -1;
			ReindexPolygons(index);
			handles.DeleteIndex(i);
			return;
		}
	}

	//recreate colliders if specified
	if (recreate_colliders == true)
	{
		sbs->DeleteColliders(meshwrapper);
		sbs->CreateColliders(meshwrapper);
	}
}

void WallObject2::ReindexPolygons(int deleted_index)
{
	//reindex all polygon indices in the given wall array

	for (int i = 0; i < parent_array->GetSize(); i++)
	{
		for (int j = 0; j < parent_array->Get(i)->handles.GetSize(); j++)
		{
			if (parent_array->Get(i)->handles[j] >= deleted_index)
				parent_array->Get(i)->handles[j]--;
		}
	}*/
}

