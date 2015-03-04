/* $Id$ */

/*
	Scalable Building Simulator - Shaft Class
	The Skyscraper Project - Version 1.9 Alpha
	Copyright (C)2004-2015 Ryan Thoryk
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

Shaft::Shaft(int number, float CenterX, float CenterZ, int _startfloor, int _endfloor)
{
	//constructor
	//creates a shaft in the location specified by x1, x2, z1, and z2
	//and that spans the altitude range specified by startalt and endalt

	//set up SBS object
	object = new Object();
	object->SetValues(this, sbs->object, "Shaft", "", false);

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
	ShowFloors = 0;
	ShowOutside = false;
	ShowInterfloors = false;
	ShowFullShaft = false;
	EnableCheck = false;
	lastcheckresult = false;
	checkfirstrun = true;
	lastposition = 0;
	InElevator = false;
	ShowFloorsFull_Enabled = false;

	std::string buffer, buffer2, buffer3;

	buffer = ToString(number);
	object->SetName(std::string("Shaft " + buffer).c_str());

	ShaftArray.resize(endfloor - startfloor + 1);
	EnableArray.resize(endfloor - startfloor + 1);
	lights.resize(endfloor - startfloor + 1);
	ModelArray.resize(endfloor - startfloor + 1);
	ControlArray.resize(endfloor - startfloor + 1);
	//TriggerArray.resize(endfloor - startfloor + 1);

	for (int i = startfloor; i <= endfloor; i++)
	{
		//Create shaft meshes
		buffer2 = ToString(number);
		buffer3 = ToString(i);
		buffer = "Shaft " + buffer2 + ":" + buffer3;
		TrimString(buffer);
		ShaftArray[i - startfloor] = new MeshObject(object, buffer.c_str());
		EnableArray[i - startfloor] = true;
	}
}

Shaft::~Shaft()
{
	//destructor

	//delete controls
	for (int i = 0; i < (int)ControlArray.size(); i++)
	{
		for (int j = 0; j < (int)ControlArray[i].size(); j++)
		{
			if (ControlArray[i][j])
			{
				ControlArray[i][j]->object->parent_deleting = true;
				delete ControlArray[i][j];
			}
			ControlArray[i][j] = 0;
		}
	}

	//delete triggers
	/*for (int i = 0; i < (int)TriggerArray.size(); i++)
	{
		for (int j = 0; j < (int)TriggerArray[i].size(); j++)
		{
			if (TriggerArray[i][j])
			{
				TriggerArray[i][j]->object->parent_deleting = true;
				delete TriggerArray[i][j];
			}
			TriggerArray[i][j] = 0;
		}
	}*/

	//delete models
	for (int i = 0; i < (int)ModelArray.size(); i++)
	{
		for (int j = 0; j < (int)ModelArray[i].size(); j++)
		{
			if (ModelArray[i][j])
			{
				ModelArray[i][j]->object->parent_deleting = true;
				delete ModelArray[i][j];
			}
			ModelArray[i][j] = 0;
		}
	}

	//delete lights
	for (int i = 0; i < (int)lights.size(); i++)
	{
		for (int j = 0; j < (int)lights[i].size(); j++)
		{
			if (lights[i][j])
			{
				lights[i][j]->object->parent_deleting = true;
				delete lights[i][j];
			}
			lights[i][j] = 0;
		}
	}

	//delete doors
	for (int i = 0; i < (int)DoorArray.size(); i++)
	{
		if (DoorArray[i].object)
		{
			DoorArray[i].object->object->parent_deleting = true;
			delete DoorArray[i].object;
		}
		DoorArray[i].object = 0;
	}
	DoorArray.clear();

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
		ReportError("AddWall: Floor " + ToString2(floor) + " out of range");
		return 0;
	}

	WallObject *wall = GetMeshObject(floor)->CreateWallObject(object, name);
	sbs->AddWallMain(wall, name, texture, thickness, origin.x + x1, origin.z + z1, origin.x + x2, origin.z + z2, height1, height2, sbs->GetFloor(floor)->Altitude + voffset1, sbs->GetFloor(floor)->Altitude + voffset2, tw, th, true);
	return wall;
}

