/*
	Scalable Building Simulator - Texture Manager
	The Skyscraper Project - Version 2.1
	Copyright (C)2004-2024 Ryan Thoryk
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
#include "globals.h"
#include "sbs.h"
#include "utility.h"
#include "texture.h"

namespace SBS {

TextureManager::TextureManager(Object *parent) : ObjectBase(parent)
{
	SetName("Texture Manager");

	AutoX = true;
	AutoY = true;
	MapIndex.resize(3);
	MapUV.resize(3);
	OldMapIndex.resize(3);
	OldMapUV.resize(3);
	MapVerts1.resize(3);
	MapVerts2.resize(3);
	MapVerts3.resize(3);
	OldMapVerts1.resize(3);
	OldMapVerts2.resize(3);
	OldMapVerts3.resize(3);
	MapMethod = 0;
	OldMapMethod = 0;
	RevX = false;
	RevY = false;
	RevZ = false;
	OldRevX = false;
	OldRevY = false;
	OldRevZ = false;
	PlanarFlat = false;
	OldPlanarFlat = false;
	PlanarRotate = false;
	OldPlanarRotate = false;
	for (int i = 0; i <= 2; i++)
	{
		MapIndex[i] = 0;
		OldMapIndex[i] = 0;
		OldMapUV[i] = Vector2::ZERO;
		MapUV[i] = Vector2::ZERO;
	}
	DefaultMapper = sbs->GetConfigInt("Skyscraper.SBS.TextureMapper", 0);
	texturecount = 0;
	materialcount = 0;
	TextureOverride = false;
	FlipTexture = false;
	mainnegflip = 0;
	mainposflip = 0;
	sidenegflip = 0;
	sideposflip = 0;
	topflip = 0;
	bottomflip = 0;
	widthscale.resize(6);
	heightscale.resize(6);

	//set default texture map values
	ResetTextureMapping(true);
}

TextureManager::~TextureManager()
{
	textureinfo.clear();

	if (textureboxes.empty() == false)
		FreeTextureBoxes();

	//remove manually-created textures
	for (size_t i = 0; i < manual_textures.size(); i++)
	{
		if (manual_textures[i])
		{
			Ogre::TextureManager::getSingleton().remove(manual_textures[i]->getHandle());
		}
	}
	manual_textures.clear();
}

void TextureManager::RegisterTextureInfo(const std::string &name, const std::string &material_name, const std::string &filename, Real widthmult, Real heightmult, bool enable_force, bool force_mode, size_t tex_size, size_t mat_size)
{
	//register texture for multipliers information
	//see TextureInfo structure for more information

	if (widthmult == 0.0)
		widthmult = 1.0;
	if (heightmult == 0.0)
		heightmult = 1.0;

	TextureInfo info;
	info.name = TrimStringCopy(name);
	info.material_name = TrimStringCopy(material_name);
	info.filename = TrimStringCopy(filename);
	info.widthmult = widthmult;
	info.heightmult = heightmult;
	info.enable_force = enable_force;
	info.force_mode = force_mode;
	info.dependencies = 0;
	info.tex_size = tex_size;
	info.mat_size = mat_size;

	textureinfo.push_back(info);
}

bool TextureManager::UnregisterTextureInfo(std::string name, std::string material_name)
{
	TrimString(name);
	TrimString(material_name);

	for (size_t i = 0; i < textureinfo.size(); i++)
	{
		if (textureinfo[i].name == name || (textureinfo[i].material_name == material_name && textureinfo[i].material_name != ""))
		{
			textureinfo.erase(textureinfo.begin() + i);
			return true;
		}
	}
	return false;
}

bool TextureManager::RotateTexture(const std::string &name, Real angle)
{
	//set a fixed rotation value for a texture

	if (sbs->Headless == true)
		return true;

	bool result;
	std::string texname = TrimStringCopy(name);
	std::string material = GetTextureMaterial(name, result, false);

	if (!result)
		return ReportError("RotateTexture: invalid texture " + texname + "\n");

	//get material name
	Ogre::MaterialPtr mMat = GetMaterialByName(material);

	if (!mMat)
		return false;

	//get texture unit state
	Ogre::TextureUnitState* state = GetTextureUnitState(mMat);

	//set rotation
	if (state)
		state->setTextureRotate(Degree(-angle));
	else
		return false;

	return true;
}

bool TextureManager::RotateAnimTexture(const std::string &name, Real speed)
{
	//set a rotation animation for a texture - speed is in revolutions per second

	if (sbs->Headless == true)
		return true;

	bool result;
	std::string texname = TrimStringCopy(name);
	std::string material = GetTextureMaterial(name, result, false);

	if (!result)
		return ReportError("RotateAnimTexture: invalid texture " + texname + "\n");

	//get material name
	Ogre::MaterialPtr mMat = GetMaterialByName(material);

	if (!mMat)
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

bool TextureManager::ScrollTexture(const std::string &name, Real x_offset, Real y_offset)
{
	//set a fixed scroll amount for a texture

	if (sbs->Headless == true)
		return true;

	bool result;
	std::string texname = TrimStringCopy(name);
	std::string material = GetTextureMaterial(name, result, false);

	if (!result)
		return ReportError("ScrollTexture: invalid texture " + texname + "\n");

	//get material name
	Ogre::MaterialPtr mMat = GetMaterialByName(material);

	if (!mMat)
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

bool TextureManager::ScrollAnimTexture(const std::string &name, Real x_speed, Real y_speed)
{
	//set a scroll animation for a texture - speed is in revolutions per second

	if (sbs->Headless == true)
		return true;

	bool result;
	std::string texname = TrimStringCopy(name);
	std::string material = GetTextureMaterial(name, result, false);

	if (!result)
		return ReportError("ScrollAnimTexture: invalid texture " + texname + "\n");

	//get material name
	Ogre::MaterialPtr mMat = GetMaterialByName(material);

	if (!mMat)
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

bool TextureManager::ScaleTexture(const std::string &name, Real x_scale, Real y_scale)
{
	//set a fixed scale amount for a texture

	if (sbs->Headless == true)
		return true;

	bool result;
	std::string texname = TrimStringCopy(name);
	std::string material = GetTextureMaterial(name, result, false);

	if (!result)
		return ReportError("ScaleTexture: invalid texture " + texname + "\n");

	//get material name
	Ogre::MaterialPtr mMat = GetMaterialByName(material);

	if (!mMat)
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

bool TextureManager::TransformTexture(const std::string &name, const std::string &type, const std::string &wave_type, Real base, Real frequency, Real phase, Real amplitude)
{
	//set a transformation type for a texture

	if (sbs->Headless == true)
		return true;

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

	if (!mMat)
		return false;

	//get texture unit state
	Ogre::TextureUnitState* state = GetTextureUnitState(mMat);

	//set transform
	if (state)
	{
		Ogre::TextureUnitState::TextureTransformType type;
		if (xform_type == "scroll_x")
			type = Ogre::TextureUnitState::TT_TRANSLATE_U;
		else if (xform_type == "scroll_y")
			type = Ogre::TextureUnitState::TT_TRANSLATE_V;
		else if (xform_type == "rotate")
			type = Ogre::TextureUnitState::TT_ROTATE;
		else if (xform_type == "scale_x")
			type = Ogre::TextureUnitState::TT_SCALE_U;
		else if (xform_type == "scale_y")
			type = Ogre::TextureUnitState::TT_SCALE_V;
		else
			return false;

		Ogre::WaveformType wavetype;
		if (wave == "sine")
			wavetype = Ogre::WFT_SINE;
		else if (wave == "triangle")
			wavetype = Ogre::WFT_TRIANGLE;
		else if (wave == "square")
			wavetype = Ogre::WFT_SQUARE;
		else if (wave == "sawtooth")
			wavetype = Ogre::WFT_SAWTOOTH;
		else if (wave == "inverse_sawtooth")
			wavetype = Ogre::WFT_INVERSE_SAWTOOTH;
		else if (wave == "pwm")
			wavetype = Ogre::WFT_PWM;
		else
			return false;

		state->setTransformAnimation(type, wavetype, base, frequency, phase, amplitude);
	}
	else
		return false;

	return true;
}

std::string TextureManager::GetTextureMaterial(const std::string &name, bool &result, bool report, const std::string &polygon_name)
{
	//perform a lookup on a texture, and return material name if it exists, or "Default" if not
	//returns false in &result if texture load failed, and if default material was used instead

	//use material_name value instead of name, if loaded as a material script instead of a direct texture
	//if report is true and texture is not found, issue warning

	if (sbs->Headless == true)
		return "";

	std::string matname = name;

	if (matname == prev_material)
	{
		result = true;
		return matname;
	}

	for (size_t i = 0; i < textureinfo.size(); i++)
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

void TextureManager::ProcessTextureFlip(Real tw, Real th)
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

bool TextureManager::GetTextureTiling(const std::string &texture, Real &tw, Real &th)
{
	//get per-texture tiling values from the textureinfo array
	for (size_t i = 0; i < textureinfo.size(); i++)
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

bool TextureManager::GetTextureForce(const std::string &texture, bool &enable_force, bool &force_mode)
{
	//get per-texture tiling values from the textureinfo array
	for (size_t i = 0; i < textureinfo.size(); i++)
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

int TextureManager::GetTextureCount()
{
	//return total number of textures
	return texturecount;
}

int TextureManager::GetMaterialCount()
{
	//return total number of materials
	return materialcount;
}

void TextureManager::FreeTextureImages()
{
	//unload images in all texture wrappers

	//for (int i = 0; i < engine->GetTextureList()->GetCount(); i++)
		//engine->GetTextureList()->Get(i)->SetImageFile(0);
}

Real TextureManager::AutoSize(Real n1, Real n2, bool iswidth, Real offset, bool enable_force, bool force_mode)
{
	//Texture autosizing formulas

	if (offset == 0)
		offset = 1;

	if (iswidth == true)
	{
		if ((AutoX == true && enable_force == false) || (enable_force == true && force_mode == true))
			return std::abs(n1 - n2) * offset;
		else
			return offset;
	}
	else
	{
		if ((AutoY == true && enable_force == false) || (enable_force == true && force_mode == true))
			return std::abs(n1 - n2) * offset;
		else
			return offset;
	}
}

void TextureManager::SetAutoSize(bool x, bool y)
{
	//enable or disable texture autosizing
	AutoX = x;
	AutoY = y;
}

void TextureManager::GetAutoSize(bool &x, bool &y)
{
	//return texture autosizing values
	x = AutoX;
	y = AutoY;
}

Vector2 TextureManager::CalculateSizing(const std::string &texture, const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, int direction, Real tw, Real th)
{
	//calculate texture autosizing based on polygon extents

	//Call texture autosizing formulas
	Real tw2 = tw, th2 = th;

	bool force_enable = false, force_mode = false;
	GetTextureForce(texture, force_enable, force_mode);

	Real width = v1.distance(v2);
	Real height = v2.distance(v3);

	tw2 = AutoSize(0, width, true, tw, force_enable, force_mode);
	th2 = AutoSize(0, height, false, th, force_enable, force_mode);

	//return results
	return Vector2(tw2, th2);
}

bool TextureManager::GetTextureMapping(PolyArray &vertices, Vector3 &v1, Vector3 &v2, Vector3 &v3, int &direction)
{
	//returns texture mapping coordinates for the specified vertices, in the v1, v2, and v3 vectors
	//this performs one of 3 methods - planar mapping, index mapping and manual vertex mapping

	if (MapMethod == 0)
	{
		//planar method

		Vector2 x, y, z;
		PolyArray varray;
		bool rev_x = false, rev_y = false, rev_z = false;

		//determine the largest projection dimension (the dimension that the polygon is generally on;
		//with a floor Y would be biggest)
		Plane plane = sbs->GetUtility()->ComputePlane(vertices);
		Vector3 normal = plane.normal;

		direction = 0; //x; faces left/right

		if (std::abs (normal.y) > std::abs (normal.x) && std::abs (normal.y) > std::abs (normal.z))
			direction = 1; //y biggest; faces up/down
		else if (std::abs (normal.z) > std::abs (normal.x))
			direction = 2; //z biggest; faces front/back
		else if (normal.x == 0)
			return false; //fail if normal vector is 0

		size_t selX = (1 << direction) & 0x3;
		size_t selY = (1 << selX) & 0x3;

		varray.reserve(vertices.size());
		for (size_t i = 0; i < vertices.size(); i++)
		{
			varray.push_back(Vector3(vertices[i][selX], vertices[i][selY], 0));
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
		Vector2 a, b;
		a = sbs->GetUtility()->GetExtents(varray, 1);
		b = sbs->GetUtility()->GetExtents(varray, 2);

		//set the result 2D coordinates
		if (direction == 0)
		{
			Vector2 pos, pos2;

			if (rev_z == false)
				pos = Vector2(b.y, b.x);
			else
				pos = Vector2(b.x, b.y);

			if (rev_y == false)
				pos2 = Vector2(a.y, a.x);
			else
				pos2 = Vector2(a.x, a.y);

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
			Vector2 pos, pos2;

			if (rev_x == false)
				pos = Vector2(b.x, b.y);
			else
				pos = Vector2(b.y, b.x);

			if (rev_z == false)
				pos2 = Vector2(a.x, a.y);
			else
				pos2 = Vector2(a.y, a.x);

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
			Vector2 pos, pos2;

			if (rev_x == false)
				pos = Vector2(a.y, a.x);
			else
				pos = Vector2(a.x, a.y);

			if (rev_y == false)
				pos2 = Vector2(b.y, b.x);
			else
				pos2 = Vector2(b.x, b.y);

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
		Plane plane = Plane(v1, v2, v3);
		Vector3 normal = plane.normal;

		direction = 0; //x; faces left/right

		if (std::abs (normal.y) > std::abs (normal.x) && std::abs (normal.y) > std::abs (normal.z))
			direction = 1; //y biggest; faces up/down
		else if (std::abs (normal.z) > std::abs (normal.x))
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
				size_t location = string.find("x");
				if (location != std::string::npos)
				{
					std::string number = string.substr(location + 1);
					if (ToInt(number) < (int)vertices.size())
						ReplaceAll(string, "x" + number, ToString(vertices[ToInt(number)].x));
					else
						ReplaceAll(string, "x" + number, "0"); //number value out of bounds
				}

				//find Y component
				location = string.find("y");
				if (location != std::string::npos)
				{
					std::string number = string.substr(location + 1);
					if (ToInt(number) < (int)vertices.size())
						ReplaceAll(string, "y" + number, ToString(vertices[ToInt(number)].y));
					else
						ReplaceAll(string, "y" + number, "0"); //number value out of bounds
				}

				//find Z component
				location = string.find("z");
				if (location != std::string::npos)
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
		Plane plane = Plane(v1, v2, v3);
		Vector3 normal = plane.normal;

		direction = 0; //x; faces left/right

		if (std::abs (normal.y) > std::abs (normal.x) && std::abs (normal.y) > std::abs (normal.z))
			direction = 1; //y biggest; faces up/down
		else if (std::abs (normal.z) > std::abs (normal.x))
			direction = 2; //z biggest; faces front/back
		else if (normal.x == 0)
			return false; //fail if normal vector is 0
	}

	return true;
}

void TextureManager::ResetTextureMapping(bool todefaults)
{
	//Resets UV texture mapping to defaults or previous values
	if (todefaults == true)
	{
		if (DefaultMapper == 0)
			SetPlanarMapping(false, false, false, false, false);
		if (DefaultMapper == 1)
			SetTextureMapping(0, Vector2(0, 0), 1, Vector2(1, 0), 2, Vector2(1, 1));
		if (DefaultMapper == 2)
			SetTextureMapping2("x0", "y0", "z0", Vector2(0, 0), "x1", "y1", "z1", Vector2(1, 0), "x2", "y2", "z2", Vector2(1, 1));
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

void TextureManager::SetPlanarMapping(bool flat, bool FlipX, bool FlipY, bool FlipZ, bool rotate)
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
	MapUV[0] = Vector2(0, 0);
	MapUV[1] = Vector2(1, 0);
	MapUV[2] = Vector2(1, 1);
	PlanarFlat = flat;
	MapMethod = 0;
	PlanarRotate = rotate;
}

void TextureManager::GetPlanarMapping(bool &flat, bool &FlipX, bool &FlipY, bool &FlipZ, bool &rotate)
{
	//gets planar texture mapping parameters
	//X, Y and Z reverse planar texture mapping per axis
	//Flat determines if depth should be ignored when mapping

	FlipX = RevX;
	FlipY = RevY;
	FlipZ = RevZ;
	flat = PlanarFlat;
	rotate = PlanarRotate;
}

void TextureManager::SetTextureMapping(int vertindex1, Vector2 uv1, int vertindex2, Vector2 uv2, int vertindex3, Vector2 uv3)
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

void TextureManager::SetTextureMapping2(const std::string & x1, const std::string & y1, const std::string & z1, Vector2 uv1, const std::string & x2, const std::string & y2, const std::string & z2, Vector2 uv2, const std::string & x3, const std::string & y3, const std::string & z3, Vector2 uv3)
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

void TextureManager::BackupMapping()
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

void TextureManager::SetTextureFlip(int mainneg, int mainpos, int sideneg, int sidepos, int top, int bottom)
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

void TextureManager::SetTextureOverride(const std::string &mainneg, const std::string &mainpos, const std::string &sideneg, const std::string &sidepos, const std::string &top, const std::string &bottom)
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

std::string TextureManager::ListTextures(bool show_filename)
{
	//list loaded textures

	std::string list;
	if (show_filename == true)
		list.append("Name --- Filename\n\n");

	for (size_t i = 0; i < textureinfo.size(); i++)
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

int TextureManager::GetTextureInfoCount()
{
	return (int)textureinfo.size();
}

bool TextureManager::GetTextureInfo(int index, TextureManager::TextureInfo &info)
{
	if (index >= 0 && index < textureinfo.size())
	{
		info = textureinfo[index];
		return true;
	}

	return false;
}

bool TextureManager::SetTextureInfo(int index, TextureManager::TextureInfo &info)
{
	if (index >= 0 && index < textureinfo.size())
	{
		textureinfo[index] = info;
		return true;
	}

	return false;
}

void TextureManager::IncrementTextureUsage(const std::string &name)
{
	for (size_t i = 0; i < textureinfo.size(); i++)
	{
		if (textureinfo[i].name == name)
		{
			textureinfo[i].dependencies++;
			return;
		}
	}
}

void TextureManager::DecrementTextureUsage(const std::string &name)
{
	for (size_t i = 0; i < textureinfo.size(); i++)
	{
		if (textureinfo[i].name == name)
		{
			textureinfo[i].dependencies--;
			return;
		}
	}
}

void TextureManager::SaveTexture(Ogre::TexturePtr texture, const std::string & filename)
{
	//save a raw texture to a file

	Ogre::Image image;
	texture->convertToImage(image, true);
	image.save(filename);
}

void TextureManager::IncrementTextureCount()
{
	texturecount++;
}

void TextureManager::DecrementTextureCount()
{
	texturecount--;
}

void TextureManager::IncrementMaterialCount()
{
	materialcount++;
}

void TextureManager::DecrementMaterialCount()
{
	materialcount--;
}

Ogre::MaterialPtr TextureManager::CreateMaterial(const std::string &name, const std::string &path)
{
	//unload material if already loaded -- FIXME
	//if (UnloadMaterial(name, path) == true)
		//UnregisterTextureInfo(name);

	//create new material
	Ogre::MaterialPtr mMat;
	try
	{
		mMat = Ogre::MaterialManager::getSingleton().create(ToString(sbs->InstanceNumber) + ":" + name, path);
	}
	catch (Ogre::Exception& e)
	{
		ReportError("Error creating material for texture " + name + "\n" + e.getDescription());
		return 0;
	}

	IncrementMaterialCount();

	EnableLighting(name, true);
	EnableShadows(name, true);

	//show only clockwise side of material
	mMat->setCullingMode(Ogre::CULL_ANTICLOCKWISE);

	return mMat;
}

void TextureManager::EnableLighting(const std::string &material_name, bool value)
{
	//enable or disable lighting on a material

	Ogre::MaterialPtr mat = GetMaterialByName(material_name);

	if (mat)
	{
		mat->setLightingEnabled(false);
		if (sbs->GetConfigBool("Skyscraper.SBS.Lighting", false) == true)
		{
			mat->setLightingEnabled(value);
			mat->setAmbient(sbs->AmbientR, sbs->AmbientG, sbs->AmbientB);
		}
	}
}

Ogre::MaterialPtr TextureManager::GetMaterialByName(const std::string &name, const std::string &group)
{
	Ogre::MaterialPtr ptr = 0;

	if (Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(group) == false)
		return ptr;

	ptr = Ogre::MaterialManager::getSingleton().getByName(ToString(sbs->InstanceNumber) + ":" + name, group);
	return ptr;
}

Ogre::TextureUnitState* TextureManager::BindTextureToMaterial(Ogre::MaterialPtr mMat, std::string texture_name, bool has_alpha)
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

Ogre::TextureUnitState* TextureManager::GetTextureUnitState(Ogre::MaterialPtr mMat)
{
	//get first texture unit state
	return mMat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
}

std::string TextureManager::GetTextureName(Ogre::MaterialPtr mMat)
{
	std::string texname = "";

	Ogre::TextureUnitState *state = GetTextureUnitState(mMat);
	if (state)
		texname = state->getTextureName();

	return texname;
}

Ogre::TexturePtr TextureManager::GetTextureByName(const std::string &name, const std::string &group)
{
	Ogre::TexturePtr ptr = 0;

	if (Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(group) == false)
		return ptr;

	ptr = Ogre::TextureManager::getSingleton().getByName(name, group);
	return ptr;
}

void TextureManager::FreeTextureBoxes()
{
	for (size_t i = 0; i < textureboxes.size(); i++)
	{
		free(textureboxes[i].buffer);

		if (textureboxes[i].box)
			delete textureboxes[i].box;
	}
	textureboxes.clear();
}

void TextureManager::SetPlanarRotate(bool value)
{
	PlanarRotate = value;
}

bool TextureManager::GetPlanarRotate()
{
	return PlanarRotate;
}

void TextureManager::EnableShadows(const std::string &material_name, bool value)
{
	//enable shadows

	Ogre::MaterialPtr mMat = GetMaterialByName(material_name);
	if (mMat)
		mMat->setReceiveShadows(value);
}

void TextureManager::SetCulling(const std::string &material_name, int mode)
{
	//sets culling mode on a material
	//if mode is 0, do not cull (both sides visible)
	//if mode is 1, cull anticlockwise (SBS default)
	//if mode is 2, cull clockwise (Ogre default)

	//get original material
	Ogre::MaterialPtr mat = GetMaterialByName(material_name);

	if (mat)
	{
		if (mode == 0)
			mat->setCullingMode(Ogre::CullingMode::CULL_NONE);
		if (mode == 1)
			mat->setCullingMode(Ogre::CullingMode::CULL_ANTICLOCKWISE);
		if (mode == 2)
			mat->setCullingMode(Ogre::CullingMode::CULL_CLOCKWISE);
	}
	else
		ReportError("SetCulling: Material " + material_name + " not found");
}

size_t TextureManager::GetMemoryUsage()
{
	size_t result = 0;

	for (size_t i = 0; i < textureinfo.size(); i++)
	{
		result += textureinfo[i].tex_size + textureinfo[i].mat_size;
	}

	return result;
}

}
