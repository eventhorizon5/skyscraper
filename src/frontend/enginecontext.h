/* $Id$ */

/*
	Skyscraper 1.11 Alpha - Engine Context
	Copyright (C)2004-2017 Ryan Thoryk
	http://www.skyscrapersim.com
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@skyscrapersim.com

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

namespace Skyscraper {

class EngineContext
{
public:

	bool Reload;
	bool Moved; //true if engine has been moved on startup

	EngineContext(EngineContext *parent, Skyscraper *frontend, Ogre::SceneManager* mSceneManager, FMOD::System *fmodsystem, const Ogre::Vector3 &position = Ogre::Vector3::ZERO, float rotation = 0.0f, const Ogre::Vector3 &area_min = Ogre::Vector3::ZERO, const Ogre::Vector3 &area_max = Ogre::Vector3::ZERO);
	~EngineContext();
	ScriptProcessor* GetScriptProcessor();
	SBS::SBS *GetSystem() { return Simcore; }
	Skyscraper *GetFrontend() { return frontend; }
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
	bool Start(Ogre::Camera *camera);
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	bool ReportFatalError(const std::string &message);
	bool IsLoadingFinished();
	void UpdateProgress(int percent);
	int GetProgress() { return progress; }
	int GetNumber() { return instance; }
	SBS::CameraState GetCameraState();
	void SetCameraState(const SBS::CameraState &state, bool set_floor = true);
	bool IsInside();
	bool IsInside(const Ogre::Vector3 &position);
	void DetachCamera(bool reset_building = false);
	void AttachCamera(Ogre::Camera *camera, bool init_state = true);
	void RefreshCamera();
	void ResetCamera();
	void RevertMovement();
	Ogre::Vector3 GetCameraPosition();
	void OnEnter();
	void OnExit();
	void CutForEngine(EngineContext *engine);
	void AddChild(EngineContext *engine);
	void RemoveChild(EngineContext *engine);
	void RemoveParent() { parent = 0; }
	void Move(Ogre::Vector3 &position, bool move_children = false);
	EngineContext* GetParent() { return parent; }
	bool IsParent(EngineContext *engine, bool recursive = false);

private:

	void StartSim();
	void UnloadSim();

	Skyscraper* frontend; //frontend
	ScriptProcessor* processor; //script processor
	SBS::SBS *Simcore; //sim engine instance
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

	//override information
	SBS::CameraState *reload_state;

	Ogre::SceneManager* mSceneManager;
	FMOD::System *fmodsystem;
	Ogre::Vector3 position;
	Ogre::Vector3 area_min;
	Ogre::Vector3 area_max;
	float rotation;

	//child engines
	std::vector<EngineContext*> children;
};

}

#endif
