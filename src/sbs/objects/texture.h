/*
	Scalable Building Simulator - Texture Object
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

#ifndef _SBS_TEXTURE_H
#define _SBS_TEXTURE_H

namespace SBS {

class SBSIMPEXP Texture : public Object
{
public:
	std::string material_name; //used if material is loaded instead of texture, as an alias
	std::string filename;
	Real widthmult;
	Real heightmult;
	bool enable_force; //enable forcing of tile or stretch mode?
	bool force_mode; //false to disable autosizing, true to enable autosizing
	int dependencies; //number of submeshes depending on this texture
	size_t tex_size; //size of texture resource in bytes
	size_t mat_size; //size of material resource in bytes

    Texture(TextureManager *manager, const std::string &name, const std::string &material_name, const std::string &filename, Real widthmult, Real heightmult, bool enable_force, bool force_mode, size_t tex_size, size_t mat_size);
    ~Texture();
	size_t GetMemoryUsage();

private:

    TextureManager* manager;
};

}

#endif
