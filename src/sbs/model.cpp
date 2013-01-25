/* $Id$ */

/*
	Scalable Building Simulator - Model Class
	The Skyscraper Project - Version 1.8 Alpha
	Copyright (C)2004-2013 Ryan Thoryk
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

extern SBS *sbs; //external pointer to the SBS engine

Model::Model(Object *parent, const char *name, const char *filename, bool center, Ogre::Vector3 position, Ogre::Vector3 rotation, float max_render_distance, float scale_multiplier, bool enable_physics, float restitution, float friction, float mass)
{
	//loads a 3D model into the simulation

	//set up SBS object
	object = new Object();
	object->SetValues(this, parent, "Model", name, false);
	Origin = position;
	Offset = 0;
	is_key = false;
	KeyID = 0;
	Name = name;

	load_error = false;
	mesh = new MeshObject(object, name, filename, max_render_distance, scale_multiplier, enable_physics, restitution, friction, mass);
	if (!mesh->MeshWrapper.get() || !mesh->SceneNode)
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
	Move(position, false, false, false);
	SetRotation(rotation);
}

Model::~Model()
{
	if (mesh)
		delete mesh;
	mesh = 0;

	//unregister from parent
	if (sbs->FastDelete == false && object->parent_deleting == false)
	{
		if (std::string(object->GetParent()->GetType()) == "Elevator")
			((Elevator*)object->GetParent()->GetRawObject())->RemoveModel(this);
		if (std::string(object->GetParent()->GetType()) == "Floor")
			((Floor*)object->GetParent()->GetRawObject())->RemoveModel(this);
		if (std::string(object->GetParent()->GetType()) == "Shaft")
			((Shaft*)object->GetParent()->GetRawObject())->RemoveModel(this);
		if (std::string(object->GetParent()->GetType()) == "Stairs")
			((Stairs*)object->GetParent()->GetRawObject())->RemoveModel(this);
		if (std::string(object->GetParent()->GetType()) == "SBS")
			sbs->RemoveModel(this);
	}

	delete object;
}

void Model::Move(const Ogre::Vector3 position, bool relative_x, bool relative_y, bool relative_z)
{
	//mesh->Move(position, relative_x, relative_y, relative_z, Origin);
	mesh->Move(position, relative_x, relative_y, relative_z, Offset);
}

Ogre::Vector3 Model::GetPosition()
{
	return mesh->GetPosition() - Offset;
}

void Model::SetRotation(const Ogre::Vector3 rotation)
{
	mesh->SetRotation(rotation);
}

void Model::Rotate(const Ogre::Vector3 rotation, float speed)
{
	mesh->Rotate(rotation, speed);
}

Ogre::Vector3 Model::GetRotation()
{
	return mesh->GetRotation();
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
