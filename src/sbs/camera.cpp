/* $Id$ */

/*
	Scalable Building Simulator - Camera Object Class
	The Skyscraper Project - Version 1.9 Alpha
	Copyright (C)2004-2014 Ryan Thoryk
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

#include <OgreCamera.h>
#include <OgreBulletDynamicsCharacter.h>
#include <OgreBulletCollisionsRay.h>
#include <OgreBulletCollisionsCapsuleShape.h>
#include "globals.h"
#include "sbs.h"
#include "callbutton.h"
#include "buttonpanel.h"
#include "camera.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

#undef SMALL_EPSILON
#define SMALL_EPSILON 0.000001f

Camera::Camera(Ogre::Camera *camera)
{
	//set up SBS object
	object = new Object();
	object->SetValues(this, sbs->object, "Camera", "Camera", true);

	//init variables
	CurrentFloor = 0;
	StartFloor = 0;
	StartPositionX = 0;
	StartPositionZ = 0;
	StartDirection = Ogre::Vector3(0, 0, 0);
	StartRotation = Ogre::Vector3(0, 0, 0);
	FloorTemp = 0;
	velocity = Ogre::Vector3(0, 0, 0);
	desired_velocity = Ogre::Vector3(0, 0, 0);
	angle_velocity = Ogre::Vector3(0, 0, 0);
	desired_angle_velocity = Ogre::Vector3(0, 0, 0);
	cfg_jumpspeed = sbs->GetConfigFloat("Skyscraper.SBS.Camera.JumpSpeed", 9.0f);
	cfg_walk_accelerate = sbs->GetConfigFloat("Skyscraper.SBS.Camera.WalkAccelerate", 0.040f);
	cfg_walk_maxspeed = sbs->GetConfigFloat("Skyscraper.SBS.Camera.WalkMaxSpeed", 0.1f);
	cfg_walk_maxspeed_mult = sbs->GetConfigFloat("Skyscraper.SBS.Camera.WalkMaxSpeed_Mult", 10.0f);
	cfg_walk_maxspeed_multreal = sbs->GetConfigFloat("Skyscraper.SBS.Camera.WalkMaxSpeed_MultReal", 1.0f);
	cfg_walk_brake = sbs->GetConfigFloat("Skyscraper.SBS.Camera.WalkBrake", 0.040f);
	cfg_rotate_accelerate = sbs->GetConfigFloat("Skyscraper.SBS.Camera.RotateAccelerate", 0.005f);
	cfg_rotate_maxspeed = sbs->GetConfigFloat("Skyscraper.SBS.Camera.RotateMaxSpeed", 0.015f);
	cfg_rotate_brake = sbs->GetConfigFloat("Skyscraper.SBS.Camera.RotateBrake", 0.015f);
	cfg_body_height = sbs->GetConfigFloat("Skyscraper.SBS.Camera.BodyHeight", 3.0f);
	cfg_body_width = sbs->GetConfigFloat("Skyscraper.SBS.Camera.BodyWidth", 1.64f);
	cfg_legs_height = sbs->GetConfigFloat("Skyscraper.SBS.Camera.LegsHeight", 3.0f);
	cfg_legs_width = sbs->GetConfigFloat("Skyscraper.SBS.Camera.LegsWidth", 1.312f);
	cfg_lookspeed = sbs->GetConfigFloat("Skyscraper.SBS.Camera.LookSpeed", 150.0f);
	cfg_turnspeed = sbs->GetConfigFloat("Skyscraper.SBS.Camera.TurnSpeed", 100.0f);
	cfg_spinspeed = sbs->GetConfigFloat("Skyscraper.SBS.Camera.SpinSpeed", 150.0f);
	cfg_floatspeed = sbs->GetConfigFloat("Skyscraper.SBS.Camera.FloatSpeed", 140.0f);
	cfg_stepspeed = sbs->GetConfigFloat("Skyscraper.SBS.Camera.StepSpeed", 70.0f);
	cfg_strafespeed = sbs->GetConfigFloat("Skyscraper.SBS.Camera.StrafeSpeed", 70.0f);
	cfg_speed = sbs->GetConfigFloat("Skyscraper.SBS.Camera.Speed", 1.0f);
	cfg_speedfast = sbs->GetConfigFloat("Skyscraper.SBS.Camera.FastSpeed", 2.0f);
	cfg_speedslow = sbs->GetConfigFloat("Skyscraper.SBS.Camera.SlowSpeed", 0.5f);
	cfg_zoomspeed = sbs->GetConfigFloat("Skyscraper.SBS.Camera.ZoomSpeed", 0.2f);
	speed = 1;
	Collisions = 0;
	lastfloor = 0;
	lastfloorset = false;
	MouseDown = false;
	ReportCollisions = sbs->GetConfigBool("Skyscraper.SBS.Camera.ReportCollisions", false);
	Freelook = sbs->GetConfigBool("Skyscraper.SBS.Camera.Freelook", false);
	Freelook_speed = sbs->GetConfigFloat("Skyscraper.SBS.Camera.FreelookSpeed", 200.0f);
	FOV = sbs->GetConfigFloat("Skyscraper.SBS.Camera.FOV", 71.263794f);
	FarClip = sbs->GetConfigFloat("Skyscraper.SBS.Camera.MaxDistance", 0.0f);
	object_number = 0;
	object_line = 0;
	HitPosition = 0;
	RotationStopped = false;
	MovementStopped = false;
	accum_movement = 0;
	collision_reset = false;
	EnableBullet = sbs->GetConfigBool("Skyscraper.SBS.Camera.EnableBullet", true);
	use_startdirection = false;

	//set up camera and scene nodes
	MainCamera = camera;
	MainCamera->setNearClipDistance(0.1f);
	MainCamera->setPosition(Ogre::Vector3(0, sbs->ToRemote((cfg_body_height + cfg_legs_height + 0.5f) / 2), 0));
	CameraNode = sbs->mSceneManager->getRootSceneNode()->createChildSceneNode("Camera");
	CameraNode->attachObject(MainCamera);
	SetFOVAngle(FOV);
	SetMaxRenderDistance(FarClip);

	//set up collider character
	float width = cfg_legs_width / 2;
	if (cfg_body_width > cfg_legs_width)
		width = cfg_body_width / 2;

	float height = (cfg_body_height + cfg_legs_height - 0.5f) - (width * 2);
	float step_height = cfg_legs_height - 0.5f;

	mCharacter = new OgreBulletDynamics::CharacterController("CameraCollider", sbs->mWorld, CameraNode, sbs->ToRemote(width), sbs->ToRemote(height), sbs->ToRemote(step_height));
	EnableCollisions(sbs->GetConfigBool("Skyscraper.SBS.Camera.EnableCollisions", true));

	//create debug shape
	mShape = new OgreBulletCollisions::CapsuleCollisionShape(sbs->ToRemote(width), sbs->ToRemote(height), Ogre::Vector3::UNIT_Y);
	mCharacter->setShape(mShape);

	//other movement options
	mCharacter->setJumpSpeed(sbs->ToRemote(cfg_jumpspeed));
	mCharacter->setFallSpeed(sbs->ToRemote(sbs->GetConfigFloat("Skyscraper.SBS.Camera.FallSpeed", 177.65f)));
	SetGravity(sbs->GetConfigFloat("Skyscraper.SBS.Camera.Gravity", 32.1719f)); // 9.806 m/s/s
	GravityStatus = sbs->GetConfigBool("Skyscraper.SBS.Camera.GravityStatus", true);
}

Camera::~Camera()
{
	//Destructor
	if (mCharacter)
		delete mCharacter;
	std::string nodename = CameraNode->getChild(0)->getName();
	CameraNode->detachAllObjects();
	CameraNode->getParent()->removeChild(CameraNode);
	sbs->mSceneManager->destroySceneNode(nodename);
	sbs->mSceneManager->destroySceneNode(CameraNode->getName());
	CameraNode = 0;

	delete object;
}

void Camera::SetPosition(const Ogre::Vector3 &vector)
{
	//sets the camera to an absolute position in 3D space
	if (EnableBullet == true)
	{
		mCharacter->setPosition(sbs->ToRemote(vector) - MainCamera->getPosition());
		mCharacter->updateTransform(true, false, false);
	}
	else
		MainCamera->setPosition(sbs->ToRemote(vector));
}

void Camera::SetDirection(const Ogre::Vector3 &vector)
{
	//sets the camera's direction to an absolute position
	//MainCamera->lookAt(sbs->ToRemote(vector));
}

void Camera::SetRotation(Ogre::Vector3 vector)
{
	//sets the camera's rotation in degrees

	//keep rotation within 360 degree boundaries
	if (vector.x > 359)
		vector.x -= 360;
	if (vector.x < 0)
		vector.x += 360;
	if (vector.y > 359)
		vector.y -= 360;
	if (vector.y < 0)
		vector.y += 360;
	if (vector.z > 359)
		vector.z -= 360;
	if (vector.z < 0)
		vector.z += 360;

	Ogre::Quaternion x(Ogre::Degree(vector.x), Ogre::Vector3::UNIT_X);
	Ogre::Quaternion y(Ogre::Degree(vector.y), Ogre::Vector3::NEGATIVE_UNIT_Y);
	Ogre::Quaternion z(Ogre::Degree(vector.z), Ogre::Vector3::UNIT_Z);
	Ogre::Quaternion camrot = x * z;
	Ogre::Quaternion bodyrot = y;
	rotation = vector;
	MainCamera->setOrientation(camrot);
	if (EnableBullet == true)
	{
		mCharacter->setOrientation(bodyrot);
		mCharacter->updateTransform(false, true, true);
	}
}

Ogre::Vector3 Camera::GetPosition()
{
	//returns the camera's current position
	if (CameraNode)
		return sbs->ToLocal(CameraNode->getPosition() + MainCamera->getPosition());
	else
		return Ogre::Vector3(0, 0, 0);
}

void Camera::GetDirection(Ogre::Vector3 &front, Ogre::Vector3 &top)
{
	//returns the camera's current direction in front and top vectors

	Ogre::Quaternion dir = MainCamera->getDerivedOrientation();

	front = dir.zAxis();
	front.x = -front.x; //convert to left-hand coordinate system
	front.y = -front.y; //convert to left-hand coordinate system
	front.normalise();
	top = dir.yAxis();
	top.z = -top.z; //convert to left-hand coordinate system
	top.normalise();
}

Ogre::Vector3 Camera::GetRotation()
{
	//returns the camera's current rotation

	return rotation;
}

void Camera::UpdateCameraFloor()
{
	int newlastfloor;

	if (lastfloorset == true)
		newlastfloor = sbs->GetFloorNumber(GetPosition().y, lastfloor, true);
	else
		newlastfloor = sbs->GetFloorNumber(GetPosition().y);

	//if camera moved to a different floor, update floor indicators
	if ((lastfloor != newlastfloor) && sbs->GetFloor(newlastfloor))
		sbs->GetFloor(newlastfloor)->UpdateFloorIndicators();

	lastfloor = newlastfloor;
	lastfloorset = true;
	CurrentFloor = lastfloor;
	if (sbs->GetFloor(CurrentFloor))
		CurrentFloorID = sbs->GetFloor(CurrentFloor)->ID;
}

bool Camera::Move(Ogre::Vector3 vector, float speed, bool flip)
{
	//moves the camera in a relative amount specified by a vector

	if (MovementStopped == true && vector == Ogre::Vector3::ZERO)
		return false;

	MovementStopped = false;

	if (vector == Ogre::Vector3::ZERO)
		MovementStopped = true;

	if (flip == true)
		vector = vector * Ogre::Vector3(-1, 1, 1);

	//multiply vector with camera's orientation, and flip X axis
	if (EnableBullet == true)
	{
		vector = mCharacter->getRootNode()->getOrientation() * vector * Ogre::Vector3(-1, 1, 1);
		accum_movement += (sbs->ToRemote(vector) * speed);
	}
	else
	{
		vector = MainCamera->getOrientation() * sbs->ToRemote(vector);
		accum_movement += (vector * speed);
	}

	return true;
}

void Camera::Rotate(const Ogre::Vector3 &vector, float speed)
{
	//rotates the camera in a relative amount in world space

	Ogre::Vector3 rot = GetRotation() + (vector * speed);
	SetRotation(rot);
}

void Camera::RotateLocal(const Ogre::Vector3 &vector, float speed)
{
	//rotates the camera in a relative amount in local camera space

	if (RotationStopped == true && vector == Ogre::Vector3::ZERO)
		return;

	RotationStopped = false;

	if (vector == Ogre::Vector3::ZERO)
		RotationStopped = true;

	//convert rotation values to degrees
	float xdeg = Ogre::Math::RadiansToDegrees(vector.x) * speed; //X axis (up/down)
	float ydeg = Ogre::Math::RadiansToDegrees(vector.y) * speed; //Y axis (left/right)
	float zdeg = Ogre::Math::RadiansToDegrees(vector.z) * speed; //Z axis (clockwise/counterclockwise)
	rotation.x += xdeg;
	rotation.y += ydeg;
	rotation.z += zdeg;

	if (rotation.x > 359)
		rotation.x -= 360;
	if (rotation.y > 359)
		rotation.y -= 360;
	if (rotation.z > 359)
		rotation.z -= 360;
	if (rotation.x < 0)
		rotation.x += 360;
	if (rotation.y < 0)
		rotation.y += 360;
	if (rotation.z < 0)
		rotation.z += 360;

	Ogre::Quaternion rot(Ogre::Degree(rotation.y), Ogre::Vector3::NEGATIVE_UNIT_Y);
	if (EnableBullet == true)
	{
		//rotate character collider
		mCharacter->setOrientation(rot);
		mCharacter->updateTransform(false, true, true);

		//rotate camera
		MainCamera->pitch(Ogre::Degree(xdeg));
		MainCamera->roll(Ogre::Degree(zdeg));
	}
	else
		MainCamera->setOrientation(rot);
}

void Camera::SetStartDirection(const Ogre::Vector3 &vector)
{
	StartDirection = vector;
	use_startdirection = true;
}

Ogre::Vector3 Camera::GetStartDirection()
{
	return StartDirection;
}

void Camera::SetStartRotation(const Ogre::Vector3 &vector)
{
	StartRotation = vector;
}

Ogre::Vector3 Camera::GetStartRotation()
{
	return StartRotation;
}

void Camera::SetToStartPosition(bool disable_current_floor)
{
	if (sbs->GetFloor(StartFloor))
		SetPosition(Ogre::Vector3(StartPositionX, 0, StartPositionZ));
	GotoFloor(StartFloor, disable_current_floor);
}

void Camera::SetToStartDirection()
{
	if (use_startdirection == true)
		SetDirection(StartDirection);
}

void Camera::SetToStartRotation()
{
	if (use_startdirection == false)
		SetRotation(StartRotation);
}

void Camera::CheckElevator()
{
	//check to see if user (camera) is in an elevator

	//first checks to see if camera is within an elevator's height range, and then
	//checks for a collision with the elevator's floor below

	SBS_PROFILE("Camera::CheckElevator");

	for (int i = 1; i <= sbs->Elevators(); i++)
	{
		Elevator *elevator = sbs->GetElevator(i);

		if (!elevator)
			continue;

		bool result = elevator->Check(GetPosition());
		if (result == true)
			return;
	}

	//user is not in an elevator if all elevators returned false
	sbs->InElevator = false;
	sbs->ElevatorSync = false;
}

void Camera::CheckShaft()
{
	//check to see if user (camera) is in a shaft

	if (sbs->AutoShafts == false)
		return;

	SBS_PROFILE("Camera::CheckShaft");
	for (int i = 1; i <= sbs->Shafts(); i++)
	{
		Shaft *shaft = sbs->GetShaft(i);

		if (!shaft)
			continue; //skip if shaft doesn't exist

		shaft->Check(GetPosition(), CurrentFloor);
	}
}

void Camera::CheckStairwell()
{
	//check to see if user (camera) is in a stairwell

	if (sbs->AutoStairs == false)
		return;

	SBS_PROFILE("Camera::CheckStairwell");
	for (int i = 1; i <= sbs->StairsNum(); i++)
	{
		Stairs *stairs = sbs->GetStairs(i);

		if (!stairs)
			continue;

		stairs->Check(GetPosition(), CurrentFloor, FloorTemp);
	}
	FloorTemp = CurrentFloor;
}

void Camera::ClickedObject(bool shift, bool ctrl, bool alt)
{
	//get mesh object that the user clicked on, and perform related work

	//cast a ray from the camera in the direction of the clicked position
	SBS_PROFILE("Camera::ClickedObject");
	int width = MainCamera->getViewport()->getActualWidth();
	int height = MainCamera->getViewport()->getActualHeight();
	float x = (float)sbs->mouse_x / (float)width;
	float y = (float)sbs->mouse_y / (float)height;
	Ogre::Ray ray = MainCamera->getCameraToViewportRay(x, y);

	//generate left-hand coordinate ray
	
	//get a collision callback from Bullet
	//OgreBulletCollisions::CollisionAllRayResultCallback callback (ray, sbs->mWorld, 1000);
	OgreBulletCollisions::CollisionClosestRayResultCallback callback (ray, sbs->mWorld, 1000);

	//check for collision
	sbs->mWorld->launchRay(callback);

	//exit if no collision
	if (callback.doesCollide() == false)
		return;

	polyname = "";
	WallObject* wall = 0;
	MeshObject* meshobject;
	float best_distance = 2000000000.;
	MeshObject* bestmesh = 0;
	int best_i = -1;
	std::string prevmesh = "";
	Ogre::Vector3 pos = GetPosition();

	//get collided collision object
	//for (int i = 0; i < callback.getCollisionCount(); i++)
	//{
		//OgreBulletCollisions::Object* object = callback.getCollidedObject(i);
		OgreBulletCollisions::Object* object = callback.getCollidedObject();

		if (!object)
			//continue;
			return;

		//get name of collision object's parent scenenode (which is the same name as the mesh object)
		meshname = object->getRootNode()->getName();

		if (meshname == prevmesh)
			//continue;
			return;

		prevmesh = meshname;

		//get hit/intersection position
		//HitPosition = sbs->ToLocal(callback.getCollisionPoint(i));
		HitPosition = sbs->ToLocal(callback.getCollisionPoint());

		//get wall name
		meshobject = sbs->FindMeshObject(meshname);
		if (!meshobject)
			//continue;
			return;

		Ogre::Vector3 isect;
		float distance = best_distance;
		Ogre::Vector3 normal = Ogre::Vector3::ZERO;
		int num = meshobject->FindWallIntersect(ray.getOrigin(), ray.getPoint(1000), isect, distance, normal, false, false);

		/*if (distance < best_distance)
		{
			best_distance = distance;
			bestmesh = meshobject;
			best_i = num;
		}
	}*/

	//if (best_i > -1 && bestmesh)
	//{
		//wall = bestmesh->Walls[best_i];
		if (num > -1)
			wall = meshobject->Walls[num];
		if (wall)
		{
			polyname = wall->GetName();
			//meshname = bestmesh->name;
			meshname = meshobject->name;
			//meshobject = bestmesh;
		}
		else
			polyname = "";
	//}

	//get and strip object number
	std::string number;
	object_number = 0;
	if (wall)
	{
		object_number = wall->GetNumber();
		int index = (int)polyname.find(")");
		if (index > -1)
			polyname.erase(polyname.begin(), polyname.begin() + index + 1);
	}
	if ((int)meshname.find("(") == 0)
	{
		int index = (int)meshname.find(")");
		if (object_number == 0)
			object_number = atoi(meshname.substr(1, index - 1).c_str());
		meshname.erase(meshname.begin(), meshname.begin() + index + 1);
	}
	number = ToString(object_number);

	//store parameters of object
	Object *obj = sbs->GetObject(object_number);
	if (obj)
	{
		object_line = obj->linenum;
		object_cmd = obj->command;
		object_cmd_processed = obj->command_processed;
	}

	//show result
	if (wall)
		sbs->Report("Clicked on object " + number + ": Mesh: " + meshname + ", Polygon: " + polyname);
	else
		sbs->Report("Clicked on object " + number + ": " + meshname);

	//object checks and actions
	if (obj)
	{
		//delete wall if ctrl is pressed
		if (wall && ctrl == true && object_number > 0)
		{
			if (std::string(obj->GetType()) == "Wall")
				sbs->DeleteObject(obj);
			else
				sbs->Report("Cannot delete object " + number);
			return;
		}

		//get original object (parent object of clicked mesh)
		if (obj->GetParent())
		{
			std::string type = obj->GetParent()->GetType();

			//check controls
			if (type == "Control")
			{
				Control *control = (Control*)obj->GetParent()->GetRawObject();

				if (control)
				{
					if (shift == false)
						control->Press();
					else
						control->ToggleLock();
				}
			}

			//check doors
			if (type == "Door")
			{
				Door *door = (Door*)obj->GetParent()->GetRawObject();

				if (door)
				{
					//delete door if ctrl key is pressed
					if (ctrl == true)
					{
						sbs->DeleteObject(obj->GetParent());
						return;
					}

					if (shift == false)
					{
						if (door->IsOpen() == false)
						{
							if (door->IsMoving == false)
								door->Open(pos);
							else
								door->Close();
						}
						else
						{
							if (door->IsMoving == false)
								door->Close();
							else
								door->Open(pos);
						}
					}
					else
						door->ToggleLock(pos);
				}
			}

			//check models
			if (type == "Model")
			{
				Model *model = (Model*)obj->GetParent()->GetRawObject();

				if (model)
				{
					//delete model if ctrl key is pressed
					if (ctrl == true)
					{
						sbs->DeleteObject(obj->GetParent());
						return;
					}

					//if model is a key, add key to keyring and delete model
					if (model->IsKey() == true)
					{
						sbs->AddKey(model->GetKeyID(), model->Name);
						sbs->DeleteObject(obj->GetParent());
						return;
					}
				}
			}
		}
	}

	//check call buttons
	if ((int)meshname.find("Call Button") != -1)
	{
		//user clicked on a call button
		int index = (int)meshname.find(":");
		int index2 = (int)meshname.find(":", index + 1);
		int floor = atoi(meshname.substr(12, index - 12).c_str());
		int number = atoi(meshname.substr(index + 1, index2 - index - 1).c_str());

		CallButton *buttonref = 0;
		if (sbs->GetFloor(floor))
			buttonref = sbs->GetFloor(floor)->CallButtonArray[number];

		if (buttonref)
		{
			std::string direction = meshname.substr(index2 + 1);
			TrimString(direction);

			if (shift == false)
			{
				//press button
				if (direction == "Up")
					buttonref->Call(true);
				else
					buttonref->Call(false);
			}
			else
			{
				//if shift is held, change button status instead
				/*if (direction == "Up")
					buttonref->UpLight(!buttonref->UpStatus);
				else
					buttonref->DownLight(!buttonref->DownStatus);*/

				//if shift is held, toggle lock
				buttonref->ToggleLock();
			}
		}
	}

	//check shaft doors
	if ((int)meshname.find("Shaft Door") != -1 && shift == true)
	{
		//user clicked on a shaft door
		int elevator = atoi(meshname.substr(9, meshname.find(":") - 9).c_str());
		int index = (int)meshname.find("Shaft Door");
		int index2 = (int)meshname.find(":", index);
		int number = atoi(meshname.substr(index + 10, index2 - (index + 10)).c_str());
		int floor = atoi(meshname.substr(index2 + 1, meshname.length() - index2 - 2).c_str());

		Elevator *elev = sbs->GetElevator(elevator);
		if (elev)
		{
			if (elev->AreShaftDoorsOpen(number, floor) == false)
				elev->OpenDoorsEmergency(number, 3, floor);
			else
				elev->CloseDoorsEmergency(number, 3, floor);
		}
	}

	//check elevator doors
	if ((int)meshname.find("ElevatorDoor") != -1 && shift == true)
	{
		//user clicked on an elevator door
		int elevator = atoi(meshname.substr(13, meshname.find(":") - 13).c_str());
		int number = atoi(meshname.substr(meshname.find(":") + 1, meshname.length() - meshname.find(":") - 1).c_str());

		Elevator *elev = sbs->GetElevator(elevator);
		if (elev)
		{
			if (elev->AreDoorsOpen(number) == false)
				elev->OpenDoorsEmergency(number, 2);
			else
				elev->CloseDoorsEmergency(number, 2);
		}
	}
}

