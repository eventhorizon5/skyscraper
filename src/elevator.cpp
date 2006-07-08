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
	Elevator_movable = ElevatorMesh->GetMovable();
	ElevatorMesh->SetZBufMode(CS_ZBUF_USE);

	buffer = Number;
	buffer.Insert(0, "FloorIndicator ");
	buffer.Trim();
	FloorIndicator = (sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData()));
	FloorIndicator_object = FloorIndicator->GetMeshObject ();
	FloorIndicator_factory = FloorIndicator_object->GetFactory();
	FloorIndicator_state = scfQueryInterface<iThingFactoryState> (FloorIndicator_factory);
	FloorIndicator_movable = FloorIndicator->GetMovable();
	FloorIndicator->SetZBufMode(CS_ZBUF_USE);

	buffer = Number;
	buffer.Insert(0, "ElevatorDoorL ");
	buffer.Trim();
	ElevatorDoorL = (sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData()));
	ElevatorDoorL_object = ElevatorDoorL->GetMeshObject ();
	ElevatorDoorL_factory = ElevatorDoorL_object->GetFactory();
	ElevatorDoorL_state = scfQueryInterface<iThingFactoryState> (ElevatorDoorL_factory);
	ElevatorDoorL_movable = ElevatorDoorL->GetMovable();
	ElevatorDoorL->SetZBufMode(CS_ZBUF_USE);

	buffer = Number;
	buffer.Insert(0, "ElevatorDoorR ");
	buffer.Trim();
	ElevatorDoorR = (sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData()));
	ElevatorDoorR_object = ElevatorDoorR->GetMeshObject ();
	ElevatorDoorR_factory = ElevatorDoorR_object->GetFactory();
	ElevatorDoorR_state = scfQueryInterface<iThingFactoryState> (ElevatorDoorR_factory);
	ElevatorDoorR_movable = ElevatorDoorR->GetMovable();
	ElevatorDoorR->SetZBufMode(CS_ZBUF_USE);

	buffer = Number;
	buffer.Insert(0, "Plaque ");
	buffer.Trim();
	Plaque = (sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData()));
	Plaque_object = Plaque->GetMeshObject ();
	Plaque_factory = Plaque_object->GetFactory();
	Plaque_state = scfQueryInterface<iThingFactoryState> (Plaque_factory);
	Plaque_movable = Plaque->GetMovable();
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
	Elevator_movable->SetPosition(vpos);
	Elevator_movable->UpdateMove();
	FloorIndicator_movable->SetPosition(vpos);
	FloorIndicator_movable->UpdateMove();
	Plaque_movable->SetPosition(vpos);
	Plaque_movable->UpdateMove();
	ElevatorDoorL_movable->SetPosition(vpos);
	ElevatorDoorL_movable->UpdateMove();
	ElevatorDoorR_movable->SetPosition(vpos);
	ElevatorDoorR_movable->UpdateMove();
	
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

	//check to see if person is in elevator
	CheckElevator();

