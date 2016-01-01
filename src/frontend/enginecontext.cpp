/* $Id$ */

/*
	Skyscraper 1.10 Alpha - Engine Context
	Copyright (C)2003-2015 Ryan Thoryk
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

#include "globals.h"
#include "sbs.h"
#include "debugpanel.h"
#include "skyscraper.h"
#include "enginecontext.h"
#include "unix.h"

using namespace SBS;

namespace Skyscraper {

EngineContext::EngineContext(Skyscraper *frontend, Ogre::SceneManager* mSceneManager, FMOD::System *fmodsystem, int instance_number, const Ogre::Vector3 &position, const Ogre::Vector3 &area_min, const Ogre::Vector3 &area_max)
{
	this->frontend = frontend;
	finish_time = 0;
	shutdown = false;
	loading = false;
	running = false;
	Reload = false;
	PositionOverride = false;
	reload_state.floor = 0;
	reload_state.collisions = false;
	reload_state.gravity = false;
	reload_state.freelook = false;
	this->mSceneManager = mSceneManager;
	this->fmodsystem = fmodsystem;
	this->position = position;
	this->area_min = area_min;
	this->area_max = area_max;
	Simcore = 0;
	processor = 0;
	raised = false;
	progress = 0;

	instance = instance_number;
	Report("\nStarting instance " + ToString(instance) + "...");

	//add instance number to reports
	InstancePrompt = ToString(instance) + "> ";

	StartSim();
}

EngineContext::~EngineContext()
{
	UnloadSim();
}

ScriptProcessor* EngineContext::GetScriptProcessor()
{
	return processor;
}

bool EngineContext::IsCameraActive()
{
	if (!Simcore)
		return false;

	return Simcore->camera->IsActive();
}

void EngineContext::Shutdown()
{
	//request a shutdown

	shutdown = true;
}

bool EngineContext::Run()
{
	if (!Simcore)
		return false;

	//run script processor
	if (processor)
	{
		bool result = processor->Run();

		if (loading == true)
		{
			if (result == false)
			{
				ReportError("Error processing building\n");
				Shutdown();
				frontend->CloseProgressDialog();
				return false;
			}
			else if (processor->IsFinished == true)
			{
				//building has finished loading
				finish_time = Simcore->GetCurrentTime();
			}

			if (Simcore->RenderOnStartup == false)
				return false;
		}
		else if (processor->IsFinished == true && result == true)
		{
			Simcore->Prepare(false);
			Simcore->DeleteColliders = false;
		}
	}
	else
		return false;

	//force window raise on startup, and report on missing files, if any
	if (Simcore->GetCurrentTime() - finish_time > 0 && raised == false && loading == false)
	{
		frontend->RaiseWindow();
		raised = true;

		processor->ReportMissingFiles();
	}

	//process internal clock
	Simcore->AdvanceClock();
	if (running == true)
		Simcore->CalculateFrameRate();

	if (loading == false)
	{
		//run SBS main loop
		Simcore->MainLoop();

		//get input
		if (IsCameraActive() == true)
			frontend->GetInput(this);

		//process camera loop
		Simcore->CameraLoop();
	}

	return true;
}

bool EngineContext::Load(std::string filename)
{
	//load simulator and data file

	if (!Simcore || !processor)
		return false;

	//exit if no building specified
	if (filename == "")
		return false;

	loading = true;

	//initialize simulator
	Simcore->Initialize();

	//load building data file
	Report("\nLoading building data from " + filename + "...\n");
	Simcore->BuildingFilename = filename;

	filename.insert(0, "buildings/");

	//load script processor object and load building

	processor->Reset();

	if (!processor->LoadDataFile(filename))
	{
		loading = false;
		return false;
	}

	//create progress dialog
	frontend->CreateProgressDialog(filename);

	return true;
}

void EngineContext::DoReload()
{
	//reload the current building

	if (!Simcore)
		return;

	PositionOverride = true;

	//store camera state information
	std::string filename = Simcore->BuildingFilename;
	reload_state = GetCameraState();

	//unload current simulator
	UnloadSim();

	//start a new simulator
	StartSim();

	//load building file
	if (Load(filename) == false)
	{
		PositionOverride = false;
		Reload = false;
		return;
	}

	Reload = false;
}

std::string EngineContext::GetFilename()
{
	if (Simcore)
		return Simcore->BuildingFilename;
	return "";
}

void EngineContext::StartSim()
{
	//Create simulator object
	if (!Simcore)
		Simcore = new ::SBS::SBS(mSceneManager, fmodsystem, instance, position, area_min, area_max);

	//load script processor
	if (!processor)
		processor = new ScriptProcessor(this);

	//refresh console to fix banner message on Linux
	frontend->RefreshConsole();

	//Pause for 2 seconds, if first instance
	if (instance == 0)
	{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		Sleep(2000);
#else
		sleep(2);
#endif
	}
}

void EngineContext::UnloadSim()
{
	if (Simcore)
	{
		delete Simcore;
		Report("\nSBS instance " + ToString(instance) + " unloaded");
	}
	Simcore = 0;

	//unload script processor
	if (processor)
		delete processor;
	processor = 0;

	loading = false;
	running = false;
	raised = false;
}

bool EngineContext::Start(Ogre::Camera *camera)
{
	if (!Simcore)
		return false;

	//cut outside sim boundaries if specified
	Simcore->CutOutsideBoundaries(frontend->CutLandscape, frontend->CutBuildings, frontend->CutExternal, frontend->CutFloors);

	//start simulator
	if (!Simcore->Start(camera))
		return ReportError("Error starting simulator\n");

	//set to saved position if reloading building
	if (PositionOverride == true)
	{
		PositionOverride = false;
		SetCameraState(reload_state);
	}

	loading = false;
	running = true;

	return true;
}

void EngineContext::Report(const std::string &message)
{
	frontend->Report(InstancePrompt + message);
}

bool EngineContext::ReportError(const std::string &message)
{
	return frontend->ReportError(InstancePrompt + message);
}

bool EngineContext::ReportFatalError(const std::string &message)
{
	ReportError(InstancePrompt + message);
	frontend->ShowError(InstancePrompt + message);
	return false;
}

bool EngineContext::IsLoadingFinished()
{
	if (!processor)
		return false;

	return (loading == true && processor->IsFinished == true);
}

void EngineContext::UpdateProgress(int percent)
{
	//update progress bar

	progress = percent;
	frontend->UpdateProgress();
}

CameraState EngineContext::GetCameraState()
{
	CameraState state;
	state.position = Simcore->camera->GetPosition() + Simcore->GetPosition();
	state.rotation = Simcore->camera->GetRotation();
	state.floor = Simcore->camera->CurrentFloor;
	state.collisions = Simcore->camera->CollisionsEnabled();
	state.gravity = Simcore->camera->GetGravityStatus();
	state.freelook = Simcore->camera->Freelook;
	return state;
}

void EngineContext::SetCameraState(const CameraState &state, bool set_floor)
{
	if (!Simcore)
		return;

	if (set_floor == true)
		Simcore->camera->GotoFloor(state.floor, true);
	Simcore->camera->SetPosition(state.position - Simcore->GetPosition());
	Simcore->camera->SetRotation(state.rotation);
	Simcore->camera->EnableCollisions(state.collisions);
	Simcore->camera->EnableGravity(state.gravity);
	Simcore->camera->Freelook = state.freelook;
}

bool EngineContext::IsInside()
{
	//return true if user is inside the boundaries of this engine context

	if (!Simcore)
		return false;

	return Simcore->IsInside();
}

bool EngineContext::IsInside(const Ogre::Vector3 &position)
{
	//return true if user is inside the boundaries of this engine context

	if (!Simcore)
		return false;

	return Simcore->IsInside(position - Simcore->GetPosition());
}

}
