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

#include <wx/panel.h>
#include "globals.h"
#include "vm.h"
#include "skyscraper.h"
#include "mainscreen.h"
#include "sbs.h"
#include "camera.h"
#include "enginecontext.h"

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
}

EngineContext* VM::CreateEngine(EngineContext *parent, const Vector3 &position, Real rotation, const Vector3 &area_min, const Vector3 &area_max)
{
	EngineContext* engine = new EngineContext(parent, this, frontend->mSceneMgr, frontend->GetSoundSystem(), position, rotation, area_min, area_max);
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

	frontend->Report("Setting engine " + ToString(number) + " as active");

	//switch context to new engine instance
	active_engine = engine;
	active_engine->AttachCamera(frontend->mCameras, !switch_engines);

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
		frontend->RefreshViewport();

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
				if (DeleteEngine(engines[i]) == true)
				{
					frontend->RefreshViewport();
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
					frontend->UnloadSky();

				frontend->Pause = false;
				engines[i]->DoReload(); //handle engine reload

				//create sky system if primary engine
				if (i == 0)
					frontend->CreateSky(engines[i]);
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
		frontend->UnloadToMenu();
		return;
	}

	//update OpenXR
	UpdateOpenXR();

	//render graphics
	result = frontend->Render();
	if (!result)
		return;

	//handle a building reload
	HandleReload();

	//handle behavior when a user exits an engine area
	SwitchEngines();
}

void VM::UpdateOpenXR()
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	//update OpenXR camera transformations
	if (frontend->GetConfigBool("Skyscraper.Frontend.VR", false) == true)
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

}
