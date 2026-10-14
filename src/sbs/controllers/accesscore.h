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

//The access control core holds credentials, groups, schedules and the event log,
//and makes grant/deny decisions. It has no Ogre or SBS dependencies, so it can be
//unit-tested on its own (see tools/accesstest, which defines ACCESSCORE_STANDALONE).

#ifndef _SBS_ACCESSCORE_H
#define _SBS_ACCESSCORE_H

#include <string>
#include <vector>
#include <deque>
#include <map>
#include <cstdint>

//when built as part of SBS, export these classes from SBS.dll so other modules
//(such as the script processor in VM.dll on Windows) can use them.
//define ACCESSCORE_STANDALONE to build without Ogre/SBS, as the unit tests do.
#ifndef ACCESSCORE_STANDALONE
	#include "globals.h"
#endif
#ifndef SBSIMPEXP
	#define SBSIMPEXP
#endif

namespace SBS {

enum class AccessResult
{
	Granted = 0,
	DeniedUnknown,		//card is not enrolled
	DeniedDisabled,		//card has been disabled (lost/revoked)
	DeniedNoRule,		//no group rule covers this access point
	DeniedSchedule,		//a rule exists, but not at this time
	DeniedLockdown,		//building is locked down and card has no override
	DeniedPassback		//anti-passback: card already used to enter and hasn't left
};

SBSIMPEXP const char* AccessResultName(AccessResult result);

struct Credential
{
	int facility_code = 0;		//0-255 for the 26-bit H10301 format
	int card_number = 0;		//0-65535 for the 26-bit H10301 format
	std::string holder;		//card holder name
	std::vector<std::string> groups; //access group names
	std::string home_floor;		//destination dispatch home floor label (or @N for an internal number), empty = none
	bool enabled = true;
};

struct TimeWindow
{
	int days_mask = 0x7F;		//bit 0 = Sunday ... bit 6 = Saturday
	int start_minute = 0;		//0-1439
	int end_minute = 1440;		//exclusive; if less than start, the window crosses midnight
};

struct SBSIMPEXP Schedule
{
	std::string name;
	std::vector<TimeWindow> windows;

	bool Contains(int day_of_week, int minute_of_day) const;
};

struct AccessRule
{
	std::string access_point;	//normalized access point name
	std::string schedule;		//schedule name, "Always" if empty
	std::string floor_spec;		//for elevator rules: floors this rule covers (see FloorSpecMatches), empty otherwise
	std::string display;		//access point as written, for display and export
};

struct AccessGroup
{
	std::string name;
	bool override_lockdown = false;
	std::vector<AccessRule> rules;
};

struct AccessEvent
{
	unsigned long sim_time_ms = 0;	//SBS run time when the event happened
	std::string clock;		//virtual clock time, "HH:MM"
	std::string access_point;
	std::string kind;		//"GRANTED", "DENIED", "ALARM", "EXIT", "INFO"
	int facility_code = -1;
	int card_number = -1;
	std::string holder;
	std::string detail;		//deny reason or alarm type
};

class SBSIMPEXP AccessCore
{
public:
	AccessCore();

	//schedules
	bool AddScheduleWindow(const std::string &name, int days_mask, int start_minute, int end_minute, std::string &error);
	const Schedule* GetSchedule(const std::string &name) const;
	bool RemoveScheduleWindow(const std::string &name, int index, std::string &error);
	bool RemoveSchedule(const std::string &name, std::string &error); //fails if a rule uses it
	static bool IsBuiltInSchedule(const std::string &name);

	//groups
	bool AddGroup(const std::string &name, bool override_lockdown, std::string &error);
	bool AddGroupRule(const std::string &group, const std::string &access_point, const std::string &schedule, std::string &error);
	const AccessGroup* GetGroup(const std::string &name) const;
	bool RemoveGroup(const std::string &name, std::string &error); //also removes it from cards
	bool SetGroupOverride(const std::string &name, bool value);
	bool RemoveGroupRule(const std::string &group, int index, std::string &error);
	std::vector<std::string> GetGroupNames() const;
	std::vector<std::string> GetScheduleNames() const;

	//credentials
	bool AddCredential(const Credential &credential, std::string &error);
	Credential* GetCredential(int facility_code, int card_number);
	const Credential* GetCredential(int facility_code, int card_number) const;
	bool SetCredentialEnabled(int facility_code, int card_number, bool enabled);
	std::vector<const Credential*> GetCredentials() const;

	//decisions
	void SetLockdown(bool value) { lockdown = value; }
	bool GetLockdown() const { return lockdown; }
	AccessResult Evaluate(int facility_code, int card_number, const std::string &access_point, int day_of_week, int minute_of_day) const;
	AccessResult EvaluateFloor(int facility_code, int card_number, int elevator, int floor, const std::vector<std::string> &labels, int day_of_week, int minute_of_day) const;

	//event log
	void Log(const AccessEvent &event);
	const std::deque<AccessEvent>& GetLog() const { return log; }
	unsigned long GetLogTotal() const { return log_total; } //events logged since start (keeps counting past capacity)
	unsigned long GetVersion() const { return version; } //changes whenever schedules, groups, rules or cards change
	void SetLogCapacity(size_t capacity);
	std::string FormatEvent(const AccessEvent &event) const;
	bool ExportLog(const std::string &filename, std::string &error) const;

	//helpers
	static std::string Normalize(const std::string &name);
	static bool ParseDays(const std::string &text, int &days_mask);
	static bool ParseClock(const std::string &text, int &minute_of_day);
	static std::string FormatClock(int minute_of_day);
	static std::string FormatDays(int days_mask, char separator = ',');
	static std::string FormatWindow(const TimeWindow &window); //script form: "Sat|Sun, 09:00, 13:00"
	static bool ParseFloorList(const std::string &text, std::vector<int> &floors);
	static bool IsValidFloorSpec(const std::string &spec);
	static bool FloorSpecMatches(const std::string &spec, int floor, const std::vector<std::string> &labels);
	static bool ParseElevatorRule(const std::string &access_point, int &elevator, std::string &floor_spec);
	static bool ParseElevatorPoint(const std::string &access_point, int &elevator);
	static std::string ElevatorFloorPoint(int elevator, int floor);
	static bool ParseStationPoint(const std::string &access_point, int &floor, int &number);
	static std::string StationPoint(int floor, int number);
	static uint32_t EncodeH10301(int facility_code, int card_number);
	static bool DecodeH10301(uint32_t raw, int &facility_code, int &card_number);

private:
	std::map<std::string, Schedule> schedules; //keyed by normalized name
	std::map<std::string, AccessGroup> groups; //keyed by normalized name
	std::map<uint32_t, Credential> credentials; //keyed by (facility << 16) | card
	std::deque<AccessEvent> log;
	size_t log_capacity;
	unsigned long log_total;
	unsigned long version;
	bool lockdown;

	static uint32_t CardKey(int facility_code, int card_number);
	AccessResult EvaluateRules(const Credential *credential, const std::string &point, int floor, const std::vector<std::string> *labels, int day_of_week, int minute_of_day) const;
};

}

#endif
