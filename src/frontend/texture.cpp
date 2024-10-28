/*
	Skyscraper 2.1 - Client Texture Functions
	Copyright (C)2003-2024 Ryan Thoryk
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
#include "skyscraper.h"
#include "vm.h"
#include "texture.h"

using namespace SBS;

namespace Skyscraper
{

void Skyscraper::TextureFunctions()
{
	//load texture
	bool has_alpha = false;
	Ogre::TexturePtr mTex = LoadTexture(filename2, mipmaps, has_alpha, use_alpha_color, alpha_color);

	if (!mTex)
		return;
	std::string texturename = mTex->getName();

	//create a new material
	std::string matname = TrimStringCopy(name);
	Ogre::MaterialPtr mMat = CreateMaterial(matname, "General");

	if (mMat == 0)
		return;

	//bind texture to material
	BindTextureToMaterial(mMat, texturename, has_alpha);
}

Ogre::TexturePtr Skyscraper::LoadTexture(::SBS::SBS *Simcore, const std::string &filename, int mipmaps, bool &has_alpha, bool use_alpha_color, Ogre::ColourValue alpha_color)
{
	//set verbosity level
	Ogre::TextureManager::getSingleton().setVerbose(Verbose);

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
	std::string path = Simcore->GetMountPath(filename2, filename2);
	Ogre::TexturePtr mTex;
	std::string texturename;
	has_alpha = false;

	try
	{
		if (use_alpha_color == false)
		{
			//get any existing texture
			mTex = GetTextureByName(filename2, path);

			//if not found, load new texture
			if (!mTex)
			{
				mTex = Ogre::TextureManager::getSingleton().load(filename2, path, Ogre::TEX_TYPE_2D, mipmaps);
				Simcore->GetTextureManager()->IncrementTextureCount();
			}

			if (!mTex)
			{
				vm->ReportError("Error loading texture" + filename2, "");
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
			mTex = GetTextureByName(texturename, path);

			//if not found, load new texture
			if (!mTex)
				mTex = loadChromaKeyedTexture(Simcore, filename2, path, texturename, Ogre::ColourValue::White);

			if (!mTex)
			{
				vm->ReportError("Error loading texture" + filename2, "");
				return mTex;
			}
			has_alpha = true;
		}
	}
	catch (Ogre::Exception &e)
	{
		//texture needs to be removed if a load failed
		Ogre::ResourcePtr wrapper = GetTextureByName(filename2, path);
		if (wrapper.get())
			Ogre::TextureManager::getSingleton().remove(wrapper);

		vm->ReportError("Error loading texture " + filename2 + "\n" + e.getDescription(), "");
		return mTex;
	}

	return mTex;
}

Ogre::TexturePtr Skyscraper::GetTextureByName(const std::string &name, const std::string &group)
{
	Ogre::TexturePtr ptr = 0;

	if (Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(group) == false)
		return ptr;

	ptr = Ogre::TextureManager::getSingleton().getByName(name, group);
	return ptr;
}

// from http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Creating+transparency+based+on+a+key+colour+in+code
/** Utility function that generates a texture with transparency based on a certain colour value
 @remarks
     The generated texture will either have full opacity or full transparency values
 @param filename
     The name of the image file this texture will be based on, residing in resGroup
 @param keyCol
     The colour value which should cause transparency (alpha component is ignored)
 @param resGroup
     The resource group the base image file belongs and the generated texture will belong to
 @param prefix
     The generated texture will be accessible via the name prefix+filename
 @param numMipmaps
     Usually you leave that on default, but if you never intend to scale the texture, you can set it to 0
 @param threshold
     If the difference between the pixel colour of the image and keyCol is less than this, it gets
     transparent. It should be between 0 and 1. The default value is less than (1 / 256), which means
     that for an 8 Bit channels input file, only the EXACT keyCol gets transparent.
 @return
     Returns the name of the texture resource the generated texture can be addressed by (is prefix+filename)
 */
