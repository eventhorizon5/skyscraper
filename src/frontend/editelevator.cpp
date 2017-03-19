/* $Id$ */

/*
	Skyscraper 1.11 Alpha - Edit Elevator Form
	Copyright (C)2003-2017 Ryan Thoryk
	http://www.skyscrapersim.com
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@skyscrapersim.com

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
#include <wx/string.h>
#include <wx/intl.h>
//*)
#include "globals.h"
#include "sbs.h"
#include "camera.h"
#include "floor.h"
#include "elevator.h"
#include "elevatordoor.h"
#include "elevatorcar.h"
#include "callbutton.h"
#include "debugpanel.h"
#include "editelevator.h"
#include "skyscraper.h"

using namespace SBS;

namespace Skyscraper {

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
const long editelevator::ID_CHECKBOX1 = wxNewId();
const long editelevator::ID_chkRun = wxNewId();
const long editelevator::ID_bUp = wxNewId();
const long editelevator::ID_bGoToggle = wxNewId();
const long editelevator::ID_bDown = wxNewId();
const long editelevator::ID_tCar = wxNewId();
const long editelevator::ID_sCar = wxNewId();
const long editelevator::ID_tDoor = wxNewId();
const long editelevator::ID_sDoor = wxNewId();
const long editelevator::ID_bRefresh = wxNewId();
const long editelevator::ID_bDumpFloors = wxNewId();
const long editelevator::ID_bDumpQueues = wxNewId();
const long editelevator::ID_bCall = wxNewId();
const long editelevator::ID_bGo = wxNewId();
const long editelevator::ID_bOpen = wxNewId();
const long editelevator::ID_bOpenManual = wxNewId();
const long editelevator::ID_bOpenShaftDoor = wxNewId();
const long editelevator::ID_bStop = wxNewId();
const long editelevator::ID_bChime = wxNewId();
const long editelevator::ID_bEnqueueUp = wxNewId();
const long editelevator::ID_bEnqueueDown = wxNewId();
const long editelevator::ID_bClose = wxNewId();
const long editelevator::ID_bCloseManual = wxNewId();
const long editelevator::ID_bCloseShaftDoor = wxNewId();
const long editelevator::ID_bHoldDoors = wxNewId();
const long editelevator::ID_bStopDoors = wxNewId();
const long editelevator::ID_STATICTEXT3 = wxNewId();
const long editelevator::ID_txtNumber = wxNewId();
const long editelevator::ID_STATICTEXT5 = wxNewId();
const long editelevator::ID_txtName = wxNewId();
const long editelevator::ID_bSetName = wxNewId();
const long editelevator::ID_STATICTEXT9 = wxNewId();
const long editelevator::ID_txtType = wxNewId();
const long editelevator::ID_bSetType = wxNewId();
const long editelevator::ID_STATICTEXT4 = wxNewId();
const long editelevator::ID_txtEnabled = wxNewId();
const long editelevator::ID_STATICTEXT6 = wxNewId();
const long editelevator::ID_txtShaft = wxNewId();
const long editelevator::ID_STATICTEXT7 = wxNewId();
const long editelevator::ID_txtHeight = wxNewId();
const long editelevator::ID_STATICTEXT8 = wxNewId();
const long editelevator::ID_txtDoorSize = wxNewId();
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
const long editelevator::ID_STATICTEXT44 = wxNewId();
const long editelevator::ID_txtSkipFloorText = wxNewId();
const long editelevator::ID_bSetSkipFloorText = wxNewId();
const long editelevator::ID_STATICTEXT52 = wxNewId();
const long editelevator::ID_txtAlarm = wxNewId();
const long editelevator::ID_STATICTEXT28 = wxNewId();
const long editelevator::ID_txtWaitForDoors = wxNewId();
const long editelevator::ID_STATICTEXT67 = wxNewId();
const long editelevator::ID_txtNudgeMode = wxNewId();
const long editelevator::ID_bSetNudge = wxNewId();
const long editelevator::ID_STATICTEXT32 = wxNewId();
const long editelevator::ID_txtDoorSensor = wxNewId();
const long editelevator::ID_bDoorSensor = wxNewId();
const long editelevator::ID_STATICTEXT86 = wxNewId();
const long editelevator::ID_txtActiveCallFloor = wxNewId();
const long editelevator::ID_STATICTEXT87 = wxNewId();
const long editelevator::ID_txtActiveCallDirection = wxNewId();
const long editelevator::ID_STATICTEXT70 = wxNewId();
const long editelevator::ID_txtNotified = wxNewId();
const long editelevator::ID_STATICTEXT75 = wxNewId();
const long editelevator::ID_txtWaitForTimer = wxNewId();
const long editelevator::ID_STATICTEXT76 = wxNewId();
const long editelevator::ID_txtLastChimeDirection = wxNewId();
const long editelevator::ID_STATICTEXT56 = wxNewId();
const long editelevator::ID_txtLevelingSpeed = wxNewId();
const long editelevator::ID_bSetLevelingSpeed = wxNewId();
const long editelevator::ID_STATICTEXT57 = wxNewId();
const long editelevator::ID_txtLevelingOffset = wxNewId();
const long editelevator::ID_bSetLevelingOffset = wxNewId();
const long editelevator::ID_STATICTEXT88 = wxNewId();
const long editelevator::ID_txtLevelingOpen = wxNewId();
const long editelevator::ID_bLevelingOpen = wxNewId();
const long editelevator::ID_STATICTEXT58 = wxNewId();
const long editelevator::ID_txtMusicOn = wxNewId();
const long editelevator::ID_bSetMusicOn = wxNewId();
const long editelevator::ID_STATICTEXT77 = wxNewId();
const long editelevator::ID_txtMusicOnMove = wxNewId();
const long editelevator::ID_bSetMusicOnMove = wxNewId();
const long editelevator::ID_STATICTEXT78 = wxNewId();
const long editelevator::ID_txtFloorSounds = wxNewId();
const long editelevator::ID_bSetFloorSounds = wxNewId();
const long editelevator::ID_STATICTEXT79 = wxNewId();
const long editelevator::ID_txtFloorBeeps = wxNewId();
const long editelevator::ID_bSetFloorBeeps = wxNewId();
const long editelevator::ID_STATICTEXT80 = wxNewId();
const long editelevator::ID_txtMessageSounds = wxNewId();
const long editelevator::ID_bSetMessageSounds = wxNewId();
const long editelevator::ID_STATICTEXT81 = wxNewId();
const long editelevator::ID_txtAutoEnable = wxNewId();
const long editelevator::ID_bSetAutoEnable = wxNewId();
const long editelevator::ID_STATICTEXT82 = wxNewId();
const long editelevator::ID_txtReOpen = wxNewId();
const long editelevator::ID_bSetReOpen = wxNewId();
const long editelevator::ID_STATICTEXT83 = wxNewId();
const long editelevator::ID_txtAutoDoors = wxNewId();
const long editelevator::ID_bSetAutoDoors = wxNewId();
const long editelevator::ID_STATICTEXT84 = wxNewId();
const long editelevator::ID_txtOpenOnStart = wxNewId();
const long editelevator::ID_STATICTEXT85 = wxNewId();
const long editelevator::ID_txtInterlocks = wxNewId();
const long editelevator::ID_bInterlocks = wxNewId();
const long editelevator::ID_STATICTEXT14 = wxNewId();
const long editelevator::ID_txtFloor = wxNewId();
const long editelevator::ID_STATICTEXT15 = wxNewId();
const long editelevator::ID_txtPosition = wxNewId();
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
const long editelevator::ID_STATICTEXT71 = wxNewId();
const long editelevator::ID_txtMusicPosition = wxNewId();
const long editelevator::ID_STATICTEXT38 = wxNewId();
const long editelevator::ID_txtQueueDirection = wxNewId();
const long editelevator::ID_STATICTEXT40 = wxNewId();
const long editelevator::ID_txtQueueLastUp = wxNewId();
const long editelevator::ID_STATICTEXT41 = wxNewId();
const long editelevator::ID_txtQueueLastDown = wxNewId();
const long editelevator::ID_STATICTEXT51 = wxNewId();
const long editelevator::ID_txtQueueLastDirection = wxNewId();
const long editelevator::ID_STATICTEXT68 = wxNewId();
const long editelevator::ID_txtQueueResets = wxNewId();
const long editelevator::ID_STATICTEXT69 = wxNewId();
const long editelevator::ID_txtLimitQueue = wxNewId();
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
const long editelevator::ID_STATICTEXT72 = wxNewId();
const long editelevator::ID_txtActiveDirection = wxNewId();
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
const long editelevator::ID_STATICTEXT60 = wxNewId();
const long editelevator::ID_txtParking = wxNewId();
const long editelevator::ID_STATICTEXT73 = wxNewId();
const long editelevator::ID_txtManualMove = wxNewId();
const long editelevator::ID_STATICTEXT63 = wxNewId();
const long editelevator::ID_txtSlowSpeed = wxNewId();
const long editelevator::ID_bSetSlowSpeed = wxNewId();
const long editelevator::ID_STATICTEXT64 = wxNewId();
const long editelevator::ID_txtManualSpeed = wxNewId();
const long editelevator::ID_bSetManualSpeed = wxNewId();
const long editelevator::ID_STATICTEXT74 = wxNewId();
const long editelevator::ID_txtInspectionSpeed = wxNewId();
const long editelevator::ID_bSetInspectionSpeed = wxNewId();
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

editelevator::editelevator(DebugPanel* parent,wxWindowID id)
{
	//(*Initialize(editelevator)
	wxFlexGridSizer* FlexGridSizer8;
	wxStaticBoxSizer* StaticBoxSizer10;
	wxFlexGridSizer* FlexGridSizer11;
	wxBoxSizer* BoxSizer7;
	wxBoxSizer* BoxSizer11;
	wxFlexGridSizer* FlexGridSizer4;
	wxFlexGridSizer* FlexGridSizer9;
	wxStaticBoxSizer* StaticBoxSizer4;
	wxStaticBoxSizer* StaticBoxSizer9;
	wxStaticBoxSizer* StaticBoxSizer6;
	wxFlexGridSizer* FlexGridSizer10;
	wxBoxSizer* BoxSizer12;
	wxFlexGridSizer* FlexGridSizer5;
	wxBoxSizer* BoxSizer5;

	Create(parent, wxID_ANY, _("Elevator Editor"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxMINIMIZE_BOX, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer6 = new wxBoxSizer(wxVERTICAL);
	tElevator = new wxStaticText(this, ID_tElevator, _("Elevator"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_tElevator"));
	BoxSizer6->Add(tElevator, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	sNumber = new wxScrollBar(this, ID_sNumber, wxDefaultPosition, wxDefaultSize, wxSB_HORIZONTAL, wxDefaultValidator, _T("ID_sNumber"));
	sNumber->SetScrollbar(0, 0, 0, 0);
	BoxSizer6->Add(sNumber, 0, wxALL|wxEXPAND, 0);
	tFloor = new wxStaticText(this, ID_tFloor, _("Floor"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_tFloor"));
	BoxSizer6->Add(tFloor, 0, wxTOP|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 10);
	sFloor = new wxScrollBar(this, ID_sFloor, wxDefaultPosition, wxDefaultSize, wxSB_HORIZONTAL, wxDefaultValidator, _T("ID_sFloor"));
	sFloor->SetScrollbar(0, 0, 0, 0);
	BoxSizer6->Add(sFloor, 0, wxALL|wxEXPAND, 0);
	FlexGridSizer4 = new wxFlexGridSizer(0, 4, 0, 5);
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
	BoxSizer5->Add(bInsService, 1, wxEXPAND, 5);
	FlexGridSizer4->Add(BoxSizer5, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer10 = new wxFlexGridSizer(2, 1, 0, 0);
	FlexGridSizer11 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticBoxSizer10 = new wxStaticBoxSizer(wxVERTICAL, this, _("Fire Ph 1"));
	Fire1Off = new wxRadioButton(this, ID_Fire1Off, _("Off"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator, _T("ID_Fire1Off"));
	Fire1Off->SetValue(true);
	StaticBoxSizer10->Add(Fire1Off, 1, wxALL|wxALIGN_LEFT, 5);
	Fire1On = new wxRadioButton(this, ID_Fire1On, _("On"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_Fire1On"));
	StaticBoxSizer10->Add(Fire1On, 1, wxALL|wxALIGN_LEFT, 5);
	Fire1Bypass = new wxRadioButton(this, ID_Fire1Bypass, _("Bypass"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_Fire1Bypass"));
	StaticBoxSizer10->Add(Fire1Bypass, 1, wxALL|wxALIGN_LEFT, 5);
	FlexGridSizer11->Add(StaticBoxSizer10, 1, wxTOP|wxBOTTOM|wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer9 = new wxStaticBoxSizer(wxVERTICAL, this, _("Fire Ph 2"));
	Fire2Off = new wxRadioButton(this, ID_Fire2Off, _("Off"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator, _T("ID_Fire2Off"));
	StaticBoxSizer9->Add(Fire2Off, 1, wxALL|wxALIGN_LEFT, 5);
	Fire2On = new wxRadioButton(this, ID_Fire2On, _("On"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_Fire2On"));
	StaticBoxSizer9->Add(Fire2On, 1, wxALL|wxALIGN_LEFT, 5);
	Fire2Hold = new wxRadioButton(this, ID_Fire2Hold, _("Hold"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_Fire2Hold"));
	StaticBoxSizer9->Add(Fire2Hold, 1, wxALL|wxALIGN_LEFT, 5);
	FlexGridSizer11->Add(StaticBoxSizer9, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer10->Add(FlexGridSizer11, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
	chkVisible = new wxCheckBox(this, ID_CHECKBOX1, _("Visible"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	chkVisible->SetValue(false);
	BoxSizer12->Add(chkVisible, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	chkRun = new wxCheckBox(this, ID_chkRun, _("Run"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkRun"));
	chkRun->SetValue(false);
	BoxSizer12->Add(chkRun, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer10->Add(BoxSizer12, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer4->Add(FlexGridSizer10, 1, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer6 = new wxStaticBoxSizer(wxVERTICAL, this, _("Manual"));
	bUp = new wxToggleButton(this, ID_bUp, _("Up"), wxDefaultPosition, wxSize(50,0), 0, wxDefaultValidator, _T("ID_bUp"));
	StaticBoxSizer6->Add(bUp, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bGoToggle = new wxToggleButton(this, ID_bGoToggle, _("Go"), wxDefaultPosition, wxSize(50,-1), 0, wxDefaultValidator, _T("ID_bGoToggle"));
	StaticBoxSizer6->Add(bGoToggle, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bDown = new wxToggleButton(this, ID_bDown, _("Down"), wxDefaultPosition, wxSize(50,0), 0, wxDefaultValidator, _T("ID_bDown"));
	StaticBoxSizer6->Add(bDown, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer4->Add(StaticBoxSizer6, 1, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer8 = new wxFlexGridSizer(0, 1, 0, 0);
	tCar = new wxStaticText(this, ID_tCar, _("Car"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_tCar"));
	FlexGridSizer8->Add(tCar, 1, wxEXPAND, 5);
	sCar = new wxScrollBar(this, ID_sCar, wxDefaultPosition, wxDefaultSize, wxSB_HORIZONTAL, wxDefaultValidator, _T("ID_sCar"));
	sCar->SetScrollbar(0, 0, 0, 0);
	sCar->SetMinSize(wxSize(125,-1));
	FlexGridSizer8->Add(sCar, 1, wxBOTTOM|wxEXPAND, 5);
	tDoor = new wxStaticText(this, ID_tDoor, _("Door"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_tDoor"));
	FlexGridSizer8->Add(tDoor, 1, wxEXPAND, 5);
	sDoor = new wxScrollBar(this, ID_sDoor, wxDefaultPosition, wxDefaultSize, wxSB_HORIZONTAL, wxDefaultValidator, _T("ID_sDoor"));
	sDoor->SetScrollbar(0, 0, 0, 0);
	FlexGridSizer8->Add(sDoor, 1, wxBOTTOM|wxEXPAND, 5);
	bRefresh = new wxButton(this, ID_bRefresh, _("Refresh Values"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bRefresh"));
	FlexGridSizer8->Add(bRefresh, 1, wxEXPAND, 5);
	bDumpFloors = new wxButton(this, ID_bDumpFloors, _("Dump Floor List"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bDumpFloors"));
	FlexGridSizer8->Add(bDumpFloors, 1, wxEXPAND, 5);
	bDumpQueues = new wxButton(this, ID_bDumpQueues, _("Dump Queues"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bDumpQueues"));
	FlexGridSizer8->Add(bDumpQueues, 1, wxEXPAND, 5);
	FlexGridSizer4->Add(FlexGridSizer8, 1, wxLEFT, 5);
	BoxSizer6->Add(FlexGridSizer4, 1, wxTOP|wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(BoxSizer6, 2, wxTOP|wxLEFT|wxRIGHT|wxALIGN_TOP, 5);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Control"));
	BoxSizer8 = new wxBoxSizer(wxVERTICAL);
	bCall = new wxButton(this, ID_bCall, _("Call"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bCall"));
	bCall->SetFocus();
	BoxSizer8->Add(bCall, 0, wxEXPAND, 0);
	bGo = new wxButton(this, ID_bGo, _("Go"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bGo"));
	BoxSizer8->Add(bGo, 0, wxEXPAND, 0);
	bOpen = new wxButton(this, ID_bOpen, _("Open"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bOpen"));
	BoxSizer8->Add(bOpen, 0, wxEXPAND, 0);
	bOpenManual = new wxButton(this, ID_bOpenManual, _("Open Manual"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bOpenManual"));
	BoxSizer8->Add(bOpenManual, 0, wxEXPAND, 0);
	bOpenShaftDoor = new wxButton(this, ID_bOpenShaftDoor, _("Open Shaft Doors"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bOpenShaftDoor"));
	BoxSizer8->Add(bOpenShaftDoor, 1, wxEXPAND, 5);
	bStop = new wxButton(this, ID_bStop, _("Emergency Stop"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bStop"));
	BoxSizer8->Add(bStop, 0, wxEXPAND, 0);
	bChime = new wxButton(this, ID_bChime, _("Chime"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bChime"));
	BoxSizer8->Add(bChime, 1, wxEXPAND, 5);
	StaticBoxSizer1->Add(BoxSizer8, 1, wxALL|wxALIGN_TOP, 0);
	BoxSizer9 = new wxBoxSizer(wxVERTICAL);
	bEnqueueUp = new wxButton(this, ID_bEnqueueUp, _("Enqueue Up"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bEnqueueUp"));
	BoxSizer9->Add(bEnqueueUp, 0, wxEXPAND, 0);
	bEnqueueDown = new wxButton(this, ID_bEnqueueDown, _("Enqueue Down"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bEnqueueDown"));
	BoxSizer9->Add(bEnqueueDown, 0, wxEXPAND, 0);
	bClose = new wxButton(this, ID_bClose, _("Close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bClose"));
	BoxSizer9->Add(bClose, 0, wxEXPAND, 0);
	bCloseManual = new wxButton(this, ID_bCloseManual, _("Close Manual"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bCloseManual"));
	BoxSizer9->Add(bCloseManual, 0, wxEXPAND, 0);
	bCloseShaftDoor = new wxButton(this, ID_bCloseShaftDoor, _("Close Shaft Doors"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bCloseShaftDoor"));
	BoxSizer9->Add(bCloseShaftDoor, 1, wxEXPAND, 5);
	bHoldDoors = new wxButton(this, ID_bHoldDoors, _("Hold Doors"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bHoldDoors"));
	BoxSizer9->Add(bHoldDoors, 0, wxEXPAND, 0);
	bStopDoors = new wxButton(this, ID_bStopDoors, _("Stop Doors"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bStopDoors"));
	BoxSizer9->Add(bStopDoors, 1, wxEXPAND, 5);
	StaticBoxSizer1->Add(BoxSizer9, 1, wxALL|wxALIGN_TOP, 0);
	BoxSizer2->Add(StaticBoxSizer1, 1, wxRIGHT|wxALIGN_TOP, 5);
	BoxSizer1->Add(BoxSizer2, 0, wxEXPAND, 0);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("General"));
	BoxSizer11 = new wxBoxSizer(wxHORIZONTAL);
	FlexGridSizer1 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Number:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	FlexGridSizer1->Add(StaticText3, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtNumber = new wxTextCtrl(this, ID_txtNumber, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtNumber"));
	txtNumber->SetToolTip(_("Number"));
	FlexGridSizer1->Add(txtNumber, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer1->Add(-1,-1,0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Name:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	FlexGridSizer1->Add(StaticText5, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtName = new wxTextCtrl(this, ID_txtName, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtName"));
	txtName->SetToolTip(_("Name"));
	FlexGridSizer1->Add(txtName, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bSetName = new wxButton(this, ID_bSetName, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetName"));
	FlexGridSizer1->Add(bSetName, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 0);
	StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _("Type:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	FlexGridSizer1->Add(StaticText9, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtType = new wxTextCtrl(this, ID_txtType, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtType"));
	txtType->SetToolTip(_("Type"));
	FlexGridSizer1->Add(txtType, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bSetType = new wxButton(this, ID_bSetType, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetType"));
	FlexGridSizer1->Add(bSetType, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 0);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Enabled:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	FlexGridSizer1->Add(StaticText4, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtEnabled = new wxTextCtrl(this, ID_txtEnabled, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtEnabled"));
	txtEnabled->SetToolTip(_("Enabled Status"));
	FlexGridSizer1->Add(txtEnabled, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer1->Add(-1,-1,0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("Shaft:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	FlexGridSizer1->Add(StaticText6, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtShaft = new wxTextCtrl(this, ID_txtShaft, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtShaft"));
	txtShaft->SetToolTip(_("Shaft Number"));
	FlexGridSizer1->Add(txtShaft, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer1->Add(-1,-1,0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Height:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	FlexGridSizer1->Add(StaticText7, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtHeight = new wxTextCtrl(this, ID_txtHeight, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtHeight"));
	txtHeight->SetToolTip(_("Height"));
	FlexGridSizer1->Add(txtHeight, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer1->Add(-1,-1,0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("DoorSize:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	FlexGridSizer1->Add(StaticText8, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtDoorSize = new wxTextCtrl(this, ID_txtDoorSize, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtDoorSize"));
	txtDoorSize->SetToolTip(_("Door Width"));
	FlexGridSizer1->Add(txtDoorSize, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
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
	txtDoorsOpen->SetToolTip(_("Doors Open"));
	FlexGridSizer1->Add(txtDoorsOpen, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer1->Add(-1,-1,0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	StaticText12 = new wxStaticText(this, ID_STATICTEXT12, _("Brakes:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT12"));
	FlexGridSizer1->Add(StaticText12, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtBrakes = new wxTextCtrl(this, ID_txtBrakes, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtBrakes"));
	txtBrakes->SetToolTip(_("Brakes"));
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
	txtDoorTimer->SetToolTip(_("Door Timer"));
	FlexGridSizer1->Add(txtDoorTimer, 1, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	bSetDoorTimer = new wxButton(this, ID_bSetDoorTimer, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetDoorTimer"));
	FlexGridSizer1->Add(bSetDoorTimer, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText61 = new wxStaticText(this, ID_STATICTEXT61, _("QuickClose:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT61"));
	FlexGridSizer1->Add(StaticText61, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtQuickClose = new wxTextCtrl(this, ID_txtQuickClose, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtQuickClose"));
	txtQuickClose->SetToolTip(_("Quick Close"));
	FlexGridSizer1->Add(txtQuickClose, 1, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	bSetQuickClose = new wxButton(this, ID_bSetQuickClose, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetQuickClose"));
	FlexGridSizer1->Add(bSetQuickClose, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText44 = new wxStaticText(this, ID_STATICTEXT44, _("FlrSkipTxt:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT44"));
	FlexGridSizer1->Add(StaticText44, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtSkipFloorText = new wxTextCtrl(this, ID_txtSkipFloorText, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtSkipFloorText"));
	txtSkipFloorText->SetToolTip(_("Floor Skip Text"));
	FlexGridSizer1->Add(txtSkipFloorText, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetSkipFloorText = new wxButton(this, ID_bSetSkipFloorText, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetSkipFloorText"));
	FlexGridSizer1->Add(bSetSkipFloorText, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText52 = new wxStaticText(this, ID_STATICTEXT52, _("Alarm On:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT52"));
	FlexGridSizer1->Add(StaticText52, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtAlarm = new wxTextCtrl(this, ID_txtAlarm, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtAlarm"));
	txtAlarm->SetToolTip(_("Alarm On"));
	FlexGridSizer1->Add(txtAlarm, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText28 = new wxStaticText(this, ID_STATICTEXT28, _("WaitDoors:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT28"));
	FlexGridSizer1->Add(StaticText28, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtWaitForDoors = new wxTextCtrl(this, ID_txtWaitForDoors, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtWaitForDoors"));
	txtWaitForDoors->SetToolTip(_("Wait for Doors"));
	FlexGridSizer1->Add(txtWaitForDoors, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText67 = new wxStaticText(this, ID_STATICTEXT67, _("Nudge:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT67"));
	FlexGridSizer1->Add(StaticText67, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtNudgeMode = new wxTextCtrl(this, ID_txtNudgeMode, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtNudgeMode"));
	txtNudgeMode->SetToolTip(_("Nudge Mode"));
	FlexGridSizer1->Add(txtNudgeMode, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetNudge = new wxButton(this, ID_bSetNudge, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetNudge"));
	FlexGridSizer1->Add(bSetNudge, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText32 = new wxStaticText(this, ID_STATICTEXT32, _("Sensor:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT32"));
	FlexGridSizer1->Add(StaticText32, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtDoorSensor = new wxTextCtrl(this, ID_txtDoorSensor, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtDoorSensor"));
	txtDoorSensor->SetToolTip(_("Door Sensor Status"));
	FlexGridSizer1->Add(txtDoorSensor, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bDoorSensor = new wxButton(this, ID_bDoorSensor, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bDoorSensor"));
	FlexGridSizer1->Add(bDoorSensor, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer11->Add(FlexGridSizer1, 0, wxALL|wxALIGN_TOP, 0);
	FlexGridSizer9 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText86 = new wxStaticText(this, ID_STATICTEXT86, _("ActCallFloor:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT86"));
	FlexGridSizer9->Add(StaticText86, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtActiveCallFloor = new wxTextCtrl(this, ID_txtActiveCallFloor, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtActiveCallFloor"));
	txtActiveCallFloor->SetToolTip(_("Active Call Floor"));
	FlexGridSizer9->Add(txtActiveCallFloor, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer9->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText87 = new wxStaticText(this, ID_STATICTEXT87, _("ActCallDir:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT87"));
	FlexGridSizer9->Add(StaticText87, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtActiveCallDirection = new wxTextCtrl(this, ID_txtActiveCallDirection, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtActiveCallDirection"));
	txtActiveCallDirection->SetToolTip(_("Active Call Direction and Type"));
	FlexGridSizer9->Add(txtActiveCallDirection, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer9->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText70 = new wxStaticText(this, ID_STATICTEXT70, _("Notified:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT70"));
	FlexGridSizer9->Add(StaticText70, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtNotified = new wxTextCtrl(this, ID_txtNotified, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtNotified"));
	txtNotified->SetToolTip(_("Notified"));
	FlexGridSizer9->Add(txtNotified, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer9->Add(-1,-1,1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	StaticText75 = new wxStaticText(this, ID_STATICTEXT75, _("WaitTimer:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT75"));
	FlexGridSizer9->Add(StaticText75, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtWaitForTimer = new wxTextCtrl(this, ID_txtWaitForTimer, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtWaitForTimer"));
	txtWaitForTimer->SetToolTip(_("WaitForTimer"));
	FlexGridSizer9->Add(txtWaitForTimer, 1, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer9->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText76 = new wxStaticText(this, ID_STATICTEXT76, _("ChimeDir:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT76"));
	FlexGridSizer9->Add(StaticText76, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtLastChimeDirection = new wxTextCtrl(this, ID_txtLastChimeDirection, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtLastChimeDirection"));
	txtLastChimeDirection->SetToolTip(_("Last Chime Direction"));
	FlexGridSizer9->Add(txtLastChimeDirection, 1, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer9->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText56 = new wxStaticText(this, ID_STATICTEXT56, _("LevSpeed:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT56"));
	FlexGridSizer9->Add(StaticText56, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtLevelingSpeed = new wxTextCtrl(this, ID_txtLevelingSpeed, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtLevelingSpeed"));
	txtLevelingSpeed->SetToolTip(_("Leveling Speed"));
	FlexGridSizer9->Add(txtLevelingSpeed, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetLevelingSpeed = new wxButton(this, ID_bSetLevelingSpeed, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetLevelingSpeed"));
	FlexGridSizer9->Add(bSetLevelingSpeed, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText57 = new wxStaticText(this, ID_STATICTEXT57, _("LevOffset:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT57"));
	FlexGridSizer9->Add(StaticText57, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtLevelingOffset = new wxTextCtrl(this, ID_txtLevelingOffset, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtLevelingOffset"));
	txtLevelingOffset->SetToolTip(_("Leveling Offset"));
	FlexGridSizer9->Add(txtLevelingOffset, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetLevelingOffset = new wxButton(this, ID_bSetLevelingOffset, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetLevelingOffset"));
	FlexGridSizer9->Add(bSetLevelingOffset, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText88 = new wxStaticText(this, ID_STATICTEXT88, _("LevOpen:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT88"));
	FlexGridSizer9->Add(StaticText88, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtLevelingOpen = new wxTextCtrl(this, ID_txtLevelingOpen, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtLevelingOpen"));
	txtLevelingOpen->SetToolTip(_("Leveling Open Offset"));
	FlexGridSizer9->Add(txtLevelingOpen, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bLevelingOpen = new wxButton(this, ID_bLevelingOpen, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bLevelingOpen"));
	FlexGridSizer9->Add(bLevelingOpen, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText58 = new wxStaticText(this, ID_STATICTEXT58, _("MusicOn:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT58"));
	FlexGridSizer9->Add(StaticText58, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtMusicOn = new wxTextCtrl(this, ID_txtMusicOn, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtMusicOn"));
	txtMusicOn->SetToolTip(_("Music On"));
	FlexGridSizer9->Add(txtMusicOn, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetMusicOn = new wxButton(this, ID_bSetMusicOn, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetMusicOn"));
	FlexGridSizer9->Add(bSetMusicOn, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText77 = new wxStaticText(this, ID_STATICTEXT77, _("MusicMove:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT77"));
	FlexGridSizer9->Add(StaticText77, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtMusicOnMove = new wxTextCtrl(this, ID_txtMusicOnMove, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtMusicOnMove"));
	txtMusicOnMove->SetToolTip(_("MusicOnMove"));
	FlexGridSizer9->Add(txtMusicOnMove, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetMusicOnMove = new wxButton(this, ID_bSetMusicOnMove, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetMusicOnMove"));
	FlexGridSizer9->Add(bSetMusicOnMove, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText78 = new wxStaticText(this, ID_STATICTEXT78, _("FlrSounds:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT78"));
	FlexGridSizer9->Add(StaticText78, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtFloorSounds = new wxTextCtrl(this, ID_txtFloorSounds, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtFloorSounds"));
	txtFloorSounds->SetToolTip(_("Floor Sounds"));
	FlexGridSizer9->Add(txtFloorSounds, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetFloorSounds = new wxButton(this, ID_bSetFloorSounds, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetFloorSounds"));
	FlexGridSizer9->Add(bSetFloorSounds, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText79 = new wxStaticText(this, ID_STATICTEXT79, _("FlrBeeps:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT79"));
	FlexGridSizer9->Add(StaticText79, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtFloorBeeps = new wxTextCtrl(this, ID_txtFloorBeeps, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtFloorBeeps"));
	txtFloorBeeps->SetToolTip(_("Floor Beeps"));
	FlexGridSizer9->Add(txtFloorBeeps, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetFloorBeeps = new wxButton(this, ID_bSetFloorBeeps, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetFloorBeeps"));
	FlexGridSizer9->Add(bSetFloorBeeps, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText80 = new wxStaticText(this, ID_STATICTEXT80, _("MsgSounds:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT80"));
	FlexGridSizer9->Add(StaticText80, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtMessageSounds = new wxTextCtrl(this, ID_txtMessageSounds, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtMessageSounds"));
	txtMessageSounds->SetToolTip(_("Message Sounds"));
	FlexGridSizer9->Add(txtMessageSounds, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetMessageSounds = new wxButton(this, ID_bSetMessageSounds, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetMessageSounds"));
	FlexGridSizer9->Add(bSetMessageSounds, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText81 = new wxStaticText(this, ID_STATICTEXT81, _("AutoEnable:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT81"));
	FlexGridSizer9->Add(StaticText81, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtAutoEnable = new wxTextCtrl(this, ID_txtAutoEnable, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtAutoEnable"));
	txtAutoEnable->SetToolTip(_("Auto Enable"));
	FlexGridSizer9->Add(txtAutoEnable, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetAutoEnable = new wxButton(this, ID_bSetAutoEnable, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetAutoEnable"));
	FlexGridSizer9->Add(bSetAutoEnable, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText82 = new wxStaticText(this, ID_STATICTEXT82, _("ReOpen:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT82"));
	FlexGridSizer9->Add(StaticText82, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtReOpen = new wxTextCtrl(this, ID_txtReOpen, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtReOpen"));
	txtReOpen->SetToolTip(_("ReOpen"));
	FlexGridSizer9->Add(txtReOpen, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetReOpen = new wxButton(this, ID_bSetReOpen, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetReOpen"));
	FlexGridSizer9->Add(bSetReOpen, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText83 = new wxStaticText(this, ID_STATICTEXT83, _("AutoDoors:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT83"));
	FlexGridSizer9->Add(StaticText83, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtAutoDoors = new wxTextCtrl(this, ID_txtAutoDoors, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtAutoDoors"));
	txtAutoDoors->SetToolTip(_("Auto Doors"));
	FlexGridSizer9->Add(txtAutoDoors, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetAutoDoors = new wxButton(this, ID_bSetAutoDoors, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetAutoDoors"));
	FlexGridSizer9->Add(bSetAutoDoors, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText84 = new wxStaticText(this, ID_STATICTEXT84, _("OpenStart:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT84"));
	FlexGridSizer9->Add(StaticText84, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtOpenOnStart = new wxTextCtrl(this, ID_txtOpenOnStart, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtOpenOnStart"));
	txtOpenOnStart->SetToolTip(_("Open On Start"));
	FlexGridSizer9->Add(txtOpenOnStart, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer9->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText85 = new wxStaticText(this, ID_STATICTEXT85, _("Interlocks:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT85"));
	FlexGridSizer9->Add(StaticText85, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtInterlocks = new wxTextCtrl(this, ID_txtInterlocks, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtInterlocks"));
	txtInterlocks->SetToolTip(_("Interlocks"));
	FlexGridSizer9->Add(txtInterlocks, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bInterlocks = new wxButton(this, ID_bInterlocks, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bInterlocks"));
	FlexGridSizer9->Add(bInterlocks, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer11->Add(FlexGridSizer9, 0, wxLEFT|wxALIGN_TOP, 5);
	StaticBoxSizer2->Add(BoxSizer11, 1, wxALL|wxALIGN_TOP, 0);
	BoxSizer3->Add(StaticBoxSizer2, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_TOP, 5);
	BoxSizer10 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer5 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Position"));
	FlexGridSizer2 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticText14 = new wxStaticText(this, ID_STATICTEXT14, _("Floor:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT14"));
	FlexGridSizer2->Add(StaticText14, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtFloor = new wxTextCtrl(this, ID_txtFloor, wxEmptyString, wxDefaultPosition, wxSize(100,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtFloor"));
	txtFloor->SetToolTip(_("Floor Number"));
	FlexGridSizer2->Add(txtFloor, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText15 = new wxStaticText(this, ID_STATICTEXT15, _("Pos:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT15"));
	FlexGridSizer2->Add(StaticText15, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtPosition = new wxTextCtrl(this, ID_txtPosition, wxEmptyString, wxDefaultPosition, wxSize(100,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtPosition"));
	txtPosition->SetToolTip(_("Position"));
	FlexGridSizer2->Add(txtPosition, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText17 = new wxStaticText(this, ID_STATICTEXT17, _("StartFlr:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT17"));
	FlexGridSizer2->Add(StaticText17, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtOriginFloor = new wxTextCtrl(this, ID_txtOriginFloor, wxEmptyString, wxDefaultPosition, wxSize(100,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtOriginFloor"));
	txtOriginFloor->SetToolTip(_("Starting Floor"));
	FlexGridSizer2->Add(txtOriginFloor, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText18 = new wxStaticText(this, ID_STATICTEXT18, _("EStart:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT18"));
	FlexGridSizer2->Add(StaticText18, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtElevStart = new wxTextCtrl(this, ID_txtElevStart, wxEmptyString, wxDefaultPosition, wxSize(100,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtElevStart"));
	txtElevStart->SetToolTip(_("Elevator Start"));
	FlexGridSizer2->Add(txtElevStart, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText19 = new wxStaticText(this, ID_STATICTEXT19, _("DOrigin:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT19"));
	FlexGridSizer2->Add(StaticText19, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtDoorOrigin = new wxTextCtrl(this, ID_txtDoorOrigin, wxEmptyString, wxDefaultPosition, wxSize(100,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtDoorOrigin"));
	txtDoorOrigin->SetToolTip(_("Door Origin"));
	FlexGridSizer2->Add(txtDoorOrigin, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText20 = new wxStaticText(this, ID_STATICTEXT20, _("SDOrigin:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT20"));
	FlexGridSizer2->Add(StaticText20, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtShaftDoorOrigin = new wxTextCtrl(this, ID_txtShaftDoorOrigin, wxEmptyString, wxDefaultPosition, wxSize(100,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtShaftDoorOrigin"));
	txtShaftDoorOrigin->SetToolTip(_("Shaft Door Origin"));
	FlexGridSizer2->Add(txtShaftDoorOrigin, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText50 = new wxStaticText(this, ID_STATICTEXT50, _("OnFloor:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT50"));
	FlexGridSizer2->Add(StaticText50, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtOnFloor = new wxTextCtrl(this, ID_txtOnFloor, wxEmptyString, wxDefaultPosition, wxSize(100,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtOnFloor"));
	txtOnFloor->SetToolTip(_("On Floor Number"));
	FlexGridSizer2->Add(txtOnFloor, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText25 = new wxStaticText(this, ID_STATICTEXT25, _("Motor:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT25"));
	FlexGridSizer2->Add(StaticText25, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtMotor = new wxTextCtrl(this, ID_txtMotor, wxEmptyString, wxDefaultPosition, wxSize(100,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtMotor"));
	txtMotor->SetToolTip(_("Motor Position"));
	FlexGridSizer2->Add(txtMotor, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText54 = new wxStaticText(this, ID_STATICTEXT54, _("Camera:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT54"));
	FlexGridSizer2->Add(StaticText54, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtCameraOffset = new wxTextCtrl(this, ID_txtCameraOffset, wxEmptyString, wxDefaultPosition, wxSize(100,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtCameraOffset"));
	txtCameraOffset->SetToolTip(_("Camera Offset"));
	FlexGridSizer2->Add(txtCameraOffset, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText71 = new wxStaticText(this, ID_STATICTEXT71, _("Music:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT71"));
	FlexGridSizer2->Add(StaticText71, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtMusicPosition = new wxTextCtrl(this, ID_txtMusicPosition, wxEmptyString, wxDefaultPosition, wxSize(100,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtMusicPosition"));
	txtMusicPosition->SetToolTip(_("Music Position"));
	FlexGridSizer2->Add(txtMusicPosition, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer5->Add(FlexGridSizer2, 0, wxALL|wxALIGN_TOP, 0);
	BoxSizer10->Add(StaticBoxSizer5, 0, wxALL|wxALIGN_LEFT, 0);
	StaticBoxSizer3 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Queues"));
	FlexGridSizer3 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticText38 = new wxStaticText(this, ID_STATICTEXT38, _("Direction:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT38"));
	FlexGridSizer3->Add(StaticText38, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtQueueDirection = new wxTextCtrl(this, ID_txtQueueDirection, wxEmptyString, wxDefaultPosition, wxSize(100,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtQueueDirection"));
	txtQueueDirection->SetToolTip(_("Queue Direction"));
	FlexGridSizer3->Add(txtQueueDirection, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText40 = new wxStaticText(this, ID_STATICTEXT40, _("LastUp:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT40"));
	FlexGridSizer3->Add(StaticText40, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtQueueLastUp = new wxTextCtrl(this, ID_txtQueueLastUp, wxEmptyString, wxDefaultPosition, wxSize(100,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtQueueLastUp"));
	txtQueueLastUp->SetToolTip(_("Queue Last Up"));
	FlexGridSizer3->Add(txtQueueLastUp, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText41 = new wxStaticText(this, ID_STATICTEXT41, _("LastDwn:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT41"));
	FlexGridSizer3->Add(StaticText41, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtQueueLastDown = new wxTextCtrl(this, ID_txtQueueLastDown, wxEmptyString, wxDefaultPosition, wxSize(100,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtQueueLastDown"));
	txtQueueLastDown->SetToolTip(_("Queue Last Down"));
	FlexGridSizer3->Add(txtQueueLastDown, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText51 = new wxStaticText(this, ID_STATICTEXT51, _("Last:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT51"));
	FlexGridSizer3->Add(StaticText51, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtQueueLastDirection = new wxTextCtrl(this, ID_txtQueueLastDirection, wxEmptyString, wxDefaultPosition, wxSize(100,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtQueueLastDirection"));
	txtQueueLastDirection->SetToolTip(_("Queue Last Direction"));
	FlexGridSizer3->Add(txtQueueLastDirection, 1, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText68 = new wxStaticText(this, ID_STATICTEXT68, _("Resets:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT68"));
	FlexGridSizer3->Add(StaticText68, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtQueueResets = new wxTextCtrl(this, ID_txtQueueResets, wxEmptyString, wxDefaultPosition, wxSize(100,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtQueueResets"));
	txtQueueResets->SetToolTip(_("Queue Resets"));
	FlexGridSizer3->Add(txtQueueResets, 1, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText69 = new wxStaticText(this, ID_STATICTEXT69, _("Limit:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT69"));
	FlexGridSizer3->Add(StaticText69, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtLimitQueue = new wxTextCtrl(this, ID_txtLimitQueue, wxEmptyString, wxDefaultPosition, wxSize(100,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtLimitQueue"));
	txtLimitQueue->SetToolTip(_("Limit Queue"));
	FlexGridSizer3->Add(txtLimitQueue, 1, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer3->Add(-1,-1,1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bResetQueues = new wxButton(this, ID_bResetQueues, _("Reset"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bResetQueues"));
	FlexGridSizer3->Add(bResetQueues, 1, wxTOP|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer3->Add(FlexGridSizer3, 0, wxALL|wxALIGN_TOP, 0);
	BoxSizer10->Add(StaticBoxSizer3, 0, wxALL|wxALIGN_LEFT, 0);
	BoxSizer3->Add(BoxSizer10, 0, wxBOTTOM|wxRIGHT|wxALIGN_TOP, 5);
	StaticBoxSizer7 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Motion"));
	FlexGridSizer6 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText21 = new wxStaticText(this, ID_STATICTEXT21, _("Speed:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT21"));
	FlexGridSizer6->Add(StaticText21, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtSpeed = new wxTextCtrl(this, ID_txtSpeed, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtSpeed"));
	txtSpeed->SetToolTip(_("Speed"));
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
	txtRate->SetToolTip(_("Rate"));
	FlexGridSizer6->Add(txtRate, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer6->Add(-1,-1,0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	StaticText27 = new wxStaticText(this, ID_STATICTEXT27, _("Direction:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT27"));
	FlexGridSizer6->Add(StaticText27, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtDirection = new wxTextCtrl(this, ID_txtDirection, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtDirection"));
	txtDirection->SetToolTip(_("Direction"));
	FlexGridSizer6->Add(txtDirection, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer6->Add(-1,-1,0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	StaticText72 = new wxStaticText(this, ID_STATICTEXT72, _("ActiveDir:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT72"));
	FlexGridSizer6->Add(StaticText72, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtActiveDirection = new wxTextCtrl(this, ID_txtActiveDirection, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtActiveDirection"));
	txtActiveDirection->SetToolTip(_("Active Direction"));
	FlexGridSizer6->Add(txtActiveDirection, 1, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer6->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("JerkRate:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	FlexGridSizer6->Add(StaticText2, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtJerkRate = new wxTextCtrl(this, ID_txtJerkRate, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtJerkRate"));
	txtJerkRate->SetToolTip(_("Jerk Rate"));
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
	txtIsIdle->SetToolTip(_("IsIdle"));
	FlexGridSizer6->Add(txtIsIdle, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer6->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText24 = new wxStaticText(this, ID_STATICTEXT24, _("Manual:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT24"));
	FlexGridSizer6->Add(StaticText24, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtManualGo = new wxTextCtrl(this, ID_txtManualGo, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtManualGo"));
	txtManualGo->SetToolTip(_("Manual"));
	FlexGridSizer6->Add(txtManualGo, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer6->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText55 = new wxStaticText(this, ID_STATICTEXT55, _("Leveling:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT55"));
	FlexGridSizer6->Add(StaticText55, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtLeveling = new wxTextCtrl(this, ID_txtLeveling, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtLeveling"));
	txtLeveling->SetToolTip(_("Leveling"));
	FlexGridSizer6->Add(txtLeveling, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer6->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText60 = new wxStaticText(this, ID_STATICTEXT60, _("Parking:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT60"));
	FlexGridSizer6->Add(StaticText60, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtParking = new wxTextCtrl(this, ID_txtParking, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtParking"));
	txtParking->SetToolTip(_("Parking"));
	FlexGridSizer6->Add(txtParking, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer6->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText73 = new wxStaticText(this, ID_STATICTEXT73, _("ManualMove:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT73"));
	FlexGridSizer6->Add(StaticText73, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtManualMove = new wxTextCtrl(this, ID_txtManualMove, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtManualMove"));
	txtManualMove->SetToolTip(_("Manual Move"));
	FlexGridSizer6->Add(txtManualMove, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
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
	StaticText74 = new wxStaticText(this, ID_STATICTEXT74, _("InsSpeed:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT74"));
	FlexGridSizer6->Add(StaticText74, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtInspectionSpeed = new wxTextCtrl(this, ID_txtInspectionSpeed, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtInspectionSpeed"));
	txtInspectionSpeed->SetToolTip(_("Inspection Mode Speed Multiplier"));
	FlexGridSizer6->Add(txtInspectionSpeed, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetInspectionSpeed = new wxButton(this, ID_bSetInspectionSpeed, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetInspectionSpeed"));
	FlexGridSizer6->Add(bSetInspectionSpeed, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer7->Add(FlexGridSizer6, 0, wxALL|wxALIGN_TOP, 0);
	BoxSizer3->Add(StaticBoxSizer7, 0, wxBOTTOM|wxRIGHT|wxALIGN_TOP, 5);
	BoxSizer7 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer8 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Destination"));
	FlexGridSizer7 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText30 = new wxStaticText(this, ID_STATICTEXT30, _("Floor:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT30"));
	FlexGridSizer7->Add(StaticText30, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtDestFloor = new wxTextCtrl(this, ID_txtDestFloor, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtDestFloor"));
	txtDestFloor->SetToolTip(_("Destination Floor"));
	FlexGridSizer7->Add(txtDestFloor, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer7->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText43 = new wxStaticText(this, ID_STATICTEXT43, _("IsMoving:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT43"));
	FlexGridSizer7->Add(StaticText43, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtIsMoving = new wxTextCtrl(this, ID_txtIsMoving, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtIsMoving"));
	txtIsMoving->SetToolTip(_("IsMoving"));
	FlexGridSizer7->Add(txtIsMoving, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer7->Add(-1,-1,1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText31 = new wxStaticText(this, ID_STATICTEXT31, _("Move:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT31"));
	FlexGridSizer7->Add(StaticText31, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	txtMoveElevator = new wxTextCtrl(this, ID_txtMoveElevator, wxEmptyString, wxDefaultPosition, wxSize(75,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtMoveElevator"));
	txtMoveElevator->SetToolTip(_("Move"));
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
	StaticText59 = new wxStaticText(this, ID_STATICTEXT59, _("NotifyEarly:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT59"));
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
	StaticBoxSizer8->Add(FlexGridSizer7, 0, wxALIGN_TOP, 5);
	BoxSizer7->Add(StaticBoxSizer8, 0, wxEXPAND, 5);
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
	StaticText62 = new wxStaticText(this, ID_STATICTEXT62, _("NudgeTmr:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT62"));
	FlexGridSizer5->Add(StaticText62, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	txtNudgeTimer = new wxTextCtrl(this, ID_txtNudgeTimer, wxEmptyString, wxDefaultPosition, wxSize(75,-1), 0, wxDefaultValidator, _T("ID_txtNudgeTimer"));
	txtNudgeTimer->SetToolTip(_("Nudge Timer"));
	FlexGridSizer5->Add(txtNudgeTimer, 1, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	bSetNudgeTimer = new wxButton(this, ID_bSetNudgeTimer, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetNudgeTimer"));
	FlexGridSizer5->Add(bSetNudgeTimer, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer4->Add(FlexGridSizer5, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer7->Add(StaticBoxSizer4, 1, wxALIGN_LEFT, 5);
	BoxSizer3->Add(BoxSizer7, 1, wxBOTTOM|wxRIGHT|wxALIGN_TOP, 5);
	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxEXPAND, 0);
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
	Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&editelevator::On_chkVisible_Click);
	Connect(ID_chkRun,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&editelevator::On_chkRun_Click);
	Connect(ID_bUp,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bUp_Toggle);
	Connect(ID_bGoToggle,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bGoToggle_Toggle);
	Connect(ID_bDown,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bDown_Toggle);
	Connect(ID_bRefresh,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bRefresh_Click);
	Connect(ID_bDumpFloors,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bDumpFloors_Click);
	Connect(ID_bDumpQueues,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bDumpQueues_Click);
	Connect(ID_bCall,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bCall_Click);
	Connect(ID_bGo,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bGo_Click);
	Connect(ID_bOpen,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bOpen_Click);
	Connect(ID_bOpenManual,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bOpenManual_Click);
	Connect(ID_bOpenShaftDoor,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bOpenShaftDoor_Click);
	Connect(ID_bStop,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bStop_Click);
	Connect(ID_bChime,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bChime_Click);
	Connect(ID_bEnqueueUp,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bEnqueueUp_Click);
	Connect(ID_bEnqueueDown,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bEnqueueDown_Click);
	Connect(ID_bClose,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bClose_Click);
	Connect(ID_bCloseManual,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bCloseManual_Click);
	Connect(ID_bCloseShaftDoor,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bCloseShaftDoor_Click);
	Connect(ID_bHoldDoors,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bHoldDoors_Click);
	Connect(ID_bStopDoors,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bStopDoors_Click);
	Connect(ID_bSetName,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetName_Click);
	Connect(ID_bSetType,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetType_Click);
	Connect(ID_bSetDoorTimer,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetDoorTimer_Click);
	Connect(ID_bSetQuickClose,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetQuickClose_Click);
	Connect(ID_bSetSkipFloorText,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetSkipFloorText_Click);
	Connect(ID_bSetNudge,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetNudge_Click);
	Connect(ID_bDoorSensor,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bDoorSensor_Click);
	Connect(ID_bSetLevelingSpeed,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetLevelingSpeed_Click);
	Connect(ID_bSetLevelingOffset,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetLevelingOffset_Click);
	Connect(ID_bLevelingOpen,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetLevelingOpen_Click);
	Connect(ID_bSetMusicOn,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetMusicOn_Click);
	Connect(ID_bSetMusicOnMove,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetMusicOnMove_Click);
	Connect(ID_bSetFloorSounds,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetFloorSounds_Click);
	Connect(ID_bSetFloorBeeps,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetFloorBeeps_Click);
	Connect(ID_bSetMessageSounds,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetMessageSounds_Click);
	Connect(ID_bSetAutoEnable,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetAutoEnable_Click);
	Connect(ID_bSetReOpen,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetReOpen_Click);
	Connect(ID_bSetAutoDoors,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetAutoDoors_Click);
	Connect(ID_bInterlocks,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bInterlocks_Click);
	Connect(ID_bResetQueues,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bResetQueues_Click);
	Connect(ID_bSetSpeed,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetSpeed_Click);
	Connect(ID_bSetAcceleration,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetAcceleration_Click);
	Connect(ID_bSetDeceleration,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetDeceleration_Click);
	Connect(ID_bSetAccelJerk,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetAccelJerk_Click);
	Connect(ID_bSetDecelJerk,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetDecelJerk_Click);
	Connect(ID_bSetSlowSpeed,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetSlowSpeed_Click);
	Connect(ID_bSetManualSpeed,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetManualSpeed_Click);
	Connect(ID_bSetInspectionSpeed,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&editelevator::On_bSetInspectionSpeed_Click);
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
	Simcore = 0;
	debugpanel = parent;
	elevator = 0;
	car = 0;
	door = 0;
	OnInit();
}

editelevator::~editelevator()
{
}


void editelevator::On_bCall_Click(wxCommandEvent& event)
{
	//calls elevator to the current camera floor

	if (elevator && car)
	{
		if (car->GetFloor() > Simcore->camera->CurrentFloor)
			elevator->AddRoute(Simcore->camera->CurrentFloor, -1, 1);
		if (car->GetFloor() < Simcore->camera->CurrentFloor)
			elevator->AddRoute(Simcore->camera->CurrentFloor, 1, 1);
	}
}

void editelevator::On_bEnqueueUp_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->AddRoute(floor_number, 1, 0);
}

void editelevator::On_bGo_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->Go(floor_number);
}

void editelevator::On_bEnqueueDown_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->AddRoute(floor_number, -1, 0);
}

void editelevator::On_bOpen_Click(wxCommandEvent& event)
{
	if (car)
		car->OpenDoors(sDoor->GetThumbPosition());
}

void editelevator::On_bClose_Click(wxCommandEvent& event)
{
	if (car)
		car->CloseDoors(sDoor->GetThumbPosition());
}

void editelevator::On_bOpenManual_Click(wxCommandEvent& event)
{
	if (car)
		car->OpenDoorsEmergency(sDoor->GetThumbPosition());
}

void editelevator::On_bCloseManual_Click(wxCommandEvent& event)
{
	if (car)
		car->CloseDoorsEmergency(sDoor->GetThumbPosition());
}

void editelevator::On_bStop_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->Stop(true);
}

void editelevator::On_bHoldDoors_Click(wxCommandEvent& event)
{
	if (car)
		car->HoldDoors(sDoor->GetThumbPosition());
}

void editelevator::On_bSetName_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->Name = txtName->GetValue();
}

void editelevator::On_bSetSpeed_Click(wxCommandEvent& event)
{
	if (elevator)
		 elevator->ElevatorSpeed = atof(txtSpeed->GetValue());
}

void editelevator::On_bSetAcceleration_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->Acceleration = atof(txtAcceleration->GetValue());
}

void editelevator::On_bSetDeceleration_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->Deceleration = atof(txtDeceleration->GetValue());
}

void editelevator::On_bDumpFloors_Click(wxCommandEvent& event)
{
	if (car)
		car->DumpServicedFloors();
}

void editelevator::On_bDumpQueues_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->DumpQueues();
}

void editelevator::OnInit()
{
	Simcore = debugpanel->GetSystem();

	last_elevator = 0;
	floor_number = 0;
	last_car = 0;
	last_door = -1;
	last_elevator_count = 0;
	last_car_count = 0;
	last_door_count = 0;

	if (Simcore->GetElevatorCount() > 0)
	{
		//set elevator range slider
		sNumber->SetScrollbar(0, 1, Simcore->GetElevatorCount(), 1);

		//set car range slider
		sCar->SetScrollbar(1, 1, Simcore->GetElevator(sNumber->GetThumbPosition() + 1)->GetCarCount(), 1);

		//set door range slider
		sDoor->SetScrollbar(1, 1, Simcore->GetElevator(sNumber->GetThumbPosition() + 1)->GetCar(1)->NumDoors + 1, 1);
	}
	else
		sNumber->Enable(false);
}

void editelevator::Loop()
{
	//if active engine has changed, refresh values
	if (Simcore != debugpanel->GetSystem())
		OnInit();

	if (!Simcore)
		return;

	int elev_num = sNumber->GetThumbPosition() + 1;
	int car_num = sCar->GetThumbPosition() + 1;
	int door_num = sDoor->GetThumbPosition();
	int elevator_count = Simcore->GetElevatorCount();
	elevator = Simcore->GetElevator(elev_num);
	door = 0;

	if (!elevator)
		return;

	int car_count = elevator->GetCarCount();
	car = elevator->GetCar(car_num);

	if (!car)
	{
		sCar->SetThumbPosition(0);
		return;
	}

	int door_count = car->NumDoors;
	door = car->GetDoor(door_num);

	if (elev_num != last_elevator || car_count != last_car_count || door_count != last_door_count)
	{
		//number or count changed; update values
		last_elevator = elev_num;
		last_car_count = car_count;
		last_door_count = door_count;

		//set floor range slider
		sFloor->SetScrollbar(0, 1, car->GetServicedFloorCount(), 1);

		//set car range slider
		sCar->SetScrollbar(0, 1, car_count, 1);

		//set door range slider
		sDoor->SetScrollbar(1, 1, door_count + 1, 1);

		SetMainValues();
	}
	if (car_num != last_car)
	{
		//number changed; update values
		last_car = car_num;
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
		sNumber->SetScrollbar(0, 1, Simcore->GetElevatorCount(), 1);
	}

	tElevator->SetLabel(wxT("Number " + ToString(sNumber->GetThumbPosition() + 1)));
	floor_number = car->GetServicedFloor(sFloor->GetThumbPosition());
	wxString floor_name;
	if (Simcore->GetFloor(floor_number))
		floor_name = Simcore->GetFloor(floor_number)->ID;
	tFloor->SetLabel(wxT("Floor ") + ToString(floor_number) + wxT(" (" + floor_name) + wxT(")"));
	tCar->SetLabel(wxT("Car " + ToString(sCar->GetThumbPosition() + 1)));
	tDoor->SetLabel(wxT("Door " + ToString(sDoor->GetThumbPosition()) + wxT(" (0 = all)")));
	txtBrakes->SetValue(BoolToString(elevator->GetBrakeStatus()));
	txtDestFloor->SetValue(ToString(elevator->GotoFloor));
	txtDestination->SetValue(TruncateNumber(elevator->GetDestination(), 2));
	txtDirection->SetValue(ToString(elevator->Direction));
	txtDistance->SetValue(TruncateNumber(elevator->DistanceToTravel, 2));
	if (door)
	{
		txtDoorDirection->SetValue(BoolToString(door->DoorDirection));
		txtDoorSize->SetValue(TruncateNumber(door->GetDoorWrapper()->Width, 2) + wxT(" x ") + TruncateNumber(door->GetDoorWrapper()->Height, 2));
		txtDoorOrigin->SetValue(TruncateNumber(door->GetDoorWrapper()->GetPosition().x, 2) + wxT(", ") + TruncateNumber(door->GetDoorWrapper()->GetPosition().y, 2) + wxT(", ") + TruncateNumber(door->GetDoorWrapper()->GetPosition().z, 2));
		txtDoorsOpen->SetValue(BoolToString(door->AreDoorsOpen()));
		txtShaftDoorOrigin->SetValue(TruncateNumber(door->ShaftDoorOrigin.x, 2) + wxT(", ") + TruncateNumber(door->ShaftDoorOrigin.y, 2) + wxT(", ") + TruncateNumber(door->ShaftDoorOrigin.z, 2));
		txtDoorStopped->SetValue(BoolToString(door->DoorsStopped()));
		txtDoorSensor->SetValue(BoolToString(door->GetSensorStatus()));
	}
	txtElevStart->SetValue(TruncateNumber(elevator->GetElevatorStart(), 2));
	txtEnabled->SetValue(BoolToString(elevator->IsEnabled));
	txtErrorOffset->SetValue(TruncateNumber(elevator->ErrorOffset, 2));
	txtFloor->SetValue(ToString(car->GetFloor()));
	txtHeight->SetValue(TruncateNumber(car->Height, 2));
	txtMoveElevator->SetValue(BoolToString(elevator->MoveElevator));
	txtNumber->SetValue(ToString(elevator->Number));
	txtOriginFloor->SetValue(ToString(car->StartingFloor));
	txtPosition->SetValue(TruncateNumber(elevator->GetPosition().x, 2) + wxT(", ") + TruncateNumber(elevator->GetPosition().y, 2) + wxT(", ") + TruncateNumber(elevator->GetPosition().z, 2));
	txtQueueDirection->SetValue(ToString(elevator->QueuePositionDirection));
	txtQueueLastDown->SetValue(ToString(elevator->LastQueueFloor[0]));
	txtQueueLastUp->SetValue(ToString(elevator->LastQueueFloor[1]));
	txtRate->SetValue(TruncateNumber(elevator->ElevatorRate, 2));
	txtShaft->SetValue(ToString(elevator->AssignedShaft));
	txtStop->SetValue(ToString(elevator->GetEmergencyStopStatus()));
	txtStopDistance->SetValue(TruncateNumber(elevator->GetStoppingDistance(), 2));
	txtTempDecel->SetValue(TruncateNumber(elevator->TempDeceleration, 2));
	txtJerkRate->SetValue(TruncateNumber(elevator->GetJerkRate(), 4));
	txtIsMoving->SetValue(BoolToString(elevator->IsMoving));
	txtOnFloor->SetValue(BoolToString(elevator->OnFloor));
	txtAlarm->SetValue(BoolToString(car->AlarmActive));
	txtQueueLastDirection->SetValue(ToString(elevator->LastQueueDirection));
	txtIsIdle->SetValue(BoolToString(elevator->IsIdle()));
	txtWaitForDoors->SetValue(BoolToString(elevator->WaitForDoors));
	txtMotor->SetValue(TruncateNumber(elevator->MotorPosition.x, 2) + wxT(", ") + TruncateNumber(elevator->MotorPosition.y, 2) + wxT(", ") + TruncateNumber(elevator->MotorPosition.z, 2));
	txtCameraOffset->SetValue(TruncateNumber(car->CameraOffset, 2));
	txtManualGo->SetValue(BoolToString(elevator->ManualGo));
	txtLeveling->SetValue(BoolToString(elevator->Leveling));
	txtParking->SetValue(BoolToString(elevator->Parking));
	txtQueueResets->SetValue(BoolToString(elevator->QueueResets));
	txtLimitQueue->SetValue(BoolToString(elevator->LimitQueue));
	txtNudgeMode->SetValue(BoolToString(car->IsNudgeModeActive()));
	txtNotified->SetValue(BoolToString(elevator->Notified));
	txtWaitForTimer->SetValue(BoolToString(elevator->WaitForTimer));
	txtMusicOn->SetValue(BoolToString(car->MusicOn));
	txtMusicOnMove->SetValue(BoolToString(car->MusicOnMove));
	txtFloorSounds->SetValue(BoolToString(car->UseFloorSounds));
	txtFloorBeeps->SetValue(BoolToString(car->UseFloorBeeps));
	txtMessageSounds->SetValue(BoolToString(car->UseDirMessageSounds));
	txtAutoEnable->SetValue(BoolToString(car->AutoEnable));
	txtReOpen->SetValue(BoolToString(elevator->ReOpen));
	txtAutoDoors->SetValue(BoolToString(elevator->AutoDoors));
	txtOpenOnStart->SetValue(BoolToString(elevator->OpenOnStart));
	txtInterlocks->SetValue(BoolToString(elevator->Interlocks));
	txtLastChimeDirection->SetValue(ToString(elevator->LastChimeDirection));
	txtActiveCallFloor->SetValue(ToString(elevator->GetActiveCallFloor()));
	txtActiveDirection->SetValue(ToString(elevator->ActiveDirection));
	txtManualMove->SetValue(ToString(elevator->ManualMove));
	txtMusicPosition->SetValue(TruncateNumber(car->MusicPosition.x, 2) + wxT(", ") + TruncateNumber(car->MusicPosition.y, 2) + wxT(", ") + TruncateNumber(car->MusicPosition.z, 2));

	//changeable values
	if (chkVisible->GetValue() != elevator->IsEnabled)
		chkVisible->SetValue(elevator->IsEnabled);
	if (chkRun->GetValue() != elevator->IsRunning())
		chkRun->SetValue(elevator->IsRunning());
	if (bACPMode->GetValue() != elevator->ACP)
		bACPMode->SetValue(elevator->ACP);
	if (bUpPeak->GetValue() != elevator->UpPeak)
		bUpPeak->SetValue(elevator->UpPeak);
	if (bDownPeak->GetValue() != elevator->DownPeak)
		bDownPeak->SetValue(elevator->DownPeak);
	if (bIndService->GetValue() != car->IndependentServiceActive())
		bIndService->SetValue(car->IndependentServiceActive());
	if (bInsService->GetValue() != elevator->InspectionService)
		bInsService->SetValue(elevator->InspectionService);
	if (car->FirePhase2Active() == 0 && Fire2Off->GetValue() == false)
		Fire2Off->SetValue(true);
	if (car->FirePhase2Active() == 1 && Fire2On->GetValue() == false)
		Fire2On->SetValue(true);
	if (car->FirePhase2Active() == 2 && Fire2Hold->GetValue() == false)
		Fire2Hold->SetValue(true);
	if (elevator->FireServicePhase1 == 0 && Fire1Off->GetValue() == false)
		Fire1Off->SetValue(true);
	if (elevator->FireServicePhase1 == 1 && Fire1On->GetValue() == false)
		Fire1On->SetValue(true);
 	if (elevator->FireServicePhase1 == 2 && Fire1Bypass->GetValue() == false)
		Fire1Bypass->SetValue(true);

	//set call direction/type value
	{
		std::string direction = "", type;
		int dir = elevator->GetActiveCallDirection();
		int t = elevator->GetActiveCallType();
		if (dir == 1)
			direction = "Up";
		else if (dir == -1)
			direction = "Down";

		if (dir != 0)
		{
			if (t == 0)
				type = "Car";
			else if (t == 1)
				type = "Hall";
			else
				type = "System";

			direction += " - " + type;
		}

		txtActiveCallDirection->SetValue(direction);
	}
}

void editelevator::SetMainValues()
{
	if (!elevator)
		return;

	//set changeable values
	txtName->SetValue(elevator->Name);
	txtType->SetValue(elevator->Type);
	if (door)
	{
		txtDoorTimer->SetValue(ToString(door->DoorTimer));
		txtQuickClose->SetValue(ToString(door->QuickClose));
		txtNudgeTimer->SetValue(TruncateNumber(door->NudgeTimer, 4));
		txtSlowSpeed->SetValue(TruncateNumber(door->SlowSpeed, 4));
		txtManualSpeed->SetValue(TruncateNumber(door->ManualSpeed, 4));
	}
	txtSpeed->SetValue(TruncateNumber(elevator->ElevatorSpeed, 4));
	txtAcceleration->SetValue(TruncateNumber(elevator->Acceleration, 4));
	txtDeceleration->SetValue(TruncateNumber(elevator->Deceleration, 4));
	txtAccelJerk->SetValue(TruncateNumber(elevator->AccelJerk, 4));
	txtDecelJerk->SetValue(TruncateNumber(elevator->DecelJerk, 4));
	txtSkipFloorText->SetValue(elevator->GetFloorSkipText());
	txtACPFloor->SetValue(ToString(elevator->ACPFloor));
	txtRecallFloor->SetValue(ToString(elevator->RecallFloor));
	txtRecallAlternate->SetValue(ToString(elevator->RecallFloorAlternate));
	txtParkingFloor->SetValue(ToString(elevator->ParkingFloor));
	txtParkingDelay->SetValue(TruncateNumber(elevator->ParkingDelay, 4));
	txtLevelingSpeed->SetValue(TruncateNumber(elevator->LevelingSpeed, 4));
	txtLevelingOffset->SetValue(TruncateNumber(elevator->LevelingOffset, 4));
	txtLevelingOpen->SetValue(TruncateNumber(elevator->LevelingOpen, 4));
	txtNotifyEarly->SetValue(ToString(elevator->NotifyEarly));
	txtDepartureDelay->SetValue(TruncateNumber(elevator->DepartureDelay, 4));
	txtArrivalDelay->SetValue(TruncateNumber(elevator->ArrivalDelay, 4));
	txtInspectionSpeed->SetValue(TruncateNumber(elevator->InspectionSpeed, 4));
}

void editelevator::On_chkVisible_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->Enabled(chkVisible->GetValue());
}

void editelevator::On_bSetAccelJerk_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->AccelJerk = atof(txtAccelJerk->GetValue());
}

void editelevator::On_bSetDecelJerk_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->DecelJerk = atof(txtDecelJerk->GetValue());
}

void editelevator::On_bOpenShaftDoor_Click(wxCommandEvent& event)
{
	if (car)
		car->OpenDoors(sDoor->GetThumbPosition(), 3, floor_number);
}

void editelevator::On_bCloseShaftDoor_Click(wxCommandEvent& event)
{
	if (car)
		car->CloseDoors(sDoor->GetThumbPosition(), 3, floor_number);
}

void editelevator::On_bSetDoorTimer_Click(wxCommandEvent& event)
{
	if (door)
		door->DoorTimer = atoi(txtDoorTimer->GetValue());
}

void editelevator::On_bChime_Click(wxCommandEvent& event)
{
	if (door)
		door->Chime(floor_number, true);
}

void editelevator::On_bACPMode_Toggle(wxCommandEvent& event)
{
	if (elevator)
		elevator->EnableACP(bACPMode->GetValue());
}

void editelevator::On_bUpPeak_Toggle(wxCommandEvent& event)
{
	if (elevator)
		elevator->EnableUpPeak(bUpPeak->GetValue());
}

void editelevator::On_bDownPeak_Toggle(wxCommandEvent& event)
{
	if (elevator)
		elevator->EnableDownPeak(bDownPeak->GetValue());
}

void editelevator::On_bIndService_Toggle(wxCommandEvent& event)
{
	if (elevator && car)
		elevator->EnableIndependentService(bIndService->GetValue(), car->Number);
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
	}
}

void editelevator::On_bRefresh_Click(wxCommandEvent& event)
{
	SetMainValues();
}

void editelevator::On_bSetSkipFloorText_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->SetFloorSkipText(std::string(txtSkipFloorText->GetValue()));
}

void editelevator::On_bSetACPFloor_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->SetACPFloor(atoi(txtACPFloor->GetValue()));
}

void editelevator::On_bSetRecallFloor_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->SetRecallFloor(atoi(txtRecallFloor->GetValue()));
}

void editelevator::On_bSetRecallAlternate_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->SetAlternateRecallFloor(atoi(txtRecallAlternate->GetValue()));
}

void editelevator::On_Fire1Off_Select(wxCommandEvent& event)
{
	if (elevator)
	{
		CallButton *callbutton = elevator->GetPrimaryCallButton();
		if (callbutton)
			callbutton->FireService(0);
	}
}

void editelevator::On_Fire1On_Select(wxCommandEvent& event)
{
	if (elevator)
	{
		CallButton *callbutton = elevator->GetPrimaryCallButton();
		if (callbutton)
			callbutton->FireService(1);
	}
}

void editelevator::On_Fire1Bypass_Select(wxCommandEvent& event)
{
	if (elevator)
	{
		CallButton *callbutton = elevator->GetPrimaryCallButton();
		if (callbutton)
			callbutton->FireService(2);
	}
}

void editelevator::On_Fire2Off_Select(wxCommandEvent& event)
{
	if (elevator && car)
		elevator->EnableFireService2(0, car->Number);
}

void editelevator::On_Fire2On_Select(wxCommandEvent& event)
{
	if (elevator && car)
		elevator->EnableFireService2(1, car->Number);
}

void editelevator::On_Fire2Hold_Select(wxCommandEvent& event)
{
	if (elevator && car)
		elevator->EnableFireService2(2, car->Number);
}

void editelevator::On_bStopDoors_Click(wxCommandEvent& event)
{
	if (car)
		car->StopDoors(sDoor->GetThumbPosition());
}

void editelevator::On_bUp_Toggle(wxCommandEvent& event)
{
	if (elevator)
	{
		if (elevator->InspectionService == false)
			return;

		elevator->SetUpButton(bUp->GetValue());
	}
}

void editelevator::On_bGoToggle_Toggle(wxCommandEvent& event)
{
	if (elevator)
	{
		if (elevator->InspectionService == false)
			return;

		elevator->SetGoButton(bGoToggle->GetValue());
	}
}

void editelevator::On_bDown_Toggle(wxCommandEvent& event)
{
	if (elevator)
	{
		if (elevator->InspectionService == false)
			return;

		elevator->SetDownButton(bDown->GetValue());
	}
}

void editelevator::On_bResetQueues_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->ResetQueue(true, true);
}

void editelevator::On_bSetParkingFloor_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->ParkingFloor = atoi(txtParkingFloor->GetValue());
}

void editelevator::On_bSetParkingDelay_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->ParkingDelay = atof(txtParkingDelay->GetValue());
}

void editelevator::On_bSetLevelingSpeed_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->LevelingSpeed = atof(txtLevelingSpeed->GetValue());
}

void editelevator::On_bSetLevelingOffset_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->LevelingOffset = atof(txtLevelingOffset->GetValue());
}

void editelevator::On_bSetLevelingOpen_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->LevelingOpen = atof(txtLevelingOpen->GetValue());
}

void editelevator::On_bNotifyEarly_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->NotifyEarly = atoi(txtNotifyEarly->GetValue());
}

void editelevator::On_chkRun_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->SetRunState(chkRun->GetValue());
}

void editelevator::On_bSetQuickClose_Click(wxCommandEvent& event)
{
	if (door)
		door->QuickClose = atoi(txtQuickClose->GetValue());
}

void editelevator::On_bSetNudgeTimer_Click(wxCommandEvent& event)
{
	if (door)
		door->NudgeTimer = atof(txtNudgeTimer->GetValue());
}

void editelevator::On_bSetSlowSpeed_Click(wxCommandEvent& event)
{
	if (door)
		door->SlowSpeed = atof(txtSlowSpeed->GetValue());
}

void editelevator::On_bSetManualSpeed_Click(wxCommandEvent& event)
{
	if (door)
		door->ManualSpeed = atof(txtManualSpeed->GetValue());
}

void editelevator::On_bSetDepartureDelay_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->DepartureDelay = atof(txtDepartureDelay->GetValue());
}

void editelevator::On_bSetArrivalDelay_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->ArrivalDelay = atof(txtArrivalDelay->GetValue());
}

void editelevator::On_bSetInspectionSpeed_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->InspectionSpeed = atof(txtInspectionSpeed->GetValue());
}

void editelevator::On_bSetMusicOn_Click(wxCommandEvent& event)
{
	if (car)
		car->MusicOn = !car->MusicOn;
}

void editelevator::On_bSetMusicOnMove_Click(wxCommandEvent& event)
{
	if (car)
		car->MusicOnMove = !car->MusicOnMove;
}

void editelevator::On_bSetFloorSounds_Click(wxCommandEvent& event)
{
	if (car)
		car->UseFloorSounds = !car->UseFloorSounds;
}

void editelevator::On_bSetFloorBeeps_Click(wxCommandEvent& event)
{
	if (car)
		car->UseFloorBeeps = !car->UseFloorBeeps;
}

void editelevator::On_bSetMessageSounds_Click(wxCommandEvent& event)
{
	if (car)
		car->UseDirMessageSounds = !car->UseDirMessageSounds;
}

void editelevator::On_bSetAutoEnable_Click(wxCommandEvent& event)
{
	if (car)
		car->AutoEnable = !car->AutoEnable;
}

void editelevator::On_bSetReOpen_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->ReOpen = !elevator->ReOpen;
}

void editelevator::On_bSetAutoDoors_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->AutoDoors = !elevator->AutoDoors;
}

void editelevator::On_bInterlocks_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->Interlocks = !elevator->Interlocks;
}

void editelevator::On_bSetNudge_Click(wxCommandEvent& event)
{
	if (elevator)
	{
		int door = sDoor->GetThumbPosition();
		bool status = car->IsNudgeModeActive(door);
		car->EnableNudgeMode(!status, door);
	}
}

void editelevator::On_bDoorSensor_Click(wxCommandEvent& event)
{
	if (elevator)
	{
		int door = sDoor->GetThumbPosition();
		bool status = car->GetSensorStatus(door);
		car->EnableSensor(!status, door);
	}
}

void editelevator::On_bSetType_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->Type = txtType->GetValue();
}

}
