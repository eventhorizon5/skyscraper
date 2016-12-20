/* $Id$ */

/*
	Scalable Building Simulator - SceneNode Object
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

#ifndef _SBS_SCENENODE_H
#define _SBS_SCENENODE_H

namespace SBS {

class SBSIMPEXP SceneNode: public ObjectBase
{
public:

	SceneNode(Object *parent, std::string name);
	~SceneNode();
	Ogre::SceneNode* GetRawSceneNode() { return node; }
	void AddChild(SceneNode *scenenode);
	void RemoveChild(SceneNode *scenenode);
	void ShowBoundingBox(bool value);
	void SetPosition(const Ogre::Vector3 &position);
	void SetPositionRelative(const Ogre::Vector3 &position);
	Ogre::Vector3 GetPosition(bool relative = false);
	void SetRotation(const Ogre::Vector3 &rotation);
	Ogre::Vector3 GetRotation();
	void Update();
	Ogre::Quaternion GetOrientation(bool relative = false);
	void SetOrientation(const Ogre::Quaternion &q, bool relative = false);
	void Move(const Ogre::Vector3 &vector, float speed = 1.0f);
	void DetachAllObjects();
	void AttachObject(Ogre::MovableObject *object);
	void DetachObject(Ogre::MovableObject *object);
	float GetScale();
	void SetScale(float scale);
	SceneNode* CreateChild(std::string name, const Ogre::Vector3 &offset = Ogre::Vector3::ZERO);
	std::string GetFullName();
	bool IsRoot();

private:

	Ogre::SceneNode *node; //node in scene graph
	Ogre::Vector3 Rotation; //rotation vector
};

}

#endif
