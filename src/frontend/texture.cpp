/*
	Skyscraper 2.1 - Client Texture Functions
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

#include "globals.h"
#include "skyscraper.h"
#include "vm.h"
#include "texture.h"

using namespace SBS;

namespace Skyscraper
{

void Skyscraper::TextureLoader()
{
	//load texture
	bool has_alpha = false;
	Ogre::TexturePtr mTex = LoadTexture(filename2, mipmaps, has_alpha, use_alpha_color, alpha_color);

	if (!mTex)
		return false;
	std::string texturename = mTex->getName();

	//create a new material
	std::string matname = TrimStringCopy(name);
	Ogre::MaterialPtr mMat = CreateMaterial(matname, "General");

	if (mMat == 0)
		return false;

	//bind texture to material
	BindTextureToMaterial(mMat, texturename, has_alpha);
}

}
