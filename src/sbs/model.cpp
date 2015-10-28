/* $Id$ */

/*
	Scalable Building Simulator - Model Object
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
#include "unix.h"
#include "model.h"

namespace SBS {

Model::Model(Object *parent, const char *name, const char *filename, bool center, Ogre::Vector3 position, Ogre::Vector3 rotation, float max_render_distance, float scale_multiplier, bool enable_physics, float restitution, float friction, float mass)
{
	//loads a 3D model into the simulation

	//set up SBS object
	SetValues(this, parent, "Model", name, false);
	Offset = 0;
	is_key = false;
	KeyID = 0;

	load_error = false;
	mesh = new MeshObject(this, name, filename, max_render_distance, scale_multiplier, enable_physics, restitution, friction, mass);
	if (!mesh->Movable)
	{
		load_error = true;
		return;
	}

	if (center == true)
	{
		Ogre::AxisAlignedBox box = mesh->MeshWrapper.get()->getBounds();
		Ogre::Vector3 vec = box.getCenter();
		Offset = Ogre::Vector3(vec.x, -box.getMinimum().y, -vec.z);
	}

	Move(position);
	//SetRotation(rotation);

	//move mesh object to specified offset
	mesh->Move(Offset);
	mesh->SetRotation(rotation);
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
	{
		if (std::string(GetParent()->GetType()) == "Elevator")
			((Elevator*)GetParent()->GetRawObject())->RemoveModel(this);
		if (std::string(GetParent()->GetType()) == "Floor")
			((Floor*)GetParent()->GetRawObject())->RemoveModel(this);
		if (std::string(GetParent()->GetType()) == "Shaft")
			((Shaft*)GetParent()->GetRawObject())->RemoveModel(this);
		if (std::string(GetParent()->GetType()) == "Stairs")
			((Stairs*)GetParent()->GetRawObject())->RemoveModel(this);
		if (std::string(GetParent()->GetType()) == "SBS")
			sbs->RemoveModel(this);
	}
}

void Model::Enable(bool value)
{
	mesh->Enable(value);
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

bool Model::PhysicsEnabled()
{
	return mesh->Movable;
}

}
