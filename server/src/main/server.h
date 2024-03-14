/*
	Skyscraper 3.0 Alpha - Server Core
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

#ifndef SERVER_H
#define SERVER_H

int main (int argc, char* argv[]);

namespace Server {

class ScriptProcessor;

class Server
{
public:

	bool Reload;
	bool Moved; //true if engine has been moved on startup

	Server(Ogre::SceneManager* mSceneManager);
	~Server();
	ScriptProcessor* GetScriptProcessor();
	SBS::SBS *GetSystem() { return Simcore; }
	//Skyscraper *GetFrontend() { return frontend; }
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
	//SBS::CameraState GetCameraState();
	//void SetCameraState(const SBS::CameraState &state, bool set_floor = true);
	bool IsInside();
	bool IsInside(const Vector3 &position);
	void DetachCamera(bool reset_building = false);
	void AttachCamera(Ogre::Camera *camera, bool init_state = true);
	void RefreshCamera();
	void ResetCamera();
	void RevertMovement();
	Vector3 GetCameraPosition();
	void OnEnter();
	void OnExit();
	void CutForEngine(Server *engine);
	void AddChild(Server *engine);
	void RemoveChild(Server *engine);
	void RemoveParent() { parent = 0; }
	void Move(Vector3 &position, bool move_children = false);
	Server* GetParent() { return parent; }
	bool IsParent(Server *engine, bool recursive = false);

private:

	void StartSim();
	void UnloadSim();

	//Skyscraper* frontend; //frontend
	ScriptProcessor* processor; //script processor
	SBS::SBS *Simcore; //sim engine instance
	int instance; //instance number
	Server *parent; //parent engine
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
	//SBS::CameraState *reload_state;

	Ogre::SceneManager* mSceneManager;
	//FMOD::System *fmodsystem;
	//Vector3 position;
	//Vector3 area_min;
	//Vector3 area_max;
	//Real rotation;

	//child engines
	std::vector<Server*> children;
};

}

#endif
