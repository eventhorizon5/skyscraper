/* $Id$ */

/*
	Scalable Building Simulator - Sound Class
	The Skyscraper Project - Version 1.3 Alpha
	Copyright (C)2005-2009 Ryan Thoryk
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

class SBSIMPEXP Sound
{
public:

	Sound(const char *filename);
	~Sound();
	void SetPosition(csVector3 position);
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
	void Play();
	void Reset();
	void SetSpeed(int percent);
	int GetSpeed();
	void Load(const char *filename);

private:

	//sound data
	csString filename;
	csRef<iDataBuffer> sndbuffer;
	csRef<iSndSysData> snddata;
	csRef<iSndSysStream> sndstream;

	//sound source
	csRef<iSndSysSource> sndsource;
	csRef<iSndSysSourceSoftware3D> sndsource3d;

};

#endif
