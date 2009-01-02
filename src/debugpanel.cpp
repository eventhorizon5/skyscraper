/* $Id$ */

/*
	Skyscraper 1.1 Alpha - Debug Panel
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

//(*InternalHeaders(DebugPanel)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include "debugpanel.h"
#include "meshcontrol.h"
#include "editelevator.h"
#include "globals.h"
#include "sbs.h"
#include "camera.h"
#include "floor.h"
#include "elevator.h"
#include "unix.h"

extern SBS *Simcore; //external pointer to the SBS engine
DebugPanel *dp; //self pointer
MeshControl *mc;
editelevator *ee;

//(*IdInit(DebugPanel)
const long DebugPanel::ID_STATICTEXT1 = wxNewId();
const long DebugPanel::ID_STATICTEXT2 = wxNewId();
const long DebugPanel::ID_STATICTEXT3 = wxNewId();
const long DebugPanel::ID_STATICTEXT4 = wxNewId();
const long DebugPanel::ID_STATICTEXT5 = wxNewId();
const long DebugPanel::ID_STATICTEXT6 = wxNewId();
const long DebugPanel::ID_t_camerafloor = wxNewId();
const long DebugPanel::ID_t_camerap = wxNewId();
const long DebugPanel::ID_t_elevnumber = wxNewId();
const long DebugPanel::ID_t_elevfloor = wxNewId();
const long DebugPanel::ID_t_object = wxNewId();
const long DebugPanel::ID_STATICTEXT7 = wxNewId();
const long DebugPanel::ID_STATICTEXT11 = wxNewId();
const long DebugPanel::ID_chkCollisionDetection = wxNewId();
const long DebugPanel::ID_chkGravity = wxNewId();
const long DebugPanel::ID_chkFrameLimiter = wxNewId();
const long DebugPanel::ID_chkMainProcessing = wxNewId();
const long DebugPanel::ID_chkAutoShafts = wxNewId();
const long DebugPanel::ID_chkFrameSync = wxNewId();
const long DebugPanel::ID_bListAltitudes = wxNewId();
const long DebugPanel::ID_bMeshControl = wxNewId();
const long DebugPanel::ID_bInitRealtime = wxNewId();
const long DebugPanel::ID_bEditElevator = wxNewId();
//*)

BEGIN_EVENT_TABLE(DebugPanel,wxFrame)
	//(*EventTable(DebugPanel)
	//*)
END_EVENT_TABLE()

DebugPanel::DebugPanel(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(DebugPanel)
	Create(parent, wxID_ANY, _("Simulator Control Panel"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxMINIMIZE_BOX, _T("wxID_ANY"));
	Move(wxPoint(10,10));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer6 = new wxBoxSizer(wxVERTICAL);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Camera Floor:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	BoxSizer6->Add(StaticText1, 0, wxBOTTOM|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Camera Position:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	BoxSizer6->Add(StaticText2, 0, wxBOTTOM|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Elevator Number:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	BoxSizer6->Add(StaticText3, 0, wxBOTTOM|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Elevator Floor:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	BoxSizer6->Add(StaticText4, 0, wxBOTTOM|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Selected Object:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	BoxSizer6->Add(StaticText5, 0, wxBOTTOM|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("Framerate:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	BoxSizer6->Add(StaticText6, 1, wxBOTTOM|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer4->Add(BoxSizer6, 0, wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizer7 = new wxBoxSizer(wxVERTICAL);
	t_camerafloor = new wxStaticText(this, ID_t_camerafloor, wxEmptyString, wxDefaultPosition, wxSize(150,-1), wxST_NO_AUTORESIZE, _T("ID_t_camerafloor"));
	BoxSizer7->Add(t_camerafloor, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t_camerap = new wxStaticText(this, ID_t_camerap, wxEmptyString, wxDefaultPosition, wxSize(150,-1), wxST_NO_AUTORESIZE, _T("ID_t_camerap"));
	BoxSizer7->Add(t_camerap, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t_elevnumber = new wxStaticText(this, ID_t_elevnumber, wxEmptyString, wxDefaultPosition, wxSize(150,-1), wxST_NO_AUTORESIZE, _T("ID_t_elevnumber"));
	BoxSizer7->Add(t_elevnumber, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t_elevfloor = new wxStaticText(this, ID_t_elevfloor, wxEmptyString, wxDefaultPosition, wxSize(150,-1), wxST_NO_AUTORESIZE, _T("ID_t_elevfloor"));
	BoxSizer7->Add(t_elevfloor, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t_object = new wxStaticText(this, ID_t_object, wxEmptyString, wxDefaultPosition, wxSize(150,-1), wxST_NO_AUTORESIZE, _T("ID_t_object"));
	BoxSizer7->Add(t_object, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t_framerate = new wxStaticText(this, ID_STATICTEXT7, wxEmptyString, wxDefaultPosition, wxSize(150,-1), wxST_NO_AUTORESIZE, _T("ID_STATICTEXT7"));
	BoxSizer7->Add(t_framerate, 1, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer4->Add(BoxSizer7, 0, wxALIGN_RIGHT|wxALIGN_TOP, 5);
	BoxSizer2->Add(BoxSizer4, 0, wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 10);
	BoxSizer5 = new wxBoxSizer(wxVERTICAL);
	StaticText11 = new wxStaticText(this, ID_STATICTEXT11, _("Simulator Options"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	BoxSizer5->Add(StaticText11, 0, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	chkCollisionDetection = new wxCheckBox(this, ID_chkCollisionDetection, _("Collision Detection"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkCollisionDetection"));
	chkCollisionDetection->SetValue(false);
	BoxSizer5->Add(chkCollisionDetection, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkGravity = new wxCheckBox(this, ID_chkGravity, _("Gravity"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkGravity"));
	chkGravity->SetValue(false);
	BoxSizer5->Add(chkGravity, 1, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkFrameLimiter = new wxCheckBox(this, ID_chkFrameLimiter, _("Frame Limiter"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkFrameLimiter"));
	chkFrameLimiter->SetValue(false);
	BoxSizer5->Add(chkFrameLimiter, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkMainProcessing = new wxCheckBox(this, ID_chkMainProcessing, _("Main Sim Processing"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkMainProcessing"));
	chkMainProcessing->SetValue(false);
	BoxSizer5->Add(chkMainProcessing, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkAutoShafts = new wxCheckBox(this, ID_chkAutoShafts, _("Automatic Shafts"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkAutoShafts"));
	chkAutoShafts->SetValue(false);
	BoxSizer5->Add(chkAutoShafts, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkFrameSync = new wxCheckBox(this, ID_chkFrameSync, _("Framerate Sync"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkFrameSync"));
	chkFrameSync->SetValue(false);
	BoxSizer5->Add(chkFrameSync, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(BoxSizer5, 0, wxALL|wxALIGN_RIGHT|wxALIGN_TOP, 0);
	BoxSizer1->Add(BoxSizer2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 10);
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	bListAltitudes = new wxButton(this, ID_bListAltitudes, _("List Altitudes"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bListAltitudes"));
	BoxSizer3->Add(bListAltitudes, 0, wxTOP|wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bMeshControl = new wxButton(this, ID_bMeshControl, _("Realtime Mesh Control"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bMeshControl"));
	BoxSizer3->Add(bMeshControl, 0, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bInitRealtime = new wxButton(this, ID_bInitRealtime, _("Init Realtime"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bInitRealtime"));
	BoxSizer3->Add(bInitRealtime, 0, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bEditElevator = new wxButton(this, ID_bEditElevator, _("Edit Elevator"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bEditElevator"));
	BoxSizer3->Add(bEditElevator, 0, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 10);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	
	Connect(ID_chkCollisionDetection,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&DebugPanel::On_chkCollisionDetection_Click);
	Connect(ID_chkGravity,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&DebugPanel::On_chkGravity_Click);
	Connect(ID_chkFrameLimiter,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&DebugPanel::On_chkFrameLimiter_Click);
	Connect(ID_chkMainProcessing,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&DebugPanel::On_chkMainProcessing_Click);
	Connect(ID_chkAutoShafts,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&DebugPanel::On_chkAutoShafts_Click);
	Connect(ID_chkFrameSync,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&DebugPanel::On_chkFrameSync_Click);
	Connect(ID_bListAltitudes,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DebugPanel::On_bListAltitudes_Click);
	Connect(ID_bMeshControl,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DebugPanel::On_bMeshControl_Click);
	Connect(ID_bInitRealtime,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DebugPanel::On_bInitRealtime_Click);
	Connect(ID_bEditElevator,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DebugPanel::On_bEditElevator_Click);
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

void DebugPanel::On_chkCollisionDetection_Click(wxCommandEvent& event)
{
	Simcore->camera->EnableCollisions = chkCollisionDetection->GetValue();
}

void DebugPanel::On_chkFrameLimiter_Click(wxCommandEvent& event)
{
	Simcore->FrameLimiter = chkFrameLimiter->GetValue();
}

void DebugPanel::On_chkMainProcessing_Click(wxCommandEvent& event)
{

}

void DebugPanel::On_chkAutoShafts_Click(wxCommandEvent& event)
{
	Simcore->AutoShafts = chkAutoShafts->GetValue();
}

void DebugPanel::On_chkFrameSync_Click(wxCommandEvent& event)
{

}

void DebugPanel::On_bListAltitudes_Click(wxCommandEvent& event)
{
	Simcore->ListAltitudes();
}

void DebugPanel::On_bMeshControl_Click(wxCommandEvent& event)
{
	mc->Show();
}

void DebugPanel::On_bInitRealtime_Click(wxCommandEvent& event)
{

}

void DebugPanel::On_bEditElevator_Click(wxCommandEvent& event)
{
	ee->Show();
}

void DebugPanel::OnInit()
{
	//set check boxes
	chkCollisionDetection->SetValue(Simcore->camera->EnableCollisions);
	chkGravity->SetValue(Simcore->camera->GetGravityStatus());
	chkFrameLimiter->SetValue(Simcore->FrameLimiter);
	//chkMainProcessing->SetValue();
	chkAutoShafts->SetValue(Simcore->AutoShafts);

	mc = new MeshControl(dp, -1);
	ee = new editelevator(dp, -1);

	timer = new Timer();
	timer->Start(40);
}

void DebugPanel::Timer::Notify()
{
	dp->t_camerap->SetLabel(TruncateNumber(Simcore->camera->GetPosition().x, 2) + wxT(", ") + TruncateNumber(Simcore->camera->GetPosition().y, 2) + wxT(", ") + TruncateNumber(Simcore->camera->GetPosition().z, 2));
	dp->t_camerafloor->SetLabel(wxVariant((long)Simcore->camera->CurrentFloor).GetString());
	dp->t_object->SetLabel(wxString::FromAscii(Simcore->camera->GetClickedMeshName()));
	dp->t_framerate->SetLabel(TruncateNumber(Simcore->FPS, 2));

	if (Simcore->Elevators() > 0)
	{
		dp->bEditElevator->Enable(true);
		dp->t_elevnumber->SetLabel(wxVariant((long)Simcore->ElevatorNumber).GetString());
		dp->t_elevfloor->SetLabel(wxVariant((long)Simcore->GetElevator(Simcore->ElevatorNumber)->GetFloor()).GetString());
	}
	else
		dp->bEditElevator->Enable(false);

	if (ee->IsShown() == true)
		ee->Loop();

	if (mc->IsShown() == true)
	{
		mc->chkFloor->SetValue(Simcore->GetFloor(Simcore->camera->CurrentFloor)->IsEnabled);
		mc->chkSky->SetValue(Simcore->IsSkyboxEnabled);
		mc->chkLandscape->SetValue(Simcore->IsLandscapeEnabled);
		mc->chkBuildings->SetValue(Simcore->IsBuildingsEnabled);
		mc->chkExternal->SetValue(Simcore->IsExternalEnabled);
	}
}

wxString TruncateNumber(double value, int decimals)
{
	//truncates the numeric value to the specified number of decimal places (does not round)
	wxString number = wxVariant(value).GetString();
	if (decimals < 1)
		return number;
	return number.Truncate(number.find('.') + decimals + 1);
}

wxString TruncateNumber(float value, int decimals)
{
	//truncates the numeric value to the specified number of decimal places (does not round)
	wxString number = wxVariant(value).GetString();
	if (decimals < 1)
		return number;
	return number.Truncate(number.find('.') + decimals + 1);
}

void DebugPanel::On_chkGravity_Click(wxCommandEvent& event)
{
	//enables or disables gravity
	Simcore->camera->EnableGravity(chkGravity->GetValue());
}
