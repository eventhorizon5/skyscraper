/*
	Scalable Building Simulator - Generic Object Classes
	The Skyscraper Project - Version 2.1
	Copyright (C)2004-2025 Ryan Thoryk
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

#include <OgreLogManager.h>
#include "globals.h"
#include "sbs.h"
#include "scenenode.h"
#include "profiler.h"
#include "utility.h"
#include "object.h"

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

SBS* ObjectBase::GetRoot()
{
	return sbs;
}

const std::string& ObjectBase::GetName()
{
	//return object name
	return Name;
}

std::string ObjectBase::GetNameBase()
{
	//get base name of the scene node, which includes the instance and object ID

	std::string number;
	if (Parent)
		number = ToString(Parent->GetNumber());
	else
		number = ToString(sbs->GetNumber());

	return ToString(sbs->InstanceNumber) + ":(" + number + ")";
}

void ObjectBase::SetName(const std::string &name)
{
	//set object name
	Name = name;
}

void ObjectBase::Report(const std::string &message)
{
	Ogre::LogManager::getSingleton().logMessage(sbs->InstancePrompt + " " + message);
	sbs->LastNotification = message;
}

bool ObjectBase::ReportError(const std::string &message)
{
	Ogre::LogManager::getSingleton().logMessage(sbs->InstancePrompt + " " + message, Ogre::LML_CRITICAL);
	sbs->LastError = message;
	return false;
}

Object::Object(Object *parent) : ObjectBase(parent)
{
	Permanent = false;
	linenum = 0;
	Number = -1;
	parent_deleting = false;
	node = 0;
	values_set = false;
	initialized = false;
	loop_enabled = false;

	//register object with engine
	if (parent)
		Number = sbs->RegisterObject(this);
	else
		Number = 0; //root object
}

Object::~Object()
{
	//remove object from engine

	//clean up scene node
	if (node)
		node->DetachAllObjects();

	//unregister this object from parent
	if (sbs)
	{
		if (Parent && sbs->FastDelete == false)
		{
			EnableLoop(false);
			Parent->RemoveChild(this);
		}
	}

	//delete scene node
	if (node)
		delete node;
	node = 0;

	//exit if at end of engine deletion
	if (!sbs)
		return;

	//if fastdelete is enabled, don't unregister (just delete)
	if (sbs->FastDelete == true)
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
	if (!object)
		return;

	//add a child object to the internal array
	children.emplace_back(object);

	//add child's scene node
	if (object->GetSceneNode())
	{
		if (node)
			node->AddChild(object->GetSceneNode());
		else if (object->GetSceneNode())
			//if parent doesn't have a scenenode, but child does, add child to engine root node
			sbs->GetSceneNode()->AddChild(object->GetSceneNode());
	}
}

void Object::RemoveChild(Object *object)
{
	if (!object)
		return;

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

void Object::ShowBoundingBox(bool value)
{
	//show object's 3D bounding box

	if (node)
		node->ShowBoundingBox(value);
}

void Object::Move(const Vector3 &vector, Real speed, bool local, bool force)
{
	//move an object

	SBS_PROFILE("Object::Move");

	if (!node)
		return;

	node->Move(vector, speed, local, force);

	//notify about movement
	NotifyMove();
}

void Object::Move(Real X, Real Y, Real Z, Real speed, bool local, bool force)
{
	Vector3 pos (X, Y, Z);
	Move(pos, speed, local, force);
}

void Object::SetPosition(const Vector3 &position, bool relative, bool force)
{
	//set position of object

	if (!node)
		return;

	node->SetPosition(position, relative, force);

	//notify about movement
	NotifyMove();
}

void Object::SetPosition(Real X, Real Y, Real Z, bool relative, bool force)
{
	Vector3 pos (X, Y, Z);
	SetPosition(pos, relative, force);
}

void Object::SetPositionY(Real value, bool force)
{
	//set position of only Y vector

	Vector3 pos (GetPosition().x, value, GetPosition().z);
	SetPosition(pos, false, force);
}

Vector3 Object::GetPosition(bool relative)
{
	//get position of object
	//if relative is true, position is relative of parent object

	if (!node)
		return Vector3::ZERO;

	return node->GetPosition(relative);
}

void Object::SetRotation(const Vector3 &rotation, bool relative)
{
	//rotate object

	SBS_PROFILE("Object::Rotate");

	if (!node)
		return;

	node->SetRotation(rotation, relative);

	//notify about rotation
	NotifyRotate();
}

void Object::SetRotation(Real X, Real Y, Real Z, bool relative)
{
	Vector3 rot (X, Y, Z);
	SetRotation(rot, relative);
}

void Object::Rotate(const Vector3 &vector, Real speed, bool relative)
{
	//rotates object in a relative amount

	Vector3 rot = GetRotation() + (vector * speed);
	SetRotation(rot, relative);
}

void Object::Rotate(Real X, Real Y, Real Z, Real speed, bool relative)
{
	Vector3 rot (X, Y, Z);
	Rotate(rot, speed, relative);
}

Vector3 Object::GetRotation()
{
	//get rotation of object

	if (!node)
		return Vector3::ZERO;

	return node->GetRotation();
}

Quaternion Object::GetOrientation(bool relative)
{
	if (!node)
		return Quaternion::ZERO;

	return node->GetOrientation(relative);
}

void Object::SetOrientation(const Quaternion &q, bool relative)
{
	if (!node)
		return;

	return node->SetOrientation(q, relative);
}

void Object::NotifyMove(bool parent)
{
	//notify about a move
	//if parent is true, this function was called from a parent object

	if (!node)
		return;

	//sync positioning, for child scene nodes
	if (parent == true)
		node->Update();

	NotifyChildren(true, false);
	OnMove(parent);
}

void Object::NotifyRotate(bool parent)
{
	//notify about a rotate
	//if parent is true, this function was called from a parent object

	if (!node)
		return;

	//sync positioning, for child scene nodes
	if (parent == true)
		node->Update();

	NotifyChildren(false, true);
	OnRotate(parent);
}

void Object::NotifyChildren(bool move, bool rotate)
{
	//notify child objects about a parent move or rotate

	SBS_PROFILE("Object::NotifyChildren");

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
	Vector3 pos = GetPosition();

	//get original orientation
	Quaternion q;
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

bool Object::Init(bool children)
{
	//initialize object

	bool status = true;

	//call custom object initialization code
	if (initialized == false)
	{
		bool result = OnInit();
		if (!result)
			return false;
		initialized = true;
	}

	//initialize children
	if (children == true)
		status = InitChildren();
	return status;
}

bool Object::InitChildren()
{
	//initialize child objects

	int count = GetChildrenCount();

	if (count == 0)
		return true;

	bool status = true;
	for (int i = 0; i < count; i++)
	{
		bool result = children[i]->Init();
		if (!result)
			status = false;
	}

	return status;
}

void Object::EnableLoop(bool value)
{
	//enable or disable dynamic runloop

	if (!GetParent())
		return;

	if (loop_enabled == value)
		return;

	Object *parent = GetParent();
	if (parent->GetType() == "Mesh")
		parent = parent->GetParent();

	if (value == true)
		parent->RegisterLoop(this);
	else
		parent->UnregisterLoop(this);

	loop_enabled = value;
}

void Object::RegisterLoop(Object *object)
{
	//register a child object dynamic runloop

	AddArrayElement(runloops, object);
}

void Object::UnregisterLoop(Object *object)
{
	//unregister a child object dynamic runloop

	RemoveArrayElement(runloops, object);
}

bool Object::LoopChildren()
{
	//run dynamic child runloops

	bool status = true;
	for (size_t i = 0; i < runloops.size(); i++)
	{
		if (runloops[i])
		{
			bool result = runloops[i]->Loop();
			if (!result)
				status = false;
		}
	}
	return status;
}

bool Object::SelfDestruct()
{
	//have this object delete itself (make a deletion request to the sim engine root)
	//this is a dangerous function; make sure no more calls are made to this object when calling this,
	//and make sure that this is the last function called by this object

	sbs->Report("Self-destructing object " + ToString(Number));
	return sbs->DeleteObject(this);
}

std::string Object::GetNameBase()
{
	//get base name of the scene node, which includes the instance and object ID

	std::string number = ToString(GetNumber());

	return ToString(sbs->InstanceNumber) + ":(" + number + ")";
}

}
