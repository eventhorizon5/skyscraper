/*
	Scalable Building Simulator - Floor Class
	The Skyscraper Project - Version 1.1 Alpha
	Copyright ©2005-2006 Ryan Thoryk
	http://www.tliquest.net/skyscraper
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

#include "globals.h"

class Floor
{
public:

	csRef<iMeshWrapper> Level; //level mesh
		csRef<iMeshObject> Level_object;
		csRef<iMeshObjectFactory> Level_factory;
		csRef<iThingFactoryState> Level_state;

	csRef<iMeshWrapper> Interfloor; //interfloor mesh
		csRef<iMeshObject> Interfloor_object;
		csRef<iMeshObjectFactory> Interfloor_factory;
		csRef<iThingFactoryState> Interfloor_state;

	int Number; //floor number
	csString Name; //floor name
	csString ID;
	csString FloorType;
	csString Description;
	float Altitude; //floor altitude
	float Height; //floor height (not including interfloor height)
	float InterfloorHeight; //height of interfloor area
	bool IsEnabled; //is floor enabled?

	//functions
	Floor(int number, const char *name);
	~Floor();
	void SetCameraFloor();
	void AddFloor(const char *texture, float x1, float z1, float x2, float z2, float voffset, float tw, float th);
	void AddInterfloorFloor(const char *texture, float x1, float z1, float x2, float z2, float voffset, float tw, float th);
	void AddWall(const char *texture, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float voffset1, float voffset2, float tw, float th);
	void AddInterfloorWall(const char *texture, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float voffset1, float voffset2, float tw, float th);
	void Enabled(bool value);
	void AddAutoFloor(const char *texture);
	float FullHeight();

private:
	csRefArray<iMeshWrapper> Objects;
};
