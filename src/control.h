/* $Id$ */

/*
	Scalable Building Simulator - Control Object Class
	The Skyscraper Project - Version 1.8 Alpha
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

#ifndef _SBS_CONTROL_H
#define _SBS_CONTROL_H

#include "globals.h"

class SBSIMPEXP Control
{
public:
	Object *object; //SBS object
	csString Name;
	csString Direction;
	bool IsEnabled;

	//functions
	Control(Object *parent, const char *name, const char *sound, csArray<csString> &action_names, csArray<csString> &textures, const char *direction, float width, float height, float voffset);
	~Control();
	void Enabled(bool value);
	csVector3 GetPosition();
	void SetPosition(const csVector3 &position);
	void SetPositionY(float position);
	void Move(const csVector3 &position);
	bool SetSelectPosition(int position);
	bool ChangeSelectPosition(int position);
	bool NextSelectPosition(bool check_state = true);
	bool PreviousSelectPosition(bool check_state = true);
	int GetSelectPosition();
	const char* GetPositionAction(int position);
	const char* GetSelectPositionAction();
	int GetNextSelectPosition();
	int GetPreviousSelectPosition();
	int GetPositions();
	void PlaySound();
	void SetTexture(int position, const char *texture);
	int FindActionPosition(const char *name);

private:
	MeshObject* ControlMesh; //control mesh object
	int current_position; //current control position
	csArray<csString> TextureArray; //selection texture array
	csArray<csString> Actions; //button actions

	Sound *sound; //sound object

};

#endif
