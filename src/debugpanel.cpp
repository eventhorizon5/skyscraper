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
#include <crystalspace.h>
#include "sbs.h"
#include "camera.h"
#include "floor.h"
#include "elevator.h"
#include "unix.h"
extern SBS *sbs; //external pointer to the SBS engine
DebugPanel *dp; //self pointer
MeshControl *mc;

BEGIN_EVENT_TABLE(DebugPanel,wxDialog)
//(*EventTable(DebugPanel)
EVT_BUTTON(ID_bListAltitudes,DebugPanel::On_bListAltitudes_Click)
EVT_BUTTON(ID_bMeshControl,DebugPanel::On_bMeshControl_Click)
EVT_BUTTON(ID_bInitRealtime,DebugPanel::On_bInitRealtime_Click)
EVT_BUTTON(ID_bCallElevator,DebugPanel::On_bCallElevator_Click)
EVT_BUTTON(ID_bGo,DebugPanel::On_bGo_Click)
EVT_BUTTON(ID_bOpen,DebugPanel::On_bOpen_Click)
EVT_BUTTON(ID_bOpenManual,DebugPanel::On_bOpenManual_Click)
EVT_BUTTON(ID_bEnqueueUp,DebugPanel::On_bEnqueueUp_Click)
EVT_BUTTON(ID_bEnqueueDown,DebugPanel::On_bEnqueueDown_Click)
EVT_BUTTON(ID_bClose,DebugPanel::On_bClose_Click)
EVT_BUTTON(ID_bCloseManual,DebugPanel::On_bCloseManual_Click)
EVT_CHECKBOX(ID_chkFrameSync,DebugPanel::On_chkFrameSync_Change)
EVT_CHECKBOX(ID_chkFrameLimiter,DebugPanel::On_chkFrameLimiter_Change)
EVT_CHECKBOX(ID_chkAutoShafts,DebugPanel::On_chkAutoShafts_Change)
EVT_BUTTON(ID_bStop,DebugPanel::On_bStop_Click)
//*)
END_EVENT_TABLE()

