/* $Id$ */

/*
	Scalable Building Simulator - Wall Object Class
	The Skyscraper Project - Version 1.6 Alpha
	Copyright (C)2005-2009 Ryan Thoryk
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

WallObject::WallObject(csRef<iMeshWrapper> wrapper)
{
	//polygon object constructor
	meshwrapper = wrapper;
	state = scfQueryInterface<iThingFactoryState> (wrapper->GetMeshObject()->GetFactory());
}

WallObject::~WallObject()
{
	//polygon object destructor
	handles = 0;
	meshwrapper = 0;
	state = 0;
}

int WallObject::AddQuad(const csVector3 &v1, const csVector3 &v2, const csVector3 &v3, const csVector3 &v4)
{
	//add a quad polygon
	int newhandle = state->AddQuad(v1, v2, v3, v4);
	CreateHandle(newhandle);
	return newhandle;
}

int WallObject::AddPolygon(csVector3 *vertices, int num)
{
	//create a generic polygon
	int index = state->AddPolygon(vertices, num);
	CreateHandle(index);
	return index;
}

void WallObject::CreateHandle(int index)
{
	//create a polygon handle
	csRef<iPolygonHandle> newhandle = state->CreatePolygonHandle(index);
	handles.Push(newhandle);
}

void WallObject::SetPolygonName(int index, const char *name)
{
	//set polygon name
	state->SetPolygonName(csPolygonRange(index, index), name);
}
