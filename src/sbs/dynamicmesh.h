/* $Id$ */

/*
	Scalable Building Simulator - Dynamic Mesh
	The Skyscraper Project - Version 1.10 Alpha
	Copyright (C)2004-2016 Ryan Thoryk
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

#ifndef _SBS_DYNAMICMESH_H
#define _SBS_DYNAMICMESH_H

namespace SBS {

class SBSIMPEXP DynamicMesh : public ObjectBase
{
public:

	DynamicMesh(Object *parent, SceneNode *node, const std::string &name, float max_render_distance = 0);
	~DynamicMesh();

	Ogre::Entity* GetMovable();
	void Build();
	void Enable(bool value);
	void ChangeTexture(const std::string &old_texture, const std::string &new_texture);
	void EnableDebugView(bool value);
	bool IsVisible();
	void Prepare(bool force);

private:

	//raw mesh wrapper
	struct SBSIMPEXP Mesh
	{
		Mesh(DynamicMesh *parent, const std::string &name, SceneNode *node, float max_render_distance);
		~Mesh();
		void Enable(bool value);
		void ChangeTexture(const std::string &old_texture, const std::string &new_texture);
		int FindMatchingSubMesh(const std::string &material);
		Ogre::SubMesh* CreateSubMesh(const std::string &material);
		void Prepare(bool force = false);
		void EnableDebugView(bool value);
		bool IsVisible();

		Ogre::MeshPtr MeshWrapper; //mesh
		std::vector<Ogre::SubMesh*> Submeshes; //submeshes (per-material mesh)
		Ogre::Entity *Movable;
		SceneNode *node;
		DynamicMesh *Parent;
		::SBS::SBS *sbs;
	};

	std::vector<Mesh*> meshes;
	SceneNode *node;
	float render_distance;

};

}

#endif
