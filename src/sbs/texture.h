/* $Id$ */

/*
        Scalable Building Simulator - Texture Manager
        The Skyscraper Project - Version 1.11 Alpha
        Copyright (C)2004-2017 Ryan Thoryk
        http://www.skyscrapersim.com
        http://sourceforge.net/projects/skyscraper
        Contact - ryan@skyscrapersim.com

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

#ifndef _SBS_TEXTURE_H
#define _SBS_TEXTURE_H

#include <OgreOverlayPrerequisites.h>
#include <OgreColourValue.h>

namespace SBS {

class SBSIMPEXP TextureManager : public ObjectBase
{
public:

	bool TextureOverride; //if enabled, overrides textures with ones set with SetTextureOverride()
	bool FlipTexture; //if enabled, flips textures according to parameters set in SetTextureFlip()

	TextureManager(Object *parent);
	~TextureManager();
	bool LoadTexture(const std::string &filename, const std::string &name, float widthmult, float heightmult, bool enable_force = false, bool force_mode = false, int mipmaps = -1, bool use_alpha_color = false, Ogre::ColourValue alpha_color = Ogre::ColourValue::Black);
	bool LoadAnimatedTexture(std::vector<std::string> filenames, const std::string &name, float duration, float widthmult, float heightmult, bool enable_force = false, bool force_mode = false, int mipmaps = -1, bool use_alpha_color = false, Ogre::ColourValue alpha_color = Ogre::ColourValue::Black);
	bool LoadAlphaBlendTexture(const std::string &filename, const std::string &specular_filename, const std::string &blend_filename, const std::string &name, bool spherical, float widthmult, float heightmult, bool enable_force = false, bool force_mode = false, int mipmaps = -1, bool use_alpha_color = false, Ogre::ColourValue alpha_color = Ogre::ColourValue::Black);
	bool LoadMaterial(const std::string &filename, const std::string &name, float widthmult, float heightmult, bool enable_force = false, bool force_mode = false);
	bool UnloadTexture(const std::string &name, const std::string &group);
	bool UnloadMaterial(const std::string &name, const std::string &group);
	bool LoadTextureCropped(const std::string &filename, const std::string &name, int x, int y, int width, int height, float widthmult, float heightmult, bool enable_force = false, bool force_mode = false);
	bool RotateTexture(const std::string &name, float angle);
	bool RotateAnimTexture(const std::string &name, float speed);
	bool ScrollTexture(const std::string &name, float x_offset, float y_offset);
	bool ScrollAnimTexture(const std::string &name, float x_speed, float y_speed);
	bool ScaleTexture(const std::string &name, float x_scale, float y_scale);
	bool TransformTexture(const std::string &name, const std::string &type, const std::string &wave_type, float base, float frequency, float phase, float amplitude);
	float AutoSize(float n1, float n2, bool iswidth, float offset, bool enable_force, bool force_mode);
	void SetTextureMapping(int vertindex1, Ogre::Vector2 uv1, int vertindex2, Ogre::Vector2 uv2, int vertindex3, Ogre::Vector2 uv3);
	void SetTextureMapping2(const std::string &x1, const std::string &y1, const std::string &z1, Ogre::Vector2 uv1, const std::string &x2, const std::string &y2, const std::string &z2, Ogre::Vector2 uv2, const std::string &x3, const std::string &y3, const std::string &z3, Ogre::Vector2 uv3);
	void ResetTextureMapping(bool todefaults = false);
	void SetAutoSize(bool x, bool y);
	void GetAutoSize(bool &x, bool &y);
	void SetTextureOverride(const std::string &mainneg, const std::string &mainpos, const std::string &sideneg, const std::string &sidepos, const std::string &top, const std::string &bottom);
	void SetTextureFlip(int mainneg, int mainpos, int sideneg, int sidepos, int top, int bottom);
	void ProcessTextureFlip(float tw, float th);
	bool GetTextureTiling(const std::string &texture, float &tw, float &th);
	bool GetTextureForce(const std::string &texture, bool &enable_force, bool &force_mode);
	bool AddTextToTexture(const std::string &origname, const std::string &name, const std::string &font_filename, float font_size, const std::string &text, int x1, int y1, int x2, int y2, const std::string &h_align, const std::string &v_align, int ColorR, int ColorG, int ColorB, bool enable_force = false, bool force_mode = false);
	bool AddTextureOverlay(const std::string &orig_texture, const std::string &overlay_texture, const std::string &name, int x, int y, int width, int height, float widthmult, float heightmult, bool enable_force = false, bool force_mode = false);
	void FreeTextureImages();
	int GetTextureCount();
	int GetMaterialCount();
	bool GetTextureMapping(std::vector<Ogre::Vector3> &vertices, Ogre::Vector3 &v1, Ogre::Vector3 &v2, Ogre::Vector3 &v3, int &direction);
	void SetPlanarMapping(bool flat, bool FlipX, bool FlipY, bool FlipZ, bool rotate);
	void GetPlanarMapping(bool &flat, bool &FlipX, bool &FlipY, bool &FlipZ, bool &rotate);
	Ogre::Vector2 CalculateSizing(const std::string &texture, const Ogre::Vector3 &v1, const Ogre::Vector3 &v2, const Ogre::Vector3 &v3, int direction, float tw, float th);
	Ogre::TexturePtr loadChromaKeyedTexture(const std::string& filename, const std::string& resGroup, const std::string& name, const Ogre::ColourValue& keyCol = Ogre::ColourValue::Black, int numMipmaps = -1, float threshold = 0.003f);
	void SaveTexture(Ogre::TexturePtr texture, const std::string &filename);
	std::string ListTextures(bool show_filename = false);
	void IncrementTextureCount();
	void DecrementTextureCount();
	void IncrementMaterialCount();
	void DecrementMaterialCount();
	void RegisterTextureInfo(const std::string &name, const std::string &material_name, const std::string &filename, float widthmult, float heightmult, bool enable_force, bool force_mode);
	bool UnregisterTextureInfo(std::string name, std::string material_name = "");
	Ogre::MaterialPtr CreateMaterial(const std::string &name, const std::string &path);
	Ogre::MaterialPtr GetMaterialByName(const std::string &name, const std::string &group = "Autodetect");
	Ogre::TextureUnitState* BindTextureToMaterial(Ogre::MaterialPtr mMat, std::string texture_name, bool has_alpha);
	Ogre::TextureUnitState* GetTextureUnitState(Ogre::MaterialPtr mMat);
	std::string GetTextureName(Ogre::MaterialPtr mMat);
	Ogre::TexturePtr GetTextureByName(const std::string &name, const std::string &group = "Autodetect");
	std::string GetTextureMaterial(const std::string &name, bool &result, bool report = true, const std::string &polygon_name = "");
	void CopyTexture(Ogre::TexturePtr source, Ogre::TexturePtr destination);
	void CopyTexture(Ogre::TexturePtr source, Ogre::TexturePtr destination, const Ogre::Box &srcBox, const Ogre::Box &dstBox);
	void FreeTextureBoxes();
	void SetPlanarRotate(bool value);
	bool GetPlanarRotate();
	bool ComputeTextureMap(Ogre::Matrix3 &t_matrix, Ogre::Vector3 &t_vector, std::vector<Ogre::Vector3> &vertices, const Ogre::Vector3 &p1, const Ogre::Vector3 &p2, const Ogre::Vector3 &p3, float tw, float th);

	//override textures
	std::string mainnegtex, mainpostex, sidenegtex, sidepostex, toptex, bottomtex;

	//texture flipping
	int mainnegflip, mainposflip, sidenegflip, sideposflip, topflip, bottomflip;
	std::vector<float> widthscale;
	std::vector<float> heightscale;

private:

	//texture mapping
	std::vector<int> MapIndex;
	std::vector<int> OldMapIndex;
	std::vector<Ogre::Vector2> MapUV;
	std::vector<Ogre::Vector2> OldMapUV;
	std::vector<std::string> MapVerts1;
	std::vector<std::string> MapVerts2;
	std::vector<std::string> MapVerts3;
	std::vector<std::string> OldMapVerts1;
	std::vector<std::string> OldMapVerts2;
	std::vector<std::string> OldMapVerts3;
	bool AutoX, AutoY; //autosizing
	int MapMethod; //texture mapping method - 0=planar, 1=index, 2=verts
	int OldMapMethod;
	bool RevX, RevY, RevZ; //extent reversals (planar texture mapping)
	bool OldRevX, OldRevY, OldRevZ;
	bool PlanarFlat, OldPlanarFlat;
	bool PlanarRotate, OldPlanarRotate;
	int DefaultMapper; //default texture mapper

	void BackupMapping();
	bool WriteToTexture(const std::string &str, Ogre::TexturePtr destTexture, int destLeft, int destTop, int destRight, int destBottom, Ogre::FontPtr font, const Ogre::ColourValue &color, char justify = 'l', char vert_justify = 't', bool wordwrap = true);
	Ogre::TexturePtr LoadTexture(const std::string &filename, int mipmaps, bool &has_alpha, bool use_alpha_color = false, Ogre::ColourValue alpha_color = Ogre::ColourValue::Black);
	void UnloadMaterials();
	bool ComputeTextureSpace(Ogre::Matrix3 &m, Ogre::Vector3 &v, const Ogre::Vector3 &v_orig, const Ogre::Vector3 &v1, float len1, const Ogre::Vector3 &v2, float len2);

	//texture information structure
	struct TextureInfo
	{
		std::string name;
		std::string material_name; //used if material is loaded instead of texture, as an alias
		std::string filename;
		float widthmult;
		float heightmult;
		bool enable_force; //enable forcing of tile or stretch mode?
		bool force_mode; //false to disable autosizing, true to enable autosizing
	};

	std::vector<TextureInfo> textureinfo;
	std::vector<Ogre::TexturePtr> manual_textures;

	//textures/materials count
	int texturecount;
	int materialcount;

	struct TexturePixelBox
	{
		Ogre::FontPtr font;
		Ogre::PixelBox *box;
		unsigned char *buffer;
	};
	std::vector<TexturePixelBox> textureboxes;

	//function caching
	std::string prev_material;
};

}

#endif
