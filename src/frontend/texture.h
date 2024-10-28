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

#ifndef TEXTURE_H_EC
#define TEXTURE_H_EC

#include <OgreOverlayPrerequisites.h>
#include <OgreColourValue.h>
#include <OgreFont.h>

namespace Skyscraper {

class EngineContext;

class TextureManager
{
public:

	TextureManager(EngineContext *parent);
	~TextureManager();
	bool LoadTexture(const std::string &filename, const std::string &name, Real widthmult, Real heightmult, bool enable_force = false, bool force_mode = false, int mipmaps = -1, bool use_alpha_color = false, Ogre::ColourValue alpha_color = Ogre::ColourValue::Black);
	bool LoadAnimatedTexture(std::vector<std::string> filenames, const std::string &name, Real duration, Real widthmult, Real heightmult, bool enable_force = false, bool force_mode = false, int mipmaps = -1, bool use_alpha_color = false, Ogre::ColourValue alpha_color = Ogre::ColourValue::Black);
	bool LoadAlphaBlendTexture(const std::string &filename, const std::string &specular_filename, const std::string &blend_filename, const std::string &name, bool spherical, Real widthmult, Real heightmult, bool enable_force = false, bool force_mode = false, int mipmaps = -1, bool use_alpha_color = false, Ogre::ColourValue alpha_color = Ogre::ColourValue::Black);
	bool LoadMaterial(const std::string &filename, const std::string &name, Real widthmult, Real heightmult, bool enable_force = false, bool force_mode = false);
	bool UnloadTexture(const std::string &name, const std::string &group);
	bool UnloadMaterial(const std::string &name, const std::string &group);
	bool LoadTextureCropped(const std::string &filename, const std::string &name, int x, int y, int width, int height, Real widthmult, Real heightmult, bool enable_force = false, bool force_mode = false);
	bool RotateTexture(const std::string &name, Real angle);
	bool RotateAnimTexture(const std::string &name, Real speed);
	bool ScrollTexture(const std::string &name, Real x_offset, Real y_offset);
	bool ScrollAnimTexture(const std::string &name, Real x_speed, Real y_speed);
	bool ScaleTexture(const std::string &name, Real x_scale, Real y_scale);
	bool TransformTexture(const std::string &name, const std::string &type, const std::string &wave_type, Real base, Real frequency, Real phase, Real amplitude);
	bool AddTextToTexture(const std::string &origname, const std::string &name, const std::string &font_filename, Real font_size, const std::string &text, int x1, int y1, int x2, int y2, const std::string &h_align, const std::string &v_align, int ColorR, int ColorG, int ColorB, bool enable_force = false, bool force_mode = false);
	bool AddTextureOverlay(const std::string &orig_texture, const std::string &overlay_texture, const std::string &name, int x, int y, int width, int height, Real widthmult, Real heightmult, bool enable_force = false, bool force_mode = false);
	void FreeTextureImages();
	Ogre::TexturePtr loadChromaKeyedTexture(const std::string& filename, const std::string& resGroup, const std::string& name, const Ogre::ColourValue& keyCol = Ogre::ColourValue::Black, int numMipmaps = -1, Real threshold = 0.003);
	void SaveTexture(Ogre::TexturePtr texture, const std::string &filename);
	Ogre::MaterialPtr CreateMaterial(const std::string &name, const std::string &path);
	Ogre::MaterialPtr GetMaterialByName(const std::string &name, const std::string &group = "General");
	Ogre::TextureUnitState* BindTextureToMaterial(Ogre::MaterialPtr mMat, std::string texture_name, bool has_alpha);
	Ogre::TextureUnitState* GetTextureUnitState(Ogre::MaterialPtr mMat);
	std::string GetTextureName(Ogre::MaterialPtr mMat);
	Ogre::TexturePtr GetTextureByName(const std::string &name, const std::string &group = "General");
	std::string GetTextureMaterial(const std::string &name, bool &result, bool report = true, const std::string &polygon_name = "");
	void CopyTexture(Ogre::TexturePtr source, Ogre::TexturePtr destination);
	void CopyTexture(Ogre::TexturePtr source, Ogre::TexturePtr destination, const Ogre::Box &srcBox, const Ogre::Box &dstBox);
	void FreeTextureBoxes();
	void EnableLighting(const std::string &material_name, bool value);
	void EnableShadows(const std::string &material_name, bool value);
	void SetCulling(const std::string &material_name, int mode = 1);
	Ogre::MaterialPtr SetCulling(const std::string &material_name, const std::string &name, int mode);
	size_t GetMemoryUsage();

private:

	bool WriteToTexture(const std::string &str, Ogre::TexturePtr destTexture, int destLeft, int destTop, int destRight, int destBottom, Ogre::FontPtr font, const Ogre::ColourValue &color, char justify = 'l', char vert_justify = 't', bool wordwrap = true);
	Ogre::TexturePtr LoadTexture(const std::string &filename, int mipmaps, bool &has_alpha, bool use_alpha_color = false, Ogre::ColourValue alpha_color = Ogre::ColourValue::Black);
	void UnloadMaterials();
	void Report(const std::string &message);
	bool ReportError(const std::string &message);

	std::vector<Ogre::TexturePtr> manual_textures;

	struct TexturePixelBox
	{
		Ogre::FontPtr font;
		Ogre::PixelBox *box;
		unsigned char *buffer;
	};
	std::vector<TexturePixelBox> textureboxes;

	//function caching
	std::string prev_material;

	//SBS singleton
	::SBS::SBS* sbs;
	EngineContext* parent;
};

}

#endif
