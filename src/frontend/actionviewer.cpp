/*
	Skyscraper 1.12 Alpha - Action Viewer
	Copyright (C)2003-2023 Ryan Thoryk
	https://www.skyscrapersim.net
	https://sourceforge.net/projects/skyscraper/
	Contact - ryan@thoryk.com

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

//(*InternalHeaders(ActionViewer)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include <wx/app.h>

#include "globals.h"
#include "sbs.h"
#include "action.h"
#include "debugpanel.h"
#include "skyscraper.h"
#include "actionviewer.h"

using namespace SBS;

namespace Skyscraper {

//(*IdInit(ActionViewer)
const long ActionViewer::ID_ActionList = wxNewId();
const long ActionViewer::ID_bDelete = wxNewId();
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

ActionViewer::ActionViewer(DebugPanel* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(ActionViewer)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxFlexGridSizer* FlexGridSizer3;

	Create(parent, wxID_ANY, _("Action Viewer"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
	ActionList = new wxListBox(this, ID_ActionList, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_ActionList"));
	ActionList->SetMinSize(wxSize(300,300));
	FlexGridSizer1->Add(ActionList, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	bDelete = new wxButton(this, ID_bDelete, _("Delete"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bDelete"));
	BoxSizer2->Add(bDelete, 1, wxALL|wxALIGN_TOP, 5);
	bRun = new wxButton(this, ID_bRun, _("Run Action"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bRun"));
	BoxSizer2->Add(bRun, 1, wxALL|wxALIGN_TOP, 5);
	bOK = new wxButton(this, ID_bOK, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bOK"));
	BoxSizer2->Add(bOK, 1, wxALL|wxALIGN_TOP, 5);
	FlexGridSizer1->Add(BoxSizer2, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	BoxSizer1->Add(FlexGridSizer1, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer2 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer3 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Name:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer3->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tName = new wxTextCtrl(this, ID_tName, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_tName"));
	tName->SetMinSize(wxSize(200,-1));
	FlexGridSizer3->Add(tName, 1, wxTOP|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Parent Name(s):"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	FlexGridSizer3->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tParentName = new wxTextCtrl(this, ID_tParentName, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY, wxDefaultValidator, _T("ID_tParentName"));
	FlexGridSizer3->Add(tParentName, 1, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 5);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Parent Type(s):"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	FlexGridSizer3->Add(StaticText3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tType = new wxTextCtrl(this, ID_tType, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY, wxDefaultValidator, _T("ID_tType"));
	FlexGridSizer3->Add(tType, 1, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 5);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Command:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	FlexGridSizer3->Add(StaticText4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tCommand = new wxTextCtrl(this, ID_tCommand, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_tCommand"));
	FlexGridSizer3->Add(tCommand, 1, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 5);
	FlexGridSizer2->Add(FlexGridSizer3, 1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Parameters:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	FlexGridSizer2->Add(StaticText5, 1, wxTOP|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tParameters = new wxTextCtrl(this, ID_tParameters, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY, wxDefaultValidator, _T("ID_tParameters"));
	tParameters->SetMinSize(wxSize(200,100));
	FlexGridSizer2->Add(tParameters, 1, wxALL|wxEXPAND, 5);
	BoxSizer1->Add(FlexGridSizer2, 1, wxALL|wxEXPAND, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();

	Connect(ID_ActionList,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&ActionViewer::On_ActionList_Select);
	Connect(ID_bDelete,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ActionViewer::On_bDelete_Click);
	Connect(ID_bRun,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ActionViewer::On_bRun_Click);
	Connect(ID_bOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ActionViewer::On_bOK_Click);
	//*)

	lastcount = 0;
	Simcore = 0;
	panel = parent;
}

ActionViewer::~ActionViewer()
{
	//(*Destroy(ActionViewer)
	//*)
}

void ActionViewer::Loop()
{
	if (Simcore != panel->GetSystem())
	{
		//if active engine has changed, refresh values
		Simcore = panel->GetSystem();
		lastcount = 0;
	}

	if (!Simcore)
		return;

	if (Simcore->GetActionCount() != lastcount)
	{
		lastcount = Simcore->GetActionCount();

		//clear values
		ActionList->Clear();
		tName->Clear();
		tParentName->Clear();
		tType->Clear();
		tCommand->Clear();
		tParameters->Clear();

		for (int i = 0; i < Simcore->GetActionCount(); i++)
			ActionList->Append(ToString(i + 1) + wxT(" - ") + Simcore->GetAction(i)->GetName());
	}
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
		tName->SetValue(action->GetName());
		tParentName->Clear();
		tType->Clear();

		for (int i = 0; i < action->GetParentCount(); i++)
		{
			wxString index = ToString(i + 1);
			tParentName->AppendText(index + wxT(": ") + action->GetParentName(i) + wxT("\n"));
			tType->AppendText(index + wxT(": ") + action->GetParentType(i) + wxT("\n"));
		}

		tCommand->SetValue(action->GetCommandName());

		tParameters->Clear();
		for (int i = 0; i < action->GetParameterCount(); i++)
			tParameters->AppendText(action->GetParameter(i) + wxT("\n"));
	}

}

void ActionViewer::On_bDelete_Click(wxCommandEvent& event)
{
	int selection = ActionList->GetSelection();
	if (selection < 0)
		return;

	Action *action = Simcore->GetAction(selection);
	if (action)
		Simcore->RemoveAction(action);
}

}

