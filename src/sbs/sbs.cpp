/* $Id$ */

/*
	Scalable Building Simulator - Core
	The Skyscraper Project - Version 1.9 Alpha
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

#include <OgreRoot.h>
#include <OgreFileSystem.h>
#include <OgreFontManager.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreStringConverter.h>
#include <fmod.hpp>
#include <OgreBulletDynamicsRigidBody.h>
#include "globals.h"
#include "sbs.h"
#include "unix.h"
#include "revsbs.h"
#include "model.h"

SBS *sbs; //self reference

SBS::SBS(Ogre::RenderWindow* mRenderWindow, Ogre::SceneManager* mSceneManager, Ogre::Camera *camera, const char* rootdirectory, const char* directory_char, FMOD::System *fmodsystem)
{
	sbs = this;
	version = "0.9.1." + std::string(SVN_REVSTR);
	version_state = "Alpha";

	//set up SBS object
	object = new Object();
	object->SetValues(this, 0, "SBS", "SBS", true);

	//Print SBS banner
	PrintBanner();

	//Pause for 2 seconds
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	Sleep(2000);
#else
	sleep(2);
#endif

	mRoot = Ogre::Root::getSingletonPtr();
	this->mSceneManager = mSceneManager;

	root_dir = rootdirectory;
	dir_char = directory_char;

	//load config file
	configfile.load("skyscraper.ini");

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
	mouse_x = 0;
	mouse_y = 0;
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
	AutoX = true;
	AutoY = true;
	TextureOverride = false;
	ProcessElevators = GetConfigBool("Skyscraper.SBS.ProcessElevators", true);
	FlipTexture = false;
	mainnegflip = 0;
	mainposflip = 0;
	sidenegflip = 0;
	sideposflip = 0;
	topflip = 0;
	bottomflip = 0;
	widthscale.resize(6);
	heightscale.resize(6);
	remaining_delta = 0;
	start_time = 0;
	running_time = 0;
	InShaft = false;
	DisableSound = false;
	MapIndex.resize(3);
	MapUV.resize(3);
	OldMapIndex.resize(3);
	OldMapUV.resize(3);
	MapVerts1.resize(3);
	MapVerts2.resize(3);
	MapVerts3.resize(3);
	OldMapVerts1.resize(3);
	OldMapVerts2.resize(3);
	OldMapVerts3.resize(3);
	MapMethod = 0;
	OldMapMethod = 0;
	RevX = false;
	RevY = false;
	RevZ = false;
	OldRevX = false;
	OldRevY = false;
	OldRevZ = false;
	PlanarFlat = false;
	OldPlanarFlat = false;
	PlanarRotate = false;
	OldPlanarRotate = false;
	for (int i = 0; i <= 2; i++)
	{
		MapIndex[i] = 0;
		OldMapIndex[i] = 0;
		OldMapUV[i] = 0;
		MapUV[i] = 0;
	}
	DeleteColliders = false;
	soundcount = 0;
	UnitScale = GetConfigFloat("Skyscraper.SBS.UnitScale", 5);
	Verbose = GetConfigBool("Skyscraper.SBS.Verbose", false);
	InterfloorOnTop = false;
	DefaultMapper = GetConfigInt("Skyscraper.SBS.TextureMapper", 0);
	ObjectCount = 0;
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
	listener_position.x = 0;
	listener_position.y = 0;
	listener_position.z = 0;
	listener_velocity.x = 0;
	listener_velocity.y = 0;
	listener_velocity.z = 0;
	listener_forward.x = 0;
	listener_forward.y = 0;
	listener_forward.z = 0;
	listener_up.x = 0;
	listener_up.y = 0;
	listener_up.z = 0;
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
	getfloor_result = 0;
	getfloor_number = 0;
	getelevator_result = 0;
	getelevator_number = 0;
	getshaft_result = 0;
	getshaft_number = 0;
	getstairs_result = 0;
	getstairs_number = 0;
	texturecount = 0;
	materialcount = 0;
	SkyName = GetConfigString("Skyscraper.SBS.SkyName", "noon");
	ShaftDisplayRange = GetConfigInt("Skyscraper.SBS.ShaftDisplayRange", 3);
	StairsDisplayRange = GetConfigInt("Skyscraper.SBS.StairsDisplayRange", 5);
	ShaftOutsideDisplayRange = GetConfigInt("Skyscraper.SBS.ShaftOutsideDisplayRange", 3);
	StairsOutsideDisplayRange = GetConfigInt("Skyscraper.SBS.StairsOutsideDisplayRange", 3);
	FloorDisplayRange = GetConfigInt("Skyscraper.SBS.FloorDisplayRange", 3);
	SmoothFrames = GetConfigInt("Skyscraper.SBS.SmoothFrames", 200);
	RenderOnStartup = GetConfigBool("Skyscraper.SBS.RenderOnStartup", false);

	if (UnitScale <= 0)
		UnitScale = 1;

	//set default texture map values
	ResetTextureMapping(true);

	//disable sound if system is not available
	if (!fmodsystem)
		DisableSound = true;
	soundsys = fmodsystem;

	//set up sound options (mainly to set sound distance factor to feet instead of meters)
	if (DisableSound == false)
		soundsys->set3DSettings(1.0f, 3.28f, 1.0f);

	//set up physics
	mWorld = new OgreBulletDynamics::DynamicsWorld(mSceneManager, Ogre::AxisAlignedBox(Ogre::Vector3(-10000, -10000, -10000), Ogre::Vector3(10000, 10000, 10000)), Ogre::Vector3(0, 0, 0), true);
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

	//load default textures
	Report("Loading default textures...");
	SetLighting();
	LoadTexture("data/default.png", "Default", 1, 1);
	LoadTexture("data/gray2-sm.jpg", "ConnectionWall", 1, 1);
	LoadTexture("data/metal1-sm.jpg", "Connection", 1, 1);
	ResetLighting();
	Report("Done");

	//create object meshes
	Buildings = new MeshObject(object, "Buildings");
	External = new MeshObject(object, "External");
	Landscape = new MeshObject(object, "Landscape");
	//Landscape->tricollider = false;

	//create camera object
	this->camera = new Camera(camera);
}

SBS::~SBS()
{
	//engine destructor

	Report("Deleting simulator objects...");

	FastDelete = true;

	//delete controls
	for (int i = 0; i < (int)ControlArray.size(); i++)
	{
		if (ControlArray[i])
		{
			ControlArray[i]->object->parent_deleting = true;
			delete ControlArray[i];
		}
		ControlArray[i] = 0;
	}

	//delete triggers
	for (int i = 0; i < (int)TriggerArray.size(); i++)
	{
		if (TriggerArray[i])
		{
			TriggerArray[i]->object->parent_deleting = true;
			delete TriggerArray[i];
		}
		TriggerArray[i] = 0;
	}

	//delete models
	for (int i = 0; i < (int)ModelArray.size(); i++)
	{
		if (ModelArray[i])
		{
			ModelArray[i]->object->parent_deleting = true;
			delete ModelArray[i];
		}
		ModelArray[i] = 0;
	}

	//delete lights
	for (int i = 0; i < (int)lights.size(); i++)
	{
		if (lights[i])
		{
			lights[i]->object->parent_deleting = true;
			delete lights[i];
		}
		lights[i] = 0;
	}

	//delete camera object
	if (camera)
		delete camera;
	camera = 0;

	//delete callbacks
	doorcallbacks.clear();
	buttoncallbacks.clear();

	//delete floors
	for (int i = 0; i < (int)FloorArray.size(); i++)
	{
		if (FloorArray[i].object)
		{
			FloorArray[i].object->object->parent_deleting = true;
			delete FloorArray[i].object;
		}
		FloorArray[i].object = 0;
	}
	FloorArray.clear();

	//delete elevators
	for (int i = 0; i < (int)ElevatorArray.size(); i++)
	{
		if (ElevatorArray[i].object)
		{
			ElevatorArray[i].object->object->parent_deleting = true;
			delete ElevatorArray[i].object;
		}
		ElevatorArray[i].object = 0;
	}
	ElevatorArray.clear();

	//delete shafts
	for (int i = 0; i < (int)ShaftArray.size(); i++)
	{
		if (ShaftArray[i].object)
		{
			ShaftArray[i].object->object->parent_deleting = true;
			delete ShaftArray[i].object;
		}
		ShaftArray[i].object = 0;
	}
	ShaftArray.clear();

	//delete stairs
	for (int i = 0; i < (int)StairsArray.size(); i++)
	{
		if (StairsArray[i].object)
		{
			StairsArray[i].object->object->parent_deleting = true;
			delete StairsArray[i].object;
		}
		StairsArray[i].object = 0;
	}
	StairsArray.clear();

	//delete sounds
	for (int i = 0; i < (int)sounds.size(); i++)
	{
		if (sounds[i])
		{
			sounds[i]->object->parent_deleting = true;
			delete sounds[i];
		}
		sounds[i] = 0;
	}
	sounds.clear();

	//delete actions
	for (int i = 0; i < (int)ActionArray.size(); i++)
	{
		if (ActionArray[i])
			delete ActionArray[i];
		ActionArray[i] = 0;
	}
	ActionArray.clear();

	//delete wall objects
	if (SkyBox)
		delete SkyBox;
	SkyBox = 0;

	if (Landscape)
		delete Landscape;
	Landscape = 0;

	if (External)
		delete External;
	External = 0;

	if (Buildings)
		delete Buildings;
	Buildings = 0;

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

	//remove all meshes
	Ogre::MeshManager::getSingleton().removeAll();

	//remove all materials
	Ogre::MaterialManager::getSingleton().removeAll();
	Ogre::MaterialManager::getSingleton().initialise();  //restore default materials
	materialcount = 0;

	//remove all fonts
	Ogre::FontManager::getSingleton().removeAll();

	//remove all textures
	Ogre::TextureManager::getSingleton().removeAll();
	texturecount = 0;

	if (object)
		delete object;
	object = 0;

	//clear self reference
	sbs = 0;

	Report("Exiting");
}

bool SBS::Start()
{
	//Post-init startup code goes here, before the runloop

	//free text texture memory
	for (int i = 0; i < (int)textureboxes.size(); i++)
		free(textureboxes[i].buffer);
	textureboxes.clear();

	//turn on main objects
	EnableBuildings(true);
	EnableLandscape(true);
	EnableExternal(true);
	EnableSkybox(true);
	
	//turn off floors
	for (int i = 0; i < TotalFloors(); i++)
		FloorArray[i].object->Enabled(false);

	//init shafts
	for (int i = 0; i < Shafts(); i++)
	{
		if (ShaftArray[i].object)
			ShaftArray[i].object->Init();
	}

	//init stairwells
	for (int i = 0; i < StairsNum(); i++)
	{
		if (StairsArray[i].object)
			StairsArray[i].object->Init();
	}

	//init elevators
	for (int i = 0; i < Elevators(); i++)
	{
		if (ElevatorArray[i].object)
			ElevatorArray[i].object->Init();
	}

	//play looping global sounds
	for (int i = 0; i < (int)sounds.size(); i++)
	{
		if (sounds[i])
		{
			if (sounds[i]->GetLoopState() == true)
				sounds[i]->Play();
		}
	}

	//move camera to start location
	camera->SetToStartPosition(false); //also turns on start floor
	camera->SetToStartDirection();
	camera->SetToStartRotation();

	IsRunning = true;

	return true;
}

void SBS::PrintBanner()
{
	Report("\n Scalable Building Simulator " + version + " " + version_state);
	Report(" Copyright (C)2004-2015 Ryan Thoryk");
	Report(" This software comes with ABSOLUTELY NO WARRANTY. This is free");
	Report(" software, and you are welcome to redistribute it under certain");
	Report(" conditions. For details, see the file gpl.txt\n");
}

void SBS::MainLoop()
{
	//Main simulator loop
	SBS_PROFILE("SBS::MainLoop");

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

	//update physics
	int steps = 0;
	if (camera->EnableBullet == true)
	{
		if (enable_advanced_profiling == false)
			SBSProfileManager::Start_Profile("Collisions/Physics");
		else
			SBSProfileManager::Start_Profile("Bullet");
		steps = mWorld->stepSimulation(elapsed, 1);
		SBSProfileManager::Stop_Profile();
	}

	//only move character if Bullet processed a step (within it's 60fps timestep)
	if (camera->EnableBullet == true)
	{
		if (steps >= 1)
			camera->MoveCharacter();
	}
	else
		camera->MoveCharacter();

	//sync camera to physics
	camera->Sync();

	//update sound
	if (enable_advanced_profiling == false)
		SBSProfileManager::Start_Profile("Sound");
	else
		SBSProfileManager::Start_Profile("FMOD");
	soundsys->update();
	SBSProfileManager::Stop_Profile();

	elapsed = remaining_delta + ((float)timing / 1000.0f);

	//limit the elapsed value to prevent major slowdowns during debugging
	if (elapsed > .5f)
		elapsed = .5f;

	SBSProfileManager::Start_Profile("Simulator Loop");
	while (elapsed >= delta)
	{
		//Determine floor that the camera is on
		camera->UpdateCameraFloor();

		//run elevator handlers
		if (ProcessElevators == true)
		{
			for (int i = 1; i <= Elevators(); i++)
			{
				if (GetElevator(i))
					GetElevator(i)->MonitorLoop();
			}

			//check if the user is in an elevator
			camera->CheckElevator();
		}

		//check if the user is in a shaft
		camera->CheckShaft();

		//check if the user is in a stairwell
		camera->CheckStairwell();

		//open/close doors by using door callback
		ProcessDoors();

		//process call button callbacks
		ProcessCallButtons();

		//process misc operations on current floor
		if (GetFloor(camera->CurrentFloor))
			GetFloor(camera->CurrentFloor)->Loop();

		//process auto areas
		CheckAutoAreas();

		//process triggers
		for (int i = 0; i < (int)TriggerArray.size(); i++)
		{
			if (TriggerArray[i])
				TriggerArray[i]->Check();
		}

		elapsed -= delta;
	}
	remaining_delta = elapsed;

	//process timers
	ProcessTimers();

	SBSProfileManager::Stop_Profile();
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

bool SBS::AddWallMain(Object *parent, MeshObject* mesh, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float altitude1, float altitude2, float tw, float th, bool autosize)
{
	WallObject *object = new WallObject(mesh, parent, true);
	bool result = AddWallMain(object, name, texture, thickness, x1, z1, x2, z2, height_in1, height_in2, altitude1, altitude2, tw, th, autosize);
	delete object;
	return result;
}

bool SBS::AddWallMain(WallObject* wallobject, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float altitude1, float altitude2, float tw, float th, bool autosize)
{
	//Adds a wall with the specified dimensions

	//exit if coordinates are invalid
	if (x1 == x2 && z1 == z2)
	{
		std::string name2 = name;
		return ReportError("Invalid coordinates for wall '" + name2 + "'");
	}

	//determine axis of wall
	int axis = 0;
	if (fabsf(x1 - x2) > (fabsf(z1 - z2) + 0.00001))
		//x axis
		axis = 1;
	else
		//z axis
		axis = 2;

	//convert to clockwise coordinates if on x-axis, or counterclockwise if on z-axis
	if ((x1 > x2 && axis == 1) || (z1 < z2 && axis == 2))
	{
		//reverse coordinates
		float temp = x1;
		x1 = x2;
		x2 = temp;
		temp = z1;
		z1 = z2;
		z2 = temp;
		temp = altitude1;
		altitude1 = altitude2;
		altitude2 = temp;
		temp = height_in1;
		height_in1 = height_in2;
		height_in2 = temp;
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

	if (FlipTexture == true)
		ProcessTextureFlip(tw, th);

	if (DrawMainN == true)
	{
		if (FlipTexture == true)
		{
			tw2 = widthscale[0];
			th2 = heightscale[0];
		}
		if (TextureOverride == true)
			texture2 = mainnegtex;

		NewName = name;
		if (GetDrawWallsCount() > 1)
			NewName.append(":front");
		wallobject->AddQuad(NewName.c_str(), texture2.c_str(), v1, v2, v3, v4, tw2, th2, autosize); //front wall
	}

	if (DrawMainP == true)
	{
		if (FlipTexture == true)
		{
			tw2 = widthscale[1];
			th2 = heightscale[1];
		}
		if (TextureOverride == true)
			texture2 = mainpostex;

		NewName = name;
		NewName.append(":back");
		wallobject->AddQuad(NewName.c_str(), texture2.c_str(), v6, v5, v8, v7, tw2, th2, autosize); //back wall
	}

	if (DrawSideN == true && thickness != 0.0f)
	{
		if (FlipTexture == true)
		{
			tw2 = widthscale[2];
			th2 = heightscale[2];
		}
		if (TextureOverride == true)
			texture2 = sidenegtex;

		NewName = name;
		NewName.append(":left");
		if (axis == 1)
			wallobject->AddQuad(NewName.c_str(), texture2.c_str(), v5, v1, v4, v8, tw2, th2, autosize); //left wall
		else
			wallobject->AddQuad(NewName.c_str(), texture2.c_str(), v2, v6, v7, v3, tw2, th2, autosize); //left wall
	}

	if (DrawSideP == true && thickness != 0.0f)
	{
		if (FlipTexture == true)
		{
			tw2 = widthscale[3];
			th2 = heightscale[3];
		}
		if (TextureOverride == true)
			texture2 = sidepostex;

		NewName = name;
		NewName.append(":right");
		if (axis == 1)
			wallobject->AddQuad(NewName.c_str(), texture2.c_str(), v2, v6, v7, v3, tw2, th2, autosize); //right wall
		else
			wallobject->AddQuad(NewName.c_str(), texture2.c_str(), v5, v1, v4, v8, tw2, th2, autosize); //right wall
	}

	if (DrawTop == true && thickness != 0.0f)
	{
		if (FlipTexture == true)
		{
			tw2 = widthscale[4];
			th2 = heightscale[4];
		}
		if (TextureOverride == true)
			texture2 = toptex;

		NewName = name;
		NewName.append(":top");
		wallobject->AddQuad(NewName.c_str(), texture2.c_str(), v5, v6, v2, v1, tw2, th2, autosize); //top wall
	}

	if (DrawBottom == true && thickness != 0.0f)
	{
		if (FlipTexture == true)
		{
			tw2 = widthscale[5];
			th2 = heightscale[5];
		}
		if (TextureOverride == true)
			texture2 = bottomtex;

		NewName = name;
		NewName.append(":bottom");
		wallobject->AddQuad(NewName.c_str(), texture2.c_str(), v4, v3, v7, v8, tw2, th2, autosize); //bottom wall
	}

	return true;
}

bool SBS::AddFloorMain(Object *parent, MeshObject* mesh, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float altitude1, float altitude2, bool reverse_axis, bool texture_direction, float tw, float th, bool autosize, bool legacy_behavior)
{
	WallObject *object = new WallObject(mesh, parent, true);
	bool result = AddFloorMain(object, name, texture, thickness, x1, z1, x2, z2, altitude1, altitude2, reverse_axis, texture_direction, tw, th, autosize, legacy_behavior);
	delete object;
	return result;
}

bool SBS::AddFloorMain(WallObject* wallobject, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float altitude1, float altitude2, bool reverse_axis, bool texture_direction, float tw, float th, bool autosize, bool legacy_behavior)
{
	//Adds a floor with the specified dimensions and vertical offset

	//direction determines the direction of slope (for different altitude values):
	//false - left/right from altitude1 to altitude2, or legacy (broken) "ReverseAxis = false" behavior if legacy_behavior is true
	//true - back/forwards from altitude1 to altitude2, or legacy (broken) "ReverseAxis = true" behavior if legacy_behavior is true

	//exit if coordinates are invalid
	if (x1 == x2 && z1 == z2)
	{
		std::string name2 = name;
		return ReportError("Invalid coordinates for floor '" + name2 + "'");
	}

	//convert to clockwise coordinates
	float temp;

	//determine axis of floor
	int axis = 0;
	if (fabsf(x1 - x2) > (fabsf(z1 - z2) + 0.00001))
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
			temp = x1;
			x1 = x2;
			x2 = temp;

			if (reverse_axis == true)
			{
				temp = altitude1;
				altitude1 = altitude2;
				altitude2 = temp;
			}
		}
		if (z1 > z2)
		{
			temp = z1;
			z1 = z2;
			z2 = temp;

			if (reverse_axis == false)
			{
				temp = altitude1;
				altitude1 = altitude2;
				altitude2 = temp;
			}
		}
	}
	else
	{
		//legacy (broken) behavior, for compatibility with previous versions

		if (x1 > x2 && axis == 1)
		{
			//reverse coordinates if the difference between x coordinates is greater
			temp = x1;
			x1 = x2;
			x2 = temp;
			temp = z1;
			z1 = z2;
			z2 = temp;
			temp = altitude1;
			altitude1 = altitude2;
			altitude2 = temp;
		}
		if (z1 > z2 && axis == 2)
		{
			//reverse coordinates if the difference between z coordinates is greater
			temp = x1;
			x1 = x2;
			x2 = temp;
			temp = z1;
			z1 = z2;
			z2 = temp;
			temp = altitude1;
			altitude1 = altitude2;
			altitude2 = temp;
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

	if (FlipTexture == true)
		ProcessTextureFlip(tw, th);

	//turn on rotation if set
	bool old_planarrotate = PlanarRotate;
	PlanarRotate = texture_direction;

	if (DrawMainN == true)
	{
		if (FlipTexture == true)
		{
			tw2 = widthscale[0];
			th2 = heightscale[0];
		}
		if (TextureOverride == true)
			texture2 = mainnegtex;

		NewName = name;
		if (GetDrawWallsCount() > 1)
			NewName.append(":front");
		wallobject->AddQuad(NewName.c_str(), texture2.c_str(), v1, v2, v3, v4, tw2, th2, autosize); //bottom wall
	}

	if (DrawMainP == true)
	{
		if (FlipTexture == true)
		{
			tw2 = widthscale[1];
			th2 = heightscale[1];
		}
		if (TextureOverride == true)
			texture2 = mainpostex;

		NewName = name;
		NewName.append(":back");
		wallobject->AddQuad(NewName.c_str(), texture2.c_str(), v8, v7, v6, v5, tw2, th2, autosize); //top wall
	}

	if (DrawSideN == true && thickness != 0.0f)
	{
		if (FlipTexture == true)
		{
			tw2 = widthscale[2];
			th2 = heightscale[2];
		}
		if (TextureOverride == true)
			texture2 = sidenegtex;

		NewName = name;
		NewName.append(":left");
		wallobject->AddQuad(NewName.c_str(), texture2.c_str(), v8, v5, v1, v4, tw2, th2, autosize); //left wall
	}

	if (DrawSideP == true && thickness != 0.0f)
	{
		if (FlipTexture == true)
		{
			tw2 = widthscale[3];
			th2 = heightscale[3];
		}
		if (TextureOverride == true)
			texture2 = sidepostex;

		NewName = name;
		NewName.append(":right");
		wallobject->AddQuad(NewName.c_str(), texture2.c_str(), v6, v7, v3, v2, tw2, th2, autosize); //right wall
	}

	if (DrawTop == true && thickness != 0.0f)
	{
		if (FlipTexture == true)
		{
			tw2 = widthscale[4];
			th2 = heightscale[4];
		}
		if (TextureOverride == true)
			texture2 = toptex;

		NewName = name;
		NewName.append(":top");
		wallobject->AddQuad(NewName.c_str(), texture2.c_str(), v5, v6, v2, v1, tw2, th2, autosize); //front wall
	}

	if (DrawBottom == true && thickness != 0.0f)
	{
		if (FlipTexture == true)
		{
			tw2 = widthscale[5];
			th2 = heightscale[5];
		}
		if (TextureOverride == true)
			texture2 = bottomtex;

		NewName = name;
		NewName.append(":bottom");
		wallobject->AddQuad(NewName.c_str(), texture2.c_str(), v7, v8, v4, v3, tw2, th2, autosize); //back wall
	}

	PlanarRotate = old_planarrotate;

	return true;
}

void SBS::Report(std::string message)
{
	Ogre::LogManager::getSingleton().logMessage(message);
	LastNotification = message;
}

bool SBS::ReportError(std::string message)
{
	Ogre::LogManager::getSingleton().logMessage(message, Ogre::LML_CRITICAL);
	LastError = message;
	return false;
}

bool SBS::CreateWallBox(WallObject* wallobject, const char *name, const char *texture, float x1, float x2, float z1, float z2, float height_in, float voffset, float tw, float th, bool inside, bool outside, bool top, bool bottom, bool autosize)
{
	//create 4 walls

	//exit if coordinates are invalid
	if (x1 == x2 && z1 == z2)
	{
		std::string name2 = name;
		return ReportError("Invalid coordinates for wall '" + name2 + "'");
	}

	std::string NewName;

	//swap values if the first is greater than the second
	if (x1 > x2)
	{
		float tmpx = x1;
		x1 = x2;
		x2 = tmpx;
	}
	if (z1 > z2)
	{
		float tmpz = z1;
		z1 = z2;
		z2 = tmpz;
	}

	if (inside == true)
	{
		//generate a box visible from the inside

		NewName = name;
		NewName.append(":inside");

		wallobject->AddQuad( //front
			NewName.c_str(),
			texture,
			Ogre::Vector3(x1, voffset, z1),
			Ogre::Vector3(x2, voffset, z1),
			Ogre::Vector3(x2, voffset + height_in, z1),
			Ogre::Vector3(x1, voffset + height_in, z1), tw, th, autosize);
		wallobject->AddQuad( //right
			NewName.c_str(),
			texture,
			Ogre::Vector3(x2, voffset, z1),
			Ogre::Vector3(x2, voffset, z2),
			Ogre::Vector3(x2, voffset + height_in, z2),
			Ogre::Vector3(x2, voffset + height_in, z1), tw, th, autosize);
		wallobject->AddQuad( //back
			NewName.c_str(),
			texture,
			Ogre::Vector3(x2, voffset, z2),
			Ogre::Vector3(x1, voffset, z2),
			Ogre::Vector3(x1, voffset + height_in, z2),
			Ogre::Vector3(x2, voffset + height_in, z2), tw, th, autosize);
		wallobject->AddQuad( //left
			NewName.c_str(),
			texture,
			Ogre::Vector3(x1, voffset, z2),
			Ogre::Vector3(x1, voffset, z1),
			Ogre::Vector3(x1, voffset + height_in, z1),
			Ogre::Vector3(x1, voffset + height_in, z2), tw, th, autosize);
		if (bottom == true)
		{
			wallobject->AddQuad( //bottom
				NewName.c_str(),
				texture,
				Ogre::Vector3(x1, voffset, z2),
				Ogre::Vector3(x2, voffset, z2),
				Ogre::Vector3(x2, voffset, z1),
				Ogre::Vector3(x1, voffset, z1), tw, th, autosize);
		}
		if (top == true)
		{
			wallobject->AddQuad( //top
				NewName.c_str(),
				texture,
				Ogre::Vector3(x1, voffset + height_in, z1),
				Ogre::Vector3(x2, voffset + height_in, z1),
				Ogre::Vector3(x2, voffset + height_in, z1),
				Ogre::Vector3(x1, voffset + height_in, z2), tw, th, autosize);
		}
	}

	if (outside == true)
	{
		NewName = name;
		NewName.append(":outside");

		wallobject->AddQuad( //front
			NewName.c_str(),
			texture,
			Ogre::Vector3(x1, voffset + height_in, z1),
			Ogre::Vector3(x2, voffset + height_in, z1),
			Ogre::Vector3(x2, voffset, z1),
			Ogre::Vector3(x1, voffset, z1), tw, th, autosize);
		wallobject->AddQuad( //right
			NewName.c_str(),
			texture,
			Ogre::Vector3(x2, voffset + height_in, z1),
			Ogre::Vector3(x2, voffset + height_in, z2),
			Ogre::Vector3(x2, voffset, z2),
			Ogre::Vector3(x2, voffset, z1), tw, th, autosize);
		wallobject->AddQuad( //back
			NewName.c_str(),
			texture,
			Ogre::Vector3(x2, voffset + height_in, z2),
			Ogre::Vector3(x1, voffset + height_in, z2),
			Ogre::Vector3(x1, voffset, z2),
			Ogre::Vector3(x2, voffset, z2), tw, th, autosize);
		wallobject->AddQuad( //left
			NewName.c_str(),
			texture,
			Ogre::Vector3(x1, voffset + height_in, z2),
			Ogre::Vector3(x1, voffset + height_in, z1),
			Ogre::Vector3(x1, voffset, z1),
			Ogre::Vector3(x1, voffset, z2), tw, th, autosize);
		if (bottom == true)
		{
			wallobject->AddQuad( //bottom
				NewName.c_str(),
				texture,
				Ogre::Vector3(x1, voffset, z1),
				Ogre::Vector3(x2, voffset, z1),
				Ogre::Vector3(x2, voffset, z2),
				Ogre::Vector3(x1, voffset, z2), tw, th, autosize);
		}
		if (top == true)
		{
			wallobject->AddQuad( //top
				NewName.c_str(),
				texture,
				Ogre::Vector3(x1, voffset + height_in, z2),
				Ogre::Vector3(x2, voffset + height_in, z2),
				Ogre::Vector3(x2, voffset + height_in, z1),
				Ogre::Vector3(x1, voffset + height_in, z1), tw, th, autosize);
		}
	}
	return true;
}

bool SBS::CreateWallBox2(WallObject* wallobject, const char *name, const char *texture, float CenterX, float CenterZ, float WidthX, float LengthZ, float height_in, float voffset, float tw, float th, bool inside, bool outside, bool top, bool bottom, bool autosize)
{
	//create 4 walls from a central point

	float x1;
	float x2;
	float z1;
	float z2;

	x1 = CenterX - (WidthX / 2);
	x2 = CenterX + (WidthX / 2);
	z1 = CenterZ - (LengthZ / 2);
	z2 = CenterZ + (LengthZ / 2);

	return CreateWallBox(wallobject, name, texture, x1, x2, z1, z2, height_in, voffset, tw, th, inside, outside, top, bottom, autosize);
}

bool SBS::AddCustomWall(WallObject* wallobject, const char *name, const char *texture, std::vector<Ogre::Vector3> &varray, float tw, float th)
{
	//Adds a wall from a specified array of 3D vectors

	std::vector<Ogre::Vector3> varray1 = varray;
	std::vector<Ogre::Vector3> varray2;

	//get number of stored vertices
	int num = (int)varray.size();

	//create a second array with reversed vertices
	varray2.reserve(num);
	for (int i = num - 1; i >= 0; i--)
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

	//get texture sizing info
	float tw2 = tw, th2 = th;
	float mw, mh;
	if (GetTextureTiling(texture, mw, mh))
	{
		//multiply the tiling parameters (tw and th) by
		//the stored multipliers for that texture
		tw2 = tw * mw;
		th2 = th * mh;
	}

	//add the polygons
	if (DrawMainN == true)
	{
		std::string NewName;
		NewName = name;
		NewName.append(":0");
		wallobject->AddPolygon(NewName.c_str(), texture, varray1, tw2, th2, true);
	}
	if (DrawMainP == true)
	{
		std::string NewName;
		NewName = name;
		NewName.append(":1");
		wallobject->AddPolygon(NewName.c_str(), texture, varray2, tw2, th2, true);
	}

	return true;
}

bool SBS::AddCustomFloor(WallObject* wallobject, const char *name, const char *texture, std::vector<Ogre::Vector2> &varray, float altitude, float tw, float th)
{
	//Same as AddCustomWall, with only one altitude value value
	std::vector<Ogre::Vector3> varray3;

	//set up 3D vertex array
	varray3.reserve(varray.size());
	for (int i = 0; i < (int)varray.size(); i++)
	{
		varray3.push_back(Ogre::Vector3(varray[i].x, altitude, varray[i].y));
	}

	//pass data on to AddCustomWall function
	return AddCustomWall(wallobject, name, texture, varray3, tw, th);
}

bool SBS::AddTriangleWall(WallObject* wallobject, const char *name, const char *texture, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float tw, float th)
{
	//Adds a triangular wall with the specified dimensions
	std::vector<Ogre::Vector3> varray;

	//set up temporary vertex array
	varray.reserve(3);
	varray.push_back(Ogre::Vector3(x1, y1, z1));
	varray.push_back(Ogre::Vector3(x2, y2, z2));
	varray.push_back(Ogre::Vector3(x3, y3, z3));

	//pass data on to AddCustomWall function
	return AddCustomWall(wallobject, name, texture, varray, tw, th);
}

void SBS::EnableBuildings(bool value)
{
	//turns buildings on/off
	Buildings->Enable(value);
	IsBuildingsEnabled = value;
}

void SBS::EnableLandscape(bool value)
{
	//turns landscape on/off
	Landscape->Enable(value);
	IsLandscapeEnabled = value;
}

void SBS::EnableExternal(bool value)
{
	//turns external on/off
	External->Enable(value);
	IsExternalEnabled = value;
}

void SBS::EnableSkybox(bool value)
{
	//turns skybox on/off
	if (SkyBox)
	{
		SkyBox->Enable(value);
		IsSkyboxEnabled = value;
	}
	else
		IsSkyboxEnabled = true;
}

void SBS::CreateSky(const char *filenamebase)
{
	//create skybox

	std::string file = filenamebase;
	Mount(std::string("data/sky-" + file + ".zip").c_str(), "sky");

	//load textures
	SetLighting();
	LoadTexture("sky/up.jpg", "SkyTop", 1, 1, false, false, false, 0);
	LoadTexture("sky/down.jpg", "SkyBottom", 1, 1, false, false, false, 0);
	LoadTexture("sky/left.jpg", "SkyLeft", 1, 1, false, false, false, 0);
	LoadTexture("sky/right.jpg", "SkyRight", 1, 1, false, false, false, 0);
	LoadTexture("sky/front.jpg", "SkyFront", 1, 1, false, false, false, 0);
	LoadTexture("sky/back.jpg", "SkyBack", 1, 1, false, false, false, 0);
	ResetLighting();

	SkyBox = new MeshObject(object, "SkyBox");
	SkyBox->no_collider = true;

	//create a skybox that extends by default 30 miles (30 * 5280 ft) in each direction
	float skysize = GetConfigInt("Skyscraper.SBS.HorizonDistance", 30) * 5280.0f;
	ResetTextureMapping(true);
	WallObject *wall = new WallObject(SkyBox, SkyBox->object, true);

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

	ResetTextureMapping();
	delete wall;
}

int SBS::GetFloorNumber(float altitude, int lastfloor, bool checklastfloor)
{
	//Returns floor number located at a specified altitude

	if (TotalFloors() == 0)
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
		return fabsf(x1 - x2);
	if (x1 == x2)
		return fabsf(z1 - z2);
	if ((x1 != x2) && (z2 != x2))
		return sqrtf(powf(fabsf(x1 - x2), 2) + powf(fabsf(z1 - z2), 2)); //calculate diagonals
	return 0;
}

Object* SBS::CreateShaft(int number, float CenterX, float CenterZ, int _startfloor, int _endfloor)
{
	//create a shaft object

	for (size_t i = 0; i < ShaftArray.size(); i++)
	{
		if (ShaftArray[i].number == number)
		{
			std::string num = ToString(number);
			ReportError("Shaft " + num + " already exists");
			return 0;
		}
	}

	//verify floor range
	if (_startfloor > _endfloor)
	{
		ReportError("CreateShaft: starting floor is greater than ending floor");
		return 0;
	}

	if (IsValidFloor(_startfloor) == false)
	{
		std::string num = ToString(_startfloor);
		ReportError("CreateShaft: Invalid starting floor " + num);
		return 0;
	}
	if (IsValidFloor(_endfloor) == false)
	{
		std::string num = ToString(_endfloor);
		ReportError("CreateShaft: Invalid ending floor " + num);
		return 0;
	}

	ShaftMap shaft;
	shaft.number = number;
	shaft.object = new Shaft(number, CenterX, CenterZ, _startfloor, _endfloor);
	ShaftArray.push_back(shaft);
	return shaft.object->object;
}

Object* SBS::CreateStairwell(int number, float CenterX, float CenterZ, int _startfloor, int _endfloor)
{
	//create a stairwell object

	for (size_t i = 0; i < StairsArray.size(); i++)
	{
		if (StairsArray[i].number == number)
		{
			std::string num = ToString(number);
			ReportError("Stairwell " + num + " already exists");
			return 0;
		}
	}

	//verify floor range
	if (_startfloor > _endfloor)
	{
		ReportError("CreateStairwell: starting floor is greater than ending floor");
		return 0;
	}
	if (IsValidFloor(_startfloor) == false)
	{
		std::string num = ToString(_startfloor);
		ReportError("CreateStairwell: Invalid starting floor " + num);
		return 0;
	}
	if (IsValidFloor(_endfloor) == false)
	{
		std::string num = ToString(_endfloor);
		ReportError("CreateStairwell: Invalid ending floor " + num);
		return 0;
	}

	StairsMap stairs;
	stairs.number = number;
	stairs.object = new Stairs(number, CenterX, CenterZ, _startfloor, _endfloor);
	StairsArray.push_back(stairs);
	return stairs.object->object;
}

bool SBS::NewElevator(int number)
{
	//create a new elevator object
	for (size_t i = 0; i < ElevatorArray.size(); i++)
		if (ElevatorArray[i].number == number)
			return false;

	ElevatorMap elev;
	elev.number = number;
	elev.object = new Elevator(number);
	ElevatorArray.push_back(elev);
	return true;
}

bool SBS::NewFloor(int number)
{
	//create a new floor object
	for (size_t i = 0; i < FloorArray.size(); i++)
		if (FloorArray[i].number == number)
			return false;

	FloorMap floor;
	floor.number = number;
	floor.object = new Floor(number);
	FloorArray.push_back(floor);

	if (number < 0)
		Basements++;
	else
		Floors++;
	return true;
}

int SBS::Elevators()
{
	//return the number of elevators
	return (int)ElevatorArray.size();
}

int SBS::TotalFloors()
{
	//return the number of floors
	return (int)FloorArray.size();
}

int SBS::Shafts()
{
	//return the number of shafts
	return (int)ShaftArray.size();
}

int SBS::StairsNum()
{
	//return the number of stairs
	return (int)StairsArray.size();
}

Floor* SBS::GetFloor(int number)
{
	//return pointer to floor object

	//return previous cached entry if the same
	if (getfloor_number == number && getfloor_result)
		return getfloor_result;

	if (FloorArray.size() > 0)
	{
		//quick prediction
		if (Basements + number < (int)FloorArray.size() && Basements + number >= 0)
		{
			FloorMap floor = FloorArray[Basements + number];
			if (floor.number == number)
			{
				if (floor.object)
				{
					getfloor_number = number;
					getfloor_result = floor.object;
					return floor.object;
				}
				else
				{
					getfloor_number = 0;
					getfloor_result = 0;
					return 0;
				}
			}
			else if (number < 0)
			{
				if (-(number + 1) < (int)FloorArray.size())
				{
					floor = FloorArray[-(number + 1)];
					if (floor.number == number)
					{
						if (floor.object)
						{
							getfloor_number = number;
							getfloor_result = floor.object;
							return floor.object;
						}
						else
						{
							getfloor_number = 0;
							getfloor_result = 0;
							return 0;
						}
					}
				}
			}
		}
	}

	for (size_t i = 0; i < (int)FloorArray.size(); i++)
	{
		if (FloorArray[i].number == number)
		{
			getfloor_number = number;
			getfloor_result = FloorArray[i].object;
			return getfloor_result;
		}
	}

	getfloor_number = 0;
	getfloor_result = 0;
	return 0;
}

Elevator* SBS::GetElevator(int number)
{
	//return pointer to elevator object

	if (number < 1 || number > Elevators())
		return 0;

	if (getelevator_number == number && getelevator_result)
		return getelevator_result;

	if ((int)ElevatorArray.size() > number - 1)
	{
		//quick prediction
		if (ElevatorArray[number - 1].number == number)
		{
			if (ElevatorArray[number - 1].object)
			{
				getelevator_number = number;
				getelevator_result = ElevatorArray[number - 1].object;
				return getelevator_result;
			}
			else
			{
				getelevator_number = 0;
				getelevator_result = 0;
				return 0;
			}
		}
	}

	for (size_t i = 0; i < ElevatorArray.size(); i++)
	{
		if (ElevatorArray[i].number == number)
		{
			getelevator_number = number;
			getelevator_result = ElevatorArray[i].object;
			return getelevator_result;
		}
	}

	getelevator_number = 0;
	getelevator_result = 0;
	return 0;
}

Shaft* SBS::GetShaft(int number)
{
	//return pointer to shaft object

	if (number < 1 || number > Shafts())
		return 0;

	if (getshaft_number == number && getshaft_result)
		return getshaft_result;

	if ((int)ShaftArray.size() > number - 1)
	{
		//quick prediction
		if (ShaftArray[number - 1].number == number)
		{
			if (ShaftArray[number - 1].object)
			{
				getshaft_number = number;
				getshaft_result = ShaftArray[number - 1].object;
				return getshaft_result;
			}
			else
			{
				getshaft_number = 0;
				getshaft_result = 0;
				return 0;
			}
		}
	}

	for (size_t i = 0; i < ShaftArray.size(); i++)
	{
		if (ShaftArray[i].number == number)
		{
			getshaft_number = number;
			getshaft_result = ShaftArray[i].object;
			return getshaft_result;
		}
	}

	getshaft_number = 0;
	getshaft_result = 0;
	return 0;
}

Stairs* SBS::GetStairs(int number)
{
	//return pointer to stairs object

	if (number < 1 || number > StairsNum())
		return 0;

	if (getstairs_number == number && getstairs_result)
		return getstairs_result;

	if ((int)StairsArray.size() > number - 1)
	{
		//quick prediction
		if (StairsArray[number - 1].number == number)
		{
			if (StairsArray[number - 1].object)
			{
				getstairs_number = number;
				getstairs_result = StairsArray[number - 1].object;
				return getstairs_result;
			}
			else
			{
				getstairs_number = 0;
				getstairs_result = 0;
				return 0;
			}
		}
	}

	for (size_t i = 0; i < StairsArray.size(); i++)
	{
		if (StairsArray[i].number == number)
		{
			getstairs_number = number;
			getstairs_result = StairsArray[i].object;
			return getstairs_result;
		}
	}

	getstairs_number = 0;
	getstairs_result = 0;
	return 0;
}

bool SBS::SetWallOrientation(const char *direction)
{
	//changes internal wall orientation parameter.
	//direction can either be "left" (negative), "center" (0), or "right" (positive).
	//default on startup is 1, or center.
	//the parameter is used to determine the location of the wall's
	//x1/x2 or z1/z2 coordinates in relation to the thickness extents

	std::string temp = direction;
	SetCase(temp, false);

	if (temp == "left")
		wall_orientation = 0;
	else if (temp == "center")
		wall_orientation = 1;
	else if (temp == "right")
		wall_orientation = 2;
	else
		return ReportError("SetWallOrientation: Invalid wall orientation");
	return true;
}

int SBS::GetWallOrientation()
{
	return wall_orientation;
}

bool SBS::SetFloorOrientation(const char *direction)
{
	//changes internal floor orientation parameter.
	//direction can either be "bottom" (negative), "center" (0), or "top" (positive).
	//default on startup is 2, or top.
	//the parameter is used to determine the location of the floor's
	//x1/x2 or z1/z2 coordinates in relation to the thickness extents

	std::string temp = direction;
	SetCase(temp, false);

	if (temp == "bottom")
		floor_orientation = 0;
	else if (temp == "center")
		floor_orientation = 1;
	else if (temp == "top")
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

void SBS::AddDoorwayWalls(WallObject *wallobject, const char *texture, float tw, float th)
{
	//add joining doorway polygons if needed

	if (wall1a == true && wall2a == true)
	{
		//true if doorway is facing forward/backward
		//false if doorway is facing left/right
		bool direction = fabsf(wall_extents_x.x - wall_extents_x.y) > fabsf(wall_extents_z.x - wall_extents_z.y);

		DrawWalls(false, true, false, false, false, false);
		if (direction == true)
			AddWallMain(wallobject, "DoorwayLeft", texture, 0, wall_extents_x.x, wall_extents_z.x, wall_extents_x.x, wall_extents_z.y, wall_extents_y.y - wall_extents_y.x, wall_extents_y.y - wall_extents_y.x, wall_extents_y.x, wall_extents_y.x, tw, th, true);
		else
			AddWallMain(wallobject, "DoorwayLeft", texture, 0, wall_extents_x.x, wall_extents_z.x, wall_extents_x.y, wall_extents_z.x, wall_extents_y.y - wall_extents_y.x, wall_extents_y.y - wall_extents_y.x, wall_extents_y.x, wall_extents_y.x, tw, th, true);
		ResetWalls();

		DrawWalls(true, false, false, false, false, false);
		if (direction == true)
			AddWallMain(wallobject, "DoorwayRight", texture, 0, wall_extents_x.y, wall_extents_z.x, wall_extents_x.y, wall_extents_z.y, wall_extents_y.y - wall_extents_y.x, wall_extents_y.y - wall_extents_y.x, wall_extents_y.x, wall_extents_y.x, tw, th, true);
		else
			AddWallMain(wallobject, "DoorwayRight", texture, 0, wall_extents_x.x, wall_extents_z.y, wall_extents_x.y, wall_extents_z.y, wall_extents_y.y - wall_extents_y.x, wall_extents_y.y - wall_extents_y.x, wall_extents_y.x, wall_extents_y.x, tw, th, true);

		AddFloorMain(wallobject, "DoorwayTop", texture, 0, wall_extents_x.x, wall_extents_z.x, wall_extents_x.y, wall_extents_z.y, wall_extents_y.y, wall_extents_y.y, false, false, tw, th, true);
		ResetWalls();

		ResetDoorwayWalls();
	}
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

void SBS::SetListenerPosition(const Ogre::Vector3 &position)
{
	//set position of sound listener object

	unsigned int timing;
	if (SmoothFrames > 0)
		timing = GetAverageTime();
	else
		timing = GetElapsedTime();

	//calculate sound velocity
	if (timing > 0)
	{
		listener_velocity.x = (position.x - listener_position.x) * (1000 / timing);
		listener_velocity.y = (position.y - listener_position.y) * (1000 / timing);
		listener_velocity.z = (position.z - listener_position.z) * (1000 / timing);
	}

	listener_position.x = position.x;
	listener_position.y = position.y;
	listener_position.z = position.z;

	if (DisableSound == false)
		soundsys->set3DListenerAttributes(0, &listener_position, &listener_velocity, &listener_forward, &listener_up);
}

void SBS::SetListenerDirection(const Ogre::Vector3 &front, const Ogre::Vector3 &top)
{
	//set direction of sound listener object
	listener_forward.x = front.x;
	listener_forward.y = front.y;
	listener_forward.z = front.z;
	listener_up.x = top.x;
	listener_up.y = top.y;
	listener_up.z = top.z;

	if (DisableSound == false)
		soundsys->set3DListenerAttributes(0, &listener_position, &listener_velocity, &listener_forward, &listener_up);
}

WallObject* SBS::AddWall(const char *meshname, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float altitude1, float altitude2, float tw, float th)
{
	//meshname can either be:
	//external, landscape, or buildings

	//Adds a wall with the specified dimensions
	std::string mesh = meshname;
	TrimString(mesh);
	SetCase(mesh, false);

	WallObject *wall;
	if (mesh == "external")
		wall = External->CreateWallObject(External->object, name);
	if (mesh == "buildings")
		wall = Buildings->CreateWallObject(Buildings->object, name);
	if (mesh == "landscape")
		wall = Landscape->CreateWallObject(Landscape->object, name);

	AddWallMain(wall, name, texture, thickness, x1, z1, x2, z2, height_in1, height_in2, altitude1, altitude2, tw, th, true);
	return wall;
}

WallObject* SBS::AddFloor(const char *meshname, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float altitude1, float altitude2, bool reverse_axis, bool texture_direction, float tw, float th, bool legacy_behavior)
{
	//meshname can either be:
	//external, landscape, or buildings

	//Adds a floor with the specified dimensions and vertical offset
	std::string mesh = meshname;
	TrimString(mesh);
	SetCase(mesh, false);

	WallObject *wall;
	if (mesh == "external")
		wall = External->CreateWallObject(External->object, name);
	if (mesh == "buildings")
		wall = Buildings->CreateWallObject(Buildings->object, name);
	if (mesh == "landscape")
		wall = Landscape->CreateWallObject(Landscape->object, name);

	AddFloorMain(wall, name, texture, thickness, x1, z1, x2, z2, altitude1, altitude2, reverse_axis, texture_direction, tw, th, true, legacy_behavior);
	return wall;
}

WallObject* SBS::AddGround(const char *name, const char *texture, float x1, float z1, float x2, float z2, float altitude, int tile_x, int tile_z)
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

	WallObject *wall = Landscape->CreateWallObject(Landscape->object, name);

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

			DrawWalls(false, true, false, false, false, false);
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

bool SBS::RegisterDoorCallback(Door *door)
{
	//register a door object for callbacks (used for door movement)

	int index = -1;
	for (int i = 0; i < (int)doorcallbacks.size(); i++)
	{
		if (doorcallbacks[i] == door)
			index = i;
	}

	if (index == -1)
	{
		//if door isn't already in the array, add it
		doorcallbacks.push_back(door);
	}
	else
	{
		//otherwise change door's direction
		if (doorcallbacks[index])
			doorcallbacks[index]->OpenDoor = !doorcallbacks[index]->OpenDoor;
	}
	return true;
}

bool SBS::UnregisterDoorCallback(Door *door)
{
	int index = -1;
	for (int i = 0; i < (int)doorcallbacks.size(); i++)
	{
		if (doorcallbacks[i] == door)
			index = i;
	}

	if (index != -1 && doorcallbacks[index])
	{
		//unregister existing door callback
		if (doorcallbacks[index]->IsMoving == false)
		{
			for (int i = 0; i < (int)doorcallbacks.size(); i++)
			{
				if (doorcallbacks[i] == door)
					doorcallbacks.erase(doorcallbacks.begin() + i);
			}
			return true;
		}
		else
			return ReportError("Door in use; cannot unregister callback");
	}
	else
		return false;
}

bool SBS::RegisterCallButtonCallback(CallButton *button)
{
	//register a door object for callbacks (used for door movement)

	int index = -1;
	for (int i = 0; i < (int)buttoncallbacks.size(); i++)
	{
		if (buttoncallbacks[i] == button)
			index = i;
	}

	if (index == -1)
	{
		//if call button isn't already in the array, add it
		buttoncallbacks.push_back(button);
	}
	else
		return false;
	return true;
}

bool SBS::UnregisterCallButtonCallback(CallButton *button)
{
	int index = -1;
	for (int i = 0; i < (int)buttoncallbacks.size(); i++)
	{
		if (buttoncallbacks[i] == button)
			index = i;
	}

	if (index != -1 && buttoncallbacks[index])
	{
		//unregister existing call button callback
		for (int i = 0; i < (int)buttoncallbacks.size(); i++)
		{
			if (buttoncallbacks[i] == button)
				buttoncallbacks.erase(buttoncallbacks.begin() + i);
		}
	}
	else
		return false;
	return true;
}

bool SBS::RegisterTimerCallback(TimerObject *timer)
{
	//register a timer object for callbacks

	int index = -1;
	for (int i = 0; i < (int)timercallbacks.size(); i++)
	{
		if (timercallbacks[i] == timer)
			index = i;
	}

	if (index == -1)
	{
		//if timer isn't already in the array, add it
		timercallbacks.push_back(timer);
	}
	else
		return false;
	return true;
}

bool SBS::UnregisterTimerCallback(TimerObject *timer)
{
	int index = -1;
	for (int i = 0; i < (int)timercallbacks.size(); i++)
	{
		if (timercallbacks[i] == timer)
			index = i;
	}

	if (index != -1 && timercallbacks[index])
	{
		//unregister existing call button callback
		for (int i = 0; i < (int)timercallbacks.size(); i++)
		{
			if (timercallbacks[i] == timer)
				timercallbacks.erase(timercallbacks.begin() + i);
		}
	}
	else
		return false;
	return true;
}

void SBS::ProcessCallButtons()
{
	//process all registered call buttons

	//the up and down sections need to be processed separately due to the removal of callbacks
	//during the run of each

	for (int i = 0; i < (int)buttoncallbacks.size(); i++)
	{
		//process up calls
		if (buttoncallbacks[i])
			buttoncallbacks[i]->Loop(1);
	}

	for (int i = 0; i < (int)buttoncallbacks.size(); i++)
	{
		//process down calls
		if (buttoncallbacks[i])
			buttoncallbacks[i]->Loop(-1);
	}
}

void SBS::ProcessDoors()
{
	//process all registered doors
	for (int i = 0; i < (int)doorcallbacks.size(); i++)
	{
		if (doorcallbacks[i])
		{
			if (doorcallbacks[i]->IsMoving == true)
				doorcallbacks[i]->MoveDoor();
			else
				UnregisterDoorCallback(doorcallbacks[i]);
		}
	}
}

void SBS::ProcessTimers()
{
	//process all registered timers
	for (int i = 0; i < (int)timercallbacks.size(); i++)
	{
		if (timercallbacks[i])
			timercallbacks[i]->Check();
	}
}

int SBS::GetDoorCallbackCount()
{
	//return the number of registered door callbacks
	return (int)doorcallbacks.size();
}

int SBS::GetCallButtonCallbackCount()
{
	//return the number of registered call button callbacks
	return (int)buttoncallbacks.size();
}

int SBS::GetTimerCallbackCount()
{
	//return the number of registered call button callbacks
	return (int)timercallbacks.size();
}

bool SBS::Mount(const char *filename, const char *path)
{
	//mounts a zip file into the virtual filesystem

	std::string file = VerifyFile(filename);
	std::string Path = path;

	Report("Mounting " + file + " as path " + Path);
	try
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(file, "Zip", Path, true);
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
	
	Ogre::Vector3 position = camera->GetPosition();
	int floor = camera->CurrentFloor;

	for (int i = 0; i < (int)FloorAutoArea.size(); i++)
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

Object* SBS::AddSound(const char *name, const char *filename, Ogre::Vector3 position, bool loop, float volume, int speed, float min_distance, float max_distance, float doppler_level, float cone_inside_angle, float cone_outside_angle, float cone_outside_volume, Ogre::Vector3 direction)
{
	//create a looping sound object
	Sound *sound = new Sound(object, name, false);
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
	sound->Loop(loop);
	if (loop && IsRunning == true)
		sound->Play();

	return sound->object;
}

std::vector<Sound*> SBS::GetSound(const char *name)
{
	//get sound by name

	std::string findname = name;
	SetCase(findname, false);
	std::vector<Sound*> soundlist;
	for (int i = 0; i < (int)sounds.size(); i++)
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

Ogre::Vector3 SBS::ToLocal(const Ogre::Vector3& remote_value, bool rescale)
{
	//convert remote (OGRE) vertex positions to local (SBS) positions
	//also convert Z value to OGRE's right-hand coordinate system

	Ogre::Vector3 newvalue;
	newvalue.x = remote_value.x;
	newvalue.y = remote_value.y;
	newvalue.z = -remote_value.z; //flip z value for OGRE's right-hand coordinate system
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

Ogre::Vector3 SBS::ToRemote(const Ogre::Vector3& local_value, bool rescale)
{
	//convert local (SBS) vertex positions to remote (OGRE) positions
	
	Ogre::Vector3 newvalue;
	newvalue.x = local_value.x;
	newvalue.y = local_value.y;
	newvalue.z = -local_value.z; //flip z value for OGRE's right-hand coordinate system
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

/*WallObject* SBS::GetWallObject(std::vector<WallObject*> &wallarray, int polygon_index)
{
	//returns the wall object that contains the specified polygon index
	for (int i = 0; i < (int)wallarray.size(); i++)
	{
		for (int j = 0; j < (int)wallarray[i]->handles.size(); j++)
		{
			if (wallarray[i]->handles[j] == polygon_index)
				return wallarray[i];
		}
	}
	return 0;
}*/

