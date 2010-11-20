/* $Id$ */

/*
	Scalable Building Simulator - Shaft Subsystem Class
	The Skyscraper Project - Version 1.8 Alpha
	Copyright (C)2004-2010 Ryan Thoryk
	http://www.skyscrapersim.com
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@tliquest.net

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
#include "shaft.h"
#include "camera.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

Shaft::Shaft(int number, int type, float CenterX, float CenterZ, int _startfloor, int _endfloor)
{
	//constructor
	//creates a shaft in the location specified by x1, x2, z1, and z2
	//and that spans the altitude range specified by startalt and endalt
	//types are currently:
	//1 = pipe/utility shaft
	//2 = elevator shaft
	//3 = stairwell shaft

	//set up SBS object
	object = new Object();
	object->SetValues(this, sbs->object, "Shaft", "", false);

	ShaftType = type;
	ShaftNumber = number;
	startfloor = _startfloor;
	endfloor = _endfloor;

	//make sure start and ending floors are within a valid range
	if (startfloor < -sbs->Basements)
		return;
	if (endfloor > sbs->Floors - 1)
		return;

	origin = Ogre::Vector3(CenterX, sbs->GetFloor(_startfloor)->Altitude, CenterZ);
	InsideShaft = false;
	IsEnabled = true;
	top = sbs->GetFloor(endfloor)->Altitude + sbs->GetFloor(endfloor)->FullHeight();
	bottom = sbs->GetFloor(startfloor)->Altitude;
	cutstart = 0;
	cutend = 0;
	ShowFloors = false;
	ShowOutside = false;
	ShowFullShaft = false;
	EnableCheck = false;
	lastcheckresult = false;
	checkfirstrun = true;

	std::string buffer, buffer2, buffer3;

	buffer = Ogre::StringConverter::toString(number);
	object->SetName(std::string("Shaft " + buffer).c_str());

	ShaftArray.resize(endfloor - startfloor + 1);
	EnableArray.resize(endfloor - startfloor + 1);
	lights.resize(endfloor - startfloor + 1);
	ModelArray.resize(endfloor - startfloor + 1);

	for (int i = startfloor; i <= endfloor; i++)
	{
		//Create shaft meshes
		buffer2 = Ogre::StringConverter::toString(number);
		buffer3 = Ogre::StringConverter::toString(i);
		buffer = "Shaft " + buffer2 + ":" + buffer3;
		TrimString(buffer);
		ShaftArray[i - startfloor] = new MeshObject(object, buffer.c_str());
		EnableArray[i - startfloor] = true;
	}
}

Shaft::~Shaft()
{
	//destructor

	//delete models
	for (int i = 0; i < (int)ModelArray.size(); i++)
	{
		for (int j = 0; j < (int)ModelArray[i].size(); j++)
		{
			if (ModelArray[i][j])
				delete ModelArray[i][j];
			ModelArray[i][j] = 0;
		}
	}

	//delete lights
	for (int i = 0; i < (int)lights.size(); i++)
	{
		for (int j = 0; j < (int)lights[i].size(); j++)
		{
			if (lights[i][j])
				delete lights[i][j];
			lights[i][j] = 0;
		}
	}

	//delete mesh array objects
	for (int i = 0; i < (int)ShaftArray.size(); i++)
	{
		if (ShaftArray[i])
			delete ShaftArray[i];
		ShaftArray[i] = 0;
	}
	ShaftArray.clear();

	//unregister from parent
	if (sbs->FastDelete == false && object->parent_deleting == false)
		sbs->RemoveShaft(this);

	delete object;
}

WallObject* Shaft::AddWall(int floor, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th)
{
	//exit with an error if floor is invalid
	if (IsValidFloor(floor) == false)
	{
		ReportError("AddWall: Floor " + std::string(_itoa(floor, intbuffer, 10)) + " out of range");
		return 0;
	}

	WallObject *wall = GetMeshObject(floor)->CreateWallObject(this->object, name);
	sbs->AddWallMain(wall, name, texture, thickness, origin.x + x1, origin.z + z1, origin.x + x2, origin.z + z2, height1, height2, sbs->GetFloor(floor)->Altitude + voffset1, sbs->GetFloor(floor)->Altitude + voffset2, tw, th, true);
	return wall;
}

WallObject* Shaft::AddFloor(int floor, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, float tw, float th)
{
	//exit with an error if floor is invalid
	if (IsValidFloor(floor) == false)
	{
		sbs->ReportError("Shaft " + std::string(_itoa(ShaftNumber, intbuffer, 10)) + " - AddFloor: Floor " + std::string(_itoa(floor, intbuffer, 10)) + " out of range");
		return 0;
	}

	//get shaft extents
	float altitude = sbs->GetFloor(floor)->Altitude;

	if (altitude + voffset1 < bottom)
		bottom = altitude + voffset1;
	if (altitude + voffset2 < bottom)
		bottom = altitude + voffset2;
	if (altitude + voffset1 > top)
		top = altitude + voffset1;
	if (altitude + voffset2 > top)
		top = altitude + voffset2;

	WallObject *wall = GetMeshObject(floor)->CreateWallObject(this->object, name);
	sbs->AddFloorMain(wall, name, texture, thickness, origin.x + x1, origin.z + z1, origin.x + x2, origin.z + z2, altitude + voffset1, altitude + voffset2, tw, th, true);
	return wall;
}

void Shaft::Enabled(int floor, bool value, bool EnableShaftDoors)
{
	if (IsEnabledFloor(floor) != value && floor >= startfloor && floor <= endfloor && EnableCheck == false)
	{
		//turns shaft on/off for a specific floor
		if (value == true)
		{
			GetMeshObject(floor)->Enable(value);
			EnableArray[floor - startfloor] = true;

			//models
			for (size_t i = 0; i < ModelArray[floor - startfloor].size(); i++)
			{
				if (ModelArray[floor - startfloor][i])
					ModelArray[floor - startfloor][i]->Enable(true);
			}
		}
		else
		{
			//leave bottom and top on
			if (floor != startfloor && floor != endfloor)
			{
				GetMeshObject(floor)->Enable(value);
				EnableArray[floor - startfloor] = false;

				//models
				for (size_t i = 0; i < ModelArray[floor - startfloor].size(); i++)
				{
					if (ModelArray[floor - startfloor][i])
						ModelArray[floor - startfloor][i]->Enable(false);
				}
			}
			else
				return;
		}
		if (value == false && (floor == startfloor || floor == endfloor))
			return;

		if (EnableShaftDoors == true)
		{
			for (size_t i = 0; i < elevators.size(); i++)
			{
				Elevator *elevator = sbs->GetElevator(elevators[i]);
				for(size_t j = 0; j < elevator->ServicedFloors.size(); j++)
				{
					if (elevator->ServicedFloors[j] == floor)
						elevator->ShaftDoorsEnabled(0, elevator->ServicedFloors[j], value);
				}
			}
		}
	}
}

bool Shaft::IsShaft(Ogre::MeshPtr test)
{
	for (size_t i = 0; i < ShaftArray.size(); i++)
	{
		if (test == ShaftArray[i]->MeshWrapper)
			return true;
	}
	return false;
}

void Shaft::EnableWholeShaft(bool value, bool EnableShaftDoors, bool force)
{
	//turn on/off entire shaft
	
	if (force == true)
		IsEnabled = false;
	
	if (((value == false && IsEnabled == true) || (value == true && IsEnabled == false)) && EnableCheck == false)
	{
		for (int i = startfloor; i <= endfloor; i++)
		{
			if (force == true)
				EnableArray[i - startfloor] = false;
			Enabled(i, value, EnableShaftDoors);
		}
	}
	IsEnabled = value;
	if (ShowFullShaft == true)
		EnableCheck = true;
}

bool Shaft::IsInShaft(const Ogre::Vector3 &position)
{
	//if last position is the same as new, return previous result
	if (position == lastposition && checkfirstrun == false)
		return lastcheckresult;

	checkfirstrun = false;

	if (position.y > bottom && position.y < top)
	{
		/*csHitBeamResult result = ShaftArray[0]->MeshWrapper->HitBeam(sbs->ToRemote(position), sbs->ToRemote(Ogre::Vector3(position.x, position.y - (top - bottom), position.z)));

		//cache values
		lastcheckresult = result.hit;
		lastposition = position;
		
		return result.hit;*/
	}
	
	//cache values
	lastcheckresult = false;
	lastposition = position;

	return false;
}

