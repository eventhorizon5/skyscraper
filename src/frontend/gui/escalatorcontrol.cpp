/*
	Skyscraper 2.1 - Escalator Control
	Copyright (C)2004-2024 Ryan Thoryk
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

//(*InternalHeaders(EscalatorControl)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include "globals.h"
#include "sbs.h"
#include "skyscraper.h"
#include "debugpanel.h"
#include "escalator.h"
#include "escalatorcontrol.h"

namespace Skyscraper {

//(*IdInit(EscalatorControl)
const long EscalatorControl::ID_LISTBOX1 = wxNewId();
const long EscalatorControl::ID_STATICTEXT1 = wxNewId();
const long EscalatorControl::ID_txtName = wxNewId();
const long EscalatorControl::ID_STATICTEXT2 = wxNewId();
const long EscalatorControl::ID_txtParent = wxNewId();
const long EscalatorControl::ID_txtRun = wxNewId();
const long EscalatorControl::ID_SLIDER1 = wxNewId();
const long EscalatorControl::ID_bOK = wxNewId();
//*)

BEGIN_EVENT_TABLE(EscalatorControl,wxDialog)
    //(*EventTable(EscalatorControl)
    //*)
END_EVENT_TABLE()

EscalatorControl::EscalatorControl(DebugPanel* parent, wxWindowID id)
{
    //(*Initialize(EscalatorControl)
    wxBoxSizer* BoxSizer1;
    wxFlexGridSizer* FlexGridSizer1;
    wxFlexGridSizer* FlexGridSizer2;
    wxFlexGridSizer* FlexGridSizer3;
    wxFlexGridSizer* FlexGridSizer4;
    wxFlexGridSizer* FlexGridSizer5;

    Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
    FlexGridSizer1 = new wxFlexGridSizer(0, 3, 0, 0);
    FlexGridSizer2 = new wxFlexGridSizer(0, 3, 0, 0);
    ListBox1 = new wxListBox(this, ID_LISTBOX1, wxDefaultPosition, wxSize(150,300), 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX1"));
    FlexGridSizer2->Add(ListBox1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer4 = new wxFlexGridSizer(0, 1, 0, 0);
    FlexGridSizer3 = new wxFlexGridSizer(0, 2, 0, 0);
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Name:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    FlexGridSizer3->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    txtName = new wxTextCtrl(this, ID_txtName, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_txtName"));
    FlexGridSizer3->Add(txtName, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Parent:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    FlexGridSizer3->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    txtParent = new wxTextCtrl(this, ID_txtParent, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_txtParent"));
    FlexGridSizer3->Add(txtParent, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer4->Add(FlexGridSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer5 = new wxFlexGridSizer(0, 1, 0, 0);
    txtRun = new wxStaticText(this, ID_txtRun, _("Run"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_txtRun"));
    FlexGridSizer5->Add(txtRun, 1, wxALL|wxEXPAND, 5);
    Slider1 = new wxSlider(this, ID_SLIDER1, 0, -1, 1, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER1"));
    FlexGridSizer5->Add(Slider1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer4->Add(FlexGridSizer5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    FlexGridSizer4->Add(BoxSizer1, 1, wxALL|wxEXPAND, 5);
    bOK = new wxButton(this, ID_bOK, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bOK"));
    FlexGridSizer4->Add(bOK, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer2->Add(FlexGridSizer4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(FlexGridSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(FlexGridSizer1);
    FlexGridSizer1->SetSizeHints(this);

    Connect(ID_bOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&EscalatorControl::On_bOK_Click);
    //*)

	lastcount = 0;
	Simcore = 0;
	panel = parent;
	escalator = 0;
}

EscalatorControl::~EscalatorControl()
{
    //(*Destroy(EscalatorControl)
    //*)
}

void EscalatorControl::On_bOK_Click(wxCommandEvent& event)
{
	this->Close();
}

void EscalatorControl::Loop()
{
	if (Simcore != panel->GetSystem())
	{
		//if active engine has changed, refresh values
		Simcore = panel->GetSystem();
	}

	if (!Simcore)
		return;

	BuildList();

	int selection = ListBox1->GetSelection();

	if (selection >= 0)
	{
		SBS::Escalator *newescalator = Simcore->GetEscalator(selection);

		//if a new camera has been selected, update values
		if (newescalator && escalator != newescalator)
		{
			escalator = Simcore->GetEscalator(selection);
			txtName->SetValue(escalator->GetName());
			txtParent->SetValue(escalator->GetParent()->GetName());
			Slider1->SetValue(escalator->GetRun());
		}
	}
	else
		escalator = 0;

	if (!escalator)
		return;
}

void EscalatorControl::BuildList(bool restore_selection)
{
	int count = Simcore->GetEscalatorCount();

	if (count != lastcount)
	{
		lastcount = count;
		int old_selection = ListBox1->GetSelection();
		ListBox1->Clear();

		for (int i = 0; i < count; i++)
		{
			::SBS::Escalator *esc = Simcore->GetEscalator(i);
			if (esc)
				ListBox1->Append(SBS::ToString(i + 1) + wxT(": ") + esc->GetName() + wxT(" (") + esc->GetParent()->GetName() + wxT(")"));
		}

		if (count > 0)
		{
			if (restore_selection == false)
				ListBox1->SetSelection(0);
			else
				ListBox1->SetSelection(old_selection);
		}
		else
		{
			//clear values
			txtName->SetValue(wxT(""));
			txtParent->SetValue(wxT(""));
			Slider1->SetValue(0);
		}
	}
}

void EscalatorControl::On_Slider1_OnUpdate(wxCommandEvent& event)
{
	if (escalator)
		escalator->SetRun(Slider1->GetValue());
}

}
