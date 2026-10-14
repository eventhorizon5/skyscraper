/*
	Scalable Building Simulator - Access Controller and Access Point Classes
	The Skyscraper Project - Version 2.1
	Copyright (C)2004-2026 Ryan Thoryk
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

#include <ctime>
#include <algorithm>
#include <fstream>
#include "globals.h"
#include "sbs.h"
#include "door.h"
#include "floor.h"
#include "camera.h"
#include "elevator.h"
#include "elevatorcar.h"
#include "buttonpanel.h"
#include "callstation.h"
#include "utility.h"
#include "person.h"
#include "controller.h"
#include "cardreader.h"
#include "access.h"

namespace SBS {

//
// AccessPoint
//

AccessPoint::AccessPoint(AccessController *controller, const std::string &name)
{
	this->controller = controller;
	this->name = name;
	unlock_time = 5000;
	held_time = 30000;
	auto_open = false;
	close_time = -1;
	released = false;
	held_unlocked = false;
	opening_internally = false;
	release_until = 0;
}

AccessPoint::~AccessPoint()
{
	//detach from doors, so they revert to their plain DoorLock behavior
	for (size_t i = 0; i < doors.size(); i++)
	{
		if (doors[i].door)
			doors[i].door->SetAccessPoint(0);
	}
	doors.clear();
}

void AccessPoint::AddDoor(Door *door)
{
	if (!door)
		return;

	for (size_t i = 0; i < doors.size(); i++)
	{
		if (doors[i].door == door)
			return;
	}

	DoorState state;
	state.door = door;
	state.was_open = (door->IsOpen() || door->IsMoving);
	state.authorized = state.was_open; //a door that starts open is not an alarm
	state.open_since = 0;
	state.held_alarm = false;
	doors.emplace_back(state);

	door->SetAccessPoint(this);
}

void AccessPoint::RemoveDoor(Door *door)
{
	for (size_t i = 0; i < doors.size(); i++)
	{
		if (doors[i].door == door)
		{
			doors.erase(doors.begin() + i);
			return;
		}
	}
}

void AccessPoint::Grant()
{
	unsigned long now = controller->GetRoot()->GetRunTime();
	released = true;
	release_until = now + unlock_time;

	if (auto_open == true)
	{
		opening_internally = true;
		Vector3 position = controller->GetRoot()->camera->GetPosition();
		for (size_t i = 0; i < doors.size(); i++)
		{
			if (doors[i].door && doors[i].door->IsOpen() == false && doors[i].door->IsMoving == false)
				doors[i].door->Open(position, true, true);
		}
		opening_internally = false;
	}
}

void AccessPoint::RequestExit()
{
	controller->LogEvent("EXIT", name, -1, -1, "RequestToExit");
	Grant();
}

void AccessPoint::SetHeldUnlocked(bool value)
{
	held_unlocked = value;
	controller->LogEvent("INFO", name, -1, -1, value ? "HeldUnlocked" : "Normal");
}

int AccessPoint::GetAutoCloseTime()
{
	if (close_time >= 0)
		return close_time;
	return controller->GetDefaultAutoClose();
}

bool AccessPoint::IsReleased()
{
	if (controller->GetCore()->GetLockdown() == true)
		return released; //during lockdown, only override-card grants release

	return released || held_unlocked;
}

std::string AccessPoint::GetStatus()
{
	std::string status = IsReleased() ? "released" : "locked";

	for (size_t i = 0; i < doors.size(); i++)
	{
		if (doors[i].held_alarm == true)
			return status + ", held open";
		if (doors[i].was_open == true && doors[i].authorized == false)
			return status + ", forced open";
	}
	return status;
}

void AccessPoint::OnDoorOpened(Door *door, bool locked_side, bool forced)
{
	//called when a door begins opening, before it moves

	for (size_t i = 0; i < doors.size(); i++)
	{
		DoorState &state = doors[i];
		if (state.door != door)
			continue;

		bool authorized = true;

		if (opening_internally == false)
		{
			if (forced == true)
			{
				//opened by force (script action, or bypass) while not released
				authorized = IsReleased();
			}
			else if (locked_side == true)
			{
				//the lock check passed on a locked side, so the access point must be released
				authorized = IsReleased();
			}
			//an unlocked side is free egress, so it's authorized
		}

		state.authorized = authorized;
		state.held_alarm = false;
		state.open_since = controller->GetRoot()->GetRunTime();

		if (authorized == false)
			controller->LogEvent("ALARM", name, -1, -1, "ForcedOpen");
		return;
	}
}

void AccessPoint::Update(unsigned long now)
{
	//end the release window
	if (released == true && now >= release_until)
		released = false;

	for (size_t i = 0; i < doors.size(); i++)
	{
		DoorState &state = doors[i];
		if (!state.door)
			continue;

		bool open = (state.door->IsOpen() == true || state.door->IsMoving == true);

		if (open == true && state.open_since == 0)
			state.open_since = now; //door was already open, or opened by a path that didn't notify

		if (open == false && state.was_open == true)
		{
			//door closed; it relocks on its own since the release has ended or will end
			state.authorized = false;
			state.held_alarm = false;
			state.open_since = 0;
		}

		//auto-close: once the door is fully open and the time is up, close it
		//(IsMoving is true while closing, so this only triggers once per opening)
		int auto_close = GetAutoCloseTime();
		if (auto_close > 0 && state.open_since > 0 && state.door->IsOpen() == true && state.door->IsMoving == false)
		{
			if (now - state.open_since >= (unsigned long)auto_close)
			{
				state.door->Close(true);
				open = true; //still moving; treat as open until it finishes closing
			}
		}

		if (open == true && state.held_alarm == false && held_time > 0 && state.open_since > 0)
		{
			if (now - state.open_since >= (unsigned long)held_time && IsReleased() == false)
			{
				state.held_alarm = true;
				controller->LogEvent("ALARM", name, -1, -1, "HeldOpen");
			}
		}

		state.was_open = open;
	}
}

//
// AccessController
//

AccessController::AccessController(Object *parent) : Object(parent)
{
	SetValues("AccessController", "Access Controller", true, false);

	active_badge = -1;
	default_close_time = 0;
	station_unlock_time = 10000;
	next_person_card = 1;

	EnableLoop(true);

	Report("Created");
}

AccessController::~AccessController()
{
	//delete card readers (owned by the controller)
	for (size_t i = 0; i < readers.size(); i++)
	{
		if (readers[i])
		{
			readers[i]->DetachController();
			readers[i]->parent_deleting = true;
			delete readers[i];
		}
	}
	readers.clear();

	for (size_t i = 0; i < points.size(); i++)
		delete points[i];
	points.clear();
}

AccessPoint* AccessController::GetAccessPoint(const std::string &name, bool create)
{
	std::string key = AccessCore::Normalize(name);

	for (size_t i = 0; i < points.size(); i++)
	{
		if (AccessCore::Normalize(points[i]->GetName()) == key)
			return points[i];
	}

	if (create == false)
		return 0;

	AccessPoint *point = new AccessPoint(this, name);
	points.emplace_back(point);
	return point;
}

AccessPoint* AccessController::GetAccessPoint(int index)
{
	if (index < 0 || index >= (int)points.size())
		return 0;
	return points[index];
}

CardReader* AccessController::AddCardReader(Floor *floor, const std::string &access_point, const std::string &grant_sound, const std::string &deny_sound, const std::string &idle_texture, const std::string &grant_texture, const std::string &deny_texture, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset)
{
	//create a card reader; readers are owned by the access controller and parented to the engine,
	//and if a floor is given, voffset is relative to that floor's base

	//make sure the access point exists, so readers can be placed before doors
	GetAccessPoint(access_point, true);

	CardReader *reader = new CardReader(GetRoot(), this, access_point, grant_sound, deny_sound, idle_texture, grant_texture, deny_texture, direction, width, height);

	Real altitude = voffset;
	if (floor)
		altitude += floor->GetBase();

	reader->SetPosition(CenterX, altitude, CenterZ);
	return reader;
}

int AccessController::GetReaderState(const std::string &access_point)
{
	//lock state for a reader's light: 1 = unlocked (green), 0 = locked (red), -1 = unknown

	unsigned long now = GetRoot()->GetRunTime();

	//destination dispatch call station: unlocked while a badge tap has floors released
	int station_floor = 0, station_number = 0;
	if (AccessCore::ParseStationPoint(access_point, station_floor, station_number) == true)
	{
		for (size_t i = 0; i < station_releases.size(); i++)
		{
			StationRelease &rel = station_releases[i];
			if (rel.floor == station_floor && rel.number == station_number)
				return (!rel.released.empty() && now < rel.release_until) ? 1 : 0;
		}
		return 0;
	}

	//elevator: unlocked while a badge tap has floors released, or in a service mode that ignores lockout
	int elevator = 0;
	if (AccessCore::ParseElevatorPoint(access_point, elevator) == true)
	{
		ElevatorSecurity *sec = GetElevatorSecurity(elevator);
		if (!sec)
			return -1;
		if (!sec->released.empty() && now < sec->release_until)
			return 1;
		Elevator *e = GetRoot()->GetElevator(elevator);
		if (e && e->InServiceMode() == true)
			return 1;
		return 0;
	}

	//door access point: unlocked while released by a badge or held unlocked by the controller
	AccessPoint *point = GetAccessPoint(access_point);
	if (!point)
		return -1;
	return point->IsReleased() ? 1 : 0;
}

void AccessController::RegisterReader(CardReader *reader)
{
	readers.emplace_back(reader);
}

void AccessController::UnregisterReader(CardReader *reader)
{
	for (size_t i = 0; i < readers.size(); i++)
	{
		if (readers[i] == reader)
		{
			readers.erase(readers.begin() + i);
			return;
		}
	}
}

void AccessController::GetClock(int &day_of_week, int &minute_of_day)
{
	if (clock_source)
	{
		clock_source(day_of_week, minute_of_day);
		return;
	}

	//fall back to the system clock
	time_t timestamp = time(0);
	struct tm datetime = *localtime(&timestamp);
	day_of_week = datetime.tm_wday;
	minute_of_day = datetime.tm_hour * 60 + datetime.tm_min;
}

AccessResult AccessController::Present(const std::string &access_point, int facility_code, int card_number, CardReader *reader)
{
	//"CallStation F:N" goes to that destination dispatch call station
	int station_floor = 0, station_number = 0;
	if (AccessCore::ParseStationPoint(access_point, station_floor, station_number) == true)
	{
		Floor *floor = GetRoot()->GetFloor(station_floor);
		CallStation *station = floor ? floor->GetCallStation(station_number) : 0;
		if (!station)
		{
			ReportError("Unknown call station " + access_point);
			if (reader)
				reader->ShowResult(false);
			return AccessResult::DeniedNoRule;
		}
		return PresentStation(station, facility_code, card_number, reader);
	}

	//"Elevator N" goes to that elevator's floor lockout
	int elevator = 0;
	if (AccessCore::ParseElevatorPoint(access_point, elevator) == true && GetElevatorSecurity(elevator))
		return PresentElevator(elevator, facility_code, card_number, reader);

	AccessPoint *point = GetAccessPoint(access_point);
	if (!point)
	{
		ReportError("Unknown access point " + access_point);
		if (reader)
			reader->ShowResult(false);
		return AccessResult::DeniedNoRule;
	}

	int day = 0, minute = 0;
	GetClock(day, minute);

	AccessResult result = core.Evaluate(facility_code, card_number, point->GetName(), day, minute);

	//anti-passback for zones this door leads into
	std::vector<Zone*> entry_zones = ZonesForPoint(point->GetName());
	if (result == AccessResult::Granted && CheckPassback(entry_zones, facility_code, card_number, point->GetName()) == true)
		result = AccessResult::DeniedPassback;

	if (result == AccessResult::Granted)
	{
		LogEvent("GRANTED", point->GetName(), facility_code, card_number, "");
		point->Grant();
		NotifyEntryGrant(entry_zones, facility_code, card_number);
	}
	else
		LogEvent("DENIED", point->GetName(), facility_code, card_number, AccessResultName(result));

	if (reader)
		reader->ShowResult(result == AccessResult::Granted);

	return result;
}

AccessResult AccessController::PresentWallet(const std::string &access_point, CardReader *reader)
{
	int facility = 0, card = 0;
	if (GetActiveBadge(facility, card) == false)
	{
		ReportError("No badge in wallet");
		if (reader)
			reader->ShowResult(false);
		return AccessResult::DeniedUnknown;
	}
	return Present(access_point, facility, card, reader);
}

void AccessController::Lockdown(bool value)
{
	core.SetLockdown(value);
	LogEvent("INFO", "Building", -1, -1, value ? "LockdownOn" : "LockdownOff");
}

bool AccessController::WalletAdd(int facility_code, int card_number)
{
	for (size_t i = 0; i < wallet.size(); i++)
	{
		if (wallet[i].facility_code == facility_code && wallet[i].card_number == card_number)
			return false;
	}

	Badge badge;
	badge.facility_code = facility_code;
	badge.card_number = card_number;
	wallet.emplace_back(badge);

	if (active_badge < 0)
		active_badge = 0;

	const Credential *credential = core.GetCredential(facility_code, card_number);
	std::string holder = credential ? credential->holder : "unenrolled";
	Report("Added badge " + ToString(facility_code) + ":" + ToString(card_number) + " (" + holder + ") to wallet");
	return true;
}

bool AccessController::WalletRemove(int facility_code, int card_number)
{
	for (size_t i = 0; i < wallet.size(); i++)
	{
		if (wallet[i].facility_code == facility_code && wallet[i].card_number == card_number)
		{
			wallet.erase(wallet.begin() + i);
			if (active_badge >= (int)wallet.size())
				active_badge = (int)wallet.size() - 1;
			return true;
		}
	}
	return false;
}

bool AccessController::NextBadge()
{
	if (wallet.empty())
		return false;

	active_badge = (active_badge + 1) % (int)wallet.size();

	int facility = 0, card = 0;
	GetActiveBadge(facility, card);
	const Credential *credential = core.GetCredential(facility, card);
	Report("Active badge: " + ToString(facility) + ":" + ToString(card) + (credential ? " (" + credential->holder + ")" : ""));
	return true;
}

bool AccessController::GetActiveBadge(int &facility_code, int &card_number)
{
	if (active_badge < 0 || active_badge >= (int)wallet.size())
		return false;

	facility_code = wallet[active_badge].facility_code;
	card_number = wallet[active_badge].card_number;
	return true;
}

bool AccessController::SetActiveBadge(int facility_code, int card_number)
{
	for (size_t i = 0; i < wallet.size(); i++)
	{
		if (wallet[i].facility_code == facility_code && wallet[i].card_number == card_number)
		{
			active_badge = (int)i;
			return true;
		}
	}
	return false;
}

std::vector<std::pair<int, int> > AccessController::GetWallet()
{
	std::vector<std::pair<int, int> > result;
	for (size_t i = 0; i < wallet.size(); i++)
		result.emplace_back(std::make_pair(wallet[i].facility_code, wallet[i].card_number));
	return result;
}

bool AccessController::ExportScript(const std::string &filename)
{
	//write the access database as an <Access> section, so changes made in the Access Control
	//window can be pasted into the building file (replacing its existing <Access> section)

	std::ofstream file(filename.c_str());
	if (!file.is_open())
		return ReportError("Cannot open " + filename);

	file << "#Access control exported from Skyscraper\n";
	file << "#Replace the building's <Access> section with this one. Zones (AddAccessZone, ZoneEntry),\n";
	file << "#door access points (SetAccessPoint) and readers stay where they are in the building file.\n\n";
	file << "<Access>\n";

	//settings
	if (default_close_time > 0)
		file << "    AutoClose " << ToString(default_close_time / 1000.0) << "\n";
	if (station_unlock_time != 10000)
		file << "    StationUnlock " << ToString(station_unlock_time / 1000.0) << "\n";
	for (size_t i = 0; i < elevator_security.size(); i++)
	{
		ElevatorSecurity &sec = elevator_security[i];
		file << "    SecureFloors " << sec.elevator << ", " << sec.floor_spec << ", " << ToString(sec.unlock_time / 1000.0);
		if (sec.auto_select == true)
			file << ", true";
		file << "\n";
	}

	//time profiles
	file << "\n    #time profiles (schedules)\n";
	std::vector<std::string> schedules = core.GetScheduleNames();
	for (size_t i = 0; i < schedules.size(); i++)
	{
		if (AccessCore::IsBuiltInSchedule(schedules[i]))
			continue;
		const Schedule *schedule = core.GetSchedule(schedules[i]);
		if (!schedule)
			continue;
		if (schedule->windows.empty())
			file << "    #" << schedule->name << " has no time windows, so it never allows access\n";
		for (size_t j = 0; j < schedule->windows.size(); j++)
			file << "    Schedule " << schedule->name << ", " << AccessCore::FormatWindow(schedule->windows[j]) << "\n";
	}

	//access levels and their rules
	file << "\n    #access levels (groups) and their rules\n";
	std::vector<std::string> groups = core.GetGroupNames();
	for (size_t i = 0; i < groups.size(); i++)
	{
		const AccessGroup *group = core.GetGroup(groups[i]);
		if (!group)
			continue;
		file << "    Group " << group->name << (group->override_lockdown ? ", override" : "") << "\n";
		for (size_t j = 0; j < group->rules.size(); j++)
			file << "    GroupRule " << group->name << ", " << group->rules[j].display << ", " << group->rules[j].schedule << "\n";
	}

	//virtual people
	if (!people_groups.empty() || !service_people_groups.empty())
		file << "\n";
	if (!people_groups.empty())
	{
		file << "    PeopleAccess ";
		for (size_t i = 0; i < people_groups.size(); i++)
			file << (i > 0 ? "|" : "") << people_groups[i];
		file << "\n";
	}
	if (!service_people_groups.empty())
	{
		file << "    ServicePeopleAccess ";
		for (size_t i = 0; i < service_people_groups.size(); i++)
			file << (i > 0 ? "|" : "") << service_people_groups[i];
		file << "\n";
	}

	//cards (virtual people's badges are created automatically, so they're left out)
	file << "\n    #cards: facility, number, holder, access levels, home floor\n";
	std::vector<const Credential*> cards = core.GetCredentials();
	for (size_t i = 0; i < cards.size(); i++)
	{
		const Credential *c = cards[i];
		if (c->facility_code == 0 && people_badges.find(c->holder) != people_badges.end())
			continue;

		std::string list;
		for (size_t j = 0; j < c->groups.size(); j++)
			list += (j > 0 ? "|" : "") + c->groups[j];
		file << "    Card " << c->facility_code << ", " << c->card_number << ", " << c->holder << ", " << list;
		if (c->home_floor != "")
			file << ", " << c->home_floor;
		file << "\n";
		if (c->enabled == false)
			file << "    #card " << c->facility_code << ":" << c->card_number << " was disabled; use \"access disable " << c->facility_code << ", " << c->card_number << "\" to disable it again\n";
	}

	file << "\n";
	for (size_t i = 0; i < wallet.size(); i++)
		file << "    WalletAdd " << wallet[i].facility_code << ", " << wallet[i].card_number << "\n";

	file << "<EndAccess>\n";

	Report("Exported access control to " + filename);
	return true;
}

std::vector<std::string> AccessController::GetPlaceNames()
{
	std::vector<std::string> names;
	for (size_t i = 0; i < points.size(); i++)
		names.emplace_back(points[i]->GetName());
	for (size_t i = 0; i < elevator_security.size(); i++)
		names.emplace_back("Elevator " + ToString(elevator_security[i].elevator) + ":Floors " + elevator_security[i].floor_spec);
	return names;
}

AccessResult AccessController::CheckAccess(int facility_code, int card_number, const std::string &place, std::string &detail)
{
	//decide whether a card could get in somewhere right now, without logging or unlocking anything

	detail = "";
	int day = 0, minute = 0;
	GetClock(day, minute);

	//elevator floors: "Elevator N:Floor X" checks one floor; "Elevator N" or a floor list checks every secured floor
	int elevator = 0;
	std::string spec;
	bool elevator_rule = AccessCore::ParseElevatorRule(place, elevator, spec);
	if (elevator_rule == false && AccessCore::ParseElevatorPoint(place, elevator) == true)
		elevator_rule = true;

	if (elevator_rule == true)
	{
		std::vector<int> floors;
		int single = 0;
		if (!spec.empty() && spec.find_first_of("|-~@") == std::string::npos && ResolveFloor(spec, single))
			floors.push_back(single);
		else
		{
			std::vector<int> secured = GetSecuredFloors(elevator);
			for (size_t i = 0; i < secured.size(); i++)
			{
				if (spec.empty() || AccessCore::FloorSpecMatches(spec, secured[i], GetFloorLabels(secured[i])))
					floors.push_back(secured[i]);
			}
		}

		if (floors.empty())
		{
			detail = "no secured floors match";
			return AccessResult::DeniedNoRule;
		}

		std::string allowed, refused;
		AccessResult reason = AccessResult::DeniedNoRule;
		for (size_t i = 0; i < floors.size(); i++)
		{
			AccessResult result = core.EvaluateFloor(facility_code, card_number, elevator, floors[i], GetFloorLabels(floors[i]), day, minute);
			std::string label = GetFloorLabel(floors[i]);
			if (result == AccessResult::Granted)
				allowed += (allowed.empty() ? "" : ",") + label;
			else
			{
				refused += (refused.empty() ? "" : ",") + label + " (" + AccessResultName(result) + ")";
				if (result != AccessResult::DeniedNoRule)
					reason = result;
			}
		}

		if (!allowed.empty())
			detail = "allowed floors: " + allowed;
		if (!refused.empty())
			detail += std::string(detail.empty() ? "" : "; ") + "refused: " + refused;
		return allowed.empty() ? reason : AccessResult::Granted;
	}

	return core.Evaluate(facility_code, card_number, place, day, minute);
}

bool AccessController::IsWalletBadge(int facility_code, int card_number)
{
	for (size_t i = 0; i < wallet.size(); i++)
	{
		if (wallet[i].facility_code == facility_code && wallet[i].card_number == card_number)
			return true;
	}
	return false;
}

void AccessController::ListWallet()
{
	Report("\n--- Wallet ---\n");
	for (size_t i = 0; i < wallet.size(); i++)
	{
		const Credential *credential = core.GetCredential(wallet[i].facility_code, wallet[i].card_number);
		std::string marker = ((int)i == active_badge) ? "* " : "  ";
		Report(marker + ToString(wallet[i].facility_code) + ":" + ToString(wallet[i].card_number) + " - " + (credential ? credential->holder : "unenrolled"));
	}
	Report("");
}

void AccessController::LogEvent(const std::string &kind, const std::string &access_point, int facility_code, int card_number, const std::string &detail)
{
	AccessEvent event;
	event.sim_time_ms = GetRoot()->GetRunTime();

	int day = 0, minute = 0;
	GetClock(day, minute);
	event.clock = AccessCore::FormatClock(minute);

	event.kind = kind;
	event.access_point = access_point;
	event.facility_code = facility_code;
	event.card_number = card_number;
	event.detail = detail;

	if (facility_code >= 0 && card_number >= 0)
	{
		const Credential *credential = core.GetCredential(facility_code, card_number);
		if (credential)
			event.holder = credential->holder;
	}

	core.Log(event);
	Report(core.FormatEvent(event));
}

void AccessController::ShowLog(int count)
{
	const std::deque<AccessEvent> &log = core.GetLog();

	if (count <= 0 || count > (int)log.size())
		count = (int)log.size();

	Report("\n--- Access Log (last " + ToString(count) + ") ---\n");
	for (size_t i = log.size() - count; i < log.size(); i++)
		Report(core.FormatEvent(log[i]));
	Report("");
}

bool AccessController::ExportLog(const std::string &filename)
{
	std::string error;
	if (core.ExportLog(filename, error) == false)
		return ReportError(error);

	Report("Exported " + ToString((int)core.GetLog().size()) + " events to " + filename);
	return true;
}

void AccessController::ListCards()
{
	Report("\n--- Cards ---\n");

	std::vector<const Credential*> cards = core.GetCredentials();
	for (size_t i = 0; i < cards.size(); i++)
	{
		std::string groups;
		for (size_t j = 0; j < cards[i]->groups.size(); j++)
		{
			if (j > 0)
				groups += "|";
			groups += cards[i]->groups[j];
		}
		Report(ToString(cards[i]->facility_code) + ":" + ToString(cards[i]->card_number) + " - " + cards[i]->holder + " [" + groups + "]" + (cards[i]->enabled ? "" : " DISABLED"));
	}
	Report("");
}

void AccessController::ListAccessPoints()
{
	Report("\n--- Access Points ---\n");
	for (size_t i = 0; i < points.size(); i++)
	{
		int auto_close = points[i]->GetAutoCloseTime();
		std::string closer = (auto_close > 0) ? ", auto-close " + ToString(auto_close / 1000.0) + " s" : ", auto-close off";
		Report(points[i]->GetName() + " - doors: " + ToString(points[i]->GetDoorCount()) + ", " + points[i]->GetStatus() + closer);
	}
	Report("");
}

AccessController::ElevatorSecurity* AccessController::GetElevatorSecurity(int elevator)
{
	for (size_t i = 0; i < elevator_security.size(); i++)
	{
		if (elevator_security[i].elevator == elevator)
			return &elevator_security[i];
	}
	return 0;
}

bool AccessController::SecureElevatorFloors(int elevator, const std::string &floor_spec, int unlock_time, bool auto_select)
{
	if (elevator < 1)
		return ReportError("Invalid elevator number");
	if (AccessCore::IsValidFloorSpec(floor_spec) == false)
		return ReportError("Invalid floor list: " + floor_spec);

	ElevatorSecurity *sec = GetElevatorSecurity(elevator);
	if (!sec)
	{
		ElevatorSecurity entry;
		entry.elevator = elevator;
		elevator_security.emplace_back(entry);
		sec = &elevator_security.back();
	}

	//repeated SecureFloors lines add to the list
	if (sec->floor_spec.empty())
		sec->floor_spec = floor_spec;
	else
		sec->floor_spec += "|" + floor_spec;

	sec->unlock_time = unlock_time;
	sec->auto_select = auto_select;
	return true;
}

std::vector<std::string> AccessController::GetFloorLabels(int floor)
{
	std::vector<std::string> labels;
	Floor *f = GetRoot()->GetFloor(floor);
	if (f)
	{
		labels.emplace_back(f->ID);
		if (f->NumberID != f->ID)
			labels.emplace_back(f->NumberID);
	}
	return labels;
}

std::string AccessController::GetFloorLabel(int floor)
{
	Floor *f = GetRoot()->GetFloor(floor);
	if (f && f->ID != "")
		return f->ID;
	return "@" + ToString(floor);
}

bool AccessController::ResolveFloor(const std::string &label, int &floor)
{
	std::string value = label;
	TrimString(value);
	if (value.empty())
		return false;

	//@N is an internal floor number
	if (value[0] == '@')
	{
		int number = 0;
		if (!IsNumeric(value.substr(1), number))
			return false;
		if (!GetRoot()->GetFloor(number))
			return false;
		floor = number;
		return true;
	}

	return GetRoot()->GetUtility()->GetFloorFromID(value, floor);
}

bool AccessController::IsFloorSecured(int elevator, int floor)
{
	ElevatorSecurity *sec = GetElevatorSecurity(elevator);
	if (!sec || sec->floor_spec.empty())
		return false;
	return AccessCore::FloorSpecMatches(sec->floor_spec, floor, GetFloorLabels(floor));
}

std::vector<int> AccessController::GetSecuredFloors(int elevator)
{
	//resolve the secured floor list against the building's floors (done on use,
	//so the Access section can come before the floors are created)

	std::vector<int> floors;
	ElevatorSecurity *sec = GetElevatorSecurity(elevator);
	if (!sec || sec->floor_spec.empty())
		return floors;

	for (int floor = -GetRoot()->Basements; floor < GetRoot()->Floors; floor++)
	{
		if (AccessCore::FloorSpecMatches(sec->floor_spec, floor, GetFloorLabels(floor)))
			floors.emplace_back(floor);
	}
	return floors;
}

bool AccessController::CheckElevatorFloor(int elevator, int floor)
{
	//returns true if a floor selection is allowed; unsecured floors are always allowed,
	//and secured floors need a current badge release. A release is used up by one selection.

	ElevatorSecurity *sec = GetElevatorSecurity(elevator);
	if (!sec)
		return true;

	if (IsFloorSecured(elevator, floor) == false)
		return true;

	unsigned long now = GetRoot()->GetRunTime();
	if (now < sec->release_until && std::find(sec->released.begin(), sec->released.end(), floor) != sec->released.end())
	{
		sec->released.clear();
		NotifyEntryGrant(ZonesForFloor(floor), sec->card_facility, sec->card_number);
		LogEvent("INFO", "Elevator " + ToString(elevator), -1, -1, "FloorSelected " + GetFloorLabel(floor));
		return true;
	}

	FlashElevatorReaders(elevator, false);
	return false;
}

AccessResult AccessController::PresentElevator(int elevator, int facility_code, int card_number, CardReader *reader, bool allow_autoselect)
{
	ElevatorSecurity *sec = GetElevatorSecurity(elevator);
	std::string name = "Elevator " + ToString(elevator);

	std::vector<int> secured = GetSecuredFloors(elevator);
	if (!sec || secured.empty())
	{
		ReportError(name + " has no secured floors (use SecureFloors in the Access section)");
		if (reader)
			reader->ShowResult(false);
		return AccessResult::DeniedNoRule;
	}

	int day = 0, minute = 0;
	GetClock(day, minute);

	//check every secured floor; remember the most useful deny reason
	std::vector<int> allowed;
	AccessResult reason = AccessResult::DeniedNoRule;
	for (size_t i = 0; i < secured.size(); i++)
	{
		AccessResult result = core.EvaluateFloor(facility_code, card_number, elevator, secured[i], GetFloorLabels(secured[i]), day, minute);
		if (result == AccessResult::Granted)
			allowed.emplace_back(secured[i]);
		else if (result != AccessResult::DeniedNoRule)
			reason = result; //unknown, disabled, lockdown or schedule beat "no rule"
	}

	//anti-passback: drop floors leading into a zone this card is already inside
	for (size_t i = 0; i < allowed.size();)
	{
		if (CheckPassback(ZonesForFloor(allowed[i]), facility_code, card_number, name + " Floor " + GetFloorLabel(allowed[i])) == true)
		{
			allowed.erase(allowed.begin() + i);
			reason = AccessResult::DeniedPassback;
		}
		else
			i++;
	}

	if (allowed.empty())
	{
		LogEvent("DENIED", name, facility_code, card_number, AccessResultName(reason));
		if (reader)
			reader->ShowResult(false);
		else
			FlashElevatorReaders(elevator, false);
		return reason;
	}

	sec->released = allowed;
	sec->release_until = GetRoot()->GetRunTime() + sec->unlock_time;
	sec->card_facility = facility_code;
	sec->card_number = card_number;

	std::string floors;
	for (size_t i = 0; i < allowed.size(); i++)
	{
		if (i > 0)
			floors += ",";
		floors += GetFloorLabel(allowed[i]);
	}
	LogEvent("GRANTED", name, facility_code, card_number, "Floors " + floors);

	if (reader)
		reader->ShowResult(true);
	else
		FlashElevatorReaders(elevator, true);

	//auto-select when the badge allows exactly one secured floor
	if (sec->auto_select == true && allow_autoselect == true && allowed.size() == 1)
	{
		Elevator *e = GetRoot()->GetElevator(elevator);
		if (e)
			e->SelectFloor(allowed[0]);
	}

	return AccessResult::Granted;
}

void AccessController::FlashElevatorReaders(int elevator, bool granted)
{
	//show a result on every reader in this elevator's cars
	FlashReaders("Elevator " + ToString(elevator), granted);
}

void AccessController::FlashReaders(const std::string &access_point, bool granted)
{
	std::string name = AccessCore::Normalize(access_point);
	for (size_t i = 0; i < readers.size(); i++)
	{
		if (readers[i] && AccessCore::Normalize(readers[i]->GetAccessPointName()) == name)
			readers[i]->ShowResult(granted);
	}
}

AccessController::StationRelease* AccessController::GetStationRelease(CallStation *station, bool create)
{
	int floor = station->GetFloor();
	for (size_t i = 0; i < station_releases.size(); i++)
	{
		if (station_releases[i].floor == floor && station_releases[i].number == station->Number)
			return &station_releases[i];
	}
	if (create == false)
		return 0;

	StationRelease entry;
	entry.floor = floor;
	entry.number = station->Number;
	station_releases.emplace_back(entry);
	return &station_releases.back();
}

bool AccessController::IsStationFloorSecured(CallStation *station, int floor)
{
	//a destination is secured at a call station if it's a secured floor
	//on any elevator that the station's dispatch controller uses

	DispatchController *controller = station ? station->GetController() : 0;
	if (!controller)
		return false;

	for (int i = 0; i < controller->GetElevatorCount(); i++)
	{
		if (IsFloorSecured(controller->GetElevator(i), floor))
			return true;
	}
	return false;
}

bool AccessController::CheckStationFloor(CallStation *station, int floor)
{
	//returns true if a destination selection is allowed at this station; a badge release
	//is used up by one selection

	if (!station || IsStationFloorSecured(station, floor) == false)
		return true;

	std::string name = AccessCore::StationPoint(station->GetFloor(), station->Number);
	StationRelease *rel = GetStationRelease(station, false);
	unsigned long now = GetRoot()->GetRunTime();

	if (rel && now < rel->release_until && std::find(rel->released.begin(), rel->released.end(), floor) != rel->released.end())
	{
		rel->released.clear();
		NotifyEntryGrant(ZonesForFloor(floor), rel->card_facility, rel->card_number);
		LogEvent("INFO", name, -1, -1, "FloorSelected " + GetFloorLabel(floor));
		return true;
	}

	LogEvent("DENIED", name, -1, -1, "BadgeRequired Floor " + GetFloorLabel(floor));
	FlashReaders(name, false);
	return false;
}

AccessResult AccessController::PresentStation(CallStation *station, int facility_code, int card_number, CardReader *reader, bool use_home_floor)
{
	//badge tap at a destination dispatch call station:
	//  - if the card has a default (home) floor it may use, call a car there right away
	//  - otherwise release the card's secured floors so one can be entered on the keypad

	std::string name = AccessCore::StationPoint(station->GetFloor(), station->Number);

	const Credential *credential = core.GetCredential(facility_code, card_number);
	AccessResult reason = AccessResult::Granted;
	if (!credential)
		reason = AccessResult::DeniedUnknown;
	else if (credential->enabled == false)
		reason = AccessResult::DeniedDisabled;

	if (reason != AccessResult::Granted)
	{
		LogEvent("DENIED", name, facility_code, card_number, AccessResultName(reason));
		if (reader)
			reader->ShowResult(false);
		station->Error(0);
		return reason;
	}

	DispatchController *controller = station->GetController();
	if (!controller)
	{
		ReportError(name + " has no dispatch controller");
		if (reader)
			reader->ShowResult(false);
		return AccessResult::DeniedNoRule;
	}

	int day = 0, minute = 0;
	GetClock(day, minute);

	//collect secured floors this card may travel to from this station
	std::vector<int> allowed;
	AccessResult deny_reason = AccessResult::DeniedNoRule;
	for (int i = 0; i < controller->GetElevatorCount(); i++)
	{
		int elevator = controller->GetElevator(i);
		std::vector<int> secured = GetSecuredFloors(elevator);

		for (size_t j = 0; j < secured.size(); j++)
		{
			int floor = secured[j];
			if (std::find(allowed.begin(), allowed.end(), floor) != allowed.end())
				continue;

			AccessResult result = core.EvaluateFloor(facility_code, card_number, elevator, floor, GetFloorLabels(floor), day, minute);
			if (result == AccessResult::Granted)
				allowed.emplace_back(floor);
			else if (result != AccessResult::DeniedNoRule)
				deny_reason = result;
		}
	}
	std::sort(allowed.begin(), allowed.end());

	//anti-passback: drop floors leading into a zone this card is already inside
	for (size_t i = 0; i < allowed.size();)
	{
		if (CheckPassback(ZonesForFloor(allowed[i]), facility_code, card_number, name + " Floor " + GetFloorLabel(allowed[i])) == true)
		{
			allowed.erase(allowed.begin() + i);
			deny_reason = AccessResult::DeniedPassback;
		}
		else
			i++;
	}

	//home floor: call a car there right away, if it's reachable and allowed
	int home = 0;
	bool has_home = (credential->home_floor != "" && ResolveFloor(credential->home_floor, home) == true);
	if (credential->home_floor != "" && has_home == false)
		ReportError("Home floor '" + credential->home_floor + "' for card " + ToString(facility_code) + ":" + ToString(card_number) + " doesn't match a floor");
	if (use_home_floor == true && has_home == true && home != station->GetFloor() && controller->ServesFloor(home))
	{
		bool secured = IsStationFloorSecured(station, home);
		bool permitted = !secured || std::find(allowed.begin(), allowed.end(), home) != allowed.end();
		if (permitted == true && secured == false && CheckPassback(ZonesForFloor(home), facility_code, card_number, name + " Floor " + GetFloorLabel(home)) == true)
			permitted = false;

		if (permitted == true)
		{
			StationRelease *rel = GetStationRelease(station, true);
			rel->released.assign(1, home);
			rel->release_until = GetRoot()->GetRunTime() + station_unlock_time;
			rel->card_facility = facility_code;
			rel->card_number = card_number;
			if (secured == false)
				NotifyEntryGrant(ZonesForFloor(home), facility_code, card_number); //secured floors notify on selection

			LogEvent("GRANTED", name, facility_code, card_number, "HomeFloor " + GetFloorLabel(home));
			if (reader)
				reader->ShowResult(true);
			else
				FlashReaders(name, true);

			station->SelectFloor(home);
			return AccessResult::Granted;
		}
	}

	if (allowed.empty())
	{
		LogEvent("DENIED", name, facility_code, card_number, AccessResultName(deny_reason));
		if (reader)
			reader->ShowResult(false);
		else
			FlashReaders(name, false);
		station->Error(0);
		return deny_reason;
	}

	//no usable home floor: release the allowed secured floors for keypad entry
	StationRelease *rel = GetStationRelease(station, true);
	rel->released = allowed;
	rel->release_until = GetRoot()->GetRunTime() + station_unlock_time;
	rel->card_facility = facility_code;
	rel->card_number = card_number;

	std::string floors;
	for (size_t i = 0; i < allowed.size(); i++)
	{
		if (i > 0)
			floors += ",";
		floors += GetFloorLabel(allowed[i]);
	}
	LogEvent("GRANTED", name, facility_code, card_number, "Floors " + floors);
	if (reader)
		reader->ShowResult(true);
	else
		FlashReaders(name, true);

	return AccessResult::Granted;
}

CardReader* AccessController::AddCabReader(ElevatorCar *car, const std::string &grant_sound, const std::string &deny_sound, const std::string &idle_texture, const std::string &grant_texture, const std::string &deny_texture, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset)
{
	//create a card reader inside an elevator car; it moves with the car, and its
	//position is relative to the car, like other car controls

	if (!car || !car->GetElevator())
		return 0;

	std::string name = "Elevator " + ToString(car->GetElevator()->Number);
	CardReader *reader = new CardReader(car, this, name, grant_sound, deny_sound, idle_texture, grant_texture, deny_texture, direction, width, height);
	reader->Move(CenterX, voffset, CenterZ);
	return reader;
}

CardReader* AccessController::AddPanelReader(ButtonPanel *panel, const std::string &grant_sound, const std::string &deny_sound, const std::string &idle_texture, const std::string &grant_texture, const std::string &deny_texture, int row, int column, Real bwidth, Real bheight, Real hoffset, Real voffset)
{
	//create a card reader mounted on an elevator button panel, placed by row and column
	//like a button; it takes the panel's facing direction and moves and hides with the panel

	if (!panel)
		return 0;

	//readers on car panels serve that elevator; readers on call station panels serve that station
	std::string name;
	ElevatorCar *car = dynamic_cast<ElevatorCar*>(panel->GetParent());
	CallStation *station = dynamic_cast<CallStation*>(panel->GetParent());
	if (car && car->GetElevator())
		name = "Elevator " + ToString(car->GetElevator()->Number);
	else if (station)
		name = AccessCore::StationPoint(station->GetFloor(), station->Number);
	else
	{
		ReportError("Panel card readers are only supported on elevator car and call station panels");
		return 0;
	}

	if (row < 1 || row > panel->Rows || column < 1 || column > panel->Columns)
	{
		ReportError("Panel position out of range: row " + ToString(row) + ", column " + ToString(column));
		return 0;
	}

	if (bwidth == 0)
		bwidth = 1;
	if (bheight == 0)
		bheight = 1;

	CardReader *reader = new CardReader(panel, this, name, grant_sound, deny_sound, idle_texture, grant_texture, deny_texture, panel->Direction, panel->ButtonWidth * bwidth, panel->ButtonHeight * bheight);
	reader->Move(panel->GetGridPosition(row, column, bwidth, bheight, hoffset, voffset));

	if (panel->IsEnabled() == false)
		reader->Enabled(false);

	return reader;
}

std::vector<std::string> AccessController::GetElevatorStatus()
{
	std::vector<std::string> lines;
	unsigned long now = GetRoot()->GetRunTime();
	for (size_t i = 0; i < elevator_security.size(); i++)
	{
		ElevatorSecurity &sec = elevator_security[i];
		std::vector<int> secured = GetSecuredFloors(sec.elevator);
		std::string floors;
		for (size_t j = 0; j < secured.size(); j++)
		{
			if (j > 0)
				floors += ",";
			floors += GetFloorLabel(secured[j]);
		}
		if (floors == "")
			floors = "none match '" + sec.floor_spec + "'";
		std::string state = (!sec.released.empty() && now < sec.release_until) ? ", released" : "";
		lines.emplace_back("Elevator " + ToString(sec.elevator) + " - secured floors (labels): " + floors + ", unlock " + ToString(sec.unlock_time / 1000.0) + " s" + (sec.auto_select ? ", auto-select" : "") + state);
	}
	return lines;
}

void AccessController::ListElevators()
{
	Report("\n--- Secured Elevators ---\n");
	std::vector<std::string> lines = GetElevatorStatus();
	for (size_t i = 0; i < lines.size(); i++)
		Report(lines[i]);
	Report("");
}

bool AccessController::IsElevatorFloorReleased(int elevator, int floor)
{
	ElevatorSecurity *sec = GetElevatorSecurity(elevator);
	if (!sec)
		return false;
	return GetRoot()->GetRunTime() < sec->release_until && std::find(sec->released.begin(), sec->released.end(), floor) != sec->released.end();
}

bool AccessController::IsStationFloorReleased(CallStation *station, int floor)
{
	StationRelease *rel = GetStationRelease(station, false);
	if (!rel)
		return false;
	return GetRoot()->GetRunTime() < rel->release_until && std::find(rel->released.begin(), rel->released.end(), floor) != rel->released.end();
}

//
// secured zones
//

bool AccessController::AddZone(const std::string &name, const Vector3 &min, const Vector3 &max, int passback, const std::string &alarm_on, const std::string &alarm_off)
{
	std::string key = AccessCore::Normalize(name);
	for (size_t i = 0; i < zones.size(); i++)
	{
		if (AccessCore::Normalize(zones[i].name) == key)
			return ReportError("Zone " + name + " already exists");
	}

	Zone zone;
	zone.name = name;
	zone.min = Vector3(std::min(min.x, max.x), std::min(min.y, max.y), std::min(min.z, max.z));
	zone.max = Vector3(std::max(min.x, max.x), std::max(min.y, max.y), std::max(min.z, max.z));
	zone.passback = passback;
	zone.alarm_on = alarm_on;
	zone.alarm_off = alarm_off;
	zones.emplace_back(zone);
	return true;
}

bool AccessController::AddZoneEntry(const std::string &zone_name, const std::string &entry)
{
	//an entry is a door access point name, or "Floor <labels>" for elevator and call station arrivals

	std::string key = AccessCore::Normalize(zone_name);
	for (size_t i = 0; i < zones.size(); i++)
	{
		if (AccessCore::Normalize(zones[i].name) != key)
			continue;

		//"Floor 6" / "Floors 6-9" (a space after the word) is a floor entry; anything else is a door name
		std::string raw = entry;
		TrimString(raw);
		SetCase(raw, false);
		std::string value = AccessCore::Normalize(entry);
		if (raw.compare(0, 6, "floor ") == 0 || raw.compare(0, 7, "floors ") == 0)
		{
			std::string spec = value.substr(value.compare(0, 6, "floors") == 0 ? 6 : 5);
			if (spec.empty() || AccessCore::IsValidFloorSpec(spec) == false)
				return ReportError("Invalid floor list in zone entry: " + entry);
			zones[i].floor_entries.emplace_back(spec);
		}
		else
			zones[i].door_entries.emplace_back(value);
		return true;
	}
	return ReportError("Unknown zone " + zone_name);
}

std::vector<AccessController::Zone*> AccessController::ZonesForPoint(const std::string &access_point)
{
	std::vector<Zone*> list;
	std::string key = AccessCore::Normalize(access_point);
	for (size_t i = 0; i < zones.size(); i++)
	{
		if (std::find(zones[i].door_entries.begin(), zones[i].door_entries.end(), key) != zones[i].door_entries.end())
			list.emplace_back(&zones[i]);
	}
	return list;
}

std::vector<AccessController::Zone*> AccessController::ZonesForFloor(int floor)
{
	std::vector<Zone*> list;
	std::vector<std::string> labels = GetFloorLabels(floor);
	for (size_t i = 0; i < zones.size(); i++)
	{
		for (size_t j = 0; j < zones[i].floor_entries.size(); j++)
		{
			if (AccessCore::FloorSpecMatches(zones[i].floor_entries[j], floor, labels))
			{
				list.emplace_back(&zones[i]);
				break;
			}
		}
	}
	return list;
}

bool AccessController::CheckPassback(const std::vector<Zone*> &list, int facility_code, int card_number, const std::string &where)
{
	//returns true if entry must be blocked: the card is already inside (or was just granted into) a zone
	//with hard anti-passback. Soft anti-passback only logs.

	if (facility_code < 0)
		return false;

	//override groups are exempt
	const Credential *credential = core.GetCredential(facility_code, card_number);
	if (credential)
	{
		for (size_t i = 0; i < credential->groups.size(); i++)
		{
			const AccessGroup *group = core.GetGroup(credential->groups[i]);
			if (group && group->override_lockdown == true)
				return false;
		}
	}

	bool block = false;
	for (size_t i = 0; i < list.size(); i++)
	{
		Zone *zone = list[i];
		if (zone->passback == PassbackOff)
			continue;

		for (size_t j = 0; j < zone->cards.size(); j++)
		{
			if (zone->cards[j].facility_code == facility_code && zone->cards[j].card_number == card_number)
			{
				LogEvent("ALARM", zone->name, facility_code, card_number, std::string(zone->passback == PassbackHard ? "PassbackDenied at " : "Passback at ") + where);
				if (zone->passback == PassbackHard)
					block = true;
				break;
			}
		}
	}
	return block;
}

void AccessController::NotifyEntryGrant(const std::vector<Zone*> &list, int facility_code, int card_number)
{
	//a badge was granted at an entry to these zones; the holder is expected to walk in shortly

	if (facility_code < 0)
		return;

	unsigned long expires = GetRoot()->GetRunTime() + 60000;

	for (size_t i = 0; i < list.size(); i++)
	{
		Zone *zone = list[i];
		bool found = false;
		for (size_t j = 0; j < zone->cards.size(); j++)
		{
			if (zone->cards[j].facility_code == facility_code && zone->cards[j].card_number == card_number)
			{
				if (zone->cards[j].confirmed == false)
					zone->cards[j].expires = expires;
				found = true;
				break;
			}
		}
		if (found == false)
		{
			ZoneCard card;
			card.facility_code = facility_code;
			card.card_number = card_number;
			card.confirmed = false;
			card.expires = expires;
			zone->cards.emplace_back(card);
		}
	}
}

void AccessController::SetZoneAlarm(Zone &zone, bool value, const std::string &reason)
{
	if (zone.alarm == value)
		return;

	zone.alarm = value;
	if (value == true)
	{
		LogEvent("ALARM", zone.name, -1, -1, reason);
		if (zone.alarm_on != "")
			GetRoot()->RunAction(zone.alarm_on);
	}
	else
	{
		LogEvent("INFO", zone.name, -1, -1, "AlarmCleared " + reason);
		if (zone.alarm_off != "")
			GetRoot()->RunAction(zone.alarm_off);
	}
}

void AccessController::UpdateZones(unsigned long now)
{
	if (zones.empty())
		return;

	//the player counts as inside a zone when standing in its area, but not while
	//riding an elevator or in a stairwell that passes through it
	Vector3 position = GetRoot()->camera->GetPosition();
	bool in_transit = (GetRoot()->InElevator == true || GetRoot()->InStairwell == true);

	for (size_t i = 0; i < zones.size(); i++)
	{
		Zone &zone = zones[i];

		//unconfirmed entry grants lapse (the holder never walked in)
		for (size_t j = 0; j < zone.cards.size();)
		{
			if (zone.cards[j].confirmed == false && now >= zone.cards[j].expires)
				zone.cards.erase(zone.cards.begin() + j);
			else
				j++;
		}

		bool inside = (in_transit == false &&
			position.x >= zone.min.x && position.x <= zone.max.x &&
			position.y >= zone.min.y && position.y <= zone.max.y &&
			position.z >= zone.min.z && position.z <= zone.max.z);

		if (inside == zone.player_inside)
			continue;

		zone.player_inside = inside;

		if (inside == true)
		{
			//entering: match the most recent unconfirmed grant for a badge the player carries
			//(grants for virtual people's badges don't cover the player)
			ZoneCard *best = 0;
			for (size_t j = 0; j < zone.cards.size(); j++)
			{
				if (zone.cards[j].confirmed == true || IsWalletBadge(zone.cards[j].facility_code, zone.cards[j].card_number) == false)
					continue;
				if (!best || zone.cards[j].expires > best->expires)
					best = &zone.cards[j];
			}

			zone.entries++;
			if (best)
			{
				best->confirmed = true;
				zone.occupant_facility = best->facility_code;
				zone.occupant_number = best->card_number;
				LogEvent("ENTER", zone.name, best->facility_code, best->card_number, "");
			}
			else
			{
				zone.occupant_facility = -1;
				zone.occupant_number = -1;
				LogEvent("ENTER", zone.name, -1, -1, "NoBadge");
				SetZoneAlarm(zone, true, "UnbadgedEntry");
			}
		}
		else
		{
			//leaving: the card is out, for anti-passback
			if (zone.occupant_facility >= 0)
			{
				for (size_t j = 0; j < zone.cards.size(); j++)
				{
					if (zone.cards[j].facility_code == zone.occupant_facility && zone.cards[j].card_number == zone.occupant_number)
					{
						zone.cards.erase(zone.cards.begin() + j);
						break;
					}
				}
			}
			LogEvent("EXIT", zone.name, zone.occupant_facility, zone.occupant_number, "");
			zone.occupant_facility = -1;
			zone.occupant_number = -1;

			//an unbadged entry alarm clears when the intruder leaves
			SetZoneAlarm(zone, false, "AreaVacated");
		}
	}
}

bool AccessController::ResetZone(const std::string &zone_name)
{
	std::string key = AccessCore::Normalize(zone_name);
	for (size_t i = 0; i < zones.size(); i++)
	{
		if (AccessCore::Normalize(zones[i].name) != key && key != "all")
			continue;

		zones[i].cards.clear();
		if (zones[i].player_inside == true && zones[i].occupant_facility >= 0)
		{
			//keep the player's own card as inside
			ZoneCard card;
			card.facility_code = zones[i].occupant_facility;
			card.card_number = zones[i].occupant_number;
			card.confirmed = true;
			zones[i].cards.emplace_back(card);
		}
		SetZoneAlarm(zones[i], false, "Reset");
		LogEvent("INFO", zones[i].name, -1, -1, "PassbackReset");
		if (key != "all")
			return true;
	}
	if (key == "all")
		return true;
	return ReportError("Unknown zone " + zone_name);
}

std::vector<std::string> AccessController::GetZoneStatus()
{
	std::vector<std::string> lines;
	for (size_t i = 0; i < zones.size(); i++)
	{
		Zone &zone = zones[i];
		int inside = 0;
		for (size_t j = 0; j < zone.cards.size(); j++)
		{
			if (zone.cards[j].confirmed == true)
				inside++;
		}
		if (zone.player_inside == true && zone.occupant_facility < 0)
			inside++; //unbadged player

		std::string mode = zone.passback == PassbackHard ? "hard" : (zone.passback == PassbackSoft ? "soft" : "off");
		std::string occupant = "no";
		if (zone.player_inside == true)
			occupant = zone.occupant_facility >= 0 ? "yes (" + ToString(zone.occupant_facility) + ":" + ToString(zone.occupant_number) + ")" : "yes (no badge)";
		lines.emplace_back(zone.name + " - occupancy " + ToString(inside) + ", you inside: " + occupant + ", anti-passback " + mode + ", entries " + ToString(zone.entries) + (zone.alarm ? ", ALARM" : ""));
	}
	return lines;
}

std::vector<std::string> AccessController::GetZoneNames()
{
	std::vector<std::string> names;
	for (size_t i = 0; i < zones.size(); i++)
		names.emplace_back(zones[i].name);
	return names;
}

void AccessController::ListZones()
{
	Report("\n--- Zones ---\n");
	std::vector<std::string> lines = GetZoneStatus();
	for (size_t i = 0; i < lines.size(); i++)
		Report(lines[i]);
	Report("");
}

//
// virtual people
//

void AccessController::SetPeopleGroups(const std::vector<std::string> &groups, bool service)
{
	if (service == true)
		service_people_groups = groups;
	else
		people_groups = groups;
}

bool AccessController::GetPersonCredential(Person *person, int &facility_code, int &card_number)
{
	//virtual people get a badge on first use, in the groups set by PeopleAccess
	//(or ServicePeopleAccess for service people); facility code 0, numbers from 1 up

	if (!person)
		return false;

	std::map<std::string, Badge>::iterator it = people_badges.find(person->GetName());
	if (it != people_badges.end())
	{
		facility_code = it->second.facility_code;
		card_number = it->second.card_number;
		return true;
	}

	const std::vector<std::string> &groups = person->GetServiceAccess() ? service_people_groups : people_groups;
	if (groups.empty() || next_person_card > 65535)
		return false;

	Credential credential;
	credential.facility_code = 0;
	credential.card_number = next_person_card++;
	credential.holder = person->GetName();
	credential.groups = groups;

	std::string error;
	if (core.AddCredential(credential, error) == false)
		return ReportError("Badge for " + person->GetName() + ": " + error);

	Badge badge;
	badge.facility_code = credential.facility_code;
	badge.card_number = credential.card_number;
	people_badges[person->GetName()] = badge;

	facility_code = badge.facility_code;
	card_number = badge.card_number;
	return true;
}

bool AccessController::PersonRequestElevatorFloor(Person *person, int elevator, int floor)
{
	//returns true if the person may select this floor in the car, tapping their badge if needed

	if (IsFloorSecured(elevator, floor) == false)
		return true;
	Elevator *e = GetRoot()->GetElevator(elevator);
	if (e && e->InServiceMode() == true)
		return true;

	int facility = 0, card = 0;
	if (GetPersonCredential(person, facility, card) == false)
		return false;

	PresentElevator(elevator, facility, card, 0, false);
	return IsElevatorFloorReleased(elevator, floor);
}

bool AccessController::PersonRequestStationFloor(Person *person, CallStation *station, int floor)
{
	//returns true if the person may enter this destination at the call station, tapping their badge if needed

	if (IsStationFloorSecured(station, floor) == false)
		return true;

	int facility = 0, card = 0;
	if (GetPersonCredential(person, facility, card) == false)
		return false;

	PresentStation(station, facility, card, 0, false);
	return IsStationFloorReleased(station, floor);
}

bool AccessController::Loop()
{
	unsigned long now = GetRoot()->GetRunTime();

	for (size_t i = 0; i < points.size(); i++)
		points[i]->Update(now);

	for (size_t i = 0; i < readers.size(); i++)
	{
		if (readers[i])
			readers[i]->Update(now);
	}

	//end elevator floor releases
	for (size_t i = 0; i < elevator_security.size(); i++)
	{
		ElevatorSecurity &sec = elevator_security[i];
		if (!sec.released.empty() && now >= sec.release_until)
			sec.released.clear();
	}

	for (size_t i = 0; i < station_releases.size(); i++)
	{
		StationRelease &rel = station_releases[i];
		if (!rel.released.empty() && now >= rel.release_until)
			rel.released.clear();
	}

	UpdateZones(now);

	return true;
}

void AccessController::Report(const std::string &message)
{
	Object::Report("Access: " + message);
}

bool AccessController::ReportError(const std::string &message)
{
	return Object::ReportError("Access: " + message);
}

}
