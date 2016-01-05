/* $Id$ */

/*
	Scalable Building Simulator - Camera Object
	The Skyscraper Project - Version 1.10 Alpha
	Copyright (C)2004-2015 Ryan Thoryk
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

namespace SBS {

#undef SMALL_EPSILON
#define SMALL_EPSILON 0.000001f

Camera::Camera(Object *parent) : Object(parent)
{
	//set up SBS object
	SetValues("Camera", "Camera", true);

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
	BinocularsFOV = sbs->GetConfigFloat("Skyscraper.SBS.Camera.BinocularsFOV", 10.0f);
	AttachedModel = 0;
	prev_orientation = Ogre::Quaternion::ZERO;
	prev_position = Ogre::Vector3::ZERO;
	MainCamera = 0;
	Gravity = 0;
	GravityStatus = false;
	FirstAttach = false;
	LastHitMeshNumber = -1;

	//set up camera and scene nodes

	//set up collider character
	float width = cfg_legs_width / 2;
	if (cfg_body_width > cfg_legs_width)
		width = cfg_body_width / 2;

	float height = (cfg_body_height + cfg_legs_height - 0.5f) - (width * 2);
	float step_height = cfg_legs_height - 0.5f;

	mCharacter = 0;
	mShape = 0;

	mCharacter = new OgreBulletDynamics::CharacterController(GetSceneNode()->GetFullName() + " Character", sbs->mWorld, GetSceneNode()->GetRawSceneNode(), sbs->ToRemote(width), sbs->ToRemote(height), sbs->ToRemote(step_height));

	//create debug shape
	mShape = new OgreBulletCollisions::CapsuleCollisionShape(sbs->ToRemote(width), sbs->ToRemote(height), Ogre::Vector3::UNIT_Y);
	mCharacter->setShape(mShape);

	//other movement options
	mCharacter->setJumpSpeed(sbs->ToRemote(cfg_jumpspeed));
	mCharacter->setFallSpeed(sbs->ToRemote(sbs->GetConfigFloat("Skyscraper.SBS.Camera.FallSpeed", 177.65f)));
}

Camera::~Camera()
{
	//Destructor

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

void Camera::SetPosition(const Ogre::Vector3 &position)
{
	//sets the camera to an absolute position in 3D space

	if (!MainCamera)
		return;

	if (EnableBullet == true)
		GetSceneNode()->SetPosition(position - sbs->ToLocal(MainCamera->getPosition()));
	else
		MainCamera->setPosition(sbs->ToRemote(position));

	OnMove(false);
}

void Camera::SetDirection(const Ogre::Vector3 &direction)
{
	//sets the camera's direction to an absolute position

	if (!MainCamera)
		return;

	//MainCamera->lookAt(sbs->ToRemote(direction));
}

void Camera::SetRotation(const Ogre::Vector3 &rotation)
{
	//sets the camera's rotation in degrees

	if (!MainCamera)
		return;

	//keep rotation within 360 degree boundaries
	Ogre::Vector3 vector = rotation;
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
	Rotation = vector;
	MainCamera->setOrientation(camrot);

	if (EnableBullet == true)
		mCharacter->setOrientation(sbs->GetOrientation() * bodyrot);

	OnRotate(false);
}

Ogre::Vector3 Camera::GetPosition(bool relative)
{
	//returns the camera's current position
	//"relative" value is ignored for camera object

	Ogre::Vector3 cameraposition = Ogre::Vector3::ZERO;

	if (MainCamera)
		cameraposition = sbs->ToLocal(MainCamera->getPosition());

	return GetSceneNode()->GetPosition() + cameraposition;
}

void Camera::GetDirection(Ogre::Vector3 &front, Ogre::Vector3 &top)
{
	//returns the camera's current direction in front and top vectors

	if (!MainCamera)
		return;

	Ogre::Quaternion dir = sbs->GetOrientation().Inverse() * MainCamera->getDerivedOrientation();

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

	return Rotation;
}

void Camera::UpdateCameraFloor()
{
	if (!MainCamera)
		return;

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

	if (!MainCamera)
		return false;

	if (MovementStopped == true && vector == Ogre::Vector3::ZERO)
		return false;

	MovementStopped = false;

	if (vector == Ogre::Vector3::ZERO)
		MovementStopped = true;

	if (flip == true)
		vector *= Ogre::Vector3(-1, 1, 1);

	Ogre::Quaternion orientation;

	if (EnableBullet == true)
		orientation = GetSceneNode()->GetRawSceneNode()->_getDerivedOrientation();
	else
		orientation = MainCamera->getOrientation();

	//flip X axis
	vector *= Ogre::Vector3(-1, 1, 1);

	//multiply vector with camera's orientation
	vector = orientation * sbs->ToRemote(vector);

	accum_movement += (vector * speed);

	return true;
}

bool Camera::MovePosition(Ogre::Vector3 vector, float speed)
{
	//moves the camera in a relative amount, using SetPosition, instead of character movement

	if (!MainCamera)
		return false;

	if (vector == Ogre::Vector3::ZERO)
		return false;

	SetPosition(GetPosition() + (vector * speed));
	return true;
}

void Camera::Rotate(const Ogre::Vector3 &rotation, float speed)
{
	//rotates the camera in a relative amount in world space

	if (!MainCamera)
		return;

	Ogre::Vector3 rot = GetRotation() + (rotation * speed);
	SetRotation(rot);
}

void Camera::RotateLocal(const Ogre::Vector3 &rotation, float speed)
{
	//rotates the camera in a relative amount in local camera space

	if (!MainCamera)
		return;

	if (RotationStopped == true && rotation == Ogre::Vector3::ZERO)
		return;

	RotationStopped = false;

	if (rotation == Ogre::Vector3::ZERO)
		RotationStopped = true;

	//convert rotation values to degrees
	float xdeg = Ogre::Math::RadiansToDegrees(rotation.x) * speed; //X axis (up/down)
	float ydeg = Ogre::Math::RadiansToDegrees(rotation.y) * speed; //Y axis (left/right)
	float zdeg = Ogre::Math::RadiansToDegrees(rotation.z) * speed; //Z axis (clockwise/counterclockwise)
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

	Ogre::Quaternion rot(Ogre::Degree(Rotation.y), Ogre::Vector3::NEGATIVE_UNIT_Y);
	if (EnableBullet == true)
	{
		//rotate character collider
		mCharacter->setOrientation(sbs->GetOrientation() * rot);

		//rotate camera
		MainCamera->pitch(Ogre::Degree(xdeg));
		MainCamera->roll(Ogre::Degree(zdeg));
	}
	else
		MainCamera->setOrientation(rot);

	OnRotate(false);
}

void Camera::SetStartDirection(const Ogre::Vector3 &direction)
{
	StartDirection = direction;
	use_startdirection = true;
}

Ogre::Vector3 Camera::GetStartDirection()
{
	return StartDirection;
}

void Camera::SetStartRotation(const Ogre::Vector3 &rotation)
{
	StartRotation = rotation;
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

	SBS_PROFILE("Camera::CheckElevator");

	if (!MainCamera)
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

	if (!MainCamera)
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

	if (!MainCamera)
		return;

	if (sbs->AutoStairs == false)
		return;

	SBS_PROFILE("Camera::CheckStairwell");
	for (int i = 1; i <= sbs->GetStairsCount(); i++)
	{
		Stairs *stairs = sbs->GetStairs(i);

		if (!stairs)
			continue;

		stairs->Check(GetPosition(), CurrentFloor, FloorTemp);
	}
	FloorTemp = CurrentFloor;
}

void Camera::ClickedObject(bool shift, bool ctrl, bool alt, bool right)
{
	//get mesh object that the user clicked on, and perform related work

	if (!MainCamera)
		return;

	Ogre::Vector3 pos = GetPosition();

	//cast a ray from the camera in the direction of the clicked position
	SBS_PROFILE("Camera::ClickedObject");
	int width = MainCamera->getViewport()->getActualWidth();
	int height = MainCamera->getViewport()->getActualHeight();
	float x = (float)sbs->mouse_x / (float)width;
	float y = (float)sbs->mouse_y / (float)height;
	Ogre::Ray ray = MainCamera->getCameraToViewportRay(x, y);

	MeshObject* mesh = 0;
	WallObject* wall = 0;

	bool hit = sbs->HitBeam(ray, 1000.0f, mesh, wall, HitPosition);

	if (hit == false)
		return;

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
		sbs->Report("Clicked on object " + number + ": Mesh: " + meshname + ", Wall: " + wallname);
	else
		sbs->Report("Clicked on object " + number + ": " + meshname);

	//object checks and actions

	std::string type = obj->GetType();
	Object *mesh_parent = 0;

	if (obj->GetParent())
	{
		std::string parent_type = obj->GetParent()->GetType();

		//if object is a wall, and parent is a mesh, get mesh's (parent's) parent
		if (parent_type == "Mesh")
			mesh_parent = obj->GetParent()->GetParent();
		else
			mesh_parent = obj->GetParent();
	}

	//get original object (parent object of clicked mesh)
	if (mesh_parent)
	{
		//check controls
		Control *control = dynamic_cast<Control*>(mesh_parent);

		if (control)
		{
			//delete control if ctrl and alt keys are pressed
			if (ctrl == true && alt == true && shift == false)
			{
				sbs->DeleteObject(mesh_parent);
				return;
			}

			//toggle lock status if ctrl and shift are pressed
			if (ctrl == true && shift == true)
				control->ToggleLock();
			else
				control->Press(right);
		}

		//check doors
		Door *door = dynamic_cast<Door*>(mesh_parent);

		if (door && right == false)
		{
			//delete door if ctrl and alt keys are pressed
			if (ctrl == true && alt == true && shift == false)
			{
				sbs->DeleteObject(mesh_parent);
				return;
			}

			//toggle lock status if ctrl and shift are pressed
			if (ctrl == true && shift == true)
				door->ToggleLock(pos);
			else
			{
				//open and close doors
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
		}

		//check models
		Model *model = dynamic_cast<Model*>(mesh_parent);

		if (model && right == false)
		{
			//delete model if ctrl and alt keys are pressed
			if (ctrl == true && alt == true && shift == false)
			{
				sbs->DeleteObject(mesh_parent);
				return;
			}

			//if model is a key, add key to keyring and delete model
			if (model->IsKey() == true)
			{
				sbs->AddKey(model->GetKeyID(), model->GetName());
				sbs->DeleteObject(mesh_parent);
				return;
			}
		}

		//check call buttons
		CallButton *callbutton = dynamic_cast<CallButton*>(mesh_parent);

		if (callbutton && right == false)
		{
			int index = (int)meshname.find(":");
			int index2 = (int)meshname.find(":", index + 1);

			if (index > -1 && index2 > -1)
			{
				std::string direction = meshname.substr(index2 + 1);
				TrimString(direction);

				//delete call button if ctrl and alt keys are pressed
				if (ctrl == true && alt == true && shift == false)
				{
					sbs->DeleteObject(callbutton);
					return;
				}

				if (ctrl == true && shift == true)
				{
					//if ctrl and shift are held, toggle lock
					callbutton->ToggleLock();
				}
				else
				{
					//press button
					if (direction == "Up")
						callbutton->Call(true);
					else
						callbutton->Call(false);
				}
			}
		}

		//check elevator doors
		ElevatorDoor::DoorWrapper *wrapper = dynamic_cast<ElevatorDoor::DoorWrapper*>(mesh_parent);

		if (wrapper && shift == true && right == false)
		{
			ElevatorDoor *parent = wrapper->parent;
			if (!parent)
				return;

			Elevator *elevator = parent->elev;
			if (!elevator)
				return;

			int number = parent->Number;
			int floor = wrapper->floor;

			if (wrapper->IsShaftDoor == true)
			{
				//check shaft doors
				if (elevator->AreShaftDoorsOpen(number, floor) == false)
					elevator->OpenDoorsEmergency(number, 3, floor);
				else
					elevator->CloseDoorsEmergency(number, 3, floor);
			}
			else
			{
				//check elevator doors
				if (elevator->AreDoorsOpen(number) == false)
					elevator->OpenDoorsEmergency(number, 2);
				else
					elevator->CloseDoorsEmergency(number, 2);
			}
		}

		//check floor and directional indicators
		FloorIndicator *ind = dynamic_cast<FloorIndicator*>(mesh_parent);
		DirectionalIndicator *ind2 = dynamic_cast<DirectionalIndicator*>(mesh_parent);

		if ((ind || ind2) && right == false)
		{
			//delete indicator if ctrl and alt keys are pressed
			if (ctrl == true && alt == true && shift == false)
			{
				sbs->DeleteObject(mesh_parent);
				return;
			}
		}
	}

	//delete wall if ctrl and alt are pressed
	if (ctrl == true && alt == true && shift == false && right == false && object_number > 0)
	{
		if (wall && type == "Wall")
			sbs->DeleteObject(obj);
		else
			sbs->ReportError("Cannot delete object " + number);
		return;
	}
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

void Camera::Loop(float delta)
{
	SBS_PROFILE_MAIN("Camera Loop");

	if (!MainCamera)
		return;

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
					std::string name = sbs->ProcessFullName(node->getParentSceneNode()->getName(), instance, number, false);

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
			sbs->Report(LastHitMesh);

		if (LastHitMesh != "")
		{
			//get SBS object
			Object *obj = sbs->GetObject(LastHitMeshNumber);

			//get original object (parent object of mesh)
			if (obj)
			{
				if (obj->GetParent())
				{
					//check elevator doors (door bumpers feature)
					ElevatorDoor::DoorWrapper *wrapper = dynamic_cast<ElevatorDoor::DoorWrapper*>(obj->GetParent());

					if (wrapper)
					{
						ElevatorDoor* door = wrapper->parent;

						//make sure both internal and external doors are closing
						if (door->OpenDoor == -1 && door->GetWhichDoors() == 1)
						{
							//either open doors if the hit door was an internal door or a shaft door on the elevator floor
							if (wrapper->IsShaftDoor == false || (wrapper->IsShaftDoor == true && wrapper->floor == door->elev->GetFloor()))
								door->elev->OpenDoors(door->Number, 1);
						}
					}
				}
			}
		}
	}

	//if a model is attached, run it's loop
	if (AttachedModel)
		AttachedModel->Loop();

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
	if (!MainCamera)
		return;

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
	//calculate movement and rotation acceleration

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

void Camera::SetGravity(float gravity, bool save_value, bool camera_only)
{
	if (save_value == true)
		Gravity = gravity;

	if (EnableBullet == true && MainCamera)
	{
		if (camera_only == false)
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
	if (!MainCamera)
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

void Camera::SetFOVAngle(float angle)
{
	//set camera FOV angle

	if (!MainCamera)
		return;

	if (angle > 0 && angle < 179.63)
	{
		float ratio = MainCamera->getAspectRatio();
		MainCamera->setFOVy(Ogre::Degree(angle / ratio));
	}
}

float Camera::GetFOVAngle()
{
	if (!MainCamera)
		return 0.0f;

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

	if (!MainCamera)
		return;

	if (mode == 0)
		MainCamera->setPolygonMode(Ogre::PM_SOLID);
	if (mode == 1)
		MainCamera->setPolygonMode(Ogre::PM_WIREFRAME);
	if (mode == 2)
		MainCamera->setPolygonMode(Ogre::PM_POINTS);
}

void Camera::EnableCollisions(bool value)
{
	if (!MainCamera)
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
	if (!MainCamera)
		return false;

	if (EnableBullet == true)
		return mCharacter->onGround();
	else
		return false;
}

void Camera::Sync()
{
	//sync scene node with bullet object

	if (!MainCamera)
		return;

	if (EnableBullet == true)
		mCharacter->sync();

	//notify on movement or rotation
	Ogre::Vector3 position = sbs->ToRemote(GetPosition());
	Ogre::Quaternion orientation = GetOrientation();

	if (prev_position != position)
	{
		GetSceneNode()->Update();
		NotifyMove();
	}
	if (prev_orientation != orientation)
	{
		GetSceneNode()->Update();
		NotifyRotate();
	}

	prev_position = position;
	prev_orientation = orientation;
}

void Camera::SetMaxRenderDistance(float value)
{
	//set distance of camera's far clipping plane - set to 0 for infinite

	if (!MainCamera)
		return;

	MainCamera->setFarClipDistance(sbs->ToRemote(value));
	FarClip = value;
}

float Camera::GetMaxRenderDistance()
{
	return FarClip;
}

void Camera::ShowDebugShape(bool value)
{
	if (!MainCamera)
		return;

	if (EnableBullet == true)
		mCharacter->showDebugShape(value);
}

void Camera::MoveCharacter()
{
	if (!MainCamera)
		return;

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

	if (!MainCamera)
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
			Ogre::Vector3 pos = GetPosition();
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

	if (!MainCamera)
		return;

	if (value == true)
		SetFOVAngle(BinocularsFOV);
	else
		SetToDefaultFOV();
}

bool Camera::IsMeshVisible(MeshObject *mesh)
{
	//returns if a mesh object is visible in the camera's view frustum or not

	if (!mesh || !MainCamera)
		return false;

	return mesh->IsVisible(MainCamera);
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
	Ogre::Vector3 front, top;
	GetDirection(front, top);

	Ogre::Vector3 hit_position;
	MeshObject *mesh = 0;
	WallObject *wall = 0;
	bool hit = false;

	//do a raycast from the collider's position, in the forward direction
	Ogre::Vector3 position = GetPosition();

	for (int i = GetPosition().y; i > 0; i--)
	{
		position.y = i;
		Ogre::Ray ray (sbs->ToRemote(position), sbs->ToRemote(front, false));

		hit = sbs->HitBeam(ray, 2.0f, mesh, wall, hit_position);

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

	if (!MainCamera)
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

	if (!MainCamera)
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

bool Camera::Attach(Ogre::Camera *camera, bool init_state)
{
	if (camera->isAttached() == true)
		return false;

	MainCamera = camera;
	MainCamera->setNearClipDistance(0.1f);
	MainCamera->setPosition(0, sbs->ToRemote((cfg_body_height + cfg_legs_height + 0.5f) / 2), 0);
	GetSceneNode()->AttachObject(MainCamera);

	SetFOVAngle(FOV);
	SetMaxRenderDistance(FarClip);

	//move camera to start location
	if (FirstAttach == false)
	{
		SetGravity(sbs->GetConfigFloat("Skyscraper.SBS.Camera.Gravity", 32.1719f), true, false); // 9.806 m/s/s

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

	return true;
}

bool Camera::Detach()
{
	if (!MainCamera)
		return false;

	if (MainCamera->isAttached() == false)
		return false;

	GetSceneNode()->DetachObject(MainCamera);
	MainCamera = 0;

	return true;
}

void Camera::OnMove(bool parent)
{
	if (EnableBullet == true)
		mCharacter->updateTransform(true, false, false);
}

void Camera::OnRotate(bool parent)
{
	if (EnableBullet == true)
		mCharacter->updateTransform(false, true, false);
}

CameraState Camera::GetCameraState()
{
	//returns camera state info
	//the position value is in a global/absolute scene format, not engine-relative

	CameraState state;
	state.position = GetPosition() + sbs->GetPosition();
	state.rotation = GetRotation();
	state.floor = CurrentFloor;
	state.collisions = CollisionsEnabled();
	state.gravity = GetGravityStatus();
	state.freelook = Freelook;
	state.desired_velocity = desired_velocity;
	state.velocity = velocity;
	state.desired_angle_velocity = desired_angle_velocity;
	state.angle_velocity = angle_velocity;
	state.accum_movement = accum_movement;
	return state;
}

void Camera::SetCameraState(const CameraState &state, bool set_floor)
{
	//sets camera state
	//the position value is in a global/absolute scene format, not engine-relative

	Ogre::Vector3 position = state.position - sbs->GetPosition();
	if (set_floor == true)
		GotoFloor(state.floor, true);
	else
		GotoFloor(sbs->GetFloorNumber(state.position.y));
	SetPosition(position);
	SetRotation(state.rotation);
	EnableCollisions(state.collisions);
	EnableGravity(state.gravity);
	Freelook = state.freelook;
	desired_velocity = state.desired_velocity;
	velocity = state.velocity;
	desired_angle_velocity = state.desired_angle_velocity;
	angle_velocity = state.angle_velocity;
	accum_movement = state.accum_movement;
}

}
