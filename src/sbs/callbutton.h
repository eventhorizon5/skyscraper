/* $Id$ */

/*
	Scalable Building Simulator - Call Button Object
	The Skyscraper Project - Version 1.10 Alpha
	Copyright (C)2004-2015 Ryan Thoryk
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

#ifndef _SBS_CALLBUTTON_H
#define _SBS_CALLBUTTON_H

#include "sound.h"

namespace SBS {

class Floor;

class SBSIMPEXP CallButton : public Object
{
public:

	//functions
	CallButton(Object *parent, std::vector<int> &elevators, int floornum, int number, const std::string &sound_file, const std::string &BackTexture, const std::string &UpButtonTexture, const std::string &UpButtonTexture_Lit, const std::string &DownButtonTexture, const std::string &DownButtonTexture_Lit, float CenterX, float CenterZ, float voffset, const std::string &direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th);
	~CallButton();
	void Enabled(bool value);
	bool Call(bool direction); //true is up, false is down
	void UpLight(bool value);
	void DownLight(bool value);
	void SetLights(int up, int down);
	bool ServicesElevator(int elevator);
	void Loop();
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	void SetLocked(bool value, int keyid);
	bool IsLocked();
	bool ToggleLock(bool force = false);
	int GetKeyID();
	void FireService(int value);
	int GetFloor();
	void SetLightsGroup(int up, int down);
	bool AddElevator(int elevator);
	bool RemoveElevator(int elevator);
	bool GetUpStatus() { return UpStatus; }
	bool GetDownStatus() { return DownStatus; }
	bool IsEnabled() { return is_enabled; }
	void ElevatorArrived(int number, bool direction);
	bool GetElevatorArrived(int &number, bool &direction);

private:
	void Process(int direction);

	MeshObject* CallButtonMeshBack; //call button mesh object
	MeshObject* CallButtonMeshUp; //call button mesh object
	MeshObject* CallButtonMeshDown; //call button mesh object

	std::string UpTexture, UpTextureLit;
	std::string DownTexture, DownTextureLit;
	std::string Direction; //direction the buttons face; either 'front', 'back', 'left', or 'right'

	int Number; //call button index number (on the specified floor)
	bool UpStatus; //status of up light
	bool DownStatus; //status of down light
	bool is_enabled;
	int elevator_arrived;
	bool elevator_direction;

	std::vector<int> Elevators; //elevators this call button set is assigned to

	Floor *floor; //floor this call button set is on
	Sound *sound; //sound object

	bool ProcessedUp;
	bool ProcessedDown;
	bool Locked;
	int KeyID;
	bool UpExists; //does up button exist?
	bool DownExists; //does down button exist?
};

}

#endif
