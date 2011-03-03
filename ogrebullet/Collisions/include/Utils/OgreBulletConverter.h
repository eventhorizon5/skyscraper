/***************************************************************************

This source file is part of OGREBULLET
(Object-oriented Graphics Rendering Engine Bullet Wrapper)
For the latest info, see http://www.ogre3d.org/phpBB2addons/viewforum.php?f=10

Copyright (c) 2007 tuan.kuranes@gmail.com



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
#ifndef _OGREBULLETCOLLISIONS_OgreBtConverter_H
#define _OGREBULLETCOLLISIONS_OgreBtConverter_H

#include "OgreBulletCollisionsPreRequisites.h"

#include "OgreVector3.h"
#include "OgreQuaternion.h"

namespace OgreBulletCollisions
{
    class _OgreBulletExport OgreBtConverter
    {
    public:
        OgreBtConverter(){};
        ~OgreBtConverter(){};

        static btVector3 to(const Ogre::Vector3 &V)
        {
            return btVector3(V.x, V.y, V.z);
        };

        static btQuaternion to(const Ogre::Quaternion &Q)
        {
            return btQuaternion(Q.x, Q.y, Q.z, Q.w);
        };

    };
    class _OgreBulletExport BtOgreConverter
    {
    public:
        BtOgreConverter(){};
        ~BtOgreConverter(){};

        static Ogre::Vector3 to(const btVector3 &V)
        {
            return Ogre::Vector3(V.x(), V.y(), V.z());
        };

        static Ogre::Quaternion to(const btQuaternion &Q)
        {
            return Ogre::Quaternion(Q.w(), Q.x(), Q.y(), Q.z());
            //return Ogre::Quaternion(Q.x(), Q.y(), Q.z(), Q[3]);
        };
    };
}
#endif //_OGREBULLETCOLLISIONS_OgreBtConverter_H