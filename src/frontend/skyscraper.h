/*
	Skyscraper 2.0 Alpha - Simulation Frontend
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

#ifndef SKYSCRAPER_H
#define SKYSCRAPER_H

#include <wx/app.h>
#include <OgrePrerequisites.h>
#include <OgreCommon.h>
#include <Ogre.h>
#include <OgreLog.h>
#include <OgreTrays.h>

//wxWidgets definitions
class wxCmdLineParser;
class wxProgressDialog;

namespace Ogre {
	class SceneNode;
	class Rectangle2D;
	class ConfigFile;
	class OverlaySystem;
}

namespace FMOD {
	class System;
	class Sound;
	class Channel;
}

namespace Caelum {
	class CaelumSystem;
}

namespace SBS {
	class SBS;
}

int main (int argc, char* argv[]);

namespace Skyscraper {

class DebugPanel;
class MainScreen;
class EngineContext;
class Console;
class LoadDialog;
class ScriptProcessor;

class Skyscraper : public wxApp, public Ogre::LogListener
{
	friend class MainScreen;

public:

	//OGRE engine data
	Ogre::Root* mRoot;
	Ogre::RenderWindow* mRenderWindow;
	Ogre::Viewport* mViewport;
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;
	Ogre::OverlaySystem* mOverlaySystem;

	std::string version;
	std::string version_rev;
	std::string version_state;
	std::string version_frontend;

	std::string Platform;
	std::string SkyName;
	std::string Renderer;
	std::string Architecture;

	bool StartupRunning;
	bool Pause; //pause simulator
	bool DisableSound;
	bool IntroMusic;
	bool FullScreen;
	bool Shutdown;
	int SkyMult; //sky time multiplier
	bool ConcurrentLoads; //set to true for buildings to be loaded while another sim is active and rendering
	bool RenderOnStartup; //override SBS engine setting with same name
	bool CutLandscape, CutBuildings, CutExternal, CutFloors;
	bool Verbose;
	bool ShowMenu; //show main menu
	bool CheckScript;
	bool Headless;
	bool RTSS;
	int macos_major; //macos major version
	int macos_minor; //macos minor version

	bool Loop();
	virtual bool OnInit(void);
	virtual int OnExit(void);
	bool DrawBackground();

	Ogre::RenderWindow* CreateRenderWindow(const Ogre::NameValuePairList* miscParams = 0, const std::string& windowName = "");
	void destroyRenderWindow();
	const std::string getOgreHandle() const;
	bool Render();
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	bool ReportFatalError(const std::string &message);
	void ShowError(const std::string &message);
	void ShowMessage(const std::string &message);
	bool Initialize();
	bool GetMenuInput();
	void StartSound();
	void StopSound();
	std::string SelectBuilding();
	bool Load(const std::string &filename, EngineContext *parent = 0, const Vector3 &position = Vector3::ZERO, Real rotation = 0.0, const Vector3 &area_min = Vector3::ZERO, const Vector3 &area_max = Vector3::ZERO);
	bool Start(EngineContext *engine);
	void AllowResize(bool value);
	void UnloadToMenu();
	void Quit();
	int GetConfigInt(const std::string &key, int default_value);
	std::string GetConfigString(const std::string &key, const std::string &default_value);
	bool GetConfigBool(const std::string &key, bool default_value);
	Real GetConfigFloat(const std::string &key, Real default_value);
	std::string GetKeyConfigString(const std::string &key, const std::string &default_value);
	bool InitSky(EngineContext *engine);
	void ShowConsole(bool send_button = true);
	void CreateProgressDialog(const std::string &message);
	void CloseProgressDialog();
	void UpdateProgress();
	void SetFullScreen(bool enabled);
	inline Caelum::CaelumSystem* GetCaelumSystem() { return mCaelumSystem; };
	void SetLocation(Real latitude, Real longitude);
	void SetDateTimeNow();
	void SetDateTime(double julian_date_time);
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
	void RaiseWindow();
	void RefreshConsole();
	void RefreshViewport();
	bool IsValidEngine(EngineContext *engine);
	bool IsValidSystem(::SBS::SBS *sbs);
	int RegisterEngine(EngineContext *engine);
	EngineContext* GetFirstValidEngine();
	void EnableSky(bool value);
	void UpdateSky();
	void UnregisterDebugPanel() { dpanel = 0; }
	virtual void MacOpenFile(const wxString &filename);
	void UnloadSky();
	void CreateSky(EngineContext* engine);
	void ToggleStats();
	void EnableStats(bool value);
	std::string GetDataPath();

private:
	//sound data
	FMOD::System *soundsys;
	FMOD::Sound *sound;
	FMOD::Channel *channel;

	//button locations
	struct buttondata
	{
		Real x;
		Real y;
		Real size_x;
		Real size_y;
		std::string filename;
		std::string filename_selected;
		std::string filename_pressed;
		Real offset_x;
		Real offset_y;
		bool drawn_selected, drawn_pressed;
		int active_button;
		Ogre::SceneNode* node;
		Ogre::Rectangle2D* rect;
	};
	buttondata *buttons;
	int buttoncount;

	Ogre::Rectangle2D* background_rect;
	Ogre::SceneNode* background_node;
	std::string background_image;
	bool DrawImage(const std::string &filename, buttondata *button, Real x, Real y, bool center, const std::string &filename_selected = "", const std::string &filename_pressed = "");
	void Click(int index);
	void UnloadSim();
	void DeleteButtons();
	void messageLogged(const std::string &message, Ogre::LogMessageLevel lml, bool maskDebug, const std::string &logName, bool &skipThisMessage);
	bool RunEngines();
	void SwitchEngines();
	void HandleEngineShutdown();
	void HandleReload();
	int GetFreeInstanceNumber();
	void ShowProgressDialog();
	void ReInit();

	Ogre::ConfigFile *configfile;
	Ogre::ConfigFile *keyconfigfile;
	Caelum::CaelumSystem *mCaelumSystem;
	Ogre::LogManager* logger;
	bool showconsole;
	wxProgressDialog *progdialog;
	wxCmdLineParser *parser;
	OgreBites::TrayManager* mTrayMgr;
	int show_stats;

	//control panel
	DebugPanel *dpanel;

	//main window
	MainScreen *window;

	//console window
	Console *console;

	//load dialog window
	LoadDialog *loaddialog;

	//progress dialog initial data
	bool show_progress;
	std::string prog_text;

	bool new_location, new_time;
	Real latitude, longitude;
	double datetime;
	bool sky_error;

	//additional path for user data
	std::string data_path;

	EngineContext *active_engine;
	std::vector<EngineContext*> engines;
};

DECLARE_APP(Skyscraper)

}

#endif
