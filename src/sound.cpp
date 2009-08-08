/* $Id$ */

/*
	Scalable Building Simulator - Sound Class
	The Skyscraper Project - Version 1.5 Alpha
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

#include "globals.h"
#include "sound.h"
#include "sbs.h"
#include "camera.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

Sound::Sound()
{
	//first set default values
	Position = csVector3(0);
	Volume = 1.0;
	MaxDistance = -1.0;
	MinDistance = 1.0;
	Direction = 0;
	DirectionalRadiation = 0;
	SoundLoop = false;
	Speed = 100;
}

Sound::~Sound()
{
	//destructor

}

void Sound::SetPosition(csVector3 position)
{
	//set position of sound object
	Position = position;
	if (sndsource3d)
		sndsource3d->SetPosition(Position);
}

void Sound::SetPositionY(float position)
{
	//set vertical position of sound object
	Position.y = position;
	if (sndsource3d)
		sndsource3d->SetPosition(Position);
}

csVector3 Sound::GetPosition()
{
	//get position of sound object
	if (sndsource3d)
		return sndsource3d->GetPosition();
	else
		return csVector3(0, 0, 0);
}

void Sound::SetVolume(float value)
{
	//set volume of sound
	Volume = value;
	if (sndsource3d)
		sndsource3d->SetVolume(Volume);
}

float Sound::GetVolume()
{
	//returns volume
	if (sndsource3d)
		return sndsource3d->GetVolume();
	else
		return 0;
}

void Sound::SetMinimumDistance(float distance)
{
	MinDistance = distance;
	if (sndsource3d)
		sndsource3d->SetMinimumDistance(MinDistance);
}

float Sound::GetMinimumDistance()
{
	if (sndsource3d)
		return sndsource3d->GetMinimumDistance();
	else
		return 0;
}

void Sound::SetMaximumDistance(float distance)
{
	//set the max distance at which the sound can be heard at full volume
	MaxDistance = distance;
	if (sndsource3d)
		sndsource3d->SetMaximumDistance(MaxDistance);
}

float Sound::GetMaximumDistance()
{
	if (sndsource3d)
		return sndsource3d->GetMaximumDistance();
	else
		return 0;
}

void Sound::SetDirection(csVector3 direction)
{
	Direction = direction;
	if (sndsource3d)
		sndsource3d->SetDirection(Direction);
}

csVector3 Sound::GetDirection()
{
	if (sndsource3d)
		return sndsource3d->GetDirection();
	else
		return 0;
}

void Sound::SetDirectionalRadiation(float rad)
{
	//from CS:
	//The directional radiation applies to sound that are oriented in a particular direction.
	//This value is expressed in radians and describes the half-angle of a cone spreading
	//from the position of the source and opening in the direction of the source.
	//Set this value to 0.0f for an omni-directional sound.

	DirectionalRadiation = rad;
	if (sndsource3d)
		sndsource3d->SetDirectionalRadiation(DirectionalRadiation);
}

float Sound::GetDirectionalRadiation()
{
	if (sndsource3d)
		return sndsource3d->GetDirectionalRadiation();
	else
		return 0;
}

void Sound::Loop(bool value)
{
	SoundLoop = value;
	if (sndstream)
	{
		if (value == true)
			sndstream->SetLoopState(CS_SNDSYS_STREAM_LOOP);
		else
			sndstream->SetLoopState(CS_SNDSYS_STREAM_DONTLOOP);
	}
}

bool Sound::GetLoopState()
{
	if (sndstream)
	{
		if (sndstream->GetLoopState() == CS_SNDSYS_STREAM_LOOP)
			return true;
		else
			return false;
	}
	else
		return false;
}

void Sound::Pause()
{
	if (sndstream)
		sndstream->Pause();
}

bool Sound::IsPaused()
{
	if (sndstream)
	{
		if (sndstream->GetPauseState() == CS_SNDSYS_STREAM_PAUSED)
			return true;
		else
			return false;
	}
	else
		return false;
}

bool Sound::IsPlaying()
{
	if (IsPaused() == false)
		return true;
	else
		return false;
}

void Sound::SetSpeed(int percent)
{
	Speed = percent;
	if (sndstream)
		sndstream->SetPlayRatePercent(Speed);
}

int Sound::GetSpeed()
{
	if (sndstream)
		return sndstream->GetPlayRatePercent();
	else
		return 0;
}

void Sound::Stop()
{
	Pause();
	Reset();
}

void Sound::Play()
{
	Reset();
	if (sndstream)
		sndstream->Unpause();
}

void Sound::Reset()
{
	if (sndstream)
		sndstream->ResetPosition();
}

void Sound::Load(const char *filename)
{
	//clear old object references
	sndsource3d = 0;
	sndsource = 0;
	sndstream = 0;

	//exit if sound is disabled
	if (sbs->DisableSound == true)
		return;

	//load new sound
	csString full_filename = "/root/data/";
	full_filename.Append(filename);
	csRef<iDataBuffer> sndbuffer = sbs->vfs->ReadFile(full_filename.GetData());
	if (!sndbuffer)
	{
		sbs->ReportError("Can't load file '%s'", filename);
		return;
	}

	csRef<iSndSysData> snddata = sbs->sndloader->LoadSound(sndbuffer);
	if (!snddata)
	{
		sbs->ReportError("Can't load sound '%s'", filename);
		return;
	}

	sndstream = sbs->sndrenderer->CreateStream(snddata, CS_SND3D_ABSOLUTE);
	if (!sndstream)
	{
		sbs->ReportError("Can't create stream for '%s'", filename);
		return;
	}

	sndsource = sbs->sndrenderer->CreateSource(sndstream);
	if (!sndsource)
	{
		sbs->ReportError("Can't create source for '%s'", filename);
		return;
	}
	sndsource3d = scfQueryInterface<iSndSysSourceSoftware3D> (sndsource);

	//load previously stored values into new sound objects
	SetPosition(Position);
	SetVolume(Volume);
	SetMaximumDistance(MaxDistance);
	SetMinimumDistance(MinDistance);
	SetDirection(Direction);
	SetDirectionalRadiation(DirectionalRadiation);
	Loop(SoundLoop);
	SetSpeed(Speed);
}
