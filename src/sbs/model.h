/* $Id$ */

/*
	Scalable Building Simulator - Model Class
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

#ifndef _SBS_MODEL_H
#define _SBS_MODEL_H

class SBSIMPEXP Model : public Object
{
public:
	std::string Name;
	bool load_error;

	//functions
	Model(Object *parent, const char *name, const char *filename, bool center, Ogre::Vector3 position, Ogre::Vector3 rotation, float max_render_distance = 0, float scale_multiplier = 1, bool enable_physics = false, float restitution = 0, float friction = 0, float mass = 0);
	~Model();
	bool IsEnabled();
	void Enable(bool value);
	bool IsKey();
	int GetKeyID();
	void SetKey(int keyid);

private:

	MeshObject* mesh;
	Ogre::Vector3 Offset;
	bool is_key;
	int KeyID;
};

#endif
