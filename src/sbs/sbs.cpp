/*
	Scalable Building Simulator - Core
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

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreFileSystem.h>
#include <OgreConfigFile.h>
#include <OgreTimer.h>
#include "OgreStringVector.h"
#ifndef DISABLE_SOUND
#include <fmod.hpp>
#endif
#include <OgreBulletDynamicsRigidBody.h>
#include "globals.h"
#include "sbs.h"
#include "manager.h"
#include "camera.h"
#include "dynamicmesh.h"
#include "floor.h"
#include "elevator.h"
#include "elevatorcar.h"
#include "shaft.h"
#include "stairs.h"
#include "action.h"
#include "person.h"
#include "texman.h"
#include "light.h"
#include "wall.h"
#include "control.h"
#include "trigger.h"
#include "soundsystem.h"
#include "sound.h"
#include "model.h"
#include "primitive.h"
#include "custom.h"
#include "timer.h"
#include "profiler.h"
#include "controller.h"
#include "callstation.h"
#include "doorsystem.h"
#include "gitrev.h"
#include "buttonpanel.h"
#include "polymesh.h"
#include "utility.h"
#include "geometry.h"
#include "escalator.h"
#include "map.h"
#include "shape.h"
#include "reverb.h"

namespace SBS {

SBS::SBS(Ogre::SceneManager* mSceneManager, FMOD::System *fmodsystem, int instance_number, const Vector3 &area_min, const Vector3 &area_max) : Object(0)
{
	sbs = this;
	this->mSceneManager = mSceneManager;

	version = "1.1.0." + ToString(GIT_REV);
	version_state = "Alpha";

	//root object needs to self-register
	ObjectCount = 0;
	RegisterObject(this);
	InstanceNumber = instance_number;

	//set up SBS object
	SetValues("SBS", "SBS", true);

	mRoot = Ogre::Root::getSingletonPtr();

	//load config file
	configfile = new Ogre::ConfigFile();
	configfile->load("skyscraper.ini");

	//initialize variables
	BuildingName = "";
	BuildingDesigner = "";
	BuildingLocation = "";
	BuildingDescription = "";
	BuildingVersion = "";
	IsRunning = false;
	Floors = 0;
	Basements = 0;
	IsFalling = false;
	InStairwell = false;
	InElevator = false;
	IsBuildingsEnabled = false;
	IsExternalEnabled = false;
	IsLandscapeEnabled = false;
	IsSkyboxEnabled = false;
	fps_frame_count = 0;
	fps_tottime = 0;
	FPS = 0;
	AutoShafts = GetConfigBool("Skyscraper.SBS.AutoShafts", true);
	AutoStairs = GetConfigBool("Skyscraper.SBS.AutoStairs", true);
	ElevatorSync = false;
	ElevatorNumber = 1;
	CarNumber = 1;
	delta = 0.01;
	ProcessElevators = GetConfigBool("Skyscraper.SBS.ProcessElevators", true);
	remaining_delta = 0;
	start_time = 0;
	running_time = 0;
	InShaft = false;
	DeleteColliders = false;
	soundcount = 0;
	reverbcount = 0;
	Verbose = GetConfigBool("Skyscraper.SBS.Verbose", false);
	InterfloorOnTop = false;
	FastDelete = false;
	SkyBox = 0;
	Landscape = 0;
	External = 0;
	Buildings = 0;
	current_time = 0;
	current_virtual_time = 0;
	elapsed_time = 0;
	average_time = 0;
	timer = new Ogre::Timer();
	AmbientR = 1;
	AmbientG = 1;
	AmbientB = 1;
	OldAmbientR = 1;
	OldAmbientG = 1;
	OldAmbientB = 1;
	TexelOverride = false;
	enable_profiling = false;
	enable_advanced_profiling = false;
	SkyName = GetConfigString("Skyscraper.SBS.SkyName", "noon");
	ShaftDisplayRange = GetConfigInt("Skyscraper.SBS.ShaftDisplayRange", 3);
	StairsDisplayRange = GetConfigInt("Skyscraper.SBS.StairsDisplayRange", 5);
	ShaftOutsideDisplayRange = GetConfigInt("Skyscraper.SBS.ShaftOutsideDisplayRange", 3);
	StairsOutsideDisplayRange = GetConfigInt("Skyscraper.SBS.StairsOutsideDisplayRange", 3);
	FloorDisplayRange = GetConfigInt("Skyscraper.SBS.FloorDisplayRange", 3);
	SmoothFrames = GetConfigInt("Skyscraper.SBS.SmoothFrames", 200);
	RenderOnStartup = GetConfigBool("Skyscraper.SBS.RenderOnStartup", false);
	RandomActivity = GetConfigBool("Skyscraper.SBS.RandomActivity", false);
	Malfunctions = GetConfigBool("Skyscraper.SBS.Malfunctions", false);
	power_state = true;
	Lobby = 0;
	MapGenerator = 0;
	auto_bounds = true;

	//create utility object
	utility = new Utility(this);

	//create polymesh (geometry processor) object
	polymesh = new PolyMesh(this);

	//create geometry controller object
	geometry = new GeometryController(this);

	//set padding factor for meshes
	Ogre::MeshManager::getSingleton().setBoundsPaddingFactor(0.0);

	camera = 0;
	Buildings = 0;
	External = 0;
	Landscape = 0;
	mWorld = 0;
	soundsystem = 0;
	area_trigger = 0;
	texturemanager = 0;
	floor_manager = 0;
	elevator_manager = 0;
	shaft_manager = 0;
	stairwell_manager = 0;
	door_manager = 0;
	revolvingdoor_manager = 0;
	vehicle_manager = 0;
	controller_manager = 0;
	teleporter_manager = 0;

	//Print SBS banner
	PrintBanner();

	//add instance number to reports
	InstancePrompt = ToString(InstanceNumber) + ">";

	//create main engine area trigger
	SetBounds(area_min, area_max);
	bounds = Ogre::AxisAlignedBox::BOX_NULL;
	bounds_set = false;

	//create sound system object if sound is enabled
	if (fmodsystem)
		soundsystem = new SoundSystem(this, fmodsystem);
}

void SBS::Initialize()
{
	//create texture manager
	texturemanager = new TextureManager(this);

	//set up physics
	Ogre::AxisAlignedBox box (Vector3::ZERO, Vector3::ZERO);
	mWorld = new OgreBulletDynamics::DynamicsWorld(mSceneManager, box, Vector3::ZERO, true);
	mWorld->setAllowedCcdPenetration(0);

	/*debugDrawer = new OgreBulletCollisions::DebugDrawer();
	debugDrawer->setDrawWireframe(true);
	mWorld->setDebugDrawer(debugDrawer);
	Ogre::SceneNode *node = mSceneManager->getRootSceneNode()->createChildSceneNode("debugDrawer", Vector3::ZERO);
	node->attachObject(static_cast<Ogre::SimpleRenderable*> (debugDrawer));
	*/

	//mount sign texture packs
	GetUtility()->Mount("signs-sans.zip", "signs/sans");
	GetUtility()->Mount("signs-sans_bold.zip", "signs/sans_bold");
	GetUtility()->Mount("signs-sans_cond.zip", "signs/sans_cond");
	GetUtility()->Mount("signs-sans_cond_bold.zip", "signs/sans_cond_bold");

	//create object meshes
	Buildings = new MeshObject(this, "Buildings");
	External = new MeshObject(this, "External");
	Landscape = new MeshObject(this, "Landscape");
	//Landscape->tricollider = false;

	//create manager objects
	floor_manager = new FloorManager(this);
	elevator_manager = new ElevatorManager(this);
	shaft_manager = new ShaftManager(this);
	stairwell_manager = new StairwellManager(this);
	door_manager = new DoorManager(this);
	revolvingdoor_manager = new RevolvingDoorManager(this);
	vehicle_manager = new VehicleManager(this);
	controller_manager = new ControllerManager(this);
	teleporter_manager = new TeleporterManager(this);

	//create camera object
	this->camera = new Camera(this);

	//create map generator object
	MapGenerator = new Map(this, "Map Generator");

	//report ready status
	Report("Ready");
}

