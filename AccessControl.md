# Skyscraper Access Control Module

HID-style card access control for [Skyscraper](https://www.skyscrapersim.net): badges, card readers, door strikes, elevator floor lockout, badge-driven destination dispatch, secured zones with anti-passback, time profiles and access levels, an event log, and an Access Control window to manage it all.

Built against `eventhorizon5/skyscraper` master at commit `385d53af` (September 19, 2026). Licensed under the GNU GPL v2 or later, like Skyscraper itself.

## Contents

- [Features](#features)
- [Files](#files)
- [Installing and building](#installing-and-building)
- [Sample buildings](#sample-buildings)
- [How access is decided](#how-access-is-decided)
- [Floor labels](#floor-labels)
- [Script reference](#script-reference)
- [Doors](#doors)
- [Card readers](#card-readers)
- [Elevator floor lockout](#elevator-floor-lockout)
- [Destination dispatch call stations](#destination-dispatch-call-stations)
- [Secured zones and anti-passback](#secured-zones-and-anti-passback)
- [Virtual people](#virtual-people)
- [Access Control window](#access-control-window)
- [Console commands](#console-commands)
- [The event log and deny reasons](#the-event-log-and-deny-reasons)
- [Testing status](#testing-status)
- [Known limits](#known-limits)

## Features

| Area | What it does |
| --- | --- |
| **Doors** | Doors join an access point. A valid badge releases the lock for a set time, then it relocks. Free egress from one side, auto-close, held-open and forced-open alarms. |
| **Card readers** | Wall readers, and readers built into elevator and call station button panels. The light shows the lock state: red is locked, green is unlocked, and it alternates red and green for an invalid badge. |
| **Elevators** | Secured floors need a badge tapped at the in-car reader. Service modes (independent, fire, inspection) override lockout. Optional auto-select. |
| **Destination dispatch** | Tap at a lobby call station and a car is called to your home floor. Secured destinations typed on the keypad need a badge first. |
| **Zones** | Areas such as a tenant floor or server room. Tracks who's inside, raises an alarm for unbadged entry, and enforces hard or soft anti-passback. |
| **People** | Virtual people carry badges in chosen access levels and obey the rules. |
| **Access model** | Time profiles (schedules), access levels (groups) and their rules, cards, lockdown, and override levels. |
| **Management** | Access Control window (F12): cards, access levels, time profiles, live status, live log, access checks, USB badge reader input, and export back to script. |
| **Badges** | Your wallet of badges. Press G or right-click a reader to switch the active badge. |
| **Log** | Every grant, denial, alarm and change, with the reason, exportable to CSV. |

## Files

| Path | Status | Purpose |
| --- | --- | --- |
| `src/sbs/controllers/accesscore.h/.cpp` | New | Decision logic with no Ogre dependency: cards, access levels and rules, time profiles, floor label matching, 26-bit Wiegand, event log. |
| `src/sbs/controllers/access.h/.cpp` | New | `AccessController` (one per sim engine): access points and door strikes, readers, elevator lockout, call stations, zones, virtual people's badges, export. |
| `src/sbs/objects/cardreader.h/.cpp` | New | The clickable card reader and its lights. |
| `src/script/access.cpp` | New | The `<Access>` section and all access script and console commands. |
| `src/gui/accesscontrol.h/.cpp` | New | The Access Control window. |
| `src/sbs/system/lock.h/.cpp` | Changed | `DoorLock` can belong to an access point. |
| `src/sbs/objects/door.cpp` | Changed | Opening a door notifies its access point. |
| `src/sbs/objects/elevator.cpp` | Changed | `SelectFloor` checks secured floors. |
| `src/sbs/objects/elevatorcar.cpp` | Changed | Readers in a car hide and show with it. |
| `src/sbs/objects/buttonpanel.h/.cpp` | Changed | Grid positions for panel readers; panel readers hide with and are deleted with their panel. |
| `src/sbs/objects/callstation.cpp` | Changed | Secured destinations need a badge at the station. |
| `src/sbs/objects/person.cpp` | Changed | Virtual people tap badges for secured floors. |
| `src/sbs/sbs.h/.cpp` | Changed | Owns the access controller; creates it on first use and deletes it first on shutdown. |
| `src/script/section.h/.cpp`, `scriptproc.h/.cpp` | Changed | Registers the `<Access>` section and new settings. |
| `src/script/commands.cpp`, `floors.cpp`, `elevatorcars.cpp` | Changed | Every door-creation command honors `SetAccessPoint`. |
| `src/gui/debugpanel.h/.cpp` | Changed | Adds the **Access Control** button to the control panel. |
| `src/frontend/mainscreen.h/.cpp`, `keyboard.ini` | Changed | The **G** key switches badges. |
| `buildings/Simple - Access Control.bld` | New | Sample: secured stair doors, secured elevator floors, a secured zone with an alarm. |
| `buildings/Simple - Access Control DD.bld` | New | Sample: destination dispatch with badge readers on every call station and in the car. |
| `data/reader_*.png`, `data/reader_panel_*.png` | New | Reader textures: portrait for walls, landscape for button panels. |
| `tools/accesstest/accesstest.cpp` | New | Unit tests for the decision logic. |

## Installing and building

### Get the files in

Use either method:

- **Patch:** from the root of a Skyscraper checkout at commit `385d53af`, run `git apply --3way skyscraper-access-control.patch`.
- **Copy:** unzip `skyscraper-access-control-files.zip` over the checkout. It contains every new and changed file in place. On Windows, choose **Replace** when asked; on macOS, merge folders rather than replacing them.

Copying whole files over a **newer** Skyscraper version can undo the maintainer's later changes to the 13 changed files, so start from commit `385d53af`, or use the patch with `--3way`.

### Build

Follow Skyscraper's `Compiling.md`. Two things are specific to this module:

- **Re-run CMake (Configure, then Generate) after adding the files.** The build finds source files with CMake's file globbing, so new `.cpp` files are only picked up when CMake runs. No `CMakeLists.txt` change is needed.
- The module adds nothing new to link against.

### Windows notes (Visual Studio 2026)

These came up while building on Windows with Visual Studio 18 (2026). None of them are caused by this module, but they're needed to build Skyscraper there.

| Problem | Fix |
| --- | --- |
| "Visual Studio 18 2026" isn't in CMake's generator list | Update CMake to 4.2 or later. Choose platform **x64**. |
| `CMAKE_INSTALL_PREFIX` shows `Program Files (x86)` | Check the build is set up for **x64**, not Win32. If not, use File → Delete Cache and configure again. |
| `Cannot open include file: 'btBulletCollisionCommon.h'` | Point `BULLET_INCLUDE_DIR` at the folder that actually contains that file, often `C:/bullet/include/bullet`. |
| Bullet libraries | Use `BulletCollision_RelWithDebugInfo.lib` (not `Bullet3Collision`), `BulletDynamics_...`, `LinearMath_...` and `BulletSoftBody_...`. Point to the files, not the folder. Leave the `_DEBUG` entries as NOTFOUND. |
| `Cannot open include file: 'fmod.hpp'` | Set `FMOD_INCLUDE_DIR` to the FMOD Engine's `api/core/inc` folder, and `FMOD_LIBRARY` to `api/core/lib/x64/fmod_vc.lib`. |
| `STL1011 ... experimental/coroutine` errors in the OpenXR project | Add `/D_SILENCE_EXPERIMENTAL_COROUTINE_DEPRECATION_WARNINGS` to the end of `CMAKE_CXX_FLAGS` (tick Advanced to see it). |
| Linker errors mixing Debug with the prebuilt libraries | Build **RelWithDebInfo**, which matches the prebuilt dependencies. |
| "Entry Point Not Found ... FMOD ... SBS.dll" when starting | Copy `fmod.dll` from the FMOD Engine's `api/core/lib/x64` folder into the Skyscraper folder, replacing the older one from `skyscraper_dlls.zip`. |
| F5 tries to run ALL_BUILD | Right-click the **skyscraper** project and choose **Set as Startup Project**, or run `skyscraper.exe` directly. |

### Unit tests

The decision logic can be tested without building Skyscraper:

```
cd tools/accesstest
g++ -std=c++17 -DACCESSCORE_STANDALONE -I../../src/sbs/controllers accesstest.cpp ../../src/sbs/controllers/accesscore.cpp -o accesstest
./accesstest
```

## Sample buildings

Load these from the main menu with **Other Building**.

### Simple - Access Control

- **Stair doors (floors labeled 2–10):** locked from the stairwell side, with free egress from the floor side. To get back onto a floor from the stairs, badge at the reader beside the door. The top stair door also keeps its original key lock.
- **Elevator 1:** floors labeled **6–9** are secured. Badge at the reader in the bottom row of the button panel, then press your floor within 10 seconds.
- **Floor labeled 6:** a secured zone with hard anti-passback. Arriving without a badge, for example on independent service, rings an alarm bell until you leave.
- **Doors** auto-close 8 seconds after opening.
- **Virtual people** get Staff badges.

### Simple - Access Control DD

- Every floor's destination dispatch call station has a reader under its keypad. Elevator 1's floors labeled 6–9 are secured.
- **Jane's** tap calls a car to her home floor, 6. **Bob's** calls one to 4.
- The **night guard's** tap unlocks every secured floor for 10 seconds, to choose on the keypad.
- The **Visitor** badge is refused.
- Typing **7** without a badge shows `??`.

### Badges in both samples

| Badge | Holder | Access |
| --- | --- | --- |
| 42:10001 | Jane Smith | Staff (07:00 to 19:00), plus floor 6 and its stair door at any time; home floor 6 |
| 42:10002 | Bob Lee | Staff (07:00 to 19:00); home floor 4 |
| 42:10050 | Visitor (DD sample only) | No access |
| 42:10099 | Night Guard | Security override: everywhere, any time, even in lockdown; home floor 1 |

Use the sky time (F12 → Sky Control) to test the business-hours schedule.

## How access is decided

Access is built from four things:

1. **Time profiles** (schedules) say *when*. Each is one or more windows of days plus a start and end time.
2. **Access levels** (groups) contain **rules**. Each rule pairs a *place* with a time profile. A place is a door access point name, or elevator floors such as `Elevator 1:Floors 6-9`.
3. **Cards** belong to one or more access levels. A card gets everything any of its levels allow.
4. An **override** level opens everything at any time, including during lockdown and despite anti-passback.

When a badge is presented, the checks run in this order:

1. Is the card enrolled? If not, `UnknownCard`.
2. Is it enabled? If not, `Disabled`.
3. Does it have an override level? If so, it's granted.
4. Is the building in lockdown? If so, `Lockdown`.
5. Does a rule cover this place? If not, `NoRule`.
6. Is that rule's time profile active now? If not, `Schedule`.
7. Would entry break anti-passback? If so, `Passback`.

The clock is the sky system's virtual date and time, the same clock as the `%hour%` and `%minute%` script variables. Without a running sky, the system clock is used.

Names of time profiles, access levels and places ignore case and spaces.

## Floor labels

Wherever floors appear in access control (`SecureFloors`, elevator rules, `ZoneEntry`, home floors, the log), they're written as **the labels on the buttons and keypads**, not Skyscraper's internal floor numbers. In the samples, internal floor 0 (the lobby) is labeled 1, so the floors labeled 6–9 are internal floors 5–8.

| Written as | Matches |
| --- | --- |
| `6`, `6-9`, `-2 to -1` | Floors whose label is that number, or a number in that range. A floor's ID and its keypad NumberID both count. |
| `R`, `L`, `B2` | That text label exactly, ignoring case. |
| `6-9\|R` | Several items joined with `\|`. |
| `@5-8` | Internal floor numbers, as used by `ServicedFloors` and floor sections. |

Labels are matched when they're used, so the `<Access>` section can come before the floors are created. `access elevators` shows which floors each list matched.

## Script reference

### The `<Access>` section

```
<Access>
    AutoClose 8                                        #close access-controlled doors after 8 s (0 = off)
    StationUnlock 10                                   #seconds a call-station tap allows keypad entry

    #time profiles
    Schedule Business Hours, Mon-Fri, 07:00, 19:00
    Schedule Business Hours, Sat, 09:00, 13:00         #repeat a name to add a window
    Schedule Night Shift, Daily, 22:00, 06:00          #overnight is fine

    #elevator floor lockout
    SecureFloors 1, 6-9, 10                            #elevator, floors, unlock seconds[, autoselect]

    #access levels and rules
    Group Staff
    GroupRule Staff, Lobby Door, Business Hours
    GroupRule Staff, Elevator 1:Floors 7-9, Business Hours
    Group Security, override

    #virtual people
    PeopleAccess Staff
    ServicePeopleAccess Staff|Security

    #cards and the player's wallet
    Card 42, 10001, Jane Smith, Staff, 6               #facility, number, holder, levels, home floor
    WalletAdd 42, 10001
<EndAccess>
```

| Command | Syntax | Notes |
| --- | --- | --- |
| `Schedule` | `name, days, start, end` | Days: `Mon-Fri`, `Sat,Sun` or `Sat\|Sun`, `Fri-Mon` (wraps), `Daily`, `Weekdays`, `Weekends`. Times: 24-hour `HH:MM`; `24:00` is allowed as an end time. `Always` and `Never` are built in. |
| `Group` | `name[, override]` | An access level. `override` opens everything at any time. |
| `GroupRule` | `level, place[, schedule]` | Place is a door access point name or `Elevator N:Floors <labels>`. The schedule defaults to `Always`. |
| `Card` | `facility, number, holder, levels[, home_floor]` | Facility 0–255, number 0–65535. Levels are separated by `\|`. The home floor is a label (or `@N`). |
| `WalletAdd` | `facility, number` | Gives the player this badge. |
| `SecureFloors` | `elevator, floors[, unlock_seconds[, autoselect]]` | Default unlock 10 s. Repeat to add floors. |
| `StationUnlock` | `seconds` | Default 10. |
| `AutoClose` | `seconds` | Default auto-close for all access-controlled doors; 0 = off. |
| `PeopleAccess` | `levels` | Access levels for virtual people's badges. |
| `ServicePeopleAccess` | `levels` | The same, for service-access people. |

### Placement commands (usable in any section)

| Command | Syntax | Notes |
| --- | --- | --- |
| `SetAccessPoint` | `name, unlock_seconds[, held_seconds[, autoopen[, autoclose_seconds]]]` or `none` | Doors created after it join this access point. Held default 30 s (0 = no held-open alarm). `autoopen` opens the door on grant. `autoclose_seconds` overrides `AutoClose` for this point (0 = off). |
| `AddCardReader` | `place, grant_sound, deny_sound, idle_tex, green_tex, red_tex, direction, centerx, centerz, width, height, voffset` | A wall reader. In a Floor section, voffset is from the floor base. In an Elevator or Car section, the position is relative to the car (use `Elevator N` as the place). Use `CallStation F:N` to make a station reader. |
| `AddPanelReader` | `panel, grant_sound, deny_sound, idle_tex, green_tex, red_tex, row, column, width, height[, hoffset, voffset]` | A reader in a button panel, placed like `AddButton` (sizes in buttons). Use it after `CreatePanel` in an Elevator, Car or CallStation section; in a CallStation section the panel number is ignored. |
| `AddAccessZone` | `name, x1, y1, z1, x2, y2, z2, passback[, alarm_on_action, alarm_off_action]` | Passback is `Off`, `Soft` or `Hard`. In a Floor section, y is from the floor base. |
| `ZoneEntry` | `zone, entry` | Entry is a door access point name, or `Floor <labels>` for elevator and call station arrivals. Use it after `AddAccessZone`. |

Example: a stair door that's free from the floor side and badge-controlled from the stairwell side, with a reader:

```
<Floor 3>
    SetLock 2, 0                          #lock only the stairwell side
    SetAccessPoint Stair Reentry 3, 5, 30
    AddStairsDoor ...                     #any door command
    SetAccessPoint none
    SetLock 0, 0
    AddCardReader Stair Reentry 3, beep1.wav, buzz.wav, ReaderIdle, ReaderGranted, ReaderDenied, right, 1.7, 17.3, 0.35, 0.5, 4
<EndFloor>
```

## Doors

- **Locking:** a door on an access point must be locked. If `SetLock` wasn't used before it, both sides are locked. Use `SetLock 1` or `SetLock 2` first to leave one side as free egress.
- **Grant:** a valid badge releases the lock for the unlock time. Open the door within that window, or it opens by itself with `autoopen`. When the window ends, the lock applies again; an open door can still be closed.
- **Controller unlock:** **Hold Unlocked** (Access Control window, or `access unlock`) keeps a door unlocked until **Return to Normal**. In lockdown, held-unlocked doors lock again.
- **Auto-close:** an access-controlled door closes itself that many seconds after it starts opening. Reopening restarts the timer. Like Skyscraper's own door timers, it doesn't detect someone standing in the doorway.
- **Alarms:**
  - `HeldOpen`: a door is left open past the held time.
  - `ForcedOpen`: a door is opened without a grant, for example by a script action. Opening from a free-egress side isn't an alarm.
- **Keys:** Ctrl+Shift-click can't change the lock of a door on an access point; the access controller decides.
- Existing doors without access points behave exactly as before.

## Card readers

- **Click** a reader to present your active badge. **Right-click** it, or press **G** anywhere, to switch badges.
- **Lights** show the lock state, rechecked ten times a second:

| Light | Meaning |
| --- | --- |
| Solid red | Locked |
| Solid green | Unlocked by a valid badge, or held unlocked by the controller. For elevator and call station readers, secured floors are released by a badge tap, or the car is in a service mode that ignores lockout. |
| Alternating red and green (2 s) | An invalid badge (unknown, disabled, no rule, wrong time, lockdown, anti-passback), or a secured floor pressed or typed without a badge |

- **Textures:** the reader commands take three textures. The **green** one is the unlocked light and the **red** one the locked light. The **idle** one only shows if the reader's place doesn't exist, which usually means a typo.
- **Sounds:** the grant sound plays for a valid badge, the deny sound for an invalid one.
- **Panel readers** take the panel's facing direction, ride with the car, and hide with the panel. Wall readers in a car ride with the car.

## Elevator floor lockout

- `SecureFloors` lists the floors in an elevator that need a badge. Other floors work as before.
- **Pressing a secured floor** without a badge does nothing (the button doesn't light), and the car's readers flash red and green.
- **Tapping a badge** at the car's reader releases every secured floor that badge allows, for the unlock time. Selecting one uses up the release, so each tap is good for one floor.
- **Autoselect:** with `autoselect` set, a badge that allows exactly one secured floor selects it automatically.
- **Service modes** override lockout: independent service, fire service phases 1 and 2, and inspection.
- **Keypad elevators** use the same check.
- **Lockdown** refuses secured floors to everyone except override levels.
- **Not yet covered:** hall call buttons on secured floors. Skyscraper's existing `SetLock` with a key still works on them.

## Destination dispatch call stations

- **Station readers:** put one on a call station with `AddPanelReader` in its CallStation section. The station's place name is `CallStation F:N` (internal floor, station number).
- **Home floor:** a badge with a home floor it may use calls a car there straight away, and the station shows which elevator to take.
- **No usable home floor** (none set, it's the current floor, or not allowed): a tap releases the badge's allowed secured floors for `StationUnlock` seconds, to type on the keypad. One selection per tap.
- **Which destinations are secured:** any secured floor of any elevator the station's dispatch controller uses. A badge may choose it if allowed on any of those elevators.
- **Refusals:**
  - Typing a secured destination without a badge shows `??` and flashes the station reader.
  - Unknown, disabled or no-access badges show `XX`.
- **No second tap:** cars sent by destination dispatch don't need an in-car badge for that trip, since the station already checked, as in real systems.

## Secured zones and anti-passback

A zone is a box-shaped area, such as a tenant floor or server room, with badge-controlled ways in.

```
<Floor 5>
    AddSound ZoneAlarm, bell1.wav, 0, 8, 0, false
    AddAccessZone Tenant Floor 6, -20, 0, -20, 20, 10, 20, Hard, ZoneAlarmOn, ZoneAlarmOff
    ZoneEntry Tenant Floor 6, Stair Reentry 5
    ZoneEntry Tenant Floor 6, Floor 6
<EndFloor>
AddAction ZoneAlarmOn, Floor 5, PlaySound, ZoneAlarm, true
AddAction ZoneAlarmOff, Floor 5, StopSound, ZoneAlarm
```

- **Badged entry:**
  - A badge granted at one of the zone's entries counts for 60 seconds. An entry is its door, or a secured floor reached with a badge.
  - Walking in within that time logs `ENTER` with that badge.
  - Only badges you carry count for you.
- **Unbadged entry:**
  - Walking in any other way logs `UnbadgedEntry`, raises the zone alarm and runs the alarm-on action. Examples are tailgating, a propped door, or independent service.
  - The alarm clears and runs the alarm-off action when you leave, or with **Reset Zone** or `access resetzone`.
- **Inside** means within the box, but not riding an elevator or in a stairwell that passes through it.
- **Anti-passback:** once a badge enters, it can't enter again until its holder leaves.
  - `Hard` refuses the badge (`Passback`). For elevators and stations, floors leading into the zone are removed from that badge's choices.
  - `Soft` allows it but logs an alarm.
  - A grant that's never followed by walking in lapses after 60 seconds.
  - Override levels are exempt.
- **Occupancy** shows in `access zones` and in the Access Control window.
- **Starting position:** don't put the starting camera inside a zone, or the unbadged-entry alarm goes off at load.

## Virtual people

- **Getting badges:** with `PeopleAccess` (and optionally `ServicePeopleAccess`) set, each virtual person gets a badge the first time they need one. It has facility code 0, is numbered from 1, carries the person's name, and is in the listed access levels.
- **Using them:** people tap automatically at call stations and in cars when going to a secured floor. Their taps show in the log, and they give up a trip they aren't allowed to make.
- **Without badges:** people without badges also give up trips to secured floors, rather than getting stuck.
- **Zones:** virtual people don't have positions within a floor, so they don't count toward zone occupancy or set off zone alarms.

## Access Control window

Open the control panel (**F12**) and click **Access Control**.

- The window follows the active building and refreshes about twice a second.
- It's greyed out for buildings without an `<Access>` section.
- The top line shows the building, your active badge, and lockdown.

| Tab | What it does |
| --- | --- |
| **Cards** | Every card with holder, access levels, home floor, status and wallet. Select a card to edit holder, home floor, and its **access levels as a tick list**. **Add / Update Card**, **Enable**, **Disable (lost badge)**, **Give Me This Badge**, **Take Badge Back**, **Use This Badge**. **Export as Script** writes the whole `<Access>` section. |
| **Access Levels** | Create and delete levels; switch **override** on or off. Select a level to see its rules, and add rules (choose a place and a time profile; elevator places can be edited, such as `Elevator 1:Floors 7-9`) or remove them. A rule for a new door name creates that access point. **Check access right now** tells you whether a card would get into a place at the current sky time and why, without unlocking anything or logging. |
| **Time Profiles** | Create and delete profiles; add a time window by ticking days and entering start and end times, or remove one. Overnight windows are fine. A profile still used by a rule can't be deleted. |
| **Doors, Elevators and Zones** | Live status of every door (locked, released, held open, forced open, auto-close), secured elevator and zone. **Momentary Unlock**, **Hold Unlocked**, **Return to Normal**, auto-close for all doors, **Reset Zone**, and **building lockdown**. |
| **Live Log** | Every event, newest first; granted is green, denied red, alarms orange. Filter by any text. **Export CSV**. |
| **Badge Reader Input** | Present badges by typing, or with a **USB badge reader** in keyboard-emulation mode. Choose where to present and the reader's format: `facility:card`, card number only (with a default facility), or a raw 26-bit Wiegand value in decimal or hex (parity is checked). An unknown badge is logged with its number, so to enroll a real badge, tap it once and add that number on the Cards tab. |

**Keeping changes:** changes made in the window last until the building is reloaded. To keep them, use **Export as Script** on the Cards tab and replace the building's `<Access>` section with the exported one. Zones, door access points and readers stay in the building file and aren't part of the export.

**Switching badges:** press **G**, or change the key with `Skyscraper.Frontend.Keyboard.Badge` in `keyboard.ini`.

## Console commands

Type these in the console (F12 → Console), or use them in scripts.

| Command | Effect |
| --- | --- |
| `access help` | List commands |
| `access cards` / `points` / `elevators` / `zones` / `wallet` | List cards, doors with status, secured elevators, zones, or your badges |
| `access next` | Switch to the next badge |
| `access tap place` | Present your active badge at a place (a door, `Elevator N`, or `CallStation F:N`) |
| `access present place, facility, number` | Present any card |
| `access addbadge facility, number` | Add a badge to your wallet |
| `access disable facility, number` / `enable` | Revoke or restore a card |
| `access grant door` / `exit door` | Momentary release (guard button, request to exit) |
| `access unlock door` / `lock door` | Hold unlocked, or return to normal |
| `access lockdown on` / `off` | Building lockdown |
| `access autoclose seconds` | Auto-close for all doors (0 = off) |
| `access autoclose door, seconds` | One door; `default` follows the global setting |
| `access resetzone zone` / `all` | Clear zone alarms and anti-passback |
| `access log [count]` | Recent events (default 20) |
| `access export filename` | Write the log to CSV |

## The event log and deny reasons

Every event records the sky time, the event type, where it happened, the card, the holder and a detail. Event types are `GRANTED`, `DENIED`, `ALARM`, `ENTER`, `EXIT` and `INFO`. The log keeps the last 5,000 events.

| Deny reason | Meaning | What to change |
| --- | --- | --- |
| `UnknownCard` | The card isn't enrolled | Add it (Cards tab, or a `Card` line) |
| `Disabled` | The card was revoked | **Enable** it |
| `NoRule` | None of the card's access levels covers this place | Add a rule, or give the card another level |
| `Schedule` | A rule exists, but its time profile isn't active now | Check the profile and the sky time |
| `Lockdown` | The building is in lockdown | Turn lockdown off, or use an override level |
| `Passback` | Anti-passback: the badge already entered and hasn't left | Leave the zone, or **Reset Zone** |

Other details you'll see:

| Detail | Meaning |
| --- | --- |
| `BadgeRequired Floor 7` | A secured floor was typed at a call station without a badge |
| `FloorSelected 7` | A released secured floor was selected |
| `HomeFloor 6` | A station tap called a car to the home floor |
| `UnbadgedEntry`, `HeldOpen`, `ForcedOpen`, `Passback at ...` | Alarms |

## Testing status

- **Unit tests:** the decision logic passes its unit tests. They cover time windows including overnight, day parsing and formatting, every deny reason, lockdown and override, floor label matching, elevator rules, call station names, editing profiles, levels and rules, 26-bit Wiegand encoding and parity, and log capacity and CSV export.
- **Compile check:** every new and changed `.cpp` passes a compile check with the project's include paths, against OGRE 1.12, wxWidgets 3.2 and Bullet headers.
- **Windows build:** the full module builds and runs on Windows with Visual Studio 2026, OGRE 14.3.4 and the prebuilt dependencies.
- **Confirmed in the simulator:**
  - doors and readers, auto-close, and the console commands
  - elevator floor lockout, with readers on the wall and in the button panel
  - destination dispatch badge readers, home floors, and `??` and `XX` refusals
- **Built but not yet confirmed in the simulator:**
  - floor labels
  - zones and anti-passback
  - virtual people's badges
  - the Access Control window and its tabs
  - the G key and USB reader input
  - the red, green and alternating reader lights

## Known limits

- **On-screen indicator:** there isn't one for the active badge in the 3D view. It shows in the console and the Access Control window.
- **Badge models:** there are no badges to pick up in the world; badges come from `WalletAdd` or the Access Control window.
- **Hall call buttons:** those on secured floors aren't badge-controlled yet; use `SetLock` with a key.
- **Actions:** access points aren't `AddAction` parents yet, so an exit button uses a script command (`access exit door`) rather than an action.
- **Placement:** zones, door access points and readers are placed in the building file, not the Access Control window.
- **Per building:** each building in a city has its own cards and settings; cities don't share them.
- **Virtual people** don't have positions within a floor, so zone occupancy only counts you.
