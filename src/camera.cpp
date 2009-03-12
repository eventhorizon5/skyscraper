/* $Id$ */

/*
	Scalable Building Simulator - Camera Object Class
	The Skyscraper Project - Version 1.3 Alpha
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
#include "callbutton.h"
#include "buttonpanel.h"
#include "camera.h"
#include "sbs.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

Camera::Camera()
{
	MainCamera = sbs->view->GetCamera();
	MainCamera->SetSector(sbs->area);

	//init variables
	CurrentFloor = 0;
	OriginalPosition = 0;
	StartFloor = 0;
	StartPositionX = 0;
	StartPositionZ = 0;
	StartDirection = csVector3(0, 0, 0);
	StartRotation = csVector3(0, 0, 0);
	FloorTemp = 0;
	velocity.Set(0, 0, 0);
	desired_velocity.Set(0, 0, 0);
	angle_velocity.Set(0, 0, 0);
	desired_angle_velocity.Set(0, 0, 0);
	cfg_jumpspeed = 0.08f;
	cfg_walk_accelerate = 0.040f;
	cfg_walk_maxspeed = 0.1f;
	cfg_walk_maxspeed_mult = 10.0f;
	cfg_walk_maxspeed_multreal = 1.0f;
	cfg_walk_brake = 0.040f;
	cfg_rotate_accelerate = 0.005f;
	cfg_rotate_maxspeed = 0.015f;
	cfg_rotate_brake = 0.015f;
	cfg_look_accelerate = 0.028f;
	cfg_body_height = 2.7f;
	cfg_body_width = 1.64f;
	cfg_body_depth = 1.64f;
	cfg_legs_height = 2.3f;
	cfg_legs_width = 1.312f;
	cfg_legs_depth = 1.312f;
	speed = 1;
	EnableCollisions = true;
	GravityStatus = true;
	SetGravity(sbs->MetersToFeet(9.806)); // 9.806 m/s/s
}

Camera::~Camera()
{
	//Destructor
}

void Camera::SetPosition(csVector3 vector)
{
	//sets the camera to an absolute position in 3D space
	MainCamera->GetTransform().SetOrigin(vector);
}

void Camera::SetDirection(csVector3 vector)
{
	//sets the camera's direction to an absolute position
	MainCamera->GetTransform().LookAt(vector, csVector3 (0, 1, 0));
}

void Camera::SetRotation(csVector3 vector)
{
	//sets the camera's rotation in degrees

	//keep rotation within 360 degree boundaries
	if (vector.x > 360)
		vector.x -= 360;
	if (vector.x < 0)
		vector.x += 360;
	if (vector.y > 360)
		vector.y -= 360;
	if (vector.y < 0)
		vector.y += 360;
	if (vector.z > 360)
		vector.z -= 360;
	if (vector.z < 0)
		vector.z += 360;

	//convert to radians
	csVector3 rot;
	rot.x = DegreesToRadians(vector.x);
	rot.y = DegreesToRadians(vector.y);
	rot.z = DegreesToRadians(vector.z);

	collider_actor.SetRotation(rot);
}

csVector3 Camera::GetPosition()
{
	//returns the camera's current position
	return MainCamera->GetTransform().GetOrigin();
}

csVector3 Camera::GetDirection()
{
	//returns the camera's current direction
	return csVector3(0, 0, 0);
}

csVector3 Camera::GetRotation()
{
	//returns the camera's current rotation
	float x = RadiansToDegrees(collider_actor.GetRotation().x);
	float y = RadiansToDegrees(collider_actor.GetRotation().y);
	float z = RadiansToDegrees(collider_actor.GetRotation().z);

	//keep values within the 0-360 range
	if (x > 360)
		x -= 360 * int(x / 360);
	if (x < 0)
		x = 360 + (x - (360 * int(x / 360)));
	if (y > 360)
		y -= 360 * int(y / 360);
	if (y < 0)
		y = 360 + (y - (360 * int(y / 360)));
	if (z > 360)
		z -= 360 * int(z / 360);
	if (z < 0)
		z = 360 + (z - (360 * int(z / 360)));

	return csVector3(x, y, z);
}

void Camera::UpdateCameraFloor()
{
	CurrentFloor = sbs->GetFloorNumber(MainCamera->GetTransform().GetOrigin().y);
}

bool Camera::Move(csVector3 vector, float speed)
{
	//moves the camera in a relative amount specified by a vector
	MainCamera->Move(vector * speed, EnableCollisions);
	return true;
}

void Camera::Rotate(csVector3 vector, float speed)
{
	//rotates the camera in a relative amount
	csVector3 rot = GetRotation() * vector * speed;

	SetRotation(rot);
}

void Camera::SetStartDirection(csVector3 vector)
{
	StartDirection = vector;
}

csVector3 Camera::GetStartDirection()
{
	return StartDirection;
}

void Camera::SetStartRotation(csVector3 vector)
{
	StartRotation = vector;
}

csVector3 Camera::GetStartRotation()
{
	return StartRotation;
}

void Camera::SetToStartPosition()
{
	SetPosition(csVector3(StartPositionX, sbs->GetFloor(StartFloor)->Altitude + sbs->GetFloor(StartFloor)->InterfloorHeight + cfg_body_height + cfg_legs_height, StartPositionZ));
}

void Camera::SetToStartDirection()
{
	SetDirection(StartDirection);
}

void Camera::SetToStartRotation()
{
	SetRotation(StartRotation);
}

void Camera::CheckElevator()
{
	//check to see if user (camera) is in an elevator

	//first checks to see if camera is within an elevator's height range, and then
	//checks for a collision with the elevator's floor below

	bool test = false;
	for (int i = 1; i <= sbs->Elevators(); i++)
	{
		if (sbs->GetElevator(i)->IsInElevator(GetPosition()) == true)
		{
			test = true;
			if (sbs->InElevator == false || sbs->ElevatorNumber != i)
			{
				sbs->GetElevator(i)->EnableObjects(true);
				sbs->GetElevator(i)->UpdateFloorIndicators();
			}
			sbs->InElevator = true;
			sbs->ElevatorNumber = i;
			sbs->ElevatorSync = true;
			return;
		}
		else if (sbs->InElevator == true && sbs->ElevatorNumber == i)
			sbs->GetElevator(i)->EnableObjects(false); //turn off objects if user is not in the checked elevator
	}
	//user is not in an elevator if all elevators returned false
	if (test == false)
	{
		sbs->InElevator = false;
		sbs->ElevatorSync = false;
	}
}

void Camera::CheckShaft()
{
	//check to see if user (camera) is in a shaft

	for (int i = 1; i <= sbs->Shafts(); i++)
	{
		if (sbs->GetShaft(i)->IsInShaft(GetPosition()) == true)
		{
			if (sbs->GetShaft(i)->InsideShaft == false && sbs->InElevator == false)
			{
				//user is in the shaft
				sbs->GetShaft(i)->InsideShaft = true;

				//turn on entire shaft
				sbs->GetShaft(i)->EnableWholeShaft(true, true);
			}
			else if (sbs->GetShaft(i)->InsideShaft == true && sbs->InElevator == true)
			{
				//user has moved from the shaft to an elevator
				sbs->GetShaft(i)->InsideShaft = false;

				//turn off entire shaft if ShowFullShafts is false
				if (sbs->ShowFullShafts == false && sbs->GetShaft(i)->ShowFullShaft == false)
					sbs->GetShaft(i)->EnableWholeShaft(false, true);
				else
					sbs->GetShaft(i)->EnableWholeShaft(true, true);
			}
			else if (sbs->GetShaft(i)->InsideShaft == false && sbs->InElevator == true && sbs->ShowFullShafts == false && sbs->GetShaft(i)->ShowFullShaft == false)
			{
				//if user is in an elevator, show a range of the shaft at a time (while it's moving)
				sbs->GetShaft(i)->EnableRange(CurrentFloor, sbs->ShaftDisplayRange, true, false);
				sbs->GetElevator(sbs->ElevatorNumber)->ShaftDoorsEnabledRange(CurrentFloor, sbs->ShaftDisplayRange);
			}

			if (sbs->GetShaft(i)->InsideShaft == false && sbs->InElevator == true && sbs->GetElevator(sbs->ElevatorNumber)->IsMoving == true)
			{
				//if specified, show floors or outside if user is in a moving elevator
				sbs->EnableFloorRange(CurrentFloor, sbs->FloorDisplayRange, true, true, i);

				if (sbs->GetShaft(i)->ShowOutside == true)
				{
					if (sbs->GetShaft(i)->ShowOutsideList.Find(CurrentFloor) != -1)
					{
						sbs->EnableSkybox(true);
						sbs->EnableBuildings(true);
						sbs->EnableLandscape(true);
						sbs->EnableExternal(true);
					}
					else
					{
						sbs->EnableSkybox(false);
						sbs->EnableBuildings(false);
						sbs->EnableLandscape(false);
						sbs->EnableExternal(false);
					}
				}
			}
		}
		else if (sbs->GetShaft(i)->InsideShaft == true)
		{
			//user has moved out of the shaft
			sbs->GetShaft(i)->InsideShaft = false;

			//turn off shaft
			if (sbs->GetShaft(i)->ShowFullShaft == false)
				sbs->GetShaft(i)->EnableWholeShaft(false, true);
		}
		else if (sbs->GetShaft(i)->InsideShaft == false)
		{
			//show specified shaft range if outside the shaft
			if (sbs->GetShaft(i)->ShowFullShaft == false)
				sbs->GetShaft(i)->EnableRange(CurrentFloor, sbs->ShaftOutsideDisplayRange, true, true);
			else
				sbs->GetShaft(i)->EnableWholeShaft(true, true);
		}
	}
}

void Camera::CheckStairwell()
{
	//check to see if user (camera) is in a stairwell

	for (int i = 1; i <= sbs->StairsNum(); i++)
	{
		if (sbs->GetStairs(i)->IsInStairwell(GetPosition()) == true)
		{
			if (sbs->GetStairs(i)->InsideStairwell == false)
			{
				sbs->GetStairs(i)->InsideStairwell = true;

				//turn on entire stairwell if ShowFullStairs is true
				if (sbs->ShowFullStairs == true)
					sbs->GetStairs(i)->EnableWholeStairwell(true);
			}
			if (sbs->GetStairs(i)->InsideStairwell == true)
			{
				//show specified stairwell range while in the stairwell
				sbs->GetStairs(i)->EnableRange(CurrentFloor, sbs->StairsDisplayRange);

				//if user walked to a different floor, enable new floor and disable previous
				if (CurrentFloor != FloorTemp)
				{
					sbs->GetFloor(FloorTemp)->Enabled(false);
					sbs->GetFloor(FloorTemp)->EnableGroup(false);
					sbs->GetFloor(CurrentFloor)->Enabled(true);
					sbs->GetFloor(CurrentFloor)->EnableGroup(true);
				}
			}
		}
		else if (sbs->GetStairs(i)->InsideStairwell == true)
		{
			sbs->GetStairs(i)->InsideStairwell = false;

			//turn off stairwell if ShowFullStairs is true
			if (sbs->ShowFullStairs == true)
				sbs->GetStairs(i)->EnableWholeStairwell(false);
		}
		else if (sbs->GetStairs(i)->InsideStairwell == false)
		{
			//show specified stairwell range if outside the stairwell
			sbs->GetStairs(i)->EnableRange(CurrentFloor, sbs->StairsOutsideDisplayRange);
		}
	}
	FloorTemp = CurrentFloor;
}

void Camera::ClickedObject()
{
	//some code and comments from the CrystalSpace manual
	//this returns the mesh that the user clicks on

	// Setup a 2D vector with our mouse position.  We invert the y
	// (based on vertical screen dimension) because CS assumes y=0
	// is down for 3D calculations.
	csVector2 v2d (sbs->mouse_x, MainCamera->GetShiftY() * 2 - sbs->mouse_y);

	// We calculate the inverse perspective of this 2D point at
	// z=100.  This results in a 3D position in camera space at
	// z=100 that directly corresponds to the 2D position we
	// clicked on.  We use z=100 to ensure that we will at least
	// hit all objects that are before that distance.
	csVector3 v3d = MainCamera->InvPerspective(v2d, 100);

	// We are going to cast a beam in the current sector of the
	// camera from our camera position in the direction of the
	// 'v3d' point.  First we transform the v3d camera space
	// location to world space.
	csVector3 startbeam = MainCamera->GetTransform().GetOrigin();
	csVector3 endbeam = MainCamera->GetTransform().This2Other(v3d);

	// Now do the actual intersection.
	csSectorHitBeamResult result = sbs->area->HitBeam(startbeam, endbeam, true);
	if (!result.mesh)
		return;

	//get mesh name
	meshname = result.mesh->QueryObject()->GetName();

	//get polygon name
	csRef<iThingFactoryState> state = scfQueryInterface<iThingFactoryState> (result.mesh->GetMeshObject()->GetFactory());
	polyname = state->GetPolygonName(result.polygon_idx);

	//show result
	sbs->Report("Clicked on object - Mesh: " + meshname + ", Polygon: " + polyname);

	//check call buttons
	if (meshname.Find("Call Button") != -1)
	{
		//user clicked on a call button
		int floor = atoi(meshname.Slice(12, meshname.Find(":") - 12));
		int number = atoi(meshname.Slice(meshname.Find(":") + 1));
		//press button
		sbs->GetFloor(floor)->CallButtonArray[number]->Press(result.polygon_idx);
	}

	//check elevator buttons
	if (meshname.Find("Button Panel") != -1)
	{
		//user clicked on an elevator button
		int elevator = atoi(meshname.Slice(13, meshname.Find(":") - 13));
		int number = atoi(meshname.Slice(meshname.Find(":") + 1));

		//press button
		if (number == 1)
			sbs->GetElevator(elevator)->Panel->Press(result.polygon_idx);
		else
			sbs->GetElevator(elevator)->Panel2->Press(result.polygon_idx);
	}

	//check doors
	if (meshname.Find("Door") != -1)
	{
		//user clicked on a door
		if (meshname.Slice(0, 5) == "Floor")
		{
			int marker = meshname.Find(":");
			int floornumber = atoi(meshname.Slice(5, marker - 5));
			int doornumber = atoi(meshname.Slice(marker + 5));
			if (sbs->GetFloor(floornumber)->DoorArray[doornumber]->IsOpen() == false)
				sbs->GetFloor(floornumber)->DoorArray[doornumber]->Open();
			else
				sbs->GetFloor(floornumber)->DoorArray[doornumber]->Close();
		}
		if (meshname.Slice(0, 9) == "Stairwell")
		{
			int marker = meshname.Find(":");
			int stairsnumber = atoi(meshname.Slice(9, marker - 9));
			int doornumber = atoi(meshname.Slice(marker + 5));
			if (sbs->GetStairs(stairsnumber)->IsDoorOpen(doornumber) == false)
				sbs->GetStairs(stairsnumber)->OpenDoor(doornumber);
			else
				sbs->GetStairs(stairsnumber)->CloseDoor(doornumber);
		}
	}
}

const char *Camera::GetClickedMeshName()
{
	//return name of last clicked mesh

	return meshname.GetData();
}

const char *Camera::GetClickedPolyName()
{
	//return name of last clicked polygon

	return polyname.GetData();
}

void Camera::CreateColliders()
{
	// Define the player bounding box.

	collider_actor.SetCollideSystem(sbs->collision_sys);
	collider_actor.SetEngine(sbs->engine);
	csVector3 legs (cfg_legs_width, cfg_legs_height, cfg_legs_depth);
	csVector3 body (cfg_body_width, cfg_body_height, cfg_body_depth);
	csVector3 shift (0, -(cfg_legs_height + cfg_body_height), 0);
	collider_actor.InitializeColliders (MainCamera, legs, body, shift);
	collider_actor.SetCamera(MainCamera, true);
}

void Camera::Loop()
{
	//set collision detection status
	collider_actor.SetCD(EnableCollisions);

	//calculate acceleration
	InterpolateMovement();

	//general movement
	float delta = sbs->vc->GetElapsedTicks() / 1000.0f;
	collider_actor.Move(delta, speed, velocity, angle_velocity);

	//sync sound listener object to camera position
	sbs->SetListenerLocation(GetPosition());
}

void Camera::Strafe(float speed)
{
	speed *= cfg_walk_maxspeed_multreal;
	desired_velocity.x = 140.0f * speed * cfg_walk_maxspeed	* cfg_walk_maxspeed_multreal;
}

void Camera::Step(float speed)
{
	speed *= cfg_walk_maxspeed_multreal;
	desired_velocity.z = 140.0f * speed * cfg_walk_maxspeed * cfg_walk_maxspeed_multreal;
}

void Camera::Float(float speed)
{
	speed *= cfg_walk_maxspeed_multreal;
	desired_velocity.y = 140.0f * speed * cfg_walk_maxspeed * cfg_walk_maxspeed_multreal;
}

void Camera::Jump()
{
	velocity.y = 110.0f * cfg_jumpspeed;
	desired_velocity.y = 0.0f;
}

void Camera::Look(float speed)
{
	desired_angle_velocity.x = 150.0f * speed * cfg_rotate_maxspeed;
}

void Camera::Turn(float speed)
{
	desired_angle_velocity.y = 100.0f * speed * cfg_rotate_maxspeed * cfg_walk_maxspeed_multreal;
}

void Camera::InterpolateMovement()
{
	//calculate acceleration
	float elapsed = sbs->vc->GetElapsedTicks() / 1000.0f;
	elapsed *= 1700.0f;

	for (size_t i = 0; i < 3; i++)
	{
		if (velocity[i] < desired_velocity[i])
		{
			velocity[i] += cfg_walk_accelerate * elapsed;
			if (velocity[i] > desired_velocity[i])
				velocity[i] = desired_velocity[i];
		}
		else
		{
			velocity[i] -= cfg_walk_accelerate * elapsed;
			if (velocity[i] < desired_velocity[i])
				velocity[i] = desired_velocity[i];
		}
	}

	for (size_t i = 0; i < 3; i++)
	{
		if (angle_velocity[i] < desired_angle_velocity[i])
		{
			angle_velocity[i] += cfg_rotate_accelerate * elapsed;
			if (angle_velocity[i] > desired_angle_velocity[i])
				angle_velocity[i] = desired_angle_velocity[i];
		}
		else
		{
			angle_velocity[i] -= cfg_rotate_accelerate * elapsed;
			if (angle_velocity[i] < desired_angle_velocity[i])
				angle_velocity[i] = desired_angle_velocity[i];
		}
	}
}

void Camera::SetGravity(float gravity)
{
	Gravity = gravity;
	collider_actor.SetGravity(Gravity);
}

float Camera::GetGravity()
{
	return Gravity;
}

void Camera::EnableGravity(bool value)
{
	if (value == true)
		collider_actor.SetGravity(Gravity);
	else
		collider_actor.SetGravity(0.0f);
	GravityStatus = value;
}

bool Camera::GetGravityStatus()
{
	return GravityStatus;
}
