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

#ifndef _OGREBULLETCOLLISIONS_CollisionWorld_H
#define _OGREBULLETCOLLISIONS_CollisionWorld_H

#include "OgreBulletCollisionsPreRequisites.h"


namespace OgreBulletCollisions
{
    // -------------------------------------------------------------------------
    // basic CollisionWorld
    class _OgreBulletExport CollisionsWorld 
    {
    public:
        CollisionsWorld(Ogre::SceneManager *scn, const Ogre::AxisAlignedBox &bounds, bool init = true);
	    virtual ~CollisionsWorld();

        void addObject(Object *obj, int filterGrp = 1, short int collisionFilter = -1);

		/// Returns false if obj was not found.
		bool removeObject(Object *obj);

        void discreteCollide();

        bool isObjectregistered(Object *) const;
        Object *findObject(Ogre::SceneNode *node) const;
        Object *findObject(btCollisionObject *object) const;


        // mShowDebugContactPoints getter
        bool getShowDebugContactPoints() const { return mShowDebugContactPoints; }
        // mShowDebugContactPoints setter
        void setShowDebugContactPoints(bool show);
        // mShowDebugShapes getter
        bool getShowDebugShapes() const { return mShowDebugShapes; }
        // mShowDebugShapes setter
        void setShowDebugShapes(bool val);

		DebugDrawer *getDebugDrawer(){return mDebugDrawer;};
		void setDebugDrawer(DebugDrawer *debugdrawer);

		DebugLines *getDebugContactPoints(){return mDebugContactPoints;};
		void setDebugContactPoints(DebugLines *debugcontacts);


        Ogre::SceneManager *getSceneManager() const {return mScnMgr;}
        btCollisionWorld *getBulletCollisionWorld() const {return mWorld;}

        void launchRay (CollisionRayResultCallback &ray, short int collisionFilterMask = -1);

    protected:
        btCollisionWorld*          mWorld;
        btCollisionDispatcher*     mDispatcher;

		btBroadphaseInterface*	   mBroadphase;

        Ogre::AxisAlignedBox       mBounds;

		btDefaultCollisionConfiguration	mDefaultCollisionConfiguration;

        std::deque<Object *>        mObjects;

        bool                        mShowDebugShapes;
        bool                        mShowDebugContactPoints;
        DebugLines *                mDebugContactPoints;

        Ogre::SceneManager *        mScnMgr;
		OgreBulletCollisions::DebugDrawer *mDebugDrawer;
    };
}
#endif //_OGREBULLETCOLLISIONS_CollisionWorld_H

