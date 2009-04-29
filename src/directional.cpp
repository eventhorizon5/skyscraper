/* $Id$ */

/*
	Scalable Building Simulator - Directional Indicator Subsystem Class
	The Skyscraper Project - Version 1.4 Alpha
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

#include "globals.h"
#include "directional.h"
#include "sbs.h"
#include "elevator.h"

extern SBS *sbs; //external pointer to the SBS engine

DirectionalIndicator::DirectionalIndicator(int elevator, const char *BackTexture, const char *UpTexture, const char *DownTexture, float CenterX, float CenterZ, float voffset, const char *direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th)
{
	//create a directional indicator

	IsEnabled = true;
	Elevator = elevator;
}

DirectionalIndicator::~DirectionalIndicator()
{

}

void DirectionalIndicator::Enabled(bool value)
{
	//turns panel on/off
	if (value == true)
	{
		DirectionalMesh->GetFlags().Reset (CS_ENTITY_INVISIBLEMESH);
		DirectionalMesh->GetFlags().Reset (CS_ENTITY_NOSHADOWS);
		DirectionalMesh->GetFlags().Reset (CS_ENTITY_NOHITBEAM);
		IsEnabled = true;
	}
	else
	{
		DirectionalMesh->GetFlags().Set (CS_ENTITY_INVISIBLEMESH);
		DirectionalMesh->GetFlags().Set (CS_ENTITY_NOSHADOWS);
		DirectionalMesh->GetFlags().Set (CS_ENTITY_NOHITBEAM);
		IsEnabled = false;
	}
}

