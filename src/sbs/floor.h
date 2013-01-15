/* $Id$ */

/*
	Scalable Building Simulator - Floor Class
	The Skyscraper Project - Version 1.8 Alpha
	Copyright (C)2004-2013 Ryan Thoryk
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
#include "control.h"
#include "trigger.h"

class SBSIMPEXP Floor
{
public:
	Object *object; //SBS object
	MeshObject *Level; //level mesh
	MeshObject *Interfloor; //interfloor mesh
	MeshObject *ColumnFrame; //columnframe mesh

	int Number; //floor number
	std::string Name; //floor name
	std::string ID;
	std::string FloorType;
	std::string Description;
	std::string IndicatorTexture; //elevator indicator texture name
	float Altitude; //floor altitude
	float Height; //floor height (not including interfloor height)
	float InterfloorHeight; //height of interfloor area
	bool IsEnabled; //is floor enabled?
	bool IsColumnFrameEnabled; //is columnframe enabled?
	std::vector<CallButton*> CallButtonArray; //pointer array to call button objects
	std::vector<FloorIndicator*> FloorIndicatorArray; //pointer array to floor indicator objects
	std::vector<int> Group; //floor group

	//functions
	Floor(int number);
	~Floor();
	void SetCameraFloor();
	WallObject* AddFloor(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, bool direction, float tw, float th, bool isexternal, bool legacy_behavior = false);
	WallObject* AddInterfloorFloor(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, bool direction, float tw, float th, bool legacy_behavior = false);
	WallObject* AddWall(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float voffset1, float voffset2, float tw, float th, bool isexternal);
	WallObject* AddInterfloorWall(const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float voffset1, float voffset2, float tw, float th);
	void Enabled(bool value);
	float FullHeight();
	Object* AddCallButtons(std::vector<int> &elevators, const char *BackTexture, const char *UpButtonTexture, const char *UpButtonTexture_Lit, const char *DownButtonTexture, const char *DownButtonTexture_Lit, float CenterX, float CenterZ, float voffset, const char *direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th);
	void Cut(const Ogre::Vector3 &start, const Ogre::Vector3 &end, bool cutwalls, bool cutfloors, bool fast, int checkwallnumber = 0, const char *checkstring = "", bool prepare = false);
	void CutAll(const Ogre::Vector3 &start, const Ogre::Vector3 &end, bool cutwalls, bool cutfloors, bool prepare = false);
	void AddGroupFloor(int number);
	void RemoveGroupFloor(int number);
	void EnableGroup(bool value);
	bool IsInGroup(int floor);
	Object* AddDoor(const char *open_sound, const char *close_sound, bool open_state, const char *texture, float thickness, int direction, float speed, float CenterX, float CenterZ, float width, float height, float voffset, float tw, float th);
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
	std::vector<int> GetCallButtons(int elevator);
	void AddFillerWalls(const char *texture, float thickness, float CenterX, float CenterZ, float width, float height, float voffset, bool direction, float tw, float th);
	Object* AddSound(const char *name, const char *filename, Ogre::Vector3 position, bool loop = true, float volume = 1.0, int speed = 100, float min_distance = 1.0, float max_distance = -1.0, float doppler_level = 0.0, float cone_inside_angle = 360, float cone_outside_angle = 360, float cone_outside_volume = 1.0, Ogre::Vector3 direction = Ogre::Vector3(0, 0, 0));
	void Report(std::string message);
	bool ReportError(std::string message);
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
	void RemoveLight(Light *light);
	void RemoveModel(Model *model);
	void RemoveControl(Control *control);
	void RemoveTrigger(Trigger *trigger);
	Object* AddLight(const char *name, int type, Ogre::Vector3 position, Ogre::Vector3 direction, float color_r, float color_g, float color_b, float spec_color_r, float spec_color_g, float spec_color_b, float spot_inner_angle, float spot_outer_angle, float spot_falloff, float att_range, float att_constant, float att_linear, float att_quadratic);
	Object* AddModel(const char *name, const char *filename, bool center, Ogre::Vector3 position, Ogre::Vector3 rotation, float max_render_distance = 0, float scale_multiplier = 1, bool enable_physics = false, float restitution = 0, float friction = 0, float mass = 0);
	void ReplaceTexture(const std::string &oldtexture, const std::string &newtexture);
	Object* AddControl(const char *name, const char *sound, const char *direction, float CenterX, float CenterZ, float width, float height, float voffset, std::vector<std::string> &action_names, std::vector<std::string> &textures);
	Object* AddTrigger(const char *name, const char *sound_file, Ogre::Vector3 &area_min, Ogre::Vector3 &area_max, std::vector<std::string> &action_names);
	std::vector<Sound*> GetSound(const char *name);

private:
	char intbuffer[65];
	char buffer[20];

	//sound objects
	std::vector<Sound*> sounds;

	//doors
	std::vector<Door*> DoorArray; //pointer array to door objects

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
};

#endif
