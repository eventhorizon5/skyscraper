/* $Id$ */

/*
	Scalable Building Simulator - Elevator Button Panel Class
	The Skyscraper Project - Version 1.1 Alpha
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

#ifndef _SBS_BUTTON_H
#define _SBS_BUTTON_H

class SBSIMPEXP ButtonPanel
{
public:

	int elevator;
	csString Direction;
	csVector3 Origin;
	float Width;
	float Height;
	float ButtonWidth;
	float ButtonHeight;
	float SpacingX;
	float SpacingY;
	int Rows;
	int Columns;
	csVector2 GridSize;

	//functions
	ButtonPanel(int _elevator, int index, const char *texture, int rows, int columns, const char *direction, float CenterX, float CenterZ, float buttonwidth, float buttonheight, float spacingX, float spacingY, float voffset, float tw, float th);
	~ButtonPanel();
	void AddFloorButton(const char *texture, int row, int column, int floor, float width, float height);
	void AddControlButton(const char *texture, int row, int column, const char *type, float width, float height);
	void DeleteButton(int row, int column);
	void Press(int index);
	void Move(csVector3 position);
	void SetToElevatorAltitude();
	void Enabled(bool value);
	int AddWall(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th);

private:
	csRef<iMeshWrapper> ButtonPanelMesh; //button panel mesh object
	csRef<iThingFactoryState> ButtonPanel_state;

	void AddButton(const char *name, const char *texture, int row, int column, float bwidth, float bheight);
};

#endif
