/*
	Scalable Building Simulator - SceneNode Object
	The Skyscraper Project - Version 1.12 Alpha
	Copyright (C)2004-2024 Ryan Thoryk
	https://www.skyscrapersim.net
	https://sourceforge.net/projects/skyscraper/
	Contact - ryan@skyscrapersim.net

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
	Rotation = Vector3::ZERO;

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
		ReportError("Error creating scene node:\n" + e.getDescription());
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

void SceneNode::SetPosition(const Vector3 &position)
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

void SceneNode::SetPositionRelative(const Vector3 &position)
{
	//set position of scene node
	//position is relative of parent scene node

	if (!node)
		return;

	Vector3 pos = sbs->ToRemote(position);
	node->setPosition(pos);
	Update();
}

Vector3 SceneNode::GetPosition(bool relative)
{
	//get position of scene node
	//if relative is true, position is relative of parent scene node

	if (!node)
		return Vector3::ZERO;

	if (relative == false)
	{
		Vector3 pos = GetDerivedPosition();
		if (IsRoot() == false)
			return sbs->FromGlobal(pos);
		else
			return pos;
	}

	return sbs->ToLocal(node->getPosition());
}

void SceneNode::SetRotation(const Vector3 &rotation)
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

	Quaternion x(Degree(Rotation.x), Vector3::UNIT_X);
	Quaternion y(Degree(Rotation.y), Vector3::NEGATIVE_UNIT_Y);
	Quaternion z(Degree(Rotation.z), Vector3::UNIT_Z);
	Quaternion rot = x * y * z;

	SetOrientation(rot, true);
}

Vector3 SceneNode::GetRotation()
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

Quaternion SceneNode::GetOrientation(bool relative)
{
	//return raw orientation

	if (node)
	{
		if (relative == false)
		{
			if (IsRoot() == false)
				return sbs->FromGlobal(GetDerivedOrientation());
			else
				return GetDerivedOrientation();
		}
		else
			return node->getOrientation();
	}

	return Quaternion::ZERO;
}

void SceneNode::SetOrientation(const Quaternion &q, bool relative)
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

void SceneNode::Move(const Vector3 &vector, Real speed)
{
	//move this scene node

	if (!node)
		return;

	Vector3 v = vector * speed;
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
			ReportError("Error attaching object:\n" + e.getDescription());
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
			if (sbs->Verbose)
				ReportError("Error detaching object:\n" + e.getDescription());
		}
	}
}

Real SceneNode::GetScale()
{
	//get scaling factor

	if (node)
		return node->getScale().x;
	return 0.0;
}

void SceneNode::SetScale(Real scale)
{
	//set scaling factor

	if (!node)
		return;

	node->setScale(Vector3(scale, scale, scale));
}

SceneNode* SceneNode::CreateChild(std::string name, const Vector3 &offset)
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

void SceneNode::Pitch(Real &degree)
{
	//rotate on X axis

	node->pitch(Degree(degree));
}

void SceneNode::Yaw(Real &degree)
{
	//rotate on Y axis

	node->yaw(Degree(-degree));
}

void SceneNode::Roll(Real &degree)
{
	//rotate on Z axis

	node->roll(Degree(degree));
}

Vector3 SceneNode::GetDerivedPosition()
{
	//gets the position of the node as derived from all parents
	return sbs->ToLocal(node->_getDerivedPosition());
}

Quaternion SceneNode::GetDerivedOrientation()
{
	//gets the orientation of the node as derived from all parents

	return node->_getDerivedOrientation();
}

void SceneNode::SetDirection(const Vector3 &direction)
{
	node->setDirection(sbs->ToRemote(direction));
}

void SceneNode::LookAt(const Vector3 &point)
{
	node->lookAt(sbs->ToRemote(point), Ogre::Node::TS_PARENT);
}

}
