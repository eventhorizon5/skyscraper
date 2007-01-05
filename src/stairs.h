/* $Id$ */

/*
	Scalable Building Simulator - Stairs Subsystem Class
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

#ifndef _SBS_STAIRS_H
#define _SBS_STAIRS_H

#include "globals.h"

class Stairs
{
public:

	int StairsNum; //number
	int startfloor; //starting floor
	int endfloor; //ending floor
	csVector3 origin; //origin vector

	Stairs(int number, float CenterX, float CenterZ, int _startfloor, int _endfloor);
	~Stairs();
	int AddStairs(int floor, const char *name, const char *texture, const char *direction, float CenterX, float CenterZ, float width, float risersize, float treadsize, int num_stairs, float voffset, float tw, float th);
	int AddWall(int floor, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th);
	int AddFloor(int floor, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, float tw, float th);
	void Enabled(int floor, bool value);
	void EnableWholeStairwell(bool value);
	bool IsInStairwell(const csVector3 &position);

private:
	csRefArray<iMeshWrapper> StairArray; //stairwell array
	csRefArray<iThingFactoryState> StairArray_state; //stairwell mesh array state
	csRefArray<iMeshWrapper> StairDoorArray; //stair door array
	csRefArray<iThingFactoryState> StairDoorArray_state; //stair mesh array state
};

#endif
