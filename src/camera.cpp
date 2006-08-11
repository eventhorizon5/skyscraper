/*
	Scalable Building Simulator - Camera Object Class
	The Skyscraper Project - Version 1.1 Alpha
	Copyright ©2005-2006 Ryan Thoryk
	http://www.tliquest.net/skyscraper
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@tliquest.net
	
	$Id$

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
	delete MainCamera;
	MainCamera = 0;
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

void Camera::Move(csVector3 vector)
{
	//moves the camera in a relative amount specified by a vector
	MainCamera->Move(vector);
}

void Camera::Rotate(csVector3 vector)
{
	//rotates the camera in a relative amount
	
	rotX += vector.x;
	rotY += vector.y;
	rotZ += vector.z;
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

