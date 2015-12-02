/* $Id$ */

/*
	Skyscraper 1.10 Alpha - Simulation Frontend
	Copyright (C)2004-2015 Ryan Thoryk
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
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <fmod.hpp>
#include "Caelum.h"
#include "fileio.h"
#include "debugpanel.h"

#if OGRE_VERSION >= 0x00010900
#include <OgreOverlaySystem.h>
#endif

int main (int argc, char* argv[]);

namespace Skyscraper {

class EngineContext;

class Skyscraper : public wxApp, public Ogre::LogListener
{
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

	bool IsRunning;
	bool IsLoading;
	bool StartupRunning;
	bool Pause; //pause simulator
	bool DisableSound;
	bool IntroMusic;
	bool FullScreen;
	bool Shutdown;
	bool Reload;
	bool PositionOverride;
	Ogre::Vector3 override_position;
	Ogre::Vector3 override_rotation;
	int override_floor;
	bool override_collisions;
	bool override_gravity;
	bool override_freelook;
	int SkyMult; //sky time multiplier

	unsigned int current_time;

	void Loop();
	virtual bool OnInit(void);
	virtual int OnExit(void);
	void DrawBackground();

	std::string BuildingFile;

	//engine related stuff
	Ogre::RenderWindow* CreateRenderWindow(const Ogre::NameValuePairList* miscParams = 0, const std::string& windowName = "");
	void destroyRenderWindow();
	const std::string getOgreHandle() const;
	void Render();
	void GetInput();
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	bool ReportFatalError(const std::string &message);
	void ShowError(const std::string &message);
	bool Initialize();
	void GetMenuInput();
	void StartSound();
	void StopSound();
	bool SelectBuilding();
	bool Load();
	bool Start();
	void AllowResize(bool value);
	void Unload();
	void Quit();
	int GetConfigInt(const std::string &key, int default_value);
	std::string GetConfigString(const std::string &key, const std::string &default_value);
	bool GetConfigBool(const std::string &key, bool default_value);
	float GetConfigFloat(const std::string &key, float default_value);
	bool InitSky();
	void ShowConsole(bool send_button = true);
	void CreateProgressDialog();
	void CloseProgressDialog();
	void UpdateProgress(int percent);
	void SetFullScreen(bool enabled);
	inline Caelum::CaelumSystem* GetCaelumSystem() { return mCaelumSystem; };
	void SetLocation(float latitude, float longitude);
	void SetDateTime(double julian_date_time);
	EngineContext* GetActiveEngine() { return active_engine; }
	void CreateEngine();
	bool DeleteEngine(EngineContext *engine);

private:
	//mouse status
	bool MouseDown;

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

	Ogre::ConfigFile configfile;
	Caelum::CaelumSystem *mCaelumSystem;
	Ogre::LogManager* logger;
	bool showconsole;
	bool raised;
	wxProgressDialog *progdialog;

	bool new_location, new_time;
	float latitude, longitude;
	double datetime;

	EngineContext *active_engine;
	std::vector<EngineContext*> engines;
};

class MainScreen : public wxFrame
{
public:
	MainScreen(int width, int height);
	virtual ~MainScreen();
	void OnIconize(wxIconizeEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnClose(wxCloseEvent& event);
	void ShowWindow();
	void OnIdle(wxIdleEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnActivate(wxActivateEvent & event);
	void OnEnterWindow(wxMouseEvent& event);
	void OnLeaveWindow(wxMouseEvent& event);
	bool Active;
	bool InLoop;

	DECLARE_EVENT_TABLE()
};

class EngineContext
{
public:

	EngineContext(Ogre::SceneManager* mSceneManager, FMOD::System *fmodsystem);
	~EngineContext();
	ScriptProcessor* GetScriptProcessor();
	SBS::SBS *GetSystem() { return Simcore; }

private:

	ScriptProcessor* processor; //script processor
	SBS::SBS *Simcore; //sim engine instance

};

extern Skyscraper *skyscraper;
extern DebugPanel *dpanel;

DECLARE_APP(Skyscraper)

}

#endif
