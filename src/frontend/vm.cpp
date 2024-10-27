/*
	Skyscraper 2.0 - Nanokernel
	Copyright (C)2003-2024 Ryan Thoryk
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
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreFontManager.h>
#include <OgreRectangle2D.h>
#include <OgreRTShaderSystem.h>
#include <OgreBitesConfigDialog.h>
#include <OgreSGTechniqueResolverListener.h>
#include <OgreOverlaySystem.h>
#include <fmod.hpp>
#include <fmod_errors.h>
#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
#include "malloc.h"
#endif
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "OgreOpenXRRenderWindow.h"
#endif
#include "Caelum.h"
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "OgreOpenXRRenderWindow.h"
#endif
#include "globals.h"
#include "sbs.h"
#include "vm.h"
#include "skyscraper.h"
#include "mainscreen.h"
#include "camera.h"
#include "scenenode.h"
#include "enginecontext.h"
#include "profiler.h"

using namespace SBS;

namespace Skyscraper {

//Virtual Manager system

VM::VM(Skyscraper *frontend)
{
	this->frontend = frontend;
	active_engine = 0;
	Shutdown = false;
	ConcurrentLoads = false;
	RenderOnStartup = false;
	CheckScript = false;
	Pause = false;
	mOverlaySystem = 0;
	mRoot = 0;
	mRenderWindow = 0;
	mSceneMgr = 0;
	sound = 0;
	channel = 0;
	SkyMult = 0;
	mCaelumSystem = 0;
	logger = 0;
	soundsys = 0;
	new_location = false;
	new_time = false;
	latitude = 0.0;
	longitude = 0.0;
	datetime = 0.0;
	CutLandscape = true;
	CutBuildings = true;
	CutExternal = false;
	CutFloors = false;
	RTSS = false;
	sky_error = 0;
	mTrayMgr = 0;
	show_stats = -1;
	first_run = true;
	DisableSound = false;

	Report("Started", "VM>");
}

VM::~VM()
{
	//delete Caelum
	if (mCaelumSystem)
		delete mCaelumSystem;

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

EngineContext* VM::CreateEngine(EngineContext *parent, const Vector3 &position, Real rotation, const Vector3 &area_min, const Vector3 &area_max)
{
	EngineContext* engine = new EngineContext(parent, this, mSceneMgr, GetSoundSystem(), position, rotation, area_min, area_max);
	return engine;
}

bool VM::DeleteEngine(EngineContext *engine)
{
	//delete a specified sim engine instance

	if (!engine)
		return false;

	for (size_t i = 0; i < engines.size(); i++)
	{
		if (engines[i] == engine)
		{
			engines[i] = 0;
			delete engine;
			Report("Engine instance " + ToString(i) + " deleted", "VM>");

			int count = GetEngineCount();

			if (active_engine == engine)
			{
				active_engine = 0;
				if (count > 0)
				{
					int number = GetFirstValidEngine()->GetNumber();
					SetActiveEngine(number);
				}
			}
			else if (active_engine)
				active_engine->RefreshCamera();

			//exit to main menu if all engines have been deleted
			if (count == 0)
				Shutdown = true;

			return true;
		}
	}
	return false;
}

void VM::DeleteEngines()
{
	//delete all sim emgine instances

	Report("Deleting all engines...", "VM>");
	for (size_t i = 0; i < engines.size(); i++)
	{
		if (engines[i])
			delete engines[i];
	}
	engines.clear();
	active_engine = 0;
}

EngineContext* VM::FindActiveEngine()
{
	//find engine instance with an active camera

	for (size_t i = 0; i < engines.size(); i++)
	{
		if (engines[i])
		{
			if (engines[i]->IsCameraActive() == true)
				return engines[i];
		}
	}
	return active_engine;
}

void VM::SetActiveEngine(int number, bool switch_engines)
{
	//set an engine instance to be active

	EngineContext *engine = GetEngine(number);

	if (!engine)
		return;

	if (active_engine == engine)
		return;

	//don't switch if the camera's already active in the specified engine
	if (engine->IsCameraActive() == true)
		return;

	//don't switch to engine if it's loading
	if (engine->IsLoading() == true)
		return;

	CameraState state;
	bool state_set = false;

	if (active_engine)
	{
		//get previous engine's camera state
		if (switch_engines == true)
		{
			state = active_engine->GetCameraState();
			state_set = true;
		}

		//detach camera from current engine
		active_engine->DetachCamera(switch_engines);
	}

	Report("Setting engine " + ToString(number) + " as active", "VM>");

	//switch context to new engine instance
	active_engine = engine;
	active_engine->AttachCamera(mCameras, !switch_engines);

	//apply camera state to new engine
	if (switch_engines == true && state_set == true)
		active_engine->SetCameraState(state, false);

	//update mouse cursor for freelook mode
	frontend->GetWindow()->EnableFreelook(active_engine->GetSystem()->camera->Freelook);
}

bool VM::RunEngines()
{
	bool result = true;
	bool isloading = IsEngineLoading();

	if (ConcurrentLoads == true && isloading == true)
		RefreshViewport();

	for (size_t i = 0; i < engines.size(); i++)
	{
		if (!engines[i])
			continue;

		//process engine run loops, and also prevent other instances from running if
		//one or more engines are loading
		if (ConcurrentLoads == true || isloading == false || engines[i]->IsLoading() == true)
		{
			bool run = true;
			if (i > 0 && ConcurrentLoads == false)
			{
				//if concurrent loads is off, skip running if previous engine is not finished loading
				if (engines[i - 1])
				{
					if (engines[i - 1]->IsLoading() == true && engines[i - 1]->IsLoadingFinished() == false)
						run = false;
				}
			}

			if (engines[i]->IsLoadingFinished() == false && run == true)
			{
				if (engines[i]->Run() == false)
					result = false;
			}
		}

		//start engine if loading is finished
		if (engines[i]->IsLoadingFinished() == true)
		{
			if (active_engine)
			{
				//exit if active engine is still loading
				if (active_engine->IsLoading() == true && active_engine->IsLoadingFinished() == false)
					continue;

				//exit if active engine is finished, but other buildings are still loading
				if (active_engine->IsLoadingFinished() == true && isloading == true)
					continue;
			}
			frontend->Start(engines[i]);
		}
	}
	return result;
}

bool VM::IsEngineLoading()
{
	//return true if an engine is loading

	bool result = false;
	for (size_t i = 0; i < engines.size(); i++)
	{
		if (engines[i])
		{
			if (engines[i]->IsLoading() == true && engines[i]->IsLoadingFinished() == false)
				result = true;
		}
	}
	return result;
}

void VM::HandleEngineShutdown()
{
	//shutdown an engine if requested

	bool deleted = false;

	for (size_t i = 0; i < engines.size(); i++)
	{
		if (engines[i])
		{
			if (engines[i]->GetShutdownState() == true)
			{
				Report("Shutdown requested for engine instance " + ToString(i), "VM>");

				if (DeleteEngine(engines[i]) == true)
				{
					RefreshViewport();
					i--;
					deleted = true;
				}
			}
		}
	}

	//clean up empty engine slots at the end of the list
	if (deleted == true)
	{
		for (size_t i = engines.size() - 1; i < engines.size(); --i)
		{
			if (!engines[i])
				engines.erase(engines.begin() + i);
			else
				break;
		}
	}
}

void VM::HandleReload()
{
	//reload building if requested

	for (size_t i = 0; i < engines.size(); i++)
	{
		if (engines[i])
		{
			if (engines[i]->Reload == true)
			{
				//unload sky system if primary engine
				if (i == 0)
					UnloadSky();

				Pause = false;
				Report("Reloading engine instance " + ToString(i), "VM>");

				engines[i]->DoReload(); //handle engine reload

				//create sky system if primary engine
				if (i == 0)
					CreateSky(engines[i]);
			}
		}
	}
}

EngineContext* VM::GetEngine(int number)
{
	//get an engine by instance number

	if (number < 0 || number >= (int)engines.size())
		return 0;

	return engines[number];
}

int VM::GetEngineCount()
{
	//get number of valid engines

	int count = 0;

	for (size_t i = 0; i < engines.size(); i++)
	{
		if (engines[i])
			count++;
	}
	return count;
}

void VM::SwitchEngines()
{
	//if user is no longer inside the active engine, find a new engine to attach to

	if (!active_engine)
		return;

	if (active_engine->IsInside() == true)
		return;

	EngineContext *parent = active_engine->GetParent();

	//if active engine has a parent, switch to the parent if possible
	if (parent)
	{
		if (parent->IsInside() == true && parent->IsCameraActive() == false)
		{
			SetActiveEngine(parent->GetNumber(), true);
			return;
		}
	}

	//otherwise search for a valid engine to attach to
	Report("Searing for engine to attach to...", "VM>");
	for (size_t i = 0; i < engines.size(); i++)
	{
		if (engines[i] != active_engine && engines[i])
		{
			if (engines[i]->IsInside() == true && engines[i]->IsCameraActive() == false)
			{
				SetActiveEngine((int)i, true);
				return;
			}
		}
	}

	//if user has stepped outside of all sim engines, revert the movement
	//to place them back into the active engine
	active_engine->RevertMovement();
}

bool VM::IsValidEngine(EngineContext *engine)
{
	if (!engine)
		return false;

	for (size_t i = 0; i < engines.size(); i++)
	{
		if (engines[i] == engine)
			return true;
	}
	return false;
}

bool VM::IsValidSystem(::SBS::SBS *sbs)
{
	if (!sbs)
		return false;

	for (size_t i = 0; i < engines.size(); i++)
	{
		if (engines[i])
		{
			if (engines[i]->GetSystem() == sbs)
				return true;
		}
	}
	return false;
}

int VM::GetFreeInstanceNumber()
{
	//get an available engine instance number

	for (size_t i = 0; i < engines.size(); i++)
	{
		if (!engines[i])
			return (int)i;
	}
	return (int)engines.size();
}

int VM::RegisterEngine(EngineContext *engine)
{
	//register an engine, returning the assigned instance number

	int number = GetFreeInstanceNumber();

	if (number < (int)engines.size())
		engines[number] = engine;
	else
		engines.push_back(engine);

	return number;
}

EngineContext* VM::GetFirstValidEngine()
{
	for (size_t i = 0; i < engines.size(); i++)
	{
		if (engines[i])
			return engines[i];
	}
	return 0;
}

Skyscraper* VM::GetFrontend()
{
	return frontend;
}

void VM::CheckCamera()
{
	if (active_engine->IsCameraActive() == false)
		active_engine = FindActiveEngine();

}

bool VM::StartEngine(EngineContext* engine, std::vector<Ogre::Camera*> &cameras)
{
	//start a sim engine

	Report("Initiating engine start", "VM>");
	return engine->Start(cameras);
}

::SBS::SBS* VM::GetActiveSystem()
{
	if (active_engine)
		return active_engine->GetSystem();
	return 0;
}

ScriptProcessor* VM::GetActiveScriptProcessor()
{
	if (active_engine)
		return active_engine->GetScriptProcessor();
	return 0;
}

void VM::ClickedObject(bool left, bool shift, bool ctrl, bool alt, bool right, Real scale, bool center_only)
{
	//click on an object

	EngineContext *engine = GetActiveEngine();

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

	for (int i = 0; i < GetEngineCount(); i++)
	{
		//go through each engine and try a mouse click, find which hit is nearest
		EngineContext *engine = GetEngine(i);
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

void VM::UnclickedObject()
{
	EngineContext *engine = GetActiveEngine();

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

void VM::Run()
{
	//run system

	//run sim engines
	bool result = RunEngines();

	//delete an engine if requested
	HandleEngineShutdown();

	//exit if full shutdown request received
	if (Shutdown == true)
	{
		Shutdown = false;
		Report("Unloading due to shutdown request", "VM>");
		frontend->UnloadToMenu();
	}

	if (result == false && (ConcurrentLoads == false || GetEngineCount() == 1))
		return;

	if (!GetActiveEngine())
		return;

	//make sure active engine is the one the camera is active in
	CheckCamera();

	//exit if any engine is loading, unless RenderOnStartup is true
	if (IsEngineLoading() == true && RenderOnStartup == false)
		return;

	//if in CheckScript mode, exit
	if (CheckScript == true)
	{
		Report("Unloading to menu...", "VM>");
		frontend->UnloadToMenu();
		return;
	}

	//update Caelum
	UpdateSky();

	//update OpenXR
	UpdateOpenXR();

	//render graphics
	result = Render();
	if (!result)
		return;

	//handle a building reload
	HandleReload();

	//handle behavior when a user exits an engine area
	SwitchEngines();

	//update first run status
	if (first_run == true)
		first_run = false;
}

void VM::UpdateOpenXR()
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	//update OpenXR camera transformations
	if (GetConfigBool(frontend->configfile, "Skyscraper.Frontend.VR", false) == true)
	{
		EngineContext* engine = GetActiveEngine();

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

bool VM::Load(const std::string &filename, EngineContext *parent, const Vector3 &position, Real rotation, const Vector3 &area_min, const Vector3 &area_max)
{
	//load simulator and data file

	Report("Loading engine for building file '" + filename + "'...", "VM>");

	//set parent to master engine, if not set
	if (parent == 0 && GetEngineCount() >= 1)
		parent = GetFirstValidEngine();

	//Create simulator instance
	EngineContext* engine = CreateEngine(parent, position, rotation, area_min, area_max);

	if (!GetActiveEngine())
		active_engine = engine;

	//have instance load building
	bool result = engine->Load(filename);

	if (result == false)
	{
		if (GetEngineCount() == 1)
			frontend->UnloadToMenu();
		else
			DeleteEngine(engine);
		return false;
	}

	//override SBS startup render option, if specified
	if (RenderOnStartup == true)
		engine->GetSystem()->RenderOnStartup = true;

	return true;
}

void VM::Report(const std::string &message, const std::string &prompt)
{
	try
	{
		if (Ogre::LogManager::getSingletonPtr())
			Ogre::LogManager::getSingleton().logMessage(prompt + " " + message);
	}
	catch (Ogre::Exception &e)
	{
		frontend->ShowError("VM: Error writing message to log\n" + e.getDescription());
	}
}

bool VM::ReportError(const std::string &message, const std::string &prompt)
{
	try
	{
		if (Ogre::LogManager::getSingletonPtr())
			Ogre::LogManager::getSingleton().logMessage(prompt + " " + message, Ogre::LML_CRITICAL);
	}
	catch (Ogre::Exception &e)
	{
		frontend->ShowError("VM: Error writing message to log\n" + e.getDescription());
	}
	return false;
}

bool VM::ReportFatalError(const std::string &message, const std::string &prompt)
{
	ReportError(message, prompt);
	frontend->ShowError(message);
	return false;
}

Ogre::ConfigFile* VM::LoadConfiguration(const std::string &filename, bool delete_after_use)
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
		ReportFatalError("Error loading configuration files\nDetails: " + e.getDescription(), "VM>");
		return 0;
	}
}

int VM::GetConfigInt(Ogre::ConfigFile *file, const std::string &key, int default_value)
{
	std::string result = file->getSetting(key, "", ToString(default_value));
	return ToInt(result);
}

std::string VM::GetConfigString(Ogre::ConfigFile *file, const std::string &key, const std::string &default_value)
{
	if (!file)
		return "";
	return file->getSetting(key, "", default_value);
}

bool VM::GetConfigBool(Ogre::ConfigFile *file, const std::string &key, bool default_value)
{
	if (!file)
		return false;
	std::string result = file->getSetting(key, "", BoolToString(default_value));
	return ToBool(result);
}

Real VM::GetConfigFloat(Ogre::ConfigFile *file, const std::string &key, Real default_value)
{
	if (!file)
		return 0.0;
	std::string result = file->getSetting(key, "", ToString(default_value));
	return ToFloat(result);
}

bool VM::Initialize(const std::string &data_path)
{
	//initialize OGRE
	try
	{
		mRoot = Ogre::Root::getSingletonPtr();
	}
	catch (Ogre::Exception &e)
	{
		return ReportFatalError("Error during initial OGRE check\nDetails: " + e.getDescription(), "VM>");
	}

	if(!mRoot)
	{
		try
		{
			//set up custom logger
			if (!logger)
			{
				logger = new Ogre::LogManager();
				Ogre::Log *log = logger->createLog(data_path + "skyscraper.log", true, !frontend->showconsole, false);
				log->addListener(frontend);
			}

			//report on system startup
			Report("Skyscraper version " + frontend->version_frontend + " starting...\n", "VM>");

			//load OGRE
			Report("Loading OGRE...", "VM>");
			mRoot = new Ogre::Root();
		}
		catch (Ogre::Exception &e)
		{
			return ReportFatalError("Error initializing OGRE\nDetails: " + e.getDescription(), "VM>");
		}
		catch (...)
		{
			return ReportFatalError("Error initializing OGRE", "VM>");
		}
	}

	//set up overlay system
	try
	{
		Report("", "VM>");
		Report("Loading Overlay System...", "VM>");
		mOverlaySystem = new Ogre::OverlaySystem();
	}
	catch (Ogre::Exception &e)
	{
		return ReportFatalError("Error creating overlay system\nDetails: " + e.getDescription(), "VM>");
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
		return ReportFatalError("Error configuring render system\nDetails: " + e.getDescription(), "VM>");
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
		Report("", "VM>");
		Report("Initializing OGRE...", "VM>");
		mRoot->initialise(false);

		if (frontend->Headless == false)
		{
			Report("", "VM>");
			Report("Creating render window...", "VM>");
			mRenderWindow = frontend->CreateRenderWindow(0, "SkyscraperVR");
		}
	}
	catch (Ogre::Exception &e)
	{
		return ReportFatalError("Error initializing render window\nDetails: " + e.getDescription(), "VM>");
	}
	catch (...)
	{
		return ReportFatalError("Error initializing render window", "VM>");
	}

	if (frontend->Headless == false)
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
		Report("", "VM>");
		Report("Loading resources...", "VM>");
		cf.load("resources.cfg");
	}
	catch (Ogre::Exception &e)
	{
		return ReportFatalError("Error loading resources.cfg\nDetails: " + e.getDescription(), "VM>");
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
		return ReportFatalError("Error initializing resources\nDetails: " + e.getDescription(), "VM>");
	}

	//create scene manager
	try
	{
		mSceneMgr = mRoot->createSceneManager();
	}
	catch (Ogre::Exception &e)
	{
		return ReportFatalError("Error creating scene manager\nDetails: " + e.getDescription(), "VM>");
	}

	mSceneMgr->addRenderQueueListener(mOverlaySystem);

	//enable shadows
	if (GetConfigBool(frontend->configfile, "Skyscraper.Frontend.Shadows", false) == true)
	{
		try
		{
			Report("Enabling shadows", "VM>");
			mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
		}
		catch (Ogre::Exception &e)
		{
			ReportFatalError("Error setting shadow technique\nDetails: " + e.getDescription(), "VM>");
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

	if (frontend->Headless == false)
	{
		try
		{
			//define camera configuration
			int cameras = 1; //use one camera for standard mode
			if (GetConfigBool(frontend->configfile, "Skyscraper.Frontend.VR", false) == true)
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
			return ReportFatalError("Error creating camera and viewport\nDetails: " + e.getDescription(), "VM>");
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
	int filtermode = GetConfigInt(frontend->configfile, "Skyscraper.Frontend.TextureFilter", 3);
	int maxanisotropy = GetConfigInt(frontend->configfile, "Skyscraper.Frontend.MaxAnisotropy", 8);

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
	DisableSound = GetConfigBool(frontend->configfile, "Skyscraper.Frontend.DisableSound", false);
	if (DisableSound == false)
	{
		Report("", "VM>");
		Report("FMOD Sound System, copyright (C) Firelight Technologies Pty, Ltd., 1994-2024\n", "VM>");

		FMOD_RESULT result = FMOD::System_Create(&soundsys);
		if (result != FMOD_OK)
		{
			std::string fmod_result = FMOD_ErrorString(result);
			ReportFatalError("Error initializing sound:\n" + fmod_result, "VM>");
			DisableSound = true;
		}
		else
		{
			char name [] = "Skyscraper"; //set name for PulseAudio on Linux
			result = soundsys->init(100, FMOD_INIT_NORMAL, &name);
			if (result != FMOD_OK)
			{
				std::string fmod_result = FMOD_ErrorString(result);
				ReportFatalError("Error initializing sound:\n" + fmod_result, "VM>");
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

				Report("Sound initialized: FMOD Engine version " + s_version, "VM>");
			}
		}
	}
	else
		Report("Sound Disabled", "VM>");

	try
	{
		mTrayMgr = new OgreBites::TrayManager("InterfaceName", mRenderWindow);
	}
	catch (Ogre::Exception &e)
	{
		ReportFatalError("Error starting tray manager:\nDetails: " + e.getDescription(), "VM>");
	}

	if (mTrayMgr)
	{
		mTrayMgr->hideCursor();
	}

	//report hardware concurrency
	int c = std::thread::hardware_concurrency();
	Report("Reported hardware concurrency: " + ToString(c) + "\n", "VM>");

	Report("Initialization complete", "VM>");
	Report("", "VM>");

	return true;
}

bool VM::Render()
{
	SBS_PROFILE_MAIN("Render");

	if (frontend->Headless == true)
		return true;

	// Render to the frame buffer
	try
	{
		mRoot->renderOneFrame();
	}
	catch (Ogre::Exception &e)
	{
		return ReportFatalError("Error in render operation\nDetails: " + e.getDescription(), "VM>");
	}

	//update frame statistics
	Ogre::FrameEvent a;
	if (mTrayMgr)
		mTrayMgr->frameRendered(a);

	return true;
}

bool VM::PlaySound(const std::string &filename)
{
	//load new sound
#if (FMOD_VERSION >> 16 == 4)
		FMOD_RESULT result = soundsys->createSound(filename.c_str(), (FMOD_MODE)(FMOD_2D | FMOD_ACCURATETIME | FMOD_SOFTWARE | FMOD_LOOP_NORMAL), 0, &sound);
#else
		FMOD_RESULT result = soundsys->createSound(filename.c_str(), (FMOD_MODE)(FMOD_2D | FMOD_ACCURATETIME | FMOD_LOOP_NORMAL), 0, &sound);
#endif
	if (result != FMOD_OK)
	{
		return ReportError("Can't load file '" + filename + "':\n" + FMOD_ErrorString(result), "VM>");
	}

#if (FMOD_VERSION >> 16 == 4)
	result = soundsys->playSound(FMOD_CHANNEL_FREE, sound, true, &channel);
#else
	result = soundsys->playSound(sound, 0, true, &channel);
#endif

	if (result != FMOD_OK)
	{
		return ReportError("Error playing " + filename, "VM>");
	}

	channel->setLoopCount(-1);
	channel->setVolume(1.0);
	channel->setPaused(false);

	return true;
}

void VM::StopSound()
{
	//stop and unload sound
	if (channel)
		channel->stop();
	if (sound)
		sound->release();
	sound = 0;
}

void VM::ClearScene()
{
	//clear scene
	mSceneMgr->clearScene();
}

void VM::UnloadSky()
{
	//unload Caelum sky system

	new_time = false;

	if (mCaelumSystem)
	{
		Caelum::CaelumPlugin* ptr = Caelum::CaelumPlugin::getSingletonPtr();
		mCaelumSystem->clear();
		mCaelumSystem->detachAllViewports();
		delete mCaelumSystem;
		mCaelumSystem = 0;
		Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup("Caelum");
		Caelum::CaelumPlugin::getSingleton().shutdown();
		delete ptr;
	}
}

void VM::CreateSky(EngineContext *engine)
{
	//create sky system

	//load Caelum plugin
	if (GetConfigBool(frontend->configfile, "Skyscraper.Frontend.Caelum", true) == true)
	{
		try
		{
			new Caelum::CaelumPlugin();
			Caelum::CaelumPlugin::getSingleton().initialise();
		}
		catch (Ogre::Exception &e)
		{
			if (e.getDescription() != "!msSingleton failed. There can be only one singleton")
				ReportFatalError("Error initializing Caelum plugin:\nDetails: " + e.getDescription(), "VM>");
			return;
		}
	}

	/*(if (sky_error == true)
	{
		engine->GetSystem()->CreateSky();
		return;
	}*/

	bool sky_result = true;
	if (GetConfigBool(frontend->configfile, "Skyscraper.Frontend.Caelum", true) == true)
		sky_result = InitSky(engine);

	//create old sky if Caelum is turned off, or failed to initialize
	if (sky_result == false)
		engine->GetSystem()->CreateSky();
}

