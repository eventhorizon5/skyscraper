/*
	Scalable Building Simulator - Elevator Car Object
	The Skyscraper Project - Version 2.1
	Copyright (C)2004-2025 Ryan Thoryk
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

#ifndef _SBS_ELEVATORCAR_H
#define _SBS_ELEVATORCAR_H

#include "doorsystem.h"

namespace SBS {

class SBSIMPEXP ElevatorCar : public Object
{
	friend class Elevator;
	friend class RouteController;

public:

	int Number; //car number
	std::string Name; //car name
	std::vector<int> ServicedFloors; //list of floors this car services
	int NumDoors; //number of elevator doors
	Real Height; //car height
	bool HeightSet;
	bool Fan; //fan enabled status
	Vector3 MusicPosition; //music emitter position, relative of elevator center
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
	std::string MusicUp; //car up music sound to play
	std::string MusicDown; //car down music sound to play
	std::string EmergencyStopSound; //car emergency stop sound
	bool AlarmActive; //true if alarm is active
	bool UseFloorBeeps; //true if floor beeps should be used
	bool UseFloorSounds; //true if floor sounds should be used
	bool UseDirMessageSounds; //true if direction message sounds should be used
	bool UseDoorMessageSounds; //true if door message sounds should be used
	bool MessageOnMove; //true if directional messages should be played when the elevator starts moving, otherwise when the doors open
	bool MessageOnStart; //this determines if the directional messages should be played at the start or end of the door open sequence
	bool MessageOnClose; //this determines if the directional messages should be played when the doors are closing
	bool MusicOn; //music enabled status
	bool MusicOnMove; //true if music should only play during move
	bool MusicAlwaysOn; //true if music should always be played, and the on/off state should be volume-based
	bool AutoEnable; //true if interior objects should automatically be enabled/disabled
	Real CameraOffset; //camera vertical offset
	int StartingFloor; //car starting floor
	bool Created; //has car been created with the CreateCar function?
	int Offset; //floor number offset from the primary elevator
	bool GotoFloor; //true if car is heading to a floor
	int LateDirection; //temporary call direction used for NotifyLate option

	ElevatorCar(Elevator *parent, int number);
	~ElevatorCar();
	bool CreateCar(int floor);
	Elevator* GetElevator();
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	Wall* AddWall(const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real height1, Real height2, Real voffset1, Real voffset2, Real tw, Real th);
	Wall* AddFloor(const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real voffset1, Real voffset2, bool reverse_axis, bool texture_direction, Real tw, Real th, bool legacy_behavior = false);
	FloorIndicator* AddFloorIndicator(const std::string &texture_prefix, const std::string &blank_texture, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset);
	Indicator* AddKeypadIndicator(const std::string& sound, const std::string& texture_prefix, const std::string& blank_texture, const std::string& direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, Real timer_duration);
	ButtonPanel* CreateButtonPanel(const std::string &texture, int rows, int columns, const std::string &direction, Real CenterX, Real CenterZ, Real buttonwidth, Real buttonheight, Real spacingX, Real spacingY, Real voffset, Real tw, Real th);
	void DumpServicedFloors();
	bool AddServicedFloor(int number, bool create_shaft_door = true);
	void RemoveServicedFloor(int number, bool remove_shaft_door = true);
	bool IsServicedFloor(int floor, bool report = true);
	int GetFloorIndex(int floor);
	int GetServicedFloorCount();
	int GetServicedFloor(int index);
	bool CheckServicedFloors();
	void Alarm();
	void OpenHatch();
	bool OnInit();
	bool Loop();
	bool Enabled(bool value);
	bool EnableObjects(bool value);
	void UpdateFloorIndicators();
	int GetTopFloor();
	int GetBottomFloor();
	void AddDirectionalIndicators(bool relative, bool active_direction, bool single, bool vertical, const std::string &BackTexture, const std::string &uptexture, const std::string &uptexture_lit, const std::string &downtexture, const std::string &downtexture_lit, Real CenterX, Real CenterZ, Real voffset, const std::string &direction, Real BackWidth, Real BackHeight, bool ShowBack, Real tw, Real th);
	DirectionalIndicator* AddDirectionalIndicator(bool active_direction, bool single, bool vertical, const std::string &BackTexture, const std::string &uptexture, const std::string &uptexture_lit, const std::string &downtexture, const std::string &downtexture_lit, Real CenterX, Real CenterZ, Real voffset, const std::string &direction, Real BackWidth, Real BackHeight, bool ShowBack, Real tw, Real th);
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
	DoorWrapper* AddDoors(int number, const std::string &lefttexture, const std::string &righttexture, Real thickness, Real CenterX, Real CenterZ, Real width, Real height, bool direction, Real tw, Real th);
	bool AddShaftDoors(int number, const std::string &lefttexture, const std::string &righttexture, Real thickness, Real CenterX, Real CenterZ, Real voffset, Real tw, Real th);
	DoorWrapper* AddShaftDoor(int floor, int number, const std::string &lefttexture, const std::string &righttexture, Real tw, Real th);
	DoorWrapper* AddShaftDoor(int floor, int number, const std::string &lefttexture, const std::string &righttexture, Real thickness, Real CenterX, Real CenterZ, Real voffset, Real tw, Real th);
	void Chime(int number, int floor, bool direction, bool early = false);
	void EnableDoors(bool value);
	bool AddFloorSigns(int door_number, bool relative, const std::string &texture_prefix, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset);
	int AreDoorsMoving(int number = 0, bool car_doors = true, bool shaft_doors = true);
	bool AreDoorsOpening(int number = 0, bool car_doors = true, bool shaft_doors = true);
	bool AreDoorsClosing(int number = 0, bool car_doors = true, bool shaft_doors = true);
	void SetShaftDoors(int number, Real thickness, Real CenterX, Real CenterZ);
	bool DoorExists(int number);
	bool ShaftDoorsExist(int number, int floor, bool include_nonserviced = false);
	Sound* AddSound(const std::string &name, const std::string &filename, Vector3 position, bool loop = true, Real volume = 1.0, int speed = 100, Real min_distance = 1.0, Real max_distance = -1.0, Real doppler_level = 0.0, Real cone_inside_angle = 360, Real cone_outside_angle = 360, Real cone_outside_volume = 1.0, Vector3 direction = Vector3(0, 0, 0));
	Reverb* AddReverb(const std::string &name, const std::string &type, const Vector3 &position, Real min_distance, Real max_distance);
	DoorWrapper* AddDoorComponent(int number, const std::string &name, const std::string &texture, const std::string &sidetexture, Real thickness, const std::string &direction, Real OpenSpeed, Real CloseSpeed, Real x1, Real z1, Real x2, Real z2, Real height, Real voffset, Real tw, Real th, Real side_tw, Real side_th);
	DoorWrapper* AddShaftDoorComponent(int number, int floor, const std::string &name, const std::string &texture, const std::string &sidetexture, Real thickness, const std::string &direction, Real OpenSpeed, Real CloseSpeed, Real x1, Real z1, Real x2, Real z2, Real height, Real voffset, Real tw, Real th, Real side_tw, Real side_th);
	void AddShaftDoorsComponent(int number, const std::string &name, const std::string &texture, const std::string &sidetexture, Real thickness, const std::string &direction, Real OpenSpeed, Real CloseSpeed, Real x1, Real z1, Real x2, Real z2, Real height, Real voffset, Real tw, Real th, Real side_tw, Real side_th);
	DoorWrapper* FinishDoors(int number, bool DoorWalls = true, bool TrackWalls = true);
	DoorWrapper* FinishShaftDoor(int number, int floor, bool DoorWalls = true, bool TrackWalls = true);
	bool FinishShaftDoors(int number, bool DoorWalls = true, bool TrackWalls = true);
	ButtonPanel* GetPanel(int index);
	Control* GetFloorButton(int floor);
	Door* AddDoor(std::string name, const std::string &open_sound, const std::string &close_sound, bool open_state, const std::string &texture, const std::string &side_texture, Real thickness, const std::string &face_direction, const std::string &open_direction, bool rotate, Real open_speed, Real close_speed, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, Real tw, Real th, Real side_tw, Real side_th);
	Door* CreateDoor(std::string name, const std::string &open_sound, const std::string &close_sound, bool rotate);
	Door* GetDoor(const std::string &name);
	Door* GetStdDoor(int number);
	void RemovePanel(ButtonPanel* panel);
	void RemoveDirectionalIndicator(DirectionalIndicator *indicator);
	void RemoveElevatorDoor(ElevatorDoor *door);
	void RemoveFloorIndicator(FloorIndicator *indicator);
	void RemoveDoor(Door *door);
	void RemoveSound(Sound *sound);
	void RemoveReverb();
	void RemoveLight(Light *light);
	void RemoveModel(Model *model);
	void RemovePrimitive(Primitive *prim);
	void RemoveCustomObject(CustomObject *object);
	void RemoveControl(Control *control);
	void RemoveTrigger(Trigger *trigger);
	bool IsNudgeModeActive(int number = 0);
	void EnableNudgeMode(bool value, int number = 0);
	Light* AddLight(const std::string &name, int type);
	Light* GetLight(const std::string &name);
	Model* AddModel(const std::string &name, const std::string &filename, bool center, Vector3 position, Vector3 rotation, Real max_render_distance = 0, Real scale_multiplier = 1, bool enable_physics = false, Real restitution = 0, Real friction = 0, Real mass = 0);
	void AddModel(Model *model);
	Primitive* AddPrimitive(const std::string &name);
	void AddPrimitive(Primitive *primitive);
	CustomObject* AddCustomObject(const std::string &name, const Vector3 &position, const Vector3 &rotation, Real max_render_distance = 0, Real scale_multiplier = 1);
	void AddCustomObject(CustomObject *object);
	void AddDisplayFloor(int floor);
	Control* AddControl(const std::string &name, const std::string &sound, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, int selection_position, std::vector<std::string> &action_names, std::vector<std::string> &textures);
	Trigger* AddTrigger(const std::string &name, const std::string &sound_file, Vector3 &area_min, Vector3 &area_max, std::vector<std::string> &action_names);
	bool ReplaceTexture(const std::string &oldtexture, const std::string &newtexture);
	std::vector<Sound*> GetSound(const std::string &name);
	Reverb* GetReverb();
	void ResetLights();
	void ChangeLight(int floor, bool value);
	void EnableSensor(bool value, int number = 0);
	bool GetSensorStatus(int number = 0);
	std::string GetFloorDisplay();
	bool GetHoldStatus(int number = 0);
	void ResetNudgeTimer(bool start = true, int number = 0);
	void ResetDoorState(int number = 0);
	Model* GetModel(std::string name);
	Primitive* GetPrimitive(std::string name);
	CustomObject* GetCustomObject(std::string name);
	void SetBeepSound(const std::string &filename);
	void SetFloorSound(const std::string &prefix);
	void SetMessageSound(bool type, bool direction, const std::string &filename);
	void SetMusic(const std::string &filename);
	bool PlayFloorBeep();
	bool PlayFloorSound();
	bool PlayMessageSound(bool type);
	Real SetHeight();
	bool IsInCar(const Vector3 &position, bool camera = false);
	bool Check(Vector3 &position);
	void StopCarSound();
	int GetFloor();
	bool OnTopFloor();
	bool OnBottomFloor();
	bool InCar();
	int GetNearestServicedFloor();
	Real GetDestinationAltitude(int floor);
	Real GetDestinationOffset(int floor);
	void SetFloor(int floor, bool move_parent = true);
	bool IsLeveled();
	bool IsOnFloor(int floor, bool leveled = true);
	void NotifyArrival(int floor, bool early = false, int direction = 0);
	bool IndependentServiceActive();
	bool IndependentServiceOnOtherCar();
	int FirePhase2Active();
	bool FirePhase2OnOtherCar();
	void SetControls(const std::string &action_name);
	void FlashIndicators(bool value);
	CameraTexture* AddCameraTexture(const std::string &name, int quality, Real fov, const Vector3 &position, bool use_rotation, const Vector3 &rotation);
	void RemoveCameraTexture(CameraTexture* camtex);
	bool RespondingToCall(int floor, int direction);
	int RespondingToCall(int floor);
	bool AddElevatorIDSigns(int door_number, bool relative, const std::string& texture_prefix, const std::string& direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset);

	MeshObject* Mesh; //car mesh object

	//for keypad
	std::vector<std::string> InvalidInput;
	bool Input(const std::string& text);
	void ProcessCache();
	Real TimerDelay;

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
	Reverb *reverb; //reverb object

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

	//Primitives
	std::vector<Primitive*> PrimArray;

	//custom objects
	std::vector<CustomObject*> CustomObjectArray;

	//Controls
	std::vector<Control*> ControlArray;

	//Triggers
	std::vector<Trigger*> TriggerArray;

	//CameraTextures
	std::vector<CameraTexture*> CameraTextureArray;

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
	int last_music_direction;

	//internal data for door open/close hold feature
	int doorhold_direction;
	int doorhold_whichdoors;
	int doorhold_floor;
	bool doorhold_manual;

	//cache objects for IsInCar()
	Vector3 lastposition;
	bool lastcheckresult;
	bool checkfirstrun;

	//for keypad
	std::string InputCache;
	void UpdateKeypadIndicator(const std::string& text, bool play_sound = true);
	Indicator* indicator;
	void KeypadError(bool type = 0);
	bool GetFloorFromID(const std::string& floor, int& result);
	void Requested(int floor);

	class KeypadTimer; //internal timer class

	//keypad timer object
	KeypadTimer* keypad_timer;
};

}

#endif
