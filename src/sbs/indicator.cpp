/*
	Scalable Building Simulator - Indicator Object
	The Skyscraper Project - Version 1.12 Alpha
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
#include "manager.h"
#include "texture.h"
#include "timer.h"
#include "sound.h"
#include "indicator.h"

namespace SBS {

class Indicator::Timer : public TimerObject
{
public:
	Indicator *parent;
	Timer(const std::string &name, Indicator *parent) : TimerObject(parent, name)
	{
		this->parent = parent;
	}
	virtual void Notify();
};

Indicator::Indicator(Object *parent, const std::string &sound, const std::string &texture_prefix, const std::string &blank_texture, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, Real timer_duration) : Object(parent)
{
	//creates a new display panel at the specified position

	//set up SBS object
	SetValues("Indicator", "Indicator", false);

	is_enabled = true;
	Prefix = texture_prefix;
	Blank = blank_texture;
	this->timer_duration = timer_duration;
	this->sound = 0;

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

		sbs->AddWallMain(this, Mesh, "Indicator", Blank, 0, -width / 2, 0, width / 2, 0, height, height, 0, 0, 1, 1, false);
	}
	if (tmpdirection == "left" || tmpdirection == "right")
	{
		if (tmpdirection == "left")
			sbs->DrawWalls(true, false, false, false, false, false);
		else
			sbs->DrawWalls(false, true, false, false, false, false);

		sbs->AddWallMain(this, Mesh, "Indicator", Blank, 0, 0, width / 2, 0, -width / 2, height, height, 0, 0, 1, 1, false);
	}
	sbs->ResetWalls();

	//create sound
	if (sound != "")
	{
		this->sound = new Sound(this, "Indicator Sound", true);
		soundfile = sound;
		TrimString(soundfile);
	}

	//move object
	Move(CenterX, voffset, CenterZ);

	//create timer
	timer = new Timer("Dispatch Timer", this);
}

Indicator::~Indicator()
{
	if (Mesh)
	{
		Mesh->parent_deleting = true;
		delete Mesh;
	}
	Mesh = 0;

	if (sound)
	{
		sound->parent_deleting = true;
		delete sound;
	}
	sound = 0;

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

void Indicator::Enabled(bool value)
{
	//turns display on/off

	if (is_enabled == value)
		return;

	Mesh->Enabled(value);
	is_enabled = value;
}

void Indicator::Update(const std::string &text, bool play_sound)
{
	//update display with given text

	if (text == "" && Blank != "")
	{
		Mesh->ChangeTexture(Blank);
		return;
	}

	//don't update texture if no value
	if (text == "")
		return;

	std::string texture = text;
	texture.insert(0, Prefix);

	Mesh->ChangeTexture(texture);
	sbs->GetTextureManager()->EnableLighting(texture, false);

	active_text = text;
	if (play_sound == true)
		PlaySound();

	timer->Start(timer_duration * 1000.0);
}

void Indicator::Off()
{
	if (Blank != "")
		Mesh->ChangeTexture(Blank);

	active_text = "";
}

void Indicator::Timer::Notify()
{
	//turn off indicator display when timer expires

	parent->Off();
}

bool Indicator::PlaySound()
{
	//play floor beep sound

	if (soundfile == "" || !sound)
		return false;

	if (sbs->Verbose)
		Report("playing indicator sound");

	std::string newsound = soundfile;
	std::string expanded = active_text;

	//don't use a sound file name with the "??" characters, expand to a full name for both error types
	if (active_text == "??")
		expanded = "invalid";
	if (active_text == "XX")
		expanded = "error";

	//remove direction characters
	ReplaceAll(expanded, "<", "");
	ReplaceAll(expanded, ">", "");
	ReplaceAll(expanded, "[]", "");

	//change the asterisk into the current active text value
	ReplaceAll(newsound, "*", expanded);
	TrimString(newsound);
	sound->Stop();
	sound->Load(newsound);
	sound->SetLoopState(false);
	sound->Play();
	return true;
}

}
