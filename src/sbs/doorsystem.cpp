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

#include "globals.h"
#include "sbs.h"
#include "profiler.h"
#include "mesh.h"
#include "floor.h"
#include "elevator.h"
#include "elevatorcar.h"
#include "elevatordoor.h"
#include "door.h"
#include "doorsystem.h"

namespace SBS {

//
// Door Component
//

DoorComponent::DoorComponent(const std::string &doorname, DoorWrapper *Wrapper, const std::string &Direction, bool OpenClockwise, Real OpenSpeed, Real CloseSpeed, DynamicMesh *dynmesh)
{
	name = doorname;
	wrapper = Wrapper;

	//create object mesh
	mesh = new MeshObject(wrapper, doorname, dynmesh);

	//keep colliders attached, to fix performance issues when moving in and out of an elevator
	mesh->remove_on_disable = false;

	std::string direction_check = Direction;
	SetCase(direction_check, false);
	TrimString(direction_check);

	//set direction value from strings
	direction = 0;
	if (direction_check == "down")
		direction = 1;
	if (direction_check == "left" || direction_check == "forward" || direction_check == "front")
		direction = 2;
	if (direction_check == "right" || direction_check == "back")
		direction = 3;

	open_speed = OpenSpeed;
	close_speed = CloseSpeed;
	active_speed = 0;
	openchange = 0;
	marker1 = 0;
	marker2 = 0;
	door_section = 0;
	stopping_distance = 0;
	temp_change = 0;
	accelerating = false;
	is_open = false;
	finished = false;
	sign_changed = false;
	old_difference = 0;
	recheck_difference = false;
	reversed = false;
	offset = 0;
	rotation = 0;
	Clockwise = OpenClockwise;
}

DoorComponent::~DoorComponent()
{
	if (mesh)
		delete mesh;
	mesh = 0;
}

void DoorComponent::MoveDoors(bool open, bool manual)
{
	//opens or closes elevator doors
	//currently only supports doors on either the left/right or front/back
	//diagonal doors will be done later, by possibly using relative plane movement

	//this door system is based on offsets from the door origin (starting position).
	//when opening, the door starts at the origin, accelerates to marker 1, moves at a constant
	//rate to marker 2, and then decelerates after marker 2.
	//the acceleration/deceleration sections aren't used if manual is true (in that case, it simply sets a speed value, and moves
	//the doors until they reach the ends

	//direction is either 0 for up, 1 for down, 2 for left/forward and 3 for right/backward

	//first get position and origin of door, and adjust values to reflect the "edge" of the door
	SBS_PROFILE("DoorComponent::MoveDoors");
	Real tempposition, temporigin;

	if (wrapper->rotate == true)
		return;

	//get parent values
	bool DoorChanged = false;
	bool DoorDirection = false;
	bool NudgeStatus = false;
	bool PreviousOpen = false;
	if (wrapper->parent_elevdoor)
	{
		DoorChanged = wrapper->parent_elevdoor->GetDoorChanged();
		DoorDirection = wrapper->parent_elevdoor->DoorDirection;
		NudgeStatus = wrapper->parent_elevdoor->GetNudgeStatus();
		PreviousOpen = wrapper->parent_elevdoor->GetPreviousOpen();
	}
	else
	{
		DoorChanged = wrapper->parent_door->GetDoorChanged();
		DoorDirection = wrapper->parent_door->DoorDirection;
		NudgeStatus = false;
		PreviousOpen = wrapper->parent_door->GetPreviousOpen();

		//exit if trying to run standard doors in manual mode
		if (manual == true)
			return;
	}

	if (finished == true && DoorChanged == false)
		return;

	if (direction > 1)
	{
		if (DoorDirection == false)
			tempposition = mesh->GetPosition(true).z + wrapper->Shift;
		else
			tempposition = mesh->GetPosition(true).x + wrapper->Shift;
		temporigin = wrapper->Shift;
	}
	else
	{
		tempposition = mesh->GetPosition(true).y;
		temporigin = 0;
	}

	//get distance from starting point
	Real difference = std::abs(tempposition - temporigin);

	if (old_difference != 0 && manual == true && recheck_difference == true)
	{
		//check if the position went beyond 0
		if ((tempposition - temporigin > 0 && old_difference < 0) || (tempposition - temporigin < 0 && old_difference > 0))
			sign_changed = true;
	}

	if (recheck_difference == false && old_difference != 0)
	{
		recheck_difference = true;
		old_difference = 0;
	}
	else
		old_difference = tempposition - temporigin;

	//debug - show current section as function is running
	//Report("Door section: " + ToString(door_section));

	if ((DoorChanged == false && door_section == 0) || (DoorChanged == true && finished == true))
	{
		//initialization code

		finished = false;
		recheck_difference = false;

		//marker1 is the position to stop accelerating at (accelerates to marker 1)
		//marker2 is the position to start decelerating at (runs full speed until marker 2)
		if (manual == false)
		{
			Real speed;
			if (open == true)
				speed = open_speed;
			else
				speed = close_speed;

			if (wrapper->parent_elevdoor)
			{
				if (NudgeStatus == false || wrapper->parent_elevdoor->SlowSpeed == 0)
					openchange = speed / 50;
				else
					openchange = (speed * wrapper->parent_elevdoor->SlowSpeed) / 50;
			}
			else
				openchange = speed / 50;

			if (direction > 1)
			{
				//get width and offset values (offset is the distance the door component
				//is from the edge of the door frame)
				Real width;
				Real mainwidth = wrapper->Width / 2;
				if (DoorDirection == false)
				{
					width = std::abs(extents_max.z - extents_min.z);
					if (direction == 2)
						offset = mainwidth + (extents_min.z - wrapper->Shift);
					else
						offset = mainwidth - (extents_max.z - wrapper->Shift);
				}
				else
				{
					width = std::abs(extents_max.x - extents_min.x);
					if (direction == 2)
						offset = mainwidth + (extents_min.x - wrapper->Shift);
					else
						offset = mainwidth - (extents_max.x - wrapper->Shift);
				}
				Real newwidth = width + offset;
				marker1 = newwidth / 4;
				marker2 = (mainwidth + (width - mainwidth)) - marker1 + offset;
			}
			else
			{
				Real height = std::abs(extents_max.y - extents_min.y);
				Real mainheight = wrapper->Height / 2;
				if (direction == 0)
					offset = wrapper->Height - (extents_max.y - wrapper->voffset);
				else
					offset = extents_min.y - wrapper->voffset;

				Real newheight = height + offset;
				marker1 = newheight / 4;
				marker2 = (mainheight + (height - mainheight)) - marker1 + offset;
			}
			active_speed = 0;
		}
		else
		{
			//manual movement positioning (same as normal positioning, but the markers are at the
			//door frame extents
			if (direction > 1)
			{
				Real width;
				Real mainwidth = wrapper->Width / 2;
				if (DoorDirection == false)
				{
					width = std::abs(extents_max.z - extents_min.z);
					if (direction == 2)
						offset = mainwidth + (extents_min.z - wrapper->Shift);
					else
						offset = mainwidth - (extents_max.z - wrapper->Shift);
				}
				else
				{
					width = std::abs(extents_max.x - extents_min.x);
					if (direction == 2)
						offset = mainwidth + (extents_min.x - wrapper->Shift);
					else
						offset = mainwidth - (extents_max.x - wrapper->Shift);
				}
				marker1 = 0;
				marker2 = mainwidth + (width - mainwidth) + offset;
			}
			else
			{
				Real height = std::abs(extents_max.y - extents_min.y);
				Real mainheight = wrapper->Height / 2;
				if (direction == 0)
					offset = wrapper->Height - (extents_max.y - wrapper->voffset);
				else
					offset = extents_min.y - wrapper->voffset;

				marker1 = 0;
				marker2 = mainheight + (height - mainheight) + offset;
			}

			if (open == true)
				active_speed = wrapper->parent_elevdoor->ManualSpeed;
			else
				active_speed = -wrapper->parent_elevdoor->ManualSpeed;
		}
	}
	else if (PreviousOpen != open && DoorChanged == true && reversed == false)
	{
		//if a different direction was specified during movement
		//change directions immediately
		if (open == true)
		{
			//relocate marker 1 to the door's current position, in order to stop it
			Real offset = marker1 - difference;
			if (difference >= marker1)
				//place marker at door position
				marker1 = difference;
			else
				//place marker to the right based on the offset, to bring door back to full speed
				marker1 = difference + offset;
		}
		else //closing
		{
			//relocate marker 2 to the door's current position, in order to stop it
			Real offset = difference - marker2;
			if (difference <= marker2)
				//place marker at door position
				marker2 = difference;
			else
				//place marker to the left based on the offset, to bring door back to full speed
				marker2 = difference - offset;
		}
		reversed = true;
	}

	//Speed up doors (only if manual is false)
	if (manual == false)
	{
		//if door is opening and is left of marker 1
		//or if door is closing and is to the right of marker 2
		if ((difference <= marker1 && open == true) || (difference >= marker2 && open == false))
		{
			accelerating = true;
			if (DoorChanged == false)
			{
				//normal door acceleration
				if (open == true)
					active_speed += openchange;
				else
					active_speed -= openchange;
			}
			else
			{
				//reverse movement if transitioning from close to open, or open to close
				//this will trigger if door is closing, and is told to open while left of relocated marker 1
				//or will trigger if door is opening, and is told to close while right of relocated marker 2
				if (difference <= marker1 && open == true)
					active_speed += openchange;
				else if (difference >= marker2 && open == false)
					active_speed -= openchange;
			}

			//move elevator doors
			Move();

			//get stopping distance
			stopping_distance = difference;

			door_section = 1;
			return;
		}
	}

	door_section = 2;

	//Normal door movement
	if ((difference < marker2 && open == true) || (difference > marker1 && open == false))
	{
		if (sign_changed == false) //don't run movement code if difference went beyond 0, for manual only
		{
			Move();
			door_section = 3;
			return;
		}
	}

	accelerating = false;

	//slow down doors (only if manual is false)
	if (manual == false)
	{
		if ((active_speed > 0 && open == true) || (active_speed < 0 && open == false))
		{
			if (open == true)
				active_speed -= openchange;
			else
				active_speed += openchange;

			Move();
			door_section = 4;
			return;
		}
	}

	//report on what section preceded the finishing code (should be 4)
	//Report("Door section: " + ToString(door_section));

	//place doors in positions (fixes any overrun errors)
	Reset(open);
}

void DoorComponent::RotateDoors(bool open)
{
	if (wrapper->rotate == false)
		return;

	if (Clockwise == true)
	{
		if (open == true)
			rotation += open_speed * wrapper->GetRoot()->delta;
		else
			rotation -= close_speed * wrapper->GetRoot()->delta;
	}
	else
	{
		if (open == true)
			rotation -= open_speed * wrapper->GetRoot()->delta;
		else
			rotation += close_speed * wrapper->GetRoot()->delta;
	}

	//if opened fully, set state to opened
	if (rotation >= 90 || rotation <= -90)
	{
		is_open = true;
		finished = true;
		if (rotation >= 90)
			rotation = 90;
		else
			rotation = -90;
	}

	//if closed fully, set state to closed
	if ((Clockwise == true && rotation <= 0) || (Clockwise == false && rotation >= 0))
	{
		is_open = false;
		finished = true;
		rotation = 0;
	}

	mesh->SetRotation(0, rotation, 0);
}

void DoorComponent::Move()
{
	//move elevator doors

	bool DoorDirection = false;
	Real speed = 0;
	if (wrapper->parent_elevdoor)
	{
		DoorDirection = wrapper->parent_elevdoor->DoorDirection;
		speed = active_speed * wrapper->parent_elevdoor->GetRoot()->delta;
	}
	else
	{
		DoorDirection = wrapper->parent_door->DoorDirection;
		speed = active_speed * wrapper->parent_door->GetRoot()->delta;
	}

	//up movement
	if (direction == 0)
		mesh->Move(Vector3(0, 1, 0), speed);

	//down movement
	if (direction == 1)
		mesh->Move(Vector3(0, -1, 0), speed);

	if (DoorDirection == false)
	{
		//left movement
		if (direction == 2)
			mesh->Move(Vector3(0, 0, -1), speed);

		//right movement
		if (direction == 3)
			mesh->Move(Vector3(0, 0, 1), speed);
	}
	else
	{
		//left movement
		if (direction == 2)
			mesh->Move(Vector3(-1, 0, 0), speed);

		//right movement
		if (direction == 3)
			mesh->Move(Vector3(1, 0, 0), speed);
	}
}

void DoorComponent::Reset(bool open)
{
	//reset door state

	if (wrapper->rotate == true)
		return;

	//get parent values
	bool DoorDirection = false;
	if (wrapper->parent_elevdoor)
	{
		DoorDirection = wrapper->parent_elevdoor->DoorDirection;
		mesh->SetPosition(wrapper->parent_elevdoor->elev->GetPosition().x, mesh->GetParent()->GetPosition().y, wrapper->parent_elevdoor->elev->GetPosition().z);
	}
	else
	{
		DoorDirection = wrapper->parent_door->DoorDirection;
		mesh->SetPosition(wrapper->parent_door->GetPosition());
	}

	if (open == true)
	{
		//move elevator doors
		if (direction > 1)
		{
			Real mainwidth = wrapper->Width / 2;
			if (DoorDirection == false)
			{
				Real width = std::abs(extents_max.z - extents_min.z);
				if (direction == 2)
					mesh->Move(Vector3(0, 0, -(mainwidth + (width - mainwidth) + offset)));
				else
					mesh->Move(Vector3(0, 0, mainwidth + (width - mainwidth) + offset));
			}
			else
			{
				Real width = std::abs(extents_max.x - extents_min.x);
				if (direction == 2)
					mesh->Move(Vector3(-(mainwidth + (width - mainwidth) + offset), 0, 0));
				else
					mesh->Move(Vector3(mainwidth + (width - mainwidth) + offset, 0, 0));
			}
		}
		else
		{
			Real mainheight = wrapper->Height / 2;
			Real height = std::abs(extents_max.y - extents_min.y);
			if (direction == 0)
				mesh->Move(Vector3(0, mainheight + (height - mainheight) + offset, 0));
			else
				mesh->Move(Vector3(0, -(mainheight + (height - mainheight) + offset), 0));
		}

		//the door is open now
		is_open = true;
		finished = true;

		//reset values
		active_speed = 0;
		door_section = 0;
		sign_changed = false;
		old_difference = 0;
		stopping_distance = 0;
		reversed = false;
	}
	else
	{
		//the door is closed now
		is_open = false;
		finished = true;

		//reset values
		openchange = 0;
		marker1 = 0;
		marker2 = 0;
		temp_change = 0;
		accelerating = false;
		recheck_difference = false;
		active_speed = 0;
		door_section = 0;
		sign_changed = false;
		old_difference = 0;
		stopping_distance = 0;
		reversed = false;
	}
}

//
// Door Wrapper
//

DoorWrapper::DoorWrapper(Object *parent_obj, ElevatorDoor *door_object, bool shaftdoor, int shaftdoor_floor) : Object(parent_obj)
{
	parent_elevdoor = door_object;
	parent_door = 0;
	Open = false;
	IsEnabled = true;
	Width = 0;
	Height = 0;
	Thickness = 0;
	IsShaftDoor = shaftdoor;
	Shift = 0;
	voffset = 0;
	floor = shaftdoor_floor;
	rotate = false;

	std::string name;
	if (IsShaftDoor == true)
		name = "Shaft Door " + ToString(parent_elevdoor->elev->Number) + ":" + ToString(parent_elevdoor->car->Number) + ":" + ToString(parent_elevdoor->Number) + ":" + ToString(shaftdoor_floor);
	else
		name = "Elevator Door " + ToString(parent_elevdoor->car->Number) + ":" + ToString(parent_elevdoor->Number);

	SetValues("DoorWrapper", name, false);

	if (IsShaftDoor == true)
		SetPosition(parent_elevdoor->elev->GetPosition().x, GetPosition().y + sbs->GetFloor(floor)->GetBase(true), parent_elevdoor->elev->GetPosition().z);
}

DoorWrapper::DoorWrapper(Door *parent, bool rotate) : Object(parent)
{
	parent_elevdoor = 0;
	parent_door = parent;
	Open = false;
	IsEnabled = true;
	Width = 0;
	Height = 0;
	Thickness = 0;
	IsShaftDoor = false;
	Shift = 0;
	voffset = 0;
	floor = 0;
	this->rotate = rotate;

	std::string name = parent->GetName();
	SetValues("DoorWrapper", name, false);
}

DoorWrapper::~DoorWrapper()
{
	for (size_t i = 0; i < doors.size(); i++)
	{
		if (doors[i])
			delete doors[i];
	}
	doors.clear();

	//unregister from parent
	if (parent_deleting == false)
	{
		if (parent_elevdoor)
			parent_elevdoor->RemoveShaftDoor(this);
	}
}

DoorComponent* DoorWrapper::CreateDoor(const std::string &doorname, const std::string &direction, bool OpenClockwise, Real OpenSpeed, Real CloseSpeed, DynamicMesh *dynmesh)
{
	//initialize a door component

	DoorComponent *door = new DoorComponent(doorname, this, direction, OpenClockwise, OpenSpeed, CloseSpeed, dynmesh);
	doors.push_back(door);
	return door;
}

void DoorWrapper::Enabled(bool value)
{
	//enable/disable door meshes

	if (value == IsEnabled)
		return;

	for (size_t i = 0; i < doors.size(); i++)
		doors[i]->mesh->Enabled(value);

	IsEnabled = value;
}

void DoorWrapper::MoveDoors(bool open, bool manual)
{
	//calls per-door move function
	for (size_t i = 0; i < doors.size(); i++)
	{
		if (rotate == false)
			doors[i]->MoveDoors(open, manual);
		else
			doors[i]->RotateDoors(open);
	}
}

bool DoorWrapper::CheckDoorsOpen()
{
	//checks to see if doors are open or closed, and returns true if the status changed

	for (size_t i = 0; i < doors.size(); i++)
	{
		//exit if the status is the same on any door
		if (doors[i]->is_open == Open)
			return false;
	}

	//if the status changed, set the new status and return true
	Open = !Open;
	return true;
}

bool DoorWrapper::IsFinished()
{
	//checks to see if all of the doors are finished

	for (size_t i = 0; i < doors.size(); i++)
	{
		//exit if any door is not finished
		if (doors[i]->finished == false)
			return false;
	}
	return true;
}

void DoorWrapper::ResetFinished()
{
	//reset finished state on all doors

	for (size_t i = 0; i < doors.size(); i++)
	{
		doors[i]->finished = false;
	}
}

void DoorWrapper::StopDoors()
{
	//stop all doors

	for (size_t i = 0; i < doors.size(); i++)
	{
		doors[i]->active_speed = 0;
		doors[i]->door_section = 0;
	}
}

void DoorWrapper::ResetState()
{
	//reset door state in case of an internal malfunction

	Open = false;

	for (size_t i = 0; i < doors.size(); i++)
		doors[i]->Reset(false);
}

void DoorWrapper::OnClick(Vector3 &position, bool shift, bool ctrl, bool alt, bool right)
{
	if (parent_door)
	{
		parent_door->OnClick(position, shift, ctrl, alt, right);
		return;
	}

	if (shift == true && right == false)
	{
		if (parent_elevdoor)
		{
			ElevatorCar *car = parent_elevdoor->car;
			if (!car)
				return;

			int number = parent_elevdoor->Number;

			if (IsShaftDoor == true)
			{
				//check shaft doors
				if (abs(car->AreDoorsMoving(number, false, true)) == 2)
					car->StopDoors(number);
				else
				{
					if (car->AreShaftDoorsOpen(number, floor) == false)
						car->OpenDoorsEmergency(number, 3, floor);
					else
						car->CloseDoorsEmergency(number, 3, floor);
				}
			}
			else
			{
				//check elevator doors
				if (abs(car->AreDoorsMoving(number, true, false)) == 2)
					car->StopDoors(number);
				else
				{
					if (car->AreDoorsOpen(number) == false)
						car->OpenDoorsEmergency(number, 2);
					else
						car->CloseDoorsEmergency(number, 2);
				}
			}
		}
	}
}

void DoorWrapper::OnHit()
{
	//check elevator doors (door bumpers feature)

	if (parent_elevdoor)
	{
		//make sure both internal and external doors are closing
		if (parent_elevdoor->OpenDoor == -1 && parent_elevdoor->GetWhichDoors() == 1)
		{
			//either open doors if the hit door was an internal door or a shaft door on the elevator floor
			if (IsShaftDoor == false || (IsShaftDoor == true && floor == parent_elevdoor->car->GetFloor()))
				parent_elevdoor->car->OpenDoors(parent_elevdoor->Number, 1);
		}
	}
}

}
