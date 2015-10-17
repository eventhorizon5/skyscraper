/* $Id$ */

/*
	Scalable Building Simulator - Sound Object
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

#ifndef _SBS_SOUND_H
#define _SBS_SOUND_H

namespace SBS {

class SBSIMPEXP Sound : public Object
{
public:

	bool SetVelocity; //set sound velocity on move

	Sound(Object *parent, const char *name, bool permanent);
	~Sound();
	void SetVolume(float value);
	float GetVolume();
	void SetDistances(float min, float max);
	float GetMinimumDistance();
	float GetMaximumDistance();
	void SetDirection(Ogre::Vector3 direction);
	Ogre::Vector3 GetDirection();
	void Loop(bool value);
	bool GetLoopState();
	void Pause();
	bool IsPaused();
	void Stop();
	bool Play(bool reset = true);
	void Reset();
	void SetSpeed(int percent);
	int GetSpeed();
	bool Load(const char *filename, bool force = false);
	bool IsPlaying();
	float GetPlayPosition();
	void SetPlayPosition(float percent);
	void SetConeSettings(float inside_angle = 360.0, float outside_angle = 360.0, float outside_volume = 1.0);
	void SetDopplerLevel(float level);
	bool IsLoaded();
	void PlayQueued(const char *filename, bool stop = true, bool loop = false);
	void ProcessQueue();
	void Report(std::string message);
	bool ReportError(std::string message);
	void OnMove();
	void OnRotate();

private:

	bool IsValid();

	//sound data
	FMOD::Sound *sound;
	FMOD::Channel *channel;

	// sound parameters
	Ogre::Vector3 Position;
	Ogre::Vector3 Velocity;
	float Volume;
	float MaxDistance;
	float MinDistance;
	Ogre::Vector3 Direction;
	bool SoundLoop;
	int Speed;
	float Percent;
	std::string Filename;
	float default_speed;
	float doppler_level;
	bool loaded;
	bool position_queued;

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
