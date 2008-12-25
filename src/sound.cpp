/* $Id:$ */

/*
	Scalable Building Simulator - Sound Class
	The Skyscraper Project - Version 1.2 Alpha
	Copyright (C)2005-2008 Ryan Thoryk
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

Sound::Sound(const char *filename)
{
	sndbuffer = sbs->vfs->ReadFile(filename);
	if (!sndbuffer)
	{
		sbs->ReportError("Can't load file '%s'", filename);
		return;
	}

	snddata = sbs->sndloader->LoadSound(sndbuffer);
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

	sndsource3d->SetPosition(csVector3(0, 0, 0));
	sndsource3d->SetVolume(1.0f);
	sndstream->SetLoopState(CS_SNDSYS_STREAM_DONTLOOP);
}

Sound::~Sound()
{
	//destructor

}

void Sound::SetPosition(csVector3 position)
{
	//set position of sound object
	sndsource3d->SetPosition(position);
}

csVector3 Sound::GetPosition()
{
	//get position of sound object
	return sndsource3d->GetPosition();
}

void Sound::SetVolume(float value)
{
	//set volume of sound
	sndsource3d->SetVolume(value);
}

float Sound::GetVolume()
{
	//returns volume
	return sndsource3d->GetVolume();
}

void Sound::SetMinimumDistance(float distance)
{
	sndsource3d->SetMinimumDistance(distance);
}

float Sound::GetMinimumDistance()
{
	return sndsource3d->GetMinimumDistance();
}

void Sound::SetMaximumDistance(float distance)
{
	//set the max distance at which the sound can be heard at full volume
	sndsource3d->SetMaximumDistance(distance);
}

float Sound::GetMaximumDistance()
{
	return sndsource3d->GetMaximumDistance();
}

void Sound::SetDirection(csVector3 direction)
{
	sndsource3d->SetDirection(direction);
}

csVector3 Sound::GetDirection()
{
	return sndsource3d->GetDirection();
}

void Sound::SetDirectionalRadiation(float rad)
{
	//from CS:
	//The directional radiation applies to sound that are oriented in a particular direction. 
	//This value is expressed in radians and describes the half-angle of a cone spreading
	//from the position of the source and opening in the direction of the source.
	//Set this value to 0.0f for an omni-directional sound. 

	sndsource3d->SetDirectionalRadiation(rad);
}

float Sound::GetDirectionalRadiation()
{
	return sndsource3d->GetDirectionalRadiation();
}

void Sound::Loop(bool value)
{
	if (value == true)
		sndstream->SetLoopState(CS_SNDSYS_STREAM_LOOP);
	else
		sndstream->SetLoopState(CS_SNDSYS_STREAM_DONTLOOP);
}

bool Sound::GetLoopState()
{
	if (sndstream->GetLoopState() == CS_SNDSYS_STREAM_LOOP)
		return true;
	else
		return false;
}

void Sound::Pause()
{
	sndstream->Pause();
}

bool Sound::IsPaused()
{
	if (sndstream->GetPauseState() == CS_SNDSYS_STREAM_PAUSED)
		return true;
	else
		return false;
}

void Sound::SetSpeed(int percent)
{
	sndstream->SetPlayRatePercent(percent);
}

int Sound::GetSpeed()
{
	return sndstream->GetPlayRatePercent();
}

void Sound::Stop()
{
	sndstream->Pause();
	sndstream->ResetPosition();
}

void Sound::Play()
{
	sndstream->Unpause();
}

void Sound::Reset()
{
	sndstream->ResetPosition();
}
