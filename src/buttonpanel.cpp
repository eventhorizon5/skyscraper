/* $Id$ */

/*
	Scalable Building Simulator - Elevator Button Panel Class
	The Skyscraper Project - Version 1.1 Alpha
	Copyright (C)2005-2006 Ryan Thoryk
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

#include <crystalspace.h>
#include "buttonpanel.h"
#include "elevator.h"
#include "sbs.h"
#include "camera.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

ButtonPanel::ButtonPanel(int elevator, const char *texture, int rows, int columns, int direction, double CenterX, double CenterZ, double height, double voffset, double tw, double th)
{
	//Create an elevator button panel

	Elevator = elevator;
	Direction = direction;
	Origin.x = CenterX;
	Origin.z = CenterZ;
	Height = height;
	Voffset = voffset;

}

ButtonPanel::~ButtonPanel()
{

}

void ButtonPanel::AddFloorButton(const char *texture, int row, int column, int floor)
{

}

void ButtonPanel::AddControlButton(const char *texture, int row, int column, int type)
{

}

void ButtonPanel::DeleteButton(int row, int column)
{

}

void ButtonPanel::Press(csVector3 isect)
{
	//check clicked location against button polygons

}
