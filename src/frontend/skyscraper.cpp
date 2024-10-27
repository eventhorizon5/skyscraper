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
#include "wx/progdlg.h"
#include "wx/cmdline.h"
#include "wx/filename.h"
#include "wx/filefn.h"
#include "wx/stdpaths.h"
#include "wx/joystick.h"
#include "wx/panel.h"
#endif
#include <locale>
#include <time.h>
#include <thread>
#include "globals.h"
#include "sbs.h"
#include "camera.h"
#include "gui/debugpanel.h"
#include "skyscraper.h"
#include "vm.h"
#include "enginecontext.h"
#include "scriptproc.h"
#include "gui/console.h"
#include "mainscreen.h"
#include "gui/loaddialog.h"
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
#include "malloc.h"
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

bool Skyscraper::OnInit(void)
{
	version = "2.1";
	version_rev = ToString(GIT_REV);
	version_state = "Alpha";
	version_frontend = version + ".0." + version_rev;
	StartupRunning = false;
	FullScreen = false;
	buttons = 0;
	buttoncount = 0;
	console = 0;
	progdialog = 0;
	dpanel = 0;
	window = 0;
	console = 0;
	loaddialog = 0;
	Verbose = false;
	show_progress = false;
	ShowMenu = false;
	Headless = false;
	background_rect = 0;
	background_node = 0;
	configfile = 0;
	keyconfigfile = 0;
	joyconfigfile = 0;
	parser = 0;
	macos_major = 0;
	macos_minor = 0;

	//create VM instance
	vm = new VM(this);

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
		Verbose = true;

	//set CheckScript mode if specified
	if (parser->Found(wxT("check-script")) == true)
		vm->CheckScript = true;

	//set headless mode if specified
	if (parser->Found(wxT("headless")) == true)
		Headless = true;

	//load config files
	configfile = vm->LoadConfiguration(data_path + "skyscraper.ini");
	keyconfigfile = vm->LoadConfiguration(data_path + "keyboard.ini");
	joyconfigfile = vm->LoadConfiguration(data_path + "joystick.ini");
	vm->LoadConfiguration("plugins.cfg", true);
	vm->LoadConfiguration("resources.cfg", true);

	showconsole = vm->GetConfigBool(configfile, "Skyscraper.Frontend.ShowConsole", true);

	//turn off console if specified on command line
	if (parser->Found(wxT("no-console")) == true)
		showconsole = false;

	//create console window
	if (showconsole == true)
		ShowConsole(false);

	//Create main window and set size from INI file defaults
	//if (Headless == false)
	//{
		window = new MainScreen(this, vm->GetConfigInt(configfile, "Skyscraper.Frontend.Menu.Width", 640), vm->GetConfigInt(configfile, "Skyscraper.Frontend.Menu.Height", 480));
		//AllowResize(false);
		window->ShowWindow();
		window->CenterOnScreen();
	//}

	//start and initialize VM
	if (!vm->Initialize(data_path))
		return vm->ReportError("Error initializing VM", "");

	//set up joystick if available
	wxJoystick joystick(wxJOYSTICK1);
	if (!joystick.IsOk())
		vm->Report("No joystick detected", "");
	else
	{
		vm->Report("", "");
		vm->Report("Joystick detected: " + joystick.GetProductName().ToStdString(), "");
		vm->Report("", "");
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
		filename = vm->GetConfigString(configfile, "Skyscraper.Frontend.AutoLoad", "");

	ShowMenu = vm->GetConfigBool(configfile, "Skyscraper.Frontend.Menu.Show", true);

	//turn off menu if specified on command line
	if (parser->Found(wxT("no-menu")) == true)
		ShowMenu = false;

	if (Headless == true || vm->GetConfigBool(configfile, "Skyscraper.Frontend.VR", false) == true)
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
	vm->Report("Cleaning up...", "");

	if (loaddialog)
		loaddialog->Destroy();
	loaddialog = 0;

	if (progdialog)
		progdialog->Destroy();
	progdialog = 0;

	UnloadSim();

	//cleanup sound
	vm->StopSound();

	//delete console window
	if (console)
		console->Destroy();
	console = 0;

	CloseProgressDialog();

	if (window)
		window->Destroy();
	window = 0;

	if (configfile)
		delete configfile;
	configfile = 0;

	if (keyconfigfile)
		delete keyconfigfile;
	keyconfigfile = 0;

	if (joyconfigfile)
		delete joyconfigfile;
	joyconfigfile = 0;

	if (parser)
		delete parser;
	parser = 0;

	delete vm;
	return wxApp::OnExit();
}

void Skyscraper::UnloadSim()
{
	//delete control panel object
	if(dpanel)
		delete dpanel;
	dpanel = 0;

	//unload sky system
	vm->UnloadSky();

	if (console)
		console->bSend->Enable(false);

	//delete all sim engines
	vm->DeleteEngines();
	vm->Clear();
}

void Skyscraper::ShowError(const std::string &message)
{
	//show error dialog
	wxMessageDialog dialog(0, message, _("Skyscraper"), wxOK | wxICON_ERROR);
	dialog.ShowModal();
}

void Skyscraper::ShowMessage(const std::string &message)
{
	//show message dialog
	wxMessageDialog dialog(0, message, _("Skyscraper"), wxOK | wxICON_INFORMATION);
	dialog.ShowModal();
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

		return vm->Render();
	}

	//show progress dialog if needed
	if (show_progress == true)
		ShowProgressDialog();

	//run sim engine instances
	vm->Run();

	//ProfileManager::dumpAll();

	return true;
}

