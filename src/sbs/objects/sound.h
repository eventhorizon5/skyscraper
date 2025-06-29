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

#ifndef _SBS_SOUND_H
#define _SBS_SOUND_H

namespace SBS {

class SBSIMPEXP Sound : public Object
{
public:

	bool SetVelocity; //set sound velocity on move

	Sound(Object *parent, const std::string &name, bool permanent);
	~Sound();
	void SetVolume(Real value);
	Real GetVolume();
	void SetDistances(Real min, Real max);
	Real GetMinimumDistance();
	Real GetMaximumDistance();
	void SetDirection(const Vector3 &direction);
	Vector3 GetDirection();
	void SetLoopState(bool value);
	bool GetLoopState();
	void Pause(bool value = true);
	bool IsPaused();
	void Stop();
	bool Play(bool reset = true);
	void Reset();
	void SetSpeed(int percent);
	int GetSpeed();
	bool Load(const std::string &filename, bool force = false);
	bool IsPlaying();
	Real GetPlayPosition();
	void SetPlayPosition(Real percent);
	void SetConeSettings(Real inside_angle = 360.0, Real outside_angle = 360.0, Real outside_volume = 1.0);
	void SetDopplerLevel(Real level);
	bool IsLoaded();
	void PlayQueued(const std::string &filename, bool stop = true, bool loop = false);
	void ProcessQueue();
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	void OnMove(bool parent);
	void OnRotate(bool parent);
	void Unload();
	void Enabled(bool value);
	bool IsEnabled();
#ifndef DISABLE_SOUND
	FMOD::Channel* GetChannel();
#endif
	bool GetNearestReverbPosition(Vector3 &position);

private:

	bool IsValid();

#ifndef DISABLE_SOUND
	//sound channel
	FMOD::Channel *channel;
#endif

	//associated sound data
	SoundData *sound;

	SoundSystem *system;

	// sound parameters
	Vector3 Position;
	Vector3 Velocity;
	float Volume;
	float MaxDistance;
	float MinDistance;
	Vector3 Direction;
	bool SoundLoop;
	int Speed;
	float Percent;
	std::string Filename;
	float default_speed;
	float doppler_level;
	bool position_queued;
	bool enabled;

	struct SBSIMPEXP SoundEntry
	{
		std::string filename;
		bool loop;
		bool played;
	};

	std::vector<SoundEntry> queue;
};

}

#endif
