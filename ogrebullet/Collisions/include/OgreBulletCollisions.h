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

#ifndef _OGREBULLETCOLLISIONS_H
#define _OGREBULLETCOLLISIONS_H

#include "OgreBulletCollisionsPreRequisites.h"


#ifdef _PRECOMP


    #include "Utils/OgreBulletConverter.h"

	//Debug Helpers
	#include "Debug/OgreBulletCollisionsDebugLines.h"
	#include "Debug/OgreBulletCollisionsDebugShape.h"
	#include "Debug/OgreBulletCollisionsDebugDrawer.h"
	#include "Debug/OgreBulletCollisionsDebugContact.h"

    //Shapes
    #include "OgreBulletCollisionsShape.h"

    #include "Shapes/OgreBulletCollisionsBoxShape.h"
    #include "Shapes/OgreBulletCollisionsConeShape.h"
    #include "Shapes/OgreBulletCollisionsSphereShape.h"
    #include "Shapes/OgreBulletCollisionsCylinderShape.h"
	#include "Shapes/OgreBulletCollisionsTriangleShape.h"
	#include "Shapes/OgreBulletCollisionsTerrainShape.h"
	#include "Shapes/OgreBulletCollisionsCylinderShape.h"
	#include "Shapes/OgreBulletCollisionsCapsuleShape.h"
    #include "Shapes/OgreBulletCollisionsStaticPlaneShape.h"

    #include "Shapes/OgreBulletCollisionsCompoundShape.h"

    #include "Shapes/OgreBulletCollisionsMultiSphereShape.h"

    #include "Shapes/OgreBulletCollisionsConvexHullShape.h"
    #include "Shapes/OgreBulletCollisionsMinkowskiSumShape.h"

	#include "Shapes/OgreBulletCollisionsTrimeshShape.h"
	#include "Shapes/OgreBulletCollisionsGImpactShape.h"

    //Utils
    #include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"

    // Ray
    //#include "OgreBulletCollisionsConvexCast.h"

    // Core Collisions
    #include "OgreBulletCollisionsObject.h"
    #include "OgreBulletCollisionsObjectState.h"

    #include "OgreBulletCollisionsWorld.h"

    #include "OgreBulletCollisionsRay.h"





    #include "OgreString.h"
    #include "OgreFontManager.h"

    #include "BulletCollision/CollisionShapes/btMinkowskiSumShape.h"
    #include "BulletCollision/CollisionShapes/btTriangleShape.h"


#endif //_PRECOMP

#endif //_OGREBULLETCOLLISIONS_H

