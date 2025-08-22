/*
	Scalable Building Simulator - Camera Object
	The Skyscraper Project - Version 2.1
	Copyright (C)2004-2025 Ryan Thoryk
	https://www.skyscrapersim.net
	https://sourceforge.net/projects/skyscraper/
	Contact - ryan@skyscrapersim.net

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
#include "utility.h"
#include "manager.h"
#include "floor.h"
#include "elevator.h"
#include "elevatorcar.h"
#include "shaft.h"
#include "stairs.h"
#include "model.h"
#include "dynamicmesh.h"
#include "mesh.h"
#include "wall.h"
#include "profiler.h"
#include "scenenode.h"
#include "vehicle.h"
#include "camera.h"

namespace SBS {

Camera::Camera(Object *parent) : Object(parent)
{
	//set up SBS object
	SetValues("Camera", "Camera", true);

	//init variables
	CurrentFloor = 0;
	StartFloor = 0;
	StartPositionX = 0;
	StartPositionZ = 0;
	StartDirection = Vector3(0, 0, 0);
	StartRotation = Vector3(0, 0, 0);
	FloorTemp = 0;
	velocity = Vector3(0, 0, 0);
	desired_velocity = Vector3(0, 0, 0);
	angle_velocity = Vector3(0, 0, 0);
	desired_angle_velocity = Vector3(0, 0, 0);
	cfg_jumpspeed = sbs->GetConfigFloat("Skyscraper.SBS.Camera.JumpSpeed", 9.0);
	cfg_walk_accelerate = sbs->GetConfigFloat("Skyscraper.SBS.Camera.WalkAccelerate", 0.040);
	cfg_walk_maxspeed = sbs->GetConfigFloat("Skyscraper.SBS.Camera.WalkMaxSpeed", 0.1);
	cfg_walk_maxspeed_mult = sbs->GetConfigFloat("Skyscraper.SBS.Camera.WalkMaxSpeed_Mult", 10.0);
	cfg_walk_maxspeed_multreal = sbs->GetConfigFloat("Skyscraper.SBS.Camera.WalkMaxSpeed_MultReal", 1.0);
	cfg_walk_brake = sbs->GetConfigFloat("Skyscraper.SBS.Camera.WalkBrake", 0.040);
	cfg_rotate_accelerate = sbs->GetConfigFloat("Skyscraper.SBS.Camera.RotateAccelerate", 0.005);
	cfg_rotate_maxspeed = sbs->GetConfigFloat("Skyscraper.SBS.Camera.RotateMaxSpeed", 0.015);
	cfg_rotate_brake = sbs->GetConfigFloat("Skyscraper.SBS.Camera.RotateBrake", 0.015);
	cfg_body_height = sbs->GetConfigFloat("Skyscraper.SBS.Camera.BodyHeight", 3.0);
	cfg_body_width = sbs->GetConfigFloat("Skyscraper.SBS.Camera.BodyWidth", 1.64);
	cfg_legs_height = sbs->GetConfigFloat("Skyscraper.SBS.Camera.LegsHeight", 3.0);
	cfg_legs_width = sbs->GetConfigFloat("Skyscraper.SBS.Camera.LegsWidth", 1.312);
	cfg_lookspeed = sbs->GetConfigFloat("Skyscraper.SBS.Camera.LookSpeed", 150.0);
	cfg_turnspeed = sbs->GetConfigFloat("Skyscraper.SBS.Camera.TurnSpeed", 100.0);
	cfg_spinspeed = sbs->GetConfigFloat("Skyscraper.SBS.Camera.SpinSpeed", 150.0);
	cfg_floatspeed = sbs->GetConfigFloat("Skyscraper.SBS.Camera.FloatSpeed", 140.0);
	cfg_stepspeed = sbs->GetConfigFloat("Skyscraper.SBS.Camera.StepSpeed", 70.0);
	cfg_strafespeed = sbs->GetConfigFloat("Skyscraper.SBS.Camera.StrafeSpeed", 70.0);
	cfg_speed = sbs->GetConfigFloat("Skyscraper.SBS.Camera.Speed", 1.0);
	cfg_speedfast = sbs->GetConfigFloat("Skyscraper.SBS.Camera.FastSpeed", 2.0);
	cfg_speedslow = sbs->GetConfigFloat("Skyscraper.SBS.Camera.SlowSpeed", 0.5);
	cfg_zoomspeed = sbs->GetConfigFloat("Skyscraper.SBS.Camera.ZoomSpeed", 0.2);
	speed = 1;
	Collisions = 0;
	lastfloor = 0;
	lastfloorset = false;
	MouseLeftDown = false;
	MouseRightDown = false;
	ReportCollisions = sbs->GetConfigBool("Skyscraper.SBS.Camera.ReportCollisions", false);
	Freelook = sbs->GetConfigBool("Skyscraper.SBS.Camera.Freelook", false);
	Freelook_speed = sbs->GetConfigFloat("Skyscraper.SBS.Camera.FreelookSpeed", 200.0);
	FOV = sbs->GetConfigFloat("Skyscraper.SBS.Camera.FOV", 71.263794);
	last_fov = FOV;
	FarClip = sbs->GetConfigFloat("Skyscraper.SBS.Camera.MaxDistance", 0.0);
	object_number = 0;
	object_line = 0;
	HitPosition = Vector3::ZERO;
	RotationStopped = false;
	MovementStopped = false;
	accum_movement = Vector3::ZERO;
	prev_accum_movement = Vector3::ZERO;
	collision_reset = false;
	EnableBullet = sbs->GetConfigBool("Skyscraper.SBS.Camera.EnableBullet", true);
	use_startdirection = false;
	BinocularsFOV = sbs->GetConfigFloat("Skyscraper.SBS.Camera.BinocularsFOV", 10.0);
	RestrictRotation = sbs->GetConfigBool("Skyscraper.SBS.Camera.RestrictRotation", true);
	AttachedModel = 0;
	prev_orientation = Quaternion::ZERO;
	prev_position = Vector3::ZERO;
	Gravity = 0;
	GravityStatus = false;
	FirstAttach = false;
	LastHitMeshNumber = -1;
	mouse_x = 0;
	mouse_y = 0;
	inside_vehicle = false;
	vehicle = 0;
	old_freelook_mode = false;
	BinocularsState = false;

	//set up collider character
	Real width = cfg_legs_width / 2;
	if (cfg_body_width > cfg_legs_width)
		width = cfg_body_width / 2;

	Real height = (cfg_body_height + cfg_legs_height - 0.5) - (width * 2);
	Real step_height = cfg_legs_height - 0.5;

	mCharacter = 0;
	mShape = 0;

	if (EnableBullet == true)
	{
		mCharacter = new OgreBulletDynamics::CharacterController(GetSceneNode()->GetFullName() + " Character", sbs->mWorld, GetSceneNode()->GetRawSceneNode(), sbs->ToRemote(width), sbs->ToRemote(height), sbs->ToRemote(step_height));

		//create debug shape
		mShape = new OgreBulletCollisions::CapsuleCollisionShape(sbs->ToRemote(width), sbs->ToRemote(height), Vector3::UNIT_Y);
		mCharacter->setShape(mShape);

		//other movement options
		mCharacter->setJumpSpeed(sbs->ToRemote(cfg_jumpspeed));
		mCharacter->setFallSpeed(sbs->ToRemote(sbs->GetConfigFloat("Skyscraper.SBS.Camera.FallSpeed", 177.65)));
	}
}

Camera::~Camera()
{
	//Destructor

	//reset view mode on shutdown
	SetViewMode(0);

	//delete model
	if (AttachedModel)
	{
		AttachedModel->parent_deleting = true;
		delete AttachedModel;
		AttachedModel = 0;
	}

	if (mCharacter)
		delete mCharacter;

	//detach the camera
	Detach();

	if (GetSceneNode()->GetRawSceneNode()->numChildren() > 0)
	{
		std::string nodename = GetSceneNode()->GetRawSceneNode()->getChild(0)->getName();
		sbs->mSceneManager->destroySceneNode(nodename);
	}
}

void Camera::SetPosition(const Vector3 &position, bool force)
{
	//sets the camera to an absolute position in 3D space

	if (Cameras.empty())
		return;

	if (EnableBullet == true)
		GetSceneNode()->SetPosition(position - sbs->ToLocal(Cameras[0]->getPosition()), false, force);
	else
	{
		for (size_t i = 0; i < Cameras.size(); i++)
		{
			Cameras[i]->setPosition(sbs->ToRemote(position));
		}
	}

	OnMove(false);
}

void Camera::SetDirection(const Vector3 &direction)
{
	//sets the camera's direction to an absolute position

	if (Cameras.empty())
		return;

	//MainCamera->lookAt(sbs->ToRemote(direction));
}

void Camera::SetRotation(const Vector3 &rotation)
{
	//sets the camera's rotation in degrees

	if (Cameras.empty())
		return;

	//keep rotation within 360 degree boundaries
	Vector3 vector = rotation;
	if (RestrictRotation && vector.x > 90 && vector.x < 270)
		vector.x = vector.x > 180 ? 270 : 90;
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

	Quaternion x(Degree(vector.x), Vector3::UNIT_X);
	Quaternion y(Degree(vector.y), Vector3::NEGATIVE_UNIT_Y);
	Quaternion z(Degree(vector.z), Vector3::UNIT_Z);
	Quaternion camrot = x * z;
	Quaternion bodyrot = y;
	Rotation = vector;
	for (size_t i = 0; i < Cameras.size(); i++)
	{
		Cameras[i]->setOrientation(camrot);
	}

	if (EnableBullet == true)
		mCharacter->setOrientation(sbs->GetUtility()->ToGlobal(bodyrot));

	OnRotate(false);
}

Vector3 Camera::GetPosition(bool relative)
{
	//returns the camera's current position
	//"relative" value is ignored for camera object

	Vector3 cameraposition = Vector3::ZERO;

	if (!Cameras.empty())
		cameraposition = sbs->ToLocal(Cameras[0]->getPosition());

	return GetSceneNode()->GetPosition() + cameraposition;
}

void Camera::GetDirection(Vector3 &front, Vector3 &top, bool global)
{
	//returns the camera's current direction in front and top vectors

	if (Cameras.empty())
		return;

	Quaternion dir;
	if (global == false)
		dir = sbs->GetUtility()->FromGlobal(Cameras[0]->getDerivedOrientation());
	else
		dir = Cameras[0]->getDerivedOrientation();

	front = dir.zAxis();
	front.x = -front.x; //convert to left-hand coordinate system
	front.y = -front.y; //convert to left-hand coordinate system
	front.normalise();
	top = dir.yAxis();
	top.z = -top.z; //convert to left-hand coordinate system
	top.normalise();
}

Vector3 Camera::GetRotation()
{
	//returns the camera's current rotation

	return Rotation;
}

void Camera::UpdateCameraFloor()
{
	SBS_PROFILE("Camera::UpdateCameraFloor");

	if (Cameras.empty())
		return;

	int newlastfloor;

	if (lastfloorset == true)
		newlastfloor = sbs->GetFloorNumber((float)GetPosition().y, lastfloor, true);
	else
		newlastfloor = sbs->GetFloorNumber((float)GetPosition().y);

	//if camera moved to a different floor, update floor indicators
	if ((lastfloor != newlastfloor) && sbs->GetFloor(newlastfloor))
		sbs->GetFloor(newlastfloor)->UpdateFloorIndicators();

	lastfloor = newlastfloor;
	lastfloorset = true;
	CurrentFloor = lastfloor;
	if (sbs->GetFloor(CurrentFloor))
		CurrentFloorID = sbs->GetFloor(CurrentFloor)->ID;
}

bool Camera::Move(Vector3 vector, Real speed, bool flip)
{
	//moves the camera in a relative amount specified by a vector

	if (Cameras.empty())
		return false;

	if (MovementStopped == true && vector == Vector3::ZERO)
		return false;

	MovementStopped = false;

	if (vector == Vector3::ZERO)
		MovementStopped = true;

	if (flip == true)
		vector *= Vector3(-1, 1, 1);

	Quaternion orientation;

	if (EnableBullet == true)
		orientation = GetSceneNode()->GetDerivedOrientation();
	else
		orientation = Cameras[0]->getOrientation();

	//flip X axis
	vector *= Vector3(-1, 1, 1);

	//multiply vector with camera's orientation
	vector = orientation * sbs->ToRemote(vector);

	accum_movement += (vector * speed);

	return true;
}

bool Camera::MovePosition(Vector3 vector, Real speed)
{
	//moves the camera in a relative amount, using SetPosition, instead of character movement

	if (Cameras.empty())
		return false;

	if (vector == Vector3::ZERO)
		return false;

	SetPosition(GetPosition() + (vector * speed));
	return true;
}

void Camera::Rotate(const Vector3 &rotation, Real speed)
{
	//rotates the camera in a relative amount in world space

	if (Cameras.empty())
		return;

	Vector3 rot = GetRotation() + (rotation * speed);
	SetRotation(rot);
}

void Camera::RotateLocal(const Vector3 &rotation, Real speed)
{
	//rotates the camera in a relative amount in local camera space

	if (Cameras.empty())
		return;

	if (RotationStopped == true && rotation == Vector3::ZERO)
		return;

	RotationStopped = false;

	if (rotation == Vector3::ZERO)
		RotationStopped = true;

	//convert rotation values to degrees
	Real xdeg = (float)Ogre::Math::RadiansToDegrees(rotation.x) * speed; //X axis (up/down)
	Real ydeg = (float)Ogre::Math::RadiansToDegrees(rotation.y) * speed; //Y axis (left/right)
	Real zdeg = (float)Ogre::Math::RadiansToDegrees(rotation.z) * speed; //Z axis (clockwise/counterclockwise)
	if (RestrictRotation && Rotation.x + xdeg > 90 && Rotation.x + xdeg < 270)
		xdeg = Rotation.x > 180 ? 270-Rotation.x : 90-Rotation.x;
	Rotation.x += xdeg;
	Rotation.y += ydeg;
	Rotation.z += zdeg;

	if (Rotation.x > 359)
		Rotation.x -= 360;
	if (Rotation.y > 359)
		Rotation.y -= 360;
	if (Rotation.z > 359)
		Rotation.z -= 360;
	if (Rotation.x < 0)
		Rotation.x += 360;
	if (Rotation.y < 0)
		Rotation.y += 360;
	if (Rotation.z < 0)
		Rotation.z += 360;

	for (size_t i = 0; i < Cameras.size(); i++)
	{
		Quaternion rot(Degree(Rotation.y), Vector3::NEGATIVE_UNIT_Y);
		if (EnableBullet == true)
		{
			//rotate character collider
			mCharacter->setOrientation(sbs->GetUtility()->ToGlobal(rot));

			//rotate camera
			Cameras[i]->pitch(Degree(xdeg));
			Cameras[i]->roll(Degree(zdeg));
		}
		else
			Cameras[i]->setOrientation(rot);
	}

	OnRotate(false);
}

void Camera::SetStartDirection(const Vector3 &direction)
{
	StartDirection = direction;
	use_startdirection = true;
}

Vector3 Camera::GetStartDirection()
{
	return StartDirection;
}

void Camera::SetStartRotation(const Vector3 &rotation)
{
	StartRotation = rotation;
}

Vector3 Camera::GetStartRotation()
{
	return StartRotation;
}

void Camera::SetToStartPosition(bool disable_current_floor)
{
	if (sbs->GetFloor(StartFloor))
	{
		SetPosition(Vector3(StartPositionX, 0, StartPositionZ));
		GotoFloor(StartFloor, disable_current_floor);
	}
	else
		SetPosition(Vector3(StartPositionX, GetHeight(), StartPositionZ));
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

	SBS_PROFILE("Camera::CheckElevator");

	if (Cameras.empty())
		return;

	for (int i = 1; i <= sbs->GetElevatorCount(); i++)
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

	if (Cameras.empty())
		return;

	if (sbs->AutoShafts == false)
		return;

	SBS_PROFILE("Camera::CheckShaft");

	for (int i = 1; i <= sbs->GetShaftCount(); i++)
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

	if (Cameras.empty())
		return;

	if (sbs->AutoStairs == false)
		return;

	SBS_PROFILE("Camera::CheckStairwell");

	for (int i = 1; i <= sbs->GetStairwellCount(); i++)
	{
		Stairwell *stairs = sbs->GetStairwell(i);

		if (!stairs)
			continue;

		stairs->Check(GetPosition(), CurrentFloor, FloorTemp);
	}
	FloorTemp = CurrentFloor;
}

Real Camera::ClickedObject(Camera *camera, bool shift, bool ctrl, bool alt, bool right, Real scale, bool center_only, bool hit_only)
{
	//get mesh object that the user clicked on, and perform related work

	if (!camera)
		return false;

	Real result = -1;

	SBS_PROFILE("Camera::ClickedObject");

	if (!camera->GetOgreCamera())
		return false;

	//cast a ray from the camera in the direction of the clicked position
	int width = camera->GetOgreCamera()->getViewport()->getActualWidth();
	int height = camera->GetOgreCamera()->getViewport()->getActualHeight();

	if (width == 0 || height == 0)
		return result;

	Real x, y;
	if (center_only == false)
	{
		x = (float)camera->mouse_x / (float)width * scale;
		y = (float)camera->mouse_y / (float)height * scale;
	}
	else
	{
		x = 0.5;
		y = 0.5;
	}

	Ray ray = camera->GetOgreCamera()->getCameraToViewportRay(x, y);

	//convert ray's origin and direction to engine-relative values
	ray.setOrigin(sbs->ToRemote(sbs->GetUtility()->FromGlobal(sbs->ToLocal(ray.getOrigin()))));
	ray.setDirection(sbs->GetOrientation().Inverse() * ray.getDirection());

	MeshObject* mesh = 0;
	Wall* wall = 0;
	Polygon* polygon = 0;

	Vector3 pos = sbs->ToLocal(ray.getOrigin());

	//only report if in verbose mode, and if the camera is outside of this simulator
	if (sbs->Verbose && hit_only == false)
	{
		if (Cameras.size() == 0)
		{
			Report("Clicked from (" + ToString(pos.x) + ", " + ToString(pos.y) + ", " + ToString(pos.z) + ")");
		}
	}

	bool hit = sbs->GetUtility()->HitBeam(ray, 1000.0, mesh, wall, polygon, HitPosition);
	Vector3 hit_pos = HitPosition - sbs->GetPosition();

	//report hit position if in verbose mode
	if (hit == true)
	{
		result = pos.distance(hit_pos);
		if (sbs->Verbose && hit_only == false)
			Report("Hit at (" + ToString(hit_pos.x) + ", " + ToString(hit_pos.y) + ", " + ToString(hit_pos.z)+ ")");
	}

	if (hit == false || hit_only == true)
		return result;

	meshname = mesh->GetName();
	wallname = "";
	Object *obj = mesh;

	if (wall)
	{
		wallname = wall->GetName();
		obj = wall;
	}

	//store parameters of object
	object_number = obj->GetNumber();
	object_line = obj->linenum;
	object_cmd = obj->command;
	object_cmd_processed = obj->command_processed;

	//show result
	std::string number = ToString(object_number);
	if (wall)
		Report("Clicked on object " + number + ": Mesh: " + meshname + ", Wall: " + wallname);


	//object checks and actions

	//get original object (parent object of clicked mesh)
	Object *mesh_parent = GetMeshParent(obj);

	if (!mesh_parent)
		return result;

	if (mesh_parent->GetType() == "ButtonPanel")
	{
		//for call station panels, the object needs to be the third level
		Object* callpanel = mesh_parent->GetParent();
		if (callpanel)
		{
			if (callpanel->GetType() == "CallStation")
				mesh_parent = callpanel;
		}
	}

	//show result
	if (!wall)
		Report("Clicked on object " + number + ": " + mesh_parent->GetName()+ " - Mesh: " + meshname);

	//delete object if ctrl and alt keys are pressed
	if (ctrl == true && alt == true && shift == false && right == false)
	{
		//delete a wall object, for certain parent objects
		if (wall && obj->GetType() == "Wall")
		{
			std::string type = mesh_parent->GetType();
			if (type == "Floor" || type == "ElevatorCar" || type == "Shaft" || type == "Stairwell" || type == "SBS")
			{
				sbs->DeleteObject(obj);
				return result;
			}
		}

		//for standard objects, delete the mesh parent object
		sbs->DeleteObject(mesh_parent);
		return result;
	}

	//call object's OnClick function
	mesh_parent->OnClick(pos, shift, ctrl, alt, right);
	return result;
}

void Camera::UnclickedObject()
{
	//this function is called when a user releases the mouse button on an object

	Object *obj = sbs->GetObject(object_number);

	if (!obj)
		return;

	//get original object (parent object of clicked mesh)
	Object *mesh_parent = GetMeshParent(obj);

	if (!mesh_parent)
		return;

	//call object's OnUnclick function
	mesh_parent->OnUnclick(MouseRightDown);

	MouseLeftDown = false;
	MouseRightDown = false;
}

Object* Camera::GetMeshParent(Object *object)
{
	//get original object (parent object of clicked mesh)

	if (!object)
		return 0;

	Object *mesh_parent = object->GetParent();

	if (!mesh_parent)
		return 0;

	//if object is a wall, and parent is a mesh, get mesh's (parent's) parent
	//or if object is a mesh, and parent is a scenenode, get scenenode's parent
	if (mesh_parent->GetType() == "Mesh" || mesh_parent->GetType() == "SceneNode")
	{
		mesh_parent = mesh_parent->GetParent();

		if (!mesh_parent)
			return 0;
	}

	return mesh_parent;
}

std::string Camera::GetClickedMeshName()
{
	//return name of last clicked mesh

	return meshname;
}

std::string Camera::GetClickedWallName()
{
	//return name of last clicked polygon

	return wallname;
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

std::string Camera::GetClickedObjectCommand()
{
	//return command line of last clicked object
	return object_cmd;
}

std::string Camera::GetClickedObjectCommandP()
{
	//return processed command line of last clicked object
	return object_cmd_processed;
}

bool Camera::Loop()
{
	SBS_PROFILE_MAIN("Camera Loop");

	if (Cameras.empty())
		return true;

	//get delta value
	unsigned long timing;
	if (sbs->SmoothFrames > 0)
		timing = sbs->GetAverageTime();
	else
		timing = sbs->GetElapsedTime();
	Real delta = timing / 1000.0;

	//reset collisions if needed
	if (collision_reset == true && EnableBullet == true)
		mCharacter->resetCollisions();
	collision_reset = false;

	//get name of the last hit mesh object
	if (sbs->GetRunTime() > 10) //due to initialization-related crashes, wait before checking
	{
		if (EnableBullet == true)
		{
			if (mCharacter->getLastCollision())
			{
				Ogre::SceneNode *node = mCharacter->getLastCollision()->getRootNode();
				if (node)
				{
					int instance = 0, number = 0;
					std::string name = sbs->GetUtility()->ProcessFullName(node->getParentSceneNode()->getName(), instance, number, false);

					if (instance == sbs->InstanceNumber)
					{
						LastHitMesh = name;
						LastHitMeshNumber = number;
					}
				}
			}
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
			Report(LastHitMesh);

		if (LastHitMesh != "")
		{
			//get SBS object
			Object *obj = sbs->GetObject(LastHitMeshNumber);

			if (obj)
			{
				//call hit on mesh object
				obj->OnHit();

				//call hit on original object (parent object of mesh)
				if (obj->GetParent())
					obj->GetParent()->OnHit();
			}
		}
	}

	//if a model is attached, run it's loop
	if (AttachedModel)
		AttachedModel->Loop();

	//sync camera with collider
	Sync();

	return true;
}

void Camera::Strafe(Real speed)
{
	speed *= cfg_walk_maxspeed_multreal;
	desired_velocity.x = -cfg_strafespeed * speed * cfg_walk_maxspeed * cfg_walk_maxspeed_multreal;
}

void Camera::Step(Real speed)
{
	speed *= cfg_walk_maxspeed_multreal;
	desired_velocity.z = cfg_stepspeed * speed * cfg_walk_maxspeed * cfg_walk_maxspeed_multreal;
}

void Camera::Float(Real speed)
{
	speed *= cfg_walk_maxspeed_multreal;
	desired_velocity.y = cfg_floatspeed * speed * cfg_walk_maxspeed * cfg_walk_maxspeed_multreal;
}

void Camera::Jump()
{
	if (Cameras.empty())
		return;

	//velocity.y = cfg_jumpspeed;
	//desired_velocity.y = 0.0;
	if (EnableBullet == true)
	{
		if (mCharacter->getGravity() != 0)
			mCharacter->jump();
	}
}

void Camera::Look(Real speed)
{
	//look up/down by rotating camera on X axis
	desired_angle_velocity.x = cfg_lookspeed * speed * cfg_rotate_maxspeed;
}

void Camera::Turn(Real speed)
{
	//turn camera by rotating on Y axis
	desired_angle_velocity.y = cfg_turnspeed * speed * cfg_rotate_maxspeed * cfg_walk_maxspeed_multreal;
}

void Camera::Spin(Real speed)
{
	//spin camera by rotating on Z axis
	desired_angle_velocity.z = cfg_spinspeed * speed * cfg_rotate_maxspeed;
}

void Camera::FreelookMove(const Vector3 &rotation)
{
	desired_angle_velocity = rotation * Freelook_speed;
	angle_velocity = desired_angle_velocity;
}

void Camera::InterpolateMovement(Real delta)
{
	//calculate movement and rotation acceleration

	delta *= 1700.0;

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

void Camera::SetGravity(Real gravity, bool save_value)
{
	if (save_value == true)
		Gravity = gravity;

	if (EnableBullet == true && !Cameras.empty())
	{
		sbs->mWorld->setGravity(Vector3(0, sbs->ToRemote(-gravity), 0));
		mCharacter->setGravity(sbs->ToRemote(gravity));
	}
}

Real Camera::GetGravity()
{
	return Gravity;
}

void Camera::EnableGravity(bool value)
{
	if (Cameras.empty())
		return;

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

void Camera::SetFOVAngle(Real angle)
{
	//set camera FOV angle

	if (Cameras.empty())
		return;

	if (angle > 0 && angle < 179.63)
	{
		Real ratio = (float)Cameras[0]->getAspectRatio();
		if (ratio > 0)
		{
			for (size_t i = 0; i < Cameras.size(); i++)
			{
				Cameras[i]->setFOVy(Degree(angle / ratio));
			}
		}
	}
}

Real Camera::GetFOVAngle()
{
	if (Cameras.empty())
		return 0.0;

	return (float)(Cameras[0]->getFOVy().valueDegrees() * Cameras[0]->getAspectRatio());
}

void Camera::SetToDefaultFOV()
{
	//set to default FOV angle value
	SetFOVAngle(FOV);
}

Real Camera::GetHeight()
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

	if (Cameras.empty())
		return;

	for (size_t i = 0; i < Cameras.size(); i++)
	{
		if (mode == 0)
			Cameras[i]->setPolygonMode(Ogre::PM_SOLID);
		if (mode == 1)
			Cameras[i]->setPolygonMode(Ogre::PM_WIREFRAME);
		if (mode == 2)
			Cameras[i]->setPolygonMode(Ogre::PM_POINTS);
	}
}

void Camera::EnableCollisions(bool value)
{
	if (Cameras.empty())
		return;

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
	if (Cameras.empty())
		return false;

	if (EnableBullet == true)
		return mCharacter->onGround();
	else
		return false;
}

void Camera::Sync()
{
	//sync scene node with bullet object

	if (Cameras.empty())
		return;

	SBS_PROFILE_MAIN("Camera Sync");

	if (EnableBullet == true)
		mCharacter->sync();

	//notify on movement or rotation
	Vector3 position = sbs->ToRemote(GetPosition());
	Quaternion orientation = GetOrientation();

	if (prev_position.positionEquals(position) == false)
	{
		GetSceneNode()->Update();
		NotifyMove();
	}
	if (prev_orientation.orientationEquals(orientation) == false)
	{
		GetSceneNode()->Update();
		NotifyRotate();
	}

	prev_position = position;
	prev_orientation = orientation;
}

void Camera::SetMaxRenderDistance(Real value)
{
	//set distance of camera's far clipping plane - set to 0 for infinite

	if (Cameras.empty())
		return;

	for (size_t i = 0; i < Cameras.size(); i++)
	{
		Cameras[i]->setFarClipDistance(sbs->ToRemote(value));
	}
	FarClip = value;
}

Real Camera::GetMaxRenderDistance()
{
	return FarClip;
}

void Camera::ShowDebugShape(bool value)
{
	if (Cameras.empty())
		return;

	if (EnableBullet == true)
		mCharacter->showDebugShape(value);
}

void Camera::MoveCharacter()
{
	if (Cameras.empty())
		return;

	SBS_PROFILE_MAIN("MoveCharacter");

	if (EnableBullet == true)
		mCharacter->setWalkDirection(accum_movement, 1);
	else
	{
		for (size_t i = 0; i < Cameras.size(); i++)
		{
			Cameras[i]->move(accum_movement);
		}
	}
	prev_accum_movement = accum_movement;
	accum_movement = Vector3::ZERO;
}

void Camera::ResetCollisions()
{
	//queue a collision reset for next loop cycle
	collision_reset = true;
}

void Camera::GotoFloor(int floor, bool disable_current)
{
	//have camera warp to specified floor

	if (Cameras.empty())
		return;

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
			Vector3 pos = GetPosition();
			pos.y = floorobj->GetBase() + GetHeight();
			SetPosition(pos);
			floorobj->Enabled(true);
			floorobj->EnableGroup(true);
		}
	}
}

void Camera::Binoculars(bool value)
{
	//enable or disable binoculars mode

	if (Cameras.empty())
		return;

	if (value == BinocularsState)
		return;

	if (value == true)
	{
		last_fov = GetFOVAngle();
		SetFOVAngle(BinocularsFOV);
	}
	else
		SetFOVAngle(last_fov);

	BinocularsState = value;
}

bool Camera::IsMeshVisible(MeshObject *mesh)
{
	//returns if a mesh object is visible in the camera's view frustum or not

	if (!mesh || Cameras.empty())
		return false;

	return mesh->IsVisible(Cameras[0]);
}

bool Camera::IsDynamicMeshVisible(DynamicMesh *mesh, int mesh_index)
{
	if (!mesh || Cameras.empty())
		return false;

	return mesh->IsVisible(Cameras[0], mesh_index);
}

void Camera::AttachModel(Model *model)
{
	//attach a model to the camera
	//this is used for picking up a model, which changes the model's parent to the camera

	if (!model || AttachedModel)
		return;

	AttachedModel = model;
}

void Camera::DetachModel()
{
	//detach a model from the camera

	AttachedModel = 0;
}

bool Camera::PickUpModel()
{
	//pick up model
	Vector3 front, top;
	GetDirection(front, top);

	Vector3 hit_position;
	MeshObject *mesh = 0;
	Wall *wall = 0;
	Polygon *polygon = 0;
	bool hit = false;

	//do a raycast from the collider's position, in the forward direction
	Vector3 position = GetPosition();

	for (int i = (int)GetPosition().y; i > 0; i--)
	{
		position.y = i;
		Ray ray (sbs->ToRemote(position), sbs->ToRemote(front, false));

		hit = sbs->GetUtility()->HitBeam(ray, 2.0, mesh, wall, polygon, hit_position);

		if (hit == true)
			break;
	}

	if (hit == false)
		return false;

	Model *model = dynamic_cast<Model*>(mesh->GetParent());

	if (model)
	{
		if (model->IsPhysical() == true)
		{
			model->PickUp();
			AttachModel(model);
			return true;
		}
	}

	return false;
}

void Camera::DropModel()
{
	//drop model

	if (AttachedModel)
	{
		AttachedModel->Drop();
		DetachModel();
	}
}

bool Camera::IsModelAttached()
{
	if (AttachedModel)
		return true;
	return false;
}

void Camera::ResetState()
{
	//reset camera position and state

	if (Cameras.empty())
		return;

	Floor *floor = sbs->GetFloor(CurrentFloor);
	if (floor)
	{
		floor->Enabled(false);
		floor->EnableGroup(false);
	}

	EnableGravity(true);
	EnableCollisions(true);
	SetToStartPosition(true);
	SetToStartRotation();
	SetToDefaultFOV();

	floor = sbs->GetFloor(StartFloor);
	if (floor)
	{
		floor->Enabled(true);
		floor->EnableGroup(true);
	}

	sbs->EnableBuildings(true);
	sbs->EnableLandscape(true);
	sbs->EnableExternal(true);
	sbs->EnableSkybox(true);
}

void Camera::ResetView()
{
	//reset rotation/direction and FOV of camera

	if (Cameras.empty())
		return;

	SetToStartDirection();
	SetToStartRotation();
	SetToDefaultFOV();
}

void Camera::Refresh()
{
	if (mCharacter)
		mCharacter->resetLastCollision();
}

bool Camera::Attach(std::vector<Ogre::Camera*>& cameras, bool init_state)
{
	if (cameras.empty())
		return false;

	if (cameras[0]->isAttached() == true)
		return false;

	Cameras = cameras;

	for (size_t i = 0; i < Cameras.size(); i++)
	{
		Cameras[i]->setNearClipDistance(0.1);
		Cameras[i]->setPosition(0, sbs->ToRemote((cfg_body_height + cfg_legs_height + 0.5) / 2), 0);
		GetSceneNode()->AttachObject(Cameras[i]);
	}

	SetFOVAngle(FOV);
	SetMaxRenderDistance(FarClip);

	//move camera to start location
	if (FirstAttach == false)
	{
		SetGravity(sbs->GetConfigFloat("Skyscraper.SBS.Camera.Gravity", 32.1719), true); // 9.806 m/s/s

		if (init_state == true)
		{
			GravityStatus = sbs->GetConfigBool("Skyscraper.SBS.Camera.GravityStatus", true);
			EnableCollisions(sbs->GetConfigBool("Skyscraper.SBS.Camera.EnableCollisions", true));
			SetToStartPosition(false); //also turns on start floor
			SetToStartDirection();
			SetToStartRotation();
		}
		FirstAttach = true;
	}
	else
	{
		//re-apply rotation
		if (init_state == true)
			SetRotation(Rotation);
	}

	Refresh();
	Sync();

	sbs->GetElevatorManager()->EnableAll(true);

	return true;
}

bool Camera::Detach()
{
	if (Cameras.empty())
		return false;

	if (Cameras[0]->isAttached() == false)
		return false;

	for (size_t i = 0; i < Cameras.size(); i++)
	{
		GetSceneNode()->DetachObject(Cameras[i]);
	}

	Cameras.clear();

	return true;
}

void Camera::OnMove(bool parent)
{
	if (EnableBullet == true)
		mCharacter->updateTransform(true, false, false);
}

void Camera::OnRotate(bool parent)
{
	if (parent == true)
		OnMove(parent); //update position if parent object has been rotated

	if (EnableBullet == true)
		mCharacter->updateTransform(false, true, false);
}

CameraState Camera::GetCameraState()
{
	//returns camera state info
	//the position value is in a global/absolute scene format, not engine-relative

	CameraState state;
	state.position = sbs->GetUtility()->ToGlobal(GetPosition());
	state.rotation = GetRotation() + sbs->GetRotation();
	state.floor = CurrentFloor;
	state.collisions = CollisionsEnabled();
	state.gravity = GetGravityStatus();
	state.freelook = Freelook;
	state.desired_velocity = desired_velocity;
	state.velocity = velocity;
	state.desired_angle_velocity = desired_angle_velocity;
	state.angle_velocity = angle_velocity;
	state.accum_movement = accum_movement;
	state.fov = GetFOVAngle();
	state.speed = speed;
	return state;
}

void Camera::SetCameraState(const CameraState &state, bool set_floor)
{
	//sets camera state
	//the position value is in a global/absolute scene format, not engine-relative

	Vector3 position = sbs->GetUtility()->FromGlobal(state.position);
	if (set_floor == true)
		GotoFloor(state.floor, true);
	else
		GotoFloor(sbs->GetFloorNumber((float)position.y));
	SetPosition(position);
	SetRotation(state.rotation - sbs->GetRotation());
	EnableCollisions(state.collisions);
	EnableGravity(state.gravity);
	Freelook = state.freelook;
	desired_velocity = state.desired_velocity;
	velocity = state.velocity;
	desired_angle_velocity = state.desired_angle_velocity;
	angle_velocity = state.angle_velocity;
	accum_movement = state.accum_movement;
	SetFOVAngle(state.fov);
	speed = state.speed;
}

void Camera::RevertMovement()
{
	accum_movement = -prev_accum_movement;
}

bool Camera::MouseDown()
{
	//returns true if either mouse button is down
	return (MouseLeftDown || MouseRightDown);
}

void Camera::CheckObjects()
{
	//check if the user is in an elevator
	if (sbs->ProcessElevators == true)
		CheckElevator();

	//check if the user is in a shaft
	CheckShaft();

	//check if the user is in a stairwell
	CheckStairwell();
}

void Camera::Teleport(Real X, Real Y, Real Z)
{
	//teleport/warp user to specified location

	Vector3 destination (X, Y, Z);

	sbs->GetTeleporterManager()->teleported = true;
	GotoFloor(sbs->GetFloorNumber(destination.y));
	SetPosition(destination, true);
}

void Camera::Drive(bool left, bool right, bool down, bool up, bool key_down)
{
	if (!vehicle)
		return;

	if (key_down == true)
		vehicle->KeyPressed(left, right, down, up);
	else
		vehicle->KeyReleased(left, right, down, up);
}

void Camera::Crouch(bool value)
{
	if (mCharacter)
		mCharacter->crouch(value);
}

void Camera::SetOrientation(const Quaternion &orientation)
{
	//set orientation of main camera object, not collider

	for (size_t i = 0; i < Cameras.size(); i++)
	{
		Cameras[i]->setOrientation(orientation);
	}
}

void Camera::AttachToVehicle(bool value)
{
	//attach/detach camera from a vehicle

	if (Cameras.empty())
		return;

	if (vehicle && value == false)
	{
		Vector3 newpos = GetPosition() + (vehicle->GetOrientation() * Vector3(vehicle->GetWidth(), 0, 0));
		SetPosition(newpos);

		Freelook = old_freelook_mode;
		inside_vehicle = false;
		vehicle->AttachCamera(false);
		vehicle = 0;
		EnableCollisions(true);
		for (size_t i = 0; i < Cameras.size(); i++)
		{
			Cameras[i]->setOrientation(old_camera_orientation);
		}
		if (EnableBullet == true)
			mCharacter->setOrientation(old_character_orientation);
	}
	else if (value == true)
	{
		//search for a vehicle

		Vector3 direction, other;
		GetDirection(direction, other);
		Ray ray = Ray(sbs->ToRemote(GetPosition()), direction);

		MeshObject* mesh = 0;
		Wall* wall = 0;
		Polygon* polygon = 0;

		bool hit = sbs->GetUtility()->HitBeam(ray, 50, mesh, wall, polygon, HitPosition);

		if (hit == false)
		{
			ReportError("AttachToVehicle: No vehicles found");
			return;
		}

		meshname = mesh->GetName();
		Object *obj = mesh;

		//get original object (parent object of clicked mesh)
		Object *mesh_parent = GetMeshParent(obj);

		if (mesh_parent->GetType() == "Vehicle")
		{
			//if a vehicle is found, attach to it

			Vehicle *v = dynamic_cast<Vehicle*> (mesh_parent);

			if (v)
			{
				vehicle = v;
				old_freelook_mode = Freelook;
				Freelook = true;
				inside_vehicle = true;
				EnableCollisions(false);
				old_camera_orientation = Cameras[0]->getOrientation();
				if (EnableBullet == true)
					old_character_orientation = mCharacter->getWorldOrientation();
				vehicle->AttachCamera(true);
			}
		}
		else
			ReportError("AttachToVehicle: No vehicles found");
	}
}

Ogre::Camera* Camera::GetOgreCamera(int index)
{
	if (!Cameras.empty())
		return Cameras[index];
	return 0;
}

}
