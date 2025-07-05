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
#include "controllereditor.h"

namespace Skyscraper
{

//(*IdInit(ControllerEditor)
const wxWindowID ControllerEditor::ID_lControllers = wxNewId();
const wxWindowID ControllerEditor::ID_STATICTEXT1 = wxNewId();
const wxWindowID ControllerEditor::ID_tDD = wxNewId();
const wxWindowID ControllerEditor::ID_STATICTEXT2 = wxNewId();
const wxWindowID ControllerEditor::ID_tHybrid = wxNewId();
const wxWindowID ControllerEditor::ID_STATICTEXT3 = wxNewId();
const wxWindowID ControllerEditor::ID_tRange = wxNewId();
const wxWindowID ControllerEditor::ID_STATICTEXT4 = wxNewId();
const wxWindowID ControllerEditor::ID_tMaxPassengers = wxNewId();
const wxWindowID ControllerEditor::ID_STATICTEXT5 = wxNewId();
const wxWindowID ControllerEditor::ID_tReprocess = wxNewId();
const wxWindowID ControllerEditor::ID_bSetReprocess = wxNewId();
const wxWindowID ControllerEditor::ID_STATICTEXT6 = wxNewId();
const wxWindowID ControllerEditor::ID_sFireService = wxNewId();
const wxWindowID ControllerEditor::ID_STATICTEXT7 = wxNewId();
const wxWindowID ControllerEditor::ID_tRecallFloor = wxNewId();
const wxWindowID ControllerEditor::ID_STATICTEXT8 = wxNewId();
const wxWindowID ControllerEditor::ID_tBottomFloor = wxNewId();
const wxWindowID ControllerEditor::ID_STATICTEXT9 = wxNewId();
const wxWindowID ControllerEditor::ID_tTopFloor = wxNewId();
const wxWindowID ControllerEditor::ID_lElevators = wxNewId();
const wxWindowID ControllerEditor::ID_lCallStations = wxNewId();
const wxWindowID ControllerEditor::ID_lRoutes = wxNewId();
//*)

BEGIN_EVENT_TABLE(ControllerEditor,wxDialog)
    //(*EventTable(ControllerEditor)
    //*)
END_EVENT_TABLE()

ControllerEditor::ControllerEditor(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(ControllerEditor)
    wxFlexGridSizer* FlexGridSizer1;
    wxFlexGridSizer* FlexGridSizer2;
    wxFlexGridSizer* FlexGridSizer3;
    wxStaticBoxSizer* StaticBoxSizer1;
    wxStaticBoxSizer* StaticBoxSizer2;
    wxStaticBoxSizer* StaticBoxSizer3;
    wxStaticBoxSizer* StaticBoxSizer4;

    Create(parent, id, _("Controller Editor"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
    FlexGridSizer1 = new wxFlexGridSizer(0, 3, 0, 0);
    FlexGridSizer2 = new wxFlexGridSizer(0, 4, 0, 0);
    StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, this, _("Controllers"));
    lControllers = new wxListBox(this, ID_lControllers, wxDefaultPosition, wxSize(200,200), 0, 0, 0, wxDefaultValidator, _T("ID_lControllers"));
    StaticBoxSizer1->Add(lControllers, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3 = new wxFlexGridSizer(0, 3, 0, 0);
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("DD Status:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    FlexGridSizer3->Add(StaticText1, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tDD = new wxTextCtrl(this, ID_tDD, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_tDD"));
    FlexGridSizer3->Add(tDD, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3->Add(-1,-1,1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Hybrid:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    FlexGridSizer3->Add(StaticText2, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tHybrid = new wxTextCtrl(this, ID_tHybrid, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_tHybrid"));
    FlexGridSizer3->Add(tHybrid, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3->Add(-1,-1,1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Range:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    FlexGridSizer3->Add(StaticText3, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tRange = new wxTextCtrl(this, ID_tRange, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_tRange"));
    FlexGridSizer3->Add(tRange, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3->Add(-1,-1,1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Max Passengers:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    FlexGridSizer3->Add(StaticText4, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tMaxPassengers = new wxTextCtrl(this, ID_tMaxPassengers, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_tMaxPassengers"));
    FlexGridSizer3->Add(tMaxPassengers, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3->Add(-1,-1,1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Reprocess:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    FlexGridSizer3->Add(StaticText5, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tReprocess = new wxTextCtrl(this, ID_tReprocess, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_tReprocess"));
    FlexGridSizer3->Add(tReprocess, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
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
    FlexGridSizer3->Add(tRecallFloor, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3->Add(-1,-1,1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("Bottom Floor:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
    FlexGridSizer3->Add(StaticText8, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tBottomFloor = new wxTextCtrl(this, ID_tBottomFloor, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_tBottomFloor"));
    FlexGridSizer3->Add(tBottomFloor, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3->Add(-1,-1,1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _("Top Floor:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
    FlexGridSizer3->Add(StaticText9, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tTopFloor = new wxTextCtrl(this, ID_tTopFloor, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_tTopFloor"));
    FlexGridSizer3->Add(tTopFloor, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3->Add(-1,-1,1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer1->Add(FlexGridSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer2->Add(StaticBoxSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer2 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Elevators"));
    lElevators = new wxListBox(this, ID_lElevators, wxDefaultPosition, wxSize(200,200), 0, 0, 0, wxDefaultValidator, _T("ID_lElevators"));
    StaticBoxSizer2->Add(lElevators, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer2->Add(StaticBoxSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer3 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Call Stations"));
    lCallStations = new wxListBox(this, ID_lCallStations, wxDefaultPosition, wxSize(200,200), 0, 0, 0, wxDefaultValidator, _T("ID_lCallStations"));
    StaticBoxSizer3->Add(lCallStations, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer2->Add(StaticBoxSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer4 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Routes"));
    lRoutes = new wxListBox(this, ID_lRoutes, wxDefaultPosition, wxSize(200,200), 0, 0, 0, wxDefaultValidator, _T("ID_lRoutes"));
    StaticBoxSizer4->Add(lRoutes, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer2->Add(StaticBoxSizer4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(FlexGridSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(FlexGridSizer1);
    FlexGridSizer1->SetSizeHints(this);
    //*)
}

ControllerEditor::~ControllerEditor()
{
    //(*Destroy(ControllerEditor)
    //*)
}

void ControllerEditor::Loop()
{

}

}
