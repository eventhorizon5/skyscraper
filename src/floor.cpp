/*
	Scalable Building Simulator - Floor Class
	The Skyscraper Project - Version 1.1 Alpha
	Copyright ©2005-2006 Ryan Thoryk
	http://www.tliquest.net/skyscraper
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

#include <crystalspace.h>
#include "floor.h"
#include "sbs.h"
#include "camera.h"

extern SBS *sbs; //external pointer to the SBS engine
extern Camera *c; //external pointer to the camera

Floor::Floor(int number)
{
	csString buffer;

	//Set floor's object number
	Number = number;

	//Create primary level mesh
	buffer = Number;
	buffer.Insert(0, "Level ");
	buffer.Trim();
	Level = (sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData()));
	Level_object = Level->GetMeshObject ();
	Level_factory = Level_object->GetFactory();
	Level_state = scfQueryInterface<iThingFactoryState> (Level_factory);
	Level->SetZBufMode(CS_ZBUF_USE);

	//Create interfloor mesh
	buffer = Number;
	buffer.Insert(0, "Interfloor ");
	buffer.Trim();
	Interfloor = (sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData()));
	Interfloor_object = Level->GetMeshObject ();
	Interfloor_factory = Level_object->GetFactory();
	Interfloor_state = scfQueryInterface<iThingFactoryState> (Level_factory);
	Interfloor->SetZBufMode(CS_ZBUF_USE);

	//set enabled flag
	IsEnabled = true;

	//init other variables
	Name = "";
	ID = "";
	FloorType = "";
	Description = "";
	Altitude = 0;
	Height = 0;
	InterfloorHeight = 0;
	DoorHeight = 0;
	DoorWidth = 0;
}

Floor::~Floor()
{
	//Destructor

	if (Level)
		delete Level;

	if (Interfloor)
		delete Interfloor;
}

void Floor::SetCameraFloor()
{
	//Moves camera to specified floor (sets altitude to the floor's altitude plus CameraAltitude)
	
	csVector3 camlocation = c->GetPosition();
	c->SetPosition(csVector3(camlocation.x, Altitude + c->DefaultAltitude, camlocation.z));
}

int Floor::AddFloor(const char *texture, double x1, double z1, double x2, double z2, double voffset1, double voffset2, double tw, double th, bool isexternal)
{
	//Adds a floor with the specified dimensions and vertical offset
	double tw2;
	double th2;

	//Set horizontal scaling
	x1 = x1 * sbs->HorizScale;
	x2 = x2 * sbs->HorizScale;
	z1 = z1 * sbs->HorizScale;
	z2 = z2 * sbs->HorizScale;

	//Call texture autosizing formulas
	tw2 = AutoSize(x1, x2, true, isexternal, tw);
	th2 = AutoSize(z1, z2, false, isexternal, th);
	
	if (isexternal == false)
		return sbs->AddFloorMain(Level_state, texture, x1, z1, x2, z2, Altitude + voffset1, Altitude + voffset2, tw2, th2);
	else
		return sbs->AddFloorMain(sbs->External_state, texture, x1, z1, x2, z2, Altitude + voffset1, Altitude + voffset2, tw2, th2);
}

int Floor::AddInterfloorFloor(const char *texture, double x1, double z1, double x2, double z2, double voffset1, double voffset2, double tw, double th)
{
	//Adds an interfloor floor with the specified dimensions and vertical offset
	double tw2;
	double th2;

	//Set horizontal scaling
	x1 = x1 * sbs->HorizScale;
	x2 = x2 * sbs->HorizScale;
	z1 = z1 * sbs->HorizScale;
	z2 = z2 * sbs->HorizScale;

	//Texture autosizing formulas
	tw2 = AutoSize(x1, x2, true, false, tw);
	th2 = AutoSize(z1, z2, false, false, th);

	return sbs->AddFloorMain(Interfloor_state, texture, x1, z1, x2, z2, Altitude + Height + voffset1, Altitude + Height + voffset2, tw2, th2);
}

int Floor::AddWall(const char *texture, double x1, double z1, double x2, double z2, double height_in1, double height_in2, double voffset1, double voffset2, double tw, double th, bool revX, bool revY, bool revZ, bool isexternal)
{
	//Adds a wall with the specified dimensions
	double tw2 = tw;
	double th2;
	double tempw1;
	double tempw2;
	
	//Set horizontal scaling
	x1 = x1 * sbs->HorizScale;
	x2 = x2 * sbs->HorizScale;
	z1 = z1 * sbs->HorizScale;
	z2 = z2 * sbs->HorizScale;

	//Call texture autosizing formulas
	if (z1 == z2)
		tw2 = AutoSize(x1, x2, true, isexternal, tw);
	if (x1 == x2)
		tw2 = AutoSize(z1, z2, true, isexternal, tw);
	if ((z1 != z2) && (x1 != x2))
	{
		//calculate diagonals
	    if (x1 > x2)
			tempw1 = x1 - x2;
		else
			tempw1 = x2 - x1;
		if (z1 > z2)
			tempw2 = z1 - z2;
		else
			tempw2 = z2 - z1;
	    tw2 = AutoSize(0, sqrt(pow(tempw1, 2) + pow(tempw2, 2)), true, isexternal, tw);
	}
	th2 = AutoSize(0, height_in1, false, isexternal, th);
	
	if (isexternal == false)
		return sbs->AddWallMain(Level_state, texture, x1, z1, x2, z2, height_in1, height_in2, Altitude + voffset1, Altitude + voffset2, tw2, th2, revX, revY, revZ);
	else
		return sbs->AddWallMain(sbs->External_state, texture, x1, z1, x2, z2, height_in1, height_in2, Altitude + voffset1, Altitude + voffset2, tw2, th2, revX, revY, revZ);
}

int Floor::AddInterfloorWall(const char *texture, double x1, double z1, double x2, double z2, double height_in1, double height_in2, double voffset1, double voffset2, double tw, double th)
{
	//Adds an interfloor wall with the specified dimensions
	double tw2 = tw;
	double th2;
	
	//Set horizontal scaling
	x1 = x1 * sbs->HorizScale;
	x2 = x2 * sbs->HorizScale;
	z1 = z1 * sbs->HorizScale;
	z2 = z2 * sbs->HorizScale;

	//Texture autosizing formulas
	if (z1 == z2)
		tw2 = AutoSize(x1, x2, true, false, tw);
	if (x1 == x2)
		tw2 = AutoSize(z1, z2, true, false, tw);
	th2 = AutoSize(0, height_in1, false, false, th);
	
	return sbs->AddWallMain(Interfloor_state, texture, x1, z1, x2, z2, height_in1, height_in2, Altitude + Height + voffset1, Altitude + Height + voffset2, tw2, th2, false, false, false);
}

void Floor::Enabled(bool value)
{
	//turns floor on/off
	if (value == true)
	{
		Level->GetFlags().Reset (CS_ENTITY_INVISIBLEMESH);
		Level->GetFlags().Reset (CS_ENTITY_NOSHADOWS);
		Level->GetFlags().Reset (CS_ENTITY_NOHITBEAM);

		Interfloor->GetFlags().Reset (CS_ENTITY_INVISIBLEMESH);
		Interfloor->GetFlags().Reset (CS_ENTITY_NOSHADOWS);
		Interfloor->GetFlags().Reset (CS_ENTITY_NOHITBEAM);

		IsEnabled = true;
	}
	else
	{
		Level->GetFlags().Set (CS_ENTITY_INVISIBLEMESH);
		Level->GetFlags().Set (CS_ENTITY_NOSHADOWS);
		Level->GetFlags().Set (CS_ENTITY_NOHITBEAM);

		Interfloor->GetFlags().Set (CS_ENTITY_INVISIBLEMESH);
		Interfloor->GetFlags().Set (CS_ENTITY_NOSHADOWS);
		Interfloor->GetFlags().Set (CS_ENTITY_NOHITBEAM);

		IsEnabled = false;
	}
}

void Floor::AddAutoFloor(const char *texture)
{
	//autogenerate a floor based on either wall bounds, or by specified boundaries

}

double Floor::FullHeight()
{
	//calculate full height of a floor
	return Height + InterfloorHeight;
}

int Floor::AddCallButtons(bool up, bool down, int elevatornumber, bool direction, const char *backtexture, const char *buttontexture, double CenterX, double CenterZ, double width, double height, double tw, double th)
{
	//adds call buttons

	double x1, x2;
	double z1, z2;
	int firstidx;

	if (direction == false)
	{
		x1 = CenterX;
		x2 = CenterX;
		z1 = CenterZ - (width / 2);
		z2 = CenterZ + (width / 2);
	}
	else
	{
		x1 = CenterX - (width / 2);
		x2 = CenterX + (width / 2);
		z1 = CenterZ;
		z2 = CenterZ;
	}

	//firstidx = sbs->AddWallMain(CallButtonsUp_state, texture, x1, z1, x2, z2, height, height, voffset + GetPosition().y, voffset + GetPosition().y, tw, th);
	//sbs->AddWallMain(CallButtonsDown_state, texture, x1, z1, x2, z2, height, height, voffset + GetPosition().y, voffset + GetPosition().y, tw, th);
	return firstidx;
}
