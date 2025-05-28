/*
	Skyscraper 2.1 - wxWidgets Frontend
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
#include "globals.h"
#include "vm.h"
#include "hal.h"
#include "skyscraper.h"
#include "mainscreen.h"

#if defined(__WXGTK__)
   // NOTE: Find the GTK install config with `pkg-config --cflags gtk+-2.0`
   #include "gtk/gtk.h"
   #include "gdk/gdk.h"
   #include "gdk/gdkx.h"
   #include "GL/glx.h"
#endif

namespace Skyscraper {

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

bool Skyscraper::SetCWD()
{
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
	return true;
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

}

#endif
