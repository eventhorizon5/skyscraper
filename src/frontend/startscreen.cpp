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

#ifdef USING_WX
#include "wx/wx.h"
#endif
#include "globals.h"
#include "skyscraper.h"
#include "vm.h"
#include "hal.h"
#include "mainscreen.h"
#include "startscreen.h"

using namespace SBS;

namespace Skyscraper
{

StartScreen::StartScreen(Skyscraper *frontend)
{
	this->frontend = frontend;
	this->vm = frontend->GetVM();
	buttons = 0;
	buttoncount = 0;
	background_rect = 0;
	background_node = 0;
}

StartScreen::~StartScreen()
{

}

bool StartScreen::DrawBackground()
{
	//draw menu background

	bool result = false;

	HAL *hal = vm->GetHAL();

	result = DrawImage("data/" + hal->GetConfigString(hal->configfile, "Skyscraper.Frontend.Menu.Image", "menu.png"), 0, -1, -1, false);

	if (result == false)
		return false;

	if (buttoncount == 0)
	{
		buttoncount = hal->GetConfigInt(hal->configfile, "Skyscraper.Frontend.Menu.Buttons", 5);
		buttons = new buttondata[buttoncount];

		for (int i = 0; i < buttoncount; i++)
		{
			buttons[i].node = 0;
			buttons[i].drawn_selected = false;
			buttons[i].drawn_pressed = false;
			buttons[i].active_button = 0;
			buttons[i].rect = 0;
		}
	}

	for (int i = 0; i < buttoncount; i++)
	{
		std::string b1, b2, b3;
		Real x = 0, y = 0;
		bool center = false;
		std::string number = ToString(i + 1);

		if (i == 0)
		{
			b1 = "data/" + hal->GetConfigString(hal->configfile, "Skyscraper.Frontend.Menu.Button1.Image", "button_triton.png");
			b2 = "data/" + hal->GetConfigString(hal->configfile, "Skyscraper.Frontend.Menu.Button1.Selected", "button_triton_selected.png");
			b3 = "data/" + hal->GetConfigString(hal->configfile, "Skyscraper.Frontend.Menu.Button1.Pressed", "button_triton_pressed.png");
			x = hal->GetConfigFloat(hal->configfile, "Skyscraper.Frontend.Menu.Button1.X", 0.0);
			y = hal->GetConfigFloat(hal->configfile, "Skyscraper.Frontend.Menu.Button1.Y", -0.08);
			center = hal->GetConfigBool(hal->configfile, "Skyscraper.Frontend.Menu.Button1.Center", true);
		}
		if (i == 1)
		{
			b1 = "data/" + hal->GetConfigString(hal->configfile, "Skyscraper.Frontend.Menu.Button2.Image", "button_glasstower.png");
			b2 = "data/" + hal->GetConfigString(hal->configfile, "Skyscraper.Frontend.Menu.Button2.Selected", "button_glasstower_selected.png");
			b3 = "data/" + hal->GetConfigString(hal->configfile, "Skyscraper.Frontend.Menu.Button2.Pressed", "button_glasstower_pressed.png");
			x = hal->GetConfigFloat(hal->configfile, "Skyscraper.Frontend.Menu.Button2.X", 0.0);
			y = hal->GetConfigFloat(hal->configfile, "Skyscraper.Frontend.Menu.Button2.Y", 0.125);
			center = hal->GetConfigBool(hal->configfile, "Skyscraper.Frontend.Menu.Button2.Center", true);
		}
		if (i == 2)
		{
			b1 = "data/" + hal->GetConfigString(hal->configfile, "Skyscraper.Frontend.Menu.Button3.Image", "button_searstower.png");
			b2 = "data/" + hal->GetConfigString(hal->configfile, "Skyscraper.Frontend.Menu.Button3.Selected", "button_searstower_selected.png");
			b3 = "data/" + hal->GetConfigString(hal->configfile, "Skyscraper.Frontend.Menu.Button3.Pressed", "button_searstower_pressed.png");
			x = hal->GetConfigFloat(hal->configfile, "Skyscraper.Frontend.Menu.Button3.X", 0.0);
			y = hal->GetConfigFloat(hal->configfile, "Skyscraper.Frontend.Menu.Button3.Y", 0.333);
			center = hal->GetConfigBool(hal->configfile, "Skyscraper.Frontend.Menu.Button3.Center", true);
		}
		if (i == 3)
		{
			b1 = "data/" + hal->GetConfigString(hal->configfile, "Skyscraper.Frontend.Menu.Button4.Image", "button_simple.png");
			b2 = "data/" + hal->GetConfigString(hal->configfile, "Skyscraper.Frontend.Menu.Button4.Selected", "button_simple_selected.png");
			b3 = "data/" + hal->GetConfigString(hal->configfile, "Skyscraper.Frontend.Menu.Button4.Pressed", "button_simple_pressed.png");
			x = hal->GetConfigFloat(hal->configfile, "Skyscraper.Frontend.Menu.Button4.X", 0.0);
			y = hal->GetConfigFloat(hal->configfile, "Skyscraper.Frontend.Menu.Button4.Y", 0.541);
			center = hal->GetConfigBool(hal->configfile, "Skyscraper.Frontend.Menu.Button4.Center", true);
		}
		if (i == 4)
		{
			b1 = "data/" + hal->GetConfigString(hal->configfile, "Skyscraper.Frontend.Menu.Button5.Image", "button_other.png");
			b2 = "data/" + hal->GetConfigString(hal->configfile, "Skyscraper.Frontend.Menu.Button5.Selected", "button_other_selected.png");
			b3 = "data/" + hal->GetConfigString(hal->configfile, "Skyscraper.Frontend.Menu.Button5.Pressed", "button_other_pressed.png");
			x = hal->GetConfigFloat(hal->configfile, "Skyscraper.Frontend.Menu.Button5.X", 0.0);
			y = hal->GetConfigFloat(hal->configfile, "Skyscraper.Frontend.Menu.Button5.Y", 0.75);
			center = hal->GetConfigBool(hal->configfile, "Skyscraper.Frontend.Menu.Button5.Center", true);
		}
		if (i > 4)
		{
			b1 = "data/" + hal->GetConfigString(hal->configfile, "Skyscraper.Frontend.Menu.Button" + number + ".Image", "");
			b2 = "data/" + hal->GetConfigString(hal->configfile, "Skyscraper.Frontend.Menu.Button" + number + ".Selected", "");
			b3 = "data/" + hal->GetConfigString(hal->configfile, "Skyscraper.Frontend.Menu.Button" + number + ".Pressed", "");
			x = hal->GetConfigFloat(hal->configfile, "Skyscraper.Frontend.Menu.Button" + number + ".X", 0.0);
			y = hal->GetConfigFloat(hal->configfile, "Skyscraper.Frontend.Menu.Button" + number + ".Y", 0.0);
			center = hal->GetConfigBool(hal->configfile, "Skyscraper.Frontend.Menu.Button" + number + ".Center", true);
		}

		result = DrawImage(b1, &buttons[i], x, y, center, b2, b3);

		if (result == false)
			return false;
	}

	return true;
}

bool StartScreen::DrawImage(const std::string &filename, buttondata *button, Real x, Real y, bool center, const std::string &filename_selected, const std::string &filename_pressed)
{
	//X and Y represent the image's top-left location.
	//values are -1 for the top left, 1 for the top right, -1 for the top, and 1 for the bottom
	//center is at 0, 0

	Real w, h;
	int w_orig = 0, h_orig = 0, w2 = 0, h2 = 0;
	bool background = false;

	std::string material = "";
	std::string Filename = filename;

	if (filename == "")
		return false;

	//exit if background has already been drawn
	if (background_image == Filename)
		return true;

	Ogre::TextureManager::getSingleton().setVerbose(false);
	Ogre::TexturePtr tex = Ogre::TextureManager::getSingleton().getByName(Filename);
	if (tex)
		material = Filename;

	//load image data from file, if not already preloaded
	if (material == "")
	{
		int count = 1;
		if (button)
			count = 3;

		for (int i = 0; i < count; i++)
		{
			if (i == 0)
				Filename = filename;
			if (i == 1)
				Filename = filename_selected;
			if (i == 2)
				Filename = filename_pressed;

			//create new material
			Ogre::MaterialPtr mat;
			try
			{
				mat = Ogre::MaterialManager::getSingleton().create(Filename, "General");
			}
			catch (Ogre::Exception& e)
			{
				return vm->GetHAL()->ReportFatalError("Error creating material for texture " + Filename + "\n" + e.getDescription(), "");
			}

			//load image data from file
			Ogre::Image img;
			try
			{
				img.load(Filename, "General");
			}
			catch (Ogre::Exception &e)
			{
				return vm->GetHAL()->ReportFatalError("Error loading texture " + Filename + "\n" + e.getDescription(), "");
			}

			w_orig = img.getWidth();
			h_orig = img.getHeight();

			//round up image size to power-of-2 value
			w2 = powf(2, ceilf(Log2((Real)w_orig)));
			h2 = powf(2, ceilf(Log2((Real)h_orig)));

			//create texture and blit image onto it - this solves texture quality issues on mainly D3D9
			tex = Ogre::TextureManager::getSingleton().createManual(Filename, "General", Ogre::TEX_TYPE_2D, w2, h2, 0, Ogre::PF_R8G8B8A8, Ogre::TU_STATIC);
			tex->getBuffer(0, 0)->blitFromMemory(img.getPixelBox(0, 0), Ogre::Box(0, 0, 0, w_orig, h_orig, img.getDepth()));

			//bind texture to material and set options
			Ogre::TextureUnitState *state = mat->getTechnique(0)->getPass(0)->createTextureUnitState(Filename);
			Ogre::Pass *pass = mat->getTechnique(0)->getPass(0);
			pass->setDepthCheckEnabled(false);
			pass->setDepthWriteEnabled(false);
			pass->setLightingEnabled(false);
			pass->setTextureFiltering(Ogre::TFO_NONE);

			//rescale texture
			state->setTextureScale((Real)w2 / (Real)w_orig, (Real)h2 / (Real)h_orig);
			state->setTextureScroll(-(Real(w2 - w_orig) / (Real)w2) / 2.0, -(Real(h2 - h_orig) / (Real)h2) / 2.0);

			if (tex->hasAlpha() == true && button)
			{
				mat->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
				mat->getTechnique(0)->getPass(0)->setAlphaRejectSettings(Ogre::CMPF_GREATER_EQUAL, 128);
			}
		}
		material = Filename;
		if (button)
		{
			button->node = 0;
			button->drawn_selected = false;
			button->drawn_pressed = false;
			button->active_button = 0;
			button->rect = 0;
		}
		else
		{
			background_rect = 0;
			background_node = 0;
		}
	}

	//exit if requested button is already visible
	if (button)
	{
		if (button->drawn_selected == false && button->drawn_pressed == false)
		{
			if (button->active_button == 1)
				return true;
			else
			{
				button->active_button = 1;
				material = filename;
			}
		}

		if (button->drawn_selected == true)
		{
			if (button->active_button == 2)
				return true;
			else
			{
				button->active_button = 2;
				material = filename_selected;
			}
		}

		if (button->drawn_pressed == true)
		{
			if (button->active_button == 3)
				return true;
			else
			{
				button->active_button = 3;
				material = filename_pressed;
			}
		}
	}

	//set values and draw button
	if (material != "")
	{
		//apply content scaling factor, fixes issues for example on Retina displays
#ifdef USING_WX
		Real scale = frontend->GetWindow()->GetContentScaleFactor();
#else
		Real scale = 1.0;
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
		//set scale to 1.0 on MacOS versions earlier than 10.15
		if (vm->macos_major == 10 && vm->macos_minor < 15)
			scale = 1.0;
#endif

		w = w_orig / (vm->GetHAL()->GetRenderWindow()->getWidth() / 2.0 / scale);
		h = h_orig / (vm->GetHAL()->GetRenderWindow()->getHeight() / 2.0 / scale);
		if (button)
		{
			//delete previous object
			if (button->node)
				button->node->detachAllObjects();
			if (button->rect)
				delete button->rect;
			button->rect = 0;

			if (button->filename == "")
			{
				//store general button data
				button->filename = filename;
				button->filename_selected = filename_selected;
				button->filename_pressed = filename_pressed;

				button->offset_x = x;
				button->offset_y = y;
				if (center == true)
				{
					button->x = x - (w / 2);
					button->y = y - (h / 2);
				}
				else
				{
					button->x = x;
					button->y = y;
				}
				button->size_x = w;
				button->size_y = h;
			}

			x = button->x;
			y = button->y;
			w = button->size_x;
			h = button->size_y;
		}
		else
		{
			background_image = material;
			background = true;
			if (center == true)
			{
				x += -(w / 2);
				y += -(h / 2);
			}
		}

		//create rectangle
		Ogre::Rectangle2D* rect = new Ogre::Rectangle2D(true);
		rect->setCorners(x, -y, x + w, -(y + h));
		Ogre::MaterialPtr materialptr = Ogre::MaterialManager::getSingleton().getByName(material);
		rect->setMaterial(materialptr);
		if (background == true)
			rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);

		//set infinite bounding box
		Ogre::AxisAlignedBox aabInf;
		aabInf.setInfinite();
		rect->setBoundingBox(aabInf);

		//attach scene node
		Ogre::SceneNode* node = vm->GetHAL()->GetSceneManager()->getRootSceneNode()->createChildSceneNode();
		node->attachObject(rect);
		if (button)
		{
			button->node = node;
			button->rect = rect;
		}
		else
		{
			background_node = node;
			background_rect = rect;
		}
	}
	return true;
}

