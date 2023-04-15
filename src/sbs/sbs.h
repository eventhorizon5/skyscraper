/*
	Scalable Building Simulator - Core
	The Skyscraper Project - Version 1.12 Alpha
	Copyright (C)2004-2023 Ryan Thoryk
	https://www.skyscrapersim.net
	https://sourceforge.net/projects/skyscraper/
	Contact - ryan@thoryk.com

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

#ifndef _SBS_H
#define _SBS_H

#include <deque>
#include <queue>
#include "OgrePrerequisites.h"
#include "OgreSharedPtr.h"

//fix naming collision on Windows
#undef GetObject
#undef GetCurrentTime

//external class declarations
namespace OgreBulletDynamics {
	class DynamicsWorld;
	class RigidBody;
	class CharacterController;
}
namespace OgreBulletCollisions {
	class DebugDrawer;
	class CollisionShape;
}

namespace Ogre {
	class ConfigFile;
	struct Box;
	typedef std::vector<String> StringVector;
	typedef SharedPtr<StringVector> StringVectorPtr;
}

namespace FMOD {
	class System;
	class Channel;
}

namespace SBS {
	//forward declarations
	class SBS;
	class MeshObject;
	class DynamicMesh;
	class FloorManager;
	class ElevatorManager;
	class ShaftManager;
	class StairwellManager;
	class DoorManager;
	class TextureManager;
	class RevolvingDoorManager;
	class VehicleManager;
	class Polygon;
	class Model;
	class Wall;
	class Door;
	class Floor;
	class Elevator;
	class ElevatorCar;
	class ElevatorDoor;
	class Shaft;
	class Camera;
	struct CameraState;
	class Stairwell;
	class Sound;
	class SoundSystem;
	struct SoundData;
	class CallButton;
	class Profiler;
	class Control;
	class Trigger;
	class Escalator;
	class Action;
	class Person;
	class ButtonPanel;
	class DirectionalIndicator;
	class FloorIndicator;
	class CameraTexture;
	class Light;
	struct ElevatorRoute;
	class SceneNode;
	class TimerObject;
	class RandomGen;
	class RevolvingDoor;
	class MovingWalkway;
	class Step;
	class Vehicle;
	class DispatchController;
	class ControllerManager;
	class CallStation;
	class Indicator;

	typedef std::vector<Ogre::Vector3> PolyArray;
	typedef std::vector<PolyArray> PolygonSet;
}

#include "object.h"

namespace SBS {

extern bool SBSIMPEXP enable_profiling; //enable general profiling
extern bool SBSIMPEXP enable_advanced_profiling;

//SBS class
class SBSIMPEXP SBS : public Object
{
	friend class MeshObject;
public:

	Real delta;

	//OGRE objects
	Ogre::Root* mRoot;
	Ogre::SceneManager* mSceneManager;

	//SBS version
	std::string version;
	std::string version_state;

	//Building information
	std::string BuildingName;
	std::string BuildingFilename;
	std::string BuildingDesigner;
	std::string BuildingLocation;
	std::string BuildingDescription;
	std::string BuildingVersion;

	//physics objects
	OgreBulletDynamics::DynamicsWorld *mWorld;
	OgreBulletCollisions::DebugDrawer *debugDrawer;

	//Internal data
	bool IsRunning; //is sim engine running?
	int Floors; //number of above-ground floors including 0
	int Basements; //number of basement floors
	Camera *camera; //camera object
	bool IsFalling; //make user fall
	bool InStairwell; //true if user is in a stairwell
	bool InElevator; //true if user is in an elevator
	bool InShaft; //true if user is in a shaft
	int ElevatorNumber; //number of currently selected elevator
	int CarNumber; //number of currently selected elevator car
	bool ElevatorSync; //true if user should move with elevator
	bool FrameLimiter; //frame limiter toggle
	int FrameRate; //max frame rate
	Real running_time; //time (in seconds) the simulator has been running
	Real start_time; //time (in seconds) that the simulator mainloop started
	bool IsBuildingsEnabled; //contains status of buildings object
	bool IsExternalEnabled; //contains status of external object
	bool IsLandscapeEnabled; //contains status of landscape object
	bool IsSkyboxEnabled; //contains status of skybox object
	Real FPS; //current frame rate
	bool AutoShafts; //true if shafts should turn on and off automatically
	bool AutoStairs; //true if stairwells should turn on and off automatically
	bool ProcessElevators; //true if elevator system should be enabled
	int ShaftDisplayRange; //number of shaft floors to display while in elevator; has no effect if shaft's ShowFullShaft is true
	int StairsDisplayRange; //number of stairwell floors to display while in stairwell; has no effect if stairwell's ShowFullStairs is not 0
	int ShaftOutsideDisplayRange; //number of shaft floors to display while outside of shaft
	int StairsOutsideDisplayRange; //number of stairwell floors to display while outside of stairwell
	int FloorDisplayRange; //number of floors to display while in elevator, if shaft's ShowFloors is true
	std::string SkyName; //base filename of sky texture pack
	bool DeleteColliders; //true if system should delete mesh colliders on each modification
	Real UnitScale; //scale of 3D positions; this value equals 1 3D unit
	bool Verbose; //set to true to enable verbose mode
	bool InterfloorOnTop; //set to true to have interfloor area on top (it's on the bottom by default)
	bool FastDelete; //used internally for quick object deletion
	std::string LastError; //most recent error message, from ReportError()
	std::string LastNotification; //most recent notification message, from Report()
	int WallCount; //wall object count
	int PolygonCount; //wall polygon object count
	unsigned int SmoothFrames;
	bool RenderOnStartup; //render objects on startup
	bool RandomActivity; //random activity is enabled
	int InstanceNumber; //SBS engine instance number
	bool Headless; //true if running in headless mode

	//public functions
	SBS(Ogre::SceneManager* mSceneManager, FMOD::System *fmodsystem, int instance_number, const Ogre::Vector3 &position = Ogre::Vector3::ZERO, Real rotation = 0.0f, const Ogre::Vector3 &area_min = Ogre::Vector3::ZERO, const Ogre::Vector3 &area_max = Ogre::Vector3::ZERO);
	~SBS();
	void Initialize();
	bool Start(Ogre::Camera *camera = 0);
	void CreateSky();
	bool AddWallMain(Wall* wallobject, const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real height_in1, Real height_in2, Real altitude1, Real altitude2, Real tw, Real th, bool autosize);
	bool AddWallMain(Object *parent, MeshObject* mesh, const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real height_in1, Real height_in2, Real altitude1, Real altitude2, Real tw, Real th, bool autosize);
	bool AddFloorMain(Wall* wallobject, const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real altitude1, Real altitude2, bool reverse_axis, bool texture_direction, Real tw, Real th, bool autosize, bool legacy_behavior = false);
	bool AddFloorMain(Object *parent, MeshObject* mesh, const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real altitude1, Real altitude2, bool reverse_axis, bool texture_direction, Real tw, Real th, bool autosize, bool legacy_behavior = false);
	void CalculateFrameRate();
	void Loop();
	Wall* CreateWallBox(MeshObject* mesh, const std::string &name, const std::string &texture, Real x1, Real x2, Real z1, Real z2, Real height_in, Real voffset, Real tw, Real th, bool inside = true, bool outside = true, bool top = true, bool bottom = true, bool autosize = true);
	Wall* CreateWallBox2(MeshObject* mesh, const std::string &name, const std::string &texture, Real CenterX, Real CenterZ, Real WidthX, Real LengthZ, Real height_in, Real voffset, Real tw, Real th, bool inside = true, bool outside = true, bool top = true, bool bottom = true, bool autosize = true);
	Wall* AddTriangleWall(MeshObject* mesh, const std::string &name, const std::string &texture, Real x1, Real y1, Real z1, Real x2, Real y2, Real z2, Real x3, Real y3, Real z3, Real tw, Real th);
	Wall* AddCustomWall(MeshObject* mesh, const std::string &name, const std::string &texture, PolyArray &varray, Real tw, Real th);
	Wall* AddCustomFloor(MeshObject* mesh, const std::string &name, const std::string &texture, std::vector<Ogre::Vector2> &varray, Real altitude, Real tw, Real th);
	void AddPolygon(Wall* wallobject, const std::string &texture, PolyArray &varray, Real tw, Real th);
	void EnableBuildings(bool value);
	void EnableLandscape(bool value);
	void EnableExternal(bool value);
	void EnableSkybox(bool value);
	int GetFloorNumber(Real altitude, int lastfloor = 0, bool checklastfloor = false);
	Real GetDistance(Real x1, Real x2, Real z1, Real z2);
	Shaft* CreateShaft(int number, Real CenterX, Real CenterZ, int startfloor, int endfloor);
	Stairwell* CreateStairwell(int number, Real CenterX, Real CenterZ, int startfloor, int endfloor);
	Elevator* NewElevator(int number);
	Floor* NewFloor(int number);
	Vehicle* NewVehicle(int number);
	DispatchController* NewController(int number);
	int GetElevatorCount();
	int GetTotalFloors(); //all floors including basements
	int GetShaftCount();
	int GetStairwellCount();
	int GetVehicleCount();
	int GetControllerCount();
	Floor* GetFloor(int number);
	Elevator* GetElevator(int number);
	Shaft* GetShaft(int number);
	Stairwell* GetStairwell(int number);
	Vehicle* GetVehicle(int number);
	DispatchController* GetController(int number);
	bool SetWallOrientation(std::string direction);
	int GetWallOrientation();
	bool SetFloorOrientation(std::string direction);
	int GetFloorOrientation();
	void DrawWalls(bool MainN, bool MainP, bool SideN, bool SideP, bool Top, bool Bottom);
	void ResetWalls(bool ToDefaults = false);
	int GetDrawWallsCount();
	Real MetersToFeet(Real meters); //converts meters to feet
	Real FeetToMeters(Real feet); //converts feet to meters
	Wall* AddDoorwayWalls(MeshObject* mesh, const std::string &wallname, const std::string &texture, Real tw, Real th);
	Wall* AddWall(MeshObject* mesh, const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real height_in1, Real height_in2, Real altitude1, Real altitude2, Real tw, Real th);
	Wall* AddFloor(MeshObject* mesh, const std::string &name, const std::string &texture, Real thickness, Real x1, Real z1, Real x2, Real z2, Real altitude1, Real altitude2, bool reverse_axis, bool texture_direction, Real tw, Real th, bool legacy_behavior = false);
	Wall* AddGround(const std::string &name, const std::string &texture, Real x1, Real z1, Real x2, Real z2, Real altitude, int tile_x, int tile_z);
	void EnableFloorRange(int floor, int range, bool value, bool enablegroups, int shaftnumber = 0, int stairsnumber = 0);
	bool RegisterTimerCallback(TimerObject *timer);
	bool UnregisterTimerCallback(TimerObject *timer);
	void ProcessTimers();
	int GetTimerCallbackCount();
	bool Mount(const std::string &filename, const std::string &path);
	void AddFloorAutoArea(Ogre::Vector3 start, Ogre::Vector3 end);
	int GetMeshCount();
	Sound* AddSound(const std::string &name, const std::string &filename, const Ogre::Vector3 &position, bool loop = true, Real volume = 1.0, int speed = 100, Real min_distance = 1.0, Real max_distance = -1.0, Real doppler_level = 0.0, Real cone_inside_angle = 360, Real cone_outside_angle = 360, Real cone_outside_volume = 1.0, const Ogre::Vector3 &direction = Ogre::Vector3(0, 0, 0));
	int GetSoundCount();
	void IncrementSoundCount();
	void DecrementSoundCount();
	Real ToLocal(Real remote_value);
	Ogre::Vector2 ToLocal(const Ogre::Vector2& remote_value);
	Ogre::Vector3 ToLocal(const Ogre::Vector3& remote_value, bool rescale = true, bool flip_z = true);
	Real ToRemote(Real local_value);
	Ogre::Vector2 ToRemote(const Ogre::Vector2& local_value);
	Ogre::Vector3 ToRemote(const Ogre::Vector3& local_value, bool rescale = true, bool flip_z = true);
	int GetObjectCount();
	Object* GetObject(int number);
	Object* GetObject(std::string name, bool case_sensitive = true);
	std::vector<Object*> GetObjectRange(const std::string &expression);
	int RegisterObject(Object *object);
	bool UnregisterObject(int number);
	bool IsValidFloor(int floor);
	std::string DumpState();
	bool DeleteObject(Object *object);
	bool DeleteObject(int object);
	void RemoveFloor(Floor *floor);
	void RemoveElevator(Elevator *elevator);
	void RemoveShaft(Shaft *shaft);
	void RemoveStairwell(Stairwell *stairs);
	void RemoveSound(Sound *sound);
	void RemoveLight(Light *light);
	void RemoveModel(Model *model);
	void RemoveControl(Control *control);
	void RemoveTrigger(Trigger *trigger);
	void RemoveController(DispatchController *controller);
	std::string VerifyFile(const std::string &filename);
	std::string VerifyFile(std::string filename, bool &result, bool skip_cache);
	bool FileExists(const std::string &filename);
	int GetWallCount();
	int GetPolygonCount();
	void AddMeshHandle(MeshObject* handle);
	void DeleteMeshHandle(MeshObject* handle);
	void Prepare(bool report = true);
	Light* AddLight(const std::string &name, int type);
	MeshObject* FindMeshObject(const std::string &name);
	Model* AddModel(const std::string &name, const std::string &filename, bool center, const Ogre::Vector3 &position, const Ogre::Vector3 &rotation, Real max_render_distance = 0, Real scale_multiplier = 1, bool enable_physics = false, Real restitution = 0, Real friction = 0, Real mass = 0);
	void AddModel(Model *model);
	Ogre::Vector2 GetExtents(PolyArray &varray, int coord, bool flip_z = false);
	void Cut(Wall *wall, Ogre::Vector3 start, Ogre::Vector3 end, bool cutwalls, bool cutfloors, int checkwallnumber = 0, bool reset_check = true);
	Ogre::Vector3 GetPolygonDirection(PolyArray &polygon);
	int GetConfigInt(const std::string &key, int default_value);
	std::string GetConfigString(const std::string &key, const std::string &default_value);
	bool GetConfigBool(const std::string &key, bool default_value);
	Real GetConfigFloat(const std::string &key, Real default_value);
	void SplitWithPlane(int axis, PolyArray &orig, PolyArray &poly1, PolyArray &poly2, Real value);
	Ogre::Vector3 ComputeNormal(PolyArray &vertices, Real &D);
	bool InBox(const Ogre::Vector3 &start, const Ogre::Vector3 &end, const Ogre::Vector3 &test);
	void AdvanceClock();
	unsigned long GetCurrentTime();
	unsigned long GetRunTime();
	unsigned long GetElapsedTime();
	unsigned long GetAverageTime();
	std::string GetMountPath(std::string filename, std::string &newfilename);
	void ShowColliders(bool value);
	void CacheFilename(const std::string &filename, const std::string &result);
	void ResetDoorwayWalls();
	void SetLighting(Real red = 1.0, Real green = 1.0, Real blue = 1.0);
	void ResetLighting();
	Control* AddControl(const std::string &name, const std::string &sound, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, int selection_position, std::vector<std::string> &action_names, std::vector<std::string> &textures);
	Trigger* AddTrigger(const std::string &name, const std::string &sound_file, const Ogre::Vector3 &area_min, const Ogre::Vector3 &area_max, std::vector<std::string> &action_names);
	Action* AddAction(const std::string &name, std::vector<Object*> &action_parents, const std::string &command, const std::vector<std::string> &parameters);
	Action* AddAction(const std::string &name, std::vector<Object*> &action_parents, const std::string &command);
	std::vector<Action*> GetAction(std::string name);
	Action* GetAction(int index);
	int GetActionCount();
	bool AddActionParent(const std::string &name, std::vector<Object*> &parents);
	bool RemoveActionParent(const std::string &name, std::vector<Object*> &parents);
	bool RemoveActionParent(std::vector<Object*> &parents);
	bool RemoveAction(std::string name);
	bool RemoveAction(Action *action);
	bool RunAction(const std::string &name);
	bool RunAction(int index);
	std::vector<Sound*> GetSound(const std::string &name);
	void AddKey(int keyid, const std::string &name);
	bool CheckKey(int keyid);
	void ListKeys();
	bool MoveObject(Object *object, Ogre::Vector3 position, bool relative, bool X, bool Y, bool Z);
	bool RotateObject(Object *object, Ogre::Vector3 rotation, Real speed, bool relative, bool X, bool Y, bool Z);
	void RegisterControl(Control *control);
	void UnregisterControl(Control *control);
	Ogre::Vector2 GetEndPoint(const Ogre::Vector2 &StartPoint, Real angle, Real distance);
	void ShowFloorList();
	Ogre::Plane ComputePlane(PolyArray &vertices, bool flip_normal = true);
	void ShowSceneNodes(bool value);
	void GetDoorwayExtents(MeshObject *mesh, int checknumber, PolyArray &polygon);
	void ShowBoundingBoxes(bool value);
	void ListVisibleMeshes();
	int GetEscalatorCount();
	void IncrementEscalatorCount();
	void DecrementEscalatorCount();
	int GetMovingWalkwayCount();
	void IncrementMovingWalkwayCount();
	void DecrementMovingWalkwayCount();
	bool HitBeam(const Ogre::Ray &ray, Real max_distance, MeshObject *&mesh, Wall *&wall, Ogre::Vector3 &hit_position);
	void EnableRandomActivity(bool value);
	SoundSystem* GetSoundSystem();
	bool IsObjectValid(Object* object, std::string type = "");
	bool IsActionValid(Action* action);
	std::vector<ElevatorRoute*> GetRouteToFloor(int StartingFloor, int DestinationFloor, bool service_access = false);
	Person* CreatePerson(std::string name = "", int floor = 0, bool service_access = false);
	void RemovePerson(Person *person);
	bool AttachCamera(Ogre::Camera *camera, bool init_state = true);
	bool DetachCamera();
	std::string ProcessFullName(std::string name, int &instance, int &object_number, bool strip_number = false);
	int GetPersonCount();
	Person* GetPerson(int number);
	bool IsInside();
	bool GetBounds(Ogre::Vector3 &min, Ogre::Vector3 &max);
	bool IsInside(const Ogre::Vector3 &position);
	void CutOutsideBoundaries(bool landscape = true, bool buildings = true, bool external = false, bool floors = false);
	void CutInsideBoundaries(const Ogre::Vector3 &min, const Ogre::Vector3 &max, bool landscape = true, bool buildings = true, bool external = false, bool floors = false);
	void SetBounds(const Ogre::Vector3 &area_min, const Ogre::Vector3 &area_max);
	bool HasBounds();
	void ResetState();
	Ogre::Vector3 ToGlobal(const Ogre::Vector3 &position);
	Ogre::Vector3 FromGlobal(const Ogre::Vector3 &position);
	Ogre::Quaternion ToGlobal(const Ogre::Quaternion &orientation);
	Ogre::Quaternion FromGlobal(const Ogre::Quaternion &orientation);
	Model* GetModel(std::string name);
	FloorManager* GetFloorManager();
	ElevatorManager* GetElevatorManager();
	ShaftManager* GetShaftManager();
	StairwellManager* GetStairwellManager();
	DoorManager* GetDoorManager();
	ControllerManager* GetControllerManager();
	void RegisterDynamicMesh(DynamicMesh *dynmesh);
	void UnregisterDynamicMesh(DynamicMesh *dynmesh);
	TextureManager* GetTextureManager();
	RevolvingDoorManager* GetRevolvingDoorManager();
	VehicleManager* GetVehicleManager();
	void RegisterCameraTexture(CameraTexture *camtex);
	void UnregisterCameraTexture(CameraTexture *camtex);
	int GetCameraTextureCount();
	CameraTexture* GetCameraTexture(int number);

	//Meshes
	MeshObject* Buildings;
	MeshObject* External;
	MeshObject* Landscape;
	MeshObject* SkyBox;

	Real AmbientR, AmbientG, AmbientB, OldAmbientR, OldAmbientG, OldAmbientB; //ambient colors
	bool TexelOverride; //used for small square-shaped controls

	//instance prompt string
	std::string InstancePrompt;

private:

	//fps
	int fps_frame_count;
	int fps_tottime;
	Real remaining_delta;

	//orientations
	int wall_orientation;
	int floor_orientation;

	//wall/floor sides
	bool DrawMainN; //or top, if floor
	bool DrawMainP; //or bottom, if floor
	bool DrawSideN;
	bool DrawSideP;
	bool DrawTop; //or back, if floor
	bool DrawBottom; //or front, if floor

	//old wall/floor sides
	bool DrawMainNOld; //or top, if floor
	bool DrawMainPOld; //or bottom, if floor
	bool DrawSideNOld;
	bool DrawSidePOld;
	bool DrawTopOld; //or back, if floor
	bool DrawBottomOld; //or front, if floor

	//global object array (only pointers to actual objects)
	std::vector<Object*> ObjectArray;

	//manager objects
	FloorManager* floor_manager;
	ElevatorManager* elevator_manager;
	ShaftManager* shaft_manager;
	StairwellManager* stairwell_manager;
	DoorManager* door_manager;
	VehicleManager* vehicle_manager;
	ControllerManager* controller_manager;

	//dynamic meshes
	std::vector<DynamicMesh*> dynamic_meshes;

	//action array
	std::vector<Action*> ActionArray;

	//private functions
	void PrintBanner();
	void CheckAutoAreas();
	void CalculateAverageTime();
	std::vector<ElevatorRoute*> GetIndirectRoute(std::vector<int> &checked_floors, int StartingFloor, int DestinationFloor, bool service_access = false, bool top_level = true);
	ElevatorRoute* GetDirectRoute(Floor *floor, int DestinationFloor, bool service_access = false);

	//doorway data
	bool wall1a, wall1b, wall2a, wall2b;
	Ogre::Vector2 wall_extents_x, wall_extents_z, wall_extents_y;

	//timer callback array
	std::vector<TimerObject*> timercallbacks;

	//auto area structure
	struct AutoArea
	{
		Ogre::Vector3 start;
		Ogre::Vector3 end;
		bool inside;
		int camerafloor;
	};

	//floor auto area array
	std::vector<AutoArea> FloorAutoArea;

	//global lights
	std::vector<Light*> lights;

	//generic sound objects
	std::vector<Sound*> sounds;
	int soundcount;

	//texture manager
	TextureManager *texturemanager;

	//revolving door manager'
	RevolvingDoorManager *revolvingdoor_manager;

	//sound system
	SoundSystem *soundsystem;

	//mesh objects
	std::vector<MeshObject*> meshes;

	//global models
	std::vector<Model*> ModelArray;

	//global controls
	std::vector<Control*> ControlArray;

	//global triggers
	std::vector<Trigger*> TriggerArray;

	//person objects
	std::vector<Person*> PersonArray;

	int ObjectCount; //number of simulator objects

	int EscalatorCount; //number of escalators

	int MovingWalkwayCount; //number of moving walkways

	//internal clock
	unsigned long current_time;
	unsigned long current_virtual_time;
	unsigned long elapsed_time;
	unsigned long average_time;
	std::deque<unsigned long> frame_times;
	Ogre::Timer *timer;

	//config file
	Ogre::ConfigFile *configfile;

	struct VerifyResult
	{
		std::string filename;
		std::string result;
	};
	std::vector<VerifyResult> verify_results;

	//keys
	struct Key
	{
		int id;
		std::string name;
	};

	std::vector<Key> keys;

	//index of all controls used for action deletion callback
	std::vector<Control*> control_index;

	//file listing cache
	Ogre::StringVectorPtr filesystem_listing;

	//sim engine area trigger
	Trigger *area_trigger;

	//camera texture references
	std::vector<CameraTexture*> camtexarray;
};

}

#endif