void Shaft::CutFloors(bool relative, const Ogre::Vector2 &start, const Ogre::Vector2 &end, float startvoffset, float endvoffset)
{
	//Cut through floor/ceiling polygons on all associated levels, within the voffsets

	sbs->Report("Cutting for shaft " + std::string(_itoa(ShaftNumber, intbuffer, 10)) + "...");

	float voffset1, voffset2;
	cutstart = start;
	cutend = end;

	for (int i = startfloor; i <= endfloor; i++)
	{
		Floor *floorptr = sbs->GetFloor(i);
		voffset1 = 0;
		voffset2 = floorptr->FullHeight() + 1;

		if (i == startfloor)
			voffset1 = startvoffset;
		else if (i == endfloor)
			voffset2 = endvoffset;

		if (relative == true)
			floorptr->Cut(Ogre::Vector3(origin.x + start.x, voffset1, origin.z + start.y), Ogre::Vector3(origin.x + end.x, voffset2, origin.z + end.y), false, true, false);
		else
			floorptr->Cut(Ogre::Vector3(start.x, voffset1, start.y), Ogre::Vector3(end.x, voffset2, end.y), false, true, false);
		floorptr = 0;
	}

	//cut external
	voffset1 = sbs->GetFloor(startfloor)->Altitude + startvoffset;
	voffset2 = sbs->GetFloor(endfloor)->Altitude + sbs->GetFloor(endfloor)->FullHeight() + endvoffset;

	for (int i = 0; i < (int)sbs->External->Walls.size(); i++)
	{
		if (relative == true)
			sbs->Cut(sbs->External->Walls[i], Ogre::Vector3(origin.x + start.x, voffset1, origin.z + start.y), Ogre::Vector3(origin.x + end.x, voffset2, origin.z + end.y), false, true, Ogre::Vector3(0, 0, 0), Ogre::Vector3(0, 0, 0));
		else
			sbs->Cut(sbs->External->Walls[i], Ogre::Vector3(start.x, voffset1, start.y), Ogre::Vector3(end.x, voffset2, end.y), false, true, Ogre::Vector3(0, 0, 0), Ogre::Vector3(0, 0, 0));
	}
}

