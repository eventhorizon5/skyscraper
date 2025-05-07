/*
	Skyscraper 2.1 - Simulation Frontend
	Copyright (C)2003-2025 Ryan Thoryk
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

#ifdef USING_WX
#include "wx/wx.h"
#include "wx/dir.h"
#include "wx/cmdline.h"
#include "wx/filename.h"
#include "wx/filefn.h"
#include "wx/stdpaths.h"
#include "wx/joystick.h"
#else
#include <filesystem>
#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRTShaderSystem.h"
#include "OgreCameraMan.h"
#endif
#include <locale>
#include <time.h>
#include <thread>
#include "globals.h"
#include "sbs.h"
#include "camera.h"
#include "skyscraper.h"
#include "vm.h"
#include "gui.h"
#include "hal.h"
#include "sky.h"
#include "enginecontext.h"
#include "scriptproc.h"
#ifdef USING_WX
#include "mainscreen.h"
#endif
#include "profiler.h"
#include "startscreen.h"
#include "gitrev.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <sysdir.h>  // for sysdir_start_search_path_enumeration
#include <glob.h>    // for glob needed to expand ~ to user dir
#include <stdio.h>
#include <sys/sysctl.h>
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include <windows.h>
#endif

#if defined(__WXGTK__)
   // NOTE: Find the GTK install config with `pkg-config --cflags gtk+-2.0`
   #include "gtk/gtk.h"
   #include "gdk/gdk.h"
   #include "gdk/gdkx.h"
   #include "GL/glx.h"
#endif

using namespace SBS;

#ifdef USING_WX
namespace Skyscraper {

IMPLEMENT_APP_NO_MAIN(Skyscraper)

}
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#if OGRE_CPU != OGRE_CPU_ARM
#include "uexception.h"
#endif
#endif

#ifndef SW_SHOWNORMAL
	#define SW_SHOWNORMAL 1
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE

//code to get Application Support folder on Mac

std::string expandTilde(const char* str) {
    if (!str) return {};

    glob_t globbuf;
    if (glob(str, GLOB_TILDE, nullptr, &globbuf) == 0) {
        std::string result(globbuf.gl_pathv[0]);
        globfree(&globbuf);
        return result;
    } else {
        throw "Unable to expand tilde";
    }
}

std::string settingsPath() {
    char path[PATH_MAX];
    auto state = sysdir_start_search_path_enumeration(SYSDIR_DIRECTORY_APPLICATION_SUPPORT,
                                                      SYSDIR_DOMAIN_MASK_USER);
    if ((state = sysdir_get_next_search_path_enumeration(state, path))) {
        return expandTilde(path);
    } else {
        throw "Failed to get settings folder";
    }
}

#endif

namespace Skyscraper {

#ifndef USING_WX
Skyscraper::Skyscraper() : OgreBites::ApplicationContext("Skyscraper")
{
}

void Skyscraper::setup()
{
	OnInit();
}
#endif

bool Skyscraper::OnInit()
{
	StartupRunning = false;
	FullScreen = false;
	ShowMenu = false;
	mRenderWindow = 0;
	window = 0;
#ifdef USING_WX
	parser = 0;
#endif

	//create VM instance
	vm = new VM();

	vm->version = "2.1";
	vm->version_rev = ToString(GIT_REV);
	vm->version_state = "Alpha";
	vm->version_frontend = vm->version + ".0." + vm->version_rev;

#ifdef USING_WX
	gui = vm->GetGUI();
#endif
	startscreen = new StartScreen(this);

#ifdef USING_WX
	//switch current working directory to executable's path, if needed
	wxString exefile = wxStandardPaths::Get().GetExecutablePath(); //get full path and filename
	wxString app_path = wxPathOnly(exefile); //strip off filename
#if defined(__WXMAC__)
	vm->data_path = settingsPath() + "/Skyscraper/"; //Application Support folder

	if (!wxDirExists(vm->data_path))
		wxMkdir(vm->data_path);

	if (!wxDirExists(vm->data_path + wxT("buildings")))
		wxMkdir(vm->data_path + wxT("buildings"));
	if (!wxDirExists(vm->data_path + wxT("data")))
		wxMkdir(vm->data_path + wxT("data"));
	if (!wxDirExists(vm->data_path + wxT("screenshots")))
		wxMkdir(vm->data_path + wxT("screenshots"));

	wxSetWorkingDirectory(app_path + wxT("/../Resources")); //set working directory to resources folder on Mac

	if (!wxFileExists(vm->data_path + wxT("skyscraper.ini")))
		wxCopyFile("skyscraper.ini", vm->data_path + wxT("skyscraper.ini"));
	if (!wxFileExists(vm->data_path + wxT("keyboard.ini")))
		wxCopyFile("keyboard.ini", vm->data_path + wxT("keyboard.ini"));
	if (!wxFileExists(vm->data_path + wxT("joystick.ini")))
		wxCopyFile("joystick.ini", vm->data_path + wxT("joystick.ini"));

#elif defined (__WXGTK__)
	wxSetWorkingDirectory(app_path + wxT("/../")); //set working directory parent directory
#else
	wxSetWorkingDirectory(app_path); //set working directory
#endif

	//define command line options
	static const wxCmdLineEntryDesc cmdLineDesc[] =
	{
		{ wxCMD_LINE_SWITCH, "h", "help", "show this help message",
			wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },

		{ wxCMD_LINE_SWITCH, "c", "no-console", "hide the console",
			wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },

		{ wxCMD_LINE_SWITCH, "f", "fullscreen", "start up in full-screen mode",
			wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },

		{ wxCMD_LINE_SWITCH, "k", "check-script", "quickly check building script, and exit after",
			wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },

		{ wxCMD_LINE_SWITCH, "m", "no-menu", "hide the main menu",
			wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },

		{ wxCMD_LINE_SWITCH, "M", "no-music", "disable the intro music",
			wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },

		{ wxCMD_LINE_SWITCH, "p", "no-panel", "hide the control panel",
			wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },

		{ wxCMD_LINE_SWITCH, "v", "verbose", "enable verbose mode",
			wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },

		{ wxCMD_LINE_SWITCH, "V", "version", "show version",
			wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },

		{ wxCMD_LINE_PARAM, NULL, NULL, "building filename",
			wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },

		{ wxCMD_LINE_NONE }
	};

	//set up command line parser
	parser = new wxCmdLineParser(cmdLineDesc, argc, argv);

	//process command line options
	switch (parser->Parse())
	{
		case -1:
			return false; //help was given, exit
		case 0:
			break; //everything is good, continue
		default:
			return false; //exit if parameters are incorrect
	}

	//only run idle events on specified windows, to reduce overhead
	wxIdleEvent::SetMode(wxIDLE_PROCESS_SPECIFIED);
#else
	//set working directory
	std::string path = GetExeDirectory();
#ifdef __APPLE__
	path = path +  "../../../";
#elif __LINUX__
	path = path + "../";
#endif
	std::filesystem::current_path(path);
#endif

	//set up unhandled exception handler (crash report system)
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#if OGRE_CPU != OGRE_CPU_ARM
	UnhandledExceptionSetRoot(this);
#endif
#endif

	//set locale to default for conversion functions
#ifdef OGRE_DEFAULT_LOCALE
	setlocale(LC_ALL, OGRE_DEFAULT_LOCALE);
#else
	setlocale(LC_ALL, "C");
#endif

bool showconsole = true;
#ifdef USING_WX
	//show version number and exit if specified
	if (parser->Found(wxT("version")) == true)
	{
		printf("Skyscraper version %s\n", vm->version_frontend.c_str());
		return false;
	}

	//set verbose mode if specified
	if (parser->Found(wxT("verbose")) == true)
		vm->Verbose = true;

	//set CheckScript mode if specified
	if (parser->Found(wxT("check-script")) == true)
		vm->CheckScript = true;

	//turn off console if specified on command line
	if (parser->Found(wxT("no-console")) == true)
		showconsole = false;
#endif

	//load config files
	HAL *hal = vm->GetHAL();
	hal->LoadConfiguration(vm->data_path, showconsole);

#ifdef USING_WX
	//Create main window and set size from INI file defaults
	window = new MainScreen(this, hal->GetConfigInt(hal->configfile, "Skyscraper.Frontend.Menu.Width", 800), hal->GetConfigInt(hal->configfile, "Skyscraper.Frontend.Menu.Height", 600));
	//AllowResize(false);
	window->ShowWindow();
	window->CenterOnScreen();

	vm->SetParent(window);
#endif

	//initialize Ogre
	OgreBites::ApplicationContext::setup();
	addInputListener(this);

	//get overlay system if already created
	Ogre::OverlaySystem *overlay = getOverlaySystem();

	//start and initialize abstraction layer
#ifdef USING_WX
	if (!hal->Initialize(vm->data_path))
#else
	if (!hal->Initialize(vm->data_path, getRoot(), overlay))
#endif
		return ReportError("Error initializing HAL");

	Report("");
	Report("Creating render window...");
	mRenderWindow = CreateRenderWindow(0, "SkyscraperVR");

	//load system
	if (!hal->LoadSystem(vm->data_path, mRenderWindow))
		return ReportError("Error loading system");

#ifndef __FreeBSD__
#ifdef USING_WX
	//set up joystick if available
	wxJoystick joystick(wxJOYSTICK1);
	if (!joystick.IsOk())
		Report("No joystick detected");
	else
	{
		Report("");
		Report("Joystick detected: " + joystick.GetProductName().ToStdString());
		Report("");
	}
#endif
#endif
	vm->ShowPlatform();

	//autoload a building file if specified
	std::string filename;
#ifdef USING_WX
	if (parser->GetParamCount() > 0)
	{
		filename = parser->GetParam(0).ToStdString();

		//strip path from filename
		wxFileName file (filename);
		filename = file.GetFullName();
	}
	else
		filename = hal->GetConfigString(hal->configfile, "Skyscraper.Frontend.AutoLoad", "");

	ShowMenu = hal->GetConfigBool(hal->configfile, "Skyscraper.Frontend.Menu.Show", true);

	//turn off menu if specified on command line
	if (parser->Found(wxT("no-menu")) == true)
		ShowMenu = false;

	if (hal->GetConfigBool(hal->configfile, "Skyscraper.Frontend.VR", false) == true)
		ShowMenu = false;
#else
	ShowMenu = false;

#endif

	//start console
	vm->StartConsole();
#ifdef USING_WX
	gui->EnableConsole(true);
#endif

	if (filename != "")
		return Load(filename);

	if (ShowMenu == true)
	{
		StartupRunning = true;
		StartSound();
	}
	else
	{
		//or show building selection window if ShowMenu is false
		return Load(SelectBuilding());
	}

	return true;
}

int Skyscraper::OnExit()
{
	//clean up

	//cleanup
	Report("Cleaning up...");

	UnloadSim();

	//cleanup sound
	vm->GetHAL()->StopSound();

#ifdef USING_WX
	if (window)
		window->Destroy();
	window = 0;

	if (parser)
		delete parser;
	parser = 0;
#endif

	delete startscreen;
	delete vm;

#ifdef USING_WX
	return wxApp::OnExit();
#else
	return 0;
#endif
}

void Skyscraper::UnloadSim()
{
	//unload GUI
#ifdef USING_WX
	gui->Unload();
#endif

	//unload sky system
	vm->GetSkySystem()->UnloadSky();

	//delete all sim engines
	vm->DeleteEngines();

	//clear scene
	vm->GetHAL()->Clear();
}

bool Skyscraper::Loop()
{
	//Main simulator loop

	ProfileManager::Reset();
	ProfileManager::Increment_Frame_Counter();

	if (vm->unloaded == true)
	{
		UnloadSim();
		vm->unloaded = false;
		return true;
	}

	vm->ProcessConsole();

	//main menu routine
	if (StartupRunning == true)
	{
		bool result = false;

		result = startscreen->DrawBackground();

		if (result == false)
			return false;

		result = startscreen->GetMenuInput();

		if (result == false)
			return false;

		if (vm->loadstart == true)
		{
			StopMenu();
			vm->loadstart = false;
			return true;
		}

		//have HAL render frame
		return vm->GetHAL()->Render();
	}

#ifdef USING_WX
	gui->ShowProgress();
#endif

	//run sim engine instances
	std::vector<EngineContext*> newengines;
	int status = vm->Run(newengines);

	//start a new engine if needed
	if (status == 3)
	{
		for (size_t i = 0; i < newengines.size(); i++)
		{
			Start(newengines[i]);
		}
	}

	if (status == 2 ||  status == -1)
		UnloadToMenu();

	//ProfileManager::dumpAll();

	return true;
}

void Skyscraper::StartSound()
{
	//load and start background music

	if (vm->GetHAL()->DisableSound == true)
		return;

	if (vm->GetHAL()->GetConfigBool(vm->GetHAL()->configfile, "Skyscraper.Frontend.IntroMusic", true) == false)
		return;

#ifdef USING_WX
	if (parser->Found(wxT("no-music")) == true)
		return;
#endif

	std::string filename = vm->GetHAL()->GetConfigString(vm->GetHAL()->configfile, "Skyscraper.Frontend.IntroMusicFile", "intro.ogg");
	std::string filename_full = "data/" + filename;

#ifdef USING_WX
	//check for an intro sound file in the data path location instead
	if (wxFileExists(vm->data_path + filename_full))
		filename_full = vm->data_path + filename_full;
#endif

	//play music
	vm->GetHAL()->PlaySound(filename_full);
}

std::string Skyscraper::SelectBuilding()
{
#ifdef USING_WX
	return gui->SelectBuilding(vm->data_path);
#else
	return "";
#endif
}

bool Skyscraper::Load(const std::string &filename, EngineContext *parent, const Vector3 &position, Real rotation, const Vector3 &area_min, const Vector3 &area_max)
{
	//load simulator and data file

	StopMenu();

	bool result = vm->Load(true, filename, parent, position, rotation, area_min, area_max);

	if (result == false && vm->GetEngineCount() == 1)
		UnloadToMenu();

	return result;
}

void Skyscraper::StopMenu()
{
	if (StartupRunning == true)
	{
		startscreen->DeleteButtons();
		StartupRunning = false;
	}
}

bool Skyscraper::Start(EngineContext *engine)
{
	//start simulator

	if (!engine)
		return false;

	::SBS::SBS *Simcore = engine->GetSystem();
	HAL *hal = vm->GetHAL();

	if (engine == vm->GetActiveEngine())
	{
		//the sky needs to be created before Prepare() is called
		vm->GetSkySystem()->CreateSky(engine);

		//switch to fullscreen mode if specified
		bool fullscreen = hal->GetConfigBool(hal->configfile, "Skyscraper.Frontend.FullScreen", false);

#ifdef USING_WX
		//override fullscreen setting if specified on the command line
		if (parser->Found(wxT("fullscreen")) == true)
			fullscreen = true;
#endif

		if (fullscreen == true)
			SetFullScreen(true);

#ifdef USING_WX
		//resize main window
		if (FullScreen == false)
		{
#if !defined(__WXMAC__)
			window->SetBackgroundColour(*wxBLACK);
#endif
			window->SetClientSize(hal->GetConfigInt(hal->configfile, "Skyscraper.Frontend.ScreenWidth", 1024), hal->GetConfigInt(hal->configfile, "Skyscraper.Frontend.ScreenHeight", 768));
			window->Center();
		}
#endif
	}

	//start simulation
	if (!vm->StartEngine(engine, hal->mCameras))
		return false;

	//close progress dialog if no engines are loading
#ifdef USING_WX
	if (vm->IsEngineLoading() == false)
		gui->CloseProgressDialog();
#endif

	//load control panel
	if (engine == vm->GetActiveEngine())
	{
		bool panel = vm->GetHAL()->GetConfigBool(hal->configfile, "Skyscraper.Frontend.ShowControlPanel", true);

#ifdef USING_WX
		//override if disabled on the command line
		if (parser->Found(wxT("no-panel")) == true)
			panel = false;

		if (panel == true)
			gui->CreateDebugPanel();
#endif
	}

	hal->RefreshViewport();

	//set ambient light
	if (hal->GetConfigBool(hal->configfile, "Skyscraper.SBS.Lighting", false) == true)
	{
		Real value = hal->GetConfigFloat(hal->configfile, "Skyscraper.SBS.AmbientLight", 0.5);
		hal->GetSceneManager()->setAmbientLight(Ogre::ColourValue(value, value, value));
	}

	//show frame stats
	hal->EnableStats(hal->GetConfigBool(hal->configfile, "Skyscraper.Frontend.Stats", true));

	//run simulation
	Report("Running simulation...");
	hal->StopSound();

	return true;
}

void Skyscraper::AllowResize(bool value)
{
	//changes the window style to either allow or disallow resizing

#ifdef USING_WX
	if (value)
		window->SetWindowStyleFlag(wxDEFAULT_FRAME_STYLE | wxMAXIMIZE);
	else
		window->SetWindowStyleFlag(wxDEFAULT_FRAME_STYLE & ~wxMAXIMIZE);
	window->Refresh();
#endif
}

void Skyscraper::UnloadToMenu()
{
	//unload to main menu

	//exit app if ShowMenu is false
	if (ShowMenu == false)
		Quit();

	vm->Pause = false;
	UnloadSim();

	//cleanup sound
	vm->GetHAL()->StopSound();

#ifdef USING_WX
	gui->CloseProgressDialog();
#endif

	//return to main menu
	SetFullScreen(false);
#ifdef USING_WX
	window->SetClientSize(vm->GetHAL()->GetConfigInt(vm->GetHAL()->configfile, "Skyscraper.Frontend.Menu.Width", 800), vm->GetHAL()->GetConfigInt(vm->GetHAL()->configfile, "Skyscraper.Frontend.Menu.Height", 600));
	window->Center();
	window->SetCursor(wxNullCursor);
#endif
	vm->ConcurrentLoads = false;
	vm->SetRenderOnStartup(false);

	StartSound();
	StartupRunning = true;
}

void Skyscraper::Quit()
{
	//exit app

#ifdef USING_WX
	gui->EnableTimer(false);

	wxGetApp().Exit();
#else
	OnExit();
#endif
}

Ogre::RenderWindow* Skyscraper::CreateRenderWindow(const Ogre::NameValuePairList* miscParams, const std::string& windowName)
{
#ifdef USING_WX
	std::string name = windowName;

	int width, height;
	window->GetClientSize(&width, &height);

	Ogre::NameValuePairList params;
	if (miscParams)
		params = *miscParams;

	bool vsync = vm->GetHAL()->GetConfigBool(vm->GetHAL()->configfile, "Skyscraper.Frontend.Vsync", true);
	if (vsync == true)
		params["vsync"] = "true";
	else
		params["vsync"] = "false";
	params["vsyncInterval"] = "1";
	params["externalWindowHandle"] = getOgreHandle();

#if defined(__WXMAC__)
	params["macAPI"] = "cocoa";
	params["macAPICocoaUseNSView"] = "true";
#endif

	return vm->GetHAL()->CreateRenderWindow(name, width, height, params);
#else
	return 0;
#endif
}

void Skyscraper::destroyRenderWindow()
{
	vm->GetHAL()->DestroyRenderWindow();
}

const std::string Skyscraper::getOgreHandle() const
{
#ifdef USING_WX
#if defined(__WXMSW__)
	// Handle for Windows systems
	return Ogre::StringConverter::toString((size_t)((HWND)window->panel->GetHandle()));
#elif defined(__WXGTK__)
	// Handle for GTK-based systems

	// wxWidgets uses several internal GtkWidgets, the GetHandle method
	// returns a different one than this, but wxWidgets's GLCanvas uses this
	// one to interact with GLX, so we do the same.
	// NOTE: this method relies on implementation details in wxGTK and could
	//      change without any notification from the developers.
	GtkWidget* privHandle = window->m_wxwindow;

	// prevents flickering
	//gtk_widget_set_double_buffered(privHandle, false);

	gtk_widget_realize(privHandle);

	// grab the window object
	GdkWindow* gdkWin = gtk_widget_get_window((GtkWidget*)window->GetHandle());
	Display* display = GDK_WINDOW_XDISPLAY(gdkWin);
	Window wid = GDK_WINDOW_XID(gdkWin);

	// screen (returns "display.screen")
	std::string screenStr = DisplayString(display);
	screenStr = screenStr.substr(screenStr.find(".") + 1, screenStr.size());

	std::stringstream handleStream;
	handleStream << (unsigned long)display << ':' << screenStr << ':' << wid;

	// retrieve XVisualInfo
	// NOTE: '-lGL' linker flag must be specified.
	int attrlist[] = { GLX_RGBA, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 16, GLX_STENCIL_SIZE, 8, None };
	XVisualInfo* vi = glXChooseVisual(display, DefaultScreen(display), attrlist);
	handleStream << ':' << (unsigned long)vi;

	return std::string(handleStream.str());

#elif defined(__WXMAC__)
	return Ogre::StringConverter::toString((size_t)window->MacGetTopLevelWindowRef());
#else
	#error Not supported on this platform!
#endif
#else
	return "";
#endif
}

void Skyscraper::SetFullScreen(bool enabled)
{
	//enable/disable fullscreen

	FullScreen = enabled;
#ifdef USING_WX
	window->ShowFullScreen(FullScreen);
#endif

#if defined(__WXMSW__)
	//in Windows, enable double-buffering when switching to fullscreen
	//to fix framerate drop issues
	/*if (enabled == true)
		window->SetDoubleBuffered(true);
	else
		window->SetDoubleBuffered(false);*/
#endif
}

void Skyscraper::RaiseWindow()
{
#ifdef USING_WX
	window->Raise();
	window->SetFocus();
#endif
}

#ifdef USING_WX
void Skyscraper::MacOpenFile(const wxString &filename)
{
	//support launching app with a building file, on Mac

	if (vm->IsEngineLoading() == true)
		return;

	if (StartupRunning == true)
		vm->GetHAL()->StopSound();

	//strip path from filename
	wxFileName file (filename);

	Load(file.GetFullName().ToStdString());
}
#endif

std::string Skyscraper::GetDataPath()
{
	return vm->data_path;
}

MainScreen* Skyscraper::GetWindow()
{
	return window;
}

VM* Skyscraper::GetVM()
{
	return vm;
}

#ifdef USING_WX
GUI* Skyscraper::GetGUI()
{
	return gui;
}
#endif

void Skyscraper::Report(const std::string &message)
{
	vm->GetHAL()->Report(message, "");
}

bool Skyscraper::ReportError(const std::string &message)
{
	return vm->GetHAL()->ReportError(message, "");
}

}
