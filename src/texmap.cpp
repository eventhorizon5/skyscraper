/* $Id$ */

/*
	This code was originally part of Crystal Space
    Copyright (C) 1998-2001 by Jorrit Tyberghein
	OGRE port and SBS modifications Copyright (C)2010 Ryan Thoryk

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

#include "globals.h"
#include "sbs.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

bool MeshObject::ComputeTextureMap(Ogre::Matrix3 &t_matrix, Ogre::Vector3 &t_vector, std::vector<Ogre::Vector3> &vertices, const Ogre::Vector3 &p1, const Ogre::Vector2 &uv1, const Ogre::Vector3 &p2, const Ogre::Vector2 &uv2, const Ogre::Vector3 &p3, const Ogre::Vector2 &uv3)
{
	//this is modified code from the Crystal Space thingmesh system, from the "plugins/mesh/thing/object/polygon.cpp" file.
	//given an array of vertices, this returns the texture transformation matrix and vector

	//original description:
	// Some explanation. We have three points for
	// which we know the uv coordinates. This gives:
	//     P1 -> UV1
	//     P2 -> UV2
	//     P3 -> UV3
	// P1, P2, and P3 are on the same plane so we can write:
	//     P = P1 + lambda * (P2-P1) + mu * (P3-P1)
	// For the same lambda and mu we can write:
	//     UV = UV1 + lambda * (UV2-UV1) + mu * (UV3-UV1)
	// What we want is Po, Pu, and Pv (also on the same
	// plane) so that the following uv coordinates apply:
	//     Po -> 0,0
	//     Pu -> 1,0
	//     Pv -> 0,1
	// The UV equation can be written as follows:
	//     U = U1 + lambda * (U2-U1) + mu * (U3-U1)
	//     V = V1 + lambda * (V2-V1) + mu * (V3-V1)
	// This is a matrix equation (2x2 matrix):
	//     UV = UV1 + M * PL
	// We have UV in this case and we need PL so we
	// need to invert this equation:
	//     (1/M) * (UV - UV1) = PL

	csMatrix2 m (uv2.x - uv1.x, uv3.x - uv1.x, uv2.y - uv1.y, uv3.y - uv1.y);
	float det = m.Determinant();

	if (ABS(det) < 0.0001f)
	{
		ReportError("Warning: bad UV coordinates");

		/*if (!((p1-p2) < SMALL_EPSILON))
			SetTextureSpace(p1, p2, 1);
		else if (!((p1-p3) < SMALL_EPSILON))
			SetTextureSpace(p1, p3, 1);*/
		return false;
	}
	else
		m.Invert();

	Ogre::Vector2 pl;
	Ogre::Vector3 po, pu, pv;

	// For (0,0) and Po
	pl = m * (Ogre::Vector2(0, 0) - uv1);
	po = p1 + pl.x * (p2 - p1) + pl.y * (p3 - p1);

	// For (1,0) and Pu
	pl = m * (Ogre::Vector2(1, 0) - uv1);
	pu = p1 + pl.x * (p2 - p1) + pl.y * (p3 - p1);

	// For (0,1) and Pv
	pl = m * (Ogre::Vector2(0, 1) - uv1);
	pv = p1 + pl.x * (p2 - p1) + pl.y * (p3 - p1);

	ComputeTextureSpace(t_matrix, t_vector, po, pu, (pu - po).Norm(), pv, (pv - po).Norm());
	return true;
}

bool MeshObject::ComputeTextureSpace(Ogre::Matrix3 &m, Ogre::Vector3 &v, const Ogre::Vector3 &v_orig, const Ogre::Vector3 &v1, float len1, const Ogre::Vector3 &v2, float len2)
{
	//originally from Crystal Space's libs/csgeom/textrans.cpp file
	
	float d = csSquaredDist::PointPoint(v_orig, v1);
	float invl1 = csQisqrt(d);

	d = csSquaredDist::PointPoint(v_orig, v2);
	float invl2 = (d) ? csQisqrt (d) : 0;

	Ogre::Vector3 v_u = (v1 - v_orig) * len1 * invl1;
	Ogre::Vector3 v_v = (v2 - v_orig) * len2 * invl2;
	Ogre::Vector3 v_w = v_u % v_v;

	m.m11 = v_u.x;
	m.m12 = v_v.x;
	m.m13 = v_w.x;
	m.m21 = v_u.y;
	m.m22 = v_v.y;
	m.m23 = v_w.y;
	m.m31 = v_u.z;
	m.m32 = v_v.z;
	m.m33 = v_w.z;

	v = v_orig;

	float det = m.Determinant ();
	if (ABS (det) < SMALL_EPSILON)
	{
		m.Identity();
		return false;
	}
	else
		m.Invert ();

	return true;
}
