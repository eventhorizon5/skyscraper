/*
	Skyscraper 2.0 - Nanokernel
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

#ifndef VM_H
#define VM_H

#include <vector>

namespace Skyscraper {

class EngineContext;
class ScriptProcessor;
class HAL;
class SkySystem;
class GUI;

//Virtual Manager system
class VM
{
public:
	VM();
	~VM();
	HAL* GetHAL();
	SkySystem* GetSkySystem();
	EngineContext* GetActiveEngine() { return active_engine; }
	EngineContext* GetEngine(int number);
	EngineContext* CreateEngine(EngineContext *parent = 0, const Vector3 &position = Vector3::ZERO, Real rotation = 0.0, const Vector3 &area_min = Vector3::ZERO, const Vector3 &area_max = Vector3::ZERO);
	bool DeleteEngine(EngineContext *engine);
	void DeleteEngines();
	int GetEngineCount();
	int GetEngineListSize() { return (int)engines.size(); }
	EngineContext* FindActiveEngine();
	void SetActiveEngine(int number, bool switch_engines = false);
	bool IsEngineLoading();
	bool IsValidEngine(EngineContext *engine);
	bool IsValidSystem(::SBS::SBS *sbs);
	int RegisterEngine(EngineContext *engine);
	EngineContext* GetFirstValidEngine();
	int GetFreeInstanceNumber();
	int Run();
	bool StartEngine(EngineContext* engine, std::vector<Ogre::Camera*> &cameras);
	::SBS::SBS* GetActiveSystem();
	ScriptProcessor* GetActiveScriptProcessor();
	bool Load(const std::string &filename, EngineContext *parent = 0, const Vector3 &position = Vector3::ZERO, Real rotation = 0.0, const Vector3 &area_min = Vector3::ZERO, const Vector3 &area_max = Vector3::ZERO);

	bool Shutdown;
	bool ConcurrentLoads; //set to true for buildings to be loaded while another sim is active and rendering
	bool RenderOnStartup; //override SBS engine setting with same name
	bool CheckScript; //if set to true, checks building scripts instead of fully loading them
	bool Pause; //pause simulator
	bool CutLandscape, CutBuildings, CutExternal, CutFloors;
	bool Verbose; //verbose mode

	std::string version;
	std::string version_rev;
	std::string version_state;
	std::string version_frontend;

private:

	bool RunEngines();
	void CheckCamera();
	void HandleEngineShutdown();
	void HandleReload();
	void SwitchEngines();
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	bool ReportFatalError(const std::string &message);

	EngineContext *active_engine;
	std::vector<EngineContext*> engines;
	HAL *hal; //hardware abstraction layer
	SkySystem *skysystem;

	bool first_run;
};

}

#endif
