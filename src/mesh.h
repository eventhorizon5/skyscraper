/* $Id$ */

/*
	Scalable Building Simulator - 3D Mesh Functions
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

class SBSIMPEXP WallPolygon
{
public:
	csRefArray<iGeneralMeshSubMesh> *submeshes;
	csRef<iRenderBuffer> triangles;

	//array holding index extents, to get original geometry
	csArray<csVector2> index_extents;

	//texture mapping matrix and vector
	csMatrix3 t_matrix;
	csVector3 t_vector;

	iMaterialWrapper* material; //polygon materials
	csString name; //polygon names

	void GetTextureMapping(csMatrix3 &t_matrix, csVector3 &t_vector);
	iGeneralMeshSubMesh* GetSubMesh();
};

#endif
