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

Floor::Floor(int number, const char *name)
{
	//Set floor's object number
	Number = number;

	//Create primary level mesh
	Level = (sbs->engine->CreateSectorWallsMesh (sbs->area, "Level"));
	Level_object = Level->GetMeshObject ();
	Level_factory = Level_object->GetFactory();
	Level_state = scfQueryInterface<iThingFactoryState> (Level_factory);

	//Create interfloor mesh
	Interfloor = (sbs->engine->CreateSectorWallsMesh (sbs->area, "Level"));
	Interfloor_object = Level->GetMeshObject ();
	Interfloor_factory = Level_object->GetFactory();
	Interfloor_state = scfQueryInterface<iThingFactoryState> (Level_factory);

	// turn on ZUSE Z-buffer mode
	Level->SetZBufMode(CS_ZBUF_USE);

	//set enabled flag
	IsEnabled = true;
}

Floor::~Floor()
{
	//Deconstructor
}

void Floor::SetCameraFloor()
{
	//Moves camera to specified floor (sets altitude to the floor's altitude plus CameraAltitude)
	
	csVector3 camlocation = c->GetPosition();
	c->SetPosition(csVector3(camlocation.x, Altitude + c->DefaultAltitude, camlocation.z));
}

void Floor::AddFloor(const char *texture, double x1, double z1, double x2, double z2, double voffset, double tw, double th)
{
	//Adds a floor with the specified dimensions and vertical offset
	
	//Set horizontal scaling
	x1 = x1 * sbs->HorizScale;
	x2 = x2 * sbs->HorizScale;
	z1 = z1 * sbs->HorizScale;
	z2 = z2 * sbs->HorizScale;

	//Call texture autosizing formulas
	if (tw == 0)
		tw = AutoSize(x1, x2, true);
	if (th == 0)
		th = AutoSize(z1, z2, false);

	sbs->AddFloorMain(Level_state, texture, x1, z1, x2, z2, Altitude + voffset, tw, th);
}

void Floor::AddInterfloorFloor(const char *texture, double x1, double z1, double x2, double z2, double voffset, double tw, double th)
{
	//Adds an interfloor floor with the specified dimensions and vertical offset

	//Set horizontal scaling
	x1 = x1 * sbs->HorizScale;
	x2 = x2 * sbs->HorizScale;
	z1 = z1 * sbs->HorizScale;
	z2 = z2 * sbs->HorizScale;

	//Texture autosizing formulas
	if (tw == 0)
		tw = AutoSize(x1, x2, true);
	if (th == 0)
		th = AutoSize(z1, z2, false);

	sbs->AddFloorMain(Interfloor_state, texture, x1, z1, x2, z2, Altitude + Height + voffset, tw, th);
}

void Floor::AddWall(const char *texture, double x1, double z1, double x2, double z2, double height_in1, double height_in2, double voffset1, double voffset2, double tw, double th)
{
	//Adds a wall with the specified dimensions
	
	//Set horizontal scaling
	x1 = x1 * sbs->HorizScale;
	x2 = x2 * sbs->HorizScale;
	z1 = z1 * sbs->HorizScale;
	z2 = z2 * sbs->HorizScale;

	//Call texture autosizing formulas
	if ((tw == 0) && (z1 == z2))
		tw = AutoSize(x1, x2, true);
	if ((tw == 0) && (x1 == x2))
		tw = AutoSize(z1, z2, true);
	if (th == 0)
		th = AutoSize(0, height_in1, false);
	
	sbs->AddWallMain(Level_state, texture, x1, z1, x2, z2, height_in1, height_in2, Altitude + voffset1, Altitude + voffset2, tw, th);
}

void Floor::AddInterfloorWall(const char *texture, double x1, double z1, double x2, double z2, double height_in1, double height_in2, double voffset1, double voffset2, double tw, double th)
{
	//Adds an interfloor wall with the specified dimensions
	
	//Set horizontal scaling
	x1 = x1 * sbs->HorizScale;
	x2 = x2 * sbs->HorizScale;
	z1 = z1 * sbs->HorizScale;
	z2 = z2 * sbs->HorizScale;

	//Texture autosizing formulas
	if ((tw == 0) && (z1 == z2))
		tw = AutoSize(x1, x2, true);
	if ((tw == 0) && (x1 == x2))
		tw = AutoSize(z1, z2, true);
	if (th == 0)
		th = AutoSize(0, height_in1, false);
	
	sbs->AddWallMain(Interfloor_state, texture, x1, z1, x2, z2, height_in1, height_in2, Altitude + voffset1, Altitude + voffset2, tw, th);
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
	//this is going to be *hard* lol
	//autogenerate a floor based on either wall bounds, or by specified boundaries

}

double Floor::FullHeight()
{
	//calculate full height of a floor
	return Height + InterfloorHeight;
}

