//Scalable Building Simulator - Floor Class
//The Skyscraper Project - Version 1.1 Alpha
//Copyright ©2005 Ryan Thoryk
//http://www.tliquest.net/skyscraper
//http://sourceforge.net/projects/skyscraper
//Contact - ryan@tliquest.net

//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include <iostream>

//CrystalSpace Includes
#include "cssysdef.h"
#include "csutil/sysfunc.h"
#include "iutil/vfs.h"
#include "csutil/cscolor.h"
#include "cstool/csview.h"
#include "cstool/initapp.h"
#include "iutil/eventq.h"
#include "iutil/event.h"
#include "iutil/objreg.h"
#include "iutil/csinput.h"
#include "iutil/virtclk.h"
#include "iengine/sector.h"
#include "iengine/engine.h"
#include "iengine/camera.h"
#include "iengine/light.h"
#include "iengine/texture.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/material.h"
#include "imesh/thing.h"
#include "imesh/object.h"
#include "ivideo/graph3d.h"
#include "ivideo/graph2d.h"
#include "ivideo/texture.h"
#include "ivideo/material.h"
#include "ivideo/fontserv.h"
#include "ivideo/natwin.h"
#include "igraphic/imageio.h"
#include "imap/parser.h"
#include "ivaria/reporter.h"
#include "ivaria/stdrep.h"
#include "csutil/cmdhelp.h"
#include "csutil/event.h"

#include "floor.h"
#include "sbs.h"

extern SBS *sbs; //external pointer to the SBS engine

Floor::Floor(int number)
{
	//Set floor's object number
	FloorNumber = number;

	//size shaft arrays accordingly
	//ReDim CrawlSpaceShaft(ElevatorShafts)
	//ReDim Shafts(ElevatorShafts)

	//Create and name new meshes
	Level (sbs->engine->CreateSectorWallsMesh (area, "Level"));
	ls = SCF_QUERY_INTERFACE (Level->GetMeshObject (), iThingState);
	level_state = ls->GetFactory ();

	CrawlSpace (sbs->engine->CreateSectorWallsMesh (area, "CrawlSpace"));
	ShaftsFloor (sbs->engine->CreateSectorWallsMesh (area, "ShaftsFloor"));

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

	level_state->AddOutsideBox(csVector3(x1, altitude, z1), csVector3(x2, altitude, z2));
	level_state->AddInsideBox(csVector3(x1, altitude, z1), csVector3(x2, altitude, z2));
	iMaterialWrapper* tm = engine->GetMaterialList ()->FindByName (texture);
	level_state->SetPolygonMaterial (CS_POLYRANGE_LAST, tm);
	level_state->SetPolygonTextureMapping (CS_POLYRANGE_LAST, 3); //see todo below

//*** todo: implement full texture sizing - the "3" above is a single-dimension value; there needs to be 2

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

	level_state->AddOutsideBox(csVector3(x1, altitude, z1), csVector3(x2, altitude, z2));
	level_state->AddInsideBox(csVector3(x1, altitude, z1), csVector3(x2, altitude, z2));
	iMaterialWrapper* tm = engine->GetMaterialList ()->FindByName (texture);
	level_state->SetPolygonMaterial (CS_POLYRANGE_LAST, tm);
	level_state->SetPolygonTextureMapping (CS_POLYRANGE_LAST, 3); //see todo below

//*** todo: implement full texture sizing - the "3" above is a single-dimension value; there needs to be 2

}

void Floor::AddWall(const char *texture, float x1, float z1, float x2, float z2, float heightchange, float voffset, float tw, float th)
{
	//Adds a wall with the specified dimensions
	
	float wallheight = heightchange + FloorHeight;
	float altitude = FloorAltitude + voffset;

	//Call texture autosizing formulas
	if ((tw == 0) && (z1 == z2)
		tw = AutoSize(x1, x2, true);
	if ((tw == 0) && (x1 == x2))
		tw = AutoSize(z1, z2, true);
	if (th == 0)
		th = AutoSize(0, wallheight, false);
	
	level_state->AddOutsideBox(csVector3(x1, altitude, z1), csVector3(x2, altitude + wallheight, z2));
	level_state->AddInsideBox(csVector3(x1, altitude, z1), csVector3(x2, altitude + wallheight, z2));
	iMaterialWrapper* tm = engine->GetMaterialList ()->FindByName (texture);
	level_state->SetPolygonMaterial (CS_POLYRANGE_LAST, tm);
	level_state->SetPolygonTextureMapping (CS_POLYRANGE_LAST, 3); //see todo below

//*** todo: implement full texture sizing - the "3" above is a single-dimension value; there needs to be 2

}

void Floor::AddCrawlSpaceWall(const char *texture, float x1, float z1, float x2, float z2, float heightchange, float voffset, float tw, float th)
{
	//Adds a crawlspace wall with the specified dimensions
	
	float wallheight = heightchange + FloorHeight;
	float altitude = FloorAltitude + voffset;

	//Texture autosizing formulas
	if ((tw == 0) && (z1 == z2))
		tw = AutoSize(x1, x2, true);
	if ((tw == 0) && (x1 == x2)
		tw = AutoSize(z1, z2, true);
	if (th == 0)
		th = AutoSize(0, wallheight, false);
	
	float altitude = FloorAltitude + FloorHeight + voffset;

	level_state->AddOutsideBox(csVector3(x1, altitude, z1), csVector3(x2, altitude + wallheight, z2));
	level_state->AddInsideBox(csVector3(x1, altitude, z1), csVector3(x2, altitude + wallheight, z2));
	iMaterialWrapper* tm = engine->GetMaterialList ()->FindByName (texture);
	level_state->SetPolygonMaterial (CS_POLYRANGE_LAST, tm);
	level_state->SetPolygonTextureMapping (CS_POLYRANGE_LAST, 3); //see todo below

//*** todo: implement full texture sizing - the "3" above is a single-dimension value; there needs to be 2

}

void Floor::CreateWallBox(const char *texture, float WidthX, float LengthZ, float CenterX, float CenterZ, float heightchange, bool CSpace, float ResX, float ResY)
{
	float height = heightchange + FloorAltitude;
	float WallHeight = FloorHeight - CrawlSpaceHeight;
	float x1;
	float x2;
	float z1;
	float z2;
	if (CSpace == true)
	{
	    height = WallHeight;
	    WallHeight = CrawlSpaceHeight;
	}

	x1 = CenterX - (WidthX / 2);
	x2 = CenterX + (WidthX / 2);
	z1 = CenterZ - (LengthZ / 2);
	z1 = CenterZ + (LengthZ / 2);

	level_state->AddOutsideBox(csVector3(x1, height, z1), csVector3(x2, WallHeight + height, z2));
	level_state->AddInsideBox(csVector3(x1, height, z1), csVector3(x2, WallHeight + height, z2));
	iMaterialWrapper* tm = engine->GetMaterialList ()->FindByName (texture);
	level_state->SetPolygonMaterial (CS_POLYRANGE_LAST, tm);
	level_state->SetPolygonTextureMapping (CS_POLYRANGE_LAST, 3); //see todo below

//for the texture sizing, the original VB rewrite didn't call the autosize functions

}

void Floor::EnableFloor()
{

}

void Floor::DisableFloor()
{

}
