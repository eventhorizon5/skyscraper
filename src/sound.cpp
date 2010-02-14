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

#include "globals.h"
#include "sbs.h"
#include "sound.h"
#include "camera.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

Sound::Sound(Object *parent, const char *name)
{
	//set up SBS object
	object = new Object();
	object->SetValues(this, parent, "Sound", false);

	//first set default values
	PositionOffset = 0;
	Position = 0;
	Volume = sbs->confman->GetFloat("Skyscraper.SBS.Sound.Volume", 1.0);
	MaxDistance = sbs->confman->GetFloat("Skyscraper.SBS.Sound.MaxDistance", -1.0);
	MinDistance = sbs->confman->GetFloat("Skyscraper.SBS.Sound.MinDistance", 1.0);
	Direction = 0;
	DirectionalRadiation = 0;
	SoundLoop = sbs->confman->GetBool("Skyscraper.SBS.Sound.Loop", false);
	Speed = sbs->confman->GetInt("Skyscraper.SBS.Sound.Speed", 100);
	sndwrapper = sbs->sndmanager->CreateSound("");
	sbs->IncrementSoundCount();
}

Sound::~Sound()
{
	Stop();
	sbs->sndrenderer->RemoveSource(sndsource);
	sbs->sndrenderer->RemoveStream(sndstream);
	sbs->sndmanager->RemoveSound(sndwrapper);

	//destructor
	directional = 0;
	sndsource3d = 0;
	sndsource = 0;
	sndstream = 0;
	delete object;
}

void Sound::SetPosition(csVector3 position)
{
	//set position of sound object
	Position = position;
	if (sndsource3d)
		sndsource3d->SetPosition(Position); //note - do not use ToRemote for positioning
}

void Sound::SetPositionY(float position)
{
	//set vertical position of sound object
	Position.y = position;
	SetPosition(Position);
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
	if (sndsource)
		sndsource->SetVolume(Volume);
}

float Sound::GetVolume()
{
	//returns volume
	if (sndsource)
		return sndsource->GetVolume();
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
	if (directional)
		directional->SetDirection(Direction);
}

csVector3 Sound::GetDirection()
{
	if (directional)
		return directional->GetDirection();
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
	if (directional)
		directional->SetDirectionalRadiation(DirectionalRadiation);
}

float Sound::GetDirectionalRadiation()
{
	if (directional)
		return directional->GetDirectionalRadiation();
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

void Sound::Play(bool reset)
{
	if (reset == true)
		Reset();
	if (sndstream)
		sndstream->Unpause();
}

void Sound::Reset()
{
	if (sndstream)
		sndstream->ResetPosition();
}

void Sound::Load(const char *filename, bool force)
{
	//exit if filename is the same
	csString filename_new = filename;
	if (filename_new == Filename && force == false)
		return;

	//exit if none specified
	if (filename_new == "")
		return;

	//clear old object references
	directional = 0;
	sndsource3d = 0;
	sndsource = 0;
	sndstream = 0;

	//exit if sound is disabled
	if (sbs->DisableSound == true)
		return;

	//load new sound
	Filename = filename;
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
	sndwrapper->SetData(snddata);

	sndstream = sbs->sndrenderer->CreateStream(sndwrapper->GetData(), CS_SND3D_ABSOLUTE);
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
	sndsource3d = scfQueryInterface<iSndSysSource3D> (sndsource);
	if (!sndsource3d)
	{
		sbs->ReportError("Can't create 3D source for '%s'", filename);
		return;
	}
	directional = scfQueryInterface<iSndSysSource3DDirectionalSimple> (sndsource);

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

float Sound::GetPlayPosition()
{
	//returns the current sound playback position, in percent (1 = 100%)

	if (sndstream)
		return sndstream->GetPosition() / sndstream->GetFrameCount();
	else
		return 0;
}

void Sound::SetPlayPosition(float percent)
{
	//sets the current sound playback position, in percent (1 = 100%)

	if (sndstream)
		sndstream->SetPosition(percent * sndstream->GetFrameCount());
}
