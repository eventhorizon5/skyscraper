/*
	Scalable Building Simulator - Stairwell Object
	The Skyscraper Project - Version 1.12 Alpha
	Copyright (C)2004-2024 Ryan Thoryk
	https://www.skyscrapersim.net
	https://sourceforge.net/projects/skyscraper/
	Contact - ryan@skyscrapersim.net

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

class SBSIMPEXP Stairwell : public Object
{
public:
	class Level;

	int StairsNum; //number
	int startfloor; //starting floor
	int endfloor; //ending floor
	Vector2 cutstart; //cut starting vector
	Vector2 cutend; //cut ending vector
	bool Inside; //true if user/camera is in the stairwell
	bool IsEnabled; //true if the entire stairwell has been enabled
	bool ShowFloors; //true if floors should be shown while inside the stairwell; floor list in ShowFloorsList
	int ShowFullStairs; //if 1, always show full stairwell while inside instead of only a selected range, and always show whole stairwell if 2

	Stairwell(Object *parent, int number, Real CenterX, Real CenterZ, int startfloor, int endfloor);
	~Stairwell();
	void EnableWhole(bool value, bool force = false);
	bool IsInside(const Vector3 &position);
	void CutFloors(bool relative, const Vector2 &start, const Vector2 &end, Real startvoffset, Real endvoffset);
	void EnableRange(int floor, int range, bool value);
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	void ReplaceTexture(const std::string &oldtexture, const std::string &newtexture);
	void OnInit();
	void AddShowFloor(int floor);
	void RemoveShowFloor(int floor);
	bool IsShowFloor(int floor);
	void Check(Vector3 position, int current_floor, int previous_floor);
	void Loop();
	void SetShowFull(int value);
	Level* GetLevel(int floor);
	bool IsValidFloor(int floor);
	DynamicMesh* GetDynamicMesh();

	class SBSIMPEXP Level : public Object
	{
	public:
		Level(Stairwell *parent, int number);
		~Level();
		Wall* AddStairs(const std::string &name, const std::string &riser_texture, const std::string &tread_texture, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real risersize, Real treadsize, int num_stairs, Real voffset, Real tw, Real th);
		Wall* AddWall(const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real height1, Real height2, Real voffset1, Real voffset2, Real tw, Real th);
		bool AddWall(Wall *wall, const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real height1, Real height2, Real voffset1, Real voffset2, Real tw, Real th);
		Wall* AddFloor(const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real voffset1, Real voffset2, bool reverse_axis, bool texture_direction, Real tw, Real th, bool legacy_behavior = false);
		bool AddFloor(Wall *wall, const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real voffset1, Real voffset2, bool reverse_axis, bool texture_direction, Real tw, Real th, bool legacy_behavior = false);
		void Enabled(bool value);
		Door* AddDoor(std::string name, const std::string &open_sound, const std::string &close_sound, bool open_state, const std::string &texture, const std::string &side_texture, Real thickness, const std::string &face_direction, const std::string &open_direction, bool rotate, Real open_speed, Real close_speed, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, Real tw, Real th, Real side_tw, Real side_th);
		Door* CreateDoor(std::string name, const std::string &open_sound, const std::string &close_sound, bool rotate);
		Door* GetDoor(const std::string &name);
		bool Cut(bool relative, const Vector3 &start, const Vector3 &end, bool cutwalls, bool cutfloors, int checkwallnumber = 0);
		bool IsEnabled();
		void RemoveDoor(Door *door);
		void RemoveLight(Light *light);
		void RemoveModel(Model *model);
		void RemoveControl(Control *control);
		void RemoveTrigger(Trigger *trigger);
		Light* AddLight(const std::string &name, int type);
		Light* GetLight(const std::string &name);
		MeshObject* GetMeshObject();
		Model* AddModel(const std::string &name, const std::string &filename, bool center, Vector3 position, Vector3 rotation, Real max_render_distance = 0, Real scale_multiplier = 1, bool enable_physics = false, Real restitution = 0, Real friction = 0, Real mass = 0);
		void AddModel(Model *model);
		Control* AddControl(const std::string &name, const std::string &sound, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, int selection_position, std::vector<std::string> &action_names, std::vector<std::string> &textures);
		Trigger* AddTrigger(const std::string &name, const std::string &sound_file, Vector3 &area_min, Vector3 &area_max, std::vector<std::string> &action_names);
		Model* GetModel(std::string name);
		void ReplaceTexture(const std::string &oldtexture, const std::string &newtexture);
		int GetFloor();
		void Loop();
		CameraTexture* AddCameraTexture(const std::string &name, int quality, Real fov, const Vector3 &position, bool use_rotation, const Vector3 &rotation);

		bool enabled;

	private:
		MeshObject* mesh; //level mesh

		//Doors
		std::vector<Door*> DoorArray;

		//Lights
		std::vector<Light*> lights;

		//Models
		std::vector<Model*> ModelArray;

		//Controls
		std::vector<Control*> ControlArray;

		//Triggers
		//std::vector<Trigger*> TriggerArray

		//CameraTextures
		std::vector<CameraTexture*> CameraTextureArray;

		int floornum;
		Stairwell* parent;
	};

private:
	std::vector<Level*> Levels;
	std::vector<int> ShowFloorsList; //list of floors to enable while inside the stairwell

	int lastfloor;
	bool lastfloorset;

	//Doors
	DynamicMesh *DoorWrapper; //door dynamic mesh wrapper

	//dynamic mesh object
	DynamicMesh *dynamic_mesh;

	//cache objects for IsInStairwell()
	Vector3 lastposition;
	bool lastcheckresult;
	bool checkfirstrun;
};

}

#endif
