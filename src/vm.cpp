/*
	Skyscraper 3.0 - Nanokernel
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

#include "vm.h"

using namespace Skyscraper;

void VM::_bind_methods()
{
}

VM::VM()
{
	time_passed = 0.0;

	frontend = new ::Skyscraper::Skyscraper();
	frontend->OnInit();
}

VM::~VM()
{
	frontend->OnExit();
	delete frontend;
}

void VM::_process(double delta)
{
	time_passed += delta;
	frontend->Loop();
}
