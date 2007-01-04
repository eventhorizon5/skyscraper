/* $Id$ */

/*
	Skyscraper 1.1 Alpha - Debug Panel
	Copyright (C)2005-2007 Ryan Thoryk
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

#include "debugpanel.h"
#include "meshcontrol.h"
#include "editelevator.h"
#include <crystalspace.h>
#include "sbs.h"
#include "camera.h"
#include "floor.h"
#include "elevator.h"
#include "unix.h"
#include "globals.h"

extern SBS *sbs; //external pointer to the SBS engine
DebugPanel *dp; //self pointer
MeshControl *mc;
editelevator *ee;

BEGIN_EVENT_TABLE(DebugPanel,wxFrame)
//(*EventTable(DebugPanel)
EVT_BUTTON(ID_bListAltitudes,DebugPanel::On_bListAltitudes_Click)
EVT_CHECKBOX(ID_chkCollisionDetection,DebugPanel::On_chkCollisionDetection_Change)
EVT_BUTTON(ID_bMeshControl,DebugPanel::On_bMeshControl_Click)
EVT_BUTTON(ID_bInitRealtime,DebugPanel::On_bInitRealtime_Click)
EVT_BUTTON(ID_bEditElevator,DebugPanel::On_bEditElevator_Click)
EVT_CHECKBOX(ID_chkFrameSync,DebugPanel::On_chkFrameSync_Change)
EVT_CHECKBOX(ID_chkAutoShafts,DebugPanel::On_chkAutoShafts_Change)
EVT_CHECKBOX(ID_chkFrameLimiter,DebugPanel::On_chkFrameLimiter_Change)
//*)
END_EVENT_TABLE()

DebugPanel::DebugPanel(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(DebugPanel)
	Create(parent,id,_("Simulator Control Panel"),wxDefaultPosition,wxSize(477,329),wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxMINIMIZE_BOX,_T(""));
	bListAltitudes = new wxButton(this,ID_bListAltitudes,_("List Altitudes"),wxPoint(350,185),wxSize(85,25),0,wxDefaultValidator,_("ID_bListAltitudes"));
	if (false) bListAltitudes->SetDefault();
	StaticText1 = new wxStaticText(this,st4c,_("Camera Floor:"),wxPoint(15,10),wxSize(74,13),wxST_NO_AUTORESIZE,_("st4c"));
	StaticText2 = new wxStaticText(this,st5c,_("Camera Position:"),wxPoint(15,30),wxSize(89,13),wxST_NO_AUTORESIZE,_("st5c"));
	StaticText3 = new wxStaticText(this,st6c,_("Elevator Number:"),wxPoint(15,50),wxSize(89,13),wxST_NO_AUTORESIZE,_("st6c"));
	StaticText4 = new wxStaticText(this,st7c,_("Elevator Floor:"),wxPoint(15,70),wxSize(79,13),wxST_NO_AUTORESIZE,_("st7c"));
	StaticText8 = new wxStaticText(this,st11c,_("Selected Object:"),wxPoint(15,90),wxSize(89,13),wxST_NO_AUTORESIZE,_("st11c"));
	t_camerafloor = new wxStaticText(this,ID_t_camerafloor,_T(""),wxPoint(105,10),wxSize(109,18),wxST_NO_AUTORESIZE,_("ID_t_camerafloor"));
	t_camerap = new wxStaticText(this,ID_t_camerap,_T(""),wxPoint(105,30),wxSize(109,18),wxST_NO_AUTORESIZE,_("ID_t_camerap"));
	t_elevnumber = new wxStaticText(this,ID_t_elevnumber,_T(""),wxPoint(105,50),wxSize(109,18),wxST_NO_AUTORESIZE,_("ID_t_elevnumber"));
	t_elevfloor = new wxStaticText(this,ID_t_elevfloor,_T(""),wxPoint(105,70),wxSize(109,18),wxST_NO_AUTORESIZE,_("ID_t_elevfloor"));
	t_object = new wxStaticText(this,ID_t_object,_T(""),wxPoint(105,90),wxSize(109,18),wxST_NO_AUTORESIZE,_("ID_t_object"));
	StaticText17 = new wxStaticText(this,st22c,_("Simulator Options"),wxPoint(295,10),wxSize(119,13),wxALIGN_CENTRE|wxST_NO_AUTORESIZE,_("st22c"));
	chkCollisionDetection = new wxCheckBox(this,ID_chkCollisionDetection,_("Collision Detection"),wxPoint(302,35),wxDefaultSize,0,wxDefaultValidator,_("ID_chkCollisionDetection"));
	chkCollisionDetection->SetValue(false);
	bMeshControl = new wxButton(this,ID_bMeshControl,_("Realtime Mesh Control"),wxPoint(325,215),wxSize(135,25),0,wxDefaultValidator,_("ID_bMeshControl"));
	if (false) bMeshControl->SetDefault();
	bInitRealtime = new wxButton(this,ID_bInitRealtime,_("Init Realtime"),wxPoint(350,245),wxSize(85,25),0,wxDefaultValidator,_("ID_bInitRealtime"));
	if (false) bInitRealtime->SetDefault();
	bEditElevator = new wxButton(this,ID_bEditElevator,_("Edit Elevator"),wxPoint(355,275),wxSize(75,25),0,wxDefaultValidator,_("ID_bEditElevator"));
	if (false) bEditElevator->SetDefault();
	StaticText27 = new wxStaticText(this,st55c,_("Measurements are in feet"),wxPoint(230,138),wxSize(94,33),wxALIGN_CENTRE|wxST_NO_AUTORESIZE,_("st55c"));
	chkFrameSync = new wxCheckBox(this,ID_chkFrameSync,_("Framerate Sync"),wxPoint(302,93),wxDefaultSize,0,wxDefaultValidator,_("ID_chkFrameSync"));
	chkFrameSync->SetValue(false);
	chkAutoShafts = new wxCheckBox(this,ID_chkAutoShafts,_("Automatic Shafts"),wxPoint(302,79),wxDefaultSize,0,wxDefaultValidator,_("ID_chkAutoShafts"));
	chkAutoShafts->SetValue(false);
	chkMainProcessing = new wxCheckBox(this,ID_chkMainProcessing,_("Main Sim Processing"),wxPoint(302,65),wxDefaultSize,0,wxDefaultValidator,_("ID_chkMainProcessing"));
	chkMainProcessing->SetValue(false);
	chkFrameLimiter = new wxCheckBox(this,ID_chkFrameLimiter,_("Frame Limiter"),wxPoint(302,50),wxDefaultSize,0,wxDefaultValidator,_("ID_chkFrameLimiter"));
	chkFrameLimiter->SetValue(false);
	//*)
	dp = this;
	OnInit();
}

DebugPanel::~DebugPanel()
{
	delete timer;
	timer = 0;
	mc->Destroy();
	ee->Destroy();
}

void DebugPanel::On_bListAltitudes_Click(wxCommandEvent& event)
{
	sbs->ListAltitudes();
}

void DebugPanel::On_bMeshControl_Click(wxCommandEvent& event)
{
	mc->Show();
}

void DebugPanel::On_bInitRealtime_Click(wxCommandEvent& event)
{

}

void DebugPanel::On_chkFrameLimiter_Change(wxCommandEvent& event)
{
	sbs->FrameLimiter = chkFrameLimiter->GetValue();
}

void DebugPanel::On_chkFrameSync_Change(wxCommandEvent& event)
{
	sbs->FrameSync = chkFrameSync->GetValue();
}

void DebugPanel::On_chkCollisionDetection_Change(wxCommandEvent& event)
{
	sbs->EnableCollisions = chkCollisionDetection->GetValue();
}

void DebugPanel::OnInit()
{
	//set check boxes
	chkCollisionDetection->SetValue(sbs->EnableCollisions);
	chkFrameLimiter->SetValue(sbs->FrameLimiter);
	//chkMainProcessing->SetValue();
	chkAutoShafts->SetValue(sbs->AutoShafts);
	chkFrameSync->SetValue(sbs->FrameSync);

	mc = new MeshControl(dp, -1);
	ee = new editelevator(dp, -1);

	timer = new Timer();
	timer->Start(40);
}

void Timer::Notify()
{
	dp->t_camerap->SetLabel(wxVariant(RoundDouble(sbs->camera->GetPosition().x, 2)).GetString() + wxT(", ") + wxVariant(RoundDouble(sbs->camera->GetPosition().y, 2)).GetString() + wxT(", ") + wxVariant(RoundDouble(sbs->camera->GetPosition().z, 2)).GetString());
	dp->t_camerafloor->SetLabel(wxVariant((long)sbs->camera->CurrentFloor).GetString());
	dp->t_object->SetLabel(wxString::FromAscii(sbs->camera->GetClickedMeshName()));

	if (sbs->Elevators() > 0)
	{
		dp->bEditElevator->Enable(true);
		dp->t_elevnumber->SetLabel(wxVariant((long)sbs->ElevatorNumber).GetString());
		dp->t_elevfloor->SetLabel(wxVariant((long)sbs->GetElevator(sbs->ElevatorNumber)->GetFloor()).GetString());
	}
	else
		dp->bEditElevator->Enable(false);

	if (ee->IsShown() == true)
	{
		Elevator *elevator = sbs->GetElevator(ee->sNumber->GetThumbPosition() + 1);
		ee->tElevator->SetLabel(wxT("Number " + wxVariant((long)ee->sNumber->GetThumbPosition() + 1).GetString()));
		ee->tFloor->SetLabel(wxT("Floor " + wxVariant((long)ee->sFloor->GetThumbPosition()).GetString()));
		ee->txtAcceleration->SetValue(wxVariant(RoundDouble(elevator->Acceleration, 2)).GetString());
		ee->txtBrakes->SetValue(wxString::FromAscii(BoolToString(elevator->GetBrakeStatus())));
		ee->txtDeceleration->SetValue(wxVariant(RoundDouble(elevator->Deceleration, 2)).GetString());
		ee->txtDestFloor->SetValue(wxVariant((long)elevator->GotoFloor).GetString());
		ee->txtDestination->SetValue(wxVariant(RoundDouble(elevator->GetDestination(), 2)).GetString());
		ee->txtDirection->SetValue(wxVariant((long)elevator->Direction).GetString());
		ee->txtDistance->SetValue(wxVariant(RoundDouble(elevator->DistanceToTravel, 2)).GetString());
		ee->txtDoorAcceleration->SetValue(wxVariant(RoundDouble(elevator->DoorAcceleration, 2)).GetString());
		ee->txtDoorDirection->SetValue(wxString::FromAscii(BoolToString(elevator->DoorDirection)));
		ee->txtDoorHeight->SetValue(wxVariant(RoundDouble(elevator->DoorHeight, 2)).GetString());
		ee->txtDoorOrigin->SetValue(wxVariant(RoundDouble(elevator->DoorOrigin.x, 2)).GetString() + wxT(", ") + wxVariant(RoundDouble(elevator->DoorOrigin.y, 2)).GetString() + wxT(", ") + wxVariant(RoundDouble(elevator->DoorOrigin.z, 2)).GetString());
		ee->txtDoorsOpen->SetValue(wxString::FromAscii(BoolToString(elevator->AreDoorsOpen())));
		ee->txtDoorSpeed->SetValue(wxVariant(RoundDouble(elevator->GetCurrentDoorSpeed(), 2)).GetString());
		ee->txtDoorWidth->SetValue(wxVariant(RoundDouble(elevator->DoorWidth, 2)).GetString());
		ee->txtElevStart->SetValue(wxVariant(RoundDouble(elevator->GetElevatorStart(), 2)).GetString());
		ee->txtEnabled->SetValue(wxString::FromAscii(BoolToString(elevator->IsEnabled)));
		ee->txtErrorOffset->SetValue(wxVariant(RoundDouble(elevator->ErrorOffset, 2)).GetString());
		ee->txtFloor->SetValue(wxVariant((long)elevator->GetFloor()).GetString());
		ee->txtHeight->SetValue(wxVariant(RoundDouble(elevator->Height, 2)).GetString());
		ee->txtMoveElevator->SetValue(wxString::FromAscii(BoolToString(elevator->MoveElevator)));
		ee->txtMoveElevatorFloor->SetValue(wxVariant((long)elevator->MoveElevatorFloor).GetString());
		ee->txtName->SetValue(wxString::FromAscii(elevator->Name.GetData()));
		ee->txtNumber->SetValue(wxVariant((long)elevator->Number).GetString());
		ee->txtOpenSpeed->SetValue(wxVariant(RoundDouble(elevator->OpenSpeed, 2)).GetString());
		ee->txtOrigin->SetValue(wxVariant(RoundDouble(elevator->Origin.x, 2)).GetString() + wxT(", ") + wxVariant(RoundDouble(elevator->Origin.y, 2)).GetString() + wxT(", ") + wxVariant(RoundDouble(elevator->Origin.z, 2)).GetString());
		ee->txtOriginFloor->SetValue(wxVariant((long)elevator->OriginFloor).GetString());
		ee->txtPosition->SetValue(wxVariant(RoundDouble(elevator->GetPosition().x, 2)).GetString() + wxT(", ") + wxVariant(RoundDouble(elevator->GetPosition().y, 2)).GetString() + wxT(", ") + wxVariant(RoundDouble(elevator->GetPosition().z, 2)).GetString());
		ee->txtQueueDirection->SetValue(wxVariant((long)elevator->QueuePositionDirection).GetString());
		ee->txtQueueLastDown->SetValue(wxVariant((long)elevator->LastQueueFloor[0]).GetString());
		ee->txtQueueLastUp->SetValue(wxVariant((long)elevator->LastQueueFloor[1]).GetString());
		ee->txtQueuePause->SetValue(wxString::FromAscii(BoolToString(elevator->PauseQueueSearch)));
		ee->txtRate->SetValue(wxVariant(RoundDouble(elevator->ElevatorRate, 2)).GetString());
		ee->txtShaft->SetValue(wxVariant((long)elevator->AssignedShaft).GetString());
		ee->txtShaftDoorOrigin->SetValue(wxVariant(RoundDouble(elevator->ShaftDoorOrigin.x, 2)).GetString() + wxT(", ") + wxVariant(RoundDouble(elevator->ShaftDoorOrigin.y, 2)).GetString() + wxT(", ") + wxVariant(RoundDouble(elevator->ShaftDoorOrigin.z, 2)).GetString());
		ee->txtSpeed->SetValue(wxVariant(RoundDouble(elevator->ElevatorSpeed, 2)).GetString());
		ee->txtStop->SetValue(wxString::FromAscii(BoolToString(elevator->GetEmergencyStopStatus())));
		ee->txtStopDistance->SetValue(wxVariant(RoundDouble(elevator->GetStoppingDistance(), 2)).GetString());
		ee->txtTempDecel->SetValue(wxVariant(RoundDouble(elevator->TempDeceleration, 2)).GetString());
	}

	if (mc->IsShown() == true)
	{
		mc->chkFloor->SetValue(sbs->GetFloor(sbs->camera->CurrentFloor)->IsEnabled);
		mc->chkColumnFrame->SetValue(sbs->IsColumnFrameEnabled);
		mc->chkSky->SetValue(sbs->IsSkyboxEnabled);
		mc->chkLandscape->SetValue(sbs->IsLandscapeEnabled);
		mc->chkBuildings->SetValue(sbs->IsBuildingsEnabled);
		mc->chkExternal->SetValue(sbs->IsExternalEnabled);
	}
}

void DebugPanel::On_chkAutoShafts_Change(wxCommandEvent& event)
{
	sbs->AutoShafts = chkAutoShafts->GetValue();
}

void DebugPanel::On_bEditElevator_Click(wxCommandEvent& event)
{
	ee->Show();
}
