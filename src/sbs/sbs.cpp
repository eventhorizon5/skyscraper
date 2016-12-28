/* $Id$ */

/*
	Scalable Building Simulator - Core
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

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreFileSystem.h>
#include <OgreConfigFile.h>
#include <OgreTimer.h>
#include "OgreStringVector.h"
#include <fmod.hpp>
#include <OgreBulletDynamicsRigidBody.h>
#include <OgreBulletCollisionsRay.h>
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
#include "texture.h"
#include "light.h"
#include "wall.h"
#include "control.h"
#include "trigger.h"
#include "soundsystem.h"
#include "sound.h"
#include "model.h"
#include "timer.h"
#include "profiler.h"
#include "revsbs.h"

namespace SBS {

SBS::SBS(Ogre::SceneManager* mSceneManager, FMOD::System *fmodsystem, int instance_number, const Ogre::Vector3 &position, float rotation, const Ogre::Vector3 &area_min, const Ogre::Vector3 &area_max) : Object(0)
{
	sbs = this;
	this->mSceneManager = mSceneManager;

	version = "0.11.0." + std::string(SVN_REVSTR);
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
	FrameRate = 30;
	FrameLimiter = false;
	AutoShafts = GetConfigBool("Skyscraper.SBS.AutoShafts", true);
	AutoStairs = GetConfigBool("Skyscraper.SBS.AutoStairs", true);
	ElevatorSync = false;
	ElevatorNumber = 1;
	CarNumber = 1;
	wall_orientation = 1;
	floor_orientation = 2;
	DrawMainN = true;
	DrawMainP = true;
	DrawSideN = false;
	DrawSideP = false;
	DrawTop = false;
	DrawBottom = false;
	DrawMainNOld = true;
	DrawMainPOld = true;
	DrawSideNOld = false;
	DrawSidePOld = false;
	DrawTopOld = false;
	DrawBottomOld = false;
	delta = 0.01f;
	wall1a = false;
	wall1b = false;
	wall2a = false;
	wall2b = false;
	ProcessElevators = GetConfigBool("Skyscraper.SBS.ProcessElevators", true);
	remaining_delta = 0;
	start_time = 0;
	running_time = 0;
	InShaft = false;
	DeleteColliders = false;
	soundcount = 0;
	UnitScale = GetConfigFloat("Skyscraper.SBS.UnitScale", 4);
	Verbose = GetConfigBool("Skyscraper.SBS.Verbose", false);
	InterfloorOnTop = false;
	FastDelete = false;
	WallCount = 0;
	PolygonCount = 0;
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
	EscalatorCount = 0;
	MovingWalkwayCount = 0;
	RandomActivity = GetConfigBool("Skyscraper.SBS.RandomActivity", false);
	Headless = false;

	camera = 0;
	Buildings = 0;
	External = 0;
	Landscape = 0;
	mWorld = 0;
	soundsystem = 0;
	area_trigger = 0;
	texturemanager = 0;

	if (UnitScale <= 0)
		UnitScale = 1;

	//Print SBS banner
	PrintBanner();

	//add instance number to reports
	InstancePrompt = ToString(InstanceNumber) + "> ";

	//move to specified position
	Move(position);

	//rotate engine
	Rotate(0.0f, rotation, 0.0f);

	//create main engine area trigger
	SetBounds(area_min, area_max);

	//create sound system object if sound is enabled
	if (fmodsystem)
		soundsystem = new SoundSystem(this, fmodsystem);
}

void SBS::Initialize()
{
	//create texture manager
	texturemanager = new TextureManager(this);

	//set up physics
	Ogre::AxisAlignedBox box (Ogre::Vector3::ZERO, Ogre::Vector3::ZERO);
	mWorld = new OgreBulletDynamics::DynamicsWorld(mSceneManager, box, Ogre::Vector3::ZERO, true);
	mWorld->setAllowedCcdPenetration(0);

	/*debugDrawer = new OgreBulletCollisions::DebugDrawer();
	debugDrawer->setDrawWireframe(true);
	mWorld->setDebugDrawer(debugDrawer);
	Ogre::SceneNode *node = mSceneManager->getRootSceneNode()->createChildSceneNode("debugDrawer", Ogre::Vector3::ZERO);
	node->attachObject(static_cast<Ogre::SimpleRenderable*> (debugDrawer));
	*/

	//mount sign texture packs
	Mount("signs-sans.zip", "signs/sans");
	Mount("signs-sans_bold.zip", "signs/sans_bold");
	Mount("signs-sans_cond.zip", "signs/sans_cond");
	Mount("signs-sans_cond_bold.zip", "signs/sans_cond_bold");

	//create object meshes
	Buildings = new MeshObject(this, "Buildings");
	External = new MeshObject(this, "External");
	Landscape = new MeshObject(this, "Landscape");
	//Landscape->tricollider = false;

	//create manager objects
	floor_manager = new FloorManager(this);
	elevator_manager = new ElevatorManager(this);
	shaft_manager = new ShaftManager(this);
	stairs_manager = new StairsManager(this);
	door_manager = new DoorManager(this);
	revolvingdoor_manager = new RevolvingDoorManager(this);

	//create camera object
	this->camera = new Camera(this);
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

	//delete camera object
	if (camera)
	{
		camera->parent_deleting = true;
		delete camera;
	}
	camera = 0;

	//delete manager objects
	if (floor_manager)
		delete floor_manager;
	floor_manager = 0;

	if (elevator_manager)
		delete elevator_manager;
	elevator_manager = 0;

	if (shaft_manager)
		delete shaft_manager;
	shaft_manager = 0;

	if (stairs_manager)
		delete stairs_manager;
	stairs_manager = 0;

	if (door_manager)
		delete door_manager;
	door_manager = 0;

	if (revolvingdoor_manager)
		delete revolvingdoor_manager;
	revolvingdoor_manager = 0;

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
		delete soundsystem;
	soundsystem = 0;

	//delete texture manager
	if (texturemanager)
		delete texturemanager;
	texturemanager = 0;

	//delete main area trigger
	if (area_trigger)
		delete area_trigger;
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
	verify_results.clear();

	if (timer)
		delete timer;
	timer = 0;

	if (configfile)
		delete configfile;
	configfile = 0;

	//clear self reference
	sbs = 0;

	Report("Exiting");
}

bool SBS::Start(Ogre::Camera *camera)
{
	//Post-init startup code goes here, before the runloop

	//prepare 3D geometry for use
	Prepare();

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

	//attach camera object
	AttachCamera(camera);

	//enable elevators
	sbs->GetElevatorManager()->EnableAll(true);

	//enable random activity if specified
	if (RandomActivity == true)
		EnableRandomActivity(true);

	IsRunning = true;

	return true;
}

void SBS::PrintBanner()
{
	Report("");
	Report(" Scalable Building Simulator " + version + " " + version_state);
	Report(" Copyright (C)2004-2016 Ryan Thoryk");
	Report(" This software comes with ABSOLUTELY NO WARRANTY. This is free");
	Report(" software, and you are welcome to redistribute it under certain");
	Report(" conditions. For details, see the file gpl.txt\n");
}

void SBS::Loop()
{
	//Main simulator loop
	SBS_PROFILE("SBS::Loop");

	//This makes sure all timer steps are the same size, in order to prevent the physics from changing
	//depending on frame rate

	unsigned long timing;

	if (SmoothFrames > 0)
		timing = GetAverageTime();
	else
		timing = GetElapsedTime();

	float elapsed = float(timing) / 1000.0f;

	//calculate start and running time
	if (start_time == 0)
		start_time = GetRunTime() / 1000.0f;
	running_time = (GetRunTime() / 1000.0f) - start_time;

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
		soundsystem->Loop();

	elapsed += remaining_delta;

	//limit the elapsed value to prevent major slowdowns during debugging
	if (elapsed > .5f)
		elapsed = .5f;

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
}

void SBS::CalculateFrameRate()
{
	//calculate frame rate
	fps_tottime += elapsed_time;
	fps_frame_count++;
	if (fps_tottime > 500)
	{
		FPS = (float (fps_frame_count) * 1000.0f) / float (fps_tottime);
		fps_frame_count = 0;
		fps_tottime = 0;
	}
}

bool SBS::AddWallMain(Object *parent, MeshObject* mesh, const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float altitude1, float altitude2, float tw, float th, bool autosize)
{
	Wall *object = new Wall(mesh, parent, true);
	bool result = AddWallMain(object, name, texture, thickness, x1, z1, x2, z2, height_in1, height_in2, altitude1, altitude2, tw, th, autosize);
	delete object;
	return result;
}

