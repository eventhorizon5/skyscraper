/* $Id$ */

/*
	Scalable Building Simulator - Generic Object Class
	The Skyscraper Project - Version 1.6 Alpha
	Copyright (C)2005-2009 Ryan Thoryk
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

#include <csutil/csstring.h>

class SBSIMPEXP Object
{
public:
	//functions
	Object();
	~Object();
	void SetValues(void *object, Object *parent, const char *type, bool is_permanent);
	bool IsPermanent();
	Object* GetParent();
	void* GetRawObject();
	const char* GetObjectType();
	int GetObjectIndex();

private:
	bool Permanent; //is object permanent?
	Object *Parent; //parent object
	void *raw_object; //raw object
	csString Type; //object type
	int Index; //object array index

};

#endif
