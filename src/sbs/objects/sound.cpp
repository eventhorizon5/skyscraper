/*
	Scalable Building Simulator - Sound Object
	The Skyscraper Project - Version 2.1
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

#ifndef DISABLE_SOUND
	#include <fmod.hpp>
#endif
#include "globals.h"
#include "sbs.h"
#include "utility.h"
#include "floor.h"
#include "elevatorcar.h"
#include "reverb.h"
#include "soundsystem.h"
#include "sound.h"

namespace SBS {

Sound::Sound(Object *parent, const std::string &name, bool permanent) : Object(parent)
{
	//set up SBS object
	SetValues("Sound", name, permanent);

	//first set default values
	system = sbs->GetSoundSystem();
	Position = Vector3::ZERO;
	Volume = (float)sbs->GetConfigFloat("Skyscraper.SBS.Sound.Volume", 1.0);
	MaxDistance = (float)sbs->GetConfigFloat("Skyscraper.SBS.Sound.MaxDistance", 10000.0);
	MinDistance =(float) sbs->GetConfigFloat("Skyscraper.SBS.Sound.MinDistance", 1.0);
	Direction = Vector3::ZERO;
	SoundLoop = sbs->GetConfigBool("Skyscraper.SBS.Sound.Loop", false);
	Speed = sbs->GetConfigInt("Skyscraper.SBS.Sound.Speed", 100);
	Percent = 0;
	sbs->IncrementSoundCount();
	sound = 0;
#ifndef DISABLE_SOUND
	channel = 0;
#endif
	default_speed = 0;
	doppler_level = (float)sbs->GetConfigFloat("Skyscraper.SBS.Sound.Doppler", 0.0);
	position_queued = false;
	SetVelocity = false;
	enabled = true;

	if (sbs->Verbose)
		Report("Created sound");
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
#ifndef DISABLE_SOUND
	Vector3 global_position = sbs->GetUtility()->ToGlobal(GetPosition());

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

	//adjust reverb wet level based on distance to listener
	/*Vector3 reverbPos;
	bool result = GetNearestReverbPosition(reverbPos);
	if (result == true)
	{
		Vector3 listener = system->GetListenerPosition();
		float distanceSquaredToListener = pow(listener.x - reverbPos.x, 2) + pow(listener.y - reverbPos.y, 2) + pow(listener.z - reverbPos.z, 2);
		channel->setReverbProperties(0, distanceSquaredToListener);
	}*/
#endif
}

void Sound::OnRotate(bool parent)
{
	if (parent == true)
		OnMove(parent); //update position if parent object has been rotated
}

void Sound::SetVolume(Real value)
{
	//set volume of sound

#ifndef DISABLE_SOUND
	if (sbs->Verbose)
		Report("Setting volume to " + ToString(value));

	Volume = (float)value;
	if (channel)
		channel->setVolume((float)value);
#endif
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
#ifndef DISABLE_SOUND
	if (channel)
		channel->set3DMinMaxDistance((float)min, (float)max);
#endif
}

Real Sound::GetMinimumDistance()
{
	return MinDistance;
}

Real Sound::GetMaximumDistance()
{
	return MaxDistance;
}

void Sound::SetDirection(const Vector3 &direction)
{
	Direction = direction;
	Vector3 global_direction = sbs->GetOrientation() * direction;
#ifndef DISABLE_SOUND
	FMOD_VECTOR vec = { (float)global_direction.x, (float)global_direction.y, (float)global_direction.z };

	if (channel)
		channel->set3DConeOrientation(&vec);
#endif
}

Vector3 Sound::GetDirection()
{
	return Direction;
}

void Sound::SetConeSettings(Real inside_angle, Real outside_angle, Real outside_volume)
{
#ifndef DISABLE_SOUND
	if (channel)
		channel->set3DConeSettings((float)inside_angle, (float)outside_angle, (float)outside_volume);
#endif
}

void Sound::SetLoopState(bool value)
{
	SoundLoop = value;
#ifndef DISABLE_SOUND
	if (channel)
	{
		if (value == true)
			channel->setLoopCount(-1);
		else
			channel->setLoopCount(0);
	}
#endif
}

bool Sound::GetLoopState()
{
	return SoundLoop;
}

void Sound::Pause(bool value)
{
	if (!IsValid())
		return;

	if (sbs->Verbose)
	{
		if (value == true)
			Report("Pause");
		else
			Report("Unpause");
	}

#ifndef DISABLE_SOUND
	if (channel)
		channel->setPaused(value);
#endif
}

bool Sound::IsPaused()
{
	bool paused = false;
	if (!IsValid())
		return true;
#ifndef DISABLE_SOUND
	if (channel)
		channel->getPaused(&paused);
#endif
	return paused;
}

bool Sound::IsPlaying()
{
	bool result = false;

	if (!IsValid())
		return false;

#ifndef DISABLE_SOUND

	if (!channel)
		return false;

	channel->isPlaying(&result);
	if (result == true && IsPaused() == false)
		return true;
#endif
	return false;
}

