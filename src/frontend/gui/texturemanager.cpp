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
const long TextureManager::ID_STATICTEXT7 = wxNewId();
const long TextureManager::ID_tDependencies = wxNewId();
const long TextureManager::ID_STATICTEXT4 = wxNewId();
const long TextureManager::ID_tMaterial = wxNewId();
const long TextureManager::ID_STATICTEXT1 = wxNewId();
const long TextureManager::ID_tFilename = wxNewId();
const long TextureManager::ID_STATICTEXT8 = wxNewId();
const long TextureManager::ID_tWidth = wxNewId();
const long TextureManager::ID_STATICTEXT9 = wxNewId();
const long TextureManager::ID_tHeight = wxNewId();
const long TextureManager::ID_STATICTEXT11 = wxNewId();
const long TextureManager::ID_tAlpha = wxNewId();
const long TextureManager::ID_STATICTEXT12 = wxNewId();
const long TextureManager::ID_tMipMaps = wxNewId();
const long TextureManager::ID_STATICTEXT10 = wxNewId();
const long TextureManager::ID_tMemory = wxNewId();
const long TextureManager::ID_STATICTEXT5 = wxNewId();
const long TextureManager::ID_tWidthMult = wxNewId();
const long TextureManager::ID_STATICTEXT6 = wxNewId();
const long TextureManager::ID_tHeightMult = wxNewId();
const long TextureManager::ID_STATICTEXT2 = wxNewId();
const long TextureManager::ID_chkEnableForce = wxNewId();
const long TextureManager::ID_STATICTEXT3 = wxNewId();
const long TextureManager::ID_chkForceMode = wxNewId();
const long TextureManager::ID_bSave = wxNewId();
const long TextureManager::ID_bExport = wxNewId();
const long TextureManager::ID_bUnload = wxNewId();
const long TextureManager::ID_bOK = wxNewId();
//*)

BEGIN_EVENT_TABLE(TextureManager,wxDialog)
    //(*EventTable(TextureManager)
    //*)
END_EVENT_TABLE()

