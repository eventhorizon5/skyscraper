/*
	Skyscraper 1.1 Alpha - Simulation Frontend
	Copyright ©2005-2006 Ryan Thoryk
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

int main (int argc, char* argv[])
{
	return WinMain (GetModuleHandle (0), 0, GetCommandLineA (), SW_SHOWNORMAL);
}

bool Skyscraper::OnInit(void)
{
	srand (time (0));
	
	//Create new simulator object
	Simcore = new SBS();
	
	if (!Simcore->Initialize(argc, argv, "Skyscraper 1.1 Alpha"))
	{
		Simcore->ReportError("Error initializing system!");
		Cleanup();
		exit(1);
	}

	//set building file
	//Loader *loader = new Loader(NULL,-1,"");
    //loader->Show(true);

	//Simcore->BuildingFile = "buildings/Simple2.bld";
	//Simcore->BuildingFile = "buildings/Glass Tower.bld";
	Simcore->BuildingFile = "buildings/Triton Center.bld";
	//Simcore->BuildingFile = "buildings/Sears Tower.bld";

	//start simulation
	Simcore->Start();

	//load dialogs
	DebugPanel *dpanel = new DebugPanel(NULL, -1);
    dpanel->Show(true);

	//run simulation
	Simcore->Run();

	return true;
}

int Skyscraper::OnExit()
{
	//clean up
	delete Simcore;
	Simcore = 0;
	Cleanup();

	return 0;
}
