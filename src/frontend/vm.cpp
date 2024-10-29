/*
	Skyscraper 2.1 - Nanokernel
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
	SkyMult = 0;
	mCaelumSystem = 0;
	new_location = false;
	new_time = false;
	latitude = 0.0;
	longitude = 0.0;
	datetime = 0.0;
	CutLandscape = true;
	CutBuildings = true;
	CutExternal = false;
	CutFloors = false;
	sky_error = 0;
	first_run = true;

	//create HAL instance
	hal = new HAL(this);

	Report("Started");
}

VM::~VM()
{
	//delete Caelum
	if (mCaelumSystem)
		delete mCaelumSystem;

	if (hal)
		delete hal;
	hal = 0;
}

HAL* VM::GetHAL()
{
	return hal;
}

EngineContext* VM::CreateEngine(EngineContext *parent, const Vector3 &position, Real rotation, const Vector3 &area_min, const Vector3 &area_max)
{
	EngineContext* engine = new EngineContext(parent, this, hal->GetSceneManager(), hal->GetSoundSystem(), position, rotation, area_min, area_max);
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
			Report("Engine instance " + ToString(i) + " deleted");

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

	Report("Deleting all engines...");
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

	Report("Setting engine " + ToString(number) + " as active");

	//switch context to new engine instance
	active_engine = engine;
	active_engine->AttachCamera(hal->mCameras, !switch_engines);

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
		hal->RefreshViewport();

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
				Report("Shutdown requested for engine instance " + ToString(i));

				if (DeleteEngine(engines[i]) == true)
				{
					hal->RefreshViewport();
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
				Report("Reloading engine instance " + ToString(i));

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
	Report("Searing for engine to attach to...");
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

	Report("Initiating engine start");
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
		Report("Unloading due to shutdown request");
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
		Report("Unloading to menu...");
		frontend->UnloadToMenu();
		return;
	}

	//update Caelum
	UpdateSky();

	//update OpenXR
	hal->UpdateOpenXR();

	//render graphics
	result = hal->Render();
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

bool VM::Load(const std::string &filename, EngineContext *parent, const Vector3 &position, Real rotation, const Vector3 &area_min, const Vector3 &area_max)
{
	//load simulator and data file

	Report("Loading engine for building file '" + filename + "'...");

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

void VM::Report(const std::string &message)
{
	hal->Report(message, "vm:");
}

bool VM::ReportError(const std::string &message)
{
	return hal->ReportError(message, "vm:");
}

bool VM::ReportFatalError(const std::string &message)
{
	return hal->ReportFatalError(message, "vm:");
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
	if (hal->GetConfigBool(frontend->configfile, "Skyscraper.Frontend.Caelum", true) == true)
	{
		try
		{
			new Caelum::CaelumPlugin();
			Caelum::CaelumPlugin::getSingleton().initialise();
		}
		catch (Ogre::Exception &e)
		{
			if (e.getDescription() != "!msSingleton failed. There can be only one singleton")
				ReportFatalError("Error initializing Caelum plugin:\nDetails: " + e.getDescription());
			return;
		}
	}

	/*(if (sky_error == true)
	{
		engine->GetSystem()->CreateSky();
		return;
	}*/

	bool sky_result = true;
	if (hal->GetConfigBool(frontend->configfile, "Skyscraper.Frontend.Caelum", true) == true)
		sky_result = InitSky(engine);

	//create old sky if Caelum is turned off, or failed to initialize
	if (sky_result == false)
		engine->GetSystem()->CreateSky();
}

