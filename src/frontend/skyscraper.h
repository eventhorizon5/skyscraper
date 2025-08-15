/*
	Skyscraper 2.1 - Simulation Frontend
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

#ifndef SKYSCRAPER_H
#define SKYSCRAPER_H

#ifndef USING_WX
#include <filesystem>
#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRTShaderSystem.h"
#include "OgreCameraMan.h"
#endif

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

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
	std::string settingsPath();
#endif

namespace Skyscraper {

class DebugPanel;
class MainScreen;
class EngineContext;
class Console;
class LoadDialog;
class ScriptProcessor;
class VM;
class HAL;
class GUI;
class StartScreen;

#ifdef USING_WX
class Skyscraper : public wxApp
#else
class Skyscraper : public OgreBites::ApplicationContext, public OgreBites::InputListener
#endif
{
	friend class MainScreen;
	friend class VM;
	friend class HAL;

public:

	bool StartupRunning;
	bool IntroMusic;
	bool FullScreen;
	bool Maximized;
	bool Verbose;
	bool ShowMenu; //show main menu
	int wireframe;

	bool Loop();
	virtual bool OnInit();
	virtual int OnExit();

#ifdef USING_WX
	Skyscraper() {};
#else
	Skyscraper();
#endif
	virtual ~Skyscraper() {};
	void setup();

	Ogre::RenderWindow* CreateRenderWindow(const Ogre::NameValuePairList* miscParams = 0, const std::string& windowName = "");
	void destroyRenderWindow();
	const std::string getOgreHandle() const;
	void ShowError(const std::string &message);
	void ShowMessage(const std::string &message);
	void StartSound();
	std::string SelectBuilding(bool native_dialog = false);
	bool Load(const std::string &filename, EngineContext *parent = 0, const Vector3 &position = Vector3::ZERO, const Vector3 &rotation = Vector3::ZERO, const Vector3 &area_min = Vector3::ZERO, const Vector3 &area_max = Vector3::ZERO);
	bool Start(EngineContext *engine);
	void AllowResize(bool value);
	void UnloadToMenu();
	void Quit();
	std::string GetKeyConfigString(const std::string &key, const std::string &default_value);
	int GetJoystickConfigInt(const std::string &key, int default_value);
	void SetFullScreen(bool enabled);
	void RaiseWindow();
	void RefreshConsole();
	bool SetCWD();
#ifdef USING_WX
	virtual void MacOpenFile(const wxString &filename);
#else
	std::filesystem::path GetExeDirectory();
	bool keyPressed(const OgreBites::KeyboardEvent& evt);
	bool keyReleased(const OgreBites::KeyboardEvent& evt);
	bool mouseMoved(const OgreBites::MouseMotionEvent &evt);
	bool mousePressed(const OgreBites::MouseButtonEvent &evt);
	bool mouseReleased(const OgreBites::MouseButtonEvent &evt);
	bool mouseWheelRolled(const OgreBites::MouseWheelEvent &evt);
	bool touchMoved(const OgreBites::TouchFingerEvent &evt);
	bool touchPressed(const OgreBites::TouchFingerEvent &evt);
	bool touchReleased(const OgreBites::TouchFingerEvent &evt);
	bool alt_down, ctrl_down, shift_down;
#endif
	std::string GetDataPath();
	MainScreen* GetWindow();
	VM* GetVM();
#ifdef USING_WX
	GUI* GetGUI();
#endif
	void StopMenu();

	//main window
	MainScreen *window;

protected:
	//input system states
	bool colliders, boxes;
	bool strafe_left, strafe_right;
	bool float_up, float_down;
	bool spin_up, spin_down;
	bool turn_left, turn_right;
	bool look_up, look_down;
	bool step_forward, step_backward;
	bool freelook;

private:
	//loop states
	bool startscreen_loaded;

	void UnloadSim();
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
#ifndef USING_WX
	void GetKeyStates(EngineContext *engine, OgreBites::Keycode &key, bool down);
	void ProcessMovement(EngineContext *engine, bool control = false, bool shift = false, bool angle_only = false);
	void HandleMouseMovement();
	void EnableFreelook(bool value);
#else
	wxCmdLineParser *parser;
#endif
	Ogre::RenderWindow *mRenderWindow;

	//VM instance
	VM *vm;

	//GUI instance
#ifdef USING_WX
	GUI *gui;
#endif

	//Start Screen instance
	StartScreen *startscreen;
};

#ifdef USING_WX
DECLARE_APP(Skyscraper)
#endif
}

#endif