std::string SBS::TruncateNumber(float value, int decimals)
{
	//truncates the numeric value to the specified number of decimal places (does not round)
	
	if ((int)value == value)
		decimals = 0; //value is an integer

	std::stringstream buffer;
	buffer.precision(decimals);
	buffer << std::fixed << value;

	return buffer.str();
}

std::string SBS::TruncateNumber(const char *value, int decimals)
{
	//truncates the numeric value to the specified number of decimal places (does not round)
	std::string number = value;
	
	if (decimals < 1)
		return number;
	number.erase((int)number.find(".") + decimals + 1);
	if (number.at(number.length() - 1) == '.')
		number = number.substr(0, number.length() - 1); //strip of extra decimal point if even
	return number;
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
	output.append("Building Name: " + BuildingName + "\n");
	output.append("Building Filename: " + BuildingFilename + "\n");
	output.append("Building Version: " + BuildingVersion + "\n");
	output.append("InStairwell: ");
	output.append(std::string(BoolToString(InStairwell)).c_str());
	output.append("\n");
	output.append("InElevator: ");
	output.append(std::string(BoolToString(InElevator)).c_str());
	output.append("\n");
	output.append("InShaft: ");
	output.append(std::string(BoolToString(InShaft)).c_str());
	output.append("\n");
	output.append("CameraFloor: ");
	if (camera)
		output.append(ToString(camera->CurrentFloor));
	output.append("\n");
	output.append("ElevatorNumber: ");
	output.append(ToString(ElevatorNumber));
	output.append("\n");
	output.append("ElevatorSync: ");
	output.append(std::string(BoolToString(ElevatorSync)).c_str());
	output.append("\n");
	output.append("Running Time: ");
	output.append(TruncateNumber(running_time, 2));
	output.append("\n");
	output.append("BuildingsEnabled: ");
	output.append(std::string(BoolToString(IsBuildingsEnabled)).c_str());
	output.append("\n");
	output.append("ExternalEnabled: ");
	output.append(std::string(BoolToString(IsExternalEnabled)).c_str());
	output.append("\n");
	output.append("LandscapeEnabled: ");
	output.append(std::string(BoolToString(IsLandscapeEnabled)).c_str());
	output.append("\n");
	output.append("SkyboxEnabled: ");
	output.append(std::string(BoolToString(IsSkyboxEnabled)).c_str());
	output.append("\n");
	output.append("Verbose: ");
	output.append(std::string(BoolToString(Verbose)).c_str());
	output.append("\n");
	output.append("InterfloorOnTop: ");
	output.append(std::string(BoolToString(InterfloorOnTop)).c_str());
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

	if (!object->GetRawObject())
		return ReportError("Invalid raw object " + number);

	//don't delete permanent objects
	if (object->IsPermanent() == true)
		return ReportError("Cannot delete permanent object " + number);

	std::string type = object->GetType();

	//perform standard delete based on object type
	if (type == "Floor")
	{
		Floor *floor = (Floor*)object->GetRawObject();

		//make sure no shaft is dependent on this floor
		for (int i = 0; i < (int)ShaftArray.size(); i++)
		{
			Shaft *shaft = ShaftArray[i].object;
			if (shaft)
			{
				if (shaft->IsValidFloor(floor->Number) == true)
					return ReportError("Cannot delete floor " + ToString2(floor->Number) + " - in use by shaft " + ToString2(shaft->ShaftNumber));
			}
		}

		//make sure no stairwell is dependent on this floor
		for (int i = 0; i < (int)StairsArray.size(); i++)
		{
			Stairs *stairs = StairsArray[i].object;
			if (stairs)
			{
				if (stairs->IsValidFloor(floor->Number) == true)
					return ReportError("Cannot delete floor " + ToString2(floor->Number) + " - in use by stairwell " + ToString2(stairs->StairsNum));
			}
		}

		//restrict deletions to only lowest/highest floors
		if (floor->Number >= 0 && GetFloor(floor->Number + 1))
			return ReportError("Only the highest floor can be deleted");

		if (floor->Number < 0 && GetFloor(floor->Number - 1))
			return ReportError("Only the lowest basement can be deleted");

		delete floor;
		deleted = true;
	}
	if (type == "Elevator")
	{
		delete (Elevator*)object->GetRawObject();
		deleted = true;
	}
	if (type == "ButtonPanel")
	{
		delete (ButtonPanel*)object->GetRawObject();
		deleted = true;
	}
	if (type == "CallButton")
	{
		delete (CallButton*)object->GetRawObject();
		deleted = true;
	}
	if (type == "DirectionalIndicator")
	{
		delete (DirectionalIndicator*)object->GetRawObject();
		deleted = true;
	}
	if (type == "Door")
	{
		delete (Door*)object->GetRawObject();
		deleted = true;
	}
	if (type == "ElevatorDoor")
	{
		delete (ElevatorDoor*)object->GetRawObject();
		deleted = true;
	}
	if (type == "FloorIndicator")
	{
		delete (FloorIndicator*)object->GetRawObject();
		deleted = true;
	}
	if (type == "Shaft")
	{
		Shaft *shaft = (Shaft*)object->GetRawObject();

		//make sure no elevator is dependent on this shaft
		for (int i = 0; i < (int)ElevatorArray.size(); i++)
		{
			Elevator *elev = ElevatorArray[i].object;
			if (elev)
			{
				if (elev->AssignedShaft == shaft->ShaftNumber)
					return ReportError("Cannot delete shaft " + ToString2(shaft->ShaftNumber) + " - in use by elevator " + ToString2(elev->Number));
			}
		}

		delete shaft;
		deleted = true;
	}
	if (type == "Sound")
	{
		delete (Sound*)object->GetRawObject();
		deleted = true;
	}
	if (type == "Stairs")
	{
		delete (Stairs*)object->GetRawObject();
		deleted = true;
	}
	if (type == "Wall")
	{
		WallObject *obj = (WallObject*)object->GetRawObject();
		obj->DeletePolygons(true);
		delete obj;
		deleted = true;
	}
	if (type == "Model")
	{
		delete (Model*)object->GetRawObject();
		deleted = true;
	}
	if (type == "Control")
	{
		delete (Control*)object->GetRawObject();
		deleted = true;
	}
	if (type == "Trigger")
	{
		delete (Trigger*)object->GetRawObject();
		deleted = true;
	}

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

	std::string number = ToString(object->GetNumber());

	if (!object->GetRawObject())
		return ReportError("Invalid raw object " + number);

	std::string type = object->GetType();

	if (type == "Mesh")
	{
		MeshObject* mesh = (MeshObject*)object->GetRawObject();
		if (relative == false)
		{
			if (X == false)
				position.x = mesh->GetPosition().x;
			if (Y == false)
				position.y = mesh->GetPosition().y;
			if (Z == false)
				position.z = mesh->GetPosition().z;
		}
		mesh->Move(position, relative, relative, relative);
	}
	if (type == "ButtonPanel")
	{
		ButtonPanel* panel = (ButtonPanel*)object->GetRawObject();
		if (relative == true)
			panel->Move(position);
	}
	if (type == "DirectionalIndicator")
	{
		DirectionalIndicator* ind = (DirectionalIndicator*)object->GetRawObject();
		if (relative == true)
			ind->Move(position);
		else
		{
			if (X == false)
				position.x = ind->GetPosition().x;
			if (Y == false)
				position.y = ind->GetPosition().y;
			if (Z == false)
				position.z = ind->GetPosition().z;
			ind->SetPosition(position);
		}
	}
	if (type == "Door")
	{
		Door* door = (Door*)object->GetRawObject();
		if (relative == false)
		{
			if (X == false)
				position.x = door->GetPosition().x;
			if (Y == false)
				position.y = door->GetPosition().y;
			if (Z == false)
				position.z = door->GetPosition().z;
		}
		door->Move(position, relative, relative, relative);
	}
	if (type == "ElevatorDoor")
	{
		ElevatorDoor* door = (ElevatorDoor*)object->GetRawObject();
		if (relative == false)
		{
			if (X == false)
				position.x = door->GetPosition().x;
			if (Y == false)
				position.y = door->GetPosition().y;
			if (Z == false)
				position.z = door->GetPosition().z;
		}
		door->Move(position, relative, relative, relative);
	}
	if (type == "FloorIndicator")
	{
		FloorIndicator* ind = (FloorIndicator*)object->GetRawObject();
		if (relative == true)
			ind->MovePosition(position);
	}
	if (type == "Sound")
	{
		Sound* sound = (Sound*)object->GetRawObject();
		if (relative == false)
		{
			if (X == false)
				position.x = sound->GetPosition().x;
			if (Y == false)
				position.y = sound->GetPosition().y;
			if (Z == false)
				position.z = sound->GetPosition().z;
			sound->SetPosition(position);
		}
		else
		{
			Ogre::Vector3 newpos = sound->GetPosition() + position;
			sound->SetPosition(newpos);
		}
	}
	if (type == "Model")
	{
		Model* model = (Model*)object->GetRawObject();
		if (relative == false)
		{
			if (X == false)
				position.x = model->GetPosition().x;
			if (Y == false)
				position.y = model->GetPosition().y;
			if (Z == false)
				position.z = model->GetPosition().z;
		}
		model->Move(position, relative, relative, relative);
	}
	if (type == "Control")
	{
		Control* control = (Control*)object->GetRawObject();
		if (relative == true)
			control->Move(position);
		else
		{
			if (X == false)
				position.x = control->GetPosition().x;
			if (Y == false)
				position.y = control->GetPosition().y;
			if (Z == false)
				position.z = control->GetPosition().z;
			control->SetPosition(position);
		}
	}
	if (type == "Trigger")
	{
		Trigger* trigger = (Trigger*)object->GetRawObject();
		if (relative == true)
			trigger->Move(position);
		else
		{
			if (X == false)
				position.x = trigger->GetPosition().x;
			if (Y == false)
				position.y = trigger->GetPosition().y;
			if (Z == false)
				position.z = trigger->GetPosition().z;
			trigger->SetPosition(position);
		}
	}

	return true;
}

