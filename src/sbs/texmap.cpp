/* $Id$ */

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
#include "texture.h"
#include "unix.h"

namespace SBS {

#undef EPSILON
#define EPSILON 0.001f
#undef SMALL_EPSILON
#define SMALL_EPSILON 0.000001f

bool TextureManager::ComputeTextureMap(Ogre::Matrix3 &t_matrix, Ogre::Vector3 &t_vector, std::vector<Ogre::Vector3> &vertices, const Ogre::Vector3 &p1, const Ogre::Vector3 &p2, const Ogre::Vector3 &p3, float tw, float th)
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
	Ogre::Vector2 uv1, uv2, uv3;
	uv1.x = MapUV[0].x * tw;
	uv1.y = MapUV[0].y * th;
	uv2.x = MapUV[1].x * tw;
	uv2.y = MapUV[1].y * th;
	uv3.x = MapUV[2].x * tw;
	uv3.y = MapUV[2].y * th;

	//set up 2D matrix
	float m11, m12, m21, m22;
	m11 = uv2.x - uv1.x;
	m12 = uv3.x - uv1.x;
	m21 = uv2.y - uv1.y;
	m22 = uv3.y - uv1.y;

	//compute determinant of matrix
	float det = m11 * m22 - m12 * m21;

	if (fabsf(det) < SMALL_EPSILON)
		return sbs->ReportError("ComputeTextureMap: Bad UV coordinates");
	else
	{
		//invert matrix
		float inv_det = 1 / (m11 * m22 - m12 * m21);
		float tmp1, tmp2, tmp3, tmp4;
		tmp1 = m11;
		tmp2 = m12;
		tmp3 = m21;
		tmp4 = m22;

		m11 = tmp4 * inv_det;
		m12 = -tmp2 * inv_det;
		m21 = -tmp3 * inv_det;
		m22 = tmp1 * inv_det;
	}

	Ogre::Vector2 pl;
	Ogre::Vector3 po, pu, pv;

	// For (0,0) and Po
	Ogre::Vector2 v = Ogre::Vector2(0, 0) - uv1;
	pl = Ogre::Vector2(m11 * v.x + m12 * v.y, m21 * v.x + m22 * v.y);
	po = p1 + pl.x * (p2 - p1) + pl.y * (p3 - p1);
	po = Round(po, 7); //round result to prevent precision errors

	// For (1,0) and Pu
	v = Ogre::Vector2(1, 0) - uv1;
	pl = Ogre::Vector2(m11 * v.x + m12 * v.y, m21 * v.x + m22 * v.y);
	pu = p1 + pl.x * (p2 - p1) + pl.y * (p3 - p1);
	pu = Round(pu, 7); //round result to prevent precision errors

	// For (0,1) and Pv
	v = Ogre::Vector2(0, 1) - uv1;
	pl = Ogre::Vector2(m11 * v.x + m12 * v.y, m21 * v.x + m22 * v.y);
	pv = p1 + pl.x * (p2 - p1) + pl.y * (p3 - p1);
	pv = Round(pv, 7); //round result to prevent precision errors

	//compute norms of vectors
	Ogre::Vector3 len1 = pu - po;
	Ogre::Vector3 len2 = pv - po;
	float len1f = sqrtf(len1.x * len1.x + len1.y * len1.y + len1.z * len1.z);
	float len2f = sqrtf(len2.x * len2.x + len2.y * len2.y + len2.z * len2.z);

	return ComputeTextureSpace(t_matrix, t_vector, po, pu, len1f, pv, len2f);
}

bool TextureManager::ComputeTextureSpace(Ogre::Matrix3 &m, Ogre::Vector3 &v, const Ogre::Vector3 &v_orig, const Ogre::Vector3 &v1, float len1, const Ogre::Vector3 &v2, float len2)
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

	float d = v_orig.squaredDistance(v1);
	//get inverse square of d
	float invl1 = 1 / sqrtf(d);

	d = v_orig.squaredDistance(v2);
	//get inverse square of d
	float invl2 = (d) ? 1 / sqrtf(d) : 0;

	Ogre::Vector3 v_u = (v1 - v_orig) * len1 * invl1;
	Ogre::Vector3 v_v = (v2 - v_orig) * len2 * invl2;
	Ogre::Vector3 v_w = v_u.crossProduct(v_v);

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

	float det = m.Determinant();
	if (fabsf(det) < SMALL_EPSILON)
	{
		//m = m.IDENTITY;
		//return sbs->ReportError("Error computing texture space");

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
	else
		m = m.Inverse(); //standard inversion

	return true;
}

