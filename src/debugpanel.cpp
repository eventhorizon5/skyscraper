/*
	Skyscraper 1.1 Alpha - Debug Panel
	Copyright ©2005-2006 Ryan Thoryk
	http://www.tliquest.net/skyscraper
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@tliquest.net

	$Id$
	
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
extern Camera *c; //external pointer to the camera
DebugPanel *p; //self pointer
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
	EVT_BUTTON(ID_bStop,DebugPanel::On_bStop_Click)
    EVT_CHECKBOX(ID_chkFrameLimiter,DebugPanel::On_chkFrameLimiter_Change)
	EVT_CHECKBOX(ID_chkFrameSync,DebugPanel::On_chkFrameSync_Change)
//*)
END_EVENT_TABLE()

DebugPanel::DebugPanel(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(DebugPanel)
    Create(parent,id,_("Simulator Control Panel"),wxDefaultPosition,wxDefaultSize,wxCAPTION|wxTHICK_FRAME|wxSYSTEM_MENU|wxRESIZE_BOX|wxCLOSE_BOX|wxMINIMIZE_BOX);
    bListAltitudes = new wxButton(this,ID_bListAltitudes,_("List Altitudes"),wxPoint(350,185),wxSize(85,25),0);
    if (false) bListAltitudes->SetDefault();
    StaticText1 = new wxStaticText(this,st4c,_("Camera Floor:"),wxPoint(15,10),wxSize(74,13),wxST_NO_AUTORESIZE);
    StaticText2 = new wxStaticText(this,st5c,_("Camera Position:"),wxPoint(15,30),wxSize(89,13),wxST_NO_AUTORESIZE);
    StaticText3 = new wxStaticText(this,st6c,_("Elevator Number:"),wxPoint(15,50),wxSize(89,13),wxST_NO_AUTORESIZE);
    StaticText4 = new wxStaticText(this,st7c,_("Elevator Floor:"),wxPoint(15,70),wxSize(79,13),wxST_NO_AUTORESIZE);
    StaticText5 = new wxStaticText(this,st8c,_("Goto Floor:"),wxPoint(15,90),wxSize(64,13),wxST_NO_AUTORESIZE);
    StaticText6 = new wxStaticText(this,st9c,_("Distance To Dest:"),wxPoint(15,110),wxSize(89,13),wxST_NO_AUTORESIZE);
    StaticText7 = new wxStaticText(this,st10c,_("Rate:"),wxPoint(15,130),wxSize(34,13),wxST_NO_AUTORESIZE);
    StaticText8 = new wxStaticText(this,st11c,_("Selected Object:"),wxPoint(15,150),wxSize(89,13),wxST_NO_AUTORESIZE);
    t_camerafloor = new wxStaticText(this,ID_t_camerafloor,_T(""),wxPoint(105,10),wxSize(109,18),wxST_NO_AUTORESIZE|wxSUNKEN_BORDER);
    t_camerap = new wxStaticText(this,ID_t_camerap,_T(""),wxPoint(105,30),wxSize(109,18),wxST_NO_AUTORESIZE|wxSUNKEN_BORDER);
    t_elevnumber = new wxStaticText(this,ID_t_elevnumber,_T(""),wxPoint(105,50),wxSize(109,18),wxST_NO_AUTORESIZE|wxSUNKEN_BORDER);
    t_elevfloor = new wxStaticText(this,ID_t_elevfloor,_T(""),wxPoint(105,70),wxSize(109,18),wxST_NO_AUTORESIZE|wxSUNKEN_BORDER);
    t_gotofloor = new wxStaticText(this,ID_t_gotofloor,_T(""),wxPoint(105,90),wxSize(109,18),wxST_NO_AUTORESIZE|wxSUNKEN_BORDER);
    t_disttodest = new wxStaticText(this,ID_t_disttodest,_T(""),wxPoint(105,110),wxSize(109,18),wxST_NO_AUTORESIZE|wxSUNKEN_BORDER);
    t_rate = new wxStaticText(this,ID_t_rate,_T(""),wxPoint(105,130),wxSize(109,18),wxST_NO_AUTORESIZE|wxSUNKEN_BORDER);
    t_object = new wxStaticText(this,ID_t_object,_T(""),wxPoint(105,150),wxSize(109,18),wxST_NO_AUTORESIZE|wxSUNKEN_BORDER);
    StaticText17 = new wxStaticText(this,st22c,_("Simulator Options"),wxPoint(295,10),wxSize(119,13),wxALIGN_CENTRE|wxST_NO_AUTORESIZE);
    chkCollisionDetection = new wxCheckBox(this,ID_chkCollisionDetection,_("Collision Detection"),wxPoint(302,35),wxDefaultSize,0);
    chkCollisionDetection->SetValue(false);
    bMeshControl = new wxButton(this,ID_bMeshControl,_("Realtime Mesh Control"),wxPoint(325,215),wxSize(135,25),0);
    if (false) bMeshControl->SetDefault();
    bInitRealtime = new wxButton(this,ID_bInitRealtime,_("Init Realtime"),wxPoint(350,245),wxSize(85,25),0);
    if (false) bInitRealtime->SetDefault();
    t_number = new wxStaticText(this,ID_t_number,_("Number"),wxPoint(130,190),wxSize(49,13),wxALIGN_CENTRE|wxST_NO_AUTORESIZE);
    t_floor = new wxStaticText(this,ID_t_floor,_("Floor"),wxPoint(130,235),wxSize(49,13),wxALIGN_CENTRE|wxST_NO_AUTORESIZE);
    StaticText25 = new wxStaticText(this,st44c,_("On Floor"),wxPoint(35,290),wxSize(54,13),wxALIGN_CENTRE|wxST_NO_AUTORESIZE);
    elevFloor = new wxStaticText(this,ID_elevFloor,_T(""),wxPoint(35,305),wxSize(54,18),wxST_NO_AUTORESIZE|wxSUNKEN_BORDER);
    bEditElevator = new wxButton(this,ID_bEditElevator,_("Edit Elevator"),wxPoint(25,340),wxSize(75,25),0);
    if (false) bEditElevator->SetDefault();
    bCallElevator = new wxButton(this,ID_bCallElevator,_("Call Elevator"),wxPoint(115,290),wxSize(85,20),0);
    if (false) bCallElevator->SetDefault();
    bGo = new wxButton(this,ID_bGo,_("Go"),wxPoint(115,310),wxSize(85,20),0);
    if (false) bGo->SetDefault();
    bOpen = new wxButton(this,ID_bOpen,_("Open"),wxPoint(115,335),wxSize(85,20),0);
    if (false) bOpen->SetDefault();
    bOpenManual = new wxButton(this,ID_bOpenManual,_("Open Manual"),wxPoint(115,355),wxSize(85,20),0);
    if (false) bOpenManual->SetDefault();
    bEnqueueUp = new wxButton(this,ID_bEnqueueUp,_("Enqueue Up"),wxPoint(210,290),wxSize(80,20),0);
    if (false) bEnqueueUp->SetDefault();
    bEnqueueDown = new wxButton(this,ID_bEnqueueDown,_("Enqueue Down"),wxPoint(210,310),wxSize(80,20),0);
    if (false) bEnqueueDown->SetDefault();
    bClose = new wxButton(this,ID_bClose,_("Close"),wxPoint(210,335),wxSize(80,20),0);
    if (false) bClose->SetDefault();
    bCloseManual = new wxButton(this,ID_bCloseManual,_("Close Manual"),wxPoint(210,355),wxSize(80,20),0);
    if (false) bCloseManual->SetDefault();
    StaticText27 = new wxStaticText(this,st55c,_("Measurements are in feet"),wxPoint(230,138),wxSize(94,33),wxALIGN_CENTRE|wxST_NO_AUTORESIZE|wxSUNKEN_BORDER);
    s_ElevFloor = new wxScrollBar(this,ID_s_ElevFloor,wxPoint(27,255),wxSize(265,13),wxSB_HORIZONTAL);
    s_ElevFloor->SetScrollbar(0,1,10,1);
    s_ElevNum = new wxScrollBar(this,ID_s_ElevNum,wxPoint(27,210),wxSize(265,13),wxSB_HORIZONTAL);
    s_ElevNum->SetScrollbar(0,1,10,1);
    chkFrameSync = new wxCheckBox(this,ID_chkFrameSync,_("Framerate Sync"),wxPoint(302,93),wxDefaultSize,0);
    chkFrameSync->SetValue(false);
    chkAutoShafts = new wxCheckBox(this,ID_chkAutoShafts,_("Automatic Shafts"),wxPoint(302,79),wxDefaultSize,0);
    chkAutoShafts->SetValue(false);
    chkMainProcessing = new wxCheckBox(this,ID_chkMainProcessing,_("Main Sim Processing"),wxPoint(302,65),wxDefaultSize,0);
    chkMainProcessing->SetValue(false);
    chkFrameLimiter = new wxCheckBox(this,ID_chkFrameLimiter,_("Frame Limiter"),wxPoint(302,50),wxDefaultSize,0);
    chkFrameLimiter->SetValue(false);
    bStop = new wxButton(this,ID_bStop,_("Emergency Stop"),wxPoint(306,323),wxDefaultSize,0);
    if (false) bStop->SetDefault();
    //*)
	p = this;
	OnInit();
}

DebugPanel::~DebugPanel()
{
}


void DebugPanel::On_bGo_Click(wxCommandEvent& event)
{
    sbs->ElevatorArray[s_ElevNum->GetThumbPosition() + 1]->GotoFloor = s_ElevFloor->GetThumbPosition();
    sbs->ElevatorArray[s_ElevNum->GetThumbPosition() + 1]->ElevatorSync = true;
    sbs->ElevatorArray[s_ElevNum->GetThumbPosition() + 1]->MoveElevator = true;
}

void DebugPanel::On_bCallElevator_Click(wxCommandEvent& event)
{

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

}

void DebugPanel::On_bEnqueueDown_Click(wxCommandEvent& event)
{

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
		s_ElevNum->SetRange(sbs->Elevators);
		s_ElevNum->SetThumbPosition(0);

		//set floor range slider
		//s_ElevFloor->SetRange(-sbs->Basements, sbs->TotalFloors);
		s_ElevFloor->SetRange(sbs->TotalFloors);
		s_ElevFloor->SetThumbPosition(0);
	}
	else
		s_ElevNum->Enable(false);

    //set check boxes
    chkCollisionDetection->SetValue(sbs->EnableCollisions);
    chkFrameLimiter->SetValue(sbs->FrameLimiter);
    //chkMainProcessing->SetValue();
    //chkAutoShafts->SetValue();
    chkFrameSync->SetValue(sbs->FrameSync);

	mc = new MeshControl(p, -1);

	wxTimer *timer;
    timer = new Timer();
    timer->Start(40);
}

void Timer::Notify()
{
	p->t_camerap->SetLabel(wxT(wxString(_gcvt(c->GetPosition().x, 2, buffer)) + ", " + wxString(_gcvt(c->GetPosition().y, 2, buffer)) + ", " + wxString(_gcvt(c->GetPosition().z, 2, buffer))));
	p->t_camerafloor->SetLabel(wxString(_itoa(c->CurrentFloor, intbuffer, 10)));
	
	if (sbs->Elevators > 0)
	{
		p->t_elevnumber->SetLabel(wxString(_itoa(sbs->ElevatorNumber, intbuffer, 10)));
		p->t_elevfloor->SetLabel(wxString(_itoa(sbs->ElevatorArray[sbs->ElevatorNumber]->GetElevatorFloor(), intbuffer, 10)));
		p->t_gotofloor->SetLabel(wxString(_itoa(sbs->ElevatorArray[sbs->ElevatorNumber]->GotoFloor, intbuffer, 10)));
		p->t_disttodest->SetLabel(wxString(_gcvt(sbs->ElevatorArray[sbs->ElevatorNumber]->DistanceToTravel, 2, buffer)));
		p->t_rate->SetLabel(wxString(_gcvt(sbs->ElevatorArray[sbs->ElevatorNumber]->ElevatorRate, 2, buffer)));

		p->t_number->SetLabel(wxT("Number " + wxString(_itoa(p->s_ElevNum->GetThumbPosition() + 1, intbuffer, 10))));
		p->t_floor->SetLabel(wxT("Floor " + wxString(_itoa(p->s_ElevFloor->GetThumbPosition(), intbuffer, 10))));
	}

	if (mc)
	{
		mc->chkFloor->SetValue(sbs->FloorArray[c->CurrentFloor]->IsEnabled);
		mc->chkColumnFrame->SetValue(sbs->IsColumnFrameEnabled);
		mc->chkSky->SetValue(sbs->IsSkyboxEnabled);
		mc->chkLandscape->SetValue(sbs->IsLandscapeEnabled);
		mc->chkBuildings->SetValue(sbs->IsBuildingsEnabled);
		mc->chkExternal->SetValue(sbs->IsExternalEnabled);
	}
}
