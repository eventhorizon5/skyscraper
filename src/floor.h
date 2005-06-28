//Scalable Building Simulator - Floor Class
//The Skyscraper Project - Version 1.1 Alpha
//Copyright ©2005 Ryan Thoryk
//http://www.tliquest.net/skyscraper
//http://sourceforge.net/projects/skyscraper
//Contact - ryan@tliquest.net

//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "globals.h"

class Floor
{
public:

//	ITVMesh Level; //level mesh
//	ITVMesh CrawlSpace; //crawl space mesh
//	ITVMesh ShaftsFloor; //walls around shafts
//	ITVMesh PipeShaft[]; //pipe shaft object
	bool Enabled; //is floor enabled?
	//bool StairDataTable;
	int FloorNumber; //floor number
	csString FloorName; //floor name
	float FloorAltitude; //floor altitude
	float FloorHeight; //floor height (not including crawl space height)
	float CrawlSpaceHeight; //height of crawl space

	//functions
	Floor(int num);
	~Floor();
	void SetCameraFloor();
	void AddFloor(csString texture, float x1, float z1, float x2, float z2, float voffset, float tw, float th);
	void AddCrawlSpaceFloor(csString texture, float x1, float z1, float x2, float z2, float voffset, float tw, float th);
	void AddWall(csString texture, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th);
	void AddCrawlSpaceWall(csString texture, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th);
	void CreateWallBox(csString texture, float WidthX, float LengthZ, float CenterX, float CenterZ, bool CSpace, float ResX, float ResY);
	void EnableFloor();
	void DisableFloor();

private:
//	ITVMesh CrawlSpaceShaft[];
//	ITVMesh Shafts[];
//	ITVMesh Stairs[];
//	ITVMesh FakeStairDoor[];
//	ITVMesh Objects[];
};
