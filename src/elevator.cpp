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
	Number = number;

	//create object meshes
	ElevatorMesh (sbs->engine->CreateSectorWallsMesh (area, "Elevator"));
	ElevatorTS = SCF_QUERY_INTERFACE (ElevatorMesh->GetMeshObject (), iThingState);
	ElevatorState = ElevatorTS->GetFactory ();

	FloorIndicator (sbs->engine->CreateSectorWallsMesh (area, "FloorIndicator"));
	FloorIndicatorTS = SCF_QUERY_INTERFACE (FloorIndicator->GetMeshObject (), iThingState);
	FloorIndicatorState = FloorIndicatorTS->GetFactory ();

	ElevatorDoorL (sbs->engine->CreateSectorWallsMesh (area, "ElevatorDoorL"));
	ElevatorDoorL_TS = SCF_QUERY_INTERFACE (ElevatorDoorL->GetMeshObject (), iThingState);
	ElevatorDoorL_State = ElevatorDoorL_TS->GetFactory ();

	ElevatorDoorR (sbs->engine->CreateSectorWallsMesh (area, "ElevatorDoorR"));
	ElevatorDoorR_TS = SCF_QUERY_INTERFACE (ElevatorDoorR->GetMeshObject (), iThingState);
	ElevatorDoorR_State = ElevatorDoorR_TS->GetFactory ();

	Plaque (sbs->engine->CreateSectorWallsMesh (area, "Plaque"));
	PlaqueTS = SCF_QUERY_INTERFACE (Plaque->GetMeshObject (), iThingState);
	PlaqueState = PlaqueTS->GetFactory ();

	CallButtonsUp (sbs->engine->CreateSectorWallsMesh (area, "CallButtonsUp"));
	CallButtonsUp_TS = SCF_QUERY_INTERFACE (CallButtonsUp->GetMeshObject (), iThingState);
	CallButtonsUp_State = CallButtonsUp_TS->GetFactory ();

	CallButtonsDown (sbs->engine->CreateSectorWallsMesh (area, "CallButtonsDown"));
	CallButtonsDown_TS = SCF_QUERY_INTERFACE (CallButtonsDown->GetMeshObject (), iThingState);
	CallButtonsDown_State = CallButtonsDown_TS->GetFactory ();

	/*Set ElevatorMesh = Scene.CreateMeshBuilder("Elevator" + Str$(Number))
	Set FloorIndicator = Scene.CreateMeshBuilder("FloorIndicator" + Str$(Number))
	Set ElevatorInsDoorL = Scene.CreateMeshBuilder("ElevatorInsDoorL" + Str$(Number))
	Set ElevatorInsDoorR = Scene.CreateMeshBuilder("ElevatorInsDoorR" + Str$(Number))
	Set Plaque = Scene.CreateMeshBuilder("Plaque" + Str$(Number))
	Set CallButtonsUp = Scene.CreateMeshBuilder("CallButtonsUp" + Str$(Number))
	Set CallButtonsDown = Scene.CreateMeshBuilder("CallButtonsDown" + Str$(Number))
	//Set ElevatorMusic = New TV3DMedia.TVSoundWave3D
	//Set ElevatorSounds = New TV3DMedia.TVSoundWave3D
	*/

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

	int v;
	iMaterialWrapper* tm;
	
	//ElevatorMesh.SetPosition X, Floor(FloorID).FloorAltitude, Y
	//FloorIndicator.SetPosition X, Floor(FloorID).FloorAltitude, Y
	//Plaque.SetPosition X, Floor(FloorID).FloorAltitude, Y
	//ElevatorInsDoorL.SetPosition X, Floor(FloorID).FloorAltitude, Y
	//ElevatorInsDoorR.SetPosition X, Floor(FloorID).FloorAltitude, Y

	if (direction == 0)
		v = 1;
	if (direction == 1)
		v = -1;

    //Elevator
	AddFloor("Wood2", v * 15.5, 0, 0, -13.9, 0.1, 1, 1);
    AddFloor("Elev1", v * 15.5, 0, 0, -13.9, 19.5, 2, 2);
    AddWall("Wood1", v * 15.5, 0, 0, 0, 19.5, 0.1, 2, 2);
    AddWall("Wood1", v * 15.5, -13.9, 0, -13.9, 19.5, 0.1, 2, 2);
    AddWall("Wood1", v * 15.5, -13.9, v * 15.5, 0, 19.5, 0.1, 2, 2);

    //Floor Indicator
    if (direction == 0)
		AddFloorIndicator("ButtonL", v * 0.16, -0.5, v * 0.16, -2.5, 1.5, 16, -1, 1);
    if (direction == 1)
		AddFloorIndicator("ButtonL", v * 0.16, -13.9 + 0.5, v * 0.16, -13.9 + 2.5, 1.5, 16, -1, 1);

    //Button Panel
    if (direction == 0)
		AddButtonPanel("ElevExtPanels", v * 0.16, -0.3, v * 0.16, -2.7, 7, 6, 1, 1);
    if (direction == 1)
		AddButtonPanel("ElevExtPanels", v * 0.16, -13.9 + 0.3, v * 0.16, -13.9 + 2.7, 7, 6, 1, 1);

	//Plaque
    if (direction == 0)
		AddPlaque("Plaque", v * 0.16, -0.3, v * 0.16, -2.7, 1, 13, -1, 1);
    if (direction == 1)
		AddPlaque("Plaque", v * 0.16, -13.9 + 0.3, v * 0.16, -13.9 + 2.7, 1, 13, -1, 1);
    //Plaque.SetBlendingMode (TV_BLEND_ALPHA)
    //Plaque.SetColor RGBA(1, 1, 1, 0.1)

	//Interior Panels
    AddPanels("Marble3", v * 0.15, -14, v * 0.15, -11, 19.5, 0.1, 1, 1);
    AddPanels("Marble3", v * 0.15, 0, v * 0.15, -3, 19.5, 0.1, 1, 1);
    
	//Interior Doors
    AddDoors("ElevDoors", v * 0.1, -10.95, v * 0.1, -7.05, 19.5, 0.1, 1, 1);
    AddDoors("ElevDoors", v * 0.1, -6.95, v * 0.1, -2.95, 19.5, 0.1, 1, 1);
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

