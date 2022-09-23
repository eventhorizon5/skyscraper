/* $Id$ */

/*
	Skyscraper 1.11 Alpha - People Manager Dialog
	Copyright (C)2003-2018 Ryan Thoryk
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

//(*InternalHeaders(PeopleManager)
#include <wx/string.h>
#include <wx/intl.h>
//*)
#include "globals.h"
#include "sbs.h"
#include "person.h"
#include "skyscraper.h"
#include "debugpanel.h"
#include "peoplemanager.h"

namespace Skyscraper {

//(*IdInit(PeopleManager)
const long PeopleManager::ID_PersonList = wxNewId();
const long PeopleManager::ID_STATICTEXT1 = wxNewId();
const long PeopleManager::ID_tName = wxNewId();
const long PeopleManager::ID_bSetName = wxNewId();
const long PeopleManager::ID_STATICTEXT2 = wxNewId();
const long PeopleManager::ID_tFloor = wxNewId();
const long PeopleManager::ID_bSetFloor = wxNewId();
const long PeopleManager::ID_STATICTEXT3 = wxNewId();
const long PeopleManager::ID_tDestination = wxNewId();
const long PeopleManager::ID_bGo = wxNewId();
const long PeopleManager::ID_STATICTEXT5 = wxNewId();
const long PeopleManager::ID_tRouteActive = wxNewId();
const long PeopleManager::ID_bStop = wxNewId();
const long PeopleManager::ID_STATICLINE1 = wxNewId();
const long PeopleManager::ID_STATICLINE2 = wxNewId();
const long PeopleManager::ID_STATICLINE3 = wxNewId();
const long PeopleManager::ID_tStatus = wxNewId();
const long PeopleManager::ID_chkRandom = wxNewId();
const long PeopleManager::ID_chkService = wxNewId();
const long PeopleManager::ID_bNew = wxNewId();
const long PeopleManager::ID_bDelete = wxNewId();
const long PeopleManager::ID_bOK = wxNewId();
//*)

BEGIN_EVENT_TABLE(PeopleManager,wxDialog)
	//(*EventTable(PeopleManager)
	//*)
END_EVENT_TABLE()

PeopleManager::PeopleManager(DebugPanel* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(PeopleManager)
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxBoxSizer* BoxSizer3;
	wxBoxSizer* BoxSizer2;
	wxFlexGridSizer* FlexGridSizer4;
	wxFlexGridSizer* FlexGridSizer3;
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer1;

	Create(parent, id, _("People Manager"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer4 = new wxFlexGridSizer(0, 2, 0, 0);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	PersonList = new wxListBox(this, ID_PersonList, wxDefaultPosition, wxSize(150,150), 0, 0, 0, wxDefaultValidator, _T("ID_PersonList"));
	BoxSizer1->Add(PersonList, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer4->Add(BoxSizer1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer3 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer2 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Name:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer2->Add(StaticText1, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tName = new wxTextCtrl(this, ID_tName, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_tName"));
	FlexGridSizer2->Add(tName, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetName = new wxButton(this, ID_bSetName, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetName"));
	FlexGridSizer2->Add(bSetName, 1, wxBOTTOM|wxLEFT|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Floor:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	FlexGridSizer2->Add(StaticText2, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tFloor = new wxTextCtrl(this, ID_tFloor, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_tFloor"));
	FlexGridSizer2->Add(tFloor, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetFloor = new wxButton(this, ID_bSetFloor, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetFloor"));
	FlexGridSizer2->Add(bSetFloor, 1, wxBOTTOM|wxLEFT|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Destination:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	FlexGridSizer2->Add(StaticText3, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tDestination = new wxTextCtrl(this, ID_tDestination, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_tDestination"));
	FlexGridSizer2->Add(tDestination, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bGo = new wxButton(this, ID_bGo, _("Go"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bGo"));
	FlexGridSizer2->Add(bGo, 1, wxBOTTOM|wxLEFT|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Route Active:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	FlexGridSizer2->Add(StaticText5, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tRouteActive = new wxTextCtrl(this, ID_tRouteActive, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_tRouteActive"));
	FlexGridSizer2->Add(tRouteActive, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bStop = new wxButton(this, ID_bStop, _("Stop"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bStop"));
	FlexGridSizer2->Add(bStop, 1, wxBOTTOM|wxLEFT|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
	FlexGridSizer2->Add(StaticLine1, 1, wxTOP|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticLine2 = new wxStaticLine(this, ID_STATICLINE2, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE2"));
	FlexGridSizer2->Add(StaticLine2, 1, wxTOP|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticLine3 = new wxStaticLine(this, ID_STATICLINE3, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE3"));
	FlexGridSizer2->Add(StaticLine3, 1, wxTOP|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer3->Add(FlexGridSizer2, 1, wxTOP|wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tStatus = new wxTextCtrl(this, ID_tStatus, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tStatus"));
	FlexGridSizer3->Add(tStatus, 1, wxBOTTOM|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2 = new wxBoxSizer(wxVERTICAL);
	chkRandom = new wxCheckBox(this, ID_chkRandom, _("Random Activity"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkRandom"));
	chkRandom->SetValue(false);
	BoxSizer2->Add(chkRandom, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	chkService = new wxCheckBox(this, ID_chkService, _("Service Access"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkService"));
	chkService->SetValue(false);
	BoxSizer2->Add(chkService, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer3->Add(BoxSizer2, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	bNew = new wxButton(this, ID_bNew, _("New"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bNew"));
	BoxSizer3->Add(bNew, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bDelete = new wxButton(this, ID_bDelete, _("Delete"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bDelete"));
	BoxSizer3->Add(bDelete, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer3->Add(BoxSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer4->Add(FlexGridSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(FlexGridSizer4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	bOK = new wxButton(this, ID_bOK, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bOK"));
	BoxSizer4->Add(bOK, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(BoxSizer4, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);
	Center();

	Connect(ID_bSetName,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PeopleManager::On_bSetName_Click);
	Connect(ID_bSetFloor,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PeopleManager::On_bSetFloor_Click);
	Connect(ID_bGo,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PeopleManager::On_bGo_Click);
	Connect(ID_bStop,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PeopleManager::On_bStop_Click);
	Connect(ID_chkRandom,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&PeopleManager::On_chkRandom_Click);
	Connect(ID_chkService,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&PeopleManager::On_chkService_Click);
	Connect(ID_bNew,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PeopleManager::On_bNew_Click);
	Connect(ID_bDelete,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PeopleManager::On_bDelete_Click);
	Connect(ID_bOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PeopleManager::On_bOK_Click);
	//*)

	lastcount = 0;
	Simcore = 0;
	person = 0;
	floor = -1;
	dest_floor = -1;
	panel = parent;
}

PeopleManager::~PeopleManager()
{
	//(*Destroy(PeopleManager)
	//*)
}

void PeopleManager::Loop()
{
	bool reset = false;

	if (Simcore != panel->GetSystem())
	{
		//if active engine has changed, refresh values
		Simcore = panel->GetSystem();
	}

	if (!Simcore)
		return;

	BuildList();

	int selection = PersonList->GetSelection();

	if (selection >= 0)
	{
		SBS::Person *newperson = Simcore->GetPerson(selection);

		//if a new person has been selected, update values
		if (newperson && person != newperson)
		{
			person = Simcore->GetPerson(selection);

			tName->SetValue(person->GetName());
			chkRandom->SetValue(person->IsRandomActivityEnabled());
			chkService->SetValue(person->GetServiceAccess());
			reset = true;
		}
	}
	else
		person = 0;

	if (!person)
		return;

	//set dynamic values

	if (person->IsRouteActive() == true)
		tRouteActive->SetValue("True");
	else
		tRouteActive->SetValue("False");

	if (person->GetFloor() != floor || reset == true)
	{
		floor = person->GetFloor();
		tFloor->SetValue(SBS::ToString(floor));
	}

	if (person->GetDestinationFloor() != dest_floor || reset == true)
	{
		dest_floor = person->GetDestinationFloor();
		tDestination->SetValue(SBS::ToString(dest_floor));
	}

	if (person->GetStatus() != status || reset == true)
	{
		status = person->GetStatus();
		tStatus->SetValue(status);
	}
}

void PeopleManager::On_bSetName_Click(wxCommandEvent& event)
{
	if (person)
	{
		person->SetName(std::string(tName->GetValue()));
		lastcount = 0;
		BuildList(true);
	}
}

void PeopleManager::On_bSetFloor_Click(wxCommandEvent& event)
{
	if (person)
		person->SetFloor(atoi(tFloor->GetValue()));
}

void PeopleManager::On_bGo_Click(wxCommandEvent& event)
{
	if (person)
		person->GotoFloor(atoi(tDestination->GetValue()));
}

void PeopleManager::On_bNew_Click(wxCommandEvent& event)
{
	Simcore->CreatePerson();
}

void PeopleManager::On_bDelete_Click(wxCommandEvent& event)
{
	if (person)
		Simcore->DeleteObject(person);
	person = 0;
}

void PeopleManager::On_bOK_Click(wxCommandEvent& event)
{
	this->Close();
}

void PeopleManager::On_chkRandom_Click(wxCommandEvent& event)
{
	if (person)
		person->EnableRandomActivity(chkRandom->GetValue());
}

void PeopleManager::On_chkService_Click(wxCommandEvent& event)
{
	if (person)
		person->SetServiceAccess(chkService->GetValue());
}

void PeopleManager::On_bStop_Click(wxCommandEvent& event)
{
	if (person)
		person->Stop();
}

void PeopleManager::BuildList(bool restore_selection)
{
	int count = Simcore->GetPersonCount();

	if (count != lastcount)
	{
		lastcount = count;
		int old_selection = PersonList->GetSelection();
		PersonList->Clear();

		for (int i = 0; i < count; i++)
			PersonList->Append(SBS::ToString(i + 1) + wxT(": ") + Simcore->GetPerson(i)->GetName());

		if (count > 0)
		{
			if (restore_selection == false)
				PersonList->SetSelection(count - 1);
			else
				PersonList->SetSelection(old_selection);
		}
		else
		{
			//clear values
			tName->Clear();
			tRouteActive->Clear();
			tFloor->Clear();
			tDestination->Clear();
			tStatus->Clear();
			chkRandom->SetValue(false);
			chkService->SetValue(false);
		}
	}
}

}

