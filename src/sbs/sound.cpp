/* $Id$ */

/*
	Scalable Building Simulator - Sound Class
	The Skyscraper Project - Version 1.9 Alpha
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
#include "sound.h"
#include "camera.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

Sound::Sound(Object *parent, const char *name, bool permanent)
{
	//set up SBS object
	object = new Object();
	object->SetValues(this, parent, "Sound", name, permanent);

	//first set default values
	PositionOffset = 0;
	Position = 0;
	Volume = sbs->GetConfigFloat("Skyscraper.SBS.Sound.Volume", 1.0);
	MaxDistance = sbs->GetConfigFloat("Skyscraper.SBS.Sound.MaxDistance", 10000.0);
	MinDistance = sbs->GetConfigFloat("Skyscraper.SBS.Sound.MinDistance", 1.0);
	Direction = 0;
	SoundLoop = sbs->GetConfigBool("Skyscraper.SBS.Sound.Loop", false);
	Speed = sbs->GetConfigInt("Skyscraper.SBS.Sound.Speed", 100);
	Name = name;
	sbs->IncrementSoundCount();
	sound = 0;
	channel = 0;
	default_speed = 0;
	doppler_level = sbs->GetConfigFloat("Skyscraper.SBS.Sound.Doppler", 0.0);
}

Sound::~Sound()
{
	if (sbs->DisableSound == false)
	{
		Stop();
		sound->release();
		//delete sound;
		sound = 0;
		sbs->DecrementSoundCount();
	}

	//unregister from parent
	if (sbs->FastDelete == false)
	{
		if (object->parent_deleting == false)
		{
			if (std::string(object->GetParent()->GetType()) == "Elevator")
				((Elevator*)object->GetParent()->GetRawObject())->RemoveSound(this);
			if (std::string(object->GetParent()->GetType()) == "Floor")
				((Floor*)object->GetParent()->GetRawObject())->RemoveSound(this);
			if (std::string(object->GetParent()->GetType()) == "SBS")
				sbs->RemoveSound(this);
		}
	}

	//destructor
	delete object;
}

void Sound::SetPosition(const Ogre::Vector3& position, bool set_velocity)
{
	//set position of sound object

	FMOD_VECTOR pos = {position.x, position.y, position.z};
	FMOD_VECTOR vel;
	vel.x = 0;
	vel.y = 0;
	vel.z = 0;

	//calculate sound velocity
	if (sbs->GetElapsedTime() > 0 && set_velocity == true)
	{
		vel.x = (position.x - Position.x) * (1000 / sbs->GetElapsedTime());
		vel.y = (position.y - Position.y) * (1000 / sbs->GetElapsedTime());
		vel.z = (position.z - Position.z) * (1000 / sbs->GetElapsedTime());
	}

	Position = position;
	Velocity = (vel.x, vel.y, vel.z);
	if (channel)
		channel->set3DAttributes(&pos, &vel); //note - do not use ToRemote for positioning
}

void Sound::SetPositionY(float position, bool set_velocity)
{
	//set vertical position of sound object
	Ogre::Vector3 newposition = Position;
	newposition.y = position;
	SetPosition(newposition, set_velocity);
}

Ogre::Vector3 Sound::GetPosition()
{
	return Position;
}

void Sound::SetVolume(float value)
{
	//set volume of sound
	Volume = value;
	if (channel)
		channel->setVolume(value);
}

float Sound::GetVolume()
{
	//returns volume
	return Volume;
}

void Sound::SetDistances(float min, float max)
{
	//set minimum and maximum unattenuated distances
	MinDistance = min;
	MaxDistance = max;
	if (channel)
		channel->set3DMinMaxDistance(min, max);
}

float Sound::GetMinimumDistance()
{
	return MinDistance;
}

float Sound::GetMaximumDistance()
{
	return MaxDistance;
}

void Sound::SetDirection(Ogre::Vector3 direction)
{
	Direction = direction;
	FMOD_VECTOR vec;
	vec.x = direction.x;
	vec.y = direction.y;
	vec.z = direction.z;

	if (channel)
		channel->set3DConeOrientation(&vec);
}

Ogre::Vector3 Sound::GetDirection()
{
	return Direction;
}

void Sound::SetConeSettings(float inside_angle, float outside_angle, float outside_volume)
{
	if (channel)
		channel->set3DConeSettings(inside_angle, outside_angle, outside_volume);
}

void Sound::Loop(bool value)
{
	SoundLoop = value;
	if (channel)
	{
		if (value == true)
			channel->setLoopCount(-1);
		else
			channel->setLoopCount(0);
	}
}

bool Sound::GetLoopState()
{
	return SoundLoop;
}

void Sound::Pause()
{
	if (!IsValid())
		return;
	if (channel)
		channel->setPaused(true);
}

bool Sound::IsPaused()
{
	bool paused = false;
	if (!IsValid())
		return true;
	if (channel)
		channel->getPaused(&paused);
	return paused;
}

bool Sound::IsPlaying()
{
	bool result = false;

	if (!IsValid())
		return false;

	if (!channel)
		return false;

	channel->isPlaying(&result);
	if (result == true && IsPaused() == false)
		return true;
	return false;
}

void Sound::SetSpeed(int percent)
{
	Speed = percent;
	if (!channel)
		return;

	channel->setFrequency(default_speed * (percent / 100));
}

int Sound::GetSpeed()
{
	return Speed;
}

void Sound::Stop()
{
	if (sbs->Verbose == true)
	{
		char intbuffer[65];
		std::string objectnum = _itoa(object->GetNumber(), intbuffer, 10);
		sbs->Report("Stopping sound '" + Name + "', object " + objectnum + ", parent '" + object->GetParent()->GetName() + "'");
	}

	if (channel)
		channel->stop();
}

bool Sound::IsValid()
{
	if (!channel)
		return false;
	bool playing;
	FMOD_RESULT result = channel->isPlaying(&playing);
	if (result == FMOD_ERR_INVALID_HANDLE || result == FMOD_ERR_CHANNEL_STOLEN)
		return false;
	return true;
}

void Sound::Play(bool reset)
{
	if (sbs->Verbose == true)
	{
		char intbuffer[65];
		std::string objectnum = _itoa(object->GetNumber(), intbuffer, 10);
		sbs->Report("Playing sound '" + Name + "', object " + objectnum + ", parent '" + object->GetParent()->GetName() + "'");
	}

	if (!IsValid())
	{
		//prepare sound (and keep paused)
		FMOD_RESULT result = sbs->soundsys->playSound(FMOD_CHANNEL_FREE, sound, true, &channel);

		if (result != FMOD_OK || !channel)
			return;

		//get default speed value
		channel->getFrequency(&default_speed);

		//load previously stored values into new sound objects
		SetPosition(Position);
		SetVolume(Volume);
		SetDistances(MinDistance, MaxDistance);
		SetDirection(Direction);
		Loop(SoundLoop);
		SetSpeed(Speed);
		SetDopplerLevel(doppler_level);
	}

	if (reset == true)
		Reset();
	if (channel)
		channel->setPaused(false);
}

void Sound::Reset()
{
	SetPlayPosition(0);
}

void Sound::Load(const char *filename, bool force)
{
	//exit if filename is the same
	std::string filename_new = filename;
	if (filename_new == Filename && force == false)
		return;

	//exit if none specified
	if (filename_new == "")
		return;

	//clear old object references
	if (channel)
		channel->stop();
	channel = 0;
	if (sound)
		sound->release();
	sound = 0;

	//exit if sound is disabled
	if (sbs->DisableSound == true)
		return;

	//load new sound
	Filename = filename;
	std::string full_filename1 = "data/";
	full_filename1.append(filename);
	std::string full_filename = sbs->VerifyFile(full_filename1.c_str());

	FMOD_RESULT result = sbs->soundsys->createSound(full_filename.c_str(), (FMOD_MODE)(FMOD_3D | FMOD_ACCURATETIME | FMOD_SOFTWARE | FMOD_LOOP_NORMAL), 0, &sound);
	//FMOD_RESULT result = sbs->soundsys->createStream(Filename.c_str(), (FMOD_MODE)(FMOD_SOFTWARE | FMOD_3D), 0, &sound); //streamed version
	if (result != FMOD_OK)
	{
		sbs->ReportError("Can't load file '" + Filename + "'");
		return;
	}
}

float Sound::GetPlayPosition()
{
	//returns the current sound playback position, in percent (1 = 100%)

	if (!IsValid())
		return 0.0;
	if (!channel)
		return -1;

	//get length of sound in milliseconds
	unsigned int length;
	sound->getLength(&length, FMOD_TIMEUNIT_MS);

	//get sound position in milliseconds
	unsigned int position;
	channel->getPosition(&position, FMOD_TIMEUNIT_MS);

	return position / length;
}

void Sound::SetPlayPosition(float percent)
{
	//sets the current sound playback position, in percent (1 = 100%)

	if (!channel)
		return;

	//get length of sound in milliseconds
	unsigned int length;
	sound->getLength(&length, FMOD_TIMEUNIT_MS);

	unsigned int position;
	position = percent * length;
	channel->setPosition(position, FMOD_TIMEUNIT_MS);
}

const char *Sound::GetName()
{
	return Name.c_str();
}

void Sound::SetDopplerLevel(float level)
{
	if (level < 0 || level > 5)
		return;

	doppler_level = level;

	if (channel)
		channel->set3DDopplerLevel(level);
}
