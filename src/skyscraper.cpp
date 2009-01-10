/* $Id$ */

/*
	Skyscraper 1.2 Alpha - Simulation Frontend
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

#define CS_IMPLEMENT_PLATFORM_APPLICATION
#define CS_NO_MALLOC_OVERRIDE

#include <wx/wx.h>
#include <crystalspace.h>
#include "ivideo/wxwin.h"
#include "globals.h"
#include "skyscraper.h"
#include "sbs.h"
#include "debugpanel.h"
#include "loader.h"

CS_IMPLEMENT_APPLICATION
IMPLEMENT_APP(Skyscraper)

BEGIN_EVENT_TABLE(MainScreen, wxFrame)
  EVT_SHOW(MainScreen::OnShow)
  EVT_ICONIZE(MainScreen::OnIconize)
  EVT_SIZE(MainScreen::OnSize)
  EVT_CLOSE(MainScreen::OnClose)
END_EVENT_TABLE()

SBS *Simcore;
DebugPanel *dpanel;
MainScreen *window;

#ifdef CS_PLATFORM_WIN32

#ifndef SW_SHOWNORMAL
	#define SW_SHOWNORMAL 1
#endif

int main (int argc, char* argv[])
{
	return WinMain (GetModuleHandle (0), 0, GetCommandLineA (), SW_SHOWNORMAL);
}

#endif

bool Skyscraper::OnInit(void)
{
	BuildingFile = "";
	srand (time (0));

	//set building file
	wxFileDialog *Selector = new wxFileDialog(0, _("Select a Building"), _("buildings"), _(""), _("Building files (*.bld)|*.bld"), wxOPEN);
	int result = Selector->ShowModal();
	if (result == wxID_CANCEL)
	{
		//delete dialog
		delete Selector;
		Selector = 0;
		//quit
		return false;
	}

	wxSleep(1);

	//Create new simulator object
	Simcore = new SBS();

	//Create main window
	window = new MainScreen();
	window->ShowWindow();

	#if defined(wxUSE_UNICODE) && wxUSE_UNICODE
	char** csargv;
	csargv = (char**)cs_malloc(sizeof(char*) * argc);
	for (int i = 0; i < argc; i++)
		csargv[i] = strdup(wxString(argv[i]).mb_str().data());
	if (!Simcore->Initialize(argc, csargv, window->panel))
	#else
	if (!Simcore->Initialize(argc, argv, window->panel))
	#endif
	{
		Simcore->ReportError("Error initializing system!");
		Cleanup();
		exit(1);
	}

	#if defined(wxUSE_UNICODE) && wxUSE_UNICODE
	BuildingFile = Selector->GetFilename().mb_str().data();
	#else
	BuildingFile = Selector->GetFilename();
	#endif

	//load building data file
	Simcore->Report("\nLoading building data from " + BuildingFile + "...\n");

	//Pause for 1 second
	csSleep(1000);

	BuildingFile.Insert(0, "/root/buildings/");
	LoadBuilding(BuildingFile.GetData());
	//if (LoadBuilding(BuildingFile.GetData()) != 0)

	//delete dialog
	delete Selector;
	Selector = 0;

	//start simulation
	Simcore->Start(this);

	//load dialogs
	dpanel = new DebugPanel(NULL, -1);
	dpanel->Show(true);
	dpanel->SetPosition(wxPoint(10, 10));
	window->Raise();

	//show main window
	//window->ShowWindow();

	//run simulation
	Simcore->Run();

	return true;
}

int Skyscraper::OnExit()
{
	//clean up
	Simcore->Stop();
	dpanel->timer->Stop();
	dpanel->Destroy();
	delete Simcore;
	Simcore = 0;
	delete window;
	window = 0;
	Cleanup();

	return 0;
}

MainScreen::MainScreen() : wxFrame(0, -1, wxT("Skyscraper 2.0 Alpha 2"), wxDefaultPosition, wxSize(640, 480), wxDEFAULT_FRAME_STYLE)
{
	this->Center();
	new wxPanel(this, -1, wxPoint(0, 0), wxSize(1, 1));
	panel = new wxPanel(this, -1, wxPoint(0, 0), this->GetClientSize());
	//this->SetTitle(wxString::FromAscii(windowtitle));
}

MainScreen::~MainScreen()
{

}

void MainScreen::OnIconize(wxIconizeEvent& event)
{
	//csPrintf("got iconize %d\n", (int)event.Iconized());
}

void MainScreen::OnShow(wxShowEvent& event)
{
	//csPrintf("got show %d\n", (int)event.GetShow());
}

void MainScreen::OnSize(wxSizeEvent& WXUNUSED(event))
{
	panel->SetSize(this->GetClientSize());
}

void MainScreen::OnClose(wxCloseEvent& event)
{
	Simcore->Stop();
	dpanel->timer->Stop();
	wxGetApp().Exit();
}

void MainScreen::ShowWindow()
{
	Show(true);
	panel->Show(true);
}
