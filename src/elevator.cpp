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
#include "shaft.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine
extern Camera *c; //external pointer to the camera

char intbuffer[65];
char buffer[20];

Elevator::Elevator(int number)
{
	csString buffer;
	
	//set elevator number
	Number = number;

	//init variables
	Name = "";
	QueuePositionDirection = 0;
	PauseQueueSearch = false;
	ElevatorSpeed = 0;
	ElevatorSync = false;
	MoveElevator = false;
	MoveElevatorFloor = 0;
	GotoFloor = 0;
	OpenDoor = 0;
	Acceleration = 0;
	Deceleration = 0;
	OpenSpeed = 0;
	UpQueue = "";
	DownQueue = "";
	ElevatorStart = 0;
	ElevatorFloor = 0;
	DoorsOpen = false;
	ElevatorDirection = 0;
	DistanceToTravel = 0;
	Destination = 0;
	ElevatorRate = 0;
	StoppingDistance = 0;
	CalculateStoppingDistance = false;
	Brakes = false;
	ElevatorDoorSpeed = 0;
	ElevatorDoorPos = 0;
	ElevWait = false;

	//create object meshes
	buffer = Number;
	buffer.Insert(0, "Elevator ");
	buffer.Trim();
	ElevatorMesh = (sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData()));
	Elevator_object = ElevatorMesh->GetMeshObject ();
	Elevator_factory = Elevator_object->GetFactory();
	Elevator_state = scfQueryInterface<iThingFactoryState> (Elevator_factory);
	ElevatorMesh->SetZBufMode(CS_ZBUF_USE);

	buffer = Number;
	buffer.Insert(0, "FloorIndicator ");
	buffer.Trim();
	FloorIndicator = (sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData()));
	FloorIndicator_object = FloorIndicator->GetMeshObject ();
	FloorIndicator_factory = FloorIndicator_object->GetFactory();
	FloorIndicator_state = scfQueryInterface<iThingFactoryState> (FloorIndicator_factory);
	FloorIndicator->SetZBufMode(CS_ZBUF_USE);

	buffer = Number;
	buffer.Insert(0, "ElevatorDoorL ");
	buffer.Trim();
	ElevatorDoorL = (sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData()));
	ElevatorDoorL_object = ElevatorDoorL->GetMeshObject ();
	ElevatorDoorL_factory = ElevatorDoorL_object->GetFactory();
	ElevatorDoorL_state = scfQueryInterface<iThingFactoryState> (ElevatorDoorL_factory);
	ElevatorDoorL->SetZBufMode(CS_ZBUF_USE);

	buffer = Number;
	buffer.Insert(0, "ElevatorDoorR ");
	buffer.Trim();
	ElevatorDoorR = (sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData()));
	ElevatorDoorR_object = ElevatorDoorR->GetMeshObject ();
	ElevatorDoorR_factory = ElevatorDoorR_object->GetFactory();
	ElevatorDoorR_state = scfQueryInterface<iThingFactoryState> (ElevatorDoorR_factory);
	ElevatorDoorR->SetZBufMode(CS_ZBUF_USE);

	buffer = Number;
	buffer.Insert(0, "Plaque ");
	buffer.Trim();
	Plaque = (sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData()));
	Plaque_object = Plaque->GetMeshObject ();
	Plaque_factory = Plaque_object->GetFactory();
	Plaque_state = scfQueryInterface<iThingFactoryState> (Plaque_factory);
	Plaque->SetZBufMode(CS_ZBUF_USE);
}

Elevator::~Elevator()
{
	//Destructor
	if (Plaque)
		delete Plaque;

	if (ElevatorDoorR)
		delete ElevatorDoorR;

	if (ElevatorDoorL)
		delete ElevatorDoorL;

	if (FloorIndicator)
		delete FloorIndicator;

	if (ElevatorMesh)
		delete ElevatorMesh;
}

void Elevator::CreateElevator(double x, double y, int floor, int direction)
{
	//Creates elevator at specified location and floor
	//x is the horiz position where the door is
	//y is the northmost position
	//Direction: 0=x is left side, 1=x is right side

	csVector3 vpos (x, sbs->FloorArray[floor]->Altitude, y);

	//move objects to positions
	ElevatorMesh->GetMovable()->SetPosition(vpos);
	ElevatorMesh->GetMovable()->UpdateMove();
	FloorIndicator->GetMovable()->SetPosition(vpos);
	FloorIndicator->GetMovable()->UpdateMove();
	Plaque->GetMovable()->SetPosition(vpos);
	Plaque->GetMovable()->UpdateMove();
	ElevatorDoorL->GetMovable()->SetPosition(vpos);
	ElevatorDoorL->GetMovable()->UpdateMove();
	ElevatorDoorR->GetMovable()->SetPosition(vpos);
	ElevatorDoorR->GetMovable()->UpdateMove();
	
	sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": created at " + csString(_gcvt(x, 12, buffer)) + ", " + csString(_gcvt(y, 12, buffer)) + ", " + csString(_itoa(floor, buffer, 12)) + ", " + csString(_itoa(direction, buffer, 12)));
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
	//elevator alarm code

	sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": alarm on");
}