bool StartScreen::GetMenuInput()
{
	//input handler for main menu

	//exit if there aren't any buttons
	if (!buttons || buttoncount == 0)
		return false;

#ifdef USING_WX
	MainScreen *window = frontend->GetWindow();

	//get mouse coordinates
	int mouse_x = window->ScreenToClient(wxGetMousePosition()).x;
	int mouse_y = window->ScreenToClient(wxGetMousePosition()).y;

	for (int i = 0; i < buttoncount; i++)
	{
		buttondata *button = &buttons[i];

	    //only process buttons if main window is selected
        if (window->Active != false)
        {
			Real mx = mouse_x;
			Real my = mouse_y;
			Real w = mx / window->GetClientSize().x;
			Real h = my / window->GetClientSize().y;
			Real mouse_x_rel = (w * 2) - 1;
			Real mouse_y_rel = (h * 2) - 1;

			//change button status based on mouse position and button press status
			if (mouse_x_rel > button->x && mouse_x_rel < button->x + button->size_x && mouse_y_rel > button->y && mouse_y_rel < button->y + button->size_y)
			{
				if (button->drawn_selected == false && wxGetMouseState().LeftIsDown() == false)
				{
					if (button->drawn_pressed == true)
					{
						//user clicked on button
						button->drawn_selected = true;
						RightClick(i);
						return true;
        			}
        			button->drawn_selected = true;
        		}
				if (button->drawn_selected == false && wxGetMouseState().RightIsDown() == false)
				{
					if (button->drawn_pressed == true)
					{
						//user clicked on button
						button->drawn_selected = true;
						Click(i);
						return true;
					}
					button->drawn_selected = true;
				}
				if (button->drawn_pressed == false && (wxGetMouseState().LeftIsDown() == true || wxGetMouseState().RightIsDown() == true))
				{
					button->drawn_pressed = true;
					button->drawn_selected = false;
				}
			}
			else if (button->drawn_selected == true || button->drawn_pressed == true)
			{
				button->drawn_selected = false;
				button->drawn_pressed = false;
			}
		}
	}
#endif
	return true;
}

