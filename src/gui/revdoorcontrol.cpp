/*
	Skyscraper 2.1 - Revolving Door Control
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

//(*InternalHeaders(RevolvingDoorControl)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include "globals.h"
#include "sbs.h"
#include "debugpanel.h"
#include "revolvingdoor.h"
#include "revdoorcontrol.h"

namespace Skyscraper {

//(*IdInit(RevolvingDoorControl)
const wxWindowID RevolvingDoorControl::ID_LISTBOX1 = wxNewId();
const wxWindowID RevolvingDoorControl::ID_STATICTEXT1 = wxNewId();
const wxWindowID RevolvingDoorControl::ID_txtName = wxNewId();
const wxWindowID RevolvingDoorControl::ID_STATICTEXT2 = wxNewId();
const wxWindowID RevolvingDoorControl::ID_txtParent = wxNewId();
const wxWindowID RevolvingDoorControl::ID_txtState = wxNewId();
const wxWindowID RevolvingDoorControl::ID_tRun = wxNewId();
const wxWindowID RevolvingDoorControl::ID_bOK = wxNewId();
//*)

BEGIN_EVENT_TABLE(RevolvingDoorControl,wxDialog)
    //(*EventTable(RevolvingDoorControl)
    //*)
END_EVENT_TABLE()

RevolvingDoorControl::RevolvingDoorControl(DebugPanel* parent, wxWindowID id)
{
    //(*Initialize(RevolvingDoorControl)
    wxBoxSizer* BoxSizer1;
    wxFlexGridSizer* FlexGridSizer1;
    wxFlexGridSizer* FlexGridSizer2;
    wxFlexGridSizer* FlexGridSizer3;
    wxFlexGridSizer* FlexGridSizer4;
    wxFlexGridSizer* FlexGridSizer5;

    Create(parent, id, _("Revolving Door Control"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
    FlexGridSizer1 = new wxFlexGridSizer(0, 3, 0, 0);
    FlexGridSizer2 = new wxFlexGridSizer(0, 3, 0, 0);
    ListBox1 = new wxListBox(this, ID_LISTBOX1, wxDefaultPosition, wxSize(200,300), 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX1"));
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
    txtState = new wxStaticText(this, ID_txtState, _("State:"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_txtState"));
    FlexGridSizer5->Add(txtState, 1, wxALL|wxEXPAND, 5);
    tRun = new wxToggleButton(this, ID_tRun, _("Run"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_tRun"));
    FlexGridSizer5->Add(tRun, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer4->Add(FlexGridSizer5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    FlexGridSizer4->Add(BoxSizer1, 1, wxALL|wxEXPAND, 5);
    bOK = new wxButton(this, ID_bOK, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bOK"));
    FlexGridSizer4->Add(bOK, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer2->Add(FlexGridSizer4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(FlexGridSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(FlexGridSizer1);
    FlexGridSizer1->SetSizeHints(this);

    Connect(ID_tRun, wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, (wxObjectEventFunction)&RevolvingDoorControl::On_tRun_Toggle);
    Connect(ID_bOK, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&RevolvingDoorControl::On_bOK_Click);
    //*)

	lastcount = 0;
	Simcore = 0;
	panel = parent;
	door = 0;
}

RevolvingDoorControl::~RevolvingDoorControl()
{
    //(*Destroy(RevolvingDoorControl)
    //*)
}

void RevolvingDoorControl::On_bOK_Click(wxCommandEvent& event)
{
	this->Close();
}

void RevolvingDoorControl::Loop()
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
		SBS::RevolvingDoor *newdoor = Simcore->GetRevolvingDoor(selection);

		//if a new escalator has been selected, update values
		if (newdoor && door != newdoor)
		{
			door = Simcore->GetRevolvingDoor(selection);
			txtName->SetValue(door->GetName());
			txtParent->SetValue(door->GetParent()->GetName());
			tRun->SetValue(door->GetRun());
		}
	}
	else
		door = 0;

	if (!door)
		return;
}

void RevolvingDoorControl::BuildList(bool restore_selection)
{
	int count = Simcore->GetRevolvingDoorCount();

	if (count != lastcount)
	{
		lastcount = count;
		int old_selection = ListBox1->GetSelection();
		ListBox1->Clear();

		for (int i = 0; i < count; i++)
		{
			::SBS::RevolvingDoor *dr = Simcore->GetRevolvingDoor(i);
			if (dr)
				ListBox1->Append(SBS::ToString(i + 1) + wxT(": ") + dr->GetName());
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
			tRun->SetValue(false);
		}
	}
}

void RevolvingDoorControl::On_tRun_Toggle(wxCommandEvent& event)
{
	if (door)
		door->Run(tRun->GetValue());
}

}