Ogre::TexturePtr Skyscraper::loadChromaKeyedTexture(::SBS::SBS *Simcore, const std::string& filename, const std::string& resGroup, const std::string& name, const Ogre::ColourValue& keyCol, int numMipmaps, Real threshold)
{
	using namespace Ogre;
	using std::fabs;
	Image srcImg;
	Ogre::ColourValue keyCol2 = keyCol;

	String strExt;

	size_t pos = filename.find_last_of(".");
	if (pos != String::npos && pos < (filename.length() - 1))
		strExt = filename.substr(pos+1);

	//srcImg.load(filename, resGroup);
	DataStreamPtr encoded = ResourceGroupManager::getSingleton().openResource(filename, resGroup);
	SetCase(strExt, false);
	if (strExt == "gif")
	{
		//get chroma transparency color from GIF file data
		uchar enabled = 0, trans_color = 0, check = 0;
		size_t found = 0;

		//first, find graphics control extension (starts with 21:F9:04 hex)
		for (size_t i = 0; i < encoded->size(); i++)
		{
			//extension found
			if (found == 3 && i < encoded->size() - 4)
			{
				//gif transparency info

				//bit flag layout of packed field
				struct ext {
					unsigned char transparent : 1; //transparency flag is first bit
					unsigned char userinput : 1;
					unsigned char disposal : 3;
					unsigned char reserved : 3;
				};
				ext flags;

				encoded->seek(i); //transparency enabled if value is 0x1
				encoded->read(&flags, 1);
				enabled = flags.transparent;
				encoded->seek(i + 3); //transparency color
				encoded->read(&trans_color, 1);
				break;
			}

			encoded->seek(i);
			encoded->read(&check, 1);

			//check for first value, 21h
			if (check == 33 && i < encoded->size() - 4)
			{
				found = 1;

				//find second value, F9h
				i++;
				encoded->seek(i);
				encoded->read(&check, 1);
				if (check == 249)
					found++;

				//find third value, 04h
				i++;
				encoded->seek(i);
				encoded->read(&check, 1);
				if (check == 4)
					found++;
			}
		}

		if (enabled == 1)
		{
			//get color value from image palette
			uchar r, g, b;
			Ogre::uint pos = (Ogre::uint(trans_color) * 3) + 13; //palette starts at offset 0x13
			encoded->seek(pos);
			encoded->read(&r, 1);
			encoded->seek(pos + 1);
			encoded->read(&g, 1);
			encoded->seek(pos + 2);
			encoded->read(&b, 1);
			keyCol2.r = float(r / 255);
			keyCol2.g = float(g / 255);
			keyCol2.b = float(b / 255);
		}
		encoded->seek(0);
	}
	srcImg.load(encoded, strExt);

	unsigned int width = (unsigned int)srcImg.getWidth(), height = (unsigned int)srcImg.getHeight();
	// Since Ogre 1.6 Shoggoth, the OGRE_ALLOC_T memory macro must be used:
	uchar* pixelData = OGRE_ALLOC_T(uchar, PixelUtil::getMemorySize(width, height, 1, PF_A8R8G8B8), MEMCATEGORY_GENERAL);
	unsigned long pxDataIndex = 0, pxDataIndexStep = (unsigned long)PixelUtil::getNumElemBytes(PF_A8R8G8B8);

	for(unsigned int y = 0; y < height; y++)
	{
		for(unsigned int x = 0; x < width; x++)
		{
			ColourValue pixCol = srcImg.getColourAt(x, y, 0);
			ColourValue diffCol = pixCol - keyCol2;
			pixCol.a = ((std::abs(diffCol.r) < threshold) && (std::abs(diffCol.g) < threshold) && (std::abs(diffCol.b) < threshold)) ? 0.0f : 1.0f;
			Ogre::PixelUtil::packColour(pixCol, PF_A8R8G8B8, static_cast<void*>(pixelData + pxDataIndex));
			pxDataIndex += pxDataIndexStep;
		}
	}

	Image chromaKeyedImg;
	chromaKeyedImg.loadDynamicImage(pixelData, width, height, 1, PF_A8R8G8B8, true);
	// You could save the chroma keyed image at this point for caching:
	// chromaKeyedImg.save(resName);
	Ogre::TexturePtr mTex = Ogre::TextureManager::getSingleton().loadImage(name, resGroup, chromaKeyedImg, TEX_TYPE_2D, numMipmaps);
	Simcore->GetTextureManager()->IncrementTextureCount();
	return mTex;
}

Ogre::TextureUnitState* Skyscraper::BindTextureToMaterial(Ogre::MaterialPtr mMat, std::string texture_name, bool has_alpha)
{
	Ogre::TextureUnitState *state = mMat->getTechnique(0)->getPass(0)->createTextureUnitState(texture_name);

	//enable alpha blending for related textures
	if (has_alpha == true)
	{
		//mMat->setDepthWriteEnabled(false);
		//mMat->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);

		//enable hard alpha for alpha mask values 128 and above
		mMat->getTechnique(0)->getPass(0)->setAlphaRejectSettings(Ogre::CMPF_GREATER_EQUAL, 128);
	}
	return state;
}

Ogre::TextureUnitState* Skyscraper::GetTextureUnitState(Ogre::MaterialPtr mMat)
{
	//get first texture unit state
	return mMat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
}

}
