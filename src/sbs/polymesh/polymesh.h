/*
	Scalable Building Simulator - PolyMesh Geometry Processor
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

#ifndef _SBS_POLYMESH_H
#define _SBS_POLYMESH_H

#include "polygon.h"

namespace SBS {

typedef std::vector<Polygon*> Polygons;

class SBSIMPEXP PolyMesh : public ObjectBase
{
	friend class Shape;
public:

	//define geometry type
	struct TriangleIndices
	{
		//per-submesh triangle indices
		std::vector<Triangle> triangles; //triangle data, in A B C values
	};

	int WallCount; //wall object count
	int PolygonCount; //wall polygon object count

	//functions

	explicit PolyMesh(Object *parent);
	~PolyMesh();
	bool CreateMesh(MeshObject *mesh, Wall* ownerWall, Polygon* ownerPoly, const std::string &name, const std::string &texture, PolyArray &vertices, Real tw, Real th, bool autosize, Matrix3 &tex_matrix, Vector3 &tex_vector, GeometrySet &geometry, std::vector<Triangle> &triangles, PolygonSet &converted_vertices);
	bool CreateMesh(MeshObject *mesh, Wall* ownerWall, Polygon* ownerPoly, const std::string &name, const std::string &material, PolygonSet &vertices, Matrix3 &tex_matrix, Vector3 &tex_vector, GeometrySet &geometry, std::vector<Triangle> &triangles, PolygonSet &converted_vertices, Real tw, Real th, bool convert_vertices = true);
	bool CreateMesh(MeshObject *mesh, Wall* ownerWall, Polygon* ownerPoly, const std::string &name, const std::string &material, PolygonSet &vertices, std::vector<std::vector<Vector2>> &uvMap, GeometrySet &geometry, std::vector<Triangle> &triangles, PolygonSet &converted_vertices, Real tw, Real th, bool convert_vertices = true);
	Wall* FindWallIntersect(MeshObject *mesh, const Vector3 &start, const Vector3 &end, Vector3 &isect, Real &distance, Vector3 &normal, Wall *wall = 0);
	Vector2* GetTexels(Matrix3 &tex_matrix, Vector3 &tex_vector, PolygonSet &vertices, Real tw, Real th, size_t &texel_count);
	Vector2 GetExtents(int coord, bool flip_z = false);
	bool AddWallMain(Wall* wallobject, const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real height_in1, Real height_in2, Real altitude1, Real altitude2, Real tw, Real th, bool autosize, bool report = true);
	bool AddFloorMain(Wall* wallobject, const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real altitude1, Real altitude2, bool reverse_axis, bool texture_direction, Real tw, Real th, bool autosize, bool legacy_behavior = false, bool report = true);
	Wall* CreateWallBox(MeshObject* mesh, const std::string &name, const std::string &texture, Real x1, Real x2, Real z1, Real z2, Real height_in, Real voffset, Real tw, Real th, bool inside = true, bool outside = true, bool top = true, bool bottom = true, bool autosize = true, bool report = true);
	Wall* CreateWallBox2(MeshObject* mesh, const std::string &name, const std::string &texture, Real CenterX, Real CenterZ, Real WidthX, Real LengthZ, Real height_in, Real voffset, Real tw, Real th, bool inside = true, bool outside = true, bool top = true, bool bottom = true, bool autosize = true);
	Wall* AddTriangleWall(MeshObject* mesh, const std::string &name, const std::string &texture, const std::string &side_texture, Real thickness, Real x1, Real y1, Real z1, Real x2, Real y2, Real z2, Real x3, Real y3, Real z3, Real tw, Real th, Real side_tw, Real side_th, bool autosize = true);
	Wall* AddCustomWall(MeshObject* mesh, const std::string &name, const std::string &texture, const std::string &side_texture, Real thickness, PolyArray &varray, Real tw, Real th, Real side_tw, Real side_th, bool autosize = true);
	Wall* AddCustomFloor(MeshObject* mesh, const std::string &name, const std::string &texture, const std::string &side_texture, Real thickness, std::vector<Vector2> &varray, Real altitude, Real tw, Real th, Real side_tw, Real side_th, bool autosize = true);
	void AddPolygon(Wall* wallobject, const std::string &texture, const std::string &side_texture, Real thickness, PolyArray &varray, Real tw, Real th, Real side_tw, Real side_th, bool autosize = true, bool report = true);
	bool SetWallOrientation(std::string direction);
	int GetWallOrientation();
	bool SetFloorOrientation(std::string direction);
	int GetFloorOrientation();
	void DrawWalls(bool MainN, bool MainP, bool SideN, bool SideP, bool Top, bool Bottom);
	void ResetWalls(bool ToDefaults = false);
	int GetDrawWallsCount();
	Wall* AddWall(MeshObject* mesh, const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real height_in1, Real height_in2, Real altitude1, Real altitude2, Real tw, Real th);
	Wall* AddFloor(MeshObject* mesh, const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real altitude1, Real altitude2, bool reverse_axis, bool texture_direction, Real tw, Real th, bool legacy_behavior = false);
	Wall* AddGround(const std::string &name, const std::string &texture, Real x1, Real z1, Real x2, Real z2, Real altitude, int tile_x, int tile_z);
	int GetWallCount();
	int GetPolygonCount();
	void ExtrudePolygon(PolyArray &polygon, Real thickness, PolygonSet &output_faces);
	Vector2 GetExtents(PolyArray &varray, int coord, bool flip_z = false);
	void Cut(Wall *wall, Vector3 start, Vector3 end, bool cutwalls, bool cutfloors, int checkwallnumber = 0, bool reset_check = true);
	void CutOrig(Wall *wall, Vector3 start, Vector3 end, bool cutwalls, bool cutfloors, int checkwallnumber = 0, bool reset_check = true);
	void GetDoorwayExtents(MeshObject *mesh, int checknumber, PolyArray &polygon);
	Vector3 GetPolygonDirection(PolyArray &polygon);
	Vector2 GetEndPoint(const Vector2 &StartPoint, Real angle, Real distance);
	Plane ComputePlane(PolyArray &vertices, bool flip_normal = true);
	void SplitWithPlane(int axis, const PolyArray &orig, PolyArray &poly1, PolyArray &poly2, Real value);
	void SplitWithPlaneUV(int axis, const GeometryArray &orig, GeometryArray &polyLE, GeometryArray &polyGE, Real value, bool biasToGE);
	Vector3 ComputeNormal(PolyArray &vertices, Real &D);
	Vector3 ComputeNormal2(const PolyArray &vertices, Real &D);
	void ResetDoorwayWalls();
	Wall* AddDoorwayWalls(MeshObject* mesh, const std::string &wallname, const std::string &texture, Real tw, Real th);
	bool IntersectRayTri(const Vector3& ro, const Vector3& rd, const Vector3& a, const Vector3& b, const Vector3& c, double& t, double& u, double& v);
	MeshObject::TriOwner FindWallIntersect_Tri(MeshObject* mesh, const Vector3& start, const Vector3& end, Vector3& isect, Real& distance, Vector3& normal);

private:

	//orientations
	int wall_orientation;
	int floor_orientation;

	//wall/floor sides
	bool DrawMainN; //or top, if floor
	bool DrawMainP; //or bottom, if floor
	bool DrawSideN;
	bool DrawSideP;
	bool DrawTop; //or back, if floor
	bool DrawBottom; //or front, if floor

	//old wall/floor sides
	bool DrawMainNOld; //or top, if floor
	bool DrawMainPOld; //or bottom, if floor
	bool DrawSideNOld;
	bool DrawSidePOld;
	bool DrawTopOld; //or back, if floor
	bool DrawBottomOld; //or front, if floor

	//doorway data
	bool wall1a, wall1b, wall2a, wall2b;
	Vector2 wall_extents_x, wall_extents_z, wall_extents_y;
};

}

#endif
