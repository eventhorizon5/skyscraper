/* $Id$ */

/*
	Skyscraper 1.10 Alpha - Simulation Frontend
	Copyright (C)2004-2016 Ryan Thoryk
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

#ifndef SKYSCRAPER_H
#define SKYSCRAPER_H

#include <wx/app.h>
#include <wx/frame.h>
#include <wx/progdlg.h>
#include <OgreLog.h>

#if OGRE_VERSION >= 0x00010900
#include <OgreOverlaySystem.h>
#endif

namespace Ogre {
	class SceneNode;
	class Rectangle2D;
}

namespace FMOD {
	class System;
	class Sound;
	class Channel;
}

namespace Caelum {
	class CaelumSystem;
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
#if OGRE_VERSION >= 0x00010900
	Ogre::OverlaySystem* mOverlaySystem;
#endif

	std::string version;
	std::string version_rev;
	std::string version_state;
	std::string version_frontend;

	std::string Platform;
	std::string SkyName;
	std::string Renderer;

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

	void Loop();
	virtual bool OnInit(void);
	virtual int OnExit(void);
	void DrawBackground();

	//engine related stuff
	Ogre::RenderWindow* CreateRenderWindow(const Ogre::NameValuePairList* miscParams = 0, const std::string& windowName = "");
	void destroyRenderWindow();
	const std::string getOgreHandle() const;
	void Render();
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	bool ReportFatalError(const std::string &message);
	void ShowError(const std::string &message);
	bool Initialize();
	void GetMenuInput();
	void StartSound();
	void StopSound();
	std::string SelectBuilding();
	bool Load(const std::string &filename, EngineContext *parent = 0, const Ogre::Vector3 &position = Ogre::Vector3::ZERO, float rotation = 0.0f, const Ogre::Vector3 &area_min = Ogre::Vector3::ZERO, const Ogre::Vector3 &area_max = Ogre::Vector3::ZERO);
	bool Start(EngineContext *engine);
	void AllowResize(bool value);
	void UnloadToMenu();
	void Quit();
	int GetConfigInt(const std::string &key, int default_value);
	std::string GetConfigString(const std::string &key, const std::string &default_value);
	bool GetConfigBool(const std::string &key, bool default_value);
	float GetConfigFloat(const std::string &key, float default_value);
	bool InitSky(EngineContext *engine);
	void ShowConsole(bool send_button = true);
	void CreateProgressDialog(const std::string &message);
	void CloseProgressDialog();
	void UpdateProgress();
	void SetFullScreen(bool enabled);
	inline Caelum::CaelumSystem* GetCaelumSystem() { return mCaelumSystem; };
	void SetLocation(float latitude, float longitude);
	void SetDateTime(double julian_date_time);
	EngineContext* GetActiveEngine() { return active_engine; }
	EngineContext* GetEngine(int number);
	EngineContext* CreateEngine(EngineContext *parent = 0, const Ogre::Vector3 &position = Ogre::Vector3::ZERO, float rotation = 0.0f, const Ogre::Vector3 &area_min = Ogre::Vector3::ZERO, const Ogre::Vector3 &area_max = Ogre::Vector3::ZERO);
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
	void UnregisterDebugPanel() { dpanel = 0; }

private:
	//sound data
	FMOD::System *soundsys;
	FMOD::Sound *sound;
	FMOD::Channel *channel;

	//button locations
	struct buttondata
	{
		float x;
		float y;
		float size_x;
		float size_y;
		std::string filename;
		std::string filename_selected;
		std::string filename_pressed;
		float offset_x;
		float offset_y;
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
	void DrawImage(const std::string &filename, buttondata *button, float x, float y, bool center, const std::string &filename_selected = "", const std::string &filename_pressed = "");
	void Click(int index);
	void UnloadSim();
	void DeleteButtons();
	void messageLogged(const Ogre::String &message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName, bool &skipThisMessage);
	bool RunEngines();
	void SwitchEngines();
	void HandleEngineShutdown();
	void HandleReload();
	int GetFreeInstanceNumber();

	Ogre::ConfigFile *configfile;
	Caelum::CaelumSystem *mCaelumSystem;
	Ogre::LogManager* logger;
	bool showconsole;
	wxProgressDialog *progdialog;

	//control panel
	DebugPanel *dpanel;

	//main window
	MainScreen *window;

	//console window
	Console *console;

	//load dialog window
	LoadDialog *loaddialog;

	bool new_location, new_time;
	float latitude, longitude;
	double datetime;

	EngineContext *active_engine;
	std::vector<EngineContext*> engines;
};

DECLARE_APP(Skyscraper)

}

#endif
