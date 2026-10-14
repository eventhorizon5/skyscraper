/*
	Scalable Building Simulator - Card Reader Object
	The Skyscraper Project - Version 2.1
	Copyright (C)2004-2026 Ryan Thoryk
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
#include "polymesh.h"
#include "sound.h"
#include "mesh.h"
#include "access.h"
#include "cardreader.h"

namespace SBS {

CardReader::CardReader(Object *parent, AccessController *controller, const std::string &access_point, const std::string &grant_sound, const std::string &deny_sound, const std::string &idle_texture, const std::string &grant_texture, const std::string &deny_texture, const std::string &direction, Real width, Real height) : Object(parent)
{
	//create a card reader facing the given direction, centered on its position

	SetValues("CardReader", "Card Reader " + access_point, false);

	this->controller = controller;
	this->access_point = access_point;
	this->grant_sound = grant_sound;
	this->deny_sound = deny_sound;
	this->idle_texture = idle_texture;
	this->grant_texture = grant_texture;
	this->deny_texture = deny_texture;
	flash_until = 0;
	next_check = 0;
	current_texture = idle_texture;
	is_enabled = true;
	sound = 0;

	std::string Direction = direction;
	SetCase(Direction, false);
	TrimString(Direction);

	std::string name = "Card Reader " + access_point;

	ReaderMesh = new MeshObject(this, name, 0, "", "", sbs->GetConfigFloat("Skyscraper.SBS.MaxSmallRenderDistance", 100));

	//build a single flat panel, using the same layout as Control objects
	PolyMesh* polymesh = sbs->GetPolyMesh();

	sbs->TexelOverride = true;
	Wall *wall = 0;
	Real a = -width / 2, b = width / 2;

	if (Direction == "front")
	{
		polymesh->DrawWalls(true, false, false, false, false, false);
		wall = ReaderMesh->CreateWallObject(name);
		polymesh->AddWallMain(wall, name, idle_texture, 0, a, 0, b, 0, height, height, 0, 0, 1, 1, false);
	}
	else if (Direction == "back")
	{
		polymesh->DrawWalls(false, true, false, false, false, false);
		wall = ReaderMesh->CreateWallObject(name);
		polymesh->AddWallMain(wall, name, idle_texture, 0, b, 0, a, 0, height, height, 0, 0, 1, 1, false);
	}
	else if (Direction == "left")
	{
		polymesh->DrawWalls(true, false, false, false, false, false);
		wall = ReaderMesh->CreateWallObject(name);
		polymesh->AddWallMain(wall, name, idle_texture, 0, 0, b, 0, a, height, height, 0, 0, 1, 1, false);
	}
	else if (Direction == "right")
	{
		polymesh->DrawWalls(false, true, false, false, false, false);
		wall = ReaderMesh->CreateWallObject(name);
		polymesh->AddWallMain(wall, name, idle_texture, 0, 0, a, 0, b, height, height, 0, 0, 1, 1, false);
	}
	else
		ReportError("Invalid direction " + direction);

	polymesh->ResetWalls();
	sbs->TexelOverride = false;

	//create sound object
	if (grant_sound != "" || deny_sound != "")
		sound = new Sound(this, name, true);

	if (controller)
		controller->RegisterReader(this);
}

CardReader::~CardReader()
{
	if (sound)
	{
		sound->parent_deleting = true;
		delete sound;
	}
	sound = 0;

	if (ReaderMesh)
	{
		ReaderMesh->parent_deleting = true;
		delete ReaderMesh;
	}
	ReaderMesh = 0;

	if (controller && sbs->FastDelete == false)
		controller->UnregisterReader(this);
}

void CardReader::SetTexture(const std::string &texture)
{
	//change the reader's light, only when it actually changes
	if (texture == current_texture || texture == "")
		return;
	current_texture = texture;
	if (ReaderMesh)
		ReaderMesh->ChangeTexture(texture);
}

void CardReader::ShowResult(bool granted)
{
	//play the result sound; an invalid badge also flashes the light red/green.
	//A valid badge needs no special display, since the light turns green as the lock releases.

	std::string file = granted ? grant_sound : deny_sound;
	if (sound && file != "")
	{
		sound->Load(file);
		sound->Play();
	}

	if (granted == false)
		flash_until = sbs->GetRunTime() + 2000;

	next_check = 0; //update the light right away
}

void CardReader::Update(unsigned long now)
{
	//invalid badge: alternate red and green every quarter second
	if (flash_until > 0)
	{
		if (now < flash_until)
		{
			SetTexture(((now / 250) % 2 == 0) ? deny_texture : grant_texture);
			return;
		}
		flash_until = 0;
		next_check = 0;
	}

	//otherwise show the lock state, checked ten times a second
	if (now < next_check)
		return;
	next_check = now + 100;

	int state = controller ? controller->GetReaderState(access_point) : -1;
	if (state == 1)
		SetTexture(grant_texture); //unlocked: green
	else if (state == 0)
		SetTexture(deny_texture); //locked: red
	else
		SetTexture(idle_texture); //nothing to show (unknown access point)
}

bool CardReader::Enabled(bool value)
{
	if (is_enabled == value)
		return true;

	bool status = true;
	if (ReaderMesh)
		status = ReaderMesh->Enabled(value);
	is_enabled = value;
	return status;
}

void CardReader::OnClick(Vector3 &position, bool shift, bool ctrl, bool alt, bool right)
{
	if (!controller)
		return;

	//right-click cycles the active badge; left-click presents it
	if (right == true)
		controller->NextBadge();
	else
		controller->PresentWallet(access_point, this);
}

void CardReader::Report(const std::string &message)
{
	Object::Report("Card Reader " + access_point + ": " + message);
}

bool CardReader::ReportError(const std::string &message)
{
	return Object::ReportError("Card Reader " + access_point + ": " + message);
}

}
