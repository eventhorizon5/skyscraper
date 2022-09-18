/* $Id$ */

/*
	Scalable Building Simulator - Stairwell Object
	The Skyscraper Project - Version 1.11 Alpha
	Copyright (C)2004-2022 Ryan Thoryk
	http://www.skyscrapersim.com
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@skyscrapersim.com

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
	int ShowFullStairs; //if 1, always show full stairwell while inside instead of only a selected range, and always show whole stairwell if 2

	Stairs(Object *parent, int number, Real CenterX, Real CenterZ, int startfloor, int endfloor);
	~Stairs();
	Wall* AddStairs(int floor, const std::string &name, const std::string &riser_texture, const std::string &tread_texture, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real risersize, Real treadsize, int num_stairs, Real voffset, Real tw, Real th);
	Wall* AddWall(int floor, const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real height1, Real height2, Real voffset1, Real voffset2, Real tw, Real th);
	bool AddWall(Wall *wall, int floor, const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real height1, Real height2, Real voffset1, Real voffset2, Real tw, Real th);
	Wall* AddFloor(int floor, const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real voffset1, Real voffset2, bool reverse_axis, bool texture_direction, Real tw, Real th, bool legacy_behavior = false);
	bool AddFloor(Wall *wall, int floor, const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real voffset1, Real voffset2, bool reverse_axis, bool texture_direction, Real tw, Real th, bool legacy_behavior = false);
	void Enabled(int floor, bool value);
	void EnableWholeStairwell(bool value, bool force = false);
	bool IsInStairwell(const Ogre::Vector3 &position);
	Door* AddDoor(int floor, const std::string &open_sound, const std::string &close_sound, bool open_state, const std::string &texture, Real thickness, int direction, Real speed, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, Real tw, Real th);
	void CutFloors(bool relative, const Ogre::Vector2 &start, const Ogre::Vector2 &end, Real startvoffset, Real endvoffset);
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
	Light* AddLight(int floor, const std::string &name, int type, Ogre::Vector3 position, Ogre::Vector3 direction, Real color_r, Real color_g, Real color_b, Real spec_color_r, Real spec_color_g, Real spec_color_b, Real spot_inner_angle, Real spot_outer_angle, Real spot_falloff, Real att_range, Real att_constant, Real att_linear, Real att_quadratic);
	MeshObject* GetMeshObject(int floor);
	Model* AddModel(int floor, const std::string &name, const std::string &filename, bool center, Ogre::Vector3 position, Ogre::Vector3 rotation, Real max_render_distance = 0, Real scale_multiplier = 1, bool enable_physics = false, Real restitution = 0, Real friction = 0, Real mass = 0);
	void AddModel(int floor, Model *model);
	Control* AddControl(int floor, const std::string &name, const std::string &sound, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, int selection_position, std::vector<std::string> &action_names, std::vector<std::string> &textures);
	Trigger* AddTrigger(int floor, const std::string &name, const std::string &sound_file, Ogre::Vector3 &area_min, Ogre::Vector3 &area_max, std::vector<std::string> &action_names);
	void ReplaceTexture(const std::string &oldtexture, const std::string &newtexture);
	void OnInit();
	void AddShowFloor(int floor);
	void RemoveShowFloor(int floor);
	bool IsShowFloor(int floor);
	void Check(Ogre::Vector3 position, int current_floor, int previous_floor);
	void Loop();
	Model* GetModel(int floor, std::string name);
	void SetShowFull(int value);

private:
	std::vector<MeshObject*> StairArray; //stairwell array
	std::vector<bool> EnableArray;
	std::vector<int> ShowFloorsList; //list of floors to enable while inside the stairwell

	int lastfloor;
	bool lastfloorset;

	//Doors
	std::vector<std::vector<Door*> > DoorArray;
	DynamicMesh *DoorWrapper; //door dynamic mesh wrapper

	//Lights
	std::vector<std::vector<Light*> > lights;

	//Models
	std::vector<std::vector<Model*> > ModelArray;

	//Controls
	std::vector<std::vector<Control*> > ControlArray;

	//Triggers
	//std::vector<std::vector<Trigger*> > TriggerArray;

	//dynamic mesh object
	DynamicMesh *dynamic_mesh;

	//cache objects for IsInStairwell()
	Ogre::Vector3 lastposition;
	bool lastcheckresult;
	bool checkfirstrun;
};

}

#endif
