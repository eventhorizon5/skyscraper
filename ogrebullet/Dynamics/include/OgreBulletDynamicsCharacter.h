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

#ifndef _OGREBULLETDYNAMICS_Character_H
#define _OGREBULLETDYNAMICS_Character_H

#include "OgreBulletDynamicsPreRequisites.h"

#include "OgreBulletCollisionsObject.h"
#include "OgreBulletCollisionsWorld.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "Utils/OgreBulletConverter.h"

#include "OgreBulletDynamicsWorld.h"

namespace OgreBulletDynamics
{
    // -------------------------------------------------------------------------
    // basic character controller class
    class _OgreBulletExport CharacterController : public OgreBulletCollisions::Object
    {
    public:
	    CharacterController(const Ogre::String &name,
				  DynamicsWorld *world,
				  Ogre::SceneNode *node,
				  float width,
				  float height,
				  float stepHeight);

		virtual ~CharacterController();

		void addToWorld();
		void removeFromWorld();

		inline DynamicsWorld*       getDynamicsWorld();

		Ogre::SceneNode*  getSceneNode() const {return mRootNode;};

		void setWalkDirection( const Ogre::Vector3 &dir, const Ogre::Real speed );
		void setVelocityForTimeInterval( const Ogre::Vector3 &velocity, const Ogre::Real timeInterval );
		void reset();
		void warp(const Ogre::Vector3 &origin);
		void setFallSpeed(Ogre::Real speed);
		void setJumpSpeed(Ogre::Real speed);
		void setMaxJumpHeight(Ogre::Real height);
		void jump();
		bool canJump();
		void setGravity(Ogre::Real gravity);
		Ogre::Real getGravity();
		void setMaxSlope(Ogre::Real radians);
		Ogre::Real getMaxSlope();
		bool onGround();
		void sync();
		void enableCollisions(bool value);
		void resetCollisions();

	protected:
		short mCollisionGroup;
		short mCollisionMask;
		bool in_world;
		bool collisions_enabled;
		btKinematicCharacterController *m_character;
		btGhostPairCallback *callback;
		btConvexShape *capsule;
    };
    // -------------------------------------------------------------------------
    inline DynamicsWorld*       CharacterController::getDynamicsWorld()
    {
        return static_cast <DynamicsWorld* > (mWorld);
    };

}
#endif //_OGREBULLETDYNAMICS_Character_H