bool VM::InitSky(EngineContext *engine)
{
	//initialize sky

	if (!engine)
		return false;

	if (frontend->Headless == true)
		return true;

	if (hal->Renderer == "Direct3D11")
		return true;

	//ensure graphics card and render system are capable of Caelum's shaders
	if (hal->Renderer == "Direct3D9")
	{
		//on DirectX, Caelum requires a card capable of 3.0 shader levels, which would be
		//an ATI Radeon HD 2000, nVidia Geforce 6, Intel G965 or newer
		//Intel cards: http://www.intel.com/support/graphics/sb/cs-014257.htm
		Ogre::RenderSystemCapabilities::ShaderProfiles profiles = hal->mRoot->getRenderSystem()->getCapabilities()->getSupportedShaderProfiles();

		//for general sky, require both DirectX pixel and vertex shaders 2.0
		if (profiles.find("ps_2_0") == profiles.end() ||
			profiles.find("vs_2_0") == profiles.end())
				return ReportFatalError("Error initializing Caelum: 2.0 shaders not supported");

		//for clouds, require either DirectX pixel shaders 3.0 or nVidia fragment shaders 4.0
		if (profiles.find("ps_3_0") == profiles.end() &&
			profiles.find("fp40") == profiles.end())
				return ReportFatalError("Error initializing Caelum: 3.0 fragment shaders not supported");

		//for clouds, require either DirectX vetex shaders 3.0 or nVidia vertex shaders 4.0
		if (profiles.find("vs_3_0") == profiles.end() &&
			profiles.find("vp40") == profiles.end())
				return ReportFatalError("Error initializing Caelum: 3.0 vertex shaders not supported");
	}

	if (hal->Renderer == "OpenGL")
	{
		//on OpenGL, Caelum requires hardware support for shaders (OpenGL 2.0 or newer)
		Ogre::RenderSystemCapabilities::ShaderProfiles profiles = hal->mRoot->getRenderSystem()->getCapabilities()->getSupportedShaderProfiles();

		//require OpenGL ARB fragment programs
		if (profiles.find("arbfp1") == profiles.end())
			return ReportFatalError("Error initializing Caelum: fragment programs not supported");

		//require OpenGL ARB vertex programs
		if (profiles.find("arbvp1") == profiles.end())
			return ReportFatalError("Error initializing Caelum: vertex programs not supported");
	}

	//load Caelum resources
	try
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("data/caelum", "FileSystem", "Caelum", false);
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Caelum");

		if (!mCaelumSystem)
			mCaelumSystem = new Caelum::CaelumSystem(hal->mRoot, hal->GetSceneManager(), Caelum::CaelumSystem::CAELUM_COMPONENTS_NONE);
		Caelum::CaelumPlugin::getSingleton().loadCaelumSystemFromScript(mCaelumSystem, SkyName);
	}
	catch (Ogre::Exception &e)
	{
#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE //ignore Caelum errors on Mac, due to a Cg error (Cg is not available on ARM CPUs, and is not bundled with the Mac version)
		ReportFatalError("Error initializing Caelum:\nDetails: " + e.getDescription());
#endif
		sky_error = true;
	}
	catch (...)
	{
		ReportFatalError("Error initializing Caelum");
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
		for (size_t i = 0; i < hal->mViewports.size(); i++)
		{
			mCaelumSystem->attachViewport(hal->mViewports[i]);
		}
		mCaelumSystem->setAutoNotifyCameraChanged(false);
		mCaelumSystem->setSceneFogDensityMultiplier(hal->GetConfigFloat(frontend->configfile, "Skyscraper.Frontend.Caelum.FogMultiplier", 0.1) / 1000);
		if (hal->GetConfigBool(frontend->configfile, "Skyscraper.Frontend.Caelum.EnableFog", true) == false)
			mCaelumSystem->setManageSceneFog(Ogre::FOG_NONE);
		mCaelumSystem->setManageAmbientLight(hal->GetConfigBool(frontend->configfile, "Skyscraper.Frontend.Caelum.ModifyAmbient", false));

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
		ReportFatalError("Error setting Caelum parameters:\nDetails: " + e.getDescription());
		sky_error = true;
	}
	catch (...)
	{
		ReportFatalError("Error setting Caelum parameters");
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
	if (hal->GetConfigBool(frontend->configfile, "Skyscraper.Frontend.Caelum.UseSystemTime", false) == true && new_time == false)
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
		for (size_t i = 0; i < hal->mCameras.size(); i++)
		{
			mCaelumSystem->notifyCameraChanged(hal->mCameras[i]);
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

}
