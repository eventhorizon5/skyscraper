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

#ifndef _OGREBULLETCOLLISIONS_PREREQUISITES_H
#define _OGREBULLETCOLLISIONS_PREREQUISITES_H

#include "btBulletCollisionCommon.h"

#include "BulletCollision/Gimpact/btGImpactShape.h"
#include "BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"

#include "Ogre.h"

/* Some <math.h> files do not define M_PI... */
#ifndef M_PI
#define M_PI 3.14159265
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#   define _CDECL _cdecl
#   if defined( _OGREBULLET_EXPORTS ) && defined( _OGREBULLET_DYNAMIC )
#       define _OgreBulletExport __declspec( dllexport )
#   elif defined( _OGREBULLET_DYNAMIC )
#       define _OgreBulletExport __declspec( dllimport )
#   else
#       define _OgreBulletExport
#   endif
#else
#	define _OgreBulletExport
#endif

namespace OgreBulletCollisions
{
    class _OgreBulletExport BtOgreConverter;
    class _OgreBulletExport OgreBtConverter;

    class _OgreBulletExport CollisionsWorld;

    class _OgreBulletExport CollisionRayResultCallback;
    class _OgreBulletExport CollisionClosestRayResultCallback;

    class _OgreBulletExport DebugLines;

    class _OgreBulletExport DebugDrawer;

    class _OgreBulletExport DebugContact;
    class _OgreBulletExport DebugNormal;
    class _OgreBulletExport DebugContactText;
    
    class _OgreBulletExport DebugCollisionShape;
    class _OgreBulletExport BoxDebugShape;
    class _OgreBulletExport SphereDebugShape;
    class _OgreBulletExport ConeDebugShape;
    class _OgreBulletExport TriangleMeshDebugShape;
    class _OgreBulletExport RayDebugShape;

	class _OgreBulletExport GImpactConcaveShape;
	class _OgreBulletExport HeightmapCollisionShape;
    class _OgreBulletExport CollisionShape;
    class _OgreBulletExport BoxCollisionShape;
    class _OgreBulletExport SphereCollisionShape;
    class _OgreBulletExport CylinderCollisionShape;
	class _OgreBulletExport CapsuleCollisionShape;
    class _OgreBulletExport ConeCollisionShape;

    class _OgreBulletExport CompoundCollisionShape;

    class _OgreBulletExport MultiSphereCollisionShape;
    class _OgreBulletExport MinkowskiSumCollisionShape;

    class _OgreBulletExport ConvexHullCollisionShape;
    class _OgreBulletExport TriangleMeshCollisionShape;

    class _OgreBulletExport Object;

    class _OgreBulletExport ObjectState;

	class _OgreBulletExport CollisionShapebject;

    typedef std::vector<Ogre::Vector3> Vector3Array;
    typedef std::map<unsigned short, Vector3Array* > BoneIndex;
    typedef std::pair<unsigned short, Vector3Array* > BoneKeyIndex;

}

#endif //_OGREBULLETCOLLISIONS_PREREQUISITES_H

