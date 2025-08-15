/*
	Scalable Building Simulator - Sound System
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

#ifndef _SBS_SOUNDSYSTEM_H
#define _SBS_SOUNDSYSTEM_H

namespace FMOD {
	class Sound;
	class Channel;
	class System;
	class Reverb3D;
}

namespace SBS {

class SBSIMPEXP SoundSystem : public Object
{
public:

	SoundSystem(Object *parent, FMOD::System *fmodsystem);
	SoundSystem(Object *parent);
	~SoundSystem();
	void SetListenerPosition(const Vector3 &position);
	void SetListenerDirection(const Vector3 &front, const Vector3 &top);
	Vector3 GetListenerPosition() { return listener_position; }
	bool Loop();
	void Cleanup(int index = -1);
	unsigned int GetLength(SoundData *data);
	SoundData* Load(const std::string &filename);
	bool IsLoaded(std::string filename);
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
#ifndef DISABLE_SOUND
	FMOD::Channel* Prepare(SoundData *data);
	FMOD::System* GetFmodSystem();
#endif
	SoundData* GetSoundData(std::string filename);
	SoundData* GetSoundData(int number);
	int GetPlayingCount();
	int GetSoundCount();
	void ShowLoadedSounds();
	void ShowPlayingSounds(bool verbose = true);
	void ShowPlayingTotal();

private:

#ifndef DISABLE_SOUND
	//FMOD system
	FMOD::System *soundsys;
#endif

	//listener sound data
	Vector3 listener_position;
	Vector3 listener_velocity;
	Vector3 listener_forward;
	Vector3 listener_up;

	//sound data array
	std::vector<SoundData*> sounds;

	Vector3 Position;

	//reverbs
	struct Reverb
	{
#ifndef DISABLE_SOUND
		FMOD::Reverb3D* object;
#endif
		Vector3 position;
	};
	std::vector<Reverb> reverbs;
};

struct SBSIMPEXP SoundData
{
	SoundData();
	~SoundData();
	void AddHandle(Sound *handle);
	void RemoveHandle(Sound *handle);
	int GetHandleCount() { return (int)handles.size(); }
#ifndef DISABLE_SOUND
	void AddChannel(FMOD::Channel *channel);
	void RemoveChannel(FMOD::Channel *channel);
	int GetChannelCount() { return (int)channels.size(); }
#else
	int GetChannelCount() { return 0; }
#endif
	FMOD::Sound* sound; //sound data object
	std::string filename; //filename of sound file
	std::vector<Sound*> handles; //associated sound objects
#ifndef DISABLE_SOUND
	std::vector<FMOD::Channel*> channels; //associated sound channels
#endif
};

}

#endif
