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

#include <limits.h>

#include "OgreBulletCollisions.h"

#include "Debug/OgreBulletCollisionsDebugDrawer.h"

#include "OgreBulletCollisionsWorld.h"
#include "Utils/OgreBulletConverter.h"

#include "OgreBulletCollisionsObject.h"
#include "Debug/OgreBulletCollisionsDebugShape.h"
#include "OgreBulletCollisionsRay.h"



using namespace Ogre;
using namespace OgreBulletCollisions;

namespace OgreBulletCollisions
{
	// -------------------------------------------------------------------------
	CollisionsWorld::CollisionsWorld(SceneManager *scn, const AxisAlignedBox &bounds, bool init):
mScnMgr(scn),
mBounds(bounds),
mShowDebugShapes(false),
mShowDebugContactPoints(false),
mDebugContactPoints(0),
mDebugDrawer(0)
{
	mDispatcher = new btCollisionDispatcher(&mDefaultCollisionConfiguration);
	mBroadphase = new btDbvtBroadphase();

	// if not called by a inherited class
	if (init)
	{
		mWorld = new btCollisionWorld(mDispatcher, mBroadphase, &mDefaultCollisionConfiguration);

		btCollisionDispatcher * dispatcher = static_cast<btCollisionDispatcher *>(mWorld->getDispatcher());
		btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);
	}
}
// -------------------------------------------------------------------------
void  CollisionsWorld::setDebugDrawer(DebugDrawer *debugdrawer)
{
	mDebugDrawer = debugdrawer;
	(static_cast <btCollisionWorld *> (mWorld))->setDebugDrawer(mDebugDrawer);
};
// -------------------------------------------------------------------------
void  CollisionsWorld::setDebugContactPoints(DebugLines *debugcontact)
{
	mDebugContactPoints = debugcontact;
};

