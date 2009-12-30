/* $Id$ */

/*
	Scalable Building Simulator - Control Object Class
	The Skyscraper Project - Version 1.6 Alpha
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

#ifndef _SBS_CONTROL_H
#define _SBS_CONTROL_H

#include "globals.h"

class SBSIMPEXP Control
{
public:
	Object *object; //SBS object
	csString Name;
	csString ActionName;
	csString Type;
	csString Direction;
	csString TextureUnlit;
	csString TextureLit;
	bool LightStatus;
	bool IsEnabled;
	int positions; //number of knob positions; default is 3

	//functions
	Control(Object *parent, int type, const char *name, const char *action_name, const char *texture, const char *texture_lit, const char *direction, float x, float z, float width, float height, float voffset);
	~Control();
	void Enabled(bool value);
	void SetLight(bool value);
	void ChangeLight(bool value);
	csVector3 GetPosition();
	void SetPosition(const csVector3 &position);
	void SetPositionY(float position);
	void Move(const csVector3 &position);
	void SetKnobPosition(int position);

private:
	csRef<iMeshWrapper> ControlMesh; //control mesh object
	int current_position; //current knob position

};

#endif
