/*
	Skyscraper 2.1 - Simulation Frontend
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
class VM;
class HAL;
class GUI;

class Skyscraper : public wxApp
{
	friend class MainScreen;
	friend class VM;
	friend class HAL;

public:

	bool StartupRunning;
	bool IntroMusic;
	bool FullScreen;
	bool Verbose;
	bool ShowMenu; //show main menu
	bool Headless;

	bool Loop();
	virtual bool OnInit();
	virtual int OnExit();
	bool DrawBackground();

	Ogre::RenderWindow* CreateRenderWindow(const Ogre::NameValuePairList* miscParams = 0, const std::string& windowName = "");
	void destroyRenderWindow();
	const std::string getOgreHandle() const;
	void ShowError(const std::string &message);
	void ShowMessage(const std::string &message);
	bool GetMenuInput();
	void StartSound();
	std::string SelectBuilding();
	bool Load(const std::string &filename, EngineContext *parent = 0, const Vector3 &position = Vector3::ZERO, Real rotation = 0.0, const Vector3 &area_min = Vector3::ZERO, const Vector3 &area_max = Vector3::ZERO);
	bool Start(EngineContext *engine);
	void AllowResize(bool value);
	void UnloadToMenu();
	void Quit();
	std::string GetKeyConfigString(const std::string &key, const std::string &default_value);
	int GetJoystickConfigInt(const std::string &key, int default_value);
	void SetFullScreen(bool enabled);
	void RaiseWindow();
	void RefreshConsole();
	virtual void MacOpenFile(const wxString &filename);
	std::string GetDataPath();
	MainScreen* GetWindow();
	VM* GetVM();
	GUI* GetGUI();

	//main window
	MainScreen *window;

private:

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
	void ShowProgressDialog();
	void Report(const std::string &message);
	bool ReportError(const std::string &message);

	wxCmdLineParser *parser;

	Ogre::RenderWindow *mRenderWindow;

	//VM instance
	VM *vm;

	//GUI instance
	GUI *gui;
};

DECLARE_APP(Skyscraper)

}

#endif