TextureManager::TextureManager(DebugPanel* parent,wxWindowID id)
{
    //(*Initialize(TextureManager)
    wxBoxSizer* BoxSizer1;
    wxBoxSizer* BoxSizer2;
    wxFlexGridSizer* FlexGridSizer1;
    wxFlexGridSizer* FlexGridSizer2;
    wxFlexGridSizer* FlexGridSizer3;
    wxFlexGridSizer* FlexGridSizer4;

    Create(parent, wxID_ANY, _("Texture Manager"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
    FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
    FlexGridSizer3 = new wxFlexGridSizer(0, 2, 0, 0);
    TextureList = new wxListBox(this, ID_TextureList, wxDefaultPosition, wxSize(200,400), 0, 0, wxVSCROLL, wxDefaultValidator, _T("ID_TextureList"));
    FlexGridSizer3->Add(TextureList, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer2 = new wxFlexGridSizer(0, 1, 0, 0);
    FlexGridSizer4 = new wxFlexGridSizer(0, 2, 0, 0);
    StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Dependencies:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
    FlexGridSizer4->Add(StaticText7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tDependencies = new wxTextCtrl(this, ID_tDependencies, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tDependencies"));
    FlexGridSizer4->Add(tDependencies, 1, wxALL|wxEXPAND, 5);
    StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Material:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    FlexGridSizer4->Add(StaticText4, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tMaterial = new wxTextCtrl(this, ID_tMaterial, wxEmptyString, wxDefaultPosition, wxSize(200,-1), wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tMaterial"));
    FlexGridSizer4->Add(tMaterial, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Filename:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    FlexGridSizer4->Add(StaticText1, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tFilename = new wxTextCtrl(this, ID_tFilename, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tFilename"));
    FlexGridSizer4->Add(tFilename, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
    StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("Width:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
    FlexGridSizer4->Add(StaticText8, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tWidth = new wxTextCtrl(this, ID_tWidth, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tWidth"));
    FlexGridSizer4->Add(tWidth, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
    StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _("Height:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
    FlexGridSizer4->Add(StaticText9, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tHeight = new wxTextCtrl(this, ID_tHeight, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tHeight"));
    FlexGridSizer4->Add(tHeight, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
    StaticText11 = new wxStaticText(this, ID_STATICTEXT11, _("Has Alpha:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
    FlexGridSizer4->Add(StaticText11, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tAlpha = new wxTextCtrl(this, ID_tAlpha, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tAlpha"));
    FlexGridSizer4->Add(tAlpha, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
    StaticText12 = new wxStaticText(this, ID_STATICTEXT12, _("MipMaps:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT12"));
    FlexGridSizer4->Add(StaticText12, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tMipMaps = new wxTextCtrl(this, ID_tMipMaps, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tMipMaps"));
    FlexGridSizer4->Add(tMipMaps, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
    StaticText10 = new wxStaticText(this, ID_STATICTEXT10, _("Memory:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
    FlexGridSizer4->Add(StaticText10, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tMemory = new wxTextCtrl(this, ID_tMemory, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tMemory"));
    FlexGridSizer4->Add(tMemory, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
    StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("WidthMult:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    FlexGridSizer4->Add(StaticText5, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tWidthMult = new wxTextCtrl(this, ID_tWidthMult, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE, wxDefaultValidator, _T("ID_tWidthMult"));
    FlexGridSizer4->Add(tWidthMult, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
    StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("HeightMult:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
    FlexGridSizer4->Add(StaticText6, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    tHeightMult = new wxTextCtrl(this, ID_tHeightMult, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE, wxDefaultValidator, _T("ID_tHeightMult"));
    FlexGridSizer4->Add(tHeightMult, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Enable Force:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    FlexGridSizer4->Add(StaticText2, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    chkEnableForce = new wxCheckBox(this, ID_chkEnableForce, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkEnableForce"));
    chkEnableForce->SetValue(false);
    FlexGridSizer4->Add(chkEnableForce, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Force Mode:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    FlexGridSizer4->Add(StaticText3, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    chkForceMode = new wxCheckBox(this, ID_chkForceMode, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkForceMode"));
    chkForceMode->SetValue(false);
    FlexGridSizer4->Add(chkForceMode, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer2->Add(FlexGridSizer4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    bSave = new wxButton(this, ID_bSave, _("Save"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bSave"));
    BoxSizer2->Add(bSave, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    bExport = new wxButton(this, ID_bExport, _("Export"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bExport"));
    BoxSizer2->Add(bExport, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    bUnload = new wxButton(this, ID_bUnload, _("Unload"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bUnload"));
    BoxSizer2->Add(bUnload, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer2->Add(BoxSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3->Add(FlexGridSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(FlexGridSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    bOK = new wxButton(this, ID_bOK, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bOK"));
    BoxSizer1->Add(bOK, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(BoxSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(FlexGridSizer1);
    FlexGridSizer1->SetSizeHints(this);
    Center();

    Connect(ID_TextureList,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&TextureManager::On_TextureList_Select);
    Connect(ID_bSave,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&TextureManager::On_bSave_Click);
    Connect(ID_bExport,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&TextureManager::On_bExport_Click);
    Connect(ID_bUnload,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&TextureManager::On_bUnload_Click);
    Connect(ID_bOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&TextureManager::On_bOK_Click);
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

	SBS::TextureManager *manager = Simcore->GetTextureManager();

	if (!manager)
		return;

	if (manager->GetTextureInfoCount() != lastcount)
	{
		lastcount = manager->GetTextureInfoCount();

		//clear values
		TextureList->Clear();
		tMaterial->Clear();
		tFilename->Clear();
		chkEnableForce->SetValue(false);
		chkForceMode->SetValue(false);
		tHeightMult->Clear();
		tWidthMult->Clear();
		tDependencies->Clear();
		tWidth->Clear();
		tHeight->Clear();
		tAlpha->Clear();
		tMipMaps->Clear();
		tMemory->Clear();

		for (int i = 0; i < manager->GetTextureInfoCount(); i++)
		{
			SBS::TextureManager::TextureInfo info;
			if (manager->GetTextureInfo(i, info))
				TextureList->Append(SBS::ToString(i + 1) + wxT(" - ") + info.name);
		}
	}
}

void TextureManager::On_bUnload_Click(wxCommandEvent& event)
{
	int selection = TextureList->GetSelection();
	if (selection < 0)
		return;

	SBS::TextureManager::TextureInfo texture;
	Simcore->GetTextureManager()->GetTextureInfo(selection, texture);

	if (texture.name != "")
	{
		//don't unload texture if submeshes are using it
		if (texture.dependencies > 0)
			return;

		bool result = Simcore->GetTextureManager()->UnloadMaterial(texture.name, "General");
		if (result == true)
			Simcore->GetTextureManager()->UnregisterTextureInfo(texture.name, texture.material_name);
	}
}

void TextureManager::On_bOK_Click(wxCommandEvent& event)
{
	this->Close();
}

void TextureManager::On_TextureList_Select(wxCommandEvent& event)
{
	int selection = TextureList->GetSelection();
	if (selection < 0)
		return;

	SBS::TextureManager::TextureInfo texture;
	Simcore->GetTextureManager()->GetTextureInfo(selection, texture);

	if (texture.name != "")
	{
		tMaterial->SetValue(texture.material_name);
		tFilename->SetValue(texture.filename);
		tWidthMult->SetValue(SBS::TruncateNumber(texture.widthmult, 2));
		tHeightMult->SetValue(SBS::TruncateNumber(texture.heightmult, 2));
		chkEnableForce->SetValue(texture.enable_force);
		chkForceMode->SetValue(texture.force_mode);
		tDependencies->SetValue(SBS::ToString(texture.dependencies));

		std::string name = GetTextureName(texture.name);

		if (name.empty())
			return;

		//get raw texture
		Ogre::TexturePtr tex = Ogre::TextureManager::getSingleton().getByName(name, "General");
		if (tex)
		{
			tWidth->SetValue(SBS::ToString((int)tex->getWidth()));
			tHeight->SetValue(SBS::ToString((int)tex->getHeight()));
			tMipMaps->SetValue(SBS::ToString((int)tex->getNumMipmaps()));
			tMemory->SetValue(SBS::ToString((int)tex->getSize()));

			if (tex->hasAlpha() == true)
				tAlpha->SetValue("true");
			else
				tAlpha->SetValue("false");
		}
	}
}

void TextureManager::On_bSave_Click(wxCommandEvent& event)
{
	int selection = TextureList->GetSelection();
	if (selection < 0)
		return;

	SBS::TextureManager::TextureInfo texture;
	Simcore->GetTextureManager()->GetTextureInfo(selection, texture);

	if (texture.name != "")
	{
		texture.widthmult = atof(tWidthMult->GetValue());
		texture.heightmult = atof(tHeightMult->GetValue());
		texture.enable_force = chkEnableForce->GetValue();
		texture.force_mode = chkForceMode->GetValue();

		Simcore->GetTextureManager()->SetTextureInfo(selection, texture);
	}
}

void TextureManager::On_bExport_Click(wxCommandEvent& event)
{
	int selection = TextureList->GetSelection();
	if (selection < 0)
		return;

	SBS::TextureManager::TextureInfo texture;
	Simcore->GetTextureManager()->GetTextureInfo(selection, texture);

	if (texture.name != "")
	{
		std::string name = GetTextureName(texture.name);

		if (name.empty())
			return;

		//get raw texture
		Ogre::TexturePtr tex = Ogre::TextureManager::getSingleton().getByName(name, "General");
		if (tex)
			Simcore->GetTextureManager()->SaveTexture(tex, "texture.png");
	}
}

std::string TextureManager::GetTextureName(const std::string &material)
{
	if (material != "")
	{
		//get material
		Ogre::MaterialPtr mat = Simcore->GetTextureManager()->GetMaterialByName(material);

		Ogre::TextureUnitState *state = 0;
		std::string name;
		if (mat)
			state = mat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
		if (state)
			return state->getTextureName();
	}

	return "";
}

}
