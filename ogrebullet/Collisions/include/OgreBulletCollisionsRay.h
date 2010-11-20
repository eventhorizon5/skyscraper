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

#ifndef _OGREBULLETCOLLISIONS_CollisionRay_H
#define _OGREBULLETCOLLISIONS_CollisionRay_H

#include "OgreBulletCollisionsPreRequisites.h"


namespace OgreBulletCollisions
{
    // -------------------------------------------------------------------------
    // basic CollisionRay
    class _OgreBulletExport CollisionRayResultCallback 
    {
	public:
		CollisionRayResultCallback(const Ogre::Ray &ray, CollisionsWorld *world, Ogre::Real max_distance, bool init = true);
	    virtual ~CollisionRayResultCallback();

        btCollisionWorld::RayResultCallback *getBulletRay() const {return mRayResultCallback;}

		bool doesCollide () const;

		void setRay(const Ogre::Ray &ray);
		void setWorld(CollisionsWorld *world);
		void setMaxDistance(Ogre::Real max_distance);

        inline const Ogre::Ray &getRay() const;
		Ogre::Vector3 getRayStartPoint() const;
		Ogre::Vector3 getRayEndPoint() const;

    protected:

        btCollisionWorld::RayResultCallback   *mRayResultCallback;
        CollisionsWorld                       *mWorld;
        Ogre::Ray                              mRay;
		Ogre::Real                             mMaxDistance;

    };
    // -------------------------------------------------------------------------
    // basic CollisionRay inline methods
    inline const Ogre::Ray &CollisionRayResultCallback ::getRay() const 
    {
        return mRay;
    }
    // -------------------------------------------------------------------------
    //  CollisionClosestRay
    class _OgreBulletExport CollisionClosestRayResultCallback : public CollisionRayResultCallback
    { 
	public:
		CollisionClosestRayResultCallback(const Ogre::Ray &ray, CollisionsWorld *world, Ogre::Real max_distance);
        virtual ~CollisionClosestRayResultCallback(){};

        Object *getCollidedObject() const;
		Ogre::Vector3 getCollisionPoint() const;
		Ogre::Vector3 getCollisionNormal() const;

        inline btCollisionWorld::ClosestRayResultCallback *getBulletClosestRayResultCallback() const 
        {
            return static_cast <btCollisionWorld::ClosestRayResultCallback * > (mRayResultCallback);
        }
    };
}
#endif //_OGREBULLETCOLLISIONS_CollisionRay_H

