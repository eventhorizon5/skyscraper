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
#include "polygon.h"
#include "texman.h"
#include "utility.h"
#include "polymesh.h"

namespace SBS {

#undef EPSILON
#define EPSILON 0.001f
#undef SMALL_EPSILON
#define SMALL_EPSILON 0.000001f

bool TextureManager::ComputeTextureMap(Matrix3 &t_matrix, Vector3 &t_vector, PolyArray &vertices, const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, Real tw, Real th)
{
	//this is modified code from the Crystal Space thingmesh system (SetTextureSpace function),
	//from the "plugins/mesh/thing/object/polygon.cpp" file.

	//given an array of vertices, this returns the texture transformation matrix and vector

	//original description:
	// Set the texture space transformation given three vertices and
	// their uv coordinates.
	//
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

	//get stored UV mappings and adjust according to width and height
	Vector2 uv1, uv2, uv3;
	uv1.x = MapUV[0].x * tw;
	uv1.y = MapUV[0].y * th;
	uv2.x = MapUV[1].x * tw;
	uv2.y = MapUV[1].y * th;
	uv3.x = MapUV[2].x * tw;
	uv3.y = MapUV[2].y * th;

	//set up 2D matrix
	Real m11, m12, m21, m22;
	m11 = uv2.x - uv1.x;
	m12 = uv3.x - uv1.x;
	m21 = uv2.y - uv1.y;
	m22 = uv3.y - uv1.y;

	//compute determinant of matrix
	Real det = m11 * m22 - m12 * m21;

	if (std::abs(det) < SMALL_EPSILON)
		return ReportError("ComputeTextureMap: Bad UV coordinates");
	else
	{
		//invert matrix
		Real inv_det = 1 / (m11 * m22 - m12 * m21);
		Real tmp1, tmp2, tmp3, tmp4;
		tmp1 = m11;
		tmp2 = m12;
		tmp3 = m21;
		tmp4 = m22;

		m11 = tmp4 * inv_det;
		m12 = -tmp2 * inv_det;
		m21 = -tmp3 * inv_det;
		m22 = tmp1 * inv_det;
	}

	Vector2 pl;
	Vector3 po, pu, pv;

	// For (0,0) and Po
	Vector2 v = Vector2(0, 0) - uv1;
	pl = Vector2(m11 * v.x + m12 * v.y, m21 * v.x + m22 * v.y);
	po = p1 + pl.x * (p2 - p1) + pl.y * (p3 - p1);
	po = Round(po, 7); //round result to prevent precision errors

	// For (1,0) and Pu
	v = Vector2(1, 0) - uv1;
	pl = Vector2(m11 * v.x + m12 * v.y, m21 * v.x + m22 * v.y);
	pu = p1 + pl.x * (p2 - p1) + pl.y * (p3 - p1);
	pu = Round(pu, 7); //round result to prevent precision errors

	// For (0,1) and Pv
	v = Vector2(0, 1) - uv1;
	pl = Vector2(m11 * v.x + m12 * v.y, m21 * v.x + m22 * v.y);
	pv = p1 + pl.x * (p2 - p1) + pl.y * (p3 - p1);
	pv = Round(pv, 7); //round result to prevent precision errors

	//compute norms of vectors
	Vector3 len1 = pu - po;
	Vector3 len2 = pv - po;
	Real len1f = sqrt(len1.x * len1.x + len1.y * len1.y + len1.z * len1.z);
	Real len2f = sqrt(len2.x * len2.x + len2.y * len2.y + len2.z * len2.z);

	return ComputeTextureSpace(t_matrix, t_vector, po, pu, len1f, pv, len2f);
}

bool TextureManager::ComputeTextureSpace(Matrix3 &m, Vector3 &v, const Vector3 &v_orig, const Vector3 &v1, Real len1, const Vector3 &v2, Real len2)
{
	//originally from Crystal Space's libs/csgeom/textrans.cpp file

	/**
	 * Calculate the matrix using two vertices (which are preferably on the
	 * plane of the polygon and are possibly (but not necessarily) two vertices
	 * of the polygon). The first vertex is seen as the origin and the second
	 * as the u-axis of the texture space coordinate system. The v-axis is
	 * calculated on the plane of the polygon and orthogonal to the given
	 * u-axis. The length of the u-axis and the v-axis is given as the 'len1'
	 * parameter.<p>
	 * For example, if 'len1' is equal to 2 this means that texture will
	 * be tiled exactly two times between vertex 'v_orig' and 'v1'.
	 * I hope this explanation is clear since I can't seem to make it
	 * any clearer :-)
	 *
	 * Use 'v1' and 'len1' for the u-axis and 'v2' and 'len2' for the v-axis.
	 */

	Real d = v_orig.squaredDistance(v1);
	//get inverse square of d
	Real invl1 = 1 / sqrt(d);

	d = v_orig.squaredDistance(v2);
	//get inverse square of d
	Real invl2 = (d) ? 1 / sqrt(d) : 0;

	Vector3 v_u = (v1 - v_orig) * len1 * invl1;
	Vector3 v_v = (v2 - v_orig) * len2 * invl2;
	Vector3 v_w = v_u.crossProduct(v_v);

	m[0][0] = v_u.x;
	m[0][1] = v_v.x;
	m[0][2] = v_w.x;
	m[1][0] = v_u.y;
	m[1][1] = v_v.y;
	m[1][2] = v_w.y;
	m[2][0] = v_u.z;
	m[2][1] = v_v.z;
	m[2][2] = v_w.z;

	v = v_orig;

	Real det = m.Determinant();
	/*if (std::abs(det) < SMALL_EPSILON)
	{
		//m = m.IDENTITY;
		//return ReportError("Error computing texture space");

		//alternate matrix inversion method
		//(standard inversion breaks when using small numbers)
		if (v_u.x == 0)
			m[0][0] = 0;
		else
			m[0][0] = 1 / v_u.x;

		if (v_v.x == 0)
			m[0][1] = 0;
		else
			m[0][1] = 1 / v_v.x;

		if (v_w.x == 0)
			m[0][2] = 0;
		else
			m[0][2] = 1 / v_w.x;

		if (v_u.y == 0)
			m[1][0] = 0;
		else
			m[1][0] = 1 / v_u.y;

		if (v_v.y == 0)
			m[1][1] = 0;
		else
			m[1][1] = 1 / v_v.y;

		if (v_w.y == 0)
			m[1][2] = 0;
		else
			m[1][2] = 1 / v_w.y;

		if (v_u.z == 0)
			m[2][0] = 0;
		else
			m[2][0] = 1 / v_u.z;

		if (v_v.z == 0)
			m[2][1] = 0;
		else
			m[2][1] = 1 / v_v.z;

		if (v_w.z == 0)
			m[2][2] = 0;
		else
			m[2][2] = 1 / v_w.z;
	}
	else*/
		m = m.Inverse(1e-10f); //standard inversion

	return true;
}

void PolyMesh::SplitWithPlane(int axis, PolyArray &orig, PolyArray &poly1, PolyArray &poly2, Real value)
{
	//from Crystal Space libs/csgeom/poly3d.cpp
	//axis is 0 for X, 1 for Y, 2 for Z
	//splits the "orig" polygon on the desired plane into two resulting polygons

	poly1.clear();
	poly2.clear();

	//preallocate memory for a worst-case scenario
	poly1.reserve(orig.size());
	poly2.reserve(orig.size());

	Vector3 ptB;
	Real sideA = 0, sideB = 0;
	Vector3 ptA = orig[orig.size() - 1];

	if (axis == 0)
		sideA = ptA.x - value;
	if (axis == 1)
		sideA = ptA.y - value;
	if (axis == 2)
		sideA = ptA.z - value;

	if (std::abs(sideA) < SMALL_EPSILON)
		sideA = 0;

	for (int i = -1; ++i < (int)orig.size();)
	{
		ptB = orig[i];
		if (axis == 0)
			sideB = ptB.x - value;
		if (axis == 1)
			sideB = ptB.y - value;
		if (axis == 2)
			sideB = ptB.z - value;

		if (std::abs(sideB) < SMALL_EPSILON)
			sideB = 0;

		if (sideB > 0)
		{
			if (sideA < 0)
			{
				// Compute the intersection point of the line
				// from point A to point B with the partition
				// plane. This is a simple ray-plane intersection.

				Vector3 v = ptB;
				v -= ptA;

				Real sect = 0;
				if (axis == 0)
					sect = -(ptA.x - value) / v.x;
				if (axis == 1)
					sect = -(ptA.y - value) / v.y;
				if (axis == 2)
					sect = -(ptA.z - value) / v.z;
				v *= sect;
				v += ptA;
				poly1.emplace_back(v);
				poly2.emplace_back(v);
			}

			poly2.emplace_back(ptB);
		}
		else if (sideB < 0)
		{
			if (sideA > 0)
			{
				// Compute the intersection point of the line
				// from point A to point B with the partition
				// plane. This is a simple ray-plane intersection.

				Vector3 v = ptB;
				v -= ptA;

				Real sect = 0;
				if (axis == 0)
					sect = -(ptA.x - value) / v.x;
				if (axis == 1)
					sect = -(ptA.y - value) / v.y;
				if (axis == 2)
					sect = -(ptA.z - value) / v.z;
				v *= sect;
				v += ptA;
				poly1.emplace_back(v);
				poly2.emplace_back(v);
			}

			poly1.emplace_back(ptB);
		}
		else
		{
			poly1.emplace_back(ptB);
			poly2.emplace_back(ptB);
		}

		ptA = ptB;
		sideA = sideB;
	}
}

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
