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
	int AddQuad(const char *name, const csVector3 &v1, const csVector3 &v2, const csVector3 &v3, const csVector3 &v4);
	int AddPolygon(const char *name, csVector3 *vertices, int num);
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
	csRef<iGeneralMeshSubMesh> AddQuad(const char *name, const char *texture, const csVector3 &v1, const csVector3 &v2, const csVector3 &v3, const csVector3 &v4, float tw, float th, bool autosize);
	csRef<iGeneralMeshSubMesh> AddPolygon(const char *name, const char *texture, csVector3 *vertices, int num, float tw, float th, bool autosize);
	csRef<iGeneralMeshSubMesh> AddPolygon(const char *name, csRef<iMaterialWrapper> material, csVector3 *vertices, int num, csMatrix3 &tex_matrix, csVector3 &tex_vector);
	int CreateHandle(csRef<iGeneralMeshSubMesh> handle, CS::Geometry::csContour3 &vertices, csMatrix3 &tex_matrix, csVector3 &tex_vector);
	void DeletePolygons();
	void DeletePolygon(csRef<iGeneralMeshSubMesh> handle, bool recreate_colliders);
	void ReindexPolygons(int deleted_index);
	csString ProcessName(const char *name);
	CS::Geometry::csContour3* GetGeometry(csRef<iGeneralMeshSubMesh> handle);
	int GetHandleCount();
	csRef<iGeneralMeshSubMesh> GetHandle(int index);
	int FindHandleIndex(csRef<iGeneralMeshSubMesh> handle);
	void GetTextureMapping(int index, csMatrix3 &t_matrix, csVector3 &t_vector);

	//mesh wrapper
	csRef<iMeshWrapper> meshwrapper;

	//mesh factory state
	csRef<iGeneralFactoryState> state;

	//name
	csString name;

	//parent array
	csArray<WallObject2*> *parent_array;

private:
	//array set holding original polygon geometry
	csArray<CS::Geometry::csContour3> geometry;

	//texture mapping matrix and vector
	csArray<csMatrix3> t_matrix;
	csArray<csVector3> t_vector;

	//polygon index array
	csArray<iGeneralMeshSubMesh*> handles;
};

#endif