const char* Camera::GetClickedMeshName()
{
	//return name of last clicked mesh

	return meshname.c_str();
}

const char* Camera::GetClickedPolyName()
{
	//return name of last clicked polygon

	return polyname.c_str();
}

int Camera::GetClickedObjectNumber()
{
	//return number of last clicked object
	return object_number;
}

int Camera::GetClickedObjectLine()
{
	//return line number of last clicked object
	return object_line;
}

const char *Camera::GetClickedObjectCommand()
{
	//return command line of last clicked object
	return object_cmd.c_str();
}

const char *Camera::GetClickedObjectCommandP()
{
	//return processed command line of last clicked object
	return object_cmd_processed.c_str();
}

void Camera::Loop(float delta)
{
	SBS_PROFILE_MAIN("Camera Loop");

	if (collision_reset == true && EnableBullet == true)
		mCharacter->resetCollisions();
	collision_reset = false;

	//get name of the last hit mesh object
	if (sbs->GetRunTime() > 10) //due to initialization-related crashes, wait before checking
	{
		if (EnableBullet == true)
		{
			if (mCharacter->getLastCollision())
				if (mCharacter->getLastCollision()->getRootNode())
					LastHitMesh = mCharacter->getLastCollision()->getRootNode()->getName();
		}
	}

	//calculate acceleration
	InterpolateMovement(delta);

	//general movement

	RotateLocal(angle_velocity, delta * speed);
	Move(velocity, delta * speed, false);

	if (CollisionsEnabled() == true)
	{
		//report name of mesh
		if (ReportCollisions == true)
			sbs->Report(LastHitMesh);

		std::string name = LastHitMesh;

		if (name != "")
		{
			int index = (int)name.find(")");
			int number = atoi(name.substr(1, index - 1).c_str());
			name.erase(name.begin(), name.begin() + index + 1);
			std::string num = ToString(number);

			//get SBS object
			Object *obj = sbs->GetObject(number);

			//get original object (parent object of clicked mesh)
			if (obj)
			{
				if (obj->GetParent())
				{
					std::string type = obj->GetParent()->GetType();

					//check elevator doors (door bumpers feature)
					if (type == "DoorWrapper")
					{
						ElevatorDoor::DoorWrapper *wrapper = (ElevatorDoor::DoorWrapper*)obj->GetParent()->GetRawObject();
						ElevatorDoor* door = wrapper->parent;

						if (door)
						{
							int whichdoors = door->elev->GetDoor(door->Number)->GetWhichDoors();

							if (door->elev->GetDoor(door->Number)->OpenDoor == -1 && whichdoors == 1)
								door->elev->OpenDoors(door->Number, 1);
						}
					}
				}
			}
		}
	}

	//sync sound listener object to camera position
	sbs->SetListenerPosition(GetPosition());

	//set direction of listener to camera's direction
	Ogre::Vector3 front, top;
	GetDirection(front, top);
	sbs->SetListenerDirection(front, top);

	//sync camera with collider
	Sync();
}

