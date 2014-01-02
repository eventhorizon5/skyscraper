/* $Id$ */

/*
	Skyscraper 1.9 Alpha - Action Viewer
	Copyright (C)2003-2014 Ryan Thoryk
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

#include "actionviewer.h"

//(*InternalHeaders(ActionViewer)
#include <wx/string.h>
#include <wx/intl.h>
//*)
#include <wx/app.h>

#include "globals.h"
#include "sbs.h"
#include "action.h"

extern SBS *Simcore; //external pointer to the SBS engine

//(*IdInit(ActionViewer)
const long ActionViewer::ID_ActionList = wxNewId();
const long ActionViewer::ID_bRun = wxNewId();
const long ActionViewer::ID_bOK = wxNewId();
const long ActionViewer::ID_STATICTEXT1 = wxNewId();
const long ActionViewer::ID_tName = wxNewId();
const long ActionViewer::ID_STATICTEXT2 = wxNewId();
const long ActionViewer::ID_tParentName = wxNewId();
const long ActionViewer::ID_STATICTEXT3 = wxNewId();
const long ActionViewer::ID_tType = wxNewId();
const long ActionViewer::ID_STATICTEXT4 = wxNewId();
const long ActionViewer::ID_tCommand = wxNewId();
const long ActionViewer::ID_STATICTEXT5 = wxNewId();
const long ActionViewer::ID_tParameters = wxNewId();
//*)

BEGIN_EVENT_TABLE(ActionViewer,wxDialog)
	//(*EventTable(ActionViewer)
	//*)
END_EVENT_TABLE()

ActionViewer::ActionViewer(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(ActionViewer)
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxBoxSizer* BoxSizer2;
	wxFlexGridSizer* FlexGridSizer3;
	wxBoxSizer* BoxSizer1;
	
	Create(parent, wxID_ANY, _("Action Viewer"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
	ActionList = new wxListBox(this, ID_ActionList, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_ActionList"));
	ActionList->SetMinSize(wxSize(300,300));
	FlexGridSizer1->Add(ActionList, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	bRun = new wxButton(this, ID_bRun, _("Run Action"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bRun"));
	BoxSizer2->Add(bRun, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bOK = new wxButton(this, ID_bOK, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bOK"));
	BoxSizer2->Add(bOK, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer1->Add(BoxSizer2, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	BoxSizer1->Add(FlexGridSizer1, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer2 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer3 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Name:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer3->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tName = new wxTextCtrl(this, ID_tName, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_tName"));
	tName->SetMinSize(wxSize(200,-1));
	FlexGridSizer3->Add(tName, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Parent Name:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	FlexGridSizer3->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tParentName = new wxTextCtrl(this, ID_tParentName, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_tParentName"));
	FlexGridSizer3->Add(tParentName, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Parent Type:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	FlexGridSizer3->Add(StaticText3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tType = new wxTextCtrl(this, ID_tType, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_tType"));
	FlexGridSizer3->Add(tType, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Command:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	FlexGridSizer3->Add(StaticText4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tCommand = new wxTextCtrl(this, ID_tCommand, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_tCommand"));
	FlexGridSizer3->Add(tCommand, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer2->Add(FlexGridSizer3, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Parameters:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	FlexGridSizer2->Add(StaticText5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tParameters = new wxTextCtrl(this, ID_tParameters, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY, wxDefaultValidator, _T("ID_tParameters"));
	tParameters->SetMinSize(wxSize(200,100));
	FlexGridSizer2->Add(tParameters, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(FlexGridSizer2, 1, wxALL|wxEXPAND|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	
	Connect(ID_ActionList,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&ActionViewer::On_ActionList_Select);
	Connect(ID_bRun,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ActionViewer::On_bRun_Click);
	Connect(ID_bOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ActionViewer::On_bOK_Click);
	//*)

	for (int i = 0; i < Simcore->GetActionCount(); i++)
		ActionList->Append(wxVariant(i + 1).GetString() + wxT(" - ") + wxString::FromAscii(Simcore->GetAction(i)->GetName()));
}

ActionViewer::~ActionViewer()
{
	//(*Destroy(ActionViewer)
	//*)
}

void ActionViewer::On_bRun_Click(wxCommandEvent& event)
{
	int selection = ActionList->GetSelection();
	if (selection >= 0)
		Simcore->RunAction(selection);
}

void ActionViewer::On_bOK_Click(wxCommandEvent& event)
{
	this->Close();
}

void ActionViewer::On_ActionList_Select(wxCommandEvent& event)
{
	int selection = ActionList->GetSelection();
	if (selection < 0)
		return;

	Action *action = Simcore->GetAction(selection);
	if (action)
	{
		tName->SetValue(wxString::FromAscii(action->GetName()));
		tParentName->SetValue(wxString::FromAscii(action->GetParentName(0)));
		tType->SetValue(wxString::FromAscii(action->GetParentType(0)));
		tCommand->SetValue(wxString::FromAscii(action->GetCommandName()));

		tParameters->Clear();
		for (int i = 0; i < action->GetParameterCount(); i++)
			tParameters->AppendText(wxString::FromAscii(action->GetParameter(i)) + wxT("\n"));
	}

}
