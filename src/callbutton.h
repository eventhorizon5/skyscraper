/* $Id$ */

/*
	Scalable Building Simulator - Call Button Subsystem Class
	The Skyscraper Project - Version 1.2 Alpha
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

#ifndef _SBS_CALLBUTTON_H
#define _SBS_CALLBUTTON_H

class SBSIMPEXP CallButton
{
public:

	csArray<int> Elevators; //elevators this call button set is assigned to
	int floor; //floor this call button set is on
	int Number; //call button index number (on the specified floor)
	csString Direction; //direction the buttons face; either 'front', 'back', 'left', or 'right'
	bool IsEnabled;

	//functions
	CallButton(csArray<int> &elevators, int floornum, int number, const char *BackTexture, const char *UpButtonTexture, const char *DownButtonTexture, float CenterX, float CenterZ, float voffset, const char *direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th);
	~CallButton();
	void Enabled(bool value);
	void Call(int direction); //-1 is down, 1 is up
	void Press(int index);

private:
	csRef<iMeshWrapper> CallButtonMesh; //call button mesh object
	csRef<iThingFactoryState> CallButton_state;
};

#endif
