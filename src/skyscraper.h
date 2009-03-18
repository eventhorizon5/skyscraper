/* $Id$ */

/*
	Skyscraper 1.3 Alpha - Simulation Frontend
	Copyright (C)2005-2009 Ryan Thoryk
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

int main (int argc, char* argv[]);
static bool SkyscraperEventHandler(iEvent& Event);

class Skyscraper : public wxApp
{
public:

	//CS Engine data
	csRef<iEngine> engine;
	csRef<iLoader> loader;
	csRef<iGraphics3D> g3d;
	csRef<iGraphics2D> g2d;
	csRef<iKeyboardDriver> kbd;
	csRef<iVirtualClock> vc;
	csRef<iView> view;
	csRef<iConsoleOutput> console;
	csRef<iFont> font;
	csRef<iVFS> vfs;
	csRef<iImageIO> imageio;
	csRef<iCommandLineParser> cmdline;
	csRef<iStringSet> strings;
	csRef<iStandardReporterListener> stdrep;
	csRef<iEventQueue> equeue;
	csRef<iBase> plug;
	csRef<iCollideSystem> collision_sys;
	csRef<iMouseDriver> mouse;
	csRef<iReporter> rep;
	csRef<FramePrinter> printer;
	csRef<iImage> image;

	//sound system
	csRef<iSndSysRenderer> sndrenderer;
	csRef<iSndSysLoader> sndloader;

	csRef<iMaterialWrapper> material;
	csRef<iSector> area;

	bool RenderOnly; //skip sim processing and only render graphics
	bool InputOnly; //skip sim processing and only run input and rendering code
	bool IsRunning;
	bool StartupRunning;

	csTicks elapsed_time, current_time;

	void PushFrame();
	virtual bool OnInit(void);
	virtual int OnExit(void);
	void DrawBackground();

	//file loader functions
	int LoadBuilding(const char * filename);
	int LoadDataFile(const char * filename);
	csString Calc(const char *expression);
	bool IfProc(const char *expression);

	//File I/O
	csString BuildingFile;
	csArray<csString> BuildingData;
	csArray<csString> UserVariable;

	//engine related stuff
	void Render();
	void GetInput();
	void Report (const char* msg, ...);
	bool ReportError (const char* msg, ...);
	void SetupFrame();
	bool HandleEvent(iEvent& Event);
	bool Initialize(int argc, const char* const argv[], wxPanel* RenderObject);
	void GetMenuInput();
	void RenderMenu();

private:
	csEventID FocusGained;
	csEventID FocusLost;
	csEventID KeyboardDown;

	//mouse status
	bool MouseDown;

	//app directory
	csString root_dir;
	csString dir_char;

	CS_DECLARE_EVENT_SHORTCUTS;

	//canvas data
	int canvas_width, canvas_height;
	wxPanel* canvas;
	csRef<iWxWindow> wxwin;

};

class MainScreen : public wxFrame
{
public:
	MainScreen();
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
