/* $Id$ */

/*
	Scalable Building Simulator - Generic Object Class
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

namespace SBS {

ObjectBase::ObjectBase(Object *parent)
{
	Parent = parent;
	sbs = 0;

	if (parent)
		sbs = parent->GetRoot();
}

Object* ObjectBase::GetParent()
{
	//return parent object
	return Parent;
}

const std::string& ObjectBase::GetName()
{
	//return object name
	return Name;
}

void ObjectBase::SetName(const std::string &name)
{
	//set object name
	Name = name;
}

Object::Object(Object *parent, bool temporary) : ObjectBase(parent)
{
	Permanent = false;
	linenum = 0;
	Number = -1;
	Temporary = temporary;
	parent_deleting = false;
	node = 0;
	values_set = false;

	//register object with engine
	if (temporary == false)
	{
		if (parent)
			Number = sbs->RegisterObject(this);
		else
			Number = (int)ObjectArray.size(); //root object
	}
}

Object::~Object()
{
	//remove object from engine

	//exit if at end of engine deletion
	if (!sbs)
		return;

	//clean up scene node
	if (node)
		node->DetachAllObjects();

	if (sbs->FastDelete == false)
	{
		//unregister this object from parent
		if (Parent && sbs->FastDelete == false)
			Parent->RemoveChild(this);
	}

	//delete scene node
	if (node)
		delete node;
	node = 0;

	//if fastdelete is enabled, don't unregister (just delete)
	if (sbs->FastDelete == true || Temporary == true)
		return;

	sbs->UnregisterObject(Number);
	sbs->Report("Deleted object " + ToString(Number) + ": " + Name);
}

void Object::SetValues(const std::string &type, const std::string &name, bool is_permanent, bool is_movable)
{
	//set object values

	//exit if settings have already been set
	if (values_set == true)
		return;

	values_set = true;
	Permanent = is_permanent;
	Type = type;
	Name = name;

	//create scene node object
	if (is_movable == true)
		node = new SceneNode(this, Name);

	//register as child object if object has a valid parent
	if (Parent)
		Parent->AddChild(this);
}

bool Object::IsPermanent()
{
	//return permanent state
	return Permanent;
}

bool Object::IsMovable()
{
	return node != 0;
}

const std::string& Object::GetType()
{
	//return object type string
	return Type;
}

int Object::GetNumber()
{
	//return object's global numeric identifier
	return Number;
}

void Object::AddChild(Object *object)
{
	//add a child object to the internal array
	children.push_back(object);

	//add child's scene node
	if (object->GetSceneNode())
	{
		if (node)
			node->AddChild(object->GetSceneNode());
		else if (object->GetSceneNode())
			//if parent doesn't have a scenenode, but child does, add child to root node
			object->GetSceneNode()->AddToSceneRoot();
	}
}

void Object::RemoveChild(Object *object)
{
	//remove a child object in the internal array
	if (GetChildrenCount() > 0)
	{
		for (int i = 0; i < GetChildrenCount(); i++)
		{
			if (children[i] == object)
			{
				children.erase(children.begin() + i);
				break;
			}
		}
	}

	//remove child's scene node
	if (node)
		node->RemoveChild(object->GetSceneNode());
}

Object* Object::GetChild(int index)
{
	//return pointer to child object from an array index
	if (index < (int)children.size())
		return children[index];
	else
		return 0;
}

int Object::GetChildrenCount()
{
	//return number of child objects
	return (int)children.size();
}

SceneNode* Object::GetSceneNode()
{
	return node;
}

void Object::SetNumber(int number)
{
	//set object's number - will only work if object is temporary

	if (Temporary == true)
		Number = number;
}

bool Object::IsTemporary()
{
	return Temporary;
}

void Object::ShowBoundingBox(bool value)
{
	//show object's 3D bounding box

	if (node)
		node->ShowBoundingBox(value);
}

void Object::Move(const Ogre::Vector3 &vector, float speed)
{
	//move an object

	if (!node)
		return;

	node->Move(vector, speed);

	//notify about movement
	NotifyMove();
}

void Object::Move(float X, float Y, float Z, float speed)
{
	Ogre::Vector3 pos (X, Y, Z);
	Move(pos, speed);
}

void Object::SetPosition(const Ogre::Vector3 &position)
{
	//set position of object

	if (!node)
		return;

	node->SetPosition(position);

	//notify about movement
	NotifyMove();
}

void Object::SetPositionRelative(const Ogre::Vector3 &position)
{
	//set position of object
	//position is relative of parent object

	if (!node)
		return;

	node->SetPositionRelative(position);

	//notify about movement
	NotifyMove();
}

void Object::SetPosition(float X, float Y, float Z)
{
	Ogre::Vector3 pos (X, Y, Z);
	SetPosition(pos);
}

void Object::SetPositionRelative(float X, float Y, float Z)
{
	Ogre::Vector3 pos (X, Y, Z);
	SetPositionRelative(pos);
}

void Object::SetPositionY(float value)
{
	//set position of only Y vector

	Ogre::Vector3 pos (GetPosition().x, value, GetPosition().z);
	SetPosition(pos);
}

Ogre::Vector3 Object::GetPosition(bool relative)
{
	//get position of object
	//if relative is true, position is relative of parent object

	if (!node)
		return Ogre::Vector3::ZERO;

	return node->GetPosition(relative);
}

void Object::SetRotation(const Ogre::Vector3 &rotation)
{
	//rotate object

	if (!node)
		return;

	node->SetRotation(rotation);

	//notify about rotation
	NotifyRotate();
}

void Object::SetRotation(float X, float Y, float Z)
{
	Ogre::Vector3 rot (X, Y, Z);
	SetRotation(rot);
}

void Object::Rotate(const Ogre::Vector3 &vector, float speed)
{
	//rotates object in a relative amount

	Ogre::Vector3 rot = GetRotation() + (vector * speed);
	SetRotation(rot);
}

void Object::Rotate(float X, float Y, float Z, float speed)
{
	Ogre::Vector3 rot (X, Y, Z);
	Rotate(rot, speed);
}

Ogre::Vector3 Object::GetRotation()
{
	//get rotation of object

	if (!node)
		return Ogre::Vector3::ZERO;

	return node->GetRotation();
}

Ogre::Quaternion Object::GetOrientation()
{
	if (!node)
		return Ogre::Quaternion::ZERO;

	return node->GetOrientation();
}

void Object::NotifyMove(bool parent)
{
	//notify about a move
	//if parent is true, this function was called from a parent object

	if (!node)
		return;

	//sync positioning, for child scene nodes
	node->Update();

	OnMove(parent);
	NotifyChildren(true, false);
}

void Object::NotifyRotate(bool parent)
{
	//notify about a rotate
	//if parent is true, this function was called from a parent object

	if (!node)
		return;

	//sync positioning, for child scene nodes
	node->Update();

	OnRotate(parent);
	NotifyChildren(false, true);
}

void Object::NotifyChildren(bool move, bool rotate)
{
	//notify child objects about a parent move or rotate

	int count = GetChildrenCount();

	if (count == 0)
		return;

	for (int i = 0; i < count; i++)
	{
		if (move == true)
			children[i]->NotifyMove(true);
		if (rotate == true)
			children[i]->NotifyRotate(true);
	}
}

void Object::ChangeParent(Object *new_parent)
{
	//change parent of object

	//require valid parents
	if (!Parent || !new_parent)
		return;

	if (new_parent == Parent)
		return;

	//get original positioning
	Ogre::Vector3 pos = GetPosition();

	//get original orientation
	Ogre::Quaternion q;
	if (node)
		q = node->GetOrientation();

	//switch parent
	Parent->RemoveChild(this);
	Parent = new_parent;
	Parent->AddChild(this);

	//restore absolute positioning
	SetPosition(pos);

	//restore orientation
	if (node)
	{
		node->SetOrientation(q);
		NotifyRotate();
	}

	sbs->Report("Changed parent of object '" + ToString(Number) + ": " + Name + "' to '" + ToString(new_parent->GetNumber()) + ": " + new_parent->GetName() + "'");
}

bool Object::IsGlobal()
{
	//returns true if object is a child of the SBS engine object (is a global object)

	if (Number == 0 || !Parent)
		return false;

	return (Parent->GetNumber() == 0);
}

void Object::Init()
{
	//initialize object

	//call custom object initialization code
	OnInit();

	//initialize children
	InitChildren();
}

void Object::InitChildren()
{
	//initialize child objects

	int count = GetChildrenCount();

	if (count == 0)
		return;

	for (int i = 0; i < count; i++)
			children[i]->Init();
}

}
