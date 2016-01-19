/* $Id$ */

/*
	Scalable Building Simulator - Stairwell Object
	The Skyscraper Project - Version 1.10 Alpha
	Copyright (C)2004-2016 Ryan Thoryk
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

namespace SBS {

class SBSIMPEXP Stairs : public Object
{
public:

	int StairsNum; //number
	int startfloor; //starting floor
	int endfloor; //ending floor
	Ogre::Vector2 cutstart; //cut starting vector
	Ogre::Vector2 cutend; //cut ending vector
	bool InsideStairwell; //true if user/camera is in the stairwell
	bool IsEnabled; //true if the entire stairwell has been enabled
	bool ShowFloors; //true if floors should be shown while inside the stairwell; floor list in ShowFloorsList
	bool ShowFullStairs; //if true, always show full stairwell while inside instead of only a selected range

	Stairs(Object *parent, int number, float CenterX, float CenterZ, int startfloor, int endfloor);
	~Stairs();
	WallObject* AddStairs(int floor, const std::string &name, const std::string &texture, const std::string &direction, float CenterX, float CenterZ, float width, float risersize, float treadsize, int num_stairs, float voffset, float tw, float th);
	WallObject* AddWall(int floor, const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th);
	bool AddWall(WallObject *wall, int floor, const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th);
	WallObject* AddFloor(int floor, const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, bool reverse_axis, bool texture_direction, float tw, float th, bool legacy_behavior = false);
	bool AddFloor(WallObject *wall, int floor, const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, bool reverse_axis, bool texture_direction, float tw, float th, bool legacy_behavior = false);
	void Enabled(int floor, bool value);
	void EnableWholeStairwell(bool value, bool force = false);
	bool IsInStairwell(const Ogre::Vector3 &position);
	Door* AddDoor(int floor, const std::string &open_sound, const std::string &close_sound, bool open_state, const std::string &texture, float thickness, int direction, float speed, float CenterX, float CenterZ, float width, float height, float voffset, float tw, float th);
	void CutFloors(bool relative, const Ogre::Vector2 &start, const Ogre::Vector2 &end, float startvoffset, float endvoffset);
	bool Cut(bool relative, int floor, const Ogre::Vector3 &start, const Ogre::Vector3 &end, bool cutwalls, bool cutfloors, int checkwallnumber = 0);
	void EnableRange(int floor, int range, bool value);
	bool IsEnabledFloor(int floor);
	bool IsValidFloor(int floor);
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	void RemoveDoor(Door *door);
	void RemoveLight(Light *light);
	void RemoveModel(Model *model);
	void RemoveControl(Control *control);
	void RemoveTrigger(Trigger *trigger);
	Light* AddLight(int floor, const std::string &name, int type, Ogre::Vector3 position, Ogre::Vector3 direction, float color_r, float color_g, float color_b, float spec_color_r, float spec_color_g, float spec_color_b, float spot_inner_angle, float spot_outer_angle, float spot_falloff, float att_range, float att_constant, float att_linear, float att_quadratic);
	MeshObject* GetMeshObject(int floor);
	Model* AddModel(int floor, const std::string &name, const std::string &filename, bool center, Ogre::Vector3 position, Ogre::Vector3 rotation, float max_render_distance = 0, float scale_multiplier = 1, bool enable_physics = false, float restitution = 0, float friction = 0, float mass = 0);
	void AddModel(int floor, Model *model);
	Control* AddControl(int floor, const std::string &name, const std::string &sound, const std::string &direction, float CenterX, float CenterZ, float width, float height, float voffset, std::vector<std::string> &action_names, std::vector<std::string> &textures);
	Trigger* AddTrigger(int floor, const std::string &name, const std::string &sound_file, Ogre::Vector3 &area_min, Ogre::Vector3 &area_max, std::vector<std::string> &action_names);
	void ReplaceTexture(const std::string &oldtexture, const std::string &newtexture);
	void OnInit();
	void AddShowFloor(int floor);
	void RemoveShowFloor(int floor);
	bool IsShowFloor(int floor);
	void Check(Ogre::Vector3 position, int current_floor, int previous_floor);
	void Loop();
	Model* GetModel(int floor, std::string name);

private:
	std::vector<MeshObject*> StairArray; //stairwell array
	std::vector<bool> EnableArray;
	std::vector<int> ShowFloorsList; //list of floors to enable while inside the stairwell

	int lastfloor;
	bool lastfloorset;

	//Doors
	std::vector<std::vector<Door*> > DoorArray;

	//Lights
	std::vector<std::vector<Light*> > lights;

	//Models
	std::vector<std::vector<Model*> > ModelArray;

	//Controls
	std::vector<std::vector<Control*> > ControlArray;

	//Triggers
	//std::vector<std::vector<Trigger*> > TriggerArray;

	//cache objects for IsInStairwell()
	Ogre::Vector3 lastposition;
	bool lastcheckresult;
	bool checkfirstrun;
};

}

#endif
