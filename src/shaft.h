/* $Id$ */

/*
	Scalable Building Simulator - Shaft Subsystem Class
	The Skyscraper Project - Version 1.8 Alpha
	Copyright (C)2004-2010 Ryan Thoryk
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
	Ogre::Vector3 origin; //origin vector
	int startfloor; //starting floor
	int endfloor; //ending floor
	std::vector<int> elevators; //if elevator shaft, this is the list of elevators that use this shaft
	std::vector<int> stairs; //if stairwell, this lists the stairs that use this shaft
	bool InsideShaft; //true if user/camera is in the shaft
	float bottom; //shaft base
	float top; //shaft top
	Ogre::Vector2 cutstart; //cut starting vector
	Ogre::Vector2 cutend; //cut ending vector
	bool IsEnabled; //true if the entire shaft has been enabled
	bool ShowFloors; //true if floors should be shown while inside the shaft/elevator; floor list in ShowFloorsList
	bool ShowOutside; //true if outside should be shown while inside the shaft/elevator; floor list in ShowOutsideList
	std::vector<int> ShowFloorsList; //list of floors to enable while inside the shaft/elevator
	std::vector<int> ShowOutsideList; //list of floors that the outside should be enabled on
	bool ShowFullShaft; //if true, always show full shaft during elevator movement instead of only a selected range

	Shaft(int number, int type, float CenterX, float CenterZ, int _startfloor, int _endfloor);
	~Shaft();
	WallObject* AddWall(int floor, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th);
	WallObject* AddFloor(int floor, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, float tw, float th);
	void Enabled(int floor, bool value, bool EnableShaftDoors);
	bool IsShaft(Ogre::Mesh test);
	void EnableWholeShaft(bool value, bool EnableShaftDoors, bool force = false);
	bool IsInShaft(const Ogre::Vector3 &position);
	void CutFloors(bool relative, const Ogre::Vector2 &start, const Ogre::Vector2 &end, float startvoffset, float endvoffset);
	bool CutWall(bool relative, int floor, const Ogre::Vector3 &start, const Ogre::Vector3 &end, int checkwallnumber = 0, const char *checkstring = "");
	void EnableRange(int floor, int range, bool value, bool EnableShaftDoors);
	bool IsEnabledFloor(int floor);
	void AddShowFloor(int floor);
	void RemoveShowFloor(int floor);
	void AddShowOutside(int floor);
	void RemoveShowOutside(int floor);
	bool IsValidFloor(int floor);
	void AddElevator(int number);
	void RemoveElevator(int number);
	MeshObject* GetMeshObject(int floor);
	void Report(const char *message);
	bool ReportError(const char *message);
	Object* AddLight(int floor, const char *name, int type, Ogre::Vector3 position, Ogre::Vector3 direction, float radius, float max_distance, float color_r, float color_g, float color_b, float spec_color_r, float spec_color_g, float spec_color_b, float directional_cutoff_radius, float spot_falloff_inner, float spot_falloff_outer);
	Object* AddModel(int floor, const char *name, const char *filename, Ogre::Vector3 position, Ogre::Vector3 rotation, float max_render_distance = 0, float scale_multiplier = 1);

private:
	std::vector<MeshObject*> ShaftArray; //shaft mesh array
	std::vector<bool> EnableArray;
	bool EnableCheck;

	//cache objects for IsInShaft()
	Ogre::Vector3 lastposition;
	bool lastcheckresult;
	bool checkfirstrun;

	//lights
	std::vector<std::vector<Light*> > lights;

	//Models
	std::vector<std::vector<Model*> > ModelArray;

	char intbuffer[65];
	char buffer[20];
};

#endif