WallObject* Shaft::AddFloor(int floor, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, bool reverse_axis, bool texture_direction, float tw, float th, bool legacy_behavior)
{
	//exit with an error if floor is invalid
	if (IsValidFloor(floor) == false)
	{
		sbs->ReportError("Shaft " + ToString2(ShaftNumber) + " - AddFloor: Floor " + ToString2(floor) + " out of range");
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

	WallObject *wall = GetMeshObject(floor)->CreateWallObject(object, name);
	sbs->AddFloorMain(wall, name, texture, thickness, origin.x + x1, origin.z + z1, origin.x + x2, origin.z + z2, altitude + voffset1, altitude + voffset2, reverse_axis, texture_direction, tw, th, true, legacy_behavior);
	return wall;
}

void Shaft::Enabled(int floor, bool value, bool EnableShaftDoors)
{
	SBS_PROFILE("Shaft::Enabled");
	if (IsEnabledFloor(floor) != value && floor >= startfloor && floor <= endfloor && EnableCheck == false)
	{
		//turns shaft on/off for a specific floor
		if (value == true)
		{
			GetMeshObject(floor)->Enable(value);
			EnableArray[floor - startfloor] = true;

			//controls
			for (size_t i = 0; i < ControlArray[floor - startfloor].size(); i++)
			{
				if (ControlArray[floor - startfloor][i])
					ControlArray[floor - startfloor][i]->Enabled(true);
			}

			//triggers
			/*for (size_t i = 0; i < TriggerArray[floor - startfloor].size(); i++)
			{
				if (TriggerArray[floor - startfloor][i])
					TriggerArray[floor - startfloor][i]->Enabled(true);
			}*/

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

				//controls
				for (size_t i = 0; i < ControlArray[floor - startfloor].size(); i++)
				{
					if (ControlArray[floor - startfloor][i])
						ControlArray[floor - startfloor][i]->Enabled(false);
				}

				//triggers
				/*for (size_t i = 0; i < TriggerArray[floor - startfloor].size(); i++)
				{
					if (TriggerArray[floor - startfloor][i])
						TriggerArray[floor - startfloor][i]->Enabled(false);
				}*/

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

		if (EnableShaftDoors == true)
		{
			for (size_t i = 0; i < elevators.size(); i++)
			{
				Elevator *elevator = sbs->GetElevator(elevators[i]);
				if (elevator)
				{
					for(size_t j = 0; j < elevator->ServicedFloors.size(); j++)
					{
						if (elevator->ServicedFloors[j] == floor)
							elevator->ShaftDoorsEnabled(0, elevator->ServicedFloors[j], value);
					}
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
		IsEnabled = !value;
	
	if (IsEnabled == !value && EnableCheck == false)
	{
		for (int i = startfloor; i <= endfloor; i++)
		{
			if (force == true)
				EnableArray[i - startfloor] = !value;
			Enabled(i, value, EnableShaftDoors);
		}
	}
	IsEnabled = value;
	if (ShowFullShaft == true)
		EnableCheck = true;
}

bool Shaft::IsInShaft(const Ogre::Vector3 &position)
{
	//SBS_PROFILE("Shaft::IsInShaft");

	//if last position is the same as new, return previous result
	if ((position.x >= (lastposition.x - 0.01)) &&
		(position.y >= (lastposition.y - 0.01)) &&
		(position.z >= (lastposition.z - 0.01)) &&
		(position.x <= (lastposition.x + 0.01)) &&
		(position.y <= (lastposition.y + 0.01)) &&
		(position.z <= (lastposition.z + 0.01)) &&
		checkfirstrun == false)
		return lastcheckresult;

	checkfirstrun = false;

	if (position.y > bottom && position.y < top && ShaftArray.size() > 0)
	{
		//first determine if camera has X and Z values within the first shaft floor's bounding box
		if (ShaftArray[0]->InBoundingBox(position, false) == true)
		{
			bool result = (ShaftArray[0]->HitBeam(position, Ogre::Vector3::NEGATIVE_UNIT_Y, position.y - (bottom - 1)) >= 0);

			//cache values
			lastcheckresult = result;
			lastposition = position;
		
			return result;
		}
	}
	
	//cache values
	lastcheckresult = false;
	lastposition = position;

	return false;
}

void Shaft::CutFloors(bool relative, const Ogre::Vector2 &start, const Ogre::Vector2 &end, float startvoffset, float endvoffset)
{
	//Cut through floor/ceiling polygons on all associated levels, within the voffsets

	Report("cutting...");

	float voffset1, voffset2;
	cutstart = start;
	cutend = end;

	if (!sbs->GetFloor(startfloor) || !sbs->GetFloor(endfloor))
		return;

	for (int i = startfloor; i <= endfloor; i++)
	{
		Floor *floorptr = sbs->GetFloor(i);
		if (!floorptr)
			continue;

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
	voffset2 = sbs->GetFloor(endfloor)->Altitude + endvoffset;

	for (int i = 0; i < (int)sbs->External->Walls.size(); i++)
	{
		if (relative == true)
			sbs->Cut(sbs->External->Walls[i], Ogre::Vector3(origin.x + start.x, voffset1, origin.z + start.y), Ogre::Vector3(origin.x + end.x, voffset2, origin.z + end.y), false, true, Ogre::Vector3(0, 0, 0), Ogre::Vector3(0, 0, 0));
		else
			sbs->Cut(sbs->External->Walls[i], Ogre::Vector3(start.x, voffset1, start.y), Ogre::Vector3(end.x, voffset2, end.y), false, true, Ogre::Vector3(0, 0, 0), Ogre::Vector3(0, 0, 0));
	}
}

bool Shaft::Cut(bool relative, int floor, const Ogre::Vector3 &start, const Ogre::Vector3 &end, bool cutwalls, bool cutfloors, int checkwallnumber)
{
	//Cut through a wall segment
	//the Y values in start and end are both relative to the floor's altitude

	//exit with an error if floor is invalid
	if (IsValidFloor(floor) == false)
	{
		if (sbs->Verbose)
			ReportError("Cut: Floor " + ToString2(floor) + " out of range");
		else
			sbs->LastError = "Cut: Floor " + ToString2(floor) + " out of range";
		return false;
	}

	if (!sbs->GetFloor(floor))
		return false;

	float base = sbs->GetFloor(floor)->Altitude;

	for (int i = 0; i < (int)GetMeshObject(floor)->Walls.size(); i++)
	{
		bool reset = true;
		if (i > 0)
			reset = false;

		if (relative == true)
			sbs->Cut(GetMeshObject(floor)->Walls[i], Ogre::Vector3(origin.x + start.x, base + start.y, origin.z + start.z), Ogre::Vector3(origin.x + end.x, base + end.y, origin.z + end.z), cutwalls, cutfloors, Ogre::Vector3(0, 0, 0), origin, checkwallnumber, reset);
		else
			sbs->Cut(GetMeshObject(floor)->Walls[i], Ogre::Vector3(start.x, base + start.y, start.z), Ogre::Vector3(end.x, base + end.y, end.z), cutwalls, cutfloors, Ogre::Vector3(0, 0, 0), origin, checkwallnumber, reset);
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

	if (!sbs->GetFloor(floor))
		return;

	SBS_PROFILE("Shaft::EnableRange");

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
	//adds a floor number to the ShowFloors list

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
	//removes a floor number from the ShowFloors list

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

bool Shaft::IsShowFloor(int floor)
{
	//return true if a floor is in the ShowFloors list

	for (int i = 0; i < (int)ShowFloorsList.size(); i++)
	{
		if (ShowFloorsList[i] == floor)
			return true;
	}
	return false;
}

void Shaft::AddShowOutside(int floor)
{
	//adds a floor number to the ShowOutside list

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
	//removes a floor number from the ShowOutside list

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

bool Shaft::IsShowOutside(int floor)
{
	//return true if a floor is in the ShowOutside list

	for (int i = 0; i < (int)ShowOutsideList.size(); i++)
	{
		if (ShowOutsideList[i] == floor)
			return true;
	}
	return false;
}

void Shaft::AddShowInterfloor(int floor)
{
	//adds a floor number to the ShowInterfloors list

	int index = -1;
	for (int i = 0; i < (int)ShowInterfloorsList.size(); i++)
	{
		if (ShowInterfloorsList[i] == floor)
			index = i;
	}
	if (index == -1)
	{
		ShowInterfloorsList.push_back(floor);
		std::sort(ShowInterfloorsList.begin(), ShowInterfloorsList.end());
	}
}

void Shaft::RemoveShowInterfloor(int floor)
{
	//removes a floor number from the ShowInterfloors list

	int index = -1;
	for (int i = 0; i < (int)ShowInterfloorsList.size(); i++)
	{
		if (ShowInterfloorsList[i] == floor)
			index = i;
	}
	if (index != -1)
	{
		for (int i = 0; i < (int)ShowInterfloorsList.size(); i++)
		{
			if (ShowInterfloorsList[i] == floor)
				ShowInterfloorsList.erase(ShowInterfloorsList.begin() + i);
		}
	}
}

bool Shaft::IsShowInterfloor(int floor)
{
	//return true if a floor is in the ShowInterfloors list

	for (int i = 0; i < (int)ShowInterfloorsList.size(); i++)
	{
		if (ShowInterfloorsList[i] == floor)
			return true;
	}
	return false;
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

void Shaft::RemoveLight(Light *light)
{
	//remove a light reference (does not delete the object itself)
	for (int i = 0; i < (int)lights.size(); i++)
	{
		for (int j = 0; j < (int)lights[i].size(); j++)
		{
			if (lights[i][j] == light)
			{
				lights[i].erase(lights[i].begin() + i);
				return;
			}
		}
	}
}

void Shaft::RemoveModel(Model *model)
{
	//remove a model reference (does not delete the object itself)
	for (int i = 0; i < (int)ModelArray.size(); i++)
	{
		for (int j = 0; j < (int)ModelArray[i].size(); j++)
		{
			if (ModelArray[i][j] == model)
			{
				ModelArray[i].erase(ModelArray[i].begin() + i);
				return;
			}
		}
	}
}

void Shaft::RemoveControl(Control *control)
{
	//remove a control reference (does not delete the object itself)
	for (int i = 0; i < (int)ControlArray.size(); i++)
	{
		for (int j = 0; j < (int)ControlArray[i].size(); j++)
		{
			if (ControlArray[i][j] == control)
			{
				ControlArray[i].erase(ControlArray[i].begin() + i);
				return;
			}
		}
	}
}

void Shaft::RemoveTrigger(Trigger *trigger)
{
	//remove a trigger reference (does not delete the object itself)
	/*for (int i = 0; i < (int)TriggerArray.size(); i++)
	{
		for (int j = 0; j < (int)TriggerArray[i].size(); j++)
		{
			if (TriggerArray[i][j] == trigger)
			{
				TriggerArray[i].erase(TriggerArray[i].begin() + i);
				return;
			}
		}
	}*/
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
	sbs->Report("Shaft " + ToString2(ShaftNumber) + ": " + message);
}

bool Shaft::ReportError(std::string message)
{
	//general reporting function
	return sbs->ReportError("Shaft " + ToString2(ShaftNumber) + ": " + message);
}

Object* Shaft::AddLight(int floor, const char *name, int type, Ogre::Vector3 position, Ogre::Vector3 direction, float color_r, float color_g, float color_b, float spec_color_r, float spec_color_g, float spec_color_b, float spot_inner_angle, float spot_outer_angle, float spot_falloff, float att_range, float att_constant, float att_linear, float att_quadratic)
{
	//add a global light

	//exit if floor is invalid
	if (!IsValidFloor(floor))
		return 0;

	Light* light = new Light(object, name, type, position + Ogre::Vector3(origin.x, sbs->GetFloor(floor)->Altitude, origin.z), direction, color_r, color_g, color_b, spec_color_r, spec_color_g, spec_color_b, spot_inner_angle, spot_outer_angle, spot_falloff, att_range, att_constant, att_linear, att_quadratic);
	lights[floor - startfloor].push_back(light);
	return light->object;
}

Object* Shaft::AddModel(int floor, const char *name, const char *filename, bool center, Ogre::Vector3 position, Ogre::Vector3 rotation, float max_render_distance, float scale_multiplier, bool enable_physics, float restitution, float friction, float mass)
{
	//add a model

	//exit if floor is invalid
	if (!IsValidFloor(floor))
		return 0;

	Model* model = new Model(object, name, filename, center, position + Ogre::Vector3(origin.x, sbs->GetFloor(floor)->Altitude, origin.z), rotation, max_render_distance, scale_multiplier, enable_physics, restitution, friction, mass);
	if (model->load_error == true)
	{
		delete model;
		return 0;
	}
	ModelArray[floor - startfloor].push_back(model);
	return model->object;
}

Object* Shaft::AddControl(int floor, const char *name, const char *sound, const char *direction, float CenterX, float CenterZ, float width, float height, float voffset, std::vector<std::string> &action_names, std::vector<std::string> &textures)
{
	//add a control
	std::vector<Action*> actionnull; //not used
	Control* control = new Control(object, name, sound, action_names, actionnull, textures, direction, width, height, voffset, true);
	control->SetPosition(Ogre::Vector3(CenterX + origin.x, sbs->GetFloor(floor)->Altitude, CenterZ + origin.z));
	ControlArray[floor - startfloor].push_back(control);
	return control->object;
}

Object* Shaft::AddTrigger(int floor, const char *name, const char *sound_file, Ogre::Vector3 &area_min, Ogre::Vector3 &area_max, std::vector<std::string> &action_names)
{
	//triggers disabled for now

	//add a trigger
	/*Trigger* trigger = new Trigger(object, name, sound_file, area_min, area_max, action_names);
	TriggerArray[floor - startfloor].push_back(trigger);
	trigger->SetPosition(Ogre::Vector3(origin.x, sbs->GetFloor(floor)->Altitude, origin.z));
	return trigger->object;*/
	return 0;
}

void Shaft::ReplaceTexture(const std::string &oldtexture, const std::string &newtexture)
{
	for (int i = startfloor; i <= endfloor; i++)	
        	GetMeshObject(i)->ReplaceTexture(oldtexture, newtexture);
}

void Shaft::Init()
{
	//startup initialization of shafts

	if (ShowFullShaft == false)
	{
		EnableWholeShaft(false, true);

		//enable extents
		Enabled(startfloor, true, true);
		Enabled(endfloor, true, true);
	}
	else
		EnableWholeShaft(true, true, true);
}

Object* Shaft::AddDoor(int floor, const char *open_sound, const char *close_sound, bool open_state, const char *texture, float thickness, int direction, float speed, float CenterX, float CenterZ, float width, float height, float voffset, float tw, float th)
{
	//add a door

	//exit with an error if floor is invalid
	if (IsValidFloor(floor) == false)
	{
		ReportError("AddDoor: Floor " + ToString2(floor) + " out of range");
		return 0;
	}

	Floor *floorptr = sbs->GetFloor(floor);
	if (!floorptr)
		return 0;

	float x1, z1, x2, z2;
	//set up coordinates
	if (direction < 5)
	{
		x1 = CenterX;
		x2 = CenterX;
		z1 = CenterZ - (width / 2);
		z2 = CenterZ + (width / 2);
	}
	else
	{
		x1 = CenterX - (width / 2);
		x2 = CenterX + (width / 2);
		z1 = CenterZ;
		z2 = CenterZ;
	}

	//cut area
	sbs->ResetDoorwayWalls();
	if (direction < 5)
	{
		Cut(1, floor, Ogre::Vector3(x1 - 0.5f, voffset, z1), Ogre::Vector3(x2 + 0.5f, voffset + height, z2), true, false, 1);
		floorptr->Cut(Ogre::Vector3(origin.x + x1 - 0.5f, floorptr->GetBase(true) + voffset, origin.z + z1), Ogre::Vector3(origin.x + x2 + 0.5f, floorptr->GetBase(true) + voffset + height, origin.z + z2), true, false, true, 2);
	}
	else
	{
		Cut(1, floor, Ogre::Vector3(x1, voffset, z1 - 0.5f), Ogre::Vector3(x2, voffset + height, z2 + 0.5f), true, false, 1);
		floorptr->Cut(Ogre::Vector3(origin.x + x1, floorptr->GetBase(true) + voffset, origin.z + z1 - 0.5f), Ogre::Vector3(origin.x + x2, floorptr->GetBase(true) + voffset + height, origin.z + z2 + 0.5f), true, false, true, 2);
	}

	//create doorway walls
	WallObject *wall = GetMeshObject(floor)->CreateWallObject(object, "Connection Walls");
	sbs->AddDoorwayWalls(wall, "ConnectionWall", 0, 0);

	DoorArray.resize(DoorArray.size() + 1);
	DoorArray[DoorArray.size() - 1].floornumber = floor;
	std::string shaftnum = ToString(ShaftNumber);
	std::string num = ToString((int)DoorArray.size() - 1);
	DoorArray[DoorArray.size() - 1].object = new Door(object, std::string("Shaft " + shaftnum + ":Door " + num).c_str(), open_sound, close_sound, open_state, texture, thickness, direction, speed, origin.x + CenterX, origin.z + CenterZ, width, height, floorptr->Altitude + floorptr->GetBase(true) + voffset, tw, th);
	floorptr = 0;
	return DoorArray[DoorArray.size() - 1].object->object;
}

void Shaft::EnableDoor(int floor, bool value)
{
	//turn on door(s) on the specified floor

	for (int i = 0; i < (int)DoorArray.size(); i++)
	{
		if (DoorArray[i].object)
		{
			if (DoorArray[i].floornumber == floor && DoorArray[i].object)
				DoorArray[i].object->Enabled(value);
		}
	}
}

Door* Shaft::GetDoor(int number)
{
	//get door object
	if (number < (int)DoorArray.size())
	{
		if (DoorArray[number].object)
			return DoorArray[number].object;
	}

	return 0;
}

void Shaft::RemoveDoor(Door *door)
{
	//remove a door from the array (this does not delete the object)
	for (int i = 0; i < (int)DoorArray.size(); i++)
	{
		if (DoorArray[i].object == door)
		{
			DoorArray.erase(DoorArray.begin() + i);
			return;
		}
	}
}

void Shaft::Check(Ogre::Vector3 position, int current_floor)
{
	Elevator *elevator = sbs->GetElevator(sbs->ElevatorNumber);

	if (!elevator)
		return;

	if (IsInShaft(position) == true)
	{
		if (InsideShaft == false && sbs->InElevator == false)
		{
			//user is in the shaft
			InsideShaft = true;
			sbs->InShaft = true;
			InElevator = false;

			//turn on entire shaft
			EnableWholeShaft(true, true);
		}
		else if (InsideShaft == true && sbs->InElevator == true)
		{
			//user has moved from the shaft to an elevator
			InsideShaft = false;
			sbs->InShaft = false;
			InElevator = true;

			EnableWholeShaft(ShowFullShaft, true);
		}
		else if (InsideShaft == false && sbs->InElevator == true && ShowFullShaft == false)
		{
			//if user is in an elevator, show a range of the shaft at a time (while it's moving)
			EnableRange(current_floor, sbs->ShaftDisplayRange, true, false);
			elevator->ShaftDoorsEnabledRange(0, current_floor, sbs->ShaftDisplayRange);
		}

		//turn on related floors if ShowFloors is true
		//display a selected range of floors in the floor list if the user is in a moving elevator
		if (ShowFloors == 1 && InsideShaft == false && sbs->InElevator == true && elevator->IsMoving == true && elevator->Leveling == false)
			sbs->EnableFloorRange(current_floor, sbs->FloorDisplayRange, true, true, ShaftNumber);

		//display the full range of floors in the floor list
		if (ShowFloors == 2 && ShowFloorsFull_Enabled == false)
		{
			ShowFloorsFull_Enabled = true;
			for (int i = 0; i < (int)ShowFloorsList.size(); i++)
			{
				Floor *floor = sbs->GetFloor(ShowFloorsList[i]);
				if (floor->IsEnabled == false)
				{
					floor->Enabled(true);
					//floor->EnableGroup(true);
				}
			}
		}

		if (ShowOutside == true)
		{
			if (IsShowOutside(current_floor) == true)
			{
				sbs->EnableSkybox(true);
				sbs->EnableBuildings(true);
				sbs->EnableLandscape(true);
				sbs->EnableExternal(true);
			}
			else
			{
				sbs->EnableSkybox(false);
				sbs->EnableBuildings(false);
				sbs->EnableLandscape(false);
				sbs->EnableExternal(false);
			}
		}

		//display interfloors
		if (ShowInterfloors == true)
		{
			for (int i = 0; i < (int)ShowInterfloorsList.size(); i++)
			{
				Floor *floor = sbs->GetFloor(ShowInterfloorsList[i]);
				if (floor->IsInterfloorEnabled == false)
					floor->EnableInterfloor(true);
			}
		}

	}
	else if (InsideShaft == true || InElevator == true)
	{
		//user has moved out of the shaft
		InsideShaft = false;
		sbs->InShaft = false;
		InElevator = false;

		//turn off shaft
		EnableWholeShaft(false, true, true);

		//disable floors listed in ShowFloors list, when "full" mode is enabled
		if (ShowFloors == 2 && ShowFloorsFull_Enabled == true)
		{
			ShowFloorsFull_Enabled = false;
			for (int i = 0; i < (int)ShowFloorsList.size(); i++)
			{
				Floor *floor = sbs->GetFloor(ShowFloorsList[i]);
				if (floor->IsEnabled == true && sbs->camera->CurrentFloor != floor->Number)
				{
					if ((floor->EnabledGroup == true && floor->EnabledGroup_Floor == sbs->camera->CurrentFloor) == false)
					{
						floor->Enabled(false);
						//floor->EnableGroup(false);
					}
				}
			}
		}

		//disable interfloors
		if (ShowInterfloors == true)
		{
			for (int i = 0; i < (int)ShowInterfloorsList.size(); i++)
			{
				Floor *floor = sbs->GetFloor(ShowInterfloorsList[i]);
				if (floor->IsInterfloorEnabled == true && floor->IsEnabled == false)
					floor->EnableInterfloor(false);
			}
		}
	}
	else if (InsideShaft == false)
	{
		//show specified shaft range if outside the shaft
		EnableRange(current_floor, sbs->ShaftOutsideDisplayRange, true, true);
	}
}