bool SBS::RotateObject(Object *object, Ogre::Vector3 rotation, float speed, bool relative, bool X, bool Y, bool Z)
{
	//rotate an object by reference
	//if relative is false, the X, Y and Z values determine which position axes should be set

	if (!object)
		return ReportError("Invalid object");

	std::string number = ToString(object->GetNumber());

	if (!object->GetRawObject())
		return ReportError("Invalid raw object " + number);

	std::string type = object->GetType();

	if (type == "Mesh")
	{
		MeshObject* mesh = (MeshObject*)object->GetRawObject();
		if (relative == true)
			mesh->Rotate(rotation, speed);
		else
		{
			if (X == false)
				rotation.x = mesh->GetRotation().x;
			if (Y == false)
				rotation.y = mesh->GetRotation().y;
			if (Z == false)
				rotation.z = mesh->GetRotation().z;
			mesh->SetRotation(rotation);
		}
	}
	if (type == "Model")
	{
		Model* model = (Model*)object->GetRawObject();
		if (relative == true)
			model->Rotate(rotation, speed);
		else
		{
			if (X == false)
				rotation.x = model->GetRotation().x;
			if (Y == false)
				rotation.y = model->GetRotation().y;
			if (Z == false)
				rotation.z = model->GetRotation().z;
			model->SetRotation(rotation);
		}
	}

	return true;
}

