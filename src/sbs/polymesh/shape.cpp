/*
	Scalable Building Simulator - Shape Object
	The Skyscraper Project - Version 2.1
	Copyright (C)2004-2025 Ryan Thoryk
	https://www.skyscrapersim.net
	https://sourceforge.net/projects/skyscraper/
	Contact - ryan@skyscrapersim.net

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
#include "polymesh.h"
#include "wall.h"
#include "shape.h"

namespace SBS {

Shape::Shape(Wall* parent) : ObjectBase(parent)
{
	this->parent = parent;
	origin = Vector3::ZERO;
}

Shape::~Shape()
{

}

void Shape::CreateSphere(const std::string &name, const std::string &texture, const Vector3 &Center, Real radius, int latSteps, int lonSteps, Real tw, Real th, bool autosize)
{
	//generates a sphere

	if (!parent)
		return;

	if (latSteps < 2 || lonSteps < 3)
	{
		parent->ReportError("Sphere requires at least 2 latitude steps and 3 longitude steps");
		return;
	}

	PolygonSet result;
	std::vector<std::vector<Vector2>> uvMapSet;
	std::vector<Triangle> triangles;

	int index = 0;
	for (int lat = 0; lat < latSteps; ++lat)
	{
		float v1 = float(lat) / latSteps;
		float v2 = float(lat + 1) / latSteps;

		float theta1 = pi * (v1 - 0.5f);
		float theta2 = pi * (v2 - 0.5f);

		for (int lon = 0; lon < lonSteps; ++lon)
		{
			float u1 = float(lon) / lonSteps;
			float u2 = float(lon + 1) / lonSteps;

			float phi1 = 2.0f * pi * u1;
			float phi2 = 2.0f * pi * u2;

			Vector3 p1 = radius * Vector3(cosf(theta1) * cosf(phi1), sinf(theta1), cosf(theta1) * sinf(phi1));
			Vector3 p2 = radius * Vector3(cosf(theta2) * cosf(phi1), sinf(theta2), cosf(theta2) * sinf(phi1));
			Vector3 p3 = radius * Vector3(cosf(theta2) * cosf(phi2), sinf(theta2), cosf(theta2) * sinf(phi2));
			Vector3 p4 = radius * Vector3(cosf(theta1) * cosf(phi2), sinf(theta1), cosf(theta1) * sinf(phi2));

			std::vector<Vector2> uvMap;
			uvMap.reserve(6);

			//first triangle
			PolyArray poly1;
			Triangle tri1;
			poly1.reserve(3);
			poly1.emplace_back(p1 + Center + origin);
			uvMap.emplace_back(Vector2(u1, v1));
			tri1.a = 0 + index;

			poly1.emplace_back(p2 + Center + origin);
			uvMap.emplace_back(Vector2(u1, v2));
			tri1.b = 1 + index;

			poly1.emplace_back(p3 + Center + origin);
			uvMap.emplace_back(Vector2(u2, v2));
			tri1.c = 2 + index;

			result.emplace_back(poly1);
			uvMapSet.emplace_back(uvMap);
			triangles.emplace_back(tri1);

			//second triangle
			PolyArray poly2;
			Triangle tri2;
			uvMap.clear();
			poly2.reserve(3);
			poly2.emplace_back(p1 + Center + origin);
			uvMap.emplace_back(Vector2(u1, v1));
			tri2.a = 3 + index;

			poly2.emplace_back(p3 + Center + origin);
			uvMap.emplace_back(Vector2(u2, v2));
			tri2.b = 4 + index;

			poly2.emplace_back(p4 + Center + origin);
			uvMap.emplace_back(Vector2(u2, v1));
			tri2.c = 5 + index;

			result.emplace_back(poly2);
			uvMapSet.emplace_back(uvMap);
			triangles.emplace_back(tri2);
			index += 6;
		}
	}

	//create polygon from the generated sphere segments
	parent->AddPolygon(name, texture, result, uvMapSet, triangles, tw, th, autosize);
}

void Shape::CreateBox(const std::string &name, const std::string &texture, Ogre::Vector3 &center, Real width, Real height, Real depth, Real tw, Real th, bool autosize)
{
	//generates a box

	if (!parent)
		return;

	if (width <= 0 || height <= 0 || depth <= 0)
	{
		parent->ReportError("Box dimensions must be greater than zero");
		return;
	}

	Real hw = width * 0.5;
	Real hh = height * 0.5;
	Real hd = depth * 0.5;

	// 8 corners of the box
	Vector3 p000(-hw, -hh, -hd); // left bottom back
	Vector3 p001(-hw, -hh,  hd); // left bottom front
	Vector3 p010(-hw,  hh, -hd); // left top back
	Vector3 p011(-hw,  hh,  hd); // left top front
	Vector3 p100( hw, -hh, -hd); // right bottom back
	Vector3 p101( hw, -hh,  hd); // right bottom front
	Vector3 p110( hw,  hh, -hd); // right top back
	Vector3 p111( hw,  hh,  hd); // right top front

	auto addQuad = [&](const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d,
			const Vector2& uvA, const Vector2& uvB, const Vector2& uvC, const Vector2& uvD)
	{
		PolygonSet result;
		std::vector<std::vector<Vector2>> uvMapSet;

		std::vector<Vector2> uvMap;
		uvMap.reserve(2);
		std::vector<Triangle> triangles;
		triangles.reserve(2);

		PolyArray poly1;
		Triangle tri1;
		poly1.emplace_back(a + center + origin);
		uvMap.emplace_back(uvA);
		tri1.a = 2;

		poly1.emplace_back(b + center + origin);
		uvMap.emplace_back(uvB);
		tri1.b = 1;

		poly1.emplace_back(c + center + origin);
		uvMap.emplace_back(uvC);
		tri1.c = 0;

		result.emplace_back(poly1);
		uvMapSet.emplace_back(uvMap);
		triangles.emplace_back(tri1);

		uvMap.clear();
		PolyArray poly2;
		Triangle tri2;
		poly2.emplace_back(a + center + origin);
		uvMap.emplace_back(uvA);
		tri2.a = 5;

		poly2.emplace_back(c + center + origin);
		uvMap.emplace_back(uvC);
		tri2.b = 4;

		poly2.emplace_back(d + center + origin);
		uvMap.emplace_back(uvD);
		tri2.c = 3;

		result.emplace_back(poly2);
		uvMapSet.emplace_back(uvMap);
		triangles.emplace_back(tri2);

		//create polygon from the generated triangles
		parent->AddPolygon(name, texture, result, uvMapSet, triangles, tw, th, autosize);
	};

	//back face (+Z)
	addQuad(p101, p001, p011, p111, {1, 0}, {0, 0}, {0, 1}, {1, 1});

	//front face (-Z)
	addQuad(p100, p110, p010, p000, {1, 0}, {1, 1}, {0, 1}, {0, 0});

	//left face (-X)
	addQuad(p001, p000, p010, p011, {1, 0}, {0, 0}, {0, 1}, {1, 1});

	//right face (+X)
	addQuad(p100, p101, p111, p110, {0, 0}, {1, 0}, {1, 1}, {0, 1});

	//top face (+Y)
	addQuad(p011, p010, p110, p111, {0, 1}, {0, 0}, {1, 0}, {1, 1});

	//bottom face (-Y)
	addQuad(p100, p000, p001, p101, {1, 1}, {0, 1}, {0, 0}, {1, 0});
}

void Shape::CreateCylinder(const std::string &name, const std::string &texture, const Vector3 &Center, Real radius, Real height, int slices, Real tw, Real th, bool autosize)
{
	//generates a cylinder

	if (!parent)
		return;

	if (slices < 3)
	{
		parent->ReportError("Cylinder requires at least 3 slices");
		return;
	}

	PolygonSet result;
	std::vector<std::vector<Vector2>> uvMapSet;
	std::vector<Triangle> triangles;

	const Vector3 topCenter = Center + Vector3(0, height * 0.5f, 0) + origin;
	const Vector3 bottomCenter = Center - Vector3(0, height * 0.5f, 0) + origin;

	int index = 0;

	//side surface
	for (int i = 0; i < slices; ++i)
	{
		float u1 = float(i) / slices;
		float u2 = float(i + 1) / slices;

		float theta1 = u1 * 2.0f * pi;
		float theta2 = u2 * 2.0f * pi;

		Vector3 p1 = Vector3(cosf(theta1) * radius, -height * 0.5f, sinf(theta1) * radius);
		Vector3 p2 = Vector3(cosf(theta2) * radius, -height * 0.5f, sinf(theta2) * radius);
		Vector3 p3 = Vector3(cosf(theta2) * radius, height * 0.5f, sinf(theta2) * radius);
		Vector3 p4 = Vector3(cosf(theta1) * radius, height * 0.5f, sinf(theta1) * radius);

		//first triangle (p1, p2, p3)
		PolyArray poly1 = {
			p1 + Center + origin,
			p2 + Center + origin,
			p3 + Center + origin
		};
		std::vector<Vector2> uv1 = {
			{u1 * tw, 0},
			{u2 * tw, 0},
			{u2 * tw, th}
		};
		result.emplace_back(poly1);
		uvMapSet.emplace_back(uv1);
		triangles.push_back(Triangle(index + 0, index + 1, index + 2 ));

		//second triangle (p1, p3, p4)
		PolyArray poly2 = {
			p1 + Center + origin,
			p3 + Center + origin,
			p4 + Center + origin
		};
		std::vector<Vector2> uv2 = {
			{u1 * tw, 0},
			{u2 * tw, th},
			{u1 * tw, th}
		};
		result.emplace_back(poly2);
		uvMapSet.emplace_back(uv2);
		triangles.push_back(Triangle(index + 3, index + 4, index + 5));

		index += 6;
	}

	//bottom cap
	for (int i = 0; i < slices; ++i)
	{
		float theta1 = 2.0f * pi * float(i) / slices;
		float theta2 = 2.0f * pi * float(i + 1) / slices;

		Vector3 p1 = Vector3(cosf(theta1) * radius, -height * 0.5f, sinf(theta1) * radius);
		Vector3 p2 = Vector3(cosf(theta2) * radius, -height * 0.5f, sinf(theta2) * radius);

		PolyArray poly;
		poly.emplace_back(bottomCenter);
		poly.emplace_back(p1 + Center + origin);
		poly.emplace_back(p2 + Center + origin);

		std::vector<Vector2> uv = {
			{0.5f * tw, 0.5f * th},
			{(cosf(theta1) * 0.5f + 0.5f) * tw, (sinf(theta1) * 0.5f + 0.5f) * th},
			{(cosf(theta2) * 0.5f + 0.5f) * tw, (sinf(theta2) * 0.5f + 0.5f) * th}
		};

		result.emplace_back(poly);
		uvMapSet.emplace_back(uv);
		triangles.push_back(Triangle(index + 0, index + 1, index + 2));
		index += 3;
	}

	//top cap
	for (int i = 0; i < slices; ++i)
	{
		float theta1 = 2.0f * pi * float(i) / slices;
		float theta2 = 2.0f * pi * float(i + 1) / slices;

		Vector3 p1 = Vector3(cosf(theta1) * radius, height * 0.5f, sinf(theta1) * radius);
		Vector3 p2 = Vector3(cosf(theta2) * radius, height * 0.5f, sinf(theta2) * radius);

		PolyArray poly;
		poly.emplace_back(topCenter);
		poly.emplace_back(p2 + Center + origin);
		poly.emplace_back(p1 + Center + origin);

		std::vector<Vector2> uv = {
			{0.5f * tw, 0.5f * th},
			{(cosf(theta2) * 0.5f + 0.5f) * tw, (sinf(theta2) * 0.5f + 0.5f) * th},
			{(cosf(theta1) * 0.5f + 0.5f) * tw, (sinf(theta1) * 0.5f + 0.5f) * th}
		};

		result.emplace_back(poly);
		uvMapSet.emplace_back(uv);
		triangles.push_back(Triangle(index + 0, index + 1, index + 2));
		index += 3;
	}

	//create polygon object from the generated cylinder segments
	parent->AddPolygon(name, texture, result, uvMapSet, triangles, tw, th, autosize);
}

void Shape::CreateCone(const std::string &name, const std::string &texture, const Vector3 &Center, Real radius, Real height, int slices, Real tw, Real th, bool autosize)
{
	//generates a cone

	if (!parent)
		return;

	if (slices < 3)
	{
		parent->ReportError("Cone requires at least 3 slices");
		return;
	}

	PolygonSet result;
	std::vector<std::vector<Vector2>> uvMapSet;
	std::vector<Triangle> triangles;

	const Vector3 tip = Center + Vector3(0, height * 0.5f, 0) + origin;
	const Vector3 baseCenter = Center - Vector3(0, height * 0.5f, 0) + origin;

	int index = 0;

	//side surface (triangle fan from tip to base circle)
	for (int i = 0; i < slices; ++i)
	{
		float u1 = float(i) / slices;
		float u2 = float(i + 1) / slices;

		float theta1 = u1 * 2.0f * pi;
		float theta2 = u2 * 2.0f * pi;

		Vector3 p1 = Vector3(cosf(theta1) * radius, -height * 0.5f, sinf(theta1) * radius);
		Vector3 p2 = Vector3(cosf(theta2) * radius, -height * 0.5f, sinf(theta2) * radius);

		//triangle (tip, p1, p2)
		PolyArray poly = {
			tip,
			p1 + Center + origin,
			p2 + Center + origin
		};
		std::vector<Vector2> uv = {
			{0.5f * tw, 0},
			{u1 * tw, th},
			{u2 * tw, th}
		};

		result.emplace_back(poly);
		uvMapSet.emplace_back(uv);
		triangles.push_back(Triangle(index + 0, index + 1, index + 2));
		index += 3;
	}

	//bottom cap (triangle fan centered on base)
	for (int i = 0; i < slices; ++i)
	{
		float theta1 = 2.0f * pi * float(i) / slices;
		float theta2 = 2.0f * pi * float(i + 1) / slices;

		Vector3 p1 = Vector3(cosf(theta1) * radius, -height * 0.5f, sinf(theta1) * radius);
		Vector3 p2 = Vector3(cosf(theta2) * radius, -height * 0.5f, sinf(theta2) * radius);

		PolyArray poly = {
			baseCenter,
			p1 + Center + origin,
			p2 + Center + origin
		};
		std::vector<Vector2> uv = {
			{0.5f * tw, 0.5f * th},
			{(cosf(theta1) * 0.5f + 0.5f) * tw, (sinf(theta1) * 0.5f + 0.5f) * th},
			{(cosf(theta2) * 0.5f + 0.5f) * tw, (sinf(theta2) * 0.5f + 0.5f) * th}
		};

		result.emplace_back(poly);
		uvMapSet.emplace_back(uv);
		triangles.push_back(Triangle(index + 0, index + 1, index + 2));
		index += 3;
	}

	//create polygon object from the generated cone segments
	parent->AddPolygon(name, texture, result, uvMapSet, triangles, tw, th, autosize);
}

void Shape::CreateCapsule(const std::string &name, const std::string &texture, const Vector3 &Center, Real radius, Real height, int latSteps, int lonSteps, Real tw, Real th, bool autosize)
{
	//generates a capsule (cylinder with hemispherical ends)

	if (!parent)
		return;

	if (latSteps < 2 || lonSteps < 3)
	{
		parent->ReportError("Capsule requires at least 2 latitude steps and 3 longitude steps.");
		return;
	}

	PolygonSet result;
	std::vector<std::vector<Vector2>> uvMapSet;
	std::vector<Triangle> triangles;

	Real bodyHeight = height - 2.0f * radius;
	if (bodyHeight < 0.0f)
		bodyHeight = 0.0f;

	Vector3 baseCenter = Center - Vector3(0, bodyHeight * 0.5f, 0);
	Vector3 topCenter = Center + Vector3(0, bodyHeight * 0.5f, 0);

	int index = 0;

	//side cylinder
	for (int i = 0; i < lonSteps; ++i)
	{
		float u1 = float(i) / lonSteps;
		float u2 = float(i + 1) / lonSteps;

		float theta1 = u1 * 2.0f * pi;
		float theta2 = u2 * 2.0f * pi;

		Vector3 p1 = Vector3(cosf(theta1) * radius, -bodyHeight * 0.5f, sinf(theta1) * radius);
		Vector3 p2 = Vector3(cosf(theta2) * radius, -bodyHeight * 0.5f, sinf(theta2) * radius);
		Vector3 p3 = Vector3(cosf(theta2) * radius, bodyHeight * 0.5f, sinf(theta2) * radius);
		Vector3 p4 = Vector3(cosf(theta1) * radius, bodyHeight * 0.5f, sinf(theta1) * radius);

		//quad split into two triangles
		PolyArray poly1 = { p1 + Center + origin, p2 + Center + origin, p3 + Center + origin };
		std::vector<Vector2> uv1 = { {u1 * tw, 0}, {u2 * tw, 0}, {u2 * tw, th} };
		result.emplace_back(poly1); uvMapSet.emplace_back(uv1);
		triangles.push_back(Triangle(index + 0, index + 1, index + 2));

		PolyArray poly2 = { p1 + Center + origin, p3 + Center + origin, p4 + Center + origin };
		std::vector<Vector2> uv2 = { {u1 * tw, 0}, {u2 * tw, th}, {u1 * tw, th} };
		result.emplace_back(poly2); uvMapSet.emplace_back(uv2);
		triangles.push_back(Triangle(index + 3, index + 4, index + 5));

		index += 6;
	}

	//top hemisphere
	for (int lat = 0; lat < latSteps / 2; ++lat)
	{
		float v1 = float(lat) / latSteps;
		float v2 = float(lat + 1) / latSteps;
		float theta1 = pi * 0.5f * v1;
		float theta2 = pi * 0.5f * v2;

		for (int lon = 0; lon < lonSteps; ++lon)
		{
			float u1 = float(lon) / lonSteps;
			float u2 = float(lon + 1) / lonSteps;

			float phi1 = 2.0f * pi * u1;
			float phi2 = 2.0f * pi * u2;

			Vector3 p1 = radius * Vector3(cosf(theta1) * cosf(phi1), sinf(theta1), cosf(theta1) * sinf(phi1));
			Vector3 p2 = radius * Vector3(cosf(theta2) * cosf(phi1), sinf(theta2), cosf(theta2) * sinf(phi1));
			Vector3 p3 = radius * Vector3(cosf(theta2) * cosf(phi2), sinf(theta2), cosf(theta2) * sinf(phi2));
			Vector3 p4 = radius * Vector3(cosf(theta1) * cosf(phi2), sinf(theta1), cosf(theta1) * sinf(phi2));

			p1 += topCenter + origin;
			p2 += topCenter + origin;
			p3 += topCenter + origin;
			p4 += topCenter + origin;

			//first triangle
			PolyArray poly1 = { p1, p2, p3 };
			std::vector<Vector2> uv1 = { {u1 * tw, v1 * th}, {u1 * tw, v2 * th}, {u2 * tw, v2 * th} };
			result.emplace_back(poly1); uvMapSet.emplace_back(uv1);
			triangles.push_back(Triangle(index + 0, index + 1, index + 2));

			//second triangle
			PolyArray poly2 = { p1, p3, p4 };
			std::vector<Vector2> uv2 = { {u1 * tw, v1 * th}, {u2 * tw, v2 * th}, {u2 * tw, v1 * th} };
			result.emplace_back(poly2); uvMapSet.emplace_back(uv2);
			triangles.push_back(Triangle(index + 3, index + 4, index + 5));

			index += 6;
		}
	}

	//bottom hemisphere
	for (int lat = 0; lat < latSteps / 2; ++lat)
	{
		float v1 = float(lat) / latSteps;
		float v2 = float(lat + 1) / latSteps;
		float theta1 = -pi * 0.5f * v1;
		float theta2 = -pi * 0.5f * v2;

		for (int lon = 0; lon < lonSteps; ++lon)
		{
			float u1 = float(lon) / lonSteps;
			float u2 = float(lon + 1) / lonSteps;

			float phi1 = 2.0f * pi * u1;
			float phi2 = 2.0f * pi * u2;

			Vector3 p1 = radius * Vector3(cosf(theta1) * cosf(phi1), sinf(theta1), cosf(theta1) * sinf(phi1));
			Vector3 p2 = radius * Vector3(cosf(theta2) * cosf(phi1), sinf(theta2), cosf(theta2) * sinf(phi1));
			Vector3 p3 = radius * Vector3(cosf(theta2) * cosf(phi2), sinf(theta2), cosf(theta2) * sinf(phi2));
			Vector3 p4 = radius * Vector3(cosf(theta1) * cosf(phi2), sinf(theta1), cosf(theta1) * sinf(phi2));

			p1 += baseCenter + origin;
			p2 += baseCenter + origin;
			p3 += baseCenter + origin;
			p4 += baseCenter + origin;

			//first triangle
			PolyArray poly1 = { p1, p2, p3 };
			std::vector<Vector2> uv1 = { {u1 * tw, v1 * th}, {u1 * tw, v2 * th}, {u2 * tw, v2 * th} };
			result.emplace_back(poly1); uvMapSet.emplace_back(uv1);
			triangles.push_back(Triangle(index + 0, index + 1, index + 2));

			//second triangle
			PolyArray poly2 = { p1, p3, p4 };
			std::vector<Vector2> uv2 = { {u1 * tw, v1 * th}, {u2 * tw, v2 * th}, {u2 * tw, v1 * th} };
			result.emplace_back(poly2); uvMapSet.emplace_back(uv2);
			triangles.push_back(Triangle(index + 3, index + 4, index + 5));

			index += 6;
		}
	}

	//create polygon object from the generated capsule segments
	parent->AddPolygon(name, texture, result, uvMapSet, triangles, tw, th, autosize);
}

void Shape::CreatePlane(const std::string &name, const std::string &texture, const Vector3 &Center, Real width, Real depth, int xSegments, int zSegments, Real tw, Real th, bool autosize)
{
	if (!parent)
		return;

	if (xSegments < 1 || zSegments < 1)
	{
		parent->ReportError("Plane requires at least 1 segment in each direction");
		return;
	}

	PolygonSet result;
	std::vector<std::vector<Vector2>> uvMapSet;
	std::vector<Triangle> triangles;

	Vector3 originOffset = origin + Center;

	Real dx = width / xSegments;
	Real dz = depth / zSegments;

	int index = 0;

	for (int z = 0; z < zSegments; ++z)
	{
		for (int x = 0; x < xSegments; ++x)
		{
			//corner positions
			Vector3 p1 = Vector3((x + 0) * dx - width * 0.5f, 0, (z + 0) * dz - depth * 0.5f) + originOffset;
			Vector3 p2 = Vector3((x + 1) * dx - width * 0.5f, 0, (z + 0) * dz - depth * 0.5f) + originOffset;
			Vector3 p3 = Vector3((x + 1) * dx - width * 0.5f, 0, (z + 1) * dz - depth * 0.5f) + originOffset;
			Vector3 p4 = Vector3((x + 0) * dx - width * 0.5f, 0, (z + 1) * dz - depth * 0.5f) + originOffset;

			//UVs
			Real u1 = (Real)x / xSegments;
			Real u2 = (Real)(x + 1) / xSegments;
			Real v1 = (Real)z / zSegments;
			Real v2 = (Real)(z + 1) / zSegments;

			//triangle 1
			PolyArray poly1 = { p1, p2, p3 };
			std::vector<Vector2> uv1 = { {u1 * tw, v1 * th}, {u2 * tw, v1 * th}, {u2 * tw, v2 * th} };
			result.emplace_back(poly1);
			uvMapSet.emplace_back(uv1);
			triangles.emplace_back(Triangle(index + 0, index + 1, index + 2));

			//triangle 2
			PolyArray poly2 = { p1, p3, p4 };
			std::vector<Vector2> uv2 = { {u1 * tw, v1 * th}, {u2 * tw, v2 * th}, {u1 * tw, v2 * th} };
			result.emplace_back(poly2);
			uvMapSet.emplace_back(uv2);
			triangles.emplace_back(Triangle(index + 3, index + 4, index + 5));

			index += 6;
		}
	}

	//create polygon object from the generated plane segments
	parent->AddPolygon(name, texture, result, uvMapSet, triangles, tw, th, autosize);
}

void Shape::CreateCircle(const std::string &name, const std::string &texture, const Vector3 &Center, Real radius, int slices, Real tw, Real th, bool autosize)
{
	//generates a circle (disc)

	if (!parent)
		return;

	if (slices < 3)
	{
		parent->ReportError("Circle requires at least 3 slices");
		return;
	}

	PolygonSet result;
	std::vector<std::vector<Vector2>> uvMapSet;
	std::vector<Triangle> triangles;

	Vector3 centerPos = Center + origin;

	int index = 0;

	for (int i = 0; i < slices; ++i)
	{
		float theta1 = (2.0f * pi * i) / slices;
		float theta2 = (2.0f * pi * (i + 1)) / slices;

		Vector3 p1 = Vector3(cosf(theta1) * radius, 0, sinf(theta1) * radius);
		Vector3 p2 = Vector3(cosf(theta2) * radius, 0, sinf(theta2) * radius);

		PolyArray poly;
		poly.emplace_back(centerPos); // center of the circle
		poly.emplace_back(centerPos + p1);
		poly.emplace_back(centerPos + p2);

		std::vector<Vector2> uv = {
			{0.5f * tw, 0.5f * th},
			{(cosf(theta1) * 0.5f + 0.5f) * tw, (sinf(theta1) * 0.5f + 0.5f) * th},
			{(cosf(theta2) * 0.5f + 0.5f) * tw, (sinf(theta2) * 0.5f + 0.5f) * th}
		};

		result.emplace_back(poly);
		uvMapSet.emplace_back(uv);
		triangles.emplace_back(Triangle(index + 0, index + 1, index + 2));
		index += 3;
	}

	//create polygon object from the generated circle segments
	parent->AddPolygon(name, texture, result, uvMapSet, triangles, tw, th, autosize);
}

void Shape::CreateTorus(const std::string &name, const std::string &texture, const Vector3 &Center, Real majorRadius, Real minorRadius, int radialSteps, int tubeSteps, Real tw, Real th, bool autosize)
{
	//generates a torus

	if (!parent)
		return;

	if (radialSteps < 3 || tubeSteps < 3)
	{
		parent->ReportError("Torus requires at least 3 radial steps and 3 tube steps");
		return;
	}

	PolygonSet result;
	std::vector<std::vector<Vector2>> uvMapSet;
	std::vector<Triangle> triangles;

	Vector3 originOffset = Center + origin;
	int index = 0;

	for (int i = 0; i < radialSteps; ++i)
	{
		float u1 = float(i) / radialSteps;
		float u2 = float(i + 1) / radialSteps;
		float theta1 = u1 * 2.0f * pi;
		float theta2 = u2 * 2.0f * pi;

		Vector3 center1 = Vector3(cosf(theta1), 0, sinf(theta1)) * majorRadius;
		Vector3 center2 = Vector3(cosf(theta2), 0, sinf(theta2)) * majorRadius;

		for (int j = 0; j < tubeSteps; ++j)
		{
			float v1 = float(j) / tubeSteps;
			float v2 = float(j + 1) / tubeSteps;
			float phi1 = v1 * 2.0f * pi;
			float phi2 = v2 * 2.0f * pi;

			Vector3 offset1 = Vector3(cosf(theta1) * cosf(phi1), sinf(phi1), sinf(theta1) * cosf(phi1)) * minorRadius;
			Vector3 offset2 = Vector3(cosf(theta2) * cosf(phi1), sinf(phi1), sinf(theta2) * cosf(phi1)) * minorRadius;
			Vector3 offset3 = Vector3(cosf(theta2) * cosf(phi2), sinf(phi2), sinf(theta2) * cosf(phi2)) * minorRadius;
			Vector3 offset4 = Vector3(cosf(theta1) * cosf(phi2), sinf(phi2), sinf(theta1) * cosf(phi2)) * minorRadius;

			Vector3 p1 = center1 + offset1 + originOffset;
			Vector3 p2 = center2 + offset2 + originOffset;
			Vector3 p3 = center2 + offset3 + originOffset;
			Vector3 p4 = center1 + offset4 + originOffset;

			//triangle 1
			PolyArray poly1 = { p1, p2, p3 };
			std::vector<Vector2> uv1 = {
				{u1 * tw, v1 * th},
				{u2 * tw, v1 * th},
				{u2 * tw, v2 * th}
			};
			result.emplace_back(poly1);
			uvMapSet.emplace_back(uv1);
			triangles.emplace_back(Triangle(index + 0, index + 1, index + 2));

			//triangle 2
			PolyArray poly2 = { p1, p3, p4 };
			std::vector<Vector2> uv2 = {
				{u1 * tw, v1 * th},
				{u2 * tw, v2 * th},
				{u1 * tw, v2 * th}
			};
			result.emplace_back(poly2);
			uvMapSet.emplace_back(uv2);
			triangles.emplace_back(Triangle(index + 3, index + 4, index + 5));

			index += 6;
		}
	}

	//create polygon object from the generated torus segments
	parent->AddPolygon(name, texture, result, uvMapSet, triangles, tw, th, autosize);
}

void Shape::CreateDome(const std::string &name, const std::string &texture, const Vector3 &Center, Real radius, int latSteps, int lonSteps, Real tw, Real th, bool autosize)
{
	//generates a dome (half-sphere)

	if (!parent)
		return;

	if (latSteps < 2 || lonSteps < 3)
	{
		parent->ReportError("Dome requires at least 2 latitude steps and 3 longitude steps");
		return;
	}

	PolygonSet result;
	std::vector<std::vector<Vector2>> uvMapSet;
	std::vector<Triangle> triangles;

	int index = 0;
	for (int lat = 0; lat < latSteps / 2; ++lat) // top half only
	{
		float v1 = float(lat) / latSteps;
		float v2 = float(lat + 1) / latSteps;

		float theta1 = pi * (v1 - 0.5f);
		float theta2 = pi * (v2 - 0.5f);

		for (int lon = 0; lon < lonSteps; ++lon)
		{
			float u1 = float(lon) / lonSteps;
			float u2 = float(lon + 1) / lonSteps;

			float phi1 = 2.0f * pi * u1;
			float phi2 = 2.0f * pi * u2;

			Vector3 p1 = radius * Vector3(cosf(theta1) * cosf(phi1), sinf(theta1), cosf(theta1) * sinf(phi1));
			Vector3 p2 = radius * Vector3(cosf(theta2) * cosf(phi1), sinf(theta2), cosf(theta2) * sinf(phi1));
			Vector3 p3 = radius * Vector3(cosf(theta2) * cosf(phi2), sinf(theta2), cosf(theta2) * sinf(phi2));
			Vector3 p4 = radius * Vector3(cosf(theta1) * cosf(phi2), sinf(theta1), cosf(theta1) * sinf(phi2));

			std::vector<Vector2> uvMap;

			//triangle 1
			PolyArray poly1;
			poly1.emplace_back(p1 + Center + origin);
			uvMap.emplace_back(Vector2(u1 * tw, v1 * th));

			poly1.emplace_back(p2 + Center + origin);
			uvMap.emplace_back(Vector2(u1 * tw, v2 * th));

			poly1.emplace_back(p3 + Center + origin);
			uvMap.emplace_back(Vector2(u2 * tw, v2 * th));

			result.emplace_back(poly1);
			uvMapSet.emplace_back(uvMap);
			triangles.emplace_back(Triangle(index + 0, index + 1, index + 2));

			//triangle 2
			PolyArray poly2;
			uvMap.clear();
			poly2.emplace_back(p1 + Center + origin);
			uvMap.emplace_back(Vector2(u1 * tw, v1 * th));

			poly2.emplace_back(p3 + Center + origin);
			uvMap.emplace_back(Vector2(u2 * tw, v2 * th));

			poly2.emplace_back(p4 + Center + origin);
			uvMap.emplace_back(Vector2(u2 * tw, v1 * th));

			result.emplace_back(poly2);
			uvMapSet.emplace_back(uvMap);
			triangles.emplace_back(Triangle(index + 3, index + 4, index + 5));

			index += 6;
		}
	}

	//create polygon object from the generated dome segments
	parent->AddPolygon(name, texture, result, uvMapSet, triangles, tw, th, autosize);
}

void Shape::CreatePyramid(const std::string &name, const std::string &texture, const Vector3 &Center, Real width, Real depth, Real height, Real tw, Real th, bool autosize)
{
	if (!parent)
		return;

	if (width <= 0 || depth <= 0 || height <= 0)
	{
		parent->ReportError("Pyramid requires positive dimensions");
		return;
	}

	PolygonSet result;
	std::vector<std::vector<Vector2>> uvMapSet;
	std::vector<Triangle> triangles;

	Vector3 halfSize = Vector3(width * 0.5f, 0, depth * 0.5f);
	Vector3 originOffset = Center + origin;

	//base corners
	Vector3 bl = originOffset + Vector3(-halfSize.x, 0, -halfSize.z); // back-left
	Vector3 br = originOffset + Vector3(halfSize.x, 0, -halfSize.z);  // back-right
	Vector3 fr = originOffset + Vector3(halfSize.x, 0, halfSize.z);   // front-right
	Vector3 fl = originOffset + Vector3(-halfSize.x, 0, halfSize.z);  // front-left

	//tip of pyramid
	Vector3 tip = originOffset + Vector3(0, height, 0);

	int index = 0;

	//side 1: front
	{
		PolyArray poly = { tip, fr, fl };
		std::vector<Vector2> uv = { {0.5f * tw, 0}, {tw, th}, {0, th} };
		result.emplace_back(poly);
		uvMapSet.emplace_back(uv);
		triangles.push_back(Triangle(index + 0, index + 1, index + 2));
		index += 3;
	}

	//side 2: right
	{
		PolyArray poly = { tip, br, fr };
		std::vector<Vector2> uv = { {0.5f * tw, 0}, {0, th}, {tw, th} };
		result.emplace_back(poly);
		uvMapSet.emplace_back(uv);
		triangles.push_back(Triangle(index + 0, index + 1, index + 2));
		index += 3;
	}

	//side 3: back
	{
		PolyArray poly = { tip, bl, br };
		std::vector<Vector2> uv = { {0.5f * tw, 0}, {tw, th}, {0, th} };
		result.emplace_back(poly);
		uvMapSet.emplace_back(uv);
		triangles.push_back(Triangle(index + 0, index + 1, index + 2));
		index += 3;
	}

	//side 4: left
	{
		PolyArray poly = { tip, fl, bl };
		std::vector<Vector2> uv = { {0.5f * tw, 0}, {tw, th}, {0, th} };
		result.emplace_back(poly);
		uvMapSet.emplace_back(uv);
		triangles.push_back(Triangle(index + 0, index + 1, index + 2));
		index += 3;
	}

	//base (2 triangles)
	{
		//triangle 1
		PolyArray poly1 = { fl, fr, br };
		std::vector<Vector2> uv1 = { {0, 0}, {tw, 0}, {tw, th} };
		result.emplace_back(poly1);
		uvMapSet.emplace_back(uv1);
		triangles.push_back(Triangle(index + 0, index + 1, index + 2));

		//triangle 2
		PolyArray poly2 = { fl, br, bl };
		std::vector<Vector2> uv2 = { {0, 0}, {tw, th}, {0, th} };
		result.emplace_back(poly2);
		uvMapSet.emplace_back(uv2);
		triangles.push_back(Triangle(index + 3, index + 4, index + 5));

		index += 6;
	}

	//create polygon object from the generated pyramid segments
	parent->AddPolygon(name, texture, result, uvMapSet, triangles, tw, th, autosize);
}

void Shape::CreatePrism(const std::string &name, const std::string &texture, const Vector3 &Center, Real width, Real depth, Real height, Real tw, Real th, bool autosize)
{
	if (!parent)
		return;

	if (width <= 0 || depth <= 0 || height <= 0)
	{
		parent->ReportError("Prism requires positive dimensions");
		return;
	}

	PolygonSet result;
	std::vector<std::vector<Vector2>> uvMapSet;
	std::vector<Triangle> triangles;

	Vector3 originOffset = Center + origin;

	//triangle base (XY projection):
	//  p0 and p1: base edge
	//  p2: apex (middle top of triangle)
	Vector3 p0 = originOffset + Vector3(-width * 0.5f, 0, -depth * 0.5f);
	Vector3 p1 = originOffset + Vector3(width * 0.5f, 0, -depth * 0.5f);
	Vector3 p2 = originOffset + Vector3(0, 0, depth * 0.5f);

	//extruded top
	Vector3 q0 = p0 + Vector3(0, height, 0);
	Vector3 q1 = p1 + Vector3(0, height, 0);
	Vector3 q2 = p2 + Vector3(0, height, 0);

	int index = 0;

	//bottom face
	{
		PolyArray poly = { p0, p1, p2 };
		std::vector<Vector2> uv = { {0, 0}, {tw, 0}, {0.5f * tw, th} };
		result.emplace_back(poly);
		uvMapSet.emplace_back(uv);
		triangles.push_back(Triangle(index + 0, index + 1, index + 2));
		index += 3;
	}

	//top face
	{
		PolyArray poly = { q0, q2, q1 };
		std::vector<Vector2> uv = { {0, 0}, {0.5f * tw, th}, {tw, 0} };
		result.emplace_back(poly);
		uvMapSet.emplace_back(uv);
		triangles.push_back(Triangle(index + 0, index + 1, index + 2));
		index += 3;
	}

	//side 1 (p0 to p1)
	{
		PolyArray poly1 = { p0, p1, q1 };
		std::vector<Vector2> uv1 = { {0, 0}, {tw, 0}, {tw, th} };
		result.emplace_back(poly1); uvMapSet.emplace_back(uv1);
		triangles.push_back(Triangle(index + 0, index + 1, index + 2));

		PolyArray poly2 = { p0, q1, q0 };
		std::vector<Vector2> uv2 = { {0, 0}, {tw, th}, {0, th} };
		result.emplace_back(poly2); uvMapSet.emplace_back(uv2);
		triangles.push_back(Triangle(index + 3, index + 4, index + 5));
		index += 6;
	}

	//side 2 (p1 to p2)
	{
		PolyArray poly1 = { p1, p2, q2 };
		std::vector<Vector2> uv1 = { {0, 0}, {tw, 0}, {tw, th} };
		result.emplace_back(poly1); uvMapSet.emplace_back(uv1);
		triangles.push_back(Triangle(index + 0, index + 1, index + 2));

		PolyArray poly2 = { p1, q2, q1 };
		std::vector<Vector2> uv2 = { {0, 0}, {tw, th}, {0, th} };
		result.emplace_back(poly2); uvMapSet.emplace_back(uv2);
		triangles.push_back(Triangle(index + 3, index + 4, index + 5));
		index += 6;
	}

	//side 3 (p2 to p0)
	{
		PolyArray poly1 = { p2, p0, q0 };
		std::vector<Vector2> uv1 = { {0, 0}, {tw, 0}, {tw, th} };
		result.emplace_back(poly1); uvMapSet.emplace_back(uv1);
		triangles.push_back(Triangle(index + 0, index + 1, index + 2));

		PolyArray poly2 = { p2, q0, q2 };
		std::vector<Vector2> uv2 = { {0, 0}, {tw, th}, {0, th} };
		result.emplace_back(poly2); uvMapSet.emplace_back(uv2);
		triangles.push_back(Triangle(index + 3, index + 4, index + 5));
		index += 6;
	}

	//create polygon object from the generated prism segments
	parent->AddPolygon(name, texture, result, uvMapSet, triangles, tw, th, autosize);
}

void Shape::CreateTetrahedron(const std::string &name, const std::string &texture, const Vector3 &Center, Real size, Real tw, Real th, bool autosize)
{
	if (!parent)
		return;

	if (size <= 0)
	{
		parent->ReportError("Tetrahedron requires a positive size");
		return;
	}

	PolygonSet result;
	std::vector<std::vector<Vector2>> uvMapSet;
	std::vector<Triangle> triangles;

	//equilateral triangle math
	Real h = sqrtf(2.0f / 3.0f) * size;      // height from base to tip
	Real r = size / sqrtf(3.0f);             // radius of triangle inscribed circle

	//define 4 points
	Vector3 p0 = Vector3(0, h, 0);                           // tip
	Vector3 p1 = Vector3(-size * 0.5f, 0, -r);               // base left
	Vector3 p2 = Vector3(size * 0.5f, 0, -r);                // base right
	Vector3 p3 = Vector3(0, 0, size - r);                    // base center forward

	//offset to center
	p0 += Center + origin;
	p1 += Center + origin;
	p2 += Center + origin;
	p3 += Center + origin;

	int index = 0;

	//face 1: base
	{
		PolyArray poly = { p1, p2, p3 };
		std::vector<Vector2> uv = { {0, 0}, {tw, 0}, {0.5f * tw, th} };
		result.emplace_back(poly);
		uvMapSet.emplace_back(uv);
		triangles.push_back(Triangle(index + 0, index + 1, index + 2));
		index += 3;
	}

	//face 2: tip, p2, p1
	{
		PolyArray poly = { p0, p2, p1 };
		std::vector<Vector2> uv = { {0.5f * tw, 0}, {tw, th}, {0, th} };
		result.emplace_back(poly);
		uvMapSet.emplace_back(uv);
		triangles.push_back(Triangle(index + 0, index + 1, index + 2));
		index += 3;
	}

	//face 3: tip, p3, p2
	{
		PolyArray poly = { p0, p3, p2 };
		std::vector<Vector2> uv = { {0.5f * tw, 0}, {0, th}, {tw, th} };
		result.emplace_back(poly);
		uvMapSet.emplace_back(uv);
		triangles.push_back(Triangle(index + 0, index + 1, index + 2));
		index += 3;
	}

	//face 4: tip, p1, p3
	{
		PolyArray poly = { p0, p1, p3 };
		std::vector<Vector2> uv = { {0.5f * tw, 0}, {0, th}, {tw, th} };
		result.emplace_back(poly);
		uvMapSet.emplace_back(uv);
		triangles.push_back(Triangle(index + 0, index + 1, index + 2));
		index += 3;
	}

	//create polygon object from the generated tetrahedron segments
	parent->AddPolygon(name, texture, result, uvMapSet, triangles, tw, th, autosize);
}

void Shape::CreateOctahedron(const std::string &name, const std::string &texture, const Vector3 &Center, Real size, Real tw, Real th, bool autosize)
{
	if (!parent)
		return;

	if (size <= 0)
	{
		parent->ReportError("Octahedron requires a positive size");
		return;
	}

	PolygonSet result;
	std::vector<std::vector<Vector2>> uvMapSet;
	std::vector<Triangle> triangles;

	Real half = size * 0.5f;
	Vector3 originOffset = Center + origin;

	//octahedron has 6 vertices: top, bottom, and 4 around the equator
	Vector3 top = originOffset + Vector3(0, half, 0);
	Vector3 bottom = originOffset + Vector3(0, -half, 0);
	Vector3 v0 = originOffset + Vector3(-half, 0, 0);
	Vector3 v1 = originOffset + Vector3(0, 0, half);
	Vector3 v2 = originOffset + Vector3(half, 0, 0);
	Vector3 v3 = originOffset + Vector3(0, 0, -half);

	int index = 0;

	//upper pyramid
	auto add_face = [&](const Vector3& a, const Vector3& b, const Vector3& c) {
		PolyArray poly = { a, b, c };
		std::vector<Vector2> uv = {
			{0.5f * tw, 0},
			{0, th},
			{tw, th}
		};
		result.emplace_back(poly);
		uvMapSet.emplace_back(uv);
		triangles.push_back(Triangle(index + 0, index + 1, index + 2));
		index += 3;
	};

	add_face(top, v0, v1);
	add_face(top, v1, v2);
	add_face(top, v2, v3);
	add_face(top, v3, v0);

	//lower pyramid
	add_face(bottom, v1, v0);
	add_face(bottom, v2, v1);
	add_face(bottom, v3, v2);
	add_face(bottom, v0, v3);

	//create polygon object from the generated octahedron segments
	parent->AddPolygon(name, texture, result, uvMapSet, triangles, tw, th, autosize);
}

}
