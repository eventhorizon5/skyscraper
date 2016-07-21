/* $Id$ */

/*
Scalable Building Simulator - Elevator Car Object
The Skyscraper Project - Version 1.10 Alpha
Copyright (C)2004-2016 Ryan Thoryk
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

#ifndef _SBS_ELEVATORCAR_H
#define _SBS_ELEVATORCAR_H

namespace SBS {

class SBSIMPEXP ElevatorCar : public Object
{
	friend class Elevator;
public:

	std::vector<int> ServicedFloors; //list of floors this car services
	float Height; //car height
	bool HeightSet;
	bool Fan; //fan enabled status
	Ogre::Vector3 MusicPosition; //music emitter position, relative of elevator center
	std::vector<int> DisplayFloors; //list of floors to only display when updating floor indicators

	ElevatorCar(Elevator *parent, int number);
	~ElevatorCar();
	Elevator* GetElevator();
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	WallObject* AddWall(const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float height1, float height2, float voffset1, float voffset2, float tw, float th);
	WallObject* AddFloor(const std::string &name, const std::string &texture, float thickness, float x1, float z1, float x2, float z2, float voffset1, float voffset2, bool reverse_axis, bool texture_direction, float tw, float th, bool legacy_behavior = false);
	FloorIndicator* AddFloorIndicator(const std::string &texture_prefix, const std::string &direction, float CenterX, float CenterZ, float width, float height, float voffset);
	ButtonPanel* CreateButtonPanel(const std::string &texture, int rows, int columns, const std::string &direction, float CenterX, float CenterZ, float width, float height, float voffset, float spacingX, float spacingY, float tw, float th);
	void DumpServicedFloors();
	bool AddServicedFloor(int number);
	void RemoveServicedFloor(int number);
	bool IsServicedFloor(int floor, bool report = true);
	int GetFloorIndex(int floor);
	int GetServicedFloorCount();
	int GetServicedFloor(int index);

	MeshObject* Mesh; //car mesh object

private:

	Elevator* parent;
	int number;

	//sound objects
	Sound *carsound;
	Sound *idlesound;
	Sound *alarm;
	Sound *floorbeep;
	std::vector<Sound*> sounds; //generic sounds
	Sound *announcesnd;
	Sound *musicsound;

	//interior directional indicators
	std::vector<DirectionalIndicator*> DirIndicatorArray;

	//doors and shaft doors
	std::vector<ElevatorDoor*> DoorArray;

	//floor indicators
	std::vector<FloorIndicator*> FloorIndicatorArray;

	//button panel array
	std::vector<ButtonPanel*> PanelArray; //elevator button panel objects

	//standard door array
	std::vector<Door*> StdDoorArray; //pointer array to standard door objects

	//light array
	std::vector<Light*> lights;

	//Models
	std::vector<Model*> ModelArray;

	//Controls
	std::vector<Control*> ControlArray;

	//Triggers
	std::vector<Trigger*> TriggerArray;

	//misc internals
	bool DirMessageSound; //true if a directional message sound is queued, to prevent repeats
	bool DoorMessageSound; //true if a door message sound is queued, to prevent repeats

	//internal data for door open/close hold feature
	int doorhold_direction;
	int doorhold_whichdoors;
	int doorhold_floor;
	bool doorhold_manual;
};

}

#endif
