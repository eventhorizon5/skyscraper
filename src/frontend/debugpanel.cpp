/* $Id$ */

/*
	Skyscraper 1.8 Alpha - Debug Panel
	Copyright (C)2003-2012 Ryan Thoryk
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
#include <wx/string.h>
#include <wx/intl.h>
//*)
#include "debugpanel.h"
#include "meshcontrol.h"
#include "editelevator.h"
#include "keydialog.h"
#include "profilergui.h"
#include "stats.h"
#include "globals.h"
#include "sbs.h"
#include "camera.h"
#include "floor.h"
#include "elevator.h"
#include "unix.h"
#include "cameracontrol.h"
#include "console.h"
#include "objectinfo.h"

extern SBS *Simcore; //external pointer to the SBS engine
extern DebugPanel *dpanel;
DebugPanel *dp; //self pointer
MeshControl *mc;
editelevator *ee;
CameraControl *cc;
KeyDialog *kd;
Stats *stats;
Console *console;
ObjectInfo *objectinfo;
Profiler *profiler;

//(*IdInit(DebugPanel)
const long DebugPanel::ID_STATICTEXT1 = wxNewId();
const long DebugPanel::ID_STATICTEXT2 = wxNewId();
const long DebugPanel::ID_STATICTEXT8 = wxNewId();
const long DebugPanel::ID_STATICTEXT3 = wxNewId();
const long DebugPanel::ID_STATICTEXT4 = wxNewId();
const long DebugPanel::ID_STATICTEXT5 = wxNewId();
const long DebugPanel::ID_STATICTEXT10 = wxNewId();
const long DebugPanel::ID_STATICTEXT9 = wxNewId();
const long DebugPanel::ID_STATICTEXT6 = wxNewId();
const long DebugPanel::ID_t_camerafloor = wxNewId();
const long DebugPanel::ID_t_camerap = wxNewId();
const long DebugPanel::ID_t_rotation = wxNewId();
const long DebugPanel::ID_t_elevnumber = wxNewId();
const long DebugPanel::ID_t_elevfloor = wxNewId();
const long DebugPanel::ID_t_object = wxNewId();
const long DebugPanel::ID_t_clickposition = wxNewId();
const long DebugPanel::ID_t_collision = wxNewId();
const long DebugPanel::ID_STATICTEXT7 = wxNewId();
const long DebugPanel::ID_STATICTEXT11 = wxNewId();
const long DebugPanel::ID_chkCollisionDetection = wxNewId();
const long DebugPanel::ID_chkGravity = wxNewId();
const long DebugPanel::ID_chkFrameLimiter = wxNewId();
const long DebugPanel::ID_chkProcessElevators = wxNewId();
const long DebugPanel::ID_chkAutoShafts = wxNewId();
const long DebugPanel::ID_chkAutoStairs = wxNewId();
const long DebugPanel::ID_chkRandom = wxNewId();
const long DebugPanel::ID_CHECKBOX1 = wxNewId();
const long DebugPanel::ID_bListAltitudes = wxNewId();
const long DebugPanel::ID_bMeshControl = wxNewId();
const long DebugPanel::ID_bCameraControl = wxNewId();
const long DebugPanel::ID_bEditElevator = wxNewId();
const long DebugPanel::ID_bControlReference = wxNewId();
const long DebugPanel::ID_bStats = wxNewId();
const long DebugPanel::ID_bConsole = wxNewId();
const long DebugPanel::ID_bObjectInfo = wxNewId();
const long DebugPanel::ID_bProfiler = wxNewId();
const long DebugPanel::ID_PANEL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(DebugPanel,wxFrame)
	//(*EventTable(DebugPanel)
	//*)
END_EVENT_TABLE()

DebugPanel::DebugPanel(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(DebugPanel)
	wxBoxSizer* BoxSizer11;
	
	Create(parent, wxID_ANY, _("Simulator Control Panel"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxMINIMIZE_BOX, _T("wxID_ANY"));
	Move(wxPoint(10,10));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	Panel1 = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	BoxSizer11 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer6 = new wxBoxSizer(wxVERTICAL);
	StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT1, _("Camera Floor:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	BoxSizer6->Add(StaticText1, 0, wxBOTTOM|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText2 = new wxStaticText(Panel1, ID_STATICTEXT2, _("Camera Position:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	BoxSizer6->Add(StaticText2, 0, wxBOTTOM|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText7 = new wxStaticText(Panel1, ID_STATICTEXT8, _("Camera Rotation:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	BoxSizer6->Add(StaticText7, 1, wxBOTTOM|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText3 = new wxStaticText(Panel1, ID_STATICTEXT3, _("Elevator Number:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	BoxSizer6->Add(StaticText3, 0, wxBOTTOM|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText4 = new wxStaticText(Panel1, ID_STATICTEXT4, _("Elevator Floor:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	BoxSizer6->Add(StaticText4, 0, wxBOTTOM|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText5 = new wxStaticText(Panel1, ID_STATICTEXT5, _("Selected Object:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	BoxSizer6->Add(StaticText5, 0, wxBOTTOM|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText9 = new wxStaticText(Panel1, ID_STATICTEXT10, _("Clicked Position:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	BoxSizer6->Add(StaticText9, 1, wxBOTTOM|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText8 = new wxStaticText(Panel1, ID_STATICTEXT9, _("Last Collision:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	BoxSizer6->Add(StaticText8, 1, wxBOTTOM|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText6 = new wxStaticText(Panel1, ID_STATICTEXT6, _("Framerate:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	BoxSizer6->Add(StaticText6, 1, wxBOTTOM|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer4->Add(BoxSizer6, 0, wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizer7 = new wxBoxSizer(wxVERTICAL);
	t_camerafloor = new wxStaticText(Panel1, ID_t_camerafloor, wxEmptyString, wxDefaultPosition, wxSize(150,-1), wxST_NO_AUTORESIZE, _T("ID_t_camerafloor"));
	BoxSizer7->Add(t_camerafloor, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t_camerap = new wxStaticText(Panel1, ID_t_camerap, wxEmptyString, wxDefaultPosition, wxSize(150,-1), wxST_NO_AUTORESIZE, _T("ID_t_camerap"));
	BoxSizer7->Add(t_camerap, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t_rotation = new wxStaticText(Panel1, ID_t_rotation, wxEmptyString, wxDefaultPosition, wxSize(150,-1), wxST_NO_AUTORESIZE, _T("ID_t_rotation"));
	BoxSizer7->Add(t_rotation, 1, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t_elevnumber = new wxStaticText(Panel1, ID_t_elevnumber, wxEmptyString, wxDefaultPosition, wxSize(150,-1), wxST_NO_AUTORESIZE, _T("ID_t_elevnumber"));
	BoxSizer7->Add(t_elevnumber, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t_elevfloor = new wxStaticText(Panel1, ID_t_elevfloor, wxEmptyString, wxDefaultPosition, wxSize(150,-1), wxST_NO_AUTORESIZE, _T("ID_t_elevfloor"));
	BoxSizer7->Add(t_elevfloor, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t_object = new wxStaticText(Panel1, ID_t_object, wxEmptyString, wxDefaultPosition, wxSize(150,-1), wxST_NO_AUTORESIZE, _T("ID_t_object"));
	BoxSizer7->Add(t_object, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t_clickposition = new wxStaticText(Panel1, ID_t_clickposition, wxEmptyString, wxDefaultPosition, wxSize(150,-1), wxST_NO_AUTORESIZE, _T("ID_t_clickposition"));
	BoxSizer7->Add(t_clickposition, 1, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t_collision = new wxStaticText(Panel1, ID_t_collision, wxEmptyString, wxDefaultPosition, wxSize(150,-1), wxST_NO_AUTORESIZE, _T("ID_t_collision"));
	BoxSizer7->Add(t_collision, 1, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t_framerate = new wxStaticText(Panel1, ID_STATICTEXT7, wxEmptyString, wxDefaultPosition, wxSize(150,-1), wxST_NO_AUTORESIZE, _T("ID_STATICTEXT7"));
	BoxSizer7->Add(t_framerate, 1, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer4->Add(BoxSizer7, 0, wxALIGN_RIGHT|wxALIGN_TOP, 5);
	BoxSizer2->Add(BoxSizer4, 0, wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 10);
	BoxSizer5 = new wxBoxSizer(wxVERTICAL);
	StaticText11 = new wxStaticText(Panel1, ID_STATICTEXT11, _("Simulator Options"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	BoxSizer5->Add(StaticText11, 0, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	chkCollisionDetection = new wxCheckBox(Panel1, ID_chkCollisionDetection, _("Collision Detection"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkCollisionDetection"));
	chkCollisionDetection->SetValue(false);
	BoxSizer5->Add(chkCollisionDetection, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkGravity = new wxCheckBox(Panel1, ID_chkGravity, _("Gravity"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkGravity"));
	chkGravity->SetValue(false);
	BoxSizer5->Add(chkGravity, 1, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkFrameLimiter = new wxCheckBox(Panel1, ID_chkFrameLimiter, _("Frame Limiter"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkFrameLimiter"));
	chkFrameLimiter->SetValue(false);
	chkFrameLimiter->Disable();
	chkFrameLimiter->Hide();
	BoxSizer5->Add(chkFrameLimiter, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkProcessElevators = new wxCheckBox(Panel1, ID_chkProcessElevators, _("Process Elevators"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkProcessElevators"));
	chkProcessElevators->SetValue(false);
	BoxSizer5->Add(chkProcessElevators, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkAutoShafts = new wxCheckBox(Panel1, ID_chkAutoShafts, _("Automatic Shafts"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkAutoShafts"));
	chkAutoShafts->SetValue(false);
	BoxSizer5->Add(chkAutoShafts, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkAutoStairs = new wxCheckBox(Panel1, ID_chkAutoStairs, _("Automatic Stairs"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkAutoStairs"));
	chkAutoStairs->SetValue(false);
	BoxSizer5->Add(chkAutoStairs, 1, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkRandom = new wxCheckBox(Panel1, ID_chkRandom, _("Random Activity"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkRandom"));
	chkRandom->SetValue(false);
	BoxSizer5->Add(chkRandom, 1, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkVerbose = new wxCheckBox(Panel1, ID_CHECKBOX1, _("Verbose Mode"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	chkVerbose->SetValue(false);
	BoxSizer5->Add(chkVerbose, 1, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(BoxSizer5, 0, wxALL|wxALIGN_RIGHT|wxALIGN_TOP, 0);
	BoxSizer11->Add(BoxSizer2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 10);
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	bListAltitudes = new wxButton(Panel1, ID_bListAltitudes, _("List Altitudes"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bListAltitudes"));
	BoxSizer3->Add(bListAltitudes, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bMeshControl = new wxButton(Panel1, ID_bMeshControl, _("Realtime Object Control"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bMeshControl"));
	BoxSizer3->Add(bMeshControl, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bCameraControl = new wxButton(Panel1, ID_bCameraControl, _("Camera Control"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bCameraControl"));
	BoxSizer3->Add(bCameraControl, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bEditElevator = new wxButton(Panel1, ID_bEditElevator, _("Elevator Editor"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bEditElevator"));
	BoxSizer3->Add(bEditElevator, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bControlReference = new wxButton(Panel1, ID_bControlReference, _("Control Reference"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bControlReference"));
	BoxSizer3->Add(bControlReference, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bStats = new wxButton(Panel1, ID_bStats, _("Simulator Statistics"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bStats"));
	BoxSizer3->Add(bStats, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bConsole = new wxButton(Panel1, ID_bConsole, _("Script Console"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bConsole"));
	BoxSizer3->Add(bConsole, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bObjectInfo = new wxButton(Panel1, ID_bObjectInfo, _("Object Information"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bObjectInfo"));
	BoxSizer3->Add(bObjectInfo, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bProfiler = new wxButton(Panel1, ID_bProfiler, _("Profiler"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bProfiler"));
	BoxSizer3->Add(bProfiler, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer11->Add(BoxSizer3, 0, wxTOP|wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 10);
	Panel1->SetSizer(BoxSizer11);
	BoxSizer11->Fit(Panel1);
	BoxSizer11->SetSizeHints(Panel1);
	BoxSizer1->Add(Panel1, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	
	Connect(ID_chkCollisionDetection,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&DebugPanel::On_chkCollisionDetection_Click);
	Connect(ID_chkGravity,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&DebugPanel::On_chkGravity_Click);
	Connect(ID_chkFrameLimiter,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&DebugPanel::On_chkFrameLimiter_Click);
	Connect(ID_chkProcessElevators,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&DebugPanel::On_chkProcessElevators_Click);
	Connect(ID_chkAutoShafts,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&DebugPanel::On_chkAutoShafts_Click);
	Connect(ID_chkAutoStairs,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&DebugPanel::On_chkAutoStairs_Click);
	Connect(ID_chkRandom,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&DebugPanel::On_chkRandom_Click);
	Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&DebugPanel::On_chkVerbose_Click);
	Connect(ID_bListAltitudes,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DebugPanel::On_bListAltitudes_Click);
	Connect(ID_bMeshControl,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DebugPanel::On_bMeshControl_Click);
	Connect(ID_bCameraControl,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DebugPanel::On_bCameraControl_Click);
	Connect(ID_bEditElevator,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DebugPanel::On_bEditElevator_Click);
	Connect(ID_bControlReference,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DebugPanel::On_bControlReference_Click);
	Connect(ID_bStats,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DebugPanel::On_bStats_Click);
	Connect(ID_bConsole,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DebugPanel::On_bConsole_Click);
	Connect(ID_bObjectInfo,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DebugPanel::On_bObjectInfo_Click);
	Connect(ID_bProfiler,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DebugPanel::On_bProfiler_Click);
	//*)
	dp = this;
	OnInit();
}

DebugPanel::~DebugPanel()
{
	//delete timer;
	if (timer)
	{
		timer->Stop();
		delete timer;
	}
	timer = 0;
	if (mc)
		mc->Destroy();
	mc = 0;
	if (ee)
		ee->Destroy();
	ee = 0;
	if (cc)
		cc->Destroy();
	cc = 0;
	if (kd)
		kd->Destroy();
	kd = 0;
	if (stats)
		stats->Destroy();
	stats = 0;
	if (console)
		console->Destroy();
	console = 0;
	if (objectinfo)
		objectinfo->Destroy();
	objectinfo = 0;
	if (profiler)
		profiler->Destroy();
	profiler = 0;
	dpanel = 0; //clear external pointer
}

void DebugPanel::On_chkCollisionDetection_Click(wxCommandEvent& event)
{
	Simcore->camera->EnableCollisions(chkCollisionDetection->GetValue());
}

void DebugPanel::On_chkFrameLimiter_Click(wxCommandEvent& event)
{
	Simcore->FrameLimiter = chkFrameLimiter->GetValue();
}

void DebugPanel::On_chkProcessElevators_Click(wxCommandEvent& event)
{
	Simcore->ProcessElevators = chkProcessElevators->GetValue();
}

void DebugPanel::On_chkAutoShafts_Click(wxCommandEvent& event)
{
	Simcore->AutoShafts = chkAutoShafts->GetValue();
}

void DebugPanel::On_bListAltitudes_Click(wxCommandEvent& event)
{
	Simcore->ListAltitudes();
}

void DebugPanel::On_bMeshControl_Click(wxCommandEvent& event)
{
	if (mc)
	{
		mc->CenterOnScreen();
		mc->Show();
	}
}

void DebugPanel::On_bEditElevator_Click(wxCommandEvent& event)
{
	if (ee)
	{
		ee->CenterOnScreen();
		ee->Show();
	}
}

void DebugPanel::OnInit()
{
	//set check boxes
	chkCollisionDetection->SetValue(Simcore->camera->CollisionsEnabled());
	chkGravity->SetValue(Simcore->camera->GetGravityStatus());
	chkFrameLimiter->SetValue(Simcore->FrameLimiter);
	chkProcessElevators->SetValue(Simcore->ProcessElevators);
	chkAutoShafts->SetValue(Simcore->AutoShafts);
	chkAutoStairs->SetValue(Simcore->AutoStairs);
	chkVerbose->SetValue(Simcore->Verbose);
	if (Simcore->Elevators() > 0)
		chkRandom->SetValue(Simcore->GetElevator(1)->RandomActivity);

	mc = new MeshControl(dp, -1);
	ee = new editelevator(dp, -1);
	cc = new CameraControl(dp, -1);
	kd = new KeyDialog(dp, -1);
	stats = new Stats(dp, -1);
	console = new Console(dp, -1);
	objectinfo = new ObjectInfo(dp, -1);
	profiler = new Profiler(dp, -1);

	timer = new Timer();
	timer->Start(40);
}

void DebugPanel::Timer::Notify()
{
	if (dp)
	{
		dp->t_camerap->SetLabel(TruncateNumber(Simcore->camera->GetPosition().x, 2) + wxT(", ") + TruncateNumber(Simcore->camera->GetPosition().y, 2) + wxT(", ") + TruncateNumber(Simcore->camera->GetPosition().z, 2));
		dp->t_rotation->SetLabel(TruncateNumber(Simcore->camera->GetRotation().x, 2) + wxT(", ") + TruncateNumber(Simcore->camera->GetRotation().y, 2) + wxT(", ") + TruncateNumber(Simcore->camera->GetRotation().z, 2));
		dp->t_camerafloor->SetLabel(wxVariant((long)Simcore->camera->CurrentFloor).GetString() + wxT(" (") + wxString::FromAscii(Simcore->camera->CurrentFloorID.c_str()) + wxT(")"));
		dp->t_object->SetLabel(wxString::FromAscii(Simcore->camera->GetClickedMeshName()));
		dp->t_framerate->SetLabel(TruncateNumber(Simcore->FPS, 2));
		dp->t_collision->SetLabel(wxString::FromAscii(Simcore->camera->LastHitMesh.c_str()));
		dp->t_clickposition->SetLabel(TruncateNumber(Simcore->camera->HitPosition.x, 2) + wxT(", ") + TruncateNumber(Simcore->camera->HitPosition.y, 2) + wxT(", ") + TruncateNumber(Simcore->camera->HitPosition.z, 2));

		if (Simcore->Elevators() > 0)
		{
			dp->bEditElevator->Enable(true);
			dp->t_elevnumber->SetLabel(wxVariant((long)Simcore->ElevatorNumber).GetString());
			dp->t_elevfloor->SetLabel(wxVariant((long)Simcore->GetElevator(Simcore->ElevatorNumber)->GetFloor()).GetString());
		}
		else
			dp->bEditElevator->Enable(false);
	}

	if (ee)
	{
		if (ee->IsShown() == true)
			ee->Loop();
	}

	if (mc)
	{
		if (mc->IsShown() == true && Simcore->GetFloor(Simcore->camera->CurrentFloor))
		{
			mc->chkFloor->SetValue(Simcore->GetFloor(Simcore->camera->CurrentFloor)->IsEnabled);
			mc->chkColumnFrame->SetValue(Simcore->GetFloor(Simcore->camera->CurrentFloor)->IsColumnFrameEnabled);
			mc->chkSky->SetValue(Simcore->IsSkyboxEnabled);
			mc->chkLandscape->SetValue(Simcore->IsLandscapeEnabled);
			mc->chkBuildings->SetValue(Simcore->IsBuildingsEnabled);
			mc->chkExternal->SetValue(Simcore->IsExternalEnabled);
		}
	}

	if (cc)
	{
		if (cc->IsShown() == true)
			cc->Loop();
	}

	if (stats)
	{
		if (stats->IsShown() == true)
			stats->Loop();
	}

	if (objectinfo)
	{
		if (objectinfo->IsShown() == true)
			objectinfo->Loop();
	}
	if (profiler && Simcore)
	{
		if (profiler->IsShown() == true)
		{
			Simcore->enable_profiling = true;
			//Simcore->enable_advanced_profiling = true;
			profiler->Loop();
		}
		else
		{
			Simcore->enable_profiling = false;
			//Simcore->enable_advanced_profiling = false;
		}
	}
}

wxString TruncateNumber(double value, int decimals)
{
	//truncates the numeric value to the specified number of decimal places (does not round)
	wxString number = wxVariant(value).GetString();
	if ((int)value == value)
		return number; //exit if value is an integer
	if (decimals < 1)
		return number;
	return number.Truncate(number.find('.') + decimals + 1);
}

wxString TruncateNumber(float value, int decimals)
{
	//truncates the numeric value to the specified number of decimal places (does not round)
	wxString number = wxVariant(value).GetString();
	if ((int)value == value)
		return number; //exit if value is an integer
	if (decimals < 1)
		return number;
	return number.Truncate(number.find('.') + decimals + 1);
}

void DebugPanel::On_chkGravity_Click(wxCommandEvent& event)
{
	//enables or disables gravity
	Simcore->camera->EnableGravity(chkGravity->GetValue());
}

void DebugPanel::On_chkAutoStairs_Click(wxCommandEvent& event)
{
	Simcore->AutoStairs = chkAutoStairs->GetValue();
}

void DebugPanel::On_bCameraControl_Click(wxCommandEvent& event)
{
	if (cc)
	{
		cc->CenterOnScreen();
		cc->Show();
	}
}

void DebugPanel::On_bControlReference_Click(wxCommandEvent& event)
{
	if (kd)
	{
		kd->CenterOnScreen();
		kd->Show();
	}
}

void DebugPanel::On_bStats_Click(wxCommandEvent& event)
{
	if (stats)
	{
		stats->CenterOnScreen();
		stats->Show();
	}
}

void DebugPanel::On_bConsole_Click(wxCommandEvent& event)
{
	if (console)
	{
		console->CenterOnScreen();
		console->Show();
	}
}

void DebugPanel::On_chkVerbose_Click(wxCommandEvent& event)
{
	Simcore->Verbose = chkVerbose->GetValue();
}

void DebugPanel::On_bObjectInfo_Click(wxCommandEvent& event)
{
	if (objectinfo)
	{
		objectinfo->PopulateTree();
		objectinfo->CenterOnScreen();
		objectinfo->Show();
	}
}

void DebugPanel::On_chkRandom_Click(wxCommandEvent& event)
{
	if (Simcore->Elevators() > 0)
	{
		for (int i = 1; i <= Simcore->Elevators(); i++)
		{
			if (Simcore->GetElevator(i))
				Simcore->GetElevator(i)->RandomActivity = chkRandom->GetValue();
		}
	}
}

void DebugPanel::On_bProfiler_Click(wxCommandEvent& event)
{
	if (profiler)
	{
		profiler->CenterOnScreen();
		profiler->Show();
	}
}
