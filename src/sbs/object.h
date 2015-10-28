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

#ifndef _SBS_OBJECT_H
#define _SBS_OBJECT_H

namespace SBS {

class SBSIMPEXP Object
{
public:

	std::string command; //command line used to create object, if applicable
	std::string command_processed; //processed command used to create object
	std::string context; //script context object was created in
	int linenum; //script line number where object was created
	std::string includefile; //include file name
	bool parent_deleting; //true if object's parent is deleting object

	//functions
	Object(bool temporary = false);
	virtual ~Object();
	void SetValues(void *raw_object, Object *parent, const char *type, const char *name, bool is_permanent, bool is_movable = true);
	bool IsPermanent();
	bool IsMovable();
	Object* GetParent();
	void* GetRawObject();
	const char* GetType();
	int GetNumber();
	const char* GetName();
	void SetName(const char *name);
	void AddChild(Object *object);
	Object* GetChild(int index);
	int GetChildrenCount();
	void RemoveChild(Object *object);
	Ogre::SceneNode* GetSceneNode();
	void SetNumber(int number);
	bool IsTemporary();
	void ShowBoundingBox(bool value);
	virtual void Move(const Ogre::Vector3 &vector, float speed = 1.0f);
	virtual void Move(float X, float Y, float Z, float speed = 1.0f);
	virtual void SetPosition(const Ogre::Vector3 &position);
	void SetPositionRelative(const Ogre::Vector3 &position);
	virtual void SetPosition(float X, float Y, float Z);
	void SetPositionRelative(float X, float Y, float Z);
	virtual void SetPositionY(float value);
	virtual Ogre::Vector3 GetPosition(bool relative = false);
	virtual void Rotate(const Ogre::Vector3 &vector, float speed = 1.0f);
	virtual void Rotate(float X, float Y, float Z, float speed = 1.0f);
	virtual void SetRotation(Ogre::Vector3 rotation);
	virtual void SetRotation(float X, float Y, float Z);
	virtual Ogre::Vector3 GetRotation();
	virtual void OnMove(bool parent) {}; //called when object is moved
	virtual void OnRotate(bool parent) {}; //called when object is rotated
	void NotifyMove(bool parent = false);
	void NotifyRotate(bool parent = false);
	virtual void ResetState() {}; //resets the internal state of an object
	void ChangeParent(Object *new_parent);

private:
	void NotifyChildren(bool move, bool rotate);

	bool Permanent; //is object permanent?
	Object *Parent; //parent object
	void *raw_object; //raw object
	std::string Type; //object type
	int Number; //object identifier
	bool Temporary; //true if object can be deleted during runtime
	std::string Name; //object name
	std::vector<Object*> children; //object's children
	Ogre::SceneNode *SceneNode; //node in scene graph
	Ogre::Vector3 Rotation; //rotation vector
};

}

#endif