bool Shaft::CutWall(bool relative, int floor, const Ogre::Vector3 &start, const Ogre::Vector3 &end, int checkwallnumber, const char *checkstring)
{
	//Cut through a wall segment
	//the Y values in start and end are both relative to the floor's altitude

	//exit with an error if floor is invalid
	if (IsValidFloor(floor) == false)
	{
		ReportError("CutWall: Floor " + std::string(_itoa(floor, intbuffer, 10)) + " out of range");
		return false;
	}

	float base = sbs->GetFloor(floor)->Altitude;

	for (int i = 0; i < (int)GetMeshObject(floor)->Walls.size(); i++)
	{
		bool reset = true;
		if (i > 0)
			reset = false;

		if (relative == true)
			sbs->Cut(GetMeshObject(floor)->Walls[i], Ogre::Vector3(origin.x + start.x, base + start.y, origin.z + start.z), Ogre::Vector3(origin.x + end.x, base + end.y, origin.z + end.z), true, false, Ogre::Vector3(0, 0, 0), origin, checkwallnumber, checkstring, reset);
		else
			sbs->Cut(GetMeshObject(floor)->Walls[i], Ogre::Vector3(start.x, base + start.y, start.z), Ogre::Vector3(end.x, base + end.y, end.z), true, false, Ogre::Vector3(0, 0, 0), origin, checkwallnumber, checkstring, reset);
	}
	return true;
}

void Shaft::EnableRange(int floor, int range, bool value, bool EnableShaftDoors)
{
	//turn on/off a range of floors
	//if range is 3, show shaft on current floor (floor), and 1 floor below and above (3 total floors)
	//if range is 1, show only the current floor (floor)

	//exit if ShowFullShaft is true
	if (ShowFullShaft == true)
		return;

	//range must be greater than 0
	if (range < 1)
		range = 1;

	//range must be an odd number; if it's even, then add 1
	if (IsEven(range) == true)
		range++;

	int additionalfloors;
	if (range > 1)
		additionalfloors = (range - 1) / 2;
	else
		additionalfloors = 0;

	//disable floors 1 floor outside of range
	if (value == true)
	{
		if (floor - additionalfloors - 1 >= startfloor && floor - additionalfloors - 1 <= endfloor)
		{
			if (sbs->GetFloor(floor)->IsInGroup(floor - additionalfloors - 1) == false) //only disable if not in group
				Enabled(floor - additionalfloors - 1, false, EnableShaftDoors);
		}
		if (floor + additionalfloors + 1 >= startfloor && floor + additionalfloors + 1 <= endfloor)
		{
			if (sbs->GetFloor(floor)->IsInGroup(floor + additionalfloors + 1) == false) //only disable if not in group
				Enabled(floor + additionalfloors + 1, false, EnableShaftDoors);
		}
	}

	//enable floors within range
	for (int i = floor - additionalfloors; i <= floor + additionalfloors; i++)
	{
		if (i >= startfloor && i <= endfloor)
			Enabled(i, value, EnableShaftDoors);
	}
}

bool Shaft::IsEnabledFloor(int floor)
{
	if (floor >= startfloor && floor <= endfloor)
		return EnableArray[floor - startfloor];
	else
		return false;
}

