/* $Id$ */

/*
	Skyscraper 1.8 Alpha - Edit Elevator Form
	Copyright (C)20032012 Ryan Thoryk
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

//(*InternalHeaders(editelevator)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include "debugpanel.h"
#include "editelevator.h"
#include "globals.h"
#include "sbs.h"
#include "elevator.h"
#include "unix.h"

extern SBS *Simcore; //external pointer to the SBS engine
Elevator *elevator;
ElevatorDoor *door;

//(*IdInit(editelevator)
const long editelevator::ID_tElevator = wxNewId();
const long editelevator::ID_sNumber = wxNewId();
const long editelevator::ID_tFloor = wxNewId();
const long editelevator::ID_sFloor = wxNewId();
const long editelevator::ID_bACPMode = wxNewId();
const long editelevator::ID_bUpPeak = wxNewId();
const long editelevator::ID_bDownPeak = wxNewId();
const long editelevator::ID_bIndService = wxNewId();
const long editelevator::ID_bInsService = wxNewId();
const long editelevator::ID_Fire1Off = wxNewId();
const long editelevator::ID_Fire1On = wxNewId();
const long editelevator::ID_Fire1Bypass = wxNewId();
const long editelevator::ID_Fire2Off = wxNewId();
const long editelevator::ID_Fire2On = wxNewId();
const long editelevator::ID_Fire2Hold = wxNewId();
const long editelevator::ID_bUp = wxNewId();
const long editelevator::ID_bGoToggle = wxNewId();
const long editelevator::ID_bDown = wxNewId();
const long editelevator::ID_tDoor = wxNewId();
const long editelevator::ID_sDoor = wxNewId();
const long editelevator::ID_bRefresh = wxNewId();
const long editelevator::ID_bDumpFloors = wxNewId();
const long editelevator::ID_bDumpQueues = wxNewId();
const long editelevator::ID_CHECKBOX1 = wxNewId();
const long editelevator::ID_chkRun = wxNewId();
const long editelevator::ID_bCall = wxNewId();
const long editelevator::ID_bGo = wxNewId();
const long editelevator::ID_bOpen = wxNewId();
const long editelevator::ID_bOpenManual = wxNewId();
const long editelevator::ID_bOpenShaftDoor = wxNewId();
const long editelevator::ID_bStop = wxNewId();
const long editelevator::ID_bChime = wxNewId();
const long editelevator::ID_bUpIndicator = wxNewId();
const long editelevator::ID_bEnqueueUp = wxNewId();
const long editelevator::ID_bEnqueueDown = wxNewId();
const long editelevator::ID_bClose = wxNewId();
const long editelevator::ID_bCloseManual = wxNewId();
const long editelevator::ID_bCloseShaftDoor = wxNewId();
const long editelevator::ID_bAlarm = wxNewId();
const long editelevator::ID_bStopDoors = wxNewId();
const long editelevator::ID_bDownIndicator = wxNewId();
const long editelevator::ID_STATICTEXT3 = wxNewId();
const long editelevator::ID_txtNumber = wxNewId();
const long editelevator::ID_STATICTEXT5 = wxNewId();
const long editelevator::ID_txtName = wxNewId();
const long editelevator::ID_bSetName = wxNewId();
const long editelevator::ID_STATICTEXT4 = wxNewId();
const long editelevator::ID_txtEnabled = wxNewId();
const long editelevator::ID_STATICTEXT6 = wxNewId();
const long editelevator::ID_txtShaft = wxNewId();
const long editelevator::ID_STATICTEXT7 = wxNewId();
const long editelevator::ID_txtHeight = wxNewId();
const long editelevator::ID_STATICTEXT8 = wxNewId();
const long editelevator::ID_txtDoorWidth = wxNewId();
const long editelevator::ID_STATICTEXT9 = wxNewId();
const long editelevator::ID_txtDoorHeight = wxNewId();
const long editelevator::ID_STATICTEXT10 = wxNewId();
const long editelevator::ID_txtDoorDirection = wxNewId();
const long editelevator::ID_STATICTEXT11 = wxNewId();
const long editelevator::ID_txtDoorsOpen = wxNewId();
const long editelevator::ID_STATICTEXT12 = wxNewId();
const long editelevator::ID_txtBrakes = wxNewId();
const long editelevator::ID_STATICTEXT13 = wxNewId();
const long editelevator::ID_txtStop = wxNewId();
const long editelevator::ID_STATICTEXT42 = wxNewId();
const long editelevator::ID_txtDoorTimer = wxNewId();
const long editelevator::ID_bSetDoorTimer = wxNewId();
const long editelevator::ID_STATICTEXT61 = wxNewId();
const long editelevator::ID_txtQuickClose = wxNewId();
const long editelevator::ID_bSetQuickClose = wxNewId();
const long editelevator::ID_STATICTEXT32 = wxNewId();
const long editelevator::ID_txtUseSkipFloor = wxNewId();
const long editelevator::ID_STATICTEXT44 = wxNewId();
const long editelevator::ID_txtSkipFloorText = wxNewId();
const long editelevator::ID_bSetSkipFloorText = wxNewId();
const long editelevator::ID_STATICTEXT52 = wxNewId();
const long editelevator::ID_txtAlarm = wxNewId();
const long editelevator::ID_STATICTEXT28 = wxNewId();
const long editelevator::ID_txtWaitForDoors = wxNewId();
const long editelevator::ID_STATICTEXT67 = wxNewId();
const long editelevator::ID_txtNudgeMode = wxNewId();
const long editelevator::ID_STATICTEXT14 = wxNewId();
const long editelevator::ID_txtFloor = wxNewId();
const long editelevator::ID_STATICTEXT15 = wxNewId();
const long editelevator::ID_txtPosition = wxNewId();
const long editelevator::ID_STATICTEXT16 = wxNewId();
const long editelevator::ID_txtOrigin = wxNewId();
const long editelevator::ID_STATICTEXT17 = wxNewId();
const long editelevator::ID_txtOriginFloor = wxNewId();
const long editelevator::ID_STATICTEXT18 = wxNewId();
const long editelevator::ID_txtElevStart = wxNewId();
const long editelevator::ID_STATICTEXT19 = wxNewId();
const long editelevator::ID_txtDoorOrigin = wxNewId();
const long editelevator::ID_STATICTEXT20 = wxNewId();
const long editelevator::ID_txtShaftDoorOrigin = wxNewId();
const long editelevator::ID_STATICTEXT50 = wxNewId();
const long editelevator::ID_txtOnFloor = wxNewId();
const long editelevator::ID_STATICTEXT25 = wxNewId();
const long editelevator::ID_txtMotor = wxNewId();
const long editelevator::ID_STATICTEXT54 = wxNewId();
const long editelevator::ID_txtCameraOffset = wxNewId();
const long editelevator::ID_STATICTEXT38 = wxNewId();
const long editelevator::ID_txtQueueDirection = wxNewId();
const long editelevator::ID_STATICTEXT40 = wxNewId();
const long editelevator::ID_txtQueueLastUp = wxNewId();
const long editelevator::ID_STATICTEXT41 = wxNewId();
const long editelevator::ID_txtQueueLastDown = wxNewId();
const long editelevator::ID_STATICTEXT51 = wxNewId();
const long editelevator::ID_txtQueueLastDirection = wxNewId();
const long editelevator::ID_bResetQueues = wxNewId();
const long editelevator::ID_STATICTEXT21 = wxNewId();
const long editelevator::ID_txtSpeed = wxNewId();
const long editelevator::ID_bSetSpeed = wxNewId();
const long editelevator::ID_STATICTEXT22 = wxNewId();
const long editelevator::ID_txtAcceleration = wxNewId();
const long editelevator::ID_bSetAcceleration = wxNewId();
const long editelevator::ID_STATICTEXT23 = wxNewId();
const long editelevator::ID_txtDeceleration = wxNewId();
const long editelevator::ID_bSetDeceleration = wxNewId();
const long editelevator::ID_STATICTEXT1 = wxNewId();
const long editelevator::ID_txtAccelJerk = wxNewId();
const long editelevator::ID_bSetAccelJerk = wxNewId();
const long editelevator::ID_STATICTEXT29 = wxNewId();
const long editelevator::ID_txtDecelJerk = wxNewId();
const long editelevator::ID_bSetDecelJerk = wxNewId();
const long editelevator::ID_STATICTEXT26 = wxNewId();
const long editelevator::ID_txtRate = wxNewId();
const long editelevator::ID_STATICTEXT27 = wxNewId();
const long editelevator::ID_txtDirection = wxNewId();
const long editelevator::ID_STATICTEXT2 = wxNewId();
const long editelevator::ID_txtJerkRate = wxNewId();
const long editelevator::ID_STATICTEXT49 = wxNewId();
const long editelevator::ID_txtDoorStopped = wxNewId();
const long editelevator::ID_STATICTEXT53 = wxNewId();
const long editelevator::ID_txtIsIdle = wxNewId();
const long editelevator::ID_STATICTEXT24 = wxNewId();
const long editelevator::ID_txtManualGo = wxNewId();
const long editelevator::ID_STATICTEXT55 = wxNewId();
const long editelevator::ID_txtLeveling = wxNewId();
const long editelevator::ID_STATICTEXT56 = wxNewId();
const long editelevator::ID_txtLevelingSpeed = wxNewId();
const long editelevator::ID_bSetLevelingSpeed = wxNewId();
const long editelevator::ID_STATICTEXT57 = wxNewId();
const long editelevator::ID_txtLevelingOffset = wxNewId();
const long editelevator::ID_bSetLevelingOffset = wxNewId();
const long editelevator::ID_STATICTEXT58 = wxNewId();
const long editelevator::ID_txtLevelingOpen = wxNewId();
const long editelevator::ID_bSetLevelingOpen = wxNewId();
const long editelevator::ID_STATICTEXT60 = wxNewId();
const long editelevator::ID_txtParking = wxNewId();
const long editelevator::ID_STATICTEXT63 = wxNewId();
const long editelevator::ID_txtSlowSpeed = wxNewId();
const long editelevator::ID_bSetSlowSpeed = wxNewId();
const long editelevator::ID_STATICTEXT64 = wxNewId();
const long editelevator::ID_txtManualSpeed = wxNewId();
const long editelevator::ID_bSetManualSpeed = wxNewId();
const long editelevator::ID_STATICTEXT30 = wxNewId();
const long editelevator::ID_txtDestFloor = wxNewId();
const long editelevator::ID_STATICTEXT43 = wxNewId();
const long editelevator::ID_txtIsMoving = wxNewId();
const long editelevator::ID_STATICTEXT31 = wxNewId();
const long editelevator::ID_txtMoveElevator = wxNewId();
const long editelevator::ID_STATICTEXT33 = wxNewId();
const long editelevator::ID_txtDistance = wxNewId();
const long editelevator::ID_STATICTEXT34 = wxNewId();
const long editelevator::ID_txtDestination = wxNewId();
const long editelevator::ID_STATICTEXT35 = wxNewId();
const long editelevator::ID_txtStopDistance = wxNewId();
const long editelevator::ID_STATICTEXT36 = wxNewId();
const long editelevator::ID_txtTempDecel = wxNewId();
const long editelevator::ID_STATICTEXT37 = wxNewId();
const long editelevator::ID_txtErrorOffset = wxNewId();
const long editelevator::ID_STATICTEXT59 = wxNewId();
const long editelevator::ID_txtNotifyEarly = wxNewId();
const long editelevator::ID_bNotifyEarly = wxNewId();
const long editelevator::ID_STATICTEXT65 = wxNewId();
const long editelevator::ID_txtDepartureDelay = wxNewId();
const long editelevator::ID_bSetDepartureDelay = wxNewId();
const long editelevator::ID_STATICTEXT66 = wxNewId();
const long editelevator::ID_txtArrivalDelay = wxNewId();
const long editelevator::ID_bSetArrivalDelay = wxNewId();
const long editelevator::ID_STATICTEXT45 = wxNewId();
const long editelevator::ID_txtACPFloor = wxNewId();
const long editelevator::ID_bSetACPFloor = wxNewId();
const long editelevator::ID_STATICTEXT46 = wxNewId();
const long editelevator::ID_txtRecallFloor = wxNewId();
const long editelevator::ID_bSetRecallFloor = wxNewId();
const long editelevator::ID_STATICTEXT47 = wxNewId();
const long editelevator::ID_txtRecallAlternate = wxNewId();
const long editelevator::ID_bSetRecallAlternate = wxNewId();
const long editelevator::ID_STATICTEXT39 = wxNewId();
const long editelevator::ID_txtParkingFloor = wxNewId();
const long editelevator::ID_bSetParkingFloor = wxNewId();
const long editelevator::ID_STATICTEXT48 = wxNewId();
const long editelevator::ID_txtParkingDelay = wxNewId();
const long editelevator::ID_bSetParkingDelay = wxNewId();
const long editelevator::ID_STATICTEXT62 = wxNewId();
const long editelevator::ID_txtNudgeTimer = wxNewId();
const long editelevator::ID_bSetNudgeTimer = wxNewId();
//*)

BEGIN_EVENT_TABLE(editelevator,wxDialog)
//(*EventTable(editelevator)
//*)
END_EVENT_TABLE()

editelevator::editelevator(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(editelevator)
	wxBoxSizer* BoxSizer4;
	wxFlexGridSizer* FlexGridSizer4;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer7;
	wxStaticBoxSizer* StaticBoxSizer4;
	wxFlexGridSizer* FlexGridSizer5;
	wxStaticBoxSizer* StaticBoxSizer9;
	wxStaticBoxSizer* StaticBoxSizer10;
	wxStaticBoxSizer* StaticBoxSizer6;
	wxFlexGridSizer* FlexGridSizer8;
	
	Create(parent, wxID_ANY, _("Elevator Editor"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxMINIMIZE_BOX, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer6 = new wxBoxSizer(wxVERTICAL);
	tElevator = new wxStaticText(this, ID_tElevator, _("Elevator"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_tElevator"));
	BoxSizer6->Add(tElevator, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	sNumber = new wxScrollBar(this, ID_sNumber, wxDefaultPosition, wxDefaultSize, wxSB_HORIZONTAL, wxDefaultValidator, _T("ID_sNumber"));
	sNumber->SetScrollbar(0, 0, 0, 0);
	BoxSizer6->Add(sNumber, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	tFloor = new wxStaticText(this, ID_tFloor, _("Floor"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_tFloor"));
	BoxSizer6->Add(tFloor, 0, wxTOP|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 10);
	sFloor = new wxScrollBar(this, ID_sFloor, wxDefaultPosition, wxDefaultSize, wxSB_HORIZONTAL, wxDefaultValidator, _T("ID_sFloor"));
	sFloor->SetScrollbar(0, 0, 0, 0);
	BoxSizer6->Add(sFloor, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer4 = new wxFlexGridSizer(0, 5, 0, 5);
	BoxSizer5 = new wxBoxSizer(wxVERTICAL);
	bACPMode = new wxToggleButton(this, ID_bACPMode, _("ACP Mode"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bACPMode"));
	BoxSizer5->Add(bACPMode, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bUpPeak = new wxToggleButton(this, ID_bUpPeak, _("Up Peak"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bUpPeak"));
	BoxSizer5->Add(bUpPeak, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bDownPeak = new wxToggleButton(this, ID_bDownPeak, _("Down Peak"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bDownPeak"));
	BoxSizer5->Add(bDownPeak, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bIndService = new wxToggleButton(this, ID_bIndService, _("Independent Service"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bIndService"));
	BoxSizer5->Add(bIndService, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bInsService = new wxToggleButton(this, ID_bInsService, _("Inspection Service"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bInsService"));
	BoxSizer5->Add(bInsService, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer4->Add(BoxSizer5, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer10 = new wxStaticBoxSizer(wxVERTICAL, this, _("Fire Ph 1"));
	Fire1Off = new wxRadioButton(this, ID_Fire1Off, _("Off"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_Fire1Off"));
	Fire1Off->SetValue(true);
	StaticBoxSizer10->Add(Fire1Off, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	Fire1On = new wxRadioButton(this, ID_Fire1On, _("On"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_Fire1On"));
	StaticBoxSizer10->Add(Fire1On, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	Fire1Bypass = new wxRadioButton(this, ID_Fire1Bypass, _("Bypass"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_Fire1Bypass"));
	StaticBoxSizer10->Add(Fire1Bypass, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer4->Add(StaticBoxSizer10, 1, wxTOP|wxBOTTOM|wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer9 = new wxStaticBoxSizer(wxVERTICAL, this, _("Fire Ph 2"));
	Fire2Off = new wxRadioButton(this, ID_Fire2Off, _("Off"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_Fire2Off"));
	StaticBoxSizer9->Add(Fire2Off, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	Fire2On = new wxRadioButton(this, ID_Fire2On, _("On"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_Fire2On"));
	StaticBoxSizer9->Add(Fire2On, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	Fire2Hold = new wxRadioButton(this, ID_Fire2Hold, _("Hold"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_Fire2Hold"));
	StaticBoxSizer9->Add(Fire2Hold, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer4->Add(StaticBoxSizer9, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer6 = new wxStaticBoxSizer(wxVERTICAL, this, _("Manual"));
	bUp = new wxToggleButton(this, ID_bUp, _("Up"), wxDefaultPosition, wxSize(50,0), 0, wxDefaultValidator, _T("ID_bUp"));
	StaticBoxSizer6->Add(bUp, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bGoToggle = new wxToggleButton(this, ID_bGoToggle, _("Go"), wxDefaultPosition, wxSize(50,-1), 0, wxDefaultValidator, _T("ID_bGoToggle"));
	StaticBoxSizer6->Add(bGoToggle, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bDown = new wxToggleButton(this, ID_bDown, _("Down"), wxDefaultPosition, wxSize(50,0), 0, wxDefaultValidator, _T("ID_bDown"));
	StaticBoxSizer6->Add(bDown, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer4->Add(StaticBoxSizer6, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer8 = new wxFlexGridSizer(0, 1, 0, 0);
	tDoor = new wxStaticText(this, ID_tDoor, _("Door"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_tDoor"));
	FlexGridSizer8->Add(tDoor, 1, wxEXPAND|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	sDoor = new wxScrollBar(this, ID_sDoor, wxDefaultPosition, wxDefaultSize, wxSB_HORIZONTAL, wxDefaultValidator, _T("ID_sDoor"));
	sDoor->SetScrollbar(0, 0, 0, 0);
	FlexGridSizer8->Add(sDoor, 1, wxBOTTOM|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bRefresh = new wxButton(this, ID_bRefresh, _("Refresh Values"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bRefresh"));
	FlexGridSizer8->Add(bRefresh, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bDumpFloors = new wxButton(this, ID_bDumpFloors, _("Dump Floor List"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bDumpFloors"));
	FlexGridSizer8->Add(bDumpFloors, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bDumpQueues = new wxButton(this, ID_bDumpQueues, _("Dump Queues"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bDumpQueues"));
	FlexGridSizer8->Add(bDumpQueues, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	chkVisible = new wxCheckBox(this, ID_CHECKBOX1, _("Visible"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	chkVisible->SetValue(false);
	BoxSizer4->Add(chkVisible, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	chkRun = new wxCheckBox(this, ID_chkRun, _("Run"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkRun"));
	chkRun->SetValue(false);
	BoxSizer4->Add(chkRun, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer8->Add(BoxSizer4, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer4->Add(FlexGridSizer8, 1, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer6->Add(FlexGridSizer4, 1, wxTOP|wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(BoxSizer6, 2, wxTOP|wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Control"));
	BoxSizer8 = new wxBoxSizer(wxVERTICAL);
	bCall = new wxButton(this, ID_bCall, _("Call"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bCall"));
	bCall->SetFocus();
	BoxSizer8->Add(bCall, 0, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
	bGo = new wxButton(this, ID_bGo, _("Go"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bGo"));
	BoxSizer8->Add(bGo, 0, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
	bOpen = new wxButton(this, ID_bOpen, _("Open"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bOpen"));
	BoxSizer8->Add(bOpen, 0, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
	bOpenManual = new wxButton(this, ID_bOpenManual, _("Open Manual"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bOpenManual"));
	BoxSizer8->Add(bOpenManual, 0, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
	bOpenShaftDoor = new wxButton(this, ID_bOpenShaftDoor, _("Open Shaft Doors"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bOpenShaftDoor"));
	BoxSizer8->Add(bOpenShaftDoor, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bStop = new wxButton(this, ID_bStop, _("Emerg. Stop"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bStop"));
	BoxSizer8->Add(bStop, 0, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
	bChime = new wxButton(this, ID_bChime, _("Chime"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bChime"));
	BoxSizer8->Add(bChime, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bUpIndicator = new wxButton(this, ID_bUpIndicator, _("Set Up Indicator"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bUpIndicator"));
	BoxSizer8->Add(bUpIndicator, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer1->Add(BoxSizer8, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	BoxSizer9 = new wxBoxSizer(wxVERTICAL);
	bEnqueueUp = new wxButton(this, ID_bEnqueueUp, _("Enqueue Up"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bEnqueueUp"));
	BoxSizer9->Add(bEnqueueUp, 0, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
	bEnqueueDown = new wxButton(this, ID_bEnqueueDown, _("Enqueue Down"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bEnqueueDown"));
	BoxSizer9->Add(bEnqueueDown, 0, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
	bClose = new wxButton(this, ID_bClose, _("Close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bClose"));
	BoxSizer9->Add(bClose, 0, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
	bCloseManual = new wxButton(this, ID_bCloseManual, _("Close Manual"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bCloseManual"));
	BoxSizer9->Add(bCloseManual, 0, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
	bCloseShaftDoor = new wxButton(this, ID_bCloseShaftDoor, _("Close Shaft Doors"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bCloseShaftDoor"));
	BoxSizer9->Add(bCloseShaftDoor, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bAlarm = new wxButton(this, ID_bAlarm, _("Alarm"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bAlarm"));
	BoxSizer9->Add(bAlarm, 0, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
	bStopDoors = new wxButton(this, ID_bStopDoors, _("Stop Doors"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bStopDoors"));
	BoxSizer9->Add(bStopDoors, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bDownIndicator = new wxButton(this, ID_bDownIndicator, _("Set Down Indicator"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bDownIndicator"));
	BoxSizer9->Add(bDownIndicator, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer1->Add(BoxSizer9, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	BoxSizer2->Add(StaticBoxSizer1, 1, wxRIGHT|wxALIGN_RIGHT|wxALIGN_TOP, 5);
	BoxSizer1->Add(BoxSizer2, 0, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 0);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("General"));
	FlexGridSizer1 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Number:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	FlexGridSizer1->Add(StaticText3, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtNumber = new wxTextCtrl(this, ID_txtNumber, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtNumber"));
	FlexGridSizer1->Add(txtNumber, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer1->Add(-1,-1,0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Name:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	FlexGridSizer1->Add(StaticText5, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtName = new wxTextCtrl(this, ID_txtName, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtName"));
	FlexGridSizer1->Add(txtName, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bSetName = new wxButton(this, ID_bSetName, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetName"));
	FlexGridSizer1->Add(bSetName, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 0);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Enabled:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	FlexGridSizer1->Add(StaticText4, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtEnabled = new wxTextCtrl(this, ID_txtEnabled, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtEnabled"));
	FlexGridSizer1->Add(txtEnabled, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer1->Add(-1,-1,0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("Shaft:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	FlexGridSizer1->Add(StaticText6, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtShaft = new wxTextCtrl(this, ID_txtShaft, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtShaft"));
	FlexGridSizer1->Add(txtShaft, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer1->Add(-1,-1,0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Height:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	FlexGridSizer1->Add(StaticText7, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtHeight = new wxTextCtrl(this, ID_txtHeight, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtHeight"));
	FlexGridSizer1->Add(txtHeight, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer1->Add(-1,-1,0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("DWidth:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	FlexGridSizer1->Add(StaticText8, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtDoorWidth = new wxTextCtrl(this, ID_txtDoorWidth, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtDoorWidth"));
	txtDoorWidth->SetToolTip(_("Door Width"));
	FlexGridSizer1->Add(txtDoorWidth, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer1->Add(-1,-1,0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _("DHeight:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	FlexGridSizer1->Add(StaticText9, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtDoorHeight = new wxTextCtrl(this, ID_txtDoorHeight, wxEmptyString, wxPoint(-1,75), wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtDoorHeight"));
	txtDoorHeight->SetToolTip(_("Door Height"));
	FlexGridSizer1->Add(txtDoorHeight, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer1->Add(-1,-1,0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	StaticText10 = new wxStaticText(this, ID_STATICTEXT10, _("DDirection:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	FlexGridSizer1->Add(StaticText10, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtDoorDirection = new wxTextCtrl(this, ID_txtDoorDirection, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtDoorDirection"));
	txtDoorDirection->SetToolTip(_("Door Direction"));
	FlexGridSizer1->Add(txtDoorDirection, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer1->Add(-1,-1,0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	StaticText11 = new wxStaticText(this, ID_STATICTEXT11, _("DoorsOpen:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	FlexGridSizer1->Add(StaticText11, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtDoorsOpen = new wxTextCtrl(this, ID_txtDoorsOpen, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtDoorsOpen"));
	FlexGridSizer1->Add(txtDoorsOpen, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer1->Add(-1,-1,0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	StaticText12 = new wxStaticText(this, ID_STATICTEXT12, _("Brakes:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT12"));
	FlexGridSizer1->Add(StaticText12, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtBrakes = new wxTextCtrl(this, ID_txtBrakes, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtBrakes"));
	FlexGridSizer1->Add(txtBrakes, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer1->Add(-1,-1,0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	StaticText13 = new wxStaticText(this, ID_STATICTEXT13, _("EmergStop:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT13"));
	FlexGridSizer1->Add(StaticText13, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtStop = new wxTextCtrl(this, ID_txtStop, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtStop"));
	txtStop->SetToolTip(_("Emergency Stop"));
	FlexGridSizer1->Add(txtStop, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer1->Add(-1,-1,0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	StaticText42 = new wxStaticText(this, ID_STATICTEXT42, _("DoorTimer:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT42"));
	FlexGridSizer1->Add(StaticText42, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtDoorTimer = new wxTextCtrl(this, ID_txtDoorTimer, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtDoorTimer"));
	FlexGridSizer1->Add(txtDoorTimer, 1, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	bSetDoorTimer = new wxButton(this, ID_bSetDoorTimer, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetDoorTimer"));
	FlexGridSizer1->Add(bSetDoorTimer, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText61 = new wxStaticText(this, ID_STATICTEXT61, _("QuickClose:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT61"));
	FlexGridSizer1->Add(StaticText61, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtQuickClose = new wxTextCtrl(this, ID_txtQuickClose, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtQuickClose"));
	FlexGridSizer1->Add(txtQuickClose, 1, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	bSetQuickClose = new wxButton(this, ID_bSetQuickClose, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetQuickClose"));
	FlexGridSizer1->Add(bSetQuickClose, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText32 = new wxStaticText(this, ID_STATICTEXT32, _("UseFlrSkip:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT32"));
	FlexGridSizer1->Add(StaticText32, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtUseSkipFloor = new wxTextCtrl(this, ID_txtUseSkipFloor, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtUseSkipFloor"));
	txtUseSkipFloor->SetToolTip(_("Use Floor Skip Text"));
	FlexGridSizer1->Add(txtUseSkipFloor, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText44 = new wxStaticText(this, ID_STATICTEXT44, _("FlrSkipTxt:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT44"));
	FlexGridSizer1->Add(StaticText44, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtSkipFloorText = new wxTextCtrl(this, ID_txtSkipFloorText, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtSkipFloorText"));
	txtSkipFloorText->SetToolTip(_("Floor Skip Text"));
	FlexGridSizer1->Add(txtSkipFloorText, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetSkipFloorText = new wxButton(this, ID_bSetSkipFloorText, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetSkipFloorText"));
	FlexGridSizer1->Add(bSetSkipFloorText, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText52 = new wxStaticText(this, ID_STATICTEXT52, _("Alarm On:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT52"));
	FlexGridSizer1->Add(StaticText52, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtAlarm = new wxTextCtrl(this, ID_txtAlarm, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtAlarm"));
	FlexGridSizer1->Add(txtAlarm, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText28 = new wxStaticText(this, ID_STATICTEXT28, _("WaitDoors:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT28"));
	FlexGridSizer1->Add(StaticText28, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtWaitForDoors = new wxTextCtrl(this, ID_txtWaitForDoors, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtWaitForDoors"));
	txtWaitForDoors->SetToolTip(_("Wait for Doors"));
	FlexGridSizer1->Add(txtWaitForDoors, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText67 = new wxStaticText(this, ID_STATICTEXT67, _("Nudge:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT67"));
	FlexGridSizer1->Add(StaticText67, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtNudgeMode = new wxTextCtrl(this, ID_txtNudgeMode, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtNudgeMode"));
	txtNudgeMode->SetToolTip(_("Nudge Mode"));
	FlexGridSizer1->Add(txtNudgeMode, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer2->Add(FlexGridSizer1, 0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	BoxSizer3->Add(StaticBoxSizer2, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	BoxSizer10 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer5 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Position"));
	FlexGridSizer2 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticText14 = new wxStaticText(this, ID_STATICTEXT14, _("Floor:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT14"));
	FlexGridSizer2->Add(StaticText14, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtFloor = new wxTextCtrl(this, ID_txtFloor, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtFloor"));
	FlexGridSizer2->Add(txtFloor, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText15 = new wxStaticText(this, ID_STATICTEXT15, _("Pos:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT15"));
	FlexGridSizer2->Add(StaticText15, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtPosition = new wxTextCtrl(this, ID_txtPosition, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtPosition"));
	txtPosition->SetToolTip(_("Position"));
	FlexGridSizer2->Add(txtPosition, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText16 = new wxStaticText(this, ID_STATICTEXT16, _("Origin:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT16"));
	FlexGridSizer2->Add(StaticText16, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtOrigin = new wxTextCtrl(this, ID_txtOrigin, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtOrigin"));
	FlexGridSizer2->Add(txtOrigin, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText17 = new wxStaticText(this, ID_STATICTEXT17, _("OriginFlr:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT17"));
	FlexGridSizer2->Add(StaticText17, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtOriginFloor = new wxTextCtrl(this, ID_txtOriginFloor, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtOriginFloor"));
	txtOriginFloor->SetToolTip(_("Origin Floor"));
	FlexGridSizer2->Add(txtOriginFloor, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText18 = new wxStaticText(this, ID_STATICTEXT18, _("EStart:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT18"));
	FlexGridSizer2->Add(StaticText18, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtElevStart = new wxTextCtrl(this, ID_txtElevStart, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtElevStart"));
	txtElevStart->SetToolTip(_("Elevator Start"));
	FlexGridSizer2->Add(txtElevStart, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText19 = new wxStaticText(this, ID_STATICTEXT19, _("DOrigin:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT19"));
	FlexGridSizer2->Add(StaticText19, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtDoorOrigin = new wxTextCtrl(this, ID_txtDoorOrigin, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtDoorOrigin"));
	txtDoorOrigin->SetToolTip(_("Door Origin"));
	FlexGridSizer2->Add(txtDoorOrigin, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText20 = new wxStaticText(this, ID_STATICTEXT20, _("SDOrigin:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT20"));
	FlexGridSizer2->Add(StaticText20, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtShaftDoorOrigin = new wxTextCtrl(this, ID_txtShaftDoorOrigin, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtShaftDoorOrigin"));
	txtShaftDoorOrigin->SetToolTip(_("Shaft Door Origin"));
	FlexGridSizer2->Add(txtShaftDoorOrigin, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText50 = new wxStaticText(this, ID_STATICTEXT50, _("OnFloor:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT50"));
	FlexGridSizer2->Add(StaticText50, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtOnFloor = new wxTextCtrl(this, ID_txtOnFloor, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtOnFloor"));
	FlexGridSizer2->Add(txtOnFloor, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText25 = new wxStaticText(this, ID_STATICTEXT25, _("Motor:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT25"));
	FlexGridSizer2->Add(StaticText25, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtMotor = new wxTextCtrl(this, ID_txtMotor, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtMotor"));
	FlexGridSizer2->Add(txtMotor, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText54 = new wxStaticText(this, ID_STATICTEXT54, _("Camera:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT54"));
	FlexGridSizer2->Add(StaticText54, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtCameraOffset = new wxTextCtrl(this, ID_txtCameraOffset, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtCameraOffset"));
	FlexGridSizer2->Add(txtCameraOffset, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer5->Add(FlexGridSizer2, 0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	BoxSizer10->Add(StaticBoxSizer5, 0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	StaticBoxSizer3 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Queues"));
	FlexGridSizer3 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticText38 = new wxStaticText(this, ID_STATICTEXT38, _("Direction:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT38"));
	FlexGridSizer3->Add(StaticText38, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtQueueDirection = new wxTextCtrl(this, ID_txtQueueDirection, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtQueueDirection"));
	txtQueueDirection->SetToolTip(_("Queue Direction"));
	FlexGridSizer3->Add(txtQueueDirection, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText40 = new wxStaticText(this, ID_STATICTEXT40, _("LastUp:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT40"));
	FlexGridSizer3->Add(StaticText40, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtQueueLastUp = new wxTextCtrl(this, ID_txtQueueLastUp, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtQueueLastUp"));
	txtQueueLastUp->SetToolTip(_("Queue Last Up"));
	FlexGridSizer3->Add(txtQueueLastUp, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText41 = new wxStaticText(this, ID_STATICTEXT41, _("LastDwn:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT41"));
	FlexGridSizer3->Add(StaticText41, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtQueueLastDown = new wxTextCtrl(this, ID_txtQueueLastDown, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtQueueLastDown"));
	txtQueueLastDown->SetToolTip(_("Queue Last Down"));
	FlexGridSizer3->Add(txtQueueLastDown, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText51 = new wxStaticText(this, ID_STATICTEXT51, _("Last:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT51"));
	FlexGridSizer3->Add(StaticText51, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtQueueLastDirection = new wxTextCtrl(this, ID_txtQueueLastDirection, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtQueueLastDirection"));
	txtQueueLastDirection->SetToolTip(_("Queue Last Direction"));
	FlexGridSizer3->Add(txtQueueLastDirection, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer3->Add(-1,-1,1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bResetQueues = new wxButton(this, ID_bResetQueues, _("Reset"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bResetQueues"));
	FlexGridSizer3->Add(bResetQueues, 1, wxTOP|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer3->Add(FlexGridSizer3, 0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	BoxSizer10->Add(StaticBoxSizer3, 0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	BoxSizer3->Add(BoxSizer10, 0, wxBOTTOM|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticBoxSizer7 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Motion"));
	FlexGridSizer6 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText21 = new wxStaticText(this, ID_STATICTEXT21, _("Speed:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT21"));
	FlexGridSizer6->Add(StaticText21, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtSpeed = new wxTextCtrl(this, ID_txtSpeed, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtSpeed"));
	FlexGridSizer6->Add(txtSpeed, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bSetSpeed = new wxButton(this, ID_bSetSpeed, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetSpeed"));
	FlexGridSizer6->Add(bSetSpeed, 0, wxALL|wxALIGN_RIGHT|wxALIGN_TOP, 0);
	StaticText22 = new wxStaticText(this, ID_STATICTEXT22, _("Accel:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT22"));
	FlexGridSizer6->Add(StaticText22, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtAcceleration = new wxTextCtrl(this, ID_txtAcceleration, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtAcceleration"));
	txtAcceleration->SetToolTip(_("Acceleration"));
	FlexGridSizer6->Add(txtAcceleration, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bSetAcceleration = new wxButton(this, ID_bSetAcceleration, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetAcceleration"));
	FlexGridSizer6->Add(bSetAcceleration, 0, wxALL|wxALIGN_RIGHT|wxALIGN_TOP, 0);
	StaticText23 = new wxStaticText(this, ID_STATICTEXT23, _("Decel:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT23"));
	FlexGridSizer6->Add(StaticText23, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtDeceleration = new wxTextCtrl(this, ID_txtDeceleration, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtDeceleration"));
	txtDeceleration->SetToolTip(_("Deceleration"));
	FlexGridSizer6->Add(txtDeceleration, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bSetDeceleration = new wxButton(this, ID_bSetDeceleration, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetDeceleration"));
	FlexGridSizer6->Add(bSetDeceleration, 0, wxALL|wxALIGN_RIGHT|wxALIGN_TOP, 0);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("AccelJerk:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer6->Add(StaticText1, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtAccelJerk = new wxTextCtrl(this, ID_txtAccelJerk, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtAccelJerk"));
	txtAccelJerk->SetToolTip(_("Acceleration Jerk Rate"));
	FlexGridSizer6->Add(txtAccelJerk, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetAccelJerk = new wxButton(this, ID_bSetAccelJerk, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetAccelJerk"));
	FlexGridSizer6->Add(bSetAccelJerk, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText29 = new wxStaticText(this, ID_STATICTEXT29, _("DecelJerk:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT29"));
	FlexGridSizer6->Add(StaticText29, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtDecelJerk = new wxTextCtrl(this, ID_txtDecelJerk, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtDecelJerk"));
	txtDecelJerk->SetToolTip(_("Deceleration Jerk Rate"));
	FlexGridSizer6->Add(txtDecelJerk, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetDecelJerk = new wxButton(this, ID_bSetDecelJerk, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetDecelJerk"));
	FlexGridSizer6->Add(bSetDecelJerk, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText26 = new wxStaticText(this, ID_STATICTEXT26, _("Rate:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT26"));
	FlexGridSizer6->Add(StaticText26, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtRate = new wxTextCtrl(this, ID_txtRate, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtRate"));
	FlexGridSizer6->Add(txtRate, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer6->Add(-1,-1,0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	StaticText27 = new wxStaticText(this, ID_STATICTEXT27, _("Direction:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT27"));
	FlexGridSizer6->Add(StaticText27, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtDirection = new wxTextCtrl(this, ID_txtDirection, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtDirection"));
	FlexGridSizer6->Add(txtDirection, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer6->Add(-1,-1,0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("JerkRate:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	FlexGridSizer6->Add(StaticText2, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtJerkRate = new wxTextCtrl(this, ID_txtJerkRate, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtJerkRate"));
	FlexGridSizer6->Add(txtJerkRate, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer6->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText49 = new wxStaticText(this, ID_STATICTEXT49, _("DStopped:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT49"));
	FlexGridSizer6->Add(StaticText49, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtDoorStopped = new wxTextCtrl(this, ID_txtDoorStopped, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtDoorStopped"));
	txtDoorStopped->SetToolTip(_("Door Stopped"));
	FlexGridSizer6->Add(txtDoorStopped, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer6->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText53 = new wxStaticText(this, ID_STATICTEXT53, _("IsIdle:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT53"));
	FlexGridSizer6->Add(StaticText53, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtIsIdle = new wxTextCtrl(this, ID_txtIsIdle, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtIsIdle"));
	FlexGridSizer6->Add(txtIsIdle, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer6->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText24 = new wxStaticText(this, ID_STATICTEXT24, _("Manual:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT24"));
	FlexGridSizer6->Add(StaticText24, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtManualGo = new wxTextCtrl(this, ID_txtManualGo, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtManualGo"));
	FlexGridSizer6->Add(txtManualGo, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer6->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText55 = new wxStaticText(this, ID_STATICTEXT55, _("Leveling:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT55"));
	FlexGridSizer6->Add(StaticText55, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtLeveling = new wxTextCtrl(this, ID_txtLeveling, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtLeveling"));
	FlexGridSizer6->Add(txtLeveling, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer6->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText56 = new wxStaticText(this, ID_STATICTEXT56, _("LevSpeed:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT56"));
	FlexGridSizer6->Add(StaticText56, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtLevelingSpeed = new wxTextCtrl(this, ID_txtLevelingSpeed, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtLevelingSpeed"));
	txtLevelingSpeed->SetToolTip(_("Leveling Speed"));
	FlexGridSizer6->Add(txtLevelingSpeed, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetLevelingSpeed = new wxButton(this, ID_bSetLevelingSpeed, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetLevelingSpeed"));
	FlexGridSizer6->Add(bSetLevelingSpeed, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText57 = new wxStaticText(this, ID_STATICTEXT57, _("LvOffset:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT57"));
	FlexGridSizer6->Add(StaticText57, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtLevelingOffset = new wxTextCtrl(this, ID_txtLevelingOffset, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtLevelingOffset"));
	txtLevelingOffset->SetToolTip(_("Leveling Offset"));
	FlexGridSizer6->Add(txtLevelingOffset, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetLevelingOffset = new wxButton(this, ID_bSetLevelingOffset, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetLevelingOffset"));
	FlexGridSizer6->Add(bSetLevelingOffset, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText58 = new wxStaticText(this, ID_STATICTEXT58, _("LevOpen:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT58"));
	FlexGridSizer6->Add(StaticText58, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtLevelingOpen = new wxTextCtrl(this, ID_txtLevelingOpen, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtLevelingOpen"));
	txtLevelingOpen->SetToolTip(_("Leveling Open"));
	FlexGridSizer6->Add(txtLevelingOpen, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetLevelingOpen = new wxButton(this, ID_bSetLevelingOpen, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetLevelingOpen"));
	FlexGridSizer6->Add(bSetLevelingOpen, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText60 = new wxStaticText(this, ID_STATICTEXT60, _("Parking:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT60"));
	FlexGridSizer6->Add(StaticText60, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtParking = new wxTextCtrl(this, ID_txtParking, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtParking"));
	FlexGridSizer6->Add(txtParking, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer6->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText63 = new wxStaticText(this, ID_STATICTEXT63, _("SlowSpd:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT63"));
	FlexGridSizer6->Add(StaticText63, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtSlowSpeed = new wxTextCtrl(this, ID_txtSlowSpeed, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtSlowSpeed"));
	txtSlowSpeed->SetToolTip(_("Slow Speed Door Multiplier"));
	FlexGridSizer6->Add(txtSlowSpeed, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetSlowSpeed = new wxButton(this, ID_bSetSlowSpeed, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetSlowSpeed"));
	FlexGridSizer6->Add(bSetSlowSpeed, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText64 = new wxStaticText(this, ID_STATICTEXT64, _("ManualSpd:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT64"));
	FlexGridSizer6->Add(StaticText64, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtManualSpeed = new wxTextCtrl(this, ID_txtManualSpeed, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtManualSpeed"));
	txtManualSpeed->SetToolTip(_("Manual Door Speed Multiplier"));
	FlexGridSizer6->Add(txtManualSpeed, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetManualSpeed = new wxButton(this, ID_bSetManualSpeed, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetManualSpeed"));
	FlexGridSizer6->Add(bSetManualSpeed, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer7->Add(FlexGridSizer6, 0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	BoxSizer3->Add(StaticBoxSizer7, 0, wxBOTTOM|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizer7 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer8 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Destination"));
	FlexGridSizer7 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText30 = new wxStaticText(this, ID_STATICTEXT30, _("Floor:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT30"));
	FlexGridSizer7->Add(StaticText30, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtDestFloor = new wxTextCtrl(this, ID_txtDestFloor, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtDestFloor"));
	FlexGridSizer7->Add(txtDestFloor, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer7->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText43 = new wxStaticText(this, ID_STATICTEXT43, _("IsMoving:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT43"));
	FlexGridSizer7->Add(StaticText43, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtIsMoving = new wxTextCtrl(this, ID_txtIsMoving, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtIsMoving"));
	FlexGridSizer7->Add(txtIsMoving, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer7->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText31 = new wxStaticText(this, ID_STATICTEXT31, _("Move:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT31"));
	FlexGridSizer7->Add(StaticText31, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtMoveElevator = new wxTextCtrl(this, ID_txtMoveElevator, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtMoveElevator"));
	FlexGridSizer7->Add(txtMoveElevator, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer7->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText33 = new wxStaticText(this, ID_STATICTEXT33, _("Distance:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT33"));
	FlexGridSizer7->Add(StaticText33, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtDistance = new wxTextCtrl(this, ID_txtDistance, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtDistance"));
	txtDistance->SetToolTip(_("Distance to Destination"));
	FlexGridSizer7->Add(txtDistance, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer7->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText34 = new wxStaticText(this, ID_STATICTEXT34, _("Dest:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT34"));
	FlexGridSizer7->Add(StaticText34, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtDestination = new wxTextCtrl(this, ID_txtDestination, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtDestination"));
	txtDestination->SetToolTip(_("Destination"));
	FlexGridSizer7->Add(txtDestination, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer7->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText35 = new wxStaticText(this, ID_STATICTEXT35, _("StopDist:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT35"));
	FlexGridSizer7->Add(StaticText35, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtStopDistance = new wxTextCtrl(this, ID_txtStopDistance, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtStopDistance"));
	txtStopDistance->SetToolTip(_("Stopping Distance"));
	FlexGridSizer7->Add(txtStopDistance, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer7->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText36 = new wxStaticText(this, ID_STATICTEXT36, _("TmpDecel:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT36"));
	FlexGridSizer7->Add(StaticText36, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtTempDecel = new wxTextCtrl(this, ID_txtTempDecel, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtTempDecel"));
	txtTempDecel->SetToolTip(_("Temp Deceleration"));
	FlexGridSizer7->Add(txtTempDecel, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer7->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText37 = new wxStaticText(this, ID_STATICTEXT37, _("Error:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT37"));
	FlexGridSizer7->Add(StaticText37, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtErrorOffset = new wxTextCtrl(this, ID_txtErrorOffset, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtErrorOffset"));
	txtErrorOffset->SetToolTip(_("Error Offset"));
	FlexGridSizer7->Add(txtErrorOffset, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer7->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText59 = new wxStaticText(this, ID_STATICTEXT59, _("Notify:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT59"));
	FlexGridSizer7->Add(StaticText59, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtNotifyEarly = new wxTextCtrl(this, ID_txtNotifyEarly, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtNotifyEarly"));
	txtNotifyEarly->SetToolTip(_("Notify Early"));
	FlexGridSizer7->Add(txtNotifyEarly, 1, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bNotifyEarly = new wxButton(this, ID_bNotifyEarly, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bNotifyEarly"));
	FlexGridSizer7->Add(bNotifyEarly, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText65 = new wxStaticText(this, ID_STATICTEXT65, _("DDelay:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT65"));
	FlexGridSizer7->Add(StaticText65, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtDepartureDelay = new wxTextCtrl(this, ID_txtDepartureDelay, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtDepartureDelay"));
	txtDepartureDelay->SetToolTip(_("Departure Delay"));
	FlexGridSizer7->Add(txtDepartureDelay, 1, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bSetDepartureDelay = new wxButton(this, ID_bSetDepartureDelay, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetDepartureDelay"));
	FlexGridSizer7->Add(bSetDepartureDelay, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText66 = new wxStaticText(this, ID_STATICTEXT66, _("ADelay:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT66"));
	FlexGridSizer7->Add(StaticText66, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtArrivalDelay = new wxTextCtrl(this, ID_txtArrivalDelay, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtArrivalDelay"));
	txtArrivalDelay->SetToolTip(_("Arrival Delay"));
	FlexGridSizer7->Add(txtArrivalDelay, 1, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bSetArrivalDelay = new wxButton(this, ID_bSetArrivalDelay, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetArrivalDelay"));
	FlexGridSizer7->Add(bSetArrivalDelay, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer8->Add(FlexGridSizer7, 0, wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizer7->Add(StaticBoxSizer8, 0, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticBoxSizer4 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Modes"));
	FlexGridSizer5 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText45 = new wxStaticText(this, ID_STATICTEXT45, _("ACP Floor:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT45"));
	FlexGridSizer5->Add(StaticText45, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtACPFloor = new wxTextCtrl(this, ID_txtACPFloor, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtACPFloor"));
	txtACPFloor->SetToolTip(_("Anti-Crime Prevention Floor"));
	FlexGridSizer5->Add(txtACPFloor, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetACPFloor = new wxButton(this, ID_bSetACPFloor, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetACPFloor"));
	FlexGridSizer5->Add(bSetACPFloor, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText46 = new wxStaticText(this, ID_STATICTEXT46, _("Recall Flr:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT46"));
	FlexGridSizer5->Add(StaticText46, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtRecallFloor = new wxTextCtrl(this, ID_txtRecallFloor, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtRecallFloor"));
	txtRecallFloor->SetToolTip(_("Primary Recall Floor"));
	FlexGridSizer5->Add(txtRecallFloor, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetRecallFloor = new wxButton(this, ID_bSetRecallFloor, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetRecallFloor"));
	FlexGridSizer5->Add(bSetRecallFloor, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText47 = new wxStaticText(this, ID_STATICTEXT47, _("Rcl Flr Alt:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT47"));
	FlexGridSizer5->Add(StaticText47, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtRecallAlternate = new wxTextCtrl(this, ID_txtRecallAlternate, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtRecallAlternate"));
	txtRecallAlternate->SetToolTip(_("Alternate Recall Floor"));
	FlexGridSizer5->Add(txtRecallAlternate, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetRecallAlternate = new wxButton(this, ID_bSetRecallAlternate, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetRecallAlternate"));
	FlexGridSizer5->Add(bSetRecallAlternate, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText39 = new wxStaticText(this, ID_STATICTEXT39, _("Park Flr:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT39"));
	FlexGridSizer5->Add(StaticText39, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtParkingFloor = new wxTextCtrl(this, ID_txtParkingFloor, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtParkingFloor"));
	txtParkingFloor->SetToolTip(_("Parking Floor"));
	FlexGridSizer5->Add(txtParkingFloor, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetParkingFloor = new wxButton(this, ID_bSetParkingFloor, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetParkingFloor"));
	FlexGridSizer5->Add(bSetParkingFloor, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText48 = new wxStaticText(this, ID_STATICTEXT48, _("Park Dly:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT48"));
	FlexGridSizer5->Add(StaticText48, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtParkingDelay = new wxTextCtrl(this, ID_txtParkingDelay, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtParkingDelay"));
	txtParkingDelay->SetToolTip(_("Parking Delay"));
	FlexGridSizer5->Add(txtParkingDelay, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetParkingDelay = new wxButton(this, ID_bSetParkingDelay, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetParkingDelay"));
	FlexGridSizer5->Add(bSetParkingDelay, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText62 = new wxStaticText(this, ID_STATICTEXT62, _("NudgeTimer:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT62"));
	FlexGridSizer5->Add(StaticText62, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtNudgeTimer = new wxTextCtrl(this, ID_txtNudgeTimer, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtNudgeTimer"));
	FlexGridSizer5->Add(txtNudgeTimer, 1, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	bSetNudgeTimer = new wxButton(this, ID_bSetNudgeTimer, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetNudgeTimer"));
	FlexGridSizer5->Add(bSetNudgeTimer, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer4->Add(FlexGridSizer5, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer7->Add(StaticBoxSizer4, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3->Add(BoxSizer7, 1, wxBOTTOM|wxRIGHT|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();
	
	Connect(ID_bACPMode,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bACPMode_Toggle);
	Connect(ID_bUpPeak,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bUpPeak_Toggle);
	Connect(ID_bDownPeak,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bDownPeak_Toggle);
	Connect(ID_bIndService,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bIndService_Toggle);
	Connect(ID_bInsService,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bInsService_Toggle);
	Connect(ID_Fire1Off,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&editelevator::On_Fire1Off_Select);
	Connect(ID_Fire1On,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&editelevator::On_Fire1On_Select);
	Connect(ID_Fire1Bypass,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&editelevator::On_Fire1Bypass_Select);
	Connect(ID_Fire2Off,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&editelevator::On_Fire2Off_Select);
	Connect(ID_Fire2On,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&editelevator::On_Fire2On_Select);
	Connect(ID_Fire2Hold,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&editelevator::On_Fire2Hold_Select);
	Connect(ID_bUp,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bUp_Toggle);
	Connect(ID_bGoToggle,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bGoToggle_Toggle);
	Connect(ID_bDown,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bDown_Toggle);
	Connect(ID_bRefresh,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bRefresh_Click);
	Connect(ID_bDumpFloors,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bDumpFloors_Click);
	Connect(ID_bDumpQueues,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bDumpQueues_Click);
	Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&editelevator::On_chkVisible_Click);
	Connect(ID_chkRun,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&editelevator::On_chkRun_Click);
	Connect(ID_bCall,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bCall_Click);
	Connect(ID_bGo,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bGo_Click);
	Connect(ID_bOpen,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bOpen_Click);
	Connect(ID_bOpenManual,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bOpenManual_Click);
	Connect(ID_bOpenShaftDoor,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bOpenShaftDoor_Click);
	Connect(ID_bStop,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bStop_Click);
	Connect(ID_bChime,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bChime_Click);
	Connect(ID_bUpIndicator,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bUpIndicator_Click);
	Connect(ID_bEnqueueUp,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bEnqueueUp_Click);
	Connect(ID_bEnqueueDown,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bEnqueueDown_Click);
	Connect(ID_bClose,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bClose_Click);
	Connect(ID_bCloseManual,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bCloseManual_Click);
	Connect(ID_bCloseShaftDoor,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bCloseShaftDoor_Click);
	Connect(ID_bAlarm,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bAlarm_Click);
	Connect(ID_bStopDoors,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bStopDoors_Click);
	Connect(ID_bDownIndicator,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bDownIndicator_Click);
	Connect(ID_bSetName,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetName_Click);
	Connect(ID_bSetDoorTimer,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetDoorTimer_Click);
	Connect(ID_bSetQuickClose,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetQuickClose_Click);
	Connect(ID_bSetSkipFloorText,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetSkipFloorText_Click);
	Connect(ID_bResetQueues,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bResetQueues_Click);
	Connect(ID_bSetSpeed,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetSpeed_Click);
	Connect(ID_bSetAcceleration,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetAcceleration_Click);
	Connect(ID_bSetDeceleration,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetDeceleration_Click);
	Connect(ID_bSetAccelJerk,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetAccelJerk_Click);
	Connect(ID_bSetDecelJerk,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetDecelJerk_Click);
	Connect(ID_bSetLevelingSpeed,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetLevelingSpeed_Click);
	Connect(ID_bSetLevelingOffset,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetLevelingOffset_Click);
	Connect(ID_bSetLevelingOpen,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetLevelingOpen_Click);
	Connect(ID_bSetSlowSpeed,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetSlowSpeed_Click);
	Connect(ID_bSetManualSpeed,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetManualSpeed_Click);
	Connect(ID_bNotifyEarly,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bNotifyEarly_Click);
	Connect(ID_bSetDepartureDelay,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetDepartureDelay_Click);
	Connect(ID_bSetArrivalDelay,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetArrivalDelay_Click);
	Connect(ID_bSetACPFloor,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetACPFloor_Click);
	Connect(ID_bSetRecallFloor,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetRecallFloor_Click);
	Connect(ID_bSetRecallAlternate,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetRecallAlternate_Click);
	Connect(ID_bSetParkingFloor,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetParkingFloor_Click);
	Connect(ID_bSetParkingDelay,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetParkingDelay_Click);
	Connect(ID_bSetNudgeTimer,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetNudgeTimer_Click);
	//*)
	OnInit();
}

editelevator::~editelevator()
{
}


void editelevator::On_bCall_Click(wxCommandEvent& event)
{
	//calls elevator to the current camera floor
	if (Simcore->GetElevator(sNumber->GetThumbPosition() + 1)->GetFloor() > Simcore->camera->CurrentFloor)
		Simcore->GetElevator(sNumber->GetThumbPosition() + 1)->AddRoute(Simcore->camera->CurrentFloor, -1, false);
	if (Simcore->GetElevator(sNumber->GetThumbPosition() + 1)->GetFloor() < Simcore->camera->CurrentFloor)
		Simcore->GetElevator(sNumber->GetThumbPosition() + 1)->AddRoute(Simcore->camera->CurrentFloor, 1, false);
}

void editelevator::On_bEnqueueUp_Click(wxCommandEvent& event)
{
	Simcore->GetElevator(sNumber->GetThumbPosition() + 1)->AddRoute(sFloor->GetThumbPosition() - Simcore->Basements, 1, true);
}

void editelevator::On_bGo_Click(wxCommandEvent& event)
{
	Simcore->GetElevator(sNumber->GetThumbPosition() + 1)->Go(sFloor->GetThumbPosition() - Simcore->Basements);
}

void editelevator::On_bEnqueueDown_Click(wxCommandEvent& event)
{
	Simcore->GetElevator(sNumber->GetThumbPosition() + 1)->AddRoute(sFloor->GetThumbPosition() - Simcore->Basements, -1, true);
}

void editelevator::On_bOpen_Click(wxCommandEvent& event)
{
	Simcore->GetElevator(sNumber->GetThumbPosition() + 1)->OpenDoors(sDoor->GetThumbPosition());
}

void editelevator::On_bClose_Click(wxCommandEvent& event)
{
	Simcore->GetElevator(sNumber->GetThumbPosition() + 1)->CloseDoors(sDoor->GetThumbPosition());
}

void editelevator::On_bOpenManual_Click(wxCommandEvent& event)
{
	Simcore->GetElevator(sNumber->GetThumbPosition() + 1)->OpenDoorsEmergency(sDoor->GetThumbPosition());
}

void editelevator::On_bCloseManual_Click(wxCommandEvent& event)
{
	Simcore->GetElevator(sNumber->GetThumbPosition() + 1)->CloseDoorsEmergency(sDoor->GetThumbPosition());
}

void editelevator::On_bStop_Click(wxCommandEvent& event)
{
	Simcore->GetElevator(sNumber->GetThumbPosition() + 1)->Stop(true);
}

void editelevator::On_bAlarm_Click(wxCommandEvent& event)
{
	Simcore->GetElevator(sNumber->GetThumbPosition() + 1)->Alarm();
}

void editelevator::On_bSetName_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->Name = txtName->GetValue().ToAscii();
}

void editelevator::On_bSetSpeed_Click(wxCommandEvent& event)
{
	if (elevator)
		 elevator->ElevatorSpeed = atof(txtSpeed->GetValue().ToAscii());
}

void editelevator::On_bSetAcceleration_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->Acceleration = atof(txtAcceleration->GetValue().ToAscii());
}

void editelevator::On_bSetDeceleration_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->Deceleration = atof(txtDeceleration->GetValue().ToAscii());
}

void editelevator::On_bDumpFloors_Click(wxCommandEvent& event)
{
	Simcore->GetElevator(sNumber->GetThumbPosition() + 1)->DumpServicedFloors();
}

void editelevator::On_bDumpQueues_Click(wxCommandEvent& event)
{
	Simcore->GetElevator(sNumber->GetThumbPosition() + 1)->DumpQueues();
}

void editelevator::OnInit()
{
	last_elevator = 0;

	if (Simcore->Elevators() > 0)
	{
		//set elevator range slider
		sNumber->SetScrollbar(0, 1, Simcore->Elevators(), 1);

		//set floor range slider
		sFloor->SetScrollbar(Simcore->Basements, 1, Simcore->TotalFloors(), 1);

		//set door range slider
		sDoor->SetScrollbar(1, 1, Simcore->GetElevator(sNumber->GetThumbPosition() + 1)->NumDoors + 1, 1);
	}
	else
		sNumber->Enable(false);
}

void editelevator::Loop()
{
	static int last_door;
	static int last_elevator_count;
	static int last_floor_count;
	int elev_num;
	int door_num;
	int elevator_count;
	int floor_count;
	elev_num = sNumber->GetThumbPosition() + 1;
	door_num = sDoor->GetThumbPosition();
	elevator_count = Simcore->Elevators();
	floor_count = Simcore->TotalFloors();
	elevator = Simcore->GetElevator(elev_num);
	door = elevator->GetDoor(door_num);

	if (elev_num != last_elevator)
	{
		//number changed; update values
		last_elevator = elev_num;
		SetMainValues();
	}
	if (door_num != last_door)
	{
		//number changed; update values
		last_door = door_num;
		SetMainValues();
	}
	if (elevator_count != last_elevator_count && elevator_count > 0)
	{
		//elevator count changed; update slider
		last_elevator_count = elevator_count;

		//set elevator range slider
		sNumber->SetScrollbar(0, 1, Simcore->Elevators(), 1);
	}
	if (floor_count != last_floor_count)
	{
		//floor count changed; update slider
		last_floor_count = floor_count;

		//set floor range slider
		sFloor->SetScrollbar(Simcore->Basements, 1, Simcore->TotalFloors(), 1);
	}

	tElevator->SetLabel(wxT("Number " + wxVariant((long)sNumber->GetThumbPosition() + 1).GetString()));
	int floor_temp = sFloor->GetThumbPosition() - Simcore->Basements;
	wxString floor_name;
	if (Simcore->GetFloor(floor_temp))
		floor_name = wxString::FromAscii(Simcore->GetFloor(floor_temp)->ID.c_str());
	tFloor->SetLabel(wxT("Floor ") + wxVariant((long)floor_temp).GetString() + wxT(" (" + floor_name) + wxT(")"));
	tDoor->SetLabel(wxT("Door " + wxVariant((long)sDoor->GetThumbPosition()).GetString() + wxT(" (0 = all)")));
	txtBrakes->SetValue(wxString::FromAscii(BoolToString(elevator->GetBrakeStatus())));
	txtDestFloor->SetValue(wxVariant((long)elevator->GotoFloor).GetString());
	txtDestination->SetValue(TruncateNumber(elevator->GetDestination(), 2));
	txtDirection->SetValue(wxVariant((long)elevator->Direction).GetString());
	txtDistance->SetValue(TruncateNumber(elevator->DistanceToTravel, 2));
	if (door)
	{
		txtDoorDirection->SetValue(wxString::FromAscii(BoolToString(door->DoorDirection)));
		txtDoorHeight->SetValue(TruncateNumber(door->GetDoorWrapper()->Height, 2));
		txtDoorOrigin->SetValue(TruncateNumber(door->GetDoorWrapper()->Origin.x, 2) + wxT(", ") + TruncateNumber(door->GetDoorWrapper()->Origin.y, 2) + wxT(", ") + TruncateNumber(door->GetDoorWrapper()->Origin.z, 2));
		txtDoorsOpen->SetValue(wxString::FromAscii(BoolToString(door->AreDoorsOpen())));
		txtDoorWidth->SetValue(TruncateNumber(door->GetDoorWrapper()->Width, 2));
		txtShaftDoorOrigin->SetValue(TruncateNumber(door->ShaftDoorOrigin.x, 2) + wxT(", ") + TruncateNumber(door->ShaftDoorOrigin.y, 2) + wxT(", ") + TruncateNumber(door->ShaftDoorOrigin.z, 2));
		txtDoorStopped->SetValue(wxString::FromAscii(BoolToString(door->DoorsStopped())));
		txtNudgeMode->SetValue(wxString::FromAscii(BoolToString(door->GetNudgeStatus())));
	}
	txtElevStart->SetValue(TruncateNumber(elevator->GetElevatorStart(), 2));
	txtEnabled->SetValue(wxString::FromAscii(BoolToString(elevator->IsEnabled)));
	txtErrorOffset->SetValue(TruncateNumber(elevator->ErrorOffset, 2));
	txtFloor->SetValue(wxVariant((long)elevator->GetFloor()).GetString());
	txtHeight->SetValue(TruncateNumber(elevator->Height, 2));
	txtMoveElevator->SetValue(wxString::FromAscii(BoolToString(elevator->MoveElevator)));
	txtNumber->SetValue(wxVariant((long)elevator->Number).GetString());
	txtOrigin->SetValue(TruncateNumber(elevator->Origin.x, 2) + wxT(", ") + TruncateNumber(elevator->Origin.y, 2) + wxT(", ") + TruncateNumber(elevator->Origin.z, 2));
	txtOriginFloor->SetValue(wxVariant((long)elevator->OriginFloor).GetString());
	txtPosition->SetValue(TruncateNumber(elevator->GetPosition().x, 2) + wxT(", ") + TruncateNumber(elevator->GetPosition().y, 2) + wxT(", ") + TruncateNumber(elevator->GetPosition().z, 2));
	txtQueueDirection->SetValue(wxVariant((long)elevator->QueuePositionDirection).GetString());
	txtQueueLastDown->SetValue(wxVariant((long)elevator->LastQueueFloor[0]).GetString());
	txtQueueLastUp->SetValue(wxVariant((long)elevator->LastQueueFloor[1]).GetString());
	txtRate->SetValue(TruncateNumber(elevator->ElevatorRate, 2));
	txtShaft->SetValue(wxVariant((long)elevator->AssignedShaft).GetString());
	txtStop->SetValue(wxString::FromAscii(BoolToString(elevator->GetEmergencyStopStatus())));
	txtStopDistance->SetValue(TruncateNumber(elevator->GetStoppingDistance(), 2));
	txtTempDecel->SetValue(TruncateNumber(elevator->TempDeceleration, 2));
	txtJerkRate->SetValue(TruncateNumber(elevator->GetJerkRate(), 4));
	txtIsMoving->SetValue(wxString::FromAscii(BoolToString(elevator->IsMoving)));
	txtUseSkipFloor->SetValue(wxString::FromAscii(BoolToString(elevator->UseFloorSkipText)));
	txtOnFloor->SetValue(wxString::FromAscii(BoolToString(elevator->OnFloor)));
	txtAlarm->SetValue(wxString::FromAscii(BoolToString(elevator->AlarmActive)));
	txtQueueLastDirection->SetValue(wxVariant((long)elevator->LastQueueDirection).GetString());
	txtIsIdle->SetValue(wxString::FromAscii(BoolToString(elevator->IsIdle())));
	txtWaitForDoors->SetValue(wxString::FromAscii(BoolToString(elevator->WaitForDoors)));
	txtMotor->SetValue(TruncateNumber(elevator->MotorPosition.x, 2) + wxT(", ") + TruncateNumber(elevator->MotorPosition.y, 2) + wxT(", ") + TruncateNumber(elevator->MotorPosition.z, 2));
	txtCameraOffset->SetValue(TruncateNumber(elevator->CameraOffset, 2));
	txtManualGo->SetValue(wxString::FromAscii(BoolToString(elevator->ManualGo)));
	txtLeveling->SetValue(wxString::FromAscii(BoolToString(elevator->Leveling)));
	txtParking->SetValue(wxString::FromAscii(BoolToString(elevator->Parking)));
}

void editelevator::SetMainValues()
{
	//set changeable values
	txtName->SetValue(wxString::FromAscii(elevator->Name.c_str()));
	if (door)
	{
		txtDoorTimer->SetValue(wxVariant((int)door->DoorTimer).GetString());
		txtQuickClose->SetValue(wxVariant((int)door->QuickClose).GetString());
		txtNudgeTimer->SetValue(TruncateNumber(door->NudgeTimer, 4));
		txtSlowSpeed->SetValue(TruncateNumber(door->SlowSpeed, 4));
		txtManualSpeed->SetValue(TruncateNumber(door->ManualSpeed, 4));
	}
	txtSpeed->SetValue(TruncateNumber(elevator->ElevatorSpeed, 4));
	txtAcceleration->SetValue(TruncateNumber(elevator->Acceleration, 4));
	txtDeceleration->SetValue(TruncateNumber(elevator->Deceleration, 4));
	txtAccelJerk->SetValue(TruncateNumber(elevator->AccelJerk, 4));
	txtDecelJerk->SetValue(TruncateNumber(elevator->DecelJerk, 4));
	chkVisible->SetValue(elevator->IsEnabled);
	chkRun->SetValue(elevator->IsRunning());
	bACPMode->SetValue(elevator->ACP);
	bUpPeak->SetValue(elevator->UpPeak);
	bDownPeak->SetValue(elevator->DownPeak);
	bIndService->SetValue(elevator->IndependentService);
	bInsService->SetValue(elevator->InspectionService);
	txtSkipFloorText->SetValue(wxString::FromAscii(elevator->GetFloorSkipText()));
	txtACPFloor->SetValue(wxVariant((int)elevator->ACPFloor).GetString());
	txtRecallFloor->SetValue(wxVariant((int)elevator->RecallFloor).GetString());
	txtRecallAlternate->SetValue(wxVariant((int)elevator->RecallFloorAlternate).GetString());
	txtParkingFloor->SetValue(wxVariant((int)elevator->ParkingFloor).GetString());
	txtParkingDelay->SetValue(TruncateNumber(elevator->ParkingDelay, 4));
	txtLevelingSpeed->SetValue(TruncateNumber(elevator->LevelingSpeed, 4));
	txtLevelingOffset->SetValue(TruncateNumber(elevator->LevelingOffset, 4));
	txtLevelingOpen->SetValue(TruncateNumber(elevator->LevelingOpen, 4));
	txtNotifyEarly->SetValue(wxVariant((int)elevator->NotifyEarly).GetString());
	txtDepartureDelay->SetValue(TruncateNumber(elevator->DepartureDelay, 4));
	txtArrivalDelay->SetValue(TruncateNumber(elevator->ArrivalDelay, 4));
	if (elevator->FireServicePhase2 == 0)
		Fire2Off->SetValue(true);
	if (elevator->FireServicePhase2 == 1)
		Fire2On->SetValue(true);
	if (elevator->FireServicePhase2 == 2)
		Fire2Hold->SetValue(true);
	if (elevator->FireServicePhase1 == 0)
		Fire1Off->SetValue(true);
	if (elevator->FireServicePhase1 == 1)
		Fire1On->SetValue(true);
	if (elevator->FireServicePhase1 == 2)
		Fire1Bypass->SetValue(true);
}

void editelevator::On_chkVisible_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->Enabled(chkVisible->GetValue());
}

void editelevator::On_bSetAccelJerk_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->AccelJerk = atof(txtAccelJerk->GetValue().ToAscii());
}

void editelevator::On_bSetDecelJerk_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->DecelJerk = atof(txtDecelJerk->GetValue().ToAscii());
}

void editelevator::On_bOpenShaftDoor_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->OpenDoors(sDoor->GetThumbPosition(), 3, sFloor->GetThumbPosition() - Simcore->Basements);
}

void editelevator::On_bCloseShaftDoor_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->CloseDoors(sDoor->GetThumbPosition(), 3, sFloor->GetThumbPosition() - Simcore->Basements);
}

void editelevator::On_bSetDoorTimer_Click(wxCommandEvent& event)
{
	if (door)
		door->DoorTimer = atoi(txtDoorTimer->GetValue().ToAscii());
}

void editelevator::On_bChime_Click(wxCommandEvent& event)
{
	if (door)
		door->Chime(sFloor->GetThumbPosition() - Simcore->Basements, true);
}

void editelevator::On_bACPMode_Toggle(wxCommandEvent& event)
{
	if (elevator)
	{
		elevator->EnableACP(bACPMode->GetValue());
		SetMainValues();
	}
}

void editelevator::On_bUpPeak_Toggle(wxCommandEvent& event)
{
	if (elevator)
	{
		elevator->EnableUpPeak(bUpPeak->GetValue());
		SetMainValues();
	}
}

void editelevator::On_bDownPeak_Toggle(wxCommandEvent& event)
{
	if (elevator)
	{
		elevator->EnableDownPeak(bDownPeak->GetValue());
		SetMainValues();
	}
}

void editelevator::On_bIndService_Toggle(wxCommandEvent& event)
{
	if (elevator)
	{
		elevator->EnableIndependentService(bIndService->GetValue());
		SetMainValues();
	}
}

void editelevator::On_bInsService_Toggle(wxCommandEvent& event)
{
	if (elevator)
	{
		elevator->EnableInspectionService(bInsService->GetValue());

		if (bInsService->GetValue() == false)
		{
			elevator->SetGoButton(false);
			bGoToggle->SetValue(false);
			bUp->SetValue(false);
			bDown->SetValue(false);
		}
		SetMainValues();
	}
}

void editelevator::On_bRefresh_Click(wxCommandEvent& event)
{
	SetMainValues();
}

void editelevator::On_bSetSkipFloorText_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->SetFloorSkipText(txtSkipFloorText->GetValue().ToAscii());
}

void editelevator::On_bSetACPFloor_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->SetACPFloor(atoi(txtACPFloor->GetValue().ToAscii()));
}

void editelevator::On_bSetRecallFloor_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->SetRecallFloor(atoi(txtRecallFloor->GetValue().ToAscii()));
}

void editelevator::On_bSetRecallAlternate_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->SetAlternateRecallFloor(atoi(txtRecallAlternate->GetValue().ToAscii()));
}

void editelevator::On_Fire1Off_Select(wxCommandEvent& event)
{
	if (elevator)
		elevator->EnableFireService1(0);
}

void editelevator::On_Fire1On_Select(wxCommandEvent& event)
{
	if (elevator)
		elevator->EnableFireService1(1);
}

void editelevator::On_Fire1Bypass_Select(wxCommandEvent& event)
{
	if (elevator)
		elevator->EnableFireService1(2);
}

void editelevator::On_Fire2Off_Select(wxCommandEvent& event)
{
	if (elevator)
		elevator->EnableFireService2(0);
}

void editelevator::On_Fire2On_Select(wxCommandEvent& event)
{
	if (elevator)
		elevator->EnableFireService2(1);
}

void editelevator::On_Fire2Hold_Select(wxCommandEvent& event)
{
	if (elevator)
		elevator->EnableFireService2(2);
}

void editelevator::On_bStopDoors_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->StopDoors(sDoor->GetThumbPosition());
}

void editelevator::On_bUp_Toggle(wxCommandEvent& event)
{
	if (elevator)
	{
		if (elevator->InspectionService == false)
			return;

		if (bUp->GetValue() == true)
			elevator->MoveUp();
		else
			elevator->Stop(false);
	}
}

void editelevator::On_bGoToggle_Toggle(wxCommandEvent& event)
{
	if (elevator)
	{
		if (elevator->InspectionService == false)
			return;

		elevator->SetGoButton(bGoToggle->GetValue());

		if (bUp->GetValue() == true)
			elevator->MoveUp();
		else if (bDown->GetValue() == true)
			elevator->MoveDown();
		else
			elevator->Stop(false);
	}
}

void editelevator::On_bDown_Toggle(wxCommandEvent& event)
{
	if (elevator)
	{
		if (elevator->InspectionService == false)
			return;

		if (bDown->GetValue() == true)
			elevator->MoveDown();
		else
			elevator->Stop(false);
	}
}

void editelevator::On_bUpIndicator_Click(wxCommandEvent& event)
{
	Floor *floor = Simcore->GetFloor(sFloor->GetThumbPosition() - Simcore->Basements);
	if (floor && elevator)
		floor->SetDirectionalIndicators(elevator->Number, true, false);
}

void editelevator::On_bDownIndicator_Click(wxCommandEvent& event)
{
	Floor *floor = Simcore->GetFloor(sFloor->GetThumbPosition() - Simcore->Basements);
	if (floor && elevator)
		floor->SetDirectionalIndicators(elevator->Number, false, true);
}

void editelevator::On_bResetQueues_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->ResetQueue(true, true);
}

void editelevator::On_bSetParkingFloor_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->ParkingFloor = atoi(txtParkingFloor->GetValue().ToAscii());
}

void editelevator::On_bSetParkingDelay_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->ParkingDelay = atof(txtParkingDelay->GetValue().ToAscii());
}

void editelevator::On_bSetLevelingSpeed_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->LevelingSpeed = atof(txtLevelingSpeed->GetValue().ToAscii());
}

void editelevator::On_bSetLevelingOffset_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->LevelingOffset = atof(txtLevelingOffset->GetValue().ToAscii());
}

void editelevator::On_bSetLevelingOpen_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->LevelingOpen = atof(txtLevelingOpen->GetValue().ToAscii());
}

void editelevator::On_bNotifyEarly_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->NotifyEarly = atoi(txtNotifyEarly->GetValue().ToAscii());
}

void editelevator::On_chkRun_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->SetRunState(chkRun->GetValue());
}

void editelevator::On_bSetQuickClose_Click(wxCommandEvent& event)
{
	if (door)
		door->QuickClose = atoi(txtQuickClose->GetValue().ToAscii());
}

void editelevator::On_bSetNudgeTimer_Click(wxCommandEvent& event)
{
	if (door)
		door->NudgeTimer = atof(txtNudgeTimer->GetValue().ToAscii());
}

void editelevator::On_bSetSlowSpeed_Click(wxCommandEvent& event)
{
	if (door)
		door->SlowSpeed = atof(txtSlowSpeed->GetValue().ToAscii());
}

void editelevator::On_bSetManualSpeed_Click(wxCommandEvent& event)
{
	if (door)
		door->ManualSpeed = atof(txtManualSpeed->GetValue().ToAscii());
}

void editelevator::On_bSetDepartureDelay_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->DepartureDelay = atof(txtDepartureDelay->GetValue().ToAscii());
}

void editelevator::On_bSetArrivalDelay_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->ArrivalDelay = atof(txtArrivalDelay->GetValue().ToAscii());
}
