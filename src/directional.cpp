/* $Id$ */

/*
	Scalable Building Simulator - Directional Indicator Class
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

#include "globals.h"
#include "directional.h"
#include "sbs.h"
#include "elevator.h"

extern SBS *sbs; //external pointer to the SBS engine

DirectionalIndicator::DirectionalIndicator(int elevator, int floor, const char *BackTexture, const char *uptexture, const char *uptexture_lit, const char *downtexture, const char *downtexture_lit, float CenterX, float CenterZ, float voffset, const char *direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th)
{
	//create a directional indicator

	IsEnabled = true;
	Elevator = elevator;
	Floor = floor;
	Direction = direction;
	UpTextureUnlit = uptexture;
	UpTextureLit = uptexture_lit;
	DownTextureUnlit = downtexture;
	DownTextureLit = downtexture_lit;
	UpStatus = false;
	DownStatus = false;

	//create object mesh
	csString buffer, buffer2, buffer3;
	buffer2 = elevator;
	buffer3 = floor;
	buffer = "Directional Indicator " + buffer2 + ":" + buffer3;
	buffer.Trim();
	DirectionalMesh = sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData());
	Directional_state = scfQueryInterface<iThingFactoryState> (DirectionalMesh->GetMeshObject()->GetFactory());
	DirectionalMesh->SetZBufMode(CS_ZBUF_USE);
	DirectionalMesh->SetRenderPriority(sbs->engine->GetAlphaRenderPriority());
	DirectionalMesh->GetMeshObject()->SetMixMode(CS_FX_ALPHA);

	sbs->ReverseExtents(false, false, false);

	//create panel
	if (ShowBack == true)
	{
		if (Direction == "front" || Direction == "back")
		{
			if (Direction == "front")
				sbs->DrawWalls(true, false, false, false, false, false);
			else
				sbs->DrawWalls(false, true, false, false, false, false);
			sbs->AddWallMain(Directional_state, "Panel", BackTexture, 0, CenterX - (BackWidth / 2), CenterZ, CenterX + (BackWidth / 2), CenterZ, BackHeight, BackHeight, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset, tw, th);
			sbs->ResetWalls();
		}
		if (Direction == "left" || Direction == "right")
		{
			if (Direction == "left")
				sbs->DrawWalls(true, false, false, false, false, false);
			else
				sbs->DrawWalls(false, true, false, false, false, false);
			sbs->AddWallMain(Directional_state, "Panel", BackTexture, 0, CenterX, CenterZ + (BackWidth / 2), CenterX, CenterZ - (BackWidth / 2), BackHeight, BackHeight, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset, tw, th);
			sbs->ResetWalls();
		}
	}

	//create indicator lanterns
	int bottomfloor = sbs->GetElevator(elevator)->GetBottomFloor();
	int topfloor = sbs->GetElevator(elevator)->GetTopFloor();

	if (Direction == "front" || Direction == "back")
	{
		float offset;
		if (Direction == "front")
		{
			sbs->DrawWalls(true, false, false, false, false, false);
			offset = -0.01f;
		}
		else
		{
			sbs->DrawWalls(false, true, false, false, false, false);
			offset = 0.01f;
		}
		if (floor > bottomfloor && floor < topfloor)
		{
			sbs->AddWallMain(Directional_state, "Up", UpTextureUnlit, 0, CenterX - (BackWidth / 4), CenterZ + offset, CenterX + (BackWidth / 4), CenterZ + offset, (BackHeight / 7) * 2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + ((BackHeight / 7) * 4), sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + ((BackHeight / 7) * 4), 1, 1);
			sbs->AddWallMain(Directional_state, "Down", DownTextureUnlit, 0, CenterX - (BackWidth / 4), CenterZ + offset, CenterX + (BackWidth / 4), CenterZ + offset, (BackHeight / 7) * 2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (BackHeight / 7), sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (BackHeight / 7), 1, 1);
		}
		else
		{
			if (floor < topfloor)
				sbs->AddWallMain(Directional_state, "Up", UpTextureUnlit, 0, CenterX - (BackWidth / 4), CenterZ + offset, CenterX + (BackWidth / 4), CenterZ + offset, (BackHeight / 7) * 2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (((BackHeight / 7) * 3) - (BackHeight / 14)), sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (((BackHeight / 7) * 3) - (BackHeight / 14)), 1, 1);
			if (floor > bottomfloor)
				sbs->AddWallMain(Directional_state, "Down", DownTextureUnlit, 0, CenterX - (BackWidth / 4), CenterZ + offset, CenterX + (BackWidth / 4), CenterZ + offset, (BackHeight / 7) * 2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (((BackHeight / 7) * 3) - (BackHeight / 14)), sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (((BackHeight / 7) * 3) - (BackHeight / 14)), 1, 1);
		}

		sbs->ResetWalls();
	}
	else
	{
		float offset;
		if (Direction == "left")
		{
			sbs->DrawWalls(true, false, false, false, false, false);
			offset = -0.01f;
		}
		else
		{
			//right
			sbs->DrawWalls(false, true, false, false, false, false);
			offset = 0.01f;
		}
		if (floor > bottomfloor && floor < topfloor)
		{
			sbs->AddWallMain(Directional_state, "Up", UpTextureUnlit, 0, CenterX + offset, CenterZ - (BackWidth / 4), CenterX + offset, CenterZ + (BackWidth / 4), (BackHeight / 7) * 2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + ((BackHeight / 7) * 4), sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + ((BackHeight / 7) * 4), 1, 1);
			sbs->AddWallMain(Directional_state, "Down", DownTextureUnlit, 0, CenterX + offset, CenterZ - (BackWidth / 4), CenterX + offset, CenterZ + (BackWidth / 4), (BackHeight / 7) * 2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (BackHeight / 7), sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (BackHeight / 7), 1, 1);
		}
		else
		{
			if (floor < topfloor)
				sbs->AddWallMain(Directional_state, "Up", UpTextureUnlit, 0, CenterX + offset, CenterZ - (BackWidth / 4), CenterX + offset, CenterZ + (BackWidth / 4), (BackHeight / 7) * 2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (((BackHeight / 7) * 3) - (BackHeight / 14)), sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (((BackHeight / 7) * 3) - (BackHeight / 14)), 1, 1);
			if (floor > bottomfloor)
				sbs->AddWallMain(Directional_state, "Down", DownTextureUnlit, 0, CenterX + offset, CenterZ - (BackWidth / 4), CenterX + offset, CenterZ + (BackWidth / 4), (BackHeight / 7) * 2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (((BackHeight / 7) * 3) - (BackHeight / 14)), sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (((BackHeight / 7) * 3) - (BackHeight / 14)), 1, 1);
		}

		sbs->ResetWalls();
	}
	sbs->ResetExtents();

	//run mesh prepare to prevent problems with changing textures later
	csRef<iThingState> thingstate = scfQueryInterface<iThingState> (DirectionalMesh->GetMeshObject());
	thingstate->Prepare();
}

DirectionalIndicator::~DirectionalIndicator()
{

}

void DirectionalIndicator::Enabled(bool value)
{
	//turns panel on/off
	if (value == IsEnabled)
		return;

	sbs->EnableMesh(DirectionalMesh, value);
	IsEnabled = value;
}

void DirectionalIndicator::UpLight(bool value)
{
	//turn on the 'up' directional light
	if (value == UpStatus)
		return;

	//exit if indicator is disabled
	if (IsEnabled == false)
		return;

	bool result;
	if (value == true)
		sbs->ChangeTexture(DirectionalMesh, sbs->GetTextureMaterial(UpTextureUnlit, result), UpTextureLit);
	else
		sbs->ChangeTexture(DirectionalMesh, sbs->GetTextureMaterial(UpTextureLit, result), UpTextureUnlit);
	UpStatus = value;
}

void DirectionalIndicator::DownLight(bool value)
{
	//turn on the 'down' directional light
	if (value == DownStatus)
		return;

	//exit if indicator is disabled
	if (IsEnabled == false)
		return;

	bool result;
	if (value == true)
		sbs->ChangeTexture(DirectionalMesh, sbs->GetTextureMaterial(DownTextureUnlit, result), DownTextureLit);
	else
		sbs->ChangeTexture(DirectionalMesh, sbs->GetTextureMaterial(DownTextureLit, result), DownTextureUnlit);
	DownStatus = value;
}
