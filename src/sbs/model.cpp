/* $Id$ */

/*
	Scalable Building Simulator - Model Class
	The Skyscraper Project - Version 1.8 Alpha
	Copyright (C)2004-2011 Ryan Thoryk
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

Model::Model(const char *name, const char *filename, Ogre::Vector3 position, Ogre::Vector3 rotation, float max_render_distance, float scale_multiplier)
{
	//loads a 3D model into the simulation

	//set up SBS object
	object = new Object();
	object->SetValues(this, sbs->object, "Model", name, false);
	Origin = position;

	load_error = false;
	mesh = new MeshObject(object, name, true, filename, max_render_distance, scale_multiplier);
	if (!mesh->MeshWrapper.get() || !mesh->SceneNode)
	{
		load_error = true;
		return;
	}
	sbs->AddModelHandle(this);

	Move(position, false, false, false);
	SetRotation(rotation);
}

Model::~Model()
{
	if (sbs->FastDelete == false)
		sbs->DeleteModelHandle(this);
	if (mesh)
		delete mesh;
	mesh = 0;
	delete object;
}

void Model::Move(const Ogre::Vector3 position, bool relative_x, bool relative_y, bool relative_z)
{
	mesh->Move(position, relative_x, relative_y, relative_z, Origin);
}

Ogre::Vector3 Model::GetPosition()
{
	return mesh->GetPosition();
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
