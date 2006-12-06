/* $Id$ */

/*
	Skyscraper 1.1 Alpha - Debug Panel
	Copyright (C)2005-2006 Ryan Thoryk
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

BEGIN_EVENT_TABLE(DebugPanel,wxDialog)
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
	Create(parent,id,_("Simulator Control Panel"),wxDefaultPosition,wxDefaultSize,wxCAPTION|wxTHICK_FRAME|wxSYSTEM_MENU|wxRESIZE_BOX|wxCLOSE_BOX|wxMINIMIZE_BOX,_T(""));
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

	wxTimer *timer;
    timer = new Timer();
    timer->Start(40);
}

void Timer::Notify()
{
	dp->t_camerap->SetLabel(wxT(wxString(_gcvt(sbs->camera->GetPosition().x, 6, buffer), wxConvUTF8) + ", " + wxString(_gcvt(sbs->camera->GetPosition().y, 6, buffer), wxConvUTF8) + ", " + wxString(_gcvt(sbs->camera->GetPosition().z, 6, buffer), wxConvUTF8)));
	dp->t_camerafloor->SetLabel(wxString(_itoa(sbs->camera->CurrentFloor, intbuffer, 10), wxConvUTF8));

	if (sbs->Elevators > 0)
	{
		dp->bEditElevator->Enable(true);
		dp->t_elevnumber->SetLabel(wxString(_itoa(sbs->ElevatorNumber, intbuffer, 10), wxConvUTF8));
		dp->t_elevfloor->SetLabel(wxString(_itoa(sbs->ElevatorArray[sbs->ElevatorNumber]->GetFloor(), intbuffer, 10), wxConvUTF8));
	}
	else
		dp->bEditElevator->Enable(false);

	if (ee->IsShown() == true)
	{
		Elevator *elevator = sbs->ElevatorArray.Get(ee->sNumber->GetThumbPosition() + 1);
		ee->tElevator->SetLabel(wxT("Number " + wxString(_itoa(ee->sNumber->GetThumbPosition() + 1, intbuffer, 10), wxConvUTF8)));
		ee->tFloor->SetLabel(wxT("Floor " + wxString(_itoa(ee->sFloor->GetThumbPosition(), intbuffer, 10), wxConvUTF8)));
		ee->txtAcceleration->SetValue(wxString(_gcvt(elevator->Acceleration, 6, buffer)));
		ee->txtBrakes->SetValue(wxString(BoolToString(elevator->GetBrakeStatus())));
		ee->txtDeceleration->SetValue(wxString(_gcvt(elevator->Deceleration, 6, buffer)));
		ee->txtDestFloor->SetValue(wxString(_itoa(elevator->GotoFloor, intbuffer, 10)));
		ee->txtDestination->SetValue(wxString(_gcvt(elevator->GetDestination(), 6, buffer)));
		ee->txtDirection->SetValue(wxString(_itoa(elevator->Direction, intbuffer, 10)));
		ee->txtDistance->SetValue(wxString(_gcvt(elevator->DistanceToTravel, 6, buffer)));
		ee->txtDoorAcceleration->SetValue(wxString(_gcvt(elevator->DoorAcceleration, 6, buffer)));
		ee->txtDoorDirection->SetValue(wxString(BoolToString(elevator->DoorDirection)));
		ee->txtDoorHeight->SetValue(wxString(_gcvt(elevator->DoorHeight, 6, buffer)));
		ee->txtDoorOrigin->SetValue(wxT(wxString(_gcvt(elevator->DoorOrigin.x, 6, buffer), wxConvUTF8) + ", " + wxString(_gcvt(elevator->DoorOrigin.y, 6, buffer), wxConvUTF8) + ", " + wxString(_gcvt(elevator->DoorOrigin.z, 6, buffer), wxConvUTF8)));
		ee->txtDoorsOpen->SetValue(wxString(BoolToString(elevator->AreDoorsOpen())));
		ee->txtDoorSpeed->SetValue(wxString(_gcvt(elevator->GetCurrentDoorSpeed(), 6, buffer)));
		ee->txtDoorWidth->SetValue(wxString(_gcvt(elevator->DoorWidth, 6, buffer)));
		ee->txtElevStart->SetValue(wxString(_gcvt(elevator->GetElevatorStart(), 6, buffer)));
		ee->txtEnabled->SetValue(wxString(BoolToString(elevator->IsEnabled)));
		ee->txtErrorOffset->SetValue(wxString(_gcvt(elevator->ErrorOffset, 6, buffer)));
		ee->txtFloor->SetValue(wxString(_itoa(elevator->GetFloor(), intbuffer, 10)));
		ee->txtHeight->SetValue(wxString(_gcvt(elevator->Height, 6, buffer)));
		ee->txtMoveElevator->SetValue(wxString(BoolToString(elevator->MoveElevator)));
		ee->txtMoveElevatorFloor->SetValue(wxString(_itoa(elevator->MoveElevatorFloor, intbuffer, 10)));
		ee->txtName->SetValue(wxString(elevator->Name.GetData()));
		ee->txtNumber->SetValue(wxString(_itoa(elevator->Number, intbuffer, 10)));
		ee->txtOpenSpeed->SetValue(wxString(_gcvt(elevator->OpenSpeed, 6, buffer)));
		ee->txtOrigin->SetValue(wxT(wxString(_gcvt(elevator->Origin.x, 6, buffer), wxConvUTF8) + ", " + wxString(_gcvt(elevator->Origin.y, 6, buffer), wxConvUTF8) + ", " + wxString(_gcvt(elevator->Origin.z, 6, buffer), wxConvUTF8)));
		ee->txtOriginFloor->SetValue(wxString(_itoa(elevator->OriginFloor, intbuffer, 10)));
		ee->txtPosition->SetValue(wxT(wxString(_gcvt(elevator->GetPosition().x, 6, buffer), wxConvUTF8) + ", " + wxString(_gcvt(elevator->GetPosition().y, 6, buffer), wxConvUTF8) + ", " + wxString(_gcvt(elevator->GetPosition().z, 6, buffer), wxConvUTF8)));
		ee->txtQueueDirection->SetValue(wxString(_itoa(elevator->QueuePositionDirection, intbuffer, 10)));
		ee->txtQueueLastDown->SetValue(wxString(_itoa(elevator->LastQueueFloor[0], intbuffer, 10)));
		ee->txtQueueLastUp->SetValue(wxString(_itoa(elevator->LastQueueFloor[1], intbuffer, 10)));
		ee->txtQueuePause->SetValue(wxString(BoolToString(elevator->PauseQueueSearch)));
		ee->txtRate->SetValue(wxString(_gcvt(elevator->ElevatorRate, 6, buffer)));
		ee->txtShaft->SetValue(wxString(_itoa(elevator->AssignedShaft, intbuffer, 10)));
		ee->txtShaftDoorOrigin->SetValue(wxT(wxString(_gcvt(elevator->ShaftDoorOrigin.x, 6, buffer), wxConvUTF8) + ", " + wxString(_gcvt(elevator->ShaftDoorOrigin.y, 6, buffer), wxConvUTF8) + ", " + wxString(_gcvt(elevator->ShaftDoorOrigin.z, 6, buffer), wxConvUTF8)));
		ee->txtSpeed->SetValue(wxString(_gcvt(elevator->ElevatorSpeed, 6, buffer)));
		ee->txtStop->SetValue(wxString(BoolToString(elevator->GetEmergencyStopStatus())));
		ee->txtStopDistance->SetValue(wxString(_gcvt(elevator->GetStoppingDistance(), 6, buffer)));
		ee->txtTempDecel->SetValue(wxString(_gcvt(elevator->TempDeceleration, 6, buffer)));
	}

	if (mc->IsShown() == true)
	{
		mc->chkFloor->SetValue(sbs->FloorArray[sbs->camera->CurrentFloor]->IsEnabled);
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
