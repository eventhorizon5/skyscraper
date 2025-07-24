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

#include <OgreCamera.h>
#include <OgreSceneManager.h>
#include <OgreTextureManager.h>
#include <OgreTechnique.h>
#include <OgreMaterialManager.h>
#include <OgreRenderTexture.h>
#include <OgreViewport.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreImage.h>
#include "globals.h"
#include "sbs.h"
#include "texman.h"
#include "scenenode.h"
#include "elevatorcar.h"
#include "floor.h"
#include "shaft.h"
#include "stairs.h"
#include "map.h"
#include "cameratexture.h"

namespace SBS {

CameraTexture::CameraTexture(Object *parent, const std::string &name, int quality, Real fov, const Vector3 &position, bool use_rotation, const Vector3 &rotation, bool permanent) : Object(parent)
{
	//creates a CameraTexture object

	//if use_rotation is true, the rotation vector is a standard rotation, otherwise that vector represents a point in space to look at
	//texture quality is 1 for 256x256, 2 for 512x512, and 3 for 1024x1024.

	//set up SBS object
	SetValues("CameraTexture", name, permanent);

	FOV = fov;
	camera = 0;
	renderTexture = 0;
	ortho = false;
	zoom = 1.0;

	unsigned int texture_size = 256;
	if (quality == 2)
		texture_size = 512;
	if (quality == 3)
		texture_size = 1024;

	try
	{
		if (sbs->GetTextureManager()->GetTextureByName(name, "General") || sbs->GetTextureManager()->GetMaterialByName(name, "General"))
		{
			ReportError("Error creating camera texture:\nTexture with the name '" + name + "' already exists.");
			return;
		}

		//create a new render texture
		texturename = ToString(sbs->InstanceNumber) + ":" + name;
		texture = Ogre::TextureManager::getSingleton().createManual(texturename, "General", Ogre::TEX_TYPE_2D, texture_size, texture_size, 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET);
		sbs->GetTextureManager()->IncrementTextureCount();
		renderTexture = texture->getBuffer()->getRenderTarget();

		//create and set up camera
		camera = sbs->mSceneManager->createCamera(GetSceneNode()->GetFullName());
		camera->setNearClipDistance(0.1);
		camera->setFarClipDistance(0.0);
		camera->setAspectRatio(1.0);

		SetFOVAngle(fov);

		//attach camera to scene node
		GetSceneNode()->AttachObject(camera);

		//set camera position and rotation
		Move(position);
		if (use_rotation == true)
			SetRotation(rotation);
		else
			GetSceneNode()->LookAt(rotation);

		//attach camera viewport to texture
		renderTexture->addViewport(camera);
		renderTexture->getViewport(0)->setClearEveryFrame(true);
		renderTexture->getViewport(0)->setBackgroundColour(Ogre::ColourValue::Black);
		Enabled(true);

		//create a new material
		Ogre::MaterialPtr material = sbs->GetTextureManager()->CreateMaterial(name, "General");
		sbs->GetTextureManager()->BindTextureToMaterial(material, texturename, false);
		material->setLightingEnabled(false);

		//add texture multipliers
		sbs->GetTextureManager()->RegisterTexture(name, "", "", 1.0f, 1.0f, false, false, texture->getSize(), material->getSize());

		//register with system
		sbs->RegisterCameraTexture(this);

		//disable by default
		Enabled(false);

		Report("Created camera texture '" + GetName() + "'");
	}
	catch (Ogre::Exception &e)
	{
		ReportError("Error creating camera texture:\n" + e.getDescription());
	}
}

CameraTexture::~CameraTexture()
{
	renderTexture->removeAllViewports();
	if (camera)
	{
		GetSceneNode()->DetachObject(camera);
		sbs->mSceneManager->destroyCamera(camera);
	}

	sbs->GetTextureManager()->UnloadMaterial(texturename, "General");
	sbs->GetTextureManager()->UnloadTexture(texturename, "General");

	if (sbs->FastDelete == false)
	{
		sbs->GetTextureManager()->UnregisterTexture(GetName());
		sbs->UnregisterCameraTexture(this);

		//unregister from parent
		if (parent_deleting == false)
		{
			std::string type = GetParent()->GetType();

			if (type == "ElevatorCar")
				static_cast<ElevatorCar*>(GetParent())->RemoveCameraTexture(this);
			else if (type == "Floor")
				static_cast<Floor*>(GetParent())->RemoveCameraTexture(this);
			else if (type == "Shaft Level")
				static_cast<Shaft::Level*>(GetParent())->RemoveCameraTexture(this);
			else if (type == "Stairwell Level")
				static_cast<Stairwell::Level*>(GetParent())->RemoveCameraTexture(this);
		}
	}
}

bool CameraTexture::Enabled(bool value)
{
	if (renderTexture)
		renderTexture->setActive(value);

	return true;
}

bool CameraTexture::IsEnabled()
{
	if (renderTexture)
		return renderTexture->isActive();

	return false;
}

void CameraTexture::SetFOVAngle(Real angle)
{
	//set camera FOV angle

	if (!camera)
		return;

	if (angle > 0 && angle < 179.63)
	{
		Real ratio = (float)camera->getAspectRatio();
		if (ratio > 0)
			camera->setFOVy(Degree(angle / ratio));
	}
}

Real CameraTexture::GetFOVAngle()
{
	if (!camera)
		return 0.0;
	return (float)(camera->getFOVy().valueDegrees() * camera->getAspectRatio());
}

void CameraTexture::SetToDefaultFOV()
{
	//set to default FOV angle value
	SetFOVAngle(FOV);
}

void CameraTexture::LookAt(const Vector3 &position)
{
	GetSceneNode()->LookAt(position);
}

void CameraTexture::EnableOrthographic(bool value)
{
	ortho = value;

	if (!camera)
		return;

	if (value == true)
		camera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
	else
		camera->setProjectionType(Ogre::PT_PERSPECTIVE);
}

void CameraTexture::GetImage(Ogre::Image &image)
{
	//copy texture to image

	if (texture)
		texture->convertToImage(image, false);
}

void CameraTexture::SetZoom(Real value)
{
	//zoom camera in orthographic mode

	if (!camera || value <= 0)
		return;

	//get current camera view matrix
	Ogre::Affine3 vmatrix = camera->getViewMatrix();

	//remove old zoom factor
	vmatrix[0][0] /= zoom;
	vmatrix[1][2] /= zoom;

	//apply new zoom factor
	zoom = value;
	vmatrix[0][0] *= zoom;
	vmatrix[1][2] *= zoom;

	//set camera view matrix with zoom applied
	camera->setCustomViewMatrix(true, vmatrix);
}

Real CameraTexture::GetZoom()
{
	return zoom;
}

}
