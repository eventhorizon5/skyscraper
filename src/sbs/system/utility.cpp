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

#include <OgreBulletDynamicsRigidBody.h>
#include <OgreBulletCollisionsRay.h>
#include "globals.h"
#include "sbs.h"
#include "polymesh.h"
#include "wall.h"
#include "polygon.h"
#include "manager.h"
#include "floor.h"
#include "utility.h"

namespace SBS {

struct PickRays {
    Ray global;  // global space
    Ray engine;  // engine space
};

//build both rays at once from the Ogre camera ray
static PickRays MakePickRays(const Ray& camRay, SBS* sbs)
{
    //start in Ogre camera space (camRay is Ogre/remote)

    //engine space for polygon/triangle picking:
    Ray engineRay(
        sbs->ToRemote(sbs->ToLocal(camRay.getOrigin())),
        sbs->GetOrientation().Inverse() * camRay.getDirection()
    );

    //global space for Bullet
    Ray globalRay(
        sbs->ToRemote(sbs->GetUtility()->ToGlobal(sbs->ToLocal(camRay.getOrigin()))),
        sbs->GetOrientation() * camRay.getDirection()
    );

    return {globalRay, engineRay};
}

Utility::Utility(Object *parent) : ObjectBase(parent)
{
	UnitScale = sbs->GetConfigFloat("Skyscraper.SBS.UnitScale", 4);

	if (UnitScale <= 0)
		UnitScale = 1;
}

Utility::~Utility()
{
	verify_results.clear();
}

Real Utility::MetersToFeet(Real meters)
{
	//converts meters to feet
	return meters * 3.2808399;
}

Real Utility::FeetToMeters(Real feet)
{
	//converts feet to meters
	return feet / 3.2808399;
}

Vector3 Utility::ToGlobal(const Vector3 &position)
{
	//convert an engine-relative position to a global (scene) position

	return (sbs->GetOrientation().Inverse() * position) + sbs->GetPosition();
}

Vector3 Utility::FromGlobal(const Vector3 &position)
{
	//convert a global (scene) position to an engine-relative position

	return (sbs->GetOrientation() * (position - sbs->GetPosition()));
}

Quaternion Utility::ToGlobal(const Quaternion &orientation)
{
	//convert an engine-relative orientation (rotation) to a global (scene) orientation

	return (sbs->GetOrientation() * orientation);
}

Quaternion Utility::FromGlobal(const Quaternion &orientation)
{
	//convert a global (scene) orientation (rotation) to an engine-relative orientation

	return (sbs->GetOrientation().Inverse() * orientation);
}


std::string Utility::VerifyFile(const std::string &filename)
{
	bool result = false;
	return VerifyFile(filename, result, false);
}

std::string Utility::VerifyFile(std::string filename, bool &result, bool skip_cache)
{
	//verify a filename
	//if it exists, return the same filename
	//otherwise search the related folder and find a matching filename with a different
	//case (fixes case-sensitivity issues mainly on Linux)
	//returns the original string if not found
	//"result" will return if the file exists or not, but only accurately if skip_cache is true

	TrimString(filename);
	ReplaceAll(filename, "\\", "/");
	result = false;

	//check for a cached result
	if (skip_cache == false)
	{
		for (size_t i = 0; i < verify_results.size(); i++)
		{
			if (verify_results[i].filename == filename)
				return verify_results[i].result;
		}
	}

	//get filesystem archive
	Ogre::Archive *filesystem = 0;
	Ogre::ArchiveManager::ArchiveMapIterator it = Ogre::ArchiveManager::getSingleton().getArchiveIterator();
	while(it.hasMoreElements())
	{
		const std::string& key = it.peekNextKey();
		filesystem = it.getNext();

		if (!filesystem)
			return "";

		//check for a mount point
		Ogre::StringVectorPtr listing;
		std::string shortname;
		std::string group = GetMountPath(filename, shortname);

		if (group == "General")
		{
			//for the General group, check the native filesystem

			if (filesystem->exists(filename) == true)
			{
				//if exact filename exists, cache and exit
				CacheFilename(filename, filename);
				result = true;
				return filename;
			}

			//otherwise get listing of files to check
			if (!filesystem_listing)
				filesystem_listing = filesystem->list();
			listing = filesystem_listing;
		}
		else
		{
			//for other groups, check resource mount points

			if (Ogre::ResourceGroupManager::getSingleton().resourceExists(group, shortname) == true)
			{
				//if exact filename exists, cache and exit
				CacheFilename(filename, filename);
				result = true;
				return filename;
			}

			//otherwise get listing of files to check
			listing = Ogre::ResourceGroupManager::getSingleton().listResourceNames(group);
		}

		//go through file listing, to find a match with a different case
		for (size_t i = 0; i < listing->size(); i++)
		{
			std::string check = listing->at(i);
			std::string checkoriginal = SetCaseCopy(check, false);
			std::string checkfile = SetCaseCopy(filename, false);
			if (checkoriginal == checkfile)
			{
				//if match is found, cache and exit
				CacheFilename(filename, check);
				result = true;
				return check;
			}
		}
	}

	//if no match is found, cache original name and exit
	CacheFilename(filename, filename);
	return filename;
}

std::string Utility::GetFilesystemPath(std::string filename)
{
	//returns the filesystem path for the specified file
	//the filename needs to be processed by VerifyFile first

	TrimString(filename);
	ReplaceAll(filename, "\\", "/");

	//get filesystem archive
	Ogre::Archive *filesystem = 0;
	Ogre::ArchiveManager::ArchiveMapIterator it = Ogre::ArchiveManager::getSingleton().getArchiveIterator();
	while(it.hasMoreElements())
	{
		const std::string& key = it.peekNextKey();
		filesystem = it.getNext();

		if (!filesystem)
			return "";

		//check for a mount point
		std::string shortname;
		std::string group = GetMountPath(filename, shortname);

		if (group == "General")
		{
			//for the General group, check the native filesystem

			if (filesystem->exists(filename) == true)
			{
				std::string path = key;
				if (key == ".")
					path = "";
				return std::string(path + filename);
			}
		}
	}

	return "";
}

bool Utility::FileExists(const std::string &filename)
{
	//check to see if the specified file exists

	bool result;
	VerifyFile(filename, result, true);

	return result;
}

Real Utility::ToLocal(Real remote_value)
{
	//convert remote (OGRE) vertex positions to local (SBS) positions

	//note - OGRE uses a right-hand coordinate system, while SBS uses left-hand.
	//this means that all Z values that use this function must be inverted.

	return remote_value * UnitScale;
}

Vector2 Utility::ToLocal(const Vector2& remote_value)
{
	//convert remote (OGRE) vertex positions to local (SBS) positions

	//note - OGRE uses a right-hand coordinate system, while SBS uses left-hand.
	//this means that all Z values that use this function must be inverted.

	return remote_value * UnitScale;
}

Vector3 Utility::ToLocal(const Vector3& remote_value, bool rescale, bool flip_z)
{
	//convert remote (OGRE) vertex positions to local (SBS) positions
	//also convert Z value to OGRE's right-hand coordinate system

	Vector3 newvalue;
	newvalue.x = remote_value.x;
	newvalue.y = remote_value.y;

	if (flip_z == true)
		newvalue.z = -remote_value.z; //flip z value for OGRE's right-hand coordinate system
	else
		newvalue.z = remote_value.z;

	if (rescale == true)
		return newvalue * UnitScale;
	else
		return newvalue;
}

Real Utility::ToRemote(Real local_value)
{
	//convert local (SBS) vertex positions to remote (OGRE) positions

	//note - OGRE uses a right-hand coordinate system, while SBS uses left-hand.
	//this means that all Z values that use this function must be inverted.

	return local_value / UnitScale;
}

Vector2 Utility::ToRemote(const Vector2& local_value)
{
	//convert local (SBS) vertex positions to remote (OGRE) positions

	//note - OGRE uses a right-hand coordinate system, while SBS uses left-hand.
	//this means that all Z values that use this function must be inverted.

	return local_value / UnitScale;
}

Vector3 Utility::ToRemote(const Vector3& local_value, bool rescale, bool flip_z)
{
	//convert local (SBS) vertex positions to remote (OGRE) positions

	Vector3 newvalue;
	newvalue.x = local_value.x;
	newvalue.y = local_value.y;

	if (flip_z == true)
		newvalue.z = -local_value.z; //flip z value for OGRE's right-hand coordinate system
	else
		newvalue.z = local_value.z;

	if (rescale == true)
		return (newvalue / UnitScale);
	else
		return newvalue;
}

bool Utility::Mount(const std::string &filename, const std::string &path)
{
	//mounts a zip file into the virtual filesystem

	std::string newfile = "data/" + filename;
	std::string file = VerifyFile(newfile);

	Report("Mounting " + file + " as path " + path);
	try
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(file, "Zip", path, true);
	}
	catch (Ogre::Exception &e)
	{
		return ReportError("Error mounting file " + file + "\n" + e.getDescription());
	}
	return true;
}

