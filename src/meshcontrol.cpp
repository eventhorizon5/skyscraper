//(*InternalHeaders(MeshControl)
#include <wx/bitmap.h>
#include <wx/font.h>
#include <wx/fontenum.h>
#include <wx/fontmap.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/settings.h>
//*)

/* $Id$ */

/*
	Skyscraper 1.1 Alpha - Mesh Control Form
	Copyright (C)2005-2007 Ryan Thoryk
	http://www.tliquest.net/skyscraper
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
const long MeshControl::ID_bOk = wxNewId();
const long MeshControl::ID_chkFloor = wxNewId();
const long MeshControl::ID_STATICTEXT2 = wxNewId();
const long MeshControl::ID_chkColumnFrame = wxNewId();
const long MeshControl::ID_chkSky = wxNewId();
const long MeshControl::ID_chkLandscape = wxNewId();
const long MeshControl::ID_chkBuildings = wxNewId();
const long MeshControl::ID_chkExternal = wxNewId();
const long MeshControl::ID_STATICTEXT1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(MeshControl,wxDialog)
//(*EventTable(MeshControl)
//*)
END_EVENT_TABLE()

MeshControl::MeshControl(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(MeshControl)
	Create(parent,id,_("Realtime Mesh Control"),wxDefaultPosition,wxDefaultSize,wxCAPTION|wxTHICK_FRAME|wxSYSTEM_MENU|wxRESIZE_BOX|wxCLOSE_BOX|wxMINIMIZE_BOX,_T("id"));
	bOk = new wxButton(this,ID_bOk,_("OK"),wxPoint(75,145),wxDefaultSize,0,wxDefaultValidator,_T("ID_bOk"));
	chkFloor = new wxCheckBox(this,ID_chkFloor,_("Level"),wxPoint(139,26),wxDefaultSize,0,wxDefaultValidator,_T("ID_chkFloor"));
	chkFloor->SetValue(false);
	StaticText2 = new wxStaticText(this,ID_STATICTEXT2,_("Per-Floor Meshes"),wxPoint(138,7),wxDefaultSize,0,_T("ID_STATICTEXT2"));
	chkColumnFrame = new wxCheckBox(this,ID_chkColumnFrame,_("Column Frame"),wxPoint(7,99),wxDefaultSize,0,wxDefaultValidator,_T("ID_chkColumnFrame"));
	chkColumnFrame->SetValue(false);
	chkSky = new wxCheckBox(this,ID_chkSky,_("Sky"),wxPoint(7,80),wxDefaultSize,0,wxDefaultValidator,_T("ID_chkSky"));
	chkSky->SetValue(false);
	chkLandscape = new wxCheckBox(this,ID_chkLandscape,_("Landscape"),wxPoint(7,62),wxDefaultSize,0,wxDefaultValidator,_T("ID_chkLandscape"));
	chkLandscape->SetValue(false);
	chkBuildings = new wxCheckBox(this,ID_chkBuildings,_("Buildings"),wxPoint(7,45),wxDefaultSize,0,wxDefaultValidator,_T("ID_chkBuildings"));
	chkBuildings->SetValue(false);
	chkExternal = new wxCheckBox(this,ID_chkExternal,_("External"),wxPoint(7,26),wxDefaultSize,0,wxDefaultValidator,_T("ID_chkExternal"));
	chkExternal->SetValue(false);
	StaticText1 = new wxStaticText(this,ID_STATICTEXT1,_("Primary Meshes"),wxPoint(7,8),wxSize(83,13),0,_T("ID_STATICTEXT1"));
	Connect(ID_bOk,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MeshControl::On_bOk_Click);
	Connect(ID_chkFloor,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MeshControl::On_chkFloor_Change);
	Connect(ID_chkColumnFrame,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MeshControl::On_chkColumnFrame_Change);
	Connect(ID_chkSky,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MeshControl::On_chkSky_Change);
	Connect(ID_chkLandscape,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MeshControl::On_chkLandscape_Change);
	Connect(ID_chkBuildings,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MeshControl::On_chkBuildings_Change);
	Connect(ID_chkExternal,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MeshControl::On_chkExternal_Change);
	//*)
	p = this;
	OnInit();
}

MeshControl::~MeshControl()
{

}


void MeshControl::OnInit()
{

}

void MeshControl::On_bOk_Click(wxCommandEvent& event)
{
	p->Hide();
}

void MeshControl::On_chkColumnFrame_Change(wxCommandEvent& event)
{
	sbs->EnableColumnFrame(chkColumnFrame->GetValue());
}

void MeshControl::On_chkSky_Change(wxCommandEvent& event)
{
	sbs->EnableSkybox(chkSky->GetValue());
}

void MeshControl::On_chkLandscape_Change(wxCommandEvent& event)
{
	sbs->EnableLandscape(chkLandscape->GetValue());
}

void MeshControl::On_chkBuildings_Change(wxCommandEvent& event)
{
	sbs->EnableBuildings(chkBuildings->GetValue());
}

void MeshControl::On_chkExternal_Change(wxCommandEvent& event)
{
	sbs->EnableExternal(chkExternal->GetValue());
}

void MeshControl::On_chkFloor_Change(wxCommandEvent& event)
{
	sbs->GetFloor(sbs->camera->CurrentFloor)->Enabled(chkFloor->GetValue());
}