bool SBS::AddWallMain(Wall* wallobject, const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float altitude1, float altitude2, float tw, float th, bool autosize)
{
	//Adds a wall with the specified dimensions

	//exit if coordinates are invalid
	if (x1 == x2 && z1 == z2)
		return ReportError("Invalid coordinates for wall '" + name + "'");

	if (height_in1 == 0.0f && height_in2 == 0.0f)
		return ReportError("No wall height specified for wall '" + name + "'");

	//determine axis of wall
	int axis = 0;
	if (std::abs(x1 - x2) > (std::abs(z1 - z2) + 0.00001))
		//x axis
		axis = 1;
	else
		//z axis
		axis = 2;

	//convert to clockwise coordinates if on x-axis, or counterclockwise if on z-axis
	if ((x1 > x2 && axis == 1) || (z1 < z2 && axis == 2))
	{
		//reverse coordinates
		Swap(x1, x2);
		Swap(z1, z2);
		Swap(altitude1, altitude2);
		Swap(height_in1, height_in2);
	}

	//map coordinates
	Ogre::Vector3 v1 (x1, altitude1 + height_in1, z1); //left top
	Ogre::Vector3 v2 (x2, altitude2 + height_in2, z2); //right top
	Ogre::Vector3 v3 (x2, altitude2, z2); //right base
	Ogre::Vector3 v4 (x1, altitude1, z1); //left base

	Ogre::Vector3 v5 = v1;
	Ogre::Vector3 v6 = v2;
	Ogre::Vector3 v7 = v3;
	Ogre::Vector3 v8 = v4;

	//exit if outside of the engine boundaries
	if (area_trigger)
	{
		Ogre::Vector3 v1x = wallobject->GetMesh()->GetPosition() + v1;
		Ogre::Vector3 v2x = wallobject->GetMesh()->GetPosition() + v3;
		if (area_trigger->IsOutside(v1x, v2x) == true)
			return false;
	}

	//expand to specified thickness
	if (axis == 1)
	{
		//x axis
		if (wall_orientation == 0)
		{
			//left
			v5.z += thickness;
			v6.z += thickness;
			v7.z += thickness;
			v8.z += thickness;
		}
		if (wall_orientation == 1)
		{
			//center
			float half = thickness / 2;
			v1.z -= half;
			v2.z -= half;
			v3.z -= half;
			v4.z -= half;
			v5.z += half;
			v6.z += half;
			v7.z += half;
			v8.z += half;
		}
		if (wall_orientation == 2)
		{
			//right
			v1.z -= thickness;
			v2.z -= thickness;
			v3.z -= thickness;
			v4.z -= thickness;
		}
	}
	else
	{
		//z axis
		if (wall_orientation == 0)
		{
			//left
			v5.x += thickness;
			v6.x += thickness;
			v7.x += thickness;
			v8.x += thickness;
		}
		if (wall_orientation == 1)
		{
			//center
			float half = thickness / 2;
			v1.x -= half;
			v2.x -= half;
			v3.x -= half;
			v4.x -= half;
			v5.x += half;
			v6.x += half;
			v7.x += half;
			v8.x += half;
		}
		if (wall_orientation == 2)
		{
			//right
			v1.x -= thickness;
			v2.x -= thickness;
			v3.x -= thickness;
			v4.x -= thickness;
		}
	}

	//create polygons and set names
	std::string NewName, texture2 = texture;
	float tw2 = tw, th2 = th;

	bool FlipTexture = texturemanager->FlipTexture;
	bool TextureOverride = texturemanager->TextureOverride;

	if (FlipTexture == true)
		texturemanager->ProcessTextureFlip(tw, th);

	if (DrawMainN == true)
	{
		if (FlipTexture == true)
		{
			tw2 = texturemanager->widthscale[0];
			th2 = texturemanager->heightscale[0];
		}
		if (TextureOverride == true)
			texture2 = texturemanager->mainnegtex;

		NewName = name;
		if (GetDrawWallsCount() > 1)
			NewName.append(":front");
		wallobject->AddQuad(NewName, texture2, v1, v2, v3, v4, tw2, th2, autosize); //front wall
	}

	if (DrawMainP == true)
	{
		if (FlipTexture == true)
		{
			tw2 = texturemanager->widthscale[1];
			th2 = texturemanager->heightscale[1];
		}
		if (TextureOverride == true)
			texture2 = texturemanager->mainpostex;

		NewName = name;
		NewName.append(":back");
		wallobject->AddQuad(NewName, texture2, v6, v5, v8, v7, tw2, th2, autosize); //back wall
	}

	if (thickness != 0.0f)
	{
		if (DrawSideN == true)
		{
			if (FlipTexture == true)
			{
				tw2 = texturemanager->widthscale[2];
				th2 = texturemanager->heightscale[2];
			}
			if (TextureOverride == true)
				texture2 = texturemanager->sidenegtex;

			NewName = name;
			NewName.append(":left");
			if (axis == 1)
				wallobject->AddQuad(NewName, texture2, v5, v1, v4, v8, tw2, th2, autosize); //left wall
			else
				wallobject->AddQuad(NewName, texture2, v2, v6, v7, v3, tw2, th2, autosize); //left wall
		}

		if (DrawSideP == true)
		{
			if (FlipTexture == true)
			{
				tw2 = texturemanager->widthscale[3];
				th2 = texturemanager->heightscale[3];
			}
			if (TextureOverride == true)
				texture2 = texturemanager->sidepostex;

			NewName = name;
			NewName.append(":right");
			if (axis == 1)
				wallobject->AddQuad(NewName, texture2, v2, v6, v7, v3, tw2, th2, autosize); //right wall
			else
				wallobject->AddQuad(NewName, texture2, v5, v1, v4, v8, tw2, th2, autosize); //right wall
		}

		if (DrawTop == true)
		{
			if (FlipTexture == true)
			{
				tw2 = texturemanager->widthscale[4];
				th2 = texturemanager->heightscale[4];
			}
			if (TextureOverride == true)
				texture2 = texturemanager->toptex;

			NewName = name;
			NewName.append(":top");
			wallobject->AddQuad(NewName, texture2, v5, v6, v2, v1, tw2, th2, autosize); //top wall
		}

		if (DrawBottom == true)
		{
			if (FlipTexture == true)
			{
				tw2 = texturemanager->widthscale[5];
				th2 = texturemanager->heightscale[5];
			}
			if (TextureOverride == true)
				texture2 = texturemanager->bottomtex;

			NewName = name;
			NewName.append(":bottom");
			wallobject->AddQuad(NewName, texture2, v4, v3, v7, v8, tw2, th2, autosize); //bottom wall
		}
	}

	return true;
}

bool SBS::AddFloorMain(Object *parent, MeshObject* mesh, const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float altitude1, float altitude2, bool reverse_axis, bool texture_direction, float tw, float th, bool autosize, bool legacy_behavior)
{
	Wall *object = new Wall(mesh, parent, true);
	bool result = AddFloorMain(object, name, texture, thickness, x1, z1, x2, z2, altitude1, altitude2, reverse_axis, texture_direction, tw, th, autosize, legacy_behavior);
	delete object;
	return result;
}

