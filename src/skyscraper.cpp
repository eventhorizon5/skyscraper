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

#include <iostream>
#include "skyscraper.h"

//CrystalSpace Includes
#include "cssysdef.h"
#include "csutil/sysfunc.h"
#include "iutil/vfs.h"
#include "csutil/cscolor.h"
#include "cstool/csview.h"
#include "cstool/initapp.h"
#include "iutil/eventq.h"
#include "iutil/event.h"
#include "iutil/objreg.h"
#include "iutil/csinput.h"
#include "iutil/virtclk.h"
#include "iengine/sector.h"
#include "iengine/engine.h"
#include "iengine/camera.h"
#include "iengine/light.h"
#include "iengine/texture.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/material.h"
#include "imesh/thing.h"
#include "imesh/object.h"
#include "ivideo/graph3d.h"
#include "ivideo/graph2d.h"
#include "ivideo/texture.h"
#include "ivideo/material.h"
#include "ivideo/fontserv.h"
#include "ivideo/natwin.h"
#include "igraphic/imageio.h"
#include "imap/parser.h"
#include "ivaria/reporter.h"
#include "ivaria/stdrep.h"
#include "csutil/cmdhelp.h"
#include "csutil/event.h"

//#include "mainfrm.h"
#include "sbs.h"

//IMPLEMENT_APP(MainFrmApp)

SBS *Simcore;
iObjectRegistry* object_reg;

/*bool MainFrmApp::OnInit()
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
}*/

void startsim()
{
	//Create new simulator object
	Simcore = new SBS (object_reg);
	
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
	if (Simcore->Initialize ())
	   Simcore->Start ();
	//DoEvents

	//wait 2 seconds
	//Sleep(2000);

	//Dest.Print Spc(2); "Initializing TrueVision3D..."
	//DoEvents

	//Simcore->IsRunning = true;
	//EnableCollisions = true;

	delete Simcore;
	Simcore = 0;
	
}

void MainLoop()
{
	Simcore->input();
	Simcore->render();
}

int main (int argc, char* argv[])
{
    object_reg = csInitializer::CreateEnvironment (argc, argv);
	startsim();
	csInitializer::DestroyApplication (object_reg);
	return 0;
}

