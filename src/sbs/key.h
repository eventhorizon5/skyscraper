/* $Id$ */

/*
	Scalable Building Simulator - Key Class
	The Skyscraper Project - Version 1.8 Alpha
	Copyright (C)2004-2013 Ryan Thoryk
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

#ifndef _SBS_KEY_H
#define _SBS_KEY_H

class SBSIMPEXP Key
{
public:

	Object *object; //SBS object
	std::string Name; //key name
	Ogre::Vector3 origin; //key origin
	bool IsEnabled;
	int Number;

	Key(Object *parent, const char *name, const char *texture, int number, float CenterX, float CenterZ, float width, float height, float altitude, float tw, float th);
	~Key();
	void Enabled(bool value);
	void Move(const Ogre::Vector3 position, bool relative_x, bool relative_y, bool relative_z);
	Ogre::Vector3 GetPosition();

private:
	MeshObject* KeyMesh; //key mesh

};

#endif
