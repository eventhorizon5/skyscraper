/*
	Skyscraper 2.1 - Simulation Frontend
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

#include "skyscraper.h"
#include "vm.h"

using namespace SBS;

#ifndef SW_SHOWNORMAL
	#define SW_SHOWNORMAL 1
#endif

int main (int argc, char* argv[])
{
#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
	//force X11 if on Wayland
	setenv("GDK_BACKEND", "x11", false);
#endif

#ifdef USING_WX
	//main wxWidgets entry point
	wxEntry(argc, argv);
#else
	::Skyscraper::Skyscraper *skyscraper = new ::Skyscraper::Skyscraper();
	skyscraper->initApp();
	while (true)
	{
		skyscraper->Loop();
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	skyscraper->closeApp();
	return 0;
#endif
}
