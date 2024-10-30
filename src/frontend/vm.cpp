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
#include "sky.h"
#include "gui.h"
#include "profiler.h"

using namespace SBS;

namespace Skyscraper {

//Virtual Manager system

VM::VM()
{
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

	//create HAL instance
	hal = new HAL(this);

	//create sky system instance
	skysystem = new SkySystem(this);

	Report("Started");
}

VM::~VM()
{
	if (hal)
		delete hal;
	hal = 0;

	if (skysystem)
		delete skysystem;
	skysystem = 0;
}

HAL* VM::GetHAL()
{
	return hal;
}

SkySystem* VM::GetSkySystem()
{
	return skysystem;
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
	//frontend->GetWindow()->EnableFreelook(active_engine->GetSystem()->camera->Freelook); //FIXME
}

bool VM::RunEngines(EngineContext* &newengine)
{
	newengine = 0;
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
			newengine = engines[i];
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

int VM::Run(EngineContext* &newengine)
{
	//run system

	//return codes are 0 for failure, 1 for success, 2 to unload, and 3 to load a new building

	//run sim engines
	bool result = RunEngines(newengine);

	if (newengine)
		return 3;

	//delete an engine if requested
	HandleEngineShutdown();

	//exit if full shutdown request received
	if (Shutdown == true)
	{
		Shutdown = false;
		Report("Unloading due to shutdown request");
		return 2;
	}

	if (result == false && (ConcurrentLoads == false || GetEngineCount() == 1))
		return 0;

	if (!GetActiveEngine())
		return 0;

	//make sure active engine is the one the camera is active in
	CheckCamera();

	//exit if any engine is loading, unless RenderOnStartup is true
	if (IsEngineLoading() == true && RenderOnStartup == false)
		return 1;

	//if in CheckScript mode, exit
	if (CheckScript == true)
	{
		Report("Unloading to menu...");
		return 2;
	}

	//update Caelum
	skysystem->UpdateSky();

	//update OpenXR
	hal->UpdateOpenXR();

	//render graphics
	result = hal->Render();
	if (!result)
		return 0;

	//handle a building reload
	HandleReload();

	//handle behavior when a user exits an engine area
	SwitchEngines();

	//update first run status
	if (first_run == true)
		first_run = false;

	return 1;
}

bool VM::Load(const std::string &filename, EngineContext *parent, const Vector3 &position, Real rotation, const Vector3 &area_min, const Vector3 &area_max)
{
	//load simulator and data file

	//exit if no building specified
	if (filename == "")
		return false;

	Report("Loading engine for building file '" + filename + "'...");

	if (GetEngineCount() == 0)
	{
		//set sky name
		skysystem->SkyName = hal->GetConfigString(hal->configfile, "Skyscraper.Frontend.Caelum.SkyName", "DefaultSky");

		//clear scene
		hal->ClearScene();
	}

	//clear screen
	hal->GetRenderWindow()->update();

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
		if (GetEngineCount() > 1)
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

}