int SBS::Classify(int axis, std::vector<Ogre::Vector3> &vertices, float value)
{
	//from Crystal Space libs/csgeom/poly3d.cpp
	//axis is 0 for X, 1 for Y, 2 for Z

	//return codes:
	//0 - polygon is on same plane
	//1 - polygon is in front of plane
	//2 - polygon is in back of plane
	//3 - polygon intersects with plane

	int i;
	int front = 0, back = 0;

	for (i = 0; i < (int)vertices.size(); i++)
	{
		float loc = 0;
		if (axis == 0)
			loc = vertices[i].x - value;
		if (axis == 1)
			loc = vertices[i].y - value;
		if (axis == 2)
			loc = vertices[i].z - value;

		if (loc < -EPSILON)
			front++;
		else if (loc > EPSILON)
			back++;
	}

	if (back == 0 && front == 0)
		return 0; //polygon is on same plane
	if (back == 0)
		return 1; //polygon is in front of plane
	if (front == 0)
		return 2; //polygon is in back of plane
	return 3; //polygon intersects with plane
}

void SBS::SplitWithPlane(int axis, std::vector<Ogre::Vector3> &orig, std::vector<Ogre::Vector3> &poly1, std::vector<Ogre::Vector3> &poly2, float value)
{
	//from Crystal Space libs/csgeom/poly3d.cpp
	//axis is 0 for X, 1 for Y, 2 for Z
	//splits the "orig" polygon on the desired plane into two resulting polygons

	poly1.clear();
	poly2.clear();

	//preallocate memory for a worst-case scenario
	poly1.reserve(orig.size());
	poly2.reserve(orig.size());

	Ogre::Vector3 ptB;
	float sideA = 0, sideB = 0;
	Ogre::Vector3 ptA = orig[orig.size() - 1];

	if (axis == 0)
		sideA = ptA.x - value;
	if (axis == 1)
		sideA = ptA.y - value;
	if (axis == 2)
		sideA = ptA.z - value;

	if (fabsf(sideA) < SMALL_EPSILON)
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

		if (fabsf(sideB) < SMALL_EPSILON)
			sideB = 0;

		if (sideB > 0)
		{
			if (sideA < 0)
			{
				// Compute the intersection point of the line
				// from point A to point B with the partition
				// plane. This is a simple ray-plane intersection.

				Ogre::Vector3 v = ptB;
				v -= ptA;

				float sect = 0;
				if (axis == 0)
					sect = -(ptA.x - value) / v.x;
				if (axis == 1)
					sect = -(ptA.y - value) / v.y;
				if (axis == 2)
					sect = -(ptA.z - value) / v.z;
				v *= sect;
				v += ptA;
				poly1.push_back(v);
				poly2.push_back(v);
			}

			poly2.push_back(ptB);
		}
		else if (sideB < 0)
		{
			if (sideA > 0)
			{
				// Compute the intersection point of the line
				// from point A to point B with the partition
				// plane. This is a simple ray-plane intersection.

				Ogre::Vector3 v = ptB;
				v -= ptA;

				float sect = 0;
				if (axis == 0)
					sect = -(ptA.x - value) / v.x;
				if (axis == 1)
					sect = -(ptA.y - value) / v.y;
				if (axis == 2)
					sect = -(ptA.z - value) / v.z;
				v *= sect;
				v += ptA;
				poly1.push_back(v);
				poly2.push_back(v);
			}

			poly1.push_back(ptB);
		}
		else
		{
			poly1.push_back(ptB);
			poly2.push_back(ptB);
		}

		ptA = ptB;
		sideA = sideB;
	}
}

