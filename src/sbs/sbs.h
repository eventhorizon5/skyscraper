/* $Id$ */

/*
	Scalable Building Simulator - Core
	The Skyscraper Project - Version 1.10 Alpha
	Copyright (C)2004-2015 Ryan Thoryk
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
#include <fmod.hpp>

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
#include "action.h"
#include "timer.h"

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

	//FMOD objects
	FMOD::System *soundsys;

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
	bool DisableSound; //disable sound system if true
	bool DeleteColliders; //true if system should delete mesh colliders on each modification
	float UnitScale; //scale of 3D positions; this value equals 1 3D unit
	bool Verbose; //set to true to enable verbose mode
	bool InterfloorOnTop; //set to true to have interfloor area on top (it's on the bottom by default)
	bool FastDelete; //used internally for quick object deletion
	std::string LastError; //most recent error message, from ReportError()
	std::string LastNotification; //most recent notification message, from Report()
	int WallCount; //wall object count
	int PolygonCount; //wall polygon object count
	bool enable_profiling; //enable general profiling
	bool enable_advanced_profiling;
	unsigned int SmoothFrames;
	bool RenderOnStartup; //render objects on startup

	//mouse coordinates
	int mouse_x, mouse_y;

	//public functions
	SBS(Ogre::SceneManager* mSceneManager, FMOD::System *fmodsystem);
	~SBS();
	void Report(std::string message);
	bool ReportError(std::string message);
	bool LoadTexture(const char *filename, const char *name, float widthmult, float heightmult, bool enable_force = false, bool force_mode = false, int mipmaps = -1, bool use_alpha_color = false, Ogre::ColourValue alpha_color = Ogre::ColourValue::Black);
	bool LoadAnimatedTexture(std::vector<std::string> filenames, const char *name, float duration, float widthmult, float heightmult, bool enable_force = false, bool force_mode = false, int mipmaps = -1, bool use_alpha_color = false, Ogre::ColourValue alpha_color = Ogre::ColourValue::Black);
	bool LoadAlphaBlendTexture(const char *filename, const char *specular_filename, const char *blend_filename, const char *name, bool spherical, float widthmult, float heightmult, bool enable_force = false, bool force_mode = false, int mipmaps = -1, bool use_alpha_color = false, Ogre::ColourValue alpha_color = Ogre::ColourValue::Black);
	bool LoadMaterial(const char *filename, const char *name, float widthmult, float heightmult, bool enable_force = false, bool force_mode = false);
	bool UnloadTexture(const char *name, const char *group);
	bool UnloadMaterial(const char *name, const char *group);
	bool LoadTextureCropped(const char *filename, const char *name, int x, int y, int width, int height, float widthmult, float heightmult, bool enable_force = false, bool force_mode = false);
	bool RotateTexture(const char *name, float angle);
	bool RotateAnimTexture(const char *name, float speed);
	bool ScrollTexture(const char *name, float x_offset, float y_offset);
	bool ScrollAnimTexture(const char *name, float x_speed, float y_speed);
	bool ScaleTexture(const char *name, float x_scale, float y_scale);
	bool TransformTexture(const char *name, const char *type, const char *wave_type, float base, float frequency, float phase, float amplitude);
	float AutoSize(float n1, float n2, bool iswidth, float offset, bool enable_force, bool force_mode);
	void Initialize(Ogre::Camera *camera);
	bool Start();
	void CreateSky(const char *filenamebase);
	bool AddWallMain(WallObject* wallobject, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float altitude1, float altitude2, float tw, float th, bool autosize);
	bool AddWallMain(Object *parent, MeshObject* mesh, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float altitude1, float altitude2, float tw, float th, bool autosize);
	bool AddFloorMain(WallObject* wallobject, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float altitude1, float altitude2, bool reverse_axis, bool texture_direction, float tw, float th, bool autosize, bool legacy_behavior = false);
	bool AddFloorMain(Object *parent, MeshObject* mesh, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float altitude1, float altitude2, bool reverse_axis, bool texture_direction, float tw, float th, bool autosize, bool legacy_behavior = false);
	void CalculateFrameRate();
	void MainLoop();
	bool CreateWallBox(WallObject* wallobject, const char *name, const char *texture, float x1, float x2, float z1, float z2, float height_in, float voffset, float tw, float th, bool inside = true, bool outside = true, bool top = true, bool bottom = true, bool autosize = true);
	bool CreateWallBox2(WallObject* wallobject, const char *name, const char *texture, float CenterX, float CenterZ, float WidthX, float LengthZ, float height_in, float voffset, float tw, float th, bool inside = true, bool outside = true, bool top = true, bool bottom = true, bool autosize = true);
	bool AddTriangleWall(WallObject* wallobject, const char *name, const char *texture, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float tw, float th);
	bool AddCustomWall(WallObject* wallobject, const char *name, const char *texture, std::vector<Ogre::Vector3> &varray, float tw, float th);
	bool AddCustomFloor(WallObject* wallobject, const char *name, const char *texture, std::vector<Ogre::Vector2> &varray, float altitude, float tw, float th);
	void EnableBuildings(bool value);
	void EnableLandscape(bool value);
	void EnableExternal(bool value);
	void EnableSkybox(bool value);
	int GetFloorNumber(float altitude, int lastfloor = 0, bool checklastfloor = false);
	float GetDistance(float x1, float x2, float z1, float z2);
	Object* CreateShaft(int number, float CenterX, float CenterZ, int _startfloor, int _endfloor);
	Object* CreateStairwell(int number, float CenterX, float CenterZ, int _startfloor, int _endfloor);
	std::string GetTextureMaterial(const char *name, bool &result, bool report = true, const char *polygon_name = 0);
	Object* NewElevator(int number);
	Object* NewFloor(int number);
	int Elevators();
	int TotalFloors(); //all floors including basements
	int Shafts();
	int StairsNum();
	Floor* GetFloor(int number);
	Elevator* GetElevator(int number);
	Shaft* GetShaft(int number);
	Stairs* GetStairs(int number);
	bool SetWallOrientation(const char *direction);
	int GetWallOrientation();
	bool SetFloorOrientation(const char *direction);
	int GetFloorOrientation();
	void DrawWalls(bool MainN, bool MainP, bool SideN, bool SideP, bool Top, bool Bottom);
	void ResetWalls(bool ToDefaults = false);
	void SetTextureMapping(int vertindex1, Ogre::Vector2 uv1, int vertindex2, Ogre::Vector2 uv2, int vertindex3, Ogre::Vector2 uv3);
	void SetTextureMapping2(std::string x1, std::string y1, std::string z1, Ogre::Vector2 uv1, std::string x2, std::string y2, std::string z2, Ogre::Vector2 uv2, std::string x3, std::string y3, std::string z3, Ogre::Vector2 uv3);
	void ResetTextureMapping(bool todefaults = false);
	void SetAutoSize(bool x, bool y);
	void GetAutoSize(bool &x, bool &y);
	int GetDrawWallsCount();
	float MetersToFeet(float meters); //converts meters to feet
	float FeetToMeters(float feet); //converts feet to meters
	void AddDoorwayWalls(WallObject *wallobject, const char *texture, float tw, float th);
	void SetListenerPosition(const Ogre::Vector3 &position);
	void SetListenerDirection(const Ogre::Vector3 &front, const Ogre::Vector3 &top);
	void SetTextureOverride(const char *mainneg, const char *mainpos, const char *sideneg, const char *sidepos, const char *top, const char *bottom);
	void SetTextureFlip(int mainneg, int mainpos, int sideneg, int sidepos, int top, int bottom);
	WallObject* AddWall(const char *meshname, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float altitude1, float altitude2, float tw, float th);
	WallObject* AddFloor(const char *meshname, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float altitude1, float altitude2, bool reverse_axis, bool texture_direction, float tw, float th, bool legacy_behavior = false);
	WallObject* AddGround(const char *name, const char *texture, float x1, float z1, float x2, float z2, float altitude, int tile_x, int tile_z);
	void EnableFloorRange(int floor, int range, bool value, bool enablegroups, int shaftnumber = 0, int stairsnumber = 0);
	bool RegisterDoorCallback(Door *door);
	bool UnregisterDoorCallback(Door *door);
	bool RegisterCallButtonCallback(CallButton *button);
	bool UnregisterCallButtonCallback(CallButton *button);
	bool RegisterTimerCallback(TimerObject *timer);
	bool UnregisterTimerCallback(TimerObject *timer);
	void ProcessTextureFlip(float tw, float th);
	bool GetTextureTiling(const char *texture, float &tw, float &th);
	bool GetTextureForce(const char *texture, bool &enable_force, bool &force_mode);
	bool AddTextToTexture(const char *origname, const char *name, const char *font_filename, float font_size, const char *text, int x1, int y1, int x2, int y2, const char *h_align, const char *v_align, int ColorR, int ColorG, int ColorB, bool enable_force = false, bool force_mode = false);
	bool AddTextureOverlay(const char *orig_texture, const char *overlay_texture, const char *name, int x, int y, int width, int height, float widthmult, float heightmult, bool enable_force = false, bool force_mode = false);
	void ProcessDoors();
	void ProcessCallButtons();
	void ProcessTimers();
	int GetDoorCallbackCount();
	int GetCallButtonCallbackCount();
	int GetTimerCallbackCount();
	bool Mount(const char *filename, const char *path);
	void FreeTextureImages();
	void AddFloorAutoArea(Ogre::Vector3 start, Ogre::Vector3 end);
	int GetMeshCount();
	int GetTextureCount();
	int GetMaterialCount();
	Object* AddSound(const char *name, const char *filename, Ogre::Vector3 position, bool loop = true, float volume = 1.0, int speed = 100, float min_distance = 1.0, float max_distance = -1.0, float doppler_level = 0.0, float cone_inside_angle = 360, float cone_outside_angle = 360, float cone_outside_volume = 1.0, Ogre::Vector3 direction = Ogre::Vector3(0, 0, 0));
	int GetSoundCount();
	void IncrementSoundCount();
	void DecrementSoundCount();
	float ToLocal(float remote_value);
	Ogre::Vector2 ToLocal(const Ogre::Vector2& remote_value);
	Ogre::Vector3 ToLocal(const Ogre::Vector3& remote_value, bool rescale = true);
	float ToRemote(float local_value);
	Ogre::Vector2 ToRemote(const Ogre::Vector2& local_value);
	Ogre::Vector3 ToRemote(const Ogre::Vector3& local_value, bool rescale = true);
	int GetObjectCount();
	Object* GetObject(int number);
	Object* GetObject(const std::string name);
	std::vector<Object*> GetObjectRange(std::string expression);
	int RegisterObject(Object *object);
	bool UnregisterObject(int number);
	bool GetTextureMapping(std::vector<Ogre::Vector3> &vertices, Ogre::Vector3 &v1, Ogre::Vector3 &v2, Ogre::Vector3 &v3, int &direction);
	void SetPlanarMapping(bool flat, bool FlipX, bool FlipY, bool FlipZ, bool rotate);
	Ogre::Vector2 CalculateSizing(const char *texture, const Ogre::Vector3 &v1, const Ogre::Vector3 &v2, const Ogre::Vector3 &v3, int direction, float tw, float th);
	//WallObject* GetWallObject(std::vector<WallObject*> &wallarray, int polygon_index);
	std::string TruncateNumber(float value, int decimals);
	std::string TruncateNumber(const char *number, int decimals);
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
	std::string VerifyFile(const char *filename);
	bool FileExists(const char *filename);
	int GetWallCount();
	int GetPolygonCount();
	void AddMeshHandle(MeshObject* handle);
	void DeleteMeshHandle(MeshObject* handle);
	void Prepare(bool report = true);
	Object* AddLight(const char *name, int type, Ogre::Vector3 position, Ogre::Vector3 direction, float color_r, float color_g, float color_b, float spec_color_r, float spec_color_g, float spec_color_b, float spot_inner_angle, float spot_outer_angle, float spot_falloff, float att_range, float att_constant, float att_linear, float att_quadratic);
	MeshObject* FindMeshObject(Ogre::MeshPtr meshwrapper);
	MeshObject* FindMeshObject(std::string name);
	Object* AddModel(const char *name, const char *filename, bool center, Ogre::Vector3 position, Ogre::Vector3 rotation, float max_render_distance = 0, float scale_multiplier = 1, bool enable_physics = false, float restitution = 0, float friction = 0, float mass = 0);
	Ogre::Vector2 GetExtents(std::vector<Ogre::Vector3> &varray, int coord, bool flip_z = false);
	void Cut(WallObject *wall, Ogre::Vector3 start, Ogre::Vector3 end, bool cutwalls, bool cutfloors, int checkwallnumber = 0, bool reset_check = true);
	Ogre::Vector3 GetPolygonDirection(std::vector<Ogre::Vector3> &polygon);
	int GetConfigInt(std::string key, int default_value);
	std::string GetConfigString(std::string key, std::string default_value);
	bool GetConfigBool(std::string key, bool default_value);
	float GetConfigFloat(std::string key, float default_value);
	int Classify(int axis, std::vector<Ogre::Vector3> &vertices, float value);
	void SplitWithPlane(int axis, std::vector<Ogre::Vector3> &orig, std::vector<Ogre::Vector3> &poly1, std::vector<Ogre::Vector3> &poly2, float value);
	Ogre::Vector3 ComputeNormal(std::vector<Ogre::Vector3> &vertices, float &D);
	bool InPolygon(std::vector<Ogre::Vector3> &poly, const Ogre::Vector3 &v);
	int WhichSide3D(const Ogre::Vector3 &p, const Ogre::Vector3 &v1, const Ogre::Vector3 &v2);
	bool InBox(Ogre::Vector3 &start, Ogre::Vector3 &end, Ogre::Vector3 &test);
	void AdvanceClock();
	unsigned long GetCurrentTime();
	unsigned long GetRunTime();
	unsigned long GetElapsedTime();
	unsigned long GetAverageTime();
	std::string GetMountPath(const char *filename, std::string &newfilename);
	void loadChromaKeyedTexture(const std::string& filename, const std::string& resGroup, const std::string& name, const Ogre::ColourValue& keyCol = Ogre::ColourValue::Black, int numMipmaps = -1, float threshold = 0.003f);
	void ShowColliders(bool value);
	void CacheFilename(std::string filename, std::string result);
	void ResetDoorwayWalls();
	void SetLighting(float red = 1.0, float green = 1.0, float blue = 1.0);
	void ResetLighting();
	void SaveTexture(Ogre::TexturePtr texture, std::string filename);
	std::string ListTextures(bool show_filename = false);
	Object* AddControl(const char *name, const char *sound, const char *direction, float CenterX, float CenterZ, float width, float height, float voffset, std::vector<std::string> &action_names, std::vector<std::string> &textures);
	Object* AddTrigger(const char *name, const char *sound_file, Ogre::Vector3 &area_min, Ogre::Vector3 &area_max, std::vector<std::string> &action_names);
	Action* AddAction(const std::string name, std::vector<Object*> &action_parents, const std::string &command, const std::vector<std::string> &parameters);
	Action* AddAction(const std::string name, std::vector<Object*> &action_parents, const std::string &command);
	std::vector<Action*> GetAction(std::string name);
	Action* GetAction(int index);
	int GetActionCount();
	bool AddActionParent(const std::string name, std::vector<Object*> &parents);
	bool RemoveActionParent(const std::string name, std::vector<Object*> &parents);
	bool RemoveActionParent(std::vector<Object*> &parents);
	bool RemoveAction(const std::string name);
	bool RemoveAction(Action *action);
	bool RunAction(std::string name);
	bool RunAction(int index);
	void IncrementTextureCount();
	void DecrementTextureCount();
	void IncrementMaterialCount();
	void DecrementMaterialCount();
	std::vector<Sound*> GetSound(const char *name);
	void AddKey(int keyid, std::string name);
	bool CheckKey(int keyid);
	void ListKeys();
	bool MoveObject(Object *object, Ogre::Vector3 position, bool relative, bool X, bool Y, bool Z);
	bool RotateObject(Object *object, Ogre::Vector3 rotation, float speed, bool relative, bool X, bool Y, bool Z);
	void CameraLoop();
	void RegisterControl(Control *control);
	void UnregisterControl(Control *control);
	Ogre::Vector2 GetEndPoint(const Ogre::Vector2 &StartPoint, float angle, float distance);
	void RegisterTextureInfo(const char *name, const char *material_name, const char *filename, float widthmult, float heightmult, bool enable_force, bool force_mode);
	bool UnregisterTextureInfo(const char *name, const char *material_name = "");
	void ShowFloorInfo(bool all_floors, int floor = 0);
	Ogre::Plane ComputePlane(std::vector<Ogre::Vector3> &vertices);
	bool SegmentPlane(const Ogre::Vector3 &u, const Ogre::Vector3 &v, Ogre::Plane &plane, Ogre::Vector3 &isect, float &dist);
	void ShowSceneNodes(bool value);
	void GetDoorwayExtents(MeshObject *mesh, int checknumber, std::vector<Ogre::Vector3> &polygon);

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

	//listener sound objects
	FMOD_VECTOR listener_position;
	FMOD_VECTOR listener_velocity;
	FMOD_VECTOR listener_forward;
	FMOD_VECTOR listener_up;

	//meshes
	std::vector<MeshObject*> meshes;

	//global models
	std::vector<Model*> ModelArray;

	//global controls
	std::vector<Control*> ControlArray;

	//global triggers
	std::vector<Trigger*> TriggerArray;

	int ObjectCount; //number of simulator objects

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
};

#endif
