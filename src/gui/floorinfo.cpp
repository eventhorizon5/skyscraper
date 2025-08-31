/*
	Skyscraper 2.1 - Floor Information Dialog
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

//(*InternalHeaders(FloorInfo)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include "globals.h"
#include "sbs.h"
#include "debugpanel.h"
#include "floor.h"
#include "manager.h"
#include "floorinfo.h"

namespace Skyscraper {

//(*IdInit(FloorInfo)
const long FloorInfo::ID_lstFloors = wxNewId();
const long FloorInfo::ID_STATICTEXT1 = wxNewId();
const long FloorInfo::ID_txtNumber = wxNewId();
const long FloorInfo::ID_STATICTEXT2 = wxNewId();
const long FloorInfo::ID_txtName = wxNewId();
const long FloorInfo::ID_STATICTEXT3 = wxNewId();
const long FloorInfo::ID_txtID = wxNewId();
const long FloorInfo::ID_STATICTEXT4 = wxNewId();
const long FloorInfo::ID_txtNumberID = wxNewId();
const long FloorInfo::ID_STATICTEXT5 = wxNewId();
const long FloorInfo::ID_txtFloorType = wxNewId();
const long FloorInfo::ID_STATICTEXT6 = wxNewId();
const long FloorInfo::ID_txtDescription = wxNewId();
const long FloorInfo::ID_STATICTEXT7 = wxNewId();
const long FloorInfo::ID_txtAltitude = wxNewId();
const long FloorInfo::ID_STATICTEXT8 = wxNewId();
const long FloorInfo::ID_txtHeight = wxNewId();
const long FloorInfo::ID_STATICTEXT9 = wxNewId();
const long FloorInfo::ID_txtIntHeight = wxNewId();
const long FloorInfo::ID_bOK = wxNewId();
//*)

BEGIN_EVENT_TABLE(FloorInfo,wxDialog)
    //(*EventTable(FloorInfo)
    //*)
END_EVENT_TABLE()

FloorInfo::FloorInfo(DebugPanel* parent,wxWindowID id)
{
    //(*Initialize(FloorInfo)
    wxFlexGridSizer* FlexGridSizer1;
    wxFlexGridSizer* FlexGridSizer2;
    wxFlexGridSizer* FlexGridSizer3;
    wxFlexGridSizer* FlexGridSizer4;
    wxFlexGridSizer* FlexGridSizer5;

    Create(parent, id, _("Floor Information"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
    FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
    FlexGridSizer4 = new wxFlexGridSizer(0, 3, 0, 0);
    FlexGridSizer2 = new wxFlexGridSizer(0, 3, 0, 0);
    lstFloors = new wxListBox(this, ID_lstFloors, wxDefaultPosition, wxSize(200,400), 0, 0, 0, wxDefaultValidator, _T("ID_lstFloors"));
    FlexGridSizer2->Add(lstFloors, 1, wxALL|wxEXPAND, 5);
    FlexGridSizer4->Add(FlexGridSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3 = new wxFlexGridSizer(0, 3, 0, 0);
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Number:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    FlexGridSizer3->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    txtNumber = new wxTextCtrl(this, ID_txtNumber, wxEmptyString, wxDefaultPosition, wxSize(150,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtNumber"));
    FlexGridSizer3->Add(txtNumber, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Name:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    FlexGridSizer3->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    txtName = new wxTextCtrl(this, ID_txtName, wxEmptyString, wxDefaultPosition, wxSize(150,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtName"));
    FlexGridSizer3->Add(txtName, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("ID:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    FlexGridSizer3->Add(StaticText3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    txtID = new wxTextCtrl(this, ID_txtID, wxEmptyString, wxDefaultPosition, wxSize(150,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtID"));
    FlexGridSizer3->Add(txtID, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Number ID:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    FlexGridSizer3->Add(StaticText4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    txtNumberID = new wxTextCtrl(this, ID_txtNumberID, wxEmptyString, wxDefaultPosition, wxSize(150,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtNumberID"));
    FlexGridSizer3->Add(txtNumberID, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Floor Type:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    FlexGridSizer3->Add(StaticText5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    txtFloorType = new wxTextCtrl(this, ID_txtFloorType, wxEmptyString, wxDefaultPosition, wxSize(150,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtFloorType"));
    FlexGridSizer3->Add(txtFloorType, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("Description:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
    FlexGridSizer3->Add(StaticText6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    txtDescription = new wxTextCtrl(this, ID_txtDescription, wxEmptyString, wxDefaultPosition, wxSize(150,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtDescription"));
    FlexGridSizer3->Add(txtDescription, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Altitude:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
    FlexGridSizer3->Add(StaticText7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    txtAltitude = new wxTextCtrl(this, ID_txtAltitude, wxEmptyString, wxDefaultPosition, wxSize(150,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtAltitude"));
    FlexGridSizer3->Add(txtAltitude, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("Height:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
    FlexGridSizer3->Add(StaticText8, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    txtHeight = new wxTextCtrl(this, ID_txtHeight, wxEmptyString, wxDefaultPosition, wxSize(150,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtHeight"));
    FlexGridSizer3->Add(txtHeight, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _("Interfloor Height:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
    FlexGridSizer3->Add(StaticText9, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    txtIntHeight = new wxTextCtrl(this, ID_txtIntHeight, wxEmptyString, wxDefaultPosition, wxSize(150,-1), wxTE_READONLY, wxDefaultValidator, _T("ID_txtIntHeight"));
    FlexGridSizer3->Add(txtIntHeight, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer4->Add(FlexGridSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(FlexGridSizer4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer5 = new wxFlexGridSizer(0, 3, 0, 0);
    bOK = new wxButton(this, ID_bOK, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bOK"));
    FlexGridSizer5->Add(bOK, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(FlexGridSizer5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(FlexGridSizer1);
    FlexGridSizer1->SetSizeHints(this);

    Connect(ID_bOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FloorInfo::On_bOK_Click);
    //*)
    lastcount = 0;
	Simcore = 0;
	panel = parent;
	floor = 0;
}

FloorInfo::~FloorInfo()
{
    //(*Destroy(FloorInfo)
    //*)
}

void FloorInfo::Loop()
{
	if (Simcore != panel->GetSystem())
	{
		//if active engine has changed, refresh values
		Simcore = panel->GetSystem();
	}

	if (!Simcore)
		return;

	BuildList();

	int selection = lstFloors->GetSelection();

	if (selection >= 0)
	{
		SBS::Floor *newfloor = Simcore->GetFloorManager()->GetIndex(selection);

		//if a new floor has been selected, update values
		if (newfloor && floor != newfloor)
		{
			floor = newfloor;
            txtNumber->SetValue(SBS::ToString(floor->Number));
			txtName->SetValue(floor->Name);
            txtID->SetValue(floor->ID);
            txtNumberID->SetValue(floor->NumberID);
            txtFloorType->SetValue(floor->FloorType);
            txtDescription->SetValue(floor->Description);
            txtAltitude->SetValue(SBS::ToString(floor->Altitude));
            txtHeight->SetValue(SBS::ToString(floor->Height));
            txtIntHeight->SetValue(SBS::ToString(floor->InterfloorHeight));
		}
	}
	else
		floor = 0;

	if (!floor)
		return;
}

void FloorInfo::BuildList(bool restore_selection)
{
	int count = Simcore->GetTotalFloors();

	if (count != lastcount)
	{
		lastcount = count;
		int old_selection = lstFloors->GetSelection();
		lstFloors->Clear();

		for (int i = 0; i < Simcore->GetFloorManager()->GetCount(); i++)
		{
			::SBS::Floor *floor = Simcore->GetFloorManager()->GetIndex(i);
			if (floor)
				lstFloors->Append(SBS::ToString(floor->Number) + wxT(": ") + floor->Name);
		}

		if (count > 0)
		{
			if (restore_selection == false)
				lstFloors->SetSelection(0);
			else
				lstFloors->SetSelection(old_selection);
		}
		else
		{
			//clear values
            txtNumber->SetValue(wxT(""));
			txtName->SetValue(wxT(""));
            txtID->SetValue(wxT(""));
            txtNumberID->SetValue(wxT(""));
            txtFloorType->SetValue(wxT(""));
            txtDescription->SetValue(wxT(""));
            txtAltitude->SetValue(wxT(""));
            txtHeight->SetValue(wxT(""));
            txtIntHeight->SetValue(wxT(""));
		}
	}
}

void FloorInfo::On_bOK_Click(wxCommandEvent& event)
{
    this->Close();
}

}