Ogre::Vector3 SBS::GetObjectPosition(Object *object)
{
	//get object's position

	if (!object)
	{
		ReportError("Invalid object");
		return Ogre::Vector3::ZERO;
	}

	std::string number = ToString(object->GetNumber());

	if (!object->GetRawObject())
	{
		ReportError("Invalid raw object " + number);
		return Ogre::Vector3::ZERO;
	}

	std::string type = object->GetType();

	if (type == "Mesh")
	{
		MeshObject* mesh = (MeshObject*)object->GetRawObject();
		return mesh->GetPosition();
	}
	if (type == "ButtonPanel")
	{
		ButtonPanel* panel = (ButtonPanel*)object->GetRawObject();
		return panel->GetPosition();
	}
	if (type == "DirectionalIndicator")
	{
		DirectionalIndicator* ind = (DirectionalIndicator*)object->GetRawObject();
		return ind->GetPosition();
	}
	if (type == "Door")
	{
		Door* door = (Door*)object->GetRawObject();
		return door->GetPosition();
	}
	if (type == "ElevatorDoor")
	{
		ElevatorDoor* door = (ElevatorDoor*)object->GetRawObject();
		return door->GetPosition();
	}
	if (type == "FloorIndicator")
	{
		FloorIndicator* ind = (FloorIndicator*)object->GetRawObject();
		return ind->GetPosition();
	}
	if (type == "Sound")
	{
		Sound* sound = (Sound*)object->GetRawObject();
		return sound->GetPosition();
	}
	if (type == "Model")
	{
		Model* model = (Model*)object->GetRawObject();
		return model->GetPosition();
	}
	if (type == "Control")
	{
		Control* control = (Control*)object->GetRawObject();
		return control->GetPosition();
	}
	if (type == "Trigger")
	{
		Trigger* trigger = (Trigger*)object->GetRawObject();
		return trigger->GetPosition();
	}

	return Ogre::Vector3::ZERO;
}

