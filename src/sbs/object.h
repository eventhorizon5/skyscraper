/* $Id$ */

/*
	Scalable Building Simulator - Generic Object Class
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

#ifndef _SBS_OBJECT_H
#define _SBS_OBJECT_H

namespace SBS {

class Object;

//ObjectBase is used for lightweight objects, and core of Object
class SBSIMPEXP ObjectBase
{
	friend class Object;

public:
	ObjectBase(Object *parent);
	virtual ~ObjectBase() {};
	Object* GetParent();
	SBS* GetRoot();
	void SetName(const std::string &name);
	const std::string& GetName();
	std::string GetNameBase();

protected:
	SBS *sbs; //engine root

private:
	Object *Parent; //parent object
	std::string Name; //object name
};

class SBSIMPEXP Object : public ObjectBase
{
public:

	std::string command; //command line used to create object, if applicable
	std::string command_processed; //processed command used to create object
	std::string context; //script context object was created in
	int linenum; //script line number where object was created
	std::string includefile; //include file name
	bool parent_deleting; //true if object's parent is deleting object

	//functions
	Object(Object *parent, bool temporary = false);
	virtual ~Object();
	void SetValues(const std::string &type, const std::string &name, bool is_permanent, bool is_movable = true);
	bool IsPermanent();
	bool IsMovable();
	const std::string& GetType();
	int GetNumber();
	void AddChild(Object *object);
	Object* GetChild(int index);
	int GetChildrenCount();
	void RemoveChild(Object *object);
	SceneNode* GetSceneNode();
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
	virtual void SetRotation(const Ogre::Vector3 &rotation);
	virtual void SetRotation(float X, float Y, float Z);
	virtual Ogre::Vector3 GetRotation();
	Ogre::Quaternion GetOrientation(bool relative = false);
	virtual void OnMove(bool parent) {} //called when object is moved
	virtual void OnRotate(bool parent) {} //called when object is rotated
	virtual void OnClick(Ogre::Vector3 &position, bool shift, bool ctrl, bool alt, bool right) {} //called when object is clicked on
	virtual void OnUnclick(bool right) {} //called when mouse is held and released on object
	virtual void OnHit() {} //called when user hits/collides with object
	void NotifyMove(bool parent = false);
	void NotifyRotate(bool parent = false);
	virtual void ResetState() {} //resets the internal state of an object
	void ChangeParent(Object *new_parent);
	bool IsGlobal();
	void Init(bool children = true); //pre-runloop (first-run) object initialization
	virtual void OnInit() {} //called when object is initialized
	virtual void Loop() {} //object runloop
	void RegisterLoop(Object *object);
	void UnregisterLoop(Object *object);
	virtual void Enabled(bool value) {}
	virtual void Report(const std::string &message);
	virtual bool ReportError(const std::string &message);

	template <typename T> bool IsType()
	{
		//check if an object is of the given type
		return (dynamic_cast<T*>(this) > 0);
	}

	template <typename T> T* ConvertTo()
	{
		//convert object to the given type, if possible.  Returns 0 on failure
		return dynamic_cast<T*>(this);
	}

protected:
	void EnableLoop(bool value);
	void LoopChildren();
	bool SelfDestruct();

private:
	void NotifyChildren(bool move, bool rotate);
	void InitChildren();

	bool Permanent; //is object permanent?
	std::string Type; //object type
	int Number; //object identifier
	bool Temporary; //true if object can be deleted during runtime
	std::vector<Object*> children; //object's children
	SceneNode *node; //node in scene graph
	bool values_set;
	bool initialized;
	std::vector<Object*> runloops; //child object active runloops
	bool loop_enabled;
};

}

#endif