bool Skyscraper::DrawBackground()
{
	//draw menu background

	bool result = false;

	result = DrawImage("data/" + vm->GetConfigString(configfile, "Skyscraper.Frontend.Menu.Image", "menu.png"), 0, -1, -1, false);

	if (result == false)
		return false;

	if (buttoncount == 0)
	{
		buttoncount = vm->GetConfigInt(configfile, "Skyscraper.Frontend.Menu.Buttons", 5);
		buttons = new buttondata[buttoncount];

		for (int i = 0; i < buttoncount; i++)
		{
			buttons[i].node = 0;
			buttons[i].drawn_selected = false;
			buttons[i].drawn_pressed = false;
			buttons[i].active_button = 0;
			buttons[i].rect = 0;
		}
	}

	for (int i = 0; i < buttoncount; i++)
	{
		std::string b1, b2, b3;
		Real x = 0, y = 0;
		bool center = false;
		std::string number = ToString(i + 1);

		if (i == 0)
		{
			b1 = "data/" + vm->GetConfigString(configfile, "Skyscraper.Frontend.Menu.Button1.Image", "button_triton.png");
			b2 = "data/" + vm->GetConfigString(configfile, "Skyscraper.Frontend.Menu.Button1.Selected", "button_triton_selected.png");
			b3 = "data/" + vm->GetConfigString(configfile, "Skyscraper.Frontend.Menu.Button1.Pressed", "button_triton_pressed.png");
			x = vm->GetConfigFloat(configfile, "Skyscraper.Frontend.Menu.Button1.X", 0.0);
			y = vm->GetConfigFloat(configfile, "Skyscraper.Frontend.Menu.Button1.Y", -0.08);
			center = vm->GetConfigBool(configfile, "Skyscraper.Frontend.Menu.Button1.Center", true);
		}
		if (i == 1)
		{
			b1 = "data/" + vm->GetConfigString(configfile, "Skyscraper.Frontend.Menu.Button2.Image", "button_glasstower.png");
			b2 = "data/" + vm->GetConfigString(configfile, "Skyscraper.Frontend.Menu.Button2.Selected", "button_glasstower_selected.png");
			b3 = "data/" + vm->GetConfigString(configfile, "Skyscraper.Frontend.Menu.Button2.Pressed", "button_glasstower_pressed.png");
			x = vm->GetConfigFloat(configfile, "Skyscraper.Frontend.Menu.Button2.X", 0.0);
			y = vm->GetConfigFloat(configfile, "Skyscraper.Frontend.Menu.Button2.Y", 0.125);
			center = vm->GetConfigBool(configfile, "Skyscraper.Frontend.Menu.Button2.Center", true);
		}
		if (i == 2)
		{
			b1 = "data/" + vm->GetConfigString(configfile, "Skyscraper.Frontend.Menu.Button3.Image", "button_searstower.png");
			b2 = "data/" + vm->GetConfigString(configfile, "Skyscraper.Frontend.Menu.Button3.Selected", "button_searstower_selected.png");
			b3 = "data/" + vm->GetConfigString(configfile, "Skyscraper.Frontend.Menu.Button3.Pressed", "button_searstower_pressed.png");
			x = vm->GetConfigFloat(configfile, "Skyscraper.Frontend.Menu.Button3.X", 0.0);
			y = vm->GetConfigFloat(configfile, "Skyscraper.Frontend.Menu.Button3.Y", 0.333);
			center = vm->GetConfigBool(configfile, "Skyscraper.Frontend.Menu.Button3.Center", true);
		}
		if (i == 3)
		{
			b1 = "data/" + vm->GetConfigString(configfile, "Skyscraper.Frontend.Menu.Button4.Image", "button_simple.png");
			b2 = "data/" + vm->GetConfigString(configfile, "Skyscraper.Frontend.Menu.Button4.Selected", "button_simple_selected.png");
			b3 = "data/" + vm->GetConfigString(configfile, "Skyscraper.Frontend.Menu.Button4.Pressed", "button_simple_pressed.png");
			x = vm->GetConfigFloat(configfile, "Skyscraper.Frontend.Menu.Button4.X", 0.0);
			y = vm->GetConfigFloat(configfile, "Skyscraper.Frontend.Menu.Button4.Y", 0.541);
			center = vm->GetConfigBool(configfile, "Skyscraper.Frontend.Menu.Button4.Center", true);
		}
		if (i == 4)
		{
			b1 = "data/" + vm->GetConfigString(configfile, "Skyscraper.Frontend.Menu.Button5.Image", "button_other.png");
			b2 = "data/" + vm->GetConfigString(configfile, "Skyscraper.Frontend.Menu.Button5.Selected", "button_other_selected.png");
			b3 = "data/" + vm->GetConfigString(configfile, "Skyscraper.Frontend.Menu.Button5.Pressed", "button_other_pressed.png");
			x = vm->GetConfigFloat(configfile, "Skyscraper.Frontend.Menu.Button5.X", 0.0);
			y = vm->GetConfigFloat(configfile, "Skyscraper.Frontend.Menu.Button5.Y", 0.75);
			center = vm->GetConfigBool(configfile, "Skyscraper.Frontend.Menu.Button5.Center", true);
		}
		if (i > 4)
		{
			b1 = "data/" + vm->GetConfigString(configfile, "Skyscraper.Frontend.Menu.Button" + number + ".Image", "");
			b2 = "data/" + vm->GetConfigString(configfile, "Skyscraper.Frontend.Menu.Button" + number + ".Selected", "");
			b3 = "data/" + vm->GetConfigString(configfile, "Skyscraper.Frontend.Menu.Button" + number + ".Pressed", "");
			x = vm->GetConfigFloat(configfile, "Skyscraper.Frontend.Menu.Button" + number + ".X", 0.0);
			y = vm->GetConfigFloat(configfile, "Skyscraper.Frontend.Menu.Button" + number + ".Y", 0.0);
			center = vm->GetConfigBool(configfile, "Skyscraper.Frontend.Menu.Button" + number + ".Center", true);
		}

		result = DrawImage(b1, &buttons[i], x, y, center, b2, b3);

		if (result == false)
			return false;
	}

	return true;
}