bool SBS::AddFloorMain(Wall* wallobject, const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float altitude1, float altitude2, bool reverse_axis, bool texture_direction, float tw, float th, bool autosize, bool legacy_behavior)
{
	//Adds a floor with the specified dimensions and vertical offset

	//direction determines the direction of slope (for different altitude values):
	//false - left/right from altitude1 to altitude2, or legacy (broken) "ReverseAxis = false" behavior if legacy_behavior is true
	//true - back/forwards from altitude1 to altitude2, or legacy (broken) "ReverseAxis = true" behavior if legacy_behavior is true

	//exit if coordinates are invalid
	if (x1 == x2 || z1 == z2)
		return ReportError("Invalid coordinates for floor '" + name + "'");

	//convert to clockwise coordinates

	//determine axis of floor
	int axis = 0;
	if (std::abs(x1 - x2) > (std::abs(z1 - z2) + 0.00001))
		//x axis
		axis = 1;
	else
		//z axis
		axis = 2;

	if (legacy_behavior == false)
	{
		//current behavior

		if (x1 > x2)
		{
			Swap(x1, x2);

			if (reverse_axis == true)
				Swap(altitude1, altitude2);
		}
		if (z1 > z2)
		{
			Swap(z1, z2);

			if (reverse_axis == false)
				Swap(altitude1, altitude2);
		}
	}
	else
	{
		//legacy (broken) behavior, for compatibility with previous versions

		if ((x1 > x2 && axis == 1) || (z1 > z2 && axis == 2))
		{
			//reverse coordinates if the difference between x or z coordinates is greater
			Swap(x1, x2);
			Swap(z1, z2);
			Swap(altitude1, altitude2);
		}
	}

	//map coordinates
	Ogre::Vector3 v1, v2, v3, v4;

	if (reverse_axis == false)
	{
		v1 = Ogre::Vector3(x1, altitude1, z1); //bottom left
		v2 = Ogre::Vector3(x2, altitude1, z1); //bottom right
		v3 = Ogre::Vector3(x2, altitude2, z2); //top right
		v4 = Ogre::Vector3(x1, altitude2, z2); //top left
	}
	else
	{
		if (legacy_behavior == true)
		{
			v1 = Ogre::Vector3(x1, altitude1, z1); //bottom left
			v2 = Ogre::Vector3(x1, altitude1, z2); //top left
			v3 = Ogre::Vector3(x2, altitude2, z2); //top right
			v4 = Ogre::Vector3(x2, altitude2, z1); //bottom right
		}
		else
		{
			v1 = Ogre::Vector3(x2, altitude2, z1); //bottom right
			v2 = Ogre::Vector3(x2, altitude2, z2); //top right
			v3 = Ogre::Vector3(x1, altitude1, z2); //top left
			v4 = Ogre::Vector3(x1, altitude1, z1); //bottom left
		}
	}

	Ogre::Vector3 v5 = v1;
	Ogre::Vector3 v6 = v2;
	Ogre::Vector3 v7 = v3;
	Ogre::Vector3 v8 = v4;

	//exit if outside of the engine boundaries
	if (area_trigger)
	{
		Ogre::Vector3 v1x = wallobject->GetMesh()->GetPosition() + v1;
		Ogre::Vector3 v2x = wallobject->GetMesh()->GetPosition() + v3;
		if (area_trigger->IsOutside(v1x, v2x) == true)
			return false;
	}

	//expand to specified thickness
	if (floor_orientation == 0)
	{
		//bottom
		v5.y += thickness;
		v6.y += thickness;
		v7.y += thickness;
		v8.y += thickness;
	}
	if (floor_orientation == 1)
	{
		//center
		float half = thickness / 2;
		v1.y -= half;
		v2.y -= half;
		v3.y -= half;
		v4.y -= half;
		v5.y += half;
		v6.y += half;
		v7.y += half;
		v8.y += half;
	}
	if (floor_orientation == 2)
	{
		//top
		v1.y -= thickness;
		v2.y -= thickness;
		v3.y -= thickness;
		v4.y -= thickness;
	}

	//create polygons and set names
	std::string NewName, texture2 = texture;
	float tw2 = tw, th2 = th;

	bool FlipTexture = texturemanager->FlipTexture;
	bool TextureOverride = texturemanager->TextureOverride;

	if (FlipTexture == true)
		texturemanager->ProcessTextureFlip(tw, th);

	//turn on rotation if set
	bool old_planarrotate = texturemanager->GetPlanarRotate();
	texturemanager->SetPlanarRotate(texture_direction);

	if (DrawMainN == true)
	{
		if (FlipTexture == true)
		{
			tw2 = texturemanager->widthscale[0];
			th2 = texturemanager->heightscale[0];
		}
		if (TextureOverride == true)
			texture2 = texturemanager->mainnegtex;

		NewName = name;
		if (GetDrawWallsCount() > 1)
			NewName.append(":front");
		wallobject->AddQuad(NewName, texture2, v1, v2, v3, v4, tw2, th2, autosize); //bottom wall
	}

	if (DrawMainP == true)
	{
		if (FlipTexture == true)
		{
			tw2 = texturemanager->widthscale[1];
			th2 = texturemanager->heightscale[1];
		}
		if (TextureOverride == true)
			texture2 = texturemanager->mainpostex;

		NewName = name;
		NewName.append(":back");
		wallobject->AddQuad(NewName, texture2, v8, v7, v6, v5, tw2, th2, autosize); //top wall
	}

	if (thickness != 0.0f)
	{
		if (DrawSideN == true)
		{
			if (FlipTexture == true)
			{
				tw2 = texturemanager->widthscale[2];
				th2 = texturemanager->heightscale[2];
			}
			if (TextureOverride == true)
				texture2 = texturemanager->sidenegtex;

			NewName = name;
			NewName.append(":left");
			wallobject->AddQuad(NewName, texture2, v8, v5, v1, v4, tw2, th2, autosize); //left wall
		}

		if (DrawSideP == true)
		{
			if (FlipTexture == true)
			{
				tw2 = texturemanager->widthscale[3];
				th2 = texturemanager->heightscale[3];
			}
			if (TextureOverride == true)
				texture2 = texturemanager->sidepostex;

			NewName = name;
			NewName.append(":right");
			wallobject->AddQuad(NewName, texture2, v6, v7, v3, v2, tw2, th2, autosize); //right wall
		}

		if (DrawTop == true)
		{
			if (FlipTexture == true)
			{
				tw2 = texturemanager->widthscale[4];
				th2 = texturemanager->heightscale[4];
			}
			if (TextureOverride == true)
				texture2 = texturemanager->toptex;

			NewName = name;
			NewName.append(":top");
			wallobject->AddQuad(NewName, texture2, v5, v6, v2, v1, tw2, th2, autosize); //front wall
		}

		if (DrawBottom == true)
		{
			if (FlipTexture == true)
			{
				tw2 = texturemanager->widthscale[5];
				th2 = texturemanager->heightscale[5];
			}
			if (TextureOverride == true)
				texture2 = texturemanager->bottomtex;

			NewName = name;
			NewName.append(":bottom");
			wallobject->AddQuad(NewName, texture2, v7, v8, v4, v3, tw2, th2, autosize); //back wall
		}
	}

	texturemanager->SetPlanarRotate(old_planarrotate);

	return true;
}

void SBS::Report(const std::string &message)
{
	Ogre::LogManager::getSingleton().logMessage(InstancePrompt + message);
	LastNotification = message;
}

bool SBS::ReportError(const std::string &message)
{
	Ogre::LogManager::getSingleton().logMessage(InstancePrompt + message, Ogre::LML_CRITICAL);
	LastError = message;
	return false;
}

Wall* SBS::CreateWallBox(MeshObject* mesh, const std::string &name, const std::string &texture, float x1, float x2, float z1, float z2, float height_in, float voffset, float tw, float th, bool inside, bool outside, bool top, bool bottom, bool autosize)
{
	//create 4 walls

	if (!mesh)
		return 0;

	//exit if coordinates are invalid
	if (x1 == x2 && z1 == z2)
	{
		ReportError("Invalid coordinates for wall '" + name + "'");
		return 0;
	}

	//create wall object
	Wall *wall = mesh->CreateWallObject(name);

	bool x_thickness = false, z_thickness = false;
	std::string NewName, texture2 = texture;

	if (x1 != x2)
		x_thickness = true;
	if (z1 != z2)
		z_thickness = true;

	//swap values if the first is greater than the second
	if (x1 > x2)
		Swap(x1, x2);

	if (z1 > z2)
		Swap(z1, z2);

	bool TextureOverride = texturemanager->TextureOverride;

	if (inside == true)
	{
		//generate a box visible from the inside

		NewName = name;
		NewName.append(":inside");

		if (x_thickness == true)
		{
			if (TextureOverride == true)
				texture2 = texturemanager->mainnegtex;

			wall->AddQuad( //front
					NewName,
					texture2,
					Ogre::Vector3(x1, voffset, z1),
					Ogre::Vector3(x2, voffset, z1),
					Ogre::Vector3(x2, voffset + height_in, z1),
					Ogre::Vector3(x1, voffset + height_in, z1), tw, th, autosize);

			if (TextureOverride == true)
				texture2 = texturemanager->mainpostex;

			wall->AddQuad( //back
					NewName,
					texture2,
					Ogre::Vector3(x2, voffset, z2),
					Ogre::Vector3(x1, voffset, z2),
					Ogre::Vector3(x1, voffset + height_in, z2),
					Ogre::Vector3(x2, voffset + height_in, z2), tw, th, autosize);
		}
		if (z_thickness == true)
		{
			if (TextureOverride == true)
				texture2 = texturemanager->sidepostex;

			wall->AddQuad( //right
					NewName,
					texture2,
					Ogre::Vector3(x2, voffset, z1),
					Ogre::Vector3(x2, voffset, z2),
					Ogre::Vector3(x2, voffset + height_in, z2),
					Ogre::Vector3(x2, voffset + height_in, z1), tw, th, autosize);

			if (TextureOverride == true)
				texture2 = texturemanager->sidenegtex;

			wall->AddQuad( //left
					NewName,
					texture2,
					Ogre::Vector3(x1, voffset, z2),
					Ogre::Vector3(x1, voffset, z1),
					Ogre::Vector3(x1, voffset + height_in, z1),
					Ogre::Vector3(x1, voffset + height_in, z2), tw, th, autosize);
		}
		if (x_thickness == true && z_thickness == true)
		{
			if (bottom == true)
			{
				if (TextureOverride == true)
					texture2 = texturemanager->bottomtex;

				wall->AddQuad( //bottom
						NewName,
						texture2,
						Ogre::Vector3(x1, voffset, z2),
						Ogre::Vector3(x2, voffset, z2),
						Ogre::Vector3(x2, voffset, z1),
						Ogre::Vector3(x1, voffset, z1), tw, th, autosize);
			}

			if (top == true)
			{
				if (TextureOverride == true)
					texture2 = texturemanager->toptex;

				wall->AddQuad( //top
						NewName,
						texture2,
						Ogre::Vector3(x1, voffset + height_in, z1),
						Ogre::Vector3(x2, voffset + height_in, z1),
						Ogre::Vector3(x2, voffset + height_in, z2),
						Ogre::Vector3(x1, voffset + height_in, z2), tw, th, autosize);
			}
		}
	}

	if (outside == true)
	{
		NewName = name;
		NewName.append(":outside");

		if (x_thickness == true)
		{
			if (TextureOverride == true)
				texture2 = texturemanager->mainnegtex;

			wall->AddQuad( //front
					NewName,
					texture2,
					Ogre::Vector3(x1, voffset + height_in, z1),
					Ogre::Vector3(x2, voffset + height_in, z1),
					Ogre::Vector3(x2, voffset, z1),
					Ogre::Vector3(x1, voffset, z1), tw, th, autosize);

			if (TextureOverride == true)
				texture2 = texturemanager->mainpostex;

			wall->AddQuad( //back
					NewName,
					texture2,
					Ogre::Vector3(x2, voffset + height_in, z2),
					Ogre::Vector3(x1, voffset + height_in, z2),
					Ogre::Vector3(x1, voffset, z2),
					Ogre::Vector3(x2, voffset, z2), tw, th, autosize);
		}
		if (z_thickness == true)
		{
			if (TextureOverride == true)
				texture2 = texturemanager->sidepostex;

			wall->AddQuad( //right
					NewName,
					texture2,
					Ogre::Vector3(x2, voffset + height_in, z1),
					Ogre::Vector3(x2, voffset + height_in, z2),
					Ogre::Vector3(x2, voffset, z2),
					Ogre::Vector3(x2, voffset, z1), tw, th, autosize);

			if (TextureOverride == true)
				texture2 = texturemanager->sidenegtex;

			wall->AddQuad( //left
					NewName,
					texture2,
					Ogre::Vector3(x1, voffset + height_in, z2),
					Ogre::Vector3(x1, voffset + height_in, z1),
					Ogre::Vector3(x1, voffset, z1),
					Ogre::Vector3(x1, voffset, z2), tw, th, autosize);
		}
		if (x_thickness == true && z_thickness == true)
		{
			if (bottom == true)
			{
				if (TextureOverride == true)
					texture2 = texturemanager->bottomtex;

				wall->AddQuad( //bottom
						NewName,
						texture2,
						Ogre::Vector3(x1, voffset, z1),
						Ogre::Vector3(x2, voffset, z1),
						Ogre::Vector3(x2, voffset, z2),
						Ogre::Vector3(x1, voffset, z2), tw, th, autosize);
			}
			if (top == true)
			{
				if (TextureOverride == true)
					texture2 = texturemanager->toptex;

				wall->AddQuad( //top
						NewName,
						texture2,
						Ogre::Vector3(x1, voffset + height_in, z2),
						Ogre::Vector3(x2, voffset + height_in, z2),
						Ogre::Vector3(x2, voffset + height_in, z1),
						Ogre::Vector3(x1, voffset + height_in, z1), tw, th, autosize);
			}
		}
	}
	return wall;
}

