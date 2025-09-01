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
#include "wx/display.h"
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

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <sysdir.h>  // for sysdir_start_search_path_enumeration
#include <glob.h>    // for glob needed to expand ~ to user dir
#include <stdio.h>
#include <sys/sysctl.h>
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include <windows.h>
#endif

using namespace SBS;

#ifdef USING_WX
namespace Skyscraper {

IMPLEMENT_APP_NO_MAIN(Skyscraper)

}
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#if OGRE_CPU != OGRE_CPU_ARM
#ifdef USING_WX
#include "uexception.h"
#endif
#endif
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
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#if OGRE_CPU != OGRE_CPU_ARM
#ifdef USING_WX
	//initialize top-level exception handler
	InitUnhandledExceptionFilter();
#endif
#endif
#endif

	StartupRunning = false;
	FullScreen = false;
	Maximized = false;
	ShowMenu = false;
	mRenderWindow = 0;
	window = 0;
	wireframe = 0;

	//reset input states
	boxes = false;
	colliders = false;
	strafe_left = false;
	strafe_right = false;
	float_up = false;
	float_down = false;
	spin_up = false;
	spin_down = false;
	turn_left = false;
	turn_right = false;
	look_up = false;
	look_down = false;
	step_forward = false;
	step_backward = false;
	freelook = false;

	//reset loop states
	startscreen_loaded = false;

#ifdef USING_WX
	parser = 0;
#else
	alt_down = false;
	ctrl_down = false;
	shift_down = false;
#endif

	//create VM instance
	vm = new VM();

#ifdef USING_WX
	gui = vm->GetGUI();
#endif
	startscreen = new StartScreen(this);

	//set working directory
	SetCWD();

	//set up unhandled exception handler (crash report system)
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#if OGRE_CPU != OGRE_CPU_ARM
#ifdef USING_WX
	UnhandledExceptionSetRoot(this);
#endif
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
		printf("Skyscraper version %s\n", vm->version_full.c_str());
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
	window->Maximize(hal->GetConfigBool(hal->configfile, "Skyscraper.Frontend.Maximized", false));

	vm->SetParent(window);
#else

	//initialize Ogre
	OgreBites::ApplicationContext::setup();
	addInputListener(this);

	//get overlay system if already created
	Ogre::OverlaySystem *overlay = getOverlaySystem();
#endif

	//start and initialize abstraction layer
#ifdef USING_WX
	if (!hal->Initialize(vm->data_path))
#else
	if (!hal->Initialize(vm->data_path, getRoot(), overlay))
#endif
		return ReportError("Error initializing HAL");

	Report("");
	Report("Creating render window...");
#ifdef USING_WX
	mRenderWindow = CreateRenderWindow(0, "SkyscraperVR");
#else
	mRenderWindow = getRenderWindow();
#endif

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
		Report("Joystick buttons: " + ToString(joystick.GetNumberButtons()));
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
	startscreen = 0;
	delete vm;
	vm = 0;

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
		vm->loadstart = false;
		return true;
	}

	vm->ProcessConsole();

	//main menu routine
	if (StartupRunning == true)
	{
		bool result = false;

		if (startscreen_loaded == false)
		{
#ifdef USING_WX
			Maximized = window->IsMaximized();
			if (Maximized == true)
				window->Maximize(false);

			window->SetClientSize(vm->GetHAL()->GetConfigInt(vm->GetHAL()->configfile, "Skyscraper.Frontend.Menu.Width", 800), vm->GetHAL()->GetConfigInt(vm->GetHAL()->configfile, "Skyscraper.Frontend.Menu.Height", 600));
			window->Center();
			window->SetCursor(wxNullCursor);
#endif
		}

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

		if (startscreen_loaded == false)
		{
#ifdef USING_WX
			if (Maximized == true)
				window->Maximize();
#endif
		}

		startscreen_loaded = true;

		//have HAL render frame
		return vm->GetHAL()->Render();
	}

#ifdef USING_WX
	if (vm->LoadPending() == false)
		gui->ShowProgress();
#endif

	//run sim engine instances
	std::vector<EngineContext*> newengines;
	VMStatus status = vm->Run(newengines);

	//start a new engine if needed
	if (status == VMSTATUS_LOAD)
	{
		for (size_t i = 0; i < newengines.size(); i++)
		{
			Start(newengines[i]);
		}
	}

	if (status == VMSTATUS_UNLOAD ||  status == VMSTATUS_FATAL)
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

	Real volume = vm->GetHAL()->GetConfigFloat(vm->GetHAL()->configfile, "Skyscraper.Frontend.IntroMusicVolume", 1.0);

	//fix name clash
	#undef PlaySound

	//play music
	vm->GetHAL()->PlaySound(filename_full, volume);
}

std::string Skyscraper::SelectBuilding(bool native_dialog)
{
#ifdef USING_WX

	if (native_dialog == true)
		return gui->SelectBuildingNative(vm->data_path);

	if (vm->GetHAL()->GetConfigBool(vm->GetHAL()->configfile, "Skyscraper.Frontend.SelectBuildingNative", false) == false)
		return gui->SelectBuilding(vm->data_path);
	else
		return gui->SelectBuildingNative(vm->data_path);
#else
	return "";
#endif
}

bool Skyscraper::Load(const std::string &filename, EngineContext *parent, const Vector3 &position, const Vector3 &rotation, const Vector3 &area_min, const Vector3 &area_max)
{
	//load simulator and data file

	StopMenu();

	bool result = vm->Load(false, true, filename, parent, position, rotation, area_min, area_max);

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
		startscreen_loaded = false;
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
		Maximized = window->IsMaximized();

		//resize main window
		if (FullScreen == false)
		{
#if !defined(__WXMAC__)
			window->SetBackgroundColour(*wxBLACK);
#endif
			if (Maximized == false)
			{
				window->SetClientSize(hal->GetConfigInt(hal->configfile, "Skyscraper.Frontend.ScreenWidth", 1024), hal->GetConfigInt(hal->configfile, "Skyscraper.Frontend.ScreenHeight", 768));
				window->Center();
			}
		}
#endif
	}

	//start simulation
	if (!vm->StartEngine(engine))
		return false;

	//exit if engine loads are still queued
	if (vm->LoadPending() == true || vm->IsEngineLoading() == true)
		return true;

	//close progress dialog if no engines are loading
#ifdef USING_WX
	if (vm->IsEngineLoading() == false)
		gui->CloseProgressDialog();
#endif

	//load control panel
	bool panel = vm->GetHAL()->GetConfigBool(hal->configfile, "Skyscraper.Frontend.ShowControlPanel", true);

#ifdef USING_WX
		//override if disabled on the command line
		if (parser->Found(wxT("no-panel")) == true)
			panel = false;

		if (panel == true)
			gui->CreateDebugPanel();
#endif

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

void Skyscraper::destroyRenderWindow()
{
	vm->GetHAL()->DestroyRenderWindow();
}

void Skyscraper::SetFullScreen(bool enabled)
{
	//enable/disable fullscreen

	FullScreen = enabled;
#ifdef USING_WX
	window->ShowFullScreen(FullScreen);

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
if (FullScreen == true)
{
	wxDisplay display(wxDisplay::GetFromWindow(window));
	wxRect area = display.GetClientArea(); //excludes menu bar/dock
	window->Move(area.GetTopLeft());
}
else
	window->Center();
#endif

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
