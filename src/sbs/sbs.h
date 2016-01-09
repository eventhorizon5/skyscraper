/* $Id$ */

/*
	Scalable Building Simulator - Core
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

#ifndef _SBS_H
#define _SBS_H

#include <OgreVector2.h>
#include <OgreVector3.h>
#include <OgreMesh.h>
#include <OgreConfigFile.h>
#include <OgreFont.h>
#include <OgreTimer.h>
#include <OgreSceneManager.h>

//fix naming collision on Windows
#undef GetObject

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

#include "object.h"
#include "light.h"
#include "mesh.h"
#include "polygon.h"
#include "model.h"
#include "wall.h"
#include "floor.h"
#include "elevator.h"
#include "shaft.h"
#include "camera.h"
#include "stairs.h"
#include "sound.h"
#include "callbutton.h"
#include "profiler.h"
#include "control.h"
#include "trigger.h"
#include "escalator.h"
#include "action.h"
#include "timer.h"
#include "person.h"
#include "route.h"
#include "scenenode.h"

namespace SBS {

extern bool SBSIMPEXP enable_profiling; //enable general profiling
extern bool SBSIMPEXP enable_advanced_profiling;

//global functions

struct SBSIMPEXP FloorMap
{
	int number; //floor number
	Floor *object; //floor object reference
};

struct SBSIMPEXP ElevatorMap
{
	int number; //elevator number
	Elevator *object; //elevator object reference
};

struct SBSIMPEXP ShaftMap
{
	int number; //shaft number
	Shaft *object; //shaft object reference
};

struct SBSIMPEXP StairsMap
{
	int number; //stairs number
	Stairs *object; //stairs object reference
};

//SBS class
class SBSIMPEXP SBS : public Object
{
	friend class MeshObject;
public:

	float delta;

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
	bool ElevatorSync; //true if user should move with elevator
	bool FrameLimiter; //frame limiter toggle
	int FrameRate; //max frame rate
	float running_time; //time (in seconds) the simulator has been running
	float start_time; //time (in seconds) that the simulator mainloop started
	bool IsBuildingsEnabled; //contains status of buildings object
	bool IsExternalEnabled; //contains status of external object
	bool IsLandscapeEnabled; //contains status of landscape object
	bool IsSkyboxEnabled; //contains status of skybox object
	float FPS; //current frame rate
	bool AutoShafts; //true if shafts should turn on and off automatically
	bool AutoStairs; //true if stairwells should turn on and off automatically
	bool ProcessElevators; //true if elevator system should be enabled
	int ShaftDisplayRange; //number of shaft floors to display while in elevator; has no effect if shaft's ShowFullShaft is true
	int StairsDisplayRange; //number of stairwell floors to display while in stairwell; has no effect if stairwell's ShowFullStairs is true
	int ShaftOutsideDisplayRange; //number of shaft floors to display while outside of shaft
	int StairsOutsideDisplayRange; //number of stairwell floors to display while outside of stairwell
	int FloorDisplayRange; //number of floors to display while in elevator, if shaft's ShowFloors is true
	bool TextureOverride; //if enabled, overrides textures with ones set with SetTextureOverride()
	bool FlipTexture; //if enabled, flips textures according to parameters set in SetTextureFlip()
	std::string SkyName; //base filename of sky texture pack
	bool DeleteColliders; //true if system should delete mesh colliders on each modification
	float UnitScale; //scale of 3D positions; this value equals 1 3D unit
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

	//mouse coordinates
	int mouse_x, mouse_y;

	//public functions
	SBS(Ogre::SceneManager* mSceneManager, FMOD::System *fmodsystem, int instance_number, const Ogre::Vector3 &position = Ogre::Vector3::ZERO, float rotation = 0.0f, const Ogre::Vector3 &area_min = Ogre::Vector3::ZERO, const Ogre::Vector3 &area_max = Ogre::Vector3::ZERO);
	~SBS();
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	bool LoadTexture(const std::string &filename, const std::string &name, float widthmult, float heightmult, bool enable_force = false, bool force_mode = false, int mipmaps = -1, bool use_alpha_color = false, Ogre::ColourValue alpha_color = Ogre::ColourValue::Black);
	bool LoadAnimatedTexture(std::vector<std::string> filenames, const std::string &name, float duration, float widthmult, float heightmult, bool enable_force = false, bool force_mode = false, int mipmaps = -1, bool use_alpha_color = false, Ogre::ColourValue alpha_color = Ogre::ColourValue::Black);
	bool LoadAlphaBlendTexture(const std::string &filename, const std::string &specular_filename, const std::string &blend_filename, const std::string &name, bool spherical, float widthmult, float heightmult, bool enable_force = false, bool force_mode = false, int mipmaps = -1, bool use_alpha_color = false, Ogre::ColourValue alpha_color = Ogre::ColourValue::Black);
	bool LoadMaterial(const std::string &filename, const std::string &name, float widthmult, float heightmult, bool enable_force = false, bool force_mode = false);
	bool UnloadTexture(const std::string &name, const std::string &group);
	bool UnloadMaterial(const std::string &name, const std::string &group);
	bool LoadTextureCropped(const std::string &filename, const std::string &name, int x, int y, int width, int height, float widthmult, float heightmult, bool enable_force = false, bool force_mode = false);
	bool RotateTexture(const std::string &name, float angle);
	bool RotateAnimTexture(const std::string &name, float speed);
	bool ScrollTexture(const std::string &name, float x_offset, float y_offset);
	bool ScrollAnimTexture(const std::string &name, float x_speed, float y_speed);
	bool ScaleTexture(const std::string &name, float x_scale, float y_scale);
	bool TransformTexture(const std::string &name, const std::string &type, const std::string &wave_type, float base, float frequency, float phase, float amplitude);
	float AutoSize(float n1, float n2, bool iswidth, float offset, bool enable_force, bool force_mode);
	void Initialize();
	bool Start(Ogre::Camera *camera = 0);
	void CreateSky();
	bool AddWallMain(WallObject* wallobject, const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float altitude1, float altitude2, float tw, float th, bool autosize);
	bool AddWallMain(Object *parent, MeshObject* mesh, const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float altitude1, float altitude2, float tw, float th, bool autosize);
	bool AddFloorMain(WallObject* wallobject, const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float altitude1, float altitude2, bool reverse_axis, bool texture_direction, float tw, float th, bool autosize, bool legacy_behavior = false);
	bool AddFloorMain(Object *parent, MeshObject* mesh, const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float altitude1, float altitude2, bool reverse_axis, bool texture_direction, float tw, float th, bool autosize, bool legacy_behavior = false);
	void CalculateFrameRate();
	void MainLoop();
	bool CreateWallBox(WallObject* wallobject, const std::string &name, const std::string &texture, float x1, float x2, float z1, float z2, float height_in, float voffset, float tw, float th, bool inside = true, bool outside = true, bool top = true, bool bottom = true, bool autosize = true);
	bool CreateWallBox2(WallObject* wallobject, const std::string &name, const std::string &texture, float CenterX, float CenterZ, float WidthX, float LengthZ, float height_in, float voffset, float tw, float th, bool inside = true, bool outside = true, bool top = true, bool bottom = true, bool autosize = true);
	WallObject* AddTriangleWall(MeshObject* mesh, const std::string &name, const std::string &texture, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float tw, float th);
	WallObject* AddCustomWall(MeshObject* mesh, const std::string &name, const std::string &texture, std::vector<Ogre::Vector3> &varray, float tw, float th);
	WallObject* AddCustomFloor(MeshObject* mesh, const std::string &name, const std::string &texture, std::vector<Ogre::Vector2> &varray, float altitude, float tw, float th);
	void CreatePolygon(WallObject* wallobject, const std::string &name, const std::string &texture, std::vector<Ogre::Vector3> &varray, float tw, float th);
	void EnableBuildings(bool value);
	void EnableLandscape(bool value);
	void EnableExternal(bool value);
	void EnableSkybox(bool value);
	int GetFloorNumber(float altitude, int lastfloor = 0, bool checklastfloor = false);
	float GetDistance(float x1, float x2, float z1, float z2);
	Shaft* CreateShaft(int number, float CenterX, float CenterZ, int _startfloor, int _endfloor);
	Stairs* CreateStairwell(int number, float CenterX, float CenterZ, int _startfloor, int _endfloor);
	std::string GetTextureMaterial(const std::string &name, bool &result, bool report = true, const std::string &polygon_name = "");
	Elevator* NewElevator(int number);
	Floor* NewFloor(int number);
	int GetElevatorCount();
	int GetTotalFloors(); //all floors including basements
	int GetShaftCount();
	int GetStairsCount();
	Floor* GetFloor(int number);
	Elevator* GetElevator(int number);
	Shaft* GetShaft(int number);
	Stairs* GetStairs(int number);
	bool SetWallOrientation(std::string direction);
	int GetWallOrientation();
	bool SetFloorOrientation(std::string direction);
	int GetFloorOrientation();
	void DrawWalls(bool MainN, bool MainP, bool SideN, bool SideP, bool Top, bool Bottom);
	void ResetWalls(bool ToDefaults = false);
	void SetTextureMapping(int vertindex1, Ogre::Vector2 uv1, int vertindex2, Ogre::Vector2 uv2, int vertindex3, Ogre::Vector2 uv3);
	void SetTextureMapping2(const std::string &x1, const std::string &y1, const std::string &z1, Ogre::Vector2 uv1, const std::string &x2, const std::string &y2, const std::string &z2, Ogre::Vector2 uv2, const std::string &x3, const std::string &y3, const std::string &z3, Ogre::Vector2 uv3);
	void ResetTextureMapping(bool todefaults = false);
	void SetAutoSize(bool x, bool y);
	void GetAutoSize(bool &x, bool &y);
	int GetDrawWallsCount();
	float MetersToFeet(float meters); //converts meters to feet
	float FeetToMeters(float feet); //converts feet to meters
	void AddDoorwayWalls(WallObject *wallobject, const std::string &texture, float tw, float th);
	void SetTextureOverride(const std::string &mainneg, const std::string &mainpos, const std::string &sideneg, const std::string &sidepos, const std::string &top, const std::string &bottom);
	void SetTextureFlip(int mainneg, int mainpos, int sideneg, int sidepos, int top, int bottom);
	WallObject* AddWall(const std::string &meshname, const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float altitude1, float altitude2, float tw, float th);
	WallObject* AddFloor(const std::string &meshname, const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float altitude1, float altitude2, bool reverse_axis, bool texture_direction, float tw, float th, bool legacy_behavior = false);
	WallObject* AddGround(const std::string &name, const std::string &texture, float x1, float z1, float x2, float z2, float altitude, int tile_x, int tile_z);
	void EnableFloorRange(int floor, int range, bool value, bool enablegroups, int shaftnumber = 0, int stairsnumber = 0);
	bool RegisterDoorCallback(Door *door);
	bool UnregisterDoorCallback(Door *door);
	bool RegisterCallButtonCallback(CallButton *button);
	bool UnregisterCallButtonCallback(CallButton *button);
	bool RegisterTimerCallback(TimerObject *timer);
	bool UnregisterTimerCallback(TimerObject *timer);
	void ProcessTextureFlip(float tw, float th);
	bool GetTextureTiling(const std::string &texture, float &tw, float &th);
	bool GetTextureForce(const std::string &texture, bool &enable_force, bool &force_mode);
	bool AddTextToTexture(const std::string &origname, const std::string &name, const std::string &font_filename, float font_size, const std::string &text, int x1, int y1, int x2, int y2, const std::string &h_align, const std::string &v_align, int ColorR, int ColorG, int ColorB, bool enable_force = false, bool force_mode = false);
	bool AddTextureOverlay(const std::string &orig_texture, const std::string &overlay_texture, const std::string &name, int x, int y, int width, int height, float widthmult, float heightmult, bool enable_force = false, bool force_mode = false);
	void ProcessDoors();
	void ProcessCallButtons();
	void ProcessTimers();
	int GetDoorCallbackCount();
	int GetCallButtonCallbackCount();
	int GetTimerCallbackCount();
	bool Mount(const std::string &filename, const std::string &path);
	void FreeTextureImages();
	void AddFloorAutoArea(Ogre::Vector3 start, Ogre::Vector3 end);
	int GetMeshCount();
	int GetTextureCount();
	int GetMaterialCount();
	Sound* AddSound(const std::string &name, const std::string &filename, const Ogre::Vector3 &position, bool loop = true, float volume = 1.0, int speed = 100, float min_distance = 1.0, float max_distance = -1.0, float doppler_level = 0.0, float cone_inside_angle = 360, float cone_outside_angle = 360, float cone_outside_volume = 1.0, const Ogre::Vector3 &direction = Ogre::Vector3(0, 0, 0));
	int GetSoundCount();
	void IncrementSoundCount();
	void DecrementSoundCount();
	float ToLocal(float remote_value);
	Ogre::Vector2 ToLocal(const Ogre::Vector2& remote_value);
	Ogre::Vector3 ToLocal(const Ogre::Vector3& remote_value, bool rescale = true, bool flip_z = true);
	float ToRemote(float local_value);
	Ogre::Vector2 ToRemote(const Ogre::Vector2& local_value);
	Ogre::Vector3 ToRemote(const Ogre::Vector3& local_value, bool rescale = true, bool flip_z = true);
	int GetObjectCount();
	Object* GetObject(int number);
	Object* GetObject(std::string name);
	std::vector<Object*> GetObjectRange(const std::string &expression);
	int RegisterObject(Object *object);
	bool UnregisterObject(int number);
	bool GetTextureMapping(std::vector<Ogre::Vector3> &vertices, Ogre::Vector3 &v1, Ogre::Vector3 &v2, Ogre::Vector3 &v3, int &direction);
	void SetPlanarMapping(bool flat, bool FlipX, bool FlipY, bool FlipZ, bool rotate);
	Ogre::Vector2 CalculateSizing(const std::string &texture, const Ogre::Vector3 &v1, const Ogre::Vector3 &v2, const Ogre::Vector3 &v3, int direction, float tw, float th);
	bool IsValidFloor(int floor);
	std::string DumpState();
	bool DeleteObject(Object *object);
	bool DeleteObject(int object);
	void RemoveFloor(Floor *floor);
	void RemoveElevator(Elevator *elevator);
	void RemoveShaft(Shaft *shaft);
	void RemoveStairs(Stairs *stairs);
	void RemoveSound(Sound *sound);
	void RemoveLight(Light *light);
	void RemoveModel(Model *model);
	void RemoveControl(Control *control);
	void RemoveTrigger(Trigger *trigger);
	std::string VerifyFile(const std::string &filename);
	std::string VerifyFile(std::string filename, bool &result, bool skip_cache);
	bool FileExists(const std::string &filename);
	int GetWallCount();
	int GetPolygonCount();
	void AddMeshHandle(MeshObject* handle);
	void DeleteMeshHandle(MeshObject* handle);
	void Prepare(bool report = true);
	Light* AddLight(const std::string &name, int type, const Ogre::Vector3 &position, const Ogre::Vector3 &direction, float color_r, float color_g, float color_b, float spec_color_r, float spec_color_g, float spec_color_b, float spot_inner_angle, float spot_outer_angle, float spot_falloff, float att_range, float att_constant, float att_linear, float att_quadratic);
	MeshObject* FindMeshObject(Ogre::MeshPtr meshwrapper);
	MeshObject* FindMeshObject(const std::string &name);
	Model* AddModel(const std::string &name, const std::string &filename, bool center, const Ogre::Vector3 &position, const Ogre::Vector3 &rotation, float max_render_distance = 0, float scale_multiplier = 1, bool enable_physics = false, float restitution = 0, float friction = 0, float mass = 0);
	void AddModel(Model *model);
	Ogre::Vector2 GetExtents(std::vector<Ogre::Vector3> &varray, int coord, bool flip_z = false);
	void Cut(WallObject *wall, Ogre::Vector3 start, Ogre::Vector3 end, bool cutwalls, bool cutfloors, int checkwallnumber = 0, bool reset_check = true);
	Ogre::Vector3 GetPolygonDirection(std::vector<Ogre::Vector3> &polygon);
	int GetConfigInt(const std::string &key, int default_value);
	std::string GetConfigString(const std::string &key, const std::string &default_value);
	bool GetConfigBool(const std::string &key, bool default_value);
	float GetConfigFloat(const std::string &key, float default_value);
	int Classify(int axis, std::vector<Ogre::Vector3> &vertices, float value);
	void SplitWithPlane(int axis, std::vector<Ogre::Vector3> &orig, std::vector<Ogre::Vector3> &poly1, std::vector<Ogre::Vector3> &poly2, float value);
	Ogre::Vector3 ComputeNormal(std::vector<Ogre::Vector3> &vertices, float &D);
	bool InPolygon(std::vector<Ogre::Vector3> &poly, const Ogre::Vector3 &v);
	int WhichSide3D(const Ogre::Vector3 &p, const Ogre::Vector3 &v1, const Ogre::Vector3 &v2);
	bool InBox(const Ogre::Vector3 &start, const Ogre::Vector3 &end, const Ogre::Vector3 &test);
	void AdvanceClock();
	unsigned long GetCurrentTime();
	unsigned long GetRunTime();
	unsigned long GetElapsedTime();
	unsigned long GetAverageTime();
	std::string GetMountPath(std::string filename, std::string &newfilename);
	Ogre::TexturePtr loadChromaKeyedTexture(const std::string& filename, const std::string& resGroup, const std::string& name, const Ogre::ColourValue& keyCol = Ogre::ColourValue::Black, int numMipmaps = -1, float threshold = 0.003f);
	void ShowColliders(bool value);
	void CacheFilename(const std::string &filename, const std::string &result);
	void ResetDoorwayWalls();
	void SetLighting(float red = 1.0, float green = 1.0, float blue = 1.0);
	void ResetLighting();
	void SaveTexture(Ogre::TexturePtr texture, const std::string &filename);
	std::string ListTextures(bool show_filename = false);
	Control* AddControl(const std::string &name, const std::string &sound, const std::string &direction, float CenterX, float CenterZ, float width, float height, float voffset, std::vector<std::string> &action_names, std::vector<std::string> &textures);
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
	void IncrementTextureCount();
	void DecrementTextureCount();
	void IncrementMaterialCount();
	void DecrementMaterialCount();
	std::vector<Sound*> GetSound(const std::string &name);
	void AddKey(int keyid, const std::string &name);
	bool CheckKey(int keyid);
	void ListKeys();
	bool MoveObject(Object *object, Ogre::Vector3 position, bool relative, bool X, bool Y, bool Z);
	bool RotateObject(Object *object, Ogre::Vector3 rotation, float speed, bool relative, bool X, bool Y, bool Z);
	void CameraLoop();
	void RegisterControl(Control *control);
	void UnregisterControl(Control *control);
	Ogre::Vector2 GetEndPoint(const Ogre::Vector2 &StartPoint, float angle, float distance);
	void RegisterTextureInfo(const std::string &name, const std::string &material_name, const std::string &filename, float widthmult, float heightmult, bool enable_force, bool force_mode);
	bool UnregisterTextureInfo(std::string name, std::string material_name = "");
	void ShowFloorList();
	Ogre::Plane ComputePlane(std::vector<Ogre::Vector3> &vertices);
	bool SegmentPlane(const Ogre::Vector3 &u, const Ogre::Vector3 &v, Ogre::Plane &plane, Ogre::Vector3 &isect, float &dist);
	void ShowSceneNodes(bool value);
	void GetDoorwayExtents(MeshObject *mesh, int checknumber, std::vector<Ogre::Vector3> &polygon);
	void ShowBoundingBoxes(bool value);
	void ListVisibleMeshes();
	int GetEscalatorCount();
	void IncrementEscalatorCount();
	void DecrementEscalatorCount();
	bool HitBeam(const Ogre::Ray &ray, float max_distance, MeshObject *&mesh, WallObject *&wall, Ogre::Vector3 &hit_position);
	void EnableRandomActivity(bool value);
	SoundSystem* GetSoundSystem() { return soundsystem; }
	bool IsObjectValid(Object* object, std::string type = "");
	bool IsActionValid(Action* action);
	std::vector<ElevatorRoute*> GetRouteToFloor(int StartingFloor, int DestinationFloor, bool service_access = false);
	Person* CreatePerson(std::string name = "", int floor = 0, bool service_access = false);
	void RemovePerson(Person *person);
	bool AttachCamera(Ogre::Camera *camera, bool init_state = true);
	bool DetachCamera();
	std::string ProcessFullName(std::string name, int &instance, int &object_number, bool strip_number = false);
	Ogre::MaterialPtr CreateMaterial(const std::string &name, const std::string &path);
	Ogre::MaterialPtr GetMaterialByName(const std::string &name, const std::string &group = Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
	Ogre::TextureUnitState* BindTextureToMaterial(Ogre::MaterialPtr mMat, std::string texture_name, bool has_alpha);
	Ogre::TextureUnitState* GetTextureUnitState(Ogre::MaterialPtr mMat);
	std::string GetTextureName(Ogre::MaterialPtr mMat);
	Ogre::TexturePtr GetTextureByName(const std::string &name, const std::string &group = Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
	int GetPersonCount() { return (int)PersonArray.size(); }
	Person* GetPerson(int number);
	void CopyTexture(Ogre::TexturePtr source, Ogre::TexturePtr destination);
	void CopyTexture(Ogre::TexturePtr source, Ogre::TexturePtr destination, const Ogre::Box &srcBox, const Ogre::Box &dstBox);
	bool IsInside();
	bool GetBounds(Ogre::Vector3 &min, Ogre::Vector3 &max);
	bool IsInside(const Ogre::Vector3 &position);
	void CutOutsideBoundaries(bool landscape = true, bool buildings = true, bool external = false, bool floors = false);
	void CutInsideBoundaries(const Ogre::Vector3 &min, const Ogre::Vector3 &max, bool landscape = true, bool buildings = true, bool external = false, bool floors = false);
	void SetBounds(const Ogre::Vector3 &area_min, const Ogre::Vector3 &area_max);
	bool HasBounds() { return (area_trigger != 0); }
	void ResetBuilding();
	Ogre::Vector3 ToGlobal(const Ogre::Vector3 &position);
	Ogre::Vector3 FromGlobal(const Ogre::Vector3 &position);
	Ogre::Quaternion ToGlobal(const Ogre::Quaternion &orientation);
	Ogre::Quaternion FromGlobal(const Ogre::Quaternion &orientation);

	//Meshes
	MeshObject* Buildings;
	MeshObject* External;
	MeshObject* Landscape;
	MeshObject* SkyBox;

	float AmbientR, AmbientG, AmbientB, OldAmbientR, OldAmbientG, OldAmbientB; //ambient colors
	bool TexelOverride; //used for small square-shaped controls

private:

	//fps
	int fps_frame_count;
	int fps_tottime;
	float remaining_delta;

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

	//texture mapping
	std::vector<int> MapIndex;
	std::vector<int> OldMapIndex;
	std::vector<Ogre::Vector2> MapUV;
	std::vector<Ogre::Vector2> OldMapUV;
	std::vector<std::string> MapVerts1;
	std::vector<std::string> MapVerts2;
	std::vector<std::string> MapVerts3;
	std::vector<std::string> OldMapVerts1;
	std::vector<std::string> OldMapVerts2;
	std::vector<std::string> OldMapVerts3;
	bool AutoX, AutoY; //autosizing
	int MapMethod; //texture mapping method - 0=planar, 1=index, 2=verts
	int OldMapMethod;
	bool RevX, RevY, RevZ; //extent reversals (planar texture mapping)
	bool OldRevX, OldRevY, OldRevZ;
	bool PlanarFlat, OldPlanarFlat;
	bool PlanarRotate, OldPlanarRotate;
	int DefaultMapper; //default texture mapper

	//global object array (only pointers to actual objects)
	std::vector<Object*> ObjectArray;

	//object arrays
	std::vector<FloorMap> FloorArray; //floor object array
	std::vector<ElevatorMap> ElevatorArray; //elevator object array
	std::vector<ShaftMap> ShaftArray; //shaft object array
	std::vector<StairsMap> StairsArray; //stairs object array

	//action array
	std::vector<Action*> ActionArray;

	//private functions
	void PrintBanner();
	void CheckAutoAreas();
	void BackupMapping();
	bool WriteToTexture(const std::string &str, Ogre::TexturePtr destTexture, int destLeft, int destTop, int destRight, int destBottom, Ogre::FontPtr font, const Ogre::ColourValue &color, char justify = 'l', char vert_justify = 't', bool wordwrap = true);
	void CalculateAverageTime();
	std::vector<ElevatorRoute*> GetIndirectRoute(std::vector<int> &checked_floors, int StartingFloor, int DestinationFloor, bool service_access = false, bool top_level = true);
	ElevatorRoute* GetDirectRoute(Floor *floor, int DestinationFloor, bool service_access = false);
	Ogre::TexturePtr LoadTexture(const std::string &filename, int mipmaps, bool &has_alpha, bool use_alpha_color = false, Ogre::ColourValue alpha_color = Ogre::ColourValue::Black);
	void UnloadMaterials();

	//doorway data
	bool wall1a, wall1b, wall2a, wall2b;
	Ogre::Vector2 wall_extents_x, wall_extents_z, wall_extents_y;

	//texture information structure
	struct TextureInfo
	{
		std::string name;
		std::string material_name; //used if material is loaded instead of texture, as an alias
		std::string filename;
		float widthmult;
		float heightmult;
		bool enable_force; //enable forcing of tile or stretch mode?
		bool force_mode; //false to disable autosizing, true to enable autosizing
	};

	std::vector<TextureInfo> textureinfo;

	//override textures
	std::string mainnegtex, mainpostex, sidenegtex, sidepostex, toptex, bottomtex;

	//texture flipping
	int mainnegflip, mainposflip, sidenegflip, sideposflip, topflip, bottomflip;
	std::vector<float> widthscale;
	std::vector<float> heightscale;

	//door object array for callback
	std::vector<Door*> doorcallbacks;

	//call button object array for callback
	std::vector<CallButton*> buttoncallbacks;

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

	//textures/materials count
	int texturecount;
	int materialcount;

	//sound system
	SoundSystem *soundsystem;

	//meshes
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

	//internal clock
	unsigned long current_time;
	unsigned long current_virtual_time;
	unsigned long elapsed_time;
	unsigned long average_time;
	std::deque<unsigned long> frame_times;
	Ogre::Timer *timer;

	//config file
	Ogre::ConfigFile configfile;

	struct VerifyResult
	{
		std::string filename;
		std::string result;
	};
	std::vector<VerifyResult> verify_results;

	struct TexturePixelBox
	{
		Ogre::FontPtr font;
		Ogre::PixelBox box;
		unsigned char *buffer;
	};
	std::vector<TexturePixelBox> textureboxes;

	//keys
	struct Key
	{
		int id;
		std::string name;
	};

	std::vector<Key> keys;

	//index of all controls used for action deletion callback
	std::vector<Control*> control_index;

	//function caching
	Floor* getfloor_result;
	int getfloor_number;
	Elevator* getelevator_result;
	int getelevator_number;
	Shaft* getshaft_result;
	int getshaft_number;
	Stairs* getstairs_result;
	int getstairs_number;
	std::string prev_material;

	//file listing cache
	Ogre::StringVectorPtr filesystem_listing;

	//sim engine area trigger
	Trigger *area_trigger;

	//instance prompt string
	std::string InstancePrompt;
};

}

#endif