bool Skyscraper::DrawImage(const std::string &filename, buttondata *button, Real x, Real y, bool center, const std::string &filename_selected, const std::string &filename_pressed)
{
	//X and Y represent the image's top-left location.
	//values are -1 for the top left, 1 for the top right, -1 for the top, and 1 for the bottom
	//center is at 0, 0

	Real w, h;
	int w_orig = 0, h_orig = 0, w2 = 0, h2 = 0;
	bool background = false;

	std::string material = "";
	std::string Filename = filename;

	if (filename == "")
		return false;

	//exit if background has already been drawn
	if (background_image == Filename)
		return true;

	Ogre::TextureManager::getSingleton().setVerbose(false);
	Ogre::TexturePtr tex = Ogre::TextureManager::getSingleton().getByName(Filename);
	if (tex)
		material = Filename;

	//load image data from file, if not already preloaded
	if (material == "")
	{
		int count = 1;
		if (button)
			count = 3;

		for (int i = 0; i < count; i++)
		{
			if (i == 0)
				Filename = filename;
			if (i == 1)
				Filename = filename_selected;
			if (i == 2)
				Filename = filename_pressed;

			//create new material
			Ogre::MaterialPtr mat;
			try
			{
				mat = Ogre::MaterialManager::getSingleton().create(Filename, "General");
			}
			catch (Ogre::Exception& e)
			{
				return vm->ReportFatalError("Error creating material for texture " + Filename + "\n" + e.getDescription(), "");
			}

			//load image data from file
			Ogre::Image img;
			try
			{
				img.load(Filename, "General");
			}
			catch (Ogre::Exception &e)
			{
				return vm->ReportFatalError("Error loading texture " + Filename + "\n" + e.getDescription(), "");
			}

			w_orig = img.getWidth();
			h_orig = img.getHeight();

			//round up image size to power-of-2 value
			w2 = powf(2, ceilf(Log2((Real)w_orig)));
			h2 = powf(2, ceilf(Log2((Real)h_orig)));

			//create texture and blit image onto it - this solves texture quality issues on mainly D3D9
			tex = Ogre::TextureManager::getSingleton().createManual(Filename, "General", Ogre::TEX_TYPE_2D, w2, h2, 0, Ogre::PF_R8G8B8A8, Ogre::TU_STATIC);
			tex->getBuffer(0, 0)->blitFromMemory(img.getPixelBox(0, 0), Ogre::Box(0, 0, 0, w_orig, h_orig, img.getDepth()));

			//bind texture to material and set options
			Ogre::TextureUnitState *state = mat->getTechnique(0)->getPass(0)->createTextureUnitState(Filename);
			Ogre::Pass *pass = mat->getTechnique(0)->getPass(0);
			pass->setDepthCheckEnabled(false);
			pass->setDepthWriteEnabled(false);
			pass->setLightingEnabled(false);
			pass->setTextureFiltering(Ogre::TFO_NONE);

			//rescale texture
			state->setTextureScale((Real)w2 / (Real)w_orig, (Real)h2 / (Real)h_orig);
			state->setTextureScroll(-(Real(w2 - w_orig) / (Real)w2) / 2.0, -(Real(h2 - h_orig) / (Real)h2) / 2.0);

			if (tex->hasAlpha() == true && button)
			{
				mat->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
				mat->getTechnique(0)->getPass(0)->setAlphaRejectSettings(Ogre::CMPF_GREATER_EQUAL, 128);
			}
		}
		material = Filename;
		if (button)
		{
			button->node = 0;
			button->drawn_selected = false;
			button->drawn_pressed = false;
			button->active_button = 0;
			button->rect = 0;
		}
		else
		{
			background_rect = 0;
			background_node = 0;
		}
	}

	//exit if requested button is already visible
	if (button)
	{
		if (button->drawn_selected == false && button->drawn_pressed == false)
		{
			if (button->active_button == 1)
				return true;
			else
			{
				button->active_button = 1;
				material = filename;
			}
		}

		if (button->drawn_selected == true)
		{
			if (button->active_button == 2)
				return true;
			else
			{
				button->active_button = 2;
				material = filename_selected;
			}
		}

		if (button->drawn_pressed == true)
		{
			if (button->active_button == 3)
				return true;
			else
			{
				button->active_button = 3;
				material = filename_pressed;
			}
		}
	}

	//set values and draw button
	if (material != "")
	{
		//apply content scaling factor, fixes issues for example on Retina displays
		Real scale = window->GetContentScaleFactor();

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
		//set scale to 1.0 on MacOS versions earlier than 10.15
		if (macos_major == 10 && macos_minor < 15)
			scale = 1.0;
#endif

		w = w_orig / (vm->GetRenderWindow()->getWidth() / 2.0 / scale);
		h = h_orig / (vm->GetRenderWindow()->getHeight() / 2.0 / scale);
		if (button)
		{
			//delete previous object
			if (button->node)
				button->node->detachAllObjects();
			if (button->rect)
				delete button->rect;
			button->rect = 0;

			if (button->filename == "")
			{
				//store general button data
				button->filename = filename;
				button->filename_selected = filename_selected;
				button->filename_pressed = filename_pressed;

				button->offset_x = x;
				button->offset_y = y;
				if (center == true)
				{
					button->x = x - (w / 2);
					button->y = y - (h / 2);
				}
				else
				{
					button->x = x;
					button->y = y;
				}
				button->size_x = w;
				button->size_y = h;
			}

			x = button->x;
			y = button->y;
			w = button->size_x;
			h = button->size_y;
		}
		else
		{
			background_image = material;
			background = true;
			if (center == true)
			{
				x += -(w / 2);
				y += -(h / 2);
			}
		}

		//create rectangle
		Ogre::Rectangle2D* rect = new Ogre::Rectangle2D(true);
		rect->setCorners(x, -y, x + w, -(y + h));
		Ogre::MaterialPtr materialptr = Ogre::MaterialManager::getSingleton().getByName(material);
		rect->setMaterial(materialptr);
		if (background == true)
			rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);

		//set infinite bounding box
		Ogre::AxisAlignedBox aabInf;
		aabInf.setInfinite();
		rect->setBoundingBox(aabInf);

		//attach scene node
		Ogre::SceneNode* node = vm->GetSceneManager()->getRootSceneNode()->createChildSceneNode();
		node->attachObject(rect);
		if (button)
		{
			button->node = node;
			button->rect = rect;
		}
		else
		{
			background_node = node;
			background_rect = rect;
		}
	}
	return true;
}

