/* $Id$ */

/*
Scalable Building Simulator - Elevator Car Object
The Skyscraper Project - Version 1.11 Alpha
Copyright (C)2004-2016 Ryan Thoryk
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

#ifndef _SBS_ELEVATORCAR_H
#define _SBS_ELEVATORCAR_H

#include "elevatordoor.h"

namespace SBS {

class SBSIMPEXP ElevatorCar : public Object
{
	friend class Elevator;
public:

	int Number; //car number
	std::string Name; //car name
	std::vector<int> ServicedFloors; //list of floors this car services
	int NumDoors; //number of elevator doors
	float Height; //car height
	bool HeightSet;
	bool Fan; //fan enabled status
	Ogre::Vector3 MusicPosition; //music emitter position, relative of elevator center
	std::vector<int> DisplayFloors; //list of floors to only display when updating floor indicators
	bool ControlPressActive; //true if a control associated with this elevator has just been pressed
	bool IsEnabled; //true if car is enabled
	std::string UpStartSound; //car up start/speedup sound
	std::string DownStartSound; //car down start/speedup sound
	std::string UpMoveSound; //car up move sound
	std::string DownMoveSound; //car down move sound
	std::string UpStopSound; //car up stop/slowdown sound
	std::string DownStopSound; //car down stop/slowdown sound
	std::string IdleSound; //car idle/fan sound
	std::string AlarmSound; //alarm sound (loop)
	std::string AlarmSoundStop; //alarm stopping sound
	std::string BeepSound; //floor beep sound (played when changing floors)
	std::string FloorSound; //prefix of sounds played when reaching a certain floor; usually used for voices
	std::string UpMessageSound; //sound to play with message of elevator's direction if going up
	std::string DownMessageSound; //sound to play with message of elevator's direction if going down
	std::string OpenMessageSound; //sound to play with message of elevator's doors are opening
	std::string CloseMessageSound; //sound to play with message of elevator's doors are closing
	std::string Music; //car music sound to play
	std::string EmergencyStopSound; //car emergency stop sound
	bool AlarmActive; //true if alarm is active
	bool UseFloorBeeps; //true if floor beeps should be used
	bool UseFloorSounds; //true if floor sounds should be used
	bool UseDirMessageSounds; //true if direction message sounds should be used
	bool UseDoorMessageSounds; //true if door message sounds should be used
	bool MusicOn; //music enabled status
	bool MusicOnMove; //true if music should only play during move
	bool AutoEnable; //true if interior objects should automatically be enabled/disabled
	float CameraOffset; //camera vertical offset
	int StartingFloor; //car starting floor
	bool Created; //has car been created with the CreateCar function?
	int Offset; //floor number offset from the primary elevator
	bool GotoFloor; //true if car is heading to a floor

	ElevatorCar(Elevator *parent, int number);
	~ElevatorCar();
	bool CreateCar(int floor);
	Elevator* GetElevator();
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	Wall* AddWall(const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th);
	Wall* AddFloor(const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, bool reverse_axis, bool texture_direction, float tw, float th, bool legacy_behavior = false);
	FloorIndicator* AddFloorIndicator(const std::string &texture_prefix, const std::string &direction, float CenterX, float CenterZ, float width, float height, float voffset);
	ButtonPanel* CreateButtonPanel(const std::string &texture, int rows, int columns, const std::string &direction, float CenterX, float CenterZ, float width, float height, float voffset, float spacingX, float spacingY, float tw, float th);
	void DumpServicedFloors();
	bool AddServicedFloor(int number);
	void RemoveServicedFloor(int number);
	bool IsServicedFloor(int floor, bool report = true);
	int GetFloorIndex(int floor);
	int GetServicedFloorCount();
	int GetServicedFloor(int index);
	bool CheckServicedFloors();
	void Alarm();
	void OpenHatch();
	void OnInit();
	void Loop();
	void Enabled(bool value);
	void EnableObjects(bool value);
	void UpdateFloorIndicators();
	int GetTopFloor();
	int GetBottomFloor();
	void AddDirectionalIndicators(bool relative, bool active_direction, bool single, bool vertical, const std::string &BackTexture, const std::string &uptexture, const std::string &uptexture_lit, const std::string &downtexture, const std::string &downtexture_lit, float CenterX, float CenterZ, float voffset, const std::string &direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th);
	DirectionalIndicator* AddDirectionalIndicator(bool active_direction, bool single, bool vertical, const std::string &BackTexture, const std::string &uptexture, const std::string &uptexture_lit, const std::string &downtexture, const std::string &downtexture_lit, float CenterX, float CenterZ, float voffset, const std::string &direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th);
	void SetDirectionalIndicators(int floor, bool UpLight, bool DownLight);
	void UpdateDirectionalIndicators();
	void EnableDirectionalIndicators(bool value);
	ElevatorDoor* GetDoor(int number);
	bool OpenDoorsEmergency(int number = 0, int whichdoors = 1, int floor = 0, bool hold = false);
	void CloseDoorsEmergency(int number = 0, int whichdoors = 1, int floor = 0, bool hold = false);
	bool OpenDoors(int number = 0, int whichdoors = 1, int floor = 0, bool manual = false, bool hold = false);
	void CloseDoors(int number = 0, int whichdoors = 1, int floor = 0, bool manual = false, bool hold = false);
	void StopDoors(int number = 0);
	void HoldDoors(int number = 0, bool sensor = false);
	void ShaftDoorsEnabled(int number, int floor, bool value);
	void ShaftDoorsEnabledRange(int number, int floor, int range);
	bool AreDoorsOpen(int number = 0);
	bool AreShaftDoorsOpen(int number, int floor);
	bool AreShaftDoorsClosed(bool skip_current_floor = false);
	void ResetDoors(int number = 0, bool sensor = false);
	bool DoorsStopped(int number = 0);
	ElevatorDoor::DoorWrapper* AddDoors(int number, const std::string &lefttexture, const std::string &righttexture, float thickness, float CenterX, float CenterZ, float width, float height, bool direction, float tw, float th);
	bool AddShaftDoors(int number, const std::string &lefttexture, const std::string &righttexture, float thickness, float CenterX, float CenterZ, float voffset, float tw, float th);
	ElevatorDoor::DoorWrapper* AddShaftDoor(int floor, int number, const std::string &lefttexture, const std::string &righttexture, float tw, float th);
	ElevatorDoor::DoorWrapper* AddShaftDoor(int floor, int number, const std::string &lefttexture, const std::string &righttexture, float thickness, float CenterX, float CenterZ, float voffset, float tw, float th);
	void Chime(int number, int floor, bool direction);
	void EnableDoors(bool value);
	bool AddFloorSigns(int door_number, bool relative, const std::string &texture_prefix, const std::string &direction, float CenterX, float CenterZ, float width, float height, float voffset);
	int AreDoorsMoving(int number = 0, bool car_doors = true, bool shaft_doors = true);
	bool AreDoorsOpening(int number = 0, bool car_doors = true, bool shaft_doors = true);
	bool AreDoorsClosing(int number = 0, bool car_doors = true, bool shaft_doors = true);
	void SetShaftDoors(int number, float thickness, float CenterX, float CenterZ);
	bool DoorExists(int number);
	bool ShaftDoorsExist(int number, int floor);
	Sound* AddSound(const std::string &name, const std::string &filename, Ogre::Vector3 position, bool loop = true, float volume = 1.0, int speed = 100, float min_distance = 1.0, float max_distance = -1.0, float doppler_level = 0.0, float cone_inside_angle = 360, float cone_outside_angle = 360, float cone_outside_volume = 1.0, Ogre::Vector3 direction = Ogre::Vector3(0, 0, 0));
	ElevatorDoor::DoorWrapper* AddDoorComponent(int number, const std::string &name, const std::string &texture, const std::string &sidetexture, float thickness, const std::string &direction, float OpenSpeed, float CloseSpeed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th);
	ElevatorDoor::DoorWrapper* AddShaftDoorComponent(int number, int floor, const std::string &name, const std::string &texture, const std::string &sidetexture, float thickness, const std::string &direction, float OpenSpeed, float CloseSpeed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th);
	void AddShaftDoorsComponent(int number, const std::string &name, const std::string &texture, const std::string &sidetexture, float thickness, const std::string &direction, float OpenSpeed, float CloseSpeed, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th, float side_tw, float side_th);
	ElevatorDoor::DoorWrapper* FinishDoors(int number, bool DoorWalls = true, bool TrackWalls = true);
	ElevatorDoor::DoorWrapper* FinishShaftDoor(int number, int floor, bool DoorWalls = true, bool TrackWalls = true);
	bool FinishShaftDoors(int number, bool DoorWalls = true, bool TrackWalls = true);
	ButtonPanel* GetPanel(int index);
	Control* GetFloorButton(int floor);
	Door* AddDoor(const std::string &open_sound, const std::string &close_sound, bool open_state, const std::string &texture, float thickness, int direction, float speed, float CenterX, float CenterZ, float width, float height, float voffset, float tw, float th);
	Door* GetStdDoor(int number);
	void RemovePanel(ButtonPanel* panel);
	void RemoveDirectionalIndicator(DirectionalIndicator *indicator);
	void RemoveElevatorDoor(ElevatorDoor *door);
	void RemoveFloorIndicator(FloorIndicator *indicator);
	void RemoveDoor(Door *door);
	void RemoveSound(Sound *sound);
	void RemoveLight(Light *light);
	void RemoveModel(Model *model);
	void RemoveControl(Control *control);
	void RemoveTrigger(Trigger *trigger);
	bool IsNudgeModeActive(int number = 0);
	void EnableNudgeMode(bool value, int number = 0);
	Light* AddLight(const std::string &name, int type, Ogre::Vector3 position, Ogre::Vector3 direction, float color_r, float color_g, float color_b, float spec_color_r, float spec_color_g, float spec_color_b, float spot_inner_angle, float spot_outer_angle, float spot_falloff, float att_range, float att_constant, float att_linear, float att_quadratic);
	Model* AddModel(const std::string &name, const std::string &filename, bool center, Ogre::Vector3 position, Ogre::Vector3 rotation, float max_render_distance = 0, float scale_multiplier = 1, bool enable_physics = false, float restitution = 0, float friction = 0, float mass = 0);
	void AddModel(Model *model);
	void AddDisplayFloor(int floor);
	Control* AddControl(const std::string &name, const std::string &sound, const std::string &direction, float CenterX, float CenterZ, float width, float height, float voffset, int selection_position, std::vector<std::string> &action_names, std::vector<std::string> &textures);
	Trigger* AddTrigger(const std::string &name, const std::string &sound_file, Ogre::Vector3 &area_min, Ogre::Vector3 &area_max, std::vector<std::string> &action_names);
	bool ReplaceTexture(const std::string &oldtexture, const std::string &newtexture);
	std::vector<Sound*> GetSound(const std::string &name);
	void ResetLights();
	void ChangeLight(int floor, bool value);
	void EnableSensor(bool value, int number = 0);
	bool GetSensorStatus(int number = 0);
	std::string GetFloorDisplay();
	bool GetHoldStatus(int number = 0);
	void ResetNudgeTimer(bool start = true, int number = 0);
	void ResetDoorState(int number = 0);
	Model* GetModel(std::string name);
	void SetBeepSound(const std::string &filename);
	void SetFloorSound(const std::string &prefix);
	void SetMessageSound(bool type, bool direction, const std::string &filename);
	void SetMusic(const std::string &filename);
	bool PlayFloorBeep();
	bool PlayFloorSound();
	bool PlayMessageSound(bool type);
	float SetHeight();
	bool IsInCar(const Ogre::Vector3 &position, bool camera = false);
	bool Check(Ogre::Vector3 &position);
	void StopCarSound();
	int GetFloor();
	bool OnTopFloor();
	bool OnBottomFloor();
	bool InCar();
	int GetNearestServicedFloor();
	float GetDestinationAltitude(int floor);
	float GetDestinationOffset(int floor);
	void SetFloor(int floor, bool move_parent = true);
	bool IsLeveled();
	bool IsOnFloor(int floor, bool leveled = true);
	void NotifyArrival(int floor);
	bool IndependentServiceActive();
	bool IndependentServiceOnOtherCar();
	int FirePhase2Active();
	bool FirePhase2OnOtherCar();

	MeshObject* Mesh; //car mesh object

private:

	Elevator* parent;

	//functions
	void PlayStartingSounds();
	void PlayStoppingSounds(bool emergency = false);
	void PlayMovingSounds();

	//sound objects
	Sound *carsound;
	Sound *idlesound;
	Sound *alarm;
	Sound *floorbeep;
	std::vector<Sound*> sounds; //generic sounds
	Sound *announcesnd;
	Sound *musicsound;

	//interior directional indicators
	std::vector<DirectionalIndicator*> DirIndicatorArray;

	//doors and shaft doors
	std::vector<ElevatorDoor*> DoorArray;

	//floor indicators
	std::vector<FloorIndicator*> FloorIndicatorArray;

	//button panel array
	std::vector<ButtonPanel*> PanelArray; //elevator button panel objects

	//standard door array
	std::vector<Door*> StdDoorArray; //pointer array to standard door objects

	//light array
	std::vector<Light*> lights;

	//Models
	std::vector<Model*> ModelArray;

	//Controls
	std::vector<Control*> ControlArray;

	//Triggers
	std::vector<Trigger*> TriggerArray;

	//internal simulation data
	int CurrentFloor; //current car floor

	//misc internals
	bool DirMessageSound; //true if a directional message sound is queued, to prevent repeats
	bool DoorMessageSound; //true if a door message sound is queued, to prevent repeats
	ElevatorDoor* lastdoor_result;
	int lastdoor_number;
	bool FirstRun; //used for setting first-run items in the run loop
	int lastfloor;
	bool lastfloorset;

	//internal data for door open/close hold feature
	int doorhold_direction;
	int doorhold_whichdoors;
	int doorhold_floor;
	bool doorhold_manual;

	//cache objects for IsInCar()
	Ogre::Vector3 lastposition;
	bool lastcheckresult;
	bool checkfirstrun;
};

}

#endif
