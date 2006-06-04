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
#include "globals.h"

SBS *Simcore;

int main (int argc, char* argv[])
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
	//Simcore->BuildingFile = "buildings/Simple2.bld";
	Simcore->BuildingFile = "buildings/Glass Tower.bld";

	//start simulation
	Simcore->Start();

	//clean up
	delete Simcore;
	Simcore = 0;
	Cleanup();

	return 0;
}
