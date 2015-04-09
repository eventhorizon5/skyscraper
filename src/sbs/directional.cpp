/* $Id$ */

/*
	Scalable Building Simulator - Directional Indicator Class
	The Skyscraper Project - Version 1.9 Alpha
	Copyright (C)2004-2015 Ryan Thoryk
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

#include <OgreMaterial.h>
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
	DirectionalMeshUp = 0;
	DirectionalMeshDown = 0;
	DirectionalMesh = 0;
	timer = 0;
	timer_interval = sbs->GetConfigInt("Skyscraper.SBS.DirectionalIndicator.Timer", 10000);

	bool in_elevator = false;
	if (std::string(parent->GetType()) == "Elevator")
		in_elevator = true;

	//create timer
	if (ActiveDirection == false)
		timer = new Timer("Shut-off Timer", this);

	//create object mesh
	std::string base, buffer;
	base = "Directional Indicator " + ToString2(elevator) + ":" + ToString2(floor);
	buffer = base + ":Back";
	object->SetName(base.c_str());
	DirectionalMeshBack = new MeshObject(object, buffer.c_str(), 0, sbs->GetConfigFloat("Skyscraper.SBS.MaxSmallRenderDistance", 100));

	if (Single == false)
	{
		buffer = base + ":Up";
		DirectionalMeshUp = new MeshObject(object, buffer.c_str(), 0, sbs->GetConfigFloat("Skyscraper.SBS.MaxSmallRenderDistance", 100));

		buffer = base + ":Down";
		DirectionalMeshDown = new MeshObject(object, buffer.c_str(), 0, sbs->GetConfigFloat("Skyscraper.SBS.MaxSmallRenderDistance", 100));
	}
	else
	{
		buffer = base + ":Arrow";
		DirectionalMesh = new MeshObject(object, buffer.c_str(), 0, sbs->GetConfigFloat("Skyscraper.SBS.MaxSmallRenderDistance", 100));
	}

	sbs->ResetTextureMapping(true);

	//create panel
	if (ShowBack == true)
	{
		if (Direction == "front" || Direction == "back")
		{
			if (Direction == "front")
				sbs->DrawWalls(true, false, false, false, false, false);
			else
				sbs->DrawWalls(false, true, false, false, false, false);
			sbs->AddWallMain(object, DirectionalMeshBack, "Panel", BackTexture, 0, CenterX - (BackWidth / 2), CenterZ, CenterX + (BackWidth / 2), CenterZ, BackHeight, BackHeight, sbs->GetFloor(floor)->GetBase() + voffset, sbs->GetFloor(floor)->GetBase() + voffset, tw, th, false);
			sbs->ResetWalls();

		}
		if (Direction == "left" || Direction == "right")
		{
			if (Direction == "left")
				sbs->DrawWalls(true, false, false, false, false, false);
			else
				sbs->DrawWalls(false, true, false, false, false, false);
			sbs->AddWallMain(object, DirectionalMeshBack, "Panel", BackTexture, 0, CenterX, CenterZ + (BackWidth / 2), CenterX, CenterZ - (BackWidth / 2), BackHeight, BackHeight, sbs->GetFloor(floor)->GetBase() + voffset, sbs->GetFloor(floor)->GetBase() + voffset, tw, th, false);
			sbs->ResetWalls();
		}
	}

	//create indicator lanterns
	int bottomfloor = sbs->GetElevator(elevator)->GetBottomFloor();
	int topfloor = sbs->GetElevator(elevator)->GetTopFloor();

	if (Direction == "front" || Direction == "back")
	{
		float x1 = CenterX - (BackWidth / 4);
		float x2 = CenterX + (BackWidth / 4);
		float offset;
		if (Direction == "front")
		{
			offset = -0.01f;
			sbs->DrawWalls(true, false, false, false, false, false);
		}
		else
		{
			offset = 0.01f;
			sbs->DrawWalls(false, true, false, false, false, false);
		}
		if (Single == false)
		{
			if (Vertical == true)
			{
				if ((floor > bottomfloor && floor < topfloor) || ActiveDirection == true || in_elevator == true)
				{
					float height = (BackHeight / 7) * 2;
					float altitude, altitude2;
					if (in_elevator == false)
					{
						altitude = sbs->GetFloor(floor)->GetBase() + voffset + ((BackHeight / 7) * 4);
						altitude2 = sbs->GetFloor(floor)->GetBase() + voffset + (BackHeight / 7);
					}
					else
					{
						altitude = voffset + ((BackHeight / 7) * 4);
						altitude2 = voffset + (BackHeight / 7);
					}
					sbs->AddWallMain(object, DirectionalMeshUp, "DirectionalUp", UpTextureUnlit.c_str(), 0, x1, CenterZ + offset, x2, CenterZ + offset, height, height, altitude, altitude, 1, 1, false);
					sbs->AddWallMain(object, DirectionalMeshDown, "DirectionalDown", DownTextureUnlit.c_str(), 0, x1, CenterZ + offset, x2, CenterZ + offset, height, height, altitude2, altitude2, 1, 1, false);
				}
				else
				{
					float height = (BackHeight / 7) * 2;
					float altitude = sbs->GetFloor(floor)->GetBase() + voffset + ((BackHeight / 7) * 2.5f);
					if (floor < topfloor)
						sbs->AddWallMain(object, DirectionalMeshUp, "DirectionalUp", UpTextureUnlit.c_str(), 0, x1, CenterZ + offset, x2, CenterZ + offset, height, height, altitude, altitude, 1, 1, false);
					if (floor > bottomfloor)
						sbs->AddWallMain(object, DirectionalMeshDown, "DirectionalDown", DownTextureUnlit.c_str(), 0, x1, CenterZ + offset, x2, CenterZ + offset, height, height, altitude, altitude, 1, 1, false);
				}
			}
			else
			{
				//horizontal lights
				if ((floor > bottomfloor && floor < topfloor) || ActiveDirection == true || in_elevator == true)
				{
					x1 = (CenterX - (BackWidth / 2)) + ((BackWidth / 7) * 4);
					x2 = (CenterX - (BackWidth / 2)) + ((BackWidth / 7) * 6);
					float x3 = (CenterX - (BackWidth / 2)) + (BackWidth / 7);
					float x4 = (CenterX - (BackWidth / 2)) + ((BackWidth / 7) * 3);
					float height = (BackHeight / 6) * 4;
					float altitude;
					if (in_elevator == false)
						altitude = sbs->GetFloor(floor)->GetBase() + voffset + (BackHeight / 6);
					else
						altitude = voffset + (BackHeight / 6);

					sbs->AddWallMain(object, DirectionalMeshUp, "DirectionalUp", UpTextureUnlit.c_str(), 0, x1, CenterZ + offset, x2, CenterZ + offset, height, height, altitude, altitude, 1, 1, false);
					sbs->AddWallMain(object, DirectionalMeshDown, "DirectionalDown", DownTextureUnlit.c_str(), 0, x3, CenterZ + offset, x4, CenterZ + offset, height, height, altitude, altitude, 1, 1, false);
				}
				else
				{
					float height = (BackHeight / 7) * 2;
					float altitude = sbs->GetFloor(floor)->GetBase() + voffset + ((BackHeight / 7) * 2.5f);
					if (floor < topfloor)
						sbs->AddWallMain(object, DirectionalMeshUp, "DirectionalUp", UpTextureUnlit.c_str(), 0, x1, CenterZ + offset, x2, CenterZ + offset, height, height, altitude, altitude, 1, 1, false);
					if (floor > bottomfloor)
						sbs->AddWallMain(object, DirectionalMeshDown, "DirectionalDown", DownTextureUnlit.c_str(), 0, x1, CenterZ + offset, x2, CenterZ + offset, height, height, altitude, altitude, 1, 1, false);
				}
			}
		}
		else
		{
			float height = (BackHeight / 6) * 4;
			float altitude = sbs->GetFloor(floor)->GetBase() + voffset + (BackHeight / 6);
			sbs->AddWallMain(object, DirectionalMesh, "Directional", UpTextureUnlit.c_str(), 0, x1, CenterZ + offset, x2, CenterZ + offset, height, height, altitude, altitude, 1, 1, false);
		}
		sbs->ResetWalls();
	}
	else
	{
		float z1 = CenterZ - (BackWidth / 4);
		float z2 = CenterZ + (BackWidth / 4);
		float offset;
		if (Direction == "left")
		{
			offset = -0.01f;
			sbs->DrawWalls(true, false, false, false, false, false);
		}
		else
		{
			//right
			offset = 0.01f;
			sbs->DrawWalls(false, true, false, false, false, false);
		}
		if (Single == false)
		{
			if (Vertical == true)
			{
				if ((floor > bottomfloor && floor < topfloor) || ActiveDirection == true || in_elevator == true)
				{
					float height = (BackHeight / 7) * 2;
					float altitude = sbs->GetFloor(floor)->GetBase() + voffset + ((BackHeight / 7) * 4);
					float altitude2 = sbs->GetFloor(floor)->GetBase() + voffset + (BackHeight / 7);
					sbs->AddWallMain(object, DirectionalMeshUp, "DirectionalUp", UpTextureUnlit.c_str(), 0, CenterX + offset, z1, CenterX + offset, z2, height, height, altitude, altitude, 1, 1, false);
					sbs->AddWallMain(object, DirectionalMeshDown, "DirectionalDown", DownTextureUnlit.c_str(), 0, CenterX + offset, z1, CenterX + offset, z2, height, height, altitude2, altitude2, 1, 1, false);
				}
				else
				{
					float height = (BackHeight / 7) * 2;
					float altitude = sbs->GetFloor(floor)->GetBase() + voffset + ((BackHeight / 7) * 2.5f);
					if (floor < topfloor)
						sbs->AddWallMain(object, DirectionalMeshUp, "DirectionalUp", UpTextureUnlit.c_str(), 0, CenterX + offset, z1, CenterX + offset, z2, height, height, altitude, altitude, 1, 1, false);
					if (floor > bottomfloor)
						sbs->AddWallMain(object, DirectionalMeshDown, "DirectionalDown", DownTextureUnlit.c_str(), 0, CenterX + offset, z1, CenterX + offset, z2, height, height, altitude, altitude, 1, 1, false);
				}
			}
			else
			{
				//horizontal lights
				if ((floor > bottomfloor && floor < topfloor) || ActiveDirection == true || in_elevator == true)
				{
					z1 = (CenterZ - (BackWidth / 2)) + ((BackWidth / 7) * 4);
					z2 = (CenterZ - (BackWidth / 2)) + ((BackWidth / 7) * 6);
					float z3 = (CenterZ - (BackWidth / 2)) + (BackWidth / 7);
					float z4 = (CenterZ - (BackWidth / 2)) + ((BackWidth / 7) * 3);
					float height = (BackHeight / 6) * 4;
					float altitude = sbs->GetFloor(floor)->GetBase() + voffset + (BackHeight / 6);
					sbs->AddWallMain(object, DirectionalMeshUp, "DirectionalUp", UpTextureUnlit.c_str(), 0, CenterX + offset, z1, CenterX + offset, z2, height, height, altitude, altitude, 1, 1, false);
					sbs->AddWallMain(object, DirectionalMeshDown, "DirectionalDown", DownTextureUnlit.c_str(), 0, CenterX + offset, z3, CenterX + offset, z4, height, height, altitude, altitude, 1, 1, false);
				}
				else
				{
					float height = (BackHeight / 7) * 2;
					float altitude = sbs->GetFloor(floor)->GetBase() + voffset + ((BackHeight / 7) * 2.5f);
					if (floor < topfloor)
						sbs->AddWallMain(object, DirectionalMeshUp, "DirectionalUp", UpTextureUnlit.c_str(), 0, CenterX + offset, z1, CenterX + offset, z2, height, height, altitude, altitude, 1, 1, false);
					if (floor > bottomfloor)
						sbs->AddWallMain(object, DirectionalMeshDown, "DirectionalDown", DownTextureUnlit.c_str(), 0, CenterX + offset, z1, CenterX + offset, z2, height, height, altitude, altitude, 1, 1, false);
				}
			}
		}
		else
		{
			float height = (BackHeight / 6) * 4;
			float altitude = sbs->GetFloor(floor)->GetBase() + voffset + (BackHeight / 6);
			sbs->AddWallMain(object, DirectionalMesh, "Directional", UpTextureUnlit.c_str(), 0, CenterX + offset, z1, CenterX + offset, z2, height, height, altitude, altitude, 1, 1, false);
		}
		sbs->ResetWalls();
	}
	sbs->ResetTextureMapping();
}

DirectionalIndicator::~DirectionalIndicator()
{
	if (timer)
		delete timer;
	timer = 0;

	if (DirectionalMeshBack)
		delete DirectionalMeshBack;
	DirectionalMeshBack = 0;
	
	if (DirectionalMesh)
		delete DirectionalMesh;
	DirectionalMesh = 0;
	
	if (DirectionalMeshUp)
		delete DirectionalMeshUp;
	DirectionalMeshUp = 0;
	
	if (DirectionalMeshDown)
		delete DirectionalMeshDown;
	DirectionalMeshDown = 0;

	//unregister from parent
	if (sbs->FastDelete == false && object->parent_deleting == false)
	{
		if (std::string(object->GetParent()->GetType()) == "Elevator")
			((Elevator*)object->GetParent()->GetRawObject())->RemoveDirectionalIndicator(this);
		if (std::string(object->GetParent()->GetType()) == "Floor")
			((Floor*)object->GetParent()->GetRawObject())->RemoveDirectionalIndicator(this);
	}

	delete object;
}

void DirectionalIndicator::Enabled(bool value)
{
	//turns panel on/off

	if (value == IsEnabled)
		return;

	DirectionalMeshBack->Enable(value);
	if (DirectionalMeshUp)
		DirectionalMeshUp->Enable(value);
	if (DirectionalMeshDown)
		DirectionalMeshDown->Enable(value);
	if (DirectionalMesh)
		DirectionalMesh->Enable(value);

	IsEnabled = value;
}

void DirectionalIndicator::UpLight(bool value)
{
	//turn on the 'up' directional light

	Elevator *elev = sbs->GetElevator(elevator_num);

	if (!elev)
		value = false;

	if (timer && elev)
	{
		//stop or start timer
		if (value == false && DownStatus == false)
			timer->Stop();
		else if (value == true && (elev->AutoDoors == false || elev->ShaftDoorsExist(0, floor_num) == false))
			timer->Start(timer_interval, true);
	}

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

	Elevator *elev = sbs->GetElevator(elevator_num);

	if (!elev)
		value = false;

	if (timer && elev)
	{
		//stop or start timer
		if (value == false && UpStatus == false)
			timer->Stop();
		else if (value == true && (elev->AutoDoors == false || elev->ShaftDoorsExist(0, floor_num) == false))
			timer->Start(timer_interval, true);
	}

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
			DirectionalMeshUp->ChangeTexture(UpTextureLit.c_str());
		if (up == 2 && DirectionalMeshUp)
			DirectionalMeshUp->ChangeTexture(UpTextureUnlit.c_str());
		if (down == 1 && DirectionalMeshDown)
			DirectionalMeshDown->ChangeTexture(DownTextureLit.c_str());
		if (down == 2 && DirectionalMeshDown)
			DirectionalMeshDown->ChangeTexture(DownTextureUnlit.c_str());
	}
	else
	{
		if (DirectionalMesh)
		{
			if (up == 1)
				DirectionalMesh->ChangeTexture(UpTextureLit.c_str());
			if (down == 1)
				DirectionalMesh->ChangeTexture(DownTextureLit.c_str());
			if (up == 2 || down == 2)
				DirectionalMesh->ChangeTexture(UpTextureUnlit.c_str());
		}
	}
}

void DirectionalIndicator::SetPosition(const Ogre::Vector3& position)
{
	//set position of indicator objects

	if (DirectionalMeshBack)
		DirectionalMeshBack->Move(position, false, false, false);
	if (DirectionalMeshUp)
		DirectionalMeshUp->Move(position, false, false, false);
	if (DirectionalMeshDown)
		DirectionalMeshDown->Move(position, false, false, false);
	if (DirectionalMesh)
		DirectionalMesh->Move(position, false, false, false);
}

void DirectionalIndicator::Move(const Ogre::Vector3& position)
{
	//move indicator objects

	if (DirectionalMeshBack)
		DirectionalMeshBack->Move(position, true, true, true);
	if (DirectionalMeshUp)
		DirectionalMeshUp->Move(position, true, true, true);
	if (DirectionalMeshDown)
		DirectionalMeshDown->Move(position, true, true, true);
	if (DirectionalMesh)
		DirectionalMesh->Move(position, true, true, true);
}

Ogre::Vector3 DirectionalIndicator::GetPosition()
{
	//return indicator position
	return DirectionalMeshBack->GetPosition();
}

void DirectionalIndicator::Timer::Notify()
{
	//when shut-off timer triggers, switch off lights

	if (indicator->UpStatus == false && indicator->DownStatus == false)
		return;

	//turn off all lights
	indicator->UpLight(false);
	indicator->DownLight(false);
}
