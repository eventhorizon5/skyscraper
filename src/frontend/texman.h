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
	Ogre::TexturePtr loadChromaKeyedTexture(const std::string& filename, const std::string& resGroup, const std::string& name, const Ogre::ColourValue& keyCol = Ogre::ColourValue::Black, int numMipmaps = -1, Real threshold = 0.003);
	void SaveTexture(Ogre::TexturePtr texture, const std::string &filename);
	void CopyTexture(Ogre::TexturePtr source, Ogre::TexturePtr destination);
	void CopyTexture(Ogre::TexturePtr source, Ogre::TexturePtr destination, const Ogre::Box &srcBox, const Ogre::Box &dstBox);

private:

	bool WriteToTexture(const std::string &str, Ogre::TexturePtr destTexture, int destLeft, int destTop, int destRight, int destBottom, Ogre::FontPtr font, const Ogre::ColourValue &color, char justify = 'l', char vert_justify = 't', bool wordwrap = true);
	Ogre::TexturePtr LoadTexture(const std::string &filename, int mipmaps, bool &has_alpha, bool use_alpha_color = false, Ogre::ColourValue alpha_color = Ogre::ColourValue::Black);
	void UnloadMaterials();
	void Report(const std::string &message);
	bool ReportError(const std::string &message);

	//SBS singleton
	::SBS::SBS* sbs;
	EngineContext* parent;
};

}

#endif
