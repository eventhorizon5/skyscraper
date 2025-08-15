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

#include "globals.h"
#include "sbs.h"
#include "utility.h"
#include "texman.h"
#include "texture.h"

namespace SBS {

Texture::Texture(TextureManager* manager, const std::string &name, const std::string &material_name, const std::string &filename, Real widthmult, Real heightmult, bool enable_force, bool force_mode, size_t tex_size, size_t mat_size) : Object(manager)
{
	//set up SBS object
	SetValues("Texture", name, false, false);

    this->manager = manager;

	if (widthmult == 0.0)
		widthmult = 1.0;
	if (heightmult == 0.0)
		heightmult = 1.0;

	this->material_name = material_name;
	this->filename = filename;
	this->widthmult = widthmult;
	this->heightmult = heightmult;
	this->enable_force = enable_force;
	this->force_mode = force_mode;
	this->dependencies = 0;
	this->tex_size = tex_size;
	this->mat_size = mat_size;
}

Texture::~Texture()
{

}

size_t Texture::GetMemoryUsage()
{
	//returns the memory usage of this texture

	return tex_size + mat_size;
}

}
