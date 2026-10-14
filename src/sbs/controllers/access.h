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

#ifndef _SBS_ACCESS_H
#define _SBS_ACCESS_H

#include <functional>
#include <map>
#include "accesscore.h"

namespace SBS {

class AccessController;
class CardReader;

//an access point binds one or more lockable targets (phase 1: doors) to card readers
class SBSIMPEXP AccessPoint
{
public:
	AccessPoint(AccessController *controller, const std::string &name);
	~AccessPoint();

	const std::string& GetName() { return name; }

	//settings
	void SetUnlockTime(int milliseconds) { unlock_time = milliseconds; }
	void SetHeldTime(int milliseconds) { held_time = milliseconds; }
	void SetAutoOpen(bool value) { auto_open = value; }
	void SetAutoCloseTime(int milliseconds) { close_time = milliseconds; } //-1 = use the controller default, 0 = off
	int GetUnlockTime() { return unlock_time; }
	int GetHeldTime() { return held_time; }
	int GetAutoCloseTime(); //effective auto-close time in milliseconds, 0 if off

	//targets
	void AddDoor(Door *door);
	void RemoveDoor(Door *door);
	int GetDoorCount() { return (int)doors.size(); }

	//state
	void Grant(); //momentary release after a valid card or a Grant action
	void RequestExit(); //momentary release from the secure side
	void SetHeldUnlocked(bool value); //hold unlocked (Unlock action) or return to normal (Lock action)
	bool IsReleased(); //true if targets should currently behave as unlocked
	std::string GetStatus();

	//called by DoorLock when a door starts opening
	void OnDoorOpened(Door *door, bool locked_side, bool forced);

	//called from the access controller runloop
	void Update(unsigned long now);

private:
	struct DoorState
	{
		Door *door;
		bool was_open;
		bool authorized; //current opening was authorized
		unsigned long open_since;
		bool held_alarm;
	};

	AccessController *controller;
	std::string name;
	std::vector<DoorState> doors;
	int unlock_time; //milliseconds a grant releases the lock
	int held_time; //milliseconds before a door-held alarm
	bool auto_open; //open the door automatically on grant
	int close_time; //auto-close time in milliseconds; -1 = controller default, 0 = off
	bool released;
	bool held_unlocked;
	bool opening_internally; //set while this access point opens its own doors
	unsigned long release_until;
};

//access controller: one per sim engine, created on first use
class SBSIMPEXP AccessController : public Object
{
public:
	AccessController(Object *parent);
	~AccessController();

	AccessCore* GetCore() { return &core; }

	//access points
	AccessPoint* GetAccessPoint(const std::string &name, bool create = false);
	int GetAccessPointCount() { return (int)points.size(); }
	AccessPoint* GetAccessPoint(int index);

	//card readers
	CardReader* AddCardReader(Floor *floor, const std::string &access_point, const std::string &grant_sound, const std::string &deny_sound, const std::string &idle_texture, const std::string &grant_texture, const std::string &deny_texture, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset);
	void RegisterReader(CardReader *reader);
	int GetReaderState(const std::string &access_point); //1 = unlocked, 0 = locked, -1 = unknown
	void UnregisterReader(CardReader *reader);

	//elevator floor lockout
	bool SecureElevatorFloors(int elevator, const std::string &floor_spec, int unlock_time, bool auto_select);
	bool IsFloorSecured(int elevator, int floor);
	std::vector<int> GetSecuredFloors(int elevator);
	std::vector<std::string> GetFloorLabels(int floor); //a floor's ID and NumberID
	std::string GetFloorLabel(int floor); //a floor's ID, as shown on buttons
	bool ResolveFloor(const std::string &label, int &floor); //label (or @N internal number) to internal floor number
	bool CheckElevatorFloor(int elevator, int floor); //called by Elevator::SelectFloor
	AccessResult PresentElevator(int elevator, int facility_code, int card_number, CardReader *reader = 0, bool allow_autoselect = true);
	bool IsElevatorFloorReleased(int elevator, int floor);
	CardReader* AddCabReader(ElevatorCar *car, const std::string &grant_sound, const std::string &deny_sound, const std::string &idle_texture, const std::string &grant_texture, const std::string &deny_texture, const std::string &direction, Real CenterX, Real CenterZ, Real width, Real height, Real voffset);
	CardReader* AddPanelReader(ButtonPanel *panel, const std::string &grant_sound, const std::string &deny_sound, const std::string &idle_texture, const std::string &grant_texture, const std::string &deny_texture, int row, int column, Real bwidth, Real bheight, Real hoffset, Real voffset);
	void ListElevators();

	//destination dispatch call stations
	bool IsStationFloorSecured(CallStation *station, int floor);
	bool CheckStationFloor(CallStation *station, int floor); //called by CallStation::SelectFloor
	AccessResult PresentStation(CallStation *station, int facility_code, int card_number, CardReader *reader = 0, bool use_home_floor = true);
	bool IsStationFloorReleased(CallStation *station, int floor);

	//secured zones (areas)
	enum PassbackMode { PassbackOff = 0, PassbackSoft = 1, PassbackHard = 2 };
	bool AddZone(const std::string &name, const Vector3 &min, const Vector3 &max, int passback, const std::string &alarm_on, const std::string &alarm_off);
	bool AddZoneEntry(const std::string &zone, const std::string &entry);
	bool ResetZone(const std::string &zone); //clear alarm and anti-passback state
	void ListZones();