SBS::~SBS()
{
	//engine destructor

	Report("Deleting simulator objects...");

	FastDelete = true;

	//delete people
	for (size_t i = 0; i < PersonArray.size(); i++)
	{
		if (PersonArray[i])
		{
			PersonArray[i]->parent_deleting = true;
			delete PersonArray[i];
		}
		PersonArray[i] = 0;
	}

	//delete controls
	for (size_t i = 0; i < ControlArray.size(); i++)
	{
		if (ControlArray[i])
		{
			ControlArray[i]->parent_deleting = true;
			delete ControlArray[i];
		}
		ControlArray[i] = 0;
	}

	//delete triggers
	for (size_t i = 0; i < TriggerArray.size(); i++)
	{
		if (TriggerArray[i])
		{
			TriggerArray[i]->parent_deleting = true;
			delete TriggerArray[i];
		}
		TriggerArray[i] = 0;
	}

	//delete models
	for (size_t i = 0; i < ModelArray.size(); i++)
	{
		if (ModelArray[i])
		{
			ModelArray[i]->parent_deleting = true;
			delete ModelArray[i];
		}
		ModelArray[i] = 0;
	}

	//delete primitives
	for (size_t i = 0; i < PrimArray.size(); i++)
	{
		if (PrimArray[i])
		{
			PrimArray[i]->parent_deleting = true;
			delete PrimArray[i];
		}
		PrimArray[i] = 0;
	}

	//delete custom objects
	for (size_t i = 0; i < CustomObjectArray.size(); i++)
	{
		if (CustomObjectArray[i])
		{
			CustomObjectArray[i]->parent_deleting = true;
			delete CustomObjectArray[i];
		}
		CustomObjectArray[i] = 0;
	}

	//delete lights
	for (size_t i = 0; i < lights.size(); i++)
	{
		if (lights[i])
		{
			lights[i]->parent_deleting = true;
			delete lights[i];
		}
		lights[i] = 0;
	}

	//delete map generator
	if (MapGenerator)
		delete MapGenerator;
	MapGenerator = 0;

	//delete camera object
	if (camera)
	{
		camera->parent_deleting = true;
		delete camera;
	}
	camera = 0;

	//delete manager objects
	if (floor_manager)
	{
		floor_manager->parent_deleting = true;
		delete floor_manager;
	}
	floor_manager = 0;

	if (elevator_manager)
	{
		elevator_manager->parent_deleting = true;
		delete elevator_manager;
	}
	elevator_manager = 0;

	if (shaft_manager)
	{
		shaft_manager->parent_deleting = true;
		delete shaft_manager;
	}
	shaft_manager = 0;

	if (stairwell_manager)
	{
		stairwell_manager->parent_deleting = true;
		delete stairwell_manager;
	}
	stairwell_manager = 0;

	if (door_manager)
	{
		door_manager->parent_deleting = true;
		delete door_manager;
	}
	door_manager = 0;

	if (revolvingdoor_manager)
	{
		revolvingdoor_manager->parent_deleting = true;
		delete revolvingdoor_manager;
	}
	revolvingdoor_manager = 0;

	if (vehicle_manager)
	{
		vehicle_manager->parent_deleting = true;
		delete vehicle_manager;
	}
	vehicle_manager = 0;

	if (controller_manager)
	{
		controller_manager->parent_deleting = true;
		delete controller_manager;
	}
	controller_manager = 0;

	if (teleporter_manager)
	{
		teleporter_manager->parent_deleting = true;
		delete teleporter_manager;
	}
	teleporter_manager = 0;

	//delete sounds
	for (size_t i = 0; i < sounds.size(); i++)
	{
		if (sounds[i])
		{
			sounds[i]->parent_deleting = true;
			delete sounds[i];
		}
		sounds[i] = 0;
	}

	//delete actions
	for (size_t i = 0; i < ActionArray.size(); i++)
	{
		if (ActionArray[i])
			delete ActionArray[i];
		ActionArray[i] = 0;
	}

	//delete mesh objects
	if (SkyBox)
	{
		SkyBox->parent_deleting = true;
		delete SkyBox;
	}
	SkyBox = 0;

	if (Landscape)
	{
		Landscape->parent_deleting = true;
		delete Landscape;
	}
	Landscape = 0;

	if (External)
	{
		External->parent_deleting = true;
		delete External;
	}
	External = 0;

	if (Buildings)
	{
		Buildings->parent_deleting = true;
		delete Buildings;
	}
	Buildings = 0;

	//delete sound system
	if (soundsystem)
	{
		soundsystem->parent_deleting = true;
		delete soundsystem;
	}
	soundsystem = 0;

	//delete texture manager
	if (texturemanager)
		delete texturemanager;
	texturemanager = 0;

	//delete main area trigger
	if (area_trigger)
	{
		area_trigger->parent_deleting = true;
		delete area_trigger;
	}
	area_trigger = 0;

	//delete physics objects
	if (mWorld)
	{
		//delete mWorld->getDebugDrawer();
		//mWorld->setDebugDrawer(0);
		delete mWorld;
	}
	mWorld = 0;

	ObjectArray.clear();

	if (utility)
		delete utility;
	utility = 0;

	if (polymesh)
		delete polymesh;
	polymesh = 0;

	if (geometry)
		delete geometry;
	geometry = 0;

	if (timer)
		delete timer;
	timer = 0;

	if (configfile)
		delete configfile;
	configfile = 0;

	Report("Exiting");

	//clear self reference
	sbs = 0;
}

bool SBS::Start()
{
	//Post-init startup code goes here, before the runloop

	//prepare 3D geometry for use
	Prepare();

	//if no bounds trigger was created, set it according to generated engine bounds
	if (!area_trigger && !bounds.isNull())
	{
		Vector3 min = bounds.getMinimum();
		Vector3 max = bounds.getMaximum();
		if (auto_bounds == true)
		{
			//if using auto bounds, leave vertical space for camera
			min.y -= 100;
			max.y += 100;
		}
		SetBounds(min, max);
	}

	//free text texture memory
	texturemanager->FreeTextureBoxes();

	//reset building state
	ResetState();

	//initialize objects (cascades down through entire object tree)
	Init();

	//play looping global sounds
	for (size_t i = 0; i < sounds.size(); i++)
	{
		if (sounds[i])
		{
			if (sounds[i]->GetLoopState() == true)
				sounds[i]->Play();
		}
	}

	//enable random activity if specified
	if (RandomActivity == true)
		EnableRandomActivity(true);

	//enable malfunctions if specified
	if (Malfunctions == true)
		EnableMalfunctions(true);

	//turn on reverbs
	for (size_t i = 0; i < reverbs.size(); i++)
	{
		if (reverbs[i])
			reverbs[i]->Enabled(true);
	}

	//print a memory report
	if (Verbose == true)
		MemoryReport();

	IsRunning = true;

	return true;
}

void SBS::PrintBanner()
{
	Report("");
	Report(" Scalable Building Simulator " + version + " " + version_state);
	Report(" Copyright (C)2004-2025 Ryan Thoryk");
	Report(" This software comes with ABSOLUTELY NO WARRANTY. This is free");
	Report(" software, and you are welcome to redistribute it under certain");
	Report(" conditions. For details, see the file named LICENSE\n");
}

bool SBS::Loop(bool loading, bool isready)
{
	//Main simulator loop
	SBS_PROFILE_MAIN("SBS");

	if (!camera)
		return false;

	bool status = true;

	if (RenderOnStartup == true && (loading == true || isready == false))
		Prepare(false);

	if (loading == true)
		return true;

	//This makes sure all timer steps are the same size, in order to prevent the physics from changing
	//depending on frame rate

	unsigned long timing;

	if (SmoothFrames > 0)
		timing = GetAverageTime();
	else
		timing = GetElapsedTime();

	Real elapsed = Real(timing) / 1000.0;

	//calculate start and running time
	if (start_time == 0)
		start_time = GetRunTime() / 1000.0;
	running_time = (GetRunTime() / 1000.0) - start_time;

	//move camera or update character movement
	camera->MoveCharacter();

	//update physics
	if (camera->EnableBullet == true)
	{
		if (enable_advanced_profiling == false)
			ProfileManager::Start_Profile("Collisions/Physics");
		else
			ProfileManager::Start_Profile("Bullet");
		mWorld->stepSimulation(elapsed, 0);
		ProfileManager::Stop_Profile();
	}

	//sync camera to physics
	camera->Sync();

	//update sound
	if (soundsystem)
	{
		bool result = soundsystem->Loop();
		if (result == false)
			status = false;
	}

	elapsed += remaining_delta;

	//limit the elapsed value to prevent major slowdowns during debugging
	if (elapsed > .5)
		elapsed = .5;

	ProfileManager::Start_Profile("Simulator Loop");
	while (elapsed >= delta)
	{
		//Determine floor that the camera is on
		camera->UpdateCameraFloor();

		//process child object dynamic runloops
		LoopChildren();

		camera->CheckObjects();

		//process auto areas
		CheckAutoAreas();

		elapsed -= delta;
	}
	remaining_delta = elapsed;

	//process timers
	ProcessTimers();

	//process engine boundary trigger
	if (area_trigger)
		area_trigger->Loop();

	ProfileManager::Stop_Profile();

	//process camera loop
	camera->Loop();

	return status;
}

void SBS::CalculateFrameRate()
{
	//calculate frame rate
	fps_tottime += elapsed_time;
	fps_frame_count++;
	if (fps_tottime > 500)
	{
		FPS = (Real (fps_frame_count) * 1000.) / Real (fps_tottime);
		fps_frame_count = 0;
		fps_tottime = 0;
	}
}

void SBS::EnableBuildings(bool value)
{
	//turns buildings on/off

	if (Buildings)
		Buildings->Enabled(value);
	IsBuildingsEnabled = value;
}

void SBS::EnableLandscape(bool value)
{
	//turns landscape on/off
	if (Landscape)
		Landscape->Enabled(value);
	IsLandscapeEnabled = value;
}

void SBS::EnableExternal(bool value)
{
	//turns external on/off

	if (External)
		External->Enabled(value);
	IsExternalEnabled = value;
}

void SBS::EnableSkybox(bool value)
{
	//turns skybox on/off
	if (SkyBox)
	{
		SkyBox->Enabled(value);
		IsSkyboxEnabled = value;
	}
	else
		IsSkyboxEnabled = true;
}

void SBS::CreateSky()
{
	//create skybox

	//only create skybox if first engine instance
	if (InstanceNumber > 0)
		return;

	bool result = GetUtility()->Mount("sky-" + SkyName + ".zip", "sky");

	//load textures
	if (result == true)
	{
		SetLighting();
		texturemanager->LoadTexture("sky/up.jpg", "SkyTop", -1, -1, false, false, false, 0);
		texturemanager->LoadTexture("sky/down.jpg", "SkyBottom", -1, -1, false, false, false, 0);
		texturemanager->LoadTexture("sky/left.jpg", "SkyLeft", 1, 1, false, false, false, 0);
		texturemanager->LoadTexture("sky/right.jpg", "SkyRight", 1, 1, false, false, false, 0);
		texturemanager->LoadTexture("sky/front.jpg", "SkyFront", 1, 1, false, false, false, 0);
		texturemanager->LoadTexture("sky/back.jpg", "SkyBack", 1, 1, false, false, false, 0);
		ResetLighting();
	}

	//create a mesh for the skybox
	SkyBox = new MeshObject(this, "SkyBox");
	SkyBox->create_collider = false; //disable collisions on skybox

	//create a skybox size that extends by default 30 miles (30 * 5280 ft) in each direction
	Real skysize = GetConfigInt("Skyscraper.SBS.HorizonDistance", 30) * 5280.0;

	//reset texture mapping to default
	texturemanager->ResetTextureMapping(true);

	//set textures that the skybox will use
	texturemanager->SetTextureOverride("SkyFront", "SkyBack", "SkyLeft", "SkyRight", "SkyTop", "SkyBottom");

	//create a wall box, using the skysize value
	polymesh->CreateWallBox(SkyBox, "SkyBox", "SkyFront", -skysize, skysize, -skysize, skysize, skysize, -skysize / 2, 1, 1, true, true, true, true, false);

	//restore previous texture mapping
	texturemanager->ResetTextureMapping();
}

