/* $Id$ */

/*
	Scalable Building Simulator - Camera Texture Object
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

#include <OgreCamera.h>
#include <OgreSceneManager.h>
#include <OgreTextureManager.h>
#include <OgreTechnique.h>
#include <OgreMaterialManager.h>
#include <OgreRenderTexture.h>
#include <OgreViewport.h>
#include <OgreHardwarePixelBuffer.h>
#include "globals.h"
#include "sbs.h"
#include "texture.h"
#include "scenenode.h"
#include "cameratexture.h"

namespace SBS {

CameraTexture::CameraTexture(Object *parent, const std::string &name, bool enabled, int quality, float fov, const Ogre::Vector3 &position, bool use_rotation, const Ogre::Vector3 &rotation) : Object(parent)
{
	//creates a CameraTexture object

	//if use_rotation is true, the rotation vector is a standard rotation, otherwise that vector represents a point in space to look at

	//set up SBS object
	SetValues("CameraTexture", name, false);

	unsigned int texture_size = 256;
	if (quality == 2)
		texture_size = 512;
	if (quality == 3)
		texture_size = 1024;

	try
	{
		//create a new render texture
		std::string texturename = ToString(sbs->InstanceNumber) + ":" + name;
		texture = Ogre::TextureManager::getSingleton().createManual(texturename, "General", Ogre::TEX_TYPE_2D, texture_size, texture_size, 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET);
		sbs->GetTextureManager()->IncrementTextureCount();
		renderTexture = texture->getBuffer()->getRenderTarget();

		//create and set up camera
		camera = sbs->mSceneManager->createCamera(GetSceneNode()->GetFullName());
		camera->setNearClipDistance(0.1f);
		camera->setFarClipDistance(0.0f);
		camera->setAspectRatio(1.0f);

		//attach camera to scene node
		GetSceneNode()->AttachObject(camera);

		//set camera position and rotation
		SetPosition(position);
		if (use_rotation == true)
			SetRotation(rotation);
		else
			LookAt(rotation);

		//attach camera viewport to texture
		renderTexture->addViewport(camera);
		renderTexture->getViewport(0)->setClearEveryFrame(true);
		renderTexture->getViewport(0)->setBackgroundColour(Ogre::ColourValue::Black);
		Enabled(true);

		//create a new material
		material = sbs->GetTextureManager()->CreateMaterial(name, "General");
		sbs->GetTextureManager()->BindTextureToMaterial(material, texturename, false);

		//add texture multipliers
		sbs->GetTextureManager()->RegisterTextureInfo(name, "", "", 1.0f, 1.0f, false, false);

		if (sbs->Verbose)
			sbs->Report("Created camera texture '" + GetName() + "'");
	}
	catch (Ogre::Exception &e)
	{
		sbs->ReportError("Error creating camera texture:\n" + e.getDescription());
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

	Ogre::MaterialManager::getSingleton().remove(material->getHandle());
	Ogre::TextureManager::getSingleton().remove(texture->getHandle());

	if (sbs->FastDelete == false)
	{
		sbs->GetTextureManager()->UnregisterTextureInfo(GetName());
		sbs->GetTextureManager()->DecrementTextureCount();
		sbs->GetTextureManager()->DecrementMaterialCount();
	}
}

void CameraTexture::LookAt(const Ogre::Vector3 &point)
{
	Ogre::Vector3 newpoint = sbs->ToRemote(point);
	camera->lookAt(newpoint.x, newpoint.y, newpoint.z);
}

void CameraTexture::Enabled(bool value)
{
	renderTexture->setActive(value);
}

}
