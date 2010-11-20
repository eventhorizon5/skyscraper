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
#ifndef _OgreBulletCollisions_DEBUGOBJECT_H_
#define _OgreBulletCollisions_DEBUGOBJECT_H_

#include "OgreBulletCollisionsPreRequisites.h"

#include "Debug/OgreBulletCollisionsDebugLines.h"

namespace OgreBulletCollisions
{
   
	//------------------------------------------------------------------------------------------------
	class  DebugCollisionShape : public DebugLines
	{
	public:
		enum Mode
		{
			Mode_Unknown,
			Mode_Enabled,
			Mode_Disabled,
			Mode_Static
        };

	public:
        /** DebugCollisionShape ctor
        */
        DebugCollisionShape(CollisionShape * shape, 
                            DebugCollisionShape::Mode mode = DebugCollisionShape::Mode_Enabled);
		/** ~DebugCollisionShape ctor
		*/
        virtual ~DebugCollisionShape();
        // mStatemode getter
        DebugCollisionShape::Mode getStatemode() const { return mStatemode; }
        // mStatemode setter
        void setStatemode(DebugCollisionShape::Mode val);
        // mIsVisual getter
        bool getIsVisual() const;
        // mIsVisual setter
        void setIsVisual(bool val);

	protected:
        DebugCollisionShape::Mode mStatemode;
        bool                      mIsVisual;


	};
	//------------------------------------------------------------------------------------------------
	class  RayDebugShape:public DebugLines
	{
	public:
		RayDebugShape(const Ogre::Vector3& start,const Ogre::Vector3& direction,const Ogre::Real length);
		void setDefinition(const Ogre::Vector3& start,const Ogre::Vector3& direction,const Ogre::Real length);
		virtual ~RayDebugShape();
    };
    //------------------------------------------------------------------------------------------------
}

#endif //_OgreBulletCollisions_DEBUGOBJECT_H_