int SBS::GetFloorNumber(Real altitude, int lastfloor, bool checklastfloor)
{
	//Returns floor number located at a specified altitude

	if (GetTotalFloors() == 0)
		return 0;

	//check to see if altitude is below bottom floor
	if (GetFloor(-Basements))
	{
		if (altitude < GetFloor(-Basements)->Altitude)
			return -Basements;
	}

	//if checklastfloor is specified, compare altitude with lastfloor
	if (checklastfloor == true && GetFloor(lastfloor))
	{
		Real lastfloor_altitude = GetFloor(lastfloor)->Altitude;
		Real upperfloor_altitude;
		if (lastfloor < Floors - 1)
		{
			if (GetFloor(lastfloor + 1))
				upperfloor_altitude = GetFloor(lastfloor + 1)->Altitude;
			else
				upperfloor_altitude = GetFloor(lastfloor)->Altitude + GetFloor(lastfloor)->FullHeight();
		}
		else
			upperfloor_altitude = GetFloor(lastfloor)->Altitude + GetFloor(lastfloor)->FullHeight();

		if (upperfloor_altitude > altitude && lastfloor_altitude <= altitude)
			return lastfloor;
		else
		{
			//if altitude is below lastfloor, search downwards; otherwise search upwards
			if (altitude < lastfloor_altitude)
			{
				for (int i = lastfloor - 1; i >= -Basements; i--)
				{
					if (!GetFloor(i + 1) || !GetFloor(i))
						continue;
					if (GetFloor(i + 1)->Altitude > altitude && GetFloor(i)->Altitude <= altitude)
						return i;
				}
			}
			else if (altitude >= upperfloor_altitude)
			{
				for (int i = lastfloor + 1; i < Floors; i++)
				{
					if (!GetFloor(i))
						continue;
					if (GetFloor(i - 1))
					{
						if (GetFloor(i - 1)->Altitude <= altitude && GetFloor(i)->Altitude > altitude)
							return i - 1;
					}
					if (i == Floors - 1 && GetFloor(i)->Altitude <= altitude)
						return i; //return top floor if on top
				}
			}
		}
	}

	//otherwise do a slow linear search through floors
	for (int i = -Basements + 1; i < Floors; i++)
	{
		if (!GetFloor(i))
			continue;

		//check to see if altitude is within a floor (between the current floor's base and
		//the lower floor's base)
		if (GetFloor(i - 1))
		{
			if ((GetFloor(i)->Altitude > altitude) && (GetFloor(i - 1)->Altitude <= altitude))
				return i - 1;
		}
		//check to see if altitude is above top floor's altitude
		if ((i == Floors - 1) && (altitude > GetFloor(i)->Altitude))
			return i;
	}
	return 0;
}

Shaft* SBS::CreateShaft(int number, Real CenterX, Real CenterZ, int startfloor, int endfloor)
{
	//create a shaft object

	return shaft_manager->Create(number, CenterX, CenterZ, startfloor, endfloor);
}

Stairwell* SBS::CreateStairwell(int number, Real CenterX, Real CenterZ, int startfloor, int endfloor)
{
	//create a stairwell object

	return stairwell_manager->Create(number, CenterX, CenterZ, startfloor, endfloor);
}

Elevator* SBS::NewElevator(int number)
{
	//create a new elevator object

	return elevator_manager->Create(number);
}

Floor* SBS::NewFloor(int number)
{
	//create a new floor object

	return floor_manager->Create(number);
}

Vehicle* SBS::NewVehicle(int number)
{
	//create a new vehicle object

	return vehicle_manager->Create(number);
}

DispatchController* SBS::NewController(int number)
{
	//create a new controller object

	return controller_manager->Create(number);
}

int SBS::GetElevatorCount()
{
	//return the number of elevators
	return elevator_manager->GetCount();
}

int SBS::GetVehicleCount()
{
	//return the number of vehicles
	return vehicle_manager->GetCount();
}

int SBS::GetTotalFloors()
{
	//return the number of floors, including basements
	return floor_manager->GetCount();
}

int SBS::GetShaftCount()
{
	//return the number of shafts
	return shaft_manager->GetCount();
}

int SBS::GetStairwellCount()
{
	//return the number of stairs
	return stairwell_manager->GetCount();
}

int SBS::GetControllerCount()
{
	//return the number of shafts
	return controller_manager->GetCount();
}

Floor* SBS::GetFloor(int number)
{
	//return pointer to floor object

	return floor_manager->Get(number);
}

Elevator* SBS::GetElevator(int number)
{
	//return pointer to elevator object

	return elevator_manager->Get(number);
}

Shaft* SBS::GetShaft(int number)
{
	//return pointer to shaft object

	return shaft_manager->Get(number);
}

Stairwell* SBS::GetStairwell(int number)
{
	//return pointer to stairs object

	return stairwell_manager->Get(number);
}

Vehicle* SBS::GetVehicle(int number)
{
	//return pointer to vehicle object

	return vehicle_manager->Get(number);
}

DispatchController* SBS::GetController(int number)
{
	//return pointer to controller object

	return controller_manager->Get(number);
}

void SBS::EnableFloorRange(int floor, int range, bool value, bool enablegroups, int shaftnumber, int stairsnumber)
{
	//turn on/off a range of floors
	//if range is 3, show shaft on current floor (floor), and 1 floor below and above (3 total floors)
	//if range is 1, show only the current floor (floor)

	SBS_PROFILE("SBS::EnableFloorRange");

	//range must be greater than 0
	if (range < 1)
		range = 1;

	//range must be an odd number; if it's even, then add 1
	if (IsEven(range) == true)
		range++;

	//floor must be valid
	if (!IsValidFloor(floor))
		return;

	int additionalfloors;
	if (range > 1)
		additionalfloors = (range - 1) / 2;
	else
		additionalfloors = 0;

	Shaft *shaft = 0;
	Stairwell *stairwell = 0;

	if (shaftnumber > 0)
		shaft = GetShaft(shaftnumber);
	if (stairsnumber > 0)
		stairwell = GetStairwell(stairsnumber);

	//disable floors 1 floor outside of range, unless part of group
	if (value == true)
	{
		int floorval = floor - additionalfloors - 1;
		if (IsValidFloor(floorval) && GetFloor(floor)->IsInGroup(floorval) == false)
			GetFloor(floorval)->Enabled(false);

		floorval = floor + additionalfloors + 1;
		if (IsValidFloor(floorval) && GetFloor(floor)->IsInGroup(floorval) == false)
			GetFloor(floorval)->Enabled(false);
	}

	//enable floors within range
	for (int i = floor - additionalfloors; i <= floor + additionalfloors; i++)
	{
		Floor *floorobj = GetFloor(i);

		if (floorobj)
		{
			if (shaft)
			{
				//if a shaft is specified, only show the floor if it is in the related shaft's ShowFloorsList array
				if (shaft->ShowFloors > 0)
				{
					bool showfloor = shaft->IsShowFloor(i);

					if (showfloor == true && value == true)
					{
						if (floorobj->IsEnabled() == false)
						{
							floorobj->Enabled(true);
							if (enablegroups == true)
								floorobj->EnableGroup(true);
						}
					}
					else
					{
						//only disable floor if it hasn't been enabled separately by a related group
						if (floorobj->EnabledGroup == true)
						{
							//for now check to see if the group floor is a ShowFloor
							if (shaft->IsShowFloor(floorobj->EnabledGroup_Floor) == true)
								return;
						}

						if (floorobj->IsEnabled() == true)
						{
							floorobj->Enabled(false);
							if (enablegroups == true)
								floorobj->EnableGroup(false);
						}
					}
				}
			}
			else if (stairwell)
			{
				//if a stairwell is specified, only show the floor if it is in the related stairwell's ShowFloorsList array
				if (stairwell->ShowFloors == true)
				{
					bool showfloor = stairwell->IsShowFloor(i);

					if (showfloor == true && value == true)
					{
						if (floorobj->IsEnabled() == false)
						{
							floorobj->Enabled(true);
							if (enablegroups == true)
								floorobj->EnableGroup(true);
						}
					}
					else
					{
						//only disable floor if it hasn't been enabled separately by a related group
						if (floorobj->EnabledGroup == true)
						{
							//for now check to see if the group floor is a ShowFloor
							if (stairwell->IsShowFloor(floorobj->EnabledGroup_Floor) == true)
								return;
						}

						if (floorobj->IsEnabled() == true)
						{
							floorobj->Enabled(false);
							if (enablegroups == true)
								floorobj->EnableGroup(false);
						}
					}
				}
			}
			else
			{
				floorobj->Enabled(value);
				if (enablegroups == true)
					floorobj->EnableGroup(value);
			}
		}
	}
}

bool SBS::RegisterTimerCallback(TimerObject *timer)
{
	//register a timer object for callbacks

	if (!timer)
		return false;

	for (size_t i = 0; i < timercallbacks.size(); i++)
	{
		if (timercallbacks[i] == timer)
			return false;
	}

	//if timer isn't already in the array, add it
	timercallbacks.emplace_back(timer);

	return true;
}

bool SBS::UnregisterTimerCallback(TimerObject *timer)
{
	if (!timer)
		return false;

	return RemoveArrayElement(timercallbacks, timer);
}

void SBS::ProcessTimers()
{
	SBS_PROFILE("SBS::ProcessTimers");

	//process all registered timers
	for (size_t i = 0; i < timercallbacks.size(); i++)
	{
		if (timercallbacks[i])
			timercallbacks[i]->Loop();
	}
}

int SBS::GetTimerCallbackCount()
{
	//return the number of registered call button callbacks
	return (int)timercallbacks.size();
}

void SBS::AddFloorAutoArea(Vector3 start, Vector3 end)
{
	//adds an auto area that enables/disables floors

	AutoArea newarea;
	newarea.start = start;
	newarea.end = end;
	newarea.inside = false;
	newarea.camerafloor = 0;
	FloorAutoArea.emplace_back(newarea);
}

void SBS::CheckAutoAreas()
{
	//check all automatic areas

	SBS_PROFILE("SBS::CheckAutoAreas");

	Vector3 position = camera->GetPosition();
	int floor = camera->CurrentFloor;

	for (size_t i = 0; i < FloorAutoArea.size(); i++)
	{
		//reset inside value if floor changed
		if (FloorAutoArea[i].camerafloor != floor)
			FloorAutoArea[i].inside = false;

		if (GetUtility()->InBox(FloorAutoArea[i].start, FloorAutoArea[i].end, position) == true && FloorAutoArea[i].inside == false)
		{
			//user moved into box; enable floors
			FloorAutoArea[i].inside = true;
			FloorAutoArea[i].camerafloor = floor;
			if (floor > -Basements)
			{
				GetFloor(floor - 1)->Enabled(true);
				GetFloor(floor - 1)->EnableGroup(true);
			}
			GetFloor(floor)->Enabled(true);
			GetFloor(floor)->EnableGroup(true);
			if (floor < Floors - 1)
			{
				GetFloor(floor + 1)->Enabled(true);
				GetFloor(floor + 1)->EnableGroup(true);
			}
		}
		if (GetUtility()->InBox(FloorAutoArea[i].start, FloorAutoArea[i].end, position) == false && FloorAutoArea[i].inside == true)
		{
			//user moved out of box; disable floors except current
			FloorAutoArea[i].inside = false;
			FloorAutoArea[i].camerafloor = 0;
			if (floor > -Basements)
			{
				GetFloor(floor - 1)->Enabled(false);
				GetFloor(floor - 1)->EnableGroup(false);
			}
			if (floor < Floors - 1)
			{
				GetFloor(floor + 1)->Enabled(false);
				GetFloor(floor + 1)->EnableGroup(false);
			}
			GetFloor(floor)->Enabled(true);
			GetFloor(floor)->EnableGroup(true);
		}
	}
}

