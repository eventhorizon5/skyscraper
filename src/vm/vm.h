/*
	Skyscraper 2.1 - Nanokernel
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

#ifndef VM_H
#define VM_H

#include <thread>
#include <vector>

//DLL Exporting
#ifdef _WIN32
	#if defined(_MSC_VER)
		#define __VISUALC__ _MSC_VER
	#endif
	#if defined(__VISUALC__) || defined(__GNUC__)
		#if defined(VM_EXPORTS)
			#define VMIMPEXP __declspec(dllexport)
			#define VMIMPEXP_DATA(type) __declspec(dllexport) type
		#else
			#define VMIMPEXP __declspec(dllimport)
			#define VMIMPEXP_DATA(type) __declspec(dllimport) type
		#endif
	#else
		#define VMIMPEXP
		#define VMIMPEXP_DATA(type) type
	#endif
#endif

#ifndef VMIMPEXP
	#define VMIMPEXP
	#define VMIMPEXP_DATA(type)
#endif

namespace SBS {

	class SBS;
}

class wxWindow;
class dylib;

namespace Skyscraper {

enum EngineType
{
	ENGINETYPE_GENERIC,
	ENGINETYPE_BUILDING,
	ENGINETYPE_CITY,
	ENGINETYPE_PLANET,
	ENGINETYPE_SOLARSYSTEM
};

enum VMStatus
{
	VMSTATUS_FATAL = -1,
	VMSTATUS_ERROR = 0,
	VMSTATUS_SUCCESS = 1,
	VMSTATUS_UNLOAD = 2,
	VMSTATUS_LOAD = 3
};

class EngineContext;
class ScriptProcessor;
class HAL;
class SkySystem;
class GUI;
class VMConsole;
class Monitor;
class Editor;

//Virtual Manager system
class VMIMPEXP VM
{
	friend class VMConsole;
	friend class Monitor;

public:
	VM();
	~VM();
	void SetParent(wxWindow *parent);
	HAL* GetHAL();
	SkySystem* GetSkySystem();
	GUI* GetGUI();
	EngineContext* GetActiveEngine() { return active_engine; }
	EngineContext* GetEngine(int number);
	EngineContext* CreateEngine(EngineContext *parent = 0, const Vector3 &position = Vector3::ZERO, const Vector3 &rotation = Vector3::ZERO, const Vector3 &area_min = Vector3::ZERO, const Vector3 &area_max = Vector3::ZERO);
	bool DeleteEngine(const EngineContext *engine);
	void DeleteEngines();
	int GetEngineCount(bool loading_only = false);
	int GetEngineListSize() { return (int)engines.size(); }
	EngineContext* FindActiveEngine();
	void SetActiveEngine(int number, bool switch_engines = false, bool force = false);
	bool IsEngineLoading();
	bool IsValidEngine(const EngineContext *engine);
	bool IsValidSystem(const ::SBS::SBS *sbs);
	int RegisterEngine(EngineContext *engine);
	EngineContext* GetFirstValidEngine();
	int GetFreeInstanceNumber();
	VMStatus Run(std::vector<EngineContext*> &newengine);
	bool StartEngine(EngineContext* engine);
	::SBS::SBS* GetActiveSystem();
	ScriptProcessor* GetActiveScriptProcessor();
	bool Load(bool system, bool clear, const std::string &filename, EngineContext *parent = 0, const Vector3 &position = Vector3::ZERO, const Vector3 &rotation = Vector3::ZERO, const Vector3 &area_min = Vector3::ZERO, const Vector3 &area_max = Vector3::ZERO);
	void ShowPlatform();
	wxWindow* GetParent();
	bool UpdateProgress();
	bool ReportMissingFiles(std::vector<std::string> &missing_files);
	void StartConsole();
	void ProcessConsole();
	VMConsole* GetConsole();
	EngineContext* Initialize(bool clear, EngineContext *parent = 0, const Vector3 &position = Vector3::ZERO, const Vector3 &rotation = Vector3::ZERO, const Vector3 &area_min = Vector3::ZERO, const Vector3 &area_max = Vector3::ZERO);
	void SetRenderOnStartup(bool value);
	bool GetRenderOnStartup();
	dylib* LoadLibrary(const std::string &name);
	unsigned long Uptime();
	unsigned long GetElapsedTime(int instance);
	void ListPlayingSounds();
	unsigned long GetGlobalStats(unsigned long &meshes, unsigned long &textures, unsigned long &actions, unsigned long &sounds, unsigned long &objects, unsigned long &walls, unsigned long &polygons);
	bool IsRootLoaded();
	bool LoadPending();
	bool IsRunning() { return running; }
	Editor* GetEditor();
	struct tm GetDateTime();
	int GetEngineSlotCount();

	bool Shutdown;
	bool ConcurrentLoads; //set to true for buildings to be loaded while another sim is active and rendering
	bool CheckScript; //if set to true, checks building scripts instead of fully loading them
	bool Pause; //pause simulator
	bool CutLandscape, CutBuildings, CutExternal, CutFloors;
	bool Verbose; //verbose mode
	bool showconsole;
	bool loadstart; //true if starting an engine load
	bool unloaded;
	unsigned long time_stat, current_time; //frame timing

	std::string version;
	std::string version_rev;
	std::string version_state;
	std::string version_full;

	std::string Bits;
	std::string Platform;
	std::string Architecture;
	int macos_major; //macos major version
	int macos_minor; //macos minor version

	//additional path for user data
	std::string data_path;

private:

	bool RunEngines(std::vector<EngineContext*> &newengines);
	void CheckCamera();
	void HandleEngineShutdown();
	void HandleReload();
	void SwitchEngines();
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	bool ReportFatalError(const std::string &message);
	bool LoadQueued();

	EngineContext *active_engine;
	std::vector<EngineContext*> engines;
	HAL *hal; //hardware abstraction layer
	SkySystem *skysystem;
	GUI *gui; //GUI subsystem
	VMConsole *vmconsole; //VM console system
	Monitor *monitor; //monitor system object
	Editor *editor; //editor interface

	wxWindow *parent;

	bool first_run;
	bool RenderOnStartup; //override SBS engine setting with same name
	bool running; //true if VM has started and is currently running/rendering
	bool first_attach;

	//shared libraries
	std::vector<dylib*> dylibs;

	struct DelayLoad
	{
		std::string filename;
		bool clear;
		EngineContext *parent;
		Vector3 position;
		Vector3 rotation;
		Vector3 area_min, area_max;
		bool system;
	};
	std::vector<DelayLoad> load_queue; //delay load queue
	bool system_loaded; //true if system engines have started loaded
	bool system_finished; //true if system engines are finished loading
};

}

#endif
