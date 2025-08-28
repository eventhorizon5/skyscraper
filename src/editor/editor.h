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

#ifndef EDITOR_H

namespace Ogre {
	class ImGuiOverlay;
}

namespace Skyscraper {

class EngineContext;
class VM;
class HAL;

class VMIMPEXP Editor
{
public:
	Editor(VM *parent);
	~Editor();
	bool Initialize();
	bool Run();
	void Unload();
	void Enable(bool value);
	bool IsEnabled();
	bool IsInitialized();
	void UpdateFrame(Real size_x, Real size_y, Real scale, Real delta);

private:

	void Process();
	void ShowTransformGizmoWindow();

	//ImGui Overlay
	Ogre::ImGuiOverlay *imgui;

	bool enabled, initialized;

	VM *vm;
};

}

#endif

