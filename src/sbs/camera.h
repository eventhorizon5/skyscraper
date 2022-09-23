/* $Id$ */

/*
	Scalable Building Simulator - Camera Object
	The Skyscraper Project - Version 1.11 Alpha
	Copyright (C)2004-2018 Ryan Thoryk
	http://www.skyscrapersim.com
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@skyscrapersim.com

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

namespace SBS {

struct SBSIMPEXP CameraState
{
	Ogre::Vector3 position;
	Ogre::Vector3 rotation;
	int floor;
	bool collisions;
	bool gravity;
	bool freelook;
	Ogre::Vector3 desired_velocity;
	Ogre::Vector3 velocity;
	Ogre::Vector3 desired_angle_velocity;
	Ogre::Vector3 angle_velocity;
	Ogre::Vector3 accum_movement;
	Real fov;
};

class SBSIMPEXP Camera : public Object
{
public:
	int CurrentFloor; //floor camera's on
	std::string CurrentFloorID; //indicator ID of camera's current floor
	int StartFloor; //Starting floor
	Real StartPositionX; //starting position on X axis
	Real StartPositionZ; //starting position on Z axis
	Real cfg_jumpspeed; //Initial speed of jumping.
	Real cfg_walk_accelerate; //walk acceleration
	Real cfg_walk_maxspeed; //walk maximum speed
	Real cfg_walk_maxspeed_mult; //multiplier for max speed
	Real cfg_walk_maxspeed_multreal; //is multiplier used?
	Real cfg_walk_brake; //walk brake deceleration
	Real cfg_rotate_accelerate; //rotate acceleration
	Real cfg_rotate_maxspeed; //rotate maximum speed
	Real cfg_rotate_brake; //rotate brake deceleration
	Real cfg_body_height; //body height
	Real cfg_body_width; //body width
	Real cfg_legs_height; //legs height
	Real cfg_legs_width; //legs width
	Real cfg_lookspeed; //base look speed
	Real cfg_turnspeed; //base turn speed
	Real cfg_spinspeed; //base spin speed
	Real cfg_floatspeed; //base float speed
	Real cfg_stepspeed; //base step speed
	Real cfg_strafespeed; //base strafe speed
	Real cfg_speed; //camera speed multiplier
	Real cfg_speedfast; //camera fast speed multiplier
	Real cfg_speedslow; //camera slow speed multiplier
	Real cfg_zoomspeed; //camera zoom speed
	Ogre::Vector3 desired_velocity;
	Ogre::Vector3 velocity;
	Ogre::Vector3 desired_angle_velocity;
	Ogre::Vector3 angle_velocity;
	Real speed; //movement speed base
	bool MouseLeftDown; //mouse left button status
	bool MouseRightDown; //mouse right button status
	bool ReportCollisions; //if true, print collisions on console
	std::string LastHitMesh; //name of last hit mesh
	int LastHitMeshNumber; //object number of last hit mesh
	bool Freelook; //freelook (mouselook) is enabled/disabled
	Real Freelook_speed; //freelook speed
	Ogre::Vector3 HitPosition; //last hit position
	bool EnableBullet;
	Real BinocularsFOV; //binoculars mode FOV
	bool FirstAttach;
	bool inside_vehicle; //is user inside a vehicle?
	Vehicle *vehicle; //vehicle object, if user is inside

	//mouse coordinates
	int mouse_x, mouse_y;

	//functions
	Camera(Object *parent);
	~Camera();
	void SetPosition(const Ogre::Vector3 &position);
	void SetDirection(const Ogre::Vector3 &direction);
	void SetRotation(const Ogre::Vector3 &rotation);
	Ogre::Vector3 GetPosition(bool relative = false);
	void GetDirection(Ogre::Vector3 &front, Ogre::Vector3 &top, bool global = false);
	Ogre::Vector3 GetRotation();
	void UpdateCameraFloor();
	bool Move(Ogre::Vector3 vector, Real speed = 1.0, bool flip = true);
	bool MovePosition(Ogre::Vector3 vector, Real speed = 1.0);
	void Rotate(const Ogre::Vector3 &position, Real speed = 1.0);
	void RotateLocal(const Ogre::Vector3 &position, Real speed = 1.0);
	void SetStartDirection(const Ogre::Vector3 &direction);
	Ogre::Vector3 GetStartDirection();
	void SetStartRotation(const Ogre::Vector3 &rotation);
	Ogre::Vector3 GetStartRotation();
	void SetToStartPosition(bool disable_current_floor);
	void SetToStartDirection();
	void SetToStartRotation();
	void CheckElevator();
	void CheckShaft();
	void CheckStairwell();
	void ClickedObject(bool shift, bool ctrl, bool alt, bool right);
	void UnclickedObject();
	Object* GetMeshParent(Object *object);
	std::string GetClickedMeshName();
	std::string GetClickedWallName();
	int GetClickedObjectNumber();
	int GetClickedObjectLine();
	std::string GetClickedObjectCommand();
	std::string GetClickedObjectCommandP();
	void Loop();
	void Strafe(Real speed = 1.0);
	void Step(Real speed = 1.0);
	void Float(Real speed = 1.0);
	void Jump();
	void Look(Real speed = 1.0);
	void Turn(Real speed = 1.0);
	void Spin(Real speed = 1.0);
	void InterpolateMovement(Real delta);
	void SetGravity(Real gravity, bool save_value = true, bool camera_only = true);
	Real GetGravity();
	void EnableGravity(bool value);
	bool GetGravityStatus();
	void SetFOVAngle(Real angle);
	Real GetFOVAngle();
	void SetToDefaultFOV();
	Real GetHeight();
	void SetViewMode(int mode);
	void EnableCollisions(bool value);
	bool CollisionsEnabled();
	bool IsOnGround();
	void Sync();
	void SetMaxRenderDistance(Real value);
	Real GetMaxRenderDistance();
	void ShowDebugShape(bool value);
	void MoveCharacter();
	void ResetCollisions();
	void GotoFloor(int floor, bool disable_current = true);
	void Binoculars(bool value);
	bool IsMeshVisible(MeshObject *mesh);
	bool IsDynamicMeshVisible(DynamicMesh *mesh, int mesh_index);
	void AttachModel(Model *model);
	void DetachModel();
	bool PickUpModel();
	void DropModel();
	bool IsModelAttached();
	void ResetState();
	void ResetView();
	bool IsActive() { return (MainCamera != 0); }
	void Refresh();
	bool Attach(Ogre::Camera *camera, bool init_state = true);
	bool Detach();
	void OnMove(bool parent);
	void OnRotate(bool parent);
	CameraState GetCameraState();
	void SetCameraState(const CameraState &state, bool set_floor = true);
	void RevertMovement();
	void FreelookMove(const Ogre::Vector3 &rotation);
	bool MouseDown();
	void CheckObjects();
	void Teleport(Real X, Real Y, Real Z);
	void Drive(bool left, bool right, bool down, bool up, bool key_down);
	void Crouch(bool value);
	void SetOrientation(const Ogre::Quaternion &orientation);
	void AttachToVehicle(bool value);

private:
	Ogre::Camera* MainCamera; //main first-person view camera
	Ogre::Vector3 StartDirection; //direction camera faces on start
	Ogre::Vector3 StartRotation; //camera's startup rotation
	std::string meshname; //last clicked mesh name
	std::string wallname; //last clicked wall name
	int object_number; //last clicked object number
	int object_line; //script line number of last clicked object
	std::string object_cmd; //original script command of last clicked object
	std::string object_cmd_processed; //processed script command of last clicked object
	int FloorTemp; //previous floor check value
	Real Gravity;
	bool GravityStatus;
	int lastfloor;
	bool lastfloorset;
	Real FOV; //default FOV angle
	Ogre::Vector3 Rotation;
	bool Collisions; //collision detection status
	bool RotationStopped;
	bool MovementStopped;
	Real FarClip;
	Ogre::Vector3 accum_movement;
	Ogre::Vector3 prev_accum_movement;
	bool collision_reset;
	bool use_startdirection;
	Ogre::Quaternion prev_orientation;
	Ogre::Vector3 prev_position;

	//Models
	Model*	AttachedModel;

	//collision/physics
	OgreBulletDynamics::CharacterController* mCharacter;
	OgreBulletCollisions::CollisionShape* mShape;

	//used for restoring values when exiting vehicle
	Ogre::Quaternion old_camera_orientation;
	Ogre::Quaternion old_character_orientation;
	bool old_freelook_mode;
};

}

#endif
