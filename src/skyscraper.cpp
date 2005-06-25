//Skyscraper 1.1 Alpha - Simulation Frontend
//Copyright ©2005 Ryan Thoryk
//http://www.tliquest.net/skyscraper
//http://sourceforge.net/projects/skyscraper
//Contact - ryan@tliquest.net

//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "skyscraper.h"
#include "mainfrm.h"
#include "sbs.h"

IMPLEMENT_APP(MainFrmApp)

SBS *Simcore;

bool MainFrmApp::OnInit()
{
    MainFrm *myFrame = new  MainFrm(NULL);
    SetTopWindow(myFrame);
    myFrame->Show(TRUE);
    startsim();
    return TRUE;
}
 
int MainFrmApp::OnExit()
{
	return 0;
}

void startsim()
{
	//Create new simulator object
	Simcore = new SBS;
	
	//set default starting elevator
	Simcore->ElevatorNumber = 1;

	//set frame rate
	Simcore->FrameRate = 30;
	Simcore->FrameLimiter = true;

	//temporary static values
	//SoundDivisor = 10
	//SoundMaxDistance = 1000

	//Print banner
	//Dest.ForeColor = RGB(255, 255, 255)
	//Dest.BackColor = RGB(0, 0, 0)

	//Start simulator
	Simcore->StartEngine();
	//DoEvents

	//wait 2 seconds
	Sleep(2000);

	//Dest.Print Spc(2); "Initializing TrueVision3D..."
	//DoEvents

	Simcore->IsRunning = true;
	//EnableCollisions = true;

}

void MainLoop()
{
	Simcore->input();
	Simcore->render();
}