void StartScreen::Click(int index)
{
	//user clicked a button

	std::string number = ToString(index + 1);
	std::string filename = "";

	HAL *hal = vm->GetHAL();

	if (index == 0)
		filename = hal->GetConfigString(hal->configfile, "Skyscraper.Frontend.Menu.Button1.File", "Triton Center.bld");
	if (index == 1)
		filename = hal->GetConfigString(hal->configfile, "Skyscraper.Frontend.Menu.Button2.File", "Glass Tower.bld");
	if (index == 2)
		filename = hal->GetConfigString(hal->configfile, "Skyscraper.Frontend.Menu.Button3.File", "Sears Tower.bld");
	if (index == 3)
		filename = hal->GetConfigString(hal->configfile, "Skyscraper.Frontend.Menu.Button4.File", "Simple.bld");
	if (index > 3)
		filename = hal->GetConfigString(hal->configfile, "Skyscraper.Frontend.Menu.Button" + number + ".File", "");

	if (filename == "")
	{
		//show file selection dialog
		filename = frontend->SelectBuilding();
	}

	if (filename != "")
	{
		frontend->Load(filename);
	}
}

void StartScreen::RightClick(int index)
{
	//user clicked a button

	std::string number = ToString(index + 1);
	std::string filename = "";

	HAL *hal = vm->GetHAL();

	if (index == 0) {}
	if (index == 1) {}
	if (index == 2) {}
	if (index == 3) {}
	if (index > 3)
		filename = frontend->SelectBuilding(true);

	if (filename != "")
	{
		frontend->Load(filename);
	}
}

void StartScreen::DeleteButtons()
{
	if (buttoncount > 0)
	{
		for (int i = 0; i < buttoncount; i++)
		{
			buttondata *button = &buttons[i];

			if (button->node)
			{
				button->node->detachAllObjects();
				button->node->getParent()->removeChild(button->node);
				button->node = 0;
			}
			if (button->rect)
				delete button->rect;
			button->rect = 0;
		}
		delete [] buttons;
		buttoncount = 0;
	}
	buttons = 0;

	if (background_node)
	{
		background_node->detachAllObjects();
		background_node->getParent()->removeChild(background_node);
		background_node = 0;
	}
	if (background_rect)
		delete background_rect;
	background_rect = 0;
	background_image = "";
}

}
