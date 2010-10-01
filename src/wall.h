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

#ifndef _SBS_WALL_H
#define _SBS_WALL_H

class SBSIMPEXP WallObject : public Object
{
public:

	//functions
	WallObject(csRef<iMeshWrapper> wrapper, csRefArray<iGeneralMeshSubMesh> &submeshes, Object *proxy = 0, bool temporary = false);
	~WallObject();
	WallPolygon* AddQuad(const char *name, const char *texture, const csVector3 &v1, const csVector3 &v2, const csVector3 &v3, const csVector3 &v4, float tw, float th, bool autosize);
	WallPolygon* AddPolygon(const char *name, const char *texture, csVector3 *vertices, int num, float tw, float th, bool autosize);
	WallPolygon* AddPolygon(const char *name, csRef<iMaterialWrapper> material, csArray<CS::Geometry::csContour3> &vertices, csMatrix3 &tex_matrix, csVector3 &tex_vector);
	int CreateHandle(csRef<iRenderBuffer> triangles, csArray<csVector2> &index_extents, csMatrix3 &tex_matrix, csVector3 &tex_vector, iMaterialWrapper* material, const char *name);
	void DeletePolygons();
	void DeletePolygon(int index, bool recreate_colliders);
	void DeleteVertices(csArray<int> &deleted_indices);
	csString ProcessName(const char *name);
	int GetHandleCount();
	WallPolygon* GetHandle(int index);
	int FindPolygon(const char *name);
	void GetGeometry(int index, csArray<CS::Geometry::csContour3> &vertices, bool firstonly = false);

	//mesh wrapper
	csRef<iMeshWrapper> meshwrapper;

	//mesh factory state
	csRef<iGeneralFactoryState> state;

	//name
	csString name;

	//parent array
	csArray<WallObject*> *parent_array;

	//parent submesh array
	csRefArray<iGeneralMeshSubMesh> *submesh_array;

	csArray<WallPolygon> handles;
};

#endif
