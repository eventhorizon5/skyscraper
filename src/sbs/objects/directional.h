/*
	Scalable Building Simulator - Directional Indicator Object
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

#ifndef _SBS_DIRECTIONAL_H
#define _SBS_DIRECTIONAL_H

namespace SBS {

class SBSIMPEXP DirectionalIndicator : public Object
{
public:

	int elevator; //elevator this indicator is assigned to
	int car; //elevator car this indicator is assigned to
	int floor; //floor the indicator is on
	int blink; //number of times to turn on/off indicator
	std::string Direction; //direction the indicator faces; either 'front', 'back', 'left', or 'right'
	std::string UpTextureUnlit; //unlit up texture
	std::string UpTextureLit; //lit up texture
	std::string DownTextureUnlit; //unlit down texture
	std::string DownTextureLit; //lit down texture
	bool UpStatus; //status of up light
	bool DownStatus; //status of down light
	bool Single; //is this a single-light indicator?
	bool Vertical; //true if the lights are vertically separated or false if horizontally separated
	bool ActiveDirection; //true if this indicator displays the active elevator direction, instead of only for an available call

	//functions
	DirectionalIndicator(Object *parent, int elevator, int car, int floor, bool active_direction, bool single, bool vertical, const std::string &BackTexture, const std::string &uptexture, const std::string &uptexture_lit, const std::string &downtexture, const std::string &downtexture_lit, Real CenterX, Real CenterZ, Real voffset, const std::string &direction, Real BackWidth, Real BackHeight, bool ShowBack, Real tw, Real th);
	~DirectionalIndicator();
	void Enabled(bool value);
	void UpLight(bool value);
	void DownLight(bool value);
	void SetLights(int up, int down);
	bool IsEnabled() { return is_enabled; }
	void Off();
	void Loop();

private:
	MeshObject* DirectionalMeshBack; //indicator mesh back object
	MeshObject* DirectionalMeshUp; //indicator mesh up object
	MeshObject* DirectionalMeshDown; //indicator mesh down object
	MeshObject* DirectionalMesh; //indicator mesh single object

	bool is_enabled; //true if indicator is currently enabled

	//timer object
	class Timer; //internal timer class
	Timer* timer; //shut-off timer
	Timer* blink_timer; //blink timer
	int timer_interval;
	int blink_interval;
	bool blink_active;
};

}

#endif
