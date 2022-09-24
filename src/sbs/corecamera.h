/*
	Scalable Building Simulator - Core Camera Object
	The Skyscraper Project - Version 1.11 Alpha
	Copyright (C)2004-2022 Ryan Thoryk
	https://www.skyscrapersim.net
	https://sourceforge.net/projects/skyscraper/
	Contact - ryan@thoryk.com

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

#ifndef _SBS_CORECAMERA_H
#define _SBS_CORECAMERA_H

namespace SBS {

class SBSIMPEXP CoreCamera : public Object
{

public:

	//functions
	CoreCamera(Object *parent);
	~CoreCamera();
	bool IsAttached();
	Ogre::Viewport* GetViewport();
	void Pitch(Real &degree);
	void Roll(Real &degree);
	void SetViewMode(int mode);
	bool Attach(Ogre::Camera *camera);
	bool Detach();
	Ogre::Quaternion GetDerivedOrientation();
	Ogre::Ray GetViewportRay(Real x, Real y);
	Real GetAspectRatio();
	bool IsMeshVisible(MeshObject *mesh);
	bool IsDynamicMeshVisible(DynamicMesh *mesh, int mesh_index);
	void SetFarClipDistance(Real value);
	void SetFOVAngle(Real angle);
	Real GetFOVAngle();

private:
	Ogre::Camera* MainCamera; //main first-person view camera
};

}

#endif
