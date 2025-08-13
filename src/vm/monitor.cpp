/*
	Skyscraper 2.1 - Monitor System
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
#include "profiler.h"
#include "vm.h"
#include "enginecontext.h"
#include "monitor.h"

using namespace SBS;

namespace Skyscraper {

Monitor::Monitor(VM *vm)
{
    this->vm = vm;
	root = 0;
}

Monitor::~Monitor()
{

}

bool Monitor::CreateSim()
{
	if (root)
		return false;

	//create a solar system instance
	root = vm->Initialize(false);
	if (!root)
		return vm->ReportFatalError("Error creating root engine instance");

	root->IsSystem = true;

	bool result = root->Load("Sol.bld");
	if (!result)
		return vm->ReportFatalError("Error loading Sol.bld");

	return true;
}

void Monitor::Cleanup()
{
	root = 0;
}

bool Monitor::Run()
{
	//monitor runloop

	/*EngineContext *planet = root->GetChild(0);
	if (planet)
	{
		if (planet->IsRunning())
		{
			Vector3 vec (-1, 0, 0);
			planet->Move(vec, 1000, true);
		}
	}*/
	return true;
}

}
