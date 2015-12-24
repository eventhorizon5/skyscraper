/* $Id$ */

/*
	Scalable Building Simulator - Texture Functions
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

#include <OgreImage.h>
#include <OgreTextureManager.h>
#include <OgreTechnique.h>
#include <OgreMaterialManager.h>
#include <OgreFontManager.h>
#include <OgreHardwarePixelBuffer.h>
#include "globals.h"
#include "sbs.h"
#include "unix.h"

namespace SBS {

bool SBS::LoadTexture(const std::string &filename, const std::string &name, float widthmult, float heightmult, bool enable_force, bool force_mode, int mipmaps, bool use_alpha_color, Ogre::ColourValue alpha_color)
{
	//first verify the filename
	std::string filename2 = VerifyFile(filename);

	//load texture
	bool has_alpha = false;
	Ogre::TexturePtr mTex = LoadTexture(filename2, mipmaps, has_alpha, use_alpha_color, alpha_color);

	if (mTex.isNull())
		return false;
	std::string texturename = mTex->getName();

	//create a new material
	std::string matname = TrimStringCopy(name);
	Ogre::MaterialPtr mMat = CreateMaterial(matname, "General");

	//bind texture to material
	BindTextureToMaterial(mMat, texturename, has_alpha);

	//add texture multipliers
	RegisterTextureInfo(name, "", filename, widthmult, heightmult, enable_force, force_mode);

	if (Verbose)
		Report("Loaded texture '" + filename2 + "' as '" + matname + "'");

	return true;
}

bool SBS::LoadAnimatedTexture(std::vector<std::string> filenames, const std::string &name, float duration, float widthmult, float heightmult, bool enable_force, bool force_mode, int mipmaps, bool use_alpha_color, Ogre::ColourValue alpha_color)
{
	std::vector<std::string> filenames2;

	int num_frames = (int)filenames.size();

	//first verify the filenames
	for (int i = 0; i < (int)filenames.size(); i++)
	{
		TrimString(filenames[i]);
		filenames2.push_back(VerifyFile(filenames[i]));
	}

	bool has_alpha = false;

	for (int i = 0; i < (int)filenames2.size(); i++)
	{
		bool has_alpha2 = false;

		//load texture
		Ogre::TexturePtr mTex = LoadTexture(filenames2[i], mipmaps, has_alpha2, use_alpha_color, alpha_color);

		if (mTex.isNull())
			return false;
		std::string texturename = mTex->getName();

		if (has_alpha2 == true)
			has_alpha = true;

		if (Verbose)
			Report("Loaded texture " + filenames2[i]);
	}

	//create a new material
	std::string matname = TrimStringCopy(name);
	Ogre::MaterialPtr mMat = CreateMaterial(matname, "General");

	//bind first texture to material
	Ogre::TextureUnitState* state = BindTextureToMaterial(mMat, filenames2[0], has_alpha);

	//apply animation texture list
	if (state)
	{
		std::string *namelist = new std::string[num_frames];
		for (int i = 0; i < num_frames; i++)
			namelist[i] = filenames2[i];
		state->setAnimatedTextureName(namelist, num_frames, duration);
		delete [] namelist;
	}
	else
		return false;

	//add texture multipliers
	RegisterTextureInfo(name, "", "", widthmult, heightmult, enable_force, force_mode);

	if (Verbose)
		Report("Loaded animated texture " + matname);

	return true;
}

bool SBS::LoadAlphaBlendTexture(const std::string &filename, const std::string &specular_filename, const std::string &blend_filename, const std::string &name, bool spherical, float widthmult, float heightmult, bool enable_force, bool force_mode, int mipmaps, bool use_alpha_color, Ogre::ColourValue alpha_color)
{
	//first verify the filenames
	std::string filename2 = VerifyFile(filename);
	std::string specular_filename2 = VerifyFile(specular_filename);
	std::string blend_filename2 = VerifyFile(blend_filename);

	//load texture
	bool has_alpha = false, has_alpha2 = false;
	Ogre::TexturePtr mTex = LoadTexture(filename2, mipmaps, has_alpha, use_alpha_color, alpha_color);

	if (mTex.isNull())
		return false;
	std::string texturename = mTex->getName();

	//load specular texture
	mTex = LoadTexture(specular_filename2, mipmaps, has_alpha2, use_alpha_color, alpha_color);

	if (mTex.isNull())
		return ReportError("Error loading texture" + specular_filename2);
	std::string specular_texturename = mTex->getName();

	//load blend texture
	mTex = LoadTexture(blend_filename2, mipmaps, has_alpha2, use_alpha_color, alpha_color);

	if (mTex.isNull())
		return ReportError("Error loading texture" + blend_filename2);
	std::string blend_texturename = mTex->getName();

	//create a new material
	std::string matname = TrimStringCopy(name);
	Ogre::MaterialPtr mMat = CreateMaterial(matname, "General");

	//bind texture to material
	BindTextureToMaterial(mMat, texturename, has_alpha);
	Ogre::TextureUnitState* spec_state = BindTextureToMaterial(mMat, specular_texturename, false);
	Ogre::TextureUnitState* blend_state = BindTextureToMaterial(mMat, blend_texturename, false);

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
	RegisterTextureInfo(name, "", filename, widthmult, heightmult, enable_force, force_mode);

	if (Verbose)
		Report("Loaded alpha blended texture '" + filename2 + "' as '" + matname + "'");

	return true;
}

bool SBS::LoadMaterial(const std::string &materialname, const std::string &name, float widthmult, float heightmult, bool enable_force, bool force_mode)
{
	//set verbosity level
	Ogre::MaterialPtr mMat;
	std::string matname = materialname;

	mMat = GetMaterialByName(matname, "Materials");

	if (mMat.isNull())
		return false;

	//show only clockwise side of material
	mMat->setCullingMode(Ogre::CULL_ANTICLOCKWISE);

	//add texture multipliers
	RegisterTextureInfo(name, materialname, "", widthmult, heightmult, enable_force, force_mode);

	if (Verbose)
		Report("Loaded material " + matname);

	return true;
}

void SBS::RegisterTextureInfo(const std::string &name, const std::string &material_name, const std::string &filename, float widthmult, float heightmult, bool enable_force, bool force_mode)
{
	//register texture for multipliers information
	//see TextureInfo structure for more information

	if (widthmult == 0.0f)
		widthmult = 1.0f;
	if (heightmult == 0.0f)
		heightmult = 1.0f;

	TextureInfo info;
	info.name = TrimStringCopy(name);
	info.material_name = TrimStringCopy(material_name);
	info.filename = TrimStringCopy(filename);
	info.widthmult = widthmult;
	info.heightmult = heightmult;
	info.enable_force = enable_force;
	info.force_mode = force_mode;

	textureinfo.push_back(info);
}

bool SBS::UnregisterTextureInfo(std::string name, std::string material_name)
{
	TrimString(name);
	TrimString(material_name);

	for (int i = 0; i < (int)textureinfo.size(); i++)
	{
		if (textureinfo[i].name == name || (textureinfo[i].material_name == material_name && textureinfo[i].material_name != ""))
		{
			textureinfo.erase(textureinfo.begin() + i);
			return true;
		}
	}
	return false;
}

bool SBS::UnloadTexture(const std::string &name, const std::string &group)
{
	//unloads a texture

	Ogre::ResourcePtr wrapper = GetTextureByName(name, group);
	if (wrapper.isNull())
		return false;
	Ogre::TextureManager::getSingleton().remove(wrapper);
	DecrementTextureCount();

	return true;
}

bool SBS::UnloadMaterial(const std::string &name, const std::string &group)
{
	//unloads a material

	Ogre::ResourcePtr wrapper = GetMaterialByName(name, group);
	if (wrapper.isNull())
		return false;
	Ogre::MaterialManager::getSingleton().remove(wrapper);
	DecrementMaterialCount();

	return true;
}

bool SBS::LoadTextureCropped(const std::string &filename, const std::string &name, int x, int y, int width, int height, float widthmult, float heightmult, bool enable_force, bool force_mode)
{
	//loads only a portion of the specified texture

	Ogre::ColourValue alpha_color = Ogre::ColourValue::Black;
	int mipmaps = -1;
	bool use_alpha_color = false;

	//first verify the filename
	std::string filename2 = VerifyFile(filename);

	//load texture
	bool has_alpha = false;
	Ogre::TexturePtr mTex = LoadTexture(filename2, mipmaps, has_alpha, use_alpha_color, alpha_color);

	if (mTex.isNull())
		return false;

	std::string Name = name;
	std::string Filename = filename;

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
		return ReportError("LoadTextureCropped: invalid coordinates for '" + Name + "'");
	if (x + width > (int)mTex->getWidth() || y + height > (int)mTex->getHeight())
		return ReportError("LoadTextureCropped: invalid size for '" + Name + "'");

	//create new empty texture
	std::string texturename = ToString(sbs->InstanceNumber) + ":" + Name;
	Ogre::TexturePtr new_texture = Ogre::TextureManager::getSingleton().createManual(texturename, "General", Ogre::TEX_TYPE_2D, width, height, Ogre::MIP_UNLIMITED, Ogre::PF_R8G8B8A8, Ogre::TU_STATIC|Ogre::TU_AUTOMIPMAP);
	IncrementTextureCount();

	//copy source and overlay images onto new image
	Ogre::Box source (x, y, x + width, y + height);
	Ogre::Box dest (0, 0, width, height);
	CopyTexture(mTex, new_texture, source, dest);

	//create a new material
	Ogre::MaterialPtr mMat = CreateMaterial(Name, "General");

	//bind texture to material
	BindTextureToMaterial(mMat, texturename, has_alpha);

	if (Verbose)
		Report("Loaded cropped texture '" + filename2 + "' as '" + Name + "'");

	//add texture multipliers
	RegisterTextureInfo(name, "", filename, widthmult, heightmult, enable_force, force_mode);

	return true;
}

bool SBS::RotateTexture(const std::string &name, float angle)
{
	//set a fixed rotation value for a texture

	bool result;
	std::string texname = TrimStringCopy(name);
	std::string material = GetTextureMaterial(name, result, false);

	if (!result)
		return ReportError("RotateTexture: invalid texture " + texname + "\n");

	//get material name
	Ogre::MaterialPtr mMat = GetMaterialByName(material);

	if (mMat.isNull())
		return false;

	//get texture unit state
	Ogre::TextureUnitState* state = GetTextureUnitState(mMat);

	//set rotation
	if (state)
		state->setTextureRotate(Ogre::Degree(-angle));
	else
		return false;

	return true;
}

bool SBS::RotateAnimTexture(const std::string &name, float speed)
{
	//set a rotation animation for a texture - speed is in revolutions per second

	bool result;
	std::string texname = TrimStringCopy(name);
	std::string material = GetTextureMaterial(name, result, false);

	if (!result)
		return ReportError("RotateAnimTexture: invalid texture " + texname + "\n");

	//get material name
	Ogre::MaterialPtr mMat = GetMaterialByName(material);

	if (mMat.isNull())
		return false;

	//get texture unit state
	Ogre::TextureUnitState* state = GetTextureUnitState(mMat);

	//set rotation animation
	if (state)
		state->setRotateAnimation(speed);
	else
		return false;

	return true;
}

bool SBS::ScrollTexture(const std::string &name, float x_offset, float y_offset)
{
	//set a fixed scroll amount for a texture

	bool result;
	std::string texname = TrimStringCopy(name);
	std::string material = GetTextureMaterial(name, result, false);

	if (!result)
		return ReportError("ScrollTexture: invalid texture " + texname + "\n");

	//get material name
	Ogre::MaterialPtr mMat = GetMaterialByName(material);

	if (mMat.isNull())
		return false;

	//get texture unit state
	Ogre::TextureUnitState* state = GetTextureUnitState(mMat);

	//set scrolling value
	if (state)
		state->setTextureScroll(-x_offset, y_offset); //invert x value
	else
		return false;

	return true;
}

bool SBS::ScrollAnimTexture(const std::string &name, float x_speed, float y_speed)
{
	//set a scroll animation for a texture - speed is in revolutions per second

	bool result;
	std::string texname = TrimStringCopy(name);
	std::string material = GetTextureMaterial(name, result, false);

	if (!result)
		return ReportError("ScrollAnimTexture: invalid texture " + texname + "\n");

	//get material name
	Ogre::MaterialPtr mMat = GetMaterialByName(material);

	if (mMat.isNull())
		return false;

	//get texture unit state
	Ogre::TextureUnitState* state = GetTextureUnitState(mMat);

	//set scrolling animation
	if (state)
		state->setScrollAnimation(x_speed, -y_speed);
	else
		return false;

	return true;
}

bool SBS::ScaleTexture(const std::string &name, float x_scale, float y_scale)
{
	//set a fixed scale amount for a texture

	bool result;
	std::string texname = TrimStringCopy(name);
	std::string material = GetTextureMaterial(name, result, false);

	if (!result)
		return ReportError("ScaleTexture: invalid texture " + texname + "\n");

	//get material name
	Ogre::MaterialPtr mMat = GetMaterialByName(material);

	if (mMat.isNull())
		return false;

	//get texture unit state
	Ogre::TextureUnitState* state = GetTextureUnitState(mMat);

	//set scale
	if (state)
		state->setTextureScale(x_scale, y_scale);
	else
		return false;

	return true;
}

bool SBS::TransformTexture(const std::string &name, const std::string &type, const std::string &wave_type, float base, float frequency, float phase, float amplitude)
{
	//set a transformation type for a texture

	bool result;
	std::string texname = TrimStringCopy(name);
	std::string material = GetTextureMaterial(name, result, false);

	std::string xform_type = TrimStringCopy(type);
	SetCase(xform_type, false);
	std::string wave = TrimStringCopy(wave_type);
	SetCase(wave, false);

	if (!result)
		return ReportError("TransformTexture: invalid texture " + texname + "\n");

	//get material name
	Ogre::MaterialPtr mMat = GetMaterialByName(material);

	if (mMat.isNull())
		return false;

	//get texture unit state
	Ogre::TextureUnitState* state = GetTextureUnitState(mMat);

	//set transform
	if (state)
	{
		Ogre::TextureUnitState::TextureTransformType type;
		if (xform_type == "scroll_x")
			type = Ogre::TextureUnitState::TT_TRANSLATE_U;
		if (xform_type == "scroll_y")
			type = Ogre::TextureUnitState::TT_TRANSLATE_V;
		if (xform_type == "rotate")
			type = Ogre::TextureUnitState::TT_ROTATE;
		if (xform_type == "scale_x")
			type = Ogre::TextureUnitState::TT_SCALE_U;
		if (xform_type == "scale_y")
			type = Ogre::TextureUnitState::TT_SCALE_V;

		Ogre::WaveformType wavetype;
		if (wave == "sine")
			wavetype = Ogre::WFT_SINE;
		if (wave == "triangle")
			wavetype = Ogre::WFT_TRIANGLE;
		if (wave == "square")
			wavetype = Ogre::WFT_SQUARE;
		if (wave == "sawtooth")
			wavetype = Ogre::WFT_SAWTOOTH;
		if (wave == "inverse_sawtooth")
			wavetype = Ogre::WFT_INVERSE_SAWTOOTH;
		if (wave == "pwm")
			wavetype = Ogre::WFT_PWM;

		state->setTransformAnimation(type, wavetype, base, frequency, phase, amplitude);
	}
	else
		return false;

	return true;
}

bool SBS::AddTextToTexture(const std::string &origname, const std::string &name, const std::string &font_filename, float font_size, const std::string &text, int x1, int y1, int x2, int y2, const std::string &h_align, const std::string &v_align, int ColorR, int ColorG, int ColorB, bool enable_force, bool force_mode)
{
	//adds text to the named texture, in the given box coordinates and alignment

	//h_align is either "left", "right" or "center" - default is center
	//v_align is either "top", "bottom", or "center" - default is center

	//if either x1 or y1 are -1, the value of 0 is used.
	//If either x2 or y2 are -1, the width or height of the texture is used.

	std::string hAlign = h_align;
	std::string vAlign = v_align;
	std::string Name = TrimStringCopy(name);
	std::string Origname = TrimStringCopy(origname);
	std::string Text = TrimStringCopy(text);

	std::string font_filename2 = VerifyFile(font_filename);

	//load font
	Ogre::FontPtr font;
	Ogre::String fontname = font_filename2 + ToString(font_size);
	font = Ogre::FontManager::getSingleton().getByName(fontname, "General");

	//load if font is not already loaded
	if (font.isNull())
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
		}
		catch (Ogre::Exception &e)
		{
			return ReportError("Error loading font " + fontname + "\n" + e.getDescription());
		}
	}

	//get original texture
	Ogre::MaterialPtr ptr = GetMaterialByName(Origname);
	if (ptr.isNull())
		return ReportError("AddTextToTexture: Invalid original material '" + Origname + "'");

	std::string texname = GetTextureName(ptr);
	Ogre::TexturePtr background = GetTextureByName(texname);
	if (background.isNull())
		return ReportError("AddTextToTexture: Invalid original texture '" + texname + "'");

	bool has_alpha = background->hasAlpha();

	//get texture tiling info
	float widthmult, heightmult;
	GetTextureTiling(origname, widthmult, heightmult);

	//get height and width of texture
	int width = (int)background->getWidth();
	int height = (int)background->getHeight();

	//create new empty texture
	std::string texturename = ToString(sbs->InstanceNumber) + ":" + Name;
	Ogre::TexturePtr texture = Ogre::TextureManager::getSingleton().createManual(texturename, "General", Ogre::TEX_TYPE_2D, width, height, Ogre::MIP_UNLIMITED, Ogre::PF_R8G8B8A8, Ogre::TU_STATIC|Ogre::TU_AUTOMIPMAP);
	IncrementTextureCount();

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
	float red = (float)ColorR / 255;
	float green = (float)ColorG / 255;
	float blue = (float)ColorB / 255;

	bool result = WriteToTexture(Text, texture, x1, y1, x2, y2, font, Ogre::ColourValue(red, green, blue, 1.0), align, valign);
	if (result == false)
		return false;

	//create a new material
	Ogre::MaterialPtr mMat = CreateMaterial(Name, "General");

	//bind texture to material
	BindTextureToMaterial(mMat, texturename, has_alpha);

	//add texture multipliers
	RegisterTextureInfo(name, "", "", widthmult, heightmult, enable_force, force_mode);

	if (Verbose)
		Report("AddTextToTexture: created texture '" + Name + "'");

	CacheFilename(Name, Name);
	return true;
}

bool SBS::AddTextureOverlay(const std::string &orig_texture, const std::string &overlay_texture, const std::string &name, int x, int y, int width, int height, float widthmult, float heightmult, bool enable_force, bool force_mode)
{
	//draws the specified texture on top of another texture
	//orig_texture is the original texture to use; overlay_texture is the texture to draw on top of it

	std::string Name = name;
	std::string Origname = orig_texture;
	std::string Overlay = overlay_texture;

	//get original texture
	Ogre::MaterialPtr ptr = GetMaterialByName(Origname);

	if (ptr.isNull())
		return ReportError("AddTextureOverlay: Invalid original material '" + Origname + "'");

	std::string texname = GetTextureName(ptr);
	Ogre::TexturePtr image1 = GetTextureByName(texname);

	if (image1.isNull())
		return ReportError("AddTextureOverlay: Invalid original texture '" + texname + "'");

	bool has_alpha = image1->hasAlpha();

	//get overlay texture
	ptr = GetMaterialByName(Overlay);

	if (ptr.isNull())
		return ReportError("AddTextureOverlay: Invalid overlay material '" + Overlay + "'");

	texname = GetTextureName(ptr);
	Ogre::TexturePtr image2 = GetTextureByName(texname);

	if (image2.isNull())
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

	//create new empty texture
	std::string texturename = ToString(sbs->InstanceNumber) + ":" + Name;
	Ogre::TexturePtr new_texture = Ogre::TextureManager::getSingleton().createManual(texturename, "General", Ogre::TEX_TYPE_2D, (Ogre::uint)image1->getWidth(), (Ogre::uint)image1->getHeight(), Ogre::MIP_UNLIMITED, Ogre::PF_R8G8B8A8, Ogre::TU_STATIC|Ogre::TU_AUTOMIPMAP);
	IncrementTextureCount();

	//copy source and overlay images onto new image
	Ogre::Box source (x, y, x + width, y + height);
	Ogre::Box source_full (0, 0, image1->getWidth(), image1->getHeight());
	Ogre::Box overlay (0, 0, image2->getWidth(), image2->getHeight());
	CopyTexture(image1, new_texture, source_full, source_full);
	CopyTexture(image2, new_texture, overlay, source);

	//create a new material
	Ogre::MaterialPtr mMat = CreateMaterial(Name, "General");

	//bind texture to material
	BindTextureToMaterial(mMat, texturename, has_alpha);

	if (Verbose)
		Report("AddTextureOverlay: created texture '" + Name + "'");

	//add texture multipliers
	RegisterTextureInfo(name, "", "", widthmult, heightmult, enable_force, force_mode);

	return true;
}

std::string SBS::GetTextureMaterial(const std::string &name, bool &result, bool report, const std::string &polygon_name)
{
	//perform a lookup on a texture, and return material name if it exists, or "Default" if not
	//returns false in &result if texture load failed, and if default material was used instead

	//use material_name value instead of name, if loaded as a material script instead of a direct texture
	//if report is true and texture is not found, issue warning

	std::string matname = name;

	if (matname == prev_material)
	{
		result = true;
		return matname;
	}

	int size = (int)textureinfo.size();
	for (int i = 0; i < size; i++)
	{
		if (textureinfo[i].name == matname)
		{
			if (textureinfo[i].material_name != "")
			{
				matname = textureinfo[i].material_name;
				break;
			}
		}
	}
	Ogre::MaterialPtr material = GetMaterialByName(matname);

	if (!material.get())
	{
		//if material's not found, display a warning and use a default material
		if (report == true)
		{
			std::string message;
			if (polygon_name != "")
				message = "Texture '" + matname + "' not found for polygon '" + polygon_name + "'; using default material";
			else
				message = "Texture '" + matname + "' not found; using default material";
			ReportError(message);
		}

		//set to default material
		matname = "Default";
		result = false;
	}
	else
		result = true;

	prev_material = matname;
	return matname;
}

void SBS::ProcessTextureFlip(float tw, float th)
{
	//process texture flip info

	if (tw == 0)
		tw = 1;
	if (th == 0)
		th = 1;

	for (int i = 0; i <= 5; i++)
	{
		widthscale[i] = tw;
		heightscale[i] = th;
	}

	//texture flipping
	if (FlipTexture == true)
	{
		int info;
		for (int i = 0; i <= 5; i++)
		{
			info = 0;
			if (i == 0)
				info = mainnegflip;
			if (i == 1)
				info = mainposflip;
			if (i == 2)
				info = sidenegflip;
			if (i == 3)
				info = sideposflip;
			if (i == 4)
				info = topflip;
			if (i == 5)
				info = bottomflip;

			if (info == 1 || info == 3)
				widthscale[i] = -tw;
			if (info == 2 || info == 3)
				heightscale[i] = -th;
		}
	}
}

bool SBS::GetTextureTiling(const std::string &texture, float &tw, float &th)
{
	//get per-texture tiling values from the textureinfo array
	for (int i = 0; i < (int)textureinfo.size(); i++)
	{
		if (textureinfo[i].name == texture)
		{
			tw = textureinfo[i].widthmult;
			th = textureinfo[i].heightmult;
			return true;
		}
	}
	return false;
}

bool SBS::GetTextureForce(const std::string &texture, bool &enable_force, bool &force_mode)
{
	//get per-texture tiling values from the textureinfo array
	for (int i = 0; i < (int)textureinfo.size(); i++)
	{
		if (textureinfo[i].name == texture)
		{
			enable_force = textureinfo[i].enable_force;
			force_mode = textureinfo[i].force_mode;
			return true;
		}
	}
	return false;
}

int SBS::GetTextureCount()
{
	//return total number of textures
	return texturecount;
}

int SBS::GetMaterialCount()
{
	//return total number of materials
	return materialcount;
}

void SBS::FreeTextureImages()
{
	//unload images in all texture wrappers

	//for (int i = 0; i < engine->GetTextureList()->GetCount(); i++)
		//engine->GetTextureList()->Get(i)->SetImageFile(0);
}

float SBS::AutoSize(float n1, float n2, bool iswidth, float offset, bool enable_force, bool force_mode)
{
	//Texture autosizing formulas

	if (offset == 0)
		offset = 1;

	if (iswidth == true)
	{
		if ((AutoX == true && enable_force == false) || (enable_force == true && force_mode == true))
			return fabsf(n1 - n2) * offset;
		else
			return offset;
	}
	else
	{
		if ((AutoY == true && enable_force == false) || (enable_force == true && force_mode == true))
			return fabsf(n1 - n2) * offset;
		else
			return offset;
	}
}

void SBS::SetAutoSize(bool x, bool y)
{
	//enable or disable texture autosizing
	AutoX = x;
	AutoY = y;
}

void SBS::GetAutoSize(bool &x, bool &y)
{
	//return texture autosizing values
	x = AutoX;
	y = AutoY;
}

Ogre::Vector2 SBS::CalculateSizing(const std::string &texture, const Ogre::Vector3 &v1, const Ogre::Vector3 &v2, const Ogre::Vector3 &v3, int direction, float tw, float th)
{
	//calculate texture autosizing based on polygon extents

	//Call texture autosizing formulas
	float tw2 = tw, th2 = th;

	bool force_enable = false, force_mode = false;
	GetTextureForce(texture, force_enable, force_mode);

	float width = v1.distance(v2);
	float height = v2.distance(v3);

	tw2 = AutoSize(0, width, true, tw, force_enable, force_mode);
	th2 = AutoSize(0, height, false, th, force_enable, force_mode);

	//return results
	return Ogre::Vector2(tw2, th2);
}

bool SBS::GetTextureMapping(std::vector<Ogre::Vector3> &vertices, Ogre::Vector3 &v1, Ogre::Vector3 &v2, Ogre::Vector3 &v3, int &direction)
{
	//returns texture mapping coordinates for the specified polygon index, in the v1, v2, and v3 vectors
	//this performs one of 3 methods - planar mapping, index mapping and manual vertex mapping

	if (MapMethod == 0)
	{
		//planar method

		Ogre::Vector2 x, y, z;
		std::vector<Ogre::Vector3> varray;
		bool rev_x = false, rev_y = false, rev_z = false;

		//determine the largest projection dimension (the dimension that the polygon is generally on;
		//with a floor Y would be biggest)
		Ogre::Plane plane = ComputePlane(vertices);
		Ogre::Vector3 normal = plane.normal;

		direction = 0; //x; faces left/right

		if (fabsf (normal.y) > fabsf (normal.x) && fabsf (normal.y) > fabsf (normal.z))
			direction = 1; //y biggest; faces up/down
		else if (fabsf (normal.z) > fabsf (normal.x))
			direction = 2; //z biggest; faces front/back
		else if (normal.x == 0)
			return false; //fail if normal vector is 0

		size_t selX = (1 << direction) & 0x3;
		size_t selY = (1 << selX) & 0x3;

		varray.reserve(vertices.size());
		for (int i = 0; i < (int)vertices.size(); i++)
		{
			Ogre::Vector3 tmpvertex = vertices[i];
			varray.push_back(Ogre::Vector3(tmpvertex[selX], tmpvertex[selY], 0));
		}

		//automatically flip texture based on largest normal (corrects some situations where the texture is incorrectly reversed)
		if (PlanarRotate == false)
		{
			if (direction == 0 && normal.x > 0)
				rev_z = true;

			if (direction == 1 && normal.y > 0)
				rev_x = true;

			if (direction == 2 && normal.z < 0)
				rev_x = true;
		}
		else
		{
			if (direction == 0 && normal.x < 0)
				rev_z = true;

			if (direction == 0)
				rev_y = true;

			if (direction == 1 && normal.y > 0)
				rev_z = true;

			if (direction == 1)
				rev_x = true;

			if (direction == 2)
				rev_y = true;

			if (direction == 2 && normal.z > 0)
				rev_x = true;
		}

		//force a texture flip based on parameters
		if (RevX == true)
			rev_x = !rev_x;

		if (RevY == true)
			rev_y = !rev_y;

		if (RevZ == true)
			rev_z = !rev_z;

		//get extents of both dimensions, since the polygon is projected in 2D as X and Y coordinates
		Ogre::Vector2 a, b;
		a = GetExtents(varray, 1);
		b = GetExtents(varray, 2);

		//set the result 2D coordinates
		if (direction == 0)
		{
			Ogre::Vector2 pos, pos2;

			if (rev_z == false)
				pos = Ogre::Vector2(b.y, b.x);
			else
				pos = Ogre::Vector2(b.x, b.y);

			if (rev_y == false)
				pos2 = Ogre::Vector2(a.y, a.x);
			else
				pos2 = Ogre::Vector2(a.x, a.y);

			if (PlanarRotate == false)
			{
				v1.z = pos.x; //right
				v2.z = pos.y; //left
				v3.z = pos.y; //left
				v1.y = pos2.x; //top
				v2.y = pos2.x; //top
				v3.y = pos2.y; //bottom
			}
			else
			{
				v1.z = pos.y; //left
				v2.z = pos.y; //left
				v3.z = pos.x; //right
				v1.y = pos2.x; //top
				v2.y = pos2.y; //bottom
				v3.y = pos2.y; //bottom
			}
		}
		if (direction == 1)
		{
			Ogre::Vector2 pos, pos2;

			if (rev_x == false)
				pos = Ogre::Vector2(b.x, b.y);
			else
				pos = Ogre::Vector2(b.y, b.x);

			if (rev_z == false)
				pos2 = Ogre::Vector2(a.x, a.y);
			else
				pos2 = Ogre::Vector2(a.y, a.x);

			if (PlanarRotate == false)
			{
				v1.x = pos.x; //left
				v2.x = pos.y; //right
				v3.x = pos.y; //right
				v1.z = pos2.x; //bottom
				v2.z = pos2.x; //bottom
				v3.z = pos2.y; //top
			}
			else
			{
				v1.x = pos.y; //left
				v2.x = pos.y; //left
				v3.x = pos.x; //right
				v1.z = pos2.y; //bottom
				v2.z = pos2.x; //top
				v3.z = pos2.x; //top
			}
		}
		if (direction == 2)
		{
			Ogre::Vector2 pos, pos2;

			if (rev_x == false)
				pos = Ogre::Vector2(a.y, a.x);
			else
				pos = Ogre::Vector2(a.x, a.y);

			if (rev_y == false)
				pos2 = Ogre::Vector2(b.y, b.x);
			else
				pos2 = Ogre::Vector2(b.x, b.y);

			if (PlanarRotate == false)
			{
				v1.x = pos.x; //right
				v2.x = pos.y; //left
				v3.x = pos.y; //left
				v1.y = pos2.x; //top
				v2.y = pos2.x; //top
				v3.y = pos2.y; //bottom
			}
			else
			{
				v1.x = pos.y; //left
				v2.x = pos.y; //left
				v3.x = pos.x; //right
				v1.y = pos2.x; //top
				v2.y = pos2.y; //bottom
				v3.y = pos2.y; //bottom
			}
		}

		//use the plane equation to get the coordinate values of the dropped dimension
		if (direction == 0)
		{
			v1.x = -((normal.y * v1.y) + (normal.z * v1.z) + plane.d) / normal.x; //get X
			v2.x = -((normal.y * v2.y) + (normal.z * v2.z) + plane.d) / normal.x; //get X
			v3.x = -((normal.y * v3.y) + (normal.z * v3.z) + plane.d) / normal.x; //get X

			if (PlanarFlat == true)
				v3.x = v2.x;
		}
		if (direction == 1)
		{
			v1.y = -((normal.x * v1.x) + (normal.z * v1.z) + plane.d) / normal.y; //get Y
			v2.y = -((normal.x * v2.x) + (normal.z * v2.z) + plane.d) / normal.y; //get Y
			v3.y = -((normal.x * v3.x) + (normal.z * v3.z) + plane.d) / normal.y; //get Y

			if (PlanarFlat == true)
				v3.y = v2.y;
		}
		if (direction == 2)
		{
			v1.z = -((normal.x * v1.x) + (normal.y * v1.y) + plane.d) / normal.z; //get Z
			v2.z = -((normal.x * v2.x) + (normal.y * v2.y) + plane.d) / normal.z; //get Z
			v3.z = -((normal.x * v3.x) + (normal.y * v3.y) + plane.d) / normal.z; //get Z

			if (PlanarFlat == true)
				v3.z = v2.z;
		}
	}
	if (MapMethod == 1)
	{
		//index method
		v1 = vertices[MapIndex[0]];
		v2 = vertices[MapIndex[1]];
		v3 = vertices[MapIndex[2]];

		//determine the largest projection dimension (the dimension that the polygon is generally on;
		//with a floor Y would be biggest)
		Ogre::Plane plane = Ogre::Plane(v1, v2, v3);
		Ogre::Vector3 normal = plane.normal;

		direction = 0; //x; faces left/right

		if (fabsf (normal.y) > fabsf (normal.x) && fabsf (normal.y) > fabsf (normal.z))
			direction = 1; //y biggest; faces up/down
		else if (fabsf (normal.z) > fabsf (normal.x))
			direction = 2; //z biggest; faces front/back
		else if (normal.x == 0)
			return false; //fail if normal vector is 0
	}
	if (MapMethod == 2)
	{
		//advanced manual vertex method

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				std::string string;
				if (j == 0)
					string = MapVerts1[i];
				if (j == 1)
					string = MapVerts2[i];
				if (j == 2)
					string = MapVerts3[i];

				SetCase(string, false);

				//find X component
				int location = (int)string.find("x");
				if (location >= 0)
				{
					std::string number = string.substr(location + 1);
					if (ToInt(number) < (int)vertices.size())
						ReplaceAll(string, "x" + number, ToString(vertices[ToInt(number)].x));
					else
						ReplaceAll(string, "x" + number, "0"); //number value out of bounds
				}

				//find Y component
				location = (int)string.find("y");
				if (location >= 0)
				{
					std::string number = string.substr(location + 1);
					if (ToInt(number) < (int)vertices.size())
						ReplaceAll(string, "y" + number, ToString(vertices[ToInt(number)].y));
					else
						ReplaceAll(string, "y" + number, "0"); //number value out of bounds
				}

				//find Z component
				location = (int)string.find("z");
				if (location >= 0)
				{
					std::string number = string.substr(location + 1);
					if (ToInt(number) < (int)vertices.size())
						ReplaceAll(string, "z" + number, ToString(vertices[ToInt(number)].z));
					else
						ReplaceAll(string, "z" + number, "0"); //number value out of bounds
				}

				//store values
				if (i == 0)
				{
					if (j == 0)
						v1.x = ToFloat(string);
					if (j == 1)
						v2.x = ToFloat(string);
					if (j == 2)
						v3.x = ToFloat(string);
				}
				if (i == 1)
				{
					if (j == 0)
						v1.y = ToFloat(string);
					if (j == 1)
						v2.y = ToFloat(string);
					if (j == 2)
						v3.y = ToFloat(string);
				}
				if (i == 2)
				{
					if (j == 0)
						v1.z = ToFloat(string);
					if (j == 1)
						v2.z = ToFloat(string);
					if (j == 2)
						v3.z = ToFloat(string);
				}
			}
		}

		//determine the largest projection dimension (the dimension that the polygon is generally on;
		//with a floor Y would be biggest)
		Ogre::Plane plane = Ogre::Plane(v1, v2, v3);
		Ogre::Vector3 normal = plane.normal;

		direction = 0; //x; faces left/right

		if (fabsf (normal.y) > fabsf (normal.x) && fabsf (normal.y) > fabsf (normal.z))
			direction = 1; //y biggest; faces up/down
		else if (fabsf (normal.z) > fabsf (normal.x))
			direction = 2; //z biggest; faces front/back
		else if (normal.x == 0)
			return false; //fail if normal vector is 0
	}

	return true;
}

void SBS::ResetTextureMapping(bool todefaults)
{
	//Resets UV texture mapping to defaults or previous values
	if (todefaults == true)
	{
		if (DefaultMapper == 0)
			SetPlanarMapping(false, false, false, false, false);
		if (DefaultMapper == 1)
			SetTextureMapping(0, Ogre::Vector2(0, 0), 1, Ogre::Vector2(1, 0), 2, Ogre::Vector2(1, 1));
		if (DefaultMapper == 2)
			SetTextureMapping2("x0", "y0", "z0", Ogre::Vector2(0, 0), "x1", "y1", "z1", Ogre::Vector2(1, 0), "x2", "y2", "z2", Ogre::Vector2(1, 1));
	}
	else
	{
		if (OldMapMethod == 0)
			SetPlanarMapping(OldPlanarFlat, OldRevX, OldRevY, OldRevZ, OldPlanarRotate);
		if (OldMapMethod == 1)
			SetTextureMapping(OldMapIndex[0], OldMapUV[0], OldMapIndex[1], OldMapUV[1], OldMapIndex[2], OldMapUV[2]);
		if (OldMapMethod == 2)
			SetTextureMapping2(OldMapVerts1[0], OldMapVerts1[1], OldMapVerts1[2], OldMapUV[0], OldMapVerts2[0], OldMapVerts2[1], OldMapVerts2[2], OldMapUV[1], OldMapVerts3[0], OldMapVerts3[1], OldMapVerts3[2], OldMapUV[2]);
	}
}

void SBS::SetPlanarMapping(bool flat, bool FlipX, bool FlipY, bool FlipZ, bool rotate)
{
	//sets planar texture mapping parameters
	//X, Y and Z reverse planar texture mapping per axis
	//Flat determines if depth should be ignored when mapping

	//first backup old parameters
	BackupMapping();

	//now set new parameters
	RevX = FlipX;
	RevY = FlipY;
	RevZ = FlipZ;
	MapUV[0] = Ogre::Vector2(0, 0);
	MapUV[1] = Ogre::Vector2(1, 0);
	MapUV[2] = Ogre::Vector2(1, 1);
	PlanarFlat = flat;
	MapMethod = 0;
	PlanarRotate = rotate;
}

void SBS::SetTextureMapping(int vertindex1, Ogre::Vector2 uv1, int vertindex2, Ogre::Vector2 uv2, int vertindex3, Ogre::Vector2 uv3)
{
	//Manually sets UV texture mapping.  Use ResetTextureMapping to return to default values

	BackupMapping();

	//set new values
	MapIndex[0] = vertindex1;
	MapIndex[1] = vertindex2;
	MapIndex[2] = vertindex3;
	MapUV[0] = uv1;
	MapUV[1] = uv2;
	MapUV[2] = uv3;
	MapMethod = 1;
}

void SBS::SetTextureMapping2(const std::string & x1, const std::string & y1, const std::string & z1, Ogre::Vector2 uv1, const std::string & x2, const std::string & y2, const std::string & z2, Ogre::Vector2 uv2, const std::string & x3, const std::string & y3, const std::string & z3, Ogre::Vector2 uv3)
{
	//Manually sets UV texture mapping (advanced version)
	//Use ResetTextureMapping to return to default values

	BackupMapping();

	MapVerts1[0] = x1;
	MapVerts1[1] = y1;
	MapVerts1[2] = z1;
	MapVerts2[0] = x2;
	MapVerts2[1] = y2;
	MapVerts2[2] = z2;
	MapVerts3[0] = x3;
	MapVerts3[1] = y3;
	MapVerts3[2] = z3;
	MapUV[0] = uv1;
	MapUV[1] = uv2;
	MapUV[2] = uv3;
	MapMethod = 2;
}

void SBS::BackupMapping()
{
	//backup texture mapping parameters
	if (MapMethod == 0)
	{
		OldRevX = RevX;
		OldRevY = RevY;
		OldRevZ = RevZ;
		OldPlanarFlat = PlanarFlat;
		OldPlanarRotate = PlanarRotate;
	}
	else
	{
		for (int i = 0; i <= 2; i++)
		{
			if (MapMethod == 1)
				OldMapIndex[i] = MapIndex[i];
			if (MapMethod == 2)
			{
				OldMapVerts1[i] = MapVerts1[i];
				OldMapVerts1[i] = MapVerts1[i];
				OldMapVerts1[i] = MapVerts1[i];
			}
		}
	}
	for (int i = 0; i <= 2; i++)
		OldMapUV[i] = MapUV[i];
	OldMapMethod = MapMethod;
}

void SBS::SetTextureFlip(int mainneg, int mainpos, int sideneg, int sidepos, int top, int bottom)
{
	//flip a texture on a specified side either horizontally or vertically (or both)
	//parameters are:
	//0 = no flipping
	//1 = flip horizontally
	//2 = flip vertically
	//3 = flip both

	mainnegflip = mainneg;
	mainposflip = mainpos;
	sidenegflip = sideneg;
	sideposflip = sidepos;
	topflip = top;
	bottomflip = bottom;
	FlipTexture = true;
}

void SBS::SetTextureOverride(const std::string &mainneg, const std::string &mainpos, const std::string &sideneg, const std::string &sidepos, const std::string &top, const std::string &bottom)
{
	//set override textures and enable override

	mainnegtex = TrimStringCopy(mainneg);
	mainpostex = TrimStringCopy(mainpos);
	sidenegtex = TrimStringCopy(sideneg);
	sidepostex = TrimStringCopy(sidepos);
	toptex = TrimStringCopy(top);
	bottomtex = TrimStringCopy(bottom);
	TextureOverride = true;
}

std::string SBS::ListTextures(bool show_filename)
{
	//list loaded textures

	std::string list;
	if (show_filename == true)
		list.append("Name --- Filename\n\n");

	for (int i = 0; i < (int)textureinfo.size(); i++)
	{
		list.append(textureinfo[i].name);
		if (show_filename == true)
		{
			list.append(", ");
			list.append(textureinfo[i].filename);
		}
		list.append("\n");
	}
	return list;
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
Ogre::TexturePtr SBS::loadChromaKeyedTexture(const std::string& filename, const std::string& resGroup, const std::string& name, const Ogre::ColourValue& keyCol, int numMipmaps, float threshold)
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
		int found = 0;

		//first, find graphics control extension (starts with 21:F9:04 hex)
		for (int i = 0; i < (int)encoded->size(); i++)
		{
			//extension found
			if (found == 3 && i < (int)encoded->size() - 4)
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
			if (check == 33 && i < (int)encoded->size() - 4)
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
			pixCol.a = ((fabsf(diffCol.r) < threshold) && (fabsf(diffCol.g) < threshold) && (fabsf(diffCol.b) < threshold)) ? 0.0f : 1.0f;
			Ogre::PixelUtil::packColour(pixCol, PF_A8R8G8B8, static_cast<void*>(pixelData + pxDataIndex));
			pxDataIndex += pxDataIndexStep;
		}
	}

	Image chromaKeyedImg;
	chromaKeyedImg.loadDynamicImage(pixelData, width, height, 1, PF_A8R8G8B8, true);
	// You could save the chroma keyed image at this point for caching:
	// chromaKeyedImg.save(resName);
	Ogre::TexturePtr mTex = TextureManager::getSingleton().loadImage(name, resGroup, chromaKeyedImg, TEX_TYPE_2D, numMipmaps);
	IncrementTextureCount();
	return mTex;
}

bool SBS::WriteToTexture(const std::string &str, Ogre::TexturePtr destTexture, int destLeft, int destTop, int destRight, int destBottom, Ogre::FontPtr font, const Ogre::ColourValue &color, char justify, char vert_justify, bool wordwrap)
{
	//justify is left 'l' by default - set to 'r' or 'c' for right or center
	//vert_justify is top 't' by default - set to 'c' or 'b' for center or bottom

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

	std::string texname = GetTextureName(font->getMaterial());
	TexturePtr fontTexture = GetTextureByName(texname);

	if (fontTexture.isNull())
		return false;

	//output glyph map to file
	//SaveTexture(fontTexture, "test.png");

	HardwarePixelBufferSharedPtr fontBuffer = fontTexture->getBuffer();
	HardwarePixelBufferSharedPtr destBuffer = destTexture->getBuffer();

	PixelBox destPb = destBuffer->lock(Box(0, 0, destTexture->getWidth() - 1, destTexture->getHeight() - 1), HardwareBuffer::HBL_NORMAL);

	// The font texture buffer was created write only...so we cannot read it back :o). One solution is to copy the buffer  instead of locking it. (Maybe there is a way to create a font texture which is not write_only ?)

	int index = -1;
	for (int i = 0; i < (int)textureboxes.size(); i++)
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
		index = (int)textureboxes.size();
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

	const int fontPixelSize = (int)PixelUtil::getNumElemBytes(textureboxes[index].box.format);
	const int destPixelSize = (int)PixelUtil::getNumElemBytes(destPb.format);

	const int fontRowPitchBytes = (int)textureboxes[index].box.rowPitch * fontPixelSize;
	const int destRowPitchBytes = (int)destPb.rowPitch * destPixelSize;

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
			GlyphTexCoords[i].left = Ogre::uint32(glypheTexRect.left * fontTexture->getSrcWidth());
			GlyphTexCoords[i].top = Ogre::uint32(glypheTexRect.top * fontTexture->getSrcHeight());
			GlyphTexCoords[i].right = Ogre::uint32(glypheTexRect.right * fontTexture->getSrcWidth());
			GlyphTexCoords[i].bottom = Ogre::uint32(glypheTexRect.bottom * fontTexture->getSrcHeight());

			//get true bottom of character, since the previous routine doesn't seem to get an accurate result
			int lastline = 0;
			for (int j = 0; j < (int)GlyphTexCoords[i].getHeight(); j++)
			{
				for (int k = 0; k < (int)GlyphTexCoords[i].getWidth(); k++)
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
				for (int i = 0; i < (int)GlyphTexCoords[strindex].getHeight(); i++)
				{
					for (int j = 0; j < (int)GlyphTexCoords[strindex].getWidth(); j++)
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

void SBS::SaveTexture(Ogre::TexturePtr texture, const std::string & filename)
{
	//save a raw texture to a file

	Ogre::Image image;
	texture->convertToImage(image);
	image.save(filename);
}

void SBS::IncrementTextureCount()
{
	texturecount++;
}

void SBS::DecrementTextureCount()
{
	texturecount--;
}

void SBS::IncrementMaterialCount()
{
	materialcount++;
}

void SBS::DecrementMaterialCount()
{
	materialcount--;
}

Ogre::TexturePtr SBS::LoadTexture(const std::string &filename, int mipmaps, bool &has_alpha, bool use_alpha_color, Ogre::ColourValue alpha_color)
{
	//set verbosity level
	Ogre::TextureManager::getSingleton().setVerbose(Verbose);

	std::string filename2 = filename;

	//determine if the file is a GIF image, to force keycolor alpha
	std::string extension = filename2.substr(filename.size() - 3);
	SetCase(extension, false);
	if (extension == "gif")
		use_alpha_color = true;

	//load the texture
	std::string path = GetMountPath(filename2, filename2);
	Ogre::TexturePtr mTex;
	std::string texturename;
	has_alpha = false;

	try
	{
		if (use_alpha_color == false)
		{
			mTex = Ogre::TextureManager::getSingleton().load(filename2, path, Ogre::TEX_TYPE_2D, mipmaps);
			IncrementTextureCount();

			if (mTex.isNull())
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
			mTex = loadChromaKeyedTexture(filename2, path, texturename, Ogre::ColourValue::White);

			if (mTex.isNull())
			{
				ReportError("Error loading texture" + filename2);
				return mTex;
			}
			has_alpha = true;
		}
	}
	catch (Ogre::Exception &e)
	{
		ReportError("Error loading texture " + filename2 + "\n" + e.getDescription());
		return mTex;
	}

	return mTex;
}

Ogre::MaterialPtr SBS::CreateMaterial(const std::string &name, const std::string &path)
{
	//unload material if already loaded
	if (UnloadMaterial(name, path) == true)
		UnregisterTextureInfo(name);

	//create new material
	Ogre::MaterialPtr mMat = Ogre::MaterialManager::getSingleton().create(ToString(InstanceNumber) + ":" + name, path);
	IncrementMaterialCount();
	mMat->setLightingEnabled(false);
	//mMat->setAmbient(AmbientR, AmbientG, AmbientB);

	//show only clockwise side of material
	mMat->setCullingMode(Ogre::CULL_ANTICLOCKWISE);

	return mMat;
}

Ogre::MaterialPtr SBS::GetMaterialByName(const std::string &name, const std::string &group)
{
	Ogre::MaterialPtr ptr;

	if (Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(group) == false)
		return ptr;

	ptr = Ogre::MaterialManager::getSingleton().getByName(ToString(InstanceNumber) + ":" + name, group);
	return ptr;
}

Ogre::TextureUnitState* SBS::BindTextureToMaterial(Ogre::MaterialPtr mMat, std::string texture_name, bool has_alpha)
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

Ogre::TextureUnitState* SBS::GetTextureUnitState(Ogre::MaterialPtr mMat)
{
	//get first texture unit state
	return mMat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
}

std::string SBS::GetTextureName(Ogre::MaterialPtr mMat)
{
	std::string texname = "";

	Ogre::TextureUnitState *state = GetTextureUnitState(mMat);
	if (state)
		texname = GetTextureUnitState(mMat)->getTextureName();

	return texname;
}

Ogre::TexturePtr SBS::GetTextureByName(const std::string &name, const std::string &group)
{
	Ogre::TexturePtr ptr;

	if (Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(group) == false)
		return ptr;

	ptr = Ogre::TextureManager::getSingleton().getByName(name, group);
	return ptr;
}

void SBS::UnloadMaterials()
{
	//delete all registered materials

	for (int i = 0; i < (int)textureinfo.size(); i++)
	{
		Ogre::MaterialManager::getSingleton().remove(ToString(InstanceNumber) + ":" + textureinfo[i].name);
	}
}

void SBS::CopyTexture(Ogre::TexturePtr source, Ogre::TexturePtr destination)
{
	//copy a source texture onto a destination texture using the full sizes

	Ogre::Box srcbox (0, 0, 0, source->getWidth(), source->getHeight(), source->getDepth());
	Ogre::Box dstbox (0, 0, 0, destination->getWidth(), destination->getHeight(), destination->getDepth());

	CopyTexture(source, destination, srcbox, dstbox);
}

void SBS::CopyTexture(Ogre::TexturePtr source, Ogre::TexturePtr destination, const Ogre::Box &srcBox, const Ogre::Box &dstBox)
{
	//copy a source texture onto a destination texture using specified sizes

	Ogre::HardwarePixelBufferSharedPtr buffer = source->getBuffer();

	buffer->lock(srcBox, Ogre::HardwareBuffer::HBL_READ_ONLY);
	const Ogre::PixelBox &pb = buffer->getCurrentLock();

	destination->getBuffer()->blitFromMemory(pb, dstBox);
	buffer->unlock();
}

}
