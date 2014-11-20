/* $Id$ */

/*
	Scalable Building Simulator - Generic Object Class
	The Skyscraper Project - Version 1.9 Alpha
	Copyright (C)2004-2014 Ryan Thoryk
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
	~Object();
	void SetValues(void *object, Object *parent, const char *type, const char *name, bool is_permanent);
	bool IsPermanent();
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

protected:
	bool Permanent; //is object permanent?
	Object *Parent; //parent object
	void *raw_object; //raw object
	std::string Type; //object type
	int Number; //object identifier
	bool Temporary; //true if object can be deleted during runtime
	std::string Name; //object name
	std::vector<Object*> children; //object's children
};

#endif
