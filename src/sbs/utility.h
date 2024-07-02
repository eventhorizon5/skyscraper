/*
	Scalable Building Simulator - Utility Object
	The Skyscraper Project - Version 1.12 Alpha
	Copyright (C)2004-2024 Ryan Thoryk
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

namespace SBS {

class SBSIMPEXP Utility : public ObjectBase
{
public:
	Utility(Object *parent);
	~Utility();
	Vector2 GetExtents(PolyArray &varray, int coord, bool flip_z = false);
	void Cut(Wall *wall, Vector3 start, Vector3 end, bool cutwalls, bool cutfloors, int checkwallnumber = 0, bool reset_check = true);
	void GetDoorwayExtents(MeshObject *mesh, int checknumber, PolyArray &polygon);
	Vector3 GetPolygonDirection(PolyArray &polygon);
	Vector2 GetEndPoint(const Vector2 &StartPoint, Real angle, Real distance);
	Plane ComputePlane(PolyArray &vertices, bool flip_normal = true);
	void SplitWithPlane(int axis, PolyArray &orig, PolyArray &poly1, PolyArray &poly2, Real value);
	Vector3 ComputeNormal(PolyArray &vertices, Real &D);
	void ResetDoorwayWalls();
	Wall* AddDoorwayWalls(MeshObject* mesh, const std::string &wallname, const std::string &texture, Real tw, Real th);

private:

	//Cut function work polygons
	PolyArray temppoly, temppoly2, temppoly3, temppoly4, temppoly5, worker;
	PolygonSet newpolys;

	PolyArray newpoly;

	//doorway data
	bool wall1a, wall1b, wall2a, wall2b;
	Vector2 wall_extents_x, wall_extents_z, wall_extents_y;
};

}
