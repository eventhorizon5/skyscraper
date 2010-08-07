/* $Id$ */

/*
	Scalable Building Simulator - Directional Indicator Class
	The Skyscraper Project - Version 1.7 Alpha
	Copyright (C)2004-2010 Ryan Thoryk
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
#include "sbs.h"
#include "directional.h"
#include "elevator.h"

extern SBS *sbs; //external pointer to the SBS engine

DirectionalIndicator::DirectionalIndicator(Object *parent, int elevator, int floor, bool active_direction, bool single, bool vertical, const char *BackTexture, const char *uptexture, const char *uptexture_lit, const char *downtexture, const char *downtexture_lit, float CenterX, float CenterZ, float voffset, const char *direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th)
{
	//create a directional indicator
	//if InElevator is true, the floor parameter is ignored

	//set up SBS object
	object = new Object();
	object->SetValues(this, parent, "DirectionalIndicator", "", false);

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
	ActiveDirection = active_direction;

	//create object mesh
	csString buffer, buffer2, buffer3, buffer4;
	buffer2 = elevator;
	buffer3 = floor;
	buffer4 = object->GetNumber();
	buffer = "(" + buffer4 + ")Directional Indicator " + buffer2 + ":" + buffer3 + ":Back";
	buffer.Trim();
	object->SetName("Directional Indicator " + buffer2 + ":" + buffer3);
	DirectionalMeshBack = sbs->CreateMesh(buffer);
	DirectionalMeshBack->SetZBufMode(CS_ZBUF_USE);
	DirectionalMeshBack->SetRenderPriority(sbs->engine->GetObjectRenderPriority());

	if (Single == false)
	{
		buffer = "(" + buffer4 + ")Directional Indicator " + buffer2 + ":" + buffer3 + ":Up";
		DirectionalMeshUp = sbs->CreateMesh(buffer);
		DirectionalMeshUp->SetZBufMode(CS_ZBUF_USE);
		DirectionalMeshUp->SetRenderPriority(sbs->engine->GetObjectRenderPriority());

		buffer = "(" + buffer4 + ")Directional Indicator " + buffer2 + ":" + buffer3 + ":Down";
		DirectionalMeshDown = sbs->CreateMesh(buffer);
		DirectionalMeshDown->SetZBufMode(CS_ZBUF_USE);
		DirectionalMeshDown->SetRenderPriority(sbs->engine->GetObjectRenderPriority());
	}
	else
	{
		buffer = "(" + buffer4 + ")Directional Indicator " + buffer2 + ":" + buffer3 + ":Arrow";
		DirectionalMesh = sbs->CreateMesh(buffer);
		DirectionalMesh->SetZBufMode(CS_ZBUF_USE);
		DirectionalMesh->SetRenderPriority(sbs->engine->GetObjectRenderPriority());
	}

	sbs->ResetTextureMapping(true);

	//create panel
	if (ShowBack == true)
	{
		if (Direction == "front" || Direction == "back")
		{
			float x1, x2;
			if (Direction == "front")
			{
				x1 = CenterX - (BackWidth / 2);
				x2 = CenterX + (BackWidth / 2);
			}
			else
			{
				x2 = CenterX - (BackWidth / 2);
				x1 = CenterX + (BackWidth / 2);
			}
			sbs->AddGenWall(DirectionalMeshBack, BackTexture, x1, CenterZ, x2, CenterZ, BackHeight, sbs->GetFloor(floor)->GetBase() + voffset, tw, th);
			sbs->ResetWalls();
		}
		if (Direction == "left" || Direction == "right")
		{
			float z1, z2;
			if (Direction == "left")
			{
				z2 = CenterZ - (BackWidth / 2);
				z1 = CenterZ + (BackWidth / 2);
			}
			else
			{
				//right
				z1 = CenterZ - (BackWidth / 2);
				z2 = CenterZ + (BackWidth / 2);
			}
			sbs->AddGenWall(DirectionalMeshBack, BackTexture, CenterX, z1, CenterX, z2, BackHeight, sbs->GetFloor(floor)->GetBase() + voffset, tw, th);
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
				if ((floor > bottomfloor && floor < topfloor) || ActiveDirection == true)
				{
					sbs->AddGenWall(DirectionalMeshUp, UpTextureUnlit, x1, CenterZ + offset, x2, CenterZ + offset, (BackHeight / 7) * 2, sbs->GetFloor(floor)->GetBase() + voffset + ((BackHeight / 7) * 4), 1, 1);
					sbs->AddGenWall(DirectionalMeshDown, DownTextureUnlit, x1, CenterZ + offset, x2, CenterZ + offset, (BackHeight / 7) * 2, sbs->GetFloor(floor)->GetBase() + voffset + (BackHeight / 7), 1, 1);
				}
				else
				{
					if (floor < topfloor)
						sbs->AddGenWall(DirectionalMeshUp, UpTextureUnlit, x1, CenterZ + offset, x2, CenterZ + offset, (BackHeight / 7) * 2, sbs->GetFloor(floor)->GetBase() + voffset + ((BackHeight / 7) * 2.5), 1, 1);
					if (floor > bottomfloor)
						sbs->AddGenWall(DirectionalMeshDown, DownTextureUnlit, x1, CenterZ + offset, x2, CenterZ + offset, (BackHeight / 7) * 2, sbs->GetFloor(floor)->GetBase() + voffset + ((BackHeight / 7) * 2.5), 1, 1);
				}
			}
			else
			{
				//horizontal lights
				if ((floor > bottomfloor && floor < topfloor) || ActiveDirection == true)
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
					sbs->AddGenWall(DirectionalMeshUp, UpTextureUnlit, x1, CenterZ + offset, x2, CenterZ + offset, (BackHeight / 6) * 4, sbs->GetFloor(floor)->GetBase() + voffset + (BackHeight / 6), 1, 1);
					sbs->AddGenWall(DirectionalMeshDown, DownTextureUnlit, x3, CenterZ + offset, x4, CenterZ + offset, (BackHeight / 6) * 4, sbs->GetFloor(floor)->GetBase() + voffset + (BackHeight / 6), 1, 1);
				}
				else
				{
					if (floor < topfloor)
						sbs->AddGenWall(DirectionalMeshUp, UpTextureUnlit, x1, CenterZ + offset, x2, CenterZ + offset, (BackHeight / 7) * 2, sbs->GetFloor(floor)->GetBase() + voffset + ((BackHeight / 7) * 2.5), 1, 1);
					if (floor > bottomfloor)
						sbs->AddGenWall(DirectionalMeshDown, DownTextureUnlit, x1, CenterZ + offset, x2, CenterZ + offset, (BackHeight / 7) * 2, sbs->GetFloor(floor)->GetBase() + voffset + ((BackHeight / 7) * 2.5), 1, 1);
				}
			}
		}
		else
			sbs->AddGenWall(DirectionalMesh, UpTextureUnlit, x1, CenterZ + offset, x2, CenterZ + offset, (BackHeight / 6) * 4, sbs->GetFloor(floor)->GetBase() + voffset + (BackHeight / 6), 1, 1);
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
				if ((floor > bottomfloor && floor < topfloor) || ActiveDirection == true)
				{
					sbs->AddGenWall(DirectionalMeshUp, UpTextureUnlit, CenterX + offset, z1, CenterX + offset, z2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->GetBase() + voffset + ((BackHeight / 7) * 4), 1, 1);
					sbs->AddGenWall(DirectionalMeshDown, DownTextureUnlit, CenterX + offset, z1, CenterX + offset, z2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->GetBase() + voffset + (BackHeight / 7), 1, 1);
				}
				else
				{
					if (floor < topfloor)
						sbs->AddGenWall(DirectionalMeshUp, UpTextureUnlit, CenterX + offset, z1, CenterX + offset, z2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->GetBase() + voffset + ((BackHeight / 7) * 2.5), 1, 1);
					if (floor > bottomfloor)
						sbs->AddGenWall(DirectionalMeshDown, DownTextureUnlit, CenterX + offset, z1, CenterX + offset, z2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->GetBase() + voffset + ((BackHeight / 7) * 2.5), 1, 1);
				}
			}
			else
			{
				//horizontal lights
				if ((floor > bottomfloor && floor < topfloor) || ActiveDirection == true)
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
					sbs->AddGenWall(DirectionalMeshUp, UpTextureUnlit, CenterX + offset, z1, CenterX + offset, z2, (BackHeight / 6) * 4, sbs->GetFloor(floor)->GetBase() + voffset + (BackHeight / 6), 1, 1);
					sbs->AddGenWall(DirectionalMeshDown, DownTextureUnlit, CenterX + offset, z3, CenterX + offset, z4, (BackHeight / 6) * 4, sbs->GetFloor(floor)->GetBase() + voffset + (BackHeight / 6), 1, 1);
				}
				else
				{
					if (floor < topfloor)
						sbs->AddGenWall(DirectionalMeshUp, UpTextureUnlit, CenterX + offset, z1, CenterX + offset, z2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->GetBase() + voffset + ((BackHeight / 7) * 2.5), 1, 1);
					if (floor > bottomfloor)
						sbs->AddGenWall(DirectionalMeshDown, DownTextureUnlit, CenterX + offset, z1, CenterX + offset, z2, (BackHeight / 7) * 2, sbs->GetFloor(floor)->GetBase() + voffset + ((BackHeight / 7) * 2.5), 1, 1);
				}
			}
		}
		else
			sbs->AddGenWall(DirectionalMesh, UpTextureUnlit, CenterX + offset, z1, CenterX + offset, z2, (BackHeight / 6) * 4, sbs->GetFloor(floor)->GetBase() + voffset + (BackHeight / 6), 1, 1);
	}
	sbs->ResetTextureMapping();
}

DirectionalIndicator::~DirectionalIndicator()
{
	if (sbs->FastDelete == false)
	{
		sbs->engine->WantToDie(DirectionalMeshBack);
		sbs->engine->WantToDie(DirectionalMesh);
		sbs->engine->WantToDie(DirectionalMeshUp);
		sbs->engine->WantToDie(DirectionalMeshDown);

		//unregister from parent
		if (object->parent_deleting == false)
		{
			if (csString(object->GetParent()->GetType()) == "Elevator")
				((Elevator*)object->GetParent()->GetRawObject())->RemoveDirectionalIndicator(this);
			if (csString(object->GetParent()->GetType()) == "Floor")
				((Floor*)object->GetParent()->GetRawObject())->RemoveDirectionalIndicator(this);
		}
	}
	DirectionalMeshBack = 0;
	DirectionalMesh = 0;
	DirectionalMeshUp = 0;
	DirectionalMeshDown = 0;
	delete object;
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

	//ignore if turning off other indicator in single mode
	if (Single == true && value == false && DownStatus == true)
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

	//ignore if turning off other indicator in single mode
	if (Single == true && value == false && UpStatus == true)
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

void DirectionalIndicator::SetPosition(csVector3 position)
{
	//set position of indicator objects

	csVector3 positionnew = sbs->ToRemote(position);
	if (DirectionalMeshBack)
	{
		DirectionalMeshBack->GetMovable()->SetPosition(positionnew);
		DirectionalMeshBack->GetMovable()->UpdateMove();
	}
	if (DirectionalMeshUp)
	{
		DirectionalMeshUp->GetMovable()->SetPosition(positionnew);
		DirectionalMeshUp->GetMovable()->UpdateMove();
	}
	if (DirectionalMeshDown)
	{
		DirectionalMeshDown->GetMovable()->SetPosition(positionnew);
		DirectionalMeshDown->GetMovable()->UpdateMove();
	}
	if (DirectionalMesh)
	{
		DirectionalMesh->GetMovable()->SetPosition(positionnew);
		DirectionalMesh->GetMovable()->UpdateMove();
	}
}

void DirectionalIndicator::Move(csVector3 position)
{
	//move indicator objects

	csVector3 positionnew = sbs->ToRemote(position);
	if (DirectionalMeshBack)
	{
		DirectionalMeshBack->GetMovable()->MovePosition(positionnew);
		DirectionalMeshBack->GetMovable()->UpdateMove();
	}
	if (DirectionalMeshUp)
	{
		DirectionalMeshUp->GetMovable()->MovePosition(positionnew);
		DirectionalMeshUp->GetMovable()->UpdateMove();
	}
	if (DirectionalMeshDown)
	{
		DirectionalMeshDown->GetMovable()->MovePosition(positionnew);
		DirectionalMeshDown->GetMovable()->UpdateMove();
	}
	if (DirectionalMesh)
	{
		DirectionalMesh->GetMovable()->MovePosition(positionnew);
		DirectionalMesh->GetMovable()->UpdateMove();
	}
}

csVector3 DirectionalIndicator::GetPosition()
{
	//return indicator position
	return sbs->ToLocal(DirectionalMeshBack->GetMovable()->GetPosition());
}
