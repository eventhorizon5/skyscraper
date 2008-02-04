/* $Id$ */

/*
	Scalable Building Simulator - Camera Object Class
	The Skyscraper Project - Version 1.1 Alpha
	Copyright (C)2005-2008 Ryan Thoryk
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

#ifndef _SBS_CAMERA_H
#define _SBS_CAMERA_H

class Camera
{
public:
	float DefaultAltitude; //default vertical offset of camera from each floor
	int CurrentFloor; //floor camera's on
	float OriginalPosition;
	int StartFloor; //Starting floor
	float StartPositionX; //starting position on X axis
	float StartPositionZ; //starting position on Z axis
	float FallRate; //falling rate in m/s

	//functions
	Camera();
	~Camera();
	void SetPosition(csVector3 vector);
	void SetDirection(csVector3 vector);
	void SetRotation(csVector3 vector);
	csVector3 GetPosition();
	csVector3 GetDirection();
	csVector3 GetRotation();
	void UpdateCameraFloor();
	bool Move(csVector3 vector, float speed);
	void Rotate(csVector3 vector, float speed);
	void SetStartDirection(csVector3 vector);
	csVector3 GetStartDirection();
	void SetStartRotation(csVector3 vector);
	csVector3 GetStartRotation();
	void SetToStartPosition();
	void SetToStartDirection();
	void SetToStartRotation();
	void Gravity();
	void CheckElevator();
	void CheckShaft();
	void ClickedObject();
	const char *GetClickedMeshName();
	const char *GetClickedPolyName();

private:
	csRef<iCamera> MainCamera; //main first-person view camera
	csVector3 StartDirection; //direction camera faces on start
	csVector3 StartRotation; //camera's startup rotation
	float rotX, rotY, rotZ;
	csString meshname; //last clicked mesh name
	csString polyname; //last clicked polygon name
	char intbuffer[65];
	char buffer[20];
};

#endif