bool Skyscraper::GetMenuInput()
{
	//input handler for main menu

	//exit if there aren't any buttons
	if (!buttons || buttoncount == 0)
		return false;

	//get mouse coordinates
	int mouse_x = window->ScreenToClient(wxGetMousePosition()).x;
	int mouse_y = window->ScreenToClient(wxGetMousePosition()).y;

	for (int i = 0; i < buttoncount; i++)
	{
		buttondata *button = &buttons[i];

	    //only process buttons if main window is selected
        if (window->Active != false)
        {
			Real mx = mouse_x;
			Real my = mouse_y;
			Real w = mx / window->GetClientSize().x;
			Real h = my / window->GetClientSize().y;
			Real mouse_x_rel = (w * 2) - 1;
			Real mouse_y_rel = (h * 2) - 1;

        	//change button status based on mouse position and button press status
        	if (mouse_x_rel > button->x && mouse_x_rel < button->x + button->size_x && mouse_y_rel > button->y && mouse_y_rel < button->y + button->size_y)
        	{
        		if (button->drawn_selected == false && wxGetMouseState().LeftIsDown() == false)
        		{
        			if (button->drawn_pressed == true)
        			{
        				//user clicked on button
        				button->drawn_selected = true;
        				Click(i);
					return true;
        			}
        			button->drawn_selected = true;
        		}
        		if (button->drawn_pressed == false && wxGetMouseState().LeftIsDown() == true)
        		{
        			button->drawn_pressed = true;
        			button->drawn_selected = false;
        		}
        	}
        	else if (button->drawn_selected == true || button->drawn_pressed == true)
        	{
        		button->drawn_selected = false;
        		button->drawn_pressed = false;
        	}
        }
	}

	return true;
}

