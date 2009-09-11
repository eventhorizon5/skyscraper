/* $Id$ */

/*
	Scalable Building Simulator - Floor Indicator Subsystem Class
	The Skyscraper Project - Version 1.5 Alpha
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

#ifndef _SBS_FLOORINDICATOR_H
#define _SBS_FLOORINDICATOR_H

class SBSIMPEXP FloorIndicator
{
public:

	int Elevator; //elevator this indicator is assigned to
	bool IsEnabled;
	csString Prefix; //texture name prefix

	//functions
	FloorIndicator(int elevator, const char *texture_prefix, const char *direction, float CenterX, float CenterZ, float width, float height, float altitude);
	~FloorIndicator();
	void Enabled(bool value);
	csVector3 GetPosition();
	void SetPosition(csVector3 position);
	void MovePosition(csVector3 position);
	void Update(const char *value);

private:
	csRef<iMeshWrapper> FloorIndicatorMesh; //indicator mesh object
	csRef<iMovable> FloorIndicator_movable;
};

#endif