DebugPanel::DebugPanel(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(DebugPanel)
	Create(parent,id,_("Simulator Control Panel"),wxPoint(0,0),wxSize(480,420),wxCAPTION|wxTHICK_FRAME|wxSYSTEM_MENU|wxRESIZE_BOX|wxCLOSE_BOX|wxMINIMIZE_BOX,_T(""));
	bListAltitudes = new wxButton(this,ID_bListAltitudes,_("List Altitudes"),wxPoint(350,185),wxSize(85,25),0,wxDefaultValidator,_("ID_bListAltitudes"));
	if (false) bListAltitudes->SetDefault();
	StaticText1 = new wxStaticText(this,st4c,_("Camera Floor:"),wxPoint(15,10),wxSize(74,13),wxST_NO_AUTORESIZE,_("st4c"));
	StaticText2 = new wxStaticText(this,st5c,_("Camera Position:"),wxPoint(15,30),wxSize(89,13),wxST_NO_AUTORESIZE,_("st5c"));
	StaticText3 = new wxStaticText(this,st6c,_("Elevator Number:"),wxPoint(15,50),wxSize(89,13),wxST_NO_AUTORESIZE,_("st6c"));
	StaticText4 = new wxStaticText(this,st7c,_("Elevator Floor:"),wxPoint(15,70),wxSize(79,13),wxST_NO_AUTORESIZE,_("st7c"));
	StaticText5 = new wxStaticText(this,st8c,_("Goto Floor:"),wxPoint(15,90),wxSize(64,13),wxST_NO_AUTORESIZE,_("st8c"));
	StaticText6 = new wxStaticText(this,st9c,_("Distance To Dest:"),wxPoint(15,110),wxSize(89,13),wxST_NO_AUTORESIZE,_("st9c"));
	StaticText7 = new wxStaticText(this,st10c,_("Rate:"),wxPoint(15,130),wxSize(34,13),wxST_NO_AUTORESIZE,_("st10c"));
	StaticText8 = new wxStaticText(this,st11c,_("Selected Object:"),wxPoint(15,150),wxSize(89,13),wxST_NO_AUTORESIZE,_("st11c"));
	t_camerafloor = new wxStaticText(this,ID_t_camerafloor,_T(""),wxPoint(105,10),wxSize(109,18),wxST_NO_AUTORESIZE,_("ID_t_camerafloor"));
	t_camerap = new wxStaticText(this,ID_t_camerap,_T(""),wxPoint(105,30),wxSize(109,18),wxST_NO_AUTORESIZE,_("ID_t_camerap"));
	t_elevnumber = new wxStaticText(this,ID_t_elevnumber,_T(""),wxPoint(105,50),wxSize(109,18),wxST_NO_AUTORESIZE,_("ID_t_elevnumber"));
	t_elevfloor = new wxStaticText(this,ID_t_elevfloor,_T(""),wxPoint(105,70),wxSize(109,18),wxST_NO_AUTORESIZE,_("ID_t_elevfloor"));
	t_gotofloor = new wxStaticText(this,ID_t_gotofloor,_T(""),wxPoint(105,90),wxSize(109,18),wxST_NO_AUTORESIZE,_("ID_t_gotofloor"));
	t_disttodest = new wxStaticText(this,ID_t_disttodest,_T(""),wxPoint(105,110),wxSize(109,18),wxST_NO_AUTORESIZE,_("ID_t_disttodest"));
	t_rate = new wxStaticText(this,ID_t_rate,_T(""),wxPoint(105,130),wxSize(109,18),wxST_NO_AUTORESIZE,_("ID_t_rate"));
	t_object = new wxStaticText(this,ID_t_object,_T(""),wxPoint(105,150),wxSize(109,18),wxST_NO_AUTORESIZE,_("ID_t_object"));
	StaticText17 = new wxStaticText(this,st22c,_("Simulator Options"),wxPoint(295,10),wxSize(119,13),wxALIGN_CENTRE|wxST_NO_AUTORESIZE,_("st22c"));
	chkCollisionDetection = new wxCheckBox(this,ID_chkCollisionDetection,_("Collision Detection"),wxPoint(302,35),wxDefaultSize,0,wxDefaultValidator,_("ID_chkCollisionDetection"));
	chkCollisionDetection->SetValue(false);
	bMeshControl = new wxButton(this,ID_bMeshControl,_("Realtime Mesh Control"),wxPoint(325,215),wxSize(135,25),0,wxDefaultValidator,_("ID_bMeshControl"));
	if (false) bMeshControl->SetDefault();
	bInitRealtime = new wxButton(this,ID_bInitRealtime,_("Init Realtime"),wxPoint(350,245),wxSize(85,25),0,wxDefaultValidator,_("ID_bInitRealtime"));
	if (false) bInitRealtime->SetDefault();
	t_number = new wxStaticText(this,ID_t_number,_("Number"),wxPoint(130,190),wxSize(49,13),wxALIGN_CENTRE|wxST_NO_AUTORESIZE,_("ID_t_number"));
	t_floor = new wxStaticText(this,ID_t_floor,_("Floor"),wxPoint(130,235),wxSize(49,13),wxALIGN_CENTRE|wxST_NO_AUTORESIZE,_("ID_t_floor"));
	StaticText25 = new wxStaticText(this,st44c,_("On Floor"),wxPoint(35,290),wxSize(54,13),wxALIGN_CENTRE|wxST_NO_AUTORESIZE,_("st44c"));
	elevFloor = new wxStaticText(this,ID_elevFloor,_T(""),wxPoint(35,305),wxSize(54,18),wxST_NO_AUTORESIZE,_("ID_elevFloor"));
	bEditElevator = new wxButton(this,ID_bEditElevator,_("Edit Elevator"),wxPoint(25,340),wxSize(75,25),0,wxDefaultValidator,_("ID_bEditElevator"));
	if (false) bEditElevator->SetDefault();
	bCallElevator = new wxButton(this,ID_bCallElevator,_("Call Elevator"),wxPoint(115,290),wxSize(85,20),0,wxDefaultValidator,_("ID_bCallElevator"));
	if (false) bCallElevator->SetDefault();
	bGo = new wxButton(this,ID_bGo,_("Go"),wxPoint(115,310),wxSize(85,20),0,wxDefaultValidator,_("ID_bGo"));
	if (false) bGo->SetDefault();
	bOpen = new wxButton(this,ID_bOpen,_("Open"),wxPoint(115,335),wxSize(85,20),0,wxDefaultValidator,_("ID_bOpen"));
	if (false) bOpen->SetDefault();
	bOpenManual = new wxButton(this,ID_bOpenManual,_("Open Manual"),wxPoint(115,355),wxSize(85,20),0,wxDefaultValidator,_("ID_bOpenManual"));
	if (false) bOpenManual->SetDefault();
	bEnqueueUp = new wxButton(this,ID_bEnqueueUp,_("Enqueue Up"),wxPoint(210,290),wxSize(80,20),0,wxDefaultValidator,_("ID_bEnqueueUp"));
	if (false) bEnqueueUp->SetDefault();
	bEnqueueDown = new wxButton(this,ID_bEnqueueDown,_("Enqueue Down"),wxPoint(210,310),wxSize(80,20),0,wxDefaultValidator,_("ID_bEnqueueDown"));
	if (false) bEnqueueDown->SetDefault();
	bClose = new wxButton(this,ID_bClose,_("Close"),wxPoint(210,335),wxSize(80,20),0,wxDefaultValidator,_("ID_bClose"));
	if (false) bClose->SetDefault();
	bCloseManual = new wxButton(this,ID_bCloseManual,_("Close Manual"),wxPoint(210,355),wxSize(80,20),0,wxDefaultValidator,_("ID_bCloseManual"));
	if (false) bCloseManual->SetDefault();
	StaticText27 = new wxStaticText(this,st55c,_("Measurements are in feet"),wxPoint(230,138),wxSize(94,33),wxALIGN_CENTRE|wxST_NO_AUTORESIZE,_("st55c"));
	s_ElevFloor = new wxScrollBar(this,ID_s_ElevFloor,wxPoint(27,255),wxSize(265,13),wxSB_HORIZONTAL,wxDefaultValidator,_("ID_s_ElevFloor"));
	s_ElevFloor->SetScrollbar(0,1,10,1);
	s_ElevNum = new wxScrollBar(this,ID_s_ElevNum,wxPoint(27,210),wxSize(265,13),wxSB_HORIZONTAL,wxDefaultValidator,_("ID_s_ElevNum"));
	s_ElevNum->SetScrollbar(0,1,10,1);
	chkFrameSync = new wxCheckBox(this,ID_chkFrameSync,_("Framerate Sync"),wxPoint(302,93),wxDefaultSize,0,wxDefaultValidator,_("ID_chkFrameSync"));
	chkFrameSync->SetValue(false);
	chkAutoShafts = new wxCheckBox(this,ID_chkAutoShafts,_("Automatic Shafts"),wxPoint(302,79),wxDefaultSize,0,wxDefaultValidator,_("ID_chkAutoShafts"));
	chkAutoShafts->SetValue(false);
	chkMainProcessing = new wxCheckBox(this,ID_chkMainProcessing,_("Main Sim Processing"),wxPoint(302,65),wxDefaultSize,0,wxDefaultValidator,_("ID_chkMainProcessing"));
	chkMainProcessing->SetValue(false);
	chkFrameLimiter = new wxCheckBox(this,ID_chkFrameLimiter,_("Frame Limiter"),wxPoint(302,50),wxDefaultSize,0,wxDefaultValidator,_("ID_chkFrameLimiter"));
	chkFrameLimiter->SetValue(false);
	bStop = new wxButton(this,ID_bStop,_("Emergency Stop"),wxPoint(306,323),wxDefaultSize,0,wxDefaultValidator,_("ID_bStop"));
	if (false) bStop->SetDefault();
	//*)
	dp = this;
	OnInit();
}

