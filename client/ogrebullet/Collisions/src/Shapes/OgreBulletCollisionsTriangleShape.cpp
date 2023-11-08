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

#include "OgreBulletCollisions.h"

#include "Shapes/OgreBulletCollisionsTriangleShape.h"

#include "BulletCollision/CollisionShapes/btTriangleShape.h"
#include "Utils/OgreBulletConverter.h"
#include "Debug/OgreBulletCollisionsDebugLines.h"

using namespace Ogre;
using namespace OgreBulletCollisions;

namespace OgreBulletCollisions
{
    // -------------------------------------------------------------------------
    TriangleCollisionShape::TriangleCollisionShape(
        const Ogre::Vector3 &p1, 
        const Ogre::Vector3 &p2, 
        const Ogre::Vector3 &p3):	
        CollisionShape()
    {
            mShape = new btTriangleShape(OgreBtConverter::to(p1), 
                                         OgreBtConverter::to(p2), 
                                         OgreBtConverter::to(p3)); 
    }
    // -------------------------------------------------------------------------
    TriangleCollisionShape::TriangleCollisionShape( 
        Ogre::Real p1X, Ogre::Real p1Y, Ogre::Real p1Z, 
        Ogre::Real p2X, Ogre::Real p2Y, Ogre::Real p2Z,  
        Ogre::Real p3X, Ogre::Real p3Y, Ogre::Real p3Z):
        CollisionShape()
    {
        mShape = new btTriangleShape(btVector3(p1X, p1Z, p1Z), 
                                     btVector3(p2X, p2Y, p2Z), 
                                     btVector3(p3X, p3Y, p3Z)); 
    }
    // -------------------------------------------------------------------------
    TriangleCollisionShape::~TriangleCollisionShape()
    {
    }
}

