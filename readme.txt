Skyscraper version 1.1
©2005 Ryan Thoryk
http://www.tliquest.net/skyscraper
http://sourceforge.net/projects/skyscraper
email: ryan@tliquest.net

Legal Notice
------------------------------

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

Release Notes
------------------------------
This 1.0 release is a minor milestone release, and was forked from the stable 0.96 code due to the
instability of the newer simulation engine that's being implemented (in 0.97, renamed 1.1 alpha)
Don't expect this version to be complete in any way; it is simply a stable version of the original
simulation engine.  Newer features, a completely rewritten sim engine, loadable buildings, better graphics,
etc will all be developed for a 2.0 release.

Note - starting with version 0.95b, the TrueVision 3D 6.2 engine needs to be installed. Version 0.95 required TrueVision3D 6.1b, Version 0.94 required 6.0 and releases prior to 0.93 used version 6.0-RC3.

Skyscraper is a 3D virtual building simulator written in Visual Basic 6.0 using the
TrueVision3D graphics library which can be found at http://www.truevision3d.com.

The current building used in the simulator is the Triton Center, designed by Ryan Thoryk,
which originally got it's inspiration from the 137-story Glass Tower in the movie The Towering
Inferno, and 7 South Dearborn, a 112-story proposed building for downtown Chicago.
Original models (2D and 3D) of the Triton Center were made in MyHouse for Windows 6.5.
The building's features and locations of equipment currently don't aim to be 100% accurate
or even realistic, but they might be changed in newer models of the Triton Center.

See the changelog for new features and fixes for this release.

--------------------

Important keys used in the game:
5 - play elevator music (disabled in this release)
6 - stop elevator music (disabled in this release)
F1 - to take a screenshot (saves as c:\shot.bmp)
PgUp - Look upwards (warning - there's alignment problems with this)
PgDown - Look downwards (same problem with above)
Home - Float upwards (jump if collision detection is on)
End - Float downwards
Space - Straight out viewpoint
Z - Run (hold down Z while moving)

Current building floor layout:

B10-B1 - Parking lot, shops, etc
1 - 3 story lobby
M - Mezzanine inside the 3 story lobby
2-9 - ?
10-39 - Offices, section 1
40 - Office Skylobby
41-79 - Offices, section 2
80 - Residential Skylobby
81-99 - Hotel
100-114 - Apartments
115-117 - Residential service floors and maintenance
118-129 - Condos
130-131 - Machinery/Electrical
132 - Observatory
133 - Maintenance/Water Tanks
134 - Pool and recreation area
135 - 2 story ballroom with surrounding balcony
136 - balcony of the 135th floor ballroom
137 - Mechanical
138 - Roof w/Heliport and radio/tv towers

-need office maintenance floor, possibly 2
-make either 99 or 100 housekeeping