void Skyscraper::Click(int index)
{
	//user clicked a button

	std::string number = ToString(index + 1);
	std::string filename = "";

	if (index == 0)
		filename = vm->GetConfigString(configfile, "Skyscraper.Frontend.Menu.Button1.File", "Triton Center.bld");
	if (index == 1)
		filename = vm->GetConfigString(configfile, "Skyscraper.Frontend.Menu.Button2.File", "Glass Tower.bld");
	if (index == 2)
		filename = vm->GetConfigString(configfile, "Skyscraper.Frontend.Menu.Button3.File", "Sears Tower.bld");
	if (index == 3)
		filename = vm->GetConfigString(configfile, "Skyscraper.Frontend.Menu.Button4.File", "Simple.bld");
	if (index > 3)
		filename = vm->GetConfigString(configfile, "Skyscraper.Frontend.Menu.Button" + number + ".File", "");

	if (filename == "")
	{
		//show file selection dialog
		filename = SelectBuilding();
	}

	if (filename != "")
	{
		Load(filename);
	}
}

void Skyscraper::DeleteButtons()
{
	if (buttoncount > 0)
	{
		for (int i = 0; i < buttoncount; i++)
		{
			buttondata *button = &buttons[i];

			if (button->node)
			{
				button->node->detachAllObjects();
				button->node->getParent()->removeChild(button->node);
				button->node = 0;
			}
			if (button->rect)
				delete button->rect;
			button->rect = 0;
		}
		delete [] buttons;
		buttoncount = 0;
	}
	buttons = 0;

	if (background_node)
	{
		background_node->detachAllObjects();
		background_node->getParent()->removeChild(background_node);
		background_node = 0;
	}
	if (background_rect)
		delete background_rect;
	background_rect = 0;
	background_image = "";
}

