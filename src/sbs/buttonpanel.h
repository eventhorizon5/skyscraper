/* $Id$ */

/*
	Scalable Building Simulator - Elevator Button Panel Object
	The Skyscraper Project - Version 1.11 Alpha
	Copyright (C)2004-2018 Ryan Thoryk
	http://www.skyscrapersim.com
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@skyscrapersim.com

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

#ifndef _SBS_BUTTON_H
#define _SBS_BUTTON_H

namespace SBS {

class SBSIMPEXP ButtonPanel : public Object
{
public:

	int Index; //index number of panel
	std::string Direction;
	Real Width;
	Real Height;
	Real ButtonWidth;
	Real ButtonHeight;
	Real SpacingX;
	Real SpacingY;
	int Rows;
	int Columns;
	bool IsEnabled;

	//functions
	ButtonPanel(Object *parent, int index, const std::string &texture, int rows, int columns, const std::string &direction, Real CenterX, Real CenterZ, Real buttonwidth, Real buttonheight, Real spacingX, Real spacingY, Real voffset, Real tw, Real th, bool autosize = true);
	~ButtonPanel();
	Control* AddButton(const std::string &sound, const std::string &texture, const std::string &texture_lit, int row, int column, const std::string &type, Real width, Real height, Real hoffset = 0, Real voffset = 0);
	Control* AddControl(const std::string &sound, int row, int column, Real bwidth, Real bheight, Real hoffset, Real voffset, int selection_position, std::vector<std::string> &action_names, std::vector<std::string> &textures);
	void Enabled(bool value);
	bool AddWall(const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real height1, Real height2, Real voffset1, Real voffset2, Real tw, Real th, bool autosize);
	void ChangeLight(int floor, bool value);
	void ChangeAllLights(bool value);
	Control* GetFloorButton(int floor);
	Control* GetControl(int index);
	Control* GetControl(const std::string &name);
	void RemoveControl(Control *control);
	int GetControlCount();
	void SetControls(const std::string &action_name);

private:

	MeshObject* mesh; //button panel mesh object
	Action* off_action;
	std::vector<Control*> controls; //control objects
	std::vector<Action*> action_list; //list used for action cleanup on delete
};

}

#endif
