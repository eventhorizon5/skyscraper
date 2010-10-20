/* $Id$ */

/*
	Scalable Building Simulator - Model Class
	The Skyscraper Project - Version 1.8 Alpha
	Copyright (C)2004-2010 Ryan Thoryk
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

Model::Model(const char *name, const char *filename, csVector3 &position, csVector3 &rotation, float max_render_distance, float scale_multiplier)
{
	//loads a 3D model into the simulation

	//set up SBS object
	object = new Object();
	object->SetValues(this, sbs->object, "Model", name, false);

	load_error = false;
	mesh = new MeshObject(object, name, filename, max_render_distance, scale_multiplier);
	if (!mesh->MeshWrapper)
		load_error = true;
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

void Model::Move(const csVector3& position, bool relative_x, bool relative_y, bool relative_z)
{
	//move light - this can only be done on movable lights
	csVector3 pos;
	if (relative_x == false)
		pos.x = sbs->ToRemote(Origin.x + position.x);
	else
		pos.x = mesh->Movable->GetPosition().x + sbs->ToRemote(position.x);
	if (relative_y == false)
		pos.y = sbs->ToRemote(Origin.y + position.y);
	else
		pos.y = mesh->Movable->GetPosition().y + sbs->ToRemote(position.y);
	if (relative_z == false)
		pos.z = sbs->ToRemote(Origin.z + position.z);
	else
		pos.z = mesh->Movable->GetPosition().z + sbs->ToRemote(position.z);
	mesh->Movable->SetPosition(pos);
	mesh->Movable->UpdateMove();
}

csVector3 Model::GetPosition()
{
	return sbs->ToLocal(mesh->Movable->GetPosition());
}

void Model::SetRotation(const csVector3& rotation)
{
	//rotate light
	csMatrix3 rot = csXRotMatrix3(rotation.x) * csYRotMatrix3(rotation.y) * csZRotMatrix3(rotation.z);
	csOrthoTransform ot (rot, mesh->Movable->GetTransform().GetOrigin());
	mesh->Movable->SetTransform(ot);
	rotX = rotation.x;
	rotY = rotation.y;
	rotZ = rotation.z;
	mesh->Movable->UpdateMove();
}

void Model::Rotate(const csVector3& rotation, float speed)
{
	//rotates light in a relative amount
	rotX += rotation.x * speed;
	rotY += rotation.y * speed;
	rotZ += rotation.z * speed;
	SetRotation(csVector3(rotX, rotY, rotZ));
}

csVector3 Model::GetRotation()
{
	return csVector3(rotX, rotY, rotZ);
}

void Model::Enable(bool value)
{
	mesh->Enable(value);	
}

bool Model::IsEnabled()
{
	return mesh->IsEnabled();
}
