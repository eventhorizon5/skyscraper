/*
	Scalable Building Simulator - Manager Classes
	The Skyscraper Project - Version 1.12 Alpha
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
	Floor* GetByID(const std::string &id);
	Floor* GetByNumberID(const std::string &id);
	void Remove(Floor *floor);
	void EnableAll(bool value);
	DynamicMesh* GetFloorDynMesh() { return floors; }
	DynamicMesh* GetIFloorDynMesh() { return interfloors; }
	DynamicMesh* GetColumnDynMesh() { return columnframes; }
	void Loop();

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
	void Loop();

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
	Shaft* Create(int number, Real CenterX, Real CenterZ, int _startfloor, int _endfloor);
	int GetCount();
	Shaft* Get(int number);
	Shaft* GetIndex(int index);
	void Remove(Shaft *shaft);
	void EnableAll(bool value);
	void Loop();

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

class SBSIMPEXP StairwellManager : public Object
{
public:
	StairwellManager(Object* parent);
	~StairwellManager();
	Stairwell* Create(int number, Real CenterX, Real CenterZ, int _startfloor, int _endfloor);
	int GetCount();
	Stairwell* Get(int number);
	Stairwell* GetIndex(int index);
	void Remove(Stairwell *stairs);
	void EnableAll(bool value);
	void Loop();

private:
	struct Map
	{
		int number; //stairwell number
		Stairwell* object; //stairwell object reference
	};

	std::vector<Map> Array; //stairs object array

	//function caching
	Stairwell* get_result;
	int get_number;
};

class SBSIMPEXP DoorManager : public Object
{
public:
	DoorManager(Object* parent);
	~DoorManager();
	Door* AddDoor(std::string name, const std::string &open_sound, const std::string &close_sound, bool open_state, const std::string &texture, const std::string &side_texture, Real thickness, const std::string &face_direction, const std::string &open_direction, bool rotate, Real open_speed, Real close_speed, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, Real tw, Real th, Real side_tw, Real side_th);
	Door* CreateDoor(std::string name, const std::string &open_sound, const std::string &close_sound, bool rotate);
	Door* GetDoor(const std::string &name);
	void RemoveDoor(Door *door);
	int GetCount();
	Door* GetIndex(int index);
	void Loop();

private:
	std::vector<Door*> Array;
	DynamicMesh *wrapper; //door dynamic mesh wrapper
};

class SBSIMPEXP RevolvingDoorManager : public Object
{
public:
	RevolvingDoorManager(Object* parent);
	~RevolvingDoorManager();
	RevolvingDoor* AddDoor(std::string name, bool run, const std::string &soundfile, const std::string &texture, Real thickness, bool clockwise, int segments, Real speed, Real rotation, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, Real tw, Real th);
	void RemoveDoor(RevolvingDoor *door);
	int GetCount();
	RevolvingDoor* GetIndex(int index);
	void Loop();

private:
	std::vector<RevolvingDoor*> Array;
	DynamicMesh *wrapper; //door dynamic mesh wrapper
};

class SBSIMPEXP VehicleManager : public Object
{
public:
	VehicleManager(Object* parent);
	~VehicleManager();
	Vehicle* Create(int number);
	int GetCount();
	Vehicle* Get(int number);
	Vehicle* GetIndex(int index);
	void Remove(Vehicle *elevator);
	void Loop();

private:
	struct Map
	{
		int number; //vehicle number
		Vehicle* object; //vehicle object reference
	};

	std::vector<Map> Array; //vehicle object array

	//function caching
	Vehicle* get_result;
	int get_number;
};

class SBSIMPEXP ControllerManager : public Object
{
public:
	ControllerManager(Object* parent);
	~ControllerManager();
	DispatchController* Create(int number);
	int GetCount();
	DispatchController* Get(int number);
	DispatchController* GetIndex(int index);
	void Remove(DispatchController *controller);
	void Loop();

private:
	struct Map
	{
		int number; //controller number
		DispatchController* object; //controller object reference
	};

	std::vector<Map> Array; //controller object array

	//function caching
	DispatchController* get_result;
	int get_number;
};

}

#endif
