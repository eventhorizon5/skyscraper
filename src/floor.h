/* $Id$ */

/*
	Scalable Building Simulator - Floor Class
	The Skyscraper Project - Version 1.7 Alpha
	Copyright (C)2004-2010 Ryan Thoryk
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
#include "directional.h"
#include "sound.h"

class SBSIMPEXP Floor
{
public:
	Object *object; //SBS object
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
	csArray<FloorIndicator*> FloorIndicatorArray; //pointer array to floor indicator objects
	csArray<int> Group; //floor group
	csArray<WallObject*> level_walls;
	csArray<WallObject*> interfloor_walls;
	csArray<WallObject*> columnframe_walls;

	//functions
	Floor(int number);
	~Floor();
	void SetCameraFloor();
	WallObject* AddFloor(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, float tw, float th, bool isexternal);
	WallObject* AddInterfloorFloor(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, float tw, float th);
	WallObject* AddWall(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float voffset1, float voffset2, float tw, float th, bool isexternal);
	WallObject* AddInterfloorWall(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float voffset1, float voffset2, float tw, float th);
	void Enabled(bool value);
	float FullHeight();
	Object* AddCallButtons(csArray<int> &elevators, const char *BackTexture, const char *UpButtonTexture, const char *UpButtonTexture_Lit, const char *DownButtonTexture, const char *DownButtonTexture_Lit, float CenterX, float CenterZ, float voffset, const char *direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th);
	void Cut(const csVector3 &start, const csVector3 &end, bool cutwalls, bool cutfloors, bool fast, int checkwallnumber = 0, const char *checkstring = "");
	void CutAll(const csVector3 &start, const csVector3 &end, bool cutwalls, bool cutfloors);
	void AddGroupFloor(int number);
	void RemoveGroupFloor(int number);
	void EnableGroup(bool value);
	bool IsInGroup(int floor);
	Object* AddDoor(const char *open_sound, const char *close_sound, const char *texture, float thickness, int direction, float speed, float CenterX, float CenterZ, float width, float height, float voffset, float tw, float th);
	bool CalculateAltitude();
	void EnableColumnFrame(bool value);
	WallObject* ColumnWallBox(const char *name, const char *texture, float x1, float x2, float z1, float z2, float height_in, float voffset, float tw, float th, bool inside, bool outside, bool top, bool bottom);
	WallObject* ColumnWallBox2(const char *name, const char *texture, float CenterX, float CenterZ, float WidthX, float LengthZ, float height_in, float voffset, float tw, float th, bool inside, bool outside, bool top, bool bottom);
	Object* AddFloorIndicator(int elevator, bool relative, const char *texture_prefix, const char *direction, float CenterX, float CenterZ, float width, float height, float voffset);
	void UpdateFloorIndicators(int elevator);
	void UpdateFloorIndicators();
	void UpdateDirectionalIndicators(int elevator);
	void UpdateDirectionalIndicators();
	Object* AddDirectionalIndicator(int elevator, bool relative, bool active_direction, bool single, bool vertical, const char *BackTexture, const char *uptexture, const char *uptexture_lit, const char *downtexture, const char *downtexture_lit, float CenterX, float CenterZ, float voffset, const char *direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th);
	void SetDirectionalIndicators(int elevator, bool UpLight, bool DownLight);
	void Loop();
	csArray<int> GetCallButtons(int elevator);
	void AddFillerWalls(const char *texture, float thickness, float CenterX, float CenterZ, float width, float height, float voffset, bool direction, float tw, float th);
	Object* AddSound(const char *name, const char *filename, csVector3 position, int volume = 1.0, int speed = 100, float min_distance = 1.0, float max_distance = -1.0, float dir_radiation = 0, csVector3 direction = 0);
	void Report(const char *message);
	bool ReportError(const char *message);
	float GetBase(bool relative = false);
	void OpenDoor(int number);
	void CloseDoor(int number);
	bool IsDoorOpen(int number);
	bool IsDoorMoving(int number);
	void RemoveCallButton(CallButton *callbutton);
	void RemoveFloorIndicator(FloorIndicator *indicator);
	void RemoveDirectionalIndicator(DirectionalIndicator *indicator);
	void RemoveDoor(Door *door);
	void RemoveSound(Sound *sound);

private:
	char intbuffer[65];
	char buffer[20];

	//sound objects
	csArray<Sound*> sounds;

	//doors
	csArray<Door*> DoorArray; //pointer array to door objects

	//directional indicators
	csArray<DirectionalIndicator*> DirIndicatorArray;
};

#endif
