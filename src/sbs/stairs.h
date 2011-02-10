/* $Id$ */

/*
	Scalable Building Simulator - Stairs Subsystem Class
	The Skyscraper Project - Version 1.8 Alpha
	Copyright (C)2004-2011 Ryan Thoryk
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
	Ogre::Vector3 origin; //origin vector
	Ogre::Vector2 cutstart; //cut starting vector
	Ogre::Vector2 cutend; //cut ending vector
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
	bool IsInStairwell(const Ogre::Vector3 &position);
	Object* AddDoor(int floor, const char *open_sound, const char *close_sound, bool open_state, const char *texture, float thickness, int direction, float speed, float CenterX, float CenterZ, float width, float height, float voffset, float tw, float th);
	void CutFloors(bool relative, const Ogre::Vector2 &start, const Ogre::Vector2 &end, float startvoffset, float endvoffset);
	bool CutWall(bool relative, int floor, const Ogre::Vector3 &start, const Ogre::Vector3 &end, int checkwallnumber = 0, const char *checkstring = "");
	void EnableRange(int floor, int range);
	void EnableDoor(int floor, bool value);
	void OpenDoor(int number);
	void CloseDoor(int number);
	bool IsDoorOpen(int number);
	bool IsEnabledFloor(int floor);
	bool IsValidFloor(int floor);
	bool IsDoorMoving(int number);
	void Report(std::string message);
	bool ReportError(std::string message);
	void RemoveDoor(Door *door);
	Object* AddLight(int floor, const char *name, int type, Ogre::Vector3 position, Ogre::Vector3 direction, float color_r, float color_g, float color_b, float spec_color_r, float spec_color_g, float spec_color_b, float spot_inner_angle, float spot_outer_angle, float spot_falloff, float att_range, float att_constant, float att_linear, float att_quadratic);
	MeshObject* GetMeshObject(int floor);
	Object* AddModel(int floor, const char *name, const char *filename, Ogre::Vector3 position, Ogre::Vector3 rotation, float max_render_distance = 0, float scale_multiplier = 1, bool enable_physics = false, float restitution = 0, float friction = 0, float mass = 0);

private:
	std::vector<MeshObject*> StairArray; //stairwell array
	std::vector<MeshObject*> StairDoorArray; //stair door array
	std::vector<bool> EnableArray;

	struct DoorMap
	{
		int floornumber; //associated floor number
		Door *object; //door object reference
	};

	std::vector<DoorMap> DoorArray; //door object array

	char intbuffer[65];
	char buffer[20];

	int lastfloor;
	bool lastfloorset;

	//lights
	std::vector<std::vector<Light*> > lights;

	//Models
	std::vector<std::vector<Model*> > ModelArray;

	//cache objects for IsInStairwell()
	Ogre::Vector3 lastposition;
	bool lastcheckresult;
	bool checkfirstrun;
};

#endif
