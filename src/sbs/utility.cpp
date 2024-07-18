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

#include "globals.h"
#include "sbs.h"
#include "wall.h"
#include "polygon.h"
#include "utility.h"

namespace SBS {

std::mutex report_lock;

Utility::Utility(Object *parent) : ObjectBase(parent)
{
	ResetDoorwayWalls();

	temppoly.reserve(32);
	temppoly2.reserve(32);
	temppoly3.reserve(32);
	temppoly4.reserve(32);
	temppoly5.reserve(32);
	worker.reserve(32);
}

Utility::~Utility()
{

}

Vector2 Utility::GetExtents(PolyArray &varray, int coord, bool flip_z)
{
	//returns the smallest and largest values from a specified coordinate type
	//(x, y, or z) from a vertex array (polygon).
	//first parameter must be a vertex array
	//second must be either 1 (for x), 2 (for y) or 3 (for z)

	Real esmall = 0;
	Real ebig = 0;
	Real tempnum = 0;
	size_t i = 0;

	//return 0,0 if coord value is out of range
	if (coord < 1 || coord > 3)
		return Vector2(0, 0);

	for (i = 0; i < varray.size(); i++)
	{
		if (coord == 1)
			tempnum = varray[i].x;
		if (coord == 2)
			tempnum = varray[i].y;
		if (coord == 3)
		{
			if (flip_z == false)
				tempnum = varray[i].z;
			else
				tempnum = -varray[i].z;
		}

		if (i == 0)
		{
			esmall = tempnum;
			ebig = tempnum;
		}
		else
		{
			if (tempnum < esmall)
				esmall = tempnum;
			if (tempnum > ebig)
				ebig = tempnum;
		}
	}

	return Vector2(esmall, ebig);
}

void Utility::Cut(Wall *wall, Vector3 start, Vector3 end, bool cutwalls, bool cutfloors, int checkwallnumber, bool reset_check)
{
	//cuts a rectangular hole in the polygons within the specified range

	if (cutwalls == false && cutfloors == false)
		return;

	//swap values if the first is greater than the second
	if (start.x > end.x)
		std::swap(start.x, end.x);
	if (start.y > end.y)
		std::swap(start.y, end.y);
	if (start.z > end.z)
		std::swap(start.z, end.z);

	bool polycheck = false;

	if (reset_check == true)
	{
		if (checkwallnumber == 1)
		{
			wall1a = false;
			wall1b = false;
		}
		if (checkwallnumber == 2)
		{
			wall2a = false;
			wall2b = false;
		}
	}

	//step through each polygon
	int polycount = wall->GetPolygonCount();
	for (int i = 0; i < polycount; i++)
	{
		//get name
		Polygon *polygon = wall->GetPolygon(i);

		newpolys.clear();

		//skip empty polygons
		if (polygon->geometry.size() == 0)
			continue;

		//cut all polygons within range
		for (size_t j = 0; j < polygon->geometry.size(); j++)
		{
			//skip null geometry
			if (polygon->geometry[j].size() == 0)
				continue;

			temppoly.clear();
			temppoly2.clear();
			temppoly3.clear();
			temppoly4.clear();
			temppoly5.clear();
			worker.clear();
			Vector2 extentsx, extentsy, extentsz;
			Ogre::AxisAlignedBox bounds (start, end);
			Ogre::AxisAlignedBox polybounds;
			bool polycheck2 = false;

			//copy source polygon vertices
			for (size_t k = 0; k < polygon->geometry[j].size(); k++)
			{
				Ogre::Vector3 vertex = sbs->ToLocal(polygon->geometry[j][k].vertex);
				temppoly.push_back(vertex);
				polybounds.merge(vertex);
			}

			//skip if the polygon is completely inside the bounding box
			/*if (bounds.contains(polybounds) == true)
			{
				polycheck = true;
				continue;
			}*/

			//make sure the polygon intersects bounds (is not outside the cut area)
			if (bounds.intersects(polybounds) == true)
			{
				extentsx = GetExtents(temppoly, 1);
				extentsy = GetExtents(temppoly, 2);
				extentsz = GetExtents(temppoly, 3);

				//is polygon a wall?
				if (extentsy.x != extentsy.y)
				{
					if (cutwalls == true)
					{
						//wall
						if (std::abs(extentsx.x - extentsx.y) > std::abs(extentsz.x - extentsz.y))
						{
							//wall is facing forward/backward

							//get left side
							worker = temppoly;
							SplitWithPlane(0, worker, temppoly, temppoly2, start.x);
							worker.clear();

							//get right side
							if (temppoly2.size() > 0)
								worker = temppoly2;
							else
								worker = temppoly;
							SplitWithPlane(0, worker, temppoly3, temppoly2, end.x);
							worker.clear();

							//get lower
							if (temppoly3.size() > 0)
								worker = temppoly3;
							else if (temppoly2.size() > 0)
								worker = temppoly2;
							else if (temppoly.size() > 0)
								worker = temppoly;
							SplitWithPlane(1, worker, temppoly3, temppoly4, start.y);
							worker.clear();

							//get upper
							if (temppoly4.size() > 0)
								worker = temppoly4;
							else if (temppoly3.size() > 0)
								worker = temppoly3;
							else if (temppoly2.size() > 0)
								worker = temppoly2;
							else if (temppoly.size() > 0)
								worker = temppoly;
							SplitWithPlane(1, worker, temppoly5, temppoly4, end.y);
							worker.clear();
						}
						else
						{
							//wall is facing left/right

							//get left side
							worker = temppoly;
							SplitWithPlane(2, worker, temppoly, temppoly2, start.z);
							worker.clear();

							//get right side
							if (temppoly2.size() > 0)
								worker = temppoly2;
							else
								worker = temppoly;
							SplitWithPlane(2, worker, temppoly3, temppoly2, end.z);
							worker.clear();

							//get lower
							if (temppoly3.size() > 0)
								worker = temppoly3;
							else if (temppoly2.size() > 0)
								worker = temppoly2;
							else if (temppoly.size() > 0)
								worker = temppoly;
							SplitWithPlane(1, worker, temppoly3, temppoly4, start.y);
							worker.clear();

							//get upper
							if (temppoly4.size() > 0)
								worker = temppoly4;
							else if (temppoly3.size() > 0)
								worker = temppoly3;
							else if (temppoly2.size() > 0)
								worker = temppoly2;
							else if (temppoly.size() > 0)
								worker = temppoly;
							SplitWithPlane(1, worker, temppoly5, temppoly4, end.y);
							worker.clear();
						}
						polycheck = true;
						polycheck2 = true;

						//store extents of temppoly5 for door sides if needed
						GetDoorwayExtents(wall->GetMesh(), checkwallnumber, temppoly5);
					}
				}
				else if (cutfloors == true)
				{
					//floor

					//get left side
					worker = temppoly;
					SplitWithPlane(0, worker, temppoly, temppoly2, start.x);
					worker.clear();

					//get right side
					if (temppoly2.size() > 0)
						worker = temppoly2;
					else
						worker = temppoly;
					SplitWithPlane(0, worker, temppoly3, temppoly2, end.x);
					worker.clear();

					//get lower
					if (temppoly3.size() > 0)
						worker = temppoly3;
					else if (temppoly2.size() > 0)
						worker = temppoly2;
					else if (temppoly.size() > 0)
						worker = temppoly;
					SplitWithPlane(2, worker, temppoly3, temppoly4, start.z);
					worker.clear();

					//get upper
					if (temppoly4.size() > 0)
						worker = temppoly4;
					else if (temppoly3.size() > 0)
						worker = temppoly3;
					else if (temppoly2.size() > 0)
						worker = temppoly2;
					else if (temppoly.size() > 0)
						worker = temppoly;
					SplitWithPlane(2, worker, temppoly5, temppoly4, end.z);
					worker.clear();
					temppoly5.clear();

					polycheck = true;
					polycheck2 = true;
				}

				//create split polygons
				if (polycheck2 == true)
				{
					if (temppoly.size() > 2)
					{
						newpolys.resize(newpolys.size() + 1);
						if (newpolys[newpolys.size() - 1].capacity() < temppoly.size())
							newpolys[newpolys.size() - 1].reserve(temppoly.size());
						for (size_t k = 0; k < temppoly.size(); k++)
							newpolys[newpolys.size() - 1].push_back(temppoly[k]);
					}
					if (temppoly2.size() > 2)
					{
						newpolys.resize(newpolys.size() + 1);
						if (newpolys[newpolys.size() - 1].capacity() < temppoly2.size())
							newpolys[newpolys.size() - 1].reserve(temppoly2.size());
						for (size_t k = 0; k < temppoly2.size(); k++)
							newpolys[newpolys.size() - 1].push_back(temppoly2[k]);
					}
					if (temppoly3.size() > 2)
					{
						newpolys.resize(newpolys.size() + 1);
						if (newpolys[newpolys.size() - 1].capacity() < temppoly3.size())
							newpolys[newpolys.size() - 1].reserve(temppoly3.size());
						for (size_t k = 0; k < temppoly3.size(); k++)
							newpolys[newpolys.size() - 1].push_back(temppoly3[k]);
					}
					if (temppoly4.size() > 2)
					{
						newpolys.resize(newpolys.size() + 1);
						if (newpolys[newpolys.size() - 1].capacity() < temppoly4.size())
							newpolys[newpolys.size() - 1].reserve(temppoly4.size());
						for (size_t k = 0; k < temppoly4.size(); k++)
							newpolys[newpolys.size() - 1].push_back(temppoly4[k]);
					}

					temppoly.clear();
					temppoly2.clear();
					temppoly3.clear();
					temppoly4.clear();
				}
			}
			else
			{
				//otherwise put original polygon into array (will only be used if the related submesh is recreated)
				PolyArray poly;
				for (size_t k = 0; k < polygon->geometry[j].size(); k++)
				{
					poly.push_back(sbs->ToLocal(polygon->geometry[j][k].vertex));
				}
				newpolys.push_back(poly);
			}
		}

		//create new polygon
		if (polycheck == true)
		{
			std::string oldmat;
			Vector3 oldvector;
			Matrix3 mapping;
			std::string name = polygon->GetName();

			if (newpolys.size() > 0)
			{
				//get texture data from original polygon
				oldmat = polygon->material;
				polygon->GetTextureMapping(mapping, oldvector);
			}

			//delete original polygon
			wall->DeletePolygon(i, false);
			polygon = 0;

			//create new polygon
			if (newpolys.size() > 0)
				wall->AddPolygonSet(name, oldmat, newpolys, mapping, oldvector);

			//reset search position
			i--;
			polycount--;
			polycheck = false;
		}
	}
}

void Utility::GetDoorwayExtents(MeshObject *mesh, int checknumber, PolyArray &polygon)
{
	//calculate doorway extents, for use with AddDoorwayWalls function
	//checknumber is 1 when checking shaft walls, and 2 when checking floor walls

	if (checknumber > 0 && checknumber < 3)
	{
		Vector3 mesh_position = mesh->GetPosition();
		Real extent;

		if (wall2a == false || wall2b == false)
		{
			if (checknumber == 2)
			{
				//level walls
				if (wall2a == true)
					wall2b = true;
				wall2a = true;
				extent = GetExtents(polygon, 1).x + mesh_position.x;
				if (wall2b == false || (wall2b == true && std::abs(extent - mesh_position.x) > std::abs(wall_extents_x.x - mesh_position.x)))
					wall_extents_x.x = extent;
				extent = GetExtents(polygon, 3).x + mesh_position.z;
				if (wall2b == false || (wall2b == true && std::abs(extent - mesh_position.z) > std::abs(wall_extents_z.x - mesh_position.z)))
					wall_extents_z.x = extent;
				wall_extents_y = GetExtents(polygon, 2) + mesh_position.y;
			}
			else
			{
				//shaft walls
				if (wall1a == true)
					wall1b = true;
				wall1a = true;
				extent = GetExtents(polygon, 1).y + mesh_position.x;
				if (wall1b == false || (wall1b == true && std::abs(extent - mesh_position.x) < std::abs(wall_extents_x.y - mesh_position.x)))
					wall_extents_x.y = extent;
				extent = GetExtents(polygon, 3).y + mesh_position.z;
				if (wall1b == false || (wall1b == true && std::abs(extent - mesh_position.z) < std::abs(wall_extents_z.y - mesh_position.z)))
					wall_extents_z.y = extent;
			}
		}
	}
}

Vector3 Utility::GetPolygonDirection(PolyArray &polygon)
{
	//returns the direction the polygon faces, in a 3D vector
	//for example, <-1, 0, 0> means that the wall faces left.

	//get largest normal

	//convert to remote values for precision compatibility with Alpha 7 and earlier
	newpoly.clear();
	for (size_t i = 0; i < polygon.size(); i++)
		newpoly.push_back(sbs->ToRemote(polygon[i], true, false));

	Vector3 normal = ComputePlane(newpoly, false).normal;

	int largest_normal = 0; //x

	if (std::abs(normal.y) >= std::abs(normal.x) && std::abs(normal.y) >= std::abs(normal.z))
		largest_normal = 1; //y biggest
	else if (std::abs(normal.z) > std::abs(normal.x))
		largest_normal = 2; //z biggest

	if (largest_normal == 0)
	{
		if (normal.x < 0)
			return Vector3(1, 0, 0);
		else
			return Vector3(-1, 0, 0);
	}

	if (largest_normal == 1)
	{
		if (normal.y < 0)
			return Vector3(0, 1, 0);
		else
			return Vector3(0, -1, 0);
	}

	if (largest_normal == 2)
	{
		if (normal.z < 0)
			return Vector3(0, 0, 1);
		else
			return Vector3(0, 0, -1);
	}
	return Vector3(0, 0, 0);
}

Vector2 Utility::GetEndPoint(const Vector2 &StartPoint, Real angle, Real distance)
{
	//calculate 2D endpoint from given starting point, along with angle and distance (magnitude)
	//angle is in degrees

	angle -= 90;
	Real newangle = DegreesToRadians(angle);
	Real x = StartPoint.x + distance * cos(newangle);
	Real y = -StartPoint.y + distance * sin(newangle);
	Vector2 result (x, -y);
	return result;

}

Plane Utility::ComputePlane(PolyArray &vertices, bool flip_normal)
{
	//compute plane from a set of given vertices

	Real det;
	Vector3 normal;
	if (flip_normal == true)
		normal = -ComputeNormal(vertices, det);
	else
		normal = ComputeNormal(vertices, det);
	normal.normalise();
	return Plane(normal, det);
}

void Utility::ResetDoorwayWalls()
{
	wall1a = false;
	wall1b = false;
	wall2a = false;
	wall2b = false;
	wall_extents_x = Vector2::ZERO;
	wall_extents_y = Vector2::ZERO;
	wall_extents_z = Vector2::ZERO;
}

Wall* Utility::AddDoorwayWalls(MeshObject* mesh, const std::string &wallname, const std::string &texture, Real tw, Real th)
{
	//add joining doorway polygons if needed

	if (!mesh)
		return 0;

	if (wall1a == true && wall2a == true)
	{
		Wall *wall = mesh->CreateWallObject(wallname);

		//convert extents to relative positioning
		Vector2 extents_x = wall_extents_x - wall->GetMesh()->GetPosition().x;
		Vector2 extents_y = wall_extents_y - wall->GetMesh()->GetPosition().y;
		Vector2 extents_z = wall_extents_z - wall->GetMesh()->GetPosition().z;

		//true if doorway is facing forward/backward
		//false if doorway is facing left/right
		bool direction = std::abs(extents_x.x - extents_x.y) > std::abs(extents_z.x - extents_z.y);

		sbs->DrawWalls(false, true, false, false, false, false);
		if (direction == true)
			sbs->AddWallMain(wall, "DoorwayLeft", texture, 0, extents_x.x, extents_z.x, extents_x.x, extents_z.y, extents_y.y - extents_y.x, extents_y.y - extents_y.x, extents_y.x, extents_y.x, tw, th, true);
		else
			sbs->AddWallMain(wall, "DoorwayLeft", texture, 0, extents_x.x, extents_z.x, extents_x.y, extents_z.x, extents_y.y - extents_y.x, extents_y.y - extents_y.x, extents_y.x, extents_y.x, tw, th, true);
		sbs->ResetWalls();

		sbs->DrawWalls(true, false, false, false, false, false);
		if (direction == true)
			sbs->AddWallMain(wall, "DoorwayRight", texture, 0, extents_x.y, extents_z.x, extents_x.y, extents_z.y, extents_y.y - extents_y.x, extents_y.y - extents_y.x, extents_y.x, extents_y.x, tw, th, true);
		else
			sbs->AddWallMain(wall, "DoorwayRight", texture, 0, extents_x.x, extents_z.y, extents_x.y, extents_z.y, extents_y.y - extents_y.x, extents_y.y - extents_y.x, extents_y.x, extents_y.x, tw, th, true);

		sbs->AddFloorMain(wall, "DoorwayTop", texture, 0, extents_x.x, extents_z.x, extents_x.y, extents_z.y, extents_y.y, extents_y.y, false, false, tw, th, true);
		sbs->ResetWalls();

		ResetDoorwayWalls();

		return wall;
	}

	return 0;
}

void Utility::Report(const std::string &message)
{
	report_lock.lock();

	log_queue_data data;
	data.text = message;
	data.error = false;
	log_queue.push_back(data);

	sbs->LastNotification = message;

	report_lock.unlock();
}

bool Utility::ReportError(const std::string &message)
{
	report_lock.lock();

	log_queue_data data;
	data.text = message;
	data.error = true;
	log_queue.push_back(data);

	sbs->LastError = message;

	report_lock.unlock();
	return false;
}

void Utility::ProcessLog()
{
	report_lock.lock();

	while (log_queue.size() > 0)
	{
		try
		{
			Ogre::LogMessageLevel level = Ogre::LML_NORMAL;
			if (log_queue[0].error == true)
				level = Ogre::LML_CRITICAL;

			if (Ogre::LogManager::getSingletonPtr())
				Ogre::LogManager::getSingleton().logMessage(log_queue[0].text, level);
		}
		catch (Ogre::Exception &e)
		{

		}

		//erase queue entry
		log_queue.erase(log_queue.begin());
	}

	report_lock.unlock();
}

}
