/*
	Scalable Building Simulator - Door System
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

#ifndef _SBS_DOORSYSTEM_H
#define _SBS_DOORSYSTEM_H

namespace SBS {

struct DoorWrapper;

//door component object
struct DoorComponent
{
	DoorComponent(const std::string &doorname, DoorWrapper *Wrapper, const std::string &direction, bool OpenClockwise, Real OpenSpeed, Real CloseSpeed, DynamicMesh *dynmesh);
	~DoorComponent();
	void MoveDoors(bool open, bool manual);
	void RotateDoors(bool open);
	void Move();
	void Reset(bool open);

	MeshObject* mesh;
	int direction; //direction is either 0 for up, 1 for down, 2 for left/forward and 3 for right/backward
	Real open_speed;
	Real close_speed;
	Real active_speed;
	Real openchange;
	std::string name;
	DoorWrapper *wrapper; //associated wrapper
	Real marker1;
	Real marker2;
	int door_section; //door movement section; used for both reversal tracking and debugging
	Real stopping_distance;
	Real temp_change;
	bool accelerating;
	bool is_open;
	bool finished;
	Vector3 extents_min;
	Vector3 extents_max;
	bool sign_changed;
	Real old_difference;
	Real offset;
	bool recheck_difference;
	bool reversed;
	Real rotation;
	bool Clockwise; //if door should rotate clockwise
};

//wrapper that represents the entire set of doors
struct DoorWrapper : public Object
{
	DoorWrapper(Object *parent_obj, ElevatorDoor *door_object, bool shaftdoor, int shaftdoor_floor = 0);
	DoorWrapper(Door *parent, bool rotate);
	~DoorWrapper();

	DoorComponent* CreateDoor(const std::string &doorname, const std::string &Direction, bool OpenClockwise, Real OpenSpeed, Real CloseSpeed, DynamicMesh *dynmesh);
	void Enabled(bool value);
	bool CheckDoorsOpen();
	bool IsFinished();
	void ResetFinished();
	void MoveDoors(bool open, bool manual = false);
	void StopDoors();
	void ResetState();
	void OnClick(Vector3 &position, bool shift, bool ctrl, bool alt, bool right);
	void OnHit();

	std::vector<DoorComponent*> doors;
	std::string name;
	bool Open;
	bool IsEnabled;
	Real Width;
	Real Height;
	Real Thickness;
	ElevatorDoor *parent_elevdoor;
	Door *parent_door;
	bool IsShaftDoor;
	Real Shift;
	Real voffset;
	int floor;
	bool rotate;
};

}

#endif

