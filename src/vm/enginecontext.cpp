/*
	Skyscraper 2.1 - Engine Context
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

#ifdef USING_WX
#include "wx/wx.h"
#endif
#ifndef DISABLE_SOUND
#include <fmod.hpp>
#endif
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include <windows.h>
#endif
#include "globals.h"
#include "sbs.h"
#include "vm.h"
#include "hal.h"
#include "gui.h"
#include "camera.h"
#include "scriptproc.h"
#include "enginecontext.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
#include "malloc.h"
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX || OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <unistd.h>
#endif

using namespace SBS;

namespace Skyscraper {

EngineContext::EngineContext(EngineContext *parent, VM *vm, Ogre::SceneManager* mSceneManager, FMOD::System *fmodsystem, const Vector3 &position, Real rotation, const Vector3 &area_min, const Vector3 &area_max)
{
	this->fmodsystem = fmodsystem;
	Init(parent, vm, mSceneManager, position, rotation, area_min, area_max);
}

EngineContext::EngineContext(EngineContext *parent, VM *vm, Ogre::SceneManager* mSceneManager, const Vector3 &position, Real rotation, const Vector3 &area_min, const Vector3 &area_max)
{
	Init(parent, vm, mSceneManager, position, rotation, area_min, area_max);
}

EngineContext::~EngineContext()
{
	if (vm->IsValidEngine(parent) == true)
		parent->RemoveChild(this);

	if (children.empty() == false)
	{
		for (size_t i = 0; i < children.size(); i++)
		{
			children[i]->RemoveParent();
		}
	}

	//unload simulator
	UnloadSim();

	if (reload_state)
		delete reload_state;
	reload_state = 0;
}

void EngineContext::Init(EngineContext *parent, VM *vm, Ogre::SceneManager* mSceneManager, const Vector3 &position, Real rotation, const Vector3 &area_min, const Vector3 &area_max)
{
	this->vm = vm;
	finish_time = 0;
	shutdown = false;
	loading = false;
	running = false;
	reloading = false;
	Reload = false;
	reload_state = new CameraState;
	reload_state->floor = 0;
	reload_state->collisions = false;
	reload_state->gravity = false;
	reload_state->freelook = false;
	this->mSceneManager = mSceneManager;
	this->position = position;
	this->area_min = area_min;
	this->area_max = area_max;
	this->rotation = rotation;
	this->parent = parent;
	Simcore = 0;
	processor = 0;
	raised = false;
	progress = 0;
	inside = false;
	Moved = false;
	started = false;
	prepared = false;
	NewEngine = true;

	//register this engine, and get it's instance number
	instance = vm->RegisterEngine(this);

	Report("\nStarting instance " + ToString(instance) + "...");

	//add instance number to reports
	InstancePrompt = ToString(instance) + ">";

	if (parent)
			parent->AddChild(this);

	StartSim();
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
		bool in_main = InRunloop();
		bool result = processor->Run();

		if (loading == true)
		{
			prepared = false;

			if (result == false)
			{
				ReportError("Error processing building\n");
				Shutdown();
			#ifdef USING_WX
				vm->GetGUI()->CloseProgressDialog();
			#endif
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
			if (InRunloop() == false && prepared == false)
			{
				Simcore->Prepare(false);
				Simcore->DeleteColliders = false;
				Simcore->Init(); //initialize any new objects
				prepared = true;
			}
			else
			{
				loading = false;
			}
		}
	}
	else
		return false;

	//force window raise on startup, and report on missing files, if any
	if (Simcore->GetCurrentTime() - finish_time > 0 && raised == false && loading == false)
	{
#ifdef USING_WX
		vm->GetGUI()->RaiseWindow();
#endif
		raised = true;

		vm->ReportMissingFiles(processor->nonexistent_files);
	}

	//process internal clock
	Simcore->AdvanceClock();
	if (running == true)
		Simcore->CalculateFrameRate();

	//run SBS main loop
	Simcore->Loop(loading, processor->IsFinished);

	if (loading == false)
	{
		//run functions if user enters or leaves this engine
		if (inside == false && IsInside() == true)
			OnEnter();
		if (inside == true && IsInside() == false)
			OnExit();
	}

	return true;
}

bool EngineContext::InitSim()
{
	//initialize simulator

	if (!Simcore || !processor)
		return false;

	Simcore->Initialize();

	//load script processor object
	processor->Reset();

	return true;
}

void EngineContext::Boot()
{
	//boot simulator
	processor->Start();
}

bool EngineContext::LoadDefault()
{
	//load default simulation

	if (!Simcore || !processor)
		return false;

	loading = true;

	//initialize simulator
	InitSim();

	Report("\nLoading default simulation...\n");
	Simcore->BuildingFilename = "Default";

	//load defaults
	processor->LoadDefaults();

	//override SBS startup render option, if specified
	if (vm->GetRenderOnStartup() == true)
		Simcore->RenderOnStartup = true;

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
	InitSim();

	//load building data file
	Report("\nLoading building data from " + filename + "...\n");
	Simcore->BuildingFilename = filename;

	filename.insert(0, "buildings/");

	//load building
	if (!processor->LoadDataFile(filename))
	{
		loading = false;
		return false;
	}

	//create progress dialog
#ifdef USING_WX
	vm->GetGUI()->CreateProgressDialog(filename);
#endif

	//override SBS startup render option, if specified
	if (vm->GetRenderOnStartup() == true)
		Simcore->RenderOnStartup = true;

	return true;
}

void EngineContext::DoReload()
{
	//reload the current building

	if (!Simcore)
		return;

	reloading = true;

	//store camera state information
	std::string filename = Simcore->BuildingFilename;
	*reload_state = GetCameraState();

	//unload current simulator
	UnloadSim();

	//start a new simulator
	StartSim();

	//load building file
	if (Load(filename) == false)
	{
		reloading = false;
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
	//exit if already started
	if (started == true)
		return;

	//get offset of parent engine
	Vector3 offset;
	if (parent)
		offset = parent->GetSystem()->GetPosition();
	else
		offset = Vector3::ZERO;

	if (position != Vector3::ZERO)
		Moved = true;

	//Create simulator object
	if (!Simcore)
		Simcore = new ::SBS::SBS(mSceneManager, fmodsystem, instance, position + offset, rotation, area_min, area_max);

	//load script processor
	if (!processor)
		processor = new ScriptProcessor(this);

	//if using DirectX11, turn off models in script processor due to shader crashes
	processor->NoModels = vm->GetHAL()->DX11;

	//refresh console to fix banner message on Linux
#ifdef USING_WX
	vm->GetGUI()->RefreshConsole();
#endif

	//override verbose mode if specified
	if (vm->Verbose == true)
		Simcore->Verbose = true;

	//Pause for 2 seconds, if first instance
	if (instance == 0)
	{
		vm->Pause = true; //briefly pause frontend to prevent debug panel calls to engine
#ifdef USING_WX
		wxYield(); //this allows the banner to be printed before the sleep() call
#endif
		vm->Pause = false;
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

#ifndef DISABLE_SOUND
	//reset fmod reverb
	FMOD_REVERB_PROPERTIES prop = FMOD_PRESET_GENERIC;
	fmodsystem->setReverbProperties(0, &prop);
#endif

	//unload script processor
	if (processor)
		delete processor;
	processor = 0;

	loading = false;
	running = false;
	raised = false;
	Moved = false;
	started = false;

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
#ifdef __linux__
	//release free memory to OS on Linux
	malloc_trim(0);
#else
	//freebsd
#endif

#endif
}

bool EngineContext::Start(std::vector<Ogre::Camera*> &cameras)
{
	if (!Simcore)
		return false;

	//cut outside sim boundaries if specified
	Simcore->CutOutsideBoundaries(vm->CutLandscape, vm->CutBuildings, vm->CutExternal, vm->CutFloors);

	//if this has a parent engine, cut the parent for this new engine
	if (vm->IsValidEngine(parent) == true)
		parent->CutForEngine(this);

	//if this has child engines, and has reloaded, cut for the child engines
	if (children.empty() == false && reloading == true)
	{
		for (size_t i = 0; i < children.size(); i++)
		{
			CutForEngine(children[i]);
		}
	}

	//start simulator
	if (!Simcore->Start(cameras))
		return ReportError("Error starting simulator\n");

	//set to saved position if reloading building
	if (reloading == true)
	{
		reloading = false;
		SetCameraState(*reload_state);
	}

	loading = false;
	running = true;

	return true;
}

void EngineContext::Report(const std::string &message)
{
	vm->GetHAL()->Report(message, InstancePrompt);
}

bool EngineContext::ReportError(const std::string &message)
{
	return vm->GetHAL()->ReportError(message, InstancePrompt);
}

bool EngineContext::ReportFatalError(const std::string &message)
{
	ReportError(message);
#ifdef USING_WX
	vm->GetGUI()->ShowError(message);
#endif
	return false;
}

bool EngineContext::IsLoadingFinished()
{
	if (!processor)
		return false;

	return (loading == true && processor->IsFinished == true);
}

bool EngineContext::UpdateProgress(int percent)
{
	//update progress bar

	progress = percent;

	return vm->UpdateProgress();
}

CameraState EngineContext::GetCameraState()
{
	return Simcore->camera->GetCameraState();
}

void EngineContext::SetCameraState(const CameraState &state, bool set_floor)
{
	Simcore->camera->SetCameraState(state, set_floor);
}

bool EngineContext::IsInside()
{
	//return true if user is inside the boundaries of this engine context

	if (!Simcore)
		return false;

	if (!vm->GetActiveEngine())
		return Simcore->IsInside();

	//make sure the global camera's position is actually inside this engine
	return IsInside(vm->GetActiveEngine()->GetCameraPosition());
}

bool EngineContext::IsInside(const Vector3 &position)
{
	//return true if user is inside the boundaries of this engine context

	if (!Simcore)
		return false;

	return Simcore->IsInside(Simcore->FromGlobal(position));
}

void EngineContext::DetachCamera(bool reset_building)
{
	//detach the camera from this engine

	Simcore->DetachCamera();

	if (reset_building == true)
		Simcore->ResetState();
}

void EngineContext::AttachCamera(std::vector<Ogre::Camera*> &cameras, bool init_state)
{
	//attach the camera to this engine

	Simcore->AttachCamera(cameras, init_state);

	//reset camera position if camera is outside of the engine's area when attaching
	if (IsInside() == false)
		ResetCamera();
}

void EngineContext::RefreshCamera()
{
	Simcore->camera->Refresh();
}

void EngineContext::ResetCamera()
{
	//reset camera position
	Simcore->camera->SetToStartPosition(true);
}

void EngineContext::RevertMovement()
{
	//revert camera movement
	Simcore->camera->RevertMovement();
}

Vector3 EngineContext::GetCameraPosition()
{
	//get this engine's camera position, in global positioning

	return Simcore->ToGlobal(Simcore->camera->GetPosition());
}

void EngineContext::OnEnter()
{
	//switch to this engine on entry

	inside = true;

	if (vm->GetActiveEngine())
	{
		//if this engine is an ancestor of the active engine, don't switch to this engine
		if (vm->GetActiveEngine()->IsParent(this) == true)
			return;
	}

	//make this engine active
	vm->SetActiveEngine(instance, true);
}

void EngineContext::OnExit()
{
	inside = false;
}

void EngineContext::CutForEngine(EngineContext *engine)
{
	//cut holes in this sim engine, for a newly loaded building, if possible

	if (!engine || engine == this)
		return;

	::SBS::SBS *newsimcore = engine->GetSystem();

	Vector3 min, max, a, b, c, d, newmin, newmax;

	//get new engine's boundaries
	newsimcore->GetBounds(min, max);

	if (min == Vector3::ZERO && max == Vector3::ZERO)
		return;

	//get global positions of engine's boundaries, in 4 points representing a rectangle
	a = newsimcore->ToGlobal(Vector3(min.x, min.y, min.z));
	b = newsimcore->ToGlobal(Vector3(min.x, min.y, max.z));
	c = newsimcore->ToGlobal(Vector3(max.x, max.y, max.z));
	d = newsimcore->ToGlobal(Vector3(max.x, max.y, min.z));

	//convert global positions to this engine's relative positions
	a = Simcore->FromGlobal(a);
	b = Simcore->FromGlobal(b);
	c = Simcore->FromGlobal(c);
	d = Simcore->FromGlobal(d);

	//get new cutting bounds (get min/max values)
	newmin.x = Min(a.x, b.x, c.x, d.x);
	newmin.y = Min(a.y, b.y, c.y, d.y);
	newmin.z = Min(a.z, b.z, c.z, d.z);
	newmax.x = Max(a.x, b.x, c.x, d.x);
	newmax.y = Max(a.y, b.y, c.y, d.y);
	newmax.z = Max(a.z, b.z, c.z, d.z);

	//cut for new bounds
	Simcore->DeleteColliders = true;
	Simcore->CutInsideBoundaries(newmin, newmax, vm->CutLandscape, vm->CutBuildings, vm->CutExternal, vm->CutFloors);
	Simcore->DeleteColliders = false;

	if (IsRunning() == true)
		Simcore->Prepare();

	//if this has a valid parent, have parent cut for the specified engine
	if (vm->IsValidEngine(parent) == true)
		parent->CutForEngine(engine);
}

void EngineContext::AddChild(EngineContext *engine)
{
	if (engine)
		children.emplace_back(engine);
}

void EngineContext::RemoveChild(EngineContext *engine)
{
	for (size_t i = 0; i < children.size(); i++)
	{
		if (children[i] == engine)
		{
			children.erase(children.begin() + i);
			return;
		}
	}
}

void EngineContext::Move(Vector3 &position, bool move_children)
{
	//move this engine
	//if move_children is true, recursively call this function on all children

	this->position += position;
	Simcore->Move(position);

	if (move_children == true)
	{
		for (size_t i = 0; i < children.size(); i++)
		{
			children[i]->Move(position, move_children);
		}
	}
}

bool EngineContext::IsParent(EngineContext *engine, bool recursive)
{
	//returns true if the specified engine is a parent, or ancestor (if recursive is true) of this engine

	if (!engine)
		return false;

	if (!GetParent())
		return false;

	//if this engine is the specified engine's parent, return true
	if (engine == GetParent())
		return true;

	//ask the parent if recursive
	if (recursive == true)
		return GetParent()->IsParent(engine, recursive);

	return false;
}

VM* EngineContext::GetVM()
{
	return vm;
}

bool EngineContext::InRunloop()
{
	if (processor)
		return processor->InRunloop();
	return false;
}

bool EngineContext::IsRoot()
{
	//returns true if this engine is the root/primary engine (0)

	return (!GetParent());
}

void EngineContext::GatherReset()
{
	//reset a gather operation

	current_time = vm->GetHAL()->GetCurrentTime();
}

void EngineContext::Gather()
{
	//perform a gather operation
	//this collects timing information since the last reset

	unsigned long last = current_time;

	//get current time
	current_time = vm->GetHAL()->GetCurrentTime();
	if (last == 0)
		last = current_time;

	time_stat = current_time - last;
}

void EngineContext::ResetPrepare()
{
	prepared = false;
}

}
