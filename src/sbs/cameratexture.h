/* $Id$ */

/*
	Scalable Building Simulator - Camera Texture Class
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

#ifndef _SBS_CameraTexture_H
#define _SBS_CameraTexture_H

class SBSIMPEXP CameraTexture
{
public:

	Object *object; //SBS object
	std::string Name;
	Ogre::Vector3 Origin;

	//functions
	CameraTexture(Object *parent, const char *name, bool enabled, int quality, float fov, Ogre::Vector3 position, bool use_rotation, Ogre::Vector3 rotation);
	~CameraTexture();
	void Move(const Ogre::Vector3 position, bool relative_x, bool relative_y, bool relative_z);
	Ogre::Vector3 GetPosition();
	void SetRotation(const Ogre::Vector3 rotation);
	void LookAt(const Ogre::Vector3 point);
	void Enabled(bool value);

private:

	Ogre::Camera *camera;
	Ogre::TexturePtr texture;
	Ogre::MaterialPtr material;
	Ogre::RenderTexture *renderTexture;
};

#endif
