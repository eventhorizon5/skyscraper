/*
-----------------------------------------------------------------------------
This source file is part of ogre-procedural

For the latest info, see http://www.ogreprocedural.org

Copyright (c) 2010-2013 Michael Broutin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#include "ProceduralStableHeaders.h"
#include "ProceduralTextureBuffer.h"
#include "Ogre.h"
#include "ProceduralUtils.h"

namespace Procedural
{

using namespace Ogre;

#define PROCEDURAL_RED 0
#define PROCEDURAL_GREEN 1
#define PROCEDURAL_BLUE 2
#define PROCEDURAL_ALPHA 3

TextureBuffer::TextureBuffer(TextureBufferPtr tocopy)
{
	if (tocopy == NULL)
		OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Pointer to source image must not be NULL!", "Procedural::TextureBuffer::TextureBuffer(Procedural::TextureBufferPtr)");

	mPixels = tocopy->mPixels;
}

TextureBuffer::TextureBuffer(Ogre::uint width_height)
	: mPixels(PF_BYTE_RGBA, width_height, width_height)
{
	if (width_height < 8)
		OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Minimum edge size is 8!", "Procedural::TextureBuffer::TextureBuffer(Procedural::TextureBufferPtr)");

	mPixels.setTo(Ogre::ColourValue(0, 0, 0, 1));

#if OGRE_DEBUG_MODE
	Utils::log("Create texture buffer : " + StringConverter::toString(width_height) + "x" + StringConverter::toString(width_height));
#endif
}

TextureBuffer::~TextureBuffer() {}

void TextureBuffer::setPixel(size_t x, size_t y, Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha)
{
	setRed(x, y, red);
	setGreen(x, y, green);
	setBlue(x, y, blue);
	setAlpha(x, y, alpha);
}

void TextureBuffer::setPixel(size_t x, size_t y, Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha)
{
	setRed(x, y, red);
	setGreen(x, y, green);
	setBlue(x, y, blue);
	setAlpha(x, y, alpha);
}

void TextureBuffer::setRed(size_t x, size_t y, Ogre::uchar red)
{
    mPixels.getData(x, y)[PROCEDURAL_RED] = red;
}

void TextureBuffer::setGreen(size_t x, size_t y, Ogre::uchar green)
{
    mPixels.getData(x, y)[PROCEDURAL_GREEN] = green;
}

void TextureBuffer::setBlue(size_t x, size_t y, Ogre::uchar blue)
{
    mPixels.getData(x, y)[PROCEDURAL_BLUE] = blue;
}

void TextureBuffer::setAlpha(size_t x, size_t y, Ogre::uchar alpha)
{
    mPixels.getData(x, y)[PROCEDURAL_ALPHA] = alpha;
}

void TextureBuffer::setRed(size_t x, size_t y, Ogre::Real red)
{
    mPixels.getData(x, y)[PROCEDURAL_RED] = (Ogre::uchar)(std::min<Ogre::Real>(std::max<Ogre::Real>(red * 255.0f, 0.0f), 255.0f));
}

void TextureBuffer::setGreen(size_t x, size_t y, Ogre::Real green)
{
    mPixels.getData(x, y)[PROCEDURAL_GREEN] = (Ogre::uchar)(std::min<Ogre::Real>(std::max<Ogre::Real>(green * 255.0f, 0.0f), 255.0f));
}

void TextureBuffer::setBlue(size_t x, size_t y, Ogre::Real blue)
{
    mPixels.getData(x, y)[PROCEDURAL_BLUE] = (Ogre::uchar)(std::min<Ogre::Real>(std::max<Ogre::Real>(blue * 255.0f, 0.0f), 255.0f));
}

void TextureBuffer::setAlpha(size_t x, size_t y, Ogre::Real alpha)
{
    mPixels.getData(x, y)[PROCEDURAL_ALPHA] = (Ogre::uchar)(std::min<Ogre::Real>(std::max<Ogre::Real>(alpha * 255.0f, 0.0f), 255.0f));
}

void TextureBuffer::setData(size_t width, size_t height, Ogre::uchar* data)
{
	if (data == NULL) return;
	if (width != mPixels.getWidth() || height != mPixels.getWidth()) return;
	memcpy(mPixels.getData(), data, mPixels.getSize());
}

void TextureBuffer::setData(TextureBufferPtr buffer)
{
	if (buffer == NULL) return;
	mPixels = buffer->mPixels;
}

Ogre::ColourValue TextureBuffer::getPixel(size_t x, size_t y) const
{
    return Ogre::ColourValue(getPixelRedReal(x, y), getPixelGreenReal(x, y), getPixelBlueReal(x, y), getPixelAlphaReal(x, y));
}

Ogre::uchar TextureBuffer::getPixelRedByte(size_t x, size_t y) const
{
    return mPixels.getData(x, y)[PROCEDURAL_RED];
}

Ogre::uchar TextureBuffer::getPixelGreenByte(size_t x, size_t y) const
{
    return mPixels.getData(x, y)[PROCEDURAL_GREEN];
}

Ogre::uchar TextureBuffer::getPixelBlueByte(size_t x, size_t y) const
{
    return mPixels.getData(x, y)[PROCEDURAL_BLUE];
}

Ogre::uchar TextureBuffer::getPixelAlphaByte(size_t x, size_t y) const
{
    return mPixels.getData(x, y)[PROCEDURAL_ALPHA];
}

Ogre::Real TextureBuffer::getPixelRedReal(size_t x, size_t y) const
{
    return ((Ogre::Real)mPixels.getData(x, y)[PROCEDURAL_RED]) / 255.0f;
}

Ogre::Real TextureBuffer::getPixelGreenReal(size_t x, size_t y) const
{
    return ((Ogre::Real)mPixels.getData(x, y)[PROCEDURAL_GREEN]) / 255.0f;
}

Ogre::Real TextureBuffer::getPixelBlueReal(size_t x, size_t y) const
{
    return ((Ogre::Real)mPixels.getData(x, y)[PROCEDURAL_BLUE]) / 255.0f;
}

Ogre::Real TextureBuffer::getPixelAlphaReal(size_t x, size_t y) const
{
    return ((Ogre::Real)mPixels.getData(x, y)[PROCEDURAL_ALPHA]) / 255.0f;
}

TextureBufferPtr TextureBuffer::clone() const
{
	TextureBufferPtr clon = new TextureBuffer(mPixels.getWidth());
	clon->mPixels = mPixels;
	return clon;
}

void TextureBuffer::saveImage(Ogre::String filename)
{
	mPixels.save(filename);
}

Ogre::TexturePtr TextureBuffer::createTexture(Ogre::String name, Ogre::String group) const
{
    return Ogre::TextureManager::getSingleton().loadImage(name, group, mPixels, TEX_TYPE_2D, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TextureProcessing::TextureProcessing(TextureBufferPtr pBuffer, Ogre::String name)
{
	if (pBuffer == NULL)
		OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Texture buffer is not set!", "Procedural::TextureProcessing::TextureProcessing(TextureBufferPtr, Ogre::String)");
	mBuffer = pBuffer;
	mName = name;
#if OGRE_DEBUG_MODE
	mLog = true;
#else
	mLog = false;
#endif
}

Ogre::String TextureProcessing::getName()
{
	return mName;
}

void TextureProcessing::setLog(bool enable)
{
	mLog = enable;
}

void TextureProcessing::logMsg(Ogre::String msg)
{
	if (mLog)
		Procedural::Utils::log(msg);
}
}
