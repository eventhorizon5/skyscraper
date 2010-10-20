/* $Id$ */

/*
	Scalable Building Simulator - Stairs Subsystem Class
	The Skyscraper Project - Version 1.8 Alpha
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

#ifndef _SBS_STAIRS_H
#define _SBS_STAIRS_H

#include "door.h"

class SBSIMPEXP Stairs
{
public:

	Object *object; //SBS object
	int StairsNum; //number
	int startfloor; //starting floor
	int endfloor; //ending floor
	csVector3 origin; //origin vector
	csVector2 cutstart; //cut starting vector
	csVector2 cutend; //cut ending vector
	bool InsideStairwell; //true if user/camera is in the stairwell
	bool IsEnabled; //true if the entire stairwell has been enabled

	Stairs(int number, float CenterX, float CenterZ, int _startfloor, int _endfloor);
	~Stairs();
	WallObject* AddStairs(int floor, const char *name, const char *texture, const char *direction, float CenterX, float CenterZ, float width, float risersize, float treadsize, int num_stairs, float voffset, float tw, float th);
	WallObject* AddWall(int floor, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th);
	int AddWall(WallObject *wall, int floor, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th);
	WallObject* AddFloor(int floor, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, float tw, float th);
	int AddFloor(WallObject *wall, int floor, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, float tw, float th);
	void Enabled(int floor, bool value);
	void EnableWholeStairwell(bool value);
	bool IsInStairwell(const csVector3 &position);
	Object* AddDoor(int floor, const char *open_sound, const char *close_sound, bool open_state, const char *texture, float thickness, int direction, float speed, float CenterX, float CenterZ, float width, float height, float voffset, float tw, float th);
	void CutFloors(bool relative, const csVector2 &start, const csVector2 &end, float startvoffset, float endvoffset);
	bool CutWall(bool relative, int floor, const csVector3 &start, const csVector3 &end, int checkwallnumber = 0, const char *checkstring = "");
	void EnableRange(int floor, int range);
	void EnableDoor(int floor, bool value);
	void OpenDoor(int number);
	void CloseDoor(int number);
	bool IsDoorOpen(int number);
	bool IsEnabledFloor(int floor);
	bool IsValidFloor(int floor);
	bool IsDoorMoving(int number);
	void Report(const char *message);
	bool ReportError(const char *message);
	void RemoveDoor(Door *door);
	Object* AddLight(int floor, const char *name, int type, csVector3 position, csVector3 direction, float radius, float max_distance, float color_r, float color_g, float color_b, float spec_color_r, float spec_color_g, float spec_color_b, float directional_cutoff_radius, float spot_falloff_inner, float spot_falloff_outer);
	MeshObject* GetMeshObject(int floor);
	Object* AddModel(int floor, const char *name, const char *filename, csVector3 position, csVector3 rotation, float max_render_distance = 0, float scale_multiplier = 1);

private:
	csArray<MeshObject*> StairArray; //stairwell array
	csArray<MeshObject*> StairDoorArray; //stair door array
	csArray<bool> EnableArray;

	struct DoorMap
	{
		int floornumber; //associated floor number
		Door *object; //door object reference
	};

	csArray<DoorMap> DoorArray; //door object array

	char intbuffer[65];
	char buffer[20];

	int lastfloor;
	bool lastfloorset;

	//lights
	csArray<csArray<Light*> > lights;

	//Models
	csArray<csArray<Model*> > ModelArray;

	//cache objects for IsInStairwell()
	csVector3 lastposition;
	bool lastcheckresult;
	bool checkfirstrun;
};

#endif