void VM::ToggleStats()
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

void VM::EnableStats(bool value)
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

void VM::ReInit()
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
		ReportFatalError("Error creating overlay system\nDetails: " + e.getDescription(), "VM>");
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
		ReportFatalError("Error initializing resources\nDetails:" + e.getDescription(), "VM>");
	}

	//reinit tray manager
	try
	{
		mTrayMgr = new OgreBites::TrayManager("Tray", mRenderWindow);
	}
	catch (Ogre::Exception &e)
	{
		ReportFatalError("Error starting tray manager:\n" + e.getDescription(), "VM>");
	}

	if (mTrayMgr)
	{
		mTrayMgr->hideCursor();
	}

	show_stats = -1;
}

FMOD::System* VM::GetSoundSystem()
{
	return soundsys;
}

bool VM::InitSky(EngineContext *engine)
{
	//initialize sky

	if (!engine)
		return false;

	if (frontend->Headless == true)
		return true;

	if (Renderer == "Direct3D11")
		return true;

	//ensure graphics card and render system are capable of Caelum's shaders
	if (Renderer == "Direct3D9")
	{
		//on DirectX, Caelum requires a card capable of 3.0 shader levels, which would be
		//an ATI Radeon HD 2000, nVidia Geforce 6, Intel G965 or newer
		//Intel cards: http://www.intel.com/support/graphics/sb/cs-014257.htm
		Ogre::RenderSystemCapabilities::ShaderProfiles profiles = mRoot->getRenderSystem()->getCapabilities()->getSupportedShaderProfiles();

		//for general sky, require both DirectX pixel and vertex shaders 2.0
		if (profiles.find("ps_2_0") == profiles.end() ||
			profiles.find("vs_2_0") == profiles.end())
				return ReportFatalError("Error initializing Caelum: 2.0 shaders not supported", "VM>");

		//for clouds, require either DirectX pixel shaders 3.0 or nVidia fragment shaders 4.0
		if (profiles.find("ps_3_0") == profiles.end() &&
			profiles.find("fp40") == profiles.end())
				return ReportFatalError("Error initializing Caelum: 3.0 fragment shaders not supported", "VM>");

		//for clouds, require either DirectX vetex shaders 3.0 or nVidia vertex shaders 4.0
		if (profiles.find("vs_3_0") == profiles.end() &&
			profiles.find("vp40") == profiles.end())
				return ReportFatalError("Error initializing Caelum: 3.0 vertex shaders not supported", "VM>");
	}

	if (Renderer == "OpenGL")
	{
		//on OpenGL, Caelum requires hardware support for shaders (OpenGL 2.0 or newer)
		Ogre::RenderSystemCapabilities::ShaderProfiles profiles = mRoot->getRenderSystem()->getCapabilities()->getSupportedShaderProfiles();

		//require OpenGL ARB fragment programs
		if (profiles.find("arbfp1") == profiles.end())
			return ReportFatalError("Error initializing Caelum: fragment programs not supported", "VM>");

		//require OpenGL ARB vertex programs
		if (profiles.find("arbvp1") == profiles.end())
			return ReportFatalError("Error initializing Caelum: vertex programs not supported", "VM>");
	}

	//load Caelum resources
	try
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("data/caelum", "FileSystem", "Caelum", false);
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Caelum");

		if (!mCaelumSystem)
			mCaelumSystem = new Caelum::CaelumSystem(mRoot, mSceneMgr, Caelum::CaelumSystem::CAELUM_COMPONENTS_NONE);
		Caelum::CaelumPlugin::getSingleton().loadCaelumSystemFromScript(mCaelumSystem, SkyName);
	}
	catch (Ogre::Exception &e)
	{
#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE //ignore Caelum errors on Mac, due to a Cg error (Cg is not available on ARM CPUs, and is not bundled with the Mac version)
		ReportFatalError("Error initializing Caelum:\nDetails: " + e.getDescription(), "VM>");
#endif
		sky_error = true;
	}
	catch (...)
	{
		ReportFatalError("Error initializing Caelum", "VM>");
		sky_error = true;
	}

	if (!mCaelumSystem)
	{
		sky_error = true;
		return false;
	}

	//attach caelum to running viewport
	try
	{
		for (size_t i = 0; i < mViewports.size(); i++)
		{
			mCaelumSystem->attachViewport(mViewports[i]);
		}
		mCaelumSystem->setAutoNotifyCameraChanged(false);
		mCaelumSystem->setSceneFogDensityMultiplier(GetConfigFloat(frontend->configfile, "Skyscraper.Frontend.Caelum.FogMultiplier", 0.1) / 1000);
		if (GetConfigBool(frontend->configfile, "Skyscraper.Frontend.Caelum.EnableFog", true) == false)
			mCaelumSystem->setManageSceneFog(Ogre::FOG_NONE);
		mCaelumSystem->setManageAmbientLight(GetConfigBool(frontend->configfile, "Skyscraper.Frontend.Caelum.ModifyAmbient", false));

		//fix sky rotation
		Quaternion rot(Degree(180.0), Vector3::UNIT_Y);
		mCaelumSystem->getCaelumGroundNode()->setOrientation(rot);
		mCaelumSystem->getCaelumCameraNode()->setOrientation(rot);

		//have sky use SBS scaling factor
		Real scale = 1 / engine->GetSystem()->UnitScale;
		mCaelumSystem->getCaelumGroundNode()->setScale(scale, scale, scale);
		mCaelumSystem->getCaelumCameraNode()->setScale(scale, scale, scale);
	}
	catch (Ogre::Exception &e)
	{
		ReportFatalError("Error setting Caelum parameters:\nDetails: " + e.getDescription(), "VM>");
		sky_error = true;
	}
	catch (...)
	{
		ReportFatalError("Error setting Caelum parameters", "VM>");
		sky_error = true;
	}

	//set sky time multiplier if not already set
	if (SkyMult == 0)
		SkyMult = mCaelumSystem->getTimeScale();

	//set location if specified
	if (new_location == true)
	{
		mCaelumSystem->setObserverLatitude(Degree(latitude));
		mCaelumSystem->setObserverLongitude(Degree(longitude));
		new_location = false;
	}

	//use system time if specified
	if (GetConfigBool(frontend->configfile, "Skyscraper.Frontend.Caelum.UseSystemTime", false) == true && new_time == false)
		SetDateTimeNow();

	//set date/time if specified
	if (new_time == true)
	{
		mCaelumSystem->setJulianDay(datetime);
		new_time = false;
	}

	return true;
}

