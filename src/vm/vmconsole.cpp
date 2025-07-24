/*
	Skyscraper 2.0 - Virtual Manager Console
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
#include <ctime>
#include "globals.h"
#include "sbs.h"
#include "vm.h"
#include "hal.h"
#include "sky.h"
#include "scriptproc.h"
#include "enginecontext.h"
#include "profiler.h"
#include "gui.h"
#include "vmconsole.h"

namespace Skyscraper {

//Virtual Manager Console

VMConsoleResult consoleresult; //console input result
std::mutex mtx_io; //io lock mutex
std::string prompt; //console prompt
std::atomic<bool> shutdown; //set to true to shut down input thread

void VMConsoleInput::operator()(int delay)
{
	while (true)
	{
		if (shutdown == true)
			break;

		if (mtx_io.try_lock())
		{
			//output console prompt
			std::string color = GetColors("green");
			std::string reset = GetColors("reset");
			std::cout << color << prompt << reset;

			//get keyboard input
			std::getline(std::cin, consoleresult.textbuffer);

			mtx_io.unlock();
		}
		else
		{
			//if thread can't get lock, sleep and return
			std::this_thread::sleep_for(std::chrono::milliseconds(delay));
			continue;
		}

		//reset states
		consoleresult.server_ready = false;
		consoleresult.ready = true;
		int i = 0;

		//TODO:
		//fix thread synchronization issues

		//wait for console server to be ready
		while (consoleresult.server_ready == false)
		{
			i++;
			if (i > 500) //deadlock prevention
				break;
			std::this_thread::sleep_for(std::chrono::milliseconds(delay));
		}
		i = 0;

		//wait for console to complete
		while (consoleresult.threadwait == true)
		{
			i++;
			if (i > 500) //deadlock prevention
				break;
			std::this_thread::sleep_for(std::chrono::milliseconds(delay));
		}
	}
}

VMConsole::VMConsole(VM *vm)
{
	this->vm = vm;

	//create VM console instance
#ifdef USING_WX
	if (vm->GetGUI()->IsConsoleVisible() == false)
#endif
	{
		std::thread coninput(VMConsoleInput(), 1);
		coninput.detach();
	}

	Report("\nWelcome to Virtual Manager\n", "cyan");
}

VMConsole::~VMConsole()
{
	SBS::ProfileManager::CleanupMemory();
	shutdown = true;
}

void VMConsole::Process(const std::string &text, bool echo)
{
	//process console input

	if (consoleresult.ready == false && text.size() == 0)
		return;

	if (text.size() > 0 && echo == true)
	{
		buffer = text;
		Report(buffer, "white");
	}
	else if (consoleresult.ready == true)
	{
		if (mtx_io.try_lock())
		{
			//in lock, copy atomic string into new buffer and unlock
			buffer = consoleresult.textbuffer;

			if (vm->GetActiveEngine())
			{
				int number = vm->GetActiveEngine()->GetNumber();
				prompt = "\n" + SBS::ToString(number) + "> ";
			}
			else
				prompt = "\n> ";

			mtx_io.unlock();
		}
		else
		{
			//if thread can't get lock, sleep and return
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			return;
		}

		if (buffer == "")
		{
			//reset and exit if buffer is empty
			consoleresult.ready = false;
			return;
		}
		consoleresult.threadwait = true;
		consoleresult.server_ready = true;
	}

	ScriptProcessor *processor = vm->GetActiveScriptProcessor();

	std::string commandline = buffer;
	int pos = commandline.find(" ", 0);
	std::string command;
	Ogre::StringVector params;
	if (pos > 0)
	{
		//get command line
		command = commandline.substr(0, pos);
		SBS::TrimString(command);

		//get parameters
		SBS::SplitString(params, commandline.substr(pos), ',');
	}
	else
		command = commandline;

	//shutdown command
	if (command == "shutdown")
	{
		if (params.size() != 1)
			Report ("Incorrect number of parameters");
		else
		{
			if (params[0] == "all")
				vm->DeleteEngines();
			else
			{
				EngineContext *engine = vm->GetEngine(SBS::ToInt(params[0]));

				if (engine)
					engine->Shutdown();
			}
		}
		consoleresult.ready = false;
		consoleresult.threadwait = false;
		return;
	}

	//setactive command
	if (command == "setactive")
	{
		if (params.size() != 1)
			Report("Incorrect number of parameters");
		else
			vm->SetActiveEngine(SBS::ToInt(params[0]));
		consoleresult.ready = false;
		consoleresult.threadwait = false;
		return;
	}

	//reload command
	if (command == "reload")
	{
		if (params.size() == 0)
		{
			EngineContext *engine = vm->GetActiveEngine();

			if (engine)
				engine->Reload = true;
		}
		else if (params[0] == "all")
		{
			for (int i = 0; i < vm->GetEngineCount(); i++)
			{
				EngineContext *engine = vm->GetEngine(i);

				if (engine)
					engine->Reload = true;
			}
		}
		else
		{
			EngineContext *engine = vm->GetEngine(SBS::ToInt(params[0]));

			if (engine)
				engine->Reload = true;
		}
		consoleresult.ready = false;
		consoleresult.threadwait = false;
		return;
	}

	//vmload command
	if (command == "vmload")
	{
		if (params.size() != 1)
			Report("Incorrect number of parameters");
		else
			vm->Load(false, params[0]);
		consoleresult.ready = false;
		consoleresult.threadwait = false;
		return;
	}

	//switch command
	if (command == "switch")
	{
		if (params.size() != 1)
			Report("Incorrect number of parameters");
		else
			vm->SetActiveEngine(SBS::ToInt(params[0]), true);
		consoleresult.ready = false;
		consoleresult.threadwait = false;
		return;
	}

	//version command
	if (command == "version")
	{
		Report("Skyscraper Frontend version " + vm->version + " " + vm->version_state);
		Report("VM version " + vm->version_full);
		if (processor)
			Report("SBS version " + processor->GetEngine()->GetSystem()->version);
		else
			Report("No SBS loaded");
		consoleresult.ready = false;
		consoleresult.threadwait = false;
		return;
	}

	//arch command
	if (command == "arch")
	{
		Report(vm->Architecture);
		consoleresult.ready = false;
		consoleresult.threadwait = false;
		return;
	}

	//platform command
	if (command == "platform")
	{
		Report(vm->Platform);
		consoleresult.ready = false;
		consoleresult.threadwait = false;
		return;
	}

	//uname command
	if (command == "uname")
	{
		if (params.size() > 0)
		{
			std::string delim = " ";
			if (vm->version_state == "")
				delim = "";
			Report("VM " + vm->version + delim + vm->version_state + " (" + vm->Architecture + " " + vm->Bits + "), build " + vm->version_rev);
		}
		else
			Report("VM");
		consoleresult.ready = false;
		consoleresult.threadwait = false;
		return;
	}

	//start command
	if (command == "start")
	{
		vm->Load(false, "Ground.bld");
		consoleresult.ready = false;
		consoleresult.threadwait = false;
		return;
	}

	//ps command
	if (command == "ps")
	{
		int count = vm->GetEngineCount();
		Report(SBS::ToString(count) + " engines running\n", "cyan");
		for (int i = 0; i < count; i++)
		{
			EngineContext *engine = vm->GetEngine(i);
			Real elapsed_time = 0;
			if (vm->GetElapsedTime(i) > 0)
				elapsed_time = Real(vm->GetElapsedTime(i) / Real(vm->time_stat));
			unsigned long runtime = engine->GetSystem()->GetRunTime();
			Report(SBS::ToString(i) + ":\t" + SBS::ToString(elapsed_time * 100) + "\t\t" + SBS::ToString(runtime / 1000) + "\t\t" + engine->GetFilename(), "green");
		}
		consoleresult.ready = false;
		consoleresult.threadwait = false;
		return;
	}

	//date command
	if (command == "date")
	{
		//print current date and time
		time_t timestamp = time(0);
		struct tm datetime = *localtime(&timestamp);
		Report(asctime(&datetime));

		if (!vm->GetActiveScriptProcessor())
		{
			consoleresult.ready = false;
			consoleresult.threadwait = false;
			return;
		}

		//print simulator time
		Report("Simulator time: ");
		int year, month, day, hour, minute, second;
		SkySystem* sky = vm->GetSkySystem();
		if (sky->GetCaelumSystem())
		{
			sky->GetDate(year, month, day);
			sky->GetTime(hour, minute, second);

			std::string month_s;
			switch (month)
			{
			case 1:
				month_s = "January";
				break;
			case 2:
				month_s = "February";
				break;
			case 3:
				month_s = "March";
				break;
			case 4:
				month_s = "April";
				break;
			case 5:
				month_s = "May";
				break;
			case 6:
				month_s = "June";
				break;
			case 7:
				month_s = "July";
				break;
			case 8:
				month_s = "August";
				break;
			case 9:
				month_s = "September";
				break;
			case 10:
				month_s = "October";
				break;
			case 11:
				month_s = "November";
				break;
			case 12:
				month_s = "December";
				break;
			}

			std::string pm = "am";
			int hr = hour;
			if (hour > 12)
			{
				pm = "pm";
				hr -= 12;
			}
			Report(month_s + " " + SBS::ToString(day) + ", " + SBS::ToString(year) + " " + SBS::ToString(hr) + ":" + SBS::ToString(minute) + ":" + SBS::ToString(second) + " " + pm);
		}
		else
			Report("No sky system loaded");

		consoleresult.ready = false;
		consoleresult.threadwait = false;
		return;
	}

	//uptime command
	if (command == "uptime")
	{
		EngineContext *engine;

		if (params.size() == 0)
			engine = vm->GetActiveEngine();
		else
			engine = vm->GetEngine(SBS::ToInt(params[0]));

		if (engine)
		{
			//get running time of SBS instance
			Real uptime = engine->GetSystem()->running_time;

			Report(SBS::ToString(uptime) + " seconds");
		}
		consoleresult.ready = false;
		consoleresult.threadwait = false;
		return;
	}

	//uptime command
	if (command == "vmuptime")
	{
		unsigned long uptime = vm->Uptime() / 1000;
		Report(SBS::ToString(uptime) + " seconds");

		consoleresult.ready = false;
		consoleresult.threadwait = false;
		return;
	}

	//profile command
	if (command == "profile")
	{
		SBS::enable_profiling = true;
		SBS::enable_advanced_profiling = false;
		if (params.size() == 1)
		{
			if (params[0] == "-a")
				SBS::enable_advanced_profiling = true;
		}
		std::string output;
		SBS::ProfileManager::dumpAll(output);
		Report(output, "green");
		consoleresult.ready = false;
		consoleresult.threadwait = false;
		return;
	}

	//vminit command
	if (command == "vminit")
	{
		EngineContext* engine = vm->Initialize(false);
		engine->LoadDefault();
		consoleresult.ready = false;
		consoleresult.threadwait = false;
		return;
	}

	//boot command
	if (command == "boot")
	{
		if (vm->GetEngineCount() == 0)
		{
			ReportError("No engine, run vminit");
			consoleresult.ready = false;
			consoleresult.threadwait = false;
			return;
		}

		if (vm->GetEngineCount() == 1 && params.size() == 0)
		{
			vm->GetActiveEngine()->Boot();
			consoleresult.ready = false;
			consoleresult.threadwait = false;
			return;
		}

		if (params.size() == 1)
		{
			EngineContext *engine = vm->GetEngine(SBS::ToInt(params[0]));
			if (engine)
				engine->Boot();
			else
				ReportError("Invalid engine");
		}
		consoleresult.ready = false;
		consoleresult.threadwait = false;
		return;
	}

	//help command
	if (command == "help" || command == "?")
	{
		if (params.size() == 0)
		{
			Report("Commands:");
			Report("shutdown engine_number|all - shuts down the specified engine");
			Report("setactive engine_number - makes the specified engine active");
			Report("reload [all] - reload the current engine or all engines");
			Report("vmload filename - load building data file");
			Report("switch engine_number - switch to the specified engine");
			Report("version - print versions");
			Report("arch - print machine architecture");
			Report("platform - print platform information");
			Report("uname - print VM name and version");
			Report("start - start simulator with a ground scene");
			Report("ps - show engine process list");
			Report("date - show real and simulator date and time");
			Report("uptime [engine_number] - show the SBS engine uptime in seconds");
			Report("vmuptime - show uptime of VM in seconds");
			Report("profile [-a] - shows function-level profiling statistics");
			Report("vminit - create and initialize a simulator engine");
			Report("boot [engine_number] - start a simulator engine");
			Report("help - print this help guide\n");
			Report("All other commands will be passed to the active simulator engine, if available");
#ifdef USING_WX
			if (vm->GetGUI()->IsConsoleVisible() == false)
#endif
				Report("\nPress CTRL-c to quit");
		}
		consoleresult.ready = false;
		consoleresult.threadwait = false;
		return;
	}

	if (processor)
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
	return;
}

bool VMConsole::Report(const std::string &text, const std::string &color)
{
	//lock mutex, write to console and unlock

#ifdef USING_WX
	if (vm->GetGUI()->IsConsoleVisible() == false)
#else
	if (true)
#endif
	{
		if (mtx_io.try_lock())
		{
			vm->GetHAL()->ConsoleOut(text + "\n", color);
			mtx_io.unlock();
		}
	}
#ifdef USING_WX
	else
		vm->GetGUI()->WriteToConsole(text, color);
#endif
	return true;
}

bool VMConsole::ReportError(const std::string &text)
{
	Report(text, "red");
	return false;
}

std::string GetColors(const std::string &color)
{
	//get colors
	std::string mod;
	if (color == "blue")
		mod = "\033[1;34m";
	else if (color == "green")
		mod = "\033[1;32m";
	else if (color == "yellow")
		mod = "\033[1;33m";
	else if (color == "red")
		mod = "\033[1;31m";
	else if (color == "magenta")
		mod = "\033[1;35m";
	else if (color == "cyan")
		mod = "\033[1;36m";
	else if (color == "white")
		mod = "\033[1;37m";
	else if (color == "black")
		mod = "\033[1;30m";
	else if (color == "reset")
		mod = "\033[0m";

	return mod;
}

}
