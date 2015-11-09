/* $Id$ */

/*
	Scalable Building Simulator - Floor Object
	The Skyscraper Project - Version 1.10 Alpha
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
#include "floor.h"
#include "camera.h"
#include "unix.h"

namespace SBS {

Floor::Floor(int number)
{
	//set up SBS object
	SetValues(sbs, "Floor", "", false);

	//Set floor's object number
	Number = number;
	std::string num = ToString(Number);
	SetName("Floor " + num);

	//Create primary level mesh
	Level = new MeshObject(this, "Level " + num);

	//Create interfloor mesh
	Interfloor = new MeshObject(this, "Interfloor " + num);

	//Create columnframe mesh
	ColumnFrame = new MeshObject(this, "ColumnFrame " + num);

	//set enabled flags
	IsEnabled = true;
	IsColumnFrameEnabled = true;
	IsInterfloorEnabled = true;

	//init other variables
	Name = "";
	ID = "";
	FloorType = "";
	Description = "";
	IndicatorTexture = "";
	Altitude = 0;
	Height = 0;
	InterfloorHeight = 0;
	EnabledGroup = false;
	EnabledGroup_Floor = 0;
}

Floor::~Floor()
{
	//Destructor

	//delete escalators
	for (int i = 0; i < (int)EscalatorArray.size(); i++)
	{
		if (EscalatorArray[i])
		{
			EscalatorArray[i]->parent_deleting = true;
			delete EscalatorArray[i];
		}
		EscalatorArray[i] = 0;
	}

	//delete camera textures
	for (int i = 0; i < (int)CameraTextureArray.size(); i++)
	{
		if (CameraTextureArray[i])
		{
			CameraTextureArray[i]->parent_deleting = true;
			delete CameraTextureArray[i];
		}
		CameraTextureArray[i] = 0;
	}

	//delete controls
	for (int i = 0; i < (int)ControlArray.size(); i++)
	{
		if (ControlArray[i])
		{
			ControlArray[i]->parent_deleting = true;
			delete ControlArray[i];
		}
		ControlArray[i] = 0;
	}

	//delete triggers
	for (int i = 0; i < (int)TriggerArray.size(); i++)
	{
		if (TriggerArray[i])
		{
			TriggerArray[i]->parent_deleting = true;
			delete TriggerArray[i];
		}
		TriggerArray[i] = 0;
	}

	//delete models
	for (int i = 0; i < (int)ModelArray.size(); i++)
	{
		if (ModelArray[i])
		{
			ModelArray[i]->parent_deleting = true;
			delete ModelArray[i];
		}
		ModelArray[i] = 0;
	}

	//delete lights
	for (int i = 0; i < (int)lights.size(); i++)
	{
		if (lights[i])
		{
			lights[i]->parent_deleting = true;
			delete lights[i];
		}
		lights[i] = 0;
	}

	//delete call buttons
	for (int i = 0; i < (int)CallButtonArray.size(); i++)
	{
		if (CallButtonArray[i])
		{
			CallButtonArray[i]->parent_deleting = true;
			delete CallButtonArray[i];
		}
		CallButtonArray[i] = 0;
	}

	//delete doors
	for (int i = 0; i < (int)DoorArray.size(); i++)
	{
		if (DoorArray[i])
		{
			DoorArray[i]->parent_deleting = true;
			delete DoorArray[i];
		}
		DoorArray[i] = 0;
	}

	//delete floor indicators
	for (int i = 0; i < (int)FloorIndicatorArray.size(); i++)
	{
		if (FloorIndicatorArray[i])
		{
			FloorIndicatorArray[i]->parent_deleting = true;
			delete FloorIndicatorArray[i];
		}
		FloorIndicatorArray[i] = 0;
	}

	//delete directional indicators
	for (int i = 0; i < (int)DirIndicatorArray.size(); i++)
	{
		if (DirIndicatorArray[i])
		{
			DirIndicatorArray[i]->parent_deleting = true;
			delete DirIndicatorArray[i];
		}
		DirIndicatorArray[i] = 0;
	}

	//delete sounds
	for (int i = 0; i < (int)sounds.size(); i++)
	{
		if (sounds[i])
		{
			sounds[i]->parent_deleting = true;
			delete sounds[i];
		}
		sounds[i] = 0;
	}

	//delete meshes
	if (Level)
	{
		Level->parent_deleting = true;
		delete Level;
	}
	Level = 0;

	if (Interfloor)
	{
		Interfloor->parent_deleting = true;
		delete Interfloor;
	}
	Interfloor = 0;

	if (ColumnFrame)
	{
		ColumnFrame->parent_deleting = true;
		delete ColumnFrame;
	}
	ColumnFrame = 0;

	//delete walls in external mesh
	if (sbs->FastDelete == false)
		sbs->External->DeleteWalls(this);

	//unregister from parent
	if (sbs->FastDelete == false && parent_deleting == false)
		sbs->RemoveFloor(this);
}

WallObject* Floor::AddFloor(const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, bool reverse_axis, bool texture_direction, float tw, float th, bool isexternal, bool legacy_behavior)
{
	//Adds a floor with the specified dimensions and vertical offset

	WallObject *wall;

	if (isexternal == false)
	{
		wall = Level->CreateWallObject(name);
		sbs->AddFloorMain(wall, name, texture, thickness, x1, z1, x2, z2, GetBase(true) + voffset1, GetBase(true) + voffset2, reverse_axis, texture_direction, tw, th, true, legacy_behavior);
	}
	else
	{
		wall = sbs->External->CreateWallObject(name);
		sbs->AddFloorMain(wall, name, texture, thickness, x1, z1, x2, z2, Altitude + voffset1, Altitude + voffset2, reverse_axis, texture_direction, tw, th, true, legacy_behavior);
	}
	return wall;
}

WallObject* Floor::AddInterfloorFloor(const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, bool reverse_axis, bool texture_direction, float tw, float th, bool legacy_behavior)
{
	//Adds an interfloor floor with the specified dimensions and vertical offset

	WallObject *wall = Interfloor->CreateWallObject(name);
	sbs->AddFloorMain(wall, name, texture, thickness, x1, z1, x2, z2, voffset1, voffset2, reverse_axis, texture_direction, tw, th, true, legacy_behavior);
	return wall;
}

WallObject* Floor::AddWall(const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float voffset1, float voffset2, float tw, float th, bool isexternal)
{
	//Adds a wall with the specified dimensions

	WallObject *wall;
	if (isexternal == false)
	{
		wall = Level->CreateWallObject(name);
		sbs->AddWallMain(wall, name, texture, thickness, x1, z1, x2, z2, height_in1, height_in2, GetBase(true) + voffset1, GetBase(true) + voffset2, tw, th, true);
	}
	else
	{
		wall = sbs->External->CreateWallObject(name);
		sbs->AddWallMain(wall, name, texture, thickness, x1, z1, x2, z2, height_in1, height_in2, Altitude + voffset1, Altitude + voffset2, tw, th, true);
	}
	return wall;
}

WallObject* Floor::AddInterfloorWall(const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float voffset1, float voffset2, float tw, float th)
{
	//Adds an interfloor wall with the specified dimensions

	WallObject *wall = Interfloor->CreateWallObject(name);
	sbs->AddWallMain(wall, name, texture, thickness, x1, z1, x2, z2, height_in1, height_in2, voffset1, voffset2, tw, th, true);
	return wall;
}

void Floor::Enabled(bool value)
{
	//turns floor on/off

	if (IsEnabled == value)
		return;

	SBS_PROFILE("Floor::Enabled");
	Level->Enable(value);
	IsEnabled = value;

	if (sbs->Verbose)
	{
		if (value == true)
			Report("Enabled");
		else
			Report("Disabled");
	}

	//handle interfloors
	if (value == false)
	{
		if (sbs->InterfloorOnTop == false)
		{
			//only turn off interfloor if floor below is disabled
			Floor *floor = sbs->GetFloor(Number - 1);
			if (floor)
			{
				if (floor->IsEnabled == false)
					EnableInterfloor(false);
			}
			else
				EnableInterfloor(false);

			//turn off adjacent interfloor
			floor = sbs->GetFloor(Number + 1);
			if (floor)
			{
				if (floor->IsEnabled == false)
					floor->EnableInterfloor(false);
			}
		}
		else
		{
			//only turn off interfloor if floor above is disabled
			Floor *floor = sbs->GetFloor(Number + 1);
			if (floor)
			{
				if (floor->IsEnabled == false)
					EnableInterfloor(false);
			}
			else
				EnableInterfloor(false);

			//turn off adjacent interfloor
			floor = sbs->GetFloor(Number - 1);
			if (floor)
			{
				if (floor->IsEnabled == false)
					floor->EnableInterfloor(false);
			}
		}
	}
	else
	{
		if (sbs->InterfloorOnTop == false)
		{
			//turn on interfloor for next floor
			if (sbs->GetFloor(Number + 1))
				sbs->GetFloor(Number + 1)->EnableInterfloor(true);
		}
		else
		{
			//turn on interfloor for previous floor
			if (sbs->GetFloor(Number - 1))
				sbs->GetFloor(Number - 1)->EnableInterfloor(true);
		}
		EnableInterfloor(true);
	}

	EnableColumnFrame(value);

	//controls
	for (size_t i = 0; i < (int)ControlArray.size(); i++)
	{
		if (ControlArray[i])
			ControlArray[i]->Enabled(value);
	}

	//triggers
	for (size_t i = 0; i < (int)TriggerArray.size(); i++)
	{
		if (TriggerArray[i])
			TriggerArray[i]->Enabled(value);
	}

	//models
	for (size_t i = 0; i < (int)ModelArray.size(); i++)
	{
		if (ModelArray[i])
			ModelArray[i]->Enable(value);
	}

	//call buttons
	for (size_t i = 0; i < (int)CallButtonArray.size(); i++)
	{
		if (CallButtonArray[i])
			CallButtonArray[i]->Enabled(value);
	}

	//doors
	for (size_t i = 0; i < (int)DoorArray.size(); i++)
	{
		if (DoorArray[i])
			DoorArray[i]->Enabled(value);
	}

	//turn on/off directional indicators
	for (int i = 0; i < (int)DirIndicatorArray.size(); i++)
	{
		if (DirIndicatorArray[i])
			DirIndicatorArray[i]->Enabled(value);
	}
	UpdateDirectionalIndicators();

	//floor indicators
	for (int i = 0; i < (int)FloorIndicatorArray.size(); i++)
	{
		if (FloorIndicatorArray[i])
			FloorIndicatorArray[i]->Enabled(value);
	}
	//update floor indicator values
	UpdateFloorIndicators();

	//escalators
	for (size_t i = 0; i < (int)EscalatorArray.size(); i++)
	{
		if (EscalatorArray[i])
			EscalatorArray[i]->Enabled(value);
	}

	//sounds
	for (int i = 0; i < (int)sounds.size(); i++)
	{
		if (sounds[i])
		{
			if (sounds[i]->GetLoopState() == true)
			{
				if (value == false)
					sounds[i]->Stop();
				else
					sounds[i]->Play();
			}
		}
	}
}

float Floor::FullHeight()
{
	//calculate full height of a floor
	return InterfloorHeight + Height;
}

CallButton* Floor::AddCallButtons(std::vector<int> &elevators, const std::string &sound_file, const std::string &BackTexture, const std::string &UpButtonTexture, const std::string &UpButtonTexture_Lit, const std::string &DownButtonTexture, const std::string &DownButtonTexture_Lit, float CenterX, float CenterZ, float voffset, const std::string &direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th)
{
	//adds call buttons

	//check if any of the elevators serve the current floor
	bool check = false;
	for (int i = 0; i < (int)elevators.size(); i++)
	{
		Elevator *elev = sbs->GetElevator(elevators[i]);
		if (elev)
		{
			if (elev->IsServicedFloor(Number) == true)
			{
				check = true;
				break;
			}
		}
	}

	//exit if none of the elevators serve the floor
	if (check == false)
		return 0;

	//create call button
	int Current = (int)CallButtonArray.size();
	CallButton *button = new CallButton(elevators, Number, Current, sound_file, BackTexture, UpButtonTexture, UpButtonTexture_Lit, DownButtonTexture, DownButtonTexture_Lit, CenterX, CenterZ, voffset, direction, BackWidth, BackHeight, ShowBack, tw, th);
	CallButtonArray.push_back(button);
	return button;
}

void Floor::Cut(const Ogre::Vector3 &start, const Ogre::Vector3 &end, bool cutwalls, bool cutfloors, bool fast, int checkwallnumber, bool prepare)
{
	//caller to SBS cut function
	//Y values are relative to the floor's altitude
	//if fast is specified, skips the interfloor scan

	for (int i = 0; i < (int)Level->Walls.size(); i++)
	{
		bool reset = true;
		if (i > 0)
			reset = false;

		sbs->Cut(Level->Walls[i], Ogre::Vector3(start.x, start.y, start.z), Ogre::Vector3(end.x, end.y, end.z), cutwalls, cutfloors, checkwallnumber, reset);
	}
	if (fast == false)
	{
		for (int i = 0; i < (int)Interfloor->Walls.size(); i++)
		{
			sbs->Cut(Interfloor->Walls[i], Ogre::Vector3(start.x, start.y, start.z), Ogre::Vector3(end.x, end.y, end.z), cutwalls, cutfloors, checkwallnumber, false);
		}
	}
}

void Floor::CutAll(const Ogre::Vector3 &start, const Ogre::Vector3 &end, bool cutwalls, bool cutfloors, bool prepare)
{
	//cuts all objects related to this floor (floor, interfloor, shafts, stairs and external)
	//Y values are relative to the floor's altitude

	//cut current level
	Cut(start, end, cutwalls, cutfloors, false);

	//cut shafts
	for (int i = 1; i <= sbs->Shafts(); i++)
	{
		if (sbs->GetShaft(i))
			sbs->GetShaft(i)->Cut(false, Number, start, end, cutwalls, cutfloors);
	}

	//cut stairs
	for (int i = 1; i <= sbs->StairsNum(); i++)
	{
		if (sbs->GetStairs(i))
			sbs->GetStairs(i)->Cut(false, Number, start, end, cutwalls, cutfloors);
	}

	//cut external
	for (int i = 0; i < (int)sbs->External->Walls.size(); i++)
		sbs->Cut(sbs->External->Walls[i], Ogre::Vector3(start.x, Altitude + start.y, start.z), Ogre::Vector3(end.x, Altitude + end.y, end.z), cutwalls, cutfloors);
}

void Floor::AddGroupFloor(int number)
{
	//adds a floor number to the group list.
	//Groups are used to enable multiple floors at the same time when
	//a user arrives at a floor

	if (IsInGroup(number))
		return;

	Group.push_back(number);
	std::sort(Group.begin(), Group.end());
}

void Floor::RemoveGroupFloor(int number)
{
	//removes a floor number from the group list

	for (int i = 0; i < (int)Group.size(); i++)
	{
		if (Group[i] == number)
		{
			Group.erase(Group.begin() + i);
			return;
		}
	}
}

void Floor::EnableGroup(bool value)
{
	//enable floors grouped with this floor

	SBS_PROFILE("Floor::EnableGroup");
	if (Group.size() > 0)
	{
		for (size_t i = 0; i < Group.size(); i++)
		{
			Floor *floor = sbs->GetFloor(Group[i]);

			//check if floor exists
			if (floor)
			{
				//enable other floor
				floor->Enabled(value);

				if (value == true)
				{
					floor->EnabledGroup = true;
					floor->EnabledGroup_Floor = Number;
				}
				else
				{
					floor->EnabledGroup = false;
					floor->EnabledGroup_Floor = 0;
				}

				//enable shafts and stairs for other floor
				for (int j = 1; j <= sbs->Shafts(); j++)
				{
					Shaft *shaft = sbs->GetShaft(j);
					if (shaft)
					{
						if (shaft->IsEnabled == false)
							shaft->Enabled(Group[i], value, true);
					}
				}
				for (int j = 1; j <= sbs->StairsNum(); j++)
				{
					Stairs *stairs = sbs->GetStairs(j);
					if (stairs)
					{
						if (stairs->IsEnabled == false)
							stairs->Enabled(Group[i], value);
					}
				}
			}
		}
	}
}

bool Floor::IsInGroup(int floor)
{
	//return true if the specified floor is in the group listing

	if (Group.size() > 0)
	{
		for (size_t i = 0; i < Group.size(); i++)
		{
			if (Group[i] == floor)
				return true;
		}
	}
	return false;
}

Door* Floor::AddDoor(const std::string &open_sound, const std::string &close_sound, bool open_state, const std::string &texture, float thickness, int direction, float speed, float CenterX, float CenterZ, float width, float height, float voffset, float tw, float th)
{
	//interface to the SBS AddDoor function

	if (direction > 8 || direction < 1)
	{
		ReportError("Door direction out of range");
		return 0;
	}

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
	if (direction < 5)
		CutAll(Ogre::Vector3(x1 - 1, GetBase(true) + voffset, z1), Ogre::Vector3(x2 + 1, GetBase(true) + voffset + height, z2), true, false);
	else
		CutAll(Ogre::Vector3(x1, GetBase(true) + voffset, z1 - 1), Ogre::Vector3(x2, GetBase(true) + voffset + height, z2 + 1), true, false);

	int number = (int)DoorArray.size();
	std::string name = "Floor " + ToString(Number) + ":Door " + ToString(number);
	Door* door = new Door(this, name, open_sound, close_sound, open_state, texture, thickness, direction, speed, CenterX, CenterZ, width, height, GetBase(true) + voffset, tw, th);
	DoorArray.push_back(door);
	return door;
}

bool Floor::CalculateAltitude()
{
	//calculate the floor's altitude in relation to floor below (or above it, if it's a basement level)
	//and return the altitude value
	//if the related floor does not have an adjacent floor, return false

	if (Altitude == 0)
	{
		if (Number > 0)
		{
			if (sbs->GetFloor(Number - 1))
				Altitude = sbs->GetFloor(Number - 1)->Altitude + sbs->GetFloor(Number - 1)->FullHeight();
			else
			{
				return ReportError("Invalid floor number specified - no adjacent floor");
			}
		}

		if (Number == -1)
			Altitude = -FullHeight();

		if (Number < -1)
		{
			if (sbs->GetFloor(Number + 1))
				Altitude = sbs->GetFloor(Number + 1)->Altitude - FullHeight();
			else
				return ReportError("Invalid floor number specified - no adjacent floor");
		}
	}

	//set altitude
	SetAltitude(Altitude);

	return true;
}

void Floor::EnableColumnFrame(bool value)
{
	//enable/disable columnframe mesh
	ColumnFrame->Enable(value);
	IsColumnFrameEnabled = value;
}

void Floor::EnableInterfloor(bool value)
{
	//enable/disable interfloor mesh
	Interfloor->Enable(value);
	IsInterfloorEnabled = value;
}

WallObject* Floor::ColumnWallBox(const std::string &name, const std::string &texture, float x1, float x2, float z1, float z2, float height_in, float voffset, float tw, float th, bool inside, bool outside, bool top, bool bottom)
{
	//create columnframe wall box

	WallObject *wall = ColumnFrame->CreateWallObject(name);
	sbs->CreateWallBox(wall, name, texture, x1, x2, z1, z2, height_in, voffset, tw, th, inside, outside, top, bottom, true);
	return wall;
}

WallObject* Floor::ColumnWallBox2(const std::string &name, const std::string &texture, float CenterX, float CenterZ, float WidthX, float LengthZ, float height_in, float voffset, float tw, float th, bool inside, bool outside, bool top, bool bottom)
{
	//create columnframe wall box from a central location

	WallObject *wall = ColumnFrame->CreateWallObject(name);
	sbs->CreateWallBox2(wall, name, texture, CenterX, CenterZ, WidthX, LengthZ, height_in, voffset, tw, th, inside, outside, top, bottom, true);
	return wall;
}

FloorIndicator* Floor::AddFloorIndicator(int elevator, bool relative, const std::string &texture_prefix, const std::string &direction, float CenterX, float CenterZ, float width, float height, float voffset)
{
	//Creates a floor indicator at the specified location

	if (relative == false)
	{
		FloorIndicator *ind = new FloorIndicator(this, elevator, texture_prefix, direction, CenterX, CenterZ, width, height, GetBase(true) + voffset);
		FloorIndicatorArray.push_back(ind);
		return ind;
	}
	else
	{
		Elevator* elev = sbs->GetElevator(elevator);
		if (elev)
		{
			FloorIndicator *ind = new FloorIndicator(this, elevator, texture_prefix, direction, elev->GetPosition().x + CenterX, elev->GetPosition().z + CenterZ, width, height, GetBase(true) + voffset);
			FloorIndicatorArray.push_back(ind);
			return ind;
		}
		else
			return 0;
	}
}

void Floor::UpdateFloorIndicators(int elevator)
{
	//updates a floor indicator for a specified elevator

	for (int i = 0; i < (int)FloorIndicatorArray.size(); i++)
	{
		if (FloorIndicatorArray[i])
		{
			if (FloorIndicatorArray[i]->elev == elevator)
				FloorIndicatorArray[i]->Update();
		}
	}
}

void Floor::UpdateFloorIndicators()
{
	//updates all floor indicators

	for (int i = 0; i < (int)FloorIndicatorArray.size(); i++)
	{
		if (FloorIndicatorArray[i])
			FloorIndicatorArray[i]->Update();
	}
}

void Floor::Loop()
{
	//floor object main loop; runs if camera is currently on this floor

	if (IsEnabled == false)
		return;

	for (int i = 0; i < (int)TriggerArray.size(); i++)
	{
		if (TriggerArray[i])
			TriggerArray[i]->Loop();
	}

	for (int i = 0; i < (int)EscalatorArray.size(); i++)
	{
		if (EscalatorArray[i])
			EscalatorArray[i]->Loop();
	}

	for (int i = 0; i < (int)ModelArray.size(); i++)
	{
		if (ModelArray[i])
			ModelArray[i]->Loop();
	}
}

std::vector<int> Floor::GetCallButtons(int elevator)
{
	//get numbers of call buttons that service the specified elevator

	std::vector<int> buttons;
	buttons.reserve(CallButtonArray.size());
	for (int i = 0; i < (int)CallButtonArray.size(); i++)
	{
		//put button number onto the array if it serves the elevator
		if (CallButtonArray[i])
		{
			if (CallButtonArray[i]->ServicesElevator(elevator) == true)
				buttons.push_back(i);
		}
	}
	return buttons;
}

CallButton* Floor::GetCallButton(int elevator)
{
	//returns the first call button object that services the specified elevator

	for (int i = 0; i < (int)CallButtonArray.size(); i++)
	{
		if (CallButtonArray[i])
		{
			if (CallButtonArray[i]->ServicesElevator(elevator) == true)
				return CallButtonArray[i];
		}
	}
	return 0;
}

void Floor::AddFillerWalls(const std::string &texture, float thickness, float CenterX, float CenterZ, float width, float height, float voffset, bool direction, float tw, float th)
{
	//convenience function for adding filler walls around doors
	//direction is either "false" for a door that faces left/right, or "true" for one that faces front/back

	float x1 = 0, x2 = 0, z1 = 0, z2 = 0, depth1 = 0, depth2 = 0;

	//exit if no height given
	if (height == 0.0f)
	{
		ReportError("AddFillerWalls: no wall height specified");
		return;
	}

	if (sbs->GetWallOrientation() == 0)
	{
		depth1 = 0;
		depth2 = thickness;
	}
	if (sbs->GetWallOrientation() == 1)
	{
		depth1 = thickness / 2;
		depth2 = thickness / 2;
	}
	if (sbs->GetWallOrientation() == 2)
	{
		depth1 = thickness;
		depth2 = 0;
	}

	if (direction == false)
	{
		//door faces left/right
		x1 = CenterX - depth1;
		x2 = CenterX + depth2;
		z1 = CenterZ - (width / 2);
		z2 = CenterZ + (width / 2);
	}
	else
	{
		//door faces front/back
		x1 = CenterX - (width / 2);
		x2 = CenterX + (width / 2);
		z1 = CenterZ - depth1;
		z2 = CenterZ + depth2;
	}

	//perform a cut in the area
	CutAll(Ogre::Vector3(x1, GetBase(true) + voffset, z1), Ogre::Vector3(x2, GetBase(true) + voffset + height, z2), true, false);

	//create walls
	sbs->DrawWalls(false, true, false, false, false, false);
	if (direction == false)
		AddWall("FillerWallLeft", texture, 0, x1, z1, x2, z1, height, height, voffset, voffset, tw, th, false);
	else
		AddWall("FillerWallLeft", texture, 0, x1, z1, x1, z2, height, height, voffset, voffset, tw, th, false);
	sbs->ResetWalls();

	sbs->DrawWalls(true, false, false, false, false, false);
	if (direction == false)
		AddWall("FillerWallRight", texture, 0, x1, z2, x2, z2, height, height, voffset, voffset, tw, th, false);
	else
		AddWall("FillerWallRight", texture, 0, x2, z1, x2, z2, height, height, voffset, voffset, tw, th, false);

	AddFloor("FillerWallTop", texture, 0, x1, z1, x2, z2, height + voffset, height + voffset, false, false, tw, th, false);
	sbs->ResetWalls();
}

Sound* Floor::AddSound(const std::string &name, const std::string &filename, Ogre::Vector3 position, bool loop, float volume, int speed, float min_distance, float max_distance, float doppler_level, float cone_inside_angle, float cone_outside_angle, float cone_outside_volume, Ogre::Vector3 direction)
{
	//create a looping sound object

	Sound *sound = new Sound(this, name, false);
	sounds.push_back(sound);

	//set parameters and play sound
	sound->Move(position.x, GetBase(true) + position.y, position.z);
	sound->SetDirection(direction);
	sound->SetVolume(volume);
	sound->SetSpeed(speed);
	sound->SetDistances(min_distance, max_distance);
	sound->SetDirection(direction);
	sound->SetDopplerLevel(doppler_level);
	sound->SetConeSettings(cone_inside_angle, cone_outside_angle, cone_outside_volume);
	sound->Load(filename);
	sound->SetLoopState(loop);
	if (loop && sbs->IsRunning == true && sbs->camera->CurrentFloor == Number)
		sound->Play();

	return sound;
}

std::vector<Sound*> Floor::GetSound(const std::string &name)
{
	//get sound by name

	std::string findname = name;
	SetCase(findname, false);
	std::vector<Sound*> soundlist;
	for (int i = 0; i < (int)sounds.size(); i++)
	{
		if (sounds[i])
		{
			std::string name2 = sounds[i]->GetName();
			SetCase(name2, false);
			if (findname == name2)
				soundlist.push_back(sounds[i]);
		}
	}
	return soundlist;
}

void Floor::Report(const std::string &message)
{
	//general reporting function
	sbs->Report("Floor " + ToString(Number) + ": " + message);
}

bool Floor::ReportError(const std::string &message)
{
	//general reporting function
	return sbs->ReportError("Floor " + ToString(Number) + ": " + message);
}

float Floor::GetBase(bool relative)
{
	//returns the base of the floor
	//if Interfloor is on the bottom of the level (by default), the base is GetBase()
	//otherwise the base is just altitude
	if (relative == false)
	{
		if (sbs->InterfloorOnTop == false)
			return Altitude + InterfloorHeight;
		else
			return Altitude;
	}
	else
	{
		if (sbs->InterfloorOnTop == false)
			return InterfloorHeight;
		else
			return 0;
	}
}

DirectionalIndicator* Floor::AddDirectionalIndicator(int elevator, bool relative, bool active_direction, bool single, bool vertical, const std::string &BackTexture, const std::string &uptexture, const std::string &uptexture_lit, const std::string &downtexture, const std::string &downtexture_lit, float CenterX, float CenterZ, float voffset, const std::string &direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th)
{
	//create a directional indicator on the specified floor, associated with a given elevator

	if (sbs->Verbose)
		Report("adding directional indicator");

	Elevator *elev = sbs->GetElevator(elevator);
	if (!elev)
		return 0;

	float x, z;
	if (relative == true)
	{
		x = elev->GetPosition().x + CenterX;
		z = elev->GetPosition().z + CenterZ;
	}
	else
	{
		x = CenterX;
		z = CenterZ;
	}

	if (active_direction == false)
	{
		//if active_direction is false, only create indicator if the elevator serves the floor
		if (elev->IsServicedFloor(Number) == false)
			return 0;
	}

	DirectionalIndicator *indicator = new DirectionalIndicator(this, elevator, Number, active_direction, single, vertical, BackTexture, uptexture, uptexture_lit, downtexture, downtexture_lit, x, z, GetBase(true) + voffset, direction, BackWidth, BackHeight, ShowBack, tw, th);
	DirIndicatorArray.push_back(indicator);
	return indicator;
}

void Floor::SetDirectionalIndicators(int elevator, bool UpLight, bool DownLight)
{
	//set light status of all standard (non active-direction) directional indicators associated with the given elevator

	for (int i = 0; i < (int)DirIndicatorArray.size(); i++)
	{
		DirectionalIndicator *indicator = DirIndicatorArray[i];

		if (indicator)
		{
			if (indicator->elevator == elevator && indicator->ActiveDirection == false)
			{
				indicator->DownLight(DownLight);
				indicator->UpLight(UpLight);
			}
		}
	}
}

void Floor::UpdateDirectionalIndicators(int elevator)
{
	//updates the active-direction indicators associated with the given elevator

	SBS_PROFILE("Floor::UpdateDirectionalIndicators1");
	for (int i = 0; i < (int)DirIndicatorArray.size(); i++)
	{
		DirectionalIndicator *indicator = DirIndicatorArray[i];

		if (indicator)
		{
			if (indicator->elevator == elevator && indicator->ActiveDirection == true)
			{
				Elevator *elev = sbs->GetElevator(elevator);

				if (!elev)
					return;

				if (elev->ActiveDirection == 1)
				{
					indicator->UpLight(true);
					indicator->DownLight(false);
				}
				if (elev->ActiveDirection == 0)
				{
					indicator->UpLight(false);
					indicator->DownLight(false);
				}
				if (elev->ActiveDirection == -1)
				{
					indicator->UpLight(false);
					indicator->DownLight(true);
				}
			}
		}
	}
}

void Floor::UpdateDirectionalIndicators()
{
	//updates all active-direction indicators

	SBS_PROFILE("Floor::UpdateDirectionalIndicators2");
	for (int i = 0; i < (int)DirIndicatorArray.size(); i++)
	{
		DirectionalIndicator *indicator = DirIndicatorArray[i];

		if (indicator)
		{
			if (indicator->ActiveDirection == true)
			{
				Elevator *elev = sbs->GetElevator(indicator->elevator);

				if (!elev)
					return;

				if (elev->ActiveDirection == 1)
				{
					indicator->UpLight(true);
					indicator->DownLight(false);
				}
				if (elev->ActiveDirection == 0)
				{
					indicator->UpLight(false);
					indicator->DownLight(false);
				}
				if (elev->ActiveDirection == -1)
				{
					indicator->UpLight(false);
					indicator->DownLight(true);
				}
			}
		}
	}
}

Door* Floor::GetDoor(int number)
{
	//get door object
	if (number < (int)DoorArray.size())
	{
		if (DoorArray[number])
			return DoorArray[number];
	}

	return 0;
}

void Floor::RemoveCallButton(CallButton *callbutton)
{
	//remove a call button object from the array
	//this does not delete the object
	for (int i = 0; i < (int)CallButtonArray.size(); i++)
	{
		if (CallButtonArray[i] == callbutton)
		{
			CallButtonArray.erase(CallButtonArray.begin() + i);
			return;
		}
	}
}

void Floor::RemoveFloorIndicator(FloorIndicator *indicator)
{
	//remove a floor indicator from the array
	//this does not delete the object
	for (int i = 0; i < (int)FloorIndicatorArray.size(); i++)
	{
		if (FloorIndicatorArray[i] == indicator)
		{
			FloorIndicatorArray.erase(FloorIndicatorArray.begin() + i);
			return;
		}
	}
}

void Floor::RemoveDirectionalIndicator(DirectionalIndicator *indicator)
{
	//remove a directional indicator from the array
	//this does not delete the object
	for (int i = 0; i < (int)DirIndicatorArray.size(); i++)
	{
		if (DirIndicatorArray[i] == indicator)
		{
			DirIndicatorArray.erase(DirIndicatorArray.begin() + i);
			return;
		}
	}
}

void Floor::RemoveDoor(Door *door)
{
	//remove a door from the array
	//this does not delete the object
	for (int i = 0; i < (int)DoorArray.size(); i++)
	{
		if (DoorArray[i] == door)
		{
			DoorArray.erase(DoorArray.begin() + i);
			return;
		}
	}
}

void Floor::RemoveSound(Sound *sound)
{
	//remove a sound from the array
	//this does not delete the object
	for (int i = 0; i < (int)sounds.size(); i++)
	{
		if (sounds[i] == sound)
		{
			sounds.erase(sounds.begin() + i);
			return;
		}
	}
}

void Floor::RemoveLight(Light *light)
{
	//remove a light reference (does not delete the object itself)
	for (int i = 0; i < (int)lights.size(); i++)
	{
		if (lights[i] == light)
		{
			lights.erase(lights.begin() + i);
			return;
		}
	}
}

void Floor::RemoveModel(Model *model)
{
	//remove a model reference (does not delete the object itself)
	for (int i = 0; i < (int)ModelArray.size(); i++)
	{
		if (ModelArray[i] == model)
		{
			ModelArray.erase(ModelArray.begin() + i);
			return;
		}
	}
}

void Floor::RemoveControl(Control *control)
{
	//remove a control reference (does not delete the object itself)
	for (int i = 0; i < (int)ControlArray.size(); i++)
	{
		if (ControlArray[i] == control)
		{
			ControlArray.erase(ControlArray.begin() + i);
			return;
		}
	}
}

void Floor::RemoveTrigger(Trigger *trigger)
{
	//remove a trigger reference (does not delete the object itself)
	for (int i = 0; i < (int)TriggerArray.size(); i++)
	{
		if (TriggerArray[i] == trigger)
		{
			TriggerArray.erase(TriggerArray.begin() + i);
			return;
		}
	}
}

void Floor::RemoveCameraTexture(CameraTexture *cameratexture)
{
	//remove a camera texture reference (does not delete the object itself)
	for (int i = 0; i < (int)CameraTextureArray.size(); i++)
	{
		if (CameraTextureArray[i] == cameratexture)
		{
			CameraTextureArray.erase(CameraTextureArray.begin() + i);
			return;
		}
	}
}

void Floor::RemoveEscalator(Escalator *escalator)
{
	//remove an escalator reference (does not delete the object itself)
	for (int i = 0; i < (int)EscalatorArray.size(); i++)
	{
		if (EscalatorArray[i] == escalator)
		{
			EscalatorArray.erase(EscalatorArray.begin() + i);
			return;
		}
	}
}

Light* Floor::AddLight(const std::string &name, int type, Ogre::Vector3 position, Ogre::Vector3 direction, float color_r, float color_g, float color_b, float spec_color_r, float spec_color_g, float spec_color_b, float spot_inner_angle, float spot_outer_angle, float spot_falloff, float att_range, float att_constant, float att_linear, float att_quadratic)
{
	//add a light

	Light* light = new Light(this, name, type, position + Ogre::Vector3(0, GetBase(true), 0), direction, color_r, color_g, color_b, spec_color_r, spec_color_g, spec_color_b, spot_inner_angle, spot_outer_angle, spot_falloff, att_range, att_constant, att_linear, att_quadratic);
	lights.push_back(light);
	return light;
}

Model* Floor::AddModel(const std::string &name, const std::string &filename, bool center, Ogre::Vector3 position, Ogre::Vector3 rotation, float max_render_distance, float scale_multiplier, bool enable_physics, float restitution, float friction, float mass)
{
	//add a model
	Model* model = new Model(this, name, filename, center, position + Ogre::Vector3(0, GetBase(true), 0), rotation, max_render_distance, scale_multiplier, enable_physics, restitution, friction, mass);
	if (model->load_error == true)
	{
		delete model;
		return 0;
	}
	ModelArray.push_back(model);
	return model;
}

void Floor::AddModel(Model *model)
{
	//add a model reference

	if (!model)
		return;

	for (int i = 0; i < (int)ModelArray.size(); i++)
	{
		if (ModelArray[i] == model)
			return;
	}

	ModelArray.push_back(model);
}

void Floor::ReplaceTexture(const std::string &oldtexture, const std::string &newtexture)
{
	//change all instances of oldtexture in all meshes to newtexture
	Level->ReplaceTexture(oldtexture, newtexture);
	Interfloor->ReplaceTexture(oldtexture, newtexture);
	ColumnFrame->ReplaceTexture(oldtexture, newtexture);
}

Control* Floor::AddControl(const std::string &name, const std::string &sound, const std::string &direction, float CenterX, float CenterZ, float width, float height, float voffset, std::vector<std::string> &action_names, std::vector<std::string> &textures)
{
	//add a control
	std::vector<Action*> actionnull; //not used
	Control* control = new Control(this, name, false, sound, action_names, actionnull, textures, direction, width, height, true);
	control->Move(CenterX, GetBase(true) + voffset, CenterZ);
	ControlArray.push_back(control);
	return control;
}

Trigger* Floor::AddTrigger(const std::string &name, const std::string &sound_file, Ogre::Vector3 &area_min, Ogre::Vector3 &area_max, std::vector<std::string> &action_names)
{
	//add a trigger
	Trigger* trigger = new Trigger(this, name, false, sound_file, area_min, area_max, action_names);
	TriggerArray.push_back(trigger);
	trigger->Move(0, GetBase(true), 0);
	return trigger;
}

CameraTexture* Floor::AddCameraTexture(const std::string &name, bool enabled, int quality, float fov, Ogre::Vector3 position, bool use_rotation, Ogre::Vector3 rotation)
{
	//add a camera texture
	CameraTexture* cameratexture = new CameraTexture(this, name, enabled, quality, fov, GetBase(true) + position, use_rotation, rotation);
	CameraTextureArray.push_back(cameratexture);
	return cameratexture;
}

Escalator* Floor::AddEscalator(const std::string &name, bool run, bool run_direction, const std::string &sound_file, const std::string &texture, const std::string &direction, float CenterX, float CenterZ, float width, float risersize, float treadsize, int num_steps, float voffset, float tw, float th)
{
	//add an escalator
	Escalator* escalator = new Escalator(this, name, run, run_direction, sound_file, texture, direction, CenterX, CenterZ, width, risersize, treadsize, num_steps, GetBase(true) + voffset, tw, th);
	EscalatorArray.push_back(escalator);
	return escalator;
}

void Floor::SetAltitude(float altitude)
{
	//position object at altitude
	SetPositionY(altitude);
	Altitude = altitude;
}

void Floor::ShowInfo(bool detailed, bool display_header)
{
	//show information about this floor on the console

	//if detailed is true (default), show detailed information for this floor
	//otherwise, show a single-line listing suitable for showing in a list with other floors

	//if display_header is true, show header/key along with listing

	if (display_header == true)
		sbs->Report("\n--- Floor Information ---\n");

	if (detailed == true)
	{
		sbs->Report("Number: " + ToString(Number));
		sbs->Report("ID: " + ID);
		sbs->Report("Name: " + Name);
		sbs->Report("Type: " + FloorType);
		sbs->Report("Description: " + Description);
		sbs->Report("Height: " + ToString(Height));
		sbs->Report("InterfloorHeight: " + ToString(InterfloorHeight));
		sbs->Report("FullHeight: " + ToString(FullHeight()));
		sbs->Report("Altitude: " + ToString(Altitude));
		sbs->Report("Base: " + ToString(GetBase()));

		std::vector<int> elevator_list, stairs_list, shaft_list;
		GetElevatorList(elevator_list);
		GetStairsList(stairs_list);
		GetShaftList(shaft_list);

		std::string elevs;
		for (int i = 0; i < (int)elevator_list.size(); i++)
		{
			elevs += ToString(elevator_list[i]);
			if (i < (int)elevator_list.size() - 1)
				elevs += ", ";
		}
		sbs->Report("Elevators servicing: " + elevs);

		std::string stairs;
		for (int i = 0; i < (int)stairs_list.size(); i++)
		{
			stairs += ToString(stairs_list[i]);
			if (i < (int)stairs_list.size() - 1)
				stairs += ", ";
		}
		sbs->Report("Stairwells spanning: " + stairs);

		std::string shafts;
		for (int i = 0; i < (int)shaft_list.size(); i++)
		{
			shafts += ToString(shaft_list[i]);
			if (i < (int)shaft_list.size() - 1)
				shafts += ", ";
		}
		sbs->Report("Shafts spanning: " + shafts);

		if (display_header == true)
			sbs->Report("");
	}
	else
	{
		if (display_header == true)
			sbs->Report("Number(ID)\t----\tName\t----\tType\t----\tHeight\t----\tIFloorHeight\t----\tAltitude\t----\tBase\t----\tDescription");

		sbs->Report(ToString(Number) + "(" + ID + ")\t----\t" + Name + "\t----\t" + FloorType + "\t----\t" + ToString(Height) + "\t----\t" + ToString(InterfloorHeight) + "\t----\t" + ToString(Altitude) + "\t----\t" + ToString(GetBase()) + "\t----\t" + Description);
	}
}

void Floor::GetElevatorList(std::vector<int> &listing)
{
	//return a list of elevators that service this floor

	for (int i = 1; i <= sbs->Elevators(); i++)
	{
		Elevator *elev = sbs->GetElevator(i);
		if (elev)
		{
			if (elev->IsServicedFloor(Number) == true)
				listing.push_back(elev->Number);
		}
	}
}

void Floor::GetStairsList(std::vector<int> &listing)
{
	//return a list of stairwells that span this floor

	for (int i = 1; i <= sbs->StairsNum(); i++)
	{
		Stairs *stairs = sbs->GetStairs(i);
		if (stairs)
		{
			if (stairs->IsValidFloor(Number) == true)
				listing.push_back(stairs->StairsNum);
		}
	}
}

void Floor::GetShaftList(std::vector<int> &listing)
{
	//return a list of shafts that span this floor

	for (int i = 1; i <= sbs->Shafts(); i++)
	{
		Shaft *shaft = sbs->GetShaft(i);
		if (shaft)
		{
			if (shaft->IsValidFloor(Number) == true)
				listing.push_back(shaft->ShaftNumber);
		}
	}
}

}
