/* $Id$ */

/*
	Skyscraper 1.1 Alpha - Simulation Frontend
	Copyright (C)2005-2006 Ryan Thoryk
	http://www.tliquest.net/skyscraper
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

#include <wx/wx.h>
#include <crystalspace.h>
#include "skyscraper.h"
#include "sbs.h"
#include "debugpanel.h"
#include "loader.h"

CS_IMPLEMENT_APPLICATION
IMPLEMENT_APP(Skyscraper)

SBS *Simcore;

#ifdef CS_PLATFORM_WIN32

int main (int argc, char* argv[])
{
	return WinMain (GetModuleHandle (0), 0, GetCommandLineA (), SW_SHOWNORMAL);
}

#endif

bool Skyscraper::OnInit(void)
{
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

	#if defined(wxUSE_UNICODE) && wxUSE_UNICODE
	char** csargv;
	csargv = (char**)cs_malloc(sizeof(char*) * argc);
	for (int i = 0; i < argc; i++)
		csargv[i] = strdup(wxString(argv[i]).mb_str().data());
	if (!Simcore->Initialize(argc, csargv, "Skyscraper 1.1 Alpha"))
	#else
	if (!Simcore->Initialize(argc, argv, "Skyscraper 1.1 Alpha"))
	#endif
	{
		Simcore->ReportError("Error initializing system!");
		Cleanup();
		exit(1);
	}

	#if defined(wxUSE_UNICODE) && wxUSE_UNICODE
	Simcore->BuildingFile = Selector->GetFilename().mb_str().data();
	#else
	Simcore->BuildingFile = Selector->GetFilename();
	#endif

	//delete dialog
	delete Selector;
	Selector = 0;

	//start simulation
	Simcore->Start();

	//load dialogs
	DebugPanel *dpanel = new DebugPanel(NULL, -1);
	dpanel->Show(true);
	dpanel->SetPosition(wxPoint(10, 10));

	//run simulation
	Simcore->Run();

	//clean up
	dpanel->timer->Stop();
	dpanel->Destroy();
	delete Simcore;
	Simcore = 0;
	Cleanup();

	return true;
}

int Skyscraper::OnExit()
{
	return 0;
}
