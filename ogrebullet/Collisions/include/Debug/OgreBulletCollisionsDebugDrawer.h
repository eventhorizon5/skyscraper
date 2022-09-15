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
#ifndef _OgreBulletCollisions_DebugDrawer_H_
#define _OgreBulletCollisions_DebugDrawer_H_

#include "OgreBulletCollisionsPreRequisites.h"

#include "Debug/OgreBulletCollisionsDebugLines.h"

namespace OgreBulletCollisions
{
	//------------------------------------------------------------------------------------------------
	class _OgreBulletExport DebugDrawer : public DebugLines, public btIDebugDraw
	{
	public:
        DebugDrawer();
		virtual ~DebugDrawer();


        virtual void	setDebugMode(int mode){mDebugMode = mode;};
        virtual int		getDebugMode() const { return mDebugMode;};

        virtual void	drawLine(const btVector3& from,const btVector3& to,const btVector3& color);
        virtual void	drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,
            btScalar distance,int lifeTime,const btVector3& color);

        void drawAabb(const Ogre::Vector3& from,const Ogre::Vector3& to,const Ogre::Vector3& color);
        void drawLine(const Ogre::Vector3& from,const Ogre::Vector3& to,const Ogre::Vector3& color);
        void drawContactPoint(const Ogre::Vector3& PointOnB,const Ogre::Vector3& normalOnB,
            Ogre::Real distance,int lifeTime,const Ogre::Vector3& color);

        void setDrawAabb(bool enable);
        void setDrawWireframe(bool enable);
        void setDrawFeaturesText(bool enable);
        void setDrawContactPoints(bool enable);
        void setNoDeactivation(bool enable);
        void setNoHelpText(bool enable);
        void setDrawText(bool enable);
        void setProfileTimings(bool enable);
        void setEnableSatComparison(bool enable);
        void setDisableBulletLCP (bool enable);
        void setEnableCCD(bool enable);

        bool doesDrawAabb () const;
        bool doesDrawWireframe () const;
        bool doesDrawFeaturesText () const;
        bool doesDrawContactPoints () const;
        bool doesNoDeactivation () const;
        bool doesNoHelpText () const;
        bool doesDrawText () const;
        bool doesProfileTimings () const;
        bool doesEnableSatComparison () const;
        bool doesDisableBulletLCP () const;
        bool doesEnableCCD () const;

        void drawAabb(const btVector3& from,const btVector3& to,const btVector3& color);

		// TODO
		void	draw3dText(const btVector3& location,const char* textString);

		void	reportErrorWarning(const char* warningString);
	protected:
        int mDebugMode;
	};
}

#endif //_OgreBulletCollisions_DebugDrawer_H_

