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

#include "OgreBulletCollisionsObject.h"
#include "Debug/OgreBulletCollisionsDebugShape.h"

#include "OgreBulletCollisionsObjectState.h"
#include "OgreBulletCollisionsShape.h"
#include "OgreBulletCollisionsWorld.h"

using namespace Ogre;
using namespace OgreBulletCollisions;

#include "BulletCollision/CollisionDispatch/btInternalEdgeUtility.h"
extern ContactAddedCallback             gContactAddedCallback;

static bool CustomMaterialCombinerCallback(btManifoldPoint& cp, const btCollisionObject* colObj0,int partId0,int index0,const btCollisionObject* colObj1,int partId1,int index1)
{

        btAdjustInternalEdgeContacts(cp, colObj1, colObj0, partId1, index1);
        //btAdjustInternalEdgeContacts(cp,colObj1,colObj0, partId1,index1, BT_TRIANGLE_CONVEX_BACKFACE_MODE);
        //btAdjustInternalEdgeContacts(cp,colObj1,colObj0, partId1,index1, BT_TRIANGLE_CONVEX_DOUBLE_SIDED+BT_TRIANGLE_CONCAVE_DOUBLE_SIDED);
		return true;
}

namespace OgreBulletCollisions
{
    //-----------------------------------------------------------------------
    const Ogre::String Object::mMovableType = "OgreBullet::Object";
    // -------------------------------------------------------------------------
    Object::Object(const String &name, CollisionsWorld *world, bool init)
        :	
        MovableObject(name),
        mWorld(world),
        mShape(0),
        mState(0),
        mRootNode(0),
        mBounds(Vector3::ZERO, Vector3::ZERO),
        mDebugShape(0),
        mShapeNode(0),
        mDebugNode(0),
		is_static(false)
    {
        if (init)
        {
            mObject = new btCollisionObject();
            mState = new ObjectState(this);
        }
	gContactAddedCallback = CustomMaterialCombinerCallback;
    }
    // -------------------------------------------------------------------------
    Object::~Object()
    {
        if (mRootNode)
        {
            showDebugShape(false);
            //mShapeNode->detachObject (this);
            //mRootNode->removeAndDestroyChild (mShapeNode->getName ());
            //mRootNode->getParentSceneNode ()->removeAndDestroyChild (mRootNode->getName ());
        }

        getBulletCollisionWorld()->removeCollisionObject( mObject );
		getCollisionWorld()->removeObject(this);

        delete mObject;        
        //delete mShape;
        delete mState;
        delete mDebugShape;
    }
    //-----------------------------------------------------------------------
    void Object::showDebugShape(bool show)
    {
        if (show && mDebugShape == 0 && mShape)
        {
            mDebugShape = new DebugCollisionShape(mShape);
            if (mDebugShape->getIsVisual ())
            {
                assert (mDebugNode == 0);
                mDebugNode = mRootNode->createChildSceneNode(mName + "DebugShape");
                mDebugNode->setInheritScale(false);
                mDebugNode->attachObject (mDebugShape);
            }
        }
        else if (show != true && mDebugShape != 0)
        {
            if (mDebugShape->getIsVisual ())
            {
                assert (mDebugNode);
                mDebugNode->detachObject (mDebugShape->getName());
                mRootNode->removeAndDestroyChild (mDebugNode->getName());
                mDebugNode = 0;
            }
            assert (mDebugNode == 0);
            delete mDebugShape;
            mDebugShape = 0;
        }
    }
    // -------------------------------------------------------------------------
    void Object::setTransform(const btVector3 &pos, const btQuaternion &quat)
    { 
        mRootNode->setPosition(pos[0], pos[1], pos[2]);
        mRootNode->setOrientation(quat.getW(),quat.getX(), quat.getY(), quat.getZ());
    }
    // -------------------------------------------------------------------------
    void Object::setPosition(const btVector3 &pos)
    {
		mRootNode->setPosition(pos[0], pos[1], pos[2]);
    }
    // -------------------------------------------------------------------------
    void Object::setOrientation(const btQuaternion &quat)
    {   
		mRootNode->setOrientation(quat.getW(),quat.getX(), quat.getY(), quat.getZ());
    }
    // -------------------------------------------------------------------------
    void Object::setTransform(const btTransform& worldTrans)
    { 
		//this sets the node's transformation based on the Bullet collider's world transformation
        mRootNode->setPosition(worldTrans.getOrigin()[0], worldTrans.getOrigin()[1],worldTrans.getOrigin()[2]);
        mRootNode->setOrientation(worldTrans.getRotation().getW(),worldTrans.getRotation().getX(), worldTrans.getRotation().getY(), worldTrans.getRotation().getZ());
    }
    //-----------------------------------------------------------------------
    void Object::setShape(CollisionShape *shape)
    {
        mShape = shape;

        //mRootNode = mWorld->getSceneManager()->getRootSceneNode()->createChildSceneNode(mName);
        //mShapeNode = mRootNode->createChildSceneNode(mName + "Shape");
        //mShapeNode->attachObject(this);

        mObject->setCollisionShape(shape->getBulletShape());
        showDebugShape(mWorld->getShowDebugShapes()); 

		_notifyMoved();
    }
    // -------------------------------------------------------------------------
    //-----------------------------------------------------------------------
    void Object::_notifyAttached(Node* parent, bool isTagPoint)
    {
        MovableObject::_notifyAttached(parent,isTagPoint);
        if (parent)
        {
            Object* other_object = mWorld->findObject(static_cast<SceneNode*>(parent));
            if ((other_object) && (other_object != this))
            {
                static_cast<SceneNode*>(parent)->detachObject(other_object);

            }
            //setPosition(parent->getPosition());
            //setOrientation(parent->getOrientation());
        } 
    }
#if (OGRE_VERSION >=  ((1 << 16) | (5 << 8) | 0)) // must have at least shoggoth (1.5.0)
    //-----------------------------------------------------------------------
	void Object::visitRenderables(Renderable::Visitor* visitor, 
		bool debugRenderables)
	{
		//visitor->visit(this, 0, false);
	}
#endif
    //-----------------------------------------------------------------------
    const Ogre::String& Object::getMovableType() const
    {
        return mMovableType;
    }

