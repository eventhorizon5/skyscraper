/* $Id$ */

/*
	Scalable Building Simulator - Call Button Subsystem Class
	The Skyscraper Project - Version 1.1 Alpha
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

//#include <wx/wx.h>
#include <crystalspace.h>
#include "callbutton.h"
#include "sbs.h"
#include "camera.h"
#include "elevator.h"

extern SBS *sbs; //external pointer to the SBS engine

CallButton::CallButton()
{

}

CallButton::~CallButton()
{

}

void CallButton::Create(int floornum, const char *BackTexture, const char *UpButtonTexture, const char *DownButtonTexture, double CenterX, double CenterZ, double voffset, const char *direction, double BackWidth, double BackHeight, bool ShowBack, double tw, double th)
{
	//create a set of call buttons

	floor = floornum;

}

void CallButton::Enabled(bool value)
{

}

void CallButton::Call(int direction)
{
	//calls the closest elevator in the elevator array list to the current floor,
	//and also depending on the direction it's travelling

	int closest = sbs->ElevatorArray[Elevators[0]]->GetFloor();
	int closest_elev = 0;

	for (int i = Elevators[0]; i < Elevators.GetSize(); i++)
	{
		int current = sbs->ElevatorArray[Elevators[i]]->GetFloor();
		
		//if elevator is closer than the previously checked one
		if (abs(current) < closest)
		{
			//and if it's above the current floor and should be called down, or below the
			//current floor and called up
			if ((current > floor && direction == -1) || (current < floor && direction == 1))
			{
				//and if it's either going the same direction as the call or not moving at all
				if (sbs->ElevatorArray[Elevators[i]]->Direction == direction || sbs->ElevatorArray[Elevators[i]]->Direction == 0)
				{
					closest = current;
					closest_elev = i;
				}
			}
		}
	}

	if (sbs->ElevatorArray[Elevators[closest_elev]]->GetFloor() == floor)
		//open elevator if it's on the same floor
		sbs->ElevatorArray[Elevators[closest_elev]]->OpenDoors();
	else
		//otherwise add a route entry to this floor
		sbs->ElevatorArray[Elevators[closest_elev]]->AddRoute(floor, direction);
}
