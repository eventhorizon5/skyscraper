/*
	Skyscraper 2.1 - Virtual Manager Console
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

#include <thread>
#include <iostream>
#include "globals.h"
#include "sbs.h"
#include "vm.h"
#include "hal.h"
#include "scriptproc.h"
#include "enginecontext.h"
#include "vmconsole.h"

using namespace SBS;

namespace Skyscraper {

//Virtual Manager Console

VMConsoleResult consoleresult; //console input result

void VMConsoleInput::operator()(int delay)
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
		std::cout << "> ";
		std::getline(std::cin, consoleresult.textbuffer);
		consoleresult.ready = true;
	}
}

VMConsole::VMConsole(VM *vm)
{
	this->vm = vm;

	//create VM console instance
	std::thread coninput(VMConsoleInput(), 1);
	coninput.detach();
}

VMConsole::~VMConsole()
{

}

void VMConsole::Process()
{
	//process console input
	if (consoleresult.ready == true)
	{
		vm->GetHAL()->Report(consoleresult.textbuffer, ">");

		ScriptProcessor *processor = vm->GetActiveScriptProcessor();
		if (processor)
		{
			processor->GetEngine()->GetSystem()->DeleteColliders = true;

			//load new commands into script interpreter, and run
			processor->LoadFromText(consoleresult.textbuffer);
		}
		else
		{
			vm->Report("No active engine");
		}
		consoleresult.ready = false;
	}
}

}
