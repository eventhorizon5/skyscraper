/*
	Skyscraper 2.1 - Startup Screen
	Copyright (C)2003-2025 Ryan Thoryk
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

namespace Skyscraper
{

class StartScreen
{
public:

	explicit StartScreen(Skyscraper* frontend);
	~StartScreen();
	bool DrawBackground();
	bool GetMenuInput();
	void DeleteButtons();

private:

	//button locations
	struct buttondata
	{
		Real x;
		Real y;
		Real size_x;
		Real size_y;
		std::string filename;
		std::string filename_selected;
		std::string filename_pressed;
		Real offset_x;
		Real offset_y;
		bool drawn_selected, drawn_pressed;
		int active_button;
		Ogre::SceneNode* node;
		Ogre::Rectangle2D* rect;
	};
	buttondata *buttons;
	int buttoncount;

	Ogre::Rectangle2D* background_rect;
	Ogre::SceneNode* background_node;
	std::string background_image;

	bool DrawImage(const std::string &filename, buttondata *button, Real x, Real y, bool center, const std::string &filename_selected = "", const std::string &filename_pressed = "");
	void Click(int index);
	void RightClick(int index);

	Skyscraper *frontend;
	VM *vm;

};

}
