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

#include <crystalspace.h>
#include "skyscraper.h"
#include "sbs.h"
#include "floor.h"
#include "elevator.h"
#include "globals.h"

SBS *Simcore;
Floor *testfloor;

int main (int argc, char* argv[])
{
	srand (time (0));
	
	//Create new simulator object
	Simcore = new SBS();
	
	//set default starting elevator
	Simcore->ElevatorNumber = 1;

	//set frame rate
	Simcore->FrameRate = 30;
	Simcore->FrameLimiter = true;

	if (!Simcore->Initialize(argc, argv, "Skyscraper 1.1 Alpha"))
	{
		Simcore->ReportError("Error initializing system!");
		Cleanup();
		exit(1);
	}

	LoadTritonCenter(); //temporary building loader
	Simcore->Start();

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
	
	testfloor = new Floor(1, "L");
	testfloor->FloorAltitude = 0;
	testfloor->FloorHeight = 20;
	testfloor->CrawlSpaceHeight = 0;
	testfloor->CreateWallBox("stone", 10, 10, 0, 0, 0, false, 0, 0);

	//Add lights
	Simcore->AddLight(0, -3, 5, 0, 10, 1, 0, 0);
	Simcore->AddLight(0, 3, 5, 0, 10, 0, 0, 1);
	Simcore->AddLight(0, 0, 5, -3, 10, 0, 1, 0);
	
}