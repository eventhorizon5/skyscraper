/*
	Scalable Building Simulator - Destination Dispatch Controller
	The Skyscraper Project - Version 1.12 Alpha
	Copyright (C)2004-2023 Ryan Thoryk
	https://www.skyscrapersim.net
	https://sourceforge.net/projects/skyscraper/
	Contact - ryan@thoryk.com

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

#include "globals.h"
#include "sbs.h"
#include "destination.h"

namespace SBS {

Destination::Destination(Object *parent, std::vector<int> &elevators, int floornum) : Object(parent)
{
	//create a set of call buttons

	floor = sbs->GetFloor(floornum);
	if (!floor)
		return;

	//set up SBS object
	SetValues("Destination", "", false);

	Elevators.resize(elevators.size());
	for (size_t i = 0; i < elevators.size(); i++)
		Elevators[i] = elevators[i];

	if (sbs->Verbose)
		Report("Created");
}

Destination::~Destination()
{

}

}
