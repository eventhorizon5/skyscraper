/*
	Scalable Building Simulator - Camera Texture Object
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

#ifndef _SBS_CAMERATEXTURE_H
#define _SBS_CAMERATEXTURE_H

namespace SBS {

class SBSIMPEXP CameraTexture : public Object
{
public:

	//functions
	CameraTexture(Object *parent, const std::string &name, int quality, Real fov, const Vector3 &position, bool use_rotation, const Vector3 &rotation, bool permanent = false);
	~CameraTexture();
	bool Enabled(bool value);
	bool IsEnabled();
	void SetFOVAngle(Real angle);
	Real GetFOVAngle();
	void SetToDefaultFOV();
	void LookAt(const Vector3 &position);
	void EnableOrthographic(bool value);
	void GetImage(Ogre::Image &image);
	void SetZoom(Real value);
	Real GetZoom();

private:

	float FOV;
	bool ortho;

	Ogre::Camera *camera;
	std::string texturename;
	Ogre::RenderTexture *renderTexture;
	Ogre::TexturePtr texture;

	Real zoom;
};

}

#endif