	//virtual people badges
	void SetPeopleGroups(const std::vector<std::string> &groups, bool service);
	bool GetPersonCredential(Person *person, int &facility_code, int &card_number);
	bool PersonRequestElevatorFloor(Person *person, int elevator, int floor); //tap a badge for a person, if needed
	bool PersonRequestStationFloor(Person *person, CallStation *station, int floor);
	void SetStationUnlockTime(int milliseconds) { station_unlock_time = milliseconds; }

	//decisions
	AccessResult Present(const std::string &access_point, int facility_code, int card_number, CardReader *reader = 0);
	AccessResult PresentWallet(const std::string &access_point, CardReader *reader = 0);
	void Lockdown(bool value);

	//auto-close default for all access points (milliseconds, 0 = off)
	void SetDefaultAutoClose(int milliseconds) { default_close_time = milliseconds; }
	int GetDefaultAutoClose() { return default_close_time; }

	//player wallet
	bool WalletAdd(int facility_code, int card_number);
	bool WalletRemove(int facility_code, int card_number);
	bool NextBadge();
	bool GetActiveBadge(int &facility_code, int &card_number);
	bool IsWalletBadge(int facility_code, int card_number);
	bool SetActiveBadge(int facility_code, int card_number);
	std::vector<std::pair<int, int> > GetWallet();
	int GetActiveBadgeIndex() { return active_badge; }

	//status, for the Access Control window
	std::vector<std::string> GetElevatorStatus();
	std::vector<std::string> GetZoneStatus();
	std::vector<std::string> GetZoneNames();
	bool ExportScript(const std::string &filename); //write the <Access> section (profiles, levels, rules, cards, settings)
	std::vector<std::string> GetPlaceNames(); //doors and secured elevator floors, for building rules
	AccessResult CheckAccess(int facility_code, int card_number, const std::string &place, std::string &detail); //test only, no logging or unlocking
	void ListWallet();

	//clock: returns day of week (0 = Sunday) and minute of day; defaults to the system clock
	void SetClockSource(std::function<void(int &day_of_week, int &minute_of_day)> source) { clock_source = source; }
	void GetClock(int &day_of_week, int &minute_of_day);

	//event log and reports
	void LogEvent(const std::string &kind, const std::string &access_point, int facility_code, int card_number, const std::string &detail);
	void ShowLog(int count);
	bool ExportLog(const std::string &filename);
	void ListCards();
	void ListAccessPoints();

	bool Loop();
	void Report(const std::string &message);
	bool ReportError(const std::string &message);

private:
	struct Badge
	{
		int facility_code;
		int card_number;
	};

	struct ElevatorSecurity
	{
		int elevator = 0;
		std::string floor_spec; //secured floors, as labels or @internal numbers, separated by |
		int unlock_time = 10000; //milliseconds a badge tap releases floors
		bool auto_select = false; //select the floor automatically if a badge allows exactly one
		std::vector<int> released; //floors released by the last badge tap
		unsigned long release_until = 0;
		int card_facility = -1; //badge that made the release
		int card_number = -1;
	};

	struct ZoneCard
	{
		int facility_code = -1;
		int card_number = -1;
		bool confirmed = false; //holder has physically entered
		unsigned long expires = 0; //unconfirmed entries lapse after this time
	};

	struct Zone
	{
		std::string name;
		Vector3 min, max;
		int passback = PassbackOff;
		std::string alarm_on, alarm_off; //action names
		std::vector<std::string> door_entries; //normalized access point names
		std::vector<std::string> floor_entries; //floor specs (labels)
		std::vector<ZoneCard> cards; //cards inside, or granted entry
		bool player_inside = false;
		int occupant_facility = -1; //card the player entered with, -1 if unbadged
		int occupant_number = -1;
		bool alarm = false;
		int entries = 0; //total entries, for the listing
	};

	std::vector<Zone> zones;
	std::vector<Zone*> ZonesForPoint(const std::string &access_point);
	std::vector<Zone*> ZonesForFloor(int floor);
	bool CheckPassback(const std::vector<Zone*> &list, int facility_code, int card_number, const std::string &where); //true = block
	void NotifyEntryGrant(const std::vector<Zone*> &list, int facility_code, int card_number);
	void UpdateZones(unsigned long now);
	void SetZoneAlarm(Zone &zone, bool value, const std::string &reason);

	std::vector<std::string> people_groups, service_people_groups;
	std::map<std::string, Badge> people_badges; //by person name
	int next_person_card;

	struct StationRelease
	{
		int floor = 0;
		int number = 0;
		std::vector<int> released;
		unsigned long release_until = 0;
		int card_facility = -1;
		int card_number = -1;
	};

	StationRelease* GetStationRelease(CallStation *station, bool create);
	void FlashReaders(const std::string &access_point, bool granted);
	std::vector<StationRelease> station_releases;
	int station_unlock_time;
	ElevatorSecurity* GetElevatorSecurity(int elevator);
	void FlashElevatorReaders(int elevator, bool granted);
	std::vector<ElevatorSecurity> elevator_security;

	AccessCore core;
	std::vector<AccessPoint*> points;
	std::vector<CardReader*> readers;
	std::vector<Badge> wallet;
	int active_badge;
	int default_close_time;
	std::function<void(int &, int &)> clock_source;
};

}

#endif
