/* $Id$ */

/*
	Skyscraper 1.1 Alpha - Edit Elevator Form
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
#include "editelevator.h"
#include <crystalspace.h>
#include "sbs.h"
#include "elevator.h"
#include "unix.h"

extern SBS *sbs; //external pointer to the SBS engine
Elevator *elevator;

BEGIN_EVENT_TABLE(editelevator,wxDialog)
//(*EventTable(editelevator)
EVT_BUTTON(ID_bDumpFloors,editelevator::On_bDumpFloors_Click)
EVT_BUTTON(ID_bDumpQueues,editelevator::On_bDumpQueues_Click)
EVT_BUTTON(ID_bCall,editelevator::On_bCall_Click)
EVT_BUTTON(ID_bGo,editelevator::On_bGo_Click)
EVT_BUTTON(ID_bOpen,editelevator::On_bOpen_Click)
EVT_BUTTON(ID_bOpenManual,editelevator::On_bOpenManual_Click)
EVT_BUTTON(ID_bStop,editelevator::On_bStop_Click)
EVT_BUTTON(ID_bEnqueueUp,editelevator::On_bEnqueueUp_Click)
EVT_BUTTON(ID_bEnqueueDown,editelevator::On_bEnqueueDown_Click)
EVT_BUTTON(ID_bClose,editelevator::On_bClose_Click)
EVT_BUTTON(ID_bCloseManual,editelevator::On_bCloseManual_Click)
EVT_BUTTON(ID_bAlarm,editelevator::On_bAlarm_Click)
EVT_BUTTON(ID_bSetName,editelevator::On_bSetName_Click)
EVT_BUTTON(ID_bSetSpeed,editelevator::On_bSetSpeed_Click)
EVT_BUTTON(ID_bSetAcceleration,editelevator::On_bSetAcceleration_Click)
EVT_BUTTON(ID_bSetDeceleration,editelevator::On_bSetDeceleration_Click)
EVT_BUTTON(ID_bSetOpenSpeed,editelevator::On_bSetOpenSpeed_Click)
EVT_BUTTON(ID_bSetDoorAccel,editelevator::On_bSetDoorAccel_Click)
//*)
END_EVENT_TABLE()

editelevator::editelevator(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(editelevator)
	Create(parent,id,_("Elevator Parameters"),wxDefaultPosition,wxDefaultSize,wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxMINIMIZE_BOX,_T(""));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer6 = new wxBoxSizer(wxVERTICAL);
	tElevator = new wxStaticText(this,ID_tElevator,_("Elevator"),wxDefaultPosition,wxDefaultSize,wxALIGN_CENTRE,_("ID_tElevator"));
	sNumber = new wxScrollBar(this,ID_sNumber,wxDefaultPosition,wxDefaultSize,wxSB_HORIZONTAL,wxDefaultValidator,_("ID_sNumber"));
	sNumber->SetScrollbar(0,1,10,1);
	tFloor = new wxStaticText(this,ID_tFloor,_("Floor"),wxDefaultPosition,wxDefaultSize,wxALIGN_CENTRE,_("ID_tFloor"));
	sFloor = new wxScrollBar(this,ID_sFloor,wxDefaultPosition,wxDefaultSize,wxSB_HORIZONTAL,wxDefaultValidator,_("ID_sFloor"));
	sFloor->SetScrollbar(0,1,10,1);
	BoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	bDumpFloors = new wxButton(this,ID_bDumpFloors,_("Dump Floor List"),wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_("ID_bDumpFloors"));
	if (false) bDumpFloors->SetDefault();
	bDumpQueues = new wxButton(this,ID_bDumpQueues,_("Dump Queues"),wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_("ID_bDumpQueues"));
	if (false) bDumpQueues->SetDefault();
	BoxSizer7->Add(bDumpFloors,1,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL,15);
	BoxSizer7->Add(bDumpQueues,1,wxLEFT|wxRIGHT|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL,15);
	BoxSizer6->Add(tElevator,0,wxALL|wxALIGN_CENTER,0);
	BoxSizer6->Add(sNumber,0,wxALL|wxALIGN_CENTER|wxEXPAND,0);
	BoxSizer6->Add(tFloor,0,wxTOP|wxALIGN_CENTER,10);
	BoxSizer6->Add(sFloor,0,wxALL|wxALIGN_CENTER|wxEXPAND,0);
	BoxSizer6->Add(BoxSizer7,0,wxTOP|wxALIGN_CENTER_HORIZONTAL|wxALIGN_BOTTOM|wxEXPAND,15);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL,this,_("Control"));
	BoxSizer8 = new wxBoxSizer(wxVERTICAL);
	bCall = new wxButton(this,ID_bCall,_("Call"),wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_("ID_bCall"));
	if (false) bCall->SetDefault();
	bCall->SetFocus();
	bGo = new wxButton(this,ID_bGo,_("Go"),wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_("ID_bGo"));
	if (false) bGo->SetDefault();
	bOpen = new wxButton(this,ID_bOpen,_("Open"),wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_("ID_bOpen"));
	if (false) bOpen->SetDefault();
	bOpenManual = new wxButton(this,ID_bOpenManual,_("Open Manual"),wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_("ID_bOpenManual"));
	if (false) bOpenManual->SetDefault();
	bStop = new wxButton(this,ID_bStop,_("Emerg. Stop"),wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_("ID_bStop"));
	if (false) bStop->SetDefault();
	BoxSizer8->Add(bCall,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP|wxEXPAND,0);
	BoxSizer8->Add(bGo,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP|wxEXPAND,0);
	BoxSizer8->Add(-1,10,0);
	BoxSizer8->Add(bOpen,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP|wxEXPAND,0);
	BoxSizer8->Add(bOpenManual,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP|wxEXPAND,0);
	BoxSizer8->Add(-1,10,0);
	BoxSizer8->Add(bStop,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP|wxEXPAND,0);
	BoxSizer9 = new wxBoxSizer(wxVERTICAL);
	bEnqueueUp = new wxButton(this,ID_bEnqueueUp,_("Enqueue Up"),wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_("ID_bEnqueueUp"));
	if (false) bEnqueueUp->SetDefault();
	bEnqueueDown = new wxButton(this,ID_bEnqueueDown,_("Enqueue Down"),wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_("ID_bEnqueueDown"));
	if (false) bEnqueueDown->SetDefault();
	bClose = new wxButton(this,ID_bClose,_("Close"),wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_("ID_bClose"));
	if (false) bClose->SetDefault();
	bCloseManual = new wxButton(this,ID_bCloseManual,_("Close Manual"),wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_("ID_bCloseManual"));
	if (false) bCloseManual->SetDefault();
	bAlarm = new wxButton(this,ID_bAlarm,_("Alarm"),wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_("ID_bAlarm"));
	if (false) bAlarm->SetDefault();
	BoxSizer9->Add(bEnqueueUp,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP|wxEXPAND,0);
	BoxSizer9->Add(bEnqueueDown,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP|wxEXPAND,0);
	BoxSizer9->Add(-1,10,0);
	BoxSizer9->Add(bClose,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP|wxEXPAND,0);
	BoxSizer9->Add(bCloseManual,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP|wxEXPAND,0);
	BoxSizer9->Add(-1,10,0);
	BoxSizer9->Add(bAlarm,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP|wxEXPAND,0);
	StaticBoxSizer1->Add(BoxSizer8,1,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	StaticBoxSizer1->Add(BoxSizer9,1,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	BoxSizer2->Add(BoxSizer6,2,wxLEFT|wxRIGHT|wxTOP|wxALIGN_LEFT|wxALIGN_TOP,10);
	BoxSizer2->Add(StaticBoxSizer1,1,wxRIGHT|wxTOP|wxALIGN_RIGHT|wxALIGN_TOP,5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxHORIZONTAL,this,_("General"));
	FlexGridSizer1 = new wxFlexGridSizer(0,3,0,0);
	StaticText3 = new wxStaticText(this,ID_STATICTEXT3,_("Number:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT3"));
	txtNumber = new wxTextCtrl(this,ID_txtNumber,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtNumber"));
	if ( 0 ) txtNumber->SetMaxLength(0);
	StaticText5 = new wxStaticText(this,ID_STATICTEXT5,_("Name:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT5"));
	txtName = new wxTextCtrl(this,ID_txtName,_T(""),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_txtName"));
	if ( 0 ) txtName->SetMaxLength(0);
	bSetName = new wxButton(this,ID_bSetName,_("Set"),wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT,wxDefaultValidator,_("ID_bSetName"));
	if (false) bSetName->SetDefault();
	StaticText4 = new wxStaticText(this,ID_STATICTEXT4,_("Enabled:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT4"));
	txtEnabled = new wxTextCtrl(this,ID_txtEnabled,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtEnabled"));
	if ( 0 ) txtEnabled->SetMaxLength(0);
	StaticText6 = new wxStaticText(this,ID_STATICTEXT6,_("Shaft:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT6"));
	txtShaft = new wxTextCtrl(this,ID_txtShaft,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtShaft"));
	if ( 0 ) txtShaft->SetMaxLength(0);
	StaticText7 = new wxStaticText(this,ID_STATICTEXT7,_("Height:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT7"));
	txtHeight = new wxTextCtrl(this,ID_txtHeight,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtHeight"));
	if ( 0 ) txtHeight->SetMaxLength(0);
	StaticText8 = new wxStaticText(this,ID_STATICTEXT8,_("DWidth:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT8"));
	txtDoorWidth = new wxTextCtrl(this,ID_txtDoorWidth,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtDoorWidth"));
	if ( 0 ) txtDoorWidth->SetMaxLength(0);
	StaticText9 = new wxStaticText(this,ID_STATICTEXT9,_("DHeight:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT9"));
	txtDoorHeight = new wxTextCtrl(this,ID_txtDoorHeight,_T(""),wxPoint(-1,75),wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtDoorHeight"));
	if ( 0 ) txtDoorHeight->SetMaxLength(0);
	StaticText10 = new wxStaticText(this,ID_STATICTEXT10,_("DDirection:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT10"));
	txtDoorDirection = new wxTextCtrl(this,ID_txtDoorDirection,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtDoorDirection"));
	if ( 0 ) txtDoorDirection->SetMaxLength(0);
	StaticText11 = new wxStaticText(this,ID_STATICTEXT11,_("DoorsOpen:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT11"));
	txtDoorsOpen = new wxTextCtrl(this,ID_txtDoorsOpen,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtDoorsOpen"));
	if ( 0 ) txtDoorsOpen->SetMaxLength(0);
	StaticText12 = new wxStaticText(this,ID_STATICTEXT12,_("Brakes:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT12"));
	txtBrakes = new wxTextCtrl(this,ID_txtBrakes,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtBrakes"));
	if ( 0 ) txtBrakes->SetMaxLength(0);
	StaticText13 = new wxStaticText(this,ID_STATICTEXT13,_("EmergStop:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT13"));
	txtStop = new wxTextCtrl(this,ID_txtStop,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtStop"));
	if ( 0 ) txtStop->SetMaxLength(0);
	FlexGridSizer1->Add(StaticText3,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer1->Add(txtNumber,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer1->Add(-1,-1,0);
	FlexGridSizer1->Add(StaticText5,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer1->Add(txtName,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer1->Add(bSetName,0,wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL,0);
	FlexGridSizer1->Add(StaticText4,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer1->Add(txtEnabled,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer1->Add(-1,-1,0);
	FlexGridSizer1->Add(StaticText6,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer1->Add(txtShaft,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer1->Add(-1,-1,0);
	FlexGridSizer1->Add(StaticText7,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer1->Add(txtHeight,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer1->Add(-1,-1,0);
	FlexGridSizer1->Add(StaticText8,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer1->Add(txtDoorWidth,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer1->Add(-1,-1,0);
	FlexGridSizer1->Add(StaticText9,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer1->Add(txtDoorHeight,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer1->Add(-1,-1,0);
	FlexGridSizer1->Add(StaticText10,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer1->Add(txtDoorDirection,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer1->Add(-1,-1,0);
	FlexGridSizer1->Add(StaticText11,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer1->Add(txtDoorsOpen,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer1->Add(-1,-1,0);
	FlexGridSizer1->Add(StaticText12,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer1->Add(txtBrakes,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer1->Add(-1,-1,0);
	FlexGridSizer1->Add(StaticText13,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer1->Add(txtStop,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer1->Add(-1,-1,0);
	StaticBoxSizer2->Add(FlexGridSizer1,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	BoxSizer10 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer5 = new wxStaticBoxSizer(wxHORIZONTAL,this,_("Position"));
	FlexGridSizer2 = new wxFlexGridSizer(0,2,0,0);
	StaticText14 = new wxStaticText(this,ID_STATICTEXT14,_("Floor:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT14"));
	txtFloor = new wxTextCtrl(this,ID_txtFloor,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtFloor"));
	if ( 0 ) txtFloor->SetMaxLength(0);
	StaticText15 = new wxStaticText(this,ID_STATICTEXT15,_("Pos:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT15"));
	txtPosition = new wxTextCtrl(this,ID_txtPosition,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtPosition"));
	if ( 0 ) txtPosition->SetMaxLength(0);
	StaticText16 = new wxStaticText(this,ID_STATICTEXT16,_("Origin:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT16"));
	txtOrigin = new wxTextCtrl(this,ID_txtOrigin,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtOrigin"));
	if ( 0 ) txtOrigin->SetMaxLength(0);
	StaticText17 = new wxStaticText(this,ID_STATICTEXT17,_("OriginFlr:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT17"));
	txtOriginFloor = new wxTextCtrl(this,ID_txtOriginFloor,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtOriginFloor"));
	if ( 0 ) txtOriginFloor->SetMaxLength(0);
	StaticText18 = new wxStaticText(this,ID_STATICTEXT18,_("EStart:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT18"));
	txtElevStart = new wxTextCtrl(this,ID_txtElevStart,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtElevStart"));
	if ( 0 ) txtElevStart->SetMaxLength(0);
	StaticText19 = new wxStaticText(this,ID_STATICTEXT19,_("DOrigin:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT19"));
	txtDoorOrigin = new wxTextCtrl(this,ID_txtDoorOrigin,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtDoorOrigin"));
	if ( 0 ) txtDoorOrigin->SetMaxLength(0);
	StaticText20 = new wxStaticText(this,ID_STATICTEXT20,_("SDOrigin:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT20"));
	txtShaftDoorOrigin = new wxTextCtrl(this,ID_txtShaftDoorOrigin,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtShaftDoorOrigin"));
	if ( 0 ) txtShaftDoorOrigin->SetMaxLength(0);
	FlexGridSizer2->Add(StaticText14,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer2->Add(txtFloor,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer2->Add(StaticText15,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer2->Add(txtPosition,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer2->Add(StaticText16,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer2->Add(txtOrigin,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer2->Add(StaticText17,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer2->Add(txtOriginFloor,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer2->Add(StaticText18,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer2->Add(txtElevStart,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer2->Add(StaticText19,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer2->Add(txtDoorOrigin,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer2->Add(StaticText20,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer2->Add(txtShaftDoorOrigin,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	StaticBoxSizer5->Add(FlexGridSizer2,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	StaticBoxSizer3 = new wxStaticBoxSizer(wxHORIZONTAL,this,_("Queues"));
	FlexGridSizer3 = new wxFlexGridSizer(0,2,0,0);
	StaticText38 = new wxStaticText(this,ID_STATICTEXT38,_("Direction:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT38"));
	txtQueueDirection = new wxTextCtrl(this,ID_txtQueueDirection,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtQueueDirection"));
	if ( 0 ) txtQueueDirection->SetMaxLength(0);
	StaticText39 = new wxStaticText(this,ID_STATICTEXT39,_("Pause:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT39"));
	txtQueuePause = new wxTextCtrl(this,ID_txtQueuePause,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtQueuePause"));
	if ( 0 ) txtQueuePause->SetMaxLength(0);
	StaticText40 = new wxStaticText(this,ID_STATICTEXT40,_("LastUp:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT40"));
	txtQueueLastUp = new wxTextCtrl(this,ID_txtQueueLastUp,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtQueueLastUp"));
	if ( 0 ) txtQueueLastUp->SetMaxLength(0);
	StaticText41 = new wxStaticText(this,ID_STATICTEXT41,_("LastDwn:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT41"));
	txtQueueLastDown = new wxTextCtrl(this,ID_txtQueueLastDown,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtQueueLastDown"));
	if ( 0 ) txtQueueLastDown->SetMaxLength(0);
	FlexGridSizer3->Add(StaticText38,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer3->Add(txtQueueDirection,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer3->Add(StaticText39,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer3->Add(txtQueuePause,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer3->Add(StaticText40,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer3->Add(txtQueueLastUp,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer3->Add(StaticText41,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer3->Add(txtQueueLastDown,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	StaticBoxSizer3->Add(FlexGridSizer3,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	BoxSizer10->Add(StaticBoxSizer5,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	BoxSizer10->Add(StaticBoxSizer3,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	StaticBoxSizer7 = new wxStaticBoxSizer(wxHORIZONTAL,this,_("Motion"));
	FlexGridSizer6 = new wxFlexGridSizer(0,3,0,0);
	StaticText21 = new wxStaticText(this,ID_STATICTEXT21,_("Speed:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT21"));
	txtSpeed = new wxTextCtrl(this,ID_txtSpeed,_T(""),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_txtSpeed"));
	if ( 0 ) txtSpeed->SetMaxLength(0);
	bSetSpeed = new wxButton(this,ID_bSetSpeed,_("Set"),wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT,wxDefaultValidator,_("ID_bSetSpeed"));
	if (false) bSetSpeed->SetDefault();
	StaticText22 = new wxStaticText(this,ID_STATICTEXT22,_("Accel:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT22"));
	txtAcceleration = new wxTextCtrl(this,ID_txtAcceleration,_T(""),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_txtAcceleration"));
	if ( 0 ) txtAcceleration->SetMaxLength(0);
	bSetAcceleration = new wxButton(this,ID_bSetAcceleration,_("Set"),wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT,wxDefaultValidator,_("ID_bSetAcceleration"));
	if (false) bSetAcceleration->SetDefault();
	StaticText23 = new wxStaticText(this,ID_STATICTEXT23,_("Decel:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT23"));
	txtDeceleration = new wxTextCtrl(this,ID_txtDeceleration,_T(""),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_txtDeceleration"));
	if ( 0 ) txtDeceleration->SetMaxLength(0);
	bSetDeceleration = new wxButton(this,ID_bSetDeceleration,_("Set"),wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT,wxDefaultValidator,_("ID_bSetDeceleration"));
	if (false) bSetDeceleration->SetDefault();
	StaticText24 = new wxStaticText(this,ID_STATICTEXT24,_("OpenSpd:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT24"));
	txtOpenSpeed = new wxTextCtrl(this,ID_txtOpenSpeed,_T(""),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_txtOpenSpeed"));
	if ( 0 ) txtOpenSpeed->SetMaxLength(0);
	bSetOpenSpeed = new wxButton(this,ID_bSetOpenSpeed,_("Set"),wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT,wxDefaultValidator,_("ID_bSetOpenSpeed"));
	if (false) bSetOpenSpeed->SetDefault();
	StaticText25 = new wxStaticText(this,ID_STATICTEXT25,_("DAccel:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT25"));
	txtDoorAcceleration = new wxTextCtrl(this,ID_txtDoorAcceleration,_T(""),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_txtDoorAcceleration"));
	if ( 0 ) txtDoorAcceleration->SetMaxLength(0);
	bSetDoorAccel = new wxButton(this,ID_bSetDoorAccel,_("Set"),wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT,wxDefaultValidator,_("ID_bSetDoorAccel"));
	if (false) bSetDoorAccel->SetDefault();
	StaticText26 = new wxStaticText(this,ID_STATICTEXT26,_("Rate:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT26"));
	txtRate = new wxTextCtrl(this,ID_txtRate,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtRate"));
	if ( 0 ) txtRate->SetMaxLength(0);
	StaticText27 = new wxStaticText(this,ID_STATICTEXT27,_("Dir:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT27"));
	txtDirection = new wxTextCtrl(this,ID_txtDirection,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtDirection"));
	if ( 0 ) txtDirection->SetMaxLength(0);
	StaticText28 = new wxStaticText(this,ID_STATICTEXT28,_("DSpeed:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT28"));
	txtDoorSpeed = new wxTextCtrl(this,ID_txtDoorSpeed,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtDoorSpeed"));
	if ( 0 ) txtDoorSpeed->SetMaxLength(0);
	FlexGridSizer6->Add(StaticText21,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer6->Add(txtSpeed,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer6->Add(bSetSpeed,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer6->Add(StaticText22,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer6->Add(txtAcceleration,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer6->Add(bSetAcceleration,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer6->Add(StaticText23,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer6->Add(txtDeceleration,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer6->Add(bSetDeceleration,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer6->Add(StaticText24,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer6->Add(txtOpenSpeed,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer6->Add(bSetOpenSpeed,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer6->Add(StaticText25,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer6->Add(txtDoorAcceleration,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer6->Add(bSetDoorAccel,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer6->Add(StaticText26,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer6->Add(txtRate,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer6->Add(-1,-1,0);
	FlexGridSizer6->Add(StaticText27,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer6->Add(txtDirection,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer6->Add(-1,-1,0);
	FlexGridSizer6->Add(StaticText28,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer6->Add(txtDoorSpeed,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer6->Add(-1,-1,0);
	StaticBoxSizer7->Add(FlexGridSizer6,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	StaticBoxSizer8 = new wxStaticBoxSizer(wxHORIZONTAL,this,_("Destination"));
	FlexGridSizer7 = new wxFlexGridSizer(0,2,0,0);
	StaticText30 = new wxStaticText(this,ID_STATICTEXT30,_("Floor:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT30"));
	txtDestFloor = new wxTextCtrl(this,ID_txtDestFloor,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtDestFloor"));
	if ( 0 ) txtDestFloor->SetMaxLength(0);
	StaticText31 = new wxStaticText(this,ID_STATICTEXT31,_("Move:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT31"));
	txtMoveElevator = new wxTextCtrl(this,ID_txtMoveElevator,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtMoveElevator"));
	if ( 0 ) txtMoveElevator->SetMaxLength(0);
	StaticText32 = new wxStaticText(this,ID_STATICTEXT32,_("MoveFlr:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT32"));
	txtMoveElevatorFloor = new wxTextCtrl(this,ID_txtMoveElevatorFloor,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtMoveElevatorFloor"));
	if ( 0 ) txtMoveElevatorFloor->SetMaxLength(0);
	StaticText33 = new wxStaticText(this,ID_STATICTEXT33,_("Distance:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT33"));
	txtDistance = new wxTextCtrl(this,ID_txtDistance,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtDistance"));
	if ( 0 ) txtDistance->SetMaxLength(0);
	StaticText34 = new wxStaticText(this,ID_STATICTEXT34,_("Dest:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT34"));
	txtDestination = new wxTextCtrl(this,ID_txtDestination,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtDestination"));
	if ( 0 ) txtDestination->SetMaxLength(0);
	StaticText35 = new wxStaticText(this,ID_STATICTEXT35,_("StopDist:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT35"));
	txtStopDistance = new wxTextCtrl(this,ID_txtStopDistance,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtStopDistance"));
	if ( 0 ) txtStopDistance->SetMaxLength(0);
	StaticText36 = new wxStaticText(this,ID_STATICTEXT36,_("TmpDecel:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT36"));
	txtTempDecel = new wxTextCtrl(this,ID_txtTempDecel,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtTempDecel"));
	if ( 0 ) txtTempDecel->SetMaxLength(0);
	StaticText37 = new wxStaticText(this,ID_STATICTEXT37,_("Error:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT37"));
	txtErrorOffset = new wxTextCtrl(this,ID_txtErrorOffset,_T(""),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_txtErrorOffset"));
	if ( 0 ) txtErrorOffset->SetMaxLength(0);
	FlexGridSizer7->Add(StaticText30,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer7->Add(txtDestFloor,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer7->Add(StaticText31,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer7->Add(txtMoveElevator,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer7->Add(StaticText32,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer7->Add(txtMoveElevatorFloor,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer7->Add(StaticText33,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer7->Add(txtDistance,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer7->Add(StaticText34,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer7->Add(txtDestination,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer7->Add(StaticText35,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer7->Add(txtStopDistance,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer7->Add(StaticText36,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer7->Add(txtTempDecel,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer7->Add(StaticText37,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer7->Add(txtErrorOffset,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	StaticBoxSizer8->Add(FlexGridSizer7,0,wxRIGHT|wxTOP|wxBOTTOM|wxALIGN_LEFT|wxALIGN_TOP,5);
	BoxSizer3->Add(StaticBoxSizer2,0,wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_TOP,5);
	BoxSizer3->Add(BoxSizer10,0,wxRIGHT|wxTOP|wxBOTTOM|wxALIGN_LEFT|wxALIGN_TOP,5);
	BoxSizer3->Add(StaticBoxSizer7,0,wxRIGHT|wxTOP|wxBOTTOM|wxALIGN_LEFT|wxALIGN_TOP,5);
	BoxSizer3->Add(StaticBoxSizer8,0,wxRIGHT|wxTOP|wxBOTTOM|wxALIGN_LEFT|wxALIGN_TOP,5);
	BoxSizer1->Add(BoxSizer2,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	BoxSizer1->Add(BoxSizer3,0,wxALL|wxALIGN_CENTER|wxEXPAND,0);
	this->SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();
	//*)
	OnInit();
}

editelevator::~editelevator()
{
}


void editelevator::On_bCall_Click(wxCommandEvent& event)
{
	//calls elevator to the current camera floor
	if (sbs->GetElevator(sNumber->GetThumbPosition() + 1)->GetFloor() > sbs->camera->CurrentFloor)
		sbs->GetElevator(sNumber->GetThumbPosition() + 1)->AddRoute(sbs->camera->CurrentFloor, -1);
	if (sbs->GetElevator(sNumber->GetThumbPosition() + 1)->GetFloor() < sbs->camera->CurrentFloor)
		sbs->GetElevator(sNumber->GetThumbPosition() + 1)->AddRoute(sbs->camera->CurrentFloor, 1);
}

void editelevator::On_bEnqueueUp_Click(wxCommandEvent& event)
{
	sbs->GetElevator(sNumber->GetThumbPosition() + 1)->AddRoute(sFloor->GetThumbPosition(), 1);
}

void editelevator::On_bGo_Click(wxCommandEvent& event)
{
	sbs->GetElevator(sNumber->GetThumbPosition() + 1)->GotoFloor = sFloor->GetThumbPosition();
	sbs->GetElevator(sNumber->GetThumbPosition() + 1)->MoveElevator = true;
}

void editelevator::On_bEnqueueDown_Click(wxCommandEvent& event)
{
	sbs->GetElevator(sNumber->GetThumbPosition() + 1)->AddRoute(sFloor->GetThumbPosition(), -1);
}

void editelevator::On_bOpen_Click(wxCommandEvent& event)
{
	sbs->GetElevator(sNumber->GetThumbPosition() + 1)->OpenDoors();
}

void editelevator::On_bClose_Click(wxCommandEvent& event)
{
	sbs->GetElevator(sNumber->GetThumbPosition() + 1)->CloseDoors();
}

void editelevator::On_bOpenManual_Click(wxCommandEvent& event)
{
	sbs->GetElevator(sNumber->GetThumbPosition() + 1)->OpenDoorsEmergency();
}

void editelevator::On_bCloseManual_Click(wxCommandEvent& event)
{
	sbs->GetElevator(sNumber->GetThumbPosition() + 1)->CloseDoorsEmergency();
}

void editelevator::On_bStop_Click(wxCommandEvent& event)
{
	sbs->GetElevator(sNumber->GetThumbPosition() + 1)->StopElevator();
}

void editelevator::On_bAlarm_Click(wxCommandEvent& event)
{
	sbs->GetElevator(sNumber->GetThumbPosition() + 1)->Alarm();
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

void editelevator::On_bSetOpenSpeed_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->OpenSpeed = atof(txtOpenSpeed->GetValue().ToAscii());
}

void editelevator::On_bSetDoorAccel_Click(wxCommandEvent& event)
{
	if (elevator)
		elevator->DoorAcceleration = atof(txtDoorAcceleration->GetValue().ToAscii());
}

void editelevator::On_bDumpFloors_Click(wxCommandEvent& event)
{
	sbs->GetElevator(sNumber->GetThumbPosition() + 1)->DumpServicedFloors();
}

void editelevator::On_bDumpQueues_Click(wxCommandEvent& event)
{
	sbs->GetElevator(sNumber->GetThumbPosition() + 1)->DumpQueues();
}

void editelevator::OnInit()
{
	if (sbs->Elevators() > 0)
	{
		//set elevator range slider
		//s_ElevNum->SetRange(1, sbs->Elevators());
		sNumber->SetScrollbar(0, 1, sbs->Elevators(), 1);

		//set floor range slider
		//s_ElevFloor->SetRange(-sbs->Basements, sbs->TotalFloors);
		//sFloor->SetScrollbar(0, 1, sbs->TotalFloors + 1, 1);
		sFloor->SetScrollbar(0, 1, sbs->Floors + 1, 1);
	}
	else
		sNumber->Enable(false);
}

void editelevator::Loop()
{
	static int last_elevator;
	int elev_num;
	elev_num = sNumber->GetThumbPosition() + 1;
	elevator = sbs->GetElevator(elev_num);
	if (elev_num != last_elevator)
	{
		//number changed; update values
		last_elevator = elev_num;
		SetMainValues();
	}
	tElevator->SetLabel(wxT("Number " + wxVariant((long)sNumber->GetThumbPosition() + 1).GetString()));
	tFloor->SetLabel(wxT("Floor " + wxVariant((long)sFloor->GetThumbPosition()).GetString()));
	txtBrakes->SetValue(wxString::FromAscii(BoolToString(elevator->GetBrakeStatus())));
	txtDestFloor->SetValue(wxVariant((long)elevator->GotoFloor).GetString());
	txtDestination->SetValue(wxVariant(RoundFloat(elevator->GetDestination(), 2)).GetString());
	txtDirection->SetValue(wxVariant((long)elevator->Direction).GetString());
	txtDistance->SetValue(wxVariant(RoundFloat(elevator->DistanceToTravel, 2)).GetString());
	txtDoorDirection->SetValue(wxString::FromAscii(BoolToString(elevator->DoorDirection)));
	txtDoorHeight->SetValue(wxVariant(RoundFloat(elevator->DoorHeight, 2)).GetString());
	txtDoorOrigin->SetValue(wxVariant(RoundFloat(elevator->DoorOrigin.x, 2)).GetString() + wxT(", ") + wxVariant(RoundFloat(elevator->DoorOrigin.y, 2)).GetString() + wxT(", ") + wxVariant(RoundFloat(elevator->DoorOrigin.z, 2)).GetString());
	txtDoorsOpen->SetValue(wxString::FromAscii(BoolToString(elevator->AreDoorsOpen())));
	txtDoorSpeed->SetValue(wxVariant(RoundFloat(elevator->GetCurrentDoorSpeed(), 2)).GetString());
	txtDoorWidth->SetValue(wxVariant(RoundFloat(elevator->DoorWidth, 2)).GetString());
	txtElevStart->SetValue(wxVariant(RoundFloat(elevator->GetElevatorStart(), 2)).GetString());
	txtEnabled->SetValue(wxString::FromAscii(BoolToString(elevator->IsEnabled)));
	txtErrorOffset->SetValue(wxVariant(RoundFloat(elevator->ErrorOffset, 2)).GetString());
	txtFloor->SetValue(wxVariant((long)elevator->GetFloor()).GetString());
	txtHeight->SetValue(wxVariant(RoundFloat(elevator->Height, 2)).GetString());
	txtMoveElevator->SetValue(wxString::FromAscii(BoolToString(elevator->MoveElevator)));
	txtMoveElevatorFloor->SetValue(wxVariant((long)elevator->MoveElevatorFloor).GetString());
	txtNumber->SetValue(wxVariant((long)elevator->Number).GetString());
	txtOrigin->SetValue(wxVariant(RoundFloat(elevator->Origin.x, 2)).GetString() + wxT(", ") + wxVariant(RoundFloat(elevator->Origin.y, 2)).GetString() + wxT(", ") + wxVariant(RoundFloat(elevator->Origin.z, 2)).GetString());
	txtOriginFloor->SetValue(wxVariant((long)elevator->OriginFloor).GetString());
	txtPosition->SetValue(wxVariant(RoundFloat(elevator->GetPosition().x, 2)).GetString() + wxT(", ") + wxVariant(RoundFloat(elevator->GetPosition().y, 2)).GetString() + wxT(", ") + wxVariant(RoundFloat(elevator->GetPosition().z, 2)).GetString());
	txtQueueDirection->SetValue(wxVariant((long)elevator->QueuePositionDirection).GetString());
	txtQueueLastDown->SetValue(wxVariant((long)elevator->LastQueueFloor[0]).GetString());
	txtQueueLastUp->SetValue(wxVariant((long)elevator->LastQueueFloor[1]).GetString());
	txtQueuePause->SetValue(wxString::FromAscii(BoolToString(elevator->PauseQueueSearch)));
	txtRate->SetValue(wxVariant(RoundFloat(elevator->ElevatorRate, 2)).GetString());
	txtShaft->SetValue(wxVariant((long)elevator->AssignedShaft).GetString());
	txtShaftDoorOrigin->SetValue(wxVariant(RoundFloat(elevator->ShaftDoorOrigin.x, 2)).GetString() + wxT(", ") + wxVariant(RoundFloat(elevator->ShaftDoorOrigin.y, 2)).GetString() + wxT(", ") + wxVariant(RoundFloat(elevator->ShaftDoorOrigin.z, 2)).GetString());
	txtStop->SetValue(wxString::FromAscii(BoolToString(elevator->GetEmergencyStopStatus())));
	txtStopDistance->SetValue(wxVariant(RoundFloat(elevator->GetStoppingDistance(), 2)).GetString());
	txtTempDecel->SetValue(wxVariant(RoundFloat(elevator->TempDeceleration, 2)).GetString());
}

void editelevator::SetMainValues()
{
	//set changable values
	txtName->SetValue(wxString::FromAscii(elevator->Name.GetData()));
	txtSpeed->SetValue(wxVariant(RoundFloat(elevator->ElevatorSpeed, 4)).GetString());
	txtAcceleration->SetValue(wxVariant(RoundFloat(elevator->Acceleration, 4)).GetString());
	txtDeceleration->SetValue(wxVariant(RoundFloat(elevator->Deceleration, 4)).GetString());
	txtOpenSpeed->SetValue(wxVariant(RoundFloat(elevator->OpenSpeed, 4)).GetString());
	txtDoorAcceleration->SetValue(wxVariant(RoundFloat(elevator->DoorAcceleration, 4)).GetString());
}
