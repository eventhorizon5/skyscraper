/*
	Scalable Building Simulator - Texture Manager
	The Skyscraper Project - Version 2.1
	Copyright (C)2004-2025 Ryan Thoryk
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

#ifndef _SBS_TEXMAN_H
#define _SBS_TEXMAN_H

#include <OgreOverlayPrerequisites.h>
#include <OgreColourValue.h>
#include <OgreFont.h>

namespace SBS {

class SBSIMPEXP TextureManager : public Object
{
public:

	bool TextureOverride; //if enabled, overrides textures with ones set with SetTextureOverride()
	bool FlipTexture; //if enabled, flips textures according to parameters set in SetTextureFlip()

	explicit TextureManager(Object *parent);
	~TextureManager();
	bool LoadTexture(const std::string &filename, const std::string &name, Real widthmult, Real heightmult, bool enable_force = false, bool force_mode = false, int mipmaps = -1, bool use_alpha_color = false, Ogre::ColourValue alpha_color = Ogre::ColourValue::Black);
	bool LoadAnimatedTexture(std::vector<std::string> filenames, const std::string &name, Real duration, Real widthmult, Real heightmult, bool enable_force = false, bool force_mode = false, int mipmaps = -1, bool use_alpha_color = false, Ogre::ColourValue alpha_color = Ogre::ColourValue::Black);
	bool CreateSlideshow(const std::string &name, bool start, std::vector<std::string> filenames, std::vector<Real> durations, Real widthmult, Real heightmult, bool enable_force = false, bool force_mode = false, int mipmaps = -1, bool use_alpha_color = false, Ogre::ColourValue alpha_color = Ogre::ColourValue::Black);
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
	Real AutoSize(Real n1, Real n2, bool iswidth, Real offset, bool enable_force, bool force_mode);
	void SetTextureMapping(int vertindex1, Vector2 uv1, int vertindex2, Vector2 uv2, int vertindex3, Vector2 uv3);
	void SetTextureMapping2(const std::string &x1, const std::string &y1, const std::string &z1, Vector2 uv1, const std::string &x2, const std::string &y2, const std::string &z2, Vector2 uv2, const std::string &x3, const std::string &y3, const std::string &z3, Vector2 uv3);
	void ResetTextureMapping(bool todefaults = false);
	void SetAutoSize(bool x, bool y);
	void GetAutoSize(bool &x, bool &y);
	void SetTextureOverride(const std::string &mainneg, const std::string &mainpos, const std::string &sideneg, const std::string &sidepos, const std::string &top, const std::string &bottom);
	void SetTextureFlip(int mainneg, int mainpos, int sideneg, int sidepos, int top, int bottom);
	void ProcessTextureFlip(Real tw, Real th);
	bool GetTextureTiling(const std::string &texture, Real &tw, Real &th);
	bool GetTextureForce(const std::string &texture, bool &enable_force, bool &force_mode);
	bool AddTextToTexture(const std::string &origname, const std::string &name, const std::string &font_filename, Real font_size, const std::string &text, int x1, int y1, int x2, int y2, const std::string &h_align, const std::string &v_align, int ColorR, int ColorG, int ColorB, bool enable_force = false, bool force_mode = false);
	bool AddTextureOverlay(const std::string &orig_texture, const std::string &overlay_texture, const std::string &name, int x, int y, int width, int height, Real widthmult, Real heightmult, bool enable_force = false, bool force_mode = false);
	void FreeTextureImages();
	int GetTextureCount();
	int GetMaterialCount();
	bool GetTextureMapping(PolyArray &vertices, Vector3 &v1, Vector3 &v2, Vector3 &v3, int &direction);
	void SetPlanarMapping(bool flat, bool FlipX, bool FlipY, bool FlipZ, bool rotate);
	void GetPlanarMapping(bool &flat, bool &FlipX, bool &FlipY, bool &FlipZ, bool &rotate);
	Vector2 CalculateSizing(const std::string &texture, const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, int direction, Real tw, Real th);
	Ogre::TexturePtr loadChromaKeyedTexture(const std::string& filename, const std::string& resGroup, const std::string& name, const Ogre::ColourValue& keyCol = Ogre::ColourValue::Black, int numMipmaps = -1, Real threshold = 0.003);
	void SaveTexture(Ogre::TexturePtr texture, const std::string &filename);
	std::string ListTextures(bool show_filename = false);
	void IncrementTextureCount();
	void DecrementTextureCount();
	void IncrementMaterialCount();
	void DecrementMaterialCount();
	void RegisterTexture(const std::string &name, const std::string &material_name, const std::string &filename, Real widthmult, Real heightmult, bool enable_force, bool force_mode, size_t tex_size, size_t mat_size);
	bool UnregisterTexture(std::string name, std::string material_name = "");
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
	void SetPlanarRotate(bool value);
	bool GetPlanarRotate();
	bool ComputeTextureMap(Matrix3 &t_matrix, Vector3 &t_vector, PolyArray &vertices, const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, Real tw, Real th);
	void EnableLighting(const std::string &material_name, bool value);
	void EnableShadows(const std::string &material_name, bool value);
	void IncrementTextureUsage(const std::string &name);
	void DecrementTextureUsage(const std::string &name);
	void SetCulling(const std::string &material_name, int mode = 1);
	Ogre::MaterialPtr SetCulling(const std::string &material_name, const std::string &name, int mode);
	size_t GetMemoryUsage();
	bool GetTextureImage(Ogre::TexturePtr texture);
	bool MaterialExists(const std::string &name);
	int GetTextureObjectCount();
	Texture* GetTextureObject(size_t index);
	bool SetTexture(const std::string &name, const std::string &texture);
	void StartSlideshow(const std::string &name);
	void StopSlideshow(const std::string &name);
	void StartAllSlideshows();
	void StopAllSlideshows();


	//override textures
	std::string mainnegtex, mainpostex, sidenegtex, sidepostex, toptex, bottomtex;

	//texture flipping
	int mainnegflip, mainposflip, sidenegflip, sideposflip, topflip, bottomflip;
	std::vector<Real> widthscale;
	std::vector<Real> heightscale;

private:

	//texture mapping
	std::vector<int> MapIndex;
	std::vector<int> OldMapIndex;
	std::vector<Vector2> MapUV;
	std::vector<Vector2> OldMapUV;
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
	bool ComputeTextureSpace(Matrix3 &m, Vector3 &v, const Vector3 &origin, const Vector3 &u_point, Real u_length, const Vector3 &v_point, Real v_length);
	void Report(const std::string &message);
	bool ReportError(const std::string &message);

	std::vector<Texture*> textures;
	std::vector<TextureImage*> texture_images;
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

	//slideshow system
	class Timer;
	struct Slideshow
	{
		std::string name;
		std::vector<std::string> filenames;
		std::vector<Real> durations;
		Timer* timer;
		int iterator;
		Ogre::MaterialPtr material;
		bool has_alpha;
	};
	std::vector<Slideshow*> slideshows;
};

}

#endif