Ogre::Vector3 SBS::GetObjectRotation(Object *object)
{
	//rotate an object by reference

	if (!object)
	{
		ReportError("Invalid object");
		return Ogre::Vector3::ZERO;
	}

	std::string number = ToString(object->GetNumber());

	if (!object->GetRawObject())
	{
		ReportError("Invalid raw object " + number);
		return Ogre::Vector3::ZERO;
	}

	std::string type = object->GetType();

	if (type == "Mesh")
	{
		MeshObject* mesh = (MeshObject*)object->GetRawObject();
		return mesh->GetRotation();
	}
	if (type == "Model")
	{
		Model* model = (Model*)object->GetRawObject();
		return model->GetRotation();
	}

	return Ogre::Vector3::ZERO;
}

void SBS::RemoveFloor(Floor *floor)
{
	//remove a floor (does not delete the object)
	for (int i = 0; i < (int)FloorArray.size(); i++)
	{
		if (FloorArray[i].object == floor)
		{
			if (FloorArray[i].object->Number < 0)
				Basements--;
			else
				Floors--;

			FloorArray.erase(FloorArray.begin() + i);

			//clear cached values
			getfloor_result = 0;
			getfloor_number = 0;
			return;
		}
	}
}

void SBS::RemoveElevator(Elevator *elevator)
{
	//remove an elevator (does not delete the object)
	for (int i = 0; i < (int)ElevatorArray.size(); i++)
	{
		if (ElevatorArray[i].object == elevator)
		{
			ElevatorArray.erase(ElevatorArray.begin() + i);

			//clear cached values
			getelevator_result = 0;
			getelevator_number = 0;
			return;
		}
	}
}

