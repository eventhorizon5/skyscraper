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
