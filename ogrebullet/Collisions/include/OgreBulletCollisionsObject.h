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

#ifndef _OGREBULLETCOLLISIONS_CollisionObject_H
#define _OGREBULLETCOLLISIONS_CollisionObject_H

#include "OgreBulletCollisionsPreRequisites.h"

#include "OgreBulletCollisionsWorld.h"

namespace OgreBulletCollisions
{
    // -------------------------------------------------------------------------
    /*!
	 * \brief
	 *
	 * Object is the Basic Bullet Collision representation of an Physical thing in
     * a scene. It does need a Shape Object to know its "Geometrics" Bounds.
	 *
	 * \remarks
	 * Objects doesn't need to be represented by a Visible 3D mesh.
	 *
	 * \see
	 * Ogre::MovableObject | Ogre::UserDefinedObject | OgreBulletDynamics::RigidBody
	 */
	class _OgreBulletExport Object : public Ogre::MovableObject
    {
    public:
        Object(const Ogre::String &name,  CollisionsWorld *world, bool init);

        virtual ~Object();

        // override Movables
#if (OGRE_VERSION >=  ((1 << 16) | (5 << 8) | 0)) // must have at least shoggoth (1.5.0)
		void visitRenderables(Ogre::Renderable::Visitor* visitor, bool debugRenderables);
#endif
        virtual const Ogre::String& getMovableType() const;
        virtual void _notifyAttached(Ogre::Node* parent,bool isTagPoint = false);
        //virtual const Ogre::String& getName(void) const {return mName};
        virtual void _notifyCurrentCamera(Ogre::Camera* camera);
		virtual void _notifyMoved();
        virtual const Ogre::AxisAlignedBox& getBoundingBox(void) const;
        virtual Ogre::Real getBoundingRadius(void) const;
        virtual void _updateRenderQueue(Ogre::RenderQueue* queue);
		void updateTransform(bool update_pos, bool update_rot, bool set_interpolate);


        inline const Ogre::Vector3 getWorldPosition() const {return mRootNode->_getDerivedPosition();};
		inline const Ogre::Quaternion &getWorldOrientation() const {return mRootNode->_getDerivedOrientation();};

        void setPosition(const Ogre::Vector3 &p);
        void setOrientation(const Ogre::Quaternion &q);

        inline void setPosition(const Ogre::Real x, const Ogre::Real y, const Ogre::Real z) {setPosition(Ogre::Vector3(x, y, z));};
        inline void setOrientation(const Ogre::Real x, const Ogre::Real y, const Ogre::Real z, const Ogre::Real w)  {setOrientation(Ogre::Quaternion(x,y,z,w));};

        virtual void setPosition(const btVector3 &pos);
        virtual void setOrientation(const btQuaternion &quat);
        virtual void setTransform(const btVector3 &pos, const btQuaternion &quat);
        virtual void setTransform(const btTransform& worldTrans);

        inline btCollisionObject*  getBulletObject() const { return mObject;};
        inline btCollisionWorld*  getBulletCollisionWorld() const { return mWorld->getBulletCollisionWorld ();};
        inline CollisionsWorld*  getCollisionWorld() const { return mWorld;};

        inline CollisionShape *getShape() const{ return mShape;};
        inline DebugCollisionShape* getDebugShape() const{ return mDebugShape;};

        void setShape(CollisionShape *shape);
        void showDebugShape(bool show);

        void setCollisionMask(short CollisionGroup, short CollisionMask);

        Ogre::SceneNode *getRootNode() { return mRootNode; }

        Object* getLastCollision();
        void resetLastCollision();
        void updateBoundingBox();

    protected:

        Ogre::SceneNode*        mRootNode;
        Ogre::SceneNode*        mShapeNode;
        Ogre::SceneNode*        mDebugNode;

        ObjectState   *         mState;
        CollisionsWorld*        mWorld;

        btCollisionObject*      mObject;

        Ogre::AxisAlignedBox    mBounds;

        CollisionShape*         mShape;
        DebugCollisionShape *   mDebugShape;
		bool					is_static;
		bool					update_parent;

    public:
        static const Ogre::String mMovableType;
    };
    // -------------------------------------------------------------------------
}
#endif //_OGREBULLETCOLLISIONS_CollisionObject_H

