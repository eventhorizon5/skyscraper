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
	polymesh = sbs->GetPolyMesh();
}

Shape::~Shape()
{

}

void Shape::CreateSphere(const std::string &name, const std::string &texture, const Vector3 &Center, Real radius, int latSteps, int lonSteps, Real tw, Real th, bool inside, bool autosize)
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
			if (inside == false)
				tri1.a = 0 + index;
			else
				tri1.a = 2 + index;

			poly1.emplace_back(p2 + Center + origin);
			uvMap.emplace_back(Vector2(u1, v2));
			tri1.b = 1 + index;

			poly1.emplace_back(p3 + Center + origin);
			uvMap.emplace_back(Vector2(u2, v2));
			if (inside == false)
				tri1.c = 2 + index;
			else
				tri1.c = 0 + index;

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
			if (inside == false)
				tri2.a = 3 + index;
			else
				tri2.a = 5 + index;

			poly2.emplace_back(p3 + Center + origin);
			uvMap.emplace_back(Vector2(u2, v2));
			tri2.b = 4 + index;

			poly2.emplace_back(p4 + Center + origin);
			uvMap.emplace_back(Vector2(u2, v1));
			if (inside == false)
				tri2.c = 5 + index;
			else
				tri2.c = 3 + index;

			result.emplace_back(poly2);
			uvMapSet.emplace_back(uvMap);
			triangles.emplace_back(tri2);
			index += 6;
		}
	}

	//create polygon from the generated sphere segments
	parent->AddPolygon(name, texture, result, uvMapSet, triangles, tw, th, autosize);
}

void Shape::CreateBox(const std::string &name, const std::string &texture, Ogre::Vector3 &center, Real width, Real height, Real depth, Real tw, Real th, bool inside, bool autosize)
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
		if (inside == false)
			tri1.a = 2;
		else
			tri1.a = 0;

		poly1.emplace_back(b + center + origin);
		uvMap.emplace_back(uvB);
		tri1.b = 1;

		poly1.emplace_back(c + center + origin);
		uvMap.emplace_back(uvC);
		if (inside == false)
			tri1.c = 0;
		else
			tri1.c = 2;

		result.emplace_back(poly1);
		uvMapSet.emplace_back(uvMap);
		triangles.emplace_back(tri1);

		uvMap.clear();
		PolyArray poly2;
		Triangle tri2;
		poly2.emplace_back(a + center + origin);
		uvMap.emplace_back(uvA);
		if (inside == false)
			tri2.a = 5;
		else
			tri2.a = 3;

		poly2.emplace_back(c + center + origin);
		uvMap.emplace_back(uvC);
		tri2.b = 4;

		poly2.emplace_back(d + center + origin);
		uvMap.emplace_back(uvD);
		if (inside == false)
			tri2.c = 3;
		else
			tri2.c = 5;

		result.emplace_back(poly2);
		uvMapSet.emplace_back(uvMap);
		triangles.emplace_back(tri2);

		//create polygon from the generated triangles
		parent->AddPolygon(name, texture, result, uvMapSet, triangles, tw, th, autosize);
	};

	//back face (+Z)
	if (polymesh->DrawMainP)
		addQuad(p101, p001, p011, p111, {1, 0}, {0, 0}, {0, 1}, {1, 1});

	//front face (-Z)
	if (polymesh->DrawMainN)
	addQuad(p100, p110, p010, p000, {1, 0}, {1, 1}, {0, 1}, {0, 0});

	//left face (-X)
	if (polymesh->DrawSideN)
		addQuad(p001, p000, p010, p011, {1, 0}, {0, 0}, {0, 1}, {1, 1});

	//right face (+X)
	if (polymesh->DrawSideP)
		addQuad(p100, p101, p111, p110, {0, 0}, {1, 0}, {1, 1}, {0, 1});

	//top face (+Y)
	if (polymesh->DrawTop)
		addQuad(p011, p010, p110, p111, {0, 1}, {0, 0}, {1, 0}, {1, 1});

	//bottom face (-Y)
	if (polymesh->DrawBottom)
		addQuad(p100, p000, p001, p101, {1, 1}, {0, 1}, {0, 0}, {1, 0});
}

