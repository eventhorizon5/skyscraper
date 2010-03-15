/* $Id$ */

/*
	Skyscraper 1.7 Alpha - Camera Control Form
	Copyright (C)2003-2010 Ryan Thoryk
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

//(*InternalHeaders(CameraControl)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include "debugpanel.h"
#include "globals.h"
#include "sbs.h"
#include "camera.h"
#include "skyscraper.h"
#include "unix.h"
#include "cameracontrol.h"

extern SBS *Simcore; //external pointer to the SBS engine

//(*IdInit(CameraControl)
const long CameraControl::ID_STATICTEXT3 = wxNewId();
const long CameraControl::ID_txtDirectionFront = wxNewId();
const long CameraControl::ID_STATICTEXT15 = wxNewId();
const long CameraControl::ID_txtDirectionTop = wxNewId();
const long CameraControl::ID_STATICTEXT4 = wxNewId();
const long CameraControl::ID_txtCurrentFloor = wxNewId();
const long CameraControl::ID_STATICTEXT5 = wxNewId();
const long CameraControl::ID_txtLastMesh = wxNewId();
const long CameraControl::ID_STATICTEXT6 = wxNewId();
const long CameraControl::ID_txtDesiredVel = wxNewId();
const long CameraControl::ID_STATICTEXT7 = wxNewId();
const long CameraControl::ID_txtVelocity = wxNewId();
const long CameraControl::ID_STATICTEXT8 = wxNewId();
const long CameraControl::ID_txtDesiredAngle = wxNewId();
const long CameraControl::ID_STATICTEXT9 = wxNewId();
const long CameraControl::ID_txtAngle = wxNewId();
const long CameraControl::ID_STATICTEXT10 = wxNewId();
const long CameraControl::ID_txtSpeed = wxNewId();
const long CameraControl::ID_STATICTEXT11 = wxNewId();
const long CameraControl::ID_txtMouseDown = wxNewId();
const long CameraControl::ID_STATICTEXT12 = wxNewId();
const long CameraControl::ID_txtStartFloor = wxNewId();
const long CameraControl::ID_STATICTEXT13 = wxNewId();
const long CameraControl::ID_txtStartPosition = wxNewId();
const long CameraControl::ID_STATICTEXT29 = wxNewId();
const long CameraControl::ID_txtFOV = wxNewId();
const long CameraControl::ID_STATICTEXT14 = wxNewId();
const long CameraControl::ID_txtGravity = wxNewId();
const long CameraControl::ID_bGravity = wxNewId();
const long CameraControl::ID_lblGravityEnabled = wxNewId();
const long CameraControl::ID_txtGravityEnabled = wxNewId();
const long CameraControl::ID_bGravityEnabled = wxNewId();
const long CameraControl::ID_STATICTEXT17 = wxNewId();
const long CameraControl::ID_txtCollisions = wxNewId();
const long CameraControl::ID_bCollisions = wxNewId();
const long CameraControl::ID_STATICTEXT18 = wxNewId();
const long CameraControl::ID_txtReportCollisions = wxNewId();
const long CameraControl::ID_bReportCollisions = wxNewId();
const long CameraControl::ID_STATICTEXT19 = wxNewId();
const long CameraControl::ID_txtFreelook = wxNewId();
const long CameraControl::ID_bFreelook = wxNewId();
const long CameraControl::ID_STATICTEXT20 = wxNewId();
const long CameraControl::ID_txtFreelookSpeed = wxNewId();
const long CameraControl::ID_bFreelookSpeed = wxNewId();
const long CameraControl::ID_STATICTEXT1 = wxNewId();
const long CameraControl::ID_STATICTEXT2 = wxNewId();
const long CameraControl::ID_lblPosition = wxNewId();
const long CameraControl::ID_lblRotation = wxNewId();
const long CameraControl::ID_STATICLINE2 = wxNewId();
const long CameraControl::ID_rPosition = wxNewId();
const long CameraControl::ID_rRotation = wxNewId();
const long CameraControl::ID_STATICTEXT28 = wxNewId();
const long CameraControl::ID_txtMoveSpeed = wxNewId();
const long CameraControl::ID_bZPlus = wxNewId();
const long CameraControl::ID_bYPlus = wxNewId();
const long CameraControl::ID_bXNeg = wxNewId();
const long CameraControl::ID_chkHold = wxNewId();
const long CameraControl::ID_bXPlus = wxNewId();
const long CameraControl::ID_bZNeg = wxNewId();
const long CameraControl::ID_bYNeg = wxNewId();
const long CameraControl::ID_bStartPosition = wxNewId();
const long CameraControl::ID_bStartRotation = wxNewId();
const long CameraControl::ID_bStartDirection = wxNewId();
const long CameraControl::ID_STATICTEXT16 = wxNewId();
const long CameraControl::ID_STATICTEXT21 = wxNewId();
const long CameraControl::ID_txtPositionX = wxNewId();
const long CameraControl::ID_bPositionX = wxNewId();
const long CameraControl::ID_STATICTEXT22 = wxNewId();
const long CameraControl::ID_txtPositionY = wxNewId();
const long CameraControl::ID_bPositionY = wxNewId();
const long CameraControl::ID_STATICTEXT23 = wxNewId();
const long CameraControl::ID_txtPositionZ = wxNewId();
const long CameraControl::ID_bPositionZ = wxNewId();
const long CameraControl::ID_STATICLINE1 = wxNewId();
const long CameraControl::ID_STATICTEXT27 = wxNewId();
const long CameraControl::ID_STATICTEXT24 = wxNewId();
const long CameraControl::ID_txtRotationX = wxNewId();
const long CameraControl::ID_bRotationX = wxNewId();
const long CameraControl::ID_STATICTEXT25 = wxNewId();
const long CameraControl::ID_txtRotationY = wxNewId();
const long CameraControl::ID_bRotationY = wxNewId();
const long CameraControl::ID_STATICTEXT26 = wxNewId();
const long CameraControl::ID_txtRotationZ = wxNewId();
const long CameraControl::ID_bRotationZ = wxNewId();
const long CameraControl::ID_STATICLINE3 = wxNewId();
const long CameraControl::ID_STATICTEXT30 = wxNewId();
const long CameraControl::ID_txtSetFOV = wxNewId();
const long CameraControl::ID_bSetFOV = wxNewId();
const long CameraControl::ID_bResetFOV = wxNewId();
//*)

BEGIN_EVENT_TABLE(CameraControl,wxDialog)
//(*EventTable(CameraControl)
//*)
END_EVENT_TABLE()

CameraControl::CameraControl(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(CameraControl)
	wxStaticBoxSizer* StaticBoxSizer2;
	wxFlexGridSizer* FlexGridSizer4;
	wxStaticBoxSizer* StaticBoxSizer4;
	wxFlexGridSizer* FlexGridSizer10;
	wxFlexGridSizer* FlexGridSizer3;
	wxFlexGridSizer* FlexGridSizer5;
	wxFlexGridSizer* FlexGridSizer9;
	wxFlexGridSizer* FlexGridSizer2;
	wxFlexGridSizer* FlexGridSizer7;
	wxStaticBoxSizer* StaticBoxSizer3;
	wxGridSizer* GridSizer1;
	wxFlexGridSizer* FlexGridSizer8;
	wxFlexGridSizer* FlexGridSizer14;
	wxFlexGridSizer* FlexGridSizer13;
	wxFlexGridSizer* FlexGridSizer12;
	wxFlexGridSizer* FlexGridSizer6;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer11;
	
	Create(parent, wxID_ANY, _("Camera Control"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxMINIMIZE_BOX, _T("wxID_ANY"));
	FlexGridSizer1 = new wxFlexGridSizer(2, 2, 0, 0);
	FlexGridSizer8 = new wxFlexGridSizer(0, 1, 0, 0);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Information"));
	FlexGridSizer2 = new wxFlexGridSizer(10, 2, 0, 0);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Direction Front:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	FlexGridSizer2->Add(StaticText3, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	txtDirectionFront = new wxTextCtrl(this, ID_txtDirectionFront, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_txtDirectionFront"));
	FlexGridSizer2->Add(txtDirectionFront, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText15 = new wxStaticText(this, ID_STATICTEXT15, _("Direction Top:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT15"));
	FlexGridSizer2->Add(StaticText15, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	txtDirectionTop = new wxTextCtrl(this, ID_txtDirectionTop, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_txtDirectionTop"));
	FlexGridSizer2->Add(txtDirectionTop, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Current Floor:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	FlexGridSizer2->Add(StaticText4, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	txtCurrentFloor = new wxTextCtrl(this, ID_txtCurrentFloor, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_txtCurrentFloor"));
	FlexGridSizer2->Add(txtCurrentFloor, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Last Hit Mesh:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	FlexGridSizer2->Add(StaticText5, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	txtLastMesh = new wxTextCtrl(this, ID_txtLastMesh, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_txtLastMesh"));
	FlexGridSizer2->Add(txtLastMesh, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("Desired Velocity:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	FlexGridSizer2->Add(StaticText6, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	txtDesiredVel = new wxTextCtrl(this, ID_txtDesiredVel, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_txtDesiredVel"));
	FlexGridSizer2->Add(txtDesiredVel, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Velocity:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	FlexGridSizer2->Add(StaticText7, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	txtVelocity = new wxTextCtrl(this, ID_txtVelocity, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_txtVelocity"));
	FlexGridSizer2->Add(txtVelocity, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("Des Angle Vel:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	FlexGridSizer2->Add(StaticText8, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	txtDesiredAngle = new wxTextCtrl(this, ID_txtDesiredAngle, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_txtDesiredAngle"));
	FlexGridSizer2->Add(txtDesiredAngle, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _("Anglular Velocity:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	FlexGridSizer2->Add(StaticText9, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	txtAngle = new wxTextCtrl(this, ID_txtAngle, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_txtAngle"));
	FlexGridSizer2->Add(txtAngle, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText10 = new wxStaticText(this, ID_STATICTEXT10, _("Speed:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	FlexGridSizer2->Add(StaticText10, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	txtSpeed = new wxTextCtrl(this, ID_txtSpeed, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_txtSpeed"));
	FlexGridSizer2->Add(txtSpeed, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText11 = new wxStaticText(this, ID_STATICTEXT11, _("MouseDown:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	FlexGridSizer2->Add(StaticText11, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	txtMouseDown = new wxTextCtrl(this, ID_txtMouseDown, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_txtMouseDown"));
	FlexGridSizer2->Add(txtMouseDown, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText12 = new wxStaticText(this, ID_STATICTEXT12, _("Start Floor:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT12"));
	FlexGridSizer2->Add(StaticText12, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	txtStartFloor = new wxTextCtrl(this, ID_txtStartFloor, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_txtStartFloor"));
	FlexGridSizer2->Add(txtStartFloor, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText13 = new wxStaticText(this, ID_STATICTEXT13, _("Start Position:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT13"));
	FlexGridSizer2->Add(StaticText13, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	txtStartPosition = new wxTextCtrl(this, ID_txtStartPosition, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_txtStartPosition"));
	FlexGridSizer2->Add(txtStartPosition, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText29 = new wxStaticText(this, ID_STATICTEXT29, _("FOV Angle:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT29"));
	FlexGridSizer2->Add(StaticText29, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	txtFOV = new wxTextCtrl(this, ID_txtFOV, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_txtFOV"));
	FlexGridSizer2->Add(txtFOV, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer1->Add(FlexGridSizer2, 1, wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer8->Add(StaticBoxSizer1, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticBoxSizer3 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Parameters"));
	FlexGridSizer3 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText14 = new wxStaticText(this, ID_STATICTEXT14, _("Gravity:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT14"));
	FlexGridSizer3->Add(StaticText14, 1, wxRIGHT|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	txtGravity = new wxTextCtrl(this, ID_txtGravity, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_txtGravity"));
	FlexGridSizer3->Add(txtGravity, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bGravity = new wxButton(this, ID_bGravity, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bGravity"));
	FlexGridSizer3->Add(bGravity, 1, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	lblGravityEnabled = new wxStaticText(this, ID_lblGravityEnabled, _("Gravity On:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_lblGravityEnabled"));
	FlexGridSizer3->Add(lblGravityEnabled, 1, wxRIGHT|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	txtGravityEnabled = new wxTextCtrl(this, ID_txtGravityEnabled, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_txtGravityEnabled"));
	FlexGridSizer3->Add(txtGravityEnabled, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bGravityEnabled = new wxButton(this, ID_bGravityEnabled, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bGravityEnabled"));
	FlexGridSizer3->Add(bGravityEnabled, 1, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText17 = new wxStaticText(this, ID_STATICTEXT17, _("Collisions:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT17"));
	FlexGridSizer3->Add(StaticText17, 1, wxRIGHT|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	txtCollisions = new wxTextCtrl(this, ID_txtCollisions, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_txtCollisions"));
	FlexGridSizer3->Add(txtCollisions, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bCollisions = new wxButton(this, ID_bCollisions, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bCollisions"));
	FlexGridSizer3->Add(bCollisions, 1, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText18 = new wxStaticText(this, ID_STATICTEXT18, _("Report Coll:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT18"));
	FlexGridSizer3->Add(StaticText18, 1, wxRIGHT|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	txtReportCollisions = new wxTextCtrl(this, ID_txtReportCollisions, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_txtReportCollisions"));
	FlexGridSizer3->Add(txtReportCollisions, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bReportCollisions = new wxButton(this, ID_bReportCollisions, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bReportCollisions"));
	FlexGridSizer3->Add(bReportCollisions, 1, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText19 = new wxStaticText(this, ID_STATICTEXT19, _("Freelook:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT19"));
	FlexGridSizer3->Add(StaticText19, 1, wxRIGHT|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	txtFreelook = new wxTextCtrl(this, ID_txtFreelook, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_txtFreelook"));
	FlexGridSizer3->Add(txtFreelook, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bFreelook = new wxButton(this, ID_bFreelook, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bFreelook"));
	FlexGridSizer3->Add(bFreelook, 1, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText21 = new wxStaticText(this, ID_STATICTEXT20, _("Freelook Spd:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT20"));
	FlexGridSizer3->Add(StaticText21, 1, wxRIGHT|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	txtFreelookSpeed = new wxTextCtrl(this, ID_txtFreelookSpeed, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_txtFreelookSpeed"));
	FlexGridSizer3->Add(txtFreelookSpeed, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bFreelookSpeed = new wxButton(this, ID_bFreelookSpeed, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bFreelookSpeed"));
	FlexGridSizer3->Add(bFreelookSpeed, 1, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer3->Add(FlexGridSizer3, 1, wxTOP|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer8->Add(StaticBoxSizer3, 1, wxBOTTOM|wxLEFT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer1->Add(FlexGridSizer8, 1, wxTOP|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer9 = new wxFlexGridSizer(0, 1, 0, 0);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Control"));
	FlexGridSizer5 = new wxFlexGridSizer(0, 1, 0, 0);
	GridSizer1 = new wxGridSizer(0, 2, 0, 0);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Position:"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_STATICTEXT1"));
	GridSizer1->Add(StaticText1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Rotation:"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_STATICTEXT2"));
	GridSizer1->Add(StaticText2, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	lblPosition = new wxStaticText(this, ID_lblPosition, _("0, 0, 0"), wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE|wxALIGN_CENTRE, _T("ID_lblPosition"));
	GridSizer1->Add(lblPosition, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	lblRotation = new wxStaticText(this, ID_lblRotation, _("0, 0, 0"), wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE|wxALIGN_CENTRE, _T("ID_lblRotation"));
	GridSizer1->Add(lblRotation, 1, wxALL|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer5->Add(GridSizer1, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticLine2 = new wxStaticLine(this, ID_STATICLINE2, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE2"));
	FlexGridSizer5->Add(StaticLine2, 1, wxBOTTOM|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer11 = new wxFlexGridSizer(0, 2, 0, 0);
	rPosition = new wxRadioButton(this, ID_rPosition, _("Position"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_rPosition"));
	rPosition->SetValue(true);
	FlexGridSizer11->Add(rPosition, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	rRotation = new wxRadioButton(this, ID_rRotation, _("Rotation"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_rRotation"));
	FlexGridSizer11->Add(rRotation, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer5->Add(FlexGridSizer11, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer14 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticText20 = new wxStaticText(this, ID_STATICTEXT28, _("Speed"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT28"));
	FlexGridSizer14->Add(StaticText20, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtMoveSpeed = new wxTextCtrl(this, ID_txtMoveSpeed, _("1.0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_txtMoveSpeed"));
	FlexGridSizer14->Add(txtMoveSpeed, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer5->Add(FlexGridSizer14, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer12 = new wxFlexGridSizer(0, 3, 0, 0);
	FlexGridSizer12->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bZPlus = new wxButton(this, ID_bZPlus, _("Z+"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bZPlus"));
	FlexGridSizer12->Add(bZPlus, 1, wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL, 5);
	bYPlus = new wxButton(this, ID_bYPlus, _("Y+"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bYPlus"));
	FlexGridSizer12->Add(bYPlus, 1, wxBOTTOM|wxLEFT|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	bXNeg = new wxButton(this, ID_bXNeg, _("X-"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bXNeg"));
	FlexGridSizer12->Add(bXNeg, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	chkHold = new wxCheckBox(this, ID_chkHold, _("Hold"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkHold"));
	chkHold->SetValue(false);
	FlexGridSizer12->Add(chkHold, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bXPlus = new wxButton(this, ID_bXPlus, _("X+"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bXPlus"));
	FlexGridSizer12->Add(bXPlus, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer12->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bZNeg = new wxButton(this, ID_bZNeg, _("Z-"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bZNeg"));
	FlexGridSizer12->Add(bZNeg, 1, wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	bYNeg = new wxButton(this, ID_bYNeg, _("Y-"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bYNeg"));
	FlexGridSizer12->Add(bYNeg, 1, wxTOP|wxLEFT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer5->Add(FlexGridSizer12, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer13 = new wxFlexGridSizer(0, 3, 0, 0);
	bStartPosition = new wxButton(this, ID_bStartPosition, _("StartPosition"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bStartPosition"));
	FlexGridSizer13->Add(bStartPosition, 1, wxTOP|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bStartRotation = new wxButton(this, ID_bStartRotation, _("StartRotation"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bStartRotation"));
	FlexGridSizer13->Add(bStartRotation, 1, wxTOP|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bStartDirection = new wxButton(this, ID_bStartDirection, _("StartDirection"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bStartDirection"));
	FlexGridSizer13->Add(bStartDirection, 1, wxTOP|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer5->Add(FlexGridSizer13, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer2->Add(FlexGridSizer5, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer9->Add(StaticBoxSizer2, 1, wxTOP|wxRIGHT|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	StaticBoxSizer4 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Set Camera"));
	FlexGridSizer6 = new wxFlexGridSizer(0, 1, 0, 0);
	StaticText16 = new wxStaticText(this, ID_STATICTEXT16, _("Position"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT16"));
	FlexGridSizer6->Add(StaticText16, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer4 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText22 = new wxStaticText(this, ID_STATICTEXT21, _("X:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT21"));
	FlexGridSizer4->Add(StaticText22, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtPositionX = new wxTextCtrl(this, ID_txtPositionX, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_txtPositionX"));
	FlexGridSizer4->Add(txtPositionX, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bPositionX = new wxButton(this, ID_bPositionX, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bPositionX"));
	FlexGridSizer4->Add(bPositionX, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText23 = new wxStaticText(this, ID_STATICTEXT22, _("Y:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT22"));
	FlexGridSizer4->Add(StaticText23, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtPositionY = new wxTextCtrl(this, ID_txtPositionY, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_txtPositionY"));
	FlexGridSizer4->Add(txtPositionY, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bPositionY = new wxButton(this, ID_bPositionY, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bPositionY"));
	FlexGridSizer4->Add(bPositionY, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText24 = new wxStaticText(this, ID_STATICTEXT23, _("Z:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT23"));
	FlexGridSizer4->Add(StaticText24, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtPositionZ = new wxTextCtrl(this, ID_txtPositionZ, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_txtPositionZ"));
	FlexGridSizer4->Add(txtPositionZ, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bPositionZ = new wxButton(this, ID_bPositionZ, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bPositionZ"));
	FlexGridSizer4->Add(bPositionZ, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer6->Add(FlexGridSizer4, 1, wxTOP|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
	FlexGridSizer6->Add(StaticLine1, 1, wxTOP|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText28 = new wxStaticText(this, ID_STATICTEXT27, _("Rotation"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT27"));
	FlexGridSizer6->Add(StaticText28, 1, wxTOP|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer7 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText25 = new wxStaticText(this, ID_STATICTEXT24, _("X:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT24"));
	FlexGridSizer7->Add(StaticText25, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtRotationX = new wxTextCtrl(this, ID_txtRotationX, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_txtRotationX"));
	FlexGridSizer7->Add(txtRotationX, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bRotationX = new wxButton(this, ID_bRotationX, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bRotationX"));
	FlexGridSizer7->Add(bRotationX, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText26 = new wxStaticText(this, ID_STATICTEXT25, _("Y:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT25"));
	FlexGridSizer7->Add(StaticText26, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtRotationY = new wxTextCtrl(this, ID_txtRotationY, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_txtRotationY"));
	FlexGridSizer7->Add(txtRotationY, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bRotationY = new wxButton(this, ID_bRotationY, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bRotationY"));
	FlexGridSizer7->Add(bRotationY, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText27 = new wxStaticText(this, ID_STATICTEXT26, _("Z:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT26"));
	FlexGridSizer7->Add(StaticText27, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtRotationZ = new wxTextCtrl(this, ID_txtRotationZ, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_txtRotationZ"));
	FlexGridSizer7->Add(txtRotationZ, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bRotationZ = new wxButton(this, ID_bRotationZ, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bRotationZ"));
	FlexGridSizer7->Add(bRotationZ, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer6->Add(FlexGridSizer7, 1, wxTOP|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticLine3 = new wxStaticLine(this, ID_STATICLINE3, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE3"));
	FlexGridSizer6->Add(StaticLine3, 1, wxTOP|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer10 = new wxFlexGridSizer(0, 4, 0, 0);
	StaticText30 = new wxStaticText(this, ID_STATICTEXT30, _("FOV:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT30"));
	FlexGridSizer10->Add(StaticText30, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtSetFOV = new wxTextCtrl(this, ID_txtSetFOV, _("0"), wxDefaultPosition, wxSize(50,-1), 0, wxDefaultValidator, _T("ID_txtSetFOV"));
	FlexGridSizer10->Add(txtSetFOV, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetFOV = new wxButton(this, ID_bSetFOV, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetFOV"));
	FlexGridSizer10->Add(bSetFOV, 1, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bResetFOV = new wxButton(this, ID_bResetFOV, _("Reset"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bResetFOV"));
	FlexGridSizer10->Add(bResetFOV, 1, wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer6->Add(FlexGridSizer10, 1, wxTOP|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer4->Add(FlexGridSizer6, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer9->Add(StaticBoxSizer4, 1, wxTOP|wxBOTTOM|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	FlexGridSizer1->Add(FlexGridSizer9, 1, wxALIGN_RIGHT|wxALIGN_TOP, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);
	Center();
	
	Connect(ID_bGravity,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CameraControl::On_bGravity_Click);
	Connect(ID_bGravityEnabled,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CameraControl::On_bGravityEnabled_Click);
	Connect(ID_bCollisions,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CameraControl::On_bCollisions_Click);
	Connect(ID_bReportCollisions,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CameraControl::On_bReportCollisions_Click);
	Connect(ID_bFreelook,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CameraControl::On_bFreelook_Click);
	Connect(ID_bFreelookSpeed,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CameraControl::On_bFreelookSpeed_Click);
	Connect(ID_rPosition,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&CameraControl::On_rPosition_Select);
	Connect(ID_rRotation,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&CameraControl::On_rRotation_Select);
	Connect(ID_bZPlus,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CameraControl::On_bZPlus_Click);
	Connect(ID_bYPlus,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CameraControl::On_bYPlus_Click);
	Connect(ID_bXNeg,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CameraControl::On_bXNeg_Click);
	Connect(ID_chkHold,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&CameraControl::On_chkHold_Click);
	Connect(ID_bXPlus,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CameraControl::On_bXPlus_Click);
	Connect(ID_bZNeg,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CameraControl::On_bZNeg_Click);
	Connect(ID_bYNeg,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CameraControl::On_bYNeg_Click);
	Connect(ID_bStartPosition,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CameraControl::On_bStartPosition_Click);
	Connect(ID_bStartRotation,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CameraControl::On_bStartRotation_Click);
	Connect(ID_bStartDirection,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CameraControl::On_bStartDirection_Click);
	Connect(ID_bPositionX,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CameraControl::On_bPositionX_Click);
	Connect(ID_bPositionY,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CameraControl::On_bPositionY_Click);
	Connect(ID_bPositionZ,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CameraControl::On_bPositionZ_Click);
	Connect(ID_bRotationX,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CameraControl::On_bRotationX_Click);
	Connect(ID_bRotationY,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CameraControl::On_bRotationY_Click);
	Connect(ID_bRotationZ,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CameraControl::On_bRotationZ_Click);
	Connect(ID_bSetFOV,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CameraControl::On_bSetFOV_Click);
	Connect(ID_bResetFOV,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CameraControl::On_bResetFOV_Click);
	//*)
	OnInit();
}

CameraControl::~CameraControl()
{

}


void CameraControl::OnInit()
{
	txtGravity->SetValue(TruncateNumber(Simcore->camera->GetGravity(), 4));
	txtFreelookSpeed->SetValue(wxVariant((long)Simcore->camera->Freelook_speed).GetString());
	hold_vector = csVector3(0, 0, 0);
}

void CameraControl::Loop()
{
	csVector3 direction_front, direction_top;
	Simcore->camera->GetDirection(direction_front, direction_top);
	txtDirectionFront->SetValue(TruncateNumber(direction_front.x, 2) + wxT(", ") + TruncateNumber(direction_front.y, 2) + wxT(", ") + TruncateNumber(direction_front.z, 2));
	txtDirectionTop->SetValue(TruncateNumber(direction_top.x, 2) + wxT(", ") + TruncateNumber(direction_top.y, 2) + wxT(", ") + TruncateNumber(direction_top.z, 2));
	txtCurrentFloor->SetValue(wxVariant((long)Simcore->camera->CurrentFloor).GetString() + wxT(" (") + wxString::FromAscii(Simcore->camera->CurrentFloorID) + wxT(")"));
	txtLastMesh->SetValue(wxString::FromAscii(Simcore->camera->LastHitMesh));
	txtDesiredVel->SetValue(TruncateNumber(Simcore->camera->desired_velocity.x, 2) + wxT(", ") + TruncateNumber(Simcore->camera->desired_velocity.y, 2) + wxT(", ") + TruncateNumber(Simcore->camera->desired_velocity.z, 2));
	txtVelocity->SetValue(TruncateNumber(Simcore->camera->velocity.x, 2) + wxT(", ") + TruncateNumber(Simcore->camera->velocity.y, 2) + wxT(", ") + TruncateNumber(Simcore->camera->velocity.z, 2));
	txtDesiredAngle->SetValue(TruncateNumber(Simcore->camera->desired_angle_velocity.x, 2) + wxT(", ") + TruncateNumber(Simcore->camera->desired_angle_velocity.y, 2) + wxT(", ") + TruncateNumber(Simcore->camera->desired_angle_velocity.z, 2));
	txtAngle->SetValue(TruncateNumber(Simcore->camera->angle_velocity.x, 2) + wxT(", ") + TruncateNumber(Simcore->camera->angle_velocity.y, 2) + wxT(", ") + TruncateNumber(Simcore->camera->angle_velocity.z, 2));
	txtSpeed->SetValue(TruncateNumber(Simcore->camera->speed, 2));
	txtMouseDown->SetValue(wxString::FromAscii(BoolToString(Simcore->camera->MouseDown)));
	txtStartFloor->SetValue(wxVariant((long)Simcore->camera->StartFloor).GetString());
	txtStartPosition->SetValue(TruncateNumber(Simcore->camera->StartPositionX, 2) + wxT(", ") + TruncateNumber(Simcore->camera->StartPositionZ, 2));
	txtGravityEnabled->SetValue(wxString::FromAscii(BoolToString(Simcore->camera->GetGravityStatus())));
	txtCollisions->SetValue(wxString::FromAscii(BoolToString(Simcore->camera->EnableCollisions)));
	txtReportCollisions->SetValue(wxString::FromAscii(BoolToString(Simcore->camera->ReportCollisions)));
	txtFreelook->SetValue(wxString::FromAscii(BoolToString(Simcore->camera->Freelook)));
	lblPosition->SetLabel(TruncateNumber(Simcore->camera->GetPosition().x, 2) + wxT(", ") + TruncateNumber(Simcore->camera->GetPosition().y, 2) + wxT(", ") + TruncateNumber(Simcore->camera->GetPosition().z, 2));
	lblRotation->SetLabel(TruncateNumber(Simcore->camera->GetRotation().x, 2) + wxT(", ") + TruncateNumber(Simcore->camera->GetRotation().y, 2) + wxT(", ") + TruncateNumber(Simcore->camera->GetRotation().z, 2));
	txtFOV->SetValue(TruncateNumber(Simcore->camera->GetFOVAngle(), 4));

	//move if hold vector is not zero
	if (hold_vector != csVector3(0, 0, 0))
		Simcore->camera->Move(hold_vector, 1);
}

void CameraControl::On_rPosition_Select(wxCommandEvent& event)
{

}

void CameraControl::On_rRotation_Select(wxCommandEvent& event)
{

}

void CameraControl::On_bZPlus_Click(wxCommandEvent& event)
{
	if (chkHold->GetValue() == true)
	{
		hold_vector += csVector3(0, 0, atof(txtMoveSpeed->GetValue().ToAscii()));
		return;
	}

	if (rPosition->GetValue() == true)
		Simcore->camera->Move(csVector3(0, 0, 1), atof(txtMoveSpeed->GetValue().ToAscii()));
	else
		Simcore->camera->Rotate(csVector3(0, 0, 1), atof(txtMoveSpeed->GetValue().ToAscii()));
}

void CameraControl::On_bYPlus_Click(wxCommandEvent& event)
{
	if (chkHold->GetValue() == true)
	{
		hold_vector += csVector3(0, atof(txtMoveSpeed->GetValue().ToAscii()), 0);
		return;
	}

	if (rPosition->GetValue() == true)
		Simcore->camera->Move(csVector3(0, 1, 0), atof(txtMoveSpeed->GetValue().ToAscii()));
	else
		Simcore->camera->Rotate(csVector3(0, 1, 0), atof(txtMoveSpeed->GetValue().ToAscii()));
}

void CameraControl::On_bXNeg_Click(wxCommandEvent& event)
{
	if (chkHold->GetValue() == true)
	{
		hold_vector -= csVector3(atof(txtMoveSpeed->GetValue().ToAscii()), 0, 0);
		return;
	}

	if (rPosition->GetValue() == true)
		Simcore->camera->Move(csVector3(-1, 0, 0), atof(txtMoveSpeed->GetValue().ToAscii()));
	else
		Simcore->camera->Rotate(csVector3(-1, 0, 0), atof(txtMoveSpeed->GetValue().ToAscii()));
}

void CameraControl::On_bXPlus_Click(wxCommandEvent& event)
{
	if (chkHold->GetValue() == true)
	{
		hold_vector += csVector3(atof(txtMoveSpeed->GetValue().ToAscii()), 0, 0);
		return;
	}

	if (rPosition->GetValue() == true)
		Simcore->camera->Move(csVector3(1, 0, 0), atof(txtMoveSpeed->GetValue().ToAscii()));
	else
		Simcore->camera->Rotate(csVector3(1, 0, 0), atof(txtMoveSpeed->GetValue().ToAscii()));
}

void CameraControl::On_bZNeg_Click(wxCommandEvent& event)
{
	if (chkHold->GetValue() == true)
	{
		hold_vector -= csVector3(0, 0, atof(txtMoveSpeed->GetValue().ToAscii()));
		return;
	}

	if (rPosition->GetValue() == true)
		Simcore->camera->Move(csVector3(0, 0, -1), atof(txtMoveSpeed->GetValue().ToAscii()));
	else
		Simcore->camera->Rotate(csVector3(0, 0, -1), atof(txtMoveSpeed->GetValue().ToAscii()));
}

void CameraControl::On_bYNeg_Click(wxCommandEvent& event)
{
	if (chkHold->GetValue() == true)
	{
		hold_vector -= csVector3(0, atof(txtMoveSpeed->GetValue().ToAscii()), 0);
		return;
	}

	if (rPosition->GetValue() == true)
		Simcore->camera->Move(csVector3(0, -1, 0), atof(txtMoveSpeed->GetValue().ToAscii()));
	else
		Simcore->camera->Rotate(csVector3(0, -1, 0), atof(txtMoveSpeed->GetValue().ToAscii()));
}

void CameraControl::On_bStartPosition_Click(wxCommandEvent& event)
{
	Simcore->camera->SetToStartPosition();
}

void CameraControl::On_bStartRotation_Click(wxCommandEvent& event)
{
	Simcore->camera->SetToStartRotation();
}

void CameraControl::On_bStartDirection_Click(wxCommandEvent& event)
{
	Simcore->camera->SetToStartDirection();
}

void CameraControl::On_bGravity_Click(wxCommandEvent& event)
{
	Simcore->camera->SetGravity(atof(txtGravity->GetValue().ToAscii()));
}

void CameraControl::On_bGravityEnabled_Click(wxCommandEvent& event)
{
	Simcore->camera->EnableGravity(!Simcore->camera->GetGravityStatus());
}

void CameraControl::On_bCollisions_Click(wxCommandEvent& event)
{
	Simcore->camera->EnableCollisions = !Simcore->camera->EnableCollisions;
}

void CameraControl::On_bReportCollisions_Click(wxCommandEvent& event)
{
	Simcore->camera->ReportCollisions = !Simcore->camera->ReportCollisions;
}

void CameraControl::On_bFreelook_Click(wxCommandEvent& event)
{
	Simcore->camera->Freelook = !Simcore->camera->Freelook;
}

void CameraControl::On_bFreelookSpeed_Click(wxCommandEvent& event)
{
	Simcore->camera->Freelook_speed = atof(txtFreelookSpeed->GetValue().ToAscii());
}

void CameraControl::On_bPositionX_Click(wxCommandEvent& event)
{
	Simcore->camera->SetPosition(csVector3(atof(txtPositionX->GetValue().ToAscii()), Simcore->camera->GetPosition().y, Simcore->camera->GetPosition().z));
}

void CameraControl::On_bPositionY_Click(wxCommandEvent& event)
{
	Simcore->camera->SetPosition(csVector3(Simcore->camera->GetPosition().x, atof(txtPositionY->GetValue().ToAscii()), Simcore->camera->GetPosition().z));
}

void CameraControl::On_bPositionZ_Click(wxCommandEvent& event)
{
	Simcore->camera->SetPosition(csVector3(Simcore->camera->GetPosition().x, Simcore->camera->GetPosition().y, atof(txtPositionZ->GetValue().ToAscii())));
}

void CameraControl::On_bRotationX_Click(wxCommandEvent& event)
{
	Simcore->camera->SetRotation(csVector3(atof(txtRotationX->GetValue().ToAscii()), Simcore->camera->GetRotation().y, Simcore->camera->GetRotation().z));
}

void CameraControl::On_bRotationY_Click(wxCommandEvent& event)
{
	Simcore->camera->SetRotation(csVector3(Simcore->camera->GetRotation().x, atof(txtRotationY->GetValue().ToAscii()), Simcore->camera->GetRotation().z));
}

void CameraControl::On_bRotationZ_Click(wxCommandEvent& event)
{
	Simcore->camera->SetRotation(csVector3(Simcore->camera->GetRotation().x, Simcore->camera->GetRotation().y, atof(txtRotationZ->GetValue().ToAscii())));
}

void CameraControl::On_chkHold_Click(wxCommandEvent& event)
{
	if (chkHold->GetValue() == false)
		hold_vector = csVector3(0, 0, 0);
}

void CameraControl::On_bSetFOV_Click(wxCommandEvent& event)
{
	Simcore->camera->SetFOVAngle(atof(txtSetFOV->GetValue().ToAscii()));
}

void CameraControl::On_bResetFOV_Click(wxCommandEvent& event)
{
	Simcore->camera->SetToDefaultFOV();
}
