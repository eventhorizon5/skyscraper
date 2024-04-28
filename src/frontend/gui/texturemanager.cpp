/*
	Skyscraper 1.12 Alpha - Texture Manager Dialog
	Copyright (C)2003-2024 Ryan Thoryk
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

//(*InternalHeaders(TextureManager)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include "globals.h"
#include "sbs.h"
#include "texture.h"
#include "debugpanel.h"
#include "skyscraper.h"
#include "texturemanager.h"

namespace Skyscraper {

//(*IdInit(TextureManager)
const long TextureManager::ID_TextureList = wxNewId();
const long TextureManager::ID_STATICTEXT4 = wxNewId();
const long TextureManager::ID_tMaterial = wxNewId();
const long TextureManager::ID_STATICTEXT1 = wxNewId();
const long TextureManager::ID_tFilename = wxNewId();
const long TextureManager::ID_STATICTEXT5 = wxNewId();
const long TextureManager::ID_tWidthMult = wxNewId();
const long TextureManager::ID_STATICTEXT6 = wxNewId();
const long TextureManager::ID_tHeightMult = wxNewId();
const long TextureManager::ID_STATICTEXT2 = wxNewId();
const long TextureManager::ID_tEnableForce = wxNewId();
const long TextureManager::ID_STATICTEXT3 = wxNewId();
const long TextureManager::ID_tForceMode = wxNewId();
const long TextureManager::ID_bUnload = wxNewId();
const long TextureManager::ID_bOK = wxNewId();
const long TextureManager::ID_bFree = wxNewId();
//*)

BEGIN_EVENT_TABLE(TextureManager,wxDialog)
    //(*EventTable(TextureManager)
    //*)
END_EVENT_TABLE()

TextureManager::TextureManager(DebugPanel* parent,wxWindowID id)
{
    //(*Initialize(TextureManager)
    wxBoxSizer* BoxSizer1;
    wxFlexGridSizer* FlexGridSizer1;
    wxFlexGridSizer* FlexGridSizer2;
    wxFlexGridSizer* FlexGridSizer3;
    wxFlexGridSizer* FlexGridSizer4;

    Create(parent, wxID_ANY, _("Texture Manager"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
    FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
    FlexGridSizer3 = new wxFlexGridSizer(0, 2, 0, 0);
    TextureList = new wxListBox(this, ID_TextureList, wxDefaultPosition, wxSize(200,300), 0, 0, wxVSCROLL, wxDefaultValidator, _T("ID_TextureList"));
    FlexGridSizer3->Add(TextureList, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer2 = new wxFlexGridSizer(0, 1, 0, 0);
    FlexGridSizer4 = new wxFlexGridSizer(0, 2, 0, 0);
    StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Material:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    FlexGridSizer4->Add(StaticText4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tMaterial = new wxTextCtrl(this, ID_tMaterial, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_tMaterial"));
    FlexGridSizer4->Add(tMaterial, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Filename:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    FlexGridSizer4->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tFilename = new wxTextCtrl(this, ID_tFilename, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_tFilename"));
    FlexGridSizer4->Add(tFilename, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("WidthMult:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    FlexGridSizer4->Add(StaticText5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tWidthMult = new wxTextCtrl(this, ID_tWidthMult, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_tWidthMult"));
    FlexGridSizer4->Add(tWidthMult, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("HeightMult:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
    FlexGridSizer4->Add(StaticText6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tHeightMult = new wxTextCtrl(this, ID_tHeightMult, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_tHeightMult"));
    FlexGridSizer4->Add(tHeightMult, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Enable Force:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    FlexGridSizer4->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tEnableForce = new wxTextCtrl(this, ID_tEnableForce, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_tEnableForce"));
    FlexGridSizer4->Add(tEnableForce, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Force Mode:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    FlexGridSizer4->Add(StaticText3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tForceMode = new wxTextCtrl(this, ID_tForceMode, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_tForceMode"));
    FlexGridSizer4->Add(tForceMode, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer2->Add(FlexGridSizer4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    bUnload = new wxButton(this, ID_bUnload, _("Unload"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bUnload"));
    FlexGridSizer2->Add(bUnload, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3->Add(FlexGridSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(FlexGridSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    bOK = new wxButton(this, ID_bOK, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bOK"));
    BoxSizer1->Add(bOK, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    bFree = new wxButton(this, ID_bFree, _("Free Boxes"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bFree"));
    BoxSizer1->Add(bFree, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(BoxSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(FlexGridSizer1);
    FlexGridSizer1->SetSizeHints(this);
    Center();

    Connect(ID_bUnload,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&TextureManager::On_bUnload_Click);
    Connect(ID_bOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&TextureManager::On_bOK_Click);
    Connect(ID_bFree,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&TextureManager::On_bFree_Click);
    //*)

    lastcount = 0;
    Simcore = 0;
    panel = parent;
}

TextureManager::~TextureManager()
{
    //(*Destroy(TextureManager)
    //*)
}

void TextureManager::Loop()
{
	if (Simcore != panel->GetSystem())
	{
		//if active engine has changed, refresh values
		Simcore = panel->GetSystem();
		lastcount = 0;
	}

	if (!Simcore)
		return;

	if (Simcore->GetTextureManager()->GetTextureCount() != lastcount)
	{
		lastcount = Simcore->GetTextureManager()->GetTextureCount();

		//clear values
		TextureList->Clear();
		tMaterial->Clear();
		tFilename->Clear();
		tEnableForce->Clear();
		tForceMode->Clear();
		tHeightMult->Clear();
		tWidthMult->Clear();

		//for (int i = 0; i < Simcore->GetTextureManager()->GetTextureCount(); i++)
			//TextureList->Append(ToString(i + 1) + wxT(" - ") + Simcore->GetAction(i)->GetName());
	}
}

void TextureManager::On_bUnload_Click(wxCommandEvent& event)
{
}

void TextureManager::On_bOK_Click(wxCommandEvent& event)
{
	this->Close();
}

void TextureManager::On_bFree_Click(wxCommandEvent& event)
{
}

}
