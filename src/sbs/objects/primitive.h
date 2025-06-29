/*
	Scalable Building Simulator - Model Object
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

#ifndef _SBS_PRIMITIVE_H
#define _SBS_PRIMITIVE_H

namespace SBS {

class SBSIMPEXP Primitive: public Object
{
public:

	bool always_visible;
	int collider_type; //0 for standard, 1 for no collider

	//functions
	Primitive(Object *parent, const std::string &name);
	~Primitive();
	bool IsEnabled();
	void Enabled(bool value);
	bool IsKey();
	int GetKeyID();
	void SetKey(int keyid);
	bool IsPhysical();
	void Loop();
	void PickUp();
	void Drop();
	bool IsPickedUp();
	MeshObject* GetMeshObject() { return mesh; }
	void OnClick(Vector3 &position, bool shift, bool ctrl, bool alt, bool right);
	bool Attach(const std::string &meshname, const Vector3 &position, const Vector3 &rotation, Real max_render_distance = 0, Real scale_multiplier = 1, bool enable_physics = false, Real restitution= 0, Real friction = 0, Real mass = 0);
	void SetTexture(const std::string &texture);

private:
	void RemoveFromParent();
	void AddToParent();

	MeshObject* mesh;
	Vector3 Offset;
	bool is_key;
	int KeyID;
	bool global;
};

}

#endif