void Skyscraper::StartSound()
{
	//load and start background music

	if (vm->DisableSound == true)
		return;

	if (vm->GetConfigBool(configfile, "Skyscraper.Frontend.IntroMusic", true) == false)
		return;

	if (parser->Found(wxT("no-music")) == true)
		return;

	std::string filename = vm->GetConfigString(configfile, "Skyscraper.Frontend.IntroMusicFile", "intro.ogg");
	std::string filename_full = "data/" + filename;

	//check for an intro sound file in the data path location instead
	if (wxFileExists(data_path + filename_full))
		filename_full = data_path + filename_full;

	//play music
	vm->PlaySound(filename_full);
}

std::string Skyscraper::SelectBuilding()
{
	//choose a building from a script file

	std::string filename = "";

	//get listing of building files
	wxArrayString filelist;
	wxArrayString filelist2;
	wxDir::GetAllFiles(_("buildings/"), &filelist, _("*.bld"), wxDIR_FILES);
	wxDir::GetAllFiles(_(data_path + "buildings/"), &filelist2, _("*.bld"), wxDIR_FILES);

	//strip directory name and extension from entries
	for (size_t i = 0; i < filelist.size(); i++)
	{
		filelist[i] = filelist[i].substr(10);
		filelist[i] = filelist[i].substr(0, filelist[i].length() - 4);
	}
	for (size_t i = 0; i < filelist2.size(); i++)
	{
		filelist2[i] = filelist2[i].substr(data_path.length() + 10);
		filelist2[i] = filelist2[i].substr(0, filelist2[i].length() - 4);
	}

	for (size_t i = 0; i < filelist2.size(); i++)
	{
		bool found = false;
		for (size_t j = 0; j < filelist.size(); j++)
		{
			if (filelist[j] == filelist2[i])
			{
				found = true;
				break;
			}
		}
		if (found == false)
			filelist.Add(filelist2[i]);
	}

	//sort list
	filelist.Sort();

	//show selection dialog window
	wxSingleChoiceDialog Selector (0, _("Select a Building"), _("Load Building"), filelist);
	Selector.SetSize(wxSize(500, 400));
	Selector.CenterOnScreen();

	if (Selector.ShowModal() == wxID_OK)
	{
		filename = Selector.GetStringSelection();
		filename += ".bld";
	}

	return filename;
}

