/* $Id$ */

/*
	Scalable Building Simulator - Floor Class
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
#include "floor.h"
#include "camera.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

Floor::Floor(int number)
{
	//set up SBS object
	object = new Object();
	object->SetValues(this, sbs->object, "Floor", "", false);

	std::string buffer;

	//Set floor's object number
	Number = number;

	//Create primary level mesh
	buffer = Ogre::StringConverter::toString(Number);
	object->SetName(std::string("Floor " + buffer).c_str());
	buffer.insert(0, "Level ");
	TrimString(buffer);
	Level = new MeshObject(object, buffer.c_str());

	//Create interfloor mesh
	buffer = Ogre::StringConverter::toString(Number);
	buffer.insert(0, "Interfloor ");
	TrimString(buffer);
	Interfloor = new MeshObject(object, buffer.c_str());

	//Create columnframe mesh
	buffer = Ogre::StringConverter::toString(Number);
	buffer.insert(0, "ColumnFrame ");
	TrimString(buffer);
	ColumnFrame = new MeshObject(object, buffer.c_str());

	//set enabled flag
	IsEnabled = true;

	//init other variables
	Name = "";
	ID = "";
	FloorType = "";
	Description = "";
	IndicatorTexture = "";
	Altitude = 0;
	Height = 0;
	InterfloorHeight = 0;
}

Floor::~Floor()
{
	//Destructor

	//delete models
	for (int i = 0; i < (int)ModelArray.size(); i++)
	{
		if (ModelArray[i])
			delete ModelArray[i];
		ModelArray[i] = 0;
	}

	//delete lights
	for (int i = 0; i < (int)lights.size(); i++)
	{
		if (lights[i])
			delete lights[i];
		lights[i] = 0;
	}

	//delete call buttons
	for (int i = 0; i < (int)CallButtonArray.size(); i++)
	{
		if (CallButtonArray[i])
		{
			CallButtonArray[i]->object->parent_deleting = true;
			delete CallButtonArray[i];
		}
		CallButtonArray[i] = 0;
	}
	CallButtonArray.clear();

	//delete doors
	for (int i = 0; i < (int)DoorArray.size(); i++)
	{
		if (DoorArray[i])
		{
			DoorArray[i]->object->parent_deleting = true;
			delete DoorArray[i];
		}
		DoorArray[i] = 0;
	}
	DoorArray.clear();

	//delete floor indicators
	for (int i = 0; i < (int)FloorIndicatorArray.size(); i++)
	{
		if (FloorIndicatorArray[i])
		{
			FloorIndicatorArray[i]->object->parent_deleting = true;
			delete FloorIndicatorArray[i];
		}
	}
	FloorIndicatorArray.clear();

	//delete directional indicators
	for (int i = 0; i < (int)DirIndicatorArray.size(); i++)
	{
		if (DirIndicatorArray[i])
		{
			DirIndicatorArray[i]->object->parent_deleting = true;
			delete DirIndicatorArray[i];
		}
	}
	DirIndicatorArray.clear();

	//delete sounds
	for (int i = 0; i < (int)sounds.size(); i++)
	{
		if (sounds[i])
		{
			sounds[i]->object->parent_deleting = true;
			delete sounds[i];
		}
		sounds[i] = 0;
	}
	sounds.clear();

	//delete meshes
	if (Level)
		delete Level;
	Level = 0;

	if (Interfloor)
		delete Interfloor;
	Interfloor = 0;

	if (ColumnFrame)
		delete ColumnFrame;
	ColumnFrame = 0;

	//unregister from parent
	if (sbs->FastDelete == false)
	{
		if (object->parent_deleting == false)
			sbs->RemoveFloor(this);
	}

	delete object;
}

void Floor::SetCameraFloor()
{
	//Moves camera to specified floor (sets altitude to the floor's base plus DefaultAltitude)

	Ogre::Vector3 camlocation = sbs->camera->GetPosition();
	sbs->camera->SetPosition(Ogre::Vector3(camlocation.x, GetBase() + sbs->camera->cfg_body_height + sbs->camera->cfg_legs_height, camlocation.z));
}

WallObject* Floor::AddFloor(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, float tw, float th, bool isexternal)
{
	//Adds a floor with the specified dimensions and vertical offset

	WallObject *wall;

	if (isexternal == false)
	{
		wall = Level->CreateWallObject(this->object, name);
		sbs->AddFloorMain(wall, name, texture, thickness, x1, z1, x2, z2, GetBase() + voffset1, GetBase() + voffset2, tw, th, true);
	}
	else
	{
		wall = sbs->External->CreateWallObject(this->object, name);
		sbs->AddFloorMain(wall, name, texture, thickness, x1, z1, x2, z2, Altitude + voffset1, Altitude + voffset2, tw, th, true);
	}
	return wall;
}

WallObject* Floor::AddInterfloorFloor(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, float tw, float th)
{
	//Adds an interfloor floor with the specified dimensions and vertical offset

	WallObject *wall = Interfloor->CreateWallObject(this->object, name);
	sbs->AddFloorMain(wall, name, texture, thickness, x1, z1, x2, z2, Altitude + voffset1, Altitude + voffset2, tw, th, true);
	return wall;
}

WallObject* Floor::AddWall(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float voffset1, float voffset2, float tw, float th, bool isexternal)
{
	//Adds a wall with the specified dimensions

	WallObject *wall;
	if (isexternal == false)
	{
		wall = Level->CreateWallObject(this->object, name);
		sbs->AddWallMain(wall, name, texture, thickness, x1, z1, x2, z2, height_in1, height_in2, GetBase() + voffset1, GetBase() + voffset2, tw, th, true);
	}
	else
	{
		wall = sbs->External->CreateWallObject(this->object, name);
		sbs->AddWallMain(wall, name, texture, thickness, x1, z1, x2, z2, height_in1, height_in2, Altitude + voffset1, Altitude + voffset2, tw, th, true);
	}
	return wall;
}

WallObject* Floor::AddInterfloorWall(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float voffset1, float voffset2, float tw, float th)
{
	//Adds an interfloor wall with the specified dimensions

	WallObject *wall = Interfloor->CreateWallObject(this->object, name);
	sbs->AddWallMain(wall, name, texture, thickness, x1, z1, x2, z2, height_in1, height_in2, Altitude + voffset1, Altitude + voffset2, tw, th, true);
	return wall;
}

void Floor::Enabled(bool value)
{
	//turns floor on/off

	if (IsEnabled == value)
		return;

	Level->Enable(value);
	Interfloor->Enable(value);
	IsEnabled = value;

	EnableColumnFrame(value);

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

	//sounds
	for (int i = 0; i < (int)sounds.size(); i++)
	{
		if (sounds[i])
		{
			if (value == false)
				sounds[i]->Stop();
			else
				sounds[i]->Play();
		}
	}
}

float Floor::FullHeight()
{
	//calculate full height of a floor
	return InterfloorHeight + Height;
}

Object* Floor::AddCallButtons(std::vector<int> &elevators, const char *BackTexture, const char *UpButtonTexture, const char *UpButtonTexture_Lit, const char *DownButtonTexture, const char *DownButtonTexture_Lit, float CenterX, float CenterZ, float voffset, const char *direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th)
{
	//adds call buttons

	//check if any of the elevators serve the current floor
	bool check = false;
	for (int i = 0; i < (int)elevators.size(); i++)
	{
		if (sbs->GetElevator(elevators[i]))
		{
			if (sbs->GetElevator(elevators[i])->IsServicedFloor(Number) == true)
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
	CallButtonArray.resize(CallButtonArray.size() + 1);
	int Current = (int)CallButtonArray.size() - 1;
	CallButtonArray[Current] = new CallButton(elevators, Number, Current, BackTexture, UpButtonTexture, UpButtonTexture_Lit, DownButtonTexture, DownButtonTexture_Lit, CenterX, CenterZ, voffset, direction, BackWidth, BackHeight, ShowBack, tw, th);
	return CallButtonArray[Current]->object;
}

void Floor::Cut(const Ogre::Vector3 &start, const Ogre::Vector3 &end, bool cutwalls, bool cutfloors, bool fast, int checkwallnumber, const char *checkstring)
{
	//caller to SBS cut function
	//Y values are relative to the floor's altitude
	//if fast is specified, skips the interfloor scan

	for (int i = 0; i < (int)Level->Walls.size(); i++)
	{
		bool reset = true;
		if (i > 0)
			reset = false;

		sbs->Cut(Level->Walls[i], Ogre::Vector3(start.x, Altitude + start.y, start.z), Ogre::Vector3(end.x, Altitude + end.y, end.z), cutwalls, cutfloors, Ogre::Vector3(0, 0, 0), Ogre::Vector3(0, 0, 0), checkwallnumber, checkstring, reset);
	}
	if (fast == false)
	{
		for (int i = 0; i < (int)Interfloor->Walls.size(); i++)
		{
			sbs->Cut(Interfloor->Walls[i], Ogre::Vector3(start.x, Altitude + start.y, start.z), Ogre::Vector3(end.x, Altitude + end.y, end.z), cutwalls, cutfloors, Ogre::Vector3(0, 0, 0), Ogre::Vector3(0, 0, 0), checkwallnumber, checkstring, false);
		}
	}
}

void Floor::CutAll(const Ogre::Vector3 &start, const Ogre::Vector3 &end, bool cutwalls, bool cutfloors)
{
	//cuts all objects related to this floor (floor, interfloor, shafts, stairs and external)
	//Y values are relative to the floor's altitude

	//cut current level
	Cut(start, end, cutwalls, cutfloors, false);

	//cut shafts
	for (int i = 1; i <= sbs->Shafts(); i++)
	{
		if (cutwalls == true)
			sbs->GetShaft(i)->CutWall(false, Number, start, end);
		if (cutfloors == true)
			sbs->GetShaft(i)->CutFloors(false, Ogre::Vector2(start.x, start.z), Ogre::Vector2(end.x, end.z), Altitude, Altitude + FullHeight());
	}

	//cut stairs
	for (int i = 1; i <= sbs->StairsNum(); i++)
	{
		if (cutwalls == true)
			sbs->GetStairs(i)->CutWall(false, Number, start, end);
		if (cutfloors == true)
			sbs->GetStairs(i)->CutFloors(false, Ogre::Vector2(start.x, start.z), Ogre::Vector2(end.x, end.z), Altitude, Altitude + FullHeight());
	}

	//cut external
	for (int i = 0; i < (int)sbs->External->Walls.size(); i++)
		sbs->Cut(sbs->External->Walls[i], Ogre::Vector3(start.x, Altitude + start.y, start.z), Ogre::Vector3(end.x, Altitude + end.y, end.z), cutwalls, cutfloors, Ogre::Vector3(0, 0, 0), Ogre::Vector3(0, 0, 0));
}

void Floor::AddGroupFloor(int number)
{
	//adds a floor number to the group list.
	//Groups are used to enable multiple floors at the same time when
	//a user arrives at a floor

	int index = -1;
	for (int i = 0; i < (int)Group.size(); i++)
	{
		if (Group[i] == number)
			index = i;
	}
	if (index == -1)
	{
		Group.push_back(number);
		std::sort(Group.begin(), Group.end());
	}
}

void Floor::RemoveGroupFloor(int number)
{
	//removes a floor number from the group list

	int index = -1;
	for (int i = 0; i < (int)Group.size(); i++)
	{
		if (Group[i] == number)
			index = i;
	}
	if (index != -1)
	{
		for (int i = 0; i < (int)Group.size(); i++)
		{
			if (Group[i] == number)
				Group.erase(Group.begin() + i);
		}
	}
}

void Floor::EnableGroup(bool value)
{
	//enable floors grouped with this floor

	if (Group.size() > 0)
	{
		for (size_t i = 0; i < Group.size(); i++)
		{
			//check if floor exists
			if (sbs->GetFloor(Group[i]))
			{
				//enable other floor
				sbs->GetFloor(Group[i])->Enabled(value);

				//enable shafts and stairs for other floor
				for (int j = 1; j <= sbs->Shafts(); j++)
				{
					if (sbs->GetShaft(j))
						sbs->GetShaft(j)->Enabled(Group[i], value, true);
				}
				for (int j = 1; j <= sbs->StairsNum(); j++)
				{
					if (sbs->GetStairs(j))
						sbs->GetStairs(j)->Enabled(Group[i], value);
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

Object* Floor::AddDoor(const char *open_sound, const char *close_sound, bool open_state, const char *texture, float thickness, int direction, float speed, float CenterX, float CenterZ, float width, float height, float voffset, float tw, float th)
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

	DoorArray.resize(DoorArray.size() + 1);
	std::string floornum = _itoa(Number, intbuffer, 10);
	std::string num = _itoa((int)DoorArray.size() - 1, intbuffer, 10);
	DoorArray[DoorArray.size() - 1] = new Door(this->object, std::string("Floor " + floornum + ":Door " + num).c_str(), open_sound, close_sound, open_state, texture, thickness, direction, speed, CenterX, CenterZ, width, height, voffset + GetBase(), tw, th);
	return DoorArray[DoorArray.size() - 1]->object;
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
	return true;
}

void Floor::EnableColumnFrame(bool value)
{
	//enable/disable columnframe mesh
	ColumnFrame->Enable(value);
	IsColumnFrameEnabled = value;
}

WallObject* Floor::ColumnWallBox(const char *name, const char *texture, float x1, float x2, float z1, float z2, float height_in, float voffset, float tw, float th, bool inside, bool outside, bool top, bool bottom)
{
	//create columnframe wall box

	WallObject *wall = ColumnFrame->CreateWallObject(this->object, name);
	sbs->CreateWallBox(wall, name, texture, x1, x2, z1, z2, height_in, Altitude + voffset, tw, th, inside, outside, top, bottom, true);
	return wall;
}

WallObject* Floor::ColumnWallBox2(const char *name, const char *texture, float CenterX, float CenterZ, float WidthX, float LengthZ, float height_in, float voffset, float tw, float th, bool inside, bool outside, bool top, bool bottom)
{
	//create columnframe wall box from a central location

	WallObject *wall = ColumnFrame->CreateWallObject(this->object, name);
	sbs->CreateWallBox2(wall, name, texture, CenterX, CenterZ, WidthX, LengthZ, height_in, Altitude + voffset, tw, th, inside, outside, top, bottom, true);
	return wall;
}

Object* Floor::AddFloorIndicator(int elevator, bool relative, const char *texture_prefix, const char *direction, float CenterX, float CenterZ, float width, float height, float voffset)
{
	//Creates a floor indicator at the specified location

	int size = (int)FloorIndicatorArray.size();
	FloorIndicatorArray.resize(size + 1);

	if (relative == false)
	{
		FloorIndicatorArray[size] = new FloorIndicator(object, elevator, texture_prefix, direction, CenterX, CenterZ, width, height, GetBase() + voffset);
		return FloorIndicatorArray[size]->object;
	}
	else
	{
		Elevator* elev = sbs->GetElevator(elevator);
		if (elev)
		{
			FloorIndicatorArray[size] = new FloorIndicator(object, elevator, texture_prefix, direction, elev->Origin.x + CenterX, elev->Origin.z + CenterZ, width, height, GetBase() + voffset);
			return FloorIndicatorArray[size]->object;
		}
		else
			return 0;
	}
}

void Floor::UpdateFloorIndicators(int elevator)
{
	//changes the number texture on the floor indicators to the specified elevator's current floor

	std::string value;
	for (int i = 0; i < (int)FloorIndicatorArray.size(); i++)
	{
		if (FloorIndicatorArray[i])
		{
			if (FloorIndicatorArray[i]->elev == elevator)
			{
				Elevator *elev = sbs->GetElevator(elevator);
				if (elev->UseFloorSkipText == true && elev->IsServicedFloor(elev->GetFloor()) == false)
					value = elev->GetFloorSkipText();
				else
					value = sbs->GetFloor(elev->GetFloor())->ID;
				TrimString(value);
				FloorIndicatorArray[i]->Update(value.c_str());
			}
		}
	}
}

void Floor::UpdateFloorIndicators()
{
	//changes the number texture on the floor indicators

	std::string value;
	for (int i = 0; i < (int)FloorIndicatorArray.size(); i++)
	{
		if (FloorIndicatorArray[i])
		{
			Elevator *elevator = sbs->GetElevator(FloorIndicatorArray[i]->elev);
			if (elevator->UseFloorSkipText == true && elevator->IsServicedFloor(elevator->GetFloor()) == false)
				value = elevator->GetFloorSkipText();
			else
				value = sbs->GetFloor(elevator->GetFloor())->ID;
			TrimString(value);
			FloorIndicatorArray[i]->Update(value.c_str());
		}
	}
}

void Floor::Loop()
{
	//floor object main loop; runs if camera is currently on this floor

}

std::vector<int> Floor::GetCallButtons(int elevator)
{
	//get numbers of call buttons that service the specified elevator
	
	std::vector<int> buttons;
	for (int i = 0; i < (int)CallButtonArray.size(); i++)
	{
		//put button number onto the array if it serves the elevator
		if (CallButtonArray[i]->ServicesElevator(elevator) == true)
			buttons.push_back(i);
	}
	return buttons;
}

void Floor::AddFillerWalls(const char *texture, float thickness, float CenterX, float CenterZ, float width, float height, float voffset, bool direction, float tw, float th)
{
	//convenience function for adding filler walls around doors
	//direction is either "false" for a door that faces left/right, or "true" for one that faces front/back

	float x1 = 0, x2 = 0, z1 = 0, z2 = 0, depth1 = 0, depth2 = 0;

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
	{
		AddWall("FillerWallRight", texture, 0, x1, z2, x2, z2, height, height, voffset, voffset, tw, th, false);
	}
	else
	{
		AddWall("FillerWallRight", texture, 0, x2, z1, x2, z2, height, height, voffset, voffset, tw, th, false);
	}
	AddFloor("FillerWallTop", texture, 0, x1, z1, x2, z2, height + voffset, height + voffset, tw, th, false);
	sbs->ResetWalls();
}

Object* Floor::AddSound(const char *name, const char *filename, Ogre::Vector3 position, int volume, int speed, float min_distance, float max_distance, float dir_radiation, Ogre::Vector3 direction)
{
	//create a looping sound object
	sounds.resize(sounds.size() + 1);
	Sound *sound = sounds[sounds.size() - 1];
	sound = new Sound(this->object, name, false);

	//set parameters and play sound
	sound->SetPosition(Ogre::Vector3(position.x, GetBase() + position.y, position.z));
	sound->SetDirection(direction);
	sound->SetVolume(volume);
	sound->SetSpeed(speed);
	sound->SetDistances(min_distance, max_distance);
	sound->SetDirection(direction);
	sound->SetDirectionalRadiation(dir_radiation);
	sound->Load(filename);
	sound->Loop(true);
	sound->Play();

	return sound->object;
}

void Floor::Report(std::string message)
{
	//general reporting function
	sbs->Report("Floor " + std::string(_itoa(Number, intbuffer, 10)) + ": " + message);
}

bool Floor::ReportError(std::string message)
{
	//general reporting function
	return sbs->ReportError("Floor " + std::string(_itoa(Number, intbuffer, 10)) + ": " + message);
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

Object* Floor::AddDirectionalIndicator(int elevator, bool relative, bool active_direction, bool single, bool vertical, const char *BackTexture, const char *uptexture, const char *uptexture_lit, const char *downtexture, const char *downtexture_lit, float CenterX, float CenterZ, float voffset, const char *direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th)
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
		x = elev->Origin.x + CenterX;
		z = elev->Origin.z + CenterZ;
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

	int index = (int)DirIndicatorArray.size();
	DirIndicatorArray.resize(index + 1);
	DirIndicatorArray[index] = new DirectionalIndicator(object, elevator, Number, active_direction, single, vertical, BackTexture, uptexture, uptexture_lit, downtexture, downtexture_lit, x, z, voffset, direction, BackWidth, BackHeight, ShowBack, tw, th);
	return DirIndicatorArray[index]->object;
}

void Floor::SetDirectionalIndicators(int elevator, bool UpLight, bool DownLight)
{
	//set light status of all standard (non active-direction) directional indicators associated with the given elevator

	for (int i = 0; i < (int)DirIndicatorArray.size(); i++)
	{
		if (DirIndicatorArray[i]->elevator_num == elevator && DirIndicatorArray[i]->ActiveDirection == false)
		{
			DirIndicatorArray[i]->DownLight(DownLight);
			DirIndicatorArray[i]->UpLight(UpLight);
		}
	}
}

void Floor::UpdateDirectionalIndicators(int elevator)
{
	//updates the active-direction indicators associated with the given elevator

	for (int i = 0; i < (int)DirIndicatorArray.size(); i++)
	{
		if (DirIndicatorArray[i])
		{
			if (DirIndicatorArray[i]->elevator_num == elevator && DirIndicatorArray[i]->ActiveDirection == true)
			{
				Elevator *elev = sbs->GetElevator(elevator);
				if (elev->ActiveDirection == 1)
				{
					DirIndicatorArray[i]->UpLight(true);
					DirIndicatorArray[i]->DownLight(false);
				}
				if (elev->ActiveDirection == 0)
				{
					DirIndicatorArray[i]->UpLight(false);
					DirIndicatorArray[i]->DownLight(false);
				}
				if (elev->ActiveDirection == -1)
				{
					DirIndicatorArray[i]->UpLight(false);
					DirIndicatorArray[i]->DownLight(true);
				}
			}
		}
	}
}

void Floor::UpdateDirectionalIndicators()
{
	//updates all active-direction indicators

	std::string value;
	for (int i = 0; i < (int)DirIndicatorArray.size(); i++)
	{
		if (DirIndicatorArray[i])
		{
			if (DirIndicatorArray[i]->ActiveDirection == true)
			{
				Elevator *elev = sbs->GetElevator(DirIndicatorArray[i]->elevator_num);
				if (elev->ActiveDirection == 1)
				{
					DirIndicatorArray[i]->UpLight(true);
					DirIndicatorArray[i]->DownLight(false);
				}
				if (elev->ActiveDirection == 0)
				{
					DirIndicatorArray[i]->UpLight(false);
					DirIndicatorArray[i]->DownLight(false);
				}
				if (elev->ActiveDirection == -1)
				{
					DirIndicatorArray[i]->UpLight(false);
					DirIndicatorArray[i]->DownLight(true);
				}
			}
		}
	}
}

void Floor::OpenDoor(int number)
{
	//open door
	if (number < (int)DoorArray.size())
	{
		if (DoorArray[number])
			DoorArray[number]->Open();
	}
	else
		Report("Invalid door " + std::string(_itoa(number, intbuffer, 10)));
}

void Floor::CloseDoor(int number)
{
	//close door
	if (number < (int)DoorArray.size())
	{
		if (DoorArray[number])
			DoorArray[number]->Close();
	}
	else
		Report("Invalid door " + std::string(_itoa(number, intbuffer, 10)));
}

bool Floor::IsDoorOpen(int number)
{
	//check to see if door is open
	if (number < (int)DoorArray.size())
	{
		if (DoorArray[number])
			return DoorArray[number]->IsOpen();
	}
	else
		Report("Invalid door " + std::string(_itoa(number, intbuffer, 10)));
	return false;
}

bool Floor::IsDoorMoving(int number)
{
	//check to see if door is moving
	if (number < (int)DoorArray.size())
	{
		if (DoorArray[number])
			return DoorArray[number]->IsMoving;
	}
	else
		Report("Invalid door " + std::string(_itoa(number, intbuffer, 10)));
	return false;
}

void Floor::RemoveCallButton(CallButton *callbutton)
{
	//remove a call button object from the array
	//this does not delete the object
	for (int i = 0; i < (int)CallButtonArray.size(); i++)
	{
		if (CallButtonArray[i] == callbutton)
			CallButtonArray.erase(CallButtonArray.begin() + i);
	}
}

void Floor::RemoveFloorIndicator(FloorIndicator *indicator)
{
	//remove a floor indicator from the array
	//this does not delete the object
	for (int i = 0; i < (int)FloorIndicatorArray.size(); i++)
	{
		if (FloorIndicatorArray[i] == indicator)
			FloorIndicatorArray.erase(FloorIndicatorArray.begin() + i);
	}
}

void Floor::RemoveDirectionalIndicator(DirectionalIndicator *indicator)
{
	//remove a directional indicator from the array
	//this does not delete the object
	for (int i = 0; i < (int)DirIndicatorArray.size(); i++)
	{
		if (DirIndicatorArray[i] == indicator)
			DirIndicatorArray.erase(DirIndicatorArray.begin() + i);
	}
}

void Floor::RemoveDoor(Door *door)
{
	//remove a door from the array
	//this does not delete the object
	for (int i = 0; i < (int)DoorArray.size(); i++)
	{
		if (DoorArray[i] == door)
			DoorArray.erase(DoorArray.begin() + i);
	}
}

void Floor::RemoveSound(Sound *sound)
{
	//remove a sound from the array
	//this does not delete the object
	for (int i = 0; i < (int)sounds.size(); i++)
	{
		if (sounds[i] == sound)
			sounds.erase(sounds.begin() + i);
	}
}

Object* Floor::AddLight(const char *name, int type, Ogre::Vector3 position, Ogre::Vector3 direction, float radius, float max_distance, float color_r, float color_g, float color_b, float spec_color_r, float spec_color_g, float spec_color_b, float directional_cutoff_radius, float spot_falloff_inner, float spot_falloff_outer)
{
	//add a light

	Light* light = new Light(name, type, position + Ogre::Vector3(0, GetBase(), 0), direction, radius, max_distance, color_r, color_g, color_b, spec_color_r, spec_color_g, spec_color_b, directional_cutoff_radius, spot_falloff_inner, spot_falloff_outer);
	lights.push_back(light);
	return light->object;
}

Object* Floor::AddModel(const char *name, const char *filename, Ogre::Vector3 position, Ogre::Vector3 rotation, float max_render_distance, float scale_multiplier)
{
	//add a model
	Model* model = new Model(name, filename, position + Ogre::Vector3(0, GetBase(), 0), rotation, max_render_distance, scale_multiplier);
	if (model->load_error == true)
	{
		delete model;
		return 0;
	}
	ModelArray.push_back(model);
	return model->object;
}