    //-----------------------------------------------------------------------
    void Object::_notifyCurrentCamera(Camera* camera)
    {
    }

	void Object::_notifyMoved()
	{
		if (is_static == true)
			updateTransform(false);
	}

	void Object::updateTransform(bool set_interpolate)
	{
		Vector3 pos = mRootNode->getPosition();
		Quaternion quat = mRootNode->getOrientation();
		btTransform transform;
		transform.setOrigin(btVector3(pos.x, pos.y, pos.z));
		transform.setRotation(btQuaternion(quat.x, quat.y, quat.z, quat.w));
		mObject->setWorldTransform(transform);
		if (set_interpolate == true)
			mObject->setInterpolationWorldTransform(transform);
	}

    //-----------------------------------------------------------------------
    const AxisAlignedBox& Object::getBoundingBox(void) const
    {
        return mBounds;
    }
    //-----------------------------------------------------------------------
    Real Object::getBoundingRadius(void) const
    {
        return Ogre::Real(0.0);
    }

    //-----------------------------------------------------------------------
    void Object::_updateRenderQueue(RenderQueue* queue)
    {
    }

	void Object::enableCollisions(bool value)
	{
		if (value == true)
		{
			//mObject->setCollisionFlags(mObject->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
			mObject->getBroadphaseHandle()->m_collisionFilterGroup = btBroadphaseProxy::DefaultFilter;
			mObject->getBroadphaseHandle()->m_collisionFilterMask = btBroadphaseProxy::AllFilter;
		}
		else
		{
			//mObject->setCollisionFlags(mObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
			mObject->getBroadphaseHandle()->m_collisionFilterGroup = btBroadphaseProxy::AllFilter;
			mObject->getBroadphaseHandle()->m_collisionFilterMask = 0;
		}
	}
}

