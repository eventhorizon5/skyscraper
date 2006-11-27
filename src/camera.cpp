/* $Id$ */

/*
	Scalable Building Simulator - Camera Object Class
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
#include "camera.h"
#include "sbs.h"

extern SBS *sbs; //external pointer to the SBS engine

Camera::Camera()
{
	MainCamera = sbs->view->GetCamera();
	MainCamera->SetSector(sbs->area);
	
	// these are used store the current orientation of the camera
	rotY = rotX = rotZ = 0;

	//init variables
	DefaultAltitude = 0;
	CurrentFloor = 0;
	OriginalPosition = 0;
	StartFloor = 0;
	StartPositionX = 0;
	StartPositionZ = 0;
	StartDirection = csVector3(0, 0, 0);
	StartRotation = csVector3(0, 0, 0);

}

Camera::~Camera()
{
	//Destructor
}

void Camera::SetPosition(csVector3 vector)
{
	//sets the camera to an absolute position in 3D space
	MainCamera->GetTransform().SetOrigin(csVector3(vector.x,
												   vector.y,
												   vector.z));
}

void Camera::SetDirection(csVector3 vector)
{
	//sets the camera's direction to an absolute position
	MainCamera->GetTransform().LookAt(vector, csVector3 (0, 1, 0));
}

void Camera::SetRotation(csVector3 vector)
{
	//sets the camera's rotation to an absolute position

	// We now assign a new rotation transformation to the camera.  You
	// can think of the rotation this way: starting from the zero
	// position, you first rotate "rotY" radians on your Y axis to get
	// the first rotation.  From there you rotate "rotX" radians on the
	// your X axis to get the final rotation.  We multiply the
	// individual rotations on each axis together to get a single
	// rotation matrix.  The rotations are applied in right to left
	// order .
	csMatrix3 rot = csXRotMatrix3 (vector.x) * csYRotMatrix3 (vector.y) * csZRotMatrix3 (vector.z);
	csOrthoTransform ot (rot, MainCamera->GetTransform().GetOrigin ());
	MainCamera->SetTransform (ot);
	rotX = vector.x;
	rotY = vector.y;
	rotZ = vector.z;
}

csVector3 Camera::GetPosition()
{
	//returns the camera's current position
	return csVector3(MainCamera->GetTransform().GetOrigin().x,
					 MainCamera->GetTransform().GetOrigin().y,
					 MainCamera->GetTransform().GetOrigin().z);
}

csVector3 Camera::GetDirection()
{
	//returns the camera's current direction
	return csVector3(0, 0, 0);
}

csVector3 Camera::GetRotation()
{
	//returns the camera's current rotation
	return csVector3(0, 0, 0);
}

void Camera::UpdateCameraFloor()
{
	CurrentFloor = sbs->GetFloorNumber(MainCamera->GetTransform().GetOrigin().y);
}

bool Camera::Move(csVector3 vector, double speed)
{
	//collision detection
	if (sbs->EnableCollisions == true)
	{
		csTraceBeamResult result;
		if (vector != CS_VEC_DOWN)
			result = csColliderHelper::TraceBeam(sbs->collision_sys, sbs->area, GetPosition(), GetPosition() + (vector * speed), false);
		else
			result = csColliderHelper::TraceBeam(sbs->collision_sys, sbs->area, GetPosition(), GetPosition() + (vector * speed) - DefaultAltitude, false);

		if (result.closest_mesh)
			return false;
	}

	//moves the camera in a relative amount specified by a vector
	MainCamera->Move(vector * speed, sbs->EnableCollisions);
	return true;
}

void Camera::Rotate(csVector3 vector, double speed)
{
	//rotates the camera in a relative amount
	
	rotX += vector.x * speed;
	rotY += vector.y * speed;
	rotZ += vector.z * speed;
	SetRotation(csVector3(rotX, rotY, rotZ));
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
	rotX = vector.x;
	rotY = vector.y;
	rotZ = vector.z;
}

csVector3 Camera::GetStartRotation()
{
	return StartRotation;
}

void Camera::SetToStartPosition()
{
	SetPosition(csVector3(StartPositionX, sbs->FloorArray[StartFloor]->Altitude + DefaultAltitude, StartPositionZ));
}

void Camera::SetToStartDirection()
{
	SetDirection(StartDirection);
}

void Camera::SetToStartRotation()
{
	SetRotation(StartRotation);
}

void Camera::Gravity()
{
	csTraceBeamResult result;
	double new_time;
	static double old_time, original_position, distance;

	result = csColliderHelper::TraceBeam(sbs->collision_sys, sbs->area, GetPosition(), csVector3(GetPosition().x, GetPosition().y - DefaultAltitude, GetPosition().z), false);
	if (result.closest_mesh)
	{
		distance = 0;
		sbs->IsFalling = false;
		original_position = 0;
		old_time = 0;
	}
	else
	{
		//fall routine
		
		if (sbs->IsFalling == false)
		{
			old_time = sbs->vc->GetCurrentTicks();
			original_position = GetPosition().y;
		}
		sbs->IsFalling = true;
		new_time = sbs->vc->GetCurrentTicks();
		double time_rate = (new_time - old_time) / 1000;

		//get distance value
		//d = 0.5 * g * t^2
		distance = 0.5 * sbs->Gravity * pow(time_rate, 2);

		result = csColliderHelper::TraceBeam(sbs->collision_sys, sbs->area, csVector3(GetPosition().x, original_position, GetPosition().z), csVector3(GetPosition().x, original_position - distance, GetPosition().z), false);
		if (result.closest_mesh)
			SetPosition(csVector3(GetPosition().x, (GetPosition().y - sqrt(result.sqdistance)) + DefaultAltitude, GetPosition().z));
		else
			SetPosition(csVector3(GetPosition().x, original_position - distance, GetPosition().z));

	}
}

void Camera::CheckElevator()
{
	//check to see if user (camera) is in an elevator

	//first checks to see if camera is within an elevator's height range, and then
	//checks for a collision with the elevator's floor below

	for (int i = 1; i <= sbs->Elevators; i++)
	{
		if (sbs->ElevatorArray[i]->IsInElevator(GetPosition()) == true)
		{
			sbs->InElevator = true;
			sbs->ElevatorNumber = i;
			sbs->ElevatorSync = true;
			return;
		}
	}
}

void Camera::CheckShaft()
{
	//check to see if user (camera) is in the shaft

	if (sbs->AutoShafts == false)
		return;

	for (int i = 1; i < sbs->ShaftArray.GetSize(); i++)
	{
		if (sbs->ShaftArray[i]->IsInShaft(GetPosition()) == true)
		{
			if (sbs->ShaftArray[i]->InsideShaft == false && sbs->InElevator == false)
			{
				sbs->ShaftArray[i]->InsideShaft = true;

				//turn on entire shaft
				sbs->ShaftArray[i]->EnableWholeShaft(true);
			}
			else if (sbs->ShaftArray[i]->InsideShaft == true && sbs->InElevator == true)
			{
				sbs->ShaftArray[i]->InsideShaft = false;
					
				//turn off shaft except for camera floor
				sbs->ShaftArray[i]->EnableWholeShaft(false);
				sbs->ShaftArray[i]->Enabled(sbs->camera->CurrentFloor, true);
			}
		}
		else if (sbs->ShaftArray[i]->InsideShaft == true)
		{
			sbs->ShaftArray[i]->InsideShaft = false;
			
			//turn off entire shaft
			sbs->ShaftArray[i]->EnableWholeShaft(false);
		}
	}
}

void Camera::ClickedObject()
{
	//code and comments from the CrystalSpace manual
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
	csVector3 v3d;
	MainCamera->InvPerspective(v2d, 100, v3d);

	// We are going to cast a beam in the current sector of the
	// camera from our camera position in the direction of the
	// 'v3d' point.  First we transform the v3d camera space
	// location to world space.
	csVector3 startbeam = MainCamera->GetTransform().GetOrigin();
	csVector3 endbeam = MainCamera->GetTransform().This2Other(v3d);
	csVector3 intersect;

	// Now do the actual intersection.
	int poly = -1;
	iMeshWrapper* mesh = sbs->area->HitBeamPortals(startbeam, endbeam, intersect, &poly);
	sbs->Report("Left click on mesh: " + csString(mesh->QueryObject()->GetName()));
}
