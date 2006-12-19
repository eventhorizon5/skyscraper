/* $Id$ */

/*
	Scalable Building Simulator - Stairs Subsystem Class
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
#include "stairs.h"
#include "sbs.h"
#include "camera.h"

extern SBS *sbs; //external pointer to the SBS engine

Stairs::Stairs(int number, int CenterX, int CenterZ, int _startfloor, int _endfloor)
{
	StairsNum = number;
	startfloor = _startfloor;
	endfloor = _endfloor;
	origin.x = CenterX;
	origin.z = CenterZ;
	origin.y = sbs->GetFloor(startfloor)->Altitude;
}

Stairs::~Stairs()
{

}

int Stairs::AddStairs(int floor, const char *name, const char *texture, const char *direction, double CenterX, double CenterZ, double width, double risersize, double treadsize, int num_stairs, double voffset, double tw, double th)
{
	return 0;
}

int Stairs::AddWall(int floor, const char *name, const char *texture, double x1, double z1, double x2, double z2, double height1, double height2, double voffset1, double voffset2, double tw, double th, bool revX, bool revY, bool revZ)
{
	return sbs->AddWallMain(StairDoorArray_state[floor - startfloor], name, texture, x1, z1, x2, z2, height1, height2, sbs->GetFloor(floor)->Altitude + voffset1, sbs->GetFloor(floor)->Altitude + voffset2, tw, th, revX, revY, revZ);
}

int Stairs::AddFloor(int floor, const char *name, const char *texture, double x1, double z1, double x2, double z2, double voffset1, double voffset2, double tw, double th)
{
	//get shaft extents
/*	double altitude = sbs->GetFloor(floor)->Altitude;

	if (altitude + voffset1 < bottom)
		bottom = altitude + voffset1;
	if (altitude + voffset2 < bottom)
		bottom = altitude + voffset2;
	if (altitude + voffset1 > top)
		top = altitude + voffset1;
	if (altitude + voffset2 > top)
		top = altitude + voffset2;

	return sbs->AddFloorMain(ShaftArray_state[floor - startfloor], name, texture, x1, z1, x2, z2, sbs->GetFloor(floor)->Altitude + voffset1, sbs->GetFloor(floor)->Altitude + voffset2, tw, th);
*/
	return 0;
}