DebugPanel::~DebugPanel()
{
}


void DebugPanel::On_bGo_Click(wxCommandEvent& event)
{
    sbs->ElevatorArray[s_ElevNum->GetThumbPosition() + 1]->GotoFloor = s_ElevFloor->GetThumbPosition();
    sbs->ElevatorArray[s_ElevNum->GetThumbPosition() + 1]->MoveElevator = true;
}

void DebugPanel::On_bCallElevator_Click(wxCommandEvent& event)
{
	//calls elevator to the current camera floor
	if (sbs->ElevatorArray[s_ElevNum->GetThumbPosition() + 1]->GetFloor() > sbs->camera->CurrentFloor)
		sbs->ElevatorArray[s_ElevNum->GetThumbPosition() + 1]->AddRoute(sbs->camera->CurrentFloor, -1);
	if (sbs->ElevatorArray[s_ElevNum->GetThumbPosition() + 1]->GetFloor() < sbs->camera->CurrentFloor)
		sbs->ElevatorArray[s_ElevNum->GetThumbPosition() + 1]->AddRoute(sbs->camera->CurrentFloor, 1);
}

void DebugPanel::On_bOpen_Click(wxCommandEvent& event)
{
	sbs->ElevatorArray[s_ElevNum->GetThumbPosition() + 1]->OpenDoors();
}

void DebugPanel::On_bOpenManual_Click(wxCommandEvent& event)
{
	sbs->ElevatorArray[s_ElevNum->GetThumbPosition() + 1]->OpenDoorsEmergency();
}

void DebugPanel::On_bEnqueueUp_Click(wxCommandEvent& event)
{
	sbs->ElevatorArray[s_ElevNum->GetThumbPosition() + 1]->AddRoute(s_ElevFloor->GetThumbPosition(), 1);
}

