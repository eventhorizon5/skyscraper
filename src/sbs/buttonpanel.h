/* $Id$ */

/*
	Scalable Building Simulator - Elevator Button Panel Object
	The Skyscraper Project - Version 1.11 Alpha
	Copyright (C)2004-2016 Ryan Thoryk
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
	float Width;
	float Height;
	float ButtonWidth;
	float ButtonHeight;
	float SpacingX;
	float SpacingY;
	int Rows;
	int Columns;
	bool IsEnabled;

	//functions
	ButtonPanel(Object *parent, int index, const std::string &texture, int rows, int columns, const std::string &direction, float CenterX, float CenterZ, float buttonwidth, float buttonheight, float spacingX, float spacingY, float voffset, float tw, float th);
	~ButtonPanel();
	Control* AddButton(const std::string &sound, const std::string &texture, const std::string &texture_lit, int row, int column, const std::string &type, float width, float height, float hoffset = 0, float voffset = 0);
	Control* AddControl(const std::string &sound, int row, int column, float bwidth, float bheight, float hoffset, float voffset, int selection_position, std::vector<std::string> &action_names, std::vector<std::string> &textures);
	void Enabled(bool value);
	bool AddWall(const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th);
	void ChangeLight(int floor, bool value);
	void ChangeAllLights(bool value);
	Control* GetFloorButton(int floor);
	Control* GetControl(int index);
	void RemoveControl(Control *control);

private:

	MeshObject* mesh; //button panel mesh object
	Action* off_action;
	std::vector<Control*> controls; //control objects
	std::vector<Action*> action_list; //list used for action cleanup on delete
};

}

#endif
