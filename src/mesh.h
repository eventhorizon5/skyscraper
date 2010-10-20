/* $Id$ */

/*
	Scalable Building Simulator - Mesh and Polygon Classes
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

#ifndef _SBS_MESH_H
#define _SBS_MESH_H

class WallObject;

class SBSIMPEXP MeshObject
{
public:
	Object *object; //SBS object
	csString name; //mesh name

	MeshObject(Object* parent, const char *name, const char *filename = 0, float max_render_distance = 0, float scale_multiplier = 1);
	~MeshObject();
	void Enable(bool value);
	bool IsEnabled();
	WallObject* CreateWallObject(Object *parent, const char *name);
	iMaterialWrapper* ChangeTexture(const char *texture, bool matcheck = true);
	int FindWall(const csVector3 &point);
	void RescaleVertices(float multiplier);
	bool LoadColladaFile(const char *filename, const char *name);
	
	csRef<iMeshWrapper> MeshWrapper; //building mesh
	csRef<iGeneralFactoryState> State; //factory state
	csRefArray<iGeneralMeshSubMesh> Submeshes;
	csArray<WallObject*> Walls;
	csRef<iMovable> Movable;

private:
	bool enabled;
	csRef<iRegion> region;
};

class SBSIMPEXP WallPolygon
{
public:
	csRefArray<iGeneralMeshSubMesh> *submeshes;
	csRef<iRenderBuffer> triangles;
	csPlane3 plane;

	//array holding index extents, to get original geometry
	csArray<csVector2> index_extents;

	//texture mapping matrix and vector
	csMatrix3 t_matrix;
	csVector3 t_vector;

	iMaterialWrapper* material; //polygon materials
	csString name; //polygon names

	WallPolygon();
	~WallPolygon();
	void GetTextureMapping(csMatrix3 &t_matrix, csVector3 &t_vector);
	iGeneralMeshSubMesh* GetSubMesh();
	void GetGeometry(csRef<iMeshWrapper> meshwrapper, csArray<CS::Geometry::csContour3> &vertices, bool firstonly);
	void GetGeometry(csRef<iMeshWrapper> meshwrapper, csArray<csPoly3D> &vertices, bool firstonly);
	bool PointInside(csRef<iMeshWrapper> meshwrapper, const csVector3 &point, bool plane_check = true);
};

#endif