void Elevator::AddWall(const char *texture, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th)
{
	sbs->AddWall(ElevatorState, texture, x1, z1, x2, z2, height, voffset + GetPosition().y, tw, th);
}

void Elevator::AddFloor(const char *texture, float x1, float z1, float x2, float z2, float voffset, float tw, float th)
{
   	sbs->AddFloor(ElevatorState, texture, x1, z1, x2, z2, voffset + GetPosition().y, tw, th);
}

void Elevator::AddFloorIndicator(const char *texture, float x1, float z1, float x2, float z2, float height, float voffset)
{
	sbs->AddWall(FloorIndicatorState, texture, x1, z1, x2, z2, height, voffset + GetPosition().y, 0, 0);
}

void Elevator::AddButtonPanel(const char *texture, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th)
{
	sbs->AddWall(ElevatorState, texture, x1, z1, x2, z2, height, voffset + GetPosition().y, tw, th);
}

void Elevator::AddPanels(const char *texture, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th)
{
	sbs->AddWall(ElevatorState, texture, x1, z1, x2, z2, height, voffset + GetPosition().y, tw, th);
}

void Elevator::AddDoors(const char *texture, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th)
{
	sbs->AddWall(ElevatorDoorL_State, texture, x1, z1, x2, z2, height, voffset + GetPosition().y, tw, th);
	//sbs->AddWall(ElevatorDoorR_State, texture, x1, z1, x2, z2, height, voffset + GetPosition().y, tw, th);
}

void Elevator::AddPlaque(const char *texture, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th)
{
	sbs->AddWall(PlaqueState, texture, x1, z1, x2, z2, height, voffset + GetPosition().y, tw, th);
}

void Elevator::AddCallButtons(const char *texture, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th)
{
	sbs->AddWall(CallButtonsUp_State, texture, x1, z1, x2, z2, height, voffset + GetPosition().y, tw, th);
	//sbs->AddWall(CallButtonsDown_State, texture, x1, z1, x2, z2, height, voffset + GetPosition().y, tw, th);
}

csVector3 Elevator::GetPosition()
{
	//don't know how to get the mesh's vector position
}