void Shape::CreateCylinder(const std::string &name, const std::string &texture, const Vector3 &Center, Real radius, Real height, int slices, Real tw, Real th, bool inside, bool autosize)
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

	bool draw_side = polymesh->DrawSideN || polymesh->DrawSideP;

	//side surface
	for (int i = 0; i < slices; ++i)
	{
		if (!draw_side)
			continue;

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
		if (inside == false)
			triangles.push_back(Triangle(index + 2, index + 1, index + 0));
		else
			triangles.push_back(Triangle(index + 0, index + 1, index + 2));

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
		if (inside == false)
			triangles.push_back(Triangle(index + 5, index + 4, index + 3));
		else
			triangles.push_back(Triangle(index + 3, index + 4, index + 5));

		index += 6;
	}

	//bottom cap
	for (int i = 0; i < slices; ++i)
	{
		if (polymesh->DrawBottom == false)
			continue;

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
		if (inside == false)
			triangles.push_back(Triangle(index + 0, index + 1, index + 2));
		else
			triangles.push_back(Triangle(index + 2, index + 1, index + 0));
		index += 3;
	}

	//top cap
	for (int i = 0; i < slices; ++i)
	{
		if (polymesh->DrawTop == false)
			continue;

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
		if (inside == false)
			triangles.push_back(Triangle(index + 0, index + 1, index + 2));
		else
			triangles.push_back(Triangle(index + 2, index + 1, index + 0));
		index += 3;
	}

	//create polygon object from the generated cylinder segments
	parent->AddPolygon(name, texture, result, uvMapSet, triangles, tw, th, autosize);
}

void Shape::CreateCone(const std::string &name, const std::string &texture, const Vector3 &Center, Real radius, Real height, int slices, Real tw, Real th, bool inside, bool autosize)
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

	bool draw_side = polymesh->DrawSideN || polymesh->DrawSideP;

	//side surface (triangle fan from tip to base circle)
	for (int i = 0; i < slices; ++i)
	{
		if (!draw_side)
			continue;

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
		if (inside == false)
			triangles.push_back(Triangle(index + 2, index + 1, index + 0));
		else
			triangles.push_back(Triangle(index + 0, index + 1, index + 2));
		index += 3;
	}

	//bottom cap (triangle fan centered on base)
	for (int i = 0; i < slices; ++i)
	{
		if (!polymesh->DrawBottom)
			continue;

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
		if (inside == false)
			triangles.push_back(Triangle(index + 2, index + 1, index + 0));
		else
			triangles.push_back(Triangle(index + 0, index + 1, index + 2));
		index += 3;
	}

	//create polygon object from the generated cone segments
	parent->AddPolygon(name, texture, result, uvMapSet, triangles, tw, th, autosize);
}

