/*
	Scalable Building Simulator - Texture Image
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

#include "globals.h"
#include "sbs.h"
#include "utility.h"
#include "texman.h"
#include "texture.h"
#include "teximage.h"

namespace SBS {

TextureImage::TextureImage(TextureManager* manager, const std::string &filename) : ObjectBase(manager)
{
    SetName(filename);
	this->manager = manager;
	this->filename = filename;
}

TextureImage::~TextureImage()
{

}

Ogre::TexturePtr TextureImage::LoadTexture(int mipmaps, bool &has_alpha, bool use_alpha_color, Ogre::ColourValue alpha_color)
{
	//set verbosity level
	Ogre::TextureManager::getSingleton().setVerbose(sbs->Verbose);

	//exit if no texture specified
	if (filename == "")
		return 0;

	std::string filename2 = filename;

	//determine if the file is a GIF image, to force keycolor alpha
	std::string extension = filename2.substr(filename.size() - 3);
	SetCase(extension, false);
	if (extension == "gif")
		use_alpha_color = true;

	//load the texture
	std::string path = sbs->GetUtility()->GetMountPath(filename2, filename2);
	Ogre::TexturePtr mTex;
	std::string texturename;
	has_alpha = false;

	try
	{
		if (use_alpha_color == false)
		{
			//get any existing texture
			mTex = manager->GetTextureByName(filename2, path);

			//if not found, load new texture
			if (!mTex)
			{
				mTex = Ogre::TextureManager::getSingleton().load(filename2, path, Ogre::TEX_TYPE_2D, mipmaps);
				manager->IncrementTextureCount();
			}

			if (!mTex)
			{
				ReportError("Error loading texture" + filename2);
				return mTex;
			}
			texturename = mTex->getName();
			has_alpha = mTex->hasAlpha();
		}
		else
		{
			//load based on chroma key for alpha

			texturename = "kc_" + filename2;

			//get any existing texture
			mTex = manager->GetTextureByName(texturename, path);

			//if not found, load new texture
			if (!mTex)
				mTex = manager->loadChromaKeyedTexture(filename2, path, texturename, Ogre::ColourValue::White);

			if (!mTex)
			{
				ReportError("Error loading texture" + filename2);
				return mTex;
			}
			has_alpha = true;
		}
	}
	catch (Ogre::Exception &e)
	{
		//texture needs to be removed if a load failed
		Ogre::ResourcePtr wrapper = manager->GetTextureByName(filename2, path);
		if (wrapper.get())
			Ogre::TextureManager::getSingleton().remove(wrapper);

		ReportError("Error loading texture " + filename2 + "\n" + e.getDescription());
		return mTex;
	}

	return mTex;
}

}
