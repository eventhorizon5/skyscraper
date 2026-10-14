/*
	Scalable Building Simulator - Access Control Core
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

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include "accesscore.h"

namespace SBS {

const char* AccessResultName(AccessResult result)
{
	switch (result)
	{
		case AccessResult::Granted: return "Granted";
		case AccessResult::DeniedUnknown: return "UnknownCard";
		case AccessResult::DeniedDisabled: return "Disabled";
		case AccessResult::DeniedNoRule: return "NoRule";
		case AccessResult::DeniedSchedule: return "Schedule";
		case AccessResult::DeniedLockdown: return "Lockdown";
		case AccessResult::DeniedPassback: return "Passback";
	}
	return "Unknown";
}

bool Schedule::Contains(int day_of_week, int minute_of_day) const
{
	//returns true if the given day (0 = Sunday) and minute fall inside any window

	for (size_t i = 0; i < windows.size(); i++)
	{
		const TimeWindow &w = windows[i];

		if (w.end_minute > w.start_minute)
		{
			//normal window within one day
			if ((w.days_mask & (1 << day_of_week)) && minute_of_day >= w.start_minute && minute_of_day < w.end_minute)
				return true;
		}
		else if (w.end_minute < w.start_minute)
		{
			//window crosses midnight; the start day owns the evening part,
			//and the following day owns the morning part
			if ((w.days_mask & (1 << day_of_week)) && minute_of_day >= w.start_minute)
				return true;
			int previous_day = (day_of_week + 6) % 7;
			if ((w.days_mask & (1 << previous_day)) && minute_of_day < w.end_minute)
				return true;
		}
		//start == end is treated as an empty window
	}
	return false;
}

AccessCore::AccessCore()
{
	log_capacity = 5000;
	log_total = 0;
	version = 0;
	lockdown = false;

	//built-in schedules
	std::string error;
	AddScheduleWindow("Always", 0x7F, 0, 1440, error);
	schedules[Normalize("Never")].name = "Never";
}

uint32_t AccessCore::CardKey(int facility_code, int card_number)
{
	return ((uint32_t)(facility_code & 0xFFFF) << 16) | (uint32_t)(card_number & 0xFFFF);
}

std::string AccessCore::Normalize(const std::string &name)
{
	//lowercase and remove whitespace, matching how SBS::GetObject() compares names
	std::string result;
	result.reserve(name.size());
	for (size_t i = 0; i < name.size(); i++)
	{
		unsigned char c = (unsigned char)name[i];
		if (std::isspace(c))
			continue;
		result += (char)std::tolower(c);
	}
	return result;
}

bool AccessCore::AddScheduleWindow(const std::string &name, int days_mask, int start_minute, int end_minute, std::string &error)
{
	if (name.empty())
	{
		error = "Schedule name is empty";
		return false;
	}
	if (days_mask <= 0 || days_mask > 0x7F)
	{
		error = "Invalid days";
		return false;
	}
	if (start_minute < 0 || start_minute > 1439 || end_minute < 0 || end_minute > 1440)
	{
		error = "Invalid time";
		return false;
	}

	Schedule &schedule = schedules[Normalize(name)];
	if (schedule.name.empty())
		schedule.name = name;

	TimeWindow window;
	window.days_mask = days_mask;
	window.start_minute = start_minute;
	window.end_minute = end_minute;
	schedule.windows.emplace_back(window);
	version++;
	return true;
}

const Schedule* AccessCore::GetSchedule(const std::string &name) const
{
	std::map<std::string, Schedule>::const_iterator it = schedules.find(Normalize(name));
	if (it == schedules.end())
		return 0;
	return &it->second;
}

bool AccessCore::AddGroup(const std::string &name, bool override_lockdown, std::string &error)
{
	if (name.empty())
	{
		error = "Group name is empty";
		return false;
	}

	AccessGroup &group = groups[Normalize(name)];
	if (group.name.empty())
		group.name = name;
	if (override_lockdown == true)
		group.override_lockdown = true;
	version++;
	return true;
}

bool AccessCore::AddGroupRule(const std::string &group, const std::string &access_point, const std::string &schedule, std::string &error)
{
	std::map<std::string, AccessGroup>::iterator it = groups.find(Normalize(group));
	if (it == groups.end())
	{
		error = "Unknown group " + group;
		return false;
	}
	if (access_point.empty())
	{
		error = "Access point name is empty";
		return false;
	}

	std::string schedule_name = schedule.empty() ? "Always" : schedule;
	if (!GetSchedule(schedule_name))
	{
		error = "Unknown schedule " + schedule_name;
		return false;
	}

	AccessRule rule;
	rule.schedule = schedule_name;
	rule.display = access_point;

	//elevator floor rules ("Elevator N:Floors 6-9") keep their floor list, matched when evaluated
	int elevator = 0;
	std::string floor_spec;
	if (ParseElevatorRule(access_point, elevator, floor_spec) == true)
	{
		if (IsValidFloorSpec(floor_spec) == false)
		{
			error = "Invalid floor list in " + access_point;
			return false;
		}
		rule.access_point = Normalize("Elevator " + std::to_string(elevator));
		rule.floor_spec = floor_spec;
	}
	else
		rule.access_point = Normalize(access_point);

	it->second.rules.emplace_back(rule);
	version++;
	return true;
}

const AccessGroup* AccessCore::GetGroup(const std::string &name) const
{
	std::map<std::string, AccessGroup>::const_iterator it = groups.find(Normalize(name));
	if (it == groups.end())
		return 0;
	return &it->second;
}

bool AccessCore::IsBuiltInSchedule(const std::string &name)
{
	std::string key = Normalize(name);
	return key == "always" || key == "never";
}

bool AccessCore::RemoveScheduleWindow(const std::string &name, int index, std::string &error)
{
	if (IsBuiltInSchedule(name))
	{
		error = "The built-in schedules Always and Never can't be changed";
		return false;
	}
	std::map<std::string, Schedule>::iterator it = schedules.find(Normalize(name));
	if (it == schedules.end())
	{
		error = "Unknown schedule " + name;
		return false;
	}
	if (index < 0 || index >= (int)it->second.windows.size())
	{
		error = "Invalid time window";
		return false;
	}
	it->second.windows.erase(it->second.windows.begin() + index);
	version++;
	return true;
}

bool AccessCore::RemoveSchedule(const std::string &name, std::string &error)
{
	if (IsBuiltInSchedule(name))
	{
		error = "The built-in schedules Always and Never can't be deleted";
		return false;
	}
	std::string key = Normalize(name);
	std::map<std::string, Schedule>::iterator it = schedules.find(key);
	if (it == schedules.end())
	{
		error = "Unknown schedule " + name;
		return false;
	}

	//refuse if any rule uses it
	for (std::map<std::string, AccessGroup>::const_iterator g = groups.begin(); g != groups.end(); ++g)
	{
		for (size_t i = 0; i < g->second.rules.size(); i++)
		{
			if (Normalize(g->second.rules[i].schedule) == key)
			{
				error = "Schedule " + it->second.name + " is used by access level " + g->second.name + "; remove those rules first";
				return false;
			}
		}
	}

	schedules.erase(it);
	version++;
	return true;
}

bool AccessCore::RemoveGroup(const std::string &name, std::string &error)
{
	std::string key = Normalize(name);
	std::map<std::string, AccessGroup>::iterator it = groups.find(key);
	if (it == groups.end())
	{
		error = "Unknown group " + name;
		return false;
	}

	//take the group off every card
	for (std::map<uint32_t, Credential>::iterator c = credentials.begin(); c != credentials.end(); ++c)
	{
		std::vector<std::string> &list = c->second.groups;
		for (size_t i = 0; i < list.size();)
		{
			if (Normalize(list[i]) == key)
				list.erase(list.begin() + i);
			else
				i++;
		}
	}

	groups.erase(it);
	version++;
	return true;
}

bool AccessCore::SetGroupOverride(const std::string &name, bool value)
{
	std::map<std::string, AccessGroup>::iterator it = groups.find(Normalize(name));
	if (it == groups.end())
		return false;
	it->second.override_lockdown = value;
	version++;
	return true;
}

bool AccessCore::RemoveGroupRule(const std::string &group, int index, std::string &error)
{
	std::map<std::string, AccessGroup>::iterator it = groups.find(Normalize(group));
	if (it == groups.end())
	{
		error = "Unknown group " + group;
		return false;
	}
	if (index < 0 || index >= (int)it->second.rules.size())
	{
		error = "Invalid rule";
		return false;
	}
	it->second.rules.erase(it->second.rules.begin() + index);
	version++;
	return true;
}

std::vector<std::string> AccessCore::GetGroupNames() const
{
	std::vector<std::string> names;
	for (std::map<std::string, AccessGroup>::const_iterator it = groups.begin(); it != groups.end(); ++it)
		names.emplace_back(it->second.name);
	return names;
}

std::vector<std::string> AccessCore::GetScheduleNames() const
{
	std::vector<std::string> names;
	for (std::map<std::string, Schedule>::const_iterator it = schedules.begin(); it != schedules.end(); ++it)
		names.emplace_back(it->second.name);
	return names;
}

bool AccessCore::AddCredential(const Credential &credential, std::string &error)
{
	if (credential.facility_code < 0 || credential.facility_code > 255)
	{
		error = "Facility code must be 0-255";
		return false;
	}
	if (credential.card_number < 0 || credential.card_number > 65535)
	{
		error = "Card number must be 0-65535";
		return false;
	}
	for (size_t i = 0; i < credential.groups.size(); i++)
	{
		if (!GetGroup(credential.groups[i]))
		{
			error = "Unknown group " + credential.groups[i];
			return false;
		}
	}

	//adding an existing card replaces it
	credentials[CardKey(credential.facility_code, credential.card_number)] = credential;
	version++;
	return true;
}

Credential* AccessCore::GetCredential(int facility_code, int card_number)
{
	std::map<uint32_t, Credential>::iterator it = credentials.find(CardKey(facility_code, card_number));
	if (it == credentials.end())
		return 0;
	return &it->second;
}

const Credential* AccessCore::GetCredential(int facility_code, int card_number) const
{
	std::map<uint32_t, Credential>::const_iterator it = credentials.find(CardKey(facility_code, card_number));
	if (it == credentials.end())
		return 0;
	return &it->second;
}

bool AccessCore::SetCredentialEnabled(int facility_code, int card_number, bool enabled)
{
	Credential *credential = GetCredential(facility_code, card_number);
	if (!credential)
		return false;
	credential->enabled = enabled;
	version++;
	return true;
}

std::vector<const Credential*> AccessCore::GetCredentials() const
{
	std::vector<const Credential*> result;
	for (std::map<uint32_t, Credential>::const_iterator it = credentials.begin(); it != credentials.end(); ++it)
		result.emplace_back(&it->second);
	return result;
}

AccessResult AccessCore::Evaluate(int facility_code, int card_number, const std::string &access_point, int day_of_week, int minute_of_day) const
{
	//decision for a door or other named access point
	return EvaluateRules(GetCredential(facility_code, card_number), Normalize(access_point), 0, 0, day_of_week, minute_of_day);
}

AccessResult AccessCore::EvaluateFloor(int facility_code, int card_number, int elevator, int floor, const std::vector<std::string> &labels, int day_of_week, int minute_of_day) const
{
	//decision for an elevator floor; labels are the floor's ID and NumberID
	return EvaluateRules(GetCredential(facility_code, card_number), Normalize("Elevator " + std::to_string(elevator)), floor, &labels, day_of_week, minute_of_day);
}

AccessResult AccessCore::EvaluateRules(const Credential *credential, const std::string &point, int floor, const std::vector<std::string> *labels, int day_of_week, int minute_of_day) const
{
	//checks run in a fixed order; the first failure is the deny reason

	if (!credential)
		return AccessResult::DeniedUnknown;

	if (credential->enabled == false)
		return AccessResult::DeniedDisabled;

	bool has_override = false;
	for (size_t i = 0; i < credential->groups.size(); i++)
	{
		const AccessGroup *group = GetGroup(credential->groups[i]);
		if (group && group->override_lockdown == true)
			has_override = true;
	}

	//override groups (security, fire department) open everything, even during lockdown
	if (has_override == true)
		return AccessResult::Granted;

	if (lockdown == true)
		return AccessResult::DeniedLockdown;

	bool rule_found = false;

	for (size_t i = 0; i < credential->groups.size(); i++)
	{
		const AccessGroup *group = GetGroup(credential->groups[i]);
		if (!group)
			continue;

		for (size_t j = 0; j < group->rules.size(); j++)
		{
			const AccessRule &rule = group->rules[j];
			if (rule.access_point != point)
				continue;

			//floor rules only apply to floor decisions, and only to their floors
			if (labels)
			{
				if (rule.floor_spec.empty() || FloorSpecMatches(rule.floor_spec, floor, *labels) == false)
					continue;
			}
			else if (!rule.floor_spec.empty())
				continue;

			rule_found = true;
			const Schedule *schedule = GetSchedule(rule.schedule);
			if (schedule && schedule->Contains(day_of_week, minute_of_day))
				return AccessResult::Granted;
		}
	}

	if (rule_found == true)
		return AccessResult::DeniedSchedule;
	return AccessResult::DeniedNoRule;
}

void AccessCore::Log(const AccessEvent &event)
{
	log_total++;
	log.emplace_back(event);
	while (log.size() > log_capacity)
		log.pop_front();
}

void AccessCore::SetLogCapacity(size_t capacity)
{
	if (capacity < 1)
		capacity = 1;
	log_capacity = capacity;
	while (log.size() > log_capacity)
		log.pop_front();
}

std::string AccessCore::FormatEvent(const AccessEvent &event) const
{
	std::string card = "-";
	if (event.facility_code >= 0 && event.card_number >= 0)
		card = std::to_string(event.facility_code) + ":" + std::to_string(event.card_number);

	char buffer[512];
	snprintf(buffer, sizeof(buffer), "%-6s %-8s %-24s %-11s %-18s %s",
		event.clock.c_str(), event.kind.c_str(), event.access_point.c_str(), card.c_str(),
		event.holder.empty() ? "-" : event.holder.c_str(), event.detail.c_str());
	return buffer;
}

static std::string CsvField(const std::string &value)
{
	//quote a CSV field if needed
	if (value.find_first_of(",\"\n") == std::string::npos)
		return value;
	std::string result = "\"";
	for (size_t i = 0; i < value.size(); i++)
	{
		if (value[i] == '"')
			result += "\"\"";
		else
			result += value[i];
	}
	result += "\"";
	return result;
}

bool AccessCore::ExportLog(const std::string &filename, std::string &error) const
{
	std::ofstream file(filename.c_str());
	if (!file.is_open())
	{
		error = "Cannot open " + filename;
		return false;
	}

	file << "sim_time_ms,clock,event,access_point,facility,card,holder,detail\n";
	for (size_t i = 0; i < log.size(); i++)
	{
		const AccessEvent &e = log[i];
		file << e.sim_time_ms << "," << CsvField(e.clock) << "," << CsvField(e.kind) << "," << CsvField(e.access_point) << ",";
		if (e.facility_code >= 0)
			file << e.facility_code;
		file << ",";
		if (e.card_number >= 0)
			file << e.card_number;
		file << "," << CsvField(e.holder) << "," << CsvField(e.detail) << "\n";
	}
	return true;
}

bool AccessCore::ParseDays(const std::string &text, int &days_mask)
{
	//parses "Mon-Fri", "Sat,Sun", "Sat|Sun", "Daily", "All", or a single day

	static const char *names[7] = { "sun", "mon", "tue", "wed", "thu", "fri", "sat" };

	std::string value = Normalize(text);
	if (value == "daily" || value == "all" || value == "always")
	{
		days_mask = 0x7F;
		return true;
	}
	if (value == "weekdays")
	{
		days_mask = 0x3E;
		return true;
	}
	if (value == "weekends")
	{
		days_mask = 0x41;
		return true;
	}

	//split into items on ',' or '|' (note: Normalize has already removed spaces)
	std::vector<std::string> items;
	std::string current;
	for (size_t i = 0; i <= value.size(); i++)
	{
		if (i == value.size() || value[i] == ',' || value[i] == '|')
		{
			if (!current.empty())
				items.emplace_back(current);
			current.clear();
		}
		else
			current += value[i];
	}
	if (items.empty())
		return false;

	int mask = 0;
	for (size_t i = 0; i < items.size(); i++)
	{
		std::string item = items[i];
		size_t dash = item.find('-');
		std::string first = item.substr(0, dash);
		std::string last = (dash == std::string::npos) ? first : item.substr(dash + 1);

		int a = -1, b = -1;
		for (int d = 0; d < 7; d++)
		{
			if (first.compare(0, 3, names[d]) == 0 && first.size() >= 3)
				a = d;
			if (last.compare(0, 3, names[d]) == 0 && last.size() >= 3)
				b = d;
		}
		if (a < 0 || b < 0)
			return false;

		//ranges may wrap, such as Fri-Mon
		int d = a;
		while (true)
		{
			mask |= (1 << d);
			if (d == b)
				break;
			d = (d + 1) % 7;
		}
	}

	days_mask = mask;
	return true;
}

bool AccessCore::ParseClock(const std::string &text, int &minute_of_day)
{
	//parses "HH:MM" (24-hour); "24:00" is allowed as an end time

	std::string value = Normalize(text);
	size_t colon = value.find(':');
	if (colon == std::string::npos || colon == 0 || colon + 1 >= value.size())
		return false;

	std::string h = value.substr(0, colon);
	std::string m = value.substr(colon + 1);
	for (size_t i = 0; i < h.size(); i++)
		if (!std::isdigit((unsigned char)h[i])) return false;
	for (size_t i = 0; i < m.size(); i++)
		if (!std::isdigit((unsigned char)m[i])) return false;

	int hour = std::atoi(h.c_str());
	int minute = std::atoi(m.c_str());
	if (minute < 0 || minute > 59 || hour < 0 || hour > 24 || (hour == 24 && minute != 0))
		return false;

	minute_of_day = hour * 60 + minute;
	return true;
}

std::string AccessCore::FormatDays(int days_mask, char separator)
{
	//formats a day mask compactly, using ranges where possible: "Mon-Fri", "Sat,Sun", "Daily"
	static const char *names[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

	days_mask &= 0x7F;
	if (days_mask == 0x7F)
		return "Daily";
	if (days_mask == 0)
		return "None";

	std::string result;
	int d = 0;
	while (d < 7)
	{
		if ((days_mask & (1 << d)) == 0)
		{
			d++;
			continue;
		}
		int end = d;
		while (end + 1 < 7 && (days_mask & (1 << (end + 1))))
			end++;

		if (!result.empty())
			result += separator;
		result += names[d];
		if (end > d + 1)
			result += std::string("-") + names[end];
		else if (end == d + 1)
			result += separator + std::string(names[end]);
		d = end + 1;
	}
	return result;
}

std::string AccessCore::FormatWindow(const TimeWindow &window)
{
	return FormatDays(window.days_mask, '|') + ", " + FormatClock(window.start_minute) + ", " + (window.end_minute == 1440 ? std::string("24:00") : FormatClock(window.end_minute));
}

std::string AccessCore::FormatClock(int minute_of_day)
{
	char buffer[16];
	snprintf(buffer, sizeof(buffer), "%02d:%02d", (minute_of_day / 60) % 24, minute_of_day % 60);
	return buffer;
}

static bool ParseInt(const std::string &text, size_t &pos, int &value)
{
	//parse an optionally signed integer starting at pos
	size_t start = pos;
	if (pos < text.size() && (text[pos] == '-' || text[pos] == '+'))
		pos++;
	size_t digits = pos;
	while (pos < text.size() && std::isdigit((unsigned char)text[pos]))
		pos++;
	if (pos == digits)
	{
		pos = start;
		return false;
	}
	value = std::atoi(text.substr(start, pos - start).c_str());
	return true;
}

bool AccessCore::ParseFloorList(const std::string &text, std::vector<int> &floors)
{
	//parses floor lists like "40", "40-45", "-2", "-2 to 3", "-3--1", "40-45|50|52"
	//(items separated by '|' or ';'); ranges may be written low-high or high-low

	floors.clear();
	std::string value = Normalize(text);

	//"to" ranges become '~' so they're unambiguous
	std::string cleaned;
	for (size_t i = 0; i < value.size(); i++)
	{
		if (value.compare(i, 2, "to") == 0)
		{
			cleaned += '~';
			i++;
		}
		else
			cleaned += value[i];
	}

	std::vector<std::string> items;
	std::string current;
	for (size_t i = 0; i <= cleaned.size(); i++)
	{
		if (i == cleaned.size() || cleaned[i] == '|' || cleaned[i] == ';')
		{
			if (!current.empty())
				items.emplace_back(current);
			current.clear();
		}
		else
			current += cleaned[i];
	}
	if (items.empty())
		return false;

	for (size_t i = 0; i < items.size(); i++)
	{
		const std::string &item = items[i];
		size_t pos = 0;
		int a = 0, b = 0;
		if (!ParseInt(item, pos, a))
			return false;

		if (pos == item.size())
			b = a;
		else
		{
			if (item[pos] != '-' && item[pos] != '~')
				return false;
			pos++;
			if (!ParseInt(item, pos, b) || pos != item.size())
				return false;
		}

		if (a > b)
			std::swap(a, b);
		if (b - a > 10000)
			return false;

		for (int f = a; f <= b; f++)
		{
			if (std::find(floors.begin(), floors.end(), f) == floors.end())
				floors.emplace_back(f);
		}
	}

	std::sort(floors.begin(), floors.end());
	return true;
}

std::string AccessCore::ElevatorFloorPoint(int elevator, int floor)
{
	return "Elevator " + std::to_string(elevator) + ":Floor " + std::to_string(floor);
}

bool AccessCore::ParseElevatorPoint(const std::string &access_point, int &elevator)
{
	//matches "Elevator N" (any case and spacing)
	std::string value = Normalize(access_point);
	if (value.compare(0, 8, "elevator") != 0 || value.size() == 8)
		return false;
	size_t pos = 8;
	int number = 0;
	if (!ParseInt(value, pos, number) || pos != value.size() || number < 1)
		return false;
	elevator = number;
	return true;
}

std::string AccessCore::StationPoint(int floor, int number)
{
	return "CallStation " + std::to_string(floor) + ":" + std::to_string(number);
}

bool AccessCore::ParseStationPoint(const std::string &access_point, int &floor, int &number)
{
	//matches "CallStation F:N" or "Call Station F:N" (any case and spacing), floor may be negative
	std::string value = Normalize(access_point);
	if (value.compare(0, 11, "callstation") != 0)
		return false;
	size_t pos = 11;
	int f = 0, n = 0;
	if (!ParseInt(value, pos, f) || pos >= value.size() || value[pos] != ':')
		return false;
	pos++;
	if (!ParseInt(value, pos, n) || pos != value.size() || n < 1)
		return false;
	floor = f;
	number = n;
	return true;
}

bool AccessCore::ParseElevatorRule(const std::string &access_point, int &elevator, std::string &floor_spec)
{
	//splits "Elevator N:Floors 6-9" or "Elevator N:Floor R" into the elevator number and floor list
	//returns false if the name isn't an elevator floor rule

	std::string value = Normalize(access_point);
	if (value.compare(0, 8, "elevator") != 0)
		return false;

	size_t colon = value.find(':');
	if (colon == std::string::npos)
		return false;

	size_t pos = 8;
	int number = 0;
	if (!ParseInt(value, pos, number) || pos != colon || number < 1)
		return false;

	std::string rest = value.substr(colon + 1);
	if (rest.compare(0, 6, "floors") == 0)
		rest = rest.substr(6);
	else if (rest.compare(0, 5, "floor") == 0)
		rest = rest.substr(5);
	else
		return false;

	if (rest.empty())
		return false;

	elevator = number;
	floor_spec = rest;
	return true;
}

static void SplitSpec(const std::string &spec, std::vector<std::string> &items)
{
	//split a normalized floor spec on '|' or ';'
	items.clear();
	std::string current;
	for (size_t i = 0; i <= spec.size(); i++)
	{
		if (i == spec.size() || spec[i] == '|' || spec[i] == ';')
		{
			if (!current.empty())
				items.emplace_back(current);
			current.clear();
		}
		else
			current += spec[i];
	}
}

static bool ParseNumericRange(const std::string &item, int &a, int &b)
{
	//parses "6", "6-9", "6to9", "-2to3", "-3--1" (item already normalized)
	std::string cleaned;
	for (size_t i = 0; i < item.size(); i++)
	{
		if (item.compare(i, 2, "to") == 0)
		{
			cleaned += '~';
			i++;
		}
		else
			cleaned += item[i];
	}

	size_t pos = 0;
	if (!ParseInt(cleaned, pos, a))
		return false;
	if (pos == cleaned.size())
	{
		b = a;
		return true;
	}
	if (cleaned[pos] != '-' && cleaned[pos] != '~')
		return false;
	pos++;
	if (!ParseInt(cleaned, pos, b) || pos != cleaned.size())
		return false;
	if (a > b)
		std::swap(a, b);
	return true;
}

bool AccessCore::IsValidFloorSpec(const std::string &spec)
{
	//a floor spec is a list of items separated by '|':
	//  6-9, 12      floor labels (the numbers shown on buttons and keypads), as numbers or ranges
	//  R, L, B2     floor labels as text (exact match)
	//  @5-8         internal floor numbers (as used by ServicedFloors), when labels don't fit

	std::vector<std::string> items;
	SplitSpec(Normalize(spec), items);
	if (items.empty())
		return false;

	for (size_t i = 0; i < items.size(); i++)
	{
		const std::string &item = items[i];
		if (item[0] == '@')
		{
			std::vector<int> floors;
			if (!ParseFloorList(item.substr(1), floors))
				return false;
		}
		//any other item is valid: a numeric range, or a text label
	}
	return true;
}

bool AccessCore::FloorSpecMatches(const std::string &spec, int floor, const std::vector<std::string> &labels)
{
	//returns true if a floor (internal number, plus its labels) is covered by a floor spec

	std::vector<std::string> items;
	SplitSpec(Normalize(spec), items);

	for (size_t i = 0; i < items.size(); i++)
	{
		const std::string &item = items[i];

		//internal floor numbers
		if (item[0] == '@')
		{
			std::vector<int> floors;
			if (ParseFloorList(item.substr(1), floors) && std::find(floors.begin(), floors.end(), floor) != floors.end())
				return true;
			continue;
		}

		int a = 0, b = 0;
		bool numeric = ParseNumericRange(item, a, b);

		for (size_t j = 0; j < labels.size(); j++)
		{
			std::string label = Normalize(labels[j]);
			if (label.empty())
				continue;

			//exact label match (works for text labels like "R" and single numbers)
			if (label == item)
				return true;

			//numeric label within a numeric range
			if (numeric == true)
			{
				size_t pos = 0;
				int value = 0;
				if (ParseInt(label, pos, value) && pos == label.size() && value >= a && value <= b)
					return true;
			}
		}
	}
	return false;
}

uint32_t AccessCore::EncodeH10301(int facility_code, int card_number)
{
	//HID 26-bit format: [even parity][8-bit facility][16-bit card][odd parity]
	//even parity covers the first 12 data bits, odd parity covers the last 12

	uint32_t data = ((uint32_t)(facility_code & 0xFF) << 16) | (uint32_t)(card_number & 0xFFFF); //24 bits

	int ones_high = 0, ones_low = 0;
	for (int i = 12; i < 24; i++)
		ones_high += (data >> i) & 1;
	for (int i = 0; i < 12; i++)
		ones_low += (data >> i) & 1;

	uint32_t even = (ones_high % 2 == 1) ? 1 : 0; //makes the first 13 bits even
	uint32_t odd = (ones_low % 2 == 0) ? 1 : 0; //makes the last 13 bits odd

	return (even << 25) | (data << 1) | odd;
}

bool AccessCore::DecodeH10301(uint32_t raw, int &facility_code, int &card_number)
{
	if (raw >> 26)
		return false; //more than 26 bits

	uint32_t data = (raw >> 1) & 0xFFFFFF;
	if (EncodeH10301((data >> 16) & 0xFF, data & 0xFFFF) != raw)
		return false; //parity mismatch

	facility_code = (data >> 16) & 0xFF;
	card_number = data & 0xFFFF;
	return true;
}

}