void Shape::CreateCapsule(const std::string &name, const std::string &texture, const Vector3 &Center, Real radius, Real height, int latSteps, int lonSteps, Real tw, Real th, bool inside, bool autosize)
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
		result.emplace_back(poly1);
		uvMapSet.emplace_back(uv1);
		if (inside == false)
			triangles.push_back(Triangle(index + 2, index + 1, index + 0));
		else
			triangles.push_back(Triangle(index + 0, index + 1, index + 2));

		PolyArray poly2 = { p1 + Center + origin, p3 + Center + origin, p4 + Center + origin };
		std::vector<Vector2> uv2 = { {u1 * tw, 0}, {u2 * tw, th}, {u1 * tw, th} };
		result.emplace_back(poly2);
		uvMapSet.emplace_back(uv2);
		if (inside == false)
			triangles.push_back(Triangle(index + 5, index + 4, index + 3));
		else
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
			result.emplace_back(poly1);
			uvMapSet.emplace_back(uv1);
			if (inside == false)
				triangles.push_back(Triangle(index + 0, index + 1, index + 2));
			else
				triangles.push_back(Triangle(index + 2, index + 1, index + 0));

			//second triangle
			PolyArray poly2 = { p1, p3, p4 };
			std::vector<Vector2> uv2 = { {u1 * tw, v1 * th}, {u2 * tw, v2 * th}, {u2 * tw, v1 * th} };
			result.emplace_back(poly2);
			uvMapSet.emplace_back(uv2);
			if (inside == false)
				triangles.push_back(Triangle(index + 3, index + 4, index + 5));
			else
				triangles.push_back(Triangle(index + 5, index + 4, index + 3));

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
			result.emplace_back(poly1);
			uvMapSet.emplace_back(uv1);
			if (inside == false)
				triangles.push_back(Triangle(index + 2, index + 1, index + 0));
			else
				triangles.push_back(Triangle(index + 0, index + 1, index + 2));

			//second triangle
			PolyArray poly2 = { p1, p3, p4 };
			std::vector<Vector2> uv2 = { {u1 * tw, v1 * th}, {u2 * tw, v2 * th}, {u2 * tw, v1 * th} };
			result.emplace_back(poly2);
			uvMapSet.emplace_back(uv2);
			if (inside == false)
				triangles.push_back(Triangle(index + 5, index + 4, index + 3));
			else
				triangles.push_back(Triangle(index + 3, index + 4, index + 5));

			index += 6;
		}
	}

	//create polygon object from the generated capsule segments
	parent->AddPolygon(name, texture, result, uvMapSet, triangles, tw, th, autosize);
}

void Shape::CreatePlane(const std::string &name, const std::string &texture, const Vector3 &Center, Real width, Real depth, int xSegments, int zSegments, Real tw, Real th, bool face_down, bool autosize)
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
			if (face_down == false)
				triangles.emplace_back(Triangle(index + 2, index + 1, index + 0));
			else
				triangles.emplace_back(Triangle(index + 0, index + 1, index + 2));

			//triangle 2
			PolyArray poly2 = { p1, p3, p4 };
			std::vector<Vector2> uv2 = { {u1 * tw, v1 * th}, {u2 * tw, v2 * th}, {u1 * tw, v2 * th} };
			result.emplace_back(poly2);
			uvMapSet.emplace_back(uv2);
			if (face_down == false)
				triangles.emplace_back(Triangle(index + 5, index + 4, index + 3));
			else
				triangles.emplace_back(Triangle(index + 3, index + 4, index + 5));

			index += 6;
		}
	}

	//create polygon object from the generated plane segments
	parent->AddPolygon(name, texture, result, uvMapSet, triangles, tw, th, autosize);
}

void Shape::CreateCircle(const std::string &name, const std::string &texture, const Vector3 &Center, Real radius, int slices, Real tw, Real th, bool face_down, bool autosize)
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
		if (face_down == false)
			triangles.emplace_back(Triangle(index + 2, index + 1, index + 0));
		else
			triangles.emplace_back(Triangle(index + 0, index + 1, index + 2));
		index += 3;
	}

	//create polygon object from the generated circle segments
	parent->AddPolygon(name, texture, result, uvMapSet, triangles, tw, th, autosize);
}

void Shape::CreateTorus(const std::string &name, const std::string &texture, const Vector3 &Center, Real majorRadius, Real minorRadius, int radialSteps, int tubeSteps, Real tw, Real th, bool inside, bool autosize)
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
			if (inside == false)
				triangles.emplace_back(Triangle(index + 2, index + 1, index + 0));
			else
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
			if (inside == false)
				triangles.emplace_back(Triangle(index + 5, index + 4, index + 3));
			else
				triangles.emplace_back(Triangle(index + 3, index + 4, index + 5));

			index += 6;
		}
	}

	//create polygon object from the generated torus segments
	parent->AddPolygon(name, texture, result, uvMapSet, triangles, tw, th, autosize);
}

