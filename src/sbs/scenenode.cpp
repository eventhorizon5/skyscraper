/* $Id$ */

/*
	Scalable Building Simulator - SceneNode Object
	The Skyscraper Project - Version 1.10 Alpha
	Copyright (C)2004-2015 Ryan Thoryk
	http://www.skyscrapersim.com
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@tliquest.net

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

#include "globals.h"
#include "sbs.h"
#include "unix.h"
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
	std::string node_name = "(" + ToString(parent->GetNumber()) + ")" + name;

	node = sbs->mSceneManager->createSceneNode(node_name);

	//attach scene node to root, if no parent exists (for engine root object)
	if (!parent->GetParent())
		sbs->mSceneManager->getRootSceneNode()->addChild(node);
}

SceneNode::~SceneNode()
{
	if (sbs->FastDelete == false && node)
		sbs->mSceneManager->destroySceneNode(node);
}

void SceneNode::DetachAllObjects()
{
	if (node)
		node->detachAllObjects();
}

void SceneNode::AddChild(SceneNode *scenenode)
{
	if (node && scenenode && scenenode->GetRawSceneNode())
		node->addChild(scenenode->GetRawSceneNode());
}

void SceneNode::RemoveChild(SceneNode *scenenode)
{
	if (node && scenenode)
	{
		Ogre::SceneNode *rawnode = scenenode->GetRawSceneNode();
		if (rawnode)
			node->removeChild(rawnode);
	}
}

void SceneNode::AddToSceneRoot()
{
	if (node)
		sbs->mSceneManager->getRootSceneNode()->addChild(node);
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

	Ogre::Vector3 pos = sbs->ToRemote(position);
	node->_setDerivedPosition(pos);
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
		return sbs->ToLocal(node->_getDerivedPosition());

	return sbs->ToLocal(node->getPosition());
}

void SceneNode::SetRotation(const Ogre::Vector3 &rotation)
{
	//set rotation of scene node in degrees

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
		Rotation += 360;
	if (Rotation.z < 0)
		Rotation.z += 360;

	Ogre::Quaternion x(Ogre::Degree(Rotation.x), Ogre::Vector3::UNIT_X);
	Ogre::Quaternion y(Ogre::Degree(Rotation.y), Ogre::Vector3::NEGATIVE_UNIT_Y);
	Ogre::Quaternion z(Ogre::Degree(Rotation.z), Ogre::Vector3::UNIT_Z);
	Ogre::Quaternion rot = x * y * z;

	node->setOrientation(rot);
	Update();
}

Ogre::Vector3 SceneNode::GetRotation()
{
	//get rotation of scene node in degrees

	return Rotation;
}

void SceneNode::Update()
{
	//sync positioning
	if (node)
		node->needUpdate();
}

Ogre::Quaternion SceneNode::GetOrientation()
{
	//return raw orientation

	if (node)
		return node->_getDerivedOrientation();

	return Ogre::Quaternion::ZERO;
}

void SceneNode::SetOrientation(const Ogre::Quaternion &q)
{
	//set raw orientation

	if (!node)
		return;

	node->_setDerivedOrientation(q);

	Update();
}

void SceneNode::Move(const Ogre::Vector3 &vector, float speed)
{
	if (!node)
		return;

	Ogre::Vector3 v = vector * speed;
	node->translate(sbs->ToRemote(v));

	Update();
}

void SceneNode::AttachObject(Ogre::MovableObject *object)
{
	if (node && object)
		node->attachObject(object);
}

void SceneNode::DetachObject(Ogre::MovableObject *object)
{
	if (node && object)
		node->detachObject(object);
}

float SceneNode::GetScale()
{
	if (node)
		return node->getScale().x;
	return 0.0f;
}

void SceneNode::SetScale(float scale)
{
	if (!node)
		return;

	scale = sbs->ToRemote(scale);
	node->setScale(Ogre::Vector3(scale, scale, scale));
}

SceneNode* SceneNode::CreateChild(std::string name, const Ogre::Vector3 &offset)
{
	if (!node || !GetParent())
		return 0;

	SceneNode *scenenode = new SceneNode(GetParent(), name);
	node->addChild(scenenode->GetRawSceneNode());
	scenenode->Move(offset);

	return scenenode;
}

std::string SceneNode::GetFullName()
{
	if (!node)
		return "";

	return node->getName();
}

}
