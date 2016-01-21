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
static Object* last_collision;

static bool CustomMaterialCombinerCallback(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0,int partId0,int index0,const btCollisionObjectWrapper* colObj1,int partId1,int index1)
{

        //btAdjustInternalEdgeContacts(cp, colObj1, colObj0, partId1, index1);
        //btAdjustInternalEdgeContacts(cp,colObj1,colObj0, partId1,index1, BT_TRIANGLE_CONVEX_BACKFACE_MODE);
        //btAdjustInternalEdgeContacts(cp,colObj1,colObj0, partId1,index1, BT_TRIANGLE_CONVEX_DOUBLE_SIDED+BT_TRIANGLE_CONCAVE_DOUBLE_SIDED);

	last_collision = (Object*)colObj1->getCollisionObject()->getUserPointer();

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
		is_static(false),
    	     update_parent(false)
    {
        if (init)
        {
            mObject = new btCollisionObject();
            mState = new ObjectState(this);
	    mObject->setUserPointer(this);
        }
	gContactAddedCallback = CustomMaterialCombinerCallback;
	last_collision = 0;
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

		if (mObject)
			delete mObject;
        mObject = 0;
        if (mShape)
        	delete mShape;
        mShape = 0;
        if (mState)
        	delete mState;
        mState = 0;
        if (mDebugShape)
        	delete mDebugShape;
        mDebugShape = 0;
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
		setPosition(pos);
		setOrientation(quat);
    }
    // -------------------------------------------------------------------------
    void Object::setPosition(const btVector3 &pos)
    {
	    Ogre::Vector3 position (pos[0], pos[1], pos[2]);
	    setPosition(position);
    }
    // -------------------------------------------------------------------------
    void Object::setPosition(const Ogre::Vector3 &p)
    {
   		if (update_parent == true)
   		{
   			Ogre::SceneNode *mesh_node = mRootNode->getParentSceneNode(); //mesh (parent) scene node
   			if (!mesh_node)
   				return;

   			Ogre::SceneNode *object_node = mesh_node->getParentSceneNode(); //SBS object scene node (mesh parent)
   			if (!object_node)
   				return;

   			//get offset of this node relative to parent, and adjust parent according to that offset
   			Ogre::Vector3 offset = mRootNode->_getDerivedPosition() - mesh_node->_getDerivedPosition();
   			Ogre::Vector3 offset_top = mesh_node->_getDerivedPosition() - object_node->_getDerivedPosition();

   			object_node->_setDerivedPosition(p - offset - offset_top); //object scenenode
   			mesh_node->_setDerivedPosition(p - offset); //mesh scenenode
   		}

		mRootNode->_setDerivedPosition(p);

	    updateBoundingBox();
    }
    // -------------------------------------------------------------------------
    void Object::setOrientation(const btQuaternion &quat)
    {
	    Ogre::Quaternion q (quat.getW(),quat.getX(), quat.getY(), quat.getZ());
	    setOrientation(q);
    }
    // -------------------------------------------------------------------------
    void Object::setOrientation(const Ogre::Quaternion &q)
    {
	    if (update_parent == true)
	    {
   			Ogre::SceneNode *mesh_node = mRootNode->getParentSceneNode(); //mesh (parent) scene node
   			if (!mesh_node)
   				return;

   			Ogre::SceneNode *object_node = mesh_node->getParentSceneNode(); //SBS object scene node (mesh parent)
   			if (!object_node)
   				return;

		    object_node->_setDerivedOrientation(q);
		    mesh_node->_setDerivedOrientation(q);
	    }

	    updateBoundingBox();
	    mRootNode->_setDerivedOrientation(q);
    }
    // -------------------------------------------------------------------------
    void Object::setTransform(const btTransform& worldTrans)
    {
		//this sets the node's transformation based on the Bullet collider's world transformation
	    Ogre::Vector3 position (worldTrans.getOrigin()[0], worldTrans.getOrigin()[1], worldTrans.getOrigin()[2]);

	    setPosition(position);
	    setOrientation(worldTrans.getRotation().getW(),worldTrans.getRotation().getX(), worldTrans.getRotation().getY(), worldTrans.getRotation().getZ());
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
			updateTransform(true, true, false);
	}

	void Object::updateTransform(bool update_pos, bool update_rot, bool set_interpolate)
	{
		Vector3 pos;
		Quaternion quat;
		if (update_pos == true)
			pos = mRootNode->_getDerivedPosition();
		if (update_rot == true)
			quat = mRootNode->_getDerivedOrientation();

		btTransform transform = mObject->getWorldTransform();
		if (update_pos == true)
			transform.setOrigin(btVector3(pos.x, pos.y, pos.z));
		if (update_rot == true)
			transform.setRotation(btQuaternion(quat.x, quat.y, quat.z, quat.w));
		mObject->setWorldTransform(transform);
		if (set_interpolate == true)
			mObject->setInterpolationWorldTransform(transform);

		updateBoundingBox();
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

	void Object::setCollisionMask(short CollisionGroup, short CollisionMask)
	{
		//mObject->setCollisionFlags(mObject->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
		mObject->getBroadphaseHandle()->m_collisionFilterGroup = CollisionGroup;
		mObject->getBroadphaseHandle()->m_collisionFilterMask = CollisionMask;
	}

	Object* Object::getLastCollision()
	{
		return last_collision;
	}

	void Object::resetLastCollision()
	{
		last_collision = 0;
	}

	void Object::updateBoundingBox()
	{
		//update bounding box if a static object and in the world
		if (mObject->isStaticOrKinematicObject() && mObject->getBroadphaseHandle())
			mWorld->getBulletCollisionWorld()->updateSingleAabb(mObject);
	}
}