void Shape::CreateDome(const std::string &name, const std::string &texture, const Vector3 &Center, Real radius, int latSteps, int lonSteps, Real tw, Real th, bool inside, bool autosize)
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
			if (inside == false)
				triangles.emplace_back(Triangle(index + 0, index + 1, index + 2));
			else
				triangles.emplace_back(Triangle(index + 2, index + 1, index + 0));

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
			if (inside == false)
				triangles.emplace_back(Triangle(index + 3, index + 4, index + 5));
			else
				triangles.emplace_back(Triangle(index + 5, index + 4, index + 3));

			index += 6;
		}
	}

	//create polygon object from the generated dome segments
	parent->AddPolygon(name, texture, result, uvMapSet, triangles, tw, th, autosize);
}

void Shape::CreatePyramid(const std::string &name, const std::string &texture, const Vector3 &Center, Real width, Real depth, Real height, Real tw, Real th, bool inside, bool autosize)
{
	//creates a pyramid

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

	//side 1: back
	if (polymesh->DrawMainP)
	{
		PolyArray poly = { tip, fr, fl };
		std::vector<Vector2> uv = { {0.5f * tw, 0}, {tw, th}, {0, th} };
		result.emplace_back(poly);
		uvMapSet.emplace_back(uv);
		if (inside == false)
			triangles.push_back(Triangle(index + 2, index + 1, index + 0));
		else
			triangles.push_back(Triangle(index + 0, index + 1, index + 2));
		index += 3;
	}

	//side 2: right
	if (polymesh->DrawSideP)
	{
		PolyArray poly = { tip, br, fr };
		std::vector<Vector2> uv = { {0.5f * tw, 0}, {0, th}, {tw, th} };
		result.emplace_back(poly);
		uvMapSet.emplace_back(uv);
		if (inside == false)
			triangles.push_back(Triangle(index + 2, index + 1, index + 0));
		else
			triangles.push_back(Triangle(index + 0, index + 1, index + 2));
		index += 3;
	}

	//side 3: front
	if (polymesh->DrawMainN)
	{
		PolyArray poly = { tip, bl, br };
		std::vector<Vector2> uv = { {0.5f * tw, 0}, {tw, th}, {0, th} };
		result.emplace_back(poly);
		uvMapSet.emplace_back(uv);
		if (inside == false)
			triangles.push_back(Triangle(index + 0, index + 1, index + 2));
		else
			triangles.push_back(Triangle(index + 2, index + 1, index + 0));
		index += 3;
	}

	//side 4: left
	if (polymesh->DrawSideN)
	{
		PolyArray poly = { tip, fl, bl };
		std::vector<Vector2> uv = { {0.5f * tw, 0}, {tw, th}, {0, th} };
		result.emplace_back(poly);
		uvMapSet.emplace_back(uv);
		if (inside == false)
			triangles.push_back(Triangle(index + 2, index + 1, index + 0));
		else
			triangles.push_back(Triangle(index + 0, index + 1, index + 2));
		index += 3;
	}

	//base (2 triangles)
	if (polymesh->DrawBottom)
	{
		//triangle 1
		PolyArray poly1 = { fl, fr, br };
		std::vector<Vector2> uv1 = { {0, 0}, {tw, 0}, {tw, th} };
		result.emplace_back(poly1);
		uvMapSet.emplace_back(uv1);
		if (inside == false)
			triangles.push_back(Triangle(index + 2, index + 1, index + 0));
		else
			triangles.push_back(Triangle(index + 0, index + 1, index + 2));

		//triangle 2
		PolyArray poly2 = { fl, br, bl };
		std::vector<Vector2> uv2 = { {0, 0}, {tw, th}, {0, th} };
		result.emplace_back(poly2);
		uvMapSet.emplace_back(uv2);
		if (inside == false)
			triangles.push_back(Triangle(index + 5, index + 4, index + 3));
		else
			triangles.push_back(Triangle(index + 3, index + 4, index + 5));

		index += 6;
	}

	//create polygon object from the generated pyramid segments
	parent->AddPolygon(name, texture, result, uvMapSet, triangles, tw, th, autosize);
}

