/* $Id$ */

/*
	Scalable Building Simulator - Sound System
	The Skyscraper Project - Version 1.11 Alpha
	Copyright (C)2004-2018 Ryan Thoryk
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
#include "camera.h"
#include "sound.h"
#include "profiler.h"
#include "soundsystem.h"

namespace SBS {

SoundSystem::SoundSystem(Object *parent, FMOD::System *fmodsystem) : Object(parent)
{
	SetValues("SoundSystem", "Sound System", true, false);

	soundsys = fmodsystem;

	listener_position.x = 0;
	listener_position.y = 0;
	listener_position.z = 0;
	listener_velocity.x = 0;
	listener_velocity.y = 0;
	listener_velocity.z = 0;
	listener_forward.x = 0;
	listener_forward.y = 0;
	listener_forward.z = 0;
	listener_up.x = 0;
	listener_up.y = 0;
	listener_up.z = 0;
	Position = Ogre::Vector3::ZERO;

	//set up sound options (mainly to set sound distance factor to feet instead of meters)
	soundsys->set3DSettings(1.0f, 3.28f, 1.0f);
}

SoundSystem::~SoundSystem()
{
	for (size_t i = 0; i < sounds.size(); i++)
		delete sounds[i];
}

void SoundSystem::Loop()
{
	//update sound
	if (enable_advanced_profiling == false)
		ProfileManager::Start_Profile("Sound");
	else
		ProfileManager::Start_Profile("FMOD");

	//sync sound listener object to camera position
	if (sbs->camera->IsActive() == true)
		SetListenerPosition(sbs->camera->GetPosition());

	//set direction of listener to camera's direction
	Ogre::Vector3 front, top;
	sbs->camera->GetDirection(front, top, true);
	SetListenerDirection(front, top);

	//update FMOD
	soundsys->update();

	ProfileManager::Stop_Profile();
}

void SoundSystem::SetListenerPosition(const Ogre::Vector3 &position)
{
	//set position of sound listener object

	unsigned int timing;
	if (sbs->SmoothFrames > 0)
		timing = sbs->GetAverageTime();
	else
		timing = sbs->GetElapsedTime();

	//calculate sound velocity
	if (timing > 0)
	{
		listener_velocity.x = (float)(position.x - Position.x) * (1000 / timing);
		listener_velocity.y = (float)(position.y - Position.y) * (1000 / timing);
		listener_velocity.z = (float)(position.z - Position.z) * (1000 / timing);
	}

	Position = position;

	Ogre::Vector3 global_position = sbs->ToGlobal(position);

	listener_position.x = (float)global_position.x;
	listener_position.y = (float)global_position.y;
	listener_position.z = (float)global_position.z;

	soundsys->set3DListenerAttributes(0, &listener_position, &listener_velocity, &listener_forward, &listener_up);
}

void SoundSystem::SetListenerDirection(const Ogre::Vector3 &front, const Ogre::Vector3 &top)
{
	//set direction of sound listener object

	listener_forward.x = (float)front.x;
	listener_forward.y = (float)front.y;
	listener_forward.z = (float)front.z;
	listener_up.x = (float)top.x;
	listener_up.y = (float)top.y;
	listener_up.z = (float)top.z;

	soundsys->set3DListenerAttributes(0, &listener_position, &listener_velocity, &listener_forward, &listener_up);
}

void SoundSystem::Cleanup()
{
	//unloads sounds that are not associated with any channels

	for (int i = 0; i < GetSoundCount(); i++)
	{
		if (sounds[i]->handles.size() == 0)
		{
			delete sounds[i];
			sounds.erase(sounds.begin() + i);
			i--;
		}
	}
}

unsigned int SoundSystem::GetLength(SoundData *data)
{
	//get length of sound in milliseconds

	if (!data)
		return 0;

	unsigned int length;
	data->sound->getLength(&length, FMOD_TIMEUNIT_MS);
	return length;
}

SoundData* SoundSystem::Load(const std::string &filename)
{
	//load a sound file from specified filename

	//exit if none specified
	if (filename == "")
		return 0;

	//return existing data element if file is already loaded
	SoundData *existing = GetSoundData(filename);
	if (existing)
		return existing;

	//create new SoundData element
	SoundData *data = new SoundData();
	data->filename = SetCaseCopy(filename, false);

	//load new sound
	std::string full_filename1 = "data/";
	full_filename1.append(filename);
	std::string full_filename = sbs->VerifyFile(full_filename1);

#if (FMOD_VERSION >> 16 == 4)
	FMOD_RESULT result = soundsys->createSound(full_filename.c_str(), (FMOD_MODE)(FMOD_3D | FMOD_ACCURATETIME | FMOD_SOFTWARE | FMOD_LOOP_NORMAL), 0, &data->sound);
	//FMOD_RESULT result = soundsys->createStream(full_filename.c_str(), (FMOD_MODE)(FMOD_SOFTWARE | FMOD_3D), 0, &data.sound); //streamed version
#else
	FMOD_RESULT result = soundsys->createSound(full_filename.c_str(), (FMOD_MODE)(FMOD_3D | FMOD_ACCURATETIME | FMOD_LOOP_NORMAL), 0, &data->sound);
	//FMOD_RESULT result = soundsys->createStream(full_filename.c_str(), (FMOD_MODE)(FMOD_3D), 0, &data.sound); //streamed version
#endif

	if (result != FMOD_OK)
	{
		ReportError("Can't load file '" + filename + "'");
		return 0;
	}

	//add sound element to array
	sounds.push_back(data);

	return data;
}

bool SoundSystem::IsLoaded(std::string filename)
{
	//return true if a specific file is already loaded

	SetCase(filename, false);
	for (int i = 0; i < GetSoundCount(); i++)
	{
		std::string check = sounds[i]->filename;

		if (check == filename)
			return true;
	}
	return false;
}

FMOD::Channel* SoundSystem::Prepare(SoundData *data)
{
	//prepare a sound for play - this allocates a channel

	if (!data)
		return 0;

	FMOD::Channel *channel = 0;
#if (FMOD_VERSION >> 16 == 4)
	FMOD_RESULT result = soundsys->playSound(FMOD_CHANNEL_FREE, data->sound, true, &channel);
#else
	FMOD_RESULT result = soundsys->playSound(data->sound, 0, true, &channel);
#endif

	if (result != FMOD_OK || !channel)
		return 0;

	data->AddChannel(channel);

	return channel;
}

SoundData* SoundSystem::GetSoundData(std::string filename)
{
	//get sound data element for related filename

	SetCase(filename, false);
	for (int i = 0; i < GetSoundCount(); i++)
	{
		if (sounds[i]->filename == filename)
			return sounds[i];
	}
	return 0;
}

void SoundSystem::Report(const std::string &message)
{
	Object::Report("Sound System: " + message);
}

bool SoundSystem::ReportError(const std::string &message)
{
	return Object::ReportError("Sound System: " + message);
}

int SoundSystem::GetPlayingCount()
{
	//get number of playing channels

	int num;
	soundsys->getChannelsPlaying(&num);
	return num;
}

int SoundSystem::GetSoundCount()
{
	return (int)sounds.size();
}

void SoundSystem::ShowLoadedSounds()
{
	Object::Report("\n--- Loaded Sounds ---\n");
	Object::Report("Filename\t----\tSound Objects\t----\tChannels");
	for (int i = 0; i < GetSoundCount(); i++)
	{
		Object::Report(sounds[i]->filename + "\t----\t" + ToString(sounds[i]->GetHandleCount()) + "\t----\t" + ToString(sounds[i]->GetChannelCount()));
	}
	Object::Report("\nTotal loaded sounds: " + ToString(GetSoundCount()));
}

void SoundSystem::ShowPlayingSounds()
{
	Object::Report("\n--- Playing Sounds ---\n");
	for (int i = 0; i < GetSoundCount(); i++)
	{
		bool first = true;
		for (int j = 0; j < sounds[i]->GetHandleCount(); j++)
		{
			if (sounds[i]->handles[j]->IsPlaying() == true)
			{
				if (first == true)
					Object::Report(sounds[i]->filename + ":");
				first = false;
				Sound *sound = sounds[i]->handles[j];
				Object::Report("\t" + sound->GetName() + "\t----\tParent: " + sound->GetParent()->GetName());
			}
		}
	}
	Object::Report("\nTotal playing sounds: " + ToString(GetPlayingCount()));
}

SoundData::SoundData()
{
	sound = 0;
}

SoundData::~SoundData()
{
	if (sound)
		sound->release();
	sound = 0;
}

void SoundData::AddHandle(Sound *handle)
{
	//add a sound object handle

	if (handle == 0)
		return;

	for (int i = 0; i < GetHandleCount(); i++)
	{
		if (handles[i] == handle)
			return;
	}
	handles.push_back(handle);
}

void SoundData::RemoveHandle(Sound *handle)
{
	//remove a sound object handle

	if (handle == 0)
		return;

	for (int i = 0; i < GetHandleCount(); i++)
	{
		if (handles[i] == handle)
		{
			handles.erase(handles.begin() + i);
			RemoveChannel(handle->GetChannel());
			return;
		}
	}
}

void SoundData::AddChannel(FMOD::Channel *channel)
{
	//add a sound channel

	if (channel == 0)
		return;

	for (int i = 0; i < GetChannelCount(); i++)
	{
		if (channels[i] == channel)
			return;
	}
	channels.push_back(channel);
}

void SoundData::RemoveChannel(FMOD::Channel *channel)
{
	//remove a sound channel

	if (channel == 0)
		return;

	for (int i = 0; i < GetChannelCount(); i++)
	{
		if (channels[i] == channel)
		{
			channels.erase(channels.begin() + i);
			return;
		}
	}
}

}
