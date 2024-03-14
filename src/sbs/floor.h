/*
	Scalable Building Simulator - Floor Object
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

#ifndef _SBS_FLOOR_H
#define _SBS_FLOOR_H

namespace SBS {

class SBSIMPEXP Floor : public Object
{
public:
	MeshObject *Level; //level mesh
	MeshObject *Interfloor; //interfloor mesh
	MeshObject *ColumnFrame; //columnframe mesh

	int Number; //floor number
	std::string Name; //floor name
	std::string ID;
	std::string NumberID; //number ID of floor, for use with Destination Dispatch keypads
	std::string FloorType;
	std::string Description;
	std::string IndicatorTexture; //elevator indicator texture name
	Real Altitude; //floor altitude
	Real Height; //floor height (not including interfloor height)
	Real InterfloorHeight; //height of interfloor area
	bool IsEnabled; //is floor enabled?
	bool IsColumnFrameEnabled; //is columnframe enabled?
	bool IsInterfloorEnabled; //is interfloor enabled?
	std::vector<CallStation*> CallStationArray; //pointer array to call station objects
	std::vector<FloorIndicator*> FloorIndicatorArray; //pointer array to floor indicator objects
	std::vector<int> Group; //floor group
	bool EnabledGroup; //true if floor was enabled as part of a group, not directly
	int EnabledGroup_Floor; //number of floor that enabled this floor as part of it's own group
	bool AltitudeSet; //has altitude been set?

	//functions
	Floor(Object *parent, FloorManager *manager, int number);
	~Floor();
	Wall* AddFloor(const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real voffset1, Real voffset2, bool reverse_axis, bool texture_direction, Real tw, Real th, bool isexternal, bool legacy_behavior = false);
	Wall* AddInterfloorFloor(const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real voffset1, Real voffset2, bool reverse_axis, bool texture_direction, Real tw, Real th, bool legacy_behavior = false);
	Wall* AddWall(const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real height_in1, Real height_in2, Real voffset1, Real voffset2, Real tw, Real th, bool isexternal);
	Wall* AddInterfloorWall(const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real height_in1, Real height_in2, Real voffset1, Real voffset2, Real tw, Real th);
	void Enabled(bool value);
	Real FullHeight();
	CallStation* AddCallButtons(int controller, const std::string &sound_file_up, const std::string &sound_file_down, const std::string &BackTexture, const std::string &UpButtonTexture, const std::string &UpButtonTexture_Lit, const std::string &DownButtonTexture, const std::string &DownButtonTexture_Lit, Real CenterX, Real CenterZ, Real voffset, const std::string &direction, Real BackWidth, Real BackHeight, bool ShowBack, Real tw, Real th);
	CallStation* AddCallStation(int number);
	void Cut(const Vector3 &start, const Vector3 &end, bool cutwalls, bool cutfloors, bool fast, int checkwallnumber = 0, bool prepare = false);
	void CutAll(const Vector3 &start, const Vector3 &end, bool cutwalls, bool cutfloors, bool prepare = false);
	void AddGroupFloor(int number);
	void RemoveGroupFloor(int number);
	void EnableGroup(bool value);
	bool IsInGroup(int floor);
	Door* AddDoor(std::string name, const std::string &open_sound, const std::string &close_sound, bool open_state, const std::string &texture, const std::string &side_texture, Real thickness, const std::string &face_direction, const std::string &open_direction, bool rotate, Real open_speed, Real close_speed, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, Real tw, Real th, Real side_tw, Real side_th, bool external = false);
	Door* CreateDoor(std::string name, const std::string &open_sound, const std::string &close_sound, bool rotate);
	Door* GetDoor(const std::string &name);
	bool CalculateAltitude();
	void EnableColumnFrame(bool value);
	void EnableInterfloor(bool value);
	Wall* ColumnWallBox(const std::string &name, const std::string &texture, Real x1, Real x2, Real z1, Real z2, Real height_in, Real voffset, Real tw, Real th, bool inside, bool outside, bool top, bool bottom);
	Wall* ColumnWallBox2(const std::string &name, const std::string &texture, Real CenterX, Real CenterZ, Real WidthX, Real LengthZ, Real height_in, Real voffset, Real tw, Real th, bool inside, bool outside, bool top, bool bottom);
	FloorIndicator* AddFloorIndicator(int elevator, int car, bool relative, const std::string &texture_prefix, const std::string &blank_texture, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset);
	void UpdateFloorIndicators(int elevator);
	void UpdateFloorIndicators();
	void UpdateDirectionalIndicators(int elevator);
	void UpdateDirectionalIndicators();
	DirectionalIndicator* AddDirectionalIndicator(int elevator, int car, bool relative, bool active_direction, bool single, bool vertical, const std::string &BackTexture, const std::string &uptexture, const std::string &uptexture_lit, const std::string &downtexture, const std::string &downtexture_lit, Real CenterX, Real CenterZ, Real voffset, const std::string &direction, Real BackWidth, Real BackHeight, bool ShowBack, Real tw, Real th);
	void SetDirectionalIndicators(int elevator, int car, bool UpLight, bool DownLight);
	void Loop();
	std::vector<int> GetCallStations(int elevator);
	CallStation* GetCallStationForElevator(int elevator);
	CallStation* GetCallStation(int number);
	void AddFillerWalls(const std::string &texture, Real thickness, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, bool direction, Real tw, Real th, bool isexternal);
	Sound* AddSound(const std::string &name, const std::string &filename, Vector3 position, bool loop = true, Real volume = 1.0, int speed = 100, Real min_distance = 1.0, Real max_distance = -1.0, Real doppler_level = 0.0, Real cone_inside_angle = 360, Real cone_outside_angle = 360, Real cone_outside_volume = 1.0, Vector3 direction = Vector3(0, 0, 0));
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	Real GetBase(bool relative = false);
	Door* GetDoor(int number);
	void RemoveCallStation(CallStation* station);
	void RemoveFloorIndicator(FloorIndicator *indicator);
	void RemoveDirectionalIndicator(DirectionalIndicator *indicator);
	void RemoveDoor(Door *door);
	void RemoveSound(Sound *sound);
	void RemoveLight(Light *light);
	void RemoveModel(Model *model);
	void RemoveControl(Control *control);
	void RemoveTrigger(Trigger *trigger);
	void RemoveCameraTexture(CameraTexture *cameratexture);
	void RemoveEscalator(Escalator *escalator);
	void RemoveMovingWalkway(MovingWalkway *walkway);
	Light* AddLight(const std::string &name, int type);
	Light* GetLight(const std::string &name);
	Model* AddModel(const std::string &name, const std::string &filename, bool center, Vector3 position, Vector3 rotation, Real max_render_distance = 0, Real scale_multiplier = 1, bool enable_physics = false, Real restitution = 0, Real friction = 0, Real mass = 0);
	void AddModel(Model *model);
	void ReplaceTexture(const std::string &oldtexture, const std::string &newtexture);
	Control* AddControl(const std::string &name, const std::string &sound, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, int selection_position, std::vector<std::string> &action_names, std::vector<std::string> &textures);
	Trigger* AddTrigger(const std::string &name, const std::string &sound_file, Vector3 &area_min, Vector3 &area_max, std::vector<std::string> &action_names);
	CameraTexture* AddCameraTexture(const std::string &name, int quality, Real fov, const Vector3 &position, bool use_rotation, const Vector3 &rotation);
	Escalator* AddEscalator(const std::string &name, int run, Real speed, const std::string &sound_file, const std::string &riser_texture, const std::string &tread_texture, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real risersize, Real treadsize, int num_steps, Real voffset, Real tw, Real th);
	MovingWalkway* AddMovingWalkway(const std::string &name, int run, Real speed, const std::string &sound_file, const std::string &texture, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real treadsize, int num_steps, Real voffset, Real tw, Real th);
	std::vector<Sound*> GetSound(const std::string &name);
	void SetAltitude(Real altitude);
	void ShowInfo(bool detailed = true, bool display_header = true);
	void GetElevatorList(std::vector<int> &listing, bool get_locals = true, bool get_express = true, bool get_service = true);
	void GetStairwellList(std::vector<int> &listing);
	void GetShaftList(std::vector<int> &listing);
	ElevatorRoute* GetDirectRoute(int DestinationFloor, std::string ElevatorType);
	std::vector<int> GetDirectFloors(bool include_service);
	Model* GetModel(std::string name);
	RevolvingDoor* AddRevolvingDoor(std::string name, bool run, const std::string &soundfile, const std::string &texture, Real thickness, bool clockwise, int segments, Real speed, Real rotation, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, Real tw, Real th, bool external);
	void RemoveRevolvingDoor(RevolvingDoor *door);
	RevolvingDoor* GetRevolvingDoor(int number);
	CameraTexture* GetCameraTexture(int number);
	int GetCallStationCount();

private:
	//sound objects
	std::vector<Sound*> sounds;

	//doors
	std::vector<Door*> DoorArray; //pointer array to door objects
	std::vector<RevolvingDoor*> RDoorArray; //pointer array to revolving door objects
	DynamicMesh *DoorWrapper; //door dynamic mesh wrapper

	//lights
	std::vector<Light*> lights;

	//directional indicators
	std::vector<DirectionalIndicator*> DirIndicatorArray;

	//Models
	std::vector<Model*> ModelArray;

	//Controls
	std::vector<Control*> ControlArray;

	//Triggers
	std::vector<Trigger*> TriggerArray;

	//CameraTextures
	std::vector<CameraTexture*> CameraTextureArray;

	//Escalators
	std::vector<Escalator*> EscalatorArray;

	//Moving walkways
	std::vector<MovingWalkway*> MovingWalkwayArray;
};

}

#endif
