/*
	Scalable Building Simulator - Map Object
	The Skyscraper Project - Version 2.0
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
#include "cameratexture.h"
#include "timer.h"
#include "map.h"

namespace SBS {

class Map::Timer : public TimerObject
{
public:
	Map *parent;
	bool shot;
	Timer(const std::string &name, Map *parent) : TimerObject(parent, name)
	{
		this->parent = parent;
		shot = false;
	}
	virtual void Notify();
};

Map::Map(Object *parent, const std::string &name) : Object(parent)
{
	//set up SBS object
	SetValues("Map", name, false);

	enabled = false;

	//create orthographic camera texture, used for map generation
	//OrthoCamera = new CameraTexture(this, "MapCamera", 3, 0, Vector3(0, 50000, 0), false, Vector3(270, 0, 0));
	//OrthoCamera->EnableOrthographic(true);

	//timer = new Timer("Map Timer", this);
	timer = 0;

	//Enabled(true);
}

Map::~Map()
{
	if (OrthoCamera)
		delete OrthoCamera;
	OrthoCamera = 0;
}

void Map::Enabled(bool value)
{
	if (OrthoCamera)
		OrthoCamera->Enabled(value);
	/*if (value == true)
		timer->Start(10000, false);
	else
		timer->Stop();*/
	enabled = value;
}

bool Map::IsEnabled()
{
	return enabled;
}

void Map::Timer::Notify()
{
	/*if (parent->IsEnabled() == true && shot == false)
	{
		parent->Enabled(false);
		shot = true;
	}
	else
	{
		parent->Enabled(true);
		shot = false;
	}*/
}

void Map::GetImage(Ogre::Image &image)
{
	if (OrthoCamera)
		OrthoCamera->GetImage(image);
}

}
