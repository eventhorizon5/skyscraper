/*
	Scalable Building Simulator - Primitive Object
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

#include "globals.h"
#include "sbs.h"
#include "mesh.h"
#include "floor.h"
#include "elevator.h"
#include "elevatorcar.h"
#include "shaft.h"
#include "stairs.h"
#include "camera.h"
#include "profiler.h"
#include "primitive.h"

namespace SBS {

Primitive::Primitive(Object *parent, const std::string &name) : Object(parent)
{
	//loads a 3D primitive into the simulation

	//set up SBS object
	SetValues("Primitive", name, false);
	Offset = Vector3::ZERO;
	is_key = false;
	KeyID = 0;
	global = IsGlobal();
	always_visible = false;
	mesh = 0;
	collider_type = 0;
}

Primitive::~Primitive()
{
	if (mesh)
	{
		mesh->parent_deleting = true;
		delete mesh;
	}
	mesh = 0;

	//unregister from parent
	if (sbs->FastDelete == false && parent_deleting == false)
		RemoveFromParent();
}

bool Primitive::Enabled(bool value)
{
	if (always_visible == true)
		value = true;

	bool status = mesh->Enabled(value);
	EnableLoop(value);
	return status;
}

bool Primitive::IsEnabled()
{
	return mesh->IsEnabled();
}

bool Primitive::IsPhysical()
{
	return mesh->IsPhysical();
}

void Primitive::RemoveFromParent()
{
	std::string type = GetParent()->GetType();

	if (type == "ElevatorCar")
		static_cast<ElevatorCar*>(GetParent())->RemovePrimitive(this);
	else if (type == "Floor")
		static_cast<Floor*>(GetParent())->RemovePrimitive(this);
	else if (type == "Shaft Level")
		static_cast<Shaft::Level*>(GetParent())->RemovePrimitive(this);
	else if (type == "Stairwell Level")
		static_cast<Stairwell::Level*>(GetParent())->RemovePrimitive(this);
	else if (type == "SBS")
		sbs->RemovePrimitive(this);
}

void Primitive::AddToParent()
{
	std::string type = GetParent()->GetType();

	if (type == "ElevatorCar")
		static_cast<ElevatorCar*>(GetParent())->AddPrimitive(this);
	else if (type == "Floor")
		static_cast<Floor*>(GetParent())->AddPrimitive(this);
	else if (type == "Shaft Level")
		static_cast<Shaft::Level*>(GetParent())->AddPrimitive(this);
	else if (type == "Stairwell Level")
		static_cast<Stairwell::Level*>(GetParent())->AddPrimitive(this);
	else if (type == "SBS")
		sbs->AddPrimitive(this);
}

bool Primitive::Loop()
{
	//runloop, called by parent to allow for switching parents

	SBS_PROFILE("Primitive::Loop");

	Floor *floor = GetParent()->ConvertTo<Floor>();
	Elevator *elevator = GetParent()->ConvertTo<Elevator>();
	::SBS::SBS *root = GetParent()->ConvertTo<SBS>();

	//if primitive is a child of a floor or a global object, and is in an elevator, switch parent to elevator
	if (floor || root)
	{
		for (int i = 1; i <= sbs->GetElevatorCount(); i++)
		{
			Elevator *elev = sbs->GetElevator(i);

			if (elev)
			{
				ElevatorCar *car = elev->IsInElevator(GetPosition());
				if (car)
				{
					RemoveFromParent();
					ChangeParent(car);
					AddToParent();
					break;
				}
			}
		}
	}

	//if primitive is a child of an elevator, and is moved outside to a floor, switch parent to floor (or make it global)
	else if (elevator)
	{
		if (elevator->IsInElevator(GetPosition()) == 0)
		{
			if (global == false)
			{
				//switch parent back to floor object
				int floornum = sbs->GetFloorNumber(GetPosition().y);
				Floor *floor = sbs->GetFloor(floornum);

				if (floor)
				{
					RemoveFromParent();
					ChangeParent(floor);
					AddToParent();
				}
			}
			else
			{
				//switch parent back to engine root
				RemoveFromParent();
				ChangeParent(sbs);
				AddToParent();
			}
		}
	}

	return true;
}

void Primitive::PickUp()
{
	//pick up primitive (make prim a child of the camera object)

	if (IsPickedUp() == true)
		return;

	//pick-up prim (make primitive a child of the camera)
	RemoveFromParent();
	ChangeParent(sbs->camera);
}

void Primitive::Drop()
{
	//drop prim (make primitive a child of the proper non-camera object)

	if (IsPickedUp() == false)
		return;

	if (global == true)
		ChangeParent(sbs);
	else
	{
		bool found = false;
		for (int i = 1; i < sbs->GetElevatorCount(); i++)
		{
			Elevator *elev = sbs->GetElevator(i);

			if (elev)
			{
				ElevatorCar *car = elev->IsInElevator(GetPosition());
				if (car)
				{
					ChangeParent(car);
					found = true;
					break;
				}
			}
		}

		if (found == false)
		{
			int floornum = sbs->GetFloorNumber(GetPosition().y);
			Floor *floor = sbs->GetFloor(floornum);

			if (floor)
				ChangeParent(floor);
		}
	}

	AddToParent();
}

bool Primitive::IsPickedUp()
{
	return (GetParent() == sbs->camera);
}

void Primitive::OnClick(Vector3 &position, bool shift, bool ctrl, bool alt, bool right)
{
	if (right == false)
	{
		//do something here
	}
}

bool Primitive::Attach(const std::string &meshname, const Vector3 &position, const Vector3 &rotation, Real max_render_distance, Real scale_multiplier, bool enable_physics, Real restitution, Real friction, Real mass)
{
	if (meshname == "")
		return false;

	bool use_collider = true;
	if (collider_type == -1)
		use_collider = false;

	mesh = new MeshObject(this, GetName(), 0, "", meshname, max_render_distance, scale_multiplier, use_collider);
	mesh->EnablePhysics(enable_physics, restitution, friction, mass); //this also creates a collider

	Enabled(true);

	//move to position and specified offset
	Move(position);
	SetRotation(rotation);

	Init(false);

	return true;
}

void Primitive::SetTexture(const std::string &texture)
{
	//set texture to be used by this prim

	mesh->SetMaterial(texture);
}

}
