/* $Id$ */

/*
	Skyscraper 1.9 Alpha - Parameter Viewer Dialog
	Copyright (C)2003-2013 Ryan Thoryk
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

//(*InternalHeaders(ParameterViewer)
#include <wx/string.h>
#include <wx/intl.h>
//*)
#include <wx/filedlg.h>
#include <wx/app.h>

#include "globals.h"
#include "sbs.h"
#include "unix.h"
#include "fileio.h"
#include "skyscraper.h"
#include "objectinfo.h"
#include "textwindow.h"
#include "parameterviewer.h"

extern SBS *Simcore; //external pointer to the SBS engine
extern Skyscraper *skyscraper;
extern ObjectInfo *objectinfo;

//(*IdInit(ParameterViewer)
const long ParameterViewer::ID_tDesc = wxNewId();
const long ParameterViewer::ID_tVariables = wxNewId();
const long ParameterViewer::ID_STATICTEXT1 = wxNewId();
const long ParameterViewer::ID_TEXTCTRL1 = wxNewId();
const long ParameterViewer::ID_STATICTEXT2 = wxNewId();
const long ParameterViewer::ID_TEXTCTRL2 = wxNewId();
const long ParameterViewer::ID_STATICTEXT3 = wxNewId();
const long ParameterViewer::ID_TEXTCTRL3 = wxNewId();
const long ParameterViewer::ID_STATICTEXT4 = wxNewId();
const long ParameterViewer::ID_TEXTCTRL4 = wxNewId();
const long ParameterViewer::ID_STATICTEXT5 = wxNewId();
const long ParameterViewer::ID_TEXTCTRL5 = wxNewId();
const long ParameterViewer::ID_STATICTEXT6 = wxNewId();
const long ParameterViewer::ID_TEXTCTRL6 = wxNewId();
const long ParameterViewer::ID_STATICTEXT7 = wxNewId();
const long ParameterViewer::ID_TEXTCTRL7 = wxNewId();
const long ParameterViewer::ID_STATICTEXT8 = wxNewId();
const long ParameterViewer::ID_TEXTCTRL8 = wxNewId();
const long ParameterViewer::ID_STATICTEXT9 = wxNewId();
const long ParameterViewer::ID_TEXTCTRL9 = wxNewId();
const long ParameterViewer::ID_STATICTEXT10 = wxNewId();
const long ParameterViewer::ID_TEXTCTRL10 = wxNewId();
const long ParameterViewer::ID_l11 = wxNewId();
const long ParameterViewer::ID_t11 = wxNewId();
const long ParameterViewer::ID_l12 = wxNewId();
const long ParameterViewer::ID_t12 = wxNewId();
const long ParameterViewer::ID_l13 = wxNewId();
const long ParameterViewer::ID_t13 = wxNewId();
const long ParameterViewer::ID_l14 = wxNewId();
const long ParameterViewer::ID_t14 = wxNewId();
const long ParameterViewer::ID_l15 = wxNewId();
const long ParameterViewer::ID_t15 = wxNewId();
const long ParameterViewer::ID_l16 = wxNewId();
const long ParameterViewer::ID_t16 = wxNewId();
const long ParameterViewer::ID_l17 = wxNewId();
const long ParameterViewer::ID_t17 = wxNewId();
const long ParameterViewer::ID_l18 = wxNewId();
const long ParameterViewer::ID_t18 = wxNewId();
const long ParameterViewer::ID_l19 = wxNewId();
const long ParameterViewer::ID_t19 = wxNewId();
const long ParameterViewer::ID_l20 = wxNewId();
const long ParameterViewer::ID_t20 = wxNewId();
const long ParameterViewer::ID_STATICTEXT11 = wxNewId();
const long ParameterViewer::ID_tCommand = wxNewId();
const long ParameterViewer::ID_bCancel = wxNewId();
const long ParameterViewer::ID_bTextures = wxNewId();
const long ParameterViewer::ID_bOK = wxNewId();
//*)

BEGIN_EVENT_TABLE(ParameterViewer,wxDialog)
	//(*EventTable(ParameterViewer)
	//*)
END_EVENT_TABLE()

ParameterViewer::ParameterViewer(wxWindow* parent, wxString object_type, wxString object_parent, bool createobject, wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(ParameterViewer)
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxBoxSizer* BoxSizer2;
	wxFlexGridSizer* FlexGridSizer3;
	wxBoxSizer* BoxSizer1;
	
	Create(parent, wxID_ANY, _("Parameter Viewer"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
	tDesc = new wxStaticText(this, ID_tDesc, _("Desc"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_tDesc"));
	FlexGridSizer1->Add(tDesc, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	tVariables = new wxStaticText(this, ID_tVariables, _("Variables"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_tVariables"));
	FlexGridSizer1->Add(tVariables, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	FlexGridSizer2 = new wxFlexGridSizer(0, 2, 0, 0);
	l1 = new wxStaticText(this, ID_STATICTEXT1, _("1"), wxDefaultPosition, wxSize(150,-1), 0, _T("ID_STATICTEXT1"));
	FlexGridSizer2->Add(l1, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t1 = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	t1->SetMinSize(wxSize(200,-1));
	FlexGridSizer2->Add(t1, 1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	l2 = new wxStaticText(this, ID_STATICTEXT2, _("1"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	FlexGridSizer2->Add(l2, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t2 = new wxTextCtrl(this, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	FlexGridSizer2->Add(t2, 1, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	l3 = new wxStaticText(this, ID_STATICTEXT3, _("1"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	FlexGridSizer2->Add(l3, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t3 = new wxTextCtrl(this, ID_TEXTCTRL3, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	FlexGridSizer2->Add(t3, 1, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	l4 = new wxStaticText(this, ID_STATICTEXT4, _("1"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	FlexGridSizer2->Add(l4, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t4 = new wxTextCtrl(this, ID_TEXTCTRL4, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
	FlexGridSizer2->Add(t4, 1, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	l5 = new wxStaticText(this, ID_STATICTEXT5, _("1"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	FlexGridSizer2->Add(l5, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t5 = new wxTextCtrl(this, ID_TEXTCTRL5, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL5"));
	FlexGridSizer2->Add(t5, 1, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	l6 = new wxStaticText(this, ID_STATICTEXT6, _("1"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	FlexGridSizer2->Add(l6, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t6 = new wxTextCtrl(this, ID_TEXTCTRL6, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL6"));
	FlexGridSizer2->Add(t6, 1, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	l7 = new wxStaticText(this, ID_STATICTEXT7, _("1"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	FlexGridSizer2->Add(l7, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t7 = new wxTextCtrl(this, ID_TEXTCTRL7, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL7"));
	FlexGridSizer2->Add(t7, 1, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	l8 = new wxStaticText(this, ID_STATICTEXT8, _("1"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	FlexGridSizer2->Add(l8, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t8 = new wxTextCtrl(this, ID_TEXTCTRL8, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL8"));
	FlexGridSizer2->Add(t8, 1, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	l9 = new wxStaticText(this, ID_STATICTEXT9, _("1"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	FlexGridSizer2->Add(l9, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t9 = new wxTextCtrl(this, ID_TEXTCTRL9, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL9"));
	FlexGridSizer2->Add(t9, 1, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	l10 = new wxStaticText(this, ID_STATICTEXT10, _("1"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	FlexGridSizer2->Add(l10, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t10 = new wxTextCtrl(this, ID_TEXTCTRL10, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL10"));
	FlexGridSizer2->Add(t10, 1, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizer2->Add(FlexGridSizer2, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	FlexGridSizer3 = new wxFlexGridSizer(0, 2, 0, 0);
	l11 = new wxStaticText(this, ID_l11, _("1"), wxDefaultPosition, wxSize(150,-1), 0, _T("ID_l11"));
	FlexGridSizer3->Add(l11, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t11 = new wxTextCtrl(this, ID_t11, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_t11"));
	t11->SetMinSize(wxSize(200,-1));
	FlexGridSizer3->Add(t11, 1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	l12 = new wxStaticText(this, ID_l12, _("1"), wxDefaultPosition, wxSize(150,-1), 0, _T("ID_l12"));
	FlexGridSizer3->Add(l12, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t12 = new wxTextCtrl(this, ID_t12, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_t12"));
	t12->SetMinSize(wxSize(200,-1));
	FlexGridSizer3->Add(t12, 1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	l13 = new wxStaticText(this, ID_l13, _("1"), wxDefaultPosition, wxSize(150,-1), 0, _T("ID_l13"));
	FlexGridSizer3->Add(l13, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t13 = new wxTextCtrl(this, ID_t13, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_t13"));
	t13->SetMinSize(wxSize(200,-1));
	FlexGridSizer3->Add(t13, 1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	l14 = new wxStaticText(this, ID_l14, _("1"), wxDefaultPosition, wxSize(150,-1), 0, _T("ID_l14"));
	FlexGridSizer3->Add(l14, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t14 = new wxTextCtrl(this, ID_t14, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_t14"));
	t14->SetMinSize(wxSize(200,-1));
	FlexGridSizer3->Add(t14, 1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	l15 = new wxStaticText(this, ID_l15, _("1"), wxDefaultPosition, wxSize(150,-1), 0, _T("ID_l15"));
	FlexGridSizer3->Add(l15, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t15 = new wxTextCtrl(this, ID_t15, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_t15"));
	t15->SetMinSize(wxSize(200,-1));
	FlexGridSizer3->Add(t15, 1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	l16 = new wxStaticText(this, ID_l16, _("1"), wxDefaultPosition, wxSize(150,-1), 0, _T("ID_l16"));
	FlexGridSizer3->Add(l16, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t16 = new wxTextCtrl(this, ID_t16, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_t16"));
	t16->SetMinSize(wxSize(200,-1));
	FlexGridSizer3->Add(t16, 1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	l17 = new wxStaticText(this, ID_l17, _("1"), wxDefaultPosition, wxSize(150,-1), 0, _T("ID_l17"));
	FlexGridSizer3->Add(l17, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t17 = new wxTextCtrl(this, ID_t17, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_t17"));
	t17->SetMinSize(wxSize(200,-1));
	FlexGridSizer3->Add(t17, 1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	l18 = new wxStaticText(this, ID_l18, _("1"), wxDefaultPosition, wxSize(150,-1), 0, _T("ID_l18"));
	FlexGridSizer3->Add(l18, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t18 = new wxTextCtrl(this, ID_t18, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_t18"));
	t18->SetMinSize(wxSize(200,-1));
	FlexGridSizer3->Add(t18, 1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	l19 = new wxStaticText(this, ID_l19, _("1"), wxDefaultPosition, wxSize(150,-1), 0, _T("ID_l19"));
	FlexGridSizer3->Add(l19, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t19 = new wxTextCtrl(this, ID_t19, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_t19"));
	t19->SetMinSize(wxSize(200,-1));
	FlexGridSizer3->Add(t19, 1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	l20 = new wxStaticText(this, ID_l20, _("1"), wxDefaultPosition, wxSize(150,-1), 0, _T("ID_l20"));
	FlexGridSizer3->Add(l20, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t20 = new wxTextCtrl(this, ID_t20, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_t20"));
	t20->SetMinSize(wxSize(200,-1));
	FlexGridSizer3->Add(t20, 1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizer2->Add(FlexGridSizer3, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer1->Add(BoxSizer2, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT11, _("Script Command:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	FlexGridSizer1->Add(StaticText1, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	tCommand = new wxTextCtrl(this, ID_tCommand, _("Command"), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tCommand"));
	FlexGridSizer1->Add(tCommand, 1, wxALL|wxEXPAND|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	bCancel = new wxButton(this, ID_bCancel, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bCancel"));
	BoxSizer1->Add(bCancel, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bTextures = new wxButton(this, ID_bTextures, _("List Textures"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bTextures"));
	BoxSizer1->Add(bTextures, 1, wxALL|wxALIGN_TOP|wxALIGN_BOTTOM, 5);
	bOK = new wxButton(this, ID_bOK, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bOK"));
	BoxSizer1->Add(bOK, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer1->Add(BoxSizer1, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);
	Center();
	
	Connect(ID_TEXTCTRL1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_TEXTCTRL2,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_TEXTCTRL3,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_TEXTCTRL4,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_TEXTCTRL5,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_TEXTCTRL6,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_TEXTCTRL7,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_TEXTCTRL8,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_TEXTCTRL9,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_TEXTCTRL10,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_t11,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_t12,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_t13,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_t14,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_t15,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_t16,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_t17,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_t18,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_t19,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_t20,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_bCancel,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ParameterViewer::On_bCancel_Click);
	Connect(ID_bTextures,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ParameterViewer::On_bTextures_Click);
	Connect(ID_bOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ParameterViewer::On_bOK_Click);
	//*)
	objecttype = object_type;
	objectparent = object_parent;
	create = createobject;
	Setup();
}

ParameterViewer::~ParameterViewer()
{
	//(*Destroy(ParameterViewer)
	//*)
}

bool ParameterViewer::Setup()
{
	//set up parameter dialog

	if (create == true)
	{
		if (objecttype == wxT("Load") && objectparent == wxT("Texture"))
		{
			SetTitle(wxT("Load Texture"));

			wxString newlabel = wxT("Loads a texture.\n");
			newlabel = newlabel + wxT("Items with an asterisk (*) are optional.\nMove mouse over parameter name for help");

			tDesc->SetLabel(newlabel);

			wxFileDialog *Selector = new wxFileDialog(0, _("Select a Texture"), _("data/"), _(""), _("Image files (*.jpg *.png *.gif *.tga)|*.jpg;*.png;*.gif;*.tga"), wxFD_OPEN);
			int result = Selector->ShowModal();
			if (result == wxID_CANCEL)
			{
				//delete dialog
				delete Selector;
				Selector = 0;
				//quit
				return false;
			}
			wxString filename = wxT("data/") + Selector->GetFilename();

			l1->SetLabel(wxT("Filename:"));
			t1->SetValue(filename);

			l2->SetLabel(wxT("Name:"));
			l2->SetToolTip(wxT("Name of texture"));

			l3->SetLabel(wxT("Tile X"));
			l3->SetToolTip(wxT("The values tile_x and tile_y are per-texture multipliers. For example, if you set tile_x to 2, and you specify a texture width (tw) of 2 during an AddFloor command later, the tiling value will be 4 (2 times 2), and the texture will be repeated 4 times horizontally."));

			l4->SetLabel(wxT("Tile Y"));
			l4->SetToolTip(wxT("The values tile_x and tile_y are per-texture multipliers. For example, if you set tile_x to 2, and you specify a texture width (tw) of 2 during an AddFloor command later, the tiling value will be 4 (2 times 2), and the texture will be repeated 4 times horizontally."));

			l5->SetLabel(wxT("Force*"));
			l5->SetToolTip(wxT("The force value is optional, and if set to false, autosizing will always be disabled for this texture; if set to true, autosizing will always be enabled."));
		}

		if (objecttype == wxT("Floor") && objectparent == wxT("Floor"))
		{
			SetTitle(wxT("Create Floor"));

			t1->SetValue(wxVariant((int)Simcore->Floors).GetString());

			wxString newlabel = wxT("Create a New Floor\nFloors above ground start with 0 (so a 15-story building would have floors 0-14).\nAlso, floors must be made in the proper order: basement levels must be made first in decending order (-1, -2, -3 etc),\nand then above-ground floors in ascending order (0, 1, 2, etc)\n");
			newlabel = newlabel + wxT("Items with an asterisk (*) are optional.\nMove mouse over parameter name for help");

			tDesc->SetLabel(newlabel);
			tVariables->SetLabel(wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n"));

			l1->SetLabel(wxT("Number:"));
			l1->SetToolTip(wxT("Number of new floor (such as 1), or a range of floors (such as 5 to 10)"));

			l2->SetLabel(wxT("Name:"));
			l2->SetToolTip(wxT("the name of the current floor, required\nExample: Name = Floor %floor%"));

			l3->SetLabel(wxT("ID:"));
			l3->SetToolTip(wxT("the floor indicator name for the current floor, such as L (for Lobby), LL (lower level), M (Mezzanine), etc. This is also used to determine what texture should be loaded for the elevator floor indicators and floor signs. The texture name would be 'Button[ID]' - so if the ID is 10, the texture name would be 'Button10'.\nExample: ID = %floor%"));

			l4->SetLabel(wxT("Type:"));
			l4->SetToolTip(wxT("the type of floor the current floor is. The types are still being defined, but the currently used ones are Basement, Lobby, Mezzanine, Conference, Office, Service, Skylobby, Hotel, Apartment, Condominium, Restaurant, Observatory, Recreation, Ballroom, Communications, and Roof. (Required)\nExample: Type = Office"));

			l5->SetLabel(wxT("Description*:"));
			l5->SetToolTip(wxT("description of the current floor, optional\nExample: Description = Offices"));

			l6->SetLabel(wxT("Height:"));
			l6->SetToolTip(wxT("the floor-to-ceiling height of the current floor, required\nExample: Height = 9.5"));

			l7->SetLabel(wxT("InterfloorHeight:"));
			l7->SetToolTip(wxT(" the height in feet of the space between floors (below each floor), starting at the floor's altitude, and ending right below the level's floor; required.\nExample: InterfloorHeight = 2.24"));

			l8->SetLabel(wxT("Altitude*:"));
			l8->SetToolTip(wxT("this parameter is optional and is only recommended if the first level has an interfloor area that needs to be below ground. If this parameter is not used, the altitude will be calculated automatically.\nExample: Altitude = -2.24"));

			l9->SetLabel(wxT("Group*:"));
			l9->SetToolTip(wxT("group floors together. This is a list of comma-separated floor numbers (or a range specified with the - symbol) that should be enabled along with this floor when the user arrives at this floor. For example, if a 2-story room has a balcony, and the room base and balcony are separate floors, you would specify the other floor's number in this parameter.\nExamples:\nGroup = 5\nGroup = 4, 5\nGroup = 4 - 10"));
		}

		if (objecttype == wxT("AddFloor") && objectparent == wxT("Floor"))
		{
			//t1->SetValue(wxVariant((int)Simcore->Floors).GetString());

			SetTitle(wxT("Floor - AddFloor"));

			wxString newlabel = wxT("Adds a textured floor with the specified dimensions to the current floor/level\n");
			newlabel = newlabel + wxT("Items with an asterisk (*) are optional.\nMove mouse over parameter name for help");

			tDesc->SetLabel(newlabel);
			tVariables->SetLabel(wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n"));

			l1->SetLabel(wxT("Floor Number:"));
			l1->SetToolTip(wxT("Number of floor (such as 1), or a range of floors (such as 5 to 10)"));

			l2->SetLabel(wxT("Name:"));

			l3->SetLabel(wxT("Texture Name:"));

			l4->SetLabel(wxT("Thickness:"));

			l5->SetLabel(wxT("X1:"));

			l6->SetLabel(wxT("Z1:"));

			l7->SetLabel(wxT("X2:"));

			l8->SetLabel(wxT("Z2:"));

			l9->SetLabel(wxT("Vertical Offset1:"));
			l9->SetToolTip(wxT("Voffset1 and voffset2 are the height in feet above the current floor's altitude"));

			l10->SetLabel(wxT("Vertical Offset2:"));
			l10->SetToolTip(wxT("Voffset1 and voffset2 are the height in feet above the current floor's altitude"));

			l11->SetLabel(wxT("Reverse Axis:"));
			l11->SetToolTip(wxT("If this is false, a difference in voffset will cause the floor to angle from back to front; otherwise it'll angle from left to right."));
			t11->SetValue(wxT("false"));

			l12->SetLabel(wxT("Texture Direction:"));
			l12->SetToolTip(wxT("If this is false, the texture will be mapped horizontally; otherwise it'll be mapped vertically"));
			t12->SetValue(wxT("false"));

			l13->SetLabel(wxT("Texture Tiling Width:"));
			l13->SetToolTip(wxT("tw and th are to size/tile the texture (0 lets the app autosize them), and isexternal determines if the floor is part of the building's external framework, or is part of the current floor (is either True or False)"));
			t13->SetValue(wxT("0"));

			l14->SetLabel(wxT("Texture Tiling Height:"));
			l14->SetToolTip(wxT("tw and th are to size/tile the texture (0 lets the app autosize them), and isexternal determines if the floor is part of the building's external framework, or is part of the current floor (is either True or False)"));
			t14->SetValue(wxT("0"));

			l15->SetLabel(wxT("Is External:"));
			l15->SetToolTip(wxT("isexternal determines if the wall is part of the building's external framework (true) or if it's part of the current floor (false)"));
			t15->SetValue(wxT("false"));
		}

		if (objecttype == wxT("AddWall") && objectparent == wxT("Floor"))
		{
			//t1->SetValue(wxVariant((int)Simcore->Floors).GetString());

			SetTitle(wxT("Floor - AddWall"));

			wxString newlabel = wxT("Adds a textured wall with the specified dimensions to the current floor/level\n");
			newlabel = newlabel + wxT("Items with an asterisk (*) are optional.\nMove mouse over parameter name for help");

			tDesc->SetLabel(newlabel);
			tVariables->SetLabel(wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n"));

			l1->SetLabel(wxT("Floor Number:"));
			l1->SetToolTip(wxT("Number of floor (such as 1), or a range of floors (such as 5 to 10)"));

			l2->SetLabel(wxT("Name:"));

			l3->SetLabel(wxT("Texture Name:"));

			l4->SetLabel(wxT("Thickness:"));

			l5->SetLabel(wxT("X1:"));

			l6->SetLabel(wxT("Z1:"));

			l7->SetLabel(wxT("X2:"));

			l8->SetLabel(wxT("Z2:"));

			l9->SetLabel(wxT("Height 1:"));
			l9->SetToolTip(wxT("Height1 is the wall height in feet at the first coordinate set (x1 and z1), and height2 is for the second set (x2, and z2)"));

			l10->SetLabel(wxT("Height 2:"));
			l10->SetToolTip(wxT("Height1 is the wall height in feet at the first coordinate set (x1 and z1), and height2 is for the second set (x2, and z2)"));

			l11->SetLabel(wxT("Vertical Offset1:"));
			l11->SetToolTip(wxT("Voffset1 is the vertical offset in feet (from the floor's altitude) for the first coordinate set, and voffset2 is for the second set"));

			l12->SetLabel(wxT("Vertical Offset2:"));
			l12->SetToolTip(wxT("Voffset1 is the vertical offset in feet (from the floor's altitude) for the first coordinate set, and voffset2 is for the second set"));

			l13->SetLabel(wxT("Texture Tiling Width:"));
			l13->SetToolTip(wxT("tw and th are to size/tile the texture (0 lets the app autosize them), and isexternal determines if the floor is part of the building's external framework, or is part of the current floor (is either True or False)"));
			t13->SetValue(wxT("0"));

			l14->SetLabel(wxT("Texture Tiling Height:"));
			l14->SetToolTip(wxT("tw and th are to size/tile the texture (0 lets the app autosize them), and isexternal determines if the floor is part of the building's external framework, or is part of the current floor (is either True or False)"));
			t14->SetValue(wxT("0"));

			l15->SetLabel(wxT("Is External:"));
			l15->SetToolTip(wxT("isexternal determines if the wall is part of the building's external framework (true) or if it's part of the current floor (false)"));
			t15->SetValue(wxT("false"));
		}

		if (objecttype == wxT("AddInterfloorFloor") && objectparent == wxT("Floor"))
		{
			//t1->SetValue(wxVariant((int)Simcore->Floors).GetString());

			SetTitle(wxT("Floor - AddInterfloorFloor"));

			wxString newlabel = wxT("Adds a textured floor with the specified dimensions below the current floor/level\n");
			newlabel = newlabel + wxT("Items with an asterisk (*) are optional.\nMove mouse over parameter name for help");

			tDesc->SetLabel(newlabel);
			tVariables->SetLabel(wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n"));

			l1->SetLabel(wxT("Floor Number:"));
			l1->SetToolTip(wxT("Number of floor (such as 1), or a range of floors (such as 5 to 10)"));

			l2->SetLabel(wxT("Name:"));

			l3->SetLabel(wxT("Texture Name:"));

			l4->SetLabel(wxT("Thickness:"));

			l5->SetLabel(wxT("X1:"));

			l6->SetLabel(wxT("Z1:"));

			l7->SetLabel(wxT("X2:"));

			l8->SetLabel(wxT("Z2:"));

			l9->SetLabel(wxT("Vertical Offset1:"));
			l9->SetToolTip(wxT("Voffset1 and voffset2 are the height in feet above the current floor's altitude"));

			l10->SetLabel(wxT("Vertical Offset2:"));
			l10->SetToolTip(wxT("Voffset1 and voffset2 are the height in feet above the current floor's altitude"));

			l11->SetLabel(wxT("Reverse Axis:"));
			l11->SetToolTip(wxT("If this is false, a difference in voffset will cause the floor to angle from back to front; otherwise it'll angle from left to right."));
			t11->SetValue(wxT("false"));

			l12->SetLabel(wxT("Texture Direction:"));
			l12->SetToolTip(wxT("If this is false, the texture will be mapped horizontally; otherwise it'll be mapped vertically"));
			t12->SetValue(wxT("false"));

			l13->SetLabel(wxT("Texture Tiling Width:"));
			l13->SetToolTip(wxT("tw and th are to size/tile the texture (0 lets the app autosize them), and isexternal determines if the floor is part of the building's external framework, or is part of the current floor (is either True or False)"));
			t13->SetValue(wxT("0"));

			l14->SetLabel(wxT("Texture Tiling Height:"));
			l14->SetToolTip(wxT("tw and th are to size/tile the texture (0 lets the app autosize them), and isexternal determines if the floor is part of the building's external framework, or is part of the current floor (is either True or False)"));
			t14->SetValue(wxT("0"));
		}

		if (objecttype == wxT("AddInterfloorWall") && objectparent == wxT("Floor"))
		{
			//t1->SetValue(wxVariant((int)Simcore->Floors).GetString());

			SetTitle(wxT("Floor - AddInterfloorWall"));

			wxString newlabel = wxT("Adds a textured wall with the specified dimensions below the current floor/level\n");
			newlabel = newlabel + wxT("Items with an asterisk (*) are optional.\nMove mouse over parameter name for help");

			tDesc->SetLabel(newlabel);
			tVariables->SetLabel(wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n"));

			l1->SetLabel(wxT("Floor Number:"));
			l1->SetToolTip(wxT("Number of floor (such as 1), or a range of floors (such as 5 to 10)"));

			l2->SetLabel(wxT("Name:"));

			l3->SetLabel(wxT("Texture Name:"));

			l4->SetLabel(wxT("Thickness:"));

			l5->SetLabel(wxT("X1:"));

			l6->SetLabel(wxT("Z1:"));

			l7->SetLabel(wxT("X2:"));

			l8->SetLabel(wxT("Z2:"));

			l9->SetLabel(wxT("Height 1:"));
			l9->SetToolTip(wxT("Height1 is the wall height in feet at the first coordinate set (x1 and z1), and height2 is for the second set (x2, and z2)"));

			l10->SetLabel(wxT("Height 2:"));
			l10->SetToolTip(wxT("Height1 is the wall height in feet at the first coordinate set (x1 and z1), and height2 is for the second set (x2, and z2)"));

			l11->SetLabel(wxT("Vertical Offset1:"));
			l11->SetToolTip(wxT("Voffset1 is the vertical offset in feet (from the floor's altitude) for the first coordinate set, and voffset2 is for the second set"));

			l12->SetLabel(wxT("Vertical Offset2:"));
			l12->SetToolTip(wxT("Voffset1 is the vertical offset in feet (from the floor's altitude) for the first coordinate set, and voffset2 is for the second set"));

			l13->SetLabel(wxT("Texture Tiling Width:"));
			l13->SetToolTip(wxT("tw and th are to size/tile the texture (0 lets the app autosize them), and isexternal determines if the floor is part of the building's external framework, or is part of the current floor (is either True or False)"));
			t13->SetValue(wxT("0"));

			l14->SetLabel(wxT("Texture Tiling Height:"));
			l14->SetToolTip(wxT("tw and th are to size/tile the texture (0 lets the app autosize them), and isexternal determines if the floor is part of the building's external framework, or is part of the current floor (is either True or False)"));
			t14->SetValue(wxT("0"));
		}

		if (objecttype == wxT("AddStairsFloor") && objectparent == wxT("Floor"))
		{
			//t1->SetValue(wxVariant((int)Simcore->Floors).GetString());

			SetTitle(wxT("Floor - AddStairsFloor"));

			wxString newlabel = wxT("Adds a textured floor to the specified stairwell, on the current floor\n");
			newlabel = newlabel + wxT("Items with an asterisk (*) are optional.\nMove mouse over parameter name for help");

			tDesc->SetLabel(newlabel);
			tVariables->SetLabel(wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n"));

			l1->SetLabel(wxT("Floor Number:"));
			l1->SetToolTip(wxT("Number of floor (such as 1), or a range of floors (such as 5 to 10)"));

			l2->SetLabel(wxT("Stairwell Number:"));

			l3->SetLabel(wxT("Name:"));

			l4->SetLabel(wxT("Texture Name:"));

			l5->SetLabel(wxT("Thickness:"));

			l6->SetLabel(wxT("X1:"));

			l7->SetLabel(wxT("Z1:"));

			l8->SetLabel(wxT("X2:"));

			l9->SetLabel(wxT("Z2:"));

			l10->SetLabel(wxT("Vertical Offset1:"));
			l10->SetToolTip(wxT("Voffset1 and voffset2 are the height in feet above the current floor's altitude"));

			l11->SetLabel(wxT("Vertical Offset2:"));
			l11->SetToolTip(wxT("Voffset1 and voffset2 are the height in feet above the current floor's altitude"));

			l11->SetLabel(wxT("Reverse Axis:"));
			l11->SetToolTip(wxT("If this is false, a difference in voffset will cause the floor to angle from back to front; otherwise it'll angle from left to right."));
			t11->SetValue(wxT("false"));

			l12->SetLabel(wxT("Texture Direction:"));
			l12->SetToolTip(wxT("If this is false, the texture will be mapped horizontally; otherwise it'll be mapped vertically"));
			t12->SetValue(wxT("false"));

			l13->SetLabel(wxT("Texture Tiling Width:"));
			l13->SetToolTip(wxT("tw and th are to size/tile the texture (0 lets the app autosize them), and isexternal determines if the floor is part of the building's external framework, or is part of the current floor (is either True or False)"));
			t13->SetValue(wxT("0"));

			l14->SetLabel(wxT("Texture Tiling Height:"));
			l14->SetToolTip(wxT("tw and th are to size/tile the texture (0 lets the app autosize them), and isexternal determines if the floor is part of the building's external framework, or is part of the current floor (is either True or False)"));
			t14->SetValue(wxT("0"));
		}

		if (objecttype == wxT("AddStairsWall") && objectparent == wxT("Floor"))
		{
			//t1->SetValue(wxVariant((int)Simcore->Floors).GetString());

			SetTitle(wxT("Floor - AddStairsWall"));

			wxString newlabel = wxT("Adds a textured wall to the specified stairwell, on the current floor\n");
			newlabel = newlabel + wxT("Items with an asterisk (*) are optional.\nMove mouse over parameter name for help");

			tDesc->SetLabel(newlabel);
			tVariables->SetLabel(wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n"));

			l1->SetLabel(wxT("Floor Number:"));
			l1->SetToolTip(wxT("Number of floor (such as 1), or a range of floors (such as 5 to 10)"));

			l2->SetLabel(wxT("Stairwell Number:"));

			l3->SetLabel(wxT("Name:"));

			l4->SetLabel(wxT("Texture Name:"));

			l5->SetLabel(wxT("Thickness:"));

			l6->SetLabel(wxT("X1:"));

			l7->SetLabel(wxT("Z1:"));

			l8->SetLabel(wxT("X2:"));

			l9->SetLabel(wxT("Z2:"));

			l10->SetLabel(wxT("Height 1:"));
			l10->SetToolTip(wxT("Height1 is the wall height in feet at the first coordinate set (x1 and z1), and height2 is for the second set (x2, and z2)"));

			l11->SetLabel(wxT("Height 2:"));
			l11->SetToolTip(wxT("Height1 is the wall height in feet at the first coordinate set (x1 and z1), and height2 is for the second set (x2, and z2)"));

			l12->SetLabel(wxT("Vertical Offset1:"));
			l12->SetToolTip(wxT("Voffset1 is the vertical offset in feet (from the floor's altitude) for the first coordinate set, and voffset2 is for the second set"));

			l13->SetLabel(wxT("Vertical Offset2:"));
			l13->SetToolTip(wxT("Voffset1 is the vertical offset in feet (from the floor's altitude) for the first coordinate set, and voffset2 is for the second set"));

			l14->SetLabel(wxT("Texture Tiling Width:"));
			l14->SetToolTip(wxT("tw and th are to size/tile the texture (0 lets the app autosize them), and isexternal determines if the floor is part of the building's external framework, or is part of the current floor (is either True or False)"));
			t14->SetValue(wxT("0"));

			l15->SetLabel(wxT("Texture Tiling Height:"));
			l15->SetToolTip(wxT("tw and th are to size/tile the texture (0 lets the app autosize them), and isexternal determines if the floor is part of the building's external framework, or is part of the current floor (is either True or False)"));
			t15->SetValue(wxT("0"));
		}

		if (objecttype == wxT("AddShaftFloor") && objectparent == wxT("Floor"))
		{
			//t1->SetValue(wxVariant((int)Simcore->Floors).GetString());

			SetTitle(wxT("Floor - AddShaftFloor"));

			wxString newlabel = wxT("Adds a textured floor to the specified shaft, on the current floor\n");
			newlabel = newlabel + wxT("Items with an asterisk (*) are optional.\nMove mouse over parameter name for help");

			tDesc->SetLabel(newlabel);
			tVariables->SetLabel(wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n"));

			l1->SetLabel(wxT("Floor Number:"));
			l1->SetToolTip(wxT("Number of floor (such as 1), or a range of floors (such as 5 to 10)"));

			l2->SetLabel(wxT("Shaft Number:"));

			l3->SetLabel(wxT("Name:"));

			l4->SetLabel(wxT("Texture Name:"));

			l5->SetLabel(wxT("Thickness:"));

			l6->SetLabel(wxT("X1:"));

			l7->SetLabel(wxT("Z1:"));

			l8->SetLabel(wxT("X2:"));

			l9->SetLabel(wxT("Z2:"));

			l10->SetLabel(wxT("Vertical Offset1:"));
			l10->SetToolTip(wxT("Voffset1 and voffset2 are the height in feet above the current floor's altitude"));

			l11->SetLabel(wxT("Vertical Offset2:"));
			l11->SetToolTip(wxT("Voffset1 and voffset2 are the height in feet above the current floor's altitude"));

			l11->SetLabel(wxT("Reverse Axis:"));
			l11->SetToolTip(wxT("If this is false, a difference in voffset will cause the floor to angle from back to front; otherwise it'll angle from left to right."));
			t11->SetValue(wxT("false"));

			l12->SetLabel(wxT("Texture Direction:"));
			l12->SetToolTip(wxT("If this is false, the texture will be mapped horizontally; otherwise it'll be mapped vertically"));
			t12->SetValue(wxT("false"));

			l13->SetLabel(wxT("Texture Tiling Width:"));
			l13->SetToolTip(wxT("tw and th are to size/tile the texture (0 lets the app autosize them), and isexternal determines if the floor is part of the building's external framework, or is part of the current floor (is either True or False)"));
			t13->SetValue(wxT("0"));

			l14->SetLabel(wxT("Texture Tiling Height:"));
			l14->SetToolTip(wxT("tw and th are to size/tile the texture (0 lets the app autosize them), and isexternal determines if the floor is part of the building's external framework, or is part of the current floor (is either True or False)"));
			t14->SetValue(wxT("0"));
		}

		if (objecttype == wxT("AddShaftWall") && objectparent == wxT("Floor"))
		{
			//t1->SetValue(wxVariant((int)Simcore->Floors).GetString());

			SetTitle(wxT("Floor - AddShaftWall"));

			wxString newlabel = wxT("Adds a textured wall to the specified shaft, on the current floor\n");
			newlabel = newlabel + wxT("Items with an asterisk (*) are optional.\nMove mouse over parameter name for help");

			tDesc->SetLabel(newlabel);
			tVariables->SetLabel(wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n"));

			l1->SetLabel(wxT("Floor Number:"));
			l1->SetToolTip(wxT("Number of floor (such as 1), or a range of floors (such as 5 to 10)"));

			l2->SetLabel(wxT("Shaft Number:"));

			l3->SetLabel(wxT("Name:"));

			l4->SetLabel(wxT("Texture Name:"));

			l5->SetLabel(wxT("Thickness:"));

			l6->SetLabel(wxT("X1:"));

			l7->SetLabel(wxT("Z1:"));

			l8->SetLabel(wxT("X2:"));

			l9->SetLabel(wxT("Z2:"));

			l10->SetLabel(wxT("Height 1:"));
			l10->SetToolTip(wxT("Height1 is the wall height in feet at the first coordinate set (x1 and z1), and height2 is for the second set (x2, and z2)"));

			l11->SetLabel(wxT("Height 2:"));
			l11->SetToolTip(wxT("Height1 is the wall height in feet at the first coordinate set (x1 and z1), and height2 is for the second set (x2, and z2)"));

			l12->SetLabel(wxT("Vertical Offset1:"));
			l12->SetToolTip(wxT("Voffset1 is the vertical offset in feet (from the floor's altitude) for the first coordinate set, and voffset2 is for the second set"));

			l13->SetLabel(wxT("Vertical Offset2:"));
			l13->SetToolTip(wxT("Voffset1 is the vertical offset in feet (from the floor's altitude) for the first coordinate set, and voffset2 is for the second set"));

			l14->SetLabel(wxT("Texture Tiling Width:"));
			l14->SetToolTip(wxT("tw and th are to size/tile the texture (0 lets the app autosize them), and isexternal determines if the floor is part of the building's external framework, or is part of the current floor (is either True or False)"));
			t14->SetValue(wxT("0"));

			l15->SetLabel(wxT("Texture Tiling Height:"));
			l15->SetToolTip(wxT("tw and th are to size/tile the texture (0 lets the app autosize them), and isexternal determines if the floor is part of the building's external framework, or is part of the current floor (is either True or False)"));
			t15->SetValue(wxT("0"));
		}

		if (objecttype == wxT("ColumnWallBox") && objectparent == wxT("Floor"))
		{
			//t1->SetValue(wxVariant((int)Simcore->Floors).GetString());

			SetTitle(wxT("Floor - ColumnWallBox"));

			wxString newlabel = wxT("creates 4 walls (box) at the specified coordinate locations, as part of the current floor's columnframe mesh\n");
			newlabel = newlabel + wxT("Items with an asterisk (*) are optional.\nMove mouse over parameter name for help");

			tDesc->SetLabel(newlabel);
			tVariables->SetLabel(wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n"));

			l1->SetLabel(wxT("Floor Number:"));
			l1->SetToolTip(wxT("Number of floor (such as 1), or a range of floors (such as 5 to 10)"));

			l2->SetLabel(wxT("Name:"));

			l3->SetLabel(wxT("Texture Name:"));

			l4->SetLabel(wxT("X1:"));

			l5->SetLabel(wxT("X2:"));

			l6->SetLabel(wxT("Z1:"));

			l7->SetLabel(wxT("Z2:"));

			l8->SetLabel(wxT("Height:"));

			l9->SetLabel(wxT("Vertical Offset:"));
			l9->SetToolTip(wxT("Voffset is the starting height of the object"));

			l10->SetLabel(wxT("Texture Tiling Width:"));
			l10->SetToolTip(wxT("tw and th are to size/tile the texture (0 lets the app autosize them), and isexternal determines if the floor is part of the building's external framework, or is part of the current floor (is either True or False)"));
			t10->SetValue(wxT("0"));

			l11->SetLabel(wxT("Texture Tiling Height:"));
			l11->SetToolTip(wxT("tw and th are to size/tile the texture (0 lets the app autosize them), and isexternal determines if the floor is part of the building's external framework, or is part of the current floor (is either True or False)"));
			t11->SetValue(wxT("0"));

			l12->SetLabel(wxT("Show Inside:"));
			l12->SetLabel(wxT("Show Outside:"));
			l12->SetLabel(wxT("Show Top:"));
			l12->SetLabel(wxT("Show Bottom:"));
		}

		if (objecttype == wxT("ColumnWallBox2") && objectparent == wxT("Floor"))
		{
			//t1->SetValue(wxVariant((int)Simcore->Floors).GetString());

			SetTitle(wxT("Floor - ColumnWallBox2"));

			wxString newlabel = wxT("creates 4 walls (box) at a specified central location, as part of the current floor's columnframe mesh \n");
			newlabel = newlabel + wxT("Items with an asterisk (*) are optional.\nMove mouse over parameter name for help");

			tDesc->SetLabel(newlabel);
			tVariables->SetLabel(wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n"));

			l1->SetLabel(wxT("Floor Number:"));
			l1->SetToolTip(wxT("Number of floor (such as 1), or a range of floors (such as 5 to 10)"));

			l2->SetLabel(wxT("Name:"));

			l3->SetLabel(wxT("Texture Name:"));

			l4->SetLabel(wxT("CenterX:"));

			l5->SetLabel(wxT("CenterZ:"));

			l6->SetLabel(wxT("WidthX:"));

			l7->SetLabel(wxT("LengthZ:"));

			l8->SetLabel(wxT("Height:"));

			l9->SetLabel(wxT("Vertical Offset:"));
			l9->SetToolTip(wxT("Voffset is the starting height of the object"));

			l10->SetLabel(wxT("Texture Tiling Width:"));
			l10->SetToolTip(wxT("tw and th are to size/tile the texture (0 lets the app autosize them), and isexternal determines if the floor is part of the building's external framework, or is part of the current floor (is either True or False)"));
			t10->SetValue(wxT("0"));

			l11->SetLabel(wxT("Texture Tiling Height:"));
			l11->SetToolTip(wxT("tw and th are to size/tile the texture (0 lets the app autosize them), and isexternal determines if the floor is part of the building's external framework, or is part of the current floor (is either True or False)"));
			t11->SetValue(wxT("0"));

			l12->SetLabel(wxT("Show Inside:"));
			l12->SetLabel(wxT("Show Outside:"));
			l12->SetLabel(wxT("Show Top:"));
			l12->SetLabel(wxT("Show Bottom:"));
		}

		if (objecttype == wxT("CreateCallButtons") && objectparent == wxT("Floor"))
		{
			//t1->SetValue(wxVariant((int)Simcore->Floors).GetString());

			SetTitle(wxT("Floor - CreateCallButtons"));

			wxString newlabel = wxT("creates a call button set\n");
			newlabel = newlabel + wxT("Items with an asterisk (*) are optional.\nMove mouse over parameter name for help");

			tDesc->SetLabel(newlabel);
			tVariables->SetLabel(wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n"));

			l1->SetLabel(wxT("Floor Number:"));
			l1->SetToolTip(wxT("Number of floor (such as 1), or a range of floors (such as 5 to 10)"));

			l2->SetLabel(wxT("Elevator List"));
			l2->SetToolTip(wxT("comma-separated list of elevators the next created call button set will work with (this must be specified before CreateCallButtons)\nExample: 1, 2, 3, 4"));

			l3->SetLabel(wxT("Back Texture:"));
			l3->SetToolTip(wxT("texture of the wall plate behind the buttons"));

			l4->SetLabel(wxT("Up Button Texture:"));
			l4->SetToolTip(wxT("UpButtonTexture and DownButtonTexture are the textures used for the buttons themselves (unlit).\nUpButtonTexture_Lit and DownButtonTexture_Lit specify the lit button textures."));
			l5->SetLabel(wxT("Up Button Texture (lit):"));
			l5->SetToolTip(wxT("UpButtonTexture and DownButtonTexture are the textures used for the buttons themselves (unlit).\nUpButtonTexture_Lit and DownButtonTexture_Lit specify the lit button textures."));
			l6->SetLabel(wxT("Down Button Texture:"));
			l6->SetToolTip(wxT("UpButtonTexture and DownButtonTexture are the textures used for the buttons themselves (unlit).\nUpButtonTexture_Lit and DownButtonTexture_Lit specify the lit button textures."));
			l7->SetLabel(wxT("Down Button Texture (lit):"));
			l7->SetToolTip(wxT("UpButtonTexture and DownButtonTexture are the textures used for the buttons themselves (unlit).\nUpButtonTexture_Lit and DownButtonTexture_Lit specify the lit button textures."));

			l8->SetLabel(wxT("CenterX:"));
			l8->SetToolTip(wxT("the central location of the call button set object"));

			l9->SetLabel(wxT("CenterZ:"));
			l9->SetToolTip(wxT("the central location of the call button set object"));

			l10->SetLabel(wxT("Vertical Offset:"));
			l10->SetToolTip(wxT("altitude offset that the object is above each floor"));

			l11->SetLabel(wxT("Direction:"));
			l11->SetToolTip(wxT("direction determines the direction the call buttons face:\n'front' means they face towards the front of the building\n'back' means they face towards the back of the building\n'left' means they face left\n'right' means they face right"));

			l12->SetLabel(wxT("Back Width:"));
			l12->SetToolTip(wxT("BackWidth and BackHeight are the width and height of the wall plate"));

			l13->SetLabel(wxT("Back Height:"));
			l13->SetToolTip(wxT("BackWidth and BackHeight are the width and height of the wall plate"));

			l14->SetLabel(wxT("Show Back:"));
			l14->SetToolTip(wxT("ShowBack determines if the wall plate should be shown, and is either true or false"));

			l15->SetLabel(wxT("Texture Tiling Width:"));
			l15->SetToolTip(wxT("tw and th are the texture scaling for the wall plate."));
			t15->SetValue(wxT("0"));

			l16->SetLabel(wxT("Texture Tiling Height:"));
			l16->SetToolTip(wxT("tw and th are the texture scaling for the wall plate."));
			t16->SetValue(wxT("0"));
		}

		if (objecttype == wxT("AddStairs") && objectparent == wxT("Floor"))
		{
			//t1->SetValue(wxVariant((int)Simcore->Floors).GetString());

			SetTitle(wxT("Floor - AddStairs"));

			wxString newlabel = wxT("creates a custom staircase at the specified location.\nTo determine the height of the staircase, multiply num_stairs with risersize.\n");
			newlabel = newlabel + wxT("Items with an asterisk (*) are optional.\nMove mouse over parameter name for help");

			tDesc->SetLabel(newlabel);
			tVariables->SetLabel(wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n"));

			l1->SetLabel(wxT("Floor Number:"));
			l1->SetToolTip(wxT("Number of floor (such as 1), or a range of floors (such as 5 to 10)"));

			l2->SetLabel(wxT("Stairwell Number:"));

			l3->SetLabel(wxT("Name:"));

			l4->SetLabel(wxT("Texture:"));

			l5->SetLabel(wxT("Direction:"));
			l5->SetToolTip(wxT("The direction parameter specifies the direction the staircase faces (where the bottom step is), and so if a staircase goes up from left to right, the direction would be left."));

			l6->SetLabel(wxT("CenterX:"));

			l7->SetLabel(wxT("CenterZ:"));

			l8->SetLabel(wxT("Width:"));
			l8->SetToolTip(wxT("the step width"));

			l9->SetLabel(wxT("Riser Size:"));
			l9->SetToolTip(wxT("the height of each step riser (vertical portion)"));

			l10->SetLabel(wxT("Tread Size:"));
			l10->SetToolTip(wxT("the length of each step tread/run (horizontal portion)"));

			l11->SetLabel(wxT("Number of Stairs:"));
			l11->SetToolTip(wxT("Number of steps - Note that the tread of the top step is not drawn (the top step is the landing platform);\ntherefore for a staircase containing 10 steps, the total staircase width\nwould be comprised of 9 treads (which is why the length calculation used num_stairs minus 1).\n"));

			l12->SetLabel(wxT("Vertical Offset:"));
			l12->SetToolTip(wxT("altitude offset that the object is above each floor"));

			l13->SetLabel(wxT("Texture Tiling Width:"));
			t13->SetValue(wxT("0"));

			l14->SetLabel(wxT("Texture Tiling Height:"));
			t14->SetValue(wxT("0"));
		}

		if (objecttype == wxT("AddDoor") && objectparent == wxT("Floor"))
		{
			//t1->SetValue(wxVariant((int)Simcore->Floors).GetString());

			SetTitle(wxT("Floor - AddDoor"));

			wxString newlabel = wxT("adds a textured door in the specified location, and performs a wall cut on that area\n(this must be called after the associated wall is created)\n");
			newlabel = newlabel + wxT("Items with an asterisk (*) are optional.\nMove mouse over parameter name for help");

			tDesc->SetLabel(newlabel);
			tVariables->SetLabel(wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n"));

			l1->SetLabel(wxT("Floor Number:"));
			l1->SetToolTip(wxT("Number of floor (such as 1), or a range of floors (such as 5 to 10)"));

			l2->SetLabel(wxT("Open Sound*:"));
			l2->SetToolTip(wxT("filename of the door open sound"));

			l3->SetLabel(wxT("Close Sound*:"));
			l3->SetToolTip(wxT("filename of the door close sound"));

			l4->SetLabel(wxT("Open:"));
			l4->SetToolTip(wxT("determines if the door should be opened on start; default is false."));
			t4->SetValue(wxT("false"));

			l5->SetLabel(wxT("Texture:"));

			l6->SetLabel(wxT("Thickness:"));

			l7->SetLabel(wxT("Direction:"));
			l7->SetToolTip(wxT("Direction specifies the direction the door faces (the side in which the handle is on the left) and also the direction it opens. These are the values:\n1 - faces left, opens left\n2 - faces left, opens right\n3 - faces right, opens right\n4 - faces right, opens left\n5 - faces front, opens front\n6 - faces front, opens back\n7 - faces back, opens back\n8 - faces back, opens front"));

			l8->SetLabel(wxT("Speed:"));
			t8->SetValue(wxT("75"));
			l8->SetToolTip(wxT("The default door speed is 75; you can also specify 0 for the speed to use the system default."));

			l9->SetLabel(wxT("CenterX:"));

			l10->SetLabel(wxT("CenterZ:"));

			l11->SetLabel(wxT("Width:"));
			l11->SetToolTip(wxT("the door width"));

			l12->SetLabel(wxT("Height:"));
			l12->SetToolTip(wxT("the door width"));

			l13->SetLabel(wxT("Vertical Offset:"));
			l13->SetToolTip(wxT("altitude offset that the object is above each floor"));

			l14->SetLabel(wxT("Texture Tiling Width:"));
			t14->SetValue(wxT("0"));

			l15->SetLabel(wxT("Texture Tiling Height:"));
			t15->SetValue(wxT("0"));
		}

		if (objecttype == wxT("AddStairsDoor") && objectparent == wxT("Floor"))
		{
			//t1->SetValue(wxVariant((int)Simcore->Floors).GetString());

			SetTitle(wxT("Floor - AddStairsDoor"));

			wxString newlabel = wxT("adds a textured door for the specified stairwell, in a location relative to the stairwell's center.\nThis also performs a wall cut on that area (this must be called after the associated wall is created)\n");
			newlabel = newlabel + wxT("Items with an asterisk (*) are optional.\nMove mouse over parameter name for help");

			tDesc->SetLabel(newlabel);
			tVariables->SetLabel(wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n"));

			l1->SetLabel(wxT("Floor Number:"));
			l1->SetToolTip(wxT("Number of floor (such as 1), or a range of floors (such as 5 to 10)"));

			l2->SetLabel(wxT("Stairwell Number:"));

			l3->SetLabel(wxT("Open Sound*:"));
			l3->SetToolTip(wxT("filename of the door open sound"));

			l4->SetLabel(wxT("Close Sound*:"));
			l4->SetToolTip(wxT("filename of the door close sound"));

			l5->SetLabel(wxT("Open:"));
			l5->SetToolTip(wxT("determines if the door should be opened on start; default is false."));
			t5->SetValue(wxT("false"));

			l6->SetLabel(wxT("Texture:"));

			l7->SetLabel(wxT("Thickness:"));

			l8->SetLabel(wxT("Direction:"));
			l8->SetToolTip(wxT("Direction specifies the direction the door faces (the side in which the handle is on the left) and also the direction it opens. These are the values:\n1 - faces left, opens left\n2 - faces left, opens right\n3 - faces right, opens right\n4 - faces right, opens left\n5 - faces front, opens front\n6 - faces front, opens back\n7 - faces back, opens back\n8 - faces back, opens front"));

			l9->SetLabel(wxT("Speed:"));
			t9->SetValue(wxT("75"));
			l9->SetToolTip(wxT("The default door speed is 75; you can also specify 0 for the speed to use the system default."));

			l10->SetLabel(wxT("CenterX:"));

			l11->SetLabel(wxT("CenterZ:"));

			l12->SetLabel(wxT("Width:"));
			l12->SetToolTip(wxT("the door width"));

			l13->SetLabel(wxT("Height:"));
			l13->SetToolTip(wxT("the door width"));

			l14->SetLabel(wxT("Vertical Offset:"));
			l14->SetToolTip(wxT("altitude offset that the object is above each floor"));

			l15->SetLabel(wxT("Texture Tiling Width:"));
			t15->SetValue(wxT("0"));

			l16->SetLabel(wxT("Texture Tiling Height:"));
			t16->SetValue(wxT("0"));
		}

		if (objecttype == wxT("AddShaftStdDoor") && objectparent == wxT("Floor"))
		{
			//t1->SetValue(wxVariant((int)Simcore->Floors).GetString());

			SetTitle(wxT("Floor - AddShaftStdDoor"));

			wxString newlabel = wxT("adds a textured door for the specified shaft, in a location relative to the stairwell's center.\nThis also performs a wall cut on that area (this must be called after the associated wall is created)\n");
			newlabel = newlabel + wxT("Items with an asterisk (*) are optional.\nMove mouse over parameter name for help");

			tDesc->SetLabel(newlabel);
			tVariables->SetLabel(wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n"));

			l1->SetLabel(wxT("Floor Number:"));
			l1->SetToolTip(wxT("Number of floor (such as 1), or a range of floors (such as 5 to 10)"));

			l2->SetLabel(wxT("Shaft Number:"));

			l3->SetLabel(wxT("Open Sound*:"));
			l3->SetToolTip(wxT("filename of the door open sound"));

			l4->SetLabel(wxT("Close Sound*:"));
			l4->SetToolTip(wxT("filename of the door close sound"));

			l5->SetLabel(wxT("Open:"));
			l5->SetToolTip(wxT("determines if the door should be opened on start; default is false."));
			t5->SetValue(wxT("false"));

			l6->SetLabel(wxT("Texture:"));

			l7->SetLabel(wxT("Thickness:"));

			l8->SetLabel(wxT("Direction:"));
			l8->SetToolTip(wxT("Direction specifies the direction the door faces (the side in which the handle is on the left) and also the direction it opens. These are the values:\n1 - faces left, opens left\n2 - faces left, opens right\n3 - faces right, opens right\n4 - faces right, opens left\n5 - faces front, opens front\n6 - faces front, opens back\n7 - faces back, opens back\n8 - faces back, opens front"));

			l9->SetLabel(wxT("Speed:"));
			t9->SetValue(wxT("75"));
			l9->SetToolTip(wxT("The default door speed is 75; you can also specify 0 for the speed to use the system default."));

			l10->SetLabel(wxT("CenterX:"));

			l11->SetLabel(wxT("CenterZ:"));

			l12->SetLabel(wxT("Width:"));
			l12->SetToolTip(wxT("the door width"));

			l13->SetLabel(wxT("Height:"));
			l13->SetToolTip(wxT("the door width"));

			l14->SetLabel(wxT("Vertical Offset:"));
			l14->SetToolTip(wxT("altitude offset that the object is above each floor"));

			l15->SetLabel(wxT("Texture Tiling Width:"));
			t15->SetValue(wxT("0"));

			l16->SetLabel(wxT("Texture Tiling Height:"));
			t16->SetValue(wxT("0"));
		}

		if (objecttype == wxT("AddShaftDoor") && objectparent == wxT("Floor"))
		{
			//t1->SetValue(wxVariant((int)Simcore->Floors).GetString());

			SetTitle(wxT("Floor - AddShaftDoor"));

			wxString newlabel = wxT("creates working shaft elevator doors on the current floor only.\nThe other command, AddShaftDoors (in the elevator section) creates all shaft doors in a single command.\nThis command is useful for specifying different textures for shaft doors depending on the floor,\nand also for only creating shaft doors on one side if an elevator serves a specific floor.\nThe SetShaftDoors command in the elevator section must be used before using this command.\nParameters such as width, height, and direction are taken from the AddDoors command\n(so the regular elevator doors need to be created first).\nThese doors should be moved slightly away from the elevator doors (to separate them both).\nAlso, this command cuts any shaft walls that are within the door area\n(and so this must be called after the shaft walls are created).\n");
			newlabel = newlabel + wxT("Items with an asterisk (*) are optional.\nMove mouse over parameter name for help");

			tDesc->SetLabel(newlabel);
			tVariables->SetLabel(wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n"));

			l1->SetLabel(wxT("Floor Number:"));
			l1->SetToolTip(wxT("Number of floor (such as 1), or a range of floors (such as 5 to 10)"));

			l2->SetLabel(wxT("Elevator Number:"));

			l3->SetLabel(wxT("Number:"));
			l3->SetToolTip(wxT("specifies the number of the door to create (related to the Doors command) - if the elevator only has one door, or if the Doors command was not used, specify 1 here."));

			l4->SetLabel(wxT("Left Texture:"));
			l4->SetToolTip(wxT("texture used on left door"));

			l5->SetLabel(wxT("Right Texture:"));
			l5->SetToolTip(wxT("texture used on right door"));

			l15->SetLabel(wxT("Texture Tiling Width:"));
			t15->SetValue(wxT("0"));

			l16->SetLabel(wxT("Texture Tiling Height:"));
			t16->SetValue(wxT("0"));
		}

		if (objecttype == wxT("AddFloorIndicator") && objectparent == wxT("Floor"))
		{
			//t1->SetValue(wxVariant((int)Simcore->Floors).GetString());

			SetTitle(wxT("Floor - AddFloorIndicator"));

			wxString newlabel = wxT("creates a floor indicator associated with a specific elevator\nThis command can be given multiple times to create multiple indicators.\n");
			newlabel = newlabel + wxT("Items with an asterisk (*) are optional.\nMove mouse over parameter name for help");

			tDesc->SetLabel(newlabel);
			tVariables->SetLabel(wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n"));

			l1->SetLabel(wxT("Floor Number:"));
			l1->SetToolTip(wxT("Number of floor (such as 1), or a range of floors (such as 5 to 10)"));

			l2->SetLabel(wxT("Elevator Number:"));

			l3->SetLabel(wxT("Relative:"));
			l3->SetToolTip(wxT("determines if the CenterX and CenterZ values are relative of the elevator's center or not"));

			l4->SetLabel(wxT("Texture Prefix:"));
			l4->SetToolTip(wxT("the base name of the texture to load when displaying a floor ID; for example if the indicator is on floor 3, and you specify a prefix of 'Button', it'll load the 'Button3' texture."));

			l5->SetLabel(wxT("Direction:"));
			l5->SetToolTip(wxT("the direction the indicator faces, and can be either left, right, front or back"));

			l6->SetLabel(wxT("CenterX:"));
			l6->SetToolTip(wxT("floor indicator central position"));

			l7->SetLabel(wxT("CenterZ:"));
			l7->SetToolTip(wxT("floor indicator central position"));

			l8->SetLabel(wxT("Width:"));

			l9->SetLabel(wxT("Height:"));

			l10->SetLabel(wxT("Vertical Offset:"));
		}
		if (objecttype == wxT("AddDirectionalIndicator") && objectparent == wxT("Floor"))
		{
			//t1->SetValue(wxVariant((int)Simcore->Floors).GetString());

			SetTitle(wxT("Floor - AddDirectionalIndicator"));

			wxString newlabel = wxT("creates a single elevator directional indicator/lantern on the current floor (similar to the CreateCallButtons command)\n(if ActiveDirection is false, then it'll only create it if the elevator serves that floor).\nIt'll also automatically create the up and down lights depending on the floor\n");
			newlabel = newlabel + wxT("Items with an asterisk (*) are optional.\nMove mouse over parameter name for help");

			tDesc->SetLabel(newlabel);
			tVariables->SetLabel(wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n"));

			l1->SetLabel(wxT("Floor Number:"));
			l1->SetToolTip(wxT("Number of floor (such as 1), or a range of floors (such as 5 to 10)"));

			l2->SetLabel(wxT("Elevator Number:"));
			l2->SetToolTip(wxT("specifies the elevator to create the indicators for."));

			l3->SetLabel(wxT("Relative:"));
			l3->SetToolTip(wxT("determines if the X and Z coordinates are relative to the elevator's origin (center) or not."));

			l4->SetLabel(wxT("Active Direction:"));
			l4->SetToolTip(wxT("determines if the indicator should continuously display the active elevator direction (true), or if it should show the elevator's direction for the current call (false, default)"));
			t4->SetValue(wxT("false"));

			l5->SetLabel(wxT("Single:"));
			l5->SetToolTip(wxT("determines if a single indicator light should be created instead of two. If this is true, the unlit texture is specified in UpLight, and the DownLight value is ignored."));

			l6->SetLabel(wxT("Vertical:"));
			l6->SetToolTip(wxT("determines if the two lights should be vertically separated (true), or horizontally separated (false)"));

			l7->SetLabel(wxT("Back Texture:"));
			l7->SetToolTip(wxT("the texture of the wall plate behind the lights"));

			l8->SetLabel(wxT("Up Texture:"));
			l8->SetToolTip(wxT("UpTexture and DownTexture are the textures used for the lights themselves, and the 'Lit' texures are the ones to show when the light is on. DownTexture is ignored if Single is true."));
			l9->SetLabel(wxT("Up Texture Lit:"));
			l9->SetToolTip(wxT("UpTexture and DownTexture are the textures used for the lights themselves, and the 'Lit' texures are the ones to show when the light is on. DownTexture is ignored if Single is true."));
			l10->SetLabel(wxT("Down Texture:"));
			l10->SetToolTip(wxT("UpTexture and DownTexture are the textures used for the lights themselves, and the 'Lit' texures are the ones to show when the light is on. DownTexture is ignored if Single is true."));
			l11->SetLabel(wxT("Down Texture Lit:"));
			l11->SetToolTip(wxT("UpTexture and DownTexture are the textures used for the lights themselves, and the 'Lit' texures are the ones to show when the light is on. DownTexture is ignored if Single is true."));

			l12->SetLabel(wxT("CenterX:"));
			l12->SetToolTip(wxT("CenterX and CenterZ are the central location of the indicators"));

			l13->SetLabel(wxT("CenterZ:"));
			l13->SetToolTip(wxT("CenterX and CenterZ are the central location of the indicators"));

			l14->SetLabel(wxT("Vertical Offset:"));
			l14->SetToolTip(wxT("altitude offset that the object is above each floor"));

			l15->SetLabel(wxT("Direction:"));
			l15->SetToolTip(wxT("direction determines the direction the indicators face:\n'front' means they face towards the front of the building\n'back' means they face towards the back of the buildin\n'left' means they face left\n'right' means they face right"));

			l16->SetLabel(wxT("Back Width:"));
			l16->SetToolTip(wxT("BackWidth and BackHeight are the width and height of the wall plateh"));

			l17->SetLabel(wxT("Back Height:"));
			l17->SetToolTip(wxT("BackWidth and BackHeight are the width and height of the wall plate"));

			l18->SetLabel(wxT("Show Back:"));
			l18->SetToolTip(wxT("ShowBack determines if the wall plate should be shown, and is either true or false"));

			l19->SetLabel(wxT("Texture Tiling Width:"));
			t19->SetValue(wxT("0"));

			l20->SetLabel(wxT("Texture Tiling Height:"));
			t20->SetValue(wxT("0"));
		}
		if (objecttype == wxT("Cut") && objectparent == wxT("Floor"))
		{
			//t1->SetValue(wxVariant((int)Simcore->Floors).GetString());

			SetTitle(wxT("Floor - Cut"));

			wxString newlabel = wxT("performs a manual box cut on an area within the current floor\n");
			newlabel = newlabel + wxT("Items with an asterisk (*) are optional.\nMove mouse over parameter name for help");

			tDesc->SetLabel(newlabel);
			tVariables->SetLabel(wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n"));

			l1->SetLabel(wxT("Floor Number:"));
			l1->SetToolTip(wxT("Number of floor (such as 1), or a range of floors (such as 5 to 10)"));

			l2->SetLabel(wxT("X1:"));
			l2->SetToolTip(wxT("First X position (left side)"));

			l3->SetLabel(wxT("Y1:"));
			l3->SetToolTip(wxT("First Y position (bottom, relative of floor altitude)"));

			l4->SetLabel(wxT("Z1:"));
			l4->SetToolTip(wxT("First Z position (near side)"));

			l5->SetLabel(wxT("X2:"));
			l5->SetToolTip(wxT("Second X position (right side)"));

			l6->SetLabel(wxT("Y2:"));
			l6->SetToolTip(wxT("Second Y position (top, relative of floor altitude)"));

			l7->SetLabel(wxT("Z2:"));
			l7->SetToolTip(wxT("Second Z position (far side)"));

			l8->SetLabel(wxT("Cut Walls:"));
			l8->SetToolTip(wxT("If cutwalls is true, the function will cut walls"));
			t8->SetValue(wxT("true"));

			l9->SetLabel(wxT("Cut Floors:"));
			l9->SetToolTip(wxT("If cutfloors is true, the function will cut floors"));
			t9->SetValue(wxT("true"));
		}
		if (objecttype == wxT("CutAll") && objectparent == wxT("Floor"))
		{
			//t1->SetValue(wxVariant((int)Simcore->Floors).GetString());

			SetTitle(wxT("Floor - CutAll"));

			wxString newlabel = wxT("performs a manual box cut on all objects associated with the current floor (the level itself, interfloor, shafts, stairs and external)\n");
			newlabel = newlabel + wxT("Items with an asterisk (*) are optional.\nMove mouse over parameter name for help");

			tDesc->SetLabel(newlabel);
			tVariables->SetLabel(wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n"));

			l1->SetLabel(wxT("Floor Number:"));
			l1->SetToolTip(wxT("Number of floor (such as 1), or a range of floors (such as 5 to 10)"));

			l2->SetLabel(wxT("X1:"));
			l2->SetToolTip(wxT("First X position (left side)"));

			l3->SetLabel(wxT("Y1:"));
			l3->SetToolTip(wxT("First Y position (bottom, relative of floor altitude)"));

			l4->SetLabel(wxT("Z1:"));
			l4->SetToolTip(wxT("First Z position (near side)"));

			l5->SetLabel(wxT("X2:"));
			l5->SetToolTip(wxT("Second X position (right side)"));

			l6->SetLabel(wxT("Y2:"));
			l6->SetToolTip(wxT("Second Y position (top, relative of floor altitude)"));

			l7->SetLabel(wxT("Z2:"));
			l7->SetToolTip(wxT("Second Z position (far side)"));

			l8->SetLabel(wxT("Cut Walls:"));
			l8->SetToolTip(wxT("If cutwalls is true, the function will cut walls"));
			t8->SetValue(wxT("true"));

			l9->SetLabel(wxT("Cut Floors:"));
			l9->SetToolTip(wxT("If cutfloors is true, the function will cut floors"));
			t9->SetValue(wxT("true"));
		}
		if (objecttype == wxT("AddFillerWalls") && objectparent == wxT("Floor"))
		{
			//t1->SetValue(wxVariant((int)Simcore->Floors).GetString());

			SetTitle(wxT("Floor - AddFillerWalls"));

			wxString newlabel = wxT("helper function to add fillers around a door's cut location.\nWhen a door is created, the wall in it's location is cut to provide space; after the cut, the sides are open (if the wall has thickness) -\nthis creates a covering wall on each side and a floor above the door frame to fill the area.\n");
			newlabel = newlabel + wxT("Items with an asterisk (*) are optional.\nMove mouse over parameter name for help");

			tDesc->SetLabel(newlabel);
			tVariables->SetLabel(wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n"));

			l1->SetLabel(wxT("Floor Number:"));
			l1->SetToolTip(wxT("Number of floor (such as 1), or a range of floors (such as 5 to 10)"));

			l2->SetLabel(wxT("Texture:"));

			l3->SetLabel(wxT("Thickness:"));

			l4->SetLabel(wxT("CenterX:"));

			l5->SetLabel(wxT("CenterZ:"));

			l6->SetLabel(wxT("Width:"));

			l7->SetLabel(wxT("Height:"));

			l8->SetLabel(wxT("Vertical Offset:"));

			l9->SetLabel(wxT("Direction:"));
			l9->SetToolTip(wxT("Direction is either true if the door faces the front/back (width is along the X axis), or false if the door faces left/right (width is along the Z axis)."));
			t9->SetValue(wxT("true"));

			l10->SetLabel(wxT("Texture Tiling Width:"));
			l10->SetToolTip(wxT(""));
			t10->SetValue(wxT("0"));

			l11->SetLabel(wxT("Texture Tiling Height:"));
			l11->SetToolTip(wxT(""));
			t11->SetValue(wxT("0"));
		}
		if (objecttype == wxT("AddSound") && objectparent == wxT("Floor"))
		{
			//t1->SetValue(wxVariant((int)Simcore->Floors).GetString());

			SetTitle(wxT("Floor - AddSound"));

			wxString newlabel = wxT("creates a user-defined sound at the specified position\n");
			newlabel = newlabel + wxT("Items with an asterisk (*) are optional.\nMove mouse over parameter name for help");

			tDesc->SetLabel(newlabel);
			tVariables->SetLabel(wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n"));

			l1->SetLabel(wxT("Floor Number:"));
			l1->SetToolTip(wxT("Number of floor (such as 1), or a range of floors (such as 5 to 10)"));

			l2->SetLabel(wxT("Name:"));

			l3->SetLabel(wxT("Filename:"));

			l4->SetLabel(wxT("X:"));

			l5->SetLabel(wxT("Y:"));

			l6->SetLabel(wxT("Z:"));

			l7->SetLabel(wxT("Loop:"));
			t7->SetValue(wxT("true"));
			l7->SetToolTip(wxT("Loop specifies if the sound should loop and play on startup"));

			l8->SetLabel(wxT("Volume:"));
			t8->SetValue(wxT("1"));
			l8->SetToolTip(wxT("the volume percentage (with 1.0 being 100%) of the sound"));

			l9->SetLabel(wxT("Speed:"));
			l9->SetToolTip(wxT("the playback speed of the sound in percent"));
			t9->SetValue(wxT("100"));

			l10->SetLabel(wxT("Min Distance:"));
			t10->SetValue(wxT("1"));
			l10->SetToolTip(wxT("min_distance and max_distance set the minimum and maximum distances that the sound can be heard at full volume - by default, minimum is 1 and maximum is -1."));

			l11->SetLabel(wxT("Max Distance:"));
			t11->SetValue(wxT("-1"));
			l11->SetToolTip(wxT("min_distance and max_distance set the minimum and maximum distances that the sound can be heard at full volume - by default, minimum is 1 and maximum is -1."));

			l12->SetLabel(wxT("Doppler Level:"));
			t12->SetValue(wxT("0"));
			l12->SetToolTip(wxT("the doppler scale for the sound (0 means off, the default, 1 is normal, 5 is max)"));

			l13->SetLabel(wxT("Cone Inside Angle:"));
			t13->SetValue(wxT("360"));
			l13->SetToolTip(wxT(" the angle within which the sound is at it's normal volume (default 360)"));

			l14->SetLabel(wxT("Cone Outside Angle:"));
			t14->SetValue(wxT("360"));
			l14->SetToolTip(wxT("the outside angle that the sound is at it's normal volume (default 360, shouldn't be less than the inside angle)"));

			l15->SetLabel(wxT("Cone Outside Volume:"));
			t15->SetValue(wxT("1"));
			l15->SetToolTip(wxT("the volume level of the sound outside (0.0 to 1.0, default 1.0)"));

			l16->SetLabel(wxT("DirectionX:"));
			t16->SetValue(wxT("0"));
			l16->SetToolTip(wxT("Direction of the sound cone"));

			l17->SetLabel(wxT("DirectionY:"));
			t17->SetValue(wxT("0"));
			l17->SetToolTip(wxT("Direction of the sound cone"));

			l18->SetLabel(wxT("DirectionZ:"));
			t18->SetValue(wxT("0"));
			l18->SetToolTip(wxT("Direction of the sound cone"));

		}
		if (objecttype == wxT("AddShaftDoorComponent") && objectparent == wxT("Floor"))
		{
			//t1->SetValue(wxVariant((int)Simcore->Floors).GetString());

			SetTitle(wxT("Floor - AddShaftDoorComponent"));

			wxString newlabel = wxT("creates a single shaft door component, used for creating custom door styles\n");
			newlabel = newlabel + wxT("Items with an asterisk (*) are optional.\nMove mouse over parameter name for help");

			tDesc->SetLabel(newlabel);
			tVariables->SetLabel(wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n"));

			l1->SetLabel(wxT("Floor Number:"));
			l1->SetToolTip(wxT("Number of floor (such as 1), or a range of floors (such as 5 to 10)"));

			l2->SetLabel(wxT("Elevator:"));

			l3->SetLabel(wxT("Number:"));
			l3->SetToolTip(wxT("the number of the door to create (related to the Doors command) - if the elevator only has one door, or if the Doors command was not used, specify 1 here"));

			l4->SetLabel(wxT("Name:"));

			l5->SetLabel(wxT("Texture:"));
			l5->SetToolTip(wxT("the texture used on the main (front and back) sides of the door"));

			l6->SetLabel(wxT("Side Texture:"));
			l6->SetToolTip(wxT("the texture used on the sides (and top/bottom) of the door"));

			l7->SetLabel(wxT("Thickness:"));
			l7->SetToolTip(wxT("Loop specifies if the sound should loop and play on startup"));

			l8->SetLabel(wxT("Direction:"));
			l8->SetToolTip(wxT("Direction is either Up, Down, Left (or Front; is the same), or Right (or Back; is also the same)"));

			l9->SetLabel(wxT("Open Speed:"));

			l10->SetLabel(wxT("Close Speed:"));

			l11->SetLabel(wxT("X1:"));

			l12->SetLabel(wxT("Z1:"));

			l13->SetLabel(wxT("X2:"));

			l14->SetLabel(wxT("Z2:"));

			l15->SetLabel(wxT("Height:"));

			l16->SetLabel(wxT("Vertical Offset:"));
			t16->SetValue(wxT("0"));

			l17->SetLabel(wxT("Texture Tiling Width:"));
			t17->SetValue(wxT("0"));

			l18->SetLabel(wxT("Texture Tiling Height:"));
			t18->SetValue(wxT("0"));

			l19->SetLabel(wxT("Side Texture Tiling Width:"));
			t19->SetValue(wxT("0"));

			l20->SetLabel(wxT("Side Texture Tiling Height:"));
			t20->SetValue(wxT("0"));
		}
		if (objecttype == wxT("FinishShaftDoor") && objectparent == wxT("Floor"))
		{
			//t1->SetValue(wxVariant((int)Simcore->Floors).GetString());

			SetTitle(wxT("Floor - FinishShaftDoor"));

			wxString newlabel = wxT("finishes shaft door creation - use this after all related AddShaftDoorComponent commands are used,\nor specify it without the AddShaftDoorComponent commands if you're creating a manual shaft door.\n");
			newlabel = newlabel + wxT("Items with an asterisk (*) are optional.\nMove mouse over parameter name for help");

			tDesc->SetLabel(newlabel);
			tVariables->SetLabel(wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n"));

			l1->SetLabel(wxT("Floor Number:"));
			l1->SetToolTip(wxT("Number of floor (such as 1), or a range of floors (such as 5 to 10)"));

			l2->SetLabel(wxT("Elevator:"));

			l3->SetLabel(wxT("Number:"));
		}
		if (objecttype == wxT("AddModel") && objectparent == wxT("Floor"))
		{
			//t1->SetValue(wxVariant((int)Simcore->Floors).GetString());

			SetTitle(wxT("Floor - AddModel"));

			wxString newlabel = wxT("adds a 3D model to the floor.\nThe model's textures/materials must be defined in a separate '.material' file, and a separate collider mesh '.collider' will be loaded.\nIf a collider mesh isn't available, a simple box collider will be created.\n");
			newlabel = newlabel + wxT("Items with an asterisk (*) are optional.\nMove mouse over parameter name for help");

			tDesc->SetLabel(newlabel);
			tVariables->SetLabel(wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n"));

			l1->SetLabel(wxT("Floor Number:"));
			l1->SetToolTip(wxT("Number of floor (such as 1), or a range of floors (such as 5 to 10)"));

			l2->SetLabel(wxT("Name:"));

			l3->SetLabel(wxT("Filename:"));

			l4->SetLabel(wxT("AutoCenter:"));
			t4->SetValue(wxT("true"));
			l4->SetToolTip(wxT("either true or false, and determines if the loaded model should be automatically centered, otherwise the exact mesh positioning in the model file will be used"));

			l5->SetLabel(wxT("Center X:"));

			l6->SetLabel(wxT("Center Y:"));
			l6->SetToolTip(wxT("The CenterY value is relative to the current floor's base"));

			l7->SetLabel(wxT("Center Z:"));

			l8->SetLabel(wxT("Rotation X:"));
			t8->SetValue(wxT("0"));

			l9->SetLabel(wxT("Rotation Y:"));
			t9->SetValue(wxT("0"));

			l10->SetLabel(wxT("Rotation Z:"));
			t10->SetValue(wxT("0"));

			l11->SetLabel(wxT("Max Render Distance::"));
			t11->SetValue(wxT("0"));
			l11->SetToolTip(wxT("the maximum distance in feet that the object will be shown (0 means unlimited)"));

			l12->SetLabel(wxT("Scale Multiplier:"));
			t12->SetValue(wxT("1"));
			l12->SetToolTip(wxT("allows you to change the size of the object during the load - for example, set to 2 to double the size"));

			l13->SetLabel(wxT("Enable Physics:"));
			t13->SetValue(wxT("false"));
			l13->SetToolTip(wxT("enables Bullet physics on the object (physics will only work if you don't provide a collider mesh)"));

			l14->SetLabel(wxT("Physics Restitution:"));
			t14->SetValue(wxT("0.1"));

			l15->SetLabel(wxT("Physics Friction:"));
			t15->SetValue(wxT("0.5"));

			l16->SetLabel(wxT("Physics Mass:"));
			t16->SetValue(wxT("0.1"));
		}
		if (objecttype == wxT("AddStairsModel") && objectparent == wxT("Floor"))
		{
			//t1->SetValue(wxVariant((int)Simcore->Floors).GetString());

			SetTitle(wxT("Floor - AddStairsModel"));

			wxString newlabel = wxT("adds a 3D model to the specified stairwell, on the current floor.\nThe model's textures/materials must be defined in a separate '.material' file, and a separate collider mesh '.collider' will be loaded.\nIf a collider mesh isn't available, a simple box collider will be created.\n");
			newlabel = newlabel + wxT("Items with an asterisk (*) are optional.\nMove mouse over parameter name for help");

			tDesc->SetLabel(newlabel);
			tVariables->SetLabel(wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n"));

			l1->SetLabel(wxT("Floor Number:"));
			l1->SetToolTip(wxT("Number of floor (such as 1), or a range of floors (such as 5 to 10)"));

			l2->SetLabel(wxT("Stairwell Number:"));

			l3->SetLabel(wxT("Name:"));

			l4->SetLabel(wxT("Filename:"));

			l5->SetLabel(wxT("AutoCenter:"));
			t5->SetValue(wxT("true"));
			l5->SetToolTip(wxT("either true or false, and determines if the loaded model should be automatically centered, otherwise the exact mesh positioning in the model file will be used"));

			l6->SetLabel(wxT("Center X:"));

			l7->SetLabel(wxT("Center Y:"));
			l7->SetToolTip(wxT("The CenterY value is relative to the current floor's base"));

			l8->SetLabel(wxT("Center Z:"));

			l9->SetLabel(wxT("Rotation X:"));
			t9->SetValue(wxT("0"));

			l10->SetLabel(wxT("Rotation Y:"));
			t10->SetValue(wxT("0"));

			l11->SetLabel(wxT("Rotation Z:"));
			t11->SetValue(wxT("0"));

			l12->SetLabel(wxT("Max Render Distance::"));
			t12->SetValue(wxT("0"));
			l12->SetToolTip(wxT("the maximum distance in feet that the object will be shown (0 means unlimited)"));

			l13->SetLabel(wxT("Scale Multiplier:"));
			t13->SetValue(wxT("1"));
			l13->SetToolTip(wxT("allows you to change the size of the object during the load - for example, set to 2 to double the size"));

			l14->SetLabel(wxT("Enable Physics:"));
			t14->SetValue(wxT("false"));
			l14->SetToolTip(wxT("enables Bullet physics on the object (physics will only work if you don't provide a collider mesh)"));

			l15->SetLabel(wxT("Physics Restitution:"));
			t15->SetValue(wxT("0.1"));

			l16->SetLabel(wxT("Physics Friction:"));
			t16->SetValue(wxT("0.5"));

			l17->SetLabel(wxT("Physics Mass:"));
			t17->SetValue(wxT("0.1"));
		}
		if (objecttype == wxT("AddShaftModel") && objectparent == wxT("Floor"))
		{
			//t1->SetValue(wxVariant((int)Simcore->Floors).GetString());

			SetTitle(wxT("Floor - AddShaftModel"));

			wxString newlabel = wxT("adds a 3D model to the specified shaft, on the current floor.\nThe model's textures/materials must be defined in a separate '.material' file, and a separate collider mesh '.collider' will be loaded.\nIf a collider mesh isn't available, a simple box collider will be created.\n");
			newlabel = newlabel + wxT("Items with an asterisk (*) are optional.\nMove mouse over parameter name for help");

			tDesc->SetLabel(newlabel);
			tVariables->SetLabel(wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n"));

			l1->SetLabel(wxT("Floor Number:"));
			l1->SetToolTip(wxT("Number of floor (such as 1), or a range of floors (such as 5 to 10)"));

			l2->SetLabel(wxT("Shaft Number:"));

			l3->SetLabel(wxT("Name:"));

			l4->SetLabel(wxT("Filename:"));

			l5->SetLabel(wxT("AutoCenter:"));
			t5->SetValue(wxT("true"));
			l5->SetToolTip(wxT("either true or false, and determines if the loaded model should be automatically centered, otherwise the exact mesh positioning in the model file will be used"));

			l6->SetLabel(wxT("Center X:"));

			l7->SetLabel(wxT("Center Y:"));
			l7->SetToolTip(wxT("The CenterY value is relative to the current floor's base"));

			l8->SetLabel(wxT("Center Z:"));

			l9->SetLabel(wxT("Rotation X:"));
			t9->SetValue(wxT("0"));

			l10->SetLabel(wxT("Rotation Y:"));
			t10->SetValue(wxT("0"));

			l11->SetLabel(wxT("Rotation Z:"));
			t11->SetValue(wxT("0"));

			l12->SetLabel(wxT("Max Render Distance::"));
			t12->SetValue(wxT("0"));
			l12->SetToolTip(wxT("the maximum distance in feet that the object will be shown (0 means unlimited)"));

			l13->SetLabel(wxT("Scale Multiplier:"));
			t13->SetValue(wxT("1"));
			l13->SetToolTip(wxT("allows you to change the size of the object during the load - for example, set to 2 to double the size"));

			l14->SetLabel(wxT("Enable Physics:"));
			t14->SetValue(wxT("false"));
			l14->SetToolTip(wxT("enables Bullet physics on the object (physics will only work if you don't provide a collider mesh)"));

			l15->SetLabel(wxT("Physics Restitution:"));
			t15->SetValue(wxT("0.1"));

			l16->SetLabel(wxT("Physics Friction:"));
			t16->SetValue(wxT("0.5"));

			l17->SetLabel(wxT("Physics Mass:"));
			t17->SetValue(wxT("0.1"));
		}

		if (l1->GetLabel() == wxT("1"))
		{
			l1->Hide();
			t1->Hide();
		}
		if (l2->GetLabel() == wxT("1"))
		{
			l2->Hide();
			t2->Hide();
		}
		if (l3->GetLabel() == wxT("1"))
		{
			l3->Hide();
			t3->Hide();
		}
		if (l4->GetLabel() == wxT("1"))
		{
			l4->Hide();
			t4->Hide();
		}
		if (l5->GetLabel() == wxT("1"))
		{
			l5->Hide();
			t5->Hide();
		}
		if (l6->GetLabel() == wxT("1"))
		{
			l6->Hide();
			t6->Hide();
		}
		if (l7->GetLabel() == wxT("1"))
		{
			l7->Hide();
			t7->Hide();
		}
		if (l8->GetLabel() == wxT("1"))
		{
			l8->Hide();
			t8->Hide();
		}
		if (l9->GetLabel() == wxT("1"))
		{
			l9->Hide();
			t9->Hide();
		}
		if (l10->GetLabel() == wxT("1"))
		{
			l10->Hide();
			t10->Hide();
		}
		if (l11->GetLabel() == wxT("1"))
		{
			l11->Hide();
			t11->Hide();
		}
		if (l12->GetLabel() == wxT("1"))
		{
			l12->Hide();
			t12->Hide();
		}
		if (l13->GetLabel() == wxT("1"))
		{
			l13->Hide();
			t13->Hide();
		}
		if (l14->GetLabel() == wxT("1"))
		{
			l14->Hide();
			t14->Hide();
		}
		if (l15->GetLabel() == wxT("1"))
		{
			l15->Hide();
			t15->Hide();
		}
		if (l16->GetLabel() == wxT("1"))
		{
			l16->Hide();
			t16->Hide();
		}
		if (l17->GetLabel() == wxT("1"))
		{
			l17->Hide();
			t17->Hide();
		}
		if (l18->GetLabel() == wxT("1"))
		{
			l18->Hide();
			t18->Hide();
		}
		if (l19->GetLabel() == wxT("1"))
		{
			l19->Hide();
			t19->Hide();
		}
		if (l20->GetLabel() == wxT("1"))
		{
			l20->Hide();
			t20->Hide();
		}

		this->Fit();
		this->CentreOnScreen();
	}
	return true;
}

void ParameterViewer::On_bCancel_Click(wxCommandEvent& event)
{
	this->Hide();
}

void ParameterViewer::On_bOK_Click(wxCommandEvent& event)
{
	Simcore->DeleteColliders = true;
	ScriptProcessor *processor = skyscraper->GetScriptProcessor();
	processor->LoadFromText(command.ToAscii());
	processor->Run();
	Simcore->Prepare(false);
	Simcore->DeleteColliders = false;
	objectinfo->PopulateTree();
	this->Close();
}

void ParameterViewer::ChangeParam(wxCommandEvent& event)
{
	//command constructor

	if (objecttype == wxT("Load") && objectparent == wxT("Texture"))
	{
		command = wxT("<Textures>\n");
		command = command + wxT("Load " + wxString(t1->GetValue()) + wxT(", "));
		command = command + wxString(t2->GetValue()) + wxT(", ");
		command = command + wxString(t3->GetValue()) + wxT(", ");
		command = command + wxString(t4->GetValue()) + wxT(", ");
		command = command + wxString(t5->GetValue()) + wxT("\n");
		command = command + wxT("<EndTextures>\n");

		tCommand->SetValue(command);
		this->Fit();
		this->CentreOnScreen();
		return;
	}

	if (objecttype == wxT("Floor") && objectparent == wxT("Floor"))
	{
		wxString temp = t1->GetValue();
		temp.LowerCase();
		bool multiple = false;
		if (temp.Find(wxT("to")) > 0)
			multiple = true;

		if (multiple == true)
			command = wxT("<Floors ") + wxString(t1->GetValue()) + wxT(">\n");
		else
			command = wxT("<Floor ") + wxString(t1->GetValue()) + wxT(">\n");
		command = command + wxT("Name = ") + wxString(t2->GetValue()) + wxT("\n");
		command = command + wxT("ID = ") + wxString(t3->GetValue()) + wxT("\n");
		command = command + wxT("Type = ") + wxString(t4->GetValue()) + wxT("\n");
		if (t5->IsEmpty() == false)
			command = command + wxT("Description = ") + wxString(t5->GetValue()) + wxT("\n");
		command = command + wxT("Height = ") + wxString(t6->GetValue()) + wxT("\n");
		command = command + wxT("InterfloorHeight = ") + wxString(t7->GetValue()) + wxT("\n");
		if (t8->IsEmpty() == false)
			command = command + wxT("Altitude = ") + wxString(t8->GetValue()) + wxT("\n");
		if (t9->IsEmpty() == false)
			command = command + wxT("Group = ") + wxString(t9->GetValue()) + wxT("\n");

		if (multiple == true)
			command = command + wxT("<EndFloors>\n");
		else
			command = command + wxT("<EndFloor>\n");

		tCommand->SetValue(command);
		this->Fit();
		this->CentreOnScreen();
		return;
	}

	//generic commands

	bool multiple = false;
	wxString temp = t1->GetValue();
	temp.LowerCase();
	if (temp.Find(wxT("to")) > 0)
		multiple = true;

	if (multiple == true && objectparent == wxT("Floor"))
		command = wxT("<Floors ") + wxString(t1->GetValue()) + wxT(">\n");
	else
		command = wxT("<Floor ") + wxString(t1->GetValue()) + wxT(">\n");

	command = command + objecttype + wxT(" ");

	if (objecttype == wxT("CreateCallButtons"))
	{
		command = command + wxT("CallButtonElevators " + wxString(t2->GetValue()) + wxT("\n"));
		if (t3->IsShown() == true)
			command = command + wxString(t3->GetValue());
	}
	else
	{
		if (t2->IsShown() == true)
			command = command + wxString(t2->GetValue());
		if (t3->IsShown() == true)
			command = command + wxT(", ") + wxString(t3->GetValue());
	}
	if (t4->IsShown() == true)
		command = command + wxT(", ") + wxString(t4->GetValue());
	if (t5->IsShown() == true)
		command = command + wxT(", ") + wxString(t5->GetValue());
	if (t6->IsShown() == true)
		command = command + wxT(", ") + wxString(t6->GetValue());
	if (t7->IsShown() == true)
		command = command + wxT(", ") + wxString(t7->GetValue());
	if (t8->IsShown() == true)
		command = command + wxT(", ") + wxString(t8->GetValue());
	if (t9->IsShown() == true)
		command = command + wxT(", ") + wxString(t9->GetValue());
	if (t10->IsShown() == true)
		command = command + wxT(", ") + wxString(t10->GetValue());
	if (t11->IsShown() == true)
		command = command + wxT(", ") + wxString(t11->GetValue());
	if (t12->IsShown() == true)
		command = command + wxT(", ") + wxString(t12->GetValue());
	if (t13->IsShown() == true)
		command = command + wxT(", ") + wxString(t13->GetValue());
	if (t14->IsShown() == true)
		command = command + wxT(", ") + wxString(t14->GetValue());
	if (t15->IsShown() == true)
		command = command + wxT(", ") + wxString(t15->GetValue());
	if (t16->IsShown() == true)
		command = command + wxT(", ") + wxString(t16->GetValue());
	if (t17->IsShown() == true)
		command = command + wxT(", ") + wxString(t17->GetValue());
	if (t18->IsShown() == true)
		command = command + wxT(", ") + wxString(t18->GetValue());
	if (t19->IsShown() == true)
		command = command + wxT(", ") + wxString(t19->GetValue());
	if (t20->IsShown() == true)
		command = command + wxT(", ") + wxString(t20->GetValue());
	command = command + wxT("\n");

	if (multiple == true && objectparent == wxT("Floor"))
		command = command + wxT("<EndFloors>\n");
	else
		command = command + wxT("<EndFloor>\n");

	tCommand->SetValue(command);
	this->Fit();
	this->CentreOnScreen();
}

void ParameterViewer::On_bTextures_Click(wxCommandEvent& event)
{
	//create text window
	TextWindow *twindow = new TextWindow(NULL, -1);
	twindow->SetMinSize(wxSize(300, 200));
	twindow->tMain->SetMinSize(wxSize(300, 200));
	twindow->Fit();
	twindow->Center();
	twindow->SetTitle(wxT("Loaded Textures"));
	twindow->Show(true);
	wxString message = wxString::FromAscii(Simcore->ListTextures().c_str());
	twindow->tMain->WriteText(message);
	twindow->tMain->SetInsertionPoint(0);
}
