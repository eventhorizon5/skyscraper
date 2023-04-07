/*
	Scalable Building Simulator - Identifier Object
	The Skyscraper Project - Version 1.12 Alpha
	Copyright (C)2004-2023 Ryan Thoryk
	https://www.skyscrapersim.net
	https://sourceforge.net/projects/skyscraper/
	Contact - ryan@thoryk.com

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
#include "manager.h"
#include "texture.h"
#include "display.h"

namespace SBS {

DisplayPanel::DisplayPanel(Object *parent, const std::string &texture_prefix, const std::string &blank_texture, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset) : Object(parent)
{
	//creates a new display panel at the specified position

	//set up SBS object
	SetValues("DisplayPanel", "Display Panel", false);

	is_enabled = true;
	Prefix = texture_prefix;
	Blank = blank_texture;

	//move object
	Move(CenterX, voffset, CenterZ);

	//std::string name = "Display Panel " + ToString(elevator);
	//SetName(name);

	Mesh = new MeshObject(this, "Display Panel", 0, "", sbs->GetConfigFloat("Skyscraper.SBS.MaxSmallRenderDistance", 100));

	std::string tmpdirection = direction;
	SetCase(tmpdirection, false);

	if (tmpdirection == "front" || tmpdirection == "back")
	{
		if (tmpdirection == "front")
			sbs->DrawWalls(true, false, false, false, false, false);
		else
			sbs->DrawWalls(false, true, false, false, false, false);

		sbs->AddWallMain(this, Mesh, "Display Panel", Blank, 0, -width / 2, 0, width / 2, 0, height, height, 0, 0, 1, 1, false);
	}
	if (tmpdirection == "left" || tmpdirection == "right")
	{
		if (tmpdirection == "left")
			sbs->DrawWalls(true, false, false, false, false, false);
		else
			sbs->DrawWalls(false, true, false, false, false, false);

		sbs->AddWallMain(this, Mesh, "Display Panel", Blank, 0, 0, width / 2, 0, -width / 2, height, height, 0, 0, 1, 1, false);
	}
	sbs->ResetWalls();
}

DisplayPanel::~DisplayPanel()
{
	if (Mesh)
	{
		Mesh->parent_deleting = true;
		delete Mesh;
	}
	Mesh = 0;

	//unregister from parent
	/*if (sbs->FastDelete == false && parent_deleting == false)
	{
		std::string type = GetParent()->GetType();

		if (type == "ElevatorCar")
			static_cast<ElevatorCar*>(GetParent())->RemoveFloorIndicator(this);
		else if (type == "Floor")
			static_cast<Floor*>(GetParent())->RemoveFloorIndicator(this);
	}*/
}

void DisplayPanel::Enabled(bool value)
{
	//turns display on/off

	if (is_enabled == value)
		return;

	Mesh->Enabled(value);
	is_enabled = value;
}

void DisplayPanel::Update(std::string &texture)
{
	//update display with given texture

	if (texture == "" && Blank != "")
	{
		Mesh->ChangeTexture(Blank);
		return;
	}

	//don't update texture if no value
	if (texture == "")
		return;

	texture.insert(0, Prefix);

	Mesh->ChangeTexture(texture);
	sbs->GetTextureManager()->EnableLighting(texture, false);
}

void DisplayPanel::Off()
{
	if (Blank != "")
		Mesh->ChangeTexture(Blank);
}

}
