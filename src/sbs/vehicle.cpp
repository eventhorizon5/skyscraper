/* $Id: vehicle.cpp 5263 2018-01-12 00:59:01Z ryan $ */

/*
	Scalable Building Simulator - Vehicle Object
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

/***************************************************************************

This source file is part of OGREBULLET
(Object-oriented Graphics Rendering Engine Bullet Wrapper)
For the latest info, see http://www.ogre3d.org/phpBB2addons/viewforum.php?f=10

Copyright (c) 2007 tuan.kuranes@gmail.com (Use it Freely, even Statically, but have to contribute any changes)



This source file is not LGPL, it's public source code that you can reuse.
-----------------------------------------------------------------------------*/

#include <OgreBulletDynamicsRigidBody.h>
#include <OgreBulletDynamicsRaycastVehicle.h>
#include <OgreBulletCollisionsBoxShape.h>
#include <OgreBulletCollisionsCompoundShape.h>
#include "globals.h"
#include "sbs.h"
#include "scenenode.h"
#include "manager.h"
#include "mesh.h"
#include "camera.h"
#include "vehicle.h"

#undef EPSILON
#define EPSILON 0.001f

namespace SBS {

Vehicle::Vehicle(Object *parent, const int number) : Object(parent)
{
	//set up SBS object
	SetValues("Vehicle", "", false);
	Number = number;
	Enabled(true);

	//initialize main values
	mCarChassis = 0;
	mTuning = 0;
	mVehicleRayCaster = 0;
	mVehicle = 0;
	mChassis = 0;
	WheelCount = 0;
	mEngineForce = 0;
	mSteering = 0;
	mSteeringLeft = false;
	mSteeringRight = false;
	ChassisShift = Ogre::Vector3::ZERO;
	ChassisScale = 1;
	WheelScale = 1;
	vehicle = 0;
	CameraPoint = Ogre::Vector3::ZERO;
	camera_attached = false;
	Created = false;

	//initialize car tunables
	MaxEngineForce = 3000.f;
	SteeringIncrement = 0.04f;
	SteeringClamp = 0.8f;
	WheelFriction = 1e30f;//1000;//1e30f;
	SuspensionStiffness = 20.f;
	SuspensionDamping = 2.3f;
	SuspensionCompression = 4.4f;
	RollInfluence = 0.1f;//1.0f;
	SuspensionRestLength = 0.6;
	MaxSuspensionTravelCm = 500.0;
	FrictionSlip = 10.5;

	//set name
	std::string name = "Vehicle " + ToString(Number);
	SetName(name);
}

Vehicle::~Vehicle()
{
	//detach camera
	AttachCamera(false);

	if (mChassis)
		delete mChassis;
	mChassis = 0;

	for (size_t i = 0; i < mWheels.size(); i++)
	{
		delete mWheels[i];
	}
	mWheels.clear();

	if (mCarChassis)
	{
		mCarChassis->removeFromWorld();
		delete mCarChassis;
	}
	mCarChassis = 0;

	if (mTuning)
		delete mTuning;
	mTuning = 0;

	if (mVehicleRayCaster)
		delete mVehicleRayCaster;
	mVehicleRayCaster = 0;

	if (mVehicle)
		delete mVehicle;
	mVehicle = 0;

	if (vehicle)
	{
		vehicle->parent_deleting = true;
		delete vehicle;
	}
	vehicle = 0;

	//unregister from parent
	if (sbs->FastDelete == false && parent_deleting == false)
		sbs->GetVehicleManager()->Remove(this);
}

bool Vehicle::Create(const Ogre::Vector3 &position)
{
	if (Created == true)
		return ReportError("Vehicle already created");

	if (!vehicle)
		return ReportError("Chassis not created yet");

	Move(position);

	Created = true;

	return true;
}

bool Vehicle::CreateChassis(Real restitution, Real friction, Real mass, Real linear_dampening, Real angular_dampening)
{
	if (vehicle)
		return ReportError("Chassis already created");

	vehicle = new Object(this);
	vehicle->SetValues("SceneNode", "Vehicle Chassis", true, true);

	mChassis = new MeshObject(vehicle, GetName() + " Chassis", 0, ChassisMesh, 0, ChassisScale, false, 0, 0, 0, false);
	mChassis->SetPosition(ChassisShift);

	//initialize collider shape
	Real scale = mChassis->GetSceneNode()->GetScale();
	mChassis->GetBounds();

	if (mChassis->Bounds->isNull() == true)
		return false;

	Ogre::Vector3 bounds = mChassis->Bounds->getHalfSize() * scale;

	OgreBulletCollisions::BoxCollisionShape* chassisShape = new OgreBulletCollisions::BoxCollisionShape(bounds);
	OgreBulletCollisions::CompoundCollisionShape* compound = new OgreBulletCollisions::CompoundCollisionShape();
	compound->addChildShape(chassisShape, sbs->ToRemote(ChassisShift), Ogre::Quaternion::IDENTITY);

	mCarChassis = new OgreBulletDynamics::WheeledRigidBody(GetName() + " Chassis", sbs->mWorld);

	mCarChassis->setShape(vehicle->GetSceneNode()->GetRawSceneNode(), compound, restitution, friction, mass, true);
	mCarChassis->setDamping(linear_dampening, angular_dampening);

	mCarChassis->disableDeactivation();
	mTuning = new OgreBulletDynamics::VehicleTuning(SuspensionStiffness,
										SuspensionCompression,
										SuspensionDamping,
										MaxSuspensionTravelCm,
										FrictionSlip);

	mVehicleRayCaster = new OgreBulletDynamics::VehicleRayCaster(sbs->mWorld);
	mVehicle = new OgreBulletDynamics::RaycastVehicle(mCarChassis, mTuning, mVehicleRayCaster);

	int rightIndex = 0;
	int upIndex = 1;
	int forwardIndex = 2;

	mVehicle->setCoordinateSystem(rightIndex, upIndex, forwardIndex);

	return true;
}

bool Vehicle::AddWheel(bool engine, bool steerable, bool IsFrontWheel, Real radius, const Ogre::Vector3 &ConnectionPoint, const Ogre::Vector3 &Direction, const Ogre::Vector3 &Axle)
{
	if (!mVehicle)
		return ReportError("Chassis not created yet");

	if (engine == true)
		mWheelsEngine.push_back(WheelCount);
	if (steerable == true)
		mWheelsSteerable.push_back(WheelCount);
	WheelCount++;

	MeshObject *mesh = new MeshObject(this, GetName() + " Wheel " + ToString(WheelCount), 0, WheelMesh, 0, WheelScale, false, 0, 0, 0, false);
	mWheels.push_back(mesh);

	mVehicle->addWheel(
		mesh->GetSceneNode()->GetRawSceneNode(),
		sbs->ToRemote(ConnectionPoint),
		Direction,
		Axle,
		SuspensionRestLength,
		sbs->ToRemote(radius),
		IsFrontWheel, WheelFriction, RollInfluence);

	//mVehicle->setWheelsAttached();

	return true;
}

void Vehicle::KeyPressed(bool left, bool right, bool down, bool up)
{
	if (left == true)
		mSteeringLeft = true;
	if (right == true)
		mSteeringRight = true;
	if (down == true)
		mEngineForce = -MaxEngineForce;
	if (up == true)
		mEngineForce = MaxEngineForce;
}

void Vehicle::KeyReleased(bool left, bool right, bool down, bool up)
{
	if (left == true)
		mSteeringLeft = false;
	if (right == true)
		mSteeringRight = false;
	if (down == true || up == true)
		mEngineForce = 0;
}

void Vehicle::Loop()
{
	if (!mVehicle)
		return;

	// apply engine force on relevant wheels
	for (size_t i = 0; i < mWheelsEngine.size(); i++)
	{
		mVehicle->applyEngineForce(mEngineForce, mWheelsEngine[i]);
	}

	if (mSteeringLeft)
	{
		mSteering += SteeringIncrement;
		if (mSteering > SteeringClamp)
			mSteering = SteeringClamp;
	}
	else if (mSteeringRight)
	{
		mSteering -= SteeringIncrement;
		if (mSteering < -SteeringClamp)
			mSteering = -SteeringClamp;
	}
	else if (mSteering != 0)
	{
		if (mSteering < 0)
			mSteering += SteeringIncrement;
		else
			mSteering -= SteeringIncrement;
		if (std::abs(mSteering) < EPSILON)
			mSteering = 0;
	}

	// apply steering on relevant wheels
	for (size_t i = 0; i < mWheelsSteerable.size(); i++)
	{
		if (i < 2)
			mVehicle->setSteeringValue(mSteering, mWheelsSteerable[i]);
		else
			mVehicle->setSteeringValue(-mSteering, mWheelsSteerable[i]);
	}

	//if camera is attached, sync it's position to this vehicle
	if (camera_attached == true)
	{
		sbs->camera->SetPosition(GetPosition() + CameraPoint);
		sbs->camera->SetOrientation(GetOrientation());
	}
}

void Vehicle::Enabled(bool value)
{
	EnableLoop(value);
}

void Vehicle::OnMove(bool parent)
{
	if (mCarChassis)
		mCarChassis->updateTransform(true, true, true);
}

void Vehicle::AttachCamera(bool value)
{
	if (camera_attached == value)
		return;

	CameraPoint = Ogre::Vector3(0, 5, 0);
	camera_attached = value;

	if (value == true)
		Report("Camera attached");
	else
		Report("Camera detached");
}

Real Vehicle::GetWidth()
{
	if (!mChassis)
		return 0.0;

	Ogre::Vector3 size = mChassis->Bounds->getSize();
	if (size.x > size.z)
		return size.z;
	else
		return size.x;
}

void Vehicle::Report(const std::string &message)
{
	//general reporting function
	Object::Report(GetName() + ": " + message);
}

bool Vehicle::ReportError(const std::string &message)
{
	//general reporting function
	return Object::ReportError(GetName() + ": " + message);
}

}
