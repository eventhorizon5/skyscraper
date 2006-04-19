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

extern SBS *sbs; //external pointer to the SBS engine

Floor::Floor(int number, const char *name)
{
	//Set floor's object number
	FloorNumber = number;

	//size shaft arrays accordingly
	//ReDim CrawlSpaceShaft(ElevatorShafts)
	//ReDim Shafts(ElevatorShafts)

	//Create and name new meshes
	Level = (sbs->engine->CreateSectorWallsMesh (sbs->area, "Level"));
	Level_object = Level->GetMeshObject ();
	Level_factory = Level_object->GetFactory();
	Level_state = scfQueryInterface<iThingFactoryState> (Level_factory);

	// turn on ZUSE Z-buffer mode
	Level->SetZBufMode(CS_ZBUF_USE);

	//CrawlSpace = (sbs->engine->CreateSectorWallsMesh (sbs->area, "CrawlSpace"));
	//ShaftsFloor = (sbs->engine->CreateSectorWallsMesh (sbs->area, "ShaftsFloor"));

	//Set Level = Scene.CreateMeshBuilder("Level " + Str$(Number))
	//Set CrawlSpace = Scene.CreateMeshBuilder("CrawlSpace " + Str$(Number))
	//Set ShaftsFloor = Scene.CreateMeshBuilder("ShaftsFloor " + Str$(Number))
	//Dim i As Integer
	//For i = 1 To ElevatorShafts
	//    DoEvents
	//    Set CrawlSpaceShaft(i) = Scene.CreateMeshBuilder("CrawlSpaceShaft" + LTrim(Str$(i)) + " " + Str$(Number))
	//    Set Shafts(i) = Scene.CreateMeshBuilder("Shafts" + LTrim(Str$(i)) + " " + Str$(Number))
	//Next i

}

Floor::~Floor()
{

}

void Floor::SetCameraFloor()
{
	//Moves camera to specified floor (sets altitude to the floor's altitude plus CameraAltitude)
	
	csVector3 camlocation = sbs->view->GetCamera()->GetTransform().GetOrigin();
	sbs->view->GetCamera()->GetTransform().SetOrigin(csVector3(camlocation.x, FloorAltitude + sbs->CameraDefAltitude, camlocation.z));
}

void Floor::AddFloor(const char *texture, float x1, float z1, float x2, float z2, float voffset, float tw, float th)
{
	//Adds a floor with the specified dimensions and vertical offset
	
	//Call texture autosizing formulas
	if (tw == 0)
		tw = AutoSize(x1, x2, true);
	if (th == 0)
		th = AutoSize(z1, z2, false);

	float altitude = FloorAltitude + voffset;

	sbs->AddFloor(Level_state, texture, x1, z1, x2, z2, altitude, tw, th);
}

void Floor::AddCrawlSpaceFloor(const char *texture, float x1, float z1, float x2, float z2, float voffset, float tw, float th)
{
	//Adds a crawlspace with the specified dimensions and vertical offset

	//Texture autosizing formulas
	if (tw == 0)
		tw = AutoSize(x1, x2, true);
	if (th == 0)
		th = AutoSize(z1, z2, false);

	float altitude = FloorAltitude + FloorHeight + voffset;

	sbs->AddFloor(Level_state, texture, x1, z1, x2, z2, altitude, tw, th);
}

void Floor::AddWall(const char *texture, float x1, float z1, float x2, float z2, float heightchange, float voffset, float tw, float th)
{
	//Adds a wall with the specified dimensions
	
	float wallheight = heightchange + FloorHeight;
	float altitude = FloorAltitude + voffset;

	//Call texture autosizing formulas
	if ((tw == 0) && (z1 == z2))
		tw = AutoSize(x1, x2, true);
	if ((tw == 0) && (x1 == x2))
		tw = AutoSize(z1, z2, true);
	if (th == 0)
		th = AutoSize(0, wallheight, false);
	
	sbs->AddWall(Level_state, texture, x1, z1, x2, z2, wallheight, altitude, tw, th);
}

void Floor::AddCrawlSpaceWall(const char *texture, float x1, float z1, float x2, float z2, float heightchange, float voffset, float tw, float th)
{
	//Adds a crawlspace wall with the specified dimensions
	
	float wallheight = heightchange + FloorHeight;
	float altitude = FloorAltitude + FloorHeight + voffset;

	//Texture autosizing formulas
	if ((tw == 0) && (z1 == z2))
		tw = AutoSize(x1, x2, true);
	if ((tw == 0) && (x1 == x2))
		tw = AutoSize(z1, z2, true);
	if (th == 0)
		th = AutoSize(0, wallheight, false);
	
	sbs->AddWall(Level_state, texture, x1, z1, x2, z2, wallheight, altitude, tw, th);
}

void Floor::CreateWallBox(const char *texture, float WidthX, float LengthZ, float CenterX, float CenterZ, float heightchange, bool CSpace, float tw, float th)
{
	iMaterialWrapper* tm;
	float height = heightchange + FloorAltitude;
	float wallheight = FloorHeight - CrawlSpaceHeight;
	float x1;
	float x2;
	float z1;
	float z2;
	if (CSpace == true)
	{
	    height = height + wallheight;
	    wallheight = CrawlSpaceHeight;
	}

	x1 = CenterX - (WidthX / 2);
	x2 = CenterX + (WidthX / 2);
	z1 = CenterZ - (LengthZ / 2);
	z2 = CenterZ + (LengthZ / 2);

	//Texture autosizing formulas
	if ((tw == 0) && (z1 == z2))
		tw = AutoSize(x1, x2, true);
	if ((tw == 0) && (x1 == x2))
		tw = AutoSize(z1, z2, true);
	if (th == 0)
		th = AutoSize(0, wallheight, false);

	Level_state->AddInsideBox(csVector3(x1, height, z1), csVector3(x2, height + wallheight, z2));
	tm = sbs->engine->GetMaterialList ()->FindByName (texture);
	Level_state->SetPolygonMaterial (CS_POLYRANGE_LAST, tm);
	Level_state->SetPolygonTextureMapping (CS_POLYRANGE_LAST, 3); //see todo below

	//Level_state->AddOutsideBox(csVector3(x1 + 0.001, height + 0.1, z1 + 0.01), csVector3(x2 - 0.1, height + wallheight + 0.1, z2 + 0.1));
	Level_state->AddOutsideBox(csVector3(x1, height, z1), csVector3(x2, height + wallheight, z2));
	tm = sbs->engine->GetMaterialList ()->FindByName (texture);
	Level_state->SetPolygonMaterial (CS_POLYRANGE_LAST, tm);
	Level_state->SetPolygonTextureMapping (CS_POLYRANGE_LAST, 3); //see todo below
	
//*** todo: implement full texture sizing - the "3" above is a single-dimension value; there needs to be 2

}

void Floor::EnableFloor()
{
	Level->GetFlags().Reset (CS_ENTITY_INVISIBLEMESH);
	Level->GetFlags().Reset (CS_ENTITY_NOSHADOWS);
	Level->GetFlags().Reset (CS_ENTITY_NOHITBEAM);
}

void Floor::DisableFloor()
{
	Level->GetFlags().Set (CS_ENTITY_INVISIBLEMESH);
	Level->GetFlags().Set (CS_ENTITY_NOSHADOWS);
	Level->GetFlags().Set (CS_ENTITY_NOHITBEAM);
}
