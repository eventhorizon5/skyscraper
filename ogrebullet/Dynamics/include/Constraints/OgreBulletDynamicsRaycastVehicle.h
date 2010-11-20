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
#ifndef _OGREBULLETDYNAMICS_RaycastVehicle_H
#define _OGREBULLETDYNAMICS_RaycastVehicle_H

#include "OgreBulletDynamicsPreRequisites.h"

#include "OgreBulletDynamicsConstraint.h"


namespace OgreBulletDynamics
{

    // -------------------------------------------------------------------------
    // VehicleRayCaster  class
    class _OgreBulletExport VehicleRayCaster
    {
    public:
        VehicleRayCaster(DynamicsWorld *world);
        virtual ~VehicleRayCaster();
        
        btVehicleRaycaster *getBulletVehicleRayCaster()
        {return static_cast <btVehicleRaycaster*> (mBulletVehicleRayCaster);};
         
    private:
        btDefaultVehicleRaycaster *mBulletVehicleRayCaster;        
    };
    // -------------------------------------------------------------------------
    // VehicleTuning  class
    class _OgreBulletExport VehicleTuning
    {
    public:
        VehicleTuning(
            const Ogre::Real suspensionStiffness,
            const Ogre::Real suspensionCompression,
            const Ogre::Real suspensionDamping,
            const Ogre::Real maxSuspensionTravelCm,
            const Ogre::Real frictionSlip);
        virtual ~VehicleTuning();
        
        btRaycastVehicle::btVehicleTuning *getBulletTuning()
        {return mBulletTuning;};
        
    private:
        btRaycastVehicle::btVehicleTuning *mBulletTuning;        
    };
    // -------------------------------------------------------------------------
    // RaycastVehicle  class
    class _OgreBulletExport WheelInfo
    {
    public:
        WheelInfo(btWheelInfo &w): 
          mWheel(&w)
          {};
        virtual ~WheelInfo(){};

        btWheelInfo *getBulletWheelInfo(){return static_cast<btWheelInfo *> (mWheel);}
       
    protected:
        btWheelInfo                   *mWheel;
    };
    // -------------------------------------------------------------------------
    // RaycastVehicle  class
    class _OgreBulletExport RaycastVehicle : public ActionInterface
    {
    public:
        RaycastVehicle(WheeledRigidBody *body,  
            VehicleTuning        *vt,
            VehicleRayCaster     *caster = 0);

        virtual ~RaycastVehicle();

        btRaycastVehicle *getBulletVehicle()
        {return static_cast<btRaycastVehicle *> (mActionInterface);};
        void setCoordinateSystem(int rightIndex,int upIndex,int forwardIndex);

        void addWheel(
            Ogre::SceneNode *node,
            const Ogre::Vector3 &connectionPoint,
            const Ogre::Vector3 &wheelDirection,
            const Ogre::Vector3 &wheelAxle,
            const Ogre::Real suspensionRestLength,
            const Ogre::Real wheelRadius,
            const bool isFrontWheel,
            const Ogre::Real wheelFriction,
            const Ogre::Real rollInfluence);

        // when all wheels are attached, make vehicle aware of it
        void setWheelsAttached();
        // update wheels when needed.
        void setTransform();

        void applyEngineForce (float engineForce, int wheel);
        void setSteeringValue(float steering, int wheel);

     protected:
         VehicleTuning                   *mTuning;
         VehicleRayCaster                *mRayCaster;
    
         std::vector<btWheelInfo *>         mWheelsInfo;
         std::vector<Ogre::SceneNode *>      mWheelNodes;

         WheeledRigidBody* mChassisBody;
         Ogre::SceneNode *mNode;
         
    };
}
#endif //_OGREBULLETDYNAMICS_RaycastVehicle_H

