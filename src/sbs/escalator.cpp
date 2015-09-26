/* $Id$ */

/*
	Scalable Building Simulator - Escalator Object
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
#include "escalator.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

Escalator::Escalator(Object *parent, const char *name, const char *sound_file, const char *texture, const char *direction, float CenterX, float CenterZ, float width, float risersize, float treadsize, int num_steps, float voffset, float tw, float th)
{
	//set up SBS object
	SetValues(this, parent, "Escalator", name, false);

	IsEnabled = true;
	Run = true;
	sbs->IncrementEscalatorCount();

	//create step meshes
	for (int i = 0; i < num_steps; i++)
	{
		MeshObject *mesh = new MeshObject(this, name, 0, sbs->GetConfigFloat("Skyscraper.SBS.MaxSmallRenderDistance", 100));
		Steps.push_back(mesh);
	}

	//create sound object
	sound = new Sound(this, name, true);
	sound->Load(sound_file);
}

Escalator::~Escalator()
{
	if (sound)
	{
		sound->parent_deleting = true;
		delete sound;
	}
	sound = 0;

	//remove step meshes
	for (int i = 0; i < (int)Steps.size(); i++)
	{
		if (Steps[i])
		{
			Steps[i]->parent_deleting = true;
			delete Steps[i];
		}
		Steps[i] = 0;
	}

	//unregister from parent
	if (sbs->FastDelete == false)
	{
		sbs->DecrementEscalatorCount();

		//unregister from parent
		if (parent_deleting == false)
		{
			if (std::string(GetParent()->GetType()) == "Floor")
				((Floor*)GetParent()->GetRawObject())->RemoveEscalator(this);
		}
	}
}

void Escalator::Enabled(bool value)
{
	//enable or disable escalator

	if (IsEnabled == value)
		return;

	for (int i = 0; i < (int)Steps.size(); i++)
		Steps[i]->Enable(value);

	IsEnabled = value;
}

void Escalator::Report(std::string message)
{
	//general reporting function
	sbs->Report("Escalator " + std::string(GetName()) + ": " + message);
}

bool Escalator::ReportError(std::string message)
{
	//general reporting function
	return sbs->ReportError("Escalator " + std::string(GetName()) + ": " + message);
}

void Escalator::Loop()
{
	//run loop

	if (!IsEnabled || !Run)
	{
		if (sound->IsPlaying() == true)
			sound->Stop();
		return;
	}

	if (sound->IsPlaying() == false)
	{
		sound->Loop(true);
		sound->Play();
	}
}