Wall* SBS::CreateWallBox2(MeshObject* mesh, const std::string &name, const std::string &texture, float CenterX, float CenterZ, float WidthX, float LengthZ, float height_in, float voffset, float tw, float th, bool inside, bool outside, bool top, bool bottom, bool autosize)
{
	//create 4 walls from a central point

	float x1 = CenterX - (WidthX / 2);
	float x2 = CenterX + (WidthX / 2);
	float z1 = CenterZ - (LengthZ / 2);
	float z2 = CenterZ + (LengthZ / 2);

	return CreateWallBox(mesh, name, texture, x1, x2, z1, z2, height_in, voffset, tw, th, inside, outside, top, bottom, autosize);
}

void SBS::AddPolygon(Wall* wallobject, const std::string &texture, std::vector<Ogre::Vector3> &varray, float tw, float th)
{
	//creates a polygon in the specified wall object

	if (!wallobject)
		return;

	std::vector<Ogre::Vector3> varray1 = varray;
	std::vector<Ogre::Vector3> varray2;

	//get number of stored vertices
	size_t num = varray.size();

	//create a second array with reversed vertices
	varray2.reserve(num);
	for (size_t i = num - 1; i < num; --i)
		varray2.push_back(varray1[i]);

	//create 2 polygons (front and back) from the vertex array

	//get polygon native direction
	Ogre::Vector3 direction = GetPolygonDirection(varray1);

	//if the polygon is facing right, down or to the back, reverse faces
	//to keep the vertices clockwise
	std::vector<Ogre::Vector3> tmppoly;
	if (direction.x == 1 || direction.y == -1 || direction.z == 1)
	{
		tmppoly = varray1;
		varray1 = varray2;
		varray2 = tmppoly;
	}

	std::string name = wallobject->GetName();

	//add the polygons
	if (DrawMainN == true)
	{
		std::string NewName = name;
		if (DrawMainP == true)
			NewName.append(":0");
		wallobject->AddPolygon(NewName, texture, varray1, tw, th, true);
	}
	if (DrawMainP == true)
	{
		std::string NewName = name;
		if (DrawMainN == true)
			NewName.append(":1");
		wallobject->AddPolygon(NewName, texture, varray2, tw, th, true);
	}
}

Wall* SBS::AddCustomWall(MeshObject* mesh, const std::string &name, const std::string &texture, std::vector<Ogre::Vector3> &varray, float tw, float th)
{
	//Adds a wall from a specified array of 3D vectors

	if (!mesh)
		return 0;

	//create wall object
	Wall *wall = mesh->CreateWallObject(name);

	//create polygon in wall object
	AddPolygon(wall, texture, varray, tw, th);

	return wall;
}

Wall* SBS::AddCustomFloor(MeshObject* mesh, const std::string &name, const std::string &texture, std::vector<Ogre::Vector2> &varray, float altitude, float tw, float th)
{
	//Same as AddCustomWall, with only one altitude value value
	std::vector<Ogre::Vector3> varray3;

	//set up 3D vertex array
	varray3.reserve(varray.size());
	for (size_t i = 0; i < varray.size(); i++)
	{
		varray3.push_back(Ogre::Vector3(varray[i].x, altitude, varray[i].y));
	}

	//pass data on to AddCustomWall function
	return AddCustomWall(mesh, name, texture, varray3, tw, th);
}

Wall* SBS::AddTriangleWall(MeshObject* mesh, const std::string &name, const std::string &texture, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float tw, float th)
{
	//Adds a triangular wall with the specified dimensions
	std::vector<Ogre::Vector3> varray;

	//set up temporary vertex array
	varray.reserve(3);
	varray.push_back(Ogre::Vector3(x1, y1, z1));
	varray.push_back(Ogre::Vector3(x2, y2, z2));
	varray.push_back(Ogre::Vector3(x3, y3, z3));

	//pass data on to AddCustomWall function
	return AddCustomWall(mesh, name, texture, varray, tw, th);
}

void SBS::EnableBuildings(bool value)
{
	//turns buildings on/off
	Buildings->Enabled(value);
	IsBuildingsEnabled = value;
}

void SBS::EnableLandscape(bool value)
{
	//turns landscape on/off
	Landscape->Enabled(value);
	IsLandscapeEnabled = value;
}

void SBS::EnableExternal(bool value)
{
	//turns external on/off
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

	Mount("sky-" + SkyName + ".zip", "sky");

	//load textures
	SetLighting();
	texturemanager->LoadTexture("sky/up.jpg", "SkyTop", 1, 1, false, false, false, 0);
	texturemanager->LoadTexture("sky/down.jpg", "SkyBottom", 1, 1, false, false, false, 0);
	texturemanager->LoadTexture("sky/left.jpg", "SkyLeft", 1, 1, false, false, false, 0);
	texturemanager->LoadTexture("sky/right.jpg", "SkyRight", 1, 1, false, false, false, 0);
	texturemanager->LoadTexture("sky/front.jpg", "SkyFront", 1, 1, false, false, false, 0);
	texturemanager->LoadTexture("sky/back.jpg", "SkyBack", 1, 1, false, false, false, 0);
	ResetLighting();

	SkyBox = new MeshObject(this, "SkyBox");
	SkyBox->no_collider = true;

	//create a skybox that extends by default 30 miles (30 * 5280 ft) in each direction
	float skysize = GetConfigInt("Skyscraper.SBS.HorizonDistance", 30) * 5280.0f;
	texturemanager->ResetTextureMapping(true);
	Wall *wall = new Wall(SkyBox, SkyBox, true);

	wall->AddQuad( //front
		"SkyFront",
		"SkyFront",
		Ogre::Vector3(-skysize, -skysize, -skysize),
		Ogre::Vector3(skysize, -skysize, -skysize),
		Ogre::Vector3(skysize, skysize, -skysize),
		Ogre::Vector3(-skysize, skysize, -skysize), 1, 1, false);
	wall->AddQuad( //right
		"SkyRight",
		"SkyRight",
		Ogre::Vector3(skysize, -skysize, -skysize),
		Ogre::Vector3(skysize, -skysize, skysize),
		Ogre::Vector3(skysize, skysize, skysize),
		Ogre::Vector3(skysize, skysize, -skysize), 1, 1, false);
	wall->AddQuad( //back
		"SkyBack",
		"SkyBack",
		Ogre::Vector3(skysize, -skysize, skysize),
		Ogre::Vector3(-skysize, -skysize, skysize),
		Ogre::Vector3(-skysize, skysize, skysize),
		Ogre::Vector3(skysize, skysize, skysize), 1, 1, false);
	wall->AddQuad( //left
		"SkyLeft",
		"SkyLeft",
		Ogre::Vector3(-skysize, -skysize, skysize),
		Ogre::Vector3(-skysize, -skysize, -skysize),
		Ogre::Vector3(-skysize, skysize, -skysize),
		Ogre::Vector3(-skysize, skysize, skysize), 1, 1, false);
	wall->AddQuad( //bottom
		"SkyBottom",
		"SkyBottom",
		Ogre::Vector3(-skysize, -skysize, skysize),
		Ogre::Vector3(skysize, -skysize, skysize),
		Ogre::Vector3(skysize, -skysize, -skysize),
		Ogre::Vector3(-skysize, -skysize, -skysize), 1, -1, false);
	wall->AddQuad( //top
		"SkyTop",
		"SkyTop",
		Ogre::Vector3(-skysize, skysize, -skysize),
		Ogre::Vector3(skysize, skysize, -skysize),
		Ogre::Vector3(skysize, skysize, skysize),
		Ogre::Vector3(-skysize, skysize, skysize), -1, -1, false);

	texturemanager->ResetTextureMapping();
	delete wall;
}