bool Skyscraper::Load(const std::string &filename, EngineContext *parent, const Vector3 &position, Real rotation, const Vector3 &area_min, const Vector3 &area_max)
{
	//load simulator and data file

	if (StartupRunning == true)
	{
		DeleteButtons();
		StartupRunning = false;
	}

	//exit if no building specified
	if (filename == "")
		return false;

	if (vm->GetEngineCount() == 0)
	{
		//set sky name
		vm->SkyName = vm->GetConfigString(configfile, "Skyscraper.Frontend.Caelum.SkyName", "DefaultSky");

		//clear scene
		vm->ClearScene();
	}

	//clear screen
	if (Headless == false)
		vm->GetRenderWindow()->update();

	bool result = vm->Load(filename, parent, position, rotation, area_min, area_max);

	return result;
}

bool Skyscraper::Start(EngineContext *engine)
{
	//start simulator

	if (!engine)
		return false;

	::SBS::SBS *Simcore = engine->GetSystem();

	if (engine == vm->GetActiveEngine())
	{
		//the sky needs to be created before Prepare() is called
		vm->CreateSky(engine);

		//switch to fullscreen mode if specified
		bool fullscreen = vm->GetConfigBool(configfile, "Skyscraper.Frontend.FullScreen", false);

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
			window->SetClientSize(vm->GetConfigInt(configfile, "Skyscraper.Frontend.ScreenWidth", 1024), vm->GetConfigInt(configfile, "Skyscraper.Frontend.ScreenHeight", 768));
			window->Center();
		}
	}

	//start simulation
	if (!vm->StartEngine(engine, vm->mCameras))
		return false;

	//close progress dialog if no engines are loading
	if (vm->IsEngineLoading() == false)
		CloseProgressDialog();

	//load control panel
	if (engine == vm->GetActiveEngine())
	{
		bool panel = vm->GetConfigBool(configfile, "Skyscraper.Frontend.ShowControlPanel", true);

		//override if disabled on the command line
		if (parser->Found(wxT("no-panel")) == true)
			panel = false;

		if (panel == true)
		{
			if (!dpanel)
				dpanel = new DebugPanel(this, NULL, -1);
			dpanel->Show(true);
			dpanel->SetPosition(wxPoint(vm->GetConfigInt(configfile, "Skyscraper.Frontend.ControlPanelX", 10), vm->GetConfigInt(configfile, "Skyscraper.Frontend.ControlPanelY", 25)));
		}
	}

	RefreshViewport();

	//set ambient light
	if (vm->GetConfigBool(configfile, "Skyscraper.SBS.Lighting", false) == true)
	{
		Real value = vm->GetConfigFloat(configfile, "Skyscraper.SBS.AmbientLight", 0.5);
		vm->GetSceneManager()->setAmbientLight(Ogre::ColourValue(value, value, value));
	}

	//show frame stats
	vm->EnableStats(vm->GetConfigBool(configfile, "Skyscraper.Frontend.Stats", true));

	//run simulation
	vm->Report("Running simulation...", "");
	vm->StopSound();
	if (console)
		console->bSend->Enable(true);
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
	vm->StopSound();

	CloseProgressDialog();

	//return to main menu
	SetFullScreen(false);
	window->SetClientSize(vm->GetConfigInt(configfile, "Skyscraper.Frontend.Menu.Width", 800), vm->GetConfigInt(configfile, "Skyscraper.Frontend.Menu.Height", 600));
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
	if(dpanel)
		dpanel->EnableTimer(false);

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

	bool vsync = vm->GetConfigBool(configfile, "Skyscraper.Frontend.Vsync", true);
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

	return vm->CreateRenderWindow(name, width, height, params);
}