Real Utility::GetDistance(Real x1, Real x2, Real z1, Real z2)
{
	//returns the distance between 2 2D vectors

	if (z1 == z2)
		return std::abs(x1 - x2);
	if (x1 == x2)
		return std::abs(z1 - z2);
	if ((x1 != x2) && (z2 != x2))
		return sqrt(pow(std::abs(x1 - x2), 2) + pow(std::abs(z1 - z2), 2)); //calculate diagonals
	return 0;
}

std::string Utility::GetMountPath(std::string filename, std::string &newfilename)
{
	//get mountpoint (resource group) path of given file
	//if not found, return "General"

	Ogre::StringVector list = Ogre::ResourceGroupManager::getSingleton().getResourceGroups();
	ReplaceAll(filename, "\\", "/");
	newfilename = filename;

	for (size_t i = 0; i < list.size(); i++)
	{
		if (StartsWith(filename, list[i] + "/") == true)
		{
			newfilename = filename.substr(list[i].size() + 1);
			return list[i];
		}
	}
	return "General";
}

void Utility::CacheFilename(const std::string &filename, const std::string &result)
{
	//caches filename information for VerifyFile function
	VerifyResult verify;
	verify.filename = filename;
	verify.result = result;
	verify_results.emplace_back(verify);
}

bool Utility::GetFloorFromID(const std::string &floor, int &result)
{
	std::string converted = floor;
	int rawfloor = 0;

	if (IsNumeric(floor))
	{
		rawfloor = ToInt(floor);

		//convert back to string, to strip off any leading 0's
		converted = ToString(rawfloor);
	}

	Floor *floorobj = sbs->GetFloorManager()->GetByNumberID(converted);
	Floor *floorobj2 = sbs->GetFloorManager()->GetByID(converted);
	Floor *floorobj3 = 0;
	if (IsNumeric(floor))
		floorobj3 = sbs->GetFloorManager()->Get(rawfloor);

	if (floorobj)
	{
		result = floorobj->Number; //get by number ID first
		return true;
	}
	else if (floorobj2)
	{
		result = floorobj2->Number; //next try floor ID
		return true;
	}
	else if (floorobj3)
	{
		result = rawfloor; //and last, get by raw floor number
		return true;
	}

	return false;
}

