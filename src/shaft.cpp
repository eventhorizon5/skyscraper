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

//#include <wx/wx.h>
#include <crystalspace.h>
#include "shaft.h"
#include "sbs.h"
#include "camera.h"

extern SBS *sbs; //external pointer to the SBS engine
extern Camera *c; //external pointer to the camera

Shaft::Shaft(int type, double x1, double x2, double z1, double z2, double startalt, double endalt)
{
	//constructor
	//creates a shaft in the location specified by x1, x2, z1, and z2
	//and that spans the altitude range specified by startalt and endalt
	//types are currently:
	//1 = elevator shaft
	//2 = pipe/utility shaft

	ShaftType = type;
	startaltitude = startalt;
	endaltitude = endalt;
	location1 = csVector2(x1, z1);
	location2 = csVector2(x2, z2);

}

Shaft::~Shaft()
{
	//destructor

}

int Shaft::AddWall(const char *texture, double x1, double z1, double x2, double z2, double height1, double height2, double altitude1, double altitude2, double tw, double th, bool revX, bool revY, bool revZ, bool DrawBothSides)
{
	return sbs->AddWallMain(Shaft_state, texture, location1.x + x1, location1.y + z1, location2.x + x2, location2.y + z2, height1, height2, altitude1, altitude2, tw, th, revX, revY, revZ, DrawBothSides);
}

int Shaft::AddFloor(const char *texture, double x1, double z1, double x2, double z2, double altitude1, double altitude2, double tw, double th)
{
   	return sbs->AddFloorMain(Shaft_state, texture, location1.x + x1, location1.y + z1, location2.x + x2, location2.y + z2, altitude1, altitude2, tw, th);
}
