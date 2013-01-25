/* $Id$ */

/*
	Scalable Building Simulator - Key Class
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
#include "key.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

Key::Key(Object *parent, const char *name, const char *texture, int number, float CenterX, float CenterZ, float width, float height, float altitude, float tw, float th)
{
	//set up SBS object
	object = new Object();
	object->SetValues(this, parent, "Key", name, false);

	IsEnabled = true;
	Name = name;
	Number = number;
	float x1 = 0, z1 = 0, x2 = 0, z2 = 0;

	//Create mesh
	KeyMesh = new MeshObject(object, Name.c_str());
	KeyMesh->Move(origin, false, false, false);

	//create door
	sbs->DrawWalls(true, true, true, true, true, true);
	sbs->ResetTextureMapping(true);
	WallObject *wall;
	wall = KeyMesh->CreateWallObject(this->object, name);
	sbs->AddWallMain(wall, name, texture, 0, x1, z1, x2, z2, height, height, 0, 0, tw, th, false);
	sbs->ResetWalls();
	sbs->ResetTextureMapping();
}

Key::~Key()
{
	//destructor

	if (KeyMesh)
		delete KeyMesh;
	KeyMesh = 0;

	//unregister from parent
	if (sbs->FastDelete == false)
	{
		if (object->parent_deleting == false)
		{
			//if (std::string(object->GetParent()->GetType()) == "Floor")
				//((Floor*)object->GetParent()->GetRawObject())->RemoveKey(this);
		}
	}

	delete object;
}

void Key::Enabled(bool value)
{
	if (IsEnabled == value)
		return;

	KeyMesh->Enable(value);
	IsEnabled = value;
}

void Key::Move(const Ogre::Vector3 position, bool relative_x, bool relative_y, bool relative_z)
{
	//moves key

	KeyMesh->Move(position, relative_x, relative_y, relative_z);
	origin = GetPosition();

}

Ogre::Vector3 Key::GetPosition()
{
	//return the key position

	return KeyMesh->GetPosition();
}