/*
	if (OpenDoor == 1)
		OpenDoors();
	if (OpenDoor == -1)
		CloseDoors();
	if (OpenDoor == 2)
		OpenDoorsEmergency();
	if (OpenDoor == -2)
		CloseDoorsEmergency();
*/
	if (MoveElevator == true)
		MoveElevatorToFloor();

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
	static bool IsRunning;

	if (IsRunning == false)
	{
		IsRunning = true;
		
		//get elevator's current altitude
		ElevatorStart = GetPosition().y;

		//get elevator's current floor
		ElevatorFloor = GetElevatorFloor();

		//If elevator is already on specified floor, open doors and exit
		if (ElevatorFloor == GotoFloor)
		{
			OpenDoor = 1;
			return;
		}

		//close doors if open
		if (DoorsOpen == true)
			OpenDoor = -1;

		//Determine direction
		if (GotoFloor < ElevatorFloor)
			ElevatorDirection = -1;
		if (GotoFloor > ElevatorFloor)
			ElevatorDirection = 1;

		//Determine distance to destination floor
		DistanceToTravel = abs(abs(sbs->FloorArray[GotoFloor]->Altitude) - abs(ElevatorStart));
		Destination = sbs->FloorArray[GotoFloor]->Altitude;
		CalculateStoppingDistance = true;

		//If user is riding this elevator, then turn off floor
		if (ElevatorSync == true)
			sbs->FloorArray[c->CurrentFloor]->Enabled(false);

		//Turn off sky, buildings, and landscape
		sbs->EnableSkybox(false);
		sbs->EnableBuildings(false);
		sbs->EnableLandscape(false);

		//Play starting sound
		//"\data\elevstart.wav"

		//Get first rate increment value
		ElevatorRate = ElevatorDirection * (ElevatorSpeed * Acceleration);

		//get starting frame rate and hold value
		FPSModifierStatic = sbs->FPSModifier;
	}

	//Movement sound
    //"\data\elevmove.wav"

	//move elevator objects and camera
	Elevator_movable->MovePosition(csVector3(0, ElevatorRate * FPSModifierStatic, 0));
	Elevator_movable->UpdateMove();
	if (ElevatorSync == true)
		c->SetPosition(csVector3(c->GetPosition().x, GetPosition().y + c->DefaultAltitude, c->GetPosition().z));
	ElevatorDoorL_movable->MovePosition(csVector3(0, ElevatorRate * FPSModifierStatic, 0));
	ElevatorDoorL_movable->UpdateMove();
	ElevatorDoorR_movable->MovePosition(csVector3(0, ElevatorRate * FPSModifierStatic, 0));
	ElevatorDoorR_movable->UpdateMove();
	FloorIndicator_movable->MovePosition(csVector3(0, ElevatorRate * FPSModifierStatic, 0));
	FloorIndicator_movable->UpdateMove();
	Plaque_movable->MovePosition(csVector3(0, ElevatorRate * FPSModifierStatic, 0));
	Plaque_movable->UpdateMove();

	//move sounds

	//motion calculation
	if (Brakes == false)
	{
		//regular motion
		if (ElevatorDirection == 1)
			ElevatorRate = ElevatorRate + (ElevatorSpeed * Acceleration);
		if (ElevatorDirection == -1)
			ElevatorRate = ElevatorRate - (ElevatorSpeed * Acceleration);
	}
	else
	{
		//slow down
		if (ElevatorDirection == 1)
			ElevatorRate = ElevatorRate + (ElevatorSpeed * TempDeceleration);
		if (ElevatorDirection == -1)
			ElevatorRate = ElevatorRate - (ElevatorSpeed * TempDeceleration);
	}

	//change speeds
	if ((ElevatorDirection == 1) && (ElevatorRate > ElevatorSpeed))
		ElevatorRate = ElevatorSpeed;
	if ((ElevatorDirection == -1) && (ElevatorRate < -ElevatorSpeed))
		ElevatorRate = -ElevatorSpeed;
	if ((ElevatorDirection == 1) && (Brakes == true) && (ElevatorRate > 0))
		ElevatorRate = 0;
	if ((ElevatorDirection == -1) && (Brakes == true) && (ElevatorRate < 0))
		ElevatorRate = 0;

	//get distance needed to stop elevator
	if (CalculateStoppingDistance == true)
	{
		if (ElevatorDirection == 1)
			StoppingDistance = (GetPosition().y - ElevatorStart) * (Acceleration / Deceleration);
		else if (ElevatorDirection == -1)
			StoppingDistance = (ElevatorStart - GetPosition().y) * (Acceleration / Deceleration);
	}

	if (abs(ElevatorRate) == ElevatorSpeed)
		CalculateStoppingDistance = false;

	//Deceleration routines with floor overrun correction (there's still problems, but it works pretty good)
	//since this function cycles at a slower/less granular rate (cycles according to frames per sec), an error factor is present where the elevator overruns the dest floor,
	//even though the algorithms are all correct. Since the elevator moves by "jumping" to a new altitude every frame - and usually jumps right over the altitude value where it is supposed to
	//start the deceleration process, causing the elevator to decelerate too late, and end up passing/overrunning the dest floor's altitude.  This code corrects this problem
	//by determining if the next "jump" will overrun the deceleration marker (which is Dest's Altitude minus Stopping Distance), and temporarily altering the deceleration rate according to how far off the mark it is
	//and then starting the deceleration process immediately.

	//up movement
	if ((Brakes == false) && (ElevatorDirection == 1))
	{
		//determine if next jump altitude is over deceleration marker
		if (((GetPosition().y + (ElevatorRate * FPSModifierStatic)) > (Destination - StoppingDistance)) && (GetPosition().y != (Destination - StoppingDistance)))
		{
			CalculateStoppingDistance = false;
			//recalculate deceleration value based on distance from marker, and store result in tempdeceleration
			TempDeceleration = Deceleration * (StoppingDistance / (Destination - GetPosition().y));
			//start deceleration
			ElevatorDirection = -1;
			Brakes = true;
			//stop sounds
			//play elevator stopping sound
			//"\data\elevstop.wav"
		}
    
		//normal routine - this will rarely happen (only if the elevator happens to reach the exact deceleration marker)
		if (GetPosition().y == (Destination - StoppingDistance))
		{
			TempDeceleration = Deceleration;
			CalculateStoppingDistance = false;
			//slow down elevator
			ElevatorDirection = -1;
			Brakes = true;
			//stop sounds
			//play stopping sound
			//"\data\elevstop.wav"
		}
	}

	//down movement
	if (Brakes == false && ElevatorDirection == -1)
	{
        //determine if next jump altitude is below deceleration marker
		if (((GetPosition().y - (ElevatorRate * FPSModifierStatic)) < (Destination + StoppingDistance)) && (GetPosition().y != (Destination + StoppingDistance)))
		{
			CalculateStoppingDistance = false;
			//recalculate deceleration value based on distance from marker, and store result in tempdeceleration
			TempDeceleration = Deceleration * (StoppingDistance / (GetPosition().y - Destination));
			//start deceleration
			ElevatorDirection = 1;
			Brakes = true;
			//stop sounds
			//play stopping sound
            //"\data\elevstop.wav"
		}
    
		//normal routine - this will rarely happen (only if the elevator happens to reach the exact deceleration marker)
		if (GetPosition().y == (Destination + StoppingDistance))
		{
			TempDeceleration = Deceleration;
			CalculateStoppingDistance = false;
			//slow down elevator
			ElevatorDirection = 1;
			Brakes = true;
			//stop sounds
			//play stopping sound
			//"\data\elevstop.wav"
		}
	}

	//exit if elevator's running
	if (abs(ElevatorRate) != 0)
		return;

	//store error offset value
	if (ElevatorDirection == -1)
		ErrorOffset = GetPosition().y - Destination;
	else if (ElevatorDirection == 1)
		ErrorOffset = Destination - GetPosition().y;
	else
		ErrorOffset = 0;

	//set elevator and objects to floor altitude (corrects offset errors)
	//move elevator objects and camera
	Elevator_movable->SetPosition(csVector3(GetPosition().x, Destination, GetPosition().z));
	Elevator_movable->UpdateMove();
	if (ElevatorSync == true)
		c->SetPosition(csVector3(c->GetPosition().x, GetPosition().y + c->DefaultAltitude, c->GetPosition().z));
	ElevatorDoorL_movable->MovePosition(csVector3(ElevatorDoorL_movable->GetPosition().x, Destination, ElevatorDoorL_movable->GetPosition().z));
	ElevatorDoorL_movable->UpdateMove();
	ElevatorDoorR_movable->MovePosition(csVector3(ElevatorDoorR_movable->GetPosition().x, Destination, ElevatorDoorR_movable->GetPosition().z));
	ElevatorDoorR_movable->UpdateMove();
	FloorIndicator_movable->MovePosition(csVector3(FloorIndicator_movable->GetPosition().x, Destination, FloorIndicator_movable->GetPosition().z));
	FloorIndicator_movable->UpdateMove();
	Plaque_movable->MovePosition(csVector3(Plaque_movable->GetPosition().x, Destination, Plaque_movable->GetPosition().z));
	Plaque_movable->UpdateMove();

	//move sounds

	//reset values if at destination floor
	ElevatorRate = 0;
	ElevatorDirection = 0;
	Brakes = false;
	Destination = 0;
	DistanceToTravel = 0;
	ElevatorStart = 0;
	IsRunning = false;
	MoveElevator = false;

	//update elevator's floor number
	GetElevatorFloor();

	//Turn on floor
	if (ElevatorSync == true)
		sbs->FloorArray[GotoFloor]->Enabled(true);

	//open doors
	OpenDoor = 1;

	//Turn on sky, buildings, and landscape
	sbs->EnableSkybox(true);
	sbs->EnableBuildings(true);
	sbs->EnableLandscape(true);

}

int Elevator::AddWall(const char *texture, double x1, double z1, double x2, double z2, double height1, double height2, double voffset1, double voffset2, double tw, double th, bool revX, bool revY, bool revZ, bool DrawBothSides)
{
	return sbs->AddWallMain(Elevator_state, texture, x1, z1, x2, z2, height1, height2, voffset1 + GetPosition().y, voffset2 + GetPosition().y, tw, th, revX, revY, revZ, DrawBothSides);
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
	return Elevator_movable->GetPosition();
}

void Elevator::CheckElevator()
{
	//check to see if user (camera) is in the elevator

}
