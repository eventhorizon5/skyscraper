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

#include "globals.h"
#include "sbs.h"
#include "polymesh.h"
#include "wall.h"
#include "polygon.h"
#include "utility.h"

namespace SBS {

std::mutex report_lock;

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
        //only allow one thread at a time to run ProcessLog()
        std::lock_guard<std::mutex> guard(report_lock);

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

}
