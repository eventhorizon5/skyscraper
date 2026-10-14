/*
	Skyscraper 2.1 - Script Processor - Access Control Section and Commands
	Copyright (C)2003-2026 Ryan Thoryk
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
#include "vm.h"
#include "sky.h"
#include "enginecontext.h"
#include "floor.h"
#include "door.h"
#include "elevator.h"
#include "elevatorcar.h"
#include "buttonpanel.h"
#include "callstation.h"
#include "access.h"
#include "cardreader.h"
#include "scriptproc.h"
#include "section.h"

using namespace SBS;

namespace Skyscraper {

//get the engine's access controller, creating it (and connecting the virtual clock) on first use
static AccessController* GetAccess(::SBS::SBS *Simcore, EngineContext *engine)
{
	AccessController *access = Simcore->GetAccessController(false);
	if (access)
		return access;

	access = Simcore->GetAccessController(true);

	//schedules follow the sky system's virtual date and time (the same clock as the
	//%hour% and %minute% script variables), falling back to the system clock
	VM *vm = engine->GetVM();
	access->SetClockSource([vm](int &day_of_week, int &minute_of_day)
	{
		struct tm datetime = vm->GetDateTime();
		day_of_week = datetime.tm_wday;
		minute_of_day = datetime.tm_hour * 60 + datetime.tm_min;

		SkySystem *sky = vm->GetSkySystem();
		if (!sky)
			return;

		int hour = -1, minute = -1, second = -1;
		sky->GetTime(hour, minute, second);
		if (hour < 0)
			return; //sky system not running

		minute_of_day = hour * 60 + minute;

		//day of week from the virtual date (Sakamoto's method)
		int year = 0, month = 0, day = 0;
		sky->GetDate(year, month, day);
		if (month >= 1 && month <= 12)
		{
			static const int offsets[12] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
			if (month < 3)
				year -= 1;
			day_of_week = (year + year / 4 - year / 100 + year / 400 + offsets[month - 1] + day) % 7;
		}
	});

	return access;
}

//
// <Access> section
//

ScriptProcessor::AccessSection::AccessSection(ScriptProcessor *parent) : Section(parent)
{

}

int ScriptProcessor::AccessSection::Run(std::string &LineData)
{
	//access control database: schedules, groups, cards

	//IF/While statement stub (continue to global commands for processing)
	if (StartsWithNoCase(LineData, "if") || StartsWithNoCase(LineData, "while"))
		return sContinue;

	//process math functions
	if (MathFunctions(LineData) == sError)
		return sError;

	//process functions
	if (parent->FunctionProc() == true)
		return sNextLine;

	//Schedule command
	if (StartsWithNoCase(LineData, "schedule "))
	{
		//Schedule name, days, start, end
		int params = SplitData(LineData, 9, false);

		if (params != 4)
			return ScriptError("Syntax: Schedule name, days, start, end");

		int days = 0, start = 0, end = 0;
		if (AccessCore::ParseDays(tempdata[1], days) == false)
			return ScriptError("Invalid days: " + tempdata[1]);
		if (AccessCore::ParseClock(tempdata[2], start) == false || start > 1439)
			return ScriptError("Invalid start time: " + tempdata[2]);
		if (AccessCore::ParseClock(tempdata[3], end) == false)
			return ScriptError("Invalid end time: " + tempdata[3]);

		if (config->CheckScript == true)
			return sNextLine;

		std::string error;
		if (GetAccess(Simcore, engine)->GetCore()->AddScheduleWindow(tempdata[0], days, start, end, error) == false)
			return ScriptError(error);

		return sNextLine;
	}

	//GroupRule command (check before Group)
	if (StartsWithNoCase(LineData, "grouprule "))
	{
		//GroupRule group, access_point, schedule
		int params = SplitData(LineData, 10, false);

		if (params < 2 || params > 3)
			return ScriptError("Syntax: GroupRule group, access_point, schedule");

		if (config->CheckScript == true)
			return sNextLine;

		std::string schedule = (params == 3) ? tempdata[2] : "Always";

		AccessController *access = GetAccess(Simcore, engine);
		std::string error;

		if (access->GetCore()->AddGroupRule(tempdata[0], tempdata[1], schedule, error) == false)
			return ScriptError(error);

		//create the access point for doors, so it shows in listings even before doors are attached
		//(elevator floor rules such as "Elevator 1:Floors 6-9" don't need one)
		int elevator = 0;
		std::string floor_spec;
		if (AccessCore::ParseElevatorRule(tempdata[1], elevator, floor_spec) == false)
			access->GetAccessPoint(tempdata[1], true);
		return sNextLine;
	}

	//Group command
	if (StartsWithNoCase(LineData, "group "))
	{
		//Group name[, override]
		int params = SplitData(LineData, 6, false);

		if (params < 1 || params > 2)
			return ScriptError("Syntax: Group name[, override]");

		bool override_lockdown = false;
		if (params == 2)
		{
			std::string flag = tempdata[1];
			SetCase(flag, false);
			if (flag != "override")
				return ScriptError("Unknown group option: " + tempdata[1]);
			override_lockdown = true;
		}

		if (config->CheckScript == true)
			return sNextLine;

		std::string error;
		if (GetAccess(Simcore, engine)->GetCore()->AddGroup(tempdata[0], override_lockdown, error) == false)
			return ScriptError(error);

		return sNextLine;
	}

	//Card command
	if (StartsWithNoCase(LineData, "card "))
	{
		//Card facility, number, holder, groups[, home_floor]
		//home_floor is the floor's label as shown on buttons (or @N for an internal number)
		int params = SplitData(LineData, 5, false);

		if (params < 4 || params > 5)
			return ScriptError("Syntax: Card facility, number, holder, groups[, home_floor]");

		int facility = 0, number = 0;
		if (!IsNumeric(tempdata[0], facility))
			return ScriptError("Invalid facility code: " + tempdata[0]);
		if (!IsNumeric(tempdata[1], number))
			return ScriptError("Invalid card number: " + tempdata[1]);

		Credential credential;
		credential.facility_code = facility;
		credential.card_number = number;
		credential.holder = tempdata[2];

		//groups are separated by '|'
		std::string groups = tempdata[3];
		std::string current;
		for (size_t i = 0; i <= groups.size(); i++)
		{
			if (i == groups.size() || groups[i] == '|')
			{
				TrimString(current);
				if (!current.empty())
					credential.groups.emplace_back(current);
				current.clear();
			}
			else
				current += groups[i];
		}

		if (params == 5)
		{
			credential.home_floor = tempdata[4];
			TrimString(credential.home_floor);
		}

		if (config->CheckScript == true)
			return sNextLine;

		std::string error;
		if (GetAccess(Simcore, engine)->GetCore()->AddCredential(credential, error) == false)
			return ScriptError(error);

		return sNextLine;
	}

	//SecureFloors command
	if (StartsWithNoCase(LineData, "securefloors "))
	{
		//SecureFloors elevator, floors[, unlock_seconds[, autoselect]]
		//floors are labels as shown on buttons: "6-9", "R", "6-9|R"; use @ for internal numbers: "@5-8"
		int params = SplitData(LineData, 13, false);

		if (params < 2 || params > 4)
			return ScriptError("Syntax: SecureFloors elevator, floors[, unlock_seconds[, autoselect]]");

		int elevator = 0;
		if (!IsNumeric(tempdata[0], elevator) || elevator < 1)
			return ScriptError("Invalid elevator number: " + tempdata[0]);

		if (AccessCore::IsValidFloorSpec(tempdata[1]) == false)
			return ScriptError("Invalid floor list: " + tempdata[1]);

		Real unlock = 10;
		if (params >= 3 && (!IsNumeric(tempdata[2], unlock) || unlock <= 0))
			return ScriptError("Invalid unlock time: " + tempdata[2]);

		bool autoselect = (params == 4) ? ToBool(tempdata[3]) : false;

		if (config->CheckScript == true)
			return sNextLine;

		GetAccess(Simcore, engine)->SecureElevatorFloors(elevator, tempdata[1], (int)(unlock * 1000), autoselect);
		return sNextLine;
	}

	//StationUnlock command
	if (StartsWithNoCase(LineData, "stationunlock "))
	{
		//StationUnlock seconds - how long a badge tap at a call station allows keypad entry of secured floors
		int params = SplitData(LineData, 14);

		Real seconds = 0;
		if (params != 1 || !IsNumeric(tempdata[0], seconds) || seconds <= 0)
			return ScriptError("Syntax: StationUnlock seconds");

		if (config->CheckScript == true)
			return sNextLine;

		GetAccess(Simcore, engine)->SetStationUnlockTime((int)(seconds * 1000));
		return sNextLine;
	}

	//AutoClose command
	if (StartsWithNoCase(LineData, "autoclose "))
	{
		//AutoClose seconds - default auto-close time for all access-controlled doors (0 = off)
		int params = SplitData(LineData, 10);

		Real seconds = 0;
		if (params != 1 || !IsNumeric(tempdata[0], seconds) || seconds < 0)
			return ScriptError("Syntax: AutoClose seconds");

		if (config->CheckScript == true)
			return sNextLine;

		GetAccess(Simcore, engine)->SetDefaultAutoClose((int)(seconds * 1000));
		return sNextLine;
	}

	//WalletAdd command
	if (StartsWithNoCase(LineData, "walletadd "))
	{
		//WalletAdd facility, number
		int params = SplitData(LineData, 10);

		if (params != 2)
			return ScriptError("Syntax: WalletAdd facility, number");

		int facility = 0, number = 0;
		if (!IsNumeric(tempdata[0], facility) || !IsNumeric(tempdata[1], number))
			return ScriptError("Invalid card");

		if (config->CheckScript == true)
			return sNextLine;

		GetAccess(Simcore, engine)->WalletAdd(facility, number);
		return sNextLine;
	}

	//end of section
	if (StartsWithNoCase(LineData, "<endaccess>"))
	{
		config->SectionNum = SECTION_NONE;
		config->Context = "None";
		if (parent->InRunloop() == false)
			engine->Report("Finished access control");
		return sNextLine;
	}

	return sContinue;
}

//
// global commands, usable in any section and from the console
//

void ScriptProcessor::Section::AttachDoorToAccessPoint(::SBS::Door *door)
{
	//attach a newly created door to the current access point (set with SetAccessPoint)

	if (!door || config->accesspoint == "")
		return;

	AccessController *access = GetAccess(Simcore, engine);
	AccessPoint *point = access->GetAccessPoint(config->accesspoint, true);
	point->SetUnlockTime(config->accesspoint_unlock);
	point->SetHeldTime(config->accesspoint_held);
	point->SetAutoOpen(config->accesspoint_autoopen);
	if (config->accesspoint_autoclose >= 0)
		point->SetAutoCloseTime(config->accesspoint_autoclose);

	//a door on an access point must be locked; if SetLock wasn't used, lock both sides
	//(use SetLock 1 or 2 before the door to leave one side as free egress)
	if (door->GetLockSides() == 0)
		door->SetLocked(3, 0);

	point->AddDoor(door);
}

int ScriptProcessor::CommandsSection::AccessCommands(std::string &LineData)
{
	//SetAccessPoint command
	if (StartsWithNoCase(LineData, "setaccesspoint "))
	{
		//SetAccessPoint name, unlock_seconds[, held_seconds[, autoopen[, autoclose_seconds]]]
		//SetAccessPoint none
		std::string value = LineData.substr(15);
		TrimString(value);
		std::string lower = value;
		SetCase(lower, false);

		if (lower == "none")
		{
			config->accesspoint = "";
			return sNextLine;
		}

		int params = SplitData(LineData, 15);

		if (params < 2 || params > 5)
			return ScriptError("Syntax: SetAccessPoint name, unlock_seconds[, held_seconds[, autoopen[, autoclose_seconds]]]");

		Real unlock = 0, held = 30;
		if (!IsNumeric(tempdata[1], unlock) || unlock <= 0)
			return ScriptError("Invalid unlock time: " + tempdata[1]);
		if (params >= 3 && (!IsNumeric(tempdata[2], held) || held < 0))
			return ScriptError("Invalid held time: " + tempdata[2]);

		Real autoclose = -1;
		if (params == 5 && (!IsNumeric(tempdata[4], autoclose) || autoclose < 0))
			return ScriptError("Invalid auto-close time: " + tempdata[4]);

		config->accesspoint = tempdata[0];
		config->accesspoint_unlock = (int)(unlock * 1000);
		config->accesspoint_held = (int)(held * 1000);
		config->accesspoint_autoopen = (params >= 4) ? ToBool(tempdata[3]) : false;
		config->accesspoint_autoclose = (params == 5) ? (int)(autoclose * 1000) : -1;
		return sNextLine;
	}

	//AddCardReader command
	if (StartsWithNoCase(LineData, "addcardreader "))
	{
		//AddCardReader access_point, grant_sound, deny_sound, idle_texture, grant_texture, deny_texture,
		//              direction, centerx, centerz, width, height, voffset
		int params = SplitData(LineData, 14);

		if (params != 12)
			return ScriptError("Incorrect number of parameters");

		for (int i = 7; i <= 11; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		if (config->CheckScript == true)
			return sNextLine;

		//in an Elevator or Car section, the reader goes inside the car and moves with it
		if (config->SectionNum == SECTION_ELEVATOR || config->SectionNum == SECTION_ELEVATORCAR)
		{
			ElevatorCar *car = 0;
			if (config->SectionNum == SECTION_ELEVATORCAR)
			{
				Elevator *elev = Simcore->GetElevator(config->CurrentOld);
				if (elev)
					car = elev->GetCar(config->Current);
			}
			else
			{
				Elevator *elev = Simcore->GetElevator(config->Current);
				if (elev)
					car = elev->GetCar(1);
			}

			if (!car)
				return ScriptError("Card readers in an elevator need its car created first (CreateElevator)");

			CardReader *reader = GetAccess(Simcore, engine)->AddCabReader(car, tempdata[1], tempdata[2], tempdata[3], tempdata[4], tempdata[5], tempdata[6], ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]));
			if (!reader)
				return ScriptError("Unable to create card reader");

			StoreCommand(reader);
			return sNextLine;
		}

		//in a Floor section, voffset is relative to that floor's base
		Floor *floor = 0;
		if (config->SectionNum == SECTION_FLOOR)
			floor = Simcore->GetFloor(config->Current);

		CardReader *reader = GetAccess(Simcore, engine)->AddCardReader(floor, tempdata[0], tempdata[1], tempdata[2], tempdata[3], tempdata[4], tempdata[5], tempdata[6], ToFloat(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), ToFloat(tempdata[10]), ToFloat(tempdata[11]));

		if (!reader)
			return ScriptError("Unable to create card reader");

		StoreCommand(reader);
		return sNextLine;
	}

	//AddPanelReader command
	if (StartsWithNoCase(LineData, "addpanelreader "))
	{
		//AddPanelReader panel, grant_sound, deny_sound, idle_texture, grant_texture, deny_texture,
		//               row, column, width, height[, hoffset, voffset]
		//places a card reader on an elevator button panel, like AddButton
		int params = SplitData(LineData, 15);

		if (params != 10 && params != 12)
			return ScriptError("Incorrect number of parameters");

		for (int i = 6; i < params; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}
		if (!IsNumeric(tempdata[0]))
			return ScriptError("Invalid panel number: " + tempdata[0]);

		if (config->SectionNum != SECTION_ELEVATOR && config->SectionNum != SECTION_ELEVATORCAR && config->SectionNum != SECTION_CALLSTATION)
			return ScriptError("AddPanelReader must be used in an Elevator, Car or CallStation section");

		if (config->CheckScript == true)
			return sNextLine;

		Real hoffset = 0, voffset = 0;
		if (params == 12)
		{
			hoffset = ToFloat(tempdata[10]);
			voffset = ToFloat(tempdata[11]);
		}

		//call station: readers go on the station's button panel (create it first with CreatePanel)
		if (config->SectionNum == SECTION_CALLSTATION)
		{
			Floor *floor = Simcore->GetFloor(config->CurrentOld);
			CallStation *station = floor ? floor->GetCallStation(config->Current) : 0;
			if (!station)
				return ScriptError("Invalid call station");

			ButtonPanel *panel = station->GetPanel();
			if (!panel)
				return ScriptError("Call station has no panel; use CreatePanel first");

			CardReader *reader = GetAccess(Simcore, engine)->AddPanelReader(panel, tempdata[1], tempdata[2], tempdata[3], tempdata[4], tempdata[5], ToInt(tempdata[6]), ToInt(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), hoffset, voffset);
			if (!reader)
				return ScriptError("Unable to create panel card reader");

			StoreCommand(reader);
			return sNextLine;
		}

		ElevatorCar *car = 0;
		if (config->SectionNum == SECTION_ELEVATORCAR)
		{
			Elevator *elev = Simcore->GetElevator(config->CurrentOld);
			if (elev)
				car = elev->GetCar(config->Current);
		}
		else
		{
			Elevator *elev = Simcore->GetElevator(config->Current);
			if (elev)
				car = elev->GetCar(1);
		}
		if (!car)
			return ScriptError("Invalid elevator car");

		ButtonPanel *panel = car->GetPanel(ToInt(tempdata[0]));
		if (!panel)
			return ScriptError("Invalid panel number: " + tempdata[0]);

		CardReader *reader = GetAccess(Simcore, engine)->AddPanelReader(panel, tempdata[1], tempdata[2], tempdata[3], tempdata[4], tempdata[5], ToInt(tempdata[6]), ToInt(tempdata[7]), ToFloat(tempdata[8]), ToFloat(tempdata[9]), hoffset, voffset);
		if (!reader)
			return ScriptError("Unable to create panel card reader");

		StoreCommand(reader);
		return sNextLine;
	}

	//AddAccessZone command
	if (StartsWithNoCase(LineData, "addaccesszone "))
	{
		//AddAccessZone name, x1, y1, z1, x2, y2, z2, passback[, alarm_on_action, alarm_off_action]
		//passback is Off, Soft or Hard; in a Floor section, y values are relative to the floor's base
		int params = SplitData(LineData, 14);

		if (params != 8 && params != 10)
			return ScriptError("Syntax: AddAccessZone name, x1, y1, z1, x2, y2, z2, passback[, alarm_on_action, alarm_off_action]");

		for (int i = 1; i <= 6; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		std::string mode = tempdata[7];
		SetCase(mode, false);
		TrimString(mode);
		int passback = 0;
		if (mode == "off")
			passback = AccessController::PassbackOff;
		else if (mode == "soft")
			passback = AccessController::PassbackSoft;
		else if (mode == "hard")
			passback = AccessController::PassbackHard;
		else
			return ScriptError("Passback must be Off, Soft or Hard");

		if (config->CheckScript == true)
			return sNextLine;

		Real base = 0;
		if (config->SectionNum == SECTION_FLOOR)
		{
			Floor *floor = Simcore->GetFloor(config->Current);
			if (floor)
				base = floor->GetBase();
		}

		Vector3 min(ToFloat(tempdata[1]), ToFloat(tempdata[2]) + base, ToFloat(tempdata[3]));
		Vector3 max(ToFloat(tempdata[4]), ToFloat(tempdata[5]) + base, ToFloat(tempdata[6]));
		std::string alarm_on = (params == 10) ? tempdata[8] : "";
		std::string alarm_off = (params == 10) ? tempdata[9] : "";

		if (GetAccess(Simcore, engine)->AddZone(tempdata[0], min, max, passback, alarm_on, alarm_off) == false)
			return ScriptError("Unable to create zone " + tempdata[0]);
		return sNextLine;
	}

	//ZoneEntry command
	if (StartsWithNoCase(LineData, "zoneentry "))
	{
		//ZoneEntry zone, entry - entry is a door access point name, or "Floor <labels>" for elevator arrivals
		int params = SplitData(LineData, 10, false);

		if (params != 2)
			return ScriptError("Syntax: ZoneEntry zone, access_point | Floor labels");

		if (config->CheckScript == true)
			return sNextLine;

		if (GetAccess(Simcore, engine)->AddZoneEntry(tempdata[0], tempdata[1]) == false)
			return ScriptError("Unable to add zone entry");
		return sNextLine;
	}

	//PeopleAccess and ServicePeopleAccess commands
	if (StartsWithNoCase(LineData, "peopleaccess ") || StartsWithNoCase(LineData, "servicepeopleaccess "))
	{
		//PeopleAccess groups - badge groups for virtual people (separated by |)
		bool service = StartsWithNoCase(LineData, "service");
		int start = service ? 20 : 13;
		int params = SplitData(LineData, start, false);

		if (params != 1)
			return ScriptError("Syntax: PeopleAccess groups");

		std::vector<std::string> groups;
		std::string list = tempdata[0];
		std::string current;
		for (size_t i = 0; i <= list.size(); i++)
		{
			if (i == list.size() || list[i] == '|')
			{
				TrimString(current);
				if (!current.empty())
					groups.emplace_back(current);
				current.clear();
			}
			else
				current += list[i];
		}

		if (config->CheckScript == true)
			return sNextLine;

		AccessController *access = GetAccess(Simcore, engine);
		for (size_t i = 0; i < groups.size(); i++)
		{
			if (!access->GetCore()->GetGroup(groups[i]))
				return ScriptError("Unknown group " + groups[i]);
		}
		access->SetPeopleGroups(groups, service);
		return sNextLine;
	}

	//Access command (mainly for the console)
	std::string trimmed = LineData;
	TrimString(trimmed);
	SetCase(trimmed, false);
	if (StartsWithNoCase(LineData, "access ") || trimmed == "access")
	{
		std::string rest = (LineData.size() > 7) ? LineData.substr(7) : "";
		TrimString(rest);

		//split subcommand from its arguments
		std::string sub = rest, args;
		size_t space = rest.find(' ');
		if (space != std::string::npos)
		{
			sub = rest.substr(0, space);
			args = rest.substr(space + 1);
			TrimString(args);
		}
		SetCase(sub, false);

		if (config->CheckScript == true)
			return sNextLine;

		AccessController *access = GetAccess(Simcore, engine);

		if (sub == "" || sub == "help")
		{
			access->Report("Access commands: cards, points, elevators, zones, resetzone zone|all, wallet, next, log [count], export filename, present point, facility, number, "
				"tap point, autoclose [point,] seconds, addbadge facility, number, disable facility, number, enable facility, number, grant point, unlock point, lock point, lockdown on|off");
			return sNextLine;
		}
		if (sub == "cards")
		{
			access->ListCards();
			return sNextLine;
		}
		if (sub == "points")
		{
			access->ListAccessPoints();
			return sNextLine;
		}
		if (sub == "elevators")
		{
			access->ListElevators();
			return sNextLine;
		}
		if (sub == "zones")
		{
			access->ListZones();
			return sNextLine;
		}
		if (sub == "resetzone")
		{
			if (args == "")
				return ScriptError("Syntax: Access resetzone zone|all");
			access->ResetZone(args);
			return sNextLine;
		}
		if (sub == "wallet")
		{
			access->ListWallet();
			return sNextLine;
		}
		if (sub == "next")
		{
			access->NextBadge();
			return sNextLine;
		}
		if (sub == "log")
		{
			int count = 20;
			if (args != "" && !IsNumeric(args, count))
				return ScriptError("Invalid count: " + args);
			access->ShowLog(count);
			return sNextLine;
		}
		if (sub == "export")
		{
			if (args == "")
				return ScriptError("Syntax: Access export filename");
			access->ExportLog(args);
			return sNextLine;
		}
		if (sub == "autoclose")
		{
			//Access autoclose seconds            - all access-controlled doors
			//Access autoclose point, seconds     - one access point (seconds = default to follow the global setting)
			std::string value = args;
			size_t comma = value.rfind(',');
			std::string point_name;
			if (comma != std::string::npos)
			{
				point_name = value.substr(0, comma);
				value = value.substr(comma + 1);
				TrimString(point_name);
			}
			TrimString(value);
			std::string lower = value;
			SetCase(lower, false);

			if (point_name == "")
			{
				Real seconds = 0;
				if (!IsNumeric(value, seconds) || seconds < 0)
					return ScriptError("Syntax: Access autoclose seconds");
				access->SetDefaultAutoClose((int)(seconds * 1000));
				access->Report("Auto-close for all access points: " + (seconds > 0 ? ToString(seconds) + " s" : std::string("off")));
				return sNextLine;
			}

			AccessPoint *point = access->GetAccessPoint(point_name);
			if (!point)
				return ScriptError("Unknown access point " + point_name);

			if (lower == "default")
				point->SetAutoCloseTime(-1);
			else
			{
				Real seconds = 0;
				if (!IsNumeric(value, seconds) || seconds < 0)
					return ScriptError("Syntax: Access autoclose point, seconds|default");
				point->SetAutoCloseTime((int)(seconds * 1000));
			}
			access->Report("Auto-close for " + point->GetName() + ": " + (point->GetAutoCloseTime() > 0 ? ToString(point->GetAutoCloseTime() / 1000.0) + " s" : std::string("off")));
			return sNextLine;
		}
		if (sub == "lockdown")
		{
			std::string mode = args;
			SetCase(mode, false);
			if (mode != "on" && mode != "off")
				return ScriptError("Syntax: Access lockdown on|off");
			access->Lockdown(mode == "on");
			return sNextLine;
		}

		//subcommands that take comma-separated parameters
		int start = (int)(LineData.size() - args.size());
		int params = (args == "") ? 0 : SplitData(LineData, start);

		if (sub == "present")
		{
			//Access present point, facility, number
			int facility = 0, number = 0;
			if (params != 3 || !IsNumeric(tempdata[1], facility) || !IsNumeric(tempdata[2], number))
				return ScriptError("Syntax: Access present point, facility, number");
			access->Present(tempdata[0], facility, number);
			return sNextLine;
		}
		if (sub == "tap")
		{
			//Access tap point - present the active wallet badge
			if (params != 1)
				return ScriptError("Syntax: Access tap point");
			access->PresentWallet(tempdata[0]);
			return sNextLine;
		}
		if (sub == "addbadge" || sub == "disable" || sub == "enable")
		{
			int facility = 0, number = 0;
			if (params != 2 || !IsNumeric(tempdata[0], facility) || !IsNumeric(tempdata[1], number))
				return ScriptError("Syntax: Access " + sub + " facility, number");

			if (sub == "addbadge")
				access->WalletAdd(facility, number);
			else
			{
				bool enable = (sub == "enable");
				if (access->GetCore()->SetCredentialEnabled(facility, number, enable) == false)
					return ScriptError("Unknown card");
				access->LogEvent("INFO", "Cards", facility, number, enable ? "Enabled" : "Disabled");
			}
			return sNextLine;
		}
		if (sub == "grant" || sub == "unlock" || sub == "lock" || sub == "exit")
		{
			if (params != 1)
				return ScriptError("Syntax: Access " + sub + " point");

			AccessPoint *point = access->GetAccessPoint(tempdata[0]);
			if (!point)
				return ScriptError("Unknown access point " + tempdata[0]);

			if (sub == "grant")
			{
				access->LogEvent("GRANTED", point->GetName(), -1, -1, "Manual");
				point->Grant();
			}
			else if (sub == "exit")
				point->RequestExit();
			else
				point->SetHeldUnlocked(sub == "unlock");
			return sNextLine;
		}

		return ScriptError("Unknown access command: " + sub);
	}

	return sContinue;
}

}
