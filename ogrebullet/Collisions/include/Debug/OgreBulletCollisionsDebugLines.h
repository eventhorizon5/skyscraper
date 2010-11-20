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
#ifndef _OgreBulletCollisions_DEBUGLines_H_
#define _OgreBulletCollisions_DEBUGLines_H_

#include "OgreBulletCollisionsPreRequisites.h"

namespace OgreBulletCollisions
{
    //------------------------------------------------------------------------------------------------
    class _OgreBulletExport DebugLines:public Ogre::SimpleRenderable
    {
    public:
        DebugLines(void);
        ~DebugLines(void);

        void addLine (const Ogre::Vector3 &start,const Ogre::Vector3 &end)
        {
            clear ();

            _points.push_back (start);
            _points.push_back (end);
        }

        void addLine(Ogre::Real start_x, Ogre::Real start_y, Ogre::Real start_z, 
            Ogre::Real end_x, Ogre::Real end_y, Ogre::Real end_z)
        {
            addLine (Ogre::Vector3(start_x,start_y,start_z),
                Ogre::Vector3(end_x,end_y,end_z));
        }

        void addPoint (const Ogre::Vector3 &pt)
        {
            clear();

            _points.push_back(pt);
        }

        void addPoint (Ogre::Real x, Ogre::Real y, Ogre::Real z)
        {
            addPoint (Ogre::Vector3(x, y, z));
        }

        void draw ();
        void clear ();

        Ogre::Real getSquaredViewDepth (const Ogre::Camera *cam) const;
        Ogre::Real getBoundingRadius (void) const;

    protected:

        Vector3Array _points;
        bool _drawn;

        static bool _materials_created;
    };
}
#endif //_OgreBulletCollisions_DEBUGLines_H_

