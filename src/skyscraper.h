/* $Id$ */

/*
	Skyscraper 1.8 Alpha - Simulation Frontend
	Copyright (C)2004-2010 Ryan Thoryk
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
#include <wx/panel.h>
#include <wx/frame.h>
#include "fileio.h"

int main (int argc, char* argv[]);

class Skyscraper : public wxApp
{
public:

	//OGRE engine data
	Ogre::Root* mRoot;
	Ogre::RenderWindow* mRenderWindow;
	Ogre::Viewport* mViewport;
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;

	Ogre::String version;
	Ogre::String version_rev;
	Ogre::String version_state;
	Ogre::String version_frontend;

	Ogre::String Platform;

	bool RenderOnly; //skip sim processing and only render graphics
	bool InputOnly; //skip sim processing and only run input and rendering code
	bool IsRunning;
	bool StartupRunning;
	bool Starting;
	bool Pause; //pause simulator
	bool DisableSound;
	bool IntroMusic;
	bool FullScreen;
	bool Shutdown;
	bool Reload;
	bool PositionOverride;
	Ogre::Vector3 override_position;
	Ogre::Vector3 override_rotation;
	bool Shaders;

	unsigned int elapsed_time, current_time;

	void Loop();
	virtual bool OnInit(void);
	virtual int OnExit(void);
	void DrawBackground();

	Ogre::String BuildingFile;

	//engine related stuff
	Ogre::RenderWindow* CreateRenderWindow(const Ogre::NameValuePairList* miscParams = 0, const Ogre::String& windowName = Ogre::String(""));
	void destroyRenderWindow();
	const Ogre::String getOgreHandle() const;
	void Render();
	void GetInput();
	void Report(std::string message, ...);
	bool ReportError(std::string message, ...);
	bool Initialize(wxPanel* RenderObject);
	void GetMenuInput();
	void StartSound();
	void StopSound();
	bool SelectBuilding();
	bool Start();
	void AllowResize(bool value);
	void Unload();
	void Quit();

private:
	//mouse status
	bool MouseDown;

	//app directory
	Ogre::String root_dir;
	Ogre::String dir_char;

	//canvas data
	int canvas_width, canvas_height;
	wxPanel* canvas;

	//sound data
	//csRef<iSndSysStream> sndstream;
	//csRef<iSndSysSource> sndsource;

	//script processor
	ScriptProcessor* processor;

	//button locations
	struct buttondata
	{
		int x;
		int y;
		int size_x;
		int size_y;
		Ogre::String filename;
		Ogre::Image* button_image;
		Ogre::String filename_selected;
		Ogre::Image* selected_image;
		Ogre::String filename_pressed;
		Ogre::Image* pressed_image;
		int offset_x;
		int offset_y;
		bool drawn_selected, drawn_pressed;
	};
	buttondata button1, button2, button3, button4, button5;

	//Ogre::Image* background_image; //background image

	void DrawImage(const char *filename, buttondata *button, int x, int y, bool center, const char *filename_selected = 0, const char *filename_pressed = 0);
	void Click(int index);
	void UnloadSim();

	bool DrewButtons;
};

class MainScreen : public wxFrame
{
public:
	MainScreen(int width, int height);
	~MainScreen();
	void OnIconize(wxIconizeEvent& event);
	void OnShow(wxShowEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnClose(wxCloseEvent& event);
	void ShowWindow();
	void OnIdle(wxIdleEvent& event);
	wxPanel *panel;

	DECLARE_EVENT_TABLE()

};

DECLARE_APP(Skyscraper)

#endif
