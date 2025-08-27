/*
	Skyscraper 2.1 - Editor
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

#include <OgreRoot.h>
#include <OgreImGuiOverlay.h>
#include <OgreImGuiInputListener.h>

//ImGuizmo system
//#include "ImGuizmo.h"

#include "globals.h"
#include "sbs.h"
#include "vm.h"
#include "hal.h"
#include "enginecontext.h"
#include "editor.h"

namespace Skyscraper {

Editor::Editor(VM *parent)
{
	this->vm = parent;
	enabled = false;
	initialized = false;
}

Editor::~Editor()
{

}

bool Editor::Initialize()
{
	HAL *hal = vm->GetHAL();

	//initialize ImGui Overlay
	if (hal->GetConfigBool(hal->configfile, "Skyscraper.Frontend.VR", false) == false)
	{
		try
		{
			imgui = new Ogre::ImGuiOverlay();
			imgui->setZOrder(300);
			imgui->show();
			Ogre::OverlayManager::getSingleton().addOverlay(imgui);
		}
		catch(Ogre::Exception &e)
		{
			return hal->ReportFatalError("Error initializing ImGui overlay\nDetails: " + e.getDescription(), "editor:");
		}
	}

	initialized = true;
	return true;
}

bool Editor::Run()
{
	//process imgui

	if (!imgui || !initialized)
		return false;

	Ogre::ImGuiOverlay::NewFrame();
	//ImGuizmo::BeginFrame();

	if (!enabled)
		return true;

	ImGui::ShowDemoWindow();

	return true;
}

void Editor::Unload()
{
	//remove imgui overlay

	if (!imgui || !initialized)
		return;

	imgui->hide();
	Ogre::OverlayManager::getSingleton().destroy(imgui);
	//delete imgui;
	imgui = 0;
	initialized = false;
}

void Editor::Enable(bool value)
{
	enabled = value;
}

bool Editor::IsEnabled()
{
	return enabled;
}

bool Editor::IsInitialized()
{
	return initialized;
}

void Editor::UpdateFrame(Real size_x, Real size_y, Real scale, Real delta)
{
	//update ImGui frame data

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)size_x, (float)size_y);
	io.DisplayFramebufferScale = ImVec2(scale, scale);
	io.DeltaTime = delta > 0.f ? delta : (1.0f / 60.0f);
}

}
