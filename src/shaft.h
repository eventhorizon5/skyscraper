/* $Id$ */

/*
	Scalable Building Simulator - Shaft Subsystem Class
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

#ifndef _SBS_SHAFT_H
#define _SBS_SHAFT_H

#include "globals.h"

class Shaft
{
public:

	int ShaftType; //type of shaft
	int ShaftNumber; //shaft number
	csVector3 origin; //origin vector
	int startfloor; //starting floor
	int endfloor; //ending floor
	csArray<int> elevators; //if elevator shaft, this is the list of elevators that use this shaft
	csArray<int> stairs; //if stairwell, this lists the stairs that use this shaft
	bool InsideShaft; //true if user/camera is in the shaft
	double bottom; //shaft base
	double top; //shaft top

	Shaft(int number, int type, double CenterX, double CenterZ, int _startfloor, int _endfloor);
	~Shaft();
	int AddWall(int floor, const char *name, const char *texture, double thickness, double x1, double z1, double x2, double z2, double height1, double height2, double voffset1, double voffset2, double tw, double th);
	int AddFloor(int floor, const char *name, const char *texture, double thickness, double x1, double z1, double x2, double z2, double voffset1, double voffset2, double tw, double th);
	void Enabled(int floor, bool value);
	bool IsShaft(csRef<iMeshWrapper> test);
	void EnableWholeShaft(bool value);
	bool IsInShaft(const csVector3 &position);

private:
	csRefArray<iMeshWrapper> ShaftArray; //shaft mesh array
	csRefArray<iThingFactoryState> ShaftArray_state; //shaft mesh array state
};

#endif
