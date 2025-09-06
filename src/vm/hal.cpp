/*
	Skyscraper 2.1 - Hardware Abstraction Layer
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

//OGRE interfaces
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreFontManager.h>
#include <OgreRectangle2D.h>
#include <OgreRTShaderSystem.h>
#include <OgreBitesConfigDialog.h>
#include <OgreSGTechniqueResolverListener.h>
#include <OgreOverlaySystem.h>

#ifndef DISABLE_SOUND
	//FMOD
	#include <fmod_errors.h>
#endif

//OpenXR interfaces
#if USING_OPENXR
#include "OgreOpenXRRenderWindow.h"
#endif

//Linux malloc support
#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
#include "malloc.h"
#endif

#include <iostream>

//simulator interfaces
#include "globals.h"
#include "sbs.h"
#include "vm.h"
#include "camera.h"
#include "scenenode.h"
#include "enginecontext.h"
#include "hal.h"
#include "gui.h"
#include "editor.h"
#include "profiler.h"

using namespace SBS;

namespace Skyscraper {

//Hardware Abstraction Layer

HAL::HAL(VM *vm)
{
    this->vm = vm;
	mOverlaySystem = 0;
	mRoot = 0;
	mRenderWindow = 0;
	mSceneMgr = 0;
	sound = 0;
	channel = 0;
	soundsys = 0;
	RTSS = false;
	mTrayMgr = 0;
	show_stats = -1;
	logger = 0;
	log = 0;
	DisableSound = false;
	configfile = 0;
	keyconfigfile = 0;
	joyconfigfile = 0;
	DX11 = false;
	timer = new Ogre::Timer();
}

HAL::~HAL()
{
#ifndef DISABLE_SOUND
	if (soundsys)
		soundsys->release();
#endif

	delete mOverlaySystem;

	Ogre::ResourceGroupManager::getSingleton().shutdownAll();

	if (configfile)
		delete configfile;
	configfile = 0;

	if (keyconfigfile)
		delete keyconfigfile;
	keyconfigfile = 0;

	if (joyconfigfile)
		delete joyconfigfile;
	joyconfigfile = 0;

	log->removeListener(this);

	//shutdown Ogre
#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE
	delete mRoot;
#endif

	delete logger;
}

void HAL::ClickedObject(bool left, bool shift, bool ctrl, bool alt, bool right, Real scale, bool center_only)
{
	//click on an object

	EngineContext *engine = vm->GetActiveEngine();

	if (!engine)
		return;

	//get SBS camera
	Camera *camera = engine->GetSystem()->camera;

	if (!camera)
		return;

	if (left == true)
		camera->MouseLeftDown = true;
	else
		camera->MouseRightDown = true;

	Real result = -1;
	Real nearest = 1000;
	Camera *nearest_cam = 0;

	for (int i = 0; i < vm->GetEngineCount(); i++)
	{
		//go through each engine and try a mouse click, find which hit is nearest
		EngineContext *engine = vm->GetEngine(i);
		if (engine)
		{
			Camera *camera2 = engine->GetSystem()->camera;
			if (camera2)
				result = camera2->ClickedObject(camera, shift, ctrl, alt, right, scale, center_only, true);

			if (result < nearest && result >= 0)
			{
				nearest = result;
				nearest_cam = camera2;
			}
		}
	}

	//perform a mouseclick on the nearest result
	if (nearest_cam)
		nearest_cam->ClickedObject(camera, shift, ctrl, alt, right, scale, center_only);
}

void HAL::UnclickedObject()
{
	//unclick the clicked object

	EngineContext *engine = vm->GetActiveEngine();

	if (!engine)
		return;

	//get SBS camera
	Camera *camera = engine->GetSystem()->camera;

	if (!camera)
		return;

	camera->UnclickedObject();
	camera->MouseLeftDown = false;
	camera->MouseRightDown = false;
}

void HAL::UpdateOpenXR()
{
#if USING_OPENXR
	SBS_PROFILE_MAIN("UpdateOpenXR");

	//update OpenXR camera transformations
	if (GetConfigBool(configfile, "Skyscraper.Frontend.VR", false) == true)
	{
		EngineContext* engine = vm->GetActiveEngine();

		if (engine)
		{
			::SBS::SBS* Simcore = engine->GetSystem();

			if (Simcore->camera)
			{
				for (int i = 0; i < 2; i++)
				{
					Ogre::Camera* camera = Simcore->camera->GetOgreCamera(i);
					Vector3 cameranode_pos = Simcore->camera->GetSceneNode()->GetPosition() - Simcore->camera->GetPosition();
					SetOpenXRParameters(i, Simcore->ToRemote(cameranode_pos), camera->getDerivedOrientation());
				}

				//update controllers

				//left controller for movement
				OpenXRControllerState leftState;
				if (GetControllerState(0, &leftState))
				{
					//std::string out = "Thumbstick: (" + ToString(leftState.joystickX) + ", " + ToString(leftState.joystickY) + ")";
					//Report(out);
					const float deadzone = 0.1f;
					float x = leftState.joystickX;
					float y = leftState.joystickY;

					if (std::abs(x) > deadzone || std::abs(y) > deadzone) {
						Ogre::Vector3 localMove(x, 0, -y);

						Ogre::Quaternion viewRot = Simcore->camera->GetOrientation();
						Ogre::Vector3 worldMove = viewRot * localMove;

						float walkSpeed = 3.0f; // units per second
						worldMove *= walkSpeed * Simcore->delta;

						Ogre::Vector3 currentPos = Simcore->camera->GetPosition();
						Simcore->camera->SetPosition(currentPos + worldMove);
					}
				}

				//right controller for rotation
				OpenXRControllerState rightState;
				if (GetControllerState(0, &rightState))
				{
					float turnRate = 1.0;
					float yawChange = rightState.joystickX * turnRate * Simcore->delta;
					Simcore->camera->Turn(yawChange);
				}
			}
		}
	}
#endif
}

void HAL::Report(const std::string &message, const std::string &prompt)
{
	std::string delim = "";
	if (prompt.size() > 0)
		delim = " ";
	try
	{
		if (Ogre::LogManager::getSingletonPtr())
			Ogre::LogManager::getSingleton().logMessage(prompt + delim + message);
	}
	catch (Ogre::Exception &e)
	{
#ifdef USING_WX
		vm->GetGUI()->ShowError("VM: Error writing message to log\n" + e.getDescription());
#endif
	}
}

bool HAL::ReportError(const std::string &message, const std::string &prompt)
{
	std::string delim = "";
	if (prompt.size() > 0)
		delim = " ";
	try
	{
		if (Ogre::LogManager::getSingletonPtr())
			Ogre::LogManager::getSingleton().logMessage(prompt + delim + message, Ogre::LML_CRITICAL);
	}
	catch (Ogre::Exception &e)
	{
#ifdef USING_WX
		vm->GetGUI()->ShowError("VM: Error writing message to log\n" + e.getDescription());
#endif
	}
	return false;
}

bool HAL::ReportFatalError(const std::string &message, const std::string &prompt)
{
	ReportError(message, prompt);
#ifdef USING_WX
	vm->GetGUI()->ShowError(message);
#endif
	return false;
}

Ogre::ConfigFile* HAL::ConfigLoad(const std::string &filename, bool delete_after_use)
{
	//load a config file
	try
	{
		Ogre::ConfigFile* file = new Ogre::ConfigFile();
		file->load(filename);
		if (delete_after_use == true)
		{
			delete file;
			file = 0;
		}
		return file;
	}
	catch (Ogre::Exception &e)
	{
		ReportFatalError("Error loading configuration files\nDetails: " + e.getDescription());
		return 0;
	}
}

int HAL::GetConfigInt(Ogre::ConfigFile *file, const std::string &key, int default_value)
{
	std::string result = file->getSetting(key, "", ToString(default_value));
	return ToInt(result);
}

std::string HAL::GetConfigString(Ogre::ConfigFile *file, const std::string &key, const std::string &default_value)
{
	if (!file)
		return "";
	return file->getSetting(key, "", default_value);
}

bool HAL::GetConfigBool(Ogre::ConfigFile *file, const std::string &key, bool default_value)
{
	if (!file)
		return false;
	std::string result = file->getSetting(key, "", BoolToString(default_value));
	return ToBool(result);
}

Real HAL::GetConfigFloat(Ogre::ConfigFile *file, const std::string &key, Real default_value)
{
	if (!file)
		return 0.0;
	std::string result = file->getSetting(key, "", ToString(default_value));
	return ToFloat(result);
}

bool HAL::Initialize(const std::string &data_path, Ogre::Root *root, Ogre::OverlaySystem *overlay)
{
	//initialize HAL system

	mRoot = root;

	//initialize OGRE
	try
	{
		if (!mRoot)
			mRoot = Ogre::Root::getSingletonPtr();
	}
	catch (Ogre::Exception &e)
	{
		return ReportFatalError("Error during initial OGRE check\nDetails: " + e.getDescription());
	}

	if(!mRoot)
	{
		try
		{
			//set up custom logger
			if (!logger)
			{
				logger = new Ogre::LogManager();
				log = logger->createLog(data_path + "skyscraper.log", true, !vm->showconsole, false);
				log->addListener(this);
			}

			//report on system startup
			Report("Skyscraper version " + vm->version_full + " starting...\n", "");

			//load OGRE
			Report("Loading OGRE...");
			mRoot = new Ogre::Root();
		}
		catch (Ogre::Exception &e)
		{
			return ReportFatalError("Error initializing OGRE\nDetails: " + e.getDescription());
		}
		catch (...)
		{
			return ReportFatalError("Error initializing OGRE");
		}
	}

	//set up overlay system
	try
	{
		Report("");
		Report("Loading Overlay System...");
		mOverlaySystem = overlay;
		if (!mOverlaySystem)
			mOverlaySystem = new Ogre::OverlaySystem();
	}
	catch (Ogre::Exception &e)
	{
		return ReportFatalError("Error creating overlay system\nDetails: " + e.getDescription());
	}

	//configure render system
	try
	{
		if(!mRoot->getRenderSystem())
		{
			//if no render systems are loaded, try to load previous config
			if(!mRoot->restoreConfig())
			{
				//show dialog if load failed
				mRoot->showConfigDialog(OgreBites::getNativeConfigDialog());
			}
		}
	}
	catch (Ogre::Exception &e)
	{
		return ReportFatalError("Error configuring render system\nDetails: " + e.getDescription());
	}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	//set rendersystem options
	Ogre::RenderSystem *rendersystem = mRoot->getRenderSystem();
	if (rendersystem)
	{
		Ogre::ConfigOptionMap CurrentRendererOptions = mRoot->getRenderSystem()->getConfigOptions();
		Ogre::ConfigOptionMap::iterator configItr = CurrentRendererOptions.begin();

		for (; configItr != CurrentRendererOptions.end(); configItr++)
		{
			if ((configItr)->first == "Floating-point mode")
			{
				//if using DirectX, prevent it from switching into single-point floating point mode
				rendersystem->setConfigOption("Floating-point mode", "Consistent");
				break;
			}

			if ((configItr)->first == "Auto hardware buffer management")
			{
				//fix black screen when resizing window using DirectX on Windows
				rendersystem->setConfigOption("Auto hardware buffer management", "Yes");
			}
		}
	}
#endif

	//initialize render window
	try
	{
		Report("");
		Report("Initializing OGRE...");
		mRoot->initialise(false);
	}
	catch (Ogre::Exception &e)
	{
		return ReportFatalError("Error initializing render window\nDetails: " + e.getDescription());
	}
	catch (...)
	{
		return ReportFatalError("Error initializing render window");
	}

	return true;
}

bool HAL::LoadSystem(const std::string &data_path, Ogre::RenderWindow *renderwindow)
{
	//load HAL system resources

	if (!renderwindow)
		return false;

	mRenderWindow = renderwindow;

	//get renderer info
	Renderer = mRoot->getRenderSystem()->getCapabilities()->getRenderSystemName();

	//shorten name
	size_t loc = Renderer.find("Rendering Subsystem");
	Renderer = Renderer.substr(0, loc - 1);

	//get graphics card device information
	GPUDevice = mRoot->getRenderSystem()->getCapabilities()->getDeviceName();

	//load resource configuration
	Ogre::ConfigFile cf;
	try
	{
		Report("");
		Report("Loading resources...");
		cf.load("resources.cfg");
	}
	catch (Ogre::Exception &e)
	{
		return ReportFatalError("Error loading resources.cfg\nDetails: " + e.getDescription());
	}

	//add resource locations
	try
	{
		std::string name, locationType;
		Ogre::ConfigFile::SettingsBySection_ settingsBySection = cf.getSettingsBySection();
		for (const auto& p : settingsBySection) {
			for (const auto& r : p.second) {
				locationType = r.first;
				name = r.second;
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, locationType);
			}
		}

		//add app's directory to resource manager
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("General");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(".", "FileSystem", "General", true);
		if (data_path != "")
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(data_path, "FileSystem", "General", true);

		//add materials group, and autoload
#ifdef USING_WX
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("data/materials", "FileSystem", "Materials", true);
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Materials");

		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("media/packs/SdkTrays.zip", "Zip", "Trays", true);
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Trays");
#endif
	}
	catch (Ogre::Exception &e)
	{
		return ReportFatalError("Error initializing resources\nDetails: " + e.getDescription());
	}

	//create scene manager
	try
	{
		mSceneMgr = mRoot->createSceneManager();
	}
	catch (Ogre::Exception &e)
	{
		return ReportFatalError("Error creating scene manager\nDetails: " + e.getDescription());
	}

	mSceneMgr->addRenderQueueListener(mOverlaySystem);

	//enable shadows
	if (GetConfigBool(configfile, "Skyscraper.Frontend.Shadows", false) == true)
	{
		try
		{
			Report("Enabling shadows");
			mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
		}
		catch (Ogre::Exception &e)
		{
			ReportFatalError("Error setting shadow technique\nDetails: " + e.getDescription());
		}
	}

	std::string renderer = mRoot->getRenderSystem()->getName();

	if (renderer != "Direct3D9 Rendering Subsystem" && renderer != "OpenGL Rendering Subsystem" && renderer != "Metal Rendering Subsystem")
		RTSS = true;

	if (RTSS == true)
	{
		//Enable the RT Shader System
		if (Ogre::RTShader::ShaderGenerator::initialize())
		{
			Ogre::RTShader::ShaderGenerator* shaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
			shaderGenerator->addSceneManager(mSceneMgr);

			// forward scheme not found events to the RTSS
			OgreBites::SGTechniqueResolverListener* schemeNotFoundHandler = new OgreBites::SGTechniqueResolverListener(shaderGenerator);
			Ogre::MaterialManager::getSingleton().addListener(schemeNotFoundHandler);

			//uncomment this to dump RTSS shaders
			//shaderGenerator->setShaderCachePath("shaders/");
		}
	}

	if (renderer == "Direct3D11 Rendering Subsystem")
		DX11 = true;

	try
	{
		//define camera configuration
		int cameras = 1; //use one camera for standard mode
		if (GetConfigBool(configfile, "Skyscraper.Frontend.VR", false) == true)
			cameras = 2; //use two cameras for VR mode

		for (int i = 0; i < cameras; i++)
		{
			mCameras.emplace_back(mSceneMgr->createCamera("Camera " + ToString(i + 1)));
			if (mRenderWindow)
			{
				Ogre::Viewport* viewport = mRenderWindow->addViewport(mCameras[i], (cameras - 1) - i, 0, 0, 1, 1);
				viewport->setOverlaysEnabled(true);
				mViewports.emplace_back(viewport);
				mCameras[i]->setAspectRatio(Real(mViewports[i]->getActualWidth()) / Real(mViewports[i]->getActualHeight()));
			}
		}
	}
	catch (Ogre::Exception &e)
	{
		return ReportFatalError("Error creating camera and viewport\nDetails: " + e.getDescription());
	}

	//set up default material shader scheme
	if (RTSS == true)
	{
		for (size_t i = 0; i < mViewports.size(); i++)
		{
			mViewports[i]->setMaterialScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
		}
	}

	//setup texture filtering
	int filtermode = GetConfigInt(configfile, "Skyscraper.Frontend.TextureFilter", 3);
	int maxanisotropy = GetConfigInt(configfile, "Skyscraper.Frontend.MaxAnisotropy", 8);

	if (filtermode < 0 || filtermode > 3)
		filtermode = 3;

	if (filtermode < 3)
		maxanisotropy = 1;

	Ogre::TextureFilterOptions filter = Ogre::TFO_NONE;
	if (filtermode == 1)
		filter = Ogre::TFO_BILINEAR;
	else if (filtermode == 2)
		filter = Ogre::TFO_TRILINEAR;
	else if (filtermode == 3)
		filter = Ogre::TFO_ANISOTROPIC;

	Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(filter);
	Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(maxanisotropy);

#ifndef DISABLE_SOUND
	//initialize FMOD (sound)
	DisableSound = GetConfigBool(configfile, "Skyscraper.Frontend.DisableSound", false);
	if (DisableSound == false)
	{
		Report("");
		Report("FMOD Sound System, copyright (C) Firelight Technologies Pty, Ltd., 1994-2024\n");

		FMOD_RESULT result = FMOD::System_Create(&soundsys);
		if (result != FMOD_OK)
		{
			std::string fmod_result = FMOD_ErrorString(result);
			ReportFatalError("Error initializing sound:\n" + fmod_result);
			DisableSound = true;
		}
		else
		{
			char name [] = "Skyscraper"; //set name for PulseAudio on Linux
			result = soundsys->init(100, FMOD_INIT_NORMAL, &name);
			if (result != FMOD_OK)
			{
				std::string fmod_result = FMOD_ErrorString(result);
				ReportFatalError("Error initializing sound:\n" + fmod_result);
				DisableSound = true;
			}
			else
			{
				//get FMOD version information
				unsigned int version;
				soundsys->getVersion(&version);
				int major = version >> 16;
				int minor = (version >> 8) & 255;
				int rev = version & 255;

				std::string s_version;
				char hexString[25];

				snprintf(hexString, 25, "%x.%x.%x", major, minor, rev);
				s_version = std::string(hexString);

				Report("Sound initialized: FMOD Engine version " + s_version);
			}
		}
	}
	else
#endif
		Report("Sound Disabled");

	try
	{
		mTrayMgr = new OgreBites::TrayManager("InterfaceName", mRenderWindow);
	}
	catch (Ogre::Exception &e)
	{
		ReportFatalError("Error starting tray manager:\nDetails: " + e.getDescription());
	}

	if (mTrayMgr)
	{
		mTrayMgr->hideCursor();
	}

	//initialize editor
	vm->GetEditor()->Initialize();

	//report hardware concurrency
	int c = std::thread::hardware_concurrency();
	Report("Reported hardware concurrency: " + ToString(c) + "\n");

	Report("Initialization complete");
	Report("");

	return true;
}

bool HAL::Render()
{
	SBS_PROFILE_MAIN("Render");

	//process editor
	vm->GetEditor()->Run();

	//render to the frame buffer
	try
	{
		mRoot->renderOneFrame();
	}
	catch (Ogre::Exception &e)
	{
		return ReportFatalError("Error in render operation\nDetails: " + e.getDescription());
	}

	//update frame statistics
	Ogre::FrameEvent a;
	if (mTrayMgr)
		mTrayMgr->frameRendered(a);

	return true;
}

bool HAL::PlaySound(const std::string &filename, Real volume)
{
	//play a sound
#ifndef DISABLE_SOUND

	//load new sound
#if (FMOD_VERSION >> 16 == 4)
		FMOD_RESULT result = soundsys->createSound(filename.c_str(), (FMOD_MODE)(FMOD_2D | FMOD_ACCURATETIME | FMOD_SOFTWARE | FMOD_LOOP_NORMAL), 0, &sound);
#else
		FMOD_RESULT result = soundsys->createSound(filename.c_str(), (FMOD_MODE)(FMOD_2D | FMOD_ACCURATETIME | FMOD_LOOP_NORMAL), 0, &sound);
#endif
	if (result != FMOD_OK)
	{
		return ReportError("Can't load file '" + filename + "':\n" + FMOD_ErrorString(result));
	}

#if (FMOD_VERSION >> 16 == 4)
	result = soundsys->playSound(FMOD_CHANNEL_FREE, sound, true, &channel);
#else
	result = soundsys->playSound(sound, 0, true, &channel);
#endif

	if (result != FMOD_OK)
	{
		return ReportError("Error playing " + filename);
	}

	channel->setLoopCount(-1);
	channel->setVolume(volume);
	channel->setPaused(false);

#endif
	return true;
}

void HAL::StopSound()
{
	//stop and unload sound
#ifndef DISABLE_SOUND
	if (channel)
		channel->stop();
	if (sound)
		sound->release();
	sound = 0;
#endif
}

void HAL::ClearScene()
{
	//clear scene
	if (mSceneMgr)
		mSceneMgr->clearScene();
}

void HAL::ToggleStats()
{
	//toggle frame statistics

	show_stats++;

	if (!mTrayMgr)
		return;

	if (show_stats == 0)
	{
		mTrayMgr->showFrameStats(OgreBites::TrayLocation::TL_TOPRIGHT);
		mTrayMgr->toggleAdvancedFrameStats();
	}
	else if (show_stats == 1)
		mTrayMgr->toggleAdvancedFrameStats();
	else if (show_stats == 2)
	{
		mTrayMgr->hideFrameStats();
		show_stats = -1;
	}
}

void HAL::EnableStats(bool value)
{
	//turn on or off frame statistics

	if (value == true)
	{
		show_stats = -1;
		ToggleStats();
	}
	else
	{
		show_stats = 1;
		ToggleStats();
	}
}

void HAL::ReInit()
{
	//reinitialize HAL system

	EnableStats(false);

	delete mTrayMgr;
	mTrayMgr = 0;

	//unload editor interface
	vm->GetEditor()->Unload();

	//reinit overlay system
	try
	{
		mSceneMgr->removeRenderQueueListener(mOverlaySystem);
		delete mOverlaySystem;
		mOverlaySystem = new Ogre::OverlaySystem();
		mSceneMgr->addRenderQueueListener(mOverlaySystem);
	}
	catch (Ogre::Exception &e)
	{
		ReportFatalError("Error creating overlay system\nDetails: " + e.getDescription());
	}

	//initialize system resources
	try
	{
		Ogre::ResourceGroupManager::getSingleton().clearResourceGroup("Materials");
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Materials");
		Ogre::ResourceGroupManager::getSingleton().clearResourceGroup("Trays");
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Trays");
	}
	catch (Ogre::Exception &e)
	{
		ReportFatalError("Error initializing resources\nDetails:" + e.getDescription());
	}

	//reinit tray manager
	try
	{
		mTrayMgr = new OgreBites::TrayManager("Tray", mRenderWindow);
	}
	catch (Ogre::Exception &e)
	{
		ReportFatalError("Error starting tray manager:\n" + e.getDescription());
	}

	if (mTrayMgr)
	{
		mTrayMgr->hideCursor();
	}

	show_stats = -1;
}

FMOD::System* HAL::GetSoundSystem()
{
	return soundsys;
}

Ogre::RenderWindow* HAL::GetRenderWindow()
{
	return mRenderWindow;
}

void HAL::Clear()
{
	//do a full clear of Ogre objects

	//remove all meshes
	Ogre::MeshManager::getSingleton().removeAll();

	//remove all materials
	if (RTSS)
		Ogre::RTShader::ShaderGenerator::getSingleton().removeAllShaderBasedTechniques();
	Ogre::MaterialManager::getSingleton().removeAll();
	Ogre::MaterialManager::getSingleton().initialise();  //restore default materials

	//remove all fonts
	Ogre::FontManager::getSingleton().removeAll();

	//remove all textures
	Ogre::TextureManager::getSingleton().removeAll();

	//clear scene manager
	if (mSceneMgr)
		mSceneMgr->clearScene();

	//free unused hardware buffers
	Ogre::HardwareBufferManager::getSingleton()._freeUnusedBufferCopies();

#ifndef DISABLE_SOUND
	//reset FMOD reverb
	soundsys->setReverbProperties(0, 0);
#endif

	ReInit();

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
#ifdef __linux__
	//release free memory to OS on Linux
	malloc_trim(0);
#else
	//freebsd
#endif
#endif
}

Ogre::SceneManager* HAL::GetSceneManager()
{
	return mSceneMgr;
}

Ogre::RenderWindow* HAL::CreateRenderWindow(const std::string &name, int width, int height, const Ogre::NameValuePairList &params)
{
	//create the render window
#if USING_OPENXR
	if (GetConfigBool(configfile, "Skyscraper.Frontend.VR", false) == true)
	{
		if (mRoot->getRenderSystem()->getName() != "Direct3D11 Rendering Subsystem")
		{
			ReportFatalError("VR mode requires the DirectX 11 renderer\nDelete ogre.cfg and re-run Skyscraper");
			return 0;
		}
		Ogre::RenderWindow* win2 = Ogre::Root::getSingleton().createRenderWindow(name, width, height, false, &params);
		mRenderWindow = CreateOpenXRRenderWindow(mRoot->getRenderSystem());
		mRenderWindow->create(name, width, height, false, &params);
	}
	else
#endif
		mRenderWindow = Ogre::Root::getSingleton().createRenderWindow(name, width, height, false, &params);

	mRenderWindow->setActive(true);
	mRenderWindow->windowMovedOrResized();

	return mRenderWindow;
}

void HAL::DestroyRenderWindow()
{
	if (mRenderWindow)
	   Ogre::Root::getSingleton().detachRenderTarget(mRenderWindow);

	mRenderWindow->destroy();
	mRenderWindow = 0;
}

void HAL::RefreshViewport()
{
	//refresh viewport to prevent rendering issues

	for (size_t i = 0; i < mViewports.size(); i++)
	{
		mViewports[i]->_updateDimensions();
	}
}

void HAL::Report(const std::string &message)
{
	Report(message, "hal:");
}

bool HAL::ReportError(const std::string &message)
{
	return ReportError(message, "hal:");
}

bool HAL::ReportFatalError(const std::string &message)
{
	return ReportFatalError(message, "hal:");
}

void HAL::LoadConfiguration(const std::string &data_path, bool show_console)
{
	//load configuration files

	configfile = ConfigLoad(data_path + "skyscraper.ini");
	keyconfigfile = ConfigLoad(data_path + "keyboard.ini");
	joyconfigfile = ConfigLoad(data_path + "joystick.ini");
	ConfigLoad("plugins.cfg", true);
	ConfigLoad("resources.cfg", true);

	if (show_console == false)
		return;

	vm->showconsole = GetConfigBool(configfile, "Skyscraper.Frontend.ShowConsole", true);

	//create console window
#ifdef USING_WX
	if (vm->showconsole == true)
		vm->GetGUI()->ShowConsole(false);
#endif
}

void HAL::messageLogged(const std::string &message, Ogre::LogMessageLevel lml, bool maskDebug, const std::string &logName, bool &skipThisMessage)
{
	//callback function that receives OGRE log messages

#ifdef USING_WX
	vm->GetGUI()->WriteToConsole(message);
#endif
}

void HAL::ConsoleOut(const std::string &message, const std::string &color)
{
	//console output
	std::string mod = GetColors(color);
	std::string reset = GetColors("reset");
	std::cout << mod << message << reset;
}

std::string HAL::GetColors(const std::string &color)
{
	//get colors
	std::string mod;
	if (color == "blue")
		mod = "\033[1;34m";
	else if (color == "green")
		mod = "\033[1;32m";
	else if (color == "yellow")
		mod = "\033[1;33m";
	else if (color == "red")
		mod = "\033[1;31m";
	else if (color == "magenta")
		mod = "\033[1;35m";
	else if (color == "cyan")
		mod = "\033[1;36m";
	else if (color == "white")
		mod = "\033[1;37m";
	else if (color == "black")
		mod = "\033[1;30m";
	else if (color == "reset")
		mod = "\033[0m";

	return mod;
}

unsigned long HAL::GetCurrentTime()
{
	//get current time
	return timer->getMilliseconds();
}

bool HAL::IsVREnabled()
{
	return GetConfigBool(configfile, "Skyscraper.Frontend.VR", false);
}

}
