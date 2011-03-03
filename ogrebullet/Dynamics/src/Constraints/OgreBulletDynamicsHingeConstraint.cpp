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
#include "Constraints/OgreBulletDynamicsHingeConstraint.h"

using namespace Ogre;

namespace OgreBulletDynamics
{
    // -------------------------------------------------------------------------
    HingeConstraint::HingeConstraint(RigidBody * rbA, RigidBody * rbB, const Vector3& pivotInA,
        const Vector3& pivotInB, const Vector3& axisInA, const Vector3& axisInB):
        TypedConstraint(rbA, rbB)
    {
        btVector3 vec[4];
        vec[0] = OgreBulletCollisions::OgreBtConverter::to(pivotInA);
        vec[1] = OgreBulletCollisions::OgreBtConverter::to(pivotInB);
        vec[2] = OgreBulletCollisions::OgreBtConverter::to(axisInA);
        vec[3] = OgreBulletCollisions::OgreBtConverter::to(axisInB);
       
        mConstraint = new btHingeConstraint(
            *rbA->getBulletRigidBody (),
            *rbB->getBulletRigidBody (),
            vec[0], vec[1], vec[2], vec[3]);
/*
        mConstraint = new btHingeConstraint(
            *rbA->getBulletRigidBody (),
            *rbB->getBulletRigidBody (), 
            OgreBulletCollisions::OgreBtConverter::to(pivotInA),
            OgreBulletCollisions::OgreBtConverter::to(pivotInB), 
            OgreBulletCollisions::OgreBtConverter::to(axisInA), 
            OgreBulletCollisions::OgreBtConverter::to(axisInB));*/
    }
    // -------------------------------------------------------------------------
        HingeConstraint::HingeConstraint(RigidBody * rbA, 
            const Vector3& pivotInA, 
            const Vector3&  axisInA):
    TypedConstraint(rbA)
    {
        btVector3 vec[2];
        vec[0] = OgreBulletCollisions::OgreBtConverter::to(pivotInA);
        vec[1] = OgreBulletCollisions::OgreBtConverter::to(axisInA);

        mConstraint = new btHingeConstraint(*rbA->getBulletRigidBody (),
            vec[0], vec[1]);

/*
        mConstraint = new btHingeConstraint(*rbA->getBulletRigidBody (),
            OgreBulletCollisions::OgreBtConverter::to(pivotInA),
            OgreBulletCollisions::OgreBtConverter::to(axisInA));*/
    }
    // -------------------------------------------------------------------------
    HingeConstraint::~HingeConstraint()
    {
    }
}
