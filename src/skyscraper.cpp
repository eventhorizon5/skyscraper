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

#include "sbs.h"
#include "floor.h"
#include "elevator.h"
#include "globals.h"

SBS *Simcore;
Floor *testfloor;

int main (int argc, char* argv[])
{
	//Create new simulator object
	Simcore = new SBS(argc, argv);
	
	//set default starting elevator
	Simcore->ElevatorNumber = 1;

	//set frame rate
	Simcore->FrameRate = 30;
	Simcore->FrameLimiter = true;

	//Start simulator
	if (Simcore->Initialize("Skyscraper 1.1 Alpha"))
	{
		LoadTritonCenter(); //temporary building loader
		Simcore->Start();
	}

	delete testfloor;
	delete Simcore;
	testfloor = 0;
	Simcore = 0;
	Cleanup();

	return 0;
}

void LoadTritonCenter()
{
	//This is a temporary function to load the Triton Center, since file loading
	//support is not implemented yet
	Simcore->LoadTexture("stone", "/lib/std/stone4.gif");
	
	Simcore->SetStartPosition(0, 5, -3);
	
	testfloor = new Floor(1);
	testfloor->FloorAltitude = 0;
	testfloor->FloorHeight = 20;
	testfloor->CrawlSpaceHeight = 0;
	testfloor->CreateWallBox("stone", 10, 10, 0, 0, 0, false, 0, 0);

	//Add lights
	Simcore->AddLight(0, -3, 5, 0, 10, 1, 0, 0);
	Simcore->AddLight(0, 3, 5, 0, 10, 0, 0, 1);
	Simcore->AddLight(0, 0, 5, -3, 10, 0, 1, 0);
	
}