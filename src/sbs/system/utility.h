/*
	Scalable Building Simulator - Utility Object
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

#ifndef _SBS_UTILITY_H
#define _SBS_UTILITY_H

namespace SBS {

class SBSIMPEXP Utility : public ObjectBase
{
	friend class SBS;

public:

	Real UnitScale; //scale of 3D positions; this value equals 1 3D unit

	explicit Utility(Object *parent);
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
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	void ProcessLog();
	Real MetersToFeet(Real meters); //converts meters to feet
	Real FeetToMeters(Real feet); //converts feet to meters
	Vector3 ToGlobal(const Vector3 &position);
	Vector3 FromGlobal(const Vector3 &position);
	Quaternion ToGlobal(const Quaternion &orientation);
	Quaternion FromGlobal(const Quaternion &orientation);
	std::string VerifyFile(const std::string &filename);
	std::string VerifyFile(std::string filename, bool &result, bool skip_cache);
	bool FileExists(const std::string &filename);
	Real ToLocal(Real remote_value);
	Vector2 ToLocal(const Vector2& remote_value);
	Vector3 ToLocal(const Vector3& remote_value, bool rescale = true, bool flip_z = true);
	Real ToRemote(Real local_value);
	Vector2 ToRemote(const Vector2& local_value);
	Vector3 ToRemote(const Vector3& local_value, bool rescale = true, bool flip_z = true);
	bool Mount(const std::string &filename, const std::string &path);
	Real GetDistance(Real x1, Real x2, Real z1, Real z2);
	std::string GetFilesystemPath(std::string filename);
	std::string GetMountPath(std::string filename, std::string &newfilename);
	void CacheFilename(const std::string &filename, const std::string &result);

	//EnableArray() function
	template <typename T>
	bool EnableArray(const std::vector<T> &array, bool value)
	{
		//enable or disable an object array

		bool status = true;
		for (size_t i = 0; i < array.size(); i++)
		{
			if (array[i])
			{
				bool result = array[i]->Enabled(value);
				if (!result)
					status = false;
			}
		}
		return status;
	}

private:

	//Cut function work polygons
	PolyArray temppoly, temppoly2, temppoly3, temppoly4, temppoly5, worker;
	PolygonSet newpolys;

	PolyArray newpoly;

	//doorway data
	bool wall1a, wall1b, wall2a, wall2b;
	Vector2 wall_extents_x, wall_extents_z, wall_extents_y;

	struct log_queue_data
	{
		std::string text;
		bool error;
	};
	std::vector<log_queue_data> log_queue;

	struct VerifyResult
	{
		std::string filename;
		std::string result;
	};
	std::vector<VerifyResult> verify_results;

	//file listing cache
	Ogre::StringVectorPtr filesystem_listing;
};

}

#endif
