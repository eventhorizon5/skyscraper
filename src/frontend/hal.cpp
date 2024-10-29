/*
	Skyscraper 2.0 - Hardware Abstraction Layer
	Copyright (C)2004-2024 Ryan Thoryk
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

//FMOD
#include <fmod_errors.h>

//OpenXR interfaces
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "OgreOpenXRRenderWindow.h"
#endif
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "OgreOpenXRRenderWindow.h"
#endif

//Linux malloc support
#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
#include "malloc.h"
#endif

//simulator interfaces
#include "globals.h"
#include "sbs.h"
#include "vm.h"
#include "skyscraper.h"
#include "mainscreen.h"
#include "camera.h"
#include "scenenode.h"
#include "enginecontext.h"
#include "hal.h"
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
	DisableSound = false;
	configfile = 0;
	keyconfigfile = 0;
	joyconfigfile = 0;
}

HAL::~HAL()
{
	if (soundsys)
		soundsys->release();

	delete mOverlaySystem;

	Ogre::ResourceGroupManager::getSingleton().shutdownAll();

#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE
	mRoot->shutdown(); //shutdown root instead of delete, to fix a crash on certain systems
	//delete mRoot;
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
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	//update OpenXR camera transformations
	if (GetConfigBool(vm->GetFrontend()->configfile, "Skyscraper.Frontend.VR", false) == true)
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
		vm->GetFrontend()->ShowError("VM: Error writing message to log\n" + e.getDescription());
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
		vm->GetFrontend()->ShowError("VM: Error writing message to log\n" + e.getDescription());
	}
	return false;
}

bool HAL::ReportFatalError(const std::string &message, const std::string &prompt)
{
	ReportError(message, prompt);
	vm->GetFrontend()->ShowError(message);
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

bool HAL::Initialize(const std::string &data_path)
{
	//initialize OGRE
	try
	{
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
				Ogre::Log *log = logger->createLog(data_path + "skyscraper.log", true, !vm->GetFrontend()->showconsole, false);
				log->addListener(vm->GetFrontend());
			}

			//report on system startup
			Report("Skyscraper version " + vm->GetFrontend()->version_frontend + " starting...\n", "");

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

		if (vm->GetFrontend()->Headless == false)
		{
			Report("");
			Report("Creating render window...");
			mRenderWindow = vm->GetFrontend()->CreateRenderWindow(0, "SkyscraperVR");
		}
	}
	catch (Ogre::Exception &e)
	{
		return ReportFatalError("Error initializing render window\nDetails: " + e.getDescription());
	}
	catch (...)
	{
		return ReportFatalError("Error initializing render window");
	}

	if (vm->GetFrontend()->Headless == false)
	{
		//get renderer info
		Renderer = mRoot->getRenderSystem()->getCapabilities()->getRenderSystemName();

		//shorten name
		int loc = Renderer.find("Rendering Subsystem");
		Renderer = Renderer.substr(0, loc - 1);
	}

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
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("data/materials", "FileSystem", "Materials", true);
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Materials");

		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("media/packs/SdkTrays.zip", "Zip", "Trays", true);
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Trays");
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
	if (GetConfigBool(vm->GetFrontend()->configfile, "Skyscraper.Frontend.Shadows", false) == true)
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

	if (vm->GetFrontend()->Headless == false)
	{
		try
		{
			//define camera configuration
			int cameras = 1; //use one camera for standard mode
			if (GetConfigBool(vm->GetFrontend()->configfile, "Skyscraper.Frontend.VR", false) == true)
				cameras = 2; //use two cameras for VR mode

			for (int i = 0; i < cameras; i++)
			{
				mCameras.push_back(mSceneMgr->createCamera("Camera " + ToString(i + 1)));
				mViewports.push_back(mRenderWindow->addViewport(mCameras[i], (cameras - 1) - i, 0, 0, 1, 1));
				mCameras[i]->setAspectRatio(Real(mViewports[i]->getActualWidth()) / Real(mViewports[i]->getActualHeight()));
			}
		}
		catch (Ogre::Exception &e)
		{
			return ReportFatalError("Error creating camera and viewport\nDetails: " + e.getDescription());
		}
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
	int filtermode = GetConfigInt(vm->GetFrontend()->configfile, "Skyscraper.Frontend.TextureFilter", 3);
	int maxanisotropy = GetConfigInt(vm->GetFrontend()->configfile, "Skyscraper.Frontend.MaxAnisotropy", 8);

	if (filtermode < 0 || filtermode > 3)
		filtermode = 3;

	if (filtermode < 3)
		maxanisotropy = 1;

	Ogre::TextureFilterOptions filter;
	if (filtermode == 0)
		filter = Ogre::TFO_NONE;
	else if (filtermode == 1)
		filter = Ogre::TFO_BILINEAR;
	else if (filtermode == 2)
		filter = Ogre::TFO_TRILINEAR;
	else if (filtermode == 3)
		filter = Ogre::TFO_ANISOTROPIC;

	Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(filter);
	Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(maxanisotropy);

	//initialize FMOD (sound)
	DisableSound = GetConfigBool(vm->GetFrontend()->configfile, "Skyscraper.Frontend.DisableSound", false);
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

	if (vm->GetFrontend()->Headless == true)
		return true;

	// Render to the frame buffer
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

bool HAL::PlaySound(const std::string &filename)
{
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
	channel->setVolume(1.0);
	channel->setPaused(false);

	return true;
}

void HAL::StopSound()
{
	//stop and unload sound
	if (channel)
		channel->stop();
	if (sound)
		sound->release();
	sound = 0;
}

void HAL::ClearScene()
{
	//clear scene
	mSceneMgr->clearScene();
}

void HAL::ToggleStats()
{
	show_stats++;

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
	EnableStats(false);

	delete mTrayMgr;
	mTrayMgr = 0;

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
	mSceneMgr->clearScene();

	//free unused hardware buffers
	Ogre::HardwareBufferManager::getSingleton()._freeUnusedBufferCopies();

	ReInit();

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
	//release free memory to OS on Linux
	malloc_trim(0);
#endif
}

Ogre::SceneManager* HAL::GetSceneManager()
{
	return mSceneMgr;
}

Ogre::RenderWindow* HAL::CreateRenderWindow(const std::string &name, int width, int height, const Ogre::NameValuePairList &params)
{
	//create the render window
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	if (GetConfigBool(vm->GetFrontend()->configfile, "Skyscraper.Frontend.VR", false) == true)
	{
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

	if (vm->GetFrontend()->Headless == false)
	{
		for (size_t i = 0; i < mViewports.size(); i++)
		{
			mViewports[i]->_updateDimensions();
		}
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

void HAL::LoadConfiguration(const std::string data_path)
{
	//load configuration files

	configfile = ConfigLoad(data_path + "skyscraper.ini");
	keyconfigfile = ConfigLoad(data_path + "keyboard.ini");
	joyconfigfile = ConfigLoad(data_path + "joystick.ini");
	ConfigLoad("plugins.cfg", true);
	ConfigLoad("resources.cfg", true);
}

}
