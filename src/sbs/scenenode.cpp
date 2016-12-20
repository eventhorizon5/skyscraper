/* $Id$ */

/*
	Scalable Building Simulator - SceneNode Object
	The Skyscraper Project - Version 1.11 Alpha
	Copyright (C)2004-2016 Ryan Thoryk
	http://www.skyscrapersim.com
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@skyscrapersim.com

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include "globals.h"
#include "sbs.h"
#include "scenenode.h"

namespace SBS {

SceneNode::SceneNode(Object *parent, std::string name) : ObjectBase(parent)
{
	SetName(name);
	node = 0;
	Rotation = Ogre::Vector3::ZERO;

	if (!sbs->mSceneManager || !parent)
		return;

	//set up scene node
	std::string node_name = GetNameBase() + name;

	try
	{
		node = sbs->mSceneManager->createSceneNode(node_name);
	}
	catch (Ogre::Exception &e)
	{
		sbs->ReportError("Error creating scene node:\n" + e.getDescription());
		return;
	}

	//attach scene node to root, if no parent exists (for engine root object)
	if (!parent->GetParent() && !parent->GetSceneNode())
		sbs->mSceneManager->getRootSceneNode()->addChild(node);
}

SceneNode::~SceneNode()
{
	if (node)
		sbs->mSceneManager->destroySceneNode(node);
}

void SceneNode::DetachAllObjects()
{
	//detach all movable objects from this scene node

	if (node)
		node->detachAllObjects();
}

void SceneNode::AddChild(SceneNode *scenenode)
{
	//add a child scene node

	if (node && scenenode && scenenode->GetRawSceneNode())
		node->addChild(scenenode->GetRawSceneNode());
}

void SceneNode::RemoveChild(SceneNode *scenenode)
{
	//remove a child scene node

	if (node && scenenode)
	{
		Ogre::SceneNode *rawnode = scenenode->GetRawSceneNode();
		if (rawnode)
			node->removeChild(rawnode);
	}
}

void SceneNode::ShowBoundingBox(bool value)
{
	//show scene node's 3D bounding box

	if (node)
		node->showBoundingBox(value);
}

void SceneNode::SetPosition(const Ogre::Vector3 &position)
{
	//set position of scene node

	if (!node)
		return;

	if (IsRoot() == false)
		node->_setDerivedPosition(sbs->ToRemote(sbs->ToGlobal(position)));
	else
		node->_setDerivedPosition(sbs->ToRemote(position));
	Update();
}

void SceneNode::SetPositionRelative(const Ogre::Vector3 &position)
{
	//set position of scene node
	//position is relative of parent scene node

	if (!node)
		return;

	Ogre::Vector3 pos = sbs->ToRemote(position);
	node->setPosition(pos);
	Update();
}

Ogre::Vector3 SceneNode::GetPosition(bool relative)
{
	//get position of scene node
	//if relative is true, position is relative of parent scene node

	if (!node)
		return Ogre::Vector3::ZERO;

	if (relative == false)
	{
		Ogre::Vector3 pos = sbs->ToLocal(node->_getDerivedPosition());
		if (IsRoot() == false)
			return sbs->FromGlobal(pos);
		else
			return pos;
	}

	return sbs->ToLocal(node->getPosition());
}

void SceneNode::SetRotation(const Ogre::Vector3 &rotation)
{
	//set rotation of scene node in degrees
	//this sets the rotation of all three vectors

	if (!node)
		return;

	Rotation = rotation;

	if (Rotation.x > 359)
		Rotation.x -= 360;
	if (Rotation.y > 359)
		Rotation.y -= 360;
	if (Rotation.z > 359)
		Rotation.z -= 360;
	if (Rotation.x < 0)
		Rotation.x += 360;
	if (Rotation.y < 0)
		Rotation.y += 360;
	if (Rotation.z < 0)
		Rotation.z += 360;

	Ogre::Quaternion x(Ogre::Degree(Rotation.x), Ogre::Vector3::UNIT_X);
	Ogre::Quaternion y(Ogre::Degree(Rotation.y), Ogre::Vector3::NEGATIVE_UNIT_Y);
	Ogre::Quaternion z(Ogre::Degree(Rotation.z), Ogre::Vector3::UNIT_Z);
	Ogre::Quaternion rot = x * y * z;

	SetOrientation(rot, true);
}

Ogre::Vector3 SceneNode::GetRotation()
{
	//get rotation of scene node in degrees
	//this returns the rotation of all three vectors

	return Rotation;
}

void SceneNode::Update()
{
	//sync positioning
	//this mainly needs to be called on child scenenodes of a parent that has moved/rotated,
	//to recalculate offsets

	if (node)
		node->needUpdate();
}

Ogre::Quaternion SceneNode::GetOrientation(bool relative)
{
	//return raw orientation

	if (node)
	{
		if (relative == false)
		{
			if (IsRoot() == false)
				return sbs->FromGlobal(node->_getDerivedOrientation());
			else
				return node->_getDerivedOrientation();
		}
		else
			return node->getOrientation();
	}

	return Ogre::Quaternion::ZERO;
}

void SceneNode::SetOrientation(const Ogre::Quaternion &q, bool relative)
{
	//set raw orientation

	if (!node)
		return;

	if (relative == false)
	{
		if (IsRoot() == false)
			node->_setDerivedOrientation(sbs->ToGlobal(q));
		else
			node->_setDerivedOrientation(q);
	}
	else
		node->setOrientation(q);

	Update();
}

void SceneNode::Move(const Ogre::Vector3 &vector, float speed)
{
	//move this scene node

	if (!node)
		return;

	Ogre::Vector3 v = vector * speed;
	node->translate(sbs->ToRemote(v));

	Update();
}

void SceneNode::AttachObject(Ogre::MovableObject *object)
{
	//attach a movable object to this node

	if (node && object)
	{
		try
		{
			node->attachObject(object);
		}
		catch (Ogre::Exception &e)
		{
			sbs->ReportError("Error attaching object:\n" + e.getDescription());
		}
	}
}

void SceneNode::DetachObject(Ogre::MovableObject *object)
{
	//detach a movable object from this node

	if (node && object)
	{
		try
		{
			node->detachObject(object);
		}
		catch (Ogre::Exception &e)
		{
			sbs->ReportError("Error detaching object:\n" + e.getDescription());
		}
	}
}

float SceneNode::GetScale()
{
	//get scaling factor

	if (node)
		return node->getScale().x;
	return 0.0f;
}

void SceneNode::SetScale(float scale)
{
	//set scaling factor

	if (!node)
		return;

	node->setScale(Ogre::Vector3(scale, scale, scale));
}

SceneNode* SceneNode::CreateChild(std::string name, const Ogre::Vector3 &offset)
{
	//create a raw child scenenode, at the specified offset

	if (!node || !GetParent())
		return 0;

	SceneNode *scenenode = new SceneNode(GetParent(), name);
	node->addChild(scenenode->GetRawSceneNode());
	scenenode->Move(offset);

	return scenenode;
}

std::string SceneNode::GetFullName()
{
	//get full name of a scene node, which includes object ID information

	if (!node)
		return "";

	return node->getName();
}

bool SceneNode::IsRoot()
{
	 return (this == sbs->GetSceneNode());
}

}
