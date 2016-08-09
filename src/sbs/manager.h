/* $Id$ */

/*
	Scalable Building Simulator - Manager Classes
	The Skyscraper Project - Version 1.10 Alpha
	Copyright (C)2004-2016 Ryan Thoryk
	http://www.skyscrapersim.com
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@skyscrapersim.com

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

#ifndef _SBS_MANAGER_H
#define _SBS_MANAGER_H

namespace SBS {

class SBSIMPEXP FloorManager : public Object
{
public:
	FloorManager(Object* parent);
	~FloorManager();
	Floor* Create(int number);
	int GetCount(); //all floors including basements
	Floor* Get(int number);
	Floor* GetIndex(int index);
	void Remove(Floor *floor);
	void EnableAll(bool value);
	DynamicMesh* GetFloorDynMesh() { return floors; }
	DynamicMesh* GetIFloorDynMesh() { return interfloors; }
	DynamicMesh* GetColumnDynMesh() { return columnframes; }

private:
	struct Map
	{
		int number; //floor number
		Floor* object; //floor object reference
	};

	std::vector<Map> Array; //floor object array

	//dynamic mesh objects
	DynamicMesh* floors;
	DynamicMesh* interfloors;
	DynamicMesh* columnframes;

	//function caching
	Floor* get_result;
	int get_number;
};

class SBSIMPEXP ElevatorManager : public Object
{
public:
	ElevatorManager(Object* parent);
	~ElevatorManager();
	Elevator* Create(int number);
	int GetCount();
	Elevator* Get(int number);
	Elevator* GetIndex(int index);
	void Remove(Elevator *elevator);
	void EnableAll(bool value);

private:
	struct Map
	{
		int number; //elevator number
		Elevator* object; //elevator object reference
	};

	std::vector<Map> Array; //elevator object array

	//function caching
	Elevator* get_result;
	int get_number;
};

class SBSIMPEXP ShaftManager : public Object
{
public:
	ShaftManager(Object* parent);
	~ShaftManager();
	Shaft* Create(int number, float CenterX, float CenterZ, int _startfloor, int _endfloor);
	int GetCount();
	Shaft* Get(int number);
	Shaft* GetIndex(int index);
	void Remove(Shaft *shaft);
	void EnableAll(bool value);

private:
	struct Map
	{
		int number; //shaft number
		Shaft* object; //shaft object reference
	};

	std::vector<Map> Array; //shaft object array

	//function caching
	Shaft* get_result;
	int get_number;
};

class SBSIMPEXP StairsManager : public Object
{
public:
	StairsManager(Object* parent);
	~StairsManager();
	Stairs* Create(int number, float CenterX, float CenterZ, int _startfloor, int _endfloor);
	int GetCount();
	Stairs* Get(int number);
	Stairs* GetIndex(int index);
	void Remove(Stairs *stairs);
	void EnableAll(bool value);

private:
	struct Map
	{
		int number; //stairwell number
		Stairs* object; //stairwell object reference
	};

	std::vector<Map> Array; //stairs object array

	//function caching
	Stairs* get_result;
	int get_number;
};

class SBSIMPEXP DoorManager : public Object
{
public:
	DoorManager(Object* parent);
	~DoorManager();
	Door* AddDoor(const std::string &open_sound, const std::string &close_sound, bool open_state, const std::string &texture, float thickness, int direction, float speed, float CenterX, float CenterZ, float width, float height, float voffset, float tw, float th);
	void RemoveDoor(Door *door);
	int GetCount();
	Door* GetIndex(int index);

private:
	std::vector<Door*> Array;
	DynamicMesh *wrapper; //door dynamic mesh wrapper
};

}

#endif
