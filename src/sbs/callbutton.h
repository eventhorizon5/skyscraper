/* $Id$ */

/*
	Scalable Building Simulator - Call Button Subsystem Class
	The Skyscraper Project - Version 1.8 Alpha
	Copyright (C)2004-2013 Ryan Thoryk
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

class SBSIMPEXP CallButton
{
public:

	Object *object; //SBS object
	std::vector<int> Elevators; //elevators this call button set is assigned to
	int floor; //floor this call button set is on
	int Number; //call button index number (on the specified floor)
	std::string Direction; //direction the buttons face; either 'front', 'back', 'left', or 'right'
	bool IsEnabled;
	bool UpStatus; //status of up light
	bool DownStatus; //status of down light

	//functions
	CallButton(std::vector<int> &elevators, int floornum, int number, const char *BackTexture, const char *UpButtonTexture, const char *UpButtonTexture_Lit, const char *DownButtonTexture, const char *DownButtonTexture_Lit, float CenterX, float CenterZ, float voffset, const char *direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th);
	~CallButton();
	void Enabled(bool value);
	void Call(bool direction); //true is up, false is down
	void UpLight(bool value);
	void DownLight(bool value);
	void SetLights(int up, int down);
	bool ServicesElevator(int elevator);
	void Loop(bool direction);
	void Report(const char *message);
	bool ReportError(const char *message);

private:
	MeshObject* CallButtonBackMesh; //call button mesh object
	MeshObject* CallButtonMeshUp; //call button mesh object
	MeshObject* CallButtonMeshDown; //call button mesh object

	std::string UpTexture, UpTextureLit;
	std::string DownTexture, DownTextureLit;

	bool ProcessedUp;
	bool ProcessedDown;

	char intbuffer[65];
	char buffer[20];
};

#endif
