/***************************************************************************

This source file is part of OGREBULLET
(Object-oriented Graphics Rendering Engine Bullet Wrapper)
For the latest info, see http://www.ogre3d.org/phpBB2addons/viewforum.php?f=10

Copyright (c) 2007 tuan.kuranes@gmail.com (Use it Freely, even Statically, but have to contribute any changes)



Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#include "OgreBulletDynamics.h"

#include "OgreBulletDynamicsWorld.h"
#include "OgreBulletDynamicsRigidBody.h"
#include "OgreBulletDynamicsConstraint.h"
#include "Constraints/OgreBulletDynamicsRaycastVehicle.h"

using namespace Ogre;

namespace OgreBulletDynamics
{
    // -------------------------------------------------------------------------
    VehicleRayCaster::VehicleRayCaster(DynamicsWorld *world)
    {
        mBulletVehicleRayCaster = new btDefaultVehicleRaycaster(world->getBulletDynamicsWorld ());
    }
    // -------------------------------------------------------------------------
    VehicleRayCaster::~VehicleRayCaster()
    {
        if(mBulletVehicleRayCaster != 0)
        {
            delete mBulletVehicleRayCaster;
            mBulletVehicleRayCaster = 0;
        }
    }
    // -------------------------------------------------------------------------
    VehicleTuning::VehicleTuning(const Ogre::Real suspensionStiffness,
				const Ogre::Real suspensionCompression,
				const Ogre::Real suspensionDamping,
				const Ogre::Real maxSuspensionTravelCm,
				const Ogre::Real frictionSlip)
    {
        mBulletTuning = new btRaycastVehicle::btVehicleTuning();

        mBulletTuning->m_suspensionStiffness =  suspensionStiffness;
        mBulletTuning->m_suspensionCompression = suspensionCompression;
        mBulletTuning->m_suspensionDamping = suspensionDamping;
        mBulletTuning->m_maxSuspensionTravelCm = maxSuspensionTravelCm;
        mBulletTuning->m_frictionSlip = frictionSlip;
    }
    // -------------------------------------------------------------------------
    VehicleTuning::~VehicleTuning()
    {
        if(mBulletTuning != 0)
        {
            delete mBulletTuning;
            mBulletTuning = 0;
        }
    }
    // -------------------------------------------------------------------------
    RaycastVehicle::RaycastVehicle(WheeledRigidBody *body,  
                                VehicleTuning *vt,
                                VehicleRayCaster *caster) :
        ActionInterface(body),
        mTuning(vt),
        mRayCaster(caster)
    {
        if (mRayCaster == 0)
            mRayCaster = new VehicleRayCaster(mWorld);

        btRaycastVehicle *v = new btRaycastVehicle(
                *(mTuning->getBulletTuning()),
                body->getBulletRigidBody (), 
                mRayCaster->getBulletVehicleRayCaster()
                );
        mActionInterface = v;
        mWorld->addVehicle(this);
        body->setVehicle (this);
    }
    // -------------------------------------------------------------------------
    RaycastVehicle::~RaycastVehicle()
    {
    }
    // -------------------------------------------------------------------------
    void RaycastVehicle::setCoordinateSystem(int rightIndex,int upIndex,int forwardIndex)
    {
        getBulletVehicle()->setCoordinateSystem(rightIndex, upIndex, forwardIndex);
    }
    // -------------------------------------------------------------------------
    void RaycastVehicle::addWheel(SceneNode *node,
                                  const Ogre::Vector3 &connectionPoint,
                                  const Ogre::Vector3 &wheelDirection,
                                  const Ogre::Vector3 &wheelAxle,

                                  const Ogre::Real suspensionRestLength,
                                  const Ogre::Real wheelRadius,
                                  const bool isFrontWheel,
                                  
                                  const Ogre::Real wheelFriction,
                                  const Ogre::Real rollInfluence)
    {
        btRaycastVehicle *v = static_cast<btRaycastVehicle *> (mActionInterface);

        mWheelsInfo.push_back (
            &v->addWheel(OgreBulletCollisions::OgreBtConverter::to(connectionPoint),
                        OgreBulletCollisions::OgreBtConverter::to(wheelDirection),
                        OgreBulletCollisions::OgreBtConverter::to(wheelAxle),
                        suspensionRestLength,
                        wheelRadius, 
                        *(mTuning->getBulletTuning ()),
                        isFrontWheel));

        {
            const size_t wheelCurrent = mWheelsInfo.size() - 1;
            mWheelsInfo[wheelCurrent]->m_suspensionStiffness = mTuning->getBulletTuning()->m_suspensionStiffness;
            mWheelsInfo[wheelCurrent]->m_wheelsDampingRelaxation = mTuning->getBulletTuning()->m_suspensionDamping;
            mWheelsInfo[wheelCurrent]->m_wheelsDampingCompression = mTuning->getBulletTuning()->m_suspensionCompression;

            mWheelsInfo[wheelCurrent]->m_frictionSlip = wheelFriction;
            mWheelsInfo[wheelCurrent]->m_rollInfluence = rollInfluence;
        }

        // create wheel scene Node
        {
            node->setPosition (connectionPoint);

            mWheelNodes.push_back ( node);
            //const size_t wheelCurrent = mWheelsInfo.size() - 1;

            //mWheelsInfo[wheelCurrent]->
        }
    }
    // -------------------------------------------------------------------------
    void RaycastVehicle::setWheelsAttached()
    {
        btRaycastVehicle *v = static_cast<btRaycastVehicle *> (mActionInterface);
        for (int i=0; i < v->getNumWheels(); i++)
        {
            btWheelInfo& wheel = v->getWheelInfo(i);

            wheel.m_suspensionStiffness = mTuning->getBulletTuning()->m_suspensionStiffness;
            wheel.m_wheelsDampingRelaxation = mTuning->getBulletTuning()->m_suspensionDamping;
            wheel.m_wheelsDampingCompression = mTuning->getBulletTuning()->m_suspensionCompression;

            //wheel.m_frictionSlip = mBulletTuning->getBulletTuning()->m_wheelFriction;
            //wheel.m_rollInfluence = mBulletTuning->getBulletTuning()->m_rollInfluence;
        }
    }
    // -------------------------------------------------------------------------
    void RaycastVehicle::setTransform()
    {
        //should update wheels as well ?
        for (int i=0; i < getBulletVehicle()->getNumWheels(); i++)
        {
            //synchronize the wheels with the (interpolated) chassis world transform
            getBulletVehicle()->updateWheelTransform(i, true);
            //draw wheels (cylinders)
            const btTransform &w = getBulletVehicle()->getWheelInfo(i).m_worldTransform;

            mWheelNodes[i]->setPosition (w.getOrigin()[0], w.getOrigin()[1], w.getOrigin()[2]);
            mWheelNodes[i]->setOrientation (w.getRotation().getW(), w.getRotation().getX(), w.getRotation().getY(), w.getRotation().getZ());

        }
    }
    // -------------------------------------------------------------------------
    void RaycastVehicle::applyEngineForce (float engineForce, int wheel)
    {
        getBulletVehicle()->applyEngineForce (engineForce, wheel);
    }
    // -------------------------------------------------------------------------
    void RaycastVehicle::setSteeringValue(float steering, int wheel)
    {
        getBulletVehicle()->setSteeringValue (steering, wheel);
    }
    // -------------------------------------------------------------------------
}
