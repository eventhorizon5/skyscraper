/* $Id$ */

/*
	Scalable Building Simulator - Directional Indicator Object
	The Skyscraper Project - Version 1.10 Alpha
	Copyright (C)2004-2016 Ryan Thoryk
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
#include "floor.h"
#include "mesh.h"
#include "texture.h"
#include "elevator.h"
#include "elevatorcar.h"
#include "timer.h"
#include "directional.h"

namespace SBS {

//shut-off timer
class DirectionalIndicator::Timer : public TimerObject
{
public:
	DirectionalIndicator *indicator;
	Timer(const std::string &name, DirectionalIndicator *parent) : TimerObject(parent, name)
	{
		indicator = parent;
	}
	virtual void Notify();
};

DirectionalIndicator::DirectionalIndicator(Object *parent, int elevator, int car, int floor, bool active_direction, bool single, bool vertical, const std::string &BackTexture, const std::string &uptexture, const std::string &uptexture_lit, const std::string &downtexture, const std::string &downtexture_lit, float CenterX, float CenterZ, float voffset, const std::string &direction, float BackWidth, float BackHeight, bool ShowBack, float tw, float th) : Object(parent)
{
	//create a directional indicator

	//set up SBS object
	SetValues("DirectionalIndicator", "", false);

	is_enabled = true;
	this->elevator = elevator;
	this->car = car;
	this->floor = floor;
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
	timer_interval = sbs->GetConfigInt("Skyscraper.SBS.DirectionalIndicator.Timer", 15000);

	//validate objects
	if (!sbs->GetElevator(elevator))
		return;
	if (!sbs->GetElevator(elevator)->GetCar(car))
		return;
	if (!sbs->GetFloor(floor))
		return;

	bool in_elevator = false;
	if (parent->GetType() == "Elevator")
		in_elevator = true;

	//create timer
	if (ActiveDirection == false)
		timer = new Timer("Shut-off Timer", this);

	//move object
	Move(CenterX, voffset, CenterZ);

	//create object mesh
	std::string base = "Directional Indicator " + ToString(elevator) + ":" + ToString(car) + ":" + ToString(floor);
	SetName(base);
	DirectionalMeshBack = new MeshObject(this, base + ":Back", 0, "", sbs->GetConfigFloat("Skyscraper.SBS.MaxSmallRenderDistance", 100));

	if (Single == false)
	{
		DirectionalMeshUp = new MeshObject(this, base + ":Up", 0, "", sbs->GetConfigFloat("Skyscraper.SBS.MaxSmallRenderDistance", 100));
		DirectionalMeshDown = new MeshObject(this, base + ":Down", 0, "", sbs->GetConfigFloat("Skyscraper.SBS.MaxSmallRenderDistance", 100));
	}
	else
		DirectionalMesh = new MeshObject(this, base + ":Arrow", 0, "", sbs->GetConfigFloat("Skyscraper.SBS.MaxSmallRenderDistance", 100));

	sbs->GetTextureManager()->ResetTextureMapping(true);

	//create panel
	if (ShowBack == true)
	{
		if (Direction == "front" || Direction == "back")
		{
			if (Direction == "front")
				sbs->DrawWalls(true, false, false, false, false, false);
			else
				sbs->DrawWalls(false, true, false, false, false, false);
			sbs->AddWallMain(this, DirectionalMeshBack, "Panel", BackTexture, 0, -BackWidth / 2, 0, BackWidth / 2, 0, BackHeight, BackHeight, 0, 0, tw, th, false);
			sbs->ResetWalls();

		}
		if (Direction == "left" || Direction == "right")
		{
			if (Direction == "left")
				sbs->DrawWalls(true, false, false, false, false, false);
			else
				sbs->DrawWalls(false, true, false, false, false, false);
			sbs->AddWallMain(this, DirectionalMeshBack, "Panel", BackTexture, 0, 0, BackWidth / 2, 0, -BackWidth / 2, BackHeight, BackHeight, 0, 0, tw, th, false);
			sbs->ResetWalls();
		}
	}

	//create indicator lanterns
	int bottomfloor = sbs->GetElevator(elevator)->GetCar(car)->GetBottomFloor();
	int topfloor = sbs->GetElevator(elevator)->GetCar(car)->GetTopFloor();

	if (Direction == "front" || Direction == "back")
	{
		float x1 = -BackWidth / 4;
		float x2 = BackWidth / 4;
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
					float altitude = (BackHeight / 7) * 4;
					float altitude2 = BackHeight / 7;

					sbs->AddWallMain(this, DirectionalMeshUp, "DirectionalUp", UpTextureUnlit, 0, x1, offset, x2, offset, height, height, altitude, altitude, 1, 1, false);
					sbs->AddWallMain(this, DirectionalMeshDown, "DirectionalDown", DownTextureUnlit, 0, x1, offset, x2, offset, height, height, altitude2, altitude2, 1, 1, false);
				}
				else
				{
					float height = (BackHeight / 7) * 2;
					float altitude = (BackHeight / 7) * 2.5f;
					if (floor < topfloor)
						sbs->AddWallMain(this, DirectionalMeshUp, "DirectionalUp", UpTextureUnlit, 0, x1, offset, x2, offset, height, height, altitude, altitude, 1, 1, false);
					if (floor > bottomfloor)
						sbs->AddWallMain(this, DirectionalMeshDown, "DirectionalDown", DownTextureUnlit, 0, x1, offset, x2, offset, height, height, altitude, altitude, 1, 1, false);
				}
			}
			else
			{
				//horizontal lights
				if ((floor > bottomfloor && floor < topfloor) || ActiveDirection == true || in_elevator == true)
				{
					x1 = (-BackWidth / 2) + ((BackWidth / 7) * 4);
					x2 = (-BackWidth / 2) + ((BackWidth / 7) * 6);
					float x3 = (-BackWidth / 2) + (BackWidth / 7);
					float x4 = (-BackWidth / 2) + ((BackWidth / 7) * 3);
					float height = (BackHeight / 6) * 4;
					float altitude = BackHeight / 6;

					sbs->AddWallMain(this, DirectionalMeshUp, "DirectionalUp", UpTextureUnlit, 0, x1, offset, x2, offset, height, height, altitude, altitude, 1, 1, false);
					sbs->AddWallMain(this, DirectionalMeshDown, "DirectionalDown", DownTextureUnlit, 0, x3, offset, x4, offset, height, height, altitude, altitude, 1, 1, false);
				}
				else
				{
					float height = (BackHeight / 7) * 2;
					float altitude = (BackHeight / 7) * 2.5f;
					if (floor < topfloor)
						sbs->AddWallMain(this, DirectionalMeshUp, "DirectionalUp", UpTextureUnlit, 0, x1, offset, x2, offset, height, height, altitude, altitude, 1, 1, false);
					if (floor > bottomfloor)
						sbs->AddWallMain(this, DirectionalMeshDown, "DirectionalDown", DownTextureUnlit, 0, x1, offset, x2, offset, height, height, altitude, altitude, 1, 1, false);
				}
			}
		}
		else
		{
			float height = (BackHeight / 6) * 4;
			float altitude = BackHeight / 6;
			sbs->AddWallMain(this, DirectionalMesh, "Directional", UpTextureUnlit, 0, x1, offset, x2, offset, height, height, altitude, altitude, 1, 1, false);
		}
		sbs->ResetWalls();
	}
	else
	{
		float z1 = -BackWidth / 4;
		float z2 = BackWidth / 4;
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
					float altitude = (BackHeight / 7) * 4;
					float altitude2 = BackHeight / 7;
					sbs->AddWallMain(this, DirectionalMeshUp, "DirectionalUp", UpTextureUnlit, 0, offset, z1, offset, z2, height, height, altitude, altitude, 1, 1, false);
					sbs->AddWallMain(this, DirectionalMeshDown, "DirectionalDown", DownTextureUnlit, 0, offset, z1, offset, z2, height, height, altitude2, altitude2, 1, 1, false);
				}
				else
				{
					float height = (BackHeight / 7) * 2;
					float altitude = (BackHeight / 7) * 2.5f;
					if (floor < topfloor)
						sbs->AddWallMain(this, DirectionalMeshUp, "DirectionalUp", UpTextureUnlit, 0, offset, z1, offset, z2, height, height, altitude, altitude, 1, 1, false);
					if (floor > bottomfloor)
						sbs->AddWallMain(this, DirectionalMeshDown, "DirectionalDown", DownTextureUnlit, 0, offset, z1, offset, z2, height, height, altitude, altitude, 1, 1, false);
				}
			}
			else
			{
				//horizontal lights
				if ((floor > bottomfloor && floor < topfloor) || ActiveDirection == true || in_elevator == true)
				{
					z1 = (-BackWidth / 2) + ((BackWidth / 7) * 4);
					z2 = (-BackWidth / 2) + ((BackWidth / 7) * 6);
					float z3 = (-BackWidth / 2) + (BackWidth / 7);
					float z4 = (-BackWidth / 2) + ((BackWidth / 7) * 3);
					float height = (BackHeight / 6) * 4;
					float altitude = BackHeight / 6;
					sbs->AddWallMain(this, DirectionalMeshUp, "DirectionalUp", UpTextureUnlit, 0, offset, z1, offset, z2, height, height, altitude, altitude, 1, 1, false);
					sbs->AddWallMain(this, DirectionalMeshDown, "DirectionalDown", DownTextureUnlit, 0, offset, z3, offset, z4, height, height, altitude, altitude, 1, 1, false);
				}
				else
				{
					float height = (BackHeight / 7) * 2;
					float altitude = (BackHeight / 7) * 2.5f;
					if (floor < topfloor)
						sbs->AddWallMain(this, DirectionalMeshUp, "DirectionalUp", UpTextureUnlit, 0, offset, z1, offset, z2, height, height, altitude, altitude, 1, 1, false);
					if (floor > bottomfloor)
						sbs->AddWallMain(this, DirectionalMeshDown, "DirectionalDown", DownTextureUnlit, 0, offset, z1, offset, z2, height, height, altitude, altitude, 1, 1, false);
				}
			}
		}
		else
		{
			float height = (BackHeight / 6) * 4;
			float altitude = BackHeight / 6;
			sbs->AddWallMain(this, DirectionalMesh, "Directional", UpTextureUnlit, 0, offset, z1, offset, z2, height, height, altitude, altitude, 1, 1, false);
		}
		sbs->ResetWalls();
	}
	sbs->GetTextureManager()->ResetTextureMapping();
}

DirectionalIndicator::~DirectionalIndicator()
{
	if (timer)
	{
		timer->parent_deleting = true;
		delete timer;
	}
	timer = 0;

	if (DirectionalMesh)
	{
		DirectionalMesh->parent_deleting = true;
		delete DirectionalMesh;
	}
	DirectionalMesh = 0;

	if (DirectionalMeshUp)
	{
		DirectionalMeshUp->parent_deleting = true;
		delete DirectionalMeshUp;
	}
	DirectionalMeshUp = 0;

	if (DirectionalMeshDown)
	{
		DirectionalMeshDown->parent_deleting = true;
		delete DirectionalMeshDown;
	}
	DirectionalMeshDown = 0;

	if (DirectionalMeshBack)
	{
		DirectionalMeshBack->parent_deleting = true;
		delete DirectionalMeshBack;
	}
	DirectionalMeshBack = 0;

	//unregister from parent
	if (sbs->FastDelete == false && parent_deleting == false)
	{
		std::string type = GetParent()->GetType();

		if (type == "ElevatorCar")
			static_cast<ElevatorCar*>(GetParent())->RemoveDirectionalIndicator(this);
		else if (type == "Floor")
			static_cast<Floor*>(GetParent())->RemoveDirectionalIndicator(this);
	}
}

void DirectionalIndicator::Enabled(bool value)
{
	//turns panel on/off

	if (value == is_enabled)
		return;

	DirectionalMeshBack->Enable(value);
	if (DirectionalMeshUp)
		DirectionalMeshUp->Enable(value);
	if (DirectionalMeshDown)
		DirectionalMeshDown->Enable(value);
	if (DirectionalMesh)
		DirectionalMesh->Enable(value);

	is_enabled = value;
}

void DirectionalIndicator::UpLight(bool value)
{
	//turn on the 'up' directional light

	Elevator *elev = sbs->GetElevator(elevator);

	if (!elev)
		value = false;

	if (timer && elev)
	{
		if (!elev->GetCar(car))
			return;

		//stop or start timer
		if (value == false && DownStatus == false)
			timer->Stop();
		else if (value == true && (elev->AutoDoors == false || elev->GetCar(car)->ShaftDoorsExist(0, floor) == false))
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
}

void DirectionalIndicator::DownLight(bool value)
{
	//turn on the 'down' directional light

	Elevator *elev = sbs->GetElevator(elevator);

	if (!elev)
		value = false;

	if (timer && elev)
	{
		if (!elev->GetCar(car))
			return;

		//stop or start timer
		if (value == false && UpStatus == false)
			timer->Stop();
		else if (value == true && (elev->AutoDoors == false || elev->GetCar(car)->ShaftDoorsExist(0, floor) == false))
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
}

void DirectionalIndicator::SetLights(int up, int down)
{
	//set status of directional lights
	//values are 0 for no change, 1 for on, and 2 for off

	if (Single == false)
	{
		if (up == 1 && DirectionalMeshUp)
			DirectionalMeshUp->ChangeTexture(UpTextureLit);
		if (up == 2 && DirectionalMeshUp)
			DirectionalMeshUp->ChangeTexture(UpTextureUnlit);
		if (down == 1 && DirectionalMeshDown)
			DirectionalMeshDown->ChangeTexture(DownTextureLit);
		if (down == 2 && DirectionalMeshDown)
			DirectionalMeshDown->ChangeTexture(DownTextureUnlit);
	}
	else
	{
		if (DirectionalMesh)
		{
			if (up == 1)
				DirectionalMesh->ChangeTexture(UpTextureLit);
			if (down == 1)
				DirectionalMesh->ChangeTexture(DownTextureLit);
			if (up == 2 || down == 2)
				DirectionalMesh->ChangeTexture(UpTextureUnlit);
		}
	}

	//change status
	if (up == 1)
		UpStatus = true;
	if (up == 2)
		UpStatus = false;
	if (down == 1)
		DownStatus = true;
	if (down == 2)
		DownStatus = false;
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

}
