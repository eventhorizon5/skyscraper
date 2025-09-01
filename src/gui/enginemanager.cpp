/*
	Skyscraper 2.1 - Engine Manager Dialog
	Copyright (C)2003-2025 Ryan Thoryk
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

//(*InternalHeaders(EngineManager)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include "globals.h"
#include "sbs.h"
#include "camera.h"
#include "debugpanel.h"
#include "vm.h"
#include "enginecontext.h"
#include "moveengine.h"
#include "loaddialog.h"
#include "enginemanager.h"

namespace Skyscraper {

//(*IdInit(EngineManager)
const wxWindowID EngineManager::ID_EngineList = wxNewId();
const wxWindowID EngineManager::ID_STATICTEXT7 = wxNewId();
const wxWindowID EngineManager::ID_tType = wxNewId();
const wxWindowID EngineManager::ID_STATICTEXT8 = wxNewId();
const wxWindowID EngineManager::ID_tParent = wxNewId();
const wxWindowID EngineManager::ID_STATICTEXT4 = wxNewId();
const wxWindowID EngineManager::ID_tPosition = wxNewId();
const wxWindowID EngineManager::ID_STATICTEXT3 = wxNewId();
const wxWindowID EngineManager::ID_tBoundsMin = wxNewId();
const wxWindowID EngineManager::ID_STATICTEXT5 = wxNewId();
const wxWindowID EngineManager::ID_tBoundsMax = wxNewId();
const wxWindowID EngineManager::ID_STATICTEXT1 = wxNewId();
const wxWindowID EngineManager::ID_tActive = wxNewId();
const wxWindowID EngineManager::ID_STATICTEXT2 = wxNewId();
const wxWindowID EngineManager::ID_tState = wxNewId();
const wxWindowID EngineManager::ID_STATICTEXT6 = wxNewId();
const wxWindowID EngineManager::ID_tUptime = wxNewId();
const wxWindowID EngineManager::ID_STATICLINE2 = wxNewId();
const wxWindowID EngineManager::ID_CLoads = wxNewId();
const wxWindowID EngineManager::ID_chkRender = wxNewId();
const wxWindowID EngineManager::ID_chkPaused = wxNewId();
const wxWindowID EngineManager::ID_bSetActive = wxNewId();
const wxWindowID EngineManager::ID_bReload = wxNewId();
const wxWindowID EngineManager::ID_bMove = wxNewId();
const wxWindowID EngineManager::ID_bLoad = wxNewId();
const wxWindowID EngineManager::ID_bReset = wxNewId();
const wxWindowID EngineManager::ID_bShutdown = wxNewId();
const wxWindowID EngineManager::ID_bOk = wxNewId();
//*)

BEGIN_EVENT_TABLE(EngineManager,wxDialog)
	//(*EventTable(EngineManager)
	//*)
END_EVENT_TABLE()

EngineManager::EngineManager(DebugPanel* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(EngineManager)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer7;
	wxBoxSizer* BoxSizer8;
	wxBoxSizer* BoxSizer9;
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxFlexGridSizer* FlexGridSizer4;
	wxFlexGridSizer* FlexGridSizer5;

	Create(parent, wxID_ANY, _("Engine Manager"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer5 = new wxFlexGridSizer(0, 2, 0, 0);
	BoxSizer8 = new wxBoxSizer(wxVERTICAL);
	EngineList = new wxListBox(this, ID_EngineList, wxDefaultPosition, wxSize(300,200), 0, 0, 0, wxDefaultValidator, _T("ID_EngineList"));
	BoxSizer8->Add(EngineList, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, 5);
	FlexGridSizer5->Add(BoxSizer8, 1, wxALL|wxEXPAND, 5);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	FlexGridSizer4 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer2 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Type:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	FlexGridSizer2->Add(StaticText7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tType = new wxTextCtrl(this, ID_tType, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tType"));
	tType->SetMinSize(wxSize(200,-1));
	FlexGridSizer2->Add(tType, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("Parent:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	FlexGridSizer2->Add(StaticText8, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tParent = new wxTextCtrl(this, ID_tParent, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tParent"));
	tParent->SetMinSize(wxSize(200,-1));
	FlexGridSizer2->Add(tParent, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Position:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	FlexGridSizer2->Add(StaticText4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tPosition = new wxTextCtrl(this, ID_tPosition, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tPosition"));
	tPosition->SetMinSize(wxSize(200,-1));
	FlexGridSizer2->Add(tPosition, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Bounds Min:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	FlexGridSizer2->Add(StaticText3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tBoundsMin = new wxTextCtrl(this, ID_tBoundsMin, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tBoundsMin"));
	tBoundsMin->SetMinSize(wxSize(200,-1));
	FlexGridSizer2->Add(tBoundsMin, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Bounds Max:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	FlexGridSizer2->Add(StaticText5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tBoundsMax = new wxTextCtrl(this, ID_tBoundsMax, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tBoundsMax"));
	tBoundsMax->SetMinSize(wxSize(200,-1));
	FlexGridSizer2->Add(tBoundsMax, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Camera Active:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer2->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tActive = new wxTextCtrl(this, ID_tActive, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tActive"));
	tActive->SetMinSize(wxSize(200,-1));
	FlexGridSizer2->Add(tActive, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("State:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	FlexGridSizer2->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tState = new wxTextCtrl(this, ID_tState, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tState"));
	tState->SetMinSize(wxSize(200,-1));
	FlexGridSizer2->Add(tState, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("Uptime:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	FlexGridSizer2->Add(StaticText6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tUptime = new wxTextCtrl(this, ID_tUptime, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tUptime"));
	tUptime->SetMinSize(wxSize(200,-1));
	FlexGridSizer2->Add(tUptime, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer4->Add(FlexGridSizer2, 1, wxBOTTOM|wxEXPAND, 5);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	StaticLine2 = new wxStaticLine(this, ID_STATICLINE2, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE2"));
	BoxSizer4->Add(StaticLine2, 1, wxEXPAND, 5);
	FlexGridSizer4->Add(BoxSizer4, 1, wxBOTTOM|wxEXPAND, 5);
	BoxSizer7 = new wxBoxSizer(wxVERTICAL);
	chkCLoads = new wxCheckBox(this, ID_CLoads, _("Concurrent Loading"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CLoads"));
	chkCLoads->SetValue(false);
	BoxSizer7->Add(chkCLoads, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	chkRender = new wxCheckBox(this, ID_chkRender, _("Render on Startup"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkRender"));
	chkRender->SetValue(false);
	BoxSizer7->Add(chkRender, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	chkPaused = new wxCheckBox(this, ID_chkPaused, _("Paused"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkPaused"));
	chkPaused->SetValue(false);
	BoxSizer7->Add(chkPaused, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer4->Add(BoxSizer7, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2 = new wxBoxSizer(wxVERTICAL);
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	bSetActive = new wxButton(this, ID_bSetActive, _("Set Active"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bSetActive"));
	BoxSizer5->Add(bSetActive, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bReload = new wxButton(this, ID_bReload, _("Reload"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bReload"));
	BoxSizer5->Add(bReload, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bMove = new wxButton(this, ID_bMove, _("Move"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bMove"));
	BoxSizer5->Add(bMove, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(BoxSizer5, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	bLoad = new wxButton(this, ID_bLoad, _("Load Building"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bLoad"));
	BoxSizer6->Add(bLoad, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bReset = new wxButton(this, ID_bReset, _("Reset"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bReset"));
	BoxSizer6->Add(bReset, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bShutdown = new wxButton(this, ID_bShutdown, _("Shutdown"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bShutdown"));
	BoxSizer6->Add(bShutdown, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(BoxSizer6, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer4->Add(BoxSizer2, 1, wxEXPAND, 5);
	BoxSizer1->Add(FlexGridSizer4, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer5->Add(BoxSizer1, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer1->Add(FlexGridSizer5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
	bOk = new wxButton(this, ID_bOk, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bOk"));
	BoxSizer9->Add(bOk, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(BoxSizer9, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->SetSizeHints(this);
	Center();

	Connect(ID_CLoads, wxEVT_COMMAND_CHECKBOX_CLICKED, (wxObjectEventFunction)&EngineManager::On_chkCLoads_Click);
	Connect(ID_chkRender, wxEVT_COMMAND_CHECKBOX_CLICKED, (wxObjectEventFunction)&EngineManager::On_chkRender_Click);
	Connect(ID_chkPaused, wxEVT_COMMAND_CHECKBOX_CLICKED, (wxObjectEventFunction)&EngineManager::On_chkPaused_Click);
	Connect(ID_bSetActive, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&EngineManager::On_bSetActive_Click);
	Connect(ID_bReload, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&EngineManager::On_bReload_Click);
	Connect(ID_bMove, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&EngineManager::On_bMove_Click);
	Connect(ID_bLoad, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&EngineManager::On_bLoad_Click);
	Connect(ID_bReset, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&EngineManager::On_bReset_Click);
	Connect(ID_bShutdown, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&EngineManager::On_bShutdown_Click);
	Connect(ID_bOk, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&EngineManager::On_bOk_Click);
	//*)
	panel = parent;
	loader = 0;
	moveengine = 0;
	OnInit();
}

EngineManager::~EngineManager()
{
	//(*Destroy(EngineManager)
	//*)

	if (loader)
		loader->Destroy();
	loader = 0;
	if (moveengine)
		moveengine->Destroy();
	moveengine = 0;
}

void EngineManager::OnInit()
{
	lastcount = 0;
	engine = 0;

	chkCLoads->SetValue(panel->GetRoot()->ConcurrentLoads);
	chkRender->SetValue(panel->GetRoot()->GetRenderOnStartup());
}

void EngineManager::Loop()
{
	int count = panel->GetRoot()->GetEngineCount();
	if (count != lastcount)
	{
		int selection = EngineList->GetSelection();
		int size = panel->GetRoot()->GetEngineListSize();

		lastcount = count;
		EngineList->Clear();

		for (int i = 0; i < size; i++)
		{
			EngineContext *engine = panel->GetRoot()->GetEngine(i);
			std::string filename = "Empty";

			if (engine)
				filename = engine->GetFilename();

			EngineList->Append(SBS::ToString(i) + wxT(" - ") + filename);
		}

		if (selection == -1)
			selection = 0;

		if (selection >= size)
			selection = size - 1;

		if (count > 0 && selection < size)
			EngineList->SetSelection(selection);
	}

	if (count == 0)
		return;

	int selection = EngineList->GetSelection();
	engine = panel->GetRoot()->GetEngine(selection);

	if (engine)
	{
		if (!engine->GetSystem())
			return;

		Vector3 position = engine->GetSystem()->GetPosition();
		Vector3 min, max;
		engine->GetSystem()->GetBounds(min, max);

		if (min.y <= -999999)
			min.y = 0;
		if (max.y >= 999999)
			max.y = 0;

		tPosition->SetValue(TruncateNumber(position.x, 2) + wxT(", ") + TruncateNumber(position.y, 2) + wxT(", ") + TruncateNumber(position.z, 2));
		tBoundsMin->SetValue(TruncateNumber(min.x, 2) + wxT(", ") + TruncateNumber(min.y, 2) + wxT(", ") + TruncateNumber(min.z, 2));
		tBoundsMax->SetValue(TruncateNumber(max.x, 2) + wxT(", ") + TruncateNumber(max.y, 2) + wxT(", ") + TruncateNumber(max.z, 2));

		//set camera state
		if (engine->IsCameraActive() == true)
			tActive->SetValue("True");
		else
			tActive->SetValue("False");

		//set engine running state
		tState->SetValue(engine->GetStatus());

		//set engine uptime
		tUptime->SetValue(SBS::ToString(engine->GetSystem()->GetRunTime() / 1000));

		//set paused state
		chkPaused->SetValue(engine->Paused);

		if (engine->GetParent())
			tParent->SetValue(SBS::ToString(engine->GetParent()->GetNumber()));
		else
			tParent->SetValue("None");
		tType->SetValue(engine->GetType());
	}
	else
	{
		tPosition->Clear();
		tBoundsMin->Clear();
		tBoundsMax->Clear();
		tActive->Clear();
		tState->SetValue("Unloaded");
		tType->Clear();
		tParent->Clear();
	}

	if (moveengine)
		moveengine->Loop();
}

void EngineManager::On_bSetActive_Click(wxCommandEvent& event)
{
	int selection = EngineList->GetSelection();

	if (selection >= 0)
		panel->GetRoot()->SetActiveEngine(selection);
}

void EngineManager::On_bLoad_Click(wxCommandEvent& event)
{
	int selection = EngineList->GetSelection();
	engine = panel->GetRoot()->GetEngine(selection);

	if (!loader)
		loader = new LoadDialog(panel, this, -1);

	loader->SetEngineParent(engine);
	loader->CenterOnScreen();
	loader->Show();
}

void EngineManager::On_bReload_Click(wxCommandEvent& event)
{
	int selection = EngineList->GetSelection();

	if (selection >= 0)
	{
		EngineContext *engine = panel->GetRoot()->GetEngine(selection);

		if (engine)
			engine->Reload = true;
	}
}

void EngineManager::On_bShutdown_Click(wxCommandEvent& event)
{
	int selection = EngineList->GetSelection();

	if (selection >= 0)
	{
		EngineContext *engine = panel->GetRoot()->GetEngine(selection);

		if (engine)
			engine->Shutdown();
	}
}

void EngineManager::On_chkCLoads_Click(wxCommandEvent& event)
{
	panel->GetRoot()->ConcurrentLoads = chkCLoads->GetValue();
}

void EngineManager::On_chkRender_Click(wxCommandEvent& event)
{
	panel->GetRoot()->SetRenderOnStartup(chkRender->GetValue());
}

void EngineManager::On_bOk_Click(wxCommandEvent& event)
{
	this->Close();
}

void EngineManager::On_bMove_Click(wxCommandEvent& event)
{
	int selection = EngineList->GetSelection();

	if (selection >= 0)
	{
		EngineContext *engine = panel->GetRoot()->GetEngine(selection);

		if (engine)
		{
			if (moveengine)
				delete moveengine;
			moveengine = 0;

			moveengine = new MoveEngine(panel, this, -1, engine);
			moveengine->Show();
		}
	}
}

void EngineManager::On_chkPaused_Click(wxCommandEvent& event)
{
	int selection = EngineList->GetSelection();

	if (selection >= 0)
	{
		EngineContext *engine = panel->GetRoot()->GetEngine(selection);

		if (engine)
			engine->Paused = chkPaused->GetValue();
	}
}

void EngineManager::On_bReset_Click(wxCommandEvent& event)
{
	int selection = EngineList->GetSelection();

	if (selection >= 0)
	{
		EngineContext *engine = panel->GetRoot()->GetEngine(selection);

		if (engine)
			engine->Reset();
	}
}

}
