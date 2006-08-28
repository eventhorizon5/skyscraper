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

Shaft::Shaft(int type, double x1, double x2, double z1, double z2, int _startfloor, int _endfloor)
{
	//constructor
	//creates a shaft in the location specified by x1, x2, z1, and z2
	//and that spans the floor range specified by startfloor and endfloor
	//types are currently:
	//1 = elevator shaft
	//2 = pipe/utility shaft

	ShaftType = type;
	startfloor = _startfloor;
	endfloor = _endfloor;
	location1 = csVector2(x1, z1);
	location2 = csVector2(x2, z2);

}

Shaft::~Shaft()
{
	//destructor

}

void Shaft::OpenShaftDoors(int floor)
{
	//Simulates manually opening shaft doors
	//Slowly opens the shaft's elevator doors no matter where elevator is.
	//Cannot be used with OpenDoorsEmergency.
	//This is if the elevator is not lined up with the shaft doors,
	//and the user needs to open the shaft doors, usually while on top of elevator.

}
