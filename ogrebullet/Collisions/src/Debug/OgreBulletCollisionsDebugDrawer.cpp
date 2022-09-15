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

#include "Debug/OgreBulletCollisionsDebugDrawer.h"
#include "Utils/OgreBulletConverter.h"

using namespace OgreBulletCollisions;
using namespace Ogre;

//------------------------------------------------------------------------------------------------
DebugDrawer::DebugDrawer() :
    DebugLines(),
    btIDebugDraw(),
    mDebugMode(0)
{
}
//------------------------------------------------------------------------------------------------
DebugDrawer::~DebugDrawer() 
{
}
//------------------------------------------------------------------------------------------------
void	DebugDrawer::reportErrorWarning(const char* warningString)
{
	Ogre::LogManager::getSingleton().getDefaultLog()->logMessage(warningString);
}
//------------------------------------------------------------------------------------------------
void DebugDrawer::setDrawAabb(bool enable)
{ 
    if (enable) 
        mDebugMode |= btIDebugDraw::DBG_DrawAabb;
    else
        mDebugMode &= ~btIDebugDraw::DBG_DrawAabb;
}
//------------------------------------------------------------------------------------------------
void	DebugDrawer::draw3dText(const btVector3& location,const char* textString)
{
}
//------------------------------------------------------------------------------------------------
bool DebugDrawer::doesDrawAabb () const {return (mDebugMode & btIDebugDraw::DBG_DrawAabb) != 0;};
//------------------------------------------------------------------------------------------------
bool DebugDrawer::doesDrawWireframe () const {return (mDebugMode & btIDebugDraw::DBG_DrawWireframe) != 0;};
//------------------------------------------------------------------------------------------------
bool DebugDrawer::doesDrawFeaturesText () const {return (mDebugMode & btIDebugDraw::DBG_DrawFeaturesText) != 0;};
//------------------------------------------------------------------------------------------------
bool DebugDrawer::doesDrawContactPoints () const {return (mDebugMode & btIDebugDraw::DBG_DrawContactPoints) != 0;};
//------------------------------------------------------------------------------------------------
bool DebugDrawer::doesNoDeactivation () const {return (mDebugMode & btIDebugDraw::DBG_NoDeactivation) != 0;};
//------------------------------------------------------------------------------------------------
bool DebugDrawer::doesNoHelpText () const {return (mDebugMode & btIDebugDraw::DBG_NoHelpText) != 0;};
//------------------------------------------------------------------------------------------------
bool DebugDrawer::doesDrawText () const {return (mDebugMode & btIDebugDraw::DBG_DrawText) != 0;};
//------------------------------------------------------------------------------------------------
bool DebugDrawer::doesProfileTimings () const {return (mDebugMode & btIDebugDraw::DBG_ProfileTimings) != 0;};
//------------------------------------------------------------------------------------------------
bool DebugDrawer::doesEnableSatComparison () const {return (mDebugMode & btIDebugDraw::DBG_EnableSatComparison) != 0;};
//------------------------------------------------------------------------------------------------
bool DebugDrawer::doesDisableBulletLCP () const {return (mDebugMode & btIDebugDraw::DBG_DisableBulletLCP) != 0;};
//------------------------------------------------------------------------------------------------
bool DebugDrawer::doesEnableCCD () const {return (mDebugMode & btIDebugDraw::DBG_EnableCCD) != 0;};
//------------------------------------------------------------------------------------------------
void DebugDrawer::setDrawWireframe(bool enable)
{ 
    if (enable) 
        mDebugMode |= btIDebugDraw::DBG_DrawWireframe;
    else
        mDebugMode &= ~btIDebugDraw::DBG_DrawWireframe;
}
//------------------------------------------------------------------------------------------------
void DebugDrawer::setDrawFeaturesText(bool enable)
{ 
    if (enable) 
        mDebugMode |= btIDebugDraw::DBG_DrawFeaturesText;
    else
        mDebugMode &= ~btIDebugDraw::DBG_DrawFeaturesText;
}
//------------------------------------------------------------------------------------------------
void DebugDrawer::setDrawContactPoints(bool enable)
{ 
    if (enable) 
        mDebugMode |= btIDebugDraw::DBG_DrawContactPoints;
    else
        mDebugMode &= ~btIDebugDraw::DBG_DrawContactPoints;
}
//------------------------------------------------------------------------------------------------
void DebugDrawer::setNoDeactivation(bool enable)
{ 
    if (enable) 
        mDebugMode |= btIDebugDraw::DBG_NoDeactivation;
    else
        mDebugMode &= ~btIDebugDraw::DBG_NoDeactivation;
}
//------------------------------------------------------------------------------------------------
void DebugDrawer::setNoHelpText(bool enable)
{ 
    if (enable) 
        mDebugMode |= btIDebugDraw::DBG_NoHelpText;
    else
        mDebugMode &= ~btIDebugDraw::DBG_NoHelpText;
}
//------------------------------------------------------------------------------------------------
void DebugDrawer::setDrawText(bool enable)
{ 
    if (enable) 
        mDebugMode |= btIDebugDraw::DBG_DrawText;
    else
        mDebugMode &= ~btIDebugDraw::DBG_DrawText;
}
//------------------------------------------------------------------------------------------------
void DebugDrawer::setProfileTimings(bool enable)
{ 
    if (enable) 
        mDebugMode |= btIDebugDraw::DBG_ProfileTimings;
    else
        mDebugMode &= ~btIDebugDraw::DBG_ProfileTimings;
}
//------------------------------------------------------------------------------------------------
void DebugDrawer::setEnableSatComparison(bool enable)
{ 
    if (enable) 
        mDebugMode |= btIDebugDraw::DBG_EnableSatComparison;
    else
        mDebugMode &= ~btIDebugDraw::DBG_EnableSatComparison;
}
//------------------------------------------------------------------------------------------------
void DebugDrawer::setDisableBulletLCP (bool enable)
{ 
    if (enable) 
        mDebugMode |= btIDebugDraw::DBG_DisableBulletLCP ;
    else
        mDebugMode &= ~btIDebugDraw::DBG_DisableBulletLCP ;
}
//------------------------------------------------------------------------------------------------
void DebugDrawer::setEnableCCD(bool enable)
{ 
    if (enable) 
        mDebugMode |= btIDebugDraw::DBG_EnableCCD;
    else
        mDebugMode &= ~btIDebugDraw::DBG_EnableCCD;
}
//------------------------------------------------------------------------------------------------
void DebugDrawer::drawAabb(const btVector3& from,const btVector3& to,const btVector3& color)
{
    if (mDebugMode & btIDebugDraw::DBG_DrawAabb)
    {
        drawAabb (BtOgreConverter::to(from), BtOgreConverter::to(to), BtOgreConverter::to (color));
    }
}
//------------------------------------------------------------------------------------------------
void	DebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
{
    if (mDebugMode > 0)
    {
        drawLine (BtOgreConverter::to(from), BtOgreConverter::to(to), BtOgreConverter::to (color));
    }
}
//------------------------------------------------------------------------------------------------
void	DebugDrawer::drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,
                                              btScalar distance,int lifeTime,const btVector3& color)
{
    if (mDebugMode & btIDebugDraw::DBG_DrawContactPoints)
    {
        //
    }
}
//------------------------------------------------------------------------------------------------
void DebugDrawer::drawAabb (const Ogre::Vector3& from,const Ogre::Vector3& to,const Ogre::Vector3& color)
{
    if (mDebugMode & btIDebugDraw::DBG_DrawAabb)
    {
        Vector3 halfExtents = (to-from)* 0.5f;
        Vector3 center = (to+from) *0.5f;
        int i,j;

        Vector3 edgecoord(1.f,1.f,1.f),pa,pb;
        for (i=0;i<4;i++)
        {
            for (j=0;j<3;j++)
            {
                pa = Vector3(edgecoord[0]*halfExtents[0], edgecoord[1]*halfExtents[1],		
                    edgecoord[2]*halfExtents[2]);
                pa+=center;

                int othercoord = j%3;
                edgecoord[othercoord]*=-1.f;
                pb = Vector3(edgecoord[0]*halfExtents[0], edgecoord[1]*halfExtents[1],	
                    edgecoord[2]*halfExtents[2]);
                pb+=center;

                drawLine(pa,pb,color);
            }
            edgecoord = Vector3(-1.f,-1.f,-1.f);
            if (i<3)
                edgecoord[i]*=-1.f;
        }
    }
}
//------------------------------------------------------------------------------------------------
void DebugDrawer::drawLine(const Ogre::Vector3& from,const Ogre::Vector3& to,const Ogre::Vector3& color)
{
    if (mDebugMode > 0)
    {
        addLine (from, to);
    }
}
//------------------------------------------------------------------------------------------------
void DebugDrawer::drawContactPoint(const Ogre::Vector3& PointOnB,const Ogre::Vector3& normalOnB,
                                   Real distance,int lifeTime,const Ogre::Vector3& color)
{
    if (mDebugMode & btIDebugDraw::DBG_DrawContactPoints)
    {

    }
    //...
}
//------------------------------------------------------------------------------------------------

