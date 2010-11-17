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
#include <OgreTechnique.h>
#include <OgreFont.h>
#include <OgreHardwarePixelBuffer.h>
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

void SBS::WriteToTexture(const std::string &str, Ogre::TexturePtr destTexture, Ogre::Box destRectangle, Ogre::FontPtr font, const Ogre::ColourValue &color, char justify, bool wordwrap)
{
	//justify is left by default - set to 'r' or 'c' for right or center

	using namespace Ogre;

	if (destTexture->getHeight() < destRectangle.bottom)
		destRectangle.bottom = destTexture->getHeight();
	if (destTexture->getWidth() < destRectangle.right)
		destRectangle.right = destTexture->getWidth();

	if (!font->isLoaded())
		font->load();

	TexturePtr fontTexture = (TexturePtr)TextureManager::getSingleton().getByName(font->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName());

	HardwarePixelBufferSharedPtr fontBuffer = fontTexture->getBuffer();
	HardwarePixelBufferSharedPtr destBuffer = destTexture->getBuffer();

	PixelBox destPb = destBuffer->lock(destRectangle, HardwareBuffer::HBL_NORMAL);

	// The font texture buffer was created write only...so we cannot read it back :o). One solution is to copy the buffer  instead of locking it. (Maybe there is a way to create a font texture which is not write_only ?)

	// create a buffer
	size_t nBuffSize = fontBuffer->getSizeInBytes();
	uint8* buffer = (uint8*)calloc(nBuffSize, sizeof(uint8));

	// create pixel box using the copy of the buffer
	PixelBox fontPb(fontBuffer->getWidth(), fontBuffer->getHeight(),fontBuffer->getDepth(), fontBuffer->getFormat(), buffer);
	fontBuffer->blitToMemory(fontPb);

	uint8* fontData = static_cast<uint8*>(fontPb.data);
	uint8* destData = static_cast<uint8*>(destPb.data);

	const size_t fontPixelSize = PixelUtil::getNumElemBytes(fontPb.format);
	const size_t destPixelSize = PixelUtil::getNumElemBytes(destPb.format);

	const size_t fontRowPitchBytes = fontPb.rowPitch * fontPixelSize;
	const size_t destRowPitchBytes = destPb.rowPitch * destPixelSize;

	Box *GlyphTexCoords;
	GlyphTexCoords = new Box[str.size()];

	Font::UVRect glypheTexRect;
	size_t charheight = 0;
	size_t charwidth = 0;

	for(unsigned int i = 0; i < str.size(); i++)
	{
		if ((str[i] != '\t') && (str[i] != '\n') && (str[i] != ' '))
		{
			glypheTexRect = font->getGlyphTexCoords(str[i]);
			GlyphTexCoords[i].left = glypheTexRect.left * fontTexture->getSrcWidth();
			GlyphTexCoords[i].top = glypheTexRect.top * fontTexture->getSrcHeight();
			GlyphTexCoords[i].right = glypheTexRect.right * fontTexture->getSrcWidth();
			GlyphTexCoords[i].bottom = glypheTexRect.bottom * fontTexture->getSrcHeight();

			if (GlyphTexCoords[i].getHeight() > charheight)
				charheight = GlyphTexCoords[i].getHeight();
			if (GlyphTexCoords[i].getWidth() > charwidth)
				charwidth = GlyphTexCoords[i].getWidth();
		}
	}

	size_t cursorX = 0;
	size_t cursorY = 0;
	size_t lineend = destRectangle.getWidth();
	bool carriagereturn = true;
	for (unsigned int strindex = 0; strindex < str.size(); strindex++)
	{
		switch(str[strindex])
		{
			case ' ':
				cursorX += charwidth;
				break;
			case '\t':
				cursorX += charwidth * 3;
				break;
			case '\n':
				cursorY += charheight; carriagereturn = true;
				break;
			default:
			{
				//wrapping
				if ((cursorX + GlyphTexCoords[strindex].getWidth()> lineend) && !carriagereturn)
				{
					cursorY += charheight;
					carriagereturn = true;
				}

				//justify
				if (carriagereturn)
				{
					size_t l = strindex;
					size_t textwidth = 0;
					size_t wordwidth = 0;

					while((l < str.size()) && (str[l] != '\n'))
					{
						wordwidth = 0;

						switch (str[l])
						{
							case ' ':
								wordwidth = charwidth; ++l;
								break;
							case '\t':
								wordwidth = charwidth *3; ++l;
								break;
							case '\n':
								l = str.size();
						}

						if (wordwrap)
						{
							while((l < str.size()) && (str[l] != ' ') && (str[l] != '\t') && (str[l] != '\n'))
							{
								wordwidth += GlyphTexCoords[l].getWidth();
								++l;
							}
						}
						else
						{
							wordwidth += GlyphTexCoords[l].getWidth();
							l++;
						}

						if ((textwidth + wordwidth) <= destRectangle.getWidth())
							textwidth += (wordwidth);
						else
							break;
					}

					if ((textwidth == 0) && (wordwidth > destRectangle.getWidth()))
						textwidth = destRectangle.getWidth();

					switch (justify)
					{
						case 'c':
							cursorX = (destRectangle.getWidth() - textwidth)/2;
							lineend = destRectangle.getWidth() - cursorX;
							break;

						case 'r':
							cursorX = (destRectangle.getWidth() - textwidth);
							lineend = destRectangle.getWidth();
							break;

						default:
							cursorX = 0;
							lineend = textwidth;
							break;
					}

					carriagereturn = false;
				}

				//abort - net enough space to draw
				if ((cursorY + charheight) > destRectangle.getHeight())
					goto stop;

				//draw pixel by pixel
				for (size_t i = 0; i < GlyphTexCoords[strindex].getHeight(); i++)
					for (size_t j = 0; j < GlyphTexCoords[strindex].getWidth(); j++)
					{
						float alpha =  color.a * (fontData[(i + GlyphTexCoords[strindex].top) * fontRowPitchBytes + (j + GlyphTexCoords[strindex].left) * fontPixelSize + 1] / 255.0);
						float invalpha = 1.0 - alpha;
						size_t offset = (i + cursorY) * destRowPitchBytes + (j + cursorX) * destPixelSize;
						ColourValue pix;
						PixelUtil::unpackColour(&pix, destPb.format, &destData[offset]);
						pix = (pix * invalpha) + (color * alpha);
						PixelUtil::packColour(pix, destPb.format, &destData[offset]);
					}

				cursorX += GlyphTexCoords[strindex].getWidth();
			}//default
		}//switch
	}//for

	stop:
	delete[] GlyphTexCoords;

	destBuffer->unlock();

	// Free the memory allocated for the buffer
	free(buffer); buffer = 0;
}