int SBS::GetMeshCount()
{
	//return total number of mesh objects
	return (int)meshes.size();
}

Sound* SBS::AddSound(const std::string &name, const std::string &filename, const Vector3 &position, bool loop, Real volume, int speed, Real min_distance, Real max_distance, Real doppler_level, Real cone_inside_angle, Real cone_outside_angle, Real cone_outside_volume, const Vector3 &direction)
{
	//create a looping sound object
	Sound *sound = new Sound(this, name, false);
	sounds.emplace_back(sound);

	//set parameters and play sound
	sound->SetPosition(position);
	sound->SetDirection(direction);
	sound->SetVolume(volume);
	sound->SetSpeed(speed);
	sound->SetDistances(min_distance, max_distance);
	sound->SetDirection(direction);
	sound->SetDopplerLevel(doppler_level);
	sound->SetConeSettings(cone_inside_angle, cone_outside_angle, cone_outside_volume);
	sound->Load(filename);
	sound->SetLoopState(loop);
	if (loop && IsRunning == true)
		sound->Play();

	return sound;
}

std::vector<Sound*> SBS::GetSound(const std::string &name)
{
	//get sound by name

	std::string findname = name;
	SetCase(findname, false);
	std::vector<Sound*> soundlist;
	for (size_t i = 0; i < sounds.size(); i++)
	{
		if (sounds[i])
		{
			std::string name2 = sounds[i]->GetName();
			SetCase(name2, false);
			if (findname == name2)
				soundlist.emplace_back(sounds[i]);
		}
	}
	return soundlist;
}

int SBS::GetSoundCount()
{
	//return total number of allocated sounds
	return soundcount;
}

void SBS::IncrementSoundCount()
{
	soundcount++;
}

void SBS::DecrementSoundCount()
{
	soundcount--;
}

Reverb* SBS::AddReverb(const std::string &name, const std::string &type, const Vector3 &position, Real min_distance, Real max_distance)
{
	//create a reverb object
	Reverb *reverb = new Reverb(this, name, type, position, min_distance, max_distance, false);
	reverbs.emplace_back(reverb);
	return reverb;
}

Reverb* SBS::GetReverb(const std::string &name)
{
	//get reverb by name

	std::string findname = name;
	SetCase(findname, false);
	for (size_t i = 0; i < reverbs.size(); i++)
	{
		if (reverbs[i])
		{
			std::string name2 = reverbs[i]->GetName();
			SetCase(name2, false);
			if (findname == name2)
				return reverbs[i];
		}
	}
	return 0;
}

int SBS::GetTotalReverbCount()
{
	//return total number of allocated reverbs
	return reverbcount;
}

void SBS::IncrementReverbCount()
{
	reverbcount++;
}

void SBS::DecrementReverbCount()
{
	reverbcount--;
}

Real SBS::ToLocal(Real remote_value)
{
	return GetUtility()->ToLocal(remote_value);
}

Vector2 SBS::ToLocal(const Vector2& remote_value)
{
	return GetUtility()->ToLocal(remote_value);
}

Vector3 SBS::ToLocal(const Vector3& remote_value, bool rescale, bool flip_z)
{
	return GetUtility()->ToLocal(remote_value, rescale, flip_z);
}

Real SBS::ToRemote(Real local_value)
{
	return GetUtility()->ToRemote(local_value);
}

Vector2 SBS::ToRemote(const Vector2& local_value)
{
	return GetUtility()->ToRemote(local_value);
}

Vector3 SBS::ToRemote(const Vector3& local_value, bool rescale, bool flip_z)
{
	return GetUtility()->ToRemote(local_value, rescale, flip_z);
}

int SBS::GetObjectCount()
{
	//return number of registered SBS objects
	return ObjectCount;
}

Object* SBS::GetObject(int number)
{
	//return object pointer from global array
	if (number >= 0 && number < (int)ObjectArray.size())
		return ObjectArray[number];
	else
		return 0;
}

int SBS::RegisterObject(Object *object)
{
	//add object to global array
	ObjectCount++;
	ObjectArray.emplace_back(object);
	return (int)ObjectArray.size() - 1;
}

bool SBS::UnregisterObject(int number)
{
	//remove object
	//note - this doesn't delete the objects

	if (number < (int)ObjectArray.size())
	{
		if (ObjectArray[number])
		{
			if (ObjectArray[number]->GetNumber() == number)
			{
				std::vector<Object*> objects;
				objects.emplace_back(ObjectArray[number]);
				RemoveActionParent(objects);
				ObjectArray[number] = 0;
				ObjectCount--;
				return true;
			}
		}
	}
	return false;
}

bool SBS::IsValidFloor(int floor)
{
	//determine if a floor is valid

	if (GetFloor(floor))
		return true;
	return false;
}

std::string SBS::DumpState()
{
	//dump basic simulator state to a string

	std::string output = "SBS version: " + version + "\n";
	output.append("Instance number: " + ToString(InstanceNumber) + "\n");
	output.append("Building Name: " + BuildingName + "\n");
	output.append("Building Filename: " + BuildingFilename + "\n");
	output.append("Building Version: " + BuildingVersion + "\n");
	output.append("InStairwell: ");
	output.append(BoolToString(InStairwell));
	output.append("\n");
	output.append("InElevator: ");
	output.append(BoolToString(InElevator));
	output.append("\n");
	output.append("InShaft: ");
	output.append(BoolToString(InShaft));
	output.append("\n");
	output.append("CameraFloor: ");
	if (camera)
		output.append(ToString(camera->CurrentFloor));
	output.append("\n");
	output.append("ElevatorNumber: ");
	output.append(ToString(ElevatorNumber));
	output.append("\n");
	output.append("CarNumber: ");
	output.append(ToString(CarNumber));
	output.append("\n");
	output.append("ElevatorSync: ");
	output.append(BoolToString(ElevatorSync));
	output.append("\n");
	output.append("Running Time: ");
	output.append(TruncateNumber(running_time, 2));
	output.append("\n");
	output.append("BuildingsEnabled: ");
	output.append(BoolToString(IsBuildingsEnabled));
	output.append("\n");
	output.append("ExternalEnabled: ");
	output.append(BoolToString(IsExternalEnabled));
	output.append("\n");
	output.append("LandscapeEnabled: ");
	output.append(BoolToString(IsLandscapeEnabled));
	output.append("\n");
	output.append("SkyboxEnabled: ");
	output.append(BoolToString(IsSkyboxEnabled));
	output.append("\n");
	output.append("Verbose: ");
	output.append(BoolToString(Verbose));
	output.append("\n");
	output.append("InterfloorOnTop: ");
	output.append(BoolToString(InterfloorOnTop));
	output.append("\n");
	output.append("Object Count: ");
	output.append(ToString(ObjectCount));
	output.append("\n");
	if (camera)
	{
		output.append("Camera Floor: ");
		output.append(ToString(camera->CurrentFloor));
		output.append("\n");
		output.append("Camera Position: " + TruncateNumber(camera->GetPosition().x, 2) + ", " + TruncateNumber(camera->GetPosition().y, 2) + ", " + TruncateNumber(camera->GetPosition().z, 2) + "\n");
	}

	return output;
}

bool SBS::DeleteObject(Object *object)
{
	//object deletion routine
	//this should be called to delete a simulator object during runtime

	if (!object)
		return ReportError("Invalid object");

	std::string number = ToString(object->GetNumber());
	bool deleted = false;

	//don't delete permanent objects
	if (object->IsPermanent() == true)
		return ReportError("Cannot delete permanent object " + number);

	std::string type = object->GetType();

	//perform standard delete based on object type
	if (type == "Floor")
	{
		Floor *floor = static_cast<Floor*>(object);

		//make sure no shaft is dependent on this floor
		for (int i = 0; i < shaft_manager->GetCount(); i++)
		{
			Shaft *shaft = shaft_manager->GetIndex(i);
			if (shaft)
			{
				if (shaft->IsValidFloor(floor->Number) == true)
					return ReportError("Cannot delete floor " + ToString(floor->Number) + " - in use by shaft " + ToString(shaft->ShaftNumber));
			}
		}

		//make sure no stairwell is dependent on this floor
		for (int i = 0; i < stairwell_manager->GetCount(); i++)
		{
			Stairwell *stairwell = stairwell_manager->GetIndex(i);
			if (stairwell)
			{
				if (stairwell->IsValidFloor(floor->Number) == true)
					return ReportError("Cannot delete floor " + ToString(floor->Number) + " - in use by stairwell " + ToString(stairwell->StairsNum));
			}
		}

		//restrict deletions to only lowest/highest floors
		if (floor->Number >= 0 && GetFloor(floor->Number + 1))
			return ReportError("Only the highest floor can be deleted");

		if (floor->Number < 0 && GetFloor(floor->Number - 1))
			return ReportError("Only the lowest basement can be deleted");

		deleted = true;
	}
	else if (type == "Elevator")
		deleted = true;
	else if (type == "ButtonPanel")
	{
		if (object->GetParent()->GetType() == "ElevatorCar")
			deleted = true;
	}
	else if (type == "CallButton")
		deleted = true;
	else if (type == "DirectionalIndicator")
		deleted = true;
	else if (type == "Door")
		deleted = true;
	else if (type == "RevolvingDoor")
		deleted = true;
	else if (type == "ElevatorDoor")
		deleted = true;
	else if (type == "FloorIndicator")
		deleted = true;
	else if (type == "Shaft")
	{
		Shaft *shaft = static_cast<Shaft*>(object);

		//make sure no elevator is dependent on this shaft
		for (int i = 0; i < elevator_manager->GetCount(); i++)
		{
			Elevator *elev = elevator_manager->GetIndex(i);
			if (elev)
			{
				if (elev->AssignedShaft == shaft->ShaftNumber)
					return ReportError("Cannot delete shaft " + ToString(shaft->ShaftNumber) + " - in use by elevator " + ToString(elev->Number));
			}
		}

		deleted = true;
	}
	else if (type == "Sound")
		deleted = true;
	else if (type == "Stairwell")
		deleted = true;
	else if (type == "Wall")
		deleted = true;
	else if (type == "Model")
		deleted = true;
	else if (type == "Control")
		deleted = true;
	else if (type == "Trigger")
		deleted = true;
	else if (type == "DoorWrapper")
	{
		DoorWrapper* wrapper = static_cast<DoorWrapper*>(object);
		if (wrapper->IsShaftDoor == false)
			return ReportError("Deleting the main elevator door wrapper is not supported yet");

		deleted = true;
	}
	else if (type == "Escalator")
		deleted = true;
	else if (type == "Person")
		deleted = true;
	else if (type == "ElevatorCar")
	{
		ElevatorCar *car = static_cast<ElevatorCar*>(object);
		if (car->Number != car->GetElevator()->GetCarCount())
			return ReportError("Only the highest elevator car can be deleted");
		if (car->Number == 1)
			return ReportError("Cannot delete the primary elevator car");

		deleted = true;
	}
	else if (type == "MovingWalkway")
		deleted = true;
	else if (type == "Vehicle")
		deleted = true;
	else if (type == "Light")
		deleted = true;
	else if (type == "CallStation")
		deleted = true;
	else if (type == "DispatchController")
		deleted = true;
	else if (type == "Primitive")
		deleted = true;
	else if (type == "CustomObject")
		deleted = true;
	else if (type == "Reverb")
		deleted = true;
	else if (type == "CameraTexture")
		deleted = true;

	//delete object
	if (deleted == true)
		delete object;

	camera->ResetCollisions();

	return deleted;
}

