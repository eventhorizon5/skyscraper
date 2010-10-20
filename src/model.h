/* $Id$ */

/*
	Scalable Building Simulator - Model Class
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

#ifndef _SBS_MODEL_H
#define _SBS_MODEL_H

#include <csutil/csstring.h>

class SBSIMPEXP Model
{
public:
	Object *object; //SBS object
	csString Name;
	csVector3 Origin;
	bool load_error;

	//functions
	Model(const char *name, const char *filename, csVector3 &position, csVector3 &rotation, float max_render_distance = 0, float scale_multiplier = 1);
	~Model();
	void Move(const csVector3& position, bool relative_x, bool relative_y, bool relative_z);
	csVector3 GetPosition();
	void Rotate(const csVector3& rotation, float speed);
	void SetRotation(const csVector3& rotation);
	csVector3 GetRotation();
	void Enabled(bool value);
	bool IsEnabled();
	void Enable(bool value);

private:

	MeshObject* mesh;
	float rotX, rotY, rotZ;

};

#endif