void SBS::RemoveShaft(Shaft *shaft)
{
	//remove a shaft (does not delete the object)
	for (int i = 0; i < (int)ShaftArray.size(); i++)
	{
		if (ShaftArray[i].object == shaft)
		{
			ShaftArray.erase(ShaftArray.begin() + i);

			//clear cached values
			getshaft_result = 0;
			getshaft_number = 0;
			return;
		}
	}
}

void SBS::RemoveStairs(Stairs *stairs)
{
	//remove a stairs object (does not delete the object)
	for (int i = 0; i < (int)StairsArray.size(); i++)
	{
		if (StairsArray[i].object == stairs)
		{
			StairsArray.erase(StairsArray.begin() + i);

			//clear cached values
			getstairs_result = 0;
			getstairs_number = 0;
			return;
		}
	}
}

void SBS::RemoveSound(Sound *sound)
{
	//remove a sound from the array
	//this does not delete the object

	for (int i = 0; i < (int)sounds.size(); i++)
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
	for (int i = 0; i < (int)lights.size(); i++)
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
	for (int i = 0; i < (int)ModelArray.size(); i++)
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
	for (int i = 0; i < (int)ControlArray.size(); i++)
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
	for (int i = 0; i < (int)TriggerArray.size(); i++)
	{
		if (TriggerArray[i] == trigger)
		{
			TriggerArray.erase(TriggerArray.begin() + i);
			return;
		}
	}
}

