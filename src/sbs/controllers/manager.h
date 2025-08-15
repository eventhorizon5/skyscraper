/*
	Scalable Building Simulator - Manager Classes
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

#ifndef _SBS_MANAGER_H
#define _SBS_MANAGER_H

namespace SBS {

class SBSIMPEXP Manager : public Object
{
public:
	explicit Manager(Object* parent);
	virtual ~Manager() {};
	virtual int GetCount() = 0;
	virtual bool Loop() = 0;
};

class SBSIMPEXP FloorManager : public Manager
{
public:
	explicit FloorManager(Object *parent);
	~FloorManager() override;
	Floor* Create(int number);
	int GetCount() override; //all floors including basements
	Floor* Get(int number);
	Floor* GetIndex(int index);
	Floor* GetByID(const std::string &id);
	Floor* GetByNumberID(const std::string &id);
	void Remove(Floor *floor);
	bool EnableAll(bool value);
	DynamicMesh* GetFloorDynMesh() { return floors; }
	DynamicMesh* GetIFloorDynMesh() { return interfloors; }
	DynamicMesh* GetColumnDynMesh() { return columnframes; }
	bool Loop() override;

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

class SBSIMPEXP ElevatorManager : public Manager
{
public:
	explicit ElevatorManager(Object* parent);
	~ElevatorManager() override;
	Elevator* Create(int number);
	int GetCount() override;
	Elevator* Get(int number);
	Elevator* GetIndex(int index);
	void Remove(Elevator *elevator);
	bool EnableAll(bool value);
	bool Loop() override;

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

class SBSIMPEXP ShaftManager : public Manager
{
public:
	explicit ShaftManager(Object* parent);
	~ShaftManager() override;
	Shaft* Create(int number, Real CenterX, Real CenterZ, int _startfloor, int _endfloor);
	int GetCount() override;
	Shaft* Get(int number);
	Shaft* GetIndex(int index);
	void Remove(Shaft *shaft);
	bool EnableAll(bool value);
	bool Loop() override;

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

class SBSIMPEXP StairwellManager : public Manager
{
public:
	explicit StairwellManager(Object* parent);
	~StairwellManager() override;
	Stairwell* Create(int number, Real CenterX, Real CenterZ, int _startfloor, int _endfloor);
	int GetCount() override;
	Stairwell* Get(int number);
	Stairwell* GetIndex(int index);
	void Remove(Stairwell *stairs);
	bool EnableAll(bool value);
	bool Loop() override;

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

class SBSIMPEXP DoorManager : public Manager
{
public:
	explicit DoorManager(Object* parent);
	~DoorManager() override;
	Door* AddDoor(std::string name, const std::string &open_sound, const std::string &close_sound, bool open_state, const std::string &texture, const std::string &side_texture, Real thickness, const std::string &face_direction, const std::string &open_direction, bool rotate, Real open_speed, Real close_speed, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, Real tw, Real th, Real side_tw, Real side_th);
	Door* CreateDoor(std::string name, const std::string &open_sound, const std::string &close_sound, bool rotate);
	Door* GetDoor(const std::string &name);
	void RemoveDoor(Door *door);
	int GetCount() override;
	Door* GetIndex(int index);
	bool Loop() override;

private:
	std::vector<Door*> Array;
	DynamicMesh *wrapper; //door dynamic mesh wrapper
};

class SBSIMPEXP RevolvingDoorManager : public Manager
{
public:
	explicit RevolvingDoorManager(Object* parent);
	~RevolvingDoorManager() override;
	RevolvingDoor* AddDoor(std::string name, bool run, const std::string &soundfile, const std::string &texture, Real thickness, bool clockwise, int segments, Real speed, Real rotation, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, Real tw, Real th);
	void RemoveDoor(RevolvingDoor *door);
	int GetCount() override;
	RevolvingDoor* GetIndex(int index);
	bool Loop() override;

private:
	std::vector<RevolvingDoor*> Array;
	DynamicMesh *wrapper; //door dynamic mesh wrapper
};

class SBSIMPEXP VehicleManager : public Manager
{
public:
	explicit VehicleManager(Object* parent);
	~VehicleManager() override;
	Vehicle* Create(int number);
	int GetCount() override;
	Vehicle* Get(int number);
	Vehicle* GetIndex(int index);
	void Remove(Vehicle *elevator);
	bool Loop() override;

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

class SBSIMPEXP ControllerManager : public Manager
{
public:
	explicit ControllerManager(Object* parent);
	~ControllerManager() override;
	DispatchController* Create(int number);
	int GetCount() override;
	DispatchController* Get(int number);
	DispatchController* GetIndex(int index);
	void Remove(DispatchController *controller);
	bool Loop() override;

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

class SBSIMPEXP TeleporterManager : public Manager
{
public:
	explicit TeleporterManager(Object* parent);
	~TeleporterManager() override;
	Teleporter* Create(std::string name, const std::string &idle_sound, const std::string &teleport_sound, Real width, Real height, const Vector3 &destination);
	Teleporter* Get(const std::string &name);
	void Remove(Teleporter *teleporter);
	int GetCount() override;
	Teleporter* GetIndex(int index);
	bool Loop() override;

	bool teleported;

private:
	std::vector<Teleporter*> Array;
};

}

#endif