void Sound::SetSpeed(int percent)
{
	Speed = percent;
#ifndef DISABLE_SOUND
	if (!channel)
		return;

	if (sbs->Verbose)
		Report("Setting speed to " + ToString(percent));

	channel->setFrequency(default_speed * ((float)percent / 100));
#endif
}

int Sound::GetSpeed()
{
	return Speed;
}

void Sound::Stop()
{
#ifndef DISABLE_SOUND
	if (sbs->Verbose == true)
		Report("Stopping");

	if (channel)
		channel->stop();
#endif
}

bool Sound::IsValid()
{
#ifndef DISABLE_SOUND
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
#endif
	return true;
}

bool Sound::Play(bool reset)
{
	//exit if sound is disabled
	if (!system || !enabled)
		return true;

	//blacklist certain empty sounds
	if (Filename == "none.wav" || Filename == "beno/none.wav" || Filename == "")
		return true;

#ifndef DISABLE_SOUND
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

#endif
	return true;
}

void Sound::Reset()
{
	if (sbs->Verbose)
		Report("Reset");

	SetPlayPosition(0);
}

bool Sound::Load(const std::string &filename, bool force)
{
	//exit if sound is disabled
	if (!system || !enabled)
		return false;

	//blacklist certain empty sounds
	if (filename == "none.wav" || filename == "beno/none.wav")
		return false;

	//exit if filename is the same
	if (filename == Filename && force == false)
		return false;

	//clear current references
	Unload();

	if (sbs->Verbose)
		Report("Loading sound " + filename);

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

#ifndef DISABLE_SOUND

	if (!channel)
		return -1;

	//get length of sound in milliseconds
	unsigned int length = system->GetLength(sound);

	//get sound position in milliseconds
	unsigned int position;
	channel->getPosition(&position, FMOD_TIMEUNIT_MS);

	if (length > 0)
		Percent = (float)position / (float)length;
#endif
	return Percent;
}

void Sound::SetPlayPosition(Real percent)
{
	//sets the current sound playback position, in percent (1 = 100%)

	Percent = (float)percent;

#ifndef DISABLE_SOUND
	if (channel)
	{
		if (sbs->Verbose)
			Report("Setting play position to " + ToString(percent));

		//get length of sound in milliseconds
		unsigned int length = system->GetLength(sound);

		unsigned int position = (unsigned int)(percent * length);
		channel->setPosition(position, FMOD_TIMEUNIT_MS);

		position_queued = true;
	}
#endif
}

void Sound::SetDopplerLevel(Real level)
{
	if (level < 0 || level > 5)
		return;

	doppler_level = (float)level;

#ifndef DISABLE_SOUND
	if (channel)
		channel->set3DDopplerLevel((float)level);
#endif
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

	if (sbs->Verbose)
		Report("Playing queued sound");

	//queue sound object
	SoundEntry snd;
	snd.filename = filename;
	snd.loop = loop;
	snd.played = false;
	queue.emplace_back(snd);

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

	if (sbs->Verbose)
		Report("Processing queued sound");

	//play new sound
	bool result = Load(snd->filename);
	if (result)
	{
		SetLoopState(snd->loop);
		Play();
		snd->played = true;
	}
	else
	{
		//dequeue sound that could not be loaded
		queue.erase(queue.begin());
	}
}

void Sound::Unload()
{
	//stop and unload the sound channel

	Stop();

	if (sbs->Verbose)
		Report("Unloading");

	if (sound)
		sound->RemoveHandle(this);
	sound = 0;
#ifndef DISABLE_SOUND
	channel = 0;
#endif
}

#ifndef DISABLE_SOUND
FMOD::Channel* Sound::GetChannel()
{
	return channel;
}
#endif

bool Sound::GetNearestReverbPosition(Vector3 &position)
{
	std::string type = GetParent()->GetType();
	position = Vector3::ZERO;
	bool result = false;

	if (type == "ElevatorCar")
	{
		ElevatorCar *car = static_cast<ElevatorCar*>(GetParent());
		if (car->GetReverb())
		{
			position = car->GetReverb()->GetPosition();
			result = true;
		}
	}
	else if (type == "Floor")
	{
		Floor *floor = static_cast<Floor*>(GetParent());
		Real nearest = 99999999;
		for (int i = 0; i < floor->GetReverbCount(); i++)
		{
			Reverb *reverb = floor->GetReverb(i);
			if (!reverb)
				continue;

			Real distance = reverb->GetPosition().distance(position);
			if (distance < nearest)
			{
				nearest = distance;
				position = reverb->GetPosition();
				result = true;
			}
		}
	}
	else if (type == "SBS")
	{
		Real nearest = 99999999;
		for (int i = 0; i < sbs->GetReverbCount(); i++)
		{
			Reverb *reverb = sbs->GetReverb(i);
			if (!reverb)
				continue;

			Real distance = reverb->GetPosition().distance(position);
			if (distance < nearest)
			{
				nearest = distance;
				position = reverb->GetPosition();
				result = true;
			}
		}
	}

	return result;
}

bool Sound::Enabled(bool value)
{
	//enable or disable this sound object

	if (value == false)
		Stop();

	enabled = value;
	return true;
}

bool Sound::IsEnabled()
{
	return enabled;
}

}