void Elevator::CallElevator(int floor, int direction)
{
	//Calls elevator from specified floor, and gives desired direction to travel

	csString direction2;
	if (direction == -1)
		direction2 = "down";
	else
		direction2 = "up";

	sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": calling to floor " + csString(_itoa(floor, intbuffer, 10)) + " direction " + direction2);
}

void Elevator::StopElevator()
{
	//Tells elevator to stop moving, no matter where it is

	sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": emergency stop");
}

void Elevator::OpenHatch()
{
	//Opens the elevator's upper escape hatch, allowing access to the shaft

	sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": opening hatch");
}

void Elevator::OpenDoorsEmergency()
{
	//Simulates manually prying doors open.
	//Slowly opens the elevator doors no matter where elevator is.
	//If lined up with shaft doors, then opens the shaft doors also

	sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": manually opening doors");
	MoveDoors(true, true);
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

	int i;
	for (i = -sbs->Basements; i <= sbs->TotalFloors; i++)
	{
		if (i < sbs->TotalFloors)
			if ((GetPosition().y >= sbs->FloorArray[i]->Altitude) && (GetPosition().y < sbs->FloorArray[i + 1]->Altitude))
				return i;
			if ((i == sbs->TotalFloors) && (GetPosition().y >= sbs->FloorArray[i]->Altitude))
				return i;
	}

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

	sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": manually closing doors");
	MoveDoors(false, true);
}

void Elevator::OpenDoors()
{
	//Opens elevator doors

	sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": opening doors");
	MoveDoors(true, false);
}

void Elevator::CloseDoors()
{
	//Closes elevator doors

	sbs->Report("Elevator " + csString(_itoa(Number, intbuffer, 10)) + ": closing doors");
	MoveDoors(false, false);
}

void Elevator::MoveDoors(bool open, bool emergency)
{

}

void Elevator::MoveElevatorToFloor()
{
	//Main processing routine; sends elevator to floor specified in GotoFloor

}

int Elevator::AddWall(const char *texture, double x1, double z1, double x2, double z2, double height1, double height2, double voffset1, double voffset2, double tw, double th, bool DrawBothSides)
{
	return sbs->AddWallMain(Elevator_state, texture, x1, z1, x2, z2, height1, height2, voffset1 + GetPosition().y, voffset2 + GetPosition().y, tw, th, false, false, false, DrawBothSides);
}

int Elevator::AddFloor(const char *texture, double x1, double z1, double x2, double z2, double voffset1, double voffset2, double tw, double th)
{
   	return sbs->AddFloorMain(Elevator_state, texture, x1, z1, x2, z2, voffset1 + GetPosition().y, voffset2 + GetPosition().y, tw, th);
}

int Elevator::AddFloorIndicator(const char *basename, double x1, double z1, double x2, double z2, double height, double voffset, double tw, double th)
{
	//Creates a floor indicator at the specified location
	csString texture;
	BaseName = basename;
	texture = BaseName + sbs->FloorArray[OriginFloor]->ID;
	return sbs->AddWallMain(FloorIndicator_state, texture.GetData(), x1, z1, x2, z2, height, height, voffset + GetPosition().y, voffset + GetPosition().y, 0, 0, false, false, false, false);
}

int Elevator::AddDoors(const char *texture, double CenterX, double CenterZ, double width, double height, bool direction, double tw, double th)
{
	//adds elevator doors
	double x1, x2, x3, x4;
	double z1, z2, z3, z4;

	DoorDirection = direction;

	if (direction == false)
	{
		x1 = CenterX;
		x2 = CenterX;
		x3 = CenterX;
		x4 = CenterX;
		z1 = CenterZ - (width / 2);
		z2 = CenterZ;
		z3 = CenterZ;
		z4 = CenterZ + (width / 2);
	}
	else
	{
		x1 = CenterX - (width / 2);
		x2 = CenterX;
		x3 = CenterX;
		x4 = CenterX + (width / 2);
		z1 = CenterZ;
		z2 = CenterZ;
		z3 = CenterZ;
		z4 = CenterZ;
	}

	int firstidx = sbs->AddWallMain(ElevatorDoorL_state, texture, x1, z1, x2, z2, height, height, GetPosition().y, GetPosition().y, tw, th, false, false, false);
	sbs->AddWallMain(ElevatorDoorR_state, texture, x3, z3, x4, z4, height, height, GetPosition().y, GetPosition().y, tw, th, false, false, false);
	return firstidx;
}

int Elevator::AddPlaque(const char *texture, double x1, double z1, double x2, double z2, double height, double voffset, double tw, double th)
{
	return sbs->AddWallMain(Plaque_state, texture, x1, z1, x2, z2, height, height, voffset + GetPosition().y, voffset + GetPosition().y, tw, th, false, false, false);
}

const csVector3 Elevator::GetPosition()
{
	//returns the elevator's position
	return ElevatorMesh->GetMovable()->GetPosition();
}