// -------------------------------------------------------------------------
CollisionsWorld::~CollisionsWorld()
{
	delete mWorld;
	delete mBroadphase;
	delete mDispatcher;
}
// -------------------------------------------------------------------------
void CollisionsWorld::setShowDebugContactPoints(bool show)
{
	if (show && !mShowDebugContactPoints)
	{
		assert (mDebugContactPoints == 0);
		mDebugContactPoints = new DebugLines();
		getSceneManager()->getRootSceneNode ()->createChildSceneNode ("DebugContactPoints")->attachObject (mDebugContactPoints);
		mShowDebugContactPoints = true;
		return;
	}
	if (!show && mShowDebugContactPoints)
	{
		assert (mDebugContactPoints != 0);
		Ogre::SceneNode * n = (Ogre::SceneNode *)getSceneManager()->getRootSceneNode ()->getChild ("DebugContactPoints");
		n->detachObject (mDebugContactPoints);
		n->getParentSceneNode()->removeAndDestroyChild("DebugContactPoints");
		delete mDebugContactPoints;
        mDebugContactPoints = 0;
		mShowDebugContactPoints = false;
		return;
	}
}
// -------------------------------------------------------------------------
void CollisionsWorld::setShowDebugShapes(bool show)
{
	if (show && !mShowDebugShapes)
	{
		//assert (mDebugShapes == 0);
		std::deque<Object*>::iterator it = mObjects.begin();
		while (it != mObjects.end())
		{
			(*it)->showDebugShape(show);
			++it;
		}
		mShowDebugShapes = true;
		return;
	}
	if (!show && mShowDebugShapes)
	{
		//assert (mDebugShapes != 0);
		std::deque<Object*>::iterator it = mObjects.begin();
		while (it != mObjects.end())
		{
			(*it)->showDebugShape(show);
			++it;
		}
		mShowDebugShapes = false;
		return;
	}
}
// -------------------------------------------------------------------------
void CollisionsWorld::addObject(Object *obj, int filterGrp, short int collisionFilter)
{
	mObjects.push_back (obj);
	mWorld->addCollisionObject(obj->getBulletObject(), filterGrp, collisionFilter);
}
//------------------------------------------------------------------------- 
bool CollisionsWorld::removeObject(Object *obj)
{
	std::deque<Object*>::iterator it = find(mObjects.begin(),mObjects.end(), obj);
	if (it == mObjects.end())
		return false;
	mObjects.erase(it);
	return true;
}
// -------------------------------------------------------------------------
bool CollisionsWorld::isObjectregistered(Object *obj) const
{
	std::deque<Object *>::const_iterator itRes = std::find(mObjects.begin(), mObjects.end(), obj);
	if (itRes != mObjects.end())
		return true;
	return false;
}
// -------------------------------------------------------------------------
Object *CollisionsWorld::findObject(const btCollisionObject *object) const
{
	std::deque<Object *>::const_iterator it = mObjects.begin();
	while (it != mObjects.end())
	{
		if ((*it)->getBulletObject() == object)
			return (*it);
		++it;
	}
	return 0;
}
// -------------------------------------------------------------------------
Object *CollisionsWorld::findObject(SceneNode *node) const
{
	std::deque<Object *>::const_iterator it = mObjects.begin();
	while (it != mObjects.end())
	{
		//if ((*it)->getParentNode() == node)
		if((*it)->getRootNode() == node)
			return (*it);
		++it;
	}
	return 0;
}
// -------------------------------------------------------------------------
void CollisionsWorld::discreteCollide()
{
	if (mDebugDrawer) 
		mDebugDrawer->clear ();
	if (mDebugContactPoints) 
		mDebugContactPoints->clear ();


	mWorld->performDiscreteCollisionDetection();

	///one way to draw all the contact points is iterating over contact manifolds / points:
	const unsigned int  numManifolds = mWorld->getDispatcher()->getNumManifolds();
	for (unsigned int i=0;i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = mWorld->getDispatcher()->getManifoldByIndexInternal(i);

		const btCollisionObject* obA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
		const btCollisionObject* obB = static_cast<const btCollisionObject*>(contactManifold->getBody1());

		contactManifold->refreshContactPoints(obA->getWorldTransform(),obB->getWorldTransform());

		const unsigned int numContacts = contactManifold->getNumContacts();
		for (unsigned int j = 0;j < numContacts; j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);

			if (mShowDebugContactPoints)
			{
				btVector3 ptA = pt.getPositionWorldOnA();
				btVector3 ptB = pt.getPositionWorldOnB();
				btVector3 ptDistB = ptB  + pt.m_normalWorldOnB *100;

				mDebugContactPoints->addLine(ptA.x(),ptA.y(),ptA.z(),
					ptB.x(),ptB.y(),ptB.z());
				mDebugContactPoints->addLine(ptB.x(),ptB.y(),ptB.z(),
					ptDistB.x(),ptDistB.y(),ptDistB.z());
			}
		}
		//you can un-comment out this line, and then all points are removed
		//contactManifold->clearManifold();	
	}
	if (mDebugContactPoints) 
		mDebugContactPoints->draw();


	/*
	if (mShowDebugShapes)
	{
	std::deque<Object*>::iterator it = mObjects.begin();
	while (it != mObjects.end())
	{

	//(*it)->getBulletObject()->getWorldTransform().getOpenGLMatrix( m );
	mShowDebugDrawShapes->
	//GL_ShapeDrawer::drawOpenGL(m,objects[i].getCollisionShape(),btVector3(1,1,1),getDebugMode());

	++it;
	}
	}
	*/

	if (mDebugDrawer) 
	{
		// draw lines that step Simulation sent.
		//mDebugDrawer->draw();

		const bool drawFeaturesText = (mDebugDrawer->getDebugMode () & btIDebugDraw::DBG_DrawFeaturesText) != 0;
		if (drawFeaturesText)
		{
			// on all bodies we have
			// we get all shapes and draw more information
			//depending on mDebugDrawer mode.
			std::deque<Object*>::iterator it = mObjects.begin();
			while (it != mObjects.end())
			{
				//(*it)->drawFeaturesText();
				++it;
			}
		}
	}
	if (mDebugContactPoints) 
	{
		// draw lines that step Simulation sent.
		mDebugContactPoints->draw();
	}
}

// -------------------------------------------------------------------------
void CollisionsWorld::launchRay(CollisionRayResultCallback &rayresult, short int collisionFilterMask)
{
	mWorld->rayTest(
		OgreBtConverter::to(rayresult.getRayStartPoint()), 
		OgreBtConverter::to(rayresult.getRayEndPoint()), 
		*rayresult.getBulletRay ()
		//, collisionFilterMask
		);
}
}