void Camera::Strafe(float speed)
{
	speed *= cfg_walk_maxspeed_multreal;
	desired_velocity.x = -cfg_strafespeed * speed * cfg_walk_maxspeed * cfg_walk_maxspeed_multreal;
}

void Camera::Step(float speed)
{
	speed *= cfg_walk_maxspeed_multreal;
	desired_velocity.z = cfg_stepspeed * speed * cfg_walk_maxspeed * cfg_walk_maxspeed_multreal;
}

void Camera::Float(float speed)
{
	speed *= cfg_walk_maxspeed_multreal;
	desired_velocity.y = cfg_floatspeed * speed * cfg_walk_maxspeed * cfg_walk_maxspeed_multreal;
}

void Camera::Jump()
{
	//velocity.y = cfg_jumpspeed;
	//desired_velocity.y = 0.0f;
	if (mCharacter->getGravity() != 0 && EnableBullet == true)
		mCharacter->jump();
}

void Camera::Look(float speed)
{
	//look up/down by rotating camera on X axis
	desired_angle_velocity.x = cfg_lookspeed * speed * cfg_rotate_maxspeed;
}

void Camera::Turn(float speed)
{
	//turn camera by rotating on Y axis
	desired_angle_velocity.y = cfg_turnspeed * speed * cfg_rotate_maxspeed * cfg_walk_maxspeed_multreal;
}