void Skyscraper::destroyRenderWindow()
{
	vm->DestroyRenderWindow();
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

void Skyscraper::messageLogged(const std::string &message, Ogre::LogMessageLevel lml, bool maskDebug, const std::string &logName, bool &skipThisMessage)
{
	//callback function that receives OGRE log messages
	if (console)
	{
		console->Write(message);
		console->Update();
	}
}

void Skyscraper::ShowConsole(bool send_button)
{
	if (!console)
		console = new Console(this, NULL, -1);
	console->Show();
	console->Raise();
	console->SetPosition(wxPoint(vm->GetConfigInt(configfile, "Skyscraper.Frontend.ConsoleX", 10), vm->GetConfigInt(configfile, "Skyscraper.Frontend.ConsoleY", 25)));
	console->bSend->Enable(send_button);
}

void Skyscraper::CreateProgressDialog(const std::string &message)
{
	//don't create progress dialog if concurrent loading is enabled, and one engine is already running
	if (vm->GetEngineCount() > 1 && vm->ConcurrentLoads == true)
	{
		if (vm->GetFirstValidEngine()->IsRunning() == true)
			return;
	}

	if (!progdialog)
	{
		//show progress dialog in a queued fashion
		show_progress = true;
		prog_text = message;
	}
	else
	{
		wxString msg = progdialog->GetMessage();
		msg += "\n";
		msg += message;
		progdialog->Update(progdialog->GetValue(), msg);
	}

	//stop control panel timer
	if (dpanel)
		dpanel->EnableTimer(false);
}

void Skyscraper::CloseProgressDialog()
{
	//close progress dialog
	if (progdialog)
		progdialog->Destroy();
	progdialog = 0;

	//start control panel timer
	if (dpanel)
		dpanel->EnableTimer(true);
}

void Skyscraper::ShowProgressDialog()
{
	if (!progdialog)
		progdialog = new wxProgressDialog(wxT("Loading..."), prog_text, 100, window);

	show_progress = false;
}

void Skyscraper::UpdateProgress()
{
	if (!progdialog)
		return;

	int total_percent = vm->GetEngineCount() * 100;
	int current_percent = 0;

	for (size_t i = 0; i < vm->GetEngineCount(); i++)
	{
		if (vm->GetEngine(i))
			current_percent += vm->GetEngine(i)->GetProgress();
	}

	int final = ((Real)current_percent / (Real)total_percent) * 100;
	progdialog->Update(final);
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

void Skyscraper::RefreshConsole()
{
	if (console)
	{
		console->Refresh();
		console->Update();
	}
}

void Skyscraper::RefreshViewport()
{
	//refresh viewport to prevent rendering issues

	if (Headless == false)
	{
		for (size_t i = 0; i < vm->mViewports.size(); i++)
		{
			vm->mViewports[i]->_updateDimensions();
		}
	}
}

void Skyscraper::MacOpenFile(const wxString &filename)
{
	//support launching app with a building file, on Mac

	if (vm->IsEngineLoading() == true)
		return;

	if (StartupRunning == true)
		vm->StopSound();

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
		vm->Report("Running on MacOS 10." + ToString((int)major) + "." + ToString((int)minor), "");
		macos_major = 10;
		macos_minor = (int)major;
	}
	else
	{
		vm->Report("Running on MacOS " + ToString((int)major) + "." + ToString((int)minor), "");
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
		vm->Report("Running on Microsoft Windows " + ToString((int)osInfo.dwMajorVersion) + "." + ToString((int)osInfo.dwMinorVersion), "");
	}
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
	struct utsname osInfo{};
	uname(&osInfo);
	vm->Report("Running on Linux " + std::string(osInfo.release), "");
#endif
}

}
