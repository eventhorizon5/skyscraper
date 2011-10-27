/* $Id$ */

/*
	Scalable Building Simulator - Texture Functions
	The Skyscraper Project - Version 1.8 Alpha
	Copyright (C)2004-2011 Ryan Thoryk
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
void SBS::loadChromaKeyedTexture(const std::string& filename, const std::string& resGroup, const std::string& name, const Ogre::ColourValue& keyCol, int numMipmaps, float threshold)
 { 
     using namespace Ogre; 
     using std::fabs; 
     Image srcImg; 
	 Ogre::ColourValue keyCol2 = keyCol;

	 String strExt;

	 size_t pos = filename.find_last_of(".");
	 if( pos != String::npos && pos < (filename.length() - 1))
	 {
		strExt = filename.substr(pos+1);
	 }

	 //srcImg.load(filename, resGroup); 
	 DataStreamPtr encoded = ResourceGroupManager::getSingleton().openResource(filename, resGroup);
	 SetCase(strExt, false);
	 if (strExt == "gif")
	 {
		//get chroma transparency color from GIF file data
	    uchar enabled = 0;
		uchar trans_color = 0;
		encoded->seek(784); //transparency enabled if value is 0x1
		encoded->read(&enabled, 1);
		encoded->seek(787); //transparency color
		encoded->read(&trans_color, 1);

		if (enabled == 1 && trans_color <= 255)
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
			if (r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 00 && b <= 255)
			{
				keyCol2.r = r / 255;
				keyCol2.g = g / 255;
				keyCol2.b = b / 255;
			}
		}
		encoded->seek(0);
	 }
	 srcImg.load(encoded, strExt);

     unsigned int width = srcImg.getWidth(), height = srcImg.getHeight();
     // Since Ogre 1.6 Shoggoth, the OGRE_ALLOC_T memory macro must be used:
     uchar* pixelData = OGRE_ALLOC_T(uchar, PixelUtil::getMemorySize(width, height, 1, PF_A8R8G8B8), MEMCATEGORY_GENERAL);
     unsigned long pxDataIndex = 0, pxDataIndexStep = PixelUtil::getNumElemBytes(PF_A8R8G8B8); 
 
     for(unsigned int y = 0; y < height; ++y) 
     { 
         for(unsigned int x = 0; x < width; ++x) 
         { 
             ColourValue pixCol = srcImg.getColourAt(x, y, 0); 
             ColourValue diffCol = pixCol - keyCol2; 
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

bool SBS::WriteToTexture(const std::string &str, Ogre::TexturePtr destTexture, Ogre::Box destRectangle, Ogre::FontPtr font, const Ogre::ColourValue &color, char justify, char vert_justify, bool wordwrap)
{
	//justify is left 'l' by default - set to 'r' or 'c' for right or center
	//vert_justify is top 't' by defautl - set to 'c' or 'b' for center or bottom

	using namespace Ogre;

	if (destTexture->getHeight() < destRectangle.bottom - 1)
		destRectangle.bottom = destTexture->getHeight() - 1;
	if (destTexture->getWidth() < destRectangle.right - 1)
		destRectangle.right = destTexture->getWidth() - 1;

	try
	{
		if (!font->isLoaded())
			font->load();
	}
	catch (Ogre::Exception &e)
	{
		sbs->ReportError("Error loading font " + font->getName() + "\n" + e.getDescription());
		return false;
	}

	TexturePtr fontTexture = (TexturePtr)TextureManager::getSingleton().getByName(font->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName());

	//output glyph map to file
	/*Image image;
	fontTexture->convertToImage(image);
	image.save("test.png");*/

	HardwarePixelBufferSharedPtr fontBuffer = fontTexture->getBuffer();
	HardwarePixelBufferSharedPtr destBuffer = destTexture->getBuffer();

	//PixelBox destPb = destBuffer->lock(destRectangle, HardwareBuffer::HBL_NORMAL);
	PixelBox destPb = destBuffer->lock(Box(0, 0, destTexture->getWidth() - 1, destTexture->getHeight() - 1), HardwareBuffer::HBL_NORMAL);

	// The font texture buffer was created write only...so we cannot read it back :o). One solution is to copy the buffer  instead of locking it. (Maybe there is a way to create a font texture which is not write_only ?)

	int index = -1;
	for (int i = 0; i < textureboxes.size(); i++)
	{
		if (textureboxes[i].font == font)
		{
			index = i;
			break;
		}
	}
	if (index == -1)
	{
		// create a buffer
		index = textureboxes.size();
		textureboxes.resize(textureboxes.size() + 1);
		textureboxes[index].font = font;
		size_t nBuffSize = fontBuffer->getSizeInBytes();
		textureboxes[index].buffer = (unsigned char*)calloc(nBuffSize, sizeof(unsigned char));

		// create pixel box using the copy of the buffer
		textureboxes[index].box = Ogre::PixelBox(fontBuffer->getWidth(), fontBuffer->getHeight(), fontBuffer->getDepth(), fontBuffer->getFormat(), textureboxes[index].buffer);
		fontBuffer->blitToMemory(textureboxes[index].box); //this is very slow
	}


	unsigned char* fontData = static_cast<unsigned char*>(textureboxes[index].box.data);
	unsigned char* destData = static_cast<unsigned char*>(destPb.data);

	const int fontPixelSize = PixelUtil::getNumElemBytes(textureboxes[index].box.format);
	const int destPixelSize = PixelUtil::getNumElemBytes(destPb.format);

	const int fontRowPitchBytes = textureboxes[index].box.rowPitch * fontPixelSize;
	const int destRowPitchBytes = destPb.rowPitch * destPixelSize;

	Box *GlyphTexCoords;
	GlyphTexCoords = new Box[str.size()];

	Font::UVRect glypheTexRect;
	int charheight = 0; //max character height
	int charwidth = 0; //max character width

	for(unsigned int i = 0; i < str.size(); i++)
	{
		if ((str[i] != '\t') && (str[i] != '\n') && (str[i] != ' '))
		{
			glypheTexRect = font->getGlyphTexCoords(str[i]);
			GlyphTexCoords[i].left = glypheTexRect.left * fontTexture->getSrcWidth();
			GlyphTexCoords[i].top = glypheTexRect.top * fontTexture->getSrcHeight();
			GlyphTexCoords[i].right = glypheTexRect.right * fontTexture->getSrcWidth();
			GlyphTexCoords[i].bottom = glypheTexRect.bottom * fontTexture->getSrcHeight();

			//get true bottom of character, since the previous routine doesn't seem to get an accurate result
			int lastline = 0;
			for (int j = 0; j < GlyphTexCoords[i].getHeight(); j++)
			{
				for (int k = 0; k < GlyphTexCoords[i].getWidth(); k++)
				{
					float alpha =  color.a * (fontData[(j + GlyphTexCoords[i].top) * fontRowPitchBytes + (k + GlyphTexCoords[i].left) * fontPixelSize + 1] / 255.0);
					if (alpha > 0.0)
						lastline = j;
				}
			}
			GlyphTexCoords[i].bottom = GlyphTexCoords[i].top + lastline + 1;

			if (GlyphTexCoords[i].getHeight() > charheight)
				charheight = GlyphTexCoords[i].getHeight();
			if (GlyphTexCoords[i].getWidth() > charwidth)
				charwidth = GlyphTexCoords[i].getWidth();
		}
	}

	int cursorX = 0;
	int cursorY = 0;
	int lineend = destRectangle.getWidth();
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
				/*if ((cursorX + GlyphTexCoords[strindex].getWidth() > lineend) && !carriagereturn)
				{
					cursorY += charheight;
					carriagereturn = true;
				}*/

				//justify
				if (carriagereturn)
				{
					int l = strindex;
					int textwidth = 0;
					int wordwidth = 0;

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

						textwidth += wordwidth;
					}

					if (textwidth == 0)
						textwidth = destRectangle.getWidth();

					switch (justify)
					{
						case 'c':
							cursorX = destRectangle.left + (destRectangle.getWidth() / 2) - (textwidth / 2);
							lineend = destRectangle.getWidth() - cursorX;
							break;

						case 'r':
							cursorX = destRectangle.right - textwidth;
							lineend = destRectangle.getWidth();
							break;

						default:
							cursorX = destRectangle.right;
							lineend = textwidth;
							break;
					}

					switch (vert_justify)
					{
						case 'c':
							cursorY = destRectangle.top + (destRectangle.getHeight() / 2) - (charheight / 2);
							break;

						case 'b':
							cursorY = destRectangle.bottom - charheight + cursorY;
							break;
						default:
							cursorY = destRectangle.top;
					}
					carriagereturn = false;
				}

				//abort - not enough space to draw
				if ((cursorY + charheight) >= destTexture->getHeight())
				{
					Report("Text '" + str + "' out of bounds\n");
					goto stop;
				}
				//printf("%d, %d\n", cursorX, cursorY);

				//draw pixel by pixel
				for (int i = 0; i < GlyphTexCoords[strindex].getHeight(); i++)
				{
					for (int j = 0; j < GlyphTexCoords[strindex].getWidth(); j++)
					{
						float alpha =  color.a * (fontData[(i + GlyphTexCoords[strindex].top) * fontRowPitchBytes + (j + GlyphTexCoords[strindex].left) * fontPixelSize + 1] / 255.0);
						float invalpha = 1.0 - alpha;
						int offset = (i + cursorY) * destRowPitchBytes + (j + cursorX) * destPixelSize;
						if (offset >= 0)
						{
							ColourValue pix;
							PixelUtil::unpackColour(&pix, destPb.format, &destData[offset]);
							pix = (pix * invalpha) + (color * alpha);
							PixelUtil::packColour(pix, destPb.format, &destData[offset]);
						}
					}
				}

				cursorX += GlyphTexCoords[strindex].getWidth();
				break;
			}//default
		}//switch
	}//for

	stop:
	delete[] GlyphTexCoords;

	destBuffer->unlock();
	return true;
}
