/* $Id$ */

/*
	Scalable Building Simulator - Polygon Object Class
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

#ifndef _SBS_WALL_H
#define _SBS_WALL_H

//thing mesh version
class WallObject : public Object
{
public:

	//functions
	WallObject(csRef<iMeshWrapper> wrapper, Object *proxy = 0, bool temporary = false);
	~WallObject();
	int AddQuad(const csVector3 &v1, const csVector3 &v2, const csVector3 &v3, const csVector3 &v4);
	int AddPolygon(csVector3 *vertices, int num);
	void CreateHandle(int index);
	void SetPolygonName(int index, const char *name);
	void DeletePolygons();
	void DeletePolygon(int index, bool recreate_colliders);
	void ReindexPolygons(int deleted_index);

	//polygon index array
	csArray<int> handles;

	//mesh wrapper
	csRef<iMeshWrapper> meshwrapper;

	//mesh factory state
	csRef<iThingFactoryState> state;

	//name
	csString name;

	//parent array
	csArray<WallObject*> *parent_array;
};

//genmesh version
class WallObject2 : public Object
{
public:

	//functions
	WallObject2(csRef<iMeshWrapper> wrapper, Object *proxy = 0, bool temporary = false);
	~WallObject2();
	int AddQuad(const csVector3 &v1, const csVector3 &v2, const csVector3 &v3, const csVector3 &v4);
	int AddPolygon(csVector3 *vertices, int num);
	void CreateHandle(int index);
	void SetPolygonName(int index, const char *name);
	void DeletePolygons();
	void DeletePolygon(int index, bool recreate_colliders);
	void ReindexPolygons(int deleted_index);

	//polygon index array
	csArray<int> handles;

	//mesh wrapper
	csRef<iMeshWrapper> meshwrapper;

	//mesh factory state
	csRef<iThingFactoryState> state;

	//name
	csString name;

	//parent array
	csArray<WallObject2*> *parent_array;
};

#endif
