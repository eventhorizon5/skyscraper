/* $Id$ */

/*
	Scalable Building Simulator - Sound Class
	The Skyscraper Project - Version 1.6 Alpha
	Copyright (C)2004-2010 Ryan Thoryk
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

#include <isndsys/ss_stream.h>
#include <isndsys/ss_source.h>
#include <isndsys/ss_manager.h>

class SBSIMPEXP Sound
{
public:

	Object *object; //SBS object
	csVector3 PositionOffset; //optional position offset, used only by other objects

	Sound(Object *parent, const char *name);
	~Sound();
	void SetPosition(csVector3 position);
	void SetPositionY(float position);
	csVector3 GetPosition();
	void SetVolume(float value);
	float GetVolume();
	void SetMinimumDistance(float distance);
	float GetMinimumDistance();
	void SetMaximumDistance(float distance);
	float GetMaximumDistance();
	void SetDirection(csVector3 direction);
	csVector3 GetDirection();
	void SetDirectionalRadiation(float rad);
	float GetDirectionalRadiation();
	void Loop(bool value);
	bool GetLoopState();
	void Pause();
	bool IsPaused();
	void Stop();
	void Play(bool reset = true);
	void Reset();
	void SetSpeed(int percent);
	int GetSpeed();
	void Load(const char *filename, bool force = false);
	bool IsPlaying();
	float GetPlayPosition();
	void SetPlayPosition(float percent);

private:

	//sound data
	csRef<iSndSysStream> sndstream;

	//sound source
	csRef<iSndSysSource> sndsource;
	csRef<iSndSysSource3D> sndsource3d;
	csRef<iSndSysSource3DDirectionalSimple> directional;
	csRef<iSndSysWrapper> sndwrapper;

	// sound parameters
	csVector3 Position;
	float Volume;
	float MaxDistance;
	float MinDistance;
	csVector3 Direction;
	float DirectionalRadiation;
	bool SoundLoop;
	int Speed;
	csString Filename;

};

#endif
