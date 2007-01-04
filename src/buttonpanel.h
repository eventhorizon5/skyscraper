/* $Id$ */

/*
	Scalable Building Simulator - Elevator Button Panel Class
	The Skyscraper Project - Version 1.1 Alpha
	Copyright (C)2005-2007 Ryan Thoryk
	http://www.tliquest.net/skyscraper
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

#include "globals.h"

class ButtonPanel
{
public:

	int elevator;
	csString Direction;
	csVector3 Origin;
	double Width;
	double Height;
	double Spacing;
	int Rows;
	int Columns;
	csVector2 GridSize;

	//functions
	ButtonPanel(int _elevator, const char *texture, int rows, int columns, const char *direction, double CenterX, double CenterZ, double width, double height, double voffset, double spacing, double tw, double th);
	~ButtonPanel();
	void AddFloorButton(const char *texture, int row, int column, int floor);
	void AddControlButton(const char *texture, int row, int column, const char *type);
	void DeleteButton(int row, int column);
	void Press(int index);
	void Move(csVector3 position);
	void SetToElevatorAltitude();

private:
	csRef<iMeshWrapper> ButtonPanelMesh; //button panel mesh object
	csRef<iThingFactoryState> ButtonPanel_state;

	void AddButton(const char *name, const char *texture, int row, int column);
};

#endif