std::string SBS::VerifyFile(const char *filename)
{
	//verify a filename
	//if it exists, return the same filename
	//otherwise search the related folder and find a matching filename with a different
	//case (fixes case-sensitivity issues mainly on Linux)
	//returns the original string if not found

	std::string file = filename;
	TrimString(file);

	ReplaceAll(file, "\\", "/");

	//check for a cached result
	for (int i = 0; i < (int)verify_results.size(); i++)
	{
		if (verify_results[i].filename == file)
			return verify_results[i].result;
	}

#if OGRE_VERSION >= 0x00010900
	Ogre::FileSystemArchive filesystem(".", "FileSystem", false);
#else
	Ogre::FileSystemArchive filesystem(".", "FileSystem");
#endif
	if (filesystem.exists(file))
	{
		CacheFilename(file, file);
		return file;
	}

	std::string directory;
	int loc = (int)file.find_last_of("/");
	directory = file.substr(0, loc + 1);

	Ogre::StringVectorPtr listing = filesystem.list();
	for (int i = 0; i < (int)listing->size(); i++)
	{
		std::string check = listing->at(i);
		std::string checkoriginal = SetCaseCopy(check, false);
		std::string checkfile = SetCaseCopy(file, false);
		if (checkoriginal == checkfile)
		{
			CacheFilename(file, check);
			return check;
		}
	}

	CacheFilename(file, file);
	return file;
}

bool SBS::FileExists(const char *filename)
{
	//check to see if the specified file exists

	std::string file = filename;
	TrimString(file);

#if OGRE_VERSION >= 0x00010900
	Ogre::FileSystemArchive filesystem(".", "FileSystem", false);
#else
	Ogre::FileSystemArchive filesystem(".", "FileSystem");
#endif
	if (filesystem.exists(file))
		return true;

	//if a corrected filename is found, return true
	std::string verify = VerifyFile(file.c_str());
	if (verify != file)
		return true;
	return false;
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

	for (int i = 0; i < (int)meshes.size(); i++)
	{
		meshes[i]->Prepare();
	}

	if (report == true)
		Report("Creating colliders...");

	for (int i = 0; i < (int)meshes.size(); i++)
	{
		if (meshes[i]->tricollider == true)
			meshes[i]->CreateCollider();
		else
			meshes[i]->CreateBoxCollider(1);
	}

	if (report == true)
		Report("Finished prepare");
}

Object* SBS::AddLight(const char *name, int type, Ogre::Vector3 position, Ogre::Vector3 direction, float color_r, float color_g, float color_b, float spec_color_r, float spec_color_g, float spec_color_b, float spot_inner_angle, float spot_outer_angle, float spot_falloff, float att_range, float att_constant, float att_linear, float att_quadratic)
{
	//add a global light

	Light* light = new Light(object, name, type, position, direction, color_r, color_g, color_b, spec_color_r, spec_color_g, spec_color_b, spot_inner_angle, spot_outer_angle, spot_falloff, att_range, att_constant, att_linear, att_quadratic);
	lights.push_back(light);
	return light->object;
}

void SBS::AddMeshHandle(MeshObject* handle)
{
	meshes.push_back(handle);
}

void SBS::DeleteMeshHandle(MeshObject* handle)
{
	for (int i = 0; i < (int)meshes.size(); i++)
	{
		if (meshes[i] == handle)
		{
			meshes.erase(meshes.begin() + i);
			return;
		}
	}
}

MeshObject* SBS::FindMeshObject(Ogre::MeshPtr meshwrapper)
{
	//find a mesh object by searching for matching wrapper
	for (int i = 0; i < (int)meshes.size(); i++)
	{
		if (meshes[i]->MeshWrapper == meshwrapper)
			return meshes[i];
	}
	return 0;
}

