/* $Id$ */

/*
	Scalable Building Simulator - Generic Object Class
	The Skyscraper Project - Version 1.8 Alpha
	Copyright (C)2004-2010 Ryan Thoryk
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

extern SBS *sbs; //external pointer to the SBS engine

Object::Object(bool temporary)
{
	Permanent = 0;
	Parent = 0;
	raw_object = 0;
	linenum = 0;
	Number = -1;
	Temporary = temporary;
	parent_deleting = false;

	//register object with engine
	if (temporary == false)
		Number = sbs->RegisterObject(this);
}

Object::~Object()
{
	//remove object from engine

	//if fastdelete is enabled, don't unregister (just delete)
	if (sbs->FastDelete == true || Temporary == true)
		return;

	sbs->UnregisterObject(Number);
}

void Object::SetValues(void *object, Object *parent, const char *type, const char *name, bool is_permanent)
{
	//set object values
	raw_object = object;
	Parent = parent;
	Permanent = is_permanent;
	Type = type;
	Name = name;

	//register as child object if object has a valid parent
	if (Parent)
		Parent->AddChild(this);
}

bool Object::IsPermanent()
{
	//return permanent state
	return Permanent;
}

Object* Object::GetParent()
{
	//return parent object
	return Parent;
}

void* Object::GetRawObject()
{
	//return raw object
	return raw_object;
}

const char* Object::GetType()
{
	//return object type string
	return Type;
}

int Object::GetNumber()
{
	//return object's global numeric identifier
	return Number;
}

const char* Object::GetName()
{
	//return object name
	return Name;
}

void Object::SetName(const char *name)
{
	//set object name
	Name = name;
}

void Object::AddChild(Object *object)
{
	//add a child object to the internal array
	children.Push(object);
}

Object* Object::GetChild(int index)
{
	//return pointer to child object from an array index
	if (index < children.GetSize())
		return children[index];
	else
		return 0;
}

int Object::GetChildrenCount()
{
	//return number of child objects
	return children.GetSize();
}
