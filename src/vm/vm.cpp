/*
	Skyscraper 2.1 - Nanokernel
	Copyright (C)2003-2025 Ryan Thoryk
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

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <sys/sysctl.h>
#endif
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include <windows.h>
#endif
#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
#include <sys/utsname.h>
#endif
#include <iostream>
#include "dylib.hpp"
#include "globals.h"
#include "sbs.h"
#include "polymesh.h"
#include "vm.h"
#include "camera.h"
#include "scenenode.h"
#include "soundsystem.h"
#include "enginecontext.h"
#include "hal.h"
#include "sky.h"
#include "gui.h"
#include "profiler.h"
#include "gitrev.h"
#include "monitor.h"
#include "editor.h"
#include "vmconsole.h"

using namespace SBS;

namespace Skyscraper {

extern VMConsoleInput inputmgr; //console input manager

//Virtual Manager system

VM::VM()
{
	//initialize values
	parent = 0;
	active_engine = 0;
	Shutdown = false;
	ConcurrentLoads = false;
	RenderOnStartup = false;
	CheckScript = false;
	Pause = false;
	CutLandscape = true;
	CutBuildings = true;
	CutExternal = false;
	CutFloors = false;
	first_run = true;
	Verbose = false;
	showconsole = false;
	vmconsole = 0;
	loadstart = false;
	unloaded = false;
	monitor = 0;
	system_loaded = false;
	system_finished = false;
	running = false;
	first_attach = false;

	macos_major = 0;
	macos_minor = 0;

	version = "2.1";
	version_rev = ToString(GIT_REV);
	version_state = "Alpha";
	version_full = version + ".0." + version_rev;

	//create HAL instance
	hal = new HAL(this);

	//create sky system instance
	skysystem = new SkySystem(this);

	//create GUI instance
#ifdef USING_WX
	gui = new GUI(this);
#else
	gui = 0;
#endif

	//create monitor instance
	monitor = new Monitor(this);

	//create editor instance
	editor = new Editor(this);

	//LoadLibrary("test");

	Report("Started");
}

VM::~VM()
{
	Report("Shutting down...");

	//delete editor instance
	if (editor)
		delete editor;
	editor = 0;

	//delete monitor instance
	if (monitor)
		delete monitor;
	monitor = 0;

	//delete sky system instance
	if (skysystem)
		delete skysystem;
	skysystem = 0;

	//delete HAL instance
	if (hal)
		delete hal;
	hal = 0;

	//delete GUI instance
#ifdef USING_WX
	if (gui)
		delete gui;
	gui = 0;
#endif

	//delete VM console
	if (vmconsole)
		delete vmconsole;
	vmconsole = 0;
}

void VM::SetParent(wxWindow *parent)
{
	//sets the parent WX window to be used by the system, mainly for the GUI

	this->parent = parent;
}

HAL* VM::GetHAL()
{
	//returns HAL instance

	return hal;
}

SkySystem* VM::GetSkySystem()
{
	//returns sky system instance

	return skysystem;
}

GUI* VM::GetGUI()
{
	return gui;
}

EngineContext* VM::CreateEngine(EngineContext *parent, const Vector3 &position, const Vector3 &rotation, const Vector3 &area_min, const Vector3 &area_max)
{
	EngineContext* engine = new EngineContext(ENGINETYPE_GENERIC, parent, this, hal->GetSceneManager(), hal->GetSoundSystem(), position, rotation, area_min, area_max);
	return engine;
}

bool VM::DeleteEngine(const EngineContext *engine)
{
	//delete a specified sim engine instance

	if (!engine)
		return false;

	for (size_t i = 0; i < engines.size(); i++)
	{
		if (engines[i] == engine)
		{
			//don't delete the primary engine if others are running
			if (i == 0 && engines.size() > 1)
				return ReportError("Cannot delete primary engine with children");

			//delete the engine
			engines[i] = 0;
			delete engine;
			Report("Engine instance " + ToString(i) + " deleted");

			int count = GetEngineCount();

			if (active_engine == engine)
			{
				//if deleting the active engine, switch to the first valid engine
				active_engine = 0;
				if (count > 0)
				{
					int number = GetFirstValidEngine()->GetNumber();
					SetActiveEngine(number, false, true);
				}
			}
			else if (active_engine)
			{
				//if deleting another engine (not the active one), refresh the camera
				active_engine->RefreshCamera();
			}

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

	monitor->Cleanup();

	Report("Deleting all engines...");
	for (size_t i = 0; i < engines.size(); i++)
	{
		if (engines[i])
			delete engines[i];
	}
	engines.clear();
	active_engine = 0;
	system_loaded = false;
	system_finished = false;
	first_attach = false;
	running = false;
	unloaded = true;
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
	return 0;
}

void VM::SetActiveEngine(int number, bool switch_engines, bool force)
{
	//set an engine instance to be active

	EngineContext *engine = GetEngine(number);
	if (!engine)
		return;

	//don't switch if the engine is fully active (active and camera attached)
	if (active_engine == engine && engine->IsCameraActive())
		return;

	//don't switch if the camera's already active in the specified engine
	if (engine->IsCameraActive())
		return;

	//don't switch to engine if it's loading (unless forced)
	if (engine->IsLoading() && !force)
		return;

	//capture prior camera state if there is an active camera
	CameraState prev_state {};
	bool have_prev_state = false;

	if (active_engine && (engine->IsSystem == false || running == true) && engine->was_reloaded == false)
	{
		if (active_engine->IsCameraActive())
		{
			prev_state = active_engine->GetCameraState(); //always capture if available
			have_prev_state = true;
		}

		//detach camera from current engine
		active_engine->DetachCamera(switch_engines);
	}

	//switch context to new engine instance and attach camera
	if (engine->IsSystem == false || running == true)
	{
		Report("Setting engine " + ToString(number) + " as active");
		active_engine = engine;

		//init_state sets defaults for AttachCamera,
		//the SetCameraState command below will override it if relevant
		bool init_state = !switch_engines;
		if (first_attach == false)
		{
			first_attach = true;
			init_state = true;
		}

		active_engine->AttachCamera(hal->mCameras, init_state);
	}

	//apply camera state to new engine
	if (have_prev_state && switch_engines == true)
		active_engine->SetCameraState(prev_state, false);

	//apply camera state after reloading
	if (engine->was_reloaded == true)
		active_engine->SetCameraState(false);

	//update mouse cursor for freelook mode
	//frontend->GetWindow()->EnableFreelook(active_engine->GetSystem()->camera->Freelook); //FIXME

	engine->was_reloaded = false;
}

bool VM::RunEngines(std::vector<EngineContext*> &newengines)
{
	//run sim engine instances, and returns the new engine(s) created (if applicable)
	//to be started by the frontend

	SBS_PROFILE_MAIN("Engines");

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
		if (ConcurrentLoads == true || isloading == false || engines[i]->IsLoading() == true || RenderOnStartup == true)
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
				//process engine runloop
				engines[i]->GatherReset();
				if (engines[i]->Run() == false)
					result = false;
				engines[i]->Gather();
			}
		}

		//start engine if loading is finished
		if (RenderOnStartup == false)
		{
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
				engines[i]->NewEngine = false;
				newengines.emplace_back(engines[i]);
			}
		}
		else
		{
			//when RenderOnStartup is true, only add new engines to the list
			if (engines[i]->NewEngine == true)
			{
				newengines.emplace_back(engines[i]);
				engines[i]->NewEngine = false;
			}
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
			//delete engine if it's shutdown state is true
			if (engines[i]->GetShutdownState() == true)
			{
				Report("Shutdown requested for engine instance " + ToString(i));

				if (DeleteEngine(engines[i]) == true)
				{
					hal->RefreshViewport();
					i--;
					deleted = true;
				}
				else
					engines[i]->CancelShutdown(); //cancel shutdown if delete failed
			}
		}
	}

	//clean up empty engine slots at the end of the list
	if (deleted == true && engines.size() > 0)
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
					skysystem->UnloadSky();

				Pause = false;
				Report("Reloading engine instance " + ToString(i));

				engines[i]->DoReload(); //handle engine reload

				//create sky system if primary engine
				if (i == 0)
					skysystem->CreateSky(engines[i]);
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

int VM::GetEngineCount(bool loading_only)
{
	//get number of valid engines

	int count = 0;

	for (size_t i = 0; i < engines.size(); i++)
	{
		if (engines[i])
		{
			if (loading_only == true)
			{
				if (engines[i]->IsLoading() == true)
					count++;
			}
			else
				count++;
		}
	}
	return count;
}

int VM::GetEngineSlotCount()
{
	//return total number of engine slots

	return (int)engines.size();
}

void VM::SwitchEngines()
{
	//if user is no longer inside the active engine, find a new engine to attach to

	if (!active_engine)
		return;

	//exit if user is inside an engine
	//continue to processing if the camera is not active in the engine
	if (active_engine->IsInside() == true && active_engine->IsCameraActive() == true)
		return;

	//exit if active engine is loading
	if (active_engine->IsLoading() == true)
		return;

	//if the active engine was reloaded, try to reattach to that engine after load
	if (active_engine->IsInside() == true && active_engine->IsCameraActive() == false && active_engine->Paused == false)
	{
		SetActiveEngine(active_engine->GetNumber(), true);
		return;
	}

	EngineContext *parent = active_engine->GetParent();

	//if active engine has a parent, switch to the parent if possible
	if (parent)
	{
		if (parent->IsInside() == true && parent->IsCameraActive() == false && parent->Paused == false)
		{
			SetActiveEngine(parent->GetNumber(), true);
			return;
		}
	}

	//otherwise search for a valid engine to attach to
	Report("Searching for engine to attach to...");
	for (size_t i = 0; i < engines.size(); i++)
	{
		if (engines[i] != active_engine && engines[i])
		{
			if (engines[i]->IsInside() == true && engines[i]->IsCameraActive() == false && engines[i]->Paused == false)
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

bool VM::IsValidEngine(const EngineContext *engine)
{
	//returns true if the specified engine is valid (currently running)

	if (!engine)
		return false;

	for (size_t i = 0; i < engines.size(); i++)
	{
		if (engines[i] == engine)
			return true;
	}
	return false;
}

bool VM::IsValidSystem(const ::SBS::SBS *sbs)
{
	//returns true if the specified SBS instance is valid (being used by an engine context)

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
	{
		engine->time_stat = 0;
		engines.emplace_back(engine);
	}

	return number;
}

EngineContext* VM::GetFirstValidEngine()
{
	//returns the first valid (running) engine context

	for (size_t i = 0; i < engines.size(); i++)
	{
		if (engines[i])
			return engines[i];
	}
	return 0;
}

void VM::CheckCamera()
{
	//if the camera is not active in the active engine,
	//switch the active engine to the next one that has an active camera

	if (active_engine->IsCameraActive() == false)
	{
		EngineContext *newengine = FindActiveEngine();
		if (newengine)
		{
			Report("Switching to active engine " + ToString(newengine->GetNumber()));
			active_engine = newengine;
		}
	}
}

bool VM::StartEngine(EngineContext* engine)
{
	//start a sim engine

	Report("Initiating engine start");
	return engine->Start();
}

::SBS::SBS* VM::GetActiveSystem()
{
	//returns the active engine's SBS instance

	if (active_engine)
		return active_engine->GetSystem();
	return 0;
}

ScriptProcessor* VM::GetActiveScriptProcessor()
{
	//returns the active engine's script processor

	if (active_engine)
		return active_engine->GetScriptProcessor();
	return 0;
}

VMStatus VM::Run(std::vector<EngineContext*> &newengines)
{
	//run system

	//return codes:
	//VMSTATUS_FATAL: fatal error
	//VMSTATUS_ERROR: failure
	//VMSTATUS_SUCCESS: success
	//VMSTATUS_UNLOAD: unload
	//VMSTATUS_LOAD: load new building(s)

	SBS_PROFILE_MAIN("VM");

	//show progress dialog if needed
	//gui->ShowProgress();

	//load queued buildings
	LoadQueued();

	//get time for frame statistics
	unsigned long last = current_time;
	current_time = hal->GetCurrentTime();

	//run monitor
	bool monresult = monitor->Run();

	//run sim engines
	bool result = RunEngines(newengines);

	time_stat = hal->GetCurrentTime() - last;

	if (newengines.size() > 0)
		return VMSTATUS_LOAD;

	//delete an engine if requested
	HandleEngineShutdown();

	//exit if full shutdown request received
	if (Shutdown == true)
	{
		Shutdown = false;
		Report("Unloading due to shutdown request");
		return VMSTATUS_UNLOAD;
	}

	//exit if an engine failed to run, if it's either the only engine or if ConcurrentLoads is on
	if (result == false && (ConcurrentLoads == false || GetEngineCount() == 1))
		return VMSTATUS_ERROR;

	//don't continue if no available active engine
	if (!GetActiveEngine())
		return VMSTATUS_ERROR;

	//make sure active engine is the one the camera is active in
	CheckCamera();

	//exit if any engine is loading, unless RenderOnStartup is true
	if (IsEngineLoading() == true && RenderOnStartup == false)
		return VMSTATUS_SUCCESS;

	//if in CheckScript mode, exit
	if (CheckScript == true)
	{
		Report("Unloading to menu...");
		return VMSTATUS_UNLOAD;
	}

	//update running state;
	running = true;

	//update Caelum
	skysystem->UpdateSky();

	//update OpenXR
	hal->UpdateOpenXR();

	//render graphics
	result = hal->Render();
	if (!result)
		return VMSTATUS_FATAL;

	//handle a building reload
	HandleReload();

	//handle behavior when a user exits an engine area
	SwitchEngines();

	//update first run status
	if (first_run == true)
		first_run = false;

	return VMSTATUS_SUCCESS;
}

bool VM::Load(bool system, bool clear, const std::string &filename, EngineContext *parent, const Vector3 &position, const Vector3 &rotation, const Vector3 &area_min, const Vector3 &area_max)
{
	//load simulator and data file

	//exit if no building specified
	if (filename == "")
		return false;

	//add the building load request to the queue
	DelayLoad delay_load;
	delay_load.filename = filename;
	delay_load.clear = clear;
	delay_load.parent = parent;
	delay_load.position = position;
	delay_load.rotation = rotation;
	delay_load.area_min = area_min;
	delay_load.area_max = area_max;
	delay_load.system = system;

	load_queue.emplace_back(delay_load);

	monitor->CreateSim();

	return true;
}

bool VM::LoadQueued()
{
	//load engines in a queued fashion

	if (load_queue.size() == 0)
		return false;

	bool system_found = false;
	for (size_t i = 0; i < load_queue.size(); i++)
	{
		DelayLoad &load = load_queue[i];
		if (load.system == true)
		{
			system_found = true;
			break;
		}
	}

	if (system_found == false && system_loaded == true)
		system_finished = true;

	for (size_t i = 0; i < load_queue.size(); i++)
	{
		DelayLoad &load = load_queue[i];

		if (load.system == false)
		{
			if (system_finished == false)
				continue;

			if (engines[0]->IsRunning() == false)
				continue;
		}

		Report("Loading engine for building file '" + load.filename + "'...");

		//if no parent is specified, select first planet as parent
		if (load.parent == 0)
		{
			for (size_t j = 0; j < engines.size(); j++)
			{
				if (engines[j])
				{
					if (engines[j]->type == ENGINETYPE_PLANET)
					{
						load.parent = engines[j];
						break;
					}
				}
			}
		}

		//boot SBS
		EngineContext* engine = Initialize(load.clear, load.parent, load.position, load.rotation, load.area_min, load.area_max);
		engine->IsSystem = load.system;

		//exit if init failed
		if (!engine)
			return false;

		//have new engine instance load building
		bool result = engine->Load(load.filename);

		//delete engine if load failed, if more than one engine is running
		if (result == false)
		{
			if (GetEngineCount() > 1)
				DeleteEngine(engine);
		}

		if (load.system == true)
			system_loaded = true;
		load_queue.pop_back();
		i--;
	}
	return true;
}

EngineContext* VM::Initialize(bool clear, EngineContext *parent, const Vector3 &position, const Vector3 &rotation, const Vector3 &area_min, const Vector3 &area_max)
{
	//bootstrap simulator

	loadstart = true;

	if (GetEngineCount() == 0)
	{
		//set sky name
		skysystem->SkyName = hal->GetConfigString(hal->configfile, "Skyscraper.Frontend.Caelum.SkyName", "DefaultSky");

		//clear scene
		if (clear == true)
			hal->ClearScene();
	}

	//clear screen
	try
	{
		hal->GetRenderWindow()->update();
	}
	catch (...)
	{
		ReportFatalError("Error updating render window");
		return 0;
	}

	//set parent to master engine, if not set
	if (parent == 0 && GetEngineCount() >= 1)
		parent = GetFirstValidEngine();

	//Create simulator instance
	EngineContext* engine = CreateEngine(parent, position, rotation, area_min, area_max);

	if (!GetActiveEngine())
		active_engine = engine;

	//set render on startup state
	//SetRenderOnStartup(hal->GetConfigBool(hal->configfile, "Skyscraper.SBS.RenderOnStartup", false));

	return engine;
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

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE

// Code to get OS version on Mac
int get_macos_version(uint32_t &major, uint32_t &minor, bool &osx)
{
	//returns the OS major and minor version
	//if osx is true, os is 10.x.x releases, otherwise is 11.x or greater

	char osversion[32];
	size_t osversion_len = sizeof(osversion) - 1;
	int osversion_name[] = { CTL_KERN, KERN_OSRELEASE };

	if (sysctl(osversion_name, 2, osversion, &osversion_len, NULL, 0) == -1) {
		printf("get_macos_version: sysctl() failed\n");
		return 1;
	}

	if (sscanf(osversion, "%u.%u", &major, &minor) != 2) {
		printf("get_macos_version: sscanf() failed\n");
		return 1;
	}

	if (major >= 20) {
		major -= 9;
		osx = false; //macOS 11 and newer
	} else {
		major -= 4;
		osx = true; //macOS 10.1.1 and newer
	}

	return 0;
}

#endif

void VM::ShowPlatform()
{
	//set platform name

#if OGRE_ARCH_TYPE == OGRE_ARCHITECTURE_32
	Bits = "32-bit";
#endif
#if OGRE_ARCH_TYPE == OGRE_ARCHITECTURE_64
	Bits = "64-bit";
#endif

#if OGRE_CPU == OGRE_CPU_X86
	Architecture = "x86";
#elif OGRE_CPU == OGRE_CPU_PPC
	Architecture = "PPC";
#elif OGRE_CPU == OGRE_CPU_ARM
	Architecture = "ARM";
#elif OGRE_CPU == OGRE_CPU_MIPS
	Architecture = "MIPS";
#elif OGRE_CPU == OGRE_CPU_UNKNOWN
	Architecture = "Unknown";
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	Platform = "Windows " + Architecture + " " + Bits;
#elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX
	#ifdef  __FreeBSD__
		Platform = "FreeBSD " + Architecture + " " + Bits;
	#else
		Platform = "Linux " + Architecture + " " + Bits;
	#endif
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE
	Platform = "MacOS " + Architecture + " " + Bits;
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
	//report MacOS version if applicable
	uint32_t major = 0, minor = 0;
	bool osx = true;
	get_macos_version(major, minor, osx);

	if (osx == true)
	{
		GetHAL()->Report("Running on MacOS 10." + ToString((int)major) + "." + ToString((int)minor), "");
		macos_major = 10;
		macos_minor = (int)major;
	}
	else
	{
		GetHAL()->Report("Running on MacOS " + ToString((int)major) + "." + ToString((int)minor), "");
		macos_major = (int)major;
		macos_minor = (int)minor;
	}
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	//get Windows version

	NTSTATUS(WINAPI* RtlGetVersion)(LPOSVERSIONINFOEXW);
	OSVERSIONINFOEXW osInfo;

	*(FARPROC*)&RtlGetVersion = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");

	if (NULL != RtlGetVersion)
	{
		osInfo.dwOSVersionInfoSize = sizeof(osInfo);
		RtlGetVersion(&osInfo);
		GetHAL()->Report("Running on Microsoft Windows " + ToString((int)osInfo.dwMajorVersion) + "." + ToString((int)osInfo.dwMinorVersion), "");
	}
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
	struct utsname osInfo{};
	uname(&osInfo);
	#ifdef  __FreeBSD__
		GetHAL()->Report("Running on FreeBSD " + std::string(osInfo.release), "");
	#else
		GetHAL()->Report("Running on Linux " + std::string(osInfo.release), "");
	#endif
#endif
}

wxWindow* VM::GetParent()
{
	//get parent WX window, this is assigned by the frontend

	return parent;
}

bool VM::UpdateProgress()
{
	//update progress based on total sim engine status

	int total_percent = GetEngineCount() * 100;
	int current_percent = 0;

	for (size_t i = 0; i < GetEngineCount(); i++)
	{
		if (GetEngine(i))
			current_percent += GetEngine(i)->GetProgress();
	}

	int final = ((Real)current_percent / (Real)total_percent) * 100;

#ifdef USING_WX
	if (gui)
		return gui->UpdateProgress(final);
#endif
	return true;
}

bool VM::ReportMissingFiles(std::vector<std::string> &missing_files)
{
	//report missing files
#ifdef USING_WX
	if (gui)
		return gui->ReportMissingFiles(missing_files);
#endif
	return true;
}

void VM::StartConsole()
{
	//create VM console
	vmconsole = new VMConsole(this);
}

void VM::ProcessConsole()
{
	//if enabled, process console input
	if (vmconsole)
		vmconsole->Process();

}

VMConsole* VM::GetConsole()
{
	return vmconsole;
}

void VM::SetRenderOnStartup(bool value)
{
	RenderOnStartup = value;

	//override SBS startup render option, if specified
	for (int i = 0; i < engines.size(); i++)
	{
		if (engines[i])
			engines[i]->GetSystem()->RenderOnStartup = value;
	}
}

bool VM::GetRenderOnStartup()
{
	return RenderOnStartup;
}

dylib* VM::LoadLibrary(const std::string &name)
{
	//load shared library/DLL

	try
	{
		dylib *newlib = new dylib("./", name);
		dylibs.emplace_back(newlib);
		return newlib;
	}
	catch(const std::exception& e)
	{
		ReportError(e.what());
	}

	//return 0 on load failure
	return 0;
}

unsigned long VM::Uptime()
{
	//get uptime of VM in milliseconds

	return hal->GetCurrentTime();
}

unsigned long VM::GetElapsedTime(int instance)
{
	//get elapsed time (execution time) of the specified engine instance

	if (instance >= engines.size())
		return 0;
	if (engines[instance])
		return engines[instance]->time_stat;
	return 0;
}

void VM::ListPlayingSounds()
{
	//list playing sounds in all engines

	Report("Listing playing sounds in all engines");
	for (size_t i = 0; i < engines.size(); i++)
	{
		if (!engines[i])
			continue;

		::SBS::SBS* Simcore = engines[i]->GetSystem();
		
		if (Simcore)
		{
			SoundSystem* soundsys = Simcore->GetSoundSystem();
			if (soundsys)
			{
				soundsys->ShowPlayingSounds(false);
				if (i == engines.size() - 1)
					soundsys->ShowPlayingTotal();
			}
		}
	}
}

unsigned long VM::GetGlobalStats(unsigned long &meshes, unsigned long &textures, unsigned long &actions, unsigned long &sounds, unsigned long &objects, unsigned long &walls, unsigned long &polygons)
{
	meshes = 0;
	textures = 0;
	actions = 0;
	sounds = 0;
	objects = 0;
	walls = 0;
	polygons = 0;
	unsigned long total = 0;

	for (size_t i = 0; i < engines.size(); i++)
	{
		if (!engines[i])
			continue;

		::SBS::SBS* Simcore = engines[i]->GetSystem();

		if (Simcore)
		{
			++total;
			meshes += Simcore->GetMeshCount();
			textures += Simcore->GetTextureCount();
			actions += Simcore->GetActionCount();
			sounds += Simcore->GetSoundCount();
			objects += Simcore->GetObjectCount();
			walls += Simcore->GetPolyMesh()->GetWallCount();
			polygons += Simcore->GetPolyMesh()->GetPolygonCount();
		}
	}

	return total;
}

bool VM::IsRootLoaded()
{
	//returns true if the root engine is loaded or running

	if (engines.size() == 0)
		return false;

	if (engines[0])
	{
		if (engines[0]->IsLoadingFinished() == true || engines[0]->IsRunning() == true)
			return true;
	}

	return false;
}

bool VM::LoadPending()
{
	return !load_queue.empty();
}

Editor* VM::GetEditor()
{
	return editor;
}

struct tm VM::GetDateTime()
{
	time_t timestamp = time(0);
	struct tm datetime = *localtime(&timestamp);
	return datetime;
}

}
