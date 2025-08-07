/*
	This code was originally part of Crystal Space
	Available at http://www.crystalspace3d.org
	Copyright (C) 1998-2005 by Jorrit Tyberghein
	Modifications Copyright (C)2010 Ryan Thoryk

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library; if not, write to the Free
	Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <math.h>
#include "globals.h"
#include "sbs.h"
#include "polymesh.h"

namespace SBS {

Vector3 PolyMesh::ComputeNormal(PolyArray &vertices, Real &D)
{
	//from Crystal Space libs/csgeom/poly3d.cpp
	//calculate polygon normal

	float ayz = 0;
	float azx = 0;
	float axy = 0;
	size_t i, i1;
	float x1, y1, z1, x, y, z;

	i1 = vertices.size() - 1;
	x1 = (float)vertices[i1].x;
	y1 = (float)vertices[i1].y;
	z1 = (float)vertices[i1].z;
	for (i = 0; i < vertices.size(); i++)
	{
		x = (float)vertices[i].x;
		y = (float)vertices[i].y;
		z = (float)vertices[i].z;
		ayz += (z1 + z) * (y - y1);
		azx += (x1 + x) * (z - z1);
		axy += (y1 + y) * (x - x1);
		x1 = x;
		y1 = y;
		z1 = z;
	}

	float sqd = ayz * ayz + azx * azx + axy * axy;
	float invd;
	if (sqd < SMALL_EPSILON)
		invd = 1.0f / SMALL_EPSILON;
	else
		invd = 1.0f / sqrt(sqd);
	Vector3 norm;
	norm.x = ayz * invd;
	norm.y = azx * invd;
	norm.z = axy * invd;
	D = -norm.x * vertices[0].x - norm.y * vertices[0].y - norm.z * vertices[0].z;
	return norm;
}

}
