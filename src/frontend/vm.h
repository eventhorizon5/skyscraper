/*
	Skyscraper 2.1 - Nanokernel
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
#include <OgrePrerequisites.h>
#include <OgreCommon.h>
#include <Ogre.h>
#include <OgreLog.h>
#include <OgreTrays.h>

namespace Ogre {
	class GLContext;
}

namespace Skyscraper {

class Skyscraper;
class EngineContext;
class ScriptProcessor;

//Virtual Manager system
class VM
{
public:
	VM(Skyscraper *frontend);
	~VM();
	bool Initialize(const std::string &data_path);
	bool Render();
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
	Skyscraper* GetFrontend();
	void Run();
	void Run0();
	bool StartEngine(EngineContext* engine, std::vector<Ogre::Camera*> &cameras);
	::SBS::SBS* GetActiveSystem();
	ScriptProcessor* GetActiveScriptProcessor();
	void ClickedObject(bool left, bool shift, bool ctrl, bool alt, bool right, Real scale, bool center_only);
	void UnclickedObject();
	bool Load(const std::string &filename, EngineContext *parent = 0, const Vector3 &position = Vector3::ZERO, Real rotation = 0.0, const Vector3 &area_min = Vector3::ZERO, const Vector3 &area_max = Vector3::ZERO);
	Ogre::ConfigFile* LoadConfiguration(const std::string &filename, bool delete_after_use = false);
	int GetConfigInt(Ogre::ConfigFile *file, const std::string &key, int default_value);
	std::string GetConfigString(Ogre::ConfigFile *file, const std::string &key, const std::string &default_value);
	bool GetConfigBool(Ogre::ConfigFile *file, const std::string &key, bool default_value);
	Real GetConfigFloat(Ogre::ConfigFile *file, const std::string &key, Real default_value);
	void Report(const std::string &message, const std::string &prompt);
	bool ReportError(const std::string &message, const std::string &prompt);
	bool ReportFatalError(const std::string &message, const std::string &prompt);
	bool PlaySound(const std::string &filename);
	void StopSound();
	void ClearScene();
	void CreateSky(EngineContext *engine);
	void UnloadSky();
	void ToggleStats();
	void EnableStats(bool value);
	FMOD::System* GetSoundSystem();
	bool InitSky(EngineContext *engine);
	void EnableSky(bool value);
	void UpdateSky();
	inline Caelum::CaelumSystem* GetCaelumSystem() { return mCaelumSystem; };
	void SetLocation(Real latitude, Real longitude);
	void SetDateTimeNow();
	void SetDateTime(double julian_date_time);
	void GetTime(int &hour, int &minute, int &second);
	Ogre::RenderWindow* GetRenderWindow();
	std::vector<Ogre::Viewport*> mViewports;
	void Clear();
	Ogre::SceneManager* GetSceneManager();
	Ogre::RenderWindow* CreateRenderWindow(const std::string &name, int width, int height, const Ogre::NameValuePairList &params);
	void DestroyRenderWindow();
	void RefreshViewport();
	Ogre::RenderSystem* GetRenderSystem();
	
	bool Shutdown;
	bool ConcurrentLoads; //set to true for buildings to be loaded while another sim is active and rendering
	bool RenderOnStartup; //override SBS engine setting with same name
	bool CheckScript; //if set to true, checks building scripts instead of fully loading them
	bool Pause; //pause simulator
	int SkyMult; //sky time multiplier
	bool CutLandscape, CutBuildings, CutExternal, CutFloors;
	bool RTSS;
	std::string SkyName;
	std::string Renderer;
	bool DisableSound;
	std::vector<Ogre::Camera*> mCameras;
	Ogre::Root* mRoot;
	Ogre::RenderWindow* mRenderWindow;
	bool newthread;

private:

	bool RunEngines();
	void UpdateOpenXR();
	void CheckCamera();
	void HandleEngineShutdown();
	void HandleReload();
	void SwitchEngines();
	void ReInit();
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	bool ReportFatalError(const std::string &message);

	Skyscraper *frontend;
	EngineContext *active_engine;
	std::vector<EngineContext*> engines;

	//OGRE engine data
	Ogre::SceneManager* mSceneMgr;
	Ogre::OverlaySystem* mOverlaySystem;

	//sound data
	FMOD::System *soundsys;
	FMOD::Sound *sound;
	FMOD::Channel *channel;

	Caelum::CaelumSystem *mCaelumSystem;
	Ogre::LogManager* logger;
	OgreBites::TrayManager* mTrayMgr;
	int show_stats;

	bool new_location, new_time;
	Real latitude, longitude;
	double datetime;
	bool sky_error;

	//GL graphics context
	Ogre::GLContext* gl_context;

	bool first_run;
};

#endif

}
