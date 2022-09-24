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

#include <OgreCamera.h>
#include "globals.h"
#include "sbs.h"
#include "dynamicmesh.h"
#include "mesh.h"
#include "scenenode.h"
#include "corecamera.h"

namespace SBS {

CoreCamera::CoreCamera(Object *parent) : Object(parent)
{
	//set up SBS object
	SetValues("Camera", "Camera", true);

	MainCamera = 0;
}

CoreCamera::~CoreCamera()
{
	Detach();
}

bool CoreCamera::IsAttached()
{
	return (MainCamera != 0);
}

Ogre::Viewport* CoreCamera::GetViewport()
{
	if (MainCamera)
		return MainCamera->getViewport();
	else
		return 0;
}

void CoreCamera::Pitch(Real &degree)
{
	GetSceneNode()->Pitch(degree);
}

void CoreCamera::Roll(Real &degree)
{
	GetSceneNode()->Roll(degree);
}

void CoreCamera::SetViewMode(int mode)
{
	//set view mode of camera:
	//0 - solid polygons (normal)
	//1 - wireframe mode
	//2 - point mode

	if (!IsAttached())
		return;

	if (mode == 0)
		MainCamera->setPolygonMode(Ogre::PM_SOLID);
	if (mode == 1)
		MainCamera->setPolygonMode(Ogre::PM_WIREFRAME);
	if (mode == 2)
		MainCamera->setPolygonMode(Ogre::PM_POINTS);
}

bool CoreCamera::Attach(Ogre::Camera *camera)
{
	//attach an OGRE camera

	if (camera->isAttached() == true)
		return false;

	MainCamera = camera;
	MainCamera->setNearClipDistance(0.1);
	GetSceneNode()->AttachObject(MainCamera);
	return true;
}

bool CoreCamera::Detach()
{
	//detach an OGRE camera

	if (!IsAttached())
		return false;

	if (MainCamera->isAttached() == false)
		return false;

	GetSceneNode()->DetachObject(MainCamera);
	MainCamera = 0;

	return true;
}

Ogre::Quaternion CoreCamera::GetDerivedOrientation()
{
	return GetSceneNode()->GetDerivedOrientation();
}

Ogre::Ray CoreCamera::GetViewportRay(Real x, Real y)
{
	if (!IsAttached())
		return Ogre::Ray();

	return MainCamera->getCameraToViewportRay(x, y);
}

Real CoreCamera::GetAspectRatio()
{
	if (!IsAttached())
		return 0;

	return MainCamera->getAspectRatio();
}

bool CoreCamera::IsMeshVisible(MeshObject *mesh)
{
	//returns if a mesh object is visible in the camera's view frustum or not

	if (!mesh || !IsAttached())
		return false;

	return mesh->IsVisible(MainCamera);
}

bool CoreCamera::IsDynamicMeshVisible(DynamicMesh *mesh, int mesh_index)
{
	if (!mesh || !IsAttached())
		return false;

	return mesh->IsVisible(MainCamera, mesh_index);
}

void CoreCamera::SetFarClipDistance(Real value)
{
	if (!IsAttached())
		return;

	MainCamera->setFarClipDistance(sbs->ToRemote(value));
}

void CoreCamera::SetFOVAngle(Real angle)
{
	if (!IsAttached())
		return;

	if (angle > 0 && angle < 179.63)
	{
		Real ratio = (float)GetAspectRatio();
		if (ratio > 0)
			MainCamera->setFOVy(Ogre::Degree(angle / ratio));
	}
}

Real CoreCamera::GetFOVAngle()
{
	return (float)(MainCamera->getFOVy().valueDegrees() * GetAspectRatio());
}

}