void Shaft::AddShowFloor(int floor)
{
	//adds a floor number to the ShowFloors array

	int index = -1;
	for (int i = 0; i < (int)ShowFloorsList.size(); i++)
	{
		if (ShowFloorsList[i] == floor)
			index = i;
	}
	if (index == -1)
	{
		ShowFloorsList.push_back(floor);
		std::sort(ShowFloorsList.begin(), ShowFloorsList.end());
	}
}

void Shaft::RemoveShowFloor(int floor)
{
	//removes a floor number from the ShowFloors array

	int index = -1;
	for (int i = 0; i < (int)ShowFloorsList.size(); i++)
	{
		if (ShowFloorsList[i] == floor)
			index = i;
	}
	if (index != -1)
	{
		for (int i = 0; i < (int)ShowFloorsList.size(); i++)
		{
			if (ShowFloorsList[i] == floor)
				ShowFloorsList.erase(ShowFloorsList.begin() + i);
		}
	}
}

void Shaft::AddShowOutside(int floor)
{
	//adds a floor number to the ShowFloors array

	int index = -1;
	for (int i = 0; i < (int)ShowOutsideList.size(); i++)
	{
		if (ShowOutsideList[i] == floor)
			index = i;
	}
	if (index == -1)
	{
		ShowOutsideList.push_back(floor);
		std::sort(ShowOutsideList.begin(), ShowOutsideList.end());
	}
}

void Shaft::RemoveShowOutside(int floor)
{
	//removes a floor number from the ShowFloors array

	int index = -1;
	for (int i = 0; i < (int)ShowOutsideList.size(); i++)
	{
		if (ShowOutsideList[i] == floor)
			index = i;
	}
	if (index != -1)
	{
		for (int i = 0; i < (int)ShowOutsideList.size(); i++)
		{
			if (ShowOutsideList[i] == floor)
				ShowOutsideList.erase(ShowOutsideList.begin() + i);
		}
	}
}

bool Shaft::IsValidFloor(int floor)
{
	//return true if the shaft services the specified floor

	if (floor < startfloor || floor > endfloor)
		return false;

	if (!ShaftArray[floor - startfloor])
		return false;

	return true;
}

void Shaft::AddElevator(int number)
{
	//add specified elevator to list
	elevators.push_back(number);
	std::sort(elevators.begin(), elevators.end());
}

void Shaft::RemoveElevator(int number)
{
	//remove specified elevator from list
	for (int i = 0; i < (int)elevators.size(); i++)
	{
		if (elevators[i] == number)
			elevators.erase(elevators.begin() + i);
	}
}

MeshObject* Shaft::GetMeshObject(int floor)
{
	//returns the mesh object for the specified floor

	if (!IsValidFloor(floor))
		return 0;

	return ShaftArray[floor - startfloor];
}

void Shaft::Report(std::string message)
{
	//general reporting function
	sbs->Report("Shaft " + std::string(_itoa(ShaftNumber, intbuffer, 10)) + ": " + message);
}

bool Shaft::ReportError(std::string message)
{
	//general reporting function
	return sbs->ReportError("Shaft " + std::string(_itoa(ShaftNumber, intbuffer, 10)) + ": " + message);
}

Object* Shaft::AddLight(int floor, const char *name, int type, Ogre::Vector3 position, Ogre::Vector3 direction, float radius, float max_distance, float color_r, float color_g, float color_b, float spec_color_r, float spec_color_g, float spec_color_b, float directional_cutoff_radius, float spot_falloff_inner, float spot_falloff_outer)
{
	//add a global light

	//exit if floor is invalid
	if (!IsValidFloor(floor))
		return 0;

	Light* light = new Light(name, type, position + Ogre::Vector3(origin.x, sbs->GetFloor(floor)->Altitude, origin.z), direction, radius, max_distance, color_r, color_g, color_b, spec_color_r, spec_color_g, spec_color_b, directional_cutoff_radius, spot_falloff_inner, spot_falloff_outer);
	lights[floor - startfloor].push_back(light);
	return light->object;
}

Object* Shaft::AddModel(int floor, const char *name, const char *filename, Ogre::Vector3 position, Ogre::Vector3 rotation, float max_render_distance, float scale_multiplier)
{
	//add a model

	//exit if floor is invalid
	if (!IsValidFloor(floor))
		return 0;

	Model* model = new Model(name, filename, position + Ogre::Vector3(origin.x, sbs->GetFloor(floor)->Altitude, origin.z), rotation, max_render_distance, scale_multiplier);
	if (model->load_error == true)
	{
		delete model;
		return 0;
	}
	ModelArray[floor - startfloor].push_back(model);
	return model->object;
}