std::string Utility::ProcessFullName(std::string name, int &instance, int &object_number, bool strip_number)
{
	//if given a full object ID name (such as "0:(4)Landscape"),
	//return base name and parse out instance number and object number

	//if strip_number is false, leave object number identifier in string

	//get and strip off engine instance number
	size_t index = name.find(":(");
	instance = ToInt(name.substr(0, index));
	name.erase(name.begin(), name.begin() + index + 1);

	//get and optionally strip off object number
	index = name.find(")");
	object_number = ToInt(name.substr(1, index - 1));

	if (strip_number == true)
		name.erase(name.begin(), name.begin() + index + 1);

	return name;
}

bool Utility::HitBeam(const Ray &ray, Real max_distance, MeshObject *&mesh, Wall *&wall, Polygon *&polygon, Vector3 &hit_position)
{
	//use a given ray and distance, and return the nearest hit mesh and if applicable, wall object
	//note that the ray's origin and direction need to be in engine-relative values

	//create a ray for absolute (global) positioning, and another for engine offsets (engine-relative positioning)
	const PickRays rays = MakePickRays(ray, sbs);
	wall = 0;
	polygon = 0;

    //ray test in Bullet; get a collision callback
    OgreBulletCollisions::CollisionClosestRayResultCallback callback(rays.global, sbs->mWorld, max_distance);

	//check for collision
    sbs->mWorld->launchRay(callback);

	//exit if no collision
    if (!callback.doesCollide())
		return false;

    //get collided collision object
    OgreBulletCollisions::Object* object = callback.getCollidedObject();
    if (!object)
		return false;

	//get name of collision object's grandparent scenenode (which is the same name as the mesh object)
    std::string meshname;
    if (dynamic_cast<OgreBulletDynamics::WheeledRigidBody*>(object) == 0)
        meshname = object->getRootNode()->getParentSceneNode()->getName();
    else
        meshname = object->getRootNode()->getChild(0)->getName(); //for vehicles, the child of the root node is the mesh

	//get associated mesh object
    mesh = sbs->FindMeshObject(meshname);
    if (!mesh)
		return false;

    //hit position — keep space consistent
    hit_position = ToLocal(callback.getCollisionPoint());

    //wall resolution
    Vector3 rs = rays.engine.getOrigin();
    Vector3 re = rays.engine.getPoint(max_distance);

	//get wall object, if any
    Vector3 isect; Real distance = 2e9; Vector3 normal = Vector3::ZERO;
    MeshObject::TriOwner owner = sbs->GetPolyMesh()->FindWallIntersect_Tri(mesh, rs, re, isect, distance, normal);
	wall = owner.wall;
	polygon = owner.poly;

    return true;
}

bool Utility::InBox(const Vector3 &start, const Vector3 &end, const Vector3 &test)
{
	//determine if a point (test) is inside the box defines by start and end vertices

	if (test.x > start.x && test.y > start.y && test.z > start.z && test.x < end.x && test.y < end.y && test.z < end.z)
		return true;
	return false;
}

}
