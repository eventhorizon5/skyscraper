/*
	Scalable Building Simulator - Proximity Trigger Object
	The Skyscraper Project - Version 2.1
	Copyright (C)2004-2024 Ryan Thoryk
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

#ifndef _SBS_TRIGGER_H
#define _SBS_TRIGGER_H

namespace SBS {

class SBSIMPEXP Trigger : public Object
{
public:

	//functions
	Trigger(Object *parent, const std::string &name, bool permanent, const std::string &sound_file, Vector3 area_min, Vector3 area_max, const std::vector<std::string> &action_names);
	~Trigger();
	void Enabled(bool value);
	bool SetSelectPosition(int position);
	bool ChangeSelectPosition(int position);
	bool NextSelectPosition(bool check_state = true);
	bool PreviousSelectPosition(bool check_state = true);
	int GetSelectPosition();
	std::string GetPositionAction(int position);
	std::string GetSelectPositionAction();
	int GetNextSelectPosition();
	int GetPreviousSelectPosition();
	int GetPositions();
	void PlaySound();
	int FindActionPosition(const std::string &name);
	bool DoAction();
	void Loop();
	bool IsInside();
	bool IsInside(const Vector3 &position);
	bool IsEnabled() { return is_enabled; }
	Ogre::AxisAlignedBox GetBounds(bool relative = false);
	Vector3 GetMin();
	Vector3 GetMax();
	bool IsOutside(Vector3 v1, Vector3 v2);
	virtual void OnEntry() {}
	virtual void OnExit() {}

private:
	Ogre::AxisAlignedBox *area_box;
	int current_position; //current trigger position
	bool is_inside;
	bool is_enabled;
	std::vector<std::string> Actions; //trigger actions

	Sound *sound; //sound object
};

}

#endif