bool SBS::DeleteObject(int object)
{
	//delete object by numeric ID
	return DeleteObject(GetObject(object));
}

bool SBS::MoveObject(Object *object, Vector3 position, bool relative, bool X, bool Y, bool Z)
{
	//move an object by reference
	//if relative is false, the X, Y and Z values determine which position axes should be set

	if (!object)
		return ReportError("Invalid object");

	if (relative == false)
	{
		if (X == false)
			position.x = object->GetPosition().x;
		if (Y == false)
			position.y = object->GetPosition().y;
		if (Z == false)
			position.z = object->GetPosition().z;

		object->SetPosition(position);
	}
	else
		object->Move(position);

	return true;
}

bool SBS::RotateObject(Object *object, Vector3 rotation, Real speed, bool relative, bool X, bool Y, bool Z)
{
	//rotate an object by reference
	//if relative is false, the X, Y and Z values determine which position axes should be set

	if (!object)
		return ReportError("Invalid object");

	if (relative == true)
		object->Rotate(rotation, speed);
	else
	{
		if (X == false)
			rotation.x = object->GetRotation().x;
		if (Y == false)
			rotation.y = object->GetRotation().y;
		if (Z == false)
			rotation.z = object->GetRotation().z;
		object->SetRotation(rotation);
	}

	return true;
}

void SBS::RemoveFloor(Floor *floor)
{
	//remove a floor (does not delete the object)

	floor_manager->Remove(floor);
}

void SBS::RemoveElevator(Elevator *elevator)
{
	//remove an elevator (does not delete the object)

	elevator_manager->Remove(elevator);
}

void SBS::RemoveShaft(Shaft *shaft)
{
	//remove a shaft (does not delete the object)

	shaft_manager->Remove(shaft);
}

void SBS::RemoveStairwell(Stairwell *stairs)
{
	//remove a stairs object (does not delete the object)

	stairwell_manager->Remove(stairs);
}

void SBS::RemoveSound(Sound *sound)
{
	//remove a sound from the array
	//this does not delete the object

	RemoveArrayElement(sounds, sound);
}

void SBS::RemoveReverb(Reverb *reverb)
{
	//remove a reverb from the array
	//this does not delete the object

	RemoveArrayElement(reverbs, reverb);
}

void SBS::RemoveLight(Light *light)
{
	//remove a light reference (does not delete the object itself)

	RemoveArrayElement(lights, light);
}

void SBS::RemoveModel(Model *model)
{
	//remove a model reference (does not delete the object itself)

	RemoveArrayElement(ModelArray, model);
}

void SBS::RemovePrimitive(Primitive *prim)
{
	//remove a prim reference (does not delete the object itself)

	RemoveArrayElement(PrimArray, prim);
}

void SBS::RemoveCustomObject(CustomObject *object)
{
	//remove a custom object reference (does not delete the object itself)

	RemoveArrayElement(CustomObjectArray, object);
}

void SBS::RemoveControl(Control *control)
{
	//remove a control reference (does not delete the object itself)

	RemoveArrayElement(ControlArray, control);
}

void SBS::RemoveTrigger(Trigger *trigger)
{
	//remove a trigger reference (does not delete the object itself)

	RemoveArrayElement(TriggerArray, trigger);
}

void SBS::RemoveController(DispatchController *controller)
{
	//remove a floor (does not delete the object)

	controller_manager->Remove(controller);
}

void SBS::Prepare(bool report, bool renderonly)
{
	//prepare objects for run

	SBS_PROFILE_MAIN("Prepare");

	//prepare mesh objects
	if (report == true)
		Report("Preparing meshes...");
	for (size_t i = 0; i < meshes.size(); i++)
	{
		meshes[i]->Prepare(false);
	}

	//process dynamic meshes
	if (report == true)
		Report("Processing geometry...");
	for (size_t i = 0; i < dynamic_meshes.size(); i++)
	{
		if (sbs->Verbose && report == true)
			Report("DynamicMesh " + ToString((int)i) + " of " + ToString((int)dynamic_meshes.size()));
		dynamic_meshes[i]->Prepare();
	}

	if (renderonly == false)
	{
		if (report == true)
			Report("Creating colliders...");
		for (size_t i = 0; i < meshes.size(); i++)
		{
			if (meshes[i]->tricollider == true && meshes[i]->IsPhysical() == false)
				meshes[i]->CreateCollider();
			else
				meshes[i]->CreateBoxCollider();
		}
	}

	if (report == true)
		Report("Finished prepare");
}

Light* SBS::AddLight(const std::string &name, int type)
{
	//add a global light

	Light* light = new Light(this, name, type);
	lights.emplace_back(light);
	return light;
}

void SBS::AddMeshHandle(MeshObject* handle)
{
	AddArrayElement(meshes, handle);
}

void SBS::DeleteMeshHandle(MeshObject* handle)
{
	RemoveArrayElement(meshes, handle);
}

MeshObject* SBS::FindMeshObject(const std::string &name)
{
	//find a mesh object by searching for matching wrapper
	for (size_t i = 0; i < meshes.size(); i++)
	{
		if (meshes[i]->name == name)
			return meshes[i];
	}
	return 0;
}

Model* SBS::AddModel(const std::string &name, const std::string &filename, bool center, const Vector3 &position, const Vector3 &rotation, Real max_render_distance, Real scale_multiplier, bool enable_physics, Real restitution, Real friction, Real mass)
{
	//add a model
	Model* model = new Model(this, name, filename, center, position, rotation, max_render_distance, scale_multiplier, enable_physics, restitution, friction, mass);
	if (model->load_error == true)
	{
		delete model;
		return 0;
	}
	ModelArray.emplace_back(model);
	return model;
}

void SBS::AddModel(Model *model)
{
	//add a model reference

	if (!model)
		return;

	for (size_t i = 0; i < ModelArray.size(); i++)
	{
		if (ModelArray[i] == model)
			return;
	}

	ModelArray.emplace_back(model);
}

Primitive* SBS::AddPrimitive(const std::string &name)
{
	//add a prim
	Primitive* prim = new Primitive(this, name);
	PrimArray.emplace_back(prim);
	return prim;
}

void SBS::AddPrimitive(Primitive *primitive)
{
	//add a primitive reference

	if (!primitive)
		return;

	for (size_t i = 0; i < PrimArray.size(); i++)
	{
		if (PrimArray[i] == primitive)
			return;
	}

	PrimArray.emplace_back(primitive);
}

CustomObject* SBS::AddCustomObject(const std::string &name, const Vector3 &position, const Vector3 &rotation, Real max_render_distance, Real scale_multiplier)
{
	//add a custom object
	CustomObject* object = new CustomObject(this, name, position, rotation, max_render_distance, scale_multiplier);
	CustomObjectArray.emplace_back(object);
	return object;
}

void SBS::AddCustomObject(CustomObject *object)
{
	//add a custom object reference

	if (!object)
		return;

	for (size_t i = 0; i < CustomObjectArray.size(); i++)
	{
		if (CustomObjectArray[i] == object)
			return;
	}

	CustomObjectArray.emplace_back(object);
}

int SBS::GetConfigInt(const std::string &key, int default_value)
{
	std::string result = configfile->getSetting(key, "", ToString(default_value));
	return ToInt(result);
}

std::string SBS::GetConfigString(const std::string &key, const std::string &default_value)
{
	return configfile->getSetting(key, "", default_value);
}

bool SBS::GetConfigBool(const std::string &key, bool default_value)
{
	std::string result = configfile->getSetting(key, "", ToString(default_value));
	return ToBool(result);
}

Real SBS::GetConfigFloat(const std::string &key, Real default_value)
{
	std::string result = configfile->getSetting(key, "", ToString(default_value));
	return ToFloat(result);
}

void SBS::AdvanceClock()
{
	//advance the clock

	unsigned long last = current_time;

	//get current time
	current_time = GetCurrentTime();
	if (last == 0)
		last = current_time;

	if (current_time < last)
		elapsed_time = current_time + ((unsigned long)-1 - last) + 1;
	else
		elapsed_time = current_time - last;
	current_virtual_time += elapsed_time;
	frame_times.emplace_back(current_time);
	CalculateAverageTime();
}

unsigned long SBS::GetCurrentTime()
{
	//get current time
	return timer->getMilliseconds();
}

unsigned long SBS::GetRunTime()
{
	//returns simulator run time
	return current_virtual_time;
}

unsigned long SBS::GetElapsedTime()
{
	//returns the actual elapsed time between frames
	return elapsed_time;
}

unsigned long SBS::GetAverageTime()
{
	//returns the average elapsed time between frames
	return average_time;
}