void Shape::CreatePrism(const std::string &name, const std::string &texture, const Vector3 &Center, Real width, Real depth, Real height, Real tw, Real th, bool inside, bool autosize)
{
	//creates a triangular prism

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
	if (polymesh->DrawBottom)
	{
		PolyArray poly = { p0, p1, p2 };
		std::vector<Vector2> uv = { {0, 0}, {tw, 0}, {0.5f * tw, th} };
		result.emplace_back(poly);
		uvMapSet.emplace_back(uv);
		if (inside == false)
			triangles.push_back(Triangle(index + 0, index + 1, index + 2));
		else
			triangles.push_back(Triangle(index + 2, index + 1, index + 0));
		index += 3;
	}

	//top face
	if (polymesh->DrawTop)
	{
		PolyArray poly = { q0, q2, q1 };
		std::vector<Vector2> uv = { {0, 0}, {0.5f * tw, th}, {tw, 0} };
		result.emplace_back(poly);
		uvMapSet.emplace_back(uv);
		if (inside == false)
			triangles.push_back(Triangle(index + 0, index + 1, index + 2));
		else
			triangles.push_back(Triangle(index + 2, index + 1, index + 0));
		index += 3;
	}

	//side 1 (p0 to p1)
	if (polymesh->DrawSideN || polymesh->DrawSideP)
	{
		PolyArray poly1 = { p0, p1, q1 };
		std::vector<Vector2> uv1 = { {0, 0}, {tw, 0}, {tw, th} };
		result.emplace_back(poly1); uvMapSet.emplace_back(uv1);
		if (inside == false)
			triangles.push_back(Triangle(index + 2, index + 1, index + 0));
		else
			triangles.push_back(Triangle(index + 0, index + 1, index + 2));

		PolyArray poly2 = { p0, q1, q0 };
		std::vector<Vector2> uv2 = { {0, 0}, {tw, th}, {0, th} };
		result.emplace_back(poly2); uvMapSet.emplace_back(uv2);
		if (inside == false)
			triangles.push_back(Triangle(index + 5, index + 4, index + 3));
		else
			triangles.push_back(Triangle(index + 3, index + 4, index + 5));
		index += 6;
	}

	//side 2 (p1 to p2)
	if (polymesh->DrawSideN || polymesh->DrawSideP)
	{
		PolyArray poly1 = { p1, p2, q2 };
		std::vector<Vector2> uv1 = { {0, 0}, {tw, 0}, {tw, th} };
		result.emplace_back(poly1); uvMapSet.emplace_back(uv1);
		if (inside == false)
			triangles.push_back(Triangle(index + 2, index + 1, index + 0));
		else
			triangles.push_back(Triangle(index + 0, index + 1, index + 2));

		PolyArray poly2 = { p1, q2, q1 };
		std::vector<Vector2> uv2 = { {0, 0}, {tw, th}, {0, th} };
		result.emplace_back(poly2); uvMapSet.emplace_back(uv2);
		if (inside == false)
			triangles.push_back(Triangle(index + 5, index + 4, index + 3));
		else
			triangles.push_back(Triangle(index + 3, index + 4, index + 5));
		index += 6;
	}

	//side 3 (p2 to p0)
	if (polymesh->DrawSideN || polymesh->DrawSideP)
	{
		PolyArray poly1 = { p2, p0, q0 };
		std::vector<Vector2> uv1 = { {0, 0}, {tw, 0}, {tw, th} };
		result.emplace_back(poly1); uvMapSet.emplace_back(uv1);
		if (inside == false)
			triangles.push_back(Triangle(index + 2, index + 1, index + 0));
		else
			triangles.push_back(Triangle(index + 0, index + 1, index + 2));

		PolyArray poly2 = { p2, q0, q2 };
		std::vector<Vector2> uv2 = { {0, 0}, {tw, th}, {0, th} };
		result.emplace_back(poly2); uvMapSet.emplace_back(uv2);
		if (inside == false)
			triangles.push_back(Triangle(index + 5, index + 4, index + 3));
		else
			triangles.push_back(Triangle(index + 3, index + 4, index + 5));
		index += 6;
	}

	//create polygon object from the generated prism segments
	parent->AddPolygon(name, texture, result, uvMapSet, triangles, tw, th, autosize);
}

