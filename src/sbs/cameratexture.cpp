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

#include <OgreCamera.h>
#include <OgreTextureManager.h>
#include <OgreTechnique.h>
#include <OgreMaterialManager.h>
#include <OgreHardwarePixelBuffer.h>
#include "globals.h"
#include "sbs.h"
#include "unix.h"
#include "cameratexture.h"

extern SBS *sbs; //external pointer to the SBS engine

CameraTexture::CameraTexture(Object *parent, const char *name, bool enabled, int quality, float fov, Ogre::Vector3 position, bool use_rotation, Ogre::Vector3 rotation)
{
	//creates a CameraTexture object

	//if use_rotation is true, the rotation vector is a standard rotation, otherwise that vector represents a point in space to look at

	//set up SBS object
	object = new Object();
	object->SetValues(this, parent, "CameraTexture", name, false);

	Name = name;
	Origin = position;

	unsigned int texture_size = 256;
	if (quality == 2)
		texture_size = 512;
	if (quality == 3)
		texture_size = 1024;

	try
	{
		//create a new render texture
		texture = Ogre::TextureManager::getSingleton().createManual(Name, "General", Ogre::TEX_TYPE_2D, texture_size, texture_size, 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET);
		sbs->IncrementTextureCount();
		renderTexture = texture->getBuffer()->getRenderTarget();

		//create and set up camera
		camera = sbs->mSceneManager->createCamera(Name);
		camera->setNearClipDistance(0.1f);
		camera->setFarClipDistance(0.0f);
		camera->setAspectRatio(1.0f);

		//set camera position and rotation
		camera->setPosition(sbs->ToRemote(Origin));
		if (use_rotation == true)
			SetRotation(rotation);
		else
			LookAt(rotation);

		//attach camera viewport to texture
		renderTexture->addViewport(camera);
		renderTexture->getViewport(0)->setClearEveryFrame(true);
		renderTexture->getViewport(0)->setBackgroundColour(Ogre::ColourValue::Black);
		Enabled(true);

		//unload material if already loaded
		if (sbs->UnloadMaterial(name, "General") == true)
			sbs->UnregisterTextureInfo(name);

		//create a new material
		material = Ogre::MaterialManager::getSingleton().create(Name, "General");
		sbs->IncrementMaterialCount();
		material->setLightingEnabled(false);

		//bind texture to material
		material->getTechnique(0)->getPass(0)->createTextureUnitState(Name);

		//show only clockwise side of material
		material->setCullingMode(Ogre::CULL_ANTICLOCKWISE);

		//add texture multipliers
		sbs->RegisterTextureInfo(name, "", "", 1.0f, 1.0f, false, false);

		if (sbs->Verbose)
			sbs->Report("Created camera texture '" + Name + "'");
	}
	catch (Ogre::Exception &e)
	{
		sbs->ReportError("Error creating camera texture:\n" + e.getDescription());
	}
}

CameraTexture::~CameraTexture()
{
	renderTexture->removeAllViewports();
	sbs->mSceneManager->destroyCamera(camera);

	if (sbs->FastDelete == false)
	{
		Ogre::ResourcePtr matwrapper = material;
		Ogre::MaterialManager::getSingleton().remove(matwrapper);
		Ogre::ResourcePtr texwrapper = texture;
		Ogre::TextureManager::getSingleton().remove(texwrapper);
		sbs->UnregisterTextureInfo(Name.c_str());
		sbs->DecrementTextureCount();
		sbs->DecrementMaterialCount();
	}

	delete object;
}

void CameraTexture::Move(const Ogre::Vector3 position, bool relative_x, bool relative_y, bool relative_z)
{
	//move CameraTexture
	Ogre::Vector3 pos;
	if (relative_x == false)
		pos.x = sbs->ToRemote(Origin.x + position.x);
	else
		pos.x = camera->getPosition().x + sbs->ToRemote(position.x);
	if (relative_y == false)
		pos.y = sbs->ToRemote(Origin.y + position.y);
	else
		pos.y = camera->getPosition().y + sbs->ToRemote(position.y);
	if (relative_z == false)
		pos.z = sbs->ToRemote(Origin.z + position.z);
	else
		pos.z = camera->getPosition().z + sbs->ToRemote(position.z);
	camera->setPosition(pos);
}

Ogre::Vector3 CameraTexture::GetPosition()
{
	return sbs->ToLocal(camera->getPosition());
}

void CameraTexture::SetRotation(const Ogre::Vector3 rotation)
{
	Ogre::Quaternion x(Ogre::Degree(rotation.x), Ogre::Vector3::UNIT_X);
	Ogre::Quaternion y(Ogre::Degree(rotation.y), Ogre::Vector3::NEGATIVE_UNIT_Y);
	Ogre::Quaternion z(Ogre::Degree(rotation.z), Ogre::Vector3::UNIT_Z);
	Ogre::Quaternion rot = x * y * z;
	camera->setOrientation(rot);
}

void CameraTexture::LookAt(const Ogre::Vector3 point)
{
	Ogre::Vector3 newpoint = sbs->ToRemote(point);
	camera->lookAt(newpoint.x, newpoint.y, newpoint.z);
}

void CameraTexture::Enabled(bool value)
{
	renderTexture->setActive(value);
}
