/*
	Scalable Building Simulator - SceneNode Object
	The Skyscraper Project - Version 1.12 Alpha
	Copyright (C)2004-2024 Ryan Thoryk
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
	void SetPosition(const Vector3 &position);
	void SetPositionRelative(const Vector3 &position);
	Vector3 GetPosition(bool relative = false);
	void SetRotation(const Vector3 &rotation);
	Vector3 GetRotation();
	void Update();
	Quaternion GetOrientation(bool relative = false);
	void SetOrientation(const Quaternion &q, bool relative = false);
	void Move(const Vector3 &vector, Real speed = 1.0);
	void DetachAllObjects();
	void AttachObject(Ogre::MovableObject *object);
	void DetachObject(Ogre::MovableObject *object);
	Real GetScale();
	void SetScale(Real scale);
	SceneNode* CreateChild(std::string name, const Vector3 &offset = Vector3::ZERO);
	std::string GetFullName();
	bool IsRoot();
	void Pitch(Real &degree);
	void Yaw(Real &degree);
	void Roll(Real &degree);
	Vector3 GetDerivedPosition();
	Quaternion GetDerivedOrientation();
	void SetDirection(const Vector3 &direction);
	void LookAt(const Vector3 &point);

private:

	Ogre::SceneNode *node; //node in scene graph
	Vector3 Rotation; //rotation vector
};

}

#endif
