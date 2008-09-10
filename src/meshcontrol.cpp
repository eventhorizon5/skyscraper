/* $Id$ */

/*
	Skyscraper 1.1 Alpha - Mesh Control Form
	Copyright (C)2005-2008 Ryan Thoryk
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

//(*InternalHeaders(MeshControl)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include "debugpanel.h"
#include "meshcontrol.h"
#include "globals.h"
#include "sbs.h"
#include "camera.h"
#include "floor.h"
#include "elevator.h"

extern SBS *sbs; //external pointer to the SBS engine
MeshControl *p; //self pointer

//(*IdInit(MeshControl)
const long MeshControl::ID_STATICTEXT1 = wxNewId();
const long MeshControl::ID_chkExternal = wxNewId();
const long MeshControl::ID_chkBuildings = wxNewId();
const long MeshControl::ID_chkLandscape = wxNewId();
const long MeshControl::ID_chkSky = wxNewId();
const long MeshControl::ID_chkColumnFrame = wxNewId();
const long MeshControl::ID_chkElevators = wxNewId();
const long MeshControl::ID_STATICTEXT2 = wxNewId();
const long MeshControl::ID_chkFloor = wxNewId();
const long MeshControl::ID_bOk = wxNewId();
//*)

BEGIN_EVENT_TABLE(MeshControl,wxDialog)
	//(*EventTable(MeshControl)
	//*)
END_EVENT_TABLE()

MeshControl::MeshControl(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(MeshControl)
	Create(parent, wxID_ANY, _("Realtime Mesh Control"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Primary Meshes"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	BoxSizer3->Add(StaticText1, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkExternal = new wxCheckBox(this, ID_chkExternal, _("External"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkExternal"));
	chkExternal->SetValue(false);
	BoxSizer3->Add(chkExternal, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkBuildings = new wxCheckBox(this, ID_chkBuildings, _("Buildings"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkBuildings"));
	chkBuildings->SetValue(false);
	BoxSizer3->Add(chkBuildings, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkLandscape = new wxCheckBox(this, ID_chkLandscape, _("Landscape"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkLandscape"));
	chkLandscape->SetValue(false);
	BoxSizer3->Add(chkLandscape, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkSky = new wxCheckBox(this, ID_chkSky, _("Sky"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkSky"));
	chkSky->SetValue(false);
	BoxSizer3->Add(chkSky, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkColumnFrame = new wxCheckBox(this, ID_chkColumnFrame, _("ColumnFrame"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkColumnFrame"));
	chkColumnFrame->SetValue(false);
	BoxSizer3->Add(chkColumnFrame, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkElevators = new wxCheckBox(this, ID_chkElevators, _("Elevators"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkElevators"));
	chkElevators->SetValue(false);
	BoxSizer3->Add(chkElevators, 1, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(BoxSizer3, 0, wxRIGHT|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 10);
	BoxSizer4 = new wxBoxSizer(wxVERTICAL);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Per-Floor Meshes"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	BoxSizer4->Add(StaticText2, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkFloor = new wxCheckBox(this, ID_chkFloor, _("Level"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkFloor"));
	chkFloor->SetValue(false);
	BoxSizer4->Add(chkFloor, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(BoxSizer4, 0, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 0);
	BoxSizer1->Add(BoxSizer2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 10);
	bOk = new wxButton(this, ID_bOk, _("OK"), wxPoint(75,145), wxDefaultSize, 0, wxDefaultValidator, _T("ID_bOk"));
	BoxSizer1->Add(bOk, 0, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 10);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();

	Connect(ID_chkExternal,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MeshControl::On_chkExternal_Click);
	Connect(ID_chkBuildings,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MeshControl::On_chkBuildings_Click);
	Connect(ID_chkLandscape,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MeshControl::On_chkLandscape_Click);
	Connect(ID_chkSky,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MeshControl::On_chkSky_Click);
	Connect(ID_chkColumnFrame,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MeshControl::On_chkColumnFrame_Click);
	Connect(ID_chkElevators,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MeshControl::On_chkElevators_Click);
	Connect(ID_chkFloor,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MeshControl::On_chkFloor_Click);
	Connect(ID_bOk,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MeshControl::On_bOk_Click);
	//*)
	p = this;
	OnInit();
}

MeshControl::~MeshControl()
{
}


void MeshControl::OnInit()
{
	chkElevators->SetValue(true);
}

void MeshControl::On_bOk_Click(wxCommandEvent& event)
{
	p->Hide();
}

void MeshControl::On_chkColumnFrame_Click(wxCommandEvent& event)
{
	sbs->EnableColumnFrame(chkColumnFrame->GetValue());
}

void MeshControl::On_chkSky_Click(wxCommandEvent& event)
{
	sbs->EnableSkybox(chkSky->GetValue());
}

void MeshControl::On_chkLandscape_Click(wxCommandEvent& event)
{
	sbs->EnableLandscape(chkLandscape->GetValue());
}

void MeshControl::On_chkBuildings_Click(wxCommandEvent& event)
{
	sbs->EnableBuildings(chkBuildings->GetValue());
}

void MeshControl::On_chkExternal_Click(wxCommandEvent& event)
{
	sbs->EnableExternal(chkExternal->GetValue());
}

void MeshControl::On_chkFloor_Click(wxCommandEvent& event)
{
	sbs->GetFloor(sbs->camera->CurrentFloor)->Enabled(chkFloor->GetValue());
}

void MeshControl::On_chkElevators_Click(wxCommandEvent& event)
{
	if (chkElevators->GetValue() == true)
	{
		for (int i = 1; i <= sbs->Elevators(); i++)
			sbs->GetElevator(i)->Enabled(true);
	}
	else
	{
		for (int i = 1; i <= sbs->Elevators(); i++)
			sbs->GetElevator(i)->Enabled(false);
	}
}
