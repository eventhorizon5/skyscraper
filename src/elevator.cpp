/*
	Scalable Building Simulator - Elevator Subsystem Class
	The Skyscraper Project - Version 1.1 Alpha
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
#include "elevator.h"
#include "sbs.h"
#include "camera.h"

extern SBS *sbs; //external pointer to the SBS engine
extern Camera *c; //external pointer to the camera

Elevator::Elevator(int number)
{
	Number = number;

	//create object meshes
	ElevatorMesh = (sbs->engine->CreateSectorWallsMesh (sbs->area, "Elevator"));
	Elevator_object = ElevatorMesh->GetMeshObject ();
	Elevator_factory = Elevator_object->GetFactory();
	Elevator_state = scfQueryInterface<iThingFactoryState> (Elevator_factory);
	ElevatorMesh->SetZBufMode(CS_ZBUF_USE);

	FloorIndicator = (sbs->engine->CreateSectorWallsMesh (sbs->area, "FloorIndicator"));
	FloorIndicator_object = FloorIndicator->GetMeshObject ();
	FloorIndicator_factory = FloorIndicator_object->GetFactory();
	FloorIndicator_state = scfQueryInterface<iThingFactoryState> (FloorIndicator_factory);
	FloorIndicator->SetZBufMode(CS_ZBUF_USE);

	ElevatorDoorL = (sbs->engine->CreateSectorWallsMesh (sbs->area, "ElevatorDoorL"));
	ElevatorDoorL_object = ElevatorDoorL->GetMeshObject ();
	ElevatorDoorL_factory = ElevatorDoorL_object->GetFactory();
	ElevatorDoorL_state = scfQueryInterface<iThingFactoryState> (ElevatorDoorL_factory);
	ElevatorDoorL->SetZBufMode(CS_ZBUF_USE);

	ElevatorDoorR = (sbs->engine->CreateSectorWallsMesh (sbs->area, "ElevatorDoorR"));
	ElevatorDoorR_object = ElevatorDoorR->GetMeshObject ();
	ElevatorDoorR_factory = ElevatorDoorR_object->GetFactory();
	ElevatorDoorR_state = scfQueryInterface<iThingFactoryState> (ElevatorDoorR_factory);
	ElevatorDoorR->SetZBufMode(CS_ZBUF_USE);

	Plaque = (sbs->engine->CreateSectorWallsMesh (sbs->area, "Plaque"));
	Plaque_object = Plaque->GetMeshObject ();
	Plaque_factory = Plaque_object->GetFactory();
	Plaque_state = scfQueryInterface<iThingFactoryState> (Plaque_factory);
	Plaque->SetZBufMode(CS_ZBUF_USE);

	CallButtonsUp = (sbs->engine->CreateSectorWallsMesh (sbs->area, "CallButtonsUp"));
	CallButtonsUp_object = CallButtonsUp->GetMeshObject ();
	CallButtonsUp_factory = CallButtonsUp_object->GetFactory();
	CallButtonsUp_state = scfQueryInterface<iThingFactoryState> (CallButtonsUp_factory);
	CallButtonsUp->SetZBufMode(CS_ZBUF_USE);

	CallButtonsDown = (sbs->engine->CreateSectorWallsMesh (sbs->area, "CallButtonsDown"));
	CallButtonsDown_object = CallButtonsDown->GetMeshObject ();
	CallButtonsDown_factory = CallButtonsDown_object->GetFactory();
	CallButtonsDown_state = scfQueryInterface<iThingFactoryState> (CallButtonsDown_factory);
	CallButtonsDown->SetZBufMode(CS_ZBUF_USE);

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
	sbs->AddWallMain(Elevator_state, texture, x1, z1, x2, z2, height, height, voffset + GetPosition().y, voffset + GetPosition().y, tw, th);
}

void Elevator::AddFloor(const char *texture, float x1, float z1, float x2, float z2, float voffset, float tw, float th)
{
   	sbs->AddFloorMain(Elevator_state, texture, x1, z1, x2, z2, voffset + GetPosition().y, tw, th);
}

void Elevator::AddFloorIndicator(const char *texture, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th)
{
	sbs->AddWallMain(FloorIndicator_state, texture, x1, z1, x2, z2, height, height, voffset + GetPosition().y, voffset + GetPosition().y, 0, 0);
}

void Elevator::AddButtonPanel(const char *texture, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th)
{
	sbs->AddWallMain(Elevator_state, texture, x1, z1, x2, z2, height, height, voffset + GetPosition().y, voffset + GetPosition().y, tw, th);
}

void Elevator::AddPanels(const char *texture, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th)
{
	sbs->AddWallMain(Elevator_state, texture, x1, z1, x2, z2, height, height, voffset + GetPosition().y, voffset + GetPosition().y, tw, th);
}

void Elevator::AddDoors(const char *texture, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th)
{
	sbs->AddWallMain(ElevatorDoorL_state, texture, x1, z1, x2, z2, height, height, voffset + GetPosition().y, voffset + GetPosition().y, tw, th);
	//sbs->AddWallMain(ElevatorDoorR_state, texture, x1, z1, x2, z2, height, height, voffset + GetPosition().y, voffset + GetPosition().y, tw, th);
}

void Elevator::AddPlaque(const char *texture, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th)
{
	sbs->AddWallMain(Plaque_state, texture, x1, z1, x2, z2, height, height, voffset + GetPosition().y, voffset + GetPosition().y, tw, th);
}

void Elevator::AddCallButtons(const char *texture, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th)
{
	sbs->AddWallMain(CallButtonsUp_state, texture, x1, z1, x2, z2, height, height, voffset + GetPosition().y, voffset + GetPosition().y, tw, th);
	//sbs->AddWallMain(CallButtonsDown_state, texture, x1, z1, x2, z2, height, height, voffset + GetPosition().y, voffset + GetPosition().y, tw, th);
}

csVector3 Elevator::GetPosition()
{
	//don't know how to get the mesh's position
	return (0, 0, 0);
}
