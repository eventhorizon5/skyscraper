/*
	Scalable Building Simulator - Directional Indicator Object
	The Skyscraper Project - Version 2.1
	Copyright (C)2004-2025 Ryan Thoryk
	https://www.skyscrapersim.net
	https://sourceforge.net/projects/skyscraper/
	Contact - ryan@skyscrapersim.net

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
#include "polymesh.h"
#include "floor.h"
#include "mesh.h"
#include "texman.h"
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
	int type;
	bool blink_up, blink_down;
	int blink_count;

	Timer(const std::string &name, DirectionalIndicator *parent, int type = 0) : TimerObject(parent, name)
	{
		indicator = parent;
		this->type = type;
		blink_up = false;
		blink_down = false;
		blink_count = 0;
	}
	virtual void Notify();
};

DirectionalIndicator::DirectionalIndicator(Object *parent, int elevator, int car, int floor, bool active_direction, bool single, bool vertical, const std::string &BackTexture, const std::string &uptexture, const std::string &uptexture_lit, const std::string &downtexture, const std::string &downtexture_lit, Real CenterX, Real CenterZ, Real voffset, const std::string &direction, Real BackWidth, Real BackHeight, bool ShowBack, Real tw, Real th) : Object(parent)
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
	blink_timer = 0;
	blink = sbs->GetConfigInt("Skyscraper.SBS.DirectionalIndicator.Blink", 0);
	blink_active = false;
	timer_interval = sbs->GetConfigInt("Skyscraper.SBS.DirectionalIndicator.Timer", 15000);
	blink_interval = sbs->GetConfigInt("Skyscraper.SBS.DirectionalIndicator.BlinkTimer", 500);

	SetCase(Direction, false);
	TrimString(Direction);

	//validate objects
	if (!sbs->GetElevator(elevator))
		return;
	if (!sbs->GetElevator(elevator)->GetCar(car))
		return;
	if (!sbs->GetFloor(floor))
		return;

	bool in_elevator = false;
	if (parent->GetType() == "ElevatorCar")
		in_elevator = true;

	//create timer
	if (ActiveDirection == false)
	{
		timer = new Timer("Shut-off Timer", this, 0);
		blink_timer = new Timer("Blink Timer", this, 1);
	}

	//move object
	Move(CenterX, voffset, CenterZ);

	sbs->GetTextureManager()->EnableLighting(UpTextureLit, false);
	sbs->GetTextureManager()->EnableLighting(DownTextureLit, false);

	//create object mesh
	SetName("Directional Indicator " + ToString(elevator) + ":" + ToString(car) + ":" + ToString(floor));
	DirectionalMeshBack = new MeshObject(this, "Back", 0, "", "", sbs->GetConfigFloat("Skyscraper.SBS.MaxSmallRenderDistance", 100));

	if (Single == false)
	{
		DirectionalMeshUp = new MeshObject(this, "Up", 0, "", "", sbs->GetConfigFloat("Skyscraper.SBS.MaxSmallRenderDistance", 100));
		DirectionalMeshDown = new MeshObject(this, "Down", 0, "", "", sbs->GetConfigFloat("Skyscraper.SBS.MaxSmallRenderDistance", 100));
	}
	else
		DirectionalMesh = new MeshObject(this, "Arrow", 0, "", "", sbs->GetConfigFloat("Skyscraper.SBS.MaxSmallRenderDistance", 100));

	sbs->GetTextureManager()->ResetTextureMapping(true);

	PolyMesh *polymesh = sbs->GetPolyMesh();

	//create panel
	if (ShowBack == true)
	{
		Wall *wall = DirectionalMeshBack->CreateWallObject("Panel");
		if (Direction == "front" || Direction == "back")
		{
			if (Direction == "front")
				polymesh->DrawWalls(true, false, false, false, false, false);
			else
				polymesh->DrawWalls(false, true, false, false, false, false);
			polymesh->AddWallMain(wall, "Panel", BackTexture, 0, -BackWidth / 2, 0, BackWidth / 2, 0, BackHeight, BackHeight, 0, 0, tw, th, false);
			polymesh->ResetWalls();

		}
		else if (Direction == "left" || Direction == "right")
		{
			if (Direction == "left")
				polymesh->DrawWalls(true, false, false, false, false, false);
			else
				polymesh->DrawWalls(false, true, false, false, false, false);
			polymesh->AddWallMain(wall, "Panel", BackTexture, 0, 0, BackWidth / 2, 0, -BackWidth / 2, BackHeight, BackHeight, 0, 0, tw, th, false);
			polymesh->ResetWalls();
		}
	}

	//create indicator lanterns
	int bottomfloor = sbs->GetElevator(elevator)->GetCar(car)->GetBottomFloor();
	int topfloor = sbs->GetElevator(elevator)->GetCar(car)->GetTopFloor();

	if (Direction == "front" || Direction == "back")
	{
		Real x1 = -BackWidth / 4;
		Real x2 = BackWidth / 4;
		Real offset;
		if (Direction == "front")
		{
			offset = -0.01;
			polymesh->DrawWalls(true, false, false, false, false, false);
		}
		else
		{
			offset = 0.01;
			polymesh->DrawWalls(false, true, false, false, false, false);
		}
		if (Single == false)
		{
			if (Vertical == true)
			{
				if ((floor > bottomfloor && floor < topfloor) || ActiveDirection == true || in_elevator == true)
				{
					Real height = (BackHeight / 7) * 2;
					Real altitude = (BackHeight / 7) * 4;
					Real altitude2 = BackHeight / 7;

					Wall *wall = DirectionalMeshUp->CreateWallObject("DirectionalUp");
					polymesh->AddWallMain(wall, "DirectionalUp", UpTextureUnlit, 0, x1, offset, x2, offset, height, height, altitude, altitude, 1, 1, false);
					wall = DirectionalMeshDown->CreateWallObject("DirectionalDown");
					polymesh->AddWallMain(wall, "DirectionalDown", DownTextureUnlit, 0, x1, offset, x2, offset, height, height, altitude2, altitude2, 1, 1, false);
				}
				else
				{
					Real height = (BackHeight / 7) * 2;
					Real altitude = (BackHeight / 7) * 2.5;
					if (floor < topfloor)
					{
						Wall *wall = DirectionalMeshUp->CreateWallObject("DirectionalUp");
						polymesh->AddWallMain(wall, "DirectionalUp", UpTextureUnlit, 0, x1, offset, x2, offset, height, height, altitude, altitude, 1, 1, false);
					}
					if (floor > bottomfloor)
					{
						Wall *wall = DirectionalMeshDown->CreateWallObject("DirectionalDown");
						polymesh->AddWallMain(wall, "DirectionalDown", DownTextureUnlit, 0, x1, offset, x2, offset, height, height, altitude, altitude, 1, 1, false);
					}
				}
			}
			else
			{
				//horizontal lights
				if ((floor > bottomfloor && floor < topfloor) || ActiveDirection == true || in_elevator == true)
				{
					x1 = (-BackWidth / 2) + ((BackWidth / 7) * 4);
					x2 = (-BackWidth / 2) + ((BackWidth / 7) * 6);
					Real x3 = (-BackWidth / 2) + (BackWidth / 7);
					Real x4 = (-BackWidth / 2) + ((BackWidth / 7) * 3);
					Real height = (BackHeight / 6) * 4;
					Real altitude = BackHeight / 6;

					Wall *wall = DirectionalMeshUp->CreateWallObject("DirectionalUp");
					polymesh->AddWallMain(wall, "DirectionalUp", UpTextureUnlit, 0, x1, offset, x2, offset, height, height, altitude, altitude, 1, 1, false);
					wall = DirectionalMeshDown->CreateWallObject("DirectionalDown");
					polymesh->AddWallMain(wall, "DirectionalDown", DownTextureUnlit, 0, x3, offset, x4, offset, height, height, altitude, altitude, 1, 1, false);
				}
				else
				{
					Real height = (BackHeight / 7) * 2;
					Real altitude = (BackHeight / 7) * 2.5;
					if (floor < topfloor)
					{
						Wall *wall = DirectionalMeshUp->CreateWallObject("DirectionalUp");
						polymesh->AddWallMain(wall, "DirectionalUp", UpTextureUnlit, 0, x1, offset, x2, offset, height, height, altitude, altitude, 1, 1, false);
					}
					if (floor > bottomfloor)
					{
						Wall *wall = DirectionalMeshDown->CreateWallObject("DirectionalDown");
						polymesh->AddWallMain(wall, "DirectionalDown", DownTextureUnlit, 0, x1, offset, x2, offset, height, height, altitude, altitude, 1, 1, false);
					}
				}
			}
		}
		else
		{
			Real height = (BackHeight / 6) * 4;
			Real altitude = BackHeight / 6;
			Wall *wall = DirectionalMesh->CreateWallObject("Directional");
			polymesh->AddWallMain(wall, "Directional", UpTextureUnlit, 0, x1, offset, x2, offset, height, height, altitude, altitude, 1, 1, false);
		}
		polymesh->ResetWalls();
	}
	else
	{
		Real z1 = -BackWidth / 4;
		Real z2 = BackWidth / 4;
		Real offset;
		if (Direction == "left")
		{
			offset = -0.01;
			polymesh->DrawWalls(true, false, false, false, false, false);
		}
		else
		{
			//right
			offset = 0.01;
			polymesh->DrawWalls(false, true, false, false, false, false);
		}
		if (Single == false)
		{
			if (Vertical == true)
			{
				if ((floor > bottomfloor && floor < topfloor) || ActiveDirection == true || in_elevator == true)
				{
					Real height = (BackHeight / 7) * 2;
					Real altitude = (BackHeight / 7) * 4;
					Real altitude2 = BackHeight / 7;
					Wall *wall = DirectionalMeshUp->CreateWallObject("DirectionalUp");
					polymesh->AddWallMain(wall, "DirectionalUp", UpTextureUnlit, 0, offset, z1, offset, z2, height, height, altitude, altitude, 1, 1, false);
					wall = DirectionalMeshDown->CreateWallObject("DirectionalDown");
					polymesh->AddWallMain(wall, "DirectionalDown", DownTextureUnlit, 0, offset, z1, offset, z2, height, height, altitude2, altitude2, 1, 1, false);
				}
				else
				{
					Real height = (BackHeight / 7) * 2;
					Real altitude = (BackHeight / 7) * 2.5;
					if (floor < topfloor)
					{
						Wall *wall = DirectionalMeshUp->CreateWallObject("DirectionalUp");
						polymesh->AddWallMain(wall, "DirectionalUp", UpTextureUnlit, 0, offset, z1, offset, z2, height, height, altitude, altitude, 1, 1, false);

					}
					if (floor > bottomfloor)
					{
						Wall *wall = DirectionalMeshDown->CreateWallObject("DirectionalDown");
						polymesh->AddWallMain(wall, "DirectionalDown", DownTextureUnlit, 0, offset, z1, offset, z2, height, height, altitude, altitude, 1, 1, false);

					}
				}
			}
			else
			{
				//horizontal lights
				if ((floor > bottomfloor && floor < topfloor) || ActiveDirection == true || in_elevator == true)
				{
					z1 = (-BackWidth / 2) + ((BackWidth / 7) * 4);
					z2 = (-BackWidth / 2) + ((BackWidth / 7) * 6);
					Real z3 = (-BackWidth / 2) + (BackWidth / 7);
					Real z4 = (-BackWidth / 2) + ((BackWidth / 7) * 3);
					Real height = (BackHeight / 6) * 4;
					Real altitude = BackHeight / 6;
					Wall *wall = DirectionalMeshUp->CreateWallObject("DirectionalUp");
					polymesh->AddWallMain(wall, "DirectionalUp", UpTextureUnlit, 0, offset, z1, offset, z2, height, height, altitude, altitude, 1, 1, false);
					wall = DirectionalMeshDown->CreateWallObject("DirectionalDown");
					polymesh->AddWallMain(wall, "DirectionalDown", DownTextureUnlit, 0, offset, z3, offset, z4, height, height, altitude, altitude, 1, 1, false);
				}
				else
				{
					Real height = (BackHeight / 7) * 2;
					Real altitude = (BackHeight / 7) * 2.5;
					if (floor < topfloor)
					{
						Wall *wall = DirectionalMeshUp->CreateWallObject("DirectionalUp");
						polymesh->AddWallMain(wall, "DirectionalUp", UpTextureUnlit, 0, offset, z1, offset, z2, height, height, altitude, altitude, 1, 1, false);
					}
					if (floor > bottomfloor)
					{
						Wall *wall = DirectionalMeshDown->CreateWallObject("DirectionalDown");
						polymesh->AddWallMain(wall, "DirectionalDown", DownTextureUnlit, 0, offset, z1, offset, z2, height, height, altitude, altitude, 1, 1, false);
					}
				}
			}
		}
		else
		{
			Real height = (BackHeight / 6) * 4;
			Real altitude = BackHeight / 6;
			Wall *wall = DirectionalMesh->CreateWallObject("Directional");
			polymesh->AddWallMain(wall, "Directional", UpTextureUnlit, 0, offset, z1, offset, z2, height, height, altitude, altitude, 1, 1, false);
		}
		polymesh->ResetWalls();
	}
	sbs->GetTextureManager()->ResetTextureMapping();

	EnableLoop(true);
}

DirectionalIndicator::~DirectionalIndicator()
{
	if (timer)
	{
		timer->parent_deleting = true;
		delete timer;
	}
	timer = 0;

	if (blink_timer)
	{
		blink_timer->parent_deleting = true;
		delete blink_timer;
	}
	blink_timer = 0;

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

bool DirectionalIndicator::Enabled(bool value)
{
	//turns panel on/off

	if (value == is_enabled)
		return true;

	bool status = true, result;

	result = DirectionalMeshBack->Enabled(value);
	if (!result)
		status = false;

	if (DirectionalMeshUp)
	{
		bool result = DirectionalMeshUp->Enabled(value);
		if (!result)
			status = false;
	}
	if (DirectionalMeshDown)
	{
		bool result = DirectionalMeshDown->Enabled(value);
		if (!result)
			status = false;
	}
	if (DirectionalMesh)
	{
		bool result = DirectionalMesh->Enabled(value);
		if (!result)
			status = false;
	}

	is_enabled = value;

	return status;
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

	//turn off up status if in single mode, and switching directions
	if (Single == true && DownStatus == true)
		DownStatus = false;

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

	//turn off up status if in single mode, and switching directions
	if (Single == true && UpStatus == true)
		UpStatus = false;

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

	//blink indicator if specified
	if (blink > 0 && blink_active == false && blink_timer)
	{
		blink_timer->blink_up = UpStatus;
		blink_timer->blink_down = DownStatus;
		blink_timer->blink_count = blink;
		blink_timer->Start(blink_interval);
		blink_active = true;
	}
}

void DirectionalIndicator::Timer::Notify()
{
	//when shut-off timer triggers, switch off lights
	if (type == 0)
	{

		if (indicator->UpStatus == false && indicator->DownStatus == false)
			return;

		//turn off all lights
		indicator->UpLight(false);
		indicator->DownLight(false);
	}

	//for blink timer, blink lights up to blink_count
	if (type == 1)
	{
		if (blink_up == true)
		{
			//flip up indicator status
			indicator->UpLight(!indicator->UpStatus);
		}
		if (blink_down == true)
		{
			//flip down indicator status
			indicator->DownLight(!indicator->DownStatus);
		}
		blink_count--;

		if (blink_count == 0)
		{
			Stop();
			indicator->blink_active = false;
		}
	}
}

void DirectionalIndicator::Off()
{
	//turn off indicator

	SetLights(2, 2);
}

bool DirectionalIndicator::Loop()
{
	if (sbs->GetPower() == false)
		Off();

	return true;
}

}
