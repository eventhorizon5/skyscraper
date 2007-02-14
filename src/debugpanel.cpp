//(*InternalHeaders(DebugPanel)
#include <wx/bitmap.h>
#include <wx/font.h>
#include <wx/fontenum.h>
#include <wx/fontmap.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/settings.h>
//*)

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
#include "globals.h"
#include "sbs.h"
#include "camera.h"
#include "floor.h"
#include "elevator.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine
DebugPanel *dp; //self pointer
MeshControl *mc;
editelevator *ee;

//(*IdInit(DebugPanel)
const long DebugPanel::ID_bListAltitudes = wxNewId();
const long DebugPanel::st4c = wxNewId();
const long DebugPanel::st5c = wxNewId();
const long DebugPanel::st6c = wxNewId();
const long DebugPanel::st7c = wxNewId();
const long DebugPanel::st11c = wxNewId();
const long DebugPanel::ID_t_camerafloor = wxNewId();
const long DebugPanel::ID_t_camerap = wxNewId();
const long DebugPanel::ID_t_elevnumber = wxNewId();
const long DebugPanel::ID_t_elevfloor = wxNewId();
const long DebugPanel::ID_t_object = wxNewId();
const long DebugPanel::st22c = wxNewId();
const long DebugPanel::ID_chkCollisionDetection = wxNewId();
const long DebugPanel::ID_bMeshControl = wxNewId();
const long DebugPanel::ID_bInitRealtime = wxNewId();
const long DebugPanel::ID_bEditElevator = wxNewId();
const long DebugPanel::st55c = wxNewId();
const long DebugPanel::ID_chkFrameSync = wxNewId();
const long DebugPanel::ID_chkAutoShafts = wxNewId();
const long DebugPanel::ID_chkMainProcessing = wxNewId();
const long DebugPanel::ID_chkFrameLimiter = wxNewId();
//*)

BEGIN_EVENT_TABLE(DebugPanel,wxFrame)
//(*EventTable(DebugPanel)
//*)
END_EVENT_TABLE()

