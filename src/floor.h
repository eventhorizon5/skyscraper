/* $Id$ */

/*
	Scalable Building Simulator - Floor Class
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

#ifndef _SBS_FLOOR_H
#define _SBS_FLOOR_H

#include "globals.h"
#include "callbutton.h"

class Floor
{
public:

	csRef<iMeshWrapper> Level; //level mesh
	csRef<iThingFactoryState> Level_state;

	csRef<iMeshWrapper> Interfloor; //interfloor mesh
	csRef<iThingFactoryState> Interfloor_state;

	int Number; //floor number
	csString Name; //floor name
	csString ID;
	csString FloorType;
	csString Description;
	csString IndicatorTexture; //elevator indicator texture name
	double Altitude; //floor altitude
	double Height; //floor height (not including interfloor height)
	double InterfloorHeight; //height of interfloor area
	bool IsEnabled; //is floor enabled?
	csArray<CallButton*> CallButtonArray; //pointer array to call button objects
	csArray<int> floor_polys; //index refs to floor polygons
	csArray<int> ifloor_polys; //index refs to interfloor floor polygons
	csArray<int> wall_polys; //index refs to wall polygons
	csArray<int> iwall_polys; //index refs to interfloor wall polygons
	csArray<int> Group; //floor group

	//functions
	Floor(int number);
	~Floor();
	void SetCameraFloor();
	int AddFloor(const char *name, const char *texture, double thickness, double x1, double z1, double x2, double z2, double voffset1, double voffset2, double tw, double th, double tw_s, double th_s, bool isexternal);
	void DeleteFloor(int index);
	int AddInterfloorFloor(const char *name, const char *texture, double thickness, double x1, double z1, double x2, double z2, double voffset1, double voffset2, double tw, double th, double tw_s, double th_s);
	void DeleteInterfloorFloor(int index);
	int AddWall(const char *name, const char *texture, double thickness, double x1, double z1, double x2, double z2, double height_in1, double height_in2, double voffset1, double voffset2, double tw, double th, double tw_s, double th_s, bool revX, bool revY, bool revZ, bool isexternal);
	void DeleteWall(int index);
	int AddInterfloorWall(const char *name, const char *texture, double thickness, double x1, double z1, double x2, double z2, double height_in1, double height_in2, double voffset1, double voffset2, double tw_s, double th_s, double tw, double th);
	void DeleteInterfloorWall(int index);
	void Enabled(bool value);
	void AddAutoFloor(const char *name, const char *texture, double voffset, double tw, double th);
	double FullHeight();
	void AddCallButtons(csArray<int> &elevators, const char *BackTexture, const char *UpButtonTexture, const char *DownButtonTexture, double CenterX, double CenterZ, double voffset, const char *direction, double BackWidth, double BackHeight, bool ShowBack, double tw, double th);
	void CutFloor(double x1, double x2, double z1, double z2);
	void AddGroupFloor(int number);
	void RemoveGroupFloor(int number);
	void EnableGroup(bool value);

private:

};

#endif
