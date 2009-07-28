/* $Id$ */

/*
	Scalable Building Simulator - Directional Indicator Class
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

#include "globals.h"
#include "directional.h"
#include "sbs.h"
#include "elevator.h"

extern SBS *sbs; //external pointer to the SBS engine

DirectionalIndicator::DirectionalIndicator(int elevator, int floor, bool single, bool vertical, const char *BackTexture, const char *uptexture, const char *uptexture_lit, const char *downtexture, const char *downtexture_lit, float CenterX, float CenterZ, float voffset, const char *direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th)
{
	//create a directional indicator

	IsEnabled = true;
	elevator_num = elevator;
	floor_num = floor;
	Direction = direction;
	UpTextureUnlit = uptexture;
	UpTextureLit = uptexture_lit;
	DownTextureUnlit = downtexture;
	DownTextureLit = downtexture_lit;
	UpStatus = false;
	DownStatus = false;
	Single = single;
	Vertical = vertical;

	//create object mesh
	csString buffer, buffer2, buffer3;
	buffer2 = elevator;
	buffer3 = floor;
	buffer = "Directional Indicator " + buffer2 + ":" + buffer3 + ":Back";
	buffer.Trim();
	DirectionalMeshBack = sbs->engine->CreateSectorWallsMesh (sbs->area, buffer.GetData());
	Directional_back_state = scfQueryInterface<iThingFactoryState> (DirectionalMeshBack->GetMeshObject()->GetFactory());
	DirectionalMeshBack->SetZBufMode(CS_ZBUF_USE);
	DirectionalMeshBack->SetRenderPriority(sbs->engine->GetObjectRenderPriority());

	if (Single == false)
	{
		buffer = "Directional Indicator " + buffer2 + ":" + buffer3 + ":Up";
		DirectionalMeshUp = CS::Geometry::GeneralMeshBuilder::CreateFactoryAndMesh(sbs->engine, sbs->area, buffer, buffer + " factory");
		DirectionalMeshUp->SetZBufMode(CS_ZBUF_USE);
		DirectionalMeshUp->SetRenderPriority(sbs->engine->GetObjectRenderPriority());
		csRef<iMaterialWrapper> mat = sbs->engine->GetMaterialList()->FindByName(UpTextureUnlit);
		DirectionalMeshUp->GetMeshObject()->SetMaterialWrapper(mat);

		buffer = "Directional Indicator " + buffer2 + ":" + buffer3 + ":Down";
		DirectionalMeshDown = CS::Geometry::GeneralMeshBuilder::CreateFactoryAndMesh(sbs->engine, sbs->area, buffer, buffer + " factory");
		DirectionalMeshDown->SetZBufMode(CS_ZBUF_USE);
		DirectionalMeshDown->SetRenderPriority(sbs->engine->GetObjectRenderPriority());
		mat = sbs->engine->GetMaterialList()->FindByName(DownTextureUnlit);
		DirectionalMeshDown->GetMeshObject()->SetMaterialWrapper(mat);
	}
	else
	{
		buffer = "Directional Indicator " + buffer2 + ":" + buffer3 + ":Arrow";
		DirectionalMesh = CS::Geometry::GeneralMeshBuilder::CreateFactoryAndMesh(sbs->engine, sbs->area, buffer, buffer + " factory");
		DirectionalMesh->SetZBufMode(CS_ZBUF_USE);
		DirectionalMesh->SetRenderPriority(sbs->engine->GetAlphaRenderPriority());
		DirectionalMesh->GetMeshObject()->SetMixMode(CS_FX_ALPHA);
		csRef<iMaterialWrapper> mat = sbs->engine->GetMaterialList()->FindByName(UpTextureUnlit);
		DirectionalMesh->GetMeshObject()->SetMaterialWrapper(mat);
	}

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
			sbs->AddWallMain(Directional_back_state, "Panel", BackTexture, 0, CenterX - (BackWidth / 2), CenterZ, CenterX + (BackWidth / 2), CenterZ, BackHeight, BackHeight, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset, tw, th);
			sbs->ResetWalls();
		}
		if (Direction == "left" || Direction == "right")
		{
			if (Direction == "left")
				sbs->DrawWalls(true, false, false, false, false, false);
			else
				sbs->DrawWalls(false, true, false, false, false, false);
			sbs->AddWallMain(Directional_back_state, "Panel", BackTexture, 0, CenterX, CenterZ + (BackWidth / 2), CenterX, CenterZ - (BackWidth / 2), BackHeight, BackHeight, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset, tw, th);
			sbs->ResetWalls();
		}
	}

	//create indicator lanterns
	int bottomfloor = sbs->GetElevator(elevator)->GetBottomFloor();
	int topfloor = sbs->GetElevator(elevator)->GetTopFloor();

	if (Direction == "front" || Direction == "back")
	{
		float x1, x2;
		float offset;
		if (Direction == "front")
		{
			offset = -0.01f;
			x1 = CenterX - (BackWidth / 4);
			x2 = CenterX + (BackWidth / 4);
		}
		else
		{
			offset = 0.01f;
			x2 = CenterX - (BackWidth / 4);
			x1 = CenterX + (BackWidth / 4);
		}
		if (Single == false)
		{
			if (Vertical == true)
			{
				if (floor > bottomfloor && floor < topfloor)
				{
					sbs->AddGenWall(DirectionalMeshUp, UpTextureUnlit, x1, CenterZ + offset, x2, CenterZ + offset, (BackHeight / 7) * 2, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + ((BackHeight / 7) * 4), 1, 1);
					sbs->AddGenWall(DirectionalMeshDown, DownTextureUnlit, x1, CenterZ + offset, x2, CenterZ + offset, (BackHeight / 7) * 2, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (BackHeight / 7), 1, 1);
				}
				else
				{
					if (floor < topfloor)
						sbs->AddGenWall(DirectionalMeshUp, UpTextureUnlit, x1, CenterZ + offset, x2, CenterZ + offset, (BackHeight / 7) * 2, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + ((BackHeight / 7) * 2.5), 1, 1);
					if (floor > bottomfloor)
						sbs->AddGenWall(DirectionalMeshDown, DownTextureUnlit, x1, CenterZ + offset, x2, CenterZ + offset, (BackHeight / 7) * 2, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + ((BackHeight / 7) * 2.5), 1, 1);
				}
			}
			else
			{
				//horizontal lights
				if (floor > bottomfloor && floor < topfloor)
				{
					float x3, x4;
					if (Direction == "front")
					{
						x1 = (CenterX - (BackWidth / 2)) + ((BackWidth / 7) * 4);
						x2 = (CenterX - (BackWidth / 2)) + ((BackWidth / 7) * 6);
						x3 = (CenterX - (BackWidth / 2)) + (BackWidth / 7);
						x4 = (CenterX - (BackWidth / 2)) + ((BackWidth / 7) * 3);
					}
					else
					{
						x1 = (CenterX - (BackWidth / 2)) + ((BackWidth / 7) * 6);
						x2 = (CenterX - (BackWidth / 2)) + ((BackWidth / 7) * 4);
						x3 = (CenterX - (BackWidth / 2)) + ((BackWidth / 7)) * 3;
						x4 = (CenterX - (BackWidth / 2)) + (BackWidth / 7);
					}
					sbs->AddGenWall(DirectionalMeshUp, UpTextureUnlit, x1, CenterZ + offset, x2, CenterZ + offset, (BackHeight / 6) * 4, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (BackHeight / 6), 1, 1);
					sbs->AddGenWall(DirectionalMeshDown, DownTextureUnlit, x3, CenterZ + offset, x4, CenterZ + offset, (BackHeight / 6) * 4, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (BackHeight / 6), 1, 1);
				}
				else
				{
					if (floor < topfloor)
						sbs->AddGenWall(DirectionalMeshUp, UpTextureUnlit, x1, CenterZ + offset, x2, CenterZ + offset, (BackHeight / 7) * 2, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + ((BackHeight / 7) * 2.5), 1, 1);
					if (floor > bottomfloor)
						sbs->AddGenWall(DirectionalMeshDown, DownTextureUnlit, x1, CenterZ + offset, x2, CenterZ + offset, (BackHeight / 7) * 2, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + ((BackHeight / 7) * 2.5), 1, 1);
				}
			}
		}
		else
			sbs->AddGenWall(DirectionalMesh, UpTextureUnlit, x1, CenterZ + offset, x2, CenterZ + offset, (BackHeight / 6) * 4, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (BackHeight / 6), 1, 1);
	}
	else
	{
		float z1, z2;
		float offset;
		if (Direction == "left")
		{
			offset = -0.01f;
			z2 = CenterZ - (BackWidth / 4);
			z1 = CenterZ + (BackWidth / 4);
		}
		else
		{
			//right
			offset = 0.01f;
			z1 = CenterZ - (BackWidth / 4);
			z2 = CenterZ + (BackWidth / 4);
		}
		if (Single == false)
		{
			if (Vertical == true)
			{
				if (floor > bottomfloor && floor < topfloor)
				{
					sbs->AddGenWall(DirectionalMeshUp, UpTextureUnlit, CenterX + offset, z1, CenterX + offset, z2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + ((BackHeight / 7) * 4), 1, 1);
					sbs->AddGenWall(DirectionalMeshDown, DownTextureUnlit, CenterX + offset, z1, CenterX + offset, z2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (BackHeight / 7), 1, 1);
				}
				else
				{
					if (floor < topfloor)
						sbs->AddGenWall(DirectionalMeshUp, UpTextureUnlit, CenterX + offset, z1, CenterX + offset, z2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + ((BackHeight / 7) * 2.5), 1, 1);
					if (floor > bottomfloor)
						sbs->AddGenWall(DirectionalMeshDown, DownTextureUnlit, CenterX + offset, z1, CenterX + offset, z2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + ((BackHeight / 7) * 2.5), 1, 1);
				}
			}
			else
			{
				//horizontal lights
				if (floor > bottomfloor && floor < topfloor)
				{
					float z3, z4;
					if (Direction == "left")
					{
						z1 = (CenterZ - (BackWidth / 2)) + ((BackWidth / 7) * 6);
						z2 = (CenterZ - (BackWidth / 2)) + ((BackWidth / 7) * 4);
						z3 = (CenterZ - (BackWidth / 2)) + ((BackWidth / 7)) * 3;
						z4 = (CenterZ - (BackWidth / 2)) + (BackWidth / 7);
					}
					else
					{
						z1 = (CenterZ - (BackWidth / 2)) + ((BackWidth / 7) * 4);
						z2 = (CenterZ - (BackWidth / 2)) + ((BackWidth / 7) * 6);
						z3 = (CenterZ - (BackWidth / 2)) + (BackWidth / 7);
						z4 = (CenterZ - (BackWidth / 2)) + ((BackWidth / 7) * 3);
					}
					sbs->AddGenWall(DirectionalMeshUp, UpTextureUnlit, CenterX + offset, z1, CenterX + offset, z2, (BackHeight / 6) * 4, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (BackHeight / 6), 1, 1);
					sbs->AddGenWall(DirectionalMeshDown, DownTextureUnlit, CenterX + offset, z3, CenterX + offset, z4, (BackHeight / 6) * 4, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (BackHeight / 6), 1, 1);
				}
				else
				{
					if (floor < topfloor)
						sbs->AddGenWall(DirectionalMeshUp, UpTextureUnlit, CenterX + offset, z1, CenterX + offset, z2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + ((BackHeight / 7) * 2.5), 1, 1);
					if (floor > bottomfloor)
						sbs->AddGenWall(DirectionalMeshDown, DownTextureUnlit, CenterX + offset, z1, CenterX + offset, z2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + ((BackHeight / 7) * 2.5), 1, 1);
				}
			}
		}
		else
			sbs->AddGenWall(DirectionalMesh, UpTextureUnlit, CenterX + offset, z1, CenterX + offset, z2, (BackHeight / 6) * 4, sbs->GetFloor(floor)->Altitude + sbs->GetFloor(floor)->InterfloorHeight + voffset + (BackHeight / 6), 1, 1);
	}
	sbs->ResetExtents();
}

DirectionalIndicator::~DirectionalIndicator()
{

}

void DirectionalIndicator::Enabled(bool value)
{
	//turns panel on/off
	if (value == IsEnabled)
		return;

	sbs->EnableMesh(DirectionalMeshBack, value);
	if (DirectionalMeshUp)
		sbs->EnableMesh(DirectionalMeshUp, value);
	if (DirectionalMeshDown)
		sbs->EnableMesh(DirectionalMeshDown, value);
	if (DirectionalMesh)
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

	//set light status
	if (value == true)
		SetLights(1, 0);
	else
		SetLights(2, 0);

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

	//set light status
	if (value == true)
		SetLights(0, 1);
	else
		SetLights(0, 2);

	DownStatus = value;
}

void DirectionalIndicator::SetLights(int up, int down)
{
	//set status of directional lights
	//values are 0 for no change, 1 for on, and 2 for off

	if (Single == false)
	{
		if (up == 1 && DirectionalMeshUp)
			sbs->ChangeTexture(DirectionalMeshUp, UpTextureLit);
		if (up == 2 && DirectionalMeshUp)
			sbs->ChangeTexture(DirectionalMeshUp, UpTextureUnlit);
		if (down == 1 && DirectionalMeshDown)
			sbs->ChangeTexture(DirectionalMeshDown, DownTextureLit);
		if (down == 2 && DirectionalMeshDown)
			sbs->ChangeTexture(DirectionalMeshDown, DownTextureUnlit);
	}
	else
	{
		if (DirectionalMesh)
		{
			if (up == 1)
				sbs->ChangeTexture(DirectionalMesh, UpTextureLit);
			if (down == 1)
				sbs->ChangeTexture(DirectionalMesh, DownTextureLit);
			if (up == 2 || down == 2)
				sbs->ChangeTexture(DirectionalMesh, UpTextureUnlit);
		}
	}
}
