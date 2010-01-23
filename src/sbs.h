/* $Id$ */

/*
	Scalable Building Simulator - Core
	The Skyscraper Project - Version 1.6 Alpha
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

#include <iengine/engine.h>
#include <ivideo/graph2d.h>
#include <ivideo/graph3d.h>
#include <imap/loader.h>
#include <csutil/array.h>
#include <csutil/csstring.h>
#include <csutil/ref.h>
#include <iengine/sector.h>
#include <iengine/mesh.h>
#include <imesh/object.h>
#include <imesh/thing.h>
#include <iutil/virtclk.h>
#include <iutil/vfs.h>
#include <isndsys/ss_renderer.h>
#include <isndsys/ss_loader.h>
#include <iutil/cfgmgr.h>
#include <ivaria/view.h>
#include <csgeom/poly3d.h>
#include <cstool/collider.h>
#include <ivaria/reporter.h>
#include <isndsys/ss_manager.h>
#include <iutil/objreg.h>

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

	//CS engine objects
	csRef<iEngine> engine;
	csRef<iGraphics3D> g3d;
	csRef<iGraphics2D> g2d;
	csRef<iLoader> loader;
	csRef<iVirtualClock> vc;
	csRef<iView> view;
	csRef<iVFS> vfs;
	csRef<iCollideSystem> collision_sys;
	csRef<iReporter> rep;
	csRef<iSndSysRenderer> sndrenderer;
	csRef<iSndSysLoader> sndloader;
	csRef<iSndSysManager> sndmanager;
	csRef<iSector> area;
	csRef<iObjectRegistry> object_reg;
	csRef<iConfigManager> confman;

	csTicks elapsed_time;
	float delta;

	//Building information
	csString BuildingName;
	csString BuildingDesigner;
	csString BuildingLocation;
	csString BuildingDescription;
	csString BuildingVersion;

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
	float HorizScale; //horizontal X/Z scaling multiplier (in feet). Normally is 1
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
	csString SkyName; //base filename of sky texture pack
	csString root_dir; //app directory
	csString dir_char;
	bool DisableSound; //disable sound system if true
	bool RecreateColliders; //true if system should recreate mesh colliders on each modification
	float UnitScale; //scale of 3D positions; this value equals 1 3D unit
	bool Verbose; //set to true to enable verbose mode
	bool InterfloorOnTop; //set to true to have interfloor area on top (it's on the bottom by default)
	bool FastDelete; //used internally for quick object deletion

	//mouse coordinates
	int mouse_x, mouse_y;

	//public functions
	SBS();
	~SBS();
	void Report (const char* msg, ...);
	bool ReportError (const char* msg, ...);
	bool LoadTexture(const char *filename, const char *name, float widthmult, float heightmult, bool enable_force = false, bool force_mode = false);
	bool UnloadTexture(const char *name);
	bool LoadTextureCropped(const char *filename, const char *name, int x, int y, int width, int height, float widthmult, float heightmult, bool enable_force = false, bool force_mode = false);
	float AutoSize(float n1, float n2, bool iswidth, float offset, bool enable_force, bool force_mode);
	bool Initialize(iSCF* scf, iObjectRegistry* objreg, iView* view, const char* rootdirectory, const char* directory_char);
	bool Start();
	int CreateSky(const char *filenamebase);
	void AddLight(const char *name, float x, float y, float z, float radius, float r, float g, float b);
	int AddWallMain(WallObject* wallobject, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float altitude1, float altitude2, float tw, float th);
	int AddWallMain(Object *parent, csRef<iMeshWrapper> mesh, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float altitude1, float altitude2, float tw, float th);
	int AddFloorMain(WallObject* wallobject, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float altitude1, float altitude2, float tw, float th);
	int AddFloorMain(Object *parent, csRef<iMeshWrapper> mesh, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float altitude1, float altitude2, float tw, float th);
	void CalculateFrameRate();
	void MainLoop();
	int CreateWallBox(WallObject* wallobject, const char *name, const char *texture, float x1, float x2, float z1, float z2, float height_in, float voffset, float tw, float th, bool inside = true, bool outside = true, bool top = true, bool bottom = true);
	int CreateWallBox2(WallObject* wallobject, const char *name, const char *texture, float CenterX, float CenterZ, float WidthX, float LengthZ, float height_in, float voffset, float tw, float th, bool inside = true, bool outside = true, bool top = true, bool bottom = true);
	int AddTriangleWall(WallObject* wallobject, const char *name, const char *texture, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float tw, float th);
	int AddCustomWall(WallObject* wallobject, const char *name, const char *texture, csPoly3D &varray, float tw, float th);
	csVector2 GetExtents(csPoly3D &varray, int coord);
	csVector2 GetExtents(csRef<iThingFactoryState> state, int coord);
	csVector2 GetExtents(const csVector3 *varray, int count, int coord);
	void InitMeshes();
	void EnableBuildings(bool value);
	void EnableLandscape(bool value);
	void EnableExternal(bool value);
	void EnableSkybox(bool value);
	int GetFloorNumber(float altitude, int lastfloor = 0, bool checklastfloor = false);
	float GetDistance(float x1, float x2, float z1, float z2);
	void DumpVertices(WallObject* wallobject);
	void ListAltitudes();
	Object* CreateShaft(int number, int type, float CenterX, float CenterZ, int _startfloor, int _endfloor);
	Object* CreateStairwell(int number, float CenterX, float CenterZ, int _startfloor, int _endfloor);
	void SetTexture(csRef<iThingFactoryState> mesh, int index, const char *texture, bool has_thickness, float tw, float th);
	iMaterialWrapper* ChangeTexture(iMeshWrapper *mesh, const char *texture, bool matcheck = true);
	iMaterialWrapper* GetTextureMaterial(const char *texture, bool &result, const char *polygon_name = 0);
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
	void SetTextureMapping(int vertindex1, csVector2 uv1, int vertindex2, csVector2 uv2, int vertindex3, csVector2 uv3);
	void SetTextureMapping2(csString x1, csString y1, csString z1, csVector2 uv1, csString x2, csString y2, csString z2, csVector2 uv2, csString x3, csString y3, csString z3, csVector2 uv3);
	void ResetTextureMapping(bool todefaults = false);
	void ReverseAxis(bool value);
	bool GetReverseAxis();
	void SetAutoSize(bool x, bool y);
	void GetAutoSize(bool &x, bool &y);
	int GetDrawWallsCount();
	csVector3 GetPoint(csRef<iThingFactoryState> mesh, const char *polyname, const csVector3 &start, const csVector3 &end);
	void Cut(csRef<iMeshWrapper> mesh, csArray<WallObject*> &wallarray, csVector3 start, csVector3 end, bool cutwalls, bool cutfloors, csVector3 mesh_origin, csVector3 object_origin, int checkwallnumber = 0, const char *checkstring = "");
	float MetersToFeet(float meters); //converts meters to feet
	float FeetToMeters(float feet); //converts feet to meters
	int AddDoorwayWalls(WallObject *wallobject, const char *texture, float tw, float th);
	void SetListenerLocation(const csVector3 &location);
	void SetListenerDirection(const csVector3 &front, const csVector3 &top);
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
	void EnableMesh(csRef<iMeshWrapper> mesh, bool value);
	iMeshWrapper* AddGenWall(csRef<iMeshWrapper> mesh, const char *texture, float x1, float z1, float x2, float z2, float height, float altitude, float tw, float th);
	void ProcessDoors();
	void ProcessCallButtons();
	int GetDoorCallbackCount();
	int GetCallButtonCallbackCount();
	bool Mount(const char *filename, const char *path);
	void FreeTextureImages();
	void AddFloorAutoArea(csVector3 start, csVector3 end);
	int GetMeshCount();
	int GetTextureCount();
	int GetMaterialCount();
	int GetMeshFactoryCount();
	void CreateColliders(csRef<iMeshWrapper> mesh);
	void DeleteColliders(csRef<iMeshWrapper> mesh);
	Object* AddSound(const char *name, const char *filename, csVector3 position, int volume = 1.0, int speed = 100, float min_distance = 1.0, float max_distance = -1.0, float dir_radiation = 0, csVector3 direction = 0);
	int GetSoundCount();
	void IncrementSoundCount();
	float ToLocal(float remote_value);
	csVector2 ToLocal(csVector2 remote_value);
	csVector3 ToLocal(csVector3 remote_value);
	float ToRemote(float local_value);
	csVector2 ToRemote(csVector2 local_value);
	csVector3 ToRemote(csVector3 local_value);
	int GetObjectCount();
	Object* GetObject(int number);
	int RegisterObject(Object *object);
	bool UnregisterObject(int number);
	void GetTextureMapping(iThingFactoryState *state, int index, csVector3 &v1, csVector3 &v2, csVector3 &v3);
	void SetPlanarMapping(bool flat, bool X, bool Y, bool Z);
	csVector3 GetWallExtents(csRef<iThingFactoryState> state, const char *name, float altitude,  bool get_max);
	csVector3 GetPolygonDirection(csPoly3D &polygon);
	csVector2 CalculateSizing(const char *texture, csVector2 x, csVector2 y, csVector2 z, float tw, float th);
	void ApplyTextureMapping(iThingFactoryState *state, int start_index, int end_index, float tw, float th);
	WallObject* CreateWallObject(csArray<WallObject*> &array, csRef<iMeshWrapper> mesh, Object *parent, const char *name);
	WallObject* GetWallObject(csArray<WallObject*> &wallarray, int polygon_index);
	csString TruncateNumber(double value, int decimals);
	csString TruncateNumber(float value, int decimals);
	csString TruncateNumber(const char *number, int decimals);

	//Meshes
	csRef<iMeshWrapper> Buildings; //building mesh
		csRef<iThingFactoryState> Buildings_state;
		csArray<WallObject*> Buildings_walls;

	csRef<iMeshWrapper> External; //external mesh
		csRef<iThingFactoryState> External_state;
		csArray<WallObject*> External_walls;

	csRef<iMeshWrapper> Landscape; //landscape mesh
		csRef<iThingFactoryState> Landscape_state;
		csArray<WallObject*> Landscape_walls;

	csRef<iMeshWrapper> SkyBox; //skybox mesh
		csRef<iThingFactoryState> SkyBox_state;
		Object *Skybox_object;

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
	csArray<int> MapIndex;
	csArray<int> OldMapIndex;
	csArray<csVector2> MapUV;
	csArray<csVector2> OldMapUV;
	csArray<csString> MapVerts1;
	csArray<csString> MapVerts2;
	csArray<csString> MapVerts3;
	csArray<csString> OldMapVerts1;
	csArray<csString> OldMapVerts2;
	csArray<csString> OldMapVerts3;
	bool AutoX, AutoY; //autosizing
	int MapMethod; //texture mapping method - 0=planar, 1=index, 2=verts
	int OldMapMethod;
	bool RevX, RevY, RevZ; //extent reversals (planar texture mapping)
	bool OldRevX, OldRevY, OldRevZ;
	bool PlanarFlat, OldPlanarFlat;
	int DefaultMapper; //default texture mapper

	//global object array (only pointers to actual objects)
	csArray<Object*> ObjectArray;

	//object arrays
	csArray<FloorMap> FloorArray; //floor object array
	csArray<ElevatorMap> ElevatorArray; //elevator object array
	csArray<ShaftMap> ShaftArray; //shaft object array
	csArray<StairsMap> StairsArray; //stairs object array

	//private functions
	void PrintBanner();
	void CheckAutoAreas();
	void BackupMapping();

	//doorway data
	bool wall1a, wall1b, wall2a, wall2b;
	csVector2 wall_extents_x, wall_extents_z, wall_extents_y;

	//texture information structure
	struct TextureInfo
	{
		csString name;
		float widthmult;
		float heightmult;
		bool enable_force; //enable forcing of tile or stretch mode?
		bool force_mode; //false to disable autosizing, true to enable autosizing
	};

	csArray<TextureInfo> textureinfo;

	//override textures
	csString mainnegtex, mainpostex, sidenegtex, sidepostex, toptex, bottomtex;

	//texture flipping
	int mainnegflip, mainposflip, sidenegflip, sideposflip, topflip, bottomflip;
	csArray<float> widthscale;
	csArray<float> heightscale;

	//door object array for callback
	csArray<Door*> doorcallbacks;

	//call button object array for callback
	csArray<CallButton*> buttoncallbacks;

	//auto area structure
	struct AutoArea
	{
		csBox3 box;
		bool inside;
		int camerafloor;
	};

	//floor auto area array
	csArray<AutoArea> FloorAutoArea;

	//generic sound objects
	csArray<Sound*> sounds;
	int soundcount;

	int ObjectCount; //number of simulator objects
};

#endif