int SBS::GetFloorNumber(float altitude, int lastfloor, bool checklastfloor)
{
	//Returns floor number located at a specified altitude

	if (GetTotalFloors() == 0)
		return 0;

	//check to see if altitude is below bottom floor
	if (altitude < GetFloor(-Basements)->Altitude)
		return -Basements;

	//if checklastfloor is specified, compare altitude with lastfloor
	if (checklastfloor == true && GetFloor(lastfloor))
	{
		float lastfloor_altitude = GetFloor(lastfloor)->Altitude;
		float upperfloor_altitude;
		if (lastfloor < Floors - 1)
			upperfloor_altitude = GetFloor(lastfloor + 1)->Altitude;
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
					if (GetFloor(i + 1)->Altitude > altitude && GetFloor(i)->Altitude <= altitude)
						return i;
				}
			}
			else if (altitude >= upperfloor_altitude)
			{
				for (int i = lastfloor + 1; i < Floors; i++)
				{
					if (GetFloor(i - 1)->Altitude <= altitude && GetFloor(i)->Altitude > altitude)
						return i - 1;
					if (i == Floors - 1 && GetFloor(i)->Altitude <= altitude)
						return i; //return top floor if on top
				}
			}
		}
	}

	//otherwise do a slow linear search through floors
	for (int i = -Basements + 1; i < Floors; i++)
	{
		//check to see if altitude is within a floor (between the current floor's base and
		//the lower floor's base)
		if ((GetFloor(i)->Altitude > altitude) && (GetFloor(i - 1)->Altitude <= altitude))
			return i - 1;
		//check to see if altitude is above top floor's altitude
		if ((i == Floors - 1) && (altitude > GetFloor(i)->Altitude))
			return i;
	}
	return 0;
}

float SBS::GetDistance(float x1, float x2, float z1, float z2)
{
	//returns the distance between 2 2D vectors

	if (z1 == z2)
		return std::abs(x1 - x2);
	if (x1 == x2)
		return std::abs(z1 - z2);
	if ((x1 != x2) && (z2 != x2))
		return sqrtf(powf(std::abs(x1 - x2), 2) + powf(std::abs(z1 - z2), 2)); //calculate diagonals
	return 0;
}

Shaft* SBS::CreateShaft(int number, float CenterX, float CenterZ, int startfloor, int endfloor)
{
	//create a shaft object

	return shaft_manager->Create(number, CenterX, CenterZ, startfloor, endfloor);
}

