/*
    Skyscraper 2.1 - Controller Editor
    Copyright (C)2004-2025 Ryan Thoryk
    https://www.skyscrapersim.net
    https://sourceforge.net/projects/skyscraper/
    Contact - ryan@skyscrapersim.net

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

//(*InternalHeaders(ControllerEditor)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include "globals.h"
#include "sbs.h"
#include "debugpanel.h"
#include "elevator.h"
#include "callstation.h"
#include "controller.h"
#include "controllereditor.h"

namespace Skyscraper
{

//(*IdInit(ControllerEditor)
const long ControllerEditor::ID_lControllers = wxNewId();
const long ControllerEditor::ID_tDD = wxNewId();
const long ControllerEditor::ID_tHybrid = wxNewId();
const long ControllerEditor::ID_STATICTEXT3 = wxNewId();
const long ControllerEditor::ID_tRange = wxNewId();
const long ControllerEditor::ID_STATICTEXT4 = wxNewId();
const long ControllerEditor::ID_tMaxPassengers = wxNewId();
const long ControllerEditor::ID_STATICTEXT5 = wxNewId();
const long ControllerEditor::ID_tReprocess = wxNewId();
const long ControllerEditor::ID_bSetReprocess = wxNewId();
const long ControllerEditor::ID_STATICTEXT6 = wxNewId();
const long ControllerEditor::ID_sFireService = wxNewId();
const long ControllerEditor::ID_STATICTEXT7 = wxNewId();
const long ControllerEditor::ID_tRecallFloor = wxNewId();
const long ControllerEditor::ID_STATICTEXT8 = wxNewId();
const long ControllerEditor::ID_tBottomFloor = wxNewId();
const long ControllerEditor::ID_STATICTEXT9 = wxNewId();
const long ControllerEditor::ID_tTopFloor = wxNewId();
const long ControllerEditor::ID_lElevators = wxNewId();
const long ControllerEditor::ID_bRemoveElevator = wxNewId();
const long ControllerEditor::ID_bResetArrival = wxNewId();
const long ControllerEditor::ID_tAddElevator = wxNewId();
const long ControllerEditor::ID_bAddElevator = wxNewId();
const long ControllerEditor::ID_STATICLINE1 = wxNewId();
const long ControllerEditor::ID_STATICLINE2 = wxNewId();
const long ControllerEditor::ID_STATICLINE3 = wxNewId();
const long ControllerEditor::ID_bCallUp = wxNewId();
const long ControllerEditor::ID_bCall = wxNewId();
const long ControllerEditor::ID_lCallStations = wxNewId();
const long ControllerEditor::ID_STATICTEXT1 = wxNewId();
const long ControllerEditor::ID_tStationFloor = wxNewId();
const long ControllerEditor::ID_STATICTEXT2 = wxNewId();
const long ControllerEditor::ID_tUpStatus = wxNewId();
const long ControllerEditor::ID_STATICTEXT17 = wxNewId();
const long ControllerEditor::ID_tDownStatus = wxNewId();
const long ControllerEditor::ID_bPressUp = wxNewId();
const long ControllerEditor::ID_bPressDown = wxNewId();
const long ControllerEditor::ID_lRoutes = wxNewId();
const long ControllerEditor::ID_STATICTEXT10 = wxNewId();
const long ControllerEditor::ID_tStartingFloor = wxNewId();
const long ControllerEditor::ID_STATICTEXT11 = wxNewId();
const long ControllerEditor::ID_tDestFloor = wxNewId();
const long ControllerEditor::ID_STATICTEXT12 = wxNewId();
const long ControllerEditor::ID_tDirection = wxNewId();
const long ControllerEditor::ID_STATICTEXT13 = wxNewId();
const long ControllerEditor::ID_tRequests = wxNewId();
const long ControllerEditor::ID_STATICTEXT14 = wxNewId();
const long ControllerEditor::ID_tProcessed = wxNewId();
const long ControllerEditor::ID_STATICTEXT15 = wxNewId();
const long ControllerEditor::ID_tRouteElevator = wxNewId();
const long ControllerEditor::ID_STATICTEXT16 = wxNewId();
const long ControllerEditor::ID_tDestination = wxNewId();
//*)

BEGIN_EVENT_TABLE(ControllerEditor,wxDialog)
    //(*EventTable(ControllerEditor)
    //*)
END_EVENT_TABLE()

ControllerEditor::ControllerEditor(DebugPanel* parent,wxWindowID id)
{
    //(*Initialize(ControllerEditor)
    wxFlexGridSizer* FlexGridSizer1;
    wxFlexGridSizer* FlexGridSizer2;
    wxFlexGridSizer* FlexGridSizer3;
    wxFlexGridSizer* FlexGridSizer4;
    wxFlexGridSizer* FlexGridSizer5;
    wxFlexGridSizer* FlexGridSizer6;
    wxStaticBoxSizer* StaticBoxSizer1;
    wxStaticBoxSizer* StaticBoxSizer2;
    wxStaticBoxSizer* StaticBoxSizer3;
    wxStaticBoxSizer* StaticBoxSizer4;

    Create(parent, wxID_ANY, _("Controller Editor"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
    FlexGridSizer1 = new wxFlexGridSizer(0, 3, 0, 0);
    FlexGridSizer2 = new wxFlexGridSizer(0, 4, 0, 0);
    StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, this, _("Controllers"));
    lControllers = new wxListBox(this, ID_lControllers, wxDefaultPosition, wxSize(200,200), 0, 0, 0, wxDefaultValidator, _T("ID_lControllers"));
    StaticBoxSizer1->Add(lControllers, 1, wxALL|wxEXPAND, 5);
    FlexGridSizer3 = new wxFlexGridSizer(0, 3, 0, 0);
    FlexGridSizer3->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tDD = new wxToggleButton(this, ID_tDD, _("Destination Dispatch"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_tDD"));
    FlexGridSizer3->Add(tDD, 1, wxEXPAND, 5);
    FlexGridSizer3->Add(-1,-1,1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tHybrid = new wxToggleButton(this, ID_tHybrid, _("Hybrid"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_tHybrid"));
    FlexGridSizer3->Add(tHybrid, 1, wxEXPAND, 5);
    FlexGridSizer3->Add(-1,-1,1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Range:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    FlexGridSizer3->Add(StaticText3, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tRange = new wxTextCtrl(this, ID_tRange, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_tRange"));
    FlexGridSizer3->Add(tRange, 1, wxEXPAND, 5);
    FlexGridSizer3->Add(-1,-1,1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Max Passengers:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    FlexGridSizer3->Add(StaticText4, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tMaxPassengers = new wxTextCtrl(this, ID_tMaxPassengers, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_tMaxPassengers"));
    FlexGridSizer3->Add(tMaxPassengers, 1, wxEXPAND, 5);
    FlexGridSizer3->Add(-1,-1,1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Reprocess:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    FlexGridSizer3->Add(StaticText5, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tReprocess = new wxTextCtrl(this, ID_tReprocess, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_tReprocess"));
    FlexGridSizer3->Add(tReprocess, 1, wxEXPAND, 5);
    bSetReprocess = new wxButton(this, ID_bSetReprocess, _("Set"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bSetReprocess"));
    FlexGridSizer3->Add(bSetReprocess, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("Fire Service:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
    FlexGridSizer3->Add(StaticText6, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    sFireService = new wxSlider(this, ID_sFireService, 0, 0, 2, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_sFireService"));
    FlexGridSizer3->Add(sFireService, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3->Add(-1,-1,1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Recall Floor:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
    FlexGridSizer3->Add(StaticText7, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tRecallFloor = new wxTextCtrl(this, ID_tRecallFloor, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_tRecallFloor"));
    FlexGridSizer3->Add(tRecallFloor, 1, wxEXPAND, 5);
    FlexGridSizer3->Add(-1,-1,1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("Bottom Floor:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
    FlexGridSizer3->Add(StaticText8, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tBottomFloor = new wxTextCtrl(this, ID_tBottomFloor, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_tBottomFloor"));
    FlexGridSizer3->Add(tBottomFloor, 1, wxEXPAND, 5);
    FlexGridSizer3->Add(-1,-1,1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _("Top Floor:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
    FlexGridSizer3->Add(StaticText9, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tTopFloor = new wxTextCtrl(this, ID_tTopFloor, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_tTopFloor"));
    FlexGridSizer3->Add(tTopFloor, 1, wxEXPAND, 5);
    FlexGridSizer3->Add(-1,-1,1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer1->Add(FlexGridSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer2->Add(StaticBoxSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer2 = new wxStaticBoxSizer(wxVERTICAL, this, _("Elevators"));
    lElevators = new wxListBox(this, ID_lElevators, wxDefaultPosition, wxSize(200,200), 0, 0, 0, wxDefaultValidator, _T("ID_lElevators"));
    StaticBoxSizer2->Add(lElevators, 1, wxALL|wxEXPAND, 5);
    FlexGridSizer4 = new wxFlexGridSizer(0, 3, 0, 0);
    FlexGridSizer4->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer4->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    bRemoveElevator = new wxButton(this, ID_bRemoveElevator, _("Remove Elevator"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bRemoveElevator"));
    FlexGridSizer4->Add(bRemoveElevator, 1, wxALL|wxEXPAND, 5);
    FlexGridSizer4->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer4->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    bResetArrival = new wxButton(this, ID_bResetArrival, _("Reset Arrival"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bResetArrival"));
    FlexGridSizer4->Add(bResetArrival, 1, wxALL|wxEXPAND, 5);
    FlexGridSizer4->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tAddElevator = new wxTextCtrl(this, ID_tAddElevator, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_tAddElevator"));
    FlexGridSizer4->Add(tAddElevator, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    bAddElevator = new wxButton(this, ID_bAddElevator, _("Add Elevator"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bAddElevator"));
    FlexGridSizer4->Add(bAddElevator, 1, wxALL|wxEXPAND, 5);
    StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
    FlexGridSizer4->Add(StaticLine1, 1, wxEXPAND, 5);
    StaticLine2 = new wxStaticLine(this, ID_STATICLINE2, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE2"));
    FlexGridSizer4->Add(StaticLine2, 1, wxEXPAND, 5);
    StaticLine3 = new wxStaticLine(this, ID_STATICLINE3, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE3"));
    FlexGridSizer4->Add(StaticLine3, 1, wxEXPAND, 5);
    FlexGridSizer4->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer4->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer4->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer4->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    bCallUp = new wxButton(this, ID_bCallUp, _("Call Up"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bCallUp"));
    FlexGridSizer4->Add(bCallUp, 1, wxALL|wxEXPAND, 5);
    bCall = new wxButton(this, ID_bCall, _("Call Down"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bCall"));
    FlexGridSizer4->Add(bCall, 1, wxALL|wxEXPAND, 5);
    StaticBoxSizer2->Add(FlexGridSizer4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer2->Add(StaticBoxSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer3 = new wxStaticBoxSizer(wxVERTICAL, this, _("Call Stations"));
    lCallStations = new wxListBox(this, ID_lCallStations, wxDefaultPosition, wxSize(200,200), 0, 0, 0, wxDefaultValidator, _T("ID_lCallStations"));
    StaticBoxSizer3->Add(lCallStations, 1, wxALL|wxEXPAND, 5);
    FlexGridSizer6 = new wxFlexGridSizer(0, 2, 0, 0);
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Floor:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    FlexGridSizer6->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tStationFloor = new wxTextCtrl(this, ID_tStationFloor, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_tStationFloor"));
    FlexGridSizer6->Add(tStationFloor, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Up Status:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    FlexGridSizer6->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tUpStatus = new wxTextCtrl(this, ID_tUpStatus, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_tUpStatus"));
    FlexGridSizer6->Add(tUpStatus, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText17 = new wxStaticText(this, ID_STATICTEXT17, _("Down Status:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT17"));
    FlexGridSizer6->Add(StaticText17, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tDownStatus = new wxTextCtrl(this, ID_tDownStatus, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_tDownStatus"));
    FlexGridSizer6->Add(tDownStatus, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer6->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    bPressUp = new wxButton(this, ID_bPressUp, _("Press Up"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bPressUp"));
    FlexGridSizer6->Add(bPressUp, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer6->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    bPressDown = new wxButton(this, ID_bPressDown, _("Press Down"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bPressDown"));
    FlexGridSizer6->Add(bPressDown, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer3->Add(FlexGridSizer6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer2->Add(StaticBoxSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer4 = new wxStaticBoxSizer(wxVERTICAL, this, _("Routes"));
    lRoutes = new wxListBox(this, ID_lRoutes, wxDefaultPosition, wxSize(200,200), 0, 0, 0, wxDefaultValidator, _T("ID_lRoutes"));
    StaticBoxSizer4->Add(lRoutes, 1, wxALL|wxEXPAND, 5);
    FlexGridSizer5 = new wxFlexGridSizer(0, 2, 0, 0);
    StaticText10 = new wxStaticText(this, ID_STATICTEXT10, _("Starting Floor:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
    FlexGridSizer5->Add(StaticText10, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tStartingFloor = new wxTextCtrl(this, ID_tStartingFloor, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_tStartingFloor"));
    FlexGridSizer5->Add(tStartingFloor, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText11 = new wxStaticText(this, ID_STATICTEXT11, _("Destination Floor:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
    FlexGridSizer5->Add(StaticText11, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tDestFloor = new wxTextCtrl(this, ID_tDestFloor, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_tDestFloor"));
    FlexGridSizer5->Add(tDestFloor, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText12 = new wxStaticText(this, ID_STATICTEXT12, _("Direction:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT12"));
    FlexGridSizer5->Add(StaticText12, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tDirection = new wxTextCtrl(this, ID_tDirection, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_tDirection"));
    FlexGridSizer5->Add(tDirection, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText13 = new wxStaticText(this, ID_STATICTEXT13, _("Requests:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT13"));
    FlexGridSizer5->Add(StaticText13, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tRequests = new wxTextCtrl(this, ID_tRequests, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_tRequests"));
    FlexGridSizer5->Add(tRequests, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText14 = new wxStaticText(this, ID_STATICTEXT14, _("Processed:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT14"));
    FlexGridSizer5->Add(StaticText14, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tProcessed = new wxTextCtrl(this, ID_tProcessed, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_tProcessed"));
    FlexGridSizer5->Add(tProcessed, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText15 = new wxStaticText(this, ID_STATICTEXT15, _("Assigned Elevator:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT15"));
    FlexGridSizer5->Add(StaticText15, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tRouteElevator = new wxTextCtrl(this, ID_tRouteElevator, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_tRouteElevator"));
    FlexGridSizer5->Add(tRouteElevator, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText16 = new wxStaticText(this, ID_STATICTEXT16, _("Destination:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT16"));
    FlexGridSizer5->Add(StaticText16, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tDestination = new wxTextCtrl(this, ID_tDestination, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_tDestination"));
    FlexGridSizer5->Add(tDestination, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer4->Add(FlexGridSizer5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer2->Add(StaticBoxSizer4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(FlexGridSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(FlexGridSizer1);
    FlexGridSizer1->SetSizeHints(this);

    Connect(ID_tDD,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&ControllerEditor::On_tDD_Toggle);
    Connect(ID_tHybrid,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&ControllerEditor::On_tHybrid_Toggle);
    Connect(ID_bRemoveElevator,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ControllerEditor::On_bRemoveElevator_Click);
    Connect(ID_bResetArrival,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ControllerEditor::On_bResetArrival_Click);
    Connect(ID_bAddElevator,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ControllerEditor::On_bAddElevator_Click);
    Connect(ID_bCallUp,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ControllerEditor::On_bCallUp_Click);
    Connect(ID_bCall,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ControllerEditor::On_bCall_Click);
    Connect(ID_bPressUp,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ControllerEditor::On_bPressUp_Click);
    Connect(ID_bPressDown,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ControllerEditor::On_bPressDown_Click);
    //*)
    lastcount = 0;
    lastcount_elev = 0;
    lastcount_station = 0;
    lastcount_route = 0;
    Simcore = 0;
    panel = parent;
    controller = 0;
    full_update = false;
}

ControllerEditor::~ControllerEditor()
{
    //(*Destroy(ControllerEditor)
    //*)
}

void ControllerEditor::Loop()
{
    if (Simcore != panel->GetSystem())
    {
        //if active engine has changed, refresh values
        Simcore = panel->GetSystem();
    }

    if (!Simcore)
        return;

    BuildList();

    int selection = lControllers->GetSelection() + 1;

    if (selection > 0)
    {
        SBS::DispatchController* newcontroller = Simcore->GetController(selection);

        //if a new controller has been selected, update values
        if (newcontroller && controller != newcontroller)
        {
            controller = Simcore->GetController(selection);
            tDD->SetValue(controller->DestinationDispatch);
            tHybrid->SetValue(controller->Hybrid);
            tRange->SetValue(SBS::ToString(controller->Range));
            tMaxPassengers->SetValue(SBS::ToString(controller->MaxPassengers));
            tReprocess->SetValue(SBS::BoolToString(controller->Reprocess));
            sFireService->SetValue(0);
            tRecallFloor->SetValue(SBS::ToString(controller->GetRecallFloor()));
            tBottomFloor->SetValue(SBS::ToString(controller->GetBottomFloor()));
            tTopFloor->SetValue(SBS::ToString(controller->GetTopFloor()));
            full_update = true;
        }
        else
            full_update = false;
    }
    else
        controller = 0;

    if (!controller)
        return;

    BuildElevatorList();
    BuildStationList();

    selection = lCallStations->GetSelection();

    if (selection >= 0)
    {
        SBS::CallStation* station = controller->GetCallStation(selection);

        if (station)
        {
            tStationFloor->SetValue(SBS::ToString(station->GetFloor()));
            tUpStatus->SetValue(SBS::BoolToString(station->GetUpStatus()));
            tDownStatus->SetValue(SBS::BoolToString(station->GetDownStatus()));
        }
    }

    BuildRouteList();

    selection = lRoutes->GetSelection();

    if (selection >= 0)
    {
        SBS::DispatchController::Route route;
        bool result = controller->GetRoute(selection, route);

        if (result == true)
        {
            tStartingFloor->SetValue(SBS::ToString(route.starting_floor));
            tDestFloor->SetValue(SBS::ToString(route.destination_floor));
            tDirection->SetValue(SBS::BoolToString(route.direction));
            tRequests->SetValue(SBS::ToString(route.requests));
            tProcessed->SetValue(SBS::BoolToString(route.processed));
            tRouteElevator->SetValue(SBS::ToString(route.assigned_elevator));
            tDestination->SetValue(SBS::ToString(route.destination));
        }
    }
}

void ControllerEditor::BuildList(bool restore_selection)
{
    int count = Simcore->GetControllerCount();

    if (count != lastcount)
    {
        lastcount = count;
        int old_selection = lControllers->GetSelection();
        lControllers->Clear();

        for (int i = 0; i < count; i++)
        {
            ::SBS::DispatchController* controller = Simcore->GetController(i + 1);
            if (controller)
                lControllers->Append(SBS::ToString(i + 1) + wxT(": ") + controller->GetName());
        }

        if (count > 0)
        {
            if (restore_selection == false)
                lControllers->SetSelection(0);
            else
                lControllers->SetSelection(old_selection);
        }
        else
        {
            //clear values
            tDD->SetValue(wxT(""));
            tHybrid->SetValue(wxT(""));
            tRange->SetValue(wxT(""));
            tMaxPassengers->SetValue(wxT(""));
            tReprocess->SetValue(wxT(""));
            sFireService->SetValue(0);
            tRecallFloor->SetValue(wxT(""));
            tBottomFloor->SetValue(wxT(""));
            tTopFloor->SetValue(wxT(""));
        }
    }
}

void ControllerEditor::BuildElevatorList(bool restore_selection)
{
    if (!controller)
        return;

    int count = controller->GetElevatorCount();

    if (count != lastcount_elev || full_update == true)
    {
        lastcount_elev = count;
        int old_selection = lElevators->GetSelection();
        lElevators->Clear();

        for (int i = 0; i < count; i++)
        {
            ::SBS::Elevator* elevator = Simcore->GetElevator(controller->GetElevator(i));
            if (elevator)
                lElevators->Append(SBS::ToString(i + 1) + wxT(": ") + elevator->GetName());
        }

        if (count > 0)
        {
            if (restore_selection == false)
                lElevators->SetSelection(0);
            else
                lElevators->SetSelection(old_selection);
        }
        else
        {
            //clear values
        }
    }
}

void ControllerEditor::BuildStationList(bool restore_selection)
{
    if (!controller)
        return;

    int count = controller->GetCallStationCount();

    if (count != lastcount_station || full_update == true)
    {
        lastcount_station = count;
        int old_selection = lCallStations->GetSelection();
        lCallStations->Clear();

        for (int i = 0; i < count; i++)
        {
            ::SBS::CallStation* station = controller->GetCallStation(i);
            if (station)
                lCallStations->Append(SBS::ToString(i + 1) + wxT(": ") + station->GetName());
        }

        if (count > 0)
        {
            if (restore_selection == false)
                lCallStations->SetSelection(0);
            else
                lCallStations->SetSelection(old_selection);
        }
        else
        {
            //clear values
        }
    }
}

void ControllerEditor::BuildRouteList(bool restore_selection)
{
    if (!controller)
        return;

    int count = controller->GetRouteCount();

    if (count != lastcount_route || full_update == true)
    {
        lastcount_route = count;
        int old_selection = lRoutes->GetSelection();
        lRoutes->Clear();

        for (int i = 0; i < count; i++)
        {
            ::SBS::DispatchController::Route route;
            bool result = controller->GetRoute(i, route);
            if (result == true)
                lRoutes->Append(SBS::ToString(i + 1));
        }

        if (count > 0)
        {
            if (restore_selection == false)
                lRoutes->SetSelection(0);
            else
                lRoutes->SetSelection(old_selection);
        }
        else
        {
            //clear values
            tStartingFloor->SetValue(wxT(""));
            tDestFloor->SetValue(wxT(""));
            tDirection->SetValue(wxT(""));
            tRequests->SetValue(wxT(""));
            tProcessed->SetValue(wxT(""));
            tRouteElevator->SetValue(wxT(""));
            tDestination->SetValue(wxT(""));
        }
    }
}

void ControllerEditor::On_bAddElevator_Click(wxCommandEvent& event)
{
    if (!controller)
        return;

    controller->AddElevator(SBS::ToInt(tAddElevator->GetValue().ToStdString()));
}


void ControllerEditor::On_bRemoveElevator_Click(wxCommandEvent& event)
{
    if (!controller)
        return;

    controller->RemoveElevatorIndex(lElevators->GetSelection());
}

void ControllerEditor::On_bResetArrival_Click(wxCommandEvent& event)
{
    if (!controller)
        return;

    controller->ResetArrival(SBS::ToInt(tAddElevator->GetValue().ToStdString()));
}

void ControllerEditor::On_bCall_Click(wxCommandEvent& event)
{
    //call down
    if (!controller)
        return;

    ::SBS::CallStation* station = controller->GetCallStation(lCallStations->GetSelection());

    controller->CallElevator(station, false);
}

void ControllerEditor::On_bCallUp_Click(wxCommandEvent& event)
{
    //call up
    if (!controller)
        return;

    ::SBS::CallStation* station = controller->GetCallStation(lCallStations->GetSelection());

    controller->CallElevator(station, true);
}

void ControllerEditor::On_bPressUp_Click(wxCommandEvent& event)
{
    //press up
    if (!controller)
        return;

    ::SBS::CallStation* station = controller->GetCallStation(lCallStations->GetSelection());

    if (station)
        station->Call(true);
}

void ControllerEditor::On_bPressDown_Click(wxCommandEvent& event)
{
    //press down
    if (!controller)
        return;

    ::SBS::CallStation* station = controller->GetCallStation(lCallStations->GetSelection());

    if (station)
        station->Call(false);
}

void ControllerEditor::On_tDD_Toggle(wxCommandEvent& event)
{
    //change destination dispatch status on controller
    if (!controller)
        return;

    controller->DestinationDispatch = tDD->GetValue();
}

void ControllerEditor::On_tHybrid_Toggle(wxCommandEvent& event)
{
    //change hybrid status on controller
    if (!controller)
        return;

    controller->Hybrid = tHybrid->GetValue();
}

}
