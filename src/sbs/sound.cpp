/* $Id$ */

/*
	Scalable Building Simulator - Sound Object
	The Skyscraper Project - Version 1.11 Alpha
	Copyright (C)2004-2017 Ryan Thoryk
	http://www.skyscrapersim.com
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@skyscrapersim.com

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
#include "floor.h"
#include "elevatorcar.h"
#include "soundsystem.h"
#include "sound.h"

namespace SBS {

Sound::Sound(Object *parent, const std::string &name, bool permanent) : Object(parent)
{
	//set up SBS object
	SetValues("Sound", name, permanent);

	//first set default values
	system = sbs->GetSoundSystem();
	Position = 0;
	Volume = (float)sbs->GetConfigFloat("Skyscraper.SBS.Sound.Volume", 1.0);
	MaxDistance = (float)sbs->GetConfigFloat("Skyscraper.SBS.Sound.MaxDistance", 10000.0);
	MinDistance =(float) sbs->GetConfigFloat("Skyscraper.SBS.Sound.MinDistance", 1.0);
	Direction = 0;
	SoundLoop = sbs->GetConfigBool("Skyscraper.SBS.Sound.Loop", false);
	Speed = sbs->GetConfigInt("Skyscraper.SBS.Sound.Speed", 100);
	Percent = 0;
	sbs->IncrementSoundCount();
	sound = 0;
	channel = 0;
	default_speed = 0;
	doppler_level = (float)sbs->GetConfigFloat("Skyscraper.SBS.Sound.Doppler", 0.0);
	position_queued = false;
	SetVelocity = false;
}

Sound::~Sound()
{
	if (system)
	{
		if (sbs->FastDelete == false)
		{
			Unload();
			sbs->DecrementSoundCount();
		}
		else
			Stop();
	}

	//unregister from parent
	if (sbs->FastDelete == false)
	{
		if (parent_deleting == false)
		{
			std::string type = GetParent()->GetType();

			if (type == "ElevatorCar")
				static_cast<ElevatorCar*>(GetParent())->RemoveSound(this);
			else if (type == "Floor")
				static_cast<Floor*>(GetParent())->RemoveSound(this);
			else if (type == "SBS")
				sbs->RemoveSound(this);
		}
	}
}

void Sound::OnMove(bool parent)
{
	Ogre::Vector3 global_position = sbs->ToGlobal(GetPosition());

	FMOD_VECTOR pos = {(float)global_position.x, (float)global_position.y, (float)global_position.z};
	FMOD_VECTOR vel = { 0, 0, 0 };

	//calculate sound velocity
	if (sbs->GetElapsedTime() > 0 && SetVelocity == true)
	{
		vel.x = (float)(GetPosition().x - Position.x) * (1000 / sbs->GetElapsedTime());
		vel.y = (float)(GetPosition().y - Position.y) * (1000 / sbs->GetElapsedTime());
		vel.z = (float)(GetPosition().z - Position.z) * (1000 / sbs->GetElapsedTime());
	}

	Position = GetPosition();
	Velocity.x = vel.x;
	Velocity.y = vel.y;
	Velocity.z = vel.z;
	if (channel)
		channel->set3DAttributes(&pos, &vel); //note - do not use ToRemote for positioning
}

void Sound::OnRotate(bool parent)
{
	if (parent == true)
		OnMove(parent); //update position if parent object has been rotated
}

void Sound::SetVolume(Real value)
{
	//set volume of sound
	Volume = (float)value;
	if (channel)
		channel->setVolume((float)value);
}

Real Sound::GetVolume()
{
	//returns volume
	return Volume;
}

void Sound::SetDistances(Real min, Real max)
{
	//set minimum and maximum unattenuated distances
	MinDistance = (float)min;
	MaxDistance = (float)max;
	if (channel)
		channel->set3DMinMaxDistance((float)min, (float)max);
}

Real Sound::GetMinimumDistance()
{
	return MinDistance;
}

Real Sound::GetMaximumDistance()
{
	return MaxDistance;
}

void Sound::SetDirection(const Ogre::Vector3 &direction)
{
	Direction = direction;
	Ogre::Vector3 global_direction = sbs->GetOrientation() * direction;
	FMOD_VECTOR vec = { (float)global_direction.x, (float)global_direction.y, (float)global_direction.z };

	if (channel)
		channel->set3DConeOrientation(&vec);
}

Ogre::Vector3 Sound::GetDirection()
{
	return Direction;
}

void Sound::SetConeSettings(Real inside_angle, Real outside_angle, Real outside_volume)
{
	if (channel)
		channel->set3DConeSettings((float)inside_angle, (float)outside_angle, (float)outside_volume);
}

void Sound::SetLoopState(bool value)
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
		Report("Stopping");

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
	{
		if (sound)
			sound->RemoveChannel(channel);
		channel = 0;
		return false;
	}
	return true;
}

bool Sound::Play(bool reset)
{
	//exit if sound is disabled
	if (!system)
		return false;

	if (!sound)
	{
		sound = system->GetSoundData(Filename);

		if (!sound)
		{
			if (sbs->Verbose)
				ReportError("No sound loaded");
			return false;
		}
		sound->AddHandle(this);
	}

	if (sbs->Verbose)
		Report("Playing");

	if (!IsValid())
	{
		//prepare sound (and keep paused)
		channel = system->Prepare(sound);

		if (!channel)
			return false;

		//get default speed value
		channel->getFrequency(&default_speed);

		//load previously stored values into new sound objects
		SetPosition(Position);
		SetVolume(Volume);
		SetDistances(MinDistance, MaxDistance);
		SetDirection(Direction);
		SetLoopState(SoundLoop);
		SetSpeed(Speed);
		SetDopplerLevel(doppler_level);
		if (position_queued == true)
			SetPlayPosition(Percent);
		position_queued = false;
	}

	if (reset == true)
		Reset();

	if (channel)
		channel->setPaused(false);

	return true;
}

void Sound::Reset()
{
	SetPlayPosition(0);
}

bool Sound::Load(const std::string &filename, bool force)
{
	//exit if sound is disabled
	if (!system)
		return false;

	//exit if filename is the same
	if (filename == Filename && force == false)
		return false;

	//clear current references
	Unload();

	//have sound system load sound file
	sound = system->Load(filename);
	Filename = filename;

	if (sound)
	{
		sound->AddHandle(this);
		return true;
	}
	return false;
}

Real Sound::GetPlayPosition()
{
	//returns the current sound playback position, in percent (1 = 100%)

	if (!IsValid())
		return Percent;

	if (!channel)
		return -1;

	//get length of sound in milliseconds
	unsigned int length = system->GetLength(sound);

	//get sound position in milliseconds
	unsigned int position;
	channel->getPosition(&position, FMOD_TIMEUNIT_MS);

	if (length > 0)
		Percent = float(position / length);
	return Percent;
}

void Sound::SetPlayPosition(Real percent)
{
	//sets the current sound playback position, in percent (1 = 100%)

	Percent = (float)percent;

	if (channel)
	{
		//get length of sound in milliseconds
		unsigned int length = system->GetLength(sound);

		unsigned int position = (unsigned int)(percent * length);
		channel->setPosition(position, FMOD_TIMEUNIT_MS);

		position_queued = true;
	}
}

void Sound::SetDopplerLevel(Real level)
{
	if (level < 0 || level > 5)
		return;

	doppler_level = (float)level;

	if (channel)
		channel->set3DDopplerLevel((float)level);
}

bool Sound::IsLoaded()
{
	return (sound != 0);
}

void Sound::Report(const std::string &message)
{
	Object::Report("Sound '" + GetName() + "', parent '" + GetParent()->GetName() + "': " + message);
}

bool Sound::ReportError(const std::string &message)
{
	return Object::ReportError("Sound '" + GetName() + "', parent '" + GetParent()->GetName() + "': " + message);
}

void Sound::PlayQueued(const std::string &filename, bool stop, bool loop)
{
	//loads and plays a sound, with queuing support
	//if "stop" is true, stops currently playing sound
	//use the ProcessQueue function to process queued sounds

	//queue sound object
	SoundEntry snd;
	snd.filename = filename;
	snd.loop = loop;
	snd.played = false;
	queue.push_back(snd);

	if (stop == true)
		Stop();

	ProcessQueue();
}

void Sound::ProcessQueue()
{
	//if using the PlayQueued function, use this function to process queued sounds

	if (queue.empty())
		return;

	SoundEntry *snd = &queue.front();

	if (IsPlaying() == true)
		return;
	else if (snd->played == true)
	{
		//dequeue sound that already played
		queue.erase(queue.begin());
		return;
	}

	//play new sound
	Load(snd->filename);
	SetLoopState(snd->loop);
	Play();
	snd->played = true;
}

void Sound::Unload()
{
	//stop and unload the sound channel

	Stop();
	if (sound)
		sound->RemoveHandle(this);
	sound = 0;
	channel = 0;
}

FMOD::Channel* Sound::GetChannel()
{
	return channel;
}

}