void Camera::Spin(float speed)
{
	//spin camera by rotating on Z axis
	desired_angle_velocity.z = cfg_spinspeed * speed * cfg_rotate_maxspeed;
}

void Camera::InterpolateMovement(float delta)
{
	//calculate acceleration

	delta *= 1700.0f;

	for (size_t i = 0; i < 3; i++)
	{
		if (velocity[i] < desired_velocity[i])
		{
			velocity[i] += cfg_walk_accelerate * delta;
			if (velocity[i] > desired_velocity[i])
				velocity[i] = desired_velocity[i];
		}
		else
		{
			velocity[i] -= cfg_walk_brake * delta;
			if (velocity[i] < desired_velocity[i])
				velocity[i] = desired_velocity[i];
		}
	}

	for (size_t i = 0; i < 3; i++)
	{
		if (angle_velocity[i] < desired_angle_velocity[i])
		{
			angle_velocity[i] += cfg_rotate_accelerate * delta;
			if (angle_velocity[i] > desired_angle_velocity[i])
				angle_velocity[i] = desired_angle_velocity[i];
		}
		else
		{
			angle_velocity[i] -= cfg_rotate_brake * delta;
			if (angle_velocity[i] < desired_angle_velocity[i])
				angle_velocity[i] = desired_angle_velocity[i];
		}
	}
}

