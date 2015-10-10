/* $Id$ */

/*
	Skyscraper 1.10 Alpha - Mesh Control Form
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

//(*InternalHeaders(MeshControl)
#include <wx/string.h>
#include <wx/intl.h>
//*)
#include "debugpanel.h"
#include "meshcontrol.h"
#include "globals.h"
#include "sbs.h"
#include "camera.h"
#include "floor.h"
#include "elevator.h"

extern SBS::SBS *Simcore; //external pointer to the SBS engine
MeshControl *p; //self pointer

//(*IdInit(MeshControl)
const long MeshControl::ID_chkExternal = wxNewId();
const long MeshControl::ID_chkBuildings = wxNewId();
const long MeshControl::ID_chkLandscape = wxNewId();
const long MeshControl::ID_chkSky = wxNewId();
const long MeshControl::ID_chkElevators = wxNewId();
const long MeshControl::ID_chkFloor = wxNewId();
const long MeshControl::ID_chkShafts = wxNewId();
const long MeshControl::ID_chkStairs = wxNewId();
const long MeshControl::ID_chkInterfloor = wxNewId();
const long MeshControl::ID_chkColumnFrame = wxNewId();
const long MeshControl::ID_chkAllFloors = wxNewId();
const long MeshControl::ID_chkAllShafts = wxNewId();
const long MeshControl::ID_chkAllStairs = wxNewId();
const long MeshControl::ID_chkAllInterfloors = wxNewId();
const long MeshControl::ID_chkAllColumnFrames = wxNewId();
const long MeshControl::ID_bOk = wxNewId();
//*)

BEGIN_EVENT_TABLE(MeshControl,wxDialog)
	//(*EventTable(MeshControl)
	//*)
END_EVENT_TABLE()

MeshControl::MeshControl(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(MeshControl)
	wxStaticBoxSizer* StaticBoxSizer2;
	wxBoxSizer* BoxSizer3;
	wxStaticBoxSizer* StaticBoxSizer3;
	wxStaticBoxSizer* StaticBoxSizer1;

	Create(parent, wxID_ANY, _("Realtime Object Control"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, this, _("Global Objects"));
	chkExternal = new wxCheckBox(this, ID_chkExternal, _("External"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkExternal"));
	chkExternal->SetValue(false);
	StaticBoxSizer1->Add(chkExternal, 0, wxTOP|wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkBuildings = new wxCheckBox(this, ID_chkBuildings, _("Buildings"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkBuildings"));
	chkBuildings->SetValue(false);
	StaticBoxSizer1->Add(chkBuildings, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkLandscape = new wxCheckBox(this, ID_chkLandscape, _("Landscape"), wxDefaultPosition, wxSize(100,-1), 0, wxDefaultValidator, _T("ID_chkLandscape"));
	chkLandscape->SetValue(false);
	StaticBoxSizer1->Add(chkLandscape, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkSky = new wxCheckBox(this, ID_chkSky, _("Sky"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkSky"));
	chkSky->SetValue(false);
	StaticBoxSizer1->Add(chkSky, 0, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkElevators = new wxCheckBox(this, ID_chkElevators, _("Elevators"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkElevators"));
	chkElevators->SetValue(false);
	StaticBoxSizer1->Add(chkElevators, 1, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(StaticBoxSizer1, 1, wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxVERTICAL, this, _("Per-Floor Objects"));
	chkFloor = new wxCheckBox(this, ID_chkFloor, _("Level"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkFloor"));
	chkFloor->SetValue(false);
	StaticBoxSizer2->Add(chkFloor, 0, wxTOP|wxBOTTOM|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkShafts = new wxCheckBox(this, ID_chkShafts, _("Shafts"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkShafts"));
	chkShafts->SetValue(false);
	StaticBoxSizer2->Add(chkShafts, 1, wxBOTTOM|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkStairs = new wxCheckBox(this, ID_chkStairs, _("Stairs"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkStairs"));
	chkStairs->SetValue(false);
	StaticBoxSizer2->Add(chkStairs, 1, wxBOTTOM|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkInterfloor = new wxCheckBox(this, ID_chkInterfloor, _("Interfloor"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkInterfloor"));
	chkInterfloor->SetValue(false);
	StaticBoxSizer2->Add(chkInterfloor, 1, wxBOTTOM|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkColumnFrame = new wxCheckBox(this, ID_chkColumnFrame, _("Column Frame"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkColumnFrame"));
	chkColumnFrame->SetValue(false);
	StaticBoxSizer2->Add(chkColumnFrame, 1, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3->Add(StaticBoxSizer2, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer3 = new wxStaticBoxSizer(wxVERTICAL, this, _("Grouped Objects"));
	chkAllFloors = new wxCheckBox(this, ID_chkAllFloors, _("All Floors"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkAllFloors"));
	chkAllFloors->SetValue(false);
	StaticBoxSizer3->Add(chkAllFloors, 1, wxTOP|wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkAllShafts = new wxCheckBox(this, ID_chkAllShafts, _("All Shafts"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkAllShafts"));
	chkAllShafts->SetValue(false);
	StaticBoxSizer3->Add(chkAllShafts, 1, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkAllStairs = new wxCheckBox(this, ID_chkAllStairs, _("All Stairs"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkAllStairs"));
	chkAllStairs->SetValue(false);
	StaticBoxSizer3->Add(chkAllStairs, 1, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkAllInterfloors = new wxCheckBox(this, ID_chkAllInterfloors, _("All Interfloors"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkAllInterfloors"));
	chkAllInterfloors->SetValue(false);
	StaticBoxSizer3->Add(chkAllInterfloors, 1, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	chkAllColumnFrames = new wxCheckBox(this, ID_chkAllColumnFrames, _("All Column Frames"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkAllColumnFrames"));
	chkAllColumnFrames->SetValue(false);
	StaticBoxSizer3->Add(chkAllColumnFrames, 1, wxBOTTOM|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3->Add(StaticBoxSizer3, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(BoxSizer3, 1, wxLEFT|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
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
	Connect(ID_chkElevators,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MeshControl::On_chkElevators_Click);
	Connect(ID_chkFloor,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MeshControl::On_chkFloor_Click);
	Connect(ID_chkShafts,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MeshControl::On_chkShafts_Click);
	Connect(ID_chkStairs,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MeshControl::On_chkStairs_Click);
	Connect(ID_chkInterfloor,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MeshControl::On_chkInterfloor_Click);
	Connect(ID_chkColumnFrame,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MeshControl::On_chkColumnFrame_Click);
	Connect(ID_chkAllFloors,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MeshControl::On_chkAllFloors_Click);
	Connect(ID_chkAllShafts,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MeshControl::On_chkAllShafts_Click);
	Connect(ID_chkAllStairs,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MeshControl::On_chkAllStairs_Click);
	Connect(ID_chkAllInterfloors,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MeshControl::On_chkAllInterfloors_Click);
	Connect(ID_chkAllColumnFrames,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MeshControl::On_chkAllColumnFrames_Click);
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

void MeshControl::On_chkSky_Click(wxCommandEvent& event)
{
	Simcore->EnableSkybox(chkSky->GetValue());
}

void MeshControl::On_chkLandscape_Click(wxCommandEvent& event)
{
	Simcore->EnableLandscape(chkLandscape->GetValue());
}

void MeshControl::On_chkBuildings_Click(wxCommandEvent& event)
{
	Simcore->EnableBuildings(chkBuildings->GetValue());
}

void MeshControl::On_chkExternal_Click(wxCommandEvent& event)
{
	Simcore->EnableExternal(chkExternal->GetValue());
}

void MeshControl::On_chkFloor_Click(wxCommandEvent& event)
{
	if (Simcore->GetFloor(Simcore->camera->CurrentFloor))
		Simcore->GetFloor(Simcore->camera->CurrentFloor)->Enabled(chkFloor->GetValue());
}

void MeshControl::On_chkElevators_Click(wxCommandEvent& event)
{
	if (chkElevators->GetValue() == true)
	{
		for (int i = 1; i <= Simcore->Elevators(); i++)
		{
			if (Simcore->GetElevator(i))
				Simcore->GetElevator(i)->Enabled(true);
		}
	}
	else
	{
		for (int i = 1; i <= Simcore->Elevators(); i++)
		{
			if (Simcore->GetElevator(i))
				Simcore->GetElevator(i)->Enabled(false);
		}
	}
}

void MeshControl::On_chkShafts_Click(wxCommandEvent& event)
{
	for (int i = 1; i <= Simcore->Shafts(); i++)
	{
		if (Simcore->GetShaft(i))
			Simcore->GetShaft(i)->Enabled(Simcore->camera->CurrentFloor, chkShafts->GetValue(), true);
	}
}

void MeshControl::On_chkStairs_Click(wxCommandEvent& event)
{
	for (int i = 1; i <= Simcore->StairsNum(); i++)
	{
		if (Simcore->GetStairs(i))
			Simcore->GetStairs(i)->Enabled(Simcore->camera->CurrentFloor, chkStairs->GetValue());
	}
}

void MeshControl::On_chkAllFloors_Click(wxCommandEvent& event)
{
	for (int i = -Simcore->Basements; i < Simcore->Floors; i++)
	{
		if (Simcore->GetFloor(i))
			Simcore->GetFloor(i)->Enabled(chkAllFloors->GetValue());
	}
}

void MeshControl::On_chkAllShafts_Click(wxCommandEvent& event)
{
	for (int i = 1; i <= Simcore->Shafts(); i++)
	{
		for (int j = -Simcore->Basements; j <= Simcore->Floors; j++)
		{
			if (Simcore->GetShaft(i))
				Simcore->GetShaft(i)->Enabled(j, chkAllShafts->GetValue(), true);
		}
	}
}

void MeshControl::On_chkAllStairs_Click(wxCommandEvent& event)
{
	for (int i = 1; i <= Simcore->StairsNum(); i++)
	{
		for (int j = -Simcore->Basements; j <= Simcore->Floors; j++)
		{
			if (Simcore->GetStairs(i))
				Simcore->GetStairs(i)->Enabled(j, chkAllStairs->GetValue());
		}
	}
}

void MeshControl::On_chkColumnFrame_Click(wxCommandEvent& event)
{
	if (Simcore->GetFloor(Simcore->camera->CurrentFloor))
		Simcore->GetFloor(Simcore->camera->CurrentFloor)->EnableColumnFrame(chkColumnFrame->GetValue());
}

void MeshControl::On_chkAllColumnFrames_Click(wxCommandEvent& event)
{
	for (int i = -Simcore->Basements; i < Simcore->Floors; i++)
	{
		if (Simcore->GetFloor(i))
			Simcore->GetFloor(i)->EnableColumnFrame(chkAllColumnFrames->GetValue());
	}
}

void MeshControl::On_chkInterfloor_Click(wxCommandEvent& event)
{
	if (Simcore->GetFloor(Simcore->camera->CurrentFloor))
		Simcore->GetFloor(Simcore->camera->CurrentFloor)->EnableInterfloor(chkInterfloor->GetValue());
}

void MeshControl::On_chkAllInterfloors_Click(wxCommandEvent& event)
{
	for (int i = -Simcore->Basements; i < Simcore->Floors; i++)
	{
		if (Simcore->GetFloor(i))
			Simcore->GetFloor(i)->EnableInterfloor(chkAllInterfloors->GetValue());
	}
}