void VM::UpdateSky()
{
	//update sky
	SBS_PROFILE_MAIN("Sky");

	if (mCaelumSystem)
	{
		for (size_t i = 0; i < mCameras.size(); i++)
		{
			mCaelumSystem->notifyCameraChanged(mCameras[i]);
		}
		mCaelumSystem->setTimeScale(SkyMult);
		mCaelumSystem->updateSubcomponents(Real(GetActiveEngine()->GetSystem()->GetElapsedTime()) / 1000);
	}
}

void VM::SetLocation(Real latitude, Real longitude)
{
	this->latitude = latitude;
	this->longitude = longitude;
	new_location = true;
}

void VM::SetDateTimeNow()
{
	//set date and time to current time in UTC

	//get current time
	time_t now = time(0);

	//convert time to GMT
	tm* gmtm = gmtime(&now);
	if (gmtm == NULL)
		return;

	//convert time to Julian and set it
	double julian = Caelum::Astronomy::getJulianDayFromGregorianDateTime(gmtm->tm_year + 1900, gmtm->tm_mon + 1, gmtm->tm_mday, gmtm->tm_hour, gmtm->tm_min, gmtm->tm_sec);
	SetDateTime(julian);
}

void VM::SetDateTime(double julian_date_time)
{
	datetime = julian_date_time;
	new_time = true;

	if (mCaelumSystem)
		mCaelumSystem->setJulianDay(datetime);
}

