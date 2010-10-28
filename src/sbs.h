/* $Id$ */

/*
	Scalable Building Simulator - Core
	The Skyscraper Project - Version 1.8 Alpha
	Copyright (C)2004-2010 Ryan Thoryk
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

#include <Ogre.h>

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
class SBSIMPEXP SBS
{
public:

	Object *object; //SBS object

	unsigned int elapsed_time;
	float delta;

	Ogre::Root* mRoot;
	Ogre::SceneManager* mSceneManager;

	//SBS version
	Ogre::String version;
	Ogre::String version_state;

	//Building information
	Ogre::String BuildingName;
	Ogre::String BuildingFilename;
	Ogre::String BuildingDesigner;
	Ogre::String BuildingLocation;
	Ogre::String BuildingDescription;
	Ogre::String BuildingVersion;

	//Internal data
	bool IsRunning; //is sim engine running?
	int Floors; //number of above-ground floors including 0
	int Basements; //number of basement floors
	Camera *camera; //camera object
	bool RenderOnly; //skip sim processing and only render graphics
	bool InputOnly; //skip sim processing and only run input and rendering code
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
	bool ShowFullShafts; //true if entire shaft should be displayed while user is in an elevator
	bool ShowFullStairs; //true if entire stairwell should be displayed while user is in it
	int ShaftDisplayRange; //number of shaft floors to display while in elevator; has no effect if ShowFullShafts is true
	int StairsDisplayRange; //number of stairwell floors to display while in stairwell; has no effect if ShowFullStairs is true
	int ShaftOutsideDisplayRange; //number of shaft floors to display while outside of shaft
	int StairsOutsideDisplayRange; //number of stairwell floors to display while outside of stairwell
	int FloorDisplayRange; //number of floors to display while in elevator, if shaft's ShowFloors is true
	bool TextureOverride; //if enabled, overrides textures with ones set with SetTextureOverride()
	bool FlipTexture; //if enabled, flips textures according to parameters set in SetTextureFlip()
	Ogre::String SkyName; //base filename of sky texture pack
	Ogre::String root_dir; //app directory
	Ogre::String dir_char;
	bool DisableSound; //disable sound system if true
	bool RecreateColliders; //true if system should recreate mesh colliders on each modification
	float UnitScale; //scale of 3D positions; this value equals 1 3D unit
	bool Verbose; //set to true to enable verbose mode
	bool InterfloorOnTop; //set to true to have interfloor area on top (it's on the bottom by default)
	bool FastDelete; //used internally for quick object deletion
	Ogre::String LastError; //most recent error message, from ReportError()
	Ogre::String LastNotification; //most recent notification message, from Report()
	int WallCount; //wall object count
	int PolygonCount; //wall polygon object count
	bool Shaders; //true if shaders are enabled

	//mouse coordinates
	int mouse_x, mouse_y;

	//public functions
	SBS();
	~SBS();
	void Report(std::string message);
	bool ReportError(std::string message);
	bool LoadTexture(const char *filename, const char *name, float widthmult, float heightmult, bool enable_force = false, bool force_mode = false);
	bool UnloadTexture(const char *name);
	bool LoadTextureCropped(const char *filename, const char *name, int x, int y, int width, int height, float widthmult, float heightmult, bool enable_force = false, bool force_mode = false);
	float AutoSize(float n1, float n2, bool iswidth, float offset, bool enable_force, bool force_mode);
	bool Initialize(Ogre::RenderWindow* mRenderWindow, Ogre::SceneManager* mSceneManager, const char* rootdirectory, const char* directory_char);
	bool Start();
	void CreateSky(const char *filenamebase);
	int AddWallMain(WallObject* wallobject, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float altitude1, float altitude2, float tw, float th, bool autosize);
	int AddWallMain(Object *parent, Ogre::Mesh* mesh, std::vector<Ogre::SubMesh> &submeshes, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float altitude1, float altitude2, float tw, float th, bool autosize);
	int AddFloorMain(WallObject* wallobject, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float altitude1, float altitude2, float tw, float th, bool autosize);
	int AddFloorMain(Object *parent, Ogre::Mesh* mesh, std::vector<Ogre::SubMesh> &submeshes, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float altitude1, float altitude2, float tw, float th, bool autosize);
	void CalculateFrameRate();
	void MainLoop();
	int CreateWallBox(WallObject* wallobject, const char *name, const char *texture, float x1, float x2, float z1, float z2, float height_in, float voffset, float tw, float th, bool inside = true, bool outside = true, bool top = true, bool bottom = true, bool autosize = true);
	int CreateWallBox2(WallObject* wallobject, const char *name, const char *texture, float CenterX, float CenterZ, float WidthX, float LengthZ, float height_in, float voffset, float tw, float th, bool inside = true, bool outside = true, bool top = true, bool bottom = true, bool autosize = true);
	int AddTriangleWall(WallObject* wallobject, const char *name, const char *texture, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float tw, float th);
	int AddCustomWall(WallObject* wallobject, const char *name, const char *texture, std::vector<Ogre::Vector3> &varray, float tw, float th);
	void InitMeshes();
	void EnableBuildings(bool value);
	void EnableLandscape(bool value);
	void EnableExternal(bool value);
	void EnableSkybox(bool value);
	int GetFloorNumber(float altitude, int lastfloor = 0, bool checklastfloor = false);
	float GetDistance(float x1, float x2, float z1, float z2);
	void ListAltitudes();
	Object* CreateShaft(int number, int type, float CenterX, float CenterZ, int _startfloor, int _endfloor);
	Object* CreateStairwell(int number, float CenterX, float CenterZ, int _startfloor, int _endfloor);
	Ogre::Material* GetTextureMaterial(const char *texture, bool &result, const char *polygon_name = 0);
	bool NewElevator(int number);
	bool NewFloor(int number);
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
	void SetTextureMapping2(Ogre::String x1, Ogre::String y1, Ogre::String z1, Ogre::Vector2 uv1, Ogre::String x2, Ogre::String y2, Ogre::String z2, Ogre::Vector2 uv2, Ogre::String x3, Ogre::String y3, Ogre::String z3, Ogre::Vector2 uv3);
	void ResetTextureMapping(bool todefaults = false);
	void ReverseAxis(bool value);
	bool GetReverseAxis();
	void SetAutoSize(bool x, bool y);
	void GetAutoSize(bool &x, bool &y);
	int GetDrawWallsCount();
	float MetersToFeet(float meters); //converts meters to feet
	float FeetToMeters(float feet); //converts feet to meters
	int AddDoorwayWalls(WallObject *wallobject, const char *texture, float tw, float th);
	void SetListenerLocation(const Ogre::Vector3 &location);
	void SetListenerDirection(const Ogre::Vector3 &front, const Ogre::Vector3 &top);
	void SetListenerDistanceFactor(float factor);
	float GetListenerDistanceFactor();
	void SetListenerRollOffFactor(float factor);
	float GetListenerRollOffFactor();
	void SetTextureOverride(const char *mainneg, const char *mainpos, const char *sideneg, const char *sidepos, const char *top, const char *bottom);
	void SetTextureFlip(int mainneg, int mainpos, int sideneg, int sidepos, int top, int bottom);
	WallObject* AddWall(const char *meshname, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float altitude1, float altitude2, float tw, float th);
	WallObject* AddFloor(const char *meshname, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float altitude1, float altitude2, float tw, float th);
	WallObject* AddGround(const char *name, const char *texture, float x1, float z1, float x2, float z2, float altitude, int tile_x, int tile_z);
	void EnableFloorRange(int floor, int range, bool value, bool enablegroups, int shaftnumber = 0);
	bool RegisterDoorCallback(Door *door);
	bool UnregisterDoorCallback(Door *door);
	bool RegisterCallButtonCallback(CallButton *button);
	bool UnregisterCallButtonCallback(CallButton *button);
	void ProcessTextureFlip(float tw, float th);
	bool GetTextureTiling(const char *texture, float &tw, float &th);
	bool GetTextureForce(const char *texture, bool &enable_force, bool &force_mode);
	bool AddTextToTexture(const char *origname, const char *name, const char *font_filename, float font_size, const char *text, int x1, int y1, int x2, int y2, const char *h_align, const char *v_align, int ColorR, int ColorG, int ColorB, bool enable_force = false, bool force_mode = false);
	bool AddTextureOverlay(const char *orig_texture, const char *overlay_texture, const char *name, int x, int y, int width, int height, float widthmult, float heightmult, bool enable_force = false, bool force_mode = false);
	void ProcessDoors();
	void ProcessCallButtons();
	int GetDoorCallbackCount();
	int GetCallButtonCallbackCount();
	bool Mount(const char *filename, const char *path);
	void FreeTextureImages();
	void AddFloorAutoArea(Ogre::Vector3 start, Ogre::Vector3 end);
	int GetMeshCount();
	int GetTextureCount();
	int GetMaterialCount();
	int GetMeshFactoryCount();
	void CreateColliders(Ogre::Mesh* mesh);
	void DeleteColliders(Ogre::Mesh* mesh);
	Object* AddSound(const char *name, const char *filename, Ogre::Vector3 position, int volume = 1.0, int speed = 100, float min_distance = 1.0, float max_distance = -1.0, float dir_radiation = 0, Ogre::Vector3 direction = Ogre::Vector3(0, 0, 0));
	int GetSoundCount();
	void IncrementSoundCount();
	void DecrementSoundCount();
	float ToLocal(float remote_value);
	Ogre::Vector2 ToLocal(const Ogre::Vector2& remote_value);
	Ogre::Vector3 ToLocal(const Ogre::Vector3& remote_value);
	float ToRemote(float local_value);
	Ogre::Vector2 ToRemote(const Ogre::Vector2& local_value);
	Ogre::Vector3 ToRemote(const Ogre::Vector3& local_value);
	int GetObjectCount();
	Object* GetObject(int number);
	int RegisterObject(Object *object);
	bool UnregisterObject(int number);
	void GetTextureMapping(std::vector<Ogre::Vector3> &vertices, Ogre::Vector3 &v1, Ogre::Vector3 &v2, Ogre::Vector3 &v3);
	void SetPlanarMapping(bool flat, bool X, bool Y, bool Z);
	Ogre::Vector2 CalculateSizing(const char *texture, Ogre::Vector2 x, Ogre::Vector2 y, Ogre::Vector2 z, float tw, float th);
	//WallObject* GetWallObject(std::vector<WallObject*> &wallarray, int polygon_index);
	Ogre::String TruncateNumber(double value, int decimals);
	Ogre::String TruncateNumber(float value, int decimals);
	Ogre::String TruncateNumber(const char *number, int decimals);
	bool IsValidFloor(int floor);
	Ogre::String DumpState();
	bool DeleteObject(Object *object);
	bool DeleteObject(int object);
	void RemoveFloor(Floor *floor);
	void RemoveElevator(Elevator *elevator);
	void RemoveShaft(Shaft *shaft);
	void RemoveStairs(Stairs *stairs);
	void RemoveSound(Sound *sound);
	const char* VerifyFile(const char *filename);
	bool FileExists(const char *filename, bool relative = false);
	int GetWallCount();
	int GetPolygonCount();
	void AddLightHandle(Light* handle);
	void DeleteLightHandle(Light* handle);
	void AddMeshHandle(MeshObject* handle);
	void DeleteMeshHandle(MeshObject* handle);
	void AddModelHandle(Model* handle);
	void DeleteModelHandle(Model* handle);
	void Prepare();
	Object* AddLight(const char *name, int type, Ogre::Vector3 position, Ogre::Vector3 direction, float radius, float max_distance, float color_r, float color_g, float color_b, float spec_color_r, float spec_color_g, float spec_color_b, float directional_cutoff_radius, float spot_falloff_inner, float spot_falloff_outer);
	MeshObject* FindMeshObject(Ogre::Mesh* meshwrapper);
	Object* AddModel(const char *name, const char *filename, Ogre::Vector3 position, Ogre::Vector3 rotation, float max_render_distance = 0, float scale_multiplier = 1);
	void DumpVertices(WallObject* wallobject);
	Ogre::Vector2 GetExtents(std::vector<Ogre::Vector3> &varray, int coord);
	Ogre::Vector2 GetExtents(const Ogre::Vector3 *varray, int count, int coord);
	Ogre::Vector2 GetExtents(Ogre::Mesh* mesh, int coord);
	Ogre::Vector3 GetPoint(std::vector<WallObject*> &wallarray, const char *polyname, const Ogre::Vector3 &start, const Ogre::Vector3 &end);
	Ogre::Mesh* AddGenWall(Ogre::Mesh* mesh, const char *texture, float x1, float z1, float x2, float z2, float height, float altitude, float tw, float th);
	void Cut(WallObject *wall, const Ogre::Vector3& start, const Ogre::Vector3& end, bool cutwalls, bool cutfloors, const Ogre::Vector3& mesh_origin, const Ogre::Vector3& object_origin, int checkwallnumber = 0, const char *checkstring = "", bool reset_check = true);
	Ogre::Vector3 GetWallExtents(std::vector<WallObject*> &wallarray, const char *name, float altitude,  bool get_max);
	Ogre::Vector3 GetPolygonDirection(std::vector<Ogre::Vector3> &polygon);
	Ogre::HardwareIndexBuffer* PolyMesh(Ogre::Mesh* mesh, std::vector<Ogre::SubMesh> &submeshes, const char *name, const char *texture, std::vector<Ogre::Vector3> &vertices, float tw, float th, bool autosize, Ogre::Matrix3 &tex_matrix, Ogre::Vector3 &tex_vector, std::vector<Ogre::Vector2> &mesh_indices);
	Ogre::HardwareIndexBuffer* PolyMesh(Ogre::Mesh* mesh, std::vector<Ogre::SubMesh> &submeshes, const char *name, Ogre::Material* material, std::vector<std::vector<Ogre::Vector3> > &vertices, Ogre::Matrix3 &tex_matrix, Ogre::Vector3 &tex_vector, std::vector<Ogre::Vector2> &mesh_indices, bool convert_vertices = true);
	bool ComputeTextureMap(Ogre::Matrix3 &t_matrix, Ogre::Vector3 &t_vector, std::vector<Ogre::Vector3> &vertices, const Ogre::Vector3 &p1, const Ogre::Vector2 &uv1, const Ogre::Vector3 &p2, const Ogre::Vector2 &uv2, const Ogre::Vector3 &p3, const Ogre::Vector2 &uv3);
	Ogre::Vector2* GetTexels(Ogre::Matrix3 &tex_matrix, Ogre::Vector3 &tex_vector, std::vector<std::vector<Ogre::Vector3> > &vertices);
	int ReindexSubMesh(Ogre::Mesh* state, std::vector<Ogre::SubMesh> &submeshes, Ogre::HardwareIndexBuffer* indices, Ogre::Material* material, const char *name, bool add);
	int FindMatchingSubMesh(std::vector<Ogre::SubMesh> &submeshes, Ogre::Material *material);
	void DeleteVertices(std::vector<WallObject*> &wallarray, Ogre::HardwareIndexBuffer *deleted_indices);
	int GetConfigInt(const char *key, int default_value);
	const char *GetConfigString(const char *key, const char *default_value);
	bool GetConfigBool(const char *key, bool default_value);
	float GetConfigFloat(const char *key, float default_value);

	//Meshes
	MeshObject* Buildings;
	MeshObject* External;
	MeshObject* Landscape;
	MeshObject* SkyBox;

private:

	//fps
	int fps_frame_count;
	int fps_tottime;
	float remaining_delta;

	//conversion buffers
	char intbuffer[65];
	char buffer[20];

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
	bool ReverseAxisValue;

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
	std::vector<Ogre::String> MapVerts1;
	std::vector<Ogre::String> MapVerts2;
	std::vector<Ogre::String> MapVerts3;
	std::vector<Ogre::String> OldMapVerts1;
	std::vector<Ogre::String> OldMapVerts2;
	std::vector<Ogre::String> OldMapVerts3;
	bool AutoX, AutoY; //autosizing
	int MapMethod; //texture mapping method - 0=planar, 1=index, 2=verts
	int OldMapMethod;
	bool RevX, RevY, RevZ; //extent reversals (planar texture mapping)
	bool OldRevX, OldRevY, OldRevZ;
	bool PlanarFlat, OldPlanarFlat;
	int DefaultMapper; //default texture mapper

	//global object array (only pointers to actual objects)
	std::vector<Object*> ObjectArray;

	//object arrays
	std::vector<FloorMap> FloorArray; //floor object array
	std::vector<ElevatorMap> ElevatorArray; //elevator object array
	std::vector<ShaftMap> ShaftArray; //shaft object array
	std::vector<StairsMap> StairsArray; //stairs object array

	//private functions
	void PrintBanner();
	void CheckAutoAreas();
	void BackupMapping();
	bool ComputeTextureSpace(Ogre::Matrix3 &m, Ogre::Vector3 &v, const Ogre::Vector3 &v_orig, const Ogre::Vector3 &v1, float len1, const Ogre::Vector3 &v2, float len2);

	//doorway data
	bool wall1a, wall1b, wall2a, wall2b;
	Ogre::Vector2 wall_extents_x, wall_extents_z, wall_extents_y;

	//texture information structure
	struct TextureInfo
	{
		Ogre::String name;
		float widthmult;
		float heightmult;
		bool enable_force; //enable forcing of tile or stretch mode?
		bool force_mode; //false to disable autosizing, true to enable autosizing
	};

	std::vector<TextureInfo> textureinfo;

	//override textures
	Ogre::String mainnegtex, mainpostex, sidenegtex, sidepostex, toptex, bottomtex;

	//texture flipping
	int mainnegflip, mainposflip, sidenegflip, sideposflip, topflip, bottomflip;
	std::vector<float> widthscale;
	std::vector<float> heightscale;

	//door object array for callback
	std::vector<Door*> doorcallbacks;

	//call button object array for callback
	std::vector<CallButton*> buttoncallbacks;

	//auto area structure
	struct AutoArea
	{
		Ogre::Box box;
		bool inside;
		int camerafloor;
	};

	//floor auto area array
	std::vector<AutoArea> FloorAutoArea;

	//all lights
	std::vector<Light*> all_lights;

	//global lights
	std::vector<Light*> lights;

	//generic sound objects
	std::vector<Sound*> sounds;
	int soundcount;

	//meshes
	std::vector<MeshObject*> meshes;

	//global models
	std::vector<Model*> ModelArray;

	//all models
	std::vector<Model*> all_models;

	int ObjectCount; //number of simulator objects
};

#endif
