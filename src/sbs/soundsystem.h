/*
	Scalable Building Simulator - Sound System
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

#ifndef _SBS_SOUNDSYSTEM_H
#define _SBS_SOUNDSYSTEM_H

namespace FMOD {
	class Sound;
	class Channel;
	class System;
}

namespace SBS {

class SBSIMPEXP SoundSystem : public Object
{
public:

	SoundSystem(Object *parent, FMOD::System *fmodsystem);
	~SoundSystem();
	void SetListenerPosition(const Vector3 &position);
	void SetListenerDirection(const Vector3 &front, const Vector3 &top);
	void Loop();
	void Cleanup(int index = -1);
	unsigned int GetLength(SoundData *data);
	SoundData* Load(const std::string &filename);
	bool IsLoaded(std::string filename);
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	FMOD::Channel* Prepare(SoundData *data);
	SoundData* GetSoundData(std::string filename);
	SoundData* GetSoundData(int number);
	int GetPlayingCount();
	int GetSoundCount();
	void ShowLoadedSounds();
	void ShowPlayingSounds();

private:

	//FMOD system
	FMOD::System *soundsys;

	//listener sound objects
	Vector3 listener_position;
	Vector3 listener_velocity;
	Vector3 listener_forward;
	Vector3 listener_up;

	//sound data array
	std::vector<SoundData*> sounds;

	Vector3 Position;
};

struct SBSIMPEXP SoundData
{
	SoundData();
	~SoundData();
	void AddHandle(Sound *handle);
	void RemoveHandle(Sound *handle);
	int GetHandleCount() { return (int)handles.size(); }
	void AddChannel(FMOD::Channel *channel);
	void RemoveChannel(FMOD::Channel *channel);
	int GetChannelCount() { return (int)channels.size(); }
	FMOD::Sound* sound; //sound data object
	std::string filename; //filename of sound file
	std::vector<Sound*> handles; //associated sound objects
	std::vector<FMOD::Channel*> channels; //associated sound channels
};

}

#endif
