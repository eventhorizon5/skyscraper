/*
	Skyscraper 2.1 - Texture Manager
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

#include <OgreRoot.h>
#include <OgreImage.h>
#include <OgreTextureManager.h>
#include <OgreTechnique.h>
#include <OgreMaterialManager.h>
#include <OgreFont.h>
#include <OgreFontManager.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreResourceGroupManager.h>
#include <OgreRTShaderSystem.h>
#include "Caelum.h"
#include <fmod.hpp>
#include "globals.h"
#include "sbs.h"
#include "vm.h"
#include "enginecontext.h"
#include "utility.h"
#include "texman.h"
#include "texture.h"

using namespace SBS;

namespace Skyscraper {

TexMan::TexMan(EngineContext *parent)
{
	//get singleton
	sbs = parent->GetSystem();

	//load default textures
	parent->Report("Loading default textures...");
	sbs->SetLighting();
	LoadTexture("data/default.png", "Default", 1, 1);
	LoadTexture("data/gray2-sm.jpg", "ConnectionWall", 1, 1);
	LoadTexture("data/metal1-sm.jpg", "Connection", 1, 1);
	sbs->ResetLighting();
	parent->Report("Done");
}

TexMan::~TexMan()
{
	//delete materials
	UnloadMaterials();
}

bool TexMan::LoadTexture(const std::string &filename, const std::string &name, Real widthmult, Real heightmult, bool enable_force, bool force_mode, int mipmaps, bool use_alpha_color, Ogre::ColourValue alpha_color)
{
	if (sbs->Headless == true)
		return true;

	//first verify the filename
	std::string filename2 = sbs->VerifyFile(filename);

	//load texture
	bool has_alpha = false;
	Ogre::TexturePtr mTex = LoadTexture(filename2, mipmaps, has_alpha, use_alpha_color, alpha_color);

	if (!mTex)
		return false;
	std::string texturename = mTex->getName();

	//create a new material
	std::string matname = TrimStringCopy(name);
	Ogre::MaterialPtr mMat = sbs->GetTextureManager()->CreateMaterial(matname, "General");

	if (mMat == 0)
		return false;

	//bind texture to material
	sbs->GetTextureManager()->BindTextureToMaterial(mMat, texturename, has_alpha);

	//add texture multipliers
	sbs->GetTextureManager()->RegisterTextureInfo(name, "", filename, widthmult, heightmult, enable_force, force_mode, mTex->getSize(), mMat->getSize());

	if (sbs->Verbose)
		Report("Loaded texture '" + filename2 + "' as '" + matname + "', size " + ToString((int)mTex->getSize()));
	else
		Report("Loaded texture '" + filename2 + "' as '" + matname + "'");

	return true;
}

bool TexMan::LoadAnimatedTexture(std::vector<std::string> filenames, const std::string &name, Real duration, Real widthmult, Real heightmult, bool enable_force, bool force_mode, int mipmaps, bool use_alpha_color, Ogre::ColourValue alpha_color)
{
	if (sbs->Headless == true)
		return true;

	std::vector<std::string> filenames2;

	size_t num_frames = filenames.size();

	//first verify the filenames
	for (size_t i = 0; i < filenames.size(); i++)
	{
		TrimString(filenames[i]);
		filenames2.push_back(sbs->VerifyFile(filenames[i]));
	}

	bool has_alpha = false;

	size_t size = 0;
	for (size_t i = 0; i < filenames2.size(); i++)
	{
		bool has_alpha2 = false;

		//load texture
		Ogre::TexturePtr mTex = LoadTexture(filenames2[i], mipmaps, has_alpha2, use_alpha_color, alpha_color);

		if (!mTex)
			return false;
		std::string texturename = mTex->getName();

		if (has_alpha2 == true)
			has_alpha = true;

		size += mTex->getSize();

		if (sbs->Verbose)
			Report("Loaded texture " + filenames2[i] + ", size " + ToString((int)mTex->getSize()));
	}

	//create a new material
	std::string matname = TrimStringCopy(name);
	Ogre::MaterialPtr mMat = sbs->GetTextureManager()->CreateMaterial(matname, "General");

	//bind first texture to material
	Ogre::TextureUnitState* state = sbs->GetTextureManager()->BindTextureToMaterial(mMat, filenames2[0], has_alpha);

	//apply animation texture list
	if (state)
	{
		std::string *namelist = new std::string[num_frames];
		for (size_t i = 0; i < num_frames; i++)
			namelist[i] = filenames2[i];
		state->setAnimatedTextureName(namelist, num_frames, duration);
		delete [] namelist;
	}
	else
		return false;

	//add texture multipliers
	sbs->GetTextureManager()->RegisterTextureInfo(name, "", "", widthmult, heightmult, enable_force, force_mode, size, mMat->getSize());

	if (sbs->Verbose)
		Report("Loaded animated texture " + matname);

	return true;
}

bool TexMan::LoadAlphaBlendTexture(const std::string &filename, const std::string &specular_filename, const std::string &blend_filename, const std::string &name, bool spherical, Real widthmult, Real heightmult, bool enable_force, bool force_mode, int mipmaps, bool use_alpha_color, Ogre::ColourValue alpha_color)
{
	if (sbs->Headless == true)
		return true;

	//first verify the filenames
	std::string filename2 = sbs->VerifyFile(filename);
	std::string specular_filename2 = sbs->VerifyFile(specular_filename);
	std::string blend_filename2 = sbs->VerifyFile(blend_filename);

	//load texture
	bool has_alpha = false, has_alpha2 = false;
	Ogre::TexturePtr mTex = LoadTexture(filename2, mipmaps, has_alpha, use_alpha_color, alpha_color);

	if (!mTex)
		return false;
	std::string texturename = mTex->getName();

	//load specular texture
	mTex = LoadTexture(specular_filename2, mipmaps, has_alpha2, use_alpha_color, alpha_color);

	if (!mTex)
		return ReportError("Error loading texture" + specular_filename2);
	std::string specular_texturename = mTex->getName();

	//load blend texture
	mTex = LoadTexture(blend_filename2, mipmaps, has_alpha2, use_alpha_color, alpha_color);

	if (!mTex)
		return ReportError("Error loading texture" + blend_filename2);
	std::string blend_texturename = mTex->getName();

	//create a new material
	std::string matname = TrimStringCopy(name);
	Ogre::MaterialPtr mMat = sbs->GetTextureManager()->CreateMaterial(matname, "General");

	//bind texture to material
	sbs->GetTextureManager()->BindTextureToMaterial(mMat, texturename, has_alpha);
	Ogre::TextureUnitState* spec_state = sbs->GetTextureManager()->BindTextureToMaterial(mMat, specular_texturename, false);
	Ogre::TextureUnitState* blend_state = sbs->GetTextureManager()->BindTextureToMaterial(mMat, blend_texturename, false);

	if (spec_state)
		spec_state->setColourOperation(Ogre::LBO_ALPHA_BLEND);

	if (blend_state)
	{
		blend_state->setColourOperationEx(Ogre::LBX_BLEND_CURRENT_ALPHA);
		if (spherical == true)
			blend_state->setEnvironmentMap(true, Ogre::TextureUnitState::ENV_CURVED);
		else
			blend_state->setEnvironmentMap(true, Ogre::TextureUnitState::ENV_PLANAR);
	}

	//add texture multipliers
	sbs->GetTextureManager()->RegisterTextureInfo(name, "", filename, widthmult, heightmult, enable_force, force_mode, mTex->getSize(), mMat->getSize());

	if (sbs->Verbose)
		Report("Loaded alpha blended texture '" + filename2 + "' as '" + matname + "'");

	return true;
}

bool TexMan::UnloadTexture(const std::string &name, const std::string &group)
{
	//unloads a texture

	Ogre::ResourcePtr wrapper = sbs->GetTextureManager()->GetTextureByName(name, group);
	if (!wrapper)
		return false;
	Ogre::TextureManager::getSingleton().remove(wrapper);
	sbs->GetTextureManager()->DecrementTextureCount();

	Report("Unloaded texture " + name);

	return true;
}

bool TexMan::UnloadMaterial(const std::string &name, const std::string &group)
{
	//unloads a material

	Ogre::ResourcePtr wrapper = sbs->GetTextureManager()->GetMaterialByName(name, group);
	if (!wrapper)
		return false;
	Ogre::MaterialManager::getSingleton().remove(wrapper);
	sbs->GetTextureManager()->DecrementMaterialCount();

	Report("Unloaded material " + name);

	return true;
}

bool TexMan::LoadTextureCropped(const std::string &filename, const std::string &name, int x, int y, int width, int height, Real widthmult, Real heightmult, bool enable_force, bool force_mode)
{
	//loads only a portion of the specified texture

	if (sbs->Headless == true)
		return true;

	Ogre::ColourValue alpha_color = Ogre::ColourValue::Black;
	int mipmaps = -1;
	bool use_alpha_color = false;

	//first verify the filename
	std::string filename2 = sbs->VerifyFile(filename);

	//load texture
	bool has_alpha = false;
	Ogre::TexturePtr mTex = LoadTexture(filename2, mipmaps, has_alpha, use_alpha_color, alpha_color);

	if (!mTex)
		return false;

	//set default values if specified
	if (x == -1)
		x = 0;
	if (y == -1)
		y = 0;
	if (width < 1)
		width = (int)mTex->getWidth();
	if (height < 1)
		height = (int)mTex->getHeight();

	if (x > (int)mTex->getWidth() || y > (int)mTex->getHeight())
		return ReportError("LoadTextureCropped: invalid coordinates for '" + name + "'");
	if (x + width > (int)mTex->getWidth() || y + height > (int)mTex->getHeight())
		return ReportError("LoadTextureCropped: invalid size for '" + name + "'");

	//determine pixel format
	Ogre::PixelFormat format = Ogre::PF_X8R8G8B8;
	if (has_alpha == true)
		format = Ogre::PF_A8R8G8B8;

	//create new empty texture
	std::string texturename = ToString(sbs->InstanceNumber) + ":" + name;
	Ogre::TexturePtr new_texture;
	try
	{
		new_texture = Ogre::TextureManager::getSingleton().createManual(texturename, "General", Ogre::TEX_TYPE_2D, width, height, Ogre::MIP_UNLIMITED, format, Ogre::TU_DEFAULT);
		sbs->GetTextureManager()->manual_textures.push_back(new_texture);
		sbs->GetTextureManager()->IncrementTextureCount();
	}
	catch (Ogre::Exception &e)
	{
		ReportError("Error creating new texture " + texturename + "\n" + e.getDescription());
		return false;
	}

	//copy source and overlay images onto new image
	Ogre::Box source (x, y, x + width, y + height);
	Ogre::Box dest (0, 0, width, height);
	CopyTexture(mTex, new_texture, source, dest);

	//create a new material
	Ogre::MaterialPtr mMat = sbs->GetTextureManager()->CreateMaterial(name, "General");

	//bind texture to material
	sbs->GetTextureManager()->BindTextureToMaterial(mMat, texturename, has_alpha);

	if (sbs->Verbose)
		Report("Loaded cropped texture '" + filename2 + "' as '" + name + "', size " + ToString((int)new_texture->getSize()));

	//add texture multipliers
	sbs->GetTextureManager()->RegisterTextureInfo(name, "", filename, widthmult, heightmult, enable_force, force_mode, mTex->getSize(), mMat->getSize());

	return true;
}

bool TexMan::AddTextToTexture(const std::string &origname, const std::string &name, const std::string &font_filename, Real font_size, const std::string &text, int x1, int y1, int x2, int y2, const std::string &h_align, const std::string &v_align, int ColorR, int ColorG, int ColorB, bool enable_force, bool force_mode)
{
	//adds text to the named texture, in the given box coordinates and alignment

	//h_align is either "left", "right" or "center" - default is center
	//v_align is either "top", "bottom", or "center" - default is center

	//if either x1 or y1 are -1, the value of 0 is used.
	//If either x2 or y2 are -1, the width or height of the texture is used.

	if (sbs->Headless == true)
		return true;

	std::string hAlign = h_align;
	std::string vAlign = v_align;
	std::string Name = TrimStringCopy(name);
	std::string Origname = TrimStringCopy(origname);
	std::string Text = TrimStringCopy(text);

	std::string font_filename2 = sbs->VerifyFile(font_filename);

	//load font
	Ogre::FontPtr font;
	std::string fontname = font_filename2 + ToString(font_size);
	font = Ogre::FontManager::getSingleton().getByName(fontname, "General");

	//load if font is not already loaded
	if (!font)
	{
		try
		{
			font = Ogre::FontManager::getSingleton().create(fontname, "General");
			//font->setType(Ogre::FontType::FT_TRUETYPE);
			font->setSource(font_filename2);
			font->setTrueTypeSize(font_size);
			font->setTrueTypeResolution(96);
			//font->setAntialiasColour(true);
			font->addCodePointRange(Ogre::Font::CodePointRange(32, 126));
			font->load();

			//report font loaded and texture size
			std::string texname = sbs->GetTextureManager()->GetTextureName(font->getMaterial());
			Ogre::TexturePtr fontTexture = sbs->GetTextureManager()->GetTextureByName(texname);
			if (fontTexture)
				Report("Font " + font_filename2 + " loaded as size " + ToString(font_size) + ", with texture size " + ToString((int)fontTexture->getSize()));
		}
		catch (Ogre::Exception &e)
		{
			if (font)
			{
				//unload texture and font, if an error occurred
				if (Ogre::TextureManager::getSingleton().getByName(fontname + "Texture"))
					return ReportError("Error loading font " + fontname + "\n" + e.getDescription());

				if (Ogre::FontManager::getSingleton().getByHandle(font->getHandle()))
					return ReportError("Error loading font " + fontname + "\n" + e.getDescription());

				Ogre::TextureManager::getSingleton().remove(fontname + "Texture");
				Ogre::FontManager::getSingleton().remove(font->getHandle());
			}
			return ReportError("Error loading font " + fontname + "\n" + e.getDescription());
		}
	}

	//get original texture
	Ogre::MaterialPtr ptr = sbs->GetTextureManager()->GetMaterialByName(Origname);
	if (!ptr)
		return ReportError("AddTextToTexture: Invalid original material '" + Origname + "'");

	std::string texname = sbs->GetTextureManager()->GetTextureName(ptr);
	Ogre::TexturePtr background = sbs->GetTextureManager()->GetTextureByName(texname);
	if (!background)
		return ReportError("AddTextToTexture: Invalid original texture '" + texname + "'");

	bool has_alpha = background->hasAlpha();

	//get texture tiling info
	Real widthmult, heightmult;
	sbs->GetTextureManager()->GetTextureTiling(origname, widthmult, heightmult);

	//get height and width of texture
	int width = (int)background->getWidth();
	int height = (int)background->getHeight();

	//determine pixel format
	Ogre::PixelFormat format = Ogre::PF_X8R8G8B8;
	if (has_alpha == true)
		format = Ogre::PF_A8R8G8B8;

	//create new empty texture
	std::string texturename = ToString(sbs->InstanceNumber) + ":" + Name;
	Ogre::TexturePtr texture;
	try
	{
		texture = Ogre::TextureManager::getSingleton().createManual(texturename, "General", Ogre::TEX_TYPE_2D, width, height, Ogre::MIP_UNLIMITED, format, Ogre::TU_STATIC|Ogre::TU_AUTOMIPMAP);
		sbs->GetTextureManager()->manual_textures.push_back(texture);
		sbs->GetTextureManager()->IncrementTextureCount();
	}
	catch (Ogre::Exception &e)
	{
		ReportError("Error creating new texture " + texturename + "\n" + e.getDescription());
		return false;
	}

	//get new texture dimensions, if it was resized
	width = (int)texture->getWidth();
	height = (int)texture->getHeight();

	//set default values if specified
	if (x1 == -1)
		x1 = 0;
	if (y1 == -1)
		y1 = 0;
	if (x2 == -1)
		x2 = width - 1;
	if (y2 == -1)
		y2 = height - 1;

	//draw original image onto new texture
	CopyTexture(background, texture);

	TrimString(hAlign);
	TrimString(vAlign);
	char align = 'c';
	if (hAlign == "left")
		align = 'l';
	if (hAlign == "right")
		align = 'r';
	char valign = 'c';
	if (vAlign == "top")
		valign = 't';
	if (vAlign == "bottom")
		valign = 'b';

	//write text
	Real red = (Real)ColorR / 255;
	Real green = (Real)ColorG / 255;
	Real blue = (Real)ColorB / 255;

	bool result = WriteToTexture(Text, texture, x1, y1, x2, y2, font, Ogre::ColourValue((float)red, (float)green, (float)blue, 1.0), align, valign);
	if (result == false)
		return false;

	//create a new material
	Ogre::MaterialPtr mMat = sbs->GetTextureManager()->CreateMaterial(Name, "General");

	//bind texture to material
	sbs->GetTextureManager()->BindTextureToMaterial(mMat, texturename, has_alpha);

	//add texture multipliers
	sbs->GetTextureManager()->RegisterTextureInfo(name, "", "", widthmult, heightmult, enable_force, force_mode, texture->getSize(), mMat->getSize());

	if (sbs->Verbose)
		Report("AddTextToTexture: created texture '" + Name + "'");

	sbs->CacheFilename(Name, Name);
	return true;
}

bool TexMan::AddTextureOverlay(const std::string &orig_texture, const std::string &overlay_texture, const std::string &name, int x, int y, int width, int height, Real widthmult, Real heightmult, bool enable_force, bool force_mode)
{
	//draws the specified texture on top of another texture
	//orig_texture is the original texture to use; overlay_texture is the texture to draw on top of it

	if (sbs->Headless == true)
		return true;

	std::string Name = name;
	std::string Origname = orig_texture;
	std::string Overlay = overlay_texture;

	//get original texture
	Ogre::MaterialPtr ptr = sbs->GetTextureManager()->GetMaterialByName(Origname);

	if (!ptr)
		return ReportError("AddTextureOverlay: Invalid original material '" + Origname + "'");

	std::string texname = sbs->GetTextureManager()->GetTextureName(ptr);
	Ogre::TexturePtr image1 = sbs->GetTextureManager()->GetTextureByName(texname);

	if (!image1)
		return ReportError("AddTextureOverlay: Invalid original texture '" + texname + "'");

	bool has_alpha = image1->hasAlpha();

	//get overlay texture
	ptr = sbs->GetTextureManager()->GetMaterialByName(Overlay);

	if (!ptr)
		return ReportError("AddTextureOverlay: Invalid overlay material '" + Overlay + "'");

	texname = sbs->GetTextureManager()->GetTextureName(ptr);
	Ogre::TexturePtr image2 = sbs->GetTextureManager()->GetTextureByName(texname);

	if (!image2)
		return ReportError("AddTextureOverlay: Invalid overlay texture '" + texname + "'");

	//set default values if specified
	if (x == -1)
		x = 0;
	if (y == -1)
		y = 0;
	if (width < 1)
		width = (int)image2->getWidth();
	if (height < 1)
		height = (int)image2->getHeight();

	if (x > (int)image1->getWidth() || y > (int)image1->getHeight())
		return ReportError("AddTextureOverlay: invalid coordinates for '" + Name + "'");
	if (x + width > (int)image1->getWidth() || y + height > (int)image1->getHeight())
		return ReportError("AddTextureOverlay: invalid size for '" + Name + "'");

	//determine pixel format
	Ogre::PixelFormat format = Ogre::PF_X8R8G8B8;
	if (has_alpha == true)
		format = Ogre::PF_A8R8G8B8;

	//create new empty texture
	std::string texturename = ToString(sbs->InstanceNumber) + ":" + Name;
	Ogre::TexturePtr new_texture;
	try
	{
		new_texture = Ogre::TextureManager::getSingleton().createManual(texturename, "General", Ogre::TEX_TYPE_2D, (Ogre::uint)image1->getWidth(), (Ogre::uint)image1->getHeight(), Ogre::MIP_UNLIMITED, format, Ogre::TU_DEFAULT);
		sbs->GetTextureManager()->manual_textures.push_back(new_texture);
		sbs->GetTextureManager()->IncrementTextureCount();
	}
	catch (Ogre::Exception &e)
	{
		ReportError("Error creating new texture " + texturename + "\n" + e.getDescription());
		return false;
	}

	//copy source and overlay images onto new image
	Ogre::Box source (x, y, x + width, y + height);
	Ogre::Box source_full (0, 0, image1->getWidth(), image1->getHeight());
	Ogre::Box overlay (0, 0, image2->getWidth(), image2->getHeight());
	CopyTexture(image1, new_texture, source_full, source_full);
	CopyTexture(image2, new_texture, overlay, source);

	//create a new material
	Ogre::MaterialPtr mMat = sbs->GetTextureManager()->CreateMaterial(Name, "General");

	//bind texture to material
	sbs->GetTextureManager()->BindTextureToMaterial(mMat, texturename, has_alpha);

	if (sbs->Verbose)
		Report("AddTextureOverlay: created texture '" + Name + "'");

	//add texture multipliers
	sbs->GetTextureManager()->RegisterTextureInfo(name, "", "", widthmult, heightmult, enable_force, force_mode, new_texture->getSize(), mMat->getSize());

	return true;
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
Ogre::TexturePtr TexMan::loadChromaKeyedTexture(const std::string& filename, const std::string& resGroup, const std::string& name, const Ogre::ColourValue& keyCol, int numMipmaps, Real threshold)
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
	sbs->GetTextureManager()->IncrementTextureCount();
	return mTex;
}

bool TexMan::WriteToTexture(const std::string &str, Ogre::TexturePtr destTexture, int destLeft, int destTop, int destRight, int destBottom, Ogre::FontPtr font, const Ogre::ColourValue &color, char justify, char vert_justify, bool wordwrap)
{
	//justify is left 'l' by default - set to 'r' or 'c' for right or center
	//vert_justify is top 't' by default - set to 'c' or 'b' for center or bottom

	if (sbs->Headless == true)
		return true;

	using namespace Ogre;

	if ((int)destTexture->getHeight() < destBottom - 1)
		destBottom = destTexture->getHeight() - 1;
	if ((int)destTexture->getWidth() < destRight - 1)
		destRight = destTexture->getWidth() - 1;

	try
	{
		if (!font->isLoaded())
			font->load();
	}
	catch (Ogre::Exception &e)
	{
		return ReportError("Error loading font " + font->getName() + "\n" + e.getDescription());
	}

	std::string texname = sbs->GetTextureManager()->GetTextureName(font->getMaterial());
	TexturePtr fontTexture = sbs->GetTextureManager()->GetTextureByName(texname);

	if (!fontTexture)
		return false;

	//output glyph map to file
	//SaveTexture(fontTexture, "test.png");

	HardwarePixelBufferSharedPtr fontBuffer = fontTexture->getBuffer();
	HardwarePixelBufferSharedPtr destBuffer = destTexture->getBuffer();

	PixelBox destPb = destBuffer->lock(Box(0, 0, destTexture->getWidth() - 1, destTexture->getHeight() - 1), HardwareBuffer::HBL_NORMAL);

	// The font texture buffer was created write only...so we cannot read it back :o). One solution is to copy the buffer  instead of locking it. (Maybe there is a way to create a font texture which is not write_only ?)

	int index = -1;
	std::vector<::SBS::TextureManager::TexturePixelBox> &textureboxes = sbs->GetTextureManager()->textureboxes;
	for (size_t i = 0; i < textureboxes.size(); i++)
	{
		if (textureboxes[i].font == font)
		{
			index = (int)i;
			break;
		}
	}
	if (index == -1)
	{
		// create a buffer
		index = (int)textureboxes.size();
		textureboxes.resize(textureboxes.size() + 1);
		textureboxes[index].font = font;
		size_t nBuffSize = fontBuffer->getSizeInBytes();
		textureboxes[index].buffer = (unsigned char*)calloc(nBuffSize, sizeof(unsigned char));

		// create pixel box using the copy of the buffer
		textureboxes[index].box = new Ogre::PixelBox(fontBuffer->getWidth(), fontBuffer->getHeight(), fontBuffer->getDepth(), fontBuffer->getFormat(), textureboxes[index].buffer);
		fontBuffer->blitToMemory(*textureboxes[index].box); //this is very slow
	}

	unsigned char* fontData = static_cast<unsigned char*>(textureboxes[index].box->data);
	unsigned char* destData = static_cast<unsigned char*>(destPb.data);

	const int fontPixelSize = (int)PixelUtil::getNumElemBytes(textureboxes[index].box->format);
	const int destPixelSize = (int)PixelUtil::getNumElemBytes(destPb.format);

	const int fontRowPitchBytes = (int)textureboxes[index].box->rowPitch * fontPixelSize;
	const int destRowPitchBytes = (int)destPb.rowPitch * destPixelSize;

	Box *GlyphTexCoords;
	GlyphTexCoords = new Box[str.size()];

	Font::UVRect glypheTexRect;
	int charheight = 0; //max character height
	int charwidth = 0; //max character width

	for(size_t i = 0; i < str.size(); i++)
	{
		if ((str[i] != '\t') && (str[i] != '\n') && (str[i] != ' '))
		{
			glypheTexRect = font->getGlyphTexCoords(str[i]);
			GlyphTexCoords[i].left = uint32_t(glypheTexRect.left * fontTexture->getSrcWidth());
			GlyphTexCoords[i].top = uint32_t(glypheTexRect.top * fontTexture->getSrcHeight());
			GlyphTexCoords[i].right = uint32_t(glypheTexRect.right * fontTexture->getSrcWidth());
			GlyphTexCoords[i].bottom = uint32_t(glypheTexRect.bottom * fontTexture->getSrcHeight());

			if (glypheTexRect.right < glypheTexRect.left || glypheTexRect.bottom < glypheTexRect.top)
				return ReportError("Error processing font - invalid texture coordinates for font " + font->getName());

			//get true bottom of character, since the previous routine doesn't seem to get an accurate result
			size_t lastline = 0;
			for (size_t j = 0; j < GlyphTexCoords[i].getHeight(); j++)
			{
				for (size_t k = 0; k < GlyphTexCoords[i].getWidth(); k++)
				{
					float alpha =  color.a * (fontData[(j + GlyphTexCoords[i].top) * fontRowPitchBytes + (k + GlyphTexCoords[i].left) * fontPixelSize + 1] / 255.0f);
					if (alpha > 0.0)
						lastline = j;
				}
			}
			GlyphTexCoords[i].bottom = GlyphTexCoords[i].top + lastline + 1;

			if ((int)GlyphTexCoords[i].getHeight() > charheight)
				charheight = (int)GlyphTexCoords[i].getHeight();
			if ((int)GlyphTexCoords[i].getWidth() > charwidth)
				charwidth = (int)GlyphTexCoords[i].getWidth();
		}
	}

	int cursorX = 0;
	int cursorY = 0;
	//int lineend = destRight - destLeft;
	bool carriagereturn = true;
	for (size_t strindex = 0; strindex < str.size(); strindex++)
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
					int l = (int)strindex;
					int textwidth = 0;
					int wordwidth = 0;

					while((l < (int)str.size()) && (str[l] != '\n'))
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
								l = (int)str.size();
						}

						if (wordwrap)
						{
							while((l < (int)str.size()) && (str[l] != ' ') && (str[l] != '\t') && (str[l] != '\n'))
							{
								wordwidth += (int)GlyphTexCoords[l].getWidth();
								++l;
							}
						}
						else
						{
							wordwidth += (int)GlyphTexCoords[l].getWidth();
							l++;
						}

						textwidth += wordwidth;
					}

					if (textwidth == 0)
						textwidth = destRight - destLeft;

					switch (justify)
					{
						case 'c':
							cursorX = destLeft + ((destRight - destLeft) / 2) - (textwidth / 2);
							//lineend = (destRight - destLeft) - cursorX;
							break;

						case 'r':
							cursorX = destRight - textwidth;
							//lineend = destRight - destLeft;
							break;

						default:
							cursorX = destRight;
							//lineend = textwidth;
							break;
					}

					switch (vert_justify)
					{
						case 'c':
							cursorY = destTop + ((destBottom - destTop) / 2) - (charheight / 2);
							break;

						case 'b':
							cursorY = destBottom - charheight + cursorY;
							break;
						default:
							cursorY = destTop;
					}
					carriagereturn = false;
				}

				//abort - not enough space to draw
				if ((cursorY + charheight) >= (int)destTexture->getHeight())
				{
					Report("Text '" + str + "' out of bounds\n");
					goto stop;
				}
				//printf("%d, %d\n", cursorX, cursorY);

				//draw pixel by pixel
				for (size_t i = 0; i < GlyphTexCoords[strindex].getHeight(); i++)
				{
					for (size_t j = 0; j < GlyphTexCoords[strindex].getWidth(); j++)
					{
						float alpha =  color.a * (fontData[(i + GlyphTexCoords[strindex].top) * fontRowPitchBytes + (j + GlyphTexCoords[strindex].left) * fontPixelSize + 1] / 255.0f);
						float invalpha = 1.0f - alpha;
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

				cursorX += (int)GlyphTexCoords[strindex].getWidth();
				break;
			}//default
		}//switch
	}//for

	stop:
	delete[] GlyphTexCoords;

	destBuffer->unlock();
	return true;
}

void TexMan::SaveTexture(Ogre::TexturePtr texture, const std::string & filename)
{
	//save a raw texture to a file

	Ogre::Image image;
	texture->convertToImage(image, true);
	image.save(filename);
}

Ogre::TexturePtr TexMan::LoadTexture(const std::string &filename, int mipmaps, bool &has_alpha, bool use_alpha_color, Ogre::ColourValue alpha_color)
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
	std::string path = sbs->GetMountPath(filename2, filename2);
	Ogre::TexturePtr mTex;
	std::string texturename;
	has_alpha = false;

	try
	{
		if (use_alpha_color == false)
		{
			//get any existing texture
			mTex = sbs->GetTextureManager()->GetTextureByName(filename2, path);

			//if not found, load new texture
			if (!mTex)
			{
				mTex = Ogre::TextureManager::getSingleton().load(filename2, path, Ogre::TEX_TYPE_2D, mipmaps);
				sbs->GetTextureManager()->IncrementTextureCount();
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
			mTex = sbs->GetTextureManager()->GetTextureByName(texturename, path);

			//if not found, load new texture
			if (!mTex)
				mTex = loadChromaKeyedTexture(filename2, path, texturename, Ogre::ColourValue::White);

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
		Ogre::ResourcePtr wrapper = sbs->GetTextureManager()->GetTextureByName(filename2, path);
		if (wrapper.get())
			Ogre::TextureManager::getSingleton().remove(wrapper);

		ReportError("Error loading texture " + filename2 + "\n" + e.getDescription());
		return mTex;
	}

	return mTex;
}

void TexMan::UnloadMaterials()
{
	//delete all registered materials

	for (size_t i = 0; i < sbs->GetTextureManager()->textureinfo.size(); i++)
	{
		Ogre::MaterialManager::getSingleton().remove(ToString(sbs->InstanceNumber) + ":" + sbs->GetTextureManager()->textureinfo[i].name);
	}
}

void TexMan::CopyTexture(Ogre::TexturePtr source, Ogre::TexturePtr destination)
{
	//copy a source texture onto a destination texture using the full sizes

	if (sbs->Headless == true)
		return;

	Ogre::Box srcbox (0, 0, 0, source->getWidth(), source->getHeight(), source->getDepth());
	Ogre::Box dstbox (0, 0, 0, destination->getWidth(), destination->getHeight(), destination->getDepth());

	CopyTexture(source, destination, srcbox, dstbox);
}

void TexMan::CopyTexture(Ogre::TexturePtr source, Ogre::TexturePtr destination, const Ogre::Box &srcBox, const Ogre::Box &dstBox)
{
	//copy a source texture onto a destination texture using specified sizes

	if (sbs->Headless == true)
		return;

	try
	{
		//if dimensions are the same, use standard copy method to prevent
		//some crashes on systems with small npot textures
		//note - this currently doesn't work properly on DirectX, needs fixing
		if (srcBox.getWidth() == source->getWidth() &&
			srcBox.getHeight() == source->getHeight() &&
			dstBox.getWidth() == destination->getWidth() &&
			dstBox.getHeight() == destination->getHeight() &&
			sbs->mRoot->getRenderSystem()->getName() != "Direct3D9 Rendering Subsystem" &&
			sbs->mRoot->getRenderSystem()->getName() != "Direct3D11 Rendering Subsystem")
		{
			source->copyToTexture(destination);
			return;
		}

		Ogre::HardwarePixelBufferSharedPtr buffer = source->getBuffer();

		if (sbs->mRoot->getRenderSystem()->getName() == "Direct3D11 Rendering Subsystem")
		{
			//old method:
			buffer->lock(srcBox, Ogre::HardwareBuffer::HBL_READ_ONLY);
			const Ogre::PixelBox& pb = buffer->getCurrentLock();
			destination->getBuffer()->blitFromMemory(pb, dstBox);
			buffer->unlock();
		}
		else
		{
			//new method:
			destination->getBuffer()->blit(buffer, srcBox, dstBox);
		}
	}
	catch (Ogre::Exception& e)
	{
		ReportError("Error copying texture\n" + e.getDescription());
		return;
	}
}

void TexMan::Report(const std::string &message)
{
	parent->Report(message);
}

bool TexMan::ReportError(const std::string &message)
{
	return parent->ReportError(message);
}

}