void SBS::CalculateAverageTime()
{
	//calculates the average frame processing time for a specified number of frames

	if (frame_times.size() <= 1)
		return;

	//SmoothFrames is the maximum number of milliseconds to hold timing info

	//find oldest time to keep
	std::deque<unsigned long>::iterator it = frame_times.begin(), end = frame_times.end() - 2;

	while (it != end)
	{
		if (frame_times.back() - *it > SmoothFrames)
			++it;
		else
			break;
	}

	//remove old times
	frame_times.erase(frame_times.begin(), it);

	//calculate average time
	average_time = (frame_times.back() - frame_times.front()) / ((unsigned long)frame_times.size() - 1);
}

void SBS::ShowColliders(bool value)
{
	try
	{
		if (mWorld)
			mWorld->setShowDebugShapes(value);
		camera->ShowDebugShape(value);
	}
	catch (Ogre::Exception &e)
	{
		ReportError("Error enabling/disabling collider shapes\n" + e.getDescription());
	}
}

void SBS::SetLighting(Real red, Real green, Real blue)
{
	OldAmbientR = AmbientR;
	OldAmbientG = AmbientG;
	OldAmbientB = AmbientB;
	AmbientR = red;
	AmbientG = green;
	AmbientB = blue;
}

void SBS::ResetLighting()
{
	AmbientR = OldAmbientR;
	AmbientG = OldAmbientG;
	AmbientB = OldAmbientB;
}

Control* SBS::AddControl(const std::string &name, const std::string &sound, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, int selection_position, std::vector<std::string> &action_names, std::vector<std::string> &textures)
{
	//add a control
	std::vector<Action*> actionnull; //not used
	Control* control = new Control(this, name, false, sound, action_names, actionnull, textures, direction, width, height, true, selection_position);
	control->SetPosition(CenterX, voffset, CenterZ);
	ControlArray.emplace_back(control);
	return control;
}

Trigger* SBS::AddTrigger(const std::string &name, const std::string &sound_file, const Vector3 &area_min, const Vector3 &area_max, std::vector<std::string> &action_names)
{
	//add a trigger
	Trigger* trigger = new Trigger(this, name, false, sound_file, area_min, area_max, action_names);
	TriggerArray.emplace_back(trigger);
	return trigger;
}

Action* SBS::AddAction(const std::string &name, std::vector<Object*> &action_parents, const std::string &command, const std::vector<std::string> &parameters)
{
	//add a global action

	Action *action = new Action(this, name, action_parents, command, parameters);
	ActionArray.emplace_back(action);
	return action;
}

Action* SBS::AddAction(const std::string &name, std::vector<Object*> &action_parents, const std::string &command)
{
	//add a global action

	Action *action = new Action(this, name, action_parents, command);
	ActionArray.emplace_back(action);
	return action;
}

std::vector<Action*> SBS::GetAction(std::string name)
{
	//get action by name
	ReplaceAll(name, " ", "");

	//for an action that references an elevator's "Car 1", create
	//a second check with the "Car 1" part removed for compatibility
	std::string name2;
	size_t pos = name.find("Car1");
	if (name.substr(0, 8) == "Elevator" && pos != std::string::npos)
		name2 = name.substr(0, pos - 1) + name.substr(name.find(":", pos));

	std::vector<Action*> actionlist;
	for (size_t i = 0; i < ActionArray.size(); i++)
	{
		std::string actionname = ActionArray[i]->GetName();
		ReplaceAll(actionname, " ", "");
		if (actionname == name || actionname == name2)
			actionlist.emplace_back(ActionArray[i]);
	}
	return actionlist;
}

int SBS::GetActionCount()
{
	//get number of registered actions
	return (int)ActionArray.size();
}

bool SBS::AddActionParent(const std::string &name, std::vector<Object*> &parents)
{
	//add parent to actions specified by 'name'

	bool result = false;
	std::vector<Action*> actionlist = GetAction(name);

	for (size_t i = 0; i < actionlist.size(); i++)
	{
		Action *action = actionlist[i];
		for (size_t j = 0; j < parents.size(); j++)
		{
			if (action->AddParent(parents[j]))
				result = true;
		}
	}
	return result;
}

bool SBS::RemoveActionParent(const std::string &name, std::vector<Object*> &parents)
{
	//remove parent object from actions specified by 'name'

	bool result = false;
	std::vector<Action*> actionlist = GetAction(name);

	for (size_t i = 0; i < actionlist.size(); i++)
	{
		Action *action = actionlist[i];
		for (size_t j = 0; j < parents.size(); j++)
		{
			if (action->RemoveParent(parents[j]))
				result = true;
		}
	}
	return result;
}

bool SBS::RemoveActionParent(std::vector<Object*> &parents)
{
	//remove parent object from all action objects

	bool result = false;
	for (size_t i = 0; i < ActionArray.size(); i++)
	{
		Action *action = ActionArray[i];
		for (size_t j = 0; j < parents.size(); j++)
		{
			if (action->RemoveParent(parents[j]))
				result = true;
		}
	}
	return result;
}

bool SBS::RemoveAction(std::string name)
{
	//remove action by name

	ReplaceAll(name, " ", "");
	bool result = false;
	for (size_t i = 0; i < ActionArray.size(); i++)
	{
		if (ActionArray[i])
		{
			std::string actionname = ActionArray[i]->GetName();
			ReplaceAll(actionname, " ", "");
			if (actionname == name)
			{
				delete ActionArray[i];
				ActionArray.erase(ActionArray.begin() + i);
				i--;
				result = true;
			}
		}
	}
	return result;
}

bool SBS::RemoveAction(Action *action)
{
	//remove action

	if (!action)
		return false;

	bool result = false;
	for (size_t i = 0; i < ActionArray.size(); i++)
	{
		if (ActionArray[i] == action)
		{
			delete ActionArray[i];
			ActionArray.erase(ActionArray.begin() + i);
			i--;
			result = true;

			//remove reference to action in all control objects
			for (size_t j = 0; j < control_index.size(); j++)
			{
				control_index[j]->RemoveAction(action);
			}
		}
	}
	return result;
}

Object* SBS::GetObject(std::string name, bool case_sensitive)
{
	//get object by name

	ReplaceAll(name, " ", "");

	//use hints to quickly find a reference
	{
		//floors
		bool found = false;
		std::string tmpname = "Floor";
		if (case_sensitive == false)
		{
			SetCase(name, false);
			SetCase(tmpname, false);

			if (name.find("floor", 0) == 0)
				found = true;
		}
		else
		{
			if (name.find("Floor", 0) == 0)
				found = true;
		}

		if (found == true && name.find(":", 0) == std::string::npos)
		{
			std::string number = name.substr(5);
			if (IsNumeric(number))
			{
				int floor = ToInt(number);
				return sbs->GetFloor(floor);
			}
		}
	}
	{
		//elevators
		bool found = false;
		std::string tmpname = "Elevator";
		if (case_sensitive == false)
		{
			SetCase(name, false);
			SetCase(tmpname, false);

			if (name.find("elevator", 0) == 0)
				found = true;
		}
		else
		{
			if (name.find("Elevator", 0) == 0)
				found = true;
		}

		if (found == true && name.find(":", 0) == std::string::npos)
		{
			std::string number = name.substr(8);
			if (IsNumeric(number))
			{
				int elevator = ToInt(number);
				return sbs->GetElevator(elevator);
			}
		}
	}
	{
		//shafts
		bool found = false;
		std::string tmpname = "Shaft";
		if (case_sensitive == false)
		{
			SetCase(name, false);
			SetCase(tmpname, false);

			if (name.find("shaft", 0) == 0)
				found = true;
		}
		else
		{
			if (name.find("Shaft", 0) == 0)
				found = true;
		}

		if (found == true && name.find(":", 0) == std::string::npos)
		{
			std::string number = name.substr(5);
			if (IsNumeric(number))
			{
				int shaft = ToInt(number);
				return sbs->GetShaft(shaft);
			}
		}
	}
	{
		//shafts
		bool found = false;
		std::string tmpname = "Stairwell";
		if (case_sensitive == false)
		{
			SetCase(name, false);
			SetCase(tmpname, false);

			if (name.find("stairwell", 0) == 0)
				found = true;
		}
		else
		{
			if (name.find("Stairwell", 0) == 0)
				found = true;
		}

		if (found == true && name.find(":", 0) == std::string::npos)
		{
			std::string number = name.substr(9);
			if (IsNumeric(number))
			{
				int stair = ToInt(number);
				return sbs->GetStairwell(stair);
			}
		}
	}

	for (size_t i = 0; i < ObjectArray.size(); i++)
	{
		if (ObjectArray[i])
		{
			std::string tmpname = ObjectArray[i]->GetName();
			ReplaceAll(tmpname, " ", "");
			if (case_sensitive == false)
				SetCase(tmpname, false);

			//get by object name
			if (tmpname == name)
				return ObjectArray[i];

			if (ObjectArray[i]->GetParent())
			{
				std::string parent_name = ObjectArray[i]->GetParent()->GetName();
				ReplaceAll(parent_name, " ", "");
				if (case_sensitive == false)
					SetCase(parent_name, false);

				//get by "parentname:objectname"
				if (name == parent_name + ":" + tmpname)
					return ObjectArray[i];

				if (ObjectArray[i]->GetParent()->GetParent())
				{
					std::string grandparent_name = ObjectArray[i]->GetParent()->GetParent()->GetName();
					ReplaceAll(grandparent_name, " ", "");
					if (case_sensitive == false)
						SetCase(grandparent_name, false);

					//get by "grandparentname:parentname:objectname"
					if (name == grandparent_name + ":" + parent_name + ":" + tmpname)
						return ObjectArray[i];
				}
			}
		}
	}

	return 0;
}

Object* SBS::GetObjectOfParent(std::string parent_name, std::string name, const std::string &type, bool case_sensitive)
{
	//get object by name

	ReplaceAll(name, " ", "");
	ReplaceAll(parent_name, " ", "");
	if (case_sensitive == false)
	{
		SetCase(name, false);
		SetCase(parent_name, false);
	}

	for (size_t i = 0; i < ObjectArray.size(); i++)
	{
		Object *object = ObjectArray[i];

		if (object)
		{
			if (object->GetType() != type)
				continue;

			Object* parent = ObjectArray[i]->GetParent();

			if (parent)
			{
				std::string namecheck = parent->GetName();
				ReplaceAll(namecheck, " ", "");
				if (case_sensitive == false)
					SetCase(namecheck, false);

				if (namecheck == parent_name) //check object for match if parent matches
				{
					std::string tmpname = object->GetName();
					ReplaceAll(tmpname, " ", "");
					if (case_sensitive == false)
						SetCase(tmpname, false);

					//get by object name
					if (tmpname == name)
						return object;
				}
			}
		}
	}

	return 0;
}

