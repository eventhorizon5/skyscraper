/* $Id$ */

/*
	Scalable Building Simulator - Shaft Subsystem Class
	The Skyscraper Project - Version 1.6 Alpha
	Copyright (C)2005-2009 Ryan Thoryk
	http://www.skyscrapersim.com
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@tliquest.net

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

#ifndef _SBS_SHAFT_H
#define _SBS_SHAFT_H

class SBSIMPEXP Shaft
{
public:

	Object *object; //SBS object
	int ShaftType; //type of shaft
	int ShaftNumber; //shaft number
	csVector3 origin; //origin vector
	int startfloor; //starting floor
	int endfloor; //ending floor
	csArray<int> elevators; //if elevator shaft, this is the list of elevators that use this shaft
	csArray<int> stairs; //if stairwell, this lists the stairs that use this shaft
	bool InsideShaft; //true if user/camera is in the shaft
	float bottom; //shaft base
	float top; //shaft top
	csVector2 cutstart; //cut starting vector
	csVector2 cutend; //cut ending vector
	bool IsEnabled; //true if the entire shaft has been enabled
	bool ShowFloors; //true if floors should be shown while inside the shaft/elevator; floor list in ShowFloorsList
	bool ShowOutside; //true if outside should be shown while inside the shaft/elevator; floor list in ShowOutsideList
	csArray<int> ShowFloorsList; //list of floors to enable while inside the shaft/elevator
	csArray<int> ShowOutsideList; //list of floors that the outside should be enabled on
	bool ShowFullShaft; //if true, always show full shaft during elevator movement instead of only a selected range
	csArray<WallObject*> shaft_walls;

	Shaft(int number, int type, float CenterX, float CenterZ, int _startfloor, int _endfloor);
	~Shaft();
	WallObject* AddWall(int floor, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th);
	WallObject* AddFloor(int floor, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, float tw, float th);
	void Enabled(int floor, bool value, bool EnableShaftDoors);
	bool IsShaft(csRef<iMeshWrapper> test);
	void EnableWholeShaft(bool value, bool EnableShaftDoors, bool force = false);
	bool IsInShaft(const csVector3 &position);
	void CutFloors(bool relative, const csVector2 &start, const csVector2 &end, float startvoffset, float endvoffset);
	bool CutWall(bool relative, int floor, const csVector3 &start, const csVector3 &end, int checkwallnumber = 0, const char *checkstring = "");
	void EnableRange(int floor, int range, bool value, bool EnableShaftDoors);
	bool IsEnabledFloor(int floor);
	void AddShowFloor(int floor);
	void RemoveShowFloor(int floor);
	void AddShowOutside(int floor);
	void RemoveShowOutside(int floor);
	bool IsValidFloor(int floor);
	void AddElevator(int number);
	void RemoveElevator(int number);

private:
	csRefArray<iMeshWrapper> ShaftArray; //shaft mesh array
	csRefArray<iThingFactoryState> ShaftArray_state; //shaft mesh array state
	csArray<bool> EnableArray;
	bool EnableCheck;

	//cache objects for IsInShaft()
	csVector3 lastposition;
	bool lastcheckresult;
	bool checkfirstrun;

	char intbuffer[65];
	char buffer[20];
};

#endif
