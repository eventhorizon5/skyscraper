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

#include "OgreBulletCollisionsShape.h"
#include "OgreBulletCollisionsObject.h"
#include "OgreBulletCollisionsWorld.h"
#include "OgreBulletCollisionsObjectState.h"

#include "OgreBulletDynamicsWorld.h"
#include "OgreBulletDynamicsCharacter.h"
#include "OgreBulletDynamicsObjectState.h"

using namespace Ogre;
using namespace OgreBulletCollisions;

namespace OgreBulletDynamics
{

    // -------------------------------------------------------------------------
    CharacterController::CharacterController(const String &name, DynamicsWorld *world, Ogre::SceneNode *node, float width, float height, float stepHeight)
        :	
        Object(name, world, false)
    {
		in_world = false;
		//mState = new ObjectState(this);
		mState = 0;
		mRootNode = node;
		mShapeNode = mRootNode->createChildSceneNode(mName + "Node");
		btTransform startTransform;
		startTransform.setIdentity ();
		startTransform.setOrigin (btVector3(node->getPosition().x,node->getPosition().y,node->getPosition().z));
		showDebugShape(mWorld->getShowDebugShapes());
		btPairCachingGhostObject* ghost = new btPairCachingGhostObject();
		mObject = ghost;
		mObject->setWorldTransform(startTransform);
		getDynamicsWorld()->getBulletDynamicsWorld()->getPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
		btConvexShape* capsule = new btCapsuleShape(width, height);
		mObject->setCollisionShape (capsule);
		mObject->setCollisionFlags (btCollisionObject::CF_CHARACTER_OBJECT);
		mObject->setContactProcessingThreshold(0.0);
		m_character = new btKinematicCharacterController(ghost, capsule, stepHeight);
		//m_character->setUpInterpolate(false);
		collisions_enabled = true;

		addToWorld();
		
		getDynamicsWorld()->getBulletDynamicsWorld()->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(ghost->getBroadphaseHandle(),getDynamicsWorld()->getBulletDynamicsWorld()->getDispatcher());
		reset();
    }
    // -------------------------------------------------------------------------
    CharacterController::~CharacterController()
    {
		mShapeNode->detachAllObjects();
    }  
	void CharacterController::addToWorld()
	{
		if (in_world == true)
			return;

		//add collider to world
		getDynamicsWorld()->getBulletDynamicsWorld()->addCollisionObject(mObject,btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
		getDynamicsWorld()->getBulletDynamicsWorld()->addAction(m_character);

		//attach scene node
		mShapeNode->attachObject(this);
		in_world = true;
	}
	void CharacterController::removeFromWorld()
	{
		if (in_world == false)
			return;

		//remove collider from world
		getDynamicsWorld()->getBulletDynamicsWorld()->removeCollisionObject(mObject);

		//detach scene node
		mShapeNode->detachObject(this);
		in_world = false;
	}

    void CharacterController::setWalkDirection( const Ogre::Vector3 &dir, const Ogre::Real speed )
    {
		m_character->setWalkDirection(btVector3(dir.x * speed, dir.y * speed, dir.z * speed));	
    }
    void CharacterController::setVelocityForTimeInterval( const Ogre::Vector3 &velocity, const Ogre::Real timeInterval )
    {
    	m_character->setVelocityForTimeInterval(btVector3(velocity.x, velocity.y, velocity.z), timeInterval);
    }
    void CharacterController::reset()
    {
    	m_character->reset(getDynamicsWorld()->getBulletDynamicsWorld());
    }
    void CharacterController::warp(const Ogre::Vector3 &origin)
    {
    	m_character->warp(btVector3(origin.x, origin.y, origin.z));
    }
    void CharacterController::setFallSpeed(Ogre::Real speed)
    {
    	m_character->setFallSpeed(speed);
    }
    void CharacterController::setJumpSpeed(Ogre::Real speed)
    {
		m_character->setJumpSpeed(speed);
    }
    void CharacterController::setMaxJumpHeight(Ogre::Real height)
    {
		m_character->setMaxJumpHeight(height);
    }
    void CharacterController::jump()
    {
    	m_character->jump();
    }
    bool CharacterController::canJump()
    {
    	return m_character->canJump();
    }
    void CharacterController::setGravity(Ogre::Real gravity)
    {
    	m_character->setGravity(gravity);
    }
    Ogre::Real CharacterController::getGravity()
    {
    	return m_character->getGravity();
    }
    void CharacterController::setMaxSlope(Ogre::Real radians)
    {
    	m_character->setMaxSlope(radians);
    }
    Ogre::Real CharacterController::getMaxSlope()
    {
    	return m_character->getMaxSlope();
    }
    bool CharacterController::onGround()
    {
    	return m_character->onGround();
    }
	void CharacterController::sync()
	{
		setTransform(mObject->getWorldTransform());
	}
	void CharacterController::enableCollisions(bool value)
	{
		collisions_enabled = value;
		resetCollisions();
	}
	void CharacterController::resetCollisions()
	{
		//removes and adds character collider back to world to reset collisions for stationary objects
		//there might be a better way to do this, but this works well
		getDynamicsWorld()->getBulletDynamicsWorld()->removeAction(m_character);
		getDynamicsWorld()->getBulletDynamicsWorld()->removeCollisionObject(mObject);
		if (collisions_enabled == true)
			getDynamicsWorld()->getBulletDynamicsWorld()->addCollisionObject(mObject,btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
		else
			getDynamicsWorld()->getBulletDynamicsWorld()->addCollisionObject(mObject,btBroadphaseProxy::CharacterFilter, 0);
		getDynamicsWorld()->getBulletDynamicsWorld()->addAction(m_character);
	}
}

