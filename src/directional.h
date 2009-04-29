/* $Id$ */

/*
	Scalable Building Simulator - Directional Indicator Subsystem Class
	The Skyscraper Project - Version 1.4 Alpha
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

#ifndef _SBS_DIRECTIONAL_H
#define _SBS_DIRECTIONAL_H

class SBSIMPEXP DirectionalIndicator
{
public:

	int Elevator; //elevator this indicator is assigned to
	csString Direction; //direction the indicator faces; either 'front', 'back', 'left', or 'right'
	bool IsEnabled; //true if indicator is currently enabled

	//functions
	DirectionalIndicator(int elevator, const char *BackTexture, const char *UpTexture, const char *DownTexture, float CenterX, float CenterZ, float voffset, const char *direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th);
	~DirectionalIndicator();
	void Enabled(bool value);

private:
	csRef<iMeshWrapper> DirectionalMesh; //indicator mesh object
	csRef<iThingFactoryState> Directional_state;
};

#endif
