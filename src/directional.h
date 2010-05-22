/* $Id$ */

/*
	Scalable Building Simulator - Directional Indicator Class
	The Skyscraper Project - Version 1.7 Alpha
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

#ifndef _SBS_DIRECTIONAL_H
#define _SBS_DIRECTIONAL_H

class SBSIMPEXP DirectionalIndicator
{
public:

	Object *object; //SBS object
	int elevator_num; //elevator this indicator is assigned to
	int floor_num; //floor the indicator is on
	csString Direction; //direction the indicator faces; either 'front', 'back', 'left', or 'right'
	bool IsEnabled; //true if indicator is currently enabled
	csString UpTextureUnlit; //unlit up texture
	csString UpTextureLit; //lit up texture
	csString DownTextureUnlit; //unlit down texture
	csString DownTextureLit; //lit down texture
	bool UpStatus; //status of up light
	bool DownStatus; //status of down light
	bool Single; //is this a single-light indicator?
	bool Vertical; //true if the lights are vertically separated or false if horizontally separated
	bool ActiveDirection; //true if this indicator displays the active elevator direction, instead of only for an available call

	//functions
	DirectionalIndicator(Object *parent, int elevator, int floor, bool active_direction, bool single, bool vertical, const char *BackTexture, const char *uptexture, const char *uptexture_lit, const char *downtexture, const char *downtexture_lit, float CenterX, float CenterZ, float voffset, const char *direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th);
	~DirectionalIndicator();
	void Enabled(bool value);
	void UpLight(bool value);
	void DownLight(bool value);
	void SetLights(int up, int down);
	void SetPosition(csVector3 position);
	void Move(csVector3 position);
	csVector3 GetPosition();

private:
	csRef<iMeshWrapper> DirectionalMeshBack; //indicator mesh back object
	csRef<iMeshWrapper> DirectionalMeshUp; //indicator mesh up object
	csRef<iMeshWrapper> DirectionalMeshDown; //indicator mesh down object
	csRef<iMeshWrapper> DirectionalMesh; //indicator mesh single object
	csRef<iThingFactoryState> Directional_back_state;
};

#endif
