/* $Id$ */

/*
	Skyscraper 1.10 Alpha - People Manager Dialog
	Copyright (C)2003-2015 Ryan Thoryk
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

//(*InternalHeaders(EngineManager)
#include <wx/string.h>
#include <wx/intl.h>
//*)
#include "debugpanel.h"
#include "skyscraper.h"
#include "enginemanager.h"

namespace Skyscraper {

//(*IdInit(EngineManager)
const long EngineManager::ID_EngineList = wxNewId();
const long EngineManager::ID_STATICTEXT4 = wxNewId();
const long EngineManager::ID_tPosition = wxNewId();
const long EngineManager::ID_STATICTEXT1 = wxNewId();
const long EngineManager::ID_tActive = wxNewId();
const long EngineManager::ID_STATICTEXT2 = wxNewId();
const long EngineManager::ID_tState = wxNewId();
const long EngineManager::ID_STATICLINE2 = wxNewId();
const long EngineManager::ID_CLoads = wxNewId();
const long EngineManager::ID_chkRender = wxNewId();
const long EngineManager::ID_bSetActive = wxNewId();
const long EngineManager::ID_bReload = wxNewId();
const long EngineManager::ID_bLoad = wxNewId();
const long EngineManager::ID_bShutdown = wxNewId();
const long EngineManager::ID_bOk = wxNewId();
//*)

BEGIN_EVENT_TABLE(EngineManager,wxDialog)
	//(*EventTable(EngineManager)
	//*)
END_EVENT_TABLE()

EngineManager::EngineManager(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(EngineManager)
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxBoxSizer* BoxSizer7;
	wxBoxSizer* BoxSizer2;
	wxFlexGridSizer* FlexGridSizer4;
	wxBoxSizer* BoxSizer9;
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer8;
	wxBoxSizer* BoxSizer1;
	wxFlexGridSizer* FlexGridSizer5;
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer5;

	Create(parent, wxID_ANY, _("Engine Manager"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer5 = new wxFlexGridSizer(0, 2, 0, 0);
	BoxSizer8 = new wxBoxSizer(wxVERTICAL);
	EngineList = new wxListBox(this, ID_EngineList, wxDefaultPosition, wxSize(150,150), 0, 0, 0, wxDefaultValidator, _T("ID_EngineList"));
	BoxSizer8->Add(EngineList, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	FlexGridSizer5->Add(BoxSizer8, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	FlexGridSizer4 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer2 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Position:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	FlexGridSizer2->Add(StaticText4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tPosition = new wxTextCtrl(this, ID_tPosition, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tPosition"));
	FlexGridSizer2->Add(tPosition, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Camera Active:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer2->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tActive = new wxTextCtrl(this, ID_tActive, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tActive"));
	FlexGridSizer2->Add(tActive, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("State:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	FlexGridSizer2->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tState = new wxTextCtrl(this, ID_tState, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tState"));
	FlexGridSizer2->Add(tState, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer4->Add(FlexGridSizer2, 1, wxBOTTOM|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	StaticLine2 = new wxStaticLine(this, ID_STATICLINE2, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE2"));
	BoxSizer4->Add(StaticLine2, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer4->Add(BoxSizer4, 1, wxBOTTOM|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer7 = new wxBoxSizer(wxVERTICAL);
	chkCLoads = new wxCheckBox(this, ID_CLoads, _("Concurrent Loading"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CLoads"));
	chkCLoads->SetValue(false);
	BoxSizer7->Add(chkCLoads, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	chkRender = new wxCheckBox(this, ID_chkRender, _("Render on Startup"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkRender"));
	chkRender->SetValue(false);
	BoxSizer7->Add(chkRender, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer4->Add(BoxSizer7, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2 = new wxBoxSizer(wxVERTICAL);
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	bSetActive = new wxButton(this, ID_bSetActive, _("Set Active"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bSetActive"));
	BoxSizer5->Add(bSetActive, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bReload = new wxButton(this, ID_bReload, _("Reload"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bReload"));
	BoxSizer5->Add(bReload, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(BoxSizer5, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	bLoad = new wxButton(this, ID_bLoad, _("Load Building"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bLoad"));
	BoxSizer6->Add(bLoad, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bShutdown = new wxButton(this, ID_bShutdown, _("Shutdown"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bShutdown"));
	BoxSizer6->Add(bShutdown, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(BoxSizer6, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer4->Add(BoxSizer2, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(FlexGridSizer4, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer5->Add(BoxSizer1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(FlexGridSizer5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
	bOk = new wxButton(this, ID_bOk, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bOk"));
	BoxSizer9->Add(bOk, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(BoxSizer9, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);
	Center();

	Connect(ID_CLoads,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&EngineManager::On_chkCLoads_Click);
	Connect(ID_chkRender,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&EngineManager::On_chkRender_Click);
	Connect(ID_bSetActive,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&EngineManager::On_bSetActive_Click);
	Connect(ID_bReload,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&EngineManager::On_bReload_Click);
	Connect(ID_bLoad,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&EngineManager::On_bLoad_Click);
	Connect(ID_bShutdown,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&EngineManager::On_bShutdown_Click);
	Connect(ID_bOk,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&EngineManager::On_bOk_Click);
	//*)
	OnInit();
}

EngineManager::~EngineManager()
{
	//(*Destroy(EngineManager)
	//*)
}

void EngineManager::OnInit()
{
	lastcount = 0;
	engine = 0;

	chkCLoads->SetValue(skyscraper->ConcurrentLoads);
	chkRender->SetValue(skyscraper->RenderOnStartup);
}

void EngineManager::Loop()
{
	int count = skyscraper->GetEngineCount();
	if (count != lastcount)
	{
		lastcount = count;
		EngineList->Clear();

		for (int i = 0; i < count; i++)
		{
			EngineList->Append(wxVariant(i).GetString() + wxT(" - ") + wxString::FromAscii(skyscraper->GetEngine(i)->GetFilename().c_str()));
		}

		if (count > 0)
			EngineList->SetSelection(0);
	}

	if (count == 0)
		return;

	int selection = EngineList->GetSelection();
	engine = skyscraper->GetEngine(selection);

	if (engine)
	{
		if (!engine->GetSystem())
			return;

		Ogre::Vector3 position = engine->GetSystem()->GetPosition();
		tPosition->SetValue(TruncateNumber(position.x, 2) + wxT(", ") + TruncateNumber(position.y, 2) + wxT(", ") + TruncateNumber(position.z, 2));

		//set camera state
		if (engine->IsCameraActive() == true)
			tActive->SetValue("True");
		else
			tActive->SetValue("False");

		//set engine running state
		if (engine->GetShutdownState() == true)
			tState->SetValue("Shutdown");
		else if (engine->IsLoading() == true)
			tState->SetValue("Loading");
		else if (engine->IsRunning() == true)
			tState->SetValue("Running");
	}
}

void EngineManager::On_bSetActive_Click(wxCommandEvent& event)
{
	int selection = EngineList->GetSelection();

	if (selection >= 0)
		skyscraper->SetActiveEngine(selection);
}

void EngineManager::On_bLoad_Click(wxCommandEvent& event)
{
	skyscraper->Load(skyscraper->SelectBuilding());
}

void EngineManager::On_bReload_Click(wxCommandEvent& event)
{
	int selection = EngineList->GetSelection();

	if (selection >= 0)
	{
		EngineContext *engine = skyscraper->GetEngine(selection);

		if (engine)
			engine->Reload = true;
	}
}

void EngineManager::On_bShutdown_Click(wxCommandEvent& event)
{
	int selection = EngineList->GetSelection();

	if (selection >= 0)
	{
		EngineContext *engine = skyscraper->GetEngine(selection);

		if (engine)
			engine->Shutdown();
	}
}

void EngineManager::On_chkCLoads_Click(wxCommandEvent& event)
{
	skyscraper->ConcurrentLoads = chkCLoads->GetValue();
}

void EngineManager::On_chkRender_Click(wxCommandEvent& event)
{
	skyscraper->RenderOnStartup = chkRender->GetValue();
}

void EngineManager::On_bOk_Click(wxCommandEvent& event)
{
	this->Close();
}

}