Ogre::Vector3 SBS::ComputeNormal(std::vector<Ogre::Vector3> &vertices, float &D)
{
	//from Crystal Space libs/csgeom/poly3d.cpp
	//calculate polygon normal

	float ayz = 0;
	float azx = 0;
	float axy = 0;
	size_t i, i1;
	float x1, y1, z1, x, y, z;

	i1 = vertices.size() - 1;
	x1 = vertices[i1].x;
	y1 = vertices[i1].y;
	z1 = vertices[i1].z;
	for (i = 0; i < vertices.size(); i++)
	{
		x = vertices[i].x;
		y = vertices[i].y;
		z = vertices[i].z;
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
		invd = 1.0f / sqrtf(sqd);
	Ogre::Vector3 norm;
	norm.x = ayz * invd;
	norm.y = azx * invd;
	norm.z = axy * invd;
	D = -norm.x * vertices[0].x - norm.y * vertices[0].y - norm.z * vertices[0].z;
	return norm;
}

bool SBS::InPolygon(std::vector<Ogre::Vector3> &poly, const Ogre::Vector3 &v)
{
	//from Crystal Space libs/csgeom/poly3d.cpp
	//determine if the point 'v' is inside the given polygon

	int i, i1;
	i1 = (int)poly.size() - 1;
	for (i = 0; i < (int)poly.size(); i++)
	{
		if (WhichSide3D(v, poly[i1], poly[i]) < 0)
			return false;
		i1 = i;
	}

	return true;
}

int SBS::WhichSide3D(const Ogre::Vector3 &p, const Ogre::Vector3 &v1, const Ogre::Vector3 &v2)
{
	//from Crystal Space include/csgeom/math3d.h

	/**
	   * Tests which side of a plane the given 3D point is on.
	   * \return -1 if point p is left of plane '0-v1-v2',
	   *         1 if point p is right of plane '0-v1-v2',
	   *      or 0 if point p lies on plane '0-v1-v2'.
	   * Plane '0-v1-v2' is the plane passing through points <0,0,0>, v1, and v2.
	   *<p>
	   * Warning: the result of this function when 'p' is exactly on the plane
	   * 0-v1-v2 is undefined. It should return 0 but it will not often do that
	   * due to numerical inaccuracies. So you should probably test for this
	   * case separately.
	   */

	float s = p.x * (v1.y * v2.z - v1.z * v2.y) + p.y * (v1.z * v2.x - v1.x * v2.z) + p.z * (v1.x * v2.y - v1.y * v2.x);
	if (s < 0)
		return 1;
	else if (s > 0)
		return -1;
	else return 0;
}

bool Polygon::IntersectRay(std::vector<Ogre::Vector3> &vertices, const Ogre::Vector3 &start, const Ogre::Vector3 &end)
{
	//from Crystal Space plugins/mesh/thing/object/polygon.cpp

	/**
	 * Intersect object-space ray with this polygon. This function
	 * is similar to IntersectSegment except that it doesn't keep the length
	 * of the ray in account. It just tests if the ray intersects with the
	 * interior of the polygon. Note that this function also does back-face
	 * culling.
	 */

	// First we do backface culling on the polygon with respect to
	// the starting point of the beam.

	//compute plane
	float DD;
	Ogre::Vector3 norm = sbs->ComputeNormal(vertices, DD);
	norm.normalise();

	Ogre::Plane pl(norm.x, norm.y, norm.z, DD);

	float dot1 = pl.d + pl.normal.x * start.x + pl.normal.y * start.y + pl.normal.z * start.z;
	if (dot1 > 0)
		return false;

	// If this vector is perpendicular to the plane of the polygon we
	// need to catch this case here.
	float dot2 = pl.d + pl.normal.x * end.x + pl.normal.y * end.y + pl.normal.z * end.z;
	if (fabsf(dot1 - dot2) < SMALL_EPSILON)
		return false;

	// Now we generate a plane between the starting point of the ray and
	// every edge of the polygon. With the plane normal of that plane we
	// can then check if the end of the ray is on the same side for all
	// these planes.
	Ogre::Vector3 normal;
	Ogre::Vector3 relend = end;
	relend -= start;

	int i1 = (int)vertices.size() - 1;
	for (int i = 0; i < (int)vertices.size(); i++)
	{
		Ogre::Vector3 start2 = start - vertices[i1];
		normal = start2.crossProduct(start - vertices[i]);
		if ((relend.x * normal.x + relend.y * normal.y + relend.z * normal.z > 0))
			return false;
		i1 = i;
	}

	return true;
}

bool Polygon::IntersectSegment(const Ogre::Vector3 &start, const Ogre::Vector3 &end, Ogre::Vector3 &isect, float *pr, Ogre::Vector3 &normal, bool convert, bool rescale)
{
	//from Crystal Space plugins/mesh/thing/object/polygon.cpp

	/**
	 * Intersect object-space segment with this polygon. Return
	 * true if it intersects and the intersection point in world coordinates.
	 */

	std::vector<std::vector<Ogre::Vector3> > vertices;
	GetGeometry(vertices, false, convert, rescale, false, true);

	for (int i = 0; i < (int)vertices.size(); i++)
	{
		if (!IntersectRay(vertices[i], start, end))
			continue;

		if (IntersectSegmentPlane(vertices[i], start, end, isect, pr, normal))
			return true;
	}
	return false;
}

bool Polygon::IntersectSegmentPlane(std::vector<Ogre::Vector3> &vertices, const Ogre::Vector3 &start, const Ogre::Vector3 &end, Ogre::Vector3 &isect, float *pr, Ogre::Vector3 &normal)
{
	//from Crystal Space plugins/mesh/thing/object/polygon.cpp

	/**
	 * Intersect object-space segment with the plane of this polygon. Return
	 * true if it intersects and the intersection point in world coordinates.
	 */

	float x1 = start.x;
	float y1 = start.y;
	float z1 = start.z;
	float x2 = end.x;
	float y2 = end.y;
	float z2 = end.z;
	float r, num, denom;

	// So now we have the plane equation of the polygon:
	// A*x + B*y + C*z + D = 0
	//
	// We also have the parameter line equations of the ray
	// going through 'start' and 'end':
	// x = r*(x2-x1)+x1
	// y = r*(y2-y1)+y1
	// z = r*(z2-z1)+z1
	//
	// =>   A*(r*(x2-x1)+x1) + B*(r*(y2-y1)+y1) + C*(r*(z2-z1)+z1) + D = 0
	// Set *pr to -1 to indicate error if we return false now.
	if (pr) *pr = -1;

	//compute plane
	float DD;
	Ogre::Vector3 norm = sbs->ComputeNormal(vertices, DD);
	norm.normalise();

	Ogre::Plane plane(norm.x, norm.y, norm.z, DD);

	denom = plane.normal.x * (x2 - x1) +
			plane.normal.y * (y2 - y1) +
			plane.normal.z * (z2 - z1);

	if (fabsf(denom) < SMALL_EPSILON)
		return false;  // Lines are parallel

	num = -(plane.normal.x * x1 +
			plane.normal.y * y1 +
			plane.normal.z * z1 +
			plane.d);
	r = num / denom;

	// Calculate 'r' and 'isect' even if the intersection point is
	// not on the segment. That way we can use this function for testing
	// with rays as well.
	if (pr)
		*pr = r;

	isect.x = r * (x2 - x1) + x1;
	isect.y = r * (y2 - y1) + y1;
	isect.z = r * (z2 - z1) + z1;

	// If r is not in [0,1] the intersection point is not on the segment.
	if (r < 0 /*-SMALL_EPSILON*/ || r > 1)
		return false;

	normal = norm;
	return true;
}

bool SBS::SegmentPlane(const Ogre::Vector3 &u, const Ogre::Vector3 &v, Ogre::Plane &plane, Ogre::Vector3 &isect, float &dist)
{
	//from Crystal Space libs/csgeom/math3d.cpp

	/**
	* Intersect a 3D segment with a plane.  Returns true if there is an
	* intersection, with the intersection point returned in isect.
	* The distance from u to the intersection point is returned in dist.
	* The distance that is returned is a normalized distance with respect
	* to the given input vector. i.e. a distance of 0.5 means that the
	* intersection point is halfway u and v.
	* There are two cases in which this method will return false:
	* - If the plane and the segment are parallel, then 'dist' will be set
	*   equal to 0, and 'isect' equal to 'v'.
	* - If the segment does not cross the plane (i.e. if 'dist'>1+epsilon or
	*   'dist'<-epsilon, where epsilon is a very small value near to zero)
	*   then 'isect's value is (0, 0, 0).
	*
	* \remarks
	* 'p' is the plane, expressed as: A x + B y + C z + D = 0 , where (A,B,C) is
	* the normal vector of the plane.
	* 'u' and 'v' are the start (U point) and the end (V point) of the segment.
	* 'isect' is searched along the segment U + x (V - U); the unknown 'x' value
	* is got by: x = [(A,B,C) * U + D ] / (A,B,C) * (U - V), where * is the dot
	* product.
	*/

	float denom;
	Ogre::Vector3 uv = u - v;

	denom = plane.normal.dotProduct(uv);
	if (denom == 0)
	{
		// they are parallel
		dist = 0; //'dist' is an output arg, so it must be initialized.
		isect = v;
		return false;
	}
	dist = (plane.normal.dotProduct(u) + plane.d) / denom;
	if (dist < -SMALL_EPSILON || dist > 1 + SMALL_EPSILON)
	{
		isect = 0;//'isect' is an output arg, so it must be initialized.
		return false;
	}

	isect = u + dist * -uv;
	return true;
}

}
