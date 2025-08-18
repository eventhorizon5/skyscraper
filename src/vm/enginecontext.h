/*
	Skyscraper 2.1 - Engine Context
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

#ifndef ENGINECONTEXT_H
#define ENGINECONTEXT_H

#include "vm.h"

namespace Skyscraper {

class ScriptProcessor;

class VMIMPEXP EngineContext
{
public:

	bool Reload;
	bool Moved; //true if engine has been moved on startup
	bool started;
	bool NewEngine; //true if this is a new engine
	unsigned long current_time, time_stat;
	bool Paused;
	bool IsSystem;
	EngineType type;
	bool was_reloaded;

	EngineContext(const EngineType type, EngineContext *parent, VM *vm, Ogre::SceneManager* mSceneManager, FMOD::System *fmodsystem, const Vector3 &position = Vector3::ZERO, const Vector3 &rotation = Vector3::ZERO, const Vector3 &area_min = Vector3::ZERO, const Vector3 &area_max = Vector3::ZERO);
	EngineContext(const EngineType type, EngineContext *parent, VM *vm, Ogre::SceneManager* mSceneManager, const Vector3 &position = Vector3::ZERO, const Vector3 &rotation = Vector3::ZERO, const Vector3 &area_min = Vector3::ZERO, const Vector3 &area_max = Vector3::ZERO);
	~EngineContext();
	void Init(const EngineType type, EngineContext *parent, VM *vm, Ogre::SceneManager* mSceneManager, const Vector3 &position, const Vector3 &rotation, const Vector3 &area_min, const Vector3 &area_max);
	ScriptProcessor* GetScriptProcessor();
	::SBS::SBS *GetSystem() { return Simcore; }
	bool IsCameraActive();
	bool Run();
	void Shutdown();
	bool GetShutdownState() { return shutdown; }
	bool Load(std::string filename);
	bool IsLoading() { return loading; }
	bool IsRunning() { return running; }
	bool IsReloading() { return reloading; }
	void DoReload();
	std::string GetFilename();
	bool Start();
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	bool ReportFatalError(const std::string &message);
	bool IsLoadingFinished();
	bool UpdateProgress(int percent);
	int GetProgress() { return progress; }
	int GetNumber() { return instance; }
	::SBS::CameraState GetCameraState();
	void SetCameraState(const ::SBS::CameraState &state, bool set_floor = true);
	void SetCameraState(bool set_floor);
	bool IsInside();
	bool IsInside(const Vector3 &position);
	void DetachCamera(bool reset_building = false);
	bool AttachCamera(std::vector<Ogre::Camera*> &cameras, bool init_state = true);
	void RefreshCamera();
	void ResetCamera();
	void RevertMovement();
	Vector3 GetCameraPosition();
	void OnEnter();
	void OnExit();
	void CutForEngine(EngineContext *engine, bool child = false);
	void AddChild(EngineContext *engine);
	void RemoveChild(const EngineContext *engine);
	void RemoveParent() { parent = 0; }
	void Move(Vector3 &vector, Real speed, bool move_children = false);
	void Rotate(const Vector3 &vector, Real speed, bool relative);
	EngineContext* GetParent() { return parent; }
	bool IsParent(EngineContext *engine, bool recursive = false);
	VM* GetVM();
	bool InRunloop();
	bool IsRoot();
	bool InitSim();
	bool LoadDefault();
	void Boot();
	void GatherReset();
	void Gather();
	void ResetPrepare();
	void CancelShutdown() { shutdown = false; }
	void Reset(bool full = true);
	Vector3 GetPosition(bool relative = false);
	Vector3 GetRotation();
	EngineContext* GetChild(size_t index);
	size_t GetChildCount();
	std::string GetStatus();
	std::string GetType();

private:

	void StartSim(const Vector3 &position, const Vector3 &rotation);
	void UnloadSim();
	void Init();

	ScriptProcessor* processor; //script processor
	::SBS::SBS *Simcore; //sim engine instance
	int instance; //instance number
	EngineContext *parent; //parent engine
	unsigned long finish_time;
	bool shutdown;
	bool loading;
	bool running;
	bool reloading;
	bool raised;
	int progress;
	bool inside;
	std::string InstancePrompt;
	bool prepared;

	//override information
	::SBS::CameraState *reload_state;

	Ogre::SceneManager* mSceneManager;
	FMOD::System *fmodsystem;
	Vector3 area_min;
	Vector3 area_max;

	//child engines
	std::vector<EngineContext*> children;

	//VM instance
	VM *vm;
};

}

#endif
