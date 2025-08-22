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
public:

	Real UnitScale; //scale of 3D positions; this value equals 1 3D unit

	explicit Utility(Object *parent);
	~Utility();
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
	bool GetFloorFromID(const std::string &floor, int &result);
	std::string ProcessFullName(std::string name, int &instance, int &object_number, bool strip_number = false);
	bool HitBeam(const Ray &ray, Real max_distance, MeshObject *&mesh, Wall *&wall, Polygon *&polygon, Vector3 &hit_position);
	bool InBox(const Vector3 &start, const Vector3 &end, const Vector3 &test);

private:

	struct VerifyResult
	{
		std::string filename;
		std::string result;
	};
	std::vector<VerifyResult> verify_results;

	//file listing cache
	Ogre::StringVectorPtr filesystem_listing;
};

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

//AddArrayElement() function
template <typename T>
bool AddArrayElement(std::vector<T*> &array, T* object, bool check = false)
{
	//add an element to an array

	if (!object)
		return false;

	if (check == true)
	{
		for (size_t i = 0; i < array.size(); i++)
		{
			//exit if already registered
			if (array[i] == object)
				return false;
		}
	}

	array.emplace_back(object);
	return true;
}

//RemoveArrayElement() function
template <typename T>
bool RemoveArrayElement(std::vector<T*> &array, T* object)
{
	if (!object)
		return false;

	if (array.empty())
		return false;

	if (array.back() == object)
	{
		array.pop_back();
		return true;
	}

	for (size_t i = 0; i < array.size(); i++)
	{
		if (array[i] == object)
		{
			array.erase(array.begin() + i);
			return true;
		}
	}
	return false;
}

}

#endif
