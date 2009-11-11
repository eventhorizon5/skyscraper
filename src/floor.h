/* $Id$ */

/*
	Scalable Building Simulator - Floor Class
	The Skyscraper Project - Version 1.6 Alpha
	Copyright (C)2005-2009 Ryan Thoryk
	http://www.skyscrapersim.com
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

#include "callbutton.h"
#include "door.h"
#include "floorindicator.h"

class SBSIMPEXP Floor
{
public:
	csRef<iMeshWrapper> Level; //level mesh
	csRef<iThingFactoryState> Level_state;

	csRef<iMeshWrapper> Interfloor; //interfloor mesh
	csRef<iThingFactoryState> Interfloor_state;

	csRef<iMeshWrapper> ColumnFrame; //columnframe mesh
	csRef<iThingFactoryState> ColumnFrame_state;

	int Number; //floor number
	csString Name; //floor name
	csString ID;
	csString FloorType;
	csString Description;
	csString IndicatorTexture; //elevator indicator texture name
	float Altitude; //floor altitude
	float Height; //floor height (not including interfloor height)
	float InterfloorHeight; //height of interfloor area
	bool IsEnabled; //is floor enabled?
	bool IsColumnFrameEnabled; //is columnframe enabled?
	csArray<CallButton*> CallButtonArray; //pointer array to call button objects
	csArray<Door*> DoorArray; //pointer array to door objects
	csArray<FloorIndicator*> FloorIndicatorArray; //pointer array to floor indicator objects
	csArray<int> Group; //floor group

	//functions
	Floor(int number);
	~Floor();
	void SetCameraFloor();
	int AddFloor(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, float tw, float th, bool isexternal);
	void DeleteFloor(int index);
	int AddInterfloorFloor(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, float tw, float th);
	void DeleteInterfloorFloor(int index);
	int AddWall(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float voffset1, float voffset2, float tw, float th, bool isexternal);
	void DeleteWall(int index);
	int AddInterfloorWall(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float voffset1, float voffset2, float tw, float th);
	void DeleteInterfloorWall(int index);
	void Enabled(bool value);
	float FullHeight();
	void AddCallButtons(csArray<int> &elevators, const char *BackTexture, const char *UpButtonTexture, const char *UpButtonTexture_Lit, const char *DownButtonTexture, const char *DownButtonTexture_Lit, float CenterX, float CenterZ, float voffset, const char *direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th);
	void Cut(const csVector3 &start, const csVector3 &end, bool cutwalls, bool cutfloors, bool fast, int checkwallnumber = 0, const char *checkstring = "");
	void AddGroupFloor(int number);
	void RemoveGroupFloor(int number);
	void EnableGroup(bool value);
	void AddDoor(const char *texture, float thickness, int direction, float CenterX, float CenterZ, float width, float height, float voffset, float tw, float th);
	float CalculateAltitude();
	void EnableColumnFrame(bool value);
	int ColumnWallBox(const char *name, const char *texture, float x1, float x2, float z1, float z2, float height_in, float voffset, float tw, float th, bool inside, bool outside, bool top, bool bottom);
	int ColumnWallBox2(const char *name, const char *texture, float CenterX, float CenterZ, float WidthX, float LengthZ, float height_in, float voffset, float tw, float th, bool inside, bool outside, bool top, bool bottom);
	bool AddFloorIndicator(int elevator, bool relative, const char *texture_prefix, const char *direction, float CenterX, float CenterZ, float width, float height, float voffset);
	void UpdateFloorIndicators();
	void Loop();
	csArray<int> GetCallButtons(int elevator);
	void AddFillerWalls(const char *texture, float thickness, float CenterX, float CenterZ, float width, float height, float voffset, bool direction, float tw, float th);

private:
	char intbuffer[65];
	char buffer[20];
};

#endif
