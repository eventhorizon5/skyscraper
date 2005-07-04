//Scalable Building Simulator - Elevator Subsystem Class
//The Skyscraper Project - Version 1.1 Alpha
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

#include "elevator.h"
#include "sbs.h"

extern SBS *sbs; //external pointer to the SBS engine

Elevator::Elevator(int number)
{

}

Elevator::~Elevator()
{

}

void Elevator::CreateElevator(float x, float y, int floor, int direction)
{
	//Creates elevator at specified location and floor
	//x is the horiz position where the door is
	//y is the northmost position
	//Direction: 0=x is left side, 1=x is right side

}

void Elevator::AddRoute(int floor, int direction)
{
	//Add call route to elevator routing table, in sorted order
	//Direction values - 1=up, -1=down

}

void Elevator::DeleteRoute(int floor, int direction)
{
	//Delete call route from elevator routing table

}

void Elevator::Alarm()
{
	//elevator's alarm code

}

void Elevator::CallElevator(int floor, int direction)
{
	//Calls elevator from specified floor, and gives desired direction to travel

}

void Elevator::StopElevator()
{
	//Tells elevator to stop moving, no matter where it is

}

void Elevator::OpenHatch()
{
	//Opens the elevator's upper escape hatch, allowing access to the shaft

}

void Elevator::OpenDoorsEmergency()
{
	//Simulates manually prying doors open.
	//Slowly opens the elevator doors no matter where elevator is.
	//If lined up with shaft doors, then opens the shaft doors also

}

void Elevator::OpenShaftDoors(int floor)
{
	//Simulates manually opening shaft doors
	//Slowly opens the shaft's elevator doors no matter where elevator is.
	//Cannot be used with OpenDoorsEmergency.
	//This is if the elevator is not lined up with the shaft doors,
	//and the user needs to open the shaft doors, usually while on top of elevator.

}

void Elevator::ProcessCallQueue()
{
	//Processes the elevator's call queue, and sends elevators to called floors

}

int Elevator::GetElevatorFloor()
{
	//Determine floor that the elevator is on
	
	return 0;
}

void Elevator::MonitorLoop()
{
	//Monitors elevator and starts actions if needed

}

void Elevator::CloseDoorsEmergency()
{
	//Simulates manually closing doors.
	//Slowly closes the elevator doors no matter where elevator is.
	//If lined up with shaft doors, then closes the shaft doors also

}

void Elevator::OpenDoors()
{
	//Opens elevator doors

}

void Elevator::CloseDoors()
{
	//Closes elevator doors

}

void Elevator::MoveElevatorToFloor()
{
	//Main processing routine; sends elevator to floor specified in GotoFloor

}
