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
    CharacterController::CharacterController(const String &name, DynamicsWorld *world, Ogre::SceneNode *node)
        :	
        Object(name, world, false)
    {
		in_world = false;
		mState = new ObjectState(this);
		mRootNode = node;
		mShapeNode = mRootNode->createChildSceneNode(mName + "Node");
		btTransform startTransform;
	        startTransform.setIdentity ();
		startTransform.setOrigin (btVector3(node->getPosition().x,node->getPosition().y,node->getPosition().z));
		showDebugShape(mWorld->getShowDebugShapes());
		btPairCachingGhostObject* ghost = new btPairCachingGhostObject();
		mObject = ghost;
		mObject->setWorldTransform(startTransform);
		getBulletDynamicsWorld()->getPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
		btScalar characterHeight=1.75;
	        btScalar characterWidth =1.75;
	        btConvexShape* capsule = new btCapsuleShape(characterWidth,characterHeight);
		mObject->setCollisionShape (capsule);
		mObject->setCollisionFlags (btCollisionObject::CF_CHARACTER_OBJECT);
		btScalar stepHeight = btScalar(0.35);
	        m_character = new btKinematicCharacterController (ghost,capsule,stepHeight);

		addToWorld();

    }
    // -------------------------------------------------------------------------
    CharacterController::~CharacterController()
    {
		mShapeNode->detachObject(this);
   }  
	void CharacterController::addToWorld()
	{
		if (in_world == true)
			return;

		//add collider to world
		getBulletDynamicsWorld()->addCollisionObject(mObject,btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
		getBulletDynamicsWorld()->addAction(m_character);

		//attach scene node
		mShapeNode->attachObject(this);
		in_world = true;
	}
	void CharacterController::removeFromWorld()
	{
		if (in_world == false)
			return;

		//remove collider from world
		getBulletDynamicsWorld()->removeCollisionObject(mObject);

		//detach scene node
		mShapeNode->detachObject(this);
		in_world = false;
	}

	// -------------------------------------------------------------------------
    void CharacterController::setLinearVelocity( const Ogre::Vector3 &vel )
    {
        //getBulletCharacterController()->setLinearVelocity (btVector3(vel.x, vel.y, vel.z));
    }
    // -------------------------------------------------------------------------
    void CharacterController::setLinearVelocity( const Ogre::Real x, const Ogre::Real y, const Ogre::Real z )
    {
        //getBulletCharacterController()->setLinearVelocity (btVector3(x, y, z));
    }
    // -------------------------------------------------------------------------
	Ogre::Vector3 CharacterController::getLinearVelocity()
	{
		//const btVector3 lv = getBulletCharacterController()->getLinearVelocity();
		//return BtOgreConverter::to(lv);
		return Ogre::Vector3::ZERO;
	}
    // -------------------------------------------------------------------------
    void CharacterController::setAngularVelocity( const Ogre::Vector3 &vel )
    {
        //getBulletCharacterController()->setAngularVelocity (btVector3(vel.x, vel.y, vel.z));
    }
    // -------------------------------------------------------------------------
    void CharacterController::setAngularVelocity( const Ogre::Real x, const Ogre::Real y, const Ogre::Real z )
    {
        //getBulletCharacterController()->setAngularVelocity (btVector3(x, y, z));
    }
    // -------------------------------------------------------------------------
	Ogre::Vector3 CharacterController::getAngularVelocity()
	{
		//const btVector3 lv = getBulletCharacterController()->getAngularVelocity();
		//return BtOgreConverter::to(lv);
		return Ogre::Vector3::ZERO;
	}
    // -------------------------------------------------------------------------
    void CharacterController::setAngularFactor( const Ogre::Vector3 &vel )
    {
        //getBulletCharacterController()->setAngularFactor (btVector3(vel.x, vel.y, vel.z));
    }
    // -------------------------------------------------------------------------
    void CharacterController::setAngularFactor( const Ogre::Real x, const Ogre::Real y, const Ogre::Real z )
    {
        //getBulletCharacterController()->setAngularFactor (btVector3(x, y, z));
    }
    // -------------------------------------------------------------------------
	Ogre::Vector3 CharacterController::getAngularFactor()
	{
		//const btVector3 lv = getBulletCharacterController()->getAngularFactor();
		//return BtOgreConverter::to(lv);
		return Ogre::Vector3::ZERO;
	}
    // -------------------------------------------------------------------------
    void CharacterController::setSleepingThresholds( const Ogre::Real linear, const Ogre::Real angular )
    {
        //getBulletCharacterController()->setSleepingThresholds (linear, angular);
    }
    // -------------------------------------------------------------------------
    void CharacterController::applyImpulse( const Ogre::Vector3 &impulse, const Ogre::Vector3 &position )
    {
        //getBulletCharacterController()->applyImpulse (OgreBtConverter::to(impulse), OgreBtConverter::to(position));
    }
    // -------------------------------------------------------------------------
    void CharacterController::applyForce( const Ogre::Vector3 &impulse, const Ogre::Vector3 &position )
    {
        //getBulletCharacterController()->applyForce(OgreBtConverter::to(impulse), OgreBtConverter::to(position));
    }
    // -------------------------------------------------------------------------
    Ogre::Vector3 CharacterController::getCenterOfMassPivot( const Ogre::Vector3 &pivotPosition ) const
    {
        //const btVector3 centerOfMassPivot(getCenterOfMassTransform().inverse()* OgreBtConverter::to(pivotPosition));
        //return BtOgreConverter::to(centerOfMassPivot);
	return Ogre::Vector3::ZERO;
    }
    // -------------------------------------------------------------------------
    void CharacterController::setDeactivationTime( const float ftime )
    {
        //getBulletCharacterController()->setDeactivationTime( ftime );
    }
    // -------------------------------------------------------------------------
    void CharacterController::setDamping( const Ogre::Real linearDamping, const Ogre::Real angularDamping )
    {
        //getBulletCharacterController()->setDamping( linearDamping,  angularDamping);
    }
    // -------------------------------------------------------------------------
	void CharacterController::setGravity( const Ogre::Vector3 &gravity )
	{
		//getBulletCharacterController()->setGravity(OgreBtConverter::to(gravity));
	}
    // -------------------------------------------------------------------------
	Ogre::Vector3 CharacterController::getGravity()
	{
		//const btVector3 gravity = getBulletCharacterController()->getGravity();
		//return BtOgreConverter::to(gravity);
		return Ogre::Vector3::ZERO;
	}
}

