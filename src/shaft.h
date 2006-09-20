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

	int ShaftType; //type of shaft; 1=elevator shaft, 2=pipe/utility shaft
	csVector2 location1; //location vertex 1; is x1 and z1
	csVector2 location2; //location vertex 2; is x2 and z2
	double startaltitude; //starting altitude
	double endaltitude; //ending altitude

	Shaft(int type, double x1, double x2, double z1, double z2, double startalt, double endalt);
	~Shaft();
	int AddWall(const char *texture, double x1, double z1, double x2, double z2, double height1, double height2, double altitude1, double altitude2, double tw, double th, bool revX, bool revY, bool revZ, bool DrawBothSides);
	int AddFloor(const char *texture, double x1, double z1, double x2, double z2, double altitude1, double altitude2, double tw, double th);

private:
	csRef<iMeshWrapper> ShaftMesh; //shaft mesh object
	csRef<iMeshObject> Shaft_object;
	csRef<iMeshObjectFactory> Shaft_factory;
	csRef<iThingFactoryState> Shaft_state;
};

#endif
