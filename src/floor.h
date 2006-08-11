/*
	Scalable Building Simulator - Floor Class
	The Skyscraper Project - Version 1.1 Alpha
	Copyright ©2005-2006 Ryan Thoryk
	http://www.tliquest.net/skyscraper
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@tliquest.net

	$Id$
	
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

#ifndef _SBS_FLOOR_H
#define _SBS_FLOOR_H

#include "globals.h"

class Floor
{
public:

	csRef<iMeshWrapper> Level; //level mesh
		csRef<iMeshObject> Level_object;
		csRef<iMeshObjectFactory> Level_factory;
		csRef<iThingFactoryState> Level_state;

	csRef<iMeshWrapper> Interfloor; //interfloor mesh
		csRef<iMeshObject> Interfloor_object;
		csRef<iMeshObjectFactory> Interfloor_factory;
		csRef<iThingFactoryState> Interfloor_state;

	int Number; //floor number
	csString Name; //floor name
	csString ID;
	csString FloorType;
	csString Description;
	double Altitude; //floor altitude
	double Height; //floor height (not including interfloor height)
	double InterfloorHeight; //height of interfloor area
	double DoorHeight; //height of doors; may be removed
	double DoorWidth; //width of doors; may be removed
	bool IsEnabled; //is floor enabled?

	//functions
	Floor(int number);
	~Floor();
	void SetCameraFloor();
	int AddFloor(const char *texture, double x1, double z1, double x2, double z2, double voffset1, double voffset2, double tw, double th, bool isexternal);
	int AddInterfloorFloor(const char *texture, double x1, double z1, double x2, double z2, double voffset1, double voffset2, double tw, double th);
	int AddWall(const char *texture, double x1, double z1, double x2, double z2, double height_in1, double height_in2, double voffset1, double voffset2, double tw, double th, bool revX, bool revY, bool revZ, bool isexternal);
	int AddInterfloorWall(const char *texture, double x1, double z1, double x2, double z2, double height_in1, double height_in2, double voffset1, double voffset2, double tw, double th);
	void Enabled(bool value);
	void AddAutoFloor(const char *texture, double voffset, double tw, double th);
	double FullHeight();
	int AddCallButtons(bool up, bool down, int elevatornumber, bool direction, const char *backtexture, const char *buttontexture, double CenterX, double CenterZ, double width, double height, double tw, double th);

private:
	csRefArray<iMeshWrapper> Objects;
};

#endif
