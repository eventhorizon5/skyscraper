/*
	Scalable Building Simulator - Revolving Door Object
	The Skyscraper Project - Version 1.12 Alpha
	Copyright (C)2004-2024 Ryan Thoryk
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

#ifndef _SBS_REVOLVINGDOOR_H
#define _SBS_REVOLVINGDOOR_H

#include "lock.h"

namespace SBS {

class SBSIMPEXP RevolvingDoor : public Object, public Lock
{
public:

	bool IsMoving; //is door moving?
	bool Clockwise; //if door should rotate clockwise
	Real Speed; //rotation speed
	std::string soundfile; //sound
	int Segments;

	RevolvingDoor(Object *parent, DynamicMesh *wrapper, const std::string &name, bool run, const std::string &soundfile, const std::string &texture, Real thickness, bool clockwise, int segments, Real speed, Real rotation, Real CenterX, Real CenterZ, Real width, Real height, Real voffset, Real tw, Real th);
	~RevolvingDoor();
	void Enabled(bool value);
	void MoveDoor();
	bool IsEnabled() { return is_enabled; }
	void Loop();
	void OnClick(Vector3 &position, bool shift, bool ctrl, bool alt, bool right);
	void OnHit();
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	void Run(bool value);

private:
	MeshObject* DoorMesh; //door mesh

	//sound object
	Sound *sound;

	bool brake;
	bool is_enabled;
	Real rotation;
	bool run;
};

}

#endif
