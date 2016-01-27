/* $Id$ */

/*
	Scalable Building Simulator - Manager Classes
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

#ifndef _SBS_MANAGER_H
#define _SBS_MANAGER_H

namespace SBS {

struct SBSIMPEXP FloorMap
{
	int number; //floor number
	Floor *object; //floor object reference
};

struct SBSIMPEXP ElevatorMap
{
	int number; //elevator number
	Elevator *object; //elevator object reference
};

struct SBSIMPEXP ShaftMap
{
	int number; //shaft number
	Shaft *object; //shaft object reference
};

struct SBSIMPEXP StairsMap
{
	int number; //stairs number
	Stairs *object; //stairs object reference
};

class SBSIMPEXP FloorManager : public ObjectBase
{
public:
	FloorManager();
	~FloorManager();
	Floor* Create(int number);
	int GetCount(); //all floors including basements
	Floor* Get(int number);
	void Remove(Floor *floor);

private:
	std::vector<FloorMap> Array; //floor object array

	//function caching
	Floor* get_result;
	int get_number;
};

class SBSIMPEXP ElevatorManager : public ObjectBase
{
public:
	ElevatorManager();
	~ElevatorManager();
	Elevator* Create(int number);
	int GetCount();
	Elevator* Get(int number);
	void Remove(Elevator *elevator);

private:
	std::vector<ElevatorMap> Array; //elevator object array

	//function caching
	Elevator* get_result;
	int get_number;
};

class SBSIMPEXP ShaftManager : public ObjectBase
{
public:
	ShaftManager();
	~ShaftManager();
	Shaft* Create(int number, float CenterX, float CenterZ, int _startfloor, int _endfloor);
	int GetCount();
	Shaft* Get(int number);
	void Remove(Shaft *shaft);

private:
	std::vector<ShaftMap> Array; //shaft object array

	//function caching
	Shaft* get_result;
	int get_number;
};

class SBSIMPEXP StairsManager : public ObjectBase
{
public:
	StairsManager();
	~StairsManager();
	Stairs* Create(int number, float CenterX, float CenterZ, int _startfloor, int _endfloor);
	int GetCount();
	Stairs* Get(int number);
	void Remove(Stairs *stairs);

private:
	std::vector<StairsMap> Array; //stairs object array

	//function caching
	Stairs* get_result;
	int get_number;
};

}

#endif
