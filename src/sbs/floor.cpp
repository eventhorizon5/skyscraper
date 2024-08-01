/*
	Scalable Building Simulator - Floor Object
	The Skyscraper Project - Version 2.1
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
#include "manager.h"
#include "dynamicmesh.h"
#include "mesh.h"
#include "elevator.h"
#include "elevatorcar.h"
#include "shaft.h"
#include "stairs.h"
#include "door.h"
#include "model.h"
#include "light.h"
#include "camera.h"
#include "route.h"
#include "escalator.h"
#include "cameratexture.h"
#include "callstation.h"
#include "control.h"
#include "trigger.h"
#include "floorindicator.h"
#include "directional.h"
#include "sound.h"
#include "profiler.h"
#include "revolvingdoor.h"
#include "movingwalkway.h"
#include "controller.h"
#include "utility.h"
#include "floor.h"

namespace SBS {

Floor::Floor(Object *parent, FloorManager *manager, int number) : Object(parent)
{
	//set up SBS object
	SetValues("Floor", "", false);

	//Set floor's object number
	Number = number;
	std::string num = ToString(Number);
	SetName("Floor " + num);

	//Create primary level mesh
	Level = new MeshObject(this, "Level " + num, manager->GetFloorDynMesh());

	//Create interfloor mesh
	Interfloor = new MeshObject(this, "Interfloor " + num, manager->GetIFloorDynMesh());

	//Create columnframe mesh
	ColumnFrame = new MeshObject(this, "ColumnFrame " + num, manager->GetColumnDynMesh());

	//set enabled flags
	IsEnabled = true;
	IsColumnFrameEnabled = true;
	IsInterfloorEnabled = true;

	//init other variables
	Name = "";
	ID = "";
	NumberID = "";
	FloorType = "";
	Description = "";
	IndicatorTexture = "";
	Altitude = 0;
	Height = 0;
	InterfloorHeight = 0;
	EnabledGroup = false;
	EnabledGroup_Floor = 0;
	AltitudeSet = false;

	//create a dynamic mesh for doors
	DoorWrapper = new DynamicMesh(this, GetSceneNode(), GetName() + " Door Container", 0, true);
	DoorWrapper->force_combine = true;
}

Floor::~Floor()
{
	//Destructor

	//delete moving walkways
	for (size_t i = 0; i < MovingWalkwayArray.size(); i++)
	{
		if (MovingWalkwayArray[i])
		{
			MovingWalkwayArray[i]->parent_deleting = true;
			delete MovingWalkwayArray[i];
		}
		MovingWalkwayArray[i] = 0;
	}

	//delete escalators
	for (size_t i = 0; i < EscalatorArray.size(); i++)
	{
		if (EscalatorArray[i])
		{
			EscalatorArray[i]->parent_deleting = true;
			delete EscalatorArray[i];
		}
		EscalatorArray[i] = 0;
	}

	//delete camera textures
	for (size_t i = 0; i < CameraTextureArray.size(); i++)
	{
		if (CameraTextureArray[i])
		{
			CameraTextureArray[i]->parent_deleting = true;
			delete CameraTextureArray[i];
		}
		CameraTextureArray[i] = 0;
	}

	//delete controls
	for (size_t i = 0; i < ControlArray.size(); i++)
	{
		if (ControlArray[i])
		{
			ControlArray[i]->parent_deleting = true;
			delete ControlArray[i];
		}
		ControlArray[i] = 0;
	}

	//delete triggers
	for (size_t i = 0; i < TriggerArray.size(); i++)
	{
		if (TriggerArray[i])
		{
			TriggerArray[i]->parent_deleting = true;
			delete TriggerArray[i];
		}
		TriggerArray[i] = 0;
	}

	//delete models
	for (size_t i = 0; i < ModelArray.size(); i++)
	{
		if (ModelArray[i])
		{
			ModelArray[i]->parent_deleting = true;
			delete ModelArray[i];
		}
		ModelArray[i] = 0;
	}

	//delete lights
	for (size_t i = 0; i < lights.size(); i++)
	{
		if (lights[i])
		{
			lights[i]->parent_deleting = true;
			delete lights[i];
		}
		lights[i] = 0;
	}

	//delete call stations
	for (size_t i = 0; i < CallStationArray.size(); i++)
	{
		if (CallStationArray[i])
		{
			CallStationArray[i]->parent_deleting = true;
			delete CallStationArray[i];
		}
		CallStationArray[i] = 0;
	}

	//delete doors
	for (size_t i = 0; i < DoorArray.size(); i++)
	{
		if (DoorArray[i])
		{
			DoorArray[i]->parent_deleting = true;
			delete DoorArray[i];
		}
		DoorArray[i] = 0;
	}

	//delete revolving doors
	for (size_t i = 0; i < RDoorArray.size(); i++)
	{
		if (RDoorArray[i])
		{
			RDoorArray[i]->parent_deleting = true;
			delete RDoorArray[i];
		}
		RDoorArray[i] = 0;
	}

	if (DoorWrapper)
		delete DoorWrapper;
	DoorWrapper = 0;

	//delete floor indicators
	for (size_t i = 0; i < FloorIndicatorArray.size(); i++)
	{
		if (FloorIndicatorArray[i])
		{
			FloorIndicatorArray[i]->parent_deleting = true;
			delete FloorIndicatorArray[i];
		}
		FloorIndicatorArray[i] = 0;
	}

	//delete directional indicators
	for (size_t i = 0; i < DirIndicatorArray.size(); i++)
	{
		if (DirIndicatorArray[i])
		{
			DirIndicatorArray[i]->parent_deleting = true;
			delete DirIndicatorArray[i];
		}
		DirIndicatorArray[i] = 0;
	}

	//delete sounds
	for (size_t i = 0; i < sounds.size(); i++)
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
	if (sbs->FastDelete == false && sbs->External)
		sbs->External->DeleteWalls(this);

	//unregister from parent
	if (sbs->FastDelete == false && parent_deleting == false)
		sbs->RemoveFloor(this);
}

Wall* Floor::AddFloor(const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real voffset1, Real voffset2, bool reverse_axis, bool texture_direction, Real tw, Real th, bool isexternal, bool legacy_behavior)
{
	//Adds a floor with the specified dimensions and vertical offset

	Wall *wall = 0;

	if (isexternal == false)
	{
		wall = Level->CreateWallObject(name);
		sbs->AddFloorMain(wall, name, texture, thickness, x1, z1, x2, z2, GetBase(true) + voffset1, GetBase(true) + voffset2, reverse_axis, texture_direction, tw, th, true, legacy_behavior);
	}
	else
	{
		if (sbs->External)
		{
			wall = sbs->External->CreateWallObject(name);
			sbs->AddFloorMain(wall, name, texture, thickness, x1, z1, x2, z2, Altitude + voffset1, Altitude + voffset2, reverse_axis, texture_direction, tw, th, true, legacy_behavior);
		}
	}
	return wall;
}

Wall* Floor::AddInterfloorFloor(const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real voffset1, Real voffset2, bool reverse_axis, bool texture_direction, Real tw, Real th, bool legacy_behavior)
{
	//Adds an interfloor floor with the specified dimensions and vertical offset

	Wall *wall = Interfloor->CreateWallObject(name);
	sbs->AddFloorMain(wall, name, texture, thickness, x1, z1, x2, z2, voffset1, voffset2, reverse_axis, texture_direction, tw, th, true, legacy_behavior);
	return wall;
}

Wall* Floor::AddWall(const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real height_in1, Real height_in2, Real voffset1, Real voffset2, Real tw, Real th, bool isexternal)
{
	//Adds a wall with the specified dimensions

	Wall *wall = 0;
	if (isexternal == false)
	{
		wall = Level->CreateWallObject(name);
		sbs->AddWallMain(wall, name, texture, thickness, x1, z1, x2, z2, height_in1, height_in2, GetBase(true) + voffset1, GetBase(true) + voffset2, tw, th, true);
	}
	else
	{
		if (sbs->External)
		{
			wall = sbs->External->CreateWallObject(name);
			sbs->AddWallMain(wall, name, texture, thickness, x1, z1, x2, z2, height_in1, height_in2, Altitude + voffset1, Altitude + voffset2, tw, th, true);
		}
	}
	return wall;
}

Wall* Floor::AddInterfloorWall(const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real height_in1, Real height_in2, Real voffset1, Real voffset2, Real tw, Real th)
{
	//Adds an interfloor wall with the specified dimensions

	Wall *wall = Interfloor->CreateWallObject(name);
	sbs->AddWallMain(wall, name, texture, thickness, x1, z1, x2, z2, height_in1, height_in2, voffset1, voffset2, tw, th, true);
	return wall;
}

void Floor::Enabled(bool value)
{
	//turns floor on/off

	if (IsEnabled == value)
		return;

	SBS_PROFILE("Floor::Enabled");
	Level->Enabled(value);
	IsEnabled = value;

	EnableLoop(value);

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
	for (size_t i = 0; i < ControlArray.size(); i++)
	{
		if (ControlArray[i])
			ControlArray[i]->Enabled(value);
	}

	//triggers
	for (size_t i = 0; i < TriggerArray.size(); i++)
	{
		if (TriggerArray[i])
			TriggerArray[i]->Enabled(value);
	}

	//models
	for (size_t i = 0; i < ModelArray.size(); i++)
	{
		if (ModelArray[i])
			ModelArray[i]->Enabled(value);
	}

	//call stations
	for (size_t i = 0; i < CallStationArray.size(); i++)
	{
		if (CallStationArray[i])
			CallStationArray[i]->Enabled(value);
	}

	//doors
	for (size_t i = 0; i < DoorArray.size(); i++)
	{
		if (DoorArray[i])
			DoorArray[i]->Enabled(value);
	}
	DoorWrapper->Enabled(value);

	//turn on/off directional indicators
	for (size_t i = 0; i < DirIndicatorArray.size(); i++)
	{
		if (DirIndicatorArray[i])
			DirIndicatorArray[i]->Enabled(value);
	}
	UpdateDirectionalIndicators();

	//floor indicators
	for (size_t i = 0; i < FloorIndicatorArray.size(); i++)
	{
		if (FloorIndicatorArray[i])
			FloorIndicatorArray[i]->Enabled(value);
	}
	//update floor indicator values
	UpdateFloorIndicators();

	//escalators
	for (size_t i = 0; i < EscalatorArray.size(); i++)
	{
		if (EscalatorArray[i])
			EscalatorArray[i]->Enabled(value);
	}

	//moving walkways
	for (size_t i = 0; i < MovingWalkwayArray.size(); i++)
	{
		if (MovingWalkwayArray[i])
			MovingWalkwayArray[i]->Enabled(value);
	}

	//sounds
	for (size_t i = 0; i < sounds.size(); i++)
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

	//lights
	for (size_t i = 0; i < lights.size(); i++)
	{
		if (lights[i])
			lights[i]->Enabled(value);
	}
}

Real Floor::FullHeight()
{
	//calculate full height of a floor
	return InterfloorHeight + Height;
}

CallStation* Floor::AddCallButtons(int controller, const std::string &sound_file_up, const std::string &sound_file_down, const std::string &BackTexture, const std::string &UpButtonTexture, const std::string &UpButtonTexture_Lit, const std::string &DownButtonTexture, const std::string &DownButtonTexture_Lit, Real CenterX, Real CenterZ, Real voffset, const std::string &direction, Real BackWidth, Real BackHeight, bool ShowBack, Real tw, Real th)
{
	//create call buttons

	//make sure the controller serves this floor before proceeding
	if (sbs->GetController(controller))
	{
		if (sbs->GetController(controller)->ServesFloor(Number) == false)
			return 0;
	}

	CallStation *station = AddCallStation(0);
	station->SetController(controller);
	station->CreateCallButtons(sound_file_up, sound_file_down, BackTexture, UpButtonTexture, UpButtonTexture_Lit, DownButtonTexture, DownButtonTexture_Lit, direction, BackWidth, BackHeight, ShowBack, tw, th);

	station->Move(CenterX, GetBase(true) + voffset, CenterZ);

	return station;
}

CallStation* Floor::AddCallStation(int number)
{
	//create a new call station object

	CallStation *station = new CallStation(this, Number, number);
	CallStationArray.push_back(station);
	return station;
}

void Floor::Cut(const Vector3 &start, const Vector3 &end, bool cutwalls, bool cutfloors, bool fast, int checkwallnumber, bool prepare)
{
	//caller to SBS cut function
	//Y values are relative to the floor's altitude
	//if fast is specified, skips the interfloor scan

	for (size_t i = 0; i < Level->Walls.size(); i++)
	{
		if (!Level->Walls[i])
			continue;

		bool reset = true;
		if (i > 0)
			reset = false;

		sbs->GetUtility()->Cut(Level->Walls[i], Vector3(start.x, start.y, start.z), Vector3(end.x, end.y, end.z), cutwalls, cutfloors, checkwallnumber, reset);
	}
	if (fast == false)
	{
		for (size_t i = 0; i < Interfloor->Walls.size(); i++)
		{
			if (!Interfloor->Walls[i])
				continue;

			sbs->GetUtility()->Cut(Interfloor->Walls[i], Vector3(start.x, start.y, start.z), Vector3(end.x, end.y, end.z), cutwalls, cutfloors, checkwallnumber, false);
		}
	}
}

void Floor::CutAll(const Vector3 &start, const Vector3 &end, bool cutwalls, bool cutfloors, bool prepare)
{
	//cuts all objects related to this floor (floor, interfloor, shafts, stairs and external)
	//Y values are relative to the floor's altitude

	//cut current level
	Cut(start, end, cutwalls, cutfloors, false);

	//cut shafts
	for (int i = 1; i <= sbs->GetShaftCount(); i++)
	{
		if (sbs->GetShaft(i))
		{
			if (sbs->GetShaft(i)->GetLevel(Number))
				sbs->GetShaft(i)->GetLevel(Number)->Cut(false, start, end, cutwalls, cutfloors);
		}
	}

	Vector3 offset (0, GetBase(true), 0);

	//cut stairs
	for (int i = 1; i <= sbs->GetStairwellCount(); i++)
	{
		if (sbs->GetStairwell(i))
		{
			if (sbs->GetStairwell(i)->GetLevel(Number))
				sbs->GetStairwell(i)->GetLevel(Number)->Cut(false, start - offset, end - offset, cutwalls, cutfloors);
		}
	}

	//cut external
	if (sbs->External)
	{
		for (size_t i = 0; i < sbs->External->Walls.size(); i++)
		{
			if (!sbs->External->Walls[i])
				continue;

			sbs->GetUtility()->Cut(sbs->External->Walls[i], Vector3(start.x, Altitude + start.y, start.z), Vector3(end.x, Altitude + end.y, end.z), cutwalls, cutfloors);
		}
	}
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

	for (size_t i = 0; i < Group.size(); i++)
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
				for (int j = 1; j <= sbs->GetShaftCount(); j++)
				{
					Shaft *shaft = sbs->GetShaft(j);
					if (shaft)
					{
						if (shaft->IsEnabled == false)
						{
							if (shaft->GetLevel(Group[i]))
								shaft->GetLevel(Group[i])->Enabled(value, true);
						}
					}
				}
				for (int j = 1; j <= sbs->GetStairwellCount(); j++)
				{
					Stairwell *stairs = sbs->GetStairwell(j);
					if (stairs)
					{
						if (stairs->IsEnabled == false)
						{
							if (stairs->GetLevel(Group[i]))
								stairs->GetLevel(Group[i])->Enabled(value);
						}
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

Door* Floor::AddDoor(std::string name, const std::string &open_sound, const std::string &close_sound, bool open_state, const std::string &texture, const std::string &side_texture, Real thickness, const std::string &face_direction, const std::string &open_direction, bool rotate, Real open_speed, Real close_speed, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, Real tw, Real th, Real side_tw, Real side_th, bool external)
{
	//add a door to the floor

	//set up coordinates
	Real x1, z1, x2, z2;
	if (face_direction == "left" || face_direction == "right")
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

	Real base = 0.0;
	if (external == false)
		base = GetBase(true);

	//cut area
	if (face_direction == "left" || face_direction == "right")
		CutAll(Vector3(x1 - 1, base + voffset, z1), Vector3(x2 + 1, base + voffset + height, z2), true, false);
	else
		CutAll(Vector3(x1, base + voffset, z1 - 1), Vector3(x2, base + voffset + height, z2 + 1), true, false);

	//create an external (global) door if specified
	if (external == true)
		return sbs->GetDoorManager()->AddDoor(name, open_sound, close_sound, open_state, texture, side_texture, thickness, face_direction, open_direction, rotate, open_speed, close_speed, CenterX, CenterZ, width, height, Altitude + voffset, tw, th, side_tw, side_th);

	int number = (int)DoorArray.size();
	if (name == "")
		name = "Door " + ToString(number);

	Door* door = new Door(this, DoorWrapper, name, open_sound, close_sound, rotate);
	door->CreateDoor(open_state, texture, side_texture, thickness, face_direction, open_direction, open_speed, close_speed, CenterX, CenterZ, width, height, base + voffset, tw, th, side_tw, side_th);
	DoorArray.push_back(door);
	return door;
}

Door* Floor::CreateDoor(std::string name, const std::string &open_sound, const std::string &close_sound, bool rotate)
{
	//start creation of a manual door
	//since the door is unfinished, AddDoorComponent and FinishDoor need to be run on the returned Door object

	int number = (int)DoorArray.size();
	if (name == "")
		name = "Door " + ToString(number);

	Door* door = new Door(this, DoorWrapper, name, open_sound, close_sound, rotate);
	DoorArray.push_back(door);
	door->Move(0, GetBase(true), 0);
	return door;
}

bool Floor::CalculateAltitude()
{
	//calculate the floor's altitude in relation to floor below (or above it, if it's a basement level)
	//and return the altitude value

	//floors 0 or -1 can be created first.
	//if any of the other floors don't have an adjacent floor, return false

	if (Altitude == 0)
	{
		if (Number >= 0)
		{
			if (sbs->GetFloor(Number - 1))
				Altitude = sbs->GetFloor(Number - 1)->Altitude + sbs->GetFloor(Number - 1)->FullHeight();
			else if (Number != 0)
				return ReportError("Invalid floor number specified - no adjacent floor");
		}
		else
		{
			if (sbs->GetFloor(Number + 1))
				Altitude = sbs->GetFloor(Number + 1)->Altitude - FullHeight();
			else
			{
				if (Number == -1)
					Altitude = -FullHeight();
				else
					return ReportError("Invalid floor number specified - no adjacent floor");
			}
		}
	}

	//set altitude
	SetAltitude(Altitude);

	return true;
}

void Floor::EnableColumnFrame(bool value)
{
	//enable/disable columnframe mesh
	ColumnFrame->Enabled(value);
	IsColumnFrameEnabled = value;
}

void Floor::EnableInterfloor(bool value)
{
	//enable/disable interfloor mesh
	Interfloor->Enabled(value);
	IsInterfloorEnabled = value;
}

Wall* Floor::ColumnWallBox(const std::string &name, const std::string &texture, Real x1, Real x2, Real z1, Real z2, Real height_in, Real voffset, Real tw, Real th, bool inside, bool outside, bool top, bool bottom)
{
	//create columnframe wall box

	return sbs->CreateWallBox(ColumnFrame, name, texture, x1, x2, z1, z2, height_in, voffset, tw, th, inside, outside, top, bottom, true);
}

Wall* Floor::ColumnWallBox2(const std::string &name, const std::string &texture, Real CenterX, Real CenterZ, Real WidthX, Real LengthZ, Real height_in, Real voffset, Real tw, Real th, bool inside, bool outside, bool top, bool bottom)
{
	//create columnframe wall box from a central location

	return sbs->CreateWallBox2(ColumnFrame, name, texture, CenterX, CenterZ, WidthX, LengthZ, height_in, voffset, tw, th, inside, outside, top, bottom, true);
}

FloorIndicator* Floor::AddFloorIndicator(int elevator, int car, bool relative, const std::string &texture_prefix, const std::string &blank_texture, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset)
{
	//Creates a floor indicator at the specified location

	if (relative == false)
	{
		FloorIndicator *ind = new FloorIndicator(this, elevator, car, texture_prefix, blank_texture, direction, CenterX, CenterZ, width, height, GetBase(true) + voffset);
		FloorIndicatorArray.push_back(ind);
		return ind;
	}
	else
	{
		Elevator* elev = sbs->GetElevator(elevator);
		if (elev)
		{
			FloorIndicator *ind = new FloorIndicator(this, elevator, car, texture_prefix, blank_texture, direction, elev->GetPosition().x + CenterX, elev->GetPosition().z + CenterZ, width, height, GetBase(true) + voffset);
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

	for (size_t i = 0; i < FloorIndicatorArray.size(); i++)
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

	SBS_PROFILE("Floor::UpdateFloorIndicators");

	for (size_t i = 0; i < FloorIndicatorArray.size(); i++)
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

	SBS_PROFILE("Floor::Loop");

	LoopChildren();
}

std::vector<int> Floor::GetCallStations(int elevator)
{
	//get numbers of call stations that service the specified elevator

	std::vector<int> stations;
	stations.reserve(CallStationArray.size());
	for (size_t i = 0; i < CallStationArray.size(); i++)
	{
		//put station number onto the array if it serves the elevator
		if (CallStationArray[i])
		{
			if (CallStationArray[i]->ServicesElevator(elevator) == true)
				stations.push_back((int)i);
		}
	}
	return stations;
}

CallStation* Floor::GetCallStationForElevator(int elevator)
{
	//returns the first call button object that services the specified elevator

	for (size_t i = 0; i < CallStationArray.size(); i++)
	{
		if (CallStationArray[i])
		{
			if (CallStationArray[i]->ServicesElevator(elevator) == true)
				return CallStationArray[i];
		}
	}
	return 0;
}

CallStation* Floor::GetCallStation(int number)
{
	for (int i = 0; i < (int)CallStationArray.size(); i++)
	{
		if (CallStationArray[i])
		{
			if (CallStationArray[i]->Number == number)
				return CallStationArray[i];
		}
	}
	return 0;
}

void Floor::AddFillerWalls(const std::string &texture, Real thickness, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, bool direction, Real tw, Real th, bool isexternal)
{
	//convenience function for adding filler walls around doors
	//direction is either "false" for a door that faces left/right, or "true" for one that faces front/back

	Real x1 = 0, x2 = 0, z1 = 0, z2 = 0, depth1 = 0, depth2 = 0;

	//exit if no height given
	if (height == 0.0)
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
	if (isexternal == false)
		CutAll(Vector3(x1, GetBase(true) + voffset, z1), Vector3(x2, GetBase(true) + voffset + height, z2), true, false);
	else
		CutAll(Vector3(x1, voffset, z1), Vector3(x2, voffset + height, z2), true, false);

	//create walls
	sbs->DrawWalls(false, true, false, false, false, false);
	if (direction == false)
		AddWall("FillerWallLeft", texture, 0, x1, z1, x2, z1, height, height, voffset, voffset, tw, th, isexternal);
	else
		AddWall("FillerWallLeft", texture, 0, x1, z1, x1, z2, height, height, voffset, voffset, tw, th, isexternal);
	sbs->ResetWalls();

	sbs->DrawWalls(true, false, false, false, false, false);
	if (direction == false)
		AddWall("FillerWallRight", texture, 0, x1, z2, x2, z2, height, height, voffset, voffset, tw, th, isexternal);
	else
		AddWall("FillerWallRight", texture, 0, x2, z1, x2, z2, height, height, voffset, voffset, tw, th, isexternal);

	AddFloor("FillerWallTop", texture, 0, x1, z1, x2, z2, height + voffset, height + voffset, false, false, tw, th, isexternal);
	sbs->ResetWalls();
}

Sound* Floor::AddSound(const std::string &name, const std::string &filename, Vector3 position, bool loop, Real volume, int speed, Real min_distance, Real max_distance, Real doppler_level, Real cone_inside_angle, Real cone_outside_angle, Real cone_outside_volume, Vector3 direction)
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
	for (size_t i = 0; i < sounds.size(); i++)
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
	Object::Report("Floor " + ToString(Number) + ": " + message);
}

bool Floor::ReportError(const std::string &message)
{
	//general reporting function
	return Object::ReportError("Floor " + ToString(Number) + ": " + message);
}

Real Floor::GetBase(bool relative)
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

DirectionalIndicator* Floor::AddDirectionalIndicator(int elevator, int car, bool relative, bool active_direction, bool single, bool vertical, const std::string &BackTexture, const std::string &uptexture, const std::string &uptexture_lit, const std::string &downtexture, const std::string &downtexture_lit, Real CenterX, Real CenterZ, Real voffset, const std::string &direction, Real BackWidth, Real BackHeight, bool ShowBack, Real tw, Real th)
{
	//create a directional indicator on the specified floor, associated with a given elevator

	if (sbs->Verbose)
		Report("adding directional indicator");

	Elevator *elev = sbs->GetElevator(elevator);
	if (!elev)
		return 0;

	Real x, z;
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

	DirectionalIndicator *indicator = new DirectionalIndicator(this, elevator, car, Number, active_direction, single, vertical, BackTexture, uptexture, uptexture_lit, downtexture, downtexture_lit, x, z, GetBase(true) + voffset, direction, BackWidth, BackHeight, ShowBack, tw, th);
	DirIndicatorArray.push_back(indicator);
	return indicator;
}

void Floor::SetDirectionalIndicators(int elevator, int car, bool UpLight, bool DownLight)
{
	//set light status of all standard (non active-direction) directional indicators associated with the given elevator and car

	for (size_t i = 0; i < DirIndicatorArray.size(); i++)
	{
		DirectionalIndicator *indicator = DirIndicatorArray[i];

		if (indicator)
		{
			if (indicator->elevator == elevator && indicator->car == car && indicator->ActiveDirection == false)
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
	for (size_t i = 0; i < DirIndicatorArray.size(); i++)
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
	for (size_t i = 0; i < DirIndicatorArray.size(); i++)
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

Door* Floor::GetDoor(const std::string &name)
{
	for (int i = 0; i < DoorArray.size(); i++)
	{
		if (DoorArray[i]->GetName() == name)
			return DoorArray[i];
	}
	return 0;
}

void Floor::RemoveCallStation(CallStation* station)
{
	//remove a call station reference (does not delete the object itself)
	for (size_t i = 0; i < CallStationArray.size(); i++)
	{
		if (CallStationArray[i] == station)
		{
			CallStationArray.erase(CallStationArray.begin() + i);
			return;
		}
	}
}

void Floor::RemoveFloorIndicator(FloorIndicator *indicator)
{
	//remove a floor indicator from the array
	//this does not delete the object
	for (size_t i = 0; i < FloorIndicatorArray.size(); i++)
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
	for (size_t i = 0; i < DirIndicatorArray.size(); i++)
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
	for (size_t i = 0; i < DoorArray.size(); i++)
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
	for (size_t i = 0; i < sounds.size(); i++)
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
	for (size_t i = 0; i < lights.size(); i++)
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
	for (size_t i = 0; i < ModelArray.size(); i++)
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
	for (size_t i = 0; i < ControlArray.size(); i++)
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
	for (size_t i = 0; i < TriggerArray.size(); i++)
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
	for (size_t i = 0; i < CameraTextureArray.size(); i++)
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
	for (size_t i = 0; i < EscalatorArray.size(); i++)
	{
		if (EscalatorArray[i] == escalator)
		{
			EscalatorArray.erase(EscalatorArray.begin() + i);
			return;
		}
	}
}

void Floor::RemoveMovingWalkway(MovingWalkway *walkway)
{
	//remove an escalator reference (does not delete the object itself)
	for (size_t i = 0; i < MovingWalkwayArray.size(); i++)
	{
		if (MovingWalkwayArray[i] == walkway)
		{
			MovingWalkwayArray.erase(MovingWalkwayArray.begin() + i);
			return;
		}
	}
}

Light* Floor::AddLight(const std::string &name, int type)
{
	//add a light

	Light* light = new Light(this, name, type);
	light->Move(Vector3(0, GetBase(true), 0));
	lights.push_back(light);
	return light;
}

Light* Floor::GetLight(const std::string &name)
{
	for (int i = 0; i < lights.size(); i++)
	{
		if (lights[i]->GetName() == name)
			return lights[i];
	}
	return 0;
}

Model* Floor::AddModel(const std::string &name, const std::string &filename, bool center, Vector3 position, Vector3 rotation, Real max_render_distance, Real scale_multiplier, bool enable_physics, Real restitution, Real friction, Real mass)
{
	//add a model
	Model* model = new Model(this, name, filename, center, position + Vector3(0, GetBase(true), 0), rotation, max_render_distance, scale_multiplier, enable_physics, restitution, friction, mass);
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

	for (size_t i = 0; i < ModelArray.size(); i++)
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

Control* Floor::AddControl(const std::string &name, const std::string &sound, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, int selection_position, std::vector<std::string> &action_names, std::vector<std::string> &textures)
{
	//add a control
	std::vector<Action*> actionnull; //not used
	Control* control = new Control(this, name, false, sound, action_names, actionnull, textures, direction, width, height, true, selection_position);
	control->Move(CenterX, GetBase(true) + voffset, CenterZ);
	ControlArray.push_back(control);
	return control;
}

Trigger* Floor::AddTrigger(const std::string &name, const std::string &sound_file, Vector3 &area_min, Vector3 &area_max, std::vector<std::string> &action_names)
{
	//add a trigger
	Trigger* trigger = new Trigger(this, name, false, sound_file, area_min, area_max, action_names);
	TriggerArray.push_back(trigger);
	trigger->Move(0, GetBase(true), 0);
	return trigger;
}

CameraTexture* Floor::AddCameraTexture(const std::string &name, int quality, Real fov, const Vector3 &position, bool use_rotation, const Vector3 &rotation)
{
	//add a camera texture
	CameraTexture* cameratexture = new CameraTexture(this, name, quality, fov, GetBase(true) + position, use_rotation, rotation);
	CameraTextureArray.push_back(cameratexture);
	return cameratexture;
}

Escalator* Floor::AddEscalator(const std::string &name, int run, Real speed, const std::string &sound_file, const std::string &riser_texture, const std::string &tread_texture, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real risersize, Real treadsize, int num_steps, Real voffset, Real tw, Real th)
{
	//add an escalator
	Escalator* escalator = new Escalator(this, name, run, speed, sound_file, riser_texture, tread_texture, direction, CenterX, CenterZ, width, risersize, treadsize, num_steps, GetBase(true) + voffset, tw, th);
	EscalatorArray.push_back(escalator);
	return escalator;
}

MovingWalkway* Floor::AddMovingWalkway(const std::string &name, int run, Real speed, const std::string &sound_file, const std::string &texture, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real treadsize, int num_steps, Real voffset, Real tw, Real th)
{
	//add an escalator
	MovingWalkway* walkway = new MovingWalkway(this, name, run, speed, sound_file, texture, direction, CenterX, CenterZ, width, treadsize, num_steps, GetBase(true) + voffset, tw, th);
	MovingWalkwayArray.push_back(walkway);
	return walkway;
}

void Floor::SetAltitude(Real altitude)
{
	//position object at altitude
	SetPositionY(altitude);
	Altitude = altitude;
	AltitudeSet = true;
}

void Floor::ShowInfo(bool detailed, bool display_header)
{
	//show information about this floor on the console

	//if detailed is true (default), show detailed information for this floor
	//otherwise, show a single-line listing suitable for showing in a list with other floors

	//if display_header is true, show header/key along with listing

	if (display_header == true)
		Object::Report("\n--- Floor Information ---\n");

	if (detailed == true)
	{
		Object::Report("Number: " + ToString(Number));
		Object::Report("ID: " + ID);
		Object::Report("Name: " + Name);
		Object::Report("Type: " + FloorType);
		Object::Report("Description: " + Description);
		Object::Report("Height: " + ToString(Height));
		Object::Report("InterfloorHeight: " + ToString(InterfloorHeight));
		Object::Report("FullHeight: " + ToString(FullHeight()));
		Object::Report("Altitude: " + ToString(Altitude));
		Object::Report("Base: " + ToString(GetBase()));

		std::vector<int> elevator_list, stairs_list, shaft_list;
		GetElevatorList(elevator_list);
		GetStairwellList(stairs_list);
		GetShaftList(shaft_list);

		std::string elevs;
		for (size_t i = 0; i < elevator_list.size(); i++)
		{
			elevs += ToString(elevator_list[i]);
			if (i < elevator_list.size() - 1)
				elevs += ", ";
		}
		Object::Report("Elevators servicing: " + elevs);

		std::string stairs;
		for (size_t i = 0; i < stairs_list.size(); i++)
		{
			stairs += ToString(stairs_list[i]);
			if (i < stairs_list.size() - 1)
				stairs += ", ";
		}
		Object::Report("Stairwells spanning: " + stairs);

		std::string shafts;
		for (size_t i = 0; i < shaft_list.size(); i++)
		{
			shafts += ToString(shaft_list[i]);
			if (i < shaft_list.size() - 1)
				shafts += ", ";
		}
		Object::Report("Shafts spanning: " + shafts);

		if (display_header == true)
			Object::Report("");
	}
	else
	{
		if (display_header == true)
			Object::Report("Number(ID)\t----\tName\t----\tType\t----\tHeight\t----\tIFloorHeight\t----\tAltitude\t----\tBase\t----\tDescription");

		Object::Report(ToString(Number) + "(" + ID + ")\t----\t" + Name + "\t----\t" + FloorType + "\t----\t" + ToString(Height) + "\t----\t" + ToString(InterfloorHeight) + "\t----\t" + ToString(Altitude) + "\t----\t" + ToString(GetBase()) + "\t----\t" + Description);
	}
}

void Floor::GetElevatorList(std::vector<int> &listing, bool get_locals, bool get_express, bool get_service)
{
	//return a list of elevators that service this floor

	for (int i = 1; i <= sbs->GetElevatorCount(); i++)
	{
		Elevator *elev = sbs->GetElevator(i);
		if (elev)
		{
			std::string type = SetCaseCopy(elev->Type, false);

			if (elev->IsServicedFloor(Number) == true)
			{
				if (get_locals == true && type == "local")
					listing.push_back(elev->Number);

				else if (get_express == true && type == "express")
					listing.push_back(elev->Number);

				else if (get_service == true && type == "service")
					listing.push_back(elev->Number);
			}
		}
	}
}

void Floor::GetStairwellList(std::vector<int> &listing)
{
	//return a list of stairwells that span this floor

	for (int i = 1; i <= sbs->GetStairwellCount(); i++)
	{
		Stairwell *stairs = sbs->GetStairwell(i);
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

	for (int i = 1; i <= sbs->GetShaftCount(); i++)
	{
		Shaft *shaft = sbs->GetShaft(i);
		if (shaft)
		{
			if (shaft->IsValidFloor(Number) == true)
				listing.push_back(shaft->ShaftNumber);
		}
	}
}

ElevatorRoute* Floor::GetDirectRoute(int DestinationFloor, std::string ElevatorType)
{
	//return elevator if this floor has a direct elevator connection to another floor,
	//based on the given elevator type

	SetCase(ElevatorType, false);
	std::vector<int> list;
	GetElevatorList(list);

	for (size_t i = 0; i < list.size(); i++)
	{
		Elevator *elev = sbs->GetElevator(list[i]);
		if (elev)
		{
			ElevatorCar *car = elev->GetCarForFloor(Number);
			if (car)
			{
				std::string type = SetCaseCopy(elev->Type, false);
				bool serviced = car->IsServicedFloor(DestinationFloor);
				CallStation *station = GetCallStationForElevator(elev->Number);

				if (serviced == true && type == ElevatorType && station)
				{
					ElevatorRoute* route = new ElevatorRoute(car, DestinationFloor);
					return route;
				}
			}
		}
	}

	return 0;
}

std::vector<int> Floor::GetDirectFloors(bool include_service)
{
	//return a list of floors that can be directly accessed by the elevators that service this floor

	std::vector<int> list, result;
	GetElevatorList(list);

	for (size_t i = 0; i < list.size(); i++)
	{
		Elevator *elev = sbs->GetElevator(list[i]);
		if (elev)
		{
			std::string type = SetCaseCopy(elev->Type, false);
			if (include_service == false && type == "service")
				continue;

			ElevatorCar *car = elev->GetCarForFloor(Number);
			if (car)
			{
				for (int j = 0; j < car->GetServicedFloorCount(); j++)
				{
					int floor = car->GetServicedFloor(j);

					//skip this floor
					if (floor == Number)
						continue;

					bool found = false;
					for (size_t k = 0; k < result.size(); k++)
					{
						//make sure floor is not already in result list
						if (result[k] == floor)
						{
							found = true;
							break;
						}
					}

					if (found == false)
						result.push_back(floor);
				}
			}
		}
	}

	return result;
}

Model* Floor::GetModel(std::string name)
{
	//get a model by name

	SetCase(name, false);

	for (size_t i = 0; i < ModelArray.size(); i++)
	{
		if (SetCaseCopy(ModelArray[i]->GetName(), false) == name)
			return ModelArray[i];
	}

	return 0;
}

RevolvingDoor* Floor::AddRevolvingDoor(std::string name, bool run, const std::string &soundfile, const std::string &texture, Real thickness, bool clockwise, int segments, Real speed, Real rotation, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, Real tw, Real th, bool external)
{
	//create an external (global) door if specified
	if (external == true)
		return sbs->GetRevolvingDoorManager()->AddDoor(name, run, soundfile, texture, thickness, clockwise, segments, speed, rotation, CenterX, CenterZ, width, height, Altitude + voffset, tw, th);

	int number = (int)RDoorArray.size();
	if (name == "")
		name = "RevolvingDoor " + ToString(number);

	RevolvingDoor* door = new RevolvingDoor(this, DoorWrapper, name, run, soundfile, texture, thickness, clockwise, segments, speed, rotation, CenterX, CenterZ, width, height, GetBase(true) + voffset, tw, th);
	RDoorArray.push_back(door);
	return door;
}

void Floor::RemoveRevolvingDoor(RevolvingDoor *door)
{
	//remove a door from the array
	//this does not delete the object
	for (size_t i = 0; i < RDoorArray.size(); i++)
	{
		if (RDoorArray[i] == door)
		{
			RDoorArray.erase(RDoorArray.begin() + i);
			return;
		}
	}
}

RevolvingDoor* Floor::GetRevolvingDoor(int number)
{
	//get door object
	if (number < (int)RDoorArray.size())
	{
		if (RDoorArray[number])
			return RDoorArray[number];
	}

	return 0;
}

CameraTexture* Floor::GetCameraTexture(int number)
{
	if (number < CameraTextureArray.size())
		return CameraTextureArray[number];
	else
		return 0;
}

int Floor::GetCallStationCount()
{
	return (int)CallStationArray.size();
}

}