void DebugPanel::On_bEnqueueDown_Click(wxCommandEvent& event)
{
	sbs->ElevatorArray[s_ElevNum->GetThumbPosition() + 1]->AddRoute(s_ElevFloor->GetThumbPosition(), -1);
}

void DebugPanel::On_bClose_Click(wxCommandEvent& event)
{
	sbs->ElevatorArray[s_ElevNum->GetThumbPosition() + 1]->CloseDoors();
}

void DebugPanel::On_bCloseManual_Click(wxCommandEvent& event)
{
	sbs->ElevatorArray[s_ElevNum->GetThumbPosition() + 1]->CloseDoorsEmergency();
}

void DebugPanel::On_bListAltitudes_Click(wxCommandEvent& event)
{
	sbs->ListAltitudes();
}

void DebugPanel::On_bMeshControl_Click(wxCommandEvent& event)
{
	mc->Show(true);
}

void DebugPanel::On_bInitRealtime_Click(wxCommandEvent& event)
{

}

void DebugPanel::On_bStop_Click(wxCommandEvent& event)
{
	sbs->ElevatorArray[s_ElevNum->GetThumbPosition() + 1]->StopElevator();
}

void DebugPanel::On_chkFrameLimiter_Change(wxCommandEvent& event)
{
	sbs->FrameLimiter = chkFrameLimiter->GetValue();
}

void DebugPanel::On_chkFrameSync_Change(wxCommandEvent& event)
{
	sbs->FrameSync = chkFrameSync->GetValue();
}

void DebugPanel::OnInit()
{
	if (sbs->Elevators > 0)
	{
		//set elevator range slider
		//s_ElevNum->SetRange(1, sbs->Elevators);
		s_ElevNum->SetScrollbar(0, 1, sbs->Elevators, 1);

		//set floor range slider
		//s_ElevFloor->SetRange(-sbs->Basements, sbs->TotalFloors);
		s_ElevFloor->SetScrollbar(0, 1, sbs->TotalFloors + 1, 1);
	}
	else
		s_ElevNum->Enable(false);

    //set check boxes
    chkCollisionDetection->SetValue(sbs->EnableCollisions);
    chkFrameLimiter->SetValue(sbs->FrameLimiter);
    //chkMainProcessing->SetValue();
    chkAutoShafts->SetValue(sbs->AutoShafts);
    chkFrameSync->SetValue(sbs->FrameSync);

	mc = new MeshControl(dp, -1);

	wxTimer *timer;
    timer = new Timer();
    timer->Start(40);
}

void Timer::Notify()
{
	//this line doesn't work on unicode WX build
	#if defined(wxUSE_UNICODE) && wxUSE_UNICODE
		//insert working code here
	#else
	dp->t_camerap->SetLabel(wxT(wxString(_gcvt(sbs->camera->GetPosition().x, 6, buffer), wxConvUTF8) + ", " + wxString(_gcvt(sbs->camera->GetPosition().y, 6, buffer), wxConvUTF8) + ", " + wxString(_gcvt(sbs->camera->GetPosition().z, 6, buffer), wxConvUTF8)));
	#endif

	dp->t_camerafloor->SetLabel(wxString(_itoa(sbs->camera->CurrentFloor, intbuffer, 10), wxConvUTF8));

	if (sbs->Elevators > 0)
	{
		dp->t_elevnumber->SetLabel(wxString(_itoa(sbs->ElevatorNumber, intbuffer, 10), wxConvUTF8));
		dp->t_elevfloor->SetLabel(wxString(_itoa(sbs->ElevatorArray[sbs->ElevatorNumber]->GetFloor(), intbuffer, 10), wxConvUTF8));
		dp->t_gotofloor->SetLabel(wxString(_itoa(sbs->ElevatorArray[sbs->ElevatorNumber]->GotoFloor, intbuffer, 10), wxConvUTF8));
		dp->t_disttodest->SetLabel(wxString(_gcvt(sbs->ElevatorArray[sbs->ElevatorNumber]->DistanceToTravel, 6, buffer), wxConvUTF8));
		dp->t_rate->SetLabel(wxString(_gcvt(sbs->ElevatorArray[sbs->ElevatorNumber]->ElevatorRate, 6, buffer), wxConvUTF8));

		dp->t_number->SetLabel(wxT("Number " + wxString(_itoa(dp->s_ElevNum->GetThumbPosition() + 1, intbuffer, 10), wxConvUTF8)));
		dp->t_floor->SetLabel(wxT("Floor " + wxString(_itoa(dp->s_ElevFloor->GetThumbPosition(), intbuffer, 10), wxConvUTF8)));
	}

	if (mc)
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
