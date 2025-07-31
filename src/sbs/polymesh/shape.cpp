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

}
