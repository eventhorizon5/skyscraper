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
#include "Constraints/OgreBulletDynamics6DofConstraint.h"

using namespace Ogre;

namespace OgreBulletDynamics
{
    // -------------------------------------------------------------------------
    SixDofConstraint::SixDofConstraint(RigidBody * rbA, RigidBody * rbB, 
        const Vector3& FrameInAVector3, const Quaternion& FrameInAOrientation,
        const Vector3& FrameInBVector3, const Quaternion& FrameInBOrientation):
    TypedConstraint(rbA, rbB)
    {
        btTransform frameInA (OgreBulletCollisions::OgreBtConverter::to (FrameInAOrientation), 
                              OgreBulletCollisions::OgreBtConverter::to (FrameInAVector3));
        btTransform frameInB (OgreBulletCollisions::OgreBtConverter::to (FrameInBOrientation),
                              OgreBulletCollisions::OgreBtConverter::to (FrameInBVector3));

        mConstraint = new btGeneric6DofConstraint(
            *rbA->getBulletRigidBody (),
            *rbB->getBulletRigidBody (), 
            frameInA,
            frameInB,
			true); // Eric added this because Bullet 2.61 has a new argument (useLinearReferenceFrameA)
    }
    // -------------------------------------------------------------------------
    SixDofConstraint::~SixDofConstraint()
    {
	}
	// -------------------------------------------------------------------------
	void	 SixDofConstraint::setLinearLowerLimit(const Ogre::Vector3& linearLower)
	{
		static_cast<btGeneric6DofConstraint* > (mConstraint)->setLinearLowerLimit(OgreBulletCollisions::OgreBtConverter::to (linearLower));
	}
	// -------------------------------------------------------------------------
	void	 SixDofConstraint::setLinearUpperLimit(const Ogre::Vector3& linearUpper)
	{
		static_cast<btGeneric6DofConstraint* > (mConstraint)->setLinearLowerLimit(OgreBulletCollisions::OgreBtConverter::to (linearUpper));
	}
	// -------------------------------------------------------------------------
	void	 SixDofConstraint::setAngularLowerLimit(const Ogre::Vector3& angularLower)
	{
		static_cast<btGeneric6DofConstraint* > (mConstraint)->setLinearLowerLimit(OgreBulletCollisions::OgreBtConverter::to (angularLower));
	}
	// -------------------------------------------------------------------------
	void	 SixDofConstraint::setAngularUpperLimit(const Ogre::Vector3& angularUpper)
	{
		static_cast<btGeneric6DofConstraint* > (mConstraint)->setLinearLowerLimit(OgreBulletCollisions::OgreBtConverter::to (angularUpper));
	}
	// -------------------------------------------------------------------------
	void	 SixDofConstraint::setLimit(const int axis, const Ogre::Real lo, const Ogre::Real hi)
	{
		static_cast<btGeneric6DofConstraint* > (mConstraint)->setLimit(axis, lo, hi);
	}
	// -------------------------------------------------------------------------
	bool	 SixDofConstraint::isLimited(int limitIndex)
	{
		return static_cast<btGeneric6DofConstraint* > (mConstraint)->isLimited(limitIndex);
	}
}