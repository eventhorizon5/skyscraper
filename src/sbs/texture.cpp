/* $Id$ */

/*
	Scalable Building Simulator - Texture Functions
	The Skyscraper Project - Version 1.8 Alpha
	Copyright (C)2004-2013 Ryan Thoryk
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
#include <OgreFont.h>
#include <OgreHardwarePixelBuffer.h>
#include "globals.h"
#include "sbs.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine

bool SBS::LoadTexture(const char *filename, const char *name, float widthmult, float heightmult, bool enable_force, bool force_mode, int mipmaps, bool use_alpha_color, Ogre::ColourValue alpha_color)
{
	//set verbosity level
	Ogre::TextureManager::getSingleton().setVerbose(Verbose);

	//first verify the filename
	std::string filename2 = VerifyFile(filename);

	//determine if the file is a GIF image, to force keycolor alpha
	std::string extension = filename2.substr(filename2.size() - 3);
	SetCase(extension, false);
	if (extension == "gif")
		use_alpha_color = true;

	// Load the texture
	std::string path = GetMountPath(filename2.c_str(), filename2);
	Ogre::TexturePtr mTex;
	std::string texturename;
	bool has_alpha = false;

	try
	{
		if (use_alpha_color == false)
		{
			mTex = Ogre::TextureManager::getSingleton().load(filename2, path, Ogre::TEX_TYPE_2D, mipmaps);
			IncrementTextureCount();

			if (mTex.isNull())
				return ReportError("Error loading texture" + filename2);
			texturename = mTex->getName();
			has_alpha = mTex->hasAlpha();
		}
		else
		{
			//load based on chroma key for alpha

			texturename = "kc_" + filename2;
			loadChromaKeyedTexture(filename2, path, texturename, Ogre::ColourValue::White);
			has_alpha = true;
		}
	}
	catch (Ogre::Exception &e)
	{
		return ReportError("Error loading texture " + filename2 + "\n" + e.getDescription());
	}

	//create a new material
	std::string matname = name;
	TrimString(matname);
	Ogre::MaterialPtr mMat = Ogre::MaterialManager::getSingleton().create(matname, "General");
	IncrementMaterialCount();
	mMat->setLightingEnabled(true);
	mMat->setAmbient(AmbientR, AmbientG, AmbientB);

	//bind texture to material
	mMat->getTechnique(0)->getPass(0)->createTextureUnitState(texturename);

	//show only clockwise side of material
	mMat->setCullingMode(Ogre::CULL_ANTICLOCKWISE);

	//enable alpha blending for related textures
	if (has_alpha == true)
	{
		//mMat->setDepthWriteEnabled(false);
		//mMat->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);

		//enable hard alpha for alpha mask values 128 and above
		mMat->getTechnique(0)->getPass(0)->setAlphaRejectSettings(Ogre::CMPF_GREATER_EQUAL, 128);
	}

	Report("Loaded texture " + filename2);

	TextureInfo info;
	info.name = matname;
	info.widthmult = widthmult;
	info.heightmult = heightmult;
	info.enable_force = enable_force;
	info.force_mode = force_mode;
	textureinfo.push_back(info);
	return true;
}

bool SBS::LoadAnimatedTexture(std::vector<std::string> filenames, const char *name, float duration, float widthmult, float heightmult, bool enable_force, bool force_mode, int mipmaps, bool use_alpha_color, Ogre::ColourValue alpha_color)
{
	//set verbosity level
	Ogre::TextureManager::getSingleton().setVerbose(Verbose);

	std::vector<std::string> filenames2;

	int num_frames = filenames.size();

	//first verify the filenames
	for (int i = 0; i < filenames.size(); i++)
	{
		TrimString(filenames[i]);
		filenames2.push_back(VerifyFile(filenames[i].c_str()));
	}

	bool has_alpha = false;

	for (int i = 0; i < filenames2.size(); i++)
	{
		//determine if the file is a GIF image, to force keycolor alpha
		std::string extension = filenames2[i].substr(filenames2[i].size() - 3);
		SetCase(extension, false);
		if (extension == "gif")
			use_alpha_color = true;

		// Load the texture
		std::string path = GetMountPath(filenames2[i].c_str(), filenames2[i]);
		Ogre::TexturePtr mTex;
		std::string texturename;

		try
		{
			if (use_alpha_color == false)
			{
				mTex = Ogre::TextureManager::getSingleton().load(filenames2[i], path, Ogre::TEX_TYPE_2D, mipmaps);
				IncrementTextureCount();

				if (mTex.isNull())
					return ReportError("Error loading texture" + filenames2[i]);
				texturename = mTex->getName();
				has_alpha = mTex->hasAlpha();
			}
			else
			{
				//load based on chroma key for alpha

				texturename = "kc_" + filenames2[i];
				loadChromaKeyedTexture(filenames2[i], path, texturename, Ogre::ColourValue::White);
				has_alpha = true;
			}
		}
		catch (Ogre::Exception &e)
		{
			return ReportError("Error loading texture " + filenames2[i] + "\n" + e.getDescription());
		}

		Report("Loaded texture " + filenames2[i]);
	}

	//create a new material
	std::string matname = name;
	TrimString(matname);
	Ogre::MaterialPtr mMat = Ogre::MaterialManager::getSingleton().create(matname, "General");
	IncrementMaterialCount();
	mMat->setLightingEnabled(true);
	mMat->setAmbient(AmbientR, AmbientG, AmbientB);

	//bind first texture to material
	Ogre::TextureUnitState* state = mMat->getTechnique(0)->getPass(0)->createTextureUnitState(filenames2[0]);

	//show only clockwise side of material
	mMat->setCullingMode(Ogre::CULL_ANTICLOCKWISE);

	//enable alpha blending for related textures
	if (has_alpha == true)
	{
		//mMat->setDepthWriteEnabled(false);
		//mMat->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);

		//enable hard alpha for alpha mask values 128 and above
		mMat->getTechnique(0)->getPass(0)->setAlphaRejectSettings(Ogre::CMPF_GREATER_EQUAL, 128);
	}

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

	Report("Loaded animated texture " + matname);

	TextureInfo info;
	info.name = matname;
	info.widthmult = widthmult;
	info.heightmult = heightmult;
	info.enable_force = enable_force;
	info.force_mode = force_mode;
	textureinfo.push_back(info);
	return true;
}

bool SBS::LoadAlphaBlendTexture(const char *filename, const char *specular_filename, const char *blend_filename, const char *name, bool spherical, float widthmult, float heightmult, bool enable_force, bool force_mode, int mipmaps, bool use_alpha_color, Ogre::ColourValue alpha_color)
{
	//set verbosity level
	Ogre::TextureManager::getSingleton().setVerbose(Verbose);

	//first verify the filenames
	std::string filename2 = VerifyFile(filename);
	std::string specular_filename2 = VerifyFile(specular_filename);
	std::string blend_filename2 = VerifyFile(blend_filename);

	//determine if the file is a GIF image, to force keycolor alpha
	std::string extension = filename2.substr(filename2.size() - 3);
	SetCase(extension, false);
	if (extension == "gif")
		use_alpha_color = true;

	// Load the texture
	std::string path = GetMountPath(filename2.c_str(), filename2);
	Ogre::TexturePtr mTex;
	std::string texturename, specular_texturename, blend_texturename;
	bool has_alpha = false;

	try
	{
		if (use_alpha_color == false)
		{
			mTex = Ogre::TextureManager::getSingleton().load(filename2, path, Ogre::TEX_TYPE_2D, mipmaps);
			IncrementTextureCount();

			if (mTex.isNull())
				return ReportError("Error loading texture" + filename2);
			texturename = mTex->getName();
			has_alpha = mTex->hasAlpha();
		}
		else
		{
			//load based on chroma key for alpha

			texturename = "kc_" + filename2;
			loadChromaKeyedTexture(filename2, path, texturename, Ogre::ColourValue::White);
			has_alpha = true;
		}
	}
	catch (Ogre::Exception &e)
	{
		return ReportError("Error loading texture " + filename2 + "\n" + e.getDescription());
	}

	//load specular texture
	try
	{
		mTex = Ogre::TextureManager::getSingleton().load(specular_filename2, path, Ogre::TEX_TYPE_2D, mipmaps);
		IncrementTextureCount();

		if (mTex.isNull())
			return ReportError("Error loading texture" + specular_filename2);
		specular_texturename = mTex->getName();
	}
	catch (Ogre::Exception &e)
	{
		return ReportError("Error loading texture " + specular_filename2 + "\n" + e.getDescription());
	}

	//load blend texture
	try
	{
		mTex = Ogre::TextureManager::getSingleton().load(blend_filename2, path, Ogre::TEX_TYPE_2D, mipmaps);
		IncrementTextureCount();

		if (mTex.isNull())
			return ReportError("Error loading texture" + blend_filename2);
		blend_texturename = mTex->getName();
	}
	catch (Ogre::Exception &e)
	{
		return ReportError("Error loading texture " + blend_filename2 + "\n" + e.getDescription());
	}

	//create a new material
	std::string matname = name;
	TrimString(matname);
	Ogre::MaterialPtr mMat = Ogre::MaterialManager::getSingleton().create(matname, "General");
	IncrementMaterialCount();
	mMat->setLightingEnabled(true);
	mMat->setAmbient(AmbientR, AmbientG, AmbientB);

	//bind texture to material
	Ogre::TextureUnitState* state1 = mMat->getTechnique(0)->getPass(0)->createTextureUnitState(texturename);
	Ogre::TextureUnitState* state2 = mMat->getTechnique(0)->getPass(0)->createTextureUnitState(specular_texturename);
	Ogre::TextureUnitState* state3 = mMat->getTechnique(0)->getPass(0)->createTextureUnitState(blend_texturename);

	if (state2)
		state2->setColourOperation(Ogre::LBO_ALPHA_BLEND);

	if (state3)
	{
		state3->setColourOperationEx(Ogre::LBX_BLEND_CURRENT_ALPHA);
		if (spherical == true)
			state3->setEnvironmentMap(true, Ogre::TextureUnitState::ENV_CURVED);
		else
			state3->setEnvironmentMap(true, Ogre::TextureUnitState::ENV_PLANAR);
	}

	//show only clockwise side of material
	mMat->setCullingMode(Ogre::CULL_ANTICLOCKWISE);

	//enable alpha blending for related textures
	if (has_alpha == true)
	{
		//mMat->setDepthWriteEnabled(false);
		//mMat->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);

		//enable hard alpha for alpha mask values 128 and above
		mMat->getTechnique(0)->getPass(0)->setAlphaRejectSettings(Ogre::CMPF_GREATER_EQUAL, 128);
	}

	Report("Loaded alpha blended texture " + filename2);

	TextureInfo info;
	info.name = matname;
	info.widthmult = widthmult;
	info.heightmult = heightmult;
	info.enable_force = enable_force;
	info.force_mode = force_mode;
	textureinfo.push_back(info);
	return true;
}

bool SBS::LoadMaterial(const char *materialname, const char *name, float widthmult, float heightmult, bool enable_force, bool force_mode)
{
	//set verbosity level
	Ogre::MaterialManager::getSingleton().setVerbose(Verbose);
	Ogre::MaterialPtr mMat;
	std::string matname = materialname;

	try
	{
		mMat = Ogre::MaterialManager::getSingleton().getByName(matname, "Materials");

		if (mMat.isNull())
			return ReportError("Error loading material " + matname);
	}
	catch (Ogre::Exception &e)
	{
		return ReportError("Error loading material " + matname + "\n" + e.getDescription());
	}

	//show only clockwise side of material
	mMat->setCullingMode(Ogre::CULL_ANTICLOCKWISE);

	Report("Loaded material " + matname);

	TextureInfo info;
	info.name = name;
	info.material_name = matname;
	info.widthmult = widthmult;
	info.heightmult = heightmult;
	info.enable_force = enable_force;
	info.force_mode = force_mode;
	textureinfo.push_back(info);
	return true;
}

bool SBS::UnloadTexture(const char *name)
{
	//unloads a texture

	Ogre::TexturePtr wrapper = Ogre::TextureManager::getSingleton().getByName(name);
	if (!wrapper.isNull())
		return false;
	Ogre::TextureManager::getSingleton().remove(name);
	DecrementTextureCount();

	Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName(name);
	if (!wrapper.isNull())
		return false;
	Ogre::MaterialManager::getSingleton().remove(name);
	DecrementMaterialCount();

	return true;
}

bool SBS::LoadTextureCropped(const char *filename, const char *name, int x, int y, int width, int height, float widthmult, float heightmult, bool enable_force, bool force_mode)
{
	//loads only a portion of the specified texture

	Ogre::ColourValue alpha_color = Ogre::ColourValue::Black;
	int mipmaps = -1;
	bool use_alpha_color = false;

	//set verbosity level
	Ogre::TextureManager::getSingleton().setVerbose(Verbose);

	//first verify the filename
	std::string filename2 = VerifyFile(filename);

	//determine if the file is a GIF image, to force keycolor alpha
	std::string extension = filename2.substr(filename2.size() - 3);
	SetCase(extension, false);
	if (extension == "gif")
		use_alpha_color = true;

	// Load the texture
	std::string path = GetMountPath(filename2.c_str(), filename2);
	Ogre::TexturePtr mTex;
	std::string texturename;
	bool has_alpha = false;

	try
	{
		if (use_alpha_color == false)
		{
			mTex = Ogre::TextureManager::getSingleton().load(filename2, path, Ogre::TEX_TYPE_2D, mipmaps);
			IncrementTextureCount();

			if (mTex.isNull())
				return ReportError("Error loading texture" + filename2);
			texturename = mTex->getName();
			has_alpha = mTex->hasAlpha();
		}
		else
		{
			//load based on chroma key for alpha

			texturename = "kc_" + filename2;
			loadChromaKeyedTexture(filename2, path, texturename, Ogre::ColourValue::White);
			has_alpha = true;
		}
	}
	catch (Ogre::Exception &e)
	{
		return ReportError("Error loading texture " + filename2 + "\n" + e.getDescription());
	}

	std::string Name = name;
	std::string Filename = filename;

	//set default values if specified
	if (x == -1)
		x = 0;
	if (y == -1)
		y = 0;
	if (width < 1)
		width = mTex->getWidth();
	if (height < 1)
		height = mTex->getHeight();

	if (x > mTex->getWidth() || y > mTex->getHeight())
		return ReportError("LoadTextureCropped: invalid coordinates for '" + Name + "'");
	if (x + width > mTex->getWidth() || y + height > mTex->getHeight())
		return ReportError("LoadTextureCropped: invalid size for '" + Name + "'");

	//create new empty texture
	Ogre::TexturePtr new_texture = Ogre::TextureManager::getSingleton().createManual(Name, "General", Ogre::TEX_TYPE_2D, width, height, Ogre::MIP_UNLIMITED, Ogre::PF_R8G8B8A8, Ogre::TU_STATIC|Ogre::TU_AUTOMIPMAP);
	IncrementTextureCount();

	//copy source and overlay images onto new image
	Ogre::Box source (x, y, x + width, y + height);
	Ogre::Box dest (0, 0, width, height);
	new_texture->getBuffer()->blit(mTex->getBuffer(), source, dest);

	//create a new material
	Ogre::MaterialPtr mMat = Ogre::MaterialManager::getSingleton().create(Name, "General");
	IncrementMaterialCount();
	mMat->setLightingEnabled(true);
	mMat->setAmbient(AmbientR, AmbientG, AmbientB);

	//bind texture to material
	mMat->getTechnique(0)->getPass(0)->createTextureUnitState(Name);

	//show only clockwise side of material
	mMat->setCullingMode(Ogre::CULL_ANTICLOCKWISE);

	//enable alpha blending for related textures
	if (has_alpha == true)
	{
		//mMat->setDepthWriteEnabled(false);
		//mMat->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);

		//enable hard alpha for alpha mask values 128 and above
		mMat->getTechnique(0)->getPass(0)->setAlphaRejectSettings(Ogre::CMPF_GREATER_EQUAL, 128);
	}

	//add texture multipliers for new texture
	TextureInfo info;
	info.name = name;
	info.widthmult = widthmult;
	info.heightmult = heightmult;
	info.enable_force = enable_force;
	info.force_mode = force_mode;
	textureinfo.push_back(info);

	return true;
}

bool SBS::RotateTexture(const char *name, float angle)
{
	//set a fixed rotation value for a texture
	bool result;
	std::string texname = name;
	TrimString(texname);
	std::string material = GetTextureMaterial(name, result, false);

	if (!result)
		return ReportError("RotateTexture: invalid texture " + texname + "\n");

	//get material name
	Ogre::MaterialPtr mMat = Ogre::MaterialManager::getSingleton().getByName(material);

	//get first texture unit state
	Ogre::TextureUnitState* state;
	if (!mMat.isNull())
		state = mMat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
	else
		return false;

	//set rotation
	if (state)
		state->setTextureRotate(Ogre::Degree(-angle));
	else
		return false;

	return true;
}

bool SBS::RotateAnimTexture(const char *name, float speed)
{
	//set a rotation animation for a texture - speed is in revolutions per second

	bool result;
	std::string texname = name;
	TrimString(texname);
	std::string material = GetTextureMaterial(name, result, false);

	if (!result)
		return ReportError("RotateAnimTexture: invalid texture " + texname + "\n");

	//get material name
	Ogre::MaterialPtr mMat = Ogre::MaterialManager::getSingleton().getByName(material);

	//get first texture unit state
	Ogre::TextureUnitState* state;
	if (!mMat.isNull())
		state = mMat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
	else
		return false;

	//set rotation animation
	if (state)
		state->setRotateAnimation(speed);
	else
		return false;

	return true;
}

bool SBS::ScrollTexture(const char *name, float x_offset, float y_offset)
{
	//set a fixed scroll amount for a texture

	bool result;
	std::string texname = name;
	TrimString(texname);
	std::string material = GetTextureMaterial(name, result, false);

	if (!result)
		return ReportError("ScrollTexture: invalid texture " + texname + "\n");

	//get material name
	Ogre::MaterialPtr mMat = Ogre::MaterialManager::getSingleton().getByName(material);

	//get first texture unit state
	Ogre::TextureUnitState* state;
	if (!mMat.isNull())
		state = mMat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
	else
		return false;

	//set scrolling value
	if (state)
		state->setTextureScroll(-x_offset, y_offset); //invert x value
	else
		return false;

	return true;
}

bool SBS::ScrollAnimTexture(const char *name, float x_speed, float y_speed)
{
	//set a scroll animation for a texture - speed is in revolutions per second

	bool result;
	std::string texname = name;
	TrimString(texname);
	std::string material = GetTextureMaterial(name, result, false);

	if (!result)
		return ReportError("ScrollAnimTexture: invalid texture " + texname + "\n");

	//get material name
	Ogre::MaterialPtr mMat = Ogre::MaterialManager::getSingleton().getByName(material);

	//get first texture unit state
	Ogre::TextureUnitState* state;
	if (!mMat.isNull())
		state = mMat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
	else
		return false;

	//set scrolling animation
	if (state)
		state->setScrollAnimation(x_speed, -y_speed);
	else
		return false;

	return true;
}

bool SBS::ScaleTexture(const char *name, float x_scale, float y_scale)
{
	//set a fixed scale amount for a texture

	bool result;
	std::string texname = name;
	TrimString(texname);
	std::string material = GetTextureMaterial(name, result, false);

	if (!result)
		return ReportError("ScaleTexture: invalid texture " + texname + "\n");

	//get material name
	Ogre::MaterialPtr mMat = Ogre::MaterialManager::getSingleton().getByName(material);

	//get first texture unit state
	Ogre::TextureUnitState* state;
	if (!mMat.isNull())
		state = mMat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
	else
		return false;

	//set scale
	if (state)
		state->setTextureScale(x_scale, y_scale);
	else
		return false;

	return true;
}

bool SBS::TransformTexture(const char *name, const char *type, const char *wave_type, float base, float frequency, float phase, float amplitude)
{
	//set a transformation type for a texture

	bool result;
	std::string texname = name;
	TrimString(texname);
	std::string material = GetTextureMaterial(name, result, false);

	std::string xform_type = type;
	TrimString(xform_type);
	SetCase(xform_type, false);
	std::string wave = wave_type;
	TrimString(wave);
	SetCase(wave, false);

	if (!result)
		return ReportError("TransformTexture: invalid texture " + texname + "\n");

	//get material name
	Ogre::MaterialPtr mMat = Ogre::MaterialManager::getSingleton().getByName(material);

	//get first texture unit state
	Ogre::TextureUnitState* state;
	if (!mMat.isNull())
		state = mMat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
	else
		return false;

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

bool SBS::AddTextToTexture(const char *origname, const char *name, const char *font_filename, float font_size, const char *text, int x1, int y1, int x2, int y2, const char *h_align, const char *v_align, int ColorR, int ColorG, int ColorB, bool enable_force, bool force_mode)
{
	//adds text to the named texture, in the given box coordinates and alignment

	//h_align is either "left", "right" or "center" - default is center
	//v_align is either "top", "bottom", or "center" - default is center

	//if either x1 or y1 are -1, the value of 0 is used.
	//If either x2 or y2 are -1, the width or height of the texture is used.

	std::string hAlign = h_align;
	std::string vAlign = v_align;
	std::string Name = name;
	std::string Origname = origname;
	std::string Text = text;
	TrimString(Text);
	TrimString(Name);
	TrimString(Origname);

	std::string font_filename2 = VerifyFile(font_filename);

	//load font
	Ogre::FontPtr font;
	Ogre::String fontname = font_filename2 + Ogre::StringConverter::toString(font_size);
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
			font->addCodePointRange(Ogre::Font::CodePointRange(48, 122));
			font->load();
		}
		catch (Ogre::Exception &e)
		{
			sbs->ReportError("Error loading font " + fontname + "\n" + e.getDescription());
			return false;
		}
	}

	//get original texture
	Ogre::MaterialPtr ptr = Ogre::MaterialManager::getSingleton().getByName(Origname);
	if (ptr.isNull())
	{
		ReportError("AddTextToTexture: Invalid original material '" + Origname + "'");
		return false;
	}
	std::string texname = ptr->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName();
	Ogre::TexturePtr background = Ogre::TextureManager::getSingleton().getByName(texname);
	if (background.isNull())
	{
		ReportError("AddTextToTexture: Invalid original texture '" + texname + "'");
		return false;
	}

	bool has_alpha = background->hasAlpha();

	//get texture tiling info
	float widthmult, heightmult;
	GetTextureTiling(origname, widthmult, heightmult);

	//get height and width of texture
	int width, height;
	width = background->getWidth();
	height = background->getHeight();

	//create new empty texture
	Ogre::TexturePtr texture = Ogre::TextureManager::getSingleton().createManual(Name, "General", Ogre::TEX_TYPE_2D, width, height, Ogre::MIP_UNLIMITED, Ogre::PF_R8G8B8A8, Ogre::TU_STATIC|Ogre::TU_AUTOMIPMAP);
	IncrementTextureCount();

	//get new texture dimensions, if it was resized
	width = texture->getWidth();
	height = texture->getHeight();

	//add texture multipliers for new texture
	TextureInfo info;
	info.name = name;
	info.widthmult = widthmult;
	info.heightmult = heightmult;
	info.enable_force = enable_force;
	info.force_mode = force_mode;
	textureinfo.push_back(info);

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
	texture->getBuffer()->blit(background->getBuffer());

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

	bool result = WriteToTexture(Text, texture, Ogre::Box(x1, y1, x2, y2), font, Ogre::ColourValue(red, green, blue, 1.0), align, valign);
	if (result == false)
		return false;

	//create a new material
	Ogre::MaterialPtr mMat = Ogre::MaterialManager::getSingleton().create(Name, "General");
	IncrementMaterialCount();
	mMat->setLightingEnabled(true);
	mMat->setAmbient(AmbientR, AmbientG, AmbientB);

	//bind texture to material
	mMat->getTechnique(0)->getPass(0)->createTextureUnitState(Name);

	//show only clockwise side of material
	mMat->setCullingMode(Ogre::CULL_ANTICLOCKWISE);

	//enable alpha blending for related textures
	if (has_alpha == true)
	{
		//mMat->setDepthWriteEnabled(false);
		//mMat->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);

		//enable hard alpha for alpha mask values 128 and above
		mMat->getTechnique(0)->getPass(0)->setAlphaRejectSettings(Ogre::CMPF_GREATER_EQUAL, 128);
	}

	Report("AddTextToTexture: created texture " + Name);
	CacheFilename(Name, Name);
	return true;
}

bool SBS::AddTextureOverlay(const char *orig_texture, const char *overlay_texture, const char *name, int x, int y, int width, int height, float widthmult, float heightmult, bool enable_force, bool force_mode)
{
	//draws the specified texture on top of another texture
	//orig_texture is the original texture to use; overlay_texture is the texture to draw on top of it

	std::string Name = name;
	std::string Origname = orig_texture;
	std::string Overlay = overlay_texture;

	//get original texture
	Ogre::MaterialPtr ptr = Ogre::MaterialManager::getSingleton().getByName(Origname);
	if (ptr.isNull())
	{
		ReportError("AddTextureOverlay: Invalid original material '" + Origname + "'");
		return false;
	}
	std::string texname = ptr->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName();
	Ogre::TexturePtr image1 = Ogre::TextureManager::getSingleton().getByName(texname);
	if (image1.isNull())
	{
		ReportError("AddTextureOverlay: Invalid original texture '" + texname + "'");
		return false;
	}

	bool has_alpha = image1->hasAlpha();

	//get overlay texture
	ptr = Ogre::MaterialManager::getSingleton().getByName(Overlay);
	if (ptr.isNull())
	{
		ReportError("AddTextureOverlay: Invalid overlay material '" + Overlay + "'");
		return false;
	}
	texname = ptr->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName();
	Ogre::TexturePtr image2 = Ogre::TextureManager::getSingleton().getByName(texname);
	if (image2.isNull())
	{
		ReportError("AddTextureOverlay: Invalid overlay texture '" + texname + "'");
		return false;
	}

	//set default values if specified
	if (x == -1)
		x = 0;
	if (y == -1)
		y = 0;
	if (width < 1)
		width = image2->getWidth();
	if (height < 1)
		height = image2->getHeight();

	if (x > image1->getWidth() || y > image1->getHeight())
		return ReportError("AddTextureOverlay: invalid coordinates for '" + Name + "'");
	if (x + width > image1->getWidth() || y + height > image1->getHeight())
		return ReportError("AddTextureOverlay: invalid size for '" + Name + "'");

	//create new empty texture
	Ogre::TexturePtr new_texture = Ogre::TextureManager::getSingleton().createManual(Name, "General", Ogre::TEX_TYPE_2D, image1->getWidth(), image1->getHeight(), Ogre::MIP_UNLIMITED, Ogre::PF_R8G8B8A8, Ogre::TU_STATIC|Ogre::TU_AUTOMIPMAP);
	IncrementTextureCount();

	//copy source and overlay images onto new image
	Ogre::Box source (x, y, x + width, y + height);
	Ogre::Box source_full (0, 0, image1->getWidth(), image1->getHeight());
	Ogre::Box overlay (0, 0, image2->getWidth(), image2->getHeight());
	new_texture->getBuffer()->blit(image1->getBuffer(), source_full, source_full);
	new_texture->getBuffer()->blit(image2->getBuffer(), overlay, source);

	//create a new material
	Ogre::MaterialPtr mMat = Ogre::MaterialManager::getSingleton().create(Name, "General");
	IncrementMaterialCount();
	mMat->setLightingEnabled(true);
	mMat->setAmbient(AmbientR, AmbientG, AmbientB);

	//bind texture to material
	mMat->getTechnique(0)->getPass(0)->createTextureUnitState(Name);

	//show only clockwise side of material
	mMat->setCullingMode(Ogre::CULL_ANTICLOCKWISE);

	//enable alpha blending for related textures
	if (has_alpha == true)
	{
		//mMat->setDepthWriteEnabled(false);
		//mMat->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);

		//enable hard alpha for alpha mask values 128 and above
		mMat->getTechnique(0)->getPass(0)->setAlphaRejectSettings(Ogre::CMPF_GREATER_EQUAL, 128);
	}

	//add texture multipliers for new texture
	TextureInfo info;
	info.name = name;
	info.widthmult = widthmult;
	info.heightmult = heightmult;
	info.enable_force = enable_force;
	info.force_mode = force_mode;
	textureinfo.push_back(info);

	return true;
}

std::string SBS::GetTextureMaterial(const char *name, bool &result, bool report, const char *polygon_name)
{
	//perform a lookup on a texture, and return material name if it exists, or "Default" if not
	//returns false in &result if texture load failed, and if default material was used instead

	//use material_name value instead of name, if loaded as a material script instead of a direct texture
	//if report is true and texture is not found, issue warning
	std::string matname = name;
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
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(matname);

	std::string final_material = matname;

	if (!material.get())
	{
		//if material's not found, display a warning and use a default material
		std::string message;
		if (report == true)
		{
			if (polygon_name)
				message = "Texture '" + matname + "' not found for polygon '" + std::string(polygon_name) + "'; using default material";
			else
				message = "Texture '" + matname + "' not found; using default material";
			ReportError(message);
		}

		//set to default material
		final_material = "Default";
		result = false;
	}
	else
		result = true;
	return final_material;
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

bool SBS::GetTextureTiling(const char *texture, float &tw, float &th)
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

bool SBS::GetTextureForce(const char *texture, bool &enable_force, bool &force_mode)
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
			return fabs(n1 - n2) * offset;
		else
			return offset;
	}
	else
	{
		if ((AutoY == true && enable_force == false) || (enable_force == true && force_mode == true))
			return fabs(n1 - n2) * offset;
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

Ogre::Vector2 SBS::CalculateSizing(const char *texture, const Ogre::Vector3 &v1, const Ogre::Vector3 &v2, const Ogre::Vector3 &v3, int direction, float tw, float th)
{
	//calculate texture autosizing based on polygon extents

	//Call texture autosizing formulas
	float tw2 = tw, th2 = th;

	bool force_enable = false, force_mode = false;
	bool result = GetTextureForce(texture, force_enable, force_mode);

	float width = v1.distance(v2);
	float height = v2.distance(v3);

	tw2 = AutoSize(0, width, true, tw, force_enable, force_mode);
	th2 = AutoSize(0, height, false, th, force_enable, force_mode);

	//return results
	return Ogre::Vector2(tw2, th2);
}

void SBS::GetTextureMapping(std::vector<Ogre::Vector3> &vertices, Ogre::Vector3 &v1, Ogre::Vector3 &v2, Ogre::Vector3 &v3, int &direction)
{
	//returns texture mapping coordinates for the specified polygon index, in the v1, v2, and v3 vectors
	//this performs one of 3 methods - planar mapping, index mapping and manual vertex mapping

	if (MapMethod == 0)
	{
		//planar method

		Ogre::Vector2 x, y, z;
		std::vector<Ogre::Vector3> varray1, varray2;
		bool rev_x = false, rev_y = false, rev_z = false;

		//copy vertices into polygon object
		varray1.reserve(vertices.size());
		for (int i = 0; i < (int)vertices.size(); i++)
			varray1.push_back(vertices[i]);

		//determine the largest projection dimension (the dimension that the polygon is generally on;
		//with a floor Y would be biggest)
		Ogre::Plane plane = Ogre::Plane(varray1[0], varray1[1], varray1[2]);
		Ogre::Vector3 normal = plane.normal;

		direction = 0; //x; faces left/right

		if (fabsf (normal.y) > fabsf (normal.x) && fabsf (normal.y) > fabsf (normal.z))
			direction = 1; //y biggest; faces up/down
		else if (fabsf (normal.z) > fabsf (normal.x))
			direction = 2; //z biggest; faces front/back

		size_t selX = (1 << direction) & 0x3;
		size_t selY = (1 << selX) & 0x3;

		varray2.reserve(varray1.size());
		for (int i = 0; i < (int)varray1.size(); i++)
		{
			Ogre::Vector3 tmpvertex = varray1[i];
			varray2.push_back(Ogre::Vector3(tmpvertex[selX], tmpvertex[selY], 0));
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
		a = GetExtents(varray2, 1);
		b = GetExtents(varray2, 2);

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
				int location = string.find("x");
				if (location >= 0)
				{
					std::string number = string.substr(location + 1);
					if (atoi(number.c_str()) < (int)vertices.size())
						ReplaceAll(string, std::string("x" + number).c_str(), _gcvt(vertices[atoi(number.c_str())].x, 12, buffer));
					else
						ReplaceAll(string, std::string("x" + number).c_str(), "0"); //number value out of bounds
				}

				//find Y component
				location = string.find("y");
				if (location >= 0)
				{
					std::string number = string.substr(location + 1);
					if (atoi(number.c_str()) < (int)vertices.size())
						ReplaceAll(string, std::string("y" + number).c_str(), _gcvt(vertices[atoi(number.c_str())].y, 12, buffer));
					else
						ReplaceAll(string, std::string("y" + number).c_str(), "0"); //number value out of bounds
				}

				//find Z component
				location = string.find("z");
				if (location >= 0)
				{
					std::string number = string.substr(location + 1);
					if (atoi(number.c_str()) < (int)vertices.size())
						ReplaceAll(string, std::string("z" + number).c_str(), _gcvt(vertices[atoi(number.c_str())].z, 12, buffer));
					else
						ReplaceAll(string, std::string("z" + number).c_str(), "0"); //number value out of bounds
				}

				//store values
				if (i == 0)
				{
					if (j == 0)
						v1.x = atof(string.c_str());
					if (j == 1)
						v2.x = atof(string.c_str());
					if (j == 2)
						v3.x = atof(string.c_str());
				}
				if (i == 1)
				{
					if (j == 0)
						v1.y = atof(string.c_str());
					if (j == 1)
						v2.y = atof(string.c_str());
					if (j == 2)
						v3.y = atof(string.c_str());
				}
				if (i == 2)
				{
					if (j == 0)
						v1.z = atof(string.c_str());
					if (j == 1)
						v2.z = atof(string.c_str());
					if (j == 2)
						v3.z = atof(string.c_str());
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
	}
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

void SBS::SetTextureMapping2(std::string x1, std::string y1, std::string z1, Ogre::Vector2 uv1, std::string x2, std::string y2, std::string z2, Ogre::Vector2 uv2, std::string x3, std::string y3, std::string z3, Ogre::Vector2 uv3)
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

void SBS::SetTextureOverride(const char *mainneg, const char *mainpos, const char *sideneg, const char *sidepos, const char *top, const char *bottom)
{
	//set override textures and enable override
	mainnegtex = mainneg;
	TrimString(mainnegtex);
	mainpostex = mainpos;
	TrimString(mainpostex);
	sidenegtex = sideneg;
	TrimString(sidenegtex);
	sidepostex = sidepos;
	TrimString(sidepostex);
	toptex = top;
	TrimString(toptex);
	bottomtex = bottom;
	TrimString(bottomtex);
	TextureOverride = true;
}

std::string SBS::ListTextures()
{
	//list loaded textures

	std::string list;
	for (int i = 0; i < textureinfo.size(); i++)
	{
		list.append(textureinfo[i].name);
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
     IncrementTextureCount();
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
	//SaveTexture(fontTexture, "test.png");

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

void SBS::SaveTexture(Ogre::TexturePtr texture, std::string filename)
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
