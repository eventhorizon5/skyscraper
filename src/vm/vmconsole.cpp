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
#include <mutex>
#include "globals.h"
#include "sbs.h"
#include "vm.h"
#include "hal.h"
#include "scriptproc.h"
#include "enginecontext.h"
#include "vmconsole.h"

namespace Skyscraper {

//Virtual Manager Console

VMConsoleResult consoleresult; //console input result
std::mutex mtx; //io lock mutex

void VMConsoleInput::operator()(int delay)
{
	while (true)
	{
		mtx.try_lock();
		std::cout << "> ";
		std::getline(std::cin, consoleresult.textbuffer);
		mtx.unlock();
		consoleresult.ready = true;

		//wait on VMConsole server
		while (consoleresult.server_ready == false)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(delay));
		}

		//wait for VMConsole to complete
		while (consoleresult.threadwait == true)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(delay));
		}
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
		std::string buffer;
		mtx.try_lock();
		buffer = consoleresult.textbuffer;
		mtx.unlock();

		consoleresult.server_ready = true;

		if (buffer == "")
		{
			consoleresult.ready = false;
			return;
		}
		consoleresult.threadwait = true;

		ScriptProcessor *processor = vm->GetActiveScriptProcessor();

		std::string commandline = buffer;
		int pos = commandline.find(" ", 0);
		if (pos > 0)
		{
			std::string command = commandline.substr(0, pos);
			SBS::TrimString(command);
			Ogre::StringVector params;
			SBS::SplitString(params, commandline.substr(pos), ' ');

			//commands
			if (command == "shutdown")
			{
				if (params.size() != 1)
					Report ("Incorrect number of parameters");

				if (params[0] == "all")
					vm->DeleteEngines();
				else
				{
					EngineContext *engine = vm->GetEngine(SBS::ToInt(params[0]));

					if (engine)
						engine->Shutdown();
				}
			}
		}
		else if (processor)
		{
			processor->GetEngine()->GetSystem()->DeleteColliders = true;

			//load new commands into script interpreter, and run
			processor->LoadFromText(buffer);
		}
		else
		{
			Report("No active engine");
		}

		consoleresult.ready = false;
		consoleresult.threadwait = false;
	}
}

bool VMConsole::Report(const std::string &text)
{
	std::cout << std::endl << text << std::endl;
	return true;
}

bool VMConsole::ReportError(const std::string &text)
{
	Report(text);
	return false;
}

}
