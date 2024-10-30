/*
	Skyscraper 2.1 - Simulation Frontend
	Copyright (C)2003-2024 Ryan Thoryk
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

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#include "wx/dir.h"
#include "wx/cmdline.h"
#include "wx/filename.h"
#include "wx/filefn.h"
#include "wx/stdpaths.h"
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
#include "mainscreen.h"
#include "profiler.h"
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

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
#include <sys/utsname.h>
#endif

#if defined(__WXGTK__)
   // NOTE: Find the GTK install config with `pkg-config --cflags gtk+-2.0`
   #include "gtk/gtk.h"
   #include "gdk/gdk.h"
   #include "gdk/gdkx.h"
   #include "GL/glx.h"
#endif

using namespace SBS;

namespace Skyscraper {

IMPLEMENT_APP_NO_MAIN(Skyscraper)

}

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

// Code to get OS version on Mac
int get_macos_version(uint32_t &major, uint32_t &minor, bool &osx)
{
	//returns the OS major and minor version
	//if osx is true, os is 10.x.x releases, otherwise is 11.x or greater

	char osversion[32];
	size_t osversion_len = sizeof(osversion) - 1;
	int osversion_name[] = { CTL_KERN, KERN_OSRELEASE };

	if (sysctl(osversion_name, 2, osversion, &osversion_len, NULL, 0) == -1) {
		printf("get_macos_version: sysctl() failed\n");
		return 1;
	}

	if (sscanf(osversion, "%u.%u", &major, &minor) != 2) {
		printf("get_macos_version: sscanf() failed\n");
		return 1;
	}

	if (major >= 20) {
		major -= 9;
		osx = false; //macOS 11 and newer
	} else {
		major -= 4;
		osx = true; //macOS 10.1.1 and newer
	}

	return 0;
}

#endif

int main (int argc, char* argv[])
{

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#if OGRE_CPU != OGRE_CPU_ARM
	//initialize top-level exception handler
	Skyscraper::InitUnhandledExceptionFilter();
#endif
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
	//force X11 if on Wayland
	setenv("GDK_BACKEND", "x11", false);
#endif

	//main wxWidgets entry point
	wxEntry(argc, argv);

	return 0;
}

namespace Skyscraper {

bool Skyscraper::OnInit()
{
	StartupRunning = false;
	FullScreen = false;
	buttons = 0;
	buttoncount = 0;
	ShowMenu = false;
	Headless = false;
	background_rect = 0;
	background_node = 0;
	macos_major = 0;
	macos_minor = 0;

	//create VM instance
	vm = new VM();

	vm->version = "2.1";
	vm->version_rev = ToString(GIT_REV);
	vm->version_state = "Alpha";
	vm->version_frontend = vm->version + ".0." + vm->version_rev;

	gui = vm->GetGUI();

	//switch current working directory to executable's path, if needed
	wxString exefile = wxStandardPaths::Get().GetExecutablePath(); //get full path and filename
	wxString app_path = wxPathOnly(exefile); //strip off filename
#if defined(__WXMAC__)
	data_path = settingsPath() + "/Skyscraper/"; //Application Support folder

	if (!wxDirExists(data_path))
		wxMkdir(data_path);

	if (!wxDirExists(data_path + wxT("buildings")))
		wxMkdir(data_path + wxT("buildings"));
	if (!wxDirExists(data_path + wxT("data")))
		wxMkdir(data_path + wxT("data"));
	if (!wxDirExists(data_path + wxT("screenshots")))
		wxMkdir(data_path + wxT("screenshots"));

	wxSetWorkingDirectory(app_path + wxT("/../Resources")); //set working directory to resources folder on Mac

	if (!wxFileExists(data_path + wxT("skyscraper.ini")))
		wxCopyFile("skyscraper.ini", data_path + wxT("skyscraper.ini"));
	if (!wxFileExists(data_path + wxT("keyboard.ini")))
		wxCopyFile("keyboard.ini", data_path + wxT("keyboard.ini"));
	if (!wxFileExists(data_path + wxT("joystick.ini")))
		wxCopyFile("joystick.ini", data_path + wxT("joystick.ini"));

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

		{ wxCMD_LINE_SWITCH, "H", "headless", "run in headless mode",
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

	//show version number and exit if specified
	if (parser->Found(wxT("version")) == true)
	{
		printf("Skyscraper version %s\n", version_frontend.c_str());
		return false;
	}

	//set verbose mode if specified
	if (parser->Found(wxT("verbose")) == true)
		vm->Verbose = true;

	//set CheckScript mode if specified
	if (parser->Found(wxT("check-script")) == true)
		vm->CheckScript = true;

	//set headless mode if specified
	if (parser->Found(wxT("headless")) == true)
		Headless = true;

	//load config files
	HAL *hal = vm->GetHAL();
	hal->LoadConfiguration();

	showconsole = hal->GetConfigBool(configfile, "Skyscraper.Frontend.ShowConsole", true);

	//turn off console if specified on command line
	if (parser->Found(wxT("no-console")) == true)
		showconsole = false;

	//create console window
	if (showconsole == true)
		ShowConsole(false);

	//Create main window and set size from INI file defaults
	//if (Headless == false)
	//{
		window = new MainScreen(this, hal->GetConfigInt(configfile, "Skyscraper.Frontend.Menu.Width", 640), hal->GetConfigInt(configfile, "Skyscraper.Frontend.Menu.Height", 480));
		//AllowResize(false);
		window->ShowWindow();
		window->CenterOnScreen();
	//}

	//start and initialize abstraction layer
	if (!hal->Initialize(data_path))
		return hal->ReportError("Error initializing HAL", "");

	//if (vm->GetFrontend()->Headless == false)
	//{
		hal->Report("", "");
		hal->Report("Creating render window...", "");
		mRenderWindow = CreateRenderWindow(0, "SkyscraperVR");
	//}

	//set up joystick if available
	wxJoystick joystick(wxJOYSTICK1);
	if (!joystick.IsOk())
		hal->Report("No joystick detected", "");
	else
	{
		hal->Report("", "");
		hal->Report("Joystick detected: " + joystick.GetProductName().ToStdString(), "");
		hal->Report("", "");
	}

	ShowPlatform();

	//autoload a building file if specified
	std::string filename;
	if (parser->GetParamCount() > 0)
	{
		filename = parser->GetParam(0).ToStdString();

		//strip path from filename
		wxFileName file (filename);
		filename = file.GetFullName();
	}
	else
		filename = hal->GetConfigString(configfile, "Skyscraper.Frontend.AutoLoad", "");

	ShowMenu = hal->GetConfigBool(configfile, "Skyscraper.Frontend.Menu.Show", true);

	//turn off menu if specified on command line
	if (parser->Found(wxT("no-menu")) == true)
		ShowMenu = false;

	if (Headless == true || hal->GetConfigBool(configfile, "Skyscraper.Frontend.VR", false) == true)
		ShowMenu = false;

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
	vm->GetHAL()->Report("Cleaning up...", "");

	//shut down GUI
	delete gui;
	gui = 0;

	UnloadSim();

	//cleanup sound
	vm->GetHAL()->StopSound();

	if (window)
		window->Destroy();
	window = 0;

	if (parser)
		delete parser;
	parser = 0;

	delete gui;
	delete vm;
	return wxApp::OnExit();
}

void Skyscraper::UnloadSim()
{
	//unload GUI
	gui->Unload();

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

	//main menu routine
	if (StartupRunning == true)
	{
		bool result = false;

		result = DrawBackground();

		if (result == false)
			return false;

		result = GetMenuInput();

		if (result == false)
			return false;

		//have HAL render frame
		return vm->GetHAL()->Render();
	}

	gui->ShowProgress();

	//run sim engine instances
	bool status = vm->Run();

	if (status == 2)
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

	if (parser->Found(wxT("no-music")) == true)
		return;

	std::string filename = vm->GetHAL()->GetConfigString(vm->GetHAL()->configfile, "Skyscraper.Frontend.IntroMusicFile", "intro.ogg");
	std::string filename_full = "data/" + filename;

	//check for an intro sound file in the data path location instead
	if (wxFileExists(data_path + filename_full))
		filename_full = data_path + filename_full;

	//play music
	vm->GetHAL()->PlaySound(filename_full);
}

std::string Skyscraper::SelectBuilding()
{
	return gui->SelectBuilding(data_path);
}

bool Skyscraper::Load(const std::string &filename, EngineContext *parent, const Vector3 &position, Real rotation, const Vector3 &area_min, const Vector3 &area_max)
{
	//load simulator and data file

	if (StartupRunning == true)
	{
		DeleteButtons();
		StartupRunning = false;
	}

	bool result = vm->Load(filename, parent, position, rotation, area_min, area_max);

	if (result == false && vm->GetEngineCount() == 1)
		UnloadToMenu();

	return result;
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

		//override fullscreen setting if specified on the command line
		if (parser->Found(wxT("fullscreen")) == true)
			fullscreen = true;

		if (fullscreen == true)
			SetFullScreen(true);

		//resize main window
		if (FullScreen == false)
		{
#if !defined(__WXMAC__)
			window->SetBackgroundColour(*wxBLACK);
#endif
			window->SetClientSize(hal->GetConfigInt(hal->configfile, "Skyscraper.Frontend.ScreenWidth", 1024), hal->GetConfigInt(hal->configfile, "Skyscraper.Frontend.ScreenHeight", 768));
			window->Center();
		}
	}

	//start simulation
	if (!vm->StartEngine(engine, hal->mCameras))
		return false;

	//close progress dialog if no engines are loading
	if (vm->IsEngineLoading() == false)
		CloseProgressDialog();

	//load control panel
	if (engine == vm->GetActiveEngine())
	{
		bool panel = vm->GetHAL()->GetConfigBool(hal->configfile, "Skyscraper.Frontend.ShowControlPanel", true);

		//override if disabled on the command line
		if (parser->Found(wxT("no-panel")) == true)
			panel = false;

		if (panel == true)
			gui->CreateDebugPanel();
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
	hal->Report("Running simulation...", "");
	hal->StopSound();

	gui->EnableConsole(true);

	return true;
}

void Skyscraper::AllowResize(bool value)
{
	//changes the window style to either allow or disallow resizing

	if (value)
		window->SetWindowStyleFlag(wxDEFAULT_FRAME_STYLE | wxMAXIMIZE);
	else
		window->SetWindowStyleFlag(wxDEFAULT_FRAME_STYLE & ~wxMAXIMIZE);
	window->Refresh();
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

	CloseProgressDialog();

	//return to main menu
	SetFullScreen(false);
	window->SetClientSize(vm->GetHAL()->GetConfigInt(vm->GetHAL()->configfile, "Skyscraper.Frontend.Menu.Width", 800), vm->GetHAL()->GetConfigInt(vm->GetHAL()->configfile, "Skyscraper.Frontend.Menu.Height", 600));
	window->Center();
	window->SetCursor(wxNullCursor);

	vm->ConcurrentLoads = false;
	vm->RenderOnStartup = false;

	StartSound();
	StartupRunning = true;
}

void Skyscraper::Quit()
{
	//exit app

	gui->EnableTimer(false);

	wxGetApp().Exit();
}

Ogre::RenderWindow* Skyscraper::CreateRenderWindow(const Ogre::NameValuePairList* miscParams, const std::string& windowName)
{
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
}

void Skyscraper::destroyRenderWindow()
{
	vm->GetHAL()->DestroyRenderWindow();
}

const std::string Skyscraper::getOgreHandle() const
{
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
}

void Skyscraper::SetFullScreen(bool enabled)
{
	//enable/disable fullscreen

	FullScreen = enabled;
	window->ShowFullScreen(FullScreen);

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
	window->Raise();
	window->SetFocus();
}

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

std::string Skyscraper::GetDataPath()
{
	return data_path;
}

MainScreen* Skyscraper::GetWindow()
{
	return window;
}

VM* Skyscraper::GetVM()
{
	return vm;
}

void Skyscraper::ShowPlatform()
{
	//set platform name
	std::string bits;

#if OGRE_ARCH_TYPE == OGRE_ARCHITECTURE_32
	bits = "32-bit";
#endif
#if OGRE_ARCH_TYPE == OGRE_ARCHITECTURE_64
	bits = "64-bit";
#endif

#if OGRE_CPU == OGRE_CPU_X86
	Architecture = "x86";
#elif OGRE_CPU == OGRE_CPU_PPC
	Architecture = "PPC";
#elif OGRE_CPU == OGRE_CPU_ARM
	Architecture = "ARM";
#elif OGRE_CPU == OGRE_CPU_MIPS
	Architecture = "MIPS";
#elif OGRE_CPU == OGRE_CPU_UNKNOWN
	Architecture = "Unknown";
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	Platform = "Windows " + Architecture + " " + bits;
#elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX
	Platform = "Linux " + Architecture + " " + bits;
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE
	Platform = "MacOS " + Architecture + " " + bits;
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
	//report MacOS version if applicable
	uint32_t major = 0, minor = 0;
	bool osx = true;
	get_macos_version(major, minor, osx);

	if (osx == true)
	{
		vm->GetHAL()->Report("Running on MacOS 10." + ToString((int)major) + "." + ToString((int)minor), "");
		macos_major = 10;
		macos_minor = (int)major;
	}
	else
	{
		vm->GetHAL()->Report("Running on MacOS " + ToString((int)major) + "." + ToString((int)minor), "");
		macos_major = (int)major;
		macos_minor = (int)minor;
	}
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	//get Windows version

	NTSTATUS(WINAPI* RtlGetVersion)(LPOSVERSIONINFOEXW);
	OSVERSIONINFOEXW osInfo;

	*(FARPROC*)&RtlGetVersion = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");

	if (NULL != RtlGetVersion)
	{
		osInfo.dwOSVersionInfoSize = sizeof(osInfo);
		RtlGetVersion(&osInfo);
		vm->GetHAL()->Report("Running on Microsoft Windows " + ToString((int)osInfo.dwMajorVersion) + "." + ToString((int)osInfo.dwMinorVersion), "");
	}
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
	struct utsname osInfo{};
	uname(&osInfo);
	vm->GetHAL()->Report("Running on Linux " + std::string(osInfo.release), "");
#endif
}

}