DebugPanel::DebugPanel(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(DebugPanel)
	Create(parent,id,_("Simulator Control Panel"),wxDefaultPosition,wxDefaultSize,wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxMINIMIZE_BOX,_T("id"));
	bListAltitudes = new wxButton(this,ID_bListAltitudes,_("List Altitudes"),wxPoint(350,185),wxSize(85,25),0,wxDefaultValidator,_T("ID_bListAltitudes"));
	StaticText1 = new wxStaticText(this,st4c,_("Camera Floor:"),wxPoint(15,10),wxSize(74,13),wxST_NO_AUTORESIZE,_T("st4c"));
	StaticText2 = new wxStaticText(this,st5c,_("Camera Position:"),wxPoint(15,30),wxSize(89,13),wxST_NO_AUTORESIZE,_T("st5c"));
	StaticText3 = new wxStaticText(this,st6c,_("Elevator Number:"),wxPoint(15,50),wxSize(89,13),wxST_NO_AUTORESIZE,_T("st6c"));
	StaticText4 = new wxStaticText(this,st7c,_("Elevator Floor:"),wxPoint(15,70),wxSize(79,13),wxST_NO_AUTORESIZE,_T("st7c"));
	StaticText8 = new wxStaticText(this,st11c,_("Selected Object:"),wxPoint(15,90),wxSize(89,13),wxST_NO_AUTORESIZE,_T("st11c"));
	t_camerafloor = new wxStaticText(this,ID_t_camerafloor,wxEmptyString,wxPoint(105,10),wxSize(109,18),wxST_NO_AUTORESIZE,_T("ID_t_camerafloor"));
	t_camerap = new wxStaticText(this,ID_t_camerap,wxEmptyString,wxPoint(105,30),wxSize(109,18),wxST_NO_AUTORESIZE,_T("ID_t_camerap"));
	t_elevnumber = new wxStaticText(this,ID_t_elevnumber,wxEmptyString,wxPoint(105,50),wxSize(109,18),wxST_NO_AUTORESIZE,_T("ID_t_elevnumber"));
	t_elevfloor = new wxStaticText(this,ID_t_elevfloor,wxEmptyString,wxPoint(105,70),wxSize(109,18),wxST_NO_AUTORESIZE,_T("ID_t_elevfloor"));
	t_object = new wxStaticText(this,ID_t_object,wxEmptyString,wxPoint(105,90),wxSize(109,18),wxST_NO_AUTORESIZE,_T("ID_t_object"));
	StaticText17 = new wxStaticText(this,st22c,_("Simulator Options"),wxPoint(295,10),wxSize(119,13),wxALIGN_CENTRE|wxST_NO_AUTORESIZE,_T("st22c"));
	chkCollisionDetection = new wxCheckBox(this,ID_chkCollisionDetection,_("Collision Detection"),wxPoint(302,35),wxDefaultSize,0,wxDefaultValidator,_T("ID_chkCollisionDetection"));
	chkCollisionDetection->SetValue(false);
	bMeshControl = new wxButton(this,ID_bMeshControl,_("Realtime Mesh Control"),wxPoint(325,215),wxSize(135,25),0,wxDefaultValidator,_T("ID_bMeshControl"));
	bInitRealtime = new wxButton(this,ID_bInitRealtime,_("Init Realtime"),wxPoint(350,245),wxSize(85,25),0,wxDefaultValidator,_T("ID_bInitRealtime"));
	bEditElevator = new wxButton(this,ID_bEditElevator,_("Edit Elevator"),wxPoint(355,275),wxSize(75,25),0,wxDefaultValidator,_T("ID_bEditElevator"));
	StaticText27 = new wxStaticText(this,st55c,_("Measurements are in feet"),wxPoint(230,138),wxSize(94,33),wxALIGN_CENTRE|wxST_NO_AUTORESIZE,_T("st55c"));
	chkFrameSync = new wxCheckBox(this,ID_chkFrameSync,_("Framerate Sync"),wxPoint(302,93),wxDefaultSize,0,wxDefaultValidator,_T("ID_chkFrameSync"));
	chkFrameSync->SetValue(false);
	chkAutoShafts = new wxCheckBox(this,ID_chkAutoShafts,_("Automatic Shafts"),wxPoint(302,79),wxDefaultSize,0,wxDefaultValidator,_T("ID_chkAutoShafts"));
	chkAutoShafts->SetValue(false);
	chkMainProcessing = new wxCheckBox(this,ID_chkMainProcessing,_("Main Sim Processing"),wxPoint(302,65),wxDefaultSize,0,wxDefaultValidator,_T("ID_chkMainProcessing"));
	chkMainProcessing->SetValue(false);
	chkFrameLimiter = new wxCheckBox(this,ID_chkFrameLimiter,_("Frame Limiter"),wxPoint(302,50),wxDefaultSize,0,wxDefaultValidator,_T("ID_chkFrameLimiter"));
	chkFrameLimiter->SetValue(false);
	Connect(ID_bListAltitudes,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DebugPanel::On_bListAltitudes_Click);
	Connect(ID_chkCollisionDetection,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&DebugPanel::On_chkCollisionDetection_Change);
	Connect(ID_bMeshControl,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DebugPanel::On_bMeshControl_Click);
	Connect(ID_bInitRealtime,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DebugPanel::On_bInitRealtime_Click);
	Connect(ID_bEditElevator,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DebugPanel::On_bEditElevator_Click);
	Connect(ID_chkFrameSync,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&DebugPanel::On_chkFrameSync_Change);
	Connect(ID_chkAutoShafts,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&DebugPanel::On_chkAutoShafts_Change);
	Connect(ID_chkFrameLimiter,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&DebugPanel::On_chkFrameLimiter_Change);
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
	dp->t_camerap->SetLabel(wxVariant(RoundFloat(sbs->camera->GetPosition().x, 2)).GetString() + wxT(", ") + wxVariant(RoundFloat(sbs->camera->GetPosition().y, 2)).GetString() + wxT(", ") + wxVariant(RoundFloat(sbs->camera->GetPosition().z, 2)).GetString());
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
		ee->Loop();

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