void Shape::CreateTetrahedron(const std::string &name, const std::string &texture, const Vector3 &Center, Real size, Real tw, Real th, bool inside, bool autosize)
{
	//creates a tetrahedron

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
	if (polymesh->DrawBottom)
	{
		PolyArray poly = { p1, p2, p3 };
		std::vector<Vector2> uv = { {0, 0}, {tw, 0}, {0.5f * tw, th} };
		result.emplace_back(poly);
		uvMapSet.emplace_back(uv);
		if (inside == false)
			triangles.push_back(Triangle(index + 0, index + 1, index + 2));
		else
			triangles.push_back(Triangle(index + 2, index + 1, index + 0));
		index += 3;
	}

	//face 2: tip, p2, p1
	if (polymesh->DrawSideN || polymesh->DrawSideP)
	{
		PolyArray poly = { p0, p2, p1 };
		std::vector<Vector2> uv = { {0.5f * tw, 0}, {tw, th}, {0, th} };
		result.emplace_back(poly);
		uvMapSet.emplace_back(uv);
		if (inside == false)
			triangles.push_back(Triangle(index + 0, index + 1, index + 2));
		else
			triangles.push_back(Triangle(index + 2, index + 1, index + 0));
		index += 3;
	}

	//face 3: tip, p3, p2
	if (polymesh->DrawSideN || polymesh->DrawSideP)
	{
		PolyArray poly = { p0, p3, p2 };
		std::vector<Vector2> uv = { {0.5f * tw, 0}, {0, th}, {tw, th} };
		result.emplace_back(poly);
		uvMapSet.emplace_back(uv);
		if (inside == false)
			triangles.push_back(Triangle(index + 0, index + 1, index + 2));
		else
			triangles.push_back(Triangle(index + 2, index + 1, index + 0));
		index += 3;
	}

	//face 4: tip, p1, p3
	if (polymesh->DrawSideN || polymesh->DrawSideP)
	{
		PolyArray poly = { p0, p1, p3 };
		std::vector<Vector2> uv = { {0.5f * tw, 0}, {0, th}, {tw, th} };
		result.emplace_back(poly);
		uvMapSet.emplace_back(uv);
		if (inside == false)
			triangles.push_back(Triangle(index + 0, index + 1, index + 2));
		else
			triangles.push_back(Triangle(index + 2, index + 1, index + 0));
		index += 3;
	}

	//create polygon object from the generated tetrahedron segments
	parent->AddPolygon(name, texture, result, uvMapSet, triangles, tw, th, autosize);
}

