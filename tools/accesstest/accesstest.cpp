//Standalone unit tests for the access control core (no Ogre or SBS needed)
//Build: g++ -std=c++17 -DACCESSCORE_STANDALONE -I../../src/sbs/controllers accesstest.cpp ../../src/sbs/controllers/accesscore.cpp -o accesstest

#include <cstdio>
#include <string>
#include "accesscore.h"

using namespace SBS;

static int failures = 0;
#define CHECK(cond) do { if (!(cond)) { printf("FAIL line %d: %s\n", __LINE__, #cond); failures++; } } while (0)

int main()
{
	AccessCore core;
	std::string err;
	int mask = 0, m = 0;

	//day and clock parsing
	CHECK(AccessCore::ParseDays("Mon-Fri", mask) && mask == 0x3E);
	CHECK(AccessCore::ParseDays("Sat, Sun", mask) && mask == 0x41);
	CHECK(AccessCore::ParseDays("Fri-Mon", mask) && mask == (0x20 | 0x40 | 0x01 | 0x02));
	CHECK(AccessCore::ParseDays("Daily", mask) && mask == 0x7F);
	CHECK(!AccessCore::ParseDays("Funday", mask));
	CHECK(AccessCore::ParseClock("07:30", m) && m == 450);
	CHECK(AccessCore::ParseClock("24:00", m) && m == 1440);
	CHECK(!AccessCore::ParseClock("25:00", m));
	CHECK(!AccessCore::ParseClock("7", m));

	//schedules
	CHECK(core.AddScheduleWindow("Business Hours", 0x3E, 7 * 60, 19 * 60, err));
	CHECK(core.AddScheduleWindow("Night", 0x3E, 22 * 60, 6 * 60, err)); //crosses midnight
	const Schedule *biz = core.GetSchedule("businesshours");
	CHECK(biz && biz->Contains(1, 8 * 60) && !biz->Contains(0, 8 * 60) && !biz->Contains(1, 19 * 60));
	const Schedule *night = core.GetSchedule("Night");
	CHECK(night->Contains(5, 23 * 60));		//Friday 23:00
	CHECK(night->Contains(6, 5 * 60));		//Saturday 05:00 (from Friday's window)
	CHECK(!night->Contains(0, 5 * 60));		//Sunday 05:00 (Saturday not in mask)
	CHECK(!night->Contains(1, 12 * 60));
	CHECK(core.GetSchedule("Always")->Contains(3, 1439));
	CHECK(!core.GetSchedule("Never")->Contains(3, 600));

	//groups and cards
	CHECK(core.AddGroup("Staff", false, err));
	CHECK(core.AddGroup("IT", false, err));
	CHECK(core.AddGroup("Security", true, err));
	CHECK(core.AddGroupRule("Staff", "Lobby Door", "Business Hours", err));
	CHECK(core.AddGroupRule("IT", "Server Room Door", "Always", err));
	CHECK(!core.AddGroupRule("Nobody", "X", "Always", err));
	CHECK(!core.AddGroupRule("IT", "X", "NoSuchSchedule", err));

	Credential jane; jane.facility_code = 42; jane.card_number = 10001; jane.holder = "Jane"; jane.groups = { "Staff", "IT" };
	Credential bob; bob.facility_code = 42; bob.card_number = 10002; bob.holder = "Bob"; bob.groups = { "Staff" };
	Credential guard; guard.facility_code = 42; guard.card_number = 10099; guard.holder = "Guard"; guard.groups = { "Security" };
	Credential bad; bad.facility_code = 300; bad.card_number = 1;
	Credential badgroup; badgroup.facility_code = 1; badgroup.card_number = 1; badgroup.groups = { "Ghosts" };
	CHECK(core.AddCredential(jane, err));
	CHECK(core.AddCredential(bob, err));
	CHECK(core.AddCredential(guard, err));
	CHECK(!core.AddCredential(bad, err));
	CHECK(!core.AddCredential(badgroup, err));

	//decisions: Monday 09:00 and Sunday 09:00
	CHECK(core.Evaluate(42, 10001, "Server Room Door", 1, 540) == AccessResult::Granted);
	CHECK(core.Evaluate(42, 10001, "server room door", 0, 540) == AccessResult::Granted); //name matching ignores case/spaces
	CHECK(core.Evaluate(42, 10002, "Server Room Door", 1, 540) == AccessResult::DeniedNoRule);
	CHECK(core.Evaluate(42, 10002, "Lobby Door", 1, 540) == AccessResult::Granted);
	CHECK(core.Evaluate(42, 10002, "Lobby Door", 0, 540) == AccessResult::DeniedSchedule);
	CHECK(core.Evaluate(42, 55555, "Lobby Door", 1, 540) == AccessResult::DeniedUnknown);
	CHECK(core.SetCredentialEnabled(42, 10002, false));
	CHECK(core.Evaluate(42, 10002, "Lobby Door", 1, 540) == AccessResult::DeniedDisabled);
	core.SetLockdown(true);
	CHECK(core.Evaluate(42, 10001, "Server Room Door", 1, 540) == AccessResult::DeniedLockdown);
	CHECK(core.Evaluate(42, 10099, "Server Room Door", 1, 540) == AccessResult::Granted); //override group
	core.SetLockdown(false);

	//Wiegand 26-bit round trip and parity checks
	for (int f = 0; f < 256; f += 17)
	{
		for (int c = 0; c < 65536; c += 4099)
		{
			uint32_t raw = AccessCore::EncodeH10301(f, c);
			int f2 = -1, c2 = -1;
			CHECK(raw < (1u << 26));
			CHECK(AccessCore::DecodeH10301(raw, f2, c2) && f2 == f && c2 == c);
			CHECK(!AccessCore::DecodeH10301(raw ^ 1u, f2, c2));			//flip odd parity bit
			CHECK(!AccessCore::DecodeH10301(raw ^ (1u << 25), f2, c2));	//flip even parity bit
		}
	}
	//known vector: facility 1, card 1 -> data 0x010001, 2 ones total
	//high 12 bits have one 1 (even parity bit = 1), low 12 bits have one 1 (odd parity bit = 0)
	CHECK(AccessCore::EncodeH10301(1, 1) == ((1u << 25) | (0x010001u << 1) | 0u));

	//log capacity and export
	core.SetLogCapacity(3);
	for (int i = 0; i < 5; i++)
	{
		AccessEvent e; e.sim_time_ms = i; e.kind = "INFO"; e.access_point = "P"; e.detail = "n" + std::to_string(i);
		core.Log(e);
	}
	CHECK(core.GetLog().size() == 3 && core.GetLog().front().detail == "n2");
	CHECK(core.ExportLog("/tmp/access_log_test.csv", err));

	//floor lists and elevator rules
	std::vector<int> fl;
	CHECK(AccessCore::ParseFloorList("40-45", fl) && fl.size() == 6 && fl.front() == 40 && fl.back() == 45);
	CHECK(AccessCore::ParseFloorList("-2", fl) && fl.size() == 1 && fl[0] == -2);
	CHECK(AccessCore::ParseFloorList("-2 to 1", fl) && fl.size() == 4 && fl[0] == -2 && fl[3] == 1);
	CHECK(AccessCore::ParseFloorList("-3--1", fl) && fl.size() == 3 && fl[0] == -3 && fl[2] == -1);
	CHECK(AccessCore::ParseFloorList("5|7-8|5", fl) && fl.size() == 3 && fl[0] == 5 && fl[1] == 7 && fl[2] == 8);
	CHECK(AccessCore::ParseFloorList("45-40", fl) && fl.size() == 6);
	CHECK(!AccessCore::ParseFloorList("ten", fl));
	CHECK(!AccessCore::ParseFloorList("4-", fl));
	int en = 0;
	std::string spec;
	CHECK(AccessCore::ParseElevatorRule("Elevator 1:Floors 6-9", en, spec) && en == 1 && spec == "6-9");
	CHECK(AccessCore::ParseElevatorRule("elevator 12 : floor R", en, spec) && en == 12 && spec == "r");
	CHECK(!AccessCore::ParseElevatorRule("Server Room Door", en, spec));
	CHECK(!AccessCore::ParseElevatorRule("Elevator 1:Lobby", en, spec));
	CHECK(AccessCore::ParseElevatorPoint("Elevator 3", en) && en == 3);
	CHECK(!AccessCore::ParseElevatorPoint("Elevator 3:Floor 2", en));
	CHECK(!AccessCore::ParseElevatorPoint("Elevator", en));

	//floor specs match labels (ID and NumberID), or internal numbers with @
	std::vector<std::string> lab5 = { "6", "6" };	//internal floor 5 labeled 6
	std::vector<std::string> labR = { "R", "11" };	//roof labeled R, keypad number 11
	std::vector<std::string> labB = { "B1", "-1" };	//basement
	CHECK(AccessCore::FloorSpecMatches("6-9", 5, lab5));
	CHECK(!AccessCore::FloorSpecMatches("7-9", 5, lab5));
	CHECK(AccessCore::FloorSpecMatches("6", 5, lab5));
	CHECK(AccessCore::FloorSpecMatches("@5-8", 5, lab5));
	CHECK(!AccessCore::FloorSpecMatches("@6-8", 5, lab5));
	CHECK(AccessCore::FloorSpecMatches("1-3|R", 10, labR));
	CHECK(AccessCore::FloorSpecMatches("r", 10, labR));
	CHECK(AccessCore::FloorSpecMatches("11", 10, labR));		//keypad number
	CHECK(AccessCore::FloorSpecMatches("b1", -1, labB));
	CHECK(AccessCore::FloorSpecMatches("-2 to -1", -1, labB));
	CHECK(!AccessCore::FloorSpecMatches("L", 0, lab5));
	CHECK(AccessCore::IsValidFloorSpec("6-9|R|@0"));
	CHECK(!AccessCore::IsValidFloorSpec("@x"));
	CHECK(!AccessCore::IsValidFloorSpec(""));

	//elevator floor rules evaluate by label
	CHECK(core.AddGroup("Tenant", false, err));
	CHECK(core.AddGroupRule("Tenant", "Elevator 1:Floor 6", "Always", err));
	CHECK(core.AddGroupRule("Tenant", "Elevator 2:Floors 9-10|R", "Always", err));
	CHECK(!core.AddGroupRule("Tenant", "Elevator 1:Floors @x", "Always", err));
	Credential t; t.facility_code = 7; t.card_number = 1; t.groups = { "Tenant" };
	CHECK(core.AddCredential(t, err));
	CHECK(core.EvaluateFloor(7, 1, 1, 5, lab5, 1, 600) == AccessResult::Granted);
	CHECK(core.EvaluateFloor(7, 1, 1, 6, std::vector<std::string>{ "7", "7" }, 1, 600) == AccessResult::DeniedNoRule);
	CHECK(core.EvaluateFloor(7, 1, 2, 10, labR, 1, 600) == AccessResult::Granted);
	CHECK(core.EvaluateFloor(7, 1, 3, 5, lab5, 1, 600) == AccessResult::DeniedNoRule);
	CHECK(core.Evaluate(7, 1, "Elevator 1", 1, 600) == AccessResult::DeniedNoRule); //floor rules don't open plain points
	CHECK(core.EvaluateFloor(42, 10099, 1, 5, lab5, 1, 600) == AccessResult::Granted); //override group

	int sf = 0, sn = 0;
	CHECK(AccessCore::ParseStationPoint("CallStation 0:1", sf, sn) && sf == 0 && sn == 1);
	CHECK(AccessCore::ParseStationPoint("call station -2 : 3", sf, sn) && sf == -2 && sn == 3);
	CHECK(!AccessCore::ParseStationPoint("CallStation 0", sf, sn));
	CHECK(!AccessCore::ParseStationPoint("CallStation 0:0", sf, sn));
	CHECK(AccessCore::StationPoint(-1, 2) == "CallStation -1:2");
	CHECK(std::string(AccessResultName(AccessResult::DeniedPassback)) == "Passback");

	//editing: schedules, groups, rules
	{
		AccessCore ed;
		std::string e;
		int m2 = 0;
		CHECK(AccessCore::FormatDays(0x3E) == "Mon-Fri");
		CHECK(AccessCore::FormatDays(0x41) == "Sun,Sat");
		CHECK(AccessCore::FormatDays(0x7F) == "Daily");
		CHECK(AccessCore::FormatDays(0x0A) == "Mon,Wed");
		CHECK(AccessCore::FormatDays(0x06) == "Mon,Tue");
		CHECK(AccessCore::ParseDays(AccessCore::FormatDays(0x41), m2) && m2 == 0x41);
		CHECK(AccessCore::ParseDays(AccessCore::FormatDays(0x5D), m2) && m2 == 0x5D);
		CHECK(ed.AddScheduleWindow("Day", 0x3E, 420, 1140, e));
		CHECK(ed.AddScheduleWindow("Day", 0x40, 540, 780, e));
		CHECK(AccessCore::FormatWindow(ed.GetSchedule("Day")->windows[0]) == "Mon-Fri, 07:00, 19:00");
		CHECK(AccessCore::FormatWindow(ed.GetSchedule("Day")->windows[1]) == "Sat, 09:00, 13:00");
		TimeWindow ww; ww.days_mask = 0x41; ww.start_minute = 0; ww.end_minute = 1440;
		CHECK(AccessCore::FormatWindow(ww) == "Sun|Sat, 00:00, 24:00");
		CHECK(AccessCore::FormatDays(0x41, '|') == "Sun|Sat");
		CHECK(ed.AddGroup("Staff", false, e));
		CHECK(ed.AddGroupRule("Staff", "Lobby Door", "Day", e));
		CHECK(ed.GetGroup("Staff")->rules[0].display == "Lobby Door");
		unsigned long v = ed.GetVersion();
		CHECK(!ed.RemoveSchedule("Day", e));		//in use
		CHECK(!ed.RemoveSchedule("Always", e));		//built in
		CHECK(ed.RemoveScheduleWindow("Day", 1, e) && ed.GetSchedule("Day")->windows.size() == 1);
		CHECK(ed.GetVersion() > v);
		Credential c; c.facility_code = 1; c.card_number = 2; c.groups = { "Staff" };
		CHECK(ed.AddCredential(c, e));
		CHECK(ed.Evaluate(1, 2, "Lobby Door", 1, 600) == AccessResult::Granted);
		CHECK(ed.SetGroupOverride("Staff", true) && ed.Evaluate(1, 2, "Elsewhere", 0, 0) == AccessResult::Granted);
		CHECK(ed.SetGroupOverride("Staff", false));
		CHECK(ed.RemoveGroupRule("Staff", 0, e) && ed.Evaluate(1, 2, "Lobby Door", 1, 600) == AccessResult::DeniedNoRule);
		CHECK(ed.RemoveSchedule("Day", e) && !ed.GetSchedule("Day"));
		CHECK(ed.RemoveGroup("Staff", e) && !ed.GetGroup("Staff") && ed.GetCredential(1, 2)->groups.empty());
	}

	if (failures == 0)
		printf("All access core tests passed\n");
	return failures == 0 ? 0 : 1;
}