std::vector<Object*> SBS::GetObjectRange(const std::string &expression)
{
	//get object by name range expression (ex. "Floors 1 to 3")

	std::vector<Object*> objects;
	size_t temp = expression.find("to", 0);

	//the name 'elevator' matches the previous search - in this case, detect it and undo
	size_t temp2 = expression.find("tor", 0);
	if (temp == temp2)
		temp = 0;

	std::string type;

	if (temp > 0 && temp != std::string::npos)
	{
		if (expression.substr(0, 6) == "Floors")
			type = "floor";
		else if (expression.substr(0, 9) == "Elevators")
			type = "elevator";
		else if (expression.substr(0, 6) == "Shafts")
			type = "shaft";
		else if (expression.substr(0, 10) == "Stairwells")
			type = "stairwell";
		else
		{
			ReportError("GetObjectRange: Invalid object type");
			return objects;
		}

		std::string str1 = expression.substr(type.size() + 1, temp - (type.size() + 1));
		std::string str2 = expression.substr(temp + 2, expression.length() - (temp + 2));
		TrimString(str1);
		TrimString(str2);
		int RangeL = 0, RangeH = 0;
		if (!IsNumeric(str1, RangeL) || !IsNumeric(str2, RangeH))
		{
			ReportError("GetObjectRange: Invalid range");
			return objects;
		}

		for (size_t i = 0; i < ObjectArray.size(); i++)
		{
			if (ObjectArray[i])
			{
				std::string tmpname = ObjectArray[i]->GetName();
				for (int j = RangeL; j <= RangeH; j++)
				{
					std::string number = ToString(j);
					if (type == "floor")
					{
						if (tmpname == "Floor " + number)
							objects.emplace_back(ObjectArray[i]);
					}
					if (type == "elevator")
					{
						if (tmpname == "Elevator " + number)
							objects.emplace_back(ObjectArray[i]);
					}
					if (type == "shaft")
					{
						if (tmpname == "Shaft " + number)
							objects.emplace_back(ObjectArray[i]);
					}
					if (type == "stairwell")
					{
						if (tmpname == "Stairwell " + number)
							objects.emplace_back(ObjectArray[i]);
					}
				}
			}
		}
	}
	else
	{
		//return single result
		Object *obj = GetObject(expression);
		if (obj)
			objects.emplace_back(obj);
	}

	return objects;
}

Action* SBS::GetAction(int index)
{
	if (index >= 0 && index < (int)ActionArray.size())
		return ActionArray[index];
	return 0;
}

bool SBS::RunAction(const std::string &name)
{
	//run action by name - will run multiple actions if the name is the same

	std::vector<Action*> actionlist = GetAction(name);

	bool result = true;
	for (size_t i = 0; i < actionlist.size(); i++)
	{
		bool result2 = false;
		bool hold = false; //not used

		if (actionlist[i])
			result2 = actionlist[i]->DoAction(this, hold);

		if (result2 == false)
			result = false;
	}
	return result;
}

bool SBS::RunAction(int index)
{
	//run action by index number

	Action *action = GetAction(index);
	bool hold = false; //not used
	if (action)
		return action->DoAction(this, hold);
	return false;
}

void SBS::AddKey(int keyid, const std::string &name)
{
	//adds key 'keyid' to the user's keyring
	Key key;
	key.id = keyid;
	key.name = name;
	keys.emplace_back(key);

	Report("Added key " + ToString(keyid) + " (" + name + ") to keyring");
}

bool SBS::CheckKey(int keyid)
{
	//checks to see if the user has the specified key

	for (size_t i = 0; i < keys.size(); i++)
	{
		if (keys[i].id == keyid)
			return true;
	}
	return false;
}

void SBS::ListKeys()
{
	//list all keys

	Report("\n--- Keys ---\n");

	for (size_t i = 0; i < keys.size(); i++)
	{
		std::string id = ToString(keys[i].id);
		Report(id + " - " + keys[i].name);
	}
	Report("");
}

void SBS::RegisterControl(Control *control)
{
	//add control to index

	AddArrayElement(control_index, control);
}

void SBS::UnregisterControl(Control *control)
{
	//remove control from index

	RemoveArrayElement(control_index, control);
}

void SBS::ShowFloorList()
{
	//show floor information for all floors

	bool header_shown = false;
	for (int i = -Basements; i < Floors; i++)
	{
		Floor *floor = GetFloor(i);
		if (floor)
		{
			if (header_shown == false)
			{
				floor->ShowInfo(false, true);
				header_shown = true;
			}
			else
				floor->ShowInfo(false, false);
		}
	}
	Report("");
}

void SBS::ShowSceneNodes(bool value)
{
	//show all scene nodes for debugging

	mSceneManager->setDisplaySceneNodes(value);
}

void SBS::ShowBoundingBoxes(bool value)
{
	//show all mesh bounding boxes for debugging

	mSceneManager->showBoundingBoxes(value);
}

void SBS::ListVisibleMeshes()
{
	//list all meshes visible by the main camera

	Report("\n--- Visible Dynamic Meshes ---");
	Report("Name\t-\tSubmeshes\n");
	int count = 0;
	int submeshes = 0;
	int total = 0;

	for (size_t i = 0; i < dynamic_meshes.size(); i++)
	{
		for (int j = 0; j < dynamic_meshes[i]->GetMeshCount(); j++)
		{
			if (camera->IsDynamicMeshVisible(dynamic_meshes[i], j) == true)
			{
				submeshes = dynamic_meshes[i]->GetSubMeshCount(j);
				Report(dynamic_meshes[i]->GetMeshName(j) + "\t-\t" + ToString(submeshes));
				count++;
				total += submeshes;
			}
		}
	}
	Report("Total: " + ToString(count) + " meshes, " + ToString(total) + " submeshes");
	Report("");
}

int SBS::GetEscalatorCount()
{
	//return total number of escalators
	return (int)EscalatorArray.size();
}

int SBS::GetMovingWalkwayCount()
{
	//return total number of moving walkways
	return (int)MovingWalkwayArray.size();
}

int SBS::GetRevolvingDoorCount()
{
	//return total number of revolving doors
	return (int)RevolvingDoorArray.size();
}

void SBS::EnableRandomActivity(bool value)
{
	//enable random activity, by creating random people

	if (value == true)
	{
		//create regular people
		int people = GetConfigInt("Skyscraper.SBS.Person.RandomPeople", 0);
		people = people == 0 ? GetTotalFloors() : people;
		for (int i = 0; i < people; i++)
		{
			Person *person = CreatePerson("Random " + ToString(i + 1), Lobby, false);

			//enable random activity on the person
			person->EnableRandomActivity(true);
		}

		//create a service person
		Person *person = CreatePerson("Random " + ToString(people + 1), Lobby, true);

		//enable random activity on the person
		person->EnableRandomActivity(true);
	}
	else
	{
		for (size_t i = 0; i < PersonArray.size(); i++)
		{
			if (PersonArray[i]->IsRandomActivityEnabled() == true)
			{
				Person *person = PersonArray[i];
				delete person;
				i--;
			}
		}
	}

	RandomActivity = value;
}

void SBS::EnableMalfunctions(bool value)
{
	//enable malfunctions

	for (int i = 1; i <= elevator_manager->GetCount(); i++)
	{
		Elevator *elevator = elevator_manager->Get(i);
		if (elevator)
		{
			elevator->EnableMalfunctions(value);
		}
	}

	for (size_t i = 0; i < EscalatorArray.size(); i++)
	{
		Escalator *escalator = EscalatorArray[i];
		if (escalator)
		{
			escalator->EnableMalfunctions(value);
		}
	}

	Malfunctions = value;
}

bool SBS::IsObjectValid(Object *object, const std::string &type)
{
	//test if an object is valid

	if (type == "Floor")
	{
		for (int i = 0; i < floor_manager->GetCount(); i++)
		{
			if (floor_manager->GetIndex(i) == static_cast<Floor*>(object))
				return true;
		}
	}
	else if (type == "Elevator")
	{
		for (int i = 0; i < elevator_manager->GetCount(); i++)
		{
			if (elevator_manager->GetIndex(i) == static_cast<Elevator*>(object))
				return true;
		}
	}
	else if (type == "Shaft")
	{
		for (int i = 0; i < shaft_manager->GetCount(); i++)
		{
			if (shaft_manager->GetIndex(i) == static_cast<Shaft*>(object))
				return true;
		}
	}
	else if (type == "Stairwell")
	{
		for (int i = 0; i < stairwell_manager->GetCount(); i++)
		{
			if (stairwell_manager->GetIndex(i) == static_cast<Stairwell*>(object))
				return true;
		}
	}
	else if (type == "Mesh")
	{
		for (size_t i = 0; i < meshes.size(); i++)
		{
			if (meshes[i] == static_cast<MeshObject*>(object))
				return true;
		}
	}
	else if (type == "Control")
	{
		for (size_t i = 0; i < control_index.size(); i++)
		{
			if (control_index[i] == static_cast<Control*>(object))
				return true;
		}
	}

	//do a slow full scan of the object array for all other objects
	for (size_t i = 0; i < ObjectArray.size(); i++)
	{
		if (ObjectArray[i] == object)
			return true;
	}
	return false;
}

bool SBS::IsActionValid(Action *action)
{
	//test if an action is valid

	for (size_t i = 0; i < ActionArray.size(); i++)
	{
		if (ActionArray[i] == action)
			return true;
	}
	return false;
}

Person* SBS::CreatePerson(std::string name, int floor, bool service_access)
{
	//create a person

	if (name == "")
	{
		int number = GetPersonCount() + 1;
		name = "Person " + ToString(number);
	}
	Person *person = new Person(this, name, floor, service_access);
	PersonArray.emplace_back(person);
	return person;
}

void SBS::RemovePerson(Person *person)
{
	//remove a person (does not delete the object)

	RemoveArrayElement(PersonArray, person);
}

bool SBS::AttachCamera(std::vector<Ogre::Camera*> &cameras, bool init_state)
{
	if (cameras.size() > 0)
		return camera->Attach(cameras, init_state);
	return false;
}

bool SBS::DetachCamera()
{
	if (!camera)
		return false;

	return camera->Detach();
}

Person* SBS::GetPerson(int number)
{
	if (number < 0 || number > (int)PersonArray.size() - 1)
		return 0;

	return PersonArray[number];
}

