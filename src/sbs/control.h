/*
	Scalable Building Simulator - Control Object
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

#ifndef _SBS_CONTROL_H
#define _SBS_CONTROL_H

#include "lock.h"

namespace SBS {

class SBSIMPEXP Control : public Object, public Lock
{
public:
	std::string Direction;

	//functions
	Control(Object *parent, const std::string &name, bool permanent, const std::string &sound, const std::vector<std::string> &action_names, const std::vector<Action*> &actions, std::vector<std::string> &textures, const std::string &direction, Real width, Real height, bool center, int selection_position);
	~Control();
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
	void SetTexture(int position, const std::string &texture);
	std::string GetTexture(int position);
	int FindActionPosition(const std::string &name);
	int FindNumericActionPosition();
	bool DoAction();
	bool Press(bool reverse = false);
	void ChangeFloorLight(int floor, bool value);
	void ChangeLight(bool value);
	void RemoveAction(Action *action);
	bool IsEnabled() { return is_enabled; }
	bool GetLightStatus() { return light_status; }
	void OnClick(Vector3 &position, bool shift, bool ctrl, bool alt, bool right);
	void OnUnclick(bool right);
	void Report(const std::string &message);
	bool ReportError(const std::string &message);

private:
	MeshObject* ControlMesh; //control mesh object
	int current_position; //current control position
	std::vector<std::string> TextureArray; //selection texture array
	std::vector<std::string> ActionNames; //control actions (uses name-based searches, slower and for dynamic controls)
	std::vector<Action*> Actions; //control actions (faster and for static controls, uses pointer storage)

	Sound *sound; //sound object
	bool light_status; //light on/off value, used for floor buttons
	bool is_enabled;
	bool action_hold; //storage for current position action's "hold" value
	bool action_result; //value of last DoAction result
};

}

#endif
