/* $Id$ */

/*
	Scalable Building Simulator - Texture Functions
	The Skyscraper Project - Version 1.8 Alpha
	Copyright (C)2004-2010 Ryan Thoryk
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

#include <OgreImage.h>
#include <OgreTextureManager.h>
#include "globals.h"
#include "sbs.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

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
void SBS::loadChromaKeyedTexture(const Ogre::String& filename, const Ogre::String& resGroup, const Ogre::String& name, const Ogre::ColourValue& keyCol, int numMipmaps, float threshold)
 { 
     using namespace Ogre; 
     using std::fabs; 
     Image srcImg; 
     srcImg.load(filename, resGroup); 
     unsigned int width = srcImg.getWidth(), height = srcImg.getHeight();
     // Since Ogre 1.6 Shoggoth, the OGRE_ALLOC_T memory macro must be used:
     uchar* pixelData = OGRE_ALLOC_T(uchar, PixelUtil::getMemorySize(width, height, 1, PF_A8R8G8B8), MEMCATEGORY_GENERAL);
     unsigned long pxDataIndex = 0, pxDataIndexStep = PixelUtil::getNumElemBytes(PF_A8R8G8B8); 
 
     for(unsigned int y = 0; y < height; ++y) 
     { 
         for(unsigned int x = 0; x < width; ++x) 
         { 
             ColourValue pixCol = srcImg.getColourAt(x, y, 0); 
             ColourValue diffCol = pixCol - keyCol; 
             pixCol.a = ((fabs(diffCol.r) < threshold) && (fabs(diffCol.g) < threshold) && (fabs(diffCol.b) < threshold)) ? 0 : 1; 
             Ogre::PixelUtil::packColour(pixCol, PF_A8R8G8B8, static_cast<void*>(pixelData + pxDataIndex)); 
             pxDataIndex += pxDataIndexStep; 
         } 
     } 
 
     Image chromaKeyedImg;
     chromaKeyedImg.loadDynamicImage(pixelData, width, height, 1, PF_A8R8G8B8, true); 
     // You could save the chroma keyed image at this point for caching:
     // chromaKeyedImg.save(resName); 
     TextureManager::getSingleton().loadImage(name, resGroup, chromaKeyedImg, TEX_TYPE_2D, numMipmaps);
 }
