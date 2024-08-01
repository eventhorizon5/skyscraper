/*
	Scalable Building Simulator - Model Object
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
#include "mesh.h"
#include "floor.h"
#include "elevator.h"
#include "elevatorcar.h"
#include "shaft.h"
#include "stairs.h"
#include "camera.h"
#include "profiler.h"
#include "model.h"

namespace SBS {

Model::Model(Object *parent, const std::string &name, const std::string &filename, bool center, const Vector3 &position, const Vector3 &rotation, Real max_render_distance, Real scale_multiplier, bool enable_physics, Real restitution, Real friction, Real mass) : Object(parent)
{
	//loads a 3D model into the simulation

	//set up SBS object
	SetValues("Model", name, false);
	Offset = Vector3::ZERO;
	is_key = false;
	KeyID = 0;
	global = IsGlobal();
	this->center = center;

	if (filename == "")
		custom = true;
	else
		custom = false;

	load_error = false;
	mesh = new MeshObject(this, name, 0, filename, max_render_distance, scale_multiplier, enable_physics, restitution, friction, mass);
	if (mesh->model_loaded == false && filename != "")
	{
		load_error = true;
		return;
	}

	Enabled(true);

	//move to position and specified offset
	Move(position);
	SetRotation(rotation);

	if (filename != "")
		Init(false);
}

Model::~Model()
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

void Model::Enabled(bool value)
{
	mesh->Enabled(value);
	EnableLoop(value);
}

bool Model::IsEnabled()
{
	return mesh->IsEnabled();
}

bool Model::IsKey()
{
	return is_key;
}

int Model::GetKeyID()
{
	return KeyID;
}

void Model::SetKey(int keyid)
{
	is_key = true;
	KeyID = keyid;
}

bool Model::IsPhysical()
{
	return mesh->IsPhysical();
}

void Model::RemoveFromParent()
{
	std::string type = GetParent()->GetType();

	if (type == "ElevatorCar")
		static_cast<ElevatorCar*>(GetParent())->RemoveModel(this);
	else if (type == "Floor")
		static_cast<Floor*>(GetParent())->RemoveModel(this);
	else if (type == "Shaft Level")
		static_cast<Shaft::Level*>(GetParent())->RemoveModel(this);
	else if (type == "Stairwell Level")
		static_cast<Stairwell::Level*>(GetParent())->RemoveModel(this);
	else if (type == "SBS")
		sbs->RemoveModel(this);
}

void Model::AddToParent()
{
	std::string type = GetParent()->GetType();

	if (type == "ElevatorCar")
		static_cast<ElevatorCar*>(GetParent())->AddModel(this);
	else if (type == "Floor")
		static_cast<Floor*>(GetParent())->AddModel(this);
	else if (type == "Shaft Level")
		static_cast<Shaft::Level*>(GetParent())->AddModel(this);
	else if (type == "Stairwell Level")
		static_cast<Stairwell::Level*>(GetParent())->AddModel(this);
	else if (type == "SBS")
		sbs->AddModel(this);
}

void Model::Loop()
{
	//runloop, called by parent to allow for switching parents

	SBS_PROFILE("Model::Loop");

	Floor *floor = GetParent()->ConvertTo<Floor>();
	Elevator *elevator = GetParent()->ConvertTo<Elevator>();
	::SBS::SBS *root = GetParent()->ConvertTo<SBS>();

	//if model is a child of a floor or a global object, and is in an elevator, switch parent to elevator
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

	//if model is a child of an elevator, and is moved outside to a floor, switch parent to floor (or make it global)
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
}

void Model::PickUp()
{
	//pick up model (make model a child of the camera object)

	if (IsPickedUp() == true)
		return;

	//pick-up model (make model a child of the camera)
	RemoveFromParent();
	ChangeParent(sbs->camera);
}

void Model::Drop()
{
	//drop model (make model a child of the proper non-camera object)

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

bool Model::IsPickedUp()
{
	return (GetParent() == sbs->camera);
}

void Model::OnInit()
{
	if (center == true)
	{
		Offset = mesh->GetOffset();

		//move mesh object to specified offset
		mesh->Move(Offset);
	}
}

void Model::OnClick(Vector3 &position, bool shift, bool ctrl, bool alt, bool right)
{
	if (right == false)
	{
		//if model is a key, add key to keyring and delete model
		if (IsKey() == true)
		{
			sbs->AddKey(GetKeyID(), GetName());
			SelfDestruct(); //self-destruct this model
			return;
		}
	}
}

}