void Shape::CreateOctahedron(const std::string &name, const std::string &texture, const Vector3 &Center, Real size, Real tw, Real th, bool inside, bool autosize)
{
	//creates an octahedron
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
		if (inside == false)
			triangles.push_back(Triangle(index + 0, index + 1, index + 2));
		else
			triangles.push_back(Triangle(index + 2, index + 1, index + 0));
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

void Shape::CreateIcosahedron(const std::string &name, const std::string &texture, const Vector3 &Center, Real radius, Real tw, Real th, bool inside, bool autosize)
{
	//creates an icosahedron

	if (!parent)
		return;

	if (radius <= 0)
	{
		parent->ReportError("Icosahedron requires a positive radius");
		return;
	}

	PolygonSet result;
	std::vector<std::vector<Vector2>> uvMapSet;
	std::vector<Triangle> triangles;

	//golden ratio
	const Real phi = (1.0f + sqrtf(5.0f)) * 0.5f;

	//base (unnormalized) vertex set for an icosahedron
	std::vector<Vector3> v = {
		{-1,  phi,  0}, { 1,  phi,  0}, {-1, -phi,  0}, { 1, -phi,  0},
		{ 0, -1,  phi}, { 0,  1,  phi}, { 0, -1, -phi}, { 0,  1, -phi},
		{ phi,  0, -1}, { phi,  0,  1}, {-phi,  0, -1}, {-phi,  0,  1}
	};

	//normalize to unit length, then scale to 'radius'
	for (auto &p : v)
	{
		const Real len = sqrtf(p.x * p.x + p.y * p.y + p.z * p.z);
		const Real s = (len > 0.0f) ? (radius / len) : 0.0f;
		p.x *= s; p.y *= s; p.z *= s;
	}

	//face index list (20 faces)
	const int f[20][3] = {
		{0,11,5}, {0,5,1}, {0,1,7}, {0,7,10}, {0,10,11},
		{1,5,9}, {5,11,4}, {11,10,2}, {10,7,6}, {7,1,8},
		{3,9,4}, {3,4,2}, {3,2,6}, {3,6,8}, {3,8,9},
		{4,9,5}, {2,4,11}, {6,2,10}, {8,6,7}, {9,8,1}
	};

	Vector3 offset = Center + origin;
	int index = 0;

	for (int i = 0; i < 20; ++i)
	{
		const Vector3 p0 = v[f[i][0]] + offset;
		const Vector3 p1 = v[f[i][1]] + offset;
		const Vector3 p2 = v[f[i][2]] + offset;

		//one triangle per face
		PolyArray poly = { p0, p1, p2 };

		//simple triangular UVs (face-local); tweak as needed
		std::vector<Vector2> uv = {
			{0.5f * tw, 0.0f},
			{0.0f,      th },
			{tw,        th }
		};

		result.emplace_back(poly);
		uvMapSet.emplace_back(uv);
		if (inside == false)
		{
			Triangle tri (index + 0, index + 1, index + 2);
			triangles.emplace_back(tri);
		}
		else
		{
			Triangle tri (index + 2, index + 1, index + 0);
			triangles.emplace_back(tri);
		}
		index += 3;
	}

	//create polygon object from the generated icosahedron segments
	parent->AddPolygon(name, texture, result, uvMapSet, triangles, tw, th, autosize);
}

void Shape::CreateGeoSphere(const std::string &name, const std::string &texture, const Vector3 &Center, Real radius, int subdivisions, Real tw, Real th, bool inside, bool autosize)
{
	//creates a geodesic sphere by subdividing an icosahedron

	if (!parent)
		return;

	if (radius <= 0)
	{
		parent->ReportError("Geosphere requires a positive radius");
		return;
	}

	if (subdivisions < 0)
	{
		parent->ReportError("Geosphere requires a non-negative subdivision count");
		return;
	}

	PolygonSet result;
	std::vector<std::vector<Vector2>> uvMapSet;
	std::vector<Triangle> triangles;

	// ---- base icosahedron ----
	const Real phi = (1.0f + sqrtf(5.0f)) * 0.5f;

	std::vector<Vector3> base = {
		{-1,  phi,  0}, { 1,  phi,  0}, {-1, -phi,  0}, { 1, -phi,  0},
		{ 0, -1,  phi}, { 0,  1,  phi}, { 0, -1, -phi}, { 0,  1, -phi},
		{ phi,  0, -1}, { phi,  0,  1}, {-phi,  0, -1}, {-phi,  0,  1}
	};

	//normalize to unit, then scale to radius
	auto normalize_to_radius = [&](const Vector3& p)->Vector3 {
		Real len = sqrtf(p.x*p.x + p.y*p.y + p.z*p.z);
		if (len == 0)
			return Vector3(0,0,0);
		Real s = radius / len;
		return Vector3(p.x * s, p.y * s, p.z * s);
	};

	for (auto &p : base) p = normalize_to_radius(p);

	static const int facesIdx[20][3] = {
		{0,11,5}, {0,5,1}, {0,1,7}, {0,7,10}, {0,10,11},
		{1,5,9}, {5,11,4}, {11,10,2}, {10,7,6}, {7,1,8},
		{3,9,4}, {3,4,2}, {3,2,6}, {3,6,8}, {3,8,9},
		{4,9,5}, {2,4,11}, {6,2,10}, {8,6,7}, {9,8,1}
	};

	struct Tri { Vector3 a, b, c; };
	std::vector<Tri> faces; faces.reserve(20);
	for (int i = 0; i < 20; ++i)
	{
		faces.push_back({ base[facesIdx[i][0]], base[facesIdx[i][1]], base[facesIdx[i][2]] });
	}

	// ---- subdivide ----
	auto midpoint_on_sphere = [&](const Vector3& p, const Vector3& q)->Vector3 {
		Vector3 m = Vector3((p.x + q.x) * 0.5f, (p.y + q.y) * 0.5f, (p.z + q.z) * 0.5f);
		//project midpoint back to sphere of radius
		return normalize_to_radius(m);
	};

	subdivisions = std::max(0, subdivisions);
	for (int s = 0; s < subdivisions; ++s)
	{
		std::vector<Tri> next;
		next.reserve(faces.size() * 4);
		for (const auto& t : faces)
		{
			Vector3 ab = midpoint_on_sphere(t.a, t.b);
			Vector3 bc = midpoint_on_sphere(t.b, t.c);
			Vector3 ca = midpoint_on_sphere(t.c, t.a);
			// split into 4
			next.push_back({ t.a, ab, ca });
			next.push_back({ t.b, bc, ab });
			next.push_back({ t.c, ca, bc });
			next.push_back({ ab, bc, ca });
		}
		faces.swap(next);
	}

	// ---- spherical UVs per-vertex, with per-triangle seam fix ----
	auto sph_uv = [&](const Vector3& p)->Vector2 {
		//convert to unit first for stable UVs
		Real len = sqrtf(p.x * p.x + p.y * p.y + p.z * p.z);
		Vector3 n = (len>0)? Vector3(p.x / len, p.y / len, p.z / len) : Vector3(0, 1, 0);
		Real u = 0.5f + atan2f(n.z, n.x) / (2.0f * pi); // [0,1)
		Real v = 0.5f - asinf(n.y) / pi;                // [0,1]
		return Vector2(u, v);
	};

	auto fix_seam = [&](Vector2& a, Vector2& b, Vector2& c)
	{
		//if triangle straddles U wrap (e.g., 0.98, 0.01), lift small u by +1.0 to keep ordering tight
		float umax = std::max({a.x, b.x, c.x});
		float umin = std::min({a.x, b.x, c.x});
		if (umax - umin > 0.5f)
		{
			for (auto* uv : {&a, &b, &c})
			{
				if (uv->x < 0.5f)
					uv->x += 1.0f;
			}
		}
	};

	Vector3 offset = Center + origin;
	int index = 0;

	result.reserve(faces.size());
	uvMapSet.reserve(faces.size());
	triangles.reserve(faces.size());

	for (const auto& t : faces)
	{
		Vector3 p0 = t.a + offset;
		Vector3 p1 = t.b + offset;
		Vector3 p2 = t.c + offset;

		Vector2 uv0 = sph_uv(t.a);
		Vector2 uv1 = sph_uv(t.b);
		Vector2 uv2 = sph_uv(t.c);
		fix_seam(uv0, uv1, uv2);

		PolyArray poly = { p0, p1, p2 };
		std::vector<Vector2> uvs = {
			Vector2(uv0.x * tw, uv0.y * th),
			Vector2(uv1.x * tw, uv1.y * th),
			Vector2(uv2.x * tw, uv2.y * th)
		};

		result.emplace_back(poly);
		uvMapSet.emplace_back(uvs);
		if (inside == false)
		{
			Triangle tri(index + 0, index + 1, index + 2);
			triangles.emplace_back(tri);
		}
		else
		{
			Triangle tri(index + 2, index + 1, index + 0);
			triangles.emplace_back(tri);
		}
		index += 3;
	}

	//create polygon object from the generated geosphere segments
	parent->AddPolygon(name, texture, result, uvMapSet, triangles, tw, th, autosize);
}

}