MeshObject* SBS::FindMeshObject(std::string name)
{
	//find a mesh object by searching for matching wrapper
	for (int i = 0; i < (int)meshes.size(); i++)
	{
		if (meshes[i]->name == name)
			return meshes[i];
	}
	return 0;
}

Object* SBS::AddModel(const char *name, const char *filename, bool center, Ogre::Vector3 position, Ogre::Vector3 rotation, float max_render_distance, float scale_multiplier, bool enable_physics, float restitution, float friction, float mass)
{
	//add a model
	Model* model = new Model(object, name, filename, center, position, rotation, max_render_distance, scale_multiplier, enable_physics, restitution, friction, mass);
	if (model->load_error == true)
	{
		delete model;
		return 0;
	}
	ModelArray.push_back(model);
	return model->object;
}

int SBS::GetConfigInt(std::string key, int default_value)
{
	std::string result = configfile.getSetting(key, Ogre::StringUtil::BLANK, ToString2(default_value));
	return Ogre::StringConverter::parseInt(result);
}

std::string SBS::GetConfigString(std::string key, std::string default_value)
{
	return configfile.getSetting(key, Ogre::StringUtil::BLANK, default_value);
}

bool SBS::GetConfigBool(std::string key, bool default_value)
{
	std::string result = configfile.getSetting(key, Ogre::StringUtil::BLANK, ToString2(default_value));
	return Ogre::StringConverter::parseBool(result);
}

float SBS::GetConfigFloat(std::string key, float default_value)
{
	std::string result = configfile.getSetting(key, Ogre::StringUtil::BLANK, ToString2(default_value));
	return Ogre::StringConverter::parseReal(result);
}

bool SBS::InBox(Ogre::Vector3 &start, Ogre::Vector3 &end, Ogre::Vector3 &test)
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

std::string SBS::GetMountPath(const char *filename, std::string &newfilename)
{
	//get mountpoint (resource group) path of given file
	//if not found, return "General"
	
	std::string file = filename;
	Ogre::StringVector list = Ogre::ResourceGroupManager::getSingleton().getResourceGroups();
	newfilename = file;

	for (int i = 0; i < (int)list.size(); i++)
	{
		std::string check = file + "/";
		if (StartsWith(check, list[i].c_str()) == true)
		{
			newfilename = file.substr(list[i].size() + 1);
			return list[i];
		}
		check = file + "\\";
		if (StartsWith(check, list[i].c_str()) == true)
		{
			newfilename = file.substr(list[i].size() + 1);
			return list[i];
		}
	}
	return "General";
}

void SBS::ShowColliders(bool value)
{
	if (mWorld)
		mWorld->setShowDebugShapes(value);
	camera->ShowDebugShape(value);
}

void SBS::CacheFilename(std::string filename, std::string result)
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

Object* SBS::AddControl(const char *name, const char *sound, const char *direction, float CenterX, float CenterZ, float width, float height, float voffset, std::vector<std::string> &action_names, std::vector<std::string> &textures)
{
	//add a control
	std::vector<Action*> actionnull; //not used
	Control* control = new Control(object, name, sound, action_names, actionnull, textures, direction, width, height, voffset, true);
	control->SetPosition(Ogre::Vector3(CenterX, 0, CenterZ));
	ControlArray.push_back(control);
	return control->object;
}

Object* SBS::AddTrigger(const char *name, const char *sound_file, Ogre::Vector3 &area_min, Ogre::Vector3 &area_max, std::vector<std::string> &action_names)
{
	//add a trigger
	Trigger* trigger = new Trigger(object, name, sound_file, area_min, area_max, action_names);
	TriggerArray.push_back(trigger);
	return trigger->object;
}

Action* SBS::AddAction(const std::string name, std::vector<Object*> &action_parents, const std::string &command, const std::vector<std::string> &parameters)
{
	//add a global action

	Action *action = new Action(name, action_parents, command, parameters);
	ActionArray.push_back(action);
	return action;
}

Action* SBS::AddAction(const std::string name, std::vector<Object*> &action_parents, const std::string &command)
{
	//add a global action

	Action *action = new Action(name, action_parents, command);
	ActionArray.push_back(action);
	return action;
}

std::vector<Action*> SBS::GetAction(std::string name)
{
	//get action by name
	ReplaceAll(name, " ", "");
	std::vector<Action*> actionlist;
	for (int i = 0; i < (int)ActionArray.size(); i++)
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

bool SBS::AddActionParent(const std::string name, std::vector<Object*> &parents)
{
	//add parent to actions specified by 'name'

	bool result = false;
	std::vector<Action*> actionlist = GetAction(name);

	for (int i = 0; i < (int)actionlist.size(); i++)
	{
		Action *action = actionlist[i];
		for (int j = 0; j < (int)parents.size(); j++)
		{
			if (action->AddParent(parents[j]))
				result = true;
		}
	}
	return result;
}

bool SBS::RemoveActionParent(const std::string name, std::vector<Object*> &parents)
{
	//remove parent object from actions specified by 'name'

	bool result = false;
	std::vector<Action*> actionlist = GetAction(name);

	for (int i = 0; i < (int)actionlist.size(); i++)
	{
		Action *action = actionlist[i];
		for (int j = 0; j < (int)parents.size(); j++)
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
	for (int i = 0; i < (int)ActionArray.size(); i++)
	{
		Action *action = ActionArray[i];
		for (int j = 0; j < (int)parents.size(); j++)
		{
			if (action->RemoveParent(parents[j]))
				result = true;
		}
	}
	return result;
}

bool SBS::RemoveAction(const std::string name)
{
	//remove action by name

	bool result = false;
	for (int i = 0; i < (int)ActionArray.size(); i++)
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
	for (int i = 0; i < (int)ActionArray.size(); i++)
	{
		if (ActionArray[i] == action)
		{
			delete ActionArray[i];
			ActionArray.erase(ActionArray.begin() + i);
			i--;
			result = true;

			//remove reference to action in all control objects
			for (int j = 0; j < (int)control_index.size(); j++)
			{
				control_index[j]->RemoveAction(action);
			}
		}
	}
	return result;
}

Object* SBS::GetObject(const std::string name)
{
	//get object by name

	std::string name2 = name;
	ReplaceAll(name2, " ", "");

	for (int i = 0; i < (int)ObjectArray.size(); i++)
	{
		if (ObjectArray[i])
		{
			std::string tmpname = ObjectArray[i]->GetName();
			ReplaceAll(tmpname, " ", "");
			if (tmpname == name2)
				return ObjectArray[i];
		}
	}
	return 0;
}

std::vector<Object*> SBS::GetObjectRange(std::string expression)
{
	//get object by name range expression (ex. "Floors 1 to 3")

	std::vector<Object*> objects;
	int temp = (int)expression.find("to", 0);

	//the name 'elevator' matches the previous search - in this case, detect it and undo
	int temp2 = (int)expression.find("tor", 0);
	if (temp == temp2)
		temp = 0;

	std::string type;

	if (temp > 0)
	{
		if (expression.substr(0, 6) == "Floors")
			type = "floor";
		else if (expression.substr(0, 9) == "Elevators")
			type = "elevator";
		else if (expression.substr(0, 6) == "Shafts")
			type = "shaft";
		else if (expression.substr(0, 6) == "Stairwells")
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
		if (!IsNumeric(str1.c_str(), RangeL) || !IsNumeric(str2.c_str(), RangeH))
		{
			ReportError("GetObjectRange: Invalid range");
			return objects;
		}

		for (int i = 0; i < (int)ObjectArray.size(); i++)
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

bool SBS::RunAction(std::string name)
{
	//run action by name - will run multiple actions if the name is the same

	std::vector<Action*> actionlist = GetAction(name);

	bool result = true;
	for (int i = 0; i < (int)actionlist.size(); i++)
	{
		bool result2 = false;

		if (actionlist[i])
			result2 = actionlist[i]->DoAction();

		if (result2 == false)
			result = false;
	}
	return result;
}

bool SBS::RunAction(int index)
{
	//run action by index number

	Action *action = GetAction(index);
	if (action)
		return action->DoAction();
	return false;
}

void SBS::AddKey(int keyid, std::string name)
{
	//adds key 'keyid' to the user's keyring
	Key key;
	key.id = keyid;
	key.name = name;
	keys.push_back(key);

	std::string id = ToString(keyid);
	Report("Added key " + id + " (" + name + ") to keyring");
}

bool SBS::CheckKey(int keyid)
{
	//checks to see if the user has the specified key

	for (int i = 0; i < (int)keys.size(); i++)
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

	for (int i = 0; i < (int)keys.size(); i++)
	{
		std::string id = ToString(keys[i].id);
		Report(id + " - " + keys[i].name);
	}
	Report("");
}

void SBS::CameraLoop()
{
	unsigned long timing;
	if (SmoothFrames > 0)
		timing = GetAverageTime();
	else
		timing = GetElapsedTime();
	float elapsed = timing / 1000.0f;

	//Process camera loop
	camera->Loop(elapsed);
}

void SBS::RegisterControl(Control *control)
{
	//add control to index
	control_index.push_back(control);
}

void SBS::UnregisterControl(Control *control)
{
	//remove control from index

	for (int i = 0; i < (int)control_index.size(); i++)
	{
		if (control_index[i] == control)
		{
			control_index.erase(control_index.begin() + i);
			i--;
		}
	}
}

void SBS::ShowFloorInfo(bool all_floors, int floor)
{
	//show floor information for all floors, or specified floor

	if (all_floors == false)
	{
		Floor *flr = GetFloor(floor);

		if (!flr)
			return;

		Report("\n--- Floor Information ---\n");
		Report("Number: " + ToString2(floor));
		Report("ID: " + flr->ID);
		Report("Name: " + flr->Name);
		Report("Type: " + flr->FloorType);
		Report("Description: " + flr->Description);
		Report("Height: " + ToString2(flr->Height));
		Report("InterfloorHeight: " + ToString2(flr->InterfloorHeight));
		Report("FullHeight: " + ToString2(flr->FullHeight()));
		Report("Altitude: " + ToString2(flr->Altitude));
		Report("Base: " + ToString2(flr->GetBase()));
		Report("");
	}
	else
	{
		Report("\n--- Floor Information ---\n");
		Report("Number(ID)\t----\tName\t----\tType\t----\tHeight\t----\tIFloorHeight\t----\tAltitude\t----\tDescription");
		for (int i = -Basements; i < Floors; i++)
		{
			Floor *floor = GetFloor(i);
			Report(ToString2(i) + "(" + floor->ID + ")\t----\t" + floor->Name + "\t----\t" + floor->FloorType + "\t----\t" + ToString2(floor->Height) + "\t----\t" + ToString2(floor->InterfloorHeight) + "\t----\t" + ToString2(floor->Altitude) + "\t----\t" + floor->Description);
		}
		Report("");
	}
}