bool SBS::IsInside()
{
	//return true if the user is inside the sim engine's area

	if (area_trigger)
		return area_trigger->IsInside();

	//if no trigger is defined, user is always inside the area
	return true;
}

bool SBS::IsInside(const Vector3 &position)
{
	//return true if the specified position is inside the sim engine's area

	if (area_trigger)
		return area_trigger->IsInside(position);

	//if no trigger is defined, position is always inside the area
	return true;
}

bool SBS::GetBounds(Vector3 &min, Vector3 &max)
{
	if (area_trigger)
	{
		min = area_trigger->GetMin();
		max = area_trigger->GetMax();
		return true;
	}

	min = Vector3::ZERO;
	max = Vector3::ZERO;
	return false;
}

void SBS::CutOutsideBoundaries(bool landscape, bool buildings, bool external, bool floors)
{
	//cut landscape and buildings for engine bounds if needed
	//run this function before calling Start()

	if (!area_trigger)
		return;

	Report("Cutting outside boundaries...");
	Vector3 min = area_trigger->GetMin();
	Vector3 max = area_trigger->GetMax();

	if (landscape == true && Landscape)
		Landscape->CutOutsideBounds(min, max, true, true);
	if (buildings == true && Buildings)
		Buildings->CutOutsideBounds(min, max, true, true);
	if (external == true && External)
		External->CutOutsideBounds(min, max, true, true);

	if (floors == true)
	{
		for (int i = 0; i < floor_manager->GetCount(); i++)
			floor_manager->GetIndex(i)->Level->CutOutsideBounds(min, max, true, true);
	}
}

void SBS::CutInsideBoundaries(const Vector3 &min, const Vector3 &max, bool landscape, bool buildings, bool external, bool floors)
{
	//cut landscape and buildings for specified bounds
	//run this function before calling Start()

	if (landscape == true && Landscape)
		Landscape->Cut(min, max, true, true);
	if (buildings == true && Buildings)
		Buildings->Cut(min, max, true, true);
	if (external == true && External)
		External->Cut(min, max, true, true);

	if (floors == true)
	{
		for (int i = 0; i < floor_manager->GetCount(); i++)
			floor_manager->GetIndex(i)->Level->Cut(min, max, true, true);
	}
}

void SBS::SetBounds(const Vector3 &area_min, const Vector3 &area_max)
{
	//don't set bounds if the primary engine
	if (InstanceNumber == 0)
		return;

	if (area_min != Vector3::ZERO && area_max != Vector3::ZERO && !area_trigger)
	{
		std::vector<std::string> names;
		names.emplace_back("Off");
		area_trigger = new Trigger(this, "System Boundary", true, "", area_min, area_max, names);
		auto_bounds = false;
	}
}

void SBS::ResetState()
{
	//reset building to original state

	//turn on main objects
	EnableBuildings(true);
	EnableLandscape(true);
	EnableExternal(true);
	EnableSkybox(true);

	//turn off interior objects
	if (floor_manager)
		floor_manager->EnableAll(false);
	if (shaft_manager)
		shaft_manager->EnableAll(false);
	if (stairwell_manager)
		stairwell_manager->EnableAll(false);
	if (elevator_manager)
		elevator_manager->EnableAll(false);

	//turn off reverbs
	for (size_t i = 0; i < reverbs.size(); i++)
	{
		if (reverbs[i])
			reverbs[i]->Enabled(false);
	}

	//reset camera state
	if (camera)
		camera->ResetState();
}

Light* SBS::GetLight(std::string name)
{
	//get a light by name

	SetCase(name, false);

	for (size_t i = 0; i < lights.size(); i++)
	{
		if (SetCaseCopy(lights[i]->GetName(), false) == name)
			return lights[i];
	}

	return 0;
}

Model* SBS::GetModel(std::string name)
{
	//get a model by name

	SetCase(name, false);

	for (size_t i = 0; i < ModelArray.size(); i++)
	{
		if (SetCaseCopy(ModelArray[i]->GetName(), false) == name)
			return ModelArray[i];
	}

	return 0;
}

Primitive* SBS::GetPrimitive(std::string name)
{
	//get a primitive by name

	SetCase(name, false);

	for (size_t i = 0; i < PrimArray.size(); i++)
	{
		if (SetCaseCopy(PrimArray[i]->GetName(), false) == name)
			return PrimArray[i];
	}

	return 0;
}

CustomObject* SBS::GetCustomObject(std::string name)
{
	//get a custom object by name

	SetCase(name, false);

	for (size_t i = 0; i < CustomObjectArray.size(); i++)
	{
		if (SetCaseCopy(CustomObjectArray[i]->GetName(), false) == name)
			return CustomObjectArray[i];
	}

	return 0;
}

void SBS::RegisterDynamicMesh(DynamicMesh *dynmesh)
{
	//register a dynamic mesh with the system

	AddArrayElement(dynamic_meshes, dynmesh);
}

void SBS::UnregisterDynamicMesh(DynamicMesh *dynmesh)
{
	//unregister a dynamic mesh from the system

	RemoveArrayElement(dynamic_meshes, dynmesh);
}

SoundSystem* SBS::GetSoundSystem()
{
	return soundsystem;
}

int SBS::GetPersonCount()
{
	return (int)PersonArray.size();
}

bool SBS::HasBounds()
{
	return (area_trigger != 0);
}

FloorManager* SBS::GetFloorManager()
{
	return floor_manager;
}

ElevatorManager* SBS::GetElevatorManager()
{
	return elevator_manager;
}

ShaftManager* SBS::GetShaftManager()
{
	return shaft_manager;
}

StairwellManager* SBS::GetStairwellManager()
{
	return stairwell_manager;
}

DoorManager* SBS::GetDoorManager()
{
	return door_manager;
}

TextureManager* SBS::GetTextureManager()
{
	return texturemanager;
}

RevolvingDoorManager* SBS::GetRevolvingDoorManager()
{
	return revolvingdoor_manager;
}

VehicleManager* SBS::GetVehicleManager()
{
	return vehicle_manager;
}

ControllerManager* SBS::GetControllerManager()
{
	return controller_manager;
}

TeleporterManager* SBS::GetTeleporterManager()
{
	return teleporter_manager;
}

void SBS::RegisterCameraTexture(CameraTexture *camtex)
{
	//register a camera texture

	AddArrayElement(camtexarray, camtex);
}

void SBS::UnregisterCameraTexture(CameraTexture *camtex)
{
	//unregister a camera texture

	RemoveArrayElement(camtexarray, camtex);
}

int SBS::GetCameraTextureCount()
{
	return (int)camtexarray.size();
}

CameraTexture* SBS::GetCameraTexture(int number)
{
	if (number < camtexarray.size())
		return camtexarray[number];
	return 0;
}

Utility* SBS::GetUtility()
{
	return utility;
}

GeometryController* SBS::GetGeometry()
{
	return geometry;
}

void SBS::MemoryReport()
{
	//print a memory report
	Report("");
	Report("--- Memory Report ---");

	size_t total = 0;
	for (size_t i = 0; i < meshes.size(); i++)
		total += meshes[i]->GetSize();

	//texture memory
	Report("Textures: " + ToString(texturemanager->GetMemoryUsage() / 1024) + " kb");

	//mesh memory
	Report("Meshes: " + ToString(total / 1024) + " kb");

	Report("");
}

void SBS::RegisterEscalator(Escalator *escalator)
{
	//add escalator to index

	AddArrayElement(EscalatorArray, escalator);
}

void SBS::UnregisterEscalator(Escalator *escalator)
{
	//remove escalator from index

	RemoveArrayElement(EscalatorArray, escalator);
}

Escalator* SBS::GetEscalator(int index)
{
	if (index >= 0 && index < EscalatorArray.size())
		return EscalatorArray[index];
	return 0;
}

void SBS::RegisterMovingWalkway(MovingWalkway *walkway)
{
	//add moving walkway to index

	AddArrayElement(MovingWalkwayArray, walkway);
}

void SBS::UnregisterMovingWalkway(MovingWalkway *walkway)
{
	//remove moving walkway from index

	RemoveArrayElement(MovingWalkwayArray, walkway);
}

MovingWalkway* SBS::GetMovingWalkway(int index)
{
	if (index >= 0 && index < MovingWalkwayArray.size())
		return MovingWalkwayArray[index];
	return 0;
}

void SBS::RegisterRevolvingDoor(RevolvingDoor* door)
{
	//add revolving door to index
	RevolvingDoorArray.emplace_back(door);
}

void SBS::UnregisterRevolvingDoor(RevolvingDoor* door)
{
	//remove revolving door from index

	RemoveArrayElement(RevolvingDoorArray, door);
}

RevolvingDoor* SBS::GetRevolvingDoor(int index)
{
	if (index >= 0 && index < RevolvingDoorArray.size())
		return RevolvingDoorArray[index];
	return 0;
}

void SBS::SetPower(bool value)
{
	//set building power state

	power_state = value;
}

bool SBS::GetPower()
{
	//return building power state

	return power_state;
}

int SBS::GetReverbCount()
{
	return (int)reverbs.size();
}

Reverb* SBS::GetReverb(int index)
{
	if (index >= 0 && index < (int)reverbs.size())
		return reverbs[index];
	return 0;
}

void SBS::EnableMap(bool value)
{
	//enable or disable map generator
	if (MapGenerator)
		MapGenerator->Enabled(value);
}

int SBS::GetTextureCount()
{
	if (GetTextureManager())
		return GetTextureManager()->GetMaterialCount();

	return 0;
}

PolyMesh* SBS::GetPolyMesh()
{
	//return pointer to the PolyMesh geometry processor
	return polymesh;
}

Trigger* SBS::GetAreaTrigger()
{
	return area_trigger;
}

MeshObject* SBS::GetLandscapeMesh()
{
	return Landscape;
}

Real SBS::GetUnitScale()
{
	//get unit scaling factor

	return utility->UnitScale;
}

Vector3 SBS::GetCenter()
{
	//return the center point of this sim engine, based on the bounds

	if (HasBounds() == false)
		return Vector3(0, 0, 0);

	return area_trigger->GetBounds().getCenter();
}

Shape* SBS::CreateShape(Wall *wall)
{
	//Creates a shape in the specified wall object
	//returns a Shape object, which must be deleted by the caller after use

	if (!wall)
		return 0;

	Shape *shape = new Shape(wall);
	return shape;
}

void SBS::MergeBounds(Ogre::AxisAlignedBox &box)
{
	if (box.isNull())
		return;

	if (bounds_set == true)
		bounds.merge(box);
	else
		bounds = box;
	bounds_set = true;
}

}
