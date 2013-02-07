/* $Id$ */

/*
	Scalable Building Simulator - Control Object Class
	The Skyscraper Project - Version 1.8 Alpha
	Copyright (C)2004-2013 Ryan Thoryk
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
#include "action.h"

class SBSIMPEXP Control
{
public:
	Object *object; //SBS object
	std::string Name;
	std::string Direction;
	bool IsEnabled;

	//functions
	Control(Object *parent, const char *name, const char *sound, const std::vector<std::string> &action_names, std::vector<std::string> &textures, const char *direction, float width, float height, float voffset);
	~Control();
	void Enabled(bool value);
	Ogre::Vector3 GetPosition();
	void SetPosition(const Ogre::Vector3 &position);
	void SetPositionY(float position);
	void Move(const Ogre::Vector3 &position);
	void Move(const Ogre::Vector3 position, bool relative_x, bool relative_y, bool relative_z);
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
	int FindNumericActionPosition();
	bool DoAction();
	bool Press();
	void ChangeFloorLight(int floor, bool value);
	void ChangeLight(bool value);
	void SetLocked(bool value, int keyid);
	bool IsLocked();
	void ToggleLock(bool force = false);
	int GetKeyID();

private:
	MeshObject* ControlMesh; //control mesh object
	int current_position; //current control position
	std::vector<std::string> TextureArray; //selection texture array
	std::vector<std::string> Actions; //button actions

	Sound *sound; //sound object
	bool Locked;
	int KeyID;
	bool light_status; //light on/off value, used for floor buttons

};

#endif
