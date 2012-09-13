/* $Id$ */

/*
	Scalable Building Simulator - Elevator Button Panel Class
	The Skyscraper Project - Version 1.8 Alpha
	Copyright (C)20042012 Ryan Thoryk
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

#ifndef _SBS_BUTTON_H
#define _SBS_BUTTON_H

#include "control.h"

class SBSIMPEXP ButtonPanel
{
public:

	Object *object; //SBS object
	int elevator;
	int Index; //index number of panel
	std::string Direction;
	Ogre::Vector3 Origin;
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
	ButtonPanel(int _elevator, int index, const char *texture, int rows, int columns, const char *direction, float CenterX, float CenterZ, float buttonwidth, float buttonheight, float spacingX, float spacingY, float voffset, float tw, float th);
	~ButtonPanel();
	void AddButton(const char *sound, const char *texture, const char *texture_lit, int row, int column, const char *type, float width, float height, float hoffset = 0, float voffset = 0);
	void AddControl(const char *sound, int row, int column, float bwidth, float bheight, float hoffset, float voffset, std::vector<std::string> &action_names, std::vector<std::string> &textures);
	void DeleteButton(int row, int column);
	void Press(int index);
	void Move(const Ogre::Vector3 &position);
	void SetToElevatorAltitude();
	void Enabled(bool value);
	int AddWall(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th);
	Control* GetControl(int index);
	void ChangeLight(int floor, bool value);
	int GetFloorButtonIndex(int floor);
	void RemoveControl(Control *control);

private:
	MeshObject* ButtonPanelMesh; //button panel mesh object

	std::vector<Control*> controls; //control objects

};

#endif