Stairs* SBS::CreateStairwell(int number, float CenterX, float CenterZ, int startfloor, int endfloor)
{
	//create a stairwell object

	return stairs_manager->Create(number, CenterX, CenterZ, startfloor, endfloor);
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

int SBS::GetElevatorCount()
{
	//return the number of elevators
	return elevator_manager->GetCount();
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

int SBS::GetStairsCount()
{
	//return the number of stairs
	return stairs_manager->GetCount();
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

Stairs* SBS::GetStairs(int number)
{
	//return pointer to stairs object

	return stairs_manager->Get(number);
}

bool SBS::SetWallOrientation(std::string direction)
{
	//changes internal wall orientation parameter.
	//direction can either be "left" (negative), "center" (0), or "right" (positive).
	//default on startup is 1, or center.
	//the parameter is used to determine the location of the wall's
	//x1/x2 or z1/z2 coordinates in relation to the thickness extents

	SetCase(direction, false);

	if (direction == "left")
		wall_orientation = 0;
	else if (direction == "center")
		wall_orientation = 1;
	else if (direction == "right")
		wall_orientation = 2;
	else
		return ReportError("SetWallOrientation: Invalid wall orientation");
	return true;
}

int SBS::GetWallOrientation()
{
	return wall_orientation;
}

bool SBS::SetFloorOrientation(std::string direction)
{
	//changes internal floor orientation parameter.
	//direction can either be "bottom" (negative), "center" (0), or "top" (positive).
	//default on startup is 2, or top.
	//the parameter is used to determine the location of the floor's
	//x1/x2 or z1/z2 coordinates in relation to the thickness extents

	SetCase(direction, false);

	if (direction == "bottom")
		floor_orientation = 0;
	else if (direction == "center")
		floor_orientation = 1;
	else if (direction == "top")
		floor_orientation = 2;
	else
		return ReportError("SetFloorOrientation: Invalid floor orientation");
	return true;
}

int SBS::GetFloorOrientation()
{
	return floor_orientation;
}

void SBS::DrawWalls(bool MainN, bool MainP, bool SideN, bool SideP, bool Top, bool Bottom)
{
	//sets which walls should be drawn

	//first backup old parameters
	DrawMainNOld = DrawMainN;
	DrawMainPOld = DrawMainP;
	DrawSideNOld = DrawSideN;
	DrawSidePOld = DrawSideP;
	DrawTopOld = DrawTop;
	DrawBottomOld = DrawBottom;

	//now set new parameters
	DrawMainN = MainN;
	DrawMainP = MainP;
	DrawSideN = SideN;
	DrawSideP = SideP;
	DrawTop = Top;
	DrawBottom = Bottom;
}

void SBS::ResetWalls(bool ToDefaults)
{
	//if ToDefaults is true, this resets the DrawWalls data to the defaults.
	//if ToDefaults is false, this reverts the DrawWalls data to the previous settings.

	if (ToDefaults == true)
		DrawWalls(true, true, false, false, false, false);
	else
		DrawWalls(DrawMainNOld, DrawMainPOld, DrawSideNOld, DrawSidePOld, DrawTopOld, DrawBottomOld);
}

int SBS::GetDrawWallsCount()
{
	//gets the number of wall polygons enabled

	int sides = 0;

	if (DrawMainN == true)
		sides++;
	if (DrawMainP == true)
		sides++;
	if (DrawSideN == true)
		sides++;
	if (DrawSideP == true)
		sides++;
	if (DrawTop == true)
		sides++;
	if (DrawBottom == true)
		sides++;

	return sides;
}

float SBS::MetersToFeet(float meters)
{
	//converts meters to feet
	return meters * 3.2808399f;
}

float SBS::FeetToMeters(float feet)
{
	//converts feet to meters
	return feet / 3.2808399f;
}

Wall* SBS::AddDoorwayWalls(MeshObject* mesh, const std::string &wallname, const std::string &texture, float tw, float th)
{
	//add joining doorway polygons if needed

	if (!mesh)
		return 0;

	if (wall1a == true && wall2a == true)
	{
		Wall *wall = mesh->CreateWallObject(wallname);

		//convert extents to relative positioning
		Ogre::Vector2 extents_x = wall_extents_x - wall->GetMesh()->GetPosition().x;
		Ogre::Vector2 extents_y = wall_extents_y - wall->GetMesh()->GetPosition().y;
		Ogre::Vector2 extents_z = wall_extents_z - wall->GetMesh()->GetPosition().z;

		//true if doorway is facing forward/backward
		//false if doorway is facing left/right
		bool direction = std::abs(extents_x.x - extents_x.y) > std::abs(extents_z.x - extents_z.y);

		DrawWalls(false, true, false, false, false, false);
		if (direction == true)
			AddWallMain(wall, "DoorwayLeft", texture, 0, extents_x.x, extents_z.x, extents_x.x, extents_z.y, extents_y.y - extents_y.x, extents_y.y - extents_y.x, extents_y.x, extents_y.x, tw, th, true);
		else
			AddWallMain(wall, "DoorwayLeft", texture, 0, extents_x.x, extents_z.x, extents_x.y, extents_z.x, extents_y.y - extents_y.x, extents_y.y - extents_y.x, extents_y.x, extents_y.x, tw, th, true);
		ResetWalls();

		DrawWalls(true, false, false, false, false, false);
		if (direction == true)
			AddWallMain(wall, "DoorwayRight", texture, 0, extents_x.y, extents_z.x, extents_x.y, extents_z.y, extents_y.y - extents_y.x, extents_y.y - extents_y.x, extents_y.x, extents_y.x, tw, th, true);
		else
			AddWallMain(wall, "DoorwayRight", texture, 0, extents_x.x, extents_z.y, extents_x.y, extents_z.y, extents_y.y - extents_y.x, extents_y.y - extents_y.x, extents_y.x, extents_y.x, tw, th, true);

		AddFloorMain(wall, "DoorwayTop", texture, 0, extents_x.x, extents_z.x, extents_x.y, extents_z.y, extents_y.y, extents_y.y, false, false, tw, th, true);
		ResetWalls();

		ResetDoorwayWalls();

		return wall;
	}

	return 0;
}

void SBS::ResetDoorwayWalls()
{
	wall1a = false;
	wall1b = false;
	wall2a = false;
	wall2b = false;
	wall_extents_x = 0;
	wall_extents_y = 0;
	wall_extents_z = 0;
}

Wall* SBS::AddWall(MeshObject* mesh, const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float altitude1, float altitude2, float tw, float th)
{
	//Adds a wall with the specified dimensions, to the specified mesh object

	if (!mesh)
		return 0;

	Wall *wall = mesh->CreateWallObject(name);

	AddWallMain(wall, name, texture, thickness, x1, z1, x2, z2, height_in1, height_in2, altitude1, altitude2, tw, th, true);
	return wall;
}

Wall* SBS::AddFloor(MeshObject* mesh, const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float altitude1, float altitude2, bool reverse_axis, bool texture_direction, float tw, float th, bool legacy_behavior)
{
	//Adds a floor with the specified dimensions and vertical offset, to the specified mesh object

	if (!mesh)
		return 0;

	Wall *wall = mesh->CreateWallObject(name);

	AddFloorMain(wall, name, texture, thickness, x1, z1, x2, z2, altitude1, altitude2, reverse_axis, texture_direction, tw, th, true, legacy_behavior);
	return wall;
}

Wall* SBS::AddGround(const std::string &name, const std::string &texture, float x1, float z1, float x2, float z2, float altitude, int tile_x, int tile_z)
{
	//Adds ground based on a tiled-floor layout, with the specified dimensions and vertical offset
	//this does not support thickness

	Ogre::Vector3 v1, v2, v3, v4;

	float minx, minz, maxx, maxz;

	//get min and max values
	if (x1 < x2)
	{
		minx = x1;
		maxx = x2;
	}
	else
	{
		minx = x2;
		maxx = x1;
	}
	if (z1 < z2)
	{
		minz = z1;
		maxz = z2;
	}
	else
	{
		minz = z2;
		maxz = z1;
	}

	Wall *wall = Landscape->CreateWallObject(name);

	Report("Creating ground...");

	//create polygon tiles
	for (float i = minx; i < maxx; i += tile_x)
	{
		float sizex, sizez;

		if (i + tile_x > maxx)
			sizex = maxx - i;
		else
			sizex = (float)tile_x;

		for (float j = minz; j < maxz; j += tile_z)
		{
			if (j + tile_z > maxz)
				sizez = maxz - i;
			else
				sizez = (float)tile_z;

			DrawWalls(true, true, false, false, false, false);
			AddFloorMain(wall, name, texture, 0, i, j, i + sizex, j + sizez, altitude, altitude, false, false, 1, 1, false);
			ResetWalls(false);
		}
	}
	Report("Finished ground");
	return wall;
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
	Stairs *stairs = 0;

	if (shaftnumber > 0)
		shaft = GetShaft(shaftnumber);
	if (stairsnumber > 0)
		stairs = GetStairs(stairsnumber);

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
						if (floorobj->IsEnabled == false)
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

						if (floorobj->IsEnabled == true)
						{
							floorobj->Enabled(false);
							if (enablegroups == true)
								floorobj->EnableGroup(false);
						}
					}
				}
			}
			else if (stairs)
			{
				//if a stairwell is specified, only show the floor if it is in the related stairwell's ShowFloorsList array
				if (stairs->ShowFloors == true)
				{
					bool showfloor = stairs->IsShowFloor(i);

					if (showfloor == true && value == true)
					{
						if (floorobj->IsEnabled == false)
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
							if (stairs->IsShowFloor(floorobj->EnabledGroup_Floor) == true)
								return;
						}

						if (floorobj->IsEnabled == true)
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
	timercallbacks.push_back(timer);

	return true;
}

bool SBS::UnregisterTimerCallback(TimerObject *timer)
{
	if (!timer)
		return false;

	for (size_t i = 0; i < timercallbacks.size(); i++)
	{
		//unregister existing call button callback
		if (timercallbacks[i] == timer)
		{
			timercallbacks.erase(timercallbacks.begin() + i);
			return true;
		}
	}

	return false;
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

bool SBS::Mount(const std::string &filename, const std::string &path)
{
	//mounts a zip file into the virtual filesystem

	std::string newfile = "data/" + filename;
	std::string file = VerifyFile(newfile);

	Report("Mounting " + file + " as path " + path);
	try
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(file, "Zip", path, true);
	}
	catch (Ogre::Exception &e)
	{
		return ReportError("Error mounting file " + file + "\n" + e.getDescription());
	}
	return true;
}

void SBS::AddFloorAutoArea(Ogre::Vector3 start, Ogre::Vector3 end)
{
	//adds an auto area that enables/disables floors

	AutoArea newarea;
	newarea.start = start;
	newarea.end = end;
	newarea.inside = false;
	newarea.camerafloor = 0;
	FloorAutoArea.push_back(newarea);
}

void SBS::CheckAutoAreas()
{
	//check all automatic areas

	SBS_PROFILE("SBS::CheckAutoAreas");

	Ogre::Vector3 position = camera->GetPosition();
	int floor = camera->CurrentFloor;

	for (size_t i = 0; i < FloorAutoArea.size(); i++)
	{
		//reset inside value if floor changed
		if (FloorAutoArea[i].camerafloor != floor)
			FloorAutoArea[i].inside = false;

		if (InBox(FloorAutoArea[i].start, FloorAutoArea[i].end, position) == true && FloorAutoArea[i].inside == false)
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
		if (InBox(FloorAutoArea[i].start, FloorAutoArea[i].end, position) == false && FloorAutoArea[i].inside == true)
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

Sound* SBS::AddSound(const std::string &name, const std::string &filename, const Ogre::Vector3 &position, bool loop, float volume, int speed, float min_distance, float max_distance, float doppler_level, float cone_inside_angle, float cone_outside_angle, float cone_outside_volume, const Ogre::Vector3 &direction)
{
	//create a looping sound object
	Sound *sound = new Sound(this, name, false);
	sounds.push_back(sound);

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
				soundlist.push_back(sounds[i]);
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

float SBS::ToLocal(float remote_value)
{
	//convert remote (OGRE) vertex positions to local (SBS) positions

	//note - OGRE uses a right-hand coordinate system, while SBS uses left-hand.
	//this means that all Z values that use this function must be inverted.

	return remote_value * UnitScale;
}

Ogre::Vector2 SBS::ToLocal(const Ogre::Vector2& remote_value)
{
	//convert remote (OGRE) vertex positions to local (SBS) positions

	//note - OGRE uses a right-hand coordinate system, while SBS uses left-hand.
	//this means that all Z values that use this function must be inverted.

	return remote_value * UnitScale;
}

Ogre::Vector3 SBS::ToLocal(const Ogre::Vector3& remote_value, bool rescale, bool flip_z)
{
	//convert remote (OGRE) vertex positions to local (SBS) positions
	//also convert Z value to OGRE's right-hand coordinate system

	Ogre::Vector3 newvalue;
	newvalue.x = remote_value.x;
	newvalue.y = remote_value.y;

	if (flip_z == true)
		newvalue.z = -remote_value.z; //flip z value for OGRE's right-hand coordinate system
	else
		newvalue.z = remote_value.z;

	if (rescale == true)
		return newvalue * UnitScale;
	else
		return newvalue;
}

float SBS::ToRemote(float local_value)
{
	//convert local (SBS) vertex positions to remote (OGRE) positions

	//note - OGRE uses a right-hand coordinate system, while SBS uses left-hand.
	//this means that all Z values that use this function must be inverted.

	return local_value / UnitScale;
}

Ogre::Vector2 SBS::ToRemote(const Ogre::Vector2& local_value)
{
	//convert local (SBS) vertex positions to remote (OGRE) positions

	//note - OGRE uses a right-hand coordinate system, while SBS uses left-hand.
	//this means that all Z values that use this function must be inverted.

	return local_value / UnitScale;
}

Ogre::Vector3 SBS::ToRemote(const Ogre::Vector3& local_value, bool rescale, bool flip_z)
{
	//convert local (SBS) vertex positions to remote (OGRE) positions

	Ogre::Vector3 newvalue;
	newvalue.x = local_value.x;
	newvalue.y = local_value.y;

	if (flip_z == true)
		newvalue.z = -local_value.z; //flip z value for OGRE's right-hand coordinate system
	else
		newvalue.z = local_value.z;

	if (rescale == true)
		return (newvalue / UnitScale);
	else
		return newvalue;
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
	ObjectArray.push_back(object);
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
				objects.push_back(ObjectArray[number]);
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
		for (int i = 0; i < stairs_manager->GetCount(); i++)
		{
			Stairs *stairs = stairs_manager->GetIndex(i);
			if (stairs)
			{
				if (stairs->IsValidFloor(floor->Number) == true)
					return ReportError("Cannot delete floor " + ToString(floor->Number) + " - in use by stairwell " + ToString(stairs->StairsNum));
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
	else if (type == "Stairs")
		deleted = true;
	else if (type == "Wall")
	{
		Wall *obj = static_cast<Wall*>(object);
		obj->DeletePolygons(true);
		deleted = true;
	}
	else if (type == "Model")
		deleted = true;
	else if (type == "Control")
		deleted = true;
	else if (type == "Trigger")
		deleted = true;
	else if (type == "DoorWrapper")
	{
		ElevatorDoor::DoorWrapper* wrapper = static_cast<ElevatorDoor::DoorWrapper*>(object);
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

bool SBS::MoveObject(Object *object, Ogre::Vector3 position, bool relative, bool X, bool Y, bool Z)
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

bool SBS::RotateObject(Object *object, Ogre::Vector3 rotation, float speed, bool relative, bool X, bool Y, bool Z)
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

void SBS::RemoveStairs(Stairs *stairs)
{
	//remove a stairs object (does not delete the object)

	stairs_manager->Remove(stairs);
}

void SBS::RemoveSound(Sound *sound)
{
	//remove a sound from the array
	//this does not delete the object

	for (size_t i = 0; i < sounds.size(); i++)
	{
		if (sounds[i] == sound)
		{
			sounds.erase(sounds.begin() + i);
			return;
		}
	}
}

void SBS::RemoveLight(Light *light)
{
	//remove a light reference (does not delete the object itself)
	for (size_t i = 0; i < lights.size(); i++)
	{
		if (lights[i] == light)
		{
			lights.erase(lights.begin() + i);
			return;
		}
	}
}

void SBS::RemoveModel(Model *model)
{
	//remove a model reference (does not delete the object itself)
	for (size_t i = 0; i < ModelArray.size(); i++)
	{
		if (ModelArray[i] == model)
		{
			ModelArray.erase(ModelArray.begin() + i);
			return;
		}
	}
}

void SBS::RemoveControl(Control *control)
{
	//remove a control reference (does not delete the object itself)
	for (size_t i = 0; i < ControlArray.size(); i++)
	{
		if (ControlArray[i] == control)
		{
			ControlArray.erase(ControlArray.begin() + i);
			return;
		}
	}
}

void SBS::RemoveTrigger(Trigger *trigger)
{
	//remove a trigger reference (does not delete the object itself)
	for (size_t i = 0; i < TriggerArray.size(); i++)
	{
		if (TriggerArray[i] == trigger)
		{
			TriggerArray.erase(TriggerArray.begin() + i);
			return;
		}
	}
}

std::string SBS::VerifyFile(const std::string &filename)
{
	bool result = false;
	return VerifyFile(filename, result, false);
}

std::string SBS::VerifyFile(std::string filename, bool &result, bool skip_cache)
{
	//verify a filename
	//if it exists, return the same filename
	//otherwise search the related folder and find a matching filename with a different
	//case (fixes case-sensitivity issues mainly on Linux)
	//returns the original string if not found
	//"result" will return if the file exists or not, but only accurately if skip_cache is true

	TrimString(filename);
	ReplaceAll(filename, "\\", "/");
	result = false;

	//check for a cached result
	if (skip_cache == false)
	{
		for (size_t i = 0; i < verify_results.size(); i++)
		{
			if (verify_results[i].filename == filename)
				return verify_results[i].result;
		}
	}

#if OGRE_VERSION >= 0x00010900
	Ogre::FileSystemArchive filesystem(".", "FileSystem", false);
#else
	Ogre::FileSystemArchive filesystem(".", "FileSystem");
#endif

	//check for a mount point
	Ogre::StringVectorPtr listing;
	std::string shortname;
	std::string group = GetMountPath(filename, shortname);

	if (group == "General")
	{
		//for the General group, check the native filesystem

		if (filesystem.exists(filename) == true)
		{
			//if exact filename exists, cache and exit
			CacheFilename(filename, filename);
			result = true;
			return filename;
		}

		//otherwise get listing of files to check
		if (filesystem_listing.isNull())
			filesystem_listing = filesystem.list();
		listing = filesystem_listing;
	}
	else
	{
		//for other groups, check resource mount points

		if (Ogre::ResourceGroupManager::getSingleton().resourceExists(group, shortname) == true)
		{
			//if exact filename exists, cache and exit
			CacheFilename(filename, filename);
			result = true;
			return filename;
		}

		//otherwise get listing of files to check
		listing = Ogre::ResourceGroupManager::getSingleton().listResourceNames(group);
	}

	//go through file listing, to find a match with a different case
	for (size_t i = 0; i < listing->size(); i++)
	{
		std::string check = listing->at(i);
		std::string checkoriginal = SetCaseCopy(check, false);
		std::string checkfile = SetCaseCopy(filename, false);
		if (checkoriginal == checkfile)
		{
			//if match is found, cache and exit
			CacheFilename(filename, check);
			result = true;
			return check;
		}
	}

	//if no match is found, cache original name and exit
	CacheFilename(filename, filename);
	return filename;
}

bool SBS::FileExists(const std::string &filename)
{
	//check to see if the specified file exists

	bool result;
	VerifyFile(filename, result, true);

	return result;
}

int SBS::GetWallCount()
{
	//return total number of registered walls
	return WallCount;
}

int SBS::GetPolygonCount()
{
	//return total number of registered walls
	return PolygonCount;
}

void SBS::Prepare(bool report)
{
	//prepare objects for run

	if (report == true)
	{
		Report("Preparing objects...");
		Report("Processing geometry...");
	}

	//prepare mesh objects
	for (size_t i = 0; i < meshes.size(); i++)
	{
		meshes[i]->Prepare();
	}

	//process dynamic meshes
	for (size_t i = 0; i < dynamic_meshes.size(); i++)
	{
		dynamic_meshes[i]->Prepare();
	}

	if (report == true)
		Report("Creating colliders...");

	for (size_t i = 0; i < meshes.size(); i++)
	{
		if (meshes[i]->tricollider == true)
			meshes[i]->CreateCollider();
		else
			meshes[i]->CreateBoxCollider();
	}

	if (report == true)
		Report("Finished prepare");
}

Light* SBS::AddLight(const std::string &name, int type, const Ogre::Vector3 &position, const Ogre::Vector3 &direction, float color_r, float color_g, float color_b, float spec_color_r, float spec_color_g, float spec_color_b, float spot_inner_angle, float spot_outer_angle, float spot_falloff, float att_range, float att_constant, float att_linear, float att_quadratic)
{
	//add a global light

	Light* light = new Light(this, name, type, position, direction, color_r, color_g, color_b, spec_color_r, spec_color_g, spec_color_b, spot_inner_angle, spot_outer_angle, spot_falloff, att_range, att_constant, att_linear, att_quadratic);
	lights.push_back(light);
	return light;
}

void SBS::AddMeshHandle(MeshObject* handle)
{
	meshes.push_back(handle);
}

void SBS::DeleteMeshHandle(MeshObject* handle)
{
	for (size_t i = 0; i < meshes.size(); i++)
	{
		if (meshes[i] == handle)
		{
			meshes.erase(meshes.begin() + i);
			return;
		}
	}
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

Model* SBS::AddModel(const std::string &name, const std::string &filename, bool center, const Ogre::Vector3 &position, const Ogre::Vector3 &rotation, float max_render_distance, float scale_multiplier, bool enable_physics, float restitution, float friction, float mass)
{
	//add a model
	Model* model = new Model(this, name, filename, center, position, rotation, max_render_distance, scale_multiplier, enable_physics, restitution, friction, mass);
	if (model->load_error == true)
	{
		delete model;
		return 0;
	}
	ModelArray.push_back(model);
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

	ModelArray.push_back(model);
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

float SBS::GetConfigFloat(const std::string &key, float default_value)
{
	std::string result = configfile->getSetting(key, "", ToString(default_value));
	return ToFloat(result);
}

bool SBS::InBox(const Ogre::Vector3 &start, const Ogre::Vector3 &end, const Ogre::Vector3 &test)
{
	//determine if a point (test) is inside the box defines by start and end vertices

	if (test.x > start.x && test.y > start.y && test.z > start.z && test.x < end.x && test.y < end.y && test.z < end.z)
		return true;
	return false;
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
	frame_times.push_back(current_time);
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

std::string SBS::GetMountPath(std::string filename, std::string &newfilename)
{
	//get mountpoint (resource group) path of given file
	//if not found, return "General"

	Ogre::StringVector list = Ogre::ResourceGroupManager::getSingleton().getResourceGroups();
	ReplaceAll(filename, "\\", "/");
	newfilename = filename;

	for (size_t i = 0; i < list.size(); i++)
	{
		if (StartsWith(filename, list[i] + "/") == true)
		{
			newfilename = filename.substr(list[i].size() + 1);
			return list[i];
		}
	}
	return "General";
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

void SBS::CacheFilename(const std::string &filename, const std::string &result)
{
	//caches filename information for VerifyFile function
	VerifyResult verify;
	verify.filename = filename;
	verify.result = result;
	verify_results.push_back(verify);
}

void SBS::SetLighting(float red, float green, float blue)
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

Control* SBS::AddControl(const std::string &name, const std::string &sound, const std::string &direction, float CenterX, float CenterZ, float width, float height, float voffset, int selection_position, std::vector<std::string> &action_names, std::vector<std::string> &textures)
{
	//add a control
	std::vector<Action*> actionnull; //not used
	Control* control = new Control(this, name, false, sound, action_names, actionnull, textures, direction, width, height, true, selection_position);
	control->SetPosition(CenterX, voffset, CenterZ);
	ControlArray.push_back(control);
	return control;
}

Trigger* SBS::AddTrigger(const std::string &name, const std::string &sound_file, const Ogre::Vector3 &area_min, const Ogre::Vector3 &area_max, std::vector<std::string> &action_names)
{
	//add a trigger
	Trigger* trigger = new Trigger(this, name, false, sound_file, area_min, area_max, action_names);
	TriggerArray.push_back(trigger);
	return trigger;
}

Action* SBS::AddAction(const std::string &name, std::vector<Object*> &action_parents, const std::string &command, const std::vector<std::string> &parameters)
{
	//add a global action

	Action *action = new Action(this, name, action_parents, command, parameters);
	ActionArray.push_back(action);
	return action;
}

Action* SBS::AddAction(const std::string &name, std::vector<Object*> &action_parents, const std::string &command)
{
	//add a global action

	Action *action = new Action(this, name, action_parents, command);
	ActionArray.push_back(action);
	return action;
}

std::vector<Action*> SBS::GetAction(std::string name)
{
	//get action by name
	ReplaceAll(name, " ", "");
	std::vector<Action*> actionlist;
	for (size_t i = 0; i < ActionArray.size(); i++)
	{
		std::string actionname = ActionArray[i]->GetName();
		ReplaceAll(actionname, " ", "");
		if (actionname == name)
			actionlist.push_back(ActionArray[i]);
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

Object* SBS::GetObject(std::string name)
{
	//get object by name

	ReplaceAll(name, " ", "");

	for (size_t i = 0; i < ObjectArray.size(); i++)
	{
		if (ObjectArray[i])
		{
			std::string tmpname = ObjectArray[i]->GetName();
			ReplaceAll(tmpname, " ", "");
			if (tmpname == name)
				return ObjectArray[i];
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
							objects.push_back(ObjectArray[i]);
					}
					if (type == "elevator")
					{
						if (tmpname == "Elevator " + number)
							objects.push_back(ObjectArray[i]);
					}
					if (type == "shaft")
					{
						if (tmpname == "Shaft " + number)
							objects.push_back(ObjectArray[i]);
					}
					if (type == "stairwell")
					{
						if (tmpname == "Stairwell " + number)
							objects.push_back(ObjectArray[i]);
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
			objects.push_back(obj);
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
	keys.push_back(key);

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
	control_index.push_back(control);
}

void SBS::UnregisterControl(Control *control)
{
	//remove control from index

	for (size_t i = 0; i < control_index.size(); i++)
	{
		if (control_index[i] == control)
		{
			control_index.erase(control_index.begin() + i);
			return;
		}
	}
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
		for (size_t j = 0; j < dynamic_meshes[i]->GetMeshCount(); j++)
		{
			if (camera->IsDynamicMeshVisible(dynamic_meshes[i], (int)j) == true)
			{
				submeshes = dynamic_meshes[i]->GetSubMeshCount((int)j);
				Report(dynamic_meshes[i]->GetMeshName((int)j) + "\t-\t" + ToString(submeshes));
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
	//return total number of allocated sounds
	return EscalatorCount;
}

void SBS::IncrementEscalatorCount()
{
	EscalatorCount++;
}

void SBS::DecrementEscalatorCount()
{
	EscalatorCount--;
}

int SBS::GetMovingWalkwayCount()
{
	//return total number of allocated sounds
	return MovingWalkwayCount;
}

void SBS::IncrementMovingWalkwayCount()
{
	MovingWalkwayCount++;
}

void SBS::DecrementMovingWalkwayCount()
{
	MovingWalkwayCount--;
}

bool SBS::HitBeam(const Ogre::Ray &ray, float max_distance, MeshObject *&mesh, Wall *&wall, Ogre::Vector3 &hit_position)
{
	//use a given ray and distance, and return the nearest hit mesh and if applicable, wall object
	//note that the ray's origin and direction need to be in engine-relative values

	//create a new ray that has absolute positioning, for engine offsets
	Ogre::Ray ray2 (ToRemote(ToGlobal(ToLocal(ray.getOrigin()))), GetOrientation() * ray.getDirection());

	//get a collision callback from Bullet
	OgreBulletCollisions::CollisionClosestRayResultCallback callback (ray2, mWorld, max_distance);

	//check for collision
	mWorld->launchRay(callback);

	//exit if no collision
	if (callback.doesCollide() == false)
		return false;

	//get collided collision object
	OgreBulletCollisions::Object* object = callback.getCollidedObject();

	if (!object)
		return false;

	//get name of collision object's grandparent scenenode (which is the same name as the mesh object)
	std::string meshname = object->getRootNode()->getParentSceneNode()->getName();

	//get hit/intersection position
	hit_position = ToLocal(callback.getCollisionPoint());

	//get associated mesh object
	mesh = FindMeshObject(meshname);
	if (!mesh)
		return false;

	//get wall object, if any
	Ogre::Vector3 isect;
	float distance = 2000000000.;
	Ogre::Vector3 normal = Ogre::Vector3::ZERO;
	wall = mesh->FindWallIntersect(ray.getOrigin(), ray.getPoint(max_distance), isect, distance, normal);

	return true;
}

void SBS::EnableRandomActivity(bool value)
{
	//enable random activity, by creating random people

	if (value == true)
	{
		//create regular people
		for (int i = 0; i < GetTotalFloors(); i++)
		{
			Person *person = CreatePerson("Random " + ToString(i + 1), 0, false);

			//enable random activity on the person
			person->EnableRandomActivity(true);
		}

		//create a service person
		int i = GetTotalFloors();
		Person *person = CreatePerson("Random " + ToString(i + 1), 0, true);

		//enable random activity on the person
		person->EnableRandomActivity(true);
	}
	else if (value == false)
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

bool SBS::IsObjectValid(Object *object, std::string type)
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
	else if (type == "Stairs")
	{
		for (int i = 0; i < stairs_manager->GetCount(); i++)
		{
			if (stairs_manager->GetIndex(i) == static_cast<Stairs*>(object))
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
	PersonArray.push_back(person);
	return person;
}

void SBS::RemovePerson(Person *person)
{
	//remove a person (does not delete the object)

	for (size_t i = 0; i < PersonArray.size(); i++)
	{
		if (PersonArray[i] == person)
		{
			PersonArray.erase(PersonArray.begin() + i);
			return;
		}
	}
}

bool SBS::AttachCamera(Ogre::Camera *camera, bool init_state)
{
	if (camera)
		return this->camera->Attach(camera, init_state);
	return false;
}

bool SBS::DetachCamera()
{
	return camera->Detach();
}

std::string SBS::ProcessFullName(std::string name, int &instance, int &object_number, bool strip_number)
{
	//if given a full object ID name (such as "0:(4)Landscape"),
	//return base name and parse out instance number and object number

	//if strip_number is false, leave object number identifier in string

	//get and strip off engine instance number
	size_t index = name.find(":(");
	instance = ToInt(name.substr(0, index));
	name.erase(name.begin(), name.begin() + index + 1);

	//get and optionally strip off object number
	index = name.find(")");
	object_number = ToInt(name.substr(1, index - 1));

	if (strip_number == true)
		name.erase(name.begin(), name.begin() + index + 1);

	return name;
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

bool SBS::IsInside(const Ogre::Vector3 &position)
{
	//return true if the specified position is inside the sim engine's area

	if (area_trigger)
		return area_trigger->IsInside(position);

	//if no trigger is defined, position is always inside the area
	return true;
}

bool SBS::GetBounds(Ogre::Vector3 &min, Ogre::Vector3 &max)
{
	if (area_trigger)
	{
		min = area_trigger->GetMin();
		max = area_trigger->GetMax();
		return true;
	}

	min = Ogre::Vector3::ZERO;
	max = Ogre::Vector3::ZERO;
	return false;
}

void SBS::CutOutsideBoundaries(bool landscape, bool buildings, bool external, bool floors)
{
	//cut landscape and buildings for engine bounds if needed
	//run this function before calling Start()

	if (!area_trigger)
		return;

	Report("Cutting outside boundaries...");
	Ogre::Vector3 min = area_trigger->GetMin();
	Ogre::Vector3 max = area_trigger->GetMax();

	if (landscape == true)
		Landscape->CutOutsideBounds(min, max, true, true);
	if (buildings == true)
		Buildings->CutOutsideBounds(min, max, true, true);
	if (external == true)
		External->CutOutsideBounds(min, max, true, true);

	if (floors == true)
	{
		for (int i = 0; i < floor_manager->GetCount(); i++)
			floor_manager->GetIndex(i)->Level->CutOutsideBounds(min, max, true, true);
	}
}

void SBS::CutInsideBoundaries(const Ogre::Vector3 &min, const Ogre::Vector3 &max, bool landscape, bool buildings, bool external, bool floors)
{
	//cut landscape and buildings for specified bounds
	//run this function before calling Start()

	if (landscape == true)
		Landscape->Cut(min, max, true, true);
	if (buildings == true)
		Buildings->Cut(min, max, true, true);
	if (external == true)
		External->Cut(min, max, true, true);

	if (floors == true)
	{
		for (int i = 0; i < floor_manager->GetCount(); i++)
			floor_manager->GetIndex(i)->Level->Cut(min, max, true, true);
	}
}

void SBS::SetBounds(const Ogre::Vector3 &area_min, const Ogre::Vector3 &area_max)
{
	//don't set bounds if the primary engine
	if (InstanceNumber == 0)
		return;

	if (area_min != Ogre::Vector3::ZERO && area_max != Ogre::Vector3::ZERO && !area_trigger)
	{
		std::vector<std::string> names;
		names.push_back("Off");
		area_trigger = new Trigger(this, "System Boundary", true, "", area_min, area_max, names);
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
	floor_manager->EnableAll(false);
	shaft_manager->EnableAll(false);
	stairs_manager->EnableAll(false);
	elevator_manager->EnableAll(false);

	//reset camera state
	camera->ResetState();
}

Ogre::Vector3 SBS::ToGlobal(const Ogre::Vector3 &position)
{
	//convert an engine-relative position to a global (scene) position

	return (GetOrientation().Inverse() * position) + GetPosition();
}

Ogre::Vector3 SBS::FromGlobal(const Ogre::Vector3 &position)
{
	//convert a global (scene) position to an engine-relative position

	return (GetOrientation() * (position - GetPosition()));
}

Ogre::Quaternion SBS::ToGlobal(const Ogre::Quaternion &orientation)
{
	//convert an engine-relative orientation (rotation) to a global (scene) orientation

	return (GetOrientation() * orientation);
}

Ogre::Quaternion SBS::FromGlobal(const Ogre::Quaternion &orientation)
{
	//convert a global (scene) orientation (rotation) to an engine-relative orientation

	return (GetOrientation().Inverse() * orientation);
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

void SBS::RegisterDynamicMesh(DynamicMesh *dynmesh)
{
	//register a dynamic mesh with the system

	dynamic_meshes.push_back(dynmesh);
}

void SBS::UnregisterDynamicMesh(DynamicMesh *dynmesh)
{
	//unregister a dynamic mesh from the system

	for (size_t i = 0; i < dynamic_meshes.size(); i++)
	{
		if (dynamic_meshes[i] == dynmesh)
		{
			dynamic_meshes.erase(dynamic_meshes.begin() + i);
			return;
		}
	}
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

StairsManager* SBS::GetStairsManager()
{
	return stairs_manager;
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

}
