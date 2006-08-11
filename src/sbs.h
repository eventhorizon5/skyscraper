/*
	Scalable Building Simulator - Core
	The Skyscraper Project - Version 1.1 Alpha
	Copyright ©2005-2006 Ryan Thoryk
	http://www.tliquest.net/skyscraper
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@tliquest.net

	$Id$
	
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

#include "globals.h"
#include "floor.h"
#include "elevator.h"

//global functions
bool IsEven(int Number);
double AutoSize(double n1, double n2, bool iswidth, bool external, double offset);
bool IsNumeric(const char *expression);
static bool SBSEventHandler(iEvent& Event);
void Cleanup();

struct iEngine;
struct iSector;
struct iView;
struct iFont;
struct iFile;
struct iKeyboardDriver;
struct iImageLoader;
struct iLoaderPlugin;
struct iMeshWrapper;
struct iConsoleOutput;
struct iVirtualClock;
struct iObjectRegistry;
struct iGraphics3D;
struct iGraphics2D;
struct iLoader;
struct iVFS;
struct iEvent;
class DemoSequenceManager;
class csTransform;

class SBS
{
public:

    //Engine data
	csRef<iEngine> engine;
	csRef<iLoader> loader;
	csRef<iGraphics3D> g3d;
	csRef<iGraphics2D> g2d;
	csRef<iKeyboardDriver> kbd;
	csRef<iVirtualClock> vc;
	csRef<iView> view;
	csRef<iLight> light;
	csRef<iConsoleOutput> console;
	csRef<iFont> font;
	csRef<iVFS> vfs;
	csRef<iImageIO> imageio;
	csRef<iCommandLineParser> cmdline;
	csRef<iGeneralMeshState> gmSingle;
	csRef<iStringSet> strings;
	csRef<iStandardReporterListener> stdrep;
	csRef<iEventQueue> equeue;
	csRef<iBase> plug;

	iMaterialWrapper* material;
	iLightList* ll;
	iSector* area;

	csTicks elapsed_time, current_time;

	//Building information
	csString BuildingName;
	csString BuildingDesigner;
	csString BuildingLocation;
	csString BuildingDescription;
	csString BuildingVersion;

	//Internal data
    double Gravity; //gravity variable for physics algorithms
    bool IsRunning; //is sim engine running?
    int ElevatorShafts; //number of elevator shafts
	int TotalFloors; //number of above-ground floors including 0
	int Basements; //number of basement floors
	int Elevators; //number of elevators
	int PipeShafts; //number of pipe shafts
	int StairsNum; //number of stairwells
	csArray<Floor*> FloorArray; //pointer array to floor objects
	csArray<Elevator*> ElevatorArray; //pointer array to elevator objects
	bool RenderOnly; //skip sim processing and only render graphics
    bool InputOnly; //skip sim processing and only run input and rendering code
    bool IsFalling; //make user fall
	double FallRate; //falling rate
    bool InStairwell; //true if user is in a stairwell
	bool InElevator; //true is user is in an elevator
    int ElevatorNumber; //number of currently selected elevator
	bool FrameLimiter; //frame limiter toggle
    int FrameRate; //max frame rate
	double FPSModifier; //modification value for FPS changes
	bool FrameSync; //synchronize movement to frame rate
	bool EnableCollisions; //turns collisions on/off
	double HorizScale; //horizontal X/Z scaling multiplier (in feet). Normally is 1
	csStringArray UserVariable;
	bool IsBuildingsEnabled; //contains status of buildings object
	bool IsColumnFrameEnabled; //contains status of column frame object
	bool IsExternalEnabled; //contains status of external object
	bool IsLandscapeEnabled; //contains status of landscape object
	bool IsSkyboxEnabled; //contains status of skybox object
	double FPS; //current frame rate

	//File I/O
	csString BuildingFile;
	csStringArray BuildingData;

	//public functions
	SBS();
	~SBS();
	void Report (const char* msg, ...);
	bool ReportError (const char* msg, ...);
	void Wait(long Milliseconds);
	bool LoadTexture(const char *filename, const char *name);
	bool Initialize(int argc, const char* const argv[], const char *windowtitle);
 	void Start();
	void Run();
	int CreateSky();
	void AddLight(const char *name, double x, double y, double z, double radius, double r, double g, double b);
	int AddWallMain(csRef<iThingFactoryState> dest, const char *texture, double x1, double z1, double x2, double z2, double height_in1, double height_in2, double altitude1, double altitude2, double tw, double th, bool revX, bool revY, bool revZ, bool DrawBothSides = true);
	int AddFloorMain(csRef<iThingFactoryState> dest, const char *texture, double x1, double z1, double x2, double z2, double altitude1, double altitude2, double tw, double th);
	bool HandleEvent(iEvent& Event);
	void SetupFrame();
	void FinishFrame();
	void GetInput();
	void Render();
	void PushFrame();
	int CreateWallBox(csRef<iThingFactoryState> dest, const char *texture, double x1, double x2, double z1, double z2, double height_in, double voffset, double tw, double th);
	int CreateWallBox2(csRef<iThingFactoryState> dest, const char *texture, double CenterX, double CenterZ, double WidthX, double LengthZ, double height_in, double voffset, double tw, double th);
	int AddTriangleWall(csRef<iThingFactoryState> dest, const char *texture, double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double tw, double th, bool revX, bool revY, bool revZ, bool IsExternal);
	int AddCustomWall(csRef<iThingFactoryState> dest, const char *texture, csPoly3D &varray, double tw, double th, bool revX, bool revY, bool revZ, bool IsExternal);
	int AddCustomFloor(csRef<iThingFactoryState> dest, const char *texture, csPoly3D &varray, double tw, double th, bool revX, bool revY, bool revZ, bool IsExternal);
	csString Calc(const char *expression);
	csVector2 GetExtents(csPoly3D &varray, int coord);
	void InitMeshes();
	void EnableBuildings(bool value);
	void EnableLandscape(bool value);
	void EnableExternal(bool value);
	void EnableColumnFrame(bool value);
	void EnableSkybox(bool value);
	int GetFloorNumber(double altitude);
	double GetDistance(double x1, double x2, double z1, double z2);
	void Fall();

	//file loader functions
	int LoadBuilding(const char * filename);
	int LoadDataFile(const char * filename);

	//Meshes
	csRef<iMeshWrapper> Buildings; //building mesh
		csRef<iMeshObject> Buildings_object;
		csRef<iMeshObjectFactory> Buildings_factory;
		csRef<iThingFactoryState> Buildings_state;

	csRef<iMeshWrapper> External; //external mesh
		csRef<iMeshObject> External_object;
		csRef<iMeshObjectFactory> External_factory;
		csRef<iThingFactoryState> External_state;

	csRef<iMeshWrapper> Landscape; //landscape mesh
		csRef<iMeshObject> Landscape_object;
		csRef<iMeshObjectFactory> Landscape_factory;
		csRef<iThingFactoryState> Landscape_state;

	csRef<iMeshWrapper> ColumnFrame; //column frame mesh
		csRef<iMeshObject> ColumnFrame_object;
		csRef<iMeshObjectFactory> ColumnFrame_factory;
		csRef<iThingFactoryState> ColumnFrame_state;

	csRef<iMeshWrapper> SkyBox; //skybox mesh
		csRef<iMeshObject> SkyBox_object;
		csRef<iMeshObjectFactory> SkyBox_factory;
		csRef<iThingFactoryState> SkyBox_state;

private:

	csEventID FocusGained;
	csEventID FocusLost;
	csEventID Process;
	csEventID FinalProcess;
	csEventID KeyboardDown;

	//fps
	int fps_frame_count;
	int fps_tottime;

	//private functions
	void PrintBanner();
};