void Camera::SetGravity(float gravity, bool save_value)
{
	if (save_value == true)
		Gravity = gravity;
	if (EnableBullet == true)
	{
		sbs->mWorld->setGravity(Ogre::Vector3(0, sbs->ToRemote(-gravity), 0));
		mCharacter->setGravity(sbs->ToRemote(gravity));
	}
}

float Camera::GetGravity()
{
	return Gravity;
}

void Camera::EnableGravity(bool value)
{
	if (value == true)
		SetGravity(Gravity);
	else
	{
		SetGravity(0, false);
		velocity.y = 0;
		desired_velocity.y = 0;
	}
	if (EnableBullet == true)
		mCharacter->reset();
	GravityStatus = value;
}

bool Camera::GetGravityStatus()
{
	return GravityStatus;
}

void Camera::SetFOVAngle(float angle)
{
	//set camera FOV angle
	if (angle > 0 && angle < 179.63)
	{
		float ratio = MainCamera->getAspectRatio();
		MainCamera->setFOVy(Ogre::Degree(angle / ratio));
	}
}

float Camera::GetFOVAngle()
{
	return MainCamera->getFOVy().valueDegrees() * MainCamera->getAspectRatio();
}

void Camera::SetToDefaultFOV()
{
	//set to default FOV angle value
	SetFOVAngle(FOV);
}

