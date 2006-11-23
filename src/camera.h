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

#ifndef _SBS_CAMERA_H
#define _SBS_CAMERA_H

#include "globals.h"

class Camera
{
public:
	double DefaultAltitude; //default vertical offset of camera from each floor
	int CurrentFloor; //floor camera's on
	double OriginalPosition;
	int StartFloor; //Starting floor
	double StartPositionX; //starting position on X axis
	double StartPositionZ; //starting position on Z axis
	double rotZ;
	double rotY;
    double rotX;

	//functions
	Camera();
	~Camera();
	void ColliderInit();
	void SetPosition(csVector3 vector);
	void SetDirection(csVector3 vector);
	void SetRotation(csVector3 vector);
	csVector3 GetPosition();
	csVector3 GetDirection();
	csVector3 GetRotation();
	void UpdateCameraFloor();
	void Move(csVector3 vector, double speed);
	void Rotate(csVector3 vector, double speed);
	void SetStartDirection(csVector3 vector);
	csVector3 GetStartDirection();
	void SetStartRotation(csVector3 vector);
	csVector3 GetStartRotation();
	void SetToStartPosition();
	void SetToStartDirection();
	void SetToStartRotation();
	void EnableCollisions(bool value);
	bool GetCollisionStatus();

private:
	csRef<iCamera> MainCamera; //main first-person view camera
	csVector3 StartDirection; //direction camera faces on start
	csVector3 StartRotation; //camera's startup rotation
	csColliderActor *collider_actor; //used for collision detection and gravity
};

#endif
