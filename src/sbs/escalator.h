/* $Id$ */

/*
	Scalable Building Simulator - Escalator Object
	The Skyscraper Project - Version 1.11 Alpha
	Copyright (C)2004-2016 Ryan Thoryk
	http://www.skyscrapersim.com
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@skyscrapersim.com

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

#ifndef _SBS_ESCALATOR_H
#define _SBS_ESCALATOR_H

#include "mesh.h"

namespace SBS {

class SBSIMPEXP Escalator : public Object
{
public:
	int Run;
	float Speed;

	Escalator(Object *parent, const std::string &name, int run, float speed, const std::string &sound_file, const std::string &texture, const std::string &direction, float CenterX, float CenterZ, float width, float risersize, float treadsize, int num_steps, float voffset, float tw, float th);
	~Escalator();
	void Enabled(bool value);
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	void Loop();
	bool IsEnabled() { return is_enabled; }
	void OnClick(Ogre::Vector3 &position, bool shift, bool ctrl, bool alt, bool right);

private:
	DynamicMesh* StepContainer;
	Sound *sound; //sound object
	bool is_enabled;
	Ogre::Vector3 start, end;
	std::string Direction;
	float treadsize;

	class Step : public MeshObject
	{
	public:
		Step(Object* parent, const std::string &name, DynamicMesh* wrapper) : MeshObject(parent, name, wrapper) {}
		~Step() {}
		void Move(const Ogre::Vector3 &vector, float speed = 1.0f);
		void OnHit();

		Ogre::Vector3 vector;
	};

	std::vector<Step*> Steps;

	void CreateSteps(const std::string &texture, const std::string &direction, float width, float risersize, float treadsize, float tw, float th);
	void MoveSteps();
};

}

#endif