float Camera::GetHeight()
{
	//return camera's height

	return cfg_body_height + cfg_legs_height;
}

void Camera::SetViewMode(int mode)
{
	//set view mode of camera:
	//0 - solid polygons (normal)
	//1 - wireframe mode
	//2 - point mode

	if (mode == 0)
		MainCamera->setPolygonMode(Ogre::PM_SOLID);
	if (mode == 1)
		MainCamera->setPolygonMode(Ogre::PM_WIREFRAME);
	if (mode == 2)
		MainCamera->setPolygonMode(Ogre::PM_POINTS);
}

void Camera::EnableCollisions(bool value)
{
	if (value == Collisions)
		return;

	Collisions = value;
	if (EnableBullet == true)
		mCharacter->enableCollisions(value);
}

bool Camera::CollisionsEnabled()
{
	return Collisions;
}

bool Camera::IsOnGround()
{
	if (EnableBullet == true)
		return mCharacter->onGround();
	else
		return false;
}

void Camera::Sync()
{
	//sync scene node with bullet object
	if (EnableBullet == true)
		mCharacter->sync();
}

void Camera::SetMaxRenderDistance(float value)
{
	//set distance of camera's far clipping plane - set to 0 for infinite
	MainCamera->setFarClipDistance(sbs->ToRemote(value));
	FarClip = value;
}

float Camera::GetMaxRenderDistance()
{
	return FarClip;
}

void Camera::ShowDebugShape(bool value)
{
	if (EnableBullet == true)
		mCharacter->showDebugShape(value);
}

void Camera::MoveCharacter()
{
	if (EnableBullet == true)
		mCharacter->setWalkDirection(accum_movement, 1);
	else
		MainCamera->move(accum_movement);
	accum_movement = 0;
}

void Camera::ResetCollisions()
{
	//queue a collision reset for next loop cycle
	collision_reset = true;
}

void Camera::GotoFloor(int floor, bool disable_current)
{
	//have camera warp to specified floor

	if (sbs->IsValidFloor(floor) == true)
	{
		Floor* origfloor = sbs->GetFloor(CurrentFloor);
		if (disable_current == true && origfloor)
		{
			origfloor->Enabled(false);
			origfloor->EnableGroup(false);
		}

		Floor* floorobj = sbs->GetFloor(floor);
		if (floorobj)
		{
			Ogre::Vector3 pos = GetPosition();
			pos.y = floorobj->GetBase() + GetHeight();
			SetPosition(pos);
			floorobj->Enabled(true);
			floorobj->EnableGroup(true);
		}
	}
}