void VM::GetTime(int &hour, int &minute, int &second)
{
	hour = -1;
	minute = -1;
	second = -1.0;

	if (!mCaelumSystem)
		return;

	Caelum::LongReal julian = mCaelumSystem->getJulianDay(), sec;
	int year, month, day;
	Caelum::Astronomy::getGregorianDateTimeFromJulianDay(julian, year, month, day, hour, minute, sec);
	second = (int)sec;
}

void VM::EnableSky(bool value)
{
	//enable or disable sky system

	//enable/disable old skybox system in engine 0
	if (GetEngine(0))
		GetEngine(0)->GetSystem()->EnableSkybox(value);

	//enable/disable Caelum sky system
	if (mCaelumSystem)
	{
		mCaelumSystem->getCaelumGroundNode()->setVisible(value);
		mCaelumSystem->getCaelumCameraNode()->setVisible(value);
	}
}

Ogre::RenderWindow* VM::GetRenderWindow()
{
	return mRenderWindow;
}

void VM::Clear()
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

Ogre::SceneManager* VM::GetSceneManager()
{
	return mSceneMgr;
}

Ogre::RenderWindow* VM::CreateRenderWindow(const std::string &name, int width, int height, const Ogre::NameValuePairList &params)
{
	//create the render window
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	if (GetConfigBool(frontend->configfile, "Skyscraper.Frontend.VR", false) == true)
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

void VM::DestroyRenderWindow()
{
	if (mRenderWindow)
	   Ogre::Root::getSingleton().detachRenderTarget(mRenderWindow);

	mRenderWindow->destroy();
	mRenderWindow = 0;
}

void VM::RefreshViewport()
{
	//refresh viewport to prevent rendering issues

	if (frontend->Headless == false)
	{
		for (size_t i = 0; i < mViewports.size(); i++)
		{
			mViewports[i]->_updateDimensions();
		}
	}
}

}
